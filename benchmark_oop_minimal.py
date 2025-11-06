# Minimal OOP Benchmark - Pure computation

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def compute(self):
        return self.x * self.x + self.y * self.y

class Vector(Point):
    def __init__(self, x, y, z):
        super().__init__(x, y)
        self.z = z

    def compute(self):
        return self.x * self.x + self.y * self.y + self.z * self.z

# Main computation
total = 0
i = 0
while i < 5000000:
    p = Point(i, i + 1)
    total = total + p.compute()
    i = i + 1

print("Point computation done")

total_vec = 0
i = 0
while i < 5000000:
    v = Vector(i, i + 1, i + 2)
    total_vec = total_vec + v.compute()
    i = i + 1

print("Vector computation done")
print("Results computed")
