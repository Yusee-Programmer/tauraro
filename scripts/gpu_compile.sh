#!/usr/bin/env bash
# gpu_compile.sh — compile the @kernel functions in a Tauraro file to a GPU module.
#
#   Route B pipeline:  tauraroc <file> --emit gpu  ->  LLVM IR  ->  llc  ->  .spv / .ptx
#
# Usage:
#   scripts/gpu_compile.sh <file.tr> <spirv|nvptx> <out> [tauraroc]
#
#   scripts/gpu_compile.sh tests/gpu/kernels.tr spirv kernels.spv
#   scripts/gpu_compile.sh tests/gpu/kernels.tr nvptx kernels.ptx
#   SM=sm_80 scripts/gpu_compile.sh kernels.tr nvptx kernels.ptx   # pick a CUDA arch
#
# The resulting module is loaded at runtime with std.gpu's Module.load_file(path)
# (SPIR-V for the OpenCL backend, PTX/cubin for the CUDA backend).
set -euo pipefail

FILE="${1:?usage: gpu_compile.sh <file.tr> <spirv|nvptx> <out> [tauraroc]}"
TARGET="${2:-spirv}"
OUT="${3:?output path required}"
TC="${4:-./tauraroc.exe}"

case "$TARGET" in
  nvptx|ptx|cuda)
    "$TC" "$FILE" --emit gpu --gpu-target nvptx \
      | llc -mtriple=nvptx64-nvidia-cuda -mcpu="${SM:-sm_50}" -o "$OUT"
    ;;
  spirv|spv|opencl|vulkan)
    "$TC" "$FILE" --emit gpu --gpu-target spirv \
      | llc -mtriple=spirv64-unknown-unknown -filetype=obj -o "$OUT"
    ;;
  *)
    echo "unknown target '$TARGET' (use spirv or nvptx)" >&2; exit 2 ;;
esac

echo "wrote $OUT ($(wc -c < "$OUT") bytes)"
