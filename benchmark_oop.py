"""
OOP Performance Benchmark Suite
Comprehensive tests comparing Python vs Tauraro compiled C
"""

import time

print("=" * 70)
print("TAURARO OOP PERFORMANCE BENCHMARK SUITE")
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
# BENCHMARK 2: Method Calls in Tight Loop (1,000,000 calls)
# ============================================================
print("BENCHMARK 2: Method Calls in Tight Loop (1,000,000 calls)")
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

    def perimeter(self):
        return 2 * (self.width + self.height)

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
# BENCHMARK 5: Multiple Objects with Updates (10,000 objects × 100 updates)
# ============================================================
print("BENCHMARK 5: Multiple Objects with Updates (10,000 × 100)")
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

    def get_value(self):
        return self.value

start_time = time.time()
for i in range(100000):
    calc = Calculator()
    calc.add(10).multiply(2).add(5)
end_time = time.time()

print(f"Completed {100000} method chains")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# BENCHMARK 7: Complex Object Graph (1,000 objects × 1,000 operations)
# ============================================================
print("BENCHMARK 7: Complex Object Graph (1,000 × 1,000)")
print("-" * 70)

class Node:
    def __init__(self, value):
        self.value = value
        self.next = None

    def set_next(self, node):
        self.next = node

    def get_value(self):
        return self.value

nodes = []
for i in range(1000):
    nodes.append(Node(i))

start_time = time.time()
# Link nodes
for i in range(len(nodes) - 1):
    nodes[i].set_next(nodes[i + 1])

# Traverse and accumulate
total = 0
for i in range(1000):
    node = nodes[0]
    while node is not None:
        total = total + node.get_value()
        node = node.next
        if node is nodes[0]:  # Prevent infinite loop
            break
end_time = time.time()

print(f"Traversed linked list 1000 times")
print(f"Time: {(end_time - start_time) * 1000:.2f}ms")
print()

# ============================================================
# SUMMARY
# ============================================================
print("=" * 70)
print("BENCHMARK COMPLETE")
print("=" * 70)
print("Run with Python: python benchmark_oop.py")
print("Run with Tauraro: ./target/release/tauraro.exe run benchmark_oop.py")
print("Compile to C: ./target/release/tauraro.exe compile benchmark_oop.py --backend c")
print("=" * 70)
