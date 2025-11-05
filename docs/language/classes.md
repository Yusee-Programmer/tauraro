# Classes and Object-Oriented Programming

Tauraro supports full Python-style object-oriented programming with classes, inheritance, properties, and magic methods.

## Class Basics

### Defining Classes

```python
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def greet(self):
        return f"Hello, I'm {self.name}"

# Create instance
person = Person("Alice", 30)
print(person.name)        # Alice
print(person.greet())     # Hello, I'm Alice
```

### Instance Attributes

```python
class Dog:
    def __init__(self, name, breed):
        self.name = name      # Instance attribute
        self.breed = breed
        self.tricks = []      # Mutable attribute

    def add_trick(self, trick):
        self.tricks.append(trick)

dog = Dog("Buddy", "Golden Retriever")
dog.add_trick("roll over")
print(dog.tricks)  # ['roll over']
```

### Class Attributes

Shared across all instances.

```python
class Dog:
    species = "Canis familiaris"  # Class attribute

    def __init__(self, name):
        self.name = name          # Instance attribute

dog1 = Dog("Buddy")
dog2 = Dog("Max")

print(dog1.species)  # Canis familiaris
print(dog2.species)  # Canis familiaris

# Modifying class attribute affects all instances
Dog.species = "Domestic Dog"
print(dog1.species)  # Domestic Dog
```

## Methods

### Instance Methods

```python
class Calculator:
    def __init__(self, value=0):
        self.value = value

    def add(self, n):
        self.value += n
        return self

    def multiply(self, n):
        self.value *= n
        return self

calc = Calculator(10)
calc.add(5).multiply(2)
print(calc.value)  # 30
```

### Class Methods

Bound to the class, not instance.

```python
class Person:
    population = 0

    def __init__(self, name):
        self.name = name
        Person.population += 1

    @classmethod
    def get_population(cls):
        return f"Population: {cls.population}"

    @classmethod
    def from_birth_year(cls, name, birth_year):
        age = 2024 - birth_year
        return cls(name, age)

person = Person.from_birth_year("Alice", 1990)
print(Person.get_population())
```

### Static Methods

Independent of class or instance.

```python
class MathUtils:
    @staticmethod
    def add(a, b):
        return a + b

    @staticmethod
    def is_even(n):
        return n % 2 == 0

print(MathUtils.add(5, 10))      # 15
print(MathUtils.is_even(4))      # True
```

## Properties

### Basic Properties

```python
class Circle:
    def __init__(self, radius):
        self._radius = radius

    @property
    def radius(self):
        """Get the radius"""
        return self._radius

    @radius.setter
    def radius(self, value):
        """Set the radius"""
        if value < 0:
            raise ValueError("Radius cannot be negative")
        self._radius = value

    @property
    def area(self):
        """Computed property"""
        return 3.14159 * self._radius ** 2

circle = Circle(5)
print(circle.radius)     # 5
print(circle.area)       # 78.53975

circle.radius = 10       # Uses setter
print(circle.area)       # 314.159
```

### Read-Only Properties

```python
class Person:
    def __init__(self, first, last):
        self.first = first
        self.last = last

    @property
    def full_name(self):
        return f"{self.first} {self.last}"

person = Person("Alice", "Smith")
print(person.full_name)  # Alice Smith
# person.full_name = "Bob"  # ERROR - no setter
```

## Inheritance

### Single Inheritance

```python
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        return "Some sound"

class Dog(Animal):
    def speak(self):
        return "Woof!"

class Cat(Animal):
    def speak(self):
        return "Meow!"

dog = Dog("Buddy")
print(dog.name)      # Buddy
print(dog.speak())   # Woof!
```

### super()

Call parent class methods.

```python
class Animal:
    def __init__(self, name, species):
        self.name = name
        self.species = species

class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name, "Dog")
        self.breed = breed

dog = Dog("Buddy", "Golden Retriever")
print(dog.species)  # Dog
print(dog.breed)    # Golden Retriever
```

### Multiple Inheritance

```python
class Flyable:
    def fly(self):
        return "Flying high!"

class Swimmable:
    def swim(self):
        return "Swimming fast!"

class Duck(Flyable, Swimmable):
    def quack(self):
        return "Quack!"

duck = Duck()
print(duck.fly())    # Flying high!
print(duck.swim())   # Swimming fast!
print(duck.quack())  # Quack!
```

### Method Resolution Order (MRO)

```python
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
print(d.method())      # B
print(D.__mro__)       # (D, B, C, A, object)
```

## Magic Methods (Dunder Methods)

### Object Representation

```python
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        """User-friendly string"""
        return f"Point({self.x}, {self.y})"

    def __repr__(self):
        """Developer-friendly representation"""
        return f"Point(x={self.x}, y={self.y})"

point = Point(3, 4)
print(str(point))   # Point(3, 4)
print(repr(point))  # Point(x=3, y=4)
```

### Arithmetic Operators

```python
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y)

    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar)

    def __str__(self):
        return f"Vector({self.x}, {self.y})"

v1 = Vector(1, 2)
v2 = Vector(3, 4)
print(v1 + v2)   # Vector(4, 6)
print(v1 * 2)    # Vector(2, 4)
```

### Comparison Operators

```python
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def __eq__(self, other):
        return self.age == other.age

    def __lt__(self, other):
        return self.age < other.age

    def __le__(self, other):
        return self.age <= other.age

alice = Person("Alice", 30)
bob = Person("Bob", 25)

print(alice > bob)   # True
print(alice == bob)  # False
print(sorted([alice, bob], key=lambda p: p.age))
```

### Container Methods

```python
class MyList:
    def __init__(self):
        self.items = []

    def __len__(self):
        return len(self.items)

    def __getitem__(self, index):
        return self.items[index]

    def __setitem__(self, index, value):
        self.items[index] = value

    def __delitem__(self, index):
        del self.items[index]

    def __contains__(self, item):
        return item in self.items

    def append(self, item):
        self.items.append(item)

mylist = MyList()
mylist.append(1)
mylist.append(2)
print(len(mylist))    # 2
print(mylist[0])      # 1
print(1 in mylist)    # True
```

### Callable Objects

```python
class Multiplier:
    def __init__(self, factor):
        self.factor = factor

    def __call__(self, x):
        return x * self.factor

times_3 = Multiplier(3)
print(times_3(10))  # 30
print(times_3(5))   # 15
```

### Context Managers

```python
class FileManager:
    def __init__(self, filename, mode):
        self.filename = filename
        self.mode = mode
        self.file = None

    def __enter__(self):
        self.file = open(self.filename, self.mode)
        return self.file

    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.file:
            self.file.close()

with FileManager("data.txt", "w") as f:
    f.write("Hello, World!")
```

## Advanced Features

### Abstract Base Classes

```python
from abc import ABC, abstractmethod

class Shape(ABC):
    @abstractmethod
    def area(self):
        pass

    @abstractmethod
    def perimeter(self):
        pass

class Rectangle(Shape):
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self):
        return self.width * self.height

    def perimeter(self):
        return 2 * (self.width + self.height)

rect = Rectangle(5, 10)
print(rect.area())       # 50
```

### Dataclasses

*Note: If implemented in Tauraro*

```python
from dataclasses import dataclass

@dataclass
class Point:
    x: float
    y: float

    def distance_from_origin(self):
        return (self.x**2 + self.y**2)**0.5

point = Point(3.0, 4.0)
print(point)                      # Point(x=3.0, y=4.0)
print(point.distance_from_origin())  # 5.0
```

### Metaclasses

```python
class Meta(type):
    def __new__(cls, name, bases, attrs):
        # Customize class creation
        attrs['class_id'] = id(cls)
        return super().__new__(cls, name, bases, attrs)

class MyClass(metaclass=Meta):
    pass

print(MyClass.class_id)
```

## Type Annotations with Classes

```python
class BankAccount:
    def __init__(self, balance: float = 0.0):
        self.balance: float = balance

    def deposit(self, amount: float) -> None:
        self.balance += amount

    def withdraw(self, amount: float) -> bool:
        if amount <= self.balance:
            self.balance -= amount
            return True
        return False

    def get_balance(self) -> float:
        return self.balance

# Type annotations are enforced at runtime in Tauraro
account: BankAccount = BankAccount(100.0)
account.deposit(50.0)
```

## Design Patterns

### Singleton

```python
class Singleton:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

s1 = Singleton()
s2 = Singleton()
print(s1 is s2)  # True
```

### Factory

```python
class AnimalFactory:
    @staticmethod
    def create_animal(animal_type):
        if animal_type == "dog":
            return Dog()
        elif animal_type == "cat":
            return Cat()
        else:
            raise ValueError(f"Unknown animal type: {animal_type}")

animal = AnimalFactory.create_animal("dog")
```

### Builder

```python
class Pizza:
    def __init__(self):
        self.size = None
        self.cheese = False
        self.pepperoni = False

    def __str__(self):
        return f"Pizza(size={self.size}, cheese={self.cheese}, pepperoni={self.pepperoni})"

class PizzaBuilder:
    def __init__(self):
        self.pizza = Pizza()

    def set_size(self, size):
        self.pizza.size = size
        return self

    def add_cheese(self):
        self.pizza.cheese = True
        return self

    def add_pepperoni(self):
        self.pizza.pepperoni = True
        return self

    def build(self):
        return self.pizza

pizza = PizzaBuilder().set_size("large").add_cheese().add_pepperoni().build()
print(pizza)
```

## Best Practices

1. **Encapsulation**: Use properties for controlled access
2. **Single Responsibility**: One class, one purpose
3. **Composition over Inheritance**: Prefer has-a over is-a
4. **Private Attributes**: Use `_prefix` for internal use
5. **Docstrings**: Document classes and public methods
6. **Type Hints**: Use for better code clarity and optimization
7. **Magic Methods**: Implement `__str__`, `__repr__`, `__eq__` appropriately
8. **super()**: Always use super() for parent class calls

## Performance Considerations

- Type-annotated attributes enable C compilation optimizations
- Properties have slight overhead compared to direct access
- Multiple inheritance adds complexity to method lookups
- `__slots__` can reduce memory usage (if implemented)

## Next Steps

- [Inheritance and Polymorphism](inheritance.md)
- [Magic Methods Reference](magic-methods.md)
- [Type System](../types/hybrid-typing.md)
- [Design Patterns](../examples/patterns.md)
- [Performance Optimization](../advanced/performance.md)
