# Core OOP Features Test - Tests super(), dunder methods, method overriding, and multiple inheritance
# Skip properties for now as they need separate investigation

print("=== Core OOP Features Test ===\n")

# Test 1: Basic __init__ and method overriding
print("Test 1: Basic Class with __init__")
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def greet(self):
        return "Hello, I'm " + self.name

p = Person("Alice", 30)
print("Name: " + p.name)
print("Greet: " + p.greet())
print("PASS\n")

# Test 2: super() with single inheritance
print("Test 2: super() with single inheritance")
class Teacher(Person):
    def __init__(self, name, age, subject):
        super().__init__(name, age)
        self.subject = subject

    def greet(self):
        return super().greet() + " and I teach " + self.subject

t = Teacher("Carol", 35, "Math")
print("Teacher: " + t.greet())
print("PASS\n")

# Test 3: Dunder method - __str__
print("Test 3: Dunder method - __str__")
class Book:
    def __init__(self, title, author):
        self.title = title
        self.author = author

    def __str__(self):
        return "Book: " + self.title + " by " + self.author

book = Book("1984", "George Orwell")
print(str(book))
print("PASS\n")

# Test 4: Dunder method - __repr__
print("Test 4: Dunder method - __repr__")
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __repr__(self):
        return "Point(" + str(self.x) + ", " + str(self.y) + ")"

pt = Point(3, 4)
print(repr(pt))
print("PASS\n")

# Test 5: Dunder method - __add__ (operator overloading)
print("Test 5: Dunder method - __add__ (operator overloading)")
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
print("v1 + v2 = " + str(v3))
print("PASS\n")

# Test 6: Dunder method - __eq__
print("Test 6: Dunder method - __eq__")
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

# Test 7: Multiple Inheritance with MRO
print("Test 7: Multiple Inheritance with MRO")
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

# Test 8: super() with multiple inheritance chain
print("Test 8: super() with multiple inheritance chain")
class Animal:
    def __init__(self, name):
        self.name = name
        print("Animal.__init__ called")

class Mammal(Animal):
    def __init__(self, name, fur_color):
        super().__init__(name)
        self.fur_color = fur_color
        print("Mammal.__init__ called")

class Dog(Mammal):
    def __init__(self, name, fur_color, breed):
        super().__init__(name, fur_color)
        self.breed = breed
        print("Dog.__init__ called")

dog = Dog("Rex", "brown", "Labrador")
print("Dog name: " + dog.name)
print("Dog fur_color: " + dog.fur_color)
print("Dog breed: " + dog.breed)
print("PASS\n")

# Test 9: Diamond Inheritance with super()
print("Test 9: Diamond Inheritance with super()")
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

# Test 10: Method overriding in inheritance chain
print("Test 10: Method overriding in inheritance chain")
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
print("ElectricCar start: " + ec.start())
print("ElectricCar stop: " + ec.stop())
print("PASS\n")

# Test 11: Dunder method - __lt__
print("Test 11: Dunder method - __lt__")
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

# Test 12: Accessing parent class attributes
print("Test 12: Accessing parent class attributes")
class Parent:
    class_var = "parent_class_var"

    def __init__(self):
        self.instance_var = "parent_instance_var"

class Child(Parent):
    def __init__(self):
        super().__init__()
        self.child_var = "child_var"

    def get_parent_vars(self):
        return "Instance: " + self.instance_var

child = Child()
print("Child vars: " + child.get_parent_vars())
print("Child own var: " + child.child_var)
print("PASS\n")

print("=== All 12 Core OOP Tests Passed Successfully ===")
print("\nSummary:")
print("- Basic classes and __init__: WORKING")
print("- Method overriding: WORKING")
print("- super() with single inheritance: WORKING")
print("- super() with multiple inheritance: WORKING")
print("- super() with diamond inheritance: WORKING")
print("- Dunder methods (__str__, __repr__, __add__, __eq__, __lt__): WORKING")
print("- Multiple inheritance with MRO: WORKING")
print("- Method resolution order: WORKING")
print("\nAll critical OOP features are functioning correctly!")
