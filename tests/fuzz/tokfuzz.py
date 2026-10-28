#!/usr/bin/env python3
"""Token-level mutation fuzzer:  tokfuzz.py <seed> <count> [jobs]

Tokenises valid programs from tests/regression + tests/lang and applies 1-3 token/byte level
mutations (delete / duplicate / swap / replace tokens, unbalanced brackets, bad indentation, stray
characters, unterminated strings and comments, huge nesting, empty file, non-UTF8 bytes, NULs,
tabs, CRLF, very long lines/identifiers), then compiles and classifies:

  DIAG   normal Tauraro diagnostic ("error: [CODE] ...") and a non-zero exit        (good)
  OK     still a valid program (exit 0)
  ICE    "[ICE]" / "[C-1]" / raw C compiler text / gcc warning text leaked           (bug)
  NODIAG non-zero exit without any "error: [CODE]" diagnostic                       (bug)
  CRASH  signal, timeout or sanitizer report                                        (bug)

Failures are kept under <out>/<class>/<seed>/{prog.tr,note.txt}.
Env: TAURAROC (default ./tauraroc), TOK_OUT (default ./tok_out), TOK_TIMEOUT (seconds, default 60).
"""
import os, re, sys, random, subprocess, shutil, tempfile, concurrent.futures as cf

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
TAURAROC = os.environ.get("TAURAROC", os.path.join(ROOT, "tauraroc"))
OUT = os.path.abspath(os.environ.get("TOK_OUT", "tok_out"))
TIMEOUT = int(os.environ.get("TOK_TIMEOUT", "60"))

TOK = re.compile(rb'"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'|#[^\n]*|[A-Za-z_][A-Za-z0-9_]*|\d+(?:\.\d+)?|\n[ \t]*|[ \t]+|->|==|!=|<=|>=|\*\*|//|::|<<|>>|[+\-*/%&|^]=|.', re.S)

STRAY = [b"@", b"$", b"`", b"\\", b"?", b"::", b"!", b"~", b";", b"\x00", b"\xff", b"\xc3", b"\xe2\x82", b"\r", b"\t",
         b"'", b'"', b'"""', b"#", b"{", b"}", b"(", b")", b"[", b"]", b"->", b"=>", b"...", b"0x", b"1e", b"1_", b"0b2", b"\xef\xbb\xbf",
         b"def", b"class", b"match", b"case", b"unsafe", b"async", b"await", b"defer", b"spawn", b"extern", b"enum", b"try", b"catch",
         b"raise", b"throws", b"import", b"from", b"const", b"static", b"macro", b"lambda", b"in", b"is", b"not", b"as", b"mut", b"return"]


def toks(src):
    return TOK.findall(src)


def m_delete(t, r):
    if not t: return None
    del t[r.randrange(len(t))]; return "delete token"

def m_dup(t, r):
    if not t: return None
    i = r.randrange(len(t)); t.insert(i, t[i]); return "duplicate token"

def m_swap(t, r):
    if len(t) < 2: return None
    i = r.randrange(len(t) - 1); t[i], t[i + 1] = t[i + 1], t[i]; return "swap tokens"

def m_replace(t, r):
    if not t: return None
    t[r.randrange(len(t))] = r.choice(STRAY); return "replace token"

def m_insert(t, r):
    t.insert(r.randrange(len(t) + 1), b" " + r.choice(STRAY) + b" "); return "insert stray"

def m_unbalance(t, r):
    idx = [i for i, x in enumerate(t) if x in (b"(", b")", b"[", b"]", b"{", b"}")]
    if not idx: return None
    i = r.choice(idx)
    if r.random() < 0.5: del t[i]
    else: t[i] = r.choice([b"(", b")", b"[", b"]", b"{", b"}"])
    return "unbalance brackets"

def m_unterminated(t, r):
    idx = [i for i, x in enumerate(t) if x[:1] in (b'"', b"'")]
    if not idx: return None
    i = r.choice(idx); t[i] = t[i][:-1] if r.random() < 0.7 else t[i][:1]; return "unterminated string"

def m_indent(t, r):
    idx = [i for i, x in enumerate(t) if x[:1] == b"\n"]
    if not idx: return None
    i = r.choice(idx); t[i] = b"\n" + r.choice([b" ", b"  ", b"   ", b"\t", b" \t", b"          ", b""]) ; return "bad indentation"

def m_truncate(t, r):
    if len(t) < 4: return None
    del t[r.randrange(len(t)):]; return "truncate"

def m_nest(t, r):
    n = r.choice([50, 200, 1000, 5000, 20000])
    open_, close = r.choice([(b"(", b")"), (b"[", b"]"), (b"{", b"}"), (b"-", b""), (b"not ", b""), (b"lambda: ", b"")])
    t.insert(r.randrange(len(t) + 1), b" x = " + open_ * n + b"1" + close * n + b"\n"); return "huge nesting x%d" % n

def m_nest_blocks(t, r):
    n = r.choice([30, 100, 400])
    body = b"".join(b" " * (2 * i) + b"if true:\n" for i in range(n)) + b" " * (2 * n) + b"pass\n"
    t.append(b"\ndef zz_deep():\n" + body); return "deep block nesting %d" % n

def m_longtok(t, r):
    t.insert(r.randrange(len(t) + 1), b" " + b"a" * r.choice([300, 5000, 100000]) + b" "); return "very long identifier"

def m_bytes(t, r):
    return None

def m_crlf(t, r):
    for i, x in enumerate(t):
        if x[:1] == b"\n": t[i] = b"\r" + x
    return "CRLF"

MUTS = [m_delete] * 4 + [m_dup] * 2 + [m_swap] * 3 + [m_replace] * 3 + [m_insert] * 3 + [m_unbalance] * 3 + [m_unterminated] * 2 + \
       [m_indent] * 3 + [m_truncate] + [m_nest, m_nest_blocks, m_longtok, m_crlf]

RAWC = re.compile(r"\[ICE\]|\[C-1\]|build/\S+\.[ch]:\d+:\d+|\.c:\d+:\d+: (?:warning|error)|In function '|undefined reference|cc1:|collect2|\bld: |gcc: |clang: |warning: .*\[-W|note: in expansion|AddressSanitizer|runtime error:|Segmentation|Aborted|core dumped|Traceback|panicked at|RUST_BACKTRACE|stack overflow|Stack overflow", re.I)


def classify(rc, text):
    if rc == 124: return "CRASH", "timeout"
    if rc < 0 or rc >= 128: return "CRASH", "signal rc=%d" % rc
    m = RAWC.search(text)
    if m: return "ICE", "raw: " + m.group(0)
    if rc == 0:
        if re.search(r"^warning\b(?!: \[)", text, re.M): return "ICE", "raw warning"
        return "OK", ""
    if not re.search(r"^error: \S", text, re.M): return "NODIAG", ""
    return "DIAG", ""


def make(seed, files):
    r = random.Random(seed)
    path = files[r.randrange(len(files))]
    src = open(path, "rb").read()
    k = r.random()
    if k < 0.03: return path, b"", "empty file"
    if k < 0.05: return path, bytes(r.randrange(256) for _ in range(r.choice([1, 10, 200, 3000]))), "random bytes"
    if k < 0.07: return path, b" \n\n  \n# only comment\n", "blank file"
    t = toks(src)
    descs = []
    for _ in range(r.choice([1, 1, 1, 2, 3])):
        for _try in range(5):
            d = r.choice(MUTS)(t, r)
            if d: descs.append(d); break
    return path, b"".join(t), ", ".join(descs)


def one(task):
    seed, files, tmp_root = task
    path, data, desc = make(seed, files)
    d = tempfile.mkdtemp(prefix="tf", dir=tmp_root)
    name = "tok_%d.tr" % seed
    target = os.path.join(d, name)
    try:
        open(target, "wb").write(data)
        env = dict(os.environ, TAURARO_SHOW_C_ERRORS="0")
        try:
            p = subprocess.run([TAURAROC, name, "-o", "out"], cwd=d, capture_output=True, timeout=TIMEOUT, env=env)
            rc, text = p.returncode, (p.stdout + p.stderr).decode("utf-8", "replace")
        except subprocess.TimeoutExpired:
            rc, text = 124, "TIMEOUT"
        cls, why = classify(rc, text)
        keep = (data, text[:4000]) if cls in ("ICE", "CRASH", "NODIAG") else None
        return seed, cls, desc + (" | " + why if why else ""), path, keep
    finally:
        shutil.rmtree(d, ignore_errors=True)


def main():
    seed0, count = int(sys.argv[1]), int(sys.argv[2])
    jobs = int(sys.argv[3]) if len(sys.argv) > 3 else 3
    files = sorted(os.path.join(ROOT, "tests", d, f) for d in ("regression", "lang") for f in os.listdir(os.path.join(ROOT, "tests", d))
                   if f.endswith(".tr") and os.path.getsize(os.path.join(ROOT, "tests", d, f)) < 6000)
    tmp_root = tempfile.mkdtemp(prefix="tokfuzz")
    counts = {}
    with cf.ThreadPoolExecutor(jobs) as ex:
        for seed, cls, desc, path, keep in ex.map(one, [(seed0 + k, files, tmp_root) for k in range(count)]):
            counts[cls] = counts.get(cls, 0) + 1
            if keep:
                dd = os.path.join(OUT, cls, str(seed)); os.makedirs(dd, exist_ok=True)
                open(os.path.join(dd, "prog.tr"), "wb").write(keep[0])
                open(os.path.join(dd, "note.txt"), "w").write("%s\nbase: %s\n%s\n\n%s" % (desc, os.path.relpath(path, ROOT), cls, keep[1]))
                print("%s seed=%d %s [%s]" % (cls, seed, desc, os.path.relpath(path, ROOT)), flush=True)
    print("== results:", ", ".join("%s=%d" % kv for kv in sorted(counts.items())))
    shutil.rmtree(tmp_root, ignore_errors=True)


if __name__ == "__main__":
    main()
