/*
 * std/font/font_native.c -- TrueType / OpenType support for std.font.
 *
 *   - sfnt parsing: TTF, OTF (CFF / CFF2 outlines), TTC collections, WOFF 1.0
 *     (zlib-compressed tables, inflated here);
 *   - tables head, hhea, maxp, hmtx, cmap (formats 0, 4, 6, 10, 12, 13 and 14
 *     variation sequences), name, OS/2, post, loca/glyf (simple and composite
 *     glyphs), CFF/CFF2 Type 2 charstrings, kern (format 0), GPOS pair
 *     adjustment (formats 1 and 2) and GSUB ligatures;
 *   - simple left-to-right shaping (cmap, variation selectors, ligatures,
 *     kerning);
 *   - an anti-aliased rasterizer (signed-area accumulation, as in font-rs /
 *     stb_truetype's v2 rasterizer) and RGBA compositing;
 *   - a glyph subsetter that writes a standalone TrueType font (CFF outlines
 *     are converted to quadratic contours), used by std.encoding.pdf;
 *   - a directory scanner for the system font search.
 *
 * Linked through `# tauraro-cpp-shim: font_native.c` in std/font/native.tr.
 * Every read from font data is bounds checked: malformed fonts produce
 * errors or empty glyphs, never out-of-range accesses.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

/* ── bounded readers ─────────────────────────────────────────────────────── */
typedef struct { const uint8_t* p; uint32_t n; } tf_buf;

static uint32_t rd8(tf_buf b, uint64_t o) { return o < b.n ? b.p[o] : 0; }
static uint32_t rd16(tf_buf b, uint64_t o) { return o + 2 <= b.n ? ((uint32_t)b.p[o] << 8) | b.p[o + 1] : 0; }
static int32_t rds16(tf_buf b, uint64_t o) { return (int16_t)rd16(b, o); }
static uint32_t rd24(tf_buf b, uint64_t o) { return o + 3 <= b.n ? ((uint32_t)b.p[o] << 16) | ((uint32_t)b.p[o + 1] << 8) | b.p[o + 2] : 0; }
static uint32_t rd32(tf_buf b, uint64_t o) {
    return o + 4 <= b.n ? ((uint32_t)b.p[o] << 24) | ((uint32_t)b.p[o + 1] << 16) | ((uint32_t)b.p[o + 2] << 8) | b.p[o + 3] : 0;
}
static tf_buf sub(tf_buf b, uint64_t o, uint64_t n) {
    tf_buf r = { NULL, 0 };
    if (o > b.n) return r;
    if (n > b.n - o) n = b.n - o;
    r.p = b.p + o; r.n = (uint32_t)n;
    return r;
}
static tf_buf subfrom(tf_buf b, uint64_t o) { return sub(b, o, o <= b.n ? b.n - o : 0); }

/* ── growable buffers ────────────────────────────────────────────────────── */
typedef struct { uint8_t* p; size_t n, cap; int bad; } tf_out;
static void out_need(tf_out* o, size_t k) {
    if (o->bad) return;
    if (o->n + k > o->cap) {
        size_t c = o->cap ? o->cap * 2 : 256;
        uint8_t* q;
        while (c < o->n + k) c *= 2;
        q = (uint8_t*)realloc(o->p, c);
        if (!q) { o->bad = 1; return; }
        o->p = q; o->cap = c;
    }
}
static void out_bytes(tf_out* o, const void* d, size_t k) { out_need(o, k); if (o->bad) return; if (k) memcpy(o->p + o->n, d, k); o->n += k; }
static void out_u8(tf_out* o, uint32_t v) { uint8_t b = (uint8_t)v; out_bytes(o, &b, 1); }
static void out_u16(tf_out* o, uint32_t v) { uint8_t b[2] = { (uint8_t)(v >> 8), (uint8_t)v }; out_bytes(o, b, 2); }
static void out_u32(tf_out* o, uint32_t v) { uint8_t b[4] = { (uint8_t)(v >> 24), (uint8_t)(v >> 16), (uint8_t)(v >> 8), (uint8_t)v }; out_bytes(o, b, 4); }
static void out_pad4(tf_out* o) { while (!o->bad && (o->n & 3)) out_u8(o, 0); }
static void put16(uint8_t* p, uint32_t v) { p[0] = (uint8_t)(v >> 8); p[1] = (uint8_t)v; }
static void put32(uint8_t* p, uint32_t v) { p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16); p[2] = (uint8_t)(v >> 8); p[3] = (uint8_t)v; }

/* ── inflate (RFC 1951) for WOFF tables ──────────────────────────────────── */
typedef struct { const uint8_t* in; size_t n, pos; uint32_t bits; int nb; int err; } tf_inf;
static int inf_bit(tf_inf* s, int k) {
    uint32_t v;
    while (s->nb < k) {
        if (s->pos >= s->n) { s->err = 1; return 0; }
        s->bits |= (uint32_t)s->in[s->pos++] << s->nb; s->nb += 8;
    }
    v = s->bits & ((1u << k) - 1); s->bits >>= k; s->nb -= k;
    return (int)v;
}
typedef struct { uint16_t count[16]; uint16_t sym[320]; } tf_huff;
static int huff_build(tf_huff* h, const uint8_t* len, int n) {
    uint16_t offs[16]; int i, left = 1;
    memset(h->count, 0, sizeof h->count);
    for (i = 0; i < n; i++) h->count[len[i]]++;
    h->count[0] = 0;
    for (i = 1; i < 16; i++) { left <<= 1; left -= h->count[i]; if (left < 0) return -1; }
    offs[1] = 0;
    for (i = 1; i < 15; i++) offs[i + 1] = (uint16_t)(offs[i] + h->count[i]);
    for (i = 0; i < n; i++) if (len[i]) h->sym[offs[len[i]]++] = (uint16_t)i;
    return 0;
}
static int huff_dec(tf_inf* s, const tf_huff* h) {
    int code = 0, first = 0, index = 0, l;
    for (l = 1; l < 16; l++) {
        code |= inf_bit(s, 1);
        if (s->err) return -1;
        if (code - h->count[l] < first) return h->sym[index + (code - first)];
        index += h->count[l]; first += h->count[l]; first <<= 1; code <<= 1;
    }
    s->err = 1; return -1;
}
static const uint16_t inf_lbase[29] = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
static const uint8_t inf_lext[29] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
static const uint16_t inf_dbase[30] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
static const uint8_t inf_dext[30] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

/* Inflate a zlib stream into exactly `outn` bytes. 0 on success. */
static int tf_zlib_inflate(const uint8_t* in, size_t n, uint8_t* out, size_t outn) {
    tf_inf s; size_t op = 0; int final;
    memset(&s, 0, sizeof s); s.in = in; s.n = n;
    if (n < 2 || (in[0] & 15) != 8 || ((in[0] << 8) | in[1]) % 31 != 0) return -1;
    s.pos = 2;
    do {
        int type;
        final = inf_bit(&s, 1); type = inf_bit(&s, 2);
        if (s.err) return -1;
        if (type == 0) {
            uint32_t len, nlen;
            s.bits = 0; s.nb = 0;
            if (s.pos + 4 > n) return -1;
            len = in[s.pos] | (in[s.pos + 1] << 8); nlen = in[s.pos + 2] | (in[s.pos + 3] << 8); s.pos += 4;
            if ((len ^ 0xFFFF) != nlen || s.pos + len > n || op + len > outn) return -1;
            memcpy(out + op, in + s.pos, len); op += len; s.pos += len;
        } else if (type == 1 || type == 2) {
            tf_huff lh, dh; uint8_t lens[320]; int i;
            if (type == 1) {
                for (i = 0; i < 144; i++) lens[i] = 8;
                for (; i < 256; i++) lens[i] = 9;
                for (; i < 280; i++) lens[i] = 7;
                for (; i < 288; i++) lens[i] = 8;
                huff_build(&lh, lens, 288);
                for (i = 0; i < 30; i++) lens[i] = 5;
                huff_build(&dh, lens, 30);
            } else {
                static const uint8_t ord[19] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
                int hl = inf_bit(&s, 5) + 257, hd = inf_bit(&s, 5) + 1, hc = inf_bit(&s, 4) + 4, k = 0;
                uint8_t cl[19]; tf_huff ch;
                memset(cl, 0, sizeof cl);
                for (i = 0; i < hc; i++) cl[ord[i]] = (uint8_t)inf_bit(&s, 3);
                if (s.err || hl > 286 || huff_build(&ch, cl, 19) < 0) return -1;
                while (k < hl + hd) {
                    int sym = huff_dec(&s, &ch), rep = 0, val = 0;
                    if (sym < 0) return -1;
                    if (sym < 16) { lens[k++] = (uint8_t)sym; continue; }
                    if (sym == 16) { if (k == 0) return -1; val = lens[k - 1]; rep = 3 + inf_bit(&s, 2); }
                    else if (sym == 17) rep = 3 + inf_bit(&s, 3);
                    else rep = 11 + inf_bit(&s, 7);
                    if (s.err || k + rep > hl + hd) return -1;
                    while (rep--) lens[k++] = (uint8_t)val;
                }
                if (huff_build(&lh, lens, hl) < 0 || huff_build(&dh, lens + hl, hd) < 0) return -1;
            }
            for (;;) {
                int sym = huff_dec(&s, &lh);
                if (sym < 0) return -1;
                if (sym < 256) { if (op >= outn) return -1; out[op++] = (uint8_t)sym; }
                else if (sym == 256) break;
                else {
                    int len, ds, dist;
                    sym -= 257; if (sym >= 29) return -1;
                    len = inf_lbase[sym] + inf_bit(&s, inf_lext[sym]);
                    ds = huff_dec(&s, &dh); if (ds < 0 || ds >= 30) return -1;
                    dist = inf_dbase[ds] + inf_bit(&s, inf_dext[ds]);
                    if (s.err || (size_t)dist > op || op + (size_t)len > outn) return -1;
                    while (len--) { out[op] = out[op - dist]; op++; }
                }
            }
        } else return -1;
    } while (!final);
    return op == outn ? 0 : -1;
}

/* ── font object ─────────────────────────────────────────────────────────── */
#define TF_MAXFD 256
typedef struct { tf_buf b; uint32_t count; uint32_t offsize; uint64_t offs; uint64_t base; } tf_index;

typedef struct { float x, y; } tf_pt;
typedef struct {
    uint8_t* cmd; tf_pt* pt; int ncmd, npt, ccap, pcap; int bad;
    float cx, cy, sx, sy; int open;
} tf_path;   /* cmd: 0 move(1 pt) 1 line(1) 2 quad(2) 3 cubic(3) 4 close(0) */

typedef struct { int gid, cluster, cluster_end; double adv; } tf_sg;

typedef struct tf_font {
    uint8_t* data; uint32_t len; tf_buf all, sf;   /* sf = sfnt from its table directory */
    tf_buf head, hhea, maxp, hmtx, cmap, name, os2, post, loca, glyf, cff, kern, gpos, gsub, cvt, fpgm, prep;
    int upem, nglyphs, nhm, locfmt, nfonts;
    int ascent, descent, line_gap, xmin, ymin, xmax, ymax;
    tf_buf cmap_sub; int cmap_fmt; tf_buf uvs;
    int is_cff, is_cff2;
    tf_index cs, gsubrs, lsubrs, fdarray;
    tf_index fd_lsubrs[TF_MAXFD]; int nfd;
    tf_buf fdselect; int cid;
    int vs_regions[64]; int nvs;           /* CFF2 region counts per vsindex */
    uint32_t* kern_sub; int nkern_sub;
    uint32_t* liga_sub; int nliga_sub;
    tf_buf kern0; uint32_t kern0_n;
    tf_sg* sg; int nsg, sgcap;
    char err[160];
} tf_font;

static int tf_tag(const char* t) { return (t[0] << 24) | (t[1] << 16) | (t[2] << 8) | t[3]; }

static tf_buf tf_table(tf_font* f, const char* tag) {
    tf_buf none = { NULL, 0 };
    uint32_t n = rd16(f->sf, 4), i, want = (uint32_t)tf_tag(tag);
    for (i = 0; i < n; i++) {
        uint64_t r = 12 + 16 * (uint64_t)i;
        if (rd32(f->sf, r) == want) {
            uint32_t off = rd32(f->sf, r + 8), ln = rd32(f->sf, r + 12);
            if ((uint64_t)off + ln > f->all.n) return none;
            return sub(f->all, off, ln);
        }
    }
    return none;
}

/* WOFF 1.0 -> plain sfnt (newly allocated), NULL on error. */
static uint8_t* tf_unwoff(const uint8_t* d, uint32_t n, uint32_t* outlen, char* err) {
    tf_buf b = { d, n };
    uint32_t nt = rd16(b, 12), total = rd32(b, 16), i;
    uint8_t* o; uint32_t pos;
    if (n < 44 || nt == 0 || nt > 4096 || total < 12 + 16 * nt || total > (1u << 28)) { strcpy(err, "font: bad WOFF header"); return NULL; }
    o = (uint8_t*)calloc(1, total + 4);
    if (!o) { strcpy(err, "font: out of memory"); return NULL; }
    put32(o, rd32(b, 4)); put16(o + 4, nt);
    { uint32_t es = 0, p2 = 1; while (p2 * 2 <= nt) { p2 *= 2; es++; } put16(o + 6, p2 * 16); put16(o + 8, es); put16(o + 10, nt * 16 - p2 * 16); }
    pos = 12 + 16 * nt;
    for (i = 0; i < nt; i++) {
        uint64_t r = 44 + 20 * (uint64_t)i;
        uint32_t tag = rd32(b, r), off = rd32(b, r + 4), clen = rd32(b, r + 8), olen = rd32(b, r + 12), ck = rd32(b, r + 16);
        if (r + 20 > n || (uint64_t)off + clen > n || clen > olen || (uint64_t)pos + olen > total) { free(o); strcpy(err, "font: bad WOFF table directory"); return NULL; }
        put32(o + 12 + 16 * i, tag); put32(o + 16 + 16 * i, ck); put32(o + 20 + 16 * i, pos); put32(o + 24 + 16 * i, olen);
        if (clen == olen) memcpy(o + pos, d + off, olen);
        else if (tf_zlib_inflate(d + off, clen, o + pos, olen) != 0) { free(o); strcpy(err, "font: corrupt WOFF table data"); return NULL; }
        pos += olen; while (pos & 3 && pos < total) pos++;
    }
    *outlen = total;
    return o;
}

/* ── CFF ─────────────────────────────────────────────────────────────────── */
static tf_index cff_index(tf_buf b, uint64_t off, int cff2, uint64_t* end) {
    tf_index x; memset(&x, 0, sizeof x); x.b = b;
    if (cff2) { x.count = rd32(b, off); off += 4; } else { x.count = rd16(b, off); off += 2; }
    if (x.count == 0) { if (end) *end = off; return x; }
    x.offsize = rd8(b, off); off += 1;
    if (x.offsize < 1 || x.offsize > 4 || off + (uint64_t)(x.count + 1) * x.offsize > b.n) { x.count = 0; if (end) *end = b.n; return x; }
    x.offs = off;
    x.base = off + (uint64_t)(x.count + 1) * x.offsize - 1;
    if (end) {
        uint64_t last = 0; uint32_t k;
        for (k = 0; k < x.offsize; k++) last = (last << 8) | rd8(b, x.offs + (uint64_t)x.count * x.offsize + k);
        *end = x.base + last;
    }
    return x;
}
static tf_buf idx_get(const tf_index* x, uint32_t i) {
    tf_buf none = { NULL, 0 };
    uint64_t a = 0, z = 0; uint32_t k;
    if (i >= x->count) return none;
    for (k = 0; k < x->offsize; k++) a = (a << 8) | rd8(x->b, x->offs + (uint64_t)i * x->offsize + k);
    for (k = 0; k < x->offsize; k++) z = (z << 8) | rd8(x->b, x->offs + (uint64_t)(i + 1) * x->offsize + k);
    if (a < 1 || z < a) return none;
    if (x->base + z > x->b.n) return none;
    return sub(x->b, x->base + a, z - a);
}

/* Parse a DICT; calls back with (op, operands, n). Operators 12 x are 1200+x. */
typedef struct { double v[48]; int n; } dict_ops;
static double dict_real(tf_buf d, uint64_t* p) {
    char s[64]; int k = 0, done = 0;
    while (!done && *p < d.n) {
        uint32_t b = rd8(d, (*p)++); int h;
        for (h = 0; h < 2 && !done; h++) {
            int nib = h == 0 ? (int)(b >> 4) : (int)(b & 15);
            if (k > 60) { done = 1; break; }
            if (nib <= 9) s[k++] = (char)('0' + nib);
            else if (nib == 10) s[k++] = '.';
            else if (nib == 11) s[k++] = 'E';
            else if (nib == 12) { s[k++] = 'E'; s[k++] = '-'; }
            else if (nib == 14) s[k++] = '-';
            else if (nib == 15) done = 1;
        }
    }
    s[k] = 0;
    return atof(s);
}
/* Look up operator `want` in a DICT; fills ops. Returns 1 when found. */
static int dict_find(tf_buf d, int want, dict_ops* res) {
    uint64_t p = 0; dict_ops st; st.n = 0;
    while (p < d.n) {
        uint32_t b = rd8(d, p);
        if (b <= 21 || b == 24) {
            int op = (int)b; p++;
            if (b == 12) { op = 1200 + (int)rd8(d, p); p++; }
            if (op == want) { *res = st; return 1; }
            st.n = 0;
            continue;
        }
        if (b == 23) { p++; st.n = 0; continue; }   /* CFF2 blend: never on the operators we read */
        {
            double v = 0;
            if (b == 28) { v = (int16_t)rd16(d, p + 1); p += 3; }
            else if (b == 29) { v = (int32_t)rd32(d, p + 1); p += 5; }
            else if (b == 30) { p++; v = dict_real(d, &p); }
            else if (b >= 32 && b <= 246) { v = (int)b - 139; p++; }
            else if (b >= 247 && b <= 250) { v = ((int)b - 247) * 256 + (int)rd8(d, p + 1) + 108; p += 2; }
            else if (b >= 251 && b <= 254) { v = -((int)b - 251) * 256 - (int)rd8(d, p + 1) - 108; p += 2; }
            else { p++; continue; }
            if (st.n < 48) st.v[st.n++] = v;
        }
    }
    return 0;
}

static int cff_subr_bias(uint32_t n) { return n < 1240 ? 107 : n < 33900 ? 1131 : 32768; }

static int tf_init_cff(tf_font* f) {
    tf_buf c = f->cff, top;
    dict_ops o;
    uint64_t end = 0;
    int major = (int)rd8(c, 0), hdr = (int)rd8(c, 2);
    if (c.n < 4) return -1;
    if (major == 2) {
        uint32_t tlen = rd16(c, 3);
        f->is_cff2 = 1;
        top = sub(c, (uint64_t)hdr, tlen);
        f->gsubrs = cff_index(c, (uint64_t)hdr + tlen, 1, &end);
    } else {
        tf_index names, tops, strs;
        names = cff_index(c, (uint64_t)hdr, 0, &end);
        tops = cff_index(c, end, 0, &end);
        strs = cff_index(c, end, 0, &end);
        (void)names; (void)strs;
        f->gsubrs = cff_index(c, end, 0, &end);
        top = idx_get(&tops, 0);
    }
    if (!dict_find(top, 17, &o) || o.n < 1) return -1;
    f->cs = cff_index(c, (uint64_t)o.v[0], f->is_cff2, NULL);
    if (f->cs.count == 0) return -1;
    if (dict_find(top, 1236, &o) && o.n >= 1) {      /* FDArray */
        uint32_t i;
        f->fdarray = cff_index(c, (uint64_t)o.v[0], f->is_cff2, NULL);
        f->cid = 1;
        f->nfd = f->fdarray.count > TF_MAXFD ? TF_MAXFD : (int)f->fdarray.count;
        for (i = 0; i < (uint32_t)f->nfd; i++) {
            tf_buf fd = idx_get(&f->fdarray, i);
            memset(&f->fd_lsubrs[i], 0, sizeof(tf_index));
            if (dict_find(fd, 18, &o) && o.n >= 2) {
                tf_buf pr = sub(c, (uint64_t)o.v[1], (uint64_t)o.v[0]);
                dict_ops s2;
                if (dict_find(pr, 19, &s2) && s2.n >= 1)
                    f->fd_lsubrs[i] = cff_index(c, (uint64_t)o.v[1] + (uint64_t)s2.v[0], f->is_cff2, NULL);
            }
        }
        if (dict_find(top, 1237, &o) && o.n >= 1) f->fdselect = subfrom(c, (uint64_t)o.v[0]);
    } else if (dict_find(top, 18, &o) && o.n >= 2) {
        tf_buf pr = sub(c, (uint64_t)o.v[1], (uint64_t)o.v[0]);
        dict_ops s2;
        if (dict_find(pr, 19, &s2) && s2.n >= 1)
            f->lsubrs = cff_index(c, (uint64_t)o.v[1] + (uint64_t)s2.v[0], f->is_cff2, NULL);
    }
    if (f->is_cff2 && dict_find(top, 24, &o) && o.n >= 1) {
        /* ItemVariationStore after a u16 length */
        uint64_t vs = (uint64_t)o.v[0] + 2;
        uint32_t cnt = rd16(c, vs + 6), i;
        f->nvs = cnt > 64 ? 64 : (int)cnt;
        for (i = 0; i < (uint32_t)f->nvs; i++) {
            uint32_t ivd = rd32(c, vs + 8 + 4 * (uint64_t)i);
            f->vs_regions[i] = (int)rd16(c, vs + ivd + 4);
        }
    }
    return 0;
}

static int cff_fd_for(tf_font* f, int gid) {
    tf_buf s = f->fdselect;
    int fmt = (int)rd8(s, 0);
    if (s.n == 0) return 0;
    if (fmt == 0) return (int)rd8(s, 1 + (uint64_t)gid);
    if (fmt == 3) {
        uint32_t n = rd16(s, 1), i;
        for (i = 0; i < n; i++) {
            uint32_t first = rd16(s, 3 + 3 * (uint64_t)i), next = rd16(s, 6 + 3 * (uint64_t)i);
            if ((uint32_t)gid >= first && (uint32_t)gid < next) return (int)rd8(s, 5 + 3 * (uint64_t)i);
        }
    }
    if (fmt == 4) {
        uint32_t n = rd32(s, 1), i;
        for (i = 0; i < n && i < 1000000; i++) {
            uint32_t first = rd32(s, 5 + 6 * (uint64_t)i), next = rd32(s, 11 + 6 * (uint64_t)i);
            if ((uint32_t)gid >= first && (uint32_t)gid < next) return (int)rd16(s, 9 + 6 * (uint64_t)i);
        }
    }
    return 0;
}

/* ── path building ───────────────────────────────────────────────────────── */
static void path_free(tf_path* p) { free(p->cmd); free(p->pt); memset(p, 0, sizeof *p); }
static void path_cmd(tf_path* p, int c, int npts, const tf_pt* pts) {
    int i;
    if (p->bad) return;
    if (p->ncmd + 1 > p->ccap) {
        int nc = p->ccap ? p->ccap * 2 : 64; uint8_t* q;
        if (nc > (1 << 24)) { p->bad = 1; return; }
        q = (uint8_t*)realloc(p->cmd, (size_t)nc);
        if (!q) { p->bad = 1; return; }
        p->cmd = q; p->ccap = nc;
    }
    if (p->npt + npts > p->pcap) {
        int nc = p->pcap ? p->pcap * 2 : 128; tf_pt* q;
        while (nc < p->npt + npts) nc *= 2;
        if (nc > (1 << 25)) { p->bad = 1; return; }
        q = (tf_pt*)realloc(p->pt, (size_t)nc * sizeof(tf_pt));
        if (!q) { p->bad = 1; return; }
        p->pt = q; p->pcap = nc;
    }
    p->cmd[p->ncmd++] = (uint8_t)c;
    for (i = 0; i < npts; i++) p->pt[p->npt++] = pts[i];
}
static void path_close(tf_path* p) { if (p->open) { path_cmd(p, 4, 0, NULL); p->open = 0; } }
static void path_move(tf_path* p, float x, float y) {
    tf_pt a; path_close(p); a.x = x; a.y = y; path_cmd(p, 0, 1, &a); p->open = 1; p->cx = p->sx = x; p->cy = p->sy = y;
}
static void path_line(tf_path* p, float x, float y) {
    tf_pt a; if (!p->open) path_move(p, p->cx, p->cy); a.x = x; a.y = y; path_cmd(p, 1, 1, &a); p->cx = x; p->cy = y;
}
static void path_quad(tf_path* p, float x1, float y1, float x, float y) {
    tf_pt a[2]; if (!p->open) path_move(p, p->cx, p->cy); a[0].x = x1; a[0].y = y1; a[1].x = x; a[1].y = y; path_cmd(p, 2, 2, a); p->cx = x; p->cy = y;
}
static void path_cubic(tf_path* p, float x1, float y1, float x2, float y2, float x, float y) {
    tf_pt a[3]; if (!p->open) path_move(p, p->cx, p->cy);
    a[0].x = x1; a[0].y = y1; a[1].x = x2; a[1].y = y2; a[2].x = x; a[2].y = y; path_cmd(p, 3, 3, a); p->cx = x; p->cy = y;
}

/* ── glyf outlines ───────────────────────────────────────────────────────── */
static tf_buf glyf_data(tf_font* f, int gid) {
    tf_buf none = { NULL, 0 };
    uint64_t a, z;
    if (gid < 0 || gid >= f->nglyphs || f->glyf.n == 0) return none;
    if (f->locfmt) { a = rd32(f->loca, 4 * (uint64_t)gid); z = rd32(f->loca, 4 * (uint64_t)gid + 4); }
    else { a = 2 * (uint64_t)rd16(f->loca, 2 * (uint64_t)gid); z = 2 * (uint64_t)rd16(f->loca, 2 * (uint64_t)gid + 2); }
    if (z <= a || z > f->glyf.n) return none;
    return sub(f->glyf, a, z - a);
}

typedef struct { float m[6]; } tf_xf;   /* x' = m0 x + m2 y + m4 ; y' = m1 x + m3 y + m5 */
static void xf_apply(const tf_xf* t, float x, float y, float* ox, float* oy) {
    *ox = t->m[0] * x + t->m[2] * y + t->m[4];
    *oy = t->m[1] * x + t->m[3] * y + t->m[5];
}

static int glyf_outline(tf_font* f, int gid, const tf_xf* t, tf_path* p, int depth) {
    tf_buf g = glyf_data(f, gid);
    int nc;
    if (depth > 8) return -1;
    if (g.n < 10) return 0;
    nc = rds16(g, 0);
    if (nc >= 0) {
        uint64_t q = 10 + 2 * (uint64_t)nc, i;
        uint32_t npts, ilen;
        uint8_t* flags; int32_t* xs; int32_t* ys;
        int32_t v = 0; int c, start;
        if (nc == 0) return 0;
        npts = rd16(g, 10 + 2 * (uint64_t)(nc - 1)) + 1;
        if (q + 2 > g.n || npts > g.n) return -1;
        ilen = rd16(g, q); q += 2 + ilen;
        flags = (uint8_t*)malloc(npts); xs = (int32_t*)malloc(npts * sizeof(int32_t)); ys = (int32_t*)malloc(npts * sizeof(int32_t));
        if (!flags || !xs || !ys) { free(flags); free(xs); free(ys); return -1; }
        for (i = 0; i < npts;) {
            uint32_t fl = rd8(g, q++), rep = 0;
            if (fl & 8) rep = rd8(g, q++);
            flags[i++] = (uint8_t)fl;
            while (rep-- && i < npts) flags[i++] = (uint8_t)fl;
        }
        for (i = 0; i < npts; i++) {
            uint8_t fl = flags[i];
            if (fl & 2) { int d = (int)rd8(g, q++); v += (fl & 16) ? d : -d; }
            else if (!(fl & 16)) { v += rds16(g, q); q += 2; }
            xs[i] = v;
        }
        v = 0;
        for (i = 0; i < npts; i++) {
            uint8_t fl = flags[i];
            if (fl & 4) { int d = (int)rd8(g, q++); v += (fl & 32) ? d : -d; }
            else if (!(fl & 32)) { v += rds16(g, q); q += 2; }
            ys[i] = v;
        }
        start = 0;
        for (c = 0; c < nc; c++) {
            int end = (int)rd16(g, 10 + 2 * (uint64_t)c), n, k, first;
            float px, py, sx0, sy0;
            if (end < start || end >= (int)npts) break;
            n = end - start + 1;
            /* find a starting on-curve point (or synthesize the midpoint) */
            first = -1;
            for (k = 0; k < n; k++) if (flags[start + k] & 1) { first = k; break; }
            if (first < 0) {
                float ax, ay, bx, by;
                xf_apply(t, (float)xs[start], (float)ys[start], &ax, &ay);
                xf_apply(t, (float)xs[end], (float)ys[end], &bx, &by);
                sx0 = (ax + bx) / 2; sy0 = (ay + by) / 2; first = 0;
                path_move(p, sx0, sy0);
                /* all off-curve: walk from point 0 */
                {
                    float cxo = ax, cyo = ay;
                    for (k = 1; k <= n; k++) {
                        int idx = start + (k % n);
                        float nx, ny;
                        xf_apply(t, (float)xs[idx], (float)ys[idx], &nx, &ny);
                        if (k == n) { path_quad(p, cxo, cyo, sx0, sy0); break; }
                        path_quad(p, cxo, cyo, (cxo + nx) / 2, (cyo + ny) / 2);
                        cxo = nx; cyo = ny;
                    }
                }
                path_close(p);
                start = end + 1;
                continue;
            }
            xf_apply(t, (float)xs[start + first], (float)ys[start + first], &sx0, &sy0);
            path_move(p, sx0, sy0);
            {
                int have_ctrl = 0; float cxo = 0, cyo = 0;
                for (k = 1; k <= n; k++) {
                    int idx = start + ((first + k) % n);
                    xf_apply(t, (float)xs[idx], (float)ys[idx], &px, &py);
                    if (flags[idx] & 1) {
                        if (have_ctrl) path_quad(p, cxo, cyo, px, py); else path_line(p, px, py);
                        have_ctrl = 0;
                    } else {
                        if (have_ctrl) path_quad(p, cxo, cyo, (cxo + px) / 2, (cyo + py) / 2);
                        cxo = px; cyo = py; have_ctrl = 1;
                    }
                }
                if (have_ctrl) path_quad(p, cxo, cyo, sx0, sy0);
            }
            path_close(p);
            start = end + 1;
        }
        free(flags); free(xs); free(ys);
        return 0;
    } else {
        uint64_t q = 10; uint32_t fl; int guard = 0;
        do {
            int cg; float a = 1, b = 0, c2 = 0, d = 1, e = 0, ff = 0; tf_xf t2;
            fl = rd16(g, q); cg = (int)rd16(g, q + 2); q += 4;
            if (q > g.n) return -1;
            if (fl & 1) { e = (float)rds16(g, q); ff = (float)rds16(g, q + 2); q += 4; }
            else { e = (float)(int8_t)rd8(g, q); ff = (float)(int8_t)rd8(g, q + 1); q += 2; }
            if (!(fl & 2)) { e = 0; ff = 0; }   /* point matching: unsupported */
            if (fl & 8) { a = d = (float)rds16(g, q) / 16384.0f; q += 2; }
            else if (fl & 0x40) { a = (float)rds16(g, q) / 16384.0f; d = (float)rds16(g, q + 2) / 16384.0f; q += 4; }
            else if (fl & 0x80) { a = (float)rds16(g, q) / 16384.0f; b = (float)rds16(g, q + 2) / 16384.0f; c2 = (float)rds16(g, q + 4) / 16384.0f; d = (float)rds16(g, q + 6) / 16384.0f; q += 8; }
            /* compose: t ∘ component */
            t2.m[0] = t->m[0] * a + t->m[2] * b; t2.m[1] = t->m[1] * a + t->m[3] * b;
            t2.m[2] = t->m[0] * c2 + t->m[2] * d; t2.m[3] = t->m[1] * c2 + t->m[3] * d;
            xf_apply(t, e, ff, &t2.m[4], &t2.m[5]);
            if (cg != gid && glyf_outline(f, cg, &t2, p, depth + 1) < 0) return -1;
            if (++guard > 256) break;
        } while (fl & 0x20);
        return 0;
    }
}

/* ── Type 2 charstrings ──────────────────────────────────────────────────── */
typedef struct {
    tf_font* f; tf_path* p; double st[520]; int n; int nstems; int width_done; double x, y;
    tf_index* lsub; int vsindex; int depth; int done; double trans[32];
} cs_ctx;

static void cs_rmove(cs_ctx* c, double dx, double dy) { c->x += dx; c->y += dy; path_move(c->p, (float)c->x, (float)c->y); }
static void cs_rline(cs_ctx* c, double dx, double dy) { c->x += dx; c->y += dy; path_line(c->p, (float)c->x, (float)c->y); }
static void cs_rcurve(cs_ctx* c, double a, double b, double d, double e, double g, double h) {
    double x1 = c->x + a, y1 = c->y + b, x2 = x1 + d, y2 = y1 + e;
    c->x = x2 + g; c->y = y2 + h;
    path_cubic(c->p, (float)x1, (float)y1, (float)x2, (float)y2, (float)c->x, (float)c->y);
}

static int cs_run(cs_ctx* c, tf_buf s) {
    uint64_t i = 0;
    if (++c->depth > 12) return -1;
    while (i < s.n && !c->done) {
        uint32_t b = rd8(s, i);
        int k;
        if (b >= 32 || b == 28) {
            double v;
            if (b == 28) { v = (int16_t)rd16(s, i + 1); i += 3; }
            else if (b <= 246) { v = (int)b - 139; i++; }
            else if (b <= 250) { v = ((int)b - 247) * 256 + (int)rd8(s, i + 1) + 108; i += 2; }
            else if (b <= 254) { v = -((int)b - 251) * 256 - (int)rd8(s, i + 1) - 108; i += 2; }
            else { v = (int32_t)rd32(s, i + 1) / 65536.0; i += 5; }
            if (c->n < 513) c->st[c->n++] = v; else return -1;
            continue;
        }
        i++;
        switch (b) {
        case 1: case 3: case 18: case 23:          /* stems */
            if (!c->f->is_cff2 && !c->width_done && (c->n & 1)) { memmove(c->st, c->st + 1, sizeof(double) * (size_t)(c->n - 1)); c->n--; }
            c->width_done = 1; c->nstems += c->n / 2; c->n = 0; break;
        case 19: case 20:                          /* hintmask / cntrmask */
            if (!c->f->is_cff2 && !c->width_done && (c->n & 1)) { memmove(c->st, c->st + 1, sizeof(double) * (size_t)(c->n - 1)); c->n--; }
            c->width_done = 1; c->nstems += c->n / 2; c->n = 0;
            i += (uint64_t)(c->nstems + 7) / 8; break;
        case 21:
            k = 0; if (!c->f->is_cff2 && !c->width_done && c->n > 2) k = 1;
            c->width_done = 1; if (c->n - k < 2) return -1;
            cs_rmove(c, c->st[k], c->st[k + 1]); c->n = 0; break;
        case 22:
            k = 0; if (!c->f->is_cff2 && !c->width_done && c->n > 1) k = 1;
            c->width_done = 1; if (c->n - k < 1) return -1;
            cs_rmove(c, c->st[k], 0); c->n = 0; break;
        case 4:
            k = 0; if (!c->f->is_cff2 && !c->width_done && c->n > 1) k = 1;
            c->width_done = 1; if (c->n - k < 1) return -1;
            cs_rmove(c, 0, c->st[k]); c->n = 0; break;
        case 5:
            for (k = 0; k + 1 < c->n; k += 2) cs_rline(c, c->st[k], c->st[k + 1]);
            c->n = 0; break;
        case 6: case 7: {
            int hz = b == 6;
            for (k = 0; k < c->n; k++) { if (hz) cs_rline(c, c->st[k], 0); else cs_rline(c, 0, c->st[k]); hz = !hz; }
            c->n = 0; break;
        }
        case 8:
            for (k = 0; k + 5 < c->n; k += 6) cs_rcurve(c, c->st[k], c->st[k + 1], c->st[k + 2], c->st[k + 3], c->st[k + 4], c->st[k + 5]);
            c->n = 0; break;
        case 24:
            for (k = 0; k + 5 < c->n - 2; k += 6) cs_rcurve(c, c->st[k], c->st[k + 1], c->st[k + 2], c->st[k + 3], c->st[k + 4], c->st[k + 5]);
            if (k + 1 < c->n) cs_rline(c, c->st[k], c->st[k + 1]);
            c->n = 0; break;
        case 25:
            for (k = 0; k + 1 < c->n - 6; k += 2) cs_rline(c, c->st[k], c->st[k + 1]);
            if (k + 5 < c->n) cs_rcurve(c, c->st[k], c->st[k + 1], c->st[k + 2], c->st[k + 3], c->st[k + 4], c->st[k + 5]);
            c->n = 0; break;
        case 26: {                                  /* vvcurveto */
            double dx1 = 0; k = 0;
            if (c->n & 1) { dx1 = c->st[0]; k = 1; }
            for (; k + 3 < c->n; k += 4) { cs_rcurve(c, dx1, c->st[k], c->st[k + 1], c->st[k + 2], 0, c->st[k + 3]); dx1 = 0; }
            c->n = 0; break;
        }
        case 27: {                                  /* hhcurveto */
            double dy1 = 0; k = 0;
            if (c->n & 1) { dy1 = c->st[0]; k = 1; }
            for (; k + 3 < c->n; k += 4) { cs_rcurve(c, c->st[k], dy1, c->st[k + 1], c->st[k + 2], c->st[k + 3], 0); dy1 = 0; }
            c->n = 0; break;
        }
        case 30: case 31: {                         /* vhcurveto / hvcurveto */
            int h = b == 31;
            k = 0;
            while (k + 3 < c->n) {
                double last = (c->n - k == 5) ? c->st[k + 4] : 0;
                if (h) cs_rcurve(c, c->st[k], 0, c->st[k + 1], c->st[k + 2], last, c->st[k + 3]);
                else cs_rcurve(c, 0, c->st[k], c->st[k + 1], c->st[k + 2], c->st[k + 3], last);
                k += 4; h = !h;
            }
            c->n = 0; break;
        }
        case 10: case 29: {                         /* callsubr / callgsubr */
            tf_index* x = b == 10 ? c->lsub : &c->f->gsubrs;
            int idx; tf_buf sb;
            if (c->n < 1 || !x || x->count == 0) return -1;
            idx = (int)c->st[--c->n] + cff_subr_bias(x->count);
            if (idx < 0 || (uint32_t)idx >= x->count) return -1;
            sb = idx_get(x, (uint32_t)idx);
            if (cs_run(c, sb) < 0) return -1;
            break;
        }
        case 11: c->depth--; return 0;
        case 14:
            if (!c->f->is_cff2) { path_close(c->p); c->done = 1; }
            c->n = 0; break;
        case 15: if (c->n >= 1) c->vsindex = (int)c->st[0]; c->n = 0; break;
        case 16: {                                  /* blend: keep default values */
            int nb, regions = 0;
            if (c->n < 1) return -1;
            nb = (int)c->st[--c->n];
            if (c->vsindex >= 0 && c->vsindex < c->f->nvs) regions = c->f->vs_regions[c->vsindex];
            if (nb < 0 || (int64_t)nb * (regions + 1) > c->n) return -1;
            c->n -= nb * regions;
            break;
        }
        case 12: {
            uint32_t e = rd8(s, i++);
            double* a = c->st;
            switch (e) {
            case 35:
                if (c->n >= 12) { cs_rcurve(c, a[0], a[1], a[2], a[3], a[4], a[5]); cs_rcurve(c, a[6], a[7], a[8], a[9], a[10], a[11]); }
                break;
            case 34:
                if (c->n >= 7) { double y0 = c->y; cs_rcurve(c, a[0], 0, a[1], a[2], a[3], 0); cs_rcurve(c, a[4], 0, a[5], y0 - c->y, a[6], 0); }
                break;
            case 36:
                if (c->n >= 9) { double y0 = c->y; cs_rcurve(c, a[0], a[1], a[2], a[3], a[4], 0); cs_rcurve(c, a[5], 0, a[6], a[7], a[8], y0 - c->y - a[7]); }
                break;
            case 37:
                if (c->n >= 11) {
                    double x0 = c->x, y0 = c->y, dx = 0, dy = 0; int j;
                    for (j = 0; j < 10; j += 2) { dx += a[j]; dy += a[j + 1]; }
                    cs_rcurve(c, a[0], a[1], a[2], a[3], a[4], a[5]);
                    if (fabs(dx) > fabs(dy)) cs_rcurve(c, a[6], a[7], a[8], a[9], a[10], (y0) - (c->y + a[7] + a[9]));
                    else cs_rcurve(c, a[6], a[7], a[8], a[9], (x0) - (c->x + a[6] + a[8]), a[10]);
                }
                break;
            case 9: if (c->n >= 1) a[c->n - 1] = fabs(a[c->n - 1]); continue;
            case 10: if (c->n >= 2) { a[c->n - 2] += a[c->n - 1]; c->n--; } continue;
            case 11: if (c->n >= 2) { a[c->n - 2] -= a[c->n - 1]; c->n--; } continue;
            case 12: if (c->n >= 2) { a[c->n - 2] = a[c->n - 1] != 0 ? a[c->n - 2] / a[c->n - 1] : 0; c->n--; } continue;
            case 14: if (c->n >= 1) a[c->n - 1] = -a[c->n - 1]; continue;
            case 18: if (c->n >= 1) c->n--; continue;
            case 24: if (c->n >= 2) { a[c->n - 2] *= a[c->n - 1]; c->n--; } continue;
            case 26: if (c->n >= 1) a[c->n - 1] = a[c->n - 1] > 0 ? sqrt(a[c->n - 1]) : 0; continue;
            case 27: if (c->n >= 1 && c->n < 513) { a[c->n] = a[c->n - 1]; c->n++; } continue;
            case 28: if (c->n >= 2) { double t = a[c->n - 1]; a[c->n - 1] = a[c->n - 2]; a[c->n - 2] = t; } continue;
            case 3: if (c->n >= 2) { a[c->n - 2] = (a[c->n - 2] != 0 && a[c->n - 1] != 0); c->n--; } continue;
            case 4: if (c->n >= 2) { a[c->n - 2] = (a[c->n - 2] != 0 || a[c->n - 1] != 0); c->n--; } continue;
            case 5: if (c->n >= 1) a[c->n - 1] = (a[c->n - 1] == 0); continue;
            case 15: if (c->n >= 2) { a[c->n - 2] = (a[c->n - 2] == a[c->n - 1]); c->n--; } continue;
            case 22: if (c->n >= 4) { a[c->n - 4] = a[c->n - 2] <= a[c->n - 1] ? a[c->n - 4] : a[c->n - 3]; c->n -= 3; } continue;
            case 23: if (c->n < 513) a[c->n++] = 0.5; continue;
            case 20: if (c->n >= 2) { int j = (int)a[c->n - 1]; if (j >= 0 && j < 32) c->trans[j] = a[c->n - 2]; c->n -= 2; } continue;
            case 21: if (c->n >= 1) { int j = (int)a[c->n - 1]; a[c->n - 1] = (j >= 0 && j < 32) ? c->trans[j] : 0; } continue;
            case 29: if (c->n >= 1) { int j = (int)a[c->n - 1]; if (j < 0) j = 0; a[c->n - 1] = (j < c->n - 1) ? a[c->n - 2 - j] : 0; } continue;
            case 30:
                if (c->n >= 2) {
                    int nn = (int)a[c->n - 2], jj = (int)a[c->n - 1], q2;
                    c->n -= 2;
                    if (nn > 0 && nn <= c->n) {
                        double tmp[520]; double* base = a + c->n - nn;
                        jj %= nn; if (jj < 0) jj += nn;
                        for (q2 = 0; q2 < nn; q2++) tmp[(q2 + jj) % nn] = base[q2];
                        memcpy(base, tmp, sizeof(double) * (size_t)nn);
                    }
                }
                continue;
            default: break;
            }
            c->n = 0; break;
        }
        default: c->n = 0; break;     /* reserved */
        }
    }
    c->depth--;
    return 0;
}

static int cff_outline(tf_font* f, int gid, tf_path* p) {
    cs_ctx c; tf_buf s;
    if (gid < 0 || (uint32_t)gid >= f->cs.count) return 0;
    memset(&c, 0, sizeof c);
    c.f = f; c.p = p; c.lsub = &f->lsubrs;
    if (f->cid) { int fd = cff_fd_for(f, gid); if (fd < 0 || fd >= f->nfd) fd = 0; c.lsub = &f->fd_lsubrs[fd]; }
    s = idx_get(&f->cs, (uint32_t)gid);
    if (cs_run(&c, s) < 0) return -1;
    path_close(p);
    return 0;
}

/* Outline of glyph `gid` in font units (y up). */
static int tf_outline(tf_font* f, int gid, tf_path* p) {
    int r;
    memset(p, 0, sizeof *p);
    if (gid < 0 || gid >= f->nglyphs) return 0;
    if (f->is_cff) r = cff_outline(f, gid, p);
    else { tf_xf t = { { 1, 0, 0, 1, 0, 0 } }; r = glyf_outline(f, gid, &t, p, 0); path_close(p); }
    if (p->bad) r = -1;
    return r;
}

/* ── cmap ────────────────────────────────────────────────────────────────── */
static void tf_pick_cmap(tf_font* f) {
    uint32_t n = rd16(f->cmap, 2), i;
    int best = -1, bestfmt = 0; tf_buf bestb = { NULL, 0 };
    for (i = 0; i < n && i < 512; i++) {
        uint32_t pid = rd16(f->cmap, 4 + 8 * (uint64_t)i), eid = rd16(f->cmap, 6 + 8 * (uint64_t)i), off = rd32(f->cmap, 8 + 8 * (uint64_t)i);
        tf_buf s = subfrom(f->cmap, off);
        int fmt = (int)rd16(s, 0), score = -1;
        if (pid == 0 && eid == 5 && fmt == 14) { f->uvs = s; continue; }
        if (fmt != 0 && fmt != 4 && fmt != 6 && fmt != 10 && fmt != 12 && fmt != 13) continue;
        if (pid == 3 && eid == 10) score = 9;
        else if (pid == 0 && (eid == 4 || eid == 6)) score = 8;
        else if (pid == 3 && eid == 1) score = 7;
        else if (pid == 0) score = 6;
        else if (pid == 3 && eid == 0) score = 5;
        else if (pid == 1 && eid == 0) score = 2;
        if (fmt == 12 || fmt == 10) score += 0; else if (fmt == 13) score -= 3;
        if (score > best) { best = score; bestb = s; bestfmt = fmt; }
    }
    f->cmap_sub = bestb; f->cmap_fmt = best >= 0 ? bestfmt : -1;
    /* a symbol (3,0) cmap maps U+F0xx; remember so we can try that range */
    if (best == 5) f->cmap_fmt |= 0x100;
}

static int cmap_lookup(tf_font* f, uint32_t cp) {
    tf_buf s = f->cmap_sub; int fmt = f->cmap_fmt & 0xFF;
    if (f->cmap_fmt < 0) return 0;
    if (fmt == 0) return cp < 256 ? (int)rd8(s, 6 + cp) : 0;
    if (fmt == 6) {
        uint32_t first = rd16(s, 6), cnt = rd16(s, 8);
        if (cp >= first && cp < first + cnt) return (int)rd16(s, 10 + 2 * (uint64_t)(cp - first));
        return 0;
    }
    if (fmt == 10) {
        uint32_t first = rd32(s, 12), cnt = rd32(s, 16);
        if (cp >= first && (uint64_t)cp < (uint64_t)first + cnt) return (int)rd16(s, 20 + 2 * (uint64_t)(cp - first));
        return 0;
    }
    if (fmt == 4) {
        uint32_t segx2 = rd16(s, 6), lo = 0, hi = segx2 / 2;
        if (cp > 0xFFFF) return 0;
        while (lo < hi) {
            uint32_t mid = (lo + hi) / 2;
            uint32_t end = rd16(s, 14 + 2 * (uint64_t)mid);
            if (end < cp) lo = mid + 1; else hi = mid;
        }
        if (lo < segx2 / 2) {
            uint64_t eo = 14, so = 16 + segx2, dlo = so + segx2, ro = dlo + segx2;
            uint32_t end = rd16(s, eo + 2 * (uint64_t)lo), start = rd16(s, so + 2 * (uint64_t)lo);
            uint32_t delta = rd16(s, dlo + 2 * (uint64_t)lo), ro_v = rd16(s, ro + 2 * (uint64_t)lo);
            if (cp < start || cp > end) return 0;
            if (ro_v == 0) return (int)((cp + delta) & 0xFFFF);
            {
                uint64_t a = ro + 2 * (uint64_t)lo + ro_v + 2 * (uint64_t)(cp - start);
                uint32_t g = rd16(s, a);
                return g ? (int)((g + delta) & 0xFFFF) : 0;
            }
        }
        return 0;
    }
    if (fmt == 12 || fmt == 13) {
        uint32_t n = rd32(s, 12), lo = 0, hi = n;
        if ((uint64_t)n * 12 + 16 > s.n) { n = (s.n - 16) / 12; hi = n; }
        while (lo < hi) {
            uint32_t mid = lo + (hi - lo) / 2;
            uint32_t a = rd32(s, 16 + 12 * (uint64_t)mid), z = rd32(s, 20 + 12 * (uint64_t)mid);
            if (cp < a) hi = mid;
            else if (cp > z) lo = mid + 1;
            else {
                uint32_t g = rd32(s, 24 + 12 * (uint64_t)mid);
                return fmt == 12 ? (int)(g + (cp - a)) : (int)g;
            }
        }
    }
    return 0;
}

static int tf_glyph_index(tf_font* f, uint32_t cp) {
    int g = cmap_lookup(f, cp);
    if (g == 0 && (f->cmap_fmt & 0x100) && cp < 0x100) g = cmap_lookup(f, 0xF000 + cp);
    if (g < 0 || g >= f->nglyphs) g = 0;
    return g;
}

/* Variation sequence lookup (cmap format 14); -1 = no special mapping. */
static int tf_glyph_index_vs(tf_font* f, uint32_t cp, uint32_t vs) {
    tf_buf s = f->uvs; uint32_t n = rd32(s, 6), i;
    if (s.n == 0) return -1;
    for (i = 0; i < n && i < 512; i++) {
        uint64_t r = 10 + 11 * (uint64_t)i;
        if (rd24(s, r) != vs) continue;
        {
            uint32_t defo = rd32(s, r + 3), ndo = rd32(s, r + 7), k;
            if (defo) {
                tf_buf d = subfrom(s, defo); uint32_t cnt = rd32(d, 0);
                for (k = 0; k < cnt && k < 1000000; k++) {
                    uint32_t st = rd24(d, 4 + 4 * (uint64_t)k), add = rd8(d, 7 + 4 * (uint64_t)k);
                    if ((uint64_t)4 + 4 * (uint64_t)k + 4 > d.n) break;
                    if (cp >= st && cp <= st + add) return tf_glyph_index(f, cp);
                }
            }
            if (ndo) {
                tf_buf d = subfrom(s, ndo); uint32_t cnt = rd32(d, 0);
                for (k = 0; k < cnt && k < 1000000; k++) {
                    if ((uint64_t)4 + 5 * (uint64_t)k + 5 > d.n) break;
                    if (rd24(d, 4 + 5 * (uint64_t)k) == cp) { int g = (int)rd16(d, 7 + 5 * (uint64_t)k); return g < f->nglyphs ? g : 0; }
                }
            }
        }
    }
    return -1;
}

/* ── metrics ─────────────────────────────────────────────────────────────── */
static int tf_advance(tf_font* f, int gid) {
    if (f->nhm <= 0 || gid < 0 || gid >= f->nglyphs) return 0;
    if (gid >= f->nhm) gid = f->nhm - 1;
    return (int)rd16(f->hmtx, 4 * (uint64_t)gid);
}
static int tf_lsb(tf_font* f, int gid) {
    if (f->nhm <= 0 || gid < 0 || gid >= f->nglyphs) return 0;
    if (gid < f->nhm) return rds16(f->hmtx, 4 * (uint64_t)gid + 2);
    return rds16(f->hmtx, 4 * (uint64_t)f->nhm + 2 * (uint64_t)(gid - f->nhm));
}

/* ── OpenType layout helpers ─────────────────────────────────────────────── */
static int ot_coverage(tf_buf c, int gid) {
    int fmt = (int)rd16(c, 0);
    if (fmt == 1) {
        uint32_t n = rd16(c, 2), lo = 0, hi = n;
        while (lo < hi) {
            uint32_t m = (lo + hi) / 2, g = rd16(c, 4 + 2 * (uint64_t)m);
            if (g == (uint32_t)gid) return (int)m;
            if (g < (uint32_t)gid) lo = m + 1; else hi = m;
        }
    } else if (fmt == 2) {
        uint32_t n = rd16(c, 2), lo = 0, hi = n;
        while (lo < hi) {
            uint32_t m = (lo + hi) / 2, a = rd16(c, 4 + 6 * (uint64_t)m), z = rd16(c, 6 + 6 * (uint64_t)m);
            if ((uint32_t)gid < a) hi = m;
            else if ((uint32_t)gid > z) lo = m + 1;
            else return (int)(rd16(c, 8 + 6 * (uint64_t)m) + (uint32_t)gid - a);
        }
    }
    return -1;
}
static int ot_class(tf_buf c, int gid) {
    int fmt = (int)rd16(c, 0);
    if (fmt == 1) {
        uint32_t st = rd16(c, 2), n = rd16(c, 4);
        if ((uint32_t)gid >= st && (uint32_t)gid < st + n) return (int)rd16(c, 6 + 2 * (uint64_t)(gid - st));
    } else if (fmt == 2) {
        uint32_t n = rd16(c, 2), lo = 0, hi = n;
        while (lo < hi) {
            uint32_t m = (lo + hi) / 2, a = rd16(c, 4 + 6 * (uint64_t)m), z = rd16(c, 6 + 6 * (uint64_t)m);
            if ((uint32_t)gid < a) hi = m;
            else if ((uint32_t)gid > z) lo = m + 1;
            else return (int)rd16(c, 8 + 6 * (uint64_t)m);
        }
    }
    return 0;
}
static int vr_size(uint32_t fmt) { int n = 0; while (fmt) { n += (int)(fmt & 1); fmt >>= 1; } return 2 * n; }

/* Collect subtable offsets (within table t) of lookups of `type` referenced by
 * the feature `feat` (any script). Extension lookups are resolved. */
static void ot_collect(tf_buf t, const char* feat, int type, int ext_type, uint32_t** out, int* nout) {
    tf_buf fl = subfrom(t, rd16(t, 6)), ll = subfrom(t, rd16(t, 8));
    uint32_t nf = rd16(fl, 0), nl = rd16(ll, 0), i, k;
    uint8_t* want; int cap = 0;
    if (t.n < 10 || nl == 0) return;
    want = (uint8_t*)calloc(nl, 1);
    if (!want) return;
    for (i = 0; i < nf; i++) {
        if (rd32(fl, 2 + 6 * (uint64_t)i) != (uint32_t)tf_tag(feat)) continue;
        {
            tf_buf ft = subfrom(fl, rd16(fl, 6 + 6 * (uint64_t)i));
            uint32_t cnt = rd16(ft, 2);
            for (k = 0; k < cnt; k++) { uint32_t li = rd16(ft, 4 + 2 * (uint64_t)k); if (li < nl) want[li] = 1; }
        }
    }
    for (i = 0; i < nl; i++) {
        uint32_t lo, lt, ns;
        if (!want[i]) continue;
        lo = rd16(ll, 2 + 2 * (uint64_t)i);
        {
            tf_buf lk = subfrom(ll, lo);
            uint64_t lbase = (uint64_t)(lk.p - t.p);
            lt = rd16(lk, 0); ns = rd16(lk, 4);
            for (k = 0; k < ns; k++) {
                uint64_t so = lbase + rd16(lk, 6 + 2 * (uint64_t)k);
                uint32_t typ = lt;
                if (lt == (uint32_t)ext_type) { typ = rd16(t, so + 2); so += rd32(t, so + 4); }
                if (typ != (uint32_t)type || so >= t.n) continue;
                if (*nout >= cap) {
                    uint32_t* q; cap = cap ? cap * 2 : 16;
                    q = (uint32_t*)realloc(*out, sizeof(uint32_t) * (size_t)cap);
                    if (!q) { free(want); return; }
                    *out = q;
                }
                (*out)[(*nout)++] = (uint32_t)so;
                if (*nout > 4096) { free(want); return; }
            }
        }
    }
    free(want);
}

/* GPOS pair adjustment of the first glyph's x advance, in font units. */
static int gpos_pair(tf_font* f, int g1, int g2, int* found) {
    int i;
    *found = 0;
    for (i = 0; i < f->nkern_sub; i++) {
        tf_buf s = subfrom(f->gpos, f->kern_sub[i]);
        int fmt = (int)rd16(s, 0), ci;
        uint32_t vf1 = rd16(s, 4), vf2 = rd16(s, 6);
        int s1 = vr_size(vf1), s2 = vr_size(vf2), xadv_off = -1;
        ci = ot_coverage(subfrom(s, rd16(s, 2)), g1);
        if (ci < 0) continue;
        if (vf1 & 4) xadv_off = vr_size(vf1 & 3);
        if (fmt == 1) {
            uint32_t nsets = rd16(s, 8);
            if ((uint32_t)ci >= nsets) continue;
            {
                tf_buf ps = subfrom(s, rd16(s, 10 + 2 * (uint64_t)ci));
                uint32_t n = rd16(ps, 0), lo = 0, hi = n, rec = 2 + (uint32_t)s1 + (uint32_t)s2;
                while (lo < hi) {
                    uint32_t m = (lo + hi) / 2, g = rd16(ps, 2 + (uint64_t)rec * m);
                    if (g == (uint32_t)g2) {
                        *found = 1;
                        return xadv_off >= 0 ? rds16(ps, 2 + (uint64_t)rec * m + 2 + (uint64_t)xadv_off) : 0;
                    }
                    if (g < (uint32_t)g2) lo = m + 1; else hi = m;
                }
            }
        } else if (fmt == 2) {
            tf_buf cd1 = subfrom(s, rd16(s, 8)), cd2 = subfrom(s, rd16(s, 10));
            uint32_t n1 = rd16(s, 12), n2 = rd16(s, 14);
            uint32_t c1 = (uint32_t)ot_class(cd1, g1), c2 = (uint32_t)ot_class(cd2, g2);
            if (c1 < n1 && c2 < n2) {
                uint64_t rec = 16 + ((uint64_t)c1 * n2 + c2) * (uint64_t)(s1 + s2);
                *found = 1;
                return xadv_off >= 0 ? rds16(s, rec + (uint64_t)xadv_off) : 0;
            }
        }
    }
    return 0;
}

static int tf_kerning(tf_font* f, int g1, int g2) {
    int found = 0, v;
    if (f->nkern_sub > 0) { v = gpos_pair(f, g1, g2, &found); if (found) return v; }
    if (f->kern0_n > 0) {
        uint32_t key = ((uint32_t)g1 << 16) | (uint32_t)g2, lo = 0, hi = f->kern0_n;
        while (lo < hi) {
            uint32_t m = (lo + hi) / 2, k = rd32(f->kern0, 6 * (uint64_t)m);
            if (k == key) return rds16(f->kern0, 6 * (uint64_t)m + 4);
            if (k < key) lo = m + 1; else hi = m;
        }
    }
    return 0;
}

/* GSUB ligature at position i of gids[0..n): returns the ligature glyph and
 * the number of components consumed (>= 2), or 0. */
static int gsub_liga(tf_font* f, const int* gids, int n, int i, int* used) {
    int j;
    for (j = 0; j < f->nliga_sub; j++) {
        tf_buf s = subfrom(f->gsub, f->liga_sub[j]);
        int ci = ot_coverage(subfrom(s, rd16(s, 2)), gids[i]);
        uint32_t nsets = rd16(s, 4), nl, k;
        tf_buf ls;
        if (ci < 0 || (uint32_t)ci >= nsets || rd16(s, 0) != 1) continue;
        ls = subfrom(s, rd16(s, 6 + 2 * (uint64_t)ci));
        nl = rd16(ls, 0);
        for (k = 0; k < nl; k++) {
            tf_buf lg = subfrom(ls, rd16(ls, 2 + 2 * (uint64_t)k));
            uint32_t lig = rd16(lg, 0), cc = rd16(lg, 2), m; int ok = 1;
            if (cc < 2 || i + (int)cc > n) continue;
            for (m = 1; m < cc; m++) if (rd16(lg, 4 + 2 * (uint64_t)(m - 1)) != (uint32_t)gids[i + (int)m]) { ok = 0; break; }
            if (ok && lig < (uint32_t)f->nglyphs) { *used = (int)cc; return (int)lig; }
        }
    }
    return 0;
}

/* ── loading ─────────────────────────────────────────────────────────────── */
static void tf_free_font(tf_font* f) {
    if (!f) return;
    free(f->data); free(f->kern_sub); free(f->liga_sub); free(f->sg); free(f);
}

int64_t trc_font_load(const char* data, int64_t n, int64_t index) {
    tf_font* f = (tf_font*)calloc(1, sizeof(tf_font));
    uint32_t magic, off = 0;
    if (!f) return 0;
    if (n < 12 || n > 0x7FFFFFFF) { strcpy(f->err, "font: data too short"); return (int64_t)(intptr_t)f; }
    magic = ((uint32_t)(uint8_t)data[0] << 24) | ((uint32_t)(uint8_t)data[1] << 16) | ((uint32_t)(uint8_t)data[2] << 8) | (uint8_t)data[3];
    if (magic == 0x774F4646) {          /* wOFF */
        uint32_t L = 0;
        f->data = tf_unwoff((const uint8_t*)data, (uint32_t)n, &L, f->err);
        if (!f->data) return (int64_t)(intptr_t)f;
        f->len = L;
    } else if (magic == 0x774F4632) {   /* wOF2 */
        strcpy(f->err, "font: WOFF2 is not supported");
        return (int64_t)(intptr_t)f;
    } else {
        f->data = (uint8_t*)malloc((size_t)n);
        if (!f->data) { strcpy(f->err, "font: out of memory"); return (int64_t)(intptr_t)f; }
        memcpy(f->data, data, (size_t)n); f->len = (uint32_t)n;
    }
    f->all.p = f->data; f->all.n = f->len;
    magic = rd32(f->all, 0);
    f->nfonts = 1;
    if (magic == 0x74746366) {          /* ttcf */
        uint32_t nf = rd32(f->all, 8);
        f->nfonts = (int)(nf > 65535 ? 65535 : nf);
        if (index < 0 || index >= (int64_t)nf) { strcpy(f->err, "font: collection index out of range"); return (int64_t)(intptr_t)f; }
        off = rd32(f->all, 12 + 4 * (uint64_t)index);
        magic = rd32(f->all, off);
    } else if (index != 0) { strcpy(f->err, "font: not a collection (index must be 0)"); return (int64_t)(intptr_t)f; }
    if (magic != 0x00010000 && magic != 0x74727565 && magic != 0x4F54544F) { strcpy(f->err, "font: not a TrueType/OpenType font"); return (int64_t)(intptr_t)f; }
    f->sf = subfrom(f->all, off);
    if (f->sf.n < 12 + 16 * (uint64_t)rd16(f->sf, 4)) { strcpy(f->err, "font: truncated table directory"); return (int64_t)(intptr_t)f; }
    f->head = tf_table(f, "head"); f->hhea = tf_table(f, "hhea"); f->maxp = tf_table(f, "maxp");
    f->hmtx = tf_table(f, "hmtx"); f->cmap = tf_table(f, "cmap"); f->name = tf_table(f, "name");
    f->os2 = tf_table(f, "OS/2"); f->post = tf_table(f, "post"); f->loca = tf_table(f, "loca");
    f->glyf = tf_table(f, "glyf"); f->kern = tf_table(f, "kern"); f->gpos = tf_table(f, "GPOS");
    f->gsub = tf_table(f, "GSUB"); f->cvt = tf_table(f, "cvt "); f->fpgm = tf_table(f, "fpgm"); f->prep = tf_table(f, "prep");
    f->cff = tf_table(f, "CFF ");
    if (f->cff.n == 0) f->cff = tf_table(f, "CFF2");
    if (f->head.n < 54 || f->hhea.n < 36 || f->maxp.n < 6) { strcpy(f->err, "font: missing head/hhea/maxp table"); return (int64_t)(intptr_t)f; }
    f->upem = (int)rd16(f->head, 18);
    if (f->upem < 16 || f->upem > 16384) { strcpy(f->err, "font: bad unitsPerEm"); return (int64_t)(intptr_t)f; }
    f->xmin = rds16(f->head, 36); f->ymin = rds16(f->head, 38); f->xmax = rds16(f->head, 40); f->ymax = rds16(f->head, 42);
    f->locfmt = rds16(f->head, 50) ? 1 : 0;
    f->nglyphs = (int)rd16(f->maxp, 4);
    f->ascent = rds16(f->hhea, 4); f->descent = rds16(f->hhea, 6); f->line_gap = rds16(f->hhea, 8);
    f->nhm = (int)rd16(f->hhea, 34);
    if ((uint64_t)f->nhm * 4 > f->hmtx.n) f->nhm = (int)(f->hmtx.n / 4);
    if (f->nhm > f->nglyphs) f->nhm = f->nglyphs;
    if (f->cff.n > 0) {
        f->is_cff = 1;
        if (tf_init_cff(f) != 0) { strcpy(f->err, "font: corrupt CFF table"); return (int64_t)(intptr_t)f; }
        if ((int)f->cs.count < f->nglyphs) f->nglyphs = (int)f->cs.count;
    } else {
        uint64_t need;
        if (f->glyf.n == 0 || f->loca.n == 0) { strcpy(f->err, "font: no glyph outlines (glyf or CFF)"); return (int64_t)(intptr_t)f; }
        need = f->locfmt ? 4 * ((uint64_t)f->nglyphs + 1) : 2 * ((uint64_t)f->nglyphs + 1);
        if (need > f->loca.n) f->nglyphs = (int)(f->locfmt ? f->loca.n / 4 : f->loca.n / 2) - 1;
        if (f->nglyphs < 0) f->nglyphs = 0;
    }
    if (f->nglyphs == 0) { strcpy(f->err, "font: no glyphs"); return (int64_t)(intptr_t)f; }
    tf_pick_cmap(f);
    /* kern: version 0, first horizontal format-0 subtable */
    if (f->kern.n >= 4 && rd16(f->kern, 0) == 0) {
        uint32_t nt = rd16(f->kern, 2), i; uint64_t p = 4;
        for (i = 0; i < nt && p + 6 <= f->kern.n; i++) {
            uint32_t ln = rd16(f->kern, p + 2), cov = rd16(f->kern, p + 4);
            if ((cov >> 8) == 0 && (cov & 1) && !(cov & 4)) {
                uint32_t np = rd16(f->kern, p + 6);
                f->kern0 = sub(f->kern, p + 14, (uint64_t)np * 6);
                f->kern0_n = f->kern0.n / 6;
                break;
            }
            if (ln < 6) break;
            p += ln;
        }
    }
    if (f->gpos.n >= 10) ot_collect(f->gpos, "kern", 2, 9, &f->kern_sub, &f->nkern_sub);
    if (f->gsub.n >= 10) ot_collect(f->gsub, "liga", 4, 7, &f->liga_sub, &f->nliga_sub);
    return (int64_t)(intptr_t)f;
}

#define F(h) ((tf_font*)(intptr_t)(h))
int64_t trc_font_ok(int64_t h) { return h && F(h)->err[0] == 0; }
int64_t trc_font_error(int64_t h, char* out, int64_t cap) {
    size_t n;
    const char* e = h ? F(h)->err : "font: out of memory";
    n = strlen(e); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, e, n);
    return (int64_t)n;
}
void trc_font_free(int64_t h) { if (h) tf_free_font(F(h)); }

/* Number of fonts in a buffer (TTC count, 1 for a single font, 0 when it is
 * not a font). */
int64_t trc_font_count(const char* data, int64_t n) {
    tf_buf b; uint32_t m;
    if (n < 12) return 0;
    b.p = (const uint8_t*)data; b.n = (uint32_t)(n > 0x7FFFFFFF ? 0x7FFFFFFF : n);
    m = rd32(b, 0);
    if (m == 0x74746366) return rd32(b, 8);
    if (m == 0x00010000 || m == 0x74727565 || m == 0x4F54544F || m == 0x774F4646) return 1;
    return 0;
}

int64_t trc_font_info(int64_t h, int64_t what) {
    tf_font* f = F(h);
    switch (what) {
    case 0: return f->upem;
    case 1: return f->nglyphs;
    case 2: return f->ascent;
    case 3: return f->descent;
    case 4: return f->line_gap;
    case 5: return f->xmin;
    case 6: return f->ymin;
    case 7: return f->xmax;
    case 8: return f->ymax;
    case 9: return f->is_cff ? (f->is_cff2 ? 2 : 1) : 0;
    case 10: return f->os2.n >= 6 ? (int64_t)rd16(f->os2, 4) : 400;                    /* weight */
    case 11: return f->os2.n >= 10 ? (int64_t)rd16(f->os2, 8) : 0;                     /* fsType */
    case 12: return f->os2.n >= 74 ? (int64_t)rds16(f->os2, 68) : f->ascent;           /* typo asc */
    case 13: return f->os2.n >= 74 ? (int64_t)rds16(f->os2, 70) : f->descent;          /* typo desc */
    case 14: return f->os2.n >= 74 ? (int64_t)rds16(f->os2, 72) : f->line_gap;
    case 15: return (f->os2.n >= 90 && rd16(f->os2, 0) >= 2) ? (int64_t)rds16(f->os2, 86) : 0; /* xHeight */
    case 16: return (f->os2.n >= 90 && rd16(f->os2, 0) >= 2) ? (int64_t)rds16(f->os2, 88) : 0; /* capHeight */
    case 17: return f->post.n >= 8 ? (int64_t)(int32_t)rd32(f->post, 4) : 0;            /* italicAngle 16.16 */
    case 18: return f->post.n >= 10 ? (int64_t)rds16(f->post, 8) : 0;                    /* underlinePosition */
    case 19: return f->post.n >= 12 ? (int64_t)rds16(f->post, 10) : 0;                   /* underlineThickness */
    case 20: return f->post.n >= 16 ? (int64_t)(rd32(f->post, 12) != 0) : 0;             /* isFixedPitch */
    case 21: return f->os2.n >= 64 ? (int64_t)rd16(f->os2, 62) : 0;                      /* fsSelection */
    case 22: return f->nfonts;
    case 23: return f->nkern_sub > 0 || f->kern0_n > 0;
    case 24: return f->nliga_sub > 0;
    case 25: return f->uvs.n > 0;
    case 26: return (int64_t)rd16(f->head, 44);                                           /* macStyle */
    case 27: return f->os2.n >= 78 ? (int64_t)rd16(f->os2, 74) : 0;                      /* winAscent */
    case 28: return f->os2.n >= 78 ? (int64_t)rd16(f->os2, 76) : 0;                      /* winDescent */
    default: return 0;
    }
}

int64_t trc_font_glyph_index(int64_t h, int64_t cp) { return cp < 0 || cp > 0x10FFFF ? 0 : tf_glyph_index(F(h), (uint32_t)cp); }
int64_t trc_font_glyph_index_vs(int64_t h, int64_t cp, int64_t vs) {
    int g;
    if (cp < 0 || cp > 0x10FFFF || vs < 0 || vs > 0x10FFFF) return 0;
    g = tf_glyph_index_vs(F(h), (uint32_t)cp, (uint32_t)vs);
    return g < 0 ? tf_glyph_index(F(h), (uint32_t)cp) : g;
}
int64_t trc_font_advance(int64_t h, int64_t gid) { return tf_advance(F(h), (int)gid); }
int64_t trc_font_lsb(int64_t h, int64_t gid) { return tf_lsb(F(h), (int)gid); }
int64_t trc_font_kerning(int64_t h, int64_t g1, int64_t g2) {
    if (g1 < 0 || g2 < 0 || g1 >= F(h)->nglyphs || g2 >= F(h)->nglyphs) return 0;
    return tf_kerning(F(h), (int)g1, (int)g2);
}

/* Name table string (UTF-8) into out[cap]; returns the length (0 = absent). */
static int utf8_put(char* o, int cap, int k, uint32_t cp) {
    if (cp < 0x80) { if (k + 1 > cap) return -1; o[k] = (char)cp; return k + 1; }
    if (cp < 0x800) { if (k + 2 > cap) return -1; o[k] = (char)(0xC0 | (cp >> 6)); o[k + 1] = (char)(0x80 | (cp & 63)); return k + 2; }
    if (cp < 0x10000) { if (k + 3 > cap) return -1; o[k] = (char)(0xE0 | (cp >> 12)); o[k + 1] = (char)(0x80 | ((cp >> 6) & 63)); o[k + 2] = (char)(0x80 | (cp & 63)); return k + 3; }
    if (k + 4 > cap) return -1;
    o[k] = (char)(0xF0 | (cp >> 18)); o[k + 1] = (char)(0x80 | ((cp >> 12) & 63)); o[k + 2] = (char)(0x80 | ((cp >> 6) & 63)); o[k + 3] = (char)(0x80 | (cp & 63));
    return k + 4;
}
int64_t trc_font_name(int64_t h, int64_t id, char* out, int64_t cap) {
    tf_font* f = F(h);
    uint32_t n = rd16(f->name, 2), so = rd16(f->name, 4), i;
    int best = -1, bscore = -1, k = 0;
    for (i = 0; i < n && i < 4096; i++) {
        uint64_t r = 6 + 12 * (uint64_t)i;
        uint32_t pid = rd16(f->name, r), eid = rd16(f->name, r + 2), lang = rd16(f->name, r + 4), nid = rd16(f->name, r + 6);
        int sc = -1;
        if (nid != (uint32_t)id) continue;
        if (pid == 3 && (eid == 1 || eid == 10)) sc = lang == 0x409 ? 6 : 4;
        else if (pid == 0) sc = 5;
        else if (pid == 1 && eid == 0) sc = lang == 0 ? 3 : 1;
        if (sc > bscore) { bscore = sc; best = (int)i; }
    }
    if (best < 0) return 0;
    {
        uint64_t r = 6 + 12 * (uint64_t)best;
        uint32_t pid = rd16(f->name, r), ln = rd16(f->name, r + 8), off = rd16(f->name, r + 10);
        tf_buf s = sub(f->name, (uint64_t)so + off, ln);
        if (pid == 1) {
            uint32_t j;
            for (j = 0; j < s.n; j++) { uint32_t c = s.p[j]; int nk = utf8_put(out, (int)cap, k, c < 128 ? c : '?'); if (nk < 0) break; k = nk; }
        } else {
            uint32_t j;
            for (j = 0; j + 1 < s.n; j += 2) {
                uint32_t c = rd16(s, j);
                if (c >= 0xD800 && c < 0xDC00 && j + 3 < s.n) {
                    uint32_t lo = rd16(s, j + 2);
                    if (lo >= 0xDC00 && lo < 0xE000) { c = 0x10000 + ((c - 0xD800) << 10) + (lo - 0xDC00); j += 2; }
                }
                { int nk = utf8_put(out, (int)cap, k, c); if (nk < 0) break; k = nk; }
            }
        }
    }
    return k;
}

/* ── shaping ─────────────────────────────────────────────────────────────── */
static uint32_t utf8_next(const uint8_t* s, int64_t n, int64_t* i) {
    uint32_t c = s[*i], cp; int need, k;
    if (c < 0x80) { (*i)++; return c; }
    if (c >= 0xF0 && c < 0xF5) { need = 3; cp = c & 7; }
    else if (c >= 0xE0 && c < 0xF0) { need = 2; cp = c & 15; }
    else if (c >= 0xC2 && c < 0xE0) { need = 1; cp = c & 31; }
    else { (*i)++; return 0xFFFD; }
    if (*i + need >= n) { (*i)++; return 0xFFFD; }
    for (k = 1; k <= need; k++) {
        uint32_t cc = s[*i + k];
        if ((cc & 0xC0) != 0x80) { (*i)++; return 0xFFFD; }
        cp = (cp << 6) | (cc & 63);
    }
    *i += need + 1;
    return cp;
}
static int is_vs(uint32_t cp) { return (cp >= 0xFE00 && cp <= 0xFE0F) || (cp >= 0xE0100 && cp <= 0xE01EF); }

/* Shape UTF-8 text: flags bit0 = kerning, bit1 = ligatures.
 * Returns the glyph count; read results with trc_font_sg_*. */
int64_t trc_font_shape(int64_t h, const char* text, int64_t n, int64_t flags) {
    tf_font* f = F(h);
    const uint8_t* s = (const uint8_t*)text;
    int64_t i = 0; int cnt = 0, k;
    int* gids; int* cl; int* ce;
    f->nsg = 0;
    if (n <= 0) return 0;
    gids = (int*)malloc(sizeof(int) * (size_t)n); cl = (int*)malloc(sizeof(int) * (size_t)n); ce = (int*)malloc(sizeof(int) * (size_t)n);
    if (!gids || !cl || !ce) { free(gids); free(cl); free(ce); return 0; }
    while (i < n) {
        int64_t st = i;
        uint32_t cp = utf8_next(s, n, &i);
        if (is_vs(cp) && cnt > 0) {
            /* re-map the previous glyph through the variation sequence */
            int64_t pi = cl[cnt - 1]; uint32_t pcp = utf8_next(s, n, &pi);
            int g = tf_glyph_index_vs(f, pcp, cp);
            if (g >= 0) gids[cnt - 1] = g;
            ce[cnt - 1] = (int)i;
            continue;
        }
        if (cp == 0x200D || cp == 0x200C) {    /* ZWJ / ZWNJ: no glyph */
            if (cnt > 0) ce[cnt - 1] = (int)i;
            continue;
        }
        gids[cnt] = tf_glyph_index(f, cp); cl[cnt] = (int)st; ce[cnt] = (int)i; cnt++;
    }
    if (f->sgcap < cnt) {
        tf_sg* q = (tf_sg*)realloc(f->sg, sizeof(tf_sg) * (size_t)(cnt + 1));
        if (!q) { free(gids); free(cl); free(ce); return 0; }
        f->sg = q; f->sgcap = cnt + 1;
    }
    for (k = 0; k < cnt;) {
        int used = 1, g = gids[k];
        if ((flags & 2) && f->nliga_sub > 0) {
            int u = 0, lg = gsub_liga(f, gids, cnt, k, &u);
            if (lg > 0) { g = lg; used = u; }
        }
        f->sg[f->nsg].gid = g; f->sg[f->nsg].cluster = cl[k]; f->sg[f->nsg].cluster_end = ce[k + used - 1];
        f->sg[f->nsg].adv = tf_advance(f, g);
        f->nsg++;
        k += used;
    }
    if (flags & 1) for (k = 0; k + 1 < f->nsg; k++) f->sg[k].adv += tf_kerning(f, f->sg[k].gid, f->sg[k + 1].gid);
    free(gids); free(cl); free(ce);
    return f->nsg;
}
int64_t trc_font_sg_gid(int64_t h, int64_t i) { return i >= 0 && i < F(h)->nsg ? F(h)->sg[i].gid : 0; }
int64_t trc_font_sg_cluster(int64_t h, int64_t i) { return i >= 0 && i < F(h)->nsg ? F(h)->sg[i].cluster : 0; }
int64_t trc_font_sg_cluster_end(int64_t h, int64_t i) { return i >= 0 && i < F(h)->nsg ? F(h)->sg[i].cluster_end : 0; }
double trc_font_sg_advance(int64_t h, int64_t i) { return i >= 0 && i < F(h)->nsg ? F(h)->sg[i].adv : 0; }
/* Total advance of the last shaping, font units. */
double trc_font_sg_total(int64_t h) { double t = 0; int i; for (i = 0; i < F(h)->nsg; i++) t += F(h)->sg[i].adv; return t; }

/* ── outlines exposed to Tauraro ─────────────────────────────────────────── */
typedef struct { tf_path p; } tf_outl;
int64_t trc_font_outline(int64_t h, int64_t gid) {
    tf_outl* o = (tf_outl*)calloc(1, sizeof(tf_outl));
    if (!o) return 0;
    if (tf_outline(F(h), (int)gid, &o->p) < 0) { path_free(&o->p); }
    return (int64_t)(intptr_t)o;
}
#define O(h) ((tf_outl*)(intptr_t)(h))
int64_t trc_font_outline_ncmd(int64_t o) { return O(o)->p.ncmd; }
int64_t trc_font_outline_cmd(int64_t o, int64_t i) { return i >= 0 && i < O(o)->p.ncmd ? O(o)->p.cmd[i] : -1; }
int64_t trc_font_outline_npt(int64_t o) { return O(o)->p.npt; }
double trc_font_outline_x(int64_t o, int64_t i) { return i >= 0 && i < O(o)->p.npt ? O(o)->p.pt[i].x : 0; }
double trc_font_outline_y(int64_t o, int64_t i) { return i >= 0 && i < O(o)->p.npt ? O(o)->p.pt[i].y : 0; }
void trc_font_outline_free(int64_t o) { if (o) { path_free(&O(o)->p); free(O(o)); } }

/* Exact control box of a glyph outline (font units) into box[4]; 0 if empty. */
int64_t trc_font_glyph_box(int64_t h, int64_t gid, int64_t* box) {
    tf_path p; int i;
    float x0 = 1e30f, y0 = 1e30f, x1 = -1e30f, y1 = -1e30f;
    tf_outline(F(h), (int)gid, &p);
    for (i = 0; i < p.npt; i++) {
        if (p.pt[i].x < x0) x0 = p.pt[i].x; if (p.pt[i].x > x1) x1 = p.pt[i].x;
        if (p.pt[i].y < y0) y0 = p.pt[i].y; if (p.pt[i].y > y1) y1 = p.pt[i].y;
    }
    i = p.npt; path_free(&p);
    if (i == 0) { box[0] = box[1] = box[2] = box[3] = 0; return 0; }
    box[0] = (int64_t)floorf(x0); box[1] = (int64_t)floorf(y0); box[2] = (int64_t)ceilf(x1); box[3] = (int64_t)ceilf(y1);
    return 1;
}

/* ── rasterizer ──────────────────────────────────────────────────────────── */
typedef struct { float* a; int w, h; } tf_acc;

static void acc_line(tf_acc* r, float x0, float y0, float x1, float y1) {
    float dir, dxdy, x;
    int y, ystart, yend;
    if (y0 == y1) return;
    if (y0 < y1) dir = 1; else { float t; dir = -1; t = x0; x0 = x1; x1 = t; t = y0; y0 = y1; y1 = t; }
    dxdy = (x1 - x0) / (y1 - y0);
    x = x0;
    if (y0 < 0) { x -= y0 * dxdy; }
    ystart = y0 < 0 ? 0 : (int)y0;
    yend = (int)ceilf(y1); if (yend > r->h) yend = r->h;
    for (y = ystart; y < yend; y++) {
        float* row = r->a + (size_t)y * (size_t)(r->w + 2);
        float ytop = (float)y > y0 ? (float)y : y0;
        float ybot = (float)(y + 1) < y1 ? (float)(y + 1) : y1;
        float dy = ybot - ytop, xnext = x + dxdy * dy, d = dy * dir;
        float xa = x < xnext ? x : xnext, xb = x < xnext ? xnext : x;
        float xaf, xa_floor; int xai, xbi;
        if (xa < 0) xa = 0; if (xb < 0) xb = 0;
        if (xa > (float)r->w) xa = (float)r->w; if (xb > (float)r->w) xb = (float)r->w;
        xa_floor = floorf(xa); xai = (int)xa_floor; xbi = (int)ceilf(xb);
        if (xbi <= xai + 1) {
            float xm = 0.5f * (x + xnext);
            float xmf;
            if (xm < 0) xm = 0; if (xm > (float)r->w) xm = (float)r->w;
            xmf = xm - xa_floor;
            row[xai] += d - d * xmf;
            row[xai + 1] += d * xmf;
        } else {
            float s = 1.0f / (xb - xa), xbf, a0, am;
            int xi;
            xaf = xa - xa_floor;
            a0 = 0.5f * s * (1.0f - xaf) * (1.0f - xaf);
            xbf = xb - (float)xbi + 1.0f;
            am = 0.5f * s * xbf * xbf;
            row[xai] += d * a0;
            if (xbi == xai + 2) row[xai + 1] += d * (1.0f - a0 - am);
            else {
                float a1 = s * (1.5f - xaf), a2;
                row[xai + 1] += d * (a1 - a0);
                for (xi = xai + 2; xi < xbi - 1; xi++) row[xi] += d * s;
                a2 = a1 + (float)(xbi - xai - 3) * s;
                row[xbi - 1] += d * (1.0f - a2 - am);
            }
            if (xbi <= r->w + 1) row[xbi] += d * am;
        }
        x = xnext;
    }
}

/* Flatten a path (already in pixel space, y down) into the accumulator. */
static void acc_path(tf_acc* r, const tf_path* p) {
    int i, pi = 0; float cx = 0, cy = 0, sx = 0, sy = 0;
    for (i = 0; i < p->ncmd; i++) {
        int c = p->cmd[i];
        if (c == 0) { cx = sx = p->pt[pi].x; cy = sy = p->pt[pi].y; pi++; }
        else if (c == 1) { acc_line(r, cx, cy, p->pt[pi].x, p->pt[pi].y); cx = p->pt[pi].x; cy = p->pt[pi].y; pi++; }
        else if (c == 2) {
            tf_pt a = p->pt[pi], b = p->pt[pi + 1];
            float ddx = cx - 2 * a.x + b.x, ddy = cy - 2 * a.y + b.y, dev = ddx * ddx + ddy * ddy;
            int n = 1, k; float px = cx, py = cy;
            if (dev >= 0.333f) { n = 1 + (int)floorf(sqrtf(sqrtf(3.0f * dev))); if (n > 64) n = 64; }
            for (k = 1; k <= n; k++) {
                float t = (float)k / (float)n, mt = 1 - t;
                float nx = mt * mt * cx + 2 * mt * t * a.x + t * t * b.x, ny = mt * mt * cy + 2 * mt * t * a.y + t * t * b.y;
                acc_line(r, px, py, nx, ny); px = nx; py = ny;
            }
            cx = b.x; cy = b.y; pi += 2;
        } else if (c == 3) {
            tf_pt a = p->pt[pi], b = p->pt[pi + 1], e = p->pt[pi + 2];
            float d1x = cx - 2 * a.x + b.x, d1y = cy - 2 * a.y + b.y, d2x = a.x - 2 * b.x + e.x, d2y = a.y - 2 * b.y + e.y;
            float dev = d1x * d1x + d1y * d1y; float dev2 = d2x * d2x + d2y * d2y;
            int n = 1, k; float px = cx, py = cy;
            if (dev2 > dev) dev = dev2;
            if (dev >= 0.333f) { n = 1 + (int)floorf(sqrtf(sqrtf(3.0f * dev)) * 1.5f); if (n > 96) n = 96; }
            for (k = 1; k <= n; k++) {
                float t = (float)k / (float)n, mt = 1 - t;
                float nx = mt * mt * mt * cx + 3 * mt * mt * t * a.x + 3 * mt * t * t * b.x + t * t * t * e.x;
                float ny = mt * mt * mt * cy + 3 * mt * mt * t * a.y + 3 * mt * t * t * b.y + t * t * t * e.y;
                acc_line(r, px, py, nx, ny); px = nx; py = ny;
            }
            cx = e.x; cy = e.y; pi += 3;
        } else if (c == 4) { acc_line(r, cx, cy, sx, sy); cx = sx; cy = sy; }
    }
}

/* Rasterize glyph `gid` at `scale` px per unit with pen origin (ox, oy) in a
 * pixel grid whose y axis points down. Returns a coverage bitmap (caller
 * frees) with its top-left pixel position, or NULL for an empty glyph. */
static uint8_t* tf_raster(tf_font* f, int gid, double scale_x, double scale_y, double ox, double oy, int* bx, int* by, int* bw, int* bh) {
    tf_path p; int i, x0, y0, x1, y1, w, hh; float fx0 = 1e30f, fy0 = 1e30f, fx1 = -1e30f, fy1 = -1e30f;
    tf_acc r; uint8_t* cov; float acc;
    if (tf_outline(f, gid, &p) < 0 || p.npt == 0) { path_free(&p); return NULL; }
    for (i = 0; i < p.npt; i++) {
        float X = (float)(ox + p.pt[i].x * scale_x), Y = (float)(oy - p.pt[i].y * scale_y);
        if (!(X == X) || !(Y == Y) || fabsf(X) > 1e7f || fabsf(Y) > 1e7f) { path_free(&p); return NULL; }
        p.pt[i].x = X; p.pt[i].y = Y;
        if (X < fx0) fx0 = X; if (X > fx1) fx1 = X; if (Y < fy0) fy0 = Y; if (Y > fy1) fy1 = Y;
    }
    x0 = (int)floorf(fx0); y0 = (int)floorf(fy0); x1 = (int)ceilf(fx1); y1 = (int)ceilf(fy1);
    w = x1 - x0 + 1; hh = y1 - y0 + 1;
    if (w <= 0 || hh <= 0 || (int64_t)w * hh > 16 * 1024 * 1024) { path_free(&p); return NULL; }
    for (i = 0; i < p.npt; i++) { p.pt[i].x -= (float)x0; p.pt[i].y -= (float)y0; }
    r.w = w; r.h = hh;
    r.a = (float*)calloc((size_t)(w + 2) * (size_t)hh + 4, sizeof(float));
    cov = (uint8_t*)malloc((size_t)w * (size_t)hh);
    if (!r.a || !cov) { free(r.a); free(cov); path_free(&p); return NULL; }
    acc_path(&r, &p);
    path_free(&p);
    for (i = 0; i < hh; i++) {
        int x; float* row = r.a + (size_t)i * (size_t)(w + 2);
        acc = 0;
        for (x = 0; x < w; x++) {
            float v;
            acc += row[x];
            v = fabsf(acc); if (v > 1) v = 1;
            cov[(size_t)i * (size_t)w + (size_t)x] = (uint8_t)(v * 255.0f + 0.5f);
        }
    }
    free(r.a);
    *bx = x0; *by = y0; *bw = w; *bh = hh;
    return cov;
}

static uint8_t* g_last_cov; static int g_last_box[4];
/* Rasterize to a coverage bitmap kept until the next call; box via _box. */
int64_t trc_font_raster(int64_t h, int64_t gid, double size_px, double ox, double oy) {
    tf_font* f = F(h); double sc = size_px / f->upem;
    free(g_last_cov);
    g_last_cov = tf_raster(f, (int)gid, sc, sc, ox, oy, &g_last_box[0], &g_last_box[1], &g_last_box[2], &g_last_box[3]);
    if (!g_last_cov) { memset(g_last_box, 0, sizeof g_last_box); return 0; }
    return 1;
}
int64_t trc_font_raster_box(int64_t i) { return i >= 0 && i < 4 ? g_last_box[i] : 0; }
/* Copy the last coverage bitmap into an RGBA image buffer as white text with
 * alpha = coverage (dst must hold bw*bh*4 bytes). */
void trc_font_raster_copy(char* dst) {
    int64_t i, n = (int64_t)g_last_box[2] * g_last_box[3];
    if (!g_last_cov) return;
    for (i = 0; i < n; i++) { dst[4 * i] = dst[4 * i + 1] = dst[4 * i + 2] = (char)255; dst[4 * i + 3] = (char)g_last_cov[i]; }
}

static void blend_cov(uint8_t* img, int W, int H, const uint8_t* cov, int bx, int by, int bw, int bh, int cr, int cg, int cb, int ca) {
    int y, x;
    for (y = 0; y < bh; y++) {
        int py = by + y;
        if (py < 0 || py >= H) continue;
        for (x = 0; x < bw; x++) {
            int px = bx + x; uint8_t* d; int c = cov[(size_t)y * (size_t)bw + (size_t)x];
            float sa, da, oa;
            if (px < 0 || px >= W || c == 0) continue;
            d = img + ((size_t)py * (size_t)W + (size_t)px) * 4;
            sa = (float)c * (float)ca / (255.0f * 255.0f);
            da = d[3] / 255.0f;
            oa = sa + da * (1 - sa);
            if (oa <= 0) continue;
            d[0] = (uint8_t)((cr * sa + d[0] * da * (1 - sa)) / oa + 0.5f);
            d[1] = (uint8_t)((cg * sa + d[1] * da * (1 - sa)) / oa + 0.5f);
            d[2] = (uint8_t)((cb * sa + d[2] * da * (1 - sa)) / oa + 0.5f);
            d[3] = (uint8_t)(oa * 255.0f + 0.5f);
        }
    }
}

/* Draw glyph `gid` with its pen origin at (ox, oy) (pixels, y down, baseline)
 * into an RGBA8 image, source-over in colour (r, g, b, a). Returns 1 when
 * something was drawn. */
int64_t trc_font_draw_glyph(int64_t h, int64_t gid, double size_px, double ox, double oy, char* img, int64_t W, int64_t H, int64_t r, int64_t g, int64_t b, int64_t a) {
    tf_font* f = F(h); double sc = size_px / f->upem;
    int bx, by, bw, bh; uint8_t* cov;
    if (!img || W <= 0 || H <= 0) return 0;
    cov = tf_raster(f, (int)gid, sc, sc, ox, oy, &bx, &by, &bw, &bh);
    if (!cov) return 0;
    blend_cov((uint8_t*)img, (int)W, (int)H, cov, bx, by, bw, bh, (int)(r & 255), (int)(g & 255), (int)(b & 255), (int)(a & 255));
    free(cov);
    return 1;
}

/* ── subsetting ──────────────────────────────────────────────────────────── */
typedef struct { tf_font* f; int* o2n; int* n2o; int n, cap; } tf_subset;
#define SB(h) ((tf_subset*)(intptr_t)(h))

static int sub_add(tf_subset* s, int gid) {
    if (gid < 0 || gid >= s->f->nglyphs) gid = 0;
    if (s->o2n[gid] >= 0) return s->o2n[gid];
    if (s->n >= s->cap) {
        int nc = s->cap * 2; int* q = (int*)realloc(s->n2o, sizeof(int) * (size_t)nc);
        if (!q) return 0;
        s->n2o = q; s->cap = nc;
    }
    s->o2n[gid] = s->n; s->n2o[s->n] = gid;
    return s->n++;
}
int64_t trc_font_subset_new(int64_t h) {
    tf_font* f = F(h); tf_subset* s = (tf_subset*)calloc(1, sizeof(tf_subset)); int i;
    if (!s) return 0;
    s->f = f; s->o2n = (int*)malloc(sizeof(int) * (size_t)f->nglyphs); s->cap = 64; s->n2o = (int*)malloc(sizeof(int) * 64);
    if (!s->o2n || !s->n2o) { free(s->o2n); free(s->n2o); free(s); return 0; }
    for (i = 0; i < f->nglyphs; i++) s->o2n[i] = -1;
    sub_add(s, 0);
    return (int64_t)(intptr_t)s;
}
int64_t trc_font_subset_add(int64_t s, int64_t gid) { return sub_add(SB(s), (int)gid); }
int64_t trc_font_subset_count(int64_t s) { return SB(s)->n; }
int64_t trc_font_subset_old(int64_t s, int64_t i) { return i >= 0 && i < SB(s)->n ? SB(s)->n2o[i] : 0; }
void trc_font_subset_free(int64_t s) { if (s) { free(SB(s)->o2n); free(SB(s)->n2o); free(SB(s)); } }

/* TrueType glyph from a path (font units): cubics become quadratics. */
typedef struct { int32_t x, y; uint8_t on; } tt_pt;
static void tt_add(tt_pt** v, int* n, int* cap, double x, double y, int on) {
    if (*n >= *cap) { int nc = *cap ? *cap * 2 : 64; tt_pt* q = (tt_pt*)realloc(*v, sizeof(tt_pt) * (size_t)nc); if (!q) return; *v = q; *cap = nc; }
    if (x > 32767) x = 32767; if (x < -32768) x = -32768; if (y > 32767) y = 32767; if (y < -32768) y = -32768;
    (*v)[*n].x = (int32_t)floor(x + 0.5); (*v)[*n].y = (int32_t)floor(y + 0.5); (*v)[*n].on = (uint8_t)on; (*n)++;
}
static void path_to_glyf(const tf_path* p, tf_out* o, int* npts_out, int* ncont_out) {
    tt_pt* v = NULL; int n = 0, cap = 0, i, pi = 0;
    int* ends = NULL; int ne = 0, ecap = 0;
    double cx = 0, cy = 0; int cstart = 0, have = 0;
    for (i = 0; i <= p->ncmd; i++) {
        int c = i < p->ncmd ? p->cmd[i] : 0;
        if (c == 0 || c == 4) {
            if (have && n > cstart) {
                /* drop a closing point equal to the start */
                if (n - cstart > 1 && v[n - 1].on && v[n - 1].x == v[cstart].x && v[n - 1].y == v[cstart].y) n--;
                if (n - cstart >= 2) {
                    if (ne >= ecap) { int nc = ecap ? ecap * 2 : 16; int* q = (int*)realloc(ends, sizeof(int) * (size_t)nc); if (!q) break; ends = q; ecap = nc; }
                    ends[ne++] = n - 1;
                } else n = cstart;
            }
            have = 0;
            if (i == p->ncmd) break;
            if (c == 0) { cx = p->pt[pi].x; cy = p->pt[pi].y; pi++; cstart = n; tt_add(&v, &n, &cap, cx, cy, 1); have = 1; }
            continue;
        }
        if (c == 1) { cx = p->pt[pi].x; cy = p->pt[pi].y; pi++; tt_add(&v, &n, &cap, cx, cy, 1); }
        else if (c == 2) { tt_add(&v, &n, &cap, p->pt[pi].x, p->pt[pi].y, 0); cx = p->pt[pi + 1].x; cy = p->pt[pi + 1].y; tt_add(&v, &n, &cap, cx, cy, 1); pi += 2; }
        else if (c == 3) {
            double x0 = cx, y0 = cy, x1 = p->pt[pi].x, y1 = p->pt[pi].y, x2 = p->pt[pi + 1].x, y2 = p->pt[pi + 1].y, x3 = p->pt[pi + 2].x, y3 = p->pt[pi + 2].y;
            double dx = x3 - 3 * x2 + 3 * x1 - x0, dy = y3 - 3 * y2 + 3 * y1 - y0;
            double err = sqrt(dx * dx + dy * dy) * 0.0481125224;   /* sqrt(3)/36 */
            int k = (int)ceil(cbrt(err / 0.5)), j;
            if (k < 1) k = 1; if (k > 16) k = 16;
            for (j = 0; j < k; j++) {
                double t0 = (double)j / k, t1 = (double)(j + 1) / k;
                /* sub-cubic on [t0, t1] via polynomial evaluation */
                double ax = -x0 + 3 * x1 - 3 * x2 + x3, bxx = 3 * x0 - 6 * x1 + 3 * x2, cxx = -3 * x0 + 3 * x1, ay = -y0 + 3 * y1 - 3 * y2 + y3, byy = 3 * y0 - 6 * y1 + 3 * y2, cyy = -3 * y0 + 3 * y1;
                double h = t1 - t0;
                double px0 = ((ax * t0 + bxx) * t0 + cxx) * t0 + x0, py0 = ((ay * t0 + byy) * t0 + cyy) * t0 + y0;
                double px3 = ((ax * t1 + bxx) * t1 + cxx) * t1 + x0, py3 = ((ay * t1 + byy) * t1 + cyy) * t1 + y0;
                double dx0 = (3 * ax * t0 * t0 + 2 * bxx * t0 + cxx) * h, dy0 = (3 * ay * t0 * t0 + 2 * byy * t0 + cyy) * h;
                double dx1 = (3 * ax * t1 * t1 + 2 * bxx * t1 + cxx) * h, dy1 = (3 * ay * t1 * t1 + 2 * byy * t1 + cyy) * h;
                double c1x = px0 + dx0 / 3, c1y = py0 + dy0 / 3, c2x = px3 - dx1 / 3, c2y = py3 - dy1 / 3;
                double qx = (3 * (c1x + c2x) - (px0 + px3)) / 4, qy = (3 * (c1y + c2y) - (py0 + py3)) / 4;
                tt_add(&v, &n, &cap, qx, qy, 0);
                tt_add(&v, &n, &cap, px3, py3, 1);
            }
            cx = x3; cy = y3; pi += 3;
        }
    }
    *npts_out = 0; *ncont_out = 0;
    if (ne > 0 && n > 0) {
        int32_t xmn = v[0].x, xmx = v[0].x, ymn = v[0].y, ymx = v[0].y, px = 0, py = 0;
        int total = ends[ne - 1] + 1;
        for (i = 0; i < total; i++) { if (v[i].x < xmn) xmn = v[i].x; if (v[i].x > xmx) xmx = v[i].x; if (v[i].y < ymn) ymn = v[i].y; if (v[i].y > ymx) ymx = v[i].y; }
        out_u16(o, (uint32_t)ne); out_u16(o, (uint32_t)(xmn & 0xFFFF)); out_u16(o, (uint32_t)(ymn & 0xFFFF)); out_u16(o, (uint32_t)(xmx & 0xFFFF)); out_u16(o, (uint32_t)(ymx & 0xFFFF));
        for (i = 0; i < ne; i++) out_u16(o, (uint32_t)ends[i]);
        out_u16(o, 0);
        for (i = 0; i < total; i++) out_u8(o, v[i].on ? 1 : 0);
        for (i = 0; i < total; i++) { out_u16(o, (uint32_t)((v[i].x - px) & 0xFFFF)); px = v[i].x; }
        for (i = 0; i < total; i++) { out_u16(o, (uint32_t)((v[i].y - py) & 0xFFFF)); py = v[i].y; }
        *npts_out = total; *ncont_out = ne;
    }
    free(v); free(ends);
}

static uint32_t tbl_sum(const uint8_t* p, size_t n) {
    uint32_t s = 0; size_t i;
    for (i = 0; i + 4 <= n; i += 4) s += ((uint32_t)p[i] << 24) | ((uint32_t)p[i + 1] << 16) | ((uint32_t)p[i + 2] << 8) | p[i + 3];
    if (n & 3) { uint32_t last = 0; size_t k; for (k = 0; k < 4; k++) last = (last << 8) | (i + k < n ? p[i + k] : 0); s += last; }
    return s;
}

static uint8_t* g_sub_out; static int64_t g_sub_len;
/* Build the subset font; returns its length (bytes via trc_font_subset_take). */
int64_t trc_font_subset_build(int64_t sh) {
    tf_subset* s = SB(sh); tf_font* f = s->f;
    tf_out glyf = { 0 }, loca = { 0 }, hmtx = { 0 }, out = { 0 };
    int i, maxpts = 0, maxcont = 0;
    uint32_t* locs;
    /* composite closure (TrueType): appending while iterating */
    if (!f->is_cff) {
        for (i = 0; i < s->n; i++) {
            tf_buf g = glyf_data(f, s->n2o[i]);
            if (g.n >= 10 && rds16(g, 0) < 0) {
                uint64_t q = 10; uint32_t fl; int guard = 0;
                do {
                    fl = rd16(g, q); sub_add(s, (int)rd16(g, q + 2)); q += 4;
                    q += (fl & 1) ? 4 : 2;
                    if (fl & 8) q += 2; else if (fl & 0x40) q += 4; else if (fl & 0x80) q += 8;
                } while ((fl & 0x20) && q < g.n && ++guard < 256);
            }
        }
    }
    locs = (uint32_t*)malloc(sizeof(uint32_t) * (size_t)(s->n + 1));
    if (!locs) return -1;
    for (i = 0; i < s->n; i++) {
        int og = s->n2o[i];
        locs[i] = (uint32_t)glyf.n;
        if (!f->is_cff) {
            tf_buf g = glyf_data(f, og);
            if (g.n >= 10) {
                size_t at = glyf.n;
                out_bytes(&glyf, g.p, g.n);
                if (!glyf.bad && rds16(g, 0) < 0) {
                    uint64_t q = 10; uint32_t fl; int guard = 0;
                    do {
                        fl = rd16(g, q);
                        if (q + 4 > g.n) break;
                        { int ng = s->o2n[rd16(g, q + 2) < (uint32_t)f->nglyphs ? rd16(g, q + 2) : 0]; put16(glyf.p + at + q + 2, (uint32_t)(ng < 0 ? 0 : ng)); }
                        q += 4; q += (fl & 1) ? 4 : 2;
                        if (fl & 8) q += 2; else if (fl & 0x40) q += 4; else if (fl & 0x80) q += 8;
                    } while ((fl & 0x20) && ++guard < 256);
                } else if (rds16(g, 0) > 0) {
                    int nc = rds16(g, 0), np = (int)rd16(g, 10 + 2 * (uint64_t)(nc - 1)) + 1;
                    if (np > maxpts) maxpts = np; if (nc > maxcont) maxcont = nc;
                }
            }
        } else {
            tf_path p; int np = 0, nc = 0;
            if (tf_outline(f, og, &p) >= 0) path_to_glyf(&p, &glyf, &np, &nc);
            path_free(&p);
            if (np > maxpts) maxpts = np; if (nc > maxcont) maxcont = nc;
        }
        out_pad4(&glyf);
        {
            int lsb = tf_lsb(f, og);
            if (f->is_cff && glyf.n > locs[i] + 10) lsb = (int16_t)((glyf.p[locs[i] + 2] << 8) | glyf.p[locs[i] + 3]);
            out_u16(&hmtx, (uint32_t)tf_advance(f, og)); out_u16(&hmtx, (uint32_t)(lsb & 0xFFFF));
        }
    }
    locs[s->n] = (uint32_t)glyf.n;
    for (i = 0; i <= s->n; i++) out_u32(&loca, locs[i]);
    free(locs);
    if (glyf.n == 0) out_u32(&glyf, 0);      /* keep the table non-empty */
    {
        /* assemble tables (sorted by tag) */
        const char* tags[13]; tf_out tb[13]; int nt = 0, k;
        uint8_t head[54], hhea[36], maxp[32], post[32];
        static const uint8_t cmap4[] = { 0,0, 0,1, 0,3, 0,1, 0,0,0,12,
            0,4, 0,24, 0,0, 0,2, 0,2, 0,0, 0,0, 0xFF,0xFF, 0,0, 0xFF,0xFF, 0,1, 0,0 };
        memset(tb, 0, sizeof tb);
        memcpy(head, f->head.p, 54); put32(head + 8, 0); put16(head + 50, 1);
        memcpy(hhea, f->hhea.p, 36); put16(hhea + 34, (uint32_t)s->n);
        memset(maxp, 0, sizeof maxp);
        if (!f->is_cff && f->maxp.n >= 32) memcpy(maxp, f->maxp.p, 32);
        else { put32(maxp, 0x00010000); put16(maxp + 6, (uint32_t)maxpts); put16(maxp + 8, (uint32_t)maxcont); put16(maxp + 14, 2); }
        put32(maxp, 0x00010000); put16(maxp + 4, (uint32_t)s->n);
        if (f->is_cff) { put16(maxp + 6, (uint32_t)maxpts); put16(maxp + 8, (uint32_t)maxcont); }
        memset(post, 0, sizeof post); put32(post, 0x00030000);
        if (f->post.n >= 16) memcpy(post + 4, f->post.p + 4, 12);
#define ADD(tg, ptr, len) do { tags[nt] = tg; out_bytes(&tb[nt], ptr, len); nt++; } while (0)
        if (f->os2.n >= 78) ADD("OS/2", f->os2.p, f->os2.n);
        ADD("cmap", cmap4, sizeof cmap4);
        if (!f->is_cff && f->cvt.n) ADD("cvt ", f->cvt.p, f->cvt.n);
        if (!f->is_cff && f->fpgm.n) ADD("fpgm", f->fpgm.p, f->fpgm.n);
        ADD("glyf", glyf.p, glyf.n);
        ADD("head", head, 54);
        ADD("hhea", hhea, 36);
        ADD("hmtx", hmtx.p, hmtx.n);
        ADD("loca", loca.p, loca.n);
        ADD("maxp", maxp, 32);
        ADD("post", post, 32);
        if (!f->is_cff && f->prep.n) ADD("prep", f->prep.p, f->prep.n);
#undef ADD
        {
            uint32_t es = 0, p2 = 1, off; size_t head_at = 0;
            while (p2 * 2 <= (uint32_t)nt) { p2 *= 2; es++; }
            out_u32(&out, 0x00010000); out_u16(&out, (uint32_t)nt); out_u16(&out, p2 * 16); out_u16(&out, es); out_u16(&out, (uint32_t)nt * 16 - p2 * 16);
            off = 12 + 16 * (uint32_t)nt;
            for (k = 0; k < nt; k++) {
                out_u32(&out, (uint32_t)tf_tag(tags[k])); out_u32(&out, tb[k].bad ? 0 : tbl_sum(tb[k].p, tb[k].n)); out_u32(&out, off); out_u32(&out, (uint32_t)tb[k].n);
                off += (uint32_t)((tb[k].n + 3) & ~(size_t)3);
            }
            for (k = 0; k < nt; k++) {
                if (strcmp(tags[k], "head") == 0) head_at = out.n;
                out_bytes(&out, tb[k].p, tb[k].n); out_pad4(&out);
                free(tb[k].p);
            }
            if (!out.bad && head_at) put32(out.p + head_at + 8, 0xB1B0AFBAu - tbl_sum(out.p, out.n));
        }
    }
    free(glyf.p); free(loca.p); free(hmtx.p);
    if (glyf.bad || loca.bad || hmtx.bad || out.bad) { free(out.p); return -1; }
    free(g_sub_out); g_sub_out = out.p; g_sub_len = (int64_t)out.n;
    return g_sub_len;
}
void trc_font_subset_take(char* dst) {
    if (g_sub_out) { memcpy(dst, g_sub_out, (size_t)g_sub_len); free(g_sub_out); g_sub_out = NULL; g_sub_len = 0; }
}

/* ── system font directory scan ──────────────────────────────────────────── */
static int font_ext(const char* name) {
    size_t n = strlen(name); const char* e;
    if (n < 5) return 0;
    e = name + n - 4;
    if (e[0] != '.') { if (n >= 6 && name[n - 5] == '.' && (name[n - 4] | 32) == 'w' && (name[n - 3] | 32) == 'o' && (name[n - 2] | 32) == 'f' && (name[n - 1] | 32) == 'f') return 1; return 0; }
    {
        char a = (char)(e[1] | 32), b = (char)(e[2] | 32), c = (char)(e[3] | 32);
        return (a == 't' && b == 't' && (c == 'f' || c == 'c')) || (a == 'o' && b == 't' && (c == 'f' || c == 'c'));
    }
}
static void scan_dir(const char* dir, tf_out* o, int depth) {
    if (depth > 8 || o->n > (1u << 24)) return;
#ifdef _WIN32
    {
        char pat[1024]; WIN32_FIND_DATAA fd; HANDLE hf;
        if (snprintf(pat, sizeof pat, "%s\\*", dir) >= (int)sizeof pat) return;
        hf = FindFirstFileA(pat, &fd);
        if (hf == INVALID_HANDLE_VALUE) return;
        do {
            char full[1024];
            if (fd.cFileName[0] == '.') continue;
            if (snprintf(full, sizeof full, "%s\\%s", dir, fd.cFileName) >= (int)sizeof full) continue;
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) scan_dir(full, o, depth + 1);
            else if (font_ext(fd.cFileName)) { out_bytes(o, full, strlen(full)); out_u8(o, '\n'); }
        } while (FindNextFileA(hf, &fd));
        FindClose(hf);
    }
#else
    {
        DIR* d = opendir(dir); struct dirent* e;
        if (!d) return;
        while ((e = readdir(d)) != NULL) {
            char full[1024]; struct stat st;
            if (e->d_name[0] == '.') continue;
            if (snprintf(full, sizeof full, "%s/%s", dir, e->d_name) >= (int)sizeof full) continue;
            if (stat(full, &st) != 0) continue;
            if (S_ISDIR(st.st_mode)) scan_dir(full, o, depth + 1);
            else if (font_ext(e->d_name)) { out_bytes(o, full, strlen(full)); out_u8(o, '\n'); }
        }
        closedir(d);
    }
#endif
}
static uint8_t* g_scan; static int64_t g_scan_len;
/* Recursively list font files under `dir` (newline separated). */
int64_t trc_font_scan(const char* dir, int64_t n) {
    char path[1024]; tf_out o = { 0 };
    if (n <= 0 || n >= (int64_t)sizeof path) return 0;
    memcpy(path, dir, (size_t)n); path[n] = 0;
    scan_dir(path, &o, 0);
    free(g_scan); g_scan = o.p; g_scan_len = o.bad ? 0 : (int64_t)o.n;
    return g_scan_len;
}
void trc_font_scan_take(char* dst) { if (g_scan) { memcpy(dst, g_scan, (size_t)g_scan_len); free(g_scan); g_scan = NULL; g_scan_len = 0; } }
