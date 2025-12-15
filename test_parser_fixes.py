#!/usr/bin/env tauraro
"""Test parser fixes for return statements"""

# Test 1: Simple return
def returns_single():
    return 42

# Test 2: Tuple return
def returns_tuple():
    return 1, 2, 3

# Test 3: Tuple return with trailing comma
def returns_with_trailing():
    return 1, 2,

# Test 4: Tuple return with parentheses
def returns_with_parens():
    return (10, 20, 30)

# Test 5: Return nothing
def returns_nothing():
    return

# Test 6: Nested tuple return
def returns_nested():
    return (1, 2), (3, 4)

# Main test
print("=== Testing Return Statements ===")

result1 = returns_single()
print(f"Single: {result1}")

result2 = returns_tuple()
print(f"Tuple: {result2}")

result3 = returns_with_trailing()
print(f"Trailing comma: {result3}")

result4 = returns_with_parens()
print(f"With parens: {result4}")

result5 = returns_nothing()
print(f"Nothing: {result5}")

result6 = returns_nested()
print(f"Nested: {result6}")

# Test tuple unpacking from return
a, b, c = returns_tuple()
print(f"Unpacked: a={a}, b={b}, c={c}")

print("\n=== All Parser Tests Passed ===")
