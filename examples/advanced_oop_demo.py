# Advanced OOP Features Demo in Tauraro
# This example demonstrates all the advanced OOP features we've implemented

print("=== Advanced OOP Features Demo ===\n")

# 1. Data Classes (@dataclass)
print("1. Data Classes (@dataclass)")
from dataclasses import dataclass

@dataclass
class Person:
    def __init__(self, name, age, email=""):
        self.name = name
        self.age = age
        self.email = email

person = Person("Alice", 30, "alice@example.com")
print("Person: " + str(person))
print("Person name: " + person.name)
print()

# 2. Slots (__slots__)
print("2. Slots (__slots__)")
class Point:
    __slots__ = ['x', 'y']
    
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def __repr__(self):
        return "Point(x=" + str(self.x) + ", y=" + str(self.y) + ")"

point = Point(1, 2)
print("Point: " + str(point))
# This would normally raise an AttributeError in Python if we tried to add new attributes
# point.z = 3  # This would be restricted by __slots__
print()

# 3. Descriptors
print("3. Descriptors")
class PositiveNumber:
    def __init__(self, name):
        self.name = name
    
    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return obj.__dict__.get(self.name, 0)
    
    def __set__(self, obj, value):
        if not isinstance(value, (int, float)) or value <= 0:
            raise ValueError(self.name + " must be a positive number")
        obj.__dict__[self.name] = value
    
    def __delete__(self, obj):
        del obj.__dict__[self.name]

class Rectangle:
    width = PositiveNumber("width")
    height = PositiveNumber("height")
    
    def __init__(self, width, height):
        self.width = width
        self.height = height
    
    def area(self):
        return self.width * self.height

rect = Rectangle(5, 3)
print("Rectangle: width=" + str(rect.width) + ", height=" + str(rect.height) + ", area=" + str(rect.area()))
# rect.width = -1  # This would raise ValueError
print()

# 4. Abstract Base Classes (ABC)
print("4. Abstract Base Classes (ABC)")
from abc import ABC, abstractmethod

class Animal(ABC):
    @abstractmethod
    def make_sound(self):
        pass
    
    @abstractmethod
    def move(self):
        pass

class Dog(Animal):
    def make_sound(self):
        return "Woof!"
    
    def move(self):
        return "Running on four legs"

class Bird(Animal):
    def make_sound(self):
        return "Tweet!"
    
    def move(self):
        return "Flying with wings"

dog = Dog()
bird = Bird()
print("Dog sound: " + dog.make_sound() + ", movement: " + dog.move())
print("Bird sound: " + bird.make_sound() + ", movement: " + bird.move())
print()

# 5. Metaclasses
print("5. Metaclasses")
class SingletonMeta(type):
    _instances = {}
    
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]

class Singleton(object):
    __metaclass__ = SingletonMeta
    
    def __init__(self, value):
        self.value = value

# Creating instances
s1 = Singleton("first")
s2 = Singleton("second")
print("Singleton values: s1=" + s1.value + ", s2=" + s2.value)
print("Are they the same object? " + str(s1 is s2))
print()

# 6. Class Decorators
print("6. Class Decorators")
def add_str_method(cls):
    def __str__(self):
        items = []
        for k, v in self.__dict__.items():
            items.append(k + "=" + str(v))
        return cls.__name__ + "(" + ", ".join(items) + ")"
    cls.__str__ = __str__
    return cls

@add_str_method
class Car:
    def __init__(self, make, model):
        self.make = make
        self.model = model

car = Car("Toyota", "Camry")
print("Car: " + str(car))
print()

# 7. Operator Overloading
print("7. Operator Overloading")
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)
    
    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar)
    
    def __repr__(self):
        return "Vector(" + str(self.x) + ", " + str(self.y) + ")"
    
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

v1 = Vector(2, 3)
v2 = Vector(1, 4)
v3 = v1 + v2
v4 = v1 * 3
print("v1: " + str(v1))
print("v2: " + str(v2))
print("v1 + v2: " + str(v3))
print("v1 * 3: " + str(v4))
print("v1 == v2: " + str(v1 == v2))
print()

# 8. Custom Iterators
print("8. Custom Iterators")
class CountDown:
    def __init__(self, start):
        self.start = start
    
    def __iter__(self):
        return self
    
    def __next__(self):
        if self.start <= 0:
            raise StopIteration
        self.start -= 1
        return self.start + 1

countdown = CountDown(3)
print("Countdown from 3:")
for num in countdown:
    print(num)
print()

# 9. Multiple Inheritance with MRO
print("9. Multiple Inheritance with MRO")
class A:
    def method(self):
        return "A"

class B(A):
    def method(self):
        return "B"

class C(A):
    def method(self):
        return "C"

class D(B, C):
    pass

d = D()
print("D.method(): " + d.method())
# Note: MRO display might not work the same way in Tauraro
print()

# 10. Properties
print("10. Properties")
class Temperature:
    def __init__(self, celsius=0):
        self._celsius = celsius
    
    @property
    def celsius(self):
        return self._celsius
    
    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("Temperature cannot be below absolute zero")
        self._celsius = value
    
    @property
    def fahrenheit(self):
        return self._celsius * 9/5 + 32
    
    @fahrenheit.setter
    def fahrenheit(self, value):
        self.celsius = (value - 32) * 5/9

temp = Temperature(25)
print("Temperature: " + str(temp.celsius) + "°C, " + str(temp.fahrenheit) + "°F")
temp.fahrenheit = 86
print("Temperature: " + str(temp.celsius) + "°C, " + str(temp.fahrenheit) + "°F")
print()

print("=== All Advanced OOP Features Demonstrated Successfully ===")