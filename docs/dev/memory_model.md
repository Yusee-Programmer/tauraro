# Tauraro Memory Model — Hybrid ARC + Compile-Time Ownership (design spec)

Status: **design committed; implementation in progress (multi-session).** This is
the canonical reference the implementation is built against. See
`project_safety_overhaul_phase0` in the maintainer notes for live status.

## Goal

Achieve Rust's *guarantees* (no use-after-free, no double-free, no dangling
pointers, no use-after-move, no data races, no OOB/null/uninit in safe code)
**without Rust's visible complexity**, behind Python-style syntax. Leaks are
reduced to the cycle case (memory-safe, breakable with `Weak[T]`); no ownership
system — Rust included — prevents those 100%.

## ⚠️ Reality check: there is no MIR (discovered 2026-06)

`--emit mir` is a **stub** — it prints `"[MIR] N functions lowered"` from
`hir.functions.len` and stops (`main.tr:867`). There are **no `BasicBlock` /
`Place` / `Terminator` types, no CFG, no HIR→MIR lowering.** The real pipeline is
**lex → parse → sema(HIR) → C codegen.** All ownership/auto-drop work today
happens on the HIR + scope stack.

Implication for this plan:
- **ARC (the floor, below) is HIR-based and needs no MIR** — it is the tractable
  route and where work should go.
- The **"borrow checker on a MIR"** route is far larger than first assumed: it
  requires **building the entire MIR infrastructure from scratch first** (CFG,
  basic blocks, places, terminators, HIR→MIR lowering) *before* a single
  liveness/borrow pass can run. That is a multi-month subsystem on top of the
  checker itself. Treat it as a separate, much later bet — not the near-term path.
- A weaker "ownership optimizer" (drop elision) can run on the HIR like today's
  auto-drop, but the HIR's lack of a real CFG is exactly why the scope-heuristic
  drop keeps mishandling conditional moves/returns. A proper optimizer wants the
  MIR; until then, keep ARC as the floor and elide conservatively.

## The model: ARC is the floor, ownership is the optimizer

Three layers with a strict division of labor (the lack of which caused the
historical leak/UAF churn — an *unprincipled* mix of refcount + scope-heuristic
auto-drop + escape analysis):

1. **ARC = correctness floor.** Every managed value is reference counted by
   default: **retain on every copy, release at every scope exit — uniformly, with
   NO escape analysis and NO manual frees.** Sound by the refcount invariant for
   any aliasing pattern. (`str` already does uniform retain-on-copy.)
2. **Compile-time ownership = optimization + guarantee layer.** A move/liveness +
   inferred-borrow analysis on the **MIR** *elides* refcounting for values it can
   prove are uniquely owned (zero-cost moves + static drops) and proves
   aliasing-xor-mutation for data-race freedom on those values.
3. **Optional annotations = advanced opt-in** (never required): `from` lifetimes
   (already supported — see `docs/lang/advanced/01_lifetimes.md`), optional
   `mut`/`&mut` parameter intent, and explicit `Shared[T]`/`Weak[T]` for
   deliberate shared ownership / cycle breaking.

### The decisive invariant (why this is sound AND buildable solo)

> **The analysis may only PROMOTE a value from refcounted to zero-cost-owned when
> it can prove unique ownership. It must NEVER demote a safe refcount into an
> unsafe free. "When in doubt, refcount."**

Consequences:
- **Incomplete analysis is still sound** — it just leaves more values refcounted
  (slower), never unsafe. This is the opposite of the failed scope-heuristic
  drop work, where an incomplete escape analysis produced *unsafe frees* (UAF).
- The borrow checker is **not a gatekeeper that errors** — when it can't prove
  ownership it keeps the refcount, so safe code **always compiles with zero
  ceremony**. (Contrast Rust: can't prove ⇒ compile error ⇒ programmer
  restructures = the complexity.) Advanced devs opt into annotations to *force*
  zero-cost and get an error only then.
- Therefore the implementation can start from "**refcount everything**" (trivially
  safe) and add elisions incrementally, each verified by the oracle below — an
  always-safe road to zero-cost.

## Guarantees

| Property | Mechanism | Status |
|---|---|---|
| No use-after-free / double-free | ARC floor (always) | invariant-sound |
| No use-after-move | ownership analysis (on elided values) | layer 2 |
| Data-race freedom | inferred borrow check + Send/Sync (on owned values); atomic rc + Send on shared | layer 2 + concurrency |
| No null / OOB / uninit | `Option`/`Result`, bounds checks, definite-init (make fatal) | mostly present |
| No leaks | best-effort: ARC frees at rc 0; **cycles leak** → `Weak[T]` | inherent limit |

## Build sequence (each phase: shippable + self-host-verified)

**Oracle for every step:** the leak gate (`scripts/leak_check.sh`, widen its
workload as coverage grows) MUST stay green, gen2 (the compiler built by the
patched compiler) MUST actually run, and the gen2==gen3 fixpoint MUST hold.
Never bless a compiler that fails any of these.

- **Baseline (in place):** explicit `UnsafeSendable` boundary; leak gate.
- **Uniform ARC (the correctness floor):**
  make release unconditional at every scope/block exit (incl. if-bodies — this
  is the residual #53 leak), keep retain-on-copy uniform, and **delete the
  manual `str` frees** (~150 `.tr` sites, concentrated in net/*) that currently
  fight ARC and cause double-frees when drops are added. After this, all
  unsafe-memory bug classes are gone *by construction*; perf is "everything
  refcounted."
- **Ownership elision (MIR):** move + liveness + drop elaboration that
  removes refcounts for provably-owned values (locals, non-escaping temporaries).
  Pure optimization; getting it wrong costs perf, not safety.
- **Inferred borrow check:** aliasing-xor-mutation, region inference,
  no annotations; yields data-race freedom on owned values.
- **Advanced opt-in surface:** `from` (have it), optional `mut`/`&mut`,
  force-zero-cost errors; `--explain` lint to show refcounted-vs-owned per value.

## Why uniform ARC first, not the borrow checker

The borrow checker is the long pole (research-grade). Uniform ARC
eliminates the bug classes the project actually hits, *now*, and is sound by the
refcount invariant — no escape analysis required. Four prior attempts failed
because they added *drops* (releases) gated by an incomplete escape analysis
while manual frees were still present → double-free → corrupted gen2. Uniform ARC
removes both the gating and the manual frees, so correctness no longer depends on
proving anything. The hard ownership analysis (elision + borrow check) then layers on as
optimization, never as a correctness prerequisite.

## Current state (audit)

- `str` is already uniform ARC: `b = a` emits `_tr_str_retain(a)`; both released
  at scope exit. The retain half of #53 is effectively done.
- Residual leak: `str` declared inside `if`-bodies is not released (top-level and
  `while`-bodies are). Fixing it via scope-heuristic drops corrupts gen2 — must
  be done as the uniform-ARC change (unconditional release + remove manual frees), not a
  drop patch.
- ~234 manual `_tr_c_free`/`_tr_free` sites (~150 in `.tr`, rest vendored
  headers) + ~90 `.free()`/`.dispose()` calls — the uniform-ARC reconciliation
  surface, concentrated in `std/net/*`.

## First implementation slice

Smallest sound slice: in **one leaf module** (e.g. `std/string/str.tr`,
7 manual frees), delete the manual `str` frees, make release unconditional for
that module's `str` locals, and prove via the oracle (leak gate workload over
those functions = 0 growth, gen2 runs, fixpoint holds). Then expand module by
module up to `std/net/*`. Each module is an independently verifiable, revertible
increment — no big-bang change to the self-hosting compiler.
