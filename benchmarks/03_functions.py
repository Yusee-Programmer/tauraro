# Function Call Benchmark

import time

def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

def bench_function_calls():
    """Many function calls"""
    def dummy(x):
        return x + 1

    start = time.time()
    result = 0
    for i in range(500000):
        result = dummy(result)
    end = time.time()
    return end - start

def bench_recursion():
    """Recursive function calls"""
    start = time.time()
    result = fib(25)
    end = time.time()
    return end - start

def bench_factorial():
    """Factorial recursion"""
    start = time.time()
    for i in range(100):
        result = factorial(500)
    end = time.time()
    return end - start

print("=== Function Benchmarks ===")
print(f"Function calls: {bench_function_calls():.4f}s")
print(f"Recursion (fib 25): {bench_recursion():.4f}s")
print(f"Factorial (500): {bench_factorial():.4f}s")
