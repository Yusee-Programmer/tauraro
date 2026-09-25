# std.compress — Compression

```tauraro
from std.compress.zlib import Zlib
from std.compress.gzip import Gzip
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

---

## std.compress.gzip — Gzip

A thin RFC 1952 container wrapper around the same raw-DEFLATE runtime routines `std.compress.zlib.Zlib` wraps: `Gzip.compress` adds the 10-byte gzip header (magic bytes `1f 8b`, compression method, flags, mtime, XFL, OS) and 8-byte footer (CRC32 of the uncompressed data + ISIZE mod 2^32) around a DEFLATE payload; `Gzip.decompress` validates both and reverses it. Output is byte-compatible with real `gzip`/`gunzip` — verified by round-tripping through the actual `gunzip` binary during development.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Gzip.compress` | `(input: str, ilen: int) -> str` | `str` | Compress `ilen` bytes into a complete gzip member (header + DEFLATE + CRC32/ISIZE footer). |
| `Gzip.decompress` | `(input: str, ilen: int, max_out: int) -> str` | `str` | Decompress a gzip member, up to `max_out` bytes. Verifies CRC32 and ISIZE; returns `""` with an error message on mismatch or malformed input. |
| `Gzip.compressed_len` | `(s: str) -> int` | `int` | Exact byte length of the **most recent** `Gzip.compress()` result. Not a `strlen` — gzip output always contains embedded NUL bytes (at minimum in the MTIME field), so call this immediately after `compress()`. |

> **No -DTAURARO_COMPRESS_ZLIB build?** `Gzip.compress` still produces a spec-valid gzip member (via a hand-written DEFLATE "stored block" fallback, RFC 1951 §3.2.4) that real `gunzip` can decompress — but `Gzip.decompress` cannot decode ANY gzip data on that build, since it depends on the same opt-in `_tr_inflate` runtime routine as `Zlib.inflate`.

### Example

```tauraro
from std.compress.gzip import Gzip

mut data = "hello world hello world hello world"
mut dlen = 35   # byte count

mut compressed = Gzip.compress(data, dlen)
mut clen = Gzip.compressed_len(compressed)   # exact length, not strlen
print("compressed len: " + str(clen))

mut original = Gzip.decompress(compressed, clen, 4096)
print(original)   # "hello world hello world hello world"
```
