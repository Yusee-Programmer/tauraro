# Mathematical Computation Benchmark for Python
# Tests mathematical operations and algorithms

print("Starting Python Math Benchmark...")

# Fibonacci function
def fibonacci(n):
    if n <= 1:
        return n
    a = 0
    b = 1
    for i in range(2, n + 1):
        temp = a + b
        a = b
        b = temp
    return b

# Prime number check
def is_prime(n):
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    for i in range(3, int(n ** 0.5) + 1, 2):
        if n % i == 0:
            return False
    return True

# Find primes up to n
def find_primes(n):
    primes = []
    for i in range(2, n + 1):
        if is_prime(i):
            primes.append(i)
    return primes

# Math operations benchmark
print("Testing Fibonacci computation...")
for i in range(30):
    result = fibonacci(i)

print("Fibonacci computation completed")

print("Testing prime number finding...")
primes = find_primes(1000)
print("Found", len(primes), "primes up to 1000")

print("Testing mathematical operations...")
iterations = 10000
i = 0
result = 0
while i < iterations:
    # Square root
    result = 144 ** 0.5
    
    # Trigonometric functions
    import math
    result = math.sin(3.14159 / 4)
    
    # Logarithm
    result = math.log(100)
    
    i = i + 1

print("Mathematical operations completed")
print("Python Math Benchmark finished!")