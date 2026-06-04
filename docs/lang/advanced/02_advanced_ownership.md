# Advanced — Advanced Ownership

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

[Chapter 13](../13_memory_and_ownership.md) introduces Tauraro's ownership model. This doc goes deeper: how the compiler infers ownership automatically, when a pass is a move vs. a borrow, when to reach for `Shared[T]`, and how the explicit `move` syntax works.

---

## When You Need This

- You are getting [M-2] (use after move) errors and don't understand why
- You need to pass a value to multiple functions
- You are building data structures or concurrent systems that share mutable state
- You want to understand why your code compiles when you expected it not to

---

## How Ownership is Inferred Automatically

Every heap-allocated value has exactly one owner. The compiler tracks ownership through assignments and function calls.

### Rule 1: Assignment transfers ownership

```python
a = make_data()    # a owns the data
b = a              # b now owns the data — a is no longer valid
print(a.len())     # M-2: a was moved into b
```

### Rule 2: Passing to a function that "consumes" the value is a move

```python
def consume(data: List[int]) -> void:
    # data is freed when this function returns
    print(len(data))

mut items = [1, 2, 3]
consume(items)          # items moved into consume
print(len(items))       # M-2: items was moved
```

### Rule 3: Passing to a function that only reads is a borrow

```python
def read_only(data: List[int]) -> int:
    return len(data)    # does not hold onto data

mut items = [1, 2, 3]
mut n = read_only(items)    # borrow — items still valid afterward
print(len(items))           # OK
```

### How the compiler decides: move vs. borrow

The compiler uses the callee's parameter type and usage to infer:
- If the function stores the parameter in a field or passes it to another consuming function, it is a **move**
- If the function only reads the parameter and does not outlive the call, it is a **borrow**

In practice, for simple functions, the compiler correctly infers borrow semantics for read-only use and move semantics when the value is stored or consumed. When in doubt, annotate explicitly with `borrow` or use `Shared[T]`.

---

## Syntax Reference

### Explicit Move

```python
move x into y    # explicitly transfer ownership from x to y
```

`x` is no longer valid after this line. This is the same as assignment (`y = x`) but makes intent visible in code that handles ownership carefully.

### Clone for Deep Copy

```python
mut copy = clone(original)    # deep copy — original is still valid
```

`clone` allocates a new independent copy of the value. Both the original and the copy are valid owners. Use when you need to pass the same data to two different consuming functions.

### Borrow annotation

```python
def read_only(borrow data: List[int]) -> int:
    return len(data)
```

The `borrow` keyword on a parameter explicitly declares that this function borrows (does not consume) the value. The compiler verifies that the parameter is not stored or moved inside the function.

---

## Shared[T] — Reference-Counted Ownership

`Shared[T]` wraps a value in a reference-counted container. All copies of a `Shared[T]` point to the same underlying value. The value is freed when the last `Shared[T]` goes out of scope.

### When to use Shared[T]

- Multiple functions or data structures need to own the same value simultaneously
- You need to pass the same value across thread boundaries (see [06 — Sendable](06_sendable.md))
- The value has a non-trivial lifetime that does not fit a simple owner → callee flow

### Creating and using Shared[T]

```python
mut shared_list = Shared[List[int]].init([1, 2, 3])

# Both a and b point to the same list
mut a = shared_list
mut b = shared_list    # no M-2: Shared[T] is reference-counted

print(len(a.get()))    # 3
print(len(b.get()))    # 3 — same underlying list

a.get().append(4)
print(len(b.get()))    # 4 — b sees the mutation
```

`.init(value)` moves `value` into the shared container. `.get()` returns a borrow of the inner value.

### Shared[T] and threads

`Shared[T]` implements `Sendable` — it can cross thread boundaries safely. The reference count is updated atomically.

```python
mut shared = Shared[List[int]].init([1, 2, 3])

task_group:
    spawn process_a(shared)    # OK: Shared[T] is Sendable
    spawn process_b(shared)    # OK: second copy of the Shared reference
```

Note: `Shared[T]` alone does not protect against data races on the inner value. If two threads mutate it simultaneously, use `Mutex[T]` or `RwLock[T]` instead.

---

## Patterns

### Pattern: Share read-only data across threads

```python
# Configuration loaded once, read by many threads
mut config = Shared[Config].init(load_config())

task_group:
    spawn worker_a(config)
    spawn worker_b(config)
    spawn worker_c(config)
```

### Pattern: Shared mutable state with Mutex

```python
mut counter = Shared[Mutex[int]].init(Mutex.init(0))

def increment(c: Shared[Mutex[int]]) -> void:
    mut guard = c.get().lock()
    guard.set(guard.get() + 1)

task_group:
    spawn increment(counter)
    spawn increment(counter)
```

### Pattern: Passing ownership explicitly

```python
# When you want to be explicit that ownership transfers:
def process_and_free(move data: List[int]) -> int:
    return len(data)
    # data freed here

mut items = [1, 2, 3]
move items into raw_data
mut n = process_and_free(raw_data)
# items and raw_data are both invalid after this
```

### Pattern: Clone before passing to multiple consumers

```python
def send_to_both(data: List[int]) -> void:
    mut copy = clone(data)
    consumer_a(data)      # data moved here
    consumer_b(copy)      # copy moved here
```

---

## Common Mistakes

**Storing a borrowed value in a class field.** If a function borrows a parameter but then stores it in a field, the compiler will either reject this (if it can detect it statically) or produce a dangling reference at runtime. Use `Shared[T]` or clone.

```python
class Cache:
    pub data: List[int]    # owned

extend Cache:
    # WRONG: borrowing data but storing it — data may be freed by caller
    pub def set_borrow(self, borrow data: List[int]) -> void:
        self.data = data    # M-2/M-4 depending on context

    # RIGHT: take ownership
    pub def set_owned(self, data: List[int]) -> void:
        self.data = data    # data moved into self — correct
```

**Using a value after passing it to Shared.init().** `Shared.init(value)` *moves* the value in. The original binding is no longer valid.

```python
mut items = [1, 2, 3]
mut shared = Shared[List[int]].init(items)
print(len(items))    # M-2: items was moved into shared
```

**Reaching for clone() before understanding the ownership flow.** `clone()` allocates memory. Many apparent "need to clone" situations are actually "need to reorder the calls so the borrow doesn't conflict."

---

## Best Practices

- **Prefer ownership over borrowing for class fields.** A class that owns its data is simpler than one that borrows — no lifetime constraints to reason about.
- **Use `Shared[T]` for shared state; avoid raw shared mutable globals.** Raw global mutable state has no protection and causes data races in concurrent code.
- **Use `clone()` sparingly.** Each clone is a memory allocation. Measure before adding clones in hot paths.
- **Use `Mutex[T]` or `RwLock[T]` inside `Shared[T]` for mutable shared state across threads.** `Shared[T]` alone is not a lock.
- **The compiler's move detection is conservative.** If it emits M-2 and you believe it's a false positive, re-examine the data flow — in practice, the compiler is correct, and the fix is a structural change (reorder calls, use Shared, or clone).

---

See also:
- [13 — Memory and Ownership](../13_memory_and_ownership.md)
- [16 — Concurrency](../16_concurrency.md)
- [01 — Lifetimes](01_lifetimes.md)
- [06 — Sendable](06_sendable.md)
- [Compiler Error M-2](../19_compiler_errors.md#m-2-use-after-move)
