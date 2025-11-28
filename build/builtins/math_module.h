// Tauraro math Module Header
// Auto-generated - DO NOT EDIT

#ifndef TAURARO_MATH_MODULE_H
#define TAURARO_MATH_MODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations from main tauraro runtime
typedef struct tauraro_value tauraro_value_t;
extern tauraro_value_t* tauraro_value_new(void);
extern tauraro_value_t* tauraro_none(void);
extern tauraro_value_t* tauraro_int(long long val);
extern tauraro_value_t* tauraro_float(double val);
extern tauraro_value_t* tauraro_bool(int val);
extern tauraro_value_t* tauraro_string(const char* val);

// Module initialization
void tauraro_math_module_init(void);

// Module attribute access
tauraro_value_t* tauraro_math_get_attr(const char* name);

// Module function declarations

extern double tauraro_math_pi;
extern double tauraro_math_e;
extern double tauraro_math_tau;
extern double tauraro_math_inf;
tauraro_value_t* tauraro_math_sqrt(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_pow(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_sin(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_cos(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_tan(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_log(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_log10(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_log2(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_exp(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_floor(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_ceil(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_abs(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_fabs(int argc, tauraro_value_t** argv);


#endif // TAURARO_MATH_MODULE_H
