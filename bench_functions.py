# Function call benchmark
# Tests parameter passing and return values

print("=== Function Benchmark ===")

# Simple function with integer parameter and return
def add_one(x):
    return x + 1

# Test simple function calls
result = 0
for i in range(100000):
    result = add_one(result)
print(f"Simple function (100K calls): {result}")

# Function with multiple parameters
def add_three(a, b, c):
    return a + b + c

total = 0
for i in range(50000):
    total = add_three(i, i + 1, i + 2)
print(f"Multi-param function (50K calls): {total}")

# Recursive function
def factorial_iterative(n):
    result = 1
    for i in range(1, n + 1):
        result = result * i
    return result

fact_result = factorial_iterative(20)
print(f"Iterative factorial(20): {fact_result}")

# Nested function calls
def double(x):
    return x * 2

def quadruple(x):
    return double(double(x))

nested_result = 1
for i in range(10000):
    nested_result = quadruple(nested_result % 1000)
print(f"Nested calls (10K iters): {nested_result}")

print("Function benchmarks complete!")
