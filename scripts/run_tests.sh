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
expect_diag "param needs a type" 'def add(a, b):\n    return a + b\ndef main():\n    print(add(1, 2))\n' "[R-1] parameter 'a' of 'add' needs a type"
expect_diag "return type not inferable" 'def f(n: int):\n    if n > 0:\n        return g(n - 1)\n    return 1\ndef g(n: int):\n    return f(n)\ndef main():\n    print(f(3))\n' "[R-2] cannot infer the return type"
expect_diag "return types differ" 'def f(n: int):\n    if n > 0:\n        return 1\n    return "no"\ndef main():\n    print(f(3))\n' "[R-2] the returns of 'f' have different types"
expect_diag "empty list never typed" 'def main():\n    xs = []\n    print(xs[0])\n' "[R-3] cannot infer the element type of 'xs'"
expect_diag "empty dict never typed" 'def main():\n    d = {}\n    print(d.get("a"))\n' "[R-3] cannot infer the element type of 'd'"
expect_diag "lambda never called" 'def main():\n    f = lambda x: x + 1\n    print(1)\n' "[R-6] cannot infer the parameter types of the lambda 'f'"
expect_diag "lambda without context" 'def main():\n    fs = [lambda x: x + 1]\n    print(len(fs))\n' "[R-6] cannot infer the type of the lambda parameter 'x'"
expect_diag "unpack a non-tuple" 'def main():\n    a, b = 5\n' "[R-4] cannot unpack int"
expect_diag "inferred type seen by the checker" 'def sq(x: int):\n    return x * x\ndef main():\n    mut r = sq(2)\n    s: str = r\n' "[E-5]"
CLS='class Th:\n    pub v: int\nextend Th:\n    pub def mk(a: int) -> Th:\n        mut t = Th()\n        t.v = a\n        return t\n    pub def val(self) -> int:\n        return self.v\n'
expect_diag "str to Pointer" 'def main():\n    mut p: Pointer[int] = "s"\n' "[E-5] Type mismatch"
expect_diag "list get str index" 'def main():\n    mut v: List[int] = [1]\n    print(v.get("a"))\n' "must be an int"
expect_diag "vec slice str" 'def main():\n    mut v: Vec[int] = Vec[int].init(2)\n    mut w = v.slice("a", 1)\n' "must be an int"
expect_diag "if str cond" 'def main():\n    mut s = "a"\n    if s:\n        print(1)\n' "'if' condition must be a bool"
expect_diag "while option cond" 'def main():\n    mut o: Option[int] = Some(1)\n    while o:\n        break\n' "'while' condition must be a bool"
expect_diag "and str" 'def main():\n    mut s = "a"\n    mut z = s and true\n' "operand of 'and' must be a bool"
expect_diag "not list" 'def main():\n    mut l: List[int] = [1]\n    mut z = not l\n' "'not' cannot be applied"
expect_diag "assert tuple" 'def main():\n    mut t = (1, 2)\n    assert t\n' "assert condition must be a bool"
expect_diag "ternary cond" 'def main():\n    mut s = "a"\n    mut z = 1 if s else 2\n' "conditional expression must be a bool"
expect_diag "ternary branches" 'def main():\n    mut x: int = 1 if true else "a"\n' "two branches of this conditional"
expect_diag "shift float" 'def main():\n    mut x = 1.5 << 2\n' "needs integer operands"
expect_diag "bitnot str" 'def main():\n    mut s = "a"\n    mut x = ~s\n' "'~' needs an integer operand"
expect_diag "range str" 'def main():\n    for i in range("a"):\n        print(i)\n' "of range() must be an int"
expect_diag "for over int" 'def main():\n    mut n = 5\n    for i in n:\n        print(i)\n' "cannot loop over an int"
expect_diag "comp over int" 'def main():\n    mut l = [x for x in 5]\n' "cannot loop over an int"
expect_diag "return value void" 'def f() -> void:\n    return 1\ndef main():\n    f()\n' "has no return type but this returns"
expect_diag "return no value" 'def f() -> int:\n    return\ndef main():\n    f()\n' "this 'return' has no value"
expect_diag "let void call" 'def f():\n    print(1)\ndef main():\n    mut x = f()\n' "returns no value"
expect_diag "print void call" 'def f():\n    print(1)\ndef main():\n    print(f())\n' "returns no value"
expect_diag "len of int" 'def main():\n    print(len(5))\n' "len() needs a string"
expect_diag "option from int" 'def main():\n    mut o: Option[int] = 5\n' "expects 'Option'"
expect_diag "str index assign" 'def main():\n    mut s = "abc"\n    s[0] = "x"\n' "str cannot be modified through an index"
expect_diag "dict literal key" 'def main():\n    mut d: Dict[str, int] = {1: 2}\n' "but its key 1 is"
expect_diag "dict literal value" 'def main():\n    mut d: Dict[str, int] = {"a": "b"}\n' "but its value 1 is"
expect_diag "set literal elem" 'def main():\n    mut d: Set[int] = {"a"}\n' "but its element 1 is"
expect_diag "tuple literal arity" 'def main():\n    mut t: Tuple[int, str] = (1, "a", 3)\n' "Tuple of 2 elements"
expect_diag "tuple literal elem" 'def main():\n    mut t: Tuple[int, str] = (1, 2)\n' "element 2 of"
expect_diag "match literal kind" 'def main():\n    mut x = 5\n    match x:\n        case "a": print(1)\n        case _: print(2)\n' "pattern is a str literal"
expect_diag "char of two" 'def main():\n    mut c: char = "ab"\n' "is a char but is given a string of 2"
expect_diag "cast tuple to int" 'def main():\n    mut t = (1, 2)\n    mut n = t as int\n    print(n)\n' "cannot be cast to int"
expect_diag "cast list to float" 'def main():\n    mut l: List[int] = [1]\n    mut n = l as float\n    print(n)\n' "cannot be cast to float"
expect_diag "not set" 'def main():\n    mut s: Set[int] = {1}\n    mut z = not s\n' "cannot be applied to a Set"
expect_diag "list literal f32" 'def main():\n    mut l: List[int] = [1.5 as f32]\n' "this list literal holds f32"
expect_diag "list literal u16" 'def main():\n    mut l: List[u16] = [1, 2]\n' "this list literal holds int"
expect_diag "pointer write str" 'def main():\n    mut p: Pointer[int] = 0 as Pointer[int]\n    unsafe:\n        p.write("s")\n' "write() is given a str"
expect_diag "atomic new str" 'def main():\n    mut a = Atomic.new("str")\n' "Atomic needs a number"
expect_diag "str plus list" 'def main():\n    mut a = "x"\n    mut b = [1, 2]\n    mut c = a + b\n' "cannot apply '+' to a str and a List"
expect_diag "tuple str index" 'def main():\n    mut t = (1, 2)\n    print(t["a"])\n' "a tuple is indexed with a str"
expect_diag "sizeof unknown" 'def main():\n    print(sizeof(Unknwn))\n' "Unknown type 'Unknwn' (in this sizeof())"
expect_diag "closure ret unknown type" 'def main():\n    mut f = def () -> Foo:\n        return 1\n' "Foo"
expect_diag "global list as int" 'mut g = [1, 2]\ndef f(x: int) -> int:\n    return x\ndef main():\n    print(f(g))\n' "argument 1 of 'f()'"
expect_diag "unpack tuple kinds" 'def two() -> Tuple[str, int]:\n    return ("a", 1)\ndef main():\n    mut a, b = two()\n    if a == 42:\n        print(1)\n' "cannot apply '==' to a str and an int"
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
expect_diag "result unknown method" 'def main():\n    mut r = Result.ok(3)\n    print(r.unwrap_zz())\n' "[E-6] a Result has no method 'unwrap_zz()'"
expect_diag "option unknown method" 'def main():\n    mut o = Option.some(3)\n    print(o.unwrap_zz())\n' "[E-6] an Option has no method 'unwrap_zz()'"
expect_diag "field assign on None" 'class P:\n    pub x: int\ndef main():\n    mut p = None\n    p.x = 1\n' "[E-7] cannot assign the field 'x'"
expect_diag "dup fn" 'def f():\n    pass\ndef f():\n    pass\ndef main():\n    f()\n' "[N-5] Function 'f' is defined twice"
expect_diag "dup class" 'class A:\n    x: int\nclass A:\n    y: int\ndef main():\n    pass\n' "[N-5] 'A' is declared twice"
expect_diag "class vs enum" 'enum E:\n    A\nclass E:\n    x: int\ndef main():\n    pass\n' "[N-5] 'E' is declared twice"
expect_diag "dup variant" 'enum E:\n    A\n    B\n    A\ndef main():\n    pass\n' "[N-5] Variant 'A' is declared twice"
expect_diag "dup field" 'class A:\n    x: int\n    x: str\ndef main():\n    pass\n' "[N-5] Field 'x' is declared twice"
expect_diag "dup method" 'class A:\n    x: int\nextend A:\n    pub def f(self):\n        pass\n    pub def f(self):\n        pass\ndef main():\n    pass\n' "[N-5] Method 'A.f' is defined twice"
expect_diag "dup param" 'def f(a: int, a: int):\n    pass\ndef main():\n    f(1, 2)\n' "[N-5] Parameter 'a' of 'f()'"
expect_diag "fn vs class" 'class A:\n    x: int\ndef A():\n    pass\ndef main():\n    pass\n' "[N-5] 'A' is both a function and a class"
expect_diag "dup global" 'const X = 1\nconst X = 2\ndef main():\n    print(X)\n' "[N-5] Global 'X' is declared twice"
expect_diag "undefined generic static" 'def main():\n    mut b = Nope[str].init("a")\n' "[E-1] 'Nope' is not a defined type, so 'Nope[str].init(...)'"
expect_diag "unknown base" 'class A(Nope):\n    x: int\ndef main():\n    pass\n' "[N-6] Unknown base class 'Nope'"
expect_diag "self base" 'class A(A):\n    x: int\ndef main():\n    pass\n' "[N-6] Class 'A' cannot inherit from itself"
expect_diag "cyclic base" 'class A(B):\n    x: int\nclass B(A):\n    y: int\ndef main():\n    pass\n' "[N-6] Class 'A' inherits from 'B'"
expect_diag "enum base" 'enum E:\n    A\nclass C(E):\n    x: int\ndef main():\n    pass\n' "[N-6] Class 'C' cannot inherit from enum 'E'"
expect_diag "non-class base" 'def f():\n    pass\nclass A(f):\n    x: int\ndef main():\n    pass\n' "[N-6] Base 'f' of class 'A' is not a class"
expect_diag "missing module" 'import nonexistent_mod_zz\ndef main():\n    pass\n' "[N-7] Module 'nonexistent_mod_zz' was not found"
expect_diag "missing symbol" 'from std.math import nope_zz\ndef main():\n    pass\n' "[N-7] Module 'std.math' has no symbol 'nope_zz'"
expect_diag "variable as type" 'def main():\n    mut count = 3\n    mut y: count = 4\n    print(y)\n' "[N-4] 'count' is a variable, not a type"
expect_diag "one-letter variable as type" 'def main():\n    mut x: int = 3\n    mut y: x = 4\n    print(y)\n' "is a variable, not a type"
expect_diag "param as type" 'def f(total: int):\n    mut y: total = 4\n    print(y)\ndef main():\n    f(1)\n' "'total' is a variable, not a type"
expect_diag "instance method via type" 'class A:\n    pub v: int\n    pub def m(self) -> int:\n        return 1\ndef main():\n    print(A.m())\n' "is an instance method"
expect_diag "interface instantiated" 'interface I:\n    def f(self) -> int\ndef main():\n    mut a = I()\n' "[N-9] 'I' is an interface and cannot be instantiated"
expect_diag "static method on instance" 'class A:\n    pub x: int\n    pub def mk(px: int) -> A:\n        mut a = A()\n        a.x = px\n        return a\ndef main():\n    mut a = A.mk(1)\n    mut b = a.mk(2)\n    print(b.x)\n' "is a static method of 'A'"
expect_diag "no main" 'def foo():\n    pass\n' "[N-10] The program has no"
expect_diag "override param type" 'class B:\n    pub v: int\n    pub def f(self, a: int) -> int:\n        return a\nclass D(B):\n    pub def f(self, a: str) -> int:\n        return 1\ndef main():\n    mut d = D()\n    print(d.f("x"))\n' "[N-11] 'D.f' overrides 'B.f'"
expect_diag "override return type" 'class B:\n    pub v: int\n    pub def f(self, a: int) -> int:\n        return a\nclass D(B):\n    pub def f(self, a: int) -> str:\n        return "s"\ndef main():\n    mut d = D()\n    print(d.f(1))\n' "it returns str but 'B.f' returns int"
expect_diag "global named printf" 'mut printf: int = 3\ndef main():\n    printf = printf + 1\n    print(printf)\n' "[N-1] Global 'printf'"
expect_diag "class named sin" 'class sin:\n    pub v: int\ndef main():\n    mut o = sin()\n    print(o.v)\n' "C keyword or C library name"
expect_diag "enum named INT_MAX" 'enum INT_MAX:\n    A\n    B\ndef main():\n    mut e = INT_MAX.A\n' "C library macro"
expect_diag "function named size_t" 'def size_t(a: int) -> int:\n    return a\ndef main():\n    print(size_t(1))\n' "[N-1] 'size_t'"
expect_diag "duplicate global line" 'mut g: int = 1\nmut h: int = 2\nmut g: int = 3\ndef main():\n    print(g)\n' "ed.tr:3"
expect_diag "duplicate field line" 'class K:\n    pub a: int\n    pub b: int\n    pub a: int\ndef main():\n    mut k = K()\n    print(k.a)\n' "ed.tr:4"
expect_diag "module return line" 'def f():\n    pass\nreturn 1\ndef main():\n    pass\n' "ed.tr:3"
expect_diag "init on class without init" 'class V:\n    pub x: int\n    pub def get(self) -> int:\n        return self.x\ndef main():\n    mut v = V.init(3)\n    print(v.x)\n' "[E-1] No method 'init' found on type 'V'"
expect_diag "unknown runtime intrinsic" 'def main():\n    print(_tr_tcp_recv_zz(1))\n' "[N-12] '_tr_tcp_recv_zz' is not a function of the Tauraro runtime"
expect_diag "bare unknown name statement" 'H\ndef main():\n    pass\n' "[N-3] name 'H' is not defined"
expect_diag "unknown macro call" 'def main():\n    mut x = nomacro!(1)\n    print(x)\n' "[N-3] macro 'nomacro!' is not defined"
expect_diag "class field unknown type" 'class K:\n    pub cb: def(int) -> Foo\ndef main():\n    print(1)\n' "[N-4] Unknown type 'Foo' (in field 'cb' of 'K')"
expect_diag "method named free with value" 'class K:\n    pub v: int\n    pub def free(self) -> int:\n        return 1\ndef main():\n    mut k = K()\n    print(k.v)\n' "[N-1] 'free' cannot be used as a method name"
expect_diag "method named main" 'class K:\n    pub v: int\n    pub def main(self) -> int:\n        return 1\ndef main():\n    mut k = K()\n    print(k.v)\n' "[N-1] 'main' cannot be used as a method name"
expect_diag "print a Result" 'def f() -> Result[int, str]:\n    return Ok(1)\ndef main():\n    mut r: Result[int, str] = f()\n    print(r)\n' "[E-5] Type mismatch: a Result cannot be printed"
expect_diag "closure unknown return type" 'def main():\n    mut f = def (v: int) -> Foo:\n        return v\n    print(1)\n' "[N-4] Unknown type 'Foo' (in the return type of this closure)"
expect_diag "init on implements-only class" 'interface I:\n    def f(self) -> int\nclass V implements I:\n    pub x: int\n    pub def f(self) -> int:\n        return 1\ndef main():\n    mut v = V.init(3)\n' "[E-1] No method 'init' found on type 'V'"
expect_diag "print Vec of closures" 'def g(a: int) -> int:\n    return a\ndef main():\n    mut calls = Vec[def(int) -> int].init(2)\n    calls.push(g)\n    print(calls)\n' "a Vec of function values cannot be printed"
expect_diag "break outside loop" 'def main():\n    break\n' "[N-8] \`break\` outside a loop"
expect_diag "continue outside loop" 'def main():\n    continue\n' "[N-8] \`continue\` outside a loop"
expect_diag "return at module level" 'return 1\ndef main():\n    pass\n' "[N-8] \`return\` outside a function"
expect_diag "class as value" 'class A:\n    x: int\ndef main():\n    mut y = A\n' "[E-9] 'A' is a type, not a value"
expect_diag "function as operand" 'def f():\n    pass\ndef main():\n    mut z = f + 1\n' "[E-9] 'f' is a function, not a number"
expect_diag "redeclared param" 'def f(a: int):\n    mut a = 2\n    print(a)\ndef main():\n    f(1)\n' "[N-2] 'a' is a parameter of this function"
expect_diag "variant ctor arity" 'enum E:\n    A(int)\n    B\ndef main():\n    mut e = E.A(1, 2)\n' "[E-4] Variant 'E.A' takes 1 value(s) but 2 were given"
expect_diag "bare payload variant" 'enum E:\n    A(int)\n    B\ndef main():\n    mut e = E.A\n' "[E-4] Variant 'E.A' carries 1 value(s)"
expect_diag "variant pattern binds" 'enum E:\n    A(int)\n    B\ndef main():\n    mut e = E.A(1)\n    match e:\n        case E.A(x, y):\n            pass\n        case _:\n            pass\n' "[E-7] Pattern 'E.A' binds 2 value(s)"
expect_diag "unknown variant pattern" 'enum E:\n    A\n    B\ndef main():\n    mut e = E.A\n    match e:\n        case E.C:\n            pass\n        case _:\n            pass\n' "[E-7] Enum 'E' has no variant 'C'"
expect_diag "bare return" 'def f() -> int:\n    return\ndef main():\n    print(f())\n' "this 'return' has no value"
expect_diag "let no type no value" 'def main():\n    mut t\n    print(1)\n' "has neither a type nor an initial value"
expect_diag "missing field type" 'class A:\n    pub v:\n    def g(self) -> int:\n        return 1\ndef main():\n    print(1)\n' "expected a type here"
expect_diag "param list bracket" 'class A:\n    pub def g(self] -> int:\n        return 1\ndef main():\n    print(1)\n' "expected ')' to close the parameter list"
expect_diag "def keyword name" 'def\ndef main():\n    print(1)\n' "expected a function name after 'def'"
expect_diag "pattern literal kind" 'def main():\n    mut x = 1\n    match x:\n        case "a":\n            print(1)\n        case _:\n            pass\n' "pattern is a str literal but the matched value is an int"
expect_diag "spawn no args" 'def main():\n    mut t = Thread.spawn()\n' "'Thread.spawn()' takes at least 1 argument"
expect_diag "cast unknown type" 'def main():\n    mut x = 1\n    mut y = x as Nope\n' "Unknown type 'Nope' (in this 'as' cast)"
expect_diag "nesting too deep" "def main():\n    mut x = $(printf '(%.0s' $(seq 1 200))1$(printf ')%.0s' $(seq 1 200))\n" "nested too deeply"
expect_diag "class header bracket" 'class Box]K, V]:\n    pub a: int\ndef main():\n    print(1)\n' "expected ':' after the header of class 'Box'"
expect_diag "try without except" 'def main():\n    try:\n        print(1)\n' "'try' block has no 'except' or 'finally' clause"
expect_diag "enum payload type" 'enum E:\n    A(int)\ndef main():\n    mut e = E.A("s")\n' "payload 1 of 'E.A'"
expect_diag "spawn arg type" 'def w(x: int):\n    pass\ndef main():\n    mut t = Thread.spawn(w, "s")\n    t.join()\n' "passed through Thread.spawn to 'w()'"
expect_diag "cast int to str" 'def main():\n    mut x = 5\n    mut y = x as str\n    print(y)\n' "cannot be cast to str with 'as'"
expect_diag "cast str to float" 'def main():\n    mut s = "a"\n    mut y = s as float\n    print(y)\n' "cannot be cast to float with 'as'"
expect_diag "undefined property receiver" 'def main():\n    mut x = Foo.Bar\n' "'Foo' is not a defined type or value, so 'Foo.Bar' cannot be resolved"
expect_diag "lambda chain too deep" "def main():\n    mut x = $(printf 'lambda: %.0s' $(seq 1 300))1\n" "nested too deeply"
# --freestanding without the allocator hooks used to die in the C preprocessor (#error)
printf 'def main():\n    print(1)\n' > "$diagdir/fsnoalloc.tr"
dout=$("$TAURAROC" --freestanding "$diagdir/fsnoalloc.tr" -o "$diagdir/fsnoalloc" 2>&1)
case "$dout" in *"[TIER-1] --freestanding needs an allocator"*) ;; *) diagfail="$diagfail [freestanding without allocator hooks]";; esac
# No C compiler warning may ever reach the user for a valid program (float formatting used to leak -Wformat-truncation).
printf 'def main():\n    x = 5\n    y = x * 2.5\n    print(x, y)\n    print(1.5e300 * 3.0)\n' > "$diagdir/warn.tr"
wtr="$(cd "$(dirname "$TAURAROC")" && pwd)/$(basename "$TAURAROC")"
wout=$(cd "$diagdir" && rm -rf build && "$wtr" warn.tr -o warn 2>&1)
if [ -n "$wout" ]; then diagfail="$diagfail [C compiler output leaked for a valid program: $(echo "$wout" | head -2 | tr '\n' ' ')]"; fi
expect_diag "set unknown method" 'def main():\n    mut s: Set[int] = Set[int].init()\n    s.nope()\n' "[E-6] a Set has no method 'nope()'"
expect_diag "universal-named user method arity" 'class A:\n    pub v: int\nextend A:\n    pub def as_str(self) -> str:\n        return "a"\ndef main():\n    mut a = A()\n    print(a.as_str(1))\n' "[E-4] 'A.as_str()' takes 0 arguments"
expect_diag "str method arg kind" 'def main():\n    mut p = "a,b".split(42)\n' "[E-5] Type mismatch: argument 1 of 'str.split()'"
expect_diag "try-call element kind" 'def pn(s: str) throws str -> int:\n    return 1\ndef fl(xs: List[str], s: str) throws str -> int:\n    xs[1] = pn(s)?\n    return 1\ndef main():\n    pass\n' "[E-5] Type mismatch: an element of this List[str]"
expect_diag "static call unknown method" 'class A:\n    pub v: int\ndef main():\n    mut a = A.nope(1)\n' "[E-1] No method 'nope' found on type 'A'"
expect_diag "malformed fn type" 'def f(n: int) -> def(int{ -> int:\n    return 1\ndef main():\n    print(1)\n' "expected a type here"
expect_diag "unknown extern abi" 'extern "Rust":\n    def foo(x: int) -> int\ndef main():\n    print(1)\n' "unknown extern ABI"
expect_diag "undefined extern symbol at link" 'extern "C":\n    def tr_no_such_symbol_zz(x: int) -> int\ndef main():\n    print(tr_no_such_symbol_zz(1))\n' "[LNK-1] link failed: undefined symbol 'tr_no_such_symbol_zz'"
expect_diag "method read as field" 'class A:\n    pub v: int\n    pub def get(self) -> int:\n        return self.v\ndef main():\n    mut a = A()\n    a.get\n' "is a method of 'A', not a field"
expect_diag "generic method arg kind" 'class Box[T]:\n    pub v: T\nextend Box:\n    pub def init(v: T) -> Box[T]:\n        mut b = Box[T]()\n        b.v = v\n        return b\n    pub def put(self, x: T):\n        self.v = x\ndef main():\n    mut b: Box[str] = Box[str].init("a")\n    b.put(5)\n' "[E-5] Type mismatch: argument 1 of 'Box.put()'"
expect_diag "type args on non-generic class" 'class Box:\n    pub v: int\ndef main():\n    mut b = Box[int, str]()\n' "[E-4] class 'Box' is not generic"
expect_diag "generic fn type arg count" 'def idf[T](x: T) -> T:\n    return x\ndef main():\n    print(idf[int, str](1))\n' "[E-4] function 'idf' takes 1 type argument(s) but 2 given"
expect_diag "generic class no params" 'class Box[T]:\n    pub v: T\nextend Box:\n    pub def init(v: T) -> Box[T]:\n        mut b = Box[T]()\n        b.v = v\n        return b\n    pub def put(self, x: T):\n        self.v = x\ndef main():\n    mut b = Box()\n' "[G-2] Cannot infer the type parameters of generic class 'Box'"
expect_diag "Chan.new" 'def main():\n    mut c = Chan[int].new(1)\n' "[E-1] No method 'new' found on type 'Chan'"
expect_diag "tuple get arity" 'def main():\n    mut t = (1, 2)\n    print(t.get())\n' "[E-4] 'Tuple.get()' takes 1 argument(s)"
expect_diag "init with self" 'class A:\n    pub v: int\nextend A:\n    pub def init(self, v: int) -> A:\n        self.v = v\n        return self\ndef main():\n    mut a = A(7)\n' "[E-4] 'A.init' must not take 'self'"
expect_diag "unknown decorator" '@nosuchdecorator\ndef f(x: int) -> int:\n    return x\ndef main():\n    print(f(1))\n' "[N-9] Unknown decorator '@nosuchdecorator'"
expect_diag "unknown generic bound" 'def f[T: Nope](x: T) -> T:\n    return x\ndef main():\n    print(f(1))\n' "[G-1] Unknown bound 'Nope'"
expect_diag "duplicate extern decl" 'extern "C":\n    def getpid() -> int\n    def getpid() -> int\ndef main():\n    print(getpid())\n' "[N-5] extern function 'getpid' is declared twice"
expect_diag "implicit-binding arg kind" 'def ci(v: List[int]) -> int:\n    return 1\ndef main():\n    v = 3\n    print(ci(v))\n' "expects 'List' but is given an int"
expect_diag "closure param arg kind" 'def ci(v: List[int]) -> int:\n    return 1\ndef main():\n    g = def (v: int) -> int:\n        return ci(v)\n    print(g(1))\n' "expects 'List' but is given an int"
expect_diag "tuple has no len" 'def main():\n    q = (1, 2)\n    print(q.len)\n' "A tuple has no field 'len'"
expect_diag "float as Pointer" 'def main():\n    r = 1.5\n    p = r as Pointer[char]\n    print(1)\n' "cannot be cast to a Pointer"
expect_diag "unknown Env method" 'from std.sys.env import Env\ndef main():\n    mut e = Env.init()\n    print(e.has_var_zz("A"))\n' "No method 'has_var_zz' found on type 'Env'"
expect_diag "lambda str + int" 'def main():\n    f = lambda x: x + 1\n    print(f("s"))\n' "cannot apply '+' to a str and an int"
expect_diag "lambda int + str" 'def main():\n    f = lambda x: x + "a"\n    print(f(1))\n' "cannot apply '+' to an int and a str"
expect_diag "if on Dict" 'def main():\n    d = {"a": 1}\n    if d:\n        print(1)\n' "must be a bool, but it is a Dict"
expect_diag "while on Set" 'def main():\n    mut s = {1, 2}\n    while s:\n        print(1)\n' "must be a bool, but it is a Set"
expect_diag "unknown class decorator" '@bogus\nclass A:\n    pub x: int\ndef main():\n    print(1)\n' "Unknown decorator '@bogus' on class 'A'"
expect_diag "duplicate extern blocks" 'extern "C":\n    def getpid() -> int\nextern "C":\n    def getpid() -> int\ndef main():\n    print(getpid())\n' "declared twice (in two extern blocks)"
expect_diag "missing colon def" 'def main()\n    print(1)\n' "missing ':' at the end of this function header"
expect_diag "const field assign" 'class Cell:\n    pub v: str\ndef main():\n    const c = Cell()\n    c.v = "x"\n' "because 'c' is a constant"
expect_diag "ctor unknown type arg" 'from std.core.vec import Vec\ndef main():\n    mut v: Vec[int] = Vec[Nope].init(4)\n    v.push(1)\n' "[N-4] Unknown type 'Nope' (in a type argument of a constructor call)"
expect_diag "Env unknown method" 'from std.sys.env import Env\ndef main():\n    mut e = Env.init()\n    print(e.has_var_zz("A"))\n' "[E-1] No method 'has_var_zz' found on type 'Env'"
expect_diag "Str not imported" 'def main():\n    print(Str.substr("hello", 1, 2))\n' "[N-3] name 'Str' is not defined"
expect_diag "generic get wrong arg kind" 'class Box[K, V]:\n    pub k: K\n    pub v: V\nextend Box[K, V]:\n    pub def init(k: K, v: V) -> Box[K, V]:\n        mut b = Box[K, V]()\n        b.k = k\n        b.v = v\n        return b\n    pub def get(self, key: K) -> V:\n        return self.v\ndef main():\n    mut b = Box[int, str].init(5, "five")\n    print(b.get("x"))\n' "[E-5] Type mismatch: argument 1 of 'Box.get()'"
expect_diag "method on void call" 'class P:\n    pub x: int\n    pub def hi(self):\n        print(self.x)\ndef main():\n    mut p = P(1)\n    p.hi()\n    .foo(p, 2)\n' "cannot call '.foo()' on a call that returns nothing"
expect_diag "binop void rhs" 'def f():\n    print(1)\ndef main():\n    mut x = 5 -\n    f()\n    print(x)\n' "the right operand of '-' is a call that returns nothing"
expect_diag "call a number" 'def main():\n    print(1e (2, 3))\n' "a number is not callable"
expect_diag "Option.some no arg" 'def main():\n    mut o = Option.some()\n    print(o.is_some())\n' "'Option.some()' takes 1 argument(s) but 0 given"
expect_diag "assign to list literal" 'def main():\n    mut arr: [int; 4]\n    [arr0] = 7\n    print(arr[0])\n' "cannot assign to a list literal"
expect_diag "generic class as value" 'class Box[K, V]:\n    pub k: K\n    pub v: V\ndef main():\n    mut m = Box[str, int]\n    print(1)\n' "'Box[...]' is a type, not a value"
expect_diag "str.eq no arg" 'def main():\n    mut a = "x"\n    print(a.eq())\n' "'str.eq()' takes 1 argument but 0 were given"
expect_diag "pub without def in extend" 'class Foo:\n    pub v: int\nextend Foo:\n    pub  to_int(self) -> int:\n        return self.v\ndef main():\n    print(1)\n' "expected 'def' to declare a method of 'Foo'"
expect_diag "match expr without colon" 'def main():\n    print("a", match"b")\n' "expected ':' after the subject of a match expression"
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