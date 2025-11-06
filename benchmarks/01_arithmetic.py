# Arithmetic Operations Benchmark

import time

def bench_integer_arithmetic():
    """Test basic integer arithmetic"""
    start = time.time()
    result = 0
    for i in range(1000000):
        result = result + 1
        result = result - 1
        result = result + 2
    end = time.time()
    return end - start

def bench_float_arithmetic():
    """Test floating point arithmetic"""
    start = time.time()
    result = 0.0
    for i in range(1000000):
        result = result + 1.5
        result = result * 1.1
        result = result / 1.1
    end = time.time()
    return end - start

def bench_mixed_arithmetic():
    """Test mixed arithmetic operations"""
    start = time.time()
    x = 0
    for i in range(500000):
        x = (i * 2 + 3) * (i + 1) - (i - 1)
        x = x % 1000
    end = time.time()
    return end - start

print("=== Arithmetic Benchmarks ===")
print(f"Integer arithmetic: {bench_integer_arithmetic():.4f}s")
print(f"Float arithmetic: {bench_float_arithmetic():.4f}s")
print(f"Mixed arithmetic: {bench_mixed_arithmetic():.4f}s")
