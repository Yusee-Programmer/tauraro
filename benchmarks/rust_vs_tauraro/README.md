# Rust vs Tauraro — zero-copy hot paths

A head-to-head on the two cases where **both** languages do genuine zero-copy, so
the comparison is apples-to-apples (no language is allocating where the other
isn't, by design):

| Case | Rust | Tauraro |
|---|---|---|
| `str_view` | `&src[a..b]` — borrowed `&str` slice | `StrView.of(src, a, b)` — borrowed view |
| `enum_payload` | `enum Token<'a> { Word(&'a str) }` | `enum Token from src: Word(ref str from src)` |

Tauraro is built with `--strict`, so its borrows are **compile-time proven**, not
just convention. Each program self-times its workload (`TIME_MS:`) and the runner
also samples peak working-set memory.

## Running

```sh
powershell -File benchmarks/rust_vs_tauraro/run.ps1   # Windows
bash        benchmarks/rust_vs_tauraro/run.sh          # Linux/macOS
```

Requires `rustc` (build: `rustc -O`) and `gcc` (Tauraro emits C). Results are
written to `results.md`.

## Representative results (Windows, gcc -O2 / rustc -O)

| Case | Rust time | Rust mem | Tauraro time | Tauraro mem |
|------|----------:|---------:|-------------:|------------:|
| enum_payload | 9 ms | ~11 MB | **9 ms** | ~10.8 MB |
| str_view | 3 ms | ~6.1 MB | 19 ms | ~7.5 MB |

(Numbers vary by machine; run it yourself. Peak memory is the OS working set,
which includes runtime/DLL overhead and is roughly flat across both.)

## Honest interpretation

- **Borrowed enum payloads: Tauraro matches Rust.** Building 200k tokens that
  borrow a shared source allocates nothing for the payloads in either language;
  both store a pointer in a value-type tagged union. Tauraro is dead even on time
  and slightly *lower* on memory. This is the headline: Tauraro's borrowed-payload
  zero-copy is genuinely Rust-competitive.

- **Substring views: Rust wins (~6×), and we know exactly why.** Rust's `&str` is
  a *value* — a fat pointer `(ptr, len)` that lives in a register/stack slot, so a
  slice is literally free. Tauraro's `StrView` is currently a **heap-allocated
  class**, so the loop allocates 100k small view structs (the *string data* is
  still zero-copy — that's the 15× memory win over the ARC `s.slice()` baseline in
  the [zerocopy](../zerocopy/README.md) suite — but the *struct* allocations cost
  time). A **value-type `StrView`** (stack-allocated, no heap struct) would close
  most of this gap; it's a known, planned optimization (`is_class=false` /
  `@value_type` views are not yet wired).

## Takeaway

Tauraro's zero-copy is **real and, for borrowed payloads, on par with Rust** —
with Python-like syntax and an ARC safety floor, rather than mandatory lifetimes
everywhere. The remaining gap (heap-allocated views) is an implementation detail
with a known fix, not a fundamental design limit. The fair summary: Tauraro is
*close to* Rust on zero-copy where its representation is already a value, and has
a clear path to closing the rest.
