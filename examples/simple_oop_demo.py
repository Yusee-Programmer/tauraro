# Simple OOP Features Demo in Tauraro

print("=== Simple OOP Features Demo ===\n")

# 1. Basic Class with Constructor and Methods
print("1. Basic Class")
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def introduce(self):
        return "Hi, I'm " + self.name + " and I'm " + str(self.age) + " years old."

person = Person("Alice", 30)
print(person.introduce())
print()

# 2. Inheritance
print("2. Inheritance")
class Student(Person):
    def __init__(self, name, age, grade):
        Person.__init__(self, name, age)
        self.grade = grade
    
    def introduce(self):
        return Person.introduce(self) + " I'm in grade " + str(self.grade) + "."

student = Student("Bob", 16, 10)
print(student.introduce())
print()

# 3. Class with Properties
print("3. Properties")
class Temperature:
    def __init__(self, celsius):
        self._celsius = celsius
    
    def get_celsius(self):
        return self._celsius
    
    def set_celsius(self, value):
        if value < -273.15:
            raise ValueError("Temperature cannot be below absolute zero")
        self._celsius = value
    
    celsius = property(get_celsius, set_celsius)
    
    def get_fahrenheit(self):
        return self._celsius * 9.0/5.0 + 32
    
    def set_fahrenheit(self, value):
        self.celsius = (value - 32) * 5.0/9.0
    
    fahrenheit = property(get_fahrenheit, set_fahrenheit)

temp = Temperature(25)
print("Temperature: " + str(temp.celsius) + "°C, " + str(temp.fahrenheit) + "°F")
temp.fahrenheit = 86
print("Temperature: " + str(temp.celsius) + "°C, " + str(temp.fahrenheit) + "°F")
print()

# 4. Operator Overloading
print("4. Operator Overloading")
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)
    
    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar)
    
    def __str__(self):
        return "Vector(" + str(self.x) + ", " + str(self.y) + ")"

v1 = Vector(2, 3)
v2 = Vector(1, 4)
v3 = v1 + v2
v4 = v1 * 3
print("v1: " + str(v1))
print("v2: " + str(v2))
print("v1 + v2: " + str(v3))
print("v1 * 3: " + str(v4))
print()

# 5. Custom Iterator
print("5. Custom Iterator")
class CountDown:
    def __init__(self, start):
        self.start = start
    
    def __iter__(self):
        return self
    
    def __next__(self):
        if self.start <= 0:
            raise StopIteration
        self.start = self.start - 1
        return self.start + 1

countdown = CountDown(3)
print("Countdown from 3:")
for num in countdown:
    print(num)
print()

print("=== Simple OOP Features Demo Completed ===")