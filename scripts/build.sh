#!/usr/bin/env bash
set -euo pipefail

# Bootstrap binary — set by CI via BOOTSTRAP_BIN, or fall back to tauraroc on PATH.
BOOTSTRAP="${BOOTSTRAP_BIN:-tauraroc}"

if [ ! -x "$BOOTSTRAP" ] && ! command -v "$BOOTSTRAP" &>/dev/null; then
    echo "ERROR: bootstrap binary not found: $BOOTSTRAP"
    echo "Set BOOTSTRAP_BIN to the path of a tauraroc binary, or put tauraroc on PATH."
    exit 1
fi

# Remove the ARM64 musl wrapper - use native glibc GCC instead
# (musl lacks ucontext functions needed by the coroutine scheduler)
STATIC_FLAG=""

# For Linux, ensure -std=gnu11 and -D_GNU_SOURCE for ucontext
if [ "$(uname -s)" = "Linux" ]; then
    # Create a GCC wrapper that always uses -std=gnu11 -D_GNU_SOURCE
    mkdir -p "$HOME/.local/bin"
    if [ ! -f "$HOME/.local/bin/gcc" ]; then
        printf '#!/bin/sh\nexec /usr/bin/gcc -std=gnu11 -D_GNU_SOURCE "$@"\n' > "$HOME/.local/bin/gcc"
        chmod +x "$HOME/.local/bin/gcc"
    fi
    export PATH="$HOME/.local/bin:$PATH"
    # Use native glibc GCC on all Linux platforms
    export CC="gcc"
    export CFLAGS="-O2 -std=gnu11 -D_GNU_SOURCE"
    export LDFLAGS="-lm -lpthread"
fi

echo "==> Compiling src/main.tr → ./tauraroc"
# The in-process LLVM shim provides _tr_llvm_emit_object so the `--backend llvm` drive links.
# DEFAULT: --link the plain .c (stub returning -1 → the compiler uses the bundled-llc
# subprocess, Stage 1). No libLLVM dependency.
LLVM_SHIM="runtime/tauraro_llvm.c"
# STAGE 2 opt-in: TAURARO_LLVM_INPROC=1 builds the shim against libLLVM (via llvm-config) so
# `--backend llvm` emits objects IN-PROCESS (no llc subprocess). Requires libLLVM dev files.
if [ -n "${TAURARO_LLVM_INPROC:-}" ]; then
    LC="${LLVM_CONFIG:-llvm-config}"
    echo "==> Stage 2: in-process libLLVM emission ($("$LC" --version))"
    "${CC:-gcc}" -DTAURARO_LLVM_INPROC -O2 -c runtime/tauraro_llvm.c \
        -I"$("$LC" --includedir)" -o build_tauraro_llvm.o
    LNAME="$("$LC" --libs | tr -d ' ' | sed 's/^-l//')"            # e.g. LLVM-21
    LDIR="$("$LC" --libdir)"
    LIMP="$LDIR/lib$LNAME.dll.a"                                   # Windows import lib
    [ -f "$LIMP" ] || LIMP="$LDIR/lib$LNAME.so"                    # Linux shared
    [ -f "$LIMP" ] || LIMP="$LDIR/lib$LNAME.dylib"                 # macOS shared
    LLVM_SHIM="build_tauraro_llvm.o --link $LIMP"
fi
"$BOOTSTRAP" src/main.tr -o tauraroc --link $LLVM_SHIM $STATIC_FLAG

# New bootstrap (v0.0.4+): binary lands in CWD as ./tauraroc
# Old bootstrap (≤v0.0.3): binary lands in src/build/tauraroc
# Normalise: move from old location to CWD if needed.
if [ ! -f "./tauraroc" ] && [ -f "./src/build/tauraroc" ]; then
    echo "==> Moving src/build/tauraroc → ./tauraroc (old bootstrap compat)"
    mv "./src/build/tauraroc" "./tauraroc"
fi

if [ ! -f "./tauraroc" ]; then
    echo "ERROR: tauraroc not produced — compilation failed"
    exit 1
fi

echo "==> Done: ./tauraroc"