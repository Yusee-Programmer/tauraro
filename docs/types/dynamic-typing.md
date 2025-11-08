# Dynamic Typing

Tauraro supports full dynamic typing, allowing variables to change types at runtime.

## Dynamic Variables

```python
# Variable can hold different types
x = 10        # int
x = "hello"   # str
x = [1, 2, 3] # list
x = {"a": 1}  # dict
```

## When to Use Dynamic Typing

### Prototyping

```python
# Quick prototype - no type annotations needed
def process(data):
    if isinstance(data, list):
        return sum(data)
    elif isinstance(data, dict):
        return sum(data.values())
    else:
        return data
```

### Polymorphic Functions

```python
def flexible_add(a, b):
    """Works with multiple types."""
    return a + b

# Works with numbers
flexible_add(1, 2)  # 3

# Works with strings
flexible_add("Hello", " World")  # "Hello World"

# Works with lists
flexible_add([1, 2], [3, 4])  # [1, 2, 3, 4]
```

## Performance Trade-offs

```python
# Dynamic - flexible but slower when compiled
def sum_dynamic(numbers):
    total = 0
    for n in numbers:
        total += n
    return total

# Static - faster when compiled
def sum_static(numbers: List[int]) -> int:
    total: int = 0
    for n in numbers:
        total += n
    return total
```

**Performance:** Static typed version is ~100x faster when compiled!

## Type Checking at Runtime

```python
def safe_divide(a, b):
    if not isinstance(a, (int, float)):
        raise TypeError("a must be a number")
    if not isinstance(b, (int, float)):
        raise TypeError("b must be a number")
    if b == 0:
        raise ValueError("Cannot divide by zero")

    return a / b
```

## Next Steps

- [Hybrid Typing](hybrid-typing.md) - Mix static and dynamic
- [Type Annotations](annotations.md) - Adding type hints
- [Static Checking](static-checking.md) - Compile-time checking
