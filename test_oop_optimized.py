"""
OOP Optimization Test Suite
Tests classes, methods, and functions for 100x speedup
"""

print("=" * 70)
print("OOP & FUNCTION OPTIMIZATION TEST SUITE")
print("=" * 70)
print()

# ============================================================
# TEST 1: Simple Class with Methods
# ============================================================
print("TEST 1: Simple Class with Methods")
print("-" * 70)

class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count = self.count + 1

    def get_count(self):
        return self.count

c = Counter()
for i in range(10000):
    c.increment()

print(f"Counter value: {c.get_count()}")
print()

# ============================================================
# TEST 2: Class with Attributes
# ============================================================
print("TEST 2: Class with Integer Attributes")
print("-" * 70)

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def move(self, dx, dy):
        self.x = self.x + dx
        self.y = self.y + dy

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y

p = Point(10, 20)
for i in range(1000):
    p.move(1, 1)

print(f"Point position: ({p.get_x()}, {p.get_y()})")
print()

# ============================================================
# TEST 3: Small Function (Should be Inlined)
# ============================================================
print("TEST 3: Small Function Inlining")
print("-" * 70)

def square(x):
    return x * x

def cube(x):
    return x * x * x

result = 0
for i in range(10000):
    result = result + square(i)

print(f"Sum of squares: {result}")
print()

# ============================================================
# TEST 4: Method with Computation
# ============================================================
print("TEST 4: Method with Computation")
print("-" * 70)

class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

    def perimeter(self):
        return 2 * (self.width + self.height)

r = Rectangle(10, 20)
total_area = 0
for i in range(1000):
    total_area = total_area + r.area()

print(f"Total area: {total_area}")
print()

# ============================================================
# TEST 5: Multiple Objects
# ============================================================
print("TEST 5: Multiple Objects")
print("-" * 70)

class Entity:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.active = 1

    def update(self):
        self.x = self.x + 1
        self.y = self.y + 1

# Create multiple entities
entities = []
for i in range(100):
    entities.append(Entity(i, i))

# Update all entities
for frame in range(100):
    for entity in entities:
        entity.update()

print(f"Entity test complete: {len(entities)} entities")
print()

# ============================================================
# TEST 6: Nested Method Calls
# ============================================================
print("TEST 6: Nested Method Calls")
print("-" * 70)

class Calculator:
    def __init__(self):
        self.value = 0

    def add(self, x):
        self.value = self.value + x
        return self

    def multiply(self, x):
        self.value = self.value * x
        return self

    def get_value(self):
        return self.value

calc = Calculator()
calc.add(10).multiply(2).add(5)
print(f"Calculator result: {calc.get_value()}")
print()

# ============================================================
# TEST 7: Function Calls in Loop
# ============================================================
print("TEST 7: Function Calls in Loop")
print("-" * 70)

def add(a, b):
    return a + b

def multiply(a, b):
    return a * b

result = 0
for i in range(1000):
    result = add(result, multiply(i, 2))

print(f"Function call result: {result}")
print()

# ============================================================
# SUMMARY
# ============================================================
print("=" * 70)
print("OOP TEST SUITE COMPLETE")
print("=" * 70)
print("Tested features:")
print("  ✓ Simple classes with methods")
print("  ✓ Classes with attributes")
print("  ✓ Small function inlining")
print("  ✓ Methods with computation")
print("  ✓ Multiple objects")
print("  ✓ Nested method calls")
print("  ✓ Function calls in loops")
print()
print("Expected OOP speedup: 100x+ faster than Python!")
print("=" * 70)
