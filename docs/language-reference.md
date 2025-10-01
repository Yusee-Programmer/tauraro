# TauraroLang Language Reference

**Complete reference for TauraroLang syntax, semantics, and language features**

## Table of Contents

1. [Overview](#overview)
2. [Lexical Structure](#lexical-structure)
3. [Data Types](#data-types)
4. [Variables and Constants](#variables-and-constants)
5. [Operators](#operators)
6. [Control Flow](#control-flow)
7. [Functions](#functions)
8. [Classes and Objects](#classes-and-objects)
9. [Method Calls](#method-calls)
10. [Collections](#collections)
11. [Error Handling](#error-handling)
12. [Async Programming](#async-programming)
13. [Type System](#type-system)
14. [Memory Management](#memory-management)
15. [Modules and Imports](#modules-and-imports)

## Overview

TauraroLang is a dynamically typed programming language that uses 100% Python syntax. It features a clean, expressive syntax inspired by Python while maintaining simplicity and readability. Tauraro files use the .tr extension.

### Key Characteristics

- **Dynamic Typing**: Variables can hold values of any type
- **Python Syntax**: 100% compatible with Python syntax
- **Expression-Based**: Most constructs are expressions that return values
- **Memory Safe**: Automatic memory management with manual control options
- **Interoperable**: Seamless integration with C and Python

## Lexical Structure

### Comments

```python
# Single-line comment

"""
Multi-line comment
Can span multiple lines
"""

"""
Documentation comment
Used for generating documentation
"""
```

### Identifiers

Identifiers must start with a letter or underscore, followed by letters, digits, or underscores:

```python
valid_identifier
_private_var
MyClass
variable123
```

### Keywords

Reserved keywords in TauraroLang (Python keywords):

```
and         def         exec        not         yield
as          elif        finally     or
assert      else        for         pass
break       except      from        print
class       exec        global      raise
continue    finally     if          return
def         for         import      try
del         from        in          while
elif        global      is          with
else        if          lambda      yield
except      import      nonlocal
```

### Literals

#### Integer Literals
```python
42          # decimal
0x2A        # hexadecimal
0o52        # octal
0b101010    # binary
1_000_000   # with separators
```

#### Float Literals
```python
3.14
2.5e10
1.5E-5
.5          # 0.5
5.          # 5.0
```

#### String Literals
```python
"Hello, World!"
'Single quotes'
"""
Multi-line string
with line breaks
"""
f"Formatted string with {variable}"
r"Raw string with \n literal backslashes"
```

#### Boolean and Special Literals
```python
True
False
None
```

## Data Types

### Primitive Types

#### Integer (`int`)
64-bit signed integers:
```python
age = 25
negative = -100
large = 9223372036854775807
```

#### Float (`float`)
64-bit floating-point numbers:
```python
pi = 3.14159
scientific = 1.23e-4
infinity = float("inf")
```

#### Boolean (`bool`)
```python
is_active = True
is_complete = False
```

#### String (`str`)
UTF-8 encoded strings:
```python
name = "TauraroLang"
greeting = f"Hello, {name}!"
multiline = """
    This is a
    multi-line string
"""
```

#### None Type
```python
empty = None
```

### Collection Types

#### List (`list`)
Ordered, mutable collections:
```python
numbers = [1, 2, 3, 4, 5]
mixed = [1, "hello", True, 3.14]
empty_list = []

# List operations
numbers.append(6)
numbers[0] = 10
first = numbers[0]
length = len(numbers)
```

#### Dictionary (`dict`)
Key-value mappings:
```python
person = {
    "name": "Alice",
    "age": 30,
    "city": "New York"
}

# Dictionary operations
person["email"] = "alice@example.com"
name = person["name"]
keys = person.keys()
```

#### Tuple
Immutable ordered collections:
```python
coordinates = (10, 20)
rgb = (255, 128, 0)
single = (42,)  # Single element tuple

# Tuple unpacking
x, y = coordinates
```

## Variables and Constants

### Variable Declaration

```python
# Basic declaration
x = 42
name = "TauraroLang"

# Multiple assignment
a, b, c = 1, 2, 3
x, y = (10, 20)

# Type annotations (optional)
age: int = 25
pi: float = 3.14159
active: bool = True
```

### Assignment Operators

```python
x = 10

# Compound assignment
x += 5      # x = x + 5
x -= 3      # x = x - 3
x *= 2      # x = x * 2
x /= 4      # x = x / 4
x %= 3      # x = x % 3
x **= 2     # x = x ** 2
```

### Constants

```python
# Constants (by convention, use UPPER_CASE)
PI = 3.14159
MAX_SIZE = 1000
```

## Operators

### Arithmetic Operators

```python
a = 10
b = 3

sum = a + b         # 13
diff = a - b        # 7
product = a * b     # 30
quotient = a / b    # 3.333...
remainder = a % b   # 1
power = a ** b      # 1000

# Unary operators
neg = -a            # -10
pos = +a            # 10
```

### Comparison Operators

```python
x = 10
y = 20

x == y      # False (equality)
x != y      # True (inequality)
x < y       # True (less than)
x <= y      # True (less than or equal)
x > y       # False (greater than)
x >= y      # False (greater than or equal)
```

### Logical Operators

```python
a = True
b = False

a and b     # False (logical AND)
a or b      # True (logical OR)
not a       # False (logical NOT)

# Short-circuit evaluation
result = a and expensive_function()  # Only calls function if a is True
```

### Bitwise Operators

```python
x = 12  # 1100 in binary
y = 10  # 1010 in binary

x & y       # 8 (1000) - bitwise AND
x | y       # 14 (1110) - bitwise OR
x ^ y       # 6 (0110) - bitwise XOR
~x          # -13 - bitwise NOT
x << 2      # 48 (110000) - left shift
x >> 2      # 3 (11) - right shift
```

### String Operators

```python
first = "Hello"
second = "World"

greeting = first + " " + second  # "Hello World"
repeated = "Ha" * 3              # "HaHaHa"

# String formatting
name = "Alice"
age = 30
message = f"My name is {name} and I'm {age} years old"
```

### Membership Operators

```python
numbers = [1, 2, 3, 4, 5]
text = "Hello, World!"

2 in numbers        # True
6 in numbers        # False
"Hello" in text     # True
"Goodbye" in text   # False
```

## Control Flow

### Conditional Statements

#### If-Else
```python
age = 18

if age >= 18:
    print("Adult")
elif age >= 13:
    print("Teenager")
else:
    print("Child")

# Ternary operator
status = "Adult" if age >= 18 else "Minor"
```

#### Match Expressions
```python
value = 42

result = match value:
    case 0:
        "zero"
    case 1:
        "one"
    case 2..10:
        "small"
    case 11..100:
        "medium"
    case _:
        "large"

# Pattern matching with types
data = [1, 2, 3]
match data:
    case []:
        print("Empty list")
    case [x]:
        print(f"Single element: {x}")
    case [x, y]:
        print(f"Two elements: {x}, {y}")
    case [x, *rest]:
        print(f"First: {x}, Rest: {rest}")
```

### Loops

#### For Loops
```python
# Iterate over range
for i in range(5):
    print(i)  # 0, 1, 2, 3, 4

# Iterate over collection
fruits = ["apple", "banana", "orange"]
for fruit in fruits:
    print(fruit)

# Enumerate with index
for i, fruit in enumerate(fruits):
    print(f"{i}: {fruit}")

# Dictionary iteration
person = {"name": "Alice", "age": 30}
for key, value in person.items():
    print(f"{key}: {value}")
```

#### While Loops
```python
count = 0
while count < 5:
    print(count)
    count += 1

# Infinite loop with break
while True:
    input = get_input()
    if input == "quit":
        break
    process(input)
```

#### Loop Control
```python
for i in range(10):
    if i == 3:
        continue  # Skip iteration
    if i == 7:
        break     # Exit loop
    print(i)
```

## Functions

### Function Definition

```python
# Basic function
def greet(name):
    return f"Hello, {name}!"

# Function with default parameters
def power(base, exponent=2):
    return base ** exponent

# Function with type annotations
def add(a: int, b: int) -> int:
    return a + b

# Variable arguments
def sum_all(*args):
    total = 0
    for arg in args:
        total += arg
    return total

# Keyword arguments
def create_person(**kwargs):
    return {
        "name": kwargs.get("name", "Unknown"),
        "age": kwargs.get("age", 0)
    }
```

### Function Calls

```python
# Basic call
message = greet("Alice")

# Named arguments
result = power(base=2, exponent=3)

# Unpacking arguments
numbers = [1, 2, 3, 4, 5]
total = sum_all(*numbers)

# Unpacking keyword arguments
person_data = {"name": "Bob", "age": 25}
person = create_person(**person_data)
```

### Lambda Functions

```python
# Anonymous functions
square = lambda x: x * x
add = lambda a, b: a + b

# Higher-order functions
numbers = [1, 2, 3, 4, 5]
squares = map(lambda x: x * x, numbers)
evens = filter(lambda x: x % 2 == 0, numbers)
```

### Closures

```python
def make_counter():
    count = 0
    
    def increment():
        nonlocal count
        count += 1
        return count
    
    return increment

counter = make_counter()
print(counter())  # 1
print(counter())  # 2
```

## Classes and Objects

### Class Definition

```python
class Person:
    # Class variable
    species = "Homo sapiens"
    
    # Constructor
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    # Instance method
    def greet(self):
        return f"Hello, I'm {self.name}"
    
    # Class method
    @classmethod
    def from_string(cls, person_str):
        name, age = person_str.split(",")
        return cls(name, int(age))
    
    # Static method
    @staticmethod
    def is_adult(age):
        return age >= 18
    
    # Property
    @property
    def description(self):
        return f"{self.name} is {self.age} years old"
```

### Inheritance

```python
class Student(Person):
    def __init__(self, name, age, student_id):
        super().__init__(name, age)
        self.student_id = student_id
    
    def greet(self):
        return f"Hi, I'm {self.name}, student #{self.student_id}"
    
    def study(self, subject):
        return f"{self.name} is studying {subject}"
```

### Object Usage

```python
# Create instances
person = Person("Alice", 30)
student = Student("Bob", 20, "S12345")

# Method calls
print(person.greet())
print(student.study("Mathematics"))

# Property access
print(person.description)

# Class and static methods
person2 = Person.from_string("Charlie,25")
print(Person.is_adult(17))  # False
```

## Method Calls

TauraroLang supports both function calls and method calls with proper object-oriented semantics. Method calls are distinguished from function calls by the dot notation and are handled differently in the parser and virtual machine.

### Basic Method Calls

Methods are called on objects using the dot notation followed by parentheses:

```python
my_list = [1, 2, 3, 4, 5]

# Method call with no arguments
my_list.append(6)

# Method call with arguments
my_list.insert(0, 0)

# Method call with keyword arguments
my_dict.get("key", default="default_value")
```

### Method Call Syntax

Method calls in TauraroLang follow the same syntax as function calls but are invoked on objects:

```python
object.method()                    # No arguments
object.method(arg1, arg2)         # Positional arguments
object.method(name=value)         # Keyword arguments
object.method(arg1, name=value)   # Mixed arguments
```

### Instance Methods

When calling instance methods on class objects, the instance is automatically passed as the first argument (conventionally named `self`):

```python
class Calculator:
    def __init__(self, initial_value=0):
        self.value = initial_value
    
    def add(self, number):
        self.value += number
        return self.value
    
    def get_value(self):
        return self.value

# Create an instance
calc = Calculator(10)

# Method calls - 'self' is automatically passed
calc.add(5)        # self.value becomes 15
result = calc.get_value()  # Returns 15
```

### Method Resolution Order (MRO)

TauraroLang uses the C3 linearization algorithm for method resolution, ensuring proper inheritance handling:

```python
class Animal:
    def speak(self):
        return "Some generic sound"

class Mammal(Animal):
    def speak(self):
        return "Mammal sound"

class Dog(Mammal):
    def speak(self):
        return "Woof!"

class Pet(Dog):
    def play(self):
        return "Playing!"

my_pet = Pet()
my_pet.speak()  # Returns "Woof!" - follows MRO
my_pet.play()   # Returns "Playing!"
```

### Built-in Method Calls

All built-in types support method calls:

```python
# String methods
text = "Hello, World!"
upper_text = text.upper()
lower_text = text.lower()

# List methods
numbers = [1, 2, 3]
numbers.append(4)
numbers.extend([5, 6])
popped = numbers.pop()

# Dictionary methods
person = {"name": "Alice", "age": 30}
keys = person.keys()
values = person.values()
person.update({"city": "New York"})
```

### Chained Method Calls

Methods can be chained when they return objects that have methods:

```python
class StringBuilder:
    def __init__(self):
        self.content = ""
    
    def add(self, text):
        self.content += text
        return self  # Return self for chaining
    
    def get(self):
        return self.content

builder = StringBuilder()
result = builder.add("Hello").add(" ").add("World!").get()
# result is "Hello World!"
```

### Special Method Calls

TauraroLang supports calling special methods (dunder methods) that define object behavior:

```python
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)
    
    def __str__(self):
        return f"Vector({self.x}, {self.y})"

v1 = Vector(1, 2)
v2 = Vector(3, 4)
v3 = v1 + v2  # Calls __add__ method
print(v3)     # Calls __str__ method
```

### Super Method Calls

The `super()` function allows calling parent class methods:

```python
class Animal:
    def __init__(self, name):
        self.name = name
    
    def speak(self):
        return f"{self.name} makes a sound"

class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name)  # Call parent constructor
        self.breed = breed
    
    def speak(self):
        parent_speak = super().speak()  # Call parent method
        return f"{parent_speak} and barks"

dog = Dog("Buddy", "Golden Retriever")
dog.speak()  # Returns "Buddy makes a sound and barks"
```

Method calls in TauraroLang are fully compatible with Python's method call semantics, providing a familiar and powerful object-oriented programming experience.

## Collections

### List Operations

```python
numbers = [1, 2, 3, 4, 5]

# Access and modification
numbers[0] = 10
numbers.append(6)
numbers.insert(2, 99)
numbers.remove(3)
popped = numbers.pop()

# Slicing
subset = numbers[1:4]    # [2, 99, 4]
reversed = numbers[::-1] # Reverse order

# List comprehensions
squares = [x * x for x in range(10)]
evens = [x for x in numbers if x % 2 == 0]
```

### Dictionary Operations

```python
person = {"name": "Alice", "age": 30}

# Access and modification
person["city"] = "New York"
name = person.get("name", "Unknown")
person.update({"email": "alice@example.com", "phone": "123-456-7890"})

# Dictionary comprehensions
squares = {x: x*x for x in range(5)}
filtered = {k: v for k, v in person.items() if len(str(v)) > 3}
```

### Set Operations

```python
set1 = {1, 2, 3, 4, 5}
set2 = {4, 5, 6, 7, 8}

# Set operations
union = set1 | set2         # {1, 2, 3, 4, 5, 6, 7, 8}
intersection = set1 & set2  # {4, 5}
difference = set1 - set2    # {1, 2, 3}
symmetric_diff = set1 ^ set2 # {1, 2, 3, 6, 7, 8}

# Set methods
set1.add(9)
set1.remove(1)
is_subset = set1.issubset(set2)
```

## Error Handling

### Try-Except Blocks

```python
try:
    result = risky_operation()
    print(f"Success: {result}")
except ValueError as e:
    print(f"Value error: {e}")
except Exception as e:
    print(f"General error: {e}")
finally:
    print("Cleanup code")
```

### Raising Exceptions

```python
def divide(a, b):
    if b == 0:
        raise ValueError("Cannot divide by zero")
    return a / b

def validate_age(age):
    if age < 0:
        raise ValueError("Age cannot be negative")
    if age > 150:
        raise ValueError("Age seems unrealistic")
```

### Custom Exceptions

```python
class CustomError(Exception):
    def __init__(self, message, code):
        super().__init__(message)
        self.code = code

try:
    raise CustomError("Something went wrong", 500)
except CustomError as e:
    print(f"Error {e.code}: {e}")
```

## Async Programming

### Async Functions

```python
async def fetch_data(url):
    response = await http_get(url)
    data = await response.json()
    return data

async def process_urls(urls):
    tasks = []
    for url in urls:
        tasks.append(fetch_data(url))
    
    results = await gather(*tasks)
    return results
```

### Async Context Managers

```python
async def async_file_operation():
    async with open_async("file.txt") as f:
        content = await f.read()
        return content
```

### Generators and Async Generators

```python
# Generator function
def fibonacci():
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b

# Async generator
async def async_counter(max_count):
    for i in range(max_count):
        await sleep(1)
        yield i
```

## Type System

### Type Annotations

```python
# Variable annotations
name: str = "Alice"
age: int = 30
scores: list[int] = [85, 92, 78]
person: dict[str, any] = {"name": "Bob", "age": 25}

# Function annotations
def process_data(data: list[dict[str, any]]) -> dict[str, int]:
    result: dict[str, int] = {}
    for item in data:
        result[item["name"]] = item["score"]
    return result
```

### Generic Types

```python
# Generic function
def identity[T](value: T) -> T:
    return value

# Generic class
class Container[T]:
    def __init__(self, value: T):
        self.value = value
    
    def get(self) -> T:
        return self.value

int_container = Container[int](42)
str_container = Container[str]("hello")
```

### Union Types

```python
# Union type annotation
def process_id(id: int | str) -> str:
    if type(id) == int:
        return f"ID: {id:06d}"
    else:
        return f"ID: {id.upper()}"

# Optional types
def find_user(name: str) -> User | None:
    # Search logic here
    return found_user or None
```

### Type Checking

```python
# Runtime type checking
def safe_divide(a: any, b: any) -> float:
    if not isinstance(a, (int, float)) or not isinstance(b, (int, float)):
        raise TypeError("Arguments must be numbers")
    if b == 0:
        raise ValueError("Cannot divide by zero")
    return float(a) / float(b)
```

## Memory Management

### Automatic Memory Management

TauraroLang uses automatic garbage collection for most memory management:

```python
# Objects are automatically cleaned up when no longer referenced
def create_large_data():
    data = [i for i in range(1000000)]
    return data  # Memory will be freed when data goes out of scope
```

### Manual Memory Control

For performance-critical code, manual memory management is available:

```python
# Explicit memory allocation
buffer = allocate(1024)  # Allocate 1KB
buffer.write(0, b"Hello")
data = buffer.read(0, 5)
deallocate(buffer)  # Explicit cleanup

# RAII-style resource management
with managed_resource() as resource:
    resource.use()
    # Resource automatically cleaned up
```

### Weak References

```python
import weakref

class Parent:
    def __init__(self):
        self.children = []

class Child:
    def __init__(self, parent):
        self.parent = weakref.ref(parent)  # Weak reference to avoid cycles
        parent.children.append(self)
```

## Modules and Imports

### Module Definition

```python
# math_utils.tr
def add(a, b):
    return a + b

def multiply(a, b):
    return a * b

PI = 3.14159

# Private function (not exported)
def _internal_helper():
    return "helper"
```

### Import Statements

```python
# Import entire module
import math_utils
result = math_utils.add(2, 3)

# Import specific functions
from math_utils import add, multiply
sum = add(5, 7)
product = multiply(3, 4)

# Import with alias
import math_utils as math
pi_value = math.PI

# Import all exports
from math_utils import *
```

### Package Structure

```
my_package/
├── __init__.tr
├── core.tr
├── utils/
│   ├── __init__.tr
│   ├── helpers.tr
│   └── validators.tr
└── tests/
    └── test_core.tr
```

```python
# Import from package
import my_package.core
from my_package.utils.validators import validate_email
```

---

This language reference provides a comprehensive overview of TauraroLang's syntax and features. For more detailed examples and tutorials, see the [Getting Started Guide](getting-started.md) and [API Documentation](api-reference.md).