# Simple inheritance test
class A:
    def __init__(self, x):
        self.x = x

class B(A):
    def __init__(self, x, y):
        A.__init__(self, x)
        self.y = y

# Test
a = A(1)
b = B(2, 3)
print("Success")