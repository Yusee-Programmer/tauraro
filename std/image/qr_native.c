/*
 * std/image/qr_native.c -- QR Code (ISO/IEC 18004) encoder for std.image.qr.
 *
 * Linked through `# tauraro-cpp-shim: qr_native.c` in std/image/qr.tr.
 *
 *   versions 1-40, error correction L/M/Q/H (optionally boosted when the
 *   data leaves room), numeric / alphanumeric / byte segments chosen by a
 *   dynamic-programming segmentation that minimises the bit length, mask
 *   selection by the standard four-rule penalty score, Reed-Solomon over
 *   GF(256)/0x11D. Kanji and ECI modes are not produced.
 *
 * Written from the ISO/IEC 18004 specification. Output is one byte per
 * module (1 = dark) in row-major order.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef uint8_t u8;

#define QR_MAXN 177

static const signed char ECC_PER_BLOCK[4][41] = {
    { -1, 7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
    { -1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28 },
    { -1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
    { -1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 } };
static const signed char NUM_BLOCKS[4][41] = {
    { -1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 4, 6, 6, 6, 6, 7, 8, 8, 9, 9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25 },
    { -1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5, 5, 8, 9, 9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49 },
    { -1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8, 8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68 },
    { -1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81 } };
/* format-bits value of each ordinal (L, M, Q, H) */
static const int FMT_BITS[4] = { 1, 0, 3, 2 };

static int raw_modules(int ver) {
    int r = (16 * ver + 128) * ver + 64;
    if (ver >= 2) { int na = ver / 7 + 2; r -= (25 * na - 10) * na - 55; if (ver >= 7) r -= 36; }
    return r;
}
static int data_codewords(int ver, int ecc) {
    return raw_modules(ver) / 8 - ECC_PER_BLOCK[ecc][ver] * NUM_BLOCKS[ecc][ver];
}

/* ── bit buffer ── */
typedef struct { u8* b; int len, cap; } bits_t;
static int bits_add(bits_t* s, unsigned v, int n) {
    int i;
    if (s->len + n > s->cap) return -1;
    for (i = n - 1; i >= 0; i--) { if ((v >> i) & 1) s->b[s->len >> 3] |= (u8)(0x80 >> (s->len & 7)); s->len++; }
    return 0;
}

/* ── segmentation ── */
enum { M_NUM = 0, M_ALNUM = 1, M_BYTE = 2 };
static int alnum_val(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    switch (c) { case ' ': return 36; case '$': return 37; case '%': return 38; case '*': return 39; case '+': return 40;
                 case '-': return 41; case '.': return 42; case '/': return 43; case ':': return 44; }
    return -1;
}
static int ccbits(int mode, int ver) {
    static const int t[3][3] = { { 10, 12, 14 }, { 9, 11, 13 }, { 8, 16, 16 } };
    return t[mode][ver <= 9 ? 0 : ver <= 26 ? 1 : 2];
}
#define INF 0x3fffffff
/* Fill modes[0..n) with the cheapest mode per byte; returns total cost in 1/6 bits. */
static int segment_dp(const u8* t, int n, int ver, int force, u8* modes, u8* from) {
    int head[3], cc[3] = { 20, 33, 48 }, prev[3], cur[3], i, m, k;
    for (m = 0; m < 3; m++) head[m] = (4 + ccbits(m, ver)) * 6;
    if (n == 0) return 0;
    for (i = 0; i < n; i++) {
        int ok[3];
        ok[0] = t[i] >= '0' && t[i] <= '9'; ok[1] = alnum_val(t[i]) >= 0; ok[2] = 1;
        if (force) for (m = 0; m < 3; m++) ok[m] = (m == force - 1) && ok[m];
        for (m = 0; m < 3; m++) {
            cur[m] = INF; from[i * 3 + m] = (u8)m;
            if (!ok[m]) continue;
            if (i == 0) { cur[m] = head[m] + cc[m]; continue; }
            if (prev[m] < INF) { cur[m] = prev[m] + cc[m]; from[i * 3 + m] = (u8)m; }
            for (k = 0; k < 3; k++) {
                int c;
                if (k == m || prev[k] >= INF) continue;
                c = (prev[k] + 5) / 6 * 6 + head[m] + cc[m];
                if (c < cur[m]) { cur[m] = c; from[i * 3 + m] = (u8)k; }
            }
        }
        memcpy(prev, cur, sizeof prev);
    }
    {
        int best = 0;
        for (m = 1; m < 3; m++) if (prev[m] < prev[best]) best = m;
        if (prev[best] >= INF) return -1;
        for (i = n - 1; i >= 0; i--) { modes[i] = (u8)best; best = from[i * 3 + best]; }
        return 0;
    }
}

/* Encode text into `s` for version `ver`; returns 0 or -1 when it does not fit. */
static int encode_segments(const u8* t, int n, int ver, int force, bits_t* s, u8* modes, u8* from) {
    int i = 0;
    if (segment_dp(t, n, ver, force, modes, from) < 0) return -2;
    while (i < n) {
        int j = i, m = modes[i], cnt;
        while (j < n && modes[j] == m) j++;
        cnt = j - i;
        if (cnt >= (1 << ccbits(m, ver))) return -1;
        if (bits_add(s, m == M_NUM ? 1u : m == M_ALNUM ? 2u : 4u, 4)) return -1;
        if (bits_add(s, (unsigned)cnt, ccbits(m, ver))) return -1;
        if (m == M_NUM) {
            int k;
            for (k = i; k < j; k += 3) {
                int g = j - k < 3 ? j - k : 3, v = 0, q;
                for (q = 0; q < g; q++) v = v * 10 + (t[k + q] - '0');
                if (bits_add(s, (unsigned)v, g * 3 + 1)) return -1;
            }
        } else if (m == M_ALNUM) {
            int k;
            for (k = i; k + 1 < j; k += 2)
                if (bits_add(s, (unsigned)(alnum_val(t[k]) * 45 + alnum_val(t[k + 1])), 11)) return -1;
            if ((j - i) & 1) if (bits_add(s, (unsigned)alnum_val(t[j - 1]), 6)) return -1;
        } else {
            int k;
            for (k = i; k < j; k++) if (bits_add(s, t[k], 8)) return -1;
        }
        i = j;
    }
    return 0;
}

/* ── Reed-Solomon ── */
static u8 gf_mul(u8 x, u8 y) {
    int z = 0, i;
    for (i = 7; i >= 0; i--) { z = (z << 1) ^ ((z >> 7) * 0x11D); z ^= ((y >> i) & 1) * x; }
    return (u8)z;
}
static void rs_divisor(int degree, u8* out) {
    int i, j; u8 root = 1;
    memset(out, 0, (size_t)degree);
    out[degree - 1] = 1;
    for (i = 0; i < degree; i++) {
        for (j = 0; j < degree; j++) { out[j] = gf_mul(out[j], root); if (j + 1 < degree) out[j] ^= out[j + 1]; }
        root = gf_mul(root, 0x02);
    }
}
static void rs_remainder(const u8* data, int n, const u8* div, int degree, u8* out) {
    int i, j;
    memset(out, 0, (size_t)degree);
    for (i = 0; i < n; i++) {
        u8 f = data[i] ^ out[0];
        memmove(out, out + 1, (size_t)degree - 1); out[degree - 1] = 0;
        for (j = 0; j < degree; j++) out[j] ^= gf_mul(div[j], f);
    }
}

/* ── matrix ── */
typedef struct { int size; u8 m[QR_MAXN * QR_MAXN]; u8 fn[QR_MAXN * QR_MAXN]; } qrm;
#define M_(q, x, y) ((q)->m[(y) * (q)->size + (x)])
#define F_(q, x, y) ((q)->fn[(y) * (q)->size + (x)])
static void set_fn(qrm* q, int x, int y, int dark) { M_(q, x, y) = (u8)(dark != 0); F_(q, x, y) = 1; }
static void draw_finder(qrm* q, int cx, int cy) {
    int dx, dy;
    for (dy = -4; dy <= 4; dy++) for (dx = -4; dx <= 4; dx++) {
        int d = abs(dx) > abs(dy) ? abs(dx) : abs(dy), x = cx + dx, y = cy + dy;
        if (x >= 0 && x < q->size && y >= 0 && y < q->size) set_fn(q, x, y, d != 2 && d != 4);
    }
}
static void draw_align(qrm* q, int cx, int cy) {
    int dx, dy;
    for (dy = -2; dy <= 2; dy++) for (dx = -2; dx <= 2; dx++) {
        int d = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
        set_fn(q, cx + dx, cy + dy, d != 1);
    }
}
static int align_pos(int ver, int* out) {
    int na, step, i, size = ver * 4 + 17;
    if (ver == 1) return 0;
    na = ver / 7 + 2;
    step = ver == 32 ? 26 : (ver * 4 + na * 2 + 1) / (na * 2 - 2) * 2;
    out[0] = 6;
    for (i = na - 1; i >= 1; i--) out[i] = size - 7 - (na - 1 - i) * step;
    return na;
}
static void draw_format(qrm* q, int ecc, int mask) {
    int data = FMT_BITS[ecc] << 3 | mask, rem = data, i, bits, s = q->size;
    for (i = 0; i < 10; i++) rem = (rem << 1) ^ ((rem >> 9) * 0x537);
    bits = (data << 10 | rem) ^ 0x5412;
    for (i = 0; i <= 5; i++) set_fn(q, 8, i, (bits >> i) & 1);
    set_fn(q, 8, 7, (bits >> 6) & 1); set_fn(q, 8, 8, (bits >> 7) & 1); set_fn(q, 7, 8, (bits >> 8) & 1);
    for (i = 9; i < 15; i++) set_fn(q, 14 - i, 8, (bits >> i) & 1);
    for (i = 0; i < 8; i++) set_fn(q, s - 1 - i, 8, (bits >> i) & 1);
    for (i = 8; i < 15; i++) set_fn(q, 8, s - 15 + i, (bits >> i) & 1);
    set_fn(q, 8, s - 8, 1);
}
static void draw_functions(qrm* q, int ver, int ecc) {
    int s = q->size, i, j, na, ap[8];
    for (i = 0; i < s; i++) { set_fn(q, 6, i, i % 2 == 0); set_fn(q, i, 6, i % 2 == 0); }
    draw_finder(q, 3, 3); draw_finder(q, s - 4, 3); draw_finder(q, 3, s - 4);
    na = align_pos(ver, ap);
    for (i = 0; i < na; i++) for (j = 0; j < na; j++)
        if (!((i == 0 && j == 0) || (i == 0 && j == na - 1) || (i == na - 1 && j == 0))) draw_align(q, ap[i], ap[j]);
    draw_format(q, ecc, 0);
    if (ver >= 7) {
        int rem = ver, bits;
        for (i = 0; i < 12; i++) rem = (rem << 1) ^ ((rem >> 11) * 0x1F25);
        bits = ver << 12 | rem;
        for (i = 0; i < 18; i++) {
            int b = (bits >> i) & 1, a = s - 11 + i % 3, c = i / 3;
            set_fn(q, a, c, b); set_fn(q, c, a, b);
        }
    }
}
static void draw_codewords(qrm* q, const u8* data, int n) {
    int i = 0, right, vert, j, s = q->size;
    for (right = s - 1; right >= 1; right -= 2) {
        if (right == 6) right = 5;
        for (vert = 0; vert < s; vert++)
            for (j = 0; j < 2; j++) {
                int x = right - j, up = ((right + 1) & 2) == 0, y = up ? s - 1 - vert : vert;
                if (!F_(q, x, y) && i < n * 8) { M_(q, x, y) = (u8)((data[i >> 3] >> (7 - (i & 7))) & 1); i++; }
            }
    }
}
static void apply_mask(qrm* q, int mask) {
    int x, y, s = q->size;
    for (y = 0; y < s; y++) for (x = 0; x < s; x++) {
        int inv;
        switch (mask) {
        case 0: inv = (x + y) % 2 == 0; break;
        case 1: inv = y % 2 == 0; break;
        case 2: inv = x % 3 == 0; break;
        case 3: inv = (x + y) % 3 == 0; break;
        case 4: inv = (x / 3 + y / 2) % 2 == 0; break;
        case 5: inv = x * y % 2 + x * y % 3 == 0; break;
        case 6: inv = (x * y % 2 + x * y % 3) % 2 == 0; break;
        default: inv = ((x + y) % 2 + x * y % 3) % 2 == 0; break;
        }
        if (inv && !F_(q, x, y)) M_(q, x, y) ^= 1;
    }
}
/* penalty for one line (row or column) of length s, given by accessor */
static int line_penalty(const u8* ln, int s) {
    int pen = 0, run = 1, i;
    for (i = 1; i <= s; i++) {
        if (i < s && ln[i] == ln[i - 1]) run++;
        else { if (run >= 5) pen += 3 + (run - 5); run = 1; }
    }
    {   /* finder-like 1:1:3:1:1 with 4 light modules on a side; outside counts as light */
        u8 buf[QR_MAXN + 20]; int k;
        memset(buf, 0, sizeof buf);
        memcpy(buf + 10, ln, (size_t)s);
        for (k = 0; k + 11 <= s + 20; k++) {
            static const u8 A[11] = { 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0 }, B[11] = { 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1 };
            if (memcmp(buf + k, A, 11) == 0) pen += 40;
            if (memcmp(buf + k, B, 11) == 0) pen += 40;
        }
    }
    return pen;
}
static int penalty(const qrm* q) {
    int s = q->size, x, y, pen = 0, dark = 0;
    u8 col[QR_MAXN];
    for (y = 0; y < s; y++) pen += line_penalty(&q->m[y * s], s);
    for (x = 0; x < s; x++) { for (y = 0; y < s; y++) col[y] = q->m[y * s + x]; pen += line_penalty(col, s); }
    for (y = 0; y + 1 < s; y++) for (x = 0; x + 1 < s; x++) {
        u8 c = q->m[y * s + x];
        if (c == q->m[y * s + x + 1] && c == q->m[(y + 1) * s + x] && c == q->m[(y + 1) * s + x + 1]) pen += 3;
    }
    for (y = 0; y < s * s; y++) dark += q->m[y];
    { int total = s * s, k = (abs(dark * 20 - total * 10) + total - 1) / total - 1; pen += k * 10; }
    return pen;
}

/*
 * ecc: 0 L, 1 M, 2 Q, 3 H. mask -1 = automatic. force_mode 0 = automatic
 * segmentation, 1 numeric, 2 alphanumeric, 3 byte (a segment that cannot
 * hold the data fails with -2). out must hold 177*177 bytes; info receives
 * {size, version, mask, ecc}. Returns 0, -1 (too long for the version
 * range), -2 (bad arguments or data not representable in the forced mode).
 */
int64_t trc_qr_encode(const char* text, int64_t tlen, int64_t ecc, int64_t minver, int64_t maxver, int64_t mask,
                      int64_t boost, int64_t force_mode, char* out, int64_t* info) {
    const u8* t = (const u8*)text;
    int n = (int)tlen, ver, e = (int)ecc, rc = -1;
    bits_t s; u8 *modes = NULL, *from = NULL, *all = NULL;
    qrm* q = NULL;
    if (tlen < 0 || tlen > 8000 || ecc < 0 || ecc > 3 || minver < 1 || maxver > 40 || minver > maxver || mask < -1 || mask > 7 || force_mode < 0 || force_mode > 3) return -2;
    modes = (u8*)malloc((size_t)n + 1); from = (u8*)malloc((size_t)n * 3 + 3);
    s.cap = 23648 + 64; s.b = (u8*)calloc(1, (size_t)s.cap / 8 + 16);
    q = (qrm*)malloc(sizeof(qrm));
    if (!modes || !from || !s.b || !q) { rc = -3; goto done; }
    for (ver = (int)minver; ver <= (int)maxver; ver++) {
        int cap_bits = data_codewords(ver, e) * 8, r;
        memset(s.b, 0, (size_t)s.cap / 8 + 16); s.len = 0;
        r = encode_segments(t, n, ver, (int)force_mode, &s, modes, from);
        if (r == -2) { rc = -2; goto done; }
        if (r == 0 && s.len <= cap_bits) break;
        if (ver == maxver) { rc = -1; goto done; }
    }
    if (boost) {
        int k;
        for (k = 1; k <= 3; k++) if (s.len <= data_codewords(ver, k) * 8) e = k;
        if (e < (int)ecc) e = (int)ecc;
    }
    {
        int cap_bits = data_codewords(ver, e) * 8, ncw = cap_bits / 8, pad = 0xEC, nb, bel, raw, nshort, slen, i, j, k, qsize;
        u8* dat; u8 div[30], rem[30]; u8* res;
        /* terminator, bit padding, byte padding */
        { int term = cap_bits - s.len < 4 ? cap_bits - s.len : 4; s.len += term; }
        s.len = (s.len + 7) / 8 * 8;
        dat = (u8*)calloc((size_t)ncw + 1, 1);
        all = (u8*)calloc((size_t)raw_modules(ver) / 8 + 8, 1);
        if (!dat || !all) { free(dat); rc = -3; goto done; }
        memcpy(dat, s.b, (size_t)s.len / 8);
        for (i = s.len / 8; i < ncw; i++) { dat[i] = (u8)pad; pad ^= 0xEC ^ 0x11; }
        nb = NUM_BLOCKS[e][ver]; bel = ECC_PER_BLOCK[e][ver]; raw = raw_modules(ver) / 8;
        nshort = nb - raw % nb; slen = raw / nb;
        rs_divisor(bel, div);
        res = all;
        {
            u8* blocks = (u8*)calloc((size_t)nb, (size_t)slen + 1);
            if (!blocks) { free(dat); rc = -3; goto done; }
            for (i = 0, k = 0; i < nb; i++) {
                int dl = slen - bel + (i < nshort ? 0 : 1);
                u8* blk = blocks + (size_t)i * (slen + 1);
                memcpy(blk, dat + k, (size_t)dl);
                rs_remainder(dat + k, dl, div, bel, rem);
                k += dl;
                memcpy(blk + (i < nshort ? dl + 1 : dl), rem, (size_t)bel);
            }
            for (i = 0, k = 0; i < slen + 1; i++)
                for (j = 0; j < nb; j++)
                    if (i != slen - bel || j >= nshort) res[k++] = blocks[(size_t)j * (slen + 1) + i];
            free(blocks);
        }
        free(dat);
        qsize = ver * 4 + 17;
        q->size = qsize;
        memset(q->m, 0, sizeof q->m); memset(q->fn, 0, sizeof q->fn);
        draw_functions(q, ver, e);
        draw_codewords(q, res, raw);
        {
            int best = (int)mask;
            if (mask < 0) {
                int m, bp = 0x7fffffff;
                for (m = 0; m < 8; m++) {
                    int p;
                    apply_mask(q, m); draw_format(q, e, m);
                    p = penalty(q);
                    if (p < bp) { bp = p; best = m; }
                    apply_mask(q, m);
                }
            }
            apply_mask(q, best); draw_format(q, e, best);
            memcpy(out, q->m, (size_t)qsize * qsize);
            info[0] = qsize; info[1] = ver; info[2] = best; info[3] = e;
        }
        rc = 0;
    }
done:
    free(modes); free(from); free(s.b); free(q); free(all);
    return rc;
}
