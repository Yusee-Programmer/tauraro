# Versioning & Stability Policy

This document defines how Tauraro is versioned, what "breaking change" means
at this stage of the project, and how changes should be recorded.

## Where the version lives

The single source of truth for the compiler's version string is
`print_version()` in `src/main.tr`:

```tauraro
pub def print_version():
    print("tauraroc v0.0.5")
```

`README.md` carries a matching version badge. Both must be updated together
when the version is bumped (the `chore: update version to vX.Y.Z` commits in
git history are the precedent for this).

## Pre-1.0 policy (current)

Tauraro is **pre-1.0** (`0.x.y`). Per common pre-1.0 convention (and explicitly
*not* full SemVer yet, since SemVer reserves `0.x` for "anything may change"):

- **Any `0.x` version bump may contain breaking changes** to the language
  grammar, standard library APIs, generated-code ABI (`TrStr`, runtime
  helpers in `runtime/tauraro_rt.h`), CLI flags, or `taupkg` manifest format.
  There is **no deprecation period** during 0.x.
- We still distinguish three kinds of `0.x.y` bumps by convention, to help
  readers gauge risk, but none of them carry a stability *guarantee*:
  - **`x` (minor, e.g. 0.4 → 0.5)**: a body of related work landed (a memory
    model migration, a new subsystem, a stdlib area). Expect breaking changes.
  - **`y` (patch, e.g. 0.5.0 → 0.5.1)**: bug fixes / internal-only changes,
    not expected to break user code, but not guaranteed (pre-1.0).
  - Tags (`v0.0.1`, `v0.0.2`, ...) mark points where the bootstrap/self-host
    fixpoint was verified and a release binary was built — see
    `docs/dev/02_contributing.md` for the blessing checklist.
- Generated C / runtime ABI (`TrStr`, `_tr_*` helpers) is considered
  **internal** and may change in any `0.x` release. External FFI code that
  pokes at `TrStr` internals directly (rather than going through `extern "C":`
  function signatures) is unsupported.

## What "1.0" will mean

Tauraro reaches 1.0 once:
1. The ownership/safety model (see `docs/lang/13_memory_and_ownership.md`,
   `docs/lang/26_safety_model.md`, and the error-code reference in
   `docs/lang/19_compiler_errors.md`) is complete and documented as the
   canonical spec — not discovered bug-by-bug.
2. The automated regression suite (`tests/`, see
   `docs/dev/02_contributing.md`) covers the language surface and runs in CI
   on all supported platforms.
3. Standard library module APIs are reviewed for stability and the ones
   intended to be stable are marked as such.

After 1.0, standard SemVer applies: `MAJOR.MINOR.PATCH`, breaking changes
require a `MAJOR` bump, deprecation warnings precede removals by at least one
`MINOR` release.

## Recording changes

Every user-visible change (language feature, stdlib API, CLI flag, codegen
behavior affecting generated binaries, bug fix with observable effect) gets an
entry in `CHANGELOG.md` under `## [Unreleased]`, in the appropriate
Keep-a-Changelog category (`Added`/`Changed`/`Fixed`/`Removed`). When a version
is cut, `[Unreleased]` is renamed to the new version with a date, and a fresh
empty `[Unreleased]` section is added above it.

Internal-only refactors with no observable effect on language users (e.g.
codegen leak fixes that don't change program output, just memory behavior) may
still get a `CHANGELOG.md` entry under a `### Memory model` / `### Compiler`
heading if they're significant — see the `[0.0.5]` entry for the style.

## Error codes (M-x/T-x/etc.)

The compiler's diagnostic codes (`M-1`..`M-8`, `T-1`..`T-5`, `N-1`, `F-1`..`F-3`,
`E-1`/`E-2`, `S-1`, `I-1`..`I-3`, `L-1`, `P-1`, `U-1`) are documented in
`docs/lang/19_compiler_errors.md`. Adding a new code or changing what an
existing code means is a breaking change for tooling that pattern-matches on
codes (e.g. editor integrations) and must be noted in `CHANGELOG.md`.
