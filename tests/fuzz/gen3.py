#!/usr/bin/env python3
"""Expression-semantics fuzzer: random integer / boolean expressions emitted as Tauraro and Python.

The Python oracle emulates the Tauraro integer model: 64-bit two's-complement wrap-around, `/` and `%`
truncating toward zero (C semantics), arithmetic `>>`, shift counts masked to 0..63.  Expressions are
printed with MINIMAL parentheses (driven by the precedence table below), so a wrong precedence or
associativity in the Tauraro parser shows up as a different value.

Usage:  gen3.py <seed> <outdir> [count]
"""
import os
import random
import sys

# (precedence, assoc) -- higher binds tighter.  Same relative order as Python.
PREC = {
    "or": 1, "and": 2,
    "==": 4, "!=": 4, "<": 4, "<=": 4, ">": 4, ">=": 4,
    "|": 5, "^": 6, "&": 7,
    "<<": 8, ">>": 8,
    "+": 9, "-": 9,
    "*": 10, "/": 10, "%": 10,
}
UNARY_PREC = 11


class N:
    pass


def w64(x):
    x &= (1 << 64) - 1
    return x - (1 << 64) if x >> 63 else x


def cdiv(a, b):
    q = abs(a) // abs(b)
    return q if (a < 0) == (b < 0) else -q


def cmod(a, b):
    return a - cdiv(a, b) * b


class Gen:
    def __init__(self, seed):
        self.r = random.Random(seed)
        self.vars = []

    def lit(self):
        k = self.r.random()
        if k < 0.5:
            v = self.r.randint(0, 20)
        elif k < 0.8:
            v = self.r.randint(0, 5000)
        elif k < 0.95:
            v = self.r.choice([255, 256, 1023, 65535, 65536, 2147483647, 2147483648, 4294967295, 4294967296])
        else:
            v = self.r.choice([9223372036854775807, 4611686018427387904, 1000000007, 1099511627776])
        return ("lit", v)

    def expr(self, d):
        if d <= 0 or self.r.random() < 0.18:
            if self.vars and self.r.random() < 0.45:
                return ("var", self.r.choice(self.vars))
            return self.lit()
        k = self.r.random()
        if k < 0.10:
            return ("neg", self.expr(d - 1))
        ops = ["+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^"]
        op = self.r.choice(ops)
        a, b = self.expr(d - 1), self.expr(d - 1)
        if op in ("/", "%"):
            b = ("bin", "|", b, ("lit", 1))          # odd divisor: never zero
        if op in ("<<", ">>"):
            b = ("bin", "&", b, ("lit", 63))         # keep the count in range
        return ("bin", op, a, b)

    def cond(self, d):
        if d <= 0 or self.r.random() < 0.3:
            op = self.r.choice(["==", "!=", "<", "<=", ">", ">="])
            return ("cmp", op, self.expr(1), self.expr(1))
        if self.r.random() < 0.25:
            return ("not", self.cond(d - 1))
        if self.r.random() < 0.5:
            return ("bool", self.r.choice(["and", "or"]), self.cond(d - 1), self.cond(d - 1))
        op = self.r.choice(["==", "!=", "<", "<=", ">", ">="])
        return ("cmp", op, self.expr(d - 1), self.expr(d - 1))


def prec_of(n):
    if n[0] == "bin":
        return PREC[n[1]]
    if n[0] == "cmp":
        return PREC[n[1]]
    if n[0] == "bool":
        return PREC[n[1]]
    if n[0] in ("neg", "not"):
        return UNARY_PREC
    return 99


def show(n, parent_prec=0, right=False):
    """Print with minimal parentheses.  Binary operators are left-associative."""
    k = n[0]
    if k == "lit":
        return str(n[1])
    if k == "var":
        return n[1]
    if k == "neg":
        s = "-" + show(n[1], UNARY_PREC)
        return "(%s)" % s if parent_prec > UNARY_PREC else s
    if k == "not":
        s = "not " + show(n[1], 3)
        return "(%s)" % s if parent_prec > 3 else s
    if k in ("bin", "cmp", "bool"):
        op = n[1]
        p = PREC[op]
        # comparisons do not chain: always parenthesise a comparison operand of a comparison
        left, rgt = n[2], n[3]
        ls = show(left, p, False)
        rs = show(rgt, p, True)
        if k == "cmp":
            if left[0] == "cmp":
                ls = "(%s)" % ls
            if rgt[0] == "cmp":
                rs = "(%s)" % rs
        s = "%s %s %s" % (ls, op, rs)
        need = parent_prec > p or (right and parent_prec == p)
        return "(%s)" % s if need else s
    raise ValueError(k)


def ev(n, env):
    k = n[0]
    if k == "lit":
        return n[1]
    if k == "var":
        return env[n[1]]
    if k == "neg":
        return w64(-ev(n[1], env))
    if k == "not":
        return not ev(n[1], env)
    a = ev(n[2], env)
    if k == "cmp":
        b = ev(n[3], env)
        return {"==": a == b, "!=": a != b, "<": a < b, "<=": a <= b, ">": a > b, ">=": a >= b}[n[1]]
    if k == "bool":
        if n[1] == "and":
            return bool(a) and bool(ev(n[3], env))
        return bool(a) or bool(ev(n[3], env))
    b = ev(n[3], env)
    op = n[1]
    if op == "+": return w64(a + b)
    if op == "-": return w64(a - b)
    if op == "*": return w64(a * b)
    if op == "/": return w64(cdiv(a, b))
    if op == "%": return w64(cmod(a, b))
    if op == "<<": return w64(a << (b & 63))
    if op == ">>": return a >> (b & 63)
    if op == "&": return a & b
    if op == "|": return a | b
    if op == "^": return a ^ b
    raise ValueError(op)


def main():
    seed = int(sys.argv[1])
    outdir = sys.argv[2]
    count = int(sys.argv[3]) if len(sys.argv) > 3 else 40
    g = Gen(seed)
    g.vars = ["a", "b", "c"]
    env = {"a": g.r.randint(0, 100), "b": g.r.randint(0, 100000), "c": g.r.choice([7, 255, 1000003, 4294967296, 123456789012])}
    tau = ["def main():"]
    py = ["def w64(x):", "    x &= (1 << 64) - 1", "    return x - (1 << 64) if x >> 63 else x",
          "def cdiv(a, b):", "    q = abs(a) // abs(b)", "    return q if (a < 0) == (b < 0) else -q",
          "def cmod(a, b):", "    return a - cdiv(a, b) * b", ""]
    for name, val in env.items():
        tau.append("    mut %s = %d" % (name, val))
    expected = []
    for i in range(count):
        if g.r.random() < 0.7:
            e = g.expr(g.r.randint(2, 5))
            v = ev(e, env)
            tau.append("    print(%s)" % show(e))
            expected.append(("true" if v else "false") if isinstance(v, bool) else str(v))
        else:
            e = g.cond(g.r.randint(1, 3))
            v = bool(ev(e, env))
            tau.append("    if %s:" % show(e))
            tau.append("        print(1)")
            tau.append("    else:")
            tau.append("        print(0)")
            expected.append("1" if v else "0")
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "p.tr"), "w") as f:
        f.write("\n".join(tau) + "\n")
    # the "Python side" is just the expected output, computed by the emulating evaluator above
    with open(os.path.join(outdir, "p.py"), "w") as f:
        f.write("print(%r, end='')\n" % ("\n".join(expected) + "\n"))


if __name__ == "__main__":
    main()
