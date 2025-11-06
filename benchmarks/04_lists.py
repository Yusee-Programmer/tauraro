# List Operations Benchmark

import time

def bench_list_creation():
    """List creation and appending"""
    start = time.time()
    lst = []
    for i in range(100000):
        lst.append(i)
    end = time.time()
    return end - start

def bench_list_iteration():
    """List iteration"""
    lst = list(range(500000))
    start = time.time()
    total = 0
    for x in lst:
        total = total + x
    end = time.time()
    return end - start

def bench_list_indexing():
    """List indexing"""
    lst = list(range(100000))
    start = time.time()
    total = 0
    for i in range(len(lst)):
        total = total + lst[i]
    end = time.time()
    return end - start

def bench_list_comprehension():
    """List comprehension"""
    start = time.time()
    lst = [i * 2 for i in range(100000)]
    end = time.time()
    return end - start

print("=== List Benchmarks ===")
print(f"List creation: {bench_list_creation():.4f}s")
print(f"List iteration: {bench_list_iteration():.4f}s")
print(f"List indexing: {bench_list_indexing():.4f}s")
print(f"List comprehension: {bench_list_comprehension():.4f}s")
