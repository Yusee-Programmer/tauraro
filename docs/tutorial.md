# Tauraro Tutorial

This tutorial introduces the Tauraro programming language through simple examples. It assumes you have some programming experience, preferably with Python.

## Running Tauraro Code

Tauraro programs are stored in files with the `.tr` extension. You can run them using:

```bash
tauraro run my_program.tr
```

You can also start an interactive interpreter (REPL):

```bash
tauraro repl
```

## Using the Interpreter

Start the interpreter and try these commands:

```tauraro
>>> print("Hello, World!")
Hello, World!
>>> 2 + 2
4
>>> import math
>>> math.pi
3.141592653589793
```

## First Program

Create a file called `fibonacci.tr`:

```tauraro
#!/usr/bin/env tauraro

def fibonacci(n):
    """Calculate the nth Fibonacci number."""
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)

def main():
    print("Fibonacci sequence:")
    for i in range(10):
        print(f"F({i}) = {fibonacci(i)}")

if __name__ == "__main__":
    main()
```

Run it with:
```bash
tauraro run fibonacci.tr
```

## Basic Concepts

### Comments

```tauraro
# This is a single-line comment

"""
This is a multi-line comment
(also known as a docstring)
"""
```

### Variables and Assignment

```tauraro
# Simple assignment
name = "Tauraro"
age = 1
pi = 3.14159

# Multiple assignment
a, b, c = 1, 2, 3

# Swapping variables
a, b = b, a
```

### Data Types

```tauraro
# Numbers
integer = 42
floating = 3.14
complex_num = 2 + 3j

# Strings
single_quote = 'Hello'
double_quote = "World"
multiline = """
This is a
multiline string
"""

# Boolean
is_true = True
is_false = False

# None (null value)
nothing = None
```

### Lists

```tauraro
# Creating lists
empty_list = []
numbers = [1, 2, 3, 4, 5]
mixed = [1, "hello", 3.14, True]

# List operations
fruits = ["apple", "banana", "cherry"]
fruits.append("date")        # Add item
first = fruits[0]            # Access item
fruits[1] = "blueberry"      # Modify item
length = len(fruits)         # Get length

# List slicing
subset = fruits[1:3]         # Get items 1 to 2
last = fruits[-1]            # Get last item
```

### Dictionaries

```tauraro
# Creating dictionaries
empty_dict = {}
person = {"name": "Alice", "age": 30, "city": "New York"}

# Dictionary operations
person["email"] = "alice@example.com"  # Add/update item
name = person["name"]                  # Access item
del person["age"]                      # Remove item
keys = list(person.keys())             # Get keys
values = list(person.values())         # Get values
```

### Tuples

```tauraro
# Creating tuples
empty_tuple = ()
coordinates = (10, 20)
person = ("Alice", 30, "Engineer")

# Tuples are immutable
# person[0] = "Bob"  # This would raise an error

# Tuple unpacking
name, age, job = person
x, y = coordinates
```

### Sets

```tauraro
# Creating sets
empty_set = set()
colors = {"red", "green", "blue"}
numbers = {1, 2, 3, 4, 5}

# Set operations
colors.add("yellow")           # Add item
colors.remove("red")           # Remove item
has_blue = "blue" in colors    # Check membership
count = len(colors)            # Get size
```

## Control Flow

### If Statements

```tauraro
age = 20

if age < 13:
    print("Child")
elif age < 20:
    print("Teenager")
else:
    print("Adult")
```

### For Loops

```tauraro
# Simple for loop
for i in range(5):
    print(i)

# Loop over a list
fruits = ["apple", "banana", "cherry"]
for fruit in fruits:
    print(fruit)

# Loop with index
for i, fruit in enumerate(fruits):
    print(f"{i}: {fruit}")

# Loop over a dictionary
person = {"name": "Alice", "age": 30}
for key, value in person.items():
    print(f"{key}: {value}")
```

### While Loops

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
```

### List Comprehensions

```tauraro
# Simple list comprehension
squares = [x**2 for x in range(10)]

# With condition
even_squares = [x**2 for x in range(10) if x % 2 == 0]

# Nested comprehension
matrix = [[i*j for j in range(3)] for i in range(3)]
```

## Functions

### Defining Functions

```tauraro
def greet(name):
    """Simple greeting function."""
    return f"Hello, {name}!"

def add(a, b):
    """Add two numbers."""
    return a + b

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
```

### Lambda Functions

```tauraro
# Simple lambda
square = lambda x: x**2
print(square(5))  # Output: 25

# Lambda in higher-order functions
numbers = [1, 2, 3, 4, 5]
squared = list(map(lambda x: x**2, numbers))
evens = list(filter(lambda x: x % 2 == 0, numbers))
```

## Classes

### Defining Classes

```tauraro
class Dog:
    """A simple dog class."""
    
    # Class variable
    species = "Canis lupus"
    
    def __init__(self, name, age):
        """Initialize a dog instance."""
        self.name = name
        self.age = age
    
    def bark(self):
        """Make the dog bark."""
        return "Woof!"
    
    def get_description(self):
        """Get a description of the dog."""
        return f"{self.name} is {self.age} years old"

# Creating instances
my_dog = Dog("Buddy", 3)
print(my_dog.name)           # Output: Buddy
print(my_dog.bark())         # Output: Woof!
```

### Inheritance

```tauraro
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

# Multiple inheritance
class Mammal:
    def breathe(self):
        return "Breathing air"

class Carnivore:
    def eat(self):
        return "Eating meat"

class Lion(Mammal, Carnivore):
    def roar(self):
        return "ROAR!"
```

## Modules

### Importing Modules

```tauraro
# Import entire module
import math
result = math.sqrt(16)

# Import specific functions
from math import sqrt, pi
result = sqrt(16)

# Import with alias
import numpy as np
import math as m

# Import all (not recommended)
from math import *
```

### Creating Modules

Create a file called `mymodule.tr`:

```tauraro
"""A simple module example."""

def hello(name):
    return f"Hello, {name}!"

PI = 3.14159

class Calculator:
    def add(self, a, b):
        return a + b
    
    def multiply(self, a, b):
        return a * b
```

Use it in another file:

```tauraro
import mymodule

print(mymodule.hello("World"))
print(mymodule.PI)

calc = mymodule.Calculator()
print(calc.add(2, 3))
```

## Error Handling

```tauraro
# Basic try/except
try:
    result = 10 / 0
except ZeroDivisionError:
    print("Cannot divide by zero!")

# Multiple exceptions
try:
    # Some code that might fail
    pass
except (ValueError, TypeError) as e:
    print(f"An error occurred: {e}")
except Exception as e:
    print(f"Unexpected error: {e}")

# Finally block
try:
    file = open("data.txt")
    # Process file
finally:
    file.close()  # Always executed

# Raise exceptions
def validate_age(age):
    if age < 0:
        raise ValueError("Age cannot be negative")
    return age
```

## File I/O

```tauraro
# Reading a file
try:
    with open("data.txt", "r") as file:
        content = file.read()
        print(content)
except FileNotFoundError:
    print("File not found")

# Writing to a file
with open("output.txt", "w") as file:
    file.write("Hello, World!\n")
    file.write("This is a new line.")

# Reading lines
with open("data.txt", "r") as file:
    lines = file.readlines()
    for line in lines:
        print(line.strip())
```

## Standard Library Examples

### Working with JSON

```tauraro
import json

# Convert to JSON
data = {"name": "Alice", "age": 30}
json_string = json.dumps(data)
print(json_string)

# Parse JSON
json_string = '{"name": "Bob", "age": 25}'
data = json.loads(json_string)
print(data["name"])
```

### Working with Dates and Times

```tauraro
import datetime

# Current date and time
now = datetime.datetime.now()
print(now)

# Create specific date
birthday = datetime.datetime(1990, 5, 15)
print(birthday)

# Date arithmetic
tomorrow = now + datetime.timedelta(days=1)
print(tomorrow)
```

### Working with Regular Expressions

```tauraro
import re

# Search for pattern
text = "The phone number is 123-456-7890"
pattern = r"\d{3}-\d{3}-\d{4}"
match = re.search(pattern, text)
if match:
    print(f"Found: {match.group()}")

# Find all matches
text = "Call 123-456-7890 or 098-765-4321"
numbers = re.findall(pattern, text)
print(numbers)
```

## Next Steps

This tutorial covered the basics of Tauraro. For more advanced topics, check out:

- [Language Reference](language_reference/) for detailed syntax information
- [Standard Library](library/) documentation for built-in modules
- [Examples](../examples/) directory for practical code samples
- [Python Compatibility](python_compatibility.md) guide for Python developers