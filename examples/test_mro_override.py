# Test MRO with method overriding

print("Test: MRO with method override")
class A:
    def method(self):
        return "A"

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
print("D.method() (should be B):", d.method())
try:
    result = d.greet()
    print("D.greet() (should be from A):", result)
except Exception as e:
    print("ERROR:", str(e))
