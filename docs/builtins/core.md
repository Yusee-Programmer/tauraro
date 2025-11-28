# Core Built-in Functions

Tauraro includes all standard Python built-in functions. This reference covers the most commonly used functions.

## Input/Output

### `print(*objects, sep=' ', end='\n')`

Print objects to standard output.

```python
print("Hello, World!")
print("Answer:", 42)
print("a", "b", "c", sep="-")  # a-b-c
print("No newline", end="")
```

### `input(prompt='')`

Read a line from standard input.

```python
name = input("Enter your name: ")
age = int(input("Enter your age: "))
```

## Type Conversions

### `int(x, base=10)`

Convert to integer.

```python
int("42")        # 42
int(3.14)        # 3
int("FF", 16)    # 255 (hexadecimal)
int("1010", 2)   # 10 (binary)
```

### `float(x)`

Convert to floating-point.

```python
float("3.14")    # 3.14
float(42)        # 42.0
```

### `str(x)`

Convert to string.

```python
str(42)          # "42"
str(3.14)        # "3.14"
str([1, 2, 3])   # "[1, 2, 3]"
```

### `bool(x)`

Convert to boolean.

```python
bool(0)          # False
bool(42)         # True
bool("")         # False
bool("text")     # True
bool([])         # False
bool([1, 2])     # True
```

### `list(iterable)`

Convert to list.

```python
list("abc")              # ['a', 'b', 'c']
list(range(5))           # [0, 1, 2, 3, 4]
list((1, 2, 3))          # [1, 2, 3]
```

### `tuple(iterable)`

Convert to tuple.

```python
tuple([1, 2, 3])         # (1, 2, 3)
tuple("abc")             # ('a', 'b', 'c')
```

### `dict(**kwargs)` or `dict(mapping)`

Create dictionary.

```python
dict(a=1, b=2)           # {'a': 1, 'b': 2}
dict([('a', 1), ('b', 2)])  # {'a': 1, 'b': 2}
```

### `set(iterable)`

Create set.

```python
set([1, 2, 2, 3])        # {1, 2, 3}
set("hello")             # {'h', 'e', 'l', 'o'}
```

## Sequence Operations

### `len(s)`

Return length of sequence.

```python
len("hello")             # 5
len([1, 2, 3])           # 3
len({"a": 1, "b": 2})    # 2
```

### `range(start, stop, step)`

Generate sequence of numbers.

```python
range(5)                 # 0, 1, 2, 3, 4
range(2, 10)             # 2, 3, 4, 5, 6, 7, 8, 9
range(0, 10, 2)          # 0, 2, 4, 6, 8
range(10, 0, -1)         # 10, 9, 8, ..., 1
```

### `enumerate(iterable, start=0)`

Iterate with index.

```python
for i, item in enumerate(['a', 'b', 'c']):
    print(i, item)
# 0 a
# 1 b
# 2 c
```

### `zip(*iterables)`

Iterate multiple sequences in parallel.

```python
names = ['Alice', 'Bob', 'Charlie']
ages = [25, 30, 35]
for name, age in zip(names, ages):
    print(f"{name}: {age}")
```

### `reversed(seq)`

Reverse iteration.

```python
for x in reversed([1, 2, 3]):
    print(x)  # 3, 2, 1
```

### `sorted(iterable, key=None, reverse=False)`

Return sorted list.

```python
sorted([3, 1, 2])              # [1, 2, 3]
sorted([3, 1, 2], reverse=True)  # [3, 2, 1]
sorted(["b", "a", "c"])        # ['a', 'b', 'c']
sorted(words, key=len)         # Sort by length
```

## Mathematical Functions

### `abs(x)`

Absolute value.

```python
abs(-5)          # 5
abs(3.14)        # 3.14
abs(-2.5)        # 2.5
```

### `min(*args)` / `max(*args)`

Minimum/maximum value.

```python
min(1, 2, 3)             # 1
max([1, 5, 3])           # 5
min("apple", "banana")   # 'apple'
```

### `sum(iterable, start=0)`

Sum of numbers.

```python
sum([1, 2, 3])           # 6
sum(range(10))           # 45
sum([1, 2, 3], 10)       # 16 (10 + 1 + 2 + 3)
```

### `round(number, ndigits=0)`

Round to n digits.

```python
round(3.14159)           # 3
round(3.14159, 2)        # 3.14
round(42.5)              # 42
```

### `pow(base, exp, mod=None)`

Power operation.

```python
pow(2, 3)                # 8
pow(2, 10)               # 1024
pow(2, 10, 100)          # 24 (2^10 mod 100)
```

### `divmod(a, b)`

Division and modulo.

```python
divmod(10, 3)            # (3, 1)
quotient, remainder = divmod(17, 5)  # 3, 2
```

## Higher-Order Functions

### `map(function, *iterables)`

Apply function to every item.

```python
list(map(str, [1, 2, 3]))           # ['1', '2', '3']
list(map(lambda x: x**2, [1, 2, 3]))  # [1, 4, 9]
list(map(max, [1, 2], [3, 1]))      # [3, 2]
```

### `filter(function, iterable)`

Filter items by function.

```python
list(filter(lambda x: x > 0, [-2, -1, 0, 1, 2]))  # [1, 2]
list(filter(None, [0, 1, False, True, "", "hi"]))  # [1, True, "hi"]
```

### `all(iterable)`

True if all elements are true.

```python
all([True, True, True])   # True
all([True, False, True])  # False
all([])                   # True
```

### `any(iterable)`

True if any element is true.

```python
any([False, True, False])  # True
any([False, False])        # False
any([])                    # False
```

## Type Checking

### `type(object)`

Get type of object.

```python
type(42)                 # <class 'int'>
type("hello")            # <class 'str'>
type([1, 2, 3])          # <class 'list'>
```

### `isinstance(object, classinfo)`

Check if object is instance of class.

```python
isinstance(42, int)              # True
isinstance("hi", str)            # True
isinstance([1, 2], (list, tuple))  # True
```

### `issubclass(class, classinfo)`

Check if class is subclass.

```python
class A: pass
class B(A): pass

issubclass(B, A)         # True
issubclass(A, B)         # False
```

### `callable(object)`

Check if object is callable.

```python
callable(print)          # True
callable(lambda: None)   # True
callable(42)             # False
```

## Attribute Access

### `getattr(object, name, default=None)`

Get attribute value.

```python
class Person:
    name = "Alice"

getattr(Person, "name")           # "Alice"
getattr(Person, "age", 0)         # 0 (default)
```

### `setattr(object, name, value)`

Set attribute value.

```python
setattr(Person, "age", 30)
```

### `hasattr(object, name)`

Check if attribute exists.

```python
hasattr(Person, "name")           # True
hasattr(Person, "email")          # False
```

### `delattr(object, name)`

Delete attribute.

```python
delattr(Person, "age")
```

## Introspection

### `dir(object=None)`

List attributes and methods.

```python
dir([])                  # List methods of list
dir(str)                 # String methods
dir()                    # Names in current scope
```

### `vars(object=None)`

Return __dict__ of object.

```python
class Person:
    def __init__(self):
        self.name = "Alice"

p = Person()
vars(p)                  # {'name': 'Alice'}
```

### `globals()`

Return global symbol table.

```python
x = 10
'x' in globals()         # True
```

### `locals()`

Return local symbol table.

```python
def func():
    y = 20
    return locals()  # {'y': 20}
```

### `help(object=None)`

Display help information.

```python
help(print)              # Show print() documentation
help(str.upper)          # Show str.upper() docs
help()                   # Interactive help
```

### `id(object)`

Get identity of object.

```python
x = [1, 2, 3]
id(x)                    # Memory address
```

### `hash(object)`

Get hash value.

```python
hash("hello")            # Hash of string
hash(42)                 # Hash of int
hash((1, 2))             # Hash of tuple
```

## String Formatting

### `format(value, format_spec='')`

Format value to string.

```python
format(42, '05d')        # '00042'
format(3.14159, '.2f')   # '3.14'
format(255, 'x')         # 'ff' (hexadecimal)
format(10, 'b')          # '1010' (binary)
```

### `repr(object)`

Return official string representation.

```python
repr("hello")            # "'hello'"
repr([1, 2, 3])          # '[1, 2, 3]'
```

### `ascii(object)`

Return ASCII-only representation.

```python
ascii("hello")           # "'hello'"
ascii("café")            # "'caf\\xe9'"
```

## Character Operations

### `chr(i)`

Character from Unicode code.

```python
chr(65)                  # 'A'
chr(8364)                # '€'
```

### `ord(c)`

Unicode code from character.

```python
ord('A')                 # 65
ord('€')                 # 8364
```

## Binary Operations

### `bin(x)`

Binary representation.

```python
bin(10)                  # '0b1010'
bin(255)                 # '0b11111111'
```

### `hex(x)`

Hexadecimal representation.

```python
hex(255)                 # '0xff'
hex(16)                  # '0x10'
```

### `oct(x)`

Octal representation.

```python
oct(8)                   # '0o10'
oct(64)                  # '0o100'
```

## Iterator Functions

### `iter(object)`

Get iterator from object.

```python
it = iter([1, 2, 3])
next(it)                 # 1
next(it)                 # 2
```

### `next(iterator, default=None)`

Get next item from iterator.

```python
it = iter([1, 2, 3])
next(it)                 # 1
next(it)                 # 2
next(it, "end")          # 3
next(it, "end")          # "end"
```

## Code Execution

### `eval(expression, globals=None, locals=None)`

Evaluate expression.

```python
eval("2 + 2")            # 4
eval("x * 2", {"x": 10})  # 20
```

### `exec(code, globals=None, locals=None)`

Execute Python code.

```python
exec("x = 10")
exec("print('Hello')")
```

### `compile(source, filename, mode)`

Compile source to code object.

```python
code = compile("print('hi')", "<string>", "exec")
exec(code)
```

## Object Creation

### `object()`

Create base object.

```python
obj = object()
```

### `classmethod(function)`

Create class method.

```python
class MyClass:
    @classmethod
    def class_method(cls):
        return "Called on class"
```

### `staticmethod(function)`

Create static method.

```python
class MyClass:
    @staticmethod
    def static_method():
        return "No self or cls"
```

### `property(fget=None, fset=None, fdel=None, doc=None)`

Create property.

```python
class C:
    @property
    def x(self):
        return self._x

    @x.setter
    def x(self, value):
        self._x = value
```

### `super(type=None, object=None)`

Access parent class.

```python
class Child(Parent):
    def __init__(self):
        super().__init__()
```

## File Operations

### `open(file, mode='r', encoding=None)`

Open file.

```python
# Read text
with open("file.txt", "r") as f:
    content = f.read()

# Write text
with open("file.txt", "w") as f:
    f.write("Hello")

# Append
with open("file.txt", "a") as f:
    f.write("More")

# Binary mode
with open("file.bin", "rb") as f:
    data = f.read()
```

## Next Steps

- [Type Conversion Functions](conversions.md)
- [I/O Functions](io.md)
- [Introspection Functions](introspection.md)
- [System Programming](system-programming.md) - Memory, pointers, atomics, hardware access
- [Standard Library](../stdlib/modules.md)
