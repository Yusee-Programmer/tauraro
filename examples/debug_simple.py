# Simple debug test

class Person:
    def __init__(self, name):
        print("In __init__ with name:", name)
        self.name = name
        print("Set self.name to:", name)

print("Creating person...")
person = Person("Alice")
print("Created person")
print("person.name:", person.name)