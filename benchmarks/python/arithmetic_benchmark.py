# Arithmetic Operations Benchmark for Python
# Tests basic mathematical operations performance

print("Starting Python Arithmetic Benchmark...")

# Arithmetic operations benchmark
iterations = 5000

# Addition benchmark
i = 0
while i < iterations:
    result = 10 + 5
    i = i + 1

print("Addition completed")

# Subtraction benchmark
i = 0
while i < iterations:
    result = 10 - 5
    i = i + 1

print("Subtraction completed")

print("Arithmetic benchmark completed. Final result:", result)
print("Python Arithmetic Benchmark finished!")