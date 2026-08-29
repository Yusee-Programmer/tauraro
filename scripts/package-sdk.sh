#!/usr/bin/env bash
# Package a SELF-CONTAINED Tauraro SDK zip for `taupkg install-tauraro`.
#
# Produces:  dist/tauraroc-<platform>.zip  with ONE top-level folder:
#
#   tauraroc-<platform>/          <- the only entry in the zip
#   ├── tauraroc[.exe]            <- the compiler binary
#   ├── runtime/                  <- runtime headers (tauraro_rt.h, ...)
#   ├── std/                      <- standard library (.tr sources)
#   ├── zig/                      <- bundled zig toolchain (for --target cross-compile)
#   └── examples/                 <- runnable sample programs
#
# `taupkg install-tauraro` extracts this into ~/.taupkg/bin/, so the SDK folder is
# self-contained: tauraroc finds runtime/, std/, and zig/ NEXT TO its own binary,
# and ~/.taupkg/bin/ stays clean (no src/docs/benchmarks junk).
#
# Ship ONLY what the SDK needs (plus examples) — not the whole repo.
#
# Usage:
#   scripts/package-sdk.sh <platform> <tauraroc-binary> <zig-dir>
# Examples:
#   scripts/package-sdk.sh windows-x64 ./tauraroc.exe ./zig
#   scripts/package-sdk.sh linux-x64   ./tauraroc     /opt/zig
set -euo pipefail

PLATFORM="${1:?usage: package-sdk.sh <platform> <tauraroc-binary> <zig-dir>}"
TC_BIN="${2:?path to the built tauraroc binary}"
ZIG_DIR="${3:?path to the bundled zig directory}"

STEM="tauraroc-$PLATFORM"
OUT="dist/$STEM"

# Windows binaries carry the .exe suffix; everything else is bare.
BIN_NAME="tauraroc"
case "$PLATFORM" in *windows*) BIN_NAME="tauraroc.exe" ;; esac

[ -f "$TC_BIN" ]  || { echo "error: tauraroc binary not found: $TC_BIN" >&2; exit 1; }
[ -d "$ZIG_DIR" ] || { echo "error: zig dir not found: $ZIG_DIR" >&2; exit 1; }
[ -d runtime ]    || { echo "error: run from the repo root (no runtime/)" >&2; exit 1; }
[ -d std ]        || { echo "error: run from the repo root (no std/)" >&2; exit 1; }
[ -d examples ]   || { echo "error: run from the repo root (no examples/)" >&2; exit 1; }

rm -rf "$OUT" "dist/$STEM.zip"
mkdir -p "$OUT"

cp    "$TC_BIN"  "$OUT/$BIN_NAME"
cp -r runtime    "$OUT/runtime"
cp -r std        "$OUT/std"
cp -r "$ZIG_DIR" "$OUT/zig"
cp -r examples   "$OUT/examples"

# Zip with the single top-level folder preserved. Prefer `zip`; fall back to
# PowerShell Compress-Archive on Windows if `zip` is unavailable.
if command -v zip >/dev/null 2>&1; then
    ( cd dist && zip -rq "$STEM.zip" "$STEM" )
else
    powershell -NoProfile -Command \
      "Compress-Archive -Force -Path 'dist/$STEM' -DestinationPath 'dist/$STEM.zip'"
fi

echo "built dist/$STEM.zip"
echo "top-level entries (should be exactly '$STEM/…'):"
if command -v unzip >/dev/null 2>&1; then
    unzip -l "dist/$STEM.zip" | awk 'NR>3 {print $4}' | cut -d/ -f1 | sort -u | head
fi
