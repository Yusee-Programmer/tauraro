# 24 — Bindgen: automatic C and C++ bindings

`tauraroc bindgen` reads a C or C++ header and writes a ready-to-use Tauraro binding module — you
never hand-write `extern` declarations for a whole library. It comes in two modes:

| Mode | Command | For |
|------|---------|-----|
| **C** (default) | `tauraroc bindgen foo.h -o foo.tr` | C headers — direct `extern "C"` bindings |
| **C++** | `tauraroc bindgen foo.hpp -o foo.tr -h cpp` | C++ headers — auto-generates a C++→C shim via libclang |

The generated module records how to build and link the library, so once it exists you just
`import` it and compile your program — **no manual `--link`, no `-lstdc++`, no build script.**

> **Reverse direction:** to call *Tauraro from C++*, see [`--export-cpp`](#calling-tauraro-from-c-export-cpp)
> below and the [FFI chapter](17_extern_and_ffi.md).

---

## Quick start (C library)

```sh
tauraroc bindgen /usr/include/zlib.h -o zlib.tr --pkg zlib   # discovers -I… and -lz
```
```python
from zlib import crc32

def main() -> int:
    mut buf: [c_uchar; 5]
    buf[0] = 104 as c_uchar   # 'h'
    # …fill the rest…
    print("crc = " + crc32(0, buf.as_ptr(), 5).to_str())
    return 0
```
```sh
tauraroc app.tr -o app        # zlib is auto-linked — no --link, no -lz
```

That's the whole workflow: **bind once, then `import` + build.**

---

## `--pkg`: auto-discover a library's flags

Most C/C++ libraries ship a `pkg-config` file. `--pkg <name>` queries it for the compile flags
(`-I`/`-D`, used to parse the header) and the link flags (`-L`/`-l`, recorded in the binding), so you
never hand-pass them:

```sh
tauraroc bindgen sqlite3.h -o sqlite3.tr --pkg sqlite3
tauraroc bindgen /usr/include/cairo/cairo.h -o cairo.tr -h cpp --pkg cairo
```

If `pkg-config` doesn't know the package, bindgen warns and you fall back to explicit flags:

```sh
tauraroc bindgen mylib.h -o mylib.tr -I/opt/mylib/include -DMYLIB_STATIC
```

`--pkg` works on **both** the C and C++ paths.

---

## Auto-linking (zero build step, zero-cost)

The generated `.tr` carries machine-readable header pragmas:

```python
# tauraro-cpp-shim: foo_shim.cpp        # (C++ only) the generated shim to compile
# tauraro-cpp-lib: stdc++               # (C++ only) libraries to link
# tauraro-cpp-linkflags: -L… -lfoo      # (--pkg) link flags, appended verbatim
# tauraro-cpp-cflags: -std=c++17 -I…    # (C++ only) flags to compile the shim
```

When you `import` such a module, `tauraroc` reads these, compiles the shim once (cached), and adds
everything to the link line for you. On a **gcc / `zig cc`** toolchain the whole program is built with
`-flto`, so the C++→C shim wrapper is **inlined away** — a call into C++ costs exactly what the C++
call costs, with no shim indirection.

- Opt out with `--no-auto-cpp` (then link `foo_shim.o -lstdc++` yourself).
- On a bare `clang` host without lld, the shim is still auto-linked; only the LTO inlining is skipped.
- You still link the C++ *library itself* (via `--pkg`, `-lfoo`, or `--link libfoo.a`); only the
  generated shim is automatic.

---

## Binding C++ headers (`-h cpp`)

C++ has no stable C-callable ABI (name mangling, `this`, vtables, templates, RAII, exceptions), so
`-h cpp` uses **libclang** to walk the header and emit a `foo_shim.cpp` (`extern "C"` wrappers around
the real C++ API) next to `foo.tr`. libclang is required **only** for `-h cpp`; if it's missing, the
tool prints per-platform install guidance.

Point it at a **specific** header, not an umbrella:

```sh
# GOOD — a concrete header
tauraroc bindgen /usr/include/wx-3.2/wx/button.h -o wxbutton.tr -h cpp \
    -I/usr/lib/wx/include/gtk3-unicode-3.2 -I/usr/include/wx-3.2 -DWXUSINGDLL

# BAD — umbrella headers often parse to zero bindable declarations
tauraroc bindgen wx/wx.h -o wx.tr -h cpp …
```

On Windows/MSYS, add `--cc clang++` so libclang uses clang's own intrinsic headers (GCC's
`ia32intrin.h` isn't parseable by libclang):

```sh
tauraroc bindgen wx/button.h -o wxbutton.tr -h cpp --cc clang++ -I… -D__WXMSW__ -DWXUSINGDLL -D_UNICODE
```

### What the C++ binder covers

- **Classes** — opaque handles (`geo::Shape*` ↔ `Shape`): construct with `<pfx>_new(...)`, call
  `<pfx>_method(obj, ...)`, free with `<pfx>_delete(obj)`.
- **Methods** (static + `const`), **constructors/destructors**, **inherited public methods** (transitive,
  with base-qualified calls to defeat name-hiding and ambiguous multiple inheritance).
- **Virtual dispatch** — a factory returning `Base*` dispatches to the right override.
- **Operator overloads** — `operator+`/`==`/`[]`/`*`/`()`/… bind under clean names (`op_add`, `op_eq`,
  `op_index`, …).
- **Enums** (with values), **namespaces**, **global variables** & **static class constants** (as
  zero-arg value accessors).
- **Public data members** — become read-accessors (makes `std::pair`/`std::tuple` fields usable).
- **Default arguments** — one wrapper per trailing-default arity.
- **Free-function templates** of a single type parameter — instantiated for `int`/`double`.
- **Exceptions** — every wrapper is exception-safe: a C++ exception is caught, recorded, and turned into
  a zero return; read it with `tauraro_cpp_last_error() as str` / clear with `tauraro_cpp_clear_error()`.
  A throwing function never crashes the caller.

### Standard-library type mapping

| C++ | Tauraro binding side | Notes |
|-----|----------------------|-------|
| `std::string`, `std::string_view` | native `str` | auto-converts both directions |
| `std::vector<T>`, `std::deque<T>`, `std::array<T,N>` | element by index (`at`/`[]`/`.len`) | forced instantiation |
| `std::map`/`unordered_map<K,V>` | `_key_nth(i)` / `_val_nth(i)` enumeration | |
| `std::set`/`list`/`forward_list`/`unordered_set` | `_nth(i)` index accessor | via `begin()`+`advance` |
| `std::pair`, `std::tuple` | `_first`/`_second` / member accessors | |
| `std::shared_ptr`/`unique_ptr`/`weak_ptr<T>` | `get()` → `Pointer[T]` | pointee is usable |
| `std::function<R(Args)>` | `Pointer[void]` | pass a Tauraro `def` — see below |
| function pointer / callback + `void*` | `Pointer[void]` | see below |

### Passing a callback / `std::function`

A `std::function<R(Args)>` (or a raw function-pointer) parameter binds as `Pointer[void]`. Pass a
**top-level** Tauraro `def` with `as Pointer[void]`:

```python
def dbl(x: int) -> int:
    return x * 2

# C++ side: int g_apply(std::function<int(int)> f, int x)
g_apply(dbl as Pointer[void], 20)     # C++ calls back into dbl
```

The shim casts the pointer to the matching function type, which C++ constructs the `std::function`
from. (Capturing closures with user-data are supported through the raw callback + `void*` pattern —
see [Callback Functions](17_extern_and_ffi.md#callback-functions-function-pointers).)

---

## Calling Tauraro from C++ (`--export-cpp`)

The mirror image: expose a Tauraro module to C++.

```sh
tauraroc mylib.tr --export-cpp -o mylib   # -> mylib.dll/.so + mylib.h (C) + mylib.hpp (C++)
```

The `.hpp` is **self-contained** (no runtime header needed). Each `export def` becomes an inline
wrapper in `namespace <libname>`; Tauraro classes become **C++ RAII wrappers** (ARC → copy=retain,
dtor=release); and collections marshal to the standard library:

| Tauraro | C++ |
|---------|-----|
| `str` | `std::string` |
| `List[T]` (scalar/str/class element) | `std::vector<T>` |
| `Dict[K,V]` (str/int key, int/float/bool/str value) | `std::map<K,V>` |
| `(T0, T1, …)` tuple | `std::tuple<…>` |
| a heap class | a C++ RAII wrapper class |

```cpp
#include "mylib.hpp"
mylib::Counter c(10);          // constructor (calls the static factory `init`)
c.bump(5);                     // method
auto v = mylib::nums();        // List[int] -> std::vector<long long>
```
```sh
g++ app.cpp mylib.dll -o app
```

Full detail in the [FFI chapter](17_extern_and_ffi.md#calling-from-c-export-cpp).

---

## Best practices

1. **Prefer `--pkg`.** If the library has a pkg-config file, `--pkg <name>` removes all guesswork
   about include and link flags — and makes the binding portable to any machine that has the library.
2. **Bind a specific header, not an umbrella.** `foo/foo_all.h` often expands to thousands of
   declarations and parses to nothing bindable; point at the concrete header you need.
3. **Regenerate, don't hand-edit.** Treat the generated `.tr`/`_shim.cpp` as build artifacts. If the
   library upgrades, re-run bindgen. (Check the `.tr` into source control if you want reproducible
   builds without libclang on every machine.)
4. **On Windows/MSYS use `--cc clang++`** for C++ headers so libclang gets clang's own resource
   headers.
5. **C string returns are borrowed.** A C function returning `const char*` (a string the library owns,
   e.g. `zlibVersion()`) binds as `Pointer[char]`; `ptr as str` gives a **borrowed** view (safe, never
   freed). If *you* own a freshly-malloc'd buffer, adopt it with `_tr_str_wrap(ptr)` instead.
6. **Handle C++ exceptions.** After any wrapper call that might throw, check
   `tauraro_cpp_last_error() as str` — a non-empty message means the call was caught and returned a
   zero value.
7. **Use `c_*` types at the ABI boundary.** `int`/`long`/`size_t` are platform-dependent in C; bind
   them with `c_int`/`c_long`/`c_size_t` so the width always matches. See
   [FFI type mapping](17_extern_and_ffi.md).
8. **Keep the library on the link line.** `--pkg`/`-lfoo`/`--link libfoo.a` — the generated shim is
   auto-linked, but the actual C/C++ library is yours to provide.

---

## Troubleshooting

| Symptom | Cause / fix |
|---------|-------------|
| `no bindable declarations found` | Umbrella header, or missing `-I`/`-D` so the parse failed. Point at a concrete header; add `--pkg` or the include dirs. |
| `libclang could not fully parse the header` | Missing include dirs/macros. The tool prints the diagnostics — add the `-I`/`-D` (or `--pkg`) it needs. |
| C++ shim won't compile | A construct the binder can't yet express (deep SFINAE, variadic templates, a non-bindable overload). The function is omitted; bind a simpler entry point or wrap it in a small `extern "C"` shim of your own. |
| `undefined reference` at link time | The *library* isn't linked. Add `--pkg <name>`, `-lfoo`, or `--link libfoo.a`. |
| libclang not found | Install it (the tool prints per-platform guidance). Needed **only** for `-h cpp`. |

---

## Coverage & limitations (measured)

Bindgen is exercised against a broad set of real, installed library headers — see the published
**[stress-test matrix](../../tools/bindgen/STRESS_MATRIX.md)**: **70 libraries, 69 PASS, 37,097 wrappers
bound.** That includes big real-world APIs — GLib/GIO/GObject (8,400+), Pango (3,235), ATK (2,607),
OpenSSL (1,200+), SQLite, libcurl, libxml2, Cairo, HarfBuzz, FreeType, FontConfig, libpng/webp/jpeg/tiff,
GMP/MPFR — plus the C standard library and 12 **wxWidgets** C++ widget headers whose shims compile clean.

It is a generated-C-shim approach, so it doesn't express *every* C++ construct. The one library that
only partially binds in the matrix is **gmpxx** — GMP's `__gmp_expr` **expression templates** (lazy CRTP
proxy types) can't cross a C ABI, which is a wall every C-shim binder (SWIG, `cxx`) hits. Other known
gaps: deeply SFINAE-heavy / variadic-template APIs, some overload sets, `std::function` **returns**
(params work), and nested collection element types in `--export-cpp`. Anything it can't bind is cleanly
**omitted** (the rest of the header still binds), so a partial bind is always usable.

> **Pick the right public header.** Several "misses" are really "wrong header": `libtiff` binds via
> `tiffio.h` (not the internal `tiff.h`), `freetype` via `freetype/freetype.h` (not the `ft2build.h`
> macro shim). Point bindgen at the library's documented public entry header.

---

Next: [Casting with `as` →](23_casting_with_as.md)
