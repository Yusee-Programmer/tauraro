#!/usr/bin/env python3
"""Simple benchmark runner - tests only working features"""
import subprocess
import time
import os

def run_python(filename):
    """Run with Python and time it"""
    start = time.time()
    result = subprocess.run(['python3', filename], capture_output=True, text=True)
    elapsed = time.time() - start
    if result.returncode != 0:
        return None, f"Error: {result.stderr}"
    return elapsed, result.stdout

def compile_and_run_tauraro(filename):
    """Compile with Tauraro and run"""
    base = os.path.basename(filename).replace('.py', '')
    c_file = f"./build/{base}.c"
    exe_file = f"./build/{base}_bin"

    # Transpile
    result = subprocess.run(
        ['/home/user/tauraro/target/release/tauraro', 'compile', filename, '--backend', 'c', '-o', c_file],
        capture_output=True,
        text=True,
        timeout=60
    )
    if result.returncode != 0:
        return None, f"Transpile failed: {result.stderr}"

    # Fix main() return type issue
    subprocess.run(['bash', '/home/user/tauraro/fix_c_main_better.sh', c_file])

    # Compile with gcc
    result = subprocess.run(
        ['gcc', c_file, '-o', exe_file, '-lm', '-O3'],
        capture_output=True,
        text=True,
        timeout=60
    )
    if result.returncode != 0:
        return None, f"GCC failed: {result.stderr}"

    # Run
    start = time.time()
    result = subprocess.run([exe_file], capture_output=True, text=True, timeout=60)
    elapsed = time.time() - start
    if result.returncode != 0:
        return None, f"Runtime error: {result.stderr}"

    return elapsed, result.stdout

def main():
    os.makedirs('build', exist_ok=True)

    benchmarks = [
        '01_fibonacci_simple.py',
        '02_loop_arithmetic.py',
        '03_list_operations.py',
        '04_nested_loops.py',
    ]

    print("=" * 70)
    print("TAURARO SIMPLE BENCHMARKS")
    print("=" * 70)

    results = []

    for bench in benchmarks:
        if not os.path.exists(bench):
            continue

        print(f"\n{bench}:")
        print("-" * 70)

        # Python
        py_time, py_out = run_python(bench)
        if py_time:
            print(f"  Python:  {py_time:.4f}s")
        else:
            print(f"  Python:  FAILED - {py_out}")

        # Tauraro
        tau_time, tau_out = compile_and_run_tauraro(bench)
        if tau_time:
            print(f"  Tauraro: {tau_time:.4f}s")
            if py_time and tau_time:
                speedup = py_time / tau_time
                print(f"  Speedup: {speedup:.2f}x")
                results.append((bench, py_time, tau_time, speedup))
        else:
            print(f"  Tauraro: FAILED - {tau_out}")

    # Summary
    print("\n" + "=" * 70)
    print("SUMMARY")
    print("=" * 70)
    print(f"{'Benchmark':<30} {'Python':<12} {'Tauraro':<12} {'Speedup':<10}")
    print("-" * 70)

    for bench, py_time, tau_time, speedup in results:
        print(f"{bench:<30} {py_time:.4f}s      {tau_time:.4f}s      {speedup:.2f}x")

    if results:
        avg_speedup = sum(r[3] for r in results) / len(results)
        print(f"\nAverage speedup: {avg_speedup:.2f}x")

if __name__ == "__main__":
    main()
