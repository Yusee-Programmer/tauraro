# Test isinstance functionality

class Animal:
    def __init__(self, name):
        self.name = name

class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name)
        self.breed = breed

class Cat(Animal):
    def __init__(self, name, color):
        super().__init__(name)
        self.color = color

# Create instances
dog = Dog("Buddy", "Golden Retriever")
cat = Cat("Whiskers", "Orange")

# Test isinstance with string type names
print("isinstance(dog, 'Dog'):", isinstance(dog, "Dog"))
print("isinstance(cat, 'Cat'):", isinstance(cat, "Cat"))
print("isinstance(dog, 'Animal'):", isinstance(dog, "Animal"))
print("isinstance(dog, 'Cat'):", isinstance(dog, "Cat"))

# Test isinstance with actual class types (this might not work yet)
print("isinstance(dog, Dog):", isinstance(dog, Dog))
print("isinstance(cat, Cat):", isinstance(cat, Cat))
print("isinstance(dog, Animal):", isinstance(dog, Animal))
print("isinstance(cat, Animal):", isinstance(cat, Animal))

print("isinstance tests completed!")