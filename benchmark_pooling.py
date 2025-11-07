"""
Micro-benchmark to measure object pooling performance improvement
Tests operations that benefit most from pooling
"""
import time

print("="*70)
print("Object Pooling Performance Benchmark")
print("="*70)

# Test 1: Small integer creation (pooled)
print("\nTest 1: Small Integer Creation (pooled -5 to 256)")
start = time.time()
for _ in range(10000):
    x = 0
    for i in range(100):
        x = i
elapsed = time.time() - start
print(f"  10,000 iterations x 100 integers")
print(f"  Time: {elapsed:.4f}s")
print(f"  Rate: {(10000 * 100 / elapsed):.0f} ops/sec")

# Test 2: Integer arithmetic (results pooled)
print("\nTest 2: Integer Arithmetic (small results)")
start = time.time()
for _ in range(10000):
    result = 0
    for i in range(50):
        result = result + 1
elapsed = time.time() - start
print(f"  10,000 iterations x 50 additions")
print(f"  Time: {elapsed:.4f}s")
print(f"  Rate: {(10000 * 50 / elapsed):.0f} ops/sec")

# Test 3: Boolean operations (pooled)
print("\nTest 3: Boolean Operations (pooled)")
start = time.time()
for _ in range(10000):
    for i in range(100):
        flag = i % 2 == 0
elapsed = time.time() - start
print(f"  10,000 iterations x 100 comparisons")
print(f"  Time: {elapsed:.4f}s")
print(f"  Rate: {(10000 * 100 / elapsed):.0f} ops/sec")

# Test 4: None returns (pooled)
print("\nTest 4: None Value Creation (pooled)")
start = time.time()
for _ in range(10000):
    for i in range(100):
        x = None
elapsed = time.time() - start
print(f"  10,000 iterations x 100 None assignments")
print(f"  Time: {elapsed:.4f}s")
print(f"  Rate: {(10000 * 100 / elapsed):.0f} ops/sec")

# Test 5: Fibonacci (combines integer operations)
print("\nTest 5: Fibonacci (integer-heavy workload)")
def fib(n):
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

start = time.time()
for _ in range(100):
    result = fib(15)
elapsed = time.time() - start
print(f"  100 iterations of fib(15)")
print(f"  Time: {elapsed:.4f}s")
print(f"  Rate: {(100 / elapsed):.2f} iter/sec")

# Test 6: Range iteration (pooled integers)
print("\nTest 6: Range Iteration (pooled integers)")
start = time.time()
for _ in range(1000):
    total = 0
    for i in range(100):
        total = total + i
elapsed = time.time() - start
print(f"  1,000 iterations x range(100)")
print(f"  Time: {elapsed:.4f}s")
print(f"  Rate: {(1000 * 100 / elapsed):.0f} ops/sec")

print("\n" + "="*70)
print("Benchmark Complete!")
print("="*70)
