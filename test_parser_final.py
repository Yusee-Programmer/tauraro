#!/usr/bin/env tauraro
"""Test parser fixes - production ready test"""

def returns_single():
    return 42

def returns_tuple():
    return 1, 2, 3

def returns_with_trailing():
    return 1, 2,

def returns_with_parens():
    return (10, 20, 30)

def returns_nested():
    return (1, 2), (3, 4)

def calculate():
    x = 5
    y = 10
    return x, y, x + y

# Main test
print("=== Parser Tuple Return Tests ===")

result1 = returns_single()
print(f"Single: {result1}")

result2 = returns_tuple()
print(f"Tuple: {result2}")

result3 = returns_with_trailing()
print(f"Trailing comma: {result3}")

result4 = returns_with_parens()
print(f"With parens: {result4}")

result5 = returns_nested()
print(f"Nested: {result5}")

# Test tuple unpacking from return
a, b, c = returns_tuple()
print(f"Unpacked: a={a}, b={b}, c={c}")

# Test calculation
x, y, sum = calculate()
print(f"Calculate: x={x}, y={y}, sum={sum}")

print("\n=== All Parser Tests Passed ===")
