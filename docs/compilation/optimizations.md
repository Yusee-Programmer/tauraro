# Optimization Guide

This guide explains how to optimize Tauraro programs for maximum performance through compilation, type annotations, and best practices.

## Compilation Optimization Levels

### Level 0: No Optimization (-O0)

```bash
tauraro compile script.py -o program -O0
```

- No optimizations
- Fast compilation
- Easy debugging
- Large binary size
- **Use for:** Development and debugging

### Level 1: Basic Optimization (-O1)

```bash
tauraro compile script.py -o program -O1
```

- Basic optimizations
- Constant folding
- Dead code elimination
- Moderate performance
- **Use for:** Testing with some optimization

### Level 2: Standard Optimization (-O2, Default)

```bash
tauraro compile script.py -o program -O2
# Or simply
tauraro compile script.py -o program
```

- Good performance
- Inlining of small functions
- Loop optimizations
- Type-based optimizations
- **Use for:** Most production code

### Level 3: Maximum Optimization (-O3)

```bash
tauraro compile script.py -o program -O3
```

- Maximum performance
- Aggressive inlining
- Vectorization where possible
- Longer compile time
- **Use for:** Performance-critical production code

## Type Annotations for Speed

### Impact of Type Annotations

```python
# Without types - uses boxed values
def calculate_sum(numbers):
    total = 0
    for n in numbers:
        total += n
    return total

# With types - uses native operations
def calculate_sum_typed(numbers: List[int]) -> int:
    total: int = 0
    for n in numbers:
        total += n
    return total
```

**Performance:**
- Untyped: ~1000ms
- Typed: ~10ms
- **Speedup: 100x!**

### Type Annotation Strategies

#### 1. Annotate Hot Paths

```python
def main():
    data = load_data()  # Can be untyped

    # Hot loop - must be typed!
    total: int = 0
    for i in range(len(data)):
        value: int = process_item(data[i])
        total += value

    save_result(total)  # Can be untyped
```

#### 2. Annotate Function Signatures

```python
# Good - enables optimization
def process(data: List[int], threshold: int) -> List[int]:
    return [x for x in data if x > threshold]

# Less optimal
def process(data, threshold):
    return [x for x in data if x > threshold]
```

#### 3. Use Specific Types

```python
# Good - specific types
def calculate(x: int, y: int) -> int:
    return x * y

# Less optimal - generic types
def calculate(x, y):
    return x * y

# Also good - for flexibility
from typing import Union
def calculate_flexible(x: Union[int, float], y: Union[int, float]) -> float:
    return x * y
```

## Loop Optimizations

### Typed Loops

```python
# Optimized loop with types
def sum_array(arr: List[int]) -> int:
    total: int = 0
    for i in range(len(arr)):
        total += arr[i]
    return total

# Even better - explicit indexing
def sum_array_indexed(arr: List[int]) -> int:
    total: int = 0
    n: int = len(arr)
    for i in range(n):
        total += arr[i]
    return total
```

### Loop Unrolling

```python
# Manual loop unrolling for critical code
def process_batch(data: List[float]) -> List[float]:
    result: List[float] = []
    i: int = 0
    n: int = len(data)

    # Process 4 items at a time
    while i + 4 <= n:
        result.append(data[i] * 2.0)
        result.append(data[i+1] * 2.0)
        result.append(data[i+2] * 2.0)
        result.append(data[i+3] * 2.0)
        i += 4

    # Handle remaining items
    while i < n:
        result.append(data[i] * 2.0)
        i += 1

    return result
```

## Function Inlining

### Small Functions

```python
# This will likely be inlined at -O2 and above
def square(x: int) -> int:
    return x * x

def calculate_squares(numbers: List[int]) -> List[int]:
    return [square(n) for n in numbers]

# Compiled as:
# return [n * n for n in numbers]  # square() inlined!
```

### Force Inline (If Available)

```python
@inline
def fast_multiply(a: int, b: int) -> int:
    return a * b
```

## Memory Optimization

### Choose Appropriate Memory Strategy

```python
# Default: Automatic reference counting
def normal_function():
    data = [1, 2, 3, 4, 5]
    return sum(data)

# Manual: Zero-overhead for critical sections
@manual_memory
def performance_critical():
    buffer = allocate(1024)
    # ... fast operations ...
    free(buffer)

# Arena: Fast bulk allocation
@arena_memory
def batch_processing(items):
    results = []
    for item in items:
        processed = transform(item)  # Arena-allocated
        results.append(processed)
    return results
```

## Data Structure Choices

### Lists vs Arrays

```python
from typing import List

# Good for mixed types
mixed: List = [1, "hello", 3.14]

# Better for homogeneous data
numbers: List[int] = [1, 2, 3, 4, 5]

# Best for numeric arrays (if available)
import array
nums = array.array('i', [1, 2, 3, 4, 5])  # Compact int array
```

### Dict vs List for Lookups

```python
# O(n) lookup - slow for large n
def find_in_list(items: List[str], target: str) -> bool:
    return target in items  # Linear search

# O(1) lookup - fast
def find_in_set(items_set: Set[str], target: str) -> bool:
    return target in items_set  # Hash lookup
```

## Avoiding Common Performance Pitfalls

### 1. Unnecessary List Copies

```python
# Bad - creates copy
def process_bad(data: List[int]) -> List[int]:
    result = data[:]  # Copy!
    result.sort()
    return result

# Good - modify in place
def process_good(data: List[int]) -> List[int]:
    data.sort()  # In-place
    return data
```

### 2. String Concatenation in Loops

```python
# Bad - O(n²) complexity
def build_string_bad(items: List[str]) -> str:
    result: str = ""
    for item in items:
        result += item  # Creates new string each time!
    return result

# Good - O(n) complexity
def build_string_good(items: List[str]) -> str:
    return "".join(items)  # Efficient
```

### 3. Redundant Function Calls

```python
# Bad - calls len() repeatedly
def process_bad(data: List[int]):
    for i in range(len(data)):  # len() called each iteration
        if i < len(data) - 1:    # len() called again!
            pass

# Good - cache length
def process_good(data: List[int]):
    n: int = len(data)  # Call once
    for i in range(n):
        if i < n - 1:
            pass
```

## Profile-Guided Optimization

### Profiling

```bash
# Profile VM execution
tauraro run --profile script.py

# Compile with profiling
tauraro compile script.py -o program --profile
./program

# View profiling data
tauraro profile-report program.prof
```

### Using Profile Data

```bash
# Step 1: Generate profile
./program --profile

# Step 2: Recompile with profile data
tauraro compile script.py -o program_optimized --use-profile program.prof
```

## Link-Time Optimization (LTO)

```bash
# Enable LTO for smaller binaries and better optimization
tauraro compile script.py -o program --lto

# LTO with maximum optimization
tauraro compile script.py -o program -O3 --lto
```

## Benchmark Your Code

### Timing Functions

```python
import time

def benchmark(func, *args, iterations: int = 1000):
    """Benchmark a function."""
    start = time.time()
    for _ in range(iterations):
        func(*args)
    end = time.time()

    elapsed = end - start
    per_call = elapsed / iterations

    print(f"{func.__name__}:")
    print(f"  Total: {elapsed:.4f}s")
    print(f"  Per call: {per_call*1000:.4f}ms")

# Usage
benchmark(my_function, arg1, arg2, iterations=10000)
```

### Comparing Implementations

```python
def compare_implementations(func1, func2, *args, iterations: int = 1000):
    """Compare two implementations."""
    import time

    # Benchmark func1
    start = time.time()
    for _ in range(iterations):
        func1(*args)
    time1 = time.time() - start

    # Benchmark func2
    start = time.time()
    for _ in range(iterations):
        func2(*args)
    time2 = time.time() - start

    print(f"{func1.__name__}: {time1:.4f}s")
    print(f"{func2.__name__}: {time2:.4f}s")
    print(f"Speedup: {time1/time2:.2f}x")
```

## Complete Optimization Example

### Before Optimization

```python
def process_data(filename):
    # Load data
    with open(filename) as f:
        data = f.readlines()

    # Process
    results = []
    for line in data:
        parts = line.strip().split(',')
        if len(parts) > 2:
            value = int(parts[2])
            if value > 100:
                results.append(value)

    return results
```

### After Optimization

```python
from typing import List

def process_data_optimized(filename: str) -> List[int]:
    """Optimized version with type annotations."""
    results: List[int] = []

    with open(filename, 'r') as f:
        for line in f:  # Stream instead of readlines()
            parts = line.strip().split(',')
            if len(parts) > 2:
                value: int = int(parts[2])
                if value > 100:
                    results.append(value)

    return results
```

### Performance Improvements

| Optimization | Impact |
|--------------|--------|
| Type annotations | 10-50x faster |
| Streaming file read | 2-3x less memory |
| Typed variables | Native operations |
| -O3 compilation | 1.5-2x faster |

## Best Practices Summary

1. **Add type annotations** - Especially for hot paths
2. **Use appropriate optimization level** - -O2 for most code, -O3 for critical code
3. **Profile before optimizing** - Find the actual bottlenecks
4. **Choose right data structures** - Dict/Set for lookups, List for sequences
5. **Minimize allocations** - Reuse buffers when possible
6. **Cache expensive computations** - Store results of repeated calls
7. **Use arena memory for batches** - Fast bulk allocation
8. **Enable LTO** - Better cross-function optimization

## Next Steps

- [C Backend](c-backend.md) - Understanding compilation
- [Memory Management](../advanced/memory.md) - Memory strategies
- [Performance Tuning](../advanced/performance.md) - Advanced techniques
- [Profiling Tools](#) - Finding bottlenecks
