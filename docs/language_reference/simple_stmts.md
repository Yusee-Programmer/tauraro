# Simple Statements

Simple statements are comprised within a single logical line. Several simple statements may occur on a single line separated by semicolons.

## Expression Statements

Expression statements are used (mostly interactively) to compute and write a value, or (usually) to call a procedure (a function that returns no meaningful result; in Python, procedures return the value `None`). Other uses of expression statements are allowed and occasionally useful.

```tauraro
# Function call as expression statement
print("Hello, World!")

# Method call
numbers = [1, 2, 3]
numbers.append(4)

# Generator expression
(x**2 for x in range(10))
```

## Assignment Statements

Assignment statements are used to (re)bind names to values and to modify attributes or items of mutable objects.

### Basic Assignment

```tauraro
# Simple assignment
x = 42
name = "Alice"

# Multiple assignment
a, b, c = 1, 2, 3

# Swapping variables
a, b = b, a
```

### Augmented Assignment

```tauraro
# Augmented assignments
x += 1      # x = x + 1
y *= 2      # y = y * 2
z -= 3      # z = z - 3
```

### Annotated Assignment

Tauraro supports optional type annotations:

```tauraro
# Variable with type annotation
name: str = "Alice"
age: int = 30
height: float = 5.8

# Variable with annotation but no initial value
count: int
```

## The `assert` Statement

Assert statements are a convenient way to insert debugging assertions into a program:

```tauraro
# Simple assertion
assert x > 0

# Assertion with message
assert x > 0, "x must be positive"

# Common use cases
def divide(a, b):
    assert b != 0, "Division by zero"
    return a / b

def factorial(n):
    assert n >= 0, "Factorial is not defined for negative numbers"
    if n <= 1:
        return 1
    return n * factorial(n - 1)
```

## The `pass` Statement

`pass` is a null operation — when it is executed, nothing happens. It is useful as a placeholder when a statement is required syntactically, but no code needs to be executed:

```tauraro
# Placeholder in function definition
def todo():
    pass

# Placeholder in class definition
class MyClass:
    pass

# Placeholder in conditional
if condition:
    pass  # To be implemented
else:
    do_something()
```

## The `del` Statement

Deletion is recursively defined very similar to the way assignment is defined.

```tauraro
# Delete variables
x = 42
del x

# Delete items from list
numbers = [1, 2, 3, 4]
del numbers[0]      # Remove first element
del numbers[1:3]    # Remove slice

# Delete items from dictionary
person = {"name": "Alice", "age": 30}
del person["age"]

# Delete attributes
class Person:
    def __init__(self):
        self.name = "Alice"

p = Person()
del p.name
```

## The `return` Statement

`return` leaves the current function call with the expression list (or `None`) as return value:

```tauraro
# Return without value
def procedure():
    print("Doing something")
    return

# Return with value
def square(x):
    return x**2

# Return multiple values
def coordinates():
    return 10, 20

x, y = coordinates()

# Conditional return
def process_data(data):
    if not data:
        return None
    return data.upper()
```

## The `yield` Statement

The `yield` statement is used in generator functions:

```tauraro
# Simple generator
def count_up_to(max):
    count = 1
    while count <= max:
        yield count
        count += 1

# Using the generator
for number in count_up_to(5):
    print(number)

# Generator with multiple yields
def fibonacci():
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b
```

## The `raise` Statement

The `raise` statement is used to raise an exception:

```tauraro
# Raise built-in exception
raise ValueError("Invalid value")

# Raise with no arguments (re-raise current exception)
try:
    1 / 0
except ZeroDivisionError:
    print("Caught division by zero")
    raise  # Re-raise the same exception

# Raise exception with traceback
raise ValueError("Error message") from OSError("OS error")

# Raise existing exception instance
e = ValueError("My error")
raise e
```

## The `break` Statement

`break` terminates the nearest enclosing loop:

```tauraro
# Break in for loop
for i in range(10):
    if i == 5:
        break
    print(i)  # Prints 0, 1, 2, 3, 4

# Break in while loop
count = 0
while True:
    if count >= 5:
        break
    print(count)
    count += 1

# Break in nested loops
for i in range(3):
    for j in range(3):
        if j == 1:
            break
        print(f"i={i}, j={j}")
```

## The `continue` Statement

`continue` continues with the next iteration of the nearest enclosing loop:

```tauraro
# Continue in for loop
for i in range(5):
    if i == 2:
        continue
    print(i)  # Prints 0, 1, 3, 4

# Continue in while loop
count = 0
while count < 5:
    count += 1
    if count == 3:
        continue
    print(count)  # Prints 1, 2, 4, 5

# Continue in nested loops
for i in range(3):
    for j in range(3):
        if j == 1:
            continue
        print(f"i={i}, j={j}")  # j=1 is skipped in each inner loop
```

## The `import` Statement

The `import` statement is used to import modules:

```tauraro
# Import entire module
import math
result = math.sqrt(16)

# Import with alias
import numpy as np
import math as m

# Import specific items
from math import sqrt, pi
result = sqrt(16)

# Import all (not recommended)
from math import *

# Import with aliases
from math import sqrt as square_root
```

## The `global` Statement

The `global` statement is a declaration which holds for the entire current code block:

```tauraro
count = 0

def increment():
    global count
    count += 1

def decrement():
    global count
    count -= 1

# Multiple globals
def modify_globals():
    global x, y, z
    x = 1
    y = 2
    z = 3
```

## The `nonlocal` Statement

The `nonlocal` statement causes the listed identifiers to refer to previously bound variables in the nearest enclosing scope:

```tauraro
def outer():
    x = 1
    
    def inner():
        nonlocal x
        x = 2
    
    print(x)  # 1
    inner()
    print(x)  # 2

# Multiple nonlocals
def outer():
    x, y = 1, 2
    
    def inner():
        nonlocal x, y
        x, y = 10, 20
    
    print(x, y)  # 1 2
    inner()
    print(x, y)  # 10 20
```

## Type Hint Statements

Tauraro supports type hints for variables, function parameters, and return values:

```tauraro
# Variable type hints
name: str = "Alice"
numbers: list[int] = [1, 2, 3]
person: dict[str, int] = {"age": 30}

# Function type hints
def greet(name: str) -> str:
    return f"Hello, {name}!"

def calculate_area(length: float, width: float) -> float:
    return length * width

# Complex type hints
from typing import List, Dict, Optional

def process_items(items: List[str]) -> Dict[str, int]:
    return {item: len(item) for item in items}

def find_user(user_id: int) -> Optional[str]:
    # Returns None or a string
    pass
```

## FFI Import Statements

Tauraro supports importing C functions through FFI:

```tauraro
# Import C functions from a library
extern "libm.so" {
    fn sqrt(x: double) -> double
    fn sin(x: double) -> double
    fn cos(x: double) -> double
}

# Use imported C functions
result = sqrt(16.0)
print(result)  # 4.0

# Import with different library name on Windows
extern "msvcrt.dll" {
    fn printf(format: str, ...) -> int
} if windows else {
    fn printf(format: str, ...) -> int
}
```

## Async/Await Statements

Tauraro supports asynchronous programming:

```tauraro
import asyncio

# Async function definition
async def fetch_data(url: str) -> str:
    # Simulate async operation
    await asyncio.sleep(1)
    return f"Data from {url}"

# Async context manager
async def main():
    async with some_async_context() as resource:
        data = await fetch_data("http://example.com")
        print(data)

# Async for loop
async def process_items():
    async for item in async_generator():
        print(item)

# Running async code
asyncio.run(main())
```

## Match Statement (Pattern Matching)

Tauraro supports pattern matching similar to Python's match statement:

```tauraro
# Simple pattern matching
def handle_data(data):
    match data:
        case 0:
            return "zero"
        case 1:
            return "one"
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
```

## Decorator Statements

Functions and classes can be decorated:

```tauraro
# Simple decorator
@staticmethod
def utility_function():
    return "Utility"

# Multiple decorators
@classmethod
@property
def class_property(cls):
    return "Class property"

# Decorator with arguments
@retry(max_attempts=3)
def unreliable_function():
    # Might fail, will be retried up to 3 times
    pass

# Custom decorator
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
```