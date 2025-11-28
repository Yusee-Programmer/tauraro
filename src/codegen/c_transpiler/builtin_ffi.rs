//! Builtin FFI Module Support for C Backend
//!
//! This module handles:
//! - Generating FFI type declarations for builtin modules
//! - Generating wrapper functions that bridge TauValue <-> tauraro_value_t
//! - Supporting all builtin modules with proper function signatures

use std::collections::HashSet;

/// Generate FFI type declarations that are shared across all modules
pub fn generate_ffi_types() -> String {
    let mut output = String::new();

    output.push_str("// FFI types (shared across all modules)\n");
    output.push_str("typedef enum {\n");
    output.push_str("    TAURARO_INT = 0,\n");
    output.push_str("    TAURARO_FLOAT = 1,\n");
    output.push_str("    TAURARO_BOOL = 2,\n");
    output.push_str("    TAURARO_STRING = 3,\n");
    output.push_str("    TAURARO_LIST = 4,\n");
    output.push_str("    TAURARO_DICT = 5,\n");
    output.push_str("    TAURARO_TUPLE = 6,\n");
    output.push_str("    TAURARO_SET = 7,\n");
    output.push_str("    TAURARO_NONE = 8,\n");
    output.push_str("    TAURARO_OBJECT = 9,\n");
    output.push_str("    TAURARO_FUNCTION = 10,\n");
    output.push_str("    TAURARO_BYTES = 11,\n");
    output.push_str("    TAURARO_COMPLEX = 12,\n");
    output.push_str("    TAURARO_RANGE = 13,\n");
    output.push_str("    TAURARO_FROZENSET = 14,\n");
    output.push_str("} tauraro_type_t;\n\n");

    output.push_str("typedef union {\n");
    output.push_str("    int64_t int_val;\n");
    output.push_str("    double float_val;\n");
    output.push_str("    int bool_val;\n");
    output.push_str("    char* str_val;\n");
    output.push_str("    void* ptr_val;\n");
    output.push_str("} tauraro_data_t;\n\n");

    output.push_str("typedef struct tauraro_value {\n");
    output.push_str("    tauraro_type_t type;\n");
    output.push_str("    int ref_count;\n");
    output.push_str("    tauraro_data_t data;\n");
    output.push_str("} tauraro_value_t;\n\n");

    output
}

/// Generate FFI function declarations for imported modules
pub fn generate_ffi_declarations(builtin_modules: &[String]) -> String {
    let mut output = String::new();

    output.push_str("// FFI function declarations\n");
    for module_name in builtin_modules {
        match module_name.as_str() {
            "math" => {
                output.push_str("// Math module\n");
                output.push_str("extern double tauraro_math_pi;\n");
                output.push_str("extern double tauraro_math_e;\n");
                output.push_str("tauraro_value_t* tauraro_math_sqrt(int argc, tauraro_value_t** argv);\n");
                output.push_str("tauraro_value_t* tauraro_math_pow(int argc, tauraro_value_t** argv);\n");
                output.push_str("tauraro_value_t* tauraro_math_sin(int argc, tauraro_value_t** argv);\n");
                output.push_str("tauraro_value_t* tauraro_math_cos(int argc, tauraro_value_t** argv);\n");
            }
            "os" => {
                output.push_str("// OS module\n");
                output.push_str("tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv);\n");
            }
            "json" => {
                output.push_str("// JSON module\n");
                output.push_str("tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv);\n");
                output.push_str("tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv);\n");
            }
            "sys" => {
                output.push_str("// Sys module\n");
                output.push_str("tauraro_value_t* tauraro_sys_version(int argc, tauraro_value_t** argv);\n");
                output.push_str("tauraro_value_t* tauraro_sys_platform(int argc, tauraro_value_t** argv);\n");
            }
            _ => {
                // For other modules, add a comment
                output.push_str(&format!("// Module: {} (declarations not yet implemented)\n", module_name));
            }
        }
    }
    output.push_str("\n");

    output
}

/// Generate forward declarations for FFI wrapper functions
pub fn generate_wrapper_declarations(builtin_modules: &[String]) -> String {
    let mut output = String::new();

    output.push_str("// Forward declarations for FFI wrapper functions\n");
    for module_name in builtin_modules {
        output.push_str(&generate_module_wrapper_declarations(module_name));
    }
    output.push_str("\n");

    output
}

/// Generate forward declarations for a specific module's wrapper functions
fn generate_module_wrapper_declarations(module_name: &str) -> String {
    let mut output = String::new();

    match module_name {
        "math" => {
            output.push_str("// Math module wrappers\n");
            output.push_str("TauValue math__pi(TauValue self);\n");
            output.push_str("TauValue math__e(TauValue self);\n");
            output.push_str("TauValue math__sqrt(TauValue self, TauValue arg);\n");
            output.push_str("TauValue math__pow(TauValue self, TauValue arg1, TauValue arg2);\n");
            output.push_str("TauValue math__sin(TauValue self, TauValue arg);\n");
            output.push_str("TauValue math__cos(TauValue self, TauValue arg);\n");
        }
        "os" => {
            output.push_str("// OS module wrappers\n");
            output.push_str("TauValue os_getcwd(void);\n");
        }
        "json" => {
            output.push_str("// JSON module wrappers\n");
            output.push_str("TauValue json_dumps(TauValue obj);\n");
            output.push_str("TauValue json_loads(TauValue json_str);\n");
        }
        "sys" => {
            output.push_str("// Sys module wrappers\n");
            output.push_str("TauValue sys__version(TauValue self);\n");
            output.push_str("TauValue sys__platform(TauValue self);\n");
        }
        _ => {}
    }

    output
}

/// Generate FFI wrapper functions for all imported modules
pub fn generate_ffi_wrappers(builtin_modules: &[String]) -> String {
    let mut output = String::new();

    // Add tauraro_value_new implementation (required by FFI modules)
    output.push_str("// Implementation of tauraro_value_new (required by FFI modules)\n");
    output.push_str("tauraro_value_t* tauraro_value_new(void) {\n");
    output.push_str("    tauraro_value_t* val = (tauraro_value_t*)malloc(sizeof(tauraro_value_t));\n");
    output.push_str("    val->type = TAURARO_NONE;\n");
    output.push_str("    val->ref_count = 1;\n");
    output.push_str("    val->data.ptr_val = NULL;\n");
    output.push_str("    return val;\n");
    output.push_str("}\n\n");

    // Module constant handler (wraps tauraro_get_attribute)
    output.push_str("// Forward declaration of original tauraro_get_attribute\n");
    output.push_str("TauValue tauraro_get_attribute_original(TauObject* obj, const char* name);\n\n");

    output.push_str("// Module-aware attribute getter (handles builtin module constants)\n");
    output.push_str("TauValue tauraro_get_attribute_wrapper(TauObject* obj, const char* name) {\n");
    output.push_str("    // Check for math module constants\n");
    output.push_str("    if (strcmp(name, \"pi\") == 0) {\n");
    output.push_str("        return tauraro_float(tauraro_math_pi);\n");
    output.push_str("    }\n");
    output.push_str("    if (strcmp(name, \"e\") == 0) {\n");
    output.push_str("        return tauraro_float(tauraro_math_e);\n");
    output.push_str("    }\n");
    output.push_str("    // For sys module constants, call the FFI functions\n");
    output.push_str("    if (strcmp(name, \"version\") == 0) {\n");
    output.push_str("        tauraro_value_t* result = tauraro_sys_version(0, NULL);\n");
    output.push_str("        return tauraro_to_tau(result);\n");
    output.push_str("    }\n");
    output.push_str("    if (strcmp(name, \"platform\") == 0) {\n");
    output.push_str("        tauraro_value_t* result = tauraro_sys_platform(0, NULL);\n");
    output.push_str("        return tauraro_to_tau(result);\n");
    output.push_str("    }\n");
    output.push_str("    // Fall back to original function for regular object attributes\n");
    output.push_str("    return tauraro_get_attribute_original(obj, name);\n");
    output.push_str("}\n\n");

    output.push_str("// Rename original function and replace with wrapper\n");
    output.push_str("#define tauraro_get_attribute_original tauraro_get_attribute\n");
    output.push_str("#define tauraro_get_attribute tauraro_get_attribute_wrapper\n\n");

    // Helper function to convert TauValue to tauraro_value_t
    output.push_str("// Helper: Convert TauValue to tauraro_value_t\n");
    output.push_str("tauraro_value_t* tau_to_tauraro(TauValue val) {\n");
    output.push_str("    tauraro_value_t* result = (tauraro_value_t*)malloc(sizeof(tauraro_value_t));\n");
    output.push_str("    result->ref_count = 1;\n");
    output.push_str("    switch(val.type) {\n");
    output.push_str("        case 0: // int\n");
    output.push_str("            result->type = TAURARO_INT;\n");
    output.push_str("            result->data.int_val = val.value.i;\n");
    output.push_str("            break;\n");
    output.push_str("        case 1: // float\n");
    output.push_str("            result->type = TAURARO_FLOAT;\n");
    output.push_str("            result->data.float_val = val.value.f;\n");
    output.push_str("            break;\n");
    output.push_str("        case 2: // string\n");
    output.push_str("            result->type = TAURARO_STRING;\n");
    output.push_str("            result->data.str_val = val.value.s ? strdup(val.value.s) : NULL;\n");
    output.push_str("            break;\n");
    output.push_str("        case 3: // bool\n");
    output.push_str("            result->type = TAURARO_BOOL;\n");
    output.push_str("            result->data.bool_val = val.value.i != 0;\n");
    output.push_str("            break;\n");
    output.push_str("        case 5: // dict\n");
    output.push_str("            result->type = TAURARO_DICT;\n");
    output.push_str("            result->data.ptr_val = val.value.dict;\n");
    output.push_str("            break;\n");
    output.push_str("        default:\n");
    output.push_str("            result->type = TAURARO_NONE;\n");
    output.push_str("            result->data.ptr_val = NULL;\n");
    output.push_str("    }\n");
    output.push_str("    return result;\n");
    output.push_str("}\n\n");

    // Helper function to convert tauraro_value_t to TauValue
    output.push_str("// Helper: Convert tauraro_value_t to TauValue\n");
    output.push_str("TauValue tauraro_to_tau(tauraro_value_t* val) {\n");
    output.push_str("    if (!val) return tauraro_none();\n");
    output.push_str("    TauValue result = {.refcount = 1, .next = NULL};\n");
    output.push_str("    switch(val->type) {\n");
    output.push_str("        case TAURARO_INT:\n");
    output.push_str("            result.type = 0;\n");
    output.push_str("            result.value.i = val->data.int_val;\n");
    output.push_str("            break;\n");
    output.push_str("        case TAURARO_FLOAT:\n");
    output.push_str("            result.type = 1;\n");
    output.push_str("            result.value.f = val->data.float_val;\n");
    output.push_str("            break;\n");
    output.push_str("        case TAURARO_STRING:\n");
    output.push_str("            result.type = 2;\n");
    output.push_str("            result.value.s = val->data.str_val ? strdup(val->data.str_val) : NULL;\n");
    output.push_str("            break;\n");
    output.push_str("        case TAURARO_BOOL:\n");
    output.push_str("            result.type = 3;\n");
    output.push_str("            result.value.i = val->data.bool_val ? 1 : 0;\n");
    output.push_str("            break;\n");
    output.push_str("        case TAURARO_DICT:\n");
    output.push_str("            result.type = 5;\n");
    output.push_str("            result.value.dict = (TauDict*)val->data.ptr_val;\n");
    output.push_str("            break;\n");
    output.push_str("        default:\n");
    output.push_str("            result.type = -1;\n");
    output.push_str("            result.value.ptr = NULL;\n");
    output.push_str("    }\n");
    output.push_str("    // Free the tauraro_value_t after conversion\n");
    output.push_str("    free(val);\n");
    output.push_str("    return result;\n");
    output.push_str("}\n\n");

    // Generate wrapper functions for each module
    for module_name in builtin_modules {
        output.push_str(&generate_module_wrappers(module_name));
    }

    output
}

/// Generate wrapper functions for a specific module
fn generate_module_wrappers(module_name: &str) -> String {
    let mut output = String::new();

    match module_name {
        "math" => {
            output.push_str("// Math module wrappers\n");

            // Module constants (attributes)
            output.push_str("TauValue math__pi(TauValue self) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    return tauraro_float(tauraro_math_pi);\n");
            output.push_str("}\n\n");

            output.push_str("TauValue math__e(TauValue self) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    return tauraro_float(tauraro_math_e);\n");
            output.push_str("}\n\n");

            // Method wrappers (double underscore for method call syntax)
            output.push_str("TauValue math__sqrt(TauValue self, TauValue arg) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* arg_arr[1];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg);\n");
            output.push_str("    tauraro_value_t* result = tauraro_math_sqrt(1, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            output.push_str("TauValue math__pow(TauValue self, TauValue arg1, TauValue arg2) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* arg_arr[2];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg1);\n");
            output.push_str("    arg_arr[1] = tau_to_tauraro(arg2);\n");
            output.push_str("    tauraro_value_t* result = tauraro_math_pow(2, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            output.push_str("TauValue math__sin(TauValue self, TauValue arg) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* arg_arr[1];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg);\n");
            output.push_str("    tauraro_value_t* result = tauraro_math_sin(1, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            output.push_str("TauValue math__cos(TauValue self, TauValue arg) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* arg_arr[1];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg);\n");
            output.push_str("    tauraro_value_t* result = tauraro_math_cos(1, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");
        }
        "os" => {
            output.push_str("// OS module wrappers\n");

            // Single underscore (direct function call)
            output.push_str("TauValue os_getcwd() {\n");
            output.push_str("    tauraro_value_t* result = tauraro_os_getcwd(0, NULL);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            // Double underscore (method call)
            output.push_str("TauValue os__getcwd(TauValue self) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* result = tauraro_os_getcwd(0, NULL);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");
        }
        "json" => {
            output.push_str("// JSON module wrappers\n");

            // Single underscore versions
            output.push_str("TauValue json_dumps(TauValue arg) {\n");
            output.push_str("    tauraro_value_t* arg_arr[1];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg);\n");
            output.push_str("    tauraro_value_t* result = tauraro_json_dumps(1, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            output.push_str("TauValue json_loads(TauValue arg) {\n");
            output.push_str("    tauraro_value_t* arg_arr[1];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg);\n");
            output.push_str("    tauraro_value_t* result = tauraro_json_loads(1, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            // Double underscore versions
            output.push_str("TauValue json__dumps(TauValue self, TauValue arg) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* arg_arr[1];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg);\n");
            output.push_str("    tauraro_value_t* result = tauraro_json_dumps(1, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            output.push_str("TauValue json__loads(TauValue self, TauValue arg) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* arg_arr[1];\n");
            output.push_str("    arg_arr[0] = tau_to_tauraro(arg);\n");
            output.push_str("    tauraro_value_t* result = tauraro_json_loads(1, arg_arr);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");
        }
        "sys" => {
            output.push_str("// Sys module wrappers\n");

            // Attribute accessors
            output.push_str("TauValue sys__version(TauValue self) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* result = tauraro_sys_version(0, NULL);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");

            output.push_str("TauValue sys__platform(TauValue self) {\n");
            output.push_str("    (void)self;\n");
            output.push_str("    tauraro_value_t* result = tauraro_sys_platform(0, NULL);\n");
            output.push_str("    return tauraro_to_tau(result);\n");
            output.push_str("}\n\n");
        }
        _ => {
            // For unsupported modules, generate a placeholder
            output.push_str(&format!("// Module '{}' wrappers not yet implemented\n\n", module_name));
        }
    }

    output
}
