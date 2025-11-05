# Data Types

Tauraro supports all standard Python data types with full compatibility.

## Primitive Types

### Integers

Arbitrary precision integers like Python.

```python
x = 42
y = -17
z = 1_000_000  # Underscore separators
big = 12345678901234567890
```

**Operations:**
```python
a + b    # Addition
a - b    # Subtraction
a * b    # Multiplication
a / b    # Division (always returns float)
a // b   # Floor division
a % b    # Modulo
a ** b   # Power
```

### Floating Point

64-bit IEEE 754 floating point numbers.

```python
x = 3.14
y = -0.5
z = 1.5e-10   # Scientific notation
inf = float('inf')
nan = float('nan')
```

### Booleans

`True` and `False` are special integer values (1 and 0).

```python
x = True
y = False
z = bool(1)      # True
w = bool(0)      # False
```

**Truthy/Falsy Values:**
- False: `False`, `0`, `0.0`, `""`, `[]`, `{}`, `()`, `set()`, `None`
- True: Everything else

### None

Represents absence of value.

```python
x = None
if x is None:
    print("No value")
```

## String Types

### Strings

Immutable Unicode strings.

```python
s1 = "Hello"
s2 = 'World'
s3 = """Multi
line
string"""
s4 = "String with \"quotes\""
```

**String Operations:**
```python
s + t           # Concatenation
s * 3           # Repetition
s[i]            # Indexing
s[i:j]          # Slicing
len(s)          # Length
s in t          # Membership
```

**Common Methods:**
```python
s.upper()                # "HELLO"
s.lower()                # "hello"
s.strip()                # Remove whitespace
s.split()                # Split into list
s.replace(old, new)      # Replace substring
s.startswith(prefix)     # Check prefix
s.endswith(suffix)       # Check suffix
s.find(sub)              # Find substring
s.format(args)           # Format string
```

**String Formatting:**
```python
# f-strings (recommended)
name = "Alice"
age = 30
print(f"Hello {name}, you are {age}")
print(f"Math: {2 + 2}")
print(f"Formatted: {3.14159:.2f}")

# format() method
"Hello {}".format(name)
"x={0}, y={1}".format(10, 20)

# % formatting (old style)
"Hello %s" % name
"x=%d, y=%d" % (10, 20)
```

### Bytes

Immutable sequences of bytes.

```python
b = b"hello"
b = bytes([72, 101, 108, 108, 111])
b = "hello".encode('utf-8')

# Decode to string
s = b.decode('utf-8')
```

## Collection Types

### Lists

Mutable ordered sequences.

```python
# Creation
nums = [1, 2, 3, 4, 5]
mixed = [1, "two", 3.0, True]
nested = [[1, 2], [3, 4]]
empty = []

# Indexing and slicing
nums[0]        # 1 (first element)
nums[-1]       # 5 (last element)
nums[1:3]      # [2, 3]
nums[:2]       # [1, 2]
nums[2:]       # [3, 4, 5]
nums[::2]      # [1, 3, 5] (every 2nd)
nums[::-1]     # [5, 4, 3, 2, 1] (reversed)

# Modification
nums[0] = 10
nums.append(6)
nums.insert(0, 0)
nums.extend([7, 8])
nums.remove(3)
del nums[0]
x = nums.pop()
nums.clear()

# Methods
len(nums)
nums.sort()
nums.reverse()
nums.count(2)
nums.index(3)
```

### Tuples

Immutable ordered sequences.

```python
# Creation
t = (1, 2, 3)
t = 1, 2, 3       # Parentheses optional
single = (1,)     # Single element (note comma)
empty = ()

# Unpacking
x, y, z = (1, 2, 3)
a, *rest, b = (1, 2, 3, 4, 5)  # a=1, rest=[2,3,4], b=5

# Operations
t[0]              # Indexing
t[1:3]            # Slicing
len(t)            # Length
t + (4, 5)        # Concatenation
t * 2             # Repetition
2 in t            # Membership
```

### Dictionaries

Mutable key-value mappings.

```python
# Creation
d = {"name": "Alice", "age": 30}
d = dict(name="Alice", age=30)
empty = {}

# Access
d["name"]              # "Alice"
d.get("age")           # 30
d.get("missing", 0)    # 0 (default)

# Modification
d["age"] = 31
d["city"] = "NYC"
del d["age"]
d.clear()

# Methods
d.keys()               # dict_keys(['name', 'age'])
d.values()             # dict_values(['Alice', 30])
d.items()              # dict_items([('name', 'Alice'), ('age', 30)])
d.update({"age": 32})
d.pop("name")
d.setdefault("age", 0)

# Iteration
for key in d:
    print(key, d[key])

for key, value in d.items():
    print(key, value)
```

### Sets

Mutable unordered collections of unique elements.

```python
# Creation
s = {1, 2, 3, 4}
s = set([1, 2, 2, 3])  # {1, 2, 3}
empty = set()          # Note: {} creates dict

# Modification
s.add(5)
s.remove(3)            # Raises error if not found
s.discard(3)           # No error if not found
s.clear()

# Set operations
a = {1, 2, 3}
b = {3, 4, 5}

a | b                  # Union: {1, 2, 3, 4, 5}
a & b                  # Intersection: {3}
a - b                  # Difference: {1, 2}
a ^ b                  # Symmetric difference: {1, 2, 4, 5}

# Methods
a.union(b)
a.intersection(b)
a.difference(b)
a.symmetric_difference(b)
a.issubset(b)
a.issuperset(b)
```

### Frozensets

Immutable sets.

```python
fs = frozenset([1, 2, 3])
# Same operations as sets, but immutable
```

## Type Conversions

```python
# To integer
int("42")              # 42
int(3.14)              # 3
int("FF", 16)          # 255 (hex)
int("1010", 2)         # 10 (binary)

# To float
float("3.14")          # 3.14
float(42)              # 42.0

# To string
str(42)                # "42"
str(3.14)              # "3.14"
str([1, 2])            # "[1, 2]"
repr(obj)              # Official string representation

# To bool
bool(0)                # False
bool(1)                # True
bool([])               # False
bool([1])              # True

# To list
list("abc")            # ['a', 'b', 'c']
list((1, 2, 3))        # [1, 2, 3]
list(range(5))         # [0, 1, 2, 3, 4]

# To tuple
tuple([1, 2, 3])       # (1, 2, 3)
tuple("abc")           # ('a', 'b', 'c')

# To set
set([1, 2, 2, 3])      # {1, 2, 3}
set("hello")           # {'h', 'e', 'l', 'o'}

# To dict
dict([("a", 1), ("b", 2)])     # {'a': 1, 'b': 2}
dict(a=1, b=2)                 # {'a': 1, 'b': 2}
```

## Type Annotations

Tauraro supports static type annotations that are **enforced at runtime** (unlike Python).

```python
# Annotated variables have enforced types
x: int = 42
x = "hello"        # ERROR! Type mismatch

# Unannotated variables remain dynamic
y = 42
y = "hello"        # OK - dynamic typing

# Function annotations
def greet(name: str) -> str:
    return f"Hello {name}"

# Collection types
numbers: list = [1, 2, 3]
mapping: dict = {"key": "value"}
```

See [Hybrid Typing System](../types/hybrid-typing.md) for more details.

## Type Checking

```python
# Get type
type(42)                       # <class 'int'>
type("hello")                  # <class 'str'>

# Check type
isinstance(42, int)            # True
isinstance("hi", str)          # True
isinstance([1, 2], (list, tuple))  # True

# Check callable
callable(print)                # True
callable(42)                   # False
```

## Memory and Performance

- **Strings**: Immutable, interned for small strings
- **Lists**: Dynamic arrays, O(1) append, O(n) insert
- **Dicts**: Hash tables, O(1) average lookup
- **Sets**: Hash tables, O(1) membership test
- **Tuples**: Immutable, more memory efficient than lists

### Best Practices

1. Use tuples for immutable data
2. Use sets for fast membership testing
3. Use list comprehensions over loops
4. Prefer f-strings for formatting
5. Use type annotations for critical code paths (enables C compilation optimizations)

## Next Steps

- [Variables and Assignments](variables.md)
- [Operators](operators.md)
- [Functions](functions.md)
- [Type System](../types/hybrid-typing.md)
