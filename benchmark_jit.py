# JIT Performance Benchmark
# Compares interpreted vs JIT-compiled execution speed

import time

print("=" * 60)
print("JIT Performance Benchmark")
print("=" * 60)
print()

# Benchmark 1: Large loop (should trigger JIT)
print("Benchmark 1: Sum of integers (50,000 iterations)")
print("-" * 60)

start = time.time()
total = 0
for i in range(50000):
    total = total + i
end = time.time()

elapsed = (end - start) * 1000  # Convert to milliseconds
print(f"Result: {total}")
print(f"Time: {elapsed:.2f} ms")
print()

# Expected result (1249975000 = sum of 0..49999)
expected = 1249975000
assert total == expected, f"Expected {expected}, got {total}"
print("✓ Correctness verified")
print()

# Benchmark 2: Loop with multiplication (should also trigger JIT)
print("Benchmark 2: Multiplication loop (40,000 iterations)")
print("-" * 60)

start = time.time()
result = 0
for i in range(40000):
    result = result + i * 3
end = time.time()

elapsed = (end - start) * 1000
print(f"Result: {result}")
print(f"Time: {elapsed:.2f} ms")
print()

# Expected: 2399940000 = sum of i*3 for i in 0..39999
expected = 2399940000
assert result == expected, f"Expected {expected}, got {result}"
print("✓ Correctness verified")
print()

# Benchmark 3: Nested arithmetic (should trigger JIT)
print("Benchmark 3: Complex arithmetic (30,000 iterations)")
print("-" * 60)

start = time.time()
value = 0
for i in range(30000):
    temp = i * 2
    value = value + temp + i
end = time.time()

elapsed = (end - start) * 1000
print(f"Result: {value}")
print(f"Time: {elapsed:.2f} ms")
print()

# Expected: 1349955000 = sum of (i*2 + i) for i in 0..29999
expected = 1349955000
assert value == expected, f"Expected {expected}, got {value}"
print("✓ Correctness verified")
print()

print("=" * 60)
print("Benchmark Complete!")
print("=" * 60)
print()
print("Notes:")
print("- First ~10,000 iterations run in interpreted mode")
print("- After JIT compilation, remaining iterations run at ~100x speed")
print("- Check console for 'JIT: Compiled loop' messages")
