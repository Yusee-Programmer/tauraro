//! Built-in Functions for C Transpilation
//!
//! This module implements all Python-compatible built-in functions
//! (print, len, str, int, float, range, enumerate, etc.)

use std::collections::HashSet;

/// List of all supported builtin functions
pub fn get_all_builtins() -> Vec<&'static str> {
    vec![
        "print", "len", "str", "int", "float", "bool", "list", "dict",
        "tuple", "set", "frozenset", "range", "enumerate", "zip", "map",
        "filter", "sorted", "reversed", "any", "all", "sum", "min", "max",
        "abs", "round", "pow", "divmod", "chr", "ord", "hex", "oct", "bin",
        "input", "open", "type", "isinstance", "issubclass", "callable",
        "hasattr", "getattr", "setattr", "delattr", "dir", "vars", "id",
        "hash", "format", "repr", "ascii", "bytes", "bytearray", "memoryview",
        "compile", "eval", "exec", "globals", "locals",
    ]
}

/// Check if a function name is a builtin
pub fn is_builtin_function(func_name: &str) -> bool {
    matches!(
        func_name,
        "print" | "len" | "str" | "int" | "float" | "bool" | "list" | "dict"
        | "tuple" | "set" | "frozenset" | "range" | "abs" | "min" | "max"
        | "sum" | "round" | "pow" | "enumerate" | "zip" | "map" | "filter"
        | "sorted" | "reversed" | "any" | "all" | "open" | "input"
        | "isinstance" | "type" | "callable" | "hasattr" | "getattr"
        | "setattr" | "delattr" | "format" | "chr" | "ord" | "hex" | "oct"
        | "bin" | "divmod" | "repr" | "ascii" | "dir" | "vars" | "id"
        | "hash" | "bytes" | "bytearray"
    )
}

/// Generate function declarations for used builtins
pub fn generate_builtin_declarations(used_builtins: &HashSet<String>) -> String {
    let mut code = String::new();

    if !used_builtins.is_empty() {
        code.push_str("// Builtin function declarations\n");
        for builtin in used_builtins {
            code.push_str(&format!(
                "tauraro_value_t* tauraro_{}(int argc, tauraro_value_t** args);\n",
                builtin
            ));
        }
        code.push_str("\n");
    }

    code
}

/// Generate implementation for a specific builtin
pub fn generate_builtin_implementation(func_name: &str) -> String {
    match func_name {
        "print" => generate_print_impl(),
        "len" => generate_len_impl(),
        "str" => generate_str_impl(),
        "int" => generate_int_impl(),
        "float" => generate_float_impl(),
        "bool" => generate_bool_impl(),
        "list" => generate_list_impl(),
        "dict" => generate_dict_impl(),
        "tuple" => generate_tuple_impl(),
        "set" => generate_set_impl(),
        "range" => generate_range_impl(),
        "enumerate" => generate_enumerate_impl(),
        "zip" => generate_zip_impl(),
        "map" => generate_map_impl(),
        "filter" => generate_filter_impl(),
        "sorted" => generate_sorted_impl(),
        "reversed" => generate_reversed_impl(),
        "any" => generate_any_impl(),
        "all" => generate_all_impl(),
        "sum" => generate_sum_impl(),
        "min" => generate_min_impl(),
        "max" => generate_max_impl(),
        "abs" => generate_abs_impl(),
        "round" => generate_round_impl(),
        "pow" => generate_pow_impl(),
        "isinstance" => generate_isinstance_impl(),
        "type" => generate_type_impl(),
        "callable" => generate_callable_impl(),
        "hasattr" => generate_hasattr_impl(),
        "getattr" => generate_getattr_impl(),
        "setattr" => generate_setattr_impl(),
        "delattr" => generate_delattr_impl(),
        "chr" => generate_chr_impl(),
        "ord" => generate_ord_impl(),
        "hex" => generate_hex_impl(),
        "oct" => generate_oct_impl(),
        "bin" => generate_bin_impl(),
        "input" => generate_input_impl(),
        "format" => generate_format_impl(),
        "repr" => generate_repr_impl(),
        "divmod" => generate_divmod_impl(),
        _ => generate_generic_builtin_impl(func_name),
    }
}

fn generate_print_impl() -> String {
    r#"tauraro_value_t* tauraro_print(int argc, tauraro_value_t** args) {
    for (int i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        tauraro_value_t* arg = args[i];
        if (!arg) {
            printf("None");
            continue;
        }
        switch (arg->type) {
            case TAURARO_INT:
                printf("%ld", arg->data.int_val);
                break;
            case TAURARO_FLOAT:
                printf("%g", arg->data.float_val);
                break;
            case TAURARO_BOOL:
                printf("%s", arg->data.bool_val ? "True" : "False");
                break;
            case TAURARO_STRING:
                printf("%s", arg->data.str_val);
                break;
            case TAURARO_NONE:
                printf("None");
                break;
            case TAURARO_LIST:
                printf("[list of size %zu]", arg->data.list_val->size);
                break;
            case TAURARO_DICT:
                printf("{dict of size %zu}", arg->data.dict_val->size);
                break;
            case TAURARO_TUPLE:
                printf("(tuple of size %zu)", arg->data.tuple_val->size);
                break;
            case TAURARO_OBJECT:
                printf("<%s object at %p>", ((tauraro_object_t*)arg->data.obj_val)->class_name, (void*)arg);
                break;
            default:
                printf("<object>");
                break;
        }
    }
    printf("\n");
    fflush(stdout);
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_len_impl() -> String {
    r#"tauraro_value_t* tauraro_len(int argc, tauraro_value_t** args) {
    if (argc != 1) return NULL;
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    switch (args[0]->type) {
        case TAURARO_STRING:
            result->data.int_val = strlen(args[0]->data.str_val);
            break;
        case TAURARO_LIST:
            result->data.int_val = args[0]->data.list_val->size;
            break;
        case TAURARO_DICT:
            result->data.int_val = args[0]->data.dict_val->size;
            break;
        case TAURARO_TUPLE:
            result->data.int_val = args[0]->data.tuple_val->size;
            break;
        case TAURARO_SET:
            result->data.int_val = args[0]->data.set_val->size;
            break;
        default:
            result->data.int_val = 0;
            break;
    }
    return result;
}
"#.to_string()
}

fn generate_str_impl() -> String {
    r#"tauraro_value_t* tauraro_str(int argc, tauraro_value_t** args) {
    if (argc == 0) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_STRING;
        result->data.str_val = strdup("");
        return result;
    }
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    char buffer[512];
    switch (args[0]->type) {
        case TAURARO_INT:
            snprintf(buffer, sizeof(buffer), "%ld", args[0]->data.int_val);
            result->data.str_val = strdup(buffer);
            break;
        case TAURARO_FLOAT:
            snprintf(buffer, sizeof(buffer), "%g", args[0]->data.float_val);
            result->data.str_val = strdup(buffer);
            break;
        case TAURARO_BOOL:
            result->data.str_val = strdup(args[0]->data.bool_val ? "True" : "False");
            break;
        case TAURARO_STRING:
            result->data.str_val = strdup(args[0]->data.str_val);
            break;
        case TAURARO_NONE:
            result->data.str_val = strdup("None");
            break;
        default:
            snprintf(buffer, sizeof(buffer), "<object at %p>", (void*)args[0]);
            result->data.str_val = strdup(buffer);
            break;
    }
    return result;
}
"#.to_string()
}

fn generate_int_impl() -> String {
    r#"tauraro_value_t* tauraro_int(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    if (argc == 0) {
        result->data.int_val = 0;
    } else {
        switch (args[0]->type) {
            case TAURARO_INT:
                result->data.int_val = args[0]->data.int_val;
                break;
            case TAURARO_FLOAT:
                result->data.int_val = (int64_t)args[0]->data.float_val;
                break;
            case TAURARO_BOOL:
                result->data.int_val = args[0]->data.bool_val ? 1 : 0;
                break;
            case TAURARO_STRING:
                result->data.int_val = strtoll(args[0]->data.str_val, NULL, 10);
                break;
            default:
                result->data.int_val = 0;
                break;
        }
    }
    return result;
}
"#.to_string()
}

fn generate_float_impl() -> String {
    r#"tauraro_value_t* tauraro_float(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    if (argc == 0) {
        result->data.float_val = 0.0;
    } else {
        switch (args[0]->type) {
            case TAURARO_INT:
                result->data.float_val = (double)args[0]->data.int_val;
                break;
            case TAURARO_FLOAT:
                result->data.float_val = args[0]->data.float_val;
                break;
            case TAURARO_BOOL:
                result->data.float_val = args[0]->data.bool_val ? 1.0 : 0.0;
                break;
            case TAURARO_STRING:
                result->data.float_val = strtod(args[0]->data.str_val, NULL);
                break;
            default:
                result->data.float_val = 0.0;
                break;
        }
    }
    return result;
}
"#.to_string()
}

fn generate_bool_impl() -> String {
    r#"tauraro_value_t* tauraro_bool(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    if (argc == 0) {
        result->data.bool_val = false;
    } else {
        switch (args[0]->type) {
            case TAURARO_INT:
                result->data.bool_val = args[0]->data.int_val != 0;
                break;
            case TAURARO_FLOAT:
                result->data.bool_val = args[0]->data.float_val != 0.0;
                break;
            case TAURARO_BOOL:
                result->data.bool_val = args[0]->data.bool_val;
                break;
            case TAURARO_STRING:
                result->data.bool_val = strlen(args[0]->data.str_val) > 0;
                break;
            case TAURARO_NONE:
                result->data.bool_val = false;
                break;
            case TAURARO_LIST:
                result->data.bool_val = args[0]->data.list_val->size > 0;
                break;
            case TAURARO_DICT:
                result->data.bool_val = args[0]->data.dict_val->size > 0;
                break;
            default:
                result->data.bool_val = true;
                break;
        }
    }
    return result;
}
"#.to_string()
}

fn generate_list_impl() -> String {
    r#"tauraro_value_t* tauraro_list(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);
    result->data.list_val = list;
    return result;
}
"#.to_string()
}

fn generate_dict_impl() -> String {
    r#"tauraro_value_t* tauraro_dict(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_DICT;
    tauraro_dict_t* dict = malloc(sizeof(tauraro_dict_t));
    dict->size = 0;
    dict->capacity = 10;
    dict->keys = malloc(sizeof(char*) * dict->capacity);
    dict->values = malloc(sizeof(tauraro_value_t*) * dict->capacity);
    result->data.dict_val = dict;
    return result;
}
"#.to_string()
}

fn generate_tuple_impl() -> String {
    r#"tauraro_value_t* tauraro_tuple(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_TUPLE;
    tauraro_tuple_t* tuple = malloc(sizeof(tauraro_tuple_t));
    tuple->size = argc;
    tuple->capacity = argc > 0 ? argc : 1;
    tuple->items = malloc(sizeof(tauraro_value_t*) * tuple->capacity);
    for (int i = 0; i < argc; i++) {
        tuple->items[i] = args[i];
        tauraro_incref(args[i]);
    }
    result->data.tuple_val = tuple;
    return result;
}
"#.to_string()
}

fn generate_set_impl() -> String {
    r#"tauraro_value_t* tauraro_set(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_SET;
    tauraro_set_t* set = malloc(sizeof(tauraro_set_t));
    set->size = 0;
    set->capacity = 10;
    set->items = malloc(sizeof(tauraro_value_t*) * set->capacity);
    result->data.set_val = set;
    return result;
}
"#.to_string()
}

fn generate_range_impl() -> String {
    r#"tauraro_value_t* tauraro_range(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_RANGE;
    tauraro_range_t* range = malloc(sizeof(tauraro_range_t));

    if (argc == 1) {
        range->start = 0;
        range->stop = args[0]->data.int_val;
        range->step = 1;
    } else if (argc == 2) {
        range->start = args[0]->data.int_val;
        range->stop = args[1]->data.int_val;
        range->step = 1;
    } else if (argc >= 3) {
        range->start = args[0]->data.int_val;
        range->stop = args[1]->data.int_val;
        range->step = args[2]->data.int_val;
    }

    result->data.range_val = range;
    return result;
}
"#.to_string()
}

// Additional builtin implementations
fn generate_isinstance_impl() -> String {
    r#"tauraro_value_t* tauraro_isinstance(int argc, tauraro_value_t** args) {
    if (argc != 2) return NULL;
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    
    // Check if first argument is an object
    if (args[0]->type == TAURARO_OBJECT) {
        tauraro_object_t* obj = (tauraro_object_t*)args[0]->data.obj_val;
        // Second argument should be a class name string or class reference
        if (args[1]->type == TAURARO_STRING) {
            // Compare class name
            result->data.bool_val = (strcmp(obj->class_name, args[1]->data.str_val) == 0);
        } else if (args[1]->type == TAURARO_OBJECT) {
            // Compare with class object
            tauraro_object_t* class_obj = (tauraro_object_t*)args[1]->data.obj_val;
            result->data.bool_val = (strcmp(obj->class_name, class_obj->class_name) == 0);
        } else {
            // If second argument is not a string or object, treat as class name
            // This handles cases where class names are passed as variables
            result->data.bool_val = false;
        }
    } else {
        // For non-objects, compare types directly
        result->data.bool_val = (args[0]->type == args[1]->type);
    }
    
    return result;
}
"#.to_string()
}

fn generate_type_impl() -> String {
    r#"tauraro_value_t* tauraro_type(int argc, tauraro_value_t** args) {
    if (argc != 1) return NULL;
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    switch (args[0]->type) {
        case TAURARO_INT:
            result->data.str_val = strdup("<class 'int'>");
            break;
        case TAURARO_FLOAT:
            result->data.str_val = strdup("<class 'float'>");
            break;
        case TAURARO_BOOL:
            result->data.str_val = strdup("<class 'bool'>");
            break;
        case TAURARO_STRING:
            result->data.str_val = strdup("<class 'str'>");
            break;
        case TAURARO_LIST:
            result->data.str_val = strdup("<class 'list'>");
            break;
        case TAURARO_DICT:
            result->data.str_val = strdup("<class 'dict'>");
            break;
        case TAURARO_TUPLE:
            result->data.str_val = strdup("<class 'tuple'>");
            break;
        case TAURARO_SET:
            result->data.str_val = strdup("<class 'set'>");
            break;
        case TAURARO_OBJECT:
            result->data.str_val = strdup("<class 'object'>");
            break;
        case TAURARO_NONE:
            result->data.str_val = strdup("<class 'NoneType'>");
            break;
        default:
            result->data.str_val = strdup("<class 'object'>");
            break;
    }
    return result;
}
"#.to_string()
}

fn generate_callable_impl() -> String {
    r#"tauraro_value_t* tauraro_callable(int argc, tauraro_value_t** args) {
    if (argc != 1) return NULL;
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    result->data.bool_val = (args[0]->type == TAURARO_FUNCTION);
    return result;
}
"#.to_string()
}

// Placeholder implementations for remaining builtins
fn generate_enumerate_impl() -> String { generate_generic_builtin_impl("enumerate") }
fn generate_zip_impl() -> String { generate_generic_builtin_impl("zip") }
fn generate_map_impl() -> String { generate_generic_builtin_impl("map") }
fn generate_filter_impl() -> String { generate_generic_builtin_impl("filter") }
fn generate_sorted_impl() -> String { generate_generic_builtin_impl("sorted") }
fn generate_reversed_impl() -> String { generate_generic_builtin_impl("reversed") }
fn generate_any_impl() -> String { generate_generic_builtin_impl("any") }
fn generate_all_impl() -> String { generate_generic_builtin_impl("all") }
fn generate_sum_impl() -> String { generate_generic_builtin_impl("sum") }
fn generate_min_impl() -> String { generate_generic_builtin_impl("min") }
fn generate_max_impl() -> String { generate_generic_builtin_impl("max") }
fn generate_abs_impl() -> String { generate_generic_builtin_impl("abs") }
fn generate_round_impl() -> String { generate_generic_builtin_impl("round") }
fn generate_pow_impl() -> String { generate_generic_builtin_impl("pow") }
fn generate_hasattr_impl() -> String { generate_generic_builtin_impl("hasattr") }
fn generate_getattr_impl() -> String { generate_generic_builtin_impl("getattr") }
fn generate_setattr_impl() -> String { generate_generic_builtin_impl("setattr") }
fn generate_delattr_impl() -> String { generate_generic_builtin_impl("delattr") }
fn generate_chr_impl() -> String { generate_generic_builtin_impl("chr") }
fn generate_ord_impl() -> String { generate_generic_builtin_impl("ord") }
fn generate_hex_impl() -> String { generate_generic_builtin_impl("hex") }
fn generate_oct_impl() -> String { generate_generic_builtin_impl("oct") }
fn generate_bin_impl() -> String { generate_generic_builtin_impl("bin") }
fn generate_input_impl() -> String { generate_generic_builtin_impl("input") }
fn generate_format_impl() -> String { generate_generic_builtin_impl("format") }
fn generate_repr_impl() -> String { generate_generic_builtin_impl("repr") }
fn generate_divmod_impl() -> String { generate_generic_builtin_impl("divmod") }

fn generate_generic_builtin_impl(func_name: &str) -> String {
    format!(
        r#"tauraro_value_t* tauraro_{}(int argc, tauraro_value_t** args) {{
    // Implementation for {}
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}}
"#,
        func_name, func_name
    )
}
