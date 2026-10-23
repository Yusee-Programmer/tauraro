#!/usr/bin/env python3
"""Differential program generator for the Tauraro compiler.

Emits ONE random-but-valid program in BOTH Tauraro and Python (same semantics by
construction).  Running both and diffing stdout finds compiler bugs without any
hand-written expected output.  The programs stick to a subset where the two
languages agree:

  * ints are non-negative and bounded (every arithmetic result is reduced mod M),
    so there is no signed-overflow / floor-vs-truncating-division difference;
  * strings are ASCII (Tauraro strings are byte strings);
  * containers are never iterated while being mutated, and a container is never
    shared between two names unless the language aliases it identically;
  * helper functions (sub/at/absd/...) hide the places where the languages
    legitimately differ (index clamping, Python negative indices, ...).

Usage:  gen2.py <seed> <outdir> [size]      -> writes <outdir>/p.tr and <outdir>/p.py
"""
import random
import sys
import os

M = 1009  # all ints stay in [0, M)


class Var:
    def __init__(self, name, ty):
        self.name, self.ty = name, ty


class Gen:
    def __init__(self, seed, size):
        self.r = random.Random(seed)
        self.size = size
        self.fns = []          # (name, [param types], ret type)
        self.tau = []
        self.py = []
        self.n_tmp = 0

    # ------------------------------------------------------------------ utils
    def rnd(self, a, b):
        return self.r.randint(a, b)

    def pick(self, xs):
        return xs[self.r.randrange(len(xs))]

    def chance(self, p):
        return self.r.random() < p

    def fresh(self, prefix):
        self.n_tmp += 1
        return "%s%d" % (prefix, self.n_tmp)

    # ------------------------------------------------------------ expressions
    # every expression generator returns (tauraro_code, python_code)
    def lit_int(self):
        v = str(self.rnd(0, 60))
        return v, v

    def lit_str(self):
        s = self.pick(["", "a", "bc", "xyz", "hello", "Tauraro", "a,b,c", " pad ", "x-y", "Q"])
        return '"%s"' % s, '"%s"' % s

    def e_int(self, env, d=0):
        vs = [v for v in env if v.ty == "int"]
        opts = ["lit", "lit"]
        if vs:
            opts += ["var"] * 4
        if d < 3:
            opts += ["add", "mul", "absd", "len_s", "len_li", "at_li", "dget", "objn", "call", "find", "cmpi", "sumli", "strat"]
        k = self.pick(opts)
        if k == "lit":
            return self.lit_int()
        if k == "var":
            v = self.pick(vs)
            return v.name, v.name
        if k == "add":
            a, b = self.e_int(env, d + 1), self.e_int(env, d + 1)
            return "((%s + %s) %% %d)" % (a[0], b[0], M), "((%s + %s) %% %d)" % (a[1], b[1], M)
        if k == "mul":
            a, b = self.e_int(env, d + 1), self.e_int(env, d + 1)
            return "((%s * %s) %% %d)" % (a[0], b[0], M), "((%s * %s) %% %d)" % (a[1], b[1], M)
        if k == "absd":
            a, b = self.e_int(env, d + 1), self.e_int(env, d + 1)
            return "absd(%s, %s)" % (a[0], b[0]), "absd(%s, %s)" % (a[1], b[1])
        if k == "len_s":
            s = self.e_str(env, d + 1)
            return "%s.len()" % self.paren(s[0]), "len(%s)" % s[1]
        if k == "len_li":
            xs = self.e_li(env, d + 1)
            return "len(%s)" % xs[0], "len(%s)" % xs[1]
        if k == "at_li":
            xs, i = self.e_li(env, d + 1), self.e_int(env, d + 1)
            return "at_i(%s, %s)" % (xs[0], i[0]), "at_i(%s, %s)" % (xs[1], i[1])
        if k == "dget":
            dd, key = self.e_di(env, d + 1), self.e_str(env, d + 1)
            return "dget(%s, %s)" % (dd[0], key[0]), "dget(%s, %s)" % (dd[1], key[1])
        if k == "objn":
            o = self.e_obj(env, d + 1)
            return "%s.n" % self.paren(o[0]), "%s.n" % self.paren(o[1])
        if k == "call":
            cands = [f for f in self.fns if f[2] == "int"]
            if not cands:
                return self.lit_int()
            f = self.pick(cands)
            args = [self.cpv(t, self.expr(t, env, d + 1)) for t in f[1]]
            return "%s(%s)" % (f[0], ", ".join(a[0] for a in args)), "%s(%s)" % (f[0], ", ".join(a[1] for a in args))
        if k == "find":
            s, t = self.e_str(env, d + 1), self.lit_str()
            return "(%s.find(%s) + 1)" % (self.paren(s[0]), t[0]), "(%s.find(%s) + 1)" % (self.paren(s[1]), t[1])
        if k == "cmpi":
            a, b = self.e_int(env, d + 1), self.e_int(env, d + 1)
            return "b2i(%s < %s)" % (a[0], b[0]), "b2i(%s < %s)" % (a[1], b[1])
        if k == "sumli":
            xs = self.e_li(env, d + 1)
            return "sum_i(%s)" % xs[0], "sum_i(%s)" % xs[1]
        if k == "strat":
            s, i = self.e_str(env, d + 1), self.e_int(env, d + 1)
            return "chr_at(%s, %s)" % (s[0], i[0]), "chr_at(%s, %s)" % (s[1], i[1])
        return self.lit_int()

    def cpv(self, ty, e):
        """Containers/objects that are a bare variable get copied: Tauraro moves classes on
        assignment and rejects the same container twice in one call."""
        if not e[0].replace("_", "").isalnum():
            return e
        fn = {"li": "cp_li", "ls": "cp_ls", "di": "cp_di", "obj": "cp_obj"}.get(ty)
        if fn is None:
            return e
        return "%s(%s)" % (fn, e[0]), "%s(%s)" % (fn, e[1])

    def paren(self, code):
        return "(%s)" % code if not code.replace("_", "").isalnum() else code

    def e_str(self, env, d=0):
        vs = [v for v in env if v.ty == "str"]
        opts = ["lit", "lit"]
        if vs:
            opts += ["var"] * 4
        if d < 3:
            opts += ["cat", "cat", "sub", "upper", "int2s", "fstr", "replace", "objs", "call", "joinls", "atls", "repeat"]
        k = self.pick(opts)
        if k == "lit":
            return self.lit_str()
        if k == "var":
            v = self.pick(vs)
            return v.name, v.name
        if k == "cat":
            a, b = self.e_str(env, d + 1), self.e_str(env, d + 1)
            return "(%s + %s)" % (a[0], b[0]), "(%s + %s)" % (a[1], b[1])
        if k == "sub":
            s, a, b = self.e_str(env, d + 1), self.e_int(env, d + 1), self.e_int(env, d + 1)
            return "sub(%s, %s, %s)" % (s[0], a[0], b[0]), "sub(%s, %s, %s)" % (s[1], a[1], b[1])
        if k == "upper":
            s = self.e_str(env, d + 1)
            return "%s.upper()" % self.paren(s[0]), "%s.upper()" % self.paren(s[1])
        if k == "int2s":
            i = self.e_int(env, d + 1)
            return "str(%s)" % i[0], "str(%s)" % i[1]
        if k == "fstr":
            i, s = self.e_int(env, d + 1), self.e_str(env, d + 1)
            # bind to locals first: nested quotes inside f-string braces are a portability trap
            return 'fmt2(%s, %s)' % (i[0], s[0]), 'fmt2(%s, %s)' % (i[1], s[1])
        if k == "replace":
            s = self.e_str(env, d + 1)
            a = self.pick(["a", "b", "x", "l"])
            b = self.pick(["", "Z", "yy"])
            return '%s.replace("%s", "%s")' % (self.paren(s[0]), a, b), '%s.replace("%s", "%s")' % (self.paren(s[1]), a, b)
        if k == "objs":
            o = self.e_obj(env, d + 1)
            return "%s.s" % self.paren(o[0]), "%s.s" % self.paren(o[1])
        if k == "call":
            cands = [f for f in self.fns if f[2] == "str"]
            if not cands:
                return self.lit_str()
            f = self.pick(cands)
            args = [self.cpv(t, self.expr(t, env, d + 1)) for t in f[1]]
            return "%s(%s)" % (f[0], ", ".join(a[0] for a in args)), "%s(%s)" % (f[0], ", ".join(a[1] for a in args))
        if k == "joinls":
            xs = self.e_ls(env, d + 1)
            return "join_s(%s)" % xs[0], "join_s(%s)" % xs[1]
        if k == "atls":
            xs, i = self.e_ls(env, d + 1), self.e_int(env, d + 1)
            return "at_s(%s, %s)" % (xs[0], i[0]), "at_s(%s, %s)" % (xs[1], i[1])
        if k == "repeat":
            s, i = self.e_str(env, d + 1), self.e_int(env, d + 1)
            return "rep(%s, %s)" % (s[0], i[0]), "rep(%s, %s)" % (s[1], i[1])
        return self.lit_str()

    def e_li(self, env, d=0):
        vs = [v for v in env if v.ty == "li"]
        opts = ["lit"]
        if vs:
            opts += ["var"] * 4
        if d < 3:
            opts += ["clone", "slice", "cat", "mapadd", "objxs", "call", "splitlens", "rangeli"]
        k = self.pick(opts)
        if k == "lit":
            n = self.rnd(0, 4)
            items = [self.rnd(0, 50) for _ in range(n)]
            return "mk_li%d(%s)" % (n, ", ".join(map(str, items))), "[%s]" % ", ".join(map(str, items))
        if k == "var":
            v = self.pick(vs)
            return v.name, v.name
        if k == "clone":
            xs = self.e_li(env, d + 1)
            return "cp_li(%s)" % xs[0], "cp_li(%s)" % xs[1]
        if k == "slice":
            xs, a, b = self.e_li(env, d + 1), self.e_int(env, d + 1), self.e_int(env, d + 1)
            return "sl_li(%s, %s, %s)" % (xs[0], a[0], b[0]), "sl_li(%s, %s, %s)" % (xs[1], a[1], b[1])
        if k == "cat":
            a, b = self.e_li(env, d + 1), self.cpv("li", self.e_li(env, d + 1))
            return "cat_li(%s, %s)" % (a[0], b[0]), "cat_li(%s, %s)" % (a[1], b[1])
        if k == "mapadd":
            xs, i = self.e_li(env, d + 1), self.e_int(env, d + 1)
            return "mapadd(%s, %s)" % (xs[0], i[0]), "mapadd(%s, %s)" % (xs[1], i[1])
        if k == "objxs":
            o = self.e_obj(env, d + 1)
            return "cp_li(%s.xs)" % self.paren(o[0]), "cp_li(%s.xs)" % self.paren(o[1])
        if k == "call":
            cands = [f for f in self.fns if f[2] == "li"]
            if not cands:
                return self.e_li(env, 3)
            f = self.pick(cands)
            args = [self.cpv(t, self.expr(t, env, d + 1)) for t in f[1]]
            return "%s(%s)" % (f[0], ", ".join(a[0] for a in args)), "%s(%s)" % (f[0], ", ".join(a[1] for a in args))
        if k == "splitlens":
            s = self.e_str(env, d + 1)
            return "split_lens(%s)" % s[0], "split_lens(%s)" % s[1]
        if k == "rangeli":
            n = self.e_int(env, d + 1)
            return "range_li(%s)" % n[0], "range_li(%s)" % n[1]
        return "mk_li0()", "[]"

    def e_ls(self, env, d=0):
        vs = [v for v in env if v.ty == "ls"]
        opts = ["lit"]
        if vs:
            opts += ["var"] * 4
        if d < 3:
            opts += ["clone", "split", "mapcat", "keys", "call"]
        k = self.pick(opts)
        if k == "lit":
            n = self.rnd(0, 3)
            items = [self.lit_str() for _ in range(n)]
            return "mk_ls%d(%s)" % (n, ", ".join(i[0] for i in items)), "[%s]" % ", ".join(i[1] for i in items)
        if k == "var":
            v = self.pick(vs)
            return v.name, v.name
        if k == "clone":
            xs = self.e_ls(env, d + 1)
            return "cp_ls(%s)" % xs[0], "cp_ls(%s)" % xs[1]
        if k == "split":
            s = self.e_str(env, d + 1)
            return 'split_c(%s)' % s[0], 'split_c(%s)' % s[1]
        if k == "mapcat":
            xs, s = self.e_ls(env, d + 1), self.e_str(env, d + 1)
            return "mapcat(%s, %s)" % (xs[0], s[0]), "mapcat(%s, %s)" % (xs[1], s[1])
        if k == "keys":
            dd = self.e_di(env, d + 1)
            return "dkeys(%s)" % dd[0], "dkeys(%s)" % dd[1]
        if k == "call":
            cands = [f for f in self.fns if f[2] == "ls"]
            if not cands:
                return self.e_ls(env, 3)
            f = self.pick(cands)
            args = [self.cpv(t, self.expr(t, env, d + 1)) for t in f[1]]
            return "%s(%s)" % (f[0], ", ".join(a[0] for a in args)), "%s(%s)" % (f[0], ", ".join(a[1] for a in args))
        return "mk_ls0()", "[]"

    def e_di(self, env, d=0):
        vs = [v for v in env if v.ty == "di"]
        opts = ["new"]
        if vs:
            opts += ["var"] * 4
        if d < 3 and vs:
            opts += ["clone"]
        k = self.pick(opts)
        if k == "var":
            v = self.pick(vs)
            return v.name, v.name
        if k == "clone":
            dd = self.e_di(env, d + 1)
            return "cp_di(%s)" % dd[0], "cp_di(%s)" % dd[1]
        return "new_di()", "{}"

    def e_obj(self, env, d=0):
        vs = [v for v in env if v.ty == "obj"]
        opts = ["make"]
        if vs:
            opts += ["var"] * 4
        if d < 3:
            opts += ["call_obj"]
        k = self.pick(opts)
        if k == "var":
            v = self.pick(vs)
            return v.name, v.name
        if k == "call_obj":
            cands = [f for f in self.fns if f[2] == "obj"]
            if cands:
                f = self.pick(cands)
                args = [self.cpv(t, self.expr(t, env, d + 1)) for t in f[1]]
                return "%s(%s)" % (f[0], ", ".join(a[0] for a in args)), "%s(%s)" % (f[0], ", ".join(a[1] for a in args))
        n, s = self.e_int(env, d + 1), self.e_str(env, d + 1)
        return "P.make(%s, %s)" % (n[0], s[0]), "P.make(%s, %s)" % (n[1], s[1])

    def expr(self, ty, env, d=0):
        return {"int": self.e_int, "str": self.e_str, "li": self.e_li, "ls": self.e_ls,
                "di": self.e_di, "obj": self.e_obj}[ty](env, d)

    def e_cond(self, env):
        a, b = self.e_int(env, 1), self.e_int(env, 1)
        op = self.pick(["<", ">", "<=", ">=", "==", "!="])
        c = ("%s %s %s" % (a[0], op, b[0]), "%s %s %s" % (a[1], op, b[1]))
        if self.chance(0.3):
            s, t = self.e_str(env, 1), self.e_str(env, 1)
            op2 = self.pick(["==", "!="])
            c2 = ("%s %s %s" % (s[0], op2, t[0]), "%s %s %s" % (s[1], op2, t[1]))
            j = self.pick(["and", "or"])
            return ("(%s) %s (%s)" % (c[0], j, c2[0]), "(%s) %s (%s)" % (c[1], j, c2[1]))
        return c

    # ------------------------------------------------------------ statements
    def emit(self, ind, tau, py=None):
        self.tau.append("    " * ind + tau)
        self.py.append("    " * ind + (tau if py is None else py))

    def block(self, ind, env, n, depth, frozen, in_loop):
        for _ in range(n):
            self.stmt(ind, env, depth, frozen, in_loop)

    def stmt(self, ind, env, depth, frozen, in_loop):
        # Wrap every statement in marker comments so tests/fuzz/reduce2.py can delete whole
        # statements (with their nested blocks) from both the Tauraro and Python sides.
        self.n_unit = getattr(self, "n_unit", 0) + 1
        uid = self.n_unit
        pad = "    " * ind
        self.tau.append("%s# <<%d" % (pad, uid))
        self.py.append("%s# <<%d" % (pad, uid))
        self.stmt_inner(ind, env, depth, frozen, in_loop)
        self.tau.append("%s# >>%d" % (pad, uid))
        self.py.append("%s# >>%d" % (pad, uid))

    def stmt_inner(self, ind, env, depth, frozen, in_loop):
        mutable = lambda ty: [v for v in env if v.ty == ty and v.name not in frozen]
        opts = ["asg_i", "asg_s", "asg_li", "asg_ls", "print", "print", "app_li", "app_ls", "dset", "obj_mut", "obj_meth",
                "pop", "sort", "setidx", "extend", "obj_new", "dalias_clone", "str_cat", "try"]
        if depth < 2:
            opts += ["if", "if", "while", "for_li", "for_ls", "for_range"]
        if in_loop:
            opts += ["brk", "cnt"]
        k = self.pick(opts)
        if k == "asg_i" and mutable("int"):
            v, e = self.pick(mutable("int")), self.e_int(env)
            self.emit(ind, "%s = %s" % (v.name, e[0]), "%s = %s" % (v.name, e[1]))
        elif k == "asg_s" and mutable("str"):
            v, e = self.pick(mutable("str")), self.e_str(env)
            self.emit(ind, "%s = %s" % (v.name, e[0]), "%s = %s" % (v.name, e[1]))
        elif k == "asg_li" and mutable("li"):
            v, e = self.pick(mutable("li")), self.e_li(env)
            self.emit(ind, "%s = %s" % (v.name, e[0]), "%s = %s" % (v.name, e[1]))
        elif k == "asg_ls" and mutable("ls"):
            v, e = self.pick(mutable("ls")), self.e_ls(env)
            self.emit(ind, "%s = %s" % (v.name, e[0]), "%s = %s" % (v.name, e[1]))
        elif k == "print":
            if self.chance(0.5):
                e = self.e_int(env)
                self.emit(ind, "print(%s)" % e[0], "print(%s)" % e[1])
            else:
                e = self.e_str(env)
                self.emit(ind, "print(%s)" % e[0], "print(%s)" % e[1])
        elif k == "app_li" and mutable("li"):
            v, e = self.pick(mutable("li")), self.e_int(env)
            self.emit(ind, "%s.append(%s)" % (v.name, e[0]), "%s.append(%s)" % (v.name, e[1]))
        elif k == "app_ls" and mutable("ls"):
            v, e = self.pick(mutable("ls")), self.e_str(env)
            self.emit(ind, "%s.append(%s)" % (v.name, e[0]), "%s.append(%s)" % (v.name, e[1]))
        elif k == "dset" and mutable("di"):
            v, key, e = self.pick(mutable("di")), self.e_str(env), self.e_int(env)
            self.emit(ind, "%s[%s] = %s" % (v.name, key[0], e[0]), "%s[%s] = %s" % (v.name, key[1], e[1]))
        elif k == "obj_mut" and mutable("obj"):
            v = self.pick(mutable("obj"))
            which = self.pick(["n", "s", "xs"])
            if which == "n":
                e = self.e_int(env)
                self.emit(ind, "%s.n = %s" % (v.name, e[0]), "%s.n = %s" % (v.name, e[1]))
            elif which == "s":
                e = self.e_str(env)
                self.emit(ind, "%s.s = %s" % (v.name, e[0]), "%s.s = %s" % (v.name, e[1]))
            else:
                e = self.e_int(env)
                self.emit(ind, "%s.xs.append(%s)" % (v.name, e[0]), "%s.xs.append(%s)" % (v.name, e[1]))
        elif k == "obj_meth" and mutable("obj"):
            v = self.pick(mutable("obj"))
            e = self.e_int(env)
            self.emit(ind, "%s.bump(%s)" % (v.name, e[0]), "%s.bump(%s)" % (v.name, e[1]))
        elif k == "pop" and mutable("li") and mutable("int"):
            v, t = self.pick(mutable("li")), self.pick(mutable("int"))
            self.emit(ind, "%s = pop_i(%s)" % (t.name, v.name), "%s = pop_i(%s)" % (t.name, v.name))
        elif k == "sort" and mutable("li"):
            v = self.pick(mutable("li"))
            self.emit(ind, "sort_i(%s)" % v.name, "sort_i(%s)" % v.name)
        elif k == "setidx" and mutable("li"):
            v, i, e = self.pick(mutable("li")), self.e_int(env), self.e_int(env)
            self.emit(ind, "set_i(%s, %s, %s)" % (v.name, i[0], e[0]), "set_i(%s, %s, %s)" % (v.name, i[1], e[1]))
        elif k == "extend" and mutable("li"):
            v, e = self.pick(mutable("li")), self.e_li(env)
            self.emit(ind, "ext_i(%s, cp_li(%s))" % (v.name, e[0]), "ext_i(%s, cp_li(%s))" % (v.name, e[1]))
        elif k == "obj_new" and mutable("obj"):
            v, e = self.pick(mutable("obj")), self.cpv("obj", self.e_obj(env))
            self.emit(ind, "%s = %s" % (v.name, e[0]), "%s = %s" % (v.name, e[1]))
        elif k == "dalias_clone" and mutable("di"):
            v, e = self.pick(mutable("di")), self.e_di(env)
            self.emit(ind, "%s = %s" % (v.name, e[0]), "%s = %s" % (v.name, e[1]))
        elif k == "str_cat" and mutable("str"):
            v, e = self.pick(mutable("str")), self.e_str(env)
            self.emit(ind, "%s = %s + %s" % (v.name, v.name, e[0]), "%s = %s + %s" % (v.name, v.name, e[1]))
        elif k == "try" and mutable("int") and mutable("str"):
            t, m = self.pick(mutable("int")), self.pick(mutable("str"))
            a = self.e_int(env)
            self.emit(ind, "mut r%d: Result[int, str] = risky(%s)" % (self.n_tmp + 1, a[0]),
                      "try:\n%s    r%d = risky(%s)\n%sexcept Exception as ex:\n%s    r%d = None\n%s    %s = str(ex)" % (
                          "    " * ind, self.n_tmp + 1, a[1], "    " * ind, "    " * ind, self.n_tmp + 1, "    " * ind, m.name))
            # Tauraro side of the result handling
            rn = "r%d" % (self.n_tmp + 1)
            self.n_tmp += 1
            self.tau.append("    " * ind + "if %s.is_ok:" % rn)
            self.tau.append("    " * (ind + 1) + "%s = %s.ok" % (t.name, rn))
            self.tau.append("    " * ind + "else:")
            self.tau.append("    " * (ind + 1) + "%s = %s.err" % (m.name, rn))
            self.py.append("    " * ind + "if %s is not None:" % rn)
            self.py.append("    " * (ind + 1) + "%s = %s" % (t.name, rn))
        elif k == "if":
            c = self.e_cond(env)
            self.emit(ind, "if %s:" % c[0], "if %s:" % c[1])
            self.block(ind + 1, env, self.rnd(1, 3), depth + 1, frozen, in_loop)
            if self.chance(0.5):
                self.emit(ind, "else:")
                self.block(ind + 1, env, self.rnd(1, 3), depth + 1, frozen, in_loop)
        elif k == "while":
            c = self.fresh("w")
            self.emit(ind, "mut %s = 0" % c, "%s = 0" % c)
            self.emit(ind, "while %s < %d:" % (c, self.rnd(1, 5)))
            self.emit(ind + 1, "%s = %s + 1" % (c, c))
            self.block(ind + 1, env, self.rnd(1, 3), depth + 1, frozen, True)
        elif k == "for_li" and [v for v in env if v.ty == "li"]:
            xs = self.pick([v for v in env if v.ty == "li"])
            x = self.fresh("x")
            acc = self.pick(mutable("int")) if mutable("int") else None
            self.emit(ind, "for %s in %s:" % (x, xs.name))
            env2 = env + [Var(x, "int")]
            if acc is not None:
                self.emit(ind + 1, "%s = (%s + %s) %% %d" % (acc.name, acc.name, x, M))
            self.block(ind + 1, env2, self.rnd(1, 2), depth + 1, frozen | {xs.name, x}, True)
        elif k == "for_ls" and [v for v in env if v.ty == "ls"]:
            xs = self.pick([v for v in env if v.ty == "ls"])
            x = self.fresh("x")
            acc = self.pick(mutable("str")) if mutable("str") else None
            self.emit(ind, "for %s in %s:" % (x, xs.name))
            env2 = env + [Var(x, "str")]
            if acc is not None:
                self.emit(ind + 1, "%s = %s + %s" % (acc.name, acc.name, x))
            self.block(ind + 1, env2, self.rnd(1, 2), depth + 1, frozen | {xs.name, x}, True)
        elif k == "for_range":
            x = self.fresh("i")
            n = self.rnd(0, 5)
            self.emit(ind, "for %s in range(%d):" % (x, n))
            env2 = env + [Var(x, "int")]
            self.block(ind + 1, env2, self.rnd(1, 3), depth + 1, frozen | {x}, True)
        elif k == "brk" and in_loop:
            c = self.e_cond(env)
            self.emit(ind, "if %s:" % c[0], "if %s:" % c[1])
            self.emit(ind + 1, "break")
        elif k == "cnt" and in_loop:
            c = self.e_cond(env)
            self.emit(ind, "if %s:" % c[0], "if %s:" % c[1])
            self.emit(ind + 1, "continue")
        else:
            e = self.e_int(env)
            self.emit(ind, "print(%s)" % e[0], "print(%s)" % e[1])

    # ----------------------------------------------------------------- program
    def declare(self, ind, env_vars):
        for v in env_vars:
            a, b = self.rnd(0, 9), self.rnd(0, 9)
            init = {"int": (str(self.rnd(0, 30)),) * 2,
                    "str": self.lit_str(),
                    "li": ("mk_li2(%d, %d)" % (a, b), "[%d, %d]" % (a, b)),
                    "ls": ("mk_ls1(\"k\")", '["k"]'),
                    "di": ("new_di()", "{}"),
                    "obj": ("P.make(%d, \"o\")" % a, "P.make(%d, \"o\")" % a)}[v.ty]
            tty = {"int": "", "str": "", "li": ": List[int]", "ls": ": List[str]", "di": ": Dict[str, int]", "obj": ""}[v.ty]
            self.tau.append("    " * ind + "mut %s%s = %s" % (v.name, tty, init[0]))
            self.py.append("    " * ind + "%s = %s" % (v.name, init[1]))

    def func(self, idx):
        ret = self.pick(["int", "int", "str", "li", "ls", "obj"])
        nparams = self.rnd(0, 3)
        ptys = [self.pick(["int", "str", "li", "obj", "ls"]) for _ in range(nparams)]
        name = "f%d" % idx
        tt = {"int": "int", "str": "str", "li": "List[int]", "ls": "List[str]", "obj": "P", "di": "Dict[str, int]"}
        params = []
        env = []
        for i, t in enumerate(ptys):
            pn = "a%d" % i
            params.append((pn, t))
            env.append(Var(pn, t))
        self.tau.append("def %s(%s) -> %s:" % (name, ", ".join("%s: %s" % (p, tt[t]) for p, t in params), tt[ret]))
        self.py.append("def %s(%s):" % (name, ", ".join(p for p, _ in params)))
        # locals
        locs = [Var("l%d" % i, self.pick(["int", "str", "li", "ls", "obj", "di"])) for i in range(self.rnd(1, 4))]
        # params that are containers are treated as read-only inputs (frozen) to avoid cross-call aliasing differences
        frozen = {p for p, t in params}
        self.declare(1, locs)
        env2 = env + locs
        self.block(1, env2, self.rnd(1, 5), 1, frozen, False)
        # return a fresh value of the declared type (never a frozen param by identity)
        rv = self.expr(ret, env2, 1)
        self.tau.append("    return %s" % rv[0])
        self.py.append("    return %s" % rv[1])
        self.tau.append("")
        self.py.append("")
        self.fns.append((name, ptys, ret))

    def build(self):
        body_tau, body_py = [], []
        for i in range(self.rnd(1, 4)):
            self.func(i)
        # main
        vars_ = [Var("v0", "int"), Var("v1", "int"), Var("s0", "str"), Var("s1", "str"), Var("L0", "li"),
                 Var("L1", "li"), Var("S0", "ls"), Var("D0", "di"), Var("o0", "obj"), Var("o1", "obj")]
        self.tau.append("def work(round: int):")
        self.py.append("def work(round):")
        self.declare(1, vars_)
        self.block(1, vars_, self.size, 0, set(), False)
        self.tau.append("    print(dump(v0, v1, s0, s1, L0, L1, S0, D0, o0, o1))")
        self.py.append("    print(dump(v0, v1, s0, s1, L0, L1, S0, D0, o0, o1))")
        self.tau.append("")
        self.py.append("")
        self.tau.append("def main():")
        self.py.append("def main():")
        self.tau.append("    work(0)")
        self.tau.append("    work(1)")
        self.py.append("    work(0)")
        self.py.append("    work(1)")
        self.py.append("")
        self.py.append("main()")
        return "\n".join(self.tau) + "\n", "\n".join(self.py) + "\n"


TAU_PRELUDE = '''class P:
    pub n: int
    pub s: str
    pub xs: List[int]

extend P:
    pub def make(n: int, s: str) -> P:
        mut p = P()
        p.n = n
        p.s = s
        p.xs = []
        return p
    pub def bump(self, k: int):
        self.n = (self.n + k) % 1009
        self.xs.append(k)
    pub def tag(self) -> str:
        return self.s + "#" + str(self.n)

def cp_obj(o: P) -> P:
    mut q = P.make(o.n, o.s)
    for x in o.xs:
        q.xs.append(x)
    return q

def b2i(b: bool) -> int:
    if b: return 1
    return 0

def absd(a: int, b: int) -> int:
    if a >= b: return a - b
    return b - a

def fmt2(i: int, s: str) -> str:
    return f"<{i}|{s}>"

def at_i(xs: List[int], i: int) -> int:
    if len(xs) == 0: return 0
    return xs[i % len(xs)]

def at_s(xs: List[str], i: int) -> str:
    if len(xs) == 0: return ""
    return xs[i % len(xs)]

def chr_at(s: str, i: int) -> int:
    if s.len() == 0: return 0
    return s.char_at(i % s.len())

def sub(s: str, a: int, b: int) -> str:
    mut n = s.len()
    mut x = a % (n + 1)
    mut y = b % (n + 1)
    if x > y: return ""
    return s.slice(x, y)

def rep(s: str, n: int) -> str:
    mut out = ""
    mut i = 0
    while i < (n % 4):
        out = out + s
        i = i + 1
    return out

def sum_i(xs: List[int]) -> int:
    mut t = 0
    for x in xs:
        t = (t + x) % 1009
    return t

def pop_i(xs: List[int]) -> int:
    if len(xs) == 0: return 0
    return xs.pop()

def sort_i(xs: List[int]):
    xs.sort()

def set_i(xs: List[int], i: int, v: int):
    if len(xs) == 0: return
    xs[i % len(xs)] = v

def ext_i(xs: List[int], ys: List[int]):
    mut tmp: List[int] = []
    for y in ys:
        tmp.append(y)
    for t in tmp:
        xs.append(t)

def mk_li0() -> List[int]:
    mut xs: List[int] = []
    return xs
def mk_li1(a: int) -> List[int]:
    mut xs: List[int] = []
    xs.append(a)
    return xs
def mk_li2(a: int, b: int) -> List[int]:
    mut xs: List[int] = []
    xs.append(a)
    xs.append(b)
    return xs
def mk_li3(a: int, b: int, c: int) -> List[int]:
    mut xs: List[int] = []
    xs.append(a)
    xs.append(b)
    xs.append(c)
    return xs
def mk_li4(a: int, b: int, c: int, d: int) -> List[int]:
    mut xs: List[int] = []
    xs.append(a)
    xs.append(b)
    xs.append(c)
    xs.append(d)
    return xs

def mk_ls0() -> List[str]:
    mut xs: List[str] = []
    return xs
def mk_ls1(a: str) -> List[str]:
    mut xs: List[str] = []
    xs.append(a)
    return xs
def mk_ls2(a: str, b: str) -> List[str]:
    mut xs: List[str] = []
    xs.append(a)
    xs.append(b)
    return xs
def mk_ls3(a: str, b: str, c: str) -> List[str]:
    mut xs: List[str] = []
    xs.append(a)
    xs.append(b)
    xs.append(c)
    return xs

def cp_li(xs: List[int]) -> List[int]:
    mut out: List[int] = []
    for x in xs:
        out.append(x)
    return out

def cp_ls(xs: List[str]) -> List[str]:
    mut out: List[str] = []
    for x in xs:
        out.append(x)
    return out

def sl_li(xs: List[int], a: int, b: int) -> List[int]:
    mut out: List[int] = []
    mut n = len(xs)
    mut x = a % (n + 1)
    mut y = b % (n + 1)
    mut i = x
    while i < y:
        out.append(xs[i])
        i = i + 1
    return out

def cat_li(a: List[int], b: List[int]) -> List[int]:
    mut out: List[int] = []
    for x in a:
        out.append(x)
    for y in b:
        out.append(y)
    return out

def mapadd(xs: List[int], k: int) -> List[int]:
    mut out: List[int] = []
    for x in xs:
        out.append((x + k) % 1009)
    return out

def split_lens(s: str) -> List[int]:
    mut out: List[int] = []
    for part in s.split_exact(","):
        out.append(part.len())
    return out

def split_c(s: str) -> List[str]:
    mut out: List[str] = []
    for part in s.split_exact(","):
        out.append(part)
    return out

def range_li(n: int) -> List[int]:
    mut out: List[int] = []
    mut i = 0
    while i < (n % 6):
        out.append(i * 3)
        i = i + 1
    return out

def mapcat(xs: List[str], s: str) -> List[str]:
    mut out: List[str] = []
    for x in xs:
        out.append(x + s)
    return out

def join_s(xs: List[str]) -> str:
    mut out = ""
    mut first = true
    for x in xs:
        if not first: out = out + "|"
        out = out + x
        first = false
    return out

def new_di() -> Dict[str, int]:
    mut d: Dict[str, int] = {}
    return d

def dget(d: Dict[str, int], k: str) -> int:
    if d.has(k): return d.get(k)
    return 0

def dkeys(d: Dict[str, int]) -> List[str]:
    mut ks: List[str] = []
    for k in d.keys():
        ks.append(k)
    ks.sort()
    return ks

def cp_di(d: Dict[str, int]) -> Dict[str, int]:
    mut out: Dict[str, int] = {}
    for k in d.keys():
        out[k] = d.get(k)
    return out

def risky(x: int) throws str -> int:
    if x % 7 == 0: raise("bad" + str(x))
    return (x * 3 + 1) % 1009

def show_li(xs: List[int]) -> str:
    mut out = "["
    mut first = true
    for x in xs:
        if not first: out = out + ","
        out = out + str(x)
        first = false
    return out + "]"

def show_ls(xs: List[str]) -> str:
    mut out = "["
    mut first = true
    for x in xs:
        if not first: out = out + ","
        out = out + x
        first = false
    return out + "]"

def show_di(d: Dict[str, int]) -> str:
    mut out = "{"
    for k in dkeys(d):
        out = out + k + ":" + str(d.get(k)) + ";"
    return out + "}"

def show_obj(o: P) -> str:
    return "P(" + str(o.n) + "," + o.s + "," + show_li(o.xs) + ")"

def dump(v0: int, v1: int, s0: str, s1: str, L0: List[int], L1: List[int], S0: List[str], D0: Dict[str, int], o0: P, o1: P) -> str:
    return str(v0) + " " + str(v1) + " " + s0 + " " + s1 + " " + show_li(L0) + " " + show_li(L1) + " " + show_ls(S0) + " " + show_di(D0) + " " + show_obj(o0) + " " + show_obj(o1)

'''

PY_PRELUDE = '''class P:
    def __init__(self, n, s):
        self.n = n
        self.s = s
        self.xs = []
    @staticmethod
    def make(n, s):
        return P(n, s)
    def bump(self, k):
        self.n = (self.n + k) % 1009
        self.xs.append(k)
    def tag(self):
        return self.s + "#" + str(self.n)

def cp_obj(o):
    q = P.make(o.n, o.s)
    q.xs = list(o.xs)
    return q
def b2i(b):
    return 1 if b else 0
def absd(a, b):
    return a - b if a >= b else b - a
def fmt2(i, s):
    return "<%d|%s>" % (i, s)
def at_i(xs, i):
    return 0 if len(xs) == 0 else xs[i % len(xs)]
def at_s(xs, i):
    return "" if len(xs) == 0 else xs[i % len(xs)]
def chr_at(s, i):
    return 0 if len(s) == 0 else ord(s[i % len(s)])
def sub(s, a, b):
    n = len(s)
    x = a % (n + 1)
    y = b % (n + 1)
    if x > y:
        return ""
    return s[x:y]
def rep(s, n):
    return s * (n % 4)
def sum_i(xs):
    t = 0
    for x in xs:
        t = (t + x) % 1009
    return t
def pop_i(xs):
    return 0 if len(xs) == 0 else xs.pop()
def sort_i(xs):
    xs.sort()
def set_i(xs, i, v):
    if len(xs) == 0:
        return
    xs[i % len(xs)] = v
def ext_i(xs, ys):
    tmp = list(ys)
    for t in tmp:
        xs.append(t)
def mk_li0(): return []
def mk_li1(a): return [a]
def mk_li2(a, b): return [a, b]
def mk_li3(a, b, c): return [a, b, c]
def mk_li4(a, b, c, d): return [a, b, c, d]
def mk_ls0(): return []
def mk_ls1(a): return [a]
def mk_ls2(a, b): return [a, b]
def mk_ls3(a, b, c): return [a, b, c]
def cp_li(xs): return list(xs)
def cp_ls(xs): return list(xs)
def sl_li(xs, a, b):
    n = len(xs)
    x = a % (n + 1)
    y = b % (n + 1)
    return xs[x:y] if x < y else []
def cat_li(a, b): return list(a) + list(b)
def mapadd(xs, k): return [(x + k) % 1009 for x in xs]
def split_lens(s): return [len(p) for p in s.split(",")]
def split_c(s): return list(s.split(","))
def range_li(n): return [i * 3 for i in range(n % 6)]
def mapcat(xs, s): return [x + s for x in xs]
def join_s(xs): return "|".join(xs)
def new_di(): return {}
def dget(d, k): return d[k] if k in d else 0
def dkeys(d): return sorted(d.keys())
def cp_di(d): return dict(d)
class RiskyErr(Exception):
    pass
def risky(x):
    if x % 7 == 0:
        raise RiskyErr("bad" + str(x))
    return (x * 3 + 1) % 1009
def show_li(xs): return "[" + ",".join(str(x) for x in xs) + "]"
def show_ls(xs): return "[" + ",".join(xs) + "]"
def show_di(d): return "{" + "".join("%s:%d;" % (k, d[k]) for k in sorted(d.keys())) + "}"
def show_obj(o): return "P(%d,%s,%s)" % (o.n, o.s, show_li(o.xs))
def dump(v0, v1, s0, s1, L0, L1, S0, D0, o0, o1):
    return "%d %d %s %s %s %s %s %s %s %s" % (v0, v1, s0, s1, show_li(L0), show_li(L1), show_ls(S0), show_di(D0), show_obj(o0), show_obj(o1))

'''


def main():
    seed = int(sys.argv[1])
    outdir = sys.argv[2]
    size = int(sys.argv[3]) if len(sys.argv) > 3 else 12
    g = Gen(seed, size)
    tau, py = g.build()
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "p.tr"), "w") as f:
        f.write(TAU_PRELUDE + tau)
    with open(os.path.join(outdir, "p.py"), "w") as f:
        f.write(PY_PRELUDE + py)


if __name__ == "__main__":
    main()
