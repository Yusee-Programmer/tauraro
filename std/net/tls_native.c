/*
 * std/net/tls_native.c -- a TLS 1.3 client (RFC 8446) with certificate
 * verification, for std.net.tls.
 *
 * Linked through `# tauraro-cpp-shim: tls_native.c` in std/net/tls.tr. Uses
 * std/crypto/crypto_native.c (SHA-2, HMAC, HKDF, X25519, CSPRNG) and
 * std/crypto/pk_native.c (P-256 ECDHE, X.509 chain + CertificateVerify).
 *
 *   key exchange ...... X25519, secp256r1 (both offered), secp384r1 via HelloRetryRequest
 *   cipher suites ..... TLS_AES_128_GCM_SHA256, TLS_CHACHA20_POLY1305_SHA256,
 *                       TLS_AES_256_GCM_SHA384
 *   signatures ........ ECDSA P-256/P-384, RSA-PSS (rsae); PKCS#1 v1.5 for certificates
 *   extras ............ SNI, ALPN, KeyUpdate, close_notify, middlebox-compat CCS
 *   not supported ..... TLS 1.2 and below, PSK/resumption, 0-RTT, client certificates
 *                       (an empty Certificate is sent if the server asks for one)
 *
 * The handshake runs over a connected socket (fd), so callers can upgrade an
 * existing plaintext connection (STARTTLS).
 */
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#if defined(_WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
typedef SOCKET tls_sock;
#  define TLS_BADSOCK INVALID_SOCKET
#  define tls_closesock closesocket
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
typedef int tls_sock;
#  define TLS_BADSOCK (-1)
#  define tls_closesock close
#endif

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

/* crypto_native.c */
int64_t trc_digest(int64_t alg, const char* data, int64_t n, char* out);
int64_t trc_hmac(int64_t alg, const char* key, int64_t klen, const char* msg, int64_t mlen, char* out);
int64_t trc_hkdf_extract(int64_t alg, const char* salt, int64_t slen, const char* ikm, int64_t ilen, char* prk);
int64_t trc_hkdf_expand(int64_t alg, const char* prk, int64_t plen, const char* info, int64_t ilen, char* out, int64_t olen);
int64_t trc_x25519(char* qout, const char* scalar, const char* point);
int64_t trc_random(char* out, int64_t n);
int64_t trc_ct_eq(const char* a, int64_t alen, const char* b, int64_t blen);
/* pk_native.c */
void* trc_pk_ec_generate(int64_t cid);
void trc_pk_free(void* h);
int64_t trc_pk_public_raw(void* h, char* out, int64_t cap);
int64_t trc_pk_ecdh(void* h, const char* peer, int64_t plen, char* out);
int64_t trc_x509_verify_chain(void* store_h, const char* ders, const int64_t* lens, int64_t count,
                              const char* host, int64_t hostlen, int64_t now, int64_t purpose);
int64_t trc_x509_verify_tls_sig(const char* leaf_der, int64_t leaf_len, int64_t scheme, const char* msg, int64_t mlen, const char* sig, int64_t slen);
int64_t trc_x509_error_text(int64_t code, char* out, int64_t cap);
void* trc_pk_parse_der(const char* der_bytes, int64_t n, int64_t* err);
int64_t trc_pk_kind(void* h);
int64_t trc_pk_has_private(void* h);
int64_t trc_pk_bits(void* h);
int64_t trc_pk_sign(void* h, int64_t scheme, int64_t alg, const char* msg, int64_t mlen, char* out, int64_t cap, int64_t sig_raw);

static void tw(void* p, size_t n) { volatile u8* v = (volatile u8*)p; while (n--) *v++ = 0; }

/* AEADs live in crypto_native.c */
int64_t trc_gcm_state_size(void);
void trc_gcm_init(char* st, const char* key, int64_t klen);
int64_t trc_gcm_crypt(const char* st, int64_t enc, const char* iv12, const char* aad, int64_t alen,
                      const char* in, int64_t n, char* out, char* tag);
int64_t trc_chachapoly_crypt(const char* key, int64_t enc, const char* nonce, const char* aad, int64_t alen,
                             const char* in, int64_t n, char* out, char* tag);
#define CC(p) ((const char*)(p))
#define MC(p) ((char*)(p))
static int gcm_crypt(void* g, int enc, const u8* iv, const u8* aad, size_t alen, const u8* in, size_t n, u8* out, u8* tag) {
    return (int)trc_gcm_crypt(CC(g), enc, CC(iv), CC(aad), (int64_t)alen, CC(in), (int64_t)n, MC(out), MC(tag));
}
static int chachapoly_crypt(const u8* key, int enc, const u8* nonce, const u8* aad, size_t alen, const u8* in, size_t n, u8* out, u8* tag) {
    return (int)trc_chachapoly_crypt(CC(key), enc, CC(nonce), CC(aad), (int64_t)alen, CC(in), (int64_t)n, MC(out), MC(tag));
}

/* ═══════════════════════════ TLS 1.3 client ══════════════════════════════ */
#define TLS_MAX_REC 16384
#define TLS_MAX_CERTS 12

typedef struct {
    int suite;               /* 0x1301 / 0x1302 / 0x1303 */
    int hash, hlen, klen;
    u8 secret[48];
    u8 key[32], iv[12];
    u64 seq;
    void* gcm;               /* GCM state (crypto_native.c); NULL for ChaCha20 */
} tls_dir;

typedef struct {
    tls_sock fd; int owns_fd;
    int timeout_ms;
    char err[256];
    int state;               /* 0 handshake, 1 open, 2 closed by peer, 3 failed */
    tls_dir rd, wr;
    /* handshake */
    u8* transcript; size_t tlen, tcap;
    u8* hsbuf; size_t hslen, hscap;      /* reassembly of handshake messages */
    /* application data that arrived but has not been read */
    u8 appbuf[TLS_MAX_REC + 256]; size_t app_off, app_len;
    u8 raw[TLS_MAX_REC + 256 + 5];          /* record I/O buffers (kept off the stack) */
    u8 plain[TLS_MAX_REC + 256];
    u8 sendbuf[5 + TLS_MAX_REC + 256];
    u8 inner[TLS_MAX_REC + 1];
    char alpn[64];
    u8* certs[TLS_MAX_CERTS]; int64_t cert_lens[TLS_MAX_CERTS]; int ncerts;
    int verify; void* store; char host[256];
    int is_server;                       /* 1 for connections accepted by trc_tls_server_fd */
} tls_conn;

static void tls_fail(tls_conn* c, const char* msg) {
    if (c->state != 3) { snprintf(c->err, sizeof c->err, "%s", msg); c->state = 3; }
}

/* ── socket I/O with timeout ── */
static int sock_wait(tls_sock fd, int write, int ms) {
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
static int sock_read_full(tls_conn* c, u8* buf, size_t n) {
    size_t got = 0;
    while (got < n) {
        long r;
        if (c->timeout_ms > 0) {
            int w = sock_wait(c->fd, 0, c->timeout_ms);
            if (w == 0) return -2;
            if (w < 0) return -1;
        }
        r = (long)recv(c->fd, (char*)buf + got, (int)(n - got), 0);
        if (r == 0) return got == 0 ? 0 : -1;
        if (r < 0) {
#if defined(_WIN32)
            if (WSAGetLastError() == WSAEWOULDBLOCK) { if (sock_wait(c->fd, 0, c->timeout_ms > 0 ? c->timeout_ms : -1) == 0) return -2; continue; }
#else
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) { if (sock_wait(c->fd, 0, c->timeout_ms > 0 ? c->timeout_ms : -1) == 0) return -2; continue; }
#endif
            return -1;
        }
        got += (size_t)r;
    }
    return 1;
}
static int sock_write_all(tls_conn* c, const u8* buf, size_t n) {
    size_t sent = 0;
    while (sent < n) {
        long r;
#if defined(MSG_NOSIGNAL)
        r = (long)send(c->fd, (const char*)buf + sent, (int)(n - sent), MSG_NOSIGNAL);
#else
        r = (long)send(c->fd, (const char*)buf + sent, (int)(n - sent), 0);
#endif
        if (r <= 0) {
#if defined(_WIN32)
            if (r < 0 && WSAGetLastError() == WSAEWOULDBLOCK) { sock_wait(c->fd, 1, 1000); continue; }
#else
            if (r < 0 && errno == EINTR) continue;
            if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) { sock_wait(c->fd, 1, 1000); continue; }
#endif
            return -1;
        }
        sent += (size_t)r;
    }
    return 0;
}

/* ── buffers ── */
static int buf_append(u8** b, size_t* len, size_t* cap, const u8* d, size_t n) {
    if (*len + n > *cap) {
        size_t nc = *cap ? *cap : 4096; u8* nb;
        while (nc < *len + n) nc *= 2;
        nb = (u8*)realloc(*b, nc);
        if (!nb) return -1;
        *b = nb; *cap = nc;
    }
    memcpy(*b + *len, d, n); *len += n;
    return 0;
}

/* ── key schedule ── */
static int hkdf_label(int hash, const u8* secret, int slen, const char* label, const u8* ctx, int clen, u8* out, int olen) {
    u8 info[2 + 1 + 6 + 64 + 1 + 64]; size_t ll = strlen(label), k = 0;
    info[k++] = (u8)(olen >> 8); info[k++] = (u8)olen;
    info[k++] = (u8)(6 + ll); memcpy(info + k, "tls13 ", 6); k += 6; memcpy(info + k, label, ll); k += ll;
    info[k++] = (u8)clen; if (clen) { memcpy(info + k, ctx, (size_t)clen); k += (size_t)clen; }
    return trc_hkdf_expand(hash, (const char*)secret, slen, (const char*)info, (int64_t)k, (char*)out, olen) < 0 ? -1 : 0;
}
static void transcript_hash(tls_conn* c, int hash, u8* out) { trc_digest(hash, (const char*)c->transcript, (int64_t)c->tlen, (char*)out); }
static int derive_secret(tls_conn* c, int hash, const u8* secret, const char* label, int use_transcript, u8* out) {
    u8 h[48]; int hl = hash == 384 ? 48 : 32;
    if (use_transcript) transcript_hash(c, hash, h);
    else trc_digest(hash, "", 0, (char*)h);
    return hkdf_label(hash, secret, hl, label, h, hl, out, hl);
}
static void dir_free(tls_dir* d) { if (d->gcm) { tw(d->gcm, (size_t)trc_gcm_state_size()); free(d->gcm); d->gcm = NULL; } }
static int dir_set(tls_dir* d, int suite, const u8* secret) {
    int hash = suite == 0x1302 ? 384 : 256, hl = hash == 384 ? 48 : 32, kl = suite == 0x1301 ? 16 : 32;
    dir_free(d);
    d->suite = suite; d->hash = hash; d->hlen = hl; d->klen = kl; d->seq = 0;
    memcpy(d->secret, secret, (size_t)hl);
    if (hkdf_label(hash, secret, hl, "key", NULL, 0, d->key, kl) < 0) return -1;
    if (hkdf_label(hash, secret, hl, "iv", NULL, 0, d->iv, 12) < 0) return -1;
    if (suite != 0x1303) {
        d->gcm = malloc((size_t)trc_gcm_state_size());
        if (!d->gcm) return -1;
        trc_gcm_init((char*)d->gcm, (const char*)d->key, kl);
    }
    return 0;
}
static int dir_update(tls_dir* d) {   /* KeyUpdate: next generation of the traffic secret */
    u8 ns[48];
    if (hkdf_label(d->hash, d->secret, d->hlen, "traffic upd", NULL, 0, ns, d->hlen) < 0) return -1;
    return dir_set(d, d->suite, ns);
}
static void dir_nonce(const tls_dir* d, u8* nonce) {
    int i; memcpy(nonce, d->iv, 12);
    for (i = 0; i < 8; i++) nonce[11 - i] ^= (u8)(d->seq >> (8 * i));
}

/* ── record layer ── */
static int send_record(tls_conn* c, int type, const u8* data, size_t n) {
    u8* rec = c->sendbuf;
    while (1) {
        size_t chunk = n > TLS_MAX_REC ? TLS_MAX_REC : n;
        if (!c->wr.suite) {
            rec[0] = (u8)type; rec[1] = 3; rec[2] = (type == 22 && !c->is_server) ? 1 : 3;
            rec[3] = (u8)(chunk >> 8); rec[4] = (u8)chunk;
            memcpy(rec + 5, data, chunk);
            if (sock_write_all(c, rec, 5 + chunk) < 0) { tls_fail(c, "TLS: write failed"); return -1; }
        } else {
            u8* inner = c->inner; u8 nonce[12]; size_t ilen = chunk + 1, clen = ilen + 16;
            memcpy(inner, data, chunk); inner[chunk] = (u8)type;
            rec[0] = 23; rec[1] = 3; rec[2] = 3; rec[3] = (u8)(clen >> 8); rec[4] = (u8)clen;
            dir_nonce(&c->wr, nonce);
            if (c->wr.suite == 0x1303) chachapoly_crypt(c->wr.key, 1, nonce, rec, 5, inner, ilen, rec + 5, rec + 5 + ilen);
            else gcm_crypt(c->wr.gcm, 1, nonce, rec, 5, inner, ilen, rec + 5, rec + 5 + ilen);
            c->wr.seq++;
            if (sock_write_all(c, rec, 5 + clen) < 0) { tls_fail(c, "TLS: write failed"); return -1; }
        }
        data += chunk; n -= chunk;
        if (n == 0) break;
    }
    return 0;
}
static void send_alert(tls_conn* c, int level, int desc) { u8 a[2]; a[0] = (u8)level; a[1] = (u8)desc; send_record(c, 21, a, 2); }

/* Read one record; returns its content type and fills out/outlen with the
 * plaintext. -1 error (c->err set), -2 timeout, 0 clean EOF. */
static int read_record(tls_conn* c, u8** outp, size_t* outlen) {
    u8 hdr[5]; u8* body = c->raw; u8* out = c->plain; size_t len; int r;
    *outp = out;
    for (;;) {
        r = sock_read_full(c, hdr, 5);
        if (r == 0) return 0;
        if (r == -2) return -2;
        if (r < 0) { tls_fail(c, "TLS: connection closed during record header"); return -1; }
        len = ((size_t)hdr[3] << 8) | hdr[4];
        if (len > TLS_MAX_REC + 256) { tls_fail(c, "TLS: record too long"); return -1; }
        r = sock_read_full(c, body, len);
        if (r <= 0) { tls_fail(c, r == -2 ? "TLS: timed out" : "TLS: connection closed during record"); return r == -2 ? -2 : -1; }
        if (hdr[0] == 20) continue;                       /* ChangeCipherSpec (compat) is ignored */
        if (!c->rd.suite || hdr[0] != 23) {
            if (c->rd.suite && hdr[0] != 21) { tls_fail(c, "TLS: unexpected plaintext record"); return -1; }
            memcpy(out, body, len); *outlen = len;
            return hdr[0];
        }
        {
            u8 nonce[12]; size_t plen; int ok;
            if (len < 17) { tls_fail(c, "TLS: short encrypted record"); return -1; }
            plen = len - 16;
            dir_nonce(&c->rd, nonce);
            if (c->rd.suite == 0x1303) ok = chachapoly_crypt(c->rd.key, 0, nonce, hdr, 5, body, plen, out, body + plen);
            else ok = gcm_crypt(c->rd.gcm, 0, nonce, hdr, 5, body, plen, out, body + plen);
            if (ok < 0) { send_alert(c, 2, 20); tls_fail(c, "TLS: record authentication failed (bad_record_mac)"); return -1; }
            c->rd.seq++;
            while (plen > 0 && out[plen - 1] == 0) plen--;
            if (plen == 0) { tls_fail(c, "TLS: record without content type"); return -1; }
            *outlen = plen - 1;
            return out[plen - 1];
        }
    }
}

/* Alert handling shared by handshake and data paths. */
static int on_alert(tls_conn* c, const u8* a, size_t n) {
    if (n < 2) { tls_fail(c, "TLS: malformed alert"); return -1; }
    if (a[1] == 0) { c->state = 2; return 0; }   /* close_notify */
    {
        const char* name = "alert"; char m[160];
        switch (a[1]) {
            case 10: name = "unexpected_message"; break; case 20: name = "bad_record_mac"; break;
            case 40: name = "handshake_failure (the server may not support TLS 1.3)"; break;
            case 42: name = "bad_certificate"; break; case 45: name = "certificate_expired"; break;
            case 47: name = "illegal_parameter"; break; case 48: name = "unknown_ca"; break;
            case 50: name = "decode_error"; break; case 51: name = "decrypt_error"; break;
            case 70: name = "protocol_version (the server does not support TLS 1.3)"; break;
            case 80: name = "internal_error"; break; case 109: name = "missing_extension"; break;
            case 110: name = "unsupported_extension"; break; case 112: name = "unrecognized_name"; break;
            case 116: name = "certificate_required"; break; case 120: name = "no_application_protocol"; break;
        }
        snprintf(m, sizeof m, "TLS: server sent alert %d: %s", a[1], name);
        tls_fail(c, m);
    }
    return -1;
}

/* Next complete handshake message: type in *type, body (without the 4-byte
 * header) copied to *msg (malloc'd). The header+body go into the transcript
 * unless `no_transcript`. */
static int next_hs(tls_conn* c, int* type, u8** msg, size_t* mlen) {
    u8* rec; size_t rlen; int t;
    for (;;) {
        if (c->hslen >= 4) {
            size_t l = ((size_t)c->hsbuf[1] << 16) | ((size_t)c->hsbuf[2] << 8) | c->hsbuf[3];
            if (l > (1 << 20)) { tls_fail(c, "TLS: handshake message too large"); return -1; }
            if (c->hslen >= 4 + l) {
                *type = c->hsbuf[0];
                *msg = (u8*)malloc(l + 1);
                if (!*msg) return -1;
                memcpy(*msg, c->hsbuf + 4, l); *mlen = l;
                if (buf_append(&c->transcript, &c->tlen, &c->tcap, c->hsbuf, 4 + l) < 0) { free(*msg); return -1; }
                memmove(c->hsbuf, c->hsbuf + 4 + l, c->hslen - 4 - l); c->hslen -= 4 + l;
                return 0;
            }
        }
        t = read_record(c, &rec, &rlen);
        if (t <= 0) { if (t == 0) tls_fail(c, "TLS: connection closed during handshake"); return -1; }
        if (t == 21) { on_alert(c, rec, rlen); if (c->state != 3) tls_fail(c, "TLS: connection closed during handshake"); return -1; }
        if (t != 22) { tls_fail(c, "TLS: unexpected record during handshake"); return -1; }
        if (buf_append(&c->hsbuf, &c->hslen, &c->hscap, rec, rlen) < 0) return -1;
    }
}
static int send_hs(tls_conn* c, int type, const u8* body, size_t n) {
    u8* m = (u8*)malloc(n + 4); int r;
    if (!m) return -1;
    m[0] = (u8)type; m[1] = (u8)(n >> 16); m[2] = (u8)(n >> 8); m[3] = (u8)n;
    memcpy(m + 4, body, n);
    if (buf_append(&c->transcript, &c->tlen, &c->tcap, m, n + 4) < 0) { free(m); return -1; }
    r = send_record(c, 22, m, n + 4);
    free(m);
    return r;
}

/* ── ClientHello ── */
static size_t put16(u8* p, size_t k, unsigned v) { p[k] = (u8)(v >> 8); p[k + 1] = (u8)v; return k + 2; }
static size_t build_client_hello(tls_conn* c, u8* out, const u8* random, const u8* sid, const u8* x25519_pub,
                                 const u8* p256_pub, const u8* p384_pub, int only_group, const char* alpn, const u8* cookie, size_t cookie_len) {
    size_t k = 0, ext_start, e; size_t hl = strlen(c->host);
    k = put16(out, k, 0x0303);
    memcpy(out + k, random, 32); k += 32;
    out[k++] = 32; memcpy(out + k, sid, 32); k += 32;
    k = put16(out, k, 6); k = put16(out, k, 0x1301); k = put16(out, k, 0x1303); k = put16(out, k, 0x1302);
    out[k++] = 1; out[k++] = 0;
    ext_start = k; k += 2;
    if (hl > 0 && !strchr(c->host, ':')) {
        int is_ip = 1; size_t i;
        for (i = 0; i < hl; i++) if (!((c->host[i] >= '0' && c->host[i] <= '9') || c->host[i] == '.')) is_ip = 0;
        if (!is_ip) {   /* server_name: IP literals are not sent (RFC 6066 section 3) */
            k = put16(out, k, 0); k = put16(out, k, (unsigned)(hl + 5)); k = put16(out, k, (unsigned)(hl + 3));
            out[k++] = 0; k = put16(out, k, (unsigned)hl); memcpy(out + k, c->host, hl); k += hl;
        }
    }
    /* supported_groups */
    k = put16(out, k, 10); k = put16(out, k, 8); k = put16(out, k, 6);
    k = put16(out, k, 0x001d); k = put16(out, k, 0x0017); k = put16(out, k, 0x0018);
    /* signature_algorithms */
    {
        static const unsigned sa[] = {0x0403, 0x0503, 0x0804, 0x0805, 0x0806, 0x0401, 0x0501, 0x0601};
        int i, n = (int)(sizeof sa / sizeof sa[0]);
        k = put16(out, k, 13); k = put16(out, k, (unsigned)(2 + 2 * n)); k = put16(out, k, (unsigned)(2 * n));
        for (i = 0; i < n; i++) k = put16(out, k, sa[i]);
    }
    /* supported_versions: TLS 1.3 only */
    k = put16(out, k, 43); k = put16(out, k, 3); out[k++] = 2; k = put16(out, k, 0x0304);
    /* psk_key_exchange_modes (psk_dhe_ke) -- harmless, some servers expect it */
    k = put16(out, k, 45); k = put16(out, k, 2); out[k++] = 1; out[k++] = 1;
    /* key_share */
    {
        size_t ks_len = 0;
        if (only_group == 0 || only_group == 0x1d) ks_len += 4 + 32;
        if (only_group == 0 || only_group == 0x17) ks_len += 4 + 65;
        if (only_group == 0x18) ks_len += 4 + 97;
        k = put16(out, k, 51); k = put16(out, k, (unsigned)(ks_len + 2)); k = put16(out, k, (unsigned)ks_len);
        if (only_group == 0 || only_group == 0x1d) { k = put16(out, k, 0x1d); k = put16(out, k, 32); memcpy(out + k, x25519_pub, 32); k += 32; }
        if (only_group == 0 || only_group == 0x17) { k = put16(out, k, 0x17); k = put16(out, k, 65); memcpy(out + k, p256_pub, 65); k += 65; }
        if (only_group == 0x18) { k = put16(out, k, 0x18); k = put16(out, k, 97); memcpy(out + k, p384_pub, 97); k += 97; }
    }
    /* ALPN: comma-separated list */
    if (alpn && *alpn) {
        size_t lstart, pos = 0, al = strlen(alpn);
        k = put16(out, k, 16); lstart = k; k += 4;
        while (pos < al) {
            size_t endp = pos;
            while (endp < al && alpn[endp] != ',') endp++;
            if (endp > pos && endp - pos < 256) { out[k++] = (u8)(endp - pos); memcpy(out + k, alpn + pos, endp - pos); k += endp - pos; }
            pos = endp + 1;
        }
        put16(out, lstart, (unsigned)(k - lstart - 2)); put16(out, lstart + 2, (unsigned)(k - lstart - 4));
    }
    if (cookie_len) { k = put16(out, k, 44); k = put16(out, k, (unsigned)cookie_len); memcpy(out + k, cookie, cookie_len); k += cookie_len; }
    e = k - ext_start - 2;
    put16(out, ext_start, (unsigned)e);
    return k;
}

static const u8 HRR_RANDOM[32] = {
    0xCF,0x21,0xAD,0x74,0xE5,0x9A,0x61,0x11,0xBE,0x1D,0x8C,0x02,0x1E,0x65,0xB8,0x91,
    0xC2,0xA2,0x11,0x16,0x7A,0xBB,0x8C,0x5E,0x07,0x9E,0x09,0xE2,0xC8,0xA8,0x33,0x9C };

typedef struct { int suite, group, version; const u8* share; size_t share_len; const u8* cookie; size_t cookie_len; int hrr; } sh_info;

static int parse_server_hello(tls_conn* c, const u8* m, size_t n, const u8* sid, sh_info* s) {
    size_t k = 0, el, end;
    memset(s, 0, sizeof *s);
    if (n < 38) goto bad;
    k = 2;                                            /* legacy_version */
    s->hrr = memcmp(m + k, HRR_RANDOM, 32) == 0; k += 32;
    if (m[k] != 32 || k + 1 + 32 > n || memcmp(m + k + 1, sid, 32) != 0) { tls_fail(c, "TLS: server echoed a different session id"); return -1; }
    k += 33;
    if (k + 3 > n) goto bad;
    s->suite = (m[k] << 8) | m[k + 1]; k += 2;
    if (m[k++] != 0) goto bad;
    if (k + 2 > n) goto bad;
    el = ((size_t)m[k] << 8) | m[k + 1]; k += 2;
    if (k + el != n) goto bad;
    end = k + el;
    while (k + 4 <= end) {
        unsigned et = ((unsigned)m[k] << 8) | m[k + 1]; size_t l = ((size_t)m[k + 2] << 8) | m[k + 3];
        const u8* d = m + k + 4;
        k += 4;
        if (k + l > end) goto bad;
        if (et == 43 && l == 2) s->version = (d[0] << 8) | d[1];
        else if (et == 51) {
            if (s->hrr) { if (l != 2) goto bad; s->group = (d[0] << 8) | d[1]; }
            else { size_t sl; if (l < 4) goto bad; s->group = (d[0] << 8) | d[1]; sl = ((size_t)d[2] << 8) | d[3]; if (4 + sl != l) goto bad; s->share = d + 4; s->share_len = sl; }
        } else if (et == 44) {
            if (l < 2) goto bad;
            s->cookie = d + 2; s->cookie_len = ((size_t)d[0] << 8) | d[1];
            if (s->cookie_len + 2 != l) goto bad;
        }
        k += l;
    }
    if (s->version != 0x0304) { tls_fail(c, "TLS: server does not support TLS 1.3"); return -1; }
    if (s->suite != 0x1301 && s->suite != 0x1302 && s->suite != 0x1303) { tls_fail(c, "TLS: server chose an unsupported cipher suite"); return -1; }
    return 0;
bad:
    tls_fail(c, "TLS: malformed ServerHello");
    return -1;
}

static int handshake(tls_conn* c, const char* alpn) {
    u8 random[32], sid[32], x_priv[32], x_pub[32], base[32] = {9}, p256_pub[65], ch[2048], shared[48];
    void* p256 = NULL; void* p384 = NULL; u8 p384_pub[97]; int type, only_group = 0, hrr_done = 0, suite = 0, hash = 256, hl = 32, rc = -1;
    u8 *msg = NULL; size_t mlen, chlen; sh_info sh;
    u8 early[48], derived[48], hs_secret[48], c_hs[48], s_hs[48], master[48], c_ap[48], s_ap[48], zeros[48], th[48];
    memset(zeros, 0, sizeof zeros);
    if (trc_random((char*)random, 32) != 0 || trc_random((char*)sid, 32) != 0 || trc_random((char*)x_priv, 32) != 0) {
        tls_fail(c, "TLS: OS random generator unavailable"); return -1;
    }
    trc_x25519((char*)x_pub, (const char*)x_priv, (const char*)base);
    p256 = trc_pk_ec_generate(256);
    if (!p256 || trc_pk_public_raw(p256, (char*)p256_pub, 65) != 65) { tls_fail(c, "TLS: key generation failed"); goto out; }

    chlen = build_client_hello(c, ch, random, sid, x_pub, p256_pub, NULL, 0, alpn, NULL, 0);
    if (send_hs(c, 1, ch, chlen) < 0) goto out;

    for (;;) {
        if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
        if (type != 2) { tls_fail(c, "TLS: expected ServerHello"); goto out; }
        if (parse_server_hello(c, msg, mlen, sid, &sh) < 0) goto out;
        if (!sh.hrr) break;
        /* HelloRetryRequest: transcript becomes message_hash(CH1) || HRR */
        if (hrr_done || (sh.group != 0x1d && sh.group != 0x17 && sh.group != 0x18)) { tls_fail(c, "TLS: unsupported HelloRetryRequest"); goto out; }
        if (sh.group == 0x18) {
            p384 = trc_pk_ec_generate(384);
            if (!p384 || trc_pk_public_raw(p384, (char*)p384_pub, 97) != 97) { tls_fail(c, "TLS: key generation failed"); goto out; }
        }
        hrr_done = 1; only_group = sh.group;
        suite = sh.suite; hash = suite == 0x1302 ? 384 : 256; hl = hash == 384 ? 48 : 32;
        {
            u8 h1[48], mh[4 + 48]; size_t ch1_len = 4 + chlen, hrr_len = c->tlen - ch1_len; u8* hrr = (u8*)malloc(hrr_len);
            if (!hrr) goto out;
            memcpy(hrr, c->transcript + ch1_len, hrr_len);
            trc_digest(hash, (const char*)c->transcript, (int64_t)ch1_len, (char*)h1);
            mh[0] = 254; mh[1] = 0; mh[2] = 0; mh[3] = (u8)hl; memcpy(mh + 4, h1, (size_t)hl);
            c->tlen = 0;
            buf_append(&c->transcript, &c->tlen, &c->tcap, mh, (size_t)(4 + hl));
            buf_append(&c->transcript, &c->tlen, &c->tcap, hrr, hrr_len);
            free(hrr);
        }
        {
            u8 cookie[1024]; size_t cl = sh.cookie_len < sizeof cookie ? sh.cookie_len : 0;
            if (cl) memcpy(cookie, sh.cookie, cl);
            free(msg); msg = NULL;
            send_record(c, 20, (const u8*)"\x01", 1);    /* compat CCS before the second ClientHello */
            chlen = build_client_hello(c, ch, random, sid, x_pub, p256_pub, p384_pub, only_group, alpn, cl ? cookie : NULL, cl);
            if (send_hs(c, 1, ch, chlen) < 0) goto out;
        }
    }
    if (hrr_done && sh.suite != suite) { tls_fail(c, "TLS: cipher suite changed after HelloRetryRequest"); goto out; }
    suite = sh.suite; hash = suite == 0x1302 ? 384 : 256; hl = hash == 384 ? 48 : 32;
    /* ECDHE */
    {
        size_t slen = 0;
        if (sh.group == 0x1d && (only_group == 0 || only_group == 0x1d)) {
            if (sh.share_len != 32 || trc_x25519((char*)shared, (const char*)x_priv, (const char*)sh.share) != 0) { tls_fail(c, "TLS: bad X25519 key share"); goto out; }
            slen = 32;
        } else if (sh.group == 0x17 && (only_group == 0 || only_group == 0x17)) {
            if (trc_pk_ecdh(p256, (const char*)sh.share, (int64_t)sh.share_len, (char*)shared) != 32) { tls_fail(c, "TLS: bad P-256 key share"); goto out; }
            slen = 32;
        } else if (sh.group == 0x18 && only_group == 0x18) {
            if (trc_pk_ecdh(p384, (const char*)sh.share, (int64_t)sh.share_len, (char*)shared) != 48) { tls_fail(c, "TLS: bad P-384 key share"); goto out; }
            slen = 48;
        } else { tls_fail(c, "TLS: server picked a key share we did not send"); goto out; }
        free(msg); msg = NULL;
        trc_hkdf_extract(hash, (const char*)zeros, hl, (const char*)zeros, hl, (char*)early);
        derive_secret(c, hash, early, "derived", 0, derived);
        trc_hkdf_extract(hash, (const char*)derived, hl, (const char*)shared, (int64_t)slen, (char*)hs_secret);
        derive_secret(c, hash, hs_secret, "c hs traffic", 1, c_hs);
        derive_secret(c, hash, hs_secret, "s hs traffic", 1, s_hs);
        if (dir_set(&c->rd, suite, s_hs) < 0) goto out;
    }
    /* EncryptedExtensions */
    if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
    if (type != 8) { tls_fail(c, "TLS: expected EncryptedExtensions"); goto out; }
    {
        size_t k = 2, el = mlen >= 2 ? ((size_t)msg[0] << 8 | msg[1]) : 0;
        if (2 + el != mlen) { tls_fail(c, "TLS: malformed EncryptedExtensions"); goto out; }
        while (k + 4 <= mlen) {
            unsigned et = ((unsigned)msg[k] << 8) | msg[k + 1]; size_t l = ((size_t)msg[k + 2] << 8) | msg[k + 3];
            if (k + 4 + l > mlen) { tls_fail(c, "TLS: malformed EncryptedExtensions"); goto out; }
            if (et == 16 && l >= 3) {
                size_t pl = msg[k + 6];
                if (pl + 3 <= l && pl < sizeof c->alpn) { memcpy(c->alpn, msg + k + 7, pl); c->alpn[pl] = 0; }
            }
            k += 4 + l;
        }
    }
    free(msg); msg = NULL;
    /* [CertificateRequest] Certificate CertificateVerify Finished */
    {
        int cert_requested = 0; u8 req_ctx[256]; size_t req_ctx_len = 0;
        if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
        if (type == 13) {
            cert_requested = 1;
            if (mlen >= 1 && 1 + (size_t)msg[0] <= mlen) { req_ctx_len = msg[0]; memcpy(req_ctx, msg + 1, req_ctx_len); }
            free(msg); msg = NULL;
            if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
        }
        if (type != 11) { tls_fail(c, "TLS: expected Certificate"); goto out; }
        {
            size_t k, listlen, end;
            if (mlen < 4 || msg[0] + 4u > mlen) { tls_fail(c, "TLS: malformed Certificate"); goto out; }
            k = 1 + (size_t)msg[0];
            listlen = ((size_t)msg[k] << 16) | ((size_t)msg[k + 1] << 8) | msg[k + 2]; k += 3;
            if (k + listlen != mlen) { tls_fail(c, "TLS: malformed Certificate"); goto out; }
            end = k + listlen;
            while (k + 3 <= end) {
                size_t cl = ((size_t)msg[k] << 16) | ((size_t)msg[k + 1] << 8) | msg[k + 2], xl;
                k += 3;
                if (k + cl + 2 > end) { tls_fail(c, "TLS: malformed Certificate entry"); goto out; }
                if (c->ncerts < TLS_MAX_CERTS) {
                    c->certs[c->ncerts] = (u8*)malloc(cl ? cl : 1);
                    if (!c->certs[c->ncerts]) goto out;
                    memcpy(c->certs[c->ncerts], msg + k, cl); c->cert_lens[c->ncerts] = (int64_t)cl; c->ncerts++;
                }
                k += cl;
                xl = ((size_t)msg[k] << 8) | msg[k + 1]; k += 2 + xl;
                if (k > end) { tls_fail(c, "TLS: malformed Certificate entry"); goto out; }
            }
            if (c->ncerts == 0) { tls_fail(c, "TLS: server sent no certificate"); goto out; }
        }
        free(msg); msg = NULL;
        if (c->verify) {
            u8* all; size_t tot = 0; int i; int64_t vr; size_t off = 0;
            for (i = 0; i < c->ncerts; i++) tot += (size_t)c->cert_lens[i];
            all = (u8*)malloc(tot ? tot : 1);
            if (!all) goto out;
            for (i = 0; i < c->ncerts; i++) { memcpy(all + off, c->certs[i], (size_t)c->cert_lens[i]); off += (size_t)c->cert_lens[i]; }
            vr = trc_x509_verify_chain(c->store, (const char*)all, c->cert_lens, c->ncerts, c->host, (int64_t)strlen(c->host), 0, 1);
            free(all);
            if (vr != 0) {
                char m[200], et[120]; int64_t n = trc_x509_error_text(vr, et, sizeof et - 1); et[n] = 0;
                snprintf(m, sizeof m, "TLS: certificate verification failed: %s", et);
                send_alert(c, 2, vr == -2 ? 45 : vr == -4 ? 48 : 42);
                tls_fail(c, m); goto out;
            }
        }
        /* CertificateVerify over transcript through Certificate */
        transcript_hash(c, hash, th);
        if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
        if (type != 15 || mlen < 4) { tls_fail(c, "TLS: expected CertificateVerify"); goto out; }
        {
            unsigned scheme = ((unsigned)msg[0] << 8) | msg[1]; size_t sl = ((size_t)msg[2] << 8) | msg[3];
            u8 content[64 + 34 + 48]; static const char ctx[] = "TLS 1.3, server CertificateVerify";
            if (4 + sl != mlen) { tls_fail(c, "TLS: malformed CertificateVerify"); goto out; }
            memset(content, 0x20, 64); memcpy(content + 64, ctx, 33); content[97] = 0; memcpy(content + 98, th, (size_t)hl);
            if (scheme == 0x0401 || scheme == 0x0501 || scheme == 0x0601) { send_alert(c, 2, 47); tls_fail(c, "TLS: server used PKCS#1 v1.5 in CertificateVerify"); goto out; }
            if (!trc_x509_verify_tls_sig((const char*)c->certs[0], c->cert_lens[0], scheme, (const char*)content, 98 + hl, (const char*)msg + 4, (int64_t)sl)) {
                send_alert(c, 2, 51); tls_fail(c, "TLS: server CertificateVerify signature is invalid"); goto out;
            }
        }
        free(msg); msg = NULL;
        /* server Finished */
        {
            u8 fk[48], want[48];
            transcript_hash(c, hash, th);
            hkdf_label(hash, s_hs, hl, "finished", NULL, 0, fk, hl);
            trc_hmac(hash, (const char*)fk, hl, (const char*)th, hl, (char*)want);
            if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
            if (type != 20 || mlen != (size_t)hl || !trc_ct_eq((const char*)msg, hl, (const char*)want, hl)) {
                send_alert(c, 2, 51); tls_fail(c, "TLS: server Finished does not verify"); goto out;
            }
            free(msg); msg = NULL;
        }
        /* application secrets from transcript through server Finished */
        derive_secret(c, hash, hs_secret, "derived", 0, derived);
        trc_hkdf_extract(hash, (const char*)derived, hl, (const char*)zeros, hl, (char*)master);
        derive_secret(c, hash, master, "c ap traffic", 1, c_ap);
        derive_secret(c, hash, master, "s ap traffic", 1, s_ap);
        /* client flight */
        send_record(c, 20, (const u8*)"\x01", 1);
        if (dir_set(&c->wr, suite, c_hs) < 0) goto out;
        if (cert_requested) {
            u8 body[260 + 4]; size_t bl = 0;
            body[bl++] = (u8)req_ctx_len; memcpy(body + bl, req_ctx, req_ctx_len); bl += req_ctx_len;
            body[bl++] = 0; body[bl++] = 0; body[bl++] = 0;
            if (send_hs(c, 11, body, bl) < 0) goto out;
        }
        {
            u8 fk[48], vd[48];
            transcript_hash(c, hash, th);
            hkdf_label(hash, c_hs, hl, "finished", NULL, 0, fk, hl);
            trc_hmac(hash, (const char*)fk, hl, (const char*)th, hl, (char*)vd);
            if (send_hs(c, 20, vd, (size_t)hl) < 0) goto out;
        }
        if (dir_set(&c->wr, suite, c_ap) < 0 || dir_set(&c->rd, suite, s_ap) < 0) goto out;
    }
    c->state = 1;
    rc = 0;
out:
    free(msg);
    if (p256) trc_pk_free(p256);
    if (p384) trc_pk_free(p384);
    tw(x_priv, sizeof x_priv); tw(shared, sizeof shared); tw(early, sizeof early); tw(hs_secret, sizeof hs_secret);
    tw(master, sizeof master); tw(c_hs, sizeof c_hs); tw(s_hs, sizeof s_hs); tw(c_ap, sizeof c_ap); tw(s_ap, sizeof s_ap);
    free(c->transcript); c->transcript = NULL; c->tlen = c->tcap = 0;
    return rc;
}

/* Post-handshake messages (NewSessionTicket, KeyUpdate). */
static int on_post_handshake(tls_conn* c, const u8* d, size_t n) {
    if (buf_append(&c->hsbuf, &c->hslen, &c->hscap, d, n) < 0) return -1;
    while (c->hslen >= 4) {
        size_t l = ((size_t)c->hsbuf[1] << 16) | ((size_t)c->hsbuf[2] << 8) | c->hsbuf[3]; int t = c->hsbuf[0];
        if (c->hslen < 4 + l) break;
        if (t == 24) {   /* KeyUpdate */
            int req = l == 1 ? c->hsbuf[4] : -1;
            if (req != 0 && req != 1) { tls_fail(c, "TLS: malformed KeyUpdate"); return -1; }
            if (dir_update(&c->rd) < 0) return -1;
            if (req == 1) {
                u8 m[5] = {24, 0, 0, 1, 0};
                if (send_record(c, 22, m, 5) < 0 || dir_update(&c->wr) < 0) return -1;
            }
        } else if (t != 4) {
            tls_fail(c, "TLS: unexpected post-handshake message"); return -1;
        }
        memmove(c->hsbuf, c->hsbuf + 4 + l, c->hslen - 4 - l); c->hslen -= 4 + l;
    }
    return 0;
}

#if defined(_WIN32)
static void tls_wsa(void) { static int done = 0; if (!done) { WSADATA w; WSAStartup(MAKEWORD(2, 2), &w); done = 1; } }
#endif

static tls_conn* conn_new(const char* host, int64_t hostlen, void* store, int64_t verify, int64_t timeout_ms) {
    tls_conn* c = (tls_conn*)calloc(1, sizeof(tls_conn));
    if (!c) return NULL;
    if (hostlen > 255) hostlen = 255;
    memcpy(c->host, host, (size_t)hostlen); c->host[hostlen] = 0;
    c->store = store; c->verify = (int)verify; c->timeout_ms = (int)timeout_ms;
    c->fd = TLS_BADSOCK;
    return c;
}

/* Run the handshake over an already-connected socket. Returns a handle (check
 * trc_tls_ok) -- even on failure, so the caller can read the error. The fd is
 * not closed by trc_tls_free unless owns_fd. */
void* trc_tls_client_fd(int64_t fd, const char* host, int64_t hostlen, void* store, int64_t verify,
                        const char* alpn, int64_t timeout_ms, int64_t owns_fd) {
    tls_conn* c = conn_new(host, hostlen, store, verify, timeout_ms);
    if (!c) return NULL;
    c->fd = (tls_sock)fd; c->owns_fd = (int)owns_fd;
    if (verify && !store) { tls_fail(c, "TLS: verification requested without a trust store"); return c; }
    handshake(c, alpn);
    return c;
}

/* Resolve + connect (with timeout) + handshake. */
void* trc_tls_connect(const char* host, int64_t hostlen, int64_t port, void* store, int64_t verify,
                      const char* alpn, int64_t timeout_ms) {
    tls_conn* c = conn_new(host, hostlen, store, verify, timeout_ms);
    struct addrinfo hints, *res = NULL, *ai; char portstr[16]; int gai; char hbuf[256];
    if (!c) return NULL;
#if defined(_WIN32)
    tls_wsa();
#endif
    memcpy(hbuf, c->host, sizeof hbuf);
    if (hbuf[0] == '[') { size_t l = strlen(hbuf); if (l > 2 && hbuf[l - 1] == ']') { memmove(hbuf, hbuf + 1, l - 2); hbuf[l - 2] = 0; memmove(c->host, hbuf, l - 1); } }
    snprintf(portstr, sizeof portstr, "%d", (int)port);
    memset(&hints, 0, sizeof hints); hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;
    gai = getaddrinfo(hbuf, portstr, &hints, &res);
    if (gai != 0 || !res) { tls_fail(c, "TLS: could not resolve host"); return c; }
    for (ai = res; ai; ai = ai->ai_next) {
        tls_sock s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol); int ok = 0;
        if (s == TLS_BADSOCK) continue;
#if defined(_WIN32)
        { u_long nb = 1; ioctlsocket(s, FIONBIO, &nb);
          if (connect(s, ai->ai_addr, (int)ai->ai_addrlen) == 0 || WSAGetLastError() == WSAEWOULDBLOCK) {
              if (sock_wait(s, 1, timeout_ms > 0 ? (int)timeout_ms : -1) > 0) { int e = 0; int el = sizeof e; getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&e, &el); ok = e == 0; } }
          nb = 0; ioctlsocket(s, FIONBIO, &nb); }
#else
        { int fl = fcntl(s, F_GETFL, 0); fcntl(s, F_SETFL, fl | O_NONBLOCK);
          if (connect(s, ai->ai_addr, ai->ai_addrlen) == 0) ok = 1;
          else if (errno == EINPROGRESS && sock_wait(s, 1, timeout_ms > 0 ? (int)timeout_ms : -1) > 0) {
              int e = 0; socklen_t el = sizeof e; getsockopt(s, SOL_SOCKET, SO_ERROR, &e, &el); ok = e == 0; }
          fcntl(s, F_SETFL, fl); }
#endif
        if (ok) { int one = 1; setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof one); c->fd = s; break; }
        tls_closesock(s);
    }
    freeaddrinfo(res);
    if (c->fd == TLS_BADSOCK) { tls_fail(c, "TLS: could not connect"); return c; }
    c->owns_fd = 1;
    if (verify && !store) { tls_fail(c, "TLS: verification requested without a trust store"); return c; }
    handshake(c, alpn);
    return c;
}

int64_t trc_tls_ok(void* h) { tls_conn* c = (tls_conn*)h; return c && (c->state == 1 || c->state == 2); }
int64_t trc_tls_error(void* h, char* out, int64_t cap) {
    tls_conn* c = (tls_conn*)h; size_t n;
    if (!c) { n = strlen("TLS: out of memory"); memcpy(out, "TLS: out of memory", n); return (int64_t)n; }
    n = strlen(c->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, c->err, n); return (int64_t)n;
}
void trc_tls_set_timeout(void* h, int64_t ms) { if (h) ((tls_conn*)h)->timeout_ms = (int)ms; }
int64_t trc_tls_write(void* h, const char* d, int64_t n) {
    tls_conn* c = (tls_conn*)h;
    if (!c || c->state != 1) return -1;
    if (n == 0) return 0;
    return send_record(c, 23, (const u8*)d, (size_t)n) < 0 ? -1 : n;
}
/* Returns bytes read, 0 at clean EOF (close_notify or FIN), -1 error, -2 timeout. */
int64_t trc_tls_read(void* h, char* out, int64_t cap) {
    tls_conn* c = (tls_conn*)h; u8* rec; size_t rlen; int t;
    if (!c) return -1;
    for (;;) {
        if (c->app_len > 0) {
            size_t k = c->app_len < (size_t)cap ? c->app_len : (size_t)cap;
            memcpy(out, c->appbuf + c->app_off, k); c->app_off += k; c->app_len -= k;
            return (int64_t)k;
        }
        if (c->state == 2) return 0;
        if (c->state != 1) return -1;
        t = read_record(c, &rec, &rlen);
        if (t == 0) { c->state = 2; return 0; }
        if (t == -2) return -2;
        if (t < 0) return -1;
        if (t == 23) { memcpy(c->appbuf, rec, rlen); c->app_off = 0; c->app_len = rlen; continue; }
        if (t == 22) { if (on_post_handshake(c, rec, rlen) < 0) return -1; continue; }
        if (t == 21) { if (on_alert(c, rec, rlen) < 0) return -1; continue; }
        tls_fail(c, "TLS: unexpected record type"); return -1;
    }
}
int64_t trc_tls_alpn(void* h, char* out, int64_t cap) {
    tls_conn* c = (tls_conn*)h; size_t n;
    if (!c) return 0;
    n = strlen(c->alpn); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, c->alpn, n); return (int64_t)n;
}
int64_t trc_tls_cipher(void* h) { tls_conn* c = (tls_conn*)h; return c ? c->rd.suite : 0; }
int64_t trc_tls_peer_cert_count(void* h) { return h ? ((tls_conn*)h)->ncerts : 0; }
int64_t trc_tls_peer_cert(void* h, int64_t i, char* out, int64_t cap) {
    tls_conn* c = (tls_conn*)h;
    if (!c || i < 0 || i >= c->ncerts) return -1;
    if (!out) return c->cert_lens[i];
    if (c->cert_lens[i] > cap) return -1;
    memcpy(out, c->certs[i], (size_t)c->cert_lens[i]);
    return c->cert_lens[i];
}
int64_t trc_tls_fd(void* h) { return h ? (int64_t)((tls_conn*)h)->fd : -1; }
/* Send close_notify (if open), close the socket when owned, free everything. */
void trc_tls_close(void* h) {
    tls_conn* c = (tls_conn*)h; int i;
    if (!c) return;
    if (c->state == 1) send_alert(c, 1, 0);
    if (c->owns_fd && c->fd != TLS_BADSOCK) tls_closesock(c->fd);
    dir_free(&c->rd); dir_free(&c->wr);
    for (i = 0; i < c->ncerts; i++) free(c->certs[i]);
    free(c->hsbuf); free(c->transcript);
    tw(c, sizeof *c); free(c);
}


/* ═══════════════════════════ TLS 1.3 server ══════════════════════════════
 *
 *   key exchange ...... X25519, secp256r1, secp384r1 (HelloRetryRequest when
 *                       the client offered the group without a key share)
 *   cipher suites ..... TLS_AES_128_GCM_SHA256, TLS_CHACHA20_POLY1305_SHA256,
 *                       TLS_AES_256_GCM_SHA384 (server preference order)
 *   certificates ...... RSA (rsa_pss_rsae_sha256/384/512), ECDSA P-256 / P-384
 *   extras ............ ALPN (server preference), SNI (recorded), KeyUpdate
 *   not supported ..... client certificates, PSK / resumption, 0-RTT
 */
typedef struct {
    u8* certs[TLS_MAX_CERTS]; int64_t cert_lens[TLS_MAX_CERTS]; int ncerts;
    void* key; int key_kind;           /* 1 RSA, 2 P-256, 3 P-384 */
    char alpn[256];                    /* comma-separated, server preference */
    char err[160];
    /* routes: a ClientHello whose SNI equals route_sni[i] ("" = any) and that
     * offers ALPN route_alpn[i] ("" = any) is served with routes[i] instead
     * (e.g. an RFC 8737 "acme-tls/1" challenge certificate). Not owned. */
    void* routes[16]; char route_sni[16][256]; char route_alpn[16][64]; int nroutes;
} tls_srv_cfg;

void trc_tls_server_config_free(void* h) {
    tls_srv_cfg* g = (tls_srv_cfg*)h; int i;
    if (!g) return;
    for (i = 0; i < g->ncerts; i++) free(g->certs[i]);
    if (g->key) trc_pk_free(g->key);
    tw(g, sizeof *g); free(g);
}

/* Certificate chain (DER, leaf first, concatenated with lengths) + private key
 * (DER: PKCS#8, PKCS#1 RSA or SEC1 EC). Always returns a handle; on failure
 * trc_tls_server_config_error() is non-empty. */
void* trc_tls_server_config(const char* ders, const int64_t* lens, int64_t count, const char* key_der, int64_t key_len, const char* alpn) {
    tls_srv_cfg* g = (tls_srv_cfg*)calloc(1, sizeof(tls_srv_cfg)); int64_t i, off = 0, err = 0;
    if (!g) return NULL;
    if (count <= 0) { snprintf(g->err, sizeof g->err, "TLS server: no certificate"); return g; }
    if (count > TLS_MAX_CERTS) count = TLS_MAX_CERTS;
    for (i = 0; i < count; i++) {
        g->certs[i] = (u8*)malloc(lens[i] ? (size_t)lens[i] : 1);
        if (!g->certs[i]) { snprintf(g->err, sizeof g->err, "TLS server: out of memory"); return g; }
        memcpy(g->certs[i], ders + off, (size_t)lens[i]); g->cert_lens[i] = lens[i]; off += lens[i];
        g->ncerts++;
    }
    g->key = trc_pk_parse_der(key_der, key_len, &err);
    if (!g->key || !trc_pk_has_private(g->key)) { snprintf(g->err, sizeof g->err, "TLS server: cannot use the private key (need RSA or EC P-256/P-384)"); return g; }
    g->key_kind = (int)trc_pk_kind(g->key);
    if (g->key_kind != 1 && g->key_kind != 2 && g->key_kind != 3) { snprintf(g->err, sizeof g->err, "TLS server: unsupported key type (need RSA or EC P-256/P-384)"); return g; }
    if (alpn) { size_t al = strlen(alpn); if (al >= sizeof g->alpn) al = sizeof g->alpn - 1; memcpy(g->alpn, alpn, al); g->alpn[al] = 0; }
    return g;
}
/* Add a route (see tls_srv_cfg); returns 0, or -1 when the table is full. */
int64_t trc_tls_server_config_route(void* h, const char* sni, int64_t slen, const char* alpn, int64_t alen, void* alt) {
    tls_srv_cfg* g = (tls_srv_cfg*)h; int i;
    if (!g || !alt || slen < 0 || slen > 255 || alen < 0 || alen > 63) return -1;
    for (i = 0; i < g->nroutes; i++)   /* same key: replace */
        if (strlen(g->route_sni[i]) == (size_t)slen && memcmp(g->route_sni[i], sni, (size_t)slen) == 0 &&
            strlen(g->route_alpn[i]) == (size_t)alen && memcmp(g->route_alpn[i], alpn, (size_t)alen) == 0) { g->routes[i] = alt; return 0; }
    if (g->nroutes >= 16) return -1;
    i = g->nroutes++;
    memcpy(g->route_sni[i], sni, (size_t)slen); g->route_sni[i][slen] = 0;
    memcpy(g->route_alpn[i], alpn, (size_t)alen); g->route_alpn[i][alen] = 0;
    g->routes[i] = alt;
    return 0;
}
void trc_tls_server_config_clear_routes(void* h) { tls_srv_cfg* g = (tls_srv_cfg*)h; if (g) g->nroutes = 0; }
int64_t trc_tls_server_config_error(void* h, char* out, int64_t cap) {
    tls_srv_cfg* g = (tls_srv_cfg*)h; size_t n;
    if (!g) { memcpy(out, "TLS server: out of memory", 25); return 25; }
    n = strlen(g->err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, g->err, n); return (int64_t)n;
}

typedef struct {
    const u8* sid; size_t sid_len;
    int suites[128]; int nsuites;              /* Go clients list TLS 1.3 suites after ~16 others */
    int groups[64]; int ngroups;
    int sigalgs[64]; int nsigalgs;
    int share_group[8]; const u8* share[8]; size_t share_len[8]; int nshares;
    int tls13;
    const u8* alpn; size_t alpn_len;            /* ProtocolNameList body */
    char sni[256];
} ch_info;

static int parse_client_hello(tls_conn* c, const u8* m, size_t n, ch_info* h) {
    size_t k = 0, l, end;
    memset(h, 0, sizeof *h);
    if (n < 2 + 32 + 1) goto bad;
    k = 2 + 32;
    h->sid_len = m[k++]; if (h->sid_len > 32 || k + h->sid_len > n) goto bad;
    h->sid = m + k; k += h->sid_len;
    if (k + 2 > n) goto bad;
    l = ((size_t)m[k] << 8) | m[k + 1]; k += 2;
    if (k + l > n || (l & 1)) goto bad;
    { size_t i; for (i = 0; i < l; i += 2) if (h->nsuites < 128) h->suites[h->nsuites++] = (m[k + i] << 8) | m[k + i + 1]; }
    k += l;
    if (k + 1 > n) goto bad;
    l = m[k++]; if (k + l > n) goto bad; k += l;           /* compression methods */
    if (k == n) return 0;
    if (k + 2 > n) goto bad;
    l = ((size_t)m[k] << 8) | m[k + 1]; k += 2;
    if (k + l != n) goto bad;
    end = k + l;
    while (k + 4 <= end) {
        unsigned et = ((unsigned)m[k] << 8) | m[k + 1]; size_t el = ((size_t)m[k + 2] << 8) | m[k + 3];
        const u8* d = m + k + 4;
        k += 4;
        if (k + el > end) goto bad;
        if (et == 43) {                                     /* supported_versions */
            size_t vl = el ? d[0] : 0, i;
            if (1 + vl > el) goto bad;
            for (i = 0; i + 1 < vl; i += 2) if (d[1 + i] == 3 && d[2 + i] == 4) h->tls13 = 1;
        } else if (et == 10) {                              /* supported_groups */
            size_t gl, i; if (el < 2) goto bad; gl = ((size_t)d[0] << 8) | d[1];
            if (2 + gl > el) goto bad;
            for (i = 0; i + 1 < gl; i += 2) if (h->ngroups < 64) h->groups[h->ngroups++] = (d[2 + i] << 8) | d[3 + i];
        } else if (et == 13) {                              /* signature_algorithms */
            size_t sl, i; if (el < 2) goto bad; sl = ((size_t)d[0] << 8) | d[1];
            if (2 + sl > el) goto bad;
            for (i = 0; i + 1 < sl; i += 2) if (h->nsigalgs < 64) h->sigalgs[h->nsigalgs++] = (d[2 + i] << 8) | d[3 + i];
        } else if (et == 51) {                              /* key_share */
            size_t kl, p = 2; if (el < 2) goto bad; kl = ((size_t)d[0] << 8) | d[1];
            if (2 + kl > el) goto bad;
            while (p + 4 <= 2 + kl) {
                int g = (d[p] << 8) | d[p + 1]; size_t xl = ((size_t)d[p + 2] << 8) | d[p + 3];
                if (p + 4 + xl > 2 + kl) goto bad;
                if (h->nshares < 8) { h->share_group[h->nshares] = g; h->share[h->nshares] = d + p + 4; h->share_len[h->nshares] = xl; h->nshares++; }
                p += 4 + xl;
            }
        } else if (et == 16) {                              /* ALPN */
            size_t pl; if (el < 2) goto bad; pl = ((size_t)d[0] << 8) | d[1];
            if (2 + pl > el) goto bad;
            h->alpn = d + 2; h->alpn_len = pl;
        } else if (et == 0) {                               /* server_name */
            if (el >= 5 && d[2] == 0) {
                size_t nl = ((size_t)d[3] << 8) | d[4];
                if (5 + nl <= el && nl < sizeof h->sni) { memcpy(h->sni, d + 5, nl); h->sni[nl] = 0; }
            }
        }
        k += el;
    }
    return 0;
bad:
    send_alert(c, 2, 50);
    tls_fail(c, "TLS: malformed ClientHello");
    return -1;
}

static int ch_has(const int* v, int n, int x) { int i; for (i = 0; i < n; i++) if (v[i] == x) return 1; return 0; }

/* Server-preferred ALPN protocol also offered by the client; "" when none. */
static void pick_alpn(const tls_srv_cfg* g, const ch_info* h, char* out) {
    size_t pos = 0, al = strlen(g->alpn);
    out[0] = 0;
    if (!h->alpn) return;
    while (pos < al) {
        size_t e = pos, q = 0;
        while (e < al && g->alpn[e] != ',') e++;
        while (q < h->alpn_len) {
            size_t pl = h->alpn[q];
            if (q + 1 + pl > h->alpn_len) break;
            if (pl == e - pos && memcmp(h->alpn + q + 1, g->alpn + pos, pl) == 0) { memcpy(out, g->alpn + pos, pl); out[pl] = 0; return; }
            q += 1 + pl;
        }
        pos = e + 1;
    }
}

static int ieq_ascii(const char* a, const char* b, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) { int x = (unsigned char)a[i], y = (unsigned char)b[i]; if (x >= 'A' && x <= 'Z') x += 32; if (y >= 'A' && y <= 'Z') y += 32; if (x != y) return 0; }
    return 1;
}
static int ch_offers_alpn(const ch_info* h, const char* p) {
    size_t q = 0, n = strlen(p);
    if (!h->alpn) return 0;
    while (q < h->alpn_len) {
        size_t pl = h->alpn[q];
        if (q + 1 + pl > h->alpn_len) break;
        if (pl == n && memcmp(h->alpn + q + 1, p, n) == 0) return 1;
        q += 1 + pl;
    }
    return 0;
}
/* The configuration a ClientHello is routed to (first matching route, else g). */
static tls_srv_cfg* route_cfg(tls_srv_cfg* g, const ch_info* h) {
    int i;
    for (i = 0; i < g->nroutes; i++) {
        const char* rs = g->route_sni[i]; const char* ra = g->route_alpn[i];
        if (rs[0]) { size_t a = strlen(rs), b = strlen(h->sni); if (a != b || ieq_ascii(rs, h->sni, a) == 0) continue; }
        if (ra[0] && !ch_offers_alpn(h, ra)) continue;
        return (tls_srv_cfg*)g->routes[i];
    }
    return g;
}

static int server_handshake(tls_conn* c, tls_srv_cfg* g) {
    u8 *msg = NULL; size_t mlen; int type, rc = -1, suite = 0, hash = 256, hl = 32, group = 0, scheme = 0, hrr = 0;
    ch_info ch; u8 random[32], shared[48], x_priv[32], x_pub[32], base[32] = {9}, pub[97]; size_t publen = 0, slen = 0;
    void* eck = NULL; const u8* peer = NULL; size_t peer_len = 0; u8 sid_copy[32]; size_t sid_len = 0;
    u8 early[48], derived[48], hs_secret[48], c_hs[48], s_hs[48], master[48], c_ap[48], s_ap[48], zeros[48], th[48];
    char alpn_sel[128];
    memset(zeros, 0, sizeof zeros);
    if (!g || g->err[0]) { tls_fail(c, g ? g->err : "TLS server: no configuration"); return -1; }

    for (;;) {
        int i;
        if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
        if (type != 1) { send_alert(c, 2, 10); tls_fail(c, "TLS: expected ClientHello"); goto out; }
        if (parse_client_hello(c, msg, mlen, &ch) < 0) goto out;
        if (!ch.tls13) { send_alert(c, 2, 70); tls_fail(c, "TLS: client does not offer TLS 1.3"); goto out; }
        if (!hrr && g->nroutes) {
            g = route_cfg(g, &ch);
            if (g->err[0]) { send_alert(c, 2, 80); tls_fail(c, g->err); goto out; }
        }
        if (hrr && (ch.sid_len != sid_len || memcmp(ch.sid, sid_copy, sid_len) != 0)) { send_alert(c, 2, 47); tls_fail(c, "TLS: ClientHello changed after HelloRetryRequest"); goto out; }
        sid_len = ch.sid_len; memcpy(sid_copy, ch.sid, sid_len);
        if (!suite) {
            static const int pref[3] = {0x1301, 0x1303, 0x1302};
            for (i = 0; i < 3 && !suite; i++) if (ch_has(ch.suites, ch.nsuites, pref[i])) suite = pref[i];
            if (!suite) { send_alert(c, 2, 40); tls_fail(c, "TLS: no common cipher suite"); goto out; }
        }
        /* signature scheme for our key */
        if (!scheme) {
            if (g->key_kind == 2 && ch_has(ch.sigalgs, ch.nsigalgs, 0x0403)) scheme = 0x0403;
            else if (g->key_kind == 3 && ch_has(ch.sigalgs, ch.nsigalgs, 0x0503)) scheme = 0x0503;
            else if (g->key_kind == 1) {
                if (ch_has(ch.sigalgs, ch.nsigalgs, 0x0804)) scheme = 0x0804;
                else if (ch_has(ch.sigalgs, ch.nsigalgs, 0x0805)) scheme = 0x0805;
                else if (ch_has(ch.sigalgs, ch.nsigalgs, 0x0806)) scheme = 0x0806;
            }
            if (!scheme) { send_alert(c, 2, 40); tls_fail(c, "TLS: client accepts no signature scheme for our certificate key"); goto out; }
        }
        /* key share: server preference X25519, P-256, P-384 */
        {
            static const int gp[3] = {0x1d, 0x17, 0x18};
            int j;
            group = 0; peer = NULL;
            for (i = 0; i < 3 && !group; i++)
                for (j = 0; j < ch.nshares; j++)
                    if (ch.share_group[j] == gp[i] && (!hrr || gp[i] == hrr)) { group = gp[i]; peer = ch.share[j]; peer_len = ch.share_len[j]; break; }
            if (!group) {
                int want = 0;
                if (hrr) { send_alert(c, 2, 47); tls_fail(c, "TLS: no key share for the requested group after HelloRetryRequest"); goto out; }
                for (i = 0; i < 3 && !want; i++) if (ch_has(ch.groups, ch.ngroups, gp[i])) want = gp[i];
                if (!want) { send_alert(c, 2, 40); tls_fail(c, "TLS: no common key exchange group"); goto out; }
                /* HelloRetryRequest: transcript becomes message_hash(CH1) || HRR */
                hash = suite == 0x1302 ? 384 : 256; hl = hash == 384 ? 48 : 32;
                {
                    u8 h1[48], mh[4 + 48], body[128]; size_t bk = 0;
                    trc_digest(hash, (const char*)c->transcript, (int64_t)c->tlen, (char*)h1);
                    mh[0] = 254; mh[1] = 0; mh[2] = 0; mh[3] = (u8)hl; memcpy(mh + 4, h1, (size_t)hl);
                    c->tlen = 0;
                    buf_append(&c->transcript, &c->tlen, &c->tcap, mh, (size_t)(4 + hl));
                    bk = put16(body, bk, 0x0303);
                    memcpy(body + bk, HRR_RANDOM, 32); bk += 32;
                    body[bk++] = (u8)sid_len; memcpy(body + bk, sid_copy, sid_len); bk += sid_len;
                    bk = put16(body, bk, (unsigned)suite); body[bk++] = 0;
                    bk = put16(body, bk, 12);
                    bk = put16(body, bk, 43); bk = put16(body, bk, 2); bk = put16(body, bk, 0x0304);
                    bk = put16(body, bk, 51); bk = put16(body, bk, 2); bk = put16(body, bk, (unsigned)want);
                    if (send_hs(c, 2, body, bk) < 0) goto out;
                    if (sid_len) send_record(c, 20, (const u8*)"\x01", 1);
                }
                hrr = want;
                free(msg); msg = NULL;
                continue;
            }
        }
        break;
    }
    hash = suite == 0x1302 ? 384 : 256; hl = hash == 384 ? 48 : 32;
    pick_alpn(g, &ch, alpn_sel);
    memcpy(c->host, ch.sni, sizeof c->host);
    /* `peer` (and the other ch_info pointers) point into msg: keep it until
     * the key exchange below is done. */

    /* our key share + shared secret */
    if (trc_random((char*)random, 32) != 0) { tls_fail(c, "TLS: OS random generator unavailable"); goto out; }
    if (group == 0x1d) {
        if (peer_len != 32 || trc_random((char*)x_priv, 32) != 0) { send_alert(c, 2, 47); tls_fail(c, "TLS: bad X25519 key share"); goto out; }
        trc_x25519((char*)x_pub, (const char*)x_priv, (const char*)base);
        if (trc_x25519((char*)shared, (const char*)x_priv, (const char*)peer) != 0) { send_alert(c, 2, 47); tls_fail(c, "TLS: bad X25519 key share"); goto out; }
        memcpy(pub, x_pub, 32); publen = 32; slen = 32;
    } else {
        int bits = group == 0x17 ? 256 : 384, want = group == 0x17 ? 65 : 97;
        eck = trc_pk_ec_generate(bits);
        if (!eck || trc_pk_public_raw(eck, (char*)pub, 97) != want) { tls_fail(c, "TLS: key generation failed"); goto out; }
        publen = (size_t)want;
        if (trc_pk_ecdh(eck, (const char*)peer, (int64_t)peer_len, (char*)shared) != (bits / 8)) { send_alert(c, 2, 47); tls_fail(c, "TLS: bad ECDHE key share"); goto out; }
        slen = (size_t)(bits / 8);
    }
    free(msg); msg = NULL; peer = NULL;
    /* ServerHello */
    {
        u8 body[256]; size_t bk = 0, ext;
        bk = put16(body, bk, 0x0303);
        memcpy(body + bk, random, 32); bk += 32;
        body[bk++] = (u8)sid_len; memcpy(body + bk, sid_copy, sid_len); bk += sid_len;
        bk = put16(body, bk, (unsigned)suite); body[bk++] = 0;
        ext = bk; bk += 2;
        bk = put16(body, bk, 43); bk = put16(body, bk, 2); bk = put16(body, bk, 0x0304);
        bk = put16(body, bk, 51); bk = put16(body, bk, (unsigned)(4 + publen)); bk = put16(body, bk, (unsigned)group);
        bk = put16(body, bk, (unsigned)publen); memcpy(body + bk, pub, publen); bk += publen;
        put16(body, ext, (unsigned)(bk - ext - 2));
        if (send_hs(c, 2, body, bk) < 0) goto out;
    }
    if (sid_len && !hrr) send_record(c, 20, (const u8*)"\x01", 1);   /* middlebox compatibility */
    /* handshake secrets */
    trc_hkdf_extract(hash, (const char*)zeros, hl, (const char*)zeros, hl, (char*)early);
    derive_secret(c, hash, early, "derived", 0, derived);
    trc_hkdf_extract(hash, (const char*)derived, hl, (const char*)shared, (int64_t)slen, (char*)hs_secret);
    derive_secret(c, hash, hs_secret, "c hs traffic", 1, c_hs);
    derive_secret(c, hash, hs_secret, "s hs traffic", 1, s_hs);
    if (dir_set(&c->wr, suite, s_hs) < 0 || dir_set(&c->rd, suite, c_hs) < 0) goto out;
    /* EncryptedExtensions */
    {
        u8 body[160]; size_t bk = 2, al = strlen(alpn_sel);
        if (al) {
            bk = put16(body, bk, 16); bk = put16(body, bk, (unsigned)(al + 3)); bk = put16(body, bk, (unsigned)(al + 1));
            body[bk++] = (u8)al; memcpy(body + bk, alpn_sel, al); bk += al;
            memcpy(c->alpn, alpn_sel, al + 1);
        }
        if (ch.sni[0]) { bk = put16(body, bk, 0); bk = put16(body, bk, 0); }   /* server_name acknowledged */
        put16(body, 0, (unsigned)(bk - 2));
        if (send_hs(c, 8, body, bk) < 0) goto out;
    }
    /* Certificate */
    {
        size_t tot = 1 + 3, bk = 0, i2; u8* body;
        for (i2 = 0; i2 < (size_t)g->ncerts; i2++) tot += 3 + (size_t)g->cert_lens[i2] + 2;
        body = (u8*)malloc(tot);
        if (!body) goto out;
        body[bk++] = 0;
        body[bk++] = (u8)((tot - 4) >> 16); body[bk++] = (u8)((tot - 4) >> 8); body[bk++] = (u8)(tot - 4);
        for (i2 = 0; i2 < (size_t)g->ncerts; i2++) {
            size_t cl = (size_t)g->cert_lens[i2];
            body[bk++] = (u8)(cl >> 16); body[bk++] = (u8)(cl >> 8); body[bk++] = (u8)cl;
            memcpy(body + bk, g->certs[i2], cl); bk += cl;
            body[bk++] = 0; body[bk++] = 0;
        }
        if (send_hs(c, 11, body, bk) < 0) { free(body); goto out; }
        free(body);
    }
    /* CertificateVerify */
    {
        u8 content[64 + 34 + 48], sig[1024], body[4 + 1024]; static const char ctx[] = "TLS 1.3, server CertificateVerify";
        int64_t sl; int salg = scheme == 0x0503 || scheme == 0x0805 ? 384 : scheme == 0x0806 ? 512 : 256;
        transcript_hash(c, hash, th);
        memset(content, 0x20, 64); memcpy(content + 64, ctx, 33); content[97] = 0; memcpy(content + 98, th, (size_t)hl);
        sl = trc_pk_sign(g->key, g->key_kind == 1 ? 1 : 0, salg, (const char*)content, 98 + hl, (char*)sig, (int64_t)sizeof sig, 0);
        if (sl <= 0) { send_alert(c, 2, 80); tls_fail(c, "TLS: signing CertificateVerify failed"); goto out; }
        put16(body, 0, (unsigned)scheme); put16(body, 2, (unsigned)sl); memcpy(body + 4, sig, (size_t)sl);
        if (send_hs(c, 15, body, 4 + (size_t)sl) < 0) goto out;
    }
    /* server Finished */
    {
        u8 fk[48], vd[48];
        transcript_hash(c, hash, th);
        hkdf_label(hash, s_hs, hl, "finished", NULL, 0, fk, hl);
        trc_hmac(hash, (const char*)fk, hl, (const char*)th, hl, (char*)vd);
        if (send_hs(c, 20, vd, (size_t)hl) < 0) goto out;
    }
    /* application secrets (transcript through server Finished) */
    derive_secret(c, hash, hs_secret, "derived", 0, derived);
    trc_hkdf_extract(hash, (const char*)derived, hl, (const char*)zeros, hl, (char*)master);
    derive_secret(c, hash, master, "c ap traffic", 1, c_ap);
    derive_secret(c, hash, master, "s ap traffic", 1, s_ap);
    /* client Finished */
    {
        u8 fk[48], want[48];
        transcript_hash(c, hash, th);
        hkdf_label(hash, c_hs, hl, "finished", NULL, 0, fk, hl);
        trc_hmac(hash, (const char*)fk, hl, (const char*)th, hl, (char*)want);
        if (next_hs(c, &type, &msg, &mlen) < 0) goto out;
        if (type != 20 || mlen != (size_t)hl || !trc_ct_eq((const char*)msg, hl, (const char*)want, hl)) {
            send_alert(c, 2, 51); tls_fail(c, "TLS: client Finished does not verify"); goto out;
        }
        free(msg); msg = NULL;
    }
    if (dir_set(&c->wr, suite, s_ap) < 0 || dir_set(&c->rd, suite, c_ap) < 0) goto out;
    c->state = 1;
    rc = 0;
out:
    free(msg);
    if (eck) trc_pk_free(eck);
    tw(x_priv, sizeof x_priv); tw(shared, sizeof shared); tw(early, sizeof early); tw(hs_secret, sizeof hs_secret);
    tw(master, sizeof master); tw(c_hs, sizeof c_hs); tw(s_hs, sizeof s_hs); tw(c_ap, sizeof c_ap); tw(s_ap, sizeof s_ap);
    free(c->transcript); c->transcript = NULL; c->tlen = c->tcap = 0;
    return rc;
}

/* Accept a TLS 1.3 handshake on a connected socket. Always returns a handle
 * (check trc_tls_ok / trc_tls_error); the requested SNI host name is
 * available through trc_tls_server_name. */
void* trc_tls_server_fd(int64_t fd, void* cfg, int64_t timeout_ms, int64_t owns_fd) {
    tls_conn* c = conn_new("", 0, NULL, 0, timeout_ms);
    if (!c) return NULL;
    c->fd = (tls_sock)fd; c->owns_fd = (int)owns_fd; c->is_server = 1;
#if defined(_WIN32)
    tls_wsa();
#endif
    server_handshake(c, (tls_srv_cfg*)cfg);
    return c;
}
int64_t trc_tls_server_name(void* h, char* out, int64_t cap) {
    tls_conn* c = (tls_conn*)h; size_t n;
    if (!c) return 0;
    n = strlen(c->host); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, c->host, n); return (int64_t)n;
}

/* ── TLS termination proxy (HTTPS for servers written against plain sockets) ──
 * trc_tls_server_proxy(fd, cfg, timeout) returns the plain end of a local
 * socket pair at once; a detached thread runs the server handshake on `fd`
 * and then copies bytes both ways until either side closes. A failed
 * handshake closes the plain end, which the application sees as EOF. */
#if defined(_WIN32)
#  include <windows.h>
#else
#  include <pthread.h>
#endif

static int tp_socketpair(tls_sock sv[2]) {
#if defined(_WIN32)
    struct sockaddr_in a; int al = sizeof a; tls_sock l, c, s;
    tls_wsa();
    l = socket(AF_INET, SOCK_STREAM, 0);
    if (l == TLS_BADSOCK) return -1;
    memset(&a, 0, sizeof a); a.sin_family = AF_INET; a.sin_addr.s_addr = htonl(INADDR_LOOPBACK); a.sin_port = 0;
    if (bind(l, (struct sockaddr*)&a, sizeof a) != 0 || listen(l, 1) != 0 || getsockname(l, (struct sockaddr*)&a, &al) != 0) { closesocket(l); return -1; }
    c = socket(AF_INET, SOCK_STREAM, 0);
    if (c == TLS_BADSOCK || connect(c, (struct sockaddr*)&a, sizeof a) != 0) { closesocket(l); if (c != TLS_BADSOCK) closesocket(c); return -1; }
    s = accept(l, NULL, NULL);
    closesocket(l);
    if (s == TLS_BADSOCK) { closesocket(c); return -1; }
    sv[0] = c; sv[1] = s;
    return 0;
#else
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) != 0) return -1;
    sv[0] = fds[0]; sv[1] = fds[1];
    return 0;
#endif
}

typedef struct { tls_sock net; tls_sock plain; tls_srv_cfg* cfg; int timeout_ms; } tp_job;

static int tp_send_all(tls_sock s, const char* d, size_t n) {
    size_t off = 0;
    while (off < n) {
#if defined(MSG_NOSIGNAL)
        long r = (long)send(s, d + off, (int)(n - off), MSG_NOSIGNAL);
#else
        long r = (long)send(s, d + off, (int)(n - off), 0);
#endif
        if (r <= 0) {
#if !defined(_WIN32)
            if (r < 0 && errno == EINTR) continue;
#endif
            return -1;
        }
        off += (size_t)r;
    }
    return 0;
}

static void tp_run(tp_job* j) {
    tls_conn* c = (tls_conn*)trc_tls_server_fd((int64_t)j->net, j->cfg, j->timeout_ms, 1);
    char* buf = (char*)malloc(TLS_MAX_REC);
    if (c && buf && trc_tls_ok(c)) {
        c->timeout_ms = 0;                     /* block in reads once data is announced */
        for (;;) {
            int net_ready = 0, plain_ready = 0;
            if (c->app_len > 0) net_ready = 1;
            else {
#if defined(_WIN32)
                fd_set rs; FD_ZERO(&rs); FD_SET(c->fd, &rs); FD_SET(j->plain, &rs);
                if (select(0, &rs, NULL, NULL, NULL) <= 0) break;
                net_ready = FD_ISSET(c->fd, &rs); plain_ready = FD_ISSET(j->plain, &rs);
#else
                struct pollfd p[2]; int r;
                p[0].fd = c->fd; p[0].events = POLLIN; p[0].revents = 0;
                p[1].fd = j->plain; p[1].events = POLLIN; p[1].revents = 0;
                do { r = poll(p, 2, -1); } while (r < 0 && errno == EINTR);
                if (r <= 0) break;
                net_ready = (p[0].revents & (POLLIN | POLLHUP | POLLERR)) != 0;
                plain_ready = (p[1].revents & (POLLIN | POLLHUP | POLLERR)) != 0;
#endif
            }
            if (net_ready) {
                int64_t n = trc_tls_read(c, buf, TLS_MAX_REC);
                if (n <= 0) break;
                if (tp_send_all(j->plain, buf, (size_t)n) < 0) break;
            }
            if (plain_ready) {
                long n = (long)recv(j->plain, buf, TLS_MAX_REC, 0);
                if (n <= 0) break;
                if (trc_tls_write(c, buf, n) < 0) break;
            }
        }
    }
    free(buf);
    tls_closesock(j->plain);
    if (c) trc_tls_close(c);                   /* close_notify + closes the network socket */
    else tls_closesock(j->net);
    free(j);
}

#if defined(_WIN32)
static DWORD WINAPI tp_thread(LPVOID p) { tp_run((tp_job*)p); return 0; }
#else
static void* tp_thread(void* p) { tp_run((tp_job*)p); return NULL; }
#endif

int64_t trc_tls_server_proxy(int64_t fd, void* cfg, int64_t timeout_ms) {
    tls_sock sv[2]; tp_job* j;
    if (!cfg || ((tls_srv_cfg*)cfg)->err[0]) return -1;
    if (tp_socketpair(sv) != 0) return -1;
    j = (tp_job*)malloc(sizeof *j);
    if (!j) { tls_closesock(sv[0]); tls_closesock(sv[1]); return -1; }
    j->net = (tls_sock)fd; j->plain = sv[1]; j->cfg = (tls_srv_cfg*)cfg; j->timeout_ms = (int)timeout_ms;
#if defined(_WIN32)
    {
        HANDLE t = CreateThread(NULL, 0, tp_thread, j, 0, NULL);
        if (!t) { tls_closesock(sv[0]); tls_closesock(sv[1]); free(j); return -1; }
        CloseHandle(t);
    }
#else
    {
        pthread_t t; pthread_attr_t at;
        pthread_attr_init(&at); pthread_attr_setdetachstate(&at, PTHREAD_CREATE_DETACHED);
        if (pthread_create(&t, &at, tp_thread, j) != 0) { pthread_attr_destroy(&at); tls_closesock(sv[0]); tls_closesock(sv[1]); free(j); return -1; }
        pthread_attr_destroy(&at);
    }
#endif
    return (int64_t)sv[0];
}
