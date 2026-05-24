# std.async — Concurrency Primitives

All async primitives are backed by real OS-level threading (Win32 on Windows, pthreads on POSIX).  
They are safe to use across threads and are deadlock-free by design (single lock per primitive, condvar waits use `while` loops, `WakeAll`/`broadcast` on close/cancel).

> **Async context rule** — `spawn:`, `taskgroup:`, and `await` are only permitted inside `async def` functions. The sync-only primitives (`Mutex`, `Semaphore`, `WaitGroup`, `Barrier`, `Once`) may be used anywhere.

---

## Channel

**When**: You need to pass values between producer and consumer threads safely.
**Why**: Bounded MPMC ring buffer with blocking send/recv; `try_send`/`try_recv` for non-blocking paths; timeout variants for deadline-aware code.

```tauraro
from std.async.channel import Channel
```

### Construction

```tauraro
mut ch = Channel.new(cap)   # cap — max buffered values (>= 1)
```

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Channel.new` | `(cap: int) -> Channel` | `Channel` | Create a channel with given buffer capacity. |
| `send` | `(val: int)` | `void` | Send a value; **blocks** if the buffer is full. |
| `recv` | `() -> int` | `int` | Receive a value; **blocks** if the buffer is empty. |
| `try_send` | `(val: int) -> bool` | `bool` | Non-blocking send. `true` on success. |
| `try_recv` | `() -> int` | `int` | Non-blocking receive. Returns `0` if empty. |
| `send_timeout` | `(val: int, ms: int) -> bool` | `bool` | Send with a millisecond deadline. `true` on success. |
| `recv_timeout` | `(ms: int) -> int` | `int` | Receive with a deadline. Returns `0` on timeout. |
| `close` | `()` | `void` | Mark the channel closed; wakes blocked receivers. |
| `is_closed` | `() -> bool` | `bool` | |
| `len` | `() -> int` | `int` | Number of values currently buffered. |
| `cap` | `() -> int` | `int` | Maximum buffer capacity. |
| `is_empty` | `() -> bool` | `bool` | |
| `is_full` | `() -> bool` | `bool` | |
| `free` | `()` | `void` | Release OS resources. |

### Example

```tauraro
from std.async.channel import Channel

async def producer_consumer():
    mut ch = Channel.new(8)
    ch.send(1)
    ch.send(2)
    mut a = ch.recv()   # 1
    mut b = ch.recv()   # 2
    ch.close()
    ch.free()
```

---

## Task / Future

**When**: You need to represent the eventual result of an async operation and synchronise on it.
**Why**: `Task` has a name for logging; `Future` is a lightweight anonymous promise. Both support `.await_()`, timeouts, and error propagation.

```tauraro
from std.async.task import Task, Future
```

### Task methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Task.new` | `(name: str) -> Task` | `Task` | Create a task in the pending state. |
| `complete` | `(result: int)` | `void` | Mark done with a result value. |
| `fail` | `(msg: str)` | `void` | Mark done with an error message. |
| `cancel` | `()` | `void` | Cancel the task; wakes blocked `await_()`. |
| `await_` | `() -> int` | `int` | Block until done. Returns `result` (or `0` on cancel/error). |
| `await_timeout` | `(ms: int) -> bool` | `bool` | `true` if done within the deadline. |
| `is_done` | `() -> bool` | `bool` | |
| `is_cancelled` | `() -> bool` | `bool` | |
| `has_error` | `() -> bool` | `bool` | |
| `get_error` | `() -> str` | `str` | Error message, or `""` if none. |
| `free` | `()` | `void` | |

### Future methods

A `Future` is a single-value promise with no name.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Future.new` | `() -> Future` | `Future` | Create a future in the pending state. |
| `resolve` | `(value: int)` | `void` | Fulfill the future with a value. |
| `reject` | `(msg: str)` | `void` | Fail the future with an error message. |
| `await_` | `() -> int` | `int` | Block until resolved. Returns the value. |
| `await_timeout` | `(ms: int) -> bool` | `bool` | `true` if resolved within the deadline. |
| `is_ready` | `() -> bool` | `bool` | |
| `has_error` | `() -> bool` | `bool` | |
| `get_error` | `() -> str` | `str` | |
| `free` | `()` | `void` | |

### Example

```tauraro
from std.async.task import Task, Future

async def run_job():
    mut t = Task.new("my-job")
    t.complete(42)
    mut result = t.await_()    # 42
    t.free()

    mut f = Future.new()
    f.resolve(7)
    mut val = f.await_()       # 7
    f.free()
```

---

## TaskGroup

**When**: You need to track and wait for a set of concurrent tasks together.
**Why**: One `wait_all()` call blocks until every task finishes, regardless of completion order.

```tauraro
from std.async.group import TaskGroup
```

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `TaskGroup.new` | `() -> TaskGroup` | `TaskGroup` | Create an empty group. |
| `add` | `(name: str) -> Task` | `Task` | Create a task, add it to the group, and return it. |
| `wait_all` | `()` | `void` | Block until every task is done. |
| `wait_all_timeout` | `(ms: int) -> bool` | `bool` | `true` if all tasks finished within the deadline. |
| `cancel_all` | `()` | `void` | Cancel all pending tasks. |
| `all_done` | `() -> bool` | `bool` | `true` when every task is in a terminal state. |
| `pending_count` | `() -> int` | `int` | Tasks not yet done. |
| `len` | `() -> int` | `int` | Total tasks in the group. |
| `free` | `()` | `void` | |

### Example

```tauraro
from std.async.group import TaskGroup

async def parallel_work():
    mut tg = TaskGroup.new()
    mut t1 = tg.add("step-a")
    mut t2 = tg.add("step-b")
    t1.complete(0)
    t2.complete(0)
    tg.wait_all()
    tg.free()
```

---

## Mutex / RWLock

**When**: Multiple threads access shared mutable state.
**Why**: `Mutex` guarantees exclusive access; `RWLock` allows many concurrent readers when no writer is active.

```tauraro
from std.async.mutex import Mutex, RWLock
```

### Mutex methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Mutex.new` | `() -> Mutex` | `Mutex` | |
| `lock` | `()` | `void` | Acquire; blocks until available. |
| `unlock` | `()` | `void` | Release. |
| `try_lock` | `() -> bool` | `bool` | Acquire without blocking. `true` on success. |
| `free` | `()` | `void` | |

### RWLock methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `RWLock.new` | `() -> RWLock` | `RWLock` | |
| `read_lock` | `()` | `void` | Acquire shared read access. |
| `read_unlock` | `()` | `void` | Release read access. |
| `write_lock` | `()` | `void` | Acquire exclusive write access. |
| `write_unlock` | `()` | `void` | Release write access. |
| `free` | `()` | `void` | |

### Example

```tauraro
from std.async.mutex import Mutex

mut mu = Mutex.new()
mu.lock()
# critical section
mu.unlock()
mu.free()
```

---

## Semaphore

**When**: You need to limit concurrent access to a resource pool (e.g. max 3 DB connections).
**Why**: Counting semaphore; `acquire` blocks when the count hits zero; `release` wakes one waiter.

```tauraro
from std.async.semaphore import Semaphore
```

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Semaphore.new` | `(init: int, max: int) -> Semaphore` | `Semaphore` | Create with `init` permits and a ceiling of `max`. |
| `acquire` | `()` | `void` | Decrement; **blocks** if count is 0. |
| `try_acquire` | `() -> bool` | `bool` | Non-blocking acquire. `true` on success. |
| `acquire_timeout` | `(ms: int) -> bool` | `bool` | Acquire within a deadline. `true` on success. |
| `release` | `()` | `void` | Increment, waking one blocked `acquire`. |
| `free` | `()` | `void` | |

### Example

```tauraro
from std.async.semaphore import Semaphore

mut sem = Semaphore.new(3, 3)   # 3 slots
sem.acquire()
# use resource
sem.release()
sem.free()
```

---

## WaitGroup

**When**: A coordinator thread needs to wait for N worker threads to finish.
**Why**: Modelled after Go's `sync.WaitGroup`; simpler than tracking N futures manually.

```tauraro
from std.async.waitgroup import WaitGroup
```

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `WaitGroup.new` | `() -> WaitGroup` | `WaitGroup` | Create with count 0. |
| `add` | `(n: int)` | `void` | Increment the counter by `n`. Call before spawning workers. |
| `done` | `()` | `void` | Decrement by 1. Each worker calls this when finished. |
| `wait` | `()` | `void` | Block until the counter reaches 0. |
| `wait_timeout` | `(ms: int) -> bool` | `bool` | `true` if counter reaches 0 within the deadline. |
| `free` | `()` | `void` | |

### Example

```tauraro
from std.async.waitgroup import WaitGroup

async def fan_out():
    mut wg = WaitGroup.new()
    wg.add(2)
    # ... spawn workers that each call wg.done() ...
    wg.wait()
    wg.free()
```

---

## Barrier

**When**: N threads must all reach a checkpoint before any of them proceeds.
**Why**: Cyclic — once all threads arrive, the barrier resets automatically for the next round.

```tauraro
from std.async.barrier import Barrier
```

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Barrier.new` | `(n: int) -> Barrier` | `Barrier` | Create for `n` participants. |
| `wait` | `()` | `void` | Block until all `n` participants have arrived. |
| `free` | `()` | `void` | |

### Example

```tauraro
from std.async.barrier import Barrier

mut bar = Barrier.new(3)   # release when 3 threads arrive
bar.wait()                 # each thread calls this
bar.free()
```

---

## Once

**When**: You need lazy one-time initialisation shared across threads (e.g. singleton setup).
**Why**: Thread-safe; `do_once` returns `true` exactly once regardless of how many threads call it.

```tauraro
from std.async.once import Once
```

### Methods

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Once.new` | `() -> Once` | `Once` | |
| `do_once` | `() -> bool` | `bool` | `true` the first call; `false` on all subsequent calls. |
| `free` | `()` | `void` | |

### Example

```tauraro
from std.async.once import Once

mut once  = Once.new()
mut first = once.do_once()    # true
mut again = once.do_once()    # false
once.free()
```

---

## Timer / Ticker

**When**: You need to fire an event after a delay (Timer) or at regular intervals (Ticker).
**Why**: Delivers ticks through a `Channel` — integrates cleanly with select-style `recv` loops.

```tauraro
from std.async.timer import Timer, Ticker
```

### Timer — fires once after a delay

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Timer.new` | `(ms: int, ch: Channel) -> Timer` | `Timer` | Send one value on `ch` after `ms` milliseconds. |
| `stop` | `()` | `void` | Cancel the timer (no-op if already fired). |

### Ticker — fires periodically

| Method | Signature | Returns | Description |
|---|---|---|---|
| `Ticker.new` | `(ms: int, ch: Channel) -> Ticker` | `Ticker` | Send a value on `ch` every `ms` milliseconds. |
| `stop` | `()` | `void` | Stop periodic ticking. |

### Example

```tauraro
from std.async.timer   import Timer
from std.async.channel import Channel

async def timeout_example():
    mut sig = Channel.new(1)
    mut t = Timer.new(500, sig)   # fire after 500 ms
    mut _ = sig.recv()            # wait for the tick
    t.stop()
    sig.free()
```
