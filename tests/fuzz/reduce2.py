#!/usr/bin/env python3
"""Delta-reduce a failing gen2 case:  reduce2.py <seed> [size] [mode]

Regenerates the program, then repeatedly deletes marker-delimited statements (<<id ... >>id)
from BOTH p.tr and p.py while the Tauraro build still disagrees with Python (or crashes).
Prints the minimal Tauraro `work()` body and the first differing output line."""
import os, re, subprocess, sys, tempfile, shutil

HERE = os.path.dirname(os.path.abspath(__file__))
TAURAROC = os.environ.get("TAURAROC", os.path.abspath(os.path.join(HERE, "..", "..", "tauraroc")))


def sh(cmd, cwd, timeout=120, env=None):
    try:
        p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, timeout=timeout, env=env, errors="replace")
        return p.returncode, p.stdout, p.stderr
    except subprocess.TimeoutExpired:
        return 124, "", "TIMEOUT"


def regions(lines):
    stack, out = [], []
    for i, l in enumerate(lines):
        m = re.match(r"\s*# <<(\d+)$", l)
        if m:
            stack.append((m.group(1), i))
        m = re.match(r"\s*# >>(\d+)$", l)
        if m and stack and stack[-1][0] == m.group(1):
            uid, a = stack.pop()
            out.append((uid, a, i))
    return out


def strip_regions(lines, uid_set):
    drop = set()
    for uid, a, b in regions(lines):
        if uid in uid_set:
            drop.update(range(a, b + 1))
    return [l for i, l in enumerate(lines) if i not in drop]


def failing(d, tau, py, mode):
    open(os.path.join(d, "p.tr"), "w").write("\n".join(tau) + "\n")
    open(os.path.join(d, "p.py"), "w").write("\n".join(py) + "\n")
    rc, exp, _ = sh([sys.executable, "p.py"], d, 20)
    if rc != 0:
        return False
    flags = {"default": [], "debug": ["--debug"], "strict": ["--strict"], "O0": ["-O0"], "O3": ["-O3"]}[mode]
    rc, so, se = sh([TAURAROC, "p.tr", "-o", "p_exe"] + flags, d, 240)
    if rc != 0 or not os.path.exists(os.path.join(d, "p_exe")):
        return False
    env = dict(os.environ, ASAN_OPTIONS="detect_leaks=0:halt_on_error=1", UBSAN_OPTIONS="halt_on_error=1")
    rc, so, se = sh(["./p_exe"], d, 20, env)
    os.remove(os.path.join(d, "p_exe"))
    return rc != 0 or so != exp


def main():
    seed = sys.argv[1]
    size = sys.argv[2] if len(sys.argv) > 2 else "10"
    mode = sys.argv[3] if len(sys.argv) > 3 else "default"
    d = tempfile.mkdtemp(prefix="red")
    sh([sys.executable, os.path.join(HERE, "gen2.py"), seed, d, size], d, 30)
    tau = open(os.path.join(d, "p.tr")).read().split("\n")
    py = open(os.path.join(d, "p.py")).read().split("\n")
    if not failing(d, tau, py, mode):
        print("seed does not fail in mode", mode)
        return
    uids = [u for u, a, b in sorted(regions(tau), key=lambda r: -(r[2] - r[1]))]
    changed = True
    while changed:
        changed = False
        for u in list(uids):
            t2, p2 = strip_regions(tau, {u}), strip_regions(py, {u})
            if failing(d, t2, p2, mode):
                tau, py, changed = t2, p2, True
                uids = [x for x in uids if x != u]
    open(os.path.join(d, "p.tr"), "w").write("\n".join(tau) + "\n")
    open(os.path.join(d, "p.py"), "w").write("\n".join(py) + "\n")
    # print minimal work body
    i = next(k for k, l in enumerate(tau) if l.startswith("def work"))
    j = next(k for k, l in enumerate(tau) if l.startswith("def main"))
    print("\n".join(l for l in tau[i:j] if not re.match(r"\s*# [<>]{2}\d+$", l)))
    rc, exp, _ = sh([sys.executable, "p.py"], d, 20)
    flags = {"default": [], "debug": ["--debug"], "strict": ["--strict"], "O0": ["-O0"], "O3": ["-O3"]}[mode]
    sh([TAURAROC, "p.tr", "-o", "p_exe"] + flags, d, 240)
    rc, so, se = sh(["./p_exe"], d, 20)
    print("--- expected:", exp.strip()[:300])
    print("--- got (rc=%d):" % rc, (so or se).strip()[:300])
    print("(kept in %s)" % d)


if __name__ == "__main__":
    main()
