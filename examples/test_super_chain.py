# Simple test for super() chain
class A:
    def __init__(self):
        print("A.__init__")
        self.a = 1

class B(A):
    def __init__(self):
        print("B.__init__ calling super()")
        super().__init__()
        print("B.__init__ after super()")
        self.b = 2

class C(B):
    def __init__(self):
        print("C.__init__ calling super()")
        super().__init__()
        print("C.__init__ after super()")
        self.c = 3

c = C()
print("c.a =", c.a)
print("c.b =", c.b)
print("c.c =", c.c)
