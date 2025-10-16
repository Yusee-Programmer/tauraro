# Test variable assignment

class Person:
    def __init__(self, name):
        self.name = name

# Create instance
person = Person("Alice")
print("person variable created")

# Try to access the name
print("person.name:", person.name)