# 14 — Unsafe Blocks and Raw Pointers

---

## Why `unsafe:` Exists

Tauraro's safety model is enforced by the compiler — it tracks ownership, prevents dangling pointers, and prevents double-free. But some operations are inherently outside what a compiler can verify:

- Raw pointer arithmetic (the value could point anywhere)
- Manual heap allocation (the compiler doesn't know when you'll `free()`)
- Inline assembly (arbitrary machine instructions)
- Calling external C functions with raw pointer arguments

These operations are not forbidden — they're quarantined behind the `unsafe:` keyword. Every `unsafe:` block is a promise that *you* have verified the invariants the compiler can't.

**The goal of `unsafe:` is containment, not avoidance.** Systems code genuinely needs these operations. The keyword makes them visible and auditable — a safety reviewer can search for `unsafe:` and find every low-level operation in the codebase.

---

## The unsafe: Block

```python
mut x: int = 42
mut y: int = 0

unsafe:
    mut p: Pointer[int] = &x    # take address of x
    y = p.read()                 # dereference: y = *p = 42
    p.write(100)                 # *p = 100 — x is now 100

print(f"y = {y}, x = {x}")     # y = 42, x = 100
```

**What requires `unsafe:`:**

| Operation | Why unsafe |
|-----------|-----------|
| `&x` address-of | Produces raw pointer the GC/ownership can't track |
| `p.read()` dereference | May access freed or invalid memory |
| `p.write(v)` write through pointer | May corrupt memory |
| `p.offset(n)` pointer arithmetic | May produce out-of-bounds pointer |
| `alloc[T](n)` manual allocation | Bypasses auto-free |
| `dealloc(ptr)` manual free | Bypasses ownership tracking |
| `asm(...)` inline assembly | Arbitrary machine effects |
| Pointer cast with `as Pointer[T]` | Type-unsound reinterpretation |

**What is still safe inside `unsafe:`:**
- Type checking still runs
- Ownership tracking for variables declared **outside** the unsafe block
- The compiler still protects `Own` variables declared in outer scope

---

## Raw Pointer Type: Pointer[T]

`Pointer[T]` is a raw C pointer (`T*`). The compiler does NOT:
- Track its lifetime
- Inject `free()` for it
- Check whether the pointed-to memory is valid

```python
mut n: int = 10

unsafe:
    mut p: Pointer[int] = &n      # p is a Pointer[int] (int* in C)
```

### Taking an Address

```python
unsafe:
    mut p: Pointer[int] = &n       # address of n
    mut q: Pointer[char] = &c      # address of c
```

`&x` compiles to `&x` in C — the address of the variable.

### Reading Through a Pointer

```python
unsafe:
    mut val = p.read()    # *p in C
```

### Writing Through a Pointer

```python
unsafe:
    p.write(99)    # *p = 99 in C
```

### Pointer Arithmetic: offset

```python
unsafe:
    mut base: Pointer[int] = buf
    mut elem = base.offset(3).read()    # *(buf + 3) in C
    base.offset(3).write(100)            # *(buf + 3) = 100 in C
```

`.offset(n)` advances by `n` elements of type `T` — by `n * sizeof(T)` bytes. This matches C pointer arithmetic semantics.

### Pointer Comparison

```python
unsafe:
    if p as usize == 0 as usize:    # null check
        raise("null pointer")

    if p as usize == q as usize:    # pointer equality
        print("same address")
```

Cast to `usize` to compare addresses as integers.

---

## Manual Allocation: alloc and dealloc

`alloc[T](n)` allocates heap memory for `n` elements of type `T`. Returns a `Pointer[T]`.

```python
unsafe:
    mut buf: Pointer[int] = alloc[int](8)    # 8 ints, zero-initialized

    mut i = 0
    while i < 8:
        buf.offset(i).write(i * 10)
        i = i + 1

    i = 0
    while i < 8:
        print(buf.offset(i).read())     # 0 10 20 30 40 50 60 70
        i = i + 1

    dealloc(buf)    # free the memory
```

**Ownership contract:** `alloc` gives you a pointer that **you** must `dealloc`. The compiler will not inject `free()` for it. Forget to call `dealloc` → memory leak. Call `dealloc` twice → heap corruption. Allocation failures abort immediately — you never need to check for null.

**Best practice:** Wrap every `alloc`/`dealloc` pair inside a class with `init` and `drop` methods. Then the class instance gets normal `Own` tracking:

```python
class Buffer:
    pub ptr: Pointer[int]
    pub size: int

extend Buffer:
    pub def init(n: int) -> Buffer:
        mut b = Buffer()
        b.size = n
        unsafe:
            b.ptr = alloc[int](n)
        return b

    pub def get(self, i: int) -> int:
        unsafe: return self.ptr.offset(i).read()

    pub def set(self, i: int, v: int) -> void:
        unsafe: self.ptr.offset(i).write(v)

    pub def drop(self) -> void:
        unsafe: dealloc(self.ptr)

def main():
    mut buf = Buffer.init(4)
    buf.set(0, 10)
    buf.set(1, 20)
    buf.set(2, 30)
    buf.set(3, 40)
    mut i = 0
    while i < buf.size:
        print(buf.get(i))
        i = i + 1
    buf.drop()    # explicit destructor call — buf.ptr is freed here
```

---

## Pointer Casts

Any pointer can be cast to/from `Pointer[void]` and `usize`:

```python
unsafe:
    mut n: int = 42
    mut p: Pointer[int] = &n

    # To void*:
    mut vp: Pointer[void] = p as Pointer[void]

    # Back to typed pointer:
    mut tp: Pointer[int] = vp as Pointer[int]

    # Pointer to integer (the address):
    mut addr: usize = p as usize
    print(f"address = {addr}")

    # Integer to pointer (dangerous!):
    mut rp: Pointer[int] = addr as Pointer[int]
```

These casts are zero-cost reinterpretations of the pointer value.

---

## Null Pointer Pattern

The idiom for null pointers:

```python
mut p: Pointer[int] = 0 as Pointer[int]    # null pointer

unsafe:
    if p as usize == 0 as usize:
        print("p is null")
    else:
        print(p.read())
```

Or using `none` (equivalent for pointer types):
```python
mut p: Pointer[int] = none
```

---

## sizeof Operator

`sizeof(T)` returns the size of a type in bytes:

```python
mut int_size    = sizeof(int)     # 8  (64-bit int)
mut float_size  = sizeof(float)   # 8  (double)
mut bool_size   = sizeof(bool)    # 1
mut char_size   = sizeof(char)    # 1
mut ptr_size    = sizeof(Pointer[void])    # 8  (64-bit pointer)
```

For class types:
```python
class Vec3:
    pub x: float
    pub y: float
    pub z: float

mut vec3_size = sizeof(Vec3)    # 24 bytes (3 × 8-byte doubles)
```

`sizeof` compiles to C's `sizeof` operator. It is evaluated at compile time.

**Use case:** Computing array sizes, alignment, serialization:
```python
unsafe:
    mut buf: Pointer[u8] = alloc[u8](sizeof(Header))
    # ... fill in header fields via pointer
```

---

## Inline Assembly: asm()

`asm(...)` emits inline assembly inside an `unsafe:` block:

### Simple Form (no operands)

```python
unsafe:
    asm("nop")          # no-op instruction
    asm("pause")        # x86 spin-loop hint
    asm("mfence")       # memory fence
```

### Extended Form (with operands)

```python
unsafe:
    mut cycles: int = 0
    asm("rdtsc", "=A"(cycles), "", "")    # read time-stamp counter
```

Four-argument form: `asm(code, outputs, inputs, clobbers)`:
- `code` — the assembly instruction(s)
- `outputs` — constraint string for output operands
- `inputs` — constraint string for input operands
- `clobbers` — registers modified (e.g., `"memory"` for memory barriers)

### Memory Barrier

```python
unsafe:
    asm("", "", "", "memory")    # compiler memory barrier
```

Prevents the compiler from reordering memory operations across this point.

### x86 Examples

```python
unsafe:
    # Halt (used in bare-metal kernels)
    asm("hlt")

    # Port I/O (x86 only)
    mut port: u16 = 0x60 as u16
    mut data: u8 = 0
    asm("inb %1, %0", "=a"(data), "Nd"(port), "")

    # Flush TLB entry
    mut addr: usize = 0
    asm("invlpg (%0)", "", "r"(addr), "memory")
```

**Best practice:** Test inline assembly output with `--emit c` to see the generated `__asm__` statements.

---

## Pointer[void] and Type Erasure

`Pointer[void]` (`void*` in C) is used to hold a pointer of unknown type. Common in C FFI:

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

---

## Checking Pointer Validity

The runtime includes a helper used internally for debugging:

```python
# _is_invalid_ptr(addr) — returns true if addr looks like a bad pointer
# (null, dangling, debug-fill patterns like 0xcdcdcdcd, odd-aligned, etc.)
# Not guaranteed to catch all bad pointers — this is heuristic only
```

For production code, rely on architecture (keep allocations in known ranges) and correctness arguments, not runtime checks.

---

## Best Practices for unsafe:

1. **Minimize scope.** Make `unsafe:` blocks as small as possible — just the raw operation, nothing else.

2. **Wrap in safe abstractions.** Put the unsafe code inside a class method with a safe Tauraro interface. Callers see the safe API; the `unsafe:` is invisible to them.

3. **Comment the invariant.** Every `unsafe:` block should have a comment explaining why the operation is sound:
   ```python
   unsafe:
       # SAFETY: buf was allocated with alloc[int](size) and i is in [0, size)
       buf.offset(i).read()
   ```

4. **Prefer the standard safe wrappers.** `alloc[T](n)` + `dealloc` is safer than manual allocation because `alloc` aborts on out-of-memory rather than returning null.

5. **Don't use pointer arithmetic to iterate lists.** Use `List[T]` with index access — it's the same performance and stays tracked.

---

Next: [Modules →](15_modules.md)
