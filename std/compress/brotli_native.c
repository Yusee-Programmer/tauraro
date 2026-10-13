/*
 * std/compress/brotli_native.c -- Brotli (RFC 7932) for std.compress.brotli.
 *
 * Linked through `# tauraro-cpp-shim: brotli_native.c` in
 * std/compress/brotli.tr.
 *
 * Decoding only: Google's reference decoder (MIT licence, brotli v1.2.0),
 * vendored in vendor/brotli/ (including the 122 KB RFC 7932 static
 * dictionary and its 121 transforms) and compiled into this one translation
 * unit. Large-window brotli is not enabled (it is not RFC 7932 and no HTTP
 * server sends it).
 *
 * Handles are passed to Tauraro as int64 (the pointer value). Status codes
 * are the ones documented in bzip2_native.c.
 *
 * vendor revision: brotli v1.2.0
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "vendor/brotli/common/constants.c"
#include "vendor/brotli/common/context.c"
#include "vendor/brotli/common/dictionary.c"
#include "vendor/brotli/common/platform.c"
#include "vendor/brotli/common/shared_dictionary.c"
#include "vendor/brotli/common/transform.c"
#include "vendor/brotli/dec/bit_reader.c"
#include "vendor/brotli/dec/huffman.c"
#include "vendor/brotli/dec/prefix.c"
#include "vendor/brotli/dec/state.c"
#include "vendor/brotli/dec/static_init.c"
#include "vendor/brotli/dec/decode.c"

/* ── growable byte buffer ───────────────────────────────────────────────── */

typedef struct {
    unsigned char *p;
    size_t len, cap;
} trc_br_obuf;

static int trc_br_reserve(trc_br_obuf *b, size_t extra) {
    if (b->cap - b->len >= extra) return 1;
    size_t nc = b->cap ? b->cap * 2 : 65536;
    while (nc - b->len < extra) nc *= 2;
    unsigned char *np = (unsigned char *)realloc(b->p, nc);
    if (!np) return 0;
    b->p = np;
    b->cap = nc;
    return 1;
}

/* ── decoder ────────────────────────────────────────────────────────────── */

typedef struct {
    trc_br_obuf out;
    BrotliDecoderState *s;
    int64_t total, max_out;
    int failed, done;
    int brotli_error;      /* BrotliDecoderErrorCode of the failure */
} trc_brd;

int64_t trc_brd_new(int64_t max_out) {
    trc_brd *d = (trc_brd *)calloc(1, sizeof(trc_brd));
    if (!d) return 0;
    d->s = BrotliDecoderCreateInstance(NULL, NULL, NULL);
    if (!d->s) { free(d); return 0; }
    d->max_out = max_out;
    return (int64_t)(intptr_t)d;
}

static int trc_brd_map(trc_brd *d) {
    int e = (int)BrotliDecoderGetErrorCode(d->s);
    d->brotli_error = e;
    if (e == BROTLI_DECODER_ERROR_ALLOC_CONTEXT_MODES || e == BROTLI_DECODER_ERROR_ALLOC_TREE_GROUPS ||
        e == BROTLI_DECODER_ERROR_ALLOC_CONTEXT_MAP || e == BROTLI_DECODER_ERROR_ALLOC_RING_BUFFER_1 ||
        e == BROTLI_DECODER_ERROR_ALLOC_RING_BUFFER_2 || e == BROTLI_DECODER_ERROR_ALLOC_BLOCK_TYPE_TREES)
        return -2;
    return -1;
}

int64_t trc_brd_feed(int64_t h, const char *in, int64_t n) {
    trc_brd *d = (trc_brd *)(intptr_t)h;
    if (!d || n < 0) return -7;
    if (d->failed) return d->failed;
    if (d->done) return n > 0 ? (d->failed = -6) : 1;
    size_t avail_in = (size_t)n;
    const uint8_t *next_in = (const uint8_t *)in;
    for (;;) {
        if (!trc_br_reserve(&d->out, 65536)) return d->failed = -2;
        size_t room = d->out.cap - d->out.len;
        if (d->max_out > 0) {
            int64_t left = d->max_out - d->total + 1;
            if (left < 1) left = 1;
            if ((int64_t)room > left) room = (size_t)left;
        }
        size_t avail_out = room;
        uint8_t *next_out = d->out.p + d->out.len;
        BrotliDecoderResult r = BrotliDecoderDecompressStream(d->s, &avail_in, &next_in,
                                                              &avail_out, &next_out, NULL);
        size_t made = room - avail_out;
        d->out.len += made;
        d->total += (int64_t)made;
        if (d->max_out > 0 && d->total > d->max_out) return d->failed = -3;
        if (r == BROTLI_DECODER_RESULT_SUCCESS) {
            d->done = 1;
            if (avail_in > 0) return d->failed = -6;
            return 1;
        }
        if (r == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT) continue;
        if (r == BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT) return 0;
        return d->failed = trc_brd_map(d);
    }
}

int64_t trc_brd_finish(int64_t h) {
    trc_brd *d = (trc_brd *)(intptr_t)h;
    if (!d) return -7;
    if (d->failed) return d->failed;
    if (d->done) return 1;
    return d->failed = -4;
}

int64_t trc_brd_avail(int64_t h) {
    trc_brd *d = (trc_brd *)(intptr_t)h;
    return d ? (int64_t)d->out.len : 0;
}

void trc_brd_take(int64_t h, char *dst) {
    trc_brd *d = (trc_brd *)(intptr_t)h;
    if (!d) return;
    if (d->out.len) memcpy(dst, d->out.p, d->out.len);
    d->out.len = 0;
}

/* Brotli's own error code of the last failure (0 if none). */
int64_t trc_brd_error_code(int64_t h) {
    trc_brd *d = (trc_brd *)(intptr_t)h;
    return d ? d->brotli_error : 0;
}

void trc_brd_free(int64_t h) {
    trc_brd *d = (trc_brd *)(intptr_t)h;
    if (!d) return;
    BrotliDecoderDestroyInstance(d->s);
    free(d->out.p);
    free(d);
}
