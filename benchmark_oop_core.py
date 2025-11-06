"""
Core OOP Performance Benchmark
Tests: object creation, method calls, field access
"""

# ============================================================
# BENCHMARK 1: Object Creation and Method Calls
# ============================================================
print("BENCHMARK 1: Object Creation + Method Calls")
print("-" * 70)

class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count = self.count + 1

counter = Counter()
for i in range(100000):
    counter.increment()

print(f"Counter value: {counter.count}")
print()

# ============================================================
# BENCHMARK 2: Multiple Objects with Field Access
# ============================================================
print("BENCHMARK 2: Multiple Objects with Field Access")
print("-" * 70)

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def move(self, dx, dy):
        self.x = self.x + dx
        self.y = self.y + dy

points = []
for i in range(1000):
    points.append(Point(i, i))

for i in range(100):
    for point in points:
        point.move(1, 1)

print(f"Total points: {len(points)}")
print(f"First point: ({points[0].x}, {points[0].y})")
print()

# ============================================================
# BENCHMARK 3: Method with Computation
# ============================================================
print("BENCHMARK 3: Method with Computation")
print("-" * 70)

class Rectangle:
    def __init__(self, w, h):
        self.width = w
        self.height = h

    def area(self):
        return self.width * self.height

rect = Rectangle(10, 20)
total = 0
for i in range(10000):
    total = total + rect.area()

print(f"Total area: {total}")
print()

# ============================================================
# SUMMARY
# ============================================================
print("=" * 70)
print("BENCHMARK COMPLETE")
print("=" * 70)
