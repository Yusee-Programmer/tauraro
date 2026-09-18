# std.prof — CPU Sampling, Memory Stats, and Metrics

```tauraro
from std.prof import CpuProfiler, MemProf, Registry, Counter, Gauge, Histogram
```

Three independent pieces, usable together or separately: a statistical CPU
sampling profiler, process memory snapshots, and a general-purpose metrics
registry (counters/gauges/histograms). Also available as a `tauraroc --prof`
CLI flag that auto-instruments a whole program's CPU profile with zero
source changes — see below.

---

## CpuProfiler

Statistical sampling, not instrumentation: a background timer/signal records
the **leaf program counter** of the running program at a fixed interval (no
compiler codegen changes, near-zero overhead when off, ~1-2% at the default
1ms interval). This gives a flat "which function was on-CPU X% of the time"
profile — the same shape as py-spy's or pprof's simplest mode.

> **Scope note** — v1 captures the leaf PC only, not a full unwound call
> stack (that needs frame-pointer walking, unreliable under `-O2`, or
> DWARF/CFI unwinding, both a larger and riskier undertaking deferred to a
> future round). Symbol resolution is best-effort: POSIX resolves via
> `dladdr()` (works when the binary isn't fully stripped); Windows v1
> deliberately does not link a symbol-resolution library, so it always
> reports raw hex addresses — post-process with `addr2line`/`dumpbin`
> against the same binary if you need names.

| Method | Signature | Returns | Description |
|---|---|---|---|
| `CpuProfiler.start` | `() -> CpuProfiler` | `CpuProfiler` | Begin sampling at the default 1ms interval. |
| `CpuProfiler.start_interval` | `(interval_us: int) -> CpuProfiler` | `CpuProfiler` | Begin sampling at an explicit interval, in microseconds. |
| `stop` | `(self)` | `void` | Stop sampling. Safe to call more than once. |
| `report` | `(self) -> str` | `str` | Stops sampling (if still running) and returns a flat text profile, sorted by sample count descending. |

```tauraro
mut prof = CpuProfiler.start()
... do work ...
print(prof.report())
# CPU profile: 73 samples, 14 distinct PCs
#    15.07%      11  00007ff60b17a819
#    15.07%      11  00007ff60b17a84f
#    ...
```

The sample buffer is process-wide (not per-`CpuProfiler` instance) — calling
`.report()` reports everything sampled since the last `start()`, across
however many `CpuProfiler` handles were created.

---

## MemProf

Process-wide resident memory, via the OS (working set on Windows,
`/proc/self/statm` on Linux, `task_info` on macOS).

> **Scope note** — this is process-level only: it does not attribute memory
> to allocation sites or Tauraro types (a heap profiler in the
> pprof/valgrind-massif sense would need hooking `TAURARO_ALLOC`/
> `TAURARO_FREE`, called from thousands of sites across the runtime —
> deliberately deferred rather than risking a subtle bug from wrapping them
> unconditionally).

| Method | Signature | Returns | Description |
|---|---|---|---|
| `MemProf.rss_bytes` | `() -> int` | `int` | Current resident set size in bytes, or `-1` if unavailable. |
| `MemProf.peak_rss_bytes` | `() -> int` | `int` | Peak resident set size since process start, in bytes, or `-1` if unavailable. |

---

## Counter / Gauge / Histogram / Registry

General-purpose instrumentation, independent of the CPU/memory profilers —
use for request counts, queue depths, latency distributions, or your own
networking counters (see the note below).

```tauraro
mut reg = Registry.init()
reg.counter("requests_total").inc()
reg.gauge("queue_depth").set(12.0)
reg.histogram("request_ms", [1.0, 10.0, 100.0, 1000.0]).observe(42.0)
print(reg.report())
```

| Type | Method | Signature | Description |
|---|---|---|---|
| `Counter` | `.inc` | `(self)` | Increment by 1. |
| | `.inc_by` | `(self, n: int)` | Increment by `n`. |
| | `.get` | `(self) -> int` | Current value. |
| `Gauge` | `.set` | `(self, v: float)` | Set the current value. |
| | `.add` | `(self, delta: float)` | Add `delta` to the current value. |
| | `.get` | `(self) -> float` | Current value. |
| `Histogram` | `.observe` | `(self, v: float)` | Record an observation into the appropriate bucket. |
| | `.mean` | `(self) -> float` | Mean of all observed values (`0.0` if none). |
| `Registry` | `.counter` | `(self, name: str) -> Counter` | Get-or-create a named counter. |
| | `.gauge` | `(self, name: str) -> Gauge` | Get-or-create a named gauge. |
| | `.histogram` | `(self, name: str, bounds: Vec[float]) -> Histogram` | Get-or-create a named histogram (`bounds` only used on first creation). |
| | `.report` | `(self) -> str` | Plain-text dump of every registered metric. |

`Histogram` buckets are upper-inclusive cumulative edges: `bounds = [1.0,
10.0, 100.0]` means "<=1", "<=10", "<=100" — a value greater than every bound
still counts toward `.count`/`.sum` but no individual bucket.

**Networking**: there is deliberately no automatic `std.net` hook. Call
`reg.counter("net.bytes_sent").inc_by(n)` (etc.) yourself around your own
`std.net.tcp` call sites — same `Registry`, same `report()` output, with zero
risk to `std.net`'s own already-tested code paths.

---

## `tauraroc --prof`

Auto-instruments a whole program's CPU profile with **zero source changes**:

```
tauraroc --run --prof myapp.tr
tauraroc myapp.tr -o myapp --prof && ./myapp
```

Wraps the compiled program's `main()` with `CpuProfiler`-equivalent
start/stop calls (1ms interval) and prints the resulting flat profile to
**stderr** on exit. This is independent of (and composable with) explicitly
importing `std.prof` in your own code — use `--prof` for "profile the whole
run", or `CpuProfiler`/`Registry` directly when you want to profile only
part of a program or combine CPU sampling with your own metrics.
