# Interactive Mode (REPL)

Tauraro includes an interactive Read-Eval-Print Loop (REPL) for quick experimentation and testing.

## Starting the REPL

```bash
./target/release/tauraro repl
```

You'll see the Tauraro prompt:

```
Tauraro REPL v1.0
>>>
```

## Basic Usage

### Simple Expressions

```python
>>> 2 + 2
4

>>> 10 * 5
50

>>> "Hello" + " World"
'Hello World'
```

### Variables

```python
>>> x = 10
>>> y = 20
>>> x + y
30

>>> name = "Alice"
>>> f"Hello, {name}!"
'Hello, Alice!'
```

### Lists and Collections

```python
>>> numbers = [1, 2, 3, 4, 5]
>>> numbers
[1, 2, 3, 4, 5]

>>> sum(numbers)
15

>>> [x ** 2 for x in numbers]
[1, 4, 9, 16, 25]
```

## Importing Modules

```python
>>> import math
>>> math.sqrt(144)
12.0

>>> math.pi
3.141592653589793

>>> import httpx
>>> response = httpx.get("https://api.github.com")
>>> response.status_code
200
```

## Defining Functions

```python
>>> def greet(name):
...     return f"Hello, {name}!"
...
>>> greet("World")
'Hello, World!'

>>> def fibonacci(n):
...     if n <= 1:
...         return n
...     return fibonacci(n-1) + fibonacci(n-2)
...
>>> fibonacci(10)
55
```

## Multi-line Input

```python
>>> def calculate_sum(numbers):
...     total = 0
...     for n in numbers:
...         total += n
...     return total
...
>>> calculate_sum([1, 2, 3, 4, 5])
15
```

## Classes

```python
>>> class Person:
...     def __init__(self, name):
...         self.name = name
...     def greet(self):
...         return f"Hello, I'm {self.name}"
...
>>> alice = Person("Alice")
>>> alice.greet()
"Hello, I'm Alice"
```

## Testing Code Snippets

The REPL is perfect for testing small code snippets:

```python
>>> # Test list comprehension
>>> squares = [x**2 for x in range(10)]
>>> squares
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

>>> # Test string methods
>>> text = "hello world"
>>> text.upper()
'HELLO WORLD'

>>> # Test dictionary operations
>>> scores = {"Alice": 95, "Bob": 87}
>>> scores["Alice"]
95
```

## Special Variables

```python
>>> _ # Last result
>>> __ # Second-to-last result
```

## Exit the REPL

```python
>>> exit()
# or Ctrl+D
```

## Tips

1. **Use tab completion** (if available) to autocomplete
2. **Use `help(object)`** to get documentation
3. **Test code before adding to files**
4. **Import modules to explore them**
5. **Use `dir(object)`** to see available attributes

## Next Steps

- [REPL Features](features.md) - Advanced REPL features
- [Special Commands](commands.md) - REPL commands
- [Quick Start](../getting-started/quick-start.md) - Getting started guide
