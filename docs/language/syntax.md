# Tauraro Syntax Overview

Tauraro is Python-compatible, meaning valid Python code is valid Tauraro code. This guide covers the syntax with Tauraro-specific enhancements.

## Comments

```python
# Single-line comment

"""
Multi-line comment
or docstring
"""

'''
Also a multi-line
comment
'''
```

## Variables and Assignment

```python
# Dynamic typing (Python-style)
x = 10
name = "Alice"
is_valid = True

# Static typing (Tauraro-enhanced)
age: int = 25
price: float = 19.99
message: str = "Hello"

# Multiple assignment
a, b, c = 1, 2, 3

# Swap values
a, b = b, a

# Augmented assignment
count = 0
count += 1
count *= 2
```

## Data Types

### Numeric Types

```python
# Integers
x = 42
big = 1_000_000
hex_val = 0xFF
binary = 0b1010

# Floats
pi = 3.14159
scientific = 1.5e10

# Complex numbers
z = 3 + 4j

# Booleans
is_active = True
is_empty = False
```

### Strings

```python
# Single or double quotes
s1 = 'hello'
s2 = "world"

# Multi-line strings
text = """
This is a
multi-line string
"""

# Raw strings
path = r"C:\Users\Name"

# F-strings (formatted)
name = "Alice"
age = 30
message = f"{name} is {age} years old"

# String operations
upper = "hello".upper()
split = "a,b,c".split(",")
joined = "-".join(["a", "b", "c"])
```

### Collections

```python
# Lists (mutable)
numbers = [1, 2, 3, 4, 5]
mixed = [1, "two", 3.0, True]
numbers.append(6)
numbers[0] = 10

# Tuples (immutable)
coords = (10, 20)
single = (42,)  # Note the comma

# Dictionaries
person = {
    "name": "Alice",
    "age": 30,
    "city": "NYC"
}
person["job"] = "Engineer"

# Sets
unique = {1, 2, 3, 4}
unique.add(5)
```

## Operators

### Arithmetic

```python
# Basic operations
a + b    # Addition
a - b    # Subtraction
a * b    # Multiplication
a / b    # Division (float result)
a // b   # Floor division
a % b    # Modulus
a ** b   # Exponentiation

# Unary
-a       # Negation
+a       # Positive
```

### Comparison

```python
a == b   # Equal
a != b   # Not equal
a < b    # Less than
a > b    # Greater than
a <= b   # Less than or equal
a >= b   # Greater than or equal

# Chaining
1 < x < 10
```

### Logical

```python
a and b  # Logical AND
a or b   # Logical OR
not a    # Logical NOT
```

### Bitwise

```python
a & b    # Bitwise AND
a | b    # Bitwise OR
a ^ b    # Bitwise XOR
~a       # Bitwise NOT
a << b   # Left shift
a >> b   # Right shift
```

### Membership

```python
x in collection      # Is x in collection?
x not in collection  # Is x not in collection?
```

### Identity

```python
a is b      # Same object?
a is not b  # Different object?
```

## Control Flow

### If Statements

```python
if condition:
    # do something
elif other_condition:
    # do something else
else:
    # default case

# Ternary operator
result = "yes" if condition else "no"
```

### For Loops

```python
# Iterate over sequence
for item in items:
    print(item)

# Range-based
for i in range(10):
    print(i)

# With index
for i, item in enumerate(items):
    print(i, item)

# Dictionary iteration
for key, value in dict.items():
    print(key, value)

# Loop control
for item in items:
    if item == target:
        break      # Exit loop
    if skip_item(item):
        continue   # Next iteration
```

### While Loops

```python
while condition:
    # do something

# With else clause
while condition:
    # loop body
else:
    # executed if loop completes normally
```

## Functions

### Basic Functions

```python
def greet(name):
    """Simple function"""
    return f"Hello, {name}!"

# With type annotations
def add(a: int, b: int) -> int:
    """Add two integers"""
    return a + b

# Default parameters
def greet(name="World"):
    return f"Hello, {name}!"

# Variable arguments
def sum_all(*args):
    return sum(args)

# Keyword arguments
def config(**kwargs):
    for key, value in kwargs.items():
        print(f"{key} = {value}")
```

### Lambda Functions

```python
# Anonymous functions
square = lambda x: x ** 2
add = lambda a, b: a + b

# Use in higher-order functions
numbers = [1, 2, 3, 4, 5]
squared = list(map(lambda x: x ** 2, numbers))
evens = list(filter(lambda x: x % 2 == 0, numbers))
```

## Classes

### Basic Classes

```python
class Person:
    """A simple person class"""

    def __init__(self, name: str, age: int):
        self.name = name
        self.age = age

    def greet(self):
        return f"Hi, I'm {self.name}"

    def birthday(self):
        self.age += 1
```

### Inheritance

```python
class Employee(Person):
    """Employee inherits from Person"""

    def __init__(self, name: str, age: int, employee_id: str):
        super().__init__(name, age)
        self.employee_id = employee_id

    def get_id(self):
        return self.employee_id
```

### Special Methods

```python
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __str__(self):
        return f"Vector({self.x}, {self.y})"

    def __repr__(self):
        return f"Vector(x={self.x}, y={self.y})"

    def __len__(self):
        return 2

    def __getitem__(self, index):
        return [self.x, self.y][index]
```

### Class and Static Methods

```python
class MyClass:
    class_var = 0

    @classmethod
    def class_method(cls):
        return cls.class_var

    @staticmethod
    def static_method():
        return "I don't need self or cls"
```

### Properties

```python
class Temperature:
    def __init__(self, celsius: float):
        self._celsius = celsius

    @property
    def celsius(self):
        return self._celsius

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("Temperature below absolute zero!")
        self._celsius = value

    @property
    def fahrenheit(self):
        return self._celsius * 9/5 + 32
```

## Exception Handling

```python
try:
    result = risky_operation()
except ValueError as e:
    print(f"ValueError: {e}")
except ZeroDivisionError:
    print("Cannot divide by zero")
except Exception as e:
    print(f"Unexpected error: {e}")
else:
    # Executed if no exception
    print("Success!")
finally:
    # Always executed
    cleanup()

# Raising exceptions
raise ValueError("Invalid input")
raise RuntimeError("Something went wrong")
```

## Comprehensions

### List Comprehensions

```python
# Basic
squares = [x**2 for x in range(10)]

# With condition
evens = [x for x in range(10) if x % 2 == 0]

# Nested
matrix = [[i*j for j in range(5)] for i in range(5)]
```

### Dict Comprehensions

```python
# Create dictionary
squares = {x: x**2 for x in range(5)}

# Transform dictionary
uppercase = {k: v.upper() for k, v in original.items()}
```

### Set Comprehensions

```python
# Unique squares
squares = {x**2 for x in range(-5, 6)}
```

### Generator Expressions

```python
# Memory-efficient iteration
gen = (x**2 for x in range(1000000))
```

## Modules and Imports

```python
# Import module
import math

# Import specific function
from math import sqrt, pi

# Import with alias
import numpy as np
from datetime import datetime as dt

# Import all (not recommended)
from math import *

# Relative imports
from .module import function
from ..package import Class
```

## Context Managers

```python
# File handling
with open("file.txt", "r") as f:
    content = f.read()

# Multiple context managers
with open("in.txt") as fin, open("out.txt", "w") as fout:
    fout.write(fin.read())

# Custom context manager
class MyContext:
    def __enter__(self):
        print("Entering")
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        print("Exiting")
```

## Decorators

```python
# Function decorator
def timer(func):
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f"{func.__name__} took {end - start:.4f}s")
        return result
    return wrapper

@timer
def slow_function():
    time.sleep(1)

# Class decorator
def singleton(cls):
    instances = {}
    def get_instance(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]
    return get_instance

@singleton
class Database:
    pass
```

## Type Annotations (Tauraro-Enhanced)

```python
# Variable annotations
age: int = 25
prices: list[float] = [9.99, 19.99]
mapping: dict[str, int] = {"a": 1, "b": 2}

# Function annotations
def process(data: list[int]) -> int:
    return sum(data)

# Optional types
from typing import Optional
def find(name: str) -> Optional[str]:
    return result if found else None

# Union types
from typing import Union
def handle(value: Union[int, str]) -> str:
    return str(value)
```

## Assertions

```python
assert condition, "Error message"
assert x > 0, "x must be positive"
```

## Pass Statement

```python
# Placeholder for empty blocks
def not_implemented():
    pass

class EmptyClass:
    pass
```

## Next Steps

- [Data Types in Detail](data-types.md)
- [Functions Guide](functions.md)
- [Classes and OOP](classes.md)
- [Type System](../types/hybrid-typing.md)
