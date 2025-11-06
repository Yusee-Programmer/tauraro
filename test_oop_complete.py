# Complete OOP test for C compilation

# Basic class definition
class Animal:
    def __init__(self, name):
        self.name = name
        self.age = 0

    def speak(self):
        print("Animal " + self.name + " makes a sound")

    def get_info(self):
        return self.name + " is " + str(self.age) + " years old"

# Inheritance
class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name)
        self.breed = breed

    def speak(self):
        print("Dog " + self.name + " barks")

    def get_breed(self):
        return self.breed

# Multiple inheritance
class Swimmer:
    def swim(self):
        print("Swimming")

class Cat(Animal, Swimmer):
    def __init__(self, name, color):
        super().__init__(name)
        self.color = color

    def speak(self):
        print("Cat " + self.name + " meows")

# Test basic class
print("=== Basic Class ===")
animal = Animal("Generic")
animal.age = 5
print(animal.get_info())
animal.speak()

# Test inheritance
print("\n=== Inheritance ===")
dog = Dog("Buddy", "Golden Retriever")
dog.age = 3
print(dog.get_info())
dog.speak()
print("Breed: " + dog.get_breed())

# Test multiple inheritance
print("\n=== Multiple Inheritance ===")
cat = Cat("Whiskers", "Orange")
cat.age = 2
print(cat.get_info())
cat.speak()
cat.swim()

print("\nAll OOP tests completed!")
