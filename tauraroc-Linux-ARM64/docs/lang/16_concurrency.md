# 16 — Concurrency

---

## Two Concurrency Models

| Model | Keyword | C implementation | Use when |
|-------|---------|-----------------|---------|
| Cooperative | `async def` / `await` | Synchronous call (forward-compatible) | I/O pipelines, logical async |
| Preemptive | `spawn` / `task_group:` | OS threads (`pthreads` / Win32) | Parallel CPU work, real threading |
| Shared state | `shared` | Atomic refcount box | Multiple owners of one value |

---

## Cooperative: async / await

### Declaring Async Functions

```python
async def fetch(id: int) -> str:
    return f"item-{id}"

async def pipeline(n: int) -> int:
    mut data = await fetch(n)
    return len(data)

async def run():
    mut r1 = await pipeline(1)
    mut r2 = await pipeline(42)
    print(f"pipeline(1)  = {r1}")
    print(f"pipeline(42) = {r2}")
```

### Current Semantics: Synchronous

In the current compiler, `async`/`await` executes **synchronously**. `await f(x)` is a direct function call — no scheduler, no event loop, no suspension, no heap future allocation.

**Why compile async as synchronous today?**

1. **Forward compatibility:** The syntax is locked in now. When a true async runtime is added, all existing `async`/`await` code continues to work unchanged.

2. **Correct single-threaded semantics:** For sequential I/O pipelines that don't need parallelism, synchronous execution *is* the correct behavior. The code reads as async and runs correctly.

3. **Zero overhead today:** No future allocation, no scheduler, no executor. Just a function call.

**How async compiles:**
```c
// async def fetch(id: int) -> str:
//     return f"item-{id}"
// compiles identically to:
// def fetch(id: int) -> str:
//     return f"item-{id}"

static inline char* fetch(long long id) {
    return _tr_format("item-%lld", id);
}

// await fetch(n)  →  fetch(n)
char* data = fetch(n);
```

### Rules for async/await

- `async def` can `await` other `async def` functions
- A non-async `def` can call an `async def` without `await` (just a normal call)
- `await` on a non-async function is a no-op pass-through
- Return type of `await f()` is the return type of `f`, not a future wrapper
- `@inline` applies to `async def` exactly as to regular functions

**Best practice:** Use `async`/`await` when your code models I/O or you expect to add real async scheduling later. Don't add it purely for style — it communicates something about the function's intended behavior.

---

## Preemptive: spawn

`spawn f(arg)` starts a real OS thread that runs `f` with argument `arg`:

```python
def worker(id: int) -> void:
    print(f"  worker {id} running")
    # ... do work
    print(f"  worker {id} done")

def main():
    spawn worker(1)    # starts OS thread, detaches it
    spawn worker(2)    # starts another OS thread
    spawn worker(3)
    # program may exit before all workers finish (they're detached)
    print("main thread continues immediately")
```

### Detached Spawn

A `spawn` outside a `task_group:` is **detached** — fire-and-forget. The spawning thread doesn't wait for it.

**How detached spawn compiles:**
```c
// spawn worker(1)
{
    _TrThread _th = _tr_thread_start(_tr_spawn_wrap_worker, (void*)(uintptr_t)(1));
    _tr_thread_detach(_th);
}
```

`_tr_thread_start` calls `pthread_create` on Linux/macOS or `CreateThread` on Windows.

### Spawn Wrappers

POSIX and Win32 threads both take a single `void*` argument. The compiler auto-generates a wrapper for every spawned function that packs the argument into a `uintptr_t`:

```c
// generated for: spawn worker(id)
static void* _tr_spawn_wrap_worker(void* _arg) {
    long long _v = (long long)((uintptr_t)_arg);
    worker(_v);
    return NULL;
}
```

This is generated automatically — you don't write it.

**Argument constraint:** The single-argument pack works correctly for:
- Integer types up to 64 bits
- Pointer types (class instances, strings)

For multiple arguments, pack them into a class:

```python
class WorkArgs:
    pub task_id: int
    pub priority: int
    pub data: str

extend WorkArgs:
    pub def init(id: int, pri: int, d: str) -> WorkArgs:
        mut w = WorkArgs()
        w.task_id = id
        w.priority = pri
        w.data = d
        return w

def worker(args: WorkArgs) -> void:
    print(f"task {args.task_id} (pri {args.priority}): {args.data}")

def main():
    spawn worker(WorkArgs.init(1, 5, "hello"))
    spawn worker(WorkArgs.init(2, 3, "world"))
```

---

## Structured Concurrency: task_group:

`task_group:` creates a scope that waits for all threads spawned inside it before continuing:

```python
def compute(n: int) -> void:
    mut result = n * n
    print(f"  compute({n}) = {result}")

def run_parallel():
    task_group:
        spawn compute(4)
        spawn compute(7)
        spawn compute(12)
    # ALL THREE are guaranteed done here
    print("all workers finished")

def main():
    run_parallel()
    print("main continues after all workers")
```

**How task_group: compiles:**
```c
_tr_tg_begin();
{
    _tr_tg_push(_tr_thread_start(_tr_spawn_wrap_compute, (void*)(uintptr_t)(4)));
    _tr_tg_push(_tr_thread_start(_tr_spawn_wrap_compute, (void*)(uintptr_t)(7)));
    _tr_tg_push(_tr_thread_start(_tr_spawn_wrap_compute, (void*)(uintptr_t)(12)));
}
_tr_taskgroup_wait();
```

`_tr_taskgroup_wait()` iterates over all tracked thread handles and calls `pthread_join` (or `WaitForSingleObject` on Windows) on each one. After it returns, all threads are done.

### Task Group Limit

The task group holds a fixed array of `_TR_MAX_TG_THREADS` = 64 thread handles. Spawning more than 64 threads in one `task_group:` silently ignores extra handles. For workloads requiring more than 64 threads, use multiple `task_group:` blocks or a thread pool pattern.

### Nested task_group: Blocks

Nesting is supported:

```python
task_group:
    spawn phase1_worker(1)
    spawn phase1_worker(2)
# phase1 done

task_group:
    spawn phase2_worker(1)
    spawn phase2_worker(2)
# phase2 done
```

Each `task_group:` block is independent — the previous group is joined before the next starts.

---

## Shared Ownership Across Threads

For shared mutable state, use `shared`. This wraps a value in an atomic reference-counted box:

```python
class Counter:
    pub value: int

extend Counter:
    pub def init() -> Counter:
        mut c = Counter()
        c.value = 0
        return c

    pub def increment(self) -> void:
        self.value = self.value + 1

    pub def get(self) -> int:
        return self.value

def main():
    mut c = Counter.init()
    shared s1 = c           # ref count = 1
    shared s2 = s1          # ref count = 2

    task_group:
        spawn s1.increment()    # thread 1 increments
        spawn s2.increment()    # thread 2 increments (same Counter!)

    # Both threads joined here
    print(f"count = {s1.get()}")    # 2 (or potentially a race condition)
```

**Warning:** `shared` makes the refcount thread-safe. The underlying data (`Counter.value`) is NOT protected by a mutex. If two threads call `increment()` simultaneously, there's a data race. To fix:

```python
# Option 1: Use an atomic type for the value
# (requires extern "C" to access atomic operations)

# Option 2: Use mutex (requires extern "C" pthread_mutex_t)
extern "C":
    def pthread_mutex_lock(m: Pointer[void]) -> int
    def pthread_mutex_unlock(m: Pointer[void]) -> int
```

**Best practice:** Use `shared` for read-only data, or data accessed by only one thread at a time. For mutable shared state, protect with an explicit mutex or use atomic operations.

---

## Thread Safety Guidelines

| Pattern | Thread safe? | Notes |
|---------|-------------|-------|
| `spawn f(val)` with value copy | ✓ | val is copied into the thread |
| `shared x` reference count | ✓ | Refcount is `_Atomic int` |
| Reading `shared x.field` | ✓ if no writes | Safe if no thread writes |
| Writing `shared x.field` in parallel | ✗ | Data race — add mutex |
| `List[T]` shared across threads | ✗ | No synchronization |
| `Dict` shared across threads | ✗ | No synchronization |
| Channels (future) | — | Not yet implemented |

---

## Common Concurrency Errors

### Using local variable after spawn (use-after-free)

```python
def main():
    mut local = Counter.init()         # local variable
    spawn process(local)               # spawns thread with pointer to local
    # local freed here when main() returns!
    # but spawned thread is still running and accessing it → UB
```

**Fix:** Use `shared` so the data lives until the last reference drops:
```python
shared s = Counter.init()
spawn process(s)   # s is refcounted — outlives main()
```

### Forgetting task_group: (data race)

```python
spawn update_state(1)    # starts thread 1
spawn update_state(2)    # starts thread 2
read_result()            # reads state — threads may not be done yet!
```

**Fix:**
```python
task_group:
    spawn update_state(1)
    spawn update_state(2)
read_result()    # guaranteed after both threads complete
```

### Spawning too many threads in one group

```python
task_group:
    mut i = 0
    while i < 200:           # spawning 200 threads — exceeds limit of 64
        spawn worker(i)
        i = i + 1
```

**Fix:** Batch into groups of 64:
```python
mut i = 0
while i < 200:
    task_group:
        mut batch = 0
        while batch < 64 and i < 200:
            spawn worker(i)
            i = i + 1
            batch = batch + 1
```

---

## Summary

| Construct | Compiles to | Notes |
|-----------|------------|-------|
| `async def f()` | Normal C function | Synchronous today; forward-compatible |
| `await f(x)` | Direct call `f(x)` | Return type of `f`, not a future |
| `spawn f(x)` (outside group) | `_tr_thread_start` + detach | Fire-and-forget |
| `task_group: { spawn f(x) }` | `_tr_tg_begin` + push + wait | Structured concurrency; max 64 threads |
| `shared x = val` | `_TrSharedBox*` refcounted | Refcount atomic; data is not |
| `shared y = x` (clone) | `_tr_shared_clone(x)` | Increments refcount atomically |

---

Next: [Extern & FFI →](17_extern_and_ffi.md)
