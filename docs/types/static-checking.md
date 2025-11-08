# Static Type Checking

Tauraro performs type checking during compilation when type annotations are present.

## Type Checking Modes

### VM Mode (Dynamic)
```bash
tauraro run script.py
# Types checked at runtime
```

### Compilation Mode (Static)
```bash
tauraro compile script.py -o program
# Types checked at compile time
```

## Type Safety

```python
def add_numbers(a: int, b: int) -> int:
    return a + b

# OK - types match
result = add_numbers(5, 10)

# Error at compile time - type mismatch
result = add_numbers("hello", "world")  # Compilation error!
```

## Gradual Typing

```python
# Mix typed and untyped code
def typed_function(x: int) -> int:
    return x * 2

def untyped_function(x):
    return x * 2  # Dynamic - no compile-time checking

# Both work, but typed_function is faster when compiled
```

## Next Steps

- [Type Annotations](annotations.md) - Adding type hints
- [Dynamic Typing](dynamic-typing.md) - Runtime typing
- [Hybrid Typing](hybrid-typing.md) - Best of both worlds
