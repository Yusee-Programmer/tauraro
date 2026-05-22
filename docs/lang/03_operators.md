# 03 — Operators

---

## Arithmetic Operators

```python
a = 10 + 3     # 13   addition
b = 10 - 3     # 7    subtraction
c = 10 * 3     # 30   multiplication
d = 10 / 3     # 3    integer division (both operands are int)
e = 10 % 3     # 1    remainder (modulo)
f = -10        # -10  unary negation
```

### Integer Division vs Float Division

**Critical:** When both operands are integer types, `/` performs **integer division** — the result is truncated toward zero.

```python
10 / 3      # 3  (not 3.333...)
-10 / 3     # -3 (truncates toward zero, not floor)
7 / 2       # 3
```

To get a float result, cast at least one operand:
```python
precise = 10 as float / 3        # 3.333...
also    = 10 / 3 as float        # 3.333... (same — cast binds tighter)
wrong   = (10 / 3) as float      # 3.0  — division first, then cast
```

**Best practice:** Any time you divide and need the fractional part, explicitly cast before dividing.

### Modulo

```python
10 % 3   # 1
-10 % 3  # -1  (sign follows the dividend, like C)
10 % -3  # 1
```

The `%` operator's sign follows the dividend (the left operand), identical to C.

---

## Comparison Operators

```python
a == b    # equal
a != b    # not equal
a < b     # less than
a > b     # greater than
a <= b    # less than or equal
a >= b    # greater than or equal
```

All comparisons produce a `bool`.

### String Comparison

Using `==` and `!=` on two `str` values compares **content** (via `strcmp`), not pointer identity:

```python
s1 = "hello"
s2 = "hello"
s1 == s2   # true — same content
```

Using `<`, `>`, `<=`, `>=` on strings is lexicographic (byte-by-byte comparison).

---

## Logical Operators

```python
a and b    # logical AND — true iff both are true
a or b     # logical OR  — true iff at least one is true
not a      # logical NOT — true iff a is false
```

Hausa equivalents:
```python
a da b     # same as: a and b
a ko b     # same as: a or b
ba a       # same as: not a
```

### Short-Circuit Evaluation

`and` and `or` short-circuit. The right operand is not evaluated if the left determines the result:

```python
# If ptr is null, ptr.value is never accessed (avoids null dereference):
if ptr != none and ptr.value > 0:
    process(ptr)

# If cache is valid, disk_read() is never called:
if cache_hit or disk_read():
    use_data()
```

**Best practice:** Put the cheaper or safest check on the left side of `and`/`or`.

---

## Bitwise Operators

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

**Bitwise with flags pattern:**

```python
const PERM_READ    = 1 << 0    # 0b001
const PERM_WRITE   = 1 << 1    # 0b010
const PERM_EXEC    = 1 << 2    # 0b100

mut perms = PERM_READ | PERM_WRITE     # set two flags

# Test a flag:
if perms & PERM_READ != 0:
    print("read allowed")

# Clear a flag:
perms = perms & ~PERM_WRITE

# Toggle a flag:
perms = perms ^ PERM_EXEC
```

---

## Assignment Operators

### Simple Assignment

```python
x = 42
name = "Tauraro"
```

### Augmented Assignment

```python
mut x = 10
x += 5     # x = x + 5  → 15
x -= 3     # x = x - 3  → 12
x *= 2     # x = x * 2  → 24
x /= 4     # x = x / 4  → 6  (integer division if x is int)
x %= 5     # x = x % 5  → 1
```

Augmented assignment operators compile to their expanded forms in C. There is no special compound-assignment optimization — the compiler handles it.

**Note:** Augmented assignment requires the variable to be declared with `mut`. Using `+=` on an immutable variable is a compile error:

```python
x = 10
x += 5    # ERROR: cannot assign to immutable binding 'x'
```

---

## The `in` Membership Operator

```python
items = [1, 2, 3, 4, 5]

if 3 in items:
    print("found 3")

if 99 not in items:
    print("99 is absent")
```

For `List[T]`, `in` compiles to a linear scan: `O(n)`. There is no set type with `O(1)` membership (yet). For repeated membership checks, use a `Dict` with sentinel values or maintain a sorted list and use binary search.

---

## The `as` Cast Operator

`as` performs explicit type conversion:

```python
n: int = 42
f: float = n as float          # int → float (widening, exact)
i: int   = 3.99 as int         # float → int (truncates toward 0 → 3)
b: i8    = 300 as i8           # int → i8 (wraps: 300 % 256 = 44)
u: u64   = -1 as u64           # signed → unsigned (all bits set → 2^64 - 1)
```

**What `as` compiles to:**
```python
n as float     →   (double)(n)
f as int       →   (long long)(f)
n as i8        →   (int8_t)(n)
n as u64       →   (unsigned long long)(n)
```

### Pointer Casts (unsafe only)

```python
unsafe:
    p: Pointer[void] = x as Pointer[void]    # any pointer → void*
    q: Pointer[int]  = p as Pointer[int]     # void* → typed pointer
    n: usize = p as usize                     # pointer → integer (address)
    p2: Pointer[int] = n as Pointer[int]      # integer → pointer
```

Pointer casts must be inside `unsafe:`. Outside unsafe, the compiler rejects casting to `Pointer[T]`.

---

## Operator Precedence (Highest to Lowest)

| Level | Operators | Associativity |
|-------|-----------|---------------|
| 1 (highest) | `()` (grouping), `.` (access), `[]` (index), function call | left |
| 2 | Unary: `-`, `not`, `~`, `&`, `*` | right |
| 3 | `as` (cast) | left |
| 4 | `*`, `/`, `%` | left |
| 5 | `+`, `-` | left |
| 6 | `<<`, `>>` | left |
| 7 | `&` (bitwise AND) | left |
| 8 | `^` (bitwise XOR) | left |
| 9 | `\|` (bitwise OR) | left |
| 10 | `==`, `!=`, `<`, `>`, `<=`, `>=`, `in` | left |
| 11 | `not` | right |
| 12 | `and` | left |
| 13 | `or` | left |
| 14 (lowest) | `=`, `+=`, `-=`, `*=`, `/=`, `%=` | right |

**When in doubt, use parentheses.** This makes precedence explicit and avoids common mistakes:

```python
# Ambiguous:
result = a + b * c & d

# Clear:
result = (a + (b * c)) & d
```

---

## Operator Overloading

Classes can define custom behavior for arithmetic operators by implementing special methods:

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

**How operator overloading compiles:** `a + b` where `a` is a class type becomes `Vec2___add__(a, b)` in C. There is no dynamic dispatch — the types are known at compile time.

---

## Common Mistakes

### Division Surprise

```python
avg = total / count     # integer division if both are int!
```

**Fix:** `avg = total as float / count`

### Operator on Wrong Types

```python
mut a: int = 10
mut b: float = 3.0
mut c = a * b    # ERROR [T-2]: cannot multiply int and float
```

**Fix:** `mut c = a as float * b`

### Bitwise vs Logical Confusion

```python
if x & 1 == 1:     # WRONG: == has higher precedence than &, evaluates as x & (1 == 1) = x & true
    ...

if (x & 1) == 1:   # CORRECT: parentheses make precedence explicit
    ...
```

**Best practice:** Always parenthesize when mixing bitwise and comparison operators.

---

Next: [Control Flow →](04_control_flow.md)
