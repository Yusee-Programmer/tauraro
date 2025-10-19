# Simple class test without string concatenation

class Person:
    def __init__(self, name):
        self.name = name

person = Person("Alice")
print(person.name)