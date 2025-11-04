# Test class instantiation from within another class method

print("Testing nested class instantiation...")

class Inner:
    def __init__(self, value):
        print(f"Inner.__init__: value={value}")
        self.value = value
        print(f"Exiting Inner.__init__")

class Outer:
    def __init__(self):
        print("Outer.__init__")
        self.inners = []

    def create_inner(self, value):
        print(f"Outer.create_inner: value={value}")
        print(f"About to create Inner...")
        inner = Inner(value)
        print(f"Inner created: {inner}")
        if inner == None:
            print("ERROR: inner is None!")
        else:
            print(f"SUCCESS: inner.value = {inner.value}")
            self.inners.append(inner)
        return inner

print("Creating Outer...")
outer = Outer()
print(f"Outer created: {outer}")

print("\nCreating Inner from Outer...")
inner1 = outer.create_inner(42)
print(f"Returned: {inner1}")
