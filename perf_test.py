# Performance test for TaggedValue
# Simple arithmetic-heavy workload

print("=== Performance Test ===")

# Test 1: Simple integer arithmetic (should use TaggedValue fast path)
total = 0
for i in range(1000000):
    total = total + 1

print(f"Sum of 1M iterations: {total}")

# Test 2: Mixed operations
result = 0
for i in range(100000):
    result = result + i
    result = result - 5
    result = result + 10

print(f"Mixed ops result: {result}")

print("Tests complete!")
