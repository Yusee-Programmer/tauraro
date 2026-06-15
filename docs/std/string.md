# std.string — String Utilities and Formatting

```tauraro
from std.string.str import Str   # string utilities (static methods)
from std.string.fmt import Fmt   # number/value formatting (static methods)
```

All methods are **static** — called as `Str.method(...)` or `Fmt.method(...)`.  
`str` values are reference-counted, immutable-from-the-outside UTF-8 byte sequences; every `Str`/`Fmt` operation returns a freshly allocated string rather than mutating its input. If you need to build a string incrementally (e.g. in a loop), use `StringBuilder` from `std.core.string` — see [StringBuilder](#stdcorestring--stringbuilder) below — instead of repeated `+` concatenation.

---

## std.string.str — Str class

**When**: Any string inspection, transformation, or parsing task.
**Why**: A single import gives you length, predicates, slicing, case, trimming, padding, search, split, join, format — without managing individual helper imports.

### Measurement

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.len` | `(s: str) -> int` | `int` | Length in bytes. |

### Predicates

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.starts_with` | `(s: str, prefix: str) -> bool` | `bool` | `true` if `s` begins with `prefix`. |
| `Str.ends_with` | `(s: str, suffix: str) -> bool` | `bool` | `true` if `s` ends with `suffix`. |
| `Str.contains` | `(s: str, sub: str) -> bool` | `bool` | `true` if `sub` appears anywhere in `s`. |
| `Str.contains_char` | `(s: str, c: int) -> bool` | `bool` | `true` if character with ASCII code `c` appears in `s`. |
| `Str.eq` | `(a: str, b: str) -> bool` | `bool` | Byte-by-byte equality (same as `a == b`). |

### Character classification

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.is_digit` | `(s: str) -> bool` | `bool` | `true` when every byte is `'0'–'9'`. |
| `Str.is_alpha` | `(s: str) -> bool` | `bool` | `true` when every byte is an ASCII letter. |
| `Str.is_alnum` | `(s: str) -> bool` | `bool` | `true` when every byte is a letter or digit. |
| `Str.is_space` | `(s: str) -> bool` | `bool` | `true` when every byte is whitespace. |
| `Str.is_upper` | `(s: str) -> bool` | `bool` | `true` when no lowercase letters are present. |
| `Str.is_lower` | `(s: str) -> bool` | `bool` | `true` when no uppercase letters are present. |

### Slicing and construction

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.slice` | `(s: str, start: int, end: int) -> str` | `str` | Byte slice `[start, end)`. Clamps to valid range. |
| `Str.repeat` | `(s: str, n: int) -> str` | `str` | Concatenate `s` with itself `n` times. |
| `Str.reverse` | `(s: str) -> str` | `str` | Reverse the byte sequence. |

### Case conversion

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.to_upper` | `(s: str) -> str` | `str` | ASCII uppercase. |
| `Str.to_lower` | `(s: str) -> str` | `str` | ASCII lowercase. |
| `Str.capitalize` | `(s: str) -> str` | `str` | First letter uppercase, rest lowercase. |
| `Str.title` | `(s: str) -> str` | `str` | Capitalize the first letter of each word. |

### Trimming

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.trim` | `(s: str) -> str` | `str` | Remove leading and trailing whitespace. |
| `Str.trim_left` | `(s: str) -> str` | `str` | Remove leading whitespace only. |
| `Str.trim_right` | `(s: str) -> str` | `str` | Remove trailing whitespace only. |

### Character access

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.char_at` | `(s: str, i: int) -> int` | `int` | ASCII code of the byte at index `i`. |

### Padding and alignment

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.lpad` | `(s: str, width: int, pad_char: int) -> str` | `str` | Left-pad `s` to `width` using `pad_char` (ASCII code). |
| `Str.rpad` | `(s: str, width: int, pad_char: int) -> str` | `str` | Right-pad `s` to `width`. |
| `Str.center` | `(s: str, width: int) -> str` | `str` | Center `s` within `width` using spaces. |

### Search

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.index_of` | `(s: str, sub: str) -> int` | `int` | First byte index of `sub` in `s`, or `-1` if not found. |
| `Str.last_index_of` | `(s: str, sub: str) -> int` | `int` | Last byte index of `sub` in `s`, or `-1` if not found. |
| `Str.count` | `(s: str, sub: str) -> int` | `int` | Number of non-overlapping occurrences of `sub`. |

### Split, join, replace

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.split` | `(s: str, sep: str) -> List[str]` | `List[str]` | Split on every occurrence of `sep`. Returns a C-backed `List[str]`. |
| `Str.split_to_vec` | `(s: str, sep: str) -> Vec[str]` | `Vec[str]` | Same split logic, but returns `Vec[str]` — compatible with `FloatTransform`, `Regex.split`, etc. |
| `Str.join` | `(parts: List[str], sep: str) -> str` | `str` | Join `parts` with `sep` between each adjacent pair. |
| `Str.replace` | `(s: str, old: str, new_: str) -> str` | `str` | Replace **all** occurrences of `old` with `new_`. |
| `Str.replace_first` | `(s: str, old: str, new_: str) -> str` | `str` | Replace only the **first** occurrence. |

### Template formatting

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.format` | `(template: str, values: Vec[str]) -> str` | `str` | Replace each `{}` placeholder in `template` with successive values. |

### Parsing

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.parse_int` | `(s: str) -> int` | `int` | Parse a decimal integer string. Accepts leading `'-'` or `'+'`. Returns `0` on empty or non-numeric input. |
| `Str.parse_float` | `(s: str) -> float` | `float` | Parse a decimal float string (integer + optional fractional part). |
| `Str.parse_bool` | `(s: str) -> bool` | `bool` | `true` for `"true"`, `"1"`, or `"yes"`; `false` otherwise. |

### Line and word splitting

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.lines` | `(s: str) -> Vec[str]` | `Vec[str]` | Split on `'\n'`. A trailing newline does not produce an empty trailing element. |
| `Str.words` | `(s: str) -> Vec[str]` | `Vec[str]` | Split on runs of whitespace (space, tab, CR, LF). |

### Prefix and suffix removal

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Str.strip_prefix` | `(s: str, prefix: str) -> str` | `str` | Return `s` with `prefix` stripped, or `s` unchanged if it does not start with `prefix`. |
| `Str.strip_suffix` | `(s: str, suffix: str) -> str` | `str` | Return `s` with `suffix` stripped, or `s` unchanged if it does not end with `suffix`. |
| `Str.remove_char` | `(s: str, c: int) -> str` | `str` | Return `s` with every occurrence of character `c` (ASCII code) removed. |

### Example

```tauraro
from std.string.str import Str
from std.core.vec   import Vec

mut s = "  Hello, World!  "
print(Str.trim(s))                    # "Hello, World!"
print(Str.to_upper("hello"))          # "HELLO"
print(Str.capitalize("hello world"))  # "Hello world"
print(Str.title("hello world"))       # "Hello World"

print(str(Str.len("abc")))            # 3
print(str(Str.index_of("abcabc", "bc")))   # 1
print(str(Str.last_index_of("abcabc", "bc")))  # 4
print(str(Str.count("banana", "an")))  # 2

mut parts = Str.split("a,b,c", ",")   # ["a", "b", "c"]
print(Str.join(parts, " - "))          # "a - b - c"

print(Str.slice("abcdef", 2, 5))      # "cde"
print(Str.lpad("7", 3, 48))           # "007"  (ASCII 48 = '0')
print(Str.center("hi", 6))            # "  hi  "

print(str(Str.is_digit("123")))       # true
print(str(Str.is_alpha("abc")))       # true

# Template formatting
mut vals = Vec[str].init(3)
vals.push("Alice")
vals.push("30")
mut msg = Str.format("Name: {}, Age: {}", vals)  # "Name: Alice, Age: 30"
print(msg)

# Parsing
print(str(Str.parse_int("-42")))          # -42
print(str(Str.parse_float("3.14")))       # 3.14
print(str(Str.parse_bool("true")))        # true

# Lines / words
mut ls = Str.lines("one\ntwo\nthree")     # ["one", "two", "three"]
mut ws = Str.words("  hello   world  ")   # ["hello", "world"]
print(str(ls.len()))   # 3
print(str(ws.len()))   # 2

# Prefix / suffix removal
print(Str.strip_prefix("foobar", "foo"))  # "bar"
print(Str.strip_suffix("foobar", "bar"))  # "foo"
print(Str.remove_char("hello", 108))      # "heo"  (108 = 'l')
```

---

## std.string.fmt — Fmt class

**When**: Converting numbers to strings in various bases, padding output columns, or building formatted strings.
**Why**: A single class with consistent naming; no `sprintf` or format specifiers needed.

### Integer → string

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Fmt.int_to_str` | `(n: int) -> str` | `str` | Decimal representation (handles negatives). |
| `Fmt.int_to_hex` | `(n: int) -> str` | `str` | Lowercase hexadecimal (no `0x` prefix). |
| `Fmt.int_to_bin` | `(n: int) -> str` | `str` | Binary representation (no `0b` prefix). |
| `Fmt.int_to_oct` | `(n: int) -> str` | `str` | Octal representation (no `0o` prefix). |

### Float → string

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Fmt.float_to_str` | `(f: float, decimals: int) -> str` | `str` | Decimal with the given number of decimal places. |

### Bool → string

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Fmt.bool_to_str` | `(b: bool) -> str` | `str` | `"true"` or `"false"`. |

### Padding

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Fmt.pad_left` | `(s: str, width: int, pad_char: int) -> str` | `str` | Left-pad `s` to `width` using `pad_char` (ASCII code). |
| `Fmt.pad_right` | `(s: str, width: int, pad_char: int) -> str` | `str` | Right-pad `s` to `width`. |
| `Fmt.zero_pad` | `(n: int, width: int) -> str` | `str` | Decimal `n` left-padded with `'0'` to `width` characters. |

### Template formatting

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Fmt.format` | `(template: str, args: Vec[str]) -> str` | `str` | Replace each `{}` placeholder in `template` with successive values from `args`. |

### Example

```tauraro
from std.string.fmt import Fmt
from std.core.vec   import Vec

print(Fmt.int_to_str(255))           # "255"
print(Fmt.int_to_hex(255))           # "ff"
print(Fmt.int_to_bin(10))            # "1010"
print(Fmt.int_to_oct(8))             # "10"
print(Fmt.zero_pad(7, 4))            # "0007"
print(Fmt.float_to_str(3.14159, 2))  # "3.14"
print(Fmt.bool_to_str(true))         # "true"
print(Fmt.pad_left("hi", 6, 32))     # "    hi"  (space = ASCII 32)

mut args = Vec[str].init(3)
args.push(Fmt.int_to_str(42))
args.push(Fmt.float_to_str(9.81, 2))
mut msg = Fmt.format("Answer: {}, g: {}", args)
print(msg)   # "Answer: 42, g: 9.81"
```

---

## std.core.string — StringBuilder

**When**: Building a string incrementally (e.g. inside a loop) — `Str`/`Fmt` always allocate a new string per call, so chained `+` concatenation in a loop is O(n²). `StringBuilder` amortizes growth like a growable buffer.
**Why**: All of `Str`'s and `Fmt`'s own implementations (`slice`, `to_upper`, `format`, `int_to_str`, etc.) are built on `StringBuilder` internally — it's the same tool available to your code.

```tauraro
from std.core.string import StringBuilder

mut sb = StringBuilder.init(16)
sb.append("Hello, ")
sb.append("World")
sb.append_char(33)          # '!'
mut out = sb.to_owned()      # heap str, independent of sb
sb.free()
print(out)                    # "Hello, World!"
```

| Method | Signature | Returns | Description |
|---|---|---|---|
| `StringBuilder.init` | `(initial_capacity: int) -> StringBuilder` | `StringBuilder` | Create a builder with at least `initial_capacity` bytes reserved (minimum 16). |
| `append` | `(self, s: str)` | `void` | Append the bytes of `s`, growing the buffer if needed. |
| `append_char` | `(self, c: int)` | `void` | Append a single byte with ASCII code `c`. |
| `append_int` | `(self, n: int)` | `void` | Append the decimal representation of `n` (handles negatives). |
| `append_float` | `(self, f: float)` | `void` | Append `f.to_str()`. |
| `len` | `(self) -> int` | `int` | Number of bytes written so far. |
| `as_str` | `(self) -> str` | `str` | Borrowed view of the builder's current contents — valid only while `sb` is alive and not mutated further. |
| `to_owned` | `(self) -> str` | `str` | Allocate and return a new, independent `str` copy of the current contents. **Prefer this** when returning the result from a function. |
| `to_string` | `(self) -> StringObj` | `StringObj` | Allocate a new `StringObj` (heap-owned string view) wrapping a copy of the current contents. |
| `clear` | `(self)` | `void` | Reset length to 0 without freeing the underlying buffer (keeps capacity). |
| `free` | `(self)` | `void` | Release the builder's internal buffer and the builder itself. Call when done with `sb`. |

> **`as_str()` vs `to_owned()`**: `as_str()` returns a string that aliases the builder's internal buffer — it becomes invalid once `sb.free()` runs or `sb` is mutated again. `to_owned()` copies the data into a new allocation that you own independently, which is almost always what you want when returning a built string from a function (the pattern used throughout `Str`/`Fmt`: `mut out = sb.to_owned(); sb.free(); return out`).

### Example

```tauraro
from std.core.string import StringBuilder

def build_csv(nums: Vec[int]) -> str:
    mut sb = StringBuilder.init(64)
    mut i  = 0
    while i < nums.len():
        if i > 0: sb.append(",")
        sb.append_int(nums.get(i))
        i = i + 1
    mut out = sb.to_owned()
    sb.free()
    return out
```
