#!/usr/bin/env tauraro

print("Testing for loop with tuples (no unpacking)...")

# Test with list of tuples, but don't unpack
items = [(1, 2), (3, 4), (5, 6)]
for item in items:
    print(f"item = {item}")
    print(f"item[0] = {item[0]}")
    print(f"item[1] = {item[1]}")
    print()

print("Test passed!")
