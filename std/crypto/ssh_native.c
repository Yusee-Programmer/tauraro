/*
 * std/crypto/ssh_native.c -- small C helpers for std.crypto.ssh.
 *
 * Linked through the `# tauraro-cpp-shim: ssh_native.c` pragma in
 * std/crypto/ssh.tr.
 *
 *   trc_ssh_md5 ........... MD5 (RFC 1321), only for legacy `ssh-keygen -E md5`
 *                           fingerprints -- never use it for security
 *   trc_ssh_mod_minus1 .... d mod (p - 1) on big-endian byte strings, to
 *                           rebuild the RSA CRT exponents OpenSSH keys omit
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint32_t u32;

/* ─────────────────────────────── MD5 ─────────────────────────────────── */
static u32 md5_rol(u32 x, int c) { return (x << c) | (x >> (32 - c)); }
static void md5_block(u32 h[4], const u8* p) {
    static const u32 K[64] = {
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
        0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
        0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
        0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
        0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
        0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
    static const int R[64] = {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
        5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20, 4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
        6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
    u32 w[16], a = h[0], b = h[1], c = h[2], d = h[3]; int i;
    for (i = 0; i < 16; i++) w[i] = (u32)p[4*i] | ((u32)p[4*i+1] << 8) | ((u32)p[4*i+2] << 16) | ((u32)p[4*i+3] << 24);
    for (i = 0; i < 64; i++) {
        u32 f; int g;
        if (i < 16) { f = (b & c) | (~b & d); g = i; }
        else if (i < 32) { f = (d & b) | (~d & c); g = (5 * i + 1) % 16; }
        else if (i < 48) { f = b ^ c ^ d; g = (3 * i + 5) % 16; }
        else { f = c ^ (b | ~d); g = (7 * i) % 16; }
        f = f + a + K[i] + w[g];
        a = d; d = c; c = b; b = b + md5_rol(f, R[i]);
    }
    h[0] += a; h[1] += b; h[2] += c; h[3] += d;
}
int64_t trc_ssh_md5(const char* data, int64_t n, char* out16) {
    u32 h[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    const u8* p = (const u8*)data; u8 tail[128]; int64_t i, full = n / 64, rem; uint64_t bits = (uint64_t)n * 8; int tl;
    for (i = 0; i < full; i++) md5_block(h, p + 64 * i);
    rem = n - 64 * full;
    memset(tail, 0, sizeof tail);
    if (rem) memcpy(tail, p + 64 * full, (size_t)rem);
    tail[rem] = 0x80;
    tl = rem < 56 ? 64 : 128;
    for (i = 0; i < 8; i++) tail[tl - 8 + i] = (u8)(bits >> (8 * i));
    md5_block(h, tail);
    if (tl == 128) md5_block(h, tail + 64);
    for (i = 0; i < 16; i++) out16[i] = (char)(u8)(h[i / 4] >> (8 * (i % 4)));
    return 16;
}

/* ───────────────────── d mod (p - 1), big-endian bytes ─────────────────── */
/* Writes plen bytes (big-endian, zero-padded) to out. Returns plen or -1.
 * Shift-and-subtract over bits of d; only used when loading a key. */
int64_t trc_ssh_mod_minus1(const char* d, int64_t dlen, const char* p, int64_t plen, char* out) {
    u8 *m, *r; int64_t i, k; int bit;
    if (plen < 1 || plen > 2048 || dlen < 0 || dlen > 4096) return -1;
    m = (u8*)calloc((size_t)plen + 1, 1); r = (u8*)calloc((size_t)plen + 1, 1);
    if (!m || !r) { free(m); free(r); return -1; }
    /* m = p - 1 (one extra leading byte so r can hold 2*m transiently) */
    memcpy(m + 1, p, (size_t)plen);
    for (i = plen; i >= 1; i--) { if (m[i]-- != 0) break; }
    for (i = 0; i < dlen; i++) {
        for (bit = 7; bit >= 0; bit--) {
            int carry = ((u8)d[i] >> bit) & 1, ge = 1;
            for (k = plen; k >= 0; k--) { int v = (r[k] << 1) | carry; r[k] = (u8)v; carry = v >> 8; }
            for (k = 0; k <= plen; k++) { if (r[k] != m[k]) { ge = r[k] > m[k]; break; } }
            if (ge) {
                int borrow = 0;
                for (k = plen; k >= 0; k--) { int v = r[k] - m[k] - borrow; borrow = v < 0; r[k] = (u8)(v + (borrow ? 256 : 0)); }
            }
        }
    }
    memcpy(out, r + 1, (size_t)plen);
    memset(r, 0, (size_t)plen + 1); free(m); free(r);
    return plen;
}
