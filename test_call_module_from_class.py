# Test calling module function from class method

def factory(a, b, c):
    print(f"factory: a={a}, b={b}, c={c}")
    return a + b + c

class TestClass:
    def call_factory(self, x, y, z):
        print(f"call_factory: x={x}, y={y}, z={z}")
        result = factory(x, y, z)
        print(f"factory returned: {result}")
        return result

obj = TestClass()
result = obj.call_factory(10, 20, 30)
print(f"Final result: {result}")
