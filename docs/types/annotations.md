# Type Annotations

Type annotations in Tauraro are optional but provide significant performance benefits when compiling to C.

## Basic Annotations

```python
# Variables
age: int = 30
height: float = 1.75
name: str = "Alice"
is_active: bool = True

# Functions
def add(a: int, b: int) -> int:
    return a + b

def greet(name: str) -> str:
    return f"Hello, {name}!"
```

## Collection Types

```python
from typing import List, Dict, Tuple, Set, Optional

# List of integers
numbers: List[int] = [1, 2, 3, 4, 5]

# Dictionary
scores: Dict[str, int] = {"Alice": 95, "Bob": 87}

# Tuple
point: Tuple[float, float] = (3.14, 2.71)

# Set
tags: Set[str] = {"python", "rust"}

# Optional (can be None)
middle_name: Optional[str] = None
```

## Function Annotations

```python
def calculate_area(width: float, height: float) -> float:
    return width * height

def process_items(items: List[int]) -> List[int]:
    return [x * 2 for x in items]

def find_user(user_id: int) -> Optional[dict]:
    # May return None
    return user_db.get(user_id)
```

## Performance Impact

**Without types:**
```python
def sum_numbers(numbers):
    total = 0
    for n in numbers:
        total += n
    return total
# Compiled: ~100ms
```

**With types:**
```python
def sum_numbers(numbers: List[int]) -> int:
    total: int = 0
    for n in numbers:
        total += n
    return total
# Compiled: ~1ms (100x faster!)
```

## Next Steps

- [Hybrid Typing](hybrid-typing.md) - Mix typed and untyped code
- [Type Inference](inference.md) - Automatic type detection
- [Variables](../language/variables.md) - Variable declarations
