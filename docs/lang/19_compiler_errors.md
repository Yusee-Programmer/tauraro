# 19 — Compiler Error Reference

Every error the Tauraro compiler emits includes a rule code `[X-N]`. This page
documents every rule **as actually implemented in `src/sema.tr`**, organized by
category, with its cause, a triggering example, and the fix.

> Some codes are emitted from more than one check (see "Known overlaps" at the
> bottom). Where that's the case, each situation is documented as a separate
> numbered case under the same code.

---

## Quick Reference

| Code | Category | Short Description |
|------|----------|-------------------|
| [M-1] | Memory | Use after move |
| [M-2] | Memory | Move while borrowed |
| [M-3] | Memory | Aliased mutable access in a call |
| [M-4] | Memory | Mutation while borrowed |
| [M-5] | Memory | Use of a possibly-moved value (flow-sensitive) |
| [M-6] | Memory | Use after `dealloc()` |
| [M-7] | Memory | `none` assigned to a non-`Option` type |
| [M-8] | Memory | Assign to immutable binding |
| [T-1] | Concurrency | Type crossing a thread boundary is not `Sendable` |
| [T-2] | Concurrency | `Sendable` class has a non-`Sendable` field |
| [T-3] | Concurrency (warning) | `Sendable` class has an unguarded primitive field |
| [T-4] | Type | Unhandled `Result` from a `throws` call |
| [T-5] | Type | Numeric value used as an `if`/`while` condition |
| [T-6] | Concurrency | A borrow (`ref`/`mut ref`) passed across a thread boundary |
| [N-1] | Name | Reserved/keyword name used as a declaration |
| [F-3] | Function | Missing `return` on a code path |
| [E-1] | Existence | (1) Non-exhaustive `match`; (2) explicit `main()` call; (3) no such method on type |
| [E-2] | Existence | Nested declaration used outside `main()` |
| [S-1] | Shared | `Shared[Self]` field creates a reference cycle |
| [I-1] | Interface / Init | (1) `implements` an undefined interface; (2) variable used before assignment |
| [I-2] | Interface / Init | (1) Class missing an interface method; (2) variable not initialized on all paths |
| [I-3] | Interface | Method signature doesn't match the interface |
| [L-1] | Lifetime | Local pointer may not outlive its function |
| [P-1] | Unsafe | `.write()` on a `Pointer` outside `unsafe:` |
| [U-1] | Unsafe | `alloc`/`dealloc`/`alloc_array` outside `unsafe:` (with `--strict`) |

See [13 — Memory and Ownership](13_memory_and_ownership.md) for the conceptual
model behind the M-series and L-1.

---

## Memory Rules (M-series)

### [M-1] Use After Move

**Message:** `'data' was moved and cannot be used again.`

**Cause:** A variable was passed to a function/context that takes ownership
(moves it), then used again afterward.

```python
# WRONG:
data = load_bytes()
send(data)           # data is moved here
print(len(data))     # M-1: data was moved and cannot be used again

# RIGHT: use before moving, or clone
print(len(data))
send(data)
```

**FIX:** Use the variable that now owns the value, or call `.clone()` to copy
before moving.

---

### [M-2] Move While Borrowed

**Message:** `Cannot move 'data' while it is borrowed.`

**Cause:** A variable is moved into another binding/call while an active
borrow (`ref`/`mut_ref`/a `view`-style local) into it is still alive.

```python
mut data = [1, 2, 3]
view = ref data[0]
consume(data)    # M-2: data is borrowed by 'view'
```

**FIX:** The borrow must end before `data` can be moved.

---

### [M-3] Aliased Mutable Access in a Call

**Message:** `'buf' appears twice in the same call, creating aliased mutable access.`

**Cause:** The same mutable value is passed as two arguments to the same call,
giving the callee two simultaneous mutable handles to the same memory.

```python
mut buf = [1, 2, 3]
merge(buf, buf)    # M-3: 'buf' appears twice in the same call
```

**FIX:** Clone one of the arguments: `merge(buf, buf.clone())`.

---

### [M-4] Mutation While Borrowed

**Message:** `Cannot mutate 'items' while 'view' holds a reference into it.`

**Cause:** A container/value is mutated (e.g. `.append()`, re-assignment,
`.clear()`) while a `ref`/`mut_ref` borrow into it is still live.

```python
mut items = [1, 2, 3]
view = ref items[0]
items.append(4)    # M-4: 'items' is mutated while 'view' borrows into it
```

**FIX:** Finish using `view` before modifying `items`, or copy it first:
`mut copy = view`.

---

### [M-5] Use of a Possibly-Moved Value (Flow-Sensitive)

**Message:** `'data' may have been moved on some code paths, making this use unsafe.`

**Cause:** On at least one branch of an `if`/`match`/loop, `data` was moved,
but on others it wasn't — so using `data` after the branches merge is unsafe
in general.

```python
data = load_bytes()
if flag:
    send(data)        # moved on this branch only
print(len(data))       # M-5: 'data' may have been moved on some code paths
```

**FIX:** Ensure `data` is not moved before this point on any branch, or
restructure so the use is inside the branch where it's still valid.

---

### [M-6] Use After `dealloc()`

**Message:** `'buf' was freed by 'dealloc()' and can no longer be used.`

**Cause:** A `Pointer[T]` that was passed to `dealloc()` is used again. This
also covers double-`dealloc()` (the second call is itself a use of a freed
pointer).

```python
unsafe:
    mut buf: Pointer[char] = alloc[char](128)
    dealloc(buf)
    dealloc(buf)    # M-6: 'buf' was freed by 'dealloc()' and can no longer be used
```

**FIX:** Remove all uses of `buf` after `dealloc()`, or restructure so the
pointer is freed only when no longer needed.

---

### [M-7] `none` Assigned to a Non-`Option` Type

**Message:** `Cannot assign 'none' to 'x' which has type 'int'. Only Option[T] can hold 'none'.`

```python
# WRONG:
mut x: int = none    # M-7

# RIGHT:
mut x: Option[int] = none    # Option can hold none
mut x: int = 0               # use a real value when 'not set' is needed
```

**FIX:** Use `Option[T]` as the type, or give the variable a real initial value.

---

### [M-8] Immutable by Default

**Message:** `Cannot assign to 'count' a second time because it is immutable.`

**Cause:** Assigning to a variable declared without `mut`.

```python
# WRONG:
x = 10
x = 20    # M-8: cannot assign to 'x' a second time because it is immutable

# RIGHT:
mut x = 10
x = 20    # OK
```

**FIX:** Declare it as `mut count = ...` if it needs to change.

---

## No Double-Free, No Dangling Pointers

These are *structural guarantees*, not separate error codes:

- **No double-free of `Own` variables**: the compiler injects exactly one
  `free()` per `Own` variable at scope exit, on every path. Manual
  double-`dealloc()` of an `unsafe:` pointer is caught by **[M-6]** above.
- **No dangling pointers from local returns**: returning a pointer/borrow into
  a local without a `from` annotation is caught by **[L-1]** (see below), not
  an M-code.

---

## Concurrency Rules (T-1 / T-2 / T-3)

See [16 — Concurrency](16_concurrency.md) and
[Advanced — Sendable](advanced/06_sendable.md) for the full model.

### [T-1] Type Is Not Sendable

**Message:** `Type 'X' is not Sendable and cannot be safely shared across threads.` /
`'Shared[X]' cannot safely cross thread boundaries because 'X' is not Sendable.`

**Cause:** A value crossing a thread boundary (`spawn`, `Thread.spawn`,
`ThreadPool.spawn`, or `Shared[T]`) has a type that does not
`implements Sendable`.

```python
class Config:
    pub data: List[int]    # List is NOT Sendable

def main():
    mut cfg = Config()
    spawn process(cfg)     # T-1: 'Config' is not Sendable
```

**FIX:** Wrap in `Mutex[Config]`/`Atomic[T]` for the offending field(s), or add
`implements Sendable` to `Config` once its fields are all `Sendable`.

---

### [T-2] Sendable Class Has a Non-Sendable Field

**Message:** `Class 'Counter' declares 'implements Sendable' but field 'data: List[int]' is not Sendable.`

```python
# WRONG:
class Counter implements Sendable:
    pub items: List[int]    # T-2: List[int] is not Sendable

# RIGHT:
class Counter implements Sendable:
    pub items: Mutex[List[int]]
```

**FIX:** Wrap the field in `Mutex[T]`/`RwLock[T]`/`Atomic[T]`, or remove
`implements Sendable` if the class is only used on one thread.

---

### [T-3] Primitive Field in a Sendable Class (warning)

**Message:** `Sendable class 'Counter' has primitive field 'count: int' that may cause data races if mutated from multiple threads.`

This is a **warning**, not an error — primitive fields are allowed in
`Sendable` classes (unlike T-2), but mutating them from multiple threads
without `Atomic[T]` is a data race.

```python
class Counter implements Sendable:
    pub count: int    # T-3 warning

# Fix for concurrent mutation:
class Counter implements Sendable:
    pub count: Atomic[int]
```

**FIX:** Use `Atomic[T]` for fields mutated from multiple threads, or ensure
the field is written only before the object is shared.

---

### [T-6] Borrow Crossing a Thread Boundary

**Message:** `a borrow (ref/mut ref) cannot cross a thread boundary: the borrowed
value may be mutated or freed by another thread, or outlive its source.`

`Thread.spawn`/`ThreadPool.spawn`/`spawn:` are not *scoped* — the spawned work
may run after the borrow's source goes out of scope, and a `mut ref` shared with
another thread is an aliased mutable access (a data race). This is the same reason
Rust's `thread::spawn` requires `'static`. (`[T-1]` does not catch it because a
`ref T` parameter erases to a Sendable `T`.)

```python
def run(x: mut ref int):
    mut t: Thread = Thread.spawn(worker, x)   # T-6: borrow crosses the boundary
    t.join()
```

**FIX:** Pass an *owned* value, a `Shared[T]` (reference-counted handle), or a
`Mutex[T]`/`Atomic[T]` instead of a borrow.

---

## Type Rules (T-4 / T-5)

### [T-4] Unhandled Result from `throws` Function

**Message:** `'parse_int()' returns a Result and its error must be handled.`

```python
# WRONG:
parse_int(s)    # T-4: result discarded

# RIGHT (three options):
parse_int(s)?             # propagate with ?
mut r = parse_int(s)      # assign to a variable
_ = parse_int(s)          # explicitly discard
```

**FIX:** Assign the result and match on it, use `?` to propagate, or
`_ = fn(...)` to explicitly discard.

---

### [T-5] Numeric Value Used as a Condition

**Message:** `'x' is a number (int) and cannot be used as an 'if' condition.`

**Cause:** Tauraro does not treat `0`/non-zero as truthy/falsy implicitly —
`if`/`while` conditions must be `bool`.

```python
mut x: int = 0

# WRONG:
if x:           # T-5: 'x' is a number and cannot be used as an 'if' condition
    print("nonzero")

# RIGHT:
if x != 0:
    print("nonzero")
```

**FIX:** Write `if x != 0:` to explicitly check for non-zero.

---

## Name Rules (N-series)

### [N-1] Reserved Name Used as Declaration

**Message:** `'int' is a keyword/built-in type and cannot be used as a name.` /
`'int' is a ... and is reserved.`

**Cause:** A variable, function, or top-level declaration was given the name
of a language keyword or built-in type (`int`, `float`, `str`, `bool`, `List`,
`Dict`, `Option`, `Result`, ...).

```python
# WRONG:
def int(x: str) -> int:    # N-1: 'int' is reserved
    return 0

# RIGHT:
def to_int(x: str) -> int:
    return int(x)
```

**FIX:** Choose a different name (e.g. `my_int`, `to_int`).

---

## Function Rules (F-series)

### [F-3] Missing Return on Code Path

**Message:** `Function 'sign' returns 'int' but is missing a return statement on at least one code path.`

```python
# WRONG:
def sign(n: int) -> int:
    if n > 0: return 1
    if n < 0: return -1
    # F-3: the n == 0 path has no return

# RIGHT:
def sign(n: int) -> int:
    if n > 0:   return 1
    elif n < 0: return -1
    else:       return 0
```

**FIX:** Add a return at the end, or ensure all `if`/`elif`/`else` branches
return.

**Not checked for:** `void` functions, `init`/constructor functions, interface
method signatures (no body), `extern "C"` declarations (no body).

> `[F-1]` and `[F-2]` are reserved for future function-call checks
> (argument-count and parameter-shadowing) — see "Reserved / Not Yet
> Implemented" below.

---

## Existence Rules (E-series)

`[E-1]` is currently emitted for **three unrelated situations**. All three
share the "E" (existence/exhaustiveness) category but are otherwise distinct
checks — see "Known overlaps" below.

### [E-1] (1): Non-Exhaustive `match`

**Message:** `Non-exhaustive match on 'Color': missing variant 'Blue'`

**Cause:** A `match` on an `enum` value does not cover every variant and has
no wildcard (`_`) arm.

```python
enum Color:
    Red
    Green
    Blue

def name(c: Color) -> str:
    match c:
        Color.Red:   return "red"
        Color.Green: return "green"
    # E-1: missing variant 'Blue'
```

**FIX:** Add a `Color.Blue` arm, or a wildcard `_:` arm.

---

### [E-1] (2): Explicit Call to `main()`

**Message:** `Explicit call to 'main()' is forbidden. The compiler automatically invokes main() as the program entry point.`

```python
def main():
    print("hi")
    main()    # E-1: explicit call to main() is forbidden
```

**FIX:** Remove the `main()` call — the compiler invokes it automatically.

---

### [E-1] (3): No Method Found on Type

**Message:** `No method 'foo' found on type 'Bar'.`

**Cause:** A method call `obj.foo(...)` was made on a value whose class `Bar`
(nor any base class) declares a method `foo`, and `foo` is not a universal
dunder/built-in (`init`, `to_str`, `__eq__`, ...) or a compiler-dispatched
method on a built-in type (`Thread`, `Mutex`, `File`, `OS`, ...).

```python
pub class Foo:
    pub x: int

def main():
    mut f = Foo()
    f.nonexistent_method()   # E-1: no method 'nonexistent_method' found on type 'Foo'
```

**FIX:** Define `pub def nonexistent_method(self, ...)` in `Foo`, or add it via
`extend Foo:` on `Foo` or a base class.

---

### [E-2] Nested Declaration Outside `main()`

**Message:** `Nested class/def/enum/interface declarations are only supported inside main().`

**Cause:** A `class`, `def`, `enum`, `interface`, or `extend` statement appears
inside the body of a function other than `main()`. See
[Local (Nested) Declarations in `main()`](05_functions.md#local-nested-declarations-in-main).

```python
def helper():
    class Foo:    # E-2: not inside main()
        pub x: int

def main():
    class Foo:    # OK: declared inside main()
        pub x: int
```

**FIX:** Move the declaration to module (top-level) scope, or move the
surrounding logic into `main()`.

---

## Shared Ownership (S-series)

### [S-1] `Shared[Self]` Reference Cycle

**Message:** `'Node' has a 'Shared[Node]' field 'parent' - this creates a reference cycle that leaks memory.`

**Cause:** A class has a field typed `Shared[Self]` (directly or via a field
of its own type), which creates a strong reference cycle that the
reference-counted `Shared[T]` can never collect.

```python
# WRONG:
class Node implements Shared:
    pub parent: Shared[Node]    # S-1: reference cycle

# RIGHT: use Weak for back-references
class Node implements Shared:
    pub parent: Weak[Node]
```

**FIX:** Use `Weak[Node]` for back-references to break the cycle.

---

## Interface Rules (I-series)

`[I-1]` and `[I-2]` are each emitted for **two unrelated situations** — one
about `implements`/interface conformance, the other about definite
assignment of local variables. `[I-3]` is interface-only. See "Known overlaps"
below.

### [I-1] (1): `implements` an Undefined Interface

**Message:** `Class 'Worker' declares 'implements Runnable' but interface 'Runnable' is not defined.`

```python
# WRONG: 'Runnable' was never declared
class Worker implements Runnable:
    pub def run(self): ...
```

**FIX:** Define `interface Runnable:` before this class, or check for typos.

---

### [I-1] (2): Variable Used Before Assignment

**Message:** `Variable 'total' is used before being assigned a value.`

```python
def sum_positive(items: List[int]) -> int:
    mut total: int
    print(total)    # I-1: 'total' is used before being assigned a value
    for x in items:
        if x > 0: total = total + x
    return total
```

**FIX:** Assign a value before use, e.g. `mut total = 0`.

---

### [I-2] (1): Class Missing an Interface Method

**Message:** `Class 'Worker' implements 'Runnable' but is missing method 'run'.`

```python
interface Runnable:
    def run(self) -> void

# WRONG: Worker never defines run()
class Worker implements Runnable:
    pub id: int
```

**FIX:** Add the missing method to `extend Worker:`.

---

### [I-2] (2): Not Initialized on All Code Paths

**Message:** `'total' is not initialized on all code paths before this use.`

```python
def classify(n: int) -> int:
    mut result: int
    if n > 0:
        result = 1
    # I-2: 'result' is not initialized on all code paths (n <= 0 branch)
    return result
```

**FIX:** Initialize `result` before the `if`, or ensure every branch assigns
a value.

---

### [I-3] Method Signature Mismatch With Interface

**Message:** `Class 'Worker': method 'run' returns 'int' but interface 'Runnable' declares '-> void'.` /
`Class 'Worker': method 'run' has 2 parameter(s) but interface 'Runnable' requires 1.`

**Cause:** A class implements an interface, but one of its methods has a
different return type or parameter count than the interface declares.

```python
interface Runnable:
    def run(self) -> void

# WRONG: return type mismatch
class Worker implements Runnable:
    pub def run(self) -> int:    # I-3: declares '-> void'
        return 0
```

**FIX:** Match the interface's return type and parameter list exactly.

---

## Lifetime Rules (L-series)

### [L-1] Local Pointer May Not Outlive Its Function

**Message:** `'x' is a local Pointer that may not outlive this function call. Returning it is unsafe.`

**Cause:** A function returns a `Pointer[T]`/`ref T`/`mut_ref T` that points
into a local variable (not a parameter), without the memory escaping via
`unsafe:` heap allocation. The pointee is freed when the function returns.

```python
# WRONG:
def get_ref() -> Pointer[int]:
    mut x = 42
    return &x       # L-1: 'x' is a local Pointer that may not outlive this function

# RIGHT: return by value
def get_value() -> int:
    return 42
```

When you genuinely need to return a pointer into a **parameter's** data, use
the `from` lifetime annotation — see
[Advanced — Lifetimes](advanced/01_lifetimes.md):

```python
def get_first(data: List[int]) -> Pointer[int] from data:
    return data.raw_ptr()
```

**FIX:** Annotate the return type with `from <param>` if the pointer borrows
from a parameter, or wrap the allocation in `unsafe:` if it is genuinely
heap-allocated and intentionally escapes.

---

## Unsafe Rules (P-series / U-series)

### [P-1] `.write()` on a Pointer Outside `unsafe:`

**Message:** `'.write()' on a Pointer mutates raw memory and must be inside an 'unsafe:' block.`

```python
# WRONG:
mut p: Pointer[int] = ...
p.write(42)    # P-1: must be inside 'unsafe:'

# RIGHT:
unsafe:
    p.write(42)
```

**FIX:** Wrap the call in `unsafe:`.

---

### [U-1] Manual Memory Outside `unsafe:` Block (`--strict`)

**Message:** `'alloc' used outside an 'unsafe:' block.` / `'alloc'/'dealloc' used outside an 'unsafe:' block.`

**Cause:** `alloc`/`dealloc`/`alloc_array` (and similar raw-memory builtins)
were called outside an `unsafe:` block when the compiler was invoked with
`--strict`.

```python
# WRONG (with --strict):
mut buf: Pointer[char] = alloc[char](256)    # U-1

# RIGHT:
unsafe:
    mut buf: Pointer[char] = alloc[char](256)
    # ... use buf ...
    dealloc(buf)
```

**FIX:** Wrap raw memory operations in `unsafe:` to signal manual memory
management.

**Best Practice:** Always compile with `--strict` for production code — the
`unsafe:` annotation documents that the block has been manually reviewed.

---

## Known Overlaps (Same Code, Different Checks)

These codes are emitted by more than one independent check in `src/sema.tr`.
This is documented here for accuracy rather than fixed by renaming, to avoid
an error-code churn for tooling that pattern-matches on codes mid-0.x. A
future `0.x` release may split these into distinct codes (noted in
`CHANGELOG.md` as a breaking change when it happens — see
[00 — Versioning Policy](../dev/00_versioning_policy.md)):

- **`[E-1]`**: non-exhaustive `match` / explicit `main()` call / no method on type.
- **`[I-1]`**: undefined `implements` interface / variable used before assignment.
- **`[I-2]`**: class missing an interface method / variable not initialized on all paths.

---

## Reserved / Not Yet Implemented

These codes (or specific checks) are referenced in other docs as planned
diagnostics but are **not currently emitted** by `src/sema.tr`. Code that
triggers these situations either compiles without a dedicated error, or fails
later with a generic message:

- **`[F-1]`** — Wrong number of arguments to a function call. Not currently
  checked by sema; a mismatched call may fail at the C-compiler stage with a
  generic error instead.
- **`[F-2]`** — A local variable shadowing a function parameter name. Not
  currently detected; the shadowing local simply takes effect (see
  [05 — Functions](05_functions.md#function-rules-quick-reference)).
- **Numeric type-mismatch diagnostics** described in
  [02 — Variables and Types](02_variables_and_types.md) (e.g. assigning `int`
  to `float` without `as`, `int + float` without a cast) and
  [03 — Operators](03_operators.md), [04 — Control Flow](04_control_flow.md)
  (ternary branch type mismatch), and [07 — Collections](07_collections.md)
  (list element / list-assignment type mismatch) — these produce a generic
  type error today, not a stable `[T-N]` code. `[T-1]`/`[T-2]`/`[T-5]` are
  already allocated to the concurrency and condition checks documented above.
- **`mut x` with no type and no initializer** (["cannot infer type"], see
  [02 — Variables and Types](02_variables_and_types.md)) — produces a generic
  error, not a dedicated code.

---

## Parse Errors

Parse errors have no rule code — they are reported directly with the source
location.

### Unexpected Indentation

```
ParseError at line 5: unexpected indentation
```

**Cause:** Inconsistent indentation — mixing tabs and spaces, or wrong depth.

```python
# WRONG:
def foo():
    x = 1
  y = 2    # 2 spaces instead of 4
```

**Fix:** Use exactly 4 spaces per indentation level. Never use tabs.

---

### Unexpected Token

```
ParseError at line 8: unexpected token '='
```

**Cause:** Often a syntax error in an expression, or a keyword used in the
wrong context.

---

### Missing Colon

```
ParseError at line 3: expected ':' after 'if' condition
```

**Fix:** Add `:` at the end of every `if`, `elif`, `else`, `while`, `for`,
`def`, `class`, and `extend` header.

---

### Missing Parameter Type Annotation

```
ParseError at line 2: expected ':' after parameter name
```

**Cause:** A function parameter was written without `: Type`. All parameters
must be annotated (see [05 — Functions](05_functions.md)).

---

## Linker / C Compiler Errors

These appear after the Tauraro compiler succeeds but the C compiler
(GCC/Clang) fails.

### undefined reference to `function_name`

**Cause:** A function declared in `extern "C"` is not present in any linked
library.

```
undefined reference to `curl_easy_init`
```

**Fix:** Pass the library: `tauraroc main.tr -l curl --run`

---

### conflicting types for 'name'

**Cause:** Two declarations of the same C symbol with different types —
typically a mismatch between your `extern "C"` annotation and the actual C
function signature.

**Fix:** Correct the `extern "C"` declaration to match the C header exactly.

---

### assignment from incompatible pointer type

**Cause:** Assigning a `List_ptr*` (generic list) to a `List_i64*` (typed
list). This happens when an empty list literal `[]` has no type context.

**Fix:** Annotate empty list literals explicitly: `mut data: List[int] = []`

---

## Runtime Errors (Abort / Crash)

These are not compile errors — they crash at runtime.

### Null Pointer Dereference

**Symptom:** Segmentation fault or access violation.

**Cause:** Accessing a field or method through a null pointer.

```python
mut p: Point = none
p.x = 10    # CRASH: p is null
```

**Fix:** Initialize before use, or null-check:
```python
if p as usize != 0 as usize:
    p.x = 10
```

---

### Out-of-Bounds List Access

**Symptom:** Crash or incorrect value.

**Cause:** `list[i]` where `i >= len(list)`.

**Fix:** Bounds-check manually: `if i >= 0 and i < len(list): ...`

---

### Stack Overflow

**Symptom:** Crash on deep recursion.

**Cause:** Unbounded recursion or very deep call stacks.

**Fix:** Convert recursive algorithms to iterative form, or increase the OS
stack size limit.

---

## Diagnostic Tips

### When to use — quick summary

| Goal | Tool |
|------|------|
| Fast type-check without compiling | `tauraroc --check program.tr` |
| See what the parser understood | `tauraroc --emit ast program.tr` |
| Inspect generated C before GCC runs | `tauraroc --emit c program.tr` |
| See all pipeline phases | `tauraroc --verbose program.tr` |

### Use --check for fast feedback

```bash
tauraroc --check program.tr    # type-checks without generating C
```

### Use --emit ast to see the parse tree

```bash
tauraroc --emit ast program.tr
```

### Use --emit c to inspect generated C

```bash
tauraroc --emit c program.tr
```

If an error comes from GCC, look at the generated C and search for the
reported line number. Type mismatches and undefined symbols are usually
obvious in the C output.

### Use --verbose for the full pipeline

```bash
tauraroc --verbose program.tr    # lex → parse → sema → codegen
```

---

Next: [Advanced Patterns →](20_advanced_patterns.md)
