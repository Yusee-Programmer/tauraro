#!/usr/bin/env python3
"""Nested loops and multiplication"""

def multiply_matrix_elements(size):
    total = 0
    for i in range(size):
        for j in range(size):
            total += (i * j)
    return total

def count_pairs(n):
    count = 0
    for i in range(n):
        for j in range(i, n):
            if (i + j) % 2 == 0:
                count += 1
    return count

def main():
    size = 500

    result1 = multiply_matrix_elements(size)
    print(f"Matrix sum ({size}x{size}): {result1}")

    result2 = count_pairs(1000)
    print(f"Even sum pairs: {result2}")

    return result1 + result2

if __name__ == "__main__":
    main()
