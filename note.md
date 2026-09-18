# Tauraro v0.0.9 — Release Notes

**Focus of this release: correctness and reliability hardening across concurrency, generics, and the standard library, plus a new profiling module and a substantially more trustworthy CI pipeline.**

Where v0.0.8 turned Tauraro into a batteries-included, cross-platform
toolchain, v0.0.9 is a **stabilization release**: a large batch of real,
independently-reproduced compiler and runtime bugs fixed (generic
interfaces, async/await exception propagation, ownership/ARC edge cases,
Windows-specific codegen), five new/completed standard-library areas
(`std.cli`, `std.log`, `std.regex` capture groups, `std.crypto` UUID v3/v5 +
ULID, `std.encoding.yaml`), a brand-new `std.prof` profiling module with a
`tauraroc --prof` CLI flag, GPU compute support, a turnkey UEFI target, and
a CI pipeline that no longer silently hides failures behind unrelated
matrix-leg noise.

> Pre-1.0 policy: until 1.0, any `0.x` bump may contain breaking changes with
> no deprecation period. Nothing user-facing was removed in this release.

---

## ✨ Highlights

- **Async/await is now a real green-thread runtime**, not one OS thread per
  `await`. Every `async def`/`await` task is a lightweight stackful
  coroutine (Windows Fibers / POSIX `ucontext`) scheduled cooperatively on
  a small number of OS threads, integrated with the `_TrIOPoll` reactor
  (epoll/IOCP-select/kqueue) so an `await` on socket I/O parks the task on
  fd-readiness instead of blocking a thread. `AsyncPool` adds multi-core
  work-stealing on top (a Chase-Lev deque per worker, Go/Tokio-style load
  balancing), and `await_all(...)` now runs on that pool instead of
  spawning one raw OS thread per sub-call — a 300-way `await_all` used to
  mean 300 OS threads; it now stays bounded near the machine's core count.
- **`tauraroc --lib`**: builds a shared library (`.dll`/`.so`) with a
  self-contained C header, so C/Rust/etc. can call into Tauraro code —
  `export def` functions get real C-ABI external linkage.
- **`tauraroc fmt`/`tauraroc lint`**, and incremental compilation (a build
  now reuses any module's cached object file when its generated C is
  byte-identical to the previous build) landed alongside `std.encoding.toml`
  (a full TOML parser/serializer).
- **`std.prof` — a real profiling module, plus `tauraroc --prof`.**
  Statistical CPU sampling (leaf-PC, SIGPROF/`ITIMER_PROF` on POSIX,
  `SuspendThread`-based on Windows), process memory stats (RSS/peak RSS),
  and a general-purpose metrics registry (`Counter`/`Gauge`/`Histogram`).
  `tauraroc --prof` auto-instruments a whole program's CPU profile with
  **zero source changes**.
- **Six real generic-interfaces bugs fixed in one pass** — a parser
  infinite loop/segfault on generic bounds with type arguments, missing
  dynamic-dispatch boxing for bare interface return types, mis-codegen for
  method calls inside monomorphized generic bodies, mis-parsed multi-type-arg
  explicit generic calls, silently-discarded interface type arguments on a
  non-generic class implementing a generic interface, and un-inferred
  implicit multi-generic calls.
- **`std.cli`, `std.log`, and `std.encoding.yaml` are new**; `std.regex`
  gains capture groups (numbered + named, `(?<name>...)`/`(?P<name>...)`),
  `Match`/`Captures`, and backreference replace; `std.crypto` gains UUID
  v3/v5 (namespace-based) and ULID (including monotonic). 166+ new test
  assertions across these areas.
- **A real, longstanding MD5 bug fixed** — the round-2 (`G`) function had
  its `B`/`D` operands transposed, so every MD5 digest was wrong for every
  input. Verified against RFC 4122 known-answer vectors.
- **Async/await exception propagation fixed**, including a Windows-Fiber-
  specific cross-boundary `longjmp` crash — an exception raised inside an
  `await`ed call is now correctly caught by the awaiter's own `try`/`except`,
  on every backend, at every nesting depth tested (1–3 levels deep).
- **GPU compute support landed**: `std.gpu` (CUDA/OpenCL/CPU device API via
  runtime `dlopen`, zero SDK dependency) and `@kernel`-annotated functions
  compiling straight to PTX/SPIR-V, plus automatic linking so GPU programs
  need no manual `-l`/`-L` flags.
- **`--target uefi-x64`** is now a turnkey target: the compiler generates
  the UEFI boot glue itself (no hand-written stub required) — pair with
  `--freestanding` for a self-contained `.efi` binary.
- **CI no longer silently hides failures.** The Android and benchmark jobs
  were being skipped whenever *any unrelated* platform leg in the build
  matrix failed (a GitHub Actions default-`needs` gotcha); both now run
  independently of unrelated leg failures. Separately, three genuinely
  different Windows CI failures (a `regex.h` availability assumption, a
  `-lsystre`/`-ltre` link-flag guess, and a runtime null-pointer crash when
  regex truly isn't available on a given runner) were tracked down and
  fixed — the last one exposed that GitHub's `windows-latest` runner image
  is not stable across runs of the same label, so `std.regex` now detects
  and gracefully skips instead of crashing when the platform has no
  `<regex.h>` at all.

---

## 🚀 Added

### Async/await — green-thread runtime + multi-core work-stealing
- `async def`/`await` no longer spawn an OS thread per `await` and block
  the caller — each task is a stackful coroutine with its own small stack,
  and `await` is a cheap context switch driven by a cooperative scheduler
  integrated with the platform reactor (epoll/IOCP-select/kqueue), so
  socket `await`s park on fd-readiness instead of blocking a thread.
  `Coro.sleep_ms`/`Coro.yield_now`/`Coro.await_readable`/`await_writable`
  are available via `std/async/coro`. (`spawn`/`task_group` keep
  OS-thread parallelism for CPU-bound work, unchanged.)
- **`AsyncPool`/`AsyncTask`** — a multi-core, work-stealing pool for
  top-level async calls. Each of N persistent worker threads owns a
  lock-free Chase-Lev work-stealing deque; an idle worker steals a
  not-yet-started task from a busier sibling instead of waiting.
  ```python
  mut pool = AsyncPool.auto()             # sized to the machine's cores
  mut t = pool.spawn(fetch_page, url)     # -> AsyncTask, runs immediately
  t.join()                                # cross-thread join, like Thread.join()
  ```
  `AsyncPool.spawn` is Sendable-checked at compile time exactly like
  `spawn`/`Thread.spawn`/`ThreadPool.spawn`.
- **`await_all(f1(), f2(), ...)`** now runs on `AsyncPool` instead of
  spawning one raw OS thread per sub-call — a large batch stays bounded
  near the machine's core count instead of one thread per call, with no
  syntax/semantics change.

### Developer tooling
- **`tauraroc --lib`** — builds a shared library (`.dll`/`.so`) instead of
  an executable, compiling every module `-fPIC` and linking `-shared`, and
  emits a self-contained C header declaring the `export def` functions so
  C/Rust/etc. consumers can call into Tauraro code. (Exports with
  primitive/pointer signatures are fully self-contained; signatures using
  Tauraro struct types like `TrStr`/`Result` still need the runtime
  header — a documented limitation.)
- **`tauraroc fmt [-w] <file>`** — a source formatter that preserves
  comments, is idempotent (`fmt(fmt(x)) == fmt(x)`, CI-verified), and
  refuses to rewrite a file containing a construct it can't yet render
  rather than risk corrupting it.
- **`tauraroc lint <file>`** — runs module resolution + semantic analysis
  and reports warnings/errors without producing an executable.
- **Incremental compilation** — a build compiles each module's generated C
  to its own object file and reuses the cached `.o` for any module whose
  generated C is byte-identical to the previous build (and whose shared
  headers/compile flags are unchanged); a single-module edit now
  recompiles only the affected module(s).
- **`--debug` builds emit C `#line N "source.tr"` directives**, so GCC
  diagnostics and GDB backtraces reference the original `.tr` source
  instead of generated C.
- **Parser diagnostics** now report a precise column and a source snippet
  with a caret (`file:line:col: error: <msg>`, the offending line, and a
  `^` under the exact column) followed by a `FIX:` hint.
- `std.encoding.toml` — a TOML parser and serializer (comments,
  bare/quoted keys, basic & literal strings, `_`-separated integers,
  floats, booleans, arrays, inline tables, `[table]`/`[a.b.c]` nested
  headers).

### `std.prof` — profiling (new module + `--prof` CLI flag)
- **`CpuProfiler`** — statistical sampling profiler. Captures the leaf
  program counter at a configurable interval (default 1ms); `.report()`
  returns a flat, sorted-by-sample-count text profile. Best-effort by
  design: on POSIX, `SIGPROF`+`setitimer(ITIMER_PROF, ...)`; on Windows, a
  dedicated sampler thread using `SuspendThread`/`GetThreadContext`/
  `ResumeThread` (no SIGPROF equivalent exists there). Symbol resolution
  via `dladdr()` on POSIX; Windows v1 reports raw addresses only (no
  `dbghelp` dependency, a deliberate risk-avoidance call).
- **`MemProf`** — `rss_bytes()`/`peak_rss_bytes()`, process-level only (no
  per-allocation-site attribution).
- **`Counter`/`Gauge`/`Histogram`/`Registry`** — general-purpose metrics,
  usable standalone or alongside CPU/memory profiling (e.g. for your own
  networking counters around `std.net` call sites).
- **`tauraroc --prof`** — auto-wraps a compiled program's `main()` with CPU
  sampling start/stop and prints the resulting flat profile to stderr on
  exit, with **zero source changes** required.

### `std.gpu` — portable GPU compute
- Runtime device API (`Device`/`Buffer[T]`/`Module`/`Kernel`/`Dim3`) backed
  by CUDA, OpenCL, or CPU fallback, resolved via runtime `dlopen` — no GPU
  SDK needed to *build* a program that uses it.
- `@kernel`-annotated functions compile to real GPU code (PTX for CUDA,
  SPIR-V for OpenCL) via a dedicated LLVM IR lowering path, callable with a
  one-line `kernel_launch(...)`. `@device` helper functions, 2D/3D grids,
  and compile-time argument-count checking are supported.
- GPU program linking is now automatic — no manual `-l`/`-L` flags needed
  to build and run a `std.gpu`-using program.

### `--target uefi-x64` — turnkey UEFI target
- The compiler now generates its own UEFI boot glue (the zig UEFI stub) at
  compile time — no hand-written boot stub required, pair with
  `--freestanding` for a self-contained `.efi` binary.
- Real keyboard/mouse input support and a small native "chrome" toolkit
  layer for building interactive UEFI applications.

### Standard library — Tier 2 completion
- **`std.cli`** — clap-style CLI parsing: flags, typed options, positionals,
  subcommands, auto-generated `--help` (`Cli`/`CliArgs`). 54/54 tests.
- **`std.log`** — leveled structured logging: text/JSON formats, stdout/file
  sinks, per-target filtering (`Logger`/`Fields`). 18/18 tests.
- **`std.encoding.yaml`** — a real YAML parser + serializer (block/flow
  styles, anchors). 65/65 tests.
- **`std.regex`** — capture groups (numbered *and* named, via
  `(?<name>...)` or Python-style `(?P<name>...)`), `Match`/`Captures`
  objects, and `$1`/`$name`/`$$` backreference replace. `Regex.available()`
  lets callers detect (and gracefully handle) platforms with no POSIX
  `<regex.h>` at all, instead of misreading a silent no-op stub as "no
  match ever found". 54/54 tests.
- **`std.crypto`** — UUID v3/v5 (namespace + name based) and ULID
  (including a monotonic variant). 41/41 (`uuid.tr`) tests.

---

## 🔧 Changed

- CI's Android build (`build-termux-android`) and benchmark job no longer
  get silently skipped when an *unrelated* platform leg in the 4-way build
  matrix fails — both now run whenever the workflow itself wasn't
  cancelled, so a flaky macOS or Windows leg can no longer hide a real
  Android or benchmark regression.
- Windows regex linking no longer guesses `-lsystre -ltre` from a filename
  convention — the compiler now does a real standalone compile+link probe
  and only adds those flags when they actually resolve, since MinGW
  distributions vary in what they bundle (and, as this release's
  investigation confirmed, the *same* `windows-latest` runner label can
  differ across runs of the same CI job).

---

## 🐛 Fixed

### Concurrency / async
- **`Type.auto()`-style zero-arg static factory constructors** (e.g.
  `ThreadPool.auto()`, `AsyncPool.auto()`) never had their return type
  inferred, silently miscompiling every call on the result as a call to an
  undeclared function.
- **An exception raised inside an `await`ed call was never caught by the
  awaiter's own `try`/`except`** — the exception stack was plain
  thread-local, but `await` runs the callee as a genuinely separate child
  coroutine with no path back to the parent's handler. Fixed by making the
  exception/panic stack a shared, refcounted object inherited by every
  coroutine in a nested-`await` call chain.
- **Windows Fiber cross-boundary `longjmp` crash on exception raise** — the
  fix above still crashed with `STATUS_INVALID_HANDLE` on the Windows Fiber
  backend, since a raw `longjmp` across fiber boundaries leaves Windows'
  own "current fiber" tracking stale. Fixed by routing the unwind through
  the already-safe symmetric `SwitchToFiber` suspend/resume path instead of
  ever jumping directly across a fiber boundary. Verified at 1–3 levels of
  nested-`await` unwind depth.
- Work-stealing multi-core async scheduler correctness fixes.

### Generics / interfaces (six bugs, one investigation pass)
- Parser infinite loop/segfault on generic bounds with type arguments
  (`[C: DbConnection[RS], RS: DbResultSet]`).
- Missing dynamic-dispatch boxing for bare interface return types
  (`-> SomeInterface`) — return statements now box concrete classes into
  the interface's vtable wrapper, matching `let`/parameter sites.
- Mis-codegen for method calls inside monomorphized generic bodies — a
  method call on a generic-bound receiver now resolves its real return
  type instead of falling back to a bare/void call.
- Explicit multi-type-arg generic calls (`do_thing[MyConn, MyRS](c)`) were
  mis-parsed as an index/tuple expression.
- A non-generic class implementing a generic interface with concrete type
  arguments (`class MyConn implements DbConnection[MyRS]:`) had those
  arguments silently discarded, producing the wrong vtable/object shape.
- Implicit (non-bracketed) calls to multi-generic functions now correctly
  infer directly-appearing generics and cross-reference bound generics
  through the receiver's own `implements Iface[Concrete]` declaration.
- Generic classes type mismatch; a symbol-kind resolution bug; a sema
  ordering bug where a module-level `mut`/`let` global could be referenced
  by a function positioned earlier in the same file, before the global's
  own registration pass had reached it.
- Interface-typed field/local assignment (`obj.field = ConcreteClass()`)
  had the same missing vtable-boxing issue as bare interface returns.
- `Dict[K,V].init()`/`.new()` (and the `Map[K,V]` alias) called with no
  arguments failed to compile — defaulted to a capacity hint of 16,
  matching `Set[T].init()`'s existing behavior.
- `await` on a `throws`-declared function tried to cast an aggregate
  `Result`/`Option`/`Tuple` return value to/from a scalar integer; the
  coroutine result channel now heap-allocates room for aggregate returns
  instead of assuming everything fits in a pointer/integer-sized slot.
- `fname[T](args)` with a single explicit type argument blindly used the
  type argument itself as the call's result type instead of looking up the
  function's actual declared return type — correct only when the return
  type genuinely *is* that generic parameter, silently wrong otherwise
  (worse: silently corrupting instead of erroring when reached through
  `await`, since that channel is untyped).
- **`async def` methods declared inside an `extend` block were not
  recognized as async at all** — only async *free functions* worked before
  this fix; an async method's body calling `await` failed with `[C-4]`
  despite being declared `async def`.
- `Map`/`Dict` `.get()`/`.get_or()`/`.set()`/`.free()` checked the raw,
  un-substituted generic type-parameter name instead of resolving it
  through the active monomorphization first, miscasting a pointer directly
  to a non-scalar struct type inside certain monomorphized methods.
- A macro-generated top-level declaration's function body could be silently
  omitted from every generated `.c` file (undefined-reference link error
  despite `--check` passing cleanly) — the resolver's per-module bookkeeping
  was snapshotted before macro expansion ran, so a macro-spliced
  declaration was invisible to it; macro expansion now tracks and attributes
  each newly-generated declaration to its triggering declaration's module.
- The portable-C bootstrap seed was missing the in-process-LLVM shim on
  Windows specifically (`scripts/regen-bootstrap.ps1` never copied it,
  unlike the `.sh` version), so a bootstrap regenerated on Windows failed
  to link with `undefined reference to '_tr_llvm_emit_object'`.

### Ownership / ARC
- **F-1 collection-push leak closed** — `v.push(Box.init(k))` for a
  `Vec[HeapClass]` retained the fresh constructor result instead of moving
  it, leaking the temporary's reference.
- **F-4 Mutex-owned-collection leak closed** — a `Mutex[Coll[..]]` wrapping
  a fresh collection never freed the collection (or its owned elements) on
  drop.
- **F-2 borrow-vs-consume leak closed**, unblocked by a latent `Dict`/`Map`
  runtime bug where `Dict_has` reported any key stored with a `false`/`NULL`
  value as absent — this had been silently hiding the compiler's own
  interprocedural "does this function consume its argument" analysis.
  All four fuzz findings (F-1..F-4) are now leak-free and part of the core
  regression gate.
- A heap-class element read via `list[i]` (an index expression) into
  another list was missing its retain.
- `List[Class]` element *assignment* (`list[i] = obj`) codegen fixed.
- `ThreadLocal[T].get()`'s cast was wrong for class/pointer element types.
- Two real heap-corruption bugs: a strict-aliasing undefined-behavior issue
  that only manifested at `-O2`, and an unsound F-2 auto-drop case.

### Standard library
- **MD5 was wrong for every input** — round-2's `G` function had its `B`
  and `D` operands transposed. Fixed and verified against RFC 4122
  known-answer vectors.
- `TAURARO_HAVE_REGEX` never covered MinGW at all (only Linux/Apple/Unix),
  so `std.regex` silently ran as a no-op stub on every Windows build;
  detection was extended, then further hardened this release (see
  "Changed" and the Windows CI section below) after it turned out the
  header's mere *existence* still couldn't be assumed.
- A parser bug where a keyword used as a method name after `.` or in a
  `def <name>(self, ...)` position was silently mishandled; also, a keyword
  used as a function *parameter* name corrupted parsing with zero
  diagnostic output (this had been silently truncating `std.encoding.yaml`'s
  parser class to its first 5 methods).
- A single-line `case X: stmt1; stmt2` match-arm lowered `stmt2` as if it
  belonged to a separate, always-true arm instead of `X`'s own case body.
- A `(str, Vec[str])` tuple's second (pointer) slot miscompiled as a bare
  `int64` assignment in generated C.
- `std.encoding.yaml`: an anchor (`key: &name`) on a multi-line block value
  registered against an empty/null value instead of the block that
  follows; the serializer wrote a nested sequence at the same indent as its
  key, which the parser's strict-indent block-nesting rule can't read back.
- **`Captures.matched()`/`.len()` dereferenced a null pointer** when
  `std.regex`'s underlying compile had failed (or regex support wasn't
  available at all), crashing with a bounds-check panic instead of honoring
  the module's own documented "safe no-op" contract. Every other
  `Captures` accessor already routed through `.len()`'s bounds check, so
  this one fix closes the gap for all of them.
- `Chan[T]`'s `http_server.tr`: `read_next`/`read_next_into` freed the
  borrowed `""` literal returned on a closed connection as if it were a
  heap allocation, corrupting the heap on the next unrelated allocation.
- A chained `Thread.spawn(fn, arg).detach()` call nested the spawn as the
  receiver of `.detach()`, so the inner spawn's wrapper was never emitted.
- Package resolution didn't search a package's own `src/` subdirectory for
  a `mod.tr`'s sibling imports (the pattern `templa`-style packages use).
- Whole-number float literals (e.g. `7.0`) were emitted into generated C
  without a decimal marker, silently becoming integer division in
  expressions like `7.0 / 2.0`.
- Broken escaped-quote f-strings in `std/test/mod.tr` that caused 39
  cascading parse errors, making `TestRunner` itself unusable.
- Several escape-analysis gaps (`str`/collection locals referenced only
  through an f-string interpolation, closure capture, comprehension,
  slice, or `super` call) that could leave a value eligible for an
  incorrect auto-drop; a `Dict[K,str]`/`Map[K,str]` double-free/UAF when
  the same string was stored under two keys.
- A method call on a receiver naming no defined type or value now produces
  a hard, named `[E-1]` diagnostic instead of a silent bogus free-function
  call and an opaque C compile error.
- `Option.some(x)`/`Result.ok(x)`/`Result.err(x)` dropped their payload
  type when assigned to an unannotated local, mis-resolving a following
  `.unwrap()` call.

### CI / Windows-specific (this release's own stabilization pass)
- `build-termux-android` and the benchmark job no longer get silently
  skipped by an unrelated matrix-leg failure (see "Changed").
- `TAURARO_HAVE_REGEX`'s MinGW branch now checks `__has_include(<regex.h>)`
  instead of assuming the header exists whenever `__MINGW32__`/
  `__MINGW64__` is defined — an earlier version of this guard broke CI
  outright the first time it ran on a MinGW distribution without the
  header.
- Windows `-lsystre -ltre` linking no longer guessed from a filename
  convention (see "Changed").
- **The real, final bug**: even with regex correctly detected as
  unavailable, `Captures.matched()` still crashed instead of reporting "no
  match" (see the standard-library section above) — `std.regex`'s tests now
  detect unavailability via `Regex.available()` and skip cleanly instead of
  reporting spurious failures for a platform limitation outside this
  codebase's control.
- A real Linux-only `MemProf.peak_rss_bytes()` race: it compared against
  `getrusage().ru_maxrss` while `rss_bytes()` reads `/proc/self/statm`'s
  current RSS — two independently-updated kernel counters that can
  transiently disagree right after a burst of allocation. Fixed by folding
  the current reading into the returned peak.
- An over-strict `CpuProfiler` test assertion: statistical sampling can
  legitimately capture zero samples during a short run on a contended CI
  runner; the test now accepts the runtime's own documented "no samples
  captured" outcome as well as a populated report.
- A missing `bootstrap/c/module_tauraro_llvm.c` in the portable-C bootstrap
  seed (and the regen script not copying it), which could regress the
  fixpoint build silently.

---

## 📚 Notes & tips

- **`std.regex` on Windows is now honest about platform limitations.**
  Check `Regex.available()` before relying on match results if you're
  targeting an environment where POSIX `<regex.h>` availability can't be
  guaranteed (this release's own investigation found it isn't guaranteed
  even across different runs of GitHub's `windows-latest` label).
- **`std.prof`'s CPU sampling is best-effort by design** — a short-lived
  program on a busy/virtualized CI runner can legitimately capture zero
  samples; check the report text rather than assuming a nonzero sample
  count.
- Full standard library reference: [`docs/std/README.md`](docs/std/README.md).
  Full compiling/cross-compilation reference, including the new
  `uefi-x64` target and `--prof`: [`docs/lang/22_compiling_and_cross_compilation.md`](docs/lang/22_compiling_and_cross_compilation.md).

## ⚠️ Known limitations

- `std.regex` still requires a POSIX-compatible `<regex.h>` under the
  hood; it is not vendored, so its availability is platform/toolchain
  dependent (gracefully detectable via `Regex.available()`, not silently
  wrong).
- `std.prof`'s CPU sampling captures the leaf program counter only, not a
  full unwound call stack; Windows symbol resolution (`dbghelp`) is not
  wired up, so Windows profiles report raw addresses.
- `std.prof`'s `MemProf` is process-level only — no per-allocation-site
  attribution.

---

## ⬆️ Upgrading

- No source changes required for existing code. New capabilities are
  opt-in (`import std.prof`, `--prof`, `import std.gpu`, `--target
  uefi-x64`, `import std.cli`/`std.log`/`std.encoding.yaml`).
- If you were relying on `std.regex` silently reporting "no match" on a
  platform without real regex support, switch to checking
  `Regex.available()` explicitly — the old silent-stub behavior for
  `Captures.matched()`/`.len()` could previously crash instead in some
  code paths and is now well-defined either way.
- Rebuild from the release SDK as usual; the bundled zig toolchain is
  unchanged in this release.

_`tauraroc --version` reports **v0.0.9**._
