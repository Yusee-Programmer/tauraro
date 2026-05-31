# 13 — Memory and Ownership

---

## The Core Guarantee

> *The compiler inserts every `free()`. You never write one.*

Tauraro's ownership system is a compiler analysis — not a set of rules you manually follow, but an automatic inference pass that runs on every function. You write clean code. The compiler determines where memory is freed, when pointers are valid, and whether access is safe.

**Contrast with C:** You call `free()` manually. Forget it: memory leak. Double it: heap corruption. Use after: undefined behavior.  
**Contrast with Rust:** You express ownership explicitly via `&`, `&mut`, `Box`, `Rc`, lifetimes `'a`. Correct but verbose.  
**Tauraro:** You write `mut p = Point.init(3, 4)`. The compiler determines `p` is `Own`, injects `free(p)` at scope exit, and verifies no use-after-free.

---

## Ownership States

Every variable has exactly one ownership state, assigned by the semantic analysis phase:

| State | Meaning | `free()` injected? |
|-------|---------|-------------------|
| `Own` | Variable owns heap memory | Yes, at scope exit |
| `Borrow` | Temporary read/write access — caller still owns | No |
| `Move` | Ownership transferred to another binding | No (new owner handles it) |
| `Shared` | Reference-counted via `shared` keyword | Yes, when refcount drops to zero |
| `Stack` | Stack-allocated or scalar value | No |

You never annotate these. The compiler assigns them based on:
- How the variable is initialized (heap allocation → `Own`)
- How it's passed to functions (by value → Borrow, by name in consuming context → Move)
- Whether `shared` was used

---

## Rule M-1: Automatic Ownership Inference

Every heap allocation — `Point.init()`, `[]`, `{}`, `alloc[T](n)` — is marked `Own`. The compiler traces the value through assignments and injects a `free()` at scope exit — exactly once, on every exit path:

```python
def example() -> void:
    mut p = Point.init(1, 2)    # Own: p owns this heap Point
    p.describe()                 # Borrow: describe takes p, doesn't consume it
    # scope ends → free(p) injected automatically
```

---

## Rule M-2: No Use After Move

Once a variable is moved (ownership transferred to another context), the original binding is invalid:

```python
data = load_bytes()
send(data)              # data moved into send — send takes ownership
print(len(data))        # ERROR [M-2]: 'data' was moved and is no longer valid
```

**The compiler traces the control-flow graph.** If `send` is defined as taking an `Own` parameter, every path after the call is checked to ensure `data` is not accessed.

**Fix patterns:**
```python
# Option 1: Use before moving
print(len(data))         # use first
send(data)               # then move

# Option 2: Clone (if the type supports it)
mut copy = clone(data)
send(data)
print(len(copy))

# Option 3: Borrow semantics — pass by pointer
send_ref(&data)          # send takes a reference, data still valid after
print(len(data))
```

---

## Rule M-3: No Double-Free

The compiler emits exactly one `free()` per owned variable, on every exit path. For branching code, each branch gets its own `free()` — so `free()` executes exactly once no matter which path is taken:

```python
def conditional(flag: bool) -> void:
    mut obj = MyClass.init()

    if flag:
        return                   # free(obj) injected here
    
    obj.process()
    # free(obj) injected here too
```

---

## Rule M-4: No Dangling Pointers

A reference (borrow) cannot outlive its source. Returning a pointer to a local variable is rejected:

```python
def get_local_ref() -> Point:
    mut p = Point.init(1, 2)
    return &p                  # ERROR [M-4]: returning pointer to local variable 'p'
                               # 'p' will be freed when this function returns
```

**Fix:** Return the value (transfer ownership):
```python
def get_point() -> Point:
    return Point.init(1, 2)    # OK: transfers ownership to caller
```

---

## Rule M-5: No Aliased Mutation

While a borrow to `x` is active, `x` cannot be reassigned or mutated in ways that would invalidate the borrow:

```python
mut list = [1, 2, 3]
view = list               # 'view' borrows list
list = [4, 5, 6]          # ERROR [M-5]: cannot reassign 'list' while 'view' borrows it
```

This prevents iterator invalidation — a common C++ bug where the container is modified while iterating.

---

## Rule M-6: Immutable by Default

Variables declared without `mut` cannot be reassigned:

```python
count = 10
count = count + 1    # ERROR [M-6]: cannot assign to immutable binding 'count'
```

**Fix:** `mut count = 10`

---

## Rule M-7: None Requires Optional Type

`none` can only be assigned to pointer types and `Option[T]`:

```python
mut x: int = none    # ERROR [M-7]: cannot assign 'none' to 'x' which has type 'int'
                     # FIX: use Option[int] as the type, or give 'x' a real initial value
```

**Fix:**
```python
mut x: Option[int] = none    # OK: Option can hold none
mut x: int = 0               # OK: use zero as the "not set" sentinel
```

---

## Ownership in Practice

### Passing to Functions

By default, class instances are passed by borrow — the function reads the instance, the caller retains ownership:

```python
def describe(p: Point) -> void:    # borrows p — Point* in C
    print(f"Point({p.x}, {p.y})")
    # no free here

def main():
    mut p = Point.init(3, 4)
    describe(p)                     # borrow — p still valid
    describe(p)                     # borrow again — still valid
    print(p.x)                      # still valid
    # scope ends → free(p) injected
```

### Returning Ownership

When a function returns a heap object, it transfers ownership to the caller:

```python
def make_list(n: int) -> List[int]:
    mut result: List[int] = []
    for i in range(n):
        result.append(i * i)
    return result     # ownership transferred — no free injected here

def main():
    mut nums = make_list(5)    # nums owns the returned list
    for x in nums: print(x)
    # scope ends → List_i64_free(nums) injected
```

### Ownership and Branches

The compiler handles early returns correctly:

```python
def find_first(items: List[str], prefix: str) -> str:
    mut i = 0
    while i < len(items):
        if items[i][0] as char == prefix[0] as char:
            return items[i]    # returns a borrowed str — no free
        i = i + 1
    return ""    # returns a literal — no free
```

---

## Shared Ownership

For values that must be accessed from multiple places simultaneously — especially across threads — use `shared`:

```python
mut counter = Counter.init(0)
shared s1 = counter           # ref count = 1
shared s2 = s1                # ref count = 2, both point to same Counter
s1.increment()
s2.increment()
print(s1.get())               # 2 — both accessed the same Counter
# scope ends: s1 drops (ref count → 1), s2 drops (ref count → 0 → free)
```

### How shared Works

`shared` wraps the object in a reference-counted box with an atomic reference count. When the last `shared` reference drops, the underlying object is freed. The reference count itself is thread-safe — atomic increments and decrements without a mutex.

**Important:** `shared` makes the reference count thread-safe. It does **not** make mutations to the underlying object thread-safe. If multiple threads call `s1.increment()` simultaneously, the counter's internal state may race. Use a mutex for mutually exclusive access:

```python
extern "C":
    def pthread_mutex_lock(m: Pointer[void]) -> int
    def pthread_mutex_unlock(m: Pointer[void]) -> int
```

Or use `shared` only for read-only objects.

### When to Use `shared`

- Configuration objects accessed by multiple threads
- Shared read-only data structures
- Reference-counted resources (file handles, connections)

**Don't use `shared` for single-threaded ownership** — regular `Own` variables are simpler and faster.

---

## The Scope of Ownership Tracking

The ownership system tracks all variables in safe code. Variables in `unsafe:` blocks declared as `Pointer[T]` are **not** tracked — you are responsible for their lifetime.

```python
def main():
    mut p = Point.init(3, 4)         # Own — tracked, freed at scope exit

    unsafe:
        mut raw: Pointer[Point] = p as Pointer[Point]   # NOT tracked
        # raw is a raw pointer — the compiler doesn't free it
        # p (the Own variable) is still tracked and freed at scope exit
```

This is why `unsafe:` is the quarantine zone — it's where the ownership guarantees end.

---


---

## Safety Rules Summary

| Rule | Guarantee | Error code |
|------|-----------|-----------|
| M-1: Ownership inference | Every Own variable freed exactly once | — |
| M-2: No use-after-move | Cannot access a moved variable | `[M-2]` |
| M-3: No double-free | Single `free()` per path | — |
| M-4: No dangling pointers | Borrows can't outlive source | `[M-4]` |
| M-5: No aliased mutation | Can't mutate while borrowed | `[M-5]` |
| M-6: Immutable by default | Can't reassign without `mut` | `[M-6]` |
| M-7: None requires optional | `none` only for pointer/Option types | `[M-7]` |

---

Next: [Unsafe & Pointers →](14_unsafe_and_pointers.md)
