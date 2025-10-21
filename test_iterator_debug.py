#!/usr/bin/env tauraro

print("Iterator Debug Test")
print("=" * 40)

# Create a simple list of tuples
items = [(1, 2), (3, 4)]

print(f"items = {items}")
print(f"items[0] = {items[0]}")
print(f"items[1] = {items[1]}")
print()

print("Now iterating:")
count = 0
for item in items:
    count = count + 1
    print(f"Iteration {count}")
    print(f"  item = {item}")
    print(f"  type(item) = {type(item)}")
    if count >= 3:
        print("ERROR: Too many iterations!")
        break

print()
print(f"Total iterations: {count}")
