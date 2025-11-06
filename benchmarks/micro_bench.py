# Quick Micro-Benchmark for Python

import time

print("Python Micro-Benchmarks")
print("=" * 40)

# 1. Simple loop
start = time.time()
total = 0
for i in range(10000000):
    total += 1
print(f"Loop (10M): {time.time() - start:.4f}s")

# 2. Arithmetic
start = time.time()
x = 0
for i in range(5000000):
    x = i * 2 + 3
print(f"Arithmetic (5M): {time.time() - start:.4f}s")

# 3. Function calls
def add(a, b):
    return a + b

start = time.time()
result = 0
for i in range(1000000):
    result = add(result, 1)
print(f"Function calls (1M): {time.time() - start:.4f}s")

# 4. List operations
start = time.time()
lst = []
for i in range(100000):
    lst.append(i)
print(f"List append (100K): {time.time() - start:.4f}s")

# 5. Fibonacci
def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

start = time.time()
result = fib(30)
print(f"Fibonacci(30): {time.time() - start:.4f}s (result={result})")

print("=" * 40)
