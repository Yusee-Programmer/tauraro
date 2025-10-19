# Comprehensive OOP Test Suite for Tauraro
print("=" * 80)
print("TAURARO COMPREHENSIVE OOP TEST SUITE")
print("=" * 80)

# ============================================================================
# PART 1: BASIC CLASS FEATURES
# ============================================================================
print("\n" + "=" * 80)
print("PART 1: BASIC CLASS FEATURES")
print("=" * 80)

print("\n--- Test 1.1: Class instantiation with __init__ ---")
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

p = Person("Alice", 30)
print(f"Name: {p.name}, Age: {p.age}")

print("\n--- Test 1.2: Instance methods ---")
class Calculator:
    def __init__(self, value):
        self.value = value

    def add(self, n):
        self.value = self.value + n
        return self.value

    def get_value(self):
        return self.value

calc = Calculator(10)
calc.add(5)
print(f"Calculator value: {calc.get_value()}")

print("\n--- Test 1.3: Multiple instances independence ---")
class Counter:
    def __init__(self):
        self.count = 0

    def increment(self):
        self.count = self.count + 1

c1 = Counter()
c2 = Counter()
c1.increment()
c1.increment()
c2.increment()
print(f"Counter 1: {c1.count}, Counter 2: {c2.count}")

# ============================================================================
# PART 2: INHERITANCE
# ============================================================================
print("\n" + "=" * 80)
print("PART 2: INHERITANCE")
print("=" * 80)

print("\n--- Test 2.1: Simple inheritance ---")
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        return "Some sound"

class Dog(Animal):
    def speak(self):
        return "Woof!"

dog = Dog("Buddy")
print(f"Dog name: {dog.name}")
print(f"Dog speaks: {dog.speak()}")

print("\n--- Test 2.2: Method overriding ---")
class Shape:
    def area(self):
        return 0

class Rectangle(Shape):
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

rect = Rectangle(5, 3)
print(f"Rectangle area: {rect.area()}")
