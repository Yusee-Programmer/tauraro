# Attribute access test

class Person:
    def __init__(self, name):
        self.name = name

person = Person("Alice")
print("Name attribute exists:", hasattr(person, 'name'))
print("Name value:", person.name)