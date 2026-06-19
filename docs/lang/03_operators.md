# 03 — Operators

---

## Arithmetic Operators

### When to use
Use arithmetic operators for numeric computation on integer or float values. The key rule to
remember is that `/` performs integer division when both operands are integers.

### How it works

```python
a = 10 + 3     # 13   addition
b = 10 - 3     # 7    subtraction
c = 10 * 3     # 30   multiplication
d = 10 / 3     # 3    integer division (both operands are int)
e = 10 % 3     # 1    remainder (modulo)
f = -10        # -10  unary negation
g = 2 ** 8     # 256  exponentiation
```

**Integer Division vs Float Division**

**Critical:** When both operands are integer types, `/` performs **integer division** — the
result is truncated toward zero, not rounded.

```python
10 / 3      # 3  (not 3.333...)
-10 / 3     # -3 (truncates toward zero, not floor)
7 / 2       # 3
```

The `//` operator is an alias for integer division, mirroring Python semantics for readability:

```python
10 // 3     # 3  — explicit integer division
```

To get a float result, cast at least one operand before dividing:
```python
precise = 10 as float / 3        # 3.333...
also    = 10 / 3 as float        # 3.333... (same — cast binds tighter)
wrong   = (10 / 3) as float      # 3.0  — division first, then cast — WRONG
```

**Modulo:**

```python
10 % 3   # 1
-10 % 3  # -1  (sign follows the dividend, like C)
10 % -3  # 1
```

The `%` operator's sign follows the dividend (left operand), identical to C behavior.

**Exponentiation:**

```python
x = 2 ** 10    # 1024 — integer exponentiation
y = 2.0 ** 0.5 # square root via exponentiation
```

### Common Mistakes

```python
# Integer division surprise:
avg = total / count         # integer division if both are int — loses fractional part
avg = total as float / count  # CORRECT

# Cast applied after division:
result = (a / b) as float   # WRONG: still integer division, then cast
result = a as float / b     # CORRECT: cast before dividing
```

### Best Practices

- Any time you divide and need a fractional result, cast the leftmost operand to `float` before
  the `/`.
- Use `//` instead of `/` when integer division is intentional — it makes the intent explicit to
  readers.
- Use underscore separators in large numeric literals: `1_000_000`, `0xFF_FF_FF_FF`.

---

## Comparison Operators

### When to use
Use comparison operators to produce a `bool` result from two values of the same type. All
comparisons are between values of the same type (no implicit coercion — see Rule T-1).

### How it works

```python
a == b    # equal
a != b    # not equal
a < b     # less than
a > b     # greater than
a <= b    # less than or equal
a >= b    # greater than or equal
```

All comparisons produce a `bool`.

**String Comparison**

Using `==` and `!=` on two `str` values compares **content** (via `strcmp`), not pointer
identity:

```python
s1 = "hello"
s2 = "hello"
s1 == s2   # true — same content
```

Using `<`, `>`, `<=`, `>=` on strings performs lexicographic (byte-by-byte) comparison.

### Common Mistakes

```python
# Comparing int and float without cast:
n: int = 10
f: float = 10.0
n == f    # ERROR: cannot compare int and float
n as float == f    # OK
```

### Best Practices

- When checking whether a float calculation produced a specific value, use a range comparison
  rather than `==`: `abs(result - expected) < 0.0001`.
- Use `==` on strings freely — it compares content, not identity.

---

## Logical Operators

### When to use
Use logical operators to combine boolean conditions in `if`, `while`, and ternary expressions.
`and` and `or` short-circuit, which makes them useful as cheap null/validity guards.

### How it works

```python
a and b    # logical AND — true iff both are true
a or b     # logical OR  — true iff at least one is true
not a      # logical NOT — true iff a is false
```

**Short-Circuit Evaluation**

`and` and `or` short-circuit. The right operand is not evaluated if the left already determines
the result:

```python
# If ptr is null, ptr.value is never accessed (avoids null dereference):
if ptr != none and ptr.value > 0:
    process(ptr)

# If cache is valid, the expensive disk_read() call is never made:
if cache_hit or disk_read():
    use_data()
```

### Common Mistakes

```python
# Using & and | instead of and/or on booleans:
if x > 0 & y > 0:    # WRONG: & is bitwise, not logical; unexpected precedence
if x > 0 and y > 0:  # CORRECT

# not applied to the wrong operand:
if not x == 0:    # parsed as: (not x) == 0 — likely not what you meant
if not (x == 0):  # CORRECT — or write: if x != 0:
```

### Best Practices

- Put the cheaper or safest check on the left of `and`/`or` to take full advantage of
  short-circuiting.
- Use `not` sparingly — prefer `!=` or a positive condition when readable.
- Parenthesize compound logical expressions to make precedence explicit.

---

## Bitwise Operators

### When to use
Use bitwise operators for flag manipulation, low-level protocol parsing, hardware register
access, hash functions, and any operation that works on individual bits of an integer.

### How it works

```python
a & b     # bitwise AND
a | b     # bitwise OR
a ^ b     # bitwise XOR
~a        # bitwise NOT (complement)
a << n    # left shift by n bits
a >> n    # right shift by n bits (arithmetic on signed, logical on unsigned)
```

```python
flags = 0b0101 & 0b0110    # 0b0100 = 4
union = 0b0101 | 0b0110    # 0b0111 = 7
diff  = 0b0101 ^ 0b0110    # 0b0011 = 3
inv   = ~0b0101             # all bits flipped (−6 as i64)
left  = 1 << 3              # 8
right = 32 >> 2             # 8
```

**Flag manipulation pattern:**

```python
const PERM_READ    = 1 << 0    # 0b001
const PERM_WRITE   = 1 << 1    # 0b010
const PERM_EXEC    = 1 << 2    # 0b100

mut perms = PERM_READ | PERM_WRITE     # set two flags

# Test a flag:
if (perms & PERM_READ) != 0:
    print("read allowed")

# Clear a flag:
perms = perms & ~PERM_WRITE

# Toggle a flag:
perms = perms ^ PERM_EXEC
```

### Common Mistakes

```python
# Bitwise & vs comparison precedence:
if x & 1 == 1:      # WRONG: == has higher precedence than &
                    # parsed as: x & (1 == 1) = x & true (which is x & 1 but misleading)

if (x & 1) == 1:    # CORRECT: parentheses make intent explicit
```

```python
# Right-shifting a signed negative value:
n: int = -16
n >> 2    # implementation-defined in C; avoid right-shifting negative signed integers
(n as u64) >> 2    # CORRECT: cast to unsigned first for logical shift
```

### Best Practices

- Always parenthesize bitwise operations when combining them with comparison operators.
- Define flag constants with named `const` values using `1 << n` notation — never write raw
  hex constants like `0x04` for flags without a named alias.
- Use `u64`/`u32`/`u8` types for bit fields; avoid bitwise operations on signed types.

---

## Assignment Operators

### When to use
Use augmented assignment (`+=`, `-=`, etc.) when updating a mutable variable in-place. All
forms require the variable to be declared with `mut`.

### How it works

**Simple Assignment:**
```python
x = 42
name = "Tauraro"
```

**Augmented Assignment:**
```python
mut x = 10
x += 5     # x = x + 5  → 15
x -= 3     # x = x - 3  → 12
x *= 2     # x = x * 2  → 24
x /= 4     # x = x / 4  → 6  (integer division if x is int)
x %= 5     # x = x % 5  → 1
```

Augmented assignment operators compile to their expanded forms in C.

### Common Mistakes

```python
x = 10
x += 5    # ERROR [M-8]: cannot assign to immutable binding 'x'
```

Augmented assignment requires `mut`. Forgetting it on the original declaration is the most
common cause of this error.

### Best Practices

- Prefer `x += 1` over `x = x + 1` — it is shorter and more readable.
- Use augmented assignment consistently to signal that a variable is being updated, not replaced.

---

## The `in` and `not in` Membership Operators

### When to use
Use `in` to test whether a value is present in a list or a substring exists in a string. Use
`not in` for the negated form. Both return a `bool`.

### How it works

```python
items = [1, 2, 3, 4, 5]

if 3 in items:
    print("found 3")

if 99 not in items:
    print("99 is absent")

# String membership:
sentence = "Hello, world!"
if "world" in sentence:
    print("contains world")
```

For `List[T]`, `in` compiles to a linear scan: O(n). There is no built-in set type with O(1)
membership (yet). For repeated membership checks on large collections, use a `Dict` with
sentinel values or maintain a sorted list and use binary search.

### Common Mistakes

```python
# Using in on a Dict checks keys, not values:
d = Dict[str, int]()
d.insert("a", 1)
if "a" in d:    # checks keys — this is correct for Dict
    ...
```

### Best Practices

- For lists where membership is checked many times in a hot path, consider restructuring to use
  a `Dict` with the values as keys.
- Use `not in` instead of `not (x in items)` — the former is more readable.

---

## The `is` Operator

### When to use
Use `is` to test whether an enum variable holds a specific variant. It is the lightweight form
of a `match` expression when you only need to branch on a single variant.

### How it works

```python
enum Status:
    Ok
    Pending
    Failed(code: int)

s = get_status()

if s is Status.Ok:
    print("all good")

if s is Status.Failed:
    print("failed")
```

`is` compiles to a tag comparison — a single integer equality check.

### Common Mistakes

```python
# is does not destructure — use match to access variant fields:
if s is Status.Failed:
    print(s.code)    # ERROR: cannot access variant field via 'is'

# Use match instead:
match s:
    case Status.Failed(code):
        print(f"failed with code {code}")
    case _:
        pass
```

### Best Practices

- Use `is` for simple boolean variant checks.
- Use `match` whenever you need to access the data inside a variant.

---

## The `as` Cast Operator

### When to use
Use `as` every time you need a value in a different type. It is mandatory — there is no
implicit coercion. See Rule T-1 in the type system documentation.

### How it works

```python
n: int = 42
f: float = n as float          # int → float (widening, exact)
i: int   = 3.99 as int         # float → int (truncates toward 0 → 3)
b: i8    = 300 as i8           # int → i8 (wraps: 300 % 256 = 44)
u: u64   = -1 as u64           # signed → unsigned (all bits set → 2^64 - 1)
```

**Pointer Casts (unsafe only):**

```python
unsafe:
    p: Pointer[void] = x as Pointer[void]    # any pointer → void*
    q: Pointer[int]  = p as Pointer[int]     # void* → typed pointer
    n: usize = p as usize                     # pointer → integer (address)
    p2: Pointer[int] = n as Pointer[int]      # integer → pointer
```

Pointer casts must be inside `unsafe:`. Outside unsafe, the compiler rejects casting to
`Pointer[T]`.

### Common Mistakes

```python
# Cast applied too late — integer division already happened:
result = (total / count) as float   # WRONG: 3.0 instead of 3.333...
result = total as float / count     # CORRECT: 3.333...

# Narrowing without checking range:
val: i8 = big_int as i8    # silently wraps if big_int > 127
```

### Best Practices

- Cast as early as possible in an expression, not at the end.
- Document narrowing casts with a comment stating the invariant that guarantees the value fits.

---

## Operator Precedence (Highest to Lowest)

Understanding precedence prevents subtle bugs, especially when mixing bitwise and comparison
operators.

| Level | Operators | Associativity |
|-------|-----------|---------------|
| 1 (highest) | `()` grouping, `.` field access, `[]` index, function call | left |
| 2 | Unary: `-`, `not`, `~` | right |
| 3 | `as` (cast) | left |
| 4 | `**` (power) | right |
| 5 | `*`, `/`, `//`, `%` | left |
| 6 | `+`, `-` | left |
| 7 | `<<`, `>>` | left |
| 8 | `&` (bitwise AND) | left |
| 9 | `^` (bitwise XOR) | left |
| 10 | `\|` (bitwise OR) | left |
| 11 | `==`, `!=`, `<`, `>`, `<=`, `>=`, `in`, `not in`, `is` | left |
| 12 | `not` | right |
| 13 | `and` | left |
| 14 | `or` | left |
| 15 (lowest) | `=`, `+=`, `-=`, `*=`, `/=`, `%=` | right |

**When in doubt, use parentheses.** This makes precedence explicit and prevents common mistakes:

```python
# Ambiguous:
result = a + b * c & d

# Clear:
result = (a + (b * c)) & d
```

---

## Operator Overloading

### When to use
Use operator overloading when you have a domain type (vector, money, matrix, complex number,
date) where the standard arithmetic or comparison operators have a natural, well-understood
meaning. Do not overload operators to mean something non-obvious — it surprises readers and
makes debugging harder.

### How it works

Classes can define custom behavior for arithmetic operators by implementing dunder methods.
The compiler dispatches to these methods automatically when an operator is applied to a class
instance. Dispatch is fully static — the types are known at compile time, no dynamic dispatch.

```python
class Vec2:
    pub x: float
    pub y: float

extend Vec2:
    pub def init(x: float, y: float) -> Vec2:
        mut v = Vec2()
        v.x = x
        v.y = y
        return v

    pub def __add__(self, other: Vec2) -> Vec2:
        return Vec2.init(self.x + other.x, self.y + other.y)

    pub def __sub__(self, other: Vec2) -> Vec2:
        return Vec2.init(self.x - other.x, self.y - other.y)

    pub def __mul__(self, scalar: float) -> Vec2:
        return Vec2.init(self.x * scalar, self.y * scalar)

    pub def __eq__(self, other: Vec2) -> bool:
        return self.x == other.x and self.y == other.y

def main():
    mut a = Vec2.init(1.0, 2.0)
    mut b = Vec2.init(3.0, 4.0)
    mut c = a + b              # calls Vec2.__add__(a, b)
    mut d = c * 2.0            # calls Vec2.__mul__(c, 2.0)
    print(f"c = ({c.x}, {c.y})")
    print(f"d = ({d.x}, {d.y})")
```

A simpler example with a `Money` type:

```python
class Money:
    pub cents: int

extend Money:
    pub def __add__(self, other: Money) -> Money:
        mut m = Money()
        m.cents = self.cents + other.cents
        return m

    pub def __lt__(self, other: Money) -> bool:
        return self.cents < other.cents

mut a = Money.init(100)
mut b = Money.init(50)
mut total = a + b      # calls Money___add__
mut cheap = a < b      # calls Money___lt__
```

**Supported overloadable operators:**

| Method | Operator | Example |
|--------|----------|---------|
| `__add__(self, other)` | `+` | `a + b` |
| `__sub__(self, other)` | `-` | `a - b` |
| `__mul__(self, other)` | `*` | `a * b` |
| `__div__(self, other)` | `/` | `a / b` |
| `__mod__(self, other)` | `%` | `a % b` |
| `__eq__(self, other)` | `==` | `a == b` |
| `__ne__(self, other)` | `!=` | `a != b` |
| `__lt__(self, other)` | `<` | `a < b` |
| `__le__(self, other)` | `<=` | `a <= b` |
| `__gt__(self, other)` | `>` | `a > b` |
| `__ge__(self, other)` | `>=` | `a >= b` |
| `__len__(self)` | `len(x)` | `len(a)` |
| `__str__(self)` | `str(x)` / f-string | `f"{a}"` |
| `__getitem__(self, i)` | `x[i]` | `a[i]` |
| `__setitem__(self, i, v)` | `x[i] = v` | `a[i] = v` |

See [21 — Operator Overloading](21_operator_overloading.md) for the full list of supported
dunders.

### Common Mistakes

```python
# Overloading __add__ for two different right-hand side types requires two methods:
pub def __add__(self, other: Vec2) -> Vec2: ...   # Vec2 + Vec2
pub def __mul__(self, scalar: float) -> Vec2: ... # Vec2 * float
# There is no implicit conversion: Vec2 * 2 (int) will error — cast: Vec2 * (2 as float)
```

### Best Practices

- Only overload operators when the semantic meaning is universally clear (addition of vectors,
  comparison of dates).
- Always implement `__eq__` and `__ne__` together — an inconsistent equality pair is a logic
  bug.
- If you implement any comparison operator, implement all six (`__lt__`, `__le__`, `__gt__`,
  `__ge__`, `__eq__`, `__ne__`) for consistency.

---

## Common Operator Mistakes Summary

### Division Surprise

```python
avg = total / count     # integer division if both are int!
```

**Fix:** `avg = total as float / count`

### Type Mismatch

```python
mut a: int = 10
mut b: float = 3.0
mut c = a * b    # ERROR: cannot multiply int and float
```

**Fix:** `mut c = a as float * b`

> Note: `[T-2]` is reserved for the Sendable field-type check described in
> [16 — Concurrency](16_concurrency.md); the numeric type-mismatch diagnostics
> on this page are not yet assigned a stable code (see "Reserved" in
> [19 — Compiler Errors](19_compiler_errors.md)).

### Bitwise vs Logical Confusion

```python
if x & 1 == 1:     # WRONG: == binds tighter than &
                   # parsed as: x & (1 == 1)

if (x & 1) == 1:   # CORRECT: parentheses make precedence explicit
```

**Rule:** Always parenthesize when mixing bitwise and comparison operators.

---

Next: [Control Flow →](04_control_flow.md)
