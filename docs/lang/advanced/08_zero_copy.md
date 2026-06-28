# Advanced — Zero-Copy Guide

> Companion to the [Lifetimes & Borrow Checking](01_lifetimes.md) reference. This
> page is about **when zero-copy actually wins**, *how* to write it, and the
> honest performance picture — measured from the generated C, not assumed.

---

## What "zero-copy" means in Tauraro

Zero-copy = avoiding work the ARC floor would otherwise do:

1. **No data copy.** A `str` slice normally allocates a new buffer and copies
   bytes; a view borrows a pointer + length and copies nothing.
2. **No refcount traffic.** Reading a refcounted value normally retains it
   (and releases it later); a *proven* borrow elides both.

Under `--strict`, the compiler proves the borrow is safe and removes that work.
Without `--strict`, the same code runs — just with the ARC retain/release intact.

---

## The most important honest fact

**Tauraro's ARC is *already* zero-copy for most reads.** Passing a `str` / class
/ `Dict` by value passes a pointer — it does **not** copy or retain. Iterating a
list (`for x in items`) borrows each element without retaining. Indexing
(`list[i]`, `.first()`, `.last()`) returns an alias. So for those operations,
adding `ref` changes **nothing** at runtime — there was no copy to remove.

Zero-copy gives a *large* win only where ARC genuinely had to **allocate, copy,
or retain**. There are three such cases, and they're the ones worth annotating:

| Pattern | ARC does | Zero-copy does | Typical win |
|---|---|---|---|
| **Substring views** (`StrView`) | `s.slice(a,b)` allocates + copies bytes | borrows `{ptr,len}` | **~35× faster, ~15× less memory** |
| **Borrowed enum payloads** (`enum E from r`) | `Word(s.slice(...))` allocates | stores a pointer | **~20× faster, far fewer allocations** |
| **Dict value borrows** (`ref str = d.get(k)`) | retains + releases each access | pure alias | eliminates per-access refcount ops |

Everywhere else — pass-by-ref of pointers, list iteration — the win is **parity**,
because ARC was already optimal. That is a *strength*: there is nothing to fix.

---

## 1. Substring views — `StrView`

`StrView` (in `std/string/str`) holds a borrowed pointer into a source string
plus a length. Slicing and comparing copy **nothing**; only `.to_str()`
materialises an owned `str`. It is a `@value_type` class — a **stack value**
`(ptr, len)`, exactly like Rust's `&str` fat pointer: no heap allocation per
view, and a `List[StrView]` stores the views inline. In the
[Rust-vs-Tauraro benchmark](../../../benchmarks/rust_vs_tauraro/README.md) this
puts substring views at ~4 ms vs Rust's ~3 ms, at comparable memory.

```python
from std.string.str import StrView

def main():
    mut csv = "name,age,city"
    mut name = StrView.of(csv, 0, 4)      # "name" — no allocation
    mut age  = StrView.of(csv, 5, 3)      # "age"  — no allocation
    print(name.eq("name"))                # byte compare, no allocation
    mut whole = StrView.all(csv)
    print(whole.slice(0, 4).eq("name"))   # zero-copy sub-view of a view
    mut owned = age.to_str()              # ONLY here do we allocate
```

**Use it for:** parsing/tokenizing — splitting a large input into many fields
where you only inspect them, materialising the few you keep.

```python
# Tokenize without allocating a string per token
mut i = 0
while i < line.length():
    mut tok = StrView.of(line, start, count)
    if tok.eq("GET"):
        ...
    i = i + 1
```

---

## 2. Borrowed enum payloads — `enum E from r`

A variant payload written `ref str from r` stores a borrowed slice instead of an
owned copy. Building millions of tokens that point into one source buffer
allocates **nothing** for the payloads.

```python
enum Token from src:
    Word(ref str from src)
    Number(int)
    End

def lex(s: ref str from input) -> Token from input:
    ...   # Token.Word(slice_of_s) borrows; no string allocated
```

Compare with the owned form `enum Token: Word(str)`, where each `Word(s.slice(…))`
allocates and copies.

---

## 3. Dict value borrows — `ref str = d.get(k)`

Reading a `str` value out of a `Dict`/`Map` normally **retains and releases** it
on every access. Bind it as a borrow and the refcount traffic disappears — when
the compiler can prove the dict isn't mutated while the borrow is live:

```python
def sum_lens(d: ref Dict[str, str]) -> int:
    mut s = 0
    for k in d.keys():
        v: ref str = d.get(k)     # zero-copy borrow — no retain, no release
        s = s + v.len()
    return s
```

**The safety net:** if the dict *is* mutated while `v` is live, the compiler
keeps the safe ARC retain (no use-after-free), and `--strict` rejects it with
`[B-2]`. So the borrow is either zero-cost or a hard error — never unsound.

---

## When NOT to bother

These are already zero-copy — annotating adds the `--strict` guarantee but no
speed:

```python
def f(s: str): ...              # passes a pointer; ref str is identical at runtime
def f(r: Record): ...           # passes a pointer
for x in items: ...             # borrows each element already
mut v = items[i]                # alias already
```

If a benchmark shows these at "parity", that's correct — the ARC baseline had no
copy to remove.

---

## Best practices

- **Profile first.** Annotate the hot path, not everything. The big wins are
  copy/allocation-heavy code (parsing, slicing, building borrowed structures).
- **Prefer `StrView` for text processing.** It's the single biggest lever.
- **Use borrowed enum payloads for ASTs/tokens** that point into a stable source
  buffer you keep alive.
- **Borrow dict/map values in read loops** with `ref str = d.get(k)`, and don't
  mutate the map inside the loop.
- **Keep the source alive.** A view/borrow is only valid while its source lives.
  `--strict` enforces this; without it, ARC keeps the source alive via the retain
  it didn't elide.
- **Don't expect wins where ARC already aliases.** Pointer pass-by-ref is parity
  by design.

---

## Reproduce the numbers

The repository ships a benchmark suite that builds an ARC variant and a
zero-copy variant of each case and reports time + peak memory + live allocations:

```sh
powershell -File benchmarks/zerocopy/run_zerocopy.ps1   # Windows
bash        benchmarks/zerocopy/run_zerocopy.sh          # Linux/macOS
```

and a Rust-vs-Tauraro comparison for the zero-copy hot paths:

```sh
bash benchmarks/rust_vs_tauraro/run.sh
```

See [`benchmarks/zerocopy/README.md`](../../../benchmarks/zerocopy/README.md).

---

## See also

- [Lifetimes & Borrow Checking](01_lifetimes.md) — the syntax and rules.
- [Memory and Ownership](../13_memory_and_ownership.md) — the ARC floor.
