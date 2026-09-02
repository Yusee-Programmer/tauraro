# Fuzz oracle — findings

The ownership fuzzer (`tests/fuzz/gen.py` + `scripts/fuzz_check.sh`) surfaced these
bugs/imprecisions on its **first runs**. They are why the generator has a `CORE`
tier (leak-free, sound — the green regression gate) and a `HARD` tier (`FUZZ_HARD=1`
— the patterns below, kept out of the gate so they don't mask regressions). Each is
reproducible: `FUZZ_HARD=1 python3 tests/fuzz/gen.py <seed>`, or `FUZZ_ONLY=<frag>`.

The point of building the oracle was exactly this: turn "found by accident while
debugging watax" into "found by a script." All four below were found by the script.

## Fixed

### F-2 — borrowed `free`-class leak  ✅ CLOSED
`FUZZ_ONLY=f_owned_use` now reports `LIVE 0`, differential matches (no UAF), and
`f_consume` stays `LIVE 0` (no double-free). Closing it required fixing a **latent
runtime bug** first: `Dict_has` reported presence as `Dict_get(d,key)!=NULL`, so any
key stored with a `false`/NULL value (a `Dict[K,bool]`/`Map[K,bool]` holding `false`,
a set element) was invisible — which silently disabled the compiler's own
`consumes(fn,i)` summary (`fn_param_consumes`, a `Map[str,bool]`). It now walks the
bucket chain (`runtime/tauraro_rt.h`). With `consumes` finally visible:
1. **Apply the summary** — a new post-pass `apply_borrow_drops` (`src/sema.tr`,
   docs/dev/07 Stage 2) adds the auto-drop the conservative class-arg escape
   (`mark_coll_arg`) suppressed, but ONLY for a fresh owned local (constructor/owned
   factory) that `_param_consumed_in_block` PROVES is not consumed. Additive +
   strictly conservative → a miss is a leak, never a double-free.
2. **`return x.score()` is a borrow** — a pure-scalar return can't own/alias a heap
   param (`_pc_owns_return`), so `use_named` is correctly classified borrow.
3. **`.free()` consumes its receiver** — so `consume_named`'s owned arg is detected
   consumed and NOT also auto-dropped (would double-free).
Verified: soundness 18/18 + 8/8, gen2≡gen3 fixpoint, differential match. Promoted to
`CORE`. Pending the Linux ASan pass before it is considered fully sealed.

### F-3 — `Mutex[T]` local leaks its guarded content `T`  ✅ CLOSED
`FUZZ_ONLY=f_mutex_get` now reports `LIVE 0`, and the elided/pure-ARC differential
matches (no UAF). Two bugs had to be fixed together:

1. **Mutex payload ownership.** A `Mutex[T]` box that owns a FRESH, unaliased payload
   now releases it on drop. `_TrMutexBox` gained a `pdrop` slot; `Mutex.init(<fresh
   heap-class>)` emits `_tr_mutexbox_new_owned(v, _trdrop_T)` and `_tr_mutexbox_drop`
   pops/unlocks any held guard, then `_tr_obj_release`s the payload. A BORROWED
   payload (`Mutex[App].init(self)`) is detected by `_is_fresh_obj_expr` and stays a
   plain `_tr_mutexbox_new` (never owned → never double-freed — see the
   `mutex_borrowed_payload` soundness accept test).
2. **User-class-named-`Box` collision.** The fuzz names its payload class `Box`, which
   collided with the *builtin* `Box` in three drop/heap gates
   (`is_heap_class_tn`, `is_droppable_sym`, `_coll_elem_droppable`). Those lists
   excluded the name `Box`, so a *user-defined* `class Box` was wrongly treated as a
   non-droppable builtin and never released. The builtin `Box[T]` is never registered
   in `self.classes` (a `classes.contains` guard precedes each list), so removing
   `Box` from the lists only affects a genuine user class — which now drops correctly.
   (The std collection/special-drop names — `Vec`/`Map`/`Set`/`Mutex`/… — stay
   excluded: they ARE std classes, generic ones caught by the `generics>0` check.)

Verified locally: soundness corpus clean (reject 17/17, accept 8/8), gen2≡gen3
fixpoint holds, CORE fuzz gate green. Promoted to `CORE` in `gen.py`. Pending the
Linux ASan pass (the UAF net this box can't run) before it is considered fully sealed.

## Open

### F-1 — `Vec[HeapClass]` with an element `.get()` borrow leaks the container  (halved)
`FUZZ_ONLY=f_vec_box`. The `Box`-collision fix above also applies here: the `Box`
*elements* now drop, so the leak dropped from ~16 to ~8 allocs/iteration (`LIVE
7200 → 3600`). What remains is the original `container_borrows` / `coll_escaped`
interaction: reading an element with `v.get(i)` marks the **container** escaped (to
avoid freeing the borrowed element), after which the container itself is never
released. Severity: leak, not UAF. Still the highest-value remaining fix.

### F-4 — `Mutex[Map[K,V]].get()` without an annotation loses the value type
`FUZZ_ONLY=f_mutex_map`. `mut m = Mutex[Map[str, Box]].init(...)` (no explicit type
annotation) infers `m`'s type with the **nested** `Map[str, Box]` args dropped, so
`m.get().get(key)` emits `((V*)...)` — an undeclared generic placeholder → C compile
error. Root cause: type inference from a `Outer[Inner[A,B]].init(...)` constructor
call flattens the inner generic args. Workaround (what watax does): annotate the
local — `mut m: Mutex[Map[str, Box]] = ...`. Fix: preserve nested type args in
constructor-call return-type inference.

## How these map to the roadmap

F-1 and F-2 are precisely the "is_droppable_sym heuristic has holes" problem the MIR
ownership analysis is meant to retire: a principled last-use + ownership-transfer
analysis would drop the container in F-1 and know the borrow in F-2 doesn't need
suppression. They are the concrete motivating cases for that work.
