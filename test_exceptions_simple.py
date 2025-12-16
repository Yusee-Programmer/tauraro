#!/usr/bin/env python3
"""Simple exception test for C compilation"""

print("=== Exception Handling in C ===\n")

# Test 1: Basic try-except
print("Test 1: Basic try-except")
try:
    x = 10 / 2
    print(f"  Success: {x}")
except:
    print("  Should not catch")

# Test 2: Try-finally
print("\nTest 2: Try-finally")
try:
    y = 5 + 5
    print(f"  Try: {y}")
finally:
    print("  Finally executed")

# Test 3: Simple function with try-except
print("\nTest 3: Function with exception handling")
def safe_divide(a, b):
    try:
        return a / b
    except:
        return 0

result = safe_divide(10, 2)
print(f"  safe_divide(10, 2) = {result}")

print("\n=== All Tests Complete ===")
