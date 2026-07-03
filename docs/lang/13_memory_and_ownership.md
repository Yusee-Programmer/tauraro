# 13 — Memory and Ownership

---

## The Core Guarantee

> *The compiler inserts every `free()`. You never write one.*

Tauraro's ownership system is a compiler analysis pass — not a set of rules you manually follow. You write clean code. The compiler determines where memory is freed, when pointers are valid, and whether every access is safe.

**Contrast with C:** You call `free()` manually. Forget it: leak. Double it: heap corruption. Use after: undefined behavior.
**Contrast with Rust:** You express ownership explicitly via `&`, `&mut`, `Box`, `Rc`, lifetime annotations `'a`. Correct but verbose.
**Tauraro:** You write `mut p = Point.init(3, 4)`. The compiler determines `p` is `Own`, injects `free(p)` at scope exit, and verifies no use-after-free.

---

## Ownership States

Every variable has exactly one ownership state, assigned automatically by the semantic analysis phase. You never annotate these:

| State | Meaning | `free()` injected? |
|-------|---------|-------------------|
| `Own` | Variable owns heap memory | Yes, at every scope exit |
| `Borrow` | Temporary read/write access — caller still owns | No |
| `Move` | Ownership transferred to another binding | No (new owner handles it) |
| `Shared` | Reference-counted via `shared` keyword | Yes, when refcount drops to zero |
| `Stack` | Stack-allocated or scalar value | No |

The compiler assigns states based on:
- How the variable is initialized (heap allocation → `Own`)
- How it is passed to functions (by reference → `Borrow`, consuming context → `Move`)
- Whether the `shared` keyword was used

---

## Automatic Ownership Inference

### When to Use (Understand)

This is always active in safe code. Every heap allocation — `Point.init()`, `[]`, `{}`, `alloc[T](n)` — is automatically tracked. It is the foundation the M-series rules below build on, but it does not itself have an error code: it's the *absence* of a diagnostic that tells you it worked.

### How It Works

The compiler marks every heap-allocated variable as `Own` and injects `free()` at every scope exit — on the happy path, on early returns, and on every branch:

```python
def example() -> void:
    mut p = Point.init(1, 2)    # Own: p owns this heap Point
    p.describe()                 # Borrow: describe reads p, does not consume it
    # scope ends → free(p) injected automatically
```

**Returning ownership to the caller:**

```python
def make_list(n: int) -> List[int]:
    mut result: List[int] = []
    for i in range(n):
        result.append(i * i)
    return result     # ownership transferred to caller — no free here

def main():
    mut nums = make_list(5)    # nums is now Own
    for x in nums: print(x)
    # scope ends → List_i64_free(nums) injected
```

**Branches handled correctly:**

```python
def conditional(flag: bool) -> void:
    mut obj = MyClass.init()
    if flag:
        return                   # free(obj) injected on this path
    obj.process()
    # free(obj) injected here too — exactly once per path
```

### Common Mistakes

```python
# WRONG: Trying to free manually — double-free
unsafe:
    free(p)    # the compiler also injects free(p) — now freed twice
```

### Best Practices

- Never call `free()` in safe code — the compiler already does it.
- Never use `unsafe:` to manually manage memory that the ownership system already tracks.
- Let the compiler infer ownership states; only use `shared` or `unsafe:` when the use case genuinely requires it.

---

## Rule M-1: No Use After Move

### When to Use (Understand)

This rule prevents accessing a variable after its ownership has been transferred to another binding or function.

### How It Works

```python
data = load_bytes()
send(data)              # data moved into send — send takes ownership
print(len(data))        # ERROR [M-1]: 'data' was moved and cannot be used again
```

**Compiler message:** `'data' was moved and cannot be used again.`
**FIX:** *Use the variable that now owns it, or call `.clone()` to copy before moving.*

The compiler traces the control-flow graph. If `send` is defined as taking an `Own` parameter, every path after the call is checked to ensure `data` is not accessed.

**Fix patterns:**

```python
# Option 1: Use the value before moving it
print(len(data))         # use first
send(data)               # then move

# Option 2: Deep copy with clone()
mut copy = clone(data)
send(data)               # move the original
print(len(copy))         # use the copy

# Option 3: Pass by reference — send does not consume ownership
send_ref(&data)
print(len(data))         # data still valid
```

### Common Mistakes

```python
# WRONG: Accessing a variable after passing it to a consuming function
mut buf = Buffer.init(1024)
compress(buf)         # buf moved
write(buf)            # ERROR [M-1]
```

### Best Practices

- Use the value fully before passing it to a consuming function.
- Use `clone(x)` when you need the value both before and after a move.
- Prefer borrow semantics (`&data`, pointer parameters) in hot loops where cloning would be expensive.

---

## Rule M-2: No Move While Borrowed

### When to Use (Understand)

This rule prevents moving a variable away while another binding currently holds a borrow into it — the move would leave that borrow dangling.

### How It Works

```python
mut data = load_bytes()
view = &data            # view borrows data
send(data)               # ERROR [M-2]: Cannot move 'data' while it is borrowed
```

**Compiler message:** `Cannot move 'data' while it is borrowed.`
**FIX:** *The borrow must end before 'data' can be moved.*

### Common Mistakes

```python
# WRONG: moving a value while a borrow of it is still in scope
mut buf = Buffer.init(1024)
view = &buf
compress(buf)    # ERROR [M-2]: buf is borrowed by 'view'
```

### Best Practices

- Finish all uses of a borrow before moving the value it borrows from.
- If you need both, `clone()` the value and move the clone, keeping the original (and its borrow) intact.

---

## Rule M-3: No Aliased Mutable Access in a Call

### When to Use (Understand)

This rule prevents passing the **same** variable twice as arguments to one call when that creates two simultaneous mutable accesses to it.

### How It Works

```python
mut buf = [1, 2, 3]
transform(buf, buf)    # ERROR [M-3]: 'buf' appears twice in the same call,
                       # creating aliased mutable access
```

**Compiler message:** `'buf' appears twice in the same call, creating aliased mutable access.`
**FIX:** *Clone one of the arguments: `buf.clone()`*

### Common Mistakes

```python
# WRONG: passing the same list as two arguments to a function that mutates one of them
mut items = [1, 2, 3]
merge_into(items, items)    # ERROR [M-3]

# RIGHT: clone one side
merge_into(items, items.clone())
```

### Best Practices

- If a function needs two views of the same data, design it to take one argument and an index/range, not two aliases.
- Use `.clone()` at the call site when aliasing is unavoidable — it's a clear, local fix.

---

## Rule M-4: No Mutation While Borrowed

### When to Use (Understand)

This rule prevents mutating a container or object while another binding holds a reference into it — the Tauraro analog of iterator invalidation.

### How It Works

```python
mut items = [1, 2, 3]
view = &items
items.push(4)    # ERROR [M-4]: Cannot mutate 'items' while 'view' holds a
                 # reference into it
```

**Compiler message:** `Cannot mutate 'items' while 'view' holds a reference into it.`
**FIX:** *Finish using `view` before modifying `items`, or copy it first: `mut copy = view`.*

This prevents iterator invalidation — a common C++ bug where the container is replaced or resized while an iterator holds a pointer into the old allocation.

### Common Mistakes

```python
# WRONG: mutating a list while iterating a borrowed view of it
mut items = [1, 2, 3]
for x in items:
    items.push(x * 2)    # ERROR [M-4]: cannot mutate 'items' while iterating
```

### Best Practices

- Finish all reads from a borrow before mutating the source.
- Collect items to add/remove into a separate list while iterating, then apply the changes after the loop ends.

---

## Rule M-5: No Use of Possibly-Moved Values

### When to Use (Understand)

This is the flow-sensitive counterpart to [M-1]: it fires when a variable was moved on **some** branches but not others, so using it afterward would be valid on some paths and a use-after-move on others.

### How It Works

```python
mut data = load_bytes()
if flag:
    send(data)        # move on the true branch only
print(len(data))      # ERROR [M-5]: 'data' may have been moved on some code
                       # paths, making this use unsafe
```

**Compiler message:** `'data' may have been moved on some code paths, making this use unsafe.`
**FIX:** *Ensure 'data' is not moved before this point on any branch, or restructure so the use is inside the branch where it's still valid.*

### Common Mistakes

```python
# WRONG: using a variable after a conditional move
if flag:
    send(data)
else:
    log(data)          # 'data' moved on both branches here too, but...
print(len(data))        # ERROR [M-5]: still flagged at the join point

# RIGHT: move both branches into the use, or move after
if flag:
    send(data)
    return
log_and_send(data)       # single move on the remaining path
```

### Best Practices

- Keep the use of a value either entirely before, or entirely inside, the branch that moves it.
- If every branch ends up moving the value, restructure so the move happens once, after the branches converge (or have each branch `return`).

---

## Rule M-6: No Use After Free (`dealloc`)

### When to Use (Understand)

This rule tracks raw pointers passed to `dealloc()` inside `unsafe:` blocks and flags any later use as use-after-free — including a second `dealloc()` (double-free).

### How It Works

```python
unsafe:
    mut buf: Pointer[char] = alloc[char](128)
    dealloc(buf)
    dealloc(buf)    # ERROR [M-6]: 'buf' was freed by 'dealloc()' and can no
                     # longer be used
```

**Compiler message:** `'buf' was freed by 'dealloc()' and can no longer be used.`
**FIX:** *Remove all uses of 'buf' after `dealloc()`, or restructure so the pointer is freed only when no longer needed.*

### Common Mistakes

```python
# WRONG: using a pointer after dealloc
unsafe:
    mut buf: Pointer[char] = alloc[char](128)
    dealloc(buf)
    process(buf)    # ERROR [M-6]: buf was already freed
```

### Best Practices

- Structure code so every allocation has exactly one `dealloc`, as the last thing done with that pointer.
- Set the pointer to a sentinel/none-equivalent (or let it go out of scope) immediately after `dealloc` if you must keep the binding around.

---

## Rule M-7: None Requires Optional Type

### When to Use (Understand)

`none` cannot be assigned to a plain value type — it is only valid for `Option[T]` or pointer types.

### How It Works

```python
mut x: int = none    # ERROR [M-7]: cannot assign 'none' to 'x' which has type 'int'
```

**Fix:**

```python
mut x: Option[int] = None    # OK: Option can hold None
mut x: int = 0               # OK: use a sentinel value instead
```

### Common Mistakes

```python
# WRONG: Using none as a "not set" flag on a plain int or float
mut count: int = none    # ERROR [M-7]

# WRONG: Assigning none to a class field without Option type
class Node:
    pub next: Node      # no Option — cannot assign none here
```

**Fix for the class field:**

```python
class Node:
    pub next: Option[Node]    # now next can be None
```

### Best Practices

- Use `Option[T]` for any value that legitimately may not exist.
- Use `0`, `-1`, or another sentinel only if the domain guarantees that sentinel is not a valid value.
- Model nullable links in linked data structures as `Option[T]` rather than bare class types.

---

## Rule M-8: Immutable by Default

### When to Use (Understand)

All bindings are immutable by default. Use `mut` only when you need to reassign or mutate.

### How It Works

```python
count = 10
count = count + 1    # ERROR [M-8]: cannot assign to 'count' a second time
                     # because it is immutable
```

**Compiler message:** `Cannot assign to 'count' a second time because it is immutable.`
**FIX:** *Declare it as `mut count = ...` if it needs to change.*

This is intentional: most variables are set once and never changed. `mut` is an explicit signal that a variable changes.

### Common Mistakes

```python
# WRONG: Forgetting mut on an accumulator variable
total = 0
for x in items:
    total = total + x    # ERROR [M-8]: 'total' is immutable
```

### Best Practices

- Declare variables without `mut` first; add `mut` only when the compiler requires it.
- Immutable bindings are easier to reason about — keep them as the default.

---

## No Double-Free and No Dangling Pointers (structural guarantees)

Two guarantees from the original ownership-system design are enforced **structurally** rather than by a single dedicated diagnostic — they're listed here for completeness since they're part of the same spec:

- **No double-free of `Own` variables:** because the compiler emits exactly one `free()` per `Own` variable per execution path (see "Automatic Ownership Inference" above), a normal `Own` local can never be double-freed in safe code. The only way to double-free is to manually `dealloc()` a raw pointer twice inside `unsafe:` — which **is** caught, as [M-6] above.
- **No dangling pointers from local returns:** returning a `Pointer[T]` that refers to a function-local is caught by **[L-1]** (see [19 — Compiler Errors §L-1](19_compiler_errors.md#l-1-local-pointer-may-not-outlive-its-function)), not by an M-series code. The fix is the same as described here: return owned values, or use a `from <param>` lifetime annotation when the pointer is derived from a parameter (see [Advanced: Lifetimes](../advanced/01_lifetimes.md)).

```python
def get_local() -> Pointer[Point]:
    mut p = Point.init(1, 2)
    return p as Pointer[Point]    # ERROR [L-1]: 'p' is a local Pointer that
                                    # may not outlive this function call
```

---

## Shared Ownership

### When to Use

Use `shared` when:
- A value must be accessed from multiple places simultaneously.
- You are sharing data across threads (read-only or behind a mutex).
- You are building reference-counted resources (file handles, connection pools).

Do **not** use `shared` for single-threaded values — plain `Own` is simpler and faster.

### How It Works

```python
mut counter = Counter.init(0)
shared s1 = counter           # ref count = 1
shared s2 = s1                # ref count = 2 — both point to the same Counter
s1.increment()
s2.increment()
print(s1.get())               # 2 — same underlying object
# scope ends: s1 drops (ref count → 1), s2 drops (ref count → 0 → free)
```

`shared` wraps the object in a reference-counted box. The reference count is atomic — safe to increment/decrement from multiple threads. When the last `shared` reference drops, the object is freed.

**Important:** `shared` makes the reference count thread-safe. It does **not** make mutations to the underlying object thread-safe. If multiple threads call `s1.increment()` simultaneously, the counter's internal state may race. Use `Mutex[T]` for mutually exclusive access.

### ARC under the hood — atomic vs non-atomic

Every heap class instance is reference-counted (ARC): the compiler inserts
retain/release automatically, so you never call `free`. The difference is *which
kind* of refcount:

| | Refcount | Cross-thread |
|---|---|---|
| a **plain** class (`Own`) | non-atomic (fast) | **No** — `!Send`, rejected by `[T-7]` (like Rust's `Rc`) |
| `shared` / `Shared[T]` | atomic | **Yes** — the `Arc` equivalent |

So `shared` is not "the way to refcount" — *everything* is refcounted. `shared` is
specifically the **atomic** refcount you need to cross a thread boundary. This is
why `[T-7]` tells you to switch a plain class to `Shared[T]` when it must be sent to
another thread.

### `Weak[T]` and reference cycles — `[S-2]`

Reference counting cannot reclaim a **cycle** of strong references (A owns B owns A):
the counts never reach zero, so it leaks. Under `--strict` the compiler *rejects*
such cycles with `[S-2]` (this generalizes `[S-1]`, which is the direct
`Shared[Self]` case). Break the cycle by making one edge **non-owning**:

```python
class Parent:
    pub children: List[Child]     # strong (owns)
class Child:
    pub parent: Weak[Parent]      # non-owning back-reference — no cycle
```

`Weak[T]` observes an object without keeping it alive; call `.upgrade()` to get an
`Option[T]` that is `Some` only while the object is still live. (`Pointer[T]` is the
raw, unchecked alternative — also a non-owning edge, but you manage validity
yourself.)

> Because of `[S-2]` (+ `[U-1]`, no unmanaged allocation), a program that compiles
> under `--strict` is provably **leak-free**: all heap memory is ARC-managed and the
> strong-ownership graph is acyclic.

### Common Mistakes

```python
# WRONG: Using shared for single-threaded values where Own is sufficient
shared big_data = load_everything()    # unnecessary ref-count overhead

# WRONG: Assuming shared protects mutation — it only protects the refcount
shared s = Counter.init(0)
# thread A: s.increment()
# thread B: s.increment()    -- RACE on counter state, not on refcount
```

### Best Practices

- Combine `shared` with `Mutex[T]` or `Atomic[T]` when threads need to mutate shared state.
- Use `shared` for read-only configuration objects accessed across threads.
- Keep `shared` at the outer scope — copy `shared` references into threads rather than passing raw borrows.

---

## Unsafe Blocks and Ownership

The ownership system tracks all variables in **safe code**. Variables declared as `Pointer[T]` inside `unsafe:` blocks are **not tracked** — you are responsible for their lifetime:

```python
def main():
    mut p = Point.init(3, 4)         # Own — tracked, freed at scope exit

    unsafe:
        mut raw: Pointer[Point] = p as Pointer[Point]   # NOT tracked
        # raw is a raw C pointer — the compiler does not inject free for raw
        # p (the Own variable) is still tracked and freed at scope exit
```

`unsafe:` is the quarantine zone where the ownership guarantees end and manual discipline begins. Keep `unsafe:` blocks small and document exactly what lifetime invariant they rely on.

---

## clone() — Deep Copy

### When to Use

Use `clone(x)` when you need an independent copy of an owned value — for example, when you want to move the original and also keep a copy.

### How It Works

```python
mut original = Buffer.init(1024)
mut copy = clone(original)
send(original)             # original moved
process(copy)              # copy is fully independent — safe to use
```

`clone()` performs a deep copy: all nested heap allocations are duplicated. It is not free — allocate only when genuinely needed.

### Common Mistakes

```python
# WRONG: Cloning in a hot loop for no reason — allocates every iteration
for item in big_list:
    mut c = clone(item)    # allocates every iteration — use a borrow instead
    read_only_fn(c)
```

### Best Practices

- Pass borrows (`&x`) to read-only functions instead of cloning.
- Clone only at ownership transfer boundaries where you need both the original and the copy to remain valid.

---

## Advanced: Releasing Memory Early

### When to Use

Scope-based auto-drop (Rule M-1) is correct and sufficient for almost all
code: memory is freed when the owning variable goes out of scope, with no
manual `free()`. There are a few advanced situations where you want memory
released **before** scope exit:

- A long-running loop (request-handling loop, parser main loop, simulation
  step) that allocates a sizeable temporary on every iteration — waiting
  until the *function* returns to free thousands of iterations' worth of
  temporaries would inflate peak memory.
- A resource that should be released as soon as you're logically done with
  it (a `StringBuilder` used to assemble one message, a buffer used to decode
  one packet), even though the enclosing scope continues for a while longer.
- Library/class authors providing an explicit "I'm done with this" API so
  callers in tight loops aren't forced to restructure their code into many
  small scopes just to get per-iteration cleanup.

For str values specifically, also see [06 — Strings §Memory: Avoiding
String-Related Leaks and Corruption](06_strings.md#memory-avoiding-string-related-leaks-and-corruption).

### How It Works

**1. Per-iteration auto-drop already happens for locals declared inside the loop body.**

```python
mut i = 0
while i < 1000000:
    mut chunk = build_chunk(i)   # Own, declared INSIDE the loop body
    process(chunk)
    # free(chunk) injected at the END OF EACH ITERATION — not deferred
    i += 1
```

No manual action needed here — declaring the temporary *inside* the loop body
(rather than reusing one `mut` declared before the loop) is enough for the
compiler to free it every iteration.

**2. `.clear()` on collections frees elements while keeping the container.**

```python
mut batch: List[str] = []
mut i = 0
while i < 1000000:
    batch.append(make_record(i))
    if len(batch) >= 1000:
        flush(batch)
        batch.clear()    # frees the 1000 buffered str elements now,
                          # keeps `batch`'s backing array for reuse
    i += 1
```

`.clear()` releases every element the collection currently holds (including
boxed `str` values — see [03 — Memory Model Internals: List_TrStr /
Dict_free_strval](../dev/03_memory_model_internals.md) for the implementation
detail) without freeing the collection itself, so you can keep reusing it.

**3. `dispose()` for resource classes — call it when you're logically done.**

Library types that wrap a resource (a `StringBuilder`, a connection, a
buffer) typically expose an explicit cleanup method so you can release it
before the enclosing scope ends:

```python
import std.core.string

mut i = 0
while i < 1000000:
    mut sb = StringBuilder.init()
    sb.append("record ")
    sb.append_int(i)
    emit(sb.to_string())   # to_string() copies out — safe to use after free
    sb.free()              # release sb's buffer NOW, not at end of `main`
    i += 1
```

If `sb` were declared once *before* the loop and reused, the compiler's
auto-drop would only free it once, at the end of the function — calling
`.free()`/`.dispose()` per iteration is how you bound memory use in that case.

**4. Manual `dealloc` for raw pointers — see [14 — Unsafe & Pointers: Manual
Allocation](14_unsafe_and_pointers.md#manual-allocation-alloc-and-dealloc).**
Every `alloc[T](n)` must be matched by exactly one `dealloc(ptr)`; this is the
one place Tauraro requires a manual free, and it is always inside `unsafe:`.

### Common Mistakes

```python
# WRONG — reusing one builder across iterations without releasing it,
# expecting per-iteration cleanup that auto-drop does not provide here
mut sb = StringBuilder.init()    # declared ONCE, before the loop
mut i = 0
while i < 1000000:
    sb.append(f"record {i}\n")
    emit_and_reset(sb)           # if this doesn't clear sb internally,
    i += 1                       # sb's buffer grows unbounded
# free(sb) only fires here, after 1,000,000 iterations of growth
```

```python
# WRONG — naming a "construct locally, mutate, then return" cleanup
# method `free` instead of `dispose` — auto-drop frees it before return
extend Conn:
    pub def take_buffer(self) -> Buffer:
        mut raw = self.buf
        raw.write(v)
        return raw    # if Buffer has a method named `free`, auto-drop
                       # may free `raw` here, before the return takes effect
```
Fix: name such cleanup methods `dispose()`, not `free()` — see [Best
Practices & Pitfalls #2](../dev/06_best_practices_pitfalls.md) if you're
writing this kind of class yourself.

### Best Practices

- Default to scope-based auto-drop. Reach for `.clear()`/`.dispose()`/`.free()`
  only when profiling or memory growth shows a long-lived loop needs it.
- Declare loop-body temporaries *inside* the loop body so per-iteration
  auto-drop applies — don't hoist a `mut` above the loop "to avoid
  reallocating" unless the type's API is specifically designed for reuse
  (e.g. `StringBuilder` + `.clear()`).
- When writing your own resource class with a manual cleanup method, name it
  `dispose()` (not `free()`) if instances are ever constructed locally,
  mutated, and returned — see [13 — Memory and Ownership] above and [Best
  Practices & Pitfalls #2](../dev/06_best_practices_pitfalls.md).
- After calling `.free()`/`.dispose()` manually, do not use the value again —
  the compiler does not track manual frees the way it tracks scope exits, so
  use-after-free on a manually freed value is your responsibility to avoid.

---

## Ownership in Practice — Quick Reference

**Passing a class to a function (borrow — most common):**

```python
def describe(p: Point) -> void:
    print(f"Point({p.x}, {p.y})")
    # no free here — caller still owns p

def main():
    mut p = Point.init(3, 4)
    describe(p)     # borrow — p still valid
    describe(p)     # borrow again
    print(p.x)      # still valid
    # scope ends → free(p)
```

**Returning ownership from a function:**

```python
def make_point(x: int, y: int) -> Point:
    return Point.init(x, y)    # caller receives ownership

def main():
    mut p = make_point(3, 4)    # p is Own
    # scope ends → free(p)
```

**Shared ownership across threads:**

```python
shared cfg = Config.load("app.cfg")
spawn_task(cfg)     # shared reference cloned into task — both refcount
spawn_task(cfg)     # another task, another clone of the shared reference
# both tasks and main hold a ref — freed only when all three drop
```

---

## Safety Rules Summary

| Guarantee | Error code |
|-----------|-----------|
| Automatic ownership inference — every `Own` variable freed exactly once | — |
| No use-after-move | `[M-1]` |
| No move while borrowed | `[M-2]` |
| No aliased mutable access in a call | `[M-3]` |
| No mutation while borrowed | `[M-4]` |
| No use of possibly-moved values (flow-sensitive) | `[M-5]` |
| No use-after-free (`dealloc`), including double-free | `[M-6]` |
| `none` requires `Option[T]` | `[M-7]` |
| Immutable by default — no reassigning without `mut` | `[M-8]` |
| No dangling pointers from local returns | `[L-1]` (see [19 — Compiler Errors](19_compiler_errors.md#l-1-local-pointer-may-not-outlive-its-function)) |

---

> **Advanced topic:** The `from` lifetime annotation (`-> Pointer[T] from param_name`) is covered in detail in [Advanced: Lifetimes](../advanced/01_lifetimes.md). In most programs you will never need it — the compiler auto-infers lifetimes for the common case.

---

> **The full picture.** This chapter describes the always-on ARC floor that makes
> every program memory-safe by default. For the *normative* statement of what
> Tauraro guarantees — the ARC-floor invariants, what `--strict` proves and
> elides, the zero-copy soundness theorem, and exactly how each guarantee is
> verified (soundness corpus, differential oracle, sanitizers) — see
> [Advanced: Safety Specification](../advanced/09_safety_spec.md).

---

Next: [Unsafe & Pointers →](14_unsafe_and_pointers.md)
