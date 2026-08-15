# tauraro-bindgen

Generate Tauraro FFI bindings from a C header — the leverage tool that unlocks binding C
libraries (SDL, GLFW, raylib, sqlite, miniaudio, …) wholesale instead of by hand.

```sh
# build the tool
tauraroc tools/bindgen/bindgen.tr -o tauraro-bindgen

# generate bindings
./tauraro-bindgen raylib.h -o raylib.tr --cc gcc
```

Then `from raylib import ...` in your program.

## How it works

1. **Preprocess** with the C compiler (`cc -E`, flattening `#include`s and expanding macros) —
   no hand-rolled C preprocessor. `cc -E -dM` on an empty file gives the compiler/platform
   built-in macros, which are subtracted so only the library's own `#define`s survive.
2. **Tokenize** the flattened declarations.
3. **Parse** the common C shapes and **emit** Tauraro:

| C | Tauraro |
|---|---|
| `RET name(params);` | `extern "C": def name(...) -> RET` |
| `typedef struct { fields } T;` | `@value_type class T:` with fields (structs pass by value) |
| `typedef struct T T;` (opaque) | `class T: pass` + used as `Pointer[T]` |
| `typedef enum { A=1, B } E;` | `type E = c_int` + `const A/B` (auto-incrementing) |
| `typedef void (*Fn)(...)` | `type Fn = Pointer[void]` (C function pointer) |
| `typedef BASE T;` | `type T = <BASE>` |
| `#define K 42` / `#define S "x"` | `const K: c_int = 42` / `const S = "x"` (hex supported) |
| `int`/`unsigned`/`long`/… | the ABI-exact `c_*` family |
| `T*` / `char*` / `void*` | `Pointer[T]` / `Pointer[char]` / `Pointer[void]` |

Struct-by-value (small **and** large), callbacks, opaque handles, and the `c_*` widths are all
handled by the compiler's FFI (see `docs/lang/17_extern_and_ffi.md`), so the generated bindings
compile, link, and run against the real library.

## Status

Validated end-to-end on **two real production libraries** — generated, compiled, linked, and run
with correct results:

- **zlib** (`zlib.h`, 1938 lines: local `zconf.h`, opaque structs, function pointers, pointer
  typedefs, forward declarations, ~80 functions, `Z_*` constants) → linked against `libz` →
  `crc32(0,"hello",5)` = 907060870, `compressBound(1000)` = 1013.
- **sqlite3** (`sqlite3.h`, 13,775 lines: 292 functions, 32 types, callbacks, `va_list`, inline
  function-pointer params) → linked against `libsqlite3` → `sqlite3_libversion_number()` = 3050004.

Also verified struct-by-value returns, enums, and `#define` constants on synthetic headers.

> Reading a C-returned **borrowed** `const char*` (e.g. `zlibVersion()`) with `x as str` is
> unsafe — it wraps a string the library owns as a Tauraro-owned string and frees it on drop.
> Copy it instead. This is a general FFI ownership rule, not a bindgen issue.

**Done:**
- Functions, structs (`@value_type`), opaque handles, enums, primitive/fn-pointer typedefs,
  int/hex/string `#define` constants, the `c_*` (incl. `<stdint.h>`) width mapping.
- **Multi-file headers** — declarations from `#include`d system headers are filtered out via
  `cc -E` line markers (matched by the target header's basename), so a real header that pulls in
  `<stdint.h>`/`<stddef.h>` binds only *its own* symbols.
- **`#define` allowlist** — only macros `#define`d in the target header's own text are emitted
  (constants from included system headers, e.g. `INT32_MAX`, don't leak in).

**Follow-ups (Phase 1b):**
- **System-symbol collisions.** A generated symbol can clash with one the Tauraro runtime's
  system headers already declare (libc `printf`/`qsort`; Win32 `CreateWindow`). A skip-list of
  runtime-provided symbols is the fix.
- **Local `#include`s.** Macros/types from a library's *own* sub-headers (not `<system>` ones)
  should be included; currently only the top header's own `#define`s are emitted.
- **Function-like `#define` macros** are skipped (they aren't symbols); emit inline wrappers.
- **Inline function-pointer parameters** (`void (*cb)(int)`) are emitted as `Pointer[void]`.
- Big libraries with a machine-readable registry (Vulkan XML, GTK GIR) are better generated
  from that registry than from the header.
