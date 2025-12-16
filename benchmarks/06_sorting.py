#!/usr/bin/env python3
"""
Benchmark: Sorting Algorithms
Tests: Sorting performance, comparison operations, array manipulation
"""
import time
import sys
import random

def quicksort(arr):
    """Quick sort implementation"""
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    left = [x for x in arr if x < pivot]
    middle = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]
    return quicksort(left) + middle + quicksort(right)

def bubble_sort(arr):
    """Bubble sort implementation"""
    n = len(arr)
    arr = arr[:]  # Copy array
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr

def main():
    size = 10000 if len(sys.argv) < 2 else int(sys.argv[1])

    # Generate random data
    print(f"Generating {size} random numbers...")
    data = [random.randint(0, size * 10) for _ in range(size)]

    # Test 1: Built-in sorted()
    print(f"\nTest 1: Built-in sorted()")
    start = time.time()
    sorted1 = sorted(data)
    elapsed1 = time.time() - start
    print(f"  First 5: {sorted1[:5]}")
    print(f"  Last 5: {sorted1[-5:]}")
    print(f"  Time: {elapsed1:.4f} seconds")

    # Test 2: Quicksort (smaller dataset for recursion limits)
    small_size = min(size, 5000)
    small_data = data[:small_size]
    print(f"\nTest 2: Quicksort ({small_size} items)")
    start = time.time()
    sorted2 = quicksort(small_data)
    elapsed2 = time.time() - start
    print(f"  First 5: {sorted2[:5]}")
    print(f"  Last 5: {sorted2[-5:]}")
    print(f"  Time: {elapsed2:.4f} seconds")

    # Test 3: Bubble sort (very small dataset)
    tiny_size = min(size, 1000)
    tiny_data = data[:tiny_size]
    print(f"\nTest 3: Bubble sort ({tiny_size} items)")
    start = time.time()
    sorted3 = bubble_sort(tiny_data)
    elapsed3 = time.time() - start
    print(f"  First 5: {sorted3[:5]}")
    print(f"  Last 5: {sorted3[-5:]}")
    print(f"  Time: {elapsed3:.4f} seconds")

    total_time = elapsed1 + elapsed2 + elapsed3
    print(f"\nTotal time: {total_time:.4f} seconds")
    return total_time

if __name__ == "__main__":
    main()
