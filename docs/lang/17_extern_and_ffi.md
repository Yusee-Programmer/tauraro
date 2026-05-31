# 17 — Extern and C Interop (FFI)

---

## Why FFI Matters

Tauraro compiles to C, and C has the largest library ecosystem of any language. FFI (Foreign Function Interface) lets you call:
- Standard C library functions (`malloc`, `printf`, `fopen`, ...)
- System calls via libc wrappers (`read`, `write`, `mmap`, ...)
- Third-party libraries (`libcurl`, `OpenSSL`, `SDL2`, `SQLite`, ...)
- Operating system APIs (Win32, POSIX, ...)
- Any native library with a C interface

---

## extern "C" Declarations

Declare C functions you want to call:

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

The compiler emits the appropriate C prototype for each declaration. Tauraro types map to C types: `int` → 64-bit integer, `str` → string pointer, `usize` → platform word size, `Pointer[void]` → typeless pointer.

---

## Variadic Functions

C variadic functions use `...` in the parameter list:

```python
extern "C":
    def printf(fmt: str, ...) -> int
    def fprintf(stream: Pointer[void], fmt: str, ...) -> int
    def sprintf(buf: str, fmt: str, ...) -> int
    def snprintf(buf: str, n: usize, fmt: str, ...) -> int
    def sscanf(s: str, fmt: str, ...) -> int
```

The type safety of the variadic part is your responsibility — the compiler doesn't check that format strings match argument types.

```python
unsafe:
    printf("%s has %d items\n", name, count)    # direct C printf call
```

**Best practice:** Prefer Tauraro's built-in `print(f"...")` over `printf` — it's type-safe and generates the correct format string automatically. Use `printf` only when you need exact C output control.

---

## Math and Standard Library Functions

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
    print(sqrt(16.0))     # 4.0
    print(floor(3.7))     # 3.0
    print(ceil(3.2))      # 4.0
```

Link with `-lm` (math library) on Linux:
```bash
tauraroc main.tr -lm --run
```

---

## System APIs

### POSIX File I/O

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
    if fd < 0: raise("cannot open file: " + path)
    
    unsafe:
        mut buf: Pointer[char] = alloc[char](4096)
        mut n = read(fd, buf as Pointer[void], 4095 as usize)
        buf.offset(n).write('\0')
        close(fd)
        return buf as str
```

### Win32 API

```python
extern "C":
    def GetLastError() -> u32
    def CreateFileA(
        name: str,
        access: u32,
        share: u32,
        sec: Pointer[void],
        creation: u32,
        flags: u32,
        tmpl: Pointer[void]
    ) -> Pointer[void]
    def CloseHandle(h: Pointer[void]) -> bool
    def WriteFile(
        h: Pointer[void],
        buf: Pointer[void],
        n: u32,
        written: Pointer[u32],
        overlapped: Pointer[void]
    ) -> bool

const GENERIC_READ  = 0x80000000 as u32
const GENERIC_WRITE = 0x40000000 as u32
const OPEN_EXISTING = 3 as u32
const CREATE_ALWAYS = 2 as u32
const INVALID_HANDLE = 0xFFFFFFFFFFFFFFFF as usize
```

---

## Linking External Libraries

Pass linker flags on the command line:

```bash
# Link against libcurl on Linux
tauraroc main.tr -L /usr/lib -l curl --run

# Link against a local .a library
tauraroc main.tr -L ./libs -l mylib --run

# Link multiple libraries
tauraroc main.tr -l curl -l ssl -l crypto --run
```

On Windows with MinGW:
```bash
tauraroc main.tr -L "C:/MinGW/lib" -l ws2_32 -l user32 -l gdi32 --run
```

---

## Struct Interop: C Structs as Tauraro Classes

When a C library uses structs, declare matching Tauraro classes:

```python
# C struct in libc:
# struct stat {
#     uint64_t st_size;
#     uint64_t st_mtime;
#     // ... other fields
# };

class StatBuf:
    pub st_size:  u64
    pub st_mtime: u64
    pub _pad:     u64    # padding to match C struct layout

extern "C":
    def stat(path: str, buf: Pointer[StatBuf]) -> int

def file_size(path: str) -> int:
    mut buf = StatBuf()
    unsafe:
        mut r = stat(path, &buf)
        if r != 0: raise("stat failed")
        return buf.st_size as int
```

**Important:** The Tauraro class layout must exactly match the C struct layout. Use `@packed` if needed, and verify with `sizeof(StructName)` against the expected C size.

---

## Callback Functions (Function Pointers)

To pass a Tauraro function as a C callback:

```python
extern "C":
    # qsort takes a comparison function pointer
    def qsort(
        base: Pointer[void],
        n: usize,
        size: usize,
        cmp: lambda    # function pointer type
    ) -> void

# The comparison function must match: int cmp(const void*, const void*)
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
            nums as Pointer[void],    # base pointer to array data
            6 as usize,
            sizeof(int) as usize,
            int_cmp
        )
    for n in nums: print(n)    # 1 2 3 5 8 9
```

**How function-pointer callbacks work:** A top-level Tauraro function can be passed directly as a `lambda` (function pointer) to C.

**Limitation:** Closures cannot be passed as C callbacks because they carry a context pointer that C functions don't know about. Only plain (non-capturing) functions work as C callbacks.

---

## Dynamic Linking (.so / .dll)

To call functions from a shared library loaded at runtime:

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
    if handle as usize == 0 as usize:
        raise("cannot open: " + dlerror())
    mut fn_ptr = dlsym(handle, func_name)
    if fn_ptr as usize == 0 as usize:
        raise("cannot find symbol: " + func_name)
    return fn_ptr as lambda
```

On Windows, the equivalent is `LoadLibraryA`, `GetProcAddress`, `FreeLibrary`.

---

## extern "C" Best Practices

1. **Group related declarations.** One `extern "C":` block per library:
   ```python
   extern "C":    # libc
       def malloc(n: usize) -> Pointer[void]
       def free(p: Pointer[void]) -> void
   
   extern "C":    # libcurl
       def curl_easy_init() -> Pointer[void]
       def curl_easy_cleanup(h: Pointer[void]) -> void
   ```

2. **Wrap C calls in safe functions.** Don't call `extern "C"` functions directly from main logic — wrap them in Tauraro functions that handle errors:
   ```python
   def safe_open(path: str) throws str -> int:
       mut fd = open(path, O_RDONLY, 0)
       if fd < 0: raise("cannot open " + path)
       return fd
   ```

3. **Match types exactly.** Use `u32` for `uint32_t`, `usize` for `size_t`, `Pointer[void]` for `void*`. Mismatches cause silent truncation or misinterpretation on different platforms.

4. **Use `unsafe:` for all C pointer operations.** Even though the `extern "C"` declaration itself doesn't require `unsafe:`, any code that passes raw pointers or uses the returned value as a pointer should be in `unsafe:`.

5. **Check return values.** Most C library functions signal errors via return codes. Check them:
   ```python
   if close(fd) != 0: raise("close failed")
   ```

---

## Common FFI Errors

### Wrong argument type

```python
extern "C":
    def write(fd: int, buf: Pointer[void], n: int) -> int

write(1, "hello\n", 6)    # ERROR: "hello\n" is str (char*), not Pointer[void]
```
**Fix:**
```python
unsafe:
    write(1, "hello\n" as Pointer[void], 6)
```

### Missing link flag

```
undefined reference to `sqrt'
```
**Fix:** Add `-lm` to the compile command.

### ABI mismatch

Calling a C function that returns `long` but declaring it as returning `int`:
```python
extern "C":
    def time(t: Pointer[int]) -> int    # WRONG: time() returns long (64-bit on x86-64)
    def time(t: Pointer[int]) -> int    # should be -> usize or -> i64
```
**Fix:** Match the exact C return type.

---

Next: [GPU & Inline Assembly →](18_gpu_and_asm.md)
