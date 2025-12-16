#!/usr/bin/env python3
"""
Benchmark: Hash Table Operations
Tests: Dictionary operations, hashing, memory allocation
"""
import time
import sys

def hash_insert_test(size):
    """Test dictionary insertions"""
    d = {}
    for i in range(size):
        d[f"key_{i}"] = i
    return d

def hash_lookup_test(d, iterations):
    """Test dictionary lookups"""
    size = len(d)
    total = 0
    for i in range(iterations):
        key = f"key_{i % size}"
        if key in d:
            total += d[key]
    return total

def hash_delete_test(d, count):
    """Test dictionary deletions"""
    keys_to_delete = list(d.keys())[:count]
    for key in keys_to_delete:
        del d[key]
    return len(d)

def main():
    size = 100000 if len(sys.argv) < 2 else int(sys.argv[1])

    # Test 1: Insertions
    print(f"Test 1: Hash insertions ({size} items)")
    start = time.time()
    d = hash_insert_test(size)
    elapsed1 = time.time() - start
    print(f"  Dictionary size: {len(d)}")
    print(f"  Time: {elapsed1:.4f} seconds")

    # Test 2: Lookups
    lookups = size * 10
    print(f"\nTest 2: Hash lookups ({lookups} operations)")
    start = time.time()
    total = hash_lookup_test(d, lookups)
    elapsed2 = time.time() - start
    print(f"  Total sum: {total}")
    print(f"  Time: {elapsed2:.4f} seconds")

    # Test 3: Deletions
    delete_count = size // 2
    print(f"\nTest 3: Hash deletions ({delete_count} items)")
    start = time.time()
    remaining = hash_delete_test(d, delete_count)
    elapsed3 = time.time() - start
    print(f"  Remaining items: {remaining}")
    print(f"  Time: {elapsed3:.4f} seconds")

    total_time = elapsed1 + elapsed2 + elapsed3
    print(f"\nTotal time: {total_time:.4f} seconds")
    return total_time

if __name__ == "__main__":
    main()
