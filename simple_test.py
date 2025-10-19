# Simple OOP Test for Tauraro

print("=== Simple OOP Test ===")

# Basic class definition and instantiation
class Person:
    def __init__(self, name):
        self.name = name
    
    def introduce(self):
        return f"Hi, I'm {self.name}"

# Class instantiation and method calling
person1 = Person("Alice")
print(person1.introduce())

print("=== Test Completed ===")