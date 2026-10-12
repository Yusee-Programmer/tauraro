# std.encoding

Data encoding and decoding: JSON, CSV, TOML, INI, YAML, XML, Base64, Base32, hexadecimal, CBOR, and the Protocol Buffers wire format.

## Import

```tauraro
# Import the whole module
from std.encoding import JsonDoc, JsonRef, JsonWriter, Json, Base64, Hex

# Or import specific sub-modules
from std.encoding.json   import JsonDoc, JsonRef, JsonWriter, Json
from std.encoding.csv    import CsvDoc, Csv
from std.encoding.toml   import TomlValue, Toml
from std.encoding.ini    import IniDoc, Ini
from std.encoding.yaml   import YamlValue, Yaml
from std.encoding.xml    import XmlElement, Xml
from std.encoding.base64 import Base64
from std.encoding.hex    import Hex
from std.encoding.base32 import Base32
from std.encoding.cbor   import CBVal, Cbor
from std.encoding.protobuf import Proto, ProtoWriter, ProtoReader
```

---

## JSON — `std.encoding.json`

A fast, memory-safe JSON parser and serializer. The API is **zero-copy** and uses
**no raw pointers** — parsing builds a compact arena (`JsonDoc`), you read it through
lightweight borrowed views (`JsonRef` / `StrView`), and you build output with a
streaming `JsonWriter`. Everything is ARC-managed, so there is nothing to `free`
manually (except the optional `JsonWriter.free()` fast path).

```tauraro
from std.encoding.json import JsonDoc, JsonRef, JsonWriter, Json
```

> **Migration note.** The old `Pointer[JsonValue]` tree API (with `.read()` and
> manual `dispose()`) has been replaced. `.read()` on a raw pointer is now a `[P-2]`
> error by default; use the safe `JsonDoc` / `JsonRef` / `JsonWriter` types below.

### Parsing — `JsonDoc` + `JsonRef`

`Json.parse(src)` (or `JsonDoc.init(src)`) parses a string into a `JsonDoc` — a
single arena that owns all of the document's structure and string bytes. You never
index it directly; you navigate it through `JsonRef`, a tiny borrowed view
(`@value_type`) that stays valid while the `JsonDoc` is alive.

```tauraro
mut doc  = Json.parse("{\"x\": 42, \"items\": [1, 2, 3], \"name\": \"Tauraro\"}")
mut root = doc.root()                 # -> JsonRef

root.obj_get("x").get_int()           # 42
root.obj_get("name").get_str()        # "Tauraro" (owned copy)
root.obj_get("items").array_len()     # 3
root.obj_get("items").array_get(0).get_int()   # 1
# doc drops at scope end — the whole arena is reclaimed automatically
```

**`JsonRef` methods** (all borrow from the `JsonDoc`, no allocation):

| Method | Returns | Notes |
|--------|---------|-------|
| `exists()` | `bool` | `false` for a missing key / out-of-range index |
| `is_null()` `is_bool()` `is_int()` `is_float()` `is_str()` `is_array()` `is_object()` | `bool` | type check |
| `get_bool()` `get_int()` `get_float()` | value | numeric / bool accessors |
| `as_float()` | `float` | coerces an int node to float |
| `get_str()` | `str` | **owned** copy of the string value |
| `str_view()` | `StrView` | **zero-copy** borrowed view of the string bytes (no allocation) — see below |
| `str_eq(other: str)` | `bool` | compare the string value without materializing it |
| `obj_get(key: str)` | `JsonRef` | field lookup (returns a non-existent ref if absent) |
| `obj_has(key: str)` | `bool` | key presence |
| `array_len()` | `int` | element count |
| `array_get(i: int)` | `JsonRef` | element by index |
| `to_str()` | `str` | re-serialize this node (and its subtree) to a compact JSON string |

**Zero-copy string reads.** For hot paths (routing, validation) that only need to
*look at* a string, `str_view()` returns a `StrView` that borrows the doc's buffer —
no allocation at all. Materialize it with `.to_str()` only when you must keep it:

```tauraro
mut method = root.obj_get("method").str_view()
if method.eq("GET"): ...              # comparison, zero allocations
mut owned = method.to_str()           # allocate only when storing it
```

### Writing — `JsonWriter`

`JsonWriter` streams JSON directly into a growable buffer — no intermediate tree, no
per-node allocation. Ideal for building responses.

```tauraro
mut w = JsonWriter.init(64)           # initial capacity in bytes
w.field_str("status", "ok")           # {"status":"ok",
w.field_int("count", 3)               #  "count":3}
mut body = w.finish()                 # -> owned str; also frees the writer
```

Nested structures use the explicit begin/end + `key` / value calls:

```tauraro
mut w = JsonWriter.init(128)
w.begin_object()
    w.key("user"); w.begin_object()
        w.field_str("name", "Ada")
        w.field_int("id", 7)
    w.end_object()
    w.key("tags"); w.begin_array()
        w.str_val("a"); w.str_val("b")
    w.end_array()
w.end_object()
print(w.finish())   # {"user":{"name":"Ada","id":7},"tags":["a","b"]}
```

**`JsonWriter` methods:**

| Method | Description |
|--------|-------------|
| `init(capacity: int)` | new writer with a starting byte capacity |
| `begin_object()` / `end_object()` | `{` … `}` |
| `begin_array()` / `end_array()` | `[` … `]` |
| `key(name: str)` | write an object key (call a value method next) |
| `int_val(n)` / `str_val(s)` / `bool_val(b)` / `null_val()` | write a bare value |
| `field_int(name, n)` / `field_str(name, s)` / `field_bool(name, b)` | key + value in one call |
| `view()` | borrow the buffer as `str` **without** freeing (valid until the next write / `free`) |
| `finish()` | return an **owned** `str` and free the writer |
| `free()` | release the writer without producing a string |

### `Json` static helper

```tauraro
Json.parse(src: str) -> JsonDoc       # parse a string into an arena document
```

### Example

```tauraro
from std.encoding.json import Json, JsonWriter

def main():
    # Parse
    mut doc  = Json.parse("{\"x\": 42, \"y\": [1, 2]}")
    mut root = doc.root()
    print(root.obj_get("x").get_int())               # 42
    print(root.obj_get("y").array_get(1).get_int())  # 2

    # Build
    mut w = JsonWriter.init(64)
    w.begin_object()
    w.field_str("name", "Tauraro")
    w.field_bool("stable", true)
    w.end_object()
    print(w.finish())   # {"name":"Tauraro","stable":true}
```

---

## CSV — `std.encoding.csv`

An RFC 4180 CSV parser and serializer. Unlike JSON/TOML/YAML, a CSV document
isn't a nested tree — it's a flat 2D grid of string fields — so the API is
shaped around row/column access (`CsvDoc`) instead of the tagged-node
`obj_get`/`array_get` shape used by the other formats.

Supports quoted fields (`"a,b"`), escaped quotes inside quoted fields
(`""` → `"`), fields with embedded newlines when quoted, both `\n` and
`\r\n` line endings, a trailing newline or lack thereof at EOF, custom
delimiters (semicolon, tab, ...), and an optional header row for
lookup-by-column-name.

```tauraro
from std.encoding.csv import CsvDoc, Csv

mut doc = Csv.parse("name,age\nAda,36\n\"Grace,M\",85\n")
print(doc.row_count())     # 3
print(doc.field(1, 0))     # "Ada"
print(doc.field(2, 0))     # "Grace,M"  (comma preserved from the quoted field)

print(Csv.stringify(doc.rows))   # round-trips back to CSV text
```

**`CsvDoc`** methods:

| Method | Returns | Description |
|---|---|---|
| `.row_count()` | `int` | total number of rows (including the header row, if any) |
| `.col_count(row: int)` | `int` | number of fields in `row` (0 if out of range) |
| `.row(row: int)` | `Vec[str]` | the fields of `row` |
| `.field(row: int, col: int)` | `str` | a single field ("" if out of range) |
| `.data_row_count()` | `int` | row count excluding the header row (if `has_header`) |
| `.col_index(name: str)` | `int` | header column index for `name` (-1 if no header / not found) |
| `.field_by_name(row: int, name: str)` | `str` | field lookup by column name; `row` is a data-row index (0 = first row after the header) |
| `.has_header` | `bool` | true if this doc was parsed with `parse_with_header*` |
| `.rows` | `Vec[Vec[str]]` | the raw grid, pass directly to `Csv.stringify` |

**`Csv`** static helpers:

| Method | Description |
|---|---|
| `Csv.parse(src: str) -> CsvDoc` | parse comma-delimited CSV text |
| `Csv.parse_with_delimiter(src: str, delim: int) -> CsvDoc` | parse with a custom delimiter (pass the char code, e.g. `59` for `;`, `9` for tab) |
| `Csv.parse_with_header(src: str) -> CsvDoc` | parse comma-delimited CSV, treating row 0 as column names |
| `Csv.parse_with_header_and_delimiter(src: str, delim: int) -> CsvDoc` | combine both of the above |
| `Csv.stringify(rows: Vec[Vec[str]]) -> str` | serialize rows to comma-delimited CSV text (CRLF line endings, minimal quoting) |
| `Csv.stringify_with_delimiter(rows: Vec[Vec[str]], delim: int) -> str` | serialize with a custom delimiter |

Quoting on write is **minimal**: a field is wrapped in `"..."` (with `"`
doubled) only when it actually contains the delimiter, a double quote, or a
newline — plain fields are left bare.

### Example

```tauraro
from std.encoding.csv import Csv
from std.core.vec import Vec

def main():
    mut doc = Csv.parse_with_header("name,age\nAda,36\nGrace,85\n")
    print(doc.field_by_name(0, "name"))   # "Ada"
    print(doc.field_by_name(1, "age"))    # "85"

    mut rows = Vec[Vec[str]].init(2)
    mut row = Vec[str].init(2)
    row.push("has,comma")
    row.push("plain")
    rows.push(row)
    print(Csv.stringify(rows))   # "has,comma",plain\r\n
```

---

## TOML — `std.encoding.toml`

A TOML parser and serializer: comments, bare/quoted keys, basic & literal
strings, `_`-separated integers, floats, booleans, arrays, inline tables,
and `[table]`/`[a.b.c]` nested table headers.

```tauraro
from std.encoding.toml import Toml, TomlValue

mut doc = Toml.parse("name = \"Tauraro\"\nversion = 9\n\n[deps]\nfoo = \"1.0\"\n")
print(doc.obj_get("name").get_str())            # "Tauraro"
print(doc.obj_get("deps").obj_get("foo").get_str())  # "1.0"

print(Toml.stringify(doc))   # round-trips back to TOML text
```

**`TomlValue`** is a tagged tree node (`@value_type`), mirroring
`JsonDoc`'s shape:

| Method | Description |
|---|---|
| `.is_none()` / `.is_bool()` / `.is_int()` / `.is_float()` / `.is_str()` / `.is_array()` / `.is_table()` | type check |
| `.get_bool()` / `.get_int()` / `.get_float()` / `.get_str()` | value accessors |
| `.array_len()` / `.array_get(i: int)` | array access |
| `.obj_get(key: str)` | table field lookup (a `TOML_TABLE` value) |
| `TomlValue.init_bool/_int/_float/_str/_array/_table(...)` | build a value tree to pass to `Toml.stringify` |

**`Toml`** static helpers: `Toml.parse(src: str) -> TomlValue`,
`Toml.stringify(v: TomlValue) -> str`.

---

## INI — `std.encoding.ini`

An INI file parser and serializer covering the common practical dialect:
`[section]` headers (values before any header land in an implicit default
section named `""`), both `key = value` and `key: value` separators, both
`;` and `#` comments (full-line and inline — never stripped out of a quoted
value), quoted values (`"..."` / `'...'`, quotes stripped on parse and added
back on stringify only when needed), and whitespace trimming around keys,
values, and section names. Unlike JSON/TOML/YAML there is no tagged value
tree — every value is a plain `str`; the caller decides how to interpret it.

```tauraro
from std.encoding.ini import IniDoc, Ini

mut doc = Ini.parse("[server]\nhost = localhost\nport = 8080\n")
print(doc.get("server", "host"))   # "localhost"
print(doc.get("server", "port"))   # "8080" (still a str — caller converts)

print(Ini.stringify(doc))   # round-trips back to INI text
```

**`IniDoc`** methods:

| Method | Returns | Notes |
|---|---|---|
| `.get(section: str, key: str)` | `str` | empty string if the section or key is missing |
| `.has(section: str, key: str)` | `bool` | distinguishes "missing" from "present but empty" |
| `.set(section: str, key: str, value: str)` | — | create-or-update; creates the section if needed |
| `.section_names()` | `Vec[str]` | all section names, in document order (`""` = default/global section) |
| `.keys(section: str)` | `Vec[str]` | keys within a section, in insertion order (empty `Vec` if the section doesn't exist) |
| `.has_section(name: str)` | `bool` | section presence check |

**`Ini`** static helpers: `Ini.parse(src: str) -> IniDoc`,
`Ini.stringify(doc: IniDoc) -> str`, `Ini.doc() -> IniDoc` (build a new,
empty document programmatically).

### Example

```tauraro
from std.encoding.ini import Ini

def main():
    mut doc = Ini.parse("root = top-level\n\n[db]\nhost = 127.0.0.1\nname = \"my app\" ; inline comment\n")
    print(doc.get("", "root"))       # "top-level"
    print(doc.get("db", "host"))     # "127.0.0.1"
    print(doc.get("db", "name"))     # "my app"

    mut built = Ini.doc()
    built.set("client", "timeout", "30")
    print(Ini.stringify(built))      # "[client]\ntimeout = 30\n"
```

---

## YAML — `std.encoding.yaml`

A YAML parser and serializer: block and flow styles, comments, anchors.
Same tagged-tree shape as TOML/JSON.

```tauraro
from std.encoding.yaml import Yaml, YamlValue

mut doc = Yaml.parse("name: Tauraro\nitems:\n  - a\n  - b\n")
print(doc.obj_get("name").get_str())             # "Tauraro"
print(doc.obj_get("items").array_get(0).get_str())  # "a"

print(Yaml.stringify(doc))   # round-trips back to YAML text
```

**`YamlValue`** methods mirror `TomlValue`'s (`.is_null()`/`.is_bool()`/
.../`.obj_get(key)`/`.array_get(i)`/`.array_len()`), plus
`YamlValue.init_null()` for an explicit YAML `null`. **`Yaml`** static
helpers: `Yaml.parse(src: str) -> YamlValue`, `Yaml.stringify(v: YamlValue)
-> str`.

---

## XML — `std.encoding.xml`

An XML parser and serializer covering the practical subset used by real
config/data files: elements (`<tag>...</tag>` and self-closing `<tag/>`),
attributes (double- or single-quoted), nested elements, text content,
comments (skipped), the `<?xml ... ?>` declaration (skipped), and the 5
predefined entities (`&lt; &gt; &amp; &apos; &quot;`) in both text and
attribute values.

```tauraro
from std.encoding.xml import Xml, XmlElement

mut root = Xml.parse("<config><server host=\"localhost\" port=\"9090\"/></config>")
mut server = root.find_child("server")
print(server.attr("host"))   # "localhost"
print(server.attr("port"))   # "9090"

print(Xml.stringify(root))   # round-trips back to XML text
```

Unlike JSON/TOML/YAML's scalar-tagged tree, XML elements have both
attributes and children, so `XmlElement` is its own shape rather than the
`is_bool()`/`is_table()`-style tagged value used by the other formats.

**`XmlElement`** (`@value_type`, ARC-managed — no raw pointers, no `unsafe:`):

| Method | Description |
|---|---|
| `.tag_name() -> str` | the element's tag name |
| `.text() -> str` | direct text content (concatenated across text runs at this level) |
| `.set_text(s: str)` / `.append_text(s: str)` | set/append text content (for building a tree) |
| `.attr(name: str) -> str` | attribute value, or `""` if absent |
| `.has_attr(name: str) -> bool` | attribute presence |
| `.set_attr(name: str, value: str)` | set (or overwrite) an attribute |
| `.attr_count() -> int` / `.attr_name_at(i)` / `.attr_val_at(i)` | iterate all attributes |
| `.children() -> Vec[XmlElement]` | all direct children |
| `.child_count() -> int` / `.child_at(i: int)` | direct children by index |
| `.add_child(child: XmlElement)` | append a child (for building a tree) |
| `.find_child(tag: str) -> XmlElement` | first direct child with a matching tag; returns an empty sentinel (`.is_none()` true) if none match |
| `.find_all(tag: str) -> Vec[XmlElement]` | all direct children with a matching tag |
| `.is_none() -> bool` | true only for the sentinel returned by a `find_child` miss |

**`Xml`** static helpers:

| Method | Description |
|---|---|
| `Xml.parse(src: str) -> XmlElement` | parse XML text, returning the root element |
| `Xml.stringify(root: XmlElement) -> str` | serialize a tree back to XML text (escapes the 5 entities) |
| `Xml.element(tag: str) -> XmlElement` | construct a new empty element (for building a tree programmatically) |

### Out of scope

Documented deliberately, rather than silently mishandled (same policy as
`std.encoding.toml`/`std.encoding.yaml`'s own deferrals):

- **DTDs** (`<!DOCTYPE ...>`) are skipped as an opaque markup declaration —
  not parsed or validated against.
- **Custom/numeric entity references** (`&#65;`, `&custom;`) are not
  resolved — only the 5 predefined entities are recognized. Any other `&`
  is passed through literally.
- **`<![CDATA[ ... ]]>`** sections are not implemented; use plain text
  content (with entity-escaping) instead.
- **Processing instructions** other than the leading `<?xml ... ?>`
  declaration (e.g. `<?xml-stylesheet ... ?>`) are skipped like comments,
  not captured.
- **Namespaces** (`xmlns`, `ns:tag`) are not treated specially — a
  prefixed name is kept verbatim as one string (e.g. `"ns:tag"`), with no
  prefix/local-name splitting or URI resolution.
- **Mixed content** (text interleaved with child elements at the same
  level) is preserved as best-effort concatenated text via `.text()`;
  there's no ordered content-node list distinguishing text before/after a
  particular child.

### Example

```tauraro
from std.encoding.xml import Xml

def main():
    mut root = Xml.parse("<library><book id=\"1\">Dune</book><book id=\"2\">Neuromancer</book></library>")
    mut books = root.find_all("book")
    mut i = 0
    while i < books.len:
        print(books.get(i).attr("id") + ": " + books.get(i).text())
        i = i + 1

    # Build a tree programmatically and serialize it.
    mut note = Xml.element("note")
    note.set_attr("id", "1")
    mut body = Xml.element("body")
    body.set_text("Meet at 5")
    note.add_child(body)
    print(Xml.stringify(note))   # <note id="1"><body>Meet at 5</body></note>
```

---

## Base64 — `std.encoding.base64`

RFC 4648 Base64 encoding and decoding.

### `Base64` class (static API)

| Method | Description |
|--------|-------------|
| `Base64.encode(s: str) -> str` | Encode bytes to standard Base64 (with `=` padding) |
| `Base64.decode(s: str) -> str` | Decode a standard Base64 string |
| `Base64.encode_url(s: str) -> str` | URL-safe Base64 (`-` and `_`; no padding) |
| `Base64.decode_url(s: str) -> str` | Decode URL-safe Base64 |

### Example

```tauraro
from std.encoding.base64 import Base64

def main():
    mut s   = "Hello, Tauraro!"
    mut enc = Base64.encode(s)
    mut dec = Base64.decode(enc)
    print(enc)   # → SGVsbG8sIFRhdXJhcm8h
    print(dec)   # → Hello, Tauraro!

    mut url_enc = Base64.encode_url(s)
    print(url_enc)   # no +, /, or = characters — safe in URLs
```

---

## Hex — `std.encoding.hex`

Hexadecimal encoding and decoding.

### `Hex` class (static API)

| Method | Description |
|--------|-------------|
| `Hex.encode(s: str) -> str` | Encode bytes to lowercase hex (`"48656c6c6f"`) |
| `Hex.encode_upper(s: str) -> str` | Encode bytes to uppercase hex (`"48656C6C6F"`) |
| `Hex.decode(s: str) -> str` | Decode a hex string back to bytes (accepts `0x`/`0X` prefix) |
| `Hex.encode_bytes(data: Pointer[char], len: int) -> str` | Encode a raw memory buffer |
| `Hex.format_int(n: int, width: int) -> str` | Format an int as a zero-padded hex string |

### Example

```tauraro
from std.encoding.hex import Hex

def main():
    mut enc = Hex.encode("Hello")        # → "48656c6c6f"
    mut dec = Hex.decode("48656c6c6f")   # → "Hello"
    print(enc)
    print(dec)

    print(Hex.format_int(255, 4))   # → "00ff"
    print(Hex.format_int(65535, 8)) # → "0000ffff"
```

---

## Base32 — `std.encoding.base32`

RFC 4648 Base32 with three alphabets: standard (`A-Z2-7`, §6), extended-hex
"base32hex" (`0-9A-V`, §7), and Crockford (the same alphabet `std.crypto.uuid`
uses for ULIDs — shared, not duplicated). Input is treated as binary: lengths
come from the real byte length, so embedded `0x00` bytes round-trip. Decoding is
strict and reports problems through `throws str` (use `?`, or bind the call to a
`Result[str, str]`).

| Method | Description |
|--------|-------------|
| `Base32.encode(s)` / `encode_nopad(s)` | Standard alphabet, with / without `=` padding |
| `Base32.encode_hex(s)` / `encode_hex_nopad(s)` | base32hex alphabet |
| `Base32.encode_crockford(s)` | Crockford alphabet (never padded) |
| `Base32.encode_opts(s, hex: bool, pad: bool)` | Explicit alphabet / padding choice |
| `Base32.decode(s) throws str -> str` | Standard alphabet; padding optional but, if present, exact; case-insensitive |
| `Base32.decode_padded(s)` | Like `decode`, but padding is required |
| `Base32.decode_hex(s)` / `decode_hex_padded(s)` | base32hex |
| `Base32.decode_crockford(s)` | Crockford; `I`/`L` → `1`, `O` → `0`, `-` ignored |
| `Base32.decode_opts(s, hex: bool, require_pad: bool)` | Explicit form |
| `Base32.is_valid(s)` / `is_valid_hex(s)` | `bool` — does it decode? |

Decode errors: character outside the alphabet, `=` in the middle, impossible length
(1/3/6 data characters mod 8), wrong amount of padding, non-zero trailing bits.

```tauraro
from std.encoding.base32 import Base32

Base32.encode("foobar")          # "MZXW6YTBOI======"
Base32.encode_nopad("foobar")    # "MZXW6YTBOI"
Base32.encode_hex("foobar")      # "CPNMUOJ1E8======"
mut r: Result[str, str] = Base32.decode("MZXW6YTBOI======")
if r.is_ok:
    print(r.ok)                  # foobar
```

---

## CBOR — `std.encoding.cbor`

A wire-compatible [RFC 8949](https://www.rfc-editor.org/rfc/rfc8949) encoder/decoder,
shaped like `std.serialize.msgpack`: a dynamic value tree `CBVal` plus the static
codec class `Cbor`. Integers (full −2⁶⁴…2⁶⁴−1), byte/text strings, arrays, maps with
**any** key type, tags, `false`/`true`/`null`/`undefined`/simple values, and
half/single/double floats are supported. Encoding is *preferred serialization*
(shortest heads, definite lengths, floats in the shortest exact width); decoding also
accepts indefinite-length strings/arrays/maps. The decoder is strict (`throws str`):
truncation, reserved additional-info values, bad simple values, stray breaks,
oversized lengths (checked before allocating), nesting > 512 and, for `loads`,
trailing bytes are all errors. Text strings are not UTF-8-validated.

Integers outside Tauraro's signed 64-bit `int` become *big* values
(`is_bigint()`, raw argument in `big_arg()`); they re-encode exactly.

**`CBVal` constructors:** `init_int(n)`, `init_u64_raw(bits)`, `init_nint_raw(bits)`,
`init_bytes(s)`, `init_text(s)`, `init_array()`, `init_map()`, `init_tag(num, v)`,
`init_bool(b)`, `init_null()`, `init_undefined()`, `init_float(f)`,
`init_float_w(f, 16|32|64)`, `init_simple(n)`.
**Queries:** `is_int/is_bigint/is_bytes/is_text/is_array/is_map/is_tag/is_bool/is_null/is_undefined/is_float/is_simple`,
`get_int/get_bool/get_float/get_text/get_bytes/get_simple`, `tag_number()`, `tag_content()`,
`array_push/array_len/array_get`, `map_set(text_key, v)`, `map_put(key: CBVal, v)`,
`map_get(text_key)`, `map_get_int(n)`, `map_has`, `map_len/map_key/map_val`,
`eq(other)` (deep), `to_diag()` (RFC 8949 §8 diagnostic notation).

**`Cbor` static API:**

| Method | Description |
|--------|-------------|
| `Cbor.dumps(v, out_len: List[int]) -> str` | Encode; `out_len[0]` receives the true byte length |
| `Cbor.dumps_len(v) -> (str, int)` | Encode, returning `(bytes, length)` |
| `Cbor.loads(data, len) throws str -> CBVal` | Decode exactly one item spanning `len` bytes |
| `Cbor.loads_str(data) throws str -> CBVal` | `loads` using `data.len()` |
| `Cbor.loads_prefix(data, len, consumed: List[int]) throws str -> CBVal` | Decode the first item; `consumed[0]` = bytes used (walk a CBOR sequence) |
| `Cbor.dump_int/dump_float/dump_bool/dump_text/dump_bytes/dump_list_int/dump_list_text/dump_dict_str_str/dump_dict_str_int` | One-shot helpers |
| `Cbor.load_list_int/load_list_text` | One-shot decoders |

```tauraro
from std.encoding.cbor import CBVal, Cbor

mut m = CBVal.init_map()
m.map_set("id", CBVal.init_int(7))
mut tags = CBVal.init_array()
tags.array_push(CBVal.init_text("a"))
m.map_set("tags", tags)

mut bytes_, blen = Cbor.dumps_len(m)        # a2 62 69 64 07 64 74 61 67 73 81 61 61
mut back = Cbor.loads(bytes_, blen)         # Result[CBVal, str]
print(back.ok.to_diag())                    # {"id": 7, "tags": ["a"]}
```

---

## Protocol Buffers wire format — `std.encoding.protobuf`

A low-level reader/writer for the protobuf binary encoding — varints, ZigZag,
fixed32/fixed64, length-delimited fields, field keys and unknown-field skipping.
There is **no** `.proto` compiler; you read/write fields by number.

`ProtoWriter` (call `.free()` when done): `write_int32/int64/uint32/uint64/sint32/sint64/bool/enum`,
`write_fixed32/sfixed32/fixed64/sfixed64/float/double`, `write_bytes/string`,
`write_message(field, sub_writer)`, `write_packed_varints/sint/fixed32/fixed64/float/double`,
`write_tag`, `write_raw`, primitives `put_varint/put_fixed32/put_fixed64`, and
`to_str()`, `len()`, `clear()`, `free()`.

`ProtoReader.init(data)` (or `init_range(data, start, end)`): `next_field() throws str -> bool`
sets `field` / `wire`; then read the value with `read_int32/int64/uint32/uint64/sint32/sint64/bool/enum`,
`read_fixed32/sfixed32/fixed64/sfixed64/float/double`, `read_bytes`, `read_string` (UTF-8 validated),
`read_message()` (sub-reader), `read_packed_*`; or `skip()` to ignore it. `field_raw()` returns the
current field's raw bytes so unknown fields can be copied through with `write_raw`.
Malformed input (truncation, >10-byte varints, field number 0, wire types 6/7, lengths past the
end, unmatched end-group) raises a `throws str` error.

`Proto` helpers: `zigzag(n)`, `unzigzag(v)`, `varint_size(v)`, `make_tag(field, wire)`, `tag_field`, `tag_wire`.
uint64/fixed64 values ≥ 2⁶³ read back as the same 64-bit pattern (negative `int`).

```tauraro
from std.encoding.protobuf import ProtoWriter, ProtoReader

mut w = ProtoWriter.init()
w.write_int64(1, 150)             # 08 96 01
w.write_string(2, "testing")      # 12 07 74 65 73 74 69 6e 67
mut wire = w.to_str()
w.free()

mut r = ProtoReader.init(wire)
# (inside a `throws str` function, so `?` can propagate read errors)
while r.next_field()?:
    if r.field == 1:
        mut n = r.read_int64()?       # 150
        print(n)
    elif r.field == 2:
        mut name = r.read_string()?   # "testing"
        print(name)
    else:
        r.skip()?                     # unknown field
```

---

## PDF writer — `std.encoding.pdf`

Writes PDF 1.7 documents with no external libraries: text in the twelve standard Latin fonts
(Helvetica, Times and Courier families, so nothing is embedded), exact text measurement and
word-wrapped text boxes, vector graphics, images, links, bookmarks and metadata. Content and image
streams are Flate-compressed by a built-in deflate encoder (no `-lz` needed).

Coordinates are points (1/72 inch) from the **bottom-left** corner. Text is UTF-8; characters
outside Windows-1252 (the fonts' WinAnsiEncoding) print as `?`. Metadata and bookmark titles
accept any Unicode.

```tauraro
from std.encoding.pdf import PdfDocument, PDF_HELVETICA_BOLD, PDF_TIMES, PDF_FILL, PDF_ALIGN_JUSTIFY
from std.io.file import File

mut doc = PdfDocument.init()
doc.title = "Quarterly report"
doc.author = "Finance"

mut page = doc.add_page_a4()                       # 595.28 x 841.89 pt
page.set_font(PDF_HELVETICA_BOLD, 24.0)
page.text(72.0, 770.0, "Quarterly report — Q3")
page.set_font(PDF_TIMES, 11.0)
mut body_text = File.read_text("summary.txt")
mut y = page.text_box(72.0, 740.0, 451.0, body_text, PDF_ALIGN_JUSTIFY, 1.4)

page.set_fill_color(30, 90, 200)
page.rect(72.0, y - 60.0, 200.0, 40.0, PDF_FILL)

mut logo = doc.add_jpeg(File.read_text("logo.jpg"))?   # embedded as-is
page.draw_image(logo, 450.0, 760.0, 72.0, 36.0)
page.link_url(72.0, 760.0, 300.0, 30.0, "https://example.com/q3")
doc.add_bookmark("Summary", 0, 841.0)
doc.save("report.pdf")?
```

| `PdfDocument` | Description |
|---|---|
| `PdfDocument.init()` | Empty document. Fields: `title`, `author`, `subject`, `keywords`, `creator`, `producer`, `creation_date` (`"D:YYYYMMDDHHmmSSZ"`, current UTC time when empty), `compress` (default true). |
| `add_page(w, h)`, `add_page_a4()`, `add_page_letter()`, `page_count()`, `pages` | Pages, in order. |
| `add_image(img)` | Embed a `std.image` `Image` (RGB, Flate); any non-opaque pixel adds an alpha soft mask. |
| `add_jpeg(bytes)?` | Embed JPEG data unchanged (DCTDecode): grayscale, RGB or CMYK (Adobe inverted CMYK handled). |
| `add_bookmark(title, page_index, y)` | Outline entry; the reader opens with the bookmarks panel. |
| `to_bytes()`, `save(path)?` | Serialize / write the file. |

| `PdfPage` | Description |
|---|---|
| `set_font(font, size)`, `text(x, y, s)`, `text_aligned(x, y, s, align)` | Single-line text at a baseline; `align` is `PDF_ALIGN_LEFT` / `CENTER` / `RIGHT`. |
| `text_box(x, top, width, s, align, line_height)` | Word-wrapped text (newlines start paragraphs, over-long words are broken). `PDF_ALIGN_JUSTIFY` stretches all but each paragraph's last line. Returns the y where the next line would go. |
| `text_width(s)` | Width in points in the current font. |
| `set_fill_color(r, g, b)`, `set_stroke_color(r, g, b)`, `set_fill_gray(v)`, `set_stroke_gray(v)` | Colours (0–255, gray 0–1). |
| `set_line_width`, `set_line_cap`, `set_line_join`, `set_dash(on, off)`, `clear_dash()`, `set_alpha(fill, stroke)` | Graphics state. |
| `line`, `rect`, `rounded_rect`, `circle`, `ellipse`, `polygon(points, mode)`, `polyline(points)` | Shapes; `mode` is `PDF_STROKE`, `PDF_FILL` or `PDF_FILL_STROKE`. |
| `move_to`, `line_to`, `curve_to`, `close_path`, `paint(mode)`, `clip()` | Free-form paths. |
| `save()`, `restore()`, `translate(dx, dy)`, `rotate(degrees)`, `scale(sx, sy)` | Transforms. |
| `draw_image(img, x, y, w, h)` | Place an image scaled into a rectangle. |
| `link_url(x, y, w, h, url)`, `link_page(x, y, w, h, page_index, dest_y)` | Link annotations. |
| `raw(ops)`, `content()` | Append raw content-stream operators / read the stream so far. |

Helpers: `pdf_text_width(font, size, s)`, `pdf_wrap_text(font, size, s, width)`, `pdf_winansi(s)`,
`pdf_deflate(bytes)` (zlib stream), `pdf_num(f)`, `pdf_font_name(font)`, `pdf_font_by_name(name)`.
Fonts: `PDF_HELVETICA`, `PDF_HELVETICA_BOLD`, `PDF_HELVETICA_OBLIQUE`, `PDF_HELVETICA_BOLD_OBLIQUE`,
`PDF_TIMES`, `PDF_TIMES_BOLD`, `PDF_TIMES_ITALIC`, `PDF_TIMES_BOLD_ITALIC`, `PDF_COURIER`,
`PDF_COURIER_BOLD`, `PDF_COURIER_OBLIQUE`, `PDF_COURIER_BOLD_OBLIQUE`.

Not supported: embedded TrueType/OpenType fonts (so no text outside Windows-1252), reading or
editing existing PDFs, forms, encryption, PDF/A.

---

*See `TAURARO_MASTER_SPECIFICATION.md` for the full language specification.*
