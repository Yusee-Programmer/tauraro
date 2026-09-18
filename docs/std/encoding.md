# std.encoding

Data encoding and decoding: JSON, TOML, YAML, Base64, and hexadecimal.

## Import

```tauraro
# Import the whole module
from std.encoding import JsonDoc, JsonRef, JsonWriter, Json, Base64, Hex

# Or import specific sub-modules
from std.encoding.json   import JsonDoc, JsonRef, JsonWriter, Json
from std.encoding.toml   import TomlValue, Toml
from std.encoding.yaml   import YamlValue, Yaml
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
