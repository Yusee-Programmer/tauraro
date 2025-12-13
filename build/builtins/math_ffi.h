// Tauraro math FFI Module Header
// Auto-generated - Documents the Rust FFI exports
// The actual implementation is compiled from src/builtins_ffi/math_ffi.rs

#ifndef TAURARO_MATH_FFI_H
#define TAURARO_MATH_FFI_H

#include <stdint.h>

// Tauraro value type enumeration
typedef enum {
    TAURARO_INT = 0,
    TAURARO_FLOAT = 1,
    TAURARO_BOOL = 2,
    TAURARO_STRING = 3,
    TAURARO_LIST = 4,
    TAURARO_DICT = 5,
    TAURARO_TUPLE = 6,
    TAURARO_SET = 7,
    TAURARO_NONE = 8,
    TAURARO_OBJECT = 9,
    TAURARO_FUNCTION = 10,
    TAURARO_BYTES = 11,
    TAURARO_COMPLEX = 12,
    TAURARO_RANGE = 13,
    TAURARO_FROZENSET = 14,
} tauraro_type_t;

// Tauraro value data union
typedef union {
    int64_t int_val;
    double float_val;
    int bool_val;
    char* str_val;
    void* ptr_val;
} tauraro_data_t;

// Tauraro value structure
typedef struct tauraro_value {
    tauraro_type_t type;
    int ref_count;
    tauraro_data_t data;
} tauraro_value_t;

// External: value allocation (must be provided by main program)
extern tauraro_value_t* tauraro_value_new(void);

// Module-specific FFI exports

// Math constants
extern double tauraro_math_pi;
extern double tauraro_math_e;
extern double tauraro_math_tau;
extern double tauraro_math_inf;
extern double tauraro_math_nan;

// Math functions
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
tauraro_value_t* tauraro_math_fabs(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_asin(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_acos(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_atan(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_atan2(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_sinh(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_cosh(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_math_tanh(int argc, tauraro_value_t** argv);


#endif // TAURARO_MATH_FFI_H
