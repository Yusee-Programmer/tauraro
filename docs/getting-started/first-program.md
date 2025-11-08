# Your First Tauraro Program

Welcome to Tauraro! This guide will walk you through creating and running your first Tauraro program.

## Hello, World!

Let's start with the classic "Hello, World!" program.

### Create the Program

Create a new file called `hello.py`:

```python
# hello.py
print("Hello, World!")
```

### Run with the VM

```bash
./target/release/tauraro run hello.py
```

Output:
```
Hello, World!
```

### Compile to Native

```bash
# Compile to native executable
./target/release/tauraro compile hello.py -o hello

# Run the native binary
./hello
```

Output:
```
Hello, World!
```

The compiled version runs at native C speed!

## Simple Calculator

Let's create a more interesting program - a simple calculator:

```python
# calculator.py

def add(a: int, b: int) -> int:
    return a + b

def multiply(a: int, b: int) -> int:
    return a * b

# Test the functions
x = 10
y = 5

print(f"{x} + {y} = {add(x, y)}")
print(f"{x} * {y} = {multiply(x, y)}")
```

Run it:

```bash
./target/release/tauraro run calculator.py
```

Output:
```
10 + 5 = 15
10 * 5 = 50
```

### Why Type Annotations?

Notice the type annotations (`int`, `-> int`)? They're optional but provide:
- **Better performance** when compiled to C
- **Clearer code** - easier to understand
- **Type safety** - catch errors early

Compare the performance:

```bash
# With types (faster)
./target/release/tauraro compile calculator.py -o calc_typed
time ./calc_typed

# Without types (still fast, but slower)
# Remove type annotations and recompile
time ./calc_untyped
```

## Working with Lists

```python
# lists.py

# Create a list
numbers = [1, 2, 3, 4, 5]

# List comprehension
squares = [x ** 2 for x in numbers]

print(f"Numbers: {numbers}")
print(f"Squares: {squares}")

# Filter even numbers
evens = [x for x in numbers if x % 2 == 0]
print(f"Even numbers: {evens}")

# Sum all numbers
total = sum(numbers)
print(f"Sum: {total}")
```

Run it:

```bash
./target/release/tauraro run lists.py
```

Output:
```
Numbers: [1, 2, 3, 4, 5]
Squares: [1, 4, 9, 16, 25]
Even numbers: [2, 4]
Sum: 15
```

## Functions and Recursion

```python
# fibonacci.py

def fibonacci(n: int) -> int:
    """Calculate the nth Fibonacci number."""
    if n <= 1:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

# Calculate first 10 Fibonacci numbers
for i in range(10):
    print(f"fibonacci({i}) = {fibonacci(i)}")
```

Run it:

```bash
./target/release/tauraro run fibonacci.py
```

### Performance Comparison

Compare Tauraro's speed with Python:

```bash
# Python (slow)
time python3 fibonacci.py

# Tauraro VM (faster)
time ./target/release/tauraro run fibonacci.py

# Tauraro compiled (very fast!)
./target/release/tauraro compile fibonacci.py -o fib
time ./fib
```

You'll see Tauraro is **significantly faster**, especially when compiled!

## Object-Oriented Programming

```python
# person.py

class Person:
    def __init__(self, name: str, age: int):
        self.name = name
        self.age = age

    def greet(self) -> str:
        return f"Hello, I'm {self.name} and I'm {self.age} years old."

    def birthday(self) -> None:
        self.age += 1
        print(f"Happy birthday! Now {self.age} years old.")

# Create and use objects
alice = Person("Alice", 30)
bob = Person("Bob", 25)

print(alice.greet())
print(bob.greet())

alice.birthday()
```

Run it:

```bash
./target/release/tauraro run person.py
```

Output:
```
Hello, I'm Alice and I'm 30 years old.
Hello, I'm Bob and I'm 25 years old.
Happy birthday! Now 31 years old.
```

## Using Built-in Modules

Tauraro comes with a rich standard library. Here's an example using the `math` module:

```python
# math_demo.py
import math

# Constants
print(f"Pi = {math.pi}")
print(f"E = {math.e}")

# Functions
print(f"sqrt(16) = {math.sqrt(16)}")
print(f"sin(π/2) = {math.sin(math.pi / 2)}")
print(f"log(10) = {math.log(10)}")

# Rounding
print(f"ceil(4.3) = {math.ceil(4.3)}")
print(f"floor(4.8) = {math.floor(4.8)}")
```

Run it:

```bash
./target/release/tauraro run math_demo.py
```

## HTTP Requests

Tauraro has built-in HTTP support with the `httpx` module:

```python
# http_demo.py
import httpx

# Simple GET request
response = httpx.get("https://api.github.com")
print(f"Status: {response.status_code}")

# The response is JSON
data = response.json()
print(f"GitHub API version: {data.get('current_user_url', 'N/A')}")
```

Run it:

```bash
./target/release/tauraro run http_demo.py
```

## Interactive Mode (REPL)

Tauraro includes an interactive REPL for experimentation:

```bash
./target/release/tauraro repl
```

Try these commands:

```python
>>> 2 + 2
4

>>> x = [1, 2, 3, 4, 5]
>>> sum(x)
15

>>> import math
>>> math.sqrt(144)
12.0

>>> def greet(name):
...     return f"Hello, {name}!"
...
>>> greet("World")
'Hello, World!'
```

## Next Steps

Now that you've created your first programs, explore more features:

- [Language Syntax](../language/syntax.md) - Complete syntax guide
- [Data Types](../language/data-types.md) - All available types
- [Standard Library](../stdlib/modules.md) - Built-in modules
- [Classes and OOP](../language/classes.md) - Object-oriented programming
- [Compilation Guide](../compilation/c-backend.md) - Compile to native code
- [Performance Tips](../advanced/performance.md) - Make your code faster

## Common Patterns

### Command-Line Arguments

```python
import sys

def main():
    if len(sys.argv) < 2:
        print("Usage: program <name>")
        return

    name = sys.argv[1]
    print(f"Hello, {name}!")

main()
```

Run with:
```bash
./target/release/tauraro run program.py Alice
```

### File I/O

```python
# Write to file
with open("output.txt", "w") as f:
    f.write("Hello from Tauraro!\n")

# Read from file
with open("output.txt", "r") as f:
    content = f.read()
    print(content)
```

### Error Handling

```python
def divide(a: float, b: float) -> float:
    if b == 0:
        raise ValueError("Cannot divide by zero")
    return a / b

try:
    result = divide(10, 0)
except ValueError as e:
    print(f"Error: {e}")
```

## Tips for Getting Started

1. **Start Simple** - Begin with basic programs and gradually add complexity
2. **Use Type Annotations** - They help with performance and code clarity
3. **Experiment in REPL** - Test ideas quickly in interactive mode
4. **Compile for Production** - Use VM for development, compile for deployment
5. **Read the Docs** - Explore the full documentation to learn all features

Happy coding with Tauraro!
