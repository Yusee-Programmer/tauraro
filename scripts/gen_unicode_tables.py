#!/usr/bin/env python3
"""Generate the Unicode data tables in std/unicode/unicode_native.c.

Usage:
    python3 -I scripts/gen_unicode_tables.py UCD_DIR [path/to/unicode_native.c]
    python3 -I scripts/gen_unicode_tables.py --tests UCD_DIR [path/to/122_unicode_norm.tr]
    python3 -I scripts/gen_unicode_tables.py --tests-full UCD_DIR copy_of_122.tr   (every case)

UCD_DIR must hold these files from https://www.unicode.org/Public/<ver>/ucd/
(flattened into one directory):

    UnicodeData.txt  DerivedNormalizationProps.txt  CaseFolding.txt
    SpecialCasing.txt  EastAsianWidth.txt  DerivedCoreProperties.txt
    auxiliary/GraphemeBreakProperty.txt  auxiliary/WordBreakProperty.txt
    auxiliary/SentenceBreakProperty.txt  emoji/emoji-data.txt

The script rewrites the region between the BEGIN/END GENERATED TABLES markers
of the C file and leaves the hand-written algorithms around it untouched.
It uses only the files above (not Python's unicodedata), so the output does
not depend on the Python version.

With --tests it instead refreshes the conformance subset embedded in
tests/lang/122_unicode_norm.tr (between the BEGIN/END GENERATED CASES
markers) from NormalizationTest.txt, auxiliary/GraphemeBreakTest.txt,
auxiliary/WordBreakTest.txt and auxiliary/SentenceBreakTest.txt in UCD_DIR.
"""

import os
import re
import sys

MAXCP = 0x110000
BLOCK = 128

GC_NAMES = ["Cn", "Lu", "Ll", "Lt", "Lm", "Lo", "Mn", "Mc", "Me", "Nd", "Nl", "No",
            "Pc", "Pd", "Ps", "Pe", "Pi", "Pf", "Po", "Sm", "Sc", "Sk", "So",
            "Zs", "Zl", "Zp", "Cc", "Cf", "Cs", "Co"]
GCB_NAMES = ["Other", "CR", "LF", "Control", "Extend", "ZWJ", "Regional_Indicator",
             "Prepend", "SpacingMark", "L", "V", "T", "LV", "LVT"]
WB_NAMES = ["Other", "CR", "LF", "Newline", "Extend", "ZWJ", "Regional_Indicator",
            "Format", "Katakana", "Hebrew_Letter", "ALetter", "Single_Quote",
            "Double_Quote", "MidNumLet", "MidLetter", "MidNum", "Numeric",
            "ExtendNumLet", "WSegSpace"]
SB_NAMES = ["Other", "CR", "LF", "Extend", "Sep", "Format", "Sp", "Lower", "Upper",
            "OLetter", "Numeric", "ATerm", "SContinue", "STerm", "Close"]
INCB_NAMES = {"Linker": 1, "Consonant": 2, "Extend": 3}

# flag bits (must match unicode_native.c)
F_EXTPICT = 1 << 0
F_INCB_SHIFT = 1          # 2 bits
F_NFD_NO = 1 << 3
F_NFKD_NO = 1 << 4
F_NFC_NO = 1 << 5
F_NFC_MAYBE = 1 << 6
F_NFKC_NO = 1 << 7
F_NFKC_MAYBE = 1 << 8
F_HAS_DECOMP = 1 << 9     # canonical or compatibility decomposition (non-Hangul)
F_CASED = 1 << 10         # Cased (DerivedCoreProperties)
F_CASE_IGN = 1 << 11      # Case_Ignorable


def rng(field):
    if ".." in field:
        a, b = field.split("..")
        return int(a, 16), int(b, 16)
    v = int(field, 16)
    return v, v


def read_prop_file(path):
    """Yield (lo, hi, [fields...]) for each data line."""
    with open(path, encoding="utf-8") as f:
        for line in f:
            line = line.split("#", 1)[0].strip()
            if not line:
                continue
            parts = [p.strip() for p in line.split(";")]
            lo, hi = rng(parts[0])
            yield lo, hi, parts[1:]


def gen_tests(ucd, out_path, full=False):
    """Embed a representative subset (or, with full=True, all) of the official conformance files."""
    P = lambda n: os.path.join(ucd, n)
    o = []
    w = o.append
    # NormalizationTest: c1;c2;c3;c4;c5 (code point sequences in hex).
    step = {"@Part0": 1, "@Part1": 60, "@Part2": 20, "@Part3": 3, "@Part4": 15, "@Part5": 1}
    if full:
        step = {}
    part, k, nn = None, 0, 0
    w("def norm_cases() -> List[str]:\n    mut c: List[str] = []\n")
    with open(P("NormalizationTest.txt"), encoding="utf-8") as f:
        for line in f:
            if line.startswith("@Part"):
                part, k = line.split()[0], 0
                continue
            if not line[:1].strip() or line.startswith("#"):
                continue
            if k % step.get(part, 1) == 0:
                cols = line.split(";")[:5]
                w('    c.append("%s")\n' % ";".join(cols))
                nn += 1
            k += 1
    w("    return c\n\n")
    counts = [nn]
    # Break tests: "| 0020 x 0308 |" (| = break, x = no break).
    for fname, fn, every in (("GraphemeBreakTest.txt", "grapheme_cases", 4),
                             ("WordBreakTest.txt", "word_cases", 6),
                             ("SentenceBreakTest.txt", "sentence_cases", 4)):
        rows = []
        with open(P(fname), encoding="utf-8") as f:
            for line in f:
                line = line.split("#", 1)[0].strip()
                if not line:
                    continue
                rows.append(line.replace("\u00f7", "|").replace("\u00d7", "x"))
        tail = 60
        pick = [r for i, r in enumerate(rows) if full or i % every == 0 or i >=len(rows) - tail]
        w("def %s() -> List[str]:\n    mut c: List[str] = []\n" % fn)
        for r in pick:
            w('    c.append("%s")\n' % r)
        w("    return c\n\n")
        counts.append(len(pick))
    gen = "".join(o)
    with open(out_path, encoding="utf-8") as f:
        src = f.read()
    b0 = "# BEGIN GENERATED CASES\n"
    e0 = "# END GENERATED CASES"
    i = src.index(b0) + len(b0)
    j = src.index(e0)
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(src[:i] + gen + src[j:])
    print("wrote %s: %d normalization, %d grapheme, %d word, %d sentence cases" % tuple([out_path] + counts))


def main():
    if len(sys.argv) >= 3 and sys.argv[1] in ("--tests", "--tests-full"):
        here = os.path.dirname(os.path.abspath(__file__))
        out = sys.argv[3] if len(sys.argv) > 3 else os.path.join(here, "..", "tests", "lang", "122_unicode_norm.tr")
        gen_tests(sys.argv[2], out, sys.argv[1] == "--tests-full")
        return
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(2)
    ucd = sys.argv[1]
    here = os.path.dirname(os.path.abspath(__file__))
    out_path = sys.argv[2] if len(sys.argv) > 2 else os.path.join(here, "..", "std", "unicode", "unicode_native.c")
    P = lambda n: os.path.join(ucd, n)

    version = "?"
    with open(P("DerivedCoreProperties.txt"), encoding="utf-8") as f:
        m = re.search(r"DerivedCoreProperties-(\d+\.\d+\.\d+)", f.readline())
        if m:
            version = m.group(1)

    # ── UnicodeData ─────────────────────────────────────────────────────────
    gc = bytearray(MAXCP)          # 0 = Cn
    ccc = bytearray(MAXCP)
    decomp = {}                    # cp -> (is_compat, [cps])
    upper_s, lower_s, title_s = {}, {}, {}
    gci = {n: i for i, n in enumerate(GC_NAMES)}
    with open(P("UnicodeData.txt"), encoding="utf-8") as f:
        first = None
        for line in f:
            fl = line.rstrip("\n").split(";")
            cp = int(fl[0], 16)
            name = fl[1]
            g = gci[fl[2]]
            if name.endswith(", First>"):
                first = cp
                continue
            if name.endswith(", Last>"):
                for c in range(first, cp + 1):
                    gc[c] = g
                    ccc[c] = int(fl[3])
                first = None
                continue
            gc[cp] = g
            ccc[cp] = int(fl[3])
            if fl[5]:
                d = fl[5].split()
                compat = d[0].startswith("<")
                if compat:
                    d = d[1:]
                decomp[cp] = (compat, [int(x, 16) for x in d])
            if fl[12]:
                upper_s[cp] = int(fl[12], 16)
            if fl[13]:
                lower_s[cp] = int(fl[13], 16)
            if fl[14]:
                title_s[cp] = int(fl[14], 16)

    def full_decomp(cp, compat):
        if cp not in decomp:
            return [cp]
        is_c, d = decomp[cp]
        if is_c and not compat:
            return [cp]
        out = []
        for c in d:
            out.extend(full_decomp(c, compat))
        return out

    # ── Normalization properties ───────────────────────────────────────────
    flags = [0] * MAXCP
    excluded = set()
    for lo, hi, fs in read_prop_file(P("DerivedNormalizationProps.txt")):
        prop = fs[0]
        val = fs[1] if len(fs) > 1 else ""
        bit = 0
        if prop == "NFD_QC" and val == "N": bit = F_NFD_NO
        elif prop == "NFKD_QC" and val == "N": bit = F_NFKD_NO
        elif prop == "NFC_QC" and val == "N": bit = F_NFC_NO
        elif prop == "NFC_QC" and val == "M": bit = F_NFC_MAYBE
        elif prop == "NFKC_QC" and val == "N": bit = F_NFKC_NO
        elif prop == "NFKC_QC" and val == "M": bit = F_NFKC_MAYBE
        elif prop == "Full_Composition_Exclusion":
            for c in range(lo, hi + 1):
                excluded.add(c)
            continue
        if bit:
            for c in range(lo, hi + 1):
                flags[c] |= bit

    for cp in decomp:
        if not (0xAC00 <= cp <= 0xD7A3):
            flags[cp] |= F_HAS_DECOMP

    # ── Core properties: InCB, Cased, Case_Ignorable ───────────────────────
    for lo, hi, fs in read_prop_file(P("DerivedCoreProperties.txt")):
        if fs[0] == "InCB":
            v = INCB_NAMES[fs[1]] << F_INCB_SHIFT
        elif fs[0] == "Cased":
            v = F_CASED
        elif fs[0] == "Case_Ignorable":
            v = F_CASE_IGN
        else:
            continue
        for c in range(lo, hi + 1):
            flags[c] |= v

    epres = set()
    for lo, hi, fs in read_prop_file(P("emoji-data.txt")):
        if fs[0] == "Extended_Pictographic":
            for c in range(lo, hi + 1):
                flags[c] |= F_EXTPICT
        elif fs[0] == "Emoji_Presentation":
            for c in range(lo, hi + 1):
                epres.add(c)

    # ── Break properties ───────────────────────────────────────────────────
    def load_enum(fname, names):
        arr = bytearray(MAXCP)
        idx = {n: i for i, n in enumerate(names)}
        for lo, hi, fs in read_prop_file(P(fname)):
            v = idx[fs[0]]
            for c in range(lo, hi + 1):
                arr[c] = v
        return arr

    gcb = load_enum("GraphemeBreakProperty.txt", GCB_NAMES)
    wb = load_enum("WordBreakProperty.txt", WB_NAMES)
    sb = load_enum("SentenceBreakProperty.txt", SB_NAMES)

    # ── Display width ──────────────────────────────────────────────────────
    eaw = {}
    for lo, hi, fs in read_prop_file(P("EastAsianWidth.txt")):
        if fs[0] in ("W", "F"):
            for c in range(lo, hi + 1):
                eaw[c] = 2
    width = bytearray(MAXCP)
    zero_gc = {gci["Mn"], gci["Me"], gci["Cf"], gci["Cc"], gci["Zl"], gci["Zp"], gci["Cs"]}
    for c in range(MAXCP):
        g = gc[c]
        if g in zero_gc and c != 0x00AD:
            w = 0
        elif 0x1160 <= c <= 0x11FF or 0xD7B0 <= c <= 0xD7FF:
            w = 0          # Hangul medial vowels / final consonants join the syllable
        elif c == 0x200B:
            w = 0
        elif c in eaw or c in epres:
            w = 2
        else:
            w = 1
        width[c] = w

    # ── Combined property records + two-stage index ────────────────────────
    recs = {}
    rec_list = []
    rec_of = [0] * MAXCP
    for c in range(MAXCP):
        key = (ccc[c], gc[c], gcb[c], wb[c], sb[c], width[c], flags[c])
        r = recs.get(key)
        if r is None:
            r = len(rec_list)
            recs[key] = r
            rec_list.append(key)
        rec_of[c] = r
    assert len(rec_list) < 65536

    blocks = {}
    block_list = []
    stage1 = []
    for b in range(MAXCP // BLOCK):
        chunk = tuple(rec_of[b * BLOCK:(b + 1) * BLOCK])
        i = blocks.get(chunk)
        if i is None:
            i = len(block_list)
            blocks[chunk] = i
            block_list.append(chunk)
        stage1.append(i)
    assert len(block_list) < 65536

    # ── Decomposition table ────────────────────────────────────────────────
    data = []                 # shared uint32 pool for decompositions
    pool_idx = {}

    def pool(seq):
        t = tuple(seq)
        if t in pool_idx:
            return pool_idx[t]
        # try to find it already in the pool (cheap reuse for singletons)
        off = len(data)
        data.extend(seq)
        pool_idx[t] = off
        return off

    dec_rows = []
    for cp in sorted(decomp):
        if 0xAC00 <= cp <= 0xD7A3:
            continue
        cd = full_decomp(cp, False)
        kd = full_decomp(cp, True)
        clen = 0 if cd == [cp] else len(cd)
        coff = pool(cd) if clen else 0
        koff = pool(kd)
        dec_rows.append((cp, coff, koff, clen, len(kd)))
    assert len(data) < 65536

    # ── Composition pairs ──────────────────────────────────────────────────
    comp = []
    for cp, (is_c, d) in decomp.items():
        if is_c or len(d) != 2 or cp in excluded:
            continue
        comp.append((d[0], d[1], cp))
    comp.sort()

    # ── Case mappings ──────────────────────────────────────────────────────
    lower_f = {c: [v] for c, v in lower_s.items()}
    upper_f = {c: [v] for c, v in upper_s.items()}
    title_f = {c: [v] for c, v in title_s.items()}
    for lo, hi, fs in read_prop_file(P("SpecialCasing.txt")):
        # fs: lower, title, upper, [condition], ''
        cond = fs[3].strip() if len(fs) > 3 else ""
        if cond:
            continue          # conditional mappings (Final_Sigma handled in C; locales skipped)
        cvt = lambda s: [int(x, 16) for x in s.split()]
        lower_f[lo] = cvt(fs[0])
        title_f[lo] = cvt(fs[1])
        upper_f[lo] = cvt(fs[2])
    fold_f = {}
    for lo, hi, fs in read_prop_file(P("CaseFolding.txt")):
        if fs[0] in ("C", "F"):
            fold_f[lo] = [int(x, 16) for x in fs[1].split()]

    case_data = []
    case_idx = {}

    def cpool(seq):
        t = tuple(seq)
        if t not in case_idx:
            case_idx[t] = len(case_data)
            case_data.extend(seq)
        return case_idx[t]

    simple_fold = {}
    for lo, hi, fs in read_prop_file(P("CaseFolding.txt")):
        if fs[0] in ("C", "S"):
            simple_fold[lo] = int(fs[1], 16)

    def case_rows(m, simple):
        rows = []
        for c in sorted(set(m) | set(simple)):
            v = m.get(c, [c])
            sv = simple.get(c, c)
            if v == [c] and sv == c:
                continue
            if len(v) == 1:
                rows.append((c, v[0], 0, sv))       # single code point stored inline
            else:
                rows.append((c, cpool(v), len(v), sv))
        return rows

    lower_rows = case_rows(lower_f, lower_s)
    upper_rows = case_rows(upper_f, upper_s)
    title_rows = case_rows(title_f, title_s)
    fold_rows = case_rows(fold_f, simple_fold)

    # ── Emit ───────────────────────────────────────────────────────────────
    o = []
    w = o.append
    w("/* Generated by scripts/gen_unicode_tables.py from the Unicode %s UCD. */\n" % version)
    w("#define UNI_VERSION \"%s\"\n" % version)
    w("#define UNI_BLOCK_SHIFT %d\n" % (BLOCK.bit_length() - 1))
    w("#define UNI_BLOCK_MASK %d\n\n" % (BLOCK - 1))

    def arr(ctype, name, vals, per=16):
        w("static const %s %s[%d] = {\n" % (ctype, name, len(vals)))
        for i in range(0, len(vals), per):
            w("    " + ",".join(str(v) for v in vals[i:i + per]) + ",\n")
        w("};\n\n")

    w("/* ccc, gc, gcb, wb, sb, width, flags */\n")
    w("static const uni_rec uni_recs[%d] = {\n" % len(rec_list))
    for r in rec_list:
        w("    {%d,%d,%d,%d,%d,%d,%d},\n" % r)
    w("};\n\n")
    arr("uint16_t", "uni_stage1", stage1)
    flat = [v for blk in block_list for v in blk]
    arr("uint16_t", "uni_stage2", flat, 32)

    w("/* cp, canonical offset, compatibility offset, canonical len (0 = none), compatibility len */\n")
    w("static const uni_dec uni_decs[%d] = {\n" % len(dec_rows))
    for r in dec_rows:
        w("    {0x%X,%d,%d,%d,%d},\n" % r)
    w("};\n\n")
    arr("uint32_t", "uni_dec_data", data)

    w("static const uni_comp uni_comps[%d] = {\n" % len(comp))
    for a, b, c in comp:
        w("    {0x%X,0x%X,0x%X},\n" % (a, b, c))
    w("};\n\n")

    for nm, rows in (("lower", lower_rows), ("upper", upper_rows), ("title", title_rows), ("fold", fold_rows)):
        w("static const uni_case uni_%s[%d] = {\n" % (nm, len(rows)))
        for c, v, n, sv in rows:
            w("    {0x%X,0x%X,%d,0x%X},\n" % (c, v, n, sv))
        w("};\n\n")
    arr("uint32_t", "uni_case_data", case_data)

    gen = "".join(o)
    with open(out_path, encoding="utf-8") as f:
        src = f.read()
    b0 = "/* BEGIN GENERATED TABLES */\n"
    e0 = "/* END GENERATED TABLES */"
    i = src.index(b0) + len(b0)
    j = src.index(e0)
    src = src[:i] + gen + src[j:]
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(src)
    print("wrote %s: Unicode %s, %d records, %d blocks, %d decompositions, %d compositions, "
          "%d/%d/%d/%d case rows" % (out_path, version, len(rec_list), len(block_list), len(dec_rows),
                                     len(comp), len(lower_rows), len(upper_rows), len(title_rows), len(fold_rows)))


if __name__ == "__main__":
    main()
