"""
OOP Performance Benchmark Suite - Simple Version
Core OOP tests: objects, methods, fields
"""

import time

print("=" * 70)
print("TAURARO OOP PERFORMANCE BENCHMARK")
print("=" * 70)
print()

# ============================================================
# BENCHMARK 1: Object Creation (10,000 objects)
# ============================================================
print("BENCHMARK 1: Object Creation (10,000 objects)")
print("-" * 70)

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

start_time = time.time()
points = []
for i in range(10000):
    p = Point(i, i * 2)
    points.append(p)
end_time = time.time()

print(f"Created {len(points)} Point objects")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# BENCHMARK 2: Method Calls (1,000,000 calls)
# ============================================================
print("BENCHMARK 2: Method Calls (1,000,000 calls)")
print("-" * 70)

class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count = self.count + 1

    def get_count(self):
        return self.count

counter = Counter()
start_time = time.time()
for i in range(1000000):
    counter.increment()
end_time = time.time()

print(f"Final count: {counter.get_count()}")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# BENCHMARK 3: Field Access (1,000,000 reads/writes)
# ============================================================
print("BENCHMARK 3: Field Access (1,000,000 reads/writes)")
print("-" * 70)

class Data:
    def __init__(self):
        self.value = 0

data = Data()
start_time = time.time()
for i in range(1000000):
    data.value = i
    temp = data.value
end_time = time.time()

print(f"Final value: {data.value}")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# BENCHMARK 4: Method with Computation (100,000 calls)
# ============================================================
print("BENCHMARK 4: Method with Computation (100,000 calls)")
print("-" * 70)

class Rectangle:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

rect = Rectangle(10, 20)
total_area = 0
start_time = time.time()
for i in range(100000):
    total_area = total_area + rect.area()
end_time = time.time()

print(f"Total area: {total_area}")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# BENCHMARK 5: Multiple Objects (10,000 objects × 100 updates)
# ============================================================
print("BENCHMARK 5: Multiple Objects (10,000 × 100 updates)")
print("-" * 70)

class Entity:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def move(self, dx, dy):
        self.x = self.x + dx
        self.y = self.y + dy

entities = []
for i in range(10000):
    entities.append(Entity(i, i))

start_time = time.time()
for frame in range(100):
    for entity in entities:
        entity.move(1, 1)
end_time = time.time()

print(f"Updated {len(entities)} entities 100 times")
print(f"Final position of first entity: ({entities[0].x}, {entities[0].y})")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# BENCHMARK 6: Method Chaining (100,000 chains)
# ============================================================
print("BENCHMARK 6: Method Chaining (100,000 chains)")
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

calc = Calculator()
start_time = time.time()
for i in range(100000):
    calc.value = 0
    calc.add(10).multiply(2).add(5)
end_time = time.time()

print(f"Completed 100,000 method chains")
print(f"Final value: {calc.value}")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# SUMMARY
# ============================================================
print("=" * 70)
print("BENCHMARK COMPLETE")
print("=" * 70)
