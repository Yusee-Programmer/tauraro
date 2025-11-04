# Minimal test for BitOr bug

print("Module level BitOr:")
a = 5 | 3
print(f"a = {a}")

class Test:
    def __init__(self):
        print("Class method BitOr:")
        b = 5 | 3
        print(f"b = {b}")
        self.value = b

obj = Test()
print(f"obj.value = {obj.value}")
