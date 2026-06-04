# std.compress — Compression

```tauraro
from std.compress.zlib import Zlib
```

> **Opt-in** — compile with `-DTAURARO_COMPRESS_ZLIB -lz` to enable real compression.
> Without those flags, `compress` returns the original input and `decompress` returns the input unchanged.

---

## std.compress.zlib — Zlib

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Zlib.compress` | `(input: str, ilen: int) -> str` | `str` | Compress `ilen` bytes using zlib format (with header/checksum). |
| `Zlib.decompress` | `(input: str, ilen: int, max_out: int) -> str` | `str` | Decompress a zlib stream, up to `max_out` bytes. |
| `Zlib.deflate` | `(input: str, ilen: int) -> str` | `str` | Raw DEFLATE (no zlib wrapper). |
| `Zlib.inflate` | `(input: str, ilen: int, max_out: int) -> str` | `str` | Decompress raw DEFLATE output. |
| `Zlib.compressed_len` | `(s: str) -> int` | `int` | Byte length of a result string (via `strlen`). For binary blobs track length manually. |

> **Binary blobs** — compressed output may contain null bytes.
> Use the `ilen` / `max_out` parameters to bound reads; do not rely on null termination.

---

## Example

```tauraro
from std.compress.zlib import Zlib

mut data = "hello world hello world hello"
mut dlen = 29   # byte count

mut compressed = Zlib.compress(data, dlen)
print("compressed len: " + str(Zlib.compressed_len(compressed)))

mut original = Zlib.decompress(compressed, Zlib.compressed_len(compressed), 256)
print(original)   # "hello world hello world hello"

# Raw deflate / inflate
mut raw_cmp  = Zlib.deflate(data, dlen)
mut raw_orig = Zlib.inflate(raw_cmp, Zlib.compressed_len(raw_cmp), 256)
print(raw_orig)
```
