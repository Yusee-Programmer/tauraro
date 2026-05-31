# 18 — GPU Blocks and Inline Assembly

---

## gpu: Blocks — Parallel Execution

### What gpu: Does

`gpu:` marks a block of code as a candidate for parallel execution. In the current compiler, `gpu:` blocks compile to **OpenMP parallel pragmas**. When OpenMP is available, iterations run in parallel across CPU cores. Without OpenMP, the code runs sequentially — no changes needed.

```python
gpu:
    for i in range(1000):
        data[i] = compute(i)
```

With `-fopenmp`: the loop runs in parallel across all CPU cores. Without `-fopenmp`: the loop runs sequentially, as if `gpu:` wasn't there.

### Enabling OpenMP

```bash
# GCC with OpenMP:
tauraroc program.tr -fopenmp --run

# On Windows with MinGW:
tauraroc program.tr -fopenmp -lgomp --run
```

Or set the environment variable that controls the thread count:
```bash
OMP_NUM_THREADS=8 ./program.exe
```

### Loop Parallelism Rules

**Safe to parallelize (each iteration is independent):**
```python
gpu:
    for i in range(n):
        output[i] = input[i] * input[i]    # no dependency on other iterations
```

**Unsafe (iterations depend on each other):**
```python
gpu:
    for i in range(1, n):
        result[i] = result[i-1] + delta[i]   # RACE: i depends on i-1
```

**Rule for gpu: loops:** Only use `gpu:` on loops where iterations are **fully independent** — no loop-carried dependencies, no shared mutable state without synchronization.

### Non-Loop Statements in gpu:

Statements that aren't `for` loops inside a `gpu:` block run in the parallel region but don't get per-statement parallelism directives:

```python
gpu:
    setup_parallel_data()    # runs in parallel region but as a single-threaded stmt
    for i in range(n):       # gets #pragma omp for — truly parallel
        process(i)
    collect_results()        # runs after the parallel loop
```

### gpu: Nesting

Nested `gpu:` blocks are supported (the compiler tracks depth with `in_gpu_block`):

```python
gpu:
    for i in range(rows):
        gpu:
            for j in range(cols):
                matrix[i * cols + j] = f(i, j)
```

This generates nested `#pragma omp parallel` sections. Whether the inner parallelism is effective depends on OpenMP nested parallelism settings.

### True GPU (CUDA/HIP/OpenCL)

The `gpu:` keyword is designed to be forward-compatible with true GPU compute. Current compiler: CPU OpenMP. Future: CUDA/HIP kernel dispatch.

For actual GPU programming today, compile CUDA/HIP kernels separately and call them via `extern "C"`:

```python
# kernel.cu (compiled separately with nvcc)
# extern "C" void gpu_dot_product(float* a, float* b, float* result, int n);

extern "C":
    def gpu_dot_product(a: Pointer[f32], b: Pointer[f32], result: Pointer[f32], n: int) -> void

def main():
    # ... allocate a, b, result ...
    unsafe:
        gpu_dot_product(a, b, result, n)
```

---

## Inline Assembly: asm()

Inline assembly embeds raw machine instructions directly in the compiled C. It must be inside `unsafe:`.

### Simple Form (No Operands)

```python
unsafe:
    asm("nop")           # x86: no-operation
    asm("pause")         # x86: spin-loop hint (reduce power in busy-wait)
    asm("hlt")           # x86: halt CPU (use in bare-metal OS kernels only)
    asm("cli")           # x86: disable interrupts
    asm("sti")           # x86: enable interrupts
```

### Extended Form (With Operands)

```python
unsafe:
    mut cycles: int = 0
    asm("rdtsc", "=A"(cycles), "", "")
```

Four-argument `asm` syntax:
```
asm(code, outputs, inputs, clobbers)
```

| Argument | Description |
|----------|-------------|
| `code` | The assembly instruction string |
| `outputs` | Output operand constraints: `"=constraint"(var)` |
| `inputs` | Input operand constraints: `"constraint"(expr)` |
| `clobbers` | Registers clobbered: `"reg1", "reg2"`, or `"memory"` |

### GCC Constraint Notation

| Constraint | Meaning |
|-----------|---------|
| `"r"` | Any general-purpose register |
| `"m"` | Memory operand |
| `"i"` | Immediate integer constant |
| `"a"` | `rax`/`eax` register |
| `"d"` | `rdx`/`edx` register |
| `"=r"` | Output: write to a register, save to variable |
| `"=m"` | Output: write to memory |
| `"=A"` | Output: `rax:rdx` pair (for 64-bit rdtsc result) |
| `"+r"` | Read-write operand in a register |
| `"Nd"` | 8-bit constant or `dx` register (for in/out port instructions) |

### Memory Barrier

A compiler memory barrier prevents the compiler from reordering memory accesses across the barrier:

```python
unsafe:
    asm("", "", "", "memory")
```

This does not emit any instruction — it's a pure compiler directive. Use it when you need to ensure all preceding memory writes are visible before subsequent reads, in lock-free data structures or device driver code.

For a hardware memory fence:
```python
unsafe:
    asm("mfence")    # full memory fence (all loads/stores ordered)
    asm("lfence")    # load fence
    asm("sfence")    # store fence
```

### x86-64 Examples

**Read the CPU timestamp counter (cycle count):**
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

**x86 Port I/O (for bare-metal OS):**
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

**Invalidate TLB entry (for OS paging):**
```python
def invlpg(addr: usize) -> void:
    unsafe:
        asm("invlpg (%0)", "", "r"(addr), "memory")
```

---

## Bare-Metal Full Example

A complete example combining `sizeof`, bit ops, `Pointer[T]`, `alloc`/`dealloc`, `unsafe:`, `asm`, and `gpu:`:

```python
const KB = 1024
const MB = 1024 * KB

def popcount(n: int) -> int:
    mut count = 0
    mut x = n
    while x != 0:
        count = count + (x & 1)
        x = x >> 1
    return count

def bit_reverse_u32(n: u32) -> u32:
    mut result: u32 = 0 as u32
    mut i = 0
    while i < 32:
        result = (result << 1 as u32) | (n & 1 as u32)
        n = n >> 1 as u32
        i = i + 1
    return result

def parallel_square(n: int) -> List[int]:
    mut result: List[int] = []
    mut i = 0
    while i < n:
        result.append(0)
        i = i + 1

    gpu:
        for i in range(n):
            result[i] = i * i

    return result

def raw_memory_demo() -> void:
    mut element_count = 8
    mut byte_size = element_count * sizeof(int)

    unsafe:
        mut buf: Pointer[int] = alloc[int](element_count)

        mut i = 0
        while i < element_count:
            buf.offset(i).write(i * 10)
            i = i + 1

        print(f"sizeof(int)   = {sizeof(int)}")
        print(f"buffer bytes  = {byte_size}")

        i = 0
        while i < element_count:
            print(f"  buf[{i}] = {buf.offset(i).read()}")
            i = i + 1

        dealloc(buf)

def cpu_timing_demo() -> void:
    unsafe:
        mut before: int = 0
        mut after: int = 0
        asm("", "", "", "memory")    # barrier before measurement
        # (real rdtsc would go here — simplified for portability)
        asm("", "", "", "memory")    # barrier after measurement
        print(f"timing measurement placeholder")

def main():
    print("--- sizeof ---")
    print(f"  sizeof(int)   = {sizeof(int)}")
    print(f"  sizeof(float) = {sizeof(float)}")
    print(f"  sizeof(bool)  = {sizeof(bool)}")
    print(f"  sizeof(char)  = {sizeof(char)}")

    print("--- bit operations ---")
    print(f"  popcount(0b10110101) = {popcount(0b10110101)}")
    print(f"  bit_reverse_u32(1)   = {bit_reverse_u32(1 as u32)}")

    print("--- parallel squares (gpu:) ---")
    mut squares = parallel_square(8)
    for x in squares: print(f"  {x}")

    print("--- raw memory ---")
    raw_memory_demo()
```

---

## Inspecting What the Compiler Produces

Use `--emit c` to see what C the compiler generates before passing it to GCC:

```bash
tauraroc --emit c program.tr
```

This is useful for verifying that `gpu:` blocks generate the correct OpenMP directives and that `asm()` contains the intended instructions.

---

Next: [Compiler Error Reference →](19_compiler_errors.md)
