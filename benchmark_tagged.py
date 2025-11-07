# Benchmark to test TaggedValue performance improvement
# Should show 2-3x improvement on integer arithmetic

import time

def bench_arithmetic():
    """Test arithmetic operations"""
    start = time.time()

    total = 0
    for i in range(1000000):
        total = total + i
        total = total - 1
        total = total + 2

    elapsed = time.time() - start
    print(f"Arithmetic (1M iterations): {elapsed:.2f}s")
    return total

def bench_loops():
    """Test loop performance"""
    start = time.time()

    sum_val = 0
    for i in range(1000000):
        sum_val = sum_val + i

    elapsed = time.time() - start
    print(f"Loops (1M iterations): {elapsed:.2f}s")
    return sum_val

print("=== TaggedValue Performance Benchmark ===")
print("")

result1 = bench_arithmetic()
result2 = bench_loops()

print("")
print(f"Results: {result1}, {result2}")
