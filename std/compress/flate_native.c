/*
 * std/compress/flate_native.c -- DEFLATE (RFC 1951) decoder and encoder with
 * zlib (RFC 1950) and gzip (RFC 1952) framing, no external library.
 *
 * Linked through `# tauraro-cpp-shim: flate_native.c` in std/compress/flate.tr.
 *
 * Decoder: canonical-Huffman decoding by code length counts (the method of
 * zlib's contrib/puff), with output into a growing heap buffer bounded by a
 * caller-chosen maximum. Encoder: LZ77 with hash chains, fixed or dynamic
 * Huffman per block, whichever is smaller (same as std/encoding/pdf_native.c).
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* ── decoder ──────────────────────────────────────────────────────────────── */

#define FL_MAXBITS 15
#define FL_MAXLCODES 286
#define FL_MAXDCODES 30
#define FL_FIXLCODES 288

typedef struct {
    const unsigned char* in; size_t inlen; size_t incnt;
    int bitbuf; int bitcnt;
    unsigned char* out; size_t outlen; size_t outcap; size_t outmax;
    int err;                 /* 0 ok, else FL_E_* */
} fl_state;

enum { FL_E_INPUT = 1, FL_E_DATA = 2, FL_E_LIMIT = 3, FL_E_MEM = 4 };

typedef struct { short* count; short* symbol; } fl_huff;

static int fl_bits(fl_state* s, int need) {
    long val = s->bitbuf;
    while (s->bitcnt < need) {
        if (s->incnt == s->inlen) { s->err = FL_E_INPUT; return 0; }
        val |= (long)(s->in[s->incnt++]) << s->bitcnt;
        s->bitcnt += 8;
    }
    s->bitbuf = (int)(val >> need);
    s->bitcnt -= need;
    return (int)(val & ((1L << need) - 1));
}

static int fl_put(fl_state* s, unsigned char c) {
    if (s->outlen == s->outcap) {
        size_t nc;
        unsigned char* p;
        if (s->outmax && s->outlen >= s->outmax) { s->err = FL_E_LIMIT; return -1; }
        nc = s->outcap ? s->outcap * 2 : 4096;
        if (s->outmax && nc > s->outmax) nc = s->outmax;
        p = (unsigned char*)realloc(s->out, nc + 1);
        if (!p) { s->err = FL_E_MEM; return -1; }
        s->out = p; s->outcap = nc;
    }
    s->out[s->outlen++] = c;
    return 0;
}

static int fl_stored(fl_state* s) {
    unsigned len;
    s->bitbuf = 0; s->bitcnt = 0;
    if (s->incnt + 4 > s->inlen) { s->err = FL_E_INPUT; return -1; }
    len = s->in[s->incnt] | ((unsigned)s->in[s->incnt + 1] << 8);
    if (s->in[s->incnt + 2] != (~len & 0xff) || s->in[s->incnt + 3] != ((~len >> 8) & 0xff)) { s->err = FL_E_DATA; return -1; }
    s->incnt += 4;
    if (s->incnt + len > s->inlen) { s->err = FL_E_INPUT; return -1; }
    while (len--) if (fl_put(s, s->in[s->incnt++])) return -1;
    return 0;
}

static int fl_decode(fl_state* s, const fl_huff* h) {
    int code = 0, first = 0, index = 0, len;
    for (len = 1; len <= FL_MAXBITS; len++) {
        int count;
        code |= fl_bits(s, 1);
        if (s->err) return -1;
        count = h->count[len];
        if (code - count < first) return h->symbol[index + (code - first)];
        index += count;
        first += count;
        first <<= 1;
        code <<= 1;
    }
    s->err = FL_E_DATA;
    return -1;
}

/* Build decoding tables; returns 0 for a complete code, >0 incomplete, <0 over-subscribed. */
static int fl_construct(fl_huff* h, const short* length, int n) {
    int symbol, len, left;
    short offs[FL_MAXBITS + 1];
    for (len = 0; len <= FL_MAXBITS; len++) h->count[len] = 0;
    for (symbol = 0; symbol < n; symbol++) h->count[length[symbol]]++;
    if (h->count[0] == n) return 0;
    left = 1;
    for (len = 1; len <= FL_MAXBITS; len++) {
        left <<= 1;
        left -= h->count[len];
        if (left < 0) return left;
    }
    offs[1] = 0;
    for (len = 1; len < FL_MAXBITS; len++) offs[len + 1] = (short)(offs[len] + h->count[len]);
    for (symbol = 0; symbol < n; symbol++)
        if (length[symbol] != 0) h->symbol[offs[length[symbol]]++] = (short)symbol;
    return left;
}

static const short fl_lbase[29] = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
static const short fl_lext[29]  = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
static const short fl_dbase[30] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
static const short fl_dext[30]  = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

static int fl_codes(fl_state* s, const fl_huff* lc, const fl_huff* dc) {
    int symbol;
    for (;;) {
        symbol = fl_decode(s, lc);
        if (symbol < 0) return -1;
        if (symbol < 256) {
            if (fl_put(s, (unsigned char)symbol)) return -1;
        } else if (symbol > 256) {
            int len; unsigned dist;
            symbol -= 257;
            if (symbol >= 29) { s->err = FL_E_DATA; return -1; }
            len = fl_lbase[symbol] + fl_bits(s, fl_lext[symbol]);
            if (s->err) return -1;
            symbol = fl_decode(s, dc);
            if (symbol < 0) return -1;
            if (symbol >= 30) { s->err = FL_E_DATA; return -1; }
            dist = (unsigned)(fl_dbase[symbol] + fl_bits(s, fl_dext[symbol]));
            if (s->err) return -1;
            if (dist > s->outlen) { s->err = FL_E_DATA; return -1; }
            while (len--) {
                if (fl_put(s, s->out[s->outlen - dist])) return -1;
            }
        } else {
            return 0;
        }
    }
}

static int fl_fixed(fl_state* s) {
    static int built = 0;
    static short lcnt[FL_MAXBITS + 1], lsym[FL_FIXLCODES];
    static short dcnt[FL_MAXBITS + 1], dsym[FL_MAXDCODES];
    static fl_huff lc, dc;
    if (!built) {
        short lengths[FL_FIXLCODES];
        int symbol;
        lc.count = lcnt; lc.symbol = lsym; dc.count = dcnt; dc.symbol = dsym;
        for (symbol = 0; symbol < 144; symbol++) lengths[symbol] = 8;
        for (; symbol < 256; symbol++) lengths[symbol] = 9;
        for (; symbol < 280; symbol++) lengths[symbol] = 7;
        for (; symbol < FL_FIXLCODES; symbol++) lengths[symbol] = 8;
        fl_construct(&lc, lengths, FL_FIXLCODES);
        for (symbol = 0; symbol < FL_MAXDCODES; symbol++) lengths[symbol] = 5;
        fl_construct(&dc, lengths, FL_MAXDCODES);
        built = 1;
    }
    return fl_codes(s, &lc, &dc);
}

static int fl_dynamic(fl_state* s) {
    int nlen, ndist, ncode, index, err;
    short lengths[FL_MAXLCODES + FL_MAXDCODES];
    short lcnt[FL_MAXBITS + 1], lsym[FL_MAXLCODES];
    short dcnt[FL_MAXBITS + 1], dsym[FL_MAXDCODES];
    fl_huff lc, dc;
    static const short order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    lc.count = lcnt; lc.symbol = lsym; dc.count = dcnt; dc.symbol = dsym;
    nlen = fl_bits(s, 5) + 257;
    ndist = fl_bits(s, 5) + 1;
    ncode = fl_bits(s, 4) + 4;
    if (s->err) return -1;
    if (nlen > FL_MAXLCODES || ndist > FL_MAXDCODES) { s->err = FL_E_DATA; return -1; }
    for (index = 0; index < ncode; index++) lengths[order[index]] = (short)fl_bits(s, 3);
    for (; index < 19; index++) lengths[order[index]] = 0;
    if (s->err) return -1;
    err = fl_construct(&lc, lengths, 19);
    if (err != 0) { s->err = FL_E_DATA; return -1; }
    index = 0;
    while (index < nlen + ndist) {
        int symbol, len;
        symbol = fl_decode(s, &lc);
        if (symbol < 0) return -1;
        if (symbol < 16) {
            lengths[index++] = (short)symbol;
        } else {
            len = 0;
            if (symbol == 16) {
                if (index == 0) { s->err = FL_E_DATA; return -1; }
                len = lengths[index - 1];
                symbol = 3 + fl_bits(s, 2);
            } else if (symbol == 17) {
                symbol = 3 + fl_bits(s, 3);
            } else {
                symbol = 11 + fl_bits(s, 7);
            }
            if (s->err) return -1;
            if (index + symbol > nlen + ndist) { s->err = FL_E_DATA; return -1; }
            while (symbol--) lengths[index++] = (short)len;
        }
    }
    if (lengths[256] == 0) { s->err = FL_E_DATA; return -1; }
    err = fl_construct(&lc, lengths, nlen);
    if (err && (err < 0 || nlen != lc.count[0] + lc.count[1])) { s->err = FL_E_DATA; return -1; }
    err = fl_construct(&dc, lengths + nlen, ndist);
    if (err && (err < 0 || ndist != dc.count[0] + dc.count[1])) { s->err = FL_E_DATA; return -1; }
    return fl_codes(s, &lc, &dc);
}

/* Inflate one raw DEFLATE stream starting at s->incnt. */
static int fl_inflate_raw(fl_state* s) {
    int last, type, r;
    do {
        last = fl_bits(s, 1);
        type = fl_bits(s, 2);
        if (s->err) return -1;
        if (type == 0) r = fl_stored(s);
        else if (type == 1) r = fl_fixed(s);
        else if (type == 2) r = fl_dynamic(s);
        else { s->err = FL_E_DATA; r = -1; }
        if (r) return -1;
    } while (!last);
    /* back to a byte boundary */
    s->bitbuf = 0; s->bitcnt = 0;
    return 0;
}

static uint32_t fl_crc_table[256];
static int fl_crc_ready = 0;
static uint32_t fl_crc32(uint32_t crc, const unsigned char* p, size_t n) {
    size_t i;
    if (!fl_crc_ready) {
        uint32_t c; int k; unsigned j;
        for (j = 0; j < 256; j++) { c = j; for (k = 0; k < 8; k++) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1; fl_crc_table[j] = c; }
        fl_crc_ready = 1;
    }
    crc = ~crc;
    for (i = 0; i < n; i++) crc = fl_crc_table[(crc ^ p[i]) & 0xff] ^ (crc >> 8);
    return ~crc;
}
static uint32_t fl_adler32(const unsigned char* p, size_t n) {
    uint32_t a = 1, b = 0; size_t i;
    for (i = 0; i < n; i++) { a = (a + p[i]) % 65521u; b = (b + a) % 65521u; }
    return (b << 16) | a;
}

/* mode: 0 raw DEFLATE, 1 zlib, 2 gzip (all members), 3 auto (gzip / zlib / raw).
 * info[0] = output length, info[1] = error code (0 ok, 1 truncated, 2 corrupt,
 * 3 over max_out, 4 out of memory, 5 bad header, 6 bad checksum),
 * info[2] = input bytes consumed. Returns a malloc'd, NUL-terminated buffer
 * (caller frees) or NULL on error. max_out <= 0: unlimited. */
char* trc_flate_inflate(const char* in_, int64_t n, int64_t max_out, int64_t mode, int64_t* info) {
    const unsigned char* in = (const unsigned char*)in_;
    fl_state s;
    memset(&s, 0, sizeof s);
    s.in = in; s.inlen = (size_t)n; s.outmax = max_out > 0 ? (size_t)max_out : 0;
    info[0] = 0; info[1] = 0; info[2] = 0;
    if (mode == 3) {
        if (n >= 2 && in[0] == 0x1f && in[1] == 0x8b) mode = 2;
        else if (n >= 2 && (in[0] & 0x0f) == 8 && ((in[0] << 8) | in[1]) % 31 == 0) mode = 1;
        else mode = 0;
    }
    if (mode == 1) {
        if (n < 2 || (in[0] & 0x0f) != 8 || ((in[0] << 8) | in[1]) % 31 != 0 || (in[1] & 0x20)) { info[1] = 5; return NULL; }
        s.incnt = 2;
        if (fl_inflate_raw(&s)) goto fail;
        if (s.incnt + 4 > s.inlen) { s.err = FL_E_INPUT; goto fail; }
        {
            uint32_t want = ((uint32_t)in[s.incnt] << 24) | ((uint32_t)in[s.incnt + 1] << 16) | ((uint32_t)in[s.incnt + 2] << 8) | in[s.incnt + 3];
            if (want != fl_adler32(s.out, s.outlen)) { info[1] = 6; free(s.out); return NULL; }
        }
        s.incnt += 4;
    } else if (mode == 2) {
        size_t start_out;
        int members = 0;
        while (s.incnt < s.inlen) {
            size_t p = s.incnt; unsigned flg;
            if (members > 0 && s.inlen - p < 2) break;
            if (s.inlen - p < 10 || in[p] != 0x1f || in[p + 1] != 0x8b || in[p + 2] != 8) {
                if (members > 0) break;          /* trailing garbage after a member: stop */
                info[1] = 5; free(s.out); return NULL;
            }
            flg = in[p + 3];
            p += 10;
            if (flg & 4) { size_t xl; if (p + 2 > s.inlen) { s.err = FL_E_INPUT; goto fail; } xl = in[p] | ((size_t)in[p + 1] << 8); p += 2 + xl; }
            if (flg & 8) { while (p < s.inlen && in[p]) p++; p++; }
            if (flg & 16) { while (p < s.inlen && in[p]) p++; p++; }
            if (flg & 2) p += 2;
            if (p > s.inlen) { s.err = FL_E_INPUT; goto fail; }
            s.incnt = p;
            start_out = s.outlen;
            if (fl_inflate_raw(&s)) goto fail;
            if (s.incnt + 8 > s.inlen) { s.err = FL_E_INPUT; goto fail; }
            {
                const unsigned char* t = in + s.incnt;
                uint32_t crc = t[0] | ((uint32_t)t[1] << 8) | ((uint32_t)t[2] << 16) | ((uint32_t)t[3] << 24);
                uint32_t isz = t[4] | ((uint32_t)t[5] << 8) | ((uint32_t)t[6] << 16) | ((uint32_t)t[7] << 24);
                if (crc != fl_crc32(0, s.out + start_out, s.outlen - start_out) || isz != (uint32_t)((s.outlen - start_out) & 0xffffffffu)) {
                    info[1] = 6; free(s.out); return NULL;
                }
            }
            s.incnt += 8;
            members++;
        }
    } else {
        if (fl_inflate_raw(&s)) goto fail;
    }
    if (!s.out) { s.out = (unsigned char*)malloc(1); if (!s.out) { info[1] = 4; return NULL; } }
    s.out[s.outlen] = 0;
    info[0] = (int64_t)s.outlen;
    info[2] = (int64_t)s.incnt;
    return (char*)s.out;
fail:
    free(s.out);
    info[1] = s.err == FL_E_INPUT ? 1 : s.err == FL_E_LIMIT ? 3 : s.err == FL_E_MEM ? 4 : 2;
    return NULL;
}

int64_t trc_flate_crc32(const char* p, int64_t n) { return (int64_t)fl_crc32(0, (const unsigned char*)p, (size_t)n); }
int64_t trc_flate_adler32(const char* p, int64_t n) { return (int64_t)fl_adler32((const unsigned char*)p, (size_t)n); }

/* ── encoder (shared design with std/encoding/pdf_native.c) ──────────────── */

/* ── bit writer (LSB first, as DEFLATE requires) ─────────────────────────── */
typedef struct { unsigned char* out; int64_t pos; uint32_t acc; int nbits; } fe_bits;

static void fe_put(fe_bits* b, uint32_t v, int n) {
    b->acc |= v << b->nbits;
    b->nbits += n;
    while (b->nbits >= 8) { b->out[b->pos++] = (unsigned char)(b->acc & 255); b->acc >>= 8; b->nbits -= 8; }
}
static void fe_flush(fe_bits* b) {
    if (b->nbits > 0) { b->out[b->pos++] = (unsigned char)(b->acc & 255); }
    b->acc = 0; b->nbits = 0;
}
/* Huffman codes are defined MSB first; reverse them for the LSB-first stream. */
static uint32_t fe_rev(uint32_t code, int len) {
    uint32_t r = 0; int i;
    for (i = 0; i < len; i++) { r = (r << 1) | (code & 1); code >>= 1; }
    return r;
}
static void fe_sym(fe_bits* b, int sym) {
    if (sym < 144)      fe_put(b, fe_rev(0x30 + sym, 8), 8);
    else if (sym < 256) fe_put(b, fe_rev(0x190 + (sym - 144), 9), 9);
    else if (sym < 280) fe_put(b, fe_rev(sym - 256, 7), 7);
    else                fe_put(b, fe_rev(0xC0 + (sym - 280), 8), 8);
}

static const int fe_lbase[29] = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
static const int fe_lext[29]  = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
static const int fe_dbase[30] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
static const int fe_dext[30]  = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

static void fe_match(fe_bits* b, int len, int dist) {
    int li = 28, di = 29;
    while (fe_lbase[li] > len) li--;
    fe_sym(b, 257 + li);
    if (fe_lext[li]) fe_put(b, (uint32_t)(len - fe_lbase[li]), fe_lext[li]);
    while (fe_dbase[di] > dist) di--;
    fe_put(b, fe_rev((uint32_t)di, 5), 5);
    if (fe_dext[di]) fe_put(b, (uint32_t)(dist - fe_dbase[di]), fe_dext[di]);
}

#define FE_WIN   32768
#define FE_HBITS 15
#define FE_HSIZE (1 << FE_HBITS)
#define FE_CHAIN 48
#define FE_BLOCK 65536          /* tokens per block */

static uint32_t fe_hash(const unsigned char* p) {
    return ((uint32_t)p[0] * 2654435761u ^ (uint32_t)p[1] * 40503u ^ (uint32_t)p[2]) & (FE_HSIZE - 1);
}

/* A token is a literal (dist == 0) or a match (len 3..258, dist 1..32768). */
typedef struct { uint16_t len; uint16_t dist; } fe_tok;

static int fe_lcode(int len) { int li = 28; while (fe_lbase[li] > len) li--; return li; }
static int fe_dcode(int dist) { int di = 29; while (fe_dbase[di] > dist) di--; return di; }

/* Code lengths for freq[0..n) limited to `limit` bits (Huffman, then the
 * JPEG Annex K.3 length adjustment). Unused symbols get length 0. */
static void fe_lengths(const uint32_t* freq, int n, int limit, uint8_t* lens) {
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

static void fe_codes(const uint8_t* lens, int n, uint16_t* codes) {
    int bl[16] = {0}, next[16], i, code = 0;
    for (i = 0; i < n; i++) bl[lens[i]]++;
    bl[0] = 0;
    for (i = 1; i < 16; i++) { code = (code + bl[i - 1]) << 1; next[i] = code; }
    for (i = 0; i < n; i++) if (lens[i]) codes[i] = (uint16_t)fe_rev((uint32_t)next[lens[i]]++, lens[i]);
}

static const int fe_clorder[19] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};

/* Run-length encode the code lengths (symbols 0-18 with extra-bit payloads). */
static int fe_rle(const uint8_t* all, int total, int* rs, int* rx) {
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

static void fe_block(fe_bits* b, const fe_tok* t, int nt, int final) {
    uint32_t lf[286], df[30], cf[19]; uint8_t ll[286], dl[30], cl[19], all[316];
    uint16_t lc[286], dc[30], cc[19];
    int rs[320], rx[320], nr, i, hlit, hdist, hclen;
    uint64_t fixed_bits = 3, dyn_bits = 3 + 5 + 5 + 4;
    memset(lf, 0, sizeof lf); memset(df, 0, sizeof df); memset(cf, 0, sizeof cf);
    memset(lc, 0, sizeof lc); memset(dc, 0, sizeof dc); memset(cc, 0, sizeof cc);
    for (i = 0; i < nt; i++) {
        if (t[i].dist == 0) lf[t[i].len]++;
        else { lf[257 + fe_lcode(t[i].len)]++; df[fe_dcode(t[i].dist)]++; }
    }
    lf[256] = 1;
    fe_lengths(lf, 286, 15, ll);
    fe_lengths(df, 30, 15, dl);
    { int used = 0; for (i = 0; i < 30; i++) if (dl[i]) used++;
      if (used == 0) { dl[0] = 1; dl[1] = 1; } else if (used == 1) { if (dl[0]) dl[1] = 1; else dl[0] = 1; } }
    { int used = 0; for (i = 0; i < 286; i++) if (ll[i]) used++;
      if (used == 1) { if (ll[0]) ll[1] = 1; else ll[0] = 1; } }
    for (hlit = 286; hlit > 257 && ll[hlit - 1] == 0; hlit--) {}
    for (hdist = 30; hdist > 1 && dl[hdist - 1] == 0; hdist--) {}
    memcpy(all, ll, (size_t)hlit); memcpy(all + hlit, dl, (size_t)hdist);
    nr = fe_rle(all, hlit + hdist, rs, rx);
    for (i = 0; i < nr; i++) cf[rs[i]]++;
    fe_lengths(cf, 19, 7, cl);
    { int used = 0; for (i = 0; i < 19; i++) if (cl[i]) used++;
      if (used == 1) { if (cl[0]) cl[1] = 1; else cl[0] = 1; } }
    for (hclen = 19; hclen > 4 && cl[fe_clorder[hclen - 1]] == 0; hclen--) {}
    /* cost of each encoding */
    for (i = 0; i < 286; i++) {
        int fl = i < 144 ? 8 : i < 256 ? 9 : i < 280 ? 7 : 8;
        int ex = i >= 257 ? fe_lext[i - 257] : 0;
        fixed_bits += (uint64_t)lf[i] * (uint64_t)(fl + ex);
        dyn_bits += (uint64_t)lf[i] * (uint64_t)(ll[i] + ex);
    }
    for (i = 0; i < 30; i++) { fixed_bits += (uint64_t)df[i] * (uint64_t)(5 + fe_dext[i]); dyn_bits += (uint64_t)df[i] * (uint64_t)(dl[i] + fe_dext[i]); }
    dyn_bits += 3u * (uint64_t)hclen;
    for (i = 0; i < nr; i++) dyn_bits += cl[rs[i]] + (rs[i] == 16 ? 2 : rs[i] == 17 ? 3 : rs[i] == 18 ? 7 : 0);
    fe_put(b, (uint32_t)final, 1);
    if (fixed_bits <= dyn_bits) {
        fe_put(b, 1, 2);
        for (i = 0; i < nt; i++) {
            if (t[i].dist == 0) fe_sym(b, t[i].len); else fe_match(b, t[i].len, t[i].dist);
        }
        fe_sym(b, 256);
        return;
    }
    fe_codes(ll, 286, lc); fe_codes(dl, 30, dc); fe_codes(cl, 19, cc);
    fe_put(b, 2, 2);
    fe_put(b, (uint32_t)(hlit - 257), 5); fe_put(b, (uint32_t)(hdist - 1), 5); fe_put(b, (uint32_t)(hclen - 4), 4);
    for (i = 0; i < hclen; i++) fe_put(b, cl[fe_clorder[i]], 3);
    for (i = 0; i < nr; i++) {
        fe_put(b, cc[rs[i]], cl[rs[i]]);
        if (rs[i] == 16) fe_put(b, (uint32_t)rx[i], 2);
        else if (rs[i] == 17) fe_put(b, (uint32_t)rx[i], 3);
        else if (rs[i] == 18) fe_put(b, (uint32_t)rx[i], 7);
    }
    for (i = 0; i < nt; i++) {
        if (t[i].dist == 0) fe_put(b, lc[t[i].len], ll[t[i].len]);
        else {
            int li = fe_lcode(t[i].len), di = fe_dcode(t[i].dist);
            fe_put(b, lc[257 + li], ll[257 + li]);
            if (fe_lext[li]) fe_put(b, (uint32_t)(t[i].len - fe_lbase[li]), fe_lext[li]);
            fe_put(b, dc[di], dl[di]);
            if (fe_dext[di]) fe_put(b, (uint32_t)(t[i].dist - fe_dbase[di]), fe_dext[di]);
        }
    }
    fe_put(b, lc[256], ll[256]);
}

/* Worst case: every byte a 9-bit literal, plus per-block headers and Adler-32. */
static int64_t fe_bound(int64_t n) { return n + n / 8 + 64 + (n / FE_BLOCK + 1) * 160; }

/* zlib-wrapped DEFLATE of in[0..n) into out (trc_pdf_deflate_bound(n) bytes).
 * Returns the compressed length, or -1 when memory runs out. */
static int64_t fe_deflate(const char* in_, int64_t n, char* out_, int chain_max, int framing) {
    const unsigned char* in = (const unsigned char*)in_;
    fe_bits b; int32_t* head; int32_t* prev; fe_tok* toks; int nt = 0; int64_t i = 0, k;
    uint32_t a1 = 1, a2 = 0;
    b.out = (unsigned char*)out_; b.pos = 0; b.acc = 0; b.nbits = 0;
    head = (int32_t*)malloc(sizeof(int32_t) * FE_HSIZE);
    prev = (int32_t*)malloc(sizeof(int32_t) * FE_WIN);
    toks = (fe_tok*)malloc(sizeof(fe_tok) * FE_BLOCK);
    if (!head || !prev || !toks) { free(head); free(prev); free(toks); return -1; }
    for (k = 0; k < FE_HSIZE; k++) head[k] = -1;
    if (framing == 1) { b.out[b.pos++] = 0x78; b.out[b.pos++] = 0x01; }        /* zlib: deflate, 32K window */
    else if (framing == 2) {                                                     /* gzip member header */
        static const unsigned char gz[10] = {0x1f, 0x8b, 8, 0, 0, 0, 0, 0, 0, 0xff};
        memcpy(b.out, gz, 10); b.pos = 10;
    }
    while (i < n) {
        int best = 0, bdist = 0;
        if (i + 2 < n) {
            uint32_t h = fe_hash(in + i);
            int32_t cand = head[h]; int chain = chain_max;
            int64_t maxlen = n - i < 258 ? n - i : 258;
            while (cand >= 0 && i - cand <= FE_WIN - 1 && chain-- > 0) {
                if (in[cand + best] == in[i + best] && in[cand] == in[i]) {
                    int l = 0;
                    while (l < maxlen && in[cand + l] == in[i + l]) l++;
                    if (l > best) { best = l; bdist = (int)(i - cand); if (l == maxlen) break; }
                }
                cand = prev[cand & (FE_WIN - 1)];
            }
            prev[i & (FE_WIN - 1)] = head[h];
            head[h] = (int32_t)i;
        }
        if (best >= 3) {
            toks[nt].len = (uint16_t)best; toks[nt].dist = (uint16_t)bdist; nt++;
            for (k = 1; k < best; k++) {
                int64_t j = i + k;
                if (j + 2 < n) {
                    uint32_t h2 = fe_hash(in + j);
                    prev[j & (FE_WIN - 1)] = head[h2];
                    head[h2] = (int32_t)j;
                }
            }
            i += best;
        } else {
            toks[nt].len = in[i]; toks[nt].dist = 0; nt++;
            i++;
        }
        if (nt == FE_BLOCK && i < n) { fe_block(&b, toks, nt, 0); nt = 0; }
    }
    fe_block(&b, toks, nt, 1);
    fe_flush(&b);
    if (framing == 1) {
        for (k = 0; k < n; k++) { a1 = (a1 + in[k]) % 65521u; a2 = (a2 + a1) % 65521u; }
        b.out[b.pos++] = (unsigned char)(a2 >> 8); b.out[b.pos++] = (unsigned char)a2;
        b.out[b.pos++] = (unsigned char)(a1 >> 8); b.out[b.pos++] = (unsigned char)a1;
    } else if (framing == 2) {
        uint32_t crc = fl_crc32(0, in, (size_t)n), isz = (uint32_t)((uint64_t)n & 0xffffffffu);
        int q;
        for (q = 0; q < 4; q++) b.out[b.pos++] = (unsigned char)(crc >> (8 * q));
        for (q = 0; q < 4; q++) b.out[b.pos++] = (unsigned char)(isz >> (8 * q));
    }
    free(head); free(prev); free(toks);
    return b.pos;
}


/* ── encoder API ───────────────────────────────────────────────────────────── */

/* Upper bound of the compressed size for n input bytes (any framing). */
int64_t trc_flate_bound(int64_t n) { return fe_bound(n) + 32; }

/* Compress in[0..n) into out (trc_flate_bound(n) bytes). framing: 0 raw,
 * 1 zlib, 2 gzip. level 1..9 (match-search effort; 0 = 6). Returns the
 * output length or -1 when out of memory. */
int64_t trc_flate_deflate(const char* in, int64_t n, char* out, int64_t level, int64_t framing) {
    int chain;
    if (level <= 0) level = 6;
    if (level > 9) level = 9;
    chain = level <= 1 ? 4 : level <= 3 ? 12 : level <= 6 ? 48 : level <= 8 ? 256 : 1024;
    return fe_deflate(in, n, out, chain, (int)framing);
}
