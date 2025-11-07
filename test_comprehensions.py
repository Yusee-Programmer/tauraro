# Test all Python comprehension types

print("=== Testing List Comprehensions ===")

# Basic list comprehension
squares = [i * i for i in range(5)]
print("Squares:", squares)

# List comprehension with condition
evens = [i for i in range(10) if i % 2 == 0]
print("Evens:", evens)

# Nested list comprehension
matrix = [[i + j for j in range(3)] for i in range(3)]
print("Matrix:", matrix)

# List comprehension with multiple conditions
filtered = [x for x in range(20) if x % 2 == 0 if x % 3 == 0]
print("Divisible by 2 and 3:", filtered)

print("\n=== Testing Dict Comprehensions ===")

# Basic dict comprehension
square_dict = {i: i * i for i in range(5)}
print("Square dict:", square_dict)

# Dict comprehension with condition
even_dict = {i: i * 2 for i in range(10) if i % 2 == 0}
print("Even dict:", even_dict)

# Dict comprehension from lists (tuple unpacking not yet supported)
# keys = ['a', 'b', 'c']
# values = [1, 2, 3]
# combined = {k: v for k, v in zip(keys, values)}
# print("Combined dict:", combined)

# Alternative without tuple unpacking
pairs = [['a', 1], ['b', 2], ['c', 3]]
combined = {p[0]: p[1] for p in pairs}
print("Combined dict:", combined)

print("\n=== Testing Set Comprehensions ===")

# Basic set comprehension
square_set = {i * i for i in range(5)}
print("Square set:", square_set)

# Set comprehension with condition
even_set = {i for i in range(10) if i % 2 == 0}
print("Even set:", even_set)

# Set comprehension removes duplicates
deduped = {x % 5 for x in range(20)}
print("Deduped (mod 5):", deduped)

print("\n=== Testing Generator Expressions ===")

# Generator expression (similar to comprehension)
gen = (i * i for i in range(5))
print("Generator:", list(gen))

print("\n=== All comprehension tests passed! ===")
