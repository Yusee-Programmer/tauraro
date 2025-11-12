import random

print("Testing random.choice...")

# Test 1: Simple list
names = ["Alice", "Bob", "Charlie"]
print(f"names = {names}")
print(f"len(names) = {len(names)}")

result = random.choice(names)
print(f"random.choice(names) = {result}")

# Test 2: Numbers
numbers = [1, 2, 3, 4, 5]
print(f"\nnumbers = {numbers}")
result2 = random.choice(numbers)
print(f"random.choice(numbers) = {result2}")

print("\nAll tests passed!")
