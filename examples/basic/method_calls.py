# method_calls.py - Demonstrating proper method calls in Tauraro (Python-compatible)
# This example shows how Tauraro supports Python-style method calls

print("=== Method Calls in Tauraro (Python-compatible) ===")

# Define a simple Point class
class Point:
    # Constructor method
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    # Method to calculate distance from origin
    def distance_from_origin(self):
        return (self.x ** 2 + self.y ** 2) ** 0.5
    
    # Method to move the point
    def move(self, dx, dy):
        self.x += dx
        self.y += dy
        return self  # Return self for method chaining
    
    # Method to get string representation
    def __str__(self):
        return f"Point({self.x}, {self.y})"

# Create a point instance
p = Point(3, 4)
print("Created point:", p)

# Call methods on the point
distance = p.distance_from_origin()
print("Distance from origin:", distance)

# Move the point
p.move(2, -1)
print("After moving:", p)

# Method chaining
p.move(1, 1).move(-2, 3)
print("After chaining moves:", p)

print("\n=== Class with Class Variables ===")

# Define a class with both instance and class variables
class Counter:
    # Class variable (shared by all instances)
    total_count = 0
    
    # Constructor
    def __init__(self, name):
        self.name = name
        self.count = 0
        # Increment class variable
        Counter.total_count += 1
    
    # Instance method to increment count
    def increment(self):
        self.count += 1
        return self.count
    
    # Instance method to get info
    def get_info(self):
        return f"{self.name}: {self.count}"
    
    # Class method to get total count
    @classmethod
    def get_total_count(cls):
        return cls.total_count

# Create counter instances
counter1 = Counter("Counter1")
counter2 = Counter("Counter2")

print("Total counters created:", Counter.get_total_count())

# Use counters
counter1.increment()
counter1.increment()
counter2.increment()

print(counter1.get_info())
print(counter2.get_info())
print("Total count:", Counter.get_total_count())

print("\n=== Inheritance Example ===")

# Base class
class Animal:
    def __init__(self, name, species):
        self.name = name
        self.species = species
    
    def speak(self):
        return f"{self.name} makes a sound"
    
    def info(self):
        return f"{self.name} is a {self.species}"

# Derived class
class Dog(Animal):
    def __init__(self, name, breed):
        # Call parent constructor
        super().__init__(name, "Dog")
        self.breed = breed
    
    # Override parent method
    def speak(self):
        return f"{self.name} barks!"
    
    # Add new method
    def fetch(self, item):
        return f"{self.name} fetches the {item}"

# Create instances
my_dog = Dog("Buddy", "Golden Retriever")
print(my_dog.info())
print(my_dog.speak())
print(my_dog.fetch("ball"))

print("\n=== All method call examples completed successfully! ===")