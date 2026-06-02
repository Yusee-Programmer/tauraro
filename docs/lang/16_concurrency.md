# 16 — Concurrency

---

## Concurrency Primitives Overview

| Primitive | Use when |
|-----------|---------|
| `async def` / `await` | I/O pipelines, logical async, sequential awaiting |
| `spawn f(args)` | Fire-and-forget detached OS thread |
| `task_group:` | Structured concurrency — wait for a batch of spawns |
| `await_all(f1(), f2(), ...)` | Run multiple calls in parallel, wait for all |
| `Chan[T]` | Producer-consumer pipelines between threads |
| `Mutex[T]` | Thread-safe guarded value with exclusive lock |
| `RwLock[T]` | Multiple readers or single writer |
| `ThreadPool` | Fixed worker-pool for dispatching many short jobs |
| `shared` | Atomic refcount box for shared ownership |

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
| `shared x.field` concurrent writes | ✗ | Data race — use `Mutex[T]` |
| `List[T]` shared across threads | ✗ | No synchronization |

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
