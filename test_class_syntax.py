# Test proper class syntax (lowercase)
class MyClass:
    def __init__(self):
        self.value = 42

    def get_value(self):
        return self.value

# Create instance
obj = MyClass()
print("Object created:", obj)
print("Value:", obj.get_value())

# Test class with pass
class EmptyClass:
    pass

print("Empty class created successfully")
