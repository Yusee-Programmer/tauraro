/*
 * std/math/fft_native.c -- FFT and DSP kernels for std.math.fft / std.math.dsp.
 *
 * Linked through `# tauraro-cpp-shim: fft_native.c` in std/math/fft.tr.
 *
 * Complex data is interleaved doubles (re, im, re, im, ...).
 *
 * Complex FFT: a self-sorting (Stockham) mixed-radix Cooley-Tukey transform
 * in the style of pocketfft, with radix-4/2/3/5 butterflies and a generic
 * odd-radix butterfly. Lengths whose prime factors make that too slow (large
 * primes) use Bluestein's chirp-z algorithm on a 2^a 3^b 5^c length.
 * Only the forward kernel exists: the inverse is conj(FFT(conj(x))), folded
 * into the input copy and the output scaling.
 *
 * Real FFT: even lengths use the half-length complex transform plus a
 * post-processing pass; odd lengths use the full complex transform.
 *
 * Plans own a scratch buffer, so one plan must not run on two threads at
 * once. The by-length entry points (trc_fft_*_n) use a small per-thread
 * plan cache.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#if defined(_MSC_VER)
#define FFT_TLS __declspec(thread)
#else
#define FFT_TLS _Thread_local
#endif

#define FFT_TWO_PI 6.283185307179586476925286766559

typedef struct { double r, i; } fcx;

#define FFT_MAXFCT 64

typedef struct {
    int64_t fct;
    fcx *tw;   /* (fct-1)*(ido-1) twiddles */
    fcx *tws;  /* fct roots of unity (generic radix only) */
} fft_fct;

typedef struct fft_cplan {
    int64_t n;
    int64_t nfct;
    fft_fct fct[FFT_MAXFCT];
    fcx *mem;
    fcx *scratch;
    /* Bluestein */
    int64_t n2;
    struct fft_cplan *sub;
    fcx *bk;    /* chirp c_m = exp(-i pi m^2 / n), n entries */
    fcx *bkf;   /* FFT(conj chirp, wrapped) / n2, n2 entries */
    fcx *akf;   /* work, n2 entries */
} fft_cplan;

typedef struct {
    int64_t n;
    fft_cplan *half;  /* n/2 (even n) */
    fft_cplan *full;  /* n (odd n) */
    fcx *tw;          /* exp(-2 pi i k / n), k = 0..n/2 (even n) */
    fcx *buf;
} fft_rplan;

/* exp(-2 pi i m / n), with the angle reduced to the first half-quadrant so
   twiddles stay accurate for large n */
static fcx fft_root(int64_t m, int64_t n) {
    fcx r;
    int64_t q, rem;
    double c, s, ang;
    m %= n;
    if (m < 0) m += n;
    q = (m * 4) / n;          /* quadrant 0..3 */
    rem = m * 4 - q * n;      /* angle = (q + rem/n) * pi/2 */
    if (rem * 2 > n) {
        ang = (double)(n - rem) / (double)n * (FFT_TWO_PI / 4.0);
        c = sin(ang); s = cos(ang);
    } else {
        ang = (double)rem / (double)n * (FFT_TWO_PI / 4.0);
        c = cos(ang); s = sin(ang);
    }
    switch (q) {
        case 0: r.r = c;  r.i = s;  break;
        case 1: r.r = -s; r.i = c;  break;
        case 2: r.r = -c; r.i = -s; break;
        default: r.r = s; r.i = -c; break;
    }
    r.i = -r.i;  /* forward sign */
    return r;
}

static int64_t fft_largest_prime_factor(int64_t n) {
    int64_t res = 1, x;
    while ((n & 1) == 0) { res = 2; n >>= 1; }
    for (x = 3; x * x <= n; x += 2)
        while (n % x == 0) { res = x; n /= x; }
    if (n > 1) res = n;
    return res;
}

static double fft_cost_guess(int64_t n) {
    const double lfp = 1.1;
    int64_t ni = n, x;
    double result = 0.0;
    while ((n & 1) == 0) { result += 2; n >>= 1; }
    for (x = 3; x * x <= n; x += 2)
        while (n % x == 0) { result += (x <= 5) ? (double)x : lfp * (double)x; n /= x; }
    if (n > 1) result += (n <= 5) ? (double)n : lfp * (double)n;
    return result * (double)ni;
}

/* smallest 2^a 3^b 5^c >= n */
int64_t trc_fft_good_size(int64_t n) {
    int64_t best, f3, f5;
    if (n <= 6) return n < 1 ? 1 : n;
    best = 2;
    while (best < n) best *= 2;
    for (f5 = 1; f5 < best; f5 *= 5)
        for (f3 = f5; f3 < best; f3 *= 3) {
            int64_t x = f3;
            while (x < n) x *= 2;
            if (x < best) best = x;
        }
    return best;
}

/* smallest even 2^a 3^b 5^c >= n (fast real FFT length) */
int64_t trc_fft_good_size_real(int64_t n) {
    int64_t h;
    if (n <= 2) return 2;
    h = trc_fft_good_size((n + 1) / 2);
    return 2 * h;
}

static void fft_cplan_free(fft_cplan *p);

static fft_cplan *fft_cplan_new_direct(int64_t n) {
    fft_cplan *p = (fft_cplan *)calloc(1, sizeof(fft_cplan));
    int64_t len = n, divisor, l1, k, memsz = 0, ofs = 0;
    if (!p) return NULL;
    p->n = n;
    while ((len & 3) == 0 && p->nfct < FFT_MAXFCT) { p->fct[p->nfct++].fct = 4; len >>= 2; }
    if ((len & 1) == 0) {
        fft_fct t;
        len >>= 1;
        p->fct[p->nfct++].fct = 2;
        t = p->fct[0]; p->fct[0] = p->fct[p->nfct - 1]; p->fct[p->nfct - 1] = t;
    }
    for (divisor = 3; divisor * divisor <= len; divisor += 2)
        while (len % divisor == 0) { p->fct[p->nfct++].fct = divisor; len /= divisor; }
    if (len > 1) p->fct[p->nfct++].fct = len;

    l1 = 1;
    for (k = 0; k < p->nfct; k++) {
        int64_t ip = p->fct[k].fct, ido = n / (l1 * ip);
        memsz += (ip - 1) * (ido - 1);
        if (ip > 5) memsz += ip;
        l1 *= ip;
    }
    p->mem = (fcx *)malloc((size_t)(memsz > 0 ? memsz : 1) * sizeof(fcx));
    p->scratch = (fcx *)malloc((size_t)n * sizeof(fcx));
    if (!p->mem || !p->scratch) { fft_cplan_free(p); return NULL; }
    l1 = 1;
    for (k = 0; k < p->nfct; k++) {
        int64_t ip = p->fct[k].fct, ido = n / (l1 * ip), j, i;
        p->fct[k].tw = p->mem + ofs;
        for (j = 1; j < ip; j++)
            for (i = 1; i < ido; i++)
                p->fct[k].tw[(j - 1) * (ido - 1) + i - 1] = fft_root(j * l1 * i, n);
        ofs += (ip - 1) * (ido - 1);
        if (ip > 5) {
            p->fct[k].tws = p->mem + ofs;
            for (j = 0; j < ip; j++) p->fct[k].tws[j] = fft_root(j, ip);
            ofs += ip;
        }
        l1 *= ip;
    }
    return p;
}

static void fft_forward(fft_cplan *p, fcx *c);

static fft_cplan *fft_cplan_new(int64_t n) {
    fft_cplan *p;
    int64_t n2, m;
    if (n < 1) return NULL;
    if (n < 50 || fft_largest_prime_factor(n) <= 7) return fft_cplan_new_direct(n);
    {
        double comp1 = fft_cost_guess(n);
        double comp2 = 2.0 * fft_cost_guess(trc_fft_good_size(2 * n - 1));
        comp2 *= 1.5; /* fudge factor for the extra passes */
        if (comp2 >= comp1) return fft_cplan_new_direct(n);
    }
    /* Bluestein */
    n2 = trc_fft_good_size(2 * n - 1);
    p = (fft_cplan *)calloc(1, sizeof(fft_cplan));
    if (!p) return NULL;
    p->n = n;
    p->n2 = n2;
    p->sub = fft_cplan_new_direct(n2);
    p->bk = (fcx *)malloc((size_t)n * sizeof(fcx));
    p->bkf = (fcx *)calloc((size_t)n2, sizeof(fcx));
    p->akf = (fcx *)malloc((size_t)n2 * sizeof(fcx));
    if (!p->sub || !p->bk || !p->bkf || !p->akf) { fft_cplan_free(p); return NULL; }
    for (m = 0; m < n; m++) {
        /* exp(-i pi m^2 / n) = root(m^2 mod 2n, 2n) */
        int64_t coeff = (int64_t)(((uint64_t)m * (uint64_t)m) % (uint64_t)(2 * n));
        p->bk[m] = fft_root(coeff, 2 * n);
    }
    {
        double xn2 = 1.0 / (double)n2;
        p->bkf[0].r = p->bk[0].r * xn2; p->bkf[0].i = -p->bk[0].i * xn2;
        for (m = 1; m < n; m++) {
            p->bkf[m].r = p->bkf[n2 - m].r = p->bk[m].r * xn2;
            p->bkf[m].i = p->bkf[n2 - m].i = -p->bk[m].i * xn2;
        }
        fft_forward(p->sub, p->bkf);
    }
    return p;
}

static void fft_cplan_free(fft_cplan *p) {
    if (!p) return;
    free(p->mem);
    free(p->scratch);
    if (p->sub) fft_cplan_free(p->sub);
    free(p->bk);
    free(p->bkf);
    free(p->akf);
    free(p);
}

/* ── butterflies (forward, w = exp(-2 pi i / ip)) ─────────────────────── */

#define CH(a, b, c) ch[(a) + ido * ((b) + l1 * (c))]
#define CC(a, b, c) cc[(a) + ido * ((b) + cdim * (c))]
#define WA(x, i) wa[(i) - 1 + (x) * (ido - 1)]

static inline fcx cmul(fcx a, fcx b) {
    fcx r;
    r.r = a.r * b.r - a.i * b.i;
    r.i = a.r * b.i + a.i * b.r;
    return r;
}

static void pass2(int64_t ido, int64_t l1, const fcx *cc, fcx *ch, const fcx *wa) {
    const int64_t cdim = 2;
    int64_t k, i;
    for (k = 0; k < l1; k++) {
        {
            fcx a = CC(0, 0, k), b = CC(0, 1, k);
            CH(0, k, 0).r = a.r + b.r; CH(0, k, 0).i = a.i + b.i;
            CH(0, k, 1).r = a.r - b.r; CH(0, k, 1).i = a.i - b.i;
        }
        for (i = 1; i < ido; i++) {
            fcx a = CC(i, 0, k), b = CC(i, 1, k), t;
            CH(i, k, 0).r = a.r + b.r; CH(i, k, 0).i = a.i + b.i;
            t.r = a.r - b.r; t.i = a.i - b.i;
            CH(i, k, 1) = cmul(t, WA(0, i));
        }
    }
}

static void pass3(int64_t ido, int64_t l1, const fcx *cc, fcx *ch, const fcx *wa) {
    const int64_t cdim = 3;
    const double tw1r = -0.5, tw1i = -0.86602540378443864676372317075294;
    int64_t k, i;
    for (k = 0; k < l1; k++)
        for (i = 0; i < ido; i++) {
            fcx c0 = CC(i, 0, k), c1 = CC(i, 1, k), c2 = CC(i, 2, k);
            fcx t, d, ca, y1, y2;
            t.r = c1.r + c2.r; t.i = c1.i + c2.i;
            d.r = c1.r - c2.r; d.i = c1.i - c2.i;
            CH(i, k, 0).r = c0.r + t.r; CH(i, k, 0).i = c0.i + t.i;
            ca.r = c0.r + tw1r * t.r; ca.i = c0.i + tw1r * t.i;
            /* i * tw1i * d = (-tw1i*d.i, tw1i*d.r) */
            y1.r = ca.r - tw1i * d.i; y1.i = ca.i + tw1i * d.r;
            y2.r = ca.r + tw1i * d.i; y2.i = ca.i - tw1i * d.r;
            if (i == 0) { CH(i, k, 1) = y1; CH(i, k, 2) = y2; }
            else { CH(i, k, 1) = cmul(y1, WA(0, i)); CH(i, k, 2) = cmul(y2, WA(1, i)); }
        }
}

static void pass4(int64_t ido, int64_t l1, const fcx *cc, fcx *ch, const fcx *wa) {
    const int64_t cdim = 4;
    int64_t k, i;
    if (ido == 1) {
        for (k = 0; k < l1; k++) {
            fcx c0 = CC(0, 0, k), c1 = CC(0, 1, k), c2 = CC(0, 2, k), c3 = CC(0, 3, k);
            double t1r = c0.r + c2.r, t1i = c0.i + c2.i, t2r = c0.r - c2.r, t2i = c0.i - c2.i;
            double t3r = c1.r + c3.r, t3i = c1.i + c3.i, t4r = c1.r - c3.r, t4i = c1.i - c3.i;
            CH(0, k, 0).r = t1r + t3r; CH(0, k, 0).i = t1i + t3i;
            CH(0, k, 2).r = t1r - t3r; CH(0, k, 2).i = t1i - t3i;
            CH(0, k, 1).r = t2r + t4i; CH(0, k, 1).i = t2i - t4r;
            CH(0, k, 3).r = t2r - t4i; CH(0, k, 3).i = t2i + t4r;
        }
        return;
    }
    for (k = 0; k < l1; k++) {
        for (i = 0; i < ido; i++) {
            fcx c0 = CC(i, 0, k), c1 = CC(i, 1, k), c2 = CC(i, 2, k), c3 = CC(i, 3, k);
            double t1r = c0.r + c2.r, t1i = c0.i + c2.i, t2r = c0.r - c2.r, t2i = c0.i - c2.i;
            double t3r = c1.r + c3.r, t3i = c1.i + c3.i, t4r = c1.r - c3.r, t4i = c1.i - c3.i;
            fcx y1, y2, y3;
            CH(i, k, 0).r = t1r + t3r; CH(i, k, 0).i = t1i + t3i;
            y2.r = t1r - t3r; y2.i = t1i - t3i;
            y1.r = t2r + t4i; y1.i = t2i - t4r;
            y3.r = t2r - t4i; y3.i = t2i + t4r;
            if (i == 0) { CH(i, k, 1) = y1; CH(i, k, 2) = y2; CH(i, k, 3) = y3; }
            else {
                CH(i, k, 1) = cmul(y1, WA(0, i));
                CH(i, k, 2) = cmul(y2, WA(1, i));
                CH(i, k, 3) = cmul(y3, WA(2, i));
            }
        }
    }
}

static void pass5(int64_t ido, int64_t l1, const fcx *cc, fcx *ch, const fcx *wa) {
    const int64_t cdim = 5;
    const double tw1r = 0.3090169943749474241022934171828191,
                 tw1i = -0.9510565162951535721164393333793821,
                 tw2r = -0.8090169943749474241022934171828191,
                 tw2i = -0.5877852522924731291687059546390728;
    int64_t k, i;
    for (k = 0; k < l1; k++)
        for (i = 0; i < ido; i++) {
            fcx x0 = CC(i, 0, k), x1 = CC(i, 1, k), x2 = CC(i, 2, k), x3 = CC(i, 3, k), x4 = CC(i, 4, k);
            double t1r = x1.r + x4.r, t1i = x1.i + x4.i, t4r = x1.r - x4.r, t4i = x1.i - x4.i;
            double t2r = x2.r + x3.r, t2i = x2.i + x3.i, t3r = x2.r - x3.r, t3i = x2.i - x3.i;
            fcx y[5];
            double car, cai, cbr, cbi;
            y[0].r = x0.r + t1r + t2r; y[0].i = x0.i + t1i + t2i;
            car = x0.r + tw1r * t1r + tw2r * t2r; cai = x0.i + tw1r * t1i + tw2r * t2i;
            cbr = tw1i * t4r + tw2i * t3r; cbi = tw1i * t4i + tw2i * t3i;  /* times i */
            y[1].r = car - cbi; y[1].i = cai + cbr;
            y[4].r = car + cbi; y[4].i = cai - cbr;
            car = x0.r + tw2r * t1r + tw1r * t2r; cai = x0.i + tw2r * t1i + tw1r * t2i;
            cbr = tw2i * t4r - tw1i * t3r; cbi = tw2i * t4i - tw1i * t3i;
            y[2].r = car - cbi; y[2].i = cai + cbr;
            y[3].r = car + cbi; y[3].i = cai - cbr;
            CH(i, k, 0) = y[0];
            if (i == 0) { CH(i, k, 1) = y[1]; CH(i, k, 2) = y[2]; CH(i, k, 3) = y[3]; CH(i, k, 4) = y[4]; }
            else {
                int64_t j;
                for (j = 1; j < 5; j++) CH(i, k, j) = cmul(y[j], WA(j - 1, i));
            }
        }
}

/* generic odd radix: O(ip^2) per butterfly, using the x_m +- x_{ip-m} symmetry */
static int passg(int64_t ido, int64_t l1, const fcx *cc, fcx *ch, const fcx *wa, const fcx *tws, int64_t ip) {
    const int64_t cdim = ip;
    int64_t h = (ip - 1) / 2, k, i, j, m;
    fcx *s = (fcx *)malloc((size_t)(2 * h + 1) * sizeof(fcx));
    fcx *d;
    if (!s) return -1;
    d = s + h + 1;
    for (k = 0; k < l1; k++)
        for (i = 0; i < ido; i++) {
            fcx x0 = CC(i, 0, k), y0 = x0;
            for (m = 1; m <= h; m++) {
                fcx a = CC(i, m, k), b = CC(i, ip - m, k);
                s[m].r = a.r + b.r; s[m].i = a.i + b.i;
                d[m - 1].r = a.r - b.r; d[m - 1].i = a.i - b.i;
                y0.r += s[m].r; y0.i += s[m].i;
            }
            CH(i, k, 0) = y0;
            for (j = 1; j <= h; j++) {
                double ar = x0.r, ai = x0.i, br = 0.0, bi = 0.0;
                int64_t idx = 0;
                fcx yj, yc;
                for (m = 1; m <= h; m++) {
                    idx += j;
                    if (idx >= ip) idx -= ip;
                    ar += tws[idx].r * s[m].r; ai += tws[idx].r * s[m].i;
                    br += tws[idx].i * d[m - 1].r; bi += tws[idx].i * d[m - 1].i;
                }
                yj.r = ar - bi; yj.i = ai + br;
                yc.r = ar + bi; yc.i = ai - br;
                if (i == 0) { CH(i, k, j) = yj; CH(i, k, ip - j) = yc; }
                else {
                    CH(i, k, j) = cmul(yj, WA(j - 1, i));
                    CH(i, k, ip - j) = cmul(yc, WA(ip - j - 1, i));
                }
            }
        }
    free(s);
    return 0;
}

#undef CH
#undef CC
#undef WA

static void fft_forward_direct(fft_cplan *p, fcx *c) {
    int64_t n = p->n, l1 = 1, k;
    fcx *p1 = c, *p2 = p->scratch;
    if (n == 1) return;
    for (k = 0; k < p->nfct; k++) {
        int64_t ip = p->fct[k].fct, l2 = ip * l1, ido = n / l2;
        fcx *t;
        switch (ip) {
            case 4: pass4(ido, l1, p1, p2, p->fct[k].tw); break;
            case 2: pass2(ido, l1, p1, p2, p->fct[k].tw); break;
            case 3: pass3(ido, l1, p1, p2, p->fct[k].tw); break;
            case 5: pass5(ido, l1, p1, p2, p->fct[k].tw); break;
            default: passg(ido, l1, p1, p2, p->fct[k].tw, p->fct[k].tws, ip); break;
        }
        t = p1; p1 = p2; p2 = t;
        l1 = l2;
    }
    if (p1 != c) memcpy(c, p1, (size_t)n * sizeof(fcx));
}

static void fft_forward(fft_cplan *p, fcx *c) {
    int64_t n, n2, m;
    fcx *akf;
    if (!p->sub) { fft_forward_direct(p, c); return; }
    n = p->n; n2 = p->n2; akf = p->akf;
    /* a_m = x_m * c_m */
    for (m = 0; m < n; m++) akf[m] = cmul(c[m], p->bk[m]);
    for (m = n; m < n2; m++) { akf[m].r = 0.0; akf[m].i = 0.0; }
    fft_forward_direct(p->sub, akf);
    /* conj(A * B) */
    for (m = 0; m < n2; m++) {
        fcx t = cmul(akf[m], p->bkf[m]);
        akf[m].r = t.r; akf[m].i = -t.i;
    }
    fft_forward_direct(p->sub, akf);
    /* X_k = c_k * conj(akf_k) */
    for (m = 0; m < n; m++) {
        fcx t = akf[m];
        t.i = -t.i;
        c[m] = cmul(t, p->bk[m]);
    }
}

/* ── plans: public entry points ───────────────────────────────────────── */

void *trc_fft_plan_new(int64_t n) { return (void *)fft_cplan_new(n); }
void trc_fft_plan_free(void *p) { fft_cplan_free((fft_cplan *)p); }
int64_t trc_fft_plan_is_bluestein(void *p) { return (p && ((fft_cplan *)p)->sub) ? 1 : 0; }

static void fft_c2c_plan(fft_cplan *p, const double *in, double *out, int64_t inverse, double scale) {
    int64_t n = p->n, k;
    fcx *o = (fcx *)out;
    if (inverse) {
        for (k = 0; k < n; k++) { o[k].r = in[2 * k]; o[k].i = -in[2 * k + 1]; }
    } else if (in != out) {
        memmove(out, in, (size_t)n * sizeof(fcx));
    }
    fft_forward(p, o);
    if (inverse) {
        for (k = 0; k < n; k++) { o[k].r *= scale; o[k].i *= -scale; }
    } else if (scale != 1.0) {
        for (k = 0; k < n; k++) { o[k].r *= scale; o[k].i *= scale; }
    }
}

void trc_fft_c2c(void *plan, const double *in, double *out, int64_t inverse, double scale) {
    if (!plan) return;
    fft_c2c_plan((fft_cplan *)plan, in, out, inverse, scale);
}

static fft_rplan *fft_rplan_new(int64_t n) {
    fft_rplan *p = (fft_rplan *)calloc(1, sizeof(fft_rplan));
    int64_t k;
    if (!p || n < 1) { free(p); return NULL; }
    p->n = n;
    if (n % 2 == 0) {
        int64_t h = n / 2;
        p->half = fft_cplan_new(h);
        p->tw = (fcx *)malloc((size_t)(h + 1) * sizeof(fcx));
        p->buf = (fcx *)malloc((size_t)h * sizeof(fcx));
        if (!p->half || !p->tw || !p->buf) goto fail;
        for (k = 0; k <= h; k++) p->tw[k] = fft_root(k, n);
    } else {
        p->full = fft_cplan_new(n);
        p->buf = (fcx *)malloc((size_t)n * sizeof(fcx));
        if (!p->full || !p->buf) goto fail;
    }
    return p;
fail:
    if (p->half) fft_cplan_free(p->half);
    if (p->full) fft_cplan_free(p->full);
    free(p->tw); free(p->buf); free(p);
    return NULL;
}

static void fft_rplan_free(fft_rplan *p) {
    if (!p) return;
    if (p->half) fft_cplan_free(p->half);
    if (p->full) fft_cplan_free(p->full);
    free(p->tw); free(p->buf); free(p);
}

void *trc_rfft_plan_new(int64_t n) { return (void *)fft_rplan_new(n); }
void trc_rfft_plan_free(void *p) { fft_rplan_free((fft_rplan *)p); }

/* real in (n) -> complex out (n/2+1, interleaved) */
static void fft_r2c_plan(fft_rplan *p, const double *in, double *out, double scale) {
    int64_t n = p->n, k;
    fcx *o = (fcx *)out;
    if (p->full) {
        for (k = 0; k < n; k++) { p->buf[k].r = in[k]; p->buf[k].i = 0.0; }
        fft_forward(p->full, p->buf);
        for (k = 0; k <= n / 2; k++) { o[k].r = p->buf[k].r * scale; o[k].i = p->buf[k].i * scale; }
        return;
    }
    {
        int64_t h = n / 2;
        fcx *z = p->buf;
        for (k = 0; k < h; k++) { z[k].r = in[2 * k]; z[k].i = in[2 * k + 1]; }
        fft_forward(p->half, z);
        /* k = 0 and k = h */
        {
            double z0r = z[0].r, z0i = z[0].i;
            o[0].r = (z0r + z0i) * scale; o[0].i = 0.0;
            o[h].r = (z0r - z0i) * scale; o[h].i = 0.0;
        }
        for (k = 1; k <= h / 2; k++) {
            int64_t j = h - k;
            /* both k and j = h-k from z[k], z[j] */
            fcx zk = z[k], zj = z[j];
            double er = 0.5 * (zk.r + zj.r), ei = 0.5 * (zk.i - zj.i);
            double orr = 0.5 * (zk.i + zj.i), oi = -0.5 * (zk.r - zj.r);
            fcx w = p->tw[k], t;
            /* X_k = E + w^k O */
            t.r = w.r * orr - w.i * oi; t.i = w.r * oi + w.i * orr;
            o[k].r = (er + t.r) * scale; o[k].i = (ei + t.i) * scale;
            if (j != k) {
                /* X_j = conj(E_k) + w^j conj(O_k) */
                fcx wj = p->tw[j];
                double cr = orr, ci = -oi;
                t.r = wj.r * cr - wj.i * ci; t.i = wj.r * ci + wj.i * cr;
                o[j].r = (er + t.r) * scale; o[j].i = (-ei + t.i) * scale;
            }
        }
    }
}

/* complex in (n/2+1) -> real out (n); out = scale * unnormalised inverse */
static void fft_c2r_plan(fft_rplan *p, const double *in, double *out, double scale) {
    int64_t n = p->n, k;
    const fcx *x = (const fcx *)in;
    if (p->full) {
        fcx *b = p->buf;
        b[0].r = x[0].r; b[0].i = 0.0;
        for (k = 1; k <= n / 2; k++) {
            /* conjugate for the inverse-by-forward trick */
            b[k].r = x[k].r; b[k].i = -x[k].i;
            b[n - k].r = x[k].r; b[n - k].i = x[k].i;
        }
        fft_forward(p->full, b);
        for (k = 0; k < n; k++) out[k] = b[k].r * scale;
        return;
    }
    {
        int64_t h = n / 2;
        fcx *z = p->buf;
        double x0 = x[0].r, xh = x[h].r;
        /* Z_k = E_k + i O_k with E = X_k + conj(X_{h-k}), O = (X_k - conj(X_{h-k})) conj(w^k);
           store conj(Z) for the inverse trick */
        z[0].r = x0 + xh; z[0].i = -(x0 - xh);
        for (k = 1; k < h; k++) {
            fcx xk = x[k], xc = x[h - k], w = p->tw[k];
            double er = xk.r + xc.r, ei = xk.i - xc.i;
            double dr = xk.r - xc.r, di = xk.i + xc.i;
            /* O = d * conj(w) */
            double orr = dr * w.r + di * w.i, oi = di * w.r - dr * w.i;
            /* Z = E + i O = (er - oi, ei + orr) ; conj */
            z[k].r = er - oi; z[k].i = -(ei + orr);
        }
        fft_forward(p->half, z);
        for (k = 0; k < h; k++) { out[2 * k] = z[k].r * scale; out[2 * k + 1] = -z[k].i * scale; }
    }
}

void trc_rfft_r2c(void *plan, const double *in, double *out, double scale) {
    if (plan) fft_r2c_plan((fft_rplan *)plan, in, out, scale);
}
void trc_rfft_c2r(void *plan, const double *in, double *out, double scale) {
    if (plan) fft_c2r_plan((fft_rplan *)plan, in, out, scale);
}

/* ── per-thread plan cache ────────────────────────────────────────────── */

#define FFT_CACHE 8
static FFT_TLS fft_cplan *fft_ccache[FFT_CACHE];
static FFT_TLS int64_t fft_ccache_next;
static FFT_TLS fft_rplan *fft_rcache[FFT_CACHE];
static FFT_TLS int64_t fft_rcache_next;

static fft_cplan *fft_cached_c(int64_t n) {
    int64_t i;
    fft_cplan *p;
    for (i = 0; i < FFT_CACHE; i++)
        if (fft_ccache[i] && fft_ccache[i]->n == n) return fft_ccache[i];
    p = fft_cplan_new(n);
    if (!p) return NULL;
    i = fft_ccache_next;
    fft_ccache_next = (fft_ccache_next + 1) % FFT_CACHE;
    if (fft_ccache[i]) fft_cplan_free(fft_ccache[i]);
    fft_ccache[i] = p;
    return p;
}

static fft_rplan *fft_cached_r(int64_t n) {
    int64_t i;
    fft_rplan *p;
    for (i = 0; i < FFT_CACHE; i++)
        if (fft_rcache[i] && fft_rcache[i]->n == n) return fft_rcache[i];
    p = fft_rplan_new(n);
    if (!p) return NULL;
    i = fft_rcache_next;
    fft_rcache_next = (fft_rcache_next + 1) % FFT_CACHE;
    if (fft_rcache[i]) fft_rplan_free(fft_rcache[i]);
    fft_rcache[i] = p;
    return p;
}

/* Returns 0 on success, -1 when the plan could not be allocated. */
int64_t trc_fft_c2c_n(int64_t n, const double *in, double *out, int64_t inverse, double scale) {
    fft_cplan *p = fft_cached_c(n);
    if (!p) return -1;
    fft_c2c_plan(p, in, out, inverse, scale);
    return 0;
}
int64_t trc_fft_r2c_n(int64_t n, const double *in, double *out, double scale) {
    fft_rplan *p = fft_cached_r(n);
    if (!p) return -1;
    fft_r2c_plan(p, in, out, scale);
    return 0;
}
int64_t trc_fft_c2r_n(int64_t n, const double *in, double *out, double scale) {
    fft_rplan *p = fft_cached_r(n);
    if (!p) return -1;
    fft_c2r_plan(p, in, out, scale);
    return 0;
}

/* Release this thread's cached plans. */
void trc_fft_cache_clear(void) {
    int64_t i;
    for (i = 0; i < FFT_CACHE; i++) {
        if (fft_ccache[i]) { fft_cplan_free(fft_ccache[i]); fft_ccache[i] = NULL; }
        if (fft_rcache[i]) { fft_rplan_free(fft_rcache[i]); fft_rcache[i] = NULL; }
    }
}

/* 2-D complex transform of a rows x cols row-major array. */
int64_t trc_fft2_c2c(int64_t rows, int64_t cols, const double *in, double *out, int64_t inverse, double scale) {
    int64_t r, c;
    fcx *col, *o = (fcx *)out;
    fft_cplan *pr, *pc;
    if (in != out) memmove(out, in, (size_t)(rows * cols) * sizeof(fcx));
    pr = fft_cached_c(cols);
    if (!pr) return -1;
    for (r = 0; r < rows; r++) fft_c2c_plan(pr, out + 2 * r * cols, out + 2 * r * cols, inverse, 1.0);
    pc = fft_cached_c(rows);
    col = (fcx *)malloc((size_t)rows * sizeof(fcx));
    if (!pc || !col) { free(col); return -1; }
    for (c = 0; c < cols; c++) {
        for (r = 0; r < rows; r++) col[r] = o[r * cols + c];
        fft_c2c_plan(pc, (double *)col, (double *)col, inverse, scale);
        for (r = 0; r < rows; r++) o[r * cols + c] = col[r];
    }
    free(col);
    return 0;
}

/* ── DSP kernels ──────────────────────────────────────────────────────── */

static void dsp_conv_direct(const double *a, int64_t na, const double *b, int64_t nb, double *out) {
    int64_t i, j, n = na + nb - 1;
    for (i = 0; i < n; i++) out[i] = 0.0;
    if (na < nb) { const double *t = a; int64_t tn = na; a = b; na = nb; b = t; nb = tn; }
    for (j = 0; j < nb; j++) {
        double bj = b[j];
        double *o = out + j;
        for (i = 0; i < na; i++) o[i] += a[i] * bj;
    }
}

/* Full linear convolution, out has na+nb-1 entries.
   method: 0 auto, 1 direct, 2 FFT. Returns the method used, or -1. */
int64_t trc_dsp_convolve(const double *a, int64_t na, const double *b, int64_t nb, double *out, int64_t method) {
    int64_t n, nfft, nh, k;
    double *pa, *pb, *fa, *fb;
    fft_rplan *p;
    if (na < 1 || nb < 1) return -1;
    n = na + nb - 1;
    nfft = trc_fft_good_size_real(n);
    if (method == 0) {
        double direct = (double)na * (double)nb;
        double viafft = 3.0 * (double)nfft * (log((double)nfft) / log(2.0)) * 2.5 + 4.0 * (double)nfft;
        method = (na <= 16 || nb <= 16 || direct <= viafft) ? 1 : 2;
    }
    if (method == 1) { dsp_conv_direct(a, na, b, nb, out); return 1; }
    nh = nfft / 2 + 1;
    p = fft_cached_r(nfft);
    pa = (double *)calloc((size_t)(2 * nfft + 4 * nh), sizeof(double));
    if (!p || !pa) { free(pa); return -1; }
    pb = pa + nfft; fa = pb + nfft; fb = fa + 2 * nh;
    memcpy(pa, a, (size_t)na * sizeof(double));
    memcpy(pb, b, (size_t)nb * sizeof(double));
    fft_r2c_plan(p, pa, fa, 1.0);
    fft_r2c_plan(p, pb, fb, 1.0);
    for (k = 0; k < nh; k++) {
        double ar = fa[2 * k], ai = fa[2 * k + 1], br = fb[2 * k], bi = fb[2 * k + 1];
        fa[2 * k] = ar * br - ai * bi;
        fa[2 * k + 1] = ar * bi + ai * br;
    }
    fft_c2r_plan(p, fa, pa, 1.0 / (double)nfft);
    memcpy(out, pa, (size_t)n * sizeof(double));
    free(pa);
    return 2;
}

/* Direct form II transposed IIR/FIR filter. zi (max(na,nb)-1 entries) is the
   initial state on entry and the final state on exit. a[0] must be nonzero. */
int64_t trc_dsp_lfilter(const double *b, int64_t nb, const double *a, int64_t na, const double *x, int64_t n, double *zi, double *y) {
    int64_t m = nb > na ? nb : na, i, j;
    double *bb, *aa, a0;
    if (na < 1 || nb < 1 || a[0] == 0.0) return -1;
    bb = (double *)calloc((size_t)(2 * m), sizeof(double));
    if (!bb) return -1;
    aa = bb + m;
    a0 = a[0];
    for (j = 0; j < nb; j++) bb[j] = b[j] / a0;
    for (j = 0; j < na; j++) aa[j] = a[j] / a0;
    if (m == 1) {
        for (i = 0; i < n; i++) y[i] = bb[0] * x[i];
        free(bb);
        return 0;
    }
    if (m == 3) {
        /* biquad fast path */
        double b0 = bb[0], b1 = bb[1], b2 = bb[2], a1 = aa[1], a2 = aa[2];
        double z0 = zi[0], z1 = zi[1];
        for (i = 0; i < n; i++) {
            double xi = x[i], yi = b0 * xi + z0;
            z0 = b1 * xi + z1 - a1 * yi;
            z1 = b2 * xi - a2 * yi;
            y[i] = yi;
        }
        zi[0] = z0; zi[1] = z1;
        free(bb);
        return 0;
    }
    for (i = 0; i < n; i++) {
        double xi = x[i], yi = bb[0] * xi + zi[0];
        for (j = 0; j < m - 2; j++) zi[j] = bb[j + 1] * xi + zi[j + 1] - aa[j + 1] * yi;
        zi[m - 2] = bb[m - 1] * xi - aa[m - 1] * yi;
        y[i] = yi;
    }
    free(bb);
    return 0;
}

/* Steady-state initial conditions for a unit step (scipy.signal.lfilter_zi).
   zi has max(na,nb)-1 entries. Returns 0, or -1 when singular. */
int64_t trc_dsp_lfilter_zi(const double *b, int64_t nb, const double *a, int64_t na, double *zi) {
    int64_t m = nb > na ? nb : na, k = m - 1, i, j, c;
    double *bb, *aa, *mat, *rhs, a0;
    if (na < 1 || nb < 1 || a[0] == 0.0) return -1;
    if (k == 0) return 0;
    bb = (double *)calloc((size_t)(2 * m + k * k + k), sizeof(double));
    if (!bb) return -1;
    aa = bb + m; mat = aa + m; rhs = mat + k * k;
    a0 = a[0];
    for (j = 0; j < nb; j++) bb[j] = b[j] / a0;
    for (j = 0; j < na; j++) aa[j] = a[j] / a0;
    /* (I - companion(a)^T) zi = b[1:] - a[1:] b[0] */
    for (i = 0; i < k; i++) {
        mat[i * k + i] = 1.0;
        mat[i * k + 0] += aa[i + 1];
        if (i + 1 < k) mat[i * k + i + 1] -= 1.0;
        rhs[i] = bb[i + 1] - aa[i + 1] * bb[0];
    }
    /* Gaussian elimination with partial pivoting */
    for (c = 0; c < k; c++) {
        int64_t piv = c;
        double best = fabs(mat[c * k + c]);
        for (i = c + 1; i < k; i++) if (fabs(mat[i * k + c]) > best) { best = fabs(mat[i * k + c]); piv = i; }
        if (best == 0.0) { free(bb); return -1; }
        if (piv != c) {
            for (j = 0; j < k; j++) { double t = mat[c * k + j]; mat[c * k + j] = mat[piv * k + j]; mat[piv * k + j] = t; }
            { double t = rhs[c]; rhs[c] = rhs[piv]; rhs[piv] = t; }
        }
        for (i = c + 1; i < k; i++) {
            double f = mat[i * k + c] / mat[c * k + c];
            if (f == 0.0) continue;
            for (j = c; j < k; j++) mat[i * k + j] -= f * mat[c * k + j];
            rhs[i] -= f * rhs[c];
        }
    }
    for (i = k - 1; i >= 0; i--) {
        double s = rhs[i];
        for (j = i + 1; j < k; j++) s -= mat[i * k + j] * zi[j];
        zi[i] = s / mat[i * k + i];
    }
    free(bb);
    return 0;
}

/* Zero-phase forward-backward filter with odd extension (scipy.signal.filtfilt,
   padtype='odd'). Needs n > padlen. Returns 0, or -1. */
int64_t trc_dsp_filtfilt(const double *b, int64_t nb, const double *a, int64_t na, const double *x, int64_t n, int64_t padlen, double *y) {
    int64_t m = nb > na ? nb : na, k = m - 1, ne, i;
    double *ext, *tmp, *zi, *z;
    if (padlen < 0 || n <= padlen) return -1;
    ne = n + 2 * padlen;
    ext = (double *)malloc((size_t)(2 * ne + 2 * (k + 1)) * sizeof(double));
    if (!ext) return -1;
    tmp = ext + ne; zi = tmp + ne; z = zi + k + 1;
    for (i = 0; i < padlen; i++) ext[i] = 2.0 * x[0] - x[padlen - i];
    for (i = 0; i < n; i++) ext[padlen + i] = x[i];
    for (i = 0; i < padlen; i++) ext[padlen + n + i] = 2.0 * x[n - 1] - x[n - 2 - i];
    if (trc_dsp_lfilter_zi(b, nb, a, na, zi) != 0) { free(ext); return -1; }
    for (i = 0; i < k; i++) z[i] = zi[i] * ext[0];
    if (trc_dsp_lfilter(b, nb, a, na, ext, ne, z, tmp) != 0) { free(ext); return -1; }
    /* reverse */
    for (i = 0; i < ne; i++) ext[i] = tmp[ne - 1 - i];
    for (i = 0; i < k; i++) z[i] = zi[i] * ext[0];
    if (trc_dsp_lfilter(b, nb, a, na, ext, ne, z, tmp) != 0) { free(ext); return -1; }
    for (i = 0; i < n; i++) y[i] = tmp[ne - 1 - padlen - i];
    free(ext);
    return 0;
}

/* Goertzel: DFT of x at `omega` radians/sample, written to out[0..1]. */
void trc_dsp_goertzel(const double *x, int64_t n, double omega, double *out) {
    double c = 2.0 * cos(omega), s1 = 0.0, s2 = 0.0, yr, yi, cr, ci, ang;
    int64_t i;
    for (i = 0; i < n; i++) {
        double s = x[i] + c * s1 - s2;
        s2 = s1; s1 = s;
    }
    /* y = s1 - exp(-i w) s2 ; X = exp(-i w (n-1)) y */
    yr = s1 - cos(omega) * s2;
    yi = sin(omega) * s2;
    ang = -omega * (double)(n - 1);
    cr = cos(ang); ci = sin(ang);
    out[0] = yr * cr - yi * ci;
    out[1] = yr * ci + yi * cr;
}

/* Short-time Fourier transform. Frame t covers padded samples
   [t*hop, t*hop+nwin), where the padded signal has `pad` zeros in front;
   each frame is multiplied by win, zero-padded to nfft and real-FFT'd.
   out: nframes x (nfft/2+1) complex, row-major. Returns 0 or -1. */
int64_t trc_dsp_stft(const double *x, int64_t n, const double *win, int64_t nwin, int64_t hop, int64_t nfft, int64_t pad, int64_t nframes, double *out) {
    int64_t nb = nfft / 2 + 1, t, m;
    double *frame;
    fft_rplan *p;
    if (nwin > nfft || hop < 1) return -1;
    p = fft_cached_r(nfft);
    frame = (double *)malloc((size_t)nfft * sizeof(double));
    if (!p || !frame) { free(frame); return -1; }
    for (t = 0; t < nframes; t++) {
        int64_t start = t * hop - pad;
        for (m = 0; m < nfft; m++) frame[m] = 0.0;
        for (m = 0; m < nwin; m++) {
            int64_t idx = start + m;
            if (idx >= 0 && idx < n) frame[m] = x[idx] * win[m];
        }
        fft_r2c_plan(p, frame, out + 2 * t * nb, 1.0);
    }
    free(frame);
    return 0;
}

/* Inverse STFT by weighted overlap-add (inverts trc_dsp_stft when the window
   satisfies the NOLA condition). out has `len` samples. Returns 0 or -1. */
int64_t trc_dsp_istft(const double *spec, int64_t nframes, const double *win, int64_t nwin, int64_t hop, int64_t nfft, int64_t pad, double *out, int64_t len) {
    int64_t nb = nfft / 2 + 1, t, m, total = (nframes - 1) * hop + nwin;
    double *frame, *acc, *wsum;
    fft_rplan *p;
    if (nwin > nfft || hop < 1) return -1;
    p = fft_cached_r(nfft);
    frame = (double *)malloc((size_t)(nfft + 2 * (total > 0 ? total : 1)) * sizeof(double));
    if (!p || !frame) { free(frame); return -1; }
    acc = frame + nfft; wsum = acc + (total > 0 ? total : 1);
    for (m = 0; m < total; m++) { acc[m] = 0.0; wsum[m] = 0.0; }
    for (t = 0; t < nframes; t++) {
        fft_c2r_plan(p, spec + 2 * t * nb, frame, 1.0 / (double)nfft);
        for (m = 0; m < nwin; m++) {
            acc[t * hop + m] += frame[m] * win[m];
            wsum[t * hop + m] += win[m] * win[m];
        }
    }
    for (m = 0; m < len; m++) {
        int64_t idx = m + pad;
        double v = 0.0;
        if (idx >= 0 && idx < total && wsum[idx] > 1e-10) v = acc[idx] / wsum[idx];
        out[m] = v;
    }
    free(frame);
    return 0;
}

/* Welch PSD (scipy.signal.welch semantics: frames of nwin with step `step`,
   no padding, detrend 0 none / 1 constant, one-sided, mean average).
   scaling 0 density (scale by 1/(fs*sum w^2)), 1 spectrum (1/(sum w)^2).
   out has nfft/2+1 entries. Returns the frame count, or -1. */
int64_t trc_dsp_welch(const double *x, int64_t n, const double *win, int64_t nwin, int64_t step, int64_t nfft, int64_t detrend, int64_t scaling, double fs, double *out) {
    int64_t nb = nfft / 2 + 1, nframes, t, m;
    double *frame, *spec, scale, sw = 0.0, sw2 = 0.0;
    fft_rplan *p;
    if (nwin > nfft || step < 1 || n < nwin || nwin < 1) return -1;
    nframes = (n - nwin) / step + 1;
    p = fft_cached_r(nfft);
    frame = (double *)malloc((size_t)(nfft + 2 * nb) * sizeof(double));
    if (!p || !frame) { free(frame); return -1; }
    spec = frame + nfft;
    for (m = 0; m < nwin; m++) { sw += win[m]; sw2 += win[m] * win[m]; }
    scale = scaling == 0 ? 1.0 / (fs * sw2) : 1.0 / (sw * sw);
    for (m = 0; m < nb; m++) out[m] = 0.0;
    for (t = 0; t < nframes; t++) {
        const double *seg = x + t * step;
        double mean = 0.0;
        if (detrend == 1) {
            for (m = 0; m < nwin; m++) mean += seg[m];
            mean /= (double)nwin;
        }
        for (m = 0; m < nfft; m++) frame[m] = 0.0;
        for (m = 0; m < nwin; m++) frame[m] = (seg[m] - mean) * win[m];
        fft_r2c_plan(p, frame, spec, 1.0);
        for (m = 0; m < nb; m++) out[m] += spec[2 * m] * spec[2 * m] + spec[2 * m + 1] * spec[2 * m + 1];
    }
    for (m = 0; m < nb; m++) {
        double v = out[m] * scale / (double)nframes;
        int two = (m > 0) && !((nfft % 2 == 0) && m == nb - 1);
        out[m] = two ? 2.0 * v : v;
    }
    free(frame);
    return nframes;
}
