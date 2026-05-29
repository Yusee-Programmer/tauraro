# 07 — Collections: List[T] and Dict

---

## Overview

Tauraro provides two built-in collection types:

| Type | C backing | Use when |
|------|-----------|----------|
| `List[T]` | `List_T*` struct with contiguous array | Ordered sequence of same-typed elements |
| `Dict` | `TrMap*` hash map | String-keyed associative lookup |

Both are heap-allocated. The compiler tracks ownership and injects `free()` at scope exit. You never call `free()` on them manually.

---

## List[T] — Typed Dynamic Array

### What Is List[T]

`List[T]` is a growable array. Elements are stored **contiguously** in memory — the same layout as a C array. This means:
- Random access is O(1)
- Appending is amortized O(1) (doubles capacity on growth)
- Iteration is cache-friendly
- No boxing, no indirection, no garbage collector

### Supported Element Types

| List type | C struct | Element C type |
|-----------|----------|----------------|
| `List[int]` | `List_i64` | `long long` |
| `List[i32]` | `List_i32` | `int` |
| `List[float]` | `List_f64` | `double` |
| `List[str]` | `List_str` | `char*` |
| `List[bool]` | `List_bool` | `bool` |
| `List[char]` | `List_char` | `char` |
| `List[u8]` | `List_u8` | `uint8_t` |
| `List[u32]` | `List_u32` | `unsigned int` |
| `List[i8]` | `List_i8` | `int8_t` |
| `List[MyClass]` | `List_ptr` | `MyClass*` |
| `List[MyEnum]` | `List_MyEnum` | `MyEnum` (by value) |

### Creating Lists

```python
# Empty list with explicit type
mut scores: List[int] = []

# List literal — type inferred from elements
mut primes = [2, 3, 5, 7, 11]

# List of strings
mut names = ["Alice", "Bob", "Charlie"]

# List of floats
mut temps: List[float] = [98.6, 97.1, 99.2]

# List of booleans
mut flags: List[bool] = [true, false, true, false]

# List of class instances
mut points: List[Point] = []
```

**Compiler rule:** All elements in a list literal must be the same type. A mixed-type literal is a compiler error.

**Common error — wrong type for empty list:**
```python
mut data = []         # ERROR: cannot infer type of empty list
mut data: List[int] = []  # OK: type annotation required for empty lists
```

### Appending Elements

```python
mut items: List[int] = []
items.append(10)
items.append(20)
items.append(30)
# items is now [10, 20, 30]
```

`.append(v)` adds `v` at the end. If the list is at capacity, it doubles its internal buffer (amortized O(1)).

**How `.append` compiles:**
```c
List_i64_append(items, 10LL);
```

### Reading Elements

```python
mut first = items[0]      # 10
mut second = items[1]     # 20
mut last = items[len(items) - 1]   # last element
```

**Compiler rule:** There is no automatic bounds checking. Accessing `items[i]` where `i >= len(items)` is undefined behavior. Check bounds manually when the index is not statically known:

```python
def safe_get(items: List[int], i: int) -> int:
    if i < 0 or i >= len(items): return -1
    return items[i]
```

### Modifying Elements

```python
items[0] = 99      # replace element at index 0
items[1] += 5      # arithmetic assignment on list element
```

### Getting the Length

```python
mut n = len(items)    # number of elements
```

`len(items)` compiles to `((long long)items->len)`. O(1) — a simple field read.

### Removing the Last Element

```python
mut last = items.pop()    # removes and returns the last element
```

`.pop()` decrements the length and returns the last value. Does not shrink the allocation.

### Iterating

```python
# For loop (most idiomatic):
for x in items:
    print(x)

# With index:
for i, x in enumerate(items):
    print(f"  [{i}] = {x}")

# While loop (when you need full control):
mut i = 0
while i < len(items):
    print(f"  items[{i}] = {items[i]}")
    i = i + 1
```

All three compile to equivalent C loops with no allocation overhead.

### List Comprehension

```python
# Build a new list by transforming another:
mut squares: List[int] = [x * x for x in numbers]

# With filter:
mut evens: List[int] = [x for x in numbers if x % 2 == 0]
```

**How comprehensions compile:** To a C loop that builds a `List_T*` directly. No intermediate allocations.

### Common List Patterns

**Building a list from a range:**
```python
def range_list(n: int) -> List[int]:
    mut result: List[int] = []
    for i in range(n):
        result.append(i)
    return result
```

**Filtering:**
```python
def filter_positive(nums: List[int]) -> List[int]:
    mut result: List[int] = []
    for x in nums:
        if x > 0: result.append(x)
    return result
```

**Summing:**
```python
def sum_list(items: List[int]) -> int:
    mut total = 0
    for x in items:
        total = total + x
    return total
```

**Searching:**
```python
def index_of(items: List[int], target: int) -> int:
    mut i = 0
    while i < len(items):
        if items[i] == target: return i
        i = i + 1
    return -1
```

**Reversing:**
```python
def reverse(items: List[int]) -> List[int]:
    mut result: List[int] = []
    mut i = len(items) - 1
    while i >= 0:
        result.append(items[i])
        i = i - 1
    return result
```

**Sorting (insertion sort):**
```python
def sort_asc(items: List[int]) -> void:
    mut i = 1
    while i < len(items):
        mut key = items[i]
        mut j = i - 1
        while j >= 0 and items[j] > key:
            items[j + 1] = items[j]
            j = j - 1
        items[j + 1] = key
        i = i + 1
```

---

## Dict — String-Keyed Hash Map

### What Is Dict

`Dict` is a hash map with **string keys** and dynamically-typed values. Values are stored as `void*` pointers internally, which means:
- Values are untyped at compile time
- Works well for string values or pointer values
- Numeric values must be cast to/from pointer size

```python
# Create with literal:
mut config = {"host": "localhost", "port": "8080", "debug": "true"}

# Empty Dict:
mut store: Dict = {}
```

### Dict Operations

```python
# Write a value:
config.set("timeout", "30")

# Read a value:
mut host = config.get("host")     # returns the value (as str for string values)

# Check if key exists:
if config.has("debug"):
    print("debug mode on")

# Get size:
mut n = len(config)

# Delete (via set to none — not yet native):
```

### Hausa Aliases

```python
config.makomashi("key")   # same as config.get("key")
config.kafa("key", val)   # same as config.set("key", val)
config.akwai("key")       # same as config.has("key")
```

### Dict Patterns

**Configuration map:**
```python
def get_config() -> Dict:
    return {
        "host":    "localhost",
        "port":    "5432",
        "dbname":  "myapp",
        "user":    "admin"
    }

def main():
    mut cfg = get_config()
    if cfg.has("host"):
        print(f"connecting to {cfg.get("host")}:{cfg.get("port")}")
```

**Counting occurrences (using string values):**
```python
def count_words(words: List[str]) -> Dict:
    mut counts: Dict = {}
    for word in words:
        if counts.has(word):
            mut n = int(counts.get(word))
            counts.set(word, str(n + 1))
        else:
            counts.set(word, "1")
    return counts
```

### Dict Limitations

- Keys must be strings — no integer keys, no custom hash keys
- Values are `void*` — no compile-time type safety on values
- No iteration over keys (no `for k, v in dict:` yet)

For integer-keyed maps, use an array with a computed offset. For typed maps with known structure, use a class.

---

## Ownership and Collections

Both `List[T]` and `Dict` are heap-allocated. The compiler marks them as `Own` and injects `free()` at scope exit:

```python
def demo() -> void:
    mut items: List[int] = []    # Own — heap allocated
    items.append(1)
    items.append(2)
    # scope ends: compiler injects List_i64_free(items)
```

**Returning a collection transfers ownership:**
```python
def build_list() -> List[int]:
    mut result: List[int] = []
    result.append(42)
    return result     # ownership transferred to caller — no free injected here

def main():
    mut nums = build_list()    # nums owns the list
    print(nums[0])
    # scope ends: List_i64_free(nums) injected
```

**Passing a collection to a function borrows it:**
```python
def print_all(items: List[int]) -> void:    # borrows items
    for x in items: print(x)
    # no free injected — caller still owns items

def main():
    mut nums = [1, 2, 3]
    print_all(nums)        # borrow
    print(nums[0])         # still valid — not moved
    # scope ends: List_i64_free(nums) injected
```

---

## Common Errors

### Empty list without type annotation

```python
mut data = []
# ERROR: cannot infer element type from empty list literal
```
**Fix:** `mut data: List[int] = []`

### Wrong element type

```python
mut nums: List[int] = [1, 2, "three"]
# ERROR [T-2]: element "three" has type str, expected int
```
**Fix:** Use a consistent element type.

### List[int] assigned from List[float]

```python
mut ints: List[int] = []
mut floats: List[float] = [1.0, 2.0]
ints = floats    # ERROR [T-2]: incompatible List types
```
**Fix:** Convert explicitly: `for x in floats: ints.append(x as int)`

### Dict missing key

```python
mut cfg: Dict = {}
mut val = cfg.get("host")    # undefined behavior if "host" not set
```
**Fix:** Always check first: `if cfg.has("host"): val = cfg.get("host")`

---

Next: [Classes & Extend →](08_classes.md)
