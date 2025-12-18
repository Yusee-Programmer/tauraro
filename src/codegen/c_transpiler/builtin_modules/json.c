// ==========================================
// JSON MODULE - Pure C Implementation
// ==========================================
// Provides: json.dumps(), json.loads() (simplified)
// Platform: Cross-platform

#ifndef TAURARO_JSON_MODULE_H
#define TAURARO_JSON_MODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Forward declarations
static char* tauraro_value_to_json_string(TauValue val);
static TauValue tauraro_json_parse_value(const char** json);

// json.dumps(obj) - Convert Tauraro value to JSON string
static inline TauValue tauraro_json_dumps(TauValue obj) {
    char* json_str = tauraro_value_to_json_string(obj);
    TauValue result = tauraro_string(json_str);
    return result;
}

// Helper: Convert TauValue to JSON string (recursive)
static char* tauraro_value_to_json_string(TauValue val) {
    char* result;
    char buffer[1024];

    switch (val.type) {
        case 0:  // Int
            snprintf(buffer, sizeof(buffer), "%lld", val.value.i);
            return strdup(buffer);

        case 1:  // Float
            snprintf(buffer, sizeof(buffer), "%.15g", val.value.f);
            return strdup(buffer);

        case 2:  // String
            {
                // Escape quotes and backslashes
                size_t len = strlen(val.value.s);
                char* escaped = malloc(len * 2 + 3);  // Worst case: every char needs escaping + quotes + null
                char* p = escaped;
                *p++ = '"';
                for (size_t i = 0; i < len; i++) {
                    if (val.value.s[i] == '"' || val.value.s[i] == '\\') {
                        *p++ = '\\';
                    }
                    *p++ = val.value.s[i];
                }
                *p++ = '"';
                *p = '\0';
                return escaped;
            }

        case 3:  // Bool
            return strdup(val.value.i ? "true" : "false");

        case 4:  // List
            {
                TauList* list = val.value.list;
                size_t total_size = 2;  // "[]"
                char** items = malloc(sizeof(char*) * list->size);

                // Convert each item
                for (size_t i = 0; i < list->size; i++) {
                    items[i] = tauraro_value_to_json_string(list->items[i]);
                    total_size += strlen(items[i]) + 2;  // item + ", "
                }

                // Build result
                result = malloc(total_size + 1);
                char* p = result;
                *p++ = '[';
                for (size_t i = 0; i < list->size; i++) {
                    if (i > 0) {
                        *p++ = ',';
                        *p++ = ' ';
                    }
                    strcpy(p, items[i]);
                    p += strlen(items[i]);
                    free(items[i]);
                }
                *p++ = ']';
                *p = '\0';
                free(items);
                return result;
            }

        case 5:  // Dict
            {
                TauDict* dict = val.value.dict;
                size_t total_size = 2;  // "{}"
                char** pairs = malloc(sizeof(char*) * dict->size);
                size_t count = 0;

                // Convert each key-value pair
                TauDictEntry* entry = dict->entries;
                while (entry != NULL && count < dict->size) {
                    char* key_str = tauraro_value_to_json_string(entry->key);
                    char* val_str = tauraro_value_to_json_string(entry->value);
                    size_t pair_size = strlen(key_str) + strlen(val_str) + 3;  // key:value
                    pairs[count] = malloc(pair_size + 1);
                    snprintf(pairs[count], pair_size + 1, "%s: %s", key_str, val_str);
                    total_size += strlen(pairs[count]) + 2;
                    free(key_str);
                    free(val_str);
                    entry = entry->next;
                    count++;
                }

                // Build result
                result = malloc(total_size + 1);
                char* p = result;
                *p++ = '{';
                for (size_t i = 0; i < count; i++) {
                    if (i > 0) {
                        *p++ = ',';
                        *p++ = ' ';
                    }
                    strcpy(p, pairs[i]);
                    p += strlen(pairs[i]);
                    free(pairs[i]);
                }
                *p++ = '}';
                *p = '\0';
                free(pairs);
                return result;
            }

        default:
            return strdup("null");
    }
}

// json.loads(json_str) - Parse JSON string to Tauraro value (simplified)
static inline TauValue tauraro_json_loads(TauValue json_str) {
    if (json_str.type != 2) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    const char* json = json_str.value.s;
    // Skip whitespace
    while (isspace(*json)) json++;

    return tauraro_json_parse_value(&json);
}

// Helper: Skip whitespace
static void skip_whitespace(const char** json) {
    while (isspace(**json)) (*json)++;
}

// Helper: Parse JSON value (recursive)
static TauValue tauraro_json_parse_value(const char** json) {
    skip_whitespace(json);

    // Parse null
    if (strncmp(*json, "null", 4) == 0) {
        *json += 4;
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    // Parse true
    if (strncmp(*json, "true", 4) == 0) {
        *json += 4;
        return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
    }

    // Parse false
    if (strncmp(*json, "false", 5) == 0) {
        *json += 5;
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    // Parse string
    if (**json == '"') {
        (*json)++;
        const char* start = *json;
        while (**json && **json != '"') {
            if (**json == '\\') (*json)++;  // Skip escaped char
            (*json)++;
        }
        size_t len = *json - start;
        char* str = malloc(len + 1);
        strncpy(str, start, len);
        str[len] = '\0';
        (*json)++;  // Skip closing quote
        TauValue __result = (TauValue){.type = 2, .value.s = str, .refcount = 1, .next = NULL}; return __result;
    }

    // Parse number
    if (isdigit(**json) || **json == '-') {
        char* end;
        if (strchr(*json, '.') || strchr(*json, 'e') || strchr(*json, 'E')) {
            double val = strtod(*json, &end);
            *json = end;
            return (TauValue){.type = 1, .value.f = val, .refcount = 1, .next = NULL};
        } else {
            long long val = strtoll(*json, &end, 10);
            *json = end;
            return (TauValue){.type = 0, .value.i = val, .refcount = 1, .next = NULL};
        }
    }

    // Parse array
    if (**json == '[') {
        (*json)++;
        TauList* list = malloc(sizeof(TauList));
        list->size = 0;
        list->capacity = 16;
        list->items = malloc(sizeof(TauValue) * list->capacity);

        skip_whitespace(json);
        while (**json && **json != ']') {
            if (list->size >= list->capacity) {
                list->capacity *= 2;
                list->items = realloc(list->items, sizeof(TauValue) * list->capacity);
            }
            list->items[list->size++] = tauraro_json_parse_value(json);
            skip_whitespace(json);
            if (**json == ',') {
                (*json)++;
                skip_whitespace(json);
            }
        }
        (*json)++;  // Skip ']'
        return (TauValue){.type = 4, .value.list = list, .refcount = 1, .next = NULL};
    }

    // Parse object
    if (**json == '{') {
        (*json)++;
        TauDict* dict = tauraro_dict_new();

        skip_whitespace(json);
        while (**json && **json != '}') {
            // Parse key (must be string)
            TauValue key = tauraro_json_parse_value(json);
            skip_whitespace(json);
            if (**json == ':') (*json)++;
            skip_whitespace(json);
            TauValue value = tauraro_json_parse_value(json);

            tauraro_dict_set(dict, key, value);

            skip_whitespace(json);
            if (**json == ',') {
                (*json)++;
                skip_whitespace(json);
            }
        }
        (*json)++;  // Skip '}'
        return (TauValue){.type = 5, .value.dict = dict, .refcount = 1, .next = NULL};
    }

    // Invalid JSON
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}


#endif // TAURARO_JSON_MODULE_H
