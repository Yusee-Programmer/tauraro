/*
 * std/image/webp_native.c -- WebP decode (lossy VP8 + ALPH alpha, lossless
 * VP8L, animation) and lossless VP8L encode for std.image.webp.
 *
 * Linked through `# tauraro-cpp-shim: webp_native.c` in std/image/webp.tr.
 * Pixel buffers are RGBA8, row-major, top row first, no padding.
 *
 * Written from the WebP container spec, RFC 9649 (VP8L) and RFC 6386 (VP8).
 * To produce pixels bit-identical to libwebp (and so to Pillow) the decoder
 * follows libwebp's integer arithmetic for the inverse transforms, the
 * "fancy" chroma upsampler, the YUV->RGB conversion and the animation
 * blending. The VP8 probability / quantiser tables near the end of the file
 * are data copied from libwebp (BSD-3-Clause, Copyright (c) 2010 Google
 * Inc.); see std/image/vendor/README.md for the licence text.
 *
 *   decode ....... RIFF/WEBP simple and extended (VP8X) files; VP8 key frames
 *                  (segments, both loop filters, all intra modes, multiple
 *                  token partitions); ALPH (raw or VP8L-compressed, all
 *                  filters); VP8L (all four transforms, colour cache, meta
 *                  prefix codes); ANIM/ANMF composited like WebPAnimDecoder
 *   encode ....... lossless VP8L: subtract-green + per-block predictor
 *                  transform, or a colour-indexing transform for images with
 *                  <= 256 colours; LZ77 with a hash chain; length-limited
 *                  Huffman codes. Animated files (ANMF, one full frame each).
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define WP_MAX_PIXELS ((size_t)1 << 28)

/* ═════════════════════════════ result ════════════════════════════════════ */
typedef struct {
    int w, h, nframes, loop, has_alpha, lossless, animated;
    u8** frames;
    int* delays;
    char err[160];
} wp_result;

static void wp_fail(wp_result* r, const char* m) { if (r && !r->err[0]) snprintf(r->err, sizeof r->err, "WebP: %s", m); }
static int wp_add_frame(wp_result* r, u8* rgba, int delay) {
    u8** nf; int* nd;
    nf = (u8**)realloc(r->frames, sizeof(u8*) * (size_t)(r->nframes + 1));
    if (!nf) return -1;
    r->frames = nf;
    nd = (int*)realloc(r->delays, sizeof(int) * (size_t)(r->nframes + 1));
    if (!nd) return -1;
    r->delays = nd;
    r->frames[r->nframes] = rgba; r->delays[r->nframes] = delay; r->nframes++;
    return 0;
}
void trc_webp_free(void* h) {
    wp_result* r = (wp_result*)h; int i;
    if (!r) return;
    for (i = 0; i < r->nframes; i++) free(r->frames[i]);
    free(r->frames); free(r->delays); free(r);
}
int64_t trc_webp_ok(void* h) { wp_result* r = (wp_result*)h; return r && r->nframes > 0 && !r->err[0]; }
int64_t trc_webp_w(void* h) { return h ? ((wp_result*)h)->w : 0; }
int64_t trc_webp_h(void* h) { return h ? ((wp_result*)h)->h : 0; }
int64_t trc_webp_nframes(void* h) { return h ? ((wp_result*)h)->nframes : 0; }
int64_t trc_webp_delay(void* h, int64_t i) { wp_result* r = (wp_result*)h; return (r && i >= 0 && i < r->nframes) ? r->delays[i] : 0; }
int64_t trc_webp_loop(void* h) { return h ? ((wp_result*)h)->loop : 0; }
int64_t trc_webp_has_alpha(void* h) { return h ? ((wp_result*)h)->has_alpha : 0; }
int64_t trc_webp_lossless(void* h) { return h ? ((wp_result*)h)->lossless : 0; }
int64_t trc_webp_animated(void* h) { return h ? ((wp_result*)h)->animated : 0; }
int64_t trc_webp_error(void* h, char* out, int64_t cap) {
    wp_result* r = (wp_result*)h; size_t n;
    if (!r) { memcpy(out, "WebP: out of memory", 19); return 19; }
    n = strlen(r->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, r->err, n); return (int64_t)n;
}
void trc_webp_frame_copy(void* h, int64_t i, char* out) {
    wp_result* r = (wp_result*)h;
    if (r && i >= 0 && i < r->nframes) memcpy(out, r->frames[i], (size_t)r->w * (size_t)r->h * 4);
}
void trc_webp_buf_free(char* p) { free(p); }

static u32 rd24(const u8* p) { return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16); }
static u32 rd32(const u8* p) { return rd24(p) | ((u32)p[3] << 24); }

/* ═════════════════════════════ VP8L decoder ══════════════════════════════ */
typedef struct { const u8* p; size_t n, pos; u64 val; int nbits; int eos; } lbr;

static void lbr_init(lbr* b, const u8* p, size_t n) { memset(b, 0, sizeof *b); b->p = p; b->n = n; }
static void lbr_fill(lbr* b) {
    while (b->nbits <= 56 && b->pos < b->n) { b->val |= (u64)b->p[b->pos++] << b->nbits; b->nbits += 8; }
}
static u32 lbits(lbr* b, int n) {
    u32 v;
    if (n == 0) return 0;
    if (b->nbits < n) lbr_fill(b);
    if (b->nbits < n) { b->eos = 1; b->val = 0; b->nbits = 0; return 0; }
    v = (u32)(b->val & (((u64)1 << n) - 1));
    b->val >>= n; b->nbits -= n;
    return v;
}

#define HMAXLEN 15
typedef struct {
    int single;            /* >= 0: the only symbol (0 bits) */
    u16 count[HMAXLEN + 1];
    u16* sym;              /* symbols sorted by code */
    u16 fast[256];         /* (sym << 4) | len for codes <= 8 bits; 0 = slow */
} hcode;

static void hfree(hcode* h) { free(h->sym); h->sym = NULL; }

/* Build from code lengths. 0 = ok. Codes must be complete (one symbol = 0 bits). */
static int hbuild(hcode* h, const u8* lens, int n) {
    int i, len, nsym = 0, last = -1;
    u16 offs[HMAXLEN + 2];
    u32 next[HMAXLEN + 2];
    long left = 1;
    memset(h->count, 0, sizeof h->count);
    memset(h->fast, 0, sizeof h->fast);
    h->single = -1; h->sym = NULL;
    for (i = 0; i < n; i++) if (lens[i]) { if (lens[i] > HMAXLEN) return -1; h->count[lens[i]]++; nsym++; last = i; }
    if (nsym == 0) return -1;
    if (nsym == 1) { h->single = last; return 0; }
    for (len = 1; len <= HMAXLEN; len++) { left <<= 1; left -= h->count[len]; if (left < 0) return -1; }
    if (left != 0) return -1;                         /* incomplete */
    h->sym = (u16*)malloc(sizeof(u16) * (size_t)nsym);
    if (!h->sym) return -1;
    offs[1] = 0;
    for (len = 1; len < HMAXLEN; len++) offs[len + 1] = (u16)(offs[len] + h->count[len]);
    for (i = 0; i < n; i++) if (lens[i]) h->sym[offs[lens[i]]++] = (u16)i;
    /* fast table: canonical codes, bit-reversed (LSB-first stream) */
    next[1] = 0;
    for (len = 1; len <= HMAXLEN; len++) next[len + 1] = (next[len] + h->count[len]) << 1;
    for (i = 0; i < n; i++) {
        int l = lens[i]; u32 code, rev = 0; int k;
        if (!l) continue;
        code = next[l]++;
        if (l > 8) continue;
        for (k = 0; k < l; k++) if (code & (1u << k)) rev |= 1u << (l - 1 - k);
        for (k = (int)rev; k < 256; k += 1 << l) h->fast[k] = (u16)((i << 4) | l);
    }
    return 0;
}

static int hread(lbr* b, const hcode* h) {
    int code = 0, first = 0, index = 0, len;
    if (h->single >= 0) return h->single;
    if (b->nbits < 8) lbr_fill(b);
    if (b->nbits >= 8) {
        u16 e = h->fast[b->val & 0xFF];
        if (e) { int l = e & 15; b->val >>= l; b->nbits -= l; return e >> 4; }
    }
    for (len = 1; len <= HMAXLEN; len++) {
        int count;
        code |= (int)lbits(b, 1);
        if (b->eos) return -1;
        count = h->count[len];
        if (code - count < first) return h->sym[index + (code - first)];
        index += count; first += count; first <<= 1; code <<= 1;
    }
    return -1;
}

static const u8 kCodeLengthOrder[19] = { 17, 18, 0, 1, 2, 3, 4, 5, 16, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

static int read_hcode(lbr* b, int alphabet, hcode* out) {
    u8* lens = (u8*)calloc((size_t)alphabet, 1);
    int ok = -1;
    if (!lens) return -1;
    if (lbits(b, 1)) {                                  /* simple code */
        int num = (int)lbits(b, 1) + 1;
        int first8 = (int)lbits(b, 1);
        int s0 = (int)lbits(b, first8 ? 8 : 1);
        if (s0 >= alphabet) goto done;
        lens[s0] = 1;
        if (num == 2) {
            int s1 = (int)lbits(b, 8);
            if (s1 >= alphabet) goto done;
            lens[s1] = 1;
        }
    } else {
        u8 cl[19]; hcode clh; int i, ncl = (int)lbits(b, 4) + 4, max_symbol, sym = 0, prev = 8;
        memset(cl, 0, sizeof cl);
        if (ncl > 19) goto done;
        for (i = 0; i < ncl; i++) cl[kCodeLengthOrder[i]] = (u8)lbits(b, 3);
        if (b->eos || hbuild(&clh, cl, 19) != 0) goto done;
        if (lbits(b, 1)) {
            int nb = 2 + 2 * (int)lbits(b, 3);
            max_symbol = 2 + (int)lbits(b, nb);
            if (max_symbol > alphabet) { hfree(&clh); goto done; }
        } else max_symbol = alphabet;
        while (sym < alphabet) {
            int c;
            if (max_symbol-- == 0) break;
            c = hread(b, &clh);
            if (c < 0 || b->eos) { hfree(&clh); goto done; }
            if (c < 16) { lens[sym++] = (u8)c; if (c) prev = c; }
            else {
                static const int xb[3] = { 2, 3, 7 }, xo[3] = { 3, 3, 11 };
                int rep = (int)lbits(b, xb[c - 16]) + xo[c - 16];
                int v = c == 16 ? prev : 0;
                if (sym + rep > alphabet) { hfree(&clh); goto done; }
                while (rep-- > 0) lens[sym++] = (u8)v;
            }
        }
        hfree(&clh);
    }
    if (b->eos) goto done;
    ok = hbuild(out, lens, alphabet);
done:
    free(lens);
    return ok;
}

typedef struct { hcode c[5]; } hgroup;

static const u8 kCodeToPlane[120] = {
    0x18, 0x07, 0x17, 0x19, 0x28, 0x06, 0x27, 0x29, 0x16, 0x1a, 0x26, 0x2a,
    0x38, 0x05, 0x37, 0x39, 0x15, 0x1b, 0x36, 0x3a, 0x25, 0x2b, 0x48, 0x04,
    0x47, 0x49, 0x14, 0x1c, 0x35, 0x3b, 0x46, 0x4a, 0x24, 0x2c, 0x58, 0x45,
    0x4b, 0x34, 0x3c, 0x03, 0x57, 0x59, 0x13, 0x1d, 0x56, 0x5a, 0x23, 0x2d,
    0x44, 0x4c, 0x55, 0x5b, 0x33, 0x3d, 0x68, 0x02, 0x67, 0x69, 0x12, 0x1e,
    0x66, 0x6a, 0x22, 0x2e, 0x54, 0x5c, 0x43, 0x4d, 0x65, 0x6b, 0x32, 0x3e,
    0x78, 0x01, 0x77, 0x79, 0x53, 0x5d, 0x11, 0x1f, 0x64, 0x6c, 0x42, 0x4e,
    0x76, 0x7a, 0x21, 0x2f, 0x75, 0x7b, 0x31, 0x3f, 0x63, 0x6d, 0x52, 0x5e,
    0x00, 0x74, 0x7c, 0x41, 0x4f, 0x10, 0x20, 0x62, 0x6e, 0x30, 0x73, 0x7d,
    0x51, 0x5f, 0x40, 0x72, 0x7e, 0x61, 0x6f, 0x50, 0x71, 0x7f, 0x60, 0x70 };

static int plane_to_dist(int xsize, int code) {
    int dc, yo, xo, d;
    if (code > 120) return code - 120;
    dc = kCodeToPlane[code - 1]; yo = dc >> 4; xo = 8 - (dc & 0xF);
    d = yo * xsize + xo;
    return d >= 1 ? d : 1;
}
static int prefix_value(lbr* b, int prefix) {
    int extra;
    if (prefix < 4) return prefix + 1;
    extra = (prefix - 2) >> 1;
    if (extra > 24) return -1;
    return ((2 + (prefix & 1)) << extra) + (int)lbits(b, extra) + 1;
}

typedef struct { int type, bits, xsize; u32* data; int ncolors; } ltransform;

static int div_round(int n, int bits) { return (n + (1 << bits) - 1) >> bits; }

static u32* vp8l_image(lbr* b, int xs, int ys, int level0, ltransform* tr, int* ntr, int* out_xs);

/* Decode the entropy-coded pixels of an xs*ys image. */
static u32* vp8l_pixels(lbr* b, int xs, int ys, int level0) {
    int cache_bits = 0, meta_bits = 0, ngroups = 1, mxs = 0, i, j;
    u32* meta = NULL; u32* cache = NULL; u32* out = NULL;
    hgroup* groups = NULL;
    size_t total = (size_t)xs * (size_t)ys, pos = 0;
    if (total == 0 || total > WP_MAX_PIXELS) return NULL;
    if (lbits(b, 1)) {
        cache_bits = (int)lbits(b, 4);
        if (cache_bits < 1 || cache_bits > 11) return NULL;
    }
    if (level0 && lbits(b, 1)) {
        int mys, d;
        meta_bits = (int)lbits(b, 3) + 2;
        mxs = div_round(xs, meta_bits); mys = div_round(ys, meta_bits);
        meta = vp8l_image(b, mxs, mys, 0, NULL, NULL, &d);
        if (!meta) return NULL;
        ngroups = 0;
        for (i = 0; i < mxs * mys; i++) { int g = (int)((meta[i] >> 8) & 0xFFFF); if (g + 1 > ngroups) ngroups = g + 1; }
        if ((size_t)ngroups > (b->n - b->pos) * 8 + 64) { free(meta); return NULL; }   /* each group needs bits */
    }
    if (b->eos) { free(meta); return NULL; }
    groups = (hgroup*)calloc((size_t)ngroups, sizeof(hgroup));
    if (!groups) { free(meta); return NULL; }
    for (i = 0; i < ngroups; i++) {
        static const int base[5] = { 256 + 24, 256, 256, 256, 40 };
        for (j = 0; j < 5; j++) {
            int alpha = base[j] + (j == 0 && cache_bits ? (1 << cache_bits) : 0);
            if (read_hcode(b, alpha, &groups[i].c[j]) != 0) { ngroups = i + 1; goto fail; }
        }
    }
    if (cache_bits) { cache = (u32*)calloc((size_t)1 << cache_bits, 4); if (!cache) goto fail; }
    out = (u32*)malloc(total * 4);
    if (!out) goto fail;
    while (pos < total) {
        int x = (int)(pos % (size_t)xs), y = (int)(pos / (size_t)xs);
        hgroup* g = groups;
        int code;
        if (meta) {
            int gi = (int)((meta[(y >> meta_bits) * mxs + (x >> meta_bits)] >> 8) & 0xFFFF);
            g = &groups[gi];
        }
        code = hread(b, &g->c[0]);
        if (code < 0 || b->eos) goto fail;
        if (code < 256) {
            int r = hread(b, &g->c[1]), bl = hread(b, &g->c[2]), a = hread(b, &g->c[3]);
            u32 px;
            if (r < 0 || bl < 0 || a < 0 || b->eos) goto fail;
            px = ((u32)a << 24) | ((u32)r << 16) | ((u32)code << 8) | (u32)bl;
            out[pos++] = px;
            if (cache) cache[(0x1e35a7bdu * px) >> (32 - cache_bits)] = px;
        } else if (code < 280) {
            int len = prefix_value(b, code - 256), ds, dist, k;
            if (len < 0) goto fail;
            ds = hread(b, &g->c[4]);
            if (ds < 0) goto fail;
            dist = prefix_value(b, ds);
            if (dist < 0 || b->eos) goto fail;
            dist = plane_to_dist(xs, dist);
            if ((size_t)dist > pos || (size_t)len > total - pos) goto fail;
            for (k = 0; k < len; k++) {
                u32 px = out[pos - (size_t)dist];
                out[pos++] = px;
                if (cache) cache[(0x1e35a7bdu * px) >> (32 - cache_bits)] = px;
            }
        } else {
            int idx = code - 280;
            if (!cache || idx >= (1 << cache_bits)) goto fail;
            out[pos++] = cache[idx];
        }
    }
    for (i = 0; i < ngroups; i++) for (j = 0; j < 5; j++) hfree(&groups[i].c[j]);
    free(groups); free(meta); free(cache);
    return out;
fail:
    for (i = 0; i < ngroups; i++) for (j = 0; j < 5; j++) hfree(&groups[i].c[j]);
    free(groups); free(meta); free(cache); free(out);
    return NULL;
}

/* Read transforms (level 0 only) and the pixel data. */
static u32* vp8l_image(lbr* b, int xs, int ys, int level0, ltransform* tr, int* ntr, int* out_xs) {
    int seen = 0;
    if (level0) {
        *ntr = 0;
        while (lbits(b, 1)) {
            int t = (int)lbits(b, 2), d;
            ltransform* T;
            if (b->eos || (seen & (1 << t)) || *ntr >= 4) return NULL;
            seen |= 1 << t;
            T = &tr[(*ntr)++];
            T->type = t; T->xsize = xs; T->data = NULL; T->bits = 0; T->ncolors = 0;
            if (t == 0 || t == 1) {
                T->bits = (int)lbits(b, 3) + 2;
                T->data = vp8l_image(b, div_round(xs, T->bits), div_round(ys, T->bits), 0, NULL, NULL, &d);
                if (!T->data) return NULL;
            } else if (t == 3) {
                int nc = (int)lbits(b, 8) + 1, i;
                u32* pal;
                T->ncolors = nc;
                T->bits = nc > 16 ? 0 : nc > 4 ? 1 : nc > 2 ? 2 : 3;
                pal = vp8l_image(b, nc, 1, 0, NULL, NULL, &d);
                if (!pal) return NULL;
                T->data = (u32*)calloc(256, 4);
                if (!T->data) { free(pal); return NULL; }
                for (i = 0; i < nc; i++) {
                    u32 p = pal[i];
                    if (i > 0) {
                        u32 q = T->data[i - 1];
                        p = (((p & 0xFF00FF00u) + (q & 0xFF00FF00u)) & 0xFF00FF00u) |
                            (((p & 0x00FF00FFu) + (q & 0x00FF00FFu)) & 0x00FF00FFu);
                    }
                    T->data[i] = p;
                }
                free(pal);
                xs = div_round(xs, T->bits);
            }
        }
    }
    *out_xs = xs;
    return vp8l_pixels(b, xs, ys, level0);
}

static u32 add_px(u32 a, u32 b) {
    return (((a & 0xFF00FF00u) + (b & 0xFF00FF00u)) & 0xFF00FF00u) | (((a & 0x00FF00FFu) + (b & 0x00FF00FFu)) & 0x00FF00FFu);
}
static u32 avg2(u32 a, u32 b) { return (((a ^ b) & 0xFEFEFEFEu) >> 1) + (a & b); }
static int clip255(int v) { return v < 0 ? 0 : v > 255 ? 255 : v; }
static int sub3(int a, int b, int c) { int pb = b - c, pa = a - c; return abs(pb) - abs(pa); }
static u32 lselect(u32 a, u32 b, u32 c) {
    int d = sub3((int)(a >> 24), (int)(b >> 24), (int)(c >> 24)) +
            sub3((int)((a >> 16) & 0xff), (int)((b >> 16) & 0xff), (int)((c >> 16) & 0xff)) +
            sub3((int)((a >> 8) & 0xff), (int)((b >> 8) & 0xff), (int)((c >> 8) & 0xff)) +
            sub3((int)(a & 0xff), (int)(b & 0xff), (int)(c & 0xff));
    return d <= 0 ? a : b;
}
static u32 clamp_full(u32 a, u32 b, u32 c) {
    u32 r = 0; int s;
    for (s = 0; s < 32; s += 8)
        r |= (u32)clip255((int)((a >> s) & 0xff) + (int)((b >> s) & 0xff) - (int)((c >> s) & 0xff)) << s;
    return r;
}
static u32 clamp_half(u32 c0, u32 c1) {
    u32 r = 0; int s;
    for (s = 0; s < 32; s += 8) {
        int a = (int)((c0 >> s) & 0xff), b = (int)((c1 >> s) & 0xff);
        r |= (u32)clip255(a + (a - b) / 2) << s;
    }
    return r;
}
static u32 lpredict(int mode, const u32* p, int xs) {
    u32 L = p[-1], T = p[-xs], TL = p[-xs - 1], TR = p[-xs + 1];
    switch (mode) {
    case 1: return L;
    case 2: return T;
    case 3: return TR;
    case 4: return TL;
    case 5: return avg2(avg2(L, TR), T);
    case 6: return avg2(L, TL);
    case 7: return avg2(L, T);
    case 8: return avg2(TL, T);
    case 9: return avg2(T, TR);
    case 10: return avg2(avg2(L, TL), avg2(T, TR));
    case 11: return lselect(T, L, TL);
    case 12: return clamp_full(L, T, TL);
    case 13: return clamp_half(avg2(L, T), TL);
    default: return 0xff000000u;
    }
}

/* Apply the inverse of transform T; `px` has T's input width, result has T->xsize. */
static u32* vp8l_inverse(ltransform* T, u32* px, int ys) {
    int xs = T->xsize, x, y;
    if (T->type == 0) {
        int tw = div_round(xs, T->bits);
        for (y = 0; y < ys; y++)
            for (x = 0; x < xs; x++) {
                u32* p = px + (size_t)y * xs + x; u32 pred;
                if (y == 0) pred = x == 0 ? 0xff000000u : p[-1];
                else if (x == 0) pred = p[-xs];
                else pred = lpredict((int)((T->data[(y >> T->bits) * tw + (x >> T->bits)] >> 8) & 0xF), p, xs);
                *p = add_px(*p, pred);
            }
        return px;
    }
    if (T->type == 1) {
        int tw = div_round(xs, T->bits);
        for (y = 0; y < ys; y++)
            for (x = 0; x < xs; x++) {
                u32* p = px + (size_t)y * xs + x;
                u32 m = T->data[(y >> T->bits) * tw + (x >> T->bits)];
                int g2r = (int8_t)(m & 0xff), g2b = (int8_t)((m >> 8) & 0xff), r2b = (int8_t)((m >> 16) & 0xff);
                u32 v = *p;
                int green = (int8_t)((v >> 8) & 0xff);
                int red = (int)((v >> 16) & 0xff), blue = (int)(v & 0xff);
                red = (red + ((g2r * green) >> 5)) & 0xff;
                blue = (blue + ((g2b * green) >> 5)) & 0xff;
                blue = (blue + ((r2b * (int8_t)red) >> 5)) & 0xff;
                *p = (v & 0xff00ff00u) | ((u32)red << 16) | (u32)blue;
            }
        return px;
    }
    if (T->type == 2) {
        size_t i, n = (size_t)xs * ys;
        for (i = 0; i < n; i++) {
            u32 v = px[i], g = (v >> 8) & 0xff;
            u32 rb = ((v & 0x00ff00ffu) + ((g << 16) | g)) & 0x00ff00ffu;
            px[i] = (v & 0xff00ff00u) | rb;
        }
        return px;
    }
    {   /* colour indexing: unpack into a new buffer */
        int pw = div_round(xs, T->bits), per = 1 << T->bits, bpp = 8 >> T->bits, mask = (1 << bpp) - 1;
        u32* out = (u32*)malloc((size_t)xs * ys * 4);
        if (!out) { free(px); return NULL; }
        for (y = 0; y < ys; y++)
            for (x = 0; x < xs; x++) {
                u32 g = (px[(size_t)y * pw + (x >> T->bits)] >> 8) & 0xff;
                int idx = (int)((g >> ((x & (per - 1)) * bpp)) & (u32)mask);
                out[(size_t)y * xs + x] = T->data[idx];
            }
        free(px);
        return out;
    }
}

/* Decode a VP8L bitstream (after the 5-byte header when `headerless`). ARGB out. */
static u32* vp8l_decode_stream(lbr* b, int xs, int ys) {
    ltransform tr[4]; int ntr = 0, i, pxs = 0;
    u32* px;
    memset(tr, 0, sizeof tr);
    px = vp8l_image(b, xs, ys, 1, tr, &ntr, &pxs);
    if (px) {
        for (i = ntr - 1; i >= 0 && px; i--) px = vp8l_inverse(&tr[i], px, ys);
    }
    for (i = 0; i < 4; i++) free(tr[i].data);
    return px;
}

static u8* vp8l_decode(const u8* d, size_t n, int* w, int* h, int* alpha_hint, wp_result* r) {
    lbr b; u32* argb; u8* rgba; size_t i, np;
    int xs, ys, ver;
    if (n < 5 || d[0] != 0x2f) { wp_fail(r, "bad VP8L signature"); return NULL; }
    lbr_init(&b, d + 1, n - 1);
    xs = (int)lbits(&b, 14) + 1; ys = (int)lbits(&b, 14) + 1;
    *alpha_hint = (int)lbits(&b, 1);
    ver = (int)lbits(&b, 3);
    if (ver != 0) { wp_fail(r, "unsupported VP8L version"); return NULL; }
    argb = vp8l_decode_stream(&b, xs, ys);
    if (!argb) { wp_fail(r, "corrupt VP8L data"); return NULL; }
    np = (size_t)xs * ys;
    rgba = (u8*)malloc(np * 4);
    if (!rgba) { free(argb); wp_fail(r, "out of memory"); return NULL; }
    for (i = 0; i < np; i++) {
        u32 v = argb[i];
        rgba[i * 4] = (u8)(v >> 16); rgba[i * 4 + 1] = (u8)(v >> 8); rgba[i * 4 + 2] = (u8)v; rgba[i * 4 + 3] = (u8)(v >> 24);
    }
    free(argb);
    *w = xs; *h = ys;
    return rgba;
}

/* ═════════════════════════════ VP8 (lossy) decoder ═══════════════════════ */
/* ---- tables copied from libwebp (src/dec/tree_dec.c, src/dec/quant_dec.c), BSD-3-Clause,
 * Copyright (c) 2010 Google Inc.: default coefficient probabilities (RFC 6386 13.5),
 * coefficient update probabilities (13.4), key-frame sub-block mode probabilities (11.5)
 * and the DC/AC dequantisation tables (14.1). ---- */
static const u8 vp8_coef_proba0[1056] = {
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 253, 136, 254, 255, 228,
    219, 128, 128, 128, 128, 128, 189, 129, 242, 255, 227, 213, 255, 219, 128, 128, 128, 106, 126,
    227, 252, 214, 209, 255, 255, 128, 128, 128, 1, 98, 248, 255, 236, 226, 255, 255, 128, 128, 128,
    181, 133, 238, 254, 221, 234, 255, 154, 128, 128, 128, 78, 134, 202, 247, 198, 180, 255, 219,
    128, 128, 128, 1, 185, 249, 255, 243, 255, 128, 128, 128, 128, 128, 184, 150, 247, 255, 236,
    224, 128, 128, 128, 128, 128, 77, 110, 216, 255, 236, 230, 128, 128, 128, 128, 128, 1, 101, 251,
    255, 241, 255, 128, 128, 128, 128, 128, 170, 139, 241, 252, 236, 209, 255, 255, 128, 128, 128,
    37, 116, 196, 243, 228, 255, 255, 255, 128, 128, 128, 1, 204, 254, 255, 245, 255, 128, 128, 128,
    128, 128, 207, 160, 250, 255, 238, 128, 128, 128, 128, 128, 128, 102, 103, 231, 255, 211, 171,
    128, 128, 128, 128, 128, 1, 152, 252, 255, 240, 255, 128, 128, 128, 128, 128, 177, 135, 243,
    255, 234, 225, 128, 128, 128, 128, 128, 80, 129, 211, 255, 194, 224, 128, 128, 128, 128, 128, 1,
    1, 255, 128, 128, 128, 128, 128, 128, 128, 128, 246, 1, 255, 128, 128, 128, 128, 128, 128, 128,
    128, 255, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 198, 35, 237, 223, 193, 187, 162,
    160, 145, 155, 62, 131, 45, 198, 221, 172, 176, 220, 157, 252, 221, 1, 68, 47, 146, 208, 149,
    167, 221, 162, 255, 223, 128, 1, 149, 241, 255, 221, 224, 255, 255, 128, 128, 128, 184, 141,
    234, 253, 222, 220, 255, 199, 128, 128, 128, 81, 99, 181, 242, 176, 190, 249, 202, 255, 255,
    128, 1, 129, 232, 253, 214, 197, 242, 196, 255, 255, 128, 99, 121, 210, 250, 201, 198, 255, 202,
    128, 128, 128, 23, 91, 163, 242, 170, 187, 247, 210, 255, 255, 128, 1, 200, 246, 255, 234, 255,
    128, 128, 128, 128, 128, 109, 178, 241, 255, 231, 245, 255, 255, 128, 128, 128, 44, 130, 201,
    253, 205, 192, 255, 255, 128, 128, 128, 1, 132, 239, 251, 219, 209, 255, 165, 128, 128, 128, 94,
    136, 225, 251, 218, 190, 255, 255, 128, 128, 128, 22, 100, 174, 245, 186, 161, 255, 199, 128,
    128, 128, 1, 182, 249, 255, 232, 235, 128, 128, 128, 128, 128, 124, 143, 241, 255, 227, 234,
    128, 128, 128, 128, 128, 35, 77, 181, 251, 193, 211, 255, 205, 128, 128, 128, 1, 157, 247, 255,
    236, 231, 255, 255, 128, 128, 128, 121, 141, 235, 255, 225, 227, 255, 255, 128, 128, 128, 45,
    99, 188, 251, 195, 217, 255, 224, 128, 128, 128, 1, 1, 251, 255, 213, 255, 128, 128, 128, 128,
    128, 203, 1, 248, 255, 255, 128, 128, 128, 128, 128, 128, 137, 1, 177, 255, 224, 255, 128, 128,
    128, 128, 128, 253, 9, 248, 251, 207, 208, 255, 192, 128, 128, 128, 175, 13, 224, 243, 193, 185,
    249, 198, 255, 255, 128, 73, 17, 171, 221, 161, 179, 236, 167, 255, 234, 128, 1, 95, 247, 253,
    212, 183, 255, 255, 128, 128, 128, 239, 90, 244, 250, 211, 209, 255, 255, 128, 128, 128, 155,
    77, 195, 248, 188, 195, 255, 255, 128, 128, 128, 1, 24, 239, 251, 218, 219, 255, 205, 128, 128,
    128, 201, 51, 219, 255, 196, 186, 128, 128, 128, 128, 128, 69, 46, 190, 239, 201, 218, 255, 228,
    128, 128, 128, 1, 191, 251, 255, 255, 128, 128, 128, 128, 128, 128, 223, 165, 249, 255, 213,
    255, 128, 128, 128, 128, 128, 141, 124, 248, 255, 255, 128, 128, 128, 128, 128, 128, 1, 16, 248,
    255, 255, 128, 128, 128, 128, 128, 128, 190, 36, 230, 255, 236, 255, 128, 128, 128, 128, 128,
    149, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128, 1, 226, 255, 128, 128, 128, 128, 128, 128,
    128, 128, 247, 192, 255, 128, 128, 128, 128, 128, 128, 128, 128, 240, 128, 255, 128, 128, 128,
    128, 128, 128, 128, 128, 1, 134, 252, 255, 255, 128, 128, 128, 128, 128, 128, 213, 62, 250, 255,
    255, 128, 128, 128, 128, 128, 128, 55, 93, 255, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 202, 24, 213, 235, 186, 191,
    220, 160, 240, 175, 255, 126, 38, 182, 232, 169, 184, 228, 174, 255, 187, 128, 61, 46, 138, 219,
    151, 178, 240, 170, 255, 216, 128, 1, 112, 230, 250, 199, 191, 247, 159, 255, 255, 128, 166,
    109, 228, 252, 211, 215, 255, 174, 128, 128, 128, 39, 77, 162, 232, 172, 180, 245, 178, 255,
    255, 128, 1, 52, 220, 246, 198, 199, 249, 220, 255, 255, 128, 124, 74, 191, 243, 183, 193, 250,
    221, 255, 255, 128, 24, 71, 130, 219, 154, 170, 243, 182, 255, 255, 128, 1, 182, 225, 249, 219,
    240, 255, 224, 128, 128, 128, 149, 150, 226, 252, 216, 205, 255, 171, 128, 128, 128, 28, 108,
    170, 242, 183, 194, 254, 223, 255, 255, 128, 1, 81, 230, 252, 204, 203, 255, 192, 128, 128, 128,
    123, 102, 209, 247, 188, 196, 255, 233, 128, 128, 128, 20, 95, 153, 243, 164, 173, 255, 203,
    128, 128, 128, 1, 222, 248, 255, 216, 213, 128, 128, 128, 128, 128, 168, 175, 246, 252, 235,
    205, 255, 255, 128, 128, 128, 47, 116, 215, 255, 211, 212, 255, 255, 128, 128, 128, 1, 121, 236,
    253, 212, 214, 255, 255, 128, 128, 128, 141, 84, 213, 252, 201, 202, 255, 219, 128, 128, 128,
    42, 80, 160, 240, 162, 185, 255, 205, 128, 128, 128, 1, 1, 255, 128, 128, 128, 128, 128, 128,
    128, 128, 244, 1, 255, 128, 128, 128, 128, 128, 128, 128, 128, 238, 1, 255, 128, 128, 128, 128,
    128, 128, 128, 128,
};

static const u8 vp8_coef_update[1056] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 176, 246, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 223, 241, 252, 255, 255, 255, 255, 255, 255, 255, 255, 249, 253,
    253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 244, 252, 255, 255, 255, 255, 255, 255, 255,
    255, 234, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 246, 254, 255, 255, 255, 255, 255, 255, 255, 255, 239, 253, 254, 255,
    255, 255, 255, 255, 255, 255, 255, 254, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    248, 254, 255, 255, 255, 255, 255, 255, 255, 255, 251, 255, 254, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 254, 255, 255, 255,
    255, 255, 255, 255, 255, 251, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 254,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 253, 255, 254, 255, 255, 255, 255, 255, 255,
    250, 255, 254, 255, 254, 255, 255, 255, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 217, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 225, 252, 241, 253, 255, 255, 254, 255, 255, 255,
    255, 234, 250, 241, 250, 253, 255, 253, 254, 255, 255, 255, 255, 254, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 223, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 238, 253, 254, 254,
    255, 255, 255, 255, 255, 255, 255, 255, 248, 254, 255, 255, 255, 255, 255, 255, 255, 255, 249,
    254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 247, 254, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 254,
    255, 255, 255, 255, 255, 255, 255, 255, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 255, 255, 255, 255, 255,
    255, 255, 255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 254, 253, 255, 255, 255, 255, 255, 255, 255, 255, 250, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 186, 251, 250, 255,
    255, 255, 255, 255, 255, 255, 255, 234, 251, 244, 254, 255, 255, 255, 255, 255, 255, 255, 251,
    251, 243, 253, 254, 255, 254, 255, 255, 255, 255, 255, 253, 254, 255, 255, 255, 255, 255, 255,
    255, 255, 236, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 251, 253, 253, 254, 254, 255,
    255, 255, 255, 255, 255, 255, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 250, 254, 252, 254, 255, 255, 255, 255, 255, 255, 255, 248, 254, 249,
    253, 255, 255, 255, 255, 255, 255, 255, 255, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255,
    246, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255, 252, 254, 251, 254, 254, 255, 255, 255,
    255, 255, 255, 255, 254, 252, 255, 255, 255, 255, 255, 255, 255, 255, 248, 254, 253, 255, 255,
    255, 255, 255, 255, 255, 255, 253, 255, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 251,
    254, 255, 255, 255, 255, 255, 255, 255, 255, 245, 251, 254, 255, 255, 255, 255, 255, 255, 255,
    255, 253, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 251, 253, 255, 255, 255, 255,
    255, 255, 255, 255, 252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 249,
    255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 250, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
};

static const u8 vp8_bmode_proba[900] = {
    231, 120, 48, 89, 115, 113, 120, 152, 112, 152, 179, 64, 126, 170, 118, 46, 70, 95, 175, 69,
    143, 80, 85, 82, 72, 155, 103, 56, 58, 10, 171, 218, 189, 17, 13, 152, 114, 26, 17, 163, 44,
    195, 21, 10, 173, 121, 24, 80, 195, 26, 62, 44, 64, 85, 144, 71, 10, 38, 171, 213, 144, 34, 26,
    170, 46, 55, 19, 136, 160, 33, 206, 71, 63, 20, 8, 114, 114, 208, 12, 9, 226, 81, 40, 11, 96,
    182, 84, 29, 16, 36, 134, 183, 89, 137, 98, 101, 106, 165, 148, 72, 187, 100, 130, 157, 111, 32,
    75, 80, 66, 102, 167, 99, 74, 62, 40, 234, 128, 41, 53, 9, 178, 241, 141, 26, 8, 107, 74, 43,
    26, 146, 73, 166, 49, 23, 157, 65, 38, 105, 160, 51, 52, 31, 115, 128, 104, 79, 12, 27, 217,
    255, 87, 17, 7, 87, 68, 71, 44, 114, 51, 15, 186, 23, 47, 41, 14, 110, 182, 183, 21, 17, 194,
    66, 45, 25, 102, 197, 189, 23, 18, 22, 88, 88, 147, 150, 42, 46, 45, 196, 205, 43, 97, 183, 117,
    85, 38, 35, 179, 61, 39, 53, 200, 87, 26, 21, 43, 232, 171, 56, 34, 51, 104, 114, 102, 29, 93,
    77, 39, 28, 85, 171, 58, 165, 90, 98, 64, 34, 22, 116, 206, 23, 34, 43, 166, 73, 107, 54, 32,
    26, 51, 1, 81, 43, 31, 68, 25, 106, 22, 64, 171, 36, 225, 114, 34, 19, 21, 102, 132, 188, 16,
    76, 124, 62, 18, 78, 95, 85, 57, 50, 48, 51, 193, 101, 35, 159, 215, 111, 89, 46, 111, 60, 148,
    31, 172, 219, 228, 21, 18, 111, 112, 113, 77, 85, 179, 255, 38, 120, 114, 40, 42, 1, 196, 245,
    209, 10, 25, 109, 88, 43, 29, 140, 166, 213, 37, 43, 154, 61, 63, 30, 155, 67, 45, 68, 1, 209,
    100, 80, 8, 43, 154, 1, 51, 26, 71, 142, 78, 78, 16, 255, 128, 34, 197, 171, 41, 40, 5, 102,
    211, 183, 4, 1, 221, 51, 50, 17, 168, 209, 192, 23, 25, 82, 138, 31, 36, 171, 27, 166, 38, 44,
    229, 67, 87, 58, 169, 82, 115, 26, 59, 179, 63, 59, 90, 180, 59, 166, 93, 73, 154, 40, 40, 21,
    116, 143, 209, 34, 39, 175, 47, 15, 16, 183, 34, 223, 49, 45, 183, 46, 17, 33, 183, 6, 98, 15,
    32, 183, 57, 46, 22, 24, 128, 1, 54, 17, 37, 65, 32, 73, 115, 28, 128, 23, 128, 205, 40, 3, 9,
    115, 51, 192, 18, 6, 223, 87, 37, 9, 115, 59, 77, 64, 21, 47, 104, 55, 44, 218, 9, 54, 53, 130,
    226, 64, 90, 70, 205, 40, 41, 23, 26, 57, 54, 57, 112, 184, 5, 41, 38, 166, 213, 30, 34, 26,
    133, 152, 116, 10, 32, 134, 39, 19, 53, 221, 26, 114, 32, 73, 255, 31, 9, 65, 234, 2, 15, 1,
    118, 73, 75, 32, 12, 51, 192, 255, 160, 43, 51, 88, 31, 35, 67, 102, 85, 55, 186, 85, 56, 21,
    23, 111, 59, 205, 45, 37, 192, 55, 38, 70, 124, 73, 102, 1, 34, 98, 125, 98, 42, 88, 104, 85,
    117, 175, 82, 95, 84, 53, 89, 128, 100, 113, 101, 45, 75, 79, 123, 47, 51, 128, 81, 171, 1, 57,
    17, 5, 71, 102, 57, 53, 41, 49, 38, 33, 13, 121, 57, 73, 26, 1, 85, 41, 10, 67, 138, 77, 110,
    90, 47, 114, 115, 21, 2, 10, 102, 255, 166, 23, 6, 101, 29, 16, 10, 85, 128, 101, 196, 26, 57,
    18, 10, 102, 102, 213, 34, 20, 43, 117, 20, 15, 36, 163, 128, 68, 1, 26, 102, 61, 71, 37, 34,
    53, 31, 243, 192, 69, 60, 71, 38, 73, 119, 28, 222, 37, 68, 45, 128, 34, 1, 47, 11, 245, 171,
    62, 17, 19, 70, 146, 85, 55, 62, 70, 37, 43, 37, 154, 100, 163, 85, 160, 1, 63, 9, 92, 136, 28,
    64, 32, 201, 85, 75, 15, 9, 9, 64, 255, 184, 119, 16, 86, 6, 28, 5, 64, 255, 25, 248, 1, 56, 8,
    17, 132, 137, 255, 55, 116, 128, 58, 15, 20, 82, 135, 57, 26, 121, 40, 164, 50, 31, 137, 154,
    133, 25, 35, 218, 51, 103, 44, 131, 131, 123, 31, 6, 158, 86, 40, 64, 135, 148, 224, 45, 183,
    128, 22, 26, 17, 131, 240, 154, 14, 1, 209, 45, 16, 21, 91, 64, 222, 7, 1, 197, 56, 21, 39, 155,
    60, 138, 23, 102, 213, 83, 12, 13, 54, 192, 255, 68, 47, 28, 85, 26, 85, 85, 128, 128, 32, 146,
    171, 18, 11, 7, 63, 144, 171, 4, 4, 246, 35, 27, 10, 146, 174, 171, 12, 26, 128, 190, 80, 35,
    99, 180, 80, 126, 54, 45, 85, 126, 47, 87, 176, 51, 41, 20, 32, 101, 75, 128, 139, 118, 146,
    116, 128, 85, 56, 41, 15, 176, 236, 85, 37, 9, 62, 71, 30, 17, 119, 118, 255, 17, 18, 138, 101,
    38, 60, 138, 55, 70, 43, 26, 142, 146, 36, 19, 30, 171, 255, 97, 27, 20, 138, 45, 61, 62, 219,
    1, 81, 188, 64, 32, 41, 20, 117, 151, 142, 20, 21, 163, 112, 19, 12, 61, 195, 128, 48, 4, 24,
};

static const u8 vp8_dc_table[128] = {
    4, 5, 6, 7, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 17, 18, 19, 20, 20, 21, 21, 22, 22, 23,
    23, 24, 25, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 37, 38, 39, 40, 41, 42, 43, 44,
    45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
    68, 69, 70, 71, 72, 73, 74, 75, 76, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 91,
    93, 95, 96, 98, 100, 101, 102, 104, 106, 108, 110, 112, 114, 116, 118, 122, 124, 126, 128, 130,
    132, 134, 136, 138, 140, 143, 145, 148, 151, 154, 157,
};

static const u16 vp8_ac_table[128] = {
    4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56, 57, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
    96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 119, 122, 125, 128, 131, 134, 137, 140,
    143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 177, 181, 185, 189, 193, 197, 201, 205,
    209, 213, 217, 221, 225, 229, 234, 239, 245, 249, 254, 259, 264, 269, 274, 279, 284,
};
/* ---- end of libwebp tables ---- */

typedef struct { const u8* p; const u8* end; u32 value, range; int bit_count, eof; } bdec;

static void bd_init(bdec* d, const u8* p, size_t n) {
    d->p = p; d->end = p + n; d->value = 0; d->range = 255; d->bit_count = 0; d->eof = 0;
    if (d->p < d->end) d->value = (u32)*d->p++ << 8; else d->eof++;
    if (d->p < d->end) d->value |= *d->p++; else d->eof++;
}
static int bd_bit(bdec* d, int prob) {
    u32 split = 1 + (((d->range - 1) * (u32)prob) >> 8);
    u32 big = split << 8;
    int r;
    if (d->value >= big) { r = 1; d->range -= split; d->value -= big; }
    else { r = 0; d->range = split; }
    while (d->range < 128) {
        d->value <<= 1; d->range <<= 1;
        if (++d->bit_count == 8) {
            d->bit_count = 0;
            if (d->p < d->end) d->value |= *d->p++; else d->eof++;
        }
    }
    return r;
}
static int bd_val(bdec* d, int n) { int v = 0; while (n-- > 0) v = (v << 1) | bd_bit(d, 128); return v; }
static int bd_sval(bdec* d, int n) { int v = bd_val(d, n); return bd_bit(d, 128) ? -v : v; }

#define BPS 32
enum { B_DC, B_TM, B_VE, B_HE, B_RD, B_VR, B_LD, B_VL, B_HD, B_HU };

typedef struct { int limit, ilevel, hev, inner; } vfinfo;
typedef struct {
    int w, h, mbw, mbh;
    int use_segment, update_map, absolute_delta;
    int seg_quant[4], seg_filter[4];
    u8 seg_proba[3];
    int filter_simple, filter_level, sharpness, use_lf_delta, ref_lf_delta[4], mode_lf_delta[4];
    int filter_type;
    int nparts;
    bdec br, parts[8];
    int dq[4][3][2];          /* segment, [y1, y2, uv], [dc, ac] */
    u8 coef[1056];
    int use_skip, skip_p;
    vfinfo fstr[4][2];
} vp8d;

static int clip8(int v) { return v < 0 ? 0 : v > 255 ? 255 : v; }

static const u8 kBands[17] = { 0, 1, 2, 3, 6, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 0 };
static const u8 kZigzag[16] = { 0, 1, 4, 8, 5, 2, 3, 6, 9, 12, 13, 10, 7, 11, 14, 15 };
static const u8 kCat3[] = { 173, 148, 140, 0 };
static const u8 kCat4[] = { 176, 155, 140, 135, 0 };
static const u8 kCat5[] = { 180, 157, 141, 134, 130, 0 };
static const u8 kCat6[] = { 254, 254, 243, 230, 196, 177, 153, 140, 133, 130, 129, 0 };
static const u8* const kCat3456[] = { kCat3, kCat4, kCat5, kCat6 };

#define COEFP(dec, t, b, c) (&(dec)->coef[(((t) * 8 + (b)) * 3 + (c)) * 11])

static int large_value(bdec* br, const u8* p) {
    int v;
    if (!bd_bit(br, p[3])) {
        if (!bd_bit(br, p[4])) v = 2;
        else v = 3 + bd_bit(br, p[5]);
    } else {
        if (!bd_bit(br, p[6])) {
            if (!bd_bit(br, p[7])) v = 5 + bd_bit(br, 159);
            else { v = 7 + 2 * bd_bit(br, 165); v += bd_bit(br, 145); }
        } else {
            const u8* tab;
            int bit1 = bd_bit(br, p[8]);
            int bit0 = bd_bit(br, p[9 + bit1]);
            int cat = 2 * bit1 + bit0;
            v = 0;
            for (tab = kCat3456[cat]; *tab; ++tab) v += v + bd_bit(br, *tab);
            v += 3 + (8 << cat);
        }
    }
    return v;
}

static int get_coeffs(vp8d* dec, bdec* br, int t, int ctx, const int* dq, int n, int16_t* out) {
    const u8* p = COEFP(dec, t, kBands[n], ctx);
    for (; n < 16; ++n) {
        if (!bd_bit(br, p[0])) return n;
        while (!bd_bit(br, p[1])) {
            ++n;
            if (n == 16) return 16;
            p = COEFP(dec, t, kBands[n], 0);
        }
        {
            int v;
            if (!bd_bit(br, p[2])) { v = 1; p = COEFP(dec, t, kBands[n + 1], 1); }
            else { v = large_value(br, p); p = COEFP(dec, t, kBands[n + 1], 2); }
            out[kZigzag[n]] = (int16_t)((bd_bit(br, 128) ? -v : v) * dq[n > 0]);
        }
    }
    return 16;
}

#define MUL1(a) ((((a) * 20091) >> 16) + (a))
#define MUL2(a) (((a) * 35468) >> 16)
static void idct_add(const int16_t* in, u8* dst) {
    int C[16], *tmp = C, i;
    for (i = 0; i < 4; ++i) {
        int a = in[0] + in[8], b = in[0] - in[8];
        int c = MUL2(in[4]) - MUL1(in[12]);
        int d = MUL1(in[4]) + MUL2(in[12]);
        tmp[0] = a + d; tmp[1] = b + c; tmp[2] = b - c; tmp[3] = a - d;
        tmp += 4; in++;
    }
    tmp = C;
    for (i = 0; i < 4; ++i) {
        int dc = tmp[0] + 4;
        int a = dc + tmp[8], b = dc - tmp[8];
        int c = MUL2(tmp[4]) - MUL1(tmp[12]);
        int d = MUL1(tmp[4]) + MUL2(tmp[12]);
        dst[0] = (u8)clip8(dst[0] + ((a + d) >> 3));
        dst[1] = (u8)clip8(dst[1] + ((b + c) >> 3));
        dst[2] = (u8)clip8(dst[2] + ((b - c) >> 3));
        dst[3] = (u8)clip8(dst[3] + ((a - d) >> 3));
        tmp++; dst += BPS;
    }
}
static void iwht(const int16_t* in, int16_t* out) {
    int tmp[16], i;
    for (i = 0; i < 4; ++i) {
        int a0 = in[0 + i] + in[12 + i], a1 = in[4 + i] + in[8 + i];
        int a2 = in[4 + i] - in[8 + i], a3 = in[0 + i] - in[12 + i];
        tmp[0 + i] = a0 + a1; tmp[8 + i] = a0 - a1; tmp[4 + i] = a3 + a2; tmp[12 + i] = a3 - a2;
    }
    for (i = 0; i < 4; ++i) {
        int dc = tmp[0 + i * 4] + 3;
        int a0 = dc + tmp[3 + i * 4], a1 = tmp[1 + i * 4] + tmp[2 + i * 4];
        int a2 = tmp[1 + i * 4] - tmp[2 + i * 4], a3 = dc - tmp[3 + i * 4];
        out[0] = (int16_t)((a0 + a1) >> 3); out[16] = (int16_t)((a3 + a2) >> 3);
        out[32] = (int16_t)((a0 - a1) >> 3); out[48] = (int16_t)((a3 - a2) >> 3);
        out += 64;
    }
}

/* ── intra prediction (dst has BPS stride, borders at -1) ── */
#define DST(x, y) dst[(x) + (y) * BPS]
#define AVG3(a, b, c) ((u8)(((a) + 2 * (b) + (c) + 2) >> 2))
#define AVG2(a, b) ((u8)(((a) + (b) + 1) >> 1))
static void pred_tm(u8* dst, int size) {
    const u8* top = dst - BPS; int tl = top[-1], x, y;
    for (y = 0; y < size; ++y) {
        int l = dst[-1];
        for (x = 0; x < size; ++x) dst[x] = (u8)clip8(top[x] + l - tl);
        dst += BPS;
    }
}
static void pred_ve(u8* dst, int size) { int j; for (j = 0; j < size; ++j) memcpy(dst + j * BPS, dst - BPS, (size_t)size); }
static void pred_he(u8* dst, int size) { int j; for (j = 0; j < size; ++j) { memset(dst, dst[-1], (size_t)size); dst += BPS; } }
static void pred_fill(u8* dst, int size, int v) { int j; for (j = 0; j < size; ++j) memset(dst + j * BPS, v, (size_t)size); }
/* DC for 16x16 (size 16, shift 5) and 8x8 chroma (size 8, shift 4) with edge availability */
static void pred_dc(u8* dst, int size, int has_top, int has_left) {
    int s = 0, j, sh = size == 16 ? 4 : 3;
    if (has_top && has_left) {
        for (j = 0; j < size; ++j) s += dst[-1 + j * BPS] + dst[j - BPS];
        pred_fill(dst, size, (s + size) >> (sh + 1));
    } else if (has_left) {
        for (j = 0; j < size; ++j) s += dst[-1 + j * BPS];
        pred_fill(dst, size, (s + (size >> 1)) >> sh);
    } else if (has_top) {
        for (j = 0; j < size; ++j) s += dst[j - BPS];
        pred_fill(dst, size, (s + (size >> 1)) >> sh);
    } else pred_fill(dst, size, 0x80);
}
static void pred_mb(u8* dst, int size, int mode, int mbx, int mby) {
    switch (mode) {
    case B_DC: pred_dc(dst, size, mby > 0, mbx > 0); break;
    case B_TM: pred_tm(dst, size); break;
    case B_VE: pred_ve(dst, size); break;
    default: pred_he(dst, size); break;
    }
}
static void pred4(u8* dst, int mode) {
    const u8* top = dst - BPS;
    int I = dst[-1], J = dst[-1 + BPS], K = dst[-1 + 2 * BPS], L = dst[-1 + 3 * BPS], X = top[-1];
    int A = top[0], B = top[1], C = top[2], D = top[3], E = top[4], F = top[5], G = top[6], H = top[7];
    int i;
    switch (mode) {
    case B_DC: {
        u32 dc = 4;
        for (i = 0; i < 4; ++i) dc += dst[i - BPS] + dst[-1 + i * BPS];
        dc >>= 3;
        for (i = 0; i < 4; ++i) memset(dst + i * BPS, (int)dc, 4);
        break; }
    case B_TM: pred_tm(dst, 4); break;
    case B_VE: {
        u8 v[4];
        v[0] = AVG3(X, A, B); v[1] = AVG3(A, B, C); v[2] = AVG3(B, C, D); v[3] = AVG3(C, D, E);
        for (i = 0; i < 4; ++i) memcpy(dst + i * BPS, v, 4);
        break; }
    case B_HE: {
        int a = AVG3(X, I, J), b = AVG3(I, J, K), c = AVG3(J, K, L), d = AVG3(K, L, L);
        memset(dst, a, 4); memset(dst + BPS, b, 4); memset(dst + 2 * BPS, c, 4); memset(dst + 3 * BPS, d, 4);
        break; }
    case B_RD:
        DST(0, 3) = AVG3(J, K, L);
        DST(1, 3) = DST(0, 2) = AVG3(I, J, K);
        DST(2, 3) = DST(1, 2) = DST(0, 1) = AVG3(X, I, J);
        DST(3, 3) = DST(2, 2) = DST(1, 1) = DST(0, 0) = AVG3(A, X, I);
        DST(3, 2) = DST(2, 1) = DST(1, 0) = AVG3(B, A, X);
        DST(3, 1) = DST(2, 0) = AVG3(C, B, A);
        DST(3, 0) = AVG3(D, C, B);
        break;
    case B_LD:
        DST(0, 0) = AVG3(A, B, C);
        DST(1, 0) = DST(0, 1) = AVG3(B, C, D);
        DST(2, 0) = DST(1, 1) = DST(0, 2) = AVG3(C, D, E);
        DST(3, 0) = DST(2, 1) = DST(1, 2) = DST(0, 3) = AVG3(D, E, F);
        DST(3, 1) = DST(2, 2) = DST(1, 3) = AVG3(E, F, G);
        DST(3, 2) = DST(2, 3) = AVG3(F, G, H);
        DST(3, 3) = AVG3(G, H, H);
        break;
    case B_VR:
        DST(0, 0) = DST(1, 2) = AVG2(X, A);
        DST(1, 0) = DST(2, 2) = AVG2(A, B);
        DST(2, 0) = DST(3, 2) = AVG2(B, C);
        DST(3, 0) = AVG2(C, D);
        DST(0, 3) = AVG3(K, J, I);
        DST(0, 2) = AVG3(J, I, X);
        DST(0, 1) = DST(1, 3) = AVG3(I, X, A);
        DST(1, 1) = DST(2, 3) = AVG3(X, A, B);
        DST(2, 1) = DST(3, 3) = AVG3(A, B, C);
        DST(3, 1) = AVG3(B, C, D);
        break;
    case B_VL:
        DST(0, 0) = AVG2(A, B);
        DST(1, 0) = DST(0, 2) = AVG2(B, C);
        DST(2, 0) = DST(1, 2) = AVG2(C, D);
        DST(3, 0) = DST(2, 2) = AVG2(D, E);
        DST(0, 1) = AVG3(A, B, C);
        DST(1, 1) = DST(0, 3) = AVG3(B, C, D);
        DST(2, 1) = DST(1, 3) = AVG3(C, D, E);
        DST(3, 1) = DST(2, 3) = AVG3(D, E, F);
        DST(3, 2) = AVG3(E, F, G);
        DST(3, 3) = AVG3(F, G, H);
        break;
    case B_HU:
        DST(0, 0) = AVG2(I, J);
        DST(2, 0) = DST(0, 1) = AVG2(J, K);
        DST(2, 1) = DST(0, 2) = AVG2(K, L);
        DST(1, 0) = AVG3(I, J, K);
        DST(3, 0) = DST(1, 1) = AVG3(J, K, L);
        DST(3, 1) = DST(1, 2) = AVG3(K, L, L);
        DST(3, 2) = DST(2, 2) = DST(0, 3) = DST(1, 3) = DST(2, 3) = DST(3, 3) = (u8)L;
        break;
    default: /* B_HD */
        DST(0, 0) = DST(2, 1) = AVG2(I, X);
        DST(0, 1) = DST(2, 2) = AVG2(J, I);
        DST(0, 2) = DST(2, 3) = AVG2(K, J);
        DST(0, 3) = AVG2(L, K);
        DST(3, 0) = AVG3(A, B, C);
        DST(2, 0) = AVG3(X, A, B);
        DST(1, 0) = DST(3, 1) = AVG3(I, X, A);
        DST(1, 1) = DST(3, 2) = AVG3(J, I, X);
        DST(1, 2) = DST(3, 3) = AVG3(K, J, I);
        DST(1, 3) = AVG3(L, K, J);
        break;
    }
}
#undef DST

/* ── loop filters (on frame planes with arbitrary stride) ── */
static int sclip1(int v) { return v < -128 ? -128 : v > 127 ? 127 : v; }
static int sclip2(int v) { return v < -16 ? -16 : v > 15 ? 15 : v; }
static void do_filter2(u8* p, int step) {
    int p1 = p[-2 * step], p0 = p[-step], q0 = p[0], q1 = p[step];
    int a = 3 * (q0 - p0) + sclip1(p1 - q1);
    int a1 = sclip2((a + 4) >> 3), a2 = sclip2((a + 3) >> 3);
    p[-step] = (u8)clip8(p0 + a2); p[0] = (u8)clip8(q0 - a1);
}
static void do_filter4(u8* p, int step) {
    int p1 = p[-2 * step], p0 = p[-step], q0 = p[0], q1 = p[step];
    int a = 3 * (q0 - p0);
    int a1 = sclip2((a + 4) >> 3), a2 = sclip2((a + 3) >> 3), a3 = (a1 + 1) >> 1;
    p[-2 * step] = (u8)clip8(p1 + a3); p[-step] = (u8)clip8(p0 + a2);
    p[0] = (u8)clip8(q0 - a1); p[step] = (u8)clip8(q1 - a3);
}
static void do_filter6(u8* p, int step) {
    int p2 = p[-3 * step], p1 = p[-2 * step], p0 = p[-step], q0 = p[0], q1 = p[step], q2 = p[2 * step];
    int a = sclip1(3 * (q0 - p0) + sclip1(p1 - q1));
    int a1 = (27 * a + 63) >> 7, a2 = (18 * a + 63) >> 7, a3 = (9 * a + 63) >> 7;
    p[-3 * step] = (u8)clip8(p2 + a3); p[-2 * step] = (u8)clip8(p1 + a2); p[-step] = (u8)clip8(p0 + a1);
    p[0] = (u8)clip8(q0 - a1); p[step] = (u8)clip8(q1 - a2); p[2 * step] = (u8)clip8(q2 - a3);
}
static int hev(const u8* p, int step, int t) {
    int p1 = p[-2 * step], p0 = p[-step], q0 = p[0], q1 = p[step];
    return abs(p1 - p0) > t || abs(q1 - q0) > t;
}
static int needs_filter(const u8* p, int step, int t) {
    int p1 = p[-2 * step], p0 = p[-step], q0 = p[0], q1 = p[step];
    return 4 * abs(p0 - q0) + abs(p1 - q1) <= t;
}
static int needs_filter2(const u8* p, int step, int t, int it) {
    int p3 = p[-4 * step], p2 = p[-3 * step], p1 = p[-2 * step], p0 = p[-step];
    int q0 = p[0], q1 = p[step], q2 = p[2 * step], q3 = p[3 * step];
    if (4 * abs(p0 - q0) + abs(p1 - q1) > t) return 0;
    return abs(p3 - p2) <= it && abs(p2 - p1) <= it && abs(p1 - p0) <= it &&
           abs(q3 - q2) <= it && abs(q2 - q1) <= it && abs(q1 - q0) <= it;
}
static void simple_edge(u8* p, int hstride, int vstride, int thresh) {
    int i, t2 = 2 * thresh + 1;
    for (i = 0; i < 16; ++i) if (needs_filter(p + i * vstride, hstride, t2)) do_filter2(p + i * vstride, hstride);
}
static void filter_loop(u8* p, int hstride, int vstride, int size, int thresh, int ithresh, int hev_t, int mb_edge) {
    int t2 = 2 * thresh + 1;
    while (size-- > 0) {
        if (needs_filter2(p, hstride, t2, ithresh)) {
            if (hev(p, hstride, hev_t)) do_filter2(p, hstride);
            else if (mb_edge) do_filter6(p, hstride);
            else do_filter4(p, hstride);
        }
        p += vstride;
    }
}

static void vp8_filter_mb(vp8d* dec, const vfinfo* f, int mbx, int mby, u8* Y, int ys, u8* U, u8* V, int uvs) {
    u8* y = Y + (size_t)mby * 16 * ys + mbx * 16;
    int limit = f->limit, il = f->ilevel, ht = f->hev, k;
    if (limit == 0) return;
    if (dec->filter_type == 1) {
        if (mbx > 0) simple_edge(y, 1, ys, limit + 4);
        if (f->inner) for (k = 1; k < 4; k++) simple_edge(y + 4 * k, 1, ys, limit);
        if (mby > 0) simple_edge(y, ys, 1, limit + 4);
        if (f->inner) for (k = 1; k < 4; k++) simple_edge(y + 4 * k * ys, ys, 1, limit);
    } else {
        u8* u = U + (size_t)mby * 8 * uvs + mbx * 8;
        u8* v = V + (size_t)mby * 8 * uvs + mbx * 8;
        if (mbx > 0) {
            filter_loop(y, 1, ys, 16, limit + 4, il, ht, 1);
            filter_loop(u, 1, uvs, 8, limit + 4, il, ht, 1);
            filter_loop(v, 1, uvs, 8, limit + 4, il, ht, 1);
        }
        if (f->inner) {
            for (k = 1; k < 4; k++) filter_loop(y + 4 * k, 1, ys, 16, limit, il, ht, 0);
            filter_loop(u + 4, 1, uvs, 8, limit, il, ht, 0);
            filter_loop(v + 4, 1, uvs, 8, limit, il, ht, 0);
        }
        if (mby > 0) {
            filter_loop(y, ys, 1, 16, limit + 4, il, ht, 1);
            filter_loop(u, uvs, 1, 8, limit + 4, il, ht, 1);
            filter_loop(v, uvs, 1, 8, limit + 4, il, ht, 1);
        }
        if (f->inner) {
            for (k = 1; k < 4; k++) filter_loop(y + 4 * k * ys, ys, 1, 16, limit, il, ht, 0);
            filter_loop(u + 4 * uvs, uvs, 1, 8, limit, il, ht, 0);
            filter_loop(v + 4 * uvs, uvs, 1, 8, limit, il, ht, 0);
        }
    }
}

/* ── YUV -> RGB (libwebp's fixed-point) and fancy upsampling ── */
static int mult_hi(int v, int c) { return (v * c) >> 8; }
static u8 yuv_clip(int v) { return (u8)(((v & ~16383) == 0) ? (v >> 6) : (v < 0) ? 0 : 255); }
static void yuv_rgba(int y, int u, int v, u8* o) {
    o[0] = yuv_clip(mult_hi(y, 19077) + mult_hi(v, 26149) - 14234);
    o[1] = yuv_clip(mult_hi(y, 19077) - mult_hi(u, 6419) - mult_hi(v, 13320) + 8708);
    o[2] = yuv_clip(mult_hi(y, 19077) + mult_hi(u, 33050) - 17685);
}
#define LOAD_UV(u, v) ((u32)(u) | ((u32)(v) << 16))
static void upsample_pair(const u8* ty, const u8* by, const u8* tu, const u8* tv, const u8* cu, const u8* cv,
                          u8* td, u8* bd, int len) {
    int x, last = (len - 1) >> 1;
    u32 tl = LOAD_UV(tu[0], tv[0]), l = LOAD_UV(cu[0], cv[0]);
    { u32 uv0 = (3 * tl + l + 0x00020002u) >> 2; yuv_rgba(ty[0], (int)(uv0 & 0xff), (int)(uv0 >> 16), td); }
    if (by) { u32 uv0 = (3 * l + tl + 0x00020002u) >> 2; yuv_rgba(by[0], (int)(uv0 & 0xff), (int)(uv0 >> 16), bd); }
    for (x = 1; x <= last; ++x) {
        u32 t = LOAD_UV(tu[x], tv[x]), uv = LOAD_UV(cu[x], cv[x]);
        u32 avg = tl + t + l + uv + 0x00080008u;
        u32 d12 = (avg + 2 * (t + l)) >> 3, d03 = (avg + 2 * (tl + uv)) >> 3;
        { u32 uv0 = (d12 + tl) >> 1, uv1 = (d03 + t) >> 1;
          yuv_rgba(ty[2 * x - 1], (int)(uv0 & 0xff), (int)(uv0 >> 16), td + (2 * x - 1) * 4);
          yuv_rgba(ty[2 * x], (int)(uv1 & 0xff), (int)(uv1 >> 16), td + (2 * x) * 4); }
        if (by) { u32 uv0 = (d03 + l) >> 1, uv1 = (d12 + uv) >> 1;
          yuv_rgba(by[2 * x - 1], (int)(uv0 & 0xff), (int)(uv0 >> 16), bd + (2 * x - 1) * 4);
          yuv_rgba(by[2 * x], (int)(uv1 & 0xff), (int)(uv1 >> 16), bd + (2 * x) * 4); }
        tl = t; l = uv;
    }
    if (!(len & 1)) {
        { u32 uv0 = (3 * tl + l + 0x00020002u) >> 2; yuv_rgba(ty[len - 1], (int)(uv0 & 0xff), (int)(uv0 >> 16), td + (len - 1) * 4); }
        if (by) { u32 uv0 = (3 * l + tl + 0x00020002u) >> 2; yuv_rgba(by[len - 1], (int)(uv0 & 0xff), (int)(uv0 >> 16), bd + (len - 1) * 4); }
    }
}

static void parse_quant(vp8d* dec) {
    int base = bd_val(&dec->br, 7), i;
    int dy1dc = bd_bit(&dec->br, 128) ? bd_sval(&dec->br, 4) : 0;
    int dy2dc = bd_bit(&dec->br, 128) ? bd_sval(&dec->br, 4) : 0;
    int dy2ac = bd_bit(&dec->br, 128) ? bd_sval(&dec->br, 4) : 0;
    int duvdc = bd_bit(&dec->br, 128) ? bd_sval(&dec->br, 4) : 0;
    int duvac = bd_bit(&dec->br, 128) ? bd_sval(&dec->br, 4) : 0;
    for (i = 0; i < 4; i++) {
        int q;
#define QC(v, m) ((v) < 0 ? 0 : (v) > (m) ? (m) : (v))
        if (dec->use_segment) { q = dec->seg_quant[i]; if (!dec->absolute_delta) q += base; }
        else q = base;
        dec->dq[i][0][0] = vp8_dc_table[QC(q + dy1dc, 127)];
        dec->dq[i][0][1] = vp8_ac_table[QC(q, 127)];
        dec->dq[i][1][0] = vp8_dc_table[QC(q + dy2dc, 127)] * 2;
        dec->dq[i][1][1] = (vp8_ac_table[QC(q + dy2ac, 127)] * 101581) >> 16;
        if (dec->dq[i][1][1] < 8) dec->dq[i][1][1] = 8;
        dec->dq[i][2][0] = vp8_dc_table[QC(q + duvdc, 117)];
        dec->dq[i][2][1] = vp8_ac_table[QC(q + duvac, 127)];
#undef QC
    }
}

static void precompute_filter(vp8d* dec) {
    int s, i4;
    for (s = 0; s < 4; s++) {
        int base;
        if (dec->use_segment) { base = dec->seg_filter[s]; if (!dec->absolute_delta) base += dec->filter_level; }
        else base = dec->filter_level;
        for (i4 = 0; i4 <= 1; i4++) {
            vfinfo* f = &dec->fstr[s][i4];
            int level = base;
            if (dec->use_lf_delta) { level += dec->ref_lf_delta[0]; if (i4) level += dec->mode_lf_delta[0]; }
            level = level < 0 ? 0 : level > 63 ? 63 : level;
            if (level > 0) {
                int il = level;
                if (dec->sharpness > 0) {
                    if (dec->sharpness > 4) il >>= 2; else il >>= 1;
                    if (il > 9 - dec->sharpness) il = 9 - dec->sharpness;
                }
                if (il < 1) il = 1;
                f->ilevel = il; f->limit = 2 * level + il;
                f->hev = level >= 40 ? 2 : level >= 15 ? 1 : 0;
            } else f->limit = 0;
            f->inner = i4;
        }
    }
}

typedef struct { u8 nz, nz_dc; } nzctx;

/* Decode a VP8 key frame into RGBA (alpha 255). */
static u8* vp8_decode(const u8* buf, size_t size, int* ow, int* oh, wp_result* res) {
    vp8d* dec = NULL; u32 bits; size_t plen;
    int mbx, mby, i, j;
    u8 *Y = NULL, *U = NULL, *V = NULL, *rgba = NULL;
    u8 *topy = NULL, *topu = NULL, *topv = NULL, *intra_t = NULL;
    nzctx* nzt = NULL;
    vfinfo* finfo = NULL;
    u8 ywork[17 * BPS], uwork[9 * BPS], vwork[9 * BPS];
    u8* yd = ywork + BPS + 8; u8* ud = uwork + BPS + 8; u8* vd = vwork + BPS + 8;
    int ys, uvs;
    const u8* p;
    size_t left;
    if (size < 10) { wp_fail(res, "truncated VP8 header"); return NULL; }
    bits = rd24(buf);
    if (bits & 1) { wp_fail(res, "VP8 frame is not a key frame"); return NULL; }
    if (((bits >> 1) & 7) > 3) { wp_fail(res, "bad VP8 profile"); return NULL; }
    if (!((bits >> 4) & 1)) { wp_fail(res, "VP8 frame not displayable"); return NULL; }
    plen = bits >> 5;
    if (buf[3] != 0x9d || buf[4] != 0x01 || buf[5] != 0x2a) { wp_fail(res, "bad VP8 start code"); return NULL; }
    dec = (vp8d*)calloc(1, sizeof(vp8d));
    if (!dec) { wp_fail(res, "out of memory"); return NULL; }
    dec->w = ((buf[7] << 8) | buf[6]) & 0x3fff;
    dec->h = ((buf[9] << 8) | buf[8]) & 0x3fff;
    if (dec->w == 0 || dec->h == 0) { wp_fail(res, "zero VP8 dimensions"); goto fail; }
    dec->mbw = (dec->w + 15) >> 4; dec->mbh = (dec->h + 15) >> 4;
    p = buf + 10; left = size - 10;
    if (plen > left) { wp_fail(res, "bad VP8 partition length"); goto fail; }
    bd_init(&dec->br, p, plen);
    p += plen; left -= plen;
    bd_bit(&dec->br, 128); bd_bit(&dec->br, 128);           /* colour space, clamp type */
    /* segment header */
    dec->absolute_delta = 1;
    dec->seg_proba[0] = dec->seg_proba[1] = dec->seg_proba[2] = 255;
    dec->use_segment = bd_bit(&dec->br, 128);
    if (dec->use_segment) {
        dec->update_map = bd_bit(&dec->br, 128);
        if (bd_bit(&dec->br, 128)) {
            dec->absolute_delta = bd_bit(&dec->br, 128);
            for (i = 0; i < 4; i++) dec->seg_quant[i] = bd_bit(&dec->br, 128) ? bd_sval(&dec->br, 7) : 0;
            for (i = 0; i < 4; i++) dec->seg_filter[i] = bd_bit(&dec->br, 128) ? bd_sval(&dec->br, 6) : 0;
        }
        if (dec->update_map)
            for (i = 0; i < 3; i++) dec->seg_proba[i] = (u8)(bd_bit(&dec->br, 128) ? bd_val(&dec->br, 8) : 255);
    }
    /* filter header */
    dec->filter_simple = bd_bit(&dec->br, 128);
    dec->filter_level = bd_val(&dec->br, 6);
    dec->sharpness = bd_val(&dec->br, 3);
    dec->use_lf_delta = bd_bit(&dec->br, 128);
    if (dec->use_lf_delta && bd_bit(&dec->br, 128)) {
        for (i = 0; i < 4; i++) if (bd_bit(&dec->br, 128)) dec->ref_lf_delta[i] = bd_sval(&dec->br, 6);
        for (i = 0; i < 4; i++) if (bd_bit(&dec->br, 128)) dec->mode_lf_delta[i] = bd_sval(&dec->br, 6);
    }
    dec->filter_type = dec->filter_level == 0 ? 0 : dec->filter_simple ? 1 : 2;
    /* partitions */
    dec->nparts = 1 << bd_val(&dec->br, 2);
    {
        const u8* sz = p; size_t nlast = (size_t)dec->nparts - 1;
        const u8* ps;
        if (left < 3 * nlast) { wp_fail(res, "truncated VP8 partitions"); goto fail; }
        ps = p + 3 * nlast; left -= 3 * nlast;
        for (i = 0; i < (int)nlast; i++) {
            size_t psz = rd24(sz);
            if (psz > left) psz = left;
            bd_init(&dec->parts[i], ps, psz);
            ps += psz; left -= psz; sz += 3;
        }
        bd_init(&dec->parts[nlast], ps, left);
    }
    parse_quant(dec);
    bd_bit(&dec->br, 128);                                   /* refresh entropy probs (ignored) */
    {
        int t;
        for (t = 0; t < 1056; t++)
            dec->coef[t] = (u8)(bd_bit(&dec->br, vp8_coef_update[t]) ? bd_val(&dec->br, 8) : vp8_coef_proba0[t]);
    }
    dec->use_skip = bd_bit(&dec->br, 128);
    if (dec->use_skip) dec->skip_p = bd_val(&dec->br, 8);
    if (dec->br.eof > 2) { wp_fail(res, "truncated VP8 header"); goto fail; }
    precompute_filter(dec);

    ys = dec->mbw * 16; uvs = dec->mbw * 8;
    Y = (u8*)malloc((size_t)ys * dec->mbh * 16);
    U = (u8*)malloc((size_t)uvs * dec->mbh * 8);
    V = (u8*)malloc((size_t)uvs * dec->mbh * 8);
    topy = (u8*)malloc((size_t)ys); topu = (u8*)malloc((size_t)uvs); topv = (u8*)malloc((size_t)uvs);
    intra_t = (u8*)calloc((size_t)dec->mbw * 4, 1);
    nzt = (nzctx*)calloc((size_t)dec->mbw, sizeof(nzctx));
    finfo = (vfinfo*)calloc((size_t)dec->mbw * dec->mbh, sizeof(vfinfo));
    if (!Y || !U || !V || !topy || !topu || !topv || !intra_t || !nzt || !finfo) { wp_fail(res, "out of memory"); goto fail; }

    for (mby = 0; mby < dec->mbh; mby++) {
        bdec* tbr = &dec->parts[mby & (dec->nparts - 1)];
        u8 intra_l[4] = { 0, 0, 0, 0 };
        nzctx lnz = { 0, 0 };
        memset(ywork, 0, sizeof ywork); memset(uwork, 0, sizeof uwork); memset(vwork, 0, sizeof vwork);
        for (j = 0; j < 16; ++j) yd[j * BPS - 1] = 129;
        for (j = 0; j < 8; ++j) { ud[j * BPS - 1] = 129; vd[j * BPS - 1] = 129; }
        if (mby > 0) yd[-1 - BPS] = ud[-1 - BPS] = vd[-1 - BPS] = 129;
        else { memset(yd - BPS - 1, 127, 16 + 4 + 1); memset(ud - BPS - 1, 127, 8 + 1); memset(vd - BPS - 1, 127, 8 + 1); }
        for (mbx = 0; mbx < dec->mbw; mbx++) {
            int segment = 0, skip = 0, is4, uvmode, ymode = 0;
            u8 imodes[16];
            int16_t coeffs[384];
            u8 bnz[24];                                      /* per 4x4 block: any coefficient? */
            int all_zero = 1;
            nzctx* mb = &nzt[mbx];
            /* ---- modes (partition 0) ---- */
            if (dec->update_map)
                segment = !bd_bit(&dec->br, dec->seg_proba[0]) ? bd_bit(&dec->br, dec->seg_proba[1])
                                                               : bd_bit(&dec->br, dec->seg_proba[2]) + 2;
            if (dec->use_skip) skip = bd_bit(&dec->br, dec->skip_p);
            is4 = !bd_bit(&dec->br, 145);
            if (!is4) {
                ymode = bd_bit(&dec->br, 156) ? (bd_bit(&dec->br, 128) ? B_TM : B_HE)
                                              : (bd_bit(&dec->br, 163) ? B_VE : B_DC);
                memset(intra_t + mbx * 4, ymode, 4);
                memset(intra_l, ymode, 4);
            } else {
                u8* top = intra_t + mbx * 4;
                int y, x;
                for (y = 0; y < 4; ++y) {
                    int m = intra_l[y];
                    for (x = 0; x < 4; ++x) {
                        const u8* pr = vp8_bmode_proba + (top[x] * 10 + m) * 9;
                        m = !bd_bit(&dec->br, pr[0]) ? B_DC
                          : !bd_bit(&dec->br, pr[1]) ? B_TM
                          : !bd_bit(&dec->br, pr[2]) ? B_VE
                          : !bd_bit(&dec->br, pr[3])
                              ? (!bd_bit(&dec->br, pr[4]) ? B_HE : (!bd_bit(&dec->br, pr[5]) ? B_RD : B_VR))
                              : (!bd_bit(&dec->br, pr[6]) ? B_LD
                                 : (!bd_bit(&dec->br, pr[7]) ? B_VL : (!bd_bit(&dec->br, pr[8]) ? B_HD : B_HU)));
                        top[x] = (u8)m;
                    }
                    memcpy(imodes + y * 4, top, 4);
                    intra_l[y] = (u8)m;
                }
            }
            uvmode = !bd_bit(&dec->br, 142) ? B_DC : !bd_bit(&dec->br, 114) ? B_VE : bd_bit(&dec->br, 183) ? B_TM : B_HE;
            /* ---- residuals ---- */
            memset(coeffs, 0, sizeof coeffs);
            memset(bnz, 0, sizeof bnz);
            if (!skip) {
                const int (*q)[2] = dec->dq[segment];
                int first, t_ac, x, y, ch;
                u8 tnz, lnzb;
                u32 out_t, out_l;
                int16_t* dst = coeffs;
                if (!is4) {
                    int16_t dc[16];
                    int ctx = mb->nz_dc + lnz.nz_dc, nz;
                    memset(dc, 0, sizeof dc);
                    nz = get_coeffs(dec, tbr, 1, ctx, q[1], 0, dc);
                    mb->nz_dc = lnz.nz_dc = (u8)(nz > 0);
                    iwht(dc, dst);
                    first = 1; t_ac = 0;
                } else { first = 0; t_ac = 3; }
                tnz = mb->nz & 0x0f; lnzb = lnz.nz & 0x0f;
                for (y = 0; y < 4; ++y) {
                    int l = lnzb & 1;
                    for (x = 0; x < 4; ++x) {
                        int ctx = l + (tnz & 1);
                        int nz = get_coeffs(dec, tbr, t_ac, ctx, q[0], first, dst);
                        int k, any = 0;
                        l = nz > first;
                        tnz = (u8)((tnz >> 1) | (l << 7));
                        for (k = 0; k < 16; k++) if (dst[k]) { any = 1; break; }
                        bnz[y * 4 + x] = (u8)any;
                        /* libwebp's notion of "non-zero" for the skip / inner-filter flag */
                        if (nz > 1 || dst[0] != 0) all_zero = 0;
                        dst += 16;
                    }
                    tnz >>= 4;
                    lnzb = (u8)((lnzb >> 1) | (l << 7));
                }
                out_t = tnz; out_l = (u32)(lnzb >> 4);
                for (ch = 0; ch < 4; ch += 2) {
                    tnz = (u8)(mb->nz >> (4 + ch));
                    lnzb = (u8)(lnz.nz >> (4 + ch));
                    for (y = 0; y < 2; ++y) {
                        int l = lnzb & 1;
                        for (x = 0; x < 2; ++x) {
                            int ctx = l + (tnz & 1);
                            int nz = get_coeffs(dec, tbr, 2, ctx, q[2], 0, dst);
                            int k, any = 0;
                            l = nz > 0;
                            tnz = (u8)((tnz >> 1) | (l << 3));
                            for (k = 0; k < 16; k++) if (dst[k]) { any = 1; break; }
                            bnz[16 + ch * 2 + y * 2 + x] = (u8)any;
                            if (nz > 1 || dst[0] != 0) all_zero = 0;
                            dst += 16;
                        }
                        tnz >>= 2;
                        lnzb = (u8)((lnzb >> 1) | (l << 5));
                    }
                    out_t |= (u32)(tnz << 4) << ch;
                    out_l |= (u32)(lnzb & 0xf0) << ch;
                }
                mb->nz = (u8)out_t; lnz.nz = (u8)out_l;
                skip = all_zero;
            } else {
                lnz.nz = mb->nz = 0;
                if (!is4) lnz.nz_dc = mb->nz_dc = 0;
            }
            if (tbr->eof > 2 || dec->br.eof > 2) { wp_fail(res, "truncated VP8 data"); goto fail; }
            if (dec->filter_type > 0) {
                vfinfo* f = &finfo[mby * dec->mbw + mbx];
                *f = dec->fstr[segment][is4];
                f->inner |= !skip;
            }
            /* ---- reconstruct ---- */
            if (mbx > 0) {
                for (j = -1; j < 16; ++j) memcpy(&yd[j * BPS - 4], &yd[j * BPS + 12], 4);
                for (j = -1; j < 8; ++j) { memcpy(&ud[j * BPS - 4], &ud[j * BPS + 4], 4); memcpy(&vd[j * BPS - 4], &vd[j * BPS + 4], 4); }
            }
            if (mby > 0) {
                memcpy(yd - BPS, topy + mbx * 16, 16);
                memcpy(ud - BPS, topu + mbx * 8, 8);
                memcpy(vd - BPS, topv + mbx * 8, 8);
            }
            if (is4) {
                u8* tr = yd - BPS + 16;
                int n;
                if (mby > 0) {
                    if (mbx >= dec->mbw - 1) memset(tr, topy[mbx * 16 + 15], 4);
                    else memcpy(tr, topy + (mbx + 1) * 16, 4);
                }
                memcpy(tr + 4 * BPS, tr, 4); memcpy(tr + 8 * BPS, tr, 4); memcpy(tr + 12 * BPS, tr, 4);
                for (n = 0; n < 16; ++n) {
                    u8* d = yd + (n & 3) * 4 + (n >> 2) * 4 * BPS;
                    pred4(d, imodes[n]);
                    if (bnz[n]) idct_add(coeffs + n * 16, d);
                }
            } else {
                int n;
                pred_mb(yd, 16, ymode, mbx, mby);
                for (n = 0; n < 16; ++n)
                    if (bnz[n]) idct_add(coeffs + n * 16, yd + (n & 3) * 4 + (n >> 2) * 4 * BPS);
            }
            pred_mb(ud, 8, uvmode, mbx, mby);
            pred_mb(vd, 8, uvmode, mbx, mby);
            for (i = 0; i < 4; i++) {
                if (bnz[16 + i]) idct_add(coeffs + (16 + i) * 16, ud + (i & 1) * 4 + (i >> 1) * 4 * BPS);
                if (bnz[20 + i]) idct_add(coeffs + (20 + i) * 16, vd + (i & 1) * 4 + (i >> 1) * 4 * BPS);
            }
            if (mby < dec->mbh - 1) {
                memcpy(topy + mbx * 16, yd + 15 * BPS, 16);
                memcpy(topu + mbx * 8, ud + 7 * BPS, 8);
                memcpy(topv + mbx * 8, vd + 7 * BPS, 8);
            }
            for (j = 0; j < 16; ++j) memcpy(Y + ((size_t)mby * 16 + j) * ys + mbx * 16, yd + j * BPS, 16);
            for (j = 0; j < 8; ++j) {
                memcpy(U + ((size_t)mby * 8 + j) * uvs + mbx * 8, ud + j * BPS, 8);
                memcpy(V + ((size_t)mby * 8 + j) * uvs + mbx * 8, vd + j * BPS, 8);
            }
        }
    }
    if (dec->filter_type > 0)
        for (mby = 0; mby < dec->mbh; mby++)
            for (mbx = 0; mbx < dec->mbw; mbx++)
                vp8_filter_mb(dec, &finfo[mby * dec->mbw + mbx], mbx, mby, Y, ys, U, V, uvs);
    /* ---- colour conversion ---- */
    {
        int w = dec->w, h = dec->h, y;
        size_t st = (size_t)w * 4;
        rgba = (u8*)malloc((size_t)w * h * 4);
        if (!rgba) { wp_fail(res, "out of memory"); goto fail; }
        upsample_pair(Y, NULL, U, V, U, V, rgba, NULL, w);
        for (y = 1; y + 1 < h; y += 2) {
            const u8* tu = U + (size_t)((y - 1) >> 1) * uvs; const u8* tv = V + (size_t)((y - 1) >> 1) * uvs;
            const u8* cu = U + (size_t)((y + 1) >> 1) * uvs; const u8* cv = V + (size_t)((y + 1) >> 1) * uvs;
            upsample_pair(Y + (size_t)y * ys, Y + (size_t)(y + 1) * ys, tu, tv, cu, cv, rgba + y * st, rgba + (y + 1) * st, w);
        }
        if (!(h & 1) && h > 1) {
            const u8* cu = U + (size_t)((h - 1) >> 1) * uvs; const u8* cv = V + (size_t)((h - 1) >> 1) * uvs;
            upsample_pair(Y + (size_t)(h - 1) * ys, NULL, cu, cv, cu, cv, rgba + (size_t)(h - 1) * st, NULL, w);
        }
        for (i = 0; i < w * h; i++) rgba[(size_t)i * 4 + 3] = 255;
        *ow = w; *oh = h;
    }
    free(Y); free(U); free(V); free(topy); free(topu); free(topv); free(intra_t); free(nzt); free(finfo); free(dec);
    return rgba;
fail:
    free(Y); free(U); free(V); free(topy); free(topu); free(topv); free(intra_t); free(nzt); free(finfo); free(dec);
    free(rgba);
    return NULL;
}

/* ALPH chunk -> alpha plane applied to rgba (w*h). 0 = ok. */
static int alph_apply(const u8* d, size_t n, int w, int h, u8* rgba, wp_result* res) {
    int method, filter, x, y;
    u8* a;
    if (n < 1) { wp_fail(res, "empty ALPH chunk"); return -1; }
    method = d[0] & 3; filter = (d[0] >> 2) & 3;
    a = (u8*)malloc((size_t)w * h);
    if (!a) { wp_fail(res, "out of memory"); return -1; }
    if (method == 0) {
        if (n - 1 < (size_t)w * h) { free(a); wp_fail(res, "truncated ALPH data"); return -1; }
        memcpy(a, d + 1, (size_t)w * h);
    } else if (method == 1) {
        lbr b; u32* argb; size_t i;
        lbr_init(&b, d + 1, n - 1);
        argb = vp8l_decode_stream(&b, w, h);
        if (!argb) { free(a); wp_fail(res, "corrupt ALPH data"); return -1; }
        for (i = 0; i < (size_t)w * h; i++) a[i] = (u8)(argb[i] >> 8);
        free(argb);
    } else { free(a); wp_fail(res, "unknown ALPH compression"); return -1; }
    /* unfilter in place */
    for (y = 0; y < h; y++) {
        u8* row = a + (size_t)y * w;
        const u8* prev = y > 0 ? row - w : NULL;
        if (filter == 0) continue;
        if (filter == 1 || prev == NULL) {
            u8 pred = prev ? prev[0] : 0;
            for (x = 0; x < w; x++) { row[x] = (u8)(pred + row[x]); pred = row[x]; }
        } else if (filter == 2) {
            for (x = 0; x < w; x++) row[x] = (u8)(prev[x] + row[x]);
        } else {
            int top = prev[0], tl = top, left = top;
            for (x = 0; x < w; x++) {
                int g;
                top = prev[x];
                g = left + top - tl; g = g < 0 ? 0 : g > 255 ? 255 : g;
                left = (u8)(row[x] + g);
                tl = top;
                row[x] = (u8)left;
            }
        }
    }
    for (x = 0; x < w * h; x++) rgba[(size_t)x * 4 + 3] = a[x];
    free(a);
    return 0;
}

/* Decode one frame (ALPH? + VP8, or VP8L). */
static u8* decode_frame(const u8* alph, size_t alen, const u8* bs, size_t blen, int lossless, int* w, int* h, int* has_alpha, wp_result* res) {
    u8* rgba;
    if (lossless) {
        rgba = vp8l_decode(bs, blen, w, h, has_alpha, res);
        return rgba;
    }
    rgba = vp8_decode(bs, blen, w, h, res);
    if (!rgba) { if (!res->err[0]) wp_fail(res, "corrupt VP8 data"); return NULL; }
    *has_alpha = 0;
    if (alph) {
        if (alph_apply(alph, alen, *w, *h, rgba, res) != 0) { free(rgba); return NULL; }
        *has_alpha = 1;
    }
    return rgba;
}

/* libwebp's non-premultiplied blend of src over dst (RGBA byte order). */
static void blend_px(u8* src, const u8* dst) {
    int sa = src[3];
    if (sa == 0) { memcpy(src, dst, 4); return; }
    {
        int da = dst[3];
        int dfa = (da * (256 - sa)) >> 8;
        int ba = sa + dfa;
        u32 scale = (1u << 24) / (u32)ba;
        int c;
        for (c = 0; c < 3; c++) {
            u32 bu = (u32)src[c] * (u32)sa + (u32)dst[c] * (u32)dfa;
            src[c] = (u8)((bu * scale) >> 24);
        }
        src[3] = (u8)ba;
    }
}

typedef struct { int x, y, w, h, dispose, blend, has_alpha; } fr_info;

void* trc_webp_decode(const char* data, int64_t n) {
    const u8* d = (const u8*)data;
    wp_result* r = (wp_result*)calloc(1, sizeof(wp_result));
    size_t riff_end, pos;
    int vp8x = 0, flags = 0, cw = 0, ch = 0;
    const u8* alph = NULL; size_t alen = 0;
    u8* canvas = NULL; u8* disposed = NULL;
    fr_info prev; int prev_key = 0, nfr = 0;
    if (!r) return NULL;
    memset(&prev, 0, sizeof prev);
    if (n < 20 || memcmp(d, "RIFF", 4) != 0 || memcmp(d + 8, "WEBP", 4) != 0) { wp_fail(r, "not a RIFF/WEBP file"); return r; }
    riff_end = (size_t)rd32(d + 4) + 8;
    if (riff_end > (size_t)n) riff_end = (size_t)n;   /* tolerate truncated RIFF size */
    pos = 12;
    while (pos + 8 <= riff_end) {
        const u8* c = d + pos;
        size_t sz = rd32(c + 4), body = pos + 8;
        if (sz > riff_end - body) {
            if (memcmp(c, "VP8 ", 4) == 0 || memcmp(c, "VP8L", 4) == 0) sz = riff_end - body;   /* truncated: let the codec complain */
            else { wp_fail(r, "truncated chunk"); break; }
        }
        if (memcmp(c, "VP8X", 4) == 0) {
            if (sz < 10) { wp_fail(r, "bad VP8X chunk"); break; }
            vp8x = 1; flags = c[8];
            cw = (int)rd24(c + 12) + 1; ch = (int)rd24(c + 15) + 1;
            if ((size_t)cw * (size_t)ch > WP_MAX_PIXELS) { wp_fail(r, "image too large"); break; }
            r->w = cw; r->h = ch;
            r->animated = (flags & 2) != 0;
            r->loop = 0;
        } else if (memcmp(c, "ANIM", 4) == 0) {
            if (sz >= 6) r->loop = c[12] | (c[13] << 8);
        } else if (memcmp(c, "ALPH", 4) == 0) {
            alph = d + body; alen = sz;
        } else if ((memcmp(c, "VP8 ", 4) == 0 || memcmp(c, "VP8L", 4) == 0) && !r->animated) {
            int w = 0, h = 0, ha = 0, ll = c[3] == 'L';
            u8* rgba = decode_frame(ll ? NULL : alph, alen, d + body, sz, ll, &w, &h, &ha, r);
            if (!rgba) break;
            if (vp8x && (w != cw || h != ch)) { free(rgba); wp_fail(r, "frame size does not match canvas"); break; }
            r->w = w; r->h = h; r->has_alpha = ll ? ha : ha; r->lossless = ll;
            if (wp_add_frame(r, rgba, 0) != 0) { free(rgba); wp_fail(r, "out of memory"); }
            break;                                     /* still image: done */
        } else if (memcmp(c, "ANMF", 4) == 0 && r->animated) {
            fr_info fi; size_t sp, send = body + sz;
            const u8* fal = NULL; size_t fal_n = 0; const u8* fbs = NULL; size_t fbs_n = 0; int fll = 0;
            u8* rgba; int w = 0, h = 0, ha = 0, key, yy, xx;
            if (sz < 16) { wp_fail(r, "bad ANMF chunk"); break; }
            fi.x = (int)rd24(c + 8) * 2; fi.y = (int)rd24(c + 11) * 2;
            fi.w = (int)rd24(c + 14) + 1; fi.h = (int)rd24(c + 17) + 1;
            {
                int dur = (int)rd24(c + 20), fl = c[23];
                fi.dispose = fl & 1; fi.blend = !(fl & 2);
                sp = body + 16;
                while (sp + 8 <= send) {
                    const u8* s = d + sp; size_t ssz = rd32(s + 4);
                    if (ssz > send - sp - 8) ssz = send - sp - 8;
                    if (memcmp(s, "ALPH", 4) == 0) { fal = s + 8; fal_n = ssz; }
                    else if (memcmp(s, "VP8 ", 4) == 0 || memcmp(s, "VP8L", 4) == 0) { fbs = s + 8; fbs_n = ssz; fll = s[3] == 'L'; break; }
                    sp += 8 + ssz + (ssz & 1);
                }
                if (!fbs) { wp_fail(r, "ANMF without image data"); break; }
                if (fi.x + fi.w > cw || fi.y + fi.h > ch) { wp_fail(r, "frame outside the canvas"); break; }
                rgba = decode_frame(fll ? NULL : fal, fal_n, fbs, fbs_n, fll, &w, &h, &ha, r);
                if (!rgba) break;
                if (w != fi.w || h != fi.h) { free(rgba); wp_fail(r, "frame size mismatch"); break; }
                fi.has_alpha = fll ? ha : (fal != NULL);
                if (!canvas) {
                    canvas = (u8*)calloc((size_t)cw * ch, 4); disposed = (u8*)calloc((size_t)cw * ch, 4);
                    if (!canvas || !disposed) { free(rgba); wp_fail(r, "out of memory"); break; }
                }
                /* key frame logic of WebPAnimDecoder */
                if (nfr == 0) key = 1;
                else if ((!fi.has_alpha || !fi.blend) && fi.w == cw && fi.h == ch) key = 1;
                else key = prev.dispose && ((prev.w == cw && prev.h == ch) || prev_key);
                if (key) memset(canvas, 0, (size_t)cw * ch * 4);
                else memcpy(canvas, disposed, (size_t)cw * ch * 4);
                for (yy = 0; yy < fi.h; yy++)
                    memcpy(canvas + ((size_t)(fi.y + yy) * cw + fi.x) * 4, rgba + (size_t)yy * fi.w * 4, (size_t)fi.w * 4);
                free(rgba);
                if (nfr > 0 && fi.blend && !key) {
                    for (yy = 0; yy < fi.h; yy++)
                        for (xx = 0; xx < fi.w; xx++) {
                            int cx = fi.x + xx, cy = fi.y + yy;
                            size_t o = ((size_t)cy * cw + cx) * 4;
                            if (prev.dispose && cx >= prev.x && cx < prev.x + prev.w && cy >= prev.y && cy < prev.y + prev.h) continue;
                            blend_px(canvas + o, disposed + o);
                        }
                }
                memcpy(disposed, canvas, (size_t)cw * ch * 4);
                if (fi.dispose)
                    for (yy = 0; yy < fi.h; yy++) memset(disposed + ((size_t)(fi.y + yy) * cw + fi.x) * 4, 0, (size_t)fi.w * 4);
                {
                    u8* copy = (u8*)malloc((size_t)cw * ch * 4);
                    if (!copy) { wp_fail(r, "out of memory"); break; }
                    memcpy(copy, canvas, (size_t)cw * ch * 4);
                    if (wp_add_frame(r, copy, dur) != 0) { free(copy); wp_fail(r, "out of memory"); break; }
                }
                if (fi.has_alpha) r->has_alpha = 1;
                if (fll) r->lossless = 1;
                prev = fi; prev_key = key; nfr++;
            }
        }
        pos = body + sz + (sz & 1);
    }
    free(canvas); free(disposed);
    if (!r->err[0] && r->nframes == 0) wp_fail(r, "no image data");
    if (vp8x && (flags & 0x10) && !r->animated) r->has_alpha = r->has_alpha || alph != NULL;
    return r;
}

/* ═════════════════════════════ VP8L encoder ══════════════════════════════ */
typedef struct { u8* buf; size_t len, cap; u64 acc; int n; int err; } bwr;
static void bw_byte(bwr* w, u8 b) {
    if (w->len == w->cap) {
        size_t nc = w->cap ? w->cap * 2 : 4096;
        u8* nb = (u8*)realloc(w->buf, nc);
        if (!nb) { w->err = 1; return; }
        w->buf = nb; w->cap = nc;
    }
    w->buf[w->len++] = b;
}
static void bw_put(bwr* w, u32 v, int nb) {
    if (nb == 0) return;
    w->acc |= (u64)(v & ((nb == 32) ? 0xffffffffu : ((1u << nb) - 1))) << w->n;
    w->n += nb;
    while (w->n >= 8) { bw_byte(w, (u8)w->acc); w->acc >>= 8; w->n -= 8; }
}
static void bw_flush(bwr* w) { if (w->n > 0) { bw_byte(w, (u8)w->acc); w->acc = 0; w->n = 0; } }

/* Length-limited Huffman code lengths (max `maxlen`) for n symbols. */
typedef struct { u32 f; int l, r; } hnode;
static int hcmp_u32(const void* a, const void* b) {
    const u32* x = (const u32*)a; const u32* y = (const u32*)b;
    if (x[0] != y[0]) return x[0] < y[0] ? -1 : 1;
    return x[1] < y[1] ? -1 : x[1] > y[1];
}
static void set_depth(const hnode* t, int i, int d, int nleaves, u8* lens, const int* leafsym, int* maxd) {
    /* iterative DFS */
    int stack[2 * 2400 + 8][2]; int sp = 0;
    stack[sp][0] = i; stack[sp][1] = d; sp++;
    while (sp > 0) {
        int node, depth;
        sp--; node = stack[sp][0]; depth = stack[sp][1];
        if (node < nleaves) { lens[leafsym[node]] = (u8)(depth ? depth : 1); if (depth > *maxd) *maxd = depth; }
        else { stack[sp][0] = t[node].l; stack[sp][1] = depth + 1; sp++; stack[sp][0] = t[node].r; stack[sp][1] = depth + 1; sp++; }
    }
}
static int huff_lengths(const u32* freq, int n, int maxlen, u8* lens) {
    int i, nl = 0; u32 floor_ = 1;
    int* leafsym = (int*)malloc(sizeof(int) * (size_t)n);
    u32 (*srt)[2] = (u32(*)[2])malloc(sizeof(u32) * 2 * (size_t)n);
    hnode* t = (hnode*)malloc(sizeof(hnode) * (size_t)(2 * n + 2));
    memset(lens, 0, (size_t)n);
    if (!leafsym || !srt || !t) { free(leafsym); free(srt); free(t); return -1; }
    for (i = 0; i < n; i++) if (freq[i]) nl++;
    if (nl == 0) { free(leafsym); free(srt); free(t); return 0; }
    if (nl == 1) { for (i = 0; i < n; i++) if (freq[i]) lens[i] = 1; free(leafsym); free(srt); free(t); return 0; }
    for (;;) {
        int k = 0, a, b2, qa = 0, qb, nn, maxd = 0;
        for (i = 0; i < n; i++) if (freq[i]) { srt[k][0] = freq[i] < floor_ ? floor_ : freq[i]; srt[k][1] = (u32)i; k++; }
        qsort(srt, (size_t)k, sizeof srt[0], hcmp_u32);
        for (i = 0; i < k; i++) { t[i].f = srt[i][0]; t[i].l = t[i].r = -1; leafsym[i] = (int)srt[i][1]; }
        /* two-queue merge: leaves [qa, k), internal [qb, nn) */
        qb = k; nn = k;
        while ((k - qa) + (nn - qb) > 1) {
            int pick[2], m;
            for (m = 0; m < 2; m++) {
                if (qa < k && (qb >= nn || t[qa].f <= t[qb].f)) pick[m] = qa++;
                else pick[m] = qb++;
            }
            a = pick[0]; b2 = pick[1];
            t[nn].f = t[a].f + t[b2].f; t[nn].l = a; t[nn].r = b2; nn++;
        }
        set_depth(t, nn - 1, 0, k, lens, leafsym, &maxd);
        if (maxd <= maxlen) break;
        floor_ *= 2;
    }
    free(leafsym); free(srt); free(t);
    return 0;
}
static void canon_codes(const u8* lens, int n, u16* codes) {
    u16 count[17]; u32 next[18]; int i, l;
    memset(count, 0, sizeof count);
    for (i = 0; i < n; i++) count[lens[i]]++;
    count[0] = 0; next[1] = 0;
    for (l = 1; l <= 16; l++) next[l + 1] = (next[l] + count[l]) << 1;
    for (i = 0; i < n; i++) {
        int L = lens[i], k; u32 c, rev = 0;
        if (!L) { codes[i] = 0; continue; }
        c = next[L]++;
        for (k = 0; k < L; k++) if (c & (1u << k)) rev |= 1u << (L - 1 - k);
        codes[i] = (u16)rev;
    }
}

typedef struct { int n; u8* lens; u16* codes; int zero_bits; } ecode;

/* Build + write a prefix code for histogram `freq` of `n` symbols. */
static int write_code(bwr* w, const u32* freq, int n, ecode* ec) {
    int i, used = 0, s[2] = { 0, 0 };
    ec->n = n; ec->zero_bits = 0;
    ec->lens = (u8*)calloc((size_t)n, 1); ec->codes = (u16*)calloc((size_t)n, 2);
    if (!ec->lens || !ec->codes) return -1;
    for (i = 0; i < n; i++) if (freq[i]) { if (used < 2) s[used] = i; used++; }
    if (used <= 2 && s[0] < 256 && s[1] < 256) {
        bw_put(w, 1, 1);
        if (used <= 1) {
            ec->zero_bits = 1;
            bw_put(w, 0, 1);
            if (s[0] <= 1) { bw_put(w, 0, 1); bw_put(w, (u32)s[0], 1); }
            else { bw_put(w, 1, 1); bw_put(w, (u32)s[0], 8); }
            return 0;
        }
        bw_put(w, 1, 1);
        if (s[0] <= 1) { bw_put(w, 0, 1); bw_put(w, (u32)s[0], 1); }
        else { bw_put(w, 1, 1); bw_put(w, (u32)s[0], 8); }
        bw_put(w, (u32)s[1], 8);
        ec->lens[s[0]] = 1; ec->lens[s[1]] = 1;
        canon_codes(ec->lens, n, ec->codes);
        return 0;
    }
    if (huff_lengths(freq, n, 15, ec->lens) != 0) return -1;
    if (used == 1) ec->zero_bits = 1;
    canon_codes(ec->lens, n, ec->codes);
    {
        /* RLE the code lengths into code-length symbols */
        int* tok = (int*)malloc(sizeof(int) * 2 * (size_t)n); int nt = 0;
        u32 clf[19]; u8 cll[19]; u16 clc[19]; int ncl, k;
        if (!tok) return -1;
        memset(clf, 0, sizeof clf);
        i = 0;
        while (i < n) {
            int v = ec->lens[i], run = 1;
            while (i + run < n && ec->lens[i + run] == v) run++;
            if (v == 0 && run >= 3) {
                int r = run;
                while (r >= 3) {
                    if (r >= 11) { int m = r > 138 ? 138 : r; tok[nt++] = 18 | ((m - 11) << 8); r -= m; }
                    else { int m = r > 10 ? 10 : r; tok[nt++] = 17 | ((m - 3) << 8); r -= m; }
                }
                while (r-- > 0) tok[nt++] = 0;
            } else if (v != 0 && run >= 4) {
                int r = run - 1;
                tok[nt++] = v;
                while (r >= 3) { int m = r > 6 ? 6 : r; tok[nt++] = 16 | ((m - 3) << 8); r -= m; }
                while (r-- > 0) tok[nt++] = v;
            } else { int r = run; while (r-- > 0) tok[nt++] = v; }
            i += run;
        }
        for (k = 0; k < nt; k++) clf[tok[k] & 0xff]++;
        if (huff_lengths(clf, 19, 7, cll) != 0) { free(tok); return -1; }
        canon_codes(cll, 19, clc);
        {
            int nz = 0;
            for (k = 0; k < 19; k++) if (cll[k]) nz++;
            ncl = 19;
            while (ncl > 4 && cll[kCodeLengthOrder[ncl - 1]] == 0) ncl--;
            bw_put(w, 0, 1);                              /* normal code */
            bw_put(w, (u32)(ncl - 4), 4);
            for (k = 0; k < ncl; k++) bw_put(w, cll[kCodeLengthOrder[k]], 3);
            bw_put(w, 0, 1);                              /* no max_symbol */
            for (k = 0; k < nt; k++) {
                int sym = tok[k] & 0xff, ex = tok[k] >> 8;
                if (nz > 1) bw_put(w, clc[sym], cll[sym]);
                if (sym == 16) bw_put(w, (u32)ex, 2);
                else if (sym == 17) bw_put(w, (u32)ex, 3);
                else if (sym == 18) bw_put(w, (u32)ex, 7);
            }
        }
        free(tok);
    }
    return 0;
}
static void put_sym(bwr* w, const ecode* ec, int s) { if (!ec->zero_bits) bw_put(w, ec->codes[s], ec->lens[s]); }
static void ecode_free(ecode* e) { free(e->lens); free(e->codes); e->lens = NULL; e->codes = NULL; }

static void prefix_encode(int v, int* code, int* nextra, int* extra) {   /* v >= 1 */
    int d = v - 1, hb, sb;
    if (d < 4) { *code = d; *nextra = 0; *extra = 0; return; }
    hb = 31; while (!((u32)d >> hb)) hb--;
    sb = (d >> (hb - 1)) & 1;
    *nextra = hb - 1;
    *code = 2 * hb + sb;
    *extra = d & ((1 << *nextra) - 1);
}
static int dist_to_code(int xs, int dist) {
    int c;
    for (c = 1; c <= 120; c++) if (plane_to_dist(xs, c) == dist) return c;
    return dist + 120;
}

/* tokens: literal (argb) or copy(len, dist) */
typedef struct { u32 v; int len; } etok;

static etok* lz77(const u32* px, int xs, int ys, int* ntok) {
    size_t n = (size_t)xs * ys, i = 0;
    etok* t = (etok*)malloc(sizeof(etok) * (n + 1));
    int hbits = 16;
    int* head = (int*)malloc(sizeof(int) << hbits);
    int* chain = (int*)malloc(sizeof(int) * (n + 1));
    int nt = 0;
    if (!t || !head || !chain) { free(t); free(head); free(chain); return NULL; }
    memset(head, 0xff, sizeof(int) << hbits);
#define HASH2(k) ((u32)(((px[k] * 0x9E3779B1u) ^ (px[(k) + 1] * 0x85EBCA6Bu)) >> (32 - hbits)))
    while (i < n) {
        int best = 0, bestd = 0;
        if (i + 1 < n) {
            int cand[3], nc = 0, k, tries = 0;
            u32 h = HASH2(i);
            int p = head[h];
            if (i >= 1) cand[nc++] = 1;
            if (i >= (size_t)xs) cand[nc++] = xs;
            for (k = 0; k < nc; k++) {
                size_t s = i - (size_t)cand[k]; int L = 0;
                while (i + L < n && L < 4096 && px[s + L] == px[i + L]) L++;
                if (L > best) { best = L; bestd = cand[k]; }
            }
            while (p >= 0 && tries < 24) {
                size_t s = (size_t)p; int L = 0;
                if (i - s > 1048576 - 120) break;
                while (i + L < n && L < 4096 && px[s + L] == px[i + L]) L++;
                if (L > best + 1 || (L > best && (int)(i - s) < bestd)) { best = L; bestd = (int)(i - s); }
                p = chain[p]; tries++;
            }
        }
        if (best >= 3) {
            size_t k;
            t[nt].v = (u32)bestd; t[nt].len = best; nt++;
            for (k = 0; k < (size_t)best; k++, i++)
                if (i + 1 < n) { u32 h = HASH2(i); chain[i] = head[h]; head[h] = (int)i; }
        } else {
            t[nt].v = px[i]; t[nt].len = 0; nt++;
            if (i + 1 < n) { u32 h = HASH2(i); chain[i] = head[h]; head[h] = (int)i; }
            i++;
        }
    }
#undef HASH2
    free(head); free(chain);
    *ntok = nt;
    return t;
}

/* Write an entropy-coded image (no colour cache, single prefix-code group). */
static int write_image(bwr* w, const u32* px, int xs, int ys, int level0) {
    int nt = 0, i, rc = -1;
    etok* t = lz77(px, xs, ys, &nt);
    u32 *fg = NULL, fr[256], fb[256], fa[256], fd[40];
    ecode cg, cr, cb, ca, cd;
    memset(&cg, 0, sizeof cg); memset(&cr, 0, sizeof cr); memset(&cb, 0, sizeof cb); memset(&ca, 0, sizeof ca); memset(&cd, 0, sizeof cd);
    if (!t) return -1;
    fg = (u32*)calloc(280, 4);
    if (!fg) { free(t); return -1; }
    memset(fr, 0, sizeof fr); memset(fb, 0, sizeof fb); memset(fa, 0, sizeof fa); memset(fd, 0, sizeof fd);
    for (i = 0; i < nt; i++) {
        if (t[i].len == 0) {
            u32 v = t[i].v;
            fg[(v >> 8) & 0xff]++; fr[(v >> 16) & 0xff]++; fb[v & 0xff]++; fa[v >> 24]++;
        } else {
            int c, ne, ex;
            prefix_encode(t[i].len, &c, &ne, &ex); fg[256 + c]++;
            prefix_encode(dist_to_code(xs, (int)t[i].v), &c, &ne, &ex); fd[c]++;
        }
    }
    bw_put(w, 0, 1);                                      /* no colour cache */
    if (level0) bw_put(w, 0, 1);                          /* no meta prefix codes */
    if (write_code(w, fg, 280, &cg) || write_code(w, fr, 256, &cr) || write_code(w, fb, 256, &cb) ||
        write_code(w, fa, 256, &ca) || write_code(w, fd, 40, &cd)) goto done;
    for (i = 0; i < nt; i++) {
        if (t[i].len == 0) {
            u32 v = t[i].v;
            put_sym(w, &cg, (int)((v >> 8) & 0xff)); put_sym(w, &cr, (int)((v >> 16) & 0xff));
            put_sym(w, &cb, (int)(v & 0xff)); put_sym(w, &ca, (int)(v >> 24));
        } else {
            int c, ne, ex;
            prefix_encode(t[i].len, &c, &ne, &ex); put_sym(w, &cg, 256 + c); bw_put(w, (u32)ex, ne);
            prefix_encode(dist_to_code(xs, (int)t[i].v), &c, &ne, &ex); put_sym(w, &cd, c); bw_put(w, (u32)ex, ne);
        }
    }
    rc = w->err ? -1 : 0;
done:
    ecode_free(&cg); ecode_free(&cr); ecode_free(&cb); ecode_free(&ca); ecode_free(&cd);
    free(fg); free(t);
    return rc;
}

static u32 sub_px(u32 a, u32 b) {
    return (((a | 0x00ff00ffu) - (b & 0xff00ff00u)) & 0xff00ff00u) | (((a | 0xff00ff00u) - (b & 0x00ff00ffu)) & 0x00ff00ffu);
}

/* Encode the VP8L bitstream (header included) for rgba. Returns malloc'd buffer. */
static u8* vp8l_encode(const u8* rgba, int w, int h, size_t* out_n) {
    bwr bw; size_t np = (size_t)w * h, i;
    u32* argb = (u32*)malloc(np * 4);
    u32 pal[256]; int npal = 0, has_alpha = 0;
    memset(&bw, 0, sizeof bw);
    if (!argb) return NULL;
    for (i = 0; i < np; i++) {
        const u8* p = rgba + i * 4;
        argb[i] = ((u32)p[3] << 24) | ((u32)p[0] << 16) | ((u32)p[1] << 8) | p[2];
        if (p[3] != 255) has_alpha = 1;
    }
    for (i = 0; i < np && npal <= 256; i++) {
        int k, f = 0;
        for (k = 0; k < npal; k++) if (pal[k] == argb[i]) { f = 1; break; }
        if (!f) { if (npal == 256) { npal = 257; break; } pal[npal++] = argb[i]; }
    }
    bw_byte(&bw, 0x2f);
    bw_put(&bw, (u32)(w - 1), 14); bw_put(&bw, (u32)(h - 1), 14);
    bw_put(&bw, (u32)has_alpha, 1); bw_put(&bw, 0, 3);
    if (npal <= 256) {
        /* colour-indexing transform with pixel bundling */
        int bits = npal > 16 ? 0 : npal > 4 ? 1 : npal > 2 ? 2 : 3;
        int pw = div_round(w, bits), per = 1 << bits, bpp = 8 >> bits, x, y, k;
        u32 dpal[256];
        u32* packed = (u32*)calloc((size_t)pw * h, 4);
        u8 idxmap_ok = 1;
        if (!packed) { free(argb); free(bw.buf); return NULL; }
        /* sort palette for slightly better compression of the index image */
        for (k = 1; k < npal; k++) { u32 v = pal[k]; int j = k - 1; while (j >= 0 && pal[j] > v) { pal[j + 1] = pal[j]; j--; } pal[j + 1] = v; }
        for (y = 0; y < h; y++)
            for (x = 0; x < w; x++) {
                u32 v = argb[(size_t)y * w + x]; int lo = 0, hi = npal - 1, idx = 0;
                while (lo <= hi) { int mid = (lo + hi) >> 1; if (pal[mid] == v) { idx = mid; break; } if (pal[mid] < v) lo = mid + 1; else hi = mid - 1; }
                packed[(size_t)y * pw + (x >> bits)] |= (u32)idx << (8 + (x & (per - 1)) * bpp);
            }
        for (k = 0; k < pw * h; k++) packed[k] |= 0xff000000u;
        (void)idxmap_ok;
        for (k = 0; k < npal; k++) dpal[k] = k == 0 ? pal[0] : sub_px(pal[k], pal[k - 1]);
        bw_put(&bw, 1, 1); bw_put(&bw, 3, 2); bw_put(&bw, (u32)(npal - 1), 8);
        if (write_image(&bw, dpal, npal, 1, 0) != 0) { free(packed); free(argb); free(bw.buf); return NULL; }
        bw_put(&bw, 0, 1);                                /* no more transforms */
        if (write_image(&bw, packed, pw, h, 1) != 0) { free(packed); free(argb); free(bw.buf); return NULL; }
        free(packed);
    } else {
        /* subtract green, then predictor transform with the best mode per 16x16 block */
        int pb = 4, tw = div_round(w, pb), th = div_round(h, pb), bx, by, x, y;
        u32* modes = (u32*)malloc((size_t)tw * th * 4);
        u32* res = (u32*)malloc(np * 4);
        if (!modes || !res) { free(modes); free(res); free(argb); free(bw.buf); return NULL; }
        for (i = 0; i < np; i++) {
            u32 v = argb[i], g = (v >> 8) & 0xff;
            argb[i] = (v & 0xff00ff00u) | ((((v >> 16) - g) & 0xff) << 16) | (((v & 0xff) - g) & 0xff);
        }
        for (by = 0; by < th; by++)
            for (bx = 0; bx < tw; bx++) {
                int m, best = 0; long bestcost = -1;
                for (m = 0; m < 14; m++) {
                    long cost = 0;
                    for (y = by << pb; y < h && y < (by + 1) << pb; y++)
                        for (x = bx << pb; x < w && x < (bx + 1) << pb; x++) {
                            const u32* p = argb + (size_t)y * w + x; u32 pred, r; int s;
                            if (y == 0) pred = x == 0 ? 0xff000000u : p[-1];
                            else if (x == 0) pred = p[-w];
                            else pred = lpredict(m, p, w);
                            r = sub_px(*p, pred);
                            for (s = 0; s < 32; s += 8) { int c = (int)((r >> s) & 0xff); cost += c < 128 ? c : 256 - c; }
                        }
                    if (bestcost < 0 || cost < bestcost) { bestcost = cost; best = m; }
                }
                modes[by * tw + bx] = 0xff000000u | ((u32)best << 8);
            }
        for (y = 0; y < h; y++)
            for (x = 0; x < w; x++) {
                const u32* p = argb + (size_t)y * w + x; u32 pred;
                if (y == 0) pred = x == 0 ? 0xff000000u : p[-1];
                else if (x == 0) pred = p[-w];
                else pred = lpredict((int)((modes[(y >> pb) * tw + (x >> pb)] >> 8) & 0xf), p, w);
                res[(size_t)y * w + x] = sub_px(*p, pred);
            }
        /* transforms are listed in encoding order: subtract green first */
        bw_put(&bw, 1, 1); bw_put(&bw, 2, 2);
        bw_put(&bw, 1, 1); bw_put(&bw, 0, 2); bw_put(&bw, (u32)(pb - 2), 3);
        if (write_image(&bw, modes, tw, th, 0) != 0) { free(modes); free(res); free(argb); free(bw.buf); return NULL; }
        bw_put(&bw, 0, 1);
        if (write_image(&bw, res, w, h, 1) != 0) { free(modes); free(res); free(argb); free(bw.buf); return NULL; }
        free(modes); free(res);
    }
    free(argb);
    bw_flush(&bw);
    if (bw.err) { free(bw.buf); return NULL; }
    *out_n = bw.len;
    return bw.buf;
}

static void put32le(u8* p, u32 v) { p[0] = (u8)v; p[1] = (u8)(v >> 8); p[2] = (u8)(v >> 16); p[3] = (u8)(v >> 24); }
static void put24le(u8* p, u32 v) { p[0] = (u8)v; p[1] = (u8)(v >> 8); p[2] = (u8)(v >> 16); }

char* trc_webp_encode_lossless(const char* rgba, int64_t w, int64_t h, int64_t* out_len) {
    size_t n = 0, pad, total; u8* bs; u8* out;
    *out_len = 0;
    if (w < 1 || h < 1 || w > 16384 || h > 16384) return NULL;
    bs = vp8l_encode((const u8*)rgba, (int)w, (int)h, &n);
    if (!bs) return NULL;
    pad = n & 1;
    total = 12 + 8 + n + pad;
    out = (u8*)malloc(total);
    if (!out) { free(bs); return NULL; }
    memcpy(out, "RIFF", 4); put32le(out + 4, (u32)(total - 8)); memcpy(out + 8, "WEBP", 4);
    memcpy(out + 12, "VP8L", 4); put32le(out + 16, (u32)n);
    memcpy(out + 20, bs, n);
    if (pad) out[20 + n] = 0;
    free(bs);
    *out_len = (int64_t)total;
    return (char*)out;
}

/* ── animated encoder: every frame is a full-canvas lossless ANMF ── */
typedef struct { int w, h, loop, nframes, any_alpha; u8* buf; size_t len, cap; } wp_anim;
static int anim_put(wp_anim* a, const u8* p, size_t n) {
    if (a->len + n > a->cap) {
        size_t nc = a->cap ? a->cap : 4096; u8* nb;
        while (nc < a->len + n) nc *= 2;
        nb = (u8*)realloc(a->buf, nc);
        if (!nb) return -1;
        a->buf = nb; a->cap = nc;
    }
    memcpy(a->buf + a->len, p, n); a->len += n;
    return 0;
}
void* trc_webp_anim_begin(int64_t w, int64_t h, int64_t loop) {
    wp_anim* a;
    if (w < 1 || h < 1 || w > 16384 || h > 16384) return NULL;
    a = (wp_anim*)calloc(1, sizeof(wp_anim));
    if (!a) return NULL;
    a->w = (int)w; a->h = (int)h; a->loop = (int)(loop < 0 ? 0 : loop > 65535 ? 65535 : loop);
    return a;
}
int64_t trc_webp_anim_add(void* hh, const char* rgba, int64_t duration_ms) {
    wp_anim* a = (wp_anim*)hh; size_t n = 0, i; u8* bs; u8 hdr[24];
    if (!a) return -1;
    bs = vp8l_encode((const u8*)rgba, a->w, a->h, &n);
    if (!bs) return -1;
    for (i = 0; i < (size_t)a->w * a->h; i++) if (((const u8*)rgba)[i * 4 + 3] != 255) { a->any_alpha = 1; break; }
    memcpy(hdr, "ANMF", 4); put32le(hdr + 4, (u32)(16 + 8 + n + (n & 1)));
    put24le(hdr + 8, 0); put24le(hdr + 11, 0);
    put24le(hdr + 14, (u32)(a->w - 1)); put24le(hdr + 17, (u32)(a->h - 1));
    put24le(hdr + 20, (u32)(duration_ms < 0 ? 0 : duration_ms > 0xffffff ? 0xffffff : duration_ms));
    hdr[23] = 0x02;                                       /* no blending, no dispose */
    if (anim_put(a, hdr, 24) != 0) { free(bs); return -1; }
    memcpy(hdr, "VP8L", 4); put32le(hdr + 4, (u32)n);
    if (anim_put(a, hdr, 8) != 0 || anim_put(a, bs, n) != 0) { free(bs); return -1; }
    if (n & 1) { u8 z = 0; if (anim_put(a, &z, 1) != 0) { free(bs); return -1; } }
    free(bs);
    a->nframes++;
    return 0;
}
char* trc_webp_anim_finish(void* hh, int64_t* out_len) {
    wp_anim* a = (wp_anim*)hh; u8* out; size_t total; u8 hd[12 + 18 + 14];
    *out_len = 0;
    if (!a) return NULL;
    if (a->nframes == 0) { free(a->buf); free(a); return NULL; }
    total = 12 + 18 + 14 + a->len;
    out = (u8*)malloc(total);
    if (!out) { free(a->buf); free(a); return NULL; }
    memcpy(hd, "RIFF", 4); put32le(hd + 4, (u32)(total - 8)); memcpy(hd + 8, "WEBP", 4);
    memcpy(hd + 12, "VP8X", 4); put32le(hd + 16, 10);
    hd[20] = (u8)(0x02 | (a->any_alpha ? 0x10 : 0)); hd[21] = hd[22] = hd[23] = 0;
    put24le(hd + 24, (u32)(a->w - 1)); put24le(hd + 27, (u32)(a->h - 1));
    memcpy(hd + 30, "ANIM", 4); put32le(hd + 34, 6);
    put32le(hd + 38, 0);                                  /* background colour */
    hd[42] = (u8)a->loop; hd[43] = (u8)(a->loop >> 8);
    memcpy(out, hd, sizeof hd);
    memcpy(out + sizeof hd, a->buf, a->len);
    free(a->buf); free(a);
    *out_len = (int64_t)total;
    return (char*)out;
}
