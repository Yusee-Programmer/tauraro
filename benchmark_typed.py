# Python Type-Annotated Benchmark (equivalent to Tauraro typed)

# 1. Fibonacci with type hints
def fib(n: int) -> int:
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

# 2. Sum with typed variables
def sum_range(n: int) -> int:
    total: int = 0
    i: int = 0
    while i < n:
        total = total + i
        i = i + 1
    return total

# 3. Factorial with types
def factorial(n: int) -> int:
    result: int = 1
    i: int = 1
    while i <= n:
        result = result * i
        i = i + 1
    return result

# 4. Prime check with bool return
def is_prime(n: int) -> bool:
    if n < 2:
        return False
    i: int = 2
    while i * i <= n:
        if n % i == 0:
            return False
        i = i + 1
    return True

# 5. Count primes
def count_primes(n: int) -> int:
    count: int = 0
    i: int = 2
    while i <= n:
        if is_prime(i):
            count = count + 1
        i = i + 1
    return count

# 6. Typed arithmetic operations
def compute(a: int, b: int, c: int) -> int:
    x: int = a * b
    y: int = b + c
    z: int = x - y
    return z * 2

# Run benchmarks
print("=== PYTHON TYPED BENCHMARK ===")

print("Fibonacci(25):")
print(fib(25))

print("Sum(1..10000):")
print(sum_range(10000))

print("Factorial(12):")
print(factorial(12))

print("Primes up to 1000:")
print(count_primes(1000))

print("Compute(10, 20, 30):")
print(compute(10, 20, 30))

# Heavy typed computation
print("Heavy typed loop:")
result: int = 0
j: int = 0
while j < 5000:
    result = result + compute(j, j + 1, j + 2)
    j = j + 1
print(result)

print("=== TYPED BENCHMARK COMPLETE ===")
