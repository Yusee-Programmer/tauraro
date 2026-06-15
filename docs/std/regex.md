# std.regex — POSIX Regular Expressions

```tauraro
from std.regex import Regex
```

> **Platform note** — Uses POSIX extended regex (`regex.h`). Available on Linux, macOS, and MinGW/GCC on Windows.
> On MSVC without `<regex.h>`, all methods return safe no-op results.
> No external library is needed; the runtime header detects support automatically.

---

## Regex

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Regex.compile` | `(pattern: str) -> Regex` | `Regex` | Compile a POSIX extended regex pattern. |
| `Regex.compile_icase` | `(pattern: str) -> Regex` | `Regex` | Same, but case-insensitive matching. |
| `match` | `(self, text: str) -> bool` | `bool` | `true` when the entire `text` is matched by the pattern (use `contains`/`find` for partial matches). |
| `find_start` | `(self, text: str, from_: int) -> int` | `int` | Byte offset of first match at or after `from_`, or `-1`. |
| `find_len` | `(self, text: str, from_: int) -> int` | `int` | Byte length of first match at or after `from_`, or `0`. |
| `find` | `(self, text: str, from_: int) -> str` | `str` | Matched substring, or `""` when no match. |
| `find_all` | `(self, text: str) -> Vec[str]` | `Vec[str]` | All non-overlapping matched substrings. |
| `replace_first` | `(self, text: str, repl: str) -> str` | `str` | Replace the first match with `repl`. |
| `replace_all` | `(self, text: str, repl: str) -> str` | `str` | Replace every non-overlapping match with `repl`. |
| `count` | `(self, text: str) -> int` | `int` | Number of non-overlapping matches. |
| `contains` | `(self, text: str) -> bool` | `bool` | `true` when text contains at least one match. |
| `split` | `(self, text: str) -> Vec[str]` | `Vec[str]` | Split `text` on each match; returns the pieces between. |
| `free` | `(self)` | `void` | Release compiled regex resources. |

Fields: `pattern_: str`, `ignore_case: bool`.

---

## Example

```tauraro
from std.regex import Regex

# Simple match
mut re = Regex.compile("^[0-9]+$")
print(str(re.match("42")))     # true
print(str(re.match("hi")))     # false

# Find all words
mut words_re = Regex.compile("[A-Za-z]+")
mut words    = words_re.find_all("hello, world! foo")
mut i        = 0
while i < words.len:
    print(words.get(i))
    i = i + 1
# hello
# world
# foo

# Replace
mut email_re = Regex.compile("[a-z]+@[a-z]+\\.[a-z]+")
mut redacted = email_re.replace_all("contact me at alice@example.com or bob@test.org", "[email]")
print(redacted)   # contact me at [email] or [email]

# Split on whitespace
mut ws = Regex.compile("[ \\t]+")
mut parts = ws.split("one   two\tthree")
# parts = ["one", "two", "three"]

# Case-insensitive
mut ci = Regex.compile_icase("hello")
print(str(ci.match("HELLO")))  # true

re.free()
words_re.free()
email_re.free()
ws.free()
ci.free()
```
