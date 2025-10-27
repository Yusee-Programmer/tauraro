#ifndef TAURARO_MYMATH_H
#define TAURARO_MYMATH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>

#ifndef TAURARO_TYPES_DEFINED
#define TAURARO_TYPES_DEFINED

// Tauraro runtime data structures
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
        struct tauraro_list* list_val;
        struct tauraro_dict* dict_val;
        struct tauraro_tuple* tuple_val;
        struct tauraro_set* set_val;
        struct tauraro_object* obj_val;
        struct tauraro_function* func_val;
        struct tauraro_bytes* bytes_val;
        struct tauraro_complex* complex_val;
        struct tauraro_range* range_val;
        void* ptr_val;
    } data;
} tauraro_value_t;

typedef struct tauraro_list {
    tauraro_value_t** items;
    size_t size;
    size_t capacity;
} tauraro_list_t;

typedef struct tauraro_dict {
    char** keys;
    tauraro_value_t** values;
    size_t size;
    size_t capacity;
} tauraro_dict_t;

typedef struct tauraro_tuple {
    tauraro_value_t** items;
    size_t size;
    size_t capacity;
} tauraro_tuple_t;

typedef struct tauraro_set {
    tauraro_value_t** items;
    size_t size;
    size_t capacity;
} tauraro_set_t;

typedef struct tauraro_bytes {
    unsigned char* data;
    size_t size;
} tauraro_bytes_t;

typedef struct tauraro_complex {
    double real;
    double imag;
} tauraro_complex_t;

typedef struct tauraro_range {
    int64_t start;
    int64_t stop;
    int64_t step;
} tauraro_range_t;

typedef struct tauraro_function {
    char* name;
    void* func_ptr;
    tauraro_value_t** defaults;
    size_t num_defaults;
    struct tauraro_dict* closure;
} tauraro_function_t;


#endif // TAURARO_TYPES_DEFINED

#ifndef TAURARO_OOP_DEFINED
#define TAURARO_OOP_DEFINED

// OOP struct definitions

typedef struct tauraro_object {
    char* class_name;
    struct tauraro_class* class_ptr;
    struct tauraro_dict* fields;
    struct tauraro_dict* methods;
    struct tauraro_list* bases;
} tauraro_object_t;

typedef struct tauraro_class {
    char* name;
    struct tauraro_list* bases;
    struct tauraro_list* mro;  // Method Resolution Order
    struct tauraro_dict* methods;
    struct tauraro_dict* class_attrs;
    struct tauraro_dict* properties;
} tauraro_class_t;


#endif // TAURARO_OOP_DEFINED

#ifndef TAURARO_RUNTIME_DECLARED
#define TAURARO_RUNTIME_DECLARED

// Runtime support functions
tauraro_value_t* tauraro_value_new(void);
tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_print(int argc, tauraro_value_t** argv);

#endif // TAURARO_RUNTIME_DECLARED

// Module: mymath - Global variables and comments
// User-defined module for mathematical operations
tauraro_value_t* mymath_PI = NULL;
tauraro_value_t* mymath_E = NULL;

// Module: mymath - Function implementations

tauraro_value_t* mymath_cube(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* x = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* binop_left = x;
    tauraro_value_t* binop_right = x;
    tauraro_value_t* temp_result = tauraro_mul(binop_left, binop_right);
    tauraro_value_t* result = temp_result;
    tauraro_value_t* binop_left_1 = result;
    tauraro_value_t* binop_right_1 = x;
    tauraro_value_t* temp_result_1 = tauraro_mul(binop_left, binop_right);
    result = temp_result;
    tauraro_value_t* temp_result_2 = result;
    return temp_result;
}


tauraro_value_t* mymath_square(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* x = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* binop_left = x;
    tauraro_value_t* binop_right = x;
    tauraro_value_t* temp_result = tauraro_mul(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* mymath_multiply(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* a = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* b = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* binop_left = a;
    tauraro_value_t* binop_right = b;
    tauraro_value_t* temp_result = tauraro_mul(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* mymath_add(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* a = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* b = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* binop_left = a;
    tauraro_value_t* binop_right = b;
    tauraro_value_t* temp_result = tauraro_add(binop_left, binop_right);
    return temp_result;
}


#endif // TAURARO_MYMATH_H
