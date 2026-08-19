# C++ bindings (`tauraroc bindgen -h cpp`) — design (Path B, libclang)

## Why a shim is unavoidable (and why that's fine)

C++ has no stable C-callable ABI: methods are **name-mangled** (`Widget::area() const` →
`_ZNK2ui6Widget4areaEv`), carry an implicit `this`, use vtables, templates, RAII, and exceptions
— none of which cross a C ABI. So you can't bind a C++ header the way you bind a C header. Every
language bridges C++ through a C shim; the goal of `-h cpp` is that **the bindgen writes the shim
for you** so you never hand-write it.

## Architecture (validated end-to-end on this machine)

`tauraroc bindgen foo.hpp -h cpp -o foo.tr` will:

1. **Detect libclang** (see below). If absent → print per-platform install guidance and stop.
   libclang is needed *only* for `-h cpp`; C headers never touch it.
2. **Generate + compile a tiny clang-c AST walker** (`cxxwalk.c`, ~40 lines) against the detected
   libclang, run it on the header, and read a flat **IR** of the public API.
3. **Emit two files** from the IR: `foo_shim.cpp` (`extern "C"` wrappers) and `foo.tr` (opaque
   handles + wrapper declarations), reusing the existing C-path type mapping + collision handling.
4. You compile the shim once (`c++ -c foo_shim.cpp`) and link it — the one extra step you'd do for
   any C++ library. You wrote zero shim code.

**Proven:** `cxxwalk.c` compiles against the local libclang and already extracts the full public
API — classes, constructors/destructors, methods (with `static`/`const` flags), parameters (type +
name), free functions, enums (with values), namespaces, **public-members-only** — as a clean IR:

```
NS geo
ENUM Kind
EVAL Circle 0
EVAL Square 2
EVAL Triangle 3
EENUM
CLASS Shape
CTOR
PARAM double|x
PARAM double|y
ECTOR
DTOR
METHOD .c double|area        # .c = const, s. = static
METHOD .. void|move
PARAM double|dx
PARAM double|dy
METHOD s. Shape *|unit
ECLASS
FUNC double|distance
PARAM const Shape *|a
PARAM const Shape *|b
```

Generated shim (target):
```cpp
#include "shapes.hpp"
extern "C" {
  geo::Shape* geo_Shape_new(double x, double y) { return new geo::Shape(x, y); }
  void        geo_Shape_delete(geo::Shape* self){ delete self; }
  double      geo_Shape_area(const geo::Shape* self){ return self->area(); }
  void        geo_Shape_move(geo::Shape* self,double dx,double dy){ self->move(dx,dy); }
  geo::Shape* geo_Shape_unit(){ return geo::Shape::unit(); }
  double      geo_distance(const geo::Shape* a,const geo::Shape* b){ return geo::distance(a,b); }
}
```
Generated `.tr` (opaque handle + wrappers): `class Shape: pass` + `extern "C": def geo_Shape_new(...)
-> Pointer[Shape]`, etc.

## Complex headers: adaptive include discovery + diagnostics

Real-world C++ headers (`wx/wx.h`, Qt, …) pull in library and platform headers that live behind
`-I` paths and require feature `-D` macros. The bindgen is **adaptive, not prescriptive**:

- **Compiler-queried include paths.** libclang may be built against a *different* toolchain than
  your `cc`, so its builtin header search can miss `<string>`/`<vector>`. Before walking, the tool
  runs `cc -x c++ -E -v` on an empty file and parses the `#include <...> search starts here:` block,
  feeding every directory to libclang as `-I`. This resolves the common case out of the box on
  Linux/macOS/Windows, for both gcc and clang.
- **Passthrough flags.** Add the library's own paths/macros — they are forwarded to libclang:
  ```sh
  tauraroc bindgen wx/wx.h -h cpp -o wx.tr \
      -I/usr/include/wx-3.2 -I/usr/lib/wx/include/gtk3-unicode-3.2 -DWXUSINGDLL
  ```
  Accepted: `-I<dir>` / `-I <dir>`, `-D<macro>` / `-D <macro>`, `-isystem <dir>`, `-std=…`, `-f…`.
- **Diagnostics instead of silent failure.** The walker reports libclang's error/fatal diagnostics.
  If a header can't be fully parsed (e.g. a missing include), the tool prints the exact
  `fatal error: '…' file not found` and tells you to re-run with the needed `-I`, rather than
  silently emitting *"0 classes, 0 wrappers"*. If some bindings are produced despite a fatal error,
  it warns that they may be incomplete.

## libclang detection (cross-platform)

Detect by **test-compiling a probe** (`#include <clang-c/Index.h>` + `clang_createIndex`); if
`cc probe.c -lclang` links, libclang's headers *and* lib are both usable. Fallback path search:

- **Windows:** `C:\Program Files\LLVM\{bin,lib,include}`, MSYS2 `mingw64/{bin,lib,include}`, PATH.
- **Linux:** `/usr/lib/llvm-*/`, `/usr/lib/x86_64-linux-gnu/`, `llvm-config --libdir/--includedir`.
- **macOS:** `$(brew --prefix llvm)/`, Xcode's `usr/lib/libclang.dylib`.

## Install guidance (printed when libclang is missing)

- **Windows:** `winget install LLVM.LLVM` — or the LLVM installer from
  <https://github.com/llvm/llvm-project/releases> — or MSYS2 `pacman -S mingw-w64-x86_64-clang`.
- **Debian/Ubuntu:** `sudo apt install libclang-dev`
- **Fedora:** `sudo dnf install clang-devel`
- **Arch:** `sudo pacman -S clang`
- **macOS:** `brew install llvm` (or Xcode Command Line Tools).

Size: `libclang` ≈ 32 MB (+ `libLLVM` ≈ 130 MB when dynamically linked) or ≈ 80–120 MB
self-contained. It is **not** bundled — `-h cpp` uses a system-installed libclang, detected at use
time. The bundled zig does **not** expose libclang.

## Implementation status — DONE (validated end-to-end)

- ✅ Architecture proven: walker compiles against libclang, extracts the full public API as a
  clean IR (`cxxwalk.c` here; embedded in the compiler as a string, emitted + compiled on demand).
- ✅ Tauraro-side generator (`_cpp_generate` in `src/bindgen.tr`): parses the IR → emits the shim
  (`extern "C"` wrappers with namespace-qualified calls) + the `.tr` bindings. Type mapping covers
  primitives (the C-path `c_*` family) and class handles; references/by-value classes are bridged
  via shim deref / heap-copy (`new`).
- ✅ Detection + install-guidance (`_detect_libclang`): test-compiles a probe with `-lclang`; on
  failure prints per-platform install steps and stops.
- ✅ `-h cpp` / `--cpp` flag in the `bindgen` subcommand (default stays C).

**End-to-end proof** (`shapes.hpp` here, driven from `use_shapes.tr`): a Tauraro program constructs a
real `geo::Shape`, calls a `const` method (`area`→12), a mutating method (`move`→ area 20), a `static`
method (`unit`→ area 1), a free function (`distance`→21), and the destructor — all through the
generated shim:

```sh
tauraroc bindgen shapes.hpp -o shapes.tr -h cpp     # -> shapes.tr + shapes_shim.cpp
c++ -c shapes.cpp -o shapes.o                       # your C++ library (here, the fixture impl)
c++ -c shapes_shim.cpp -o shapes_shim.o             # the generated shim
tauraroc use_shapes.tr --link shapes.o --link shapes_shim.o -lstdc++ -o use_shapes
```

### Modeling notes (how the bridge stays correct + ARC-safe)

- **Opaque handles** are the bare Tauraro class (`Shape`), *not* `Pointer[Shape]`: a Tauraro class
  value is already one C pointer, so `Shape` == C `Shape*` (what the shim returns/takes). An empty
  `class Shape: pass` obtained from FFI is not ARC-managed (no drop is inserted), so handles are not
  double-freed. `Shape**` would map to `Pointer[Shape]`.
- The opaque class is emitted with a **multiline `pass` body** (not `class Shape: pass`) — the inline
  single-line form makes the parser swallow the following top-level `extern` block.
- The instance-method receiver is named **`obj`**, never `self` — a param literally named `self` is
  treated as an implicit method receiver and dropped from the emitted C prototype.

## Honest scope

The walker uses the *real* Clang parser, so parsing is complete. The **generator** will cover the
high-value cases first — classes, methods, ctors/dtors, static methods, free functions, enums,
namespaces, primitive + pointer params. The genuinely hard tail (templates needing explicit
instantiation, `std::` containers in signatures, operator overloads, multiple/virtual inheritance,
exception translation) is the same tail every C++ binding tool faces and is added incrementally.
