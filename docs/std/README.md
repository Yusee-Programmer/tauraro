# Tauraro Standard Library

Complete reference for the Tauraro standard library (`std`).

See also: [Language Documentation](../lang/README.md) ·
[Developer & Contributor Documentation](../dev/README.md) (compiler internals,
building libraries with `taupkg`)

## Modules

| Module | Description |
|---|---|
| [`std.async`](async.md) | Concurrency: channels, tasks, mutexes, semaphores, barriers, StructuredGroup, IOPoll, EventLoop |
| [`std.collections`](collections.md) | Data structures: Stack, Queue, Deque, Set (with algebra), Counter, Pair/Triple, MinHeap/MaxHeap, LinkedList, Graph |
| [`std.compress`](compress.md) | Compression: zlib compress/decompress, raw deflate/inflate (`-lz` required) |
| [`std.crypto`](crypto.md) | Cryptography: SHA-256, HMAC-SHA256, MD5, UUID v4 |
| [`std.encoding`](encoding.md) | Data encoding: JSON, Base64, Hex |
| [`std.gpu`](../lang/18_gpu_and_asm.md) | OpenMP-backed parallel dispatch (`Gpu.parallel`); replaces the deprecated `gpu:` block |
| [`std.io`](io.md) | File I/O, directory operations, path manipulation, console, buffered I/O |
| [`std.iter`](iter.md) | Range construction, int/float vector transforms, folds, prefix sums, normalization |
| [`std.math`](math.md) | Integer math, floating-point math, bitwise operations, statistics, random |
| [`std.net`](net.md) | TCP, UDP, DNS, URL, HTTP client (7 verbs), HTTPS client (OpenSSL), HTTP server + router |
| [`std.regex`](regex.md) | POSIX extended regex: match, find, replace, split, count |
| [`std.string`](string.md) | String utilities (Str), formatting (Fmt), parsing, line/word splitting, `split_to_vec` |
| [`std.sys`](sys.md) | Environment variables, file system, process control, timing, OS info, platform detection, graceful-shutdown signal handling |
| [`std.test`](test.md) | Lightweight unit-testing framework |
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
