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


---

## Pure-Tauraro codecs: LZ4, Zstandard, xxHash

These need **no external library and no build flag** — they are plain Tauraro
and behave identically on every platform (Linux/macOS/Windows/Android, bare/WASM
friendly code paths).

```tauraro
from std.compress.lz4  import Lz4
from std.compress.zstd import Zstd
from std.compress.xxhash import Xxh32, Xxh64
from std.compress.common import CBuf, CResult
from std.compress import Compress        # by-name API
```

### Binary data convention and `CResult`

Inputs are a `str` used as a raw byte buffer plus an **explicit byte length**
(never `strlen`, so embedded NULs are fine). Outputs are a `CResult`:

| Member | Description |
|---|---|
| `res.ok: bool` | `true` on success |
| `res.err: str` | reason when `ok == false` (`""` on success) |
| `res.data: CBuf` | output bytes: `res.data.data` (`Pointer[char]`) and `res.data.len` |
| `res.len() -> int` | output byte count (0 on failure) |
| `res.to_str() -> str` | byte-exact `str` copy of the output (use `len()` for its length) |
| `res.free()` | release the output buffer |

`CBuf` (std.compress.common) is the growable buffer: `init(cap)`, `push_u8/u16le/u32le/u64le`, `push_raw(ptr, n)`, `push_str(s, n)`, `push_fill`, `copy_back(dist, n)`, `to_str()`, `free()`.

Every decompressor validates all lengths/offsets against the input and takes a
`max_out` limit (decompression-bomb guard); malformed input returns
`ok == false`, never a crash (fuzzed under ASan/UBSan).

### std.compress.lz4 — `Lz4`

| Method | Signature | Description |
|---|---|---|
| `Lz4.compress_block` | `(data: str, n: int) -> CResult` | Raw LZ4 block (greedy hash matcher, "LZ4 fast" strategy). |
| `Lz4.decompress_block` | `(data: str, n: int, max_out: int) -> CResult` | Decode a raw block; fails if output would exceed `max_out`. |
| `Lz4.compress_frame` | `(data: str, n: int, checksum: bool) -> CResult` | LZ4 frame: independent 64 KiB blocks, content size recorded, optional XXH32 content checksum, incompressible blocks stored. |
| `Lz4.decompress_frame` | `(data: str, n: int, max_out: int) -> CResult` | Decode one or more frames (skippable frames ignored); verifies header, block and content checksums, content size. Dictionaries unsupported. |
| `Lz4.max_compressed_size` | `(n: int) -> int` | Worst-case block size for `n` input bytes. |

Interoperable with the `lz4` CLI and liblz4 (validated against `LZ4_compress_default`, `LZ4_compress_HC`, `LZ4F_compressFrame` for all block sizes, linked/independent blocks and checksum options).

### std.compress.zstd — `Zstd`

| Method | Signature | Description |
|---|---|---|
| `Zstd.compress` | `(data: str, n: int, checksum: bool) -> CResult` | Zstandard frame. Per 128 KiB block: RLE block, or greedy-LZ77 compressed block using the *predefined* FSE tables with raw literals, or Raw block — whichever is valid/smallest. Optional XXH64-based content checksum. |
| `Zstd.decompress` | `(data: str, n: int, max_out: int) -> CResult` | Full RFC 8878 decoder: Raw/RLE/Compressed blocks, Huffman (1/4 streams, treeless) and FSE (predefined/RLE/compressed/repeat) tables, repeat offsets, checksum + content-size verification, concatenated/skippable frames. |

Limits: dictionaries (non-zero `Dictionary_ID`) and legacy pre-1.0 formats are rejected with an error; the encoder does not Huffman-compress literals or use custom FSE tables/repeat offsets, so its ratio is below `zstd -1` on text (but always valid — verified with `zstd -d` and Python's `compression.zstd`).

### std.compress.xxhash — `Xxh32`, `Xxh64`

| Method | Description |
|---|---|
| `Xxh32.hash(data: str, len: int, seed: int) -> int` | XXH32 (unsigned 32-bit value). `hash_ptr(p, len, seed)` takes a raw `Pointer[char]`. |
| `Xxh64.hash(data: str, len: int, seed: int) -> int` | XXH64 as an `int` carrying the 64-bit pattern. `Xxh64.low32(...)` is the unsigned low 32 bits (what zstd stores). |

### Unified API — `Compress`

```tauraro
from std.compress import Compress
mut c = Compress.compress("zstd", data, dlen)               # "lz4", "lz4-block", "zstd"
mut d = Compress.decompress("zstd", c.to_str(), c.len(), 1 << 26)
if d.ok: print(d.to_str())
c.free()
d.free()
```

`Compress.algorithms() -> List[str]`, `Compress.supports(name) -> bool`, `Compress.compress_with(name, data, n, checksum)`. `"lz4-block"` has no framing — keep the original size and pass it (or an upper bound) as `max_out`. Unknown names return `ok == false`.
