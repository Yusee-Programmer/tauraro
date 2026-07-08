#!/usr/bin/env python3
# Ownership fuzzer for the Tauraro compiler.
#
# Emits (to stdout) a random-but-always-valid Tauraro program that exercises the
# drop / escape / borrow analysis — the machinery behind is_droppable_sym and the
# Phase-C borrow check that has historically had special-casing holes (the
# class-with-free double-free and the Mutex.get() use-after-free were both gaps
# here). Every generated program:
#
#   * is deterministic  -> prints "CHK <int>" (a checksum of the work it did),
#   * is leak-measurable -> prints "LIVE <int>" (net heap allocs after a warm-up),
#   * is UB-free by construction -> so ANY of these is a COMPILER bug:
#       - elided output != pure-ARC (--no-elide) output   (unsound elision)
#       - LIVE > 0                                          (leak)
#       - a crash / ASan error                             (double-free / UAF)
#
# The harness scripts/fuzz_check.sh drives it (differential + memcount + ASan).
#
# Usage: python3 gen.py <seed>
import sys, random, os

PRELUDE = '''extern "C":
    def _tr_c_free(ptr: Pointer[char])
    def _tr_mem_live() -> int

from std.core.map import Map
from std.async.mutex import Mutex

# Plain heap class (non-atomic ARC) — used in collections / Mutex / Shared.
class Box:
    pub v: int
extend Box:
    pub def init(v: int) -> Box:
        mut b = Box()
        b.v = v
        return b
    pub def get(self) -> int:
        return self.v

# Class WITH a user free() — MANUALLY managed (not refcount-balanced). Passing it
# to a call that frees it must suppress the caller's auto-drop (coll_escaped), else
# double-free (the exact JsonWriter bug). Int-only so free() is a COMPLETE release
# (no fields), keeping the baseline leak-free — the check under test is the STRUCT
# not being freed twice.
class Named:
    pub n: int
extend Named:
    pub def init(n: int) -> Named:
        mut x = Named()
        x.n = n
        return x
    pub def score(self) -> int:
        return self.n
    pub def free(self):
        unsafe: _tr_c_free(self as Pointer[char])

# Pure-ARC class (no user free()) — safe to own as a field / store in collections.
class Leaf:
    pub s: str
    pub n: int
extend Leaf:
    pub def init(s: str, n: int) -> Leaf:
        mut l = Leaf()
        l.s = s
        l.n = n
        return l

# Nested class field: Holder OWNS a Leaf -> its drop must recurse into it.
class Holder:
    pub tag: Leaf
    pub k:   int
extend Holder:
    pub def init(tag: Leaf, k: int) -> Holder:
        mut h = Holder()
        h.tag = tag
        h.k   = k
        return h

def make_leaf(k: int) -> Leaf:
    return Leaf.init("l" + k.to_str(), k)

# Factory: returns an OWNED Named (interproc return-ownership inference must
# reclaim the result at call sites that don't keep it).
def make_named(k: int) -> Named:
    return Named.init(k)

# Borrows its argument (reads only) — caller keeps ownership.
def use_named(x: Named) -> int:
    return x.score()

# CONSUMES its argument by calling free() on it — the double-free trigger: the
# caller must NOT also auto-drop x.
def consume_named(x: Named):
    mut _s = x.score()
    x.free()
'''

# Each fragment: returns Tauraro lines for the body of workload(); every fragment
# adds a deterministic amount to `acc` for the given loop counter `k`.
def f_owned_use(k):        # owned class-with-free local, borrowed then auto-dropped
    return [f'    mut a{k} = make_named({k})',
            f'    acc = acc + use_named(a{k})']

def f_consume(k):          # pass class-with-free to a freeing method  (double-free class)
    return [f'    mut b{k} = make_named({k})',
            f'    consume_named(b{k})',
            f'    acc = acc + {k}']

def f_vec_box(k):          # Vec of plain heap class (collection element ownership)
    return [f'    mut v{k} = Vec[Box].init(3)',
            f'    v{k}.push(Box.init({k}))',
            f'    v{k}.push(Box.init({k}+1))',
            f'    acc = acc + v{k}.get(0).get() + v{k}.get(1).get()']

def f_mutex_get(k):        # Mutex[Box].get() borrow, reused across the loop  (UAF class)
    return [f'    mut m{k} = Mutex[Box].init(Box.init({k}))',
            f'    mut g{k} = m{k}.get()',
            f'    acc = acc + g{k}.v',
            f'    m{k}.unlock()']

def f_mutex_map(k):        # Mutex[Map[str,Box]].get() insert+get  (the exact watax pattern)
    return [f'    mut mm{k} = Mutex[Map[str, Box]].init(Map[str, Box].init(8))',
            f'    mut t{k} = mm{k}.get()',
            f'    t{k}.insert("k", Box.init({k}))',
            f'    acc = acc + t{k}.get("k").v',
            f'    mm{k}.unlock()']

def f_nested(k):           # Holder{Leaf} nested owned field (pure ARC, recursive drop)
    return [f'    mut h{k} = Holder.init(make_leaf({k}), {k})',
            f'    acc = acc + h{k}.tag.n + h{k}.k']

def f_shared(k):           # shared wrap of an existing value + clone (atomic ARC)
    return [f'    shared s{k} = Box.init({k})',
            f'    acc = acc + s{k}.get()']

# CORE = patterns that are leak-free / sound on the current compiler; this is the
# default set the CI gate fuzzes, so any failure is a REGRESSION. HARD adds patterns
# that currently expose open bugs the fuzzer found (see tests/fuzz/FINDINGS.md) —
# enable with FUZZ_HARD=1 to hunt/track them without reddening the regression gate.
CORE = [f_consume, f_nested, f_shared]
HARD = [f_owned_use, f_vec_box, f_mutex_get, f_mutex_map]

def main():
    seed = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    frags = CORE + HARD if os.environ.get("FUZZ_HARD") == "1" else CORE
    only = os.environ.get("FUZZ_ONLY")           # e.g. FUZZ_ONLY=f_vec_box -> only that fragment
    rng = random.Random(seed)
    if only:
        frags = [globals()[only]]
    n = rng.randint(3, 9)
    picks = [rng.choice(frags) for _ in range(n)]

    body = []
    for i, frag in enumerate(picks):
        body += frag(i + 1)     # k = 1..n (distinct var names, non-zero values)

    out = []
    out.append(PRELUDE)
    out.append("def workload() -> int:")
    out.append("    mut acc = 0")
    out += body
    out.append("    return acc")
    out.append("")
    out.append("def main():")
    out.append("    mut warm = workload()")
    out.append("    mut before = _tr_mem_live()")
    out.append("    mut acc = 0")
    out.append("    mut i = 0")
    out.append("    while i < 200:")
    out.append("        acc = acc + workload()")
    out.append("        i = i + 1")
    out.append("    mut leaked = _tr_mem_live() - before")
    out.append('    print("CHK " + acc.to_str())')
    out.append('    print("LIVE " + leaked.to_str())')
    print("\n".join(out))

if __name__ == "__main__":
    main()
