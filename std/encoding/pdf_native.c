/*
 * std/encoding/pdf_native.c -- helpers for std.encoding.pdf:
 *   - zlib stream compression (LZ77 with hash chains, fixed or dynamic
 *     Huffman blocks, whichever is smaller),
 *     so FlateDecode works without linking libz;
 *   - JPEG header inspection for DCTDecode pass-through;
 *   - RGBA -> RGB + alpha plane split for image XObjects with an SMask.
 *
 * Linked through `# tauraro-cpp-shim: pdf_native.c` in std/encoding/pdf.tr.
 * Output buffers are allocated by the Tauraro side (sized with
 * trc_pdf_deflate_bound) and filled here.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* ── bit writer (LSB first, as DEFLATE requires) ─────────────────────────── */
typedef struct { unsigned char* out; int64_t pos; uint32_t acc; int nbits; } pd_bits;

static void pd_put(pd_bits* b, uint32_t v, int n) {
    b->acc |= v << b->nbits;
    b->nbits += n;
    while (b->nbits >= 8) { b->out[b->pos++] = (unsigned char)(b->acc & 255); b->acc >>= 8; b->nbits -= 8; }
}
static void pd_flush(pd_bits* b) {
    if (b->nbits > 0) { b->out[b->pos++] = (unsigned char)(b->acc & 255); }
    b->acc = 0; b->nbits = 0;
}
/* Huffman codes are defined MSB first; reverse them for the LSB-first stream. */
static uint32_t pd_rev(uint32_t code, int len) {
    uint32_t r = 0; int i;
    for (i = 0; i < len; i++) { r = (r << 1) | (code & 1); code >>= 1; }
    return r;
}
static void pd_sym(pd_bits* b, int sym) {
    if (sym < 144)      pd_put(b, pd_rev(0x30 + sym, 8), 8);
    else if (sym < 256) pd_put(b, pd_rev(0x190 + (sym - 144), 9), 9);
    else if (sym < 280) pd_put(b, pd_rev(sym - 256, 7), 7);
    else                pd_put(b, pd_rev(0xC0 + (sym - 280), 8), 8);
}

static const int pd_lbase[29] = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
static const int pd_lext[29]  = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
static const int pd_dbase[30] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
static const int pd_dext[30]  = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

static void pd_match(pd_bits* b, int len, int dist) {
    int li = 28, di = 29;
    while (pd_lbase[li] > len) li--;
    pd_sym(b, 257 + li);
    if (pd_lext[li]) pd_put(b, (uint32_t)(len - pd_lbase[li]), pd_lext[li]);
    while (pd_dbase[di] > dist) di--;
    pd_put(b, pd_rev((uint32_t)di, 5), 5);
    if (pd_dext[di]) pd_put(b, (uint32_t)(dist - pd_dbase[di]), pd_dext[di]);
}

#define PD_WIN   32768
#define PD_HBITS 15
#define PD_HSIZE (1 << PD_HBITS)
#define PD_CHAIN 48
#define PD_BLOCK 65536          /* tokens per block */

static uint32_t pd_hash(const unsigned char* p) {
    return ((uint32_t)p[0] * 2654435761u ^ (uint32_t)p[1] * 40503u ^ (uint32_t)p[2]) & (PD_HSIZE - 1);
}

/* A token is a literal (dist == 0) or a match (len 3..258, dist 1..32768). */
typedef struct { uint16_t len; uint16_t dist; } pd_tok;

static int pd_lcode(int len) { int li = 28; while (pd_lbase[li] > len) li--; return li; }
static int pd_dcode(int dist) { int di = 29; while (pd_dbase[di] > dist) di--; return di; }

/* Code lengths for freq[0..n) limited to `limit` bits (Huffman, then the
 * JPEG Annex K.3 length adjustment). Unused symbols get length 0. */
static void pd_lengths(const uint32_t* freq, int n, int limit, uint8_t* lens) {
    int sym[288], nsym = 0, i, j;
    uint64_t w[576]; int parent[576], alive[576], nodes;
    int depth_count[64]; int bits[64];
    memset(lens, 0, (size_t)n);
    for (i = 0; i < n; i++) if (freq[i]) sym[nsym++] = i;
    if (nsym == 0) return;
    if (nsym == 1) { lens[sym[0]] = 1; return; }
    for (i = 0; i < nsym; i++) { w[i] = freq[sym[i]]; parent[i] = -1; alive[i] = 1; }
    nodes = nsym;
    for (;;) {
        int a = -1, b2 = -1;
        for (i = 0; i < nodes; i++) {
            if (!alive[i]) continue;
            if (a < 0 || w[i] < w[a]) { b2 = a; a = i; }
            else if (b2 < 0 || w[i] < w[b2]) b2 = i;
        }
        if (b2 < 0) break;
        w[nodes] = w[a] + w[b2]; parent[nodes] = -1; alive[nodes] = 1;
        parent[a] = nodes; parent[b2] = nodes; alive[a] = 0; alive[b2] = 0;
        nodes++;
    }
    memset(depth_count, 0, sizeof depth_count);
    for (i = 0; i < nsym; i++) {
        int d = 0, x = i;
        while (parent[x] >= 0) { x = parent[x]; d++; }
        if (d > 63) d = 63;
        depth_count[d]++;
    }
    memcpy(bits, depth_count, sizeof bits);
    for (i = 63; i > limit; i--) {
        while (bits[i] > 0) {
            j = i - 2;
            while (j > 0 && bits[j] == 0) j--;
            bits[i] -= 2; bits[i - 1] += 1; bits[j + 1] += 2; bits[j] -= 1;
        }
    }
    /* most frequent symbols get the shortest codes */
    for (i = 1; i < nsym; i++) {
        int s2 = sym[i]; j = i - 1;
        while (j >= 0 && (freq[sym[j]] < freq[s2] || (freq[sym[j]] == freq[s2] && sym[j] > s2))) { sym[j + 1] = sym[j]; j--; }
        sym[j + 1] = s2;
    }
    j = 0;
    for (i = 1; i <= limit; i++) { int c = bits[i]; while (c-- > 0 && j < nsym) lens[sym[j++]] = (uint8_t)i; }
}

static void pd_codes(const uint8_t* lens, int n, uint16_t* codes) {
    int bl[16] = {0}, next[16], i, code = 0;
    for (i = 0; i < n; i++) bl[lens[i]]++;
    bl[0] = 0;
    for (i = 1; i < 16; i++) { code = (code + bl[i - 1]) << 1; next[i] = code; }
    for (i = 0; i < n; i++) if (lens[i]) codes[i] = (uint16_t)pd_rev((uint32_t)next[lens[i]]++, lens[i]);
}

static const int pd_clorder[19] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};

/* Run-length encode the code lengths (symbols 0-18 with extra-bit payloads). */
static int pd_rle(const uint8_t* all, int total, int* rs, int* rx) {
    int i = 0, m = 0;
    while (i < total) {
        int v = all[i], run = 1;
        while (i + run < total && all[i + run] == v) run++;
        if (v == 0 && run >= 3) {
            int r = run; while (r >= 3) { int t = r > 138 ? 138 : r; if (t <= 10) { rs[m] = 17; rx[m++] = t - 3; } else { rs[m] = 18; rx[m++] = t - 11; } r -= t; }
            i += run - r; continue;
        }
        if (v != 0 && run >= 4) {
            int r = run - 1; rs[m] = v; rx[m++] = 0;
            while (r >= 3) { int t = r > 6 ? 6 : r; rs[m] = 16; rx[m++] = t - 3; r -= t; }
            i += run - r; continue;
        }
        rs[m] = v; rx[m++] = 0; i++;
    }
    return m;
}

static void pd_block(pd_bits* b, const pd_tok* t, int nt, int final) {
    uint32_t lf[286], df[30], cf[19]; uint8_t ll[286], dl[30], cl[19], all[316];
    uint16_t lc[286], dc[30], cc[19];
    int rs[320], rx[320], nr, i, hlit, hdist, hclen;
    uint64_t fixed_bits = 3, dyn_bits = 3 + 5 + 5 + 4;
    memset(lf, 0, sizeof lf); memset(df, 0, sizeof df); memset(cf, 0, sizeof cf);
    memset(lc, 0, sizeof lc); memset(dc, 0, sizeof dc); memset(cc, 0, sizeof cc);
    for (i = 0; i < nt; i++) {
        if (t[i].dist == 0) lf[t[i].len]++;
        else { lf[257 + pd_lcode(t[i].len)]++; df[pd_dcode(t[i].dist)]++; }
    }
    lf[256] = 1;
    pd_lengths(lf, 286, 15, ll);
    pd_lengths(df, 30, 15, dl);
    { int used = 0; for (i = 0; i < 30; i++) if (dl[i]) used++;
      if (used == 0) { dl[0] = 1; dl[1] = 1; } else if (used == 1) { if (dl[0]) dl[1] = 1; else dl[0] = 1; } }
    { int used = 0; for (i = 0; i < 286; i++) if (ll[i]) used++;
      if (used == 1) { if (ll[0]) ll[1] = 1; else ll[0] = 1; } }
    for (hlit = 286; hlit > 257 && ll[hlit - 1] == 0; hlit--) {}
    for (hdist = 30; hdist > 1 && dl[hdist - 1] == 0; hdist--) {}
    memcpy(all, ll, (size_t)hlit); memcpy(all + hlit, dl, (size_t)hdist);
    nr = pd_rle(all, hlit + hdist, rs, rx);
    for (i = 0; i < nr; i++) cf[rs[i]]++;
    pd_lengths(cf, 19, 7, cl);
    { int used = 0; for (i = 0; i < 19; i++) if (cl[i]) used++;
      if (used == 1) { if (cl[0]) cl[1] = 1; else cl[0] = 1; } }
    for (hclen = 19; hclen > 4 && cl[pd_clorder[hclen - 1]] == 0; hclen--) {}
    /* cost of each encoding */
    for (i = 0; i < 286; i++) {
        int fl = i < 144 ? 8 : i < 256 ? 9 : i < 280 ? 7 : 8;
        int ex = i >= 257 ? pd_lext[i - 257] : 0;
        fixed_bits += (uint64_t)lf[i] * (uint64_t)(fl + ex);
        dyn_bits += (uint64_t)lf[i] * (uint64_t)(ll[i] + ex);
    }
    for (i = 0; i < 30; i++) { fixed_bits += (uint64_t)df[i] * (uint64_t)(5 + pd_dext[i]); dyn_bits += (uint64_t)df[i] * (uint64_t)(dl[i] + pd_dext[i]); }
    dyn_bits += 3u * (uint64_t)hclen;
    for (i = 0; i < nr; i++) dyn_bits += cl[rs[i]] + (rs[i] == 16 ? 2 : rs[i] == 17 ? 3 : rs[i] == 18 ? 7 : 0);
    pd_put(b, (uint32_t)final, 1);
    if (fixed_bits <= dyn_bits) {
        pd_put(b, 1, 2);
        for (i = 0; i < nt; i++) {
            if (t[i].dist == 0) pd_sym(b, t[i].len); else pd_match(b, t[i].len, t[i].dist);
        }
        pd_sym(b, 256);
        return;
    }
    pd_codes(ll, 286, lc); pd_codes(dl, 30, dc); pd_codes(cl, 19, cc);
    pd_put(b, 2, 2);
    pd_put(b, (uint32_t)(hlit - 257), 5); pd_put(b, (uint32_t)(hdist - 1), 5); pd_put(b, (uint32_t)(hclen - 4), 4);
    for (i = 0; i < hclen; i++) pd_put(b, cl[pd_clorder[i]], 3);
    for (i = 0; i < nr; i++) {
        pd_put(b, cc[rs[i]], cl[rs[i]]);
        if (rs[i] == 16) pd_put(b, (uint32_t)rx[i], 2);
        else if (rs[i] == 17) pd_put(b, (uint32_t)rx[i], 3);
        else if (rs[i] == 18) pd_put(b, (uint32_t)rx[i], 7);
    }
    for (i = 0; i < nt; i++) {
        if (t[i].dist == 0) pd_put(b, lc[t[i].len], ll[t[i].len]);
        else {
            int li = pd_lcode(t[i].len), di = pd_dcode(t[i].dist);
            pd_put(b, lc[257 + li], ll[257 + li]);
            if (pd_lext[li]) pd_put(b, (uint32_t)(t[i].len - pd_lbase[li]), pd_lext[li]);
            pd_put(b, dc[di], dl[di]);
            if (pd_dext[di]) pd_put(b, (uint32_t)(t[i].dist - pd_dbase[di]), pd_dext[di]);
        }
    }
    pd_put(b, lc[256], ll[256]);
}

/* Worst case: every byte a 9-bit literal, plus per-block headers and Adler-32. */
int64_t trc_pdf_deflate_bound(int64_t n) { return n + n / 8 + 64 + (n / PD_BLOCK + 1) * 160; }

/* zlib-wrapped DEFLATE of in[0..n) into out (trc_pdf_deflate_bound(n) bytes).
 * Returns the compressed length, or -1 when memory runs out. */
int64_t trc_pdf_deflate(const char* in_, int64_t n, char* out_) {
    const unsigned char* in = (const unsigned char*)in_;
    pd_bits b; int32_t* head; int32_t* prev; pd_tok* toks; int nt = 0; int64_t i = 0, k;
    uint32_t a1 = 1, a2 = 0;
    b.out = (unsigned char*)out_; b.pos = 0; b.acc = 0; b.nbits = 0;
    head = (int32_t*)malloc(sizeof(int32_t) * PD_HSIZE);
    prev = (int32_t*)malloc(sizeof(int32_t) * PD_WIN);
    toks = (pd_tok*)malloc(sizeof(pd_tok) * PD_BLOCK);
    if (!head || !prev || !toks) { free(head); free(prev); free(toks); return -1; }
    for (k = 0; k < PD_HSIZE; k++) head[k] = -1;
    b.out[b.pos++] = 0x78; b.out[b.pos++] = 0x01;        /* CMF/FLG: deflate, 32K window */
    while (i < n) {
        int best = 0, bdist = 0;
        if (i + 2 < n) {
            uint32_t h = pd_hash(in + i);
            int32_t cand = head[h]; int chain = PD_CHAIN;
            int64_t maxlen = n - i < 258 ? n - i : 258;
            while (cand >= 0 && i - cand <= PD_WIN - 1 && chain-- > 0) {
                if (in[cand + best] == in[i + best] && in[cand] == in[i]) {
                    int l = 0;
                    while (l < maxlen && in[cand + l] == in[i + l]) l++;
                    if (l > best) { best = l; bdist = (int)(i - cand); if (l == maxlen) break; }
                }
                cand = prev[cand & (PD_WIN - 1)];
            }
            prev[i & (PD_WIN - 1)] = head[h];
            head[h] = (int32_t)i;
        }
        if (best >= 3) {
            toks[nt].len = (uint16_t)best; toks[nt].dist = (uint16_t)bdist; nt++;
            for (k = 1; k < best; k++) {
                int64_t j = i + k;
                if (j + 2 < n) {
                    uint32_t h2 = pd_hash(in + j);
                    prev[j & (PD_WIN - 1)] = head[h2];
                    head[h2] = (int32_t)j;
                }
            }
            i += best;
        } else {
            toks[nt].len = in[i]; toks[nt].dist = 0; nt++;
            i++;
        }
        if (nt == PD_BLOCK && i < n) { pd_block(&b, toks, nt, 0); nt = 0; }
    }
    pd_block(&b, toks, nt, 1);
    pd_flush(&b);
    for (k = 0; k < n; k++) { a1 = (a1 + in[k]) % 65521u; a2 = (a2 + a1) % 65521u; }
    b.out[b.pos++] = (unsigned char)(a2 >> 8); b.out[b.pos++] = (unsigned char)a2;
    b.out[b.pos++] = (unsigned char)(a1 >> 8); b.out[b.pos++] = (unsigned char)a1;
    free(head); free(prev); free(toks);
    return b.pos;
}

/* JPEG frame header: info[0]=width, info[1]=height, info[2]=components,
 * info[3]=1 when an Adobe APP14 marker is present (inverted CMYK).
 * Returns 0, or -1 when no SOF marker is found. */
int64_t trc_pdf_jpeg_info(const char* d_, int64_t n, int64_t* info) {
    const unsigned char* d = (const unsigned char*)d_;
    int64_t i = 2; info[3] = 0;
    if (n < 4 || d[0] != 0xFF || d[1] != 0xD8) return -1;
    while (i + 4 <= n) {
        int m; int64_t len;
        if (d[i] != 0xFF) return -1;
        m = d[i + 1];
        if (m == 0xFF) { i++; continue; }
        if (m == 0xD8 || m == 0x01 || (m >= 0xD0 && m <= 0xD7)) { i += 2; continue; }
        len = ((int64_t)d[i + 2] << 8) | d[i + 3];
        if (m == 0xEE && len >= 7 && i + 9 <= n && memcmp(d + i + 4, "Adobe", 5) == 0) info[3] = 1;
        if ((m >= 0xC0 && m <= 0xCF) && m != 0xC4 && m != 0xC8 && m != 0xCC) {
            if (i + 10 > n) return -1;
            info[1] = ((int64_t)d[i + 5] << 8) | d[i + 6];
            info[0] = ((int64_t)d[i + 7] << 8) | d[i + 8];
            info[2] = d[i + 9];
            return 0;
        }
        if (m == 0xD9 || m == 0xDA) return -1;
        i += 2 + len;
    }
    return -1;
}

/* Split npix RGBA pixels into rgb (3*npix) and alpha (npix).
 * Returns 1 when any pixel is not fully opaque. */
int64_t trc_pdf_split_rgba(const char* rgba_, int64_t npix, char* rgb, char* alpha) {
    const unsigned char* p = (const unsigned char*)rgba_; int64_t i; int64_t translucent = 0;
    for (i = 0; i < npix; i++) {
        rgb[3 * i] = (char)p[4 * i]; rgb[3 * i + 1] = (char)p[4 * i + 1]; rgb[3 * i + 2] = (char)p[4 * i + 2];
        alpha[i] = (char)p[4 * i + 3];
        if (p[4 * i + 3] != 255) translucent = 1;
    }
    return translucent;
}

/* Glyph widths (1/1000 em) of the standard Latin fonts for WinAnsiEncoding
* codes 32..255, measured from the metric-compatible URW base35 fonts.
* Oblique variants share their upright widths; Courier is 600 throughout. */
static const uint16_t pd_widths[6][224] = {
    /* Helvetica */
    {278,278,355,556,556,889,667,191,333,333,389,584,278,333,278,278,
     556,556,556,556,556,556,556,556,556,556,278,278,584,584,584,556,
     1015,667,667,722,722,667,611,778,722,278,500,667,556,833,722,778,
     667,778,722,667,611,722,667,944,667,667,611,278,278,278,469,556,
     333,556,556,500,556,556,278,556,556,222,222,500,222,833,556,556,
     556,556,333,500,278,556,500,722,500,500,500,334,260,334,584,350,
     556,350,222,556,333,1000,556,556,333,1000,667,333,1000,350,611,350,
     350,222,222,333,333,350,556,1000,333,1000,500,333,944,350,500,667,
     278,333,556,556,556,556,260,556,333,737,370,556,584,333,737,333,
     400,584,333,333,333,556,537,278,333,333,365,556,834,834,834,611,
     667,667,667,667,667,667,1000,722,667,667,667,667,278,278,278,278,
     722,722,778,778,778,778,778,584,778,722,722,722,722,667,667,611,
     556,556,556,556,556,556,889,500,556,556,556,556,278,278,278,278,
     556,556,556,556,556,556,556,584,611,556,556,556,556,500,556,500},
    /* Helvetica-Bold */
    {278,333,474,556,556,889,722,238,333,333,389,584,278,333,278,278,
     556,556,556,556,556,556,556,556,556,556,333,333,584,584,584,611,
     975,722,722,722,722,667,611,778,722,278,556,722,611,833,722,778,
     667,778,722,667,611,722,667,944,667,667,611,333,278,333,584,556,
     333,556,611,556,611,556,333,611,611,278,278,556,278,889,611,611,
     611,611,389,556,333,611,556,778,556,556,500,389,280,389,584,350,
     556,350,278,556,500,1000,556,556,333,1000,667,333,1000,350,611,350,
     350,278,278,500,500,350,556,1000,333,1000,556,333,944,350,500,667,
     278,333,556,556,556,556,280,556,333,737,370,556,584,333,737,333,
     400,584,333,333,333,611,556,278,333,333,365,556,834,834,834,611,
     722,722,722,722,722,722,1000,722,667,667,667,667,278,278,278,278,
     722,722,778,778,778,778,778,584,778,722,722,722,722,667,667,611,
     556,556,556,556,556,556,889,556,556,556,556,556,278,278,278,278,
     611,611,611,611,611,611,611,584,611,611,611,611,611,556,611,556},
    /* Times-Roman */
    {250,333,408,500,500,833,778,180,333,333,500,564,250,333,250,278,
     500,500,500,500,500,500,500,500,500,500,278,278,564,564,564,444,
     921,722,667,667,722,611,556,722,722,333,389,722,611,889,722,722,
     556,722,667,556,611,722,722,944,722,722,611,333,278,333,469,500,
     333,444,500,444,500,444,333,500,500,278,278,500,278,778,500,500,
     500,500,333,389,278,500,500,722,500,500,444,480,200,480,541,350,
     500,350,333,500,444,1000,500,500,333,1000,556,333,889,350,611,350,
     350,333,333,444,444,350,500,1000,333,980,389,333,722,350,444,722,
     250,333,500,500,500,500,200,500,333,760,276,500,564,333,760,333,
     400,564,300,300,333,500,453,250,333,300,310,500,750,750,750,444,
     722,722,722,722,722,722,889,667,611,611,611,611,333,333,333,333,
     722,722,722,722,722,722,722,564,722,722,722,722,722,722,556,500,
     444,444,444,444,444,444,667,444,444,444,444,444,278,278,278,278,
     500,500,500,500,500,500,500,564,500,500,500,500,500,500,500,500},
    /* Times-Bold */
    {250,333,555,500,500,1000,833,278,333,333,500,570,250,333,250,278,
     500,500,500,500,500,500,500,500,500,500,333,333,570,570,570,500,
     930,722,667,722,722,667,611,778,778,389,500,778,667,944,722,778,
     611,778,722,556,667,722,722,1000,722,722,667,333,278,333,581,500,
     333,500,556,444,556,444,333,500,556,278,333,556,278,833,556,500,
     556,556,444,389,333,556,500,722,500,500,444,394,220,394,520,350,
     500,350,333,500,500,1000,500,500,333,1000,556,333,1000,350,667,350,
     350,333,333,500,500,350,500,1000,333,1000,389,333,722,350,444,722,
     250,333,500,500,500,500,220,500,333,747,300,500,570,333,747,333,
     400,570,300,300,333,556,540,250,333,300,330,500,750,750,750,500,
     722,722,722,722,722,722,1000,722,667,667,667,667,389,389,389,389,
     722,722,778,778,778,778,778,570,778,722,722,722,722,722,611,556,
     500,500,500,500,500,500,722,444,444,444,444,444,278,278,278,278,
     500,556,500,500,500,500,500,570,500,556,556,556,556,500,556,500},
    /* Times-Italic */
    {250,333,420,500,500,833,778,214,333,333,500,675,250,333,250,278,
     500,500,500,500,500,500,500,500,500,500,333,333,675,675,675,500,
     920,611,611,667,722,611,611,722,722,333,444,667,556,833,667,722,
     611,722,611,500,556,722,611,833,611,556,556,389,278,389,422,500,
     333,500,500,444,500,444,278,500,500,278,278,444,278,722,500,500,
     500,500,389,389,278,500,444,667,444,444,389,400,275,400,541,350,
     500,350,333,500,556,889,500,500,333,1000,500,333,944,350,556,350,
     350,333,333,556,556,350,500,889,333,980,389,333,667,350,389,556,
     250,389,500,500,500,500,275,500,333,760,276,500,675,333,760,333,
     400,675,300,300,333,500,523,250,333,300,310,500,750,750,750,500,
     611,611,611,611,611,611,889,667,611,611,611,611,333,333,333,333,
     722,667,722,722,722,722,722,675,722,722,722,722,722,556,611,500,
     500,500,500,500,500,500,667,444,444,444,444,444,278,278,278,278,
     500,500,500,500,500,500,500,675,500,500,500,500,500,444,500,444},
    /* Times-BoldItalic */
    {250,389,555,500,500,833,778,278,333,333,500,570,250,333,250,278,
     500,500,500,500,500,500,500,500,500,500,333,333,570,570,570,500,
     832,667,667,667,722,667,667,722,778,389,500,667,611,889,722,722,
     611,722,667,556,611,722,667,889,667,611,611,333,278,333,570,500,
     333,500,500,444,500,444,333,500,556,278,278,500,278,778,556,500,
     500,500,389,389,278,556,444,667,500,444,389,348,220,348,570,350,
     500,350,333,500,500,1000,500,500,333,1000,556,333,944,350,611,350,
     350,333,333,500,500,350,500,1000,333,1000,389,333,722,350,389,611,
     250,389,500,500,500,500,220,500,333,747,266,500,606,333,747,333,
     400,570,300,300,333,576,500,250,333,300,300,500,750,750,750,500,
     667,667,667,667,667,667,944,667,667,667,667,667,389,389,389,389,
     722,722,722,722,722,722,722,570,722,722,722,722,722,611,611,500,
     500,500,500,500,500,500,722,444,444,444,444,444,278,278,278,278,
     500,556,500,500,500,500,500,570,500,556,556,556,556,444,500,444},
};

/* font: 0 Helvetica, 1 Helvetica-Bold, 2 Helvetica-Oblique, 3 Helvetica-BoldOblique,
 * 4 Times-Roman, 5 Times-Bold, 6 Times-Italic, 7 Times-BoldItalic, 8..11 Courier family.
 * Returns the width of WinAnsi-encoded bytes in 1/1000 em. */
int64_t trc_pdf_text_width(int64_t font, const char* s_, int64_t n) {
    static const int map[8] = {0, 1, 0, 1, 2, 3, 4, 5};
    const unsigned char* s = (const unsigned char*)s_; int64_t i, w = 0;
    if (font >= 8) return 600 * n;
    if (font < 0) font = 0;
    for (i = 0; i < n; i++) if (s[i] >= 32) w += pd_widths[map[font]][s[i] - 32];
    return w;
}
