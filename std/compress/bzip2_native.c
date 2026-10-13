/*
 * std/compress/bzip2_native.c -- bzip2 compression and decompression for
 * std.compress.bzip2.
 *
 * Linked through `# tauraro-cpp-shim: bzip2_native.c` in std/compress/bzip2.tr.
 * The codec itself is libbzip2 1.0.8 (BSD-style licence), vendored unmodified
 * in vendor/bzip2/ and compiled into this one translation unit with
 * BZ_NO_STDIO (no FILE* API, no stdio dependency).
 *
 * The functions below wrap a bz_stream in a small handle that owns a growable
 * output buffer, so Tauraro can feed input in chunks and collect output
 * between calls (streaming), or feed everything at once (one-shot).
 *
 * Handles are passed to Tauraro as int64 (the pointer value).
 *
 * Status codes (shared with xz_native.c and brotli_native.c):
 *    1  stream complete              0  ok, more input expected
 *   -1  corrupt data                -2  out of memory
 *   -3  output larger than max_out  -4  truncated input
 *   -5  unsupported format option   -6  trailing data after the stream
 *   -7  bad argument / misuse       -8  wrong format (bad magic)
 *
 * vendor revision: bzip2 1.0.8
 */
#define BZ_NO_STDIO 1

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "vendor/bzip2/bzlib_private.h"
#include "vendor/bzip2/blocksort.c"
#include "vendor/bzip2/huffman.c"
#include "vendor/bzip2/crctable.c"
#include "vendor/bzip2/randtable.c"
#include "vendor/bzip2/compress.c"
#include "vendor/bzip2/decompress.c"
#include "vendor/bzip2/bzlib.c"

/* libbzip2 calls this only when one of its internal consistency assertions
 * fails (a library bug, never a property of the input). */
void bz_internal_error(int errcode) {
    (void)errcode;
    abort();
}

/* ── growable output buffer ─────────────────────────────────────────────── */

typedef struct {
    unsigned char *p;
    size_t len, cap;
} trc_bz2_obuf;

static int trc_bz2_reserve(trc_bz2_obuf *b, size_t extra) {
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
    trc_bz2_obuf out;
    bz_stream z;
    int active;        /* z is initialised (inside a stream)            */
    int started;       /* bytes of the current stream have been fed     */
    int64_t streams;   /* completed streams                              */
    int64_t total;     /* total output bytes produced                    */
    int64_t max_out;   /* <= 0: unlimited                                */
    int failed;
} trc_bz2d;

int64_t trc_bz2d_new(int64_t max_out) {
    trc_bz2d *d = (trc_bz2d *)calloc(1, sizeof(trc_bz2d));
    if (!d) return 0;
    d->max_out = max_out;
    return (int64_t)(intptr_t)d;
}

static int trc_bz2d_map(int ret) {
    if (ret == BZ_MEM_ERROR) return -2;
    if (ret == BZ_DATA_ERROR_MAGIC) return -8;
    if (ret == BZ_PARAM_ERROR || ret == BZ_SEQUENCE_ERROR) return -7;
    return -1;
}

int64_t trc_bz2d_feed(int64_t h, const char *in, int64_t n) {
    trc_bz2d *d = (trc_bz2d *)(intptr_t)h;
    if (!d || n < 0) return -7;
    if (d->failed) return d->failed;
    size_t pos = 0;
    if (n == 0 && !d->active) return 0;
    for (;;) {
        if (!d->active) {
            memset(&d->z, 0, sizeof(d->z));
            int r = BZ2_bzDecompressInit(&d->z, 0, 0);
            if (r != BZ_OK) return d->failed = trc_bz2d_map(r);
            d->active = 1;
            d->started = 0;
        }
        if (!trc_bz2_reserve(&d->out, 65536)) return d->failed = -2;
        size_t room = d->out.cap - d->out.len;
        if (d->max_out > 0) {
            /* never produce more than one byte past the limit */
            int64_t left = d->max_out - d->total + 1;
            if (left < 1) left = 1;
            if ((int64_t)room > left) room = (size_t)left;
        }
        if (room > 0x40000000u) room = 0x40000000u;
        size_t inl = (size_t)n - pos;
        if (inl > 0x40000000u) inl = 0x40000000u;
        d->z.next_in = (char *)in + pos;
        d->z.avail_in = (unsigned int)inl;
        d->z.next_out = (char *)d->out.p + d->out.len;
        d->z.avail_out = (unsigned int)room;
        int r = BZ2_bzDecompress(&d->z);
        size_t used = inl - d->z.avail_in;
        size_t made = room - d->z.avail_out;
        if (used > 0) d->started = 1;
        pos += used;
        d->out.len += made;
        d->total += (int64_t)made;
        if (d->max_out > 0 && d->total > d->max_out) return d->failed = -3;
        if (r == BZ_STREAM_END) {
            BZ2_bzDecompressEnd(&d->z);
            d->active = 0;
            d->started = 0;
            d->streams++;
            if (pos >= (size_t)n) break;
            continue;
        }
        if (r != BZ_OK) {
            int st = trc_bz2d_map(r);
            if (st == -8 && d->streams > 0) st = -6;   /* garbage after a stream */
            return d->failed = st;
        }
        /* all input consumed and the decoder did not fill the output space:
         * it is waiting for more input */
        if (pos >= (size_t)n && d->z.avail_out > 0) break;
    }
    return 0;
}

/* Bytes of output waiting to be taken. */
int64_t trc_bz2d_avail(int64_t h) {
    trc_bz2d *d = (trc_bz2d *)(intptr_t)h;
    return d ? (int64_t)d->out.len : 0;
}

/* Copy the waiting output to `dst` (at least trc_bz2d_avail bytes) and clear it. */
void trc_bz2d_take(int64_t h, char *dst) {
    trc_bz2d *d = (trc_bz2d *)(intptr_t)h;
    if (!d) return;
    if (d->out.len) memcpy(dst, d->out.p, d->out.len);
    d->out.len = 0;
}

/* End of input: 1 when every stream fed so far is complete. */
int64_t trc_bz2d_finish(int64_t h) {
    trc_bz2d *d = (trc_bz2d *)(intptr_t)h;
    if (!d) return -7;
    if (d->failed) return d->failed;
    if (d->active && d->started) return d->failed = -4;
    return 1;
}

int64_t trc_bz2d_streams(int64_t h) {
    trc_bz2d *d = (trc_bz2d *)(intptr_t)h;
    return d ? d->streams : 0;
}

void trc_bz2d_free(int64_t h) {
    trc_bz2d *d = (trc_bz2d *)(intptr_t)h;
    if (!d) return;
    if (d->active) BZ2_bzDecompressEnd(&d->z);
    free(d->out.p);
    free(d);
}

/* ── encoder ────────────────────────────────────────────────────────────── */

typedef struct {
    trc_bz2_obuf out;
    bz_stream z;
    int done;
    int failed;
} trc_bz2e;

int64_t trc_bz2e_new(int64_t level) {
    if (level < 1) level = 1;
    if (level > 9) level = 9;
    trc_bz2e *e = (trc_bz2e *)calloc(1, sizeof(trc_bz2e));
    if (!e) return 0;
    if (BZ2_bzCompressInit(&e->z, (int)level, 0, 0) != BZ_OK) {
        free(e);
        return 0;
    }
    return (int64_t)(intptr_t)e;
}

static int64_t trc_bz2e_run(trc_bz2e *e, const char *in, int64_t n, int action) {
    size_t pos = 0;
    for (;;) {
        if (!trc_bz2_reserve(&e->out, 65536)) return e->failed = -2;
        size_t room = e->out.cap - e->out.len;
        if (room > 0x40000000u) room = 0x40000000u;
        size_t inl = (size_t)n - pos;
        if (inl > 0x40000000u) inl = 0x40000000u;
        e->z.next_in = (char *)in + pos;
        e->z.avail_in = (unsigned int)inl;
        e->z.next_out = (char *)e->out.p + e->out.len;
        e->z.avail_out = (unsigned int)room;
        int act = action;
        if (action == BZ_FINISH && inl < (size_t)n - pos) act = BZ_RUN;
        int r = BZ2_bzCompress(&e->z, act);
        pos += inl - e->z.avail_in;
        e->out.len += room - e->z.avail_out;
        if (r == BZ_STREAM_END) { e->done = 1; return 1; }
        if (r != BZ_RUN_OK && r != BZ_FINISH_OK) return e->failed = -7;
        if (action == BZ_RUN && pos >= (size_t)n) return 0;
    }
}

int64_t trc_bz2e_feed(int64_t h, const char *in, int64_t n) {
    trc_bz2e *e = (trc_bz2e *)(intptr_t)h;
    if (!e || n < 0 || e->done) return -7;
    if (e->failed) return e->failed;
    if (n == 0) return 0;
    return trc_bz2e_run(e, in, n, BZ_RUN);
}

int64_t trc_bz2e_finish(int64_t h) {
    trc_bz2e *e = (trc_bz2e *)(intptr_t)h;
    if (!e) return -7;
    if (e->failed) return e->failed;
    if (e->done) return 1;
    return trc_bz2e_run(e, "", 0, BZ_FINISH);
}

int64_t trc_bz2e_avail(int64_t h) {
    trc_bz2e *e = (trc_bz2e *)(intptr_t)h;
    return e ? (int64_t)e->out.len : 0;
}

void trc_bz2e_take(int64_t h, char *dst) {
    trc_bz2e *e = (trc_bz2e *)(intptr_t)h;
    if (!e) return;
    if (e->out.len) memcpy(dst, e->out.p, e->out.len);
    e->out.len = 0;
}

void trc_bz2e_free(int64_t h) {
    trc_bz2e *e = (trc_bz2e *)(intptr_t)h;
    if (!e) return;
    BZ2_bzCompressEnd(&e->z);
    free(e->out.p);
    free(e);
}
