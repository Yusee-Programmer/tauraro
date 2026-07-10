#!/usr/bin/env bash
# Differential oracle: native ≡ c. For every program in tests/native/*.tr, compile it BOTH
# ways — the C backend (--backend c -> gcc executable) and the native backend
# (--backend native -> x86-64/ELF object -> link with runtime.o) — run each, and assert the
# two produce byte-identical stdout. This proves the from-scratch machine-code backend
# matches the reference C backend on every feature the corpus exercises, and guards every
# future native slice against silent divergence. See project_native_backend.
#
# The native backend targets x86-64 ELF, so this is a Linux/x86-64 gate (same as
# native_run.sh); on other hosts the system linker is a different arch — skip gracefully.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"; [ -x "$TAURAROC" ] || TAURAROC="./tauraroc.exe"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
CC="${CC:-cc}"
command -v "$CC" >/dev/null 2>&1 || CC=gcc
command -v "$CC" >/dev/null 2>&1 || { echo "(no cc/gcc — skipping native≡c diff)"; exit 0; }

HOSTARCH="$(uname -m 2>/dev/null || echo unknown)"
case "$HOSTARCH" in
    x86_64|amd64) : ;;
    *) echo "(host arch is $HOSTARCH; the native backend targets x86-64 — skipping native≡c diff)"; exit 0 ;;
esac

echo "=============================================================="
echo "  Differential oracle: native ≡ c  (x86-64/ELF vs C backend)"
echo "=============================================================="
mkdir -p build
bash scripts/build_runtime_o.sh build/runtime.o || { echo "FAIL: runtime.o"; exit 1; }

pass=0; fail=0
for src in tests/native/*.tr; do
    [ -f "$src" ] || continue
    name="$(basename "$src" .tr)"

    # 1) C backend -> executable -> run.
    if ! "$TAURAROC" "$src" --backend c -o "build/c_$name" >/tmp/c_build.log 2>&1; then
        echo "  ✗ $name: C backend failed to build"; sed -n '1,8p' /tmp/c_build.log; fail=$((fail+1)); continue
    fi
    out_c="$("build/c_$name" 2>&1)"

    # 2) Native backend -> ELF object (must NOT fall back) -> link with runtime.o -> run.
    if ! "$TAURAROC" "$src" --backend native -o "build/$name.o" >/tmp/nat_build.log 2>&1; then
        echo "  ✗ $name: native backend fell back / errored (uses an unsupported feature?)"
        sed -n '1,8p' /tmp/nat_build.log; fail=$((fail+1)); continue
    fi
    if ! "$CC" "build/$name.o" build/runtime.o -lm -o "build/nat_$name" 2>/tmp/nat_ld.log; then
        echo "  ✗ $name: link failed"; sed -n '1,8p' /tmp/nat_ld.log; fail=$((fail+1)); continue
    fi
    out_nat="$("build/nat_$name" 2>&1)"

    # 3) Differential assertion.
    if [ "$out_c" = "$out_nat" ]; then
        echo "  ✓ $name: native ≡ c ($(printf '%s' "$out_c" | wc -l | tr -d ' ') lines match)"
        pass=$((pass+1))
    else
        echo "  ✗ $name: OUTPUT DIVERGES"
        echo "    --- C backend ---";      printf '%s\n' "$out_c"   | sed 's/^/      /'
        echo "    --- native backend ---"; printf '%s\n' "$out_nat" | sed 's/^/      /'
        fail=$((fail+1))
    fi
done

echo "--------------------------------------------------------------"
echo "  native≡c: $pass passed, $fail failed"
[ "$fail" -eq 0 ] || { echo "DIFFERENTIAL FAIL"; exit 1; }
echo "DIFFERENTIAL OK ✅ — the native x86-64/ELF backend matches the C backend on all $pass programs"
