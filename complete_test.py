# Complete test for all fixes

print("=== Testing f-string formatting ===")
name = "Alice"
age = 30
greeting = f"Hello, {name}! You are {age} years old."
print(greeting)

# Test isinstance with classes
print("\n=== Testing isinstance ===")

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

dog = Dog("Buddy", "Golden Retriever")
cat = Cat("Whiskers", "Orange")

print("isinstance(dog, Dog):", isinstance(dog, Dog))
print("isinstance(dog, Animal):", isinstance(dog, Animal))
print("isinstance(cat, Cat):", isinstance(cat, Cat))
print("isinstance(cat, Animal):", isinstance(cat, Animal))

# Test tuple parsing
print("\n=== Testing tuple parsing ===")
t1 = ()  # Empty tuple
t2 = (1,)  # Single element tuple
t3 = (1, 2, 3)  # Multiple element tuple
t4 = 1, 2, 3  # Tuple without parentheses

print("Empty tuple:", t1)
print("Single element tuple:", t2)
print("Multiple element tuple:", t3)
print("Tuple without parentheses:", t4)

# Test complex inheritance
print("\n=== Testing complex inheritance ===")

class A:
    def method_a(self):
        return "A"

class B(A):
    def method_b(self):
        return "B"

class C(A):
    def method_c(self):
        return "C"

class D(B, C):
    def method_d(self):
        return "D"

d = D()
print("Method A:", d.method_a())
print("Method B:", d.method_b())
print("Method C:", d.method_c())
print("Method D:", d.method_d())

print("\n=== All tests completed ===")