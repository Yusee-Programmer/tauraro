# Comprehensive tuple parsing test

print("=== Tuple Parsing Test ===")
print()

# Test 1: Empty tuple
print("Test 1: Empty tuple")
empty = ()
print("Empty tuple:", empty)
print("Type:", type(empty))
print("Length:", len(empty))
print()

# Test 2: Single element tuple (with trailing comma)
print("Test 2: Single element tuple")
single = (1,)
print("Single tuple:", single)
print("Type:", type(single))
print("Length:", len(single))
print()

# Test 3: Multiple element tuple
print("Test 3: Multiple element tuple")
multi = (1, 2, 3, 4, 5)
print("Multi tuple:", multi)
print("Length:", len(multi))
print()

# Test 4: Tuple without parentheses
print("Test 4: Tuple without parentheses")
no_parens = 1, 2, 3
print("Tuple:", no_parens)
print("Type:", type(no_parens))
print()

# Test 5: Nested tuples
print("Test 5: Nested tuples")
nested = (1, (2, 3), (4, (5, 6)))
print("Nested tuple:", nested)
print()

# Test 6: Tuple unpacking
print("Test 6: Tuple unpacking")
a, b, c = (1, 2, 3)
print("a =", a, ", b =", b, ", c =", c)
print()

# Test 7: Tuple with mixed types
print("Test 7: Tuple with mixed types")
mixed = (1, "hello", 3.14, True, None)
print("Mixed tuple:", mixed)
print()

# Test 8: Tuple indexing
print("Test 8: Tuple indexing")
t = (10, 20, 30, 40, 50)
print("t[0] =", t[0])
print("t[2] =", t[2])
print("t[-1] =", t[-1])
print()

# Test 9: Tuple slicing
print("Test 9: Tuple slicing")
t = (0, 1, 2, 3, 4, 5)
print("t[1:4] =", t[1:4])
print("t[:3] =", t[:3])
print("t[3:] =", t[3:])
print()

# Test 10: Tuple in function return
print("Test 10: Tuple in function return")
def get_coords():
    return (10, 20)

x, y = get_coords()
print("Coordinates: x =", x, ", y =", y)
print()

print("=== All Tuple Tests Completed ===")
