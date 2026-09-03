# `std.gpu` — GPU compute

Portable GPU computing for Tauraro. A unified API that runs on **CUDA**,
**OpenCL**, or the **CPU**, chosen automatically at runtime — with **no build- or
link-time dependency on any GPU SDK**. The vendor driver is loaded dynamically
at first use (`nvcuda.dll`/`libcuda.so`, `OpenCL.dll`/`libOpenCL.so`); when no
GPU is present the backend degrades to the CPU so buffer code stays correct
everywhere.

```tauraro
from std.gpu import Device, Backend, Buffer, Module, Kernel, Dim3, Gpu
```

There are two ways to use it:

1. **Device + buffer + prebuilt kernel** — allocate typed device memory, copy
   data, and launch kernels you author in Tauraro (`@kernel`, compiled to
   PTX/SPIR-V — see [Writing kernels](#writing-kernels-kernel)) or in
   OpenCL-C source.
2. **CPU data-parallel dispatch** — `Gpu.parallel` runs an index range across
   all CPU cores via OpenMP.

---

## Contents

- [Quick start](#quick-start)
- [`Device` — discovery & control](#device--discovery--control)
- [`Buffer[T]` — device memory](#buffert--device-memory)
- [`Module` / `Kernel` / `Dim3` — kernels](#module--kernel--dim3--kernels)
- [Writing kernels (`@kernel`)](#writing-kernels-kernel)
- [The kernel build workflow](#the-kernel-build-workflow)
- [`Gpu` — CPU parallel dispatch (OpenMP)](#gpu--cpu-parallel-dispatch-openmp)
- [Type-width rules](#type-width-rules)
- [Backend & driver notes](#backend--driver-notes)
- [API reference](#api-reference)

---

## Quick start

Device query and a buffer round-trip run correctly on **any** machine (on the
CPU fallback the "device" buffer is host memory):

```tauraro
from std.gpu import Device, Buffer
from std.core.vec import Vec

def main():
    Device.detect()
    print(Device.backend_name())          # "CUDA" / "OpenCL" / "CPU"
    print(Device.name())                  # e.g. "NVIDIA GeForce RTX 4090"

    mut host = Vec[float].init(4)
    host.push(1.0); host.push(2.0); host.push(3.0); host.push(4.0)

    mut dev = Buffer[float].from_vec(host)  # upload
    mut back = dev.download()               # download -> Vec[float]
    dev.free()
```

---

## `Device` — discovery & control

`Device` is a static namespace over the process-wide GPU context. Backend
selection is lazy (on first use) or forced with `Device.detect()`.

| Method | Returns | Description |
|---|---|---|
| `Device.detect()` | `Backend` | Force backend selection now; returns the active backend |
| `Device.backend()` | `Backend` | The active `Backend` (`Cpu` / `Cuda` / `OpenCL`) |
| `Device.backend_name()` | `str` | `"CUDA"` / `"OpenCL"` / `"CPU"` |
| `Device.name()` | `str` | Selected device name (or a CPU note) |
| `Device.count()` | `int` | Number of GPU devices found (0 on CPU) |
| `Device.has_gpu()` | `bool` | True when a real GPU backend is active |
| `Device.compute_capability()` | `int` | CUDA compute capability as `major*10+minor` (e.g. `89`); 0 otherwise |
| `Device.total_memory()` | `int` | Device memory in bytes (0 on CPU) |
| `Device.synchronize()` | `void` | Block until all queued device work finishes |
| `Device.last_error()` | `str` | Last GPU error message (`""` when none) |

`Backend` is an enum: `Backend.Cpu`, `Backend.Cuda`, `Backend.OpenCL`
(with a `.name()` method). Selection order is **CUDA → OpenCL → CPU**.

---

## `Buffer[T]` — device memory

`Buffer[T]` owns a block of device memory holding `len` elements of type `T`.
The element size is `sizeof(T)`, so the same code is correct for `int`,
`float`, and value-type structs. Call `free()` when done — device lifetime is
explicit (buffers are not auto-dropped).

```tauraro
mut a = Buffer[float].alloc(1024)      # 1024 uninitialized elements
mut b = Buffer[float].zeros(1024)      # 1024 zeroed elements
mut c = Buffer[float].from_vec(host)   # upload a Vec[float]
mut back = c.download()                # download -> Vec[float]
a.free(); b.free(); c.free()
```

| Method | Description |
|---|---|
| `Buffer[T].alloc(n)` | Allocate `n` uninitialized elements |
| `Buffer[T].zeros(n)` | Allocate `n` zeroed elements |
| `Buffer[T].from_vec(v)` | Upload a host `Vec[T]` into a fresh buffer |
| `b.upload(v)` | Copy a host `Vec[T]` into `b` |
| `b.download()` | Download `b` into a fresh `Vec[T]` |
| `b.copy_from_ptr(src, n)` | Copy `n` elements from a host `Pointer[T]` |
| `b.copy_to_ptr(dst, n)` | Copy `n` elements to a host `Pointer[T]` |
| `b.fill_bytes(v)` | Set every byte to `v` (like `memset`) |
| `b.byte_size()` | Allocated size in bytes |
| `b.is_valid()` | True when the allocation succeeded |
| `b.len` | Element count (field) |
| `b.handle` | Opaque device handle (field), passed to `Kernel.arg_buffer` |
| `b.free()` | Release the device allocation |

On the CPU fallback these are host `malloc` + `memcpy`, so `from_vec` /
`download` round-trip correctly with no GPU.

---

## `Module` / `Kernel` / `Dim3` — kernels

A `Module` is a compiled kernel container; a `Kernel` is one entry point in it.

```tauraro
mut m = Module.load_file("kernels.spv")   # precompiled SPIR-V (or .ptx for CUDA)
mut k = m.kernel("saxpy")
k.arg_buffer(0, y.handle).arg_buffer(1, x.handle).arg_f64(2, 2.0).arg_i32(3, n)
k.launch1d(n, 256)                          # ceil(n/256) blocks of 256 threads
Device.synchronize()
```

**Module** loaders:

| Method | Backend | Description |
|---|---|---|
| `Module.embedded()` | any | Load the module **auto-embedded at build time** via `--gpu-embed` — no file, no separate `llc` step (**the easiest entry**) |
| `Module.load_file(path)` | any | Load a precompiled `.spv` (OpenCL) or `.ptx` (CUDA) file |
| `Module.load_ptx(ptx)` | CUDA | Load a PTX/cubin image from a string |
| `Module.load_spirv(il, n)` | OpenCL | Load `n` bytes of SPIR-V from a pointer |
| `Module.load_il(il, n)` | any | SPIR-V (OpenCL) or PTX/cubin (CUDA) from a pointer |
| `Module.load_source(src)` | OpenCL | Compile OpenCL-C source at runtime |
| `m.kernel(name)` | | Look up a kernel entry point |
| `m.is_valid()` / `m.free()` | | Validity check / release |

**Kernel** — bind arguments positionally (chainable), then launch:

| Method | Description |
|---|---|
| `k.arg_buffer(idx, handle)` | Bind a device buffer (`buf.handle`) as arg `idx` |
| `k.arg_i32(idx, v)` / `arg_i64` | Bind a 32-/64-bit integer scalar |
| `k.arg_f32(idx, v)` / `arg_f64` | Bind a 32-/64-bit float scalar |
| `k.launch(grid, block)` | Launch with `Dim3` grid (blocks) and block (threads/block) |
| `k.launch1d(n, block_size)` | 1-D launch covering `n` elements (grid computed by ceil-division) |
| `k.is_valid()` / `k.free()` | Validity check / release |
| Return | `launch*` returns 0 on success, non-zero on failure (see `Device.last_error()`) |

**Dim3** — a 3-D extent: `Dim3.init(x,y,z)`, `Dim3.of(x)` (`(x,1,1)`),
`Dim3.xy(x,y)` (`(x,y,1)`), with fields `.x/.y/.z`.

> `launch1d(n, bs)` pads the global size up to a multiple of `bs`, so the kernel
> **must** bounds-check `if i < n` (standard GPU practice).

---

## Writing kernels (`@kernel`)

Kernels are written in Tauraro and compiled to GPU IR by the `--emit gpu`
backend. A `@kernel def` is also **valid host code** (it runs serially on the
CPU), which makes it its own correctness reference.

```tauraro
from std.gpu import gpu_global_id

@kernel
def saxpy(y: Pointer[f32], x: Pointer[f32], a: f32, n: i32):
    mut i = gpu_global_id(0)
    if i < n:
        unsafe:
            y.offset(i).write(a * x.offset(i).read() + y.offset(i).read())
```

### The kernel subset

A kernel is a data-parallel leaf. It may use:

- **Parameters**: `Pointer[T]` (a global-memory buffer), and scalars
  (`i32`, `i64`/`int`, `f32`, `f64`/`float`, `bool`). No `self`; returns nothing.
- **Body**: local `mut` variables, arithmetic and comparisons, `if`/`while`,
  and indexed access `p.offset(i).read()` / `p.offset(i).write(v)` (in an
  `unsafe:` block, like all raw-pointer access).
- **Index/sync builtins** (from `std.gpu`):

  | Builtin | Meaning |
  |---|---|
  | `gpu_global_id(d)` | Global work-item index in dimension `d` (0/1/2) |
  | `gpu_local_id(d)` | Work-item index within its block |
  | `gpu_group_id(d)` | Block index |
  | `gpu_local_size(d)` | Block dimension (threads per block) |
  | `gpu_global_size(d)` | Total grid dimension |
  | `gpu_num_groups(d)` | Number of blocks |
  | `gpu_barrier()` | Block-wide synchronization barrier |

Anything outside this subset (heap allocation, `str`, `List`/`Dict`, closures,
recursion, exceptions, method calls other than pointer `read`/`write` and
`@device` calls) is a compile error from the GPU backend — it never
miscompiles silently.

### Multi-dimensional grids

Every index builtin takes a dimension argument (`0`/`1`/`2` = x/y/z), so a
2-D/3-D kernel needs no special syntax — just call `gpu_global_id(1)` for the
y index (and `gpu_global_id(2)` for z):

```tauraro
@kernel
def transpose(dst: Pointer[f32], src: Pointer[f32], w: i32, h: i32):
    mut x = gpu_global_id(0)
    mut y = gpu_global_id(1)
    if x < w and y < h:
        unsafe:
            dst.offset(y + x * h).write(src.offset(x + y * w).read())
```

Launch with a 2-D grid via `Dim3.of(bx, by)` for both the grid and block dims.

### `@device` helper functions

`@device def f(...) -> T: ...` marks a plain GPU-side helper — not a kernel
entry point — that a `@kernel` (or another `@device` fn) can call, regardless
of declaration order:

```tauraro
@device
def scale_bias(v: f32, k: f32, b: f32) -> f32:
    return v * k + b

@kernel
def apply(a: Pointer[f32], k: f32, b: f32, n: i32):
    mut i = gpu_global_id(0)
    if i < n:
        unsafe:
            a.offset(i).write(scale_bias(a.offset(i).read(), k, b))
```

Unlike a `@kernel` (always returns nothing), a `@device` fn returns a scalar
or `Pointer[T]` like a normal function.

### Type widths

GPU kernels usually want 32-bit types. Use `f32`/`i32` for `float`/`int`
buffers. **Note** `float` = `f64` and `int` = `i64` in Tauraro, so `Buffer[float]`
is a buffer of *doubles* — a matching kernel must use `Pointer[f64]` and
`arg_f64`. See [Type-width rules](#type-width-rules).

---

## The kernel build workflow

### Easiest: `--gpu-embed` (auto-embed)

Build with `--gpu-embed` and the compiler auto-compiles every `@kernel` to a GPU
module and **bakes it into the binary** — no separate `llc` step, no `.spv`/`.ptx`
file, no paths to manage. Load it with `Module.embedded()`:

```sh
tauraroc app.tr --gpu-embed spirv -o app     # OpenCL / Vulkan  (needs llc on PATH)
tauraroc app.tr --gpu-embed nvptx -o app     # CUDA
```

```tauraro
mut k = Module.embedded().kernel("saxpy")     # the whole build step is gone
k.arg_buffer(0, y.handle).arg_buffer(1, x.handle).arg_f64(2, 2.0).arg_i32(3, n)
k.launch1d(n, 256)
```

`Module.embedded()` returns an invalid module (see `is_valid()`) when the program
was built without `--gpu-embed`, so it degrades gracefully.

Even simpler — **`kernel_launch`** collapses load + per-arg bind + launch into one
typed call. The compiler marshals each argument by the `@kernel`'s signature (a
`Buffer[T]` goes as a device buffer, scalars by width), so you don't touch
`Module`/`Kernel`/`arg_*`:

```tauraro
from std.gpu import Dim3
kernel_launch("saxpy", Dim3.of(blocks), Dim3.of(256), y, x, 2.0, n)
```

`kernel_launch("name", grid, block, args…)` requires the kernels to be embedded
(`--gpu-embed`) and returns the launch rc (0 = ok). Buffer args may be a
`Buffer[T]` (its handle is taken automatically) or a raw device handle.

Passing the wrong number of trailing args is caught at **compile time**, not
launch time — the generated code names the exact kernel and expected/actual
counts:

```
error: implicit declaration of function '_tr_gpu_kernel_launch_ARG_COUNT_MISMATCH_saxpy_wants4_got3'
```

### Explicit: `--emit gpu` + a `.spv`/`.ptx` file

```
@kernel def   ──tauraroc --emit gpu──▶  LLVM IR  ──llc──▶  .spv (OpenCL) / .ptx (CUDA)
                                                              │
host program  ──────────────  Module.load_file(path) ────────┘  ──▶ launch
```

**1. Compile the kernels** (helper script wraps `tauraroc --emit gpu | llc`):

```sh
bash scripts/gpu_compile.sh tests/gpu/kernels.tr spirv kernels.spv    # OpenCL / Vulkan
bash scripts/gpu_compile.sh tests/gpu/kernels.tr nvptx kernels.ptx    # CUDA (SM=sm_80 …)
```

Or by hand:

```sh
tauraroc kernels.tr --emit gpu --gpu-target spirv | llc -mtriple=spirv64-unknown-unknown -filetype=obj -o kernels.spv
tauraroc kernels.tr --emit gpu --gpu-target nvptx | llc -mtriple=nvptx64-nvidia-cuda -mcpu=sm_50 -o kernels.ptx
```

**2. Load and launch** from the host program (see `tests/gpu/saxpy_host.tr`):

```tauraro
mut m = Module.load_file("kernels.spv")     # or "kernels.ptx" on CUDA
mut k = m.kernel("saxpy")
k.arg_buffer(0, y.handle).arg_buffer(1, x.handle).arg_f64(2, 3.0).arg_i32(3, n)
k.launch1d(n, 64)
Device.synchronize()
mut result = y.download()
```

---

## `Gpu` — CPU parallel dispatch (OpenMP)

The CPU compute path. `Gpu.parallel(n, fn_ptr)` runs `n` iterations across all
cores via OpenMP (when the host C compiler was built with `-fopenmp`), else
sequentially — no code change needed.

```tauraro
from std.gpu import Gpu

def square_at(i: int) -> void:
    output[i] = input[i] * input[i]      # output/input in scope (e.g. globals)

def main():
    print(f"openmp: {Gpu.has_openmp()}  threads: {Gpu.num_threads()}")
    unsafe:
        Gpu.parallel(n, square_at as Pointer[void])
```

| Method | Description |
|---|---|
| `Gpu.parallel(n, fn_ptr)` / `Gpu.parallel_for(n, fn_ptr)` | Run `[0, n)` in parallel; `fn_ptr` is a `Pointer[void]` to a `def f(i: int)` |
| `Gpu.has_openmp()` | True when OpenMP is available |
| `Gpu.thread_id()` / `Gpu.num_threads()` | Current thread index / thread count |

Enable OpenMP by building with `-fopenmp` (pass it through your C compiler
flags). Do **not** use `Gpu.parallel` for loops with loop-carried dependencies.

---

## Type-width rules

Tauraro `int` is **64-bit** and `float` is **64-bit** (C `double`). GPU code
commonly uses 32-bit types, so:

| You want on the GPU | Buffer element type | Scalar arg | Kernel param |
|---|---|---|---|
| 32-bit float | `Buffer[f32]` | `arg_f32` | `Pointer[f32]`, `f32` |
| 64-bit float | `Buffer[float]` / `Buffer[f64]` | `arg_f64` | `Pointer[f64]` |
| 32-bit int | `Buffer[i32]` | `arg_i32` | `Pointer[i32]`, `i32` |
| 64-bit int | `Buffer[int]` / `Buffer[i64]` | `arg_i64` | `Pointer[i64]` |

Mismatching widths (e.g. a `float*` kernel over a `Buffer[float]` which is
doubles) reads/writes the wrong bytes — the classic silent GPU bug. Match the
element type to the kernel parameter type.

CUDA doubles need `-arch` support; OpenCL doubles need the
`#pragma OPENCL EXTENSION cl_khr_fp64 : enable` (the `@kernel` backend and the
source path handle this for you where possible).

---

## Backend & driver notes

- **No SDK required to build.** The runtime dynamically loads the vendor driver
  at first use. Programs link and run on machines with no GPU (CPU fallback).
- **SPIR-V** loading uses `clCreateProgramWithIL`, which needs an **OpenCL 2.1+
  / SPIR-V-capable** driver. Older drivers can still use the OpenCL-C source
  path (`Module.load_source`).
- **PTX** runs on NVIDIA drivers via the CUDA driver API.
- Kernel launch requires a real GPU (CUDA or OpenCL). On the CPU backend,
  `Module.load_*` / launch report an error via `Device.last_error()`, while
  device buffers still work as host memory.

---

## API reference

Per-submodule imports:

```tauraro
from std.gpu.device   import Device, Backend
from std.gpu.buffer   import Buffer
from std.gpu.kernel   import Module, Kernel, Dim3
from std.gpu.parallel import Gpu
from std.gpu.kernel_builtins import gpu_global_id, gpu_local_id, gpu_group_id, \
                                    gpu_local_size, gpu_global_size, gpu_num_groups, gpu_barrier
```

See also:
[language chapter 18 — Parallelism & GPU](../lang/18_gpu_and_asm.md) ·
developer note [09 — GPU backend](../dev/09_gpu_backend.md).
