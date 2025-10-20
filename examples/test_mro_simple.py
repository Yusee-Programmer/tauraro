# Simple MRO test

print("Test: Simple inheritance chain")
class A:
    def greet(self):
        return "Hello from A"

class B(A):
    pass

b = B()
print("B.greet():", b.greet())
print()

print("Test: Multiple inheritance")
class C:
    def method_c(self):
        return "C"

class D(A, C):
    pass

d = D()
print("D.greet() from A:", d.greet())
print("D.method_c() from C:", d.method_c())
