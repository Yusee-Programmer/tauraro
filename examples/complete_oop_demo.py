class Animal:
    def __init__(self, name, species):
        self.name = name
        self.species = species
    
    def speak(self):
        return f"{self.name} makes a sound"

class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name, "Dog")
        self.breed = breed
    
    # Method overriding
    def speak(self):
        return f"{self.name} barks"
    
    # New method
    def fetch(self, item):
        return f"{self.name} fetches {item}"

class Cat(Animal):
    def __init__(self, name, color):
        super().__init__(name, "Cat")
        self.color = color
    
    # Method overriding
    def speak(self):
        return f"{self.name} meows"
    
    # New method
    def climb(self, object):
        return f"{self.name} climbs {object}"

# Create instances
dog = Dog("Buddy", "Golden Retriever")
cat = Cat("Whiskers", "Orange")

# Test basic functionality
print(dog.speak())
print(cat.speak())

# Test new methods
print(dog.fetch("ball"))
print(cat.climb("tree"))

# Test isinstance
print(f"Is dog an Animal? {isinstance(dog, Animal)}")
print(f"Is cat a Dog? {isinstance(cat, Dog)}")

# Test attribute access
print(f"Dog's breed: {dog.breed}")
print(f"Cat's color: {cat.color}")

# Test polymorphism
animals = [dog, cat]
for animal in animals:
    print(animal.speak())