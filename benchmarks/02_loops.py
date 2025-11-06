# Loop Performance Benchmark

import time

def bench_simple_loop():
    """Simple for loop"""
    start = time.time()
    count = 0
    for i in range(2000000):
        count = count + 1
    end = time.time()
    return end - start

def bench_nested_loops():
    """Nested loops"""
    start = time.time()
    total = 0
    for i in range(1000):
        for j in range(1000):
            total = total + 1
    end = time.time()
    return end - start

def bench_while_loop():
    """While loop"""
    start = time.time()
    i = 0
    count = 0
    while i < 1000000:
        count = count + 1
        i = i + 1
    end = time.time()
    return end - start

print("=== Loop Benchmarks ===")
print(f"Simple loop: {bench_simple_loop():.4f}s")
print(f"Nested loops: {bench_nested_loops():.4f}s")
print(f"While loop: {bench_while_loop():.4f}s")
