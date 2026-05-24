# Tauraro Standard Library

Complete reference for the Tauraro standard library (`std`).

## Modules

| Module | Description |
|---|---|
| [`std.async`](async.md) | Concurrency primitives: channels, tasks, mutexes, semaphores, barriers |
| [`std.collections`](collections.md) | Data structures: stack, queue, deque, set, counter, tuple |
| [`std.io`](io.md) | File I/O, directory operations, path manipulation, console |
| [`std.iter`](iter.md) | Range construction and vector transformations |
| [`std.math`](math.md) | Integer math, floating-point math, bitwise operations |
| [`std.string`](string.md) | String utilities and formatting |
| [`std.sys`](sys.md) | Environment variables, file system, process control, timing |
| [`std.net`](net.md) | URL building, parsing, and encoding |

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
