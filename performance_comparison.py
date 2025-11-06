#!/usr/bin/env python3
"""
Python equivalent of comprehensive OOP test for performance comparison
"""

import time

print("=" * 70)
print(" " * 15 + "Python OOP Performance Test")
print("=" * 70)

start_time = time.time()

# Test 1: Basic Class
class Animal:
    def __init__(self, name, species):
        self.name = name
        self.species = species
        self.age = 0

    def speak(self):
        return f"{self.name} makes a sound"

    def get_info(self):
        return f"{self.species} named {self.name}"

dog = Animal("Buddy", "Dog")
cat = Animal("Whiskers", "Cat")

# Test 2: Single Inheritance
class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name, "Dog")
        self.breed = breed

    def speak(self):
        return f"{self.name} says Woof!"

golden = Dog("Max", "Golden Retriever")

# Test 3: Multiple Inheritance
class Flyable:
    def __init__(self):
        self.can_fly = True

class Swimmable:
    def __init__(self):
        self.can_swim = True

class Duck(Animal, Flyable, Swimmable):
    def __init__(self, name):
        Animal.__init__(self, name, "Duck")
        Flyable.__init__(self)
        Swimmable.__init__(self)

duck = Duck("Donald")

# Test 4: Class Attributes
class Counter:
    count = 0

    def __init__(self, name):
        self.name = name
        Counter.count += 1

c1 = Counter("First")
c2 = Counter("Second")
c3 = Counter("Third")

# Test 5: Method Overriding
class Vehicle:
    def __init__(self, brand):
        self.brand = brand

class Car(Vehicle):
    def __init__(self, brand, model):
        super().__init__(brand)
        self.model = model

car = Car("Tesla", "Model S")

# Run many iterations for timing
iterations = 100000
for i in range(iterations):
    _ = Animal("Test", "Species")
    _ = Dog("Test", "Breed")
    _ = Duck("Test")
    _ = Counter("Test")
    _ = Car("Brand", "Model")

end_time = time.time()
duration = end_time - start_time

print(f"\nPython Performance:")
print(f"  Total time: {duration:.3f} seconds")
print(f"  Iterations: {iterations}")
print(f"  Time per iteration: {(duration/iterations)*1000:.6f} ms")
print("=" * 70)
