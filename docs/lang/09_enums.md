# 09 — Enums (Algebraic Data Types)

Tauraro enums are **algebraic data types (ADTs)** — each variant can carry its own named data.
They are the correct tool whenever a value can be one of several distinct shapes, and you want
the compiler to help you handle every case. This is the same concept as Rust's `enum`, Haskell's
`data`, or Scala's `sealed trait` — not the limited integer-only `enum` of C.

---

## 1. Declaring Enums

### When to use

Declare an enum whenever a value has a fixed set of possible forms and each form may carry
different data. Classic examples: command results, parser nodes, UI events, protocol messages,
and state machine states.

### How it works

**Simple enum (no data per variant):**

```python
enum Direction:
    North
    South
    East
    West

enum Color:
    Red
    Green
    Blue
    White
    Black
```

**Data-carrying variants:**

```python
enum Shape:
    Circle(radius: float)
    Rect(width: float, height: float)
    Triangle(base: float, height: float)
    Point                              # no data

enum Message:
    Text(content: str)
    Image(path: str, width: int, height: int)
    Close
    Error(code: int, msg: str)
```

Each variant can carry zero or more **named fields**. The field names matter — they appear in
destructuring patterns and serve as documentation.

### Common Mistakes

**Confusing a variant's fields with function parameters** — you cannot call `Shape.Circle(r=5.0)`;
variant fields are positional in construction and bound by position in patterns.

**Declaring an enum with only one variant** — if there is only ever one shape, use a class.

### Best Practices

- Use `PascalCase` for both the enum name and every variant name.
- Name data fields clearly: `radius`, `width`, `path` rather than `x`, `a`, `v1`.
- Include a `Close` / `None` / `Empty` no-data variant as a sentinel where natural.
- Keep enum declarations close to the code that uses them, or in a dedicated `types.tr` module.

---

## 2. Memory Representation

### When to use

Understanding memory layout matters when writing performance-sensitive code, reasoning about
cache behavior, or integrating with C.

### How it works

Tauraro enums compile to **tagged unions** — the most efficient possible C representation:

- A small integer (`tag`) identifies the active variant.
- A C `union` holds the payload for each data-carrying variant (only one is live at a time).
- No heap allocation, no virtual dispatch, no boxing.
- Matching on a variant compiles to a single `switch` on the tag integer.

```
struct Shape {
    int tag;           // 0 = Circle, 1 = Rect, 2 = Triangle, 3 = Point
    union {
        struct { float radius; }         Circle;
        struct { float width, height; }  Rect;
        struct { float base, height; }   Triangle;
    } data;
};
```

The total size is `sizeof(tag) + sizeof(largest_variant_payload)`. No variant ever allocates
extra memory.

### Common Mistakes

**Expecting recursive enum variants to work without `Pointer[T]`** — a struct cannot contain
itself in C. Recursive variants must use `Pointer[T]`:

```python
# WRONG — will not compile
enum Expr:
    Num(n: int)
    Add(left: Expr, right: Expr)    # ERROR: Expr contains Expr recursively

# CORRECT — use Pointer for recursive fields
enum Expr:
    Num(n: int)
    Add(left: Pointer[Expr], right: Pointer[Expr])
    Mul(left: Pointer[Expr], right: Pointer[Expr])
```

Allocate recursive variants in an `unsafe` block:

```python
unsafe:
    mut left_ptr: Pointer[Expr] = alloc[Expr](1)
    left_ptr.write(Expr.Num(5))
    mut right_ptr: Pointer[Expr] = alloc[Expr](1)
    right_ptr.write(Expr.Num(3))
    mut expr = Expr.Add(left_ptr, right_ptr)
```

### Best Practices

- Prefer flat (non-recursive) enums when possible — they require no allocation.
- When you do need recursive enums, wrap only the recursive fields in `Pointer[T]`, not the whole variant.

---

## 3. Constructing Enum Values

### When to use

Create an enum value at the point where you know which variant applies. Variant construction
is a single expression — no function call, no heap allocation for simple variants.

### How it works

```python
# Simple variants — just a name
mut d = Direction.North
mut c = Color.Red

# Data-carrying variants — positional fields
mut circle   = Shape.Circle(5.0)
mut rect     = Shape.Rect(3.0, 4.0)
mut triangle = Shape.Triangle(6.0, 8.0)
mut point    = Shape.Point               # no-data variant

# Multi-field variants
mut text_msg = Message.Text("hello")
mut err_msg  = Message.Error(404, "not found")
mut close    = Message.Close
```

Variant construction is a struct literal in C — it sets the tag integer and copies the payload.
The call is inlined by the compiler; there is no function call overhead.

### Common Mistakes

**Omitting the enum name prefix:**
```python
mut d = North       # ERROR: 'North' is not in scope
mut d = Direction.North   # OK: always use the fully-qualified variant name
```

**Wrong number of fields in the variant:**
```python
mut s = Shape.Rect(3.0)        # ERROR: Shape.Rect requires 2 fields (width, height)
mut s = Shape.Rect(3.0, 4.0)   # OK
```

### Best Practices

- Always qualify variant names with the enum type: `Direction.North`, not `North`.
- Construct enum values close to where they are consumed to keep data flow clear.

---

## 4. Pattern Matching

### When to use

Use `match` whenever you need to branch on which variant is active and optionally extract the
variant's data. This is the primary and idiomatic way to consume enum values in Tauraro.

### How it works

```python
def area(s: Shape) -> float:
    match s:
        case Shape.Circle(r):
            return r * r * 3.14159
        case Shape.Rect(w, h):
            return w * h
        case Shape.Triangle(b, h):
            return b * h / 2.0
        case Shape.Point:
            return 0.0
        case _:
            return 0.0
```

The names in the destructuring pattern (`r`, `w`, `h`) **bind** the variant's fields as local
immutable variables in the arm body. They shadow the original field names from the enum declaration.

**Matching simple enums:**
```python
def describe(d: Direction) -> str:
    match d:
        case Direction.North: return "heading north"
        case Direction.South: return "heading south"
        case Direction.East:  return "heading east"
        case Direction.West:  return "heading west"
        case _:               return "unknown direction"
```

**Guard conditions** — add `if <expr>:` after the pattern to filter a case:
```python
match s:
    case Shape.Circle(r) if r > 0.0:
        return r * r * 3.14159
    case Shape.Circle(r):
        return 0.0    # r <= 0 treated as degenerate
    case _:
        return 0.0
```

**Or-patterns** — match multiple variants with a single arm:
```python
match s:
    case Shape.Circle | Shape.Point:
        print("round or dimensionless")
    case _:
        print("has edges")
```

**The `is` operator** — check variant without destructuring:
```python
if s is Shape.Circle:
    print("it's a circle")
```

Matching compiles to a C `switch` on the tag integer — one comparison per arm, no indirection.

### Common Mistakes

**Matching with an unqualified variant name:**
```python
match color:
    case Red:          # ERROR: 'Red' is not in scope
    case Color.Red:    # OK
```

**Destructuring wrong number of fields:**
```python
match shape:
    case Shape.Rect(w):        # ERROR: Shape.Rect has 2 fields
    case Shape.Rect(w, h):     # OK
```

**Forgetting `case _:` when not all variants are handled** — the compiler does not currently
enforce exhaustive matching. A missing variant silently falls through without executing any arm.
Always include a catch-all unless you are certain every variant is covered:

```python
# RISKY — East and West silently unhandled
match d:
    case Direction.North: go_north()
    case Direction.South: go_south()

# SAFE
match d:
    case Direction.North: go_north()
    case Direction.South: go_south()
    case _: pass
```

### Best Practices

- Include `case _:` in every `match` unless you have explicitly handled every variant.
- When you intentionally handle every variant and want to catch future additions, use:
  ```python
  case _: raise("unreachable: unhandled variant")
  ```
  This turns a silent miss into a loud runtime failure.
- Keep match arm bodies short. If an arm needs more than 3–4 lines, extract a helper function.
- Place the most common variant first — the compiler may use this for branch prediction hints.

---

## 5. Adding Methods to Enums

### When to use

Add methods to enums via `extend` when the operation belongs logically to the enum itself: a
`Direction.opposite()`, a `Color.to_hex()`, an `Option.unwrap()`. This keeps the logic with the
type rather than scattered in standalone functions.

### How it works

`extend EnumName:` works identically to `extend ClassName:`. The first parameter `self` receives
the enum value.

```python
enum Direction:
    North
    South
    East
    West

extend Direction:
    pub def opposite(self) -> Direction:
        match self:
            case Direction.North: return Direction.South
            case Direction.South: return Direction.North
            case Direction.East:  return Direction.West
            case Direction.West:  return Direction.East
            case _:               return Direction.North

    pub def is_vertical(self) -> bool:
        match self:
            case Direction.North: return true
            case Direction.South: return true
            case _:               return false

    pub def to_str(self) -> str:
        match self:
            case Direction.North: return "North"
            case Direction.South: return "South"
            case Direction.East:  return "East"
            case Direction.West:  return "West"
            case _:               return "Unknown"

def main():
    mut d: Direction = Direction.North
    print(d.opposite().to_str())    # "South"
    print(d.is_vertical())          # true
```

**Note:** Declare the variable with an explicit type annotation (`mut d: Direction = ...`) before
calling methods on it. This ensures the compiler knows which vtable / method set to use.

### Common Mistakes

**Calling methods on an untyped enum variable** — the compiler may fail to resolve the method if
the variable's type is not explicitly declared.

**Treating `self` as mutable inside an enum method** — `self` is a copy of the enum value. You
can return a new variant but you cannot mutate the original caller's variable from inside the method.

### Best Practices

- Implement `to_str` / `__str__` on every enum you print during debugging.
- Implement `opposite`, `next`, `prev`, or domain-specific transformations as methods rather than
  standalone match blocks.

---

## 6. Enums in Collections and Class Fields

### When to use

Enums can be stored in `List[T]` and as class fields just like any other type. This is common for
message queues, event logs, state histories, and node graphs.

### How it works

```python
class Inbox:
    pub messages: List[Message]

extend Inbox:
    pub def init() -> Inbox:
        mut box = Inbox()
        box.messages = []
        return box

    pub def add(self, msg: Message) -> void:
        self.messages.append(msg)

    pub def process_all(self) -> void:
        mut i = 0
        while i < len(self.messages):
            match self.messages[i]:
                case Message.Text(content):
                    print(f"  text: {content}")
                case Message.Image(path, w, h):
                    print(f"  image: {path} ({w}x{h})")
                case Message.Error(code, msg):
                    print(f"  error {code}: {msg}")
                case Message.Close:
                    print("  connection closed")
                case _:
                    pass
            i = i + 1

def main():
    mut inbox = Inbox.init()
    inbox.add(Message.Text("hello"))
    inbox.add(Message.Image("/img/logo.png", 128, 128))
    inbox.add(Message.Error(500, "server error"))
    inbox.add(Message.Close)
    inbox.process_all()
```

`List[Message]` stores `Message` values **contiguously** — no boxing, no heap pointers per element
for simple enums. Cache-friendly and fast to iterate.

### Common Mistakes

**Storing enum variants as `List[str]` after converting to strings** — you lose the structured
data and the ability to match. Keep the enum in the list.

### Best Practices

- Use `List[EnumType]` to represent queues of events or commands.
- When processing a list of enums, prefer a `while` loop with an index over `for` if you need to
  remove elements during iteration.

---

## 7. The Built-in `Option[T]` and `Result[T, E]` Enums

### When to use

- Use `Option[T]` when a value may or may not be present — a nullable value without null pointers.
- Use `Result[T, E]` when an operation can succeed or fail with a structured error.

Both are built into the language; you do not need to declare them.

### How it works

**`Option[T]`** is equivalent to:
```python
enum Option:
    Some(value: T)
    None
```

Usage:
```python
def find_index(items: List[int], target: int) -> Option[int]:
    mut i = 0
    while i < len(items):
        if items[i] == target:
            return Option.Some(i)
        i = i + 1
    return Option.None

def main():
    mut items = [10, 20, 30, 40]
    mut result = find_index(items, 30)

    match result:
        case Option.Some(i): print(f"found at index {i}")
        case Option.None:    print("not found")
```

Safe access with `.?` (returns `None` if the inner value is absent):
```python
mut val: Option[int] = find_index(items, 99)
mut doubled = val.? * 2    # doubled is Option[int]: None if val was None
```

**`Result[T, E]`** is equivalent to:
```python
enum Result:
    Ok(value: T)
    Err(error: E)
```

Usage (typically returned by functions that declare `throws`):
```python
mut r: Result[int, str] = parse_int("42")
match r:
    case Result.Ok(v):    print(f"parsed: {v}")
    case Result.Err(msg): print(f"parse error: {msg}")
```

Shorthand accessors for quick checks:
```python
if r.is_ok:  print(f"value: {r.ok}")
if r.is_err: print(f"error: {r.err}")
```

### Common Mistakes

**Accessing `r.ok` without checking `r.is_ok` first** — if `r` holds `Err`, this is undefined
behavior. Always match or check before accessing the payload directly.

**Returning raw `None` / `null` from a function instead of `Option.None`** — `None` is a
Tauraro keyword meaning the enum variant; do not confuse it with null pointer semantics.

### Best Practices

- Prefer `match` over `.is_ok` / `.ok` for Result — it forces you to handle both branches.
- Use `Option[T]` instead of sentinel values (like `-1` for "not found").
- Document what `None` / `Err` means in the function's docstring.

---

## 8. Exhaustiveness and the Wildcard Pattern

### When to use

The `case _:` wildcard catches any variant not explicitly handled. Use it in every `match` unless
you have provably handled all variants.

### How it works

The compiler does **not** currently enforce exhaustive matching. A `match` with missing arms
silently falls through without executing any arm. There is no warning.

```python
# RISKY — if Direction gains a new variant (e.g. Up), it falls through silently
match d:
    case Direction.North: go_north()
    case Direction.South: go_south()

# SAFE — catch-all handles any variant not explicitly listed
match d:
    case Direction.North: go_north()
    case Direction.South: go_south()
    case _: pass

# ASSERTIVE — treats missing case as a programmer error
match d:
    case Direction.North: go_north()
    case Direction.South: go_south()
    case Direction.East:  go_east()
    case Direction.West:  go_west()
    case _: raise("unreachable: unexpected Direction variant")
```

### Common Mistakes

**Omitting `case _:` and later adding a new variant** — the new variant silently does nothing
when matched. This is a common source of logic bugs.

### Best Practices

- Always include `case _:` unless every variant is explicitly handled.
- When every variant is explicitly handled, add `case _: raise("unreachable")` as a safety net.

---

## 9. Full Example — Expression Evaluator

This example shows an enum with multiple data-carrying variants used in a recursive evaluator.

```python
enum Expr:
    Num(value: int)
    Add(left: int, right: int)
    Mul(left: int, right: int)
    Neg(value: int)

extend Expr:
    pub def eval(self) -> int:
        match self:
            case Expr.Num(v):    return v
            case Expr.Add(l, r): return l + r
            case Expr.Mul(l, r): return l * r
            case Expr.Neg(v):    return -v
            case _:              return 0

    pub def describe(self) -> str:
        match self:
            case Expr.Num(v):    return str(v)
            case Expr.Add(l, r): return str(l) + " + " + str(r)
            case Expr.Mul(l, r): return str(l) + " * " + str(r)
            case Expr.Neg(v):    return "-" + str(v)
            case _:              return "?"

def main():
    mut exprs: List[Expr] = [
        Expr.Num(42),
        Expr.Add(3, 4),
        Expr.Mul(6, 7),
        Expr.Neg(10)
    ]
    mut i = 0
    while i < len(exprs):
        mut e: Expr = exprs[i]
        print(f"  {e.describe()} = {e.eval()}")
        i = i + 1
    # Output:
    #   42 = 42
    #   3 + 4 = 7
    #   6 * 7 = 42
    #   -10 = -10
```

---

## Hausa Keyword Alternatives

| Tauraro keyword | Hausa equivalent |
|-----------------|-----------------|
| `enum` | `nau'i` (kind/type) |
| `match` | `duba` (check/examine) |
| `case` | `idan` (if/when) |
| `class _:` wildcard | `sauran` (the rest) |

---

Next: [Interfaces →](10_interfaces.md)
