// ==========================================
// RANDOM MODULE - Pure C Implementation
// ==========================================
// Provides: Random number generation functions matching Python's random module
// Platform: Cross-platform (uses standard C stdlib.h)

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

// Initialize random seed (should be called once at program start)
static int tauraro_random_initialized = 0;

static inline void tauraro_random_ensure_init(void) {
    if (!tauraro_random_initialized) {
        srand((unsigned int)time(NULL));
        tauraro_random_initialized = 1;
    }
}

// ==========================================
// BASIC RANDOM FUNCTIONS
// ==========================================

// random.random() - Returns random float in [0.0, 1.0)
static inline TauValue tauraro_random_random(void) {
    tauraro_random_ensure_init();
    double r = (double)rand() / (double)RAND_MAX;
    return (TauValue){.type = 1, .value.f = r, .refcount = 1, .next = NULL};
}

// random.uniform(a, b) - Returns random float in [a, b]
static inline TauValue tauraro_random_uniform(TauValue a, TauValue b) {
    tauraro_random_ensure_init();
    double lower = (a.type == 1) ? a.value.f : (double)a.value.i;
    double upper = (b.type == 1) ? b.value.f : (double)b.value.i;
    double r = (double)rand() / (double)RAND_MAX;
    double result = lower + r * (upper - lower);
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.randint(a, b) - Returns random integer in [a, b] (inclusive)
static inline TauValue tauraro_random_randint(TauValue a, TauValue b) {
    tauraro_random_ensure_init();
    int64_t lower = (a.type == 0) ? a.value.i : (int64_t)a.value.f;
    int64_t upper = (b.type == 0) ? b.value.i : (int64_t)b.value.f;

    if (upper < lower) {
        int64_t temp = lower;
        lower = upper;
        upper = temp;
    }

    int64_t range = upper - lower + 1;
    int64_t result = lower + (rand() % range);
    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// random.randrange(start, stop, step) - Returns random int from range
static inline TauValue tauraro_random_randrange(TauValue start, TauValue stop, TauValue step) {
    tauraro_random_ensure_init();
    int64_t a = (start.type == 0) ? start.value.i : (int64_t)start.value.f;
    int64_t b = (stop.type == 0) ? stop.value.i : (int64_t)stop.value.f;
    int64_t s = (step.type == 0) ? step.value.i : (int64_t)step.value.f;

    if (s == 0) s = 1;

    int64_t count = (b - a + s - 1) / s;
    if (count <= 0) count = 1;

    int64_t result = a + (rand() % count) * s;
    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// random.choice(seq) - Returns random element from sequence (simplified for lists)
static inline TauValue tauraro_random_choice(TauValue seq) {
    tauraro_random_ensure_init();

    if (seq.type != 4) {  // Not a list
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    TauList* list = seq.value.list;
    if (list->size == 0) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    size_t index = rand() % list->size;
    return list->items[index];
}

// random.seed(x) - Initialize random number generator
static inline TauValue tauraro_random_seed(TauValue x) {
    unsigned int seed;
    if (x.type == 0) {
        seed = (unsigned int)x.value.i;
    } else if (x.type == 1) {
        seed = (unsigned int)x.value.f;
    } else {
        seed = (unsigned int)time(NULL);
    }
    srand(seed);
    tauraro_random_initialized = 1;
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// ==========================================
// STATISTICAL DISTRIBUTIONS
// ==========================================

// random.gauss(mu, sigma) - Gaussian distribution
static inline TauValue tauraro_random_gauss(TauValue mu, TauValue sigma) {
    tauraro_random_ensure_init();
    double mean = (mu.type == 1) ? mu.value.f : (double)mu.value.i;
    double stddev = (sigma.type == 1) ? sigma.value.f : (double)sigma.value.i;

    // Box-Muller transform
    double u1 = (double)rand() / (double)RAND_MAX;
    double u2 = (double)rand() / (double)RAND_MAX;

    // Avoid log(0)
    if (u1 < 1e-10) u1 = 1e-10;

    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    double result = mean + stddev * z;

    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.normalvariate(mu, sigma) - Alias for gauss
static inline TauValue tauraro_random_normalvariate(TauValue mu, TauValue sigma) {
    return tauraro_random_gauss(mu, sigma);
}

// random.lognormvariate(mu, sigma) - Log-normal distribution
static inline TauValue tauraro_random_lognormvariate(TauValue mu, TauValue sigma) {
    TauValue gauss_val = tauraro_random_gauss(mu, sigma);
    double result = exp(gauss_val.value.f);
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.expovariate(lambda) - Exponential distribution
static inline TauValue tauraro_random_expovariate(TauValue lambd) {
    tauraro_random_ensure_init();
    double lambda = (lambd.type == 1) ? lambd.value.f : (double)lambd.value.i;

    if (lambda <= 0.0) lambda = 1.0;

    double u = (double)rand() / (double)RAND_MAX;
    if (u < 1e-10) u = 1e-10;

    double result = -log(u) / lambda;
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.gammavariate(alpha, beta) - Gamma distribution
static inline TauValue tauraro_random_gammavariate(TauValue alpha, TauValue beta) {
    tauraro_random_ensure_init();
    double a = (alpha.type == 1) ? alpha.value.f : (double)alpha.value.i;
    double b = (beta.type == 1) ? beta.value.f : (double)beta.value.i;

    if (a <= 0.0) a = 1.0;
    if (b <= 0.0) b = 1.0;

    // Simple gamma generation using rejection method (Marsaglia & Tsang)
    double result = 0.0;
    if (a >= 1.0) {
        double d = a - 1.0 / 3.0;
        double c = 1.0 / sqrt(9.0 * d);

        while (1) {
            double x, v;
            do {
                TauValue g = tauraro_random_gauss((TauValue){.type = 0, .value.i = 0},
                                                   (TauValue){.type = 0, .value.i = 1});
                x = g.value.f;
                v = 1.0 + c * x;
            } while (v <= 0.0);

            v = v * v * v;
            double u = (double)rand() / (double)RAND_MAX;

            if (u < 1.0 - 0.0331 * (x * x) * (x * x)) {
                result = d * v;
                break;
            }
            if (log(u) < 0.5 * x * x + d * (1.0 - v + log(v))) {
                result = d * v;
                break;
            }
        }
    } else {
        // For a < 1, use rejection method
        double u = (double)rand() / (double)RAND_MAX;
        TauValue gv = tauraro_random_gammavariate((TauValue){.type = 1, .value.f = 1.0 + a},
                                                   (TauValue){.type = 1, .value.f = b});
        result = gv.value.f * pow(u, 1.0 / a);
    }

    return (TauValue){.type = 1, .value.f = result * b, .refcount = 1, .next = NULL};
}

// random.betavariate(alpha, beta) - Beta distribution
static inline TauValue tauraro_random_betavariate(TauValue alpha, TauValue beta) {
    TauValue y1 = tauraro_random_gammavariate(alpha, (TauValue){.type = 1, .value.f = 1.0});
    TauValue y2 = tauraro_random_gammavariate(beta, (TauValue){.type = 1, .value.f = 1.0});

    double result = y1.value.f / (y1.value.f + y2.value.f);
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.paretovariate(alpha) - Pareto distribution
static inline TauValue tauraro_random_paretovariate(TauValue alpha) {
    tauraro_random_ensure_init();
    double a = (alpha.type == 1) ? alpha.value.f : (double)alpha.value.i;

    if (a <= 0.0) a = 1.0;

    double u = (double)rand() / (double)RAND_MAX;
    if (u < 1e-10) u = 1e-10;

    double result = pow(1.0 - u, -1.0 / a);
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.weibullvariate(alpha, beta) - Weibull distribution
static inline TauValue tauraro_random_weibullvariate(TauValue alpha, TauValue beta) {
    tauraro_random_ensure_init();
    double a = (alpha.type == 1) ? alpha.value.f : (double)alpha.value.i;
    double b = (beta.type == 1) ? beta.value.f : (double)beta.value.i;

    if (a <= 0.0) a = 1.0;
    if (b <= 0.0) b = 1.0;

    double u = (double)rand() / (double)RAND_MAX;
    if (u < 1e-10) u = 1e-10;

    double result = a * pow(-log(u), 1.0 / b);
    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// random.getrandbits(k) - Returns random integer with k random bits
static inline TauValue tauraro_random_getrandbits(TauValue k) {
    tauraro_random_ensure_init();
    int bits = (k.type == 0) ? (int)k.value.i : (int)k.value.f;

    if (bits <= 0) return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    if (bits > 63) bits = 63;  // Limit to int64 size

    int64_t result = 0;
    for (int i = 0; i < bits; i++) {
        result = (result << 1) | (rand() & 1);
    }

    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}
