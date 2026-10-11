#!/usr/bin/env bash
# Runs the zlib-dependent tests against a runtime built WITH real zlib
# (-DTAURARO_COMPRESS_ZLIB -lz). The default test build uses the stub runtime, so
# the compression assertions in these tests only execute here.
# Skips (exit 0) when zlib's headers are not installed.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
TAURAROC="${TAURAROC:-$ROOT/tauraroc}"
[ -f /usr/include/zlib.h ] || [ -f /usr/local/include/zlib.h ] || { echo "zlib headers not found: skipping"; exit 0; }
WORK="$(mktemp -d)"; trap 'rm -rf "$WORK"' EXIT
fail=0
for f in tests/lang/46_gzip_tar.tr tests/lang/51_compress_zip.tr tests/lang/96_zlib_binary_len.tr; do
    n="$(basename "$f" .tr)"; d="$WORK/$n"; mkdir -p "$d"; (
      cd "$d" && "$TAURAROC" "$ROOT/$f" --emit c >/dev/null 2>&1 &&
      gcc -O1 -w -DTAURARO_COMPRESS_ZLIB -I build/include -o a.exe $(find build -name '*.c') -lm -lpthread -lz &&
      ./a.exe > out.txt 2>&1 && grep -q 'all [0-9]* tests passed' out.txt ) \
      && echo "PASS  $n (real zlib)" || { echo "FAIL  $n (real zlib)"; tail -5 "$d/out.txt" 2>/dev/null; fail=1; }
done
exit $fail
