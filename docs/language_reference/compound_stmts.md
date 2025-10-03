# Compound Statements

Compound statements contain (groups of) other statements; they affect or control the execution of those other statements in some way.

## The `if` Statement

The `if` statement is used for conditional execution:

```tauraro
# Simple if statement
if x > 0:
    print("Positive")

# If-else statement
if x > 0:
    print("Positive")
else:
    print("Non-positive")

# If-elif-else statement
if x > 0:
    print("Positive")
elif x < 0:
    print("Negative")
else:
    print("Zero")

# Nested if statements
if x > 0:
    if x > 10:
        print("Large positive")
    else:
        print("Small positive")
```

## The `while` Statement

The `while` statement is used for repeated execution as long as an expression is true:

```tauraro
# Simple while loop
count = 0
while count < 5:
    print(count)
    count += 1

# While loop with break
while True:
    user_input = input("Enter 'quit' to exit: ")
    if user_input == "quit":
        break
    print(f"You entered: {user_input}")

# While loop with continue
count = 0
while count < 10:
    count += 1
    if count % 2 == 0:
        continue
    print(count)  # Prints odd numbers
```

## The `for` Statement

The `for` statement is used to iterate over the elements of a sequence or other iterable object:

```tauraro
# Simple for loop
for i in range(5):
    print(i)

# For loop over a list
fruits = ["apple", "banana", "cherry"]
for fruit in fruits:
    print(fruit)

# For loop with enumerate
for i, fruit in enumerate(fruits):
    print(f"{i}: {fruit}")

# For loop over a dictionary
person = {"name": "Alice", "age": 30}
for key, value in person.items():
    print(f"{key}: {value}")

# For loop with range
for i in range(0, 10, 2):
    print(i)  # Prints 0, 2, 4, 6, 8

# Nested for loops
matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
for row in matrix:
    for element in row:
        print(element)
```

## The `try` Statement

The `try` statement specifies exception handlers and/or cleanup code for a group of statements:

```tauraro
# Simple try-except
try:
    result = 10 / 0
except ZeroDivisionError:
    print("Cannot divide by zero")

# Try-except-else
try:
    file = open("data.txt")
except FileNotFoundError:
    print("File not found")
else:
    print("File opened successfully")
    file.close()

# Try-except-finally
try:
    file = open("data.txt")
    data = file.read()
except FileNotFoundError:
    print("File not found")
finally:
    # This always executes
    if 'file' in locals():
        file.close()

# Multiple except clauses
try:
    value = int(input("Enter a number: "))
    result = 10 / value
except ValueError:
    print("Invalid number")
except ZeroDivisionError:
    print("Cannot divide by zero")
except Exception as e:
    print(f"Unexpected error: {e}")

# Catching multiple exceptions
try:
    # Some code
    pass
except (ValueError, TypeError) as e:
    print(f"Value or type error: {e}")
```

## The `with` Statement

The `with` statement is used to wrap the execution of a block with methods defined by a context manager:

```tauraro
# Simple with statement
with open("data.txt", "r") as file:
    content = file.read()
    print(content)
# File is automatically closed

# Multiple context managers
with open("input.txt", "r") as infile, open("output.txt", "w") as outfile:
    data = infile.read()
    outfile.write(data.upper())

# Custom context manager
class MyContextManager:
    def __enter__(self):
        print("Entering context")
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        print("Exiting context")
        if exc_type is not None:
            print(f"Exception occurred: {exc_value}")
        return False  # Don't suppress exceptions

with MyContextManager() as cm:
    print("Inside context")
    # raise ValueError("Test exception")
```

## Function Definitions

A function definition defines a user-defined function object:

```tauraro
# Simple function
def greet(name):
    return f"Hello, {name}!"

# Function with default arguments
def greet_with_title(name, title="Mr./Ms."):
    return f"Hello, {title} {name}!"

# Function with variable arguments
def sum_all(*args):
    total = 0
    for num in args:
        total += num
    return total

# Function with keyword arguments
def create_person(name, age, **kwargs):
    person = {"name": name, "age": age}
    person.update(kwargs)
    return person

# Function with type hints
def calculate_area(length: float, width: float) -> float:
    return length * width

# Function with docstring
def factorial(n):
    """
    Calculate the factorial of n.
    
    Args:
        n (int): A non-negative integer
        
    Returns:
        int: The factorial of n
        
    Raises:
        ValueError: If n is negative
    """
    if n < 0:
        raise ValueError("Factorial is not defined for negative numbers")
    if n <= 1:
        return 1
    return n * factorial(n - 1)

# Lambda function (expression)
square = lambda x: x**2
```

## Class Definitions

A class definition defines a class object:

```tauraro
# Simple class
class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def introduce(self):
        return f"I'm {self.name}, {self.age} years old"

# Class with class variables
class Counter:
    count = 0  # Class variable
    
    def __init__(self):
        Counter.count += 1  # Access class variable
        self.instance_id = Counter.count  # Instance variable

# Class with inheritance
class Animal:
    def __init__(self, name):
        self.name = name
    
    def speak(self):
        pass

class Dog(Animal):
    def speak(self):
        return "Woof!"

class Cat(Animal):
    def speak(self):
        return "Meow!"

# Class with multiple inheritance
class Mammal:
    def breathe(self):
        return "Breathing air"

class Carnivore:
    def eat(self):
        return "Eating meat"

class Lion(Mammal, Carnivore):
    def roar(self):
        return "ROAR!"

# Class with special methods
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def __str__(self):
        return f"Point({self.x}, {self.y})"
    
    def __repr__(self):
        return f"Point(x={self.x}, y={self.y})"
    
    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)
    
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

# Class with property decorators
class Circle:
    def __init__(self, radius):
        self._radius = radius
    
    @property
    def radius(self):
        return self._radius
    
    @radius.setter
    def radius(self, value):
        if value < 0:
            raise ValueError("Radius cannot be negative")
        self._radius = value
    
    @property
    def area(self):
        return 3.14159 * self._radius**2

# Class with class methods and static methods
class MathUtils:
    @staticmethod
    def add(x, y):
        return x + y
    
    @classmethod
    def get_pi(cls):
        return 3.14159
```

## Coroutines

Tauraro supports asynchronous coroutines:

```tauraro
import asyncio

# Async function definition
async def fetch_data(url):
    # Simulate async operation
    await asyncio.sleep(1)
    return f"Data from {url}"

# Async generator
async def async_counter(max_count):
    count = 0
    while count < max_count:
        await asyncio.sleep(0.1)
        yield count
        count += 1

# Async context manager
class AsyncContextManager:
    async def __aenter__(self):
        print("Entering async context")
        return self
    
    async def __aexit__(self, exc_type, exc_value, traceback):
        print("Exiting async context")

# Using async features
async def main():
    # Await coroutine
    data = await fetch_data("http://example.com")
    print(data)
    
    # Async for loop
    async for count in async_counter(5):
        print(count)
    
    # Async with statement
    async with AsyncContextManager():
        print("Inside async context")

# Run async code
asyncio.run(main())
```

## Match Statement

The match statement is used for pattern matching:

```tauraro
# Simple pattern matching
def handle_data(data):
    match data:
        case 0:
            return "zero"
        case 1:
            return "one"
        case x if x > 10:
            return f"large number: {x}"
        case _:
            return "other"

# Pattern matching with sequences
def handle_coordinates(point):
    match point:
        case (0, 0):
            return "origin"
        case (0, y):
            return f"on y-axis at {y}"
        case (x, 0):
            return f"on x-axis at {x}"
        case (x, y) if x == y:
            return f"on diagonal at ({x}, {y})"
        case (x, y):
            return f"at ({x}, {y})"

# Pattern matching with objects
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

def where_is(point):
    match point:
        case Point(x=0, y=0):
            return "origin"
        case Point(x=0, y=y):
            return f"on y-axis at {y}"
        case Point(x=x, y=0):
            return f"on x-axis at {x}"
        case Point(x=x, y=y):
            return f"at ({x}, {y})"
        case _:
            return "not a point"

# Pattern matching with dictionaries
def handle_request(request):
    match request:
        case {"method": "GET", "url": url}:
            return f"GET request to {url}"
        case {"method": "POST", "url": url, "data": data}:
            return f"POST request to {url} with data: {data}"
        case {"method": method}:
            return f"Unknown method: {method}"
        case _:
            return "Invalid request format"
```

## Decorators

Functions and classes can be decorated:

```tauraro
# Simple decorator
def my_decorator(func):
    def wrapper(*args, **kwargs):
        print("Before function call")
        result = func(*args, **kwargs)
        print("After function call")
        return result
    return wrapper

@my_decorator
def greet(name):
    return f"Hello, {name}!"

# Decorator with arguments
def repeat(times):
    def decorator(func):
        def wrapper(*args, **kwargs):
            for _ in range(times):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def say_hello():
    print("Hello!")

# Multiple decorators
@staticmethod
@property
def class_property(cls):
    return "Class property"

# Class decorator
def add_str_method(cls):
    def __str__(self):
        return f"{cls.__name__} object"
    cls.__str__ = __str__
    return cls

@add_str_method
class MyClass:
    pass
```

## Type Definitions

Tauraro supports type definitions for complex type hints:

```tauraro
from typing import TypeVar, Generic, Union, Optional

# Type aliases
Vector = list[float]
ConnectionOptions = dict[str, str]

# Type variables
T = TypeVar('T')
U = TypeVar('U')

# Generic classes
class Stack(Generic[T]):
    def __init__(self) -> None:
        self._items: list[T] = []
    
    def push(self, item: T) -> None:
        self._items.append(item)
    
    def pop(self) -> T:
        return self._items.pop()

# Union types
def process_id(user_id: Union[int, str]) -> str:
    if isinstance(user_id, int):
        return f"ID_{user_id}"
    return user_id

# Optional types
def find_user(user_id: int) -> Optional[str]:
    # Returns None or a string
    pass
```

## Module-Level Code

Code at the module level (not inside functions or classes):

```tauraro
#!/usr/bin/env tauraro
"""Module docstring."""

# Module-level constants
VERSION = "1.0.0"
DEBUG = True

# Module-level variables
counter = 0

# Module-level functions
def utility_function():
    return "Utility"

# Module-level classes
class UtilityClass:
    pass

# Module execution code (runs when module is imported or executed)
if __name__ == "__main__":
    # This code runs only when the script is executed directly
    print("Running as main program")
    print(f"Version: {VERSION}")
```