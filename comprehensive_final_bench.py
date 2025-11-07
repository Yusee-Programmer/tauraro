# Comprehensive Final Benchmark
# Tests arithmetic, functions, and classes

print("=" * 50)
print("COMPREHENSIVE TAURARO PERFORMANCE BENCHMARK")
print("=" * 50)
print()

print("1. ARITHMETIC OPERATIONS")
print("-" * 50)
a = 0
b = 0
c = 0
for i in range(1000000):
    a = i + 1
    b = a - 5
    c = b + 10
print(f"   Add/Sub (1M ops): COMPLETE")

total = 0
for i in range(1000000):
    total = total + i
print(f"   Loops (1M iters): {total}")

result_mul = 100
for i in range(100000):
    result_mul = (result_mul * 2) / 2
print(f"   Mul/Div (100K ops): COMPLETE")
print()

print("2. FUNCTION CALLS")
print("-" * 50)
def add_one(x):
    return x + 1

result = 0
for i in range(100000):
    result = add_one(result)
print(f"   Simple calls (100K): {result}")

def add_three(a, b, c):
    return a + b + c

total = 0
for i in range(50000):
    total = add_three(i, i + 1, i + 2)
print(f"   Multi-param (50K): {total}")
print()

print("3. CLASS OPERATIONS")
print("-" * 50)

class Counter:
    def __init__(self, start):
        self.value = start

    def increment(self):
        self.value = self.value + 1
        return self.value

c = Counter(0)
for i in range(50000):
    c.increment()
print(f"   Method calls (50K): {c.value}")

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

p = Point(0, 0)
for i in range(10000):
    p.x = i
    p.y = i * 2
print(f"   Attribute access (10K): {p.x}, {p.y}")
print()

print("=" * 50)
print("ALL BENCHMARKS COMPLETE!")
print("=" * 50)
