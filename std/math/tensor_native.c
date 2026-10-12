/*
 * std/math/tensor_native.c -- strided kernels for std.math.tensor.
 *
 * Linked through `# tauraro-cpp-shim: tensor_native.c` in std/math/tensor.tr.
 * Tensors are float64 data with a shape and element strides; operands may be
 * non-contiguous views (transposes, slices, broadcasts with stride 0). Every
 * kernel writes a fresh contiguous (row-major) output.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#define T_MAXDIM 32

/* odometer over `shape`, tracking one input offset (and one more if b) */
static int t_next(int64_t nd, const int64_t* shape, int64_t* idx, int64_t* ao, const int64_t* ast, int64_t* bo, const int64_t* bst) {
    int64_t d;
    for (d = nd - 1; d >= 0; d--) {
        idx[d]++;
        *ao += ast[d];
        if (bo) *bo += bst[d];
        if (idx[d] < shape[d]) return 1;
        *ao -= ast[d] * shape[d];
        if (bo) *bo -= bst[d] * shape[d];
        idx[d] = 0;
    }
    return 0;
}
static int64_t t_count(int64_t nd, const int64_t* shape) {
    int64_t n = 1, d;
    for (d = 0; d < nd; d++) n *= shape[d];
    return n;
}

static double t_bin(int64_t op, double x, double y) {
    switch (op) {
        case 0: return x + y;
        case 1: return x - y;
        case 2: return x * y;
        case 3: return x / y;
        case 4: return pow(x, y);
        case 5: return x > y ? x : y;
        case 6: return x < y ? x : y;
        case 7: return x == y ? 1.0 : 0.0;
        case 8: return x != y ? 1.0 : 0.0;
        case 9: return x < y ? 1.0 : 0.0;
        case 10: return x <= y ? 1.0 : 0.0;
        case 11: return x > y ? 1.0 : 0.0;
        case 12: return x >= y ? 1.0 : 0.0;
        case 13: return atan2(x, y);
        case 14: return fmod(x, y);
        case 15: return (x != 0.0 && y != 0.0) ? 1.0 : 0.0;
        case 16: return (x != 0.0 || y != 0.0) ? 1.0 : 0.0;
    }
    return 0.0;
}

/* out (contiguous, prod(shape) elements) = a OP b, with per-operand strides
 * (stride 0 = broadcast). */
void trc_t_binary(int64_t op, int64_t nd, const int64_t* shape, double* out,
                  const double* a, int64_t aoff, const int64_t* ast,
                  const double* b, int64_t boff, const int64_t* bst) {
    int64_t idx[T_MAXDIM] = {0}, ao = aoff, bo = boff, i = 0, n = t_count(nd, shape);
    if (n == 0) return;
    if (nd == 0) { out[0] = t_bin(op, a[aoff], b[boff]); return; }
    do { out[i++] = t_bin(op, a[ao], b[bo]); } while (t_next(nd, shape, idx, &ao, ast, &bo, bst));
}

static double t_un(int64_t op, double x, double p) {
    switch (op) {
        case 0: return x;
        case 1: return -x;
        case 2: return fabs(x);
        case 3: return sqrt(x);
        case 4: return exp(x);
        case 5: return log(x);
        case 6: return sin(x);
        case 7: return cos(x);
        case 8: return tan(x);
        case 9: return tanh(x);
        case 10: return x > 0 ? x : 0.0;
        case 11: return 1.0 / (1.0 + exp(-x));
        case 12: return floor(x);
        case 13: return ceil(x);
        case 14: return round(x);
        case 15: return x * x;
        case 16: return x + p;
        case 17: return x * p;
        case 18: return pow(x, p);
        case 19: return p - x;
        case 20: return p / x;
        case 21: return x > p ? x : p;
        case 22: return x < p ? x : p;
        case 23: return x > p ? 1.0 : 0.0;
        case 24: return x < p ? 1.0 : 0.0;
        case 25: return x == p ? 1.0 : 0.0;
        case 26: return log1p(x);
        case 27: return expm1(x);
        case 28: return x / p;
        case 29: return x - p;
        case 30: return x >= 0 ? (x > 0 ? 1.0 : 0.0) : -1.0;
        case 31: return log2(x);
        case 32: return log10(x);
        case 33: return x > 0 ? x : p * x;        /* leaky relu */
        case 34: return asin(x);
        case 35: return acos(x);
        case 36: return atan(x);
    }
    return x;
}

void trc_t_unary(int64_t op, int64_t nd, const int64_t* shape, double* out,
                 const double* a, int64_t aoff, const int64_t* ast, double p) {
    int64_t idx[T_MAXDIM] = {0}, ao = aoff, i = 0, n = t_count(nd, shape);
    if (n == 0) return;
    if (nd == 0) { out[0] = t_un(op, a[aoff], p); return; }
    do { out[i++] = t_un(op, a[ao], p); } while (t_next(nd, shape, idx, &ao, ast, NULL, NULL));
}

/* Reduction over all elements. op: 0 sum, 1 prod, 2 max, 3 min, 4 argmax, 5 argmin
 * (flat row-major index), 6 sum of squares, 7 count nonzero. */
double trc_t_reduce_all(int64_t op, int64_t nd, const int64_t* shape, const double* a, int64_t aoff, const int64_t* ast) {
    int64_t idx[T_MAXDIM] = {0}, ao = aoff, i = 0, best_i = 0, n = t_count(nd, shape);
    double acc = op == 1 ? 1.0 : 0.0, best = 0.0; int first = 1;
    if (n == 0) return op == 2 ? -INFINITY : op == 3 ? INFINITY : (op == 4 || op == 5) ? -1.0 : acc;
    if (nd == 0) n = 1;
    do {
        double v = a[ao];
        switch (op) {
            case 0: acc += v; break;
            case 1: acc *= v; break;
            case 6: acc += v * v; break;
            case 7: acc += v != 0.0; break;
            default:
                if (first || ((op == 2 || op == 4) ? v > best : v < best) || (best != best && v == v)) { best = v; best_i = i; }
                first = 0;
        }
        i++;
        if (nd == 0) break;
    } while (t_next(nd, shape, idx, &ao, ast, NULL, NULL));
    if (op == 2 || op == 3) return best;
    if (op == 4 || op == 5) return (double)best_i;
    return acc;
}

/* Reduction along `axis`; out has prod(shape)/shape[axis] elements (row-major
 * over the remaining dims). argmax/argmin give the index along the axis. */
void trc_t_reduce_axis(int64_t op, int64_t nd, const int64_t* shape, int64_t axis, double* out,
                       const double* a, int64_t aoff, const int64_t* ast) {
    int64_t oshape[T_MAXDIM], ostr[T_MAXDIM], idx[T_MAXDIM] = {0}, d, k = 0, ao = aoff, i = 0, len = shape[axis], step = ast[axis];
    int64_t n;
    for (d = 0; d < nd; d++) if (d != axis) { oshape[k] = shape[d]; ostr[k] = ast[d]; k++; }
    n = t_count(nd - 1, oshape);
    if (n == 0) return;
    do {
        int64_t j, p = ao, bi = 0; double acc = op == 1 ? 1.0 : 0.0, best = 0.0;
        for (j = 0; j < len; j++, p += step) {
            double v = a[p];
            switch (op) {
                case 0: acc += v; break;
                case 1: acc *= v; break;
                case 6: acc += v * v; break;
                case 7: acc += v != 0.0; break;
                default:
                    if (j == 0 || ((op == 2 || op == 4) ? v > best : v < best)) { best = v; bi = j; }
            }
        }
        if (op == 2 || op == 3) acc = len ? best : (op == 2 ? -INFINITY : INFINITY);
        if (op == 4 || op == 5) acc = (double)(len ? bi : -1);
        out[i++] = acc;
        if (nd - 1 == 0) break;
    } while (t_next(nd - 1, oshape, idx, &ao, ostr, NULL, NULL));
}

/* C[m x n] = A[m x k] . B[k x n] for strided A and B; C contiguous. */
void trc_t_matmul(int64_t m, int64_t k, int64_t n,
                  const double* a, int64_t aoff, int64_t ars, int64_t acs,
                  const double* b, int64_t boff, int64_t brs, int64_t bcs, double* c) {
    int64_t i, j, p, ii, pp;
    const int64_t BS = 64;
    memset(c, 0, sizeof(double) * (size_t)(m * n));
    for (ii = 0; ii < m; ii += BS)
        for (pp = 0; pp < k; pp += BS) {
            int64_t imax = ii + BS < m ? ii + BS : m, pmax = pp + BS < k ? pp + BS : k;
            for (i = ii; i < imax; i++) {
                double* crow = c + i * n;
                for (p = pp; p < pmax; p++) {
                    double av = a[aoff + i * ars + p * acs];
                    const double* brow = b + boff + p * brs;
                    if (av == 0.0) continue;
                    if (bcs == 1) { for (j = 0; j < n; j++) crow[j] += av * brow[j]; }
                    else { for (j = 0; j < n; j++) crow[j] += av * brow[j * bcs]; }
                }
            }
        }
}
