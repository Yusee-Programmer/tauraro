# tauraro-bindgen

Generate Tauraro FFI bindings from a C header — the leverage tool that unlocks binding C
libraries (SDL, GLFW, raylib, sqlite, miniaudio, …) wholesale instead of by hand.

**It's built into the compiler** as a subcommand:

```sh
tauraroc bindgen raylib.h -o raylib.tr [--cc <compiler>]
```

Then `from raylib import ...` in your program. (The compiler auto-detects the C compiler for
preprocessing, so `--cc` is usually unnecessary.)

> This source (`bindgen.tr`) is the standalone copy of the same logic that lives in
> `src/bindgen.tr`; you can also build it directly with `tauraroc tools/bindgen/bindgen.tr -o
> tauraro-bindgen` if you want the tool as a separate binary.

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

**Handles:**
- Functions (incl. struct-by-value, inline function-pointer params, `va_list`), structs
  (`@value_type`), opaque handles, forward-declared opaque structs, pointer typedefs, enums (incl.
  complex/computed values, which are skipped), primitive/fn-pointer typedefs, int/hex/string
  `#define` constants, the full `c_*` (incl. `<stdint.h>`) width mapping, and storage-class /
  calling-convention / `__declspec(…)` / `__attribute__((…))` stripping.
- **Local vs system includes** — declarations from `#include`d **system** headers (flag `3` in
  `cc -E` markers) are dropped, while a library's own **local** headers (zlib's `zconf.h`) are
  bound. Type/struct names are de-duplicated.
- **`#define` allowlist** — only macros `#define`d in the target header's own text are emitted.
- **Symbol-collision skip-list** — a symbol the Tauraro runtime already declares (libc `printf`/
  `malloc`/`qsort`; Win32 `CreateWindow`) is skipped rather than re-declared, so a library header
  that also declares one compiles without a `conflicting types` error.

**Follow-ups (Phase 1b):**
- **Local-`#include` `#define`s** — currently only the top header's own `#define`s become
  constants; macros defined in a library's *own* sub-headers aren't yet emitted.
- **Function-like `#define` macros** are skipped (they aren't symbols); emit inline wrappers.
- Big libraries with a machine-readable registry (Vulkan XML, GTK GIR) are better generated
  from that registry than from the header.

> **Windows path note:** pass the header (and `-I` dirs) as **Windows paths** (`C:/…`), not MSYS
> `/c/…` paths — the tool shells out to the compiler via `cmd.exe`, which doesn't understand
> `/c/…`. Relative paths work fine.
