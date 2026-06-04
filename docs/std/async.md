# std.async — Concurrency Primitives

All concurrency primitives are backed by real OS-level threading (Win32 on Windows, pthreads on POSIX). Zero fake synchronous implementations.

## Quick Import

```python
# Import individual types
from std.async.thread  import Thread, Atomic, ThreadLocal
from std.async.mutex   import Mutex, RWLock
from std.async.send    import Sendable

# Import everything at once
from std.async import Thread, Atomic, ThreadLocal, Mutex, RWLock, Sendable
from std.async import Task, Future, Channel, TaskGroup
from std.async import Semaphore, WaitGroup, Barrier, Once, Timer, Ticker
```

---

## Sendable — Thread-Safety Marker

**When**: You need to declare a user class is safe to cross thread boundaries.
**Why**: The compiler checks all `spawn`, `Thread.spawn`, `pool.spawn`, and `await_all` call sites — if an argument's type is not `Sendable`, it is a `[T-1]` compile error.

```python
from std.async.send import Sendable
```

### Usage

```python
from std.async.send   import Sendable
from std.async.thread import Atomic

class SafeCounter implements Sendable:
    pub value: Atomic[int]

extend SafeCounter:
    pub def init() -> SafeCounter:
        mut c = SafeCounter()
        c.value = Atomic.new(0)
        return c
```

**Built-in Sendable types** (always pass compiler check):
- Primitives: `int`, `float`, `bool`, `str`, `char`
- `Atomic[T]`, `Mutex[T]`, `RwLock[T]`, `Chan[T]`
- `Thread`, `ThreadPool`, `ThreadLocal[T]`

**Not Sendable** (compiler rejects at spawn sites):
- `List[T]`, `Vec[T]`, `Dict`, `Map` — no internal synchronization
- Any user class that does not declare `implements Sendable`

---

## Thread

**When**: You need a joinable OS thread with explicit lifecycle control.
**Why**: Unlike `spawn` (fire-and-forget), `Thread.spawn` returns a handle you can `join()` or `detach()`.

```python
from std.async.thread import Thread
```

### Methods

| Method | Description |
|--------|-------------|
| `Thread.spawn(fn, arg)` | Start thread running `fn(arg)`, return joinable handle |
| `t.join()` | Block until the thread finishes |
| `t.detach()` | Detach — thread runs independently, no join needed |
| `t.free()` | Free the thread handle |
| `Thread.sleep(ms)` | Sleep the calling thread for `ms` milliseconds |
| `Thread.id()` | Return the calling thread's OS ID |

### Example

```python
from std.async.thread import Thread

def work(ms: int) -> void:
    Thread.sleep(ms)
    print(f"  done after {ms}ms")

def main():
    mut t1: Thread = Thread.spawn(work, 100)
    mut t2: Thread = Thread.spawn(work, 200)
    t1.join()
    t2.join()
    print(f"main thread id: {Thread.id()}")
```

---

## Atomic[T] — Lock-Free Integer

**When**: You need a shared counter or flag updated by multiple threads without locking.
**Why**: Backed by C11 `_Atomic long long` — sequentially consistent, zero OS overhead.

```python
from std.async.thread import Atomic
```

### Methods

| Method | Description |
|--------|-------------|
| `Atomic.new(v)` | Create atomic initialized to `v` |
| `a.load()` | Read current value |
| `a.store(v)` | Write value |
| `a.add(v)` | Add `v`, returns new value |
| `a.sub(v)` | Subtract `v`, returns new value |
| `a.swap(v)` | Exchange — stores `v`, returns old value |
| `a.cas(expected, desired)` | Compare-and-swap — returns `true` if swap happened |
| `a.increment()` | Shorthand for `a.add(1)` |
| `a.decrement()` | Shorthand for `a.sub(1)` |
| `a.free()` | Free the atomic |

### Example

```python
from std.async.thread import Thread, Atomic

def inc(counter: Atomic[int]) -> void:
    mut i = 0
    while i < 10000:
        counter.add(1)
        i += 1

def main():
    mut c: Atomic[int] = Atomic.new(0)
    mut t1: Thread = Thread.spawn(inc, c)
    mut t2: Thread = Thread.spawn(inc, c)
    t1.join()
    t2.join()
    print(f"total: {c.load()}")    # 20000
    c.free()
```

---

## ThreadLocal[T] — Per-Thread Storage

**When**: Each thread needs its own independent copy of a value.
**Why**: Changes in one thread are invisible to all others — no synchronization needed.

```python
from std.async.thread import ThreadLocal
```

### Methods

| Method | Description |
|--------|-------------|
| `ThreadLocal.new(v)` | Create thread-local, new threads start with `v` |
| `tl.get()` | Read the calling thread's value |
| `tl.set(v)` | Write the calling thread's value |
| `tl.free()` | Free the storage slot |

### Example

```python
from std.async.thread import Thread, ThreadLocal

def worker(tl: ThreadLocal[int]) -> void:
    tl.set(999)
    print(f"  worker sees: {tl.get()}")    # 999

def main():
    mut tl: ThreadLocal[int] = ThreadLocal.new(0)
    tl.set(42)
    mut t: Thread = Thread.spawn(worker, tl)
    t.join()
    print(f"main still sees: {tl.get()}")    # 42
    tl.free()
```

---

## Mutex[T] — Thread-Safe Guarded Value

**When**: Multiple threads need exclusive access to a shared mutable value.
**Why**: The value is always guarded — you cannot read or write without holding the lock.

```python
from std.async.mutex import Mutex
```

### Methods

| Method | Description |
|--------|-------------|
| `Mutex.init(v)` | Create mutex wrapping initial value `v` |
| `m.get()` | Acquire lock and return current value |
| `m.set(v)` | Store new value and release lock |
| `m.unlock()` | Release lock without storing (after `get()` if no update) |

**RAII auto-unlock:** Calling `m.unlock()` is optional. The compiler emits a cleanup guard that releases the lock when the binding from `m.get()` goes out of scope.

**Rule:** Every `get()` must be followed by exactly one `set()` or `unlock()`.

### Example

```python
from std.async.mutex import Mutex

def increment(m: Mutex[int]) -> void:
    mut v = m.get()
    m.set(v + 1)    # releases lock

def main():
    mut counter: Mutex[int] = Mutex.init(0)
    task_group:
        spawn increment(counter)
        spawn increment(counter)
        spawn increment(counter)
    mut final = counter.get()
    counter.unlock()
    print(f"final: {final}")    # 3
```

---

## RWLock[T] — Reader-Writer Lock

**When**: Multiple threads read frequently but write rarely.
**Why**: Multiple concurrent readers are safe; a writer gets exclusive access.

```python
from std.async.mutex import RWLock
```

### Methods

| Method | Description |
|--------|-------------|
| `RwLock.init(v)` | Create rwlock wrapping initial value `v` |
| `rw.read()` | Acquire read lock, return current value |
| `rw.read_unlock()` | Release read lock |
| `rw.write()` | Acquire write lock, return current value |
| `rw.write_set(v)` | Store new value and release write lock |

**RAII:** `read()` and `write()` bindings are auto-released when they go out of scope (same as Mutex).

### Example

```python
from std.async.mutex import RWLock

def reader(rw: RwLock[int]) -> void:
    mut v = rw.read()
    rw.read_unlock()
    print(f"  read: {v}")

def main():
    mut rw: RwLock[int] = RwLock.init(100)
    task_group:
        spawn reader(rw)
        spawn reader(rw)    # concurrent reads OK
    mut old = rw.write()
    rw.write_set(200)
    print(f"  updated {old} -> 200")
```

---

## Channel / Task / Future / TaskGroup

These higher-level primitives provide named tasks, futures, and group coordination on top of the OS threading layer.

```python
from std.async.task      import Task, Future
from std.async.channel   import Channel
from std.async.group     import TaskGroup
from std.async.semaphore import Semaphore
from std.async.waitgroup import WaitGroup
from std.async.barrier   import Barrier
from std.async.once      import Once
from std.async.timer     import Timer, Ticker
```

### Task

| Method | Description |
|--------|-------------|
| `Task.new(name)` | Create a task in pending state |
| `t.complete(result)` | Mark done with an integer result |
| `t.fail(msg)` | Mark done with an error message |
| `t.cancel()` | Cancel; wakes blocked `await_()` |
| `t.await_()` | Block until done, return result |
| `t.await_timeout(ms)` | `true` if done within deadline |
| `t.is_done()` | |
| `t.has_error()` | |
| `t.get_error()` | Error message or `""` |
| `t.free()` | |

### Future

| Method | Description |
|--------|-------------|
| `Future.new()` | Create a future in pending state |
| `f.resolve(value)` | Fulfill with a value |
| `f.reject(msg)` | Fail with an error message |
| `f.await_()` | Block until resolved |
| `f.await_timeout(ms)` | `true` if resolved within deadline |
| `f.free()` | |

### Channel

Bounded MPMC ring buffer channel (distinct from the built-in `Chan[T]`).

| Method | Description |
|--------|-------------|
| `Channel.new(cap)` | Buffered channel with capacity `cap` |
| `ch.send(val)` | Send; blocks if full |
| `ch.recv()` | Receive; blocks if empty |
| `ch.try_send(val)` | Non-blocking send, returns `bool` |
| `ch.try_recv()` | Non-blocking receive, returns `0` if empty |
| `ch.send_timeout(val, ms)` | Send with deadline |
| `ch.recv_timeout(ms)` | Receive with deadline |
| `ch.close()` | Mark closed; wakes blocked receivers |
| `ch.len()` | Current buffered count |
| `ch.free()` | |

### TaskGroup

| Method | Description |
|--------|-------------|
| `TaskGroup.new()` | Empty group |
| `tg.add(name)` | Create and register a task |
| `tg.wait_all()` | Block until all tasks done |
| `tg.wait_all_timeout(ms)` | `true` if all done within deadline |
| `tg.cancel_all()` | Cancel all pending tasks |
| `tg.pending_count()` | Tasks not yet done |
| `tg.free()` | |

### Semaphore

```python
mut sem = Semaphore.new(3, 3)    # 3 slots
sem.acquire()
# critical section
sem.release()
sem.free()
```

### WaitGroup

```python
mut wg = WaitGroup.new()
wg.add(2)
# spawn 2 workers that each call wg.done()
wg.wait()
wg.free()
```

### Barrier

```python
mut bar = Barrier.new(3)    # release when 3 threads arrive
bar.wait()                  # each thread calls this
bar.free()
```

### Once

```python
mut once = Once.new()
mut first = once.do_once()    # true
mut again = once.do_once()    # false
once.free()
```

### Timer / Ticker

```python
from std.async.timer   import Timer
from std.async.channel import Channel

mut sig = Channel.new(1)
mut t = Timer.new(500, sig)    # fire after 500ms
mut _ = sig.recv()             # block until tick
t.stop()
sig.free()
```
