import time

print("PYTHON MICRO-BENCHMARKS - PERFORMANCE ANALYSIS")
print("=" * 60)

print("\nTest 1: Simple Loop (1M iterations)")
start = time.time()
total = 0
for i in range(1000000):
    total = total + 1
t1 = time.time() - start
print(f"Result: {total}, Time: {t1:.4f}s")

print("\nTest 2: Simple Arithmetic (1M ops)")
start = time.time()
result = 0
for i in range(1000000):
    result = result + 1
t2 = time.time() - start
print(f"Result: {result}, Time: {t2:.4f}s")

print("\nTest 3: Function Call (100k calls)")
def dummy():
    return 42

start = time.time()
count = 0
for i in range(100000):
    x = dummy()
    count = count + 1
t3 = time.time() - start
print(f"Result: {count}, Time: {t3:.4f}s")

print("\nTest 4: Fibonacci(15) - Single recursive call")
def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

start = time.time()
result = fib(15)
t4 = time.time() - start
print(f"Result: {result}, Time: {t4:.4f}s")

print("\n" + "=" * 60)
total = t1 + t2 + t3 + t4
print(f"TOTAL TIME: {total:.4f}s")
