//! Runtime Support for C Transpilation
//!
//! This module provides runtime support functions that are needed
//! for various operations in the generated C code.

/// Generate runtime support functions
pub fn generate_runtime_support() -> String {
    r#"// Runtime support functions

tauraro_value_t* tauraro_super_call(int argc, tauraro_value_t** args) {
    // Simplified super() implementation
    // In a full implementation, this would need to access the current class context
    // For now, we'll return a simple object
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_OBJECT;
    result->data.obj_val = malloc(sizeof(tauraro_object_t));
    result->data.obj_val->class_name = strdup("super");
    return result;
}

tauraro_value_t* tauraro_isinstance(int argc, tauraro_value_t** args) {
    if (argc < 2) {
        // Error handling
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_BOOL;
        result->data.bool_val = false;
        return result;
    }
    
    tauraro_value_t* obj = args[0];
    tauraro_value_t* cls = args[1];
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    
    if (obj->type != TAURARO_OBJECT) {
        result->data.bool_val = false;
        return result;
    }
    
    if (cls->type != TAURARO_STRING) {
        result->data.bool_val = false;
        return result;
    }
    
    // Use the proper isinstance check that considers inheritance
    result->data.bool_val = tauraro_isinstance_check(obj, cls->data.str_val);
    return result;
}

tauraro_value_t* tauraro_print(int argc, tauraro_value_t** args) {
    for (int i = 0; i < argc; i++) {
        tauraro_value_t* arg = args[i];
        switch (arg->type) {
            case TAURARO_INT:
                printf("%lld", arg->data.int_val);
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
            default:
                printf("<object>");
                break;
        }
        if (i < argc - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_NONE;
    return result;
}

tauraro_value_t* tauraro_len(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        // Error handling
        return NULL;
    }
    
    tauraro_value_t* arg = args[0];
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    
    switch (arg->type) {
        case TAURARO_STRING:
            result->data.int_val = strlen(arg->data.str_val);
            break;
        case TAURARO_LIST:
            result->data.int_val = arg->data.list_val->size;
            break;
        case TAURARO_DICT:
            result->data.int_val = arg->data.dict_val->size;
            break;
        case TAURARO_TUPLE:
            result->data.int_val = arg->data.tuple_val->size;
            break;
        default:
            result->data.int_val = 0;
            break;
    }
    
    return result;
}

tauraro_value_t* tauraro_str(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        // Error handling
        return NULL;
    }
    
    tauraro_value_t* arg = args[0];
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_STRING;
    
    switch (arg->type) {
        case TAURARO_INT: {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%lld", arg->data.int_val);
            result->data.str_val = strdup(buffer);
            break;
        }
        case TAURARO_FLOAT: {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%g", arg->data.float_val);
            result->data.str_val = strdup(buffer);
            break;
        }
        case TAURARO_BOOL:
            result->data.str_val = strdup(arg->data.bool_val ? "True" : "False");
            break;
        case TAURARO_STRING:
            result->data.str_val = strdup(arg->data.str_val);
            break;
        case TAURARO_NONE:
            result->data.str_val = strdup("None");
            break;
        default:
            result->data.str_val = strdup("<object>");
            break;
    }
    
    return result;
}

tauraro_value_t* tauraro_int(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        // Error handling
        return NULL;
    }
    
    tauraro_value_t* arg = args[0];
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    
    switch (arg->type) {
        case TAURARO_INT:
            result->data.int_val = arg->data.int_val;
            break;
        case TAURARO_FLOAT:
            result->data.int_val = (int64_t)arg->data.float_val;
            break;
        case TAURARO_BOOL:
            result->data.int_val = arg->data.bool_val ? 1 : 0;
            break;
        case TAURARO_STRING: {
            result->data.int_val = strtoll(arg->data.str_val, NULL, 10);
            break;
        }
        default:
            result->data.int_val = 0;
            break;
    }
    
    return result;
}

tauraro_value_t* tauraro_float(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        // Error handling
        return NULL;
    }
    
    tauraro_value_t* arg = args[0];
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_FLOAT;
    
    switch (arg->type) {
        case TAURARO_INT:
            result->data.float_val = (double)arg->data.int_val;
            break;
        case TAURARO_FLOAT:
            result->data.float_val = arg->data.float_val;
            break;
        case TAURARO_BOOL:
            result->data.float_val = arg->data.bool_val ? 1.0 : 0.0;
            break;
        case TAURARO_STRING: {
            result->data.float_val = strtod(arg->data.str_val, NULL);
            break;
        }
        default:
            result->data.float_val = 0.0;
            break;
    }
    
    return result;
}

tauraro_value_t* tauraro_bool(int argc, tauraro_value_t** args) {
    if (argc != 1) {
        // Error handling
        return NULL;
    }
    
    tauraro_value_t* arg = args[0];
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    
    switch (arg->type) {
        case TAURARO_INT:
            result->data.bool_val = arg->data.int_val != 0;
            break;
        case TAURARO_FLOAT:
            result->data.bool_val = arg->data.float_val != 0.0;
            break;
        case TAURARO_BOOL:
            result->data.bool_val = arg->data.bool_val;
            break;
        case TAURARO_STRING:
            result->data.bool_val = strlen(arg->data.str_val) > 0;
            break;
        case TAURARO_NONE:
            result->data.bool_val = false;
            break;
        default:
            result->data.bool_val = true;
            break;
    }
    
    return result;
}

tauraro_value_t* tauraro_list(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_LIST;
    result->data.list_val = malloc(sizeof(tauraro_list_t));
    result->data.list_val->size = 0;
    result->data.list_val->capacity = 10;
    result->data.list_val->items = malloc(sizeof(tauraro_value_t*) * result->data.list_val->capacity);
    
    // Add initial items if provided
    for (int i = 0; i < argc; i++) {
        if (result->data.list_val->size >= result->data.list_val->capacity) {
            result->data.list_val->capacity *= 2;
            result->data.list_val->items = realloc(result->data.list_val->items, 
                sizeof(tauraro_value_t*) * result->data.list_val->capacity);
        }
        result->data.list_val->items[result->data.list_val->size] = args[i];
        tauraro_incref(args[i]);
        result->data.list_val->size++;
    }
    
    return result;
}

tauraro_value_t* tauraro_dict(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_DICT;
    result->data.dict_val = malloc(sizeof(tauraro_dict_t));
    result->data.dict_val->size = 0;
    result->data.dict_val->capacity = 10;
    result->data.dict_val->keys = malloc(sizeof(char*) * result->data.dict_val->capacity);
    result->data.dict_val->values = malloc(sizeof(tauraro_value_t*) * result->data.dict_val->capacity);
    return result;
}

tauraro_value_t* tauraro_tuple(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_TUPLE;
    result->data.tuple_val = malloc(sizeof(tauraro_tuple_t));
    result->data.tuple_val->size = 0;
    result->data.tuple_val->capacity = 10;
    result->data.tuple_val->items = malloc(sizeof(tauraro_value_t*) * result->data.tuple_val->capacity);
    
    // Add initial items if provided
    for (int i = 0; i < argc; i++) {
        if (result->data.tuple_val->size >= result->data.tuple_val->capacity) {
            result->data.tuple_val->capacity *= 2;
            result->data.tuple_val->items = realloc(result->data.tuple_val->items, 
                sizeof(tauraro_value_t*) * result->data.tuple_val->capacity);
        }
        result->data.tuple_val->items[result->data.tuple_val->size] = args[i];
        tauraro_incref(args[i]);
        result->data.tuple_val->size++;
    }
    
    return result;
}

tauraro_value_t* tauraro_set(int argc, tauraro_value_t** args) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_SET;
    result->data.set_val = malloc(sizeof(tauraro_set_t));
    result->data.set_val->size = 0;
    result->data.set_val->capacity = 10;
    result->data.set_val->items = malloc(sizeof(tauraro_value_t*) * result->data.set_val->capacity);
    
    // Add initial items if provided
    for (int i = 0; i < argc; i++) {
        if (result->data.set_val->size >= result->data.set_val->capacity) {
            result->data.set_val->capacity *= 2;
            result->data.set_val->items = realloc(result->data.set_val->items, 
                sizeof(tauraro_value_t*) * result->data.set_val->capacity);
        }
        result->data.set_val->items[result->data.set_val->size] = args[i];
        tauraro_incref(args[i]);
        result->data.set_val->size++;
    }
    
    return result;
}

"#.to_string()
}