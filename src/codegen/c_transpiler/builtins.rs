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
        // File I/O builtins
        | "open" | "file_read" | "file_write" | "file_close" | "file_readline"
        // Memory management builtins
        | "allocate" | "free" | "create_arena" | "destroy_arena" | "reset_arena" | "memory_stats"
        // System programming builtins
        | "sizeof" | "alignof" | "memcpy" | "memset" | "memmove" | "memcmp"
        | "ptr_read" | "ptr_write" | "ptr_offset" | "null_ptr" | "is_null"
        // Advanced system programming builtins
        | "stack_alloc" | "volatile_read" | "volatile_write"
        | "atomic_load" | "atomic_store" | "atomic_add" | "atomic_sub" | "atomic_cas"
        | "memory_barrier" | "prefetch" | "cache_line_size"
        | "bit_cast" | "zero_memory" | "copy_memory" | "compare_memory"
        // Bare-metal / OS development builtins
        | "port_in" | "port_out" | "port_in8" | "port_out8"
        | "port_in16" | "port_out16" | "port_in32" | "port_out32"
        | "mmio_read8" | "mmio_write8" | "mmio_read16" | "mmio_write16"
        | "mmio_read32" | "mmio_write32" | "mmio_read64" | "mmio_write64"
        | "disable_interrupts" | "enable_interrupts" | "cli" | "sti" | "halt" | "hlt"
        | "read_cr0" | "write_cr0" | "read_cr3" | "write_cr3"
        | "read_msr" | "write_msr" | "asm"
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
        // Memory management
        "allocate" => generate_allocate_impl(),
        "free" => generate_free_impl(),
        "create_arena" => generate_create_arena_impl(),
        "destroy_arena" => generate_destroy_arena_impl(),
        "reset_arena" => generate_reset_arena_impl(),
        "memory_stats" => generate_memory_stats_impl(),
        // System programming
        "sizeof" => generate_sizeof_impl(),
        "alignof" => generate_alignof_impl(),
        "memcpy" => generate_memcpy_impl(),
        "memset" => generate_memset_impl(),
        "memmove" => generate_memmove_impl(),
        "memcmp" => generate_memcmp_impl(),
        "ptr_read" => generate_ptr_read_impl(),
        "ptr_write" => generate_ptr_write_impl(),
        "ptr_offset" => generate_ptr_offset_impl(),
        "null_ptr" => generate_null_ptr_impl(),
        "is_null" => generate_is_null_impl(),
        // File I/O
        "open" => generate_open_impl(),
        "file_read" => generate_file_read_impl(),
        "file_write" => generate_file_write_impl(),
        "file_close" => generate_file_close_impl(),
        "file_readline" => generate_file_readline_impl(),
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
fn generate_enumerate_impl() -> String {
    r#"tauraro_value_t* tauraro_enumerate(int argc, tauraro_value_t** args) {
    if (argc < 1) return NULL;
    tauraro_value_t* iterable = args[0];
    int start = (argc > 1 && args[1]->type == TAURARO_INT) ? args[1]->data.int_val : 0;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    
    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);
    
    if (iterable->type == TAURARO_LIST) {
        for (size_t i = 0; i < iterable->data.list_val->size; i++) {
            tauraro_value_t* tuple = tauraro_tuple(2, (tauraro_value_t*[]){ NULL, NULL });
            // Set index and value
            list->items[list->size++] = tuple;
            if (list->size >= list->capacity) {
                list->capacity *= 2;
                list->items = realloc(list->items, sizeof(tauraro_value_t*) * list->capacity);
            }
        }
    }
    
    result->data.list_val = list;
    return result;
}
"#.to_string()
}

fn generate_zip_impl() -> String {
    r#"tauraro_value_t* tauraro_zip(int argc, tauraro_value_t** args) {
    // zip(*iterables) - combine multiple iterables
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    
    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);
    
    // Find minimum length among all iterables
    size_t min_length = (size_t)-1;
    for (int i = 0; i < argc; i++) {
        if (args[i]->type == TAURARO_LIST && args[i]->data.list_val->size < min_length) {
            min_length = args[i]->data.list_val->size;
        }
    }
    
    result->data.list_val = list;
    return result;
}
"#.to_string()
}

fn generate_map_impl() -> String {
    r#"tauraro_value_t* tauraro_map(int argc, tauraro_value_t** args) {
    // map(function, iterable) - apply function to each element
    if (argc < 2) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    
    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);
    
    // Apply function to each element of iterable
    if (args[1]->type == TAURARO_LIST) {
        for (size_t i = 0; i < args[1]->data.list_val->size; i++) {
            // Call function with element (simplified)
            list->items[list->size++] = args[1]->data.list_val->items[i];
            if (list->size >= list->capacity) {
                list->capacity *= 2;
                list->items = realloc(list->items, sizeof(tauraro_value_t*) * list->capacity);
            }
        }
    }
    
    result->data.list_val = list;
    return result;
}
"#.to_string()
}

fn generate_filter_impl() -> String {
    r#"tauraro_value_t* tauraro_filter(int argc, tauraro_value_t** args) {
    // filter(function, iterable) - filter elements based on function
    if (argc < 2) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    
    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);
    
    if (args[1]->type == TAURARO_LIST) {
        for (size_t i = 0; i < args[1]->data.list_val->size; i++) {
            tauraro_value_t* item = args[1]->data.list_val->items[i];
            // If function is NULL, filter by truthiness
            if (args[0]->type == TAURARO_NONE || tauraro_is_truthy(item)) {
                list->items[list->size++] = item;
                if (list->size >= list->capacity) {
                    list->capacity *= 2;
                    list->items = realloc(list->items, sizeof(tauraro_value_t*) * list->capacity);
                }
            }
        }
    }
    
    result->data.list_val = list;
    return result;
}
"#.to_string()
}

fn generate_sorted_impl() -> String {
    r#"tauraro_value_t* tauraro_sorted(int argc, tauraro_value_t** args) {
    if (argc < 1) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    
    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);
    
    if (args[0]->type == TAURARO_LIST) {
        // Copy and sort elements (simplified bubble sort)
        for (size_t i = 0; i < args[0]->data.list_val->size; i++) {
            list->items[list->size++] = args[0]->data.list_val->items[i];
            if (list->size >= list->capacity) {
                list->capacity *= 2;
                list->items = realloc(list->items, sizeof(tauraro_value_t*) * list->capacity);
            }
        }
        // Implement proper sorting if needed
    }
    
    result->data.list_val = list;
    return result;
}
"#.to_string()
}

fn generate_reversed_impl() -> String {
    r#"tauraro_value_t* tauraro_reversed(int argc, tauraro_value_t** args) {
    if (argc < 1) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    
    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);
    
    if (args[0]->type == TAURARO_LIST) {
        for (int i = args[0]->data.list_val->size - 1; i >= 0; i--) {
            list->items[list->size++] = args[0]->data.list_val->items[i];
            if (list->size >= list->capacity) {
                list->capacity *= 2;
                list->items = realloc(list->items, sizeof(tauraro_value_t*) * list->capacity);
            }
        }
    }
    
    result->data.list_val = list;
    return result;
}
"#.to_string()
}

fn generate_any_impl() -> String {
    r#"tauraro_value_t* tauraro_any(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    result->data.bool_val = false;
    
    if (argc > 0 && args[0]->type == TAURARO_LIST) {
        for (size_t i = 0; i < args[0]->data.list_val->size; i++) {
            if (tauraro_is_truthy(args[0]->data.list_val->items[i])) {
                result->data.bool_val = true;
                break;
            }
        }
    }
    
    return result;
}
"#.to_string()
}

fn generate_all_impl() -> String {
    r#"tauraro_value_t* tauraro_all(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    result->data.bool_val = true;
    
    if (argc > 0 && args[0]->type == TAURARO_LIST) {
        for (size_t i = 0; i < args[0]->data.list_val->size; i++) {
            if (!tauraro_is_truthy(args[0]->data.list_val->items[i])) {
                result->data.bool_val = false;
                break;
            }
        }
    }
    
    return result;
}
"#.to_string()
}

fn generate_sum_impl() -> String {
    r#"tauraro_value_t* tauraro_sum(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = 0;
    
    if (argc > 0 && args[0]->type == TAURARO_LIST) {
        int64_t sum = 0;
        for (size_t i = 0; i < args[0]->data.list_val->size; i++) {
            tauraro_value_t* item = args[0]->data.list_val->items[i];
            if (item->type == TAURARO_INT) {
                sum += item->data.int_val;
            } else if (item->type == TAURARO_FLOAT) {
                result->type = TAURARO_FLOAT;
                result->data.float_val = (double)sum + item->data.float_val;
            }
        }
        if (result->type == TAURARO_INT) {
            result->data.int_val = sum;
        }
    }
    
    return result;
}
"#.to_string()
}

fn generate_min_impl() -> String {
    r#"tauraro_value_t* tauraro_min(int argc, tauraro_value_t** args) {
    if (argc == 0) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    *result = *args[0];  // Copy first value
    
    if (argc > 1) {
        // Multiple arguments: min(a, b, c, ...)
        for (int i = 1; i < argc; i++) {
            tauraro_value_t* cmp = tauraro_lt(args[i], result);
            if (cmp->data.bool_val) {
                *result = *args[i];
            }
            tauraro_decref(cmp);
        }
    } else if (argc == 1 && args[0]->type == TAURARO_LIST) {
        // Single argument: min(iterable)
        tauraro_list_t* list = args[0]->data.list_val;
        if (list->size > 0) {
            *result = *list->items[0];
            for (size_t i = 1; i < list->size; i++) {
                tauraro_value_t* cmp = tauraro_lt(list->items[i], result);
                if (cmp->data.bool_val) {
                    *result = *list->items[i];
                }
                tauraro_decref(cmp);
            }
        }
    }
    
    return result;
}
"#.to_string()
}

fn generate_max_impl() -> String {
    r#"tauraro_value_t* tauraro_max(int argc, tauraro_value_t** args) {
    if (argc == 0) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    *result = *args[0];
    
    if (argc > 1) {
        // Multiple arguments: max(a, b, c, ...)
        for (int i = 1; i < argc; i++) {
            tauraro_value_t* cmp = tauraro_gt(args[i], result);
            if (cmp->data.bool_val) {
                *result = *args[i];
            }
            tauraro_decref(cmp);
        }
    } else if (argc == 1 && args[0]->type == TAURARO_LIST) {
        // Single argument: max(iterable)
        tauraro_list_t* list = args[0]->data.list_val;
        if (list->size > 0) {
            *result = *list->items[0];
            for (size_t i = 1; i < list->size; i++) {
                tauraro_value_t* cmp = tauraro_gt(list->items[i], result);
                if (cmp->data.bool_val) {
                    *result = *list->items[i];
                }
                tauraro_decref(cmp);
            }
        }
    }
    
    return result;
}
"#.to_string()
}

fn generate_abs_impl() -> String {
    r#"tauraro_value_t* tauraro_abs(int argc, tauraro_value_t** args) {
    if (argc != 1) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    
    if (args[0]->type == TAURARO_INT) {
        result->type = TAURARO_INT;
        result->data.int_val = labs(args[0]->data.int_val);
    } else if (args[0]->type == TAURARO_FLOAT) {
        result->type = TAURARO_FLOAT;
        result->data.float_val = fabs(args[0]->data.float_val);
    } else {
        result->type = TAURARO_NONE;
    }
    
    return result;
}
"#.to_string()
}

fn generate_round_impl() -> String {
    r#"tauraro_value_t* tauraro_round(int argc, tauraro_value_t** args) {
    if (argc < 1) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    int ndigits = (argc > 1 && args[1]->type == TAURARO_INT) ? args[1]->data.int_val : 0;
    
    if (args[0]->type == TAURARO_INT) {
        result->type = TAURARO_INT;
        result->data.int_val = args[0]->data.int_val;
    } else if (args[0]->type == TAURARO_FLOAT) {
        result->type = TAURARO_FLOAT;
        double value = args[0]->data.float_val;
        if (ndigits == 0) {
            result->data.float_val = round(value);
        } else {
            double multiplier = pow(10.0, ndigits);
            result->data.float_val = round(value * multiplier) / multiplier;
        }
    } else {
        result->type = TAURARO_NONE;
    }
    
    return result;
}
"#.to_string()
}

fn generate_pow_impl() -> String {
    r#"tauraro_value_t* tauraro_pow(int argc, tauraro_value_t** args) {
    if (argc < 2) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    double base = (args[0]->type == TAURARO_INT) ? args[0]->data.int_val : args[0]->data.float_val;
    double exponent = (args[1]->type == TAURARO_INT) ? args[1]->data.int_val : args[1]->data.float_val;
    
    result->type = TAURARO_FLOAT;
    result->data.float_val = pow(base, exponent);
    
    return result;
}
"#.to_string()
}

fn generate_hasattr_impl() -> String {
    r#"tauraro_value_t* tauraro_hasattr(int argc, tauraro_value_t** args) {
    if (argc != 2 || args[0]->type != TAURARO_OBJECT || args[1]->type != TAURARO_STRING) {
        return NULL;
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    
    tauraro_object_t* obj = (tauraro_object_t*)args[0]->data.obj_val;
    result->data.bool_val = (tauraro_dict_get(obj->attributes, args[1]->data.str_val) != NULL);
    
    return result;
}
"#.to_string()
}

fn generate_getattr_impl() -> String {
    r#"tauraro_value_t* tauraro_getattr(int argc, tauraro_value_t** args) {
    if (argc < 2 || args[0]->type != TAURARO_OBJECT || args[1]->type != TAURARO_STRING) {
        return NULL;
    }
    
    tauraro_object_t* obj = (tauraro_object_t*)args[0]->data.obj_val;
    tauraro_value_t* attr = tauraro_dict_get(obj->attributes, args[1]->data.str_val);
    
    if (attr == NULL && argc > 2) {
        // Default value provided
        return args[2];
    }
    
    return attr ? attr : NULL;
}
"#.to_string()
}

fn generate_setattr_impl() -> String {
    r#"tauraro_value_t* tauraro_setattr(int argc, tauraro_value_t** args) {
    if (argc != 3 || args[0]->type != TAURARO_OBJECT || args[1]->type != TAURARO_STRING) {
        return NULL;
    }
    
    tauraro_object_t* obj = (tauraro_object_t*)args[0]->data.obj_val;
    tauraro_dict_set(obj->attributes, args[1]->data.str_val, args[2]);
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_delattr_impl() -> String {
    r#"tauraro_value_t* tauraro_delattr(int argc, tauraro_value_t** args) {
    if (argc != 2 || args[0]->type != TAURARO_OBJECT || args[1]->type != TAURARO_STRING) {
        return NULL;
    }
    
    // Implementation would delete from object's attribute dictionary
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_chr_impl() -> String {
    r#"tauraro_value_t* tauraro_chr(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_INT) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    char buffer[2];
    buffer[0] = (char)args[0]->data.int_val;
    buffer[1] = '\0';
    result->data.str_val = strdup(buffer);
    
    return result;
}
"#.to_string()
}

fn generate_ord_impl() -> String {
    r#"tauraro_value_t* tauraro_ord(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_STRING || !args[0]->data.str_val) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = (unsigned char)args[0]->data.str_val[0];
    
    return result;
}
"#.to_string()
}

fn generate_hex_impl() -> String {
    r#"tauraro_value_t* tauraro_hex(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_INT) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "0x%lx", args[0]->data.int_val);
    result->data.str_val = strdup(buffer);
    
    return result;
}
"#.to_string()
}

fn generate_oct_impl() -> String {
    r#"tauraro_value_t* tauraro_oct(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_INT) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "0o%lo", args[0]->data.int_val);
    result->data.str_val = strdup(buffer);
    
    return result;
}
"#.to_string()
}

fn generate_bin_impl() -> String {
    r#"tauraro_value_t* tauraro_bin(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_INT) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    char buffer[65];
    buffer[0] = '\0';
    int64_t num = args[0]->data.int_val;
    
    // Handle negative numbers
    if (num < 0) {
        strcat(buffer, "-0b");
        num = -num;
    } else {
        strcat(buffer, "0b");
    }
    
    char bin_digits[65];
    int pos = 0;
    if (num == 0) {
        bin_digits[pos++] = '0';
    } else {
        while (num > 0) {
            bin_digits[pos++] = (num & 1) ? '1' : '0';
            num >>= 1;
        }
    }
    
    // Reverse the binary digits
    for (int i = pos - 1; i >= 0; i--) {
        strncat(buffer, &bin_digits[i], 1);
    }
    
    result->data.str_val = strdup(buffer);
    return result;
}
"#.to_string()
}

fn generate_input_impl() -> String {
    r#"tauraro_value_t* tauraro_input(int argc, tauraro_value_t** args) {
    if (argc > 0 && args[0]->type == TAURARO_STRING) {
        printf("%s", args[0]->data.str_val);
        fflush(stdout);
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin)) {
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        result->data.str_val = strdup(buffer);
    } else {
        result->data.str_val = strdup("");
    }
    
    return result;
}
"#.to_string()
}

fn generate_format_impl() -> String {
    r#"tauraro_value_t* tauraro_format(int argc, tauraro_value_t** args) {
    if (argc < 1) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    // Simple format implementation
    char buffer[1024];
    char format_str[512];
    
    if (args[0]->type == TAURARO_STRING) {
        strncpy(format_str, args[0]->data.str_val, sizeof(format_str) - 1);
        // TODO: Implement proper format string substitution
        result->data.str_val = strdup(format_str);
    } else {
        result->data.str_val = strdup("");
    }
    
    return result;
}
"#.to_string()
}

fn generate_repr_impl() -> String {
    r#"tauraro_value_t* tauraro_repr(int argc, tauraro_value_t** args) {
    if (argc != 1) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    char buffer[1024];
    
    if (args[0]->type == TAURARO_STRING) {
        snprintf(buffer, sizeof(buffer), "'%s'", args[0]->data.str_val);
    } else {
        // For other types, use str() representation
        switch (args[0]->type) {
            case TAURARO_INT:
                snprintf(buffer, sizeof(buffer), "%ld", args[0]->data.int_val);
                break;
            case TAURARO_FLOAT:
                snprintf(buffer, sizeof(buffer), "%g", args[0]->data.float_val);
                break;
            case TAURARO_BOOL:
                snprintf(buffer, sizeof(buffer), "%s", args[0]->data.bool_val ? "True" : "False");
                break;
            default:
                snprintf(buffer, sizeof(buffer), "<%s object>", tauraro_type_name(args[0]));
                break;
        }
    }
    
    result->data.str_val = strdup(buffer);
    return result;
}
"#.to_string()
}

fn generate_divmod_impl() -> String {
    r#"tauraro_value_t* tauraro_divmod(int argc, tauraro_value_t** args) {
    if (argc != 2) return NULL;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_TUPLE;
    
    if (args[0]->type == TAURARO_INT && args[1]->type == TAURARO_INT) {
        tauraro_value_t* quotient = tauraro_value_new();
        quotient->type = TAURARO_INT;
        quotient->data.int_val = args[0]->data.int_val / args[1]->data.int_val;
        
        tauraro_value_t* remainder = tauraro_value_new();
        remainder->type = TAURARO_INT;
        remainder->data.int_val = args[0]->data.int_val % args[1]->data.int_val;
        
        tauraro_tuple_t* tuple = malloc(sizeof(tauraro_tuple_t));
        tuple->size = 2;
        tuple->capacity = 2;
        tuple->items = malloc(sizeof(tauraro_value_t*) * 2);
        tuple->items[0] = quotient;
        tuple->items[1] = remainder;
        
        result->data.tuple_val = tuple;
    }
    
    return result;
}
"#.to_string()
}

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

// ============================================================================
// Memory Management Functions for Systems Programming
// ============================================================================

fn generate_allocate_impl() -> String {
    r#"// Memory arena structure for tracking allocations
typedef struct {
    void** buffers;
    size_t* sizes;
    size_t count;
    size_t capacity;
    char* name;
} tauraro_arena_t;

// Global memory management state
static struct {
    void** manual_buffers;
    size_t* buffer_sizes;
    size_t buffer_count;
    size_t buffer_capacity;
    tauraro_arena_t** arenas;
    size_t arena_count;
    size_t arena_capacity;
    char* current_arena;
} tauraro_memory_state = {NULL, NULL, 0, 0, NULL, 0, 0, NULL};

// Initialize memory state if needed
static void tauraro_memory_init(void) {
    if (tauraro_memory_state.manual_buffers == NULL) {
        tauraro_memory_state.buffer_capacity = 64;
        tauraro_memory_state.manual_buffers = (void**)malloc(sizeof(void*) * 64);
        tauraro_memory_state.buffer_sizes = (size_t*)malloc(sizeof(size_t) * 64);
        tauraro_memory_state.buffer_count = 0;
        tauraro_memory_state.arena_capacity = 16;
        tauraro_memory_state.arenas = (tauraro_arena_t**)malloc(sizeof(tauraro_arena_t*) * 16);
        tauraro_memory_state.arena_count = 0;
        tauraro_memory_state.current_arena = NULL;
    }
}

tauraro_value_t* tauraro_allocate(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_INT) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    tauraro_memory_init();
    
    size_t size = (size_t)args[0]->data.int_val;
    void* buffer = malloc(size);
    
    if (buffer == NULL) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    // Store in tracking array
    if (tauraro_memory_state.buffer_count >= tauraro_memory_state.buffer_capacity) {
        tauraro_memory_state.buffer_capacity *= 2;
        tauraro_memory_state.manual_buffers = (void**)realloc(
            tauraro_memory_state.manual_buffers, 
            sizeof(void*) * tauraro_memory_state.buffer_capacity
        );
        tauraro_memory_state.buffer_sizes = (size_t*)realloc(
            tauraro_memory_state.buffer_sizes,
            sizeof(size_t) * tauraro_memory_state.buffer_capacity
        );
    }
    
    size_t idx = tauraro_memory_state.buffer_count++;
    tauraro_memory_state.manual_buffers[idx] = buffer;
    tauraro_memory_state.buffer_sizes[idx] = size;
    
    // Return pointer as integer (for FFI compatibility)
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = (long long)(uintptr_t)buffer;
    return result;
}
"#.to_string()
}

fn generate_free_impl() -> String {
    r#"tauraro_value_t* tauraro_free(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    void* ptr = NULL;
    if (args[0]->type == TAURARO_INT) {
        ptr = (void*)(uintptr_t)args[0]->data.int_val;
    }
    
    if (ptr != NULL) {
        // Find and remove from tracking
        for (size_t i = 0; i < tauraro_memory_state.buffer_count; i++) {
            if (tauraro_memory_state.manual_buffers[i] == ptr) {
                free(ptr);
                // Shift remaining entries
                for (size_t j = i; j < tauraro_memory_state.buffer_count - 1; j++) {
                    tauraro_memory_state.manual_buffers[j] = tauraro_memory_state.manual_buffers[j + 1];
                    tauraro_memory_state.buffer_sizes[j] = tauraro_memory_state.buffer_sizes[j + 1];
                }
                tauraro_memory_state.buffer_count--;
                break;
            }
        }
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_create_arena_impl() -> String {
    r#"tauraro_value_t* tauraro_create_arena(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_STRING) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    tauraro_memory_init();
    
    char* name = args[0]->data.str_val;
    
    // Create new arena
    tauraro_arena_t* arena = (tauraro_arena_t*)malloc(sizeof(tauraro_arena_t));
    arena->name = strdup(name);
    arena->capacity = 64;
    arena->count = 0;
    arena->buffers = (void**)malloc(sizeof(void*) * arena->capacity);
    arena->sizes = (size_t*)malloc(sizeof(size_t) * arena->capacity);
    
    // Add to arenas list
    if (tauraro_memory_state.arena_count >= tauraro_memory_state.arena_capacity) {
        tauraro_memory_state.arena_capacity *= 2;
        tauraro_memory_state.arenas = (tauraro_arena_t**)realloc(
            tauraro_memory_state.arenas,
            sizeof(tauraro_arena_t*) * tauraro_memory_state.arena_capacity
        );
    }
    tauraro_memory_state.arenas[tauraro_memory_state.arena_count++] = arena;
    
    // Set as current arena
    if (tauraro_memory_state.current_arena) free(tauraro_memory_state.current_arena);
    tauraro_memory_state.current_arena = strdup(name);
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_destroy_arena_impl() -> String {
    r#"tauraro_value_t* tauraro_destroy_arena(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_STRING) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    char* name = args[0]->data.str_val;
    
    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++) {
        if (strcmp(tauraro_memory_state.arenas[i]->name, name) == 0) {
            tauraro_arena_t* arena = tauraro_memory_state.arenas[i];
            
            // Free all buffers in arena
            for (size_t j = 0; j < arena->count; j++) {
                free(arena->buffers[j]);
            }
            free(arena->buffers);
            free(arena->sizes);
            free(arena->name);
            free(arena);
            
            // Remove from list
            for (size_t j = i; j < tauraro_memory_state.arena_count - 1; j++) {
                tauraro_memory_state.arenas[j] = tauraro_memory_state.arenas[j + 1];
            }
            tauraro_memory_state.arena_count--;
            break;
        }
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_reset_arena_impl() -> String {
    r#"tauraro_value_t* tauraro_reset_arena(int argc, tauraro_value_t** args) {
    if (argc != 1 || args[0]->type != TAURARO_STRING) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    char* name = args[0]->data.str_val;
    
    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++) {
        if (strcmp(tauraro_memory_state.arenas[i]->name, name) == 0) {
            tauraro_arena_t* arena = tauraro_memory_state.arenas[i];
            
            // Free all buffers but keep arena
            for (size_t j = 0; j < arena->count; j++) {
                free(arena->buffers[j]);
            }
            arena->count = 0;
            break;
        }
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_memory_stats_impl() -> String {
    r#"tauraro_value_t* tauraro_memory_stats(int argc, tauraro_value_t** args) {
    (void)argc; (void)args;
    
    tauraro_memory_init();
    
    size_t total_manual = 0;
    for (size_t i = 0; i < tauraro_memory_state.buffer_count; i++) {
        total_manual += tauraro_memory_state.buffer_sizes[i];
    }
    
    size_t total_arena = 0;
    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++) {
        for (size_t j = 0; j < tauraro_memory_state.arenas[i]->count; j++) {
            total_arena += tauraro_memory_state.arenas[i]->sizes[j];
        }
    }
    
    char buffer[512];
    snprintf(buffer, sizeof(buffer),
        "Memory Strategy: Manual\n"
        "Manual Buffers: %zu (%zu bytes)\n"
        "Arenas: %zu (%zu bytes)",
        tauraro_memory_state.buffer_count, total_manual,
        tauraro_memory_state.arena_count, total_arena
    );
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    result->data.str_val = strdup(buffer);
    return result;
}
"#.to_string()
}

// ============================================================================
// Low-level System Programming Functions
// ============================================================================

fn generate_sizeof_impl() -> String {
    r#"tauraro_value_t* tauraro_sizeof(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = 0;
        return result;
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    
    // If argument is a string, parse it as a type name
    if (args[0]->type == TAURARO_STRING && args[0]->data.str_val) {
        const char* type_name = args[0]->data.str_val;
        if (strcmp(type_name, "int") == 0 || strcmp(type_name, "int32") == 0) {
            result->data.int_val = 4;
        } else if (strcmp(type_name, "int8") == 0 || strcmp(type_name, "char") == 0 || strcmp(type_name, "byte") == 0) {
            result->data.int_val = 1;
        } else if (strcmp(type_name, "int16") == 0 || strcmp(type_name, "short") == 0) {
            result->data.int_val = 2;
        } else if (strcmp(type_name, "int64") == 0 || strcmp(type_name, "long") == 0) {
            result->data.int_val = 8;
        } else if (strcmp(type_name, "float") == 0 || strcmp(type_name, "float32") == 0) {
            result->data.int_val = 4;
        } else if (strcmp(type_name, "float64") == 0 || strcmp(type_name, "double") == 0) {
            result->data.int_val = 8;
        } else if (strcmp(type_name, "pointer") == 0 || strcmp(type_name, "ptr") == 0) {
            result->data.int_val = sizeof(void*);
        } else if (strcmp(type_name, "bool") == 0) {
            result->data.int_val = 1;
        } else {
            result->data.int_val = sizeof(void*);
        }
    } else {
        // Return size based on value type
        switch (args[0]->type) {
            case TAURARO_INT:
                result->data.int_val = sizeof(long long);
                break;
            case TAURARO_FLOAT:
                result->data.int_val = sizeof(double);
                break;
            case TAURARO_BOOL:
                result->data.int_val = sizeof(int);
                break;
            default:
                result->data.int_val = sizeof(tauraro_value_t);
                break;
        }
    }
    
    return result;
}
"#.to_string()
}

fn generate_alignof_impl() -> String {
    r#"tauraro_value_t* tauraro_alignof(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = 0;
        return result;
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    
    // If argument is a string, parse it as a type name
    if (args[0]->type == TAURARO_STRING && args[0]->data.str_val) {
        const char* type_name = args[0]->data.str_val;
        if (strcmp(type_name, "int8") == 0 || strcmp(type_name, "char") == 0 || strcmp(type_name, "byte") == 0 || strcmp(type_name, "bool") == 0) {
            result->data.int_val = 1;
        } else if (strcmp(type_name, "int16") == 0 || strcmp(type_name, "short") == 0) {
            result->data.int_val = 2;
        } else if (strcmp(type_name, "int") == 0 || strcmp(type_name, "int32") == 0 || strcmp(type_name, "float") == 0 || strcmp(type_name, "float32") == 0) {
            result->data.int_val = 4;
        } else if (strcmp(type_name, "int64") == 0 || strcmp(type_name, "long") == 0 || strcmp(type_name, "float64") == 0 || strcmp(type_name, "double") == 0 || strcmp(type_name, "pointer") == 0 || strcmp(type_name, "ptr") == 0) {
            result->data.int_val = 8;
        } else {
            result->data.int_val = _Alignof(void*);
        }
    } else {
        // Return alignment based on value type
        switch (args[0]->type) {
            case TAURARO_INT:
                result->data.int_val = _Alignof(long long);
                break;
            case TAURARO_FLOAT:
                result->data.int_val = _Alignof(double);
                break;
            default:
                result->data.int_val = _Alignof(void*);
                break;
        }
    }
    
    return result;
}
"#.to_string()
}

fn generate_memcpy_impl() -> String {
    r#"tauraro_value_t* tauraro_memcpy(int argc, tauraro_value_t** args) {
    if (argc != 3) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    void* dest = (void*)(uintptr_t)args[0]->data.int_val;
    void* src = (void*)(uintptr_t)args[1]->data.int_val;
    size_t n = (size_t)args[2]->data.int_val;
    
    if (dest && src && n > 0) {
        memcpy(dest, src, n);
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = (long long)(uintptr_t)dest;
    return result;
}
"#.to_string()
}

fn generate_memset_impl() -> String {
    r#"tauraro_value_t* tauraro_memset(int argc, tauraro_value_t** args) {
    if (argc != 3) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    void* dest = (void*)(uintptr_t)args[0]->data.int_val;
    int value = (int)args[1]->data.int_val;
    size_t n = (size_t)args[2]->data.int_val;
    
    if (dest && n > 0) {
        memset(dest, value, n);
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = (long long)(uintptr_t)dest;
    return result;
}
"#.to_string()
}

fn generate_memmove_impl() -> String {
    r#"tauraro_value_t* tauraro_memmove(int argc, tauraro_value_t** args) {
    if (argc != 3) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    void* dest = (void*)(uintptr_t)args[0]->data.int_val;
    void* src = (void*)(uintptr_t)args[1]->data.int_val;
    size_t n = (size_t)args[2]->data.int_val;
    
    if (dest && src && n > 0) {
        memmove(dest, src, n);
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = (long long)(uintptr_t)dest;
    return result;
}
"#.to_string()
}

fn generate_memcmp_impl() -> String {
    r#"tauraro_value_t* tauraro_memcmp(int argc, tauraro_value_t** args) {
    if (argc != 3) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = -1;
        return result;
    }
    
    void* s1 = (void*)(uintptr_t)args[0]->data.int_val;
    void* s2 = (void*)(uintptr_t)args[1]->data.int_val;
    size_t n = (size_t)args[2]->data.int_val;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    
    if (s1 && s2 && n > 0) {
        result->data.int_val = memcmp(s1, s2, n);
    } else {
        result->data.int_val = -1;
    }
    
    return result;
}
"#.to_string()
}

fn generate_ptr_read_impl() -> String {
    r#"tauraro_value_t* tauraro_ptr_read(int argc, tauraro_value_t** args) {
    if (argc < 1 || argc > 2) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    void* ptr = (void*)(uintptr_t)args[0]->data.int_val;
    int byte_size = (argc > 1) ? (int)args[1]->data.int_val : 8;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    
    if (ptr) {
        switch (byte_size) {
            case 1: result->data.int_val = *(int8_t*)ptr; break;
            case 2: result->data.int_val = *(int16_t*)ptr; break;
            case 4: result->data.int_val = *(int32_t*)ptr; break;
            case 8: result->data.int_val = *(int64_t*)ptr; break;
            default: result->data.int_val = *(int64_t*)ptr; break;
        }
    } else {
        result->data.int_val = 0;
    }
    
    return result;
}
"#.to_string()
}

fn generate_ptr_write_impl() -> String {
    r#"tauraro_value_t* tauraro_ptr_write(int argc, tauraro_value_t** args) {
    if (argc < 2 || argc > 3) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }
    
    void* ptr = (void*)(uintptr_t)args[0]->data.int_val;
    long long value = args[1]->data.int_val;
    int byte_size = (argc > 2) ? (int)args[2]->data.int_val : 8;
    
    if (ptr) {
        switch (byte_size) {
            case 1: *(int8_t*)ptr = (int8_t)value; break;
            case 2: *(int16_t*)ptr = (int16_t)value; break;
            case 4: *(int32_t*)ptr = (int32_t)value; break;
            case 8: *(int64_t*)ptr = (int64_t)value; break;
            default: *(int64_t*)ptr = (int64_t)value; break;
        }
    }
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_ptr_offset_impl() -> String {
    r#"tauraro_value_t* tauraro_ptr_offset(int argc, tauraro_value_t** args) {
    if (argc != 2) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = 0;
        return result;
    }
    
    uintptr_t ptr = (uintptr_t)args[0]->data.int_val;
    long long offset = args[1]->data.int_val;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = (long long)(ptr + offset);
    return result;
}
"#.to_string()
}

fn generate_null_ptr_impl() -> String {
    r#"tauraro_value_t* tauraro_null_ptr(int argc, tauraro_value_t** args) {
    (void)argc; (void)args;
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = 0;
    return result;
}
"#.to_string()
}

fn generate_is_null_impl() -> String {
    r#"tauraro_value_t* tauraro_is_null(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_BOOL;
        result->data.bool_val = 1;
        return result;
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    result->data.bool_val = (args[0]->data.int_val == 0) ? 1 : 0;
    return result;
}
"#.to_string()
}

// File I/O implementations
fn generate_open_impl() -> String {
    r#"tauraro_value_t* tauraro_open(int argc, tauraro_value_t** args) {
    if (argc < 1) {
        fprintf(stderr, "Error: open() requires at least 1 argument (filename)\n");
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    char* filename = args[0]->data.str_val;
    char* mode = (argc > 1) ? args[1]->data.str_val : "r";

    FILE* fp = fopen(filename, mode);
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        // TODO: Raise FileNotFoundError
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    // Create file object
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_OBJECT;

    tauraro_object_t* file_obj = malloc(sizeof(tauraro_object_t));
    file_obj->class_name = strdup("file");
    file_obj->attributes = malloc(sizeof(tauraro_dict_t));
    file_obj->attributes->size = 0;
    file_obj->attributes->capacity = 10;
    file_obj->attributes->keys = malloc(sizeof(char*) * 10);
    file_obj->attributes->values = malloc(sizeof(tauraro_value_t*) * 10);
    file_obj->methods = NULL;
    file_obj->parent = NULL;
    file_obj->native_ptr = fp;  // Store FILE* pointer

    // Store mode and filename as attributes
    tauraro_value_t* mode_val = tauraro_value_new();
    mode_val->type = TAURARO_STRING;
    mode_val->data.str_val = strdup(mode);
    tauraro_dict_set(file_obj->attributes, "mode", mode_val);

    tauraro_value_t* name_val = tauraro_value_new();
    name_val->type = TAURARO_STRING;
    name_val->data.str_val = strdup(filename);
    tauraro_dict_set(file_obj->attributes, "name", name_val);

    tauraro_value_t* closed_val = tauraro_value_new();
    closed_val->type = TAURARO_BOOL;
    closed_val->data.bool_val = false;
    tauraro_dict_set(file_obj->attributes, "closed", closed_val);

    result->data.obj_val = file_obj;
    return result;
}
"#.to_string()
}

fn generate_file_read_impl() -> String {
    r#"// file.read(size=-1) - Read entire file or N bytes
tauraro_value_t* tauraro_file_read(tauraro_value_t* file_val, int size) {
    if (file_val->type != TAURARO_OBJECT) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_STRING;
        result->data.str_val = strdup("");
        return result;
    }

    tauraro_object_t* file_obj = (tauraro_object_t*)file_val->data.obj_val;
    FILE* fp = (FILE*)file_obj->native_ptr;

    // Check if file is closed
    tauraro_value_t* closed_val = tauraro_dict_get(file_obj->attributes, "closed");
    if (closed_val && closed_val->data.bool_val) {
        fprintf(stderr, "Error: I/O operation on closed file\n");
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_STRING;
        result->data.str_val = strdup("");
        return result;
    }

    char* buffer;
    size_t bytes_read;

    if (size == -1) {
        // Read entire file
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        buffer = (char*)malloc(file_size + 1);
        bytes_read = fread(buffer, 1, file_size, fp);
        buffer[bytes_read] = '\0';
    } else {
        // Read N bytes
        buffer = (char*)malloc(size + 1);
        bytes_read = fread(buffer, 1, size, fp);
        buffer[bytes_read] = '\0';
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    result->data.str_val = buffer;
    return result;
}
"#.to_string()
}

fn generate_file_write_impl() -> String {
    r#"// file.write(data) - Write string to file
tauraro_value_t* tauraro_file_write(tauraro_value_t* file_val, tauraro_value_t* data_val) {
    if (file_val->type != TAURARO_OBJECT) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = 0;
        return result;
    }

    tauraro_object_t* file_obj = (tauraro_object_t*)file_val->data.obj_val;
    FILE* fp = (FILE*)file_obj->native_ptr;

    // Check if file is closed
    tauraro_value_t* closed_val = tauraro_dict_get(file_obj->attributes, "closed");
    if (closed_val && closed_val->data.bool_val) {
        fprintf(stderr, "Error: I/O operation on closed file\n");
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = 0;
        return result;
    }

    char* data = data_val->data.str_val;
    size_t bytes_written = fwrite(data, 1, strlen(data), fp);
    fflush(fp);  // Ensure data is written

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = bytes_written;
    return result;
}
"#.to_string()
}

fn generate_file_close_impl() -> String {
    r#"// file.close() - Close file
tauraro_value_t* tauraro_file_close(tauraro_value_t* file_val) {
    if (file_val->type != TAURARO_OBJECT) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_NONE;
        return result;
    }

    tauraro_object_t* file_obj = (tauraro_object_t*)file_val->data.obj_val;
    FILE* fp = (FILE*)file_obj->native_ptr;

    // Check if already closed
    tauraro_value_t* closed_val = tauraro_dict_get(file_obj->attributes, "closed");
    if (closed_val && !closed_val->data.bool_val) {
        fclose(fp);
        closed_val->data.bool_val = true;
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}
"#.to_string()
}

fn generate_file_readline_impl() -> String {
    r#"// file.readline() - Read single line
tauraro_value_t* tauraro_file_readline(tauraro_value_t* file_val) {
    if (file_val->type != TAURARO_OBJECT) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_STRING;
        result->data.str_val = strdup("");
        return result;
    }

    tauraro_object_t* file_obj = (tauraro_object_t*)file_val->data.obj_val;
    FILE* fp = (FILE*)file_obj->native_ptr;

    // Check if file is closed
    tauraro_value_t* closed_val = tauraro_dict_get(file_obj->attributes, "closed");
    if (closed_val && closed_val->data.bool_val) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_STRING;
        result->data.str_val = strdup("");
        return result;
    }

    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), fp)) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_STRING;
        result->data.str_val = strdup(buffer);
        return result;
    }

    // EOF reached
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    result->data.str_val = strdup("");
    return result;
}
"#.to_string()
}
