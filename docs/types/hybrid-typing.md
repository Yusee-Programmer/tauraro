# Hybrid Typing System

Tauraro features a **hybrid typing system** that combines the flexibility of dynamic typing with the safety and performance of static typing.

## Overview

Unlike Python where type annotations are just hints, Tauraro **enforces type annotations at runtime** while still allowing dynamic typing for unannotated variables.

## Dynamic Typing (Default)

Variables without type annotations work like Python:

```python
# Dynamic - can change types freely
x = 10
x = "hello"
x = [1, 2, 3]
# All assignments work fine
```

## Static Typing (Opt-in)

Variables with type annotations are **type-checked and enforced**:

```python
# Static - type is enforced
x: int = 10
x = 20        # OK - still an int
x = "hello"   # TypeError! Cannot assign str to int variable
```

## Benefits of Hybrid Typing

### 1. **Gradual Adoption**

Start with dynamic typing, add types where needed:

```python
# Rapid prototyping - no types
def process_data(data):
    return data * 2

# Production code - add types for safety
def process_data(data: list[int]) -> list[int]:
    return [x * 2 for x in data]
```

### 2. **Performance Optimization**

Type annotations enable optimizations:

```python
# Without types - slower (boxed values)
def add(a, b):
    return a + b

# With types - faster (primitive arithmetic)
def add_fast(a: int, b: int) -> int:
    return a + b
```

When compiled to C:
- `add()` uses `tauraro_value_t*` (boxed, heap-allocated)
- `add_fast()` uses `int64_t` (primitive, stack-allocated)

**Performance difference**: 5-10x faster for numeric operations!

### 3. **Early Error Detection**

Catch type errors immediately:

```python
>>> balance: float = 100.0
>>> balance = "insufficient funds"  # Caught immediately!
TypeError: Cannot assign value of type 'str' to variable 'balance' of type 'float'
```

### 4. **Documentation**

Types serve as documentation:

```python
def calculate_total(
    items: list[dict],
    tax_rate: float,
    discount: float = 0.0
) -> float:
    """Calculate total with tax and discount"""
    subtotal = sum(item["price"] for item in items)
    with_tax = subtotal * (1 + tax_rate)
    return with_tax * (1 - discount)
```

## Type Enforcement Rules

### Variables

```python
# Type is set at first annotation
x: int = 5

# All future assignments must match
x = 10     # OK
x = 15     # OK
x = "no"   # TypeError!

# Can reassign to same type only
x = 20     # OK
```

### Function Parameters

```python
def greet(name: str) -> str:
    return f"Hello, {name}!"

greet("Alice")   # OK
greet(123)       # TypeError at call time
```

### Mixed Dynamic and Static

```python
# Mix both in same program
typed_var: int = 10      # Static
dynamic_var = 10         # Dynamic

typed_var = 20           # OK
typed_var = "error"      # TypeError!

dynamic_var = 20         # OK
dynamic_var = "works"    # OK - can change type
```

## Supported Types

### Basic Types

```python
x: int = 42
y: float = 3.14
z: str = "hello"
flag: bool = True
nothing: None = None
```

### Collection Types

```python
# Lists
numbers: list = [1, 2, 3]
integers: list[int] = [1, 2, 3]

# Dictionaries
data: dict = {"key": "value"}
mapping: dict[str, int] = {"a": 1, "b": 2}

# Tuples
coords: tuple = (10, 20)
point: tuple[int, int] = (10, 20)

# Sets
tags: set = {1, 2, 3}
ids: set[int] = {1, 2, 3}
```

### Complex Types

```python
# Optional types
from typing import Optional
value: Optional[int] = None
value = 42  # OK

# Union types
from typing import Union
mixed: Union[int, str] = 10
mixed = "text"  # OK

# Custom classes
class Person:
    pass

person: Person = Person()
```

## Best Practices

### 1. **Start Dynamic, Add Types Gradually**

```python
# Phase 1: Prototype
def process(data):
    return data.upper()

# Phase 2: Add types for reliability
def process(data: str) -> str:
    return data.upper()
```

### 2. **Type Hot Paths for Performance**

```python
# Critical path - use types
def calculate_physics(
    mass: float,
    velocity: float,
    time: float
) -> float:
    return 0.5 * mass * velocity ** 2

# Helper function - dynamic is fine
def format_output(value):
    return f"Result: {value}"
```

### 3. **Type Public APIs**

```python
# Public API - fully typed
def public_function(
    input: str,
    count: int = 1
) -> list[str]:
    """Public API - fully documented with types"""
    return [input] * count

# Private helper - can be dynamic
def _internal_helper(data):
    return data.split()
```

### 4. **Use Types for Safety-Critical Code**

```python
# Financial calculations - must be precise
def calculate_interest(
    principal: float,
    rate: float,
    years: int
) -> float:
    return principal * (1 + rate) ** years

# User input - validate with types
def get_age(input: str) -> int:
    age: int = int(input)
    if age < 0 or age > 150:
        raise ValueError("Invalid age")
    return age
```

## Comparison with Python

| Feature | Python | Tauraro |
|---------|--------|---------|
| Type hints | Optional, not enforced | Optional, **enforced when used** |
| Runtime checking | No | Yes |
| Performance benefit | No | Yes (when compiled) |
| Type errors | Only with mypy | At runtime |
| Gradual typing | Yes | Yes |

## Comparison with TypeScript

| Feature | TypeScript | Tauraro |
|---------|-----------|---------|
| Type checking | Compile-time only | Runtime |
| Performance | No impact | Significant when compiled |
| Type erasure | Yes | No |
| Enforcement | Development only | Always |

## Advanced Topics

### Type Inference

```python
# Tauraro infers types from context
x = 10  # Inferred as dynamic int
x = "text"  # OK - no type annotation

y: int = 10  # Explicitly typed
y = "text"   # TypeError - annotation enforced
```

### Generic Types

```python
from typing import TypeVar, Generic

T = TypeVar('T')

def identity(x: T) -> T:
    return x

result = identity(42)      # Returns int
result = identity("text")  # Returns str
```

### Type Guards

```python
def process(value: Union[int, str]) -> str:
    if isinstance(value, int):
        return f"Number: {value}"
    else:
        return f"String: {value}"
```

## Next Steps

- [Type Annotations Guide](annotations.md)
- [Static Type Checking](static-checking.md)
- [Performance Optimization](../advanced/performance.md)
- [Compilation Guide](../compilation/c-backend.md)
