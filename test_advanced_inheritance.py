# Test advanced inheritance scenarios

print("=== Advanced Inheritance Test ===")
print()

# Test 1: Single inheritance with super()
print("Test 1: Single inheritance with super()")
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        return f"{self.name} makes a sound"

class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name)
        self.breed = breed

    def speak(self):
        return f"{self.name} barks"

dog = Dog("Buddy", "Golden Retriever")
print("Name:", dog.name)
print("Breed:", dog.breed)
print("Speak:", dog.speak())
print("PASS: Test 1")
print()

# Test 2: Multiple inheritance
print("Test 2: Multiple inheritance")
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
print("PASS: Test 2")
print()

# Test 3: Diamond inheritance (MRO test)
print("Test 3: Diamond inheritance (MRO test)")
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
print("Method call:", d.method())
print("PASS: Test 3")
print()

# Test 4: Method override and super()
print("Test 4: Method override with super()")
class Shape:
    def __init__(self, color):
        self.color = color

    def describe(self):
        return f"A {self.color} shape"

class Circle(Shape):
    def __init__(self, color, radius):
        super().__init__(color)
        self.radius = radius

    def describe(self):
        base = super().describe()
        return f"{base} with radius {self.radius}"

circle = Circle("red", 5)
print("Description:", circle.describe())
print("PASS: Test 4")
print()

# Test 5: Multiple levels of inheritance
print("Test 5: Multiple levels of inheritance")
class Vehicle:
    def __init__(self, brand):
        self.brand = brand

class Car(Vehicle):
    def __init__(self, brand, model):
        super().__init__(brand)
        self.model = model

class ElectricCar(Car):
    def __init__(self, brand, model, battery):
        super().__init__(brand, model)
        self.battery = battery

tesla = ElectricCar("Tesla", "Model 3", 75)
print("Brand:", tesla.brand)
print("Model:", tesla.model)
print("Battery:", tesla.battery)
print("PASS: Test 5")
print()

print("=== All Advanced Inheritance Tests Completed ===")
