#!/usr/bin/env tauraro

print("Simple tuple unpacking test...")

# Single iteration
print("\nTest: One tuple")
items = [(1, 2)]
for a, b in items:
    print(f"a={a}, b={b}")
    print("Success!")

# Two iterations
print("\nTest: Two tuples")
items2 = [(10, 20), (30, 40)]
count = 0
for x, y in items2:
    count = count + 1
    print(f"Iteration {count}: x={x}, y={y}")

print("\nDone!")
