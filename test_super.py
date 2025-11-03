# Test super()

class Base:
    def __init__(self, x):
        self.x = x
        print(f"Base init: x={x}")

class Derived(Base):
    def __init__(self, x, y):
        super().__init__(x)
        self.y = y
        print(f"Derived init: y={y}")

d = Derived(10, 20)
print(f"d.x = {d.x}, d.y = {d.y}")
