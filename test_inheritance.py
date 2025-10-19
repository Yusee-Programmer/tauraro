# Test inheritance field persistence issue

print("=== Testing Inheritance Field Persistence ===")

class A:
    def __init__(self, x):
        self.x = x
        print("A.__init__ called with x =", x)

class B(A):
    def __init__(self, x, y):
        A.__init__(self, x)  # Call parent constructor
        self.y = y
        print("B.__init__ called with x =", x, "y =", y)

# Create instance
b = B(10, 20)

# Try to access the field set in parent constructor
print("b.x =", b.x)  # This should print 10
print("b.y =", b.y)  # This should print 20

print("=== Test Completed ===")