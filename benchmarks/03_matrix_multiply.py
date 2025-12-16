#!/usr/bin/env python3
"""
Benchmark: Matrix Multiplication
Tests: Numerical computation, nested loops, memory access patterns
"""
import time
import sys

def matrix_multiply(a, b):
    """Multiply two matrices"""
    rows_a, cols_a = len(a), len(a[0])
    rows_b, cols_b = len(b), len(b[0])

    if cols_a != rows_b:
        raise ValueError("Matrix dimensions incompatible")

    # Initialize result matrix
    result = [[0 for _ in range(cols_b)] for _ in range(rows_a)]

    # Multiply
    for i in range(rows_a):
        for j in range(cols_b):
            for k in range(cols_a):
                result[i][j] += a[i][k] * b[k][j]

    return result

def create_matrix(rows, cols, value=1):
    """Create a matrix filled with value"""
    return [[value for _ in range(cols)] for _ in range(rows)]

def main():
    size = 200 if len(sys.argv) < 2 else int(sys.argv[1])

    print(f"Creating {size}x{size} matrices...")
    a = create_matrix(size, size, 2)
    b = create_matrix(size, size, 3)

    print("Multiplying matrices...")
    start = time.time()
    result = matrix_multiply(a, b)
    elapsed = time.time() - start

    print(f"Result[0][0] = {result[0][0]}")
    print(f"Result[{size-1}][{size-1}] = {result[size-1][size-1]}")
    print(f"Time: {elapsed:.4f} seconds")
    return elapsed

if __name__ == "__main__":
    main()
