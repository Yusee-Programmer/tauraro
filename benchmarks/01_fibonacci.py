#!/usr/bin/env python3
"""
Benchmark: Recursive Fibonacci
Tests: Function call overhead, recursion depth, stack performance
"""
import time
import sys

def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

def main():
    n = 35 if len(sys.argv) < 2 else int(sys.argv[1])

    start = time.time()
    result = fibonacci(n)
    elapsed = time.time() - start

    print(f"fibonacci({n}) = {result}")
    print(f"Time: {elapsed:.4f} seconds")
    return elapsed

if __name__ == "__main__":
    main()
