#!/usr/bin/env python3
"""
Quick Comparison Script
Runs a simple benchmark to compare Tauraro and Python performance
"""

import time
import subprocess
import sys
from pathlib import Path

def create_simple_benchmark():
    """Create a simple benchmark file for both languages."""
    
    # Tauraro version
    tauraro_code = """
print("Starting Tauraro benchmark...")

count = 0
for i in range(10000):
    count = count + 1

print("Tauraro benchmark completed. Count:", count)
"""
    
    # Python version
    python_code = """
# Simple loop benchmark
print("Starting Python benchmark...")

count = 0
for i in range(10000):
    count = count + 1

print("Python benchmark completed. Count:", count)
"""
    
    # Write files
    with open("temp_tauraro.tr", "w") as f:
        f.write(tauraro_code)
    
    with open("temp_python.py", "w") as f:
        f.write(python_code)

def run_comparison():
    """Run a quick performance comparison."""
    print("Tauraro vs Python Quick Performance Comparison")
    print("=" * 50)
    
    # Run Python benchmark
    print("\nRunning Python benchmark...")
    start_time = time.time()
    python_result = subprocess.run(
        [sys.executable, "temp_python.py"],
        capture_output=True,
        text=True
    )
    python_time = time.time() - start_time
    
    if python_result.returncode == 0:
        print("Python benchmark completed successfully")
        print(f"Python execution time: {python_time:.4f} seconds")
    else:
        print("Python benchmark failed:")
        print(python_result.stderr)
        return
    
    # Run Tauraro benchmark
    tauraro_exe = Path("../target/release/tauraro.exe")
    if not tauraro_exe.exists():
        print("\n❌ Tauraro not found. Please compile with: cargo build --release")
        return
    
    print("\nRunning Tauraro benchmark...")
    start_time = time.time()
    tauraro_result = subprocess.run(
        [str(tauraro_exe), "run", "temp_tauraro.tr"],
        capture_output=True,
        text=True
    )
    tauraro_time = time.time() - start_time
    
    if tauraro_result.returncode == 0:
        print("Tauraro benchmark completed successfully")
        print(f"Tauraro execution time: {tauraro_time:.4f} seconds")
    else:
        print("Tauraro benchmark failed:")
        print(tauraro_result.stderr)
        return
    
    # Compare results
    print("\n" + "=" * 50)
    print("PERFORMANCE COMPARISON")
    print("=" * 50)
    print(f"Python: {python_time:.4f} seconds")
    print(f"Tauraro: {tauraro_time:.4f} seconds")
    
    if python_time > 0 and tauraro_time > 0:
        if tauraro_time < python_time:
            speedup = python_time / tauraro_time
            print(f"\n✅ Tauraro is {speedup:.2f}x FASTER than Python")
        else:
            slowdown = tauraro_time / python_time
            print(f"\n❌ Tauraro is {slowdown:.2f}x SLOWER than Python")
    
    # Show outputs
    print(f"\nPython output:\n{python_result.stdout}")
    print(f"Tauraro output:\n{tauraro_result.stdout}")

def cleanup():
    """Clean up temporary files."""
    temp_files = ["temp_tauraro.tr", "temp_python.py"]
    for file in temp_files:
        try:
            Path(file).unlink()
        except FileNotFoundError:
            pass

def main():
    try:
        create_simple_benchmark()
        run_comparison()
    finally:
        cleanup()

if __name__ == "__main__":
    main()