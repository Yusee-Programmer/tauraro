# Built-in Modules Enhancements Documentation

This document describes the enhancements made to the built-in modules in Tauraro to ensure they work like their Python counterparts.

## Collections Module

### Counter Class Enhancements

The `Counter` class now includes additional methods and operations:

- `elements()`: Returns an iterator over elements repeating each as many times as its count
- `most_common(n)`: Returns a list of the n most common elements and their counts
- Arithmetic operations:
  - `+`: Addition (adds counts)
  - `-`: Subtraction (keeps only positive counts)
  - `|`: Union (takes maximum counts)
  - `&`: Intersection (takes minimum counts)

Example usage:
```python
import collections

# Create counters
counter1 = collections.Counter(['a', 'b', 'c', 'a'])
counter2 = collections.Counter(['a', 'b', 'b', 'd'])

# Use enhanced features
print(list(counter1.elements()))  # ['a', 'a', 'b', 'c']
print(counter1.most_common(2))    # [('a', 2), ('b', 1)]

# Arithmetic operations
print(counter1 + counter2)  # Addition
print(counter1 - counter2)  # Subtraction
print(counter1 | counter2)  # Union
print(counter1 & counter2)  # Intersection
```

### DefaultDict Class

The `defaultdict` class now supports all initialization methods:

- Initialization with callable default factory
- Initialization with lambda functions
- Proper handling of missing keys

Example usage:
```python
import collections

# Different default factories
dd1 = collections.defaultdict(list)
dd2 = collections.defaultdict(int)
dd3 = collections.defaultdict(lambda: "missing")

dd1['fruits'].append('apple')
dd2['count'] += 1
print(dd3['nonexistent'])  # "missing"
```

### Deque Class

The `deque` class now includes comprehensive functionality:

- `maxlen` parameter support
- `extend()` and `extendleft()` methods
- `rotate()` method with positive and negative values
- `remove()` method
- Proper handling of maxlen constraints

Example usage:
```python
import collections

# Create deque with maxlen
d = collections.deque([1, 2, 3], maxlen=5)

# Use enhanced methods
d.extend([4, 5])
d.extendleft([0, -1])
d.rotate(2)  # Rotate right by 2
d.remove(2)  # Remove first occurrence of 2
```

## Itertools Module

### Enhanced Iterator Functions

All itertools functions have been enhanced for complete functionality:

- `count(start, step)`: Infinite counter with custom start and step
- `cycle(iterable)`: Infinite cycling through elements
- `repeat(object, times)`: Repeat object n times or infinitely
- `accumulate(iterable, func)`: Accumulate with custom functions
- `chain(*iterables)`: Chain multiple iterables
- `compress(data, selectors)`: Filter elements by selectors
- `dropwhile(predicate, iterable)`: Drop elements while predicate is true
- `takewhile(predicate, iterable)`: Take elements while predicate is true
- `filterfalse(predicate, iterable)`: Filter elements where predicate is false
- `groupby(iterable, key)`: Group consecutive elements
- `islice(iterable, start, stop, step)`: Slice iterator
- `starmap(function, iterable)`: Apply function with unpacked arguments
- `tee(iterable, n)`: Split iterator into n independent iterators
- `zip_longest(*iterables, fillvalue)`: Zip iterators with fill value

### Combinatorial Iterators

Complete implementations of combinatorial functions:

- `product(*iterables, repeat)`: Cartesian product
- `permutations(iterable, r)`: Permutations of length r
- `combinations(iterable, r)`: Combinations of length r
- `combinations_with_replacement(iterable, r)`: Combinations with replacement

Example usage:
```python
import itertools

# Infinite iterators
counter = itertools.count(10, 2)
cycler = itertools.cycle(['A', 'B', 'C'])
repeater = itertools.repeat('Tauraro', 3)

# Iterators on shortest input
numbers = [1, 2, 3, 4, 5]
print(list(itertools.accumulate(numbers, lambda x, y: x * y)))
print(list(itertools.chain([1, 2], ['a', 'b'])))
print(list(itertools.compress(['A', 'B', 'C'], [True, False, True])))

# Combinatorial iterators
print(list(itertools.combinations(['A', 'B', 'C'], 2)))
print(list(itertools.permutations(['A', 'B', 'C'], 2)))
print(list(itertools.product([1, 2], ['a', 'b'])))
```

## Functools Module

### Enhanced Higher-Order Functions

Complete implementations of functools features:

- `reduce(function, iterable, initializer)`: Apply function cumulatively
- `partial(func, *args, **keywords)`: Partial function application
- `lru_cache(maxsize, typed)`: Least-recently-used cache decorator
- `cache()`: Unbounded cache decorator
- `wraps(wrapped, assigned, updated)`: Preserve metadata in decorators
- `total_ordering`: Auto-generate comparison methods
- `cmp_to_key(cmp_func)`: Convert comparison function to key function

Example usage:
```python
import functools

# Partial function application
def multiply(x, y, z):
    return x * y * z

double = functools.partial(multiply, 2)
print(double(3, 4))  # 24

# LRU cache
@functools.lru_cache(maxsize=128)
def fibonacci(n):
    if n < 2:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

print(fibonacci(10))
print(fibonacci.cache_info())

# Wraps decorator
def my_decorator(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print(f"Calling {func.__name__}")
        return func(*args, **kwargs)
    return wrapper

@my_decorator
def greet(name):
    """Greet someone"""
    return f"Hello, {name}!"

print(greet("Tauraro"))
print(greet.__name__)  # "greet"
print(greet.__doc__)   # "Greet someone"

# Total ordering
@functools.total_ordering
class Student:
    def __init__(self, name, grade):
        self.name = name
        self.grade = grade
    
    def __eq__(self, other):
        return self.grade == other.grade
    
    def __lt__(self, other):
        return self.grade < other.grade
```

## Math Module

### New Functions Added

Additional mathematical functions have been implemented:

- `isqrt(x)`: Integer square root
- `nextafter(x, y)`: Next floating-point value after x towards y

### Enhanced Existing Functions

All existing math functions have been verified for complete functionality:

- All trigonometric functions (`sin`, `cos`, `tan`, etc.)
- All hyperbolic functions (`sinh`, `cosh`, `tanh`, etc.)
- All logarithmic functions (`log`, `log2`, `log10`, etc.)
- All power and root functions (`pow`, `sqrt`, etc.)
- Special functions (`gamma`, `lgamma`, `erf`, `erfc`)
- Classification functions (`isfinite`, `isinf`, `isnan`, `isclose`)
- Floating-point operations (`fmod`, `remainder`, `modf`, `frexp`, `ldexp`, `copysign`)

Example usage:
```python
import math

# New functions
print(math.isqrt(16))  # 4
print(math.nextafter(1.0, 2.0))  # Next representable float

# Enhanced functions
print(math.sin(math.pi / 2))  # 1.0
print(math.log(10, 2.71828))  # Natural log
print(math.isclose(1.0, 1.0000001))  # True
print(math.gamma(5))  # 24.0 (4!)
```

## Testing

Comprehensive test suites have been created for all enhanced modules:

- `advanced_collections_test.tauraro`: Tests all collections enhancements
- `advanced_functools_test.tauraro`: Tests all functools enhancements
- `advanced_itertools_test.tauraro`: Tests all itertools enhancements
- `advanced_math_test.tauraro`: Tests all math enhancements

These tests verify that the implementations work correctly and match Python's behavior as closely as possible.