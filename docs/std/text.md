# std.text — Diff, Edit Distance, and Text Wrapping

```tauraro
from std.text.diff import Diff, DiffOp        # Myers line diff + unified_diff rendering
from std.text.distance import Distance        # Levenshtein edit distance + similarity
from std.text.wrap import Wrap                # greedy word-wrap + indent
```

`std.text` is a sibling of [`std.string`](string.md), not a replacement for it: all
three modules here build on `Str`'s existing split/slice/trim helpers rather than
reimplementing basic string operations. `Str.split`/`Str.lines` turn raw text into
`List[str]`/`Vec[str]` line arrays; `std.text` is what you reach for once you have
those line arrays (or plain strings) and want to *compare* or *lay out* them.

All methods are **static** — called as `Diff.method(...)`, `Distance.method(...)`,
or `Wrap.method(...)`.

---

## std.text.diff — Diff class

**When**: Comparing two versions of line-based text (source files, config files,
generated output in a test) and either inspecting what changed programmatically,
or rendering a human-readable diff.
**Why**: Implements the same Myers O(ND) shortest-edit-script algorithm used by
real `diff`/`git diff` — the edit script is provably minimal, not just "a" diff
that happens to transform `a` into `b`.

### DiffOp — data-carrying enum

| Variant | Fields | Meaning |
|---|---|---|
| `DiffOp.Equal` | `line: str` | Line present, unchanged, in both inputs. |
| `DiffOp.Insert` | `line: str` | Line present only in `b` (added). |
| `DiffOp.Delete` | `line: str` | Line present only in `a` (removed). |

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Diff.diff` | `(a: List[str], b: List[str]) -> List[DiffOp]` | `List[DiffOp]` | Minimal edit script transforming `a` into `b` (Myers algorithm). |
| `Diff.unified_diff` | `(a: List[str], b: List[str], context: int) -> str` | `str` | Classic unified-diff text (`---`/`+++`/`@@ ... @@` format), with `context` lines of unchanged context around each change. File labels default to `"a"`/`"b"`. Returns `""` if `a` and `b` are identical. |
| `Diff.unified_diff_named` | `(a: List[str], b: List[str], name_a: str, name_b: str, context: int) -> str` | `str` | Same as `unified_diff`, with custom `---`/`+++` file labels. |

### Example

```tauraro
from std.string.str import Str
from std.text.diff import Diff, DiffOp

mut a = Str.split("one\ntwo\nthree", "\n")
mut b = Str.split("one\nTWO\nthree", "\n")

# Inspect the edit script directly:
mut ops = Diff.diff(a, b)
mut i = 0
while i < len(ops):
    match ops[i]:
        case DiffOp.Equal(line):  print("  " + line)
        case DiffOp.Delete(line): print("- " + line)
        case DiffOp.Insert(line): print("+ " + line)
    i = i + 1

# Or render the classic unified-diff text:
print(Diff.unified_diff(a, b, 3))
```

Output of the `unified_diff` call:

```
--- a
+++ b
@@ -1,3 +1,3 @@
 one
-two
+TWO
 three
```

---

## std.text.distance — Distance class

**When**: Fuzzy string matching — "did you mean...?" suggestions, deduplicating
near-identical entries, ranking search results by closeness.
**Why**: Classic Levenshtein DP (`O(n*m)` time/space), plus a normalized
`similarity` score built on top so callers don't have to normalize by hand.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Distance.levenshtein` | `(a: str, b: str) -> int` | `int` | Minimum number of single-character insertions/deletions/substitutions to turn `a` into `b`. |
| `Distance.similarity` | `(a: str, b: str) -> float` | `float` | Normalized similarity in `[0.0, 1.0]`: `1.0` = identical, `0.0` = edit distance equals the longer string's length. Two empty strings are defined as identical (`1.0`). |

### Example

```tauraro
from std.text.distance import Distance

print(str(Distance.levenshtein("kitten", "sitting")))   # 3
print(str(Distance.levenshtein("flaw", "lawn")))         # 2

print(str(Distance.similarity("kitten", "sitting")))     # ~0.571
print(str(Distance.similarity("abc", "abc")))             # 1.0
print(str(Distance.similarity("abc", "xyz")))              # 0.0
```

---

## std.text.wrap — Wrap class

**When**: Formatting text for a fixed-width terminal or CLI help output —
wrapping long lines to a column width, or indenting a block of text (e.g. for
nested/quoted output).
**Why**: Greedy word-wrap that never splits a word (an overlong word gets its
own line rather than being cut), preserves blank-line paragraph breaks, and
correctly leaves exact-width lines unwrapped.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Wrap.wrap` | `(text: str, width: int) -> List[str]` | `List[str]` | Greedy word-wrap: breaks only on whitespace, never splits a word unless it alone exceeds `width`. Blank lines in `text` (paragraph breaks) are preserved as empty output lines. |
| `Wrap.indent` | `(text: str, prefix: str) -> str` | `str` | Prefix every line of `text` with `prefix`. Empty input returns `""` (no phantom prefixed line). |

### Example

```tauraro
from std.text.wrap import Wrap

mut lines = Wrap.wrap("the quick brown fox jumps over the lazy dog", 15)
mut i = 0
while i < len(lines):
    print(lines[i])
    i = i + 1
# the quick brown
# fox jumps over
# the lazy dog

print(Wrap.indent("a\nb\nc", "> "))
# > a
# > b
# > c
```
