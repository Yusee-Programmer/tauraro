# Advanced OOP test file

class Animal:
    def __init__(self, name):
        self.name = name
    
    def speak(self):
        return f"{self.name} makes a sound"
    
    def info(self):
        return f"This is an animal named {self.name}"

class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name)
        self.breed = breed
    
    def speak(self):
        return f"{self.name} barks"
    
    def info(self):
        return f"This is a {self.breed} dog named {self.name}"

class Cat(Animal):
    def __init__(self, name, color):
        super().__init__(name)
        self.color = color
    
    def speak(self):
        return f"{self.name} meows"
    
    def info(self):
        return f"This is a {self.color} cat named {self.name}"

# Create instances
dog = Dog("Buddy", "Golden Retriever")
cat = Cat("Whiskers", "Orange")

# Test polymorphism
animals = [dog, cat]
for animal in animals:
    print(animal.speak())
    print(animal.info())
    print(isinstance(animal, Animal))
    print(isinstance(animal, Dog))
    print(isinstance(animal, Cat))
    print("---")

# Test method overriding
print(dog.speak())  # Should print "Buddy barks"
print(cat.speak())  # Should print "Whiskers meows"

# Test attribute access
print(dog.name)     # Should print "Buddy"
print(dog.breed)    # Should print "Golden Retriever"
print(cat.name)     # Should print "Whiskers"
print(cat.color)    # Should print "Orange"