# 02 — Variables, Types, and Literals

---

## Variable Declaration

### Immutable Variables (default)

#### When to use
Use immutable variables (the default) for any value that does not need to change after it is
assigned. This is the right choice for the vast majority of locals — function results, parsed
values, configuration, loop accumulators that are replaced rather than updated in-place. Prefer
immutable unless you have a specific reason for mutation.

#### How it works

```python
name = "Tauraro"       # type inferred: str
version = 1            # type inferred: int
pi: float = 3.14159    # type annotated explicitly
```

Variables declared without `mut` are **immutable** — they cannot be reassigned after declaration.
The compiler enforces this: any attempt to assign a second value to an immutable binding is a
hard error. This communicates intent clearly: if a variable never changes, the reader can rely on
that fact while reasoning about the code.

#### Common Mistakes

```python
x = 10
x = 20    # ERROR [M-6]: cannot assign to immutable binding 'x'
```

The most common mistake is forgetting to add `mut` when you later decide the variable needs to
change. The fix is always to add `mut` to the original declaration.

#### Best Practices

- Start every variable as immutable. Only add `mut` if the compiler tells you it must change.
- Immutable variables are self-documenting — `ratio = a / b` signals "this never changes" to the
  reader without any additional comments.
- Use meaningful names rather than `x` or `tmp` — immutable variables often live longer than you
  expect.

---

### Mutable Variables

#### When to use
Use `mut` for variables that are updated during their lifetime: loop counters, accumulators,
state machines, buffers being built up incrementally, and anything set in one place and modified
in another.

#### How it works

```python
mut count = 0
count = count + 1     # OK

mut x: int = 10
x = 20                # OK
```

`mut` applies to the binding itself. A `mut` variable can be reassigned to any value of its
declared type. It cannot be reassigned to a value of a different type — mutability is about the
binding, not the type system.

#### Common Mistakes

```python
mut pair = (1, 2)
pair = (3, 4)    # OK — reassigning the whole binding
```

A common misunderstanding is thinking `mut` makes the object itself mutable (as in Rust). In
Tauraro, `mut` means the binding can point to a new value; class fields have their own mutability
independent of whether the binding is `mut`.

#### Best Practices

- Minimize the scope of `mut` variables. Declare them as close to first use as possible.
- Prefer augmented assignment (`x += 1`) over `x = x + 1` — it is shorter and makes the mutation
  more obvious.
- Name mutable accumulator variables clearly: `mut total = 0`, `mut found = false`.

---

### Constants

#### When to use
Use `const` for any fixed value that is known at compile time and used in more than one place —
or that has a meaningful name even when used only once. Magic numbers are always a `const`
candidate.

#### How it works

```python
const MAX_SIZE    = 4096
const PI          = 3.14159
const APP_NAME    = "Tauraro"
const FLAG_READ   = 0x01
const FLAG_WRITE  = 0x02
```

Constants are:
- Evaluated at **compile time**
- Inlined as C `#define`-style literals wherever used — zero runtime overhead
- Cannot be assigned to or have `mut` applied
- Usable in any scope (global or local)
- Conventional naming is `UPPER_SNAKE_CASE`

#### Common Mistakes

```python
const BUFFER: List[int] = []    # ERROR: constants must be scalar literals
```

Constants can only hold scalar literals (integers, floats, booleans, string literals). They
cannot hold heap-allocated values like lists or class instances.

#### Best Practices

- Replace every magic number with a named constant: use `HTTP_NOT_FOUND` instead of `404`.
- Group related constants at the top of a file or in a dedicated module.
- Use constants in `match` cases and `if` conditions — it makes intent obvious and keeps the
  value in one place for future changes.

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
send(data)            # data moved into 'send'
print(len(data))      # ERROR [M-2]: 'data' was moved into 'send' and is no longer valid
```

**Fix:** Do not use `data` after it is passed to a function that takes ownership. If you need
`data` afterward, restructure so all reads happen before the move.

---

## The Type System

### Compiler Rule T-1: No Implicit Coercion

#### When to use `as`
Use `as` every time you need a value in a different numeric type — widening, narrowing, signed
to unsigned, float to int, or int to float. There are no exceptions: the compiler never converts
between numeric types silently.

#### How it works

```python
x: int = 10
y: float = x          # ERROR [T-1]: cannot assign int to float without explicit cast

y: float = x as float  # OK
back: int = y as int   # OK — truncates toward zero
```

**Why:** Implicit coercion is a major source of subtle bugs in C — integer overflow, sign
extension, precision loss. Explicit casts make every conversion visible and auditable.

#### Common Mistakes

```python
def area(width: int, height: int) -> float:
    return width * height    # ERROR [T-1]: int expression in float return
    # Fix:
    return (width * height) as float
```

Another common mistake is performing integer arithmetic then casting the final result, losing
precision during intermediate steps:
```python
result = (a / b) as float   # WRONG: integer division first, then cast — e.g. (7/2) as float = 3.0
result = a as float / b     # CORRECT: cast before dividing — 7.0 / 2 = 3.5
```

#### Best Practices

- Cast the earliest operand that needs to be a float, not the final result.
- When doing mixed-type arithmetic in a tight loop, define the variables with the correct type
  upfront so the casts are at the boundary, not scattered through the loop.

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

#### When to use each integer type

| Tauraro | Bits | Signed range | Recommended use |
|---------|------|-------------|-----------------|
| `int` | 64 | −9.2×10¹⁸ to 9.2×10¹⁸ | Default for all integer values in application code |
| `i64` | 64 | same as `int` | Explicit 64-bit in FFI or file formats |
| `i32` | 32 | −2.1×10⁹ to 2.1×10⁹ | Win32 handles, file offsets, APIs that require 32-bit |
| `i16` | 16 | −32,768 to 32,767 | Compact integer arrays, audio samples |
| `i8` | 8 | −128 to 127 | Small signed values, network protocol fields |
| `u64` | 64 | 0 to 1.8×10¹⁹ | Large counts, file sizes, hash values |
| `u32` | 32 | 0 to 4.3×10⁹ | IPv4 addresses, pixel values, color codes |
| `u16` | 16 | 0 to 65,535 | Port numbers, small counters, compact arrays |
| `u8` | 8 | 0 to 255 | Byte values, pixel channels, raw binary buffers |
| `usize` | 64 | platform word, unsigned | Array indices, lengths, counts (matches C `size_t`) |
| `isize` | 64 | platform word, signed | Signed differences between pointers or indices |

#### How it works

```python
count: int = 1_000_000        # default integer type
index: usize = 0              # use for array indices and sizes
byte_val: u8 = 0xFF           # byte from a binary buffer
port: u16 = 8080              # network port number
offset: i32 = -128            # signed file offset (Win32 API compatible)
```

#### Common Mistakes

```python
mut i: u8 = 200
i += 100    # WRAPS: 300 % 256 = 44, no runtime error
```

Unsigned overflow wraps silently, matching C semantics. If you need overflow detection, check
before the operation or use a wider type.

```python
arr = [10, 20, 30]
idx: int = compute_index()
print(arr[idx])    # OK — compiler accepts int as index
```

Using a signed type as an array index is accepted but can produce a negative index if the value
is negative. The bounds checker will catch this at runtime (in safe mode), but it is better to
use `usize` for indices.

#### Best Practices

- Use `int` as the default. Only switch to a narrower or unsigned type when you have a specific
  reason (FFI compatibility, memory layout, protocol field widths).
- Use `usize` for all array indices and collection lengths — it matches what the runtime expects
  and avoids the signed/unsigned comparison warning pattern from C.
- Define fixed-width types at module boundaries where the exact bit width matters.

### Float Types

#### When to use each float type

| Tauraro | Bits | Precision | Recommended use |
|---------|------|-----------|-----------------|
| `float` | 64 | ~15 significant digits | Default for all floating-point values |
| `f64` | 64 | same as `float` | Explicit 64-bit in FFI or numeric code |
| `f32` | 32 | ~7 significant digits | Large arrays (graphics, audio, ML weights) where memory matters |

#### How it works

```python
x: float = 3.14159        # default float
y: f32 = 0.5 as f32       # explicit 32-bit
scientific = 1.5e-7        # scientific notation
```

#### Common Mistakes

```python
a: f32 = 0.1
b: f32 = 0.2
c: f32 = a + b
# c may NOT equal 0.3 exactly due to f32 rounding
```

Using `f32` for financial calculations or cumulative sums leads to precision errors that compound.
Always use `float`/`f64` when precision matters.

#### Best Practices

- Use `float`/`f64` everywhere except when you have large arrays where memory bandwidth is
  the bottleneck (e.g., 3D mesh vertex data, neural network weights).
- If you mix `f32` and `f64` in the same expression, you must cast explicitly: `val as float`.

### Boolean

```python
active: bool = true
disabled: bool = false
```

Stored as a single byte internally. `true`/`false` are lowercase. `True`/`False` are also
accepted. `1`/`0` are NOT booleans — use explicit `as bool` if you need to convert.

### Character

```python
c: char = 'A'
newline: char = '\n'
zero: char = '\0'
```

Single byte (ASCII). Not a Unicode code point. For multi-byte characters, use `str` or handle
UTF-8 bytes directly as `u8` values.

### String

#### When to use
Use `str` for human-readable text and any sequence of UTF-8 bytes. String literals are stored
in read-only memory; f-strings and concatenated strings are heap-allocated and owned by the
declaring scope.

#### How it works

```python
greeting: str = "Hello, world!"
empty: str = ""
```

A `str` is a pointer to a null-terminated UTF-8 byte sequence. String literals are in read-only
memory. Dynamically-built strings (from `+` concatenation or f-strings) are heap-allocated.

**Ownership note:** A `str` holding a heap-allocated string (e.g., `f"..."` or `s + t`) is
tracked as `Own` and freed at scope exit. A `str` pointing to a string literal is not freed.
The compiler distinguishes these automatically.

#### Common Mistakes

```python
def make_greeting(name: str) -> str:
    greeting: str = f"Hello, {name}!"
    return greeting    # OK — ownership transfers to caller
```

```python
# Comparing strings with ==
s1 = "hello"
s2 = "hello"
s1 == s2    # true — compares CONTENT via strcmp, not pointer identity
```

A common mistake from C backgrounds is expecting `==` on strings to compare pointers. In
Tauraro, `==` on `str` always compares content.

#### Best Practices

- Prefer f-strings for all interpolation rather than manual `+` concatenation.
- When a function returns a `str` built from an f-string, the caller takes ownership — document
  this in the function signature or doc comment.

### Void

```python
def do_something() -> void:
    print("done")
```

`void` is only valid as a return type. Variables cannot have type `void`. A function with no
return type annotation implicitly returns `void`.

---

## Type Inference

### When to annotate explicitly

The compiler infers types from initializers. Explicit annotations are optional for locals but
required when the default inference is not what you want, when the variable is declared without
an initializer, and for all function parameters.

### How it works

```python
x = 42              # int
y = 3.14            # float
s = "hello"         # str
flag = true         # bool
items = [1, 2, 3]   # List[int]
ch = 'A'            # char
```

When the default inference is not the right type, annotate:

```python
small: i32 = 42         # force 32-bit
byte: u8 = 255          # force unsigned byte
precise: f32 = 3.14     # force 32-bit float
```

When the variable is declared without an initializer:
```python
mut result: List[int] = []   # needs type to know what List this is
```

Function parameters always require annotation:
```python
def process(data: List[int], threshold: float) -> int:
```

### Common Mistakes

```python
mut buf    # ERROR [T-5]: cannot infer type without initializer
```

If a variable is declared without a type annotation and without an initializer, the compiler
errors with `[T-5] cannot infer type without initializer`. Always provide one or the other.

### Best Practices

- Use inference for local variables — it reduces clutter and keeps the types in one place.
- Always annotate function parameters and return types — this makes the API self-documenting.
- Annotate explicitly when using a non-default numeric width to make the choice visible to
  readers.

---

## Literals

### Integer Literals

```python
decimal    = 42
underscore = 1_000_000       # readability separators — no effect on value
hex        = 0xFF
hex_cap    = 0xDEAD_BEEF
binary     = 0b1010_1010
octal      = 0o755
negative   = -42
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
plain      = "Hello, world!"
with_escape = "first\tsecond\nthird"
empty      = ""
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

#### When to use
Use f-strings whenever you need to embed variable values or expressions inside a string. They
are the preferred alternative to manual concatenation or `printf`-style format strings.

#### How it works

```python
name = "Tauraro"
msg = f"Hello, {name}!"
result = f"1 + 1 = {1 + 1}"
pi_msg = f"pi ≈ {3.14159}"
nested = f"value = {compute(x)}"
```

F-strings evaluate expressions inside `{}` at runtime and produce a formatted string. They
compile to the most efficient C representation — direct `printf` format strings with correct
format specifiers for each embedded type.

**What can go inside `{}`:**
- Variable names: `{x}`
- Arithmetic: `{a + b}`
- Function calls: `{len(items)}`
- Method calls: `{obj.method()}`

Nested f-strings are not supported. Build complex strings with `+`.

F-strings produce a heap-allocated string owned by the declaring scope.

#### Common Mistakes

```python
msg = f"result = {a / b}"    # OK if a and b are float; integer division if both are int
msg = f"result = {a as float / b}"   # explicit cast inside {} when needed
```

#### Best Practices

- Use f-strings over `+` concatenation for readability.
- Keep `{}` expressions simple — if the expression is complex, compute it first and put the
  variable in the f-string.

### Boolean Literals

```python
a = true
b = false
```

### None / Null

```python
mut ptr: str = none
mut obj: MyClass = none
```

`none` represents a null pointer. It is only valid for pointer types and optional types.
Assigning `none` to a value type is a compiler error:

```python
mut x: int = none   # ERROR [M-7]: cannot assign 'none' to 'x' which has type 'int'
```

---

## The `as` Cast Operator

### When to use
Use `as` for every explicit type conversion. It is mandatory — there is no implicit coercion
(Rule T-1). Use it for widening, narrowing, float-to-int, int-to-float, and pointer
reinterpretation (the last only inside `unsafe:`).

### How it works

```python
# Numeric widening
n: int = 42
f: float = n as float        # int → float

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

| From → To | Behavior |
|-----------|----------|
| int → float | Widening, exact for values up to 2⁵³ |
| float → int | Truncates toward zero |
| wider int → narrower int | Wraps (modulo 2ⁿ) |
| any numeric → bool | Non-zero → `true`, zero → `false` |
| int → char | Truncates to 8 bits |
| pointer → pointer | Only inside `unsafe:` |
| class → class | Not allowed; use interfaces for polymorphism |

### Common Mistakes

```python
val: i8 = 200 as i8    # wraps to -56 — no compile error, silent data loss
```

Narrowing casts never error at compile time even when the value will not fit. Add a bounds check
before the cast when the value is from user input or a calculation with unbounded range.

### Best Practices

- Cast at the boundary, not deep inside an expression.
- When narrowing, add a comment explaining the invariant that guarantees the value fits.
- Use `as bool` explicitly rather than relying on truthiness patterns from other languages.

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

**Indentation:** 4 spaces. Tabs are not allowed. Indentation is significant — it defines block
scope. The wrong indentation depth is a parse error, not a warning.

**Comments:** Use `#` for line comments:
```python
# This is a comment
x = 42  # inline comment
```

There are no block comments. Long explanations use multiple `#` lines.

---

Next: [Operators →](03_operators.md)
