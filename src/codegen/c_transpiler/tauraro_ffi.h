// FFI Runtime Support for Tauraro C Transpiler
// Provides dynamic library loading and foreign function calling

#ifndef TAURARO_FFI_H
#define TAURARO_FFI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Platform-specific includes for dynamic loading
#ifdef _WIN32
    #include <windows.h>
    typedef HMODULE library_handle_t;
    #define LOAD_LIBRARY(name) LoadLibraryA(name)
    #define GET_FUNCTION(handle, name) GetProcAddress(handle, name)
    #define CLOSE_LIBRARY(handle) FreeLibrary(handle)
    #define LIBRARY_ERROR() "Windows library error"
#else
    #include <dlfcn.h>
    typedef void* library_handle_t;
    #define LOAD_LIBRARY(name) dlopen(name, RTLD_LAZY)
    #define GET_FUNCTION(handle, name) dlsym(handle, name)
    #define CLOSE_LIBRARY(handle) dlclose(handle)
    #define LIBRARY_ERROR() dlerror()
#endif

// Forward declaration
struct tauraro_value;
typedef struct tauraro_value tauraro_value_t;

// FFI type enumeration
typedef enum {
    FFI_TYPE_VOID,
    FFI_TYPE_INT,
    FFI_TYPE_INT8,
    FFI_TYPE_INT16,
    FFI_TYPE_INT32,
    FFI_TYPE_INT64,
    FFI_TYPE_UINT,
    FFI_TYPE_UINT8,
    FFI_TYPE_UINT16,
    FFI_TYPE_UINT32,
    FFI_TYPE_UINT64,
    FFI_TYPE_FLOAT,
    FFI_TYPE_DOUBLE,
    FFI_TYPE_CHAR,
    FFI_TYPE_STRING,
    FFI_TYPE_POINTER,
    FFI_TYPE_BOOL,
} ffi_type_enum;

// Loaded library structure
typedef struct {
    char* name;
    library_handle_t handle;
} tauraro_ffi_library_t;

// Function signature structure
typedef struct {
    char* name;
    void* func_ptr;
    ffi_type_enum return_type;
    ffi_type_enum* param_types;
    int param_count;
} tauraro_ffi_function_t;

// FFI Manager structure
typedef struct {
    tauraro_ffi_library_t** libraries;
    int library_count;
    int library_capacity;
    tauraro_ffi_function_t** functions;
    int function_count;
    int function_capacity;
} tauraro_ffi_manager_t;

// Global FFI manager (singleton)
static tauraro_ffi_manager_t* global_ffi_manager = NULL;

// Initialize FFI manager
static tauraro_ffi_manager_t* tauraro_ffi_init() {
    if (global_ffi_manager != NULL) {
        return global_ffi_manager;
    }

    tauraro_ffi_manager_t* manager = (tauraro_ffi_manager_t*)malloc(sizeof(tauraro_ffi_manager_t));
    manager->library_capacity = 16;
    manager->library_count = 0;
    manager->libraries = (tauraro_ffi_library_t**)malloc(sizeof(tauraro_ffi_library_t*) * manager->library_capacity);

    manager->function_capacity = 64;
    manager->function_count = 0;
    manager->functions = (tauraro_ffi_function_t**)malloc(sizeof(tauraro_ffi_function_t*) * manager->function_capacity);

    global_ffi_manager = manager;
    return manager;
}

// Find library by name
static tauraro_ffi_library_t* tauraro_ffi_find_library(const char* name) {
    tauraro_ffi_manager_t* manager = tauraro_ffi_init();

    for (int i = 0; i < manager->library_count; i++) {
        if (strcmp(manager->libraries[i]->name, name) == 0) {
            return manager->libraries[i];
        }
    }
    return NULL;
}

// Load a dynamic library
static tauraro_value_t* tauraro_ffi_load_library(const char* library_name) {
    tauraro_ffi_manager_t* manager = tauraro_ffi_init();

    // Check if already loaded
    if (tauraro_ffi_find_library(library_name) != NULL) {
        printf("Library already loaded: %s\n", library_name);
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    // Try to load the library
    library_handle_t handle = LOAD_LIBRARY(library_name);
    if (handle == NULL) {
        fprintf(stderr, "Failed to load library %s: %s\n", library_name, LIBRARY_ERROR());
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    printf("Successfully loaded library: %s\n", library_name);

    // Create library structure
    tauraro_ffi_library_t* lib = (tauraro_ffi_library_t*)malloc(sizeof(tauraro_ffi_library_t));
    lib->name = strdup(library_name);
    lib->handle = handle;

    // Add to manager
    if (manager->library_count >= manager->library_capacity) {
        manager->library_capacity *= 2;
        manager->libraries = (tauraro_ffi_library_t**)realloc(
            manager->libraries,
            sizeof(tauraro_ffi_library_t*) * manager->library_capacity
        );
    }

    manager->libraries[manager->library_count++] = lib;

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}

// Parse FFI type from string
static ffi_type_enum tauraro_ffi_parse_type(const char* type_str) {
    if (strcmp(type_str, "void") == 0) return FFI_TYPE_VOID;
    if (strcmp(type_str, "int") == 0) return FFI_TYPE_INT;
    if (strcmp(type_str, "int32") == 0) return FFI_TYPE_INT32;
    if (strcmp(type_str, "int64") == 0) return FFI_TYPE_INT64;
    if (strcmp(type_str, "uint") == 0) return FFI_TYPE_UINT;
    if (strcmp(type_str, "uint32") == 0) return FFI_TYPE_UINT32;
    if (strcmp(type_str, "uint64") == 0) return FFI_TYPE_UINT64;
    if (strcmp(type_str, "float") == 0) return FFI_TYPE_FLOAT;
    if (strcmp(type_str, "double") == 0) return FFI_TYPE_DOUBLE;
    if (strcmp(type_str, "char") == 0) return FFI_TYPE_CHAR;
    if (strcmp(type_str, "string") == 0) return FFI_TYPE_STRING;
    if (strcmp(type_str, "pointer") == 0) return FFI_TYPE_POINTER;
    if (strcmp(type_str, "bool") == 0) return FFI_TYPE_BOOL;

    // Default to int
    return FFI_TYPE_INT;
}

// Define a foreign function
static tauraro_value_t* tauraro_ffi_define_function(
    const char* library_name,
    const char* function_name,
    const char* return_type_str,
    const char** param_type_strs,
    int param_count
) {
    tauraro_ffi_manager_t* manager = tauraro_ffi_init();

    // Find library
    tauraro_ffi_library_t* lib = tauraro_ffi_find_library(library_name);
    if (lib == NULL) {
        fprintf(stderr, "Library not loaded: %s\n", library_name);
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    // Get function pointer
    void* func_ptr = GET_FUNCTION(lib->handle, function_name);
    if (func_ptr == NULL) {
        fprintf(stderr, "Function not found: %s in library %s\n", function_name, library_name);
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    printf("Successfully loaded function: %s from %s\n", function_name, library_name);

    // Create function structure
    tauraro_ffi_function_t* func = (tauraro_ffi_function_t*)malloc(sizeof(tauraro_ffi_function_t));
    func->name = strdup(function_name);
    func->func_ptr = func_ptr;
    func->return_type = tauraro_ffi_parse_type(return_type_str);
    func->param_count = param_count;

    if (param_count > 0) {
        func->param_types = (ffi_type_enum*)malloc(sizeof(ffi_type_enum) * param_count);
        for (int i = 0; i < param_count; i++) {
            func->param_types[i] = tauraro_ffi_parse_type(param_type_strs[i]);
        }
    } else {
        func->param_types = NULL;
    }

    // Add to manager
    if (manager->function_count >= manager->function_capacity) {
        manager->function_capacity *= 2;
        manager->functions = (tauraro_ffi_function_t**)realloc(
            manager->functions,
            sizeof(tauraro_ffi_function_t*) * manager->function_capacity
        );
    }

    manager->functions[manager->function_count++] = func;

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}

// Find function by name
static tauraro_ffi_function_t* tauraro_ffi_find_function(const char* name) {
    tauraro_ffi_manager_t* manager = tauraro_ffi_init();

    for (int i = 0; i < manager->function_count; i++) {
        if (strcmp(manager->functions[i]->name, name) == 0) {
            return manager->functions[i];
        }
    }
    return NULL;
}

// Call a foreign function (simplified version - supports up to 10 args)
static tauraro_value_t* tauraro_ffi_call_function(
    const char* function_name,
    tauraro_value_t** args,
    int arg_count
) {
    tauraro_ffi_function_t* func = tauraro_ffi_find_function(function_name);
    if (func == NULL) {
        fprintf(stderr, "Function not defined: %s\n", function_name);
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    // Convert arguments based on types (simplified)
    long long arg_values[10] = {0};
    for (int i = 0; i < arg_count && i < 10; i++) {
        if (func->param_types[i] == FFI_TYPE_INT ||
            func->param_types[i] == FFI_TYPE_INT32) {
            arg_values[i] = (long long)args[i]->data.int_val;
        } else if (func->param_types[i] == FFI_TYPE_DOUBLE) {
            *((double*)&arg_values[i]) = args[i]->data.float_val;
        } else if (func->param_types[i] == FFI_TYPE_STRING) {
            arg_values[i] = (long long)args[i]->data.str_val;
        }
    }

    // Call the function (simplified calling convention)
    tauraro_value_t* result = tauraro_value_new();

    typedef long long (*func_0_t)();
    typedef long long (*func_1_t)(long long);
    typedef long long (*func_2_t)(long long, long long);
    typedef long long (*func_3_t)(long long, long long, long long);
    typedef double (*func_double_1_t)(double);
    typedef double (*func_double_2_t)(double, double);

    long long ret_int = 0;
    double ret_double = 0.0;

    switch (arg_count) {
        case 0:
            ret_int = ((func_0_t)func->func_ptr)();
            break;
        case 1:
            if (func->return_type == FFI_TYPE_DOUBLE) {
                ret_double = ((func_double_1_t)func->func_ptr)(*((double*)&arg_values[0]));
            } else {
                ret_int = ((func_1_t)func->func_ptr)(arg_values[0]);
            }
            break;
        case 2:
            if (func->return_type == FFI_TYPE_DOUBLE) {
                ret_double = ((func_double_2_t)func->func_ptr)(
                    *((double*)&arg_values[0]),
                    *((double*)&arg_values[1])
                );
            } else {
                ret_int = ((func_2_t)func->func_ptr)(arg_values[0], arg_values[1]);
            }
            break;
        case 3:
            ret_int = ((func_3_t)func->func_ptr)(arg_values[0], arg_values[1], arg_values[2]);
            break;
        default:
            fprintf(stderr, "Unsupported argument count: %d\n", arg_count);
            result->type = TAURARO_NONE;
            return result;
    }

    // Set result based on return type
    if (func->return_type == FFI_TYPE_VOID) {
        result->type = TAURARO_NONE;
    } else if (func->return_type == FFI_TYPE_DOUBLE || func->return_type == FFI_TYPE_FLOAT) {
        result->type = TAURARO_FLOAT;
        result->data.float_val = ret_double;
    } else {
        result->type = TAURARO_INT;
        result->data.int_val = (int)ret_int;
    }

    return result;
}

// Cleanup FFI manager
static void tauraro_ffi_cleanup() {
    if (global_ffi_manager == NULL) return;

    tauraro_ffi_manager_t* manager = global_ffi_manager;

    // Close all libraries
    for (int i = 0; i < manager->library_count; i++) {
        CLOSE_LIBRARY(manager->libraries[i]->handle);
        free(manager->libraries[i]->name);
        free(manager->libraries[i]);
    }
    free(manager->libraries);

    // Free all functions
    for (int i = 0; i < manager->function_count; i++) {
        free(manager->functions[i]->name);
        if (manager->functions[i]->param_types) {
            free(manager->functions[i]->param_types);
        }
        free(manager->functions[i]);
    }
    free(manager->functions);

    free(manager);
    global_ffi_manager = NULL;
}

#endif // TAURARO_FFI_H
