# REPL Features

The Tauraro REPL (Read-Eval-Print-Loop) provides an interactive environment for exploring the language, testing code, and debugging.

## Starting the REPL

```bash
tauraro repl
```

You'll see:
```
Tauraro 1.0.0 (main, Jan 2025)
[Rust-based VM] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

## Auto-Printing Expressions

Unlike Python, Tauraro automatically prints expression values **without** needing to wrap them in `print()`:

```python
>>> x = 5
>>> x
5

>>> 2 + 3
5

>>> "Hello" + " " + "World"
'Hello World'

>>> [1, 2, 3]
[1, 2, 3]
```

### Comparison with Python

**Python:**
```python
>>> x = 5
>>> x        # Must explicitly check
5
>>> 2 + 3
5
```

**Tauraro:**
```python
>>> x = 5
>>> x        # Automatically prints
5
>>> 2 + 3    # All expressions auto-print
5
```

## Built-in Introspection Functions

### `help()` - Get Help

Get help on any object:

```python
>>> help()
Welcome to Tauraro!

Tauraro is a Python-compatible programming language with Rust-like performance.

Type help() for interactive help, or help(object) for help about object.

>>> def greet(name):
...     """Greet someone by name"""
...     return f"Hello, {name}!"

>>> help(greet)
Help on function greet:

greet(name)

Greet someone by name
```

Help works with:
- Functions
- Classes
- Modules
- Built-in objects
- String names

```python
>>> help("print")
Help on built-in function print:

print(...)
    Built-in function
```

### `dir()` - List Names

List all names in current scope:

```python
>>> x = 10
>>> y = 20
>>> def foo():
...     pass

>>> dir()
['x', 'y', 'foo', 'print', 'len', 'range', ...]
```

List attributes of an object:

```python
>>> class Person:
...     def __init__(self):
...         self.name = "Alice"

>>> p = Person()
>>> dir(p)
['__init__', 'name', ...]
```

### `globals()` - View Global Variables

See all global variables as a dictionary:

```python
>>> x = 10
>>> y = 20
>>> globals()
{'x': 10, 'y': 20, 'print': <builtin>, ...}

>>> 'x' in globals()
True
```

### `locals()` - View Local Variables

Inside a function, see local variables:

```python
>>> def test():
...     a = 1
...     b = 2
...     print(locals())

>>> test()
{'a': 1, 'b': 2}
```

At module level, `locals()` returns `globals()`:

```python
>>> locals() == globals()
True
```

## Type Enforcement in REPL

Static typing is enforced in the REPL too:

```python
>>> x: int = 10
>>> x
10

>>> x = 20
>>> x
20

>>> x = "error"
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: Cannot assign value of type 'str' to variable 'x' of type 'int'
```

## Multi-line Input

Enter multi-line code with continuation prompts:

```python
>>> def factorial(n):
...     if n <= 1:
...         return 1
...     return n * factorial(n - 1)
...
>>> factorial(5)
120
```

The REPL automatically detects:
- Function definitions
- Class definitions
- Control flow blocks
- Multi-line expressions

## Previous Result: `_`

Access the last expression result with `_`:

```python
>>> 2 + 3
5

>>> _ * 2
10

>>> _ + 5
15
```

## Import Statements

Import modules in the REPL:

```python
>>> import math
>>> math.sqrt(16)
4.0

>>> from os import getcwd
>>> getcwd()
'/home/user/tauraro'
```

## Error Messages

Get detailed error messages with tracebacks:

```python
>>> def divide(a, b):
...     return a / b

>>> divide(10, 0)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "<stdin>", line 2, in divide
ZeroDivisionError: division by zero
```

## Variable Inspection

Check types and values easily:

```python
>>> x = [1, 2, 3]
>>> type(x)
<class 'list'>

>>> len(x)
3

>>> isinstance(x, list)
True
```

## Code Execution History

The REPL remembers all definitions:

```python
>>> x = 10
>>> def double(n):
...     return n * 2

# Later in the session
>>> double(x)
20
```

## Working with Classes

Define and use classes interactively:

```python
>>> class Counter:
...     def __init__(self):
...         self.count = 0
...     def increment(self):
...         self.count += 1
...     def get(self):
...         return self.count

>>> c = Counter()
>>> c.increment()
>>> c.increment()
>>> c.get()
2
```

## String Formatting

Test f-strings and formatting:

```python
>>> name = "Alice"
>>> age = 30
>>> f"{name} is {age} years old"
'Alice is 30 years old'

>>> f"In 5 years, {name} will be {age + 5}"
'In 5 years, Alice will be 35'
```

## List Comprehensions

Try comprehensions interactively:

```python
>>> [x * 2 for x in range(5)]
[0, 2, 4, 6, 8]

>>> [x for x in range(10) if x % 2 == 0]
[0, 2, 4, 6, 8]

>>> {x: x**2 for x in range(5)}
{0: 0, 1: 1, 2: 4, 3: 9, 4: 16}
```

## Testing Code Snippets

Perfect for testing before adding to scripts:

```python
>>> # Test a function
>>> def is_palindrome(s):
...     return s == s[::-1]

>>> is_palindrome("radar")
True

>>> is_palindrome("hello")
False
```

## Debugging

Use the REPL for debugging:

```python
>>> def buggy_function(x):
...     result = x * 2
...     print(f"Debug: result = {result}")
...     return result + 10

>>> buggy_function(5)
Debug: result = 10
20
```

## Performance Testing

Test performance of different approaches:

```python
>>> import time

>>> def test_loop():
...     start = time.time()
...     result = sum(range(1000000))
...     end = time.time()
...     print(f"Time: {end - start:.4f}s")
...     return result

>>> test_loop()
Time: 0.0234s
499999500000
```

## Tips and Tricks

### 1. Quick Calculations

```python
>>> 123 * 456
56088

>>> 2 ** 10
1024

>>> round(3.14159, 2)
3.14
```

### 2. String Operations

```python
>>> "hello".upper()
'HELLO'

>>> "   spaces   ".strip()
'spaces'

>>> "a,b,c".split(",")
['a', 'b', 'c']
```

### 3. List Operations

```python
>>> numbers = [1, 2, 3]
>>> numbers.append(4)
>>> numbers
[1, 2, 3, 4]

>>> numbers.extend([5, 6])
>>> numbers
[1, 2, 3, 4, 5, 6]
```

### 4. Dictionary Operations

```python
>>> person = {"name": "Alice", "age": 30}
>>> person["city"] = "NYC"
>>> person
{'name': 'Alice', 'age': 30, 'city': 'NYC'}

>>> person.keys()
dict_keys(['name', 'age', 'city'])
```

## Exiting the REPL

```python
>>> exit()
# or
>>> quit()
# or press Ctrl+D (Unix) / Ctrl+Z (Windows)
```

## REPL vs Script Execution

| Feature | REPL | Script |
|---------|------|--------|
| Auto-print expressions | Yes | No |
| Interactive | Yes | No |
| Persistent state | Yes (session) | No |
| Good for | Testing, learning | Production |
| Performance | Interpreted | Compiled available |

## Advanced REPL Usage

### Custom REPL Environment

```python
# Save in ~/.taurarorc
import math
import sys

# Auto-import commonly used functions
from os import getcwd, listdir
from datetime import datetime

# Helper functions
def timestamp():
    return datetime.now().isoformat()
```

### REPL as Calculator

```python
>>> # Financial calculations
>>> principal = 10000
>>> rate = 0.05
>>> years = 10
>>> principal * (1 + rate) ** years
16288.946267774416

>>> # Convert units
>>> miles = 100
>>> miles * 1.60934  # to kilometers
160.934
```

## Next Steps

- [Interactive Mode Guide](interactive.md)
- [Special Commands](commands.md)
- [Built-in Functions](../builtins/core.md)
- [Debugging Techniques](../advanced/debugging.md)
