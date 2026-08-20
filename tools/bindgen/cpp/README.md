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

## Complex-header hardening (validated)

The generator is hardened for the features a real C++ API uses, verified end-to-end on
`media.hpp` (overloads, nested classes, inheritance, operators, enums, `std::`, references,
by-value returns — driven from `use_media.tr`) and on libstdc++ `<complex>` (2,751 lines → 90
wrappers, compiles):

- **Overloaded functions/methods/ctors** get a `_2`, `_3`, … suffix (C and Tauraro have no
  overloading) — `encode` / `encode_2`, `Buffer_new` / `Buffer_new_2`.
- **Nested classes** (`Encoder::Stats`) — the receiver is tracked on a class *stack*, so a method
  after a nested class still binds to the right parent, and the shim uses the fully-qualified
  C++ name (`media::Encoder::Stats`).
- **Enums cross by value** as their `c_int` alias (not as an opaque pointer) — scoped `enum class`
  and plain `enum` both.
- **`std::` / template / unknown types** become opaque handles (`std::string` → `std__string`),
  passed by pointer, so the binding compiles; the shim uses the real C++ type.
- **Operator overloads** (`operator=`, `operator==`, …) are skipped (not valid C identifiers, not
  callable by name from FFI).
- **Reserved-word / receiver-colliding parameter names** (`in`, `self`, …) are sanitized on the
  Tauraro side (`in_`); const-ref/const-ptr returns are `const`-cast in the shim.
- **Anonymous enums/classes** (clang spells them `(unnamed enum at …)`) are skipped rather than
  emitting un-parseable output.
- **Referenced-but-undefined external types** are forward-declared opaque so the `.tr` compiles.

## Canonical-type resolution (the walker knows every type's real size/kind)

The walker resolves each type through its **canonical type** (libclang's `clang_getCanonicalType`
+ `getSizeOf` + `getEnumDeclIntegerType` + record layout), and emits a compact descriptor
`ptrdepth~isref~cat~detail` that the generator maps deterministically. This dissolves most of the
"hard tail":

- **Typedef'd scalars are auto-resolved** — `HRESULT`→`long`→`c_long`, `DWORD`→`unsigned long`→
  `c_ulong`, `WORD`→`c_ushort`. The old claim that value typedefs "can't be auto-sized" was wrong:
  libclang gives the exact canonical builtin. A header full of typedef'd scalars now binds with the
  correct ABI, no hand-written shim.
- **POD structs cross by value** — a trivially-copyable struct with public fields becomes a real
  `@value_type` with those fields (from libclang's field layout), so it is passed/returned by value
  and its fields are read directly in Tauraro (`r.w`). See `canon.hpp` (`Rect`).
- **`std::string` is first-class** — a `std::string` parameter accepts a native Tauraro string
  (the shim builds a temporary), and a `std::string` return comes back as a heap C string the
  caller owns. No hand-written accessor. (`canon.hpp` `name()`/`setName()`.)
- **Enums cross by value** as their true underlying integer (`enum class Plain : unsigned char`
  → `c_uchar`), not always `c_int`.

Verified end-to-end (`use_canon.tr`): `bounds.w` reads a by-value struct field, `name()` returns a
native string, `process()` returns a typedef'd `long` with an enum-by-value argument.

## Honest scope / remaining hard tail

The walker uses the *real* Clang parser, so parsing is complete, and canonical resolution handles
scalars/enums/POD-structs/`std::string`. What genuinely remains (the same for every AOT binder):

- **Templates** need explicit instantiation. A `template<class T> class Box` is not a concrete type
  and a bare `typedef Box<int>` does **not** instantiate it, so libclang has no concrete methods to
  bind — you must instantiate it (e.g. `template class Box<int>;`) for the members to exist. This is
  fundamental to AOT binding (SWIG's `%template`, etc.).
- **`std::` containers other than `string`** (`std::vector`, `std::map`, …) cross as opaque handles;
  element access needs a thin accessor.
- **System *record* types from another header** (e.g. Windows COM `comdef.h`'s `_GUID`/`IUnknown`
  by value) still collide with the system definition the runtime pulls in — the *scalars* now bind
  correctly, but by-value system structs are the residual tail; bind those through a narrow shim.
