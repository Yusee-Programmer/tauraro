//! Tauraro math builtin module - C implementation
//! Provides math functions compatible with Python's math module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// Forward declare tauraro_value_t (should match main program)
typedef enum {
    TAURARO_INT,
    TAURARO_FLOAT,
    TAURARO_BOOL,
    TAURARO_STRING,
    TAURARO_LIST,
    TAURARO_DICT,
    TAURARO_TUPLE,
    TAURARO_SET,
    TAURARO_NONE,
    TAURARO_OBJECT,
    TAURARO_FUNCTION,
    TAURARO_BYTES,
    TAURARO_COMPLEX,
    TAURARO_RANGE,
    TAURARO_FROZENSET
} tauraro_type_t;

typedef struct tauraro_value {
    tauraro_type_t type;
    int ref_count;
    union {
        int64_t int_val;
        double float_val;
        bool bool_val;
        char* str_val;
        void* ptr_val;
    } data;
} tauraro_value_t;

// Helper to create new value (declared in main program)
extern tauraro_value_t* tauraro_value_new(void);

// Math constants
double tauraro_math_pi = 3.141592653589793;
double tauraro_math_e = 2.718281828459045;

// sqrt(x) - Square root
tauraro_value_t* tauraro_math_sqrt(int argc, tauraro_value_t** argv) {
    if (argc < 1) {
        fprintf(stderr, "Error: sqrt() requires 1 argument\n");
        exit(1);
    }

    double x;
    if (argv[0]->type == TAURARO_INT) {
        x = (double)argv[0]->data.int_val;
    } else if (argv[0]->type == TAURARO_FLOAT) {
        x = argv[0]->data.float_val;
    } else {
        fprintf(stderr, "Error: sqrt() argument must be a number\n");
        exit(1);
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    result->data.float_val = sqrt(x);
    return result;
}

// pow(x, y) - x raised to power y
tauraro_value_t* tauraro_math_pow(int argc, tauraro_value_t** argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: pow() requires 2 arguments\n");
        exit(1);
    }

    double x, y;
    if (argv[0]->type == TAURARO_INT) {
        x = (double)argv[0]->data.int_val;
    } else if (argv[0]->type == TAURARO_FLOAT) {
        x = argv[0]->data.float_val;
    } else {
        fprintf(stderr, "Error: pow() first argument must be a number\n");
        exit(1);
    }

    if (argv[1]->type == TAURARO_INT) {
        y = (double)argv[1]->data.int_val;
    } else if (argv[1]->type == TAURARO_FLOAT) {
        y = argv[1]->data.float_val;
    } else {
        fprintf(stderr, "Error: pow() second argument must be a number\n");
        exit(1);
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    result->data.float_val = pow(x, y);
    return result;
}

// sin(x) - Sine
tauraro_value_t* tauraro_math_sin(int argc, tauraro_value_t** argv) {
    if (argc < 1) {
        fprintf(stderr, "Error: sin() requires 1 argument\n");
        exit(1);
    }

    double x;
    if (argv[0]->type == TAURARO_INT) {
        x = (double)argv[0]->data.int_val;
    } else if (argv[0]->type == TAURARO_FLOAT) {
        x = argv[0]->data.float_val;
    } else {
        fprintf(stderr, "Error: sin() argument must be a number\n");
        exit(1);
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    result->data.float_val = sin(x);
    return result;
}

// cos(x) - Cosine
tauraro_value_t* tauraro_math_cos(int argc, tauraro_value_t** argv) {
    if (argc < 1) {
        fprintf(stderr, "Error: cos() requires 1 argument\n");
        exit(1);
    }

    double x;
    if (argv[0]->type == TAURARO_INT) {
        x = (double)argv[0]->data.int_val;
    } else if (argv[0]->type == TAURARO_FLOAT) {
        x = argv[0]->data.float_val;
    } else {
        fprintf(stderr, "Error: cos() argument must be a number\n");
        exit(1);
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    result->data.float_val = cos(x);
    return result;
}

// tan(x) - Tangent
tauraro_value_t* tauraro_math_tan(int argc, tauraro_value_t** argv) {
    if (argc < 1) {
        fprintf(stderr, "Error: tan() requires 1 argument\n");
        exit(1);
    }

    double x;
    if (argv[0]->type == TAURARO_INT) {
        x = (double)argv[0]->data.int_val;
    } else if (argv[0]->type == TAURARO_FLOAT) {
        x = argv[0]->data.float_val;
    } else {
        fprintf(stderr, "Error: tan() argument must be a number\n");
        exit(1);
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    result->data.float_val = tan(x);
    return result;
}

// log(x) - Natural logarithm
tauraro_value_t* tauraro_math_log(int argc, tauraro_value_t** argv) {
    if (argc < 1) {
        fprintf(stderr, "Error: log() requires 1 argument\n");
        exit(1);
    }

    double x;
    if (argv[0]->type == TAURARO_INT) {
        x = (double)argv[0]->data.int_val;
    } else if (argv[0]->type == TAURARO_FLOAT) {
        x = argv[0]->data.float_val;
    } else {
        fprintf(stderr, "Error: log() argument must be a number\n");
        exit(1);
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    result->data.float_val = log(x);
    return result;
}

// exp(x) - e raised to power x
tauraro_value_t* tauraro_math_exp(int argc, tauraro_value_t** argv) {
    if (argc < 1) {
        fprintf(stderr, "Error: exp() requires 1 argument\n");
        exit(1);
    }

    double x;
    if (argv[0]->type == TAURARO_INT) {
        x = (double)argv[0]->data.int_val;
    } else if (argv[0]->type == TAURARO_FLOAT) {
        x = argv[0]->data.float_val;
    } else {
        fprintf(stderr, "Error: exp() argument must be a number\n");
        exit(1);
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    result->data.float_val = exp(x);
    return result;
}
