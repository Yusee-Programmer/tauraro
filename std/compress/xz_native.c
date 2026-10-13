/*
 * std/compress/xz_native.c -- .xz and .lzma decompression for std.compress.xz.
 *
 * Linked through `# tauraro-cpp-shim: xz_native.c` in std/compress/xz.tr.
 *
 *   .xz    XZ Embedded (public domain / 0BSD, vendored unmodified in
 *          vendor/xz/): LZMA2, CRC32 / CRC64 / SHA-256 checks, multiple
 *          blocks, concatenated streams and stream padding, BCJ filters
 *          (x86, PowerPC, IA-64, ARM, ARM-Thumb, ARM64, SPARC, RISC-V).
 *          Every function of XZ Embedded is compiled `static` into this
 *          translation unit (XZ_EXTERN), so nothing clashes with a liblzma
 *          that the program may also link.
 *   .lzma  the legacy "LZMA alone" container (13-byte header + raw LZMA1),
 *          decoded by the small streaming LZMA decoder below (written for
 *          Tauraro after the LZMA specification in the public-domain LZMA
 *          SDK). Known and unknown uncompressed sizes, with or without the
 *          end-of-payload marker, lc/lp/pb up to 8/4/4.
 *
 * Handles are passed to Tauraro as int64 (the pointer value). Status codes
 * are the ones documented in bzip2_native.c.
 *
 * vendor revision: xz-embedded v2024-12-30
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define XZ_EXTERN static
#define XZ_DEC_DYNALLOC
#define XZ_DEC_CONCATENATED
#define XZ_USE_CRC64
#define XZ_USE_SHA256
#define XZ_DEC_X86
#define XZ_DEC_POWERPC
#define XZ_DEC_IA64
#define XZ_DEC_ARM
#define XZ_DEC_ARMTHUMB
#define XZ_DEC_ARM64
#define XZ_DEC_SPARC
#define XZ_DEC_RISCV

#if defined(__GNUC__) || defined(__clang__)
/* xz.h declares the (unused) MicroLZMA API; with XZ_EXTERN=static that is
 * a "declared static but never defined" warning */
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "vendor/xz/xz_config.h"
#include "vendor/xz/xz_crc32.c"
#include "vendor/xz/xz_crc64.c"
#include "vendor/xz/xz_sha256.c"
#include "vendor/xz/xz_dec_lzma2.c"
#include "vendor/xz/xz_dec_bcj.c"
#include "vendor/xz/xz_dec_stream.c"

/* ── growable byte buffer ───────────────────────────────────────────────── */

typedef struct {
    unsigned char *p;
    size_t len, cap;
} trc_xz_obuf;

static int trc_xz_reserve(trc_xz_obuf *b, size_t extra) {
    if (b->cap - b->len >= extra) return 1;
    size_t nc = b->cap ? b->cap * 2 : 65536;
    while (nc - b->len < extra) nc *= 2;
    unsigned char *np = (unsigned char *)realloc(b->p, nc);
    if (!np) return 0;
    b->p = np;
    b->cap = nc;
    return 1;
}

static int trc_xz_push(trc_xz_obuf *b, const unsigned char *src, size_t n) {
    if (!n) return 1;
    if (!trc_xz_reserve(b, n)) return 0;
    memcpy(b->p + b->len, src, n);
    b->len += n;
    return 1;
}

/* ── .xz decoder ────────────────────────────────────────────────────────── */

#define TRC_XZ_DICT_MAX (1536u << 20)   /* xz's own maximum: 1.5 GiB */

typedef struct {
    trc_xz_obuf out;
    struct xz_dec *s;
    int64_t total, max_out;
    int fed;          /* any input seen */
    int failed;
    int done;
} trc_xzd;

static int trc_xzd_map(enum xz_ret r) {
    switch (r) {
    case XZ_MEM_ERROR:      return -2;
    case XZ_MEMLIMIT_ERROR: return -5;
    case XZ_FORMAT_ERROR:   return -8;
    case XZ_OPTIONS_ERROR:  return -5;
    case XZ_UNSUPPORTED_CHECK: return -5;
    default:                return -1;
    }
}

static int trc_xz_crc_ready = 0;

int64_t trc_xzd_new(int64_t max_out) {
    if (!trc_xz_crc_ready) {
        xz_crc32_init();
        xz_crc64_init();
        trc_xz_crc_ready = 1;
    }
    trc_xzd *d = (trc_xzd *)calloc(1, sizeof(trc_xzd));
    if (!d) return 0;
    d->s = xz_dec_init(XZ_DYNALLOC, TRC_XZ_DICT_MAX);
    if (!d->s) { free(d); return 0; }
    d->max_out = max_out;
    return (int64_t)(intptr_t)d;
}

static int64_t trc_xzd_run(trc_xzd *d, const char *in, int64_t n, int finish) {
    static const uint8_t empty[1] = {0};
    struct xz_buf b;
    b.in = n > 0 ? (const uint8_t *)in : empty;
    b.in_pos = 0;
    b.in_size = (size_t)n;
    for (;;) {
        if (!trc_xz_reserve(&d->out, 65536)) return d->failed = -2;
        size_t room = d->out.cap - d->out.len;
        if (d->max_out > 0) {
            int64_t left = d->max_out - d->total + 1;
            if (left < 1) left = 1;
            if ((int64_t)room > left) room = (size_t)left;
        }
        b.out = d->out.p;
        b.out_pos = d->out.len;
        b.out_size = d->out.len + room;
        enum xz_ret r = xz_dec_catrun(d->s, &b, finish);
        size_t made = b.out_pos - d->out.len;
        d->out.len = b.out_pos;
        d->total += (int64_t)made;
        if (d->max_out > 0 && d->total > d->max_out) return d->failed = -3;
        if (r == XZ_STREAM_END) { d->done = 1; return 1; }
        if (r == XZ_OK) {
            /* all input used and room left: wait for more input (when
             * finishing, the next call reports the truncation instead) */
            if (!finish && b.in_pos >= b.in_size && b.out_pos < b.out_size) return 0;
            continue;
        }
        if (finish && (r == XZ_BUF_ERROR || r == XZ_DATA_ERROR)) return d->failed = -4;
        if (r == XZ_BUF_ERROR) return d->failed = -4;
        return d->failed = trc_xzd_map(r);
    }
}

int64_t trc_xzd_feed(int64_t h, const char *in, int64_t n) {
    trc_xzd *d = (trc_xzd *)(intptr_t)h;
    if (!d || n < 0) return -7;
    if (d->failed) return d->failed;
    if (d->done) return n > 0 ? -6 : 1;
    if (n == 0) return 0;
    d->fed = 1;
    return trc_xzd_run(d, in, n, 0);
}

int64_t trc_xzd_finish(int64_t h) {
    trc_xzd *d = (trc_xzd *)(intptr_t)h;
    if (!d) return -7;
    if (d->failed) return d->failed;
    if (d->done) return 1;
    if (!d->fed) return d->failed = -4;
    return trc_xzd_run(d, "", 0, 1);
}

int64_t trc_xzd_avail(int64_t h) {
    trc_xzd *d = (trc_xzd *)(intptr_t)h;
    return d ? (int64_t)d->out.len : 0;
}

void trc_xzd_take(int64_t h, char *dst) {
    trc_xzd *d = (trc_xzd *)(intptr_t)h;
    if (!d) return;
    if (d->out.len) memcpy(dst, d->out.p, d->out.len);
    d->out.len = 0;
}

void trc_xzd_free(int64_t h) {
    trc_xzd *d = (trc_xzd *)(intptr_t)h;
    if (!d) return;
    xz_dec_end(d->s);
    free(d->out.p);
    free(d);
}

/* ── .lzma ("LZMA alone") decoder ───────────────────────────────────────── */

#define LZ_PROB_INIT 1024
#define LZ_STATES 12
#define LZ_POS_STATES_MAX 16
#define LZ_LEN_TO_POS 4
#define LZ_END_POS_MODEL 14
#define LZ_FULL_DIST 128
#define LZ_ALIGN_BITS 4
/* every LZMA symbol consumes far fewer input bytes than this; decoding only
 * starts a symbol with this much input buffered, so a symbol never has to be
 * suspended halfway through (unless the stream is being finished) */
#define LZ_SYMBOL_INPUT 64

typedef struct {
    uint16_t choice, choice2;
    uint16_t low[LZ_POS_STATES_MAX][8];
    uint16_t mid[LZ_POS_STATES_MAX][8];
    uint16_t high[256];
} trc_lz_len;

typedef struct {
    trc_xz_obuf out;     /* decoded bytes waiting to be taken          */
    trc_xz_obuf in;      /* buffered compressed input                  */
    size_t ipos;         /* read position in `in`                       */
    int failed, done, overrun;
    int have_header;
    unsigned lc, lp, pb;
    uint32_t dict_size;
    int size_known;
    uint64_t unpack_size;
    int64_t max_out;
    /* range decoder */
    uint32_t range, code;
    int rc_ready;
    /* dictionary: circular window */
    unsigned char *win;
    size_t win_size, win_pos, win_flushed;
    int win_full;
    uint64_t total;
    /* LZMA state */
    unsigned state;
    uint32_t rep0, rep1, rep2, rep3;
    uint16_t *lit;       /* 0x300 << (lc + lp) */
    uint16_t is_match[LZ_STATES << 4], is_rep[LZ_STATES], is_rep_g0[LZ_STATES];
    uint16_t is_rep_g1[LZ_STATES], is_rep_g2[LZ_STATES], is_rep0_long[LZ_STATES << 4];
    uint16_t pos_slot[LZ_LEN_TO_POS][64];
    uint16_t pos_special[1 + LZ_FULL_DIST - LZ_END_POS_MODEL];
    uint16_t align[1 << LZ_ALIGN_BITS];
    trc_lz_len len_dec, rep_len_dec;
} trc_lzd;

static inline uint32_t lz_byte(trc_lzd *d) {
    if (d->ipos < d->in.len) return d->in.p[d->ipos++];
    d->overrun = 1;
    return 0;
}

static inline void lz_norm(trc_lzd *d) {
    if (d->range < (1u << 24)) {
        d->range <<= 8;
        d->code = (d->code << 8) | lz_byte(d);
    }
}

static inline unsigned lz_bit(trc_lzd *d, uint16_t *p) {
    uint32_t v = *p;
    uint32_t bound = (d->range >> 11) * v;
    unsigned bit;
    if (d->code < bound) {
        v += ((1u << 11) - v) >> 5;
        d->range = bound;
        bit = 0;
    } else {
        v -= v >> 5;
        d->code -= bound;
        d->range -= bound;
        bit = 1;
    }
    *p = (uint16_t)v;
    lz_norm(d);
    return bit;
}

static inline uint32_t lz_direct(trc_lzd *d, unsigned nbits) {
    uint32_t res = 0;
    do {
        d->range >>= 1;
        d->code -= d->range;
        uint32_t t = 0u - (d->code >> 31);
        d->code += d->range & t;
        lz_norm(d);
        res = (res << 1) + (t + 1);
    } while (--nbits);
    return res;
}

static inline unsigned lz_tree(trc_lzd *d, uint16_t *probs, unsigned nbits) {
    unsigned m = 1;
    for (unsigned i = 0; i < nbits; i++) m = (m << 1) + lz_bit(d, &probs[m]);
    return m - (1u << nbits);
}

static inline unsigned lz_rtree(trc_lzd *d, uint16_t *probs, unsigned nbits) {
    unsigned m = 1, sym = 0;
    for (unsigned i = 0; i < nbits; i++) {
        unsigned bit = lz_bit(d, &probs[m]);
        m = (m << 1) + bit;
        sym |= bit << i;
    }
    return sym;
}

static unsigned lz_len(trc_lzd *d, trc_lz_len *L, unsigned pos_state) {
    if (lz_bit(d, &L->choice) == 0) return lz_tree(d, L->low[pos_state], 3);
    if (lz_bit(d, &L->choice2) == 0) return 8 + lz_tree(d, L->mid[pos_state], 3);
    return 16 + lz_tree(d, L->high, 8);
}

static void lz_init_probs(uint16_t *p, size_t n) {
    for (size_t i = 0; i < n; i++) p[i] = LZ_PROB_INIT;
}

/* Copy window bytes not yet handed out into `out`. */
static int lz_flush(trc_lzd *d) {
    if (d->win_pos > d->win_flushed) {
        if (!trc_xz_push(&d->out, d->win + d->win_flushed, d->win_pos - d->win_flushed)) return 0;
    }
    d->win_flushed = d->win_pos;
    return 1;
}

static inline int lz_put(trc_lzd *d, unsigned char b) {
    d->win[d->win_pos++] = b;
    d->total++;
    if (d->win_pos == d->win_size) {
        if (!lz_flush(d)) return 0;
        d->win_pos = 0;
        d->win_flushed = 0;
        d->win_full = 1;
    }
    return 1;
}

static inline unsigned char lz_get(trc_lzd *d, uint32_t dist) {  /* dist >= 1 */
    size_t i = d->win_pos >= dist ? d->win_pos - dist : d->win_size - dist + d->win_pos;
    return d->win[i];
}

int64_t trc_lzd_new(int64_t max_out) {
    trc_lzd *d = (trc_lzd *)calloc(1, sizeof(trc_lzd));
    if (!d) return 0;
    d->max_out = max_out;
    return (int64_t)(intptr_t)d;
}

static int lz_header(trc_lzd *d) {
    const unsigned char *h = d->in.p;
    unsigned props = h[0];
    if (props >= 9 * 5 * 5) return -8;
    d->lc = props % 9;
    props /= 9;
    d->lp = props % 5;
    d->pb = props / 5;
    d->dict_size = (uint32_t)h[1] | ((uint32_t)h[2] << 8) | ((uint32_t)h[3] << 16) | ((uint32_t)h[4] << 24);
    uint64_t us = 0;
    for (int i = 0; i < 8; i++) us |= (uint64_t)h[5 + i] << (8 * i);
    d->size_known = us != UINT64_MAX;
    d->unpack_size = us;
    /* liblzma rejects declared sizes of 256 GiB or more as not-.lzma */
    if (d->size_known && us >= ((uint64_t)1 << 38)) return -8;
    uint64_t ws = d->dict_size < 4096 ? 4096 : d->dict_size;
    if (d->size_known && us < ws) ws = us < 4096 ? 4096 : us;
    if (d->max_out > 0 && (uint64_t)d->max_out + 1 < ws) ws = (uint64_t)d->max_out + 1 < 4096 ? 4096 : (uint64_t)d->max_out + 1;
    if (ws > ((uint64_t)1 << 32)) return -5;
    d->win_size = (size_t)ws;
    d->win = (unsigned char *)malloc(d->win_size);
    size_t nlit = (size_t)0x300 << (d->lc + d->lp);
    d->lit = (uint16_t *)malloc(nlit * sizeof(uint16_t));
    if (!d->win || !d->lit) return -2;
    lz_init_probs(d->lit, nlit);
    lz_init_probs(d->is_match, sizeof(d->is_match) / 2);
    lz_init_probs(d->is_rep, LZ_STATES);
    lz_init_probs(d->is_rep_g0, LZ_STATES);
    lz_init_probs(d->is_rep_g1, LZ_STATES);
    lz_init_probs(d->is_rep_g2, LZ_STATES);
    lz_init_probs(d->is_rep0_long, sizeof(d->is_rep0_long) / 2);
    lz_init_probs(&d->pos_slot[0][0], sizeof(d->pos_slot) / 2);
    lz_init_probs(d->pos_special, sizeof(d->pos_special) / 2);
    lz_init_probs(d->align, sizeof(d->align) / 2);
    lz_init_probs((uint16_t *)&d->len_dec, sizeof(d->len_dec) / 2);
    lz_init_probs((uint16_t *)&d->rep_len_dec, sizeof(d->rep_len_dec) / 2);
    d->ipos = 13;
    d->have_header = 1;
    return 0;
}

/* Decode as many symbols as the buffered input allows. Returns 1 when the
 * stream ended, 0 when more input is needed, < 0 on error. */
static int lz_run(trc_lzd *d, int finish) {
    if (!d->have_header) {
        if (d->in.len < 13) return finish ? -4 : 0;
        int r = lz_header(d);
        if (r < 0) return r;
    }
    if (!d->rc_ready) {
        if (d->in.len - d->ipos < 5) return finish ? -4 : 0;
        uint32_t first = lz_byte(d);
        d->range = 0xFFFFFFFFu;
        d->code = 0;
        for (int i = 0; i < 4; i++) d->code = (d->code << 8) | lz_byte(d);
        if (first != 0) return -1;
        d->rc_ready = 1;
    }
    const uint32_t pb_mask = (1u << d->pb) - 1, lp_mask = (1u << d->lp) - 1;
    for (;;) {
        if (d->size_known && d->total == d->unpack_size) {
            /* an end marker may still follow; liblzma accepts either */
            if (d->code == 0) return 1;
        }
        if (!finish && d->in.len - d->ipos < LZ_SYMBOL_INPUT) return 0;
        if (d->max_out > 0 && d->total > (uint64_t)d->max_out) return -3;
        if (d->overrun) return -4;
        unsigned pos_state = (unsigned)d->total & pb_mask;
        unsigned st = d->state;
        if (lz_bit(d, &d->is_match[(st << 4) + pos_state]) == 0) {
            if (d->size_known && d->total == d->unpack_size) return -1;
            unsigned prev = (d->total || d->win_full) ? lz_get(d, 1) : 0;
            unsigned lit_state = (((unsigned)d->total & lp_mask) << d->lc) + (prev >> (8 - d->lc));
            uint16_t *probs = d->lit + 0x300u * lit_state;
            unsigned sym = 1;
            if (st >= 7) {
                unsigned match_byte = lz_get(d, d->rep0 + 1);
                do {
                    unsigned match_bit = (match_byte >> 7) & 1;
                    match_byte <<= 1;
                    unsigned bit = lz_bit(d, &probs[((1 + match_bit) << 8) + sym]);
                    sym = (sym << 1) | bit;
                    if (match_bit != bit) break;
                } while (sym < 0x100);
            }
            while (sym < 0x100) sym = (sym << 1) | lz_bit(d, &probs[sym]);
            if (!lz_put(d, (unsigned char)(sym - 0x100))) return -2;
            d->state = st < 4 ? 0 : (st < 10 ? st - 3 : st - 6);
            continue;
        }
        unsigned len;
        if (lz_bit(d, &d->is_rep[st]) != 0) {
            if (d->size_known && d->total == d->unpack_size) return -1;
            if (d->total == 0 && !d->win_full) return -1;
            if (lz_bit(d, &d->is_rep_g0[st]) == 0) {
                if (lz_bit(d, &d->is_rep0_long[(st << 4) + pos_state]) == 0) {
                    d->state = st < 7 ? 9 : 11;
                    if (!lz_put(d, lz_get(d, d->rep0 + 1))) return -2;
                    continue;
                }
            } else {
                uint32_t dist;
                if (lz_bit(d, &d->is_rep_g1[st]) == 0) {
                    dist = d->rep1;
                } else {
                    if (lz_bit(d, &d->is_rep_g2[st]) == 0) {
                        dist = d->rep2;
                    } else {
                        dist = d->rep3;
                        d->rep3 = d->rep2;
                    }
                    d->rep2 = d->rep1;
                }
                d->rep1 = d->rep0;
                d->rep0 = dist;
            }
            len = lz_len(d, &d->rep_len_dec, pos_state);
            d->state = st < 7 ? 8 : 11;
        } else {
            d->rep3 = d->rep2;
            d->rep2 = d->rep1;
            d->rep1 = d->rep0;
            len = lz_len(d, &d->len_dec, pos_state);
            d->state = st < 7 ? 7 : 10;
            unsigned len_state = len < LZ_LEN_TO_POS - 1 ? len : LZ_LEN_TO_POS - 1;
            unsigned slot = lz_tree(d, d->pos_slot[len_state], 6);
            uint32_t dist;
            if (slot < 4) {
                dist = slot;
            } else {
                unsigned ndirect = (slot >> 1) - 1;
                dist = (2u | (slot & 1u)) << ndirect;
                if (slot < LZ_END_POS_MODEL) {
                    dist += lz_rtree(d, d->pos_special + dist - slot, ndirect);
                } else {
                    dist += lz_direct(d, ndirect - LZ_ALIGN_BITS) << LZ_ALIGN_BITS;
                    dist += lz_rtree(d, d->align, LZ_ALIGN_BITS);
                }
            }
            d->rep0 = dist;
            if (dist == 0xFFFFFFFFu) {
                /* end-of-payload marker */
                if (d->overrun) return -4;
                if (d->failed) return d->failed;
                if (d->code != 0) return -1;
                if (d->size_known && d->total != d->unpack_size) return -1;
                return 1;
            }
            if (d->size_known && d->total == d->unpack_size) return -1;
            if (dist >= d->dict_size && dist >= 4096) return -1;
            if (!d->win_full && dist >= d->total) return -1;
            if (dist >= d->win_size) return -1;
        }
        if (d->overrun) return -4;
        if (d->failed) return d->failed;
        len += 2;
        if (d->size_known && d->unpack_size - d->total < len) return -1;
        uint32_t dist = d->rep0 + 1;
        for (unsigned i = 0; i < len; i++) {
            if (!lz_put(d, lz_get(d, dist))) return -2;
        }
    }
}

static int64_t trc_lzd_step(trc_lzd *d, int finish) {
    int r = lz_run(d, finish);
    if (r >= 0 && d->overrun) r = -4;
    if (d->have_header && !lz_flush(d)) r = -2;
    if (r >= 0 && d->max_out > 0 && d->total > (uint64_t)d->max_out) r = -3;
    if (r < 0) return d->failed = r;
    if (r == 1) {
        d->done = 1;
        if (d->ipos < d->in.len) return d->failed = -6;
        return 1;
    }
    /* compact the input buffer */
    if (d->ipos > 0 && d->ipos >= d->in.len / 2) {
        memmove(d->in.p, d->in.p + d->ipos, d->in.len - d->ipos);
        d->in.len -= d->ipos;
        d->ipos = 0;
    }
    if (finish) return d->failed = -4;
    return 0;
}

int64_t trc_lzd_feed(int64_t h, const char *in, int64_t n) {
    trc_lzd *d = (trc_lzd *)(intptr_t)h;
    if (!d || n < 0) return -7;
    if (d->failed) return d->failed;
    if (d->done) return n > 0 ? (d->failed = -6) : 1;
    if (!trc_xz_push(&d->in, (const unsigned char *)in, (size_t)n)) return d->failed = -2;
    return trc_lzd_step(d, 0);
}

int64_t trc_lzd_finish(int64_t h) {
    trc_lzd *d = (trc_lzd *)(intptr_t)h;
    if (!d) return -7;
    if (d->failed) return d->failed;
    if (d->done) return 1;
    return trc_lzd_step(d, 1);
}

int64_t trc_lzd_avail(int64_t h) {
    trc_lzd *d = (trc_lzd *)(intptr_t)h;
    return d ? (int64_t)d->out.len : 0;
}

void trc_lzd_take(int64_t h, char *dst) {
    trc_lzd *d = (trc_lzd *)(intptr_t)h;
    if (!d) return;
    if (d->out.len) memcpy(dst, d->out.p, d->out.len);
    d->out.len = 0;
}

void trc_lzd_free(int64_t h) {
    trc_lzd *d = (trc_lzd *)(intptr_t)h;
    if (!d) return;
    free(d->out.p);
    free(d->in.p);
    free(d->win);
    free(d->lit);
    free(d);
}
