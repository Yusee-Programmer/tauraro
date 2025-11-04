# Test parameter passing in class methods

class TestClass:
    def test_method(self, a, b, c):
        print(f"In test_method:")
        print(f"  a = {a}")
        print(f"  b = {b}")
        print(f"  c = {c}")
        return a + b + c

obj = TestClass()
result = obj.test_method(10, 20, 30)
print(f"Result: {result}")
