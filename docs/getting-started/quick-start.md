# Quick Start Tutorial

Get up and running with Tauraro in 5 minutes!

## Your First Tauraro Program

Create a file named `hello.py`:

```python
# hello.py
print("Hello, Tauraro!")
```

Run it:

```bash
tauraro run hello.py
# Output: Hello, Tauraro!
```

## Interactive REPL

Start the REPL for interactive coding:

```bash
tauraro repl
```

Try some expressions:

```python
>>> 2 + 2
4

>>> name = "Tauraro"
>>> f"Hello, {name}!"
'Hello, Tauraro!'

>>> x: int = 10  # Type annotation
>>> x
10

>>> x = "error"  # Type mismatch
TypeError: Cannot assign value of type 'str' to variable 'x' of type 'int'
```

## Basic Syntax

### Variables

```python
# Dynamic typing (like Python)
x = 10
x = "now a string"  # OK

# Static typing (optional)
age: int = 25
age = 30           # OK
age = "25"         # TypeError!
```

### Functions

```python
def greet(name: str) -> str:
    """Greet someone by name"""
    return f"Hello, {name}!"

result = greet("World")
print(result)  # Hello, World!
```

### Classes

```python
class Person:
    """A simple person class"""

    def __init__(self, name: str, age: int):
        self.name = name
        self.age = age

    def introduce(self):
        return f"I'm {self.name}, {self.age} years old"

person = Person("Alice", 30)
print(person.introduce())  # I'm Alice, 30 years old
```

### Control Flow

```python
# If statements
x = 10
if x > 5:
    print("x is greater than 5")
elif x == 5:
    print("x equals 5")
else:
    print("x is less than 5")

# For loops
for i in range(5):
    print(i)

# While loops
count = 0
while count < 5:
    print(count)
    count += 1
```

### Lists and Dictionaries

```python
# Lists
numbers = [1, 2, 3, 4, 5]
numbers.append(6)
print(numbers[0])  # 1

# Dictionaries
person = {
    "name": "Bob",
    "age": 25,
    "city": "NYC"
}
print(person["name"])  # Bob
```

## Execution Modes

### 1. VM Mode (Fast startup)

```bash
tauraro run script.py
```

Best for:
- Development
- Scripts
- Quick iterations

### 2. REPL Mode (Interactive)

```bash
tauraro repl
```

Best for:
- Learning
- Experimentation
- Debugging

### 3. Compile Mode (Maximum performance)

```bash
tauraro compile script.py -o program
./program
```

Best for:
- Production
- Performance-critical code
- Distribution

## Using Type Annotations for Performance

Type annotations make your code faster when compiled:

```python
# Without types (slower when compiled)
def add(a, b):
    return a + b

# With types (faster when compiled)
def add_fast(a: int, b: int) -> int:
    return a + b
```

When compiled to C, `add_fast` uses native `int64_t` arithmetic, while `add` uses boxed values.

## Importing Modules

```python
# Import built-in modules
import math
import sys
import os

# Use them
print(math.sqrt(16))  # 4.0
print(sys.platform)   # 'linux', 'darwin', or 'win32'
print(os.getcwd())    # Current directory
```

## Error Handling

```python
try:
    result = 10 / 0
except ZeroDivisionError as e:
    print(f"Error: {e}")
finally:
    print("Cleanup code here")
```

## REPL Special Features

The Tauraro REPL has Python-like features:

```python
>>> help()  # Interactive help
>>> dir()   # List available names
>>> globals()  # View global variables
>>> locals()   # View local variables

>>> def greet(name):
...     """Say hello"""
...     return f"Hello, {name}"
>>> help(greet)  # Show function documentation
```

## Next Steps

Now that you've learned the basics:

1. [Write more complex programs](first-program.md)
2. [Learn about the type system](../types/hybrid-typing.md)
3. [Explore built-in functions](../builtins/core.md)
4. [Study OOP features](../language/classes.md)
5. [Try compiling to C](../compilation/c-backend.md)

## Examples

Check out the `examples/` directory for more:

```bash
cd examples
tauraro run basic_oop_test.py
tauraro run comprehensive_oop_test.py
```

## Getting Help

- [Documentation](../README.md)
- [GitHub Issues](https://github.com/Yusee-Programmer/tauraro/issues)
- [Examples Directory](../examples/index.md)
