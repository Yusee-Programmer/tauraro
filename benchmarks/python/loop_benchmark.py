# Loop Performance Benchmark for Python
# Tests various loop constructs and control flow

print("Starting Python Loop Benchmark...")

# Simple loop benchmark
iterations = 5000
i = 0
while i < iterations:
    i = i + 1

print("Simple loop completed")

# Nested loop benchmark
outer_iterations = 50
inner_iterations = 50
i = 0
while i < outer_iterations:
    j = 0
    while j < inner_iterations:
        result = i * j
        j = j + 1
    i = i + 1

print("Nested loop completed")

# Loop with conditional benchmark
i = 0
count = 0
while i < iterations:
    if i % 2 == 0:
        count = count + 1
    i = i + 1

print("Conditional loop completed")

print("Loop benchmark completed. Count:", count)
print("Python Loop Benchmark finished!")