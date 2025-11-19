#!/usr/bin/env python3
"""Comprehensive benchmark suite for Tauraro vs Python comparison"""

import time
import sys

def benchmark(name, func, *args, **kwargs):
    """Run a benchmark and return the execution time"""
    start = time.time()
    result = func(*args, **kwargs)
    elapsed = time.time() - start
    print(f"{name}: {elapsed:.4f}s -> {result}")
    return elapsed

# ============================================================================
# CATEGORY 1: RECURSIVE FUNCTIONS (Function Call Overhead)
# ============================================================================

def fib_recursive(n):
    if n <= 1:
        return n
    return fib_recursive(n - 1) + fib_recursive(n - 2)

def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

def ackermann(m, n):
    if m == 0:
        return n + 1
    elif n == 0:
        return ackermann(m - 1, 1)
    else:
        return ackermann(m - 1, ackermann(m, n - 1))

# ============================================================================
# CATEGORY 2: LOOPS & ITERATION (Loop Overhead)
# ============================================================================

def sum_loop(n):
    total = 0
    for i in range(n):
        total = total + i
    return total

def nested_loop(n):
    total = 0
    for i in range(n):
        for j in range(n):
            total = total + 1
    return total

def list_iteration(n):
    lst = list(range(n))
    total = 0
    for item in lst:
        total = total + item
    return total

# ============================================================================
# CATEGORY 3: ARITHMETIC & MATH (Computation Speed)
# ============================================================================

def is_prime(n):
    if n <= 1:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True

def count_primes(n):
    count = 0
    for i in range(n):
        if is_prime(i):
            count = count + 1
    return count

def arithmetic_ops(n):
    result = 0
    for i in range(n):
        result = result + i * 2 - 1
    return result

# ============================================================================
# CATEGORY 4: LIST OPERATIONS
# ============================================================================

def list_append(n):
    lst = []
    for i in range(n):
        lst.append(i)
    return len(lst)

def list_comprehension(n):
    lst = [i * 2 for i in range(n)]
    return len(lst)

def list_sum(n):
    lst = list(range(n))
    return sum(lst)

# ============================================================================
# CATEGORY 5: STRING OPERATIONS
# ============================================================================

def string_concat(n):
    result = ""
    for i in range(n):
        result = result + str(i)
    return len(result)

def string_format(n):
    results = []
    for i in range(n):
        results.append(f"Item {i}")
    return len(results)

# ============================================================================
# CATEGORY 6: DICTIONARY OPERATIONS
# ============================================================================

def dict_operations(n):
    d = {}
    for i in range(n):
        d[str(i)] = i * 2
    return len(d)

def dict_lookup(n):
    d = {str(i): i for i in range(n)}
    total = 0
    for i in range(n):
        total = total + d.get(str(i), 0)
    return total

# ============================================================================
# MAIN BENCHMARK RUNNER
# ============================================================================

def run_benchmarks():
    print("=" * 70)
    print("TAURARO vs PYTHON COMPREHENSIVE BENCHMARK SUITE")
    print("=" * 70)
    
    results = {}
    
    # Category 1: Recursive Functions
    print("\n[CATEGORY 1: RECURSIVE FUNCTIONS]")
    results['fib_20'] = benchmark("Fibonacci(20)", fib_recursive, 20)
    results['factorial_10'] = benchmark("Factorial(10)", factorial, 10)
    # Skip Ackermann(3,3) as it takes too long
    # results['ackermann_3_3'] = benchmark("Ackermann(3,3)", ackermann, 3, 3)
    
    # Category 2: Loops & Iteration
    print("\n[CATEGORY 2: LOOPS & ITERATION]")
    results['sum_loop_1M'] = benchmark("Sum Loop (1M)", sum_loop, 1000000)
    results['nested_loop_1K'] = benchmark("Nested Loop (1Kx1K)", nested_loop, 1000)
    results['list_iter_1M'] = benchmark("List Iteration (1M)", list_iteration, 1000000)
    
    # Category 3: Arithmetic & Math
    print("\n[CATEGORY 3: ARITHMETIC & MATH]")
    results['is_prime_10K'] = benchmark("Count Primes (10K)", count_primes, 10000)
    results['arithmetic_ops_10M'] = benchmark("Arithmetic Ops (10M)", arithmetic_ops, 10000000)
    
    # Category 4: List Operations
    print("\n[CATEGORY 4: LIST OPERATIONS]")
    results['list_append_100K'] = benchmark("List Append (100K)", list_append, 100000)
    results['list_comp_1M'] = benchmark("List Comprehension (1M)", list_comprehension, 1000000)
    results['list_sum_1M'] = benchmark("List Sum (1M)", list_sum, 1000000)
    
    # Category 5: String Operations
    print("\n[CATEGORY 5: STRING OPERATIONS]")
    results['string_concat_10K'] = benchmark("String Concat (10K)", string_concat, 10000)
    results['string_format_100K'] = benchmark("String Format (100K)", string_format, 100000)
    
    # Category 6: Dictionary Operations
    print("\n[CATEGORY 6: DICTIONARY OPERATIONS]")
    results['dict_ops_100K'] = benchmark("Dict Operations (100K)", dict_operations, 100000)
    results['dict_lookup_100K'] = benchmark("Dict Lookup (100K)", dict_lookup, 100000)
    
    # Summary
    print("\n" + "=" * 70)
    print("BENCHMARK SUMMARY")
    print("=" * 70)
    total_time = sum(results.values())
    print(f"Total Time: {total_time:.2f}s")
    print(f"Number of Benchmarks: {len(results)}")
    print(f"Average Time per Benchmark: {total_time/len(results):.4f}s")

if __name__ == "__main__":
    run_benchmarks()
