# Test 3: Object-Oriented Programming
print("=== Test 3: OOP ===")

# Simple class
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def greet(self):
        print("Hello, my name is", self.name)
        print("I am", self.age, "years old")

# Create instance
person = Person("Alice", 30)
person.greet()

# Access attributes
print("Person name:", person.name)
print("Person age:", person.age)

# Modify attributes
person.age = 31
print("Updated age:", person.age)

print("\n=== Test 3 Complete ===")
