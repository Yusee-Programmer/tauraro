# Type Conversions

Tauraro provides built-in functions for converting between types.

## Numeric Conversions

### int()

Convert to integer:

```python
int(3.14)       # 3
int("42")       # 42
int("1010", 2)  # 10 (binary)
int("FF", 16)   # 255 (hex)
```

### float()

Convert to floating-point:

```python
float(42)       # 42.0
float("3.14")   # 3.14
float("1.5e2")  # 150.0
```

### bool()

Convert to boolean:

```python
bool(1)         # True
bool(0)         # False
bool("hello")   # True
bool("")        # False
bool([1, 2])    # True
bool([])        # False
```

## String Conversions

### str()

Convert to string:

```python
str(42)         # "42"
str(3.14)       # "3.14"
str([1, 2, 3])  # "[1, 2, 3]"
```

### repr()

Get string representation:

```python
repr("hello")   # "'hello'"
repr([1, 2, 3]) # "[1, 2, 3]"
```

## Collection Conversions

### list()

Convert to list:

```python
list("hello")       # ['h', 'e', 'l', 'l', 'o']
list((1, 2, 3))     # [1, 2, 3]
list(range(5))      # [0, 1, 2, 3, 4]
```

### tuple()

Convert to tuple:

```python
tuple([1, 2, 3])    # (1, 2, 3)
tuple("hello")      # ('h', 'e', 'l', 'l', 'o')
```

### set()

Convert to set:

```python
set([1, 2, 2, 3])   # {1, 2, 3}
set("hello")        # {'h', 'e', 'l', 'o'}
```

### dict()

Create dictionary:

```python
dict(a=1, b=2)      # {'a': 1, 'b': 2}
dict([(1, 'a'), (2, 'b')])  # {1: 'a', 2: 'b'}
```

## Bytes Conversions

### bytes()

Convert to bytes:

```python
bytes("hello", "utf-8")  # b'hello'
bytes([72, 101, 108, 108, 111])  # b'Hello'
```

### ord() / chr()

Character/code conversions:

```python
ord('A')        # 65
chr(65)         # 'A'
```

## Next Steps

- [Core Builtins](core.md) - All built-in functions
- [Data Types](../language/data-types.md) - Type system
