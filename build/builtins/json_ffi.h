// Tauraro json FFI Module Header
// Auto-generated - Documents the Rust FFI exports
// The actual implementation is compiled from src/builtins_ffi/json_ffi.rs

#ifndef TAURARO_JSON_FFI_H
#define TAURARO_JSON_FFI_H

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

// JSON functions
tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv);


#endif // TAURARO_JSON_FFI_H
