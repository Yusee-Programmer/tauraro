# std.compress — Compression

```tauraro
from std.compress.zlib import Zlib
from std.compress.gzip import Gzip
```

> **Opt-in** — compile with `-DTAURARO_COMPRESS_ZLIB -lz` to enable real compression.
> Without those flags, `compress` returns the original input and `decompress` returns the input unchanged.

---

## std.compress.flate — built-in DEFLATE, zlib and gzip

A DEFLATE decoder and encoder in C (no `-lz`), with raw, zlib (RFC 1950) and gzip (RFC 1952)
framing. `std.compress.zlib` and `std.compress.gzip` fall back to it when the runtime is built
without zlib, and `HttpSession` uses it for `Content-Encoding: gzip` / `deflate`.

```tauraro
from std.compress.flate import Flate

mut z = Flate.zlib_compress(data, 6)          # level 1..9
mut back = Flate.zlib_decompress(z)?
mut page = Flate.decompress_auto(body)?       # gzip, zlib or raw, sniffed
```

| Method | Description |
|---|---|
| `inflate(data)?`, `inflate_max(data, max)?`, `deflate(data, level)` | Raw DEFLATE (RFC 1951). |
| `zlib_decompress(data)?`, `zlib_decompress_max(data, max)?`, `zlib_compress(data, level)` | zlib streams; the Adler-32 is checked. |
| `gunzip(data)?`, `gunzip_max(data, max)?`, `gzip(data, level)` | gzip; every member is decoded and CRC-32 / ISIZE checked. |
| `decompress_auto(data)?`, `decompress_auto_max(data, max)?` | Detect gzip / zlib / raw from the first bytes. |
| `crc32(data)`, `adler32(data)`, `is_gzip(data)` | Checksums and sniffing. |

Decoding raises on corrupt, truncated or checksum-failing input, and when the output would pass
the limit (`FLATE_DEFAULT_MAX_OUT` = 1 GiB for the plain variants; `max <= 0` = unlimited). The
encoder uses LZ77 with hash chains and picks fixed or dynamic Huffman per block; output is within
a few percent of zlib at the same level.

---

## std.compress.zlib — Zlib

Uses the system zlib when the runtime is built with `-DTAURARO_COMPRESS_ZLIB -lz`, otherwise
`std.compress.flate` (same output format).

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Zlib.compress` | `(input: str, ilen: int) -> str` | `str` | Compress `ilen` bytes using zlib format (with header/checksum). |
| `Zlib.decompress` | `(input: str, ilen: int, max_out: int) -> str` | `str` | Decompress a zlib stream, up to `max_out` bytes. |
| `Zlib.deflate` | `(input: str, ilen: int) -> str` | `str` | Raw DEFLATE (no zlib wrapper). |
| `Zlib.inflate` | `(input: str, ilen: int, max_out: int) -> str` | `str` | Decompress raw DEFLATE output. |
| `Zlib.compressed_len` | `(s: str) -> int` | `int` | Exact byte length of a `compress`/`deflate`/`decompress`/`inflate` result. These results are length-tracked strings, so embedded `0x00` bytes are preserved and `compressed_len` (equivalently `s.len()`) is exact. |

> **Binary blobs** — compressed output may contain null bytes.
> Results are length-tracked (`.len()` / `compressed_len()` are exact). Use the `ilen` / `max_out` parameters to bound reads; do not rely on null termination of foreign buffers.

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
| `Gzip.compressed_len` | `(s: str) -> int` | `int` | Exact byte length of a `Gzip.compress()` result. Not a `strlen` — gzip output always contains embedded NUL bytes (at minimum in the MTIME field); the result string carries its own length, so this is per-result (no "last call" global) and thread-safe. |

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

The by-name API also accepts `"bzip2"` (compress and decompress) and, for
`decompress` only, `"xz"`, `"lzma"` and `"brotli"` (the native codecs below).
`Compress.detect(data) -> str` names the format from its magic bytes
(`"zstd"`, `"lz4"`, `"bzip2"`, `"xz"`, `"gzip"`, or `""`; brotli and raw LZ4
blocks have no magic). Note that importing `std.compress` links all of the
native codecs; import a sub-module directly to link only one.

---

## Native codecs: bzip2, xz / LZMA, Brotli

```tauraro
from std.compress.bzip2  import Bzip2, Bzip2Decoder, Bzip2Encoder
from std.compress.xz     import Xz, XzDecoder, Lzma, LzmaDecoder
from std.compress.brotli import Brotli, BrotliDecoder
```

Vendored, permissively licensed C codecs compiled into each module's
`*_native.c` shim (see `std/compress/vendor/README.md`). **No external library
and no build flag**; they build with gcc, clang and MinGW on Linux, macOS and
Windows.

| Module | Codec | Licence |
|---|---|---|
| `std.compress.bzip2` | libbzip2 1.0.8 (decode + encode) | bzip2 licence (BSD-style) |
| `std.compress.xz` | XZ Embedded 2024-12-30 for `.xz`; a small built-in decoder for `.lzma` | 0BSD |
| `std.compress.brotli` | Google brotli 1.2.0 decoder, with the RFC 7932 static dictionary | MIT |

The API is the same for every codec. Inputs and outputs are binary `str`s
(length-tracked, NUL bytes preserved); errors are raised (`throws str`).

| Call | Description |
|---|---|
| `X.decompress(data) throws str -> str` | Decode a complete input; output limited to 1 GiB (`COMPRESS_DEFAULT_MAX_OUT`). |
| `X.decompress_max(data, max_out) throws str -> str` | Same with an explicit limit (decompression-bomb guard); `max_out <= 0` means unlimited. Exceeding it raises `"... exceeds the max_out limit"`. |
| `XDecoder.init(max_out) throws str -> XDecoder` | Streaming decoder. |
| `dec.feed(chunk) throws str -> str` | Decode the next chunk (any size, even 1 byte) and return the output it completed. |
| `dec.finish() throws str -> str` | End of input: returns the remaining output, frees the decoder, raises if the input was truncated. |
| `dec.close()` | Free the decoder early (after an error it is already closed). |
| `Bzip2.compress(data, level) throws str -> str` | One bzip2 stream; `level` 1..9 = 100..900 KB blocks. Output is byte-identical to Python's `bz2.compress`. |
| `Bzip2Encoder.init(level)`, `enc.feed(chunk) -> str`, `enc.finish() -> str` | Streaming bzip2 encoder. |
| `Bzip2.is_bzip2(data)`, `Xz.is_xz(data)` | Magic-number checks. |
| `Bzip2Decoder.streams()` | Number of complete bzip2 streams decoded so far. |

Streaming decoders keep their output in a buffer that `feed` empties, so
memory stays bounded by the chunk sizes (plus the codec's window: up to
900 KB for bzip2, the dictionary size declared by `.xz` / `.lzma` files, up
to 16 MiB for brotli). One-shot calls grow their output geometrically (no
quadratic copying).

What is supported:

- **bzip2**: block and stream CRCs are verified; concatenated streams
  (`cat a.bz2 b.bz2`, pbzip2 output) decode to the concatenated data; garbage
  after a stream raises. Empty input decodes to `""` (as in Python).
- **xz**: LZMA2; CRC32, CRC64 and SHA-256 checks (verified; unknown check IDs
  are rejected); multiple blocks; concatenated streams and stream padding;
  BCJ filters x86, ARM, ARM-Thumb, ARM64, PowerPC, IA-64, SPARC and RISC-V.
  Not supported: the Delta filter, and dictionaries above 1.5 GiB.
- **lzma** (`.lzma`, "LZMA alone"): lc/lp/pb up to 8/4/4, known or unknown
  uncompressed size, with or without end-of-payload marker.
- **brotli**: the complete RFC 7932 format, including the static dictionary
  and transforms. A brotli stream has no magic number; data after its end
  raises. Large-window brotli (non-RFC) is not enabled.

Compression is provided for bzip2 only.

Corrupt and truncated inputs raise; they never crash (the shims were fuzzed
under ASan/UBSan with randomly corrupted Python-produced streams).

```tauraro
from std.compress.bzip2 import Bzip2
from std.compress.xz import Xz
from std.io.tar import TarReader

def open_tarball(data: str, name: str) throws str -> TarReader:
    mut raw = data
    if name.ends_with(".tar.bz2") or name.ends_with(".tbz2"):
        raw = Bzip2.decompress(data)?
    elif name.ends_with(".tar.xz") or name.ends_with(".txz"):
        raw = Xz.decompress(data)?
    return TarReader.open(raw, raw.len())
```

Decoding an HTTP `Content-Encoding: br` body:

```tauraro
from std.compress.brotli import Brotli
mut html = Brotli.decompress_max(body, 64 << 20)?
```

Throughput (one-shot decode of a 26.6 MB text corpus, x86-64, shim at -O2,
compared with Python 3.14's `bz2` / `lzma` / `brotli` 1.2.0 C extensions):
bzip2 decode 26 MB/s (Python 31), bzip2 `-9` encode 10.7 MB/s (10.1), xz
decode 39 MB/s (79; XZ Embedded is smaller and slower than liblzma), `.lzma`
decode 42 MB/s (85), brotli decode 167-173 MB/s (250-278).
