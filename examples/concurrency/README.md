# Tauraro Concurrency Examples

Runnable, self-contained examples for every concurrency model and pattern.
Pair them with the guide: [`docs/lang/advanced/07_concurrency_guide.md`](../../docs/lang/advanced/07_concurrency_guide.md).

Build & run any example:

```
tauraroc examples/concurrency/01_cpu_parallel.tr -o demo && ./demo
```

| File | Model | Primitives / pattern shown |
|------|-------|----------------------------|
| `01_cpu_parallel.tr` | OS threads | `task_group` + `Atomic` — parallel map-reduce (CPU-bound) |
| `02_shared_state.tr` | OS threads | `Mutex[T]` (compound state) + `RwLock[T]` (read-heavy) |
| `03_producer_consumer.tr` | OS threads | `Chan[T]` pipeline + `ThreadPool` (bounded fan-out) |
| `04_async_await.tr` | Green threads | `async`/`await`, `await_all` (concurrent), `await_timeout` |
| `05_green_threads.tr` | Green threads | `Coro.spawn`/`yield_now`/`run` — cooperative tasks on 1 thread |
| `06_sync_primitives.tr` | OS threads | `WaitGroup`, `Semaphore`, `Once`, `Barrier` |
| `07_combined_pipeline.tr` | OS threads | **Best-practice combo**: `Chan` + workers + `WaitGroup` + `Atomic` metrics |

## Choosing a model (quick version)

- **CPU-bound** (math, hashing, parsing loops) → OS threads: `task_group:` + `spawn`, combine with `Atomic`/`Mutex`. (`01`, `02`, `07`)
- **I/O-bound, few tasks** → `async def` + `await` / `await_all`. (`04`)
- **I/O-bound, many connections (a server)** → thread-per-core green threads: watax `listen_async`, or `Coro.spawn` one green thread per connection over a non-blocking reactor. (`05`; networked: `bench/tau_server.tr`, `../tauProject/watax/`)

## Notes

- `spawn`, `task_group:`, `await`, `await_all`, `await_timeout` may only appear **inside an `async` function** (use `async def main()`).
- `spawn`/`task_group` create **OS threads** (real parallelism); `await`/`Coro.spawn` create **green threads** (cooperative). See the guide for why this matters.
- Values shared across OS threads must be `Sendable` — use `Atomic[T]`, `Mutex[T]`, `Chan[T]`, or the `std.async` coordination primitives (`WaitGroup`, `Semaphore`, …), which are all Sendable. A plain class with mutable fields is not.
- Inside an `async`/`Coro` task, never make a **blocking** call (blocking `recv`, `Thread.sleep`) — it stalls the whole worker. Use the async I/O primitives instead.
