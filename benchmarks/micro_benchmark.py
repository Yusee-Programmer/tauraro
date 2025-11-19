import time

def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

print("PYTHON MICRO-BENCHMARK")
print("=" * 60)

start = time.time()
r1 = fib(20)
t1 = time.time() - start
print(f"Fibonacci(20):    {r1:>8}  {t1:>8.4f}s")

start = time.time()
r2 = 0
for i in range(100000):
    r2 = r2 + 1
t2 = time.time() - start
print(f"Loop 100K:        {r2:>8}  {t2:>8.4f}s")

print("=" * 60)
print(f"Total: {t1 + t2:.4f}s")
