# 07 — Collections: List, Dict, Set, and Tuples

Tauraro provides four built-in collection types. All heap-allocated collections are owned by the compiler's ownership system — they are allocated automatically when created and freed automatically when they leave scope. You never call `free()` on a collection manually.

| Type | When to use |
|------|-------------|
| `List[T]` | Ordered sequence of same-typed elements |
| `Dict` / `Dict[K, V]` | Key-to-value lookup by string or numeric key |
| `Set[T]` | Unordered collection of unique values |
| `(a, b, c)` | Fixed-size group of heterogeneous values, function multi-return |

---

## Table of Contents

1. [List\[T\] — Typed Dynamic Array](#listt--typed-dynamic-array)
2. [Dict — Hash Map](#dict--hash-map)
3. [Set\[T\] — Unique Collection](#sett--unique-collection)
4. [Tuples](#tuples)
5. [List Comprehensions](#list-comprehensions)
6. [Built-in Iteration Helpers](#built-in-iteration-helpers)
7. [Ownership and Collections](#ownership-and-collections)
8. [Common Collection Errors](#common-collection-errors)

---

## List[T] — Typed Dynamic Array

### When to use

Use `List[T]` whenever you need an ordered, indexable, growable sequence of same-typed values. It is the go-to collection for most tasks: storing results, accumulating items in a loop, passing sequences to functions.

`List[T]` maps directly to a C array with a length and capacity field. There is no boxing, no GC, no type erasure.

### How it works

**Creating lists:**

```python
# Empty list — type annotation required when the list starts empty
mut scores: List[int] = []

# List literal — type inferred from elements
mut primes = [2, 3, 5, 7, 11]

# Other element types
mut names:  List[str]   = ["Alice", "Bob", "Charlie"]
mut temps:  List[float] = [98.6, 97.1, 99.2]
mut flags:  List[bool]  = [true, false, true, false]

# List of class instances
mut points: List[Point] = []
```

**Memory layout:** Elements are stored contiguously in memory — the same layout as a C array. Random access is O(1), iteration is cache-friendly, and appending is amortized O(1) (capacity doubles on growth).

**Core operations:**

```python
mut items: List[int] = []

# Append (amortized O(1)):
items.append(10)
items.append(20)
items.append(30)

# Read by index (O(1)):
mut first  = items[0]              # 10
mut last   = items[len(items) - 1] # 30  (negative indexing not supported)

# Modify in place:
items[0] = 99
items[1] = items[1] + 5

# Length (O(1)):
mut n = len(items)

# Remove and return last element (O(1)):
mut popped = items.pop()

# Remove at index (O(n)):
items.remove(0)

# Check membership (O(n)):
mut found = items.contains(20)

# Sort in place (ascending), or descending:
items.sort()
items.sort_desc()

# Reverse in place:
items.reverse()

# Shallow copy with its own backing buffer:
mut copy = items.clone()

# A new reversed list (original unchanged):
mut rev = items.reversed()

# Aggregates (List[int] / List[float]):
mut total = items.sum()
mut lo    = items.min_val()
mut hi    = items.max_val()

# First / last element, and linear search:
mut head = items.first()
mut tail = items.last()
mut at   = items.index_of(20)   # index, or -1 if not present
```

**Iterating:**

```python
# For loop — most idiomatic:
for x in items:
    print(x)

# With index via enumerate:
for i, x in enumerate(items):
    print(f"  [{i}] = {x}")

# While loop when you need full index control:
mut i = 0
while i < len(items):
    print(f"  items[{i}] = {items[i]}")
    i = i + 1
```

All three compile to equivalent C loops with no allocation overhead.

**Common patterns:**

```python
# Build from range:
def range_list(n: int) -> List[int]:
    mut result: List[int] = []
    for i in range(n):
        result.append(i)
    return result

# Filter:
def positives(nums: List[int]) -> List[int]:
    mut result: List[int] = []
    for x in nums:
        if x > 0: result.append(x)
    return result

# Sum:
def sum_list(items: List[int]) -> int:
    mut total = 0
    for x in items: total = total + x
    return total

# Linear search:
def index_of(items: List[int], target: int) -> int:
    mut i = 0
    while i < len(items):
        if items[i] == target: return i
        i = i + 1
    return -1

# Manual sort (insertion sort):
def sort_asc(items: List[int]) -> void:
    mut i = 1
    while i < len(items):
        mut key = items[i]
        mut j   = i - 1
        while j >= 0 and items[j] > key:
            items[j + 1] = items[j]
            j = j - 1
        items[j + 1] = key
        i = i + 1
```

### Common Mistakes

**Empty list without type annotation:**
```python
mut data = []    # ERROR: cannot infer element type from empty list literal
```
Fix: `mut data: List[int] = []`

**Mixed element types:**
```python
mut nums: List[int] = [1, 2, "three"]    # ERROR [T-2]: "three" has type str, expected int
```
Fix: All elements must be the same type.

**Incompatible List assignment:**
```python
mut ints:   List[int]   = []
mut floats: List[float] = [1.0, 2.0]
ints = floats    # ERROR [T-2]: incompatible List types
```
Fix: Convert explicitly: `for x in floats: ints.append(x as int)`

**Out-of-bounds access:**
```python
mut items = [10, 20]
mut x     = items[5]    # runtime panic: "Index 5 out of bounds (length 2)"
```
List indexing and `.get(i)` are bounds-checked at runtime — an out-of-range
access aborts with a clear message rather than reading invalid memory. Validate
the index first when it comes from user input: `if i < len(items): x = items[i]`

**Using negative indices:**
```python
mut last = items[-1]    # NOT supported — undefined behavior
```
Fix: `mut last = items[len(items) - 1]`

### Best Practices

- When the list starts empty, always provide the type annotation: `mut data: List[int] = []`.
- Prefer `for x in items:` over index-based while loops unless you specifically need the index.
- Use `enumerate(items)` when you need both the index and the value — it is cleaner than a manual counter.
- For index-based access from user input or computed values, always validate: `if i >= 0 and i < len(items):`.
- Use list comprehensions for simple transforms and filters — they are concise and produce no intermediate allocations.

---

## Dict — Hash Map

### When to use

Use `Dict` for key-to-value lookups when you need to retrieve a value by name at runtime. Common uses: configuration maps, frequency counts, label lookup tables, caches keyed by identifier.

Use the untyped `Dict` when all values are strings. Use the typed `Dict[K, V]` when you need typed values or non-string keys.

### How it works

**Untyped `Dict` (string keys, string values):**

```python
# Dict literal:
mut config = {"host": "localhost", "port": "8080", "debug": "true"}

# Empty Dict:
mut store: Dict = {}

# Write:
config.set("timeout", "30")

# Read:
mut host = config.get("host")    # returns str

# Check existence:
if config.has("debug"):
    print("debug mode on")

# Size:
mut n = len(config)
```

**Typed `Dict[K, V]` (typed keys and values):**

```python
# String keys, int values:
mut scores: Dict[str, int] = {}
scores.set("alice", 95)
scores.set("bob",   87)
mut a = scores.get("alice")    # int — no cast needed

# Int keys, string values:
mut http_status: Dict[int, str] = {}
http_status.set(200, "OK")
http_status.set(404, "Not Found")
http_status.set(500, "Internal Server Error")
mut msg = http_status.get(404)    # "Not Found"
```

Supported key types: `str`, `int`, `i64`, `i32`, `usize`.

**Iterating with `.items()`:**

```python
for code, msg in http_status.items():
    print(f"  {code}: {msg}")

for word, count in scores.items():
    print(f"  {word}: {count}")
```

Iteration order follows the internal hash table order — not insertion order.

**Keys and values as lists:**

```python
mut ks = scores.keys()     # List[str]
mut vs = scores.values()   # List[int]
```

**Common patterns:**

```python
# Configuration map:
def get_config() -> Dict:
    return {
        "host":   "localhost",
        "port":   "5432",
        "dbname": "myapp",
        "user":   "admin"
    }

def main() -> void:
    mut cfg = get_config()
    if cfg.has("host"):
        print(f"connecting to {cfg.get("host")}:{cfg.get("port")}")

# Word frequency count (untyped Dict, values stored as strings):
def count_words(words: List[str]) -> Dict:
    mut counts: Dict = {}
    for word in words:
        if counts.has(word):
            mut n = int(counts.get(word))
            counts.set(word, str(n + 1))
        else:
            counts.set(word, "1")
    return counts

# Word frequency count (typed Dict[str, int]):
def count_words_typed(words: List[str]) -> Dict[str, int]:
    mut counts: Dict[str, int] = {}
    for word in words:
        if counts.has(word):
            counts.set(word, counts.get(word) + 1)
        else:
            counts.set(word, 1)
    return counts
```

### Common Mistakes

**Calling `.get()` without checking `.has()` first:**
```python
mut cfg: Dict = {}
mut val = cfg.get("host")    # undefined behavior if "host" was never set
```
Fix:
```python
if cfg.has("host"):
    mut val = cfg.get("host")
```

**Storing numbers in untyped `Dict` without converting to string:**
```python
mut d: Dict = {}
d.set("count", 5)    # ERROR: untyped Dict only accepts str values
```
Fix: Use `str(5)` or switch to `Dict[str, int]`.

**Modifying a Dict while iterating over `.items()`:**
```python
for k, v in scores.items():
    scores.set(k + "_copy", v)    # undefined behavior — modifying during iteration
```
Fix: Collect modifications in a separate list and apply them after the loop.

### Best Practices

- Prefer `Dict[K, V]` over untyped `Dict` for new code — typed access avoids manual `str()`/`int()` conversions and catches type errors at compile time.
- Always `.has()` before `.get()` unless the key is guaranteed to exist.
- For small, fixed key sets (e.g., 3–5 configuration keys), consider using a class with named fields instead of a `Dict` — named fields are faster and type-safe.
- Use `.items()` for iteration rather than iterating `.keys()` and calling `.get()` for each — it is more efficient.

---

## Set[T] — Unique Collection

### When to use

Use `Set[T]` when you need to track whether elements have been seen, remove duplicates from a collection, or perform set operations (union, intersection, difference). Sets provide O(1) average membership testing.

### How it works

A `Set[T]` is created with `Set[T].init()` (there is no `{...}` set literal —
`{}` is always a Dict literal). Supported element types are `str` and `int`.

```python
# Create a set:
mut seen: Set[int] = Set[int].init()

# Add an element (no-op if already present):
seen.add(1)
seen.add(2)
seen.add(2)    # already in set — no change

# Check membership (O(1) average):
if seen.contains(2):
    print("2 is in the set")

# Remove an element:
seen.remove(1)

# Size and emptiness:
mut n = seen.len()
mut empty = seen.is_empty()

# To iterate, convert to a List first (Sets are not directly iterable):
mut as_list = seen.to_list()
for x in as_list:
    print(x)
```

**Removing duplicates from a list:**
```python
def deduplicate(items: List[int]) -> List[int]:
    mut seen: Set[int] = Set[int].init()
    mut result: List[int] = []
    for x in items:
        if not seen.contains(x):
            seen.add(x)
            result.append(x)
    return result
```

**Membership testing (faster than `.contains()` on `List`):**
```python
def build_valid_codes() -> Set[int]:
    mut codes: Set[int] = Set[int].init()
    for c in [200, 201, 204, 301, 400, 404, 500]:
        codes.add(c)
    return codes
```

**Set operations:** `union`, `intersection`, `difference`, and `is_subset`
return/test against another set; `to_list()` materializes the elements.

### Common Mistakes

**Expecting sorted or insertion-order iteration:**
```python
mut as_list = s.to_list()   # order is unspecified
```
Fix: If you need ordered output, sort the list: `as_list.sort()`.

**Using `Set` when you also need to track counts:**
```python
mut seen: Set[str] = Set[str].init()
for word in words:
    seen.add(word)    # counts not available — just presence/absence
```
Fix: Use `Dict[str, int]` for frequency counting.

### Best Practices

- Use `Set[T]` over `List[T]` when the primary operation is membership testing — `.contains()` on a `Set` is O(1), but O(n) on a `List`.
- For deduplication, prefer the pattern of building a `Set` then reconstructing the `List` if order matters.
- Do not iterate a `Set` and expect any specific order.

---

## Tuples

### When to use

Use tuples to return multiple values from a function, or to group a small, fixed set of related values of different types without creating a class. Tuples are zero-allocation — they are passed directly in registers or on the stack.

### How it works

**Creating tuples:**

```python
mut point  = (10, 20)              # 2-element tuple
mut triple = (1, "hello", true)    # mixed types
mut empty  = ()                    # empty tuple (unit)
```

**Unpacking:**

```python
mut x, y = (3, 7)
print(x)    # 3
print(y)    # 7

mut a, b, c = (10, 20, 30)
```

**Functions returning tuples:**

```python
def min_max(items: List[int]) -> (int, int):
    mut lo = items[0]
    mut hi = items[0]
    for x in items:
        if x < lo: lo = x
        if x > hi: hi = x
    return (lo, hi)

def main() -> void:
    mut lo, hi = min_max([3, 1, 4, 1, 5, 9])
    print(f"min={lo} max={hi}")    # min=1 max=9
```

**Returning error + value pairs (alternative to `throws`):**
```python
def divide(a: int, b: int) -> (bool, int):
    if b == 0: return (false, 0)
    return (true, a / b)

def main() -> void:
    mut ok, result = divide(10, 2)
    if ok:
        print(f"result = {result}")
    else:
        print("division by zero")
```

**Tuple limits:**
- Up to 8 elements per tuple.
- All elements are stored as 64-bit integers internally. Pointer types work correctly. Mixed-type tuples with non-integer values may need explicit `as` casts when unpacking.

### Common Mistakes

**Exceeding 8 elements:**
```python
mut t = (1, 2, 3, 4, 5, 6, 7, 8, 9)    # ERROR: tuples support at most 8 elements
```
Fix: Use a class or `List`.

**Forgetting to unpack before using tuple elements:**
```python
mut result = min_max(items)
print(result)       # prints the raw tuple — probably not what you want
print(result[0])    # ERROR: tuple element access by index not supported
```
Fix: Unpack: `mut lo, hi = min_max(items)`

### Best Practices

- Use tuples for function return values only — for stored data with more than 2–3 fields, define a class.
- Name the unpacked variables descriptively: `mut lo, hi =` is better than `mut a, b =`.
- Prefer `throws` over `(bool, value)` tuples for error signaling — `throws` integrates with the `?` propagation operator.

---

## List Comprehensions

### When to use

Use list comprehensions to build a new list by transforming or filtering an existing collection in a single concise expression.

### How it works

**List comprehension — transform all elements:**

```python
mut numbers = [1, 2, 3, 4, 5]
mut squares: List[int] = [x * x for x in numbers]
# squares = [1, 4, 9, 16, 25]
```

**List comprehension with filter:**

```python
mut evens: List[int] = [x for x in numbers if x % 2 == 0]
# evens = [2, 4]
```

**Comprehension over a string list:**

```python
mut names  = ["Alice", "Bob", "Charlie"]
mut upper_names: List[str] = [n.to_upper() for n in names]
# upper_names = ["ALICE", "BOB", "CHARLIE"]
```

**How comprehensions compile:** The compiler translates list comprehensions directly to a tight C `for` loop that builds the result list. No intermediate allocations, no boxing.

> A comprehension body is a single transform expression with an optional
> trailing `if` filter: `[expr for x in seq]` or `[expr for x in seq if cond]`.
> Conditional (`a if cond else b`) expressions inside the body and generator
> expressions (`(... for ...)`) are not currently supported — use a regular
> `for` loop for those cases.

### Common Mistakes

**Missing type annotation on a comprehension result:**
```python
mut squares = [x * x for x in numbers]    # OK if numbers is List[int] — type inferred
mut evens   = []                           # ERROR: type cannot be inferred from empty literal
```

**Complex logic inside a comprehension (hard to read):**
```python
mut results = [transform(normalize(validate(x))) for x in data if expensive_check(x)]
```
Fix: Use a regular `for` loop with intermediate variables when the body is complex.

### Best Practices

- Use comprehensions for simple, one-level transforms and filters. Switch to a `for` loop when the body needs more than one operation.
- Keep comprehension conditions simple (`if x > 0`). For complex filtering logic, extract a helper function.

---

## Built-in Iteration Helpers

### When to use

Use `enumerate` when you need both the index and the value during iteration. Use `zip` to iterate two lists in parallel (e.g., names and scores, keys and values).

### How it works

**`enumerate(list)` — index + value:**

```python
mut fruits = ["apple", "banana", "cherry"]
for i, fruit in enumerate(fruits):
    print(f"  [{i}] {fruit}")
# [0] apple
# [1] banana
# [2] cherry
```

The index variable is always `int`, starting at 0. Compiles to a tight C `for` loop with no heap allocation.

**`zip(a, b)` — parallel iteration:**

```python
mut names:  List[str] = ["Alice", "Bob", "Carol"]
mut scores: List[int] = [95, 82, 78]

for name, score in zip(names, scores):
    print(f"  {name}: {score}")
# Alice: 95
# Bob: 82
# Carol: 78
```

Iteration stops at the end of the shorter list. Compiles to a single C `for` loop with no allocation.

**`range(n)` — integer range:**

```python
for i in range(10):
    print(i)    # 0 through 9

for i in range(2, 8):
    print(i)    # 2 through 7

for i in range(0, 10, 2):
    print(i)    # 0, 2, 4, 6, 8
```

`range(n)`, `range(start, stop)`, and `range(start, stop, step)` are all supported.

### Common Mistakes

**Using `zip` and expecting it to pad shorter lists:**
```python
mut a = [1, 2, 3]
mut b = [10, 20]
for x, y in zip(a, b):
    print(f"{x}, {y}")    # prints only two pairs — stops at len(b)
# 1, 10
# 2, 20
# (3 is never reached)
```
If you need all elements, ensure both lists have the same length.

**Forgetting that `enumerate` provides `int` index starting at 0:**
```python
for i, x in enumerate(items):
    print(i + 1)    # 1-based numbering — must add 1 explicitly
```

### Best Practices

- Always prefer `enumerate(items)` over a manual `mut i = 0` counter — it is cleaner and less error-prone.
- Use `zip` when processing two parallel arrays together. If the lengths may differ, document whether truncation is intentional.
- `range(n)` is the standard way to iterate a fixed number of times when you need the index value.

## Ownership and Collections

### When to use

Understanding ownership matters when you return, pass, or reassign collections. The rule is simple: the compiler injects a `free()` exactly once, at the scope where ownership ends.

### How it works

**Scope-owned collection — freed at end of function:**

```python
def demo() -> void:
    mut items: List[int] = []    # Own — heap allocated
    items.append(1)
    items.append(2)
    # scope ends: compiler injects List_i64_free(items) automatically
```

**Returning a collection transfers ownership to the caller:**

```python
def build_list() -> List[int]:
    mut result: List[int] = []
    result.append(42)
    return result    # ownership transferred — no free injected here

def main() -> void:
    mut nums = build_list()    # nums now owns the list
    print(nums[0])
    # scope ends: List_i64_free(nums) injected here
```

**Passing a collection borrows it (caller retains ownership):**

```python
def print_all(items: List[int]) -> void:    # borrows items
    for x in items: print(x)
    # no free injected — caller still owns the list

def main() -> void:
    mut nums = [1, 2, 3]
    print_all(nums)        # borrow
    print(nums[0])         # still valid — nums was not freed
    # scope ends: List_i64_free(nums) injected
```

**Compiler rule:** You must never manually call `free()` on a `List` or `Dict`. The compiler tracks ownership and inserts exactly one `free()` call. Calling `free()` manually would cause a double-free.

### Common Mistakes

**Accessing a collection after it has left scope:**
```python
mut ptr: List[int] = none
def fill() -> void:
    mut local: List[int] = [1, 2, 3]
    ptr = local    # UNSAFE: ptr holds a reference to a local that is freed on return
fill()
print(ptr[0])    # use-after-free
```
Fix: Return the list and assign it in the calling scope.

**Manually freeing a collection:**
```python
free(items)    # ERROR or double-free — compiler already injects free at scope exit
```

### Best Practices

- Do not worry about when collections are freed — the compiler handles it. Focus on scope boundaries.
- When you need a collection to outlive a function, return it.
- When you need a function to read a collection without owning it, pass it normally — borrowing is the default.
- Never store a `List` or `Dict` inside a raw pointer and free it manually. Use the ownership system.

---

## Common Collection Errors

### Empty list without type annotation

```python
mut data = []
# ERROR: cannot infer element type from empty list literal
```
Fix: `mut data: List[int] = []`

### Wrong element type in list literal

```python
mut nums: List[int] = [1, 2, "three"]
# ERROR [T-2]: element "three" has type str, expected int
```
Fix: Use a consistent element type. All elements must be the same type.

### Incompatible List types in assignment

```python
mut ints:   List[int]   = []
mut floats: List[float] = [1.0, 2.0]
ints = floats    # ERROR [T-2]: incompatible List types
```
Fix: Convert explicitly: `for x in floats: ints.append(x as int)`

### Dict missing key

```python
mut cfg: Dict = {}
mut val = cfg.get("host")    # undefined behavior if "host" not set
```
Fix: `if cfg.has("host"): val = cfg.get("host")`

### Out-of-bounds list access

```python
mut items = [1, 2, 3]
mut x = items[10]    # runtime panic: "Index 10 out of bounds (length 3)"
```
Indexing is bounds-checked at runtime. Fix: `if i < len(items): x = items[i]`

### Modifying a list while iterating it

```python
for x in items:
    if x < 0: items.remove(0)    # undefined behavior — modifying during for-in iteration
```
Fix: Collect indices to remove first, then remove after the loop. Or use a `while` loop with manual index management.

---

Next: [Classes and Extend →](08_classes.md)
