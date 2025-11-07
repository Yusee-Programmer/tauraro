# Class benchmark
# Tests attribute access, method calls, and object creation

print("=== Class Benchmark ===")

# Simple class with attributes
class Counter:
    def __init__(self, start):
        self.value = start

    def increment(self):
        self.value = self.value + 1
        return self.value

    def add(self, n):
        self.value = self.value + n
        return self.value

# Test object creation and method calls
c = Counter(0)
for i in range(50000):
    c.increment()
print(f"After 50K increments: {c.value}")

# Test method with parameters
c2 = Counter(0)
for i in range(10000):
    c2.add(i)
print(f"After adding 0-9999: {c2.value}")

# Test attribute access
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance_from_origin(self):
        # Simple distance approximation
        return self.x + self.y

p = Point(10, 20)
total_distance = 0
for i in range(10000):
    p.x = i
    p.y = i * 2
    total_distance = p.distance_from_origin()
print(f"Final distance: {total_distance}")

# Multiple objects
class Value:
    def __init__(self, val):
        self.val = val

    def double(self):
        return self.val * 2

total = 0
for i in range(5000):
    v = Value(i)
    total = total + v.double()
print(f"Sum of doubles: {total}")

print("Class benchmarks complete!")
