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

## Rule M-1: Automatic Ownership Inference

### When to Use (Understand)

This rule is always active in safe code. Every heap allocation — `Point.init()`, `[]`, `{}`, `alloc[T](n)` — is automatically tracked.

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

# WRONG: Returning a pointer to a local variable
def get_local() -> Pointer[Point]:
    mut p = Point.init(1, 2)
    return p as Pointer[Point]    # ERROR [M-4]: p freed on return, caller holds dangling pointer
```

### Best Practices

- Never call `free()` in safe code — the compiler already does it.
- Never use `unsafe:` to manually manage memory that the ownership system already tracks.
- Let the compiler infer ownership states; only use `shared` or `unsafe:` when the use case genuinely requires it.

---

## Rule M-2: No Use After Move

### When to Use (Understand)

This rule prevents accessing a variable after its ownership has been transferred to another binding or function.

### How It Works

```python
data = load_bytes()
send(data)              # data moved into send — send takes ownership
print(len(data))        # ERROR [M-2]: 'data' was moved and is no longer valid
```

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
write(buf)            # ERROR [M-2]

# WRONG: Conditionally moving then accessing on both branches
if flag:
    send(data)        # move on true branch
print(len(data))      # ERROR [M-2]: data may have been moved
```

### Best Practices

- Use the value fully before passing it to a consuming function.
- Use `clone(x)` when you need the value both before and after a move.
- Prefer borrow semantics (`&data`, pointer parameters) in hot loops where cloning would be expensive.

---

## Rule M-3: No Double-Free

### When to Use (Understand)

This rule is enforced automatically — it exists to document that the compiler guarantees exactly one `free()` per owned variable per execution path.

### How It Works

For branching code, the compiler emits a `free()` at the exit of each branch independently. The result is that `free()` is called exactly once no matter which path is taken:

```python
def process(flag: bool) -> void:
    mut obj = MyClass.init()
    if flag:
        obj.do_thing()
        return               # free(obj) here
    obj.do_other()
    # free(obj) here
    # Neither branch double-frees
```

### Common Mistakes

```python
# WRONG: Manually freeing inside unsafe: while safe code also tracks the variable
mut p = Point.init(1, 2)
unsafe:
    free(p as Pointer[void])   # manual free
# compiler also injects free(p) at scope exit → double-free
```

### Best Practices

- Never mix manual `free()` with `Own` variables in safe code — one or the other, not both.
- If you must manage memory manually, declare the pointer only inside `unsafe:` so the ownership system does not track it.

---

## Rule M-4: No Dangling Pointers

### When to Use (Understand)

This rule prevents returning or storing a reference that will outlive the object it points to.

### How It Works

```python
def get_local_ref() -> Point:
    mut p = Point.init(1, 2)
    return &p                  # ERROR [M-4]: returning reference to local 'p'
                               # 'p' will be freed when this function returns
```

**Fix — return ownership instead:**

```python
def get_point() -> Point:
    return Point.init(1, 2)    # OK: transfers ownership to caller
```

**Lifetime annotation (`from`) — advanced use:**

When a function returns a `Pointer[T]` that is derived from one of its parameters (e.g., returning a pointer into a buffer), the compiler needs to know the pointer's lifetime is bounded by that parameter. This is expressed with the `from` keyword in the return type:

```python
def get_first[T](items: List[T]) -> Pointer[T] from items:
    return &items[0]    # pointer into items — caller must not outlive items
```

> **Note:** Lifetime annotations with `from` are an advanced feature. In most code you will never need them — the compiler auto-infers the lifetime source when a function returns `Pointer[T]` and has exactly one non-primitive parameter. See [Advanced: Lifetimes](../advanced/01_lifetimes.md) for the full details.

### Common Mistakes

```python
# WRONG: Storing a borrowed reference in a longer-lived variable
def get_name(p: Person) -> str:
    return p.name    # if name is a field reference, not a copy — may dangle

# WRONG: Returning a pointer to a stack-local inside unsafe:
unsafe:
    mut local: int = 42
    return &local as Pointer[int]    # ERROR: local freed on return
```

### Best Practices

- Return owned values from functions, not references into local variables.
- Use `Shared[T]` when multiple call sites need to hold a reference to the same heap object beyond the lifetime of the original function.
- Only use the `from` lifetime annotation if the compiler explicitly requires it — do not add it speculatively.

---

## Rule M-5: No Aliased Mutation

### When to Use (Understand)

This rule prevents modification of a container while a borrow into it is active — the Tauraro analog of iterator invalidation.

### How It Works

```python
mut list = [1, 2, 3]
view = list               # 'view' borrows list
list = [4, 5, 6]          # ERROR [M-5]: cannot reassign 'list' while 'view' borrows it
```

This prevents iterator invalidation — a common C++ bug where the container is replaced or resized while an iterator holds a pointer into the old allocation.

### Common Mistakes

```python
# WRONG: Reassigning the list inside a for loop that borrows it
mut items = [1, 2, 3]
for x in items:
    items = []    # ERROR [M-5]: cannot mutate 'items' while iterating

# WRONG: Passing both the list and a view of it to the same function
mut buf = [1, 2, 3]
view = buf
transform(buf, view)    # ERROR [M-5]: buf mutated via first arg while view borrows it
```

### Best Practices

- Finish all reads from a borrow before reassigning or consuming the source.
- Use indices instead of borrows when you need to mutate a container inside a loop body.

---

## Rule M-6: Immutable by Default

### When to Use (Understand)

All bindings are immutable by default. Use `mut` only when you need to reassign or mutate.

### How It Works

```python
count = 10
count = count + 1    # ERROR [M-6]: cannot assign to immutable binding 'count'

mut count = 10
count = count + 1    # OK
```

This is intentional: most variables are set once and never changed. `mut` is an explicit signal that a variable changes.

### Common Mistakes

```python
# WRONG: Forgetting mut on an accumulator variable
total = 0
for x in items:
    total = total + x    # ERROR [M-6]: 'total' is immutable
```

### Best Practices

- Declare variables without `mut` first; add `mut` only when the compiler requires it.
- Immutable bindings are easier to reason about — keep them as the default.

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

| Rule | Guarantee | Error code |
|------|-----------|-----------|
| M-1: Ownership inference | Every `Own` variable freed exactly once | — |
| M-2: No use-after-move | Cannot access a moved variable | `[M-2]` |
| M-3: No double-free | Single `free()` per path | — |
| M-4: No dangling pointers | Borrows cannot outlive their source | `[M-4]` |
| M-5: No aliased mutation | Cannot mutate while a borrow is active | `[M-5]` |
| M-6: Immutable by default | Cannot reassign without `mut` | `[M-6]` |
| M-7: None requires optional | `none` only valid for `Option[T]` / pointer types | `[M-7]` |

---

> **Advanced topic:** The `from` lifetime annotation (`-> Pointer[T] from param_name`) is covered in detail in [Advanced: Lifetimes](../advanced/01_lifetimes.md). In most programs you will never need it — the compiler auto-infers lifetimes for the common case.

---

Next: [Unsafe & Pointers →](14_unsafe_and_pointers.md)
