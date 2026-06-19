#!/usr/bin/env bash
set -euo pipefail

# Bootstrap binary — set by CI via BOOTSTRAP_BIN, or fall back to tauraroc on PATH.
BOOTSTRAP="${BOOTSTRAP_BIN:-tauraroc}"

if [ ! -x "$BOOTSTRAP" ] && ! command -v "$BOOTSTRAP" &>/dev/null; then
    echo "ERROR: bootstrap binary not found: $BOOTSTRAP"
    echo "Set BOOTSTRAP_BIN to the path of a tauraroc binary, or put tauraroc on PATH."
    exit 1
fi

# ARM64 Linux: wrap gcc with musl-gcc for Android/Termux portability.
STATIC_FLAG=""
if [ "$(uname -m)" = "aarch64" ] && [ "$(uname -s)" = "Linux" ]; then
    echo "==> ARM64 Linux: installing musl-tools for portable binary"
    sudo apt-get install -y --no-install-recommends musl-tools
    mkdir -p "$HOME/.local/bin"
    printf '#!/bin/sh\nexec musl-gcc -std=gnu11 -D_GNU_SOURCE -D_XOPEN_SOURCE=700 "$@"\n' > "$HOME/.local/bin/gcc"
    chmod +x "$HOME/.local/bin/gcc"
    export PATH="$HOME/.local/bin:$PATH"
    STATIC_FLAG="--static"
fi

# For non-ARM64 Linux, ensure -std=gnu11 and -D_GNU_SOURCE for ucontext
if [ "$(uname -s)" = "Linux" ] && [ "$(uname -m)" != "aarch64" ]; then
    mkdir -p "$HOME/.local/bin"
    if [ ! -f "$HOME/.local/bin/gcc" ]; then
        printf '#!/bin/sh\nexec /usr/bin/gcc -std=gnu11 -D_GNU_SOURCE -D_XOPEN_SOURCE=700 "$@"\n' > "$HOME/.local/bin/gcc"
        chmod +x "$HOME/.local/bin/gcc"
    fi
    export PATH="$HOME/.local/bin:$PATH"
fi

# Also override CC in the environment for tauraroc's compilation
export CC="gcc"
export CFLAGS="-O2 -std=gnu11 -D_GNU_SOURCE -D_XOPEN_SOURCE=700"
export LDFLAGS="-lm -lpthread"

echo "==> Compiling src/main.tr → ./tauraroc"
"$BOOTSTRAP" src/main.tr -o tauraroc $STATIC_FLAG

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