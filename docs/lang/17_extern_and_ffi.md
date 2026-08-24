# 17 — Extern and C Interop (FFI)

---

## Overview

Tauraro compiles to C, which means the entire C library ecosystem is directly accessible. FFI (Foreign Function Interface) lets you call:

- Standard C library functions (`malloc`, `printf`, `fopen`, ...)
- System calls via libc wrappers (`read`, `write`, `mmap`, ...)
- Third-party C libraries (`libcurl`, `OpenSSL`, `SDL2`, `SQLite`, ...)
- Operating system APIs (Win32, POSIX, ...)
- Any native library with a C interface

**Generating bindings automatically.** *For a full guide with best practices, `--pkg` auto-discovery,
the C++ type-mapping table, and troubleshooting, see **[Chapter 24 — Bindgen](24_bindgen.md)**.*
You don't have to hand-write `extern "C"` declarations for
a whole library — `tauraroc bindgen <header.h> -o <out.tr>` reads a C header and generates the
Tauraro bindings (functions, `@value_type` structs, enums, typedefs, and constants). It filters out
`#include`d system headers, maps C types to the `c_*` family, handles struct-by-value and function
pointers, and resolves symbol collisions (skips libc names the runtime already declares, renames
type names that clash with Win32 like `Rectangle`). Then just `from <out> import ...`. See
`tools/bindgen/` and its graphics example.

**Binding C++ headers.** Add `-h cpp` (default is C): `tauraroc bindgen <header.hpp> -o <out.tr> -h
cpp`. Because C++ has no stable C-callable ABI (name mangling, `this`, vtables, RAII), the bindgen
**auto-generates a C++→C shim** (`<out>_shim.cpp`) alongside the `.tr` — covering classes, methods,
constructors/destructors, static methods, free functions, enums, and namespaces. Opaque C++ objects cross
as an opaque Tauraro class handle (e.g. `geo::Shape*` ↔ `Shape`); construct with `<pfx>_new(...)`,
call `<pfx>_method(obj, ...)`, and free with `<pfx>_delete(obj)`. This mode uses **libclang**, which
is needed *only* for `-h cpp` (C headers never use it); if it is not installed the tool prints
per-platform install guidance. For a library that needs its own headers/macros, forward them —
`tauraroc bindgen wx/wx.h -h cpp -I<dir> -D<MACRO>` (the tool also auto-discovers the compiler's own
C++ include paths, and reports libclang's diagnostics with a fix hint instead of silently binding
nothing). See `tools/bindgen/cpp/` for the design and a full `shapes.hpp` end-to-end example.
`std::string_view` parameters and returns are marshalled to/from a native `str`, just like `std::string`.

**Callbacks — `std::function<R(Args)>` parameters** accept a plain Tauraro function: bind the param as
`Pointer[void]` and pass a top-level `def` with `myfn as Pointer[void]`. The shim casts it to the
matching function pointer, which C++ constructs the `std::function` from:

```python
def dbl(x: int) -> int: return x * 2
g_apply_fn(dbl as Pointer[void], 20)     # C++ calls back into dbl -> 41
```

**Auto-discover a library's flags with `--pkg`.** Instead of hand-passing `-I`/`-D`/`-l`, add
`--pkg <name>` and the bindgen queries **pkg-config** for the library's compile flags (used to parse
the header) and link flags (recorded in the binding so `tauraroc` auto-links them). The whole workflow
becomes:

```sh
tauraroc bindgen /usr/include/zlib.h -o zlib.tr --pkg zlib   # discovers -I… and -lz
tauraroc app.tr -o app                                       # from zlib import … — auto-links zlib
```

No `--link`, no `-lz`. Verified end-to-end: a program computing `crc32("hello")` via the auto-bound,
auto-linked zlib runs correctly. `--pkg` works on both the C and C++ (`-h cpp`) paths.

**Zero-cost, zero-build-step linking.** You don't compile or link the shim by hand. The generated
binding records the shim in machine-readable header pragmas (`# tauraro-cpp-shim:` / `-cflags:` /
`-lib:`), and when you `import` the module `tauraroc` **auto-compiles the shim and links it (plus
`-lstdc++`) for you** — just build your program. On a gcc / `zig cc` toolchain the whole program is
built with `-flto`, so the `extern "C"` wrapper is **inlined away at link time**: a call into C++
costs exactly what the C++ method costs, with no shim indirection (unlike a hand-linked `.o`). You
still link the C++ *library* itself (`--link foo.o` or `-lfoo`) — only the generated shim is
automatic. Opt out with `--no-auto-cpp` (then link `<out>_shim.o -lstdc++` manually). On a bare
`clang` host without lld the shim is still auto-linked; only the LTO inlining is skipped.

**C type mapping:**

| Tauraro type | C type |
|---|---|
| `int` | `int64_t` |
| `i32` | `int32_t` |
| `u32` | `uint32_t` |
| `usize` | `size_t` (platform word size) |
| `float` | `double` |
| `f32` | `float` |
| `char` | `char` |
| `str` | `const char*` |
| `bool` | `bool` |
| `Pointer[T]` | `T*` |
| `Pointer[void]` | `void*` |
| `lambda` | Function pointer |

**C string returns are safe by default.** A C function returning `const char*` (a string the library
still owns — `zlibVersion()`, `sqlite3_errmsg()`, `getenv()`, …) binds as `-> Pointer[char]`; converting
it with `ptr as str` produces a **borrowed** view (never freed), so it can't double-free the library's
string. If you own a freshly-allocated buffer and want it freed together with the string, adopt it
explicitly with `_tr_str_wrap(ptr)` instead of `as str`.

**ABI-exact scalars (`c_*`).** `int`/`i64` are *fixed-width*; C's `int`/`long` are
platform-dependent. When a header uses `int`/`long`/`size_t`, bind them with the `c_*` family
so the width always matches the C ABI:

| Tauraro | C | | Tauraro | C |
|---|---|---|---|---|
| `c_char` | `char` | | `c_uint` | `unsigned int` |
| `c_schar` | `signed char` | | `c_long` | `long` |
| `c_uchar` | `unsigned char` | | `c_ulong` | `unsigned long` |
| `c_short` | `short` | | `c_longlong` | `long long` |
| `c_ushort` | `unsigned short` | | `c_size_t` | `size_t` |
| `c_int` | `int` | | `c_ssize_t` | `ssize_t` |
| `c_float`/`c_double`/`c_ldouble` | `float`/`double`/`long double` | | `c_int32_t` … `c_uint64_t` | fixed-width `<stdint.h>` |
| `c_void` | `void` | | `c_void_ptr` / `RawPtr` | `void*` |
| `c_wchar` | `wchar_t` | | `c_char16` / `c_char32` | `char16_t` / `char32_t` |

---

## Matching a C struct's memory layout

To pass a struct by value across FFI, declare a `@value_type` class whose fields match the C
struct, and control the layout with decorators / field types:

```python
@value_type
@packed                     # __attribute__((packed)) — no padding
class TcpSeg:
    pub src_port: u16
    pub dst_port: u16
    pub seq:      u32        # sizeof == 9, not 12

@value_type
@aligned(16)                # __attribute__((aligned(16))) — SIMD / cache line / DMA
class Vec4:
    pub x: f32
    pub y: f32
    pub z: f32
    pub w: f32

@union                      # overlapping views of the same bytes (implies @value_type)
class Pixel:
    pub rgba:  u32
    pub bytes: [u8; 4]      # sizeof == 4

@value_type
class GpioMode:
    pub mode:  Bits[u32; 2] # bitfield: `unsigned int mode : 2;`
    pub pull:  Bits[u32; 2]
    pub speed: Bits[u32; 3]

@value_type
@aligned(16)
class Simd4:
    pub lanes: Simd[f32; 4] # GCC/Clang vector: `float lanes __attribute__((vector_size(16)))`
```

- **`@packed` / `@aligned(N)` / `@union`** are decorators that stack with `@value_type` (and
  imply it — a layout-controlled aggregate is always a stack value). Reading a `@union` field
  other than the one last written is type-punning; do it in an `unsafe:` block.
- **`Bits[T; N]`** is a bitfield of `N` bits stored in `T`; valid only as a struct field. It
  reads/writes as an ordinary integer.
- **`Simd[T; N]`** maps to the compiler's vector extension. For most APIs, prefer passing SIMD
  aggregates by `Pointer[T]`; use `Simd[T; N]` only when the C API takes a vector by value.

See `examples/ffi_struct_layout.tr` for a runnable version.

> Note: printing a raw `c_long`/`c_int` value directly needs a cast (`x as int`) — the
> auto-formatter only dispatches Tauraro's own scalar types. Their main use (FFI signatures
> and struct-layout matching) needs no cast.
>
> FFI is the main place you cast with `as` — passing `str as Pointer[char]`, erasing types with
> `x as Pointer[void]`, recovering them with `vp as Pointer[T]`, and `fn as Pointer[void]` for
> callbacks. See [23 — Casting with `as`](23_casting_with_as.md) for every case, why it's needed,
> and the exact error without it.

---

## Passing callbacks to C (function pointers + userdata)

A C API that takes a callback usually looks like `f(void (*cb)(args…, void* userdata), void* userdata)`.
There are two ways to bridge one, both zero-cost. Use `c_int` (not `int`) for the callback's
parameters so the width matches C's `int`.

**1 — Closure bridge (ergonomic): `c_callback(closure)`.** It returns a C function-pointer
*trampoline* for the closure's signature; the trampoline threads the closure through the
callback's **last** argument (the `userdata`). Bind the closure to a local, then pass it as the
userdata too:

```python
extern "C":
    def run_events(cb: Pointer[void], ud: Pointer[void], a: c_int, b: c_int, c: c_int)

mut total = 0
mut cb = def(ev: c_int):
    total = total + (ev as int)
run_events(c_callback(cb), cb as Pointer[void], 10, 20, 12)   # total == 42
```

Captures are **by reference** into the current frame, so this is sound for callbacks invoked
*synchronously* (during the call, as above). For a callback that is stored and fired later,
capture heap-stable state instead of stack locals — or use pattern 2.

**2 — Explicit function + `Pointer[T]` userdata (fully sound, C-idiomatic).** A top-level
function *is* a plain C function; pass it directly and thread state through a `Pointer[T]`
userdata you control (the Rust `extern "C" fn` style):

```python
def on_event(ev: c_int, ud: Pointer[Acc]):
    unsafe:
        mut a = ud.read()
        a.total = a.total + (ev as int)
        ud.write(a)

run_events(on_event as Pointer[void], (&acc) as Pointer[void], 10, 20, 12)
```

A **non-capturing** callback needs neither — pass the top-level function directly (see the
comparator in `examples/16_extern_and_ffi.tr`). See `examples/ffi_callbacks.tr` for both patterns.

---

## `extern "C":` Declarations

### When to use

Use `extern "C":` to declare any C function you want to call from Tauraro. Group related declarations together — one block per library is a clean convention.

### How it works

```python
extern "C":
    def puts(s: str) -> int
    def strlen(s: str) -> int
    def malloc(size: usize) -> Pointer[void]
    def free(ptr: Pointer[void]) -> void
    def memcpy(dst: Pointer[void], src: Pointer[void], n: usize) -> Pointer[void]
    def memset(dst: Pointer[void], value: int, n: usize) -> Pointer[void]
    def exit(code: int) -> void
    def abort() -> void
```

The compiler emits the appropriate C prototype for each declaration. At link time the function is resolved from the C runtime, libc, or any other library you pass with `-l`.

### Common Mistakes

**Mistake: wrong return type.**
```python
extern "C":
    def time(t: Pointer[int]) -> int    # WRONG — time() returns long (64-bit on x86-64)
    def time(t: Pointer[int]) -> usize  # correct — matches the C size
```
Match the exact C return type. Mismatches cause silent truncation or misinterpretation on different platforms.

**Mistake: wrong parameter type for size arguments.**
```python
extern "C":
    def read(fd: int, buf: Pointer[void], n: int) -> int    # WRONG — n is size_t
    def read(fd: int, buf: Pointer[void], n: usize) -> int  # correct
```

### Best Practices

1. One `extern "C":` block per library — keeps declarations organized.
2. Always match C types exactly: `usize` for `size_t`, `u32` for `uint32_t`, `Pointer[void]` for `void*`.
3. Wrap `extern "C"` calls in safe Tauraro functions that check return values and handle errors before exposing them to the rest of your code.

---

## `extern "CPP":` Declarations (C++ Interop)

### When to use

Use `extern "CPP":` (or `extern "C++":`) when the symbols you're calling come
from a C++ library. Tauraro accepts both `"CPP"`/`"C++"` and `"C"` as the ABI
string in an `extern` block — they generate identical C declarations.

### How it works

```python
extern "CPP":
    def cpp_create_widget(name: str) -> Pointer[void]
    def cpp_destroy_widget(w: Pointer[void]) -> void
```

This is purely a documentation/intent marker for the block — the generated C
prototype is the same as `extern "C"`. **The actual C++ interop work happens
on the C++ side**: C++ name-mangles every symbol unless it is declared inside
an `extern "C" { ... }` block in the C++ source/header. So to call into a C++
library:

1. If the library already ships a C API (most do — e.g. `extern "C"` headers
   for SQLite's C++ wrappers, LLVM-C, etc.), declare those functions directly
   with `extern "C":`.
2. If it only exposes raw C++ classes/templates, write a small C++ shim file
   with `extern "C"` wrapper functions that forward to the C++ API, compile
   it to a `.o`/`.a` with `g++`, and link it in with `--link <path>`:

```cpp
// shim.cpp — compiled separately with g++ -c shim.cpp -o shim.o
extern "C" void* cpp_create_widget(const char* name) {
    return new Widget(name);
}
extern "C" void cpp_destroy_widget(void* w) {
    delete static_cast<Widget*>(w);
}
```

```python
extern "CPP":
    def cpp_create_widget(name: str) -> Pointer[void]
    def cpp_destroy_widget(w: Pointer[void]) -> void
```

```
tauraroc src/main.tr --link shim.o -lstdc++ -o app
```

### Common Mistakes

**Expecting `extern "CPP":` to call mangled C++ symbols directly.** It
doesn't — there is no `extern "C++"` linkage spec in C, and a hand-written
declaration can never match a compiler's name-mangling scheme. Always go
through an `extern "C"` boundary, either from the library itself or via a
shim (see above).

### Best Practices

1. Use `extern "CPP":` purely to signal "these declarations come from a C++
   library" — group them separately from plain C declarations.
2. Always cross an `extern "C"` boundary on the C++ side (library-provided or
   a thin shim).
3. Link C++ shims with `-lstdc++` (Linux/macOS) or the appropriate MSVC C++
   runtime on Windows.

---

## Exporting Tauraro Functions to C

### When to use

Use `pub export def` when you are building a shared library or embedding Tauraro in a C application and need C code to call Tauraro functions.

### How it works

```python
pub export def add(a: int, b: int) -> int:
    return a + b

pub export def greet(name: str) -> void:
    print(f"Hello, {name}!")
```

`export` suppresses name mangling — the symbol appears in the object file exactly as written. `export` implies `pub`.

Build a linkable library plus a C header for callers with `--lib`:
```sh
tauraroc mylib.tr --lib -o mylib      # -> mylib.dll/.so + mylib.h
```

### Calling from C++ (`--export-cpp`)

Add `--export-cpp` (it implies `--lib`) to also emit an **ergonomic, self-contained C++ header** —
this is the mirror image of `bindgen` (which lets Tauraro call C++). Because Tauraro compiles to C,
the C ABI *is* the bridge, so no IDL or bridge module is needed (simpler than Rust `cxx` or Swift
interop):

```sh
tauraroc mathlib.tr --export-cpp -o mathlib   # -> mathlib.dll/.so + mathlib.h + mathlib.hpp
```

```cpp
#include "mathlib.hpp"          // self-contained: no Tauraro runtime header needed
#include <iostream>
int main() {
    std::cout << mathlib::add(3, 4) << "\n";        // 7   (scalars pass straight through)
    std::cout << mathlib::greet("world") << "\n";   // "hello, world!"  (std::string <-> str)
}
```
```sh
g++ app.cpp mathlib.dll -o app        # link the Tauraro library like any other
```

Each `export def` appears as an inline wrapper in `namespace <libname>` that maps `str` to/from
`std::string` and passes scalars through.

**Tauraro classes become C++ RAII wrappers.** A class reachable through the exported API (returned or
taken by an `export def`) is exposed as a C++ class over an opaque handle, with **Tauraro's ARC mapped
to C++ value semantics** — copy retains, destruction releases, move steals. Its static factory
`def init(...) -> C` becomes a C++ constructor, and its instance methods forward through the handle:

```python
class Counter:
    mut n: int
    def init(start: int) -> Counter:      # static factory = the C++ constructor
        mut c = Counter(); c.n = start; return c
    def bump(self, by: int) -> int:
        self.n = self.n + by; return self.n

export def make_counter(start: int) -> Counter:
    return Counter.init(start)
```
```cpp
#include "counter.hpp"
counter::Counter c(10);              // direct construction (calls Counter.init)
c.bump(5);                           // -> 15   (method forwards through the handle)
counter::Counter c2 = c;             // copy => ARC retain; both released automatically
auto f = counter::make_counter(100); // factory returning a Counter, owned by the C++ value
```

**Collections marshal to the C++ standard library:**

| Tauraro | C++ |
|---|---|
| `List[T]` / `Vec[T]` — element is a scalar, `str`, **or a class** | `std::vector<T>` (a `List[Class]` becomes `std::vector<Class-wrapper>`, return-only) |
| `Dict[K, V]` / `Map[K, V]` — key `str` or int; value int / `bool` / `float` / `str` | `std::map<K, V>` |
| `(T0, T1, …)` tuple — int / `bool` / `float` / `str` slots | `std::tuple<…>` |

These work as parameters and returns — the wrapper builds the Tauraro collection from the C++ container
(and frees it) on the way in, and materialises the result into the C++ container on the way out:

```cpp
std::vector<long long> v = mylib::nums();          // List[int]  -> std::vector
std::map<std::string,long long> m = mylib::scores(); // Dict[str,int] -> std::map
auto [code, msg] = mylib::status();                // (int, str) -> std::tuple, structured binding
long long s = mylib::total({1, 2, 3});             // std::vector -> List[int] param
```

The header is **self-contained** — it declares only a stable `extern "C"` bridge ABI (retain/release/
construct/method for classes; len/get/new/push/free for collections; box/unbox for boxed tuple/dict
slots), all generated *into* the library where the runtime lives, so it never `#include`s
`tauraro_rt.h`. Signatures using a not-yet-bridged type (**nested** collections like `List[List[int]]`
or `Dict[str, List[int]]`, a `List[Class]` **parameter** — return-only for now — or `throws`) are
omitted from the C++ view (still reachable through the raw C `--lib` header), so the `.hpp` always
compiles standalone.

### Common Mistakes

**Mistake: expecting mangled Tauraro symbols to be callable from C.** Without `export`, internal Tauraro functions have mangled names that C cannot easily call.

### Best Practices

1. Export only the public API surface — keep internal helpers non-exported.
2. Document the expected C types in a comment next to each `export def`.

---

## Variadic C Functions

### When to use

Use variadic declarations when calling C functions like `printf`, `fprintf`, `sprintf`, or any other function that accepts a variable number of arguments.

### How it works

Use a trailing `args...` as the last parameter — it emits C's literal `...`
variadic signature (see [Functions — Variadic Functions](05_functions.md#variadic-functions)
for the full picture, including Tauraro-side `args...` -> `List[T]`):

```python
extern "C":
    def printf(fmt: str, args...) -> int
    def fprintf(stream: Pointer[void], fmt: str, args...) -> int
    def sprintf(buf: str, fmt: str, args...) -> int
    def snprintf(buf: str, n: usize, fmt: str, args...) -> int
    def sscanf(s: str, fmt: str, args...) -> int
```

```python
unsafe:
    printf("%s has %d items\n", name, count)
```

The type safety of variadic arguments is your responsibility — the compiler does not check that format strings match argument types.

### Common Mistakes

**Mistake: using `printf` where Tauraro's `print(f"...")` is available.**
```python
unsafe:
    printf("x = %d\n", x)    # verbose — and no type checking on the format
```
```python
print(f"x = {x}")    # preferred — type-safe, no unsafe required
```

**Mistake: passing a Tauraro `str` to `%s` in `printf`.** Tauraro `str` is already a `const char*` so this is safe, but passing an `int` to `%s` is undefined behavior with no compiler warning.

### Best Practices

1. Prefer `print(f"...")` over `printf` unless you specifically need C-level output control.
2. Treat every variadic argument position as untyped — manually verify format strings.

---

## Math and Standard Library Functions

### When to use

Use `extern "C"` to call C math functions when you need operations not built into Tauraro's standard library, or when calling from a performance-critical path where you want the C intrinsic directly.

### How it works

```python
extern "C":
    def sqrt(x: float) -> float
    def pow(base: float, exp: float) -> float
    def abs(x: int) -> int
    def fabs(x: float) -> float
    def floor(x: float) -> float
    def ceil(x: float) -> float
    def sin(x: float) -> float
    def cos(x: float) -> float
    def tan(x: float) -> float
    def log(x: float) -> float
    def log2(x: float) -> float
    def exp(x: float) -> float
    def rand() -> int
    def srand(seed: u32) -> void

def main():
    print(sqrt(16.0))    # 4.0
    print(floor(3.7))    # 3.0
    print(ceil(3.2))     # 4.0
```

Link with `-lm` on Linux:
```bash
tauraroc main.tr -lm --run
```

### Common Mistakes

**Mistake: forgetting `-lm` on Linux.** On Linux, math functions are in a separate `libm` — without `-lm` you get `undefined reference to 'sqrt'`. On Windows and macOS, math functions are included automatically.

### Best Practices

1. Always add `-lm` when using math functions on Linux.
2. Group all math declarations in a single `extern "C":` block at the top of the file.

---

## System APIs

### When to use

Use `extern "C"` to call POSIX or Win32 system APIs when Tauraro's standard library does not cover the functionality you need, or when you need fine-grained control over system calls.

### How it works

**POSIX file I/O:**
```python
extern "C":
    def open(path: str, flags: int, mode: int) -> int
    def close(fd: int) -> int
    def read(fd: int, buf: Pointer[void], count: usize) -> int
    def write(fd: int, buf: Pointer[void], count: usize) -> int
    def lseek(fd: int, offset: int, whence: int) -> int

const O_RDONLY = 0
const O_WRONLY = 1
const O_RDWR   = 2
const O_CREAT  = 64
const O_TRUNC  = 512

def read_file(path: str) -> str:
    mut fd = open(path, O_RDONLY, 0)
    if fd < 0:
        raise("cannot open file: " + path)

    unsafe:
        mut buf: Pointer[char] = alloc[char](4096)
        mut n = read(fd, buf as Pointer[void], 4095 as usize)
        buf.offset(n).write('\0')
        close(fd)
        return buf as str
```

**Win32 API:**
```python
extern "C":
    def GetLastError() -> u32
    def CreateFileA(
        name:     str,
        access:   u32,
        share:    u32,
        sec:      Pointer[void],
        creation: u32,
        flags:    u32,
        tmpl:     Pointer[void]
    ) -> Pointer[void]
    def CloseHandle(h: Pointer[void]) -> bool
    def WriteFile(
        h:          Pointer[void],
        buf:        Pointer[void],
        n:          u32,
        written:    Pointer[u32],
        overlapped: Pointer[void]
    ) -> bool

const GENERIC_READ  = 0x80000000 as u32
const GENERIC_WRITE = 0x40000000 as u32
const OPEN_EXISTING = 3 as u32
const CREATE_ALWAYS = 2 as u32
```

### Common Mistakes

**Mistake: not checking the return value of system calls.**
```python
open(path, O_RDONLY, 0)    # return value ignored — may have failed silently
```
Always check return values. POSIX calls return `-1` on error; Win32 calls return `INVALID_HANDLE_VALUE` or `FALSE`.

### Best Practices

1. Wrap every system call in a Tauraro function that checks for errors and raises on failure.
2. Use `Pointer[void]` for opaque OS handles (Win32 `HANDLE`, POSIX file descriptors are `int`).

---

## Linking External Libraries

### When to use

Pass `-l` flags to link against any C library. Pass `-L` to add a directory to the library search path.

### How it works

```bash
# Link against libcurl on Linux
tauraroc main.tr -L /usr/lib -l curl --run

# Link against a local static library
tauraroc main.tr -L ./libs -l mylib --run

# Link multiple libraries
tauraroc main.tr -l curl -l ssl -l crypto --run
```

On Windows with MinGW:
```bash
tauraroc main.tr -L "C:/MinGW/lib" -l ws2_32 -l user32 -l gdi32 --run
```

### Common Mistakes

**Mistake: wrong flag order.** On Linux, libraries must come after object files:
```bash
tauraroc main.tr -l curl --run          # correct
tauraroc -l curl main.tr --run          # may cause linker errors on Linux
```

**Mistake: using the full filename instead of the library name.**
```bash
-l libcurl.so    # wrong
-l curl          # correct — the linker prepends "lib" and appends ".so"/".a"
```

### Best Practices

1. Document all required `-l` flags in a build comment at the top of the main file.
2. Use `-L ./libs` to keep project-local libraries separate from system libraries.

---

## Struct Interop: C Structs as Tauraro Classes

### When to use

When a C library passes or returns structs by pointer, declare a matching Tauraro class with the same field layout. Tauraro classes emit as C structs directly — no `extern "C" struct` is needed.

### How it works

```python
# C struct in libc:
# struct stat {
#     uint64_t st_size;
#     uint64_t st_mtime;
#     ...
# };

class StatBuf:
    pub st_size:  u64
    pub st_mtime: u64
    pub _pad:     u64    # extra padding to match actual C struct layout

extern "C":
    def stat(path: str, buf: Pointer[StatBuf]) -> int

def file_size(path: str) -> int:
    mut buf = StatBuf()
    unsafe:
        mut r = stat(path, &buf)
        if r != 0:
            raise("stat failed")
        return buf.st_size as int
```

### Common Mistakes

**Mistake: assuming Tauraro field order matches the C struct without checking.**
C structs may have padding between fields for alignment. Use `sizeof(YourClass)` and compare against the expected C size to verify the layout before using the struct in FFI.

**Mistake: missing padding fields.**
```python
class Stat:
    pub st_size:  u64    # offset 0
    # st_mtime may be at offset 16, not 8, depending on the OS struct definition
```
Add `_pad` fields to match alignment gaps.

### Best Practices

1. Verify the layout with `sizeof(YourClass)` against the C struct size before using in FFI.
2. Add explicit `_pad` fields for any alignment gaps in the C struct.
3. Comment each field with its C type and the C struct member name.

---

## Callback Functions (Function Pointers)

### When to use

Use function-pointer callbacks when a C library accepts a function as an argument — for example `qsort`, `pthread_create`, signal handlers, or event callbacks.

### How it works

```python
extern "C":
    def qsort(
        base: Pointer[void],
        n:    usize,
        size: usize,
        cmp:  lambda    # function pointer
    ) -> void

# Comparison function: must match int cmp(const void*, const void*)
def int_cmp(a: Pointer[void], b: Pointer[void]) -> int:
    unsafe:
        mut ia = (a as Pointer[int]).read()
        mut ib = (b as Pointer[int]).read()
        if ia < ib: return -1
        if ia > ib: return 1
        return 0

def main():
    mut nums = [5, 2, 8, 1, 9, 3]
    unsafe:
        qsort(
            nums as Pointer[void],
            6 as usize,
            sizeof(int) as usize,
            int_cmp
        )
    for n in nums:
        print(n)    # 1 2 3 5 8 9
```

A top-level Tauraro function can be passed directly as a `lambda` (C function pointer) to a C callback parameter.

**Limitation:** Closures (functions that capture variables) cannot be passed as C callbacks — they carry a context pointer that C functions do not know about. Only plain, non-capturing functions work as C callbacks.

### Common Mistakes

**Mistake: passing a closure as a C callback.**
```python
mut offset = 10
def shifted_cmp(a: Pointer[void], b: Pointer[void]) -> int:
    # captures `offset` from outer scope — NOT a valid C callback
    ...
qsort(base, n, size, shifted_cmp)    # compile error or undefined behavior
```
Only non-capturing top-level functions can be passed as `lambda` to C.

### Best Practices

1. Keep C callback functions at the top level (not nested, not closures).
2. Match the C callback signature exactly — return type and all parameter types.

---

## Dynamic Linking (`.so` / `.dll`)

### When to use

Use `dlopen` / `LoadLibrary` when you need to load a library at runtime — for plugins, optional features, or libraries that may not be present at build time.

### How it works

**POSIX (`dlopen`):**
```python
extern "C":
    def dlopen(path: str, flags: int) -> Pointer[void]
    def dlsym(handle: Pointer[void], name: str) -> Pointer[void]
    def dlclose(handle: Pointer[void]) -> int
    def dlerror() -> str

const RTLD_LAZY = 1
const RTLD_NOW  = 2

def load_plugin(path: str, func_name: str) -> lambda:
    mut handle = dlopen(path, RTLD_LAZY)
    unsafe:
        if handle as usize == 0 as usize:
            raise("cannot open library: " + dlerror())
    mut fn_ptr = dlsym(handle, func_name)
    unsafe:
        if fn_ptr as usize == 0 as usize:
            raise("cannot find symbol: " + func_name)
    return fn_ptr as lambda
```

**Windows (`LoadLibraryA`):**
```python
extern "C":
    def LoadLibraryA(path: str) -> Pointer[void]
    def GetProcAddress(h: Pointer[void], name: str) -> Pointer[void]
    def FreeLibrary(h: Pointer[void]) -> bool
```

### Common Mistakes

**Mistake: ignoring `dlerror()` when `dlopen` returns null.**
Always call `dlerror()` after a failed `dlopen` to get the OS error message.

**Mistake: calling `dlclose` while the loaded function is still in use.**

### Best Practices

1. Check return values of both `dlopen` and `dlsym` before use.
2. Call `dlclose` when done to release the library from memory.

---

## Common FFI Errors

**Wrong argument type:**
```python
extern "C":
    def write(fd: int, buf: Pointer[void], n: usize) -> int

unsafe:
    write(1, "hello\n", 6)    # ERROR: str is not Pointer[void]
    write(1, "hello\n" as Pointer[void], 6 as usize)    # correct
```

**Missing link flag:**
```
undefined reference to `sqrt'
```
Fix: add `-lm` to the compile command.

**ABI mismatch:**
```python
extern "C":
    def time(t: Pointer[int]) -> int    # WRONG: time() returns long (64-bit on x86-64)
    def time(t: Pointer[int]) -> usize  # correct
```

**Struct layout mismatch:**
```python
class Stat:
    pub size: u32    # WRONG: st_size is uint64_t
    pub size: u64    # correct
```

---

## Best Practices Summary

1. **Group related declarations.** One `extern "C":` block per library.
2. **Wrap C calls in safe functions.** Don't call `extern "C"` functions directly from main logic — wrap them in Tauraro functions that handle errors.
3. **Match types exactly.** Use `u32` for `uint32_t`, `usize` for `size_t`, `Pointer[void]` for `void*`.
4. **Use `unsafe:` for all C pointer operations.** Any code that passes raw pointers or dereferences C-returned pointers should be in `unsafe:`.
5. **Check return values.** Most C library functions signal errors via return codes. Check every one.
6. **Verify struct layouts.** Compare `sizeof(YourClass)` against the expected C struct size before using it in FFI.

---

Next: [GPU and Inline Assembly →](18_gpu_and_asm.md)
