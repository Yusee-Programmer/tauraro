# std.encoding

Data encoding and decoding: JSON, CSV, TOML, INI, YAML, XML, Base64, and hexadecimal.

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

*See `TAURARO_MASTER_SPECIFICATION.md` for the full language specification.*
