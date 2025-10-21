#!/usr/bin/env tauraro
# Test 1: Basic Data Types

print("=" * 60)
print("TEST 1: BASIC DATA TYPES")
print("=" * 60)
print()

# Integers
print("INTEGERS:")
x = 42
print(f"x = {x}, type: {type(x)}")
negative = -123
print(f"negative = {negative}")
zero = 0
print(f"zero = {zero}")
print()

# Floats
print("FLOATS:")
pi = 3.14159
print(f"pi = {pi}, type: {type(pi)}")
negative_float = -2.5
print(f"negative_float = {negative_float}")
print()

# Strings
print("STRINGS:")
s1 = "Hello, Tauraro!"
print(f"s1 = {s1}, type: {type(s1)}")
s2 = 'Single quotes work too'
print(f"s2 = {s2}")
empty_str = ""
print(f"empty_str = '{empty_str}', len = {len(empty_str)}")
print()

# Booleans
print("BOOLEANS:")
true_val = True
false_val = False
print(f"true_val = {true_val}, type: {type(true_val)}")
print(f"false_val = {false_val}")
print()

# None
print("NONE:")
nothing = None
print(f"nothing = {nothing}, type: {type(nothing)}")
print()

# Type conversions (already tested, but confirm)
print("TYPE CONVERSIONS:")
print(f"int('100') = {int('100')}")
print(f"float('3.14') = {float('3.14')}")
print(f"str(42) = {str(42)}")
print(f"bool(1) = {bool(1)}")
print(f"bool(0) = {bool(0)}")
print()

print("✓ All basic data types working!")
