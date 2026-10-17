/*
 * std/net/h2_native.c -- HTTP/2 (RFC 9113) client and server, HPACK (RFC 7541).
 *
 * Linked through `# tauraro-cpp-shim: h2_native.c` in std/net/http2.tr.
 * Uses std/net/tls_native.c for https (ALPN "h2"); cleartext connections use
 * prior knowledge (h2c without Upgrade).
 *
 * Header lists cross the Tauraro boundary as a "blob": name NUL value NUL
 * name NUL value NUL ... (HTTP/2 forbids NUL in names and values).
 *
 * The client is single-threaded and event driven: requests are submitted
 * (HEADERS + DATA, respecting flow control), then trc_h2_run() reads and
 * dispatches frames until the wanted streams finish. Many streams can be in
 * flight at once on one connection (multiplexing).
 */
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
typedef SOCKET h2_sock;
#  define H2_BADSOCK INVALID_SOCKET
#  define h2_closesock closesocket
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <poll.h>
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <time.h>
typedef int h2_sock;
#  define H2_BADSOCK (-1)
#  define h2_closesock close
#endif

typedef uint8_t u8;
typedef uint32_t u32;

/* tls_native.c */
void* trc_tls_connect(const char* host, int64_t hostlen, int64_t port, void* store, int64_t verify, const char* alpn, int64_t timeout_ms);
int64_t trc_tls_ok(void* h);
int64_t trc_tls_error(void* h, char* out, int64_t cap);
void trc_tls_set_timeout(void* h, int64_t ms);
int64_t trc_tls_write(void* h, const char* d, int64_t n);
int64_t trc_tls_read(void* h, char* out, int64_t cap);
int64_t trc_tls_alpn(void* h, char* out, int64_t cap);
void trc_tls_close(void* h);
void* trc_tls_server_fd(int64_t fd, void* cfg, int64_t timeout_ms, int64_t owns_fd);
int64_t trc_tls_fd(void* h);

/* RFC 7541 Appendix B (Huffman code) and Appendix A (static table). */
static const uint32_t hp_huff_code[257] = {
  0x1ff8,0x7fffd8,0xfffffe2,0xfffffe3,0xfffffe4,0xfffffe5,0xfffffe6,0xfffffe7,
  0xfffffe8,0xffffea,0x3ffffffc,0xfffffe9,0xfffffea,0x3ffffffd,0xfffffeb,0xfffffec,
  0xfffffed,0xfffffee,0xfffffef,0xffffff0,0xffffff1,0xffffff2,0x3ffffffe,0xffffff3,
  0xffffff4,0xffffff5,0xffffff6,0xffffff7,0xffffff8,0xffffff9,0xffffffa,0xffffffb,
  0x14,0x3f8,0x3f9,0xffa,0x1ff9,0x15,0xf8,0x7fa,
  0x3fa,0x3fb,0xf9,0x7fb,0xfa,0x16,0x17,0x18,
  0x0,0x1,0x2,0x19,0x1a,0x1b,0x1c,0x1d,
  0x1e,0x1f,0x5c,0xfb,0x7ffc,0x20,0xffb,0x3fc,
  0x1ffa,0x21,0x5d,0x5e,0x5f,0x60,0x61,0x62,
  0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,
  0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,
  0xfc,0x73,0xfd,0x1ffb,0x7fff0,0x1ffc,0x3ffc,0x22,
  0x7ffd,0x3,0x23,0x4,0x24,0x5,0x25,0x26,
  0x27,0x6,0x74,0x75,0x28,0x29,0x2a,0x7,
  0x2b,0x76,0x2c,0x8,0x9,0x2d,0x77,0x78,
  0x79,0x7a,0x7b,0x7ffe,0x7fc,0x3ffd,0x1ffd,0xffffffc,
  0xfffe6,0x3fffd2,0xfffe7,0xfffe8,0x3fffd3,0x3fffd4,0x3fffd5,0x7fffd9,
  0x3fffd6,0x7fffda,0x7fffdb,0x7fffdc,0x7fffdd,0x7fffde,0xffffeb,0x7fffdf,
  0xffffec,0xffffed,0x3fffd7,0x7fffe0,0xffffee,0x7fffe1,0x7fffe2,0x7fffe3,
  0x7fffe4,0x1fffdc,0x3fffd8,0x7fffe5,0x3fffd9,0x7fffe6,0x7fffe7,0xffffef,
  0x3fffda,0x1fffdd,0xfffe9,0x3fffdb,0x3fffdc,0x7fffe8,0x7fffe9,0x1fffde,
  0x7fffea,0x3fffdd,0x3fffde,0xfffff0,0x1fffdf,0x3fffdf,0x7fffeb,0x7fffec,
  0x1fffe0,0x1fffe1,0x3fffe0,0x1fffe2,0x7fffed,0x3fffe1,0x7fffee,0x7fffef,
  0xfffea,0x3fffe2,0x3fffe3,0x3fffe4,0x7ffff0,0x3fffe5,0x3fffe6,0x7ffff1,
  0x3ffffe0,0x3ffffe1,0xfffeb,0x7fff1,0x3fffe7,0x7ffff2,0x3fffe8,0x1ffffec,
  0x3ffffe2,0x3ffffe3,0x3ffffe4,0x7ffffde,0x7ffffdf,0x3ffffe5,0xfffff1,0x1ffffed,
  0x7fff2,0x1fffe3,0x3ffffe6,0x7ffffe0,0x7ffffe1,0x3ffffe7,0x7ffffe2,0xfffff2,
  0x1fffe4,0x1fffe5,0x3ffffe8,0x3ffffe9,0xffffffd,0x7ffffe3,0x7ffffe4,0x7ffffe5,
  0xfffec,0xfffff3,0xfffed,0x1fffe6,0x3fffe9,0x1fffe7,0x1fffe8,0x7ffff3,
  0x3fffea,0x3fffeb,0x1ffffee,0x1ffffef,0xfffff4,0xfffff5,0x3ffffea,0x7ffff4,
  0x3ffffeb,0x7ffffe6,0x3ffffec,0x3ffffed,0x7ffffe7,0x7ffffe8,0x7ffffe9,0x7ffffea,
  0x7ffffeb,0xffffffe,0x7ffffec,0x7ffffed,0x7ffffee,0x7ffffef,0x7fffff0,0x3ffffee,
  0x3fffffff,
};
static const uint8_t hp_huff_len[257] = {
  13,23,28,28,28,28,28,28,28,24,30,28,28,30,28,28,
  28,28,28,28,28,28,30,28,28,28,28,28,28,28,28,28,
  6,10,10,12,13,6,8,11,10,10,8,11,8,6,6,6,
  5,5,5,6,6,6,6,6,6,6,7,8,15,6,12,10,
  13,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,8,7,8,13,19,13,14,6,
  15,5,6,5,6,5,6,6,6,5,7,7,6,6,6,5,
  6,7,6,5,5,6,7,7,7,7,7,15,11,14,13,28,
  20,22,20,20,22,22,22,23,22,23,23,23,23,23,24,23,
  24,24,22,23,24,23,23,23,23,21,22,23,22,23,23,24,
  22,21,20,22,22,23,23,21,23,22,22,24,21,22,23,23,
  21,21,22,21,23,22,23,23,20,22,22,22,23,22,22,23,
  26,26,20,19,22,23,22,25,26,26,26,27,27,26,24,25,
  19,21,26,27,27,26,27,24,21,21,26,26,28,27,27,27,
  20,24,20,21,22,21,21,23,22,22,25,25,24,24,26,23,
  26,27,26,26,27,27,27,27,27,28,27,27,27,27,27,26,
  30,
};
static const char* const hp_static[61][2] = {
  {":authority", ""},
  {":method", "GET"},
  {":method", "POST"},
  {":path", "/"},
  {":path", "/index.html"},
  {":scheme", "http"},
  {":scheme", "https"},
  {":status", "200"},
  {":status", "204"},
  {":status", "206"},
  {":status", "304"},
  {":status", "400"},
  {":status", "404"},
  {":status", "500"},
  {"accept-charset", ""},
  {"accept-encoding", "gzip, deflate"},
  {"accept-language", ""},
  {"accept-ranges", ""},
  {"accept", ""},
  {"access-control-allow-origin", ""},
  {"age", ""},
  {"allow", ""},
  {"authorization", ""},
  {"cache-control", ""},
  {"content-disposition", ""},
  {"content-encoding", ""},
  {"content-language", ""},
  {"content-length", ""},
  {"content-location", ""},
  {"content-range", ""},
  {"content-type", ""},
  {"cookie", ""},
  {"date", ""},
  {"etag", ""},
  {"expect", ""},
  {"expires", ""},
  {"from", ""},
  {"host", ""},
  {"if-match", ""},
  {"if-modified-since", ""},
  {"if-none-match", ""},
  {"if-range", ""},
  {"if-unmodified-since", ""},
  {"last-modified", ""},
  {"link", ""},
  {"location", ""},
  {"max-forwards", ""},
  {"proxy-authenticate", ""},
  {"proxy-authorization", ""},
  {"range", ""},
  {"referer", ""},
  {"refresh", ""},
  {"retry-after", ""},
  {"server", ""},
  {"set-cookie", ""},
  {"strict-transport-security", ""},
  {"transfer-encoding", ""},
  {"user-agent", ""},
  {"vary", ""},
  {"via", ""},
  {"www-authenticate", ""},
};

/* ═══════════════════════════ growable buffer ═════════════════════════════ */
typedef struct { u8* p; size_t n, cap; } hbuf;
static int hb_put(hbuf* b, const void* d, size_t n) {
    if (b->n + n > b->cap) {
        size_t nc = b->cap ? b->cap : 256; u8* np;
        while (nc < b->n + n) nc *= 2;
        np = (u8*)realloc(b->p, nc);
        if (!np) return -1;
        b->p = np; b->cap = nc;
    }
    if (n) memcpy(b->p + b->n, d, n);
    b->n += n;
    return 0;
}
static int hb_byte(hbuf* b, int c) { u8 x = (u8)c; return hb_put(b, &x, 1); }
static void hb_free(hbuf* b) { free(b->p); b->p = NULL; b->n = b->cap = 0; }

/* ═══════════════════════════ Huffman ═════════════════════════════════════ */
/* Decoding tree: node i has children tree[i][0/1]; 0xffff = no child,
 * values with bit 15 set are leaves (symbol = v & 0x7fff). Built once. */
#define HP_NONE 0xffff
static uint16_t hp_tree[512][2]; static int hp_tree_n = 0;
static volatile int hp_tree_ready = 0;
static void hp_build_tree(void) {
    int s, nodes = 1, i;
    if (hp_tree_ready) return;
    for (i = 0; i < 512; i++) hp_tree[i][0] = hp_tree[i][1] = HP_NONE;
    for (s = 0; s < 257; s++) {
        int cur = 0, b;
        for (b = hp_huff_len[s] - 1; b >= 0; b--) {
            int bit = (int)((hp_huff_code[s] >> b) & 1);
            if (b == 0) { hp_tree[cur][bit] = (uint16_t)(0x8000 | s); break; }
            if (hp_tree[cur][bit] == HP_NONE) hp_tree[cur][bit] = (uint16_t)nodes++;
            cur = hp_tree[cur][bit];
        }
    }
    hp_tree_n = nodes;
    hp_tree_ready = 1;
}
static int huff_decode(const u8* in, size_t n, hbuf* out) {
    int cur = 0, depth = 0, ones = 1; size_t i; int b;
    hp_build_tree();
    for (i = 0; i < n; i++) {
        for (b = 7; b >= 0; b--) {
            int bit = (in[i] >> b) & 1, nx = hp_tree[cur][bit];
            ones = ones && bit; depth++;
            if (nx == HP_NONE) return -1;
            if (nx & 0x8000) {
                int sym = nx & 0x7fff;
                if (sym == 256) return -1;   /* EOS in the string is an error */
                if (hb_byte(out, sym) < 0) return -1;
                cur = 0; depth = 0; ones = 1;
            } else {
                cur = nx;
            }
        }
    }
    /* padding: at most 7 bits, all ones (a prefix of EOS) */
    if (depth > 7 || !ones) return -1;
    return 0;
}
static size_t huff_len(const u8* s, size_t n) { size_t bits = 0, i; for (i = 0; i < n; i++) bits += hp_huff_len[s[i]]; return (bits + 7) / 8; }
static int huff_encode(const u8* s, size_t n, hbuf* out) {
    uint64_t acc = 0; int nb = 0; size_t i;
    for (i = 0; i < n; i++) {
        acc = (acc << hp_huff_len[s[i]]) | hp_huff_code[s[i]]; nb += hp_huff_len[s[i]];
        while (nb >= 8) { nb -= 8; if (hb_byte(out, (int)(acc >> nb) & 0xff) < 0) return -1; }
    }
    if (nb > 0) { acc = (acc << (8 - nb)) | ((1u << (8 - nb)) - 1); if (hb_byte(out, (int)acc & 0xff) < 0) return -1; }
    return 0;
}

/* ═══════════════════════════ HPACK ═══════════════════════════════════════ */
typedef struct { char* name; char* value; size_t nl, vl; } hp_entry;
typedef struct {
    hp_entry* e; int count, cap, head;   /* ring: e[(head + i) % cap] is the i-th newest */
    size_t size, max_size;               /* current / allowed table size (octets) */
    size_t settings_max;                 /* bound from SETTINGS_HEADER_TABLE_SIZE */
    int pending_update;                  /* encoder: must emit a size update */
} hp_table;

static void hp_evict_one(hp_table* t) {
    hp_entry* o = &t->e[(t->head + t->count - 1) % t->cap];
    t->size -= o->nl + o->vl + 32;
    free(o->name); free(o->value); o->name = o->value = NULL;
    t->count--;
}
static void hp_set_max(hp_table* t, size_t m) { t->max_size = m; while (t->size > t->max_size && t->count > 0) hp_evict_one(t); }
static int hp_add(hp_table* t, const char* n, size_t nl, const char* v, size_t vl) {
    size_t sz = nl + vl + 32; hp_entry* e;
    while (t->count > 0 && t->size + sz > t->max_size) hp_evict_one(t);
    if (sz > t->max_size) return 0;    /* too big: table is now empty, entry not added */
    if (t->count == t->cap) {
        int nc = t->cap ? t->cap * 2 : 32, i; hp_entry* ne = (hp_entry*)calloc((size_t)nc, sizeof(hp_entry));
        if (!ne) return -1;
        for (i = 0; i < t->count; i++) ne[i] = t->e[(t->head + i) % t->cap];
        free(t->e); t->e = ne; t->cap = nc; t->head = 0;
    }
    t->head = (t->head + t->cap - 1) % t->cap;
    e = &t->e[t->head];
    e->name = (char*)malloc(nl + 1); e->value = (char*)malloc(vl + 1);
    if (!e->name || !e->value) return -1;
    memcpy(e->name, n, nl); e->name[nl] = 0; memcpy(e->value, v, vl); e->value[vl] = 0;
    e->nl = nl; e->vl = vl;
    t->count++; t->size += sz;
    return 0;
}
static void hp_free(hp_table* t) { while (t->count) hp_evict_one(t); free(t->e); t->e = NULL; t->cap = 0; }
/* 1-based HPACK index -> name/value */
static int hp_get(const hp_table* t, uint64_t idx, const char** n, size_t* nl, const char** v, size_t* vl) {
    if (idx == 0) return -1;
    if (idx <= 61) { *n = hp_static[idx - 1][0]; *v = hp_static[idx - 1][1]; *nl = strlen(*n); *vl = strlen(*v); return 0; }
    idx -= 62;
    if (idx >= (uint64_t)t->count) return -1;
    { const hp_entry* e = &t->e[(t->head + (int)idx) % t->cap]; *n = e->name; *nl = e->nl; *v = e->value; *vl = e->vl; }
    return 0;
}

static int hp_int_decode(const u8** p, const u8* end, int prefix, uint64_t* out) {
    uint64_t v; int m = 0; u8 mask = (u8)((1u << prefix) - 1);
    if (*p >= end) return -1;
    v = **p & mask; (*p)++;
    if (v < mask) { *out = v; return 0; }
    while (*p < end) {
        u8 b = **p; (*p)++;
        if (m > 56) return -1;
        v += (uint64_t)(b & 0x7f) << m; m += 7;
        if (!(b & 0x80)) { *out = v; return 0; }
    }
    return -1;
}
static int hp_str_decode(const u8** p, const u8* end, hbuf* out) {
    int huff; uint64_t len;
    if (*p >= end) return -1;
    huff = (**p & 0x80) != 0;
    if (hp_int_decode(p, end, 7, &len) < 0 || len > (uint64_t)(end - *p)) return -1;
    if (huff) { if (huff_decode(*p, (size_t)len, out) < 0) return -1; }
    else if (hb_put(out, *p, (size_t)len) < 0) return -1;
    *p += len;
    return 0;
}
/* Decode a header block into a blob. Returns 0 or -1 (COMPRESSION_ERROR). */
static int hp_decode(hp_table* t, const u8* in, size_t n, hbuf* blob, size_t max_list) {
    const u8* p = in; const u8* end = in + n; int seen_header = 0; size_t list = 0;
    while (p < end) {
        u8 b = *p;
        if (b & 0x80) {
            uint64_t idx; const char *nm, *vv; size_t nl, vl;
            if (hp_int_decode(&p, end, 7, &idx) < 0 || hp_get(t, idx, &nm, &nl, &vv, &vl) < 0) return -1;
            if (hb_put(blob, nm, nl) < 0 || hb_byte(blob, 0) < 0 || hb_put(blob, vv, vl) < 0 || hb_byte(blob, 0) < 0) return -1;
            list += nl + vl + 32; seen_header = 1;
        } else if ((b & 0xe0) == 0x20) {
            uint64_t sz;
            if (seen_header) return -1;   /* size updates must come first */
            if (hp_int_decode(&p, end, 5, &sz) < 0 || sz > t->settings_max) return -1;
            hp_set_max(t, (size_t)sz);
        } else {
            int prefix = (b & 0x40) ? 6 : 4, index = (b & 0x40) != 0; uint64_t idx;
            hbuf name = {0}, value = {0}; int rc = -1;
            if (hp_int_decode(&p, end, prefix, &idx) < 0) return -1;
            if (idx) {
                const char *nm, *vv; size_t nl, vl;
                if (hp_get(t, idx, &nm, &nl, &vv, &vl) < 0) return -1;
                if (hb_put(&name, nm, nl) < 0) goto lit_out;
            } else if (hp_str_decode(&p, end, &name) < 0) goto lit_out;
            if (hp_str_decode(&p, end, &value) < 0) goto lit_out;
            if (hb_put(blob, name.p, name.n) < 0 || hb_byte(blob, 0) < 0 || hb_put(blob, value.p, value.n) < 0 || hb_byte(blob, 0) < 0) goto lit_out;
            if (index && hp_add(t, (const char*)name.p, name.n, (const char*)value.p, value.n) < 0) goto lit_out;
            list += name.n + value.n + 32; seen_header = 1;
            rc = 0;
        lit_out:
            hb_free(&name); hb_free(&value);
            if (rc < 0) return -1;
        }
        if (max_list && list > max_list) return -1;
    }
    return 0;
}

static int hp_int_encode(hbuf* o, int first, int prefix, uint64_t v) {
    uint64_t mask = (1u << prefix) - 1;
    if (v < mask) return hb_byte(o, first | (int)v);
    if (hb_byte(o, first | (int)mask) < 0) return -1;
    v -= mask;
    while (v >= 128) { if (hb_byte(o, (int)(v & 0x7f) | 0x80) < 0) return -1; v >>= 7; }
    return hb_byte(o, (int)v);
}
static int hp_str_encode(hbuf* o, const u8* s, size_t n) {
    size_t hl = huff_len(s, n);
    if (hl < n) { if (hp_int_encode(o, 0x80, 7, hl) < 0) return -1; return huff_encode(s, n, o); }
    if (hp_int_encode(o, 0, 7, n) < 0) return -1;
    return hb_put(o, s, n);
}
static int lc_eq(const char* a, size_t an, const char* b) { return strlen(b) == an && memcmp(a, b, an) == 0; }
static int hp_sensitive(const char* n, size_t nl) {
    return lc_eq(n, nl, "authorization") || lc_eq(n, nl, "proxy-authorization") || lc_eq(n, nl, "cookie") || lc_eq(n, nl, "set-cookie");
}
/* Encode one header (lower-case name expected). */
static int hp_encode_one(hp_table* t, hbuf* o, const char* n, size_t nl, const char* v, size_t vl) {
    int i, name_idx = 0;
    for (i = 0; i < 61; i++) {
        if (lc_eq(n, nl, hp_static[i][0])) {
            if (!name_idx) name_idx = i + 1;
            if (lc_eq(v, vl, hp_static[i][1])) return hp_int_encode(o, 0x80, 7, (uint64_t)(i + 1));
        }
    }
    for (i = 0; i < t->count; i++) {
        const hp_entry* e = &t->e[(t->head + i) % t->cap];
        if (e->nl == nl && memcmp(e->name, n, nl) == 0) {
            if (!name_idx) name_idx = 62 + i;
            if (e->vl == vl && memcmp(e->value, v, vl) == 0) return hp_int_encode(o, 0x80, 7, (uint64_t)(62 + i));
        }
    }
    if (hp_sensitive(n, nl)) {
        /* never indexed (RFC 7541 section 7.1.3) */
        if (hp_int_encode(o, 0x10, 4, (uint64_t)name_idx) < 0) return -1;
        if (!name_idx && hp_str_encode(o, (const u8*)n, nl) < 0) return -1;
        return hp_str_encode(o, (const u8*)v, vl);
    }
    if (hp_int_encode(o, 0x40, 6, (uint64_t)name_idx) < 0) return -1;
    if (!name_idx && hp_str_encode(o, (const u8*)n, nl) < 0) return -1;
    if (hp_str_encode(o, (const u8*)v, vl) < 0) return -1;
    return hp_add(t, n, nl, v, vl);
}
static int hp_encode_blob(hp_table* t, hbuf* o, const u8* blob, size_t n) {
    size_t k = 0;
    if (t->pending_update) { if (hp_int_encode(o, 0x20, 5, t->max_size) < 0) return -1; t->pending_update = 0; }
    while (k < n) {
        const char* nm = (const char*)blob + k; size_t nl = strnlen(nm, n - k), vk; const char* vv; size_t vl;
        if (k + nl >= n) return -1;
        vk = k + nl + 1; vv = (const char*)blob + vk; vl = strnlen(vv, n - vk);
        if (vk + vl >= n + 1) return -1;
        {   /* lower-case the name */
            char small[256]; char* lname = nl < sizeof small ? small : (char*)malloc(nl + 1); size_t i; int r;
            if (!lname) return -1;
            for (i = 0; i < nl; i++) lname[i] = (nm[i] >= 'A' && nm[i] <= 'Z') ? (char)(nm[i] + 32) : nm[i];
            r = hp_encode_one(t, o, lname, nl, vv, vl);
            if (lname != small) free(lname);
            if (r < 0) return -1;
        }
        k = vk + vl + 1;
    }
    return 0;
}

/* Standalone HPACK objects for std.net.http2's Hpack class. */
void* trc_hpack_new(int64_t max_size) {
    hp_table* t = (hp_table*)calloc(1, sizeof(hp_table));
    if (!t) return NULL;
    t->max_size = t->settings_max = (size_t)(max_size > 0 ? max_size : 4096);
    return t;
}
void trc_hpack_free(void* h) { if (h) { hp_free((hp_table*)h); free(h); } }
/* Encoder: shrink/grow the table and announce it in the next block. */
void trc_hpack_set_max(void* h, int64_t m) { hp_table* t = (hp_table*)h; hp_set_max(t, (size_t)m); t->settings_max = (size_t)m; t->pending_update = 1; }
int64_t trc_hpack_table_size(void* h) { return h ? (int64_t)((hp_table*)h)->size : 0; }
int64_t trc_hpack_table_count(void* h) { return h ? ((hp_table*)h)->count : 0; }
/* Encode a blob into out. Returns the length, -1 on error, or -2-len if out is too small. */
int64_t trc_hpack_encode(void* h, const char* blob, int64_t n, char* out, int64_t cap) {
    hbuf o = {0}; int64_t len;
    if (hp_encode_blob((hp_table*)h, &o, (const u8*)blob, (size_t)n) < 0) { hb_free(&o); return -1; }
    len = (int64_t)o.n;
    if (len > cap) { hb_free(&o); return -2 - len; }
    if (len) memcpy(out, o.p, (size_t)len);
    hb_free(&o);
    return len;
}
int64_t trc_hpack_decode(void* h, const char* in, int64_t n, char* out, int64_t cap) {
    hbuf blob = {0}; int64_t len;
    if (hp_decode((hp_table*)h, (const u8*)in, (size_t)n, &blob, 0) < 0) { hb_free(&blob); return -1; }
    len = (int64_t)blob.n;
    if (len > cap) { hb_free(&blob); return -2 - len; }
    if (len) memcpy(out, blob.p, (size_t)len);
    hb_free(&blob);
    return len;
}
int64_t trc_huffman_encode(const char* s, int64_t n, char* out, int64_t cap) {
    hbuf o = {0}; int64_t len;
    if (huff_encode((const u8*)s, (size_t)n, &o) < 0) { hb_free(&o); return -1; }
    len = (int64_t)o.n; if (len > cap) { hb_free(&o); return -1; }
    if (len) memcpy(out, o.p, (size_t)len);
    hb_free(&o); return len;
}
int64_t trc_huffman_decode(const char* s, int64_t n, char* out, int64_t cap) {
    hbuf o = {0}; int64_t len;
    if (huff_decode((const u8*)s, (size_t)n, &o) < 0) { hb_free(&o); return -1; }
    len = (int64_t)o.n; if (len > cap) { hb_free(&o); return -1; }
    if (len) memcpy(out, o.p, (size_t)len);
    hb_free(&o); return len;
}

/* ═══════════════════════════ HTTP/2 connection ═══════════════════════════ */
enum { F_DATA = 0, F_HEADERS = 1, F_PRIORITY = 2, F_RST = 3, F_SETTINGS = 4, F_PUSH = 5, F_PING = 6, F_GOAWAY = 7, F_WINUP = 8, F_CONT = 9 };
#define FL_END_STREAM 0x1
#define FL_ACK 0x1
#define FL_END_HEADERS 0x4
#define FL_PADDED 0x8
#define FL_PRIORITY 0x20
#define H2_WINDOW (16 * 1024 * 1024)        /* client: stream + connection receive windows */
#define H2S_STREAM_WIN (1024 * 1024)        /* server: per-stream window, replenished as the app reads */
#define H2S_CONN_WIN (16 * 1024 * 1024)     /* server: connection window, replenished on receipt */
#define H2_IN_LIMIT (64u * 1024u * 1024u)   /* cap on buffered, unprocessed input */

struct h2srv;

typedef struct {
    u32 id; int state;            /* client: 0 open, 1 half-closed (local), 2 closed, 3 reset/error */
    int status;
    hbuf headers, trailers, body; /* received header block, trailers, unread DATA */
    int got_headers;
    int64_t send_win;
    hbuf out; size_t out_off; int out_end;      /* pending outgoing DATA; out_end: END_STREAM after it */
    int req_end_sent;                            /* END_STREAM sent by us */
    hbuf out_trailers; int has_out_trailers;     /* server: trailers to send after the data */
    u32 err_code;
    char err[128];
    int used;
    int64_t recv_unacked;
    /* server side */
    int remote_end, reset, handed, resp_sent, released;
    int64_t recv_win, consumed;
} h2_stream;

typedef struct {
    void* tls; h2_sock fd;
    int timeout_ms;
    char err[256];
    int dead;
    hp_table enc, dec;
    u32 peer_max_frame, peer_max_concurrent; int64_t peer_init_win; int got_settings;
    int64_t conn_send_win; int64_t conn_recv_unacked;
    h2_stream* st; int nst, cap;
    u32 next_id;
    u32 goaway_last; int goaway;
    hbuf in; size_t in_off;       /* received bytes; frames are parsed from in.p + in_off */
    hbuf pend;                    /* bytes read while a write was blocked (appended to `in` later) */
    hbuf hdr_acc; u32 hdr_stream; int hdr_end_stream; int in_headers;   /* HEADERS + CONTINUATION */
    char authority[300]; int is_tls;
    /* server side */
    int is_server; int64_t cid; int preface; u32 last_peer; int goaway_sent; int max_streams;
    int64_t conn_recv_win;
    struct h2srv* srv;
} h2_conn;

static void h2_fail(h2_conn* c, const char* m) { if (!c->dead) { snprintf(c->err, sizeof c->err, "%s", m); c->dead = 1; } }

/* ── transport ── */
static int sock_wait(h2_sock fd, int write, int ms) {
#if defined(_WIN32)
    fd_set s; struct timeval tv; FD_ZERO(&s); FD_SET(fd, &s);
    tv.tv_sec = ms / 1000; tv.tv_usec = (ms % 1000) * 1000;
    return select(0, write ? NULL : &s, write ? &s : NULL, NULL, ms < 0 ? NULL : &tv);
#else
    struct pollfd p; int r;
    p.fd = fd; p.events = write ? POLLOUT : POLLIN; p.revents = 0;
    do { r = poll(&p, 1, ms); } while (r < 0 && errno == EINTR);
    return r;
#endif
}
/* Write all of d. On plain sockets the write never blocks blindly: while the
 * peer is not draining our data, its frames are read into `pend`, so two
 * peers that both write large bodies cannot deadlock. */
static int tx(h2_conn* c, const u8* d, size_t n) {
    if (c->dead) return -1;
    if (c->tls) { if (trc_tls_write(c->tls, (const char*)d, (int64_t)n) < 0) { h2_fail(c, "HTTP/2: write failed"); return -1; } return 0; }
    while (n > 0) {
        long r;
#if defined(MSG_DONTWAIT) && defined(MSG_NOSIGNAL)
        r = (long)send(c->fd, (const char*)d, (int)n, MSG_NOSIGNAL | MSG_DONTWAIT);
        if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            struct pollfd p; int pr;
            p.fd = c->fd; p.events = POLLOUT | POLLIN; p.revents = 0;
            do { pr = poll(&p, 1, c->timeout_ms > 0 ? c->timeout_ms : -1); } while (pr < 0 && errno == EINTR);
            if (pr == 0) { h2_fail(c, "HTTP/2: write timed out"); return -1; }
            if (pr > 0 && (p.revents & POLLIN) && c->pend.n + c->in.n < H2_IN_LIMIT) {
                u8 tmp[16384]; long k = (long)recv(c->fd, (char*)tmp, sizeof tmp, MSG_DONTWAIT);
                if (k > 0 && hb_put(&c->pend, tmp, (size_t)k) < 0) { h2_fail(c, "HTTP/2: out of memory"); return -1; }
                if (k == 0) { h2_fail(c, "HTTP/2: connection closed by peer"); return -1; }
            }
            continue;
        }
#elif defined(MSG_NOSIGNAL)
        r = (long)send(c->fd, (const char*)d, (int)n, MSG_NOSIGNAL);
#else
        r = (long)send(c->fd, (const char*)d, (int)n, 0);
#endif
        if (r <= 0) {
#if !defined(_WIN32)
            if (r < 0 && errno == EINTR) continue;
#endif
            h2_fail(c, "HTTP/2: write failed"); return -1;
        }
        d += r; n -= (size_t)r;
    }
    return 0;
}
/* Read more bytes into `in`. 1 ok, 0 EOF, -2 timeout, -1 error. */
static int rx(h2_conn* c, int ms) {
    long r; size_t room;
    if (c->in_off > 0 && c->in_off == c->in.n) { c->in.n = 0; c->in_off = 0; }
    if (c->pend.n > 0) {
        if (hb_put(&c->in, c->pend.p, c->pend.n) < 0) { h2_fail(c, "HTTP/2: out of memory"); return -1; }
        c->pend.n = 0; return 1;
    }
    if (c->in_off > 0) { memmove(c->in.p, c->in.p + c->in_off, c->in.n - c->in_off); c->in.n -= c->in_off; c->in_off = 0; }
    if (c->in.n >= H2_IN_LIMIT) { h2_fail(c, "HTTP/2: input buffer limit"); return -1; }
    if (c->in.cap - c->in.n < 20000) {
        size_t nc = c->in.cap ? c->in.cap : 32768; u8* np;
        while (nc - c->in.n < 20000) nc *= 2;
        np = (u8*)realloc(c->in.p, nc);
        if (!np) { h2_fail(c, "HTTP/2: out of memory"); return -1; }
        c->in.p = np; c->in.cap = nc;
    }
    room = c->in.cap - c->in.n;
    if (c->tls) {
        trc_tls_set_timeout(c->tls, ms > 0 ? ms : 0);
        r = (long)trc_tls_read(c->tls, (char*)c->in.p + c->in.n, (int64_t)room);
        if (r == -2) return -2;
        if (r < 0) { char e[200]; int64_t n = trc_tls_error(c->tls, e, sizeof e - 1); e[n] = 0; h2_fail(c, n ? e : "HTTP/2: read failed"); return -1; }
        if (r == 0) return 0;
    } else {
        if (ms > 0 && sock_wait(c->fd, 0, ms) == 0) return -2;
        r = (long)recv(c->fd, (char*)c->in.p + c->in.n, (int)room, 0);
        if (r < 0) {
#if !defined(_WIN32)
            if (errno == EINTR || errno == EAGAIN) return 1;
#endif
            h2_fail(c, "HTTP/2: read failed"); return -1;
        }
        if (r == 0) return 0;
    }
    c->in.n += (size_t)r;
    return 1;
}

static int send_frame(h2_conn* c, int type, int flags, u32 sid, const u8* payload, size_t n) {
    u8 h[9];
    h[0] = (u8)(n >> 16); h[1] = (u8)(n >> 8); h[2] = (u8)n; h[3] = (u8)type; h[4] = (u8)flags;
    h[5] = (u8)((sid >> 24) & 0x7f); h[6] = (u8)(sid >> 16); h[7] = (u8)(sid >> 8); h[8] = (u8)sid;
    if (n <= 4096) {
        u8 buf[9 + 4096];
        memcpy(buf, h, 9); if (n) memcpy(buf + 9, payload, n);
        return tx(c, buf, 9 + n);
    }
    if (tx(c, h, 9) < 0) return -1;
    return tx(c, payload, n);
}
static int send_goaway(h2_conn* c, u32 code) {
    u8 p[8]; u32 last = c->is_server ? c->last_peer : 0;
    p[0] = (u8)((last >> 24) & 0x7f); p[1] = (u8)(last >> 16); p[2] = (u8)(last >> 8); p[3] = (u8)last;
    p[4] = (u8)(code >> 24); p[5] = (u8)(code >> 16); p[6] = (u8)(code >> 8); p[7] = (u8)code;
    return send_frame(c, F_GOAWAY, 0, 0, p, 8);
}
static int send_winup(h2_conn* c, u32 sid, u32 inc) {
    u8 p[4]; p[0] = (u8)((inc >> 24) & 0x7f); p[1] = (u8)(inc >> 16); p[2] = (u8)(inc >> 8); p[3] = (u8)inc;
    return send_frame(c, F_WINUP, 0, sid, p, 4);
}
static int send_rst(h2_conn* c, u32 sid, u32 code) {
    u8 p[4]; p[0] = (u8)(code >> 24); p[1] = (u8)(code >> 16); p[2] = (u8)(code >> 8); p[3] = (u8)code;
    return send_frame(c, F_RST, 0, sid, p, 4);
}
static void conn_error(h2_conn* c, u32 code, const char* m) {
    if (!c->dead) send_goaway(c, code);
    h2_fail(c, m);
}
/* Encode a header blob and send it as HEADERS (+ CONTINUATION). */
static int send_header_block(h2_conn* c, u32 sid, const u8* blob, size_t n, int end_stream) {
    hbuf block = {0}; size_t off = 0; int first = 1; u32 maxf = c->peer_max_frame > 16384 ? 16384 : c->peer_max_frame;
    if (hp_encode_blob(&c->enc, &block, blob, n) < 0) { hb_free(&block); h2_fail(c, "HTTP/2: header encoding failed"); return -1; }
    for (;;) {
        size_t chunk = block.n - off; int fl = 0;
        if (chunk > maxf) chunk = maxf;
        if (off + chunk == block.n) fl |= FL_END_HEADERS;
        if (first && end_stream) fl |= FL_END_STREAM;
        if (send_frame(c, first ? F_HEADERS : F_CONT, fl, sid, block.p + off, chunk) < 0) { hb_free(&block); return -1; }
        off += chunk; first = 0;
        if (off >= block.n) break;
    }
    hb_free(&block);
    return 0;
}

static h2_stream* find_stream(h2_conn* c, u32 id) {
    int i;
    for (i = 0; i < c->nst; i++) if (c->st[i].used && c->st[i].id == id) return &c->st[i];
    return NULL;
}
static void stream_free_data(h2_stream* s) {
    hb_free(&s->headers); hb_free(&s->trailers); hb_free(&s->body); hb_free(&s->out); hb_free(&s->out_trailers);
}
static h2_stream* new_stream(h2_conn* c) {
    int i; h2_stream* s = NULL;
    for (i = 0; i < c->nst; i++) if (!c->st[i].used) { s = &c->st[i]; break; }
    if (!s) {
        if (c->nst == c->cap) {
            int nc = c->cap ? c->cap * 2 : 16; h2_stream* ns = (h2_stream*)realloc(c->st, sizeof(h2_stream) * (size_t)nc);
            if (!ns) return NULL;
            c->st = ns; c->cap = nc;
        }
        s = &c->st[c->nst++];
    }
    memset(s, 0, sizeof *s);
    s->used = 1;
    return s;
}
static int active_streams(h2_conn* c) { int i, n = 0; for (i = 0; i < c->nst; i++) if (c->st[i].used && c->st[i].state < 2) n++; return n; }

/* Client: push as much pending request body as the windows allow. */
static int flush_bodies(h2_conn* c) {
    int i;
    for (i = 0; i < c->nst; i++) {
        h2_stream* s = &c->st[i];
        if (!s->used || s->state >= 2 || s->req_end_sent) continue;
        while (s->out_off < s->out.n && c->conn_send_win > 0 && s->send_win > 0) {
            size_t chunk = s->out.n - s->out_off; int fl;
            if (chunk > c->peer_max_frame) chunk = c->peer_max_frame;
            if ((int64_t)chunk > c->conn_send_win) chunk = (size_t)c->conn_send_win;
            if ((int64_t)chunk > s->send_win) chunk = (size_t)s->send_win;
            fl = (s->out_end && s->out_off + chunk == s->out.n) ? FL_END_STREAM : 0;
            if (send_frame(c, F_DATA, fl, s->id, s->out.p + s->out_off, chunk) < 0) return -1;
            s->out_off += chunk; c->conn_send_win -= (int64_t)chunk; s->send_win -= (int64_t)chunk;
            if (fl) { s->req_end_sent = 1; s->state = 1; }
        }
        if (s->out_off == s->out.n) {
            s->out.n = 0; s->out_off = 0;
            if (s->out_end && !s->req_end_sent) {
                if (send_frame(c, F_DATA, FL_END_STREAM, s->id, NULL, 0) < 0) return -1;
                s->req_end_sent = 1; s->state = 1;
            }
        }
        if (s->req_end_sent) hb_free(&s->out);
    }
    return 0;
}

static int apply_settings(h2_conn* c, const u8* p, size_t n) {
    size_t k;
    if (n % 6) { conn_error(c, 6, "HTTP/2: bad SETTINGS frame size"); return -1; }
    for (k = 0; k < n; k += 6) {
        unsigned id = ((unsigned)p[k] << 8) | p[k + 1]; u32 v = ((u32)p[k + 2] << 24) | ((u32)p[k + 3] << 16) | ((u32)p[k + 4] << 8) | p[k + 5];
        switch (id) {
            case 1: { size_t m = v > 65536 ? 65536 : v; hp_set_max(&c->enc, m); c->enc.settings_max = m; c->enc.pending_update = 1; break; }
            case 2: if (v > 1) { conn_error(c, 1, "HTTP/2: bad ENABLE_PUSH"); return -1; } break;
            case 3: c->peer_max_concurrent = v; break;
            case 4: {
                int i; int64_t delta;
                if (v > 0x7fffffff) { conn_error(c, 3, "HTTP/2: bad initial window size"); return -1; }
                delta = (int64_t)v - c->peer_init_win; c->peer_init_win = v;
                for (i = 0; i < c->nst; i++) if (c->st[i].used) c->st[i].send_win += delta;
                break;
            }
            case 5: if (v < 16384 || v > 16777215) { conn_error(c, 1, "HTTP/2: bad max frame size"); return -1; } c->peer_max_frame = v > 16384 ? 16384 : v; break;
            default: break;
        }
    }
    return send_frame(c, F_SETTINGS, FL_ACK, 0, NULL, 0);
}

static int headers_done(h2_conn* c) {
    hbuf blob = {0}; h2_stream* s = find_stream(c, c->hdr_stream); int rc;
    rc = hp_decode(&c->dec, c->hdr_acc.p, c->hdr_acc.n, &blob, 1 << 20);
    c->hdr_acc.n = 0; c->in_headers = 0;
    if (rc < 0) { hb_free(&blob); conn_error(c, 9, "HTTP/2: header compression error"); return -1; }
    if (!s) { hb_free(&blob); return 0; }   /* stream already gone: discard (table state is kept) */
    if (!s->got_headers) {
        /* :status first; 1xx responses are informational and replaced */
        int status = 0; size_t k = 0;
        while (k < blob.n) {
            const char* nm = (const char*)blob.p + k; size_t nl = strlen(nm); const char* v = nm + nl + 1; size_t vl = strlen(v);
            if (nl == 7 && memcmp(nm, ":status", 7) == 0) status = atoi(v);
            k += nl + vl + 2;
        }
        if (status >= 100 && status < 200 && status != 101) { hb_free(&blob); return 0; }
        if (status == 0) { hb_free(&blob); send_rst(c, s->id, 1); s->state = 3; snprintf(s->err, sizeof s->err, "HTTP/2: response without :status"); return 0; }
        s->status = status; s->got_headers = 1;
        hb_free(&s->headers); s->headers = blob;
    } else {
        if (!c->hdr_end_stream) { hb_free(&blob); send_rst(c, s->id, 1); s->state = 3; snprintf(s->err, sizeof s->err, "HTTP/2: trailers without END_STREAM"); return 0; }
        hb_free(&s->trailers); s->trailers = blob;
    }
    if (c->hdr_end_stream && s->state < 2) s->state = 2;
    return 0;
}

/* Frames whose handling is the same on both sides. */
static int process_common(h2_conn* c, int type, int flags, u32 sid, u8* p, size_t n) {
    h2_stream* s;
    switch (type) {
    case F_PRIORITY:
        if (sid == 0) { conn_error(c, 1, "HTTP/2: PRIORITY on stream 0"); return -1; }
        if (n != 5) { conn_error(c, 6, "HTTP/2: bad PRIORITY"); return -1; }
        break;
    case F_SETTINGS:
        if (sid != 0) { conn_error(c, 1, "HTTP/2: SETTINGS on a stream"); return -1; }
        if (flags & FL_ACK) { if (n != 0) { conn_error(c, 6, "HTTP/2: SETTINGS ACK with payload"); return -1; } break; }
        c->got_settings = 1;
        return apply_settings(c, p, n);
    case F_PING:
        if (sid != 0) { conn_error(c, 1, "HTTP/2: PING on a stream"); return -1; }
        if (n != 8) { conn_error(c, 6, "HTTP/2: bad PING"); return -1; }
        if (!(flags & FL_ACK)) return send_frame(c, F_PING, FL_ACK, 0, p, 8);
        break;
    case F_WINUP: {
        u32 inc;
        if (n != 4) { conn_error(c, 6, "HTTP/2: bad WINDOW_UPDATE"); return -1; }
        inc = (((u32)p[0] & 0x7f) << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
        if (sid == 0) {
            if (inc == 0) { conn_error(c, 1, "HTTP/2: zero WINDOW_UPDATE"); return -1; }
            c->conn_send_win += inc;
            if (c->conn_send_win > 0x7fffffff) { conn_error(c, 3, "HTTP/2: flow control window overflow"); return -1; }
        } else if ((s = find_stream(c, sid)) != NULL) {
            if (inc == 0) { send_rst(c, sid, 1); s->state = 3; s->reset = 1; s->err_code = 1; snprintf(s->err, sizeof s->err, "HTTP/2: zero WINDOW_UPDATE"); break; }
            s->send_win += inc;
            if (s->send_win > 0x7fffffff) { send_rst(c, sid, 3); s->state = 3; s->reset = 1; s->err_code = 3; snprintf(s->err, sizeof s->err, "HTTP/2: stream window overflow"); }
        }
        break;
    }
    default: break;   /* unknown frame types are ignored */
    }
    return 0;
}

static int process_frame(h2_conn* c, int type, int flags, u32 sid, u8* p, size_t n) {
    h2_stream* s;
    if (c->in_headers && (type != F_CONT || sid != c->hdr_stream)) { conn_error(c, 1, "HTTP/2: expected CONTINUATION"); return -1; }
    switch (type) {
    case F_DATA: {
        size_t pad = 0;
        if (sid == 0) { conn_error(c, 1, "HTTP/2: DATA on stream 0"); return -1; }
        if (flags & FL_PADDED) { if (n < 1 || p[0] >= n) { conn_error(c, 1, "HTTP/2: bad padding"); return -1; } pad = p[0]; p++; n--; n -= pad; }
        c->conn_recv_unacked += (int64_t)(n + pad + ((flags & FL_PADDED) ? 1 : 0));
        s = find_stream(c, sid);
        if (s && s->state < 2) {
            if (!s->got_headers) { send_rst(c, sid, 1); s->state = 3; snprintf(s->err, sizeof s->err, "HTTP/2: DATA before HEADERS"); break; }
            if (hb_put(&s->body, p, n) < 0) { conn_error(c, 2, "HTTP/2: out of memory"); return -1; }
            s->recv_unacked += (int64_t)(n + pad);
            if (flags & FL_END_STREAM) s->state = 2;
            else if (s->recv_unacked >= H2_WINDOW / 2) { send_winup(c, sid, (u32)s->recv_unacked); s->recv_unacked = 0; }
        }
        if (c->conn_recv_unacked >= H2_WINDOW / 2) { send_winup(c, 0, (u32)c->conn_recv_unacked); c->conn_recv_unacked = 0; }
        break;
    }
    case F_HEADERS: {
        size_t pad = 0;
        if (sid == 0) { conn_error(c, 1, "HTTP/2: HEADERS on stream 0"); return -1; }
        if (flags & FL_PADDED) { if (n < 1) { conn_error(c, 1, "HTTP/2: bad padding"); return -1; } pad = p[0]; p++; n--; }
        if (flags & FL_PRIORITY) { if (n < 5) { conn_error(c, 1, "HTTP/2: bad HEADERS"); return -1; } p += 5; n -= 5; }
        if (pad > n) { conn_error(c, 1, "HTTP/2: bad padding"); return -1; }
        n -= pad;
        c->hdr_acc.n = 0;
        if (hb_put(&c->hdr_acc, p, n) < 0) return -1;
        c->hdr_stream = sid; c->hdr_end_stream = (flags & FL_END_STREAM) != 0;
        if (flags & FL_END_HEADERS) return headers_done(c);
        c->in_headers = 1;
        break;
    }
    case F_CONT:
        if (!c->in_headers) { conn_error(c, 1, "HTTP/2: unexpected CONTINUATION"); return -1; }
        if (hb_put(&c->hdr_acc, p, n) < 0) return -1;
        if (c->hdr_acc.n > (1 << 20)) { conn_error(c, 11, "HTTP/2: header block too large"); return -1; }
        if (flags & FL_END_HEADERS) return headers_done(c);
        break;
    case F_RST:
        if (n != 4) { conn_error(c, 6, "HTTP/2: bad RST_STREAM"); return -1; }
        s = find_stream(c, sid);
        if (s && s->state < 2) {
            s->err_code = ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
            s->state = 3;
            snprintf(s->err, sizeof s->err, "HTTP/2: stream reset by server (error code %u)", s->err_code);
        } else if (s && s->state == 2 && !s->req_end_sent) {
            /* the response is complete; RST(NO_ERROR) only stops our upload */
            s->req_end_sent = 1; hb_free(&s->out); s->out_off = 0;
        }
        break;
    case F_PUSH:
        conn_error(c, 1, "HTTP/2: PUSH_PROMISE although push is disabled"); return -1;
    case F_GOAWAY: {
        int i; u32 code;
        if (n < 8) { conn_error(c, 6, "HTTP/2: bad GOAWAY"); return -1; }
        c->goaway = 1;
        c->goaway_last = (((u32)p[0] & 0x7f) << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
        code = ((u32)p[4] << 24) | ((u32)p[5] << 16) | ((u32)p[6] << 8) | p[7];
        for (i = 0; i < c->nst; i++) {
            h2_stream* t = &c->st[i];
            if (t->used && t->state < 2 && t->id > c->goaway_last) {
                t->state = 3; snprintf(t->err, sizeof t->err, "HTTP/2: server is shutting down (GOAWAY, safe to retry)");
            }
        }
        if (code != 0) {
            char m[96]; snprintf(m, sizeof m, "HTTP/2: server closed the connection (GOAWAY error code %u)", code);
            for (i = 0; i < c->nst; i++) if (c->st[i].used && c->st[i].state < 2) { c->st[i].state = 3; snprintf(c->st[i].err, sizeof c->st[i].err, "%s", m); }
            h2_fail(c, m);
        }
        break;
    }
    default: return process_common(c, type, flags, sid, p, n);
    }
    return 0;
}

/* Read and process frames until `done(c, arg)` or timeout. 0 ok, -2 timeout, -1 error. */
static int pump(h2_conn* c, int (*done)(h2_conn*, u32), u32 arg, int timeout_ms) {
    for (;;) {
        if (flush_bodies(c) < 0) return -1;
        if (done(c, arg)) return 0;
        if (c->dead) return -1;
        while (c->in.n - c->in_off >= 9) {
            u8* f = c->in.p + c->in_off;
            size_t len = ((size_t)f[0] << 16) | ((size_t)f[1] << 8) | f[2];
            int type = f[3], flags = f[4];
            u32 sid = (((u32)f[5] & 0x7f) << 24) | ((u32)f[6] << 16) | ((u32)f[7] << 8) | f[8];
            if (len > 16384) { conn_error(c, 6, "HTTP/2: frame larger than SETTINGS_MAX_FRAME_SIZE"); return -1; }
            if (c->in.n - c->in_off < 9 + len) break;
            c->in_off += 9 + len;
            if (process_frame(c, type, flags, sid, f + 9, len) < 0) return -1;
            if (flush_bodies(c) < 0) return -1;
            if (done(c, arg)) return 0;
        }
        {
            int r = rx(c, timeout_ms);
            if (r == -2) return -2;
            if (r == 0) {
                int i;
                for (i = 0; i < c->nst; i++) if (c->st[i].used && c->st[i].state < 2) { c->st[i].state = 3; snprintf(c->st[i].err, sizeof c->st[i].err, "HTTP/2: connection closed by server"); }
                h2_fail(c, "HTTP/2: connection closed by server");
                return done(c, arg) ? 0 : -1;
            }
            if (r < 0) return -1;
        }
    }
}
static int done_stream(h2_conn* c, u32 sid) { h2_stream* s = find_stream(c, sid); return !s || s->state >= 2; }
static int done_all(h2_conn* c, u32 unused) { (void)unused; return active_streams(c) == 0; }
static int done_settings(h2_conn* c, u32 unused) { (void)unused; return c->got_settings; }
static int done_data(h2_conn* c, u32 sid) { h2_stream* s = find_stream(c, sid); return !s || s->state >= 2 || s->body.n > 0; }
static int done_headers(h2_conn* c, u32 sid) { h2_stream* s = find_stream(c, sid); return !s || s->state >= 2 || s->got_headers; }
static int done_drained(h2_conn* c, u32 sid) { h2_stream* s = find_stream(c, sid); return !s || s->state >= 2 || s->req_end_sent || s->out.n == s->out_off; }

/* ── connection setup ── */
#if defined(_WIN32)
static void h2_wsa(void) { static int done = 0; if (!done) { WSADATA w; WSAStartup(MAKEWORD(2, 2), &w); done = 1; } }
#endif
static h2_sock tcp_connect(const char* host, int port, int timeout_ms) {
    struct addrinfo hints, *res = NULL, *ai; char ps[16]; h2_sock fd = H2_BADSOCK;
#if defined(_WIN32)
    h2_wsa();
#endif
    snprintf(ps, sizeof ps, "%d", port);
    memset(&hints, 0, sizeof hints); hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, ps, &hints, &res) != 0 || !res) return H2_BADSOCK;
    for (ai = res; ai && fd == H2_BADSOCK; ai = ai->ai_next) {
        h2_sock s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol); int ok = 0;
        if (s == H2_BADSOCK) continue;
#if defined(_WIN32)
        { u_long nb = 1; ioctlsocket(s, FIONBIO, &nb);
          if (connect(s, ai->ai_addr, (int)ai->ai_addrlen) == 0 || WSAGetLastError() == WSAEWOULDBLOCK)
              if (sock_wait(s, 1, timeout_ms > 0 ? timeout_ms : -1) > 0) { int e = 0; int el = sizeof e; getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&e, &el); ok = e == 0; }
          nb = 0; ioctlsocket(s, FIONBIO, &nb); }
#else
        { int fl = fcntl(s, F_GETFL, 0); fcntl(s, F_SETFL, fl | O_NONBLOCK);
          if (connect(s, ai->ai_addr, ai->ai_addrlen) == 0) ok = 1;
          else if (errno == EINPROGRESS && sock_wait(s, 1, timeout_ms > 0 ? timeout_ms : -1) > 0) { int e = 0; socklen_t el = sizeof e; getsockopt(s, SOL_SOCKET, SO_ERROR, &e, &el); ok = e == 0; }
          fcntl(s, F_SETFL, fl); }
#endif
        if (ok) { int one = 1; setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof one); fd = s; }
        else h2_closesock(s);
    }
    freeaddrinfo(res);
    return fd;
}

/* tls: 1 = https (ALPN h2), 0 = h2c prior knowledge. Always returns a handle
 * (unless out of memory); check trc_h2_ok / trc_h2_error. */
void* trc_h2_connect(const char* host, int64_t hostlen, int64_t port, int64_t tls, void* store, int64_t verify, int64_t timeout_ms) {
    h2_conn* c = (h2_conn*)calloc(1, sizeof(h2_conn)); char h[256];
    static const char preface[] = "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n";
    u8 settings[18];
    if (!c) return NULL;
    if (hostlen > 255) hostlen = 255;
    memcpy(h, host, (size_t)hostlen); h[hostlen] = 0;
    c->fd = H2_BADSOCK; c->timeout_ms = (int)timeout_ms; c->is_tls = (int)tls;
    c->enc.max_size = c->enc.settings_max = 4096;
    c->dec.max_size = c->dec.settings_max = 4096;
    c->peer_init_win = 65535; c->conn_send_win = 65535; c->peer_max_concurrent = 100; c->next_id = 1; c->peer_max_frame = 16384;
    if ((tls && port == 443) || (!tls && port == 80)) snprintf(c->authority, sizeof c->authority, "%s", h);
    else snprintf(c->authority, sizeof c->authority, strchr(h, ':') ? "[%s]:%d" : "%s:%d", h, (int)port);
    if (tls) {
        char alpn[16]; int64_t an;
        c->tls = trc_tls_connect(h, (int64_t)strlen(h), port, store, verify, "h2", timeout_ms);
        if (!c->tls) { h2_fail(c, "HTTP/2: out of memory"); return c; }
        if (!trc_tls_ok(c->tls)) { char e[200]; int64_t n = trc_tls_error(c->tls, e, sizeof e - 1); e[n] = 0; h2_fail(c, e); trc_tls_close(c->tls); c->tls = NULL; return c; }
        an = trc_tls_alpn(c->tls, alpn, sizeof alpn - 1); alpn[an] = 0;
        if (strcmp(alpn, "h2") != 0) { h2_fail(c, "HTTP/2: server did not negotiate h2 (ALPN)"); return c; }
    } else {
        c->fd = tcp_connect(h, (int)port, (int)timeout_ms);
        if (c->fd == H2_BADSOCK) { h2_fail(c, "HTTP/2: could not connect"); return c; }
    }
    /* SETTINGS: ENABLE_PUSH=0, INITIAL_WINDOW_SIZE=16 MiB, MAX_HEADER_LIST_SIZE=1 MiB */
    settings[0] = 0; settings[1] = 2; settings[2] = settings[3] = settings[4] = settings[5] = 0;
    settings[6] = 0; settings[7] = 4; settings[8] = (u8)(H2_WINDOW >> 24); settings[9] = (u8)(H2_WINDOW >> 16); settings[10] = (u8)(H2_WINDOW >> 8); settings[11] = (u8)H2_WINDOW;
    settings[12] = 0; settings[13] = 6; settings[14] = 0; settings[15] = 0x10; settings[16] = 0; settings[17] = 0;
    if (tx(c, (const u8*)preface, sizeof preface - 1) < 0) return c;
    if (send_frame(c, F_SETTINGS, 0, 0, settings, 18) < 0) return c;
    if (send_winup(c, 0, H2_WINDOW - 65535) < 0) return c;
    /* the server's SETTINGS must be the first frame it sends */
    if (pump(c, done_settings, 0, c->timeout_ms) == -2) { h2_fail(c, "HTTP/2: timed out waiting for server SETTINGS"); return c; }
    if (!c->got_settings && !c->dead) h2_fail(c, "HTTP/2: no SETTINGS from server");
    return c;
}
int64_t trc_h2_ok(void* h) { h2_conn* c = (h2_conn*)h; return c && !c->dead; }
int64_t trc_h2_error(void* h, char* out, int64_t cap) {
    h2_conn* c = (h2_conn*)h; size_t n;
    if (!c) return 0;
    n = strlen(c->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, c->err, n); return (int64_t)n;
}
void trc_h2_set_timeout(void* h, int64_t ms) { if (h) ((h2_conn*)h)->timeout_ms = (int)ms; }
int64_t trc_h2_goaway(void* h) { return h ? ((h2_conn*)h)->goaway : 0; }

static int forbidden_header(const char* nm, size_t nl) {
    char lname[24]; size_t i;
    if (nl >= sizeof lname) return 0;
    for (i = 0; i < nl; i++) lname[i] = (nm[i] >= 'A' && nm[i] <= 'Z') ? (char)(nm[i] + 32) : nm[i];
    lname[nl] = 0;
    return !strcmp(lname, "connection") || !strcmp(lname, "keep-alive") || !strcmp(lname, "proxy-connection") ||
           !strcmp(lname, "transfer-encoding") || !strcmp(lname, "upgrade") || !strcmp(lname, "host");
}
/* Append the regular headers of `extra` to `blob`, dropping connection-specific ones (RFC 9113 section 8.2.2). */
static int append_extra(hbuf* blob, const char* extra, int64_t elen) {
    size_t k = 0;
    while (k < (size_t)elen) {
        const char* nm = extra + k; size_t nl = strnlen(nm, (size_t)elen - k); const char* v; size_t vl;
        if (k + nl >= (size_t)elen) break;
        v = nm + nl + 1; vl = strnlen(v, (size_t)elen - k - nl - 1);
        if (nl > 0 && nm[0] != ':' && !forbidden_header(nm, nl))
            if (hb_put(blob, nm, nl + 1) < 0 || hb_put(blob, v, vl + 1) < 0) return -1;
        k += nl + vl + 2;
    }
    return 0;
}

/* Start a request. extra = header blob (regular headers, any case; names are
 * lower-cased). has_body: 0 none (END_STREAM on HEADERS), 1 `body` is the
 * whole body, 2 streaming (`body` is sent now, more with trc_h2_send).
 * Returns the stream id, or -1. */
int64_t trc_h2_submit(void* h, const char* method, const char* path, const char* extra, int64_t elen, const char* body, int64_t blen, int64_t has_body) {
    h2_conn* c = (h2_conn*)h; h2_stream* s; hbuf blob = {0};
    if (!c || c->dead) return -1;
    if (c->goaway) { h2_fail(c, "HTTP/2: connection is shutting down (GOAWAY)"); return -1; }
    if (c->next_id > 0x7ffffffd) { h2_fail(c, "HTTP/2: stream ids exhausted; open a new connection"); return -1; }
#define PUTH(n, v) do { if (hb_put(&blob, n, strlen(n) + 1) < 0 || hb_put(&blob, v, strlen(v) + 1) < 0) goto oom; } while (0)
    PUTH(":method", method);
    PUTH(":scheme", c->is_tls ? "https" : "http");
    PUTH(":authority", c->authority);
    PUTH(":path", path);
#undef PUTH
    if (append_extra(&blob, extra, elen) < 0) goto oom;
    s = new_stream(c);
    if (!s) goto oom;
    s->id = c->next_id; c->next_id += 2;
    s->send_win = c->peer_init_win;
    if (has_body && blen > 0 && hb_put(&s->out, body, (size_t)blen) < 0) { s->used = 0; goto oom; }
    if (has_body == 1) s->out_end = 1;
    if (!has_body) { s->req_end_sent = 1; s->state = 1; }
    if (send_header_block(c, s->id, blob.p, blob.n, !has_body) < 0) { hb_free(&blob); return -1; }
    hb_free(&blob);
    if (flush_bodies(c) < 0) return -1;
    return s->id;
oom:
    hb_free(&blob);
    h2_fail(c, "HTTP/2: out of memory");
    return -1;
}
/* Streaming request body: queue `data` (END_STREAM after it when end != 0)
 * and send what the flow-control windows allow. Waits (up to timeout_ms)
 * while more than 1 MiB is still queued. 0 ok, -1 error, -2 timeout, -3 the
 * stream is closed / reset. */
int64_t trc_h2_send(void* h, int64_t sid, const char* data, int64_t n, int64_t end, int64_t timeout_ms) {
    h2_conn* c = (h2_conn*)h; h2_stream* s = c ? find_stream(c, (u32)sid) : NULL;
    if (!s || !c) return -1;
    if (c->dead) return -1;
    if (s->state >= 2 || s->req_end_sent || s->out_end) return -3;
    if (n > 0 && hb_put(&s->out, data, (size_t)n) < 0) { h2_fail(c, "HTTP/2: out of memory"); return -1; }
    if (end) s->out_end = 1;
    if (flush_bodies(c) < 0) return -1;
    if (s->out.n - s->out_off > (1u << 20)) {
        int r = pump(c, done_drained, (u32)sid, (int)timeout_ms);
        if (r == -2) return -2;
        if (r < 0) return -1;
    }
    return 0;
}
/* Wait for one stream (sid > 0) or all streams (sid == 0). 0 done, -2 timeout, -1 error. */
int64_t trc_h2_wait(void* h, int64_t sid, int64_t timeout_ms) {
    h2_conn* c = (h2_conn*)h;
    if (!c) return -1;
    if (sid > 0) {
        h2_stream* s = find_stream(c, (u32)sid);
        if (!s) return -1;
        if (s->state >= 2) return 0;
        if (c->dead) return -1;
        return pump(c, done_stream, (u32)sid, (int)timeout_ms);
    }
    return pump(c, done_all, 0, (int)timeout_ms);
}
/* cond: 1 response data buffered, 2 response headers, 3 queued request data
 * sent; each also ends when the stream finishes. 0 ok, -2 timeout, -1 error. */
int64_t trc_h2_wait_for(void* h, int64_t sid, int64_t cond, int64_t timeout_ms) {
    h2_conn* c = (h2_conn*)h; h2_stream* s = c ? find_stream(c, (u32)sid) : NULL;
    int (*d)(h2_conn*, u32) = cond == 1 ? done_data : cond == 2 ? done_headers : done_drained;
    if (!s) return -1;
    if (d(c, (u32)sid)) return 0;
    if (c->dead) return -1;
    return pump(c, d, (u32)sid, (int)timeout_ms);
}
/* Stream result accessors. */
int64_t trc_h2_status(void* h, int64_t sid) { h2_stream* s = h ? find_stream((h2_conn*)h, (u32)sid) : NULL; return s ? s->status : -1; }
int64_t trc_h2_state(void* h, int64_t sid) { h2_stream* s = h ? find_stream((h2_conn*)h, (u32)sid) : NULL; return s ? s->state : -1; }
int64_t trc_h2_stream_error(void* h, int64_t sid, char* out, int64_t cap) {
    h2_conn* c = (h2_conn*)h; h2_stream* s = c ? find_stream(c, (u32)sid) : NULL; const char* e; size_t n;
    if (!s) e = "HTTP/2: unknown stream";
    else if (s->err[0]) e = s->err;
    else if (s->state < 2 && c->dead) e = c->err;
    else e = "";
    n = strlen(e); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, e, n); return (int64_t)n;
}
int64_t trc_h2_rst_code(void* h, int64_t sid) { h2_stream* s = h ? find_stream((h2_conn*)h, (u32)sid) : NULL; return s ? (int64_t)s->err_code : -1; }
/* part: 0 headers blob, 1 trailers blob, 2 body. out NULL -> length. */
int64_t trc_h2_part(void* h, int64_t sid, int64_t part, char* out, int64_t cap) {
    h2_stream* s = h ? find_stream((h2_conn*)h, (u32)sid) : NULL; const hbuf* b;
    if (!s) return -1;
    b = part == 0 ? &s->headers : part == 1 ? &s->trailers : &s->body;
    if (!out) return (int64_t)b->n;
    if ((int64_t)b->n > cap) return -1;
    if (b->n) memcpy(out, b->p, b->n);
    return (int64_t)b->n;
}
/* Consume up to cap bytes of buffered response data (out NULL -> available). */
int64_t trc_h2_take(void* h, int64_t sid, char* out, int64_t cap) {
    h2_stream* s = h ? find_stream((h2_conn*)h, (u32)sid) : NULL; size_t k;
    if (!s) return -1;
    if (!out) return (int64_t)s->body.n;
    k = s->body.n < (size_t)cap ? s->body.n : (size_t)cap;
    if (k) { memcpy(out, s->body.p, k); memmove(s->body.p, s->body.p + k, s->body.n - k); s->body.n -= k; }
    return (int64_t)k;
}
/* Reset a stream with `code` (8 = CANCEL) but keep its data for inspection. */
void trc_h2_reset(void* h, int64_t sid, int64_t code) {
    h2_conn* c = (h2_conn*)h; h2_stream* s = c ? find_stream(c, (u32)sid) : NULL;
    if (!s || s->state >= 2) return;
    if (!c->dead) send_rst(c, s->id, (u32)code);
    s->state = 3; s->err_code = (u32)code;
    snprintf(s->err, sizeof s->err, "HTTP/2: stream cancelled (error code %u)", (unsigned)code);
}
/* Cancel (RST_STREAM CANCEL) if still open, and forget the stream. */
void trc_h2_release(void* h, int64_t sid) {
    h2_conn* c = (h2_conn*)h; h2_stream* s = c ? find_stream(c, (u32)sid) : NULL;
    if (!s) return;
    if (s->state < 2 && !c->dead) send_rst(c, s->id, 8);
    stream_free_data(s); s->used = 0;
}
int64_t trc_h2_ping(void* h, int64_t timeout_ms) {
    h2_conn* c = (h2_conn*)h; u8 p[8] = {'t', 'a', 'u', 'r', 'a', 'r', 'o', 0};
    (void)timeout_ms;
    if (!c || c->dead) return -1;
    return send_frame(c, F_PING, 0, 0, p, 8) < 0 ? -1 : 0;
}
static void conn_free(h2_conn* c) {
    int i;
    if (c->tls) trc_tls_close(c->tls);
    if (c->fd != H2_BADSOCK) h2_closesock(c->fd);
    for (i = 0; i < c->nst; i++) if (c->st[i].used) stream_free_data(&c->st[i]);
    free(c->st); hb_free(&c->hdr_acc); hb_free(&c->in); hb_free(&c->pend); hp_free(&c->enc); hp_free(&c->dec);
    free(c);
}
void trc_h2_close(void* h) {
    h2_conn* c = (h2_conn*)h;
    if (!c) return;
    if (!c->dead) send_goaway(c, 0);
    conn_free(c);
}

/* ═══════════════════════════ HTTP/2 server ═══════════════════════════════
 *
 * One Http2Server owns a listening socket and every connection accepted on
 * it, all driven by one thread: trc_h2srv_poll() / trc_h2srv_wait() poll the
 * listener and the connections, process frames for every stream, and return
 * when a request is ready (headers complete) or the awaited stream event
 * happened. A request is addressed by a 64-bit key: connection id << 32 |
 * stream id. Request data is flow-controlled per stream as the application
 * reads it; responses are queued and sent as the peer's windows allow.
 */
typedef struct h2srv {
    h2_sock lfd; void* tls_cfg; int timeout_ms; int port;
    h2_conn** conns; int nconn, capc;
    int64_t next_cid;
    int64_t* ready; int nready, capr;
    int max_streams;
    int closing;
    char err[256];
} h2srv;

static int64_t h2_now_ms(void) {
#if defined(_WIN32)
    return (int64_t)GetTickCount64();
#else
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}
#define SKEY(cid, sid) (((int64_t)(cid) << 32) | (int64_t)(sid))

static void srv_unqueue(h2srv* s, int64_t key) {
    int i, j = 0;
    for (i = 0; i < s->nready; i++) if (s->ready[i] != key) s->ready[j++] = s->ready[i];
    s->nready = j;
}
static int srv_enqueue(h2srv* s, int64_t key) {
    if (s->nready == s->capr) {
        int nc = s->capr ? s->capr * 2 : 16; int64_t* nr = (int64_t*)realloc(s->ready, sizeof(int64_t) * (size_t)nc);
        if (!nr) return -1;
        s->ready = nr; s->capr = nc;
    }
    s->ready[s->nready++] = key;
    return 0;
}
static void srv_free_stream(h2_conn* c, h2_stream* st) {
    srv_unqueue(c->srv, SKEY(c->cid, st->id));
    stream_free_data(st); st->used = 0;
}
static int srv_live_streams(h2_conn* c) {
    int i, n = 0;
    for (i = 0; i < c->nst; i++) {
        h2_stream* s = &c->st[i];
        if (s->used && !s->reset && !(s->req_end_sent && s->remote_end)) n++;
    }
    return n;
}
static int srv_lookup(h2srv* s, int64_t key, h2_conn** cc, h2_stream** ss) {
    int i; int64_t cid = key >> 32; u32 sid = (u32)(key & 0xffffffff);
    for (i = 0; i < s->nconn; i++) if (s->conns[i]->cid == cid) {
        *cc = s->conns[i]; *ss = find_stream(s->conns[i], sid);
        return *ss != NULL;
    }
    return 0;
}

static int lc_eq_n(const char* a, size_t an, const char* b) { size_t bl = strlen(b); return an == bl && memcmp(a, b, an) == 0; }
/* RFC 9113 section 8.3.1 / 8.2: pseudo-headers first, required ones present,
 * lower-case names, no connection-specific headers, te only "trailers". */
static int srv_valid_request(const u8* blob, size_t n) {
    size_t k = 0; int regular = 0, has_method = 0, has_path = 0, has_scheme = 0, is_connect = 0;
    while (k < n) {
        const char* nm = (const char*)blob + k; size_t nl = strlen(nm), i; const char* v = nm + nl + 1; size_t vl = strlen(v);
        k += nl + vl + 2;
        if (nl == 0) return -1;
        for (i = 0; i < nl; i++) if (nm[i] >= 'A' && nm[i] <= 'Z') return -1;
        if (nm[0] == ':') {
            if (regular) return -1;
            if (lc_eq_n(nm, nl, ":method")) { if (has_method++) return -1; is_connect = lc_eq_n(v, vl, "CONNECT"); }
            else if (lc_eq_n(nm, nl, ":path")) { if (has_path++ || vl == 0) return -1; }
            else if (lc_eq_n(nm, nl, ":scheme")) { if (has_scheme++) return -1; }
            else if (!lc_eq_n(nm, nl, ":authority") && !lc_eq_n(nm, nl, ":protocol")) return -1;
        } else {
            regular = 1;
            if (forbidden_header(nm, nl) && !lc_eq_n(nm, nl, "host")) return -1;
            if (lc_eq_n(nm, nl, "te") && !lc_eq_n(v, vl, "trailers")) return -1;
        }
    }
    if (!has_method) return -1;
    if (!is_connect && (!has_path || !has_scheme)) return -1;
    return 0;
}

static int srv_headers_done(h2_conn* c) {
    hbuf blob = {0}; h2_stream* s; int rc; u32 sid = c->hdr_stream;
    rc = hp_decode(&c->dec, c->hdr_acc.p, c->hdr_acc.n, &blob, 1 << 20);
    c->hdr_acc.n = 0; c->in_headers = 0;
    if (rc < 0) { hb_free(&blob); conn_error(c, 9, "HTTP/2: header compression error"); return -1; }
    s = find_stream(c, sid);
    if (!s) {
        if (sid <= c->last_peer) { hb_free(&blob); conn_error(c, 5, "HTTP/2: HEADERS on a closed stream"); return -1; }
        c->last_peer = sid;
        if (c->goaway_sent || srv_live_streams(c) >= c->max_streams) { hb_free(&blob); return send_rst(c, sid, 7); }
        if (srv_valid_request(blob.p, blob.n) < 0) { hb_free(&blob); return send_rst(c, sid, 1); }
        s = new_stream(c);
        if (!s) { hb_free(&blob); conn_error(c, 2, "HTTP/2: out of memory"); return -1; }
        s->id = sid; s->send_win = c->peer_init_win; s->recv_win = H2S_STREAM_WIN;
        s->headers = blob; s->got_headers = 1; s->remote_end = c->hdr_end_stream;
        if (srv_enqueue(c->srv, SKEY(c->cid, sid)) < 0) { conn_error(c, 2, "HTTP/2: out of memory"); return -1; }
        return 0;
    }
    if (s->remote_end || s->reset) { hb_free(&blob); if (!s->reset) { s->reset = 1; s->err_code = 5; } return send_rst(c, sid, 5); }
    if (!c->hdr_end_stream) { hb_free(&blob); s->reset = 1; s->err_code = 1; return send_rst(c, sid, 1); }
    hb_free(&s->trailers); s->trailers = blob; s->remote_end = 1;
    return 0;
}

static int srv_process_frame(h2_conn* c, int type, int flags, u32 sid, u8* p, size_t n) {
    h2_stream* s;
    if (c->in_headers && (type != F_CONT || sid != c->hdr_stream)) { conn_error(c, 1, "HTTP/2: expected CONTINUATION"); return -1; }
    if (!c->got_settings && type != F_SETTINGS) { conn_error(c, 1, "HTTP/2: the first frame must be SETTINGS"); return -1; }
    switch (type) {
    case F_DATA: {
        size_t pad = 0, flen = n;
        if (sid == 0) { conn_error(c, 1, "HTTP/2: DATA on stream 0"); return -1; }
        if (flags & FL_PADDED) { if (n < 1 || p[0] >= n) { conn_error(c, 1, "HTTP/2: bad padding"); return -1; } pad = p[0]; p++; n--; n -= pad; }
        c->conn_recv_win -= (int64_t)flen;
        if (c->conn_recv_win < 0) { conn_error(c, 3, "HTTP/2: connection flow-control window exceeded"); return -1; }
        c->conn_recv_unacked += (int64_t)flen;
        if (c->conn_recv_unacked >= H2S_CONN_WIN / 2) {
            if (send_winup(c, 0, (u32)c->conn_recv_unacked) < 0) return -1;
            c->conn_recv_win += c->conn_recv_unacked; c->conn_recv_unacked = 0;
        }
        s = find_stream(c, sid);
        if (!s) {
            if (sid > c->last_peer) { conn_error(c, 1, "HTTP/2: DATA on an idle stream"); return -1; }
            return send_rst(c, sid, 5);
        }
        if (s->reset) break;
        if (s->remote_end) { s->reset = 1; s->err_code = 5; return send_rst(c, sid, 5); }
        s->recv_win -= (int64_t)flen;
        if (s->recv_win < 0) { s->reset = 1; s->err_code = 3; hb_free(&s->out); return send_rst(c, sid, 3); }
        if (hb_put(&s->body, p, n) < 0) { conn_error(c, 2, "HTTP/2: out of memory"); return -1; }
        s->consumed += (int64_t)(flen - n);     /* padding is credited at once */
        if (flags & FL_END_STREAM) s->remote_end = 1;
        break;
    }
    case F_HEADERS: {
        size_t pad = 0;
        if (sid == 0) { conn_error(c, 1, "HTTP/2: HEADERS on stream 0"); return -1; }
        if ((sid & 1) == 0) { conn_error(c, 1, "HTTP/2: client used an even stream id"); return -1; }
        if (flags & FL_PADDED) { if (n < 1) { conn_error(c, 1, "HTTP/2: bad padding"); return -1; } pad = p[0]; p++; n--; }
        if (flags & FL_PRIORITY) { if (n < 5) { conn_error(c, 1, "HTTP/2: bad HEADERS"); return -1; } p += 5; n -= 5; }
        if (pad > n) { conn_error(c, 1, "HTTP/2: bad padding"); return -1; }
        n -= pad;
        c->hdr_acc.n = 0;
        if (hb_put(&c->hdr_acc, p, n) < 0) { conn_error(c, 2, "HTTP/2: out of memory"); return -1; }
        c->hdr_stream = sid; c->hdr_end_stream = (flags & FL_END_STREAM) != 0;
        if (flags & FL_END_HEADERS) return srv_headers_done(c);
        c->in_headers = 1;
        break;
    }
    case F_CONT:
        if (!c->in_headers) { conn_error(c, 1, "HTTP/2: unexpected CONTINUATION"); return -1; }
        if (hb_put(&c->hdr_acc, p, n) < 0) { conn_error(c, 2, "HTTP/2: out of memory"); return -1; }
        if (c->hdr_acc.n > (1 << 20)) { conn_error(c, 11, "HTTP/2: header block too large"); return -1; }
        if (flags & FL_END_HEADERS) return srv_headers_done(c);
        break;
    case F_RST:
        if (sid == 0) { conn_error(c, 1, "HTTP/2: RST_STREAM on stream 0"); return -1; }
        if (n != 4) { conn_error(c, 6, "HTTP/2: bad RST_STREAM"); return -1; }
        if (sid > c->last_peer) { conn_error(c, 1, "HTTP/2: RST_STREAM on an idle stream"); return -1; }
        s = find_stream(c, sid);
        if (s && !s->reset) {
            s->reset = 1; s->err_code = ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
            hb_free(&s->out); s->out_off = 0;
            if (!s->handed || s->released) srv_free_stream(c, s);
        }
        break;
    case F_PUSH:
        conn_error(c, 1, "HTTP/2: PUSH_PROMISE from a client"); return -1;
    case F_GOAWAY:
        if (sid != 0 || n < 8) { conn_error(c, 1, "HTTP/2: bad GOAWAY"); return -1; }
        c->goaway = 1;
        break;
    default: return process_common(c, type, flags, sid, p, n);
    }
    return 0;
}

/* Finish a released stream: RST(NO_ERROR) if the client is still sending. */
static void srv_retire(h2_conn* c, h2_stream* s) {
    if (!s->reset && !s->remote_end && !c->dead) send_rst(c, s->id, 0);
    srv_free_stream(c, s);
}
/* Send queued response data / trailers as the windows allow. */
static int srv_flush(h2_conn* c) {
    int i;
    for (i = 0; i < c->nst; i++) {
        h2_stream* s = &c->st[i];
        if (!s->used || !s->resp_sent || s->req_end_sent || s->reset) continue;
        while (s->out_off < s->out.n && c->conn_send_win > 0 && s->send_win > 0) {
            size_t chunk = s->out.n - s->out_off; int fl;
            if (chunk > c->peer_max_frame) chunk = c->peer_max_frame;
            if ((int64_t)chunk > c->conn_send_win) chunk = (size_t)c->conn_send_win;
            if ((int64_t)chunk > s->send_win) chunk = (size_t)s->send_win;
            fl = (s->out_end && !s->has_out_trailers && s->out_off + chunk == s->out.n) ? FL_END_STREAM : 0;
            if (send_frame(c, F_DATA, fl, s->id, s->out.p + s->out_off, chunk) < 0) return -1;
            s->out_off += chunk; c->conn_send_win -= (int64_t)chunk; s->send_win -= (int64_t)chunk;
            if (fl) s->req_end_sent = 1;
        }
        if (s->out_off == s->out.n) {
            s->out.n = 0; s->out_off = 0;
            if (s->out_end && !s->req_end_sent) {
                if (s->has_out_trailers) {
                    if (send_header_block(c, s->id, s->out_trailers.p, s->out_trailers.n, 1) < 0) return -1;
                    hb_free(&s->out_trailers); s->has_out_trailers = 0;
                } else if (send_frame(c, F_DATA, FL_END_STREAM, s->id, NULL, 0) < 0) return -1;
                s->req_end_sent = 1;
            }
        }
        if (s->req_end_sent) { hb_free(&s->out); if (s->released) srv_retire(c, s); }
    }
    return 0;
}
/* Process every complete frame buffered on a connection. */
static int srv_drain(h2_conn* c) {
    static const char preface[] = "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n";
    if (c->dead) return -1;
    if (!c->preface) {
        if (c->in.n - c->in_off < 24) {
            if (c->in.n - c->in_off > 0 && memcmp(c->in.p + c->in_off, preface, c->in.n - c->in_off) != 0) { conn_error(c, 1, "HTTP/2: bad connection preface"); return -1; }
            return 0;
        }
        if (memcmp(c->in.p + c->in_off, preface, 24) != 0) { conn_error(c, 1, "HTTP/2: bad connection preface"); return -1; }
        c->in_off += 24; c->preface = 1;
    }
    while (c->in.n - c->in_off >= 9) {
        u8* f = c->in.p + c->in_off;
        size_t len = ((size_t)f[0] << 16) | ((size_t)f[1] << 8) | f[2];
        int type = f[3], flags = f[4];
        u32 sid = (((u32)f[5] & 0x7f) << 24) | ((u32)f[6] << 16) | ((u32)f[7] << 8) | f[8];
        if (len > 16384) { conn_error(c, 6, "HTTP/2: frame larger than SETTINGS_MAX_FRAME_SIZE"); return -1; }
        if (c->in.n - c->in_off < 9 + len) break;
        c->in_off += 9 + len;
        if (srv_process_frame(c, type, flags, sid, f + 9, len) < 0) return -1;
        if (c->dead) return -1;
    }
    return 0;
}
/* Drop dead connections whose streams the application no longer holds;
 * close drained connections after GOAWAY. */
static void srv_reap(h2srv* s) {
    int i, j;
    for (i = 0; i < s->nconn; ) {
        h2_conn* c = s->conns[i]; int held = 0;
        if (c->dead) {
            for (j = 0; j < c->nst; j++) {
                h2_stream* st = &c->st[j];
                if (!st->used) continue;
                if (st->handed && !st->released) { if (!st->reset) { st->reset = 1; st->err_code = 8; } held = 1; }
                else srv_free_stream(c, st);
            }
        } else if ((c->goaway || c->goaway_sent) && c->preface) {
            for (j = 0; j < c->nst; j++) if (c->st[j].used) held = 1;
            if (!held) { c->dead = 1; snprintf(c->err, sizeof c->err, "HTTP/2: connection finished"); }
            held = 1;   /* freed on the next pass */
            if (c->dead) {
                int k; held = 0;
                for (k = 0; k < c->nst; k++) if (c->st[k].used) held = 1;
            }
        } else held = 1;
        if (!held) {
            conn_free(c);
            s->conns[i] = s->conns[--s->nconn];
            continue;
        }
        i++;
    }
}

static void srv_accept(h2srv* s) {
    struct sockaddr_storage sa; socklen_t sl = sizeof sa; h2_sock fd; h2_conn* c; u8 settings[18];
    fd = accept(s->lfd, (struct sockaddr*)&sa, &sl);
    if (fd == H2_BADSOCK) return;
    { int one = 1; setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof one); }
    c = (h2_conn*)calloc(1, sizeof(h2_conn));
    if (!c) { h2_closesock(fd); return; }
    c->fd = fd; c->timeout_ms = s->timeout_ms; c->is_server = 1; c->srv = s; c->cid = s->next_cid++;
    c->enc.max_size = c->enc.settings_max = 4096;
    c->dec.max_size = c->dec.settings_max = 4096;
    c->peer_init_win = 65535; c->conn_send_win = 65535; c->peer_max_concurrent = 100; c->peer_max_frame = 16384;
    c->conn_recv_win = 65535; c->max_streams = s->max_streams;
    if (s->tls_cfg) {
        char alpn[16]; int64_t an;
        c->tls = trc_tls_server_fd((int64_t)fd, s->tls_cfg, s->timeout_ms, 1);
        c->fd = H2_BADSOCK; c->is_tls = 1;
        if (!c->tls || !trc_tls_ok(c->tls)) { if (c->tls) trc_tls_close(c->tls); else h2_closesock(fd); c->tls = NULL; conn_free(c); return; }
        an = trc_tls_alpn(c->tls, alpn, sizeof alpn - 1); alpn[an] = 0;
        if (an > 0 && strcmp(alpn, "h2") != 0) { conn_free(c); return; }
    }
    if (s->nconn == s->capc) {
        int nc = s->capc ? s->capc * 2 : 8; h2_conn** n2 = (h2_conn**)realloc(s->conns, sizeof(h2_conn*) * (size_t)nc);
        if (!n2) { conn_free(c); return; }
        s->conns = n2; s->capc = nc;
    }
    s->conns[s->nconn++] = c;
    /* SETTINGS: MAX_CONCURRENT_STREAMS, INITIAL_WINDOW_SIZE, MAX_HEADER_LIST_SIZE = 1 MiB */
    settings[0] = 0; settings[1] = 3; settings[2] = (u8)(s->max_streams >> 24); settings[3] = (u8)(s->max_streams >> 16); settings[4] = (u8)(s->max_streams >> 8); settings[5] = (u8)s->max_streams;
    settings[6] = 0; settings[7] = 4; settings[8] = (u8)(H2S_STREAM_WIN >> 24); settings[9] = (u8)(H2S_STREAM_WIN >> 16); settings[10] = (u8)(H2S_STREAM_WIN >> 8); settings[11] = (u8)H2S_STREAM_WIN;
    settings[12] = 0; settings[13] = 6; settings[14] = 0; settings[15] = 0x10; settings[16] = 0; settings[17] = 0;
    if (send_frame(c, F_SETTINGS, 0, 0, settings, 18) < 0) return;
    if (send_winup(c, 0, H2S_CONN_WIN - 65535) < 0) return;
    c->conn_recv_win = H2S_CONN_WIN;
}

static int sdone_ready(h2srv* s, int64_t key) { (void)key; return s->nready > 0; }
static int sdone_data(h2srv* s, int64_t key) {
    h2_conn* c; h2_stream* st;
    if (!srv_lookup(s, key, &c, &st)) return 1;
    return st->body.n > 0 || st->remote_end || st->reset || c->dead;
}
static int sdone_drained(h2srv* s, int64_t key) {
    h2_conn* c; h2_stream* st;
    if (!srv_lookup(s, key, &c, &st)) return 1;
    return st->reset || c->dead || st->req_end_sent || (st->out.n == st->out_off && !st->has_out_trailers);
}
static int sdone_reset(h2srv* s, int64_t key) {
    h2_conn* c; h2_stream* st;
    if (!srv_lookup(s, key, &c, &st)) return 1;
    return st->reset || c->dead;
}
static int sdone_never(h2srv* s, int64_t key) { (void)s; (void)key; return 0; }

/* 1 condition met, 2 a new request is ready (stop_on_new), 0 timeout, -1 no listener and no connections. */
static int srv_pump(h2srv* s, int (*done)(h2srv*, int64_t), int64_t arg, int timeout_ms, int stop_on_new) {
    int64_t deadline = timeout_ms >= 0 ? h2_now_ms() + timeout_ms : -1;
    struct pollfd* pf = NULL; int pcap = 0;
    for (;;) {
        int i, np = 0, wait, r;
        for (i = 0; i < s->nconn; i++) {
            h2_conn* c = s->conns[i];
            if (c->dead) continue;
            if (srv_drain(c) == 0) srv_flush(c);
        }
        srv_reap(s);
        if (done(s, arg)) { free(pf); return 1; }
        if (stop_on_new && s->nready > 0) { free(pf); return 2; }
        if (s->lfd == H2_BADSOCK && s->nconn == 0) { free(pf); return -1; }
        if (deadline >= 0) { int64_t left = deadline - h2_now_ms(); wait = left > 0 ? (int)left : 0; } else wait = -1;
        if (pcap < s->nconn + 1) { pcap = s->nconn + 8; free(pf); pf = (struct pollfd*)malloc(sizeof(struct pollfd) * (size_t)pcap); if (!pf) return -1; }
        if (s->lfd != H2_BADSOCK && !s->closing) { pf[np].fd = s->lfd; pf[np].events = POLLIN; pf[np].revents = 0; np++; }
        for (i = 0; i < s->nconn; i++) {
            h2_conn* c = s->conns[i];
            if (c->dead) continue;
            if (c->pend.n > 0) wait = 0;
            pf[np].fd = c->tls ? (h2_sock)trc_tls_fd(c->tls) : c->fd; pf[np].events = POLLIN; pf[np].revents = 0; np++;
        }
        if (np == 0) {
            /* only dead connections still held by the application */
            if (wait != 0) {
#if defined(_WIN32)
                Sleep(wait < 0 || wait > 50 ? 50 : wait);
#else
                poll(NULL, 0, wait < 0 || wait > 50 ? 50 : wait);
#endif
            }
            if (deadline >= 0 && h2_now_ms() >= deadline) { free(pf); return 0; }
            continue;
        }
#if defined(_WIN32)
        r = WSAPoll(pf, (ULONG)np, wait);
#else
        do { r = poll(pf, (nfds_t)np, wait); } while (r < 0 && errno == EINTR);
#endif
        if (r < 0) { free(pf); snprintf(s->err, sizeof s->err, "HTTP/2 server: poll failed"); return -1; }
        {
            int k = 0;
            if (s->lfd != H2_BADSOCK && !s->closing) { if (pf[0].revents & POLLIN) srv_accept(s); k = 1; }
            for (i = 0; i < s->nconn && k < np; i++) {
                h2_conn* c = s->conns[i]; h2_sock fd;
                if (c->dead) continue;
                fd = c->tls ? (h2_sock)trc_tls_fd(c->tls) : c->fd;
                if (pf[k].fd != fd) continue;   /* a connection accepted during this pass */
                if ((pf[k].revents & (POLLIN | POLLHUP | POLLERR)) || c->pend.n > 0) {
                    int rr = rx(c, 5000);
                    if (rr == 0) h2_fail(c, "HTTP/2: connection closed by peer");
                }
                k++;
            }
        }
        if (r == 0 && deadline >= 0 && h2_now_ms() >= deadline) {
            /* last pass: process what arrived, then report the timeout */
            for (i = 0; i < s->nconn; i++) if (!s->conns[i]->dead && srv_drain(s->conns[i]) == 0) srv_flush(s->conns[i]);
            srv_reap(s);
            if (done(s, arg)) { free(pf); return 1; }
            if (stop_on_new && s->nready > 0) { free(pf); return 2; }
            free(pf); return 0;
        }
    }
}

/* Listen on host:port (port 0 = any). tls_cfg: a TlsServerConfig handle or NULL (h2c). */
void* trc_h2srv_new(const char* host, int64_t hostlen, int64_t port, void* tls_cfg, int64_t timeout_ms, int64_t max_streams) {
    h2srv* s = (h2srv*)calloc(1, sizeof(h2srv)); char h[256]; char ps[16];
    struct addrinfo hints, *res = NULL, *ai;
    if (!s) return NULL;
#if defined(_WIN32)
    h2_wsa();
#endif
    s->lfd = H2_BADSOCK; s->tls_cfg = tls_cfg; s->timeout_ms = (int)timeout_ms; s->next_cid = 1;
    s->max_streams = max_streams > 0 ? (int)max_streams : 100;
    if (hostlen > 255) hostlen = 255;
    memcpy(h, host, (size_t)hostlen); h[hostlen] = 0;
    snprintf(ps, sizeof ps, "%d", (int)port);
    memset(&hints, 0, sizeof hints); hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM; hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(h[0] ? h : NULL, ps, &hints, &res) != 0 || !res) { snprintf(s->err, sizeof s->err, "HTTP/2 server: cannot resolve %.200s", h); return s; }
    for (ai = res; ai && s->lfd == H2_BADSOCK; ai = ai->ai_next) {
        h2_sock fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol); int one = 1;
        if (fd == H2_BADSOCK) continue;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof one);
        if (bind(fd, ai->ai_addr, (int)ai->ai_addrlen) == 0 && listen(fd, 128) == 0) s->lfd = fd;
        else h2_closesock(fd);
    }
    freeaddrinfo(res);
    if (s->lfd == H2_BADSOCK) { snprintf(s->err, sizeof s->err, "HTTP/2 server: cannot listen on %.200s:%d", h, (int)port); return s; }
    {
        struct sockaddr_storage sa; socklen_t sl = sizeof sa;
        if (getsockname(s->lfd, (struct sockaddr*)&sa, &sl) == 0) {
            if (sa.ss_family == AF_INET) s->port = ntohs(((struct sockaddr_in*)&sa)->sin_port);
            else if (sa.ss_family == AF_INET6) s->port = ntohs(((struct sockaddr_in6*)&sa)->sin6_port);
        }
    }
    return s;
}
int64_t trc_h2srv_ok(void* h) { h2srv* s = (h2srv*)h; return s && s->lfd != H2_BADSOCK; }
int64_t trc_h2srv_error(void* h, char* out, int64_t cap) {
    h2srv* s = (h2srv*)h; size_t n;
    if (!s) return 0;
    n = strlen(s->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, s->err, n); return (int64_t)n;
}
int64_t trc_h2srv_port(void* h) { return h ? ((h2srv*)h)->port : 0; }
int64_t trc_h2srv_conn_count(void* h) { h2srv* s = (h2srv*)h; int i, n = 0; if (!s) return 0; for (i = 0; i < s->nconn; i++) if (!s->conns[i]->dead) n++; return n; }
/* Next ready request key; 0 on timeout (timeout_ms < 0 waits forever), -1 when the server is closed. */
int64_t trc_h2srv_poll(void* h, int64_t timeout_ms) {
    h2srv* s = (h2srv*)h; int r; int64_t key;
    if (!s) return -1;
    r = srv_pump(s, sdone_ready, 0, (int)timeout_ms, 0);
    if (r != 1 || s->nready == 0) return r < 0 ? -1 : 0;
    key = s->ready[0];
    memmove(s->ready, s->ready + 1, sizeof(int64_t) * (size_t)(s->nready - 1)); s->nready--;
    {
        h2_conn* c; h2_stream* st;
        if (srv_lookup(s, key, &c, &st)) st->handed = 1;
    }
    return key;
}
/* Drive the server for up to timeout_ms without waiting for anything (keeps connections alive). */
void trc_h2srv_run(void* h, int64_t timeout_ms) { if (h) srv_pump((h2srv*)h, sdone_never, 0, (int)timeout_ms, 1); }
int64_t trc_h2srv_pending(void* h) { return h ? ((h2srv*)h)->nready : 0; }
/* cond 1: request data / end / reset; cond 2: queued response sent; cond 3: reset / connection lost.
 * 1 met, 2 new request ready (stop_on_new), 0 timeout, -1 gone. */
int64_t trc_h2srv_wait(void* h, int64_t key, int64_t cond, int64_t timeout_ms, int64_t stop_on_new) {
    h2srv* s = (h2srv*)h; int r;
    if (!s) return -1;
    r = srv_pump(s, cond == 1 ? sdone_data : cond == 3 ? sdone_reset : sdone_drained, key, (int)timeout_ms, (int)stop_on_new);
    return r;
}
/* part: 0 request headers, 1 request trailers. out NULL -> length. */
int64_t trc_h2srv_part(void* h, int64_t key, int64_t part, char* out, int64_t cap) {
    h2_conn* c; h2_stream* st; const hbuf* b;
    if (!h || !srv_lookup((h2srv*)h, key, &c, &st)) return -1;
    b = part == 0 ? &st->headers : &st->trailers;
    if (!out) return (int64_t)b->n;
    if ((int64_t)b->n > cap) return -1;
    if (b->n) memcpy(out, b->p, b->n);
    return (int64_t)b->n;
}
/* Consume buffered request data, returning flow-control credit to the client. */
int64_t trc_h2srv_take(void* h, int64_t key, char* out, int64_t cap) {
    h2_conn* c; h2_stream* st; size_t k;
    if (!h || !srv_lookup((h2srv*)h, key, &c, &st)) return -1;
    if (!out) return (int64_t)st->body.n;
    k = st->body.n < (size_t)cap ? st->body.n : (size_t)cap;
    if (k) { memcpy(out, st->body.p, k); memmove(st->body.p, st->body.p + k, st->body.n - k); st->body.n -= k; }
    st->consumed += (int64_t)k;
    if (!st->remote_end && !st->reset && !c->dead && st->consumed >= H2S_STREAM_WIN / 4) {
        send_winup(c, st->id, (u32)st->consumed);
        st->recv_win += st->consumed; st->consumed = 0;
    }
    return (int64_t)k;
}
/* what: 0 request ended, 1 reset (error code + 1, 0 if not), 2 response ended,
 * 3 connection alive, 4 queued response bytes, 5 response headers sent, 6 TLS. */
int64_t trc_h2srv_info(void* h, int64_t key, int64_t what) {
    h2_conn* c; h2_stream* st;
    if (!h || !srv_lookup((h2srv*)h, key, &c, &st)) return -1;
    switch (what) {
        case 0: return st->remote_end;
        case 1: return st->reset ? (int64_t)st->err_code + 1 : 0;
        case 2: return st->req_end_sent;
        case 3: return !c->dead;
        case 4: return (int64_t)(st->out.n - st->out_off);
        case 5: return st->resp_sent;
        case 6: return c->is_tls;
        default: return -1;
    }
}
static int srv_usable(h2srv* s, int64_t key, h2_conn** c, h2_stream** st) {
    if (!s || !srv_lookup(s, key, c, st)) return -1;
    if ((*c)->dead) return -1;
    if ((*st)->reset) return -3;
    return 0;
}
/* Response HEADERS with :status (end != 0: no body, e.g. gRPC trailers-only).
 * 0 ok, -1 error, -3 stream reset, -4 already responded. */
int64_t trc_h2srv_respond(void* h, int64_t key, int64_t status, const char* extra, int64_t elen, int64_t end) {
    h2_conn* c; h2_stream* st; hbuf blob = {0}; char sb[16]; int r = srv_usable((h2srv*)h, key, &c, &st);
    if (r < 0) return r;
    if (st->resp_sent) return -4;
    snprintf(sb, sizeof sb, "%d", (int)status);
    if (hb_put(&blob, ":status", 8) < 0 || hb_put(&blob, sb, strlen(sb) + 1) < 0 || append_extra(&blob, extra, elen) < 0) { hb_free(&blob); return -1; }
    r = send_header_block(c, st->id, blob.p, blob.n, end != 0);
    hb_free(&blob);
    if (r < 0) return -1;
    st->resp_sent = 1;
    if (end) { st->req_end_sent = 1; st->out_end = 1; }
    return 0;
}
/* Queue response DATA (end: END_STREAM after it). 0 ok, -1 error, -3 reset, -4 no headers yet / already ended. */
int64_t trc_h2srv_send(void* h, int64_t key, const char* data, int64_t n, int64_t end) {
    h2_conn* c; h2_stream* st; int r = srv_usable((h2srv*)h, key, &c, &st);
    if (r < 0) return r;
    if (!st->resp_sent || st->out_end) return -4;
    if (n > 0 && hb_put(&st->out, data, (size_t)n) < 0) return -1;
    if (end) st->out_end = 1;
    return srv_flush(c) < 0 ? -1 : 0;
}
/* Trailers (end of the response, after queued data). */
int64_t trc_h2srv_trailers(void* h, int64_t key, const char* extra, int64_t elen) {
    h2_conn* c; h2_stream* st; int r = srv_usable((h2srv*)h, key, &c, &st);
    if (r < 0) return r;
    if (!st->resp_sent || st->out_end) return -4;
    hb_free(&st->out_trailers);
    if (append_extra(&st->out_trailers, extra, elen) < 0) return -1;
    st->has_out_trailers = 1; st->out_end = 1;
    return srv_flush(c) < 0 ? -1 : 0;
}
void trc_h2srv_reset(void* h, int64_t key, int64_t code) {
    h2_conn* c; h2_stream* st;
    if (!h || !srv_lookup((h2srv*)h, key, &c, &st) || st->reset) return;
    if (!c->dead) send_rst(c, st->id, (u32)code);
    st->reset = 1; st->err_code = (u32)code; hb_free(&st->out); st->out_off = 0;
}
/* The application is done with a request. An unfinished response is reset
 * (INTERNAL_ERROR); queued data still goes out before the stream is freed. */
void trc_h2srv_release(void* h, int64_t key) {
    h2_conn* c; h2_stream* st;
    if (!h || !srv_lookup((h2srv*)h, key, &c, &st)) return;
    if (!st->reset && !c->dead && !st->out_end) trc_h2srv_reset(h, key, 2);
    st->released = 1;
    if (st->reset || c->dead || st->req_end_sent) srv_retire(c, st);
}
/* Graceful shutdown: stop accepting, send GOAWAY(NO_ERROR); open streams may finish. */
void trc_h2srv_shutdown(void* h) {
    h2srv* s = (h2srv*)h; int i;
    if (!s) return;
    s->closing = 1;
    if (s->lfd != H2_BADSOCK) { h2_closesock(s->lfd); s->lfd = H2_BADSOCK; }
    for (i = 0; i < s->nconn; i++) if (!s->conns[i]->dead && !s->conns[i]->goaway_sent) { send_goaway(s->conns[i], 0); s->conns[i]->goaway_sent = 1; }
}
void trc_h2srv_close(void* h) {
    h2srv* s = (h2srv*)h; int i;
    if (!s) return;
    for (i = 0; i < s->nconn; i++) { if (!s->conns[i]->dead && !s->conns[i]->goaway_sent) send_goaway(s->conns[i], 0); conn_free(s->conns[i]); }
    if (s->lfd != H2_BADSOCK) h2_closesock(s->lfd);
    free(s->conns); free(s->ready); free(s);
}
