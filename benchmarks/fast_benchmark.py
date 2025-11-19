import time

def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

def simple_loop(n):
    total = 0
    for i in range(n):
        total = total + 1
    return total

def is_prime(n):
    if n <= 1:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True

def primes(n):
    count = 0
    for i in range(n):
        if is_prime(i):
            count = count + 1
    return count

print("PYTHON FAST BENCHMARK")
print("=" * 60)

start = time.time()
r1 = fib(25)
t1 = time.time() - start
print(f"Fibonacci(25):     {r1:>10}  {t1:>10.4f}s")

start = time.time()
r2 = simple_loop(100000)
t2 = time.time() - start
print(f"Simple Loop 100K:  {r2:>10}  {t2:>10.4f}s")

start = time.time()
r3 = primes(2000)
t3 = time.time() - start
print(f"Count Primes 2K:   {r3:>10}  {t3:>10.4f}s")

total = t1 + t2 + t3
print("=" * 60)
print(f"Total: {total:.4f}s")
