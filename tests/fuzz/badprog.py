#!/usr/bin/env python3
"""Invalid-program fuzzer:  badprog.py <seed> <count> [jobs]

Takes valid programs from tests/regression + tests/lang, applies ONE random source mutation that
should make the program invalid (undefined name, wrong argument count, wrong literal type, bad
return type, unknown method/field, deleted declaration ...), compiles it, and classifies the result:

  DIAG   a normal Tauraro diagnostic (what we want)
  OK     the mutation was benign / still valid
  ICE    the C compiler rejected generated C ("[ICE]" or raw "error: compiling")   <- compiler bug
  CRASH  tauraroc itself crashed or hung                                            <- compiler bug

Each ICE / CRASH is kept under <out>/<class>/<n>/ (mutated source + output).
Env: TAURAROC (compiler path, default ./tauraroc), BAD_OUT (default ./bad_out).
"""
import os, re, sys, random, subprocess, shutil, tempfile, concurrent.futures as cf

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
TAURAROC = os.environ.get("TAURAROC", os.path.join(ROOT, "tauraroc"))
OUT = os.path.abspath(os.environ.get("BAD_OUT", "bad_out"))

IDENT = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
KW = set("def class mut pub if elif else while for in return from import extend enum match case "
         "and or not is true false None self int str float bool void List Dict Set Vec Map Option Result "
         "async await spawn defer unsafe throws raise lambda as try catch const use".split())


def lines_of(src):
    return src.split("\n")


def m_undef_name(ls, r):
    cands = [i for i, l in enumerate(ls) if l.strip() and not l.strip().startswith("#") and "(" in l and not l.lstrip().startswith(("def ", "class "))]
    if not cands: return None
    i = r.choice(cands)
    ids = [m for m in IDENT.finditer(ls[i]) if m.group(0) not in KW]
    if not ids: return None
    m = r.choice(ids)
    ls[i] = ls[i][:m.start()] + m.group(0) + "_zz" + ls[i][m.end():]
    return "undefined/renamed identifier"


def m_wrong_literal(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r"(?<![\w\"'.])\d+(?![\w.\"'])", l) if not l.strip().startswith("#")]
    if not cands: return None
    i, m = r.choice(cands)
    ls[i] = ls[i][:m.start()] + '"str"' + ls[i][m.end():]
    return "int literal -> str"


def m_str_to_int(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r'"[^"\\\n]*"', l) if not l.strip().startswith("#") and "import" not in l]
    if not cands: return None
    i, m = r.choice(cands)
    ls[i] = ls[i][:m.start()] + "42" + ls[i][m.end():]
    return "str literal -> int"


def m_drop_arg(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r"\b(\w+)\(([^()]*,[^()]*)\)", l) if not l.lstrip().startswith(("def ", "class ", "#"))]
    if not cands: return None
    i, m = r.choice(cands)
    args = m.group(2).split(",")
    args.pop(r.randrange(len(args)))
    ls[i] = ls[i][:m.start(2)] + ",".join(args) + ls[i][m.end(2):]
    return "dropped call argument"


def m_extra_arg(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r"\b(\w+)\(([^()]*)\)", l) if not l.lstrip().startswith(("def ", "class ", "#", "if ", "while ")) and m.group(1) not in KW]
    if not cands: return None
    i, m = r.choice(cands)
    sep = ", " if m.group(2).strip() else ""
    ls[i] = ls[i][:m.end(2)] + sep + "1" + ls[i][m.end(2):]
    return "extra call argument"


def m_ret_type(ls, r):
    cands = [i for i, l in enumerate(ls) if re.search(r"\)\s*->\s*(int|str|float|bool)\s*:", l)]
    if not cands: return None
    i = r.choice(cands)
    new = r.choice(["int", "str", "float", "bool", "List[int]", "Foo"])
    ls[i] = re.sub(r"->\s*(int|str|float|bool)\s*:", "-> " + new + ":", ls[i], count=1)
    return "changed return type -> " + new


def m_param_type(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r"(\w+):\s*(int|str|float|bool)\b", l) if l.lstrip().startswith("def ")]
    if not cands: return None
    i, m = r.choice(cands)
    new = r.choice(["int", "str", "float", "bool", "List[int]", "Foo", "Option[int]"])
    ls[i] = ls[i][:m.start(2)] + new + ls[i][m.end(2):]
    return "changed param type -> " + new


def m_method(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r"\.(\w+)\(", l) if not l.lstrip().startswith(("#", "from ", "import "))]
    if not cands: return None
    i, m = r.choice(cands)
    ls[i] = ls[i][:m.start(1)] + m.group(1) + "_zz" + ls[i][m.end(1):]
    return "unknown method"


def m_field(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r"\b(self|\w+)\.(\w+)\b(?!\()", l) if not l.lstrip().startswith(("#", "from ", "import "))]
    if not cands: return None
    i, m = r.choice(cands)
    ls[i] = ls[i][:m.start(2)] + m.group(2) + "_zz" + ls[i][m.end(2):]
    return "unknown field"


def m_delete_line(ls, r):
    cands = [i for i, l in enumerate(ls) if l.strip() and not l.strip().startswith("#") and (l.lstrip().startswith("mut ") or "=" in l)]
    if not cands: return None
    del ls[r.choice(cands)]
    return "deleted a statement"


def m_swap_op(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r" (\+|-|\*|<|>|==) ", l) if not l.lstrip().startswith("#")]
    if not cands: return None
    i, m = r.choice(cands)
    ls[i] = ls[i][:m.start(1)] + r.choice(["and", "%", "is", "in"]) + ls[i][m.end(1):]
    return "swapped operator"


def m_generic_arg(ls, r):
    cands = [(i, m) for i, l in enumerate(ls) for m in re.finditer(r"\b(List|Dict|Set|Option|Vec)\[(\w+)", l)]
    if not cands: return None
    i, m = r.choice(cands)
    ls[i] = ls[i][:m.start(2)] + r.choice(["Foo", "int", "str", "void", "Nope"]) + ls[i][m.end(2):]
    return "changed generic argument"


def m_type_confuse(ls, r):
    cands = [i for i, l in enumerate(ls) if re.match(r"\s*mut \w+(: [\w\[\], ]+)? = ", l)]
    if not cands: return None
    i = r.choice(cands)
    ls[i] = re.sub(r"= .*$", "= " + r.choice(['"s"', "1.5", "true", "[1, 2]", "None", "{}", "1", "(1, 2)"]), ls[i])
    return "initialiser replaced"


MUTS = [m_undef_name, m_wrong_literal, m_str_to_int, m_drop_arg, m_extra_arg, m_ret_type, m_param_type,
        m_method, m_field, m_delete_line, m_swap_op, m_generic_arg, m_type_confuse]


def classify(rc, text):
    if rc in (124,): return "CRASH", "timeout"
    if rc < 0 or rc >= 128 and "error" not in text: return "CRASH", "signal rc=%d" % rc
    if "[ICE]" in text or re.search(r"compiling .*\.c failed|build/\S+\.[ch]:\d+:\d+: error", text): return "ICE", ""
    if "Segmentation fault" in text or "Aborted" in text or "core dumped" in text: return "CRASH", "crash"
    if rc == 0: return "OK", ""
    return "DIAG", ""


def one(task):
    seed, src_path, tmp_root, idx = task
    r = random.Random(seed)
    src = open(src_path, errors="replace").read()
    ls = lines_of(src)
    random.Random(seed ^ 0x5A5A).shuffle(MUTS_LOCAL := list(MUTS))
    desc = None
    for mut in MUTS_LOCAL:
        desc = mut(ls, r)
        if desc: break
    if not desc: return seed, "OK", "no mutation applicable", src_path, None
    d = tempfile.mkdtemp(prefix="bp", dir=tmp_root)   # private working dir: tauraroc writes ./build there
    name = "mut_%d.tr" % seed
    target = os.path.join(d, name)
    try:
        open(target, "w").write("\n".join(ls))
        env = dict(os.environ, TAURARO_SHOW_C_ERRORS="0")
        try:
            p = subprocess.run([TAURAROC, name, "-o", "out"], cwd=d, capture_output=True, text=True, timeout=240, env=env, errors="replace")
            rc, text = p.returncode, p.stdout + p.stderr
        except subprocess.TimeoutExpired:
            rc, text = 124, "TIMEOUT"
        cls, why = classify(rc, text)
        keep = None
        if cls in ("ICE", "CRASH"):
            keep = (open(target).read(), text[:4000])
        return seed, cls, desc + (" | " + why if why else ""), src_path, keep
    finally:
        shutil.rmtree(d, ignore_errors=True)


def main():
    seed0, count = int(sys.argv[1]), int(sys.argv[2])
    jobs = int(sys.argv[3]) if len(sys.argv) > 3 else 3
    files = sorted(os.path.join(ROOT, "tests", d, f) for d in ("regression", "lang") for f in os.listdir(os.path.join(ROOT, "tests", d)) if f.endswith(".tr") and not f.startswith("mut_") and os.path.getsize(os.path.join(ROOT, "tests", d, f)) < 6000)
    tmp_root = tempfile.mkdtemp(prefix="badprog")
    tasks = [(seed0 + k, files[random.Random(seed0 + k).randrange(len(files))], tmp_root, k) for k in range(count)]
    counts = {}
    os.makedirs(OUT, exist_ok=True)
    kept = 0
    with cf.ThreadPoolExecutor(jobs) as ex:
        for seed, cls, desc, path, keep in ex.map(one, tasks):
            counts[cls] = counts.get(cls, 0) + 1
            if keep:
                kept += 1
                dd = os.path.join(OUT, cls, str(seed)); os.makedirs(dd, exist_ok=True)
                open(os.path.join(dd, "prog.tr"), "w").write(keep[0])
                open(os.path.join(dd, "note.txt"), "w").write("%s\nbase: %s\n%s\n\n%s" % (desc, os.path.relpath(path, ROOT), cls, keep[1]))
                print("%s seed=%d %s [%s]" % (cls, seed, desc, os.path.relpath(path, ROOT)), flush=True)
    print("== results:", ", ".join("%s=%d" % kv for kv in sorted(counts.items())))
    shutil.rmtree(tmp_root, ignore_errors=True)


if __name__ == "__main__":
    main()
