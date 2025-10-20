# Debug MRO

class A:
    def greet(self):
        return "Hello from A"

class B(A):
    def method(self):
        return "B"

class C(A):
    def method(self):
        return "C"

class D(B, C):
    pass

d = D()

# Test if we can call method from B
print("d.method() =", d.method())

# Try calling greet - this should work via MRO
try:
    result = d.greet()
    print("d.greet() =", result)
except Exception as e:
    print("ERROR calling d.greet():", e)

# Let's also test creating an instance of B directly
b = B()
print("b.greet() =", b.greet())
