# Performance Optimization

This guide covers performance optimization techniques in Tauraro, including both interpreter and C compilation optimizations.

## Type Annotations for Performance

Tauraro's unique feature: **Type annotations enable C compilation optimizations**.

### Static Typing Performance Benefits

When you use type annotations, Tauraro can compile to optimized C code with primitive types instead of boxed values.

```python
# WITHOUT type annotations - uses boxed values (slower)
def calculate_sum(n):
    total = 0
    for i in range(n):
        total += i
    return total

# WITH type annotations - compiles to primitives (5-10x faster)
def calculate_sum(n: int) -> int:
    total: int = 0
    for i in range(n):
        total += i
    return total
```

**Performance Difference:**
- Untyped: Uses `tauraro_value_t*` (heap-allocated boxed values)
- Typed: Uses `int64_t` (stack-allocated primitives)
- Result: **5-10x faster execution** when compiled to C

### Supported Type Optimizations

```python
# Optimized primitive types
x: int = 42              # → int64_t
y: float = 3.14          # → double
flag: bool = True        # → bool (C99)
name: str = "Alice"      # → char* (optimized string)

# Generic types (remain boxed)
numbers: list = [1, 2, 3]
data: dict = {"key": "value"}
items: tuple = (1, 2, 3)
unique: set = {1, 2, 3}
```

### When to Use Type Annotations

**Always annotate:**
- Loop variables in performance-critical loops
- Function parameters and return types
- Mathematical computations
- Counter variables

**Example: Optimized Matrix Multiplication**

```python
def matmul(A: list, B: list) -> list:
    """Matrix multiplication with type hints for performance."""
    n: int = len(A)
    m: int = len(B[0])
    p: int = len(B)

    result = [[0 for _ in range(m)] for _ in range(n)]

    for i in range(n):
        for j in range(m):
            sum_val: float = 0.0
            for k in range(p):
                sum_val += A[i][k] * B[k][j]
            result[i][j] = sum_val

    return result
```

## Data Structure Performance

### List vs Tuple

```python
# Lists - mutable, slightly slower
numbers = [1, 2, 3, 4, 5]
numbers.append(6)        # O(1) amortized

# Tuples - immutable, faster access
coordinates = (10, 20)   # Faster creation and access
```

**Performance Tips:**
- Use tuples for immutable data (faster)
- Use lists only when you need to modify
- Tuple unpacking is very fast

### Dictionary Performance

```python
# Fast O(1) average lookup
data = {"key": "value"}
result = data["key"]      # O(1)

# Faster than list search
numbers = [1, 2, 3, 4, 5]
if 3 in numbers:          # O(n)
    pass

number_set = {1, 2, 3, 4, 5}
if 3 in number_set:       # O(1)
    pass
```

**Performance Tips:**
- Use sets for membership testing
- Use dicts for lookups instead of lists
- Pre-size dicts when possible

### Set Operations

```python
# Very fast membership testing
items = {1, 2, 3, 4, 5}
if 3 in items:            # O(1)
    pass

# Fast set operations
a = {1, 2, 3}
b = {3, 4, 5}
union = a | b             # Fast
intersection = a & b      # Fast
```

## Algorithm Optimization

### List Comprehensions

List comprehensions are **significantly faster** than equivalent loops.

```python
# SLOW: Regular loop
result = []
for i in range(1000):
    result.append(i * 2)

# FAST: List comprehension (2-3x faster)
result = [i * 2 for i in range(1000)]
```

### Generator Expressions

Use generators for large sequences to save memory.

```python
# Memory-heavy: Creates full list
squares = [x**2 for x in range(1000000)]

# Memory-efficient: Lazy evaluation
squares = (x**2 for x in range(1000000))

# Use as needed
for square in squares:
    if square > 100:
        break
```

### Built-in Functions

Built-in functions are implemented in C and are much faster.

```python
# SLOW: Manual sum
total = 0
for x in numbers:
    total += x

# FAST: Built-in sum (10x faster)
total = sum(numbers)

# SLOW: Manual max
max_val = numbers[0]
for x in numbers[1:]:
    if x > max_val:
        max_val = x

# FAST: Built-in max
max_val = max(numbers)
```

### String Operations

```python
# SLOW: String concatenation in loop
result = ""
for s in strings:
    result += s           # Creates new string each time

# FAST: Use join (10-100x faster)
result = "".join(strings)

# SLOW: Building strings
s = ""
for i in range(1000):
    s += str(i) + ","

# FAST: Use list and join
parts = [str(i) for i in range(1000)]
s = ",".join(parts)
```

## Loop Optimization

### Minimize Work Inside Loops

```python
# SLOW: Repeated function calls
for i in range(len(items)):
    process(items[i])

# FAST: Cache length
n = len(items)
for i in range(n):
    process(items[i])

# FASTER: Direct iteration
for item in items:
    process(item)
```

### Loop Unrolling

```python
# Standard loop
for i in range(0, n):
    process(data[i])

# Unrolled loop (faster for small loops)
i = 0
while i < n - 3:
    process(data[i])
    process(data[i+1])
    process(data[i+2])
    process(data[i+3])
    i += 4
while i < n:
    process(data[i])
    i += 1
```

### Early Exit

```python
# Check conditions early
for item in large_list:
    if condition:
        result = item
        break           # Exit as soon as found

# Use any() for existence checks
if any(item > 100 for item in numbers):
    print("Found large number")
```

## Function Call Optimization

### Avoid Function Calls in Tight Loops

```python
# SLOW: Function call overhead
def square(x):
    return x * x

result = [square(i) for i in range(10000)]

# FAST: Inline operation
result = [i * i for i in range(10000)]

# FAST: Use lambda if needed
result = list(map(lambda x: x * x, range(10000)))
```

### Local Variable Access

Local variables are faster than global variables.

```python
# SLOW: Global access
global_var = 100

def process():
    for i in range(10000):
        x = global_var * i

# FAST: Local access
def process():
    local_var = global_var
    for i in range(10000):
        x = local_var * i
```

## Memory Optimization

### Use Generators for Large Sequences

```python
# Memory-heavy
def get_numbers(n):
    return [i for i in range(n)]

numbers = get_numbers(1000000)  # Uses ~8MB

# Memory-efficient
def get_numbers(n):
    for i in range(n):
        yield i

numbers = get_numbers(1000000)  # Uses ~80 bytes
```

### Reuse Objects

```python
# Creates many temporary objects
result = []
for i in range(10000):
    temp = SomeObject()
    result.append(temp.process())

# Reuse single object
obj = SomeObject()
result = []
for i in range(10000):
    result.append(obj.process())
```

## C Compilation for Maximum Performance

### Compile to C

```bash
# Compile Tauraro script to C
tauraro compile script.py

# Run compiled executable
gcc output.c -o program
./program
```

### Optimized vs Interpreted

**Interpreted Mode:**
- Good for development
- Easy debugging
- Slower execution

**Compiled Mode:**
- 10-50x faster execution
- Type-annotated code gets primitive types
- Best for production

### Compilation Best Practices

```python
# BEST: Fully typed for optimal C compilation
def fibonacci(n: int) -> int:
    if n <= 1:
        return n
    a: int = 0
    b: int = 1
    for i in range(2, n + 1):
        a, b = b, a + b
    return b

# GOOD: Partially typed
def fibonacci(n: int):
    if n <= 1:
        return n
    a = 0
    b = 1
    for i in range(2, n + 1):
        a, b = b, a + b
    return b

# OK: No types (falls back to boxed values)
def fibonacci(n):
    if n <= 1:
        return n
    a = 0
    b = 1
    for i in range(2, n + 1):
        a, b = b, a + b
    return b
```

## Profiling and Benchmarking

### Timing Code

```python
import time

# Simple timing
start = time.time()
result = expensive_function()
end = time.time()
print(f"Took {end - start:.4f} seconds")

# Context manager approach
class Timer:
    def __enter__(self):
        self.start = time.time()
        return self

    def __exit__(self, *args):
        self.end = time.time()
        print(f"Elapsed: {self.end - self.start:.4f}s")

with Timer():
    expensive_function()
```

### Comparative Benchmarking

```python
import time

def benchmark(func, *args, iterations=1000):
    start = time.time()
    for _ in range(iterations):
        func(*args)
    end = time.time()
    return (end - start) / iterations

# Compare approaches
time1 = benchmark(approach1, data)
time2 = benchmark(approach2, data)
print(f"Approach 1: {time1:.6f}s")
print(f"Approach 2: {time2:.6f}s")
print(f"Speedup: {time1/time2:.2f}x")
```

## Performance Checklist

### General Optimizations

- [ ] Use type annotations for critical code paths
- [ ] Use list comprehensions over loops
- [ ] Use built-in functions (sum, max, min, etc.)
- [ ] Use generators for large sequences
- [ ] Cache repeated calculations
- [ ] Use appropriate data structures (dict/set for lookups)
- [ ] Minimize work inside loops
- [ ] Use early exits with break
- [ ] Join strings instead of concatenating
- [ ] Reuse objects when possible

### Compilation Optimizations

- [ ] Add type annotations to all variables in hot paths
- [ ] Annotate function parameters and returns
- [ ] Use int, float, bool, str for primitives
- [ ] Compile to C for production
- [ ] Profile before and after compilation

### Memory Optimizations

- [ ] Use generators for large datasets
- [ ] Use tuples instead of lists when immutable
- [ ] Clear large data structures when done
- [ ] Avoid global variables
- [ ] Use __slots__ for classes (if supported)

## Performance Anti-Patterns

### What NOT to Do

```python
# DON'T: Modify list while iterating
for item in items:
    items.remove(item)  # BAD!

# DO: Create new list or iterate over copy
items = [item for item in items if keep(item)]

# DON'T: Use list for lookups
if item in large_list:  # O(n)
    pass

# DO: Use set or dict
if item in large_set:   # O(1)
    pass

# DON'T: Concatenate strings in loops
s = ""
for i in range(1000):
    s += str(i)

# DO: Use join
s = "".join(str(i) for i in range(1000))
```

## Real-World Example

### Before Optimization

```python
def process_data(data):
    result = []
    for i in range(len(data)):
        if data[i] > 0:
            value = data[i] * 2
            result.append(value)
    return result
```

### After Optimization

```python
def process_data(data: list) -> list:
    """Optimized with type hints and comprehension."""
    return [x * 2 for x in data if x > 0]
```

**Performance Improvement:**
- Faster list comprehension
- Type hints enable C compilation
- Result: **10-20x faster** when compiled

## Next Steps

- [C Backend Compilation](../compilation/c-backend.md)
- [Memory Management](memory.md)
- [Type System](../types/hybrid-typing.md)
- [Profiling Tools](profiling.md)
