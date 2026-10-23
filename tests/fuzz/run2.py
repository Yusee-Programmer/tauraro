#!/usr/bin/env python3
"""Differential fuzz driver:  run2.py <first_seed> <count> [size] [jobs]

For each seed: generate (gen2.py), run the Python oracle, compile + run the Tauraro
program in several modes, and classify any discrepancy:
  GENFAIL   the Python oracle itself failed (generator bug)
  CFAIL     tauraroc rejected / C compile failed       (compiler bug, or generator emitted invalid Tauraro)
  CRASH     the Tauraro binary crashed / timed out / ASan-UBSan report
  MISMATCH  stdout differs from the Python oracle     (miscompile)
  MODEDIFF  default build and --debug (ASan) build disagree
Failing cases are kept under <out>/<class>/<seed>/ together with a note.txt.
"""
import os, sys, subprocess, shutil, tempfile, concurrent.futures as cf, hashlib

ROOT = os.path.dirname(os.path.abspath(__file__))
GEN = os.path.join(ROOT, os.environ.get("FUZZ_GEN", "gen2.py"))   # gen2.py (programs) or gen3.py (expressions)
TAURAROC = os.environ.get("TAURAROC", os.path.abspath(os.path.join(ROOT, "..", "..", "tauraroc")))
OUT = os.environ.get("FUZZ_OUT", os.path.abspath("fuzz_out"))
MODES = os.environ.get("FUZZ_MODES", "default,debug").split(",")


def run(cmd, cwd, timeout, env=None):
    try:
        p = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, timeout=timeout, env=env, errors="replace")
        return p.returncode, p.stdout, p.stderr
    except subprocess.TimeoutExpired:
        return 124, "", "TIMEOUT"


def one(seed, size):
    d = tempfile.mkdtemp(prefix="fz%d_" % seed)
    try:
        rc, _, err = run([sys.executable, GEN, str(seed), d, str(size)], d, 30)
        if rc != 0:
            return seed, "GENFAIL", "generator: " + err[-300:], d
        rc, exp, err = run([sys.executable, "p.py"], d, 20)
        if rc != 0:
            return seed, "GENFAIL", "python: " + err[-300:], d
        outs = {}
        for mode in MODES:
            flags = {"default": [], "debug": ["--debug"], "strict": ["--strict"], "O0": ["-O0"], "O3": ["-O3"]}[mode]
            exe = "p_" + mode
            rc, so, se = run([TAURAROC, "p.tr", "-o", exe] + flags, d, 240)
            if rc != 0 or not os.path.exists(os.path.join(d, exe)):
                msg = "\n".join(l for l in (so + se).splitlines() if "error" in l)[:600]
                return seed, "CFAIL", "[%s] %s" % (mode, msg), d
            env = dict(os.environ, ASAN_OPTIONS="detect_leaks=0:abort_on_error=0:halt_on_error=1",
                       UBSAN_OPTIONS="halt_on_error=1:print_stacktrace=1")
            rc, so, se = run(["./" + exe], d, 20, env)
            if rc != 0:
                return seed, "CRASH", "[%s] rc=%d %s" % (mode, rc, (se or so)[-700:]), d
            outs[mode] = so
            if so != exp:
                a, b = exp.splitlines(), so.splitlines()
                first = next((i for i in range(min(len(a), len(b))) if a[i] != b[i]), min(len(a), len(b)))
                return seed, "MISMATCH", "[%s] line %d\n  want: %s\n  got:  %s" % (
                    mode, first + 1, a[first] if first < len(a) else "<eof>", b[first] if first < len(b) else "<eof>"), d
        return seed, "OK", "", d
    except Exception as e:
        return seed, "GENFAIL", "driver: %r" % e, d


def main():
    first, count = int(sys.argv[1]), int(sys.argv[2])
    size = int(sys.argv[3]) if len(sys.argv) > 3 else 12
    jobs = int(sys.argv[4]) if len(sys.argv) > 4 else (os.cpu_count() or 4)
    os.makedirs(OUT, exist_ok=True)
    stats = {}
    sigs = {}
    with cf.ThreadPoolExecutor(jobs) as ex:
        for seed, cls, note, d in ex.map(lambda s: one(s, size), range(first, first + count)):
            stats[cls] = stats.get(cls, 0) + 1
            if cls != "OK":
                dest = os.path.join(OUT, cls, str(seed))
                shutil.rmtree(dest, ignore_errors=True)
                os.makedirs(os.path.dirname(dest), exist_ok=True)
                shutil.copytree(d, dest, ignore=shutil.ignore_patterns("build", "p_*"))
                open(os.path.join(dest, "note.txt"), "w").write(note + "\n")
                sig = hashlib.md5(("%s|%s" % (cls, note.splitlines()[0][:120] if note else "")).encode()).hexdigest()[:6]
                sigs.setdefault((cls, note.splitlines()[0][:110] if note else ""), []).append(seed)
            shutil.rmtree(d, ignore_errors=True)
    print("== results:", ", ".join("%s=%d" % kv for kv in sorted(stats.items())))
    for (cls, line), seeds in sorted(sigs.items(), key=lambda kv: -len(kv[1])):
        print("  %-8s x%-3d seeds %s  %s" % (cls, len(seeds), seeds[:4], line))


if __name__ == "__main__":
    main()
