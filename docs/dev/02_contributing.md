# 02 — Building and Contributing to Tauraro

---

## Prerequisites

- GCC or Clang on `PATH` (the compiler shells out to whichever it detects — see
  `detect_c_compiler` in `src/main.tr`).
- An existing `tauraroc` / `tauraroc.exe` binary to bootstrap from (the repo always
  keeps a working "blessed" binary checked in or available via `taupkg`).

Tauraro has **no separate build system of its own** beyond the compiler itself —
"building Tauraro" means using `tauraroc.exe` to compile `src/main.tr` (the
compiler's own source) into a new `tauraroc.exe`.

> Note: this repo is *not* a Cargo/Rust project — despite some historical session
> notes referring to "cargo bootstrap", the day-to-day workflow described below
> (compiler compiles itself via `tauraroc.exe src/main.tr -o ...`) is the current
> and verified process. If you find a `Cargo.toml`-based bootstrap path, treat it as
> a legacy/CI-seed fallback, not the primary workflow.

---

## The Bootstrap (Self-Hosting) Build

The compiler is written in Tauraro and compiles itself. A change to `src/*.tr`
isn't "real" until it survives this cycle:

```bash
# Stage 1: current tauraroc.exe compiles the modified source -> new binary
./tauraroc.exe src/main.tr -o tauraroc_new.exe

# Stage 2 (gen1 -> gen2): the NEW binary compiles the SAME source again
./tauraroc_new.exe src/main.tr -o tauraroc_gen2.exe

# Stage 3 (gen2 -> gen3): repeat once more
./tauraroc_gen2.exe src/main.tr -o tauraroc_gen3.exe
```

**Fixpoint check:** `tauraroc_gen2.exe` and `tauraroc_gen3.exe` should be **byte-
identical in size** (and ideally identical output for `--emit c` on a sample
program). If gen1→gen2 changes behavior but gen2→gen3 is stable, the new feature
works but didn't exist in the binary that compiled it yet (expected for the *first*
build of a new feature). If gen2→gen3 still differs, something is non-deterministic
or broken — do not bless.

```bash
# Compare sizes (PowerShell)
(Get-Item tauraroc_gen2.exe).Length
(Get-Item tauraroc_gen3.exe).Length
```

---

## "Never Patch Generated C Files"

`build/*.c`, `build/include/**/*.c`, `build/tauraro_types.h` are **always
regenerated** by `CGenerator` on every compile. Any hand-edit to a file under
`build/` is silently overwritten the next time `tauraroc` runs.

**Rule:** every fix goes into the `.tr` source (`src/`, `std/`, `core/`) or the
hand-written runtime header (`runtime/tauraro_rt.h`), then you rebuild. If you find
yourself editing a `.c` file to test a fix, that's a signal to find the codegen
function in `src/codegen/c.tr` that emits that pattern and fix it there instead.

This also applies to GCC-crash workarounds — e.g. `pragma GCC optimize(...)` in
generated C triggers an implicit-LTO crash on GCC 15.2/MinGW; the fix is to never
emit that pragma from `c.tr`, not to strip it from `build/*.c` after the fact.

---

## Runtime Header Sync Rule

`runtime/tauraro_rt.h` is the **canonical, hand-written runtime** (allocator macros,
`TrStr` refcounting, `_TrIOPoll`, collection helpers, etc.). It is the single source
of truth.

- `main.tr`'s `read_runtime_header` searches, in order: `tauraro/runtime/tauraro_rt.h`
  (repo-relative), next to the compiler binary, `<bin>/runtime/`, then paths relative
  to the input source file, finally CWD.
- After every compile, `sync_headers_to_runtime` writes the *active* runtime header
  back to `tauraro/runtime/tauraro_rt.h` — so editing the copy that ships next to a
  binary and recompiling will propagate back to the canonical copy. **But the
  intended direction is: edit `runtime/tauraro_rt.h` directly, then rebuild.**
- If a packaged install exists at `~/.taupkg/bin/tauraroc-windows-x64/` (this
  machine has one), it has its own `runtime/tauraro_rt.h` and `std/` copies — these
  are a **separate, packaged distribution** used by `taupkg`-installed projects
  (e.g. watax, templa). After blessing a new `tauraroc.exe` and/or runtime header,
  copy the updated `tauraroc.exe`, `runtime/`, and `std/` into that directory too if
  external projects need the fix. (Path layout observed:
  `~/.taupkg/bin/tauraroc-windows-x64/{tauraroc.exe, runtime/, std/}`.)

---

## Validation Checklist Before "Blessing" a New Binary

"Blessing" = replacing the repo's `tauraroc.exe` with a newly built one and treating
it as the new baseline for future bootstraps.

1. **Self-host fixpoint** — gen1→gen2→gen3 as above; gen2 and gen3 must match
   (identical size at minimum).
2. **Full example suite** — every file in `examples/*.tr` compiles and runs without
   error or crash:
   ```bash
   for f in examples/*.tr; do
       ./tauraroc_new.exe "$f" -o /tmp/ex.exe && /tmp/ex.exe || echo "FAIL: $f"
   done
   ```
   (PowerShell equivalent: loop with `Get-ChildItem examples\*.tr`.)
   All 48 examples should pass with no errors, segfaults, or ASAN/UBSAN traps.
3. **Targeted regression tests** — if your change touches memory management,
   re-run any existing leak/stress examples (e.g. the audit examples referenced in
   recent commits: collection-escape, Dict/Map str-value release, class-field
   release on auto-drop) and confirm memory growth is at or below the previous
   baseline.
4. **Bless** — once 1–3 pass, copy the new binary over the old:
   ```bash
   cp tauraroc_gen2.exe tauraroc.exe
   ```
   Re-sync `runtime/tauraro_rt.h` if it changed (see above), and update any
   packaged copies under `~/.taupkg/bin/` if external projects depend on this fix.

---

## Running a Single Example

```bash
./tauraroc.exe examples/05_strings.tr -o /tmp/test.exe && /tmp/test.exe
```

Useful variants while developing:

```bash
# Inspect generated C without compiling
./tauraroc.exe examples/05_strings.tr --emit c
# -> writes build/ (main.c, tauraro_types.h, tauraro_rt.h, module_*.c)

# Type-check only, no codegen
./tauraroc.exe examples/05_strings.tr --check

# Compile and run in one step (no .exe kept)
./tauraroc.exe --run examples/05_strings.tr

# Debug build: ASAN + bounds-check assertions, useful for memory bugs
./tauraroc.exe --debug -o /tmp/test_dbg.exe examples/05_strings.tr && /tmp/test_dbg.exe
```

On Windows (PowerShell), paths and the `&&`-equivalent differ — see the CLI
reference in `docs/lang/01_intro.md`.

---

## Commit Conventions

There is no `CONTRIBUTING.md` in this repo yet; conventions are inferred from
`git log --oneline`. Observed style:

- **Imperative, descriptive sentences**, often starting with a verb: "Enhance
  memory management by...", "Fix...", "Add...", "Refactor...".
- Commits frequently describe the **why** (which leak/bug class) and the
  **what** (which files/functions changed), sometimes in a single long sentence
  for substantial multi-part fixes.
- Multi-step session work is often summarized as a numbered list inside one
  commit message (see e.g. `cd1e52b`), rather than split into many tiny commits.
- No enforced prefix convention (no `feat:`/`fix:` scopes observed) — plain
  English description is the norm.

Example from recent history:
```
Enhance memory management by introducing _tr_empty_heap_str for safe empty
string handling and updating string-related functions to prevent
use-after-free issues.
```

**Do not commit automatically** — per project convention, the user commits changes
manually unless they explicitly ask you to create a commit.

---

Previous: [How the Compiler Works ←](01_architecture.md) · Next: [Memory Model Internals →](03_memory_model_internals.md)
