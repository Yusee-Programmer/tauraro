# 06 — Strings and F-Strings

A `str` in Tauraro is a pointer to a null-terminated UTF-8 byte sequence. The compiler distinguishes between string literals (stored in read-only memory, never freed) and dynamic strings (heap-allocated, freed automatically at scope exit). Understanding this distinction is the key to using strings correctly.

---

## Table of Contents

1. [String Basics](#string-basics)
2. [String Literals and Escape Sequences](#string-literals-and-escape-sequences)
3. [Raw Strings](#raw-strings)
4. [String Concatenation](#string-concatenation)
5. [F-Strings (Formatted Strings)](#f-strings-formatted-strings)
6. [String Operations and Methods](#string-operations-and-methods)
7. [Type Conversion](#type-conversion)
8. [String Comparison](#string-comparison)
9. [Character Access](#character-access)
10. [StringBuilder Pattern](#stringbuilder-pattern)
11. [Raw Byte Access](#raw-byte-access)
12. [Common String Mistakes](#common-string-mistakes)

---

## String Basics

### When to use

Use `str` for any text value: messages, names, file paths, command output, keys, labels. The `str` type covers both string literals and dynamically constructed strings — the compiler handles the difference in lifetime automatically.

### How it works

| Category | Example | Storage | Freed? |
|----------|---------|---------|--------|
| String literal | `"hello"` | Read-only (static) | Never |
| F-string | `f"Hello {name}"` | Heap | At scope exit |
| Concatenation result | `a + b` | Heap | At scope exit |

The compiler tracks which strings are static and which are heap-allocated. You never call `free()` on a string manually.

```python
mut greeting = "Hello, world!"    # static — never freed
mut name = "Tauraro"
mut msg = f"Hello, {name}!"       # heap — freed at end of scope
mut combined = "prefix_" + name   # heap — freed at end of scope
```

### Common Mistakes

**Assigning `none` to a `str`:**
```python
mut s: str = none
print(len(s))    # undefined behavior — null pointer dereference
```
Fix: Always initialize with `""` for an empty string, or a real value.

### Best Practices

- Default to string literals for constant text. Reserve f-strings and concatenation for values you need to compute.
- If you only need to pass a string to a function for output (e.g., `print`), use an f-string inline rather than assigning to a variable — the compiler avoids the heap allocation when the string is used directly.

---

## String Literals and Escape Sequences

### When to use

Use string literals for all constant text: error messages, labels, file paths, configuration keys, SQL queries, anything that does not change at runtime.

### How it works

```python
mut greeting = "Hello, world!"
mut empty    = ""
mut tab      = "column1\tcolumn2"
mut newline  = "line1\nline2"
mut path     = "C:\\Users\\Tauraro"
mut quoted   = "She said \"hello\""
mut null_sep = "key\0value"          # embedded null byte
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

### Common Mistakes

**Forgetting to escape backslashes in Windows paths:**
```python
mut path = "C:\Users\name"    # \U and \n are interpreted as escape sequences
```
Fix: `mut path = "C:\\Users\\name"` or use a raw string: `r"C:\Users\name"`

**Embedding a literal newline in a string literal:**
```python
mut s = "line one
line two"    # ERROR: string literals must not span physical lines
```
Fix: Use `\n`: `mut s = "line one\nline two"`

### Best Practices

- Use raw strings (`r"..."`) for Windows paths, regular expressions, and any string that contains many backslashes.
- For multi-line output, build the string with `+` and `\n` rather than creating separate strings:
  ```python
  mut report = "Header\n" + "Body\n" + "Footer\n"
  ```

---

## Triple-Quoted Strings

### When to use

Use triple-quoted strings (`"""..."""`) for multi-line content: SQL queries, multi-line messages, embedded text blocks, and any string that would require many `\n` escapes in a regular literal.

### How it works

Delimit the string with `"""` on both ends. Newlines and any other characters inside are included literally:

```python
mut sql = """
SELECT id, name, email
FROM users
WHERE active = 1
  AND created_at > '2024-01-01'
ORDER BY name
"""

mut banner = """
+-------------------------------+
|  Welcome to Tauraro v2        |
+-------------------------------+
"""
```

The string includes all characters between the opening and closing `"""`, including newlines. There is no automatic trimming or indentation stripping — what you type is what you get.

### Common Mistakes

**Using triple-quoted strings for single-line values:** Regular `"..."` is cleaner for single-line content.

**Forgetting that the first newline is included:**
```python
mut s = """
line1
"""
print(s)    # prints an empty line, then "line1", then a newline
```
If you want to skip the leading newline, start the content immediately after `"""`:
```python
mut s = """line1
line2"""
```

---

## Raw Strings

### When to use

Use raw strings when the content contains many backslashes and you do not want escape processing — Windows paths, regular expression patterns, file format templates.

### How it works

Prefix the opening quote with `r`:

```python
mut path  = r"C:\Users\Tauraro\Documents"    # backslashes are literal
mut regex = r"\d+\.\d+"                       # no need to double-escape
mut tmpl  = r"INSERT INTO table (col) VALUES (?)"
```

No escape sequences are processed inside `r"..."`. What you type is what you get.

### Common Mistakes

**Trying to include a literal double-quote inside a raw string:**
```python
mut s = r"She said "hello""    # ERROR: raw string ends at the first unescaped "
```
Fix: Use a regular string with `\"`, or concatenate: `r"She said " + "\"hello\""`

---

## String Concatenation

### When to use

Use `+` to join a small, fixed number of strings. For building strings in a loop or combining more than four parts, use the [StringBuilder pattern](#stringbuilder-pattern) or an f-string.

### How it works

```python
mut first    = "Hello"
mut second   = "world"
mut combined = first + ", " + second + "!"    # "Hello, world!"
```

Each `+` operator allocates a new heap string. The operands must both be `str` — numbers must be converted with `str()` first:

```python
mut n   = 42
mut msg = "count: " + str(n)    # "count: 42"
```

### Common Mistakes

**Concatenating a number directly:**
```python
mut n   = 42
mut msg = "count: " + n    # ERROR: cannot concatenate str and int
```
Fix: `"count: " + str(n)` or `f"count: {n}"`

**Building strings in a loop with repeated `+`:**
```python
mut result = ""
for x in items:
    result = result + str(x) + ","    # O(n^2) — each + allocates a new string
```
Fix: Use the [StringBuilder pattern](#stringbuilder-pattern) or a single f-string.

### Best Practices

- Prefer f-strings over `+` for readability: `f"{first}, {second}!"` is cleaner than `first + ", " + second + "!"`.
- Use `+` when you have at most two or three string pieces, and when the strings are already variables.

---

## F-Strings (Formatted Strings)

### When to use

F-strings are the primary way to format output in Tauraro. Use them whenever you need to embed a computed value into a string — debug output, user messages, log lines, reports.

### How it works

Prefix the opening quote with `f`. Any expression inside `{}` is evaluated and converted to its string representation:

```python
mut name    = "Tauraro"
mut version = 2
mut score   = 98.5

print(f"Hello from {name} v{version}!")
print(f"Score: {score}")
print(f"1 + 1 = {1 + 1}")
print(f"name length: {len(name)}")
```

**What can go inside `{}`:**

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
f"x-coord = {point.x}"

# Boolean:
f"active = {is_active}"

# Type conversion:
f"hex equivalent = {str(n)}"
```

**Assignment vs. inline use:**

```python
# Inline — no heap allocation (compiler optimizes directly to output):
print(f"Hello, {name}!")

# Assigned — heap-allocated string, freed at scope exit:
mut msg = f"Hello, {name}!"
print(msg)
```

When you only need the string for immediate use (e.g., passing to `print`), the inline form is preferred for performance.

**Breaking up complex expressions for readability:**

```python
# Hard to read:
print(f"result = {compute_complex_value(data, threshold, offset)}")

# Better — assign first:
mut result = compute_complex_value(data, threshold, offset)
print(f"result = {result}")
```

### Common Mistakes

**Nesting f-strings:**
```python
mut outer = f"outer {f"inner {x}"}"    # ERROR: nested f-strings not supported
```
Fix: Assign the inner f-string to a variable first:
```python
mut inner = f"inner {x}"
mut outer = f"outer {inner}"
```

**Using `{}` with no expression:**
```python
f"value = {}"    # ERROR: empty expression in f-string
```

**Expecting format specifiers (like Python's `:.2f`):**
```python
f"score = {score:.2f}"    # ERROR: format specifiers not supported
```
Fix: Use a helper function or manual formatting:
```python
def fmt_float(f: float, decimals: int) -> str: ...
f"score = {fmt_float(score, 2)}"
```

### Best Practices

- Use f-strings for any string that contains a computed value — they are more readable and safer than manual concatenation.
- For long multi-line output, build incrementally:
  ```python
  mut out  = f"=== {title} ===\n"
  mut i    = 0
  while i < len(items):
      out = out + f"  [{i}] {items[i]}\n"
      i = i + 1
  ```
- Never nest f-strings. Assign intermediate values to named variables.

---

## String Operations and Methods

### When to use

String methods are built into the `str` type — no import needed. Use built-in operations (`len`, indexing, method calls) without any import.

### How it works

**Length:**

```python
mut s = "Hello, Tauraro"
mut n = len(s)     # 14 — built-in function
mut m = s.len()    # 14 — method form
```

`len(s)` is O(n) — it scans the null-terminated string. Cache the result if you need it multiple times.

**String methods (no import required):**

```python
mut s = "  Hello, World!  "

mut upper     = s.upper()                      # "  HELLO, WORLD!  "
mut lower     = s.lower()                      # "  hello, world!  "
mut stripped  = s.strip()                      # "Hello, World!"
mut found     = s.find("World")                # 9 (index), or -1 if not found
mut replaced  = s.replace("World", "Tauraro")  # "  Hello, Tauraro!  "
mut parts     = s.strip().split(",")           # List[str]: ["Hello", " World!"]
mut has_hello = s.contains("Hello")            # true
mut starts    = s.starts_with("  Hello")       # true
mut ends      = s.ends_with("  ")              # true
```

**Method reference:**

| Method | Returns | Description |
|--------|---------|-------------|
| `.upper()` or `.to_upper()` | `str` | Convert all characters to uppercase |
| `.lower()` or `.to_lower()` | `str` | Convert all characters to lowercase |
| `.strip()` or `.trim()` | `str` | Remove leading and trailing ASCII whitespace |
| `.find(sub)` | `int` | Index of first occurrence of `sub`, or −1 |
| `.replace(old, new)` | `str` | Replace all occurrences of `old` with `new` |
| `.split(sep)` | `List[str]` | Split on separator `sep`, return list of parts |
| `.contains(sub)` | `bool` | True if `sub` appears anywhere in the string |
| `.starts_with(prefix)` | `bool` | True if string starts with `prefix` |
| `.ends_with(suffix)` | `bool` | True if string ends with `suffix` |
| `.len()` | `int` | String length in bytes |
| `.index_of(sub)` | `int` | Same as `.find(sub)` |
| `.reverse()` | `str` | Return reversed string |
| `.repeat(n)` | `str` | Return string repeated `n` times |
| `.capitalize()` | `str` | First char upper, rest lower |

**Joining a list of strings:**

```python
mut parts = "a,b,c".split(",")        # ["a", "b", "c"]
mut joined = Str.join(parts, "-")     # "a-b-c"
```

### Common Mistakes

**Assuming `.find()` returns a boolean:**
```python
if s.find("hello"):    # WRONG: find() returns int (-1 means not found, 0 is valid!)
    ...
```
Fix: `if s.find("hello") != -1:`

**Using `.split()` result without checking length:**
```python
mut parts = s.split(",")
mut first = parts[0]    # OK only if s is non-empty and has a comma
mut second = parts[1]   # may be out of bounds if there is no comma
```
Fix: Check `len(parts) > 1` before accessing `parts[1]`.

### Best Practices

- Cache `len(s)` in a variable when you need it inside a loop — each call is O(n).
- Chain method calls for readability: `s.strip().lower().replace("-", "_")`
- For searching, always compare `.find()` against `!= -1`, never use it as a truthy/falsy value.

---

## Type Conversion

### When to use

Use `str()` to convert any value to its string representation. Use `int()` and `float()` to parse numeric strings.

### How it works

**Value to string:**

```python
mut n: int   = 42
mut f: float = 3.14
mut b: bool  = true

mut s1 = str(n)    # "42"
mut s2 = str(f)    # "3.14"
mut s3 = str(b)    # "true"
```

**String to number:**

```python
mut n = int("42")       # 42   — parses decimal integer
mut f = float("3.14")   # 3.14 — parses floating point
```

Note: `int()` and `float()` return 0 on invalid input — they do not raise an error. Use a `throws` wrapper for safe parsing:

```python
def safe_int(s: str) throws str -> int:
    if len(s) == 0:
        raise("empty string")
    mut i = 0
    while i < len(s):
        mut c: int = s[i] as int
        if c < 48 or c > 57:
            raise(f"not a digit at position {i}: '{s}'")
        i = i + 1
    return int(s)
```

**Boolean to string:**

```python
mut flag = true
mut s    = str(flag)    # "true"
```

### Common Mistakes

**Relying on `int()` to signal parse failure:**
```python
mut n = int("abc")    # returns 0, not an error
if n == 0:
    print("parse failed")    # WRONG: "0" also returns 0
```
Fix: Write a `throws`-based parser or validate the input before calling `int()`.

**Converting an integer in an f-string without `str()`:**
```python
mut n   = 42
mut msg = "answer is " + n    # ERROR: type mismatch str + int
```
Fix: `"answer is " + str(n)` or just `f"answer is {n}"`

### Best Practices

- Use f-strings for embedding numbers in strings rather than explicit `str()` calls — it is more readable.
- For user input that may be malformed, always use a `throws`-based parser instead of raw `int()`/`float()`.

---

## String Comparison

### When to use

Use `==`, `!=`, `<`, `>`, `<=`, `>=` to compare strings. All comparisons operate on the string content, not on pointer identity.

### How it works

```python
mut a = "hello"
mut b = "world"

mut eq  = (a == b)    # false  (byte-by-byte content comparison)
mut neq = (a != b)    # true
mut lt  = (a < b)     # true   (lexicographic: 'h' < 'w')
mut gt  = (a > b)     # false
```

Comparison is byte-by-byte (`strcmp` semantics). Lexicographic order follows ASCII byte values.

**Case-insensitive comparison:**
```python
import std.string
mut same = (a.lower() == b.lower())
```

### Common Mistakes

**Comparing string pointer identity (not meaningful in Tauraro):**
```python
mut a = "hello"
mut b = "hello"
# a == b is content comparison — always correct, even if pointers differ
```
This is actually correct behavior in Tauraro. Unlike C, `==` on strings always compares content.

**Comparing strings with integer-style operators expecting numeric ordering:**
```python
mut a = "9"
mut b = "10"
mut lt = (a < b)    # true by lexicographic order? No: "9" > "10" lexicographically ('9' > '1')
```
Fix: Parse to `int` before comparing numeric strings: `int(a) < int(b)`

### Best Practices

- Always use `==` for string equality — it compares content, as expected.
- For case-insensitive comparison, normalise both sides with `.lower()` before comparing.
- For numeric string comparison, parse to `int` or `float` first.

---

## Character Access

### When to use

Use indexing to examine individual characters — for parsing protocols, validating input byte-by-byte, or processing ASCII data.

### How it works

```python
mut s     = "Hello"
mut first: char = s[0]      # 'H'
mut third: char = s[2]      # 'l'
mut last: char  = s[len(s) - 1]    # 'o'  (negative indexing not supported)
```

`s[i]` gives a `char`. Cast to `int` to get the byte value:

```python
mut c: char = s[0]
mut byte    = c as int    # 72 (ASCII for 'H')
```

**No automatic bounds checking.** Accessing beyond the string length is undefined behavior:

```python
mut s = "hi"
mut c = s[5]    # undefined behavior — out of bounds
```

Always check bounds manually when the index is not statically known:

```python
if i >= 0 and i < len(s):
    mut c = s[i]
```

### Common Mistakes

**Using negative indexing:**
```python
mut last = s[-1]    # NOT supported — undefined behavior
```
Fix: `mut last = s[len(s) - 1]`

**Not checking bounds on user-provided index:**
```python
def char_at(s: str, i: int) -> char:
    return s[i]    # unsafe if i is out of range
```
Fix:
```python
def char_at(s: str, i: int) -> char:
    if i < 0 or i >= len(s): return '\0'
    return s[i]
```

### Best Practices

- Treat string indexing as a low-level operation. For higher-level parsing, consider splitting on delimiters or using string methods.
- Always validate an index before using it if it comes from user input or a computation.

---

## StringBuilder Pattern

### When to use

When building a string from many pieces — especially inside a loop — avoid repeated `+` concatenation because each `+` allocates a new heap string (O(n^2) total work). Use the StringBuilder pattern instead.

### How it works

Collect parts in a `List[str]`, then join:

```python
def build_csv(items: List[int]) -> str:
    mut parts: List[str] = []
    mut i = 0
    while i < len(items):
        parts.append(str(items[i]))
        if i < len(items) - 1:
            parts.append(",")
        i = i + 1
    # join all parts into a single string
    mut result = ""
    i = 0
    while i < len(parts):
        result = result + parts[i]
        i = i + 1
    return result
```

Or, when the template is regular, accumulate with f-strings:

```python
def build_report(title: str, scores: List[int]) -> str:
    mut out = f"=== {title} ===\n"
    mut i   = 0
    while i < len(scores):
        out = out + f"  [{i}] {scores[i]}\n"
        i = i + 1
    return out
```

The second approach still does O(n) allocations, but each allocation is proportional to the partial string size, which is acceptable for most use cases. For truly large strings (thousands of lines), the `List[str]`-then-join pattern is more efficient.

### Common Mistakes

**Concatenating in a loop naively:**
```python
mut result = ""
for x in big_list:
    result = result + str(x)    # O(n^2) — avoid for large lists
```

### Best Practices

- For up to ~10 pieces: use f-strings or direct `+`.
- For 10–1000 pieces in a loop: accumulate with f-string `+` on a `mut` variable.
- For very large output: collect into `List[str]` and join at the end.

---

## Raw Byte Access

### When to use

Access string bytes directly when implementing low-level protocols, checksums, UTF-8 parsing, or any operation that needs to inspect individual byte values.

### How it works

```python
def count_char(s: str, target: char) -> int:
    mut count = 0
    mut i     = 0
    while i < len(s):
        if s[i] as int == target as int:
            count = count + 1
        i = i + 1
    return count

def is_ascii_digit(c: char) -> bool:
    mut b = c as int
    return b >= 48 and b <= 57    # '0' = 48, '9' = 57

def hex_nibble(c: char) -> int:
    mut b = c as int
    if b >= 48 and b <= 57:  return b - 48          # '0'-'9'
    if b >= 65 and b <= 70:  return b - 55          # 'A'-'F'
    if b >= 97 and b <= 102: return b - 87          # 'a'-'f'
    return -1
```

`s[i]` gives a `char`; `char as int` gives the raw byte value (0–255).

**UTF-8 note:** `str` is a raw byte array. Multi-byte UTF-8 sequences are not automatically decoded. If your string may contain non-ASCII characters, iterate byte indices carefully and handle continuation bytes (bytes with value 128–191) appropriately.

### Common Mistakes

**Comparing a `char` directly to an integer:**
```python
if s[i] == 65:    # ERROR: char cannot be compared to int directly
```
Fix: `if s[i] as int == 65:`  or  `if s[i] == 'A':`

### Best Practices

- Define named constants for frequently used byte values:
  ```python
  mut NEWLINE: int = 10
  mut SPACE:   int = 32
  ```
- For UTF-8 processing, always document whether the function handles multi-byte characters or ASCII-only.

---

## Common String Mistakes

### Forgetting `str()` in Concatenation

```python
mut n   = 42
mut msg = "count: " + n       # ERROR: cannot concatenate str and int
mut msg = "count: " + str(n)  # OK
```

### Index Out of Bounds

```python
mut s = "hi"
mut c = s[5]    # undefined behavior — no bounds check
```
Fix: `if i < len(s): c = s[i]`

### Null String Dereference

```python
mut s: str = none
print(len(s))    # undefined behavior — null pointer dereference
```
Fix: Initialise with `""` or a real value. Never leave `str` as `none`.

### Comparing Numeric Strings Lexicographically

```python
mut a = "9"
mut b = "10"
if a < b:    # "9" > "10" in lexicographic order — this is false, probably not what you want
```
Fix: `if int(a) < int(b):`

### Ignoring UTF-8 Multi-byte Characters

```python
mut s = "café"
mut n = len(s)    # returns byte count (5), not character count (4)
```
`len()` counts bytes, not Unicode codepoints. For byte-level processing this is correct. For character counting with non-ASCII text, implement a UTF-8 aware counter manually.

---

Next: [Collections →](07_collections.md)
