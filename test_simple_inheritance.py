# Test simple inheritance without super()

print("=== Simple Inheritance Test ===")
print()

# Test 1: Basic inheritance
print("Test 1: Basic inheritance")
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        return f"{self.name} makes a sound"

class Dog(Animal):
    def speak(self):
        return f"{self.name} barks"

dog = Dog("Buddy")
print("Name:", dog.name)
print("Speak:", dog.speak())
print("PASS: Test 1")
print()

# Test 2: Method inheritance
print("Test 2: Method inheritance")
class Vehicle:
    def __init__(self, brand):
        self.brand = brand

    def start(self):
        return f"{self.brand} is starting"

class Car(Vehicle):
    pass

car = Car("Toyota")
print("Brand:", car.brand)
print("Start:", car.start())
print("PASS: Test 2")
print()

# Test 3: Multiple inheritance
print("Test 3: Multiple inheritance")
class Walker:
    def walk(self):
        return "Walking"

class Swimmer:
    def swim(self):
        return "Swimming"

class Duck(Walker, Swimmer):
    def __init__(self, name):
        self.name = name

duck = Duck("Donald")
print("Name:", duck.name)
print("Walk:", duck.walk())
print("Swim:", duck.swim())
print("PASS: Test 3")
print()

print("=== Simple Inheritance Tests Completed ===")
