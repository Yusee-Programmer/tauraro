# 04 — Control Flow

---

## Indentation-Based Scoping

### When to use
Always — indentation is mandatory in Tauraro. It is not a style choice; it defines block scope.
Every block-opening construct (`if`, `elif`, `else`, `while`, `for`, `match`, `case`, `def`,
`class`, `extend`, `with`, `unsafe`) requires an indented body.

### How it works

Tauraro uses **significant whitespace** — indentation defines block scope. The rule: increase
indentation by exactly 4 spaces to open a new block. All lines in the block must share the same
indentation.

```python
if x > 0:
    print("positive")   # inside the if block
    y = x * 2           # still inside
print("always runs")    # outside the if block
```

**Why significant whitespace?** It eliminates mismatched braces and inconsistent formatting.
The indentation you see is the indentation the compiler sees — no hidden block structure.

**Compiler rule:** Inconsistent indentation (mixing tabs and spaces, wrong depth) is a **parse
error**, not a warning. The error message will indicate the offending line number.

### Common Mistakes

```python
if x > 0:
print("positive")    # ERROR: expected indented block
```

```python
if x > 0:
    print("positive")
  y = x * 2          # ERROR: de-indentation does not match any outer block
```

### Best Practices

- Configure your editor to insert 4 spaces for the Tab key.
- Never mix tabs and spaces — the compiler rejects this.

---

## Conditionals

### `if` / `elif` / `else`

#### When to use
Use `if`/`elif`/`else` for branching on boolean conditions. For branching on a discrete set of
values or enum variants, prefer `match` — it is more exhaustive and compiles more efficiently.

#### How it works

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
- `elif` is optional and can appear multiple times
- `else` is optional and must be last

Branches are evaluated top to bottom. The first matching condition executes and the rest are
skipped.

#### Common Mistakes

```python
# Forgetting that conditions are evaluated top-down:
if score >= 70:     # matches 70, 80, 90 — the elif below never runs!
    print("C")
elif score >= 80:
    print("B")
```

Order matters. More specific (higher) conditions must come first.

#### Best Practices

- Put the most likely condition first to help branch prediction.
- If the branches are equally likely, sort from most specific (narrowest range) to least
  specific.
- When there are more than 4 or 5 discrete values, use `match` instead.

---

### Inline (Single-Statement) `if`

#### When to use
Use inline `if` only for very short guards (early returns, assignments, assertions) where the
intent is obvious at a glance. Never use inline `if` for multi-step logic.

**Important:** The inline form only works when the block contains **exactly one statement**. Do
not chain inline `if` with `elif` or `else` on the same line — this is a known parser
limitation. Use the indented multi-line form instead.

#### How it works

```python
if x < 0: x = 0
if flag: return
if n == 0: raise("division by zero")
```

#### Common Mistakes

```python
# Parser limitation: inline if followed by elif on same line FAILS
if x > 0: print("pos") elif x < 0: print("neg")   # PARSE ERROR

# Use multi-line form:
if x > 0:
    print("pos")
elif x < 0:
    print("neg")
```

#### Best Practices

- Use inline `if` only for guard clauses at the top of a function body.
- Avoid inline `if` in the middle of complex logic — use the multi-line form.

---

### Ternary Expression

#### When to use
Use the ternary expression when you need to choose between two values inline, without a full
`if`/`else` block. Both branches must produce the same type.

#### How it works

```python
label   = "pass" if score >= 60 else "fail"
sign    = 1 if x > 0 else -1
abs_val = x if x >= 0 else -x
```

The syntax is: `value_if_true if condition else value_if_false`.

**Compiler rule:** If the two branches produce different types, the compiler errors.
(This is a general type-mismatch diagnostic, not yet assigned a stable `[T-N]`
code — see "Reserved" in [19 — Compiler Errors](19_compiler_errors.md).)

#### Common Mistakes

```python
# Type mismatch in branches:
result = "ok" if success else 0    # ERROR: branches have types str and int
result = "ok" if success else "error"   # CORRECT
```

#### Best Practices

- Keep ternary expressions short — if either branch needs parentheses or is more than a few
  tokens, use a full `if`/`else` block.
- Do not nest ternary expressions. Use `if`/`elif`/`else` instead.

---

### Compound Conditions

```python
# Range check
if x >= 0 and x < 100:
    process(x)

# Privileged check
if role == "admin" or role == "root":
    allow_access()

# Null-safe pattern (short-circuit prevents null dereference)
if ptr != none and ptr.value > 0:
    use(ptr)

# Negation
if not valid:
    raise("invalid input")
```

---

## `while` Loops

### When to use
Use `while` when the number of iterations is not known in advance: reading until EOF, retrying
on failure, processing a queue until empty, running a game or event loop indefinitely.

### How it works

```python
mut i = 0
while i < 10:
    print(f"  i = {i}")
    i = i + 1
```

**Infinite Loops**

```python
while true:
    mut cmd = read_command()
    if cmd == "quit": break
    execute(cmd)
```

**`break` and `continue`**

```python
mut n = 0
while n < 20:
    n = n + 1
    if n % 2 == 0: continue    # skip even numbers (jump to condition check)
    if n > 15: break           # stop at 15
    print(n)                   # prints: 1 3 5 7 9 11 13 15
```

- `break` exits the innermost loop immediately
- `continue` skips to the next iteration of the innermost loop
- Both work for `while` and `for` loops

**Breaking out of nested loops:**

`break` and `continue` affect only the innermost enclosing loop. There is no labeled break.
Use a flag variable to break out of nested loops:

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

### Common Mistakes

```python
# Forgetting to advance the loop variable — infinite loop:
mut i = 0
while i < 10:
    print(i)
    # BUG: i is never incremented

# Condition uses assignment instead of comparison (syntax error in Tauraro, unlike C):
while n = read():    # ERROR: assignment is not a boolean expression
while n != 0:        # CORRECT
```

### Best Practices

- Always verify the loop termination condition is reachable before writing the loop body.
- For event loops and retry loops, prefer `while true: ... break` over a complex boolean
  condition that becomes stale.
- Use `while not done:` idiom for queue-draining loops rather than manually managing an index.

---

## `for` Loops

### When to use
Use `for` for:
- Counting over a range of integers (`range`)
- Iterating over all elements of a list
- Iterating with an index via `enumerate`

Use `while` when the iteration is not linear or when the loop body itself determines whether to
continue.

### How it works

**Ranging Over Integers:**

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
`range(start, stop, step)` → from `start` up to (but not including) `stop`, stepping by `step`

`range` compiles to a direct loop variable — zero allocation, maximum speed.

**Iterating Over a List:**

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

List iteration compiles to a direct index loop with no allocation and no iterator object
overhead.

**Iterating with `enumerate`:**

```python
items = ["a", "b", "c"]
for i, v in enumerate(items):
    print(f"  [{i}] = {v}")
```

`enumerate()` provides a zero-based index alongside each element.

**Range with `..` syntax:**

```python
for i in 0..10:    # 0, 1, 2, ..., 9
    print(i)
```

`0..10` is equivalent to `range(0, 10)`.

### When to Use `for` vs `while`

| Scenario | Prefer |
|----------|--------|
| Fixed range of integers | `for i in range(n):` |
| Iterate all elements of a list | `for x in items:` |
| Loop until a condition changes | `while condition:` |
| Complex multi-step state in loop | `while` with explicit variables |
| Retry / event / input loops | `while true:` + `break` |
| Enumerate with index | `for i, x in enumerate(items):` |

### Common Mistakes

```python
# Modifying a list while iterating over it:
for item in items:
    if condition(item):
        items.remove(item)    # BUG: skips elements after removal

# Correct: iterate a copy or collect indices first
for i in range(len(items) - 1, -1, -1):    # iterate backwards
    if condition(items[i]):
        items.remove_at(i)
```

```python
# Using range(len(items)) instead of iterating directly:
for i in range(len(items)):
    print(items[i])    # verbose — prefer:

for item in items:
    print(item)        # cleaner
```

### Best Practices

- Prefer `for x in items:` over `for i in range(len(items)):` when you do not need the index.
- Use `enumerate` when you need both the index and the element.
- Use `range` with a step for stride patterns (every nth element, countdown).
- Never modify the list you are iterating — collect changes and apply them after the loop.

---

## List Comprehensions

### When to use
Use list comprehensions to build a new list by transforming or filtering an existing one. They
are a concise alternative to a `for` loop that appends to a list.

### How it works

**Transform (map):**
```python
items = [1, 2, 3, 4, 5]
doubled = [x * 2 for x in items]       # [2, 4, 6, 8, 10]
strs    = [f"item {x}" for x in items] # ["item 1", "item 2", ...]
```

**Filter:**
```python
positives = [x for x in items if x > 0]
evens     = [x for x in items if x % 2 == 0]
```

**Transform and filter:**
```python
large_doubled = [x * 2 for x in items if x > 2]   # [6, 8, 10]
```

### Common Mistakes

```python
# Trying to use multiple statements inside the expression part:
# [if x > 0: x else -x for x in items]    # WRONG — use ternary:
[x if x > 0 else -x for x in items]        # CORRECT
```

### Best Practices

- Keep comprehensions to a single transformation and/or a single filter condition. When the
  logic is more complex, use a `for` loop with explicit `append`.
- Do not nest comprehensions — nested comprehensions are hard to read and debug.

---

## Pattern Matching with `match`

### When to use
Use `match` as the primary way to dispatch on enum variants and discrete values.
Prefer `match` over a chain of `if`/`elif` whenever you are testing the same variable against
multiple specific values. It compiles to a direct switch on the tag integer — the most efficient
possible dispatch.

### How it works

**Basic Value Matching:**

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

**Matching Enum Variants with Destructuring:**

This is the most powerful use of `match`. Each arm can destructure the data fields from an
enum variant:

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

The destructuring names (`r`, `w`, `h`, etc.) are bound as local immutable variables in the
arm body.

**Binding Patterns:**

```python
match maybe_val:
    case MaybeInt.Some(v):
        print(f"got: {v}")
    case MaybeInt.Nothing:
        print("nothing")
```

**Wildcard in Nested Position:**

Use `_` inside a variant pattern to ignore specific fields:

```python
match shape:
    case Shape.Rect(w, _):    # match any Rect, bind width, ignore height
        print(f"width = {w}")
    case _:
        pass
```

**Or-Patterns:**

Use `|` in a case to match multiple values with the same arm:

```python
match direction:
    case Direction.North | Direction.South:
        print("vertical")
    case Direction.East | Direction.West:
        print("horizontal")
```

**Guard Expressions:**

Add a boolean condition to a case arm with `if`:

```python
match value:
    case Shape.Circle(r) if r > 100:
        print("large circle")
    case Shape.Circle(r):
        print(f"small circle, radius {r}")
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

`pass` compiles to nothing in C. It is purely a syntax marker for intentionally empty blocks.

### Exhaustiveness

The compiler does NOT currently enforce exhaustive matching for all types. Always include
`case _:` unless you are certain every variant is handled:

```python
match direction:
    case Direction.North: go_north()
    case Direction.South: go_south()
    # BUG: East and West unhandled — silently falls through!
    case _: pass   # safe: catches East, West, any future variants
```

### Common Mistakes

```python
# Accessing variant fields outside match:
if s is Status.Failed:
    print(s.code)    # ERROR: use match to destructure

# Omitting case _: when not all variants are covered:
match status:
    case Status.Ok: handle_ok()
    # Status.Pending and Status.Failed fall through silently
```

```python
# Guard expression does not replace the variant test — both are required:
match shape:
    case _ if area(shape) > 100:    # valid but defeats the purpose of match
        print("large")
```

### Best Practices

- Always add `case _: pass` (or `case _: raise("unreachable")`) even when you think all
  variants are covered. It costs nothing and protects against silent bugs when new variants are
  added to an enum.
- Use guard expressions sparingly — they are most useful when the variant alone does not carry
  enough information for the decision.
- For enums with many variants, `match` is faster than an equivalent `if`/`elif` chain because
  it compiles to a switch statement.

---

## Scoping and Variable Lifetime

### When to use
Variable lifetime follows block scope automatically. You do not need to manage it explicitly
in most cases — the compiler inserts `free()` at the end of the block where an `Own` variable
was declared.

### How it works

Variables declared inside a block exist only in that block:

```python
mut x = 10
if true:
    mut y = 20      # y declared inside the if block
    print(x + y)    # OK: x is in outer scope
# y is out of scope here — free() injected if y was Own
print(x)            # OK
print(y)            # ERROR: 'y' is not in scope
```

**Ownership implication:** When an `Own` variable goes out of scope, the compiler injects a
`free()` call at the end of that block. See [Memory & Ownership](13_memory_and_ownership.md)
for details.

### Variable Shadowing

Declaring a new variable with the same name in an inner scope creates a new binding:

```python
x = 10
if true:
    x = 20          # reassigns the outer x if x is mut, or errors if x is immutable
    mut x = 30      # ERROR: re-declaring same name in same scope is an error
```

In Tauraro, shadowing (re-declaring with `mut`) in the **same scope** is an error. Reassigning
in an inner scope works only if the outer variable is `mut`.

### Common Mistakes

```python
# Declaring a variable inside an if and using it after:
if condition:
    result = compute()
print(result)    # ERROR: 'result' not in scope here

# Fix: declare before the if
mut result = 0
if condition:
    result = compute()
print(result)    # OK
```

### Best Practices

- Declare variables as close to their first use as possible. Narrow scope = fewer bugs.
- When a variable needs to be set inside a branch and used outside, declare it with `mut` in
  the outer scope before the branch.

---

## The `with` Statement

### When to use
Use `with` for any resource that needs deterministic cleanup: file handles, locks, network
connections, database transactions. Any class that implements `__enter__` and `__exit__` can
be used in a `with` block.

### How it works

```python
with Resource.init("path") as res:
    res.do_something()
# __exit__ is called automatically here, even if an exception was raised
```

The compiler expands `with X as alias:` into:
1. Evaluate `X` and store it in a temporary
2. Call `X.__enter__()` and bind the result to `alias`
3. Run the body
4. Call `X.__exit__(NULL, NULL, NULL)` unconditionally after the body

```python
with File.open("data.txt") as f:
    content = f.read_all()
    process(content)
# f is automatically closed here
```

### Common Mistakes

```python
# Opening a resource without 'with' and forgetting to close it:
f = File.open("data.txt")
content = f.read_all()
process(content)
# BUG: f.close() is never called if process() raises

# Always use with for resources:
with File.open("data.txt") as f:
    content = f.read_all()
    process(content)
```

### Best Practices

- Always use `with` for file I/O, locks, and any resource with a corresponding `close()` or
  `release()`.
- Implement `__enter__` and `__exit__` on your own resource classes to make them composable
  with `with`.
- See [21 — Operator Overloading](21_operator_overloading.md) for how to implement the context
  manager protocol.

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

### Enum Dispatch with `match`

```python
enum Command:
    Quit
    Move(dx: int, dy: int)
    Print(text: str)

def run(cmd: Command) -> void:
    match cmd:
        case Command.Quit:
            exit(0)
        case Command.Move(dx, dy):
            player.x += dx
            player.y += dy
        case Command.Print(text):
            print(text)
        case _:
            pass
```

---

Next: [Functions →](05_functions.md)
