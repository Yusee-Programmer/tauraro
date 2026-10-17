/*
 * std/crypto/crypto_native.c -- C kernels behind std.crypto's newer modules.
 *
 * Compiled and linked automatically when a module that carries the
 * `# tauraro-cpp-shim: crypto_native.c` pragma (std/crypto/native.tr) is
 * imported. Plain C99, no dependencies beyond libc (+ bcrypt/advapi on
 * Windows for the CSPRNG).
 *
 * Calling convention used throughout: every buffer is (pointer, explicit
 * length). Outputs are written into caller-allocated buffers, so this file
 * never allocates memory that the Tauraro side has to free. Functions that
 * can fail return a negative int; 0 (or a length) means success.
 *
 * Contents:
 *   CSPRNG ........................ trc_random
 *   constant-time compare ......... trc_ct_eq
 *   SHA-1 / SHA-256 / SHA-512/384  trc_sha1, trc_sha256, trc_sha512, trc_sha384
 *   HMAC (any of the above) ....... trc_hmac
 *   HKDF (RFC 5869) ............... trc_hkdf_extract, trc_hkdf_expand
 *   SHA-3 / SHAKE (FIPS 202) ...... trc_sha3, trc_shake, streaming trc_keccak_*
 *   BLAKE2b / BLAKE2s (RFC 7693) .. trc_blake2b, trc_blake2s
 *   BLAKE3 ........................ trc_blake3
 *   X25519 (RFC 7748) ............. trc_x25519
 *   Argon2 i/d/id (RFC 9106) ...... trc_argon2
 *   bcrypt ($2a/$2b/$2y) .......... trc_bcrypt_raw
 *   AEAD .......................... trc_aead_seal/open (AES-128/256-GCM, ChaCha20-Poly1305)
 *   scrypt (RFC 7914) ............. trc_scrypt, trc_scrypt_mem
 *   bcrypt_pbkdf (OpenSSH) ........ trc_bcrypt_pbkdf
 *   AES-CTR ....................... trc_aes_ctr
 *   age STREAM .................... trc_stream_chunk/seal/open, trc_stream_open_chunk
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
#  include <windows.h>
#  include <bcrypt.h>
#  ifdef _MSC_VER
#    pragma comment(lib, "bcrypt.lib")
#  endif
#elif defined(__linux__)
#  include <unistd.h>
#  include <sys/syscall.h>
#  include <errno.h>
#  include <fcntl.h>
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#endif

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

static u32 ld32le(const u8* p) { return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24); }
static u64 ld64le(const u8* p) { return (u64)ld32le(p) | ((u64)ld32le(p + 4) << 32); }
static void st32le(u8* p, u32 v) { p[0] = (u8)v; p[1] = (u8)(v >> 8); p[2] = (u8)(v >> 16); p[3] = (u8)(v >> 24); }
static void st64le(u8* p, u64 v) { st32le(p, (u32)v); st32le(p + 4, (u32)(v >> 32)); }
static u32 ld32be(const u8* p) { return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3]; }
static u64 ld64be(const u8* p) { return ((u64)ld32be(p) << 32) | (u64)ld32be(p + 4); }
static void st32be(u8* p, u32 v) { p[0] = (u8)(v >> 24); p[1] = (u8)(v >> 16); p[2] = (u8)(v >> 8); p[3] = (u8)v; }
static void st64be(u8* p, u64 v) { st32be(p, (u32)(v >> 32)); st32be(p + 4, (u32)v); }
static u32 rotr32(u32 x, int n) { return (x >> n) | (x << (32 - n)); }
static u32 rotl32(u32 x, int n) { return (x << n) | (x >> (32 - n)); }
static u64 rotr64(u64 x, int n) { return (x >> n) | (x << (64 - n)); }
static u64 rotl64(u64 x, int n) { return (x << n) | (x >> (64 - n)); }

/* Best-effort wipe that the optimizer may not elide. */
static void wipe(void* p, size_t n) {
    volatile u8* v = (volatile u8*)p;
    while (n--) *v++ = 0;
}

/* ═══════════════════════════ CSPRNG ═══════════════════════════════════════
 * Returns 0 on success, -1 if the OS generator is unavailable. Unlike the
 * runtime's older _tr_os_random this never falls back to rand(). */
int64_t trc_random(char* out, int64_t n) {
    u8* p = (u8*)out;
    if (n <= 0) return 0;
#if defined(_WIN32)
    while (n > 0) {
        ULONG chunk = n > 0x40000000 ? 0x40000000 : (ULONG)n;
        if (BCryptGenRandom(NULL, p, chunk, BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0) return -1;
        p += chunk; n -= chunk;
    }
    return 0;
#else
#  if defined(__linux__) && defined(SYS_getrandom)
    while (n > 0) {
        long r = syscall(SYS_getrandom, p, (size_t)n, 0);
        if (r < 0) { if (errno == EINTR) continue; break; }
        p += r; n -= r;
    }
    if (n == 0) return 0;
#  endif
    {
        int fd = open("/dev/urandom", O_RDONLY);
        if (fd < 0) return -1;
        while (n > 0) {
            ssize_t r = read(fd, p, (size_t)n);
            if (r < 0) { if (errno == EINTR) continue; close(fd); return -1; }
            if (r == 0) { close(fd); return -1; }
            p += r; n -= r;
        }
        close(fd);
        return 0;
    }
#endif
}

/* ═══════════════════════ constant-time compare ════════════════════════════
 * 1 if the two buffers are equal. Time depends only on the lengths. */
int64_t trc_ct_eq(const char* a, int64_t alen, const char* b, int64_t blen) {
    volatile u8 acc = 0;
    int64_t i, n = alen < blen ? alen : blen;
    if (alen != blen) acc = 1;
    for (i = 0; i < n; i++) acc |= (u8)(a[i] ^ b[i]);
    return acc == 0 ? 1 : 0;
}

/* ═══════════════════════════ SHA-1 ═══════════════════════════════════════ */
typedef struct { u32 h[5]; u8 buf[64]; u64 len; u32 n; } sha1_ctx;

static void sha1_block(sha1_ctx* c, const u8* p) {
    u32 w[80], a, b, cc, d, e, t;
    int i;
    for (i = 0; i < 16; i++) w[i] = ld32be(p + 4 * i);
    for (i = 16; i < 80; i++) w[i] = rotl32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    a = c->h[0]; b = c->h[1]; cc = c->h[2]; d = c->h[3]; e = c->h[4];
    for (i = 0; i < 80; i++) {
        u32 f, k;
        if (i < 20) { f = (b & cc) | (~b & d); k = 0x5a827999; }
        else if (i < 40) { f = b ^ cc ^ d; k = 0x6ed9eba1; }
        else if (i < 60) { f = (b & cc) | (b & d) | (cc & d); k = 0x8f1bbcdc; }
        else { f = b ^ cc ^ d; k = 0xca62c1d6; }
        t = rotl32(a, 5) + f + e + k + w[i];
        e = d; d = cc; cc = rotl32(b, 30); b = a; a = t;
    }
    c->h[0] += a; c->h[1] += b; c->h[2] += cc; c->h[3] += d; c->h[4] += e;
}
static void sha1_init(sha1_ctx* c) {
    c->h[0] = 0x67452301; c->h[1] = 0xefcdab89; c->h[2] = 0x98badcfe; c->h[3] = 0x10325476; c->h[4] = 0xc3d2e1f0;
    c->len = 0; c->n = 0;
}
static void sha1_update(sha1_ctx* c, const u8* p, size_t n) {
    c->len += n;
    while (n > 0) {
        size_t take = 64 - c->n; if (take > n) take = n;
        memcpy(c->buf + c->n, p, take); c->n += (u32)take; p += take; n -= take;
        if (c->n == 64) { sha1_block(c, c->buf); c->n = 0; }
    }
}
static void sha1_final(sha1_ctx* c, u8* out) {
    u64 bits = c->len * 8; u8 pad = 0x80, z = 0, lb[8]; int i;
    sha1_update(c, &pad, 1);
    while (c->n != 56) sha1_update(c, &z, 1);
    st64be(lb, bits); sha1_update(c, lb, 8);
    for (i = 0; i < 5; i++) st32be(out + 4 * i, c->h[i]);
}

/* ═══════════════════════════ SHA-256 ═════════════════════════════════════ */
static const u32 K256[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};
typedef struct { u32 h[8]; u8 buf[64]; u64 len; u32 n; } sha256_ctx;

static void sha256_block(sha256_ctx* c, const u8* p) {
    u32 w[64], s[8], t1, t2; int i;
    for (i = 0; i < 16; i++) w[i] = ld32be(p + 4 * i);
    for (i = 16; i < 64; i++) {
        u32 s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^ (w[i - 15] >> 3);
        u32 s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    for (i = 0; i < 8; i++) s[i] = c->h[i];
    for (i = 0; i < 64; i++) {
        u32 S1 = rotr32(s[4], 6) ^ rotr32(s[4], 11) ^ rotr32(s[4], 25);
        u32 ch = (s[4] & s[5]) ^ (~s[4] & s[6]);
        u32 S0 = rotr32(s[0], 2) ^ rotr32(s[0], 13) ^ rotr32(s[0], 22);
        u32 mj = (s[0] & s[1]) ^ (s[0] & s[2]) ^ (s[1] & s[2]);
        t1 = s[7] + S1 + ch + K256[i] + w[i];
        t2 = S0 + mj;
        s[7] = s[6]; s[6] = s[5]; s[5] = s[4]; s[4] = s[3] + t1;
        s[3] = s[2]; s[2] = s[1]; s[1] = s[0]; s[0] = t1 + t2;
    }
    for (i = 0; i < 8; i++) c->h[i] += s[i];
}
static void sha256_init(sha256_ctx* c) {
    static const u32 iv[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    memcpy(c->h, iv, sizeof iv); c->len = 0; c->n = 0;
}
static void sha256_update(sha256_ctx* c, const u8* p, size_t n) {
    c->len += n;
    while (n > 0) {
        size_t take = 64 - c->n; if (take > n) take = n;
        memcpy(c->buf + c->n, p, take); c->n += (u32)take; p += take; n -= take;
        if (c->n == 64) { sha256_block(c, c->buf); c->n = 0; }
    }
}
static void sha256_final(sha256_ctx* c, u8* out) {
    u64 bits = c->len * 8; u8 pad = 0x80, z = 0, lb[8]; int i;
    sha256_update(c, &pad, 1);
    while (c->n != 56) sha256_update(c, &z, 1);
    st64be(lb, bits); sha256_update(c, lb, 8);
    for (i = 0; i < 8; i++) st32be(out + 4 * i, c->h[i]);
}

/* ═══════════════════════════ SHA-512 / 384 ═══════════════════════════════ */
static const u64 K512[80] = {
  0x428a2f98d728ae22ULL,0x7137449123ef65cdULL,0xb5c0fbcfec4d3b2fULL,0xe9b5dba58189dbbcULL,
  0x3956c25bf348b538ULL,0x59f111f1b605d019ULL,0x923f82a4af194f9bULL,0xab1c5ed5da6d8118ULL,
  0xd807aa98a3030242ULL,0x12835b0145706fbeULL,0x243185be4ee4b28cULL,0x550c7dc3d5ffb4e2ULL,
  0x72be5d74f27b896fULL,0x80deb1fe3b1696b1ULL,0x9bdc06a725c71235ULL,0xc19bf174cf692694ULL,
  0xe49b69c19ef14ad2ULL,0xefbe4786384f25e3ULL,0x0fc19dc68b8cd5b5ULL,0x240ca1cc77ac9c65ULL,
  0x2de92c6f592b0275ULL,0x4a7484aa6ea6e483ULL,0x5cb0a9dcbd41fbd4ULL,0x76f988da831153b5ULL,
  0x983e5152ee66dfabULL,0xa831c66d2db43210ULL,0xb00327c898fb213fULL,0xbf597fc7beef0ee4ULL,
  0xc6e00bf33da88fc2ULL,0xd5a79147930aa725ULL,0x06ca6351e003826fULL,0x142929670a0e6e70ULL,
  0x27b70a8546d22ffcULL,0x2e1b21385c26c926ULL,0x4d2c6dfc5ac42aedULL,0x53380d139d95b3dfULL,
  0x650a73548baf63deULL,0x766a0abb3c77b2a8ULL,0x81c2c92e47edaee6ULL,0x92722c851482353bULL,
  0xa2bfe8a14cf10364ULL,0xa81a664bbc423001ULL,0xc24b8b70d0f89791ULL,0xc76c51a30654be30ULL,
  0xd192e819d6ef5218ULL,0xd69906245565a910ULL,0xf40e35855771202aULL,0x106aa07032bbd1b8ULL,
  0x19a4c116b8d2d0c8ULL,0x1e376c085141ab53ULL,0x2748774cdf8eeb99ULL,0x34b0bcb5e19b48a8ULL,
  0x391c0cb3c5c95a63ULL,0x4ed8aa4ae3418acbULL,0x5b9cca4f7763e373ULL,0x682e6ff3d6b2b8a3ULL,
  0x748f82ee5defb2fcULL,0x78a5636f43172f60ULL,0x84c87814a1f0ab72ULL,0x8cc702081a6439ecULL,
  0x90befffa23631e28ULL,0xa4506cebde82bde9ULL,0xbef9a3f7b2c67915ULL,0xc67178f2e372532bULL,
  0xca273eceea26619cULL,0xd186b8c721c0c207ULL,0xeada7dd6cde0eb1eULL,0xf57d4f7fee6ed178ULL,
  0x06f067aa72176fbaULL,0x0a637dc5a2c898a6ULL,0x113f9804bef90daeULL,0x1b710b35131c471bULL,
  0x28db77f523047d84ULL,0x32caab7b40c72493ULL,0x3c9ebe0a15c9bebcULL,0x431d67c49c100d4cULL,
  0x4cc5d4becb3e42b6ULL,0x597f299cfc657e2aULL,0x5fcb6fab3ad6faecULL,0x6c44198c4a475817ULL
};
typedef struct { u64 h[8]; u8 buf[128]; u64 len; u32 n; } sha512_ctx;

static void sha512_block(sha512_ctx* c, const u8* p) {
    u64 w[80], s[8], t1, t2; int i;
    for (i = 0; i < 16; i++) w[i] = ld64be(p + 8 * i);
    for (i = 16; i < 80; i++) {
        u64 s0 = rotr64(w[i - 15], 1) ^ rotr64(w[i - 15], 8) ^ (w[i - 15] >> 7);
        u64 s1 = rotr64(w[i - 2], 19) ^ rotr64(w[i - 2], 61) ^ (w[i - 2] >> 6);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    for (i = 0; i < 8; i++) s[i] = c->h[i];
    for (i = 0; i < 80; i++) {
        u64 S1 = rotr64(s[4], 14) ^ rotr64(s[4], 18) ^ rotr64(s[4], 41);
        u64 ch = (s[4] & s[5]) ^ (~s[4] & s[6]);
        u64 S0 = rotr64(s[0], 28) ^ rotr64(s[0], 34) ^ rotr64(s[0], 39);
        u64 mj = (s[0] & s[1]) ^ (s[0] & s[2]) ^ (s[1] & s[2]);
        t1 = s[7] + S1 + ch + K512[i] + w[i];
        t2 = S0 + mj;
        s[7] = s[6]; s[6] = s[5]; s[5] = s[4]; s[4] = s[3] + t1;
        s[3] = s[2]; s[2] = s[1]; s[1] = s[0]; s[0] = t1 + t2;
    }
    for (i = 0; i < 8; i++) c->h[i] += s[i];
}
static void sha512_init_iv(sha512_ctx* c, int is384) {
    static const u64 iv512[8] = {0x6a09e667f3bcc908ULL,0xbb67ae8584caa73bULL,0x3c6ef372fe94f82bULL,0xa54ff53a5f1d36f1ULL,
                                 0x510e527fade682d1ULL,0x9b05688c2b3e6c1fULL,0x1f83d9abfb41bd6bULL,0x5be0cd19137e2179ULL};
    static const u64 iv384[8] = {0xcbbb9d5dc1059ed8ULL,0x629a292a367cd507ULL,0x9159015a3070dd17ULL,0x152fecd8f70e5939ULL,
                                 0x67332667ffc00b31ULL,0x8eb44a8768581511ULL,0xdb0c2e0d64f98fa7ULL,0x47b5481dbefa4fa4ULL};
    memcpy(c->h, is384 ? iv384 : iv512, sizeof iv512); c->len = 0; c->n = 0;
}
static void sha512_update(sha512_ctx* c, const u8* p, size_t n) {
    c->len += n;
    while (n > 0) {
        size_t take = 128 - c->n; if (take > n) take = n;
        memcpy(c->buf + c->n, p, take); c->n += (u32)take; p += take; n -= take;
        if (c->n == 128) { sha512_block(c, c->buf); c->n = 0; }
    }
}
static void sha512_final(sha512_ctx* c, u8* out, int outlen) {
    u64 bits = c->len * 8; u8 pad = 0x80, z = 0, lb[16]; u8 full[64]; int i;
    sha512_update(c, &pad, 1);
    while (c->n != 112) sha512_update(c, &z, 1);
    memset(lb, 0, 8); st64be(lb + 8, bits); sha512_update(c, lb, 16);
    for (i = 0; i < 8; i++) st64be(full + 8 * i, c->h[i]);
    memcpy(out, full, (size_t)outlen);
}

/* ═══════════════════════ generic hash dispatch ═══════════════════════════
 * alg: 1 = SHA-1, 256 = SHA-256, 384 = SHA-384, 512 = SHA-512. */
typedef struct { int alg; union { sha1_ctx s1; sha256_ctx s256; sha512_ctx s512; } u; } md_ctx;

static int md_size(int alg) {
    switch (alg) { case 1: return 20; case 256: return 32; case 384: return 48; case 512: return 64; }
    return -1;
}
static int md_block(int alg) { return (alg == 384 || alg == 512) ? 128 : 64; }
static int md_init(md_ctx* c, int alg) {
    c->alg = alg;
    switch (alg) {
        case 1: sha1_init(&c->u.s1); return 0;
        case 256: sha256_init(&c->u.s256); return 0;
        case 384: sha512_init_iv(&c->u.s512, 1); return 0;
        case 512: sha512_init_iv(&c->u.s512, 0); return 0;
    }
    return -1;
}
static void md_update(md_ctx* c, const u8* p, size_t n) {
    switch (c->alg) {
        case 1: sha1_update(&c->u.s1, p, n); break;
        case 256: sha256_update(&c->u.s256, p, n); break;
        default: sha512_update(&c->u.s512, p, n); break;
    }
}
static void md_final(md_ctx* c, u8* out) {
    switch (c->alg) {
        case 1: sha1_final(&c->u.s1, out); break;
        case 256: sha256_final(&c->u.s256, out); break;
        case 384: sha512_final(&c->u.s512, out, 48); break;
        default: sha512_final(&c->u.s512, out, 64); break;
    }
}

/* One-shot digest; returns the digest length or -1 for an unknown alg. */
int64_t trc_digest(int64_t alg, const char* data, int64_t n, char* out) {
    md_ctx c;
    if (md_init(&c, (int)alg) < 0) return -1;
    md_update(&c, (const u8*)data, (size_t)n);
    md_final(&c, (u8*)out);
    return md_size((int)alg);
}

/* Streaming digest over an opaque state buffer of trc_digest_state_size() bytes. */
int64_t trc_digest_state_size(void) { return (int64_t)sizeof(md_ctx); }
int64_t trc_digest_begin(char* st, int64_t alg) { return md_init((md_ctx*)st, (int)alg); }
void trc_digest_update(char* st, const char* data, int64_t n) { md_update((md_ctx*)st, (const u8*)data, (size_t)n); }
int64_t trc_digest_finish(char* st, char* out) {
    md_ctx* c = (md_ctx*)st; int sz = md_size(c->alg);
    md_final(c, (u8*)out);
    return sz;
}

/* ═══════════════════════════ HMAC ════════════════════════════════════════ */
typedef struct { md_ctx inner, outer; } hmac_ctx;

static int hmac_init(hmac_ctx* h, int alg, const u8* key, size_t klen) {
    u8 k[128], pad[128]; int bs = md_block(alg), i;
    if (md_size(alg) < 0) return -1;
    memset(k, 0, sizeof k);
    if (klen > (size_t)bs) {
        md_ctx t; md_init(&t, alg); md_update(&t, key, klen); md_final(&t, k);
    } else if (klen > 0) {
        memcpy(k, key, klen);
    }
    md_init(&h->inner, alg); md_init(&h->outer, alg);
    for (i = 0; i < bs; i++) pad[i] = k[i] ^ 0x36;
    md_update(&h->inner, pad, (size_t)bs);
    for (i = 0; i < bs; i++) pad[i] = k[i] ^ 0x5c;
    md_update(&h->outer, pad, (size_t)bs);
    wipe(k, sizeof k); wipe(pad, sizeof pad);
    return 0;
}
static void hmac_final(hmac_ctx* h, u8* out) {
    u8 ih[64]; int sz = md_size(h->inner.alg);
    md_final(&h->inner, ih);
    md_update(&h->outer, ih, (size_t)sz);
    md_final(&h->outer, out);
}

int64_t trc_hmac(int64_t alg, const char* key, int64_t klen, const char* msg, int64_t mlen, char* out) {
    hmac_ctx h;
    if (hmac_init(&h, (int)alg, (const u8*)key, (size_t)klen) < 0) return -1;
    md_update(&h.inner, (const u8*)msg, (size_t)mlen);
    hmac_final(&h, (u8*)out);
    return md_size((int)alg);
}

/* ═══════════════════════════ HKDF (RFC 5869) ═════════════════════════════ */
int64_t trc_hkdf_extract(int64_t alg, const char* salt, int64_t slen, const char* ikm, int64_t ilen, char* prk) {
    u8 zeros[64];
    int sz = md_size((int)alg);
    if (sz < 0) return -1;
    if (slen == 0) { memset(zeros, 0, sizeof zeros); return trc_hmac(alg, (const char*)zeros, sz, ikm, ilen, prk); }
    return trc_hmac(alg, salt, slen, ikm, ilen, prk);
}

int64_t trc_hkdf_expand(int64_t alg, const char* prk, int64_t plen, const char* info, int64_t ilen, char* out, int64_t olen) {
    u8 t[64]; int sz = md_size((int)alg); int64_t done = 0; u8 ctr = 1; size_t tlen = 0;
    if (sz < 0 || olen > 255 * (int64_t)sz || olen < 0) return -1;
    while (done < olen) {
        hmac_ctx h; int64_t take;
        hmac_init(&h, (int)alg, (const u8*)prk, (size_t)plen);
        md_update(&h.inner, t, tlen);
        md_update(&h.inner, (const u8*)info, (size_t)ilen);
        md_update(&h.inner, &ctr, 1);
        hmac_final(&h, t);
        tlen = (size_t)sz;
        take = olen - done < sz ? olen - done : sz;
        memcpy(out + done, t, (size_t)take);
        done += take; ctr++;
    }
    wipe(t, sizeof t);
    return olen;
}

/* ═══════════════════════════ Keccak / SHA-3 ═════════════════════════════ */
static const u64 keccak_rc[24] = {
    0x0000000000000001ULL,0x0000000000008082ULL,0x800000000000808aULL,0x8000000080008000ULL,
    0x000000000000808bULL,0x0000000080000001ULL,0x8000000080008081ULL,0x8000000000008009ULL,
    0x000000000000008aULL,0x0000000000000088ULL,0x0000000080008009ULL,0x000000008000000aULL,
    0x000000008000808bULL,0x800000000000008bULL,0x8000000000008089ULL,0x8000000000008003ULL,
    0x8000000000008002ULL,0x8000000000000080ULL,0x000000000000800aULL,0x800000008000000aULL,
    0x8000000080008081ULL,0x8000000000008080ULL,0x0000000080000001ULL,0x8000000080008008ULL
};
static const int keccak_rotc[24] = {1,3,6,10,15,21,28,36,45,55,2,14,27,41,56,8,25,43,62,18,39,61,20,44};
static const int keccak_piln[24] = {10,7,11,17,18,3,5,16,8,21,24,4,15,23,19,13,12,2,20,14,22,9,6,1};

static void keccakf(u64 st[25]) {
    int i, j, r; u64 t, bc[5];
    for (r = 0; r < 24; r++) {
        for (i = 0; i < 5; i++) bc[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20];
        for (i = 0; i < 5; i++) {
            t = bc[(i + 4) % 5] ^ rotl64(bc[(i + 1) % 5], 1);
            for (j = 0; j < 25; j += 5) st[j + i] ^= t;
        }
        t = st[1];
        for (i = 0; i < 24; i++) { j = keccak_piln[i]; bc[0] = st[j]; st[j] = rotl64(t, keccak_rotc[i]); t = bc[0]; }
        for (j = 0; j < 25; j += 5) {
            for (i = 0; i < 5; i++) bc[i] = st[j + i];
            for (i = 0; i < 5; i++) st[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
        }
        st[0] ^= keccak_rc[r];
    }
}

/* Sponge state: lanes + byte position + rate + domain byte + squeeze flag. */
typedef struct { u64 st[25]; u32 pos; u32 rate; u8 dsbyte; u8 squeezing; } keccak_ctx;

static void keccak_xor_byte(keccak_ctx* k, u32 i, u8 b) { k->st[i >> 3] ^= (u64)b << (8 * (i & 7)); }
static u8 keccak_get_byte(keccak_ctx* k, u32 i) { return (u8)(k->st[i >> 3] >> (8 * (i & 7))); }

static void keccak_init(keccak_ctx* k, u32 rate, u8 ds) { memset(k, 0, sizeof *k); k->rate = rate; k->dsbyte = ds; }
static void keccak_absorb(keccak_ctx* k, const u8* p, size_t n) {
    while (n--) {
        keccak_xor_byte(k, k->pos++, *p++);
        if (k->pos == k->rate) { keccakf(k->st); k->pos = 0; }
    }
}
static void keccak_pad(keccak_ctx* k) {
    keccak_xor_byte(k, k->pos, k->dsbyte);
    keccak_xor_byte(k, k->rate - 1, 0x80);
    keccakf(k->st);
    k->pos = 0; k->squeezing = 1;
}
static void keccak_squeeze(keccak_ctx* k, u8* out, size_t n) {
    if (!k->squeezing) keccak_pad(k);
    while (n--) {
        if (k->pos == k->rate) { keccakf(k->st); k->pos = 0; }
        *out++ = keccak_get_byte(k, k->pos++);
    }
}

/* bits: 224/256/384/512. Returns digest length or -1. */
int64_t trc_sha3(int64_t bits, const char* data, int64_t n, char* out) {
    keccak_ctx k;
    if (bits != 224 && bits != 256 && bits != 384 && bits != 512) return -1;
    keccak_init(&k, (u32)(200 - 2 * (bits / 8)), 0x06);
    keccak_absorb(&k, (const u8*)data, (size_t)n);
    keccak_squeeze(&k, (u8*)out, (size_t)(bits / 8));
    return bits / 8;
}

/* bits: 128 or 256 (security level). Writes olen bytes. */
int64_t trc_shake(int64_t bits, const char* data, int64_t n, char* out, int64_t olen) {
    keccak_ctx k;
    if (bits != 128 && bits != 256) return -1;
    keccak_init(&k, (u32)(200 - 2 * (bits / 8)), 0x1f);
    keccak_absorb(&k, (const u8*)data, (size_t)n);
    keccak_squeeze(&k, (u8*)out, (size_t)olen);
    return olen;
}

/* Streaming sponge. kind: 224/256/384/512 = SHA3-n, 128/256+1000 = SHAKE128/256,
 * i.e. 1128 and 1256. */
int64_t trc_keccak_state_size(void) { return (int64_t)sizeof(keccak_ctx); }
int64_t trc_keccak_begin(char* st, int64_t kind) {
    keccak_ctx* k = (keccak_ctx*)st;
    if (kind == 224 || kind == 256 || kind == 384 || kind == 512) { keccak_init(k, (u32)(200 - 2 * (kind / 8)), 0x06); return 0; }
    if (kind == 1128) { keccak_init(k, 168, 0x1f); return 0; }
    if (kind == 1256) { keccak_init(k, 136, 0x1f); return 0; }
    return -1;
}
int64_t trc_keccak_update(char* st, const char* data, int64_t n) {
    keccak_ctx* k = (keccak_ctx*)st;
    if (k->squeezing) return -1;
    keccak_absorb(k, (const u8*)data, (size_t)n);
    return 0;
}
void trc_keccak_squeeze(char* st, char* out, int64_t n) { keccak_squeeze((keccak_ctx*)st, (u8*)out, (size_t)n); }

/* ═══════════════════════════ BLAKE2b ═════════════════════════════════════ */
static const u64 blake2b_iv[8] = {
    0x6a09e667f3bcc908ULL,0xbb67ae8584caa73bULL,0x3c6ef372fe94f82bULL,0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL,0x9b05688c2b3e6c1fULL,0x1f83d9abfb41bd6bULL,0x5be0cd19137e2179ULL
};
static const u8 blake2_sigma[12][16] = {
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3},
    {11,8,12,0,5,2,15,13,10,14,3,6,7,1,9,4}, {7,9,3,1,13,12,11,14,2,6,5,10,4,0,15,8},
    {9,0,5,7,2,4,10,15,14,1,11,12,6,8,3,13}, {2,12,6,10,0,11,8,3,4,13,7,5,15,14,1,9},
    {12,5,1,15,14,13,4,10,0,7,6,3,9,2,8,11}, {13,11,7,14,12,1,3,9,5,0,15,4,8,6,2,10},
    {6,15,14,9,11,3,0,8,12,2,13,7,1,4,10,5}, {10,2,8,4,7,6,1,5,15,11,9,14,3,12,13,0},
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3}
};
typedef struct { u64 h[8], t[2]; u8 b[128]; size_t c; size_t outlen; } blake2b_ctx;

#define B2B_G(a, b, c, d, x, y) do { \
    v[a] = v[a] + v[b] + x; v[d] = rotr64(v[d] ^ v[a], 32); \
    v[c] = v[c] + v[d];     v[b] = rotr64(v[b] ^ v[c], 24); \
    v[a] = v[a] + v[b] + y; v[d] = rotr64(v[d] ^ v[a], 16); \
    v[c] = v[c] + v[d];     v[b] = rotr64(v[b] ^ v[c], 63); } while (0)

static void blake2b_compress(blake2b_ctx* ctx, int last) {
    u64 v[16], m[16]; int i;
    for (i = 0; i < 8; i++) { v[i] = ctx->h[i]; v[i + 8] = blake2b_iv[i]; }
    v[12] ^= ctx->t[0]; v[13] ^= ctx->t[1];
    if (last) v[14] = ~v[14];
    for (i = 0; i < 16; i++) m[i] = ld64le(ctx->b + 8 * i);
    for (i = 0; i < 12; i++) {
        const u8* s = blake2_sigma[i];
        B2B_G(0, 4, 8, 12, m[s[0]], m[s[1]]);   B2B_G(1, 5, 9, 13, m[s[2]], m[s[3]]);
        B2B_G(2, 6, 10, 14, m[s[4]], m[s[5]]);  B2B_G(3, 7, 11, 15, m[s[6]], m[s[7]]);
        B2B_G(0, 5, 10, 15, m[s[8]], m[s[9]]);  B2B_G(1, 6, 11, 12, m[s[10]], m[s[11]]);
        B2B_G(2, 7, 8, 13, m[s[12]], m[s[13]]); B2B_G(3, 4, 9, 14, m[s[14]], m[s[15]]);
    }
    for (i = 0; i < 8; i++) ctx->h[i] ^= v[i] ^ v[i + 8];
}
static void blake2b_update(blake2b_ctx* ctx, const u8* in, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        if (ctx->c == 128) {
            ctx->t[0] += ctx->c; if (ctx->t[0] < ctx->c) ctx->t[1]++;
            blake2b_compress(ctx, 0); ctx->c = 0;
        }
        ctx->b[ctx->c++] = in[i];
    }
}
static int blake2b_init(blake2b_ctx* ctx, size_t outlen, const u8* key, size_t keylen) {
    int i;
    if (outlen == 0 || outlen > 64 || keylen > 64) return -1;
    for (i = 0; i < 8; i++) ctx->h[i] = blake2b_iv[i];
    ctx->h[0] ^= 0x01010000 ^ (keylen << 8) ^ outlen;
    ctx->t[0] = ctx->t[1] = 0; ctx->c = 0; ctx->outlen = outlen;
    memset(ctx->b, 0, sizeof ctx->b);
    if (keylen > 0) { blake2b_update(ctx, key, keylen); ctx->c = 128; }
    return 0;
}
static void blake2b_final(blake2b_ctx* ctx, u8* out) {
    size_t i;
    ctx->t[0] += ctx->c; if (ctx->t[0] < ctx->c) ctx->t[1]++;
    while (ctx->c < 128) ctx->b[ctx->c++] = 0;
    blake2b_compress(ctx, 1);
    for (i = 0; i < ctx->outlen; i++) out[i] = (u8)(ctx->h[i >> 3] >> (8 * (i & 7)));
}
static int blake2b(u8* out, size_t outlen, const u8* key, size_t keylen, const u8* in, size_t inlen) {
    blake2b_ctx c;
    if (blake2b_init(&c, outlen, key, keylen) < 0) return -1;
    blake2b_update(&c, in, inlen);
    blake2b_final(&c, out);
    return 0;
}

int64_t trc_blake2b(const char* data, int64_t n, const char* key, int64_t klen, char* out, int64_t olen) {
    if (blake2b((u8*)out, (size_t)olen, (const u8*)key, (size_t)klen, (const u8*)data, (size_t)n) < 0) return -1;
    return olen;
}
int64_t trc_blake2b_state_size(void) { return (int64_t)sizeof(blake2b_ctx); }
int64_t trc_blake2b_begin(char* st, const char* key, int64_t klen, int64_t olen) {
    return blake2b_init((blake2b_ctx*)st, (size_t)olen, (const u8*)key, (size_t)klen);
}
void trc_blake2b_update(char* st, const char* data, int64_t n) { blake2b_update((blake2b_ctx*)st, (const u8*)data, (size_t)n); }
int64_t trc_blake2b_finish(char* st, char* out) {
    blake2b_ctx* c = (blake2b_ctx*)st; blake2b_final(c, (u8*)out); return (int64_t)c->outlen;
}

/* ═══════════════════════════ BLAKE2s ═════════════════════════════════════ */
static const u32 blake2s_iv[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
typedef struct { u32 h[8], t[2]; u8 b[64]; size_t c; size_t outlen; } blake2s_ctx;

#define B2S_G(a, b, c, d, x, y) do { \
    v[a] = v[a] + v[b] + x; v[d] = rotr32(v[d] ^ v[a], 16); \
    v[c] = v[c] + v[d];     v[b] = rotr32(v[b] ^ v[c], 12); \
    v[a] = v[a] + v[b] + y; v[d] = rotr32(v[d] ^ v[a], 8);  \
    v[c] = v[c] + v[d];     v[b] = rotr32(v[b] ^ v[c], 7); } while (0)

static void blake2s_compress(blake2s_ctx* ctx, int last) {
    u32 v[16], m[16]; int i;
    for (i = 0; i < 8; i++) { v[i] = ctx->h[i]; v[i + 8] = blake2s_iv[i]; }
    v[12] ^= ctx->t[0]; v[13] ^= ctx->t[1];
    if (last) v[14] = ~v[14];
    for (i = 0; i < 16; i++) m[i] = ld32le(ctx->b + 4 * i);
    for (i = 0; i < 10; i++) {
        const u8* s = blake2_sigma[i];
        B2S_G(0, 4, 8, 12, m[s[0]], m[s[1]]);   B2S_G(1, 5, 9, 13, m[s[2]], m[s[3]]);
        B2S_G(2, 6, 10, 14, m[s[4]], m[s[5]]);  B2S_G(3, 7, 11, 15, m[s[6]], m[s[7]]);
        B2S_G(0, 5, 10, 15, m[s[8]], m[s[9]]);  B2S_G(1, 6, 11, 12, m[s[10]], m[s[11]]);
        B2S_G(2, 7, 8, 13, m[s[12]], m[s[13]]); B2S_G(3, 4, 9, 14, m[s[14]], m[s[15]]);
    }
    for (i = 0; i < 8; i++) ctx->h[i] ^= v[i] ^ v[i + 8];
}

int64_t trc_blake2s(const char* data, int64_t n, const char* key, int64_t klen, char* out, int64_t olen) {
    blake2s_ctx ctx; size_t i; const u8* in = (const u8*)data;
    if (olen <= 0 || olen > 32 || klen < 0 || klen > 32) return -1;
    for (i = 0; i < 8; i++) ctx.h[i] = blake2s_iv[i];
    ctx.h[0] ^= 0x01010000 ^ ((u32)klen << 8) ^ (u32)olen;
    ctx.t[0] = ctx.t[1] = 0; ctx.c = 0; ctx.outlen = (size_t)olen;
    memset(ctx.b, 0, sizeof ctx.b);
    if (klen > 0) { memcpy(ctx.b, key, (size_t)klen); ctx.c = 64; }
    for (i = 0; i < (size_t)n; i++) {
        if (ctx.c == 64) {
            ctx.t[0] += 64; if (ctx.t[0] < 64) ctx.t[1]++;
            blake2s_compress(&ctx, 0); ctx.c = 0;
        }
        ctx.b[ctx.c++] = in[i];
    }
    ctx.t[0] += (u32)ctx.c; if (ctx.t[0] < ctx.c) ctx.t[1]++;
    while (ctx.c < 64) ctx.b[ctx.c++] = 0;
    blake2s_compress(&ctx, 1);
    for (i = 0; i < (size_t)olen; i++) out[i] = (char)(u8)(ctx.h[i >> 2] >> (8 * (i & 3)));
    return olen;
}

/* ═══════════════════════════ BLAKE3 ══════════════════════════════════════ */
#define B3_CHUNK_START 1
#define B3_CHUNK_END 2
#define B3_PARENT 4
#define B3_ROOT 8
#define B3_KEYED_HASH 16
#define B3_DERIVE_KEY_CONTEXT 32
#define B3_DERIVE_KEY_MATERIAL 64
static const u32 b3_iv[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
static const int b3_perm[16] = {2,6,3,10,7,0,4,13,1,11,12,5,9,14,15,8};

static void b3_g(u32* s, int a, int b, int c, int d, u32 mx, u32 my) {
    s[a] = s[a] + s[b] + mx; s[d] = rotr32(s[d] ^ s[a], 16);
    s[c] = s[c] + s[d];      s[b] = rotr32(s[b] ^ s[c], 12);
    s[a] = s[a] + s[b] + my; s[d] = rotr32(s[d] ^ s[a], 8);
    s[c] = s[c] + s[d];      s[b] = rotr32(s[b] ^ s[c], 7);
}
static void b3_compress(const u32 cv[8], const u32 block[16], u64 counter, u32 blen, u32 flags, u32 out[16]) {
    u32 s[16], m[16], t[16]; int r, i;
    for (i = 0; i < 8; i++) s[i] = cv[i];
    s[8] = b3_iv[0]; s[9] = b3_iv[1]; s[10] = b3_iv[2]; s[11] = b3_iv[3];
    s[12] = (u32)counter; s[13] = (u32)(counter >> 32); s[14] = blen; s[15] = flags;
    memcpy(m, block, sizeof m);
    for (r = 0; r < 7; r++) {
        b3_g(s, 0, 4, 8, 12, m[0], m[1]);   b3_g(s, 1, 5, 9, 13, m[2], m[3]);
        b3_g(s, 2, 6, 10, 14, m[4], m[5]);  b3_g(s, 3, 7, 11, 15, m[6], m[7]);
        b3_g(s, 0, 5, 10, 15, m[8], m[9]);  b3_g(s, 1, 6, 11, 12, m[10], m[11]);
        b3_g(s, 2, 7, 8, 13, m[12], m[13]); b3_g(s, 3, 4, 9, 14, m[14], m[15]);
        if (r < 6) { for (i = 0; i < 16; i++) t[i] = m[b3_perm[i]]; memcpy(m, t, sizeof m); }
    }
    for (i = 0; i < 8; i++) { s[i] ^= s[i + 8]; s[i + 8] ^= cv[i]; }
    memcpy(out, s, sizeof s);
}

typedef struct { u32 cv[8]; u32 block[16]; u64 counter; u32 blen; u32 flags; } b3_output;
typedef struct { u32 cv[8]; u64 chunk_counter; u8 block[64]; u32 blen; u32 blocks_done; u32 flags; } b3_chunk;
typedef struct { u32 key[8]; b3_chunk chunk; u32 stack[54][8]; int depth; u32 flags; } b3_hasher;

static void b3_words(const u8* p, u32* w, int n) { int i; for (i = 0; i < n; i++) w[i] = ld32le(p + 4 * i); }
static void b3_chunk_init(b3_chunk* c, const u32 key[8], u64 counter, u32 flags) {
    memcpy(c->cv, key, 32); c->chunk_counter = counter; memset(c->block, 0, 64);
    c->blen = 0; c->blocks_done = 0; c->flags = flags;
}
static size_t b3_chunk_len(const b3_chunk* c) { return 64 * (size_t)c->blocks_done + c->blen; }
static u32 b3_start_flag(const b3_chunk* c) { return c->blocks_done == 0 ? B3_CHUNK_START : 0; }
static void b3_chunk_update(b3_chunk* c, const u8* in, size_t n) {
    while (n > 0) {
        size_t take;
        if (c->blen == 64) {
            u32 w[16], o[16];
            b3_words(c->block, w, 16);
            b3_compress(c->cv, w, c->chunk_counter, 64, c->flags | b3_start_flag(c), o);
            memcpy(c->cv, o, 32);
            c->blocks_done++; memset(c->block, 0, 64); c->blen = 0;
        }
        take = 64 - c->blen; if (take > n) take = n;
        memcpy(c->block + c->blen, in, take); c->blen += (u32)take; in += take; n -= take;
    }
}
static b3_output b3_chunk_output(const b3_chunk* c) {
    b3_output o;
    memcpy(o.cv, c->cv, 32); b3_words(c->block, o.block, 16);
    o.counter = c->chunk_counter; o.blen = c->blen; o.flags = c->flags | b3_start_flag(c) | B3_CHUNK_END;
    return o;
}
static void b3_output_cv(const b3_output* o, u32 cv[8]) {
    u32 t[16]; b3_compress(o->cv, o->block, o->counter, o->blen, o->flags, t); memcpy(cv, t, 32);
}
static b3_output b3_parent(const u32 l[8], const u32 r[8], const u32 key[8], u32 flags) {
    b3_output o;
    memcpy(o.cv, key, 32); memcpy(o.block, l, 32); memcpy(o.block + 8, r, 32);
    o.counter = 0; o.blen = 64; o.flags = flags | B3_PARENT;
    return o;
}
static void b3_init(b3_hasher* h, const u32 key[8], u32 flags) {
    memcpy(h->key, key, 32); b3_chunk_init(&h->chunk, key, 0, flags); h->depth = 0; h->flags = flags;
}
static void b3_update(b3_hasher* h, const u8* in, size_t n) {
    while (n > 0) {
        size_t take;
        if (b3_chunk_len(&h->chunk) == 1024) {
            u32 cv[8]; b3_output o = b3_chunk_output(&h->chunk);
            u64 total = h->chunk.chunk_counter + 1;
            b3_output_cv(&o, cv);
            while ((total & 1) == 0) {
                b3_output p = b3_parent(h->stack[--h->depth], cv, h->key, h->flags);
                b3_output_cv(&p, cv);
                total >>= 1;
            }
            memcpy(h->stack[h->depth++], cv, 32);
            b3_chunk_init(&h->chunk, h->key, h->chunk.chunk_counter + 1, h->flags);
        }
        take = 1024 - b3_chunk_len(&h->chunk); if (take > n) take = n;
        b3_chunk_update(&h->chunk, in, take); in += take; n -= take;
    }
}
static void b3_finalize(const b3_hasher* h, u8* out, size_t n) {
    b3_output o = b3_chunk_output(&h->chunk);
    int rem = h->depth; u64 ctr = 0;
    while (rem > 0) {
        u32 cv[8]; b3_output_cv(&o, cv);
        o = b3_parent(h->stack[--rem], cv, h->key, h->flags);
    }
    while (n > 0) {
        u32 w[16]; u8 blk[64]; size_t take, i;
        b3_compress(o.cv, o.block, ctr++, o.blen, o.flags | B3_ROOT, w);
        for (i = 0; i < 16; i++) st32le(blk + 4 * i, w[i]);
        take = n < 64 ? n : 64;
        memcpy(out, blk, take); out += take; n -= take;
    }
}

/* mode 0 = hash, 1 = keyed (key = 32 bytes), 2 = derive_key (key = context string). */
int64_t trc_blake3(int64_t mode, const char* key, int64_t klen, const char* data, int64_t n, char* out, int64_t olen) {
    b3_hasher h; u32 kw[8];
    if (olen < 0) return -1;
    if (mode == 0) {
        b3_init(&h, b3_iv, 0);
    } else if (mode == 1) {
        if (klen != 32) return -1;
        b3_words((const u8*)key, kw, 8);
        b3_init(&h, kw, B3_KEYED_HASH);
    } else if (mode == 2) {
        u8 ck[32]; b3_hasher ch;
        b3_init(&ch, b3_iv, B3_DERIVE_KEY_CONTEXT);
        b3_update(&ch, (const u8*)key, (size_t)klen);
        b3_finalize(&ch, ck, 32);
        b3_words(ck, kw, 8);
        b3_init(&h, kw, B3_DERIVE_KEY_MATERIAL);
    } else {
        return -1;
    }
    b3_update(&h, (const u8*)data, (size_t)n);
    b3_finalize(&h, (u8*)out, (size_t)olen);
    return olen;
}

/* ═══════════════════════════ X25519 (RFC 7748) ══════════════════════════
 * TweetNaCl-style field arithmetic: 16 limbs of 16 bits in int64. Portable
 * (no __int128) and constant-time (no secret-dependent branches/indexes). */
typedef int64_t gf[16];
static const gf gf_121665 = {0xDB41, 1};

static void car25519(gf o) {
    int i; int64_t c;
    for (i = 0; i < 16; i++) {
        o[i] += ((int64_t)1 << 16);
        c = o[i] >> 16;
        o[(i + 1) * (i < 15)] += c - 1 + 37 * (c - 1) * (i == 15);
        o[i] -= c * ((int64_t)1 << 16);
    }
}
static void sel25519(gf p, gf q, int b) {
    int64_t t, c = ~(int64_t)(b - 1); int i;
    for (i = 0; i < 16; i++) { t = c & (p[i] ^ q[i]); p[i] ^= t; q[i] ^= t; }
}
static void pack25519(u8* o, const gf n) {
    int i, j, b; gf m, t;
    for (i = 0; i < 16; i++) t[i] = n[i];
    car25519(t); car25519(t); car25519(t);
    for (j = 0; j < 2; j++) {
        m[0] = t[0] - 0xffed;
        for (i = 1; i < 15; i++) { m[i] = t[i] - 0xffff - ((m[i - 1] >> 16) & 1); m[i - 1] &= 0xffff; }
        m[15] = t[15] - 0x7fff - ((m[14] >> 16) & 1);
        b = (int)((m[15] >> 16) & 1);
        m[14] &= 0xffff;
        sel25519(t, m, 1 - b);
    }
    for (i = 0; i < 16; i++) { o[2 * i] = (u8)(t[i] & 0xff); o[2 * i + 1] = (u8)(t[i] >> 8); }
}
static void unpack25519(gf o, const u8* n) {
    int i;
    for (i = 0; i < 16; i++) o[i] = n[2 * i] + ((int64_t)n[2 * i + 1] << 8);
    o[15] &= 0x7fff;
}
static void fadd(gf o, const gf a, const gf b) { int i; for (i = 0; i < 16; i++) o[i] = a[i] + b[i]; }
static void fsub(gf o, const gf a, const gf b) { int i; for (i = 0; i < 16; i++) o[i] = a[i] - b[i]; }
static void fmul(gf o, const gf a, const gf b) {
    int64_t t[31]; int i, j;
    for (i = 0; i < 31; i++) t[i] = 0;
    for (i = 0; i < 16; i++) for (j = 0; j < 16; j++) t[i + j] += a[i] * b[j];
    for (i = 0; i < 15; i++) t[i] += 38 * t[i + 16];
    for (i = 0; i < 16; i++) o[i] = t[i];
    car25519(o); car25519(o);
}
static void fsq(gf o, const gf a) { fmul(o, a, a); }
static void finv(gf o, const gf in) {
    gf c; int a;
    for (a = 0; a < 16; a++) c[a] = in[a];
    for (a = 253; a >= 0; a--) { fsq(c, c); if (a != 2 && a != 4) fmul(c, c, in); }
    for (a = 0; a < 16; a++) o[a] = c[a];
}

/* q = scalar * point (both 32 bytes). Returns -1 if the result is all-zero
 * (a low-order peer point -- RFC 7748 section 6.1 says to abort). */
int64_t trc_x25519(char* qout, const char* scalar, const char* point) {
    u8 z[32], acc = 0; gf x, a, b, c, d, e, f; int64_t r; int i;
    u8* q = (u8*)qout;
    for (i = 0; i < 32; i++) z[i] = (u8)scalar[i];
    z[31] = (z[31] & 127) | 64; z[0] &= 248;
    unpack25519(x, (const u8*)point);
    for (i = 0; i < 16; i++) { b[i] = x[i]; d[i] = a[i] = c[i] = 0; }
    a[0] = d[0] = 1;
    for (i = 254; i >= 0; --i) {
        r = (z[i >> 3] >> (i & 7)) & 1;
        sel25519(a, b, (int)r); sel25519(c, d, (int)r);
        fadd(e, a, c); fsub(a, a, c); fadd(c, b, d); fsub(b, b, d);
        fsq(d, e); fsq(f, a); fmul(a, c, a); fmul(c, b, e);
        fadd(e, a, c); fsub(a, a, c); fsq(b, a); fsub(c, d, f);
        fmul(a, c, gf_121665); fadd(a, a, d); fmul(c, c, a); fmul(a, d, f);
        fmul(d, b, x); fsq(b, e);
        sel25519(a, b, (int)r); sel25519(c, d, (int)r);
    }
    finv(c, c); fmul(a, a, c); pack25519(q, a);
    wipe(z, sizeof z);
    for (i = 0; i < 32; i++) acc |= q[i];
    return acc == 0 ? -1 : 0;
}

/* ═══════════════════════════ Argon2 (RFC 9106) ═══════════════════════════ */
#define A2_BLOCK 1024
#define A2_QWORDS 128
#define A2_SYNC 4
typedef struct { u64 v[A2_QWORDS]; } a2_block;

/* H' -- variable-length BLAKE2b (RFC 9106 section 3.3). */
static void a2_hprime(u8* out, size_t outlen, const u8* in, size_t inlen) {
    blake2b_ctx c; u8 lenle[4];
    st32le(lenle, (u32)outlen);
    if (outlen <= 64) {
        if (blake2b_init(&c, outlen, NULL, 0) < 0) return;
        blake2b_update(&c, lenle, 4); blake2b_update(&c, in, inlen);
        blake2b_final(&c, out);
    } else {
        u8 v[64]; size_t done;
        blake2b_init(&c, 64, NULL, 0);
        blake2b_update(&c, lenle, 4); blake2b_update(&c, in, inlen);
        blake2b_final(&c, v);
        memcpy(out, v, 32); done = 32;
        while (outlen - done > 64) {
            blake2b(v, 64, NULL, 0, v, 64);
            memcpy(out + done, v, 32); done += 32;
        }
        blake2b(v, outlen - done, NULL, 0, v, 64);
        memcpy(out + done, v, outlen - done);
    }
}

static u64 a2_fblamka(u64 x, u64 y) { return x + y + 2 * (u64)(u32)x * (u64)(u32)y; }
#define A2_G(a, b, c, d) do { \
    a = a2_fblamka(a, b); d = rotr64(d ^ a, 32); \
    c = a2_fblamka(c, d); b = rotr64(b ^ c, 24); \
    a = a2_fblamka(a, b); d = rotr64(d ^ a, 16); \
    c = a2_fblamka(c, d); b = rotr64(b ^ c, 63); } while (0)
#define A2_ROUND(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15) do { \
    A2_G(v0, v4, v8, v12); A2_G(v1, v5, v9, v13); A2_G(v2, v6, v10, v14); A2_G(v3, v7, v11, v15); \
    A2_G(v0, v5, v10, v15); A2_G(v1, v6, v11, v12); A2_G(v2, v7, v8, v13); A2_G(v3, v4, v9, v14); } while (0)

static void a2_fill(const a2_block* prev, const a2_block* ref, a2_block* next, int with_xor) {
    a2_block R, T; int i;
    for (i = 0; i < A2_QWORDS; i++) R.v[i] = ref->v[i] ^ prev->v[i];
    T = R;
    if (with_xor) for (i = 0; i < A2_QWORDS; i++) T.v[i] ^= next->v[i];
    for (i = 0; i < 8; i++) {
        u64* v = R.v + 16 * i;
        A2_ROUND(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]);
    }
    for (i = 0; i < 8; i++) {
        u64* v = R.v + 2 * i;
        A2_ROUND(v[0], v[1], v[16], v[17], v[32], v[33], v[48], v[49], v[64], v[65], v[80], v[81], v[96], v[97], v[112], v[113]);
    }
    for (i = 0; i < A2_QWORDS; i++) next->v[i] = T.v[i] ^ R.v[i];
}
static void a2_next_addresses(a2_block* addr, a2_block* input, const a2_block* zero) {
    input->v[6]++;
    a2_fill(zero, input, addr, 0);
    a2_fill(zero, addr, addr, 0);
}
static void a2_load(a2_block* b, const u8* p) { int i; for (i = 0; i < A2_QWORDS; i++) b->v[i] = ld64le(p + 8 * i); }
static void a2_store(u8* p, const a2_block* b) { int i; for (i = 0; i < A2_QWORDS; i++) st64le(p + 8 * i, b->v[i]); }

/* type: 0 = Argon2d, 1 = Argon2i, 2 = Argon2id. version 0x13 (or 0x10).
 * Returns 0, or -1 bad parameters, -2 out of memory. */
int64_t trc_argon2(int64_t type, int64_t version, int64_t t_cost, int64_t m_cost, int64_t lanes,
                   const char* pwd, int64_t pwdlen, const char* salt, int64_t saltlen,
                   const char* secret, int64_t secretlen, const char* ad, int64_t adlen,
                   char* out, int64_t outlen) {
    blake2b_ctx hc; u8 h0[72], lenle[4], blockbytes[A2_BLOCK];
    u32 memory_blocks, segment_length, lane_length;
    a2_block* mem; u32 pass, slice, lane;
    if (type < 0 || type > 2 || (version != 0x10 && version != 0x13)) return -1;
    if (t_cost < 1 || lanes < 1 || lanes > 0xFFFFFF || outlen < 4 || saltlen < 8 || m_cost < 8 * lanes) return -1;
    if (pwdlen < 0 || secretlen < 0 || adlen < 0 || m_cost > (int64_t)0x7FFFFFFF) return -1;

    memory_blocks = (u32)m_cost;
    if (memory_blocks < 2 * A2_SYNC * (u32)lanes) memory_blocks = 2 * A2_SYNC * (u32)lanes;
    segment_length = memory_blocks / ((u32)lanes * A2_SYNC);
    memory_blocks = segment_length * (u32)lanes * A2_SYNC;
    lane_length = segment_length * A2_SYNC;

    mem = (a2_block*)calloc(memory_blocks, sizeof(a2_block));
    if (!mem) return -2;

    /* H0 */
    blake2b_init(&hc, 64, NULL, 0);
#define A2_LE(x) do { st32le(lenle, (u32)(x)); blake2b_update(&hc, lenle, 4); } while (0)
    A2_LE(lanes); A2_LE(outlen); A2_LE(m_cost); A2_LE(t_cost); A2_LE(version); A2_LE(type);
    A2_LE(pwdlen); blake2b_update(&hc, (const u8*)pwd, (size_t)pwdlen);
    A2_LE(saltlen); blake2b_update(&hc, (const u8*)salt, (size_t)saltlen);
    A2_LE(secretlen); blake2b_update(&hc, (const u8*)secret, (size_t)secretlen);
    A2_LE(adlen); blake2b_update(&hc, (const u8*)ad, (size_t)adlen);
#undef A2_LE
    blake2b_final(&hc, h0);

    /* First two blocks of each lane. */
    for (lane = 0; lane < (u32)lanes; lane++) {
        st32le(h0 + 64, 0); st32le(h0 + 68, lane);
        a2_hprime(blockbytes, A2_BLOCK, h0, 72);
        a2_load(&mem[lane * lane_length + 0], blockbytes);
        st32le(h0 + 64, 1);
        a2_hprime(blockbytes, A2_BLOCK, h0, 72);
        a2_load(&mem[lane * lane_length + 1], blockbytes);
    }

    for (pass = 0; pass < (u32)t_cost; pass++) {
        for (slice = 0; slice < A2_SYNC; slice++) {
            for (lane = 0; lane < (u32)lanes; lane++) {
                a2_block zero, input, addr;
                int data_indep = (type == 1) || (type == 2 && pass == 0 && slice < A2_SYNC / 2);
                u32 i, start = 0, curr, prev;
                if (data_indep) {
                    memset(&zero, 0, sizeof zero); memset(&input, 0, sizeof input); memset(&addr, 0, sizeof addr);
                    input.v[0] = pass; input.v[1] = lane; input.v[2] = slice;
                    input.v[3] = memory_blocks; input.v[4] = (u64)t_cost; input.v[5] = (u64)type;
                }
                if (pass == 0 && slice == 0) {
                    start = 2;
                    if (data_indep) a2_next_addresses(&addr, &input, &zero);
                }
                curr = lane * lane_length + slice * segment_length + start;
                prev = (curr % lane_length == 0) ? curr + lane_length - 1 : curr - 1;
                for (i = start; i < segment_length; i++, curr++, prev++) {
                    u64 pr, rel; u32 ref_lane, ref_index, area, startpos;
                    int same_lane;
                    if (curr % lane_length == 1) prev = curr - 1;
                    if (data_indep) {
                        if (i % A2_QWORDS == 0) a2_next_addresses(&addr, &input, &zero);
                        pr = addr.v[i % A2_QWORDS];
                    } else {
                        pr = mem[prev].v[0];
                    }
                    ref_lane = (u32)((pr >> 32) % (u64)lanes);
                    if (pass == 0 && slice == 0) ref_lane = lane;
                    same_lane = ref_lane == lane;
                    if (pass == 0) {
                        if (slice == 0) area = i - 1;
                        else if (same_lane) area = slice * segment_length + i - 1;
                        else area = slice * segment_length + (i == 0 ? (u32)-1 : 0);
                    } else {
                        if (same_lane) area = lane_length - segment_length + i - 1;
                        else area = lane_length - segment_length + (i == 0 ? (u32)-1 : 0);
                    }
                    rel = pr & 0xFFFFFFFFULL;
                    rel = (rel * rel) >> 32;
                    rel = area - 1 - (((u64)area * rel) >> 32);
                    startpos = 0;
                    if (pass != 0) startpos = (slice == A2_SYNC - 1) ? 0 : (slice + 1) * segment_length;
                    ref_index = (u32)((startpos + rel) % lane_length);
                    a2_fill(&mem[prev], &mem[lane_length * ref_lane + ref_index], &mem[curr],
                            (version != 0x10 && pass != 0) ? 1 : 0);
                }
            }
        }
    }

    {
        a2_block fin = mem[lane_length - 1]; int k;
        for (lane = 1; lane < (u32)lanes; lane++)
            for (k = 0; k < A2_QWORDS; k++) fin.v[k] ^= mem[lane * lane_length + lane_length - 1].v[k];
        a2_store(blockbytes, &fin);
        a2_hprime((u8*)out, (size_t)outlen, blockbytes, A2_BLOCK);
    }
    wipe(mem, (size_t)memory_blocks * sizeof(a2_block));
    free(mem);
    wipe(blockbytes, sizeof blockbytes); wipe(h0, sizeof h0);
    return 0;
}

/* ═══════════════════════════ bcrypt ══════════════════════════════════════ */
/* Blowfish initial state: the hexadecimal digits of pi's fractional part
 * (P-array = first 18 words, then the four S-boxes). */
static const uint32_t bf_P_init[18] = {
0x243f6a88,0x85a308d3,0x13198a2e,0x03707344,0xa4093822,0x299f31d0,0x082efa98,0xec4e6c89,0x452821e6,0x38d01377,0xbe5466cf,0x34e90c6c,0xc0ac29b7,0xc97c50dd,0x3f84d5b5,0xb5470917,0x9216d5d9,0x8979fb1b};
static const uint32_t bf_S_init[4][256] = {
{
0xd1310ba6,0x98dfb5ac,0x2ffd72db,0xd01adfb7,0xb8e1afed,0x6a267e96,0xba7c9045,0xf12c7f99,
0x24a19947,0xb3916cf7,0x0801f2e2,0x858efc16,0x636920d8,0x71574e69,0xa458fea3,0xf4933d7e,
0x0d95748f,0x728eb658,0x718bcd58,0x82154aee,0x7b54a41d,0xc25a59b5,0x9c30d539,0x2af26013,
0xc5d1b023,0x286085f0,0xca417918,0xb8db38ef,0x8e79dcb0,0x603a180e,0x6c9e0e8b,0xb01e8a3e,
0xd71577c1,0xbd314b27,0x78af2fda,0x55605c60,0xe65525f3,0xaa55ab94,0x57489862,0x63e81440,
0x55ca396a,0x2aab10b6,0xb4cc5c34,0x1141e8ce,0xa15486af,0x7c72e993,0xb3ee1411,0x636fbc2a,
0x2ba9c55d,0x741831f6,0xce5c3e16,0x9b87931e,0xafd6ba33,0x6c24cf5c,0x7a325381,0x28958677,
0x3b8f4898,0x6b4bb9af,0xc4bfe81b,0x66282193,0x61d809cc,0xfb21a991,0x487cac60,0x5dec8032,
0xef845d5d,0xe98575b1,0xdc262302,0xeb651b88,0x23893e81,0xd396acc5,0x0f6d6ff3,0x83f44239,
0x2e0b4482,0xa4842004,0x69c8f04a,0x9e1f9b5e,0x21c66842,0xf6e96c9a,0x670c9c61,0xabd388f0,
0x6a51a0d2,0xd8542f68,0x960fa728,0xab5133a3,0x6eef0b6c,0x137a3be4,0xba3bf050,0x7efb2a98,
0xa1f1651d,0x39af0176,0x66ca593e,0x82430e88,0x8cee8619,0x456f9fb4,0x7d84a5c3,0x3b8b5ebe,
0xe06f75d8,0x85c12073,0x401a449f,0x56c16aa6,0x4ed3aa62,0x363f7706,0x1bfedf72,0x429b023d,
0x37d0d724,0xd00a1248,0xdb0fead3,0x49f1c09b,0x075372c9,0x80991b7b,0x25d479d8,0xf6e8def7,
0xe3fe501a,0xb6794c3b,0x976ce0bd,0x04c006ba,0xc1a94fb6,0x409f60c4,0x5e5c9ec2,0x196a2463,
0x68fb6faf,0x3e6c53b5,0x1339b2eb,0x3b52ec6f,0x6dfc511f,0x9b30952c,0xcc814544,0xaf5ebd09,
0xbee3d004,0xde334afd,0x660f2807,0x192e4bb3,0xc0cba857,0x45c8740f,0xd20b5f39,0xb9d3fbdb,
0x5579c0bd,0x1a60320a,0xd6a100c6,0x402c7279,0x679f25fe,0xfb1fa3cc,0x8ea5e9f8,0xdb3222f8,
0x3c7516df,0xfd616b15,0x2f501ec8,0xad0552ab,0x323db5fa,0xfd238760,0x53317b48,0x3e00df82,
0x9e5c57bb,0xca6f8ca0,0x1a87562e,0xdf1769db,0xd542a8f6,0x287effc3,0xac6732c6,0x8c4f5573,
0x695b27b0,0xbbca58c8,0xe1ffa35d,0xb8f011a0,0x10fa3d98,0xfd2183b8,0x4afcb56c,0x2dd1d35b,
0x9a53e479,0xb6f84565,0xd28e49bc,0x4bfb9790,0xe1ddf2da,0xa4cb7e33,0x62fb1341,0xcee4c6e8,
0xef20cada,0x36774c01,0xd07e9efe,0x2bf11fb4,0x95dbda4d,0xae909198,0xeaad8e71,0x6b93d5a0,
0xd08ed1d0,0xafc725e0,0x8e3c5b2f,0x8e7594b7,0x8ff6e2fb,0xf2122b64,0x8888b812,0x900df01c,
0x4fad5ea0,0x688fc31c,0xd1cff191,0xb3a8c1ad,0x2f2f2218,0xbe0e1777,0xea752dfe,0x8b021fa1,
0xe5a0cc0f,0xb56f74e8,0x18acf3d6,0xce89e299,0xb4a84fe0,0xfd13e0b7,0x7cc43b81,0xd2ada8d9,
0x165fa266,0x80957705,0x93cc7314,0x211a1477,0xe6ad2065,0x77b5fa86,0xc75442f5,0xfb9d35cf,
0xebcdaf0c,0x7b3e89a0,0xd6411bd3,0xae1e7e49,0x00250e2d,0x2071b35e,0x226800bb,0x57b8e0af,
0x2464369b,0xf009b91e,0x5563911d,0x59dfa6aa,0x78c14389,0xd95a537f,0x207d5ba2,0x02e5b9c5,
0x83260376,0x6295cfa9,0x11c81968,0x4e734a41,0xb3472dca,0x7b14a94a,0x1b510052,0x9a532915,
0xd60f573f,0xbc9bc6e4,0x2b60a476,0x81e67400,0x08ba6fb5,0x571be91f,0xf296ec6b,0x2a0dd915,
0xb6636521,0xe7b9f9b6,0xff34052e,0xc5855664,0x53b02d5d,0xa99f8fa1,0x08ba4799,0x6e85076a,
},
{
0x4b7a70e9,0xb5b32944,0xdb75092e,0xc4192623,0xad6ea6b0,0x49a7df7d,0x9cee60b8,0x8fedb266,
0xecaa8c71,0x699a17ff,0x5664526c,0xc2b19ee1,0x193602a5,0x75094c29,0xa0591340,0xe4183a3e,
0x3f54989a,0x5b429d65,0x6b8fe4d6,0x99f73fd6,0xa1d29c07,0xefe830f5,0x4d2d38e6,0xf0255dc1,
0x4cdd2086,0x8470eb26,0x6382e9c6,0x021ecc5e,0x09686b3f,0x3ebaefc9,0x3c971814,0x6b6a70a1,
0x687f3584,0x52a0e286,0xb79c5305,0xaa500737,0x3e07841c,0x7fdeae5c,0x8e7d44ec,0x5716f2b8,
0xb03ada37,0xf0500c0d,0xf01c1f04,0x0200b3ff,0xae0cf51a,0x3cb574b2,0x25837a58,0xdc0921bd,
0xd19113f9,0x7ca92ff6,0x94324773,0x22f54701,0x3ae5e581,0x37c2dadc,0xc8b57634,0x9af3dda7,
0xa9446146,0x0fd0030e,0xecc8c73e,0xa4751e41,0xe238cd99,0x3bea0e2f,0x3280bba1,0x183eb331,
0x4e548b38,0x4f6db908,0x6f420d03,0xf60a04bf,0x2cb81290,0x24977c79,0x5679b072,0xbcaf89af,
0xde9a771f,0xd9930810,0xb38bae12,0xdccf3f2e,0x5512721f,0x2e6b7124,0x501adde6,0x9f84cd87,
0x7a584718,0x7408da17,0xbc9f9abc,0xe94b7d8c,0xec7aec3a,0xdb851dfa,0x63094366,0xc464c3d2,
0xef1c1847,0x3215d908,0xdd433b37,0x24c2ba16,0x12a14d43,0x2a65c451,0x50940002,0x133ae4dd,
0x71dff89e,0x10314e55,0x81ac77d6,0x5f11199b,0x043556f1,0xd7a3c76b,0x3c11183b,0x5924a509,
0xf28fe6ed,0x97f1fbfa,0x9ebabf2c,0x1e153c6e,0x86e34570,0xeae96fb1,0x860e5e0a,0x5a3e2ab3,
0x771fe71c,0x4e3d06fa,0x2965dcb9,0x99e71d0f,0x803e89d6,0x5266c825,0x2e4cc978,0x9c10b36a,
0xc6150eba,0x94e2ea78,0xa5fc3c53,0x1e0a2df4,0xf2f74ea7,0x361d2b3d,0x1939260f,0x19c27960,
0x5223a708,0xf71312b6,0xebadfe6e,0xeac31f66,0xe3bc4595,0xa67bc883,0xb17f37d1,0x018cff28,
0xc332ddef,0xbe6c5aa5,0x65582185,0x68ab9802,0xeecea50f,0xdb2f953b,0x2aef7dad,0x5b6e2f84,
0x1521b628,0x29076170,0xecdd4775,0x619f1510,0x13cca830,0xeb61bd96,0x0334fe1e,0xaa0363cf,
0xb5735c90,0x4c70a239,0xd59e9e0b,0xcbaade14,0xeecc86bc,0x60622ca7,0x9cab5cab,0xb2f3846e,
0x648b1eaf,0x19bdf0ca,0xa02369b9,0x655abb50,0x40685a32,0x3c2ab4b3,0x319ee9d5,0xc021b8f7,
0x9b540b19,0x875fa099,0x95f7997e,0x623d7da8,0xf837889a,0x97e32d77,0x11ed935f,0x16681281,
0x0e358829,0xc7e61fd6,0x96dedfa1,0x7858ba99,0x57f584a5,0x1b227263,0x9b83c3ff,0x1ac24696,
0xcdb30aeb,0x532e3054,0x8fd948e4,0x6dbc3128,0x58ebf2ef,0x34c6ffea,0xfe28ed61,0xee7c3c73,
0x5d4a14d9,0xe864b7e3,0x42105d14,0x203e13e0,0x45eee2b6,0xa3aaabea,0xdb6c4f15,0xfacb4fd0,
0xc742f442,0xef6abbb5,0x654f3b1d,0x41cd2105,0xd81e799e,0x86854dc7,0xe44b476a,0x3d816250,
0xcf62a1f2,0x5b8d2646,0xfc8883a0,0xc1c7b6a3,0x7f1524c3,0x69cb7492,0x47848a0b,0x5692b285,
0x095bbf00,0xad19489d,0x1462b174,0x23820e00,0x58428d2a,0x0c55f5ea,0x1dadf43e,0x233f7061,
0x3372f092,0x8d937e41,0xd65fecf1,0x6c223bdb,0x7cde3759,0xcbee7460,0x4085f2a7,0xce77326e,
0xa6078084,0x19f8509e,0xe8efd855,0x61d99735,0xa969a7aa,0xc50c06c2,0x5a04abfc,0x800bcadc,
0x9e447a2e,0xc3453484,0xfdd56705,0x0e1e9ec9,0xdb73dbd3,0x105588cd,0x675fda79,0xe3674340,
0xc5c43465,0x713e38d8,0x3d28f89e,0xf16dff20,0x153e21e7,0x8fb03d4a,0xe6e39f2b,0xdb83adf7,
},
{
0xe93d5a68,0x948140f7,0xf64c261c,0x94692934,0x411520f7,0x7602d4f7,0xbcf46b2e,0xd4a20068,
0xd4082471,0x3320f46a,0x43b7d4b7,0x500061af,0x1e39f62e,0x97244546,0x14214f74,0xbf8b8840,
0x4d95fc1d,0x96b591af,0x70f4ddd3,0x66a02f45,0xbfbc09ec,0x03bd9785,0x7fac6dd0,0x31cb8504,
0x96eb27b3,0x55fd3941,0xda2547e6,0xabca0a9a,0x28507825,0x530429f4,0x0a2c86da,0xe9b66dfb,
0x68dc1462,0xd7486900,0x680ec0a4,0x27a18dee,0x4f3ffea2,0xe887ad8c,0xb58ce006,0x7af4d6b6,
0xaace1e7c,0xd3375fec,0xce78a399,0x406b2a42,0x20fe9e35,0xd9f385b9,0xee39d7ab,0x3b124e8b,
0x1dc9faf7,0x4b6d1856,0x26a36631,0xeae397b2,0x3a6efa74,0xdd5b4332,0x6841e7f7,0xca7820fb,
0xfb0af54e,0xd8feb397,0x454056ac,0xba489527,0x55533a3a,0x20838d87,0xfe6ba9b7,0xd096954b,
0x55a867bc,0xa1159a58,0xcca92963,0x99e1db33,0xa62a4a56,0x3f3125f9,0x5ef47e1c,0x9029317c,
0xfdf8e802,0x04272f70,0x80bb155c,0x05282ce3,0x95c11548,0xe4c66d22,0x48c1133f,0xc70f86dc,
0x07f9c9ee,0x41041f0f,0x404779a4,0x5d886e17,0x325f51eb,0xd59bc0d1,0xf2bcc18f,0x41113564,
0x257b7834,0x602a9c60,0xdff8e8a3,0x1f636c1b,0x0e12b4c2,0x02e1329e,0xaf664fd1,0xcad18115,
0x6b2395e0,0x333e92e1,0x3b240b62,0xeebeb922,0x85b2a20e,0xe6ba0d99,0xde720c8c,0x2da2f728,
0xd0127845,0x95b794fd,0x647d0862,0xe7ccf5f0,0x5449a36f,0x877d48fa,0xc39dfd27,0xf33e8d1e,
0x0a476341,0x992eff74,0x3a6f6eab,0xf4f8fd37,0xa812dc60,0xa1ebddf8,0x991be14c,0xdb6e6b0d,
0xc67b5510,0x6d672c37,0x2765d43b,0xdcd0e804,0xf1290dc7,0xcc00ffa3,0xb5390f92,0x690fed0b,
0x667b9ffb,0xcedb7d9c,0xa091cf0b,0xd9155ea3,0xbb132f88,0x515bad24,0x7b9479bf,0x763bd6eb,
0x37392eb3,0xcc115979,0x8026e297,0xf42e312d,0x6842ada7,0xc66a2b3b,0x12754ccc,0x782ef11c,
0x6a124237,0xb79251e7,0x06a1bbe6,0x4bfb6350,0x1a6b1018,0x11caedfa,0x3d25bdd8,0xe2e1c3c9,
0x44421659,0x0a121386,0xd90cec6e,0xd5abea2a,0x64af674e,0xda86a85f,0xbebfe988,0x64e4c3fe,
0x9dbc8057,0xf0f7c086,0x60787bf8,0x6003604d,0xd1fd8346,0xf6381fb0,0x7745ae04,0xd736fccc,
0x83426b33,0xf01eab71,0xb0804187,0x3c005e5f,0x77a057be,0xbde8ae24,0x55464299,0xbf582e61,
0x4e58f48f,0xf2ddfda2,0xf474ef38,0x8789bdc2,0x5366f9c3,0xc8b38e74,0xb475f255,0x46fcd9b9,
0x7aeb2661,0x8b1ddf84,0x846a0e79,0x915f95e2,0x466e598e,0x20b45770,0x8cd55591,0xc902de4c,
0xb90bace1,0xbb8205d0,0x11a86248,0x7574a99e,0xb77f19b6,0xe0a9dc09,0x662d09a1,0xc4324633,
0xe85a1f02,0x09f0be8c,0x4a99a025,0x1d6efe10,0x1ab93d1d,0x0ba5a4df,0xa186f20f,0x2868f169,
0xdcb7da83,0x573906fe,0xa1e2ce9b,0x4fcd7f52,0x50115e01,0xa70683fa,0xa002b5c4,0x0de6d027,
0x9af88c27,0x773f8641,0xc3604c06,0x61a806b5,0xf0177a28,0xc0f586e0,0x006058aa,0x30dc7d62,
0x11e69ed7,0x2338ea63,0x53c2dd94,0xc2c21634,0xbbcbee56,0x90bcb6de,0xebfc7da1,0xce591d76,
0x6f05e409,0x4b7c0188,0x39720a3d,0x7c927c24,0x86e3725f,0x724d9db9,0x1ac15bb4,0xd39eb8fc,
0xed545578,0x08fca5b5,0xd83d7cd3,0x4dad0fc4,0x1e50ef5e,0xb161e6f8,0xa28514d9,0x6c51133c,
0x6fd5c7e7,0x56e14ec4,0x362abfce,0xddc6c837,0xd79a3234,0x92638212,0x670efa8e,0x406000e0,
},
{
0x3a39ce37,0xd3faf5cf,0xabc27737,0x5ac52d1b,0x5cb0679e,0x4fa33742,0xd3822740,0x99bc9bbe,
0xd5118e9d,0xbf0f7315,0xd62d1c7e,0xc700c47b,0xb78c1b6b,0x21a19045,0xb26eb1be,0x6a366eb4,
0x5748ab2f,0xbc946e79,0xc6a376d2,0x6549c2c8,0x530ff8ee,0x468dde7d,0xd5730a1d,0x4cd04dc6,
0x2939bbdb,0xa9ba4650,0xac9526e8,0xbe5ee304,0xa1fad5f0,0x6a2d519a,0x63ef8ce2,0x9a86ee22,
0xc089c2b8,0x43242ef6,0xa51e03aa,0x9cf2d0a4,0x83c061ba,0x9be96a4d,0x8fe51550,0xba645bd6,
0x2826a2f9,0xa73a3ae1,0x4ba99586,0xef5562e9,0xc72fefd3,0xf752f7da,0x3f046f69,0x77fa0a59,
0x80e4a915,0x87b08601,0x9b09e6ad,0x3b3ee593,0xe990fd5a,0x9e34d797,0x2cf0b7d9,0x022b8b51,
0x96d5ac3a,0x017da67d,0xd1cf3ed6,0x7c7d2d28,0x1f9f25cf,0xadf2b89b,0x5ad6b472,0x5a88f54c,
0xe029ac71,0xe019a5e6,0x47b0acfd,0xed93fa9b,0xe8d3c48d,0x283b57cc,0xf8d56629,0x79132e28,
0x785f0191,0xed756055,0xf7960e44,0xe3d35e8c,0x15056dd4,0x88f46dba,0x03a16125,0x0564f0bd,
0xc3eb9e15,0x3c9057a2,0x97271aec,0xa93a072a,0x1b3f6d9b,0x1e6321f5,0xf59c66fb,0x26dcf319,
0x7533d928,0xb155fdf5,0x03563482,0x8aba3cbb,0x28517711,0xc20ad9f8,0xabcc5167,0xccad925f,
0x4de81751,0x3830dc8e,0x379d5862,0x9320f991,0xea7a90c2,0xfb3e7bce,0x5121ce64,0x774fbe32,
0xa8b6e37e,0xc3293d46,0x48de5369,0x6413e680,0xa2ae0810,0xdd6db224,0x69852dfd,0x09072166,
0xb39a460a,0x6445c0dd,0x586cdecf,0x1c20c8ae,0x5bbef7dd,0x1b588d40,0xccd2017f,0x6bb4e3bb,
0xdda26a7e,0x3a59ff45,0x3e350a44,0xbcb4cdd5,0x72eacea8,0xfa6484bb,0x8d6612ae,0xbf3c6f47,
0xd29be463,0x542f5d9e,0xaec2771b,0xf64e6370,0x740e0d8d,0xe75b1357,0xf8721671,0xaf537d5d,
0x4040cb08,0x4eb4e2cc,0x34d2466a,0x0115af84,0xe1b00428,0x95983a1d,0x06b89fb4,0xce6ea048,
0x6f3f3b82,0x3520ab82,0x011a1d4b,0x277227f8,0x611560b1,0xe7933fdc,0xbb3a792b,0x344525bd,
0xa08839e1,0x51ce794b,0x2f32c9b7,0xa01fbac9,0xe01cc87e,0xbcc7d1f6,0xcf0111c3,0xa1e8aac7,
0x1a908749,0xd44fbd9a,0xd0dadecb,0xd50ada38,0x0339c32a,0xc6913667,0x8df9317c,0xe0b12b4f,
0xf79e59b7,0x43f5bb3a,0xf2d519ff,0x27d9459c,0xbf97222c,0x15e6fc2a,0x0f91fc71,0x9b941525,
0xfae59361,0xceb69ceb,0xc2a86459,0x12baa8d1,0xb6c1075e,0xe3056a0c,0x10d25065,0xcb03a442,
0xe0ec6e0e,0x1698db3b,0x4c98a0be,0x3278e964,0x9f1f9532,0xe0d392df,0xd3a0342b,0x8971f21e,
0x1b0a7441,0x4ba3348c,0xc5be7120,0xc37632d8,0xdf359f8d,0x9b992f2e,0xe60b6f47,0x0fe3f11d,
0xe54cda54,0x1edad891,0xce6279cf,0xcd3e7e6f,0x1618b166,0xfd2c1d05,0x848fd2c5,0xf6fb2299,
0xf523f357,0xa6327623,0x93a83531,0x56cccd02,0xacf08162,0x5a75ebb5,0x6e163697,0x88d273cc,
0xde966292,0x81b949d0,0x4c50901b,0x71c65614,0xe6c6c7bd,0x327a140a,0x45e1d006,0xc3f27b9a,
0xc9aa53fd,0x62a80f00,0xbb25bfe2,0x35bdd2f6,0x71126905,0xb2040222,0xb6cbcf7c,0xcd769c2b,
0x53113ec0,0x1640e3d3,0x38abbd60,0x2547adf0,0xba38209c,0xf746ce76,0x77afa1c5,0x20756060,
0x85cbfe4e,0x8ae88dd8,0x7aaaf9b0,0x4cf9aa7e,0x1948c25c,0x02fb8a8c,0x01c36ae4,0xd6ebe1f9,
0x90d4f869,0xa65cdea0,0x3f09252d,0xc208e69f,0xb74e6132,0xce77e25b,0x578fdfe3,0x3ac372e6,
},
};

typedef struct { u32 P[18]; u32 S[4][256]; } bf_ctx;

static u32 bf_F(const bf_ctx* c, u32 x) {
    return ((c->S[0][x >> 24] + c->S[1][(x >> 16) & 0xff]) ^ c->S[2][(x >> 8) & 0xff]) + c->S[3][x & 0xff];
}
static void bf_encrypt(const bf_ctx* c, u32* xl, u32* xr) {
    u32 L = *xl, R = *xr; int i;
    for (i = 0; i < 16; i += 2) {
        L ^= c->P[i]; R ^= bf_F(c, L);
        R ^= c->P[i + 1]; L ^= bf_F(c, R);
    }
    L ^= c->P[16]; R ^= c->P[17];
    *xl = R; *xr = L;
}
/* Next 32-bit word from a cyclic byte stream. */
static u32 bf_stream(const u8* data, size_t len, size_t* j) {
    u32 w = 0; int i;
    for (i = 0; i < 4; i++) { w = (w << 8) | data[*j]; *j = (*j + 1) % len; }
    return w;
}
static void bf_expand(bf_ctx* c, const u8* salt, size_t slen, const u8* key, size_t klen) {
    size_t j = 0; u32 L = 0, R = 0; int i, k;
    for (i = 0; i < 18; i++) c->P[i] ^= bf_stream(key, klen, &j);
    j = 0;
    for (i = 0; i < 18; i += 2) {
        if (slen) { L ^= bf_stream(salt, slen, &j); R ^= bf_stream(salt, slen, &j); }
        bf_encrypt(c, &L, &R); c->P[i] = L; c->P[i + 1] = R;
    }
    for (i = 0; i < 4; i++)
        for (k = 0; k < 256; k += 2) {
            if (slen) { L ^= bf_stream(salt, slen, &j); R ^= bf_stream(salt, slen, &j); }
            bf_encrypt(c, &L, &R); c->S[i][k] = L; c->S[i][k + 1] = R;
        }
}

/* Raw EksBlowfish: key = password bytes (caller appends the NUL and caps at
 * 72), salt = 16 bytes, cost 4..31. Writes 24 bytes of ciphertext (bcrypt
 * encodes only the first 23). Returns 0 or -1. */
int64_t trc_bcrypt_raw(const char* key, int64_t klen, const char* salt16, int64_t cost, char* out24) {
    static const u8 ctext[25] = "OrpheanBeholderScryDoubt";
    bf_ctx* c; u32 cdata[6]; u64 rounds, r; int i, k;
    if (cost < 4 || cost > 31 || klen < 1 || klen > 72) return -1;
    c = (bf_ctx*)malloc(sizeof(bf_ctx));
    if (!c) return -1;
    memcpy(c->P, bf_P_init, sizeof c->P); memcpy(c->S, bf_S_init, sizeof c->S);
    bf_expand(c, (const u8*)salt16, 16, (const u8*)key, (size_t)klen);
    rounds = (u64)1 << cost;
    for (r = 0; r < rounds; r++) {
        bf_expand(c, NULL, 0, (const u8*)key, (size_t)klen);
        bf_expand(c, NULL, 0, (const u8*)salt16, 16);
    }
    for (i = 0; i < 6; i++) cdata[i] = ld32be(ctext + 4 * i);
    for (k = 0; k < 64; k++)
        for (i = 0; i < 6; i += 2) bf_encrypt(c, &cdata[i], &cdata[i + 1]);
    for (i = 0; i < 6; i++) st32be((u8*)out24 + 4 * i, cdata[i]);
    wipe(c, sizeof *c); free(c);
    return 0;
}

/* ═══════════════════════ streaming BLAKE3 ════════════════════════════════ */
int64_t trc_blake3_state_size(void) { return (int64_t)sizeof(b3_hasher); }
int64_t trc_blake3_begin(char* st, int64_t mode, const char* key, int64_t klen) {
    b3_hasher* h = (b3_hasher*)st; u32 kw[8];
    if (mode == 0) { b3_init(h, b3_iv, 0); return 0; }
    if (mode == 1) {
        if (klen != 32) return -1;
        b3_words((const u8*)key, kw, 8); b3_init(h, kw, B3_KEYED_HASH); return 0;
    }
    if (mode == 2) {
        u8 ck[32]; b3_hasher ch;
        b3_init(&ch, b3_iv, B3_DERIVE_KEY_CONTEXT);
        b3_update(&ch, (const u8*)key, (size_t)klen);
        b3_finalize(&ch, ck, 32);
        b3_words(ck, kw, 8); b3_init(h, kw, B3_DERIVE_KEY_MATERIAL); return 0;
    }
    return -1;
}
void trc_blake3_update(char* st, const char* data, int64_t n) { b3_update((b3_hasher*)st, (const u8*)data, (size_t)n); }
/* Output can be read any number of times (finalize does not consume). */
void trc_blake3_finish(char* st, char* out, int64_t n) { b3_finalize((const b3_hasher*)st, (u8*)out, (size_t)n); }

/* ═══════════════════════ PBKDF2-HMAC (RFC 8018) ══════════════════════════ */
int64_t trc_pbkdf2(int64_t alg, const char* pwd, int64_t plen, const char* salt, int64_t slen,
                   int64_t iterations, char* out, int64_t dklen) {
    hmac_ctx base, h; u8 u[64], t[64], ctr[4]; int sz = md_size((int)alg);
    int64_t done = 0, it; u32 block = 1; int i;
    if (sz < 0 || iterations < 1 || dklen < 0) return -1;
    hmac_init(&base, (int)alg, (const u8*)pwd, (size_t)plen);
    while (done < dklen) {
        int64_t take;
        h = base;
        md_update(&h.inner, (const u8*)salt, (size_t)slen);
        st32be(ctr, block); md_update(&h.inner, ctr, 4);
        hmac_final(&h, u);
        memcpy(t, u, (size_t)sz);
        for (it = 1; it < iterations; it++) {
            h = base;
            md_update(&h.inner, u, (size_t)sz);
            hmac_final(&h, u);
            for (i = 0; i < sz; i++) t[i] ^= u[i];
        }
        take = dklen - done < sz ? dklen - done : sz;
        memcpy(out + done, t, (size_t)take);
        done += take; block++;
    }
    wipe(&base, sizeof base); wipe(&h, sizeof h); wipe(u, sizeof u); wipe(t, sizeof t);
    return dklen;
}

/* ═══════════════════════ hex / base64 helpers ════════════════════════════ */
int64_t trc_hex_encode(const char* d, int64_t n, char* out) {
    static const char hx[] = "0123456789abcdef"; int64_t i;
    for (i = 0; i < n; i++) { out[2 * i] = hx[((u8)d[i]) >> 4]; out[2 * i + 1] = hx[((u8)d[i]) & 15]; }
    return 2 * n;
}
static int hexval(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
/* Returns the byte count, or -1 on odd length / bad digit. */
int64_t trc_hex_decode(const char* h, int64_t n, char* out) {
    int64_t i;
    if (n % 2) return -1;
    for (i = 0; i < n / 2; i++) {
        int a = hexval((u8)h[2 * i]), b = hexval((u8)h[2 * i + 1]);
        if (a < 0 || b < 0) return -1;
        out[i] = (char)((a << 4) | b);
    }
    return n / 2;
}

/* kind: 0 = standard (RFC 4648 section 4), 1 = URL-safe (section 5),
 * 2 = bcrypt ("./A-Za-z0-9", never padded). */
static const char* b64_alpha(int kind) {
    if (kind == 1) return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    if (kind == 2) return "./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}
/* out needs 4*ceil(n/3) bytes. Returns the encoded length. */
int64_t trc_b64_encode(int64_t kind, int64_t pad, const char* d, int64_t n, char* out) {
    const char* a = b64_alpha((int)kind); const u8* p = (const u8*)d; int64_t i = 0, o = 0;
    while (n - i >= 3) {
        u32 v = ((u32)p[i] << 16) | ((u32)p[i + 1] << 8) | p[i + 2];
        out[o++] = a[v >> 18]; out[o++] = a[(v >> 12) & 63]; out[o++] = a[(v >> 6) & 63]; out[o++] = a[v & 63];
        i += 3;
    }
    if (n - i == 1) {
        u32 v = (u32)p[i] << 16;
        out[o++] = a[v >> 18]; out[o++] = a[(v >> 12) & 63];
        if (pad && kind != 2) { out[o++] = '='; out[o++] = '='; }
    } else if (n - i == 2) {
        u32 v = ((u32)p[i] << 16) | ((u32)p[i + 1] << 8);
        out[o++] = a[v >> 18]; out[o++] = a[(v >> 12) & 63]; out[o++] = a[(v >> 6) & 63];
        if (pad && kind != 2) out[o++] = '=';
    }
    return o;
}
/* Strict decoder: padding optional, but if present it must be exact; the
 * unused low bits of the last character must be zero. Returns the byte
 * count or -1. out needs 3*ceil(n/4) bytes. */
int64_t trc_b64_decode(int64_t kind, const char* s, int64_t n, char* out) {
    const char* a = b64_alpha((int)kind); int8_t rev[256]; int64_t i, o = 0, end = n, pads;
    u32 acc = 0; int bits = 0;
    memset(rev, -1, sizeof rev);
    for (i = 0; i < 64; i++) rev[(u8)a[i]] = (int8_t)i;
    while (end > 0 && s[end - 1] == '=') end--;
    pads = n - end;
    if (pads > 2 || (pads > 0 && (n % 4) != 0) || (pads > 0 && kind == 2)) return -1;
    if (end % 4 == 1) return -1;
    for (i = 0; i < end; i++) {
        int v = rev[(u8)s[i]];
        if (v < 0) return -1;
        acc = (acc << 6) | (u32)v; bits += 6;
        if (bits >= 8) { bits -= 8; out[o++] = (char)(u8)(acc >> bits); acc &= (1u << bits) - 1; }
    }
    if (acc != 0 && kind != 2) return -1;   /* bcrypt salts: ignore spare bits */
    return o;
}

static u32 aead_be32(const u8* p) { return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3]; }
static void aead_put_be32(u8* p, u32 v) { p[0] = (u8)(v >> 24); p[1] = (u8)(v >> 16); p[2] = (u8)(v >> 8); p[3] = (u8)v; }
static void aead_put_le32(u8* p, u32 v) { p[0] = (u8)v; p[1] = (u8)(v >> 8); p[2] = (u8)(v >> 16); p[3] = (u8)(v >> 24); }
/* ═══════════════════════════ AES (encryption only) ═══════════════════════ */
static const u8 aes_sbox[256] = {
0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};

typedef struct { u8 rk[240]; int nr; } aes_ctx;

static void aes_init(aes_ctx* a, const u8* key, int klen) {
    int nk = klen / 4, i; u8 t[4];
    a->nr = nk + 6;
    memcpy(a->rk, key, (size_t)klen);
    { u8 rcon = 1;
      for (i = nk; i < 4 * (a->nr + 1); i++) {
        memcpy(t, a->rk + 4 * (i - 1), 4);
        if (i % nk == 0) {
            u8 x = t[0]; t[0] = aes_sbox[t[1]] ^ rcon; t[1] = aes_sbox[t[2]]; t[2] = aes_sbox[t[3]]; t[3] = aes_sbox[x];
            rcon = (u8)((rcon << 1) ^ ((rcon & 0x80) ? 0x1b : 0));
        } else if (nk > 6 && i % nk == 4) {
            t[0] = aes_sbox[t[0]]; t[1] = aes_sbox[t[1]]; t[2] = aes_sbox[t[2]]; t[3] = aes_sbox[t[3]];
        }
        a->rk[4 * i] = a->rk[4 * (i - nk)] ^ t[0]; a->rk[4 * i + 1] = a->rk[4 * (i - nk) + 1] ^ t[1];
        a->rk[4 * i + 2] = a->rk[4 * (i - nk) + 2] ^ t[2]; a->rk[4 * i + 3] = a->rk[4 * (i - nk) + 3] ^ t[3];
      } }
}
static u8 xt(u8 x) { return (u8)((x << 1) ^ ((x & 0x80) ? 0x1b : 0)); }
static void aes_enc(const aes_ctx* a, const u8* in, u8* out) {
    u8 s[16], t[16]; int r, i;
    for (i = 0; i < 16; i++) s[i] = in[i] ^ a->rk[i];
    for (r = 1; r <= a->nr; r++) {
        for (i = 0; i < 16; i++) t[i] = aes_sbox[s[(i + 4 * (i % 4)) % 16]];   /* SubBytes + ShiftRows */
        if (r != a->nr) {
            for (i = 0; i < 4; i++) {
                u8* c = t + 4 * i; u8 a0 = c[0], a1 = c[1], a2 = c[2], a3 = c[3], all = a0 ^ a1 ^ a2 ^ a3;
                c[0] ^= all ^ xt(a0 ^ a1); c[1] ^= all ^ xt(a1 ^ a2); c[2] ^= all ^ xt(a2 ^ a3); c[3] ^= all ^ xt(a3 ^ a0);
            }
        }
        for (i = 0; i < 16; i++) s[i] = t[i] ^ a->rk[16 * r + i];
    }
    memcpy(out, s, 16);
}

/* ═══════════════════════════ GCM ═════════════════════════════════════════ */
typedef struct { aes_ctx aes; u64 hh, hl; u64 th[16], tl[16]; } gcm_ctx;

static void gcm_init(gcm_ctx* g, const u8* key, int klen) {
    u8 z[16] = {0}, h[16]; int i;
    aes_init(&g->aes, key, klen);
    aes_enc(&g->aes, z, h);
    g->hh = ((u64)aead_be32(h) << 32) | aead_be32(h + 4); g->hl = ((u64)aead_be32(h + 8) << 32) | aead_be32(h + 12);
    /* Shoup 4-bit table: th/tl[i] = i * H (bit-reflected nibble index) */
    g->th[0] = 0; g->tl[0] = 0;
    g->th[8] = g->hh; g->tl[8] = g->hl;
    for (i = 4; i > 0; i >>= 1) {
        u64 vh = g->th[2 * i], vl = g->tl[2 * i], carry = vl & 1;
        vl = (vh << 63) | (vl >> 1); vh = (vh >> 1) ^ (carry ? 0xe100000000000000ULL : 0);
        g->th[i] = vh; g->tl[i] = vl;
    }
    for (i = 2; i < 16; i <<= 1) {
        int j;
        for (j = 1; j < i; j++) { g->th[i + j] = g->th[i] ^ g->th[j]; g->tl[i + j] = g->tl[i] ^ g->tl[j]; }
    }
}
static const u64 gcm_last4[16] = {
    0x0000, 0x1c20, 0x3840, 0x2460, 0x7080, 0x6ca0, 0x48c0, 0x54e0,
    0xe100, 0xfd20, 0xd940, 0xc560, 0x9180, 0x8da0, 0xa9c0, 0xb5e0 };
static void gcm_mult(const gcm_ctx* g, u8* x) {
    int i; u8 lo, hi, rem; u64 zh, zl;
    lo = x[15] & 0xf;
    zh = g->th[lo]; zl = g->tl[lo];
    for (i = 15; i >= 0; i--) {
        lo = x[i] & 0xf; hi = (x[i] >> 4) & 0xf;
        if (i != 15) {
            rem = (u8)(zl & 0xf);
            zl = (zh << 60) | (zl >> 4); zh = (zh >> 4) ^ (gcm_last4[rem] << 48);
            zh ^= g->th[lo]; zl ^= g->tl[lo];
        }
        rem = (u8)(zl & 0xf);
        zl = (zh << 60) | (zl >> 4); zh = (zh >> 4) ^ (gcm_last4[rem] << 48);
        zh ^= g->th[hi]; zl ^= g->tl[hi];
    }
    aead_put_be32(x, (u32)(zh >> 32)); aead_put_be32(x + 4, (u32)zh); aead_put_be32(x + 8, (u32)(zl >> 32)); aead_put_be32(x + 12, (u32)zl);
}
static void ghash_update(const gcm_ctx* g, u8* y, const u8* d, size_t n) {
    while (n > 0) {
        size_t k = n < 16 ? n : 16, i;
        for (i = 0; i < k; i++) y[i] ^= d[i];
        gcm_mult(g, y);
        d += k; n -= k;
    }
}
/* enc=1: encrypt in->out, write tag. enc=0: decrypt and check tag (0 ok, -1 bad). */
static int gcm_crypt(const gcm_ctx* g, int enc, const u8* iv12, const u8* aad, size_t alen,
                     const u8* in, size_t n, u8* out, u8* tag) {
    u8 j0[16], ctr[16], ks[16], y[16] = {0}, lens[16], ek[16]; size_t i, off; u32 c;
    memcpy(j0, iv12, 12); j0[12] = 0; j0[13] = 0; j0[14] = 0; j0[15] = 1;
    ghash_update(g, y, aad, alen);
    if (!enc) ghash_update(g, y, in, n);
    memcpy(ctr, j0, 16);
    for (off = 0; off < n; off += 16) {
        size_t k = n - off < 16 ? n - off : 16;
        c = aead_be32(ctr + 12) + 1; aead_put_be32(ctr + 12, c);
        aes_enc(&g->aes, ctr, ks);
        for (i = 0; i < k; i++) out[off + i] = in[off + i] ^ ks[i];
    }
    if (enc) ghash_update(g, y, out, n);
    memset(lens, 0, 16);
    aead_put_be32(lens + 4, (u32)(alen * 8)); aead_put_be32(lens + 12, (u32)(n * 8));
    aead_put_be32(lens, (u32)((u64)alen * 8 >> 32)); aead_put_be32(lens + 8, (u32)((u64)n * 8 >> 32));
    ghash_update(g, y, lens, 16);
    aes_enc(&g->aes, j0, ek);
    for (i = 0; i < 16; i++) y[i] ^= ek[i];
    if (enc) { memcpy(tag, y, 16); return 0; }
    if (!trc_ct_eq((const char*)y, 16, (const char*)tag, 16)) { memset(out, 0, n); return -1; }
    return 0;
}

/* ═══════════════════════ ChaCha20-Poly1305 (RFC 8439) ═════════════════════ */
#define QR(a, b, c, d) a += b; d ^= a; d = (d << 16) | (d >> 16); c += d; b ^= c; b = (b << 12) | (b >> 20); \
                       a += b; d ^= a; d = (d << 8) | (d >> 24);  c += d; b ^= c; b = (b << 7) | (b >> 25)
static void chacha_block(const u8* key, u32 counter, const u8* nonce, u8* out) {
    u32 s[16], x[16]; int i;
    s[0] = 0x61707865; s[1] = 0x3320646e; s[2] = 0x79622d32; s[3] = 0x6b206574;
    for (i = 0; i < 8; i++) s[4 + i] = ld32le(key + 4 * i);
    s[12] = counter; s[13] = ld32le(nonce); s[14] = ld32le(nonce + 4); s[15] = ld32le(nonce + 8);
    memcpy(x, s, sizeof s);
    for (i = 0; i < 10; i++) {
        QR(x[0], x[4], x[8], x[12]); QR(x[1], x[5], x[9], x[13]); QR(x[2], x[6], x[10], x[14]); QR(x[3], x[7], x[11], x[15]);
        QR(x[0], x[5], x[10], x[15]); QR(x[1], x[6], x[11], x[12]); QR(x[2], x[7], x[8], x[13]); QR(x[3], x[4], x[9], x[14]);
    }
    for (i = 0; i < 16; i++) aead_put_le32(out + 4 * i, x[i] + s[i]);
}
static void chacha_xor(const u8* key, u32 counter, const u8* nonce, const u8* in, u8* out, size_t n) {
    u8 ks[64]; size_t off, i;
    for (off = 0; off < n; off += 64) {
        size_t k = n - off < 64 ? n - off : 64;
        chacha_block(key, counter++, nonce, ks);
        for (i = 0; i < k; i++) out[off + i] = in[off + i] ^ ks[i];
    }
}
typedef struct { u32 r[5], h[5], pad[4]; } poly_ctx;
static void poly_init(poly_ctx* p, const u8* key) {
    p->r[0] = (ld32le(key)) & 0x3ffffff; p->r[1] = (ld32le(key + 3) >> 2) & 0x3ffff03;
    p->r[2] = (ld32le(key + 6) >> 4) & 0x3ffc0ff; p->r[3] = (ld32le(key + 9) >> 6) & 0x3f03fff;
    p->r[4] = (ld32le(key + 12) >> 8) & 0x00fffff;
    memset(p->h, 0, sizeof p->h);
    p->pad[0] = ld32le(key + 16); p->pad[1] = ld32le(key + 20); p->pad[2] = ld32le(key + 24); p->pad[3] = ld32le(key + 28);
}
static void poly_block(poly_ctx* p, const u8* m, u32 hibit) {
    u32 r0 = p->r[0], r1 = p->r[1], r2 = p->r[2], r3 = p->r[3], r4 = p->r[4];
    u32 s1 = r1 * 5, s2 = r2 * 5, s3 = r3 * 5, s4 = r4 * 5;
    u32 h0 = p->h[0], h1 = p->h[1], h2 = p->h[2], h3 = p->h[3], h4 = p->h[4]; u64 d0, d1, d2, d3, d4; u32 c;
    h0 += (ld32le(m)) & 0x3ffffff; h1 += (ld32le(m + 3) >> 2) & 0x3ffffff; h2 += (ld32le(m + 6) >> 4) & 0x3ffffff;
    h3 += (ld32le(m + 9) >> 6) & 0x3ffffff; h4 += (ld32le(m + 12) >> 8) | hibit;
    d0 = (u64)h0 * r0 + (u64)h1 * s4 + (u64)h2 * s3 + (u64)h3 * s2 + (u64)h4 * s1;
    d1 = (u64)h0 * r1 + (u64)h1 * r0 + (u64)h2 * s4 + (u64)h3 * s3 + (u64)h4 * s2;
    d2 = (u64)h0 * r2 + (u64)h1 * r1 + (u64)h2 * r0 + (u64)h3 * s4 + (u64)h4 * s3;
    d3 = (u64)h0 * r3 + (u64)h1 * r2 + (u64)h2 * r1 + (u64)h3 * r0 + (u64)h4 * s4;
    d4 = (u64)h0 * r4 + (u64)h1 * r3 + (u64)h2 * r2 + (u64)h3 * r1 + (u64)h4 * r0;
    c = (u32)(d0 >> 26); h0 = (u32)d0 & 0x3ffffff; d1 += c;
    c = (u32)(d1 >> 26); h1 = (u32)d1 & 0x3ffffff; d2 += c;
    c = (u32)(d2 >> 26); h2 = (u32)d2 & 0x3ffffff; d3 += c;
    c = (u32)(d3 >> 26); h3 = (u32)d3 & 0x3ffffff; d4 += c;
    c = (u32)(d4 >> 26); h4 = (u32)d4 & 0x3ffffff; h0 += c * 5;
    c = h0 >> 26; h0 &= 0x3ffffff; h1 += c;
    p->h[0] = h0; p->h[1] = h1; p->h[2] = h2; p->h[3] = h3; p->h[4] = h4;
}
static void poly_update(poly_ctx* p, const u8* m, size_t n) {   /* pads the tail block with zeros */
    while (n >= 16) { poly_block(p, m, 1u << 24); m += 16; n -= 16; }
    if (n) { u8 b[16] = {0}; memcpy(b, m, n); poly_block(p, b, 1u << 24); }
}
static void poly_finish(poly_ctx* p, u8* mac) {
    u32 h0 = p->h[0], h1 = p->h[1], h2 = p->h[2], h3 = p->h[3], h4 = p->h[4], c, g0, g1, g2, g3, g4, mask; u64 f;
    c = h1 >> 26; h1 &= 0x3ffffff; h2 += c; c = h2 >> 26; h2 &= 0x3ffffff; h3 += c;
    c = h3 >> 26; h3 &= 0x3ffffff; h4 += c; c = h4 >> 26; h4 &= 0x3ffffff; h0 += c * 5;
    c = h0 >> 26; h0 &= 0x3ffffff; h1 += c;
    g0 = h0 + 5; c = g0 >> 26; g0 &= 0x3ffffff; g1 = h1 + c; c = g1 >> 26; g1 &= 0x3ffffff;
    g2 = h2 + c; c = g2 >> 26; g2 &= 0x3ffffff; g3 = h3 + c; c = g3 >> 26; g3 &= 0x3ffffff; g4 = h4 + c - (1u << 26);
    mask = (g4 >> 31) - 1;
    g0 &= mask; g1 &= mask; g2 &= mask; g3 &= mask; g4 &= mask; mask = ~mask;
    h0 = (h0 & mask) | g0; h1 = (h1 & mask) | g1; h2 = (h2 & mask) | g2; h3 = (h3 & mask) | g3; h4 = (h4 & mask) | g4;
    h0 = (h0 | (h1 << 26)); h1 = ((h1 >> 6) | (h2 << 20)); h2 = ((h2 >> 12) | (h3 << 14)); h3 = ((h3 >> 18) | (h4 << 8));
    f = (u64)h0 + p->pad[0]; h0 = (u32)f;
    f = (u64)h1 + p->pad[1] + (f >> 32); h1 = (u32)f;
    f = (u64)h2 + p->pad[2] + (f >> 32); h2 = (u32)f;
    f = (u64)h3 + p->pad[3] + (f >> 32); h3 = (u32)f;
    aead_put_le32(mac, h0); aead_put_le32(mac + 4, h1); aead_put_le32(mac + 8, h2); aead_put_le32(mac + 12, h3);
}
static int chachapoly_crypt(const u8* key, int enc, const u8* nonce, const u8* aad, size_t alen,
                            const u8* in, size_t n, u8* out, u8* tag) {
    u8 pk[64], mac[16], lens[16]; poly_ctx p;
    chacha_block(key, 0, nonce, pk);
    poly_init(&p, pk);
    poly_update(&p, aad, alen);
    if (!enc) poly_update(&p, in, n);
    chacha_xor(key, 1, nonce, in, out, n);
    if (enc) poly_update(&p, out, n);
    aead_put_le32(lens, (u32)alen); aead_put_le32(lens + 4, (u32)((u64)alen >> 32));
    aead_put_le32(lens + 8, (u32)n); aead_put_le32(lens + 12, (u32)((u64)n >> 32));
    poly_block(&p, lens, 1u << 24);
    poly_finish(&p, mac);
    wipe(pk, sizeof pk);
    if (enc) { memcpy(tag, mac, 16); return 0; }
    if (!trc_ct_eq((const char*)mac, 16, (const char*)tag, 16)) { memset(out, 0, n); return -1; }
    return 0;
}

/* Standalone AEAD entry points (std.crypto.aead).
 * alg: 1 = AES-128-GCM, 2 = AES-256-GCM, 3 = ChaCha20-Poly1305 (key 16/32/32, nonce 12).
 * seal writes n + 16 bytes; open reads n >= 16 and writes n - 16 (returns -1 on a bad tag). */
int64_t trc_aead_seal(int64_t alg, const char* key, const char* nonce, const char* aad, int64_t alen,
                      const char* pt, int64_t n, char* out) {
    if (alg == 3) { chachapoly_crypt((const u8*)key, 1, (const u8*)nonce, (const u8*)aad, (size_t)alen, (const u8*)pt, (size_t)n, (u8*)out, (u8*)out + n); return n + 16; }
    if (alg == 1 || alg == 2) {
        gcm_ctx* g = (gcm_ctx*)malloc(sizeof(gcm_ctx));
        if (!g) return -1;
        gcm_init(g, (const u8*)key, alg == 1 ? 16 : 32);
        gcm_crypt(g, 1, (const u8*)nonce, (const u8*)aad, (size_t)alen, (const u8*)pt, (size_t)n, (u8*)out, (u8*)out + n);
        wipe(g, sizeof *g); free(g);
        return n + 16;
    }
    return -1;
}
int64_t trc_aead_open(int64_t alg, const char* key, const char* nonce, const char* aad, int64_t alen,
                      const char* ct, int64_t n, char* out) {
    int r = -1;
    if (n < 16) return -1;
    if (alg == 3) r = chachapoly_crypt((const u8*)key, 0, (const u8*)nonce, (const u8*)aad, (size_t)alen, (const u8*)ct, (size_t)(n - 16), (u8*)out, (u8*)(ct + n - 16));
    else if (alg == 1 || alg == 2) {
        gcm_ctx* g = (gcm_ctx*)malloc(sizeof(gcm_ctx));
        if (!g) return -1;
        gcm_init(g, (const u8*)key, alg == 1 ? 16 : 32);
        r = gcm_crypt(g, 0, (const u8*)nonce, (const u8*)aad, (size_t)alen, (const u8*)ct, (size_t)(n - 16), (u8*)out, (u8*)(ct + n - 16));
        wipe(g, sizeof *g); free(g);
    }
    return r < 0 ? -1 : n - 16;
}

/* State API used by std/net/tls_native.c (one precomputed GCM table per direction). */
int64_t trc_gcm_state_size(void) { return (int64_t)sizeof(gcm_ctx); }
void trc_gcm_init(char* st, const char* key, int64_t klen) { gcm_init((gcm_ctx*)st, (const u8*)key, (int)klen); }
int64_t trc_gcm_crypt(const char* st, int64_t enc, const char* iv12, const char* aad, int64_t alen,
                      const char* in, int64_t n, char* out, char* tag) {
    return gcm_crypt((const gcm_ctx*)st, (int)enc, (const u8*)iv12, (const u8*)aad, (size_t)alen, (const u8*)in, (size_t)n, (u8*)out, (u8*)tag);
}
int64_t trc_chachapoly_crypt(const char* key, int64_t enc, const char* nonce, const char* aad, int64_t alen,
                             const char* in, int64_t n, char* out, char* tag) {
    return chachapoly_crypt((const u8*)key, (int)enc, (const u8*)nonce, (const u8*)aad, (size_t)alen, (const u8*)in, (size_t)n, (u8*)out, (u8*)tag);
}


/* ═══════════════════════════ scrypt (RFC 7914) ═══════════════════════════ */
static void salsa20_8(u32 B[16]) {
    u32 x[16]; int i;
    memcpy(x, B, sizeof x);
#define SR(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
    for (i = 0; i < 8; i += 2) {
        x[ 4] ^= SR(x[ 0]+x[12], 7);  x[ 8] ^= SR(x[ 4]+x[ 0], 9);
        x[12] ^= SR(x[ 8]+x[ 4],13);  x[ 0] ^= SR(x[12]+x[ 8],18);
        x[ 9] ^= SR(x[ 5]+x[ 1], 7);  x[13] ^= SR(x[ 9]+x[ 5], 9);
        x[ 1] ^= SR(x[13]+x[ 9],13);  x[ 5] ^= SR(x[ 1]+x[13],18);
        x[14] ^= SR(x[10]+x[ 6], 7);  x[ 2] ^= SR(x[14]+x[10], 9);
        x[ 6] ^= SR(x[ 2]+x[14],13);  x[10] ^= SR(x[ 6]+x[ 2],18);
        x[ 3] ^= SR(x[15]+x[11], 7);  x[ 7] ^= SR(x[ 3]+x[15], 9);
        x[11] ^= SR(x[ 7]+x[ 3],13);  x[15] ^= SR(x[11]+x[ 7],18);
        x[ 1] ^= SR(x[ 0]+x[ 3], 7);  x[ 2] ^= SR(x[ 1]+x[ 0], 9);
        x[ 3] ^= SR(x[ 2]+x[ 1],13);  x[ 0] ^= SR(x[ 3]+x[ 2],18);
        x[ 6] ^= SR(x[ 5]+x[ 4], 7);  x[ 7] ^= SR(x[ 6]+x[ 5], 9);
        x[ 4] ^= SR(x[ 7]+x[ 6],13);  x[ 5] ^= SR(x[ 4]+x[ 7],18);
        x[11] ^= SR(x[10]+x[ 9], 7);  x[ 8] ^= SR(x[11]+x[10], 9);
        x[ 9] ^= SR(x[ 8]+x[11],13);  x[10] ^= SR(x[ 9]+x[ 8],18);
        x[12] ^= SR(x[15]+x[14], 7);  x[13] ^= SR(x[12]+x[15], 9);
        x[14] ^= SR(x[13]+x[12],13);  x[15] ^= SR(x[14]+x[13],18);
    }
#undef SR
    for (i = 0; i < 16; i++) B[i] += x[i];
}
/* BlockMix: B (2r blocks of 16 words) -> Y. */
static void scrypt_blockmix(const u32* B, u32* Y, size_t r) {
    u32 X[16]; size_t i; int k;
    memcpy(X, B + (2 * r - 1) * 16, 64);
    for (i = 0; i < 2 * r; i++) {
        for (k = 0; k < 16; k++) X[k] ^= B[i * 16 + k];
        salsa20_8(X);
        memcpy(Y + ((i & 1) * r + i / 2) * 16, X, 64);
    }
}
/* Validates scrypt parameters; returns the bytes of working memory needed, or -1. */
int64_t trc_scrypt_mem(int64_t N, int64_t r, int64_t p) {
    if (N < 2 || (N & (N - 1)) != 0 || r < 1 || p < 1) return -1;
    if ((u64)r * (u64)p >= ((u64)1 << 30)) return -1;
    if ((u64)r > ((u64)1 << 40) / 128 || (u64)N > ((u64)1 << 40) / (128 * (u64)r)) return -1;
    if (r < 4 && (u64)N >= ((u64)1 << (16 * r))) return -1;   /* N < 2^(128 r / 8) */
    return (int64_t)(128 * (u64)r * (u64)N + 256 * (u64)r + 128 * (u64)r * (u64)p);
}
/* scrypt(P, S, N, r, p) -> dklen bytes. Returns 0, -1 on bad params, -2 if out of memory. */
int64_t trc_scrypt(const char* pwd, int64_t plen, const char* salt, int64_t slen,
                   int64_t N, int64_t r, int64_t p, char* out, int64_t dklen) {
    size_t blen, ru, i, k; u64 j, n; u8* B; u32 *V, *X, *Y;
    if (trc_scrypt_mem(N, r, p) < 0 || dklen < 1) return -1;
    ru = (size_t)r; n = (u64)N;
    blen = 128 * ru * (size_t)p;
    B = (u8*)malloc(blen);
    V = (u32*)malloc(128 * ru * (size_t)n);
    X = (u32*)malloc(256 * ru);
    if (!B || !V || !X) { free(B); free(V); free(X); return -2; }
    Y = X + 32 * ru;
    trc_pbkdf2(256, pwd, plen, salt, slen, 1, (char*)B, (int64_t)blen);
    for (i = 0; i < (size_t)p; i++) {
        u8* Bi = B + i * 128 * ru;
        for (k = 0; k < 32 * ru; k++) X[k] = ld32le(Bi + 4 * k);
        for (j = 0; j < n; j++) {
            memcpy(V + j * 32 * ru, X, 128 * ru);
            scrypt_blockmix(X, Y, ru);
            memcpy(X, Y, 128 * ru);
        }
        for (j = 0; j < n; j++) {
            u64 idx = (u64)X[(2 * ru - 1) * 16] & (n - 1);
            const u32* Vj = V + idx * 32 * ru;
            for (k = 0; k < 32 * ru; k++) X[k] ^= Vj[k];
            scrypt_blockmix(X, Y, ru);
            memcpy(X, Y, 128 * ru);
        }
        for (k = 0; k < 32 * ru; k++) st32le(Bi + 4 * k, X[k]);
    }
    trc_pbkdf2(256, pwd, plen, (const char*)B, (int64_t)blen, 1, out, dklen);
    wipe(B, blen); wipe(V, 128 * ru * (size_t)n); wipe(X, 256 * ru);
    free(B); free(V); free(X);
    return 0;
}

/* ═══════════════════════ bcrypt_pbkdf (OpenBSD / OpenSSH) ════════════════ */
static void bcrypt_pbkdf_hash(const u8* sha2pass, const u8* sha2salt, u8* out, bf_ctx* c) {
    static const u8 ctext[33] = "OxychromaticBlowfishSwatDynamite";
    u32 cdata[8]; int i, k;
    memcpy(c->P, bf_P_init, sizeof c->P); memcpy(c->S, bf_S_init, sizeof c->S);
    bf_expand(c, sha2salt, 64, sha2pass, 64);
    for (i = 0; i < 64; i++) {
        bf_expand(c, NULL, 0, sha2salt, 64);
        bf_expand(c, NULL, 0, sha2pass, 64);
    }
    for (i = 0; i < 8; i++) cdata[i] = ld32be(ctext + 4 * i);
    for (k = 0; k < 64; k++)
        for (i = 0; i < 8; i += 2) bf_encrypt(c, &cdata[i], &cdata[i + 1]);
    for (i = 0; i < 8; i++) st32le(out + 4 * i, cdata[i]);
    wipe(cdata, sizeof cdata);
}
/* bcrypt_pbkdf as used by openssh-key-v1 and signify. Returns 0 or -1. */
int64_t trc_bcrypt_pbkdf(const char* pass, int64_t passlen, const char* salt, int64_t saltlen,
                         int64_t rounds, char* key, int64_t keylen) {
    u8 sha2pass[64], sha2salt[64], out[32], tmp[32], cnt[4]; md_ctx m;
    size_t stride, amt, i, j, left, orig; u32 count; int64_t r; bf_ctx* c;
    if (rounds < 1 || passlen < 0 || saltlen < 1 || saltlen > (1 << 20) || keylen < 1 || keylen > 32 * 32) return -1;
    c = (bf_ctx*)malloc(sizeof(bf_ctx));
    if (!c) return -1;
    orig = left = (size_t)keylen;
    stride = (left + 31) / 32;
    amt = (left + stride - 1) / stride;
    md_init(&m, 512); md_update(&m, (const u8*)pass, (size_t)passlen); md_final(&m, sha2pass);
    for (count = 1; left > 0; count++) {
        st32be(cnt, count);
        md_init(&m, 512); md_update(&m, (const u8*)salt, (size_t)saltlen); md_update(&m, cnt, 4); md_final(&m, sha2salt);
        bcrypt_pbkdf_hash(sha2pass, sha2salt, tmp, c);
        memcpy(out, tmp, 32);
        for (r = 1; r < rounds; r++) {
            md_init(&m, 512); md_update(&m, tmp, 32); md_final(&m, sha2salt);
            bcrypt_pbkdf_hash(sha2pass, sha2salt, tmp, c);
            for (j = 0; j < 32; j++) out[j] ^= tmp[j];
        }
        if (amt > left) amt = left;
        for (i = 0; i < amt; i++) {
            size_t dest = i * stride + (count - 1);
            if (dest >= orig) break;
            key[dest] = (char)out[i];
        }
        left -= i;
    }
    wipe(c, sizeof *c); free(c);
    wipe(sha2pass, sizeof sha2pass); wipe(sha2salt, sizeof sha2salt); wipe(out, sizeof out); wipe(tmp, sizeof tmp);
    return 0;
}

/* ═══════════════════════ AES-CTR (128-bit big-endian counter) ════════════ */
/* klen 16/24/32, iv 16 bytes. out may equal in. Returns 0 or -1. */
int64_t trc_aes_ctr(const char* key, int64_t klen, const char* iv16, const char* in, int64_t n, char* out) {
    aes_ctx a; u8 ctr[16], ks[16]; int64_t off = 0; int i;
    if (klen != 16 && klen != 24 && klen != 32) return -1;
    aes_init(&a, (const u8*)key, (int)klen);
    memcpy(ctr, iv16, 16);
    while (off < n) {
        int64_t take = n - off < 16 ? n - off : 16;
        aes_enc(&a, ctr, ks);
        for (i = 0; i < take; i++) out[off + i] = (char)((u8)in[off + i] ^ ks[i]);
        off += take;
        for (i = 15; i >= 0; i--) { if (++ctr[i] != 0) break; }
    }
    wipe(&a, sizeof a); wipe(ks, sizeof ks);
    return 0;
}

/* ═══════════════ age STREAM (ChaCha20-Poly1305, 64 KiB chunks) ═══════════
 * nonce = 11-byte big-endian chunk counter || last-chunk flag. */
#define AGE_CHUNK 65536
int64_t trc_stream_chunk(int64_t enc, const char* key, int64_t counter, int64_t last,
                         const char* in, int64_t n, char* out) {
    u8 nonce[12]; int i; u64 c = (u64)counter;
    memset(nonce, 0, sizeof nonce);
    for (i = 10; i >= 3; i--) { nonce[i] = (u8)c; c >>= 8; }
    nonce[11] = last ? 1 : 0;
    if (enc) {
        if (n < 0 || n > AGE_CHUNK) return -1;
        chachapoly_crypt((const u8*)key, 1, nonce, NULL, 0, (const u8*)in, (size_t)n, (u8*)out, (u8*)out + n);
        return n + 16;
    }
    if (n < 16 || n > AGE_CHUNK + 16) return -1;
    if (chachapoly_crypt((const u8*)key, 0, nonce, NULL, 0, (const u8*)in, (size_t)(n - 16), (u8*)out, (u8*)(in + n - 16)) < 0) return -1;
    return n - 16;
}
/* Size of the STREAM ciphertext for an n-byte plaintext. */
int64_t trc_stream_sealed_len(int64_t n) {
    int64_t chunks = n == 0 ? 1 : (n + AGE_CHUNK - 1) / AGE_CHUNK;
    return n + 16 * chunks;
}
/* Whole-payload STREAM encryption; out needs trc_stream_sealed_len(n) bytes. */
int64_t trc_stream_seal(const char* key, const char* in, int64_t n, char* out) {
    int64_t off = 0, o = 0, ctr = 0;
    do {
        int64_t take = n - off > AGE_CHUNK ? AGE_CHUNK : n - off;
        int last = off + take == n;
        o += trc_stream_chunk(1, key, ctr++, last, in + off, take, out + o);
        off += take;
    } while (off < n);
    return o;
}
/* Decrypt one received chunk the way age's reference reader does: a short
 * chunk must carry the last-chunk flag; a full-size chunk is tried as a
 * middle chunk, then as a final one. Returns the plaintext length and sets
 * *is_last, or -1. */
int64_t trc_stream_open_chunk(const char* key, int64_t counter, const char* in, int64_t n, char* out, int64_t* is_last) {
    int64_t r;
    *is_last = 0;
    if (n < AGE_CHUNK + 16) {
        if (n < 16 || (n == 16 && counter > 0)) return -1;
        *is_last = 1;
        return trc_stream_chunk(0, key, counter, 1, in, n, out);
    }
    if (n != AGE_CHUNK + 16) return -1;
    r = trc_stream_chunk(0, key, counter, 0, in, n, out);
    if (r >= 0) return r;
    *is_last = 1;
    return trc_stream_chunk(0, key, counter, 1, in, n, out);
}
/* Whole-payload STREAM decryption. out needs n bytes. *released gets the
 * plaintext bytes produced by chunks that authenticated. Returns the
 * plaintext length, or -1 if a chunk fails, the final chunk is missing or
 * data follows it. */
int64_t trc_stream_open(const char* key, const char* in, int64_t n, char* out, int64_t* released) {
    int64_t off = 0, o = 0, ctr = 0, last = 0;
    *released = 0;
    while (off < n && !last) {
        int64_t take = n - off > AGE_CHUNK + 16 ? AGE_CHUNK + 16 : n - off;
        int64_t r = trc_stream_open_chunk(key, ctr++, in + off, take, out + o, &last);
        if (r < 0) return -1;
        o += r; off += take; *released = o;
    }
    if (!last || off != n) return -1;
    return o;
}
