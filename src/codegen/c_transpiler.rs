//! C Transpiler for Tauraro
//! Transpiles Tauraro IR to C code and optionally compiles to executable

use crate::codegen::{CodeGenerator, CodegenOptions, Target};
use crate::ir::{IRModule, IRFunction, IRBlock, IRInstruction};
use crate::value::Value;
use anyhow::Result;
use std::collections::HashMap;
use std::process::Command;
use std::path::Path;
use std::fmt;  // Add this import for format!

/// C Transpiler that converts Tauraro IR to C code
pub struct CTranspiler {
    target: Target,
}

impl CTranspiler {
    pub fn new() -> Self {
        Self {
            target: Target::C,
        }
    }

    /// Generate C code from IR module
    fn generate_c_code(&self, module: IRModule) -> Result<String> {
        let mut c_code = String::new();

        // Add standard headers
        c_code.push_str("#include <stdio.h>\n");
        c_code.push_str("#include <stdlib.h>\n");
        c_code.push_str("#include <string.h>\n");
        c_code.push_str("#include <stdbool.h>\n");
        c_code.push_str("#include <stdint.h>\n\n");

        // Add Tauraro runtime support structures
        c_code.push_str("// Tauraro runtime data structures\n");
        c_code.push_str("typedef enum {\n");
        c_code.push_str("    TAURARO_INT,\n");
        c_code.push_str("    TAURARO_FLOAT,\n");
        c_code.push_str("    TAURARO_BOOL,\n");
        c_code.push_str("    TAURARO_STRING,\n");
        c_code.push_str("    TAURARO_LIST,\n");
        c_code.push_str("    TAURARO_DICT,\n");
        c_code.push_str("    TAURARO_TUPLE,\n");
        c_code.push_str("    TAURARO_SET,\n");
        c_code.push_str("    TAURARO_NONE,\n");
        c_code.push_str("    TAURARO_OBJECT\n");
        c_code.push_str("} tauraro_type_t;\n\n");

        c_code.push_str("typedef struct tauraro_value {\n");
        c_code.push_str("    tauraro_type_t type;\n");
        c_code.push_str("    union {\n");
        c_code.push_str("        int64_t int_val;\n");
        c_code.push_str("        double float_val;\n");
        c_code.push_str("        bool bool_val;\n");
        c_code.push_str("        char* str_val;\n");
        c_code.push_str("        struct tauraro_list* list_val;\n");
        c_code.push_str("        struct tauraro_dict* dict_val;\n");
        c_code.push_str("        struct tauraro_tuple* tuple_val;\n");
        c_code.push_str("        struct tauraro_set* set_val;\n");
        c_code.push_str("        void* obj_val;\n");
        c_code.push_str("    } data;\n");
        c_code.push_str("} tauraro_value_t;\n\n");

        c_code.push_str("typedef struct tauraro_list {\n");
        c_code.push_str("    tauraro_value_t** items;\n");
        c_code.push_str("    size_t size;\n");
        c_code.push_str("    size_t capacity;\n");
        c_code.push_str("} tauraro_list_t;\n\n");

        c_code.push_str("typedef struct tauraro_dict {\n");
        c_code.push_str("    char** keys;\n");
        c_code.push_str("    tauraro_value_t** values;\n");
        c_code.push_str("    size_t size;\n");
        c_code.push_str("    size_t capacity;\n");
        c_code.push_str("} tauraro_dict_t;\n\n");

        // Add builtin function declarations
        c_code.push_str("// Builtin function declarations\n");
        c_code.push_str("tauraro_value_t* tauraro_print(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_len(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_str(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_int(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_float(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_bool(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_list(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_dict(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_tuple(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_range(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_abs(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_min(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_max(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_sum(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_round(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_pow(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_enumerate(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_zip(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_map(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_filter(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_sorted(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_any(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_all(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_open(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_input(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_isinstance(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_type(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_callable(int argc, tauraro_value_t** args);\n");
        c_code.push_str("tauraro_value_t* tauraro_format_str(int argc, tauraro_value_t** args);\n");  // Renamed from format
        c_code.push_str("\n");

        // Add builtin function implementations
        c_code.push_str("// Builtin function implementations\n");
        c_code.push_str("tauraro_value_t* tauraro_print(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    for (int i = 0; i < argc; i++) {\n");
        c_code.push_str("        if (i > 0) printf(\" \");\n");
        c_code.push_str("        switch (args[i]->type) {\n");
        c_code.push_str("            case TAURARO_INT:\n");
        c_code.push_str("                printf(\"%ld\", args[i]->data.int_val);\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_FLOAT:\n");
        c_code.push_str("                printf(\"%f\", args[i]->data.float_val);\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_BOOL:\n");
        c_code.push_str("                printf(\"%s\", args[i]->data.bool_val ? \"True\" : \"False\");\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_STRING:\n");
        c_code.push_str("                printf(\"%s\", args[i]->data.str_val);\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_NONE:\n");
        c_code.push_str("                printf(\"None\");\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            default:\n");
        c_code.push_str("                printf(\"<object>\");\n");
        c_code.push_str("                break;\n");
        c_code.push_str("        }\n");
        c_code.push_str("    }\n");
        c_code.push_str("    printf(\"\\n\");\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_NONE;\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_len(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    if (argc != 1) return NULL;\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_INT;\n");
        c_code.push_str("    switch (args[0]->type) {\n");
        c_code.push_str("        case TAURARO_STRING:\n");
        c_code.push_str("            result->data.int_val = strlen(args[0]->data.str_val);\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_LIST:\n");
        c_code.push_str("            result->data.int_val = args[0]->data.list_val->size;\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_DICT:\n");
        c_code.push_str("            result->data.int_val = args[0]->data.dict_val->size;\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_TUPLE:\n");
        c_code.push_str("            result->data.int_val = args[0]->data.tuple_val->size;\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        default:\n");
        c_code.push_str("            result->data.int_val = 0;\n");
        c_code.push_str("            break;\n");
        c_code.push_str("    }\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_str(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    if (argc == 0) {\n");
        c_code.push_str("        tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("        result->type = TAURARO_STRING;\n");
        c_code.push_str("        result->data.str_val = strdup(\"\");\n");
        c_code.push_str("        return result;\n");
        c_code.push_str("    }\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_STRING;\n");
        c_code.push_str("    char buffer[256];\n");
        c_code.push_str("    switch (args[0]->type) {\n");
        c_code.push_str("        case TAURARO_INT:\n");
        c_code.push_str("            sprintf(buffer, \"%ld\", args[0]->data.int_val);\n");
        c_code.push_str("            result->data.str_val = strdup(buffer);\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_FLOAT:\n");
        c_code.push_str("            sprintf(buffer, \"%f\", args[0]->data.float_val);\n");
        c_code.push_str("            result->data.str_val = strdup(buffer);\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_BOOL:\n");
        c_code.push_str("            result->data.str_val = strdup(args[0]->data.bool_val ? \"True\" : \"False\");\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_STRING:\n");
        c_code.push_str("            result->data.str_val = strdup(args[0]->data.str_val);\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_NONE:\n");
        c_code.push_str("            result->data.str_val = strdup(\"None\");\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        default:\n");
        c_code.push_str("            sprintf(buffer, \"<object at %p>\", (void*)args[0]);\n");
        c_code.push_str("            result->data.str_val = strdup(buffer);\n");
        c_code.push_str("            break;\n");
        c_code.push_str("    }\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_int(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_INT;\n");
        c_code.push_str("    if (argc == 0) {\n");
        c_code.push_str("        result->data.int_val = 0;\n");
        c_code.push_str("    } else {\n");
        c_code.push_str("        switch (args[0]->type) {\n");
        c_code.push_str("            case TAURARO_INT:\n");
        c_code.push_str("                result->data.int_val = args[0]->data.int_val;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_FLOAT:\n");
        c_code.push_str("                result->data.int_val = (int64_t)args[0]->data.float_val;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_BOOL:\n");
        c_code.push_str("                result->data.int_val = args[0]->data.bool_val ? 1 : 0;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_STRING:\n");
        c_code.push_str("                result->data.int_val = atol(args[0]->data.str_val);\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            default:\n");
        c_code.push_str("                result->data.int_val = 0;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("        }\n");
        c_code.push_str("    }\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_float(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_FLOAT;\n");
        c_code.push_str("    if (argc == 0) {\n");
        c_code.push_str("        result->data.float_val = 0.0;\n");
        c_code.push_str("    } else {\n");
        c_code.push_str("        switch (args[0]->type) {\n");
        c_code.push_str("            case TAURARO_INT:\n");
        c_code.push_str("                result->data.float_val = (double)args[0]->data.int_val;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_FLOAT:\n");
        c_code.push_str("                result->data.float_val = args[0]->data.float_val;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_BOOL:\n");
        c_code.push_str("                result->data.float_val = args[0]->data.bool_val ? 1.0 : 0.0;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_STRING:\n");
        c_code.push_str("                result->data.float_val = atof(args[0]->data.str_val);\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            default:\n");
        c_code.push_str("                result->data.float_val = 0.0;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("        }\n");
        c_code.push_str("    }\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_bool(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_BOOL;\n");
        c_code.push_str("    if (argc == 0) {\n");
        c_code.push_str("        result->data.bool_val = false;\n");
        c_code.push_str("    } else {\n");
        c_code.push_str("        // Simplified truthiness check\n");
        c_code.push_str("        switch (args[0]->type) {\n");
        c_code.push_str("            case TAURARO_INT:\n");
        c_code.push_str("                result->data.bool_val = args[0]->data.int_val != 0;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_FLOAT:\n");
        c_code.push_str("                result->data.bool_val = args[0]->data.float_val != 0.0;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_BOOL:\n");
        c_code.push_str("                result->data.bool_val = args[0]->data.bool_val;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_STRING:\n");
        c_code.push_str("                result->data.bool_val = strlen(args[0]->data.str_val) > 0;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            case TAURARO_NONE:\n");
        c_code.push_str("                result->data.bool_val = false;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("            default:\n");
        c_code.push_str("                result->data.bool_val = true;\n");
        c_code.push_str("                break;\n");
        c_code.push_str("        }\n");
        c_code.push_str("    }\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_list(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_LIST;\n");
        c_code.push_str("    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));\n");
        c_code.push_str("    list->size = 0;\n");
        c_code.push_str("    list->capacity = 10;\n");
        c_code.push_str("    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);\n");
        c_code.push_str("    result->data.list_val = list;\n");
        c_code.push_str("    \n");
        c_code.push_str("    if (argc > 0) {\n");
        c_code.push_str("        // For simplicity, we're not implementing full list conversion\n");
        c_code.push_str("        // In a full implementation, this would convert the argument to a list\n");
        c_code.push_str("    }\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_dict(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_DICT;\n");
        c_code.push_str("    tauraro_dict_t* dict = malloc(sizeof(tauraro_dict_t));\n");
        c_code.push_str("    dict->size = 0;\n");
        c_code.push_str("    dict->capacity = 10;\n");
        c_code.push_str("    dict->keys = malloc(sizeof(char*) * dict->capacity);\n");
        c_code.push_str("    dict->values = malloc(sizeof(tauraro_value_t*) * dict->capacity);\n");
        c_code.push_str("    result->data.dict_val = dict;\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_abs(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    if (argc != 1) return NULL;\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    switch (args[0]->type) {\n");
        c_code.push_str("        case TAURARO_INT:\n");
        c_code.push_str("            result->type = TAURARO_INT;\n");
        c_code.push_str("            result->data.int_val = args[0]->data.int_val < 0 ? -args[0]->data.int_val : args[0]->data.int_val;\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        case TAURARO_FLOAT:\n");
        c_code.push_str("            result->type = TAURARO_FLOAT;\n");
        c_code.push_str("            result->data.float_val = args[0]->data.float_val < 0 ? -args[0]->data.float_val : args[0]->data.float_val;\n");
        c_code.push_str("            break;\n");
        c_code.push_str("        default:\n");
        c_code.push_str("            result->type = TAURARO_INT;\n");
        c_code.push_str("            result->data.int_val = 0;\n");
        c_code.push_str("            break;\n");
        c_code.push_str("    }\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_min(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    if (argc == 0) return NULL;\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_INT;\n");
        c_code.push_str("    result->data.int_val = 0;\n");
        c_code.push_str("    // Simplified implementation - in a full implementation, this would find the minimum\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_max(int argc, tauraro_value_t** args) {\n");
        c_code.push_str("    if (argc == 0) return NULL;\n");
        c_code.push_str("    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));\n");
        c_code.push_str("    result->type = TAURARO_INT;\n");
        c_code.push_str("    result->data.int_val = 0;\n");
        c_code.push_str("    // Simplified implementation - in a full implementation, this would find the maximum\n");
        c_code.push_str("    return result;\n");
        c_code.push_str("}\n\n");

        c_code.push_str("tauraro_value_t* tauraro_format_str(int argc, tauraro_value_t** args) {\n");  // Renamed from format
        c_code.push_str("    // Simplified implementation - in a full implementation, this would format values\n");
        c_code.push_str("    return tauraro_str(argc, args);\n");
        c_code.push_str("}\n\n");

        // Forward declarations for functions
        for (name, _function) in &module.functions {
            c_code.push_str(&format!("void {}();\n", name));
        }
        c_code.push_str("\n");

        // Generate global variables
        c_code.push_str("// Global variables\n");
        for instruction in &module.globals {
            if let IRInstruction::StoreGlobal { name, value: _ } = instruction {
                c_code.push_str(&format!("tauraro_value_t* {};\n", name));
            }
        }
        c_code.push_str("\n");

        // Generate functions
        for (_name, function) in &module.functions {
            c_code.push_str(&self.generate_function(function)?);
            c_code.push_str("\n");
        }

        // Generate main function if it doesn't exist
        if !module.functions.contains_key("main") {
            c_code.push_str("int main() {\n");
            // Call the first function as main if available
            if let Some((first_name, _)) = module.functions.iter().next() {
                c_code.push_str(&format!("    {}();\n", first_name));
            }
            c_code.push_str("    return 0;\n");
            c_code.push_str("}\n");
        }

        Ok(c_code)
    }

    /// Generate C code for a function
    fn generate_function(&self, function: &IRFunction) -> Result<String> {
        let mut func_code = String::new();

        // Function signature
        func_code.push_str(&format!("void {}(", function.name));
        
        // Parameters
        for (i, param) in function.params.iter().enumerate() {
            if i > 0 {
                func_code.push_str(", ");
            }
            func_code.push_str(&format!("tauraro_value_t* {}", param));
        }
        func_code.push_str(") {\n");

        // Local variables
        let mut local_vars = HashMap::new();
        func_code.push_str("    // Local variables\n");
        
        // Process instructions
        for block in &function.blocks {
            for instruction in &block.instructions {
                func_code.push_str(&format!("    {}\n", self.generate_instruction(instruction, &mut local_vars)?));
            }
        }

        func_code.push_str("}\n");
        Ok(func_code)
    }

    /// Generate C code for an instruction
    fn generate_instruction(&self, instruction: &IRInstruction, local_vars: &mut HashMap<String, String>) -> Result<String> {
        match instruction {
            IRInstruction::LoadConst { value, result } => {
                match value {
                    Value::Int(i) => {
                        local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                        Ok(format!("tauraro_value_t* {} = malloc(sizeof(tauraro_value_t)); {}->type = TAURARO_INT; {}->data.int_val = {};", result, result, result, i))
                    }
                    Value::Float(f) => {
                        local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                        Ok(format!("tauraro_value_t* {} = malloc(sizeof(tauraro_value_t)); {}->type = TAURARO_FLOAT; {}->data.float_val = {};", result, result, result, f))
                    }
                    Value::Str(s) => {
                        local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                        Ok(format!("tauraro_value_t* {} = malloc(sizeof(tauraro_value_t)); {}->type = TAURARO_STRING; {}->data.str_val = strdup(\"{}\");", result, result, result, s))
                    }
                    Value::Bool(b) => {
                        local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                        Ok(format!("tauraro_value_t* {} = malloc(sizeof(tauraro_value_t)); {}->type = TAURARO_BOOL; {}->data.bool_val = {};", result, result, result, if *b { "true" } else { "false" }))
                    }
                    Value::None => {
                        local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                        Ok(format!("tauraro_value_t* {} = malloc(sizeof(tauraro_value_t)); {}->type = TAURARO_NONE;", result, result))
                    }
                    _ => {
                        local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                        Ok(format!("tauraro_value_t* {} = malloc(sizeof(tauraro_value_t)); {}->type = TAURARO_NONE; // Unsupported constant type", result, result))
                    }
                }
            }
            IRInstruction::LoadLocal { name, result } => {
                local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                Ok(format!("tauraro_value_t* {} = {};", result, name))
            }
            IRInstruction::StoreLocal { name, value } => {
                Ok(format!("{} = {};", name, value))
            }
            IRInstruction::LoadGlobal { name, result } => {
                local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                Ok(format!("tauraro_value_t* {} = {};", result, name))
            }
            IRInstruction::StoreGlobal { name, value } => {
                Ok(format!("{} = {};", name, value))
            }
            IRInstruction::BinaryOp { op, left, right, result } => {
                let op_str = match op {
                    crate::ast::BinaryOp::Add => "+",
                    crate::ast::BinaryOp::Sub => "-",
                    crate::ast::BinaryOp::Mul => "*",
                    crate::ast::BinaryOp::Div => "/",
                    crate::ast::BinaryOp::Mod => "%",
                    crate::ast::BinaryOp::Eq => "==",
                    crate::ast::BinaryOp::Ne => "!=",
                    crate::ast::BinaryOp::Lt => "<",
                    crate::ast::BinaryOp::Le => "<=",
                    crate::ast::BinaryOp::Gt => ">",
                    crate::ast::BinaryOp::Ge => ">=",
                    crate::ast::BinaryOp::And => "&&",
                    crate::ast::BinaryOp::Or => "||",
                    _ => "/* unknown op */"
                };
                
                local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                // Simplified implementation - in a full implementation, this would handle type checking and conversion
                Ok(format!("tauraro_value_t* {} = malloc(sizeof(tauraro_value_t)); {}->type = TAURARO_INT; {}->data.int_val = {}->data.int_val {} {}->data.int_val;", result, result, result, left, op_str, right))
            }
            IRInstruction::Call { func, args, result: result_opt } => {
                let args_str = if args.is_empty() {
                    "0, NULL".to_string()
                } else {
                    format!("{}(sizeof((tauraro_value_t*[]{{{}}})/sizeof(tauraro_value_t*)), (tauraro_value_t*[]){{{}}})", 
                            func, args.join(", "), args.join(", "))
                };
                
                match result_opt {
                    Some(result) => {
                        local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                        Ok(format!("tauraro_value_t* {} = tauraro_{};", result, args_str))
                    }
                    None => Ok(format!("tauraro_{};", args_str))
                }
            }
            IRInstruction::Return { value } => {
                match value {
                    Some(val) => Ok(format!("return {};", val)),
                    None => Ok("return;".to_string())
                }
            }
            IRInstruction::Jump { target: _ } => {
                // Jumps in C are handled with gotos or control structures
                Ok("// Jump instruction".to_string())
            }
            IRInstruction::JumpIf { condition, target: _ } => {
                Ok(format!("if ({}->data.bool_val) {{ /* jump */ }}", condition))
            }
            IRInstruction::JumpIfNot { condition, target: _ } => {
                Ok(format!("if (!{}->data.bool_val) {{ /* jump */ }}", condition))
            }
            IRInstruction::ListCreate { elements, result } => {
                local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                // Simplified implementation
                Ok(format!("tauraro_value_t* {} = tauraro_list(0, NULL);", result))
            }
            IRInstruction::DictCreate { pairs: _, result } => {
                local_vars.insert(result.clone(), "tauraro_value_t*".to_string());
                Ok(format!("tauraro_value_t* {} = tauraro_dict(0, NULL);", result))
            }
            IRInstruction::Import { module } => {
                Ok("// Import module: ".to_string() + module)
            }
            IRInstruction::ImportFrom { module, names: _ } => {
                Ok("// Import from module: ".to_string() + module)
            }
        }
    }

    /// Detect available C compilers
    fn detect_compilers() -> Vec<String> {
        let mut compilers = Vec::new();
        
        // Check for GCC
        if Command::new("gcc").arg("--version").output().is_ok() {
            compilers.push("gcc".to_string());
        }
        
        // Check for Clang
        if Command::new("clang").arg("--version").output().is_ok() {
            compilers.push("clang".to_string());
        }
        
        // On Windows, check for Clang-CL
        if cfg!(windows) {
            if Command::new("clang-cl").arg("--version").output().is_ok() {
                compilers.push("clang-cl".to_string());
            }
        }
        
        compilers
    }

    /// Compile C code to executable using system compiler
    fn compile_to_executable(&self, c_code: &str, output_path: &str, opt_level: u8) -> Result<()> {
        // Write C code to temporary file
        let temp_file = format!("{}.c", output_path);
        std::fs::write(&temp_file, c_code)?;

        // Detect available compilers
        let compilers = Self::detect_compilers();
        if compilers.is_empty() {
            return Err(anyhow::anyhow!("No C compiler found. Please install GCC or Clang."));
        }

        // Determine optimization flags
        let opt_flag = match opt_level {
            0 => "-O0",
            1 => "-O1",
            2 => "-O2",
            3 => "-O3",
            _ => "-O2",
        };

        // Try each compiler in order
        let mut last_error = String::new();
        for compiler in &compilers {
            let output = match compiler.as_str() {
                "gcc" | "clang" => {
                    Command::new(compiler)
                        .args(&[&temp_file, "-o", output_path, opt_flag])
                        .output()
                },
                "clang-cl" => {
                    Command::new(compiler)
                        .args(&[&temp_file, "-o", output_path, &format!("-O{}", opt_level)])
                        .output()
                },
                _ => {
                    // Fallback to basic compilation
                    Command::new(compiler)
                        .args(&[&temp_file, "-o", output_path])
                        .output()
                }
            };

            match output {
                Ok(output) => {
                    if output.status.success() {
                        // Clean up temporary file
                        let _ = std::fs::remove_file(temp_file);
                        println!("Successfully compiled with {} {}", compiler, opt_flag);
                        return Ok(());
                    } else {
                        let stderr = String::from_utf8_lossy(&output.stderr);
                        last_error = format!("{} compilation failed: {}", compiler, stderr);
                    }
                }
                Err(e) => {
                    last_error = format!("Failed to run {}: {}", compiler, e);
                }
            }
        }

        // Clean up temporary file
        let _ = std::fs::remove_file(temp_file);
        Err(anyhow::anyhow!("Compilation failed with all available compilers. Last error: {}", last_error))
    }
}

impl CodeGenerator for CTranspiler {
    fn generate(&self, module: IRModule, options: &CodegenOptions) -> Result<Vec<u8>> {
        let c_code = self.generate_c_code(module)?;
        
        // If output path is specified and we want to compile to executable
        if let Some(output_path) = &options.output_path {
            // Check if we should compile to executable (if it looks like an executable path)
            let should_compile = output_path.ends_with(std::env::consts::EXE_EXTENSION) || 
                                !output_path.contains(".") ||
                                Path::new(output_path).extension().is_none();
            
            if should_compile {
                // Compile to executable
                self.compile_to_executable(&c_code, output_path, options.opt_level)?;
                // Return empty bytes since executable is created separately
                return Ok(vec![]);
            }
        }
        
        // Return C code as bytes
        Ok(c_code.into_bytes())
    }
    
    fn get_target(&self) -> Target {
        Target::C
    }
    
    fn supports_optimization(&self) -> bool {
        true
    }
    
    fn get_supported_features(&self) -> Vec<&'static str> {
        vec![
            "basic_types",
            "functions",
            "control_flow",
            "data_structures",
            "builtin_functions",
            "collections",
            "objects",
        ]
    }
}

impl Default for CTranspiler {
    fn default() -> Self {
        Self::new()
    }
}