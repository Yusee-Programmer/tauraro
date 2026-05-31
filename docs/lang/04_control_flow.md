# 04 — Control Flow

---

## Indentation-Based Scoping

Tauraro uses **significant whitespace** — indentation defines block scope. The rule: increase indentation by exactly 4 spaces to open a new block. All lines in the block must share the same indentation.

```python
if x > 0:
    print("positive")   # inside the if block
    y = x * 2           # still inside
print("always runs")    # outside the if block
```

**Why significant whitespace?** It eliminates mismatched braces and inconsistent formatting. It forces consistent code style across all codebases. The indentation you see is the indentation the compiler sees — no hidden block structure.

**Compiler rule:** Inconsistent indentation (mixing tabs and spaces, wrong depth) is a **parse error**, not a warning. The error message will indicate the offending line number.

---

## Conditionals

### if / elif / else

```python
score = 85

if score >= 90:
    print("A")
elif score >= 80:
    print("B")
elif score >= 70:
    print("C")
else:
    print("F")
```

- `if` is always required
- `elif` (or `koidan` in Hausa) is optional and can appear multiple times
- `else` (or `sai`) is optional and must be last

Branches are evaluated top to bottom. The first matching condition executes and the rest are skipped.

### Inline (Single-Statement) If

When the block contains exactly one statement, it can be written on the same line as the `if`:

```python
if x < 0: x = 0
if flag: return
if n == 0: raise("division by zero")
```

**When to use:** Only for very short guards and early exits. Multi-step logic always deserves its own block.

### Ternary Expression

```python
label   = "pass" if score >= 60 else "fail"
sign    = 1 if x > 0 else -1
abs_val = x if x >= 0 else -x
```

Compiles to a C ternary: `(condition ? then_expr : else_expr)`. Both branches must produce the same type.

**Compiler rule:** If the two branches produce different types, the compiler errors with [T-2].

### Compound Conditions

```python
# Range check
if x >= 0 and x < 100:
    process(x)

# Privileged check
if role == "admin" or role == "root":
    allow_access()

# Null-safe pattern
if ptr != none and ptr.value > 0:
    use(ptr)

# Negation
if not valid:
    raise("invalid input")
```

---

## while Loops

```python
mut i = 0
while i < 10:
    print(f"  i = {i}")
    i = i + 1
```

Hausa: `yayinda i < 10:`

### Infinite Loops

```python
mut running = true
while running:
    mut cmd = read_command()
    if cmd == "quit":
        running = false
    else:
        execute(cmd)
```

Or more idiomatically with `break`:

```python
while true:
    mut cmd = read_command()
    if cmd == "quit": break
    execute(cmd)
```

`while true:` compiles to `while(1)` in C. The optimizer knows this loop body always executes at least once and applies appropriate optimizations.

### break and continue

```python
mut n = 0
while n < 20:
    n = n + 1
    if n % 2 == 0: continue    # skip even numbers (jump to condition check)
    if n > 15: break           # stop at 15
    print(n)                   # prints: 1 3 5 7 9 11 13 15
```

- `break` (or `tsaya`) exits the innermost loop immediately
- `continue` (or `ci_gaba`) skips to the next iteration of the innermost loop
- Both work for `while` and `for` loops

**Note:** `break` and `continue` affect only the innermost enclosing loop. There is no labeled break for nested loops. Use a flag variable or restructure with a function call to break out of nested loops:

```python
mut found = false
mut i = 0
while i < rows and not found:
    mut j = 0
    while j < cols and not found:
        if grid[i * cols + j] == target:
            found = true
        j = j + 1
    i = i + 1
```

---

## for Loops

### Ranging Over Integers

```python
for i in range(10):         # 0, 1, 2, ..., 9
    print(i)

for i in range(1, 6):       # 1, 2, 3, 4, 5
    print(i)

for i in range(0, 10, 2):   # 0, 2, 4, 6, 8
    print(i)

for i in range(10, 0, -1):  # 10, 9, 8, ..., 1 (countdown)
    print(i)
```

`range(n)` → `range(0, n, 1)`
`range(start, stop)` → `range(start, stop, 1)`
`range(start, stop, step)` → iterates from `start` up to (but not including) `stop`, stepping by `step`

**How range compiles:** `for i in range(n):` → `for (long long i = 0; i < n; i++)` — direct C loop, zero allocation, maximum speed.

Hausa: `ga i cikin zango(10):` (using `zango` for range)

### Iterating Over a List

```python
names = ["Alice", "Bob", "Charlie"]
for name in names:
    print(f"  Hello, {name}!")

scores = [90, 85, 72, 61, 95]
for score in scores:
    if score >= 90: print("A")
    elif score >= 80: print("B")
    else: print("other")
```

**How list iteration compiles:** Lowers to:
```c
for (long long _i = 0; _i < names->len; _i++) {
    char* name = names->data[_i];
    // body
}
```
No iterator object, no allocation — identical to hand-written C array loop.

### Iterating with enumerate

```python
items = ["a", "b", "c"]
for i, v in enumerate(items):
    print(f"  [{i}] = {v}")
```

`enumerate()` provides a zero-based index alongside each element. Compiles to a counter variable alongside the element loop.

### When to Use for vs while

| Scenario | Prefer |
|----------|--------|
| Fixed range of integers | `for i in range(n)` |
| Iterate all elements of a list | `for x in items` |
| Loop until a condition changes | `while condition:` |
| Complex multi-step state in loop | `while` with explicit variables |
| Retry / event / input loops | `while true:` + `break` |
| Enumerate with index | `for i, x in enumerate(items)` |

---

## Pattern Matching with `match`

`match` (or `duba` in Hausa) is Tauraro's exhaustive switch expression. It is the primary tool for dispatching on enum variants and discrete values.

### Basic Value Matching

```python
code = 200

match code:
    case 200: print("OK")
    case 201: print("Created")
    case 400: print("Bad Request")
    case 404: print("Not Found")
    case 500: print("Server Error")
    case _:   print(f"Unknown: {code}")
```

`case _:` is the wildcard arm — it matches anything not covered by earlier cases.

Hausa syntax:
```python
duba code:
    hali 200: buga("OK")
    hali _:   buga("noma")
```

### Matching Enum Variants with Destructuring

This is the most powerful use of `match`. Each arm can destructure the data fields from an enum variant:

```python
enum Shape:
    Circle(radius: int)
    Rect(width: int, height: int)
    Triangle(base: int, height: int)
    Point

def area(s: Shape) -> int:
    match s:
        case Shape.Circle(r):
            return r * r * 3
        case Shape.Rect(w, h):
            return w * h
        case Shape.Triangle(b, h):
            return b * h / 2
        case Shape.Point:
            return 0
        case _:
            return 0
```

The destructuring names (`r`, `w`, `h`, etc.) are bound as local variables in the arm body. They are immutable.

**How enum matching compiles:**
```c
switch (s.tag) {
    case Shape_Circle: {
        long long r = s.data.Circle.radius;
        return r * r * 3;
    }
    case Shape_Rect: {
        long long w = s.data.Rect.width;
        long long h = s.data.Rect.height;
        return w * h;
    }
    // ...
}
```
A direct `switch` on the tag integer — the most efficient possible dispatch.

### Binding Patterns

Match arms can bind the entire matched value:

```python
match maybe_val:
    case MaybeInt.Some(v):
        print(f"got: {v}")
    case MaybeInt.Nothing:
        print("nothing")
```

### Wildcard in Nested Position

Use `_` in an enum variant pattern to ignore specific fields:

```python
match shape:
    case Shape.Rect(w, _):    # match any rect, bind width, ignore height
        print(f"width = {w}")
    case _:
        pass
```

### `pass` — Explicit No-Op

When a match arm or block needs to be empty, use `pass`:

```python
match event:
    case Event.KeyPress(k): handle_key(k)
    case Event.MouseMove:   pass     # ignore mouse moves
    case Event.Quit:        exit(0)
```

`pass` compiles to nothing in C. It is purely a syntax marker that the compiler requires for non-empty blocks.

### Exhaustiveness

The compiler does NOT currently enforce exhaustive matching for all types. Always include `case _:` unless you are certain you have handled every variant:

```python
match direction:
    case Direction.North: go_north()
    case Direction.South: go_south()
    # BUG: East and West unhandled — silently falls through!
    case _: pass   # safe: catches East, West, any future variants
```

**Best practice:** Even when you think you've handled all cases, add `case _: pass` (or `case _: raise("unreachable")` for invariant violations). It costs nothing and prevents silent bugs when new variants are added to an enum.

---

## Scoping and Variable Lifetime

Variables declared inside a block exist only in that block:

```python
mut x = 10
if true:
    mut y = 20      # y declared inside the if block
    print(x + y)    # OK: x is in outer scope
# y is out of scope here
print(x)            # OK
print(y)            # ERROR: 'y' is not in scope
```

**Ownership implication:** When an `Own` variable goes out of scope, the compiler injects a `free()` call at the end of that block. See [Memory & Ownership](13_memory_and_ownership.md) for details.

### Variable Shadowing

Declaring a new variable with the same name inside an inner scope creates a new binding (shadows the outer one):

```python
x = 10
if true:
    x = 20          # this reassigns the outer x (if x is mut), or errors (if x is immutable)
    mut x = 30      # ERROR: re-declaring same name as 'let' in same scope is an error
```

In Tauraro, shadowing (re-declaring with `mut`) in the same scope is an error. Reassigning (without `mut` re-declaration) in an inner scope works if the outer variable is `mut`.

---

## Complete Examples

### FizzBuzz

```python
def fizzbuzz(n: int) -> void:
    mut i = 1
    while i <= n:
        if i % 15 == 0:     print("FizzBuzz")
        elif i % 3 == 0:    print("Fizz")
        elif i % 5 == 0:    print("Buzz")
        else:               print(f"{i}")
        i = i + 1
```

### Binary Search

```python
def binary_search(arr: List[int], target: int) -> int:
    mut lo = 0
    mut hi = len(arr) - 1
    while lo <= hi:
        mut mid = (lo + hi) / 2
        if arr[mid] == target:   return mid
        elif arr[mid] < target:  lo = mid + 1
        else:                    hi = mid - 1
    return -1
```

### Collatz Sequence

```python
def collatz_length(n: int) -> int:
    mut steps = 0
    mut x = n
    while x != 1:
        if x % 2 == 0: x = x / 2
        else:          x = x * 3 + 1
        steps = steps + 1
    return steps
```

---

## The `with` Statement

The `with` statement provides deterministic resource cleanup via the context manager protocol.
Any class that implements `__enter__` and `__exit__` can be used in a `with` block.

```python
with Resource.init("path") as res:
    res.do_something()
# __exit__ is called automatically here
```

The compiler expands `with X as alias:` into:
1. Evaluate `X` and store it in a temporary
2. Call `X.__enter__()` and bind the result to `alias`
3. Run the body
4. Call `X.__exit__(NULL, NULL, NULL)` after the body

See [21 — Operator Overloading](21_operator_overloading.md) for how to implement `__enter__` and `__exit__`.

---

Next: [Functions →](05_functions.md)
