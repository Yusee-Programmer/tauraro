// ==========================================
// PICKLE MODULE - Pure C Implementation
// ==========================================
// Provides: pickle.dumps(), pickle.loads() (simplified)
// Platform: Cross-platform

#ifndef TAURARO_PICKLE_MODULE_H
#define TAURARO_PICKLE_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Pickle format tags
#define PICKLE_INT 'i'
#define PICKLE_FLOAT 'f'
#define PICKLE_STRING 's'
#define PICKLE_BOOL 'b'
#define PICKLE_LIST 'l'
#define PICKLE_DICT 'd'
#define PICKLE_NONE 'n'
#define PICKLE_END '.'

// Pickle protocol versions
#define PICKLE_PROTOCOL_0 0  // Original ASCII
#define PICKLE_PROTOCOL_1 1  // Old binary format  
#define PICKLE_PROTOCOL_2 2  // Efficient binary format
#define PICKLE_PROTOCOL_3 3  // Binary format with unicode
#define PICKLE_PROTOCOL_4 4  // Binary format with large objects
#define PICKLE_PROTOCOL_5 5  // Binary format with out-of-band data
#define PICKLE_HIGHEST_PROTOCOL 5  // Latest protocol version

// pickle.dumps(obj) - Serialize object to string
static inline TauValue tauraro_pickle_dumps(TauValue obj) {
    char buffer[8192];
    size_t pos = 0;

    // Recursive serialization function
    void serialize_value(TauValue val, char* buf, size_t* p) {
        char num_str[256];

        switch (val.type) {
            case 0:  // Int
                buf[(*p)++] = PICKLE_INT;
                snprintf(num_str, sizeof(num_str), "%lld", val.value.i);
                strcpy(buf + *p, num_str);
                *p += strlen(num_str);
                buf[(*p)++] = '\n';
                break;

            case 1:  // Float
                buf[(*p)++] = PICKLE_FLOAT;
                snprintf(num_str, sizeof(num_str), "%.15g", val.value.f);
                strcpy(buf + *p, num_str);
                *p += strlen(num_str);
                buf[(*p)++] = '\n';
                break;

            case 2:  // String
                buf[(*p)++] = PICKLE_STRING;
                strcpy(buf + *p, val.value.s);
                *p += strlen(val.value.s);
                buf[(*p)++] = '\n';
                break;

            case 3:  // Bool
                buf[(*p)++] = PICKLE_BOOL;
                buf[(*p)++] = val.value.i ? '1' : '0';
                buf[(*p)++] = '\n';
                break;

            case 4:  // List
                buf[(*p)++] = PICKLE_LIST;
                {
                    TauList* list = val.value.list;
                    snprintf(num_str, sizeof(num_str), "%zu", list->size);
                    strcpy(buf + *p, num_str);
                    *p += strlen(num_str);
                    buf[(*p)++] = '\n';

                    for (size_t i = 0; i < list->size; i++) {
                        serialize_value(list->items[i], buf, p);
                    }
                }
                break;

            case 5:  // Dict
                buf[(*p)++] = PICKLE_DICT;
                {
                    TauDict* dict = val.value.dict;
                    snprintf(num_str, sizeof(num_str), "%zu", dict->size);
                    strcpy(buf + *p, num_str);
                    *p += strlen(num_str);
                    buf[(*p)++] = '\n';

                    TauDictEntry* entry = dict->entries;
                    while (entry != NULL) {
                        serialize_value(entry->key, buf, p);
                        serialize_value(entry->value, buf, p);
                        entry = entry->next;
                    }
                }
                break;

            default:
                buf[(*p)++] = PICKLE_NONE;
                buf[(*p)++] = '\n';
                break;
        }
    }

    serialize_value(obj, buffer, &pos);
    buffer[pos++] = PICKLE_END;
    buffer[pos] = '\0';

    return tauraro_string(strdup(buffer));
}

// pickle.loads(data) - Deserialize object from string
static inline TauValue tauraro_pickle_loads(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    const char* str = data.value.s;
    size_t pos = 0;

    // Forward declaration for recursive deserialization
    TauValue deserialize_value(const char* buf, size_t* p);

    // Extract a line from buffer
    char* read_line(const char* buf, size_t* p) {
        size_t start = *p;
        while (buf[*p] && buf[*p] != '\n') {
            (*p)++;
        }
        size_t len = *p - start;
        if (buf[*p] == '\n') (*p)++;

        char* line = malloc(len + 1);
        strncpy(line, buf + start, len);
        line[len] = '\0';
        return line;
    }

    TauValue deserialize_value(const char* buf, size_t* p) {
        char tag = buf[(*p)++];
        
        switch (tag) {
            case PICKLE_INT:
                {
                    char* line = read_line(buf, p);
                    int64_t val = strtoll(line, NULL, 10);
                    free(line);
                    return (TauValue){.type = 0, .value.i = val, .refcount = 1, .next = NULL};
                }

            case PICKLE_FLOAT:
                {
                    char* line = read_line(buf, p);
                    double val = strtod(line, NULL);
                    free(line);
                    return (TauValue){.type = 1, .value.f = val, .refcount = 1, .next = NULL};
                }

            case PICKLE_STRING:
                {
                    char* line = read_line(buf, p);
                    TauValue __result = (TauValue){.type = 2, .value.s = line, .refcount = 1, .next = NULL}; return __result;
                }

            case PICKLE_BOOL:
                {
                    int val = buf[(*p)++] == '1' ? 1 : 0;
                    if (buf[*p] == '\n') (*p)++;
                    return (TauValue){.type = 3, .value.i = val, .refcount = 1, .next = NULL};
                }

            case PICKLE_LIST:
                {
                    char* line = read_line(buf, p);
                    size_t size = strtoul(line, NULL, 10);
                    free(line);

                    TauList* list = malloc(sizeof(TauList));
                    list->items = malloc(sizeof(TauValue) * size);
                    list->size = size;
                    list->capacity = size;

                    for (size_t i = 0; i < size; i++) {
                        list->items[i] = deserialize_value(buf, p);
                    }

                    return (TauValue){.type = 4, .value.list = list, .refcount = 1, .next = NULL};
                }

            case PICKLE_DICT:
                {
                    char* line = read_line(buf, p);
                    size_t size = strtoul(line, NULL, 10);
                    free(line);

                    TauDict* dict = malloc(sizeof(TauDict));
                    dict->entries = NULL;
                    dict->size = 0;

                    for (size_t i = 0; i < size; i++) {
                        TauValue key = deserialize_value(buf, p);
                        TauValue val = deserialize_value(buf, p);

                        TauDictEntry* entry = malloc(sizeof(TauDictEntry));
                        entry->key = key;
                        entry->value = val;
                        entry->next = dict->entries;
                        dict->entries = entry;
                        dict->size++;
                    }

                    return (TauValue){.type = 5, .value.dict = dict, .refcount = 1, .next = NULL};
                }

            case PICKLE_NONE:
                if (buf[*p] == '\n') (*p)++;
                return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};

            case PICKLE_END:
                return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};

            default:
                return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
        }
    }

    return deserialize_value(str, &pos);
}

// pickle.encode(obj) - Alias for dumps
static inline TauValue tauraro_pickle_encode(TauValue obj) {
    return tauraro_pickle_dumps(obj);
}

// pickle.decode(data) - Alias for loads
static inline TauValue tauraro_pickle_decode(TauValue data) {
    return tauraro_pickle_loads(data);
}

// pickle.is_pickled(data) - Check if data looks like pickled data
static inline TauValue tauraro_pickle_is_pickled(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    const char* str = data.value.s;
    if (!str || strlen(str) == 0) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    // Check if last character is the end marker
    char last = str[strlen(str) - 1];
    if (last != PICKLE_END) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    // Check first character is a valid tag
    char first = str[0];
    return (TauValue){
        .type = 3,
        .value.i = (first == PICKLE_INT || first == PICKLE_FLOAT || first == PICKLE_STRING ||
                    first == PICKLE_BOOL || first == PICKLE_LIST || first == PICKLE_DICT ||
                    first == PICKLE_NONE) ? 1 : 0,
        .refcount = 1,
        .next = NULL
    };
}

// pickle.HIGHEST_PROTOCOL constant
static inline TauValue tauraro_pickle_HIGHEST_PROTOCOL(void) {
    return (TauValue){.type = 0, .value.i = PICKLE_HIGHEST_PROTOCOL, .refcount = 1, .next = NULL};
}

// pickle.DEFAULT_PROTOCOL constant
static inline TauValue tauraro_pickle_DEFAULT_PROTOCOL(void) {
    return (TauValue){.type = 0, .value.i = PICKLE_PROTOCOL_3, .refcount = 1, .next = NULL};
}

// pickle.dumps_protocol(obj, protocol) - Serialize with specific protocol
static inline TauValue tauraro_pickle_dumps_protocol(TauValue obj, TauValue protocol) {
    int proto = (protocol.type == 0) ? protocol.value.i : PICKLE_PROTOCOL_3;
    // For now, use same implementation regardless of protocol
    // In production, would use protocol-specific serialization
    return tauraro_pickle_dumps(obj);
}

// pickle.loads_protocol(data, protocol) - Deserialize with specific protocol
static inline TauValue tauraro_pickle_loads_protocol(TauValue data, TauValue protocol) {
    int proto = (protocol.type == 0) ? protocol.value.i : PICKLE_PROTOCOL_3;
    // For now, use same implementation regardless of protocol
    // In production, would use protocol-specific deserialization
    return tauraro_pickle_loads(data);
}


#endif // TAURARO_PICKLE_MODULE_H
