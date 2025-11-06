# OOP Benchmark - Pure computation version for C compilation

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance_from_origin(self):
        dx = self.x * self.x
        dy = self.y * self.y
        return dx + dy

    def add(self, other):
        result = Point(self.x + other.x, self.y + other.y)
        return result

class Vector(Point):
    def __init__(self, x, y, z):
        super().__init__(x, y)
        self.z = z

    def magnitude(self):
        dx = self.x * self.x
        dy = self.y * self.y
        dz = self.z * self.z
        return dx + dy + dz

    def dot(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z

# Benchmark 1: Object creation and method calls
print("=== Benchmark 1: Object Creation + Method Calls ===")
total = 0
i = 0
while i < 1000000:
    p = Point(i, i + 1)
    total = total + p.distance_from_origin()
    i = i + 1
print("Total distance: " + str(total))

# Benchmark 2: Inheritance with super()
print("\n=== Benchmark 2: Inheritance + super() ===")
total_mag = 0
i = 0
while i < 1000000:
    v = Vector(i, i + 1, i + 2)
    total_mag = total_mag + v.magnitude()
    i = i + 1
print("Total magnitude: " + str(total_mag))

# Benchmark 3: Object interaction
print("\n=== Benchmark 3: Method Calls with Arguments ===")
dot_sum = 0
v1 = Vector(1, 2, 3)
i = 0
while i < 1000000:
    v2 = Vector(i, i + 1, i + 2)
    dot_sum = dot_sum + v1.dot(v2)
    i = i + 1
print("Dot product sum: " + str(dot_sum))

# Benchmark 4: Attribute access
print("\n=== Benchmark 4: Heavy Attribute Access ===")
sum_x = 0
sum_y = 0
sum_z = 0
i = 0
while i < 1000000:
    v = Vector(i, i + 1, i + 2)
    sum_x = sum_x + v.x
    sum_y = sum_y + v.y
    sum_z = sum_z + v.z
    i = i + 1
print("Sum X: " + str(sum_x))
print("Sum Y: " + str(sum_y))
print("Sum Z: " + str(sum_z))

print("\n=== All Benchmarks Complete ===")
