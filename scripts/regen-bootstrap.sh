#!/usr/bin/env bash
# Regenerate the portable C bootstrap (bootstrap/c/) from a working tauraroc.
# Run this whenever src/main.tr starts using a construct the committed-C
# bootstrap can no longer compile. Commit the updated bootstrap/c/ tree.
#
#   scripts/regen-bootstrap.sh [path-to-tauraroc]
set -euo pipefail

COMPILER="${1:-./tauraroc}"
if [ ! -x "$COMPILER" ]; then
    COMPILER="$(command -v tauraroc || true)"
fi
if [ -z "$COMPILER" ] || [ ! -x "$COMPILER" ]; then
    echo "ERROR: no working tauraroc found. Pass the path as arg 1, or put tauraroc on PATH."
    exit 1
fi

rm -rf build
"$COMPILER" src/main.tr --emit c
[ -f build/main.c ] || { echo "ERROR: emit failed: build/main.c missing"; exit 1; }

rm -rf bootstrap/c
mkdir -p bootstrap
cp -r build bootstrap/c
echo "==> Regenerated bootstrap/c/ ($(find bootstrap/c -type f | wc -l) files). Review + commit the tree."
