# Simple property test

print("Test: Simple property")

class Circle:
    def __init__(self, radius):
        self._radius = radius

    def get_radius(self):
        print("get_radius called")
        return self._radius

    radius = property(get_radius)

c = Circle(5)
print("Created circle")
print("Getting radius...")
r = c.radius
print("Radius:", r)
