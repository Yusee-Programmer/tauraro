# 06 — Strings and F-Strings

---

## String Basics

A `str` in Tauraro is a C `char*` — a pointer to a null-terminated UTF-8 byte sequence. There are two categories:

| Category | Example | Storage | Freed? |
|----------|---------|---------|--------|
| String literal | `"hello"` | Read-only data segment | Never |
| Dynamic string | `f"..."`, `a + b` | Heap | At scope exit |

The compiler tracks which is which and injects `free()` only for heap strings.

---

## String Literals

```python
greeting = "Hello, world!"
empty    = ""
tab      = "column1\tcolumn2"
newline  = "line1\nline2"
path     = "C:\\Users\\Tauraro"
quoted   = "She said \"hello\""
```

### Escape Sequences

| Sequence | Character | Hex |
|----------|-----------|-----|
| `\n` | Line feed (newline) | `0x0A` |
| `\r` | Carriage return | `0x0D` |
| `\t` | Horizontal tab | `0x09` |
| `\\` | Backslash | `0x5C` |
| `\"` | Double quote | `0x22` |
| `\'` | Single quote | `0x27` |
| `\0` | Null byte | `0x00` |

---

## String Concatenation

```python
first = "Hello"
second = "world"
combined = first + ", " + second + "!"    # "Hello, world!"
```

String concatenation with `+` allocates a new heap string. Each `+` is a separate allocation. For building strings with many pieces, use an f-string or a series of appends — see [StringBuilder pattern](#stringbuilder-pattern) below.

**Compiler rule:** `+` on strings calls `_tr_str_concat(a, b)`, which allocates a new `char*` and copies both strings. The result is heap-owned and freed at scope exit.

---

## F-Strings

F-strings are the primary way to format output. Any expression can appear inside `{}`:

```python
name    = "Tauraro"
version = 2
score   = 98.5

print(f"Hello from {name} v{version}!")
print(f"Score: {score}")
print(f"1 + 1 = {1 + 1}")
print(f"length of name: {len(name)}")
```

### What Can Go Inside `{}`

```python
# Variable:
f"value = {x}"

# Arithmetic:
f"sum = {a + b}"

# Function call:
f"length = {len(items)}"

# Method call:
f"upper = {name.upper()}"

# Property access:
f"x = {point.x}"

# Boolean:
f"flag = {active}"

# Integer in hex (use explicit str() conversion):
f"hex = {str(n)}"    # the str() builtin converts any value to a string
```

### Nested Expressions

For complex expressions, assign to a variable first:

```python
# AVOID (hard to read):
f"result = {compute_complex_value(data, threshold, offset)}"

# BETTER:
mut result = compute_complex_value(data, threshold, offset)
f"result = {result}"
```

### How F-Strings Compile

The compiler analyzes the types of all embedded expressions and selects the correct C format specifier:

| Tauraro type | Format specifier | C output |
|---|---|---|
| `int` / `i64` | `%lld` | `printf("...%lld...", val)` |
| `i32` | `%d` | `printf("...%d...", val)` |
| `float` / `f64` | `%g` | `printf("...%g...", val)` |
| `str` | `%s` | `printf("...%s...", val)` |
| `bool` | `%s` | `printf("...%s...", val ? "true" : "false")` |
| `char` | `%c` | `printf("...%c...", val)` |

For `print(f"...")` specifically, the compiler may emit a direct `printf` call (no intermediate allocation). For f-strings assigned to variables, the compiler uses `_tr_format(...)` which allocates a heap string.

```python
# Direct printf (no allocation):
print(f"x = {x}, y = {y}")

# Allocates a heap string, assigns to 'msg':
mut msg = f"x = {x}, y = {y}"
```

---

## String Operations

### Length

```python
mut s = "Hello"
mut n = len(s)      # 5
```

`len(s)` compiles to `_tr_strlen(s)` (a `strlen` wrapper). O(n) — scans the string. For frequently-needed lengths, cache the result in a variable.

### Indexing (Character Access)

```python
mut s = "Hello"
mut first: char = s[0]      # 'H'
mut last: char  = s[4]      # 'o'
```

`s[i]` compiles to `s[i]` in C (direct array index). No bounds checking. Negative indices are not supported — use `s[len(s) - 1]` for the last character.

### String Methods

These are method calls on `str` values:

```python
mut s = "  Hello, World!  "

mut upper = s.upper()          # "  HELLO, WORLD!  "
mut lower = s.lower()          # "  hello, world!  "
mut stripped = s.strip()       # "Hello, World!"
mut found = s.find("World")    # 9  (index, or -1 if not found)
mut replaced = s.replace("World", "Tauraro")   # "  Hello, Tauraro!  "
mut parts = s.strip().split(",")               # List[str]: ["Hello", " World!"]
```

| Method | Returns | Description |
|--------|---------|-------------|
| `.upper()` | `str` | Convert to uppercase |
| `.lower()` | `str` | Convert to lowercase |
| `.strip()` | `str` | Remove leading and trailing whitespace |
| `.find(sub)` | `int` | Index of first occurrence, or −1 |
| `.replace(old, new)` | `str` | Replace all occurrences of `old` with `new` |
| `.split(sep)` | `List[str]` | Split into list of substrings at `sep` |

### Type-to-String Conversion

Use the `str()` builtin to convert any value to its string representation:

```python
n: int = 42
f: float = 3.14
b: bool = true

s1 = str(n)     # "42"
s2 = str(f)     # "3.14"
s3 = str(b)     # "true"
```

`str()` compiles to `_tr_int_to_str`, `_tr_float_to_str`, or `_tr_bool_to_str` depending on the argument type.

### String-to-Number Conversion

```python
n = int("42")           # 42   — parses string as integer
f = float("3.14")       # 3.14 — parses string as float
```

These call `_tr_str_to_int` and `_tr_str_to_float` in the runtime. No error handling — passing a non-numeric string produces 0.

For safe conversion, use a `throws` wrapper:
```python
def safe_int(s: str) throws str -> int:
    if len(s) == 0: raise("empty string")
    mut i = 0
    while i < len(s):
        mut c: int = s[i] as int
        if c < 48 or c > 57: raise(f"not a digit: {s}")
        i = i + 1
    return int(s)
```

### String Comparison

```python
a = "hello"
b = "world"

a == b         # false  (content comparison via strcmp)
a != b         # true
a < b          # true   (lexicographic: 'h' < 'w')
```

`==` and `!=` on strings compile to `strcmp(a, b) == 0` and `strcmp(a, b) != 0`.
`<`, `>`, `<=`, `>=` compile to `strcmp(a, b) < 0`, etc.

---

## StringBuilder Pattern

For building strings incrementally (e.g., in a loop), repeated `+` is inefficient because each `+` allocates a new string. Use a list of parts and join:

```python
def build_csv(items: List[int]) -> str:
    mut parts: List[str] = []
    mut i = 0
    while i < len(items):
        parts.append(str(items[i]))
        if i < len(items) - 1: parts.append(",")
        i = i + 1
    # join all parts into one string
    mut result = ""
    i = 0
    while i < len(parts):
        result = result + parts[i]
        i = i + 1
    return result
```

Or build directly with f-strings when the pattern is regular:

```python
def build_report(title: str, scores: List[int]) -> str:
    mut out = f"=== {title} ===\n"
    mut i = 0
    while i < len(scores):
        out = out + f"  [{i}] {scores[i]}\n"
        i = i + 1
    return out
```

---

## Raw Bytes Access

To access individual bytes of a string as integers:

```python
def count_char(s: str, target: char) -> int:
    mut count = 0
    mut i = 0
    while i < len(s):
        if s[i] as int == target as int:
            count = count + 1
        i = i + 1
    return count
```

`s[i]` gives you a `char`. Cast with `as int` to get the byte value (0–255).

For UTF-8 multi-byte sequences, you must handle them manually — Tauraro's `str` is a raw byte array with no built-in Unicode awareness.

---

## Hausa String Builtins

Tauraro provides Hausa-named equivalents for type conversion builtins:

```python
tsawon(s)       # same as len(s)   — "length" in Hausa
zuwa_rubutu(n)  # same as str(n)   — "to string" in Hausa
lamba(s)        # same as int(s)   — "number" in Hausa
```

These are accepted anywhere their English equivalents are.

---

## Common String Mistakes

### Forgetting `str()` in Concatenation

```python
n = 42
msg = "count: " + n       # ERROR: cannot concatenate str and int
msg = "count: " + str(n)  # OK
```

### Index Out of Bounds

```python
s = "hi"
c = s[5]    # undefined behavior — no bounds checking
```

**Fix:** Check bounds: `if i < len(s): c = s[i]`

### Null String

```python
mut s: str = none
print(len(s))    # undefined behavior — null pointer dereference
```

**Fix:** Always initialize string variables. Use `""` for an empty string, not `none`.

---

Next: [Collections →](07_collections.md)
