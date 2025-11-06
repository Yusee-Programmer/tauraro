# String Operations Benchmark

import time

def bench_string_concat():
    """String concatenation"""
    start = time.time()
    s = ""
    for i in range(10000):
        s = s + "a"
    end = time.time()
    return end - start

def bench_string_format():
    """String formatting"""
    start = time.time()
    for i in range(100000):
        s = f"Number: {i}"
    end = time.time()
    return end - start

def bench_string_operations():
    """Various string operations"""
    start = time.time()
    for i in range(50000):
        s = "hello world"
        s = s.upper()
        s = s.lower()
        s = s.replace("world", "python")
    end = time.time()
    return end - start

print("=== String Benchmarks ===")
print(f"String concat: {bench_string_concat():.4f}s")
print(f"String format: {bench_string_format():.4f}s")
print(f"String operations: {bench_string_operations():.4f}s")
