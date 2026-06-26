# Tauraro v0.0.7 — Release Notes

*Release date: 2026-06-26*

This release is the **memory-safety + developer-experience** milestone. Over the
past week Tauraro gained a full optional Rust-style lifetime and borrow-checking
system, a zero-copy story, and a completely rebuilt diagnostics experience —
while staying true to the core promise: **Python syntax, Rust-level performance,
and the compiler carries the complexity. Everything new is optional; without
`--strict` the compiler always falls back to the ARC safety floor.**

The compiler self-hosts (gen2 == gen3 byte-identical), passes the full example
suite (33/33), and the memory leak-gate, on every commit.

---

## 1. Optional lifetimes & named regions (Rust-parity, fully optional)

Lifetimes in Tauraro are **named regions** — you name a parameter the borrow
comes from, instead of inventing `'a` tokens. All of it is **erased to plain
types for codegen**; it only constrains under `--strict`.

- **Borrow types:** `ref T` (shared borrow) and `mut ref T` (exclusive borrow),
  usable on parameters, fields, and returns. Erased to `T`; usable exactly like a `T`.
- **Region sources:** `-> ref T from a`, multi-source `from a, b`, and
  `where a outlives b` outlives bounds. The `where` clause may sit inline, break
  to a continuation line, or use a multi-line parenthesised form with a trailing comma.
- **Lifetimes on *every* declaration form** (not just functions):
  - `class C from r:` with `ref`-typed fields and methods.
  - `enum Token from src:` with borrowed variant payloads `Word(ref str from src)`.
  - `interface View from r:` with `-> ref str from r` method signatures.
  - `extend T from r:` impl-block region re-declaration.
- **Validation (`--strict`):** `[L-2]` rejects region names that aren't
  parameters/region-params; `[L-4]` is relaxed by a satisfied `where … outlives …`.

## 2. Borrow checker — aliasing-XOR-mutability (NLL-precise, sound)

Enforced only under `--strict`; ARC keeps every program safe without it.

- **`[B-1]` borrow-vs-borrow:** a place may have many shared `ref` borrows **or**
  exactly one exclusive `mut` borrow — never both overlapping.
- **`[B-2]` place-vs-borrow:** a borrowed place can't be reassigned/moved while a
  borrow is live (read-while-exclusive and mutating-method-call-while-borrowed
  included). Works **cross-block** (liveness- and path-aware, no false positives
  on divergent branches) and **cross-function**.
- **`[B-3]` borrow mutability:** a shared `ref T` parameter is read-only;
  mutating it requires `mut ref T` (Rust's `&T` vs `&mut T`).
- **Method-mutability inference:** a method counts as mutating only if it stores
  into `self.<field>`, so getters remain callable on a shared borrow — zero false
  positives.

## 3. Zero-copy views

- **`StrView`** (stdlib): a borrowed pointer + length. Slicing and comparison
  allocate **nothing**; only `.to_str()` materialises. A 300k-iteration
  view+compare loop allocates zero string data.
- **`for ref x in items`** — iterate by reference; mutating the collection
  mid-loop is rejected under `--strict`.
- **Borrow-holding structs** — a `T from p` return records a borrow edge, so a
  view that outlives its source is caught (`[B-2]`).

## 4. Diagnostics — beautiful, located, colorized (cross-platform)

Errors are now caught at the Tauraro level and rendered rustc-style instead of
leaking raw C compiler errors:

```
error: [N-3] name 'aaaa' is not defined.
  --> app.tr:2:11
   2 |     print(aaaa)
     |           ^^^^
  = help: check the spelling, declare it, or import it before use.
```

- Red `error` / yellow `warning`, cyan locations, dim gutters, green `= help`,
  with a caret underlining the exact span.
- **Cross-platform color:** Linux/macOS via `isatty`; Windows enables VT
  processing automatically. Piped/redirected output stays plain ASCII. Honors
  `NO_COLOR`, and `CLICOLOR_FORCE=1` forces color on (e.g. MSYS/Git-Bash).
- Applies across **lexer, parser, and semantic** diagnostics.

## 5. Parser & semantic robustness — stop errors before C

- **`[N-3]` undefined names** are caught (e.g. a misspelled variable), instead of
  reaching GCC. Zero false positives across the whole compiler + stdlib + examples.
- **Unclosed brackets** `(` `[` `{` are now errors, located at the **opening**
  bracket (not end-of-file), one error per unclosed delimiter — covers calls,
  lists, tuples, dicts/sets, comprehensions, and indexing.
- **Inconsistent indentation** (misaligned blocks / mixed tabs & spaces) and
  stray top-level indentation are detected with a precise caret.
- **`main()` needs no explicit `return`** — the entry point is exempt from the
  definite-return check, whether written `def main():` or `def main() -> int:`.

## 6. Memory-safety & runtime fixes

- Sound **universal auto-free**: data-class locals are auto-dropped only when
  provably owned and non-escaping — fixing a class of latent use-after-free bugs
  (f-string format specs, borrowed collection elements) while preserving leak fixes.
- Fixed a nested-`extend` codegen crash, several runtime ownership-lie leaks
  (`readdir` EOF, readline buffers, `str_join`, `strftime`, DNS/TCP helpers), and
  `break`/`continue` MIR lowering that previously disabled precise drops.
- **watax** web-framework hot-path allocations trimmed (response build and
  request parse now avoid throwaway slices and per-response heap churn).

---

## Compatibility & notes

- Pre-1.0: `0.x` carries **no stability guarantee** yet; APIs and syntax may change.
- All lifetime/borrow features are **opt-in** under `--strict`. A normal build
  ignores `ref` / `mut ref` / `from` / `where` and relies on ARC.
- All English keywords have Hausa equivalents (`aiki`=`def`, `aji`=`class`, …).

🤖 Generated with [Claude Code](https://claude.com/claude-code)
