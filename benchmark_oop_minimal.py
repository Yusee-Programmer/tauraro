"""
Minimal OOP Benchmark - Pure class operations only
"""

# Test 1: Counter with method calls
class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count = self.count + 1

counter = Counter()
i = 0
while i < 100000:
    counter.increment()
    i = i + 1

print(counter.count)

# Test 2: Point with field access
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def move(self, dx, dy):
        self.x = self.x + dx
        self.y = self.y + dy

points = []
i = 0
while i < 1000:
    p = Point(i, i)
    points.append(p)
    i = i + 1

i = 0
while i < 100:
    j = 0
    while j < 1000:
        point = points[j]
        point.move(1, 1)
        j = j + 1
    i = i + 1

first_point = points[0]
print(first_point.x)
print(first_point.y)

# Test 3: Rectangle with computation
class Rectangle:
    def __init__(self, w, h):
        self.width = w
        self.height = h

    def area(self):
        return self.width * self.height

rect = Rectangle(10, 20)
total = 0
i = 0
while i < 10000:
    total = total + rect.area()
    i = i + 1

print(total)
