# std.unicode — UTF-8 and Unicode

```tauraro
from std.unicode import Unicode
```

> Pure-C implementation — no external library required.
> All string methods use **codepoint indices** (not byte offsets).

---

## Unicode — String-level methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Unicode.len` | `(s: str) -> int` | `int` | Number of Unicode codepoints (not bytes). |
| `Unicode.valid` | `(s: str) -> bool` | `bool` | `true` when `s` is valid UTF-8. |
| `Unicode.char_at` | `(s: str, idx: int) -> int` | `int` | Codepoint at codepoint index `idx`. |
| `Unicode.slice` | `(s: str, start: int, end_: int) -> str` | `str` | Substring by codepoint range `[start, end_)`. |
| `Unicode.to_upper` | `(s: str) -> str` | `str` | Convert all codepoints to upper case. |
| `Unicode.to_lower` | `(s: str) -> str` | `str` | Convert all codepoints to lower case. |
| `Unicode.is_alpha` | `(s: str) -> bool` | `bool` | `true` when every codepoint is a letter. |
| `Unicode.is_digit` | `(s: str) -> bool` | `bool` | `true` when every codepoint is a decimal digit. |
| `Unicode.is_alnum` | `(s: str) -> bool` | `bool` | `true` when every codepoint is a letter or digit. |

## Unicode — Codepoint-level methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Unicode.is_letter` | `(cp: int) -> bool` | `bool` | `true` when `cp` is a Unicode letter. |
| `Unicode.is_digit_cp` | `(cp: int) -> bool` | `bool` | `true` when `cp` is a decimal digit codepoint. |
| `Unicode.upper` | `(cp: int) -> int` | `int` | Upper-case mapping of a single codepoint. |
| `Unicode.lower` | `(cp: int) -> int` | `int` | Lower-case mapping of a single codepoint. |
| `Unicode.category` | `(cp: int) -> str` | `str` | One-letter Unicode general category (see below). |

### Unicode categories

| Return | Meaning |
|---|---|
| `"L"` | Letter |
| `"N"` | Number |
| `"Z"` | Separator (space) |
| `"C"` | Control |
| `"P"` | Punctuation |
| `"S"` | Symbol |
| `"M"` | Mark (combining) |

---

## Example

```tauraro
from std.unicode import Unicode

mut s = "Héllo"
print(str(Unicode.len(s)))         # 5  (not 6 bytes)
print(str(Unicode.valid(s)))       # true

mut c = Unicode.char_at(s, 1)
print(str(c))                      # 233  (U+00E9 'é')

mut up = Unicode.to_upper(s)
print(up)                          # "HÉLLO"

mut sl = Unicode.slice(s, 1, 4)
print(sl)                          # "éll"

print(str(Unicode.is_alpha("café")))   # true
print(str(Unicode.is_digit("123")))    # true

print(Unicode.category(65))        # "L"  (A)
print(Unicode.category(48))        # "N"  (0)
print(Unicode.category(32))        # "Z"  (space)
```
