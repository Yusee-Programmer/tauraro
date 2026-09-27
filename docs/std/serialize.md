# std.serialize

Value serialization: `Pickle` (a from-scratch, compact binary format for
scalars and nested `List`/`Dict`/`Vec`/`Set`) and `Msgpack` (a real,
wire-compatible [MessagePack](https://msgpack.org) encoder/decoder).

## Import

```tauraro
# Import the whole module
from std.serialize import PVal, Pickle, MPVal, Msgpack

# Or import specific sub-modules
from std.serialize.pickle  import PVal, Pickle
from std.serialize.msgpack import MPVal, Msgpack
```

---

## Why there's no generic "pickle any class"

Python's `pickle` can serialize *arbitrary* objects because CPython gives it
runtime introspection (`__reduce__`, `__dict__`, `__class__`, ...). Tauraro
is statically typed and compiles straight to C with no such runtime type
registry, so a library has no way to ask an arbitrary `T` "what are your
field names and values?" without deep compiler changes.

Both modules below work around this the same way JSON/TOML do in
`std.encoding`: a **tagged dynamic value type** (`PVal` for pickle, `MPVal`
for msgpack) that covers `None`/`bool`/`int`/`float`/`str` plus nested
lists/maps. You build one of these explicitly (or decode one back), and if
you want a *class* to round-trip, you write two small hook methods
yourself — see [Class-pickling convention](#class-pickling-convention)
below. This is real, useful, honest serialization for the shapes it
supports (e.g. persisting a `Dict[str, List[int]]` config/cache to disk) —
it is not a silently-wrong imitation of Python's pickle.

---

## Pickle — `std.serialize.pickle`

A own-format binary serializer/deserializer: compact, simple to reason
about, and round-trip-correct — but **not** wire-compatible with Python's
actual `pickle` format (there was no reason to match Python's binary
layout; if you need cross-language interop, use `Msgpack` instead).

```tauraro
from std.serialize.pickle import PVal, Pickle

mut cfg = PVal.init_map()
cfg.map_set("name", PVal.init_str("tauraro"))
cfg.map_set("retries", PVal.init_int(3))

mut bytes_, blen = Pickle.dumps_len(cfg)   # (str, int) -- always use `blen`,
mut back = Pickle.loads(bytes_, blen)      # never strlen() on the bytes
print(back.map_get("retries").get_int())   # 3
```

### Binary format

Every value starts with a 1-byte tag:

| Tag | Meaning | Payload |
|---|---|---|
| `0x00` | `None` | none |
| `0x01` | `bool` false | none |
| `0x02` | `bool` true | none |
| `0x03` | `int` | 8 bytes, little-endian, two's complement |
| `0x04` | `float` | 8 bytes, little-endian, IEEE-754 double bit pattern |
| `0x05` | `str` | 4-byte LE length `N`, then `N` raw bytes (embedded-NUL-safe) |
| `0x06` | `list` | 4-byte LE count `N`, then `N` values back-to-back |
| `0x07` | `map` | 4-byte LE count `N`, then `N` (4-byte-LE-length-prefixed key bytes, value) pairs |

Integers are always encoded as full 8 bytes regardless of magnitude —
this format favors simplicity and round-trip correctness over wire size.

### `PVal` — tagged dynamic value

Mirrors `std.encoding.toml`'s `TomlValue` shape.

| Method | Returns | Description |
|---|---|---|
| `PVal.init_none()` / `init_bool(b)` / `init_int(n)` / `init_float(f)` | `PVal` | scalar constructors |
| `PVal.init_str(s: str)` | `PVal` | string constructor; length derived via strlen (NUL-free text only) |
| `PVal.init_str_n(s: str, n: int)` | `PVal` | **binary-safe** string constructor — `n` is the true byte length, so `s` may contain embedded NUL bytes |
| `PVal.init_list()` / `init_map()` | `PVal` | empty container constructors |
| `.is_none()` / `.is_bool()` / `.is_int()` / `.is_float()` / `.is_str()` / `.is_list()` / `.is_map()` | `bool` | type check |
| `.get_bool()` / `.get_int()` / `.get_float()` / `.get_str()` | value | scalar accessors |
| `.get_str_len()` | `int` | true byte length of the string value (use instead of `.len()`/strlen for binary-safe strings) |
| `.list_push(v: PVal)` / `.list_len()` / `.list_get(i: int)` | — | list ops |
| `.map_set(key: str, v: PVal)` / `.map_get(key)` / `.map_has(key)` / `.map_len()` / `.map_key(i)` / `.map_val(i)` | — | map ops |

### `Pickle` static API

| Method | Description |
|---|---|
| `Pickle.dumps(v: PVal, out_len: List[int]) -> str` | serialize; true byte length written into `out_len[0]` |
| `Pickle.dumps_len(v: PVal) -> (str, int)` | same, returned as a `(bytes, length)` tuple |
| `Pickle.loads(data: str, len_: int) -> PVal` | deserialize (length must be explicit — never inferred via strlen) |
| `Pickle.dump_int/_float/_bool/_str(...) -> (str, int)` | one-shot scalar convenience wrappers |
| `Pickle.dump_list_int(xs: List[int]) -> (str, int)` / `dump_list_str(xs: List[str]) -> (str, int)` | one-shot list convenience wrappers |
| `Pickle.dump_dict_str_str(d: Dict[str,str]) -> (str, int)` / `dump_dict_str_int(d: Dict[str,int]) -> (str, int)` | one-shot dict convenience wrappers |
| `Pickle.load_list_int(data, len_) -> List[int]` / `load_list_str(data, len_) -> List[str]` | mirror-image load helpers |

> **Note on call shape.** Always bind a freshly-constructed `PVal` to a
> local before passing it into `dumps_len`/`dumps`/a convenience helper
> when the call site is a tuple-destructuring `let`
> (`mut bytes_, blen = ...`). `mut v = PVal.init_int(3); mut bytes_, blen =
> Pickle.dumps_len(v)` — not `Pickle.dumps_len(PVal.init_int(3))` used
> directly inside a `mut a, b = ...` statement. This works around a
> pre-existing compiler codegen-ordering bug unrelated to this module (see
> `bug2.txt` in the compiler repo, "BUG 2"); it's also just clear style.

### Class-pickling convention

Write a `to_pval(self) -> PVal` method and a matching static
`from_pval(v: PVal) -> T`, and drive them yourself:

```tauraro
class Point:
    x: int
    y: int

extend Point:
    def to_pval(self) -> PVal:
        mut v = PVal.init_map()
        v.map_set("x", PVal.init_int(self.x))
        v.map_set("y", PVal.init_int(self.y))
        return v

    def from_pval(v: PVal) -> Point:
        mut p = Point()
        p.x = v.map_get("x").get_int()
        p.y = v.map_get("y").get_int()
        return p

def main():
    mut p = Point(); p.x = 3; p.y = 4
    mut pv = p.to_pval()
    mut raw, rlen = Pickle.dumps_len(pv)
    mut p2 = Point.from_pval(Pickle.loads(raw, rlen))
```

`pickle.tr` cannot call these hooks automatically for you (Tauraro has no
way to detect "does this arbitrary `T` have a `to_pval` method" at compile
time without a concrete type), but the pattern is a few lines per class and
composes naturally (a class containing another picklable class just calls
the nested `to_pval()`/`from_pval()` inside its own).

---

## MessagePack — `std.serialize.msgpack`

A real [MessagePack](https://github.com/msgpack/msgpack/blob/master/spec.md)
encoder/decoder — **wire-compatible** with any conforming MessagePack
implementation in any language (unlike `Pickle`'s own format).

```tauraro
from std.serialize.msgpack import MPVal, Msgpack

mut m = MPVal.init_map()
m.map_set("id", MPVal.init_int(7))
mut tags = MPVal.init_list()
tags.list_push(MPVal.init_str("a"))
m.map_set("tags", tags)

mut bytes_, blen = Msgpack.dumps_len(m)
mut back = Msgpack.loads(bytes_, blen)
print(back.map_get("id").get_int())   # 7
```

### Coverage

Implements, per the MessagePack spec's format table: `nil`, `bool`
(true/false), positive/negative fixint, int8/16/32/64, uint8/16/32/64,
float64 (float32 is *decoded* for interop but never emitted — Tauraro's
`float` is a double, so encoding always uses the lossless float64 form),
fixstr/str8/16/32, fixarray/array16/32, fixmap/map16/map32.

**Not implemented** (documented gap, not silently wrong): `bin8/16/32` (raw
byte arrays as distinct from `str` — everything textual goes through
`str`), `ext` types, and `timestamp`. A `str` `MPVal` can still carry
arbitrary bytes via `MPVal.init_str_n`, but it's tagged as MessagePack
`str` on the wire, not `bin` — fine for most decoders, but a strict
UTF-8-validating decoder on the receiving end may reject non-UTF-8 bytes
tagged as `str`.

### `MPVal` — tagged dynamic value

Same shape as `Pickle`'s `PVal` (see above) — `init_none/_bool/_int/_float`,
`init_str`/`init_str_n`, `init_list`/`init_map`, `is_*`, `get_*`,
`get_str_len`, `list_push/_len/_get`, `map_set/_get/_has/_len/_key/_val`.

### `Msgpack` static API

Mirrors `Pickle`'s static API exactly (`dumps`, `dumps_len`, `loads`,
`dump_int/_float/_bool/_str`, `dump_list_int/_str`,
`dump_dict_str_str/_str_int`, `load_list_int/_str`) — see the Pickle
section above for signatures. The same "bind to a local before a
tuple-destructuring call" note applies here too.

### Example: byte-level spot check

Because this module is wire-compatible, you can verify specific encodings
against the spec directly:

```tauraro
mut v = MPVal.init_int(0)
mut bytes_, blen = Msgpack.dumps_len(v)
# blen == 1, and the single byte is 0x00 (positive fixint 0)

mut s = MPVal.init_str("hi")
mut sb, sn = Msgpack.dumps_len(s)
# sn == 3: 0xa2 (fixstr, length 2) 'h' 'i'
```

---

*See `TAURARO_MASTER_SPECIFICATION.md` for the full language specification.*
