# 09 — The GPU Kernel Backend (`src/codegen/gpu`)

How a `@kernel`-decorated Tauraro function becomes GPU code. This is **Route B**:
Tauraro → LLVM IR → `llc` → PTX (NVPTX/CUDA) or SPIR-V (OpenCL/Vulkan). It reuses
the same idea as the host LLVM backend (alloca-based IR, `mem2reg` for SSA) but
emits a GPU-flavoured module for a restricted function subset.

See also: user docs [`std.gpu`](../std/gpu.md) and
[language chapter 18](../lang/18_gpu_and_asm.md).

---

## Pipeline

```
@kernel def  ──sema──▶ HIR  ──GpuGenerator/GpuEmitter──▶ LLVM IR text
                                                           │  llc -mtriple=…
                                                           ▼
                                              .ptx (CUDA)  /  .spv (OpenCL)
                                                           │  Module.load_file / load_il
                                                           ▼
                                              cuModuleLoadData / clCreateProgramWithIL
```

- Driver flag: `tauraroc <file> --emit gpu --gpu-target {nvptx|spirv}` (in
  `src/main.tr`, dispatched right after sema produces the HIR).
- Toolchain: `llc` from the bundled/host LLVM. clang targets `nvptx64` and
  `spirv64`; the in-tree `spirv64` backend produces the `.spv` (no external
  `llvm-spirv` needed).

## Modules

| File | Role |
|---|---|
| `src/codegen/gpu/mod.tr` | `GpuGenerator` — driver entry. Emits the module header (triple/datalayout + builtin declarations), iterates `prog.functions`, emits each `@kernel`, and (NVPTX) appends the `!nvvm.annotations` kernel metadata. |
| `src/codegen/gpu/emit.tr` | `GpuEmitter` — walks one kernel's HIR and prints its `define`. |
| `std/gpu/kernel_builtins.tr` | The `gpu_*` index/sync builtins as ordinary functions with trivial host bodies. |

## Design decisions

**Alloca-based, not hand-SSA.** Like the host LLVM backend, every param/local
gets an `alloca` and reads/writes go through load/store. `llc`/`opt` `mem2reg`
promotes them to SSA registers, so the emitter never computes φ-nodes. Control
flow (`if`/`while`) uses freshly-numbered basic-block labels.

**Zero sema changes.** The GPU index builtins (`gpu_global_id`, …) are *real*
`std.gpu` functions with host implementations (return 0 / 1 / pass). So a
`@kernel def` is ordinary, type-checked Tauraro that also **runs serially on the
CPU** — a built-in differential reference. `@kernel` is just a decorator the
emitter recognises via `f.decorators`; sema treats the function normally.

**Subset, with hard errors.** The emitter handles the data-parallel leaf subset
(typed pointer params, scalars, arithmetic/compare, `if`/`while`, indexed
load/store, the builtins). Any other HIR node sets `ok=false` with a note; the
driver prints `[GPU] …` and exits non-zero — it never emits broken IR.

## The five host→GPU deltas

Relative to the host LLVM backend, the GPU emitter changes:

1. **Header** — a GPU `target triple` + datalayout (`nvptx64-nvidia-cuda` /
   `spirv64-unknown-unknown`) instead of target-neutral IR.
2. **Calling convention** — `ptx_kernel` / `spir_kernel` (+ `!nvvm.annotations`
   `{ptr @k, !"kernel", i32 1}` on NVPTX), and `void` return.
3. **Address spaces** — buffer pointer params are `ptr addrspace(1)` (global
   memory); loads/stores and `getelementptr` carry that address space.
4. **Precise scalar types** — kernels use exact widths (`f32`→`float`,
   `f64`→`double`, `i32`, `i64`), not the host backend's i64/double/ptr collapse.
   Binops unify operand types with `sext`/`trunc`/`fpext`/`fptrunc`/`sitofp`.
5. **Index builtins → intrinsics** — `gpu_global_id(d)` lowers to
   NVPTX `ctaid.d*ntid.d + tid.d` (via `@llvm.nvvm.read.ptx.sreg.*`) or the
   mangled OpenCL SPIR-V builtin `@_Z13get_global_idj` (`spir_func`). No runtime
   calls otherwise (the subset guarantees it).

Example emitted signature (SPIR-V):

```llvm
define spir_kernel void @saxpy(ptr addrspace(1) %arg_y, ptr addrspace(1) %arg_x,
                               float %arg_a, i32 %arg_n) { … }
```

## Runtime loading

`runtime/tauraro_rt.h` gained `clCreateProgramWithIL` (OpenCL 2.1+) plus
`_tr_gpu_module_load_il(il, len)` — SPIR-V for the OpenCL backend, PTX/cubin for
CUDA. Exposed in `std/gpu/kernel.tr` as `Module.load_il` / `load_spirv` /
`load_file` (binary-safe file read via `_tr_gpu_read_file`).

## Validation

- The generated `.ll` must pass `opt -passes=verify`.
- Differential oracle: run the `@kernel def` on the CPU (serial) and compare its
  output to the GPU run — the kernel source *is* the reference.
- Corpus: `tests/gpu/kernels.tr` (kernel definitions), `tests/gpu/saxpy_host.tr`
  (load + launch), `scripts/gpu_compile.sh` (the `--emit gpu | llc` step).

## Gotchas

- **`float` is 64-bit.** `Buffer[float]` is doubles; a `float*` kernel over it
  reads the wrong bytes. Match the buffer element type to the kernel param.
- **Per-TU statics.** Shared runtime GPU state lives behind `#ifdef _TR_MAIN`
  (single definition) / `extern` — a `static` global would give each emitted
  module its own GPU context. See the runtime `_tr_gpu` definition.
- **Driver conformance.** SPIR-V load needs an OpenCL 2.1+ driver; some older
  Intel Gen drivers crash `clBuildProgram` on any SPIR-V IL (a driver bug —
  clang's own OpenCL-C→SPIR-V crashes them too). PTX runs on NVIDIA; the
  OpenCL-C *source* path (`Module.load_source`) is the fallback for old drivers.

## Extending it

- **New scalar type** → `_gpu_scalar_ty` in `emit.tr`.
- **New builtin** → add a stub in `std/gpu/kernel_builtins.tr` and a case in
  `GpuEmitter.emit_gpu_builtin`.
- **New statement/expression** → a `case` in `emit_stmt` / `emit_expr_hir`
  (keep the "unsupported → `fail()`" default).
## Auto-embed (`--gpu-embed`)

`tauraroc app.tr --gpu-embed spirv|nvptx` bakes the compiled kernel module into
the binary: after sema, `main.tr` runs `GpuGenerator` → writes `_tr_gpu_kernels.ll`
→ shells `llc` → reads the bytes (`_tr_gpu_read_file`) → emits `_tr_gpu_blob.c`
(`gpu_blob_c`, a `static const unsigned char[]` + STRONG `_tr_gpu_embedded_blob`/
`_len` accessors) → pushes it into `all_c_files`. The runtime declares those
accessors and provides WEAK empty defaults under `_TR_MAIN`, which the strong blob
overrides (verified on mingw); so a program built WITHOUT `--gpu-embed` links fine
and `Module.embedded()` returns invalid. std: `Module.embedded()` loads the blob.

## Roadmap

- **`kernel_launch(fn, grid, block, args…)`** — type-directed launch. Needs the
  compiler to synthesize a launcher known to sema *before* it checks user code,
  then marshal each arg by the kernel's signature. Larger sema+codegen feature.
- `__local` shared memory with `gpu_barrier`; 2-D/3-D grids; `@device` helper
  functions.
