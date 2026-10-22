#!/usr/bin/env python3
"""Generate the Python-zoneinfo reference vectors hardcoded in tests/lang/121_tz.tr.

    python3 scripts/gen_tz_vectors.py > /tmp/vectors.tr   # then paste

For each zone, transitions are taken from the TZif file (a spread over its
history) plus transitions found by scanning 2038-2040 and 2100 (where the
POSIX footer rule applies). Each produces:
  UTC vectors   t-1, t, t+1 -> (utcoffset, bool(dst()), tzname())
  local vectors wall times around the gap/overlap -> timestamp() with fold=0
                and fold=1
Vectors after the zone's last listed transition are also emitted separately:
the test checks those against the embedded POSIX table (std/sys/tz_table.tr).
"""
import struct, sys
from datetime import datetime, timedelta, timezone
from zoneinfo import ZoneInfo

ZONES = [
    "America/New_York", "Europe/London", "Australia/Lord_Howe", "Asia/Kathmandu",
    "America/Sao_Paulo", "Africa/Casablanca", "Pacific/Apia", "Australia/Sydney",
]
EPOCH = datetime(1970, 1, 1)

def tzif_transitions(path):
    d = open(path, "rb").read()
    cnt = struct.unpack(">6l", d[20:44])
    isut, isstd, leap, timecnt, typecnt, charcnt = cnt
    p = 44 + timecnt * 5 + typecnt * 6 + charcnt + leap * 8 + isstd + isut
    cnt = struct.unpack(">6l", d[p + 20:p + 44])
    timecnt = cnt[3]
    p += 44
    return list(struct.unpack(f">{timecnt}q", d[p:p + 8 * timecnt]))

def info(z, ts):
    dt = datetime.fromtimestamp(ts, z)
    return int(dt.utcoffset().total_seconds()), bool(dt.dst()), dt.tzname()

def off(z, ts):
    return info(z, ts)[0]

def scan(z, start, end):
    """Transitions in [start, end) found hourly then bisected to the second."""
    out = []
    t = start
    cur = info(z, t)
    while t < end:
        nxt = info(z, t + 3600)
        if nxt != cur:
            lo, hi = t, t + 3600
            while hi - lo > 1:
                mid = (lo + hi) // 2
                if info(z, mid) == cur: lo = mid
                else: hi = mid
            out.append(hi)
            cur = nxt
        t += 3600
    return out

def pick(trans):
    if len(trans) <= 14:
        return trans
    idx = sorted(set([0, 1, len(trans) // 4, len(trans) // 3, len(trans) // 2,
                      2 * len(trans) // 3, 3 * len(trans) // 4] + list(range(len(trans) - 6, len(trans)))))
    return [trans[i] for i in idx]

def wall_ts(z, wall, fold):
    dt = (EPOCH + timedelta(seconds=wall)).replace(tzinfo=z, fold=fold)
    return int(dt.timestamp())

def tr_list(name, xs):
    return f"    mut {name}: List[int] = [" + ", ".join(str(x) for x in xs) + "]\n"

def main():
    out = sys.stdout
    fns = []
    for zname in ZONES:
        z = ZoneInfo(zname)
        ftrans = tzif_transitions(f"/usr/share/zoneinfo/{zname}")
        last = ftrans[-1] if ftrans else -(1 << 62)
        trans = pick(ftrans)
        y2038 = int(datetime(2038, 1, 1, tzinfo=timezone.utc).timestamp())
        y2040 = int(datetime(2040, 1, 1, tzinfo=timezone.utc).timestamp())
        y2100 = int(datetime(2100, 1, 1, tzinfo=timezone.utc).timestamp())
        y2101 = int(datetime(2101, 1, 1, tzinfo=timezone.utc).timestamp())
        trans += [t for t in scan(z, y2038, y2040) if t not in trans]
        trans += [t for t in scan(z, y2100, y2101) if t not in trans]
        # fixed-offset zones still get a few sample points
        if len(trans) < 3:
            trans += [y2038, y2100]
        uts, uoff, udst, uab = [], [], [], []
        for t in trans:
            for d in (-1, 0, 1):
                o, dst, ab = info(z, t + d)
                uts.append(t + d); uoff.append(o); udst.append(1 if dst else 0); uab.append(ab)
        walls, u0, u1 = [], [], []
        for t in trans:
            ob, oa = off(z, t - 1), off(z, t)
            lo, hi = min(ob, oa), max(ob, oa)
            for w in sorted(set([t + lo - 1, t + lo, (2 * t + lo + hi) // 2, t + hi - 1, t + hi])):
                walls.append(w); u0.append(wall_ts(z, w, 0)); u1.append(wall_ts(z, w, 1))
        key = zname.split("/")[-1].lower()
        fns.append(f"vec_{key}")
        out.write(f"def vec_{key}(t: TestRunner):\n")
        out.write(f"    # {zname}: {len(uts)} UTC vectors, {len(walls)} local vectors (last TZif transition {last})\n")
        out.write(tr_list("u_ts", uts) + tr_list("u_off", uoff) + tr_list("u_dst", udst))
        out.write(f"    mut u_ab = \"{' '.join(uab)}\"\n")
        out.write(tr_list("l_wall", walls) + tr_list("l_u0", u0) + tr_list("l_u1", u1))
        out.write(f"    check_zone(t, \"{zname}\", {last}, u_ts, u_off, u_dst, u_ab, l_wall, l_u0, l_u1)\n\n")
    out.write("def python_vectors(t: TestRunner):\n")
    for f in fns:
        out.write(f"    {f}(t)\n")

main()
