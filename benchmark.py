# Python Benchmark - Computational Performance Test

import time

# 1. Fibonacci (recursive)
def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

# 2. Sum of numbers
def sum_range(n):
    total = 0
    i = 0
    while i < n:
        total = total + i
        i = i + 1
    return total

# 3. Factorial (iterative)
def factorial(n):
    result = 1
    i = 1
    while i <= n:
        result = result * i
        i = i + 1
    return result

# 4. Prime check
def is_prime(n):
    if n < 2:
        return False
    i = 2
    while i * i <= n:
        if n % i == 0:
            return False
        i = i + 1
    return True

# 5. Count primes up to n
def count_primes(n):
    count = 0
    i = 2
    while i <= n:
        if is_prime(i):
            count = count + 1
        i = i + 1
    return count

# Run benchmarks
print("=== PYTHON BENCHMARK ===")

# Fibonacci
print("Fibonacci(25):")
print(fib(25))

# Sum range
print("Sum(1..10000):")
print(sum_range(10000))

# Factorial
print("Factorial(12):")
print(factorial(12))

# Count primes
print("Primes up to 1000:")
print(count_primes(1000))

print("=== BENCHMARK COMPLETE ===")
