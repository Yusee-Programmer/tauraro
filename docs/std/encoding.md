# std.encoding

Data encoding and decoding: JSON, Base64, and hexadecimal.

## Import

```tauraro
# Import the whole module
from std.encoding import JsonValue, Json, Base64, Hex

# Or import specific sub-modules
from std.encoding.json   import JsonValue, Json, JsonParser
from std.encoding.base64 import Base64
from std.encoding.hex    import Hex
```

---

## JSON — `std.encoding.json`

A full JSON parser and serializer.  All JSON values are represented as `Pointer[JsonValue]`
nodes allocated on the heap.  The `Json` class provides static helpers for the most common
operations.

### `JsonValue` class

Every JSON value is tagged with an integer discriminant:

| Tag | Constant helper | Meaning |
|-----|-----------------|---------|
| 0 | `JSON_NULL()` | `null` |
| 1 | `JSON_BOOL()` | boolean |
| 2 | `JSON_INT()` | integer |
| 3 | `JSON_FLOAT()` | floating-point |
| 4 | `JSON_STR()` | string |
| 5 | `JSON_ARRAY()` | array |
| 6 | `JSON_OBJ()` | object |

#### Constructors (static)

All constructors return `Pointer[JsonValue]`.

| Method | Description |
|--------|-------------|
| `JsonValue.init_null()` | Create a `null` node |
| `JsonValue.init_bool(b: bool)` | Create a boolean node |
| `JsonValue.init_int(n: int)` | Create an integer node |
| `JsonValue.init_float(f: float)` | Create a float node |
| `JsonValue.init_str(s: str)` | Create a string node |
| `JsonValue.init_array()` | Create an empty array node |
| `JsonValue.init_object()` | Create an empty object node |

#### Type checks (`self` methods)

```tauraro
v.read().is_null()    -> bool
v.read().is_bool()    -> bool
v.read().is_int()     -> bool
v.read().is_float()   -> bool
v.read().is_str()     -> bool
v.read().is_array()   -> bool
v.read().is_object()  -> bool
v.read().is_number()  -> bool   # true for int or float
```

#### Value getters

```tauraro
v.read().get_bool()   -> bool
v.read().get_int()    -> int
v.read().get_float()  -> float
v.read().get_str()    -> str
v.read().as_float()   -> float  # coerces int to float if needed
```

#### Array operations

```tauraro
arr.read().push(item: Pointer[JsonValue])
arr.read().array_get(i: int) -> Pointer[JsonValue]
arr.read().array_len()       -> int
```

#### Object operations

```tauraro
obj.read().obj_set(key: str, v: Pointer[JsonValue])
obj.read().obj_get(key: str) -> Pointer[JsonValue]   # returns null node if key absent
obj.read().obj_has(key: str) -> bool
obj.read().obj_len()         -> int
obj.read().obj_key(i: int)   -> str
obj.read().obj_val(i: int)   -> Pointer[JsonValue]
```

#### Serialization

```tauraro
v.read().to_str()              -> str   # compact JSON string
v.read().to_pretty(indent: int) -> str  # pretty-printed with 2-space indentation
                                         # (the `indent` parameter is currently unused)
```

### Resource cleanup

`JsonValue` trees are heap-allocated and not managed by auto-drop. Call
`v.read().dispose()` to recursively free a whole tree (child nodes, their
`items`/`keys` lists, and the node itself) once you're done with it. Note that
`dispose()` does **not** free `s_val` — string-node values are commonly string
literals, which must never be passed to a free function.

```tauraro
mut v = Json.parse(src)
# ... use v ...
v.read().dispose()
```

### `Json` class (static API)

```tauraro
Json.parse(src: str)           -> Pointer[JsonValue]   # parse JSON string
Json.stringify(v: Pointer[JsonValue]) -> str            # compact serialization
Json.pretty(v: Pointer[JsonValue])    -> str            # 2-space indented
Json.null_val()                -> Pointer[JsonValue]
Json.bool_val(b: bool)         -> Pointer[JsonValue]
Json.int_val(n: int)           -> Pointer[JsonValue]
Json.float_val(f: float)       -> Pointer[JsonValue]
Json.str_val(s: str)           -> Pointer[JsonValue]
Json.array()                   -> Pointer[JsonValue]
Json.object()                  -> Pointer[JsonValue]
```

### `JsonParser` class

Low-level recursive-descent parser.  Use `Json.parse()` instead unless you need streaming or
incremental parsing.

```tauraro
mut p = JsonParser.init(src: str)
p.parse() -> Pointer[JsonValue]
```

### Example

```tauraro
from std.encoding.json import Json, JsonValue

def main():
    # Build a JSON object programmatically
    mut obj = Json.object()
    obj.read().obj_set("name",    Json.str_val("Tauraro"))
    obj.read().obj_set("version", Json.int_val(1))
    obj.read().obj_set("stable",  Json.bool_val(true))

    mut arr = Json.array()
    arr.read().push(Json.int_val(1))
    arr.read().push(Json.int_val(2))
    arr.read().push(Json.int_val(3))
    obj.read().obj_set("items", arr)

    print(Json.stringify(obj))
    # → {"name":"Tauraro","version":1,"stable":true,"items":[1,2,3]}
    print(Json.pretty(obj))

    # Parse JSON from a string
    mut v = Json.parse("{\"x\": 42, \"y\": [1, 2]}")
    print(v.read().obj_get("x").read().get_int())   # → 42
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
