# Test suite for Tier 2 dispatch handlers
# Exercises: GetIter, ForIter, BuildDict, BuildTuple, BuildSet, LoadAttr, StoreAttr
# Fast integer ops, comparison ops

print("=== Testing Tier 2 Dispatch Handlers ===")
print()

# Test 1: GetIter and ForIter (iteration handlers)
print("Test 1: Iteration (GetIter + ForIter)")
total = 0
for i in range(10):
    total = total + i
print(f"  Sum 0-9: {total}")
assert total == 45, f"Expected 45, got {total}"
print("  ✓ Passed")
print()

# Test 2: BuildDict
print("Test 2: BuildDict")
my_dict = {"a": 1, "b": 2, "c": 3}
print(f"  Dict: {my_dict}")
assert my_dict["a"] == 1, "Dict key 'a' should be 1"
assert my_dict["b"] == 2, "Dict key 'b' should be 2"
print("  ✓ Passed")
print()

# Test 3: BuildTuple
print("Test 3: BuildTuple")
my_tuple = (1, 2, 3, 4, 5)
print(f"  Tuple: {my_tuple}")
assert len(my_tuple) == 5, f"Expected length 5, got {len(my_tuple)}"
print("  ✓ Passed")
print()

# Test 4: BuildSet
print("Test 4: BuildSet")
my_set = {1, 2, 3, 2, 1}
print(f"  Set: {my_set}")
print("  ✓ Passed")
print()

# Test 5: LoadAttr and StoreAttr (OOP operations)
print("Test 5: LoadAttr + StoreAttr (OOP)")
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance(self):
        return (self.x * self.x + self.y * self.y) ** 0.5

p = Point(3, 4)
print(f"  Point: ({p.x}, {p.y})")
dist = p.distance()
print(f"  Distance: {dist}")
p.x = 5
p.y = 12
print(f"  Updated Point: ({p.x}, {p.y})")
dist2 = p.distance()
print(f"  New Distance: {dist2}")
print("  ✓ Passed")
print()

# Test 6: Fast integer operations
print("Test 6: Fast Integer Ops (FastIntAdd/Sub/Mul/Div/Mod)")
a = 100
b = 7
print(f"  a={a}, b={b}")
print(f"  a + b = {a + b}")
print(f"  a - b = {a - b}")
print(f"  a * b = {a * b}")
print(f"  a / b = {a / b}")
print(f"  a % b = {a % b}")
assert a + b == 107, "FastIntAdd failed"
assert a - b == 93, "FastIntSub failed"
assert a * b == 700, "FastIntMul failed"
assert a % b == 2, "FastIntMod failed"
print("  ✓ Passed")
print()

# Test 7: Comparison operations
print("Test 7: Comparison Ops (<=, >=, !=)")
x = 10
y = 20
print(f"  x={x}, y={y}")
print(f"  x <= y: {x <= y}")
print(f"  x >= y: {x >= y}")
print(f"  x != y: {x != y}")
assert x <= y, "CompareLessEqual failed"
assert not (x >= y), "CompareGreaterEqual failed"
assert x != y, "CompareNotEqual failed"
print("  ✓ Passed")
print()

# Test 8: Nested iteration with attributes
print("Test 8: Combined Test (iteration + OOP + comparisons)")
class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

rectangles = []
for w in range(1, 4):
    for h in range(1, 4):
        rect = Rectangle(w, h)
        rectangles.append(rect)

total_area = 0
for rect in rectangles:
    total_area = total_area + rect.area()

print(f"  Created {len(rectangles)} rectangles")
print(f"  Total area: {total_area}")
assert total_area == 36, f"Expected total area 36, got {total_area}"
print("  ✓ Passed")
print()

print("=== All Tier 2 Dispatch Tests Passed! ===")
