//! Tauraro Type System for C Transpilation
//!
//! This module defines the C type structures that represent Tauraro's
//! Python-compatible data types at runtime.

/// Generate C type definitions for Tauraro runtime
pub fn generate_type_definitions() -> String {
    let mut code = String::new();

    code.push_str("// Tauraro runtime data structures\n");
    code.push_str("typedef enum {\n");
    code.push_str("    TAURARO_INT,\n");
    code.push_str("    TAURARO_FLOAT,\n");
    code.push_str("    TAURARO_BOOL,\n");
    code.push_str("    TAURARO_STRING,\n");
    code.push_str("    TAURARO_LIST,\n");
    code.push_str("    TAURARO_DICT,\n");
    code.push_str("    TAURARO_TUPLE,\n");
    code.push_str("    TAURARO_SET,\n");
    code.push_str("    TAURARO_NONE,\n");
    code.push_str("    TAURARO_OBJECT,\n");
    code.push_str("    TAURARO_FUNCTION,\n");
    code.push_str("    TAURARO_BYTES,\n");
    code.push_str("    TAURARO_COMPLEX,\n");
    code.push_str("    TAURARO_RANGE,\n");
    code.push_str("    TAURARO_FROZENSET\n");
    code.push_str("} tauraro_type_t;\n\n");

    // Main value structure
    code.push_str("typedef struct tauraro_value {\n");
    code.push_str("    tauraro_type_t type;\n");
    code.push_str("    int ref_count;\n");  // Reference counting for memory management
    code.push_str("    union {\n");
    code.push_str("        int64_t int_val;\n");
    code.push_str("        double float_val;\n");
    code.push_str("        bool bool_val;\n");
    code.push_str("        char* str_val;\n");
    code.push_str("        struct tauraro_list* list_val;\n");
    code.push_str("        struct tauraro_dict* dict_val;\n");
    code.push_str("        struct tauraro_tuple* tuple_val;\n");
    code.push_str("        struct tauraro_set* set_val;\n");
    code.push_str("        struct tauraro_object* obj_val;\n");
    code.push_str("        struct tauraro_function* func_val;\n");
    code.push_str("        struct tauraro_bytes* bytes_val;\n");
    code.push_str("        struct tauraro_complex* complex_val;\n");
    code.push_str("        struct tauraro_range* range_val;\n");
    code.push_str("        void* ptr_val;\n");
    code.push_str("    } data;\n");
    code.push_str("} tauraro_value_t;\n\n");

    // List structure
    code.push_str("typedef struct tauraro_list {\n");
    code.push_str("    tauraro_value_t** items;\n");
    code.push_str("    size_t size;\n");
    code.push_str("    size_t capacity;\n");
    code.push_str("} tauraro_list_t;\n\n");

    // Dictionary structure
    code.push_str("typedef struct tauraro_dict {\n");
    code.push_str("    char** keys;\n");
    code.push_str("    tauraro_value_t** values;\n");
    code.push_str("    size_t size;\n");
    code.push_str("    size_t capacity;\n");
    code.push_str("} tauraro_dict_t;\n\n");

    // Tuple structure
    code.push_str("typedef struct tauraro_tuple {\n");
    code.push_str("    tauraro_value_t** items;\n");
    code.push_str("    size_t size;\n");
    code.push_str("    size_t capacity;\n");
    code.push_str("} tauraro_tuple_t;\n\n");

    // Set structure
    code.push_str("typedef struct tauraro_set {\n");
    code.push_str("    tauraro_value_t** items;\n");
    code.push_str("    size_t size;\n");
    code.push_str("    size_t capacity;\n");
    code.push_str("} tauraro_set_t;\n\n");

    // Bytes structure
    code.push_str("typedef struct tauraro_bytes {\n");
    code.push_str("    unsigned char* data;\n");
    code.push_str("    size_t size;\n");
    code.push_str("} tauraro_bytes_t;\n\n");

    // Complex number structure
    code.push_str("typedef struct tauraro_complex {\n");
    code.push_str("    double real;\n");
    code.push_str("    double imag;\n");
    code.push_str("} tauraro_complex_t;\n\n");

    // Range structure
    code.push_str("typedef struct tauraro_range {\n");
    code.push_str("    int64_t start;\n");
    code.push_str("    int64_t stop;\n");
    code.push_str("    int64_t step;\n");
    code.push_str("} tauraro_range_t;\n\n");

    // Function structure
    code.push_str("typedef struct tauraro_function {\n");
    code.push_str("    char* name;\n");
    code.push_str("    void* func_ptr;\n");
    code.push_str("    tauraro_value_t** defaults;\n");
    code.push_str("    size_t num_defaults;\n");
    code.push_str("    struct tauraro_dict* closure;\n");
    code.push_str("} tauraro_function_t;\n\n");

    // Object structure (for OOP support)
    code.push_str("typedef struct tauraro_object {\n");
    code.push_str("    char* class_name;\n");
    code.push_str("    struct tauraro_dict* attributes;\n");
    code.push_str("    struct tauraro_dict* methods;\n");
    code.push_str("    struct tauraro_object* parent;\n");
    code.push_str("    void* native_ptr;\n");
    code.push_str("} tauraro_object_t;\n\n");

    // Frozenset structure
    code.push_str("typedef struct tauraro_frozenset {\n");
    code.push_str("    tauraro_value_t** items;\n");
    code.push_str("    size_t size;\n");
    code.push_str("    size_t capacity;\n");
    code.push_str("} tauraro_frozenset_t;\n\n");

    code
}

/// Generate forward declarations for type-related functions
pub fn generate_type_function_declarations() -> String {
    let mut code = String::new();

    code.push_str("// Type utility functions\n");
    code.push_str("tauraro_value_t* tauraro_value_new();\n");
    code.push_str("void tauraro_value_free(tauraro_value_t* value);\n");
    code.push_str("tauraro_value_t* tauraro_value_copy(tauraro_value_t* value);\n");
    code.push_str("void tauraro_incref(tauraro_value_t* value);\n");
    code.push_str("void tauraro_decref(tauraro_value_t* value);\n");
    code.push_str("\n");

    code
}

/// Generate implementation of type utility functions
pub fn generate_type_utility_functions() -> String {
    r#"// Type utility function implementations
tauraro_value_t* tauraro_value_new() {
    tauraro_value_t* value = malloc(sizeof(tauraro_value_t));
    value->ref_count = 1;
    value->type = TAURARO_NONE;
    return value;
}

void tauraro_value_free(tauraro_value_t* value) {
    if (!value) return;

    switch (value->type) {
        case TAURARO_STRING:
            if (value->data.str_val) free(value->data.str_val);
            break;
        case TAURARO_LIST:
            if (value->data.list_val) {
                for (size_t i = 0; i < value->data.list_val->size; i++) {
                    tauraro_decref(value->data.list_val->items[i]);
                }
                free(value->data.list_val->items);
                free(value->data.list_val);
            }
            break;
        case TAURARO_DICT:
            if (value->data.dict_val) {
                for (size_t i = 0; i < value->data.dict_val->size; i++) {
                    free(value->data.dict_val->keys[i]);
                    tauraro_decref(value->data.dict_val->values[i]);
                }
                free(value->data.dict_val->keys);
                free(value->data.dict_val->values);
                free(value->data.dict_val);
            }
            break;
        default:
            break;
    }

    free(value);
}

tauraro_value_t* tauraro_value_copy(tauraro_value_t* value) {
    if (!value) return NULL;

    tauraro_value_t* copy = malloc(sizeof(tauraro_value_t));
    copy->type = value->type;
    copy->ref_count = 1;

    switch (value->type) {
        case TAURARO_INT:
            copy->data.int_val = value->data.int_val;
            break;
        case TAURARO_FLOAT:
            copy->data.float_val = value->data.float_val;
            break;
        case TAURARO_BOOL:
            copy->data.bool_val = value->data.bool_val;
            break;
        case TAURARO_STRING:
            copy->data.str_val = strdup(value->data.str_val);
            break;
        default:
            // For complex types, we'll implement proper deep copy later
            copy->data = value->data;
            break;
    }

    return copy;
}

void tauraro_incref(tauraro_value_t* value) {
    if (value) {
        value->ref_count++;
    }
}

void tauraro_decref(tauraro_value_t* value) {
    if (value && --value->ref_count == 0) {
        tauraro_value_free(value);
    }
}

// Type checking functions
int tauraro_is_int(tauraro_value_t* value) {
    return value && value->type == TAURARO_INT;
}

int tauraro_is_float(tauraro_value_t* value) {
    return value && value->type == TAURARO_FLOAT;
}

int tauraro_is_string(tauraro_value_t* value) {
    return value && value->type == TAURARO_STRING;
}

int tauraro_is_bool(tauraro_value_t* value) {
    return value && value->type == TAURARO_BOOL;
}

int tauraro_is_none(tauraro_value_t* value) {
    return value == NULL || value->type == TAURARO_NONE;
}

int tauraro_is_list(tauraro_value_t* value) {
    return value && value->type == TAURARO_LIST;
}

int tauraro_is_dict(tauraro_value_t* value) {
    return value && value->type == TAURARO_DICT;
}

int tauraro_is_tuple(tauraro_value_t* value) {
    return value && value->type == TAURARO_TUPLE;
}

int tauraro_is_object(tauraro_value_t* value) {
    return value && value->type == TAURARO_OBJECT;
}

// Type conversion functions
char* tauraro_type_name(tauraro_value_t* value) {
    if (!value) return "NoneType";
    switch (value->type) {
        case TAURARO_INT: return "int";
        case TAURARO_FLOAT: return "float";
        case TAURARO_BOOL: return "bool";
        case TAURARO_STRING: return "str";
        case TAURARO_LIST: return "list";
        case TAURARO_DICT: return "dict";
        case TAURARO_TUPLE: return "tuple";
        case TAURARO_SET: return "set";
        case TAURARO_NONE: return "NoneType";
        case TAURARO_OBJECT: return "object";
        case TAURARO_FUNCTION: return "function";
        case TAURARO_BYTES: return "bytes";
        case TAURARO_COMPLEX: return "complex";
        case TAURARO_RANGE: return "range";
        case TAURARO_FROZENSET: return "frozenset";
        default: return "unknown";
    }
}

"#.to_string()
}
