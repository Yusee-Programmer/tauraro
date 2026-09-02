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

### F-1 — `Vec[HeapClass]` push leaks the pushed element  ✅ CLOSED
`FUZZ_ONLY=f_vec_box` now `LIVE 0`, differential matches. Two parts: the earlier
`Box`-collision fix made the *elements* drop (`7200 → 3600`); the rest was
`v.push(Box.init(k))` emitting `List_ptr_append(v, _tr_obj_retain(Box_init(k)))` — a
fresh ctor (rc=1) was RETAINED (rc=2) instead of MOVED, leaking the temporary's ref
(the container releases only one). `_obj_store_needs_retain` / `_obj_expr_owns_ref`
now recognise a static constructor `Class.init(...)`/`.new(...)` as a fresh owned
value that TRANSFERS (no retain), exactly like the `Class(...)` ECall constructor.

### F-4 — `Mutex[Coll[..]]` leaks its owned collection payload  ✅ CLOSED
`FUZZ_ONLY=f_mutex_map` now `LIVE 0`, differential matches. Two halves: the
INFERENCE half (`Mutex[Map[str,Box]].init(...)` unannotated no longer drops the
nested value type — see the Track-1 static-ctor inference work) and the LEAK half:
a `Mutex` wrapping a FRESH collection now OWNS it. `_TrMutexBox` gained a `cdrop`
slot (`runtime/tauraro_rt.h`); codegen emits `_tr_mutexbox_new_owned_coll(...,
_mtxcd_*)` where `_mtxcd_*` is a `static inline` wrapper (recorded by `scan_mono_ty`,
emitted into the shared header) that calls the collection's typed free
(`Dict_free_objval(..., _trdrop_Box)` etc.). A collection is not an rc-object, so it
goes through `cdrop`, never `_tr_obj_release`. A borrowed (non-fresh) payload is not
owned — no double-free.

All four fuzz findings (F-1..F-4) are now leak-free and promoted to the `CORE` gate.

## How these were fixed

F-1..F-4 were the "is_droppable_sym heuristic has holes" cases the MIR ownership
work targets. The keystone was making the interprocedural `consumes(fn,i)` summary
actually usable (it was silently disabled by the `Dict_has` runtime bug — see F-2),
then applying it (`apply_borrow_drops`) plus precise transfer/borrow/consume rules
(static-ctor transfer, pure-scalar-return borrow, `.free()` consume, Mutex-owns-
collection). Every fix validated by the differential oracle (elide ≡ pure-ARC) + the
gen2≡gen3 fixpoint; the Linux ASan CI is the final UAF gate.
