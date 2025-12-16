#!/usr/bin/env python3
"""
Benchmark: File I/O Operations
Tests: File reading, writing, buffering
"""
import time
import sys
import os

def write_test(filename, lines):
    """Test file writing"""
    with open(filename, 'w') as f:
        for i in range(lines):
            f.write(f"Line {i}: This is test data for benchmarking\n")

def read_test(filename):
    """Test file reading"""
    line_count = 0
    char_count = 0
    with open(filename, 'r') as f:
        for line in f:
            line_count += 1
            char_count += len(line)
    return line_count, char_count

def read_all_test(filename):
    """Test reading entire file at once"""
    with open(filename, 'r') as f:
        content = f.read()
    return len(content)

def main():
    lines = 100000 if len(sys.argv) < 2 else int(sys.argv[1])
    filename = "benchmark_test_file.txt"

    # Test 1: Write
    print(f"Test 1: Writing {lines} lines")
    start = time.time()
    write_test(filename, lines)
    elapsed1 = time.time() - start
    file_size = os.path.getsize(filename)
    print(f"  File size: {file_size} bytes")
    print(f"  Time: {elapsed1:.4f} seconds")

    # Test 2: Read line by line
    print(f"\nTest 2: Reading line by line")
    start = time.time()
    line_count, char_count = read_test(filename)
    elapsed2 = time.time() - start
    print(f"  Lines read: {line_count}")
    print(f"  Chars read: {char_count}")
    print(f"  Time: {elapsed2:.4f} seconds")

    # Test 3: Read all at once
    print(f"\nTest 3: Reading entire file")
    start = time.time()
    content_size = read_all_test(filename)
    elapsed3 = time.time() - start
    print(f"  Content size: {content_size} bytes")
    print(f"  Time: {elapsed3:.4f} seconds")

    # Cleanup
    if os.path.exists(filename):
        os.remove(filename)

    total_time = elapsed1 + elapsed2 + elapsed3
    print(f"\nTotal time: {total_time:.4f} seconds")
    return total_time

if __name__ == "__main__":
    main()
