# Data Model

This chapter describes the data model of Tauraro, focusing on the object model and built-in types.

## Objects, Values and Types

All data in a Tauraro program is represented by objects or by relations between objects. Every object has an identity, a type and a value.

An object's identity never changes once it has been created; you may think of it as the object's address in memory. The `is` operator compares the identity of two objects; the `id()` function returns an integer representing its identity.

An object's type determines the operations that the object supports and also defines the possible values for objects of that type. The `type()` function returns an object's type.

The value of some objects can change. Objects whose value can change are said to be mutable; objects whose value is unchangeable once they are created are called immutable.

## The Standard Type Hierarchy

Below is a list of the types that are built into Tauraro. Extension modules and additional built-in modules written in Tauraro may define additional types.

Some of the type descriptions below contain a paragraph listing 'special attributes.' These are attributes that provide access to the implementation and are not intended for general use.

### None

This type has a single value. There is a single object with this value. This object is accessed through the built-in name `None`. It is used to signify the absence of a value in many situations.

### NotImplemented

This type has a single value. There is a single object with this value. This object is accessed through the built-in name `NotImplemented`. Numeric methods and rich comparison methods should return this value if they do not implement the operation for the operands provided.

### Ellipsis

This type has a single value. There is a single object with this value. This object is accessed through the literal `...` or the built-in name `Ellipsis`. Its truth value is true.

### Numbers

These are created by numeric literals or as the result of built-in functions and operators. Python distinguishes between integers, floating point numbers, and complex numbers:

- **Integers** have unlimited precision.
- **Floating point numbers** are usually implemented using double in C.
- **Complex numbers** have a real and imaginary part, each of which is a floating point number.

### Sequences

These represent finite ordered sets indexed by non-negative numbers. The built-in sequence types are:

- **Strings**: Immutable sequences of Unicode code points.
- **Lists**: Mutable sequences, typically used to store collections of homogeneous items.
- **Tuples**: Immutable sequences, typically used to store collections of heterogeneous data.
- **Bytes**: Immutable sequences of single bytes.
- **Bytearrays**: Mutable sequences of single bytes.
- **Ranges**: Represents an immutable sequence of numbers and is commonly used for looping a specific number of times in for loops.

Most sequence types support the following operations:

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

### Mutable Sequences

Mutable sequences can be changed after they are created. The subscription and slicing notations can be used as the target of assignment and del (delete) statements.

They support the following operations in addition to those supported by immutable sequences:

| Operation | Result |
|-----------|--------|
| `s[i] = x` | item i of s is replaced by x |
| `s[i:j] = t` | slice of s from i to j is replaced by the contents of the iterable t |
| `del s[i:j]` | same as `s[i:j] = []` |
| `s[i:j:k] = t` | the elements of `s[i:j:k]` are replaced by those of t |
| `del s[i:j:k]` | removes the elements of `s[i:j:k]` from the list |
| `s.append(x)` | appends x to the end of the sequence |
| `s.clear()` | removes all items from s |
| `s.copy()` | creates a shallow copy of s |
| `s.extend(t)` | extends s with the contents of t |
| `s.insert(i, x)` | inserts x into s at the index given by i |
| `s.pop([i])` | retrieves the item at i and also removes it from s |
| `s.remove(x)` | remove the first item from s where `s[i] == x` |
| `s.reverse()` | reverses the items of s in place |

### Set Types

These represent unordered, finite sets of unique, immutable objects. The built-in set types are:

- **Sets**: Mutable sets, created by the built-in `set()` constructor.
- **Frozensets**: Immutable sets, created by the built-in `frozenset()` constructor.

They support the following operations:

| Operation | Result |
|-----------|--------|
| `len(s)` | cardinality of set s |
| `x in s` | test x for membership in s |
| `x not in s` | test x for non-membership in s |
| `s.isdisjoint(t)` | True if s has no elements in common with t |
| `s.issubset(t)` | True if every element in s is in t |
| `s <= t` | test whether every element in s is in t |
| `s < t` | test whether s is a proper subset of t |
| `s.issuperset(t)` | True if every element in t is in s |
| `s >= t` | test whether every element in t is in s |
| `s > t` | test whether s is a proper superset of t |
| `s.union(t)` | new set with elements from both s and t |
| `s | t` | new set with elements from both s and t |
| `s.intersection(t)` | new set with elements common to s and t |
| `s & t` | new set with elements common to s and t |
| `s.difference(t)` | new set with elements in s but not in t |
| `s - t` | new set with elements in s but not in t |
| `s.symmetric_difference(t)` | new set with elements in either s or t but not both |
| `s ^ t` | new set with elements in either s or t but not both |
| `s.copy()` | new set with a shallow copy of s |

### Mappings

These represent finite sets of objects indexed by arbitrary index sets. The only built-in mapping type is the dictionary.

Dictionaries are mutable; they can be changed after they are created. The subscription notation can be used as the target of assignment and del (delete) statements.

They support the following operations:

| Operation | Result |
|-----------|--------|
| `len(d)` | number of items in the dictionary d |
| `d[key]` | value corresponding to key |
| `d[key] = value` | set d[key] to value |
| `del d[key]` | remove d[key] from d |
| `key in d` | True if d has a key key, else False |
| `key not in d` | True if d has no key key, else False |
| `d.clear()` | remove all items from d |
| `d.copy()` | a shallow copy of d |
| `d.get(key[, default])` | value for key if key in d, else default |
| `d.items()` | a set-like object providing a view on d's items |
| `d.keys()` | a set-like object providing a view on d's keys |
| `d.values()` | an object providing a view on d's values |
| `d.pop(key[, default])` | value for key if key in d, else default |
| `d.popitem()` | remove and return an arbitrary (key, value) pair from d |
| `d.setdefault(key[, default])` | value for key if key in d, else set d[key]=default |
| `d.update([other])` | update d with key/value pairs from other |

### Callable Types

These are types that support function call syntax (using parentheses). For user-defined classes, defining a `__call__()` method makes its instances callable.

The built-in callable types are:

- **Functions**: Created by function definitions. The only operation on a function is to call it.
- **Methods**: Functions that are called "on" an object. Methods are accessed as attributes of objects.
- **Classes**: Class objects are callable. Calling a class object creates a new instance of the class.
- **Class Instances**: Instances of classes that implement the `__call__()` method.
- **Built-in Functions**: Functions implemented in C or other built-in modules.
- **Built-in Methods**: Methods implemented in C or other built-in modules.

### Modules

Modules are a basic organizational unit of Tauraro code. A module object has a namespace implemented by a dictionary object. Module attribute access is mapped to dictionary lookups in this namespace.

### Custom Classes

Custom class types are typically created by class definitions. A class has a namespace implemented by a dictionary object. Class attribute references are translated to lookups in this dictionary.

When a class attribute reference would yield a class method object, it is transformed into a method object whose `__self__` attribute is the class.

### Class Instances

When a class defines an `__init__()` method, class instantiation automatically invokes `__init__()` for the newly-created class instance.

### I/O Objects (also known as File Objects)

A file object represents an open file. File objects are implemented using C's stdio package and can be created with the built-in `open()` function.

### Internal Types

A few types used internally by the interpreter are exposed to the user. Their definitions may change with future versions of the interpreter.

## Special Method Names

A class can implement certain operations that are invoked by special syntax (such as arithmetic operations or subscripting and slicing) by defining methods with special names. This is Python's approach to operator overloading, allowing classes to define their own behavior with respect to language operators.

### Basic Customization

| Method | Description |
|--------|-------------|
| `__new__(cls[, ...])` | Called to create a new instance of class cls |
| `__init__(self[, ...])` | Called after the instance has been created |
| `__del__(self)` | Called when the instance is about to be destroyed |
| `__repr__(self)` | Called by the repr() built-in function |
| `__str__(self)` | Called by str() and the print() function |
| `__bytes__(self)` | Called by bytes() to compute a byte-string representation |
| `__format__(self, format_spec)` | Called by the format() built-in function |
| `__lt__(self, other)` | Called for comparisons with the < operator |
| `__le__(self, other)` | Called for comparisons with the <= operator |
| `__eq__(self, other)` | Called for comparisons with the == operator |
| `__ne__(self, other)` | Called for comparisons with the != operator |
| `__gt__(self, other)` | Called for comparisons with the > operator |
| `__ge__(self, other)` | Called for comparisons with the >= operator |
| `__hash__(self)` | Called by built-in function hash() |
| `__bool__(self)` | Called by built-in function bool() |

### Customizing Attribute Access

| Method | Description |
|--------|-------------|
| `__getattr__(self, name)` | Called when an attribute lookup has not found the attribute |
| `__getattribute__(self, name)` | Called unconditionally to implement attribute accesses |
| `__setattr__(self, name, value)` | Called when an attribute assignment is attempted |
| `__delattr__(self, name)` | Called when an attribute deletion is attempted |
| `__dir__(self)` | Called when dir() is invoked on the object |

### Implementing Descriptors

| Method | Description |
|--------|-------------|
| `__get__(self, instance, owner)` | Called to get the attribute of the owner class |
| `__set__(self, instance, value)` | Called to set the attribute on an instance |
| `__delete__(self, instance)` | Called to delete the attribute on an instance |

### Customizing Class Creation

| Method | Description |
|--------|-------------|
| `__init_subclass__(cls)` | Called when a class is subclassed |
| `__instancecheck__(self, instance)` | Called to implement isinstance(instance, class) |
| `__subclasscheck__(self, subclass)` | Called to implement issubclass(subclass, class) |

### Emulating Generic Types

| Method | Description |
|--------|-------------|
| `__class_getitem__(cls, item)` | Called to implement cls[item] |

### Emulating Callable Objects

| Method | Description |
|--------|-------------|
| `__call__(self[, args...])` | Called when the instance is "called" as a function |

### Emulating Container Types

| Method | Description |
|--------|-------------|
| `__len__(self)` | Called to implement the built-in function len() |
| `__length_hint__(self)` | Called to implement operator.length_hint() |
| `__getitem__(self, key)` | Called to implement evaluation of self[key] |
| `__setitem__(self, key, value)` | Called to implement assignment to self[key] |
| `__delitem__(self, key)` | Called to implement deletion of self[key] |
| `__missing__(self, key)` | Called by dict.__getitem__() to implement self[key] |
| `__iter__(self)` | Called to implement iteration |
| `__reversed__(self)` | Called to implement reversed() |
| `__contains__(self, item)` | Called to implement membership test operators |

### Emulating Numeric Types

| Method | Description |
|--------|-------------|
| `__add__(self, other)` | Called to implement the binary arithmetic operation + |
| `__sub__(self, other)` | Called to implement the binary arithmetic operation - |
| `__mul__(self, other)` | Called to implement the binary arithmetic operation * |
| `__matmul__(self, other)` | Called to implement the binary arithmetic operation @ |
| `__truediv__(self, other)` | Called to implement the binary arithmetic operation / |
| `__floordiv__(self, other)` | Called to implement the binary arithmetic operation // |
| `__mod__(self, other)` | Called to implement the binary arithmetic operation % |
| `__divmod__(self, other)` | Called to implement the built-in function divmod() |
| `__pow__(self, other[, modulo])` | Called to implement the binary arithmetic operation ** |
| `__lshift__(self, other)` | Called to implement the bitwise left-shift operation << |
| `__rshift__(self, other)` | Called to implement the bitwise right-shift operation >> |
| `__and__(self, other)` | Called to implement the bitwise AND operation & |
| `__xor__(self, other)` | Called to implement the bitwise XOR operation ^ |
| `__or__(self, other)` | Called to implement the bitwise OR operation \| |
| `__radd__(self, other)` | Called to implement the binary arithmetic operation + with reflected operands |
| `__rsub__(self, other)` | Called to implement the binary arithmetic operation - with reflected operands |
| `__rmul__(self, other)` | Called to implement the binary arithmetic operation * with reflected operands |
| `__rmatmul__(self, other)` | Called to implement the binary arithmetic operation @ with reflected operands |
| `__rtruediv__(self, other)` | Called to implement the binary arithmetic operation / with reflected operands |
| `__rfloordiv__(self, other)` | Called to implement the binary arithmetic operation // with reflected operands |
| `__rmod__(self, other)` | Called to implement the binary arithmetic operation % with reflected operands |
| `__rdivmod__(self, other)` | Called to implement the built-in function divmod() with reflected operands |
| `__rpow__(self, other[, modulo])` | Called to implement the binary arithmetic operation ** with reflected operands |
| `__rlshift__(self, other)` | Called to implement the bitwise left-shift operation << with reflected operands |
| `__rrshift__(self, other)` | Called to implement the bitwise right-shift operation >> with reflected operands |
| `__rand__(self, other)` | Called to implement the bitwise AND operation & with reflected operands |
| `__rxor__(self, other)` | Called to implement the bitwise XOR operation ^ with reflected operands |
| `__ror__(self, other)` | Called to implement the bitwise OR operation \| with reflected operands |
| `__iadd__(self, other)` | Called to implement the augmented arithmetic assignment += |
| `__isub__(self, other)` | Called to implement the augmented arithmetic assignment -= |
| `__imul__(self, other)` | Called to implement the augmented arithmetic assignment *= |
| `__imatmul__(self, other)` | Called to implement the augmented arithmetic assignment @= |
| `__itruediv__(self, other)` | Called to implement the augmented arithmetic assignment /= |
| `__ifloordiv__(self, other)` | Called to implement the augmented arithmetic assignment //= |
| `__imod__(self, other)` | Called to implement the augmented arithmetic assignment %= |
| `__ipow__(self, other[, modulo])` | Called to implement the augmented arithmetic assignment **= |
| `__ilshift__(self, other)` | Called to implement the augmented bitwise left-shift assignment <<= |
| `__irshift__(self, other)` | Called to implement the augmented bitwise right-shift assignment >>= |
| `__iand__(self, other)` | Called to implement the augmented bitwise AND assignment &= |
| `__ixor__(self, other)` | Called to implement the augmented bitwise XOR assignment ^= |
| `__ior__(self, other)` | Called to implement the augmented bitwise OR assignment \|= |
| `__neg__(self)` | Called to implement the unary arithmetic operation - |
| `__pos__(self)` | Called to implement the unary arithmetic operation + |
| `__abs__(self)` | Called to implement the built-in function abs() |
| `__invert__(self)` | Called to implement the unary arithmetic operation ~ |

### With Statement Context Managers

| Method | Description |
|--------|-------------|
| `__enter__(self)` | Enter the runtime context related to this object |
| `__exit__(self, exc_type, exc_value, traceback)` | Exit the runtime context related to this object |

## Coroutines

### Asynchronous Iterators

| Method | Description |
|--------|-------------|
| `__aiter__(self)` | Must return an asynchronous iterator object |
| `__anext__(self)` | Must return an awaitable resulting in a next value |

### Asynchronous Context Managers

| Method | Description |
|--------|-------------|
| `__aenter__(self)` | Enter the async runtime context |
| `__aexit__(self, exc_type, exc_value, traceback)` | Exit the async runtime context |