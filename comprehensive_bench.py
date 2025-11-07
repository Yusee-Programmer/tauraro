# Comprehensive benchmark - similar to previous tests
# This matches the format used in FINAL_OPTIMIZATION_SUMMARY.md

print("=== Comprehensive Tauraro Benchmark ===")
print("")

# Arithmetic benchmark (1M operations)
print("Starting arithmetic benchmark...")
a = 0
b = 0
c = 0
for i in range(1000000):
    a = i + 1
    b = a - 5
    c = b + 10
print(f"Arithmetic complete: a={a}, b={b}, c={c}")

# Loop benchmark (1M iterations)
print("Starting loop benchmark...")
total = 0
for i in range(1000000):
    total = total + i
print(f"Loop complete: total={total}")

# Function call benchmark
print("Starting function benchmark...")
def add_one(x):
    return x + 1

result = 0
for i in range(100000):
    result = add_one(result)
print(f"Function complete: result={result}")

print("")
print("All benchmarks complete!")
