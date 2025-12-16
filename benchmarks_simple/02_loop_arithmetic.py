#!/usr/bin/env python3
"""Loop and arithmetic operations"""

def sum_range(n):
    total = 0
    for i in range(n):
        total += i
    return total

def sum_squares(n):
    total = 0
    for i in range(n):
        total += i * i
    return total

def factorial(n):
    if n <= 1:
        return 1
    result = 1
    for i in range(2, n + 1):
        result *= i
    return result

def main():
    n = 1000000

    result1 = sum_range(n)
    print(f"Sum 1 to {n}: {result1}")

    result2 = sum_squares(100)
    print(f"Sum of squares 1 to 100: {result2}")

    result3 = factorial(20)
    print(f"Factorial(20): {result3}")

    return result1 + result2 + result3

if __name__ == "__main__":
    main()
