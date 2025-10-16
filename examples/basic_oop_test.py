# Basic OOP Test in Tauraro

print("=== Basic OOP Test ===")

# Simple class definition
class Person:
    def __init__(self, name):
        self.name = name

# Create instance
person = Person("Alice")
print("Person name: " + person.name)

# Simple inheritance
class Student(Person):
    def __init__(self, name, grade):
        Person.__init__(self, name)
        self.grade = grade

# Create instance of subclass
student = Student("Bob", "10")  # Changed to string to avoid type conversion issues
print("Student name: " + student.name)
print("Student grade: " + student.grade)

print("=== Basic OOP Test Completed ===")