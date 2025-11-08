# Best Practices

Guidelines for writing effective Tauraro code.

## Type Annotations

### DO: Annotate Performance-Critical Code

```python
# Good - fast when compiled
def process_data(items: List[int]) -> int:
    total: int = 0
    for item in items:
        total += item
    return total
```

### DON'T: Skip Types in Hot Loops

```python
# Slower when compiled
def process_data(items):
    total = 0
    for item in items:
        total += item
    return total
```

## Error Handling

### DO: Use Try/Except

```python
# Good - handles errors gracefully
def safe_divide(a: float, b: float) -> float:
    try:
        return a / b
    except ZeroDivisionError:
        print("Error: Division by zero")
        return 0.0
```

### DON'T: Ignore Errors

```python
# Bad - crashes on error
def unsafe_divide(a, b):
    return a / b  # Crashes if b == 0
```

## Resource Management

### DO: Use Context Managers

```python
# Good - automatically closes file
with open("file.txt", "r") as f:
    content = f.read()
```

### DON'T: Forget to Close Resources

```python
# Bad - file might not close
f = open("file.txt", "r")
content = f.read()
# f.close() might not be called if error occurs
```

## Performance

### DO: Use Appropriate Data Structures

```python
# Good - O(1) lookup
user_set = {"alice", "bob", "charlie"}
if "alice" in user_set:  # Fast
    pass

# Bad - O(n) lookup
user_list = ["alice", "bob", "charlie"]
if "alice" in user_list:  # Slow for large lists
    pass
```

### DO: Profile Before Optimizing

```python
import time

def benchmark(func, *args):
    start = time.time()
    result = func(*args)
    elapsed = time.time() - start
    print(f"{func.__name__}: {elapsed:.4f}s")
    return result
```

## Code Organization

### DO: Use Meaningful Names

```python
# Good
def calculate_total_price(items: List[Item]) -> float:
    return sum(item.price for item in items)

# Bad
def calc(x):
    return sum(i.p for i in x)
```

### DO: Keep Functions Small

```python
# Good - single responsibility
def validate_email(email: str) -> bool:
    return "@" in email and "." in email

def send_welcome_email(email: str):
    if validate_email(email):
        send_email(email, "Welcome!")
```

## Concurrency

### DO: Use Async for I/O-Bound Tasks

```python
import asyncio
import httpx

async def fetch_all(urls):
    tasks = [httpx.get(url) for url in urls]
    return await asyncio.gather(*tasks)
```

### DO: Use Multiprocessing for CPU-Bound Tasks

```python
import multiprocessing

def cpu_intensive(n):
    return sum(i**2 for i in range(n))

with multiprocessing.Pool() as pool:
    results = pool.map(cpu_intensive, [1000000, 2000000])
```

## Memory Management

### DO: Choose Appropriate Strategy

```python
# Default: automatic (for most code)
def normal_function():
    data = load_data()
    return process(data)

# Manual: for performance-critical sections
@manual_memory
def performance_critical():
    buffer = allocate(1024)
    try:
        result = process(buffer)
    finally:
        free(buffer)
    return result

# Arena: for batch processing
@arena_memory
def batch_process(items):
    results = []
    for item in items:
        results.append(transform(item))
    return results
```

## Testing

### DO: Write Tests

```python
def add(a: int, b: int) -> int:
    return a + b

# Test
assert add(2, 3) == 5
assert add(-1, 1) == 0
assert add(0, 0) == 0
```

## Documentation

### DO: Write Docstrings

```python
def calculate_area(width: float, height: float) -> float:
    """
    Calculate the area of a rectangle.

    Args:
        width: Width of the rectangle
        height: Height of the rectangle

    Returns:
        Area of the rectangle

    Example:
        >>> calculate_area(10, 5)
        50.0
    """
    return width * height
```

## Summary

1. **Use type annotations** for performance-critical code
2. **Handle errors** gracefully with try/except
3. **Use context managers** for resource management
4. **Choose right data structures** for the task
5. **Profile before optimizing** - measure first
6. **Write meaningful names** - code is read more than written
7. **Use async for I/O**, multiprocessing for CPU tasks
8. **Write tests** - verify correctness
9. **Document your code** - help future readers

## Next Steps

- [Design Patterns](patterns.md) - Common patterns
- [Performance Guide](../advanced/performance.md) - Optimization
- [Examples Index](index.md) - Code examples
