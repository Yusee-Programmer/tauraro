#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef TAURARO_VALUE_T_DEFINED
#define TAURARO_VALUE_T_DEFINED
typedef void tauraro_value_t;
#endif


// Math module constants
double tauraro_math_pi = 3.14159265358979323846;
double tauraro_math_e = 2.71828182845904523536;

// sqrt function
double tauraro_math_sqrt_native(double x) {
    return sqrt(x);
}

// pow function
double tauraro_math_pow_native(double x, double y) {
    return pow(x, y);
}

// sin function
double tauraro_math_sin_native(double x) {
    return sin(x);
}

// cos function
double tauraro_math_cos_native(double x) {
    return cos(x);
}

// tan function
double tauraro_math_tan_native(double x) {
    return tan(x);
}

// log function
double tauraro_math_log_native(double x) {
    return log(x);
}

// exp function
double tauraro_math_exp_native(double x) {
    return exp(x);
}

// floor function
double tauraro_math_floor_native(double x) {
    return floor(x);
}

// ceil function
double tauraro_math_ceil_native(double x) {
    return ceil(x);
}
