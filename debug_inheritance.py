# Debug inheritance to understand the issue

print("=== Debug Inheritance ===")

class A:
    def __init__(self, x):
        print("A.__init__ called with x =", x)
        self.x = x
        print("A.__init__ finished, self.x =", self.x)

class B(A):
    def __init__(self, x, y):
        print("B.__init__ called with x =", x, "y =", y)
        print("Before calling A.__init__, self.x =", getattr(self, 'x', 'NOT_SET'))
        A.__init__(self, x)  # Call parent constructor
        print("After calling A.__init__, self.x =", getattr(self, 'x', 'NOT_SET'))
        self.y = y
        print("B.__init__ finished, self.x =", getattr(self, 'x', 'NOT_SET'), "self.y =", self.y)

# Create instance
print("Creating B instance")
b = B(10, 20)

# Try to access the field set in parent constructor
print("Final check: b.x =", getattr(b, 'x', 'NOT_SET'))
print("Final check: b.y =", getattr(b, 'y', 'NOT_SET'))

print("=== Debug Completed ===")