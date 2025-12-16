#!/usr/bin/env python3
"""
Comprehensive Benchmark Runner
Runs all benchmarks across Tauraro (VM and compiled) and Python
"""
import subprocess
import time
import sys
import os
import statistics
import json
from pathlib import Path

class BenchmarkRunner:
    def __init__(self, tauraro_path, runs=3):
        self.tauraro_path = tauraro_path
        self.runs = runs
        self.results = {}

    def run_command(self, cmd, timeout=120):
        """Run a command and capture output"""
        try:
            start = time.time()
            result = subprocess.run(
                cmd,
                shell=True,
                capture_output=True,
                text=True,
                timeout=timeout
            )
            elapsed = time.time() - start

            if result.returncode != 0:
                return None, f"Error: {result.stderr}"

            return elapsed, result.stdout
        except subprocess.TimeoutExpired:
            return None, "Timeout"
        except Exception as e:
            return None, str(e)

    def run_python_benchmark(self, benchmark_file):
        """Run benchmark with Python"""
        times = []
        for i in range(self.runs):
            elapsed, output = self.run_command(f"python3 {benchmark_file}")
            if elapsed is not None:
                times.append(elapsed)
            else:
                print(f"    Run {i+1}/{self.runs}: Failed - {output}")
                return None

        return {
            "mean": statistics.mean(times),
            "stdev": statistics.stdev(times) if len(times) > 1 else 0,
            "min": min(times),
            "max": max(times),
            "runs": times
        }

    def run_tauraro_vm_benchmark(self, benchmark_file):
        """Run benchmark with Tauraro VM"""
        times = []
        cmd = f"{self.tauraro_path} run {benchmark_file}"

        for i in range(self.runs):
            elapsed, output = self.run_command(cmd, timeout=180)
            if elapsed is not None:
                times.append(elapsed)
            else:
                print(f"    Run {i+1}/{self.runs}: Failed - {output}")
                return None

        return {
            "mean": statistics.mean(times),
            "stdev": statistics.stdev(times) if len(times) > 1 else 0,
            "min": min(times),
            "max": max(times),
            "runs": times
        }

    def run_tauraro_compiled_benchmark(self, benchmark_file):
        """Run benchmark with Tauraro compiled to C"""
        times = []

        # Generate C code
        base_name = Path(benchmark_file).stem
        c_file = f"./build/{base_name}.c"
        output_exe = f"./build/{base_name}_bin"
        transpile_cmd = f"{self.tauraro_path} compile {benchmark_file} --backend c -o {c_file}"

        print(f"    Transpiling to C...")
        transpile_start = time.time()
        transpile_result = subprocess.run(
            transpile_cmd,
            shell=True,
            capture_output=True,
            text=True,
            timeout=240
        )
        transpile_time = time.time() - transpile_start

        if transpile_result.returncode != 0:
            print(f"    Transpilation failed: {transpile_result.stderr}")
            return None

        # Compile C code with gcc
        print(f"    Compiling C to native...")
        compile_cmd = f"gcc {c_file} -o {output_exe} -lm -O3"
        compile_start = time.time()
        compile_result = subprocess.run(
            compile_cmd,
            shell=True,
            capture_output=True,
            text=True,
            timeout=120
        )
        compile_time = time.time() - compile_start

        if compile_result.returncode != 0:
            print(f"    C compilation failed: {compile_result.stderr}")
            return None

        total_compile_time = transpile_time + compile_time

        # Run the compiled executable
        for i in range(self.runs):
            elapsed, output = self.run_command(output_exe, timeout=180)
            if elapsed is not None:
                times.append(elapsed)
            else:
                print(f"    Run {i+1}/{self.runs}: Failed - {output}")
                return None

        return {
            "mean": statistics.mean(times),
            "stdev": statistics.stdev(times) if len(times) > 1 else 0,
            "min": min(times),
            "max": max(times),
            "runs": times,
            "compile_time": total_compile_time
        }

    def run_benchmark(self, benchmark_name, benchmark_file):
        """Run a single benchmark across all implementations"""
        print(f"\n{'='*60}")
        print(f"Benchmark: {benchmark_name}")
        print(f"{'='*60}")

        results = {}

        # Python
        print(f"  Running with Python ({self.runs} runs)...")
        python_result = self.run_python_benchmark(benchmark_file)
        if python_result:
            results["python"] = python_result
            print(f"    Mean: {python_result['mean']:.4f}s ± {python_result['stdev']:.4f}s")

        # Tauraro Compiled (skip VM)
        print(f"  Running with Tauraro Compiled ({self.runs} runs)...")
        tauraro_compiled_result = self.run_tauraro_compiled_benchmark(benchmark_file)
        if tauraro_compiled_result:
            results["tauraro_compiled"] = tauraro_compiled_result
            print(f"    Compile time: {tauraro_compiled_result['compile_time']:.4f}s")
            print(f"    Mean: {tauraro_compiled_result['mean']:.4f}s ± {tauraro_compiled_result['stdev']:.4f}s")
            if python_result:
                speedup = python_result['mean'] / tauraro_compiled_result['mean']
                print(f"    Speedup vs Python: {speedup:.2f}x")

        return results

    def generate_report(self):
        """Generate comprehensive benchmark report"""
        print(f"\n\n{'='*60}")
        print("BENCHMARK SUMMARY")
        print(f"{'='*60}\n")

        # Summary table
        print(f"{'Benchmark':<30} {'Python':<15} {'Tauraro C':<15} {'Speedup':<10}")
        print("-" * 70)

        for benchmark_name, results in self.results.items():
            python_time = results.get("python", {}).get("mean", float('inf'))
            compiled_time = results.get("tauraro_compiled", {}).get("mean", float('inf'))

            python_str = f"{python_time:.4f}s" if python_time != float('inf') else "FAILED"
            compiled_str = f"{compiled_time:.4f}s" if compiled_time != float('inf') else "FAILED"

            speedup_str = "N/A"
            if python_time != float('inf') and compiled_time != float('inf'):
                speedup = python_time / compiled_time
                speedup_str = f"{speedup:.2f}x"

            print(f"{benchmark_name:<30} {python_str:<15} {compiled_str:<15} {speedup_str:<10}")

        # Speedup analysis
        print(f"\n{'='*60}")
        print("SPEEDUP ANALYSIS")
        print(f"{'='*60}\n")

        compiled_speedups = []
        success_count = 0
        fail_count = 0

        for benchmark_name, results in self.results.items():
            python_time = results.get("python", {}).get("mean")
            compiled_time = results.get("tauraro_compiled", {}).get("mean")

            if python_time and compiled_time:
                compiled_speedup = python_time / compiled_time
                compiled_speedups.append(compiled_speedup)
                success_count += 1
            else:
                fail_count += 1

        # Overall statistics
        print(f"Successful benchmarks: {success_count}/{success_count + fail_count}")

        if compiled_speedups:
            print(f"\nTauraro Compiled Performance:")
            print(f"  Mean speedup vs Python: {statistics.mean(compiled_speedups):.2f}x")
            print(f"  Median speedup vs Python: {statistics.median(compiled_speedups):.2f}x")
            print(f"  Best speedup: {max(compiled_speedups):.2f}x")
            print(f"  Worst speedup: {min(compiled_speedups):.2f}x")
        else:
            print("\nNo successful benchmarks to analyze.")

        # Save to JSON
        output_file = "benchmark_results.json"
        with open(output_file, 'w') as f:
            json.dump(self.results, f, indent=2)
        print(f"\nDetailed results saved to: {output_file}")

    def run_all(self):
        """Run all benchmarks"""
        benchmarks = [
            ("01_fibonacci", "benchmarks/01_fibonacci.py"),
            ("02_prime_sieve", "benchmarks/02_prime_sieve.py"),
            ("03_matrix_multiply", "benchmarks/03_matrix_multiply.py"),
            ("04_string_ops", "benchmarks/04_string_ops.py"),
            ("05_hash_operations", "benchmarks/05_hash_operations.py"),
            ("06_sorting", "benchmarks/06_sorting.py"),
            ("07_file_io", "benchmarks/07_file_io.py"),
            ("08_json_parsing", "benchmarks/08_json_parsing.py"),
            ("09_memory_allocation", "benchmarks/09_memory_allocation.py"),
            ("10_regex", "benchmarks/10_regex.py"),
        ]

        # Create build directory
        os.makedirs("build", exist_ok=True)

        for benchmark_name, benchmark_file in benchmarks:
            if not os.path.exists(benchmark_file):
                print(f"\nSkipping {benchmark_name}: File not found")
                continue

            results = self.run_benchmark(benchmark_name, benchmark_file)
            self.results[benchmark_name] = results

        self.generate_report()


def main():
    print("Tauraro Comprehensive Benchmark Suite")
    print("=" * 60)

    # Check for Tauraro executable
    tauraro_path = "./target/release/tauraro"
    if not os.path.exists(tauraro_path):
        print(f"Error: Tauraro executable not found at {tauraro_path}")
        print("Please build Tauraro first: cargo build --release")
        sys.exit(1)

    # Get number of runs
    runs = 3 if len(sys.argv) < 2 else int(sys.argv[1])
    print(f"Running each benchmark {runs} times\n")

    runner = BenchmarkRunner(tauraro_path, runs=runs)
    runner.run_all()


if __name__ == "__main__":
    main()
