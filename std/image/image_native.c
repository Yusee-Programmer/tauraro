/*
 * std/image/image_native.c -- JPEG and GIF codecs, resampling and filters
 * for std.image.{jpeg,gif,ops}.
 *
 * Linked through `# tauraro-cpp-shim: image_native.c` in std/image/native.tr.
 * All pixel buffers are RGBA8, row-major, top row first, no padding (the
 * layout of std.image's Image.pixels).
 *
 *   JPEG decode .... baseline and progressive, Huffman, any sampling factors,
 *                    restart markers, grayscale / YCbCr / RGB / CMYK / YCCK,
 *                    EXIF orientation reported (not applied)
 *   JPEG encode .... baseline, quality 1-100, 4:2:0 or 4:4:4, standard tables
 *   GIF decode ..... all frames composited onto the canvas (disposal 0-3,
 *                    transparency, interlacing), delays, loop count
 *   GIF encode ..... median-cut palette (or exact when <= 256 colours),
 *                    optional Floyd-Steinberg dithering, animation
 *   resize ......... nearest, box, bilinear, bicubic (Catmull-Rom), Lanczos-3,
 *                    alpha-correct (premultiplied) separable resampling
 *   filters ........ colour ops, convolution, Gaussian/box blur, unsharp mask,
 *                    Sobel edges, flips, rotations, crop
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

static u8 clamp8(int v) { return (u8)(v < 0 ? 0 : v > 255 ? 255 : v); }
static u8 clampf8(double v) { int i = (int)(v + 0.5); if (v < 0) i = (int)(v - 0.5); return clamp8(i); }

/* ═══════════════════════════ decoded result ══════════════════════════════ */
typedef struct {
    int w, h, nframes, loop, orientation;
    u8** frames;            /* nframes RGBA buffers of w*h*4 */
    int* delays;            /* milliseconds */
    char err[160];
} img_result;

static img_result* res_new(void) { return (img_result*)calloc(1, sizeof(img_result)); }
static int res_add_frame(img_result* r, u8* rgba, int delay) {
    u8** nf = (u8**)realloc(r->frames, sizeof(u8*) * (size_t)(r->nframes + 1));
    int* nd;
    if (!nf) return -1;
    r->frames = nf;
    nd = (int*)realloc(r->delays, sizeof(int) * (size_t)(r->nframes + 1));
    if (!nd) return -1;
    r->delays = nd;
    r->frames[r->nframes] = rgba; r->delays[r->nframes] = delay; r->nframes++;
    return 0;
}
void trc_img_free(void* h) {
    img_result* r = (img_result*)h; int i;
    if (!r) return;
    for (i = 0; i < r->nframes; i++) free(r->frames[i]);
    free(r->frames); free(r->delays); free(r);
}
int64_t trc_img_ok(void* h) { img_result* r = (img_result*)h; return r && r->nframes > 0; }
int64_t trc_img_w(void* h) { return h ? ((img_result*)h)->w : 0; }
int64_t trc_img_h(void* h) { return h ? ((img_result*)h)->h : 0; }
int64_t trc_img_nframes(void* h) { return h ? ((img_result*)h)->nframes : 0; }
int64_t trc_img_delay(void* h, int64_t i) { img_result* r = (img_result*)h; return (r && i >= 0 && i < r->nframes) ? r->delays[i] : 0; }
int64_t trc_img_loop(void* h) { return h ? ((img_result*)h)->loop : 0; }
int64_t trc_img_orientation(void* h) { return h ? ((img_result*)h)->orientation : 1; }
int64_t trc_img_error(void* h, char* out, int64_t cap) {
    img_result* r = (img_result*)h; size_t n;
    if (!r) { memcpy(out, "out of memory", 13); return 13; }
    n = strlen(r->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, r->err, n); return (int64_t)n;
}
void trc_img_frame_copy(void* h, int64_t i, char* out) {
    img_result* r = (img_result*)h;
    if (r && i >= 0 && i < r->nframes) memcpy(out, r->frames[i], (size_t)r->w * (size_t)r->h * 4);
}
/* Free a buffer returned by an encoder. */
void trc_img_buf_free(char* p) { free(p); }

/* ═══════════════════════════ JPEG decoder ════════════════════════════════ */
static const u8 jzz[64] = {
    0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,
    35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63 };

typedef struct { u8 bits[17]; u8 vals[256]; int maxcode[18]; int valptr[17]; int mincode[17]; int present; } jhuff;
typedef struct {
    int id, h, v, tq, td, ta;
    int bw, bh;              /* blocks per row/column (padded to the MCU grid) */
    int cw, ch;              /* component size in samples */
    int16_t* coef;           /* bw*bh*64, natural order */
    int dc_pred;
    u8* plane;               /* bw*8 x bh*8 samples after IDCT */
} jcomp;
typedef struct {
    const u8* p; size_t n, pos;
    u32 acc; int nbits; int marker_hit;
    u16 qt[4][64];           /* natural order */
    jhuff hdc[4], hac[4];
    jcomp c[4]; int nc;
    int w, h, hmax, vmax, mcux, mcuy, progressive, restart;
    int adobe, adobe_transform, jfif;
    int eobrun;
    int orientation;
    char* err;
} jdec;

static int jfail(jdec* j, const char* m) { snprintf(j->err, 160, "JPEG: %s", m); return -1; }

static void jfill(jdec* j) {
    while (j->nbits <= 24) {
        u32 b = 0;
        if (!j->marker_hit && j->pos < j->n) {
            b = j->p[j->pos++];
            if (b == 0xFF) {
                u8 nx = j->pos < j->n ? j->p[j->pos] : 0;
                if (nx == 0x00) j->pos++;
                else { j->marker_hit = 1; j->pos--; b = 0; }
            }
        }
        j->acc |= b << (24 - j->nbits);
        j->nbits += 8;
    }
}
static int jbits(jdec* j, int n) {
    u32 v;
    if (n == 0) return 0;
    if (j->nbits < n) jfill(j);
    v = j->acc >> (32 - n);
    j->acc <<= n; j->nbits -= n;
    return (int)v;
}
static int jbit(jdec* j) { return jbits(j, 1); }
static int jextend(int v, int s) { return v < (1 << (s - 1)) ? v - (1 << s) + 1 : v; }
static int jdecode(jdec* j, const jhuff* t) {
    int code = 0, l;
    for (l = 1; l <= 16; l++) {
        code = (code << 1) | jbit(j);
        if (code <= t->maxcode[l]) return t->vals[t->valptr[l] + code - t->mincode[l]];
    }
    return -1;
}
static void jbuild(jhuff* t) {
    int code = 0, k = 0, l;
    for (l = 1; l <= 16; l++) {
        t->valptr[l] = k; t->mincode[l] = code;
        code += t->bits[l]; k += t->bits[l];
        t->maxcode[l] = t->bits[l] ? code - 1 : -1;
        code <<= 1;
    }
    t->maxcode[17] = 0x7fffffff;
    t->present = 1;
}
static void jreset_bits(jdec* j) { j->acc = 0; j->nbits = 0; j->marker_hit = 0; }

static int j16(const u8* p) { return (p[0] << 8) | p[1]; }

/* Decode one block in a sequential (baseline/extended) scan. */
static int jblock_seq(jdec* j, jcomp* c, int16_t* blk) {
    int t = jdecode(j, &j->hdc[c->td]), k, diff;
    if (t < 0 || t > 16) return jfail(j, "bad Huffman code");
    diff = t ? jextend(jbits(j, t), t) : 0;
    c->dc_pred += diff;
    blk[0] = (int16_t)c->dc_pred;
    for (k = 1; k < 64; ) {
        int rs = jdecode(j, &j->hac[c->ta]), r, s;
        if (rs < 0) return jfail(j, "bad Huffman code");
        r = rs >> 4; s = rs & 15;
        if (s == 0) { if (rs != 0xF0) break; k += 16; continue; }
        k += r;
        if (k > 63) return jfail(j, "coefficient index out of range");
        blk[jzz[k++]] = (int16_t)jextend(jbits(j, s), s);
    }
    return 0;
}
static int jblock_dc_first(jdec* j, jcomp* c, int16_t* blk, int al) {
    int t = jdecode(j, &j->hdc[c->td]), diff;
    if (t < 0 || t > 16) return jfail(j, "bad Huffman code");
    diff = t ? jextend(jbits(j, t), t) : 0;
    c->dc_pred += diff;
    blk[0] = (int16_t)(c->dc_pred * (1 << al));
    return 0;
}
static int jblock_ac_first(jdec* j, jcomp* c, int16_t* blk, int ss, int se, int al) {
    int k = ss;
    if (j->eobrun) { j->eobrun--; return 0; }
    while (k <= se) {
        int rs = jdecode(j, &j->hac[c->ta]), r, s;
        if (rs < 0) return jfail(j, "bad Huffman code");
        r = rs >> 4; s = rs & 15;
        if (s == 0) {
            if (r < 15) {
                j->eobrun = (1 << r);
                if (r) j->eobrun += jbits(j, r);
                j->eobrun--;
                break;
            }
            k += 16;
        } else {
            k += r;
            if (k > 63) return jfail(j, "coefficient index out of range");
            blk[jzz[k++]] = (int16_t)(jextend(jbits(j, s), s) * (1 << al));
        }
    }
    return 0;
}
static int jblock_ac_refine(jdec* j, jcomp* c, int16_t* blk, int ss, int se, int al) {
    int bit = 1 << al, k;
    if (j->eobrun) {
        j->eobrun--;
        for (k = ss; k <= se; k++) {
            int16_t* p = &blk[jzz[k]];
            if (*p != 0 && jbit(j) && (*p & bit) == 0) { if (*p > 0) *p = (int16_t)(*p + bit); else *p = (int16_t)(*p - bit); }
        }
        return 0;
    }
    k = ss;
    do {
        int rs = jdecode(j, &j->hac[c->ta]), r, s;
        if (rs < 0) return jfail(j, "bad Huffman code");
        s = rs & 15; r = rs >> 4;
        if (s == 0) {
            if (r < 15) {
                j->eobrun = (1 << r) - 1;
                if (r) j->eobrun += jbits(j, r);
                r = 64;
            }
        } else {
            if (s != 1) return jfail(j, "bad refinement code");
            s = jbit(j) ? bit : -bit;
        }
        while (k <= se) {
            int16_t* p = &blk[jzz[k++]];
            if (*p != 0) {
                if (jbit(j) && (*p & bit) == 0) { if (*p > 0) *p = (int16_t)(*p + bit); else *p = (int16_t)(*p - bit); }
            } else {
                if (r == 0) { *p = (int16_t)s; break; }
                r--;
            }
        }
    } while (k <= se);
    return 0;
}

/* After a restart interval: skip to the RSTn marker and reset predictors. */
static void jrestart(jdec* j) {
    int i;
    /* find the marker */
    while (j->pos + 1 < j->n && !(j->p[j->pos] == 0xFF && j->p[j->pos + 1] >= 0xD0 && j->p[j->pos + 1] <= 0xD7)) j->pos++;
    if (j->pos + 1 < j->n) j->pos += 2;
    jreset_bits(j);
    for (i = 0; i < j->nc; i++) j->c[i].dc_pred = 0;
    j->eobrun = 0;
}

static int jscan(jdec* j, const u8* sos, int len) {
    int ns = sos[0], i, ss, se, ah, al, idx[4];
    if (len < 1 + 2 * ns + 3 || ns < 1 || ns > 4) return jfail(j, "bad SOS");
    for (i = 0; i < ns; i++) {
        int cid = sos[1 + 2 * i], tbl = sos[2 + 2 * i], k, found = -1;
        for (k = 0; k < j->nc; k++) if (j->c[k].id == cid) found = k;
        if (found < 0) return jfail(j, "scan references an unknown component");
        idx[i] = found;
        j->c[found].td = tbl >> 4; j->c[found].ta = tbl & 15;
        if (j->c[found].td > 3 || j->c[found].ta > 3) return jfail(j, "bad table selector");
    }
    ss = sos[1 + 2 * ns]; se = sos[2 + 2 * ns]; ah = sos[3 + 2 * ns] >> 4; al = sos[3 + 2 * ns] & 15;
    if (!j->progressive) { ss = 0; se = 63; ah = al = 0; }
    if (ss > se || se > 63) return jfail(j, "bad spectral selection");
    for (i = 0; i < ns; i++) {
        jcomp* c = &j->c[idx[i]];
        if ((ss == 0 && !j->hdc[c->td].present && ah == 0) || (se > 0 && !j->hac[c->ta].present)) return jfail(j, "missing Huffman table");
        c->dc_pred = 0;
    }
    jreset_bits(j); j->eobrun = 0;
    {
        int mcu = 0, total;
        if (ns == 1) {
            /* non-interleaved: the component's own block grid */
            jcomp* c = &j->c[idx[0]];
            int bx = (c->cw + 7) / 8, by = (c->ch + 7) / 8, x, y;
            total = bx * by;
            for (y = 0; y < by; y++)
                for (x = 0; x < bx; x++) {
                    int16_t* blk = c->coef + ((size_t)y * c->bw + x) * 64; int r;
                    if (!j->progressive) r = jblock_seq(j, c, blk);
                    else if (ss == 0) r = ah == 0 ? jblock_dc_first(j, c, blk, al) : (jbit(j) ? (blk[0] = (int16_t)(blk[0] | (1 << al)), 0) : 0);
                    else r = ah == 0 ? jblock_ac_first(j, c, blk, ss, se, al) : jblock_ac_refine(j, c, blk, ss, se, al);
                    if (r < 0) return -1;
                    mcu++;
                    if (j->restart && mcu % j->restart == 0 && mcu < total) jrestart(j);
                }
        } else {
            int mx, my;
            total = j->mcux * j->mcuy;
            for (my = 0; my < j->mcuy; my++)
                for (mx = 0; mx < j->mcux; mx++) {
                    for (i = 0; i < ns; i++) {
                        jcomp* c = &j->c[idx[i]]; int bx, by;
                        for (by = 0; by < c->v; by++)
                            for (bx = 0; bx < c->h; bx++) {
                                int16_t* blk = c->coef + ((size_t)(my * c->v + by) * c->bw + (size_t)(mx * c->h + bx)) * 64; int r;
                                if (!j->progressive) r = jblock_seq(j, c, blk);
                                else if (ah == 0) r = jblock_dc_first(j, c, blk, al);
                                else { if (jbit(j)) blk[0] = (int16_t)(blk[0] | (1 << al)); r = 0; }
                                if (r < 0) return -1;
                            }
                    }
                    mcu++;
                    if (j->restart && mcu % j->restart == 0 && mcu < total) jrestart(j);
                }
        }
    }
    /* continue after the entropy-coded data: find the next marker */
    while (j->pos + 1 < j->n && !(j->p[j->pos] == 0xFF && j->p[j->pos + 1] != 0 && !(j->p[j->pos + 1] >= 0xD0 && j->p[j->pos + 1] <= 0xD7))) j->pos++;
    return 0;
}

/* Float IDCT (separable, exact cosines). */
static float jcos[8][8];
static int jcos_ready = 0;
static void jinit_cos(void) {
    int x, u;
    if (jcos_ready) return;
    for (x = 0; x < 8; x++)
        for (u = 0; u < 8; u++)
            jcos[x][u] = (float)((u == 0 ? sqrt(0.125) : 0.5) * cos((2 * x + 1) * u * 3.14159265358979323846 / 16.0));
    jcos_ready = 1;
}
static void jidct(const int16_t* in, const u16* q, u8* out, int stride) {
    float t[64], r[64]; int x, y, u;
    for (u = 0; u < 64; u++) t[u] = (float)in[u] * (float)q[u];
    for (y = 0; y < 8; y++)          /* rows: over v (vertical freq) */
        for (x = 0; x < 8; x++) {
            float s = 0;
            for (u = 0; u < 8; u++) s += jcos[x][u] * t[y * 8 + u];
            r[y * 8 + x] = s;
        }
    for (x = 0; x < 8; x++)
        for (y = 0; y < 8; y++) {
            float s = 0;
            for (u = 0; u < 8; u++) s += jcos[y][u] * r[u * 8 + x];
            out[y * stride + x] = clampf8(s + 128.0f);
        }
}

static void jparse_exif(jdec* j, const u8* p, int len) {
    int le, off, n, i;
    if (len < 14 || memcmp(p, "Exif\0\0", 6) != 0) return;
    p += 6; len -= 6;
    if (p[0] == 'I' && p[1] == 'I') le = 1; else if (p[0] == 'M' && p[1] == 'M') le = 0; else return;
#define E16(q) (le ? ((q)[0] | ((q)[1] << 8)) : (((q)[0] << 8) | (q)[1]))
#define E32(q) (le ? ((q)[0] | ((q)[1] << 8) | ((q)[2] << 16) | ((u32)(q)[3] << 24)) : (((u32)(q)[0] << 24) | ((q)[1] << 16) | ((q)[2] << 8) | (q)[3]))
    off = (int)E32(p + 4);
    if (off < 8 || off + 2 > len) return;
    n = E16(p + off);
    for (i = 0; i < n; i++) {
        const u8* e = p + off + 2 + 12 * i;
        if (e + 12 > p + len) return;
        if (E16(e) == 0x0112) { int v = E16(e + 8); if (v >= 1 && v <= 8) j->orientation = v; return; }
    }
#undef E16
#undef E32
}

static int jdecode_all(jdec* j, img_result* res) {
    size_t pos = 2; int i, x, y, frame_done = 0;
    if (j->n < 4 || j->p[0] != 0xFF || j->p[1] != 0xD8) return jfail(j, "not a JPEG file");
    jinit_cos();
    j->orientation = 1;
    while (pos + 4 <= j->n) {
        int m, len;
        if (j->p[pos] != 0xFF) { pos++; continue; }
        m = j->p[pos + 1];
        if (m == 0xFF) { pos++; continue; }
        if (m == 0xD8 || (m >= 0xD0 && m <= 0xD7) || m == 0x01) { pos += 2; continue; }
        if (m == 0xD9) break;
        len = j16(j->p + pos + 2);
        if (len < 2 || pos + 2 + (size_t)len > j->n) return jfail(j, "truncated segment");
        {
            const u8* s = j->p + pos + 4; int sl = len - 2;
            switch (m) {
            case 0xDB: {   /* DQT */
                int k = 0;
                while (k < sl) {
                    int pq = s[k] >> 4, tq = s[k] & 15, q;
                    if (tq > 3) return jfail(j, "bad quantization table id");
                    k++;
                    for (q = 0; q < 64; q++) {
                        if (pq) { j->qt[tq][jzz[q]] = (u16)j16(s + k); k += 2; }
                        else j->qt[tq][jzz[q]] = s[k++];
                        if (k > sl) return jfail(j, "truncated DQT");
                    }
                }
                break;
            }
            case 0xC4: {   /* DHT */
                int k = 0;
                while (k < sl) {
                    int tc = s[k] >> 4, th = s[k] & 15, total = 0, q; jhuff* t;
                    if (th > 3 || tc > 1) return jfail(j, "bad Huffman table id");
                    t = tc ? &j->hac[th] : &j->hdc[th];
                    k++;
                    if (k + 16 > sl) return jfail(j, "truncated DHT");
                    t->bits[0] = 0;
                    for (q = 1; q <= 16; q++) { t->bits[q] = s[k + q - 1]; total += t->bits[q]; }
                    k += 16;
                    if (total > 256 || k + total > sl) return jfail(j, "bad DHT");
                    memcpy(t->vals, s + k, (size_t)total); k += total;
                    jbuild(t);
                }
                break;
            }
            case 0xC0: case 0xC1: case 0xC2: {   /* SOF0/1/2 */
                if (sl < 6) return jfail(j, "bad SOF");
                if (s[0] != 8) return jfail(j, "only 8-bit JPEGs are supported");
                j->progressive = m == 0xC2;
                j->h = j16(s + 1); j->w = j16(s + 3); j->nc = s[5];
                if (j->w <= 0 || j->h <= 0) return jfail(j, "image has zero size (DNL not supported)");
                if ((int64_t)j->w * j->h > 100000000) return jfail(j, "image too large");
                if (j->nc != 1 && j->nc != 3 && j->nc != 4) return jfail(j, "unsupported component count");
                if (sl < 6 + 3 * j->nc) return jfail(j, "bad SOF");
                j->hmax = j->vmax = 1;
                for (i = 0; i < j->nc; i++) {
                    jcomp* c = &j->c[i];
                    c->id = s[6 + 3 * i]; c->h = s[7 + 3 * i] >> 4; c->v = s[7 + 3 * i] & 15; c->tq = s[8 + 3 * i];
                    if (c->h < 1 || c->h > 4 || c->v < 1 || c->v > 4 || c->tq > 3) return jfail(j, "bad sampling factors");
                    if (c->h > j->hmax) j->hmax = c->h;
                    if (c->v > j->vmax) j->vmax = c->v;
                }
                j->mcux = (j->w + 8 * j->hmax - 1) / (8 * j->hmax);
                j->mcuy = (j->h + 8 * j->vmax - 1) / (8 * j->vmax);
                for (i = 0; i < j->nc; i++) {
                    jcomp* c = &j->c[i];
                    c->bw = j->mcux * c->h; c->bh = j->mcuy * c->v;
                    c->cw = (j->w * c->h + j->hmax - 1) / j->hmax;
                    c->ch = (j->h * c->v + j->vmax - 1) / j->vmax;
                    c->coef = (int16_t*)calloc((size_t)c->bw * c->bh * 64, sizeof(int16_t));
                    if (!c->coef) return jfail(j, "out of memory");
                }
                frame_done = 1;
                break;
            }
            case 0xC3: case 0xC5: case 0xC6: case 0xC7: case 0xC9: case 0xCA: case 0xCB: case 0xCD: case 0xCE: case 0xCF:
                return jfail(j, "lossless / hierarchical / arithmetic-coded JPEG is not supported");
            case 0xDD: if (sl >= 2) j->restart = j16(s); break;
            case 0xEE: if (sl >= 12 && memcmp(s, "Adobe", 5) == 0) { j->adobe = 1; j->adobe_transform = s[11]; } break;
            case 0xE0: if (sl >= 5 && memcmp(s, "JFIF", 5) == 0) j->jfif = 1; break;
            case 0xE1: jparse_exif(j, s, sl); break;
            case 0xDA: {   /* SOS */
                if (!frame_done) return jfail(j, "scan before frame header");
                j->pos = pos + 4 + (size_t)sl;
                if (jscan(j, s, sl) < 0) return -1;
                pos = j->pos;
                continue;
            }
            default: break;
            }
        }
        pos += 2 + (size_t)len;
    }
    if (!frame_done) return jfail(j, "no frame header");
    /* dequantize + IDCT into planes */
    for (i = 0; i < j->nc; i++) {
        jcomp* c = &j->c[i]; int bx, by, stride = c->bw * 8;
        c->plane = (u8*)malloc((size_t)stride * (size_t)c->bh * 8);
        if (!c->plane) return jfail(j, "out of memory");
        for (by = 0; by < c->bh; by++)
            for (bx = 0; bx < c->bw; bx++)
                jidct(c->coef + ((size_t)by * c->bw + bx) * 64, j->qt[c->tq], c->plane + (size_t)by * 8 * stride + (size_t)bx * 8, stride);
    }
    /* upsample + colour convert */
    {
        u8* out = (u8*)malloc((size_t)j->w * (size_t)j->h * 4);
        int rgb_ids = j->nc == 3 && j->c[0].id == 'R' && j->c[1].id == 'G' && j->c[2].id == 'B';
        int ycc = j->nc == 3 && !rgb_ids && !(j->adobe && j->adobe_transform == 0);
        if (!out) return jfail(j, "out of memory");
        for (y = 0; y < j->h; y++)
            for (x = 0; x < j->w; x++) {
                float v[4]; int k;
                for (k = 0; k < j->nc; k++) {
                    jcomp* c = &j->c[k]; int stride = c->bw * 8;
                    if (c->h == j->hmax && c->v == j->vmax) v[k] = c->plane[(size_t)y * stride + x];
                    else {
                        /* bilinear, sample centres aligned like libjpeg's fancy upsampling */
                        float fx = (x + 0.5f) * c->h / j->hmax - 0.5f, fy = (y + 0.5f) * c->v / j->vmax - 0.5f;
                        int x0 = (int)floorf(fx), y0 = (int)floorf(fy), x1, y1; float ax, ay;
                        ax = fx - x0; ay = fy - y0;
                        x1 = x0 + 1; y1 = y0 + 1;
                        if (x0 < 0) x0 = 0;
                        if (y0 < 0) y0 = 0;
                        if (x1 > c->cw - 1) x1 = c->cw - 1;
                        if (y1 > c->ch - 1) y1 = c->ch - 1;
                        if (x0 > c->cw - 1) x0 = c->cw - 1;
                        if (y0 > c->ch - 1) y0 = c->ch - 1;
                        v[k] = (c->plane[(size_t)y0 * stride + x0] * (1 - ax) + c->plane[(size_t)y0 * stride + x1] * ax) * (1 - ay)
                             + (c->plane[(size_t)y1 * stride + x0] * (1 - ax) + c->plane[(size_t)y1 * stride + x1] * ax) * ay;
                    }
                }
                {
                    u8* o = out + ((size_t)y * j->w + x) * 4; float r, g, b;
                    if (j->nc == 1) { o[0] = o[1] = o[2] = clampf8(v[0]); }
                    else if (j->nc == 3) {
                        if (ycc) {
                            r = v[0] + 1.402f * (v[2] - 128); g = v[0] - 0.344136f * (v[1] - 128) - 0.714136f * (v[2] - 128); b = v[0] + 1.772f * (v[1] - 128);
                        } else { r = v[0]; g = v[1]; b = v[2]; }
                        o[0] = clampf8(r); o[1] = clampf8(g); o[2] = clampf8(b);
                    } else {
                        float cc = v[0], mm = v[1], yy = v[2], kk = v[3];
                        if (j->adobe_transform == 2) {
                            r = v[0] + 1.402f * (v[2] - 128); g = v[0] - 0.344136f * (v[1] - 128) - 0.714136f * (v[2] - 128); b = v[0] + 1.772f * (v[1] - 128);
                            cc = 255 - r; mm = 255 - g; yy = 255 - b;
                            cc = 255 - cc; mm = 255 - mm; yy = 255 - yy;   /* back to inverted CMY like Adobe stores */
                        }
                        o[0] = clampf8(cc * kk / 255.0f); o[1] = clampf8(mm * kk / 255.0f); o[2] = clampf8(yy * kk / 255.0f);
                    }
                    o[3] = 255;
                }
            }
        res->w = j->w; res->h = j->h; res->orientation = j->orientation;
        if (res_add_frame(res, out, 0) < 0) { free(out); return jfail(j, "out of memory"); }
    }
    return 0;
}

void* trc_jpeg_decode(const char* data, int64_t n) {
    img_result* r = res_new(); jdec* j; int i;
    if (!r) return NULL;
    j = (jdec*)calloc(1, sizeof(jdec));
    if (!j) { snprintf(r->err, sizeof r->err, "JPEG: out of memory"); return r; }
    j->p = (const u8*)data; j->n = (size_t)n; j->err = r->err;
    jdecode_all(j, r);
    for (i = 0; i < 4; i++) { free(j->c[i].coef); free(j->c[i].plane); }
    free(j);
    return r;
}

/* ═══════════════════════════ JPEG encoder ════════════════════════════════ */
static const u8 std_lq[64] = {
    16,11,10,16,24,40,51,61, 12,12,14,19,26,58,60,55, 14,13,16,24,40,57,69,56, 14,17,22,29,51,87,80,62,
    18,22,37,56,68,109,103,77, 24,35,55,64,81,104,113,92, 49,64,78,87,103,121,120,101, 72,92,95,98,112,100,103,99 };
static const u8 std_cq[64] = {
    17,18,24,47,99,99,99,99, 18,21,26,66,99,99,99,99, 24,26,56,99,99,99,99,99, 47,66,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99, 99,99,99,99,99,99,99,99, 99,99,99,99,99,99,99,99, 99,99,99,99,99,99,99,99 };
static const u8 dc_l_bits[17] = {0,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0};
static const u8 dc_c_bits[17] = {0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0};
static const u8 dc_vals[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
static const u8 ac_l_bits[17] = {0,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d};
static const u8 ac_l_vals[162] = {
    0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,
    0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
    0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
    0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
    0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,
    0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
    0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa };
static const u8 ac_c_bits[17] = {0,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77};
static const u8 ac_c_vals[162] = {
    0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
    0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
    0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
    0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
    0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,
    0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
    0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa };

typedef struct { u16 code[256]; u8 size[256]; } ehuff;
typedef struct { u8* buf; size_t n, cap; u32 acc; int nbits; int fail; } ebuf;

static void eb_put(ebuf* b, int c) {
    if (b->fail) return;
    if (b->n == b->cap) {
        size_t nc = b->cap ? b->cap * 2 : 65536; u8* p = (u8*)realloc(b->buf, nc);
        if (!p) { b->fail = 1; return; }
        b->buf = p; b->cap = nc;
    }
    b->buf[b->n++] = (u8)c;
}
static void eb_16(ebuf* b, int v) { eb_put(b, v >> 8); eb_put(b, v & 255); }
static void eb_bits(ebuf* b, u32 code, int size) {
    b->acc = (b->acc << size) | (code & ((1u << size) - 1)); b->nbits += size;
    while (b->nbits >= 8) {
        int c = (int)((b->acc >> (b->nbits - 8)) & 255);
        eb_put(b, c);
        if (c == 0xFF) eb_put(b, 0);
        b->nbits -= 8;
    }
}
static void eb_flush(ebuf* b) { if (b->nbits > 0) eb_bits(b, 0x7F, 7 - ((b->nbits - 1) % 8)); b->acc = 0; b->nbits = 0; }
static void ebuild(ehuff* h, const u8* bits, const u8* vals) {
    int code = 0, k = 0, l, i;
    for (l = 1; l <= 16; l++) {
        for (i = 0; i < bits[l]; i++) { h->code[vals[k]] = (u16)code; h->size[vals[k]] = (u8)l; code++; k++; }
        code <<= 1;
    }
}
static int ebitlen(int v) { int n = 0; if (v < 0) v = -v; while (v) { n++; v >>= 1; } return n; }

static float ecos[8][8]; static int ecos_ready = 0;
static void eencode_block(ebuf* b, const float* px, const u16* q, int* dcp, const ehuff* dc, const ehuff* ac) {
    float t[64], coef[64]; int x, y, u, v, z[64], k, run, diff, s;
    if (!ecos_ready) { for (x = 0; x < 8; x++) for (u = 0; u < 8; u++) ecos[x][u] = (float)((u == 0 ? sqrt(0.125) : 0.5) * cos((2 * x + 1) * u * 3.14159265358979323846 / 16.0)); ecos_ready = 1; }
    for (y = 0; y < 8; y++)
        for (u = 0; u < 8; u++) { float sum = 0; for (x = 0; x < 8; x++) sum += px[y * 8 + x] * ecos[x][u]; t[y * 8 + u] = sum; }
    for (u = 0; u < 8; u++)
        for (v = 0; v < 8; v++) { float sum = 0; for (y = 0; y < 8; y++) sum += t[y * 8 + u] * ecos[y][v]; coef[v * 8 + u] = sum; }
    for (k = 0; k < 64; k++) { float qv = coef[jzz[k]] / (float)q[jzz[k]]; z[k] = (int)(qv < 0 ? qv - 0.5f : qv + 0.5f); }
    diff = z[0] - *dcp; *dcp = z[0];
    s = ebitlen(diff);
    eb_bits(b, dc->code[s], dc->size[s]);
    if (s) eb_bits(b, (u32)(diff < 0 ? diff - 1 : diff), s);
    run = 0;
    for (k = 1; k < 64; k++) {
        if (z[k] == 0) { run++; continue; }
        while (run > 15) { eb_bits(b, ac->code[0xF0], ac->size[0xF0]); run -= 16; }
        s = ebitlen(z[k]);
        eb_bits(b, ac->code[(run << 4) | s], ac->size[(run << 4) | s]);
        eb_bits(b, (u32)(z[k] < 0 ? z[k] - 1 : z[k]), s);
        run = 0;
    }
    if (run) eb_bits(b, ac->code[0], ac->size[0]);
}

/* quality 1..100; subsample: 1 = 4:2:0, 0 = 4:4:4; gray: write one channel. */
char* trc_jpeg_encode(const char* rgba, int64_t w, int64_t h, int64_t quality, int64_t subsample, int64_t gray, int64_t* out_len) {
    ebuf b; u16 lq[64], cq[64]; ehuff hdl, hal, hdc, hac; int i, scale, nc = gray ? 1 : 3, hs = (!gray && subsample) ? 2 : 1;
    int mcuw = 8 * hs, mcuh = 8 * hs, mx, my, dcy = 0, dccb = 0, dccr = 0;
    const u8* px = (const u8*)rgba;
    memset(&b, 0, sizeof b);
    if (w <= 0 || h <= 0 || w > 65535 || h > 65535) return NULL;
    if (quality < 1) quality = 1;
    if (quality > 100) quality = 100;
    scale = quality < 50 ? 5000 / (int)quality : 200 - 2 * (int)quality;
    for (i = 0; i < 64; i++) {
        int a = (std_lq[i] * scale + 50) / 100, c = (std_cq[i] * scale + 50) / 100;
        lq[i] = (u16)(a < 1 ? 1 : a > 255 ? 255 : a); cq[i] = (u16)(c < 1 ? 1 : c > 255 ? 255 : c);
    }
    ebuild(&hdl, dc_l_bits, dc_vals); ebuild(&hal, ac_l_bits, ac_l_vals);
    ebuild(&hdc, dc_c_bits, dc_vals); ebuild(&hac, ac_c_bits, ac_c_vals);
    /* SOI, APP0 JFIF */
    eb_16(&b, 0xFFD8);
    eb_16(&b, 0xFFE0); eb_16(&b, 16); eb_put(&b, 'J'); eb_put(&b, 'F'); eb_put(&b, 'I'); eb_put(&b, 'F'); eb_put(&b, 0);
    eb_put(&b, 1); eb_put(&b, 1); eb_put(&b, 0); eb_16(&b, 1); eb_16(&b, 1); eb_put(&b, 0); eb_put(&b, 0);
    /* DQT (zigzag order) */
    eb_16(&b, 0xFFDB); eb_16(&b, gray ? 67 : 132);
    eb_put(&b, 0); for (i = 0; i < 64; i++) eb_put(&b, lq[jzz[i]]);
    if (!gray) { eb_put(&b, 1); for (i = 0; i < 64; i++) eb_put(&b, cq[jzz[i]]); }
    /* SOF0 */
    eb_16(&b, 0xFFC0); eb_16(&b, 8 + 3 * nc); eb_put(&b, 8); eb_16(&b, (int)h); eb_16(&b, (int)w); eb_put(&b, nc);
    eb_put(&b, 1); eb_put(&b, (hs << 4) | hs); eb_put(&b, 0);
    if (!gray) { eb_put(&b, 2); eb_put(&b, 0x11); eb_put(&b, 1); eb_put(&b, 3); eb_put(&b, 0x11); eb_put(&b, 1); }
    /* DHT */
    {
        const u8* bl[4] = {dc_l_bits, ac_l_bits, dc_c_bits, ac_c_bits};
        const u8* vl[4] = {dc_vals, ac_l_vals, dc_vals, ac_c_vals};
        int ids[4] = {0x00, 0x10, 0x01, 0x11}, t;
        for (t = 0; t < (gray ? 2 : 4); t++) {
            int total = 0, k;
            for (k = 1; k <= 16; k++) total += bl[t][k];
            eb_16(&b, 0xFFC4); eb_16(&b, 3 + 16 + total); eb_put(&b, ids[t]);
            for (k = 1; k <= 16; k++) eb_put(&b, bl[t][k]);
            for (k = 0; k < total; k++) eb_put(&b, vl[t][k]);
        }
    }
    /* SOS */
    eb_16(&b, 0xFFDA); eb_16(&b, 6 + 2 * nc); eb_put(&b, nc);
    eb_put(&b, 1); eb_put(&b, 0x00);
    if (!gray) { eb_put(&b, 2); eb_put(&b, 0x11); eb_put(&b, 3); eb_put(&b, 0x11); }
    eb_put(&b, 0); eb_put(&b, 63); eb_put(&b, 0);
    for (my = 0; my < (h + mcuh - 1) / mcuh; my++)
        for (mx = 0; mx < (w + mcuw - 1) / mcuw; mx++) {
            float Y[4][64], Cb[64], Cr[64]; int bx, by, xx, yy;
            float cbsum[256], crsum[256];
            for (yy = 0; yy < mcuh; yy++)
                for (xx = 0; xx < mcuw; xx++) {
                    int sx = mx * mcuw + xx, sy = my * mcuh + yy; const u8* p; float r, g, bl, a, yv;
                    if (sx >= w) sx = (int)w - 1;
                    if (sy >= h) sy = (int)h - 1;
                    p = px + ((size_t)sy * (size_t)w + (size_t)sx) * 4;
                    a = p[3] / 255.0f;   /* composite onto white */
                    r = p[0] * a + 255 * (1 - a); g = p[1] * a + 255 * (1 - a); bl = p[2] * a + 255 * (1 - a);
                    yv = 0.299f * r + 0.587f * g + 0.114f * bl;
                    Y[(yy / 8) * hs + (xx / 8)][(yy % 8) * 8 + (xx % 8)] = yv - 128;
                    cbsum[yy * mcuw + xx] = -0.168736f * r - 0.331264f * g + 0.5f * bl;
                    crsum[yy * mcuw + xx] = 0.5f * r - 0.418688f * g - 0.081312f * bl;
                }
            for (by = 0; by < hs; by++)
                for (bx = 0; bx < hs; bx++)
                    eencode_block(&b, Y[by * hs + bx], lq, &dcy, &hdl, &hal);
            if (!gray) {
                for (yy = 0; yy < 8; yy++)
                    for (xx = 0; xx < 8; xx++) {
                        if (hs == 1) { Cb[yy * 8 + xx] = cbsum[yy * mcuw + xx]; Cr[yy * 8 + xx] = crsum[yy * mcuw + xx]; }
                        else {
                            int i0 = (2 * yy) * mcuw + 2 * xx;
                            Cb[yy * 8 + xx] = (cbsum[i0] + cbsum[i0 + 1] + cbsum[i0 + mcuw] + cbsum[i0 + mcuw + 1]) / 4;
                            Cr[yy * 8 + xx] = (crsum[i0] + crsum[i0 + 1] + crsum[i0 + mcuw] + crsum[i0 + mcuw + 1]) / 4;
                        }
                    }
                eencode_block(&b, Cb, cq, &dccb, &hdc, &hac);
                eencode_block(&b, Cr, cq, &dccr, &hdc, &hac);
            }
        }
    eb_flush(&b);
    eb_16(&b, 0xFFD9);
    if (b.fail) { free(b.buf); return NULL; }
    *out_len = (int64_t)b.n;
    return (char*)b.buf;
}

/* ═══════════════════════════ GIF decoder ═════════════════════════════════ */
typedef struct { const u8* p; size_t n, pos; } gin;
static int g_u8(gin* g) { return g->pos < g->n ? g->p[g->pos++] : -1; }
static int g_u16(gin* g) { int a = g_u8(g), b = g_u8(g); return (a < 0 || b < 0) ? -1 : a | (b << 8); }
static int g_skip_blocks(gin* g) {
    for (;;) { int len = g_u8(g); if (len < 0) return -1; if (len == 0) return 0; g->pos += (size_t)len; if (g->pos > g->n) return -1; }
}

/* LZW decode `npix` indices into out. Returns pixels produced or -1. */
static int g_lzw(gin* g, int minsize, u8* out, int npix) {
    u16 prefix[4096]; u8 suffix[4096], stack[4097];
    int clear, eoi, avail, codesize, codemask, old = -1, first = 0, produced = 0;
    u32 datum = 0; int bits = 0, blocklen = 0;
    if (minsize < 2 || minsize > 11) return -1;
    clear = 1 << minsize; eoi = clear + 1; avail = clear + 2; codesize = minsize + 1; codemask = (1 << codesize) - 1;
    { int i; for (i = 0; i < clear; i++) { prefix[i] = 0; suffix[i] = (u8)i; } }
    for (;;) {
        int code, in, sp = 0;
        while (bits < codesize) {
            if (blocklen == 0) {
                blocklen = g_u8(g);
                if (blocklen <= 0) goto done;
            }
            { int c = g_u8(g); if (c < 0) goto done; datum |= (u32)c << bits; bits += 8; blocklen--; }
        }
        code = (int)(datum & (u32)codemask); datum >>= codesize; bits -= codesize;
        if (code == clear) { codesize = minsize + 1; codemask = (1 << codesize) - 1; avail = clear + 2; old = -1; continue; }
        if (code == eoi) break;
        if (old == -1) {
            if (code >= clear) return -1;
            if (produced < npix) out[produced++] = suffix[code];
            old = code; first = code;
            continue;
        }
        in = code;
        if (code >= avail) {
            if (code > avail) return -1;
            stack[sp++] = (u8)first; code = old;
        }
        while (code >= clear) {
            if (sp >= 4096) return -1;
            stack[sp++] = suffix[code]; code = prefix[code];
        }
        first = suffix[code];
        stack[sp++] = (u8)first;
        if (avail < 4096) {
            prefix[avail] = (u16)old; suffix[avail] = (u8)first; avail++;
            if ((avail & codemask) == 0 && avail < 4096) { codesize++; codemask = (1 << codesize) - 1; }
        }
        old = in;
        while (sp > 0) { u8 v = stack[--sp]; if (produced < npix) out[produced++] = v; }
    }
done:
    /* skip remaining sub-blocks */
    if (blocklen > 0) g->pos += (size_t)blocklen;
    while (g->pos < g->n) { int len = g_u8(g); if (len <= 0) break; g->pos += (size_t)len; }
    return produced;
}

void* trc_gif_decode(const char* data, int64_t n) {
    img_result* r = res_new(); gin g; int sw, sh, flags, gct_size = 0, bgidx, i;
    u8 gct[768]; u8* canvas = NULL; u8* prev = NULL;
    int disposal = 0, transp = -1, delay = 0;
    if (!r) return NULL;
    g.p = (const u8*)data; g.n = (size_t)n; g.pos = 0;
    if (n < 13 || (memcmp(data, "GIF87a", 6) != 0 && memcmp(data, "GIF89a", 6) != 0)) { snprintf(r->err, sizeof r->err, "GIF: not a GIF file"); return r; }
    g.pos = 6;
    sw = g_u16(&g); sh = g_u16(&g); flags = g_u8(&g); bgidx = g_u8(&g); g_u8(&g);
    (void)bgidx;
    if (sw <= 0 || sh <= 0) { snprintf(r->err, sizeof r->err, "GIF: zero-size canvas"); return r; }
    if (flags & 0x80) {
        gct_size = 2 << (flags & 7);
        if (g.pos + (size_t)gct_size * 3 > g.n) { snprintf(r->err, sizeof r->err, "GIF: truncated colour table"); return r; }
        memcpy(gct, g.p + g.pos, (size_t)gct_size * 3); g.pos += (size_t)gct_size * 3;
    }
    r->w = sw; r->h = sh; r->loop = -1;
    canvas = (u8*)calloc((size_t)sw * sh, 4);
    if (!canvas) { snprintf(r->err, sizeof r->err, "GIF: out of memory"); return r; }
    for (;;) {
        int b = g_u8(&g);
        if (b < 0 || b == 0x3B) break;
        if (b == 0x21) {
            int label = g_u8(&g);
            if (label == 0xF9) {
                int len = g_u8(&g);
                if (len >= 4) {
                    int pf = g_u8(&g); delay = g_u16(&g); i = g_u8(&g);
                    disposal = (pf >> 2) & 7; transp = (pf & 1) ? i : -1;
                    g.pos += (size_t)(len - 4);
                } else g.pos += (size_t)len;
                g_skip_blocks(&g);
            } else if (label == 0xFF) {
                int len = g_u8(&g);
                if (len == 11 && g.pos + 11 <= g.n && memcmp(g.p + g.pos, "NETSCAPE2.0", 11) == 0) {
                    g.pos += 11;
                    { int sl = g_u8(&g); if (sl >= 3) { g_u8(&g); r->loop = g_u16(&g); g.pos += (size_t)(sl - 3); } else if (sl > 0) g.pos += (size_t)sl; }
                    g_skip_blocks(&g);
                } else { g.pos += (size_t)(len > 0 ? len : 0); g_skip_blocks(&g); }
            } else g_skip_blocks(&g);
            continue;
        }
        if (b != 0x2C) break;
        {
            int fx = g_u16(&g), fy = g_u16(&g), fw = g_u16(&g), fh = g_u16(&g), ff = g_u8(&g), lct_size = 0, minsize, got, x, y;
            u8 lct[768]; const u8* pal = gct; int palsize = gct_size; u8* idx; u8* frame;
            if (fw <= 0 || fh <= 0 || fx < 0 || fy < 0) break;
            if (ff & 0x80) {
                lct_size = 2 << (ff & 7);
                if (g.pos + (size_t)lct_size * 3 > g.n) break;
                memcpy(lct, g.p + g.pos, (size_t)lct_size * 3); g.pos += (size_t)lct_size * 3;
                pal = lct; palsize = lct_size;
            }
            minsize = g_u8(&g);
            idx = (u8*)calloc((size_t)fw * fh, 1);
            if (!idx) break;
            got = g_lzw(&g, minsize, idx, fw * fh);
            if (got < 0) { free(idx); snprintf(r->err, sizeof r->err, "GIF: corrupt LZW data"); break; }
            if (disposal == 3) { free(prev); prev = (u8*)malloc((size_t)sw * sh * 4); if (prev) memcpy(prev, canvas, (size_t)sw * sh * 4); }
            /* row mapping for interlaced images */
            {
                int* rowmap = (int*)malloc(sizeof(int) * (size_t)fh), k = 0, pass;
                if (!rowmap) { free(idx); break; }
                if (ff & 0x40) {
                    static const int start[4] = {0, 4, 2, 1}, step[4] = {8, 8, 4, 2};
                    for (pass = 0; pass < 4; pass++) for (y = start[pass]; y < fh; y += step[pass]) rowmap[k++] = y;
                } else for (y = 0; y < fh; y++) rowmap[y] = y;
                for (y = 0; y < fh; y++) {
                    int cy = fy + rowmap[y];
                    if (cy >= sh) continue;
                    for (x = 0; x < fw; x++) {
                        int cx = fx + x, ci = idx[(size_t)y * fw + x]; u8* o;
                        if (cx >= sw || ci == transp || ci >= palsize) continue;
                        o = canvas + ((size_t)cy * sw + cx) * 4;
                        o[0] = pal[ci * 3]; o[1] = pal[ci * 3 + 1]; o[2] = pal[ci * 3 + 2]; o[3] = 255;
                    }
                }
                free(rowmap);
            }
            free(idx);
            frame = (u8*)malloc((size_t)sw * sh * 4);
            if (!frame) break;
            memcpy(frame, canvas, (size_t)sw * sh * 4);
            res_add_frame(r, frame, delay * 10);
            if (disposal == 2) {
                for (y = fy; y < fy + fh && y < sh; y++) for (x = fx; x < fx + fw && x < sw; x++) memset(canvas + ((size_t)y * sw + x) * 4, 0, 4);
            } else if (disposal == 3 && prev) memcpy(canvas, prev, (size_t)sw * sh * 4);
            disposal = 0; transp = -1; delay = 0;
        }
    }
    free(canvas); free(prev);
    if (r->nframes == 0 && !r->err[0]) snprintf(r->err, sizeof r->err, "GIF: no image data");
    return r;
}

/* ═══════════════════════════ GIF encoder ═════════════════════════════════ */
typedef struct { int r0, r1, g0, g1, b0, b1; long count; } gbox;

static int g_hist_idx(int r, int g, int b) { return (r << 10) | (g << 5) | b; }

/* Build a palette of <= maxc colours from 5-bit histograms by median cut. */
static int g_median_cut(const long* hist, int maxc, u8* pal) {
    gbox boxes[256]; int nb = 1, i;
    boxes[0].r0 = 0; boxes[0].r1 = 31; boxes[0].g0 = 0; boxes[0].g1 = 31; boxes[0].b0 = 0; boxes[0].b1 = 31;
    /* shrink + count */
    for (;;) {
        for (i = 0; i < nb; i++) {
            gbox* bx = &boxes[i]; int r, g, b, rmin = 31, rmax = 0, gmin = 31, gmax = 0, bmin = 31, bmax = 0; long cnt = 0;
            for (r = bx->r0; r <= bx->r1; r++) for (g = bx->g0; g <= bx->g1; g++) for (b = bx->b0; b <= bx->b1; b++) {
                long c = hist[g_hist_idx(r, g, b)];
                if (c) { cnt += c; if (r < rmin) rmin = r; if (r > rmax) rmax = r; if (g < gmin) gmin = g; if (g > gmax) gmax = g; if (b < bmin) bmin = b; if (b > bmax) bmax = b; }
            }
            if (cnt) { bx->r0 = rmin; bx->r1 = rmax; bx->g0 = gmin; bx->g1 = gmax; bx->b0 = bmin; bx->b1 = bmax; }
            bx->count = cnt;
        }
        if (nb >= maxc) break;
        {
            int best = -1, bestv = 0, axis = 0, cut, r, g, b; long half, acc = 0; gbox* bx; gbox nbx;
            for (i = 0; i < nb; i++) {
                int dr = boxes[i].r1 - boxes[i].r0, dg = boxes[i].g1 - boxes[i].g0, db = boxes[i].b1 - boxes[i].b0, d = dr > dg ? (dr > db ? dr : db) : (dg > db ? dg : db);
                long v = (long)d * (boxes[i].count > 0 ? 1 : 0);
                if (boxes[i].count > 0 && d > 0 && (best < 0 || v * 1000 + boxes[i].count / 1000 > bestv)) { best = i; bestv = (int)(v * 1000 + boxes[i].count / 1000); }
            }
            if (best < 0) break;
            bx = &boxes[best];
            {
                int dr = bx->r1 - bx->r0, dg = bx->g1 - bx->g0, db = bx->b1 - bx->b0;
                axis = (dr >= dg && dr >= db) ? 0 : (dg >= db ? 1 : 2);
            }
            half = bx->count / 2;
            cut = axis == 0 ? bx->r0 : axis == 1 ? bx->g0 : bx->b0;
            for (;;) {
                long slice = 0;
                for (r = axis == 0 ? cut : bx->r0; r <= (axis == 0 ? cut : bx->r1); r++)
                    for (g = axis == 1 ? cut : bx->g0; g <= (axis == 1 ? cut : bx->g1); g++)
                        for (b = axis == 2 ? cut : bx->b0; b <= (axis == 2 ? cut : bx->b1); b++) slice += hist[g_hist_idx(r, g, b)];
                acc += slice;
                if (acc >= half || cut >= (axis == 0 ? bx->r1 : axis == 1 ? bx->g1 : bx->b1) - 1) break;
                cut++;
            }
            nbx = *bx;
            if (axis == 0) { bx->r1 = cut; nbx.r0 = cut + 1; }
            else if (axis == 1) { bx->g1 = cut; nbx.g0 = cut + 1; }
            else { bx->b1 = cut; nbx.b0 = cut + 1; }
            boxes[nb++] = nbx;
        }
    }
    /* average colour of each box */
    {
        int k = 0;
        for (i = 0; i < nb; i++) {
            int r, g, b; double sr = 0, sg = 0, sb = 0; long cnt = 0;
            for (r = boxes[i].r0; r <= boxes[i].r1; r++) for (g = boxes[i].g0; g <= boxes[i].g1; g++) for (b = boxes[i].b0; b <= boxes[i].b1; b++) {
                long c = hist[g_hist_idx(r, g, b)];
                if (c) { sr += (double)c * (r * 8 + 4); sg += (double)c * (g * 8 + 4); sb += (double)c * (b * 8 + 4); cnt += c; }
            }
            if (!cnt) continue;
            pal[k * 3] = clampf8(sr / cnt); pal[k * 3 + 1] = clampf8(sg / cnt); pal[k * 3 + 2] = clampf8(sb / cnt); k++;
        }
        return k;
    }
}

typedef struct {
    ebuf out; int w, h, frames;
} genc;

static void g_lzw_encode(ebuf* o, const u8* idx, int n, int minsize) {
    /* hash table code dictionary */
    int clear = 1 << minsize, eoi = clear + 1, next = clear + 2, codesize = minsize + 1, i, cur;
    int* hcode = (int*)malloc(sizeof(int) * 8192); int* hkey = (int*)malloc(sizeof(int) * 8192);
    u8 block[255]; int bl = 0; u32 acc = 0; int bits = 0;
#define GEMIT(code) do { acc |= (u32)(code) << bits; bits += codesize; \
        while (bits >= 8) { block[bl++] = (u8)acc; acc >>= 8; bits -= 8; if (bl == 255) { eb_put(o, 255); for (int _i = 0; _i < 255; _i++) eb_put(o, block[_i]); bl = 0; } } } while (0)
    if (!hcode || !hkey) { free(hcode); free(hkey); o->fail = 1; return; }
    for (i = 0; i < 8192; i++) hkey[i] = -1;
    eb_put(o, minsize);
    GEMIT(clear);
    if (n == 0) { GEMIT(eoi); goto flush; }
    cur = idx[0];
    for (i = 1; i < n; i++) {
        int k = idx[i], key = (cur << 8) | k, h = ((cur * 31) ^ (k * 2654435761u)) & 8191;
        while (hkey[h] != -1 && hkey[h] != key) h = (h + 1) & 8191;
        if (hkey[h] == key) { cur = hcode[h]; continue; }
        GEMIT(cur);
        if (next < 4096) {
            hkey[h] = key; hcode[h] = next++;
            if (next > (1 << codesize) && codesize < 12) codesize++;
        } else {
            GEMIT(clear);
            for (int c = 0; c < 8192; c++) hkey[c] = -1;
            next = clear + 2; codesize = minsize + 1;
        }
        cur = k;
    }
    GEMIT(cur);
    GEMIT(eoi);
flush:
    if (bits > 0) { block[bl++] = (u8)acc; }
    if (bl > 0) { eb_put(o, bl); for (i = 0; i < bl; i++) eb_put(o, block[i]); }
    eb_put(o, 0);
    free(hcode); free(hkey);
#undef GEMIT
}

void* trc_gif_begin(int64_t w, int64_t h, int64_t loop) {
    genc* e = (genc*)calloc(1, sizeof(genc));
    if (!e) return NULL;
    e->w = (int)w; e->h = (int)h;
    eb_put(&e->out, 'G'); eb_put(&e->out, 'I'); eb_put(&e->out, 'F'); eb_put(&e->out, '8'); eb_put(&e->out, '9'); eb_put(&e->out, 'a');
    eb_put(&e->out, (int)(w & 255)); eb_put(&e->out, (int)(w >> 8)); eb_put(&e->out, (int)(h & 255)); eb_put(&e->out, (int)(h >> 8));
    eb_put(&e->out, 0); eb_put(&e->out, 0); eb_put(&e->out, 0);       /* no global colour table */
    if (loop >= 0) {
        static const char app[] = "NETSCAPE2.0";
        int i;
        eb_put(&e->out, 0x21); eb_put(&e->out, 0xFF); eb_put(&e->out, 11);
        for (i = 0; i < 11; i++) eb_put(&e->out, app[i]);
        eb_put(&e->out, 3); eb_put(&e->out, 1); eb_put(&e->out, (int)(loop & 255)); eb_put(&e->out, (int)((loop >> 8) & 255)); eb_put(&e->out, 0);
    }
    return e;
}
/* delay_ms rounded to centiseconds; dither: Floyd-Steinberg; max_colors 2..256. */
int64_t trc_gif_add_frame(void* hh, const char* rgba, int64_t delay_ms, int64_t dither, int64_t max_colors) {
    genc* e = (genc*)hh; const u8* px = (const u8*)rgba; int w, h, n, i, transparent = 0, ncol = 0, minsize, bitsz;
    u8 pal[768]; u8* idx; long* hist; int exact = 1; u32 uniq[256]; int nuniq = 0;
    if (!e) return -1;
    w = e->w; h = e->h; n = w * h;
    if (max_colors < 2) max_colors = 2;
    if (max_colors > 256) max_colors = 256;
    for (i = 0; i < n; i++) if (px[i * 4 + 3] < 128) { transparent = 1; break; }
    /* exact palette when it fits */
    for (i = 0; i < n && exact; i++) {
        u32 c; int k, f = 0;
        if (px[i * 4 + 3] < 128) continue;
        c = ((u32)px[i * 4] << 16) | ((u32)px[i * 4 + 1] << 8) | px[i * 4 + 2];
        for (k = 0; k < nuniq; k++) if (uniq[k] == c) { f = 1; break; }
        if (!f) { if (nuniq >= max_colors - transparent) exact = 0; else uniq[nuniq++] = c; }
    }
    idx = (u8*)malloc((size_t)n);
    if (!idx) return -1;
    if (exact) {
        for (i = 0; i < nuniq; i++) { pal[i * 3] = (u8)(uniq[i] >> 16); pal[i * 3 + 1] = (u8)(uniq[i] >> 8); pal[i * 3 + 2] = (u8)uniq[i]; }
        ncol = nuniq;
        for (i = 0; i < n; i++) {
            u32 c; int k;
            if (px[i * 4 + 3] < 128) { idx[i] = (u8)ncol; continue; }
            c = ((u32)px[i * 4] << 16) | ((u32)px[i * 4 + 1] << 8) | px[i * 4 + 2];
            for (k = 0; k < nuniq; k++) if (uniq[k] == c) break;
            idx[i] = (u8)k;
        }
    } else {
        int* cache; float* err = NULL;
        hist = (long*)calloc(32768, sizeof(long));
        cache = (int*)malloc(sizeof(int) * 32768);
        if (!hist || !cache) { free(hist); free(cache); free(idx); return -1; }
        for (i = 0; i < 32768; i++) cache[i] = -1;
        for (i = 0; i < n; i++) if (px[i * 4 + 3] >= 128) hist[g_hist_idx(px[i * 4] >> 3, px[i * 4 + 1] >> 3, px[i * 4 + 2] >> 3)]++;
        ncol = g_median_cut(hist, (int)max_colors - transparent, pal);
        if (dither) err = (float*)calloc((size_t)(w + 2) * 2 * 3, sizeof(float));
        {
            int y, x;
            for (y = 0; y < h; y++) {
                float* cur = err ? err + (size_t)((y & 1) * (w + 2)) * 3 : NULL;
                float* nxt = err ? err + (size_t)(((y + 1) & 1) * (w + 2)) * 3 : NULL;
                if (nxt) memset(nxt, 0, sizeof(float) * (size_t)(w + 2) * 3);
                for (x = 0; x < w; x++) {
                    const u8* p = px + ((size_t)y * w + x) * 4; int r, g, b, key, best, k;
                    if (p[3] < 128) { idx[y * w + x] = (u8)ncol; continue; }
                    r = p[0]; g = p[1]; b = p[2];
                    if (cur) { r = clamp8((int)(r + cur[(x + 1) * 3])); g = clamp8((int)(g + cur[(x + 1) * 3 + 1])); b = clamp8((int)(b + cur[(x + 1) * 3 + 2])); }
                    key = g_hist_idx(r >> 3, g >> 3, b >> 3);
                    best = cur ? -1 : cache[key];
                    if (best < 0) {
                        long bd = 1L << 30;
                        for (k = 0; k < ncol; k++) {
                            long dr = r - pal[k * 3], dg = g - pal[k * 3 + 1], db = b - pal[k * 3 + 2], d = dr * dr * 2 + dg * dg * 4 + db * db * 3;
                            if (d < bd) { bd = d; best = k; }
                        }
                        if (!cur) cache[key] = best;
                    }
                    idx[y * w + x] = (u8)best;
                    if (cur) {
                        float er = (float)(r - pal[best * 3]), eg = (float)(g - pal[best * 3 + 1]), ebb = (float)(b - pal[best * 3 + 2]);
                        cur[(x + 2) * 3] += er * 7 / 16; cur[(x + 2) * 3 + 1] += eg * 7 / 16; cur[(x + 2) * 3 + 2] += ebb * 7 / 16;
                        nxt[x * 3] += er * 3 / 16; nxt[x * 3 + 1] += eg * 3 / 16; nxt[x * 3 + 2] += ebb * 3 / 16;
                        nxt[(x + 1) * 3] += er * 5 / 16; nxt[(x + 1) * 3 + 1] += eg * 5 / 16; nxt[(x + 1) * 3 + 2] += ebb * 5 / 16;
                        nxt[(x + 2) * 3] += er / 16; nxt[(x + 2) * 3 + 1] += eg / 16; nxt[(x + 2) * 3 + 2] += ebb / 16;
                    }
                }
            }
        }
        free(err); free(hist); free(cache);
    }
    if (transparent) { pal[ncol * 3] = pal[ncol * 3 + 1] = pal[ncol * 3 + 2] = 0; ncol++; }
    bitsz = 1; while ((1 << bitsz) < ncol) bitsz++;
    /* graphic control extension */
    {
        int cs = (int)((delay_ms + 5) / 10);
        eb_put(&e->out, 0x21); eb_put(&e->out, 0xF9); eb_put(&e->out, 4);
        eb_put(&e->out, (transparent ? 1 : 0) | (e->frames > 0 || transparent ? (transparent ? 2 << 2 : 1 << 2) : 0));
        eb_put(&e->out, cs & 255); eb_put(&e->out, (cs >> 8) & 255); eb_put(&e->out, transparent ? ncol - 1 : 0); eb_put(&e->out, 0);
    }
    /* image descriptor with a local colour table */
    eb_put(&e->out, 0x2C); eb_put(&e->out, 0); eb_put(&e->out, 0); eb_put(&e->out, 0); eb_put(&e->out, 0);
    eb_put(&e->out, w & 255); eb_put(&e->out, w >> 8); eb_put(&e->out, h & 255); eb_put(&e->out, h >> 8);
    eb_put(&e->out, 0x80 | (bitsz - 1));
    for (i = 0; i < (1 << bitsz); i++) {
        if (i < ncol) { eb_put(&e->out, pal[i * 3]); eb_put(&e->out, pal[i * 3 + 1]); eb_put(&e->out, pal[i * 3 + 2]); }
        else { eb_put(&e->out, 0); eb_put(&e->out, 0); eb_put(&e->out, 0); }
    }
    minsize = bitsz < 2 ? 2 : bitsz;
    g_lzw_encode(&e->out, idx, n, minsize);
    free(idx);
    e->frames++;
    return e->out.fail ? -1 : 0;
}
/* Finish; returns the file bytes (free with trc_img_buf_free) and frees the encoder. */
char* trc_gif_finish(void* hh, int64_t* out_len) {
    genc* e = (genc*)hh; char* p;
    if (!e) return NULL;
    eb_put(&e->out, 0x3B);
    if (e->out.fail) { free(e->out.buf); free(e); return NULL; }
    *out_len = (int64_t)e->out.n;
    p = (char*)e->out.buf;
    free(e);
    return p;
}

/* ═══════════════════════════ resampling ══════════════════════════════════ */
static double f_box(double x) { return (x > -0.5 && x <= 0.5) ? 1.0 : 0.0; }
static double f_tri(double x) { x = fabs(x); return x < 1 ? 1 - x : 0; }
static double f_cubic(double x) {   /* Catmull-Rom (a = -0.5) */
    double a = -0.5; x = fabs(x);
    if (x < 1) return (a + 2) * x * x * x - (a + 3) * x * x + 1;
    if (x < 2) return a * x * x * x - 5 * a * x * x + 8 * a * x - 4 * a;
    return 0;
}
static double f_sinc(double x) { if (x == 0) return 1; x *= 3.14159265358979323846; return sin(x) / x; }
static double f_lanczos(double x) { return fabs(x) < 3 ? f_sinc(x) * f_sinc(x / 3) : 0; }

/* filter: 0 nearest, 1 box, 2 bilinear, 3 bicubic, 4 lanczos3 */
static void resample_axis(const float* in, int iw, int ih, float* out, int ow, int horizontal, int filter) {
    int olen = ow, ilen = horizontal ? iw : ih, lines = horizontal ? ih : iw, o;
    double scale = (double)olen / ilen, support, fscale;
    double (*f)(double) = filter == 1 ? f_box : filter == 2 ? f_tri : filter == 3 ? f_cubic : f_lanczos;
    support = filter == 1 ? 0.5 : filter == 2 ? 1 : filter == 3 ? 2 : 3;
    fscale = scale < 1 ? 1 / scale : 1;    /* widen the kernel when shrinking */
    support *= fscale;
    for (o = 0; o < olen; o++) {
        double center = (o + 0.5) / scale; int lo = (int)floor(center - support), hi = (int)ceil(center + support), k, l;
        double wsum = 0; double wts[2048]; int n = 0;
        if (hi - lo + 1 > 2048) hi = lo + 2047;
        for (k = lo; k <= hi; k++) { double wv = f((k + 0.5 - center) / fscale); wts[n++] = wv; wsum += wv; }
        if (wsum == 0) wsum = 1;
        for (l = 0; l < lines; l++) {
            double acc[4] = {0, 0, 0, 0}; int c;
            for (k = lo, n = 0; k <= hi; k++, n++) {
                int kk = k < 0 ? 0 : k >= ilen ? ilen - 1 : k; const float* s;
                if (wts[n] == 0) continue;
                s = horizontal ? in + ((size_t)l * iw + kk) * 4 : in + ((size_t)kk * iw + l) * 4;
                for (c = 0; c < 4; c++) acc[c] += wts[n] * s[c];
            }
            {
                float* d = horizontal ? out + ((size_t)l * ow + o) * 4 : out + ((size_t)o * iw + l) * 4;
                for (c = 0; c < 4; c++) d[c] = (float)(acc[c] / wsum);
            }
        }
    }
}
int64_t trc_img_resize(const char* src, int64_t sw, int64_t sh, char* dst, int64_t dw, int64_t dh, int64_t filter) {
    const u8* s = (const u8*)src; u8* d = (u8*)dst; size_t i;
    if (sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0) return -1;
    if (filter == 0) {
        int64_t x, y;
        for (y = 0; y < dh; y++) for (x = 0; x < dw; x++) {
            int64_t sx = (x * sw) / dw, sy = (y * sh) / dh;
            memcpy(d + (y * dw + x) * 4, s + (sy * sw + sx) * 4, 4);
        }
        return 0;
    }
    {
        float* a = (float*)malloc(sizeof(float) * (size_t)(sw * sh * 4));
        float* b = (float*)malloc(sizeof(float) * (size_t)(dw * sh * 4));
        float* c = (float*)malloc(sizeof(float) * (size_t)(dw * dh * 4));
        if (!a || !b || !c) { free(a); free(b); free(c); return -1; }
        for (i = 0; i < (size_t)(sw * sh); i++) {   /* premultiply */
            float al = s[i * 4 + 3] / 255.0f;
            a[i * 4] = s[i * 4] * al; a[i * 4 + 1] = s[i * 4 + 1] * al; a[i * 4 + 2] = s[i * 4 + 2] * al; a[i * 4 + 3] = s[i * 4 + 3];
        }
        resample_axis(a, (int)sw, (int)sh, b, (int)dw, 1, (int)filter);
        resample_axis(b, (int)dw, (int)sh, c, (int)dh, 0, (int)filter);
        for (i = 0; i < (size_t)(dw * dh); i++) {
            float al = c[i * 4 + 3]; u8 ab = clampf8(al);
            if (ab == 0) { memset(d + i * 4, 0, 4); continue; }
            d[i * 4] = clampf8(c[i * 4] * 255.0f / al); d[i * 4 + 1] = clampf8(c[i * 4 + 1] * 255.0f / al); d[i * 4 + 2] = clampf8(c[i * 4 + 2] * 255.0f / al); d[i * 4 + 3] = ab;
        }
        free(a); free(b); free(c);
    }
    return 0;
}

/* ═══════════════════════════ filters ═════════════════════════════════════ */
/* Pointwise colour ops. op: 0 grayscale, 1 invert, 2 brightness (+p1),
 * 3 contrast (factor p1), 4 gamma (p1), 5 saturation (factor p1), 6 sepia,
 * 7 threshold (p1), 8 opacity (factor p1), 9 posterize (levels p1). */
void trc_img_point(int64_t op, const char* src, int64_t n, char* dst, double p1) {
    const u8* s = (const u8*)src; u8* d = (u8*)dst; int64_t i; u8 lut[256]; int use_lut = 0, k;
    if (op == 4) { for (k = 0; k < 256; k++) lut[k] = clampf8(255.0 * pow(k / 255.0, 1.0 / (p1 > 0 ? p1 : 1))); use_lut = 1; }
    if (op == 3) { for (k = 0; k < 256; k++) lut[k] = clampf8((k - 128) * p1 + 128); use_lut = 1; }
    if (op == 2) { for (k = 0; k < 256; k++) lut[k] = clampf8(k + p1); use_lut = 1; }
    if (op == 1) { for (k = 0; k < 256; k++) lut[k] = (u8)(255 - k); use_lut = 1; }
    if (op == 9) { int lv = (int)p1 < 2 ? 2 : (int)p1; for (k = 0; k < 256; k++) lut[k] = clampf8((double)((k * lv) / 256) * 255.0 / (lv - 1)); use_lut = 1; }
    for (i = 0; i < n; i++) {
        const u8* p = s + i * 4; u8* o = d + i * 4;
        if (use_lut) { o[0] = lut[p[0]]; o[1] = lut[p[1]]; o[2] = lut[p[2]]; o[3] = p[3]; continue; }
        switch (op) {
            case 0: { u8 y = clampf8(0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2]); o[0] = o[1] = o[2] = y; o[3] = p[3]; break; }
            case 5: { double y = 0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2]; o[0] = clampf8(y + (p[0] - y) * p1); o[1] = clampf8(y + (p[1] - y) * p1); o[2] = clampf8(y + (p[2] - y) * p1); o[3] = p[3]; break; }
            case 6: o[0] = clampf8(0.393 * p[0] + 0.769 * p[1] + 0.189 * p[2]); o[1] = clampf8(0.349 * p[0] + 0.686 * p[1] + 0.168 * p[2]); o[2] = clampf8(0.272 * p[0] + 0.534 * p[1] + 0.131 * p[2]); o[3] = p[3]; break;
            case 7: { u8 v = (0.299 * p[0] + 0.587 * p[1] + 0.114 * p[2]) >= p1 ? 255 : 0; o[0] = o[1] = o[2] = v; o[3] = p[3]; break; }
            case 8: o[0] = p[0]; o[1] = p[1]; o[2] = p[2]; o[3] = clampf8(p[3] * p1); break;
            default: memcpy(o, p, 4);
        }
    }
}
/* General k x k convolution (k odd) on RGB; alpha copied. Edges clamp. */
int64_t trc_img_convolve(const char* src, int64_t w, int64_t h, char* dst, const double* kernel, int64_t k, double divisor, double bias) {
    const u8* s = (const u8*)src; u8* d = (u8*)dst; int64_t x, y; int r = (int)k / 2;
    if (k < 1 || !(k & 1)) return -1;
    if (divisor == 0) divisor = 1;
    for (y = 0; y < h; y++)
        for (x = 0; x < w; x++) {
            double acc[3] = {0, 0, 0}; int i, j, c;
            for (j = -r; j <= r; j++) for (i = -r; i <= r; i++) {
                int64_t sx = x + i, sy = y + j; double kv = kernel[(j + r) * k + (i + r)]; const u8* p;
                if (sx < 0) sx = 0;
                if (sx >= w) sx = w - 1;
                if (sy < 0) sy = 0;
                if (sy >= h) sy = h - 1;
                p = s + (sy * w + sx) * 4;
                for (c = 0; c < 3; c++) acc[c] += kv * p[c];
            }
            for (c = 0; c < 3; c++) d[(y * w + x) * 4 + c] = clampf8(acc[c] / divisor + bias);
            d[(y * w + x) * 4 + 3] = s[(y * w + x) * 4 + 3];
        }
    return 0;
}
/* Separable blur: kind 0 Gaussian (sigma), 1 box (radius). All four channels. */
int64_t trc_img_blur(const char* src, int64_t w, int64_t h, char* dst, int64_t kind, double amount) {
    const u8* s = (const u8*)src; u8* d = (u8*)dst; int r, i; double* kw; float* tmp; int64_t x, y;
    if (kind == 0) { r = (int)ceil(amount * 3); if (amount <= 0) { memcpy(dst, src, (size_t)(w * h * 4)); return 0; } }
    else r = (int)amount;
    if (r < 1) { memcpy(dst, src, (size_t)(w * h * 4)); return 0; }
    if (r > 500) r = 500;
    kw = (double*)malloc(sizeof(double) * (size_t)(2 * r + 1));
    tmp = (float*)malloc(sizeof(float) * (size_t)(w * h * 4));
    if (!kw || !tmp) { free(kw); free(tmp); return -1; }
    {
        double sum = 0;
        for (i = -r; i <= r; i++) { kw[i + r] = kind == 0 ? exp(-(double)i * i / (2 * amount * amount)) : 1.0; sum += kw[i + r]; }
        for (i = 0; i <= 2 * r; i++) kw[i] /= sum;
    }
    for (y = 0; y < h; y++) for (x = 0; x < w; x++) {
        double a[4] = {0, 0, 0, 0}; int c;
        for (i = -r; i <= r; i++) { int64_t sx = x + i; const u8* p; if (sx < 0) sx = 0; if (sx >= w) sx = w - 1; p = s + (y * w + sx) * 4; for (c = 0; c < 4; c++) a[c] += kw[i + r] * p[c]; }
        for (c = 0; c < 4; c++) tmp[(y * w + x) * 4 + c] = (float)a[c];
    }
    for (y = 0; y < h; y++) for (x = 0; x < w; x++) {
        double a[4] = {0, 0, 0, 0}; int c;
        for (i = -r; i <= r; i++) { int64_t sy = y + i; const float* p; if (sy < 0) sy = 0; if (sy >= h) sy = h - 1; p = tmp + (sy * w + x) * 4; for (c = 0; c < 4; c++) a[c] += kw[i + r] * p[c]; }
        for (c = 0; c < 4; c++) d[(y * w + x) * 4 + c] = clampf8(a[c]);
    }
    free(kw); free(tmp);
    return 0;
}
/* Unsharp mask: src + amount * (src - gaussian(sigma)), above `threshold`. */
int64_t trc_img_unsharp(const char* src, int64_t w, int64_t h, char* dst, double sigma, double amount, double threshold) {
    u8* blur = (u8*)malloc((size_t)(w * h * 4)); const u8* s = (const u8*)src; u8* d = (u8*)dst; int64_t i; int c;
    if (!blur) return -1;
    if (trc_img_blur(src, w, h, (char*)blur, 0, sigma) < 0) { free(blur); return -1; }
    for (i = 0; i < w * h; i++) {
        for (c = 0; c < 3; c++) {
            int diff = s[i * 4 + c] - blur[i * 4 + c];
            d[i * 4 + c] = abs(diff) >= threshold ? clampf8(s[i * 4 + c] + amount * diff) : s[i * 4 + c];
        }
        d[i * 4 + 3] = s[i * 4 + 3];
    }
    free(blur);
    return 0;
}
/* Sobel edge magnitude on luminance (gray output, alpha kept). */
int64_t trc_img_sobel(const char* src, int64_t w, int64_t h, char* dst) {
    const u8* s = (const u8*)src; u8* d = (u8*)dst; int64_t x, y;
    float* lum = (float*)malloc(sizeof(float) * (size_t)(w * h));
    if (!lum) return -1;
    for (x = 0; x < w * h; x++) lum[x] = 0.299f * s[x * 4] + 0.587f * s[x * 4 + 1] + 0.114f * s[x * 4 + 2];
#define L(xx, yy) lum[((yy) < 0 ? 0 : (yy) >= h ? h - 1 : (yy)) * w + ((xx) < 0 ? 0 : (xx) >= w ? w - 1 : (xx))]
    for (y = 0; y < h; y++) for (x = 0; x < w; x++) {
        float gx = -L(x - 1, y - 1) - 2 * L(x - 1, y) - L(x - 1, y + 1) + L(x + 1, y - 1) + 2 * L(x + 1, y) + L(x + 1, y + 1);
        float gy = -L(x - 1, y - 1) - 2 * L(x, y - 1) - L(x + 1, y - 1) + L(x - 1, y + 1) + 2 * L(x, y + 1) + L(x + 1, y + 1);
        u8 v = clampf8(sqrt(gx * gx + gy * gy));
        d[(y * w + x) * 4] = d[(y * w + x) * 4 + 1] = d[(y * w + x) * 4 + 2] = v; d[(y * w + x) * 4 + 3] = s[(y * w + x) * 4 + 3];
    }
#undef L
    free(lum);
    return 0;
}
/* Geometry. op: 0 flip horizontal, 1 flip vertical, 2 rotate 90 cw, 3 rotate 180,
 * 4 rotate 270 cw, 5 transpose, 6 crop (x, y, cw, ch). dst sized by the caller. */
int64_t trc_img_geom(int64_t op, const char* src, int64_t w, int64_t h, char* dst, int64_t cx, int64_t cy, int64_t cw, int64_t ch) {
    const u8* s = (const u8*)src; u8* d = (u8*)dst; int64_t x, y;
    if (op == 6) {
        if (cx < 0 || cy < 0 || cw <= 0 || ch <= 0 || cx + cw > w || cy + ch > h) return -1;
        for (y = 0; y < ch; y++) memcpy(d + y * cw * 4, s + ((cy + y) * w + cx) * 4, (size_t)cw * 4);
        return 0;
    }
    for (y = 0; y < h; y++) for (x = 0; x < w; x++) {
        const u8* p = s + (y * w + x) * 4; int64_t o;
        switch (op) {
            case 0: o = y * w + (w - 1 - x); break;
            case 1: o = (h - 1 - y) * w + x; break;
            case 2: o = x * h + (h - 1 - y); break;
            case 3: o = (h - 1 - y) * w + (w - 1 - x); break;
            case 4: o = (w - 1 - x) * h + y; break;
            case 5: o = x * h + y; break;
            default: return -1;
        }
        memcpy(d + o * 4, p, 4);
    }
    return 0;
}
/* Alpha-composite `top` (tw x th) onto `base` (bw x bh) at (x, y), in place. */
void trc_img_blend(char* base, int64_t bw, int64_t bh, const char* top, int64_t tw, int64_t th, int64_t ox, int64_t oy, double opacity) {
    u8* b = (u8*)base; const u8* t = (const u8*)top; int64_t x, y;
    for (y = 0; y < th; y++) for (x = 0; x < tw; x++) {
        int64_t bx = ox + x, by = oy + y; const u8* s; u8* o; double sa, da, oa; int c;
        if (bx < 0 || by < 0 || bx >= bw || by >= bh) continue;
        s = t + (y * tw + x) * 4; o = b + (by * bw + bx) * 4;
        sa = s[3] / 255.0 * opacity; da = o[3] / 255.0; oa = sa + da * (1 - sa);
        if (oa <= 0) { memset(o, 0, 4); continue; }
        for (c = 0; c < 3; c++) o[c] = clampf8((s[c] * sa + o[c] * da * (1 - sa)) / oa);
        o[3] = clampf8(oa * 255);
    }
}
