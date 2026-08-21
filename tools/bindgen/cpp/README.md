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
- **Operator overloads** bind under clean wrapper names (`operator+` → `op_add`, `operator==` →
  `op_eq`, `operator[]` → `op_index`, `operator*` → `op_deref`/`op_mul`, …) — see the dedicated
  section below. The shim still calls the real `operator@`. A handful (copy/move assignment,
  address-of, conversion operators, `new`/`delete`) are skipped.
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

## Templates & std:: containers — automated via forced instantiation

A `template<class T> class Box` has no concrete methods until instantiated, so a naive walk skips it.
Instead of requiring the user to hand-write `template class Box<int>;`, the bindgen does it
**automatically**:

1. A first walk collects every template specialization that appears in the API (any type spelling
   with `<…>` — `Box<int>`, `std::vector<int>`).
2. It generates a synthetic TU (`#include "hdr"` + `template class Box<int>;` per spec) and does a
   second `--inst` walk. libclang now exposes each instantiation.
3. For each, the walker binds the primary template's methods with the type parameters **substituted
   at the CXType level** (a `T`/`value_type`/`type-parameter-0-i` type is replaced by the concrete
   arg, keeping pointer/ref depth), so `T get()` → `int get()`, `const T& ref()` → `Pointer[c_int]`.
   Methods whose signature still carries an unresolved dependent type (`vector<_Tp,_Alloc>` self-copy
   ctor, `at()`'s internal `__alloc_traits<…>` reference) are skipped, so the shim always compiles.

Verified end-to-end: a user template (`use_box.tr`: `Box<int>` — ctor/get/set, `makeBox` returns
`Box<int>`) and **`std::vector<int>`** (`use_vec.tr`: a C++ function returns a vector; Tauraro reads
`size()` and elements via `data()`). This automates both the template and the `std::` container tail.

### Sequence-container element accessors resolve

`at()`/`front()`/`back()`/`operator[]` on `vector`/`string`/`deque`/`list` return a `reference`
(`value_type&`) that libstdc++ routes through an `__alloc_traits<…>` typedef which stays dependent.
The walker recognizes the `::reference`/`::const_reference`/`::pointer` member-typedef suffix and
resolves it to the container's **first template argument** (its element type), so those accessors
bind and return `Pointer[<elem>]`. Verified: `at(3)` on a `std::vector<int>` returns the element `9`.

### System / COM record types no longer conflict

A record type the runtime's system headers already define (Windows COM `_GUID`, `IUnknown`,
`IDispatch`, `VARIANT`, `RECT`, …) is now referenced **bare** instead of getting a conflicting
opaque `class _GUID` (`struct _GUID` redefinition). `comdef.h` — the Windows COM smart-pointer
header — now compiles: its typedef'd scalars resolve canonically and its COM object types resolve
against the real `windows.h` definitions.

### Associative containers (`std::map`/`set`) bind too

`getSpecializedCursorTemplate` can return a forward *declaration* of the container's primary
template (true for `std::map`/`set`), which has no member cursors — hence the earlier "0 methods".
The walker now takes `clang_getCursorDefinition(...)` first, so the real definition's methods are
visited (49 for `std::map<int,double>`). Combined with `type-parameter-0-i` substitution and the
member-typedef suffix resolver (`::reference`→`value_type&`, `::size_type`→`size_t`), the useful
accessors bind: `at`/`size`/`empty`/`count`/`clear`. A `const key_type&` key to a *primitive* is
crossed **by value** (`at(m, 2)`, not `at(m, &2)`) since a const-ref-to-primitive is just the value.
Verified end-to-end (`use_map.tr`): `at(m, 2)` → `19.5`, `size` → `3`.

### By-value structs from a filtered sub-header are laid out

A library that defines a value struct in a *sub-header* (e.g. `Vec3` in `lib/math3d.hpp`, used by
value in the main header) used to cross as an opaque handle, because the walker only bound the exact
main file. Now, when an **external non-system** record is used **by value**, the walker extracts its
exact field layout via libclang (`clang_getTypeDeclaration` + field iteration, arrays included) and
emits it as a `@value_type`, so it crosses by value with the correct ABI. Verified (`use_geo.tr`):
a Tauraro program builds a `Vec3`, passes it by value to `add(Vec3, Vec3)`, and reads the returned
`Vec3` fields (`sum.x = 11`). **System** records (`windows.h`'s `_GUID`, …) are deliberately left on
the bare-reference path (`clang_Location_isInSystemHeader`), so `comdef.h` is unaffected.

### Inherited methods are bound (class hierarchies)

A derived class now exposes its **public base-class methods**, not just its own — the shim calls them
through the derived pointer (public inheritance upcasts). This is transitive (a `Leaf : Mid : Base`
binds `Base`'s methods too) and deduplicated (an override wins over the inherited version; diamond
bases aren't bound twice). Verified (`use_inherit.tr`): `Leaf` binds `id`/`kind` (from `Base`, two
levels up), `midv` (from `Mid`), and its own `leafv` — all callable. This closes the single biggest
gap for real-world C++ headers, which are overwhelmingly class hierarchies.

### Smart pointers expose `get()`

`std::shared_ptr<T>` / `unique_ptr` / `weak_ptr` now bind a `get()` that returns the pointee `T*`
(`Pointer[T]`). `get()` is inherited from a *template* base (`__shared_ptr<T,…>`) and returns a
metafunction-dependent typedef libclang won't reduce, so it's **synthesized** directly from the smart
pointer's first template argument. Verified (`use_smartptr.tr`): a `shared_ptr<Thing>` is returned
from C++, `get()` yields a `Pointer[Thing]`, and the pointee's field is read (`Thing.v = 42`).

### Typedef'd template specializations are instantiated

A `typedef Ring<int> IntRing;` (or `using`) that no function references now force-instantiates
`Ring<int>` and binds its methods (`Ring_int__push/pop/size`). This picks up template types that
appear only behind a typedef. (Extreme std template headers like `<sstream>`/`<random>` still bind
little — their instantiations are enormous and error-prone; that's a std-specific edge, not a
user-header limitation.)

### Default arguments generate overloads

A method/ctor/free function with trailing default arguments — `int area(int scale=2, int bias=5)`
— now emits **one wrapper per callable arity**, from full down to the shortest legal call, so the
C++ defaults are honoured. `area` binds as `W_area(o,scale,bias)`, `W_area_2(o,scale)` (bias=5),
and `W_area_3(o)` (scale=2, bias=5). The walker counts trailing parameters that carry a default
(anything past the `TypeRef`/`ParmDecl` skeleton), and the generator loops arity from full to
`full-ndefault`, truncating the parameter list each pass. Verified (`use_defaultargs.tr`):
`37 / 35 / 25`. Real `std::vector` picks this up automatically — its count/value/allocator ctors
now expose the shorter forms too.

### Operator overloads bind under named wrappers

C++ operator overloads — pervasive in idiomatic headers (math/geometry types, comparisons,
indexing, smart-pointer deref, functors) — now bind. Each `operator@` maps to a clean identifier
and the shim calls the real operator via explicit syntax (`self->operator+(o)`):

| C++ | wrapper | C++ | wrapper | C++ | wrapper |
|-----|---------|-----|---------|-----|---------|
| `a+b` | `op_add` | `a==b` | `op_eq` | `a[i]` | `op_index` |
| `a-b` | `op_sub` | `a!=b` | `op_ne` | `a()` | `op_call` |
| `a*b` | `op_mul` | `a<b` | `op_lt` | `*a` (unary) | `op_deref` |
| `a/b` | `op_div` | `a<=b` | `op_le` | `a->` | `op_arrow` |
| `-a` (unary) | `op_neg` | `a+=b` | `op_iadd` | `++a` | `op_inc` |

(plus `op_mod`, `op_gt`/`op_ge`, `op_isub`/`op_imul`/`op_idiv`, `op_lshift`/`op_rshift`,
`op_bitand`/`op_bitor`/`op_xor`/`op_bitnot`, `op_land`/`op_lor`/`op_lnot`, `op_dec`). Unary vs
binary `*`/`+`/`-` are disambiguated by arity (member: 0 params = unary). **Skipped:** copy/move
assignment (`operator=` — aliasing/self-ref), unary address-of, conversion operators
(`operator bool`), and `operator new`/`delete`. Verified end-to-end (`use_operators.tr`): a `Vec2`
with `+ - -(unary) [] == < +=` all compute correctly through the FFI.

### Public data members become read-accessors (incl. `std::pair`/`std::tuple`)

A class/struct that has **public data members** but isn't a trivially-copyable POD (it has a
constructor, virtual, etc.) can't cross by value — yet its fields used to be invisible (only its
methods bound). Each public field now generates a read-accessor wrapper `<pfx>_<field>(obj) ->
<fieldtype>` (shim: `return self->field;`). This is extremely common in real library types (a
`Config`/`Point`-style class with public fields *and* helper methods). Verified (`use_fields.tr`):
a non-POD `Point{int x,y; double weight; Point(...); int sum();}` exposes `Point_x/_y/_weight`
alongside `Point_sum`.

Crucially this also makes **`std::pair`/`std::tuple` returns usable**: the forced-instantiation
walker now recognizes `struct` specializations (not just `class`) and emits their fields, so
`std::pair<A,B>` binds `pair_first(obj)->A` / `pair_second(obj)->B`. Verified (`use_pairtuple.tr`):
a function returning `std::pair<int,int>` and one returning `std::pair<int,double>` are both read
correctly (`first`/`second`, mixed element types). A POD value-struct still crosses by value with
direct field access (unchanged — the accessor path is only for non-POD/opaque handles).

### Node-based containers get an index accessor (`_nth`) for iteration

A node-based container (`std::list`/`forward_list`/`set`/`multiset`/`unordered_set`/`unordered_multiset`)
has no `at()`/`operator[]`, and its iterators can't be exposed directly (a template iterator's
`operator++` returns a dependent self-type; `==`/`!=` are free namespace-scope templates). Instead the
instantiation walker detects `begin()`+`end()` and emits an `_nth(obj, i) -> <elem>` accessor whose
shim uses **`auto`+`std::advance`** — side-stepping the un-nameable iterator type entirely:

```
mut lst = make_list()
mut i = 0
while i < list_size(lst):        # size() is bound
    use(list_nth(lst, i))        # synthesized: auto it=begin(); advance(it,i); return *it;
    i = i + 1
```

Verified (`use_iterate.tr`): a returned `std::list<int>` (size 3, sum 60) and `std::set<int>`
(`nth(0)=5`, `nth(2)=25`) both traverse correctly. The element type is the container's first template
arg, classified through libclang canonical types (so `list<Widget>` yields a handle, `list<string>` a
`char*`, etc.). Random-access containers (`vector`/`deque`) are deliberately excluded — they already
bind `at(i)`/`operator[]`.

**Associative maps enumerate via two accessors.** `std::map`/`multimap`/`unordered_map`/
`unordered_multimap` have an element type of `pair<const K,V>`, so the walker emits BOTH the key and
value types and the generator synthesizes `_key_nth(i) -> K` (`it->first`) and `_val_nth(i) -> V`
(`it->second`), again via `auto`+`std::advance`. With the bound `size()` this walks a whole map across
the FFI:

```
mut i = 0
while i < map_size(m):
    use_entry(map_key_nth(m, i), map_val_nth(m, i))
    i = i + 1
```

Verified (`use_mapiter.tr`): a `std::map<int,double>` enumerates `1→1.5, 2→2.5, 3→3.5` and a
`std::map<std::string,int>` enumerates `apple→10, pear→20` (string keys via `key_nth() as str`). This
completes container coverage — every standard container is now enumerable (`vector`/`deque`/`array` by
index, `list`/`set`/`unordered_set` via `_nth`, maps via `_key_nth`/`_val_nth`).

### Exception safety — a throwing C++ function never crashes the caller

A C++ exception propagating through an `extern "C"` boundary is **undefined behavior** — in practice it
`std::terminate`s the whole process. Since most real C++ APIs throw (`std::stoi`, `vector::at`,
`map::at` on a missing key, stream/filesystem ops, validating constructors), **every** generated wrapper
body is enclosed in `try { … } catch (const std::exception& e) { … } catch (...) { … }`. On an
exception the wrapper records the message in a thread-local and returns a zero/default value (`return
{};`), so the caller keeps running. Two accessors are always emitted:

```
tauraro_cpp_clear_error()                      # reset before a call you want to check
mut r = risky(-1)                              # throws in C++ -> caught, returns 0 (no crash)
mut msg = tauraro_cpp_last_error() as str      # "" if fine, else the exception message
if msg != "":  handle_error(msg)
```

`try/catch` is zero-cost on the happy path (table-based unwinding — no overhead unless an exception
actually fires). Verified (`use_exceptions.tr`): `risky(-1)` throws `std::runtime_error("negative
input")`, the wrapper returns 0 with `tauraro_cpp_last_error()` = `"negative input"`, and the next call
`risky(7)=14` still works — the process never crashes.

### Callbacks & `void*` — function-pointer and opaque-pointer parameters

Callback-based APIs (event handlers, comparators, visitors, allocators) and opaque `void*` data are
pervasive in real headers. Both now bind correctly:

- A **`void*`** parameter (userdata, buffer, opaque handle) crosses as `Pointer[void]` (pass
  `0 as Pointer[void]` for null). `const void*` too.
- A **function-pointer** parameter — including one hidden behind a `typedef` (`typedef int
  (*Transform)(int, void*)`) — crosses as `Pointer[void]`, and the shim C-casts it back to the exact
  function-pointer type so the C++ call is well-typed. (Two fixes made this work: the walker now
  resolves a typedef-to-pointer to its canonical pointee — otherwise the callback classified as an
  empty/unknown type and produced a non-compiling shim — and the generator builds a proper
  `RET (*)(ARGS)` cast.)

Verified end-to-end (`use_callback.tr`): a Tauraro `def dbl(x, user)` is passed as the callback
(`dbl as Pointer[void]`), invoked by the C++ `apply`, and returns `42`. So a Tauraro function can be a
C callback across the FFI.

### Global variables & static class constants get value accessors

A namespace-scope global (`extern const int VERSION;`, `const double PI = 3.14159;`) and a public
**static class constant** (`static const int MAX = 100;`) each get a zero-arg **value accessor**
`<pfx>_<name>() -> <type>` whose shim simply `return`s the (possibly `extern`-linked) value. Using an
accessor rather than baking in a literal means it works for `extern` constants defined in a `.cpp`, for
runtime-initialized globals, and for non-scalar types (returned as a handle) — no compile-time value
needed. Verified (`use_globals.tr`): `cfg_VERSION()` = 7 (an `extern` global defined in the `.cpp`),
`cfg_PI()` = 3.14159, and `cfg_Limits_MAX()` = 100 (a static class constant). The accessors are
exception-safe like every other wrapper.

## Honest scope / remaining hard tail

- **Free-function templates** (`template<class T> T max(T,T)`) aren't auto-instantiated (unlike class
  templates, they have no usage site to key on) — add a typedef/alias to force one.
- **By-value *system* structs** (`windows.h`'s `_GUID` passed by value) — pointer-based COM works.
- **By-value *system* structs** (`windows.h`'s `_GUID`/`VARIANT` passed *by value*, not by pointer)
  are not laid out — that would need renaming the emitted struct to avoid redefining the one the
  runtime's system headers already provide, and would risk the working COM pointer handling for a
  rare case. Pointer-based COM (the norm) works; by-value *library* structs (above) now work too.
