# Comprehensive OOP Test for Tauraro
# Tests all OOP features including super(), dunder methods, method overriding, properties, etc.

print("=== Comprehensive OOP Test Suite ===\n")

# Test 1: Basic Class and __init__
print("Test 1: Basic Class and __init__")
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def greet(self):
        return "Hello, I'm " + self.name

p = Person("Alice", 30)
print("Name: " + p.name)
print("Age: " + str(p.age))
print("Greet: " + p.greet())
print("PASS\n")

# Test 2: Method Overriding (without super)
print("Test 2: Method Overriding (without super)")
class Student(Person):
    def __init__(self, name, age, grade):
        Person.__init__(self, name, age)
        self.grade = grade

    def greet(self):
        return "Hi, I'm " + self.name + ", a student in grade " + str(self.grade)

s = Student("Bob", 16, 10)
print("Student greet: " + s.greet())
print("PASS\n")

# Test 3: super() with single inheritance
print("Test 3: super() with single inheritance")
class Teacher(Person):
    def __init__(self, name, age, subject):
        super().__init__(name, age)
        self.subject = subject

    def greet(self):
        return super().greet() + " and I teach " + self.subject

t = Teacher("Carol", 35, "Math")
print("Teacher name: " + t.name)
print("Teacher age: " + str(t.age))
print("Teacher subject: " + t.subject)
print("Teacher greet: " + t.greet())
print("PASS\n")

# Test 4: Property getter (read-only property)
print("Test 4: Property getter (read-only property)")
class Circle:
    def __init__(self, radius):
        self._radius = radius

    def get_radius(self):
        return self._radius

    def get_area(self):
        return 3.14159 * self._radius * self._radius

    radius = property(get_radius)
    area = property(get_area)

c = Circle(5)
print("Circle radius: " + str(c.radius))
print("Circle area: " + str(c.area))
print("PASS\n")

# Test 5: Property with getter and setter
print("Test 5: Property with getter and setter")
class Temperature:
    def __init__(self, celsius):
        self._celsius = celsius

    def get_celsius(self):
        return self._celsius

    def set_celsius(self, value):
        self._celsius = value

    celsius = property(get_celsius, set_celsius)

temp = Temperature(25)
print("Initial temp: " + str(temp.celsius))
temp.celsius = 30
print("Updated temp: " + str(temp.celsius))
print("PASS\n")

# Test 6: Dunder method - __str__
print("Test 6: Dunder method - __str__")
class Book:
    def __init__(self, title, author):
        self.title = title
        self.author = author

    def __str__(self):
        return "Book: " + self.title + " by " + self.author

book = Book("1984", "George Orwell")
print(str(book))
print("PASS\n")

# Test 7: Dunder method - __repr__
print("Test 7: Dunder method - __repr__")
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __repr__(self):
        return "Point(" + str(self.x) + ", " + str(self.y) + ")"

pt = Point(3, 4)
print(repr(pt))
print("PASS\n")

# Test 8: Dunder method - __add__ (operator overloading)
print("Test 8: Dunder method - __add__ (operator overloading)")
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __str__(self):
        return "Vector(" + str(self.x) + ", " + str(self.y) + ")"

v1 = Vector(1, 2)
v2 = Vector(3, 4)
v3 = v1 + v2
print("v1: " + str(v1))
print("v2: " + str(v2))
print("v1 + v2: " + str(v3))
print("PASS\n")

# Test 9: Dunder method - __eq__ (equality comparison)
print("Test 9: Dunder method - __eq__ (equality comparison)")
class Person2:
    def __init__(self, name):
        self.name = name

    def __eq__(self, other):
        return self.name == other.name

p1 = Person2("Alice")
p2 = Person2("Alice")
p3 = Person2("Bob")
print("p1 == p2: " + str(p1 == p2))
print("p1 == p3: " + str(p1 == p3))
print("PASS\n")

# Test 10: Multiple Inheritance with MRO
print("Test 10: Multiple Inheritance with MRO")
class A:
    def method(self):
        return "A"

    def greet(self):
        return "Hello from A"

class B(A):
    def method(self):
        return "B"

class C(A):
    def method(self):
        return "C"

class D(B, C):
    pass

d = D()
print("D.method() (should be B): " + d.method())
print("D.greet() (should be from A): " + d.greet())
print("PASS\n")

# Test 11: super() with multiple inheritance
print("Test 11: super() with multiple inheritance")
class Animal:
    def __init__(self, name):
        self.name = name
        print("Animal.__init__ called with name=" + name)

    def speak(self):
        return "Some sound"

class Mammal(Animal):
    def __init__(self, name, fur_color):
        super().__init__(name)
        self.fur_color = fur_color
        print("Mammal.__init__ called with fur_color=" + fur_color)

    def speak(self):
        return super().speak() + " (mammal)"

class Dog(Mammal):
    def __init__(self, name, fur_color, breed):
        super().__init__(name, fur_color)
        self.breed = breed
        print("Dog.__init__ called with breed=" + breed)

    def speak(self):
        return "Woof!"

dog = Dog("Rex", "brown", "Labrador")
print("Dog name: " + dog.name)
print("Dog fur_color: " + dog.fur_color)
print("Dog breed: " + dog.breed)
print("Dog speaks: " + dog.speak())
print("PASS\n")

# Test 12: Diamond Inheritance with super()
print("Test 12: Diamond Inheritance with super()")
class Base:
    def __init__(self):
        self.base_attr = "base"
        print("Base.__init__ called")

class Left(Base):
    def __init__(self):
        super().__init__()
        self.left_attr = "left"
        print("Left.__init__ called")

class Right(Base):
    def __init__(self):
        super().__init__()
        self.right_attr = "right"
        print("Right.__init__ called")

class Diamond(Left, Right):
    def __init__(self):
        super().__init__()
        self.diamond_attr = "diamond"
        print("Diamond.__init__ called")

dia = Diamond()
print("Diamond base_attr: " + dia.base_attr)
print("Diamond left_attr: " + dia.left_attr)
print("Diamond right_attr: " + dia.right_attr)
print("Diamond diamond_attr: " + dia.diamond_attr)
print("PASS\n")

# Test 13: Method overriding in inheritance chain
print("Test 13: Method overriding in inheritance chain")
class Vehicle:
    def __init__(self, brand):
        self.brand = brand

    def start(self):
        return self.brand + " starting..."

    def stop(self):
        return self.brand + " stopping..."

class Car(Vehicle):
    def __init__(self, brand, model):
        super().__init__(brand)
        self.model = model

    def start(self):
        return super().start() + " (car engine)"

class ElectricCar(Car):
    def __init__(self, brand, model, battery_size):
        super().__init__(brand, model)
        self.battery_size = battery_size

    def start(self):
        return self.brand + " " + self.model + " starting silently (electric)"

    def stop(self):
        return super().stop() + " (regenerative braking)"

ec = ElectricCar("Tesla", "Model S", 100)
print("ElectricCar brand: " + ec.brand)
print("ElectricCar model: " + ec.model)
print("ElectricCar battery: " + str(ec.battery_size))
print("ElectricCar start: " + ec.start())
print("ElectricCar stop: " + ec.stop())
print("PASS\n")

# Test 14: Dunder method - __lt__ (less than)
print("Test 14: Dunder method - __lt__ (less than)")
class Number:
    def __init__(self, value):
        self.value = value

    def __lt__(self, other):
        return self.value < other.value

n1 = Number(5)
n2 = Number(10)
print("n1 < n2: " + str(n1 < n2))
print("n2 < n1: " + str(n2 < n1))
print("PASS\n")

# Test 15: Accessing parent class attributes
print("Test 15: Accessing parent class attributes")
class Parent:
    class_var = "parent_class_var"

    def __init__(self):
        self.instance_var = "parent_instance_var"

class Child(Parent):
    def __init__(self):
        super().__init__()
        self.child_var = "child_var"

    def get_parent_vars(self):
        return "Class: " + Parent.class_var + ", Instance: " + self.instance_var

child = Child()
print("Child vars: " + child.get_parent_vars())
print("Child own var: " + child.child_var)
print("PASS\n")

print("=== All 15 OOP Tests Passed Successfully ===")
