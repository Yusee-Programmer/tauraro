# Zero-copy vs ARC benchmarks

Measures the runtime payoff of Tauraro's optional **borrow / lifetime / view**
features against idiomatic **ARC** (automatic reference counting) code, across
every supported type: `str`, `List`, `Dict`, classes, enums, and interfaces.

## What "Zero-copy" means here

A key, possibly surprising fact: **`--strict` does not change codegen.** It only
*enforces* the borrow rules at compile time. The runtime difference comes entirely
from the **source** — using `ref` borrows, `StrView`, and borrowed enum/interface
payloads — which lets the compiler skip copying and/or refcount traffic. We
compile the zero-copy variants with `--strict` anyway, so the benchmark doubles
as proof that the zero-copy code is borrow-checker-clean.

Each case has two source files:

| | ARC variant (`*_arc.tr`) | Zero-copy variant (`*_zc.tr`) |
|---|---|---|
| **str_view** | `s.slice(a,b)` — allocates a new string | `StrView.of(s,a,b)` — borrowed `{ptr,len}`, no copy |
| **str_pass** | `def f(s: str)` | `def f(s: ref str)` |
| **list_iter** | `for x in items` | `for ref x in items` |
| **class_pass** | `def f(r: Record)` | `def f(r: ref Record)` |
| **enum_payload** | `Word(str)` — owned copy | `enum E from src: Word(ref str from src)` — borrowed |
| **dict_pass** | `def f(d: Dict[..])` | `def f(d: ref Dict[..])` |
| **interface** | `def label(self) -> str` | `interface N from r: def label(self) -> ref str from r` |

## Running

```sh
# Windows
powershell -File benchmarks/zerocopy/run_zerocopy.ps1
# Linux / macOS
bash benchmarks/zerocopy/run_zerocopy.sh
```

Each variant is built twice from the same emitted C: an `-O2` binary (for
**time** + **peak RSS**) and an `-O2 -DTAURARO_MEMCOUNT` binary (for **live heap
allocations** and **live string objects** held at peak). Results are written to
`results.md`.

## How to read it

- **Time** — wall-clock of the self-timed workload (lower = faster).
- **Peak (KB)** — peak resident memory.
- **Live strs** — net live `TrStr` objects. A `StrView` holds a *borrowed*
  pointer and creates **no** `TrStr`, so this column most directly exposes
  copy-avoidance.

## Headline findings

Two patterns emerge, and they tell the whole story of when zero-copy pays off:

1. **Copy-avoiding zero-copy is a huge win.** Where ARC allocates/copies and the
   borrow form does not — `StrView` slicing and **borrowed enum payloads** — the
   zero-copy variant is **~20-35x faster** and uses **far** less memory
   (`str_view`: ~15x less peak RSS, ~33,000x fewer live strings).

2. **Borrowing already-heap types is parity.** Passing a `str` / class / `Dict`
   by `ref` vs by value is ~1x: the data is a pointer either way, and Tauraro's
   ARC already elides redundant retains for read-only locals, so there is little
   left to save. This is the honest, expected result — use `ref` here for the
   *guarantee* (and the borrow checker), not for speed.

> Bottom line: **zero-copy wins when it removes an allocation/copy (views,
> borrowed payloads), not merely a pointer pass.** And it's free to adopt —
> `--strict` enforces it at zero runtime cost.

## Collection-value borrows (`ref T = d.get(k)`)

Reading a `str` value out of a `Dict`/`Map` (`v = d.get(k)`) normally **retains
and releases** the value on every access. Binding it as a borrow —
`v: ref str = d.get(k)` — now **elides** that retain/release entirely, when the
compiler can prove the collection is not mutated while the borrow is live:

```
def sum_lens(d: ref Dict[str, str]) -> int:
    mut s = 0
    for k in d.keys():
        v: ref str = d.get(k)     # zero-copy borrow — no retain, no release
        s = s + v.len()
    return s
```

This is the **universal zero-copy guarantee** under `--strict`: when you express
ownership/borrowing explicitly and the borrow is provably safe, the refcount
traffic disappears. If the dict *is* mutated while the borrow is live, the
compiler keeps the safe ARC retain (no use-after-free) — and `--strict` rejects
it outright with `[B-2]`. So the borrow is either zero-cost or a hard error,
never unsound. (`list[i]` element borrows are a planned follow-up; `for ref x`
over a list already borrows elements with no retain.)
