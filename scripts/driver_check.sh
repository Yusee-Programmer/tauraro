#!/usr/bin/env bash
# Driver-level regression checks that a .tr test file cannot express (they need to
# run the compiler binary itself from odd places / look at its exit status):
#   #22  a compiler binary living inside the checkout but invoked from an unrelated
#        CWD must still find runtime/tauraro_rt.h (it used to look only relative to
#        the CWD / the binary's own directory).
#   #25  `tauraroc --run` must propagate the program's real exit code (the raw
#        system() wait status used to wrap to 0 in exit()).
#   #23  `int << n` must be emitted on the unsigned representation (plain signed
#        shifts are UB once the result leaves the long long range).
# Usage: scripts/driver_check.sh   (uses ./tauraroc, or $TAURAROC)
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-./tauraroc}"
case "$TAURAROC" in /*) : ;; *) TAURAROC="$ROOT/${TAURAROC#./}" ;; esac
[ -x "$TAURAROC" ] || { echo "ERROR: $TAURAROC not found"; exit 1; }

FAILS=0
ok()   { echo "PASS  $1"; }
bad()  { echo "FAIL  $1"; FAILS=$((FAILS+1)); }

WORK="$(mktemp -d "$ROOT/scratch_driver_XXXXXX")"; trap 'rm -rf "$WORK"' EXIT
OUT="$(mktemp -d)"; trap 'rm -rf "$WORK" "$OUT"' EXIT

cat > "$OUT/exit3.tr" <<'TR'
extern "C":
    def _tr_exit(c: int)
def main():
    print("hello")
    _tr_exit(3)
TR

# #22: compiler copy one level below the repo root, CWD elsewhere.
cp "$TAURAROC" "$WORK/stage0"
( cd "$OUT" && "$WORK/stage0" exit3.tr -o exit3 >"$OUT/c22.log" 2>&1 )
if [ -x "$OUT/exit3" ] && [ "$("$OUT/exit3")" = "hello" ]; then ok "#22 runtime header found from a non-root CWD"
else bad "#22 runtime header found from a non-root CWD"; grep -m3 -i error "$OUT/c22.log"; fi

# #25: --run exit status.
( cd "$OUT" && "$TAURAROC" --run exit3.tr >/dev/null 2>&1 ); rc=$?
if [ "$rc" = "3" ]; then ok "#25 --run propagates the exit code (3)"; else bad "#25 --run propagates the exit code (got $rc, want 3)"; fi

# #23: emitted C for `<<` on int.
cat > "$OUT/shl.tr" <<'TR'
def shl(x: int, n: int) -> int:
    return x << n
def main():
    print(shl(255, 56))
TR
( cd "$OUT" && "$TAURAROC" --emit c shl.tr >/dev/null 2>&1 )
CF="$(ls "$OUT"/build/*.c 2>/dev/null | head -1)"
if [ -n "$CF" ] && grep -q 'unsigned long long)(.*<<' "$CF"; then ok "#23 int << emitted via unsigned"
else bad "#23 int << emitted via unsigned"; fi

[ "$FAILS" = "0" ] && { echo "driver checks: all clean"; exit 0; }
echo "driver checks: $FAILS failed"; exit 1
