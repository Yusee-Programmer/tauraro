# TauraroLang API Reference

This document provides a comprehensive reference for TauraroLang's built-in functions, VM operations, and core APIs. All functions are implemented natively in Rust for optimal performance while maintaining Python compatibility.

## Table of Contents

1. [Built-in Functions](#built-in-functions)
2. [Type Conversion Functions](#type-conversion-functions)
3. [Mathematical Functions](#mathematical-functions)
4. [String Functions](#string-functions)
5. [Collection Functions](#collection-functions)
6. [Object Introspection Functions](#object-introspection-functions)
7. [I/O Functions](#io-functions)
8. [Utility Functions](#utility-functions)
9. [VM Operations](#vm-operations)
10. [Error Handling](#error-handling)
11. [FFI (Foreign Function Interface)](#ffi-foreign-function-interface)
12. [Python Interoperability](#python-interoperability)

## Built-in Functions

### Core Functions

#### `print(*args, sep=' ', end='\n', file=None, flush=False)`
Print objects to the text stream file, separated by sep and followed by end.

**Parameters:**
- `*args`: Objects to print
- `sep`: String inserted between values (default: ' ')
- `end`: String appended after the last value (default: '\n')
- `file`: File object to write to (default: sys.stdout)
- `flush`: Whether to forcibly flush the stream (default: False)

**Returns:** `None`

**Example:**
```python
print("Hello", "World")  # Output: Hello World
print("A", "B", "C", sep="-")  # Output: A-B-C
print("No newline", end="")  # Output: No newline (no \n)
```

#### `input(prompt='')`
Read a string from standard input.

**Parameters:**
- `prompt`: Optional string to display as prompt

**Returns:** `str` - The input string (without trailing newline)

**Example:**
```python
name = input("Enter your name: ")
age = int(input("Enter your age: "))
```

#### `len(obj)`
Return the length (number of items) of an object.

**Parameters:**
- `obj`: Object with `__len__` method or built-in sequence/collection

**Returns:** `int` - The length of the object

**Supported Types:**
- Strings, lists, tuples, sets, dictionaries
- Objects with `__len__` method

**Example:**
```python
len("hello")        # 5
len([1, 2, 3])      # 3
len({'a': 1, 'b': 2})  # 2
```

#### `type(obj)`
Return the type of an object.

**Parameters:**
- `obj`: Any object

**Returns:** `str` - String representation of the object's type

**Example:**
```python
type(42)        # "int"
type("hello")   # "str"
type([1, 2, 3]) # "list"
```

#### `id(obj)`
Return the identity of an object as an integer.

**Parameters:**
- `obj`: Any object

**Returns:** `int` - Unique identifier for the object

**Example:**
```python
x = [1, 2, 3]
y = x
id(x) == id(y)  # True (same object)
```

#### `hash(obj)`
Return the hash value of an object.

**Parameters:**
- `obj`: Hashable object

**Returns:** `int` - Hash value

**Example:**
```python
hash("hello")   # Integer hash value
hash(42)        # Integer hash value
hash((1, 2, 3)) # Integer hash value
```

## Type Conversion Functions

#### `str(obj='')`
Convert an object to its string representation.

**Parameters:**
- `obj`: Object to convert (default: empty string)

**Returns:** `str` - String representation

**Behavior:**
- Calls `__str__` method if available
- Falls back to `__repr__` if `__str__` not defined
- Uses default string conversion for built-in types

**Example:**
```python
str(42)         # "42"
str(3.14)       # "3.14"
str([1, 2, 3])  # "[1, 2, 3]"
```

#### `int(x=0, base=10)`
Convert a number or string to an integer.

**Parameters:**
- `x`: Number or string to convert (default: 0)
- `base`: Base for string conversion (default: 10)

**Returns:** `int` - Integer value

**Example:**
```python
int("42")       # 42
int(3.14)       # 3
int("1010", 2)  # 10 (binary)
int("ff", 16)   # 255 (hexadecimal)
```

#### `float(x=0.0)`
Convert a string or number to a floating point number.

**Parameters:**
- `x`: String or number to convert (default: 0.0)

**Returns:** `float` - Floating point value

**Example:**
```python
float("3.14")   # 3.14
float(42)       # 42.0
float("inf")    # inf
```

#### `bool(x=False)`
Convert a value to a Boolean.

**Parameters:**
- `x`: Value to convert (default: False)

**Returns:** `bool` - Boolean value

**Falsy Values:** `False`, `None`, `0`, `0.0`, `""`, `[]`, `{}`, `set()`

**Example:**
```python
bool(1)         # True
bool(0)         # False
bool("hello")   # True
bool("")        # False
```

#### `list(iterable=[])`
Create a list from an iterable.

**Parameters:**
- `iterable`: Iterable object (default: empty list)

**Returns:** `list` - New list containing items from iterable

**Example:**
```python
list("hello")           # ['h', 'e', 'l', 'l', 'o']
list((1, 2, 3))        # [1, 2, 3]
list({1, 2, 3})        # [1, 2, 3] (order may vary)
```

#### `tuple(iterable=())`
Create a tuple from an iterable.

**Parameters:**
- `iterable`: Iterable object (default: empty tuple)

**Returns:** `tuple` - New tuple containing items from iterable

**Example:**
```python
tuple([1, 2, 3])       # (1, 2, 3)
tuple("hello")         # ('h', 'e', 'l', 'l', 'o')
```

#### `set(iterable=set())`
Create a set from an iterable.

**Parameters:**
- `iterable`: Iterable object (default: empty set)

**Returns:** `set` - New set containing unique items from iterable

**Example:**
```python
set([1, 2, 2, 3])      # {1, 2, 3}
set("hello")           # {'h', 'e', 'l', 'o'}
```

#### `dict(mapping_or_iterable=None, **kwargs)`
Create a dictionary.

**Parameters:**
- `mapping_or_iterable`: Mapping or iterable of key-value pairs
- `**kwargs`: Keyword arguments to add to dictionary

**Returns:** `dict` - New dictionary

**Example:**
```python
dict([('a', 1), ('b', 2)])     # {'a': 1, 'b': 2}
dict(a=1, b=2)                 # {'a': 1, 'b': 2}
dict({'a': 1}, b=2)            # {'a': 1, 'b': 2}
```

## Mathematical Functions

#### `abs(x)`
Return the absolute value of a number.

**Parameters:**
- `x`: Number (int, float, or complex)

**Returns:** Absolute value (same type as input)

**Example:**
```python
abs(-5)         # 5
abs(-3.14)      # 3.14
abs(3+4j)       # 5.0
```

#### `min(*args, key=None, default=None)`
Return the smallest item in an iterable or the smallest of arguments.

**Parameters:**
- `*args`: Values to compare, or single iterable
- `key`: Function to extract comparison key
- `default`: Value to return if iterable is empty

**Returns:** Minimum value

**Example:**
```python
min(1, 2, 3)           # 1
min([1, 2, 3])         # 1
min("abc", "def")      # "abc"
min([], default=0)     # 0
```

#### `max(*args, key=None, default=None)`
Return the largest item in an iterable or the largest of arguments.

**Parameters:**
- `*args`: Values to compare, or single iterable
- `key`: Function to extract comparison key
- `default`: Value to return if iterable is empty

**Returns:** Maximum value

**Example:**
```python
max(1, 2, 3)           # 3
max([1, 2, 3])         # 3
max("abc", "def")      # "def"
max([], default=0)     # 0
```

#### `sum(iterable, start=0)`
Sum the items of an iterable from left to right.

**Parameters:**
- `iterable`: Iterable of numbers
- `start`: Starting value (default: 0)

**Returns:** Sum of all items plus start value

**Example:**
```python
sum([1, 2, 3])         # 6
sum([1, 2, 3], 10)     # 16
sum(range(5))          # 10
```

#### `round(number, ndigits=None)`
Round a number to a given precision in decimal digits.

**Parameters:**
- `number`: Number to round
- `ndigits`: Number of decimal places (default: 0)

**Returns:** Rounded number

**Example:**
```python
round(3.14159)         # 3
round(3.14159, 2)      # 3.14
round(1234.5, -1)      # 1230.0
```

#### `pow(base, exp, mod=None)`
Return base raised to the power exp.

**Parameters:**
- `base`: Base number
- `exp`: Exponent
- `mod`: Optional modulus for modular exponentiation

**Returns:** `base ** exp` or `(base ** exp) % mod`

**Example:**
```python
pow(2, 3)              # 8
pow(2, 3, 5)           # 3 (8 % 5)
pow(2, -1)             # 0.5
```

#### `divmod(a, b)`
Return the quotient and remainder of dividing a by b.

**Parameters:**
- `a`: Dividend
- `b`: Divisor

**Returns:** `tuple` - (quotient, remainder)

**Example:**
```python
divmod(10, 3)          # (3, 1)
divmod(10.5, 3)        # (3.0, 1.5)
```

## String Functions

#### `ord(c)`
Return the Unicode code point of a character.

**Parameters:**
- `c`: Single character string

**Returns:** `int` - Unicode code point

**Example:**
```python
ord('A')               # 65
ord('€')               # 8364
```

#### `chr(i)`
Return the character for a Unicode code point.

**Parameters:**
- `i`: Unicode code point (0-1114111)

**Returns:** `str` - Single character string

**Example:**
```python
chr(65)                # 'A'
chr(8364)              # '€'
```

#### `hex(x)`
Convert an integer to a hexadecimal string.

**Parameters:**
- `x`: Integer to convert

**Returns:** `str` - Hexadecimal representation with '0x' prefix

**Example:**
```python
hex(255)               # '0xff'
hex(16)                # '0x10'
```

#### `oct(x)`
Convert an integer to an octal string.

**Parameters:**
- `x`: Integer to convert

**Returns:** `str` - Octal representation with '0o' prefix

**Example:**
```python
oct(8)                 # '0o10'
oct(64)                # '0o100'
```

#### `bin(x)`
Convert an integer to a binary string.

**Parameters:**
- `x`: Integer to convert

**Returns:** `str` - Binary representation with '0b' prefix

**Example:**
```python
bin(8)                 # '0b1000'
bin(255)               # '0b11111111'
```

#### `ascii(obj)`
Return an ASCII-only representation of an object.

**Parameters:**
- `obj`: Object to represent

**Returns:** `str` - ASCII representation with non-ASCII characters escaped

**Example:**
```python
ascii('hello')         # "'hello'"
ascii('café')          # "'caf\\xe9'"
```

#### `repr(obj)`
Return a string representation of an object for debugging.

**Parameters:**
- `obj`: Object to represent

**Returns:** `str` - Unambiguous string representation

**Behavior:**
- Calls `__repr__` method if available
- Provides default representation for built-in types

**Example:**
```python
repr("hello")          # "'hello'"
repr([1, 2, 3])        # "[1, 2, 3]"
```

#### `format(value, format_spec='')`
Format a value according to a format specification.

**Parameters:**
- `value`: Value to format
- `format_spec`: Format specification string

**Returns:** `str` - Formatted string

**Example:**
```python
format(42, 'd')        # '42'
format(3.14159, '.2f') # '3.14'
format(255, 'x')       # 'ff'
```

## Collection Functions

#### `range(start=0, stop, step=1)`
Create a range object representing an arithmetic sequence.

**Parameters:**
- `start`: Starting value (default: 0)
- `stop`: Stopping value (exclusive)
- `step`: Step size (default: 1)

**Returns:** `range` - Range object

**Example:**
```python
list(range(5))         # [0, 1, 2, 3, 4]
list(range(1, 6))      # [1, 2, 3, 4, 5]
list(range(0, 10, 2))  # [0, 2, 4, 6, 8]
```

#### `enumerate(iterable, start=0)`
Return an enumerate object yielding pairs of (index, value).

**Parameters:**
- `iterable`: Iterable to enumerate
- `start`: Starting index (default: 0)

**Returns:** `enumerate` - Enumerate object

**Example:**
```python
list(enumerate(['a', 'b', 'c']))           # [(0, 'a'), (1, 'b'), (2, 'c')]
list(enumerate(['a', 'b', 'c'], start=1))  # [(1, 'a'), (2, 'b'), (3, 'c')]
```

#### `zip(*iterables)`
Combine multiple iterables element-wise.

**Parameters:**
- `*iterables`: Multiple iterable objects

**Returns:** `zip` - Zip object yielding tuples

**Example:**
```python
list(zip([1, 2, 3], ['a', 'b', 'c']))     # [(1, 'a'), (2, 'b'), (3, 'c')]
list(zip([1, 2], ['a', 'b'], ['x', 'y'])) # [(1, 'a', 'x'), (2, 'b', 'y')]
```

#### `map(function, *iterables)`
Apply a function to every item of iterables.

**Parameters:**
- `function`: Function to apply
- `*iterables`: One or more iterables

**Returns:** `map` - Map object yielding results

**Example:**
```python
list(map(str, [1, 2, 3]))                  # ['1', '2', '3']
list(map(lambda x, y: x + y, [1, 2], [3, 4]))  # [4, 6]
```

#### `filter(function, iterable)`
Filter items from an iterable based on a predicate function.

**Parameters:**
- `function`: Predicate function (or None for truthiness)
- `iterable`: Iterable to filter

**Returns:** `filter` - Filter object yielding matching items

**Example:**
```python
list(filter(lambda x: x > 0, [-1, 0, 1, 2]))  # [1, 2]
list(filter(None, [0, 1, False, True, ""]))   # [1, True]
```

#### `sorted(iterable, key=None, reverse=False)`
Return a new sorted list from an iterable.

**Parameters:**
- `iterable`: Iterable to sort
- `key`: Function to extract comparison key
- `reverse`: Sort in descending order if True

**Returns:** `list` - New sorted list

**Example:**
```python
sorted([3, 1, 4, 1, 5])                   # [1, 1, 3, 4, 5]
sorted(['apple', 'pie'], key=len)         # ['pie', 'apple']
sorted([1, 2, 3], reverse=True)           # [3, 2, 1]
```

#### `reversed(seq)`
Return a reverse iterator over a sequence.

**Parameters:**
- `seq`: Sequence to reverse

**Returns:** `reversed` - Reverse iterator

**Example:**
```python
list(reversed([1, 2, 3]))                 # [3, 2, 1]
list(reversed("hello"))                   # ['o', 'l', 'l', 'e', 'h']
```

#### `all(iterable)`
Return True if all elements of the iterable are true.

**Parameters:**
- `iterable`: Iterable to check

**Returns:** `bool` - True if all elements are truthy

**Example:**
```python
all([True, True, True])                   # True
all([True, False, True])                  # False
all([])                                   # True (empty iterable)
```

#### `any(iterable)`
Return True if any element of the iterable is true.

**Parameters:**
- `iterable`: Iterable to check

**Returns:** `bool` - True if any element is truthy

**Example:**
```python
any([False, True, False])                 # True
any([False, False, False])                # False
any([])                                   # False (empty iterable)
```

## Object Introspection Functions

#### `dir(obj=None)`
Return a list of valid attributes for an object.

**Parameters:**
- `obj`: Object to inspect (default: current scope)

**Returns:** `list` - List of attribute names

**Example:**
```python
dir([])                # List methods: ['append', 'clear', ...]
dir(str)               # String methods: ['capitalize', 'center', ...]
```

#### `hasattr(obj, name)`
Check if an object has a named attribute.

**Parameters:**
- `obj`: Object to check
- `name`: Attribute name (string)

**Returns:** `bool` - True if attribute exists

**Example:**
```python
hasattr([], 'append')          # True
hasattr([], 'nonexistent')     # False
```

#### `getattr(obj, name, default=None)`
Get a named attribute from an object.

**Parameters:**
- `obj`: Object to get attribute from
- `name`: Attribute name (string)
- `default`: Default value if attribute doesn't exist

**Returns:** Attribute value or default

**Example:**
```python
getattr([], 'append')          # <built-in method append>
getattr([], 'missing', 'N/A')  # 'N/A'
```

#### `setattr(obj, name, value)`
Set a named attribute on an object.

**Parameters:**
- `obj`: Object to set attribute on
- `name`: Attribute name (string)
- `value`: Value to set

**Returns:** `None`

**Example:**
```python
class MyClass:
    pass

obj = MyClass()
setattr(obj, 'x', 42)
obj.x  # 42
```

#### `delattr(obj, name)`
Delete a named attribute from an object.

**Parameters:**
- `obj`: Object to delete attribute from
- `name`: Attribute name (string)

**Returns:** `None`

**Example:**
```python
class MyClass:
    x = 42

obj = MyClass()
delattr(obj, 'x')  # Removes the x attribute
```

#### `isinstance(obj, classinfo)`
Check if an object is an instance of a class or classes.

**Parameters:**
- `obj`: Object to check
- `classinfo`: Class or tuple of classes

**Returns:** `bool` - True if obj is an instance

**Example:**
```python
isinstance(42, int)            # True
isinstance("hello", str)       # True
isinstance([], (list, tuple))  # True
```

#### `issubclass(class_, classinfo)`
Check if a class is a subclass of another class.

**Parameters:**
- `class_`: Class to check
- `classinfo`: Class or tuple of classes

**Returns:** `bool` - True if class_ is a subclass

**Example:**
```python
class Parent:
    pass

class Child(Parent):
    pass

issubclass(Child, Parent)      # True
issubclass(Parent, Child)      # False
```

#### `callable(obj)`
Check if an object is callable (function, method, class, etc.).

**Parameters:**
- `obj`: Object to check

**Returns:** `bool` - True if object is callable

**Example:**
```python
callable(print)                # True
callable(42)                   # False
callable(lambda x: x)          # True
```

#### `vars(obj=None)`
Return the `__dict__` attribute of an object.

**Parameters:**
- `obj`: Object to inspect (default: local scope)

**Returns:** `dict` - Object's namespace dictionary

**Example:**
```python
class MyClass:
    x = 42

obj = MyClass()
vars(obj)                      # {'x': 42}
```

## I/O Functions

#### `open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)`
Open a file and return a file object.

**Parameters:**
- `file`: File path or file descriptor
- `mode`: File mode ('r', 'w', 'a', 'b', 't', '+', etc.)
- `buffering`: Buffer size (-1 for default)
- `encoding`: Text encoding (default: platform default)
- `errors`: Error handling strategy
- `newline`: Newline handling
- `closefd`: Close file descriptor when file is closed
- `opener`: Custom opener

**Returns:** File object

**Common Modes:**
- `'r'`: Read (default)
- `'w'`: Write (truncate existing)
- `'a'`: Append
- `'b'`: Binary mode
- `'t'`: Text mode (default)
- `'+'`: Read and write

**Example:**
```python
with open('file.txt', 'r') as f:
    content = f.read()

with open('data.bin', 'rb') as f:
    binary_data = f.read()
```

## Utility Functions

#### `iter(obj, sentinel=None)`
Return an iterator object.

**Parameters:**
- `obj`: Iterable object or callable
- `sentinel`: Sentinel value for callable (optional)

**Returns:** Iterator object

**Example:**
```python
it = iter([1, 2, 3])
next(it)                       # 1
next(it)                       # 2
```

#### `next(iterator, default=None)`
Get the next item from an iterator.

**Parameters:**
- `iterator`: Iterator object
- `default`: Default value if iterator is exhausted

**Returns:** Next item or default

**Example:**
```python
it = iter([1, 2])
next(it)                       # 1
next(it)                       # 2
next(it, 'done')               # 'done'
```

#### `slice(start, stop, step=None)`
Create a slice object.

**Parameters:**
- `start`: Starting index
- `stop`: Stopping index
- `step`: Step size

**Returns:** `slice` - Slice object

**Example:**
```python
s = slice(1, 5, 2)
[0, 1, 2, 3, 4, 5][s]         # [1, 3]
```

#### `globals()`
Return the global namespace dictionary.

**Returns:** `dict` - Global namespace

**Example:**
```python
x = 42
'x' in globals()               # True
```

#### `locals()`
Return the local namespace dictionary.

**Returns:** `dict` - Local namespace

**Example:**
```python
def func():
    y = 10
    return 'y' in locals()     # True

func()
```

#### `eval(expression, globals=None, locals=None)`
Evaluate a Python expression.

**Parameters:**
- `expression`: String containing Python expression
- `globals`: Global namespace (optional)
- `locals`: Local namespace (optional)

**Returns:** Result of expression evaluation

**Example:**
```python
eval('2 + 3')                  # 5
eval('x + 1', {'x': 10})       # 11
```

#### `exec(object, globals=None, locals=None)`
Execute Python code.

**Parameters:**
- `object`: String or code object containing Python code
- `globals`: Global namespace (optional)
- `locals`: Local namespace (optional)

**Returns:** `None`

**Example:**
```python
exec('x = 42')
print(x)                       # 42
```

#### `compile(source, filename, mode, flags=0, dont_inherit=False, optimize=-1)`
Compile source code into a code object.

**Parameters:**
- `source`: Source code string
- `filename`: Filename for error reporting
- `mode`: Compilation mode ('exec', 'eval', 'single')
- `flags`: Compiler flags
- `dont_inherit`: Don't inherit compiler flags
- `optimize`: Optimization level

**Returns:** Code object

**Example:**
```python
code = compile('x + 1', '<string>', 'eval')
eval(code, {'x': 10})          # 11
```

#### `breakpoint(*args, **kwargs)`
Enter the debugger.

**Parameters:**
- `*args`: Arguments passed to debugger
- `**kwargs`: Keyword arguments passed to debugger

**Returns:** `None`

**Example:**
```python
def debug_function():
    x = 42
    breakpoint()  # Enters debugger here
    return x
```

## Advanced Collection Functions

#### `frozenset(iterable=None)`
Create an immutable set.

**Parameters:**
- `iterable`: Iterable to create frozenset from

**Returns:** `frozenset` - Immutable set

**Example:**
```python
fs = frozenset([1, 2, 3, 2])   # frozenset({1, 2, 3})
fs.add(4)                      # AttributeError (immutable)
```

#### `bytearray(source=None, encoding='utf-8', errors='strict')`
Create a mutable array of bytes.

**Parameters:**
- `source`: Source data (int, iterable, string, or bytes)
- `encoding`: Text encoding for string source
- `errors`: Error handling for encoding

**Returns:** `bytearray` - Mutable byte array

**Example:**
```python
bytearray(5)                   # bytearray of 5 zero bytes
bytearray([1, 2, 3])          # bytearray from list
bytearray('hello', 'utf-8')    # bytearray from string
```

#### `bytes(source=None, encoding='utf-8', errors='strict')`
Create an immutable array of bytes.

**Parameters:**
- `source`: Source data (int, iterable, string, or bytes)
- `encoding`: Text encoding for string source
- `errors`: Error handling for encoding

**Returns:** `bytes` - Immutable byte array

**Example:**
```python
bytes(5)                       # 5 zero bytes
bytes([1, 2, 3])              # bytes from list
bytes('hello', 'utf-8')        # bytes from string
```

## VM Operations

### Memory Management

The TauraroLang VM provides automatic memory management with reference counting and garbage collection:

- **Reference Counting**: Automatic cleanup when reference count reaches zero
- **Garbage Collection**: Handles circular references
- **Arena Allocation**: Efficient memory allocation for objects
- **Memory Modes**: Automatic and manual allocation modes

### Scope Management

The VM maintains a scope stack for variable resolution:

- **Global Scope**: Module-level variables
- **Function Scope**: Local variables in functions
- **Class Scope**: Variables within class definitions
- **Nested Scopes**: Support for closures and nested functions

### Function Calls

Function call mechanism supports:

- **User-defined Functions**: Python-style function definitions
- **Built-in Functions**: Native Rust implementations
- **Native Functions**: FFI-callable functions
- **Method Calls**: Object method invocation with proper `self` binding
- **Dunder Methods**: Special method calls (`__str__`, `__len__`, etc.)

### Class System

Object-oriented programming features:

- **Class Definition**: Dynamic class creation
- **Inheritance**: Single and multiple inheritance
- **Method Resolution Order (MRO)**: C3 linearization algorithm
- **Metaclasses**: Custom class creation behavior
- **Dunder Methods**: Special method support

### Module System

Import and module management:

- **Built-in Modules**: Pre-loaded standard library modules
- **Dynamic Imports**: Runtime module loading
- **Module Caching**: Efficient module reuse
- **Namespace Management**: Proper module isolation

## Error Handling

### Exception Types

TauraroLang supports Python-compatible exception handling:

- **TypeError**: Type-related errors
- **ValueError**: Value-related errors
- **AttributeError**: Attribute access errors
- **KeyError**: Dictionary key errors
- **IndexError**: Sequence index errors
- **NameError**: Name resolution errors
- **RuntimeError**: General runtime errors

### Error Propagation

Errors are propagated through the call stack with proper cleanup:

- **Stack Unwinding**: Automatic scope cleanup on errors
- **Error Context**: Detailed error information with stack traces
- **Resource Cleanup**: Proper cleanup of resources on errors

## Performance Characteristics

### Optimization Features

- **Native Implementation**: All built-ins implemented in Rust
- **Zero-copy Operations**: Efficient data handling where possible
- **SIMD Optimizations**: Vectorized operations for numeric data
- **Lazy Evaluation**: Deferred computation for iterators
- **Inlining**: Function call optimization

### Threading Model

- **No GIL**: True parallel execution
- **Thread Safety**: All built-ins are thread-safe
- **Concurrent Collections**: Lock-free data structures where possible
- **Async Support**: Native async/await implementation

### Memory Efficiency

- **Reference Counting**: Immediate cleanup of unused objects
- **Arena Allocation**: Reduced memory fragmentation
- **Copy-on-Write**: Efficient string and collection handling
- **Garbage Collection**: Handles circular references efficiently

## FFI (Foreign Function Interface)

TauraroLang provides a comprehensive FFI system for calling C functions and working with native libraries. FFI support must be enabled with the `ffi` feature flag.

### FFI Types

#### `FFIType` Enumeration

Represents C data types for FFI operations:

- `FFIType.Void` - C `void` type
- `FFIType.Int8` - C `int8_t` type
- `FFIType.Int16` - C `int16_t` type
- `FFIType.Int32` - C `int32_t` type
- `FFIType.Int64` - C `int64_t` type
- `FFIType.UInt8` - C `uint8_t` type
- `FFIType.UInt16` - C `uint16_t` type
- `FFIType.UInt32` - C `uint32_t` type
- `FFIType.UInt64` - C `uint64_t` type
- `FFIType.Float32` - C `float` type
- `FFIType.Float64` - C `double` type
- `FFIType.Bool` - C `bool` type
- `FFIType.Pointer` - C `void*` type
- `FFIType.String` - C `const char*` type
- `FFIType.Buffer` - Binary data buffer

#### `CallingConvention` Enumeration

Specifies function calling conventions:

- `CallingConvention.C` - Standard C ABI (default)
- `CallingConvention.StdCall` - Windows stdcall convention
- `CallingConvention.FastCall` - Fast call convention
- `CallingConvention.System` - System default convention

### FFI Functions

#### `load_library(path)`
Load an external shared library.

**Parameters:**
- `path`: Path to the shared library (.dll, .so, .dylib)

**Returns:** Library handle or `None` on failure

**Example:**
```python
# Load system C library
libc = load_library("libc")

# Load custom library
mylib = load_library("./libmylib.so")
```

#### `register_function(library, name, return_type, param_types, calling_convention=None)`
Register a function from a loaded library.

**Parameters:**
- `library`: Library handle from `load_library()`
- `name`: Function name in the library
- `return_type`: `FFIType` for return value
- `param_types`: List of `FFIType` for parameters
- `calling_convention`: Optional calling convention (default: C)

**Returns:** `None`

**Example:**
```python
# Register strlen function
register_function(libc, "strlen", FFIType.Int32, [FFIType.String])

# Register custom function with stdcall convention
register_function(mylib, "my_func", FFIType.Float64, 
                 [FFIType.Int32, FFIType.Float64], 
                 CallingConvention.StdCall)
```

#### `call_function(library, name, args)`
Call a registered function from a library.

**Parameters:**
- `library`: Library handle
- `name`: Function name
- `args`: List of arguments matching registered parameter types

**Returns:** Function result converted to TauraroLang value

**Example:**
```python
# Call strlen
length = call_function(libc, "strlen", ["Hello, World!"])  # Returns 13

# Call custom function
result = call_function(mylib, "my_func", [42, 3.14])
```

#### `with_safe_ffi(callback)`
Execute FFI operations with automatic cleanup.

**Parameters:**
- `callback`: Function to execute within safe FFI context

**Returns:** Result of callback function

**Example:**
```python
def ffi_operations():
    lib = load_library("mylib")
    register_function(lib, "process", FFIType.Int32, [FFIType.Buffer])
    return call_function(lib, "process", [b"data"])

result = with_safe_ffi(ffi_operations)  # Library automatically unloaded
```

### Built-in C Functions

TauraroLang provides direct access to common C standard library functions:

#### `malloc(size)`
Allocate memory block.

**Parameters:**
- `size`: Number of bytes to allocate

**Returns:** Pointer to allocated memory or `None`

#### `free(ptr)`
Free allocated memory block.

**Parameters:**
- `ptr`: Pointer returned by `malloc()`

**Returns:** `None`

#### `strlen(str)`
Get string length.

**Parameters:**
- `str`: String to measure

**Returns:** `int` - Length of string

#### `strcmp(str1, str2)`
Compare two strings.

**Parameters:**
- `str1`: First string
- `str2`: Second string

**Returns:** `int` - 0 if equal, <0 if str1 < str2, >0 if str1 > str2

## Python Interoperability

TauraroLang provides bidirectional integration with Python. Python interop must be enabled with the `python-interop` feature flag.

### Python Import Functions

#### `python_import(module_name)`
Import a Python module.

**Parameters:**
- `module_name`: Name of Python module to import

**Returns:** Python module object or `None` on failure

**Example:**
```python
# Import standard library modules
math = python_import("math")
json = python_import("json")
os = python_import("os")

# Import third-party packages
numpy = python_import("numpy")
requests = python_import("requests")
```

#### `python_call(module_or_object, function_name, args)`
Call a Python function or method.

**Parameters:**
- `module_or_object`: Python module or object
- `function_name`: Name of function/method to call
- `args`: List of arguments

**Returns:** Result converted to TauraroLang value

**Example:**
```python
# Call module function
result = python_call(math, "sqrt", [16])  # Returns 4.0
pi_val = python_call(math, "pi", [])      # Access module constant

# Call object method
data = {"key": "value"}
json_str = python_call(json, "dumps", [data])
```

#### `python_eval(code)`
Evaluate Python expression.

**Parameters:**
- `code`: Python expression as string

**Returns:** Result of expression

**Example:**
```python
result = python_eval("2 + 3 * 4")        # Returns 14
pi_val = python_eval("math.pi")          # Returns π value
```

#### `python_exec(code)`
Execute Python statements.

**Parameters:**
- `code`: Python code as string

**Returns:** `None`

**Example:**
```python
python_exec("""
def fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)
""")

# Function is now available in Python namespace
result = python_eval("fibonacci(10)")    # Returns 55
```

### Python Object Manipulation

#### `python_getattr(obj, name)`
Get attribute from Python object.

**Parameters:**
- `obj`: Python object
- `name`: Attribute name

**Returns:** Attribute value

#### `python_setattr(obj, name, value)`
Set attribute on Python object.

**Parameters:**
- `obj`: Python object
- `name`: Attribute name
- `value`: Value to set

**Returns:** `None`

#### `python_hasattr(obj, name)`
Check if Python object has attribute.

**Parameters:**
- `obj`: Python object
- `name`: Attribute name

**Returns:** `bool` - True if attribute exists

### PyO3 Integration

When using TauraroLang from Python, the following classes and functions are available:

#### `TauraroVM` Class

Python class for running TauraroLang code.

**Methods:**

##### `TauraroVM()`
Create a new TauraroLang VM instance.

##### `eval(code)`
Evaluate TauraroLang expression.

**Parameters:**
- `code`: TauraroLang expression as string

**Returns:** Result as Python object

##### `exec(code)`
Execute TauraroLang statements.

**Parameters:**
- `code`: TauraroLang code as string

**Returns:** `None`

##### `set_strict_types(strict)`
Enable or disable strict type checking.

**Parameters:**
- `strict`: Boolean flag for strict typing

##### `get_memory_stats()`
Get VM memory usage statistics.

**Returns:** String with memory statistics

**Example (Python code):**
```python
import tauraro

# Create VM instance
vm = tauraro.TauraroVM()

# Execute TauraroLang code
vm.exec("""
def greet(name):
    return f"Hello, {name}!"
""")

# Evaluate expressions
result = vm.eval("greet('World')")  # Returns "Hello, World!"

# Check memory usage
stats = vm.get_memory_stats()
print(stats)
```

#### Module Functions

##### `tauraro_eval(code)`
Evaluate TauraroLang expression (module-level function).

##### `tauraro_exec(code)`
Execute TauraroLang code (module-level function).

##### `tauraro_call(function_name, args)`
Call TauraroLang function (module-level function).

### Type Conversion

Automatic type conversion between TauraroLang and Python:

#### TauraroLang → Python
- `int` → `int`
- `float` → `float`
- `bool` → `bool`
- `str` → `str`
- `list` → `list`
- `dict` → `dict`
- `None` → `None`

#### Python → TauraroLang
- `int` → `int`
- `float` → `float`
- `bool` → `bool`
- `str` → `str`
- `list` → `list`
- `dict` → `dict`
- `None` → `None`
- Other types → `str` (via `repr()`)

## Usage Examples

### Basic Operations
```python
# Type conversions
x = int("42")
y = float("3.14")
z = str(123)

# Collections
numbers = list(range(10))
unique = set(numbers)
pairs = dict(enumerate(numbers))

# String operations
text = "Hello, World!"
print(len(text))
print(text.upper())
print(repr(text))
```

### Advanced Features
```python
# Functional programming
numbers = [1, 2, 3, 4, 5]
squares = list(map(lambda x: x**2, numbers))
evens = list(filter(lambda x: x % 2 == 0, numbers))
total = sum(numbers)

# Object introspection
class MyClass:
    def __init__(self, value):
        self.value = value
    
    def __str__(self):
        return f"MyClass({self.value})"

obj = MyClass(42)
print(type(obj))
print(hasattr(obj, 'value'))
print(getattr(obj, 'value'))
```

### FFI Examples
```python
# Load and use C library
libc = load_library("libc")
register_function(libc, "strlen", FFIType.Int32, [FFIType.String])
register_function(libc, "strcmp", FFIType.Int32, [FFIType.String, FFIType.String])

# Call C functions
length = call_function(libc, "strlen", ["Hello"])  # Returns 5
comparison = call_function(libc, "strcmp", ["apple", "banana"])  # Returns < 0

# Memory management
register_function(libc, "malloc", FFIType.Pointer, [FFIType.Int32])
register_function(libc, "free", FFIType.Void, [FFIType.Pointer])

ptr = call_function(libc, "malloc", [1024])
if ptr:
    call_function(libc, "free", [ptr])
```

### Python Interop Examples
```python
# Import and use Python modules
math = python_import("math")
json = python_import("json")

# Call Python functions
sqrt_result = python_call(math, "sqrt", [16])  # Returns 4.0
pi_value = python_call(math, "pi", [])         # Returns π

# Work with Python objects
data = {"name": "TauraroLang", "version": "1.0"}
json_string = python_call(json, "dumps", [data])
parsed_data = python_call(json, "loads", [json_string])

# Execute Python code
python_exec("""
import numpy as np
arr = np.array([1, 2, 3, 4, 5])
mean_val = np.mean(arr)
""")

mean_result = python_eval("mean_val")  # Returns 3.0
```

### File I/O
```python
# Reading files
with open('data.txt', 'r') as f:
    content = f.read()
    lines = content.splitlines()

# Writing files
with open('output.txt', 'w') as f:
    for i in range(10):
        f.write(f"Line {i}\n")
```

This comprehensive API reference covers all built-in functions, FFI capabilities, Python interoperability features, and VM operations available in TauraroLang. Each function is implemented with performance and Python compatibility in mind, providing a robust foundation for application development with seamless integration to native libraries and Python ecosystems.