#!/usr/bin/env python3
# Benchmark: Functions

def simple_function(x: int) -> int:
    return x + 1

def function_with_params(a: int, b: int, c: int) -> int:
    return a + b * c

def recursive_fibonacci(n: int) -> int:
    if n <= 1:
        return n
    return recursive_fibonacci(n - 1) + recursive_fibonacci(n - 2)

def iterative_fibonacci(n: int) -> int:
    if n <= 1:
        return n
    a: int = 0
    b: int = 1
    i: int = 2
    while i <= n:
        temp: int = a + b
        a = b
        b = temp
        i = i + 1
    return b

def test_function_calls() -> int:
    total: int = 0
    i: int = 0
    while i < 10000000:
        total = total + simple_function(i)
        i = i + 1
    return total

def test_function_with_multiple_params() -> int:
    total: int = 0
    i: int = 0
    while i < 5000000:
        total = total + function_with_params(i, i + 1, i + 2)
        i = i + 1
    return total

def test_recursive() -> int:
    result: int = recursive_fibonacci(30)
    return result

def test_iterative() -> int:
    result: int = iterative_fibonacci(1000000)
    return result

def main():
    print("Testing simple function calls...")
    result1: int = test_function_calls()
    print(result1)

    print("Testing functions with multiple params...")
    result2: int = test_function_with_multiple_params()
    print(result2)

    print("Testing recursive fibonacci(30)...")
    result3: int = test_recursive()
    print(result3)

    print("Testing iterative fibonacci...")
    result4: int = test_iterative()
    print(result4)

    print("All function tests passed!")

main()
