print("Testing bitwise OR in class method...")

class TestClass:
    def __init__(self):
        print("In __init__")
        x = 0x10000000 | 0x00CF0000
        print(f"BitOr result: {x}")
        print(f"Type: {type(x)}")
        self.value = x
        if x == None:
            print("ERROR: x is None!")
        else:
            print(f"SUCCESS: x = {x}")

    def test_method(self):
        print("In test_method")
        y = 0x40000000 | 0x10000000
        print(f"BitOr result: {y}")
        print(f"Type: {type(y)}")
        if y == None:
            print("ERROR: y is None!")
        else:
            print(f"SUCCESS: y = {y}")
        return y

print("Creating instance...")
obj = TestClass()
print(f"obj.value = {obj.value}")

print("\nCalling test_method...")
result = obj.test_method()
print(f"test_method returned: {result}")
