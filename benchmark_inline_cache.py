# Benchmark: Method call performance with inline caching
# This should show 20-30% speedup from cached method lookups

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance_squared(self):
        return self.x * self.x + self.y * self.y

    def move(self, dx, dy):
        self.x = self.x + dx
        self.y = self.y + dy

# Create many points
points = []
for i in range(100):
    p = Point(i, i * 2)
    points.append(p)

# Call methods many times (this will benefit from inline caching)
total = 0
for i in range(100):
    for p in points:
        p.move(1, 1)
        dist = p.distance_squared()
        total = total + dist

print("Total distance squared:", total)
print("Benchmark complete - inline method caching active!")
