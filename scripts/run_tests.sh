#!/usr/bin/env bash
# Run the regression test suite under tests/lang/ and tests/regression/.
#
# Each test file uses std/test's TestRunner and is run with `tauraroc --run`.
# A test file passes iff:
#   - the compile+run exits 0, AND
#   - stdout does NOT contain the word "FAILED"
#
# Usage: scripts/run_tests.sh [path/to/single_test.tr]

set -uo pipefail

TAURAROC="${BOOTSTRAP_BIN:-./tauraroc}"
if [ ! -x "$TAURAROC" ] && [ -x "./tauraroc.exe" ]; then
    TAURAROC="./tauraroc.exe"
fi
if [ ! -x "$TAURAROC" ] && ! command -v "$TAURAROC" &>/dev/null; then
    echo "ERROR: tauraroc binary not found: $TAURAROC"
    echo "Set BOOTSTRAP_BIN, or build ./tauraroc / ./tauraroc.exe first."
    exit 1
fi

if [ "$#" -ge 1 ]; then
    FILES=("$@")
else
    FILES=()
    while IFS= read -r f; do FILES+=("$f"); done < <(find tests/lang tests/regression -name '*.tr' 2>/dev/null | sort)
fi

total=0
failed=0
failed_files=()

for f in "${FILES[@]}"; do
    total=$((total + 1))
    echo "==> $f"
    out=$("$TAURAROC" --run "$f" 2>&1)
    status=$?
    echo "$out"
    if [ $status -ne 0 ] || echo "$out" | grep -q "FAILED"; then
        failed=$((failed + 1))
        failed_files+=("$f")
    fi
done

# --- Formatter idempotency check ------------------------------------------
# `tauraroc fmt` must be idempotent: fmt(fmt(x)) == fmt(x). Verify on a couple
# of representative example files (skipped silently if examples are absent).
for FSAMPLE in examples/02_operators.tr examples/03_control_flow.tr; do
    [ -f "$FSAMPLE" ] || continue
    total=$((total + 1))
    echo "==> fmt idempotency: $FSAMPLE"
    f1=$("$TAURAROC" fmt "$FSAMPLE" 2>/dev/null)
    f1file=$(mktemp)
    printf '%s' "$f1" > "$f1file"
    f2=$("$TAURAROC" fmt "$f1file" 2>/dev/null)
    rm -f "$f1file"
    if [ "$f1" != "$f2" ]; then
        echo "  NOT IDEMPOTENT"
        failed=$((failed + 1))
        failed_files+=("fmt:$FSAMPLE")
    fi
done

# --- FFI / cdylib export check --------------------------------------------
# Build a shared library of `export def` functions and call them from a C
# program compiled against the generated header. Requires a C compiler (cc).
if command -v cc >/dev/null 2>&1 || command -v gcc >/dev/null 2>&1; then
    total=$((total + 1))
    echo "==> cdylib export"
    CCBIN=$(command -v cc || command -v gcc)
    libdir=$(mktemp -d)
    cat > "$libdir/lib.tr" <<'TREOF'
export def add(a: int, b: int) -> int:
    return a + b
export def multiply(a: int, b: int) -> int:
    return a * b
TREOF
    "$TAURAROC" "$libdir/lib.tr" -o "$libdir/lib" --lib >/dev/null 2>&1
    cat > "$libdir/consumer.c" <<'CEOF'
#include "lib.h"
#include <stdio.h>
int main(void){ printf("%lld %lld\n", add(3,4), multiply(5,6)); return 0; }
CEOF
    # The library extension is platform-dependent (.so / .dll / .dylib).
    libfile=""
    for cand in "$libdir/lib.so" "$libdir/lib.dll" "$libdir/lib.dylib"; do
        [ -f "$cand" ] && libfile="$cand" && break
    done
    cout=""
    if [ -n "$libfile" ]; then
        # Compile with -std=gnu11 on Linux for ucontext compatibility
        if [[ "$(uname -s)" == "Linux" ]]; then
            "$CCBIN" -std=gnu11 -D_GNU_SOURCE "$libdir/consumer.c" -I"$libdir" "$libfile" -o "$libdir/consumer" >/dev/null 2>&1
        else
            "$CCBIN" "$libdir/consumer.c" -I"$libdir" "$libfile" -o "$libdir/consumer" >/dev/null 2>&1
        fi
        # Run from the lib dir with it on the dynamic-loader search path so the
        # shared object is found at runtime (ELF: LD_LIBRARY_PATH, Mach-O:
        # DYLD_LIBRARY_PATH; Windows resolves the .dll from the cwd). Without
        # this the consumer fails to start on Linux/macOS -> empty output.
        _TMO=""; command -v timeout >/dev/null 2>&1 && _TMO="timeout -k 5 30"   # bound run vs CI hangs
        if [ -f "$libdir/consumer" ]; then
            case "$(uname -s)" in
                Linux)
                    cout=$(cd "$libdir" && LD_LIBRARY_PATH="$libdir:${LD_LIBRARY_PATH:-}" $_TMO ./consumer 2>/dev/null)
                    ;;
                Darwin)
                    cout=$(cd "$libdir" && DYLD_LIBRARY_PATH="$libdir:${DYLD_LIBRARY_PATH:-}" $_TMO ./consumer 2>/dev/null)
                    ;;
                MINGW*|MSYS*|CYGWIN*)
                    cout=$(cd "$libdir" && $_TMO ./consumer.exe 2>/dev/null)
                    ;;
                *)
                    cout=$(cd "$libdir" && LD_LIBRARY_PATH="$libdir:${LD_LIBRARY_PATH:-}" $_TMO ./consumer 2>/dev/null)
                    ;;
            esac
        fi
        [ -z "$cout" ] && [ -f "$libdir/consumer.exe" ] && cout=$(cd "$libdir" && $_TMO ./consumer.exe 2>/dev/null)
    fi
    if [ "$cout" != "7 30" ]; then
        echo "  FAILED (got: '$cout')"
        failed=$((failed + 1))
        failed_files+=("cdylib_export")
    fi
    rm -rf "$libdir"
fi

# --- diagnostics that must stop compilation --------------------------------
total=$((total + 1))
echo "==> compile-time diagnostics"
diagdir=$(mktemp -d)
diagfail=""
printf 'def main():\n    mut a = 1\n    mut a = 2\n    print(a)\n' > "$diagdir/redecl.tr"
dout=$("$TAURAROC" "$diagdir/redecl.tr" -o "$diagdir/redecl" 2>&1)
case "$dout" in *"[N-2] 'a' is already declared in this block"*) ;; *) diagfail="$diagfail [N-2 redeclaration]";; esac
printf 'def main():\n    mut s = "x"\n    print(s.nope("y"))\n' > "$diagdir/strm.tr"
dout=$("$TAURAROC" "$diagdir/strm.tr" -o "$diagdir/strm" 2>&1)
case "$dout" in *"No method 'nope' found on type 'str'"*) ;; *) diagfail="$diagfail [unknown str method]";; esac
printf 'def main():\n    mut a = 1\n    if a > 0:\n        mut a = 2\n        print(a)\n    print(a)\n' > "$diagdir/shadow.tr"
dout=$("$TAURAROC" "$diagdir/shadow.tr" -o "$diagdir/shadow" 2>&1)
case "$dout" in *"[N-2]"*) diagfail="$diagfail [inner-block shadowing rejected]";; esac
printf 'def f() throws str -> int:\n    return 1\ndef main():\n    mut x = f()?\n    print(x)\n' > "$diagdir/qmain.tr"
dout=$("$TAURAROC" "$diagdir/qmain.tr" -o "$diagdir/qmain" 2>&1)
case "$dout" in *"[F-4] '?' used in 'main'"*) ;; *) diagfail="$diagfail [? in non-throwing fn]";; esac
printf 'enum C:\n    R\n    G\n    B\ndef main():\n    mut x = match C.R:\n        case C.R: 1\n        case C.G: 2\n    print(x)\n' > "$diagdir/mexpr.tr"
dout=$("$TAURAROC" "$diagdir/mexpr.tr" -o "$diagdir/mexpr" 2>&1)
case "$dout" in *"Non-exhaustive match expression on 'C': missing variant: B"*) ;; *) diagfail="$diagfail [non-exhaustive match expression]";; esac
printf 'enum C:\n    R\n    G\ndef f(c: C) -> int:\n    match c:\n        case C.R if false: return 1\n        case C.G: return 2\n    return 0\ndef main():\n    print(f(C.R))\n' > "$diagdir/mguard.tr"
dout=$("$TAURAROC" "$diagdir/mguard.tr" -o "$diagdir/mguard" 2>&1)
case "$dout" in *"Non-exhaustive match on 'C': missing variant: R"*) ;; *) diagfail="$diagfail [guarded arm counted as covering]";; esac
printf 'class _Complex:\n    pub v: int\ndef main():\n    mut c = _Complex()\n    print(c.v)\n' > "$diagdir/ckw.tr"
dout=$("$TAURAROC" "$diagdir/ckw.tr" -o "$diagdir/ckw" 2>&1)
case "$dout" in *"C99/C11 keyword"*) ;; *) diagfail="$diagfail [C keyword class name]";; esac
printf 'def main():\n    mut pass = 1\n    print(pass)\n' > "$diagdir/kwname.tr"
dout=$("$TAURAROC" "$diagdir/kwname.tr" -o "$diagdir/kwname" 2>&1)
case "$dout" in *"unexpected keyword 'pass'"*) ;; *) diagfail="$diagfail [keyword used as a name]";; esac
# expect_diag <label> <program text> <substring of the compiler output>
expect_diag() {
    printf '%b' "$2" > "$diagdir/ed.tr"
    dout=$("$TAURAROC" "$diagdir/ed.tr" -o "$diagdir/ed" 2>&1)
    case "$dout" in
        *"[ICE]"*|*"[C-1]"*) diagfail="$diagfail [$1: reached the C compiler]";;
        *"$3"*) ;;
        *) diagfail="$diagfail [$1: expected '$3']";;
    esac
}
CLS='class Th:\n    pub v: int\nextend Th:\n    pub def mk(a: int) -> Th:\n        mut t = Th()\n        t.v = a\n        return t\n    pub def val(self) -> int:\n        return self.v\n'
expect_diag "str to Pointer" 'def main():\n    mut p: Pointer[int] = "s"\n' "[E-5] Type mismatch"
expect_diag "list get str index" 'def main():\n    mut v: List[int] = [1]\n    print(v.get("a"))\n' "must be an int"
expect_diag "vec slice str" 'def main():\n    mut v: Vec[int] = Vec[int].init(2)\n    mut w = v.slice("a", 1)\n' "must be an int"
expect_diag "fn arity" 'def f(x: int, y: str) -> int:\n    return x\ndef main():\n    f(1)\n' "[E-4] 'f()' takes 2 arguments but 1 was given"
expect_diag "fn arg type" 'def f(x: int) -> int:\n    return x\ndef main():\n    f("a")\n' "[E-5] Type mismatch: argument 1 of 'f()'"
expect_diag "method arity" "${CLS}"'def main():\n    mut t = Th()\n    t.val(5)\n' "[E-4] 'Th.val()' takes 0 arguments"
expect_diag "static arity" "${CLS}"'def main():\n    mut t = Th.mk(1, 2)\n' "[E-4] 'Th.mk()' takes 1 argument"
expect_diag "static arg type" "${CLS}"'def main():\n    mut t = Th.mk("s")\n' "[E-5] Type mismatch: argument 1 of 'Th.mk()'"
expect_diag "return type" 'def f() -> int:\n    return "s"\ndef main():\n    f()\n' "[E-5] Type mismatch: 'f' is declared to return an int"
expect_diag "let annotation" 'def main():\n    mut x: int = "s"\n' "[E-5] Type mismatch: the initializer of 'x'"
expect_diag "assign" 'def main():\n    mut x = 1\n    x = "s"\n' "[E-5] Type mismatch: 'x' holds an int"
expect_diag "block scope" 'def main():\n    n = 3\n    if n == 3:\n        ok = false\n    print(ok)\n' "[N-3] name 'ok' is not defined"
expect_diag "while scope" 'def main():\n    mut i = 0\n    while i < 2:\n        i = i + 1\n        seen = true\n    print(seen)\n' "[N-3] name 'seen' is not defined"
expect_diag "vec ctor type" 'class Ob:\n    pub n: int\ndef main():\n    mut v: Vec[Ob] = Vec[int].init(4)\n' "[E-5] Type mismatch: the initializer of 'v' is a Vec[Ob]"
expect_diag "index assign type" 'def f(xs: List[str]):\n    xs[0] = 5\ndef main():\n    f(["a"])\n' "[E-5] Type mismatch: an element of this List[str]"
expect_diag "closure return" 'def main():\n    mut g = def () -> List[int]:\n        return 5\n    g()\n' "[E-5] Type mismatch: this closure is declared to return a List"
expect_diag "generic init arity" 'class Bx[K, V]:\n    pub k: K\n    pub v: V\nextend Bx[K, V]:\n    pub def init(k: K, v: V) -> Bx[K, V]:\n        mut b = Bx[K, V]()\n        b.k = k\n        b.v = v\n        return b\ndef main():\n    mut b = Bx[str, int].init(1)\n' "[E-4] 'Bx.init()' takes 2 arguments"
expect_diag "builtin-named static" 'class Q:\n    _d: int\nextend Q:\n    pub def contains(s: str, sub: str) -> bool:\n        return true\ndef main():\n    print(Q.contains("a"))\n' "[E-4] 'Q.contains()' takes 2 arguments"
expect_diag "extern in" 'extern "C":\n    def sqrt(x: float) -> float\ndef main():\n    print(2.5 in sqrt(4.0))\n' "[E-5] Type mismatch: the right-hand side of 'in'"
expect_diag "str + int" 'def main():\n    mut x = "a" + 1\n' "[E-5] Type mismatch: cannot apply '+'"
expect_diag "unary minus str" 'def main():\n    mut x = -"a"\n' "[E-5] Type mismatch: unary '-'"
expect_diag "field of int" 'def main():\n    mut x = 5\n    print(x.foo)\n' "[E-6] Cannot read '.foo'"
expect_diag "unknown field" "${CLS}"'def main():\n    mut t = Th()\n    print(t.nope)\n' "[E-7] Type 'Th' has no field 'nope'"
expect_diag "field type" "${CLS}"'def main():\n    mut t = Th()\n    t.v = "s"\n' "[E-5] Type mismatch: the field 'v' of 'Th'"
expect_diag "unknown type" 'def f(x: Nope) -> int:\n    return 1\ndef main():\n    f(1)\n' "[N-4] Unknown type 'Nope'"
expect_diag "undefined var" 'def main():\n    print(zz + 1)\n' "[N-3] name 'zz' is not defined"
expect_diag "undefined receiver" 'def main():\n    zz.go()\n' "[N-3] name 'zz' is not defined"
expect_diag "undefined subscript" 'def main():\n    mut xs = [1, 2]\n    xs[kk] = 3\n' "[N-3] name 'kk' is not defined"
expect_diag "str method arity" 'def main():\n    mut s = "abc"\n    print(s.find("a", "b", "c"))\n' "[E-4] 'str.find()' takes 1 argument"
expect_diag "trailing tokens" 'def main():\n    mut b = 5 7\n' "unexpected token after the end of the statement"
expect_diag "in non-collection" 'def main():\n    mut b = 1 in 5\n' "[E-5] Type mismatch: the right-hand side of 'in'"
expect_diag "generic cannot infer" 'def conv[T](x: int) -> T:\n    return x as T\ndef main():\n    mut a = conv(3)\n' "[G-2] Cannot infer type parameter 'T'"
expect_diag "enum variant" 'enum C:\n    R\n    G\ndef main():\n    mut c = C.Gren\n' "[E-7] Enum 'C' has no variant or method 'Gren'"
expect_diag "option arithmetic" 'def f(x: Option[int]) -> int:\n    return x + 1\ndef main():\n    f(None)\n' "[E-5] Type mismatch: '+' cannot be applied to an Option value"
expect_diag "list property" 'def main():\n    mut xs = [1, 2]\n    print(xs.size)\n' "[E-7] A List has no field 'size'"
expect_diag "list element type" 'def main():\n    mut xs: List[str] = [1, 2]\n' "[E-5] Type mismatch: the initializer of 'xs' is a List[str]"
expect_diag "collection arg kind" 'def main():\n    mut xs: List[str] = []\n    xs.append(5)\n' "[E-5] Type mismatch: List[str].append() is given"
expect_diag "dict key kind" 'def main():\n    mut d: Dict[str, int] = {}\n    d.set(1, 2)\n' "[E-5] Type mismatch: the key of Dict[str, int].set()"
expect_diag "mixed list literal" 'def main():\n    mut xs = [1, "a"]\n' "[E-5] Type mismatch: a list literal mixes"
expect_diag "string minus" 'def main():\n    mut s = "a" - "b"\n' "[E-5] Type mismatch: '-' is not defined for strings"
expect_diag "None arithmetic" 'def main():\n    mut x = None\n    mut y = x + 1\n' "[E-5] Type mismatch: '+' cannot be applied to None"
expect_diag "atomic arity" 'def main():\n    mut a: Atomic[int] = Atomic.new(0)\n    a.add()\n' "[E-4] 'Atomic.add()' takes 1 argument"
expect_diag "spawn arity" 'def w(a: int, b: int) -> void:\n    pass\ndef main():\n    mut t = Thread.spawn(w, 1)\n' "[E-4] 'Thread.spawn(w, ...)'"
expect_diag "closure var arity" 'def main():\n    mut c = lambda x: x + 1\n    print(c(1, 2))\n' "[E-4] the function value 'c' takes 1 argument"
expect_diag "closure var arg kind" 'def main():\n    mut f = def(x: int) -> int:\n        return x\n    print(f("a"))\n' "[E-5] Type mismatch: argument 1 of 'f()'"
expect_diag "ctor call arity" 'class A:\n    pub v: int\nextend A:\n    pub def init(v: int) -> A:\n        mut a = A()\n        a.v = v\n        return a\n    pub def get(self) -> int:\n        return self.v\ndef main():\n    mut a = A(1, 2)\n' "[E-4] 'A.init()' takes 1 argument but 2 were given"
expect_diag "ctor call arg kind" 'class A:\n    pub v: int\nextend A:\n    pub def init(v: int) -> A:\n        mut a = A()\n        a.v = v\n        return a\n    pub def get(self) -> int:\n        return self.v\ndef main():\n    mut a = A("s")\n' "[E-5] Type mismatch: argument 1 of 'A.init()'"
expect_diag "UFCS non-instance" 'class A:\n    pub v: int\nextend A:\n    pub def init(v: int) -> A:\n        mut a = A()\n        a.v = v\n        return a\n    pub def get(self) -> int:\n        return self.v\ndef main():\n    print(A.get(5))\n' "called through the type needs a 'A' instance"
expect_diag "generic type arg count" 'class Box[T]:\n    pub v: T\nextend Box:\n    pub def init(v: T) -> Box[T]:\n        mut b = Box[T]()\n        b.v = v\n        return b\ndef main():\n    mut b = Box[str,int].init("a")\n' "[E-4] 'Box' takes 1 type argument(s) but 2 given"
if [ -n "$diagfail" ]; then
    echo "  FAILED:$diagfail"
    failed=$((failed + 1))
    failed_files+=("diagnostics")
fi
rm -rf "$diagdir"

# --- `tauraroc doc` -------------------------------------------------------
total=$((total + 1))
echo "==> doc generator"
docdir=$(mktemp -d)
cat > "$docdir/shapes.tr" <<'TREOF'
# shapes -- geometry helpers
#
# Small module used to test the doc generator.

pub const SIDES = 4   # sides of a square

# A point in the plane.
pub class Point:
    pub x: float   # horizontal
    pub y: float
    _hidden: int

extend Point:
    # Distance from the origin.
    pub def norm(self) -> float:
        return self.x * self.x + self.y * self.y
    pub def origin() -> Point: return Point()

# Area of a w x h rectangle.
pub def area(w: float,
             h: float) -> float:
    return w * h

def private_helper() -> int: return 1
TREOF
dout=$("$TAURAROC" doc "$docdir/shapes.tr" 2>&1)
"$TAURAROC" doc "$docdir/shapes.tr" -o "$docdir/html" --html >/dev/null 2>&1
dfail=""
for want in "shapes -- geometry helpers" "SIDES = 4" "sides of a square" "def area(w: float, h: float) -> float" \
            "Area of a w x h rectangle." "class \`Point\`" "A point in the plane." "x: float" "horizontal" \
            "def norm(self) -> float" "Distance from the origin." "def origin() -> Point"; do
    case "$dout" in *"$want"*) ;; *) dfail="$dfail [$want]";; esac
done
case "$dout" in *private_helper*|*_hidden*) dfail="$dfail [private item listed]";; esac
[ -f "$docdir/html/index.html" ] || dfail="$dfail [no index.html]"
ls "$docdir/html/" 2>/dev/null | grep -q "shapes.html" || dfail="$dfail [no module page]"
if [ -n "$dfail" ]; then
    echo "  FAILED (missing:$dfail)"
    failed=$((failed + 1))
    failed_files+=("doc_generator")
fi
rm -rf "$docdir"

# --- `tauraroc cov` (Linux + gcov) -----------------------------------------
if [[ "$(uname -s)" == "Linux" ]] && command -v gcov >/dev/null 2>&1; then
    total=$((total + 1))
    echo "==> coverage"
    covdir=$(mktemp -d)
    cat > "$covdir/prog.tr" <<'TREOF'
def classify(n: int) -> str:
    if n < 0:
        return "negative"
    return "non-negative"

def never_called() -> int:
    return 42

def main():
    print(classify(5))
TREOF
    cout=$("$TAURAROC" cov "$covdir/prog.tr" --lcov "$covdir/out.info" 2>&1)
    cfail=""
    case "$cout" in *"non-negative"*) ;; *) cfail="$cfail [program output]";; esac
    case "$cout" in *"prog.tr"*"3, 7"*) ;; *) cfail="$cfail [missing lines 3, 7]";; esac
    grep -q "^DA:2,1$" "$covdir/out.info" 2>/dev/null || cfail="$cfail [DA:2,1]"
    grep -q "^DA:3,0$" "$covdir/out.info" 2>/dev/null || cfail="$cfail [DA:3,0]"
    grep -q "^DA:7,0$" "$covdir/out.info" 2>/dev/null || cfail="$cfail [DA:7,0]"
    if [ -n "$cfail" ]; then
        echo "  FAILED:$cfail"
        echo "$cout" | tail -5
        failed=$((failed + 1))
        failed_files+=("coverage")
    fi
    rm -rf "$covdir"
fi

echo ""
echo "==================================="
echo "Test files: $total, failed: $failed"
if [ $failed -gt 0 ]; then
    echo "Failed files:"
    for f in "${failed_files[@]}"; do
        echo "  - $f"
    done
    exit 1
fi
echo "All test files passed."