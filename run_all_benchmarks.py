#!/usr/bin/env python3
"""
Master Benchmark Runner
Compiles all benchmark files to C executables and compares with Python
"""

import subprocess
import time
import os
import sys

# List of all benchmark files
BENCHMARKS = [
    "benchmark_01_basic_types.py",
    "benchmark_02_arithmetic.py",
    "benchmark_03_control_flow.py",
    "benchmark_04_functions.py",
    "benchmark_05_classes.py",
    "benchmark_06_lists.py",
    "benchmark_07_algorithms.py",
]

def compile_tauraro_to_exe(py_file):
    """Compile Tauraro file to C and then to executable"""
    base_name = py_file.replace(".py", "")
    exe_name = base_name + ".exe"

    print(f"\n{'='*60}")
    print(f"Compiling {py_file} to C executable...")
    print(f"{'='*60}")

    # Compile using native transpiler
    cmd = [
        "./target/release/tauraro.exe",
        "compile",
        py_file,
        "--use-native-transpiler",
        "--backend", "c",
        "-o", exe_name
    ]

    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=180)
        if result.returncode != 0:
            print(f"❌ Compilation failed for {py_file}")
            print(f"STDOUT: {result.stdout}")
            print(f"STDERR: {result.stderr}")
            return None
        print(f"✅ Successfully compiled {py_file} to {exe_name}")
        return exe_name
    except subprocess.TimeoutExpired:
        print(f"❌ Compilation timed out for {py_file}")
        return None
    except Exception as e:
        print(f"❌ Error compiling {py_file}: {e}")
        return None

def run_python_benchmark(py_file):
    """Run benchmark with Python"""
    print(f"\n📊 Running {py_file} with Python...")

    try:
        start_time = time.time()
        result = subprocess.run(
            ["python", py_file],
            capture_output=True,
            text=True,
            timeout=300
        )
        end_time = time.time()

        elapsed = end_time - start_time

        if result.returncode == 0:
            print(f"✅ Python completed in {elapsed:.4f}s")
            return elapsed, result.stdout
        else:
            print(f"❌ Python execution failed")
            print(f"STDERR: {result.stderr}")
            return None, None
    except subprocess.TimeoutExpired:
        print(f"❌ Python execution timed out (>300s)")
        return None, None
    except Exception as e:
        print(f"❌ Error running Python: {e}")
        return None, None

def run_tauraro_benchmark(exe_file):
    """Run compiled Tauraro executable"""
    print(f"\n⚡ Running {exe_file} (compiled Tauraro)...")

    try:
        start_time = time.time()
        result = subprocess.run(
            [f"./{exe_file}"],
            capture_output=True,
            text=True,
            timeout=300
        )
        end_time = time.time()

        elapsed = end_time - start_time

        if result.returncode == 0:
            print(f"✅ Tauraro completed in {elapsed:.4f}s")
            return elapsed, result.stdout
        else:
            print(f"❌ Tauraro execution failed")
            print(f"STDERR: {result.stderr}")
            return None, None
    except subprocess.TimeoutExpired:
        print(f"❌ Tauraro execution timed out (>300s)")
        return None, None
    except Exception as e:
        print(f"❌ Error running Tauraro: {e}")
        return None, None

def main():
    print("="*60)
    print("TAURARO vs PYTHON COMPREHENSIVE BENCHMARK")
    print("="*60)

    results = []

    for benchmark in BENCHMARKS:
        print(f"\n\n{'#'*60}")
        print(f"# BENCHMARK: {benchmark}")
        print(f"{'#'*60}")

        # Step 1: Compile to executable
        exe_file = compile_tauraro_to_exe(benchmark)
        if not exe_file:
            results.append({
                "name": benchmark,
                "status": "COMPILE_FAILED",
                "python_time": None,
                "tauraro_time": None,
                "speedup": None
            })
            continue

        # Step 2: Run Python benchmark
        py_time, py_output = run_python_benchmark(benchmark)

        # Step 3: Run Tauraro benchmark
        tau_time, tau_output = run_tauraro_benchmark(exe_file)

        # Step 4: Compare results
        if py_time and tau_time:
            speedup = py_time / tau_time
            print(f"\n{'='*60}")
            print(f"📈 RESULTS for {benchmark}:")
            print(f"   Python time:  {py_time:.4f}s")
            print(f"   Tauraro time: {tau_time:.4f}s")
            print(f"   Speedup:      {speedup:.2f}x")
            print(f"{'='*60}")

            # Verify outputs match
            if py_output.strip() == tau_output.strip():
                print("✅ Outputs match!")
            else:
                print("⚠️  Outputs differ!")
                print(f"Python output:\n{py_output}")
                print(f"Tauraro output:\n{tau_output}")

            results.append({
                "name": benchmark,
                "status": "SUCCESS",
                "python_time": py_time,
                "tauraro_time": tau_time,
                "speedup": speedup
            })
        else:
            results.append({
                "name": benchmark,
                "status": "RUN_FAILED",
                "python_time": py_time,
                "tauraro_time": tau_time,
                "speedup": None
            })

    # Print final summary
    print("\n\n")
    print("="*80)
    print("FINAL BENCHMARK SUMMARY")
    print("="*80)
    print(f"{'Benchmark':<35} {'Python (s)':<12} {'Tauraro (s)':<12} {'Speedup':<10} {'Status'}")
    print("-"*80)

    for r in results:
        name = r["name"][:34]
        if r["status"] == "SUCCESS":
            py_t = f"{r['python_time']:.4f}"
            tau_t = f"{r['tauraro_time']:.4f}"
            speedup = f"{r['speedup']:.2f}x"
            status = "✅ PASS"
        else:
            py_t = "N/A"
            tau_t = "N/A"
            speedup = "N/A"
            status = f"❌ {r['status']}"

        print(f"{name:<35} {py_t:<12} {tau_t:<12} {speedup:<10} {status}")

    print("="*80)

    # Calculate average speedup
    successful = [r for r in results if r["status"] == "SUCCESS"]
    if successful:
        avg_speedup = sum(r["speedup"] for r in successful) / len(successful)
        print(f"\n🎯 Average Speedup: {avg_speedup:.2f}x")
        print(f"✅ Successful: {len(successful)}/{len(results)}")
    else:
        print(f"\n❌ No successful benchmarks")

    print("\n")

if __name__ == "__main__":
    main()
