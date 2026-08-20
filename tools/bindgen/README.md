# tauraro-bindgen

Generate Tauraro FFI bindings from a C header — the leverage tool that unlocks binding C
libraries (SDL, GLFW, raylib, sqlite, miniaudio, …) wholesale instead of by hand.

**It's built into the compiler** as a subcommand:

```sh
tauraroc bindgen raylib.h -o raylib.tr [--cc <compiler>]
```

Then `from raylib import ...` in your program. (The compiler auto-detects the C compiler for
preprocessing, so `--cc` is usually unnecessary.)

**C++ headers** — add `-h cpp` (default is C):

```sh
tauraroc bindgen widget.hpp -o widget.tr -h cpp     # -> widget.tr + widget_shim.cpp
```

C++ has no stable C-callable ABI (name mangling, `this`, vtables, RAII), so the bindgen
**auto-generates a C++→C shim** (`<out>_shim.cpp`) alongside the `.tr` — classes/methods/ctors/dtors/
static methods/free functions/enums/namespaces. Compile the shim once (`c++ -c widget_shim.cpp`) and
link it (`--link widget_shim.o -lstdc++`). This mode uses **libclang**, needed *only* for `-h cpp`
(C headers never touch it); if it's missing the tool prints per-platform install steps. See
[`cpp/README.md`](cpp/README.md) for the design + the `shapes.hpp` end-to-end example.

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

Verified on **six large production headers** — all generate bindings that **compile**, and the
linked ones **run** with correct results:

| Header | Size | Extracted | Compiles | Linked call |
|---|---|---|---|---|
| **sqlite3.h** | 14,349 ln | 32 types, 298 fns | ✅ | `sqlite3_libversion_number()` = 3053004 |
| **cairo.h** | 3,348 ln | 20 types, 345 fns | ✅ | — |
| **curl.h** | 3,347 ln | 22 types, 100 fns | ✅ | `curl_easy_init()` ok |
| **libpng16/png.h** | 3,618 ln | 13 types, 276 fns | ✅ | `png_access_version_number()` = 10658 |
| **zlib.h** | 2,057 ln | 4 types, 89 fns | ✅ | `compressBound(1000)` = 1013 |
| **GL/gl.h** | 1,044 ln | 336 fns | ✅ | — |

Getting these to compile drove several parser hardenings that any complex real header needs:

- **Parenthesized/attribute-wrapped declarators** — `RET (__attribute__((__cdecl__)) name)(args)`
  (libpng's `PNG_EXPORT`). Without this, png bound **0** functions; now 276.
- **Nested anonymous struct/union fields** — flattened with a `field_` prefix (curl's
  `curl_fileinfo.strings`), instead of leaking members into the parent (duplicate-member C error).
- **Function *definitions* in headers** (`static inline`, curl's `typecheck-gcc.h` warning stubs)
  are skipped — they're never linkable externs.
- **`__attribute__`/`__declspec` on enumerators and fields** are stripped.
- **`#define` values that aren't a single clean literal** (string concatenation, macro references,
  arithmetic) are skipped rather than emitted as invalid Tauraro.
- **Referenced-but-undefined types** (defined in a filtered system/sub-header — `png_error_ptr`,
  `struct sockaddr`) are forward-declared as opaque `type X = Pointer[void]`, so the binding
  compiles. Correct for pointer-passed / function-pointer types; by-value layout is approximate.
- Scalar **type-alias returns** (`type uLong = c_ulong`; `compressBound() -> uLong`) resolve so
  `.to_str()`/arithmetic work without a manual cast (compiler-side alias resolution).

> A typedef defined in a filtered *sub-header* is bound opaque (`Pointer[void]`); this is correct
> for handle/callback types but a scalar typedef so bound needs an `as int`/`as c_ulong` cast if you
> do arithmetic on it directly. This is the inherent "hard tail" of header binding.

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
