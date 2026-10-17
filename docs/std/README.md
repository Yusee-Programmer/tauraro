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
| [`std.compress`](compress.md) | Compression: zlib compress/decompress, raw deflate/inflate (`-lz` required), gzip container format, zip; pure-Tauraro **LZ4** (block+frame), **Zstandard** (decoder + encoder), xxHash32/64; vendored **bzip2** (decode + encode), **xz / LZMA** and **Brotli** decoders with streaming and max-size limits; and a unified `Compress` by-name API |
| [`std.config`](config.md) | Layered configuration: defaults < TOML/YAML/JSON/INI/.env files < environment (`APP_DB__HOST` → `db.host`) < command line (argparse `Namespace` or `--key=value`) < overrides; typed dotted-path getters (bool words, durations, byte sizes, lists), `require_*` errors naming the source layer, `${VAR}`/`${other.key}` interpolation with cycle detection, declared specs, unknown-key detection, `std.validate.schema` integration, masked `dump()`, reload + file watching |
| [`std.crypto`](crypto.md) | Cryptography: SHA-1/2/3, SHAKE, BLAKE2/3, HMAC, HKDF, PBKDF2, Argon2, bcrypt, AES (CBC, GCM), ChaCha20-Poly1305, X25519, RSA/ECDSA (P-256/P-384) and ECDH, X.509 parsing + chain verification, DER / PKCS#10 CSRs / certificate building (`std.crypto.der`, `std.crypto.csr`), CSPRNG, HOTP/TOTP, UUID/ULID, JWT (HS256) |
| [`std.crypto`](crypto.md) | Cryptography: SHA-1/2/3, SHAKE, BLAKE2/3, HMAC, HKDF, PBKDF2, Argon2, bcrypt, AES (CBC, GCM), ChaCha20-Poly1305, X25519, RSA/ECDSA (P-256/P-384) and ECDH, X.509 parsing + chain verification, CSPRNG, HOTP/TOTP, UUID/ULID, JWT (HS/RS/PS/ES/EdDSA), JWK / JWKS + thumbprints |
| [`std.encoding`](encoding.md) | Data encoding: JSON, CSV, TOML, INI, YAML, XML, Base64, Base32, Hex, CBOR, Protocol Buffers wire format, PDF writer, Excel workbooks (.xlsx) |
| [`std.crypto`](crypto.md) | Cryptography: SHA-1/2/3, SHAKE, BLAKE2/3, HMAC, HKDF, PBKDF2, Argon2, bcrypt, AES (CBC, GCM), ChaCha20-Poly1305, X25519, RSA/ECDSA (P-256/P-384) and ECDH, X.509 parsing + chain verification, scrypt, age file encryption, OpenSSH keys / fingerprints / authorized_keys / SSHSIG signatures, CSPRNG, HOTP/TOTP, UUID/ULID, JWT (HS256) |
| [`std.encoding`](encoding.md) | Data encoding: JSON, CSV, TOML, INI, YAML, XML, Base64, Base32, Hex, CBOR, Protocol Buffers wire format, Bech32/Bech32m, PDF writer, Excel workbooks (.xlsx) |
| [`std.crypto`](crypto.md) | Cryptography: SHA-1/2/3, SHAKE, BLAKE2/3, HMAC, HKDF, PBKDF2, Argon2, bcrypt, AES (CBC, GCM), ChaCha20-Poly1305, X25519, RSA/ECDSA (P-256/P-384) and ECDH, X.509 parsing + chain verification, CSPRNG, HOTP/TOTP, UUID/ULID, JWT (HS256) |
| [`std.encoding`](encoding.md) | Data encoding: JSON, CSV, TOML, INI, YAML, XML, Base64, Base32, Hex, CBOR, Protocol Buffers wire format, PDF writer |
| [`std.html`](html.md) | HTML5: forgiving WHATWG tokenizer and tree builder (html5lib-compatible trees), DOM (`HtmlDocument`/`HtmlNode`), CSS selectors, escaping with the full entity table, readable text extraction, link resolution and an allow-list sanitizer |
| [`std.gpu`](gpu.md) | GPU compute: CUDA/OpenCL/CPU device API (`Device`/`Buffer[T]`/`Module`/`Kernel`), Tauraro `@kernel` → PTX/SPIR-V, and OpenMP CPU dispatch (`Gpu.parallel`) |
| [`std.image`](image.md) | Raster images: BMP, QOI, PNG, JPEG (baseline + progressive decode, encode), GIF (animated decode/encode); resizing, colour adjustments, blur/sharpen/edges, geometry; shared `Image` (RGBA8) type and format-sniffing `decode()` |
| [`std.audio`](audio.md) | Audio: `AudioBuffer`, WAV decode/encode, MP3 and FLAC decoding, resampling, channel mixing, level and edit helpers, tone generators; spectra and filters via `std.audio.dsp` |
| [`std.io`](io.md) | File I/O, directory operations, path manipulation, console, buffered I/O, ZIP archive read/write (`ZipWriter`/`ZipReader`), tar archive read/write (`TarWriter`/`TarReader`) |
| [`std.iter`](iter.md) | Range construction, int/float vector transforms, folds, prefix sums, normalization |
| [`std.log`](log.md) | Leveled structured logging: text/JSON formats, stdout/file sinks, per-target filtering (`Logger`/`Fields`) |
| [`std.math`](math.md) | Integer/float math, bit ops, statistics, random, `BigInt`, linear algebra, exact `Decimal`/`Rational`, `BigFloat`, n-dimensional `Tensor` with broadcasting, any-length FFT (`std.math.fft`), DSP: windows, STFT, filters, FIR/IIR design, Welch (`std.math.dsp`) |
| [`std.net`](net.md) | TCP, UDP, DNS, URL, HTTP client, `HttpSession`, verified TLS 1.3 client and server (`std.net.tls`), ACME / Let's Encrypt certificates (`std.net.acme`), HTTPS client, HTTP/2 client + server + HPACK (`std.net.http2`, `std.net.http2_server`), gRPC client + server (`std.net.grpc`), HTTP server + router, WebSocket, SMTP (STARTTLS), MIME email messages (`std.net.email`), media types + content sniffing (`std.net.mime`), IMAP and POP3 clients, DNS wire protocol (any record type), mDNS / DNS-SD browsing, NTP client, MQTT 3.1.1 / 5.0 client + embeddable broker (`std.net.mqtt`) |
| [`std.net`](net.md) | TCP, UDP, DNS, URL, HTTP client, `HttpSession`, verified TLS 1.3 client (`std.net.tls`), HTTPS client, HTTP/2 client + HPACK, HTTP server + router, WebSocket, SMTP (STARTTLS), MIME email messages (`std.net.email`), media types + content sniffing (`std.net.mime`), IMAP and POP3 clients, DNS wire protocol (any record type), mDNS / DNS-SD browsing, NTP client, MQTT 3.1.1 / 5.0 client + embeddable broker (`std.net.mqtt`), OAuth 2.0 / OpenID Connect client (`std.net.oauth`), SOCKS5 / HTTP CONNECT proxies + NO_PROXY (`std.net.proxy`) |
| [`std.prof`](prof.md) | Profiling: statistical CPU sampling (`CpuProfiler`), process memory stats (`MemProf`), metrics (`Counter`/`Gauge`/`Histogram`/`Registry`); also `tauraroc --prof` |
| [`std.regex`](regex.md) | POSIX extended regex: match, find, replace, split, count, capture groups (numbered + named), `Match`/`Captures`, backreference replace |
| [`std.serialize`](serialize.md) | Value serialization: `Pickle` (own compact binary format for scalars + nested List/Dict/Vec/Set), `Msgpack` (real, wire-compatible MessagePack codec) |
| [`std.string`](string.md) | String utilities (Str), formatting (Fmt), parsing, line/word splitting, `split_to_vec` |
| [`std.sys`](sys.md) | Environment, file system, processes, subprocesses (argv lists, pipes, env, timeouts), timing, OS/platform info, dates and IANA time zones (`std.sys.tz`: TZif, POSIX TZ rules, embedded zone table), signals, `.env`, cron expressions, memory maps, shared memory + named semaphores, pipes/FIFOs, Unix-domain sockets, shared-library loading (`dylib`) and plugins, sandboxing (seccomp + Landlock, pledge + unveil) |
| [`std.term`](term.md) | Terminal UI: ANSI SGR styling/colors (`Term`/`Style`/`Color`), cursor control, terminal size query, progress bar and spinner |
| [`std.test`](test.md) | Lightweight unit-testing framework |
| [`std.text`](text.md) | Text processing: line diff + unified diff, Levenshtein distance, word-wrap/indent, Markdown, shell word splitting/quoting (`Shlex`) |
| [`std.unicode`](unicode.md) | UTF-8 codepoint iteration, slicing, Unicode 16 classification; normalization (NFC/NFD/NFKC/NFKD), full case mapping and case folding, grapheme/word/sentence segmentation (UAX #29), terminal display width |
| [`std.version`](version.md) | Semantic Versioning 2.0.0: strict/loose parsing, spec precedence, npm-style bumps; npm/cargo version requirements (`^`, `~`, x-ranges, hyphen ranges, `,`/space AND, `\|\|` OR, pre-release rule, `includePrerelease`), `max_satisfying` |

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
