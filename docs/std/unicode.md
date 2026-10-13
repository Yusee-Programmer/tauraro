# std.unicode — UTF-8 and Unicode

```tauraro
from std.unicode import Unicode
from std.unicode import nfc, casefold, graphemes, display_width   # re-exported free functions
```

> Pure-C implementation — no external library required. Character data is
> generated from the **Unicode 16.0** Character Database into
> `std/unicode/unicode_native.c` (two-stage lookup tables, ~320 KB of object
> code) by `scripts/gen_unicode_tables.py`.
> `Unicode` string methods use **codepoint indices** (not byte offsets).

| Module | Contents |
|---|---|
| `std.unicode` | `Unicode` class (below) + re-exports of everything in the sub-modules |
| `std.unicode.normalize` | NFC / NFD / NFKC / NFKD, quick check, canonical equivalence |
| `std.unicode.casemap` | Full case mapping, titlecase, case folding, caseless matching |
| `std.unicode.segment` | Grapheme clusters, words, sentences (UAX #29), terminal display width |

---

## Unicode — String-level methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Unicode.len` | `(s: str) -> int` | `int` | Number of Unicode codepoints (not bytes). |
| `Unicode.valid` | `(s: str) -> bool` | `bool` | `true` when `s` is valid UTF-8. |
| `Unicode.char_at` | `(s: str, idx: int) -> int` | `int` | Codepoint at codepoint index `idx`. |
| `Unicode.slice` | `(s: str, start: int, end_: int) -> str` | `str` | Substring by codepoint range `[start, end_)`. |
| `Unicode.to_upper` | `(s: str) -> str` | `str` | Full uppercase mapping (`"ß"` → `"SS"`). |
| `Unicode.to_lower` | `(s: str) -> str` | `str` | Full lowercase mapping (with Greek final sigma). |
| `Unicode.casefold` | `(s: str) -> str` | `str` | Full case folding. |
| `Unicode.normalize_nfc` | `(s: str) -> str` | `str` | NFC normalization. |
| `Unicode.graphemes` | `(s: str) -> List[str]` | `List[str]` | User-perceived characters. |
| `Unicode.width` | `(s: str) -> int` | `int` | Terminal display width in columns. |
| `Unicode.is_alpha` | `(s: str) -> bool` | `bool` | `true` when every codepoint is a letter. |
| `Unicode.is_digit` | `(s: str) -> bool` | `bool` | `true` when every codepoint is a decimal digit (Nd). |
| `Unicode.is_alnum` | `(s: str) -> bool` | `bool` | `true` when every codepoint is a letter or digit. |

## Unicode — Codepoint-level methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Unicode.is_letter` | `(cp: int) -> bool` | `bool` | `true` when `cp` is a letter (General_Category `L*`). |
| `Unicode.is_digit_cp` | `(cp: int) -> bool` | `bool` | `true` when `cp` is a decimal digit (`Nd`). |
| `Unicode.upper` | `(cp: int) -> int` | `int` | Simple (single codepoint) uppercase mapping. |
| `Unicode.lower` | `(cp: int) -> int` | `int` | Simple (single codepoint) lowercase mapping. |
| `Unicode.category` | `(cp: int) -> str` | `str` | One-letter general category (see below). |
| `Unicode.general_category` | `(cp: int) -> str` | `str` | Two-letter General_Category, e.g. `"Lu"`, `"Mn"`, `"Cn"`. |
| `Unicode.chr` | `(cp: int) -> str` | `str` | UTF-8 string of one codepoint (U+FFFD for surrogates / out of range). |
| `Unicode.char_width` | `(cp: int) -> int` | `int` | Display columns of one codepoint (0, 1 or 2). |

### Unicode categories

| Return | Meaning |
|---|---|
| `"L"` | Letter |
| `"N"` | Number |
| `"Z"` | Separator (space, line/paragraph separator) |
| `"C"` | Other: control (incl. tab and newline), format, surrogate, private use, unassigned |
| `"P"` | Punctuation |
| `"S"` | Symbol |
| `"M"` | Mark (combining) |

---

## Normalization — `std.unicode.normalize`

Implements UAX #15: full canonical / compatibility decomposition, canonical
ordering by combining class, canonical composition with the composition
exclusions, and algorithmic Hangul syllable (de)composition. Every function
runs the quick check first and returns the input unchanged when it is already
in the requested form, so normalizing clean text is a single fast scan.
Ill-formed UTF-8 is replaced with U+FFFD.

| Function | Description |
|---|---|
| `nfc(s) -> str` | Canonical composition — the usual form for storage and comparison. |
| `nfd(s) -> str` | Canonical decomposition. |
| `nfkc(s) -> str` | Compatibility composition (ligatures, fullwidth forms, superscripts, ... folded). |
| `nfkd(s) -> str` | Compatibility decomposition. |
| `normalize(form, s) throws str -> str` | `form` is `"NFC"`, `"NFD"`, `"NFKC"` or `"NFKD"` (any case). |
| `is_normalized(form, s) throws str -> bool` | Quick check, falling back to a full check for "maybe". |
| `is_nfc(s)`, `is_nfd(s)`, `is_nfkc(s)`, `is_nfkd(s)` | Same, per form. |
| `canonical_eq(a, b) -> bool` | `true` when `a` and `b` are canonically equivalent. |
| `combining_class(cp) -> int` | Canonical_Combining_Class (0 for starters). |
| `unicode_version() -> str` | UCD version of the tables (`"16.0.0"`). |

## Case mapping — `std.unicode.casemap`

Full mappings from UnicodeData.txt plus the unconditional SpecialCasing.txt
entries (`"ß"` → `"SS"`, `"ﬁ"` → `"FI"`, `"İ"` → `"i̇"`); lowercasing applies the
Greek Final_Sigma rule. Language-specific rules (Turkish/Azeri dotless i,
Lithuanian) are not applied.

| Function | Description |
|---|---|
| `to_upper(s) -> str` | Full uppercase. |
| `to_lower(s) -> str` | Full lowercase (`"ΟΔΟΣ"` → `"οδος"`). |
| `to_title(s) -> str` | First cased letter of each word (UAX #29 word boundaries) titlecased, rest lowercased; a word starting with a digit is left alone (`"3rd"`). |
| `casefold(s) -> str` | Full case folding (CaseFolding.txt status C + F). |
| `caseless_eq(a, b) -> bool` | Canonical caseless match: `NFD(casefold(NFD(a))) == NFD(casefold(NFD(b)))`. |
| `simple_upper(cp)`, `simple_lower(cp)`, `simple_title(cp)`, `simple_fold(cp)` | Single-codepoint mappings (C + S folding). |

## Segmentation and width — `std.unicode.segment`

UAX #29 (Unicode 16 rules): extended grapheme clusters (emoji ZWJ sequences,
skin tones, regional-indicator flag pairs, Hangul syllables, the Indic conjunct
rule GB9c), word boundaries and sentence boundaries.

| Function | Description |
|---|---|
| `graphemes(s) -> List[str]` | Extended grapheme clusters. |
| `grapheme_count(s) -> int` | Number of grapheme clusters (no list built). |
| `grapheme_offsets(s) -> List[int]` | Byte offsets `[start0, end0, start1, end1, ...]`. |
| `word_segments(s) -> List[str]` | All word-boundary segments, spaces and punctuation included (they concatenate back to `s`). |
| `words(s) -> List[str]` | Only segments containing a letter or digit: `"It's 3.14 o'clock"` → `["It's", "3.14", "o'clock"]`. Ideographs are one word each. |
| `word_offsets(s) -> List[int]` | Byte offsets of the word-boundary segments. |
| `sentences(s) -> List[str]` | Sentences, each keeping its trailing spaces / line break. |
| `sentence_offsets(s) -> List[int]` | Byte offsets of the sentences. |
| `display_width(s) -> int` | Terminal columns: East Asian Wide/Fullwidth and emoji = 2, combining marks / format / control = 0, measured per grapheme cluster (a ZWJ family or a flag is 2, `"❤️"` with VS16 is 2). |
| `char_width(cp) -> int` | Columns of one codepoint: 0, 1 or 2. |
| `truncate_width(s, width) -> str` | Longest prefix of whole clusters that fits in `width` columns. |
| `truncate_width_ellipsis(s, width, ellipsis) -> str` | Truncate and append `ellipsis` when something was cut. |
| `pad_width(s, width) -> str` | Right-pad with spaces to `width` columns. |

Conformance: the implementation passes all of NormalizationTest.txt (19 965
lines, plus the invariance of every other assigned codepoint),
GraphemeBreakTest.txt (1 093), WordBreakTest.txt (1 826) and
SentenceBreakTest.txt (512) for Unicode 16.0; `tests/lang/122_unicode_norm.tr`
embeds a representative subset.

Performance (1 MB of mixed Latin/CJK/Greek/Hangul/emoji text, x86-64): NFC of
already-NFC text 3 ms (quick check), NFC of decomposed text 13 ms, NFD 9 ms,
case folding 11 ms, grapheme count 10 ms, `words` 25 ms, display width 13 ms.

### Regenerating the tables

```sh
mkdir ucd && cd ucd     # UnicodeData.txt, DerivedNormalizationProps.txt, CaseFolding.txt,
                        # SpecialCasing.txt, EastAsianWidth.txt, DerivedCoreProperties.txt,
                        # GraphemeBreakProperty.txt, WordBreakProperty.txt,
                        # SentenceBreakProperty.txt, emoji-data.txt from unicode.org/Public/<ver>/ucd
python3 -I scripts/gen_unicode_tables.py ucd            # rewrites std/unicode/unicode_native.c tables
python3 -I scripts/gen_unicode_tables.py --tests ucd    # refreshes the cases in 122_unicode_norm.tr
```

---

## Example

```tauraro
from std.unicode import Unicode, nfc, nfkc, casefold, caseless_eq, graphemes, words, display_width, truncate_width

mut s = "Héllo"
print(str(Unicode.len(s)))         # 5  (not 6 bytes)
print(Unicode.to_upper("straße"))  # "STRASSE"

print(nfkc("ﬁle"))                 # "file"
print(str(caseless_eq("Straße", "STRASSE")))   # true

mut g = graphemes("a👍🏽🇳🇬")
print(str(len(g)))                 # 3

mut w = words("Hello, world! It's 3.14")
print(str(len(w)))                 # 4: Hello, world, It's, 3.14

print(str(display_width("日本語")))  # 6
print(truncate_width("日本語", 5))  # "日本"

print(Unicode.category(65))        # "L"  (A)
print(Unicode.general_category(0x301))   # "Mn"
```
