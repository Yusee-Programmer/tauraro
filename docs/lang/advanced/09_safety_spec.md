# Advanced — Tauraro Safety Specification (Normative)

> This is the **normative** statement of what Tauraro's memory- and concurrency-safety
> model guarantees, what it does **not**, and how each guarantee is **verified**. Where
> the detailed mechanics live elsewhere ([01 — Lifetimes](01_lifetimes.md),
> [08 — Zero-Copy](08_zero_copy.md), [13 — Memory & Ownership](../13_memory_and_ownership.md),
> [06 — Sendable](06_sendable.md)), this page is the contract those mechanics implement.
>
> Status: **0.x — pre-1.0.** The guarantees below are the design intent and are
> *tested* (see §6), not *formally proven*. Gaps are stated honestly in §5 and §7.

---

## 1. The two-tier model

Tauraro safety is two layers, and the distinction is the whole design:

1. **The ARC floor (always on).** Every program — with *zero* annotations — is
   memory-safe at runtime via automatic reference counting. This is the baseline
   that makes the language safe-by-default and keeps lifetimes *optional*.
2. **Opt-in proofs (`--strict`).** When you annotate ownership/borrows
   (`ref`, `mut ref`, `from`, `where … outlives`), `--strict` *proves* the
   borrow discipline at compile time and the compiler *elides* the ARC traffic it
   proved unnecessary. Without `--strict`, the same annotations are accepted and
   the ARC floor keeps the program safe regardless.

**Consequence:** Tauraro reaches a safety *guarantee* comparable to Rust's by a
different *mechanism* — a sound ARC floor plus opt-in zero-cost borrows — rather
than mandatory borrow checking. The cost of an unproven borrow is a retain/release,
not a compile error.

---

## 2. The ARC floor invariant (always guaranteed)

For **any** well-typed program, compiled with or without `--strict`:

> **INV-1 (no use-after-free / no double-free).** Every owned heap value
> (`str`, `List`/`Vec`, `Dict`/`Map`, `Set`, reference-class instance, enum/tuple
> payload) is released *exactly once per logical owner*, on *every* control-flow
> path (normal fall-through, `return`, `break`, `continue`, `raise`, `?`-propagation,
> match arms, `with`/`try` bodies). A value read through an alias is never released
> while the alias is live.

> **INV-2 (no leaks of tractable forms).** Owned locals that do not escape their
> scope are dropped at scope exit. ("Tractable forms" = the cases the auto-drop
> analysis covers; see [13 — Memory & Ownership](../13_memory_and_ownership.md).
> Deliberate process-lifetime allocations — e.g. the lazy global thread pool — are
> exempt and excluded from leak accounting.)

> **INV-3 (escape safety).** A value passed where another name may retain it
> (collection insert, field store, enum/tuple payload, closure capture, `spawn`)
> is either retained by the new owner (refcount) or excluded from the caller's
> auto-drop — never both, never neither. The escape analysis that enforces this is
> **exhaustive** over every expression form: a new expression variant cannot be
> silently missed (the bootstrap's match-exhaustiveness check, `[E-1]`, forces it
> to be handled).

INV-1..INV-3 hold for **un-annotated** code. They are what "safe by default" means.
They are *enforced mechanically* (ARC + escape analysis), not by programmer
discipline, and *tested* per §6.

---

## 3. What `--strict` proves and elides

`--strict` is **compile-time only**: it adds checks and changes *nothing* about the
generated code on its own (verified: emitted C is byte-identical with and without
`--strict`). The zero-copy elision is computed **unconditionally**; `--strict` only
decides whether an *unprovable* borrow is a hard error or a silent ARC fallback.

Under `--strict`, the compiler enforces:

- **Aliasing-XOR-mutability `[B-1]`.** A place may have many shared `ref` borrows,
  or exactly one exclusive `mut ref` borrow, never both at once.
- **Shared borrows are read-only `[B-3]`.** Writing through a `ref T` parameter is
  rejected; mutation requires `mut ref T`.
- **Region validity `[L-1]`..`[L-5]`.** A returned borrow must outlive the call:
  its region must be a declared parameter/region (`from`), and a borrow-returning
  function may not return freshly-owned data.
- **No aliased mutable arguments `[M-x]`**, move-after-use, mutate-while-borrowed.
- **Unsafe is explicit `[U-1]`/`[P-1]`.** Raw allocation and raw pointer
  dereference are only allowed inside `unsafe:`.

> **THM-ELISION (soundness of zero-copy).** If the compiler elides the
> retain/release for a borrow `b` of source `s`, then `s` provably outlives `b`'s
> last use and `s` is not mutated while `b` is live. Therefore the elided
> (zero-copy) program is **observationally equivalent** to the pure-ARC program.

THM-ELISION is the central correctness claim. It is *checked continuously* by the
differential oracle (§6): every safe program is compiled twice — with elision and
with `--no-elide` (pure ARC) — and the outputs must be identical. A divergence is,
by definition, an unsound elision.

---

## 4. Unsafe code

Inside `unsafe:`, INV-1..INV-3 are the programmer's responsibility for the raw
operations performed (manual `alloc`/`dealloc`, raw `Pointer[T]` read/write, FFI).
Safe code cannot reach these operations without the `unsafe:` keyword (`[U-1]`,
`[P-1]`). Unsafe blocks are the *only* place the floor's guarantees are delegated to
the author.

---

## 5. Concurrency safety (current scope — see §7)

- Passing a value to another thread (`spawn`, `task_group`) requires it to be
  `Sendable` ([06 — Sendable](06_sendable.md)); the compiler rejects non-Sendable
  captures crossing a thread boundary.
- The ARC floor keeps cross-thread *memory* safe (no UAF) for shared handles.

**Not yet guaranteed:** Tauraro does **not** yet provide Rust's compile-time
*data-race freedom* — i.e. it does not yet prove that a `mut ref` cannot be
observed concurrently with another access. This is the largest open gap and is
tracked as roadmap item §2 (thread-aware `[B-1]` + a verified `Sendable`). Until
then, shared mutable state across threads must be guarded explicitly
(`Mutex`/`RwLock`/`Atomic`).

---

## 6. Verification regime (how the guarantees are evidenced)

The guarantees above are not aspirational; each is tied to an executable check that
runs in CI:

| Guarantee | How it is verified |
|---|---|
| INV-1/INV-2 (no UAF/double-free/leak) | **Leak gate** (`tests/leak/`, `TAURARO_MEMCOUNT` net ≤ 0 over a loop) + **ASan/UBSan** on the accept corpus (Linux CI) |
| INV-3 (escape exhaustiveness) | Match-exhaustiveness `[E-1]` on the escape walkers (compile-time, self-guarding) + escape regression tests |
| `[B-*]`/`[L-*]`/`[M-*]`/`[U-1]`/`[P-1]` actually fire | **Reject corpus** (`tests/soundness/reject/`): each program *must* fail under `--strict` with its declared `# EXPECT: [CODE]` |
| Safe patterns keep compiling/running | **Accept corpus** (`tests/soundness/accept/`): must compile under `--strict`, build, run, exit 0 |
| THM-ELISION (elision soundness) | **Differential oracle**: every accept program built with elision **and** `--no-elide`; outputs must be identical (`elide ≡ arc`) |
| Self-consistency | gen1→gen2 self-host **fixpoint** (emitted C byte-identical) |

Run locally:

```sh
bash   scripts/run_soundness.sh     # reject + accept + differential (+ ASAN=1 on Linux/macOS)
bash   scripts/leak_check.sh        # leak gate
bash   scripts/run_tests.sh         # language + regression suite
```

A safety claim is only as strong as its corpus. When a new diagnostic or borrow
rule is added, add a `reject/` test that asserts its code; when a new safe pattern
is supported, add an `accept/` test (it is automatically differential-checked).

---

## 7. Honest gaps (0.x)

- **Not formally proven.** THM-ELISION and INV-1..INV-3 are tested, not mechanized.
  A small mechanized proof of the region/outlives core is future work.
- **No compile-time data-race freedom yet** (§5) — the biggest gap vs Rust.
- **Expressiveness < Rust** by design: no lifetime variance, HRTBs, or GATs. The
  ARC floor covers what the borrow checker cannot express, so these are not needed
  for *safety* — only for squeezing out the last refcounts.
- **Single implementation, pre-1.0.** No second conformance implementation yet.

The roadmap to closing these is: exhaustive escape walkers (**done**), the
differential oracle (**done**), this spec (**done**), thread-safety (§2), and a
mechanized core proof.

---

## See also

- [01 — Lifetimes & Borrow Checking](01_lifetimes.md) — the borrow/region mechanics.
- [08 — Zero-Copy Guide](08_zero_copy.md) — where elision actually wins.
- [06 — Sendable](06_sendable.md) — cross-thread type safety.
- [19 — Compiler Errors](../19_compiler_errors.md) — full diagnostic reference.
- [13 — Memory and Ownership](../13_memory_and_ownership.md) — the ARC floor in detail.
