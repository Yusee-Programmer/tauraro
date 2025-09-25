#!/usr/bin/env python3
"""
Performance comparison between interpreted and compiled Tauraro execution
"""

import subprocess
import time
import json
import os

def time_execution(command, iterations=10):
    """Time the execution of a command multiple times and return average"""
    times = []
    
    for _ in range(iterations):
        start_time = time.perf_counter()
        try:
            result = subprocess.run(command, shell=True, capture_output=True, text=True, cwd=os.getcwd())
            end_time = time.perf_counter()
            
            if result.returncode == 0:
                times.append(end_time - start_time)
            else:
                print(f"Command failed: {command}")
                print(f"Error: {result.stderr}")
                return None
        except Exception as e:
            print(f"Exception running command: {e}")
            return None
    
    if times:
        return sum(times) / len(times)
    return None

def run_performance_comparison():
    """Run performance comparison between different execution methods"""
    
    # Test files
    test_file = "simple_compiled_benchmark.tr"
    
    print("=== Tauraro Performance Comparison ===")
    print()
    
    # Test 1: Interpreted execution
    print("Testing interpreted execution...")
    interpreted_cmd = f"..\\target\\release\\tauraro.exe run {test_file}"
    interpreted_time = time_execution(interpreted_cmd, iterations=5)
    
    if interpreted_time:
        print(f"Interpreted execution average time: {interpreted_time:.6f} seconds")
    else:
        print("Interpreted execution failed")
        return
    
    # Test 2: Python equivalent for comparison
    print("\\nTesting Python equivalent...")
    python_equivalent = """
print("Starting Simple Compiled Benchmark...")
a = 10
b = 5
result1 = a + b
result2 = a - b
result3 = a * b
result4 = a / b
print("Results:")
print(result1)
print(result2)
print(result3)
print(result4)
print("Simple Compiled Benchmark finished!")
"""
    
    # Write Python equivalent
    with open("python_equivalent.py", "w") as f:
        f.write(python_equivalent)
    
    python_cmd = "python python_equivalent.py"
    python_time = time_execution(python_cmd, iterations=5)
    
    if python_time:
        print(f"Python execution average time: {python_time:.6f} seconds")
    else:
        print("Python execution failed")
        return
    
    # Calculate speedup
    if interpreted_time and python_time:
        speedup = python_time / interpreted_time
        print(f"\\n=== Results ===")
        print(f"Tauraro (interpreted): {interpreted_time:.6f}s")
        print(f"Python: {python_time:.6f}s")
        print(f"Tauraro is {speedup:.2f}x {'faster' if speedup > 1 else 'slower'} than Python")
        
        # Save results
        results = {
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "tauraro_interpreted": interpreted_time,
            "python": python_time,
            "speedup": speedup
        }
        
        with open("performance_results.json", "w") as f:
            json.dump(results, f, indent=2)
        
        print(f"\\nResults saved to performance_results.json")

if __name__ == "__main__":
    run_performance_comparison()