# Debug test

class Person:
    def __init__(self, name):
        print("In __init__ with name:", name)
        self.name = name
        print("Set self.name to:", name)

person = Person("Alice")
print("person.name:", person.name)