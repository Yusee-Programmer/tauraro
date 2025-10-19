# Simple inheritance test
print("Creating classes...")

class A:
    def __init__(self, x):
        self.x = x
        print("A.__init__ called with x =", x)

class B(A):
    def __init__(self, x, y):
        print("B.__init__ called with x =", x, "y =", y)
        # Call parent constructor
        A.__init__(self, x)
        self.y = y
        print("B.__init__ completed")

# Test the inheritance
print("Creating B instance...")
b = B(1, 2)
print("Instance created successfully!")
print("Test completed.")