# 02 — Variables, Types, and Literals

---

## Variable Declaration

### Immutable Variables (default)

```python
name = "Tauraro"       # type inferred: str
version = 1            # type inferred: int
pi: float = 3.14159    # type annotated explicitly
```

Variables declared without `mut` are **immutable** — they cannot be reassigned after declaration. This communicates intent: if a variable never changes, don't mark it mutable.

### Mutable Variables

```python
mut count = 0
count = count + 1     # OK

mut x: int = 10
x = 20                # OK
```

`mut` applies to the binding itself. A `mut` variable can be reassigned to any value of its type.

### Constants

```python
const MAX_SIZE    = 4096
const PI          = 3.14159
const APP_NAME    = "Tauraro"
const FLAG_READ   = 0x01
const FLAG_WRITE  = 0x02
```

Constants are:
- Evaluated at compile time
- Inlined as C `#define`-style literals wherever used
- Cannot be assigned to or have `mut` applied
- Usable in any scope (global or local)

**Best practice:** Use `const` for every magic number and string literal. Replace `if code == 404:` with `if code == HTTP_NOT_FOUND:`.

---

## Mutability Rules and Compiler Errors

### Error: Assignment to Immutable Variable

```python
x = 10
x = 20    # ERROR [M-6]: cannot assign to immutable binding 'x'
```

**Fix:** Add `mut` to the declaration:
```python
mut x = 10
x = 20    # OK
```

### Error: Use After Move

```python
data = load_bytes()
send(data)            # data moved
print(len(data))      # ERROR [M-2]: 'data' was moved into 'send' and is no longer valid
```

**Fix:** Don't use `data` after it's passed to a function that takes ownership, or restructure so the use comes before the move.

---

## The Type System

### Compiler Rule T-1: No Implicit Coercion

Tauraro **never** converts between numeric types silently. Every coercion must be explicit with `as`:

```python
x: int = 10
y: float = x          # ERROR [T-1]: cannot assign int to float without explicit cast

y: float = x as float  # OK
back: int = y as int   # OK — truncates toward zero
```

**Why:** Implicit coercion is a major source of subtle bugs in C (integer overflow, sign extension, precision loss). Explicit casts make every conversion visible and auditable.

### Compiler Rule T-2: Type Mismatch in Operations

```python
mut a: int = 10
mut b: float = 3.14
mut c = a + b          # ERROR [T-2]: cannot add int and float
mut c = a as float + b  # OK
```

---

## Primitive Types

### Integer Types

| Tauraro | C equivalent | Bits | Signed range |
|---------|-------------|------|-------------|
| `int` | `long long` | 64 | −9.2×10¹⁸ to 9.2×10¹⁸ |
| `i64` | `long long` | 64 | same as `int` |
| `i32` | `int` | 32 | −2.1×10⁹ to 2.1×10⁹ |
| `i16` | `short` | 16 | −32,768 to 32,767 |
| `i8` | `int8_t` | 8 | −128 to 127 |
| `u64` | `unsigned long long` | 64 | 0 to 1.8×10¹⁹ |
| `u32` | `unsigned int` | 32 | 0 to 4.3×10⁹ |
| `u16` | `unsigned short` | 16 | 0 to 65,535 |
| `u8` | `uint8_t` | 8 | 0 to 255 |
| `usize` | `unsigned long long` | 64 | platform word size, unsigned |
| `isize` | `long long` | 64 | platform word size, signed |

**When to use which:**
- `int` — default for all integer values in application code
- `i32` — when the value must fit in 32 bits (e.g., Win32 handles, file offsets)
- `u8` — byte values, pixel data, raw binary buffers
- `usize` — sizes, counts, array indices (matches C `size_t`)
- Fixed-width types — FFI, file formats, network protocols, hardware registers

### Float Types

| Tauraro | C equivalent | Bits | Precision |
|---------|-------------|------|-----------|
| `float` | `double` | 64 | ~15 significant digits |
| `f64` | `double` | 64 | same as `float` |
| `f32` | `float` | 32 | ~7 significant digits |

**When to use `f32` vs `f64`:** Use `f32` when memory is constrained (e.g., large arrays of floats for graphics/audio). Use `float`/`f64` everywhere else — the extra precision prevents accumulation errors.

### Boolean

```python
active: bool = true
disabled: bool = false
```

Compiles to C `bool` (from `<stdbool.h>`), stored as `uint8_t`.

**Note:** `true`/`false` are lowercase. `True`/`False` are also accepted. `1`/`0` are NOT booleans — use explicit `as bool` if you need to convert.

### Character

```python
c: char = 'A'
newline: char = '\n'
zero: char = '\0'
```

Single byte (ASCII). Not a Unicode code point. For multi-byte characters, use `str` or handle UTF-8 bytes directly.

### String

```python
greeting: str = "Hello, world!"
empty: str = ""
```

A `str` is a C `char*` — a pointer to a null-terminated UTF-8 byte sequence. It is not heap-owned by default; string literals are stored in the read-only data segment. Dynamically-built strings (from `+` concatenation or f-strings) are heap-allocated.

**Ownership note:** A `str` variable that holds a heap-allocated string (e.g., from `f"..."` or `s + t`) is tracked as `Own` and freed at scope exit. A `str` variable holding a string literal is not freed (it points to static memory). The compiler distinguishes these automatically.

### Void

```python
def do_something() -> void:
    print("done")
```

`void` is only valid as a return type. Variables cannot have type `void`. A function with no return type annotation implicitly returns `void`.

---

## Type Inference

The compiler infers types from initializers. Explicit annotations are optional for locals:

```python
x = 42              # int
y = 3.14            # float
s = "hello"         # str
flag = true         # bool
items = [1, 2, 3]   # List[int]
ch = 'A'            # char
```

**When to annotate explicitly:**
- When the default inference isn't what you want:
  ```python
  small: i32 = 42         # force 32-bit
  byte: u8 = 255          # force unsigned byte
  precise: f32 = 3.14     # force 32-bit float
  ```
- When the variable is declared without an initializer:
  ```python
  mut result: List[int] = []   # needs type to know what List this is
  ```
- For function parameters — always annotate:
  ```python
  def process(data: List[int], threshold: float) -> int:
  ```

**Compiler rule:** If a variable is declared without a type annotation and without an initializer, the compiler errors: `[T-5] cannot infer type without initializer`.

---

## Literals

### Integer Literals

```python
decimal   = 42
underscore = 1_000_000       # readability separators
hex       = 0xFF
hex_cap   = 0xDEAD_BEEF
binary    = 0b1010_1010
octal     = 0o755
negative  = -42
```

All integer literals default to type `int` (64-bit). Cast with `as` for other sizes:
```python
small: i32 = 42 as i32
byte: u8   = 255 as u8
```

### Float Literals

```python
x = 3.14
y = 1.0e10          # scientific notation: 1.0 × 10¹⁰
z = -0.001
very_small = 1.5e-7
```

Float literals default to `float` (f64). For f32:
```python
f: f32 = 3.14 as f32
```

### String Literals

```python
plain = "Hello, world!"
with_escape = "first\tsecond\nthird"
empty = ""
```

Escape sequences:

| Sequence | Meaning |
|----------|---------|
| `\n` | newline (LF) |
| `\t` | horizontal tab |
| `\r` | carriage return |
| `\\` | literal backslash |
| `\"` | literal double quote |
| `\'` | literal single quote |
| `\0` | null byte |

### F-String Literals

```python
name = "Tauraro"
msg = f"Hello, {name}!"
result = f"1 + 1 = {1 + 1}"
pi_msg = f"pi ≈ {3.14159}"
nested = f"value = {compute(x)}"
```

F-strings evaluate expressions inside `{}` at runtime and produce a formatted string. They compile to the most efficient C representation — direct `printf` format strings with correct format specifiers for each embedded type.

**What can go inside `{}`:**
- Variable names: `{x}`
- Arithmetic: `{a + b}`
- Function calls: `{len(items)}`
- Method calls: `{obj.method()}`
- Nested f-strings are not supported — build complex strings with `+`

**How f-strings compile:**
```python
f"point = ({p.x}, {p.y})"
```
→
```c
_tr_format("point = (%lld, %lld)", p->x, p->y)
```
`_tr_format` is a `printf`-based allocator. The result is a heap-allocated `char*`.

### Boolean Literals

```python
a = true
b = false
c = gaskiya    # Hausa: true
d = karya      # Hausa: false
```

### None / Null

```python
mut ptr: str = none
mut obj: MyClass = none
```

`none` represents a null pointer. It is only valid for pointer types and optional types. Assigning `none` to a value type (int, float, bool, char) is a compiler error:

```python
mut x: int = none   # ERROR [M-7]: cannot assign 'none' to 'x' which has type 'int'
```

---

## The `as` Cast Operator

All type conversions use `as`:

```python
# Numeric widening
n: int = 42
f: float = n as float        # long long → double

# Numeric narrowing (truncates)
big: int = 1000
small: i8 = big as i8        # 1000 % 256 = 232 (or -24 signed)

# Float to int (truncates toward zero)
f2: float = 3.99
i: int = f2 as int           # 3, not 4

# Pointer reinterpretation (requires unsafe:)
unsafe:
    p: Pointer[void] = x as Pointer[void]
    q: Pointer[int] = p as Pointer[int]
```

**Cast rules:**
- Numeric `as` numeric: always allowed, may truncate or lose precision
- `as bool`: any non-zero value becomes `true`, zero becomes `false`
- `as char`: truncates to 8 bits
- `as Pointer[T]`: only inside `unsafe:` blocks
- Class-to-class cast: not allowed (use interfaces for polymorphism)

---

## Naming Conventions and Style

| Item | Convention | Example |
|------|-----------|---------|
| Variables | `snake_case` | `user_count`, `is_valid` |
| Functions | `snake_case` | `parse_int`, `get_name` |
| Classes | `PascalCase` | `Point`, `HttpClient` |
| Enums | `PascalCase` | `Direction`, `ParseError` |
| Interfaces | `PascalCase` | `Animal`, `Drawable` |
| Constants | `UPPER_SNAKE_CASE` | `MAX_SIZE`, `HTTP_OK` |
| Type parameters | Single uppercase | `T`, `K`, `V` |
| Modules | `snake_case` | `math.geometry` |

**Indentation:** 4 spaces. Tabs are not allowed. The indentation is significant — it defines block scope.

**Comments:** Use `#` for line comments:
```python
# This is a comment
x = 42  # inline comment
```

There are no block comments. Long explanations use multiple `#` lines.

---

Next: [Operators →](03_operators.md)
