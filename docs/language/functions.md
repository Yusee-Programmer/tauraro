# Functions

Functions in Tauraro work exactly like Python, with full support for all Python function features.

## Function Basics

### Defining Functions

```python
def greet():
    print("Hello!")

def greet_person(name):
    print(f"Hello, {name}!")

def add(a, b):
    return a + b

# Multiple return values
def min_max(numbers):
    return min(numbers), max(numbers)

minimum, maximum = min_max([1, 2, 3, 4, 5])
```

### Type Annotations

Tauraro enforces type annotations at runtime (unlike Python).

```python
# Annotated function with enforced types
def add_numbers(x: int, y: int) -> int:
    return x + y

add_numbers(5, 10)     # OK
add_numbers(5, "10")   # ERROR! Type mismatch

# Unannotated function remains dynamic
def add_dynamic(x, y):
    return x + y

add_dynamic(5, 10)     # OK
add_dynamic(5, "10")   # OK - returns "510"
```

### Docstrings

```python
def calculate_area(width, height):
    """
    Calculate the area of a rectangle.

    Args:
        width: The width of the rectangle
        height: The height of the rectangle

    Returns:
        The area (width * height)
    """
    return width * height

# Access docstring
print(calculate_area.__doc__)
help(calculate_area)
```

## Parameters and Arguments

### Positional Arguments

```python
def power(base, exponent):
    return base ** exponent

power(2, 3)        # 8
```

### Keyword Arguments

```python
def describe_pet(animal, name):
    print(f"I have a {animal} named {name}")

describe_pet(animal="dog", name="Buddy")
describe_pet(name="Buddy", animal="dog")  # Order doesn't matter
describe_pet("dog", name="Buddy")         # Mix positional and keyword
```

### Default Arguments

```python
def greet(name, greeting="Hello"):
    print(f"{greeting}, {name}!")

greet("Alice")                # Hello, Alice!
greet("Bob", "Hi")            # Hi, Bob!
greet("Charlie", greeting="Hey")  # Hey, Charlie!
```

**Warning**: Don't use mutable defaults

```python
# BAD - default is shared across calls
def append_to(element, list=[]):
    list.append(element)
    return list

# GOOD - use None and create new list
def append_to(element, list=None):
    if list is None:
        list = []
    list.append(element)
    return list
```

### Variable Arguments (*args)

```python
def sum_all(*numbers):
    return sum(numbers)

sum_all(1, 2, 3)           # 6
sum_all(1, 2, 3, 4, 5)     # 15
```

### Keyword Variable Arguments (**kwargs)

```python
def print_info(**info):
    for key, value in info.items():
        print(f"{key}: {value}")

print_info(name="Alice", age=30, city="NYC")
```

### Combined Parameters

Order matters: positional, *args, keyword-only, **kwargs

```python
def complex_function(a, b, *args, x=10, y=20, **kwargs):
    print(f"a={a}, b={b}")
    print(f"args={args}")
    print(f"x={x}, y={y}")
    print(f"kwargs={kwargs}")

complex_function(1, 2, 3, 4, x=100, z=500)
```

### Positional-Only Parameters (/)

```python
def func(a, b, /, c, d):
    return a + b + c + d

func(1, 2, 3, 4)           # OK
func(1, 2, c=3, d=4)       # OK
func(a=1, b=2, c=3, d=4)   # ERROR - a and b must be positional
```

### Keyword-Only Parameters (*)

```python
def func(a, b, *, c, d):
    return a + b + c + d

func(1, 2, c=3, d=4)       # OK
func(1, 2, 3, 4)           # ERROR - c and d must be keyword
```

## Lambda Functions

Anonymous inline functions.

```python
# Basic lambda
square = lambda x: x**2
square(5)  # 25

# Multiple arguments
add = lambda x, y: x + y
add(3, 4)  # 7

# Use in higher-order functions
numbers = [1, 2, 3, 4, 5]
squares = list(map(lambda x: x**2, numbers))
evens = list(filter(lambda x: x % 2 == 0, numbers))

# Sort by custom key
words = ["apple", "pie", "zoo", "a"]
sorted(words, key=lambda w: len(w))
```

## Closures

Functions that capture variables from enclosing scope.

```python
def make_multiplier(n):
    def multiply(x):
        return x * n
    return multiply

times_3 = make_multiplier(3)
times_5 = make_multiplier(5)

times_3(10)  # 30
times_5(10)  # 50
```

### nonlocal Keyword

Modify variables in enclosing scope.

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
print(counter())  # 3
```

## Decorators

Functions that modify other functions.

```python
# Simple decorator
def log_calls(func):
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        result = func(*args, **kwargs)
        print(f"{func.__name__} returned {result}")
        return result
    return wrapper

@log_calls
def add(a, b):
    return a + b

add(3, 4)
# Output:
# Calling add
# add returned 7
```

### Decorator with Arguments

```python
def repeat(times):
    def decorator(func):
        def wrapper(*args, **kwargs):
            for _ in range(times):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def greet(name):
    print(f"Hello, {name}!")

greet("Alice")
# Prints "Hello, Alice!" three times
```

### Class Decorators

```python
# Method decorators
class MyClass:
    @staticmethod
    def static_method():
        return "Static method"

    @classmethod
    def class_method(cls):
        return f"Class method of {cls.__name__}"

    @property
    def computed_value(self):
        return self._value * 2
```

## Recursion

Functions that call themselves.

```python
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

factorial(5)  # 120

# Tail recursion
def factorial_tail(n, acc=1):
    if n <= 1:
        return acc
    return factorial_tail(n - 1, n * acc)

# Fibonacci
def fib(n):
    if n <= 1:
        return n
    return fib(n-1) + fib(n-2)
```

## Higher-Order Functions

Functions that take or return functions.

```python
def apply_twice(func, x):
    return func(func(x))

def add_five(x):
    return x + 5

apply_twice(add_five, 10)  # 20

# Return function
def make_adder(n):
    return lambda x: x + n

add_10 = make_adder(10)
add_10(5)  # 15
```

## Generators

Functions that yield values lazily.

```python
def countdown(n):
    while n > 0:
        yield n
        n -= 1

for i in countdown(5):
    print(i)  # 5, 4, 3, 2, 1

# Generator expression
squares = (x**2 for x in range(10))
```

### Generator Methods

```python
def my_generator():
    value = yield 1
    print(f"Received: {value}")
    yield 2

gen = my_generator()
print(next(gen))        # 1
print(gen.send(100))    # Prints "Received: 100", returns 2
```

## Async Functions

*Note: If async/await is implemented in Tauraro*

```python
async def fetch_data(url):
    # Simulated async operation
    await asyncio.sleep(1)
    return f"Data from {url}"

async def main():
    result = await fetch_data("https://example.com")
    print(result)
```

## Annotations and Metadata

### Function Attributes

```python
def my_func():
    """A function"""
    pass

my_func.__name__        # 'my_func'
my_func.__doc__         # 'A function'
my_func.__module__      # '__main__'

# Custom attributes
my_func.custom_attr = "value"
```

### Type Hints

```python
from typing import List, Dict, Optional, Union

def process_items(items: List[int]) -> Dict[str, int]:
    return {"count": len(items), "sum": sum(items)}

def find_user(id: int) -> Optional[str]:
    # Returns str or None
    pass

def parse_value(val: Union[int, str]) -> int:
    # Accepts int or str
    return int(val)
```

## Function Best Practices

1. **Single Responsibility**: One function, one purpose
2. **Clear Names**: Use descriptive function names
3. **Short Functions**: Keep functions focused and brief
4. **Docstrings**: Document purpose, parameters, and return value
5. **Type Annotations**: Use for critical functions (enables optimizations)
6. **Avoid Side Effects**: Prefer pure functions when possible
7. **Default Arguments**: Use immutable defaults only
8. **Error Handling**: Validate inputs and handle errors

## Common Patterns

### Factory Functions

```python
def create_person(name, age):
    return {
        "name": name,
        "age": age,
        "greet": lambda: f"Hi, I'm {name}"
    }

person = create_person("Alice", 30)
```

### Callback Functions

```python
def process_data(data, callback):
    result = expensive_operation(data)
    callback(result)

def on_complete(result):
    print(f"Got result: {result}")

process_data(input_data, on_complete)
```

### Partial Application

```python
from functools import partial

def power(base, exponent):
    return base ** exponent

square = partial(power, exponent=2)
cube = partial(power, exponent=3)

square(5)  # 25
cube(5)    # 125
```

## Performance Considerations

- **Type annotations** enable C compilation optimizations (5-10x faster)
- **Generators** save memory for large sequences
- **Local variables** are faster than globals
- **Built-in functions** are optimized in C
- **List comprehensions** are faster than loops

## Next Steps

- [Classes and OOP](classes.md)
- [Decorators Deep Dive](decorators.md)
- [Type System](../types/hybrid-typing.md)
- [Performance Optimization](../advanced/performance.md)
