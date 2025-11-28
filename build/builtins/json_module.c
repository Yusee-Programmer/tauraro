// Tauraro JSON Module Implementation
// Auto-generated C implementation for the json built-in module

#include "json_module.h"

static int json_initialized = 0;

void tauraro_json_module_init(void) {
    if (json_initialized) return;
    json_initialized = 1;
}

// Simple JSON string builder (basic implementation)
static void append_char(char** buf, int* len, int* cap, char c) {
    if (*len >= *cap - 1) {
        *cap *= 2;
        *buf = (char*)realloc(*buf, *cap);
    }
    (*buf)[(*len)++] = c;
    (*buf)[*len] = '\0';
}

static void append_str(char** buf, int* len, int* cap, const char* s) {
    while (*s) append_char(buf, len, cap, *s++);
}

// Serialize value to JSON string
static void value_to_json(tauraro_value_t* val, char** buf, int* len, int* cap) {
    if (val == NULL) {
        append_str(buf, len, cap, "null");
        return;
    }
    
    switch (val->type) {
        case 0:  // INT
            {
                char num[32];
                snprintf(num, sizeof(num), "%lld", val->data.int_val);
                append_str(buf, len, cap, num);
            }
            break;
        case 1:  // FLOAT
            {
                char num[64];
                snprintf(num, sizeof(num), "%g", val->data.float_val);
                append_str(buf, len, cap, num);
            }
            break;
        case 2:  // BOOL
            append_str(buf, len, cap, val->data.bool_val ? "true" : "false");
            break;
        case 3:  // STRING
            append_char(buf, len, cap, '"');
            if (val->data.str_val) {
                const char* s = val->data.str_val;
                while (*s) {
                    if (*s == '"' || *s == '\\') append_char(buf, len, cap, '\\');
                    append_char(buf, len, cap, *s++);
                }
            }
            append_char(buf, len, cap, '"');
            break;
        case 8:  // NONE
            append_str(buf, len, cap, "null");
            break;
        default:
            append_str(buf, len, cap, "null");
            break;
    }
}

tauraro_value_t* tauraro_json_dumps(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) {
        return tauraro_string("null");
    }
    
    int cap = 256;
    int len = 0;
    char* buf = (char*)malloc(cap);
    buf[0] = '\0';
    
    value_to_json(argv[0], &buf, &len, &cap);
    
    tauraro_value_t* result = tauraro_string(buf);
    free(buf);
    return result;
}

tauraro_value_t* tauraro_json_loads(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    if (argv[0]->type != 3) return tauraro_none();  // Must be string
    
    const char* json = argv[0]->data.str_val;
    if (json == NULL) return tauraro_none();
    
    // Skip whitespace
    while (*json == ' ' || *json == '\t' || *json == '\n' || *json == '\r') json++;
    
    // Parse based on first character
    if (*json == 'n' && strncmp(json, "null", 4) == 0) {
        return tauraro_none();
    }
    if (*json == 't' && strncmp(json, "true", 4) == 0) {
        return tauraro_bool(1);
    }
    if (*json == 'f' && strncmp(json, "false", 5) == 0) {
        return tauraro_bool(0);
    }
    if (*json == '"') {
        // Parse string
        json++;
        char* end = strchr(json, '"');
        if (end) {
            int len = end - json;
            char* str = (char*)malloc(len + 1);
            strncpy(str, json, len);
            str[len] = '\0';
            tauraro_value_t* result = tauraro_string(str);
            free(str);
            return result;
        }
        return tauraro_none();
    }
    if (*json == '-' || (*json >= '0' && *json <= '9')) {
        // Parse number
        char* endptr;
        double val = strtod(json, &endptr);
        if (strchr(json, '.') || strchr(json, 'e') || strchr(json, 'E')) {
            return tauraro_float(val);
        }
        return tauraro_int((long long)val);
    }
    
    return tauraro_none();
}

tauraro_value_t* tauraro_json_get_attr(const char* name) {
    (void)name;
    return tauraro_none();
}
