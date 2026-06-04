# Advanced — Sendable

> This is an advanced topic. Core Tauraro development does not require understanding this. See the [Advanced Docs Index](README.md).

---

## Overview

Tauraro enforces thread safety at compile time using the `Sendable` interface. A type is `Sendable` if it is safe to pass across thread boundaries. The compiler rejects any attempt to send a non-`Sendable` type to `spawn`, through a `Chan[T]`, or to a `task_group` worker.

This catches entire classes of concurrency bugs — data races, use-after-free from threads, sharing of non-thread-safe resources — at compile time, before the program runs.

---

## When You Need This

- You are writing code that uses `spawn`, `task_group`, or `Chan[T]` and getting [T-1] Sendable errors
- You are building a type that needs to cross thread boundaries and want to declare it safe
- You are auditing concurrent code for thread safety
- You want to understand why `Shared[T]` exists and when to use it over plain ownership

---

## Syntax Reference

### Checking if a type is Sendable

The compiler checks automatically. You don't query it — it tells you via [T-1] errors.

### Declaring a class as Sendable

```python
class MyClass implements Sendable:
    pub data: int     # all fields must be Sendable or primitives
```

By declaring `implements Sendable`, you take responsibility for ensuring the class is genuinely thread-safe. The compiler checks that all fields are also `Sendable`.

### Passing across a thread boundary

```python
# Any value passed to spawn must be Sendable
task_group:
    spawn my_func(sendable_value)

# Any type used as Chan[T] must be Sendable
mut ch: Chan[MyClass] = Chan[MyClass].init(16)
```

---

## Built-in Sendable Types

| Type | Sendable | Notes |
|------|----------|-------|
| `int`, `float`, `bool`, `char` | Yes | Primitive — copied on send |
| `str` | Yes | Immutable — safe to share |
| `Shared[T]` | Yes | Reference-counted — atomic refcount |
| `Mutex[T]` | Yes | Lock-protected mutation |
| `RwLock[T]` | Yes | Read-write lock |
| `Atomic[T]` | Yes | Lock-free atomic operations |
| `Chan[T]` | Yes | Thread-safe by design |
| `Pointer[T]` | Yes (but unsafe) | Raw pointer — inherently unsafe, you take responsibility |
| `List[T]`, `Dict`, plain classes | **No** | Not thread-safe — use `Shared[T]` to wrap |

---

## Error Codes

### [T-1] Type is not Sendable

```
error [T-1]: type 'List[int]' is not Sendable — cannot pass to spawn
```

**Cause:** You attempted to pass a `List[T]`, unprotected class, or other non-Sendable type to a thread boundary.

**How it works:**

```python
mut items = [1, 2, 3]

# WRONG: List[int] is not Sendable
task_group:
    spawn process(items)    # T-1

# RIGHT: wrap in Shared
mut shared_items = Shared[List[int]].init(items)
task_group:
    spawn process(shared_items)    # OK: Shared[T] is Sendable
```

If you only need to read the list from multiple threads:

```python
# Pass by value (copy) if the list is small:
task_group:
    spawn process(clone(items))    # each thread gets its own copy
```

---

### [T-2] Race Condition Detected

```
error [T-2]: race condition — 'counter' is mutated by multiple threads without synchronization
```

**Cause:** Two or more concurrent operations read and write the same mutable value without a lock.

**How it works:**

```python
# WRONG: data race
mut counter = 0

def increment() -> void:
    counter = counter + 1    # T-2: no synchronization

task_group:
    spawn increment()
    spawn increment()

# RIGHT: use Atomic or Mutex
mut counter = Atomic[int].init(0)

def increment_safe(c: Atomic[int]) -> void:
    c.fetch_add(1)

task_group:
    spawn increment_safe(counter)
    spawn increment_safe(counter)
```

---

### [T-3] Potential Deadlock

```
error [T-3]: potential deadlock — holding 'lock_a' while awaiting 'lock_b'
```

**Cause:** The compiler detected a code path where a thread holds one lock while waiting to acquire another, which can deadlock if another thread acquires them in the opposite order.

**How it works:**

```python
mut lock_a = Mutex[int].init(0)
mut lock_b = Mutex[int].init(0)

def thread_1() -> void:
    mut g_a = lock_a.lock()    # acquires lock_a
    mut g_b = lock_b.lock()    # T-3: holding lock_a, acquiring lock_b

def thread_2() -> void:
    mut g_b = lock_b.lock()    # acquires lock_b
    mut g_a = lock_a.lock()    # deadlock if thread_1 already holds lock_a

# RIGHT: always acquire locks in the same order
def thread_1() -> void:
    mut g_a = lock_a.lock()
    mut g_b = lock_b.lock()
    # ... both operations ...

def thread_2() -> void:
    mut g_a = lock_a.lock()    # same order as thread_1
    mut g_b = lock_b.lock()
```

---

## Patterns

### Pattern: Shared read-only data

For data loaded once and read by many threads:

```python
mut config = Shared[Config].init(load_config())

task_group:
    spawn worker_a(config)
    spawn worker_b(config)
    spawn worker_c(config)
```

`Shared[T]` provides shared ownership with atomic reference counting. The config is freed when the last thread drops its reference.

---

### Pattern: Shared mutable counter with Atomic

For a simple counter that many threads increment:

```python
mut total = Atomic[int].init(0)

def worker(data: List[int], counter: Atomic[int]) -> void:
    for item in data:
        counter.fetch_add(1)

task_group:
    spawn worker(slice_a, total)
    spawn worker(slice_b, total)

print(total.load())
```

`Atomic[T]` supports: `load()`, `store(val)`, `fetch_add(n)`, `fetch_sub(n)`, `compare_exchange(expected, new)`.

---

### Pattern: Protected mutable state with Mutex

For complex mutable state that needs atomic read-modify-write:

```python
class Stats:
    pub hits:   int
    pub misses: int

extend Stats:
    pub def init() -> Stats:
        mut s = Stats()
        s.hits   = 0
        s.misses = 0
        return s

mut stats_lock = Shared[Mutex[Stats]].init(Mutex.init(Stats.init()))

def record_hit(lock: Shared[Mutex[Stats]]) -> void:
    mut guard = lock.get().lock()
    guard.get().hits = guard.get().hits + 1

def record_miss(lock: Shared[Mutex[Stats]]) -> void:
    mut guard = lock.get().lock()
    guard.get().misses = guard.get().misses + 1
```

The `Mutex[T]` guard is released automatically when `guard` goes out of scope (end of function or block).

---

### Pattern: StructuredGroup — collect all errors

`StructuredGroup` spawns multiple tasks and collects all errors, not just the first:

```python
from std.async import StructuredGroup

def main():
    mut group = StructuredGroup.init()

    group.spawn(lambda: fetch_users()?)
    group.spawn(lambda: fetch_products()?)
    group.spawn(lambda: fetch_orders()?)

    mut errors = group.wait()    # waits for all, collects all errors
    if len(errors) > 0:
        for e in errors:
            print(f"error: {e}")
```

Use `StructuredGroup` when all tasks must complete (or fail) before proceeding and you want to see every error, not just the first one.

---

### Pattern: Implementing Sendable on a custom class

If you build a class that is genuinely thread-safe (e.g., it wraps an OS primitive or uses internal locking), declare it `Sendable`:

```python
class ThreadSafeQueue implements Sendable:
    pub _lock:  Mutex[int]    # internal lock — also Sendable
    pub _items: List[str]     # protected by _lock

extend ThreadSafeQueue:
    pub def init() -> ThreadSafeQueue:
        mut q = ThreadSafeQueue()
        q._lock  = Mutex.init(0)
        q._items = []
        return q

    pub def push(self, item: str) -> void:
        mut guard = self._lock.lock()
        self._items.append(item)

    pub def pop(self) -> Option[str]:
        mut guard = self._lock.lock()
        if len(self._items) == 0: return Option.none()
        mut val = self._items[0]
        self._items = self._items[1:]    # remove first
        return Option.some(val)
```

By declaring `implements Sendable`, you assert this class is thread-safe. The compiler verifies that all fields (`_lock`, `_items`) are themselves `Sendable`.

---

## Common Mistakes

**Wrapping a `List[T]` in `Shared[T]` without a lock.** `Shared[T]` provides safe *shared ownership* (the reference count is atomic) but does not protect the inner value from concurrent mutation. Two threads mutating a `Shared[List[int]]` simultaneously is still a data race. Use `Shared[Mutex[List[int]]]` if you need mutation.

**Sending raw class instances to threads.** Plain classes are not `Sendable`. Wrapping them in `Shared[T]` makes the reference safe to copy across threads; the class itself still needs synchronization if mutated.

**Holding locks across `async` suspension points.** An `async def` that holds a `Mutex` guard and then `await`s suspends the task while holding the lock. Another task may try to acquire the same lock and deadlock. Release the lock before awaiting.

**Declaring `implements Sendable` without making the class actually safe.** The compiler checks that all fields are `Sendable` but cannot verify your algorithmic invariants. If you declare `Sendable` on a class with internal mutable state that is not locked, data races will occur at runtime.

---

## Best Practices

- **Default to owned data per thread, not shared state.** The safest concurrent code is code where each thread owns all its data. Shared state is only needed when threads genuinely need to communicate.
- **Use `Chan[T]` for communication between threads.** Instead of sharing mutable state, send values through a channel. This naturally serializes access and avoids races.
- **Use `Atomic[T]` for simple counters and flags.** Lock-free for the common case. Use `Mutex[T]` only when the update involves reading, computing, and writing multiple values atomically.
- **Use `StructuredGroup` for parallel work that must all complete.** `task_group` fires-and-forgets; `StructuredGroup` collects and surfaces every error.
- **Minimize the scope of lock guards.** Lock as late as possible, unlock as early as possible. Don't do I/O, computation, or allocation while holding a lock.

---

See also:
- [16 — Concurrency](../16_concurrency.md)
- [03 — Channel Select](03_channel_select.md)
- [02 — Advanced Ownership](02_advanced_ownership.md)
- [Compiler Error T-1](../19_compiler_errors.md#t-1-no-implicit-type-coercion) (Sendable errors use the T-series codes)
