# 16 — Concurrency

---

## Concurrency Primitives Overview

| Primitive | Use when |
|-----------|---------|
| `async def` / `await` | I/O pipelines, logical async, sequential awaiting |
| `spawn f(args)` | Fire-and-forget detached OS thread |
| `task_group:` | Structured concurrency — wait for a batch of spawns |
| `await_all(f1(), f2(), ...)` | Run multiple calls in parallel, wait for all |
| `await_timeout(fn(), ms)` | Parallel call with a millisecond deadline |
| `Chan[T]` | Producer-consumer pipelines between threads |
| `Mutex[T]` | Thread-safe guarded value with exclusive lock |
| `RwLock[T]` | Multiple readers or single writer |
| `ThreadPool` | Fixed worker-pool for dispatching many short jobs |
| `Thread` | Explicit joinable OS thread with full lifecycle control |
| `Atomic[T]` | Lock-free atomic integer operations |
| `ThreadLocal[T]` | Per-thread storage — each thread sees its own value |
| `Sendable` | Compile-time marker: type is safe to cross thread boundaries |
| `StructuredGroup` | Fault-tolerant batch — panicked threads are caught, not fatal |
| `IOPoll` / `EventLoop` | Non-blocking event-driven I/O with epoll/IOCP/kqueue |

---

## Cooperative: async / await

```python
async def fetch(id: int) -> str:
    return f"item-{id}"

async def process(n: int) -> int:
    mut data = await fetch(n)
    return len(data)

async def main():
    mut r1 = await process(1)
    mut r2 = await process(42)
    print(f"process(1)  = {r1}")    # 6
    print(f"process(42) = {r2}")    # 7
```

`await f(args)` launches `f` on an OS thread, blocks the caller until it completes, and returns the result. It enables a clear sequential pipeline style while running work on dedicated threads.

Rules:
- `async def` can `await` other `async def` functions
- A non-async `def` can call `async def` without `await` — treated as a normal call
- `await` on a non-async function is a no-op pass-through

---

## Preemptive: spawn

`spawn f(arg)` starts a detached OS thread — fire-and-forget:

```python
def worker(id: int) -> void:
    print(f"  worker {id} running")

def main():
    spawn worker(1)
    spawn worker(2)
    print("main continues — workers may still be running")
```

**Multi-arg spawn works natively:**

```python
def compute2(a: int, b: int) -> void:
    print(f"  compute2({a}, {b}) = {a * b}")

def main():
    spawn compute2(3, 4)    # works with any number of arguments
    spawn compute2(5, 6)
```

The compiler automatically heap-packs the arguments into an array and generates a wrapper function.

---

## Structured Concurrency: task_group:

`task_group:` waits for all threads spawned inside it before continuing. The limit is unlimited — you can spawn as many threads as your OS allows:

```python
def compute(n: int) -> void:
    print(f"  compute({n}) = {n * n}")

def main():
    task_group:
        spawn compute(4)
        spawn compute(7)
        spawn compute(12)
        spawn compute(20)
        spawn compute(50)    # no limit
    print("all compute() finished")
```

---

## await_all — Parallel Concurrent Calls

`await_all(f1(args), f2(args), ...)` runs all calls in parallel via `task_group:` internally and waits for all to complete:

```python
def heavy(n: int) -> void:
    mut s = 0
    mut i = 0
    while i < n:
        s += i
        i += 1
    print(f"  heavy({n}) sum = {s}")

def main():
    await_all(heavy(1000), heavy(2000), heavy(3000))
    print("all finished (ran in parallel)")
```

---

## Chan[T] — Typed Buffered Channel

`Chan[T]` is a thread-safe buffered channel for producer-consumer pipelines:

```python
def producer(ch: Chan[int]) -> void:
    mut i = 0
    while i < 5:
        ch.send(i * 10)
        i += 1
    ch.close()

def consumer(ch: Chan[int]) -> void:
    mut total = 0
    for v in ch:          # blocks until item available or channel closed
        total += v
    print(f"  total = {total}")    # 0+10+20+30+40 = 100

def main():
    mut ch: Chan[int] = Chan.init(10)    # buffer size 10
    task_group:
        spawn producer(ch)
        spawn consumer(ch)
```

### Chan[T] API

| Method | Description |
|--------|-------------|
| `Chan.init(n)` | Create buffered channel with capacity `n` |
| `ch.send(v)` | Send value; blocks if buffer full |
| `ch.recv()` | Receive value; blocks if buffer empty |
| `ch.close()` | Signal that no more values will be sent |
| `for v in ch:` | Iterate until channel closed and drained |

---

## Mutex[T] — Thread-Safe Guarded Value

`Mutex[T]` wraps a value and a mutex together. The only way to access the value is through the lock:

```python
def safe_inc(m: Mutex[int]) -> void:
    mut v = m.get()     # acquires lock, returns current value
    m.set(v + 1)        # stores new value, releases lock

def main():
    mut counter: Mutex[int] = Mutex.init(0)
    task_group:
        spawn safe_inc(counter)
        spawn safe_inc(counter)
        spawn safe_inc(counter)
    mut final = counter.get()
    counter.unlock()
    print(f"counter after 3 threads: {final}")    # 3
```

### Mutex[T] API

| Method | Description |
|--------|-------------|
| `Mutex.init(v)` | Create mutex wrapping initial value `v` |
| `m.get()` | Acquire lock and return current value |
| `m.set(v)` | Store new value and release lock |
| `m.unlock()` | Release lock without storing (use after `get()` if no update needed) |

**Rule:** Every `get()` must be followed by either `set()` or `unlock()`.

---

## RwLock[T] — Reader-Writer Lock

`RwLock[T]` allows multiple concurrent readers or one exclusive writer:

```python
def read_config(rw: RwLock[int]) -> void:
    mut v = rw.read()        # acquires read lock
    rw.read_unlock()         # release read lock
    print(f"  config: {v}")

def main():
    mut config: RwLock[int] = RwLock.init(42)
    task_group:
        spawn read_config(config)
        spawn read_config(config)    # concurrent reads are safe

    mut old = config.write()         # acquires write lock
    config.write_set(100)            # store and release write lock
    print(f"  updated from {old} to 100")
```

### RwLock[T] API

| Method | Description |
|--------|-------------|
| `RwLock.init(v)` | Create rwlock wrapping initial value `v` |
| `rw.read()` | Acquire read lock, return current value |
| `rw.read_unlock()` | Release read lock |
| `rw.write()` | Acquire write lock, return current value |
| `rw.write_set(v)` | Store new value and release write lock |

---

## ThreadPool — Fixed Worker Pool

`ThreadPool` maintains a pool of worker threads that pick up jobs from a queue:

```python
def job(id: int) -> void:
    print(f"  pool job {id} done")

def main():
    mut pool: ThreadPool = ThreadPool.new(4)    # 4 worker threads
    pool.spawn(job, 1)
    pool.spawn(job, 2)
    pool.spawn(job, 3)
    pool.wait()     # wait for all queued jobs to complete
    pool.free()     # shut down workers and free pool
    print("all pool jobs done")
```

### ThreadPool API

| Method | Description |
|--------|-------------|
| `ThreadPool.new(n)` | Create pool with `n` worker threads |
| `ThreadPool.auto()` | Create pool with one thread per CPU core |
| `pool.spawn(fn, arg)` | Dispatch `fn(arg)` to a worker |
| `pool.wait()` | Block until all queued jobs complete |
| `pool.free()` | Shutdown workers and free pool memory |

**When to use ThreadPool vs task_group:**
- `task_group:` — when you have a fixed known set of tasks
- `ThreadPool` — when tasks arrive dynamically or you want to reuse threads across many jobs

---

## Shared Ownership Across Threads

`shared` wraps a class instance in an atomic refcount box, allowing multiple threads to hold a reference to the same object:

```python
class Counter:
    pub value: int

extend Counter:
    pub def init() -> Counter:
        mut c = Counter()
        c.value = 0
        return c
    pub def increment(self) -> void:
        self.value += 1
    pub def get(self) -> int:
        return self.value

def increment_shared(c: Counter) -> void:
    c.increment()

def main():
    mut sc = Counter.init()
    shared s1 = sc
    shared s2 = s1    # same underlying object

    task_group:
        spawn increment_shared(s1)
        spawn increment_shared(s2)
    print(f"count: {s1.get()}")
```

**Warning:** `shared` makes the refcount thread-safe, but the underlying data is NOT protected. For safe mutation use `Mutex[T]` instead.

---

## Thread — Explicit Joinable Thread

`Thread` gives you a joinable handle to an OS thread, unlike `spawn` which is fire-and-forget:

```python
def work(ms: int) -> void:
    Thread.sleep(ms)
    print(f"  done after {ms}ms")

def main():
    mut t1: Thread = Thread.spawn(work, 50)
    mut t2: Thread = Thread.spawn(work, 100)
    t1.join()    # wait for t1 to finish
    t2.join()    # wait for t2 to finish
    print(f"current thread id: {Thread.id()}")
```

### Thread API

| Method | Description |
|--------|-------------|
| `Thread.spawn(fn, arg)` | Start a new thread running `fn(arg)`, returns handle |
| `t.join()` | Wait for thread to finish |
| `t.detach()` | Detach thread (runs independently, no join needed) |
| `t.free()` | Free the thread handle |
| `Thread.sleep(ms)` | Sleep current thread for `ms` milliseconds |
| `Thread.id()` | Return current thread's OS ID |

**Thread vs spawn:** Use `Thread.spawn` when you need to join (wait for a result or ordering). Use `spawn` for fire-and-forget background work.

---

## Atomic[T] — Lock-Free Integer

`Atomic[int]` wraps a value in a C11 `_Atomic` cell. All operations are lock-free and safe to call from multiple threads simultaneously:

```python
def inc_task(counter: Atomic[int]) -> void:
    mut i = 0
    while i < 1000:
        counter.add(1)
        i = i + 1

def main():
    mut counter: Atomic[int] = Atomic.new(0)
    task_group:
        spawn inc_task(counter)
        spawn inc_task(counter)
        spawn inc_task(counter)
    print(f"total: {counter.load()}")    # 3000

    # Compare-and-swap: atomically set to 42 only if current == 0
    mut old = counter.swap(0)            # exchange, returns old value
    mut ok = counter.cas(0, 42)         # ok=true when swap succeeded
    counter.free()
```

### Atomic[T] API

| Method | Description |
|--------|-------------|
| `Atomic.new(v)` | Create atomic initialized to `v` |
| `a.load()` | Read current value |
| `a.store(v)` | Write value |
| `a.add(v)` | Atomic add, returns new value |
| `a.sub(v)` | Atomic subtract, returns new value |
| `a.swap(v)` | Exchange — stores `v`, returns old value |
| `a.cas(expected, desired)` | Compare-and-swap — returns `true` if swap happened |
| `a.increment()` | Shorthand for `a.add(1)` |
| `a.decrement()` | Shorthand for `a.sub(1)` |
| `a.free()` | Free the atomic |

**When to use Atomic vs Mutex:** `Atomic[int]` is faster for counters and flags. Use `Mutex[T]` when you need to guard a complex value or need read-modify-write over multiple fields.

---

## ThreadLocal[T] — Per-Thread Storage

`ThreadLocal[T]` gives each thread its own independent copy of a value. Changes in one thread are invisible to others:

```python
def worker(tl: ThreadLocal[int]) -> void:
    tl.set(999)    # only affects THIS thread's slot
    print(f"  worker sees: {tl.get()}")    # 999

def main():
    mut tl: ThreadLocal[int] = ThreadLocal.new(0)
    tl.set(100)    # main thread's value

    mut t: Thread = Thread.spawn(worker, tl)
    t.join()

    print(f"main still sees: {tl.get()}")    # still 100
    tl.free()
```

### ThreadLocal[T] API

| Method | Description |
|--------|-------------|
| `ThreadLocal.new(v)` | Create thread-local initialized to `v` for new threads |
| `tl.get()` | Read current thread's value |
| `tl.set(v)` | Write current thread's value |
| `tl.free()` | Free the thread-local storage slot |

---

## Mutex Auto-Unlock (RAII)

Calling `m.unlock()` after `m.get()` is optional. The compiler automatically emits a cleanup guard that releases the lock when the binding goes out of scope:

```python
def safe_read(m: Mutex[int]) -> void:
    mut v = m.get()
    # No m.unlock() needed — lock released automatically when v goes out of scope
    print(f"  value = {v}")

def main():
    mut m: Mutex[int] = Mutex.init(0)
    task_group:
        spawn safe_read(m)
        spawn safe_read(m)
    # Explicit unlock is still accepted:
    mut v = m.get()
    m.unlock()
```

The same auto-unlock behavior applies to `RwLock.read()` and `RwLock.write()`.

---

## Sendable — Compile-Time Data Race Prevention

The `Sendable` interface is a **compile-time marker** that declares a type is safe to send across thread boundaries. The compiler enforces it statically at every spawn site.

### Declaring a type Sendable

```python
from std.async.send import Sendable

class SafeStats implements Sendable:
    pub hits:   Atomic[int]
    pub misses: Atomic[int]
```

Rules:
- A class that declares `implements Sendable` must have **only Sendable fields** — the compiler rejects the class definition otherwise.
- Primitive types (`int`, `float`, `bool`, `str`) are always Sendable.
- Built-in concurrency types (`Atomic[T]`, `Mutex[T]`, `RwLock[T]`, `Chan[T]`, `Thread`, `ThreadPool`, `ThreadLocal[T]`) are always Sendable.
- `List[T]`, `Vec[T]`, `Dict`, `Map` are **never Sendable** — they have no internal synchronization.

### Enforcement at spawn sites

```python
class UnsafeStats:
    pub hits: int              # plain int — NOT Sendable

def tally(s: UnsafeStats) -> void:
    s.hits += 1

def main():
    mut s = UnsafeStats()
    spawn tally(s)             # [T-1] compile error — UnsafeStats is not Sendable
    Thread.spawn(tally, s)     # [T-1] compile error
```

The `[T-1]` error fires for `spawn`, `Thread.spawn`, `pool.spawn`, and `await_all` sub-calls. It does **not** fire for plain `await f(...)` (sequential — caller blocks, no concurrent access).

### Full example

```python
from std.async.send   import Sendable
from std.async.thread import Thread, Atomic

class Counter implements Sendable:
    pub value: Atomic[int]

extend Counter:
    pub def init() -> Counter:
        mut c = Counter()
        c.value = Atomic.new(0)
        return c

def increment(c: Counter) -> void:
    mut i = 0
    while i < 1000:
        c.value.add(1)
        i += 1

def main():
    mut c = Counter.init()
    mut t1: Thread = Thread.spawn(increment, c)
    mut t2: Thread = Thread.spawn(increment, c)
    t1.join()
    t2.join()
    print(f"total: {c.value.load()}")    # 2000
    c.value.free()
```

---

## await_timeout — Time-Bounded Await

`await_timeout(fn(args), ms)` calls an `async def` function and waits up to `ms` milliseconds for it to complete. Returns `true` if the function finished in time, `false` if it timed out:

```python
async def slow_fetch(n: int) -> int:
    Thread.sleep(200)
    return n * 2

async def main():
    mut ok = await_timeout(slow_fetch(10), 500)    # true — completes in ~200ms
    if ok:
        print("done in time")
    else:
        print("timed out")

    mut ok2 = await_timeout(slow_fetch(10), 50)    # false — 200ms > 50ms deadline
    if not ok2:
        print("timed out as expected")
```

`await_timeout` is backed by `_TrTaskState` with a refcount-safe design: both the caller and the spawned thread hold a reference, and the last one to finish frees the state.

---

## Thread Safety Summary

| Pattern | Thread safe? | Notes |
|---------|-------------|-------|
| `spawn f(val)` | ✓ | Value is passed by copy |
| `shared x` refcount | ✓ | Refcount is `_Atomic int` |
| `Chan[T]` send/recv | ✓ | Fully synchronized |
| `Mutex[T]` get/set | ✓ | Exclusive lock per access |
| `RwLock[T]` read | ✓ | Multiple concurrent readers |
| `RwLock[T]` write | ✓ | Exclusive write lock |
| `ThreadPool` spawn | ✓ | Queue is synchronized |
| `Atomic[int]` ops | ✓ | C11 lock-free `_Atomic` |
| `ThreadLocal[T]` | ✓ | Per-thread — no sharing |
| `Thread.spawn` | ✓ | Joinable OS thread |
| `shared x.field` concurrent writes | ✗ | Data race — use `Mutex[T]` |
| `List[T]` shared across threads | ✗ | No synchronization |
| `StructuredGroup` spawns | ✓ | Panics caught at thread boundary |
| `IOPoll` (epoll/IOCP/kqueue) | ✓ | Platform-native event poll |
| `TcpStream` non-blocking | ✓ | Returns WOULD_BLOCK instead of blocking |

---

## Common Patterns

### Pipeline with Chan[T]

```python
mut ch: Chan[int] = Chan.init(32)
task_group:
    spawn producer(ch)
    spawn consumer(ch)
```

### Parallel batch with await_all

```python
await_all(process(data1), process(data2), process(data3))
```

### CPU-bound workload with ThreadPool

```python
mut pool: ThreadPool = ThreadPool.auto()
mut i = 0
while i < num_jobs:
    pool.spawn(do_work, i)
    i += 1
pool.wait()
pool.free()
```

---

## StructuredGroup — Structured Concurrency with Panic Recovery

`StructuredGroup` (from `std.async.structured`) is a structured-concurrency container that:
1. Runs each task in its own OS thread (similar to `task_group:`)
2. **Catches thread panics** — a thread that calls `panic(msg)` does not crash the process; the group records the error and continues waiting for other threads
3. Reports which tasks panicked after all threads finish

```tauraro
from std.async.structured import StructuredGroup

def risky(id: int) -> void:
    if id == 2:
        panic("task 2 intentionally failed")
    print(f"  task {id} OK")

def main():
    mut sg = StructuredGroup.init()
    sg.spawn(risky, 1)
    sg.spawn(risky, 2)    # will panic
    sg.spawn(risky, 3)
    sg.wait()             # blocks until all threads finish (panicked or not)

    print(f"panics: {sg.panic_count}")
    if sg.panic_count > 0:
        print("last panic: " + sg.last_panic_msg)
    sg.free()
```

### StructuredGroup API

| Method | Signature | Returns | Description |
|---|---|---|---|
| `StructuredGroup.init` | `() -> StructuredGroup` | `StructuredGroup` | Create an empty group. |
| `spawn` | `(fn, arg)` | `void` | Spawn a thread running `fn(arg)` and add it to the group. |
| `wait` | `()` | `void` | Wait for all threads to finish. Panicked threads are caught, not re-raised. |
| `free` | `()` | `void` | Free the group's resources. |

**Fields:** `panic_count: int`, `last_panic_msg: str`.

**When to use vs `task_group:`** — use `task_group:` when any panic should abort the batch; use `StructuredGroup` when you need fault tolerance (some tasks may fail and that's acceptable).

---

## Thread Panic Recovery

Any OS thread can call `panic(msg)` to signal a fatal error. Normally `panic` terminates the process. When a thread is spawned through `StructuredGroup` (or a `Thread.spawn` handle), the panic is caught at the thread boundary:

```tauraro
from std.async.thread import Thread

def fragile_work(n: int) -> void:
    if n < 0:
        panic("negative input not allowed")
    print(f"  computed {n * n}")

def main():
    mut t = Thread.spawn(fragile_work, -1)
    t.join()
    if t.panicked():
        print("thread panicked: " + t.panic_msg())
    t.free()
```

The per-thread `setjmp` boundary installed by the runtime captures the `longjmp` from `panic` and stores the message in `_TrSpawnResult`, which `.panicked()` and `.panic_msg()` read after join.

**Rule:** A panic inside a thread never terminates the process — it terminates only that thread and records the message for the joiner to inspect.

---

## Async I/O — IOPoll and EventLoop

For non-blocking, event-driven I/O Tauraro provides a cross-platform poll abstraction backed by `epoll` (Linux), `IOCP` (Windows), and `kqueue` (macOS/BSD).

### IOPoll — raw event loop

```tauraro
from std.io.poll import IOPoll, IOEvent

mut poll = IOPoll.init()
poll.add(fd, POLLIN, 0)      # watch fd for readable events (userdata=0)

mut events = poll.wait(200)  # wait up to 200 ms
mut i = 0
while i < events.len:
    mut ev = events.get(i)
    print(f"  fd={ev.fd} events={ev.events} data={ev.userdata}")
    i = i + 1
poll.destroy()
```

### EventLoop — reactor-style

`EventLoop` (from `std.io.event_loop`) wraps `IOPoll` with a run loop that fires `n` iterations:

```tauraro
from std.io.event_loop import EventLoop

mut el = EventLoop.init()
el.add_fd(server_fd, POLLIN, 0)

el.run(5)     # run 5 poll cycles with a 100 ms timeout each
el.free()
```

### Non-blocking TCP

`TcpStream` supports non-blocking sockets via `set_nonblocking()`:

```tauraro
from std.net.tcp import TcpStream

mut s = TcpStream.connect("127.0.0.1", 9000)
s.set_nonblocking()

mut n = s.recv_nb(256)      # returns immediately; n = WOULD_BLOCK (-2) if no data
if n == -2:
    print("no data yet — would block")
else:
    print(f"received {n} bytes")
s.close()
```

| Method | Returns | Description |
|---|---|---|
| `set_nonblocking()` | `bool` | Switch socket to non-blocking mode. |
| `recv_nb(cap)` | `int` | Non-blocking receive. Returns byte count, `0` on disconnect, `-1` on error, `-2` (WOULD_BLOCK) if no data available. |
| `send_nb(data)` | `int` | Non-blocking send. Returns bytes sent, `0` on disconnect, `-1` on error, `-2` if socket buffer full. |

### When to use each

| Pattern | Use when |
|---|---|
| `task_group:` / `ThreadPool` | CPU-bound parallel work; blocking I/O is acceptable |
| `StructuredGroup` | Same, but some threads may panic and you need fault tolerance |
| `IOPoll` / `EventLoop` | Many file descriptors, non-blocking I/O, event-driven server |
| `await_timeout` | Single async call with a hard deadline |
