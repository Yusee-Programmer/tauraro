# Test 8: Inheritance
print("=== Test 8: Inheritance ===")

# Base class
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        print("Animal", self.name, "makes a sound")

# Derived class
class Dog:
    def __init__(self, name, breed):
        self.name = name
        self.breed = breed

    def speak(self):
        print("Dog", self.name, "barks!")

    def info(self):
        print("Breed:", self.breed)

# Create instances
animal = Animal("Generic")
animal.speak()

dog = Dog("Buddy", "Golden Retriever")
dog.speak()
dog.info()

print("\n=== Test 8 Complete ===")
