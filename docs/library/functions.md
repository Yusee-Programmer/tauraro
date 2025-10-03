# Built-in Functions

The Python interpreter has a number of functions and types built into it that are always available. They are listed here in alphabetical order.

## Built-in Functions

### `abs(x)`

Return the absolute value of a number. The argument may be an integer, a floating point number, or an object implementing `__abs__()`. If the argument is a complex number, its magnitude is returned.

```tauraro
>>> abs(-5)
5
>>> abs(3.14)
3.14
>>> abs(3 + 4j)
5.0
```

### `all(iterable)`

Return `True` if all elements of the iterable are true (or if the iterable is empty).

```tauraro
>>> all([True, True, True])
True
>>> all([True, False, True])
False
>>> all([])
True
```

### `any(iterable)`

Return `True` if any element of the iterable is true. If the iterable is empty, return `False`.

```tauraro
>>> any([False, False, True])
True
>>> any([False, False, False])
False
>>> any([])
False
```

### `ascii(object)`

As [repr()](#repr), return a string containing a printable representation of an object, but escape the non-ASCII characters in the string returned by [repr()](#repr) using \x, \u, or \U escapes.

```tauraro
>>> ascii("Hello 世界")
"'Hello \\u4e16\\u754c'"
```

### `bin(x)`

Convert an integer number to a binary string prefixed with "0b".

```tauraro
>>> bin(3)
'0b11'
>>> bin(-10)
'-0b1010'
```

### `bool([x])`

Return a Boolean value, i.e. one of `True` or `False`. x is converted using the standard truth testing procedure.

```tauraro
>>> bool(1)
True
>>> bool(0)
False
>>> bool([])
False
>>> bool([1, 2, 3])
True
```

### `breakpoint(*args, **kws)`

This function drops you into the debugger at the call site.

```tauraro
def divide(a, b):
    if b == 0:
        breakpoint()  # Drop into debugger
    return a / b
```

### `bytearray([source[, encoding[, errors]]])`

Return a new array of bytes.

```tauraro
>>> bytearray(5)
bytearray(b'\x00\x00\x00\x00\x00')
>>> bytearray([1, 2, 3])
bytearray(b'\x01\x02\x03')
>>> bytearray("hello", "utf-8")
bytearray(b'hello')
```

### `bytes([source[, encoding[, errors]]])`

Return a new "bytes" object which is an immutable sequence of integers in the range 0 <= x < 256.

```tauraro
>>> bytes(5)
b'\x00\x00\x00\x00\x00'
>>> bytes([1, 2, 3])
b'\x01\x02\x03'
>>> bytes("hello", "utf-8")
b'hello'
```

### `callable(object)`

Return `True` if the object argument appears callable, `False` if not.

```tauraro
>>> callable(abs)
True
>>> callable(42)
False
```

### `chr(i)`

Return the string representing a character whose Unicode code point is the integer i.

```tauraro
>>> chr(97)
'a'
>>> chr(8364)
'€'
```

### `classmethod(function)`

Transform a method into a class method.

```tauraro
class MyClass:
    @classmethod
    def from_string(cls, string):
        # Create instance from string
        return cls()

# Can be called on class or instance
instance = MyClass.from_string("data")
```

### `compile(source, filename, mode, flags=0, dont_inherit=False, optimize=-1)`

Compile the source into a code or AST object.

```tauraro
code = compile('x = 5', '<string>', 'exec')
exec(code)
print(x)  # 5
```

### `complex([real[, imag]])`

Return a complex number with the value real + imag*1j or convert a string or number to a complex number.

```tauraro
>>> complex(3, 4)
(3+4j)
>>> complex("3+4j")
(3+4j)
```

### `delattr(object, name)`

This is a relative of [setattr()](#setattr). The arguments are an object and a string. The string must be the name of one of the object's attributes.

```tauraro
class Person:
    def __init__(self):
        self.name = "Alice"

p = Person()
delattr(p, "name")
# p.name now raises AttributeError
```

### `dict(**kwarg)`

Create a new dictionary.

```tauraro
>>> dict()
{}
>>> dict(a=1, b=2)
{'a': 1, 'b': 2}
>>> dict([('a', 1), ('b', 2)])
{'a': 1, 'b': 2}
```

### `dir([object])`

Without arguments, return the list of names in the current local scope. With an argument, attempt to return a list of valid attributes for that object.

```tauraro
>>> dir()
['__builtins__', '__doc__', ...]
>>> dir(str)
['__add__', '__class__', '__contains__', ...]
```

### `divmod(a, b)`

Take two (non complex) numbers as arguments and return a pair of numbers consisting of their quotient and remainder when using integer division.

```tauraro
>>> divmod(7, 3)
(2, 1)
>>> divmod(7.5, 3)
(2.0, 1.5)
```

### `enumerate(iterable, start=0)`

Return an enumerate object. iterable must be a sequence, an iterator, or some other object which supports iteration.

```tauraro
>>> seasons = ['Spring', 'Summer', 'Fall', 'Winter']
>>> list(enumerate(seasons))
[(0, 'Spring'), (1, 'Summer'), (2, 'Fall'), (3, 'Winter')]
>>> list(enumerate(seasons, start=1))
[(1, 'Spring'), (2, 'Summer'), (3, 'Fall'), (4, 'Winter')]
```

### `eval(expression[, globals[, locals]])`

The arguments are a string and optional globals and locals. If provided, globals must be a dictionary. If provided, locals can be any mapping object.

```tauraro
>>> x = 1
>>> eval('x + 1')
2
```

### `exec(object[, globals[, locals]])`

This function supports dynamic execution of Python code.

```tauraro
>>> exec('print("Hello, World!")')
Hello, World!
```

### `filter(function, iterable)`

Construct an iterator from those elements of iterable for which function returns true.

```tauraro
>>> list(filter(lambda x: x % 2 == 0, [1, 2, 3, 4, 5]))
[2, 4]
```

### `float([x])`

Return a floating point number constructed from a number or string x.

```tauraro
>>> float(3)
3.0
>>> float("3.14")
3.14
```

### `format(value[, format_spec])`

Convert a value to a "formatted" representation, as controlled by format_spec.

```tauraro
>>> format(1234, ',')
'1,234'
>>> format(1234, 'b')
'10011010010'
```

### `frozenset([iterable])`

Return a new frozenset object, optionally with elements taken from iterable.

```tauraro
>>> frozenset([1, 2, 3, 2, 1])
frozenset({1, 2, 3})
```

### `getattr(object, name[, default])`

Return the value of the named attribute of object. name must be a string.

```tauraro
>>> class Person:
...     def __init__(self):
...         self.name = "Alice"
...
>>> p = Person()
>>> getattr(p, "name")
'Alice'
>>> getattr(p, "age", 0)
0
```

### `globals()`

Return a dictionary representing the current global symbol table.

```tauraro
>>> globals()
{'__name__': '__main__', '__doc__': None, ...}
```

### `hasattr(object, name)`

The arguments are an object and a string. The result is `True` if the string is the name of one of the object's attributes, `False` if not.

```tauraro
>>> class Person:
...     def __init__(self):
...         self.name = "Alice"
...
>>> p = Person()
>>> hasattr(p, "name")
True
>>> hasattr(p, "age")
False
```

### `hash(object)`

Return the hash value of the object (if it has one). Hash values are integers.

```tauraro
>>> hash("hello")
-1649570051
>>> hash(42)
42
```

### `help([object])`

Invoke the built-in help system.

```tauraro
>>> help(str)
# Displays help for str class
>>> help(str.upper)
# Displays help for str.upper method
```

### `hex(x)`

Convert an integer number to a lowercase hexadecimal string prefixed with "0x".

```tauraro
>>> hex(255)
'0xff'
>>> hex(-42)
'-0x2a'
```

### `id(object)`

Return the "identity" of an object. This is an integer which is guaranteed to be unique and constant for this object during its lifetime.

```tauraro
>>> a = [1, 2, 3]
>>> id(a)
140234567890
```

### `input([prompt])`

If the prompt argument is present, it is written to standard output without a trailing newline.

```tauraro
>>> name = input("Enter your name: ")
Enter your name: Alice
>>> print(f"Hello, {name}!")
Hello, Alice!
```

### `int([x])`

Return an integer object constructed from a number or string x, or return 0 if no arguments are given.

```tauraro
>>> int(3.14)
3
>>> int("42")
42
>>> int("ff", 16)
255
```

### `isinstance(object, classinfo)`

Return true if the object argument is an instance of the classinfo argument, or of a (direct, indirect, or virtual) subclass thereof.

```tauraro
>>> isinstance(5, int)
True
>>> isinstance("hello", str)
True
>>> isinstance([1, 2, 3], (list, tuple))
True
```

### `issubclass(class, classinfo)`

Return true if class is a subclass (direct, indirect, or virtual) of classinfo.

```tauraro
>>> issubclass(bool, int)
True
>>> issubclass(float, int)
False
```

### `iter(object[, sentinel])`

Return an iterator object.

```tauraro
>>> iter([1, 2, 3])
<list_iterator object at 0x...>
```

### `len(s)`

Return the length (the number of items) of an object.

```tauraro
>>> len([1, 2, 3])
3
>>> len("hello")
5
```

### `list([iterable])`

Rather than being a function, list is actually a mutable sequence type.

```tauraro
>>> list()
[]
>>> list([1, 2, 3])
[1, 2, 3]
>>> list("hello")
['h', 'e', 'l', 'l', 'o']
```

### `locals()`

Update and return a dictionary representing the current local symbol table.

```tauraro
def example():
    x = 1
    y = 2
    return locals()

>>> example()
{'x': 1, 'y': 2}
```

### `map(function, iterable, ...)`

Return an iterator that applies function to every item of iterable, yielding the results.

```tauraro
>>> list(map(lambda x: x**2, [1, 2, 3, 4]))
[1, 4, 9, 16]
```

### `max(iterable, *[, key, default])`

Return the largest item in an iterable or the largest of two or more arguments.

```tauraro
>>> max([1, 2, 3, 4, 5])
5
>>> max(1, 2, 3, 4, 5)
5
```

### `memoryview(obj)`

Return a "memory view" object created from the given argument.

```tauraro
>>> v = memoryview(b'hello')
>>> v[0]
104
```

### `min(iterable, *[, key, default])`

Return the smallest item in an iterable or the smallest of two or more arguments.

```tauraro
>>> min([1, 2, 3, 4, 5])
1
>>> min(1, 2, 3, 4, 5)
1
```

### `next(iterator[, default])`

Retrieve the next item from the iterator by calling its `__next__()` method.

```tauraro
>>> it = iter([1, 2, 3])
>>> next(it)
1
>>> next(it)
2
```

### `object()`

Return a new featureless object.

```tauraro
>>> obj = object()
>>> type(obj)
<class 'object'>
```

### `oct(x)`

Convert an integer number to an octal string prefixed with "0o".

```tauraro
>>> oct(8)
'0o10'
>>> oct(64)
'0o100'
```

### `open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)`

Open file and return a corresponding file object.

```tauraro
>>> f = open('workfile', 'w')
>>> f.write('Hello, World!')
13
>>> f.close()
```

### `ord(c)`

Given a string representing one Unicode character, return an integer representing the Unicode code point of that character.

```tauraro
>>> ord('a')
97
>>> ord('€')
8364
```

### `pow(base, exp[, mod])`

Return base to the power exp; if mod is present, return base to the power exp, modulo mod.

```tauraro
>>> pow(2, 3)
8
>>> pow(2, 3, 5)
3
```

### `print(*objects, sep=' ', end='\n', file=sys.stdout, flush=False)`

Print objects to the text stream file, separated by sep and followed by end.

```tauraro
>>> print("Hello", "World", sep=", ", end="!\n")
Hello, World!
```

### `property(fget=None, fset=None, fdel=None, doc=None)`

Return a property attribute.

```tauraro
class Person:
    def __init__(self):
        self._name = ""
    
    @property
    def name(self):
        return self._name
    
    @name.setter
    def name(self, value):
        self._name = value

>>> p = Person()
>>> p.name = "Alice"
>>> print(p.name)
Alice
```

### `range(stop)`

Rather than being a function, range is actually an immutable sequence type.

```tauraro
>>> list(range(5))
[0, 1, 2, 3, 4]
>>> list(range(1, 6))
[1, 2, 3, 4, 5]
>>> list(range(0, 10, 2))
[0, 2, 4, 6, 8]
```

### `repr(object)`

Return a string containing a printable representation of an object.

```tauraro
>>> repr("hello")
"'hello'"
>>> repr(42)
'42'
```

### `reversed(seq)`

Return a reverse iterator.

```tauraro
>>> list(reversed([1, 2, 3, 4]))
[4, 3, 2, 1]
```

### `round(number[, ndigits])`

Return number rounded to ndigits precision after the decimal point.

```tauraro
>>> round(3.14159, 2)
3.14
>>> round(1234, -2)
1200
```

### `set([iterable])`

Return a new set object, optionally with elements taken from iterable.

```tauraro
>>> set([1, 2, 3, 2, 1])
{1, 2, 3}
```

### `setattr(object, name, value)`

This is the counterpart of [getattr()](#getattr). The arguments are an object, a string, and an arbitrary value.

```tauraro
>>> class Person:
...     pass
...
>>> p = Person()
>>> setattr(p, "name", "Alice")
>>> p.name
'Alice'
```

### `slice(stop)`

Return a slice object representing the set of indices specified by range(start, stop, step).

```tauraro
>>> s = slice(1, 5, 2)
>>> [0, 1, 2, 3, 4, 5][s]
[1, 3]
```

### `sorted(iterable, *, key=None, reverse=False)`

Return a new sorted list from the items in iterable.

```tauraro
>>> sorted([3, 1, 4, 1, 5, 9, 2])
[1, 1, 2, 3, 4, 5, 9]
>>> sorted(['bob', 'about', 'Zoo', 'Credit'], key=str.lower)
['about', 'bob', 'Credit', 'Zoo']
```

### `staticmethod(function)`

Transform a method into a static method.

```tauraro
class MyClass:
    @staticmethod
    def utility_function(x, y):
        return x + y

# Can be called on class or instance
result = MyClass.utility_function(1, 2)
```

### `str(object='')`

Return a str version of object.

```tauraro
>>> str(42)
'42'
>>> str([1, 2, 3])
'[1, 2, 3]'
```

### `sum(iterable[, start])`

Sums start and the items of an iterable from left to right and returns the total.

```tauraro
>>> sum([1, 2, 3, 4, 5])
15
>>> sum([1, 2, 3, 4, 5], 10)
25
```

### `super([type[, object-or-type]])`

Return a proxy object that delegates method calls to a parent or sibling class of type.

```tauraro
class Animal:
    def speak(self):
        return "Some sound"

class Dog(Animal):
    def speak(self):
        return "Woof! " + super().speak()

>>> d = Dog()
>>> d.speak()
'Woof! Some sound'
```

### `tuple([iterable])`

Rather than being a function, tuple is actually an immutable sequence type.

```tauraro
>>> tuple()
()
>>> tuple([1, 2, 3])
(1, 2, 3)
>>> tuple("hello")
('h', 'e', 'l', 'l', 'o')
```

### `type(object)`

With one argument, return the type of an object.

```tauraro
>>> type(42)
<class 'int'>
>>> type("hello")
<class 'str'>
```

### `type(name, bases, dict)`

With three arguments, return a new type object.

```tauraro
>>> MyClass = type('MyClass', (), {'x': 1})
>>> instance = MyClass()
>>> instance.x
1
```

### `vars([object])`

Return the `__dict__` attribute for a module, class, instance, or any other object with a `__dict__` attribute.

```tauraro
>>> class Person:
...     def __init__(self):
...         self.name = "Alice"
...         self.age = 30
...
>>> p = Person()
>>> vars(p)
{'name': 'Alice', 'age': 30}
```

### `zip(*iterables)`

Make an iterator that aggregates elements from each of the iterables.

```tauraro
>>> list(zip([1, 2, 3], ['a', 'b', 'c']))
[(1, 'a'), (2, 'b'), (3, 'c')]
```

### `__import__(name, globals=None, locals=None, fromlist=(), level=0)`

This function is invoked by the import statement.

```tauraro
>>> math = __import__('math')
>>> math.sqrt(16)
4.0
```