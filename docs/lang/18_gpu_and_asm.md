# 18 — GPU Blocks and Inline Assembly

---

## Overview

Tauraro provides two mechanisms for low-level parallel and hardware-specific code:

| Feature | Purpose |
|---------|---------|
| `std.gpu.Gpu` | OpenMP-backed parallel dispatch (the supported way to run work across CPU cores) |
| `gpu:` block | **Deprecated.** Still parses for source compatibility, but compiles to a plain sequential block — no pragmas are emitted. Use `std.gpu.Gpu` instead. |
| `asm(...)` | Inline assembly — must be inside `unsafe:` |

> **Note:** `@parallel` and `@vectorize` loop-hint decorators described in earlier
> drafts of this document were never implemented and do not exist in the
> compiler. Do not use them — they are silently ignored by no codegen path
> (in fact, an unrecognized decorator is a compile error). Use `std.gpu.Gpu`
> or hand-written `gpu:`-free OpenMP via `extern "C"` if you need pragmas.

---

## `std.gpu.Gpu` — Parallel Dispatch

### When to use

Use `Gpu.parallel` when you have `n` independent units of work — a
data-parallel transform, an embarrassingly parallel simulation, a matrix
operation where rows are independent, or per-pixel image processing — and you
want to run them across all available CPU cores via OpenMP.

Do **not** use it for loops with loop-carried dependencies (where iteration
`i` reads results written by iteration `i-1`).

### How it works

```python
from std.gpu import Gpu

print(f"openmp available: {Gpu.has_openmp()}")
print(f"threads: {Gpu.num_threads()}")
```

`Gpu.parallel(n, fn_ptr)` runs `n` iterations in parallel via
`_tr_gpu_openmp_parallel_i64`, where `fn_ptr` is a `Pointer[void]` to a
function taking a single `int` (the iteration index). On builds without
OpenMP, it runs the same `n` iterations sequentially — no code changes
needed, and no error or warning.

```python
from std.gpu import Gpu

def square_at(i: int) -> void:
    output[i] = input[i] * input[i]   # output/input must be in scope (e.g. globals)

def main():
    unsafe:
        Gpu.parallel(n, square_at as Pointer[void])
```

**Enabling OpenMP at build time:**
```bash
# Linux / macOS:
tauraroc program.tr -fopenmp --run

# Windows with MinGW:
tauraroc program.tr -fopenmp -lgomp --run
```

Control thread count at runtime:
```bash
OMP_NUM_THREADS=8 ./program.exe
```

### Common Mistakes

**Mistake: a loop-carried dependency inside the dispatched function.**
```python
mut acc = 0
def add_at(i: int) -> void:
    acc += input[i]    # RACE — multiple threads write acc concurrently
```
Use `Atomic[int]` for reductions:
```python
mut acc: Atomic[int] = Atomic.new(0)
def add_at(i: int) -> void:
    acc.add(input[i])
```

**Mistake: expecting `Gpu.parallel` to help for small `n`.**
For small workloads the thread dispatch overhead can exceed the benefit.
Check `Gpu.has_openmp()` and benchmark with and without `-fopenmp`.

**Mistake: writing to a shared collection from the dispatched function.**
```python
def append_at(i: int) -> void:
    shared_list.append(i)    # RACE — append is not thread-safe
```
Write to pre-allocated indexed positions (`output[i] = ...`) instead.

### Best Practices

1. Only dispatch functions where every iteration writes to a different memory location.
2. Pre-allocate the output array before calling `Gpu.parallel` — do not call `append` from inside it.
3. Wrap the `Gpu.parallel` call in `unsafe:` (it takes a raw `Pointer[void]`).
4. Benchmark with and without `-fopenmp` to verify the parallelism is helping.

---

## The Deprecated `gpu:` Block

Older code may still contain `gpu:` blocks:

```python
gpu:
    for i in range(n):
        data[i] = compute(i)
```

The parser still accepts this syntax, but codegen now emits the body as a
plain sequential block with a `/* deprecated gpu: block - use std.gpu for GPU
dispatch */` comment — **no OpenMP pragmas are generated**, regardless of
`-fopenmp`. Existing `gpu:` blocks keep compiling and running correctly
(sequentially); they simply gain no parallelism. Migrate to `std.gpu.Gpu` (above)
to actually parallelize the loop.

---

## Inline Assembly: `asm()`

### When to use

Use `asm()` when you need to:

- Issue CPU-specific instructions not available through Tauraro's standard library (`rdtsc`, `cpuid`, `pause`, `hlt`)
- Emit precise memory barriers in lock-free data structures or device drivers
- Write bare-metal OS code (port I/O, TLB invalidation, interrupt control)

Inline assembly must always be inside `unsafe:`.

### How it works

**Simple form — no operands:**
```python
unsafe:
    asm("nop")     # no-op instruction
    asm("pause")   # x86 spin-loop hint (reduces power in busy-wait loops)
    asm("hlt")     # x86 halt CPU (use in bare-metal OS kernels only)
    asm("cli")     # x86 disable interrupts
    asm("sti")     # x86 enable interrupts
```

**Extended form — with operands:**
```
asm(code, outputs, inputs, clobbers)
```

| Argument | Description |
|----------|-------------|
| `code` | The assembly instruction string |
| `outputs` | Output operand constraints: `"=constraint"(var)` |
| `inputs` | Input operand constraints: `"constraint"(expr)` |
| `clobbers` | Registers/resources clobbered: `"reg"` or `"memory"` |

**GCC constraint notation:**

| Constraint | Meaning |
|-----------|---------|
| `"r"` | Any general-purpose register |
| `"m"` | Memory operand |
| `"i"` | Immediate integer constant |
| `"a"` | `rax`/`eax` register |
| `"d"` | `rdx`/`edx` register |
| `"=r"` | Output: write to a register, save to variable |
| `"=m"` | Output: write to memory |
| `"=A"` | Output: `rax:rdx` pair (64-bit `rdtsc` result) |
| `"+r"` | Read-write operand in a register |
| `"Nd"` | 8-bit constant or `dx` register (for `in`/`out` port instructions) |

**Memory barrier — compiler only (no instruction emitted):**
```python
unsafe:
    asm("", "", "", "memory")
```
Prevents the compiler from reordering memory accesses across this point. Use in lock-free algorithms and device drivers where the compiler must not cache values in registers across the barrier.

**Hardware memory fences:**
```python
unsafe:
    asm("mfence")    # full fence — all loads and stores ordered
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

**CPUID:**
```python
def cpuid(leaf: u32) -> void:
    mut eax: u32 = 0
    mut ebx: u32 = 0
    mut ecx: u32 = 0
    mut edx: u32 = 0
    unsafe:
        asm("cpuid", "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx))
    print(f"EAX={eax} EBX={ebx} ECX={ecx} EDX={edx}")
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

**Mistake: omitting the `"memory"` clobber on a barrier.**
```python
unsafe:
    asm("")          # NOT a barrier — compiler can reorder across it
    asm("", "", "", "memory")    # correct compiler barrier
```

**Mistake: using the wrong constraint for output operands.**
```python
unsafe:
    mut val: u32 = 0
    asm("rdtsc", "a"(val), "", "")    # WRONG — "a" is an input constraint
    asm("rdtsc", "=a"(val), "", "")   # correct — "=a" is output
```

**Mistake: forgetting to declare all clobbered registers.**
If the assembly modifies a register you did not declare in the clobber list, the compiler may use that register for something else — producing wrong code with no warning.

### Best Practices

1. Wrap every `asm()` invocation in a named function (`rdtsc()`, `invlpg()`, `inb()`) so call sites are self-documenting.
2. Use `--emit c` to inspect the generated `__asm__` statements and verify they are correct before running.
3. Always include `"memory"` in the clobber list when the assembly accesses memory that the compiler might cache in a register.
4. Test inline assembly on all target platforms — constraints and instruction names are architecture-specific.

---

## True GPU Programming (CUDA / HIP / OpenCL)

### When to use

`std.gpu.Gpu` maps to OpenMP CPU parallelism. For actual GPU kernel programming today, compile CUDA or HIP kernels separately and call them via `extern "C"`.

### How it works

```python
# kernel.cu — compiled separately with nvcc:
# extern "C" void gpu_dot_product(float* a, float* b, float* result, int n);

extern "C":
    def gpu_dot_product(
        a:      Pointer[f32],
        b:      Pointer[f32],
        result: Pointer[f32],
        n:      int
    ) -> void

def main():
    unsafe:
        mut a:      Pointer[f32] = alloc[f32](n)
        mut b:      Pointer[f32] = alloc[f32](n)
        mut result: Pointer[f32] = alloc[f32](1)
        # ... fill a and b ...
        gpu_dot_product(a, b, result, n)
        print(f"dot product = {result.read()}")
        dealloc(a)
        dealloc(b)
        dealloc(result)
```

Compile with:
```bash
nvcc -c kernel.cu -o kernel.o
tauraroc main.tr kernel.o --run
```

### Best Practices

1. Keep GPU kernel files (`.cu`, `.hip`) in a separate directory and compile them independently.
2. Declare only the `extern "C"` entry points in your Tauraro code — do not expose internal CUDA types.

---

## Inspecting Compiler Output

Use `--emit c` to see the C the compiler generates before passing it to GCC. This is essential for verifying that `asm()` contains the intended instructions, and for confirming that any leftover `gpu:` blocks compile to the documented sequential `/* deprecated gpu: block */` form rather than pragmas.

```bash
tauraroc --emit c program.tr
```

Example output for `asm("pause")`:
```c
__asm__ volatile("pause");
```

---

## Full Example: Combining `std.gpu.Gpu`, `sizeof`, and `asm()`

```python
from std.gpu import Gpu

const KB = 1024
const MB = 1024 * KB

def parallel_square(n: int) -> List[int]:
    mut result: List[int] = []
    mut i = 0
    while i < n:
        result.append(i * i)
        i += 1
    return result

def cpu_timing_demo() -> void:
    unsafe:
        asm("", "", "", "memory")    # compiler barrier before measurement
        mut start = 0
        asm("rdtsc", "=A"(start), "", "")
        # ... work to time ...
        asm("", "", "", "memory")    # compiler barrier after measurement
        mut end = 0
        asm("rdtsc", "=A"(end), "", "")
        print(f"cycles elapsed: {end - start}")

def main():
    print("--- sizeof ---")
    print(f"  sizeof(int)   = {sizeof(int)}")
    print(f"  sizeof(float) = {sizeof(float)}")
    print(f"  sizeof(bool)  = {sizeof(bool)}")
    print(f"  sizeof(char)  = {sizeof(char)}")

    print("--- parallel squares ---")
    mut squares = parallel_square(8)
    for x in squares:
        print(f"  {x}")

    print(f"--- gpu ({'openmp' if Gpu.has_openmp() else 'sequential'}, {Gpu.num_threads()} threads) ---")

    print("--- cpu timing ---")
    cpu_timing_demo()
```

---

Next: [Compiler Error Reference →](19_compiler_errors.md)
