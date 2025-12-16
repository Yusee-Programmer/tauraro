#!/usr/bin/env python3
"""
Simple test for shared library generation (no imports)
"""

def add(a, b):
    return a + b

def multiply(a, b):
    result = 0
    i = 0
    while i < b:
        result = result + a
        i = i + 1
    return result

# Test
print("Testing: 5 + 3 =", add(5, 3))
print("Testing: 4 * 7 =", multiply(4, 7))
