// Tauraro Math Module Implementation
// Auto-generated C implementation for the math built-in module

#include "math_module.h"
#include <math.h>

// Math constants
double tauraro_math_pi = 3.141592653589793;
double tauraro_math_e = 2.718281828459045;
double tauraro_math_tau = 6.283185307179586;
double tauraro_math_inf = 1.0 / 0.0;  // INFINITY

static int math_initialized = 0;

void tauraro_math_module_init(void) {
    if (math_initialized) return;
    math_initialized = 1;
}

// Helper to get numeric value
static double get_number(tauraro_value_t* val) {
    if (val == NULL) return 0.0;
    if (val->type == 0) return (double)val->data.int_val;  // TAURARO_INT
    if (val->type == 1) return val->data.float_val;        // TAURARO_FLOAT
    return 0.0;
}

tauraro_value_t* tauraro_math_sqrt(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x < 0) return tauraro_none();
    return tauraro_float(sqrt(x));
}

tauraro_value_t* tauraro_math_pow(int argc, tauraro_value_t** argv) {
    if (argc < 2 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    double y = get_number(argv[1]);
    return tauraro_float(pow(x, y));
}

tauraro_value_t* tauraro_math_sin(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(sin(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_cos(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(cos(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_tan(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(tan(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_log(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x <= 0) return tauraro_none();
    return tauraro_float(log(x));
}

tauraro_value_t* tauraro_math_log10(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x <= 0) return tauraro_none();
    return tauraro_float(log10(x));
}

tauraro_value_t* tauraro_math_log2(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    if (x <= 0) return tauraro_none();
    return tauraro_float(log2(x));
}

tauraro_value_t* tauraro_math_exp(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(exp(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_floor(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(floor(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_ceil(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(ceil(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_abs(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    double x = get_number(argv[0]);
    return tauraro_float(fabs(x));
}

tauraro_value_t* tauraro_math_fabs(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL) return tauraro_none();
    return tauraro_float(fabs(get_number(argv[0])));
}

tauraro_value_t* tauraro_math_get_attr(const char* name) {
    if (strcmp(name, "pi") == 0) return tauraro_float(tauraro_math_pi);
    if (strcmp(name, "e") == 0) return tauraro_float(tauraro_math_e);
    if (strcmp(name, "tau") == 0) return tauraro_float(tauraro_math_tau);
    if (strcmp(name, "inf") == 0) return tauraro_float(tauraro_math_inf);
    // For functions, return a function pointer wrapper
    return tauraro_none();
}
