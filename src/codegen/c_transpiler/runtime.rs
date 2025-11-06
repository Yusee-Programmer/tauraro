//! Runtime Support for C Transpilation
//!
//! This module provides runtime support functions that are needed
//! for various operations in the generated C code.

/// Generate runtime support functions (operators only, builtins are in builtins.rs)
pub fn generate_runtime_support() -> String {
    r#"// Runtime support functions (operators only)

// Helper function to check if a value is truthy (for control flow)
int tauraro_is_truthy(tauraro_value_t* value) {
    if (value == NULL) return 0;

    switch (value->type) {
        case TAURARO_BOOL:
            return value->data.bool_val ? 1 : 0;
        case TAURARO_INT:
            return value->data.int_val != 0 ? 1 : 0;
        case TAURARO_FLOAT:
            return value->data.float_val != 0.0 ? 1 : 0;
        case TAURARO_STRING:
            return (value->data.str_val != NULL && value->data.str_val[0] != '\0') ? 1 : 0;
        case TAURARO_NONE:
            return 0;
        case TAURARO_LIST:
            return (value->data.list_val != NULL && value->data.list_val->size > 0) ? 1 : 0;
        case TAURARO_DICT:
            return (value->data.dict_val != NULL) ? 1 : 0;
        default:
            return 1; // Most objects are truthy
    }
}

// Arithmetic operators
tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->type = TAURARO_INT;
        result->data.int_val = left->data.int_val + right->data.int_val;
    } else if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
               (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        result->type = TAURARO_FLOAT;
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        result->data.float_val = l + r;
    } else if (left->type == TAURARO_STRING && right->type == TAURARO_STRING) {
        result->type = TAURARO_STRING;
        size_t len1 = strlen(left->data.str_val);
        size_t len2 = strlen(right->data.str_val);
        result->data.str_val = malloc(len1 + len2 + 1);
        strcpy(result->data.str_val, left->data.str_val);
        strcat(result->data.str_val, right->data.str_val);
    } else {
        result->type = TAURARO_NONE;
    }

    return result;
}

tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->type = TAURARO_INT;
        result->data.int_val = left->data.int_val - right->data.int_val;
    } else if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
               (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        result->type = TAURARO_FLOAT;
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        result->data.float_val = l - r;
    } else {
        result->type = TAURARO_NONE;
    }

    return result;
}

tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->type = TAURARO_INT;
        result->data.int_val = left->data.int_val * right->data.int_val;
    } else if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
               (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        result->type = TAURARO_FLOAT;
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        result->data.float_val = l * r;
    } else {
        result->type = TAURARO_NONE;
    }

    return result;
}

tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();

    if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
        (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        result->type = TAURARO_FLOAT;
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        if (r != 0.0) {
            result->data.float_val = l / r;
        } else {
            result->data.float_val = 0.0; // Handle division by zero
        }
    } else {
        result->type = TAURARO_NONE;
    }

    return result;
}

tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->type = TAURARO_INT;
        if (right->data.int_val != 0) {
            result->data.int_val = left->data.int_val % right->data.int_val;
        } else {
            result->data.int_val = 0; // Handle modulo by zero
        }
    } else {
        result->type = TAURARO_NONE;
    }

    return result;
}

// Comparison operators
tauraro_value_t* tauraro_eq(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;

    if (left->type != right->type) {
        result->data.bool_val = false;
    } else {
        switch (left->type) {
            case TAURARO_INT:
                result->data.bool_val = left->data.int_val == right->data.int_val;
                break;
            case TAURARO_FLOAT:
                result->data.bool_val = left->data.float_val == right->data.float_val;
                break;
            case TAURARO_BOOL:
                result->data.bool_val = left->data.bool_val == right->data.bool_val;
                break;
            case TAURARO_STRING:
                result->data.bool_val = strcmp(left->data.str_val, right->data.str_val) == 0;
                break;
            case TAURARO_NONE:
                result->data.bool_val = true;
                break;
            default:
                result->data.bool_val = false;
                break;
        }
    }

    return result;
}

tauraro_value_t* tauraro_ne(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* eq_result = tauraro_eq(left, right);
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;
    result->data.bool_val = !eq_result->data.bool_val;
    return result;
}

tauraro_value_t* tauraro_lt(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->data.bool_val = left->data.int_val < right->data.int_val;
    } else if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
               (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        result->data.bool_val = l < r;
    } else if (left->type == TAURARO_STRING && right->type == TAURARO_STRING) {
        result->data.bool_val = strcmp(left->data.str_val, right->data.str_val) < 0;
    } else {
        result->data.bool_val = false;
    }

    return result;
}

tauraro_value_t* tauraro_le(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->data.bool_val = left->data.int_val <= right->data.int_val;
    } else if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
               (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        result->data.bool_val = l <= r;
    } else if (left->type == TAURARO_STRING && right->type == TAURARO_STRING) {
        result->data.bool_val = strcmp(left->data.str_val, right->data.str_val) <= 0;
    } else {
        result->data.bool_val = false;
    }

    return result;
}

tauraro_value_t* tauraro_gt(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->data.bool_val = left->data.int_val > right->data.int_val;
    } else if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
               (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        result->data.bool_val = l > r;
    } else if (left->type == TAURARO_STRING && right->type == TAURARO_STRING) {
        result->data.bool_val = strcmp(left->data.str_val, right->data.str_val) > 0;
    } else {
        result->data.bool_val = false;
    }

    return result;
}

tauraro_value_t* tauraro_ge(tauraro_value_t* left, tauraro_value_t* right) {
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_BOOL;

    if (left->type == TAURARO_INT && right->type == TAURARO_INT) {
        result->data.bool_val = left->data.int_val >= right->data.int_val;
    } else if ((left->type == TAURARO_INT || left->type == TAURARO_FLOAT) &&
               (right->type == TAURARO_INT || right->type == TAURARO_FLOAT)) {
        double l = (left->type == TAURARO_INT) ? (double)left->data.int_val : left->data.float_val;
        double r = (right->type == TAURARO_INT) ? (double)right->data.int_val : right->data.float_val;
        result->data.bool_val = l >= r;
    } else if (left->type == TAURARO_STRING && right->type == TAURARO_STRING) {
        result->data.bool_val = strcmp(left->data.str_val, right->data.str_val) >= 0;
    } else {
        result->data.bool_val = false;
    }

    return result;
}

"#.to_string()
}