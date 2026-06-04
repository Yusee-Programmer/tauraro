# 14 — Unsafe Blocks and Raw Pointers

---

## Overview

Tauraro's safety model is enforced by the compiler — it tracks ownership, prevents dangling pointers, and prevents double-free. Some operations are inherently outside what a compiler can verify: raw pointer arithmetic, manual heap allocation, inline assembly, and calling external C functions with raw pointer arguments.

These operations are quarantined behind the `unsafe:` keyword. Every `unsafe:` block is a promise that you have verified the invariants the compiler cannot.

**The goal of `unsafe:` is containment, not avoidance.** The keyword makes low-level operations visible and auditable — a safety reviewer can search for `unsafe:` and find every raw operation in the codebase in one pass.

**`--strict` mode:** When the `--strict` flag is passed, using `alloc` or `dealloc` outside an `unsafe:` block is a compile error `[U-1]`.

---

## The `unsafe:` Block

### When to use

Use `unsafe:` when you need to:

- Take the address of a variable (`&x`)
- Dereference a raw pointer (`.read()`, `.write()`)
- Perform pointer arithmetic (`.offset(n)`)
- Manually allocate or free heap memory (`alloc`, `dealloc`)
- Call C library functions that accept raw pointer arguments
- Emit inline assembly instructions (`asm(...)`)
- Cast a value to or from a `Pointer[T]`

Do **not** use `unsafe:` for anything that does not require it. The block is an auditable surface — keep it as small as possible.

### How it works

```python
mut x: int = 42
mut y: int = 0

unsafe:
    mut p: Pointer[int] = &x    # take address of x
    y = p.read()                 # dereference — y = 42
    p.write(100)                 # x is now 100

print(f"y = {y}, x = {x}")     # y = 42, x = 100
```

**Operations that require `unsafe:`:**

| Operation | Why unsafe |
|-----------|-----------|
| `&x` address-of | Produces a raw pointer that ownership tracking cannot follow |
| `p.read()` | May access freed or invalid memory |
| `p.write(v)` | May corrupt memory |
| `p.offset(n)` | May produce an out-of-bounds pointer |
| `alloc[T](n)` | Bypasses automatic memory management |
| `dealloc(ptr)` | Bypasses ownership tracking |
| `asm(...)` | Emits arbitrary machine instructions with side effects |
| `x as Pointer[T]` | Reinterprets the value as a pointer — no type safety |

**What remains safe inside `unsafe:`:**

- Type checking still runs normally
- Ownership tracking still protects variables declared **outside** the unsafe block
- The compiler still enforces `Own` rules for outer-scope bindings

### Common Mistakes

**Mistake: too-large unsafe blocks.**
```python
# Bad — everything is inside unsafe unnecessarily
unsafe:
    mut total = 0
    for item in items:          # safe loop, does not need unsafe
        total += item
    mut p: Pointer[int] = &total
    p.write(total * 2)
```
```python
# Good — only the pointer operations are unsafe
mut total = 0
for item in items:
    total += item

unsafe:
    mut p: Pointer[int] = &total
    p.write(total * 2)
```

**Mistake: no comment explaining why the operation is sound.**
```python
unsafe:
    buf.offset(i).read()    # why is i in bounds? nobody knows
```

### Best Practices

1. Keep `unsafe:` blocks as small as possible — just the raw operation, nothing else.
2. Add a `# SAFETY:` comment to every `unsafe:` block explaining the invariant you are upholding.
3. Wrap unsafe code in a class with a safe public API so callers never see `unsafe:`.
4. Never use pointer arithmetic to iterate collections — use `List[T]` with index access instead.

---

## Raw Pointer Type: `Pointer[T]`

### When to use

`Pointer[T]` is a raw C pointer (`T*`). Use it when:

- Calling a C function that accepts or returns a pointer
- Implementing a custom allocator or memory arena
- Doing layout-sensitive work (serialization, binary protocols, kernel data structures)

The compiler does **not** track its lifetime, inject `free()` for it, or check whether the pointed-to memory is valid. Ownership is entirely your responsibility.

### How it works

```python
mut n: int = 10

unsafe:
    mut p: Pointer[int] = &n         # p is int* in C

    # Read through pointer
    mut val = p.read()               # *p in C — val = 10

    # Write through pointer
    p.write(99)                      # *p = 99 — n is now 99

    # Pointer arithmetic
    mut buf: Pointer[int] = alloc[int](4)
    buf.offset(0).write(10)
    buf.offset(1).write(20)
    mut elem = buf.offset(1).read()  # *(buf + 1) = 20
    dealloc(buf)
```

`.offset(n)` advances by `n` elements of type `T` — identical to C pointer arithmetic (`ptr + n`).

**Pointer comparison:**
```python
unsafe:
    if p as usize == 0 as usize:       # null check
        raise("null pointer")

    if p as usize == q as usize:       # same address?
        print("same location")
```

Cast to `usize` to compare addresses as integers.

### Common Mistakes

**Mistake: using `.offset()` with byte counts instead of element counts.**
```python
unsafe:
    # BAD — offset(8) advances by 8 * sizeof(int) = 64 bytes, not 8 bytes
    mut p: Pointer[int] = buf.offset(8)
```
`.offset(n)` counts elements, not bytes. For byte-level work use `Pointer[u8]`.

**Mistake: comparing pointers without casting to `usize`.**
```python
unsafe:
    if p == q:    # may not compile or produce unexpected results
        ...
    if p as usize == q as usize:    # correct
        ...
```

### Best Practices

1. Prefer `Pointer[u8]` for raw byte manipulation; use `Pointer[T]` for typed element access.
2. Always check for null before dereferencing a pointer returned from a C function.
3. Use `sizeof(T)` to compute byte sizes when communicating with C APIs.

---

## Manual Allocation: `alloc` and `dealloc`

### When to use

Use `alloc[T](n)` / `dealloc(ptr)` when:

- Implementing a custom memory allocator or memory pool
- Allocating a fixed-size buffer for FFI
- Building data structures that cannot use `List[T]`

Prefer `List[T]` for all general-purpose dynamic arrays — it handles allocation and deallocation automatically.

### How it works

`alloc[T](n)` allocates heap memory for `n` elements of type `T`, zero-initializes it, and returns a `Pointer[T]`. On allocation failure the runtime aborts immediately — you never need to check for null.

```python
unsafe:
    mut buf: Pointer[int] = alloc[int](8)    # 8 × sizeof(int) bytes, zero-filled

    mut i = 0
    while i < 8:
        buf.offset(i).write(i * 10)
        i += 1

    i = 0
    while i < 8:
        print(buf.offset(i).read())     # 0 10 20 30 40 50 60 70
        i += 1

    dealloc(buf)    # mandatory — compiler will NOT free this automatically
```

**Ownership contract:** Every `alloc` must be matched by exactly one `dealloc`. Forget to call `dealloc` — memory leak. Call `dealloc` twice — heap corruption.

**Best pattern — wrap in a class:** Put the `alloc`/`dealloc` pair inside `init` and `drop` methods. The class instance then gets normal `Own` tracking:

```python
class Buffer:
    pub ptr:  Pointer[int]
    pub size: int

extend Buffer:
    pub def init(n: int) -> Buffer:
        mut b = Buffer()
        b.size = n
        unsafe:
            b.ptr = alloc[int](n)
        return b

    pub def get(self, i: int) -> int:
        unsafe:
            # SAFETY: caller guarantees i is in [0, self.size)
            return self.ptr.offset(i).read()

    pub def set(self, i: int, v: int) -> void:
        unsafe:
            # SAFETY: caller guarantees i is in [0, self.size)
            self.ptr.offset(i).write(v)

    pub def drop(self) -> void:
        unsafe:
            dealloc(self.ptr)

def main():
    mut buf = Buffer.init(4)
    buf.set(0, 10)
    buf.set(1, 20)
    buf.set(2, 30)
    buf.set(3, 40)
    mut i = 0
    while i < buf.size:
        print(buf.get(i))
        i += 1
    buf.drop()
```

### Common Mistakes

**Mistake: double-free.**
```python
unsafe:
    mut p: Pointer[int] = alloc[int](4)
    dealloc(p)
    dealloc(p)    # heap corruption — undefined behavior
```

**Mistake: use-after-free.**
```python
unsafe:
    mut p: Pointer[int] = alloc[int](4)
    dealloc(p)
    print(p.read())    # reads freed memory — undefined behavior
```

**Mistake: forgetting to `dealloc` on all exit paths.**
```python
unsafe:
    mut buf: Pointer[int] = alloc[int](n)
    if some_condition:
        raise("error")    # dealloc never called — memory leaks
    dealloc(buf)
```
Use the class wrapper pattern above to avoid this.

### Best Practices

1. Always pair `alloc` and `dealloc` in the same function, or wrap them in a class `init`/`drop` pair.
2. Zero-initialize with `alloc` (it is always zero-filled); never assume random bytes.
3. Under `--strict`, `alloc` outside `unsafe:` is error `[U-1]`.

---

## Pointer Casts

### When to use

Cast pointers when:

- Passing a typed pointer to a C function that expects `Pointer[void]`
- Converting `Pointer[void]` returned from a C function back to a typed pointer
- Storing a pointer address as a `usize` integer (e.g., for comparisons or logging)

### How it works

```python
unsafe:
    mut n: int = 42
    mut p: Pointer[int] = &n

    # To void*:
    mut vp: Pointer[void] = p as Pointer[void]

    # Back to typed pointer:
    mut tp: Pointer[int] = vp as Pointer[int]

    # Pointer to integer (the address value):
    mut addr: usize = p as usize
    print(f"address = {addr}")

    # Integer to pointer (extremely dangerous — only when addr is known valid):
    mut rp: Pointer[int] = addr as Pointer[int]
```

All casts are zero-cost reinterpretations at the C level. The compiler does not insert any checks.

### Common Mistakes

**Mistake: casting a `usize` to a pointer without verifying the value is a valid address.**
```python
unsafe:
    mut addr: usize = 0x1234
    mut p: Pointer[int] = addr as Pointer[int]
    p.read()    # likely segfault — 0x1234 is not a valid address
```

### Best Practices

1. Cast to `Pointer[void]` when calling C functions that accept `void*`.
2. Cast back to the original type immediately after receiving a `Pointer[void]` from C.
3. Only cast integers to pointers when the value came from a prior `as usize` on a valid pointer.

---

## Null Pointer Pattern

### When to use

Use null pointers when a `Pointer[T]` represents the absence of a value — typically when bridging C APIs that return `NULL` on failure.

### How it works

```python
mut p: Pointer[int] = 0 as Pointer[int]    # null pointer

unsafe:
    if p as usize == 0 as usize:
        print("p is null — skip dereference")
    else:
        print(p.read())
```

Alternatively, use `none` for pointer types:
```python
mut p: Pointer[int] = none
```

### Common Mistakes

**Mistake: dereferencing without a null check when the pointer comes from C.**
```python
extern "C":
    def find_item(key: int) -> Pointer[void]

unsafe:
    mut result = find_item(42)
    # BAD — find_item may return NULL
    print((result as Pointer[int]).read())
```
Always null-check pointers returned from C functions.

### Best Practices

1. Check every pointer returned from a C function for null before using it.
2. Initialize pointer variables to `none` / `0 as Pointer[T]` rather than leaving them uninitialized.

---

## `sizeof` Operator

### When to use

Use `sizeof(T)` when:

- Computing byte offsets for FFI struct interop
- Passing buffer sizes to C functions
- Verifying that a Tauraro class matches a C struct layout

### How it works

`sizeof(T)` returns the byte size of type `T`. It compiles to C's `sizeof` operator and is evaluated at compile time.

```python
mut int_size   = sizeof(int)           # 8  (64-bit integer)
mut float_size = sizeof(float)         # 8  (double)
mut bool_size  = sizeof(bool)          # 1
mut char_size  = sizeof(char)          # 1
mut ptr_size   = sizeof(Pointer[void]) # 8  (64-bit pointer)
```

For class types:
```python
class Vec3:
    pub x: float
    pub y: float
    pub z: float

mut vec3_size = sizeof(Vec3)    # 24 bytes (3 × 8-byte doubles)
```

```python
unsafe:
    mut buf: Pointer[u8] = alloc[u8](sizeof(Header))
    # fill in header fields via pointer arithmetic
```

### Common Mistakes

**Mistake: assuming a class layout matches a C struct without verifying.**
```python
class MyStruct:
    pub a: u32
    pub b: u64
    # may have padding between a and b — use sizeof(MyStruct) to verify
```

### Best Practices

1. Always compare `sizeof(YourClass)` against the expected C struct size before using the class in FFI.
2. Add a `_pad` field if the Tauraro layout does not naturally match the C struct.

---

## Inline Assembly: `asm()`

### When to use

Use `asm()` when:

- Issuing CPU-specific instructions (`rdtsc`, `cpuid`, `hlt`, `pause`)
- Emitting memory barriers in lock-free data structures
- Writing bare-metal OS code (port I/O, TLB invalidation, interrupt control)

Inline assembly must always be inside `unsafe:`.

### How it works

**Simple form (no operands):**
```python
unsafe:
    asm("nop")     # no-op
    asm("pause")   # x86 spin-loop hint
    asm("mfence")  # full memory fence
```

**Extended form (with operands):**
```
asm(code, outputs, inputs, clobbers)
```

```python
unsafe:
    mut cycles: int = 0
    asm("rdtsc", "=A"(cycles), "", "")    # read CPU timestamp counter
```

| Argument | Description |
|----------|-------------|
| `code` | Assembly instruction string |
| `outputs` | Output constraints: `"=constraint"(var)` |
| `inputs` | Input constraints: `"constraint"(expr)` |
| `clobbers` | Clobbered registers or `"memory"` for a compiler barrier |

**Memory barrier (compiler only — no instruction emitted):**
```python
unsafe:
    asm("", "", "", "memory")
```

**Hardware memory fences:**
```python
unsafe:
    asm("mfence")    # full fence — all loads/stores ordered
    asm("lfence")    # load fence
    asm("sfence")    # store fence
```

**x86 timestamp counter:**
```python
def rdtsc() -> int:
    mut lo: u32 = 0
    mut hi: u32 = 0
    unsafe:
        asm("rdtsc", "=a"(lo), "d"(hi), "")
    return (hi as int << 32) | lo as int
```

**x86 port I/O (bare-metal OS):**
```python
def inb(port: u16) -> u8:
    mut data: u8 = 0
    unsafe:
        asm("inb %1, %0", "=a"(data), "Nd"(port), "")
    return data

def outb(port: u16, data: u8) -> void:
    unsafe:
        asm("outb %0, %1", "", "a"(data), "Nd"(port))
```

**TLB invalidation (OS paging):**
```python
def invlpg(addr: usize) -> void:
    unsafe:
        asm("invlpg (%0)", "", "r"(addr), "memory")
```

### Common Mistakes

**Mistake: using `asm()` outside `unsafe:`.**
```python
asm("hlt")    # compile error — asm requires unsafe:
```

**Mistake: missing the `"memory"` clobber on a barrier.**
```python
unsafe:
    asm("")          # NOT a barrier — compiler can reorder across it
    asm("", "", "", "memory")    # correct compiler barrier
```

### Best Practices

1. Wrap every `asm()` in a named function (e.g., `rdtsc()`, `invlpg()`) so call sites are readable.
2. Use `--emit c` to inspect the generated `__asm__` statements before running.
3. Include the `"memory"` clobber whenever the assembly reads or writes memory that the compiler must not cache in a register.

---

## `Pointer[void]` and Type Erasure

### When to use

Use `Pointer[void]` when bridging C APIs that use `void*` — for example `malloc`, `memcpy`, `qsort` callbacks, or plugin handles.

### How it works

```python
extern "C":
    def malloc(size: usize) -> Pointer[void]
    def free(ptr: Pointer[void]) -> void
    def memcpy(dst: Pointer[void], src: Pointer[void], n: usize) -> Pointer[void]

unsafe:
    mut raw = malloc(100 as usize)
    memcpy(raw, src_ptr as Pointer[void], 100 as usize)
    free(raw)
```

### Common Mistakes

**Mistake: casting `Pointer[void]` to the wrong type.**
```python
unsafe:
    mut p: Pointer[void] = malloc(4 as usize)
    # BAD — allocated 4 bytes, reading as int (8 bytes) — buffer overread
    mut v = (p as Pointer[int]).read()
```

### Best Practices

1. Cast `Pointer[void]` back to the exact element type that was originally allocated.
2. Keep the element type and the cast type in the same function to make it easy to verify.

---

## Summary of Best Practices

1. **Minimize scope.** Make every `unsafe:` block as small as possible.
2. **Comment the invariant.** Add a `# SAFETY:` comment explaining why the operation is sound.
3. **Wrap in safe abstractions.** Put `unsafe:` inside class methods; callers see a safe API.
4. **Always match `alloc` with `dealloc`.** Use the class wrapper pattern for any non-trivial allocation.
5. **Null-check C pointers.** Never dereference a pointer returned from C without checking for null.
6. **Use `--strict`.** Under `--strict`, `alloc` outside `unsafe:` is error `[U-1]`.
7. **Audit with search.** Find all unsafe usage in a project by searching for the literal string `unsafe:`.

---

Next: [Modules →](15_modules.md)
