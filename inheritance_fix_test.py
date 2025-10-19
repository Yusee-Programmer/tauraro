# Test inheritance attribute access fix

class Person:
    def __init__(self, name):
        self.name = name
    
    def introduce(self):
        return f"Hi, I'm {self.name}"

class Student(Person):
    def __init__(self, name, grade):
        # Call parent constructor
        Person.__init__(self, name)
        self.grade = grade
    
    def introduce(self):
        return f"Hi, I'm {self.name}, a {self.grade} grade student"

# Test instantiation
person = Person("Alice")
student = Student("Bob", "10th")

print("Person says:", person.introduce())
print("Student says:", student.introduce())

# Test attribute access
print("Person name:", person.name)
print("Student name:", student.name)
print("Student grade:", student.grade)

print("Inheritance tests completed!")