# Changelog

All notable changes to the Tauraro language and compiler (`tauraroc`) are
documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).
Versioning follows the pre-1.0 policy described in
[docs/dev/00_versioning_policy.md](docs/dev/00_versioning_policy.md): until
1.0, **any** `0.x` bump may contain breaking changes, and there is no
deprecation period.

## [Unreleased]

Work in progress on the production-readiness roadmap (formal ownership/safety
spec, automated regression suite + CI, incremental compilation, and a batch of
codegen-safety / diagnostics / stdlib / tooling improvements). Entries will be
added here as each phase lands.

### Fixed
- Whole-number float literals (e.g. `7.0`) were emitted into generated C
  without a `.`/exponent marker (`%.17g` of `7.0` is `"7"`), so expressions
  like `7.0 / 2.0` silently became integer division (`3` instead of `3.5`).
  Fixed in `_tr_float_to_c_lit` (`runtime/tauraro_rt.h`) by appending `.0`
  when no float marker is present.
- Fixed broken escaped-quote f-strings (`f"... \"...\" ..."`) in
  `std/test/mod.tr` that caused 39 cascading parse errors and made the
  `TestRunner` helper unusable.

- Escape analysis (`mark_escaped_str_args` / `mark_escaped_coll_args` in
  `src/sema.tr`) previously had a silent wildcard `case _: pass` that skipped
  several `HirExpr` variants, so `str`/collection locals referenced only
  through an f-string interpolation, a closure capture, a list/generator
  comprehension, a slice, or a `super` method call were not excluded from
  auto-drop — a potential use-after-free / double-free. All such variants are
  now walked explicitly, and the catch-all is replaced by an exhaustive list
  of leaf/literal variants so a future new `HirExpr` variant is a compile
  error rather than a silent miss.
- Storing the same `str` local under two keys of a `Dict[K,str]`/`Map[K,str]`
  (or storing one that is later auto-dropped) double-freed / use-after-freed
  at dict teardown: `dict_val_arg` boxed the value with `_tr_str_box` but no
  `_tr_str_retain`, so multiple boxes aliased one reference. Dict/Map string
  values now retain on insert (`_tr_str_box(_tr_str_retain(...))`), mirroring
  `List_TrStr_append`, and `mark_escaped_str_args` no longer excludes the
  value arg from auto-drop. Fresh string values (concat/call/method results)
  are hoisted to a temp and released after the set so reference counts stay
  balanced.
- `extern` (`_tr_*` runtime-helper) calls passing a fresh `str` argument
  (concat/call/method result) leaked the temporary `TrStr`: `gen_args_extern`
  extracted `.data` via a bare `_tr_strz()` and discarded the struct. Fresh
  string arguments are now hoisted to a temp and released by the enclosing
  statement's `flush_wraps`, matching `gen_args` for normal calls.

### Changed
- Debug builds (`--debug`) now emit C `#line N "source.tr"` directives mapping
  each generated statement back to its original Tauraro source file and line,
  so GCC diagnostics and GDB backtraces reference the `.tr` source rather than
  the generated C. Line directives are off by default (non-debug output is
  unchanged). Paths are emitted with forward slashes for cross-platform C
  string compatibility.
- Incremental compilation: the build now compiles each module's generated C to
  its own object file (`gcc -c`) and links the objects, reusing the cached `.o`
  for any module whose generated C is byte-identical to the previous build (and
  whose shared headers / compile flags are unchanged). A clean rebuild after
  editing a single module now recompiles only the affected module(s) instead of
  the whole program. The `build/` directory is kept populated between builds as
  the object cache (it is no longer wiped when `-o` is given). Verified
  correctness-preserving: an all-cache-reuse build is byte-identical to a
  from-scratch build.
- Parser diagnostics now report a precise column and a source-code snippet
  with a caret, in the style `file:line:col: error: <msg>` followed by the
  offending source line and a `^` under the exact column, then the `FIX:`
  hint. The lexer tracks per-token columns (`token_cols`) alongside lines;
  the parser is given the source text and renders the snippet. (Column info
  is diagnostic-only and never affects codegen.)

### Added
- `std/encoding/toml.tr`: a TOML parser and serializer (`TomlValue` tagged
  tree + `Toml.parse`/`Toml.stringify`), modeled on `std/encoding/json.tr`.
  Supports comments, bare/quoted keys, basic & literal strings, integers
  (with `_` separators) / floats / booleans, arrays, inline tables, and
  `[table]` / `[a.b.c]` nested table headers; round-trips. (YAML, a regex
  backtracking engine, and other encodings remain deferred.)
- `tauraroc fmt [-w] <file>`: a source formatter that re-emits canonically
  formatted Tauraro, **preserving comments** (the lexer records them out-of-band
  so the parser/codegen are unaffected). Standalone comments stay on their own
  line; trailing comments stay attached to their code line. Binary/unary
  sub-expressions are conservatively parenthesized so the output always
  re-parses to the same AST. The formatter is idempotent (`fmt(fmt(x)) ==
  fmt(x)`, verified on all formattable example files and in CI) and refuses to
  rewrite a file containing a construct it cannot yet render (rather than
  risk corrupting it).
- `tauraroc lint <file>`: runs module resolution + semantic analysis and
  reports warnings/errors without producing an executable.
- CI / `scripts/run_tests.*` now include a formatter idempotency check.
- `tests/lang/` and `tests/regression/` regression test suites (9 files,
  190 assertions) covering core language, collections, strings, classes &
  enums, error handling, concurrency, the #52/#54 string-collection fixes,
  the Phase 4 escape-walker variants, and the Phase 5 Dict-value retain fix,
  using `std/test`'s `TestRunner`.
- `scripts/run_tests.sh` / `scripts/run_tests.ps1` to run the suite via
  `tauraroc --run`.
- CI now runs the regression suite on every platform after building
  `tauraroc` (`.github/workflows/build.yml`).

## [0.0.5] - in development

### Memory model (TrStr migration, items #48-#58)
- Migrated `str`/`String` to a refcounted `TrStr` (`.data`/`.rc`) representation
  across literals, call sites, FFI/extern boundaries, and the standard library
  (`Str`, `StringBuilder`, `Fmt`, JSON/Base64/Hex/URL/DateTime).
- `List[str]` / `Dict[K,str]` / `Set[str]` specializations migrated to
  `List_TrStr` / `Dict_free_strval`, fixing use-after-free and double-free bugs
  in string-valued collections.
- Added collection escape-analysis (`coll_escaped`) and string escape-analysis
  (`str_escaped`) passes in `src/sema.tr` to drive auto-drop (RAII-lite) of
  non-escaping `List`/`Vec`/`Dict`/`Map`/`Set`/`str` locals.
- Added block-id (block-stack) tracking for correct per-branch drop insertion.
- Fixed numerous wrap-hoist leaks (`wrapstr`/`flush_wraps`/`strz`) for fresh
  TrStr temporaries in concat, method calls, and return/assign/condition
  positions.
- Added `.free()`/`.dispose()` to `Pair`, `StrPair`, `Triple`, `HttpHeader`,
  `Url`, `TimeDelta`, `DateTime`, `Date`, `Time`, `HttpConn`, `HttpResponse`,
  `JsonValue`.
- Net effect on the watax HTTP benchmark: residual per-request allocation
  dropped from multi-KB to roughly 50-300B/req with no crashes at 25k+
  requests / concurrency=500.

### Compiler / safety
- Implemented `extern "CPP":` as an ABI synonym for `extern "C":` (real C++
  interop still crosses a C boundary).
- Added `inspect(T)` compile-time reflection (types, functions, classes,
  enums, interfaces, docstrings).
- Added variadic function parameters (`args...`).
- Implemented nested (Java-style) `class`/`def`/`enum`/`interface`/`extend`
  declarations inside `main()`.
- Enforced `unsafe:` blocks around raw-pointer `.write()` calls across
  `std/` (json, map, path, collections/list, resolver, codegen).
- Improved parser error reporting with current-file tracking.

### Ecosystem
- watax (Axum-style web framework): reactor-based concurrency
  (`IOPoll`/epoll/IOCP/kqueue), thread-per-connection and reactor-pool
  listeners, templa template engine with `{% extends %}`/`{% block %}`
  inheritance, static file serving, JSON API helpers, CORS middleware,
  graceful shutdown, `TestClient`.
- taupkg package manager: semver + lockfile, local/git dependencies,
  `cp_r` packaging fixes.
- Full rewrite of `docs/lang/*`, `docs/std/*`, and new `docs/dev/*`
  contributor/internals guide.

### Build
- Fixed bootstrap binary output location (`build.sh`/`build.ps1` now move
  `src/build/tauraroc[.exe]` to the working directory if the compiler still
  places it there).
- Self-locating compiler (`_tr_exe_dir()`) so bare-name PATH invocation finds
  `runtime/`/`std/` relative to the executable.

## [0.0.2] - tagged

- Tagged early bootstrap milestone (see `git tag v0.0.2`).

## [0.0.1] - tagged

- Initial tagged bootstrap milestone (see `git tag v0.0.1`).

[Unreleased]: https://github.com/Yusee-Programmer/tauraro/compare/v0.0.2...HEAD
[0.0.5]: https://github.com/Yusee-Programmer/tauraro/compare/v0.0.2...HEAD
[0.0.2]: https://github.com/Yusee-Programmer/tauraro/releases/tag/v0.0.2
[0.0.1]: https://github.com/Yusee-Programmer/tauraro/releases/tag/v0.0.1
