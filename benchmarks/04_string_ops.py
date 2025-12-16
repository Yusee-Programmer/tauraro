#!/usr/bin/env python3
"""
Benchmark: String Operations
Tests: String concatenation, manipulation, memory allocation
"""
import time
import sys

def string_concat_test(iterations):
    """Test string concatenation"""
    result = ""
    for i in range(iterations):
        result += str(i)
    return result

def string_join_test(iterations):
    """Test string joining (more efficient)"""
    parts = [str(i) for i in range(iterations)]
    return "".join(parts)

def string_operations(text):
    """Various string operations"""
    # Case operations
    upper = text.upper()
    lower = text.lower()

    # Searching
    count = text.count("the")

    # Splitting
    words = text.split()

    # Replacing
    replaced = text.replace("the", "THE")

    return len(words), count

def main():
    iterations = 10000 if len(sys.argv) < 2 else int(sys.argv[1])

    # Test 1: String joining (efficient)
    print(f"Test 1: String join ({iterations} iterations)")
    start = time.time()
    result1 = string_join_test(iterations)
    elapsed1 = time.time() - start
    print(f"  Result length: {len(result1)}")
    print(f"  Time: {elapsed1:.4f} seconds")

    # Test 2: String operations
    text = "the quick brown fox jumps over the lazy dog " * 1000
    print(f"\nTest 2: String operations (text length: {len(text)})")
    start = time.time()
    words, count = string_operations(text)
    elapsed2 = time.time() - start
    print(f"  Words: {words}, 'the' count: {count}")
    print(f"  Time: {elapsed2:.4f} seconds")

    print(f"\nTotal time: {elapsed1 + elapsed2:.4f} seconds")
    return elapsed1 + elapsed2

if __name__ == "__main__":
    main()
