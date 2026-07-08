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
- **No strong-ownership cycles `[S-2]`.** A cycle of owning class references would
  leak (its refcounts never reach zero); `--strict` rejects it (break it with a
  `Weak[T]`/`Pointer[T]` non-owning edge). With `[U-1]` this upgrades INV-2 to a
  *total* leak-freedom guarantee: a `--strict` program that compiles is provably
  leak-free (all heap is ARC-managed, the ownership graph is acyclic).
- **Manual allocation is explicit `[U-1]`.** `alloc`/`dealloc`/`alloc_array` are
  only allowed inside `unsafe:`.

Note that the raw-pointer gates are **not** `--strict`-only — they are on by
default (see §4): `[P-2]` (raw deref `.read()` / arithmetic `.offset()`) and `[P-1]`
(raw `.write()`) are errors outside `unsafe:` in *every* build.

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
Safe code cannot reach these operations without the `unsafe:` keyword:

- `[P-2]` — raw pointer **deref** (`.read()`) and **arithmetic** (`.offset()`) —
  **on by default**, no `--strict` needed.
- `[P-1]` — raw pointer **write** (`.write()`) — on by default.
- `[U-1]` — manual **allocation** (`alloc`/`dealloc`) — under `--strict`.

Because `[P-2]`/`[P-1]` are default-on, an *ordinary* program (no flags) already has
no way to reach undefined behavior except through an explicit `unsafe:` block — the
same containment contract as Rust, with zero lifetime annotations. Unsafe blocks are
the *only* place the floor's guarantees are delegated to the author. The compiler's
own source and the standard library — the audited code that *implements* the safe
abstractions — opt out of `[P-2]` with the `# @trusted` module pragma; user code
never needs it.

---

## 5. Concurrency safety (current scope — see §7)

- Passing a value to another thread (`spawn`, `task_group`, `Thread.spawn`,
  `ThreadPool.spawn`) requires it to be `Sendable` ([06 — Sendable](06_sendable.md));
  the compiler rejects non-Sendable values crossing a thread boundary (`[T-1]`).
- A `Sendable` class is *checked*, not trusted: every field must itself be
  Sendable (`[T-2]`), a raw `Pointer` field needs the explicit `UnsafeSendable`
  opt-in, and a mutable primitive field is warned as a race risk (`[T-3]`). The
  check is **transitive through unsynchronized wrappers**: a `Shared[T]`/`Weak[T]`/
  `Chan[T]` field (or spawn argument) requires its inner `T` to be Sendable too,
  so non-thread-safe data cannot be reached on another thread *through* a handle.
  (`Mutex[T]`/`RwLock[T]`/`Atomic[T]` serialize access, so their inner type is
  protected.)
- A **borrow** (`ref`/`mut ref`) may **not** cross a thread boundary (`[T-6]`):
  `Thread.spawn` is not scoped, so a borrowed value could be mutated or freed by
  another thread, or outlive its source — the same reason Rust's `thread::spawn`
  requires `'static`. Pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]`.
- A **plain reference-counted class may not cross a thread boundary** (`[T-7]`).
  Every heap class is ARC-managed; a *plain* class uses a **non-atomic** refcount,
  so it is `!Send` exactly like Rust's `Rc` — two threads retaining/releasing it
  would race the count. This is checked **transitively**: a plain class reached
  through a `Mutex`, `Vec`, `Dict`, or another class's field is rejected too. The
  fix is `Shared[T]`, whose refcount **is** atomic (`_Atomic`) — the `Arc`
  equivalent. A framework may opt an audited class out with `UnsafeSendable`.
- The ARC floor keeps cross-thread *memory* safe (no UAF) for shared handles.

**The guarantee.** Together, `[T-1]`/`[T-2]` (only Sendable data crosses, checked
transitively), `[T-6]` (no live borrow crosses), and `[T-7]` (no non-atomic refcount
crosses, checked transitively) mean that under `--strict`, **a program that compiles
shares no non-thread-safe state and no non-atomic refcount across threads** —
everything crossing a thread is an atomic `Shared[T]` or a synchronization primitive
over Sendable data. This is the same safety *outcome* as Rust's `Send`/`Sync`,
reached with **zero lifetime annotations**. Every escape is a labeled, greppable
`UnsafeSendable` where a human took responsibility; code that compiles without one
carries the full machine-checked guarantee.

**Not yet fully proven (the honest remainder):**

1. **Detached-thread lifetimes for owned `str`/collection values.** Passing an
   *owned* `str`/collection to a *detached* `Thread.spawn` whose argument's source
   scope ends first can still dangle (`str`/collections are not classes, so `[T-7]`
   does not cover them). The structured forms — `task_group`, `await`, and `join`
   before scope exit — guarantee the caller outlives the thread; **prefer those**,
   or wrap the value in `Shared[T]`. (`[T-6]` already proves the *borrow* case; a
   `'static`-style bound on detached `Thread.spawn` for owned non-class values is
   future work.)
2. **Framework `UnsafeSendable` cores are trusted, not proven.** Like Rust's
   `unsafe impl Send/Sync`, an `UnsafeSendable` assertion is only as sound as its
   author's audit. The guarantee above is about code that compiles *without* it.

> `[T-7]` supersedes the earlier "deeply-nested aliasing through a `Mutex`" gap: an
> aliased plain refcounted value placed inside a `Mutex` (or `Vec`, or a field) and
> sent to another thread is now **caught** by the transitive check.

These remaining items are concurrency-model design decisions (scoped threads,
atomic-string split, a Send/Sync algebra). Until they land, shared *mutable*
state across threads must be guarded explicitly (`Mutex`/`RwLock`/`Atomic`) — which
all the checks above steer you toward — and detached threads must not outlive
their arguments' sources.

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
- **Partial compile-time data-race freedom** (§5): the common shapes are caught
  (`[T-1]`/`[T-2]`-transitive/`[T-6]`), but detached-thread lifetimes for owned
  refcounted values, an `Rc`/`Arc` string split, and deeply-nested aliasing remain
  — the largest open area vs Rust.
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
