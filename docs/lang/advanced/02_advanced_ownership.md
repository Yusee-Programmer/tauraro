# Advanced — Advanced Ownership

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

[Chapter 13](../13_memory_and_ownership.md) introduces Tauraro's ownership model. This doc goes deeper: how the compiler infers ownership automatically, when a pass is a move vs. a borrow, when to reach for `Shared[T]`, and how the explicit `move` syntax works.

---

## When You Need This

- You are getting [M-1] (use after move) errors and don't understand why
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
print(a.len())     # M-1: a was moved into b
```

### Rule 2: Passing to a function is always a borrow

A call never moves its arguments - even a callee that stores the parameter in a
field or a collection only takes its own reference-counted reference; the
caller's variable stays valid.

```python
def consume(data: List[int]) -> void:
    print(len(data))

mut items = [1, 2, 3]
consume(items)          # a borrow for the duration of the call
print(len(items))       # OK
```

### Rule 3: Reading through a function is a borrow too

```python
def read_only(data: List[int]) -> int:
    return len(data)    # does not hold onto data

mut items = [1, 2, 3]
mut n = read_only(items)    # borrow — items still valid afterward
print(len(items))           # OK
```

### How the compiler decides: move vs. borrow

Only a *binding transfer* (`b = a`, `mut b = a`) moves. A callee that keeps its
parameter (stores it in a field/collection, or hands it to another thread) takes
its own reference, so no annotation is needed at the call site. Use `Shared[T]`
when several owners must outlive one another across threads.

---

## Syntax Reference

### Clone for Deep Copy

```python
mut copy = clone(original)    # deep copy — original is still valid
```

`clone` allocates a new independent copy of the value. Both the original and the copy are valid owners. Use when you need to pass the same data to two different consuming functions.

### Borrow annotation

```python
def read_only(data: ref List[int]) -> int:
    return len(data)
```

A `ref T` parameter is a *shared* borrow (read-only, checked by `[B-3]` under `--strict`); `mut ref T` is an exclusive one.

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
# A binding transfer makes the hand-off explicit:
def process(data: List[int]) -> int:
    return len(data)

mut items = [1, 2, 3]
raw_data = items            # items is moved into raw_data
mut n = process(raw_data)
# items is invalid after the transfer
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

**Assuming a call consumes its argument.** Passing a value to a function, method, `append`, a field store or `Shared.init(...)` never invalidates the caller's variable: the receiver takes its own reference-counted reference. Only a binding transfer (`b = a`) moves.

```python
class Cache:
    pub data: List[int]

extend Cache:
    pub def set(self, data: List[int]) -> void:
        self.data = data    # the field takes its own reference; the caller's `data` stays valid
```

**Using a value after a binding transfer.**

```python
mut items = [1, 2, 3]
mut other = items
print(len(items))    # M-1: items was moved into other
```

**Reaching for clone() before understanding the ownership flow.** `clone()` allocates memory. Many apparent "need to clone" situations are actually "need to reorder the calls so the borrow doesn't conflict."

---

## Best Practices

- **Prefer ownership over borrowing for class fields.** A class that owns its data is simpler than one that borrows — no lifetime constraints to reason about.
- **Use `Shared[T]` for shared state; avoid raw shared mutable globals.** Raw global mutable state has no protection and causes data races in concurrent code.
- **Use `clone()` sparingly.** Each clone is a memory allocation. Measure before adding clones in hot paths.
- **Use `Mutex[T]` or `RwLock[T]` inside `Shared[T]` for mutable shared state across threads.** `Shared[T]` alone is not a lock.
- **The compiler's move detection is conservative.** If it emits M-1 and you believe it's a false positive, re-examine the data flow — in practice, the compiler is correct, and the fix is a structural change (reorder calls, use Shared, or clone).

---

See also:
- [13 — Memory and Ownership](../13_memory_and_ownership.md)
- [16 — Concurrency](../16_concurrency.md)
- [01 — Lifetimes](01_lifetimes.md)
- [06 — Sendable](06_sendable.md)
- [Compiler Error M-1](../19_compiler_errors.md#m-1-use-after-move)
