// ==========================================
// MATH MODULE - Pure C Implementation
// ==========================================
// Provides: Complete mathematical functions matching Python's math module
// Platform: Cross-platform (uses standard C math.h)

#ifndef TAURARO_MATH_MODULE_H
#define TAURARO_MATH_MODULE_H

#include <math.h>
#include <float.h>
#include <stdint.h>

// Mathematical constants
#define TAURARO_MATH_PI     3.141592653589793
#define TAURARO_MATH_E      2.718281828459045
#define TAURARO_MATH_TAU    6.283185307179586
#define TAURARO_MATH_INF    INFINITY
#define TAURARO_MATH_NAN    NAN

// Helper to convert TauValue to double
static inline double tau_to_double(TauValue v) {
    if (v.type == 1) return v.value.f;      // Float
    if (v.type == 0) return (double)v.value.i;  // Int
    return 0.0;
}

// Helper to convert TauValue to int64
static inline int64_t tau_to_int64(TauValue v) {
    if (v.type == 0) return v.value.i;      // Int
    if (v.type == 1) return (int64_t)v.value.f;  // Float
    return 0;
}

// Helper to create float TauValue
static inline TauValue tau_float(double f) {
    return (TauValue){.type = 1, .value.f = f, .refcount = 1, .next = NULL};
}

// Helper to create int TauValue
static inline TauValue tau_int(int64_t i) {
    return (TauValue){.type = 0, .value.i = i, .refcount = 1, .next = NULL};
}

// Helper to create bool TauValue
static inline TauValue tau_bool(int b) {
    return (TauValue){.type = 3, .value.i = b ? 1 : 0, .refcount = 1, .next = NULL};
}

// ==========================================
// POWER AND LOGARITHMIC FUNCTIONS
// ==========================================

static inline TauValue tauraro_math_pow(TauValue base, TauValue exponent) {
    return tau_float(pow(tau_to_double(base), tau_to_double(exponent)));
}

static inline TauValue tauraro_math_sqrt(TauValue x) {
    return tau_float(sqrt(tau_to_double(x)));
}

static inline TauValue tauraro_math_exp(TauValue x) {
    return tau_float(exp(tau_to_double(x)));
}

static inline TauValue tauraro_math_exp2(TauValue x) {
    return tau_float(exp2(tau_to_double(x)));
}

static inline TauValue tauraro_math_expm1(TauValue x) {
    return tau_float(expm1(tau_to_double(x)));
}

static inline TauValue tauraro_math_log(TauValue x) {
    return tau_float(log(tau_to_double(x)));
}

static inline TauValue tauraro_math_log2(TauValue x) {
    return tau_float(log2(tau_to_double(x)));
}

static inline TauValue tauraro_math_log10(TauValue x) {
    return tau_float(log10(tau_to_double(x)));
}

static inline TauValue tauraro_math_log1p(TauValue x) {
    return tau_float(log1p(tau_to_double(x)));
}

// ==========================================
// TRIGONOMETRIC FUNCTIONS
// ==========================================

static inline TauValue tauraro_math_sin(TauValue x) {
    return tau_float(sin(tau_to_double(x)));
}

static inline TauValue tauraro_math_cos(TauValue x) {
    return tau_float(cos(tau_to_double(x)));
}

static inline TauValue tauraro_math_tan(TauValue x) {
    return tau_float(tan(tau_to_double(x)));
}

static inline TauValue tauraro_math_asin(TauValue x) {
    return tau_float(asin(tau_to_double(x)));
}

static inline TauValue tauraro_math_acos(TauValue x) {
    return tau_float(acos(tau_to_double(x)));
}

static inline TauValue tauraro_math_atan(TauValue x) {
    return tau_float(atan(tau_to_double(x)));
}

static inline TauValue tauraro_math_atan2(TauValue y, TauValue x) {
    return tau_float(atan2(tau_to_double(y), tau_to_double(x)));
}

// ==========================================
// HYPERBOLIC FUNCTIONS
// ==========================================

static inline TauValue tauraro_math_sinh(TauValue x) {
    return tau_float(sinh(tau_to_double(x)));
}

static inline TauValue tauraro_math_cosh(TauValue x) {
    return tau_float(cosh(tau_to_double(x)));
}

static inline TauValue tauraro_math_tanh(TauValue x) {
    return tau_float(tanh(tau_to_double(x)));
}

static inline TauValue tauraro_math_asinh(TauValue x) {
    return tau_float(asinh(tau_to_double(x)));
}

static inline TauValue tauraro_math_acosh(TauValue x) {
    return tau_float(acosh(tau_to_double(x)));
}

static inline TauValue tauraro_math_atanh(TauValue x) {
    return tau_float(atanh(tau_to_double(x)));
}

// ==========================================
// ANGULAR CONVERSION
// ==========================================

static inline TauValue tauraro_math_degrees(TauValue x) {
    return tau_float(tau_to_double(x) * 180.0 / TAURARO_MATH_PI);
}

static inline TauValue tauraro_math_radians(TauValue x) {
    return tau_float(tau_to_double(x) * TAURARO_MATH_PI / 180.0);
}

// ==========================================
// ROUNDING AND ABSOLUTE VALUE
// ==========================================

static inline TauValue tauraro_math_ceil(TauValue x) {
    return tau_float(ceil(tau_to_double(x)));
}

static inline TauValue tauraro_math_floor(TauValue x) {
    return tau_float(floor(tau_to_double(x)));
}

static inline TauValue tauraro_math_trunc(TauValue x) {
    return tau_float(trunc(tau_to_double(x)));
}

static inline TauValue tauraro_math_fabs(TauValue x) {
    return tau_float(fabs(tau_to_double(x)));
}

// ==========================================
// NUMBER-THEORETIC FUNCTIONS
// ==========================================

static inline TauValue tauraro_math_factorial(TauValue x) {
    int64_t n = tau_to_int64(x);
    if (n < 0) return tau_float(NAN);
    if (n == 0 || n == 1) return tau_int(1);

    int64_t result = 1;
    for (int64_t i = 2; i <= n; i++) {
        result *= i;
    }
    return tau_int(result);
}

static inline TauValue tauraro_math_gcd(TauValue a, TauValue b) {
    int64_t x = tau_to_int64(a);
    int64_t y = tau_to_int64(b);

    x = x < 0 ? -x : x;  // abs(x)
    y = y < 0 ? -y : y;  // abs(y)

    while (y != 0) {
        int64_t temp = y;
        y = x % y;
        x = temp;
    }
    return tau_int(x);
}

static inline TauValue tauraro_math_lcm(TauValue a, TauValue b) {
    int64_t x = tau_to_int64(a);
    int64_t y = tau_to_int64(b);

    if (x == 0 || y == 0) return tau_int(0);

    // Get GCD first
    int64_t gcd_x = x < 0 ? -x : x;
    int64_t gcd_y = y < 0 ? -y : y;
    while (gcd_y != 0) {
        int64_t temp = gcd_y;
        gcd_y = gcd_x % gcd_y;
        gcd_x = temp;
    }

    // LCM = |a * b| / GCD(a, b)
    int64_t result = (x / gcd_x) * y;
    return tau_int(result < 0 ? -result : result);
}

static inline TauValue tauraro_math_isqrt(TauValue x) {
    int64_t n = tau_to_int64(x);
    if (n < 0) return tau_int(0);
    if (n == 0) return tau_int(0);

    // Newton's method for integer square root
    int64_t guess = n;
    while (1) {
        int64_t next = (guess + n / guess) / 2;
        if (next >= guess) break;
        guess = next;
    }
    return tau_int(guess);
}

// ==========================================
// FLOATING POINT OPERATIONS
// ==========================================

static inline TauValue tauraro_math_fmod(TauValue x, TauValue y) {
    return tau_float(fmod(tau_to_double(x), tau_to_double(y)));
}

static inline TauValue tauraro_math_remainder(TauValue x, TauValue y) {
    return tau_float(remainder(tau_to_double(x), tau_to_double(y)));
}

static inline TauValue tauraro_math_copysign(TauValue magnitude, TauValue sign) {
    return tau_float(copysign(tau_to_double(magnitude), tau_to_double(sign)));
}

static inline TauValue tauraro_math_nextafter(TauValue x, TauValue y) {
    return tau_float(nextafter(tau_to_double(x), tau_to_double(y)));
}

static inline TauValue tauraro_math_ldexp(TauValue x, TauValue i) {
    return tau_float(ldexp(tau_to_double(x), (int)tau_to_int64(i)));
}

// ==========================================
// CLASSIFICATION FUNCTIONS
// ==========================================

static inline TauValue tauraro_math_isfinite(TauValue x) {
    return tau_bool(isfinite(tau_to_double(x)));
}

static inline TauValue tauraro_math_isinf(TauValue x) {
    return tau_bool(isinf(tau_to_double(x)));
}

static inline TauValue tauraro_math_isnan(TauValue x) {
    return tau_bool(isnan(tau_to_double(x)));
}

static inline TauValue tauraro_math_isclose(TauValue a, TauValue b, TauValue rel_tol, TauValue abs_tol) {
    double x = tau_to_double(a);
    double y = tau_to_double(b);
    double rtol = tau_to_double(rel_tol);
    double atol = tau_to_double(abs_tol);

    if (x == y) return tau_bool(1);
    if (isinf(x) || isinf(y)) return tau_bool(x == y);

    double diff = fabs(x - y);
    return tau_bool(diff <= fabs(rtol * y) || diff <= atol);
}

// ==========================================
// SPECIAL FUNCTIONS
// ==========================================

// Gamma function using Lanczos approximation
static inline TauValue tauraro_math_gamma(TauValue x) {
    return tau_float(tgamma(tau_to_double(x)));
}

static inline TauValue tauraro_math_lgamma(TauValue x) {
    return tau_float(lgamma(tau_to_double(x)));
}

static inline TauValue tauraro_math_erf(TauValue x) {
    return tau_float(erf(tau_to_double(x)));
}

static inline TauValue tauraro_math_erfc(TauValue x) {
    return tau_float(erfc(tau_to_double(x)));
}

// ==========================================
// CONSTANTS (accessed as variables)
// ==========================================

static const double tauraro_math_pi = TAURARO_MATH_PI;
static const double tauraro_math_e = TAURARO_MATH_E;
static const double tauraro_math_tau = TAURARO_MATH_TAU;
static const double tauraro_math_inf = TAURARO_MATH_INF;
static const double tauraro_math_nan = TAURARO_MATH_NAN;


#endif // TAURARO_MATH_MODULE_H
