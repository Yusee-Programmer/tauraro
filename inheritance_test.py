# Simple inheritance test for Tauraro

print("=== Simple Inheritance Test ===")

# Basic class
class Person:
    def __init__(self, name):
        self.name = name
    
    def introduce(self):
        return f"Hi, I'm {self.name}"

# Derived class
class Student(Person):
    def __init__(self, name, grade):
        # Call parent constructor directly without super()
        Person.__init__(self, name)
        self.grade = grade
    
    def introduce(self):
        return f"Hi, I'm {self.name}, a {self.grade} grade student"

# Test instantiation
person = Person("Alice")
student = Student("Bob", "10th")

print(person.introduce())
print(student.introduce())

print("=== Test Completed ===")