#!/usr/bin/env python3
"""
Benchmark Runner Script
Executes all Tauraro and Python benchmarks and compares performance
"""

import os
import sys
import time
import subprocess
import json
from pathlib import Path

class BenchmarkRunner:
    def __init__(self):
        self.results = {}
        self.tauraro_exe = Path("./target/debug/tauraro.exe")
        self.tauraro_dir = Path("benchmarks/tauraro")
        self.python_dir = Path("benchmarks/python")
        self.results_dir = Path("benchmarks/results")
        
        # Ensure results directory exists
        self.results_dir.mkdir(exist_ok=True)
    
    def run_tauraro_benchmark(self, benchmark_name):
        """Run a Tauraro benchmark and return execution time."""
        benchmark_file = self.tauraro_dir / f"{benchmark_name}_benchmark.tr"
        
        if not benchmark_file.exists():
            print(f"Warning: Tauraro benchmark file {benchmark_file} not found")
            return None
            
        try:
            start_time = time.time()
            result = subprocess.run(
                [str(self.tauraro_exe), "run", "--optimization", "3", str(benchmark_file)],
                capture_output=True,
                text=True,
                timeout=60
            )
            end_time = time.time()
            
            if result.returncode != 0:
                print(f"Tauraro {benchmark_name} benchmark failed:")
                print(f"Error: {result.stderr}")
                return None
                
            execution_time = end_time - start_time
            print(f"Tauraro {benchmark_name}: {execution_time:.4f}s")
            return execution_time
            
        except subprocess.TimeoutExpired:
            print(f"Tauraro {benchmark_name} benchmark timed out")
            return None
        except Exception as e:
            print(f"Error running Tauraro {benchmark_name} benchmark: {e}")
            return None

    def run_python_benchmark(self, benchmark_name):
        """Run a Python benchmark and return execution time."""
        benchmark_file = self.python_dir / f"{benchmark_name}_benchmark.py"
        
        if not benchmark_file.exists():
            print(f"Warning: Python benchmark file {benchmark_file} not found")
            return None
            
        try:
            start_time = time.time()
            result = subprocess.run(
                [sys.executable, str(benchmark_file)],
                capture_output=True,
                text=True,
                timeout=60
            )
            end_time = time.time()
            
            if result.returncode != 0:
                print(f"Python {benchmark_name} benchmark failed:")
                print(f"Error: {result.stderr}")
                return None
                
            execution_time = end_time - start_time
            print(f"Python {benchmark_name}: {execution_time:.4f}s")
            return execution_time
            
        except subprocess.TimeoutExpired:
            print(f"Python {benchmark_name} benchmark timed out")
            return None
        except Exception as e:
            print(f"Error running Python {benchmark_name} benchmark: {e}")
            return None
    
    def run_benchmark_suite(self, benchmark_name):
        """Run both Tauraro and Python versions of a benchmark."""
        print(f"\n{'='*50}")
        print(f"Running {benchmark_name.upper()} Benchmark Suite")
        print(f"{'='*50}")
        
        # Run Tauraro benchmark
        tauraro_time = self.run_tauraro_benchmark(benchmark_name)
        
        # Run Python benchmark
        python_time = self.run_python_benchmark(benchmark_name)
        
        # Store results
        self.results[benchmark_name] = {
            "tauraro": tauraro_time,
            "python": python_time
        }
        
        return tauraro_time, python_time
    
    def generate_report(self):
        """Generate a comprehensive performance report."""
        print(f"\n{'='*60}")
        print("BENCHMARK RESULTS SUMMARY")
        print(f"{'='*60}")
        
        total_tauraro_time = 0
        total_python_time = 0
        successful_benchmarks = 0
        
        for benchmark_name, results in self.results.items():
            print(f"\n{benchmark_name.upper()} BENCHMARK:")
            print("-" * 30)
            
            tauraro_time = results.get("tauraro")
            python_time = results.get("python")
            
            if tauraro_time is not None and python_time is not None:
                print(f"Tauraro: {tauraro_time:.4f}s")
                print(f"Python:  {python_time:.4f}s")
                
                if python_time > 0:
                    speedup = python_time / tauraro_time
                    if speedup > 1:
                        print(f"Tauraro is {speedup:.2f}x FASTER than Python")
                    else:
                        print(f"Python is {1/speedup:.2f}x FASTER than Tauraro")
                    
                    total_tauraro_time += tauraro_time
                    total_python_time += python_time
                    successful_benchmarks += 1
            else:
                if tauraro_time is None:
                    print("Tauraro: FAILED")
                else:
                    print(f"Tauraro: {tauraro_time:.4f}s")
                    
                if python_time is None:
                    print("Python: FAILED")
                else:
                    print(f"Python: {python_time:.4f}s")
        
        # Overall performance summary
        if successful_benchmarks > 0:
            print(f"\n{'='*60}")
            print("OVERALL PERFORMANCE SUMMARY")
            print(f"{'='*60}")
            print(f"Total Tauraro Time: {total_tauraro_time:.4f}s")
            print(f"Total Python Time:  {total_python_time:.4f}s")
            
            if total_python_time > 0:
                overall_speedup = total_python_time / total_tauraro_time
                if overall_speedup > 1:
                    print(f"Overall: Tauraro is {overall_speedup:.2f}x FASTER than Python")
                else:
                    print(f"Overall: Python is {1/overall_speedup:.2f}x FASTER than Tauraro")
        
        # Save results to JSON
        self.save_results()

    def save_results(self):
        """Save benchmark results to JSON file."""
        results_file = self.results_dir / "benchmark_results.json"
        
        # Prepare data for JSON serialization
        json_data = {
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "benchmarks": self.results
        }
        
        try:
            with open(results_file, 'w') as f:
                json.dump(json_data, f, indent=2)
            print(f"\nResults saved to: {results_file}")
        except Exception as e:
            print(f"Error saving results: {e}")
    
    def run_all_benchmarks(self):
        """Run all available benchmarks"""
        benchmarks = ["arithmetic", "string", "loop", "function", "sorting", "math"]
        
        print("Starting Tauraro vs Python Benchmark Suite")
        print(f"Running {len(benchmarks)} benchmark categories...")
        
        for benchmark in benchmarks:
            self.run_benchmark_suite(benchmark)
        
        self.generate_report()

def main():
    runner = BenchmarkRunner()
    
    # Check if tauraro.exe exists
    if not runner.tauraro_exe.exists():
        print("Error: tauraro.exe not found in current directory")
        print("Please ensure Tauraro is compiled and available")
        return 1
    
    runner.run_all_benchmarks()
    return 0

if __name__ == "__main__":
    sys.exit(main())