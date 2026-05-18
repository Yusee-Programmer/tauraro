# Bootstrapping the Tauraro Compiler

This document explains how to go from the Rust bootstrap compiler to a fully
self-hosted Tauraro compiler that can compile itself.

## What "self-hosting" means here

The self-hosted compiler (`compiler/src/main.tr`) is the Tauraro compiler written
entirely in Tauraro. To run it for the first time we need an existing compiler —
the Rust bootstrap compiler (`src/` in the repo root). Once the self-hosted compiler
can compile itself, the Rust compiler is no longer required for day-to-day use.

## Prerequisites

- Rust toolchain (`cargo`, `rustc`) — only needed for Stage 0
- GCC ≥ 9 or Clang ≥ 10 on your PATH
- The repository cloned locally

## Stage 0 — Build the Rust bootstrap compiler

```bash
cargo build --release
```

This produces `target/release/tauraroc` (or `target\release\tauraroc.exe` on Windows).
All Stage 1+ steps use this binary.

Verify it works:

```bash
cargo run -- --check examples/hello.tr
# should print: "Semantic analysis OK" or similar
```

## Stage 1 — Compile the self-hosted compiler to C

Use the Rust bootstrap to compile `compiler/src/main.tr` (which imports all other
`compiler/src/*.tr` modules transitively):

```bash
cargo run --release -- compiler/src/main.tr --emit c -o compiler_bootstrap.c
```

This produces `compiler_bootstrap.c` — a single amalgamated C file containing the
entire self-hosted compiler.

Verify it looks right:

```bash
# Should contain struct definitions, vtables, snprintf calls, etc.
grep -c "typedef struct" compiler_bootstrap.c   # expect > 20
grep "tauraro_rt.h"      compiler_bootstrap.c   # expect 1 match
```

## Stage 2 — Compile the C to a native executable

```bash
gcc -O3 -o tauraroc compiler_bootstrap.c
# or with Clang:
clang -O3 -o tauraroc compiler_bootstrap.c
```

On Windows with MSVC (if GCC is not available):

```powershell
cl /O2 /Fe:tauraroc.exe compiler_bootstrap.c
```

Verify the binary runs:

```bash
./tauraroc --help
./tauraroc examples/hello.tr --run
# should print: Hello, World!
```

## Stage 3 — Self-host: compile the compiler with itself

Now use the Stage 2 binary to compile the self-hosted compiler source again:

```bash
./tauraroc compiler/src/main.tr --emit c -o compiler_stage3.c
gcc -O3 -o tauraroc_stage3 compiler_stage3.c
```

Compare outputs to verify fixpoint:

```bash
diff compiler_bootstrap.c compiler_stage3.c
# Should be identical (or differ only in timestamp comments)
```

If the diff is empty (or only trivial whitespace), the compiler has reached a
**fixpoint** — it produces the same output whether compiled by the Rust bootstrap
or by itself. This is the self-hosting milestone.

## Stage 4 — Replace the bootstrap

Once Stage 3 passes, rename/symlink:

```bash
cp tauraroc_stage3 tauraroc
# The Rust bootstrap is no longer needed for compilation.
```

You can now compile any `.tr` file with the fully self-hosted compiler:

```bash
./tauraroc myprogram.tr --run
./tauraroc myprogram.tr --emit c -o myprogram.c
./tauraroc myprogram.tr -O3 -o myprogram
```

## Troubleshooting

### "module not found" during Stage 1

The module resolver looks for `.tr` files relative to the entry file's directory.
Make sure the full compiler source tree is present:

```
compiler/
  src/
    main.tr  token.tr  lexer.tr  ast.tr  parser.tr
    sema.tr  hir.tr  resolver.tr
    codegen/
      mod.tr  c.tr  llvm.tr
```

### Compilation errors in the generated C

If `gcc` reports errors compiling `compiler_bootstrap.c`:

1. Check that `runtime/tauraro_rt.h` is in a directory GCC can find, or pass
   `-I runtime/` to the GCC command.
2. Look for `undefined reference` to a function — the forward declaration phase
   may be missing a prototype. File an issue with the failing function name.
3. `-Wall` warnings about `__auto_type` require GCC ≥ 4.9. Use a newer GCC.

### Stage 3 diff is not empty

A non-empty diff usually means the compiler is not fully deterministic. Common causes:

- Temporary variable names include a counter that is not reset between runs.
  Check `CGenerator.tmp_counter` in `codegen/c.tr`.
- String escaping differs between runs. Check `_escape_str_for_c()`.

Run both binaries on a minimal test case:

```bash
./tauraroc_bootstrap examples/hello.tr --emit c > a.c
./tauraroc_stage3    examples/hello.tr --emit c > b.c
diff a.c b.c
```

Narrow down which construct produces different output and fix the generator.

## Continuous Bootstrap CI (recommended setup)

Add these steps to your CI pipeline to keep self-hosting intact:

```yaml
- name: Build Rust bootstrap
  run: cargo build --release

- name: Stage 1 — emit C
  run: cargo run --release -- compiler/src/main.tr --emit c -o s1.c

- name: Stage 2 — native binary
  run: gcc -O3 -o tauraroc_s2 s1.c

- name: Stage 3 — re-emit C
  run: ./tauraroc_s2 compiler/src/main.tr --emit c -o s3.c

- name: Fixpoint check
  run: diff s1.c s3.c

- name: Run integration tests
  run: ./tauraroc_s2 compiler/tests/self_host_test.tr --run
```

## Milestone Checklist

- [ ] Stage 0: `cargo build --release` succeeds
- [ ] Stage 1: `compiler_bootstrap.c` generated without errors
- [ ] Stage 2: `./tauraroc examples/hello.tr --run` prints "Hello, World!"
- [ ] Stage 2: all integration tests pass (`./tauraroc compiler/tests/self_host_test.tr --run`)
- [ ] Stage 3: `diff compiler_bootstrap.c compiler_stage3.c` is empty
- [ ] **Self-hosting achieved** — Rust bootstrap retired for compilation tasks
