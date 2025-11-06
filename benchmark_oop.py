#!/usr/bin/env python3
# OOP Benchmark - Tests object creation, method calls, inheritance, and attributes

import time

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance_from_origin(self):
        return (self.x * self.x + self.y * self.y) ** 0.5

    def add(self, other):
        return Point(self.x + other.x, self.y + other.y)

class Vector(Point):
    def __init__(self, x, y, z):
        super().__init__(x, y)
        self.z = z

    def magnitude(self):
        return (self.x * self.x + self.y * self.y + self.z * self.z) ** 0.5

    def dot(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z

# Benchmark 1: Object creation
print("=== Benchmark 1: Object Creation ===")
start = time.time()
points = []
for i in range(100000):
    p = Point(i, i + 1)
    points.append(p)
end = time.time()
print(f"Created 100,000 Point objects in {(end - start)*1000:.2f}ms")

# Benchmark 2: Method calls
print("\n=== Benchmark 2: Method Calls ===")
start = time.time()
total = 0.0
for p in points:
    total = total + p.distance_from_origin()
end = time.time()
print(f"100,000 method calls in {(end - start)*1000:.2f}ms")
print(f"Total distance: {total}")

# Benchmark 3: Attribute access
print("\n=== Benchmark 3: Attribute Access ===")
start = time.time()
sum_x = 0
sum_y = 0
for p in points:
    sum_x = sum_x + p.x
    sum_y = sum_y + p.y
end = time.time()
print(f"200,000 attribute accesses in {(end - start)*1000:.2f}ms")
print(f"Sum X: {sum_x}, Sum Y: {sum_y}")

# Benchmark 4: Inheritance and super()
print("\n=== Benchmark 4: Inheritance with super() ===")
start = time.time()
vectors = []
for i in range(50000):
    v = Vector(i, i + 1, i + 2)
    vectors.append(v)
end = time.time()
print(f"Created 50,000 Vector objects (with super()) in {(end - start)*1000:.2f}ms")

# Benchmark 5: Complex method calls
print("\n=== Benchmark 5: Complex Method Calls ===")
start = time.time()
total_mag = 0.0
for v in vectors:
    total_mag = total_mag + v.magnitude()
end = time.time()
print(f"50,000 magnitude calculations in {(end - start)*1000:.2f}ms")
print(f"Total magnitude: {total_mag}")

# Benchmark 6: Object interaction
print("\n=== Benchmark 6: Object Interaction ===")
start = time.time()
dot_sum = 0.0
v1 = Vector(1, 2, 3)
for i in range(50000):
    v2 = Vector(i, i + 1, i + 2)
    dot_sum = dot_sum + v1.dot(v2)
end = time.time()
print(f"50,000 dot products in {(end - start)*1000:.2f}ms")
print(f"Dot product sum: {dot_sum}")

print("\n=== All Benchmarks Complete ===")
