"""
Test to measure overhead of value pooling
"""
import time

print("Testing pooling overhead...")

# Warm up
for i in range(1000):
    x = i

# Test: Create many small integers
iterations = 100000
start = time.time()
for i in range(iterations):
    x = 42  # Pooled value
elapsed = time.time() - start

print(f"Created {iterations} pooled integers in {elapsed:.4f}s")
print(f"Rate: {iterations/elapsed:.0f} ops/sec")

# Test: Integer arithmetic
start = time.time()
result = 0
for i in range(iterations):
    result = result + 1
elapsed = time.time() - start

print(f"Performed {iterations} additions in {elapsed:.4f}s")
print(f"Rate: {iterations/elapsed:.0f} ops/sec")
print(f"Result: {result}")
