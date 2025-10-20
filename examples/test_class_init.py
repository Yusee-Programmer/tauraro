# Test if __init__ is stored in class methods
class A:
    def __init__(self):
        print("A.__init__")
        self.a = 1

# Try to create an instance of A
a = A()
print("a.a =", a.a)
