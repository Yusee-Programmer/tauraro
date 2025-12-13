// Tauraro os FFI Module Header
// Auto-generated - Documents the Rust FFI exports
// The actual implementation is compiled from src/builtins_ffi/os_ffi.rs

#ifndef TAURARO_OS_FFI_H
#define TAURARO_OS_FFI_H

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

// OS functions
tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_chdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_listdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_mkdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rmdir(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_remove(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_rename(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_getenv(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_system(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_exists(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isfile(int argc, tauraro_value_t** argv);
tauraro_value_t* tauraro_os_path_isdir(int argc, tauraro_value_t** argv);


#endif // TAURARO_OS_FFI_H
