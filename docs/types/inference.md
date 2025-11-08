# Type Inference

Tauraro can automatically infer types in many cases, reducing the need for explicit annotations while still enabling optimizations.

## Automatic Type Inference

```python
# Type inferred from assignment
x = 10  # Inferred as int
y = 3.14  # Inferred as float
name = "Alice"  # Inferred as str

# Type inferred from operations
result = x + y  # Inferred as float
doubled = x * 2  # Inferred as int
```

## Function Return Type Inference

```python
def add(a: int, b: int):
    return a + b  # Return type inferred as int

def process(data: List[int]):
    return [x * 2 for x in data]  # Inferred as List[int]
```

## Limitations

```python
# Cannot infer - type changes
x = 10
x = "hello"  # Type changed - uses dynamic typing

# Cannot infer across function boundaries
def mystery_function(data):
    # 'data' type unknown without annotation
    return data * 2
```

## Best Practice: Annotate Public APIs

```python
# Public API - always annotate
def public_function(x: int, y: int) -> int:
    return helper(x, y)

# Private helper - can rely on inference
def helper(a, b):
    return a + b  # Types inferred from caller
```

## Next Steps

- [Type Annotations](annotations.md) - Explicit type hints
- [Hybrid Typing](hybrid-typing.md) - Mix typed/untyped code
- [Optimization](../compilation/optimizations.md) - Performance tuning
