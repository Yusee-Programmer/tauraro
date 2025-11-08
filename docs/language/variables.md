# Variables and Constants

Variables in Tauraro work just like Python - no declaration needed, dynamic typing, with optional static type annotations for performance.

## Variable Assignment

### Basic Assignment

```python
# Simple assignment
x = 10
name = "Alice"
is_valid = True

# Multiple assignment
a, b, c = 1, 2, 3

# Swap variables
x, y = y, x
```

### Augmented Assignment

```python
x = 10
x += 5   # x = 15
x -= 3   # x = 12
x *= 2   # x = 24
x /= 4   # x = 6.0
x //= 2  # x = 3.0
x %= 2   # x = 1.0
x **= 3  # x = 1.0
```

### Unpacking

```python
# List unpacking
first, *rest = [1, 2, 3, 4, 5]
# first = 1, rest = [2, 3, 4, 5]

# Tuple unpacking
x, y = (10, 20)

# Dict unpacking (gets keys)
keys = [*{"a": 1, "b": 2}]
# keys = ["a", "b"]
```

## Type Annotations

### Basic Type Hints

```python
# With type annotations
age: int = 25
height: float = 1.75
name: str = "Alice"
is_active: bool = True

# Multiple variables with types
x: int
y: int
x, y = 10, 20
```

### Why Use Type Annotations?

1. **Performance**: Compiled code is **50-100x faster** with types
2. **Clarity**: Makes code easier to understand
3. **Safety**: Catches type errors early
4. **Optimization**: Enables native C types in compilation

**Example:**

```python
# Without types - uses boxed values
def add(a, b):
    return a + b

# With types - uses native int64_t in C compilation
def add_typed(a: int, b: int) -> int:
    return a + b
```

### Collection Type Hints

```python
from typing import List, Dict, Tuple, Set, Optional

# List of integers
numbers: List[int] = [1, 2, 3, 4, 5]

# Dictionary with string keys and int values
scores: Dict[str, int] = {"Alice": 95, "Bob": 87}

# Tuple with specific types
point: Tuple[float, float] = (3.14, 2.71)

# Set of strings
tags: Set[str] = {"python", "rust", "performance"}

# Optional value (can be None)
middle_name: Optional[str] = None
```

### Function Type Hints

```python
def greet(name: str, age: int) -> str:
    return f"Hello {name}, you are {age} years old"

def calculate_area(width: float, height: float) -> float:
    return width * height

def process_items(items: List[int]) -> List[int]:
    return [x * 2 for x in items]
```

## Variable Scope

### Global Scope

```python
# Global variable
counter = 0

def increment():
    global counter
    counter += 1

increment()
print(counter)  # 1
```

### Local Scope

```python
def my_function():
    x = 10  # Local to this function
    return x

# x is not accessible here
```

### Nonlocal Scope

```python
def outer():
    count = 0

    def inner():
        nonlocal count
        count += 1
        return count

    return inner

counter = outer()
print(counter())  # 1
print(counter())  # 2
```

### Enclosing Scope (Closures)

```python
def make_multiplier(factor):
    def multiply(x):
        return x * factor  # Accesses 'factor' from enclosing scope
    return multiply

times_3 = make_multiplier(3)
print(times_3(10))  # 30
```

## Constants

Python doesn't have true constants, but by convention, we use UPPERCASE names:

```python
# Constants (by convention)
PI = 3.14159
MAX_SIZE = 100
API_URL = "https://api.example.com"

# In Tauraro, when compiled with types, these can be optimized
PI: float = 3.14159  # Compiled as const double
MAX_SIZE: int = 100  # Compiled as const int64_t
```

## Variable Naming Rules

### Valid Names

```python
# Allowed characters: letters, digits, underscore
name = "Alice"
user_name = "Bob"
item_1 = "First"
_private = "hidden"
__dunder__ = "special"

# Can start with underscore
_internal = 42
__private = 100
```

### Invalid Names

```python
# Cannot start with digit
# 1variable = 10  # SyntaxError

# Cannot use reserved keywords
# class = "MyClass"  # SyntaxError
# if = 10  # SyntaxError
# def = 20  # SyntaxError

# Cannot contain special characters
# user-name = "Bob"  # SyntaxError
# total$ = 100  # SyntaxError
```

### Naming Conventions

```python
# Variables and functions: snake_case
user_count = 10
total_amount = 100.50

def calculate_total():
    pass

# Classes: PascalCase
class UserAccount:
    pass

# Constants: UPPER_SNAKE_CASE
MAX_RETRY_COUNT = 3
DEFAULT_TIMEOUT = 30

# Private/internal: leading underscore
_internal_value = 42
__private_value = 100
```

## Dynamic vs Static Typing

### Dynamic Typing (Default)

```python
# Variable type can change
x = 10        # int
x = "hello"   # now str
x = [1, 2, 3] # now list
x = {"a": 1}  # now dict
```

### Static Typing (With Annotations)

```python
# Type is fixed
x: int = 10
# x = "hello"  # Would cause type error

# Better for performance
count: int = 0
for i in range(1000000):
    count += i  # Fast native addition when compiled
```

## Variable Lifetime and Memory

### Automatic Memory Management

```python
# Variables are automatically cleaned up
def process():
    data = [1, 2, 3, 4, 5]
    # 'data' is cleaned up when function exits
```

### Manual Memory Control

```python
# For advanced use cases, explicit memory management
@manual_memory
def performance_critical():
    buffer = allocate(1024)  # Manual allocation
    try:
        # Use buffer
        pass
    finally:
        free(buffer)  # Manual cleanup
```

### Arena Memory

```python
# Bulk allocation for temporary data
@arena_memory
def process_batch(items):
    results = []
    for item in items:
        # All temp allocations use arena
        processed = transform(item)
        results.append(processed)
    return results
    # Arena freed automatically
```

## Compilation Behavior

### VM Execution

```python
# Variables are dynamically typed
x = 10
y = "hello"
z = [1, 2, 3]
# All stored as tauraro_value_t
```

### C Compilation (No Types)

```python
# Variables use boxed values
x = 10  # tauraro_value_t* x = tauraro_make_int(10);
```

### C Compilation (With Types)

```python
# Variables use native C types
x: int = 10        # int64_t x = 10;
y: float = 3.14    # double y = 3.14;
s: str = "hello"   # char* s = "hello";
```

**Performance difference: Up to 100x faster with types!**

## Best Practices

### 1. Use Descriptive Names

```python
# Good
user_count = 10
total_price = 99.99

# Bad
x = 10
t = 99.99
```

### 2. Add Type Annotations for Performance

```python
# Good - fast when compiled
def calculate_sum(numbers: List[int]) -> int:
    total: int = 0
    for n in numbers:
        total += n
    return total

# Slower when compiled
def calculate_sum(numbers):
    total = 0
    for n in numbers:
        total += n
    return total
```

### 3. Use Constants for Magic Numbers

```python
# Good
MAX_RETRY_COUNT: int = 3
for attempt in range(MAX_RETRY_COUNT):
    try_operation()

# Bad
for attempt in range(3):  # What does 3 mean?
    try_operation()
```

### 4. Minimize Global Variables

```python
# Good - use function parameters
def calculate(value: int, multiplier: int) -> int:
    return value * multiplier

# Less ideal - global state
multiplier = 2
def calculate(value: int) -> int:
    return value * multiplier
```

### 5. Use Appropriate Scope

```python
# Good - narrow scope
def process():
    temp_data = load_data()
    result = transform(temp_data)
    return result

# Less ideal - wider scope than necessary
temp_data = None
result = None
def process():
    global temp_data, result
    temp_data = load_data()
    result = transform(temp_data)
```

## Common Patterns

### Swapping Variables

```python
# Pythonic way
a, b = b, a

# Traditional (works but not needed)
temp = a
a = b
b = temp
```

### Default Values

```python
# Using or operator
name = input_name or "Unknown"

# Using conditional
value = x if x is not None else default_value
```

### Chained Assignment

```python
# All variables get the same value
x = y = z = 0

# Lists are independent
a, b, c = [], [], []

# Watch out! This creates shared references
# bad = [[]] * 3  # All three elements share the same list
```

### Walrus Operator (Assignment Expressions)

```python
# Assign and use in same expression
if (n := len(data)) > 10:
    print(f"Processing {n} items")

# In list comprehensions
filtered = [y for x in data if (y := process(x)) is not None]
```

## Type System Integration

### Hybrid Typing

Tauraro supports **hybrid typing** - you can mix typed and untyped code:

```python
# Untyped - dynamic
def parse(data):
    return process(data)

# Typed - optimized
def calculate(x: int, y: int) -> int:
    return x + y

# Mixed
def main():
    data = parse(input())  # Dynamic
    result = calculate(10, 20)  # Optimized
    return result
```

### Gradual Typing

Start untyped, add types where needed:

```python
# Phase 1: Prototype (no types)
def process(data):
    return transform(data)

# Phase 2: Add types to hot paths
def process(data: List[int]) -> List[int]:
    result: List[int] = []
    for item in data:
        result.append(transform(item))
    return result

# Phase 3: Fully typed for maximum performance
def process(data: List[int]) -> List[int]:
    result: List[int] = []
    for item in data:
        transformed: int = transform(item)
        result.append(transformed)
    return result
```

## Next Steps

- [Data Types](data-types.md) - All available types
- [Operators](operators.md) - Operations on variables
- [Functions](functions.md) - Function parameters and returns
- [Type System](../types/hybrid-typing.md) - Advanced typing
- [Performance](../advanced/performance.md) - Optimization tips
