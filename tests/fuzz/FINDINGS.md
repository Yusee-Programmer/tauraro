# Fuzz oracle — findings

The ownership fuzzer (`tests/fuzz/gen.py` + `scripts/fuzz_check.sh`) surfaced these
bugs/imprecisions on its **first runs**. They are why the generator has a `CORE`
tier (leak-free, sound — the green regression gate) and a `HARD` tier (`FUZZ_HARD=1`
— the patterns below, kept out of the gate so they don't mask regressions). Each is
reproducible: `FUZZ_HARD=1 python3 tests/fuzz/gen.py <seed>`, or `FUZZ_ONLY=<frag>`.

The point of building the oracle was exactly this: turn "found by accident while
debugging watax" into "found by a script." All four below were found by the script.

## Fixed

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

### F-2 — class-with-`free()` passed to a *borrowing* function leaks it
`FUZZ_ONLY=f_owned_use`. The class-with-`free` double-free fix (is_droppable_sym) is
conservative: a `free()`-class passed as a non-receiver argument is marked
`coll_escaped` and never auto-dropped — correct when the callee frees it, but a
**leak** when the callee only borrows it (`use_named(a)` reads, doesn't free). "A
leak at worst, never a UAF," as documented — but a precision gap. A proper fix needs
per-callee "does this parameter take ownership / free it?" analysis (a job for the
MIR ownership pass).

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
