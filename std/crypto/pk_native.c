/*
 * std/crypto/pk_native.c -- public-key crypto and X.509 for std.crypto.
 *
 * Linked through the `# tauraro-cpp-shim: pk_native.c` pragma in
 * std/crypto/pk.tr. Depends on crypto_native.c (hashes, HMAC, CSPRNG),
 * which std/crypto/pk.tr also pulls in.
 *
 *   bignum ........ fixed-capacity little-endian u32 limbs, Montgomery math
 *   RSA ........... PKCS#1 v1.5 and PSS sign/verify, OAEP-free (no encryption)
 *   EC ............ NIST P-256 and P-384: ECDSA (RFC 6979 nonces), ECDH, keygen
 *   DER/PEM ....... minimal readers; SPKI, PKCS#1, PKCS#8, SEC1 key parsing
 *   X.509 ......... certificate parsing, signature checks, chain building,
 *                   validity, key usage / EKU / basic constraints / name
 *                   constraints (dNSName), RFC 6125 hostname matching
 *
 * All functions use caller-provided output buffers. Secret-key operations
 * use constant-time table scans and conditional swaps, but this code has not
 * been audited for every side channel; treat it accordingly.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

/* from crypto_native.c */
int64_t trc_digest(int64_t alg, const char* data, int64_t n, char* out);
int64_t trc_hmac(int64_t alg, const char* key, int64_t klen, const char* msg, int64_t mlen, char* out);
int64_t trc_random(char* out, int64_t n);
int64_t trc_b64_decode(int64_t kind, const char* s, int64_t n, char* out);
int64_t trc_ct_eq(const char* a, int64_t alen, const char* b, int64_t blen);

static int md_len(int alg) { return alg == 1 ? 20 : alg == 256 ? 32 : alg == 384 ? 48 : alg == 512 ? 64 : -1; }
static void pk_wipe(void* p, size_t n) { volatile u8* v = (volatile u8*)p; while (n--) *v++ = 0; }

/* ═══════════════════════════ bignum ══════════════════════════════════════ */
#define MAXL 132   /* 4224 bits: RSA up to 4096 plus headroom */

typedef struct {
    int n;               /* limbs */
    u32 m[MAXL];         /* modulus (odd) */
    u32 minv;            /* -m^-1 mod 2^32 */
    u32 rr[MAXL];        /* R^2 mod m */
    u32 one[MAXL];       /* R mod m (Montgomery 1) */
} mont;

static void bn_from_be(u32* d, int n, const u8* b, size_t blen) {
    int i;
    for (i = 0; i < n; i++) d[i] = 0;
    for (i = 0; (size_t)i < blen; i++) {
        size_t pos = blen - 1 - (size_t)i;   /* byte significance i */
        if (i / 4 < n) d[i / 4] |= (u32)b[pos] << (8 * (i % 4));
    }
}
static void bn_to_be(u8* b, size_t blen, const u32* d, int n) {
    size_t i;
    for (i = 0; i < blen; i++) {
        size_t sig = blen - 1 - i;
        b[i] = (sig / 4 < (size_t)n) ? (u8)(d[sig / 4] >> (8 * (sig % 4))) : 0;
    }
}
static int bn_cmp(const u32* a, const u32* b, int n) {
    int i;
    for (i = n - 1; i >= 0; i--) { if (a[i] > b[i]) return 1; if (a[i] < b[i]) return -1; }
    return 0;
}
static int bn_is_zero(const u32* a, int n) { u32 acc = 0; int i; for (i = 0; i < n; i++) acc |= a[i]; return acc == 0; }
static u32 bn_add(u32* r, const u32* a, const u32* b, int n) {
    u64 c = 0; int i;
    for (i = 0; i < n; i++) { c += (u64)a[i] + b[i]; r[i] = (u32)c; c >>= 32; }
    return (u32)c;
}
static u32 bn_sub(u32* r, const u32* a, const u32* b, int n) {
    int64_t c = 0; int i;
    for (i = 0; i < n; i++) { c += (int64_t)a[i] - b[i]; r[i] = (u32)c; c >>= 32; }
    return (u32)(c & 1);   /* borrow */
}
/* r = a if c else r, constant time */
static void bn_cmov(u32* r, const u32* a, int n, u32 c) {
    u32 mask = (u32)0 - (c & 1); int i;
    for (i = 0; i < n; i++) r[i] = (r[i] & ~mask) | (a[i] & mask);
}
static int bn_bits(const u32* a, int n) {
    int i, b;
    for (i = n - 1; i >= 0; i--) if (a[i]) { for (b = 31; b >= 0; b--) if (a[i] >> b & 1) return i * 32 + b + 1; }
    return 0;
}
static int bn_bit(const u32* a, int i) { return (int)((a[i / 32] >> (i % 32)) & 1); }

static void mont_mul(const mont* M, u32* r, const u32* a, const u32* b) {
    u32 t[MAXL + 2], s[MAXL]; int i, j, n = M->n; u64 c; u32 borrow;
    memset(t, 0, sizeof(u32) * (size_t)(n + 2));
    for (i = 0; i < n; i++) {
        u32 q;
        c = 0;
        for (j = 0; j < n; j++) { c += (u64)t[j] + (u64)a[j] * b[i]; t[j] = (u32)c; c >>= 32; }
        c += t[n]; t[n] = (u32)c; t[n + 1] = (u32)(c >> 32);
        q = t[0] * M->minv;
        c = (u64)t[0] + (u64)q * M->m[0]; c >>= 32;
        for (j = 1; j < n; j++) { c += (u64)t[j] + (u64)q * M->m[j]; t[j - 1] = (u32)c; c >>= 32; }
        c += t[n]; t[n - 1] = (u32)c; c >>= 32;
        t[n] = t[n + 1] + (u32)c;
    }
    borrow = bn_sub(s, t, M->m, n);
    /* keep s when t >= m, i.e. t[n] set or no borrow */
    memcpy(r, t, sizeof(u32) * (size_t)n);
    bn_cmov(r, s, n, (t[n] != 0) | (borrow == 0));
}
/* 2a mod m for a < m */
static void mod_dbl(const u32* m, int n, u32* a) {
    u32 s[MAXL]; u32 carry = 0, borrow; int i;
    for (i = 0; i < n; i++) { u32 nc = a[i] >> 31; a[i] = (a[i] << 1) | carry; carry = nc; }
    borrow = bn_sub(s, a, m, n);
    bn_cmov(a, s, n, carry | (borrow == 0));
}
static int mont_init(mont* M, const u32* m, int n) {
    u32 x; int i;
    if (n < 1 || n > MAXL || !(m[0] & 1)) return -1;
    M->n = n; memcpy(M->m, m, sizeof(u32) * (size_t)n);
    x = m[0];
    for (i = 0; i < 5; i++) x *= 2 - m[0] * x;
    M->minv = (u32)0 - x;
    /* R mod m: start at 1 and double 32n times */
    memset(M->one, 0, sizeof M->one); M->one[0] = 1;
    if (n == 1 && m[0] == 1) return -1;
    for (i = 0; i < 32 * n; i++) mod_dbl(M->m, n, M->one);
    memcpy(M->rr, M->one, sizeof(u32) * (size_t)n);
    for (i = 0; i < 32 * n; i++) mod_dbl(M->m, n, M->rr);
    return 0;
}
static void mont_to(const mont* M, u32* r, const u32* a) { mont_mul(M, r, a, M->rr); }
static void mont_from(const mont* M, u32* r, const u32* a) {
    u32 one[MAXL]; memset(one, 0, sizeof(u32) * (size_t)M->n); one[0] = 1; mont_mul(M, r, a, one);
}
/* r = x mod m for x of up to 2n limbs (x < m * R). */
static void mont_reduce_wide(const mont* M, u32* r, const u32* x, int xl) {
    u32 t[2 * MAXL + 1]; int i, j, n = M->n; u32 s[MAXL], borrow;
    memset(t, 0, sizeof t);
    memcpy(t, x, sizeof(u32) * (size_t)(xl < 2 * n ? xl : 2 * n));
    for (i = 0; i < n; i++) {
        u32 q = t[i] * M->minv; u64 c = 0;
        for (j = 0; j < n; j++) { c += (u64)t[i + j] + (u64)q * M->m[j]; t[i + j] = (u32)c; c >>= 32; }
        for (j = i + n; c && j <= 2 * n; j++) { c += t[j]; t[j] = (u32)c; c >>= 32; }
    }
    borrow = bn_sub(s, t + n, M->m, n);
    memcpy(r, t + n, sizeof(u32) * (size_t)n);
    bn_cmov(r, s, n, (t[2 * n] != 0) | (borrow == 0));
    /* r = x * R^-1; multiply by R^2 then reduce: x */
    mont_mul(M, r, r, M->rr);
}
/* r = a^e mod m (a, r in normal form, a < m). Fixed 4-bit window with a
 * constant-time table scan; exponent length leaks only through ebits. */
static void mont_exp(const mont* M, u32* r, const u32* a, const u32* e, int ebits) {
    u32 tbl[16][MAXL], acc[MAXL], sel[MAXL]; int n = M->n, i, w, k;
    memcpy(tbl[0], M->one, sizeof(u32) * (size_t)n);
    mont_to(M, tbl[1], a);
    for (i = 2; i < 16; i++) mont_mul(M, tbl[i], tbl[i - 1], tbl[1]);
    memcpy(acc, M->one, sizeof(u32) * (size_t)n);
    for (w = ((ebits + 3) / 4) - 1; w >= 0; w--) {
        int nib = 0;
        for (k = 0; k < 4; k++) mont_mul(M, acc, acc, acc);
        for (k = 3; k >= 0; k--) { int bi = 4 * w + k; nib = (nib << 1) | (bi < ebits ? bn_bit(e, bi) : 0); }
        memset(sel, 0, sizeof(u32) * (size_t)n);
        for (k = 0; k < 16; k++) bn_cmov(sel, tbl[k], n, (u32)(k == nib));
        mont_mul(M, acc, acc, sel);
    }
    mont_from(M, r, acc);
    pk_wipe(tbl, sizeof tbl);
}
/* schoolbook r[an+bn] = a*b */
static void bn_mul(u32* r, const u32* a, int an, const u32* b, int bn) {
    int i, j;
    memset(r, 0, sizeof(u32) * (size_t)(an + bn));
    for (i = 0; i < an; i++) {
        u64 c = 0;
        for (j = 0; j < bn; j++) { c += (u64)r[i + j] + (u64)a[i] * b[j]; r[i + j] = (u32)c; c >>= 32; }
        r[i + bn] = (u32)c;
    }
}

/* ═══════════════════════════ DER reader ══════════════════════════════════ */
typedef struct { const u8* p; size_t n; } der;

/* Read one TLV. Returns 0 on success; sets tag and value; advances d. */
static int der_next(der* d, int* tag, der* val) {
    size_t len; u8 b;
    if (d->n < 2) return -1;
    *tag = d->p[0];
    if ((*tag & 0x1f) == 0x1f) return -1;   /* high tag numbers unsupported */
    b = d->p[1];
    if (b < 0x80) { len = b; d->p += 2; d->n -= 2; }
    else {
        int k = b & 0x7f, i;
        if (k == 0 || k > 4 || d->n < (size_t)(2 + k)) return -1;
        len = 0;
        for (i = 0; i < k; i++) len = (len << 8) | d->p[2 + i];
        if (len < 0x80 || (k > 1 && d->p[2] == 0)) return -1;   /* not minimal DER */
        d->p += 2 + k; d->n -= 2 + (size_t)k;
    }
    if (len > d->n) return -1;
    val->p = d->p; val->n = len;
    d->p += len; d->n -= len;
    return 0;
}
static int der_expect(der* d, int want, der* val) { int t; if (der_next(d, &t, val) < 0 || t != want) return -1; return 0; }
/* Peek tag without consuming. */
static int der_peek(const der* d) { return d->n ? d->p[0] : -1; }
/* INTEGER -> magnitude bytes (strip one leading zero); rejects negatives. */
static int der_uint(der* d, der* mag) {
    if (der_expect(d, 0x02, mag) < 0 || mag->n == 0) return -1;
    if (mag->p[0] & 0x80) return -1;
    if (mag->n > 1 && mag->p[0] == 0) { mag->p++; mag->n--; }
    return 0;
}
static int oid_eq(const der* o, const u8* want, size_t wn) { return o->n == wn && memcmp(o->p, want, wn) == 0; }

static const u8 OID_RSA[] = {0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01};
static const u8 OID_RSA_PSS[] = {0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x0a};
static const u8 OID_MGF1[] = {0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x08};
static const u8 OID_SHA1_RSA[] = {0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x05};
static const u8 OID_SHA256_RSA[] = {0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x0b};
static const u8 OID_SHA384_RSA[] = {0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x0c};
static const u8 OID_SHA512_RSA[] = {0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x0d};
static const u8 OID_EC_PUB[] = {0x2a,0x86,0x48,0xce,0x3d,0x02,0x01};
static const u8 OID_P256[] = {0x2a,0x86,0x48,0xce,0x3d,0x03,0x01,0x07};
static const u8 OID_P384[] = {0x2b,0x81,0x04,0x00,0x22};
static const u8 OID_ECDSA_SHA1[] = {0x2a,0x86,0x48,0xce,0x3d,0x04,0x01};
static const u8 OID_ECDSA_SHA256[] = {0x2a,0x86,0x48,0xce,0x3d,0x04,0x03,0x02};
static const u8 OID_ECDSA_SHA384[] = {0x2a,0x86,0x48,0xce,0x3d,0x04,0x03,0x03};
static const u8 OID_ECDSA_SHA512[] = {0x2a,0x86,0x48,0xce,0x3d,0x04,0x03,0x04};
static const u8 OID_ED25519[] = {0x2b,0x65,0x70};
static const u8 OID_SHA1[] = {0x2b,0x0e,0x03,0x02,0x1a};
static const u8 OID_SHA256[] = {0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x01};
static const u8 OID_SHA384[] = {0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x02};
static const u8 OID_SHA512[] = {0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x03};

/* ═══════════════════════════ RSA ═════════════════════════════════════════ */
typedef struct {
    int bits, n_limbs;
    u32 n[MAXL];
    u32 e[MAXL]; int ebits;
    int has_priv;
    u32 p[MAXL], q[MAXL], dp[MAXL], dq[MAXL], qinv[MAXL];
    int half_limbs;
    u32 d[MAXL]; int has_d;     /* private exponent (kept for JWK export) */
} rsa_key;

static int rsa_set_public(rsa_key* k, const u8* n, size_t nlen, const u8* e, size_t elen) {
    while (nlen > 0 && n[0] == 0) { n++; nlen--; }
    while (elen > 0 && e[0] == 0) { e++; elen--; }
    if (nlen == 0 || nlen > 4 * (MAXL - 2) || elen == 0 || elen > nlen) return -1;
    memset(k, 0, sizeof *k);
    k->n_limbs = (int)((nlen + 3) / 4);
    bn_from_be(k->n, k->n_limbs, n, nlen);
    bn_from_be(k->e, k->n_limbs, e, elen);
    k->bits = bn_bits(k->n, k->n_limbs);
    k->ebits = bn_bits(k->e, k->n_limbs);
    if (k->bits < 512 || !(k->n[0] & 1)) return -1;
    return 0;
}
/* SubjectPublicKeyInfo's inner RSAPublicKey: SEQ { n, e } */
static int rsa_parse_pkcs1_pub(rsa_key* k, const u8* p, size_t n) {
    der d = {p, n}, seq, mn, me;
    if (der_expect(&d, 0x30, &seq) < 0) return -1;
    if (der_uint(&seq, &mn) < 0 || der_uint(&seq, &me) < 0) return -1;
    return rsa_set_public(k, mn.p, mn.n, me.p, me.n);
}
/* RSAPrivateKey: SEQ { version, n, e, d, p, q, dp, dq, qinv } */
static int rsa_parse_pkcs1_priv(rsa_key* k, const u8* p, size_t n) {
    der d = {p, n}, seq, v, mn, me, md, mp, mq, mdp, mdq, mqi; int hl;
    if (der_expect(&d, 0x30, &seq) < 0) return -1;
    if (der_uint(&seq, &v) < 0) return -1;
    if (der_uint(&seq, &mn) < 0 || der_uint(&seq, &me) < 0 || der_uint(&seq, &md) < 0) return -1;
    if (der_uint(&seq, &mp) < 0 || der_uint(&seq, &mq) < 0 || der_uint(&seq, &mdp) < 0 ||
        der_uint(&seq, &mdq) < 0 || der_uint(&seq, &mqi) < 0) return -1;
    if (rsa_set_public(k, mn.p, mn.n, me.p, me.n) < 0) return -1;
    hl = (int)((mp.n > mq.n ? mp.n : mq.n) + 3) / 4;
    if (hl > MAXL - 2) return -1;
    k->half_limbs = hl;
    bn_from_be(k->p, hl, mp.p, mp.n); bn_from_be(k->q, hl, mq.p, mq.n);
    bn_from_be(k->dp, hl, mdp.p, mdp.n); bn_from_be(k->dq, hl, mdq.p, mdq.n);
    bn_from_be(k->qinv, hl, mqi.p, mqi.n);
    if (!(k->p[0] & 1) || !(k->q[0] & 1)) return -1;
    { const u8* dd = md.p; size_t dn = md.n;
      while (dn > 0 && dd[0] == 0) { dd++; dn--; }
      if (dn > 0 && dn <= 4 * (size_t)k->n_limbs) { bn_from_be(k->d, k->n_limbs, dd, dn); k->has_d = 1; } }
    k->has_priv = 1;
    return 0;
}
/* out = in^e mod n (public op); in and out are k-byte big-endian. */
static int rsa_public_op(const rsa_key* k, const u8* in, u8* out) {
    mont M; u32 x[MAXL], r[MAXL]; size_t klen = (size_t)(k->bits + 7) / 8;
    if (mont_init(&M, k->n, k->n_limbs) < 0) return -1;
    bn_from_be(x, k->n_limbs, in, klen);
    if (bn_cmp(x, k->n, k->n_limbs) >= 0) return -1;
    mont_exp(&M, r, x, k->e, k->ebits);
    bn_to_be(out, klen, r, k->n_limbs);
    return 0;
}
/* CRT private op, checked by re-applying the public exponent. */
static int rsa_private_op(const rsa_key* k, const u8* in, u8* out) {
    mont Mp, Mq; int hl = k->half_limbs, nl = k->n_limbs;
    u32 x[MAXL], xr[2 * MAXL], s1[MAXL], s2[MAXL], h[MAXL], t[2 * MAXL], s[MAXL + 1], diff[MAXL];
    u8 chk[4 * MAXL]; size_t klen = (size_t)(k->bits + 7) / 8; u32 borrow;
    if (!k->has_priv) return -1;
    if (mont_init(&Mp, k->p, hl) < 0 || mont_init(&Mq, k->q, hl) < 0) return -1;
    bn_from_be(x, nl, in, klen);
    if (bn_cmp(x, k->n, nl) >= 0) return -1;
    memset(xr, 0, sizeof xr); memcpy(xr, x, sizeof(u32) * (size_t)nl);
    mont_reduce_wide(&Mp, s1, xr, 2 * hl); mont_exp(&Mp, s1, s1, k->dp, bn_bits(k->dp, hl));
    mont_reduce_wide(&Mq, s2, xr, 2 * hl); mont_exp(&Mq, s2, s2, k->dq, bn_bits(k->dq, hl));
    /* h = qinv * (s1 - s2) mod p ; s2 < q may exceed p, so reduce s2 mod p first */
    memset(t, 0, sizeof t); memcpy(t, s2, sizeof(u32) * (size_t)hl);
    mont_reduce_wide(&Mp, diff, t, hl);
    borrow = bn_sub(h, s1, diff, hl);
    if (borrow) bn_add(h, h, k->p, hl);
    {
        u32 a[MAXL], b[MAXL];
        mont_to(&Mp, a, h); mont_to(&Mp, b, k->qinv);
        mont_mul(&Mp, a, a, b); mont_from(&Mp, h, a);
    }
    /* s = s2 + h*q */
    bn_mul(t, h, hl, k->q, hl);
    memset(s, 0, sizeof s);
    memcpy(s, t, sizeof(u32) * (size_t)(2 * hl < nl + 1 ? 2 * hl : nl + 1));
    {
        u32 add2[MAXL + 1]; memset(add2, 0, sizeof add2); memcpy(add2, s2, sizeof(u32) * (size_t)hl);
        bn_add(s, s, add2, nl + 1);
    }
    bn_to_be(out, klen, s, nl);
    if (rsa_public_op(k, out, chk) < 0 || memcmp(chk, in, klen) != 0) { memset(out, 0, klen); return -1; }
    pk_wipe(s1, sizeof s1); pk_wipe(s2, sizeof s2); pk_wipe(h, sizeof h); pk_wipe(t, sizeof t);
    return 0;
}

static const u8 DI_SHA1[] = {0x30,0x21,0x30,0x09,0x06,0x05,0x2b,0x0e,0x03,0x02,0x1a,0x05,0x00,0x04,0x14};
static const u8 DI_SHA256[] = {0x30,0x31,0x30,0x0d,0x06,0x09,0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x01,0x05,0x00,0x04,0x20};
static const u8 DI_SHA384[] = {0x30,0x41,0x30,0x0d,0x06,0x09,0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x02,0x05,0x00,0x04,0x30};
static const u8 DI_SHA512[] = {0x30,0x51,0x30,0x0d,0x06,0x09,0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x03,0x05,0x00,0x04,0x40};

static int pkcs1_encode(int alg, const u8* hash, u8* em, size_t klen) {
    const u8* di; size_t dil, hl = (size_t)md_len(alg), tlen;
    switch (alg) {
        case 1: di = DI_SHA1; dil = sizeof DI_SHA1; break;
        case 256: di = DI_SHA256; dil = sizeof DI_SHA256; break;
        case 384: di = DI_SHA384; dil = sizeof DI_SHA384; break;
        case 512: di = DI_SHA512; dil = sizeof DI_SHA512; break;
        default: return -1;
    }
    tlen = dil + hl;
    if (klen < tlen + 11) return -1;
    em[0] = 0; em[1] = 1;
    memset(em + 2, 0xff, klen - tlen - 3);
    em[klen - tlen - 1] = 0;
    memcpy(em + klen - tlen, di, dil); memcpy(em + klen - hl, hash, hl);
    return 0;
}
static void mgf1(int alg, const u8* seed, size_t slen, u8* mask, size_t mlen) {
    u8 buf[64 + 4], h[64]; size_t done = 0; u32 ctr = 0; int hl = md_len(alg);
    memcpy(buf, seed, slen);
    while (done < mlen) {
        size_t take;
        buf[slen] = (u8)(ctr >> 24); buf[slen + 1] = (u8)(ctr >> 16); buf[slen + 2] = (u8)(ctr >> 8); buf[slen + 3] = (u8)ctr;
        trc_digest(alg, (const char*)buf, (int64_t)slen + 4, (char*)h);
        take = mlen - done < (size_t)hl ? mlen - done : (size_t)hl;
        memcpy(mask + done, h, take); done += take; ctr++;
    }
}
/* EMSA-PSS-VERIFY. slen < 0 = any salt length. */
static int pss_verify(int alg, const u8* mhash, const u8* em_full, size_t klen, int modbits, int slen) {
    size_t embits = (size_t)modbits - 1, emlen = (embits + 7) / 8, hl = (size_t)md_len(alg), dblen, i, ps;
    const u8* em = em_full + (klen - emlen); u8 db[4 * MAXL], mp[8 + 64 + 512], h2[64]; int top;
    if (klen > emlen && em_full[0] != 0) return -1;
    if (emlen < hl + 2 || em[emlen - 1] != 0xbc) return -1;
    dblen = emlen - hl - 1;
    top = (int)(8 * emlen - embits);
    if (em[0] & (0xff << (8 - top)) & 0xff) return -1;
    mgf1(alg, em + dblen, hl, db, dblen);
    for (i = 0; i < dblen; i++) db[i] ^= em[i];
    db[0] &= (u8)(0xff >> top);
    for (ps = 0; ps < dblen && db[ps] == 0; ps++) {}
    if (ps == dblen || db[ps] != 1) return -1;
    if (slen >= 0 && dblen - ps - 1 != (size_t)slen) return -1;
    if (dblen - ps - 1 > 512) return -1;
    memset(mp, 0, 8); memcpy(mp + 8, mhash, hl); memcpy(mp + 8 + hl, db + ps + 1, dblen - ps - 1);
    trc_digest(alg, (const char*)mp, (int64_t)(8 + hl + dblen - ps - 1), (char*)h2);
    return memcmp(h2, em + dblen, hl) == 0 ? 0 : -1;
}
static int pss_encode(int alg, const u8* mhash, const u8* salt, size_t slen, u8* em_full, size_t klen, int modbits) {
    size_t embits = (size_t)modbits - 1, emlen = (embits + 7) / 8, hl = (size_t)md_len(alg), dblen, i;
    u8* em = em_full + (klen - emlen); u8 mp[8 + 64 + 64], h[64], db[4 * MAXL]; int top;
    if (emlen < hl + slen + 2 || slen > 64) return -1;
    memset(em_full, 0, klen);
    memset(mp, 0, 8); memcpy(mp + 8, mhash, hl); memcpy(mp + 8 + hl, salt, slen);
    trc_digest(alg, (const char*)mp, (int64_t)(8 + hl + slen), (char*)h);
    dblen = emlen - hl - 1;
    memset(db, 0, dblen); db[dblen - slen - 1] = 1; memcpy(db + dblen - slen, salt, slen);
    mgf1(alg, h, hl, em, dblen);
    for (i = 0; i < dblen; i++) em[i] ^= db[i];
    top = (int)(8 * emlen - embits);
    em[0] &= (u8)(0xff >> top);
    memcpy(em + dblen, h, hl); em[emlen - 1] = 0xbc;
    return 0;
}

/* scheme: 0 = PKCS#1 v1.5, 1 = PSS (salt = hash length unless pss_salt >= 0). */
static int rsa_verify_hash(const rsa_key* k, int scheme, int alg, const u8* hash, const u8* sig, size_t siglen, int pss_salt) {
    u8 em[4 * MAXL], want[4 * MAXL]; size_t klen = (size_t)(k->bits + 7) / 8;
    if (md_len(alg) < 0 || siglen != klen) return -1;
    if (rsa_public_op(k, sig, em) < 0) return -1;
    if (scheme == 0) {
        if (pkcs1_encode(alg, hash, want, klen) < 0) return -1;
        return trc_ct_eq((const char*)em, (int64_t)klen, (const char*)want, (int64_t)klen) ? 0 : -1;
    }
    return pss_verify(alg, hash, em, klen, k->bits, pss_salt);
}

/* ═══════════════════════════ elliptic curves ═════════════════════════════ */
typedef struct {
    int id, nl, bytes;     /* 256 or 384; limbs; field bytes */
    mont P, N;             /* field and group-order Montgomery contexts */
    u32 b[MAXL];           /* curve b, Montgomery form */
    u32 gx[MAXL], gy[MAXL];/* generator, Montgomery form */
    u32 n[MAXL];           /* order, normal form */
    int ready;
} curve;
typedef struct { u32 x[12], y[12], z[12]; } jpt;   /* Jacobian point, Montgomery coords */

static curve C256, C384;

static void hex_to_bn(u32* d, int n, const char* hex) {
    u8 b[64]; size_t l = strlen(hex) / 2, i;
    for (i = 0; i < l; i++) {
        int hi = hex[2 * i], lo = hex[2 * i + 1];
        hi = hi <= '9' ? hi - '0' : (hi | 32) - 'a' + 10; lo = lo <= '9' ? lo - '0' : (lo | 32) - 'a' + 10;
        b[i] = (u8)(hi << 4 | lo);
    }
    bn_from_be(d, n, b, l);
}
static curve* get_curve(int id) {
    curve* c;
    const char *p, *bh, *gx, *gy, *n;
    if (id == 256) {
        c = &C256;
        p = "ffffffff00000001000000000000000000000000ffffffffffffffffffffffff";
        bh = "5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b";
        gx = "6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296";
        gy = "4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5";
        n = "ffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551";
    } else if (id == 384) {
        c = &C384;
        p = "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffeffffffff0000000000000000ffffffff";
        bh = "b3312fa7e23ee7e4988e056be3f82d19181d9c6efe8141120314088f5013875ac656398d8a2ed19d2a85c8edd3ec2aef";
        gx = "aa87ca22be8b05378eb1c71ef320ad746e1d3b628ba79b9859f741e082542a385502f25dbf55296c3a545e3872760ab7";
        gy = "3617de4a96262c6f5d9e98bf9292dc29f8f41dbd289a147ce9da3113b5f0b8c00a60b1ce1d7e819d7a431d7c90ea0e5f";
        n = "ffffffffffffffffffffffffffffffffffffffffffffffffc7634d81f4372ddf581a0db248b0a77aecec196accc52973";
    } else {
        return NULL;
    }
    if (!c->ready) {
        u32 t[MAXL];
        c->id = id; c->bytes = id / 8; c->nl = id / 32;
        hex_to_bn(t, c->nl, p); mont_init(&c->P, t, c->nl);
        hex_to_bn(c->n, c->nl, n); mont_init(&c->N, c->n, c->nl);
        hex_to_bn(t, c->nl, bh); mont_to(&c->P, c->b, t);
        hex_to_bn(t, c->nl, gx); mont_to(&c->P, c->gx, t);
        hex_to_bn(t, c->nl, gy); mont_to(&c->P, c->gy, t);
        c->ready = 1;
    }
    return c;
}
static void fadd(const curve* c, u32* r, const u32* a, const u32* b) {
    u32 s[MAXL]; u32 carry = bn_add(r, a, b, c->nl), borrow = bn_sub(s, r, c->P.m, c->nl);
    bn_cmov(r, s, c->nl, carry | (borrow == 0));
}
static void fsub(const curve* c, u32* r, const u32* a, const u32* b) {
    u32 s[MAXL]; u32 borrow = bn_sub(r, a, b, c->nl);
    bn_add(s, r, c->P.m, c->nl);
    bn_cmov(r, s, c->nl, borrow);
}
static void fmul(const curve* c, u32* r, const u32* a, const u32* b) { mont_mul(&c->P, r, a, b); }

static void pt_dbl(const curve* c, jpt* r, const jpt* p) {
    u32 delta[12], gamma[12], beta[12], alpha[12], t1[12], t2[12], x3[12], y3[12], z3[12];
    fmul(c, delta, p->z, p->z);
    fmul(c, gamma, p->y, p->y);
    fmul(c, beta, p->x, gamma);
    fsub(c, t1, p->x, delta); fadd(c, t2, p->x, delta); fmul(c, alpha, t1, t2);
    fadd(c, t1, alpha, alpha); fadd(c, alpha, t1, alpha);           /* 3 * (x-d)(x+d) */
    fmul(c, x3, alpha, alpha);
    fadd(c, t1, beta, beta); fadd(c, t1, t1, t1); fadd(c, t2, t1, t1); /* t1 = 4b, t2 = 8b */
    fsub(c, x3, x3, t2);
    fadd(c, z3, p->y, p->z); fmul(c, z3, z3, z3); fsub(c, z3, z3, gamma); fsub(c, z3, z3, delta);
    fsub(c, t1, t1, x3); fmul(c, y3, alpha, t1);
    fmul(c, t2, gamma, gamma); fadd(c, t2, t2, t2); fadd(c, t2, t2, t2); fadd(c, t2, t2, t2);
    fsub(c, y3, y3, t2);
    memcpy(r->x, x3, sizeof x3); memcpy(r->y, y3, sizeof y3); memcpy(r->z, z3, sizeof z3);
}
static void pt_add(const curve* c, jpt* r, const jpt* p, const jpt* q) {
    u32 z1z1[12], z2z2[12], u1[12], u2[12], s1[12], s2[12], h[12], i[12], j[12], rr[12], v[12], t[12];
    jpt out; int n = c->nl;
    if (bn_is_zero(p->z, n)) { *r = *q; return; }
    if (bn_is_zero(q->z, n)) { *r = *p; return; }
    fmul(c, z1z1, p->z, p->z); fmul(c, z2z2, q->z, q->z);
    fmul(c, u1, p->x, z2z2); fmul(c, u2, q->x, z1z1);
    fmul(c, s1, p->y, q->z); fmul(c, s1, s1, z2z2);
    fmul(c, s2, q->y, p->z); fmul(c, s2, s2, z1z1);
    fsub(c, h, u2, u1); fsub(c, rr, s2, s1);
    if (bn_is_zero(h, n)) {
        if (bn_is_zero(rr, n)) { pt_dbl(c, r, p); return; }
        memset(r, 0, sizeof *r); return;
    }
    fadd(c, i, h, h); fmul(c, i, i, i);
    fmul(c, j, h, i);
    fadd(c, rr, rr, rr);
    fmul(c, v, u1, i);
    fmul(c, out.x, rr, rr); fsub(c, out.x, out.x, j); fsub(c, out.x, out.x, v); fsub(c, out.x, out.x, v);
    fsub(c, t, v, out.x); fmul(c, out.y, rr, t);
    fmul(c, t, s1, j); fadd(c, t, t, t); fsub(c, out.y, out.y, t);
    fadd(c, t, p->z, q->z); fmul(c, t, t, t); fsub(c, t, t, z1z1); fsub(c, t, t, z2z2); fmul(c, out.z, t, h);
    *r = out;
}
static void pt_cmov(jpt* r, const jpt* a, int n, u32 cond) {
    bn_cmov(r->x, a->x, n, cond); bn_cmov(r->y, a->y, n, cond); bn_cmov(r->z, a->z, n, cond);
}
/* r = k * p, k given as nl limbs (normal form). Double-and-add-always with
 * conditional moves; processes every bit of the field width. */
static void pt_mul(const curve* c, jpt* r, const u32* k, const jpt* p) {
    jpt acc, t; int i;
    memset(&acc, 0, sizeof acc);
    for (i = c->nl * 32 - 1; i >= 0; i--) {
        pt_dbl(c, &acc, &acc);
        pt_add(c, &t, &acc, p);
        pt_cmov(&acc, &t, c->nl, (u32)bn_bit(k, i));
    }
    *r = acc;
}
static void f_inv(const curve* c, u32* r, const u32* a) {
    /* a^(p-2), Montgomery in/out: convert out, exponentiate, convert back */
    u32 pm2[MAXL], two[MAXL], an[MAXL];
    memset(two, 0, sizeof two); two[0] = 2;
    bn_sub(pm2, c->P.m, two, c->nl);
    mont_from(&c->P, an, a);
    mont_exp(&c->P, an, an, pm2, bn_bits(pm2, c->nl));
    mont_to(&c->P, r, an);
}
/* Jacobian -> affine normal-form big-endian x||y. Returns -1 at infinity. */
static int pt_to_affine(const curve* c, const jpt* p, u8* x, u8* y) {
    u32 zi[12], zi2[12], ax[12], ay[12];
    if (bn_is_zero(p->z, c->nl)) return -1;
    f_inv(c, zi, p->z);
    fmul(c, zi2, zi, zi);
    fmul(c, ax, p->x, zi2);
    fmul(c, zi2, zi2, zi); fmul(c, ay, p->y, zi2);
    mont_from(&c->P, ax, ax); mont_from(&c->P, ay, ay);
    if (x) bn_to_be(x, (size_t)c->bytes, ax, c->nl);
    if (y) bn_to_be(y, (size_t)c->bytes, ay, c->nl);
    return 0;
}
/* Decode SEC1 point (uncompressed or compressed) and check it is on the curve. */
static int pt_decode(const curve* c, jpt* r, const u8* s, size_t n) {
    u32 x[12], y[12], rhs[12], t[12], y2[12]; int nb = c->bytes;
    if (n == (size_t)(1 + 2 * nb) && s[0] == 4) {
        bn_from_be(x, c->nl, s + 1, (size_t)nb); bn_from_be(y, c->nl, s + 1 + nb, (size_t)nb);
        if (bn_cmp(x, c->P.m, c->nl) >= 0 || bn_cmp(y, c->P.m, c->nl) >= 0) return -1;
        mont_to(&c->P, r->x, x); mont_to(&c->P, r->y, y);
    } else if (n == (size_t)(1 + nb) && (s[0] == 2 || s[0] == 3)) {
        u32 e[MAXL], one[MAXL], yn[12];
        memset(e, 0, sizeof e);
        bn_from_be(x, c->nl, s + 1, (size_t)nb);
        if (bn_cmp(x, c->P.m, c->nl) >= 0) return -1;
        mont_to(&c->P, r->x, x);
        fmul(c, rhs, r->x, r->x); fmul(c, rhs, rhs, r->x);
        fsub(c, rhs, rhs, r->x); fsub(c, rhs, rhs, r->x); fsub(c, rhs, rhs, r->x); fadd(c, rhs, rhs, c->b);
        /* sqrt via (p+1)/4 (both curves have p = 3 mod 4) */
        memset(one, 0, sizeof one); one[0] = 1;
        bn_add(e, c->P.m, one, c->nl);
        { int i; for (i = 0; i < c->nl; i++) e[i] = (e[i] >> 2) | (i + 1 < c->nl ? e[i + 1] << 30 : 0); }
        mont_from(&c->P, t, rhs);
        mont_exp(&c->P, yn, t, e, bn_bits(e, c->nl));
        if ((int)(yn[0] & 1) != (s[0] & 1)) bn_sub(yn, c->P.m, yn, c->nl);
        mont_to(&c->P, r->y, yn);
    } else {
        return -1;
    }
    memcpy(r->z, c->P.one, sizeof(u32) * (size_t)c->nl);
    /* y^2 == x^3 - 3x + b */
    fmul(c, y2, r->y, r->y);
    fmul(c, rhs, r->x, r->x); fmul(c, rhs, rhs, r->x);
    fsub(c, rhs, rhs, r->x); fsub(c, rhs, rhs, r->x); fsub(c, rhs, rhs, r->x); fadd(c, rhs, rhs, c->b);
    return bn_cmp(y2, rhs, c->nl) == 0 ? 0 : -1;
}
static void pt_gen(const curve* c, jpt* g) {
    memcpy(g->x, c->gx, sizeof g->x); memcpy(g->y, c->gy, sizeof g->y); memcpy(g->z, c->P.one, sizeof g->z);
}
/* scalar in [1, n-1]? */
static int scalar_ok(const curve* c, const u32* k) { return !bn_is_zero(k, c->nl) && bn_cmp(k, c->n, c->nl) < 0; }

/* hash -> integer mod n (FIPS 186-4 bits2int + reduce) */
static void hash_to_z(const curve* c, u32* z, const u8* h, size_t hl) {
    size_t take = hl < (size_t)c->bytes ? hl : (size_t)c->bytes; u32 s[MAXL]; u32 borrow;
    bn_from_be(z, c->nl, h, take);   /* curves here have byte-aligned orders */
    borrow = bn_sub(s, z, c->n, c->nl);
    bn_cmov(z, s, c->nl, borrow == 0);
}
static void scal_mul(const curve* c, u32* r, const u32* a, const u32* b) {   /* mod n, normal form */
    u32 am[12], bm[12];
    mont_to(&c->N, am, a); mont_to(&c->N, bm, b); mont_mul(&c->N, am, am, bm); mont_from(&c->N, r, am);
}
static void scal_inv(const curve* c, u32* r, const u32* a) {
    u32 nm2[MAXL], two[MAXL];
    memset(two, 0, sizeof two); two[0] = 2;
    bn_sub(nm2, c->n, two, c->nl);
    mont_exp(&c->N, r, a, nm2, bn_bits(nm2, c->nl));
}
static void scal_add(const curve* c, u32* r, const u32* a, const u32* b) {
    u32 s[MAXL]; u32 carry = bn_add(r, a, b, c->nl), borrow = bn_sub(s, r, c->n, c->nl);
    bn_cmov(r, s, c->nl, carry | (borrow == 0));
}

static int ecdsa_verify_hash(const curve* c, const u8* pub, size_t publen, const u8* h, size_t hl, const u8* r_be, const u8* s_be) {
    jpt Q, G, A, B, X; u32 r[12], s[12], z[12], w[12], u1[12], u2[12], xr[12]; u8 xb[48];
    if (pt_decode(c, &Q, pub, publen) < 0) return -1;
    bn_from_be(r, c->nl, r_be, (size_t)c->bytes); bn_from_be(s, c->nl, s_be, (size_t)c->bytes);
    if (!scalar_ok(c, r) || !scalar_ok(c, s)) return -1;
    hash_to_z(c, z, h, hl);
    scal_inv(c, w, s);
    scal_mul(c, u1, z, w); scal_mul(c, u2, r, w);
    pt_gen(c, &G);
    pt_mul(c, &A, u1, &G); pt_mul(c, &B, u2, &Q); pt_add(c, &X, &A, &B);
    if (pt_to_affine(c, &X, xb, NULL) < 0) return -1;
    bn_from_be(xr, c->nl, xb, (size_t)c->bytes);
    { u32 sub[12]; u32 borrow = bn_sub(sub, xr, c->n, c->nl); bn_cmov(xr, sub, c->nl, borrow == 0); }
    return bn_cmp(xr, r, c->nl) == 0 ? 0 : -1;
}

/* RFC 6979 deterministic nonce (HMAC-DRBG over `alg`). */
static void rfc6979_k(const curve* c, int alg, const u32* x, const u8* h, size_t hl, int attempt, u32* k) {
    u8 V[64], K[64], buf[64 + 1 + 48 + 48 + 1], xo[48], ho[48], T[48]; int hlen = md_len(alg), rlen = c->bytes;
    u32 z[12]; int tries;
    bn_to_be(xo, (size_t)rlen, x, c->nl);
    hash_to_z(c, z, h, hl); bn_to_be(ho, (size_t)rlen, z, c->nl);
    memset(V, 1, (size_t)hlen); memset(K, 0, (size_t)hlen);
    memcpy(buf, V, (size_t)hlen); buf[hlen] = 0; memcpy(buf + hlen + 1, xo, (size_t)rlen); memcpy(buf + hlen + 1 + rlen, ho, (size_t)rlen);
    trc_hmac(alg, (const char*)K, hlen, (const char*)buf, hlen + 1 + 2 * rlen, (char*)K);
    trc_hmac(alg, (const char*)K, hlen, (const char*)V, hlen, (char*)V);
    memcpy(buf, V, (size_t)hlen); buf[hlen] = 1;
    trc_hmac(alg, (const char*)K, hlen, (const char*)buf, hlen + 1 + 2 * rlen, (char*)K);
    trc_hmac(alg, (const char*)K, hlen, (const char*)V, hlen, (char*)V);
    for (tries = 0; ; tries++) {
        int tl = 0;
        while (tl < rlen) {
            int take;
            trc_hmac(alg, (const char*)K, hlen, (const char*)V, hlen, (char*)V);
            take = rlen - tl < hlen ? rlen - tl : hlen;
            memcpy(T + tl, V, (size_t)take); tl += take;
        }
        bn_from_be(k, c->nl, T, (size_t)rlen);
        if (scalar_ok(c, k) && tries >= attempt) break;
        memcpy(buf, V, (size_t)hlen); buf[hlen] = 0;
        trc_hmac(alg, (const char*)K, hlen, (const char*)buf, hlen + 1, (char*)K);
        trc_hmac(alg, (const char*)K, hlen, (const char*)V, hlen, (char*)V);
    }
    pk_wipe(K, sizeof K); pk_wipe(xo, sizeof xo);
}
static int ecdsa_sign_hash(const curve* c, int alg, const u8* priv, const u8* h, size_t hl, u8* r_be, u8* s_be) {
    u32 d[12], k[12], z[12], r[12], s[12], ki[12], t[12]; jpt G, R; u8 xb[48]; int attempt;
    bn_from_be(d, c->nl, priv, (size_t)c->bytes);
    if (!scalar_ok(c, d)) return -1;
    hash_to_z(c, z, h, hl);
    pt_gen(c, &G);
    for (attempt = 0; attempt < 8; attempt++) {
        rfc6979_k(c, alg, d, h, hl, attempt, k);
        pt_mul(c, &R, k, &G);
        if (pt_to_affine(c, &R, xb, NULL) < 0) continue;
        bn_from_be(r, c->nl, xb, (size_t)c->bytes);
        { u32 sub[12]; u32 borrow = bn_sub(sub, r, c->n, c->nl); bn_cmov(r, sub, c->nl, borrow == 0); }
        if (bn_is_zero(r, c->nl)) continue;
        scal_inv(c, ki, k);
        scal_mul(c, t, r, d); scal_add(c, t, t, z); scal_mul(c, s, ki, t);
        if (bn_is_zero(s, c->nl)) continue;
        bn_to_be(r_be, (size_t)c->bytes, r, c->nl); bn_to_be(s_be, (size_t)c->bytes, s, c->nl);
        pk_wipe(d, sizeof d); pk_wipe(k, sizeof k); pk_wipe(ki, sizeof ki);
        return 0;
    }
    return -1;
}

/* DER ECDSA-Sig-Value <-> fixed r||s */
static int ecsig_from_der(const curve* c, const u8* sig, size_t n, u8* r, u8* s) {
    der d = {sig, n}, seq, a, b;
    if (der_expect(&d, 0x30, &seq) < 0 || d.n != 0) return -1;
    if (der_uint(&seq, &a) < 0 || der_uint(&seq, &b) < 0 || seq.n != 0) return -1;
    if (a.n > (size_t)c->bytes || b.n > (size_t)c->bytes) return -1;
    memset(r, 0, (size_t)c->bytes); memset(s, 0, (size_t)c->bytes);
    memcpy(r + c->bytes - a.n, a.p, a.n); memcpy(s + c->bytes - b.n, b.p, b.n);
    return 0;
}
static size_t der_put_uint(u8* o, const u8* v, size_t n) {
    size_t k = 0;
    while (n > 1 && v[0] == 0) { v++; n--; }
    o[k++] = 0x02;
    if (v[0] & 0x80) { o[k++] = (u8)(n + 1); o[k++] = 0; } else o[k++] = (u8)n;
    memcpy(o + k, v, n);
    return k + n;
}
static size_t ecsig_to_der(const u8* r, const u8* s, size_t nb, u8* out) {
    u8 tmp[2 * 52]; size_t a = der_put_uint(tmp, r, nb), b = der_put_uint(tmp + a, s, nb), k = 0;
    out[k++] = 0x30;
    if (a + b >= 0x80) { out[k++] = 0x81; }
    out[k++] = (u8)(a + b);
    memcpy(out + k, tmp, a + b);
    return k + a + b;
}

/* ═══════════════════════════ key handles ═════════════════════════════════
 * A parsed key lives in a malloc'd pk_key; Tauraro holds it as an opaque
 * pointer and must call trc_pk_free. kind: 1 RSA, 2 EC P-256, 3 EC P-384,
 * 4 Ed25519 (public only: verification is delegated to the runtime). */
typedef struct {
    int kind, has_priv;
    rsa_key rsa;
    u8 ec_pub[1 + 2 * 48]; size_t ec_publen;
    u8 ec_priv[48];
    u8 ed_pub[32];
} pk_key;

static int curve_for_oid(const der* o) { if (oid_eq(o, OID_P256, sizeof OID_P256)) return 256; if (oid_eq(o, OID_P384, sizeof OID_P384)) return 384; return 0; }

/* SubjectPublicKeyInfo */
static int spki_parse(pk_key* k, const u8* p, size_t n) {
    der d = {p, n}, seq, alg, oid, bits; int t;
    memset(k, 0, sizeof *k);
    if (der_expect(&d, 0x30, &seq) < 0) return -1;
    if (der_expect(&seq, 0x30, &alg) < 0 || der_expect(&alg, 0x06, &oid) < 0) return -1;
    if (der_expect(&seq, 0x03, &bits) < 0 || bits.n < 1 || bits.p[0] != 0) return -1;
    bits.p++; bits.n--;
    if (oid_eq(&oid, OID_RSA, sizeof OID_RSA) || oid_eq(&oid, OID_RSA_PSS, sizeof OID_RSA_PSS)) {
        k->kind = 1;
        return rsa_parse_pkcs1_pub(&k->rsa, bits.p, bits.n);
    }
    if (oid_eq(&oid, OID_EC_PUB, sizeof OID_EC_PUB)) {
        der cv; int cid; jpt Q; curve* c;
        if (der_next(&alg, &t, &cv) < 0 || t != 0x06) return -1;
        cid = curve_for_oid(&cv);
        if (!cid) return -2;
        c = get_curve(cid);
        if (pt_decode(c, &Q, bits.p, bits.n) < 0) return -1;
        k->kind = cid == 256 ? 2 : 3;
        /* store uncompressed */
        k->ec_pub[0] = 4; pt_to_affine(c, &Q, k->ec_pub + 1, k->ec_pub + 1 + c->bytes);
        k->ec_publen = (size_t)(1 + 2 * c->bytes);
        return 0;
    }
    if (oid_eq(&oid, OID_ED25519, sizeof OID_ED25519)) {
        if (bits.n != 32) return -1;
        k->kind = 4; memcpy(k->ed_pub, bits.p, 32);
        return 0;
    }
    return -2;
}
/* SEC1 ECPrivateKey: SEQ { 1, OCTET priv, [0] OID opt, [1] BIT pub opt } */
static int sec1_parse(pk_key* k, const u8* p, size_t n, int cid_hint) {
    der d = {p, n}, seq, v, priv, tagged; int t, cid = cid_hint; curve* c; jpt G, Q; u32 x[12];
    if (der_expect(&d, 0x30, &seq) < 0 || der_uint(&seq, &v) < 0 || v.n != 1 || v.p[0] != 1) return -1;
    if (der_expect(&seq, 0x04, &priv) < 0) return -1;
    while (seq.n) {
        if (der_next(&seq, &t, &tagged) < 0) return -1;
        if (t == 0xa0) { der o; if (der_expect(&tagged, 0x06, &o) < 0) return -1; cid = curve_for_oid(&o); }
    }
    if (!cid) return -2;
    c = get_curve(cid);
    if (priv.n > (size_t)c->bytes) return -1;
    memset(k, 0, sizeof *k);
    k->kind = cid == 256 ? 2 : 3; k->has_priv = 1;
    memset(k->ec_priv, 0, sizeof k->ec_priv);
    memcpy(k->ec_priv + c->bytes - priv.n, priv.p, priv.n);
    bn_from_be(x, c->nl, k->ec_priv, (size_t)c->bytes);
    if (!scalar_ok(c, x)) return -1;
    pt_gen(c, &G); pt_mul(c, &Q, x, &G);
    k->ec_pub[0] = 4; pt_to_affine(c, &Q, k->ec_pub + 1, k->ec_pub + 1 + c->bytes);
    k->ec_publen = (size_t)(1 + 2 * c->bytes);
    pk_wipe(x, sizeof x);
    return 0;
}
/* PKCS#8 PrivateKeyInfo */
static int pkcs8_parse(pk_key* k, const u8* p, size_t n) {
    der d = {p, n}, seq, v, alg, oid, okey; int t;
    if (der_expect(&d, 0x30, &seq) < 0 || der_uint(&seq, &v) < 0) return -1;
    if (der_expect(&seq, 0x30, &alg) < 0 || der_expect(&alg, 0x06, &oid) < 0) return -1;
    if (der_expect(&seq, 0x04, &okey) < 0) return -1;
    if (oid_eq(&oid, OID_RSA, sizeof OID_RSA)) {
        memset(k, 0, sizeof *k); k->kind = 1;
        if (rsa_parse_pkcs1_priv(&k->rsa, okey.p, okey.n) < 0) return -1;
        k->has_priv = 1; return 0;
    }
    if (oid_eq(&oid, OID_EC_PUB, sizeof OID_EC_PUB)) {
        der cv; int cid;
        if (der_next(&alg, &t, &cv) < 0 || t != 0x06) return -1;
        cid = curve_for_oid(&cv);
        if (!cid) return -2;
        return sec1_parse(k, okey.p, okey.n, cid);
    }
    return -2;
}

/* Parse any supported DER key. Returns a handle or NULL; *err gets a code:
 * -1 malformed, -2 unsupported algorithm. */
void* trc_pk_parse_der(const char* der_bytes, int64_t n, int64_t* err) {
    pk_key* k = (pk_key*)calloc(1, sizeof(pk_key)); int r;
    const u8* p = (const u8*)der_bytes;
    *err = 0;
    if (!k) { *err = -3; return NULL; }
    r = spki_parse(k, p, (size_t)n);
    if (r == 0) return k;
    r = pkcs8_parse(k, p, (size_t)n);
    if (r == 0) return k;
    memset(k, 0, sizeof *k);
    if (rsa_parse_pkcs1_priv(&k->rsa, p, (size_t)n) == 0) { k->kind = 1; k->has_priv = 1; return k; }
    memset(k, 0, sizeof *k);
    if (sec1_parse(k, p, (size_t)n, 0) == 0) return k;
    memset(k, 0, sizeof *k);
    if (rsa_parse_pkcs1_pub(&k->rsa, p, (size_t)n) == 0) { k->kind = 1; return k; }
    pk_wipe(k, sizeof *k); free(k);
    *err = r == -2 ? -2 : -1;
    return NULL;
}
void trc_pk_free(void* h) { if (h) { pk_wipe(h, sizeof(pk_key)); free(h); } }
int64_t trc_pk_kind(void* h) { return h ? ((pk_key*)h)->kind : 0; }
int64_t trc_pk_has_private(void* h) { return h ? ((pk_key*)h)->has_priv : 0; }
int64_t trc_pk_bits(void* h) {
    pk_key* k = (pk_key*)h;
    if (!k) return 0;
    if (k->kind == 1) return k->rsa.bits;
    if (k->kind == 2) return 256;
    if (k->kind == 3) return 384;
    if (k->kind == 4) return 256;
    return 0;
}
/* Raw public key: RSA -> modulus bytes, EC -> uncompressed point, Ed25519 -> 32 bytes. */
int64_t trc_pk_public_raw(void* h, char* out, int64_t cap) {
    pk_key* k = (pk_key*)h; size_t len;
    if (!k) return -1;
    if (k->kind == 1) {
        len = (size_t)(k->rsa.bits + 7) / 8;
        if ((int64_t)len > cap) return -1;
        bn_to_be((u8*)out, len, k->rsa.n, k->rsa.n_limbs); return (int64_t)len;
    }
    if (k->kind == 2 || k->kind == 3) { if ((int64_t)k->ec_publen > cap) return -1; memcpy(out, k->ec_pub, k->ec_publen); return (int64_t)k->ec_publen; }
    if (k->kind == 4) { if (cap < 32) return -1; memcpy(out, k->ed_pub, 32); return 32; }
    return -1;
}

/* scheme: 0 PKCS1v15, 1 PSS (RSA); ignored for EC. alg = hash id.
 * For EC, sig is DER unless sig_raw != 0 (then r||s). Returns 0 if valid. */
static int pk_verify_hash(pk_key* k, int scheme, int alg, const u8* h, size_t hl, const u8* sig, size_t siglen, int sig_raw) {
    if (k->kind == 1) return rsa_verify_hash(&k->rsa, scheme, alg, h, sig, siglen, scheme == 1 ? (int)hl : -1);
    if (k->kind == 2 || k->kind == 3) {
        curve* c = get_curve(k->kind == 2 ? 256 : 384); u8 r[48], s[48];
        if (sig_raw) {
            if (siglen != (size_t)(2 * c->bytes)) return -1;
            memcpy(r, sig, (size_t)c->bytes); memcpy(s, sig + c->bytes, (size_t)c->bytes);
        } else if (ecsig_from_der(c, sig, siglen, r, s) < 0) return -1;
        return ecdsa_verify_hash(c, k->ec_pub, k->ec_publen, h, hl, r, s);
    }
    return -1;
}
int64_t trc_pk_verify(void* h, int64_t scheme, int64_t alg, const char* msg, int64_t mlen, const char* sig, int64_t siglen, int64_t sig_raw) {
    u8 hash[64]; pk_key* k = (pk_key*)h;
    if (!k || md_len((int)alg) < 0) return -1;
    trc_digest(alg, msg, mlen, (char*)hash);
    return pk_verify_hash(k, (int)scheme, (int)alg, hash, (size_t)md_len((int)alg), (const u8*)sig, (size_t)siglen, (int)sig_raw) == 0 ? 1 : 0;
}
/* Sign; returns signature length or -1. EC signatures are DER unless sig_raw. */
int64_t trc_pk_sign(void* h, int64_t scheme, int64_t alg, const char* msg, int64_t mlen, char* out, int64_t cap, int64_t sig_raw) {
    u8 hash[64]; pk_key* k = (pk_key*)h; int hl = md_len((int)alg);
    if (!k || !k->has_priv || hl < 0) return -1;
    trc_digest(alg, msg, mlen, (char*)hash);
    if (k->kind == 1) {
        u8 em[4 * MAXL]; size_t klen = (size_t)(k->rsa.bits + 7) / 8;
        if ((int64_t)klen > cap) return -1;
        if (scheme == 0) { if (pkcs1_encode((int)alg, hash, em, klen) < 0) return -1; }
        else {
            u8 salt[64];
            if (trc_random((char*)salt, hl) != 0) return -1;
            if (pss_encode((int)alg, hash, salt, (size_t)hl, em, klen, k->rsa.bits) < 0) return -1;
        }
        if (rsa_private_op(&k->rsa, em, (u8*)out) < 0) return -1;
        return (int64_t)klen;
    }
    if (k->kind == 2 || k->kind == 3) {
        curve* c = get_curve(k->kind == 2 ? 256 : 384); u8 r[48], s[48];
        if (ecdsa_sign_hash(c, (int)alg, k->ec_priv, hash, (size_t)hl, r, s) < 0) return -1;
        if (sig_raw) {
            if (cap < 2 * c->bytes) return -1;
            memcpy(out, r, (size_t)c->bytes); memcpy(out + c->bytes, s, (size_t)c->bytes);
            return 2 * c->bytes;
        }
        if (cap < 2 * c->bytes + 9) return -1;
        return (int64_t)ecsig_to_der(r, s, (size_t)c->bytes, (u8*)out);
    }
    return -1;
}
/* EC key generation: curve 256/384. Writes the private scalar (bytes) and
 * returns its length; use trc_pk_from_ec_private to make a handle. */
void* trc_pk_ec_from_private(int64_t cid, const char* priv, int64_t n) {
    curve* c = get_curve((int)cid); pk_key* k; u32 x[12]; jpt G, Q;
    if (!c || n != c->bytes) return NULL;
    k = (pk_key*)calloc(1, sizeof(pk_key));
    if (!k) return NULL;
    memcpy(k->ec_priv, priv, (size_t)n);
    bn_from_be(x, c->nl, k->ec_priv, (size_t)c->bytes);
    if (!scalar_ok(c, x)) { free(k); return NULL; }
    k->kind = cid == 256 ? 2 : 3; k->has_priv = 1;
    pt_gen(c, &G); pt_mul(c, &Q, x, &G);
    k->ec_pub[0] = 4; pt_to_affine(c, &Q, k->ec_pub + 1, k->ec_pub + 1 + c->bytes);
    k->ec_publen = (size_t)(1 + 2 * c->bytes);
    pk_wipe(x, sizeof x);
    return k;
}
void* trc_pk_ec_generate(int64_t cid) {
    curve* c = get_curve((int)cid); u8 buf[48]; int i;
    if (!c) return NULL;
    for (i = 0; i < 64; i++) {
        void* k;
        if (trc_random((char*)buf, c->bytes) != 0) return NULL;
        k = trc_pk_ec_from_private(cid, (const char*)buf, c->bytes);
        if (k) { pk_wipe(buf, sizeof buf); return k; }
    }
    return NULL;
}
void* trc_pk_ec_from_public(int64_t cid, const char* pt, int64_t n) {
    curve* c = get_curve((int)cid); pk_key* k; jpt Q;
    if (!c) return NULL;
    if (pt_decode(c, &Q, (const u8*)pt, (size_t)n) < 0) return NULL;
    k = (pk_key*)calloc(1, sizeof(pk_key));
    if (!k) return NULL;
    k->kind = cid == 256 ? 2 : 3;
    k->ec_pub[0] = 4; pt_to_affine(c, &Q, k->ec_pub + 1, k->ec_pub + 1 + c->bytes);
    k->ec_publen = (size_t)(1 + 2 * c->bytes);
    return k;
}
void* trc_pk_rsa_from_public(const char* n, int64_t nlen, const char* e, int64_t elen) {
    pk_key* k = (pk_key*)calloc(1, sizeof(pk_key));
    if (!k) return NULL;
    if (rsa_set_public(&k->rsa, (const u8*)n, (size_t)nlen, (const u8*)e, (size_t)elen) < 0) { free(k); return NULL; }
    k->kind = 1;
    return k;
}
int64_t trc_pk_ec_private_raw(void* h, char* out) {
    pk_key* k = (pk_key*)h; int nb;
    if (!k || !k->has_priv || (k->kind != 2 && k->kind != 3)) return -1;
    nb = k->kind == 2 ? 32 : 48; memcpy(out, k->ec_priv, (size_t)nb); return nb;
}
/* ECDH: x coordinate of priv * peer_point. Returns length or -1. */
int64_t trc_pk_ecdh(void* h, const char* peer, int64_t plen, char* out) {
    pk_key* k = (pk_key*)h; curve* c; jpt Q, S; u32 d[12];
    if (!k || !k->has_priv || (k->kind != 2 && k->kind != 3)) return -1;
    c = get_curve(k->kind == 2 ? 256 : 384);
    if (pt_decode(c, &Q, (const u8*)peer, (size_t)plen) < 0) return -1;
    bn_from_be(d, c->nl, k->ec_priv, (size_t)c->bytes);
    pt_mul(c, &S, d, &Q);
    pk_wipe(d, sizeof d);
    if (pt_to_affine(c, &S, (u8*)out, NULL) < 0) return -1;
    return c->bytes;
}

/* ── JWK support ──────────────────────────────────────────────────────────
 * RSA key from raw big-endian parts: lens[0..7] = n, e, d, p, q, dp, dq, qi,
 * concatenated in buf. With only n and e (the rest 0-length) the key is
 * public. A private key is checked by a private operation that re-applies the
 * public exponent (rsa_private_op verifies its own result). */
void* trc_pk_rsa_from_parts(const char* buf, const int64_t* lens) {
    const u8* part[8]; size_t ln[8]; size_t off = 0; int i, hl; pk_key* k; rsa_key* r;
    for (i = 0; i < 8; i++) {
        part[i] = (const u8*)buf + off; ln[i] = lens[i] < 0 ? 0 : (size_t)lens[i]; off += ln[i];
        while (ln[i] > 0 && part[i][0] == 0) { part[i]++; ln[i]--; }
    }
    k = (pk_key*)calloc(1, sizeof(pk_key));
    if (!k) return NULL;
    r = &k->rsa;
    if (rsa_set_public(r, part[0], ln[0], part[1], ln[1]) < 0) { free(k); return NULL; }
    k->kind = 1;
    if (ln[2] == 0) return k;
    for (i = 3; i < 8; i++) if (ln[i] == 0) { pk_wipe(k, sizeof *k); free(k); return NULL; }
    if (ln[2] > 4 * (size_t)r->n_limbs) { pk_wipe(k, sizeof *k); free(k); return NULL; }
    bn_from_be(r->d, r->n_limbs, part[2], ln[2]); r->has_d = 1;
    hl = (int)(((ln[3] > ln[4] ? ln[3] : ln[4]) + 3) / 4);
    if (hl > MAXL - 2 || ln[5] > 4 * (size_t)hl || ln[6] > 4 * (size_t)hl || ln[7] > 4 * (size_t)hl) { pk_wipe(k, sizeof *k); free(k); return NULL; }
    r->half_limbs = hl;
    bn_from_be(r->p, hl, part[3], ln[3]); bn_from_be(r->q, hl, part[4], ln[4]);
    bn_from_be(r->dp, hl, part[5], ln[5]); bn_from_be(r->dq, hl, part[6], ln[6]);
    bn_from_be(r->qinv, hl, part[7], ln[7]);
    if (!(r->p[0] & 1) || !(r->q[0] & 1)) { pk_wipe(k, sizeof *k); free(k); return NULL; }
    r->has_priv = 1; k->has_priv = 1;
    {
        u8 in[4 * MAXL], out[4 * MAXL]; size_t klen = (size_t)(r->bits + 7) / 8;
        memset(in, 0, klen); in[klen - 1] = 2; in[klen / 2] = 0x5a;
        if (rsa_private_op(r, in, out) < 0) { pk_wipe(k, sizeof *k); free(k); return NULL; }
    }
    return k;
}
/* One RSA component as minimal big-endian bytes: 0 n, 1 e, 2 d, 3 p, 4 q,
 * 5 dp, 6 dq, 7 qi. Returns the length, or -1 if absent / not RSA. */
int64_t trc_pk_rsa_part(void* h, int64_t idx, char* out, int64_t cap) {
    pk_key* k = (pk_key*)h; rsa_key* r; const u32* src; int limbs; u8 tmp[4 * MAXL]; size_t n, s0 = 0;
    if (!k || k->kind != 1) return -1;
    r = &k->rsa;
    switch (idx) {
        case 0: src = r->n; limbs = r->n_limbs; break;
        case 1: src = r->e; limbs = r->n_limbs; break;
        case 2: if (!r->has_d) return -1; src = r->d; limbs = r->n_limbs; break;
        case 3: case 4: case 5: case 6: case 7:
            if (!r->has_priv) return -1;
            src = idx == 3 ? r->p : idx == 4 ? r->q : idx == 5 ? r->dp : idx == 6 ? r->dq : r->qinv;
            limbs = r->half_limbs; break;
        default: return -1;
    }
    n = (size_t)limbs * 4;
    bn_to_be(tmp, n, src, limbs);
    while (s0 + 1 < n && tmp[s0] == 0) s0++;
    if ((int64_t)(n - s0) > cap) return -1;
    memcpy(out, tmp + s0, n - s0);
    pk_wipe(tmp, sizeof tmp);
    return (int64_t)(n - s0);
}
/* Whether an RSA private key also carries d (needed for JWK export). */
int64_t trc_pk_rsa_has_d(void* h) { pk_key* k = (pk_key*)h; return (k && k->kind == 1 && k->rsa.has_d) ? 1 : 0; }

/* PEM: find the n-th block whose label equals `label` (or any label when
 * label is empty) and base64-decode it into out. Returns the DER length,
 * -1 if there is no such block, -2 if out is too small, -3 if the block is
 * malformed (or encrypted). The label found
 * is copied into label_out (64 bytes). */
int64_t trc_pem_decode(const char* pem, int64_t n, int64_t index, const char* label, int64_t llen, char* out, int64_t cap, char* label_out) {
    const char* p = pem; const char* end = pem + n; int64_t seen = 0;
    while (p < end) {
        const char *b = NULL, *q;
        for (q = p; q + 11 <= end; q++) if (memcmp(q, "-----BEGIN ", 11) == 0) { b = q; break; }
        if (!b) return -1;
        {
            const char *ls = b + 11, *le = ls, *body, *bend = NULL, *r; char* buf; int64_t blen = 0, got;
            while (le < end && *le != '-') le++;
            if (le + 5 > end || memcmp(le, "-----", 5) != 0) return -1;
            body = le + 5;
            for (r = body; r + 9 <= end; r++) if (memcmp(r, "-----END ", 9) == 0) { bend = r; break; }
            if (!bend) return -1;
            p = bend + 9;
            if (llen > 0 && ((int64_t)(le - ls) != llen || memcmp(ls, label, (size_t)llen) != 0)) continue;
            if (seen++ != index) continue;
            if (label_out) { size_t l = (size_t)(le - ls); if (l > 63) l = 63; memcpy(label_out, ls, l); label_out[l] = 0; }
            buf = (char*)malloc((size_t)(bend - body) + 1);
            if (!buf) return -1;
            for (r = body; r < bend; r++) {
                char ch = *r;
                if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t') continue;
                if (ch == ':' ) { /* RFC 1421 headers (Proc-Type etc.): encrypted PEM unsupported */ free(buf); return -3; }
                buf[blen++] = ch;
            }
            if ((blen / 4) * 3 > cap + 3) { free(buf); return -2; }
            {
                char* tmp = (char*)malloc((size_t)(blen / 4) * 3 + 4);
                if (!tmp) { free(buf); return -1; }
                got = trc_b64_decode(0, buf, blen, tmp);
                free(buf);
                if (got < 0) { free(tmp); return -3; }
                if (got > cap) { free(tmp); return -2; }
                memcpy(out, tmp, (size_t)got); free(tmp);
            }
            return got;
        }
    }
    return -1;
}

/* ═══════════════════════════ X.509 ═══════════════════════════════════════ */
#define X_MAX_SAN 64
typedef struct {
    u8* raw; size_t rawlen;                 /* owned copy of the DER */
    der tbs;                                /* whole TBSCertificate TLV */
    int version;
    der serial, issuer, subject, spki;      /* issuer/subject: Name TLV contents incl. header */
    int64_t not_before, not_after;
    int sig_kind, sig_hash, pss_salt;       /* sig_kind: 1 RSA v1.5, 2 RSA-PSS, 3 ECDSA, 4 Ed25519 */
    der sig;                                /* signature bits (unused-bit byte stripped) */
    int is_ca, path_len;                    /* path_len -1 = unlimited */
    int has_bc;
    int key_usage;                          /* -1 absent; bit 0 = digitalSignature ... bit 5 = keyCertSign */
    int eku;                                /* -1 absent; bit0 serverAuth, bit1 clientAuth, bit2 any, bit3 codeSigning, bit4 email, bit5 timeStamping, bit6 OCSP */
    int n_dns, n_ip;
    der dns[X_MAX_SAN], ip[X_MAX_SAN];
    der ski, aki;
    int nc_present; int n_permit, n_exclude;
    der permit[16], exclude[16];
    int unknown_critical;
    pk_key key; int key_ok;
} x509;

static int64_t days_from_civil(int64_t y, unsigned m, unsigned d) {
    int64_t era; unsigned yoe, doy, doe;
    y -= m <= 2;
    era = (y >= 0 ? y : y - 399) / 400;
    yoe = (unsigned)(y - era * 400);
    doy = (153 * (m + (m > 2 ? (unsigned)-3 : 9)) + 2) / 5 + d - 1;
    doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + (int64_t)doe - 719468;
}
static int parse_time(int tag, const der* v, int64_t* out) {
    int i, y, mo, d, h, mi, s; const u8* p = v->p;
    char buf[16];
    if (tag == 0x17) { if (v->n != 13 || p[12] != 'Z') return -1; }
    else if (tag == 0x18) { if (v->n != 15 || p[14] != 'Z') return -1; }
    else return -1;
    for (i = 0; i < (int)v->n - 1; i++) { if (p[i] < '0' || p[i] > '9') return -1; buf[i] = (char)p[i]; }
#define D2(k) ((buf[k] - '0') * 10 + (buf[(k) + 1] - '0'))
    if (tag == 0x17) { y = D2(0); y += y >= 50 ? 1900 : 2000; i = 2; }
    else { y = D2(0) * 100 + D2(2); i = 4; }
    mo = D2(i); d = D2(i + 2); h = D2(i + 4); mi = D2(i + 6); s = D2(i + 8);
#undef D2
    if (mo < 1 || mo > 12 || d < 1 || d > 31 || h > 23 || mi > 59 || s > 60) return -1;
    *out = days_from_civil(y, (unsigned)mo, (unsigned)d) * 86400 + h * 3600 + mi * 60 + s;
    return 0;
}
static int hash_for_oid(const der* o) {
    if (oid_eq(o, OID_SHA256, sizeof OID_SHA256)) return 256;
    if (oid_eq(o, OID_SHA384, sizeof OID_SHA384)) return 384;
    if (oid_eq(o, OID_SHA512, sizeof OID_SHA512)) return 512;
    if (oid_eq(o, OID_SHA1, sizeof OID_SHA1)) return 1;
    return 0;
}
/* AlgorithmIdentifier -> (kind, hash, pss salt) */
static int parse_sig_alg(der alg, int* kind, int* hash, int* salt) {
    der oid; int t;
    if (der_expect(&alg, 0x06, &oid) < 0) return -1;
    *salt = -1;
    if (oid_eq(&oid, OID_SHA256_RSA, sizeof OID_SHA256_RSA)) { *kind = 1; *hash = 256; return 0; }
    if (oid_eq(&oid, OID_SHA384_RSA, sizeof OID_SHA384_RSA)) { *kind = 1; *hash = 384; return 0; }
    if (oid_eq(&oid, OID_SHA512_RSA, sizeof OID_SHA512_RSA)) { *kind = 1; *hash = 512; return 0; }
    if (oid_eq(&oid, OID_SHA1_RSA, sizeof OID_SHA1_RSA)) { *kind = 1; *hash = 1; return 0; }
    if (oid_eq(&oid, OID_ECDSA_SHA256, sizeof OID_ECDSA_SHA256)) { *kind = 3; *hash = 256; return 0; }
    if (oid_eq(&oid, OID_ECDSA_SHA384, sizeof OID_ECDSA_SHA384)) { *kind = 3; *hash = 384; return 0; }
    if (oid_eq(&oid, OID_ECDSA_SHA512, sizeof OID_ECDSA_SHA512)) { *kind = 3; *hash = 512; return 0; }
    if (oid_eq(&oid, OID_ECDSA_SHA1, sizeof OID_ECDSA_SHA1)) { *kind = 3; *hash = 1; return 0; }
    if (oid_eq(&oid, OID_ED25519, sizeof OID_ED25519)) { *kind = 4; *hash = 0; return 0; }
    if (oid_eq(&oid, OID_RSA_PSS, sizeof OID_RSA_PSS)) {
        /* RSASSA-PSS-params: [0] hashAlg, [1] mgf, [2] saltLength, [3] trailer */
        der params, f; int h = 1, mgfh = 1, sl = 20;
        *kind = 2;
        if (der_expect(&alg, 0x30, &params) < 0) return -1;
        while (params.n) {
            if (der_next(&params, &t, &f) < 0) return -1;
            if (t == 0xa0) { der a, o; if (der_expect(&f, 0x30, &a) < 0 || der_expect(&a, 0x06, &o) < 0) return -1; h = hash_for_oid(&o); }
            else if (t == 0xa1) {
                der a, o, inner, o2;
                if (der_expect(&f, 0x30, &a) < 0 || der_expect(&a, 0x06, &o) < 0) return -1;
                if (!oid_eq(&o, OID_MGF1, sizeof OID_MGF1)) return -1;
                if (der_expect(&a, 0x30, &inner) < 0 || der_expect(&inner, 0x06, &o2) < 0) return -1;
                mgfh = hash_for_oid(&o2);
            } else if (t == 0xa2) {
                der iv; if (der_uint(&f, &iv) < 0 || iv.n > 2) return -1;
                sl = iv.n == 1 ? iv.p[0] : (iv.p[0] << 8 | iv.p[1]);
            }
        }
        if (!h || h != mgfh) return -1;
        *hash = h; *salt = sl;
        return 0;
    }
    return -2;
}
static int bitstring_bits(const der* b) {
    /* KeyUsage: bit i of the named bit list -> our bit i */
    int v = 0, i;
    if (b->n < 1) return 0;
    for (i = 1; i < (int)b->n && i < 3; i++) {
        int k; for (k = 0; k < 8; k++) if (b->p[i] & (0x80 >> k)) v |= 1 << ((i - 1) * 8 + k);
    }
    return v;
}
static void x_free(x509* x) { if (x) { free(x->raw); free(x); } }

static x509* x_parse(const u8* in, size_t inlen, int* err) {
    x509* x = (x509*)calloc(1, sizeof(x509)); der d, cert, tbs, f, alg, bits, tmp; int t;
    static const u8 ID_CE[] = {0x55, 0x1d};
    *err = -1;
    if (!x) return NULL;
    x->raw = (u8*)malloc(inlen ? inlen : 1);
    if (!x->raw) { free(x); return NULL; }
    memcpy(x->raw, in, inlen); x->rawlen = inlen;
    d.p = x->raw; d.n = inlen;
    x->key_usage = -1; x->eku = -1; x->path_len = -1;
    if (der_expect(&d, 0x30, &cert) < 0 || d.n != 0) goto bad;
    x->tbs.p = cert.p;
    if (der_expect(&cert, 0x30, &tbs) < 0) goto bad;
    x->tbs.n = (size_t)(tbs.p + tbs.n - x->tbs.p);
    if (der_expect(&cert, 0x30, &alg) < 0) goto bad;
    if (der_expect(&cert, 0x03, &bits) < 0 || bits.n < 1 || bits.p[0] != 0 || cert.n != 0) goto bad;
    x->sig.p = bits.p + 1; x->sig.n = bits.n - 1;
    if (parse_sig_alg(alg, &x->sig_kind, &x->sig_hash, &x->pss_salt) == -1) goto bad;

    /* TBSCertificate */
    x->version = 1;
    if (der_peek(&tbs) == 0xa0) {
        der v, vi;
        if (der_next(&tbs, &t, &v) < 0 || der_uint(&v, &vi) < 0 || vi.n != 1) goto bad;
        x->version = vi.p[0] + 1;
    }
    if (der_expect(&tbs, 0x02, &x->serial) < 0) goto bad;
    if (der_expect(&tbs, 0x30, &tmp) < 0) goto bad;          /* inner signature alg */
    {
        const u8* s0 = tbs.p;
        if (der_expect(&tbs, 0x30, &tmp) < 0) goto bad;
        x->issuer.p = s0; x->issuer.n = (size_t)(tbs.p - s0);
    }
    {
        der val, tv; int tt;
        if (der_expect(&tbs, 0x30, &val) < 0) goto bad;
        if (der_next(&val, &tt, &tv) < 0 || parse_time(tt, &tv, &x->not_before) < 0) goto bad;
        if (der_next(&val, &tt, &tv) < 0 || parse_time(tt, &tv, &x->not_after) < 0) goto bad;
    }
    {
        const u8* s0 = tbs.p;
        if (der_expect(&tbs, 0x30, &tmp) < 0) goto bad;
        x->subject.p = s0; x->subject.n = (size_t)(tbs.p - s0);
    }
    {
        const u8* s0 = tbs.p;
        if (der_expect(&tbs, 0x30, &tmp) < 0) goto bad;
        x->spki.p = s0; x->spki.n = (size_t)(tbs.p - s0);
        x->key_ok = spki_parse(&x->key, x->spki.p, x->spki.n) == 0;
    }
    while (tbs.n) {
        if (der_next(&tbs, &t, &f) < 0) goto bad;
        if (t != 0xa3) continue;      /* issuer/subject unique IDs */
        {
            der exts, ext;
            if (der_expect(&f, 0x30, &exts) < 0) goto bad;
            while (exts.n) {
                der oid, val, crit; int critical = 0;
                if (der_expect(&exts, 0x30, &ext) < 0 || der_expect(&ext, 0x06, &oid) < 0) goto bad;
                if (der_peek(&ext) == 0x01) { if (der_expect(&ext, 0x01, &crit) < 0 || crit.n != 1) goto bad; critical = crit.p[0] != 0; }
                if (der_expect(&ext, 0x04, &val) < 0) goto bad;
                if (oid.n == 3 && memcmp(oid.p, ID_CE, 2) == 0) {
                    switch (oid.p[2]) {
                    case 0x11: { /* subjectAltName */
                        der names, gn;
                        if (der_expect(&val, 0x30, &names) < 0) goto bad;
                        while (names.n) {
                            if (der_next(&names, &t, &gn) < 0) goto bad;
                            if (t == 0x82 && x->n_dns < X_MAX_SAN) x->dns[x->n_dns++] = gn;
                            else if (t == 0x87 && x->n_ip < X_MAX_SAN) x->ip[x->n_ip++] = gn;
                        }
                        break;
                    }
                    case 0x13: { /* basicConstraints */
                        der bc, b;
                        x->has_bc = 1;
                        if (der_expect(&val, 0x30, &bc) < 0) goto bad;
                        if (der_peek(&bc) == 0x01) { if (der_expect(&bc, 0x01, &b) < 0 || b.n != 1) goto bad; x->is_ca = b.p[0] != 0; }
                        if (der_peek(&bc) == 0x02) { if (der_uint(&bc, &b) < 0 || b.n > 1) goto bad; x->path_len = b.p[0]; }
                        break;
                    }
                    case 0x0f: { der b; if (der_expect(&val, 0x03, &b) < 0) goto bad; x->key_usage = bitstring_bits(&b); break; }
                    case 0x25: { /* extKeyUsage */
                        der s, o; static const u8 KP[] = {0x2b,0x06,0x01,0x05,0x05,0x07,0x03};
                        static const u8 ANY[] = {0x55,0x1d,0x25,0x00};
                        x->eku = 0;
                        if (der_expect(&val, 0x30, &s) < 0) goto bad;
                        while (s.n) {
                            if (der_expect(&s, 0x06, &o) < 0) goto bad;
                            if (o.n == 8 && memcmp(o.p, KP, 7) == 0) {
                                switch (o.p[7]) { case 1: x->eku |= 1; break; case 2: x->eku |= 2; break; case 3: x->eku |= 8; break;
                                                  case 4: x->eku |= 16; break; case 8: x->eku |= 32; break; case 9: x->eku |= 64; break; }
                            } else if (oid_eq(&o, ANY, sizeof ANY)) x->eku |= 4;
                        }
                        break;
                    }
                    case 0x0e: { der k; if (der_expect(&val, 0x04, &k) < 0) goto bad; x->ski = k; break; }
                    case 0x23: { der s, k; if (der_expect(&val, 0x30, &s) < 0) goto bad;
                                 if (der_peek(&s) == 0x80 && der_next(&s, &t, &k) == 0) x->aki = k;
                                 break; }
                    case 0x1e: { /* nameConstraints */
                        der nc, sub, gs, base;
                        x->nc_present = 1;
                        if (der_expect(&val, 0x30, &nc) < 0) goto bad;
                        while (nc.n) {
                            int which;
                            if (der_next(&nc, &which, &sub) < 0) goto bad;
                            while (sub.n) {
                                if (der_expect(&sub, 0x30, &gs) < 0 || der_next(&gs, &t, &base) < 0) goto bad;
                                if (t != 0x82) { if (critical) x->unknown_critical = 1; continue; }   /* only dNSName understood */
                                if (which == 0xa0 && x->n_permit < 16) x->permit[x->n_permit++] = base;
                                else if (which == 0xa1 && x->n_exclude < 16) x->exclude[x->n_exclude++] = base;
                            }
                        }
                        break;
                    }
                    case 0x20: case 0x24: case 0x36: case 0x1f: case 0x2e: break;   /* policies, CRL DPs: accepted */
                    default: if (critical) x->unknown_critical = 1;
                    }
                } else if (critical) {
                    x->unknown_critical = 1;
                }
            }
        }
    }
    *err = 0;
    return x;
bad:
    x_free(x);
    return NULL;
}

/* Does `issuer`'s key verify `child`'s signature? */
static int x_sig_ok(const x509* child, const pk_key* key) {
    u8 h[64]; int hl;
    if (child->sig_kind == 4 || key->kind == 4) return -1;          /* Ed25519 certs unsupported here */
    if (child->sig_kind == 1 || child->sig_kind == 2) { if (key->kind != 1) return -1; }
    else if (child->sig_kind == 3) { if (key->kind != 2 && key->kind != 3) return -1; }
    else return -1;
    if (child->sig_hash == 1) return -1;                             /* SHA-1 signatures refused */
    hl = md_len(child->sig_hash);
    if (hl < 0) return -1;
    trc_digest(child->sig_hash, (const char*)child->tbs.p, (int64_t)child->tbs.n, (char*)h);
    if (child->sig_kind == 2)
        return rsa_verify_hash(&key->rsa, 1, child->sig_hash, h, child->sig.p, child->sig.n, child->pss_salt);
    return pk_verify_hash((pk_key*)key, 0, child->sig_hash, h, (size_t)hl, child->sig.p, child->sig.n, 0);
}

/* ── hostname matching (RFC 6125) ── */
static int lc(int c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }
static int ieq(const u8* a, size_t an, const u8* b, size_t bn) {
    size_t i; if (an != bn) return 0;
    for (i = 0; i < an; i++) if (lc(a[i]) != lc(b[i])) return 0;
    return 1;
}
static int dns_match(const u8* pat, size_t pn, const char* host, size_t hn) {
    while (hn && host[hn - 1] == '.') hn--;
    while (pn && pat[pn - 1] == '.') pn--;
    if (pn >= 2 && pat[0] == '*' && pat[1] == '.') {
        const char* dot = memchr(host, '.', hn);
        size_t rest;
        if (!dot || dot == host) return 0;
        rest = hn - (size_t)(dot - host);
        /* wildcard must cover exactly one label and leave at least two labels */
        if (!memchr(pat + 2, '.', pn - 2)) return 0;
        return ieq(pat + 1, pn - 1, (const u8*)dot, rest);
    }
    return ieq(pat, pn, (const u8*)host, hn);
}
static int parse_ipv4(const char* s, size_t n, u8* out) {
    int part = 0, val = -1; size_t i;
    for (i = 0; i <= n; i++) {
        if (i == n || s[i] == '.') {
            if (val < 0 || part > 3) return 0;
            out[part++] = (u8)val; val = -1;
        } else if (s[i] >= '0' && s[i] <= '9') {
            val = (val < 0 ? 0 : val * 10) + (s[i] - '0');
            if (val > 255) return 0;
        } else return 0;
    }
    return part == 4;
}
static int parse_ipv6(const char* s, size_t n, u8* out) {
    u8 tmp[16]; int pos = 0, gap = -1; size_t i = 0;
    if (n >= 2 && s[0] == '[' && s[n - 1] == ']') { s++; n -= 2; }
    if (n >= 2 && s[0] == ':' && s[1] == ':') { gap = 0; i = 2; }
    while (i < n && pos < 16) {
        unsigned v = 0; int digits = 0;
        while (i < n && digits < 5) {
            int c = s[i], h;
            if (c >= '0' && c <= '9') h = c - '0'; else if (lc(c) >= 'a' && lc(c) <= 'f') h = lc(c) - 'a' + 10; else break;
            v = v * 16 + (unsigned)h; digits++; i++;
        }
        if (digits == 0 || digits > 4) return 0;
        tmp[pos++] = (u8)(v >> 8); tmp[pos++] = (u8)v;
        if (i == n) break;
        if (s[i] != ':') return 0;
        i++;
        if (i < n && s[i] == ':') { if (gap >= 0) return 0; gap = pos; i++; if (i == n) break; }
    }
    if (i != n) return 0;
    if (gap < 0) { if (pos != 16) return 0; memcpy(out, tmp, 16); return 1; }
    memset(out, 0, 16);
    memcpy(out, tmp, (size_t)gap);
    memcpy(out + 16 - (pos - gap), tmp + gap, (size_t)(pos - gap));
    return 1;
}
static int x_host_ok(const x509* x, const char* host, size_t hn) {
    u8 ip[16]; int i;
    if (parse_ipv4(host, hn, ip)) {
        for (i = 0; i < x->n_ip; i++) if (x->ip[i].n == 4 && memcmp(x->ip[i].p, ip, 4) == 0) return 1;
        return 0;
    }
    if (memchr(host, ':', hn) && parse_ipv6(host, hn, ip)) {
        for (i = 0; i < x->n_ip; i++) if (x->ip[i].n == 16 && memcmp(x->ip[i].p, ip, 16) == 0) return 1;
        return 0;
    }
    for (i = 0; i < x->n_dns; i++) if (dns_match(x->dns[i].p, x->dns[i].n, host, hn)) return 1;
    return 0;
}
/* name constraint: host within subtree `base` (dNSName) */
static int nc_within(const der* base, const u8* name, size_t nn) {
    size_t bn = base->n; const u8* b = base->p;
    if (bn && b[0] == '.') { b++; bn--; }
    if (bn == 0) return 1;
    if (nn == bn) return ieq(name, nn, b, bn);
    if (nn > bn) return name[nn - bn - 1] == '.' && ieq(name + nn - bn, bn, b, bn);
    return 0;
}
static int x_nc_ok(const x509* ca, const x509* leaf) {
    int i, j;
    if (!ca->nc_present) return 1;
    for (i = 0; i < leaf->n_dns; i++) {
        const der* nm = &leaf->dns[i]; const u8* p = nm->p; size_t n = nm->n;
        if (n >= 2 && p[0] == '*' && p[1] == '.') { p += 2; n -= 2; }
        for (j = 0; j < ca->n_exclude; j++) if (nc_within(&ca->exclude[j], p, n)) return 0;
        if (ca->n_permit > 0) {
            int ok = 0;
            for (j = 0; j < ca->n_permit; j++) if (nc_within(&ca->permit[j], p, n)) ok = 1;
            if (!ok) return 0;
        }
    }
    return 1;
}

/* ── trust store ── */
typedef struct { x509** certs; int n, cap; } x_store;

void* trc_x509_store_new(void) { return calloc(1, sizeof(x_store)); }
void trc_x509_store_free(void* h) {
    x_store* s = (x_store*)h; int i;
    if (!s) return;
    for (i = 0; i < s->n; i++) x_free(s->certs[i]);
    free(s->certs); free(s);
}
int64_t trc_x509_store_count(void* h) { return h ? ((x_store*)h)->n : 0; }
/* Add one DER certificate. Returns 0, or -1 if it does not parse. */
int64_t trc_x509_store_add_der(void* h, const char* d, int64_t n) {
    x_store* s = (x_store*)h; int err; x509* x;
    if (!s) return -1;
    x = x_parse((const u8*)d, (size_t)n, &err);
    if (!x) return -1;
    if (s->n == s->cap) {
        int nc = s->cap ? s->cap * 2 : 64; x509** nx = (x509**)realloc(s->certs, sizeof(x509*) * (size_t)nc);
        if (!nx) { x_free(x); return -1; }
        s->certs = nx; s->cap = nc;
    }
    s->certs[s->n++] = x;
    return 0;
}
/* Add every CERTIFICATE block of a PEM bundle; returns how many parsed. */
int64_t trc_x509_store_add_pem(void* h, const char* pem, int64_t n) {
    int64_t idx = 0, added = 0; char* buf = (char*)malloc(65536);
    if (!buf) return 0;
    for (;;) {
        int64_t len = trc_pem_decode(pem, n, idx++, "CERTIFICATE", 11, buf, 65536, NULL);
        if (len == -1) break;
        if (len < 0) continue;
        if (trc_x509_store_add_der(h, buf, len) == 0) added++;
    }
    free(buf);
    return added;
}
/* Load the platform's CA bundle (first readable well-known file, or the
 * Windows ROOT store). Returns how many certificates were added. */
#if defined(_WIN32)
#include <windows.h>
typedef void* (WINAPI *open_store_fn)(void*, const char*);
typedef const void* (WINAPI *enum_fn)(void*, const void*);
typedef int (WINAPI *close_fn)(void*, unsigned long);
typedef struct { unsigned long enc; unsigned char* der; unsigned long len; } win_cert_ctx_head;
#endif
int64_t trc_x509_store_load_system(void* h) {
    int64_t total = 0;
#if defined(_WIN32)
    HMODULE lib = LoadLibraryA("crypt32.dll");
    if (lib) {
        open_store_fn op = (open_store_fn)(void*)GetProcAddress(lib, "CertOpenSystemStoreA");
        enum_fn en = (enum_fn)(void*)GetProcAddress(lib, "CertEnumCertificatesInStore");
        close_fn cl = (close_fn)(void*)GetProcAddress(lib, "CertCloseStore");
        if (op && en && cl) {
            void* st = op(NULL, "ROOT");
            if (st) {
                const void* ctx = NULL;
                while ((ctx = en(st, ctx)) != NULL) {
                    const win_cert_ctx_head* c = (const win_cert_ctx_head*)ctx;
                    if (trc_x509_store_add_der(h, (const char*)c->der, (int64_t)c->len) == 0) total++;
                }
                cl(st, 0);
            }
        }
    }
    return total;
#else
    static const char* paths[] = {
        "/etc/ssl/certs/ca-certificates.crt", "/etc/pki/tls/certs/ca-bundle.crt",
        "/etc/ssl/ca-bundle.pem", "/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem",
        "/etc/ssl/cert.pem", "/usr/local/etc/openssl/cert.pem", "/usr/local/share/certs/ca-root-nss.crt",
        "/opt/homebrew/etc/openssl@3/cert.pem", NULL };
    int i;
    const char* env = getenv("SSL_CERT_FILE");
    for (i = -1; i < 0 || paths[i]; i++) {
        const char* path = i < 0 ? env : paths[i];
        FILE* f; long sz; char* buf;
        if (!path) continue;
        f = fopen(path, "rb");
        if (!f) continue;
        fseek(f, 0, SEEK_END); sz = ftell(f); fseek(f, 0, SEEK_SET);
        if (sz <= 0) { fclose(f); continue; }
        buf = (char*)malloc((size_t)sz);
        if (!buf) { fclose(f); continue; }
        if (fread(buf, 1, (size_t)sz, f) == (size_t)sz) total = trc_x509_store_add_pem(h, buf, sz);
        fclose(f); free(buf);
        if (total > 0) break;
    }
    return total;
#endif
}

/* Process-wide system store, loaded on first use and never freed. Safe to
 * call from several threads: losers of the race wait for the winner. */
void* trc_x509_store_system_shared(void) {
    static void* shared = NULL; static volatile int state = 0;   /* 0 none, 1 loading, 2 ready */
    int expect = 0;
    if (__atomic_load_n(&state, __ATOMIC_ACQUIRE) == 2) return shared;
    if (__atomic_compare_exchange_n(&state, &expect, 1, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) {
        void* s = trc_x509_store_new();
        if (s) trc_x509_store_load_system(s);
        shared = s;
        __atomic_store_n(&state, 2, __ATOMIC_RELEASE);
        return shared;
    }
    while (__atomic_load_n(&state, __ATOMIC_ACQUIRE) != 2) { }
    return shared;
}

/* ── single-certificate handle API (for std.crypto.x509) ── */
void* trc_x509_parse(const char* d, int64_t n) { int err; return x_parse((const u8*)d, (size_t)n, &err); }
void trc_x509_free(void* h) { x_free((x509*)h); }

static size_t put_esc(char* out, size_t cap, size_t k, const u8* s, size_t n) {
    size_t i;
    for (i = 0; i < n && k + 4 < cap; i++) {
        u8 c = s[i];
        if (c == ',' || c == '+' || c == '"' || c == '\\' || c == '<' || c == '>' || c == ';' || c == '=') { out[k++] = '\\'; out[k++] = (char)c; }
        else if (c < 0x20 || c == 0x7f) { static const char hx[] = "0123456789ABCDEF"; out[k++] = '\\'; out[k++] = hx[c >> 4]; out[k++] = hx[c & 15]; }
        else out[k++] = (char)c;
    }
    return k;
}
/* Distinguished name -> "CN=..., O=..., C=..." (RDNs in certificate order). */
static int64_t name_to_str(der name, char* out, int64_t cap) {
    der seq, set, atv, oid, val; int t; size_t k = 0; int first = 1;
    if (der_expect(&name, 0x30, &seq) < 0) return -1;
    while (seq.n) {
        if (der_expect(&seq, 0x31, &set) < 0) return -1;
        while (set.n) {
            const char* key = NULL; char oidbuf[48];
            if (der_expect(&set, 0x30, &atv) < 0 || der_expect(&atv, 0x06, &oid) < 0 || der_next(&atv, &t, &val) < 0) return -1;
            if (oid.n == 3 && oid.p[0] == 0x55 && oid.p[1] == 0x04) {
                switch (oid.p[2]) {
                    case 3: key = "CN"; break; case 4: key = "SN"; break; case 5: key = "serialNumber"; break;
                    case 6: key = "C"; break; case 7: key = "L"; break; case 8: key = "ST"; break;
                    case 9: key = "street"; break; case 10: key = "O"; break; case 11: key = "OU"; break;
                    case 12: key = "title"; break; case 42: key = "GN"; break; case 97: key = "organizationIdentifier"; break;
                }
            } else if (oid.n == 9 && memcmp(oid.p, "\x2a\x86\x48\x86\xf7\x0d\x01\x09\x01", 9) == 0) key = "emailAddress";
            else if (oid.n == 10 && memcmp(oid.p, "\x09\x92\x26\x89\x93\xf2\x2c\x64\x01\x19", 10) == 0) key = "DC";
            else if (oid.n == 10 && memcmp(oid.p, "\x09\x92\x26\x89\x93\xf2\x2c\x64\x01\x01", 10) == 0) key = "UID";
            if (!key) { snprintf(oidbuf, sizeof oidbuf, "OID(%u bytes)", (unsigned)oid.n); key = oidbuf; }
            if (!first && k + 2 < (size_t)cap) { out[k++] = ','; out[k++] = ' '; }
            first = 0;
            { size_t kl = strlen(key); if (k + kl + 1 < (size_t)cap) { memcpy(out + k, key, kl); k += kl; out[k++] = '='; } }
            k = put_esc(out, (size_t)cap, k, val.p, val.n);
        }
    }
    return (int64_t)k;
}

/* field: 1 subject, 2 issuer, 3 serial (hex), 4 SAN DNS list (\n), 5 SAN IP list (\n),
 * 6 SKI hex, 7 AKI hex, 8 signature algorithm name, 9 public key algorithm name. */
int64_t trc_x509_text(void* h, int64_t field, char* out, int64_t cap) {
    x509* x = (x509*)h; size_t k = 0; int i; static const char hx[] = "0123456789abcdef";
    if (!x) return -1;
    switch (field) {
    case 1: return name_to_str(x->subject, out, cap);
    case 2: return name_to_str(x->issuer, out, cap);
    case 3: case 6: case 7: {
        const der* v = field == 3 ? &x->serial : field == 6 ? &x->ski : &x->aki; size_t j;
        for (j = 0; j < v->n && k + 3 < (size_t)cap; j++) { out[k++] = hx[v->p[j] >> 4]; out[k++] = hx[v->p[j] & 15]; }
        return (int64_t)k;
    }
    case 4:
        for (i = 0; i < x->n_dns; i++) {
            if (k + x->dns[i].n + 2 >= (size_t)cap) break;
            if (i) out[k++] = '\n';
            memcpy(out + k, x->dns[i].p, x->dns[i].n); k += x->dns[i].n;
        }
        return (int64_t)k;
    case 5:
        for (i = 0; i < x->n_ip; i++) {
            char b[48]; int l = 0;
            if (x->ip[i].n == 4) l = snprintf(b, sizeof b, "%u.%u.%u.%u", x->ip[i].p[0], x->ip[i].p[1], x->ip[i].p[2], x->ip[i].p[3]);
            else if (x->ip[i].n == 16) { int g; for (g = 0; g < 8; g++) l += snprintf(b + l, sizeof b - (size_t)l, g ? ":%x" : "%x", (x->ip[i].p[2 * g] << 8) | x->ip[i].p[2 * g + 1]); }
            if (k + (size_t)l + 2 >= (size_t)cap) break;
            if (i) out[k++] = '\n';
            memcpy(out + k, b, (size_t)l); k += (size_t)l;
        }
        return (int64_t)k;
    case 8: {
        const char* s = "unknown";
        if (x->sig_kind == 1) s = x->sig_hash == 256 ? "sha256WithRSAEncryption" : x->sig_hash == 384 ? "sha384WithRSAEncryption" : x->sig_hash == 512 ? "sha512WithRSAEncryption" : "sha1WithRSAEncryption";
        else if (x->sig_kind == 2) s = "rsassaPss";
        else if (x->sig_kind == 3) s = x->sig_hash == 256 ? "ecdsa-with-SHA256" : x->sig_hash == 384 ? "ecdsa-with-SHA384" : x->sig_hash == 512 ? "ecdsa-with-SHA512" : "ecdsa-with-SHA1";
        else if (x->sig_kind == 4) s = "Ed25519";
        k = strlen(s); if ((int64_t)k > cap) return -1; memcpy(out, s, k); return (int64_t)k;
    }
    case 9: {
        const char* s = !x->key_ok ? "unsupported" : x->key.kind == 1 ? "RSA" : x->key.kind == 2 ? "EC P-256" : x->key.kind == 3 ? "EC P-384" : "Ed25519";
        k = strlen(s); if ((int64_t)k > cap) return -1; memcpy(out, s, k); return (int64_t)k;
    }
    }
    return -1;
}
/* field: 1 version, 2 not_before, 3 not_after, 4 is_ca, 5 path_len, 6 key_usage, 7 eku,
 * 8 key bits, 9 has unknown critical extension, 10 has basicConstraints */
int64_t trc_x509_int(void* h, int64_t field) {
    x509* x = (x509*)h;
    if (!x) return -1;
    switch (field) {
        case 1: return x->version; case 2: return x->not_before; case 3: return x->not_after;
        case 4: return x->is_ca; case 5: return x->path_len; case 6: return x->key_usage; case 7: return x->eku;
        case 8: return x->key_ok ? trc_pk_bits(&x->key) : 0; case 9: return x->unknown_critical; case 10: return x->has_bc;
    }
    return -1;
}
/* field: 1 raw DER, 2 TBS, 3 subject DER, 4 issuer DER, 5 SPKI DER, 6 signature */
int64_t trc_x509_bytes(void* h, int64_t field, char* out, int64_t cap) {
    x509* x = (x509*)h; const u8* p; size_t n;
    if (!x) return -1;
    switch (field) {
        case 1: p = x->raw; n = x->rawlen; break;
        case 2: p = x->tbs.p; n = x->tbs.n; break;
        case 3: p = x->subject.p; n = x->subject.n; break;
        case 4: p = x->issuer.p; n = x->issuer.n; break;
        case 5: p = x->spki.p; n = x->spki.n; break;
        case 6: p = x->sig.p; n = x->sig.n; break;
        default: return -1;
    }
    if (!out) return (int64_t)n;
    if ((int64_t)n > cap) return -1;
    memcpy(out, p, n);
    return (int64_t)n;
}
int64_t trc_x509_matches_host(void* h, const char* host, int64_t n) { return h ? x_host_ok((x509*)h, host, (size_t)n) : 0; }
/* 1 if `issuer` signed `child`. */
int64_t trc_x509_signed_by(void* child, void* issuer) {
    x509* c = (x509*)child; x509* i = (x509*)issuer;
    if (!c || !i || !i->key_ok) return 0;
    return x_sig_ok(c, &i->key) == 0;
}
/* A public-key handle for the certificate's key (caller frees with trc_pk_free). */
void* trc_x509_public_key(void* h) {
    x509* x = (x509*)h; pk_key* k;
    if (!x || !x->key_ok) return NULL;
    k = (pk_key*)malloc(sizeof(pk_key));
    if (k) *k = x->key;
    return k;
}

/* ── chain verification ──
 * chain[0] is the leaf, the rest are untrusted intermediates in any order.
 * purpose: 1 = TLS server (serverAuth), 2 = TLS client (clientAuth), 0 = any.
 * Returns 0 if a path to a trust anchor in `store` exists, else an error:
 *  -1 parse error   -2 expired/not yet valid   -3 hostname mismatch
 *  -4 no path to a trusted root   -5 bad signature   -6 not a CA / bad key usage
 *  -7 path length exceeded   -8 wrong extended key usage   -9 name constraints
 * -10 unsupported critical extension   -11 unsupported key/signature algorithm */
#define X_MAX_CHAIN 10
static const char* x_err_text(int e) {
    switch (e) {
        case 0: return "ok"; case -1: return "certificate could not be parsed";
        case -2: return "certificate expired or not yet valid"; case -3: return "certificate does not match the host name";
        case -4: return "no path to a trusted root certificate"; case -5: return "certificate signature is invalid";
        case -6: return "issuer is not a CA or may not sign certificates"; case -7: return "path length constraint exceeded";
        case -8: return "certificate is not valid for this purpose (extended key usage)"; case -9: return "name constraints violated";
        case -10: return "unsupported critical extension"; case -11: return "unsupported key or signature algorithm";
    }
    return "certificate verification failed";
}
int64_t trc_x509_error_text(int64_t code, char* out, int64_t cap) {
    const char* s = x_err_text((int)code); size_t n = strlen(s);
    if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, s, n); return (int64_t)n;
}

static int x_time_ok(const x509* x, int64_t now) { return now >= x->not_before && now <= x->not_after; }
static int x_ca_ok(const x509* ca) {
    if (ca->version >= 3 && (!ca->has_bc || !ca->is_ca)) return 0;
    if (ca->key_usage >= 0 && !(ca->key_usage & (1 << 5))) return 0;
    return 1;
}
static int same_name(const der* a, const der* b) { return a->n == b->n && memcmp(a->p, b->p, a->n) == 0; }

/* Depth-first search from `cur` (at depth) to a trust anchor. */
static int x_path(x509** chain, int nchain, int* used, x_store* store, x509* cur, int depth, int64_t now, x509* leaf, int want, int* worst) {
    int i, r;
    if (depth >= X_MAX_CHAIN) return -7;
    /* 1. directly signed by a trust anchor? */
    for (i = 0; i < store->n; i++) {
        x509* a = store->certs[i];
        if (!same_name(&a->subject, &cur->issuer) || !a->key_ok) continue;
        if (x_sig_ok(cur, &a->key) != 0) { if (*worst > -5) *worst = -5; continue; }
        if (!x_time_ok(a, now)) { if (*worst > -2) *worst = -2; continue; }
        if (a->path_len >= 0 && depth - 1 > a->path_len) { if (*worst > -7) *worst = -7; continue; }
        if (!x_nc_ok(a, leaf)) { if (*worst > -9) *worst = -9; continue; }
        return 0;
    }
    /* 2. via an intermediate */
    for (i = 1; i < nchain; i++) {
        x509* c = chain[i];
        if (used[i] || !same_name(&c->subject, &cur->issuer) || !c->key_ok) continue;
        if (x_sig_ok(cur, &c->key) != 0) { if (*worst > -5) *worst = -5; continue; }
        if (!x_time_ok(c, now)) { if (*worst > -2) *worst = -2; continue; }
        if (!x_ca_ok(c)) { if (*worst > -6) *worst = -6; continue; }
        if (c->unknown_critical) { if (*worst > -10) *worst = -10; continue; }
        if (c->path_len >= 0 && depth - 1 > c->path_len) { if (*worst > -7) *worst = -7; continue; }
        if (!x_nc_ok(c, leaf)) { if (*worst > -9) *worst = -9; continue; }
        if (want && c->eku >= 0 && !(c->eku & (want | 4))) { if (*worst > -8) *worst = -8; continue; }
        used[i] = 1;
        r = x_path(chain, nchain, used, store, c, depth + 1, now, leaf, want, worst);
        used[i] = 0;
        if (r == 0) return 0;
    }
    return -4;
}
/* ders: concatenated DER certificates; lens: their lengths (int64 each). */
int64_t trc_x509_verify_chain(void* store_h, const char* ders, const int64_t* lens, int64_t count,
                              const char* host, int64_t hostlen, int64_t now, int64_t purpose) {
    x_store* store = (x_store*)store_h; x509* chain[X_MAX_CHAIN + 6]; int used[X_MAX_CHAIN + 6]; int n = 0, i, err, worst = 0;
    int64_t off = 0, result; x509* leaf;
    if (!store || count < 1) return -1;
    if (count > X_MAX_CHAIN + 6) count = X_MAX_CHAIN + 6;
    for (i = 0; i < count; i++) {
        x509* c = x_parse((const u8*)ders + off, (size_t)lens[i], &err);
        off += lens[i];
        if (!c) { if (i == 0) { result = -1; goto done; } continue; }
        chain[n] = c; used[n] = 0; n++;
    }
    leaf = chain[0];
    if (now == 0) now = (int64_t)time(NULL);
    if (!leaf->key_ok) { result = -11; goto done; }
    if (!x_time_ok(leaf, now)) { result = -2; goto done; }
    if (hostlen > 0 && !x_host_ok(leaf, host, (size_t)hostlen)) { result = -3; goto done; }
    if (leaf->unknown_critical) { result = -10; goto done; }
    if (purpose == 1 || purpose == 2) {
        int want = purpose == 1 ? 1 : 2;
        if (leaf->eku >= 0 && !(leaf->eku & (want | 4))) { result = -8; goto done; }
        if (leaf->key_usage >= 0 && !(leaf->key_usage & 1) && leaf->key.kind != 1) { result = -6; goto done; }
    }
    /* A trust anchor that is itself the leaf (pinned self-signed cert). */
    for (i = 0; i < store->n; i++)
        if (store->certs[i]->rawlen == leaf->rawlen && memcmp(store->certs[i]->raw, leaf->raw, leaf->rawlen) == 0) { result = 0; goto done; }
    used[0] = 1;
    result = x_path(chain, n, used, store, leaf, 1, now, leaf, (purpose == 1 || purpose == 2) ? (int)purpose : 0, &worst);
    if (result != 0 && worst < 0) result = worst;
done:
    for (i = 0; i < n; i++) x_free(chain[i]);
    return result;
}
/* Verify a raw signature with a certificate's public key (used by TLS
 * CertificateVerify). scheme: TLS SignatureScheme code. 1 = valid. */
int64_t trc_x509_verify_tls_sig(const char* leaf_der, int64_t leaf_len, int64_t scheme, const char* msg, int64_t mlen, const char* sig, int64_t slen) {
    int err; x509* x = x_parse((const u8*)leaf_der, (size_t)leaf_len, &err); int ok = 0;
    if (!x) return 0;
    if (x->key_ok) {
        pk_key* k = &x->key; u8 h[64];
        switch (scheme) {
            case 0x0403: if (k->kind == 2) { trc_digest(256, msg, mlen, (char*)h); ok = pk_verify_hash(k, 0, 256, h, 32, (const u8*)sig, (size_t)slen, 0) == 0; } break;
            case 0x0503: if (k->kind == 3) { trc_digest(384, msg, mlen, (char*)h); ok = pk_verify_hash(k, 0, 384, h, 48, (const u8*)sig, (size_t)slen, 0) == 0; } break;
            case 0x0804: case 0x0805: case 0x0806: {
                int alg = scheme == 0x0804 ? 256 : scheme == 0x0805 ? 384 : 512;
                if (k->kind == 1) { trc_digest(alg, msg, mlen, (char*)h); ok = rsa_verify_hash(&k->rsa, 1, alg, h, (const u8*)sig, (size_t)slen, md_len(alg)) == 0; }
                break;
            }
            case 0x0401: case 0x0501: case 0x0601: {
                int alg = scheme == 0x0401 ? 256 : scheme == 0x0501 ? 384 : 512;
                if (k->kind == 1) { trc_digest(alg, msg, mlen, (char*)h); ok = rsa_verify_hash(&k->rsa, 0, alg, h, (const u8*)sig, (size_t)slen, -1) == 0; }
                break;
            }
        }
    }
    x_free(x);
    return ok;
}

/* RSA public exponent as minimal big-endian bytes (std.crypto.ssh). Returns the length or -1. */
int64_t trc_pk_rsa_exponent(void* h, char* out, int64_t cap) {
    pk_key* k = (pk_key*)h; u8 tmp[4 * MAXL]; size_t len = (size_t)(4 * MAXL), i = 0;
    if (!k || k->kind != 1) return -1;
    bn_to_be(tmp, len, k->rsa.e, k->rsa.n_limbs);
    while (i < len - 1 && tmp[i] == 0) i++;
    if ((int64_t)(len - i) > cap) return -1;
    memcpy(out, tmp + i, len - i);
    return (int64_t)(len - i);
}
