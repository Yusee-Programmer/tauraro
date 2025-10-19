# Debug test for inheritance
print("Creating classes...")

class A:
    def __init__(self, x):
        self.x = x
        print("A.__init__ called with x =", x)

print("Class A created")
print("A =", A)
print("A.__init__ =", A.__init__)

print("Creating instance...")
a = A(1)
print("Instance created:", a)
print("a.x =", a.x)