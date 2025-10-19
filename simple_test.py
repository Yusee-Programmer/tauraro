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

# Simple test to understand the issue

class A:
    def __init__(self, x):
        self.x = x
        print("A.__init__ called with x =", x)

a = A(10)
print("a.x =", a.x)

print("=== Test Completed ===")