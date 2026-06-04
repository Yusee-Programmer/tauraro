# 18 — GPU Blocks and Inline Assembly

---

## Overview

Tauraro provides two mechanisms for low-level parallel and hardware-specific code:

| Feature | Purpose |
|---------|---------|
| `gpu:` block | Parallel loop execution via OpenMP; forward-compatible with GPU backends |
| `@parallel` / `@vectorize` | Loop hints for parallelism and SIMD vectorization |
| `asm(...)` | Inline assembly — must be inside `unsafe:` |

---

## `gpu:` Blocks — Parallel Loop Execution

### When to use

Use `gpu:` when you have a loop whose iterations are **fully independent** and you want to run them in parallel across all available CPU cores (or, in future compiler versions, on a GPU).

Use cases:
- Data-parallel transforms: squaring, scaling, normalizing arrays
- Embarrassingly parallel simulations
- Matrix operations where rows are independent
- Image processing — per-pixel transforms

Do **not** use `gpu:` on loops with loop-carried dependencies (where iteration `i` reads results from iteration `i-1`).

### How it works

`gpu:` marks the enclosed code as a candidate for parallel execution. In the current compiler, `gpu:` generates **OpenMP parallel pragmas**. With `-fopenmp`, iterations run in parallel across CPU cores. Without `-fopenmp`, the code runs sequentially — no code changes needed.

**Basic parallel loop:**
```python
gpu:
    for i in range(1000):
        data[i] = compute(i)
```

With `-fopenmp` this compiles to:
```c
#pragma omp parallel for
for (int64_t i = 0; i < 1000; i++) {
    data[i] = compute(i);
}
```

**Enabling OpenMP:**
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

**Safe to parallelize — iterations are independent:**
```python
gpu:
    for i in range(n):
        output[i] = input[i] * input[i]    # no dependency on other iterations
```

**Unsafe to parallelize — loop-carried dependency:**
```python
gpu:
    for i in range(1, n):
        result[i] = result[i-1] + delta[i]    # RACE: i reads result written by i-1
```

**Non-loop statements in a `gpu:` block** run inside the parallel region but without per-statement parallelism directives:
```python
gpu:
    setup()               # runs in parallel region, single-threaded
    for i in range(n):    # gets #pragma omp for — truly parallel
        process(i)
    collect()             # runs after parallel loop
```

**Nested `gpu:` blocks:**
```python
gpu:
    for i in range(rows):
        gpu:
            for j in range(cols):
                matrix[i * cols + j] = f(i, j)
```
This generates nested `#pragma omp parallel` sections. Whether the inner parallelism is effective depends on the OpenMP runtime's nested parallelism setting (`OMP_NESTED=true`).

### Common Mistakes

**Mistake: parallelizing a loop with a loop-carried dependency.**
```python
mut acc = 0
gpu:
    for i in range(n):
        acc += input[i]    # RACE — multiple threads write acc concurrently
```
Use a reduction pattern or `Atomic[int]` for reductions:
```python
mut acc: Atomic[int] = Atomic.new(0)
gpu:
    for i in range(n):
        acc.add(input[i])
```

**Mistake: expecting `gpu:` to parallelize without `-fopenmp`.**
Without `-fopenmp`, `gpu:` is a no-op — the loop runs sequentially. There is no error or warning.

**Mistake: using `gpu:` on a loop that modifies a shared data structure.**
```python
gpu:
    for i in range(n):
        shared_list.append(i)    # RACE — append is not thread-safe
```
Write to pre-allocated indexed positions (`output[i] = ...`) instead of using append.

### Best Practices

1. Only use `gpu:` on loops where every iteration writes to a different memory location.
2. Pre-allocate the output array before the `gpu:` block — do not call `append` inside it.
3. Use `--emit c` to inspect the generated OpenMP pragmas before benchmarking.
4. Benchmark with and without `-fopenmp` to verify the parallelism is helping — for small `n`, the thread overhead may exceed the benefit.

---

## `@parallel` and `@vectorize` Loop Hints

### When to use

Use `@parallel` on a `for` loop when you want a single loop parallelized without wrapping the whole block in `gpu:`. Use `@vectorize` to hint that the loop body is SIMD-vectorizable.

### How it works

```python
@parallel
for i in range(n):
    output[i] = a[i] + b[i]

@vectorize
for i in range(n):
    result[i] = input[i] * 2.0
```

`@parallel` generates `#pragma omp parallel for`. `@vectorize` generates `#pragma omp simd`. Both require `-fopenmp` to take effect.

### Common Mistakes

The same dependency rules apply as for `gpu:` — only use these hints on independent loops.

### Best Practices

1. Use `@vectorize` on tight arithmetic loops operating on `f32` or `float` arrays — the compiler can emit SIMD instructions.
2. Combine `@parallel @vectorize` on the same loop for both multi-core and SIMD parallelism.

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

The `gpu:` keyword is designed to be forward-compatible with true GPU compute. In the current compiler it maps to OpenMP CPU parallelism. For actual GPU kernel programming today, compile CUDA or HIP kernels separately and call them via `extern "C"`.

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

Use `--emit c` to see the C the compiler generates before passing it to GCC. This is essential for verifying that `gpu:` blocks produce the expected OpenMP directives and that `asm()` contains the intended instructions.

```bash
tauraroc --emit c program.tr
```

Example output for a `gpu:` loop:
```c
#pragma omp parallel for
for (int64_t i = 0; i < 1000; i++) {
    data[i] = compute(i);
}
```

Example output for `asm("pause")`:
```c
__asm__ volatile("pause");
```

---

## Full Example: Combining `gpu:`, `sizeof`, and `asm()`

```python
const KB = 1024
const MB = 1024 * KB

def parallel_square(n: int) -> List[int]:
    mut result: List[int] = []
    mut i = 0
    while i < n:
        result.append(0)
        i += 1

    gpu:
        for i in range(n):
            result[i] = i * i    # each iteration writes a distinct index

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

    print("--- cpu timing ---")
    cpu_timing_demo()
```

---

Next: [Compiler Error Reference →](19_compiler_errors.md)
