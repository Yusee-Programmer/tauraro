# Function Call Benchmark for Python
# Tests function definition and calling performance

print("Starting Python Function Benchmark...")

# Simple function definition
def simple_add(a, b):
    return a + b

# Recursive function definition
def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

# Function call benchmark
iterations = 5000
i = 0
while i < iterations:
    result = simple_add(10, 5)
    i = i + 1

print("Simple function calls completed")

# Recursive function benchmark
i = 0
while i < 100:
    result = factorial(10)
    i = i + 1

print("Recursive function calls completed")

print("Function benchmark completed. Final result:", result)
print("Python Function Benchmark finished!")