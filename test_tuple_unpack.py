#!/usr/bin/env tauraro

print("Testing tuple unpacking in for loops...")

# Test 1: Simple tuple unpacking
print("\nTest 1: Simple tuple unpacking")
pairs = [(1, 2), (3, 4), (5, 6)]
for a, b in pairs:
    print(f"a={a}, b={b}")

# Test 2: With strings
print("\nTest 2: With strings")
items = [("apple", "red"), ("banana", "yellow")]
for name, color in items:
    print(f"{name} is {color}")

print("\nAll tests passed!")
