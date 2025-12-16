#!/usr/bin/env python3
"""Simple test for C compilation"""

print("=== Simple C Compilation Test ===")

# Basic arithmetic
a = 10
b = 20
c = a + b
print(f"10 + 20 = {c}")

# Type conversions
x = int("42")
y = float(x)
z = str(y)
print(f"int('42') = {x}, float(42) = {y}, str(42.0) = '{z}'")

# Simple function
def add(a, b):
    return a + b

result = add(5, 7)
print(f"add(5, 7) = {result}")

print("=== Test Complete ===")
