// Tauraro Complete Operators Implementation
// All arithmetic, bitwise, comparison, and logical operators

#ifndef TAURARO_OPERATORS_H
#define TAURARO_OPERATORS_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// Forward declaration of value type
typedef struct tauraro_value tauraro_value_t;
typedef enum tauraro_type tauraro_type_t;

// ============================================================================
// BITWISE OPERATORS
// ============================================================================

// Bitwise AND
static inline int64_t tauraro_bitwise_and_int(int64_t left, int64_t right) {
    return left & right;
}

// Bitwise OR
static inline int64_t tauraro_bitwise_or_int(int64_t left, int64_t right) {
    return left | right;
}

// Bitwise XOR
static inline int64_t tauraro_bitwise_xor_int(int64_t left, int64_t right) {
    return left ^ right;
}

// Bitwise NOT
static inline int64_t tauraro_bitwise_not_int(int64_t value) {
    return ~value;
}

// Left shift
static inline int64_t tauraro_left_shift_int(int64_t left, int64_t right) {
    // Python-like behavior: negative shift raises exception, we'll clamp to 0-63
    if (right < 0 || right > 63) return left;
    return left << right;
}

// Right shift
static inline int64_t tauraro_right_shift_int(int64_t left, int64_t right) {
    // Python-like behavior: negative shift raises exception, we'll clamp to 0-63
    if (right < 0 || right > 63) return left;
    return left >> right;
}

// ============================================================================
// POWER OPERATOR
// ============================================================================

// Integer power (optimized for integer exponents)
static inline int64_t tauraro_power_int(int64_t base, int64_t exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    if (exp == 2) return base * base;

    // Handle negative exponents (return 0 for integers like Python)
    if (exp < 0) return 0;

    // Fast exponentiation by squaring
    int64_t result = 1;
    int64_t current_base = base;
    int64_t current_exp = exp;

    while (current_exp > 0) {
        if (current_exp % 2 == 1) {
            result *= current_base;
        }
        current_base *= current_base;
        current_exp /= 2;
    }

    return result;
}

// Float power (uses math.h pow)
static inline double tauraro_power_float(double base, double exp) {
    return pow(base, exp);
}

// ============================================================================
// FLOOR DIVISION
// ============================================================================

// Integer floor division
static inline int64_t tauraro_floor_div_int(int64_t left, int64_t right) {
    if (right == 0) return 0; // Avoid division by zero

    // Python-like floor division (rounds toward negative infinity)
    int64_t quotient = left / right;
    int64_t remainder = left % right;

    // Adjust if signs differ and there's a remainder
    if ((remainder != 0) && ((left < 0) != (right < 0))) {
        quotient -= 1;
    }

    return quotient;
}

// Float floor division
static inline double tauraro_floor_div_float(double left, double right) {
    if (right == 0.0) return 0.0; // Avoid division by zero
    return floor(left / right);
}

// ============================================================================
// MODULO (Enhanced for negative numbers)
// ============================================================================

// Python-like modulo (always matches sign of divisor)
static inline int64_t tauraro_mod_int(int64_t left, int64_t right) {
    if (right == 0) return 0; // Avoid division by zero

    int64_t result = left % right;

    // Python behavior: result has same sign as divisor
    if ((result != 0) && ((left < 0) != (right < 0))) {
        result += right;
    }

    return result;
}

// Float modulo
static inline double tauraro_mod_float(double left, double right) {
    if (right == 0.0) return 0.0; // Avoid division by zero

    double result = fmod(left, right);

    // Python behavior: result has same sign as divisor
    if ((result != 0.0) && ((left < 0.0) != (right < 0.0))) {
        result += right;
    }

    return result;
}

// ============================================================================
// COMPARISON OPERATORS (Enhanced)
// ============================================================================

// String comparison
static inline bool tauraro_str_eq(const char* left, const char* right) {
    return strcmp(left, right) == 0;
}

static inline bool tauraro_str_ne(const char* left, const char* right) {
    return strcmp(left, right) != 0;
}

static inline bool tauraro_str_lt(const char* left, const char* right) {
    return strcmp(left, right) < 0;
}

static inline bool tauraro_str_le(const char* left, const char* right) {
    return strcmp(left, right) <= 0;
}

static inline bool tauraro_str_gt(const char* left, const char* right) {
    return strcmp(left, right) > 0;
}

static inline bool tauraro_str_ge(const char* left, const char* right) {
    return strcmp(left, right) >= 0;
}

// ============================================================================
// IN-PLACE OPERATORS (+=, -=, *=, /=, %=, //=, **=, &=, |=, ^=, <<=, >>=)
// ============================================================================

// In-place integer addition
static inline int64_t tauraro_iadd_int(int64_t* left, int64_t right) {
    *left += right;
    return *left;
}

// In-place integer subtraction
static inline int64_t tauraro_isub_int(int64_t* left, int64_t right) {
    *left -= right;
    return *left;
}

// In-place integer multiplication
static inline int64_t tauraro_imul_int(int64_t* left, int64_t right) {
    *left *= right;
    return *left;
}

// In-place integer division
static inline int64_t tauraro_idiv_int(int64_t* left, int64_t right) {
    if (right != 0) {
        *left /= right;
    }
    return *left;
}

// In-place integer modulo
static inline int64_t tauraro_imod_int(int64_t* left, int64_t right) {
    if (right != 0) {
        *left = tauraro_mod_int(*left, right);
    }
    return *left;
}

// In-place integer floor division
static inline int64_t tauraro_ifloordiv_int(int64_t* left, int64_t right) {
    if (right != 0) {
        *left = tauraro_floor_div_int(*left, right);
    }
    return *left;
}

// In-place integer power
static inline int64_t tauraro_ipow_int(int64_t* left, int64_t right) {
    *left = tauraro_power_int(*left, right);
    return *left;
}

// In-place bitwise AND
static inline int64_t tauraro_iand_int(int64_t* left, int64_t right) {
    *left &= right;
    return *left;
}

// In-place bitwise OR
static inline int64_t tauraro_ior_int(int64_t* left, int64_t right) {
    *left |= right;
    return *left;
}

// In-place bitwise XOR
static inline int64_t tauraro_ixor_int(int64_t* left, int64_t right) {
    *left ^= right;
    return *left;
}

// In-place left shift
static inline int64_t tauraro_ilshift_int(int64_t* left, int64_t right) {
    if (right >= 0 && right <= 63) {
        *left <<= right;
    }
    return *left;
}

// In-place right shift
static inline int64_t tauraro_irshift_int(int64_t* left, int64_t right) {
    if (right >= 0 && right <= 63) {
        *left >>= right;
    }
    return *left;
}

// ============================================================================
// FLOAT IN-PLACE OPERATORS
// ============================================================================

// In-place float addition
static inline double tauraro_iadd_float(double* left, double right) {
    *left += right;
    return *left;
}

// In-place float subtraction
static inline double tauraro_isub_float(double* left, double right) {
    *left -= right;
    return *left;
}

// In-place float multiplication
static inline double tauraro_imul_float(double* left, double right) {
    *left *= right;
    return *left;
}

// In-place float division
static inline double tauraro_idiv_float(double* left, double right) {
    if (right != 0.0) {
        *left /= right;
    }
    return *left;
}

// In-place float modulo
static inline double tauraro_imod_float(double* left, double right) {
    if (right != 0.0) {
        *left = tauraro_mod_float(*left, right);
    }
    return *left;
}

// In-place float floor division
static inline double tauraro_ifloordiv_float(double* left, double right) {
    if (right != 0.0) {
        *left = tauraro_floor_div_float(*left, right);
    }
    return *left;
}

// In-place float power
static inline double tauraro_ipow_float(double* left, double right) {
    *left = pow(*left, right);
    return *left;
}

// ============================================================================
// UNARY OPERATORS
// ============================================================================

// Unary plus (no-op for numbers)
static inline int64_t tauraro_unary_plus_int(int64_t value) {
    return value;
}

static inline double tauraro_unary_plus_float(double value) {
    return value;
}

// Unary minus (negation)
static inline int64_t tauraro_unary_minus_int(int64_t value) {
    return -value;
}

static inline double tauraro_unary_minus_float(double value) {
    return -value;
}

// Unary not (logical negation)
static inline bool tauraro_unary_not(bool value) {
    return !value;
}

// ============================================================================
// LOGICAL OPERATORS (Short-circuit simulation)
// ============================================================================

// Logical AND (short-circuit: only evaluate right if left is true)
#define TAURARO_LOGICAL_AND(left, right) ((left) ? (right) : (left))

// Logical OR (short-circuit: only evaluate right if left is false)
#define TAURARO_LOGICAL_OR(left, right) ((left) ? (left) : (right))

// ============================================================================
// MEMBERSHIP OPERATORS (in, not in)
// ============================================================================

// Check if integer is in list
static inline bool tauraro_in_list_int(int64_t value, const int64_t* list, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (list[i] == value) return true;
    }
    return false;
}

// Check if string is in list of strings
static inline bool tauraro_in_list_str(const char* value, const char** list, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (strcmp(list[i], value) == 0) return true;
    }
    return false;
}

// Check if substring is in string
static inline bool tauraro_in_string(const char* substring, const char* string) {
    return strstr(string, substring) != NULL;
}

// ============================================================================
// IDENTITY OPERATORS (is, is not)
// ============================================================================

// Check pointer identity
static inline bool tauraro_is(const void* left, const void* right) {
    return left == right;
}

static inline bool tauraro_is_not(const void* left, const void* right) {
    return left != right;
}

// ============================================================================
// CHAINED COMPARISONS (a < b < c)
// ============================================================================

// Helper for chained integer comparisons
static inline bool tauraro_chained_lt_int(int64_t a, int64_t b, int64_t c) {
    return (a < b) && (b < c);
}

static inline bool tauraro_chained_le_int(int64_t a, int64_t b, int64_t c) {
    return (a <= b) && (b <= c);
}

static inline bool tauraro_chained_gt_int(int64_t a, int64_t b, int64_t c) {
    return (a > b) && (b > c);
}

static inline bool tauraro_chained_ge_int(int64_t a, int64_t b, int64_t c) {
    return (a >= b) && (b >= c);
}

// ============================================================================
// TERNARY OPERATOR (inline conditional)
// ============================================================================

// Inline ternary for integers
static inline int64_t tauraro_ternary_int(bool condition, int64_t if_true, int64_t if_false) {
    return condition ? if_true : if_false;
}

// Inline ternary for floats
static inline double tauraro_ternary_float(bool condition, double if_true, double if_false) {
    return condition ? if_true : if_false;
}

// ============================================================================
// DIVMOD (Return quotient and remainder together)
// ============================================================================

typedef struct {
    int64_t quotient;
    int64_t remainder;
} tauraro_divmod_result_t;

// Integer divmod
static inline tauraro_divmod_result_t tauraro_divmod_int(int64_t left, int64_t right) {
    tauraro_divmod_result_t result;
    if (right == 0) {
        result.quotient = 0;
        result.remainder = 0;
    } else {
        result.quotient = tauraro_floor_div_int(left, right);
        result.remainder = tauraro_mod_int(left, right);
    }
    return result;
}

// ============================================================================
// ABS, ROUND, CEIL, FLOOR (Fast inline versions)
// ============================================================================

// Fast absolute value
static inline int64_t tauraro_abs_int(int64_t value) {
    return (value < 0) ? -value : value;
}

static inline double tauraro_abs_float(double value) {
    return fabs(value);
}

// Fast rounding
static inline int64_t tauraro_round_float_to_int(double value) {
    return (int64_t)round(value);
}

static inline double tauraro_round_float(double value, int digits) {
    double multiplier = pow(10.0, digits);
    return round(value * multiplier) / multiplier;
}

// Fast ceiling
static inline int64_t tauraro_ceil_float_to_int(double value) {
    return (int64_t)ceil(value);
}

// Fast floor
static inline int64_t tauraro_floor_float_to_int(double value) {
    return (int64_t)floor(value);
}

// ============================================================================
// MIN/MAX (Fast inline versions)
// ============================================================================

// Fast integer min
static inline int64_t tauraro_min_int(int64_t a, int64_t b) {
    return (a < b) ? a : b;
}

// Fast integer max
static inline int64_t tauraro_max_int(int64_t a, int64_t b) {
    return (a > b) ? a : b;
}

// Fast float min
static inline double tauraro_min_float(double a, double b) {
    return (a < b) ? a : b;
}

// Fast float max
static inline double tauraro_max_float(double a, double b) {
    return (a > b) ? a : b;
}

// ============================================================================
// CLAMP (Constrain value to range)
// ============================================================================

// Clamp integer to range
static inline int64_t tauraro_clamp_int(int64_t value, int64_t min_val, int64_t max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// Clamp float to range
static inline double tauraro_clamp_float(double value, double min_val, double max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

#endif // TAURARO_OPERATORS_H
