# Built-in Types

The following sections describe the standard types that are built into the interpreter.

## Truth Value Testing

Any object can be tested for truth value, for use in an if or while condition or as operand of the Boolean operations below.

By default, an object is considered true unless its class defines either a `__bool__()` method that returns `False` or a `__len__()` method that returns zero.

Here are most of the built-in objects considered false:

- constants defined to be false: `None` and `False`
- zero of any numeric type: `0`, `0.0`, `0j`, `Decimal(0)`, `Fraction(0, 1)`
- empty sequences and collections: `''`, `()`, `[]`, `{}`, `set()`, `range(0)`

Operations and built-in functions that have a Boolean result always return `0` or `False` for false and `1` or `True` for true, unless otherwise stated.

## Boolean Operations

These are the Boolean operations, ordered by ascending priority:

| Operation | Result | Notes |
|-----------|--------|-------|
| `x or y` | if x is false, then y, else x | (1) |
| `x and y` | if x is false, then x, else y | (2) |
| `not x` | if x is false, then True, else False | (3) |

Notes:
1. This is a short-circuit operator, so it only evaluates the second argument if the first one is false.
2. This is a short-circuit operator, so it only evaluates the second argument if the first one is true.
3. `not` has a lower priority than non-Boolean operators, so `not a == b` is interpreted as `not (a == b)`, and `a == not b` is a syntax error.

## Comparisons

There are eight comparison operations in Python:

| Operation | Meaning |
|-----------|---------|
| `<` | strictly less than |
| `<=` | less than or equal |
| `>` | strictly greater than |
| `>=` | greater than or equal |
| `==` | equal |
| `!=` | not equal |
| `is` | object identity |
| `is not` | negated object identity |

Objects of different types always compare unequal, and are ordered consistently but arbitrarily.

## Numeric Types

### Integer Types

```tauraro
# Integer literals
x = 42
y = 0b101010    # Binary literal
z = 0o52        # Octal literal
w = 0x2A        # Hexadecimal literal

# Integer methods
>>> (42).bit_length()
6
>>> (42).to_bytes(2, byteorder='big')
b'\x00*'
>>> int.from_bytes(b'\x00*', byteorder='big')
42
```

### Floating Point Types

```tauraro
# Float literals
x = 3.14
y = 1.5e-3      # Scientific notation
z = 3.14j       # Imaginary number

# Float methods
>>> (3.14).as_integer_ratio()
(7070651414971679, 2251799813685248)
>>> (3.14).is_integer()
False
>>> (3.0).is_integer()
True
>>> float.fromhex('0x1.8p+0')
1.5
>>> (1.5).hex()
'0x1.8000000000000p+0'
```

### Complex Types

```tauraro
# Complex literals
z = 3 + 4j
w = complex(3, 4)

# Complex attributes
>>> z.real
3.0
>>> z.imag
4.0

# Complex methods
>>> z.conjugate()
(3-4j)
```

## Iterator Types

Python supports a concept of iteration over containers. This is implemented using two distinct methods:

- `__iter__()` - This method is called when an iterator is required for a container.
- `__next__()` - This method is called to get the next value from an iterator.

```tauraro
# Creating an iterator
>>> it = iter([1, 2, 3])
>>> next(it)
1
>>> next(it)
2
>>> next(it)
3
>>> next(it)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
StopIteration
```

## Sequence Types

### Common Sequence Operations

The operations in the following table are supported by most sequence types, both mutable and immutable.

| Operation | Result |
|-----------|--------|
| `x in s` | True if an item of s is equal to x, else False |
| `x not in s` | False if an item of s is equal to x, else True |
| `s + t` | the concatenation of s and t |
| `s * n` or `n * s` | equivalent to adding s to itself n times |
| `s[i]` | ith item of s, origin 0 |
| `s[i:j]` | slice of s from i to j |
| `s[i:j:k]` | slice of s from i to j with step k |
| `len(s)` | length of s |
| `min(s)` | smallest item of s |
| `max(s)` | largest item of s |
| `s.index(x[, i[, j]])` | index of the first occurrence of x in s |
| `s.count(x)` | total number of occurrences of x in s |

### Immutable Sequence Types

#### String Methods

Strings implement all of the common sequence operations, along with the additional methods:

```tauraro
# String creation
s = "Hello, World!"
s = 'Hello, World!'
s = """Multiline
string"""

# String methods
>>> "hello".capitalize()
'Hello'
>>> "hello".upper()
'HELLO'
>>> "HELLO".lower()
'hello'
>>> "  hello  ".strip()
'hello'
>>> "hello world".split()
['hello', 'world']
>>> "a,b,c".split(",")
['a', 'b', 'c']
>>> "-".join(["a", "b", "c"])
'a-b-c'
>>> "hello".replace("l", "L")
'heLLo'
>>> "hello".find("l")
2
>>> "hello".startswith("he")
True
>>> "hello".endswith("lo")
True
>>> "42".isdigit()
True
>>> "hello".isalpha()
True
>>> "hello123".isalnum()
True
```

#### String Formatting

```tauraro
# Old-style formatting
>>> "Hello, %s!" % "World"
'Hello, World!'
>>> "Value: %d, Price: %.2f" % (42, 3.14)
'Value: 42, Price: 3.14'

# str.format() method
>>> "Hello, {}!".format("World")
'Hello, World!'
>>> "Hello, {name}!".format(name="World")
'Hello, World!'
>>> "Value: {0}, Price: {1:.2f}".format(42, 3.14)
'Value: 42, Price: 3.14'

# f-strings (formatted string literals)
>>> name = "World"
>>> value = 42
>>> f"Hello, {name}! Value: {value}"
'Hello, World! Value: 42'
>>> f"Price: {3.14159:.2f}"
'Price: 3.14'
```

#### Tuple Methods

Tuples are immutable sequences, typically used to store collections of heterogeneous data.

```tauraro
# Tuple creation
t = (1, 2, 3)
t = 1, 2, 3        # Parentheses are optional
t = (1,)           # Single element tuple (note the comma)
t = ()             # Empty tuple

# Tuple methods
>>> t = (1, 2, 3, 2, 4)
>>> t.count(2)
2
>>> t.index(3)
2
```

### Mutable Sequence Types

#### List Methods

Lists are mutable sequences, typically used to store collections of homogeneous items.

```tauraro
# List creation
l = [1, 2, 3]
l = list("hello")  # ['h', 'e', 'l', 'l', 'o']

# List methods
>>> l = [1, 2, 3]
>>> l.append(4)
>>> l
[1, 2, 3, 4]
>>> l.extend([5, 6])
>>> l
[1, 2, 3, 4, 5, 6]
>>> l.insert(0, 0)
>>> l
[0, 1, 2, 3, 4, 5, 6]
>>> l.remove(3)
>>> l
[0, 1, 2, 4, 5, 6]
>>> l.pop()
6
>>> l.pop(0)
0
>>> l
[1, 2, 4, 5]
>>> l.clear()
>>> l
[]
>>> l = [1, 3, 2, 3]
>>> l.index(3)
1
>>> l.count(3)
2
>>> l.sort()
>>> l
[1, 2, 3, 3]
>>> l.reverse()
>>> l
[3, 3, 2, 1]
>>> l.copy()
[3, 3, 2, 1]
```

#### List Comprehensions

List comprehensions provide a concise way to create lists.

```tauraro
# Basic list comprehension
>>> squares = [x**2 for x in range(10)]
>>> squares
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

# List comprehension with condition
>>> evens = [x for x in range(10) if x % 2 == 0]
>>> evens
[0, 2, 4, 6, 8]

# Nested list comprehension
>>> matrix = [[i*j for j in range(3)] for i in range(3)]
>>> matrix
[[0, 0, 0], [0, 1, 2], [0, 2, 4]]
```

### Range Type

The range type represents an immutable sequence of numbers and is commonly used for looping a specific number of times in for loops.

```tauraro
# Range creation
r = range(10)        # 0 to 9
r = range(1, 10)     # 1 to 9
r = range(0, 10, 2)  # 0, 2, 4, 6, 8

# Range methods
>>> r = range(10)
>>> r.start
0
>>> r.stop
10
>>> r.step
1
>>> 5 in r
True
>>> list(r)
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
>>> len(r)
10
```

## Text Sequence Type

### String Methods

Strings support a wide variety of methods for text processing:

```tauraro
# Case conversion
>>> "Hello".upper()
'HELLO'
>>> "Hello".lower()
'hello'
>>> "hello".capitalize()
'Hello'
>>> "hello world".title()
'Hello World'
>>> "Hello".swapcase()
'hELLO'

# Padding and alignment
>>> "42".zfill(5)
'00042'
>>> "hello".ljust(10, '-')
'hello-----'
>>> "hello".rjust(10, '-')
'-----hello'
>>> "hello".center(10, '-')
'--hello---'

# Testing methods
>>> "hello".islower()
True
>>> "HELLO".isupper()
True
>>> "Hello".istitle()
True
>>> "123".isdigit()
True
>>> "123".isnumeric()
True
>>> "hello".isalpha()
True
>>> "hello123".isalnum()
True
>>> "   ".isspace()
True

# Search and replace
>>> "hello world".find("world")
6
>>> "hello world".rfind("l")
9
>>> "hello world".index("world")
6
>>> "hello world".count("l")
3
>>> "hello world".replace("world", "universe")
'hello universe'

# Splitting and joining
>>> "a,b,c".split(",")
['a', 'b', 'c']
>>> "a b c".split()
['a', 'b', 'c']
>>> "a\nb\nc".splitlines()
['a', 'b', 'c']
>>> "-".join(["a", "b", "c"])
'a-b-c'

# Stripping whitespace
>>> "  hello  ".strip()
'hello'
>>> "  hello".lstrip()
'hello'
>>> "hello  ".rstrip()
'hello'
```

### String Formatting

```tauraro
# Format specification
>>> "{:>10}".format("hello")      # Right-aligned
'     hello'
>>> "{:<10}".format("hello")      # Left-aligned
'hello     '
>>> "{:^10}".format("hello")      # Centered
'  hello   '
>>> "{:*^10}".format("hello")     # Centered with fill
'**hello***'

# Number formatting
>>> "{:+d}".format(42)            # Show sign
'+42'
>>> "{: d}".format(42)            # Space for positive
' 42'
>>> "{:,}".format(1234567)        # Comma separator
'1,234,567'
>>> "{:.2f}".format(3.14159)      # Float precision
'3.14'
>>> "{:0>5d}".format(42)          # Zero-padded
'00042'
```

## Binary Sequence Types

### Bytes Methods

Bytes objects are immutable sequences of single bytes.

```tauraro
# Bytes creation
b = b"hello"
b = bytes([104, 101, 108, 108, 111])
b = "hello".encode("utf-8")

# Bytes methods
>>> b = b"hello"
>>> b.upper()
b'HELLO'
>>> b.split(b"e")
[b'h', b'llo']
>>> b.startswith(b"he")
True
>>> b.endswith(b"lo")
True
>>> b.find(b"ll")
2
>>> len(b)
5
```

### Bytearray Methods

Bytearray objects are a mutable counterpart to bytes objects.

```tauraro
# Bytearray creation
ba = bytearray(b"hello")
ba = bytearray([104, 101, 108, 108, 111])
ba = bytearray(5)  # Creates bytearray of 5 zero bytes

# Bytearray methods (similar to bytes but mutable)
>>> ba = bytearray(b"hello")
>>> ba[0] = ord(b"H")
>>> ba
bytearray(b'Hello')
>>> ba.append(33)  # Append exclamation mark
>>> ba
bytearray(b'Hello!')
```

## Set Types

### Set Methods

Sets are unordered collections of unique elements.

```tauraro
# Set creation
s = {1, 2, 3}
s = set([1, 2, 3, 2, 1])  # {1, 2, 3}
s = set("hello")          # {'h', 'e', 'l', 'o'}

# Set methods
>>> s = {1, 2, 3}
>>> s.add(4)
>>> s
{1, 2, 3, 4}
>>> s.remove(2)
>>> s
{1, 3, 4}
>>> s.discard(5)  # Doesn't raise error if not present
>>> s.pop()       # Remove and return arbitrary element
1
>>> s.clear()
>>> s
set()

# Set operations
>>> s1 = {1, 2, 3}
>>> s2 = {3, 4, 5}
>>> s1 | s2       # Union
{1, 2, 3, 4, 5}
>>> s1 & s2       # Intersection
{3}
>>> s1 - s2       # Difference
{1, 2}
>>> s1 ^ s2       # Symmetric difference
{1, 2, 4, 5}
>>> s1.issubset(s2)
False
>>> s1.issuperset({1, 2})
True
>>> s1.isdisjoint({4, 5})
False
```

### Frozenset Methods

Frozensets are immutable sets.

```tauraro
# Frozenset creation
fs = frozenset([1, 2, 3, 2, 1])  # frozenset({1, 2, 3})

# Frozenset methods (same as set but immutable)
>>> fs = frozenset([1, 2, 3])
>>> fs.copy()
frozenset({1, 2, 3})
>>> len(fs)
3
>>> 2 in fs
True
```

## Mapping Types

### Dictionary Methods

Dictionaries are mutable mappings from keys to values.

```tauraro
# Dictionary creation
d = {"a": 1, "b": 2}
d = dict(a=1, b=2)
d = dict([("a", 1), ("b", 2)])

# Dictionary methods
>>> d = {"a": 1, "b": 2}
>>> d["c"] = 3
>>> d
{'a': 1, 'b': 2, 'c': 3}
>>> d.get("a")
1
>>> d.get("d", 0)  # Default value
0
>>> d.keys()
dict_keys(['a', 'b', 'c'])
>>> d.values()
dict_values([1, 2, 3])
>>> d.items()
dict_items([('a', 1), ('b', 2), ('c', 3)])
>>> d.pop("b")
2
>>> d
{'a': 1, 'c': 3}
>>> d.popitem()    # Remove and return arbitrary item
('c', 3)
>>> d
{'a': 1}
>>> d.update({"d": 4, "e": 5})
>>> d
{'a': 1, 'd': 4, 'e': 5}
>>> d.clear()
>>> d
{}
>>> d = {"a": 1, "b": 2}
>>> d.setdefault("c", 3)
3
>>> d
{'a': 1, 'b': 2, 'c': 3}
```

### Dictionary Comprehensions

Dictionary comprehensions provide a concise way to create dictionaries.

```tauraro
# Basic dictionary comprehension
>>> squares = {x: x**2 for x in range(5)}
>>> squares
{0: 0, 1: 1, 2: 4, 3: 9, 4: 16}

# Dictionary comprehension with condition
>>> evens = {x: x**2 for x in range(10) if x % 2 == 0}
>>> evens
{0: 0, 2: 4, 4: 16, 6: 36, 8: 64}

# Dictionary comprehension from two sequences
>>> keys = ["a", "b", "c"]
>>> values = [1, 2, 3]
>>> d = {k: v for k, v in zip(keys, values)}
>>> d
{'a': 1, 'b': 2, 'c': 3}
```

## Context Manager Types

Context managers are objects that define the runtime context to be established when executing a with statement.

```tauraro
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

# Using context manager
with MyContextManager() as cm:
    print("Inside context")
    # raise ValueError("Test exception")
```

## Generic Alias Types

Generic alias types are used for type hints with generic types.

```tauraro
from typing import List, Dict, Tuple, Set

# Type aliases
Vector = List[float]
ConnectionOptions = Dict[str, str]
Address = Tuple[str, int]
Message = Tuple[str, str, str]

# Generic types
def process_items(items: List[str]) -> Dict[str, int]:
    return {item: len(item) for item in items}

def find_user(user_id: int) -> Tuple[str, int]:
    return ("Alice", 30)
```

## Other Built-in Types

### Module Objects

Modules are created by import statements and can be created by calling the built-in function `__import__()`.

```tauraro
import math
import sys

# Module attributes
>>> math.__name__
'math'
>>> math.__doc__
'This module provides access to the mathematical functions...'
>>> dir(math)
['__doc__', '__loader__', '__name__', '__package__', '__spec__', 'acos', ...]
```

### Class Objects

Class objects support two kinds of operations: attribute references and instantiation.

```tauraro
class MyClass:
    """A simple example class"""
    i = 12345
    
    def f(self):
        return 'hello world'

# Class attributes
>>> MyClass.i
12345
>>> MyClass.f
<function MyClass.f at 0x...>
>>> MyClass.__doc__
'A simple example class'

# Class instantiation
>>> x = MyClass()
>>> x.i
12345
```

### Instance Objects

Instance objects are created by calling a class object.

```tauraro
class Complex:
    def __init__(self, realpart, imagpart):
        self.r = realpart
        self.i = imagpart

>>> x = Complex(3.0, -4.5)
>>> x.r, x.i
(3.0, -4.5)
```

### Function Objects

Function objects are created by function definitions.

```tauraro
def my_function():
    """Do nothing, but document it."""
    pass

# Function attributes
>>> my_function.__name__
'my_function'
>>> my_function.__doc__
'Do nothing, but document it.'
```

### Method Objects

Method objects are created when a class attribute is a function object.

```tauraro
class MyClass:
    def f(self):
        return 'hello world'

>>> x = MyClass()
>>> x.f()  # Method call
'hello world'
>>> xf = x.f
>>> xf()   # Method object call
'hello world'
```

### Code Objects

Code objects represent byte-compiled executable Python code.

```tauraro
def example():
    return 42

>>> example.__code__
<code object example at 0x..., file "<stdin>", line 1>
>>> example.__code__.co_name
'example'
>>> example.__code__.co_varnames
()
```

### Type Objects

Type objects represent the types of objects.

```tauraro
>>> type(42)
<class 'int'>
>>> type("hello")
<class 'str'>
>>> type(type(42))
<class 'type'>
```

### The Null Object

The null object is the singleton object `None`.

```tauraro
>>> None
None
>>> type(None)
<class 'NoneType'>
>>> None is None
True
```

### The Ellipsis Object

The ellipsis object is the singleton object `...`.

```tauraro
>>> ...
Ellipsis
>>> type(...)
<class 'ellipsis'>
>>> ... is ...
True
```

### The NotImplemented Object

The NotImplemented object is the singleton object `NotImplemented`.

```tauraro
>>> NotImplemented
NotImplemented
>>> type(NotImplemented)
<class 'NotImplementedType'>
>>> NotImplemented is NotImplemented
True
```

## Special Attributes

The implementation adds a few special read-only attributes to several object types, where they are relevant.

```tauraro
# Object attributes
>>> obj = [1, 2, 3]
>>> obj.__class__
<class 'list'>
>>> obj.__doc__  # Only for some objects
>>> len.__name__
'len'
>>> len.__qualname__
'len'
>>> len.__module__
'builtins'
>>> len.__annotations__
{}
>>> len.__dict__  # Only for some objects
```

## Integer String Conversion Length Limitation

Python has a limit on the length of strings that can be converted to integers to prevent denial of service attacks.

```tauraro
# This will raise ValueError for very long strings
>>> int("1" * 1000000)
# ValueError: Exceeds the limit (4300) for integer string conversion
```

## Built-in Constants

The built-in constants are:

```tauraro
# Boolean constants
True
False

# Null constant
None

# Special constants
NotImplemented
Ellipsis  # or ...
```

## Built-in Exceptions

Built-in exceptions are described in the [Exceptions](exceptions.md) section.

## Built-in Functions

Built-in functions are described in the [Built-in Functions](functions.md) section.