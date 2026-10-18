/*
 * std/image/tiff_native.c -- baseline TIFF decode/encode for std.image.tiff.
 *
 * Linked through `# tauraro-cpp-shim: tiff_native.c` in std/image/tiff.tr.
 * Pixel buffers are RGBA8, row-major, top row first, no padding.
 *
 *   decode ... classic TIFF ("II"/"MM", not BigTIFF); strips and tiles;
 *              chunky and planar layouts; compression none, PackBits, LZW
 *              and Deflate (zlib / Adobe); horizontal predictor (2);
 *              1/2/4/8/16-bit samples; WhiteIsZero, BlackIsZero, RGB,
 *              palette, CMYK; associated (premultiplied) and unassociated
 *              alpha; any page of a multi-page file plus the page count
 *   encode ... 8-bit RGB / RGBA (alpha dropped when every pixel is opaque),
 *              uncompressed or LZW with the horizontal predictor; one or
 *              more pages
 *
 * Written from the TIFF 6.0 specification; the inflate below is an
 * independent implementation of RFC 1950/1951.
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

#define TF_MAX_PIXELS ((size_t)1 << 28)

typedef struct {
    int w, h, npages, page, bits, spp, photometric, compression;
    u8* rgba;
    char err[160];
} tf_result;

static void tf_fail(tf_result* r, const char* m) { if (!r->err[0]) snprintf(r->err, sizeof r->err, "TIFF: %s", m); }

void trc_tiff_free(void* h) { tf_result* r = (tf_result*)h; if (!r) return; free(r->rgba); free(r); }
int64_t trc_tiff_ok(void* h) { tf_result* r = (tf_result*)h; return r && r->rgba && !r->err[0]; }
int64_t trc_tiff_w(void* h) { return h ? ((tf_result*)h)->w : 0; }
int64_t trc_tiff_h(void* h) { return h ? ((tf_result*)h)->h : 0; }
int64_t trc_tiff_npages(void* h) { return h ? ((tf_result*)h)->npages : 0; }
int64_t trc_tiff_info(void* h, int64_t what) {
    tf_result* r = (tf_result*)h;
    if (!r) return 0;
    switch (what) { case 0: return r->bits; case 1: return r->spp; case 2: return r->photometric; default: return r->compression; }
}
int64_t trc_tiff_error(void* h, char* out, int64_t cap) {
    tf_result* r = (tf_result*)h; size_t n;
    if (!r) { memcpy(out, "TIFF: out of memory", 19); return 19; }
    n = strlen(r->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, r->err, n); return (int64_t)n;
}
void trc_tiff_copy(void* h, char* out) { tf_result* r = (tf_result*)h; if (r && r->rgba) memcpy(out, r->rgba, (size_t)r->w * r->h * 4); }
void trc_tiff_buf_free(char* p) { free(p); }

/* ═════════════════════════════ inflate ═══════════════════════════════════ */
typedef struct { const u8* p; size_t n, pos; u32 bitbuf; int bitcnt; int err; u8* out; size_t olen, ocap; } inf_s;
typedef struct { short count[16]; short sym[320]; } inf_h;

static int inf_bits(inf_s* s, int need) {
    u32 v = s->bitbuf;
    while (s->bitcnt < need) {
        if (s->pos >= s->n) { s->err = 1; return 0; }
        v |= (u32)s->p[s->pos++] << s->bitcnt;
        s->bitcnt += 8;
    }
    s->bitbuf = v >> need; s->bitcnt -= need;
    return (int)(v & ((1u << need) - 1));
}
static int inf_put(inf_s* s, u8 b) {
    if (s->olen >= s->ocap) return -1;          /* caller sizes the output exactly */
    s->out[s->olen++] = b;
    return 0;
}
static int inf_build(inf_h* h, const short* len, int n) {
    short offs[16]; int i, left = 1;
    memset(h->count, 0, sizeof h->count);
    for (i = 0; i < n; i++) h->count[len[i]]++;
    if (h->count[0] == n) return 0;
    for (i = 1; i < 16; i++) { left <<= 1; left -= h->count[i]; if (left < 0) return -1; }
    offs[1] = 0;
    for (i = 1; i < 15; i++) offs[i + 1] = (short)(offs[i] + h->count[i]);
    for (i = 0; i < n; i++) if (len[i]) h->sym[offs[len[i]]++] = (short)i;
    return left;
}
static int inf_decode(inf_s* s, const inf_h* h) {
    int code = 0, first = 0, index = 0, len;
    for (len = 1; len < 16; len++) {
        int count;
        code |= inf_bits(s, 1);
        if (s->err) return -1;
        count = h->count[len];
        if (code - count < first) return h->sym[index + (code - first)];
        index += count; first += count; first <<= 1; code <<= 1;
    }
    return -1;
}
static const short lbase[29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
static const short lext[29] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
static const short dbase[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
static const short dext[30] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };

static int inf_codes(inf_s* s, const inf_h* lc, const inf_h* dc) {
    for (;;) {
        int sym = inf_decode(s, lc);
        if (sym < 0) return -1;
        if (sym < 256) { if (inf_put(s, (u8)sym)) return 1; }
        else if (sym == 256) return 0;
        else {
            int len, dist;
            sym -= 257;
            if (sym >= 29) return -1;
            len = lbase[sym] + inf_bits(s, lext[sym]);
            sym = inf_decode(s, dc);
            if (sym < 0 || sym >= 30) return -1;
            dist = dbase[sym] + inf_bits(s, dext[sym]);
            if (s->err || (size_t)dist > s->olen) return -1;
            while (len--) { if (inf_put(s, s->out[s->olen - (size_t)dist])) return 1; }
        }
    }
}
/* Inflate into out[cap]; returns bytes produced or -1. Stops quietly when out is full. */
static long inflate_buf(const u8* p, size_t n, u8* out, size_t cap) {
    inf_s s; int last, rc;
    memset(&s, 0, sizeof s);
    s.p = p; s.n = n; s.out = out; s.ocap = cap;
    if (n >= 2 && (p[0] & 0x0f) == 8 && ((p[0] << 8) | p[1]) % 31 == 0) s.pos = 2;   /* zlib header */
    do {
        int type;
        last = inf_bits(&s, 1);
        type = inf_bits(&s, 2);
        if (s.err) break;
        if (type == 0) {
            u32 len;
            s.bitbuf = 0; s.bitcnt = 0;
            if (s.pos + 4 > s.n) { s.err = 1; break; }
            len = (u32)s.p[s.pos] | ((u32)s.p[s.pos + 1] << 8);
            s.pos += 4;
            if (s.pos + len > s.n) { s.err = 1; break; }
            while (len--) { if (inf_put(&s, s.p[s.pos++])) return (long)s.olen; }
        } else if (type == 1) {
            static inf_h lc, dc; static int ready = 0;
            if (!ready) {
                short l[288]; int i;
                for (i = 0; i < 144; i++) l[i] = 8;
                for (; i < 256; i++) l[i] = 9;
                for (; i < 280; i++) l[i] = 7;
                for (; i < 288; i++) l[i] = 8;
                inf_build(&lc, l, 288);
                for (i = 0; i < 30; i++) l[i] = 5;
                inf_build(&dc, l, 30);
                ready = 1;
            }
            rc = inf_codes(&s, &lc, &dc);
            if (rc == 1) return (long)s.olen;
            if (rc < 0) { s.err = 1; break; }
        } else if (type == 2) {
            static const short ord[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
            short lengths[320]; inf_h lc, dc; int nlen, ndist, ncode, i;
            nlen = inf_bits(&s, 5) + 257; ndist = inf_bits(&s, 5) + 1; ncode = inf_bits(&s, 4) + 4;
            if (s.err || nlen > 286 || ndist > 30) { s.err = 1; break; }
            for (i = 0; i < 19; i++) lengths[ord[i]] = (short)(i < ncode ? inf_bits(&s, 3) : 0);
            if (inf_build(&lc, lengths, 19) != 0) { s.err = 1; break; }
            i = 0;
            while (i < nlen + ndist) {
                int sym = inf_decode(&s, &lc), len = 0, rep;
                if (sym < 0) { s.err = 1; break; }
                if (sym < 16) { lengths[i++] = (short)sym; continue; }
                if (sym == 16) { if (i == 0) { s.err = 1; break; } len = lengths[i - 1]; rep = 3 + inf_bits(&s, 2); }
                else if (sym == 17) rep = 3 + inf_bits(&s, 3);
                else rep = 11 + inf_bits(&s, 7);
                if (i + rep > nlen + ndist) { s.err = 1; break; }
                while (rep--) lengths[i++] = (short)len;
            }
            if (s.err) break;
            if (lengths[256] == 0) { s.err = 1; break; }
            rc = inf_build(&lc, lengths, nlen);
            if (rc < 0 || (rc > 0 && nlen - lc.count[0] != 1)) { s.err = 1; break; }
            rc = inf_build(&dc, lengths + nlen, ndist);
            if (rc < 0 || (rc > 0 && ndist - dc.count[0] != 1)) { s.err = 1; break; }
            rc = inf_codes(&s, &lc, &dc);
            if (rc == 1) return (long)s.olen;
            if (rc < 0) { s.err = 1; break; }
        } else { s.err = 1; break; }
    } while (!last);
    if (s.err && s.olen == 0) return -1;
    return (long)s.olen;      /* tolerate truncated streams: keep what decoded */
}

/* ═════════════════════════════ LZW / PackBits ════════════════════════════ */
static long lzw_decode(const u8* p, size_t n, u8* out, size_t cap) {
    u16 prefix[4096]; u8 suffix[4096], first[4096]; u16 length[4096];
    u8 stack[4096];
    size_t olen = 0, pos = 0; u32 acc = 0; int nb = 0, width = 9, next = 258, old = -1, i;
    if (n >= 2 && p[0] == 0 && (p[1] & 1)) return -2;     /* old-style (LSB-first) LZW */
    for (i = 0; i < 256; i++) { prefix[i] = 0; suffix[i] = (u8)i; first[i] = (u8)i; length[i] = 1; }
    for (;;) {
        int code, k, len;
        while (nb < width) { if (pos >= n) return (long)olen; acc = (acc << 8) | p[pos++]; nb += 8; }
        code = (int)((acc >> (nb - width)) & ((1u << width) - 1)); nb -= width;
        if (code == 257) break;
        if (code == 256) { width = 9; next = 258; old = -1; continue; }
        if (old < 0) {
            if (code > 255) return -1;
            if (olen < cap) out[olen++] = (u8)code; else return (long)olen;
            old = code; continue;
        }
        if (code < next) {
            len = length[code];
            for (k = len - 1, i = code; k >= 0; k--) { stack[k] = suffix[i]; i = prefix[i]; }
            if (next < 4096) { prefix[next] = (u16)old; suffix[next] = first[code]; first[next] = first[old]; length[next] = (u16)(length[old] + 1); next++; }
        } else if (code == next && next < 4096) {
            prefix[next] = (u16)old; suffix[next] = first[old]; first[next] = first[old]; length[next] = (u16)(length[old] + 1); next++;
            len = length[code];
            for (k = len - 1, i = code; k >= 0; k--) { stack[k] = suffix[i]; i = prefix[i]; }
        } else return olen ? (long)olen : -1;
        for (k = 0; k < len; k++) { if (olen >= cap) return (long)olen; out[olen++] = stack[k]; }
        old = code;
        if (next + 1 >= (1 << width) && width < 12) width++;
    }
    return (long)olen;
}
static long packbits_decode(const u8* p, size_t n, u8* out, size_t cap) {
    size_t pos = 0, o = 0;
    while (pos < n && o < cap) {
        int c = (int8_t)p[pos++];
        if (c >= 0) { int k = c + 1; while (k-- > 0 && pos < n && o < cap) out[o++] = p[pos++]; }
        else if (c != -128) { int k = 1 - c; u8 v; if (pos >= n) break; v = p[pos++]; while (k-- > 0 && o < cap) out[o++] = v; }
    }
    return (long)o;
}

/* ═════════════════════════════ IFD parsing ═══════════════════════════════ */
typedef struct { const u8* d; size_t n; int be; } tfile;
static u32 t16(const tfile* f, size_t o) { if (o + 2 > f->n) return 0; return f->be ? (u32)((f->d[o] << 8) | f->d[o + 1]) : (u32)(f->d[o] | (f->d[o + 1] << 8)); }
static u32 t32(const tfile* f, size_t o) {
    if (o + 4 > f->n) return 0;
    return f->be ? ((u32)f->d[o] << 24) | ((u32)f->d[o + 1] << 16) | ((u32)f->d[o + 2] << 8) | f->d[o + 3]
                 : (u32)f->d[o] | ((u32)f->d[o + 1] << 8) | ((u32)f->d[o + 2] << 16) | ((u32)f->d[o + 3] << 24);
}
typedef struct { u32 tag, type, count, voff; } tentry;
static int tsize(u32 type) { switch (type) { case 1: case 2: case 6: case 7: return 1; case 3: case 8: return 2; case 4: case 9: case 11: case 13: return 4; case 5: case 10: case 12: return 8; default: return 0; } }
/* value i of an entry (integer types only) */
static u32 tval(const tfile* f, const tentry* e, u32 i) {
    int sz = tsize(e->type); size_t base;
    if (i >= e->count || sz == 0) return 0;
    base = ((u64)sz * e->count <= 4) ? e->voff : t32(f, e->voff);
    if ((u64)sz * e->count <= 4) {
        /* inline: the value bytes sit in the 4-byte field itself */
        size_t o = e->voff + (size_t)sz * i;
        return sz == 1 ? f->d[o] : sz == 2 ? t16(f, o) : t32(f, o);
    }
    base += (size_t)sz * i;
    if (base + (size_t)sz > f->n) return 0;
    return sz == 1 ? f->d[base] : sz == 2 ? t16(f, base) : t32(f, base);
}

typedef struct {
    tentry e[64]; int ne;
} tifd;
static const tentry* tfind(const tifd* d, u32 tag) { int i; for (i = 0; i < d->ne; i++) if (d->e[i].tag == tag) return &d->e[i]; return NULL; }
static u32 tget(const tfile* f, const tifd* d, u32 tag, u32 dflt) { const tentry* e = tfind(d, tag); return e && e->count ? tval(f, e, 0) : dflt; }

static int read_ifd(const tfile* f, u32 off, tifd* d, u32* next) {
    u32 cnt, i;
    if ((size_t)off + 2 > f->n) return -1;
    cnt = t16(f, off);
    if ((size_t)off + 2 + (size_t)cnt * 12 + 4 > f->n) return -1;
    d->ne = 0;
    for (i = 0; i < cnt; i++) {
        size_t o = off + 2 + (size_t)i * 12;
        tentry t;
        t.tag = t16(f, o); t.type = t16(f, o + 2); t.count = t32(f, o + 4); t.voff = (u32)(o + 8);
        if (d->ne < 64) d->e[d->ne++] = t;
    }
    *next = t32(f, off + 2 + (size_t)cnt * 12);
    return 0;
}

static u32 rd_sample(const u8* row, size_t idx, int bits, int be) {
    switch (bits) {
    case 8: return row[idx];
    case 16: { const u8* p = row + idx * 2; return be ? (u32)((p[0] << 8) | p[1]) : (u32)(p[0] | (p[1] << 8)); }
    default: {
        size_t bit = idx * (size_t)bits; u32 byte = row[bit >> 3];
        return (byte >> (8 - bits - (int)(bit & 7))) & ((1u << bits) - 1);
    }
    }
}
static u8 to8(u32 v, int bits) {
    if (bits == 8) return (u8)v;
    if (bits == 16) return (u8)(v >> 8);
    return (u8)((v * 255 + ((1u << bits) - 1) / 2) / ((1u << bits) - 1));
}

static void undo_predictor(u8* buf, int rows, size_t rowbytes, int width, int spp, int bits, int be) {
    int r, x, s;
    for (r = 0; r < rows; r++) {
        u8* row = buf + (size_t)r * rowbytes;
        if (bits == 8) {
            for (x = spp; x < width * spp; x++) row[x] = (u8)(row[x] + row[x - spp]);
        } else if (bits == 16) {
            for (x = 1; x < width; x++)
                for (s = 0; s < spp; s++) {
                    u8* p = row + ((size_t)x * spp + s) * 2; u8* q = p - (size_t)spp * 2;
                    u32 a = be ? (u32)((p[0] << 8) | p[1]) : (u32)(p[0] | (p[1] << 8));
                    u32 b = be ? (u32)((q[0] << 8) | q[1]) : (u32)(q[0] | (q[1] << 8));
                    u32 v = (a + b) & 0xffff;
                    if (be) { p[0] = (u8)(v >> 8); p[1] = (u8)v; } else { p[0] = (u8)v; p[1] = (u8)(v >> 8); }
                }
        }
    }
}

static long decompress(int comp, const u8* src, size_t sn, u8* dst, size_t cap) {
    switch (comp) {
    case 1: { size_t k = sn < cap ? sn : cap; memcpy(dst, src, k); return (long)k; }
    case 5: return lzw_decode(src, sn, dst, cap);
    case 8: case 32946: return inflate_buf(src, sn, dst, cap);
    case 32773: return packbits_decode(src, sn, dst, cap);
    default: return -3;
    }
}

void* trc_tiff_decode(const char* data, int64_t n, int64_t page) {
    tf_result* r = (tf_result*)calloc(1, sizeof(tf_result));
    tfile f; tifd ifd, cur; u32 off, next, w, h, spp, bits, comp, photo, planar, pred, extra = 0, sfmt;
    int np = 0, i, tiled;
    u32 chunk_w, chunk_h, across, down, nchunks, planes;
    const tentry *eoff, *ecnt, *ebps, *ecmap;
    size_t rowbytes, chunkbytes;
    u8* chunk = NULL; u8* planebuf[5] = { 0, 0, 0, 0, 0 };
    size_t plane_rowbytes;
    if (!r) return NULL;
    f.d = (const u8*)data; f.n = (size_t)(n < 0 ? 0 : n);
    if (f.n < 8) { tf_fail(r, "file too short"); return r; }
    if (f.d[0] == 'I' && f.d[1] == 'I') f.be = 0;
    else if (f.d[0] == 'M' && f.d[1] == 'M') f.be = 1;
    else { tf_fail(r, "not a TIFF file"); return r; }
    if (t16(&f, 2) == 43) { tf_fail(r, "BigTIFF is not supported"); return r; }
    if (t16(&f, 2) != 42) { tf_fail(r, "bad TIFF magic"); return r; }
    off = t32(&f, 4);
    memset(&ifd, 0, sizeof ifd);
    /* count pages, remember the requested one */
    while (off && np < 65536) {
        if (read_ifd(&f, off, &cur, &next) != 0) break;
        if (np == page) ifd = cur;
        np++;
        if (next == off) break;
        off = next;
    }
    r->npages = np;
    if (np == 0) { tf_fail(r, "no image directory"); return r; }
    if (page < 0 || page >= np) { tf_fail(r, "page index out of range"); return r; }
    w = tget(&f, &ifd, 256, 0); h = tget(&f, &ifd, 257, 0);
    spp = tget(&f, &ifd, 277, 1); bits = tget(&f, &ifd, 258, 1);
    comp = tget(&f, &ifd, 259, 1); photo = tget(&f, &ifd, 262, 1);
    planar = tget(&f, &ifd, 284, 1); pred = tget(&f, &ifd, 317, 1);
    sfmt = tget(&f, &ifd, 339, 1);
    if (tfind(&ifd, 338)) extra = tget(&f, &ifd, 338, 0); else extra = (u32)-1;
    r->w = (int)w; r->h = (int)h; r->bits = (int)bits; r->spp = (int)spp; r->photometric = (int)photo; r->compression = (int)comp;
    if (w == 0 || h == 0 || (size_t)w * h > TF_MAX_PIXELS) { tf_fail(r, "bad image dimensions"); return r; }
    if (spp < 1 || spp > 5) { tf_fail(r, "unsupported samples per pixel"); return r; }
    ebps = tfind(&ifd, 258);
    for (i = 1; ebps && i < (int)spp && i < (int)ebps->count; i++) if (tval(&f, ebps, (u32)i) != bits) { tf_fail(r, "mixed bit depths are not supported"); return r; }
    if (!(bits == 1 || bits == 2 || bits == 4 || bits == 8 || bits == 16)) { tf_fail(r, "unsupported bit depth"); return r; }
    if (sfmt != 1 && sfmt != 2) { tf_fail(r, "floating-point samples are not supported"); return r; }
    if (pred != 1 && pred != 2) { tf_fail(r, "unsupported predictor"); return r; }
    if (photo > 3 && photo != 5) { tf_fail(r, "unsupported photometric interpretation"); return r; }
    if (photo == 2 && spp < 3) { tf_fail(r, "RGB image with fewer than 3 samples"); return r; }
    if (photo == 5 && (spp < 4 || bits != 8)) { tf_fail(r, "only 8-bit CMYK is supported"); return r; }
    if (photo == 3 && spp != 1) { tf_fail(r, "palette image with several samples"); return r; }
    if (comp != 1 && comp != 5 && comp != 8 && comp != 32946 && comp != 32773) { tf_fail(r, "unsupported compression"); return r; }
    tiled = tfind(&ifd, 322) != NULL;
    if (tiled) {
        chunk_w = tget(&f, &ifd, 322, 0); chunk_h = tget(&f, &ifd, 323, 0);
        eoff = tfind(&ifd, 324); ecnt = tfind(&ifd, 325);
        if (!chunk_w || !chunk_h || chunk_w > 65536 || chunk_h > 65536) { tf_fail(r, "bad tile size"); return r; }
    } else {
        chunk_w = w; chunk_h = tget(&f, &ifd, 278, h);
        if (chunk_h == 0 || chunk_h > h) chunk_h = h;
        eoff = tfind(&ifd, 273); ecnt = tfind(&ifd, 279);
    }
    if (!eoff) { tf_fail(r, "missing strip/tile offsets"); return r; }
    across = (w + chunk_w - 1) / chunk_w; down = (h + chunk_h - 1) / chunk_h;
    planes = planar == 2 ? spp : 1;
    nchunks = across * down * planes;
    if (eoff->count < nchunks) { tf_fail(r, "too few strip/tile offsets"); return r; }
    {
        u32 cs = planar == 2 ? 1 : spp;
        rowbytes = ((size_t)chunk_w * cs * bits + 7) / 8;
        chunkbytes = rowbytes * chunk_h;
        plane_rowbytes = ((size_t)w * cs * bits + 7) / 8;
    }
    if (chunkbytes > ((size_t)1 << 30)) { tf_fail(r, "strip too large"); return r; }
    chunk = (u8*)malloc(chunkbytes ? chunkbytes : 1);
    for (i = 0; i < (int)planes; i++) {
        planebuf[i] = (u8*)calloc(plane_rowbytes * h, 1);
        if (!planebuf[i]) { tf_fail(r, "out of memory"); goto done; }
    }
    if (!chunk) { tf_fail(r, "out of memory"); goto done; }
    {
        u32 c;
        for (c = 0; c < nchunks; c++) {
            u32 pl = c / (across * down), idx = c % (across * down), cx = idx % across, cy = idx / across;
            u32 o = tval(&f, eoff, c), cnt = ecnt ? tval(&f, ecnt, c) : (u32)(f.n > o ? f.n - o : 0);
            long got; u32 rows, y;
            if (o >= f.n) { tf_fail(r, "strip offset beyond end of file"); goto done; }
            if (cnt > f.n - o) cnt = (u32)(f.n - o);
            memset(chunk, 0, chunkbytes);
            got = decompress((int)comp, f.d + o, cnt, chunk, chunkbytes);
            if (got == -2) { tf_fail(r, "old-style LZW is not supported"); goto done; }
            if (got < 0) { tf_fail(r, "corrupt compressed data"); goto done; }
            rows = chunk_h;
            if (pred == 2) undo_predictor(chunk, (int)rows, rowbytes, (int)chunk_w, planar == 2 ? 1 : (int)spp, (int)bits, f.be);
            /* copy chunk rows into the plane (bit-aligned copy via sample re-pack for tiles) */
            for (y = 0; y < rows; y++) {
                u32 py = cy * chunk_h + y;
                if (py >= h) break;
                if (!tiled) memcpy(planebuf[pl] + (size_t)py * plane_rowbytes, chunk + (size_t)y * rowbytes, plane_rowbytes);
                else {
                    u32 cs = planar == 2 ? 1 : spp, x0 = cx * chunk_w, nx = chunk_w;
                    if (x0 + nx > w) nx = w - x0;
                    if (bits >= 8) memcpy(planebuf[pl] + (size_t)py * plane_rowbytes + (size_t)x0 * cs * (bits / 8),
                                          chunk + (size_t)y * rowbytes, (size_t)nx * cs * (bits / 8));
                    else {
                        u32 s;
                        for (s = 0; s < nx * cs; s++) {
                            u32 v = rd_sample(chunk + (size_t)y * rowbytes, s, (int)bits, 0);
                            size_t bit = ((size_t)x0 * cs + s) * bits;
                            u8* dst = planebuf[pl] + (size_t)py * plane_rowbytes + (bit >> 3);
                            *dst = (u8)(*dst | (v << (8 - bits - (bit & 7))));
                        }
                    }
                }
            }
        }
    }
    r->rgba = (u8*)malloc((size_t)w * h * 4);
    if (!r->rgba) { tf_fail(r, "out of memory"); goto done; }
    ecmap = tfind(&ifd, 320);
    if (photo == 3 && (!ecmap || ecmap->count < 3u * (1u << bits))) { tf_fail(r, "missing colour map"); free(r->rgba); r->rgba = NULL; goto done; }
    {
        u32 x, y;
        int has_alpha = (extra == 1 || extra == 2) || (extra == (u32)-1 && ((photo <= 1 && spp == 2) || (photo == 2 && spp == 4)));
        int assoc = extra == 1;
        u32 color_n = photo == 2 ? 3 : photo == 5 ? 4 : 1;
        if (extra == 0) has_alpha = 0;
        if (spp <= color_n) has_alpha = 0;
        for (y = 0; y < h; y++)
            for (x = 0; x < w; x++) {
                u32 s[5] = { 0, 0, 0, 0, 0 }, k; u8* o = r->rgba + ((size_t)y * w + x) * 4;
                for (k = 0; k < spp; k++) {
                    if (planar == 2) s[k] = rd_sample(planebuf[k] + (size_t)y * plane_rowbytes, x, (int)bits, f.be);
                    else s[k] = rd_sample(planebuf[0] + (size_t)y * plane_rowbytes, (size_t)x * spp + k, (int)bits, f.be);
                }
                o[3] = has_alpha ? to8(s[color_n], (int)bits) : 255;
                if (photo == 3) {
                    u32 nc = 1u << bits;
                    o[0] = (u8)(tval(&f, ecmap, s[0]) >> 8); o[1] = (u8)(tval(&f, ecmap, nc + s[0]) >> 8); o[2] = (u8)(tval(&f, ecmap, 2 * nc + s[0]) >> 8);
                } else if (photo == 2) {
                    o[0] = to8(s[0], (int)bits); o[1] = to8(s[1], (int)bits); o[2] = to8(s[2], (int)bits);
                } else if (photo == 5) {
                    int kk = 255 - (int)s[3];
                    o[0] = (u8)(((255 - (int)s[0]) * kk + 127) / 255);
                    o[1] = (u8)(((255 - (int)s[1]) * kk + 127) / 255);
                    o[2] = (u8)(((255 - (int)s[2]) * kk + 127) / 255);
                } else {
                    u8 g = to8(s[0], (int)bits);
                    if (photo == 0) g = (u8)(255 - g);
                    o[0] = o[1] = o[2] = g;
                }
                if (has_alpha && assoc && o[3] != 255) {
                    int a = o[3], c;
                    for (c = 0; c < 3; c++) o[c] = a == 0 ? 0 : (u8)((o[c] * 255 + a / 2) / a > 255 ? 255 : (o[c] * 255 + a / 2) / a);
                }
            }
    }
done:
    free(chunk);
    for (i = 0; i < 5; i++) free(planebuf[i]);
    return r;
}

/* ═════════════════════════════ encoder ═══════════════════════════════════ */
typedef struct { u8* buf; size_t len, cap; int err; } obuf;
static void ob_need(obuf* b, size_t n) {
    if (b->err || b->len + n <= b->cap) return;
    { size_t nc = b->cap ? b->cap : 4096; u8* nb; while (nc < b->len + n) nc *= 2; nb = (u8*)realloc(b->buf, nc); if (!nb) { b->err = 1; return; } b->buf = nb; b->cap = nc; }
}
static void ob_put(obuf* b, const void* p, size_t n) { ob_need(b, n); if (b->err) return; memcpy(b->buf + b->len, p, n); b->len += n; }
static void ob_u8(obuf* b, u8 v) { ob_put(b, &v, 1); }
static void ob_u16(obuf* b, u32 v) { u8 t[2]; t[0] = (u8)v; t[1] = (u8)(v >> 8); ob_put(b, t, 2); }
static void ob_u32(obuf* b, u32 v) { u8 t[4]; t[0] = (u8)v; t[1] = (u8)(v >> 8); t[2] = (u8)(v >> 16); t[3] = (u8)(v >> 24); ob_put(b, t, 4); }
static void ob_set32(obuf* b, size_t at, u32 v) { if (b->err || at + 4 > b->len) return; b->buf[at] = (u8)v; b->buf[at + 1] = (u8)(v >> 8); b->buf[at + 2] = (u8)(v >> 16); b->buf[at + 3] = (u8)(v >> 24); }

/* TIFF LZW (MSB-first, early change) */
typedef struct { obuf* o; u32 acc; int nb; } lzbits;
static void lz_put(lzbits* b, u32 code, int width) {
    b->acc = (b->acc << width) | code; b->nb += width;
    while (b->nb >= 8) { ob_u8(b->o, (u8)(b->acc >> (b->nb - 8))); b->nb -= 8; }
    b->acc &= (1u << b->nb) - 1;
}
static int lz_width(int next) { int w = 9; while (w < 12 && next >= (1 << w)) w++; return w; }
static void lzw_encode(obuf* o, const u8* p, size_t n) {
    lzbits b; int next = 258, cur = -1; size_t i;
    int* tab = (int*)malloc(sizeof(int) * 8192);       /* hash: key -> code */
    u32* keys = (u32*)malloc(sizeof(u32) * 8192);
    b.o = o; b.acc = 0; b.nb = 0;
    if (!tab || !keys) { free(tab); free(keys); o->err = 1; return; }
    memset(tab, 0xff, sizeof(int) * 8192);
    lz_put(&b, 256, 9);
    for (i = 0; i < n; i++) {
        u8 c = p[i];
        if (cur < 0) { cur = c; continue; }
        {
            u32 key = ((u32)cur << 8) | c, hsh = (key * 2654435761u) >> 19;
            while (tab[hsh] >= 0 && keys[hsh] != key) hsh = (hsh + 1) & 8191;
            if (tab[hsh] >= 0) { cur = tab[hsh]; continue; }
            lz_put(&b, (u32)cur, lz_width(next));
            tab[hsh] = next; keys[hsh] = key; next++;
            if (next >= 4094) {
                lz_put(&b, 256, lz_width(next));
                next = 258;
                memset(tab, 0xff, sizeof(int) * 8192);
            }
            cur = c;
        }
    }
    if (cur >= 0) { lz_put(&b, (u32)cur, lz_width(next)); next++; }
    lz_put(&b, 257, lz_width(next));
    if (b.nb > 0) ob_u8(o, (u8)(b.acc << (8 - b.nb)));
    free(tab); free(keys);
}

typedef struct { obuf o; size_t last_next_ptr; int npages; } tf_enc;

void* trc_tiff_enc_begin(void) {
    tf_enc* e = (tf_enc*)calloc(1, sizeof(tf_enc));
    if (!e) return NULL;
    ob_put(&e->o, "II", 2); ob_u16(&e->o, 42);
    e->last_next_ptr = e->o.len; ob_u32(&e->o, 0);
    return e;
}
/* compression: 1 = none, 5 = LZW (+ horizontal predictor) */
int64_t trc_tiff_enc_add(void* hh, const char* rgba_, int64_t w, int64_t h, int64_t compression) {
    tf_enc* e = (tf_enc*)hh; const u8* rgba = (const u8*)rgba_;
    int alpha = 0, spp, lzw = compression == 5;
    size_t i, np, rowbytes; u32 rps, nstrips, s;
    size_t* soff; size_t* scnt; u8* strip;
    if (!e || w < 1 || h < 1 || (size_t)w * h > TF_MAX_PIXELS) return -1;
    np = (size_t)w * h;
    for (i = 0; i < np; i++) if (rgba[i * 4 + 3] != 255) { alpha = 1; break; }
    spp = alpha ? 4 : 3;
    rowbytes = (size_t)w * spp;
    rps = (u32)(8192 / rowbytes); if (rps < 1) rps = 1; if (rps > (u32)h) rps = (u32)h;
    nstrips = ((u32)h + rps - 1) / rps;
    soff = (size_t*)malloc(sizeof(size_t) * nstrips); scnt = (size_t*)malloc(sizeof(size_t) * nstrips);
    strip = (u8*)malloc(rowbytes * rps);
    if (!soff || !scnt || !strip) { free(soff); free(scnt); free(strip); return -1; }
    for (s = 0; s < nstrips; s++) {
        u32 y0 = s * rps, rows = (u32)h - y0 < rps ? (u32)h - y0 : rps, y; size_t x;
        for (y = 0; y < rows; y++) {
            const u8* src = rgba + ((size_t)(y0 + y) * w) * 4; u8* dst = strip + (size_t)y * rowbytes;
            for (x = 0; x < (size_t)w; x++) { int c; for (c = 0; c < spp; c++) dst[x * spp + c] = src[x * 4 + c]; }
            if (lzw) for (x = rowbytes - 1; x >= (size_t)spp; x--) dst[x] = (u8)(dst[x] - dst[x - spp]);
        }
        if (e->o.len & 1) ob_u8(&e->o, 0);
        soff[s] = e->o.len;
        if (lzw) lzw_encode(&e->o, strip, rowbytes * rows);
        else ob_put(&e->o, strip, rowbytes * rows);
        scnt[s] = e->o.len - soff[s];
    }
    free(strip);
    {
        /* out-of-line data: bits per sample, strip offsets, strip counts */
        size_t bps_at, off_at = 0, cnt_at = 0, ifd_at; int ntags = 12 + (lzw ? 1 : 0) + (alpha ? 1 : 0), k;
        if (e->o.len & 1) ob_u8(&e->o, 0);
        bps_at = e->o.len; for (k = 0; k < spp; k++) ob_u16(&e->o, 8);
        if (nstrips > 1) {
            off_at = e->o.len; for (s = 0; s < nstrips; s++) ob_u32(&e->o, (u32)soff[s]);
            cnt_at = e->o.len; for (s = 0; s < nstrips; s++) ob_u32(&e->o, (u32)scnt[s]);
        }
        {
            size_t res_at = e->o.len;
            ob_u32(&e->o, 72); ob_u32(&e->o, 1);           /* 72 dpi rational */
            if (e->o.len & 1) ob_u8(&e->o, 0);
            ifd_at = e->o.len;
            ob_set32(&e->o, e->last_next_ptr, (u32)ifd_at);
            ob_u16(&e->o, (u32)ntags);
#define TAG(t, ty, c, v) do { ob_u16(&e->o, t); ob_u16(&e->o, ty); ob_u32(&e->o, c); ob_u32(&e->o, v); } while (0)
#define TAG16(t, v) do { ob_u16(&e->o, t); ob_u16(&e->o, 3); ob_u32(&e->o, 1); ob_u16(&e->o, v); ob_u16(&e->o, 0); } while (0)
            TAG(256, 4, 1, (u32)w);
            TAG(257, 4, 1, (u32)h);
            if (spp <= 2) TAG16(258, 8); else TAG(258, 3, (u32)spp, (u32)bps_at);
            TAG16(259, lzw ? 5 : 1);
            TAG16(262, 2);
            TAG(273, 4, nstrips, nstrips > 1 ? (u32)off_at : (u32)soff[0]);
            TAG16(277, (u32)spp);
            TAG(278, 4, 1, rps);
            TAG(279, 4, nstrips, nstrips > 1 ? (u32)cnt_at : (u32)scnt[0]);
            TAG(282, 5, 1, (u32)res_at);
            TAG(283, 5, 1, (u32)res_at);
            TAG16(284, 1);
            if (lzw) TAG16(317, 2);
            if (alpha) TAG16(338, 2);
#undef TAG
#undef TAG16

            e->last_next_ptr = e->o.len; ob_u32(&e->o, 0);
        }
    }
    free(soff); free(scnt);
    e->npages++;
    return e->o.err ? -1 : 0;
}
char* trc_tiff_enc_finish(void* hh, int64_t* out_len) {
    tf_enc* e = (tf_enc*)hh; char* out;
    *out_len = 0;
    if (!e) return NULL;
    if (e->o.err || e->npages == 0) { free(e->o.buf); free(e); return NULL; }
    out = (char*)e->o.buf; *out_len = (int64_t)e->o.len;
    free(e);
    return out;
}
