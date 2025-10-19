# Debug test for inheritance
print("Creating classes...")

class A:
    def __init__(self, x):
        self.x = x
        print("A.__init__ called with x =", x)

class B(A):
    def __init__(self, x, y):
        print("B.__init__ called with x =", x, "y =", y)
        # This is the critical call that was failing
        A.__init__(self, x)
        self.y = y
        print("B.__init__ completed")

print("Classes created")
print("Creating B instance...")
b = B(1, 2)
print("Instance created:", b)
print("b.x =", b.x)
print("b.y =", b.y)