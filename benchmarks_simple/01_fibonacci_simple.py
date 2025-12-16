#!/usr/bin/env python3
"""Simple Fibonacci - only basic features"""

def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

def main():
    n = 30
    result = fibonacci(n)
    print(f"fibonacci({n}) = {result}")
    return result

if __name__ == "__main__":
    main()
