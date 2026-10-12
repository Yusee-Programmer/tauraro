# Tauraro Standard Library

Complete reference for the Tauraro standard library (`std`).

See also: [Language Documentation](../lang/README.md) ·
[Developer & Contributor Documentation](../dev/README.md) (compiler internals,
building libraries with `taupkg`)

## Modules

| Module | Description |
|---|---|
| [`std.async`](async.md) | Concurrency: channels, tasks, mutexes, semaphores, barriers, StructuredGroup, IOPoll, EventLoop |
| [`std.cli`](cli.md) | CLI parsing: clap-style `Cli`/`CliArgs`, and declarative `std.cli.argparse` (typed/validated values, env fallbacks, groups, nested commands, response files, help and shell completion scripts) |
| [`std.collections`](collections.md) | Data structures: Stack, Queue, Deque, Set, Counter, MinHeap/MaxHeap, LinkedList, Graph, LruCache, BTreeMap, Trie, BloomFilter, UnionFind, BitSet, RingBuffer, SkipList, IntervalTree, Rope, SlotMap, Arena/Slab allocators |
| [`std.compress`](compress.md) | Compression: zlib compress/decompress, raw deflate/inflate (`-lz` required), gzip container format, zip; pure-Tauraro **LZ4** (block+frame), **Zstandard** (decoder + encoder), xxHash32/64, and a unified `Compress` by-name API |
| [`std.crypto`](crypto.md) | Cryptography: SHA-1/2/3, SHAKE, BLAKE2/3, HMAC, HKDF, PBKDF2, Argon2, bcrypt, AES (CBC, GCM), ChaCha20-Poly1305, X25519, RSA/ECDSA (P-256/P-384) and ECDH, X.509 parsing + chain verification, CSPRNG, HOTP/TOTP, UUID/ULID, JWT (HS256) |
| [`std.encoding`](encoding.md) | Data encoding: JSON, CSV, TOML, INI, YAML, XML, Base64, Base32, Hex, CBOR, Protocol Buffers wire format |
| [`std.gpu`](gpu.md) | GPU compute: CUDA/OpenCL/CPU device API (`Device`/`Buffer[T]`/`Module`/`Kernel`), Tauraro `@kernel` → PTX/SPIR-V, and OpenMP CPU dispatch (`Gpu.parallel`) |
| [`std.image`](image.md) | Raster images: BMP, QOI, PNG, JPEG (baseline + progressive decode, encode), GIF (animated decode/encode); resizing, colour adjustments, blur/sharpen/edges, geometry; shared `Image` (RGBA8) type and format-sniffing `decode()` |
| [`std.audio`](audio.md) | Audio: `AudioBuffer`, WAV decode/encode, MP3 and FLAC decoding, resampling, channel mixing, level and edit helpers, tone generators |
| [`std.io`](io.md) | File I/O, directory operations, path manipulation, console, buffered I/O, ZIP archive read/write (`ZipWriter`/`ZipReader`), tar archive read/write (`TarWriter`/`TarReader`) |
| [`std.iter`](iter.md) | Range construction, int/float vector transforms, folds, prefix sums, normalization |
| [`std.log`](log.md) | Leveled structured logging: text/JSON formats, stdout/file sinks, per-target filtering (`Logger`/`Fields`) |
| [`std.math`](math.md) | Integer/float math, bit ops, statistics, random, `BigInt`, linear algebra, exact `Decimal`/`Rational`, `BigFloat`, n-dimensional `Tensor` with broadcasting |
| [`std.net`](net.md) | TCP, UDP, DNS, URL, HTTP client, `HttpSession`, verified TLS 1.3 client (`std.net.tls`), HTTPS client, HTTP/2 client + HPACK, HTTP server + router, WebSocket, SMTP (STARTTLS), IMAP and POP3 clients, DNS wire protocol (any record type), mDNS / DNS-SD browsing, NTP client |
| [`std.prof`](prof.md) | Profiling: statistical CPU sampling (`CpuProfiler`), process memory stats (`MemProf`), metrics (`Counter`/`Gauge`/`Histogram`/`Registry`); also `tauraroc --prof` |
| [`std.regex`](regex.md) | POSIX extended regex: match, find, replace, split, count, capture groups (numbered + named), `Match`/`Captures`, backreference replace |
| [`std.serialize`](serialize.md) | Value serialization: `Pickle` (own compact binary format for scalars + nested List/Dict/Vec/Set), `Msgpack` (real, wire-compatible MessagePack codec) |
| [`std.string`](string.md) | String utilities (Str), formatting (Fmt), parsing, line/word splitting, `split_to_vec` |
| [`std.sys`](sys.md) | Environment, file system, processes, timing, OS/platform info, signals, `.env`, cron expressions, memory maps, shared memory + named semaphores, pipes/FIFOs, Unix-domain sockets |
| [`std.term`](term.md) | Terminal UI: ANSI SGR styling/colors (`Term`/`Style`/`Color`), cursor control, terminal size query, progress bar and spinner |
| [`std.test`](test.md) | Lightweight unit-testing framework |
| [`std.text`](text.md) | Text processing: line diff + unified diff, Levenshtein distance, word-wrap/indent, Markdown, shell word splitting/quoting (`Shlex`) |
| [`std.unicode`](unicode.md) | UTF-8 codepoint iteration, slicing, case conversion, Unicode classification |

## Import conventions

```tauraro
# Import specific items
from std.async.channel import Channel
from std.collections.stack import Stack
from std.io.file import read_file, write_file
from std.math.int import abs_int, gcd, is_prime

# Import a whole sub-module via its mod.tr
from std.async import Channel, Task, Mutex
```

## Async context requirement

The following features are **only valid inside `async def` functions**:

| Keyword / class | Error code | Requirement |
|---|---|---|
| `await` | C-4 | must be inside `async def` |
| `spawn:` | C-5 | must be inside `async def` |
| `taskgroup:` | C-6 | must be inside `async def` |

The compiler enforces all three at compile time.

## Memory model

All stdlib classes use the *opaque handle* pattern — a `handle: Pointer[char]` field
pointing to a heap-allocated C struct.  Call `.free()` when you are done with an
instance, or rely on the compiler's automatic ownership inference (heap-allocated
variables are freed at scope exit when not returned or moved).

---

*Generated for Tauraro 0.x — see `TAURARO_MASTER_SPECIFICATION.md` for the full
language specification.*
