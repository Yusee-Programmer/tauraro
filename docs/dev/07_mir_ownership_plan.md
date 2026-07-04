# MIR-based ownership analysis — design & staging

## Why

The compiler's memory management is ARC + a **web of interacting heuristics** that
decide, per local, whether it is auto-dropped: `is_droppable_sym`, `coll_escaped`,
`str_escaped`, `container_borrows`, `borrows_region` (`@borrowed`), and the escape
walkers (`mark_escaped_coll_args` / `mark_escaped_str_args`) + the Phase-C borrow
check at the `SLet`. These are **conservative in the safe direction** — when unsure,
they *don't* drop (a leak, never a UAF) — which is why the safety guarantee holds.
But conservatism leaves **precision holes**, and the special-casing has had **soundness
holes** too. Both session-blocking bugs and all four fuzzer findings live here:

| | Kind | Root |
|---|---|---|
| class-with-`free` double-free (fixed) | **UAF/double-free** | `is_droppable_sym` free-branch ignored `coll_escaped` |
| `Mutex.get()` UAF (fixed) | **UAF** | Phase-C borrow check didn't cover `Mutex`/`RwLock` |
| F-1 `Vec[T].get()` container leak | leak | element borrow marks container non-droppable, never re-dropped |
| F-2 borrowed `free`-class leak | leak | `coll_escaped` set for *any* class arg, even a pure borrow |
| F-3 `Mutex[T]` payload leak | leak | mutex drop releases the lock, not the guarded value |
| F-4 nested-generic type loss | compile error | container-ctor return type flattens nested args |

Patching each case grows the web and risks the next soundness hole. The durable fix
is to **compute drops from a principled ownership analysis** and let the heuristics
retire. The fuzz oracle (`scripts/fuzz_check.sh`) is the safety net that makes this
change tractable: it catches any regression (differential elision, net-leak, ASan)
the moment it appears.

## The model

Lower each function body to a small **MIR**: a linear list of statements over
*places* (locals, `self`, params, fields, elements) with explicit **moves**,
**borrows** (with a lexical region), and **calls** annotated with per-argument
ownership effect. Over that, run two standard analyses:

1. **Liveness / last-use** — the last program point a place (or a borrow of it) is
   read. A drop is inserted at the *end of the owning scope*, after last use, on
   **every** control-flow path (the SMatch/SIf/SWhile/… cases `is_droppable_sym`
   already special-cases become one uniform rule).
2. **Ownership transfer** — each value has exactly one live owner at a time. A move
   (into a field/collection/return/consuming-call-arg) transfers ownership; the
   source is no longer dropped. A borrow does **not** transfer; the owner still
   drops after the borrow's region ends (this is F-1: the container is dropped once
   the element borrow's region closes).

Drops become **explicit MIR statements** the codegen emits verbatim — no
`is_droppable_sym` guessing at emit time.

## Interprocedural effect summary (the keystone — fixes F-2)

The precision the heuristics lack is *per-callee argument effect*. Compute, as a
whole-program monotone fixpoint (modeled exactly on the existing
`compute_return_ownership`, which already does this for return values):

```
consumes(fn, i)  = the fn moves/frees parameter i  (free(), store into
                   field/collection/enum/tuple/closure, return it, or pass it to
                   some g at position j where consumes(g, j))
consumes_self(C.m) = same, for the receiver
```

Direction & soundness: start **optimistic** (`consumes = false`, i.e. "borrowed")
and flip to `true` on the first consuming use — so a fixed-point `false` is a
**proof** the parameter is only borrowed. Anything uncertain (call to an unknown /
extern / function-pointer target, or any of the consuming uses) stays `true` =
consumed = the current conservative behavior. **Missing a borrow → a leak (safe);
never a UAF.** Then, at a call site, `coll_escaped` / move-suppression is applied to
argument `i` **iff** `consumes(callee, i)`. `use_named(x)` (reads `x`, returns an
int) → `consumes=false` → `x` is dropped by the caller → **F-2 leak gone**;
`c.send_json_writer(200, w)` → `consumes_self`/`consumes=true` → still suppressed →
no double-free.

## Staging (each stage: fixpoint gen2≡gen3 + full oracle before blessing)

1. **Effect summary as a pure ANALYSIS** — compute `consumes`/`consumes_self`,
   assert nothing changes in codegen (byte-identical C). Lands the infrastructure
   with zero risk.
2. **Apply to F-2** — gate `mark_coll_arg`'s class-instance escape on
   `consumes(callee, i)`. Validate with the fuzzer's `f_owned_use`/`f_consume`
   (borrow-vs-consume) + watax (`send_json_writer`) + full suite/soundness/ASan.
3. **F-1** — model `x = v.get(i)` as a borrow with a region; drop `v` after the
   region. Validate `f_vec_box`.
4. **F-3** — a `Mutex[T]`/`RwLock[T]` local created from a *fresh, unaliased* `T`
   (constructor arg, not a borrow) owns its payload → release it on the mutex drop;
   an aliased/borrowed payload (e.g. watax's `Mutex[App].init(self)`) does **not**.
   The ownership analysis already distinguishes these. Validate `f_mutex_get` +
   watax (must not regress — watax stores borrowed handles in mutexes).
5. **Retire the heuristics** — once drops come from MIR, delete the now-dead
   `coll_escaped`/`container_borrows`/`borrows_region` special-cases incrementally,
   each deletion guarded by the oracle producing identical output.

Once complete, promote the `HARD` fuzz fragments to `CORE` (they should be leak-free)
and the fuzzer runs the full ownership surface as the standing regression gate.

## Implementation constraint — the pass-ordering blocker (pinned)

A deep spike established the exact reason F-2 can't be a drop-in patch, so the next
(reviewed) session starts here rather than re-discovering it:

- `coll_escaped` is set by `mark_coll_arg` **during** the main lowering pass, at each
  call site (`sema.tr` ~1782).
- The drop decision (`compute_scope_drops` → `is_droppable_sym`, which reads
  `coll_escaped`) also runs **during** the main pass, at scope-exit — it "must run
  while the scope is live" (`sema.tr` ~2101), because it reads live `Symbol` state.
- The interprocedural `consumes(fn,i)` fixpoint needs **types** (to resolve
  `param.m()`'s receiver class for `consumes_self`), which only exist **after**
  lowering. And it needs the whole program's lowered bodies (like
  `compute_return_ownership`, which is why that one is a post-pass at ~2383).

So the analysis is only computable *after* lowering, but its result is needed
*during* lowering (before `compute_scope_drops` fires). The two orderings conflict.
Neither an AST pre-pass (no types) nor a HIR post-pass (too late) works.

**The fix (the restructure):** split `analyze()` into phases —
(1) lower all function/method/enum bodies to HIR and register signatures;
(2) run `compute_return_ownership` **and** the new `compute_param_ownership` fixpoint
    over the lowered program;
(3) run the escape-marking + `compute_scope_drops` pass, now consulting the
    precomputed `consumes`/`consumes_self`.
Today (1) and (3) are interleaved in one pass; separating them is the core change.
It is byte-identical-preserving if step (2)'s results are not yet consumed (Stage 1),
then `mark_coll_arg` gates on them (Stage 2). Validate each with fixpoint + the fuzz
oracle. This is a real pass-architecture change — do it under review, not unattended.

## Non-goals

Not a borrow *checker* rewrite — `--strict`'s `[B-*]`/`[L-*]`/`[S-2]` region rules
stay as-is. This is only the **drop/ownership** side (what the ARC floor guarantees
for *all* code), making it precise and principled instead of heuristic.
