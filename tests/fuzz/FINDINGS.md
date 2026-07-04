# Fuzz oracle — findings

The ownership fuzzer (`tests/fuzz/gen.py` + `scripts/fuzz_check.sh`) surfaced these
bugs/imprecisions on its **first runs**. They are why the generator has a `CORE`
tier (leak-free, sound — the green regression gate) and a `HARD` tier (`FUZZ_HARD=1`
— the patterns below, kept out of the gate so they don't mask regressions). Each is
reproducible: `FUZZ_HARD=1 python3 tests/fuzz/gen.py <seed>`, or `FUZZ_ONLY=<frag>`.

The point of building the oracle was exactly this: turn "found by accident while
debugging watax" into "found by a script." All four below were found by the script.

## Open

### F-1 — `Vec[HeapClass]` with an element `.get()` borrow leaks the container
`FUZZ_ONLY=f_vec_box`. A `Vec[Box]` local that is read with `v.get(i)` (borrowing an
element) is left **non-dropped** — the element borrow marks the container escaped
(to avoid freeing the borrowed element), but then the container and its elements are
never released. ~16 allocs/iteration leaked. Severity: leak, not UAF. This is the
`container_borrows` / `coll_escaped` interaction and is the highest-value fix.

### F-2 — class-with-`free()` passed to a *borrowing* function leaks it
`FUZZ_ONLY=f_owned_use`. The class-with-`free` double-free fix (is_droppable_sym) is
conservative: a `free()`-class passed as a non-receiver argument is marked
`coll_escaped` and never auto-dropped — correct when the callee frees it, but a
**leak** when the callee only borrows it (`use_named(a)` reads, doesn't free). "A
leak at worst, never a UAF," as documented — but a precision gap. A proper fix needs
per-callee "does this parameter take ownership / free it?" analysis (a job for the
MIR ownership pass).

### F-3 — `Mutex[T]` local leaks its guarded content `T`
`FUZZ_ONLY=f_mutex_get`. A `Mutex[Box]` local that goes out of scope frees the mutex
box but **not** the `Box` it guards (`_tr_mutexbox_cleanup` releases the lock, not
the payload). Rare in practice (mutexes are usually long-lived globals), but a real
per-instance leak.

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
