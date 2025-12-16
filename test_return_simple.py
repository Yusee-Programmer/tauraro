#!/usr/bin/env tauraro
"""Test return with trailing comma"""

def returns_with_trailing():
    return 1, 2,

def another_func():
    return 3, 4

result = returns_with_trailing()
print(f"Result: {result}")

result2 = another_func()
print(f"Result2: {result2}")
