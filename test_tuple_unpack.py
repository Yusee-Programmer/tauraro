# Test tuple unpacking
print("=" * 60)
print("Testing Tuple Unpacking")
print("=" * 60)

# Test 1: Simple tuple unpacking
print("\nTest 1: Simple tuple unpacking")
a, b = (1, 2)
print("a =", a)
print("b =", b)
if a == 1 and b == 2:
    print("PASS")
else:
    print("FAIL")

# Test 2: Three elements
print("\nTest 2: Three elements")
x, y, z = (10, 20, 30)
print("x =", x)
print("y =", y)
print("z =", z)
if x == 10 and y == 20 and z == 30:
    print("PASS")
else:
    print("FAIL")

# Test 3: String elements
print("\nTest 3: String elements")
first, second = ("hello", "world")
print("first =", first)
print("second =", second)
if first == "hello" and second == "world":
    print("PASS")
else:
    print("FAIL")

# Test 4: Mixed types
print("\nTest 4: Mixed types")
name, age, height = ("Alice", 30, 5.8)
print("name =", name)
print("age =", age)
print("height =", height)
if name == "Alice" and age == 30 and height == 5.8:
    print("PASS")
else:
    print("FAIL")

print("\n" + "=" * 60)
print("ALL TUPLE UNPACKING TESTS COMPLETED!")
print("=" * 60)
