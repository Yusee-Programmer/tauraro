# Test if Tauraro class instantiation returns None

print("Testing Tauraro class instantiation...")

class SimpleClass:
    def __init__(self, value):
        print(f"In __init__, value={value}")
        self.value = value
        print(f"Exiting __init__, self.value={self.value}")

print("Creating instance...")
obj = SimpleClass(42)
print(f"Instance created: {obj}")
print(f"Type: {type(obj)}")

if obj == None:
    print("ERROR: Instance is None!")
else:
    print(f"SUCCESS: Instance value = {obj.value}")
