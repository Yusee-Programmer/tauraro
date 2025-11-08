# Introspection Functions

Built-in functions for inspecting objects and types.

## type()

Get object type:

```python
type(42)            # <class 'int'>
type("hello")       # <class 'str'>
type([1, 2, 3])     # <class 'list'>
```

## isinstance()

Check if object is instance of class:

```python
isinstance(42, int)         # True
isinstance("hello", str)    # True
isinstance([1, 2], list)    # True
isinstance(3.14, (int, float))  # True
```

## issubclass()

Check if class is subclass:

```python
class Animal:
    pass

class Dog(Animal):
    pass

issubclass(Dog, Animal)  # True
```

## dir()

List object attributes:

```python
dir([])        # List all list methods
dir(math)      # List math module functions
dir(obj)       # List object attributes
```

## hasattr() / getattr() / setattr()

Attribute operations:

```python
class Person:
    name = "Alice"

hasattr(Person, "name")  # True
getattr(Person, "name")  # "Alice"
setattr(Person, "age", 30)  # Sets age = 30
```

## callable()

Check if object is callable:

```python
def func():
    pass

callable(func)      # True
callable(42)        # False
callable(print)     # True
```

## id()

Get object identity:

```python
x = [1, 2, 3]
id(x)  # Memory address
```

## vars()

Get object's `__dict__`:

```python
class Person:
    def __init__(self, name):
        self.name = name

p = Person("Alice")
vars(p)  # {'name': 'Alice'}
```

## Next Steps

- [Core Builtins](core.md) - All built-in functions
- [Classes](../language/classes.md) - Object-oriented programming
