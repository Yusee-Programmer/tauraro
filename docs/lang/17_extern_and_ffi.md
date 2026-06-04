# 17 — Extern and C Interop (FFI)

---

## Overview

Tauraro compiles to C, which means the entire C library ecosystem is directly accessible. FFI (Foreign Function Interface) lets you call:

- Standard C library functions (`malloc`, `printf`, `fopen`, ...)
- System calls via libc wrappers (`read`, `write`, `mmap`, ...)
- Third-party C libraries (`libcurl`, `OpenSSL`, `SDL2`, `SQLite`, ...)
- Operating system APIs (Win32, POSIX, ...)
- Any native library with a C interface

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

The corresponding C header you would ship to callers:
```c
int64_t add(int64_t a, int64_t b);
void    greet(const char* name);
```

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

Use `...` as the last parameter:

```python
extern "C":
    def printf(fmt: str, ...) -> int
    def fprintf(stream: Pointer[void], fmt: str, ...) -> int
    def sprintf(buf: str, fmt: str, ...) -> int
    def snprintf(buf: str, n: usize, fmt: str, ...) -> int
    def sscanf(s: str, fmt: str, ...) -> int
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
