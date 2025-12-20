// ==========================================
// TAURARO RUNTIME HEADER
// ==========================================
// Common type definitions and helper functions for all Tauraro builtin modules
// This header provides the core runtime support needed by C transpiled code

#ifndef TAURARO_RUNTIME_H
#define TAURARO_RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// ==========================================
// FORWARD DECLARATIONS
// ==========================================
typedef struct TauValue TauValue;
typedef struct TauList TauList;
typedef struct TauDict TauDict;
typedef struct TauDictEntry TauDictEntry;
typedef struct TauObject TauObject;
typedef struct TauClass TauClass;
typedef struct TauFunction TauFunction;
typedef struct TauClosure TauClosure;
typedef struct TauIterator TauIterator;
typedef struct TauException TauException;
typedef struct TauModule TauModule;
typedef struct TauMethod TauMethod;

// ==========================================
// TYPE DEFINITIONS
// ==========================================

// Generic value type
struct TauValue {
    int type; // 0=int, 1=float, 2=string, 3=bool, 4=list, 5=dict, 6=object, 7=function, 8=exception, 9=none
    union {
        long long i;
        double f;
        char* s;
        TauList* list;
        TauDict* dict;
        TauObject* obj;
        TauFunction* func;
        TauException* exc;
        void* ptr;
    } value;
    int refcount;  // Reference counting for GC
    struct TauValue* next; // For GC linked list
};

// Dynamic list type
struct TauList {
    TauValue* items;
    size_t size;
    size_t capacity;
    int refcount;
};

// Hash table dictionary entry
struct TauDictEntry {
    char* key;
    TauValue value;
    struct TauDictEntry* next; // For collision chaining
};

// Hash table dictionary type
struct TauDict {
    TauDictEntry** buckets;
    size_t size;
    size_t capacity;
    int refcount;
};

// Class definition with inheritance support
struct TauClass {
    char* name;
    struct TauClass* parent; // For inheritance
    TauDict* methods; // Method table
    TauDict* static_methods;
    TauDict* properties;
    size_t instance_size;
    int refcount;
};

// Object instance with class support
struct TauObject {
    TauClass* class_ref; // Reference to class definition
    char* class_name; // For compatibility
    TauDict* attributes; // Instance variables
    void* native_data; // For native extensions
    int refcount;
};

// Function type with closure support
typedef TauValue (*TauNativeFunc)(int argc, TauValue* argv);

struct TauFunction {
    char* name;
    TauNativeFunc func_ptr;
    TauDict* closure;  // Captured variables
    int refcount;
};

// Closure type
struct TauClosure {
    char* name;
    TauValue* params;
    size_t param_count;
    TauDict* captured_scope;
    int refcount;
};

// Iterator type
struct TauIterator {
    TauValue target;
    size_t index;
    int refcount;
};

// Exception type
struct TauException {
    char* type;
    char* message;
    char* traceback;
    int refcount;
};

// Module type
struct TauModule {
    char* name;
    TauDict* globals;
    int refcount;
};

// Method type
struct TauMethod {
    char* name;
    TauNativeFunc func_ptr;
    TauObject* self;
    int refcount;
};

// ==========================================
// HELPER FUNCTIONS - DECLARATIONS
// ==========================================

// Value creation helpers
static inline TauValue tauraro_none(void);
static inline TauValue tauraro_int(long long i);
static inline TauValue tauraro_float(double f);
static inline TauValue tauraro_bool(int b);
static inline TauValue tauraro_string(char* s);

// List helpers
static inline TauList* tauraro_list_new(void);
static inline TauList* tauraro_create_list(void);
static inline void tauraro_list_append(TauList* list, TauValue item);
static inline TauValue tauraro_list_get(TauList* list, size_t index);
static inline void tauraro_list_set(TauList* list, size_t index, TauValue value);
static inline void tauraro_list_free(TauList* list);

// Dict helpers
static inline TauDict* tauraro_dict_new(void);
static inline TauDict* tauraro_create_dict(void);
static inline void tauraro_dict_set(TauDict* dict, const char* key, TauValue value);
static inline TauValue tauraro_dict_get(TauDict* dict, const char* key);
static inline int tauraro_dict_has_key(TauDict* dict, const char* key);
static inline void tauraro_dict_free(TauDict* dict);

// String helpers
static inline char* tauraro_string_concat(const char* s1, const char* s2);
static inline int tauraro_string_equals(const char* s1, const char* s2);
static inline char* tauraro_string_copy(const char* s);

// Type conversion helpers
static inline double tau_to_double(TauValue v);
static inline int64_t tau_to_int64(TauValue v);
static inline char* tau_to_string(TauValue v);

// ==========================================
// HELPER FUNCTIONS - IMPLEMENTATIONS
// ==========================================

// Value creation helpers
static inline TauValue tauraro_none(void) {
    return (TauValue){.type = 9, .value.i = 0, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_int(long long i) {
    return (TauValue){.type = 0, .value.i = i, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_float(double f) {
    return (TauValue){.type = 1, .value.f = f, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_bool(int b) {
    return (TauValue){.type = 3, .value.i = b ? 1 : 0, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_string(char* s) {
    return (TauValue){.type = 2, .value.s = s, .refcount = 1, .next = NULL};
}

// Type conversion helpers
static inline double tau_to_double(TauValue v) {
    if (v.type == 1) return v.value.f;      // Float
    if (v.type == 0) return (double)v.value.i;  // Int
    return 0.0;
}

static inline int64_t tau_to_int64(TauValue v) {
    if (v.type == 0) return v.value.i;      // Int
    if (v.type == 1) return (int64_t)v.value.f;  // Float
    return 0;
}

static inline char* tau_to_string(TauValue v) {
    if (v.type == 2) return v.value.s;      // String
    
    // For other types, convert to string
    char* buffer = (char*)malloc(256);
    switch (v.type) {
        case 0:  // Int
            snprintf(buffer, 256, "%lld", v.value.i);
            break;
        case 1:  // Float
            snprintf(buffer, 256, "%.15g", v.value.f);
            break;
        case 3:  // Bool
            strcpy(buffer, v.value.i ? "True" : "False");
            break;
        case 9:  // None
            strcpy(buffer, "None");
            break;
        default:
            strcpy(buffer, "<object>");
            break;
    }
    return buffer;
}

// List helpers
static inline TauList* tauraro_list_new(void) {
    TauList* list = (TauList*)malloc(sizeof(TauList));
    list->size = 0;
    list->capacity = 16;
    list->items = (TauValue*)malloc(sizeof(TauValue) * list->capacity);
    list->refcount = 1;
    return list;
}

static inline TauList* tauraro_create_list(void) {
    return tauraro_list_new();
}

static inline void tauraro_list_append(TauList* list, TauValue item) {
    if (!list) return;
    
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->items = (TauValue*)realloc(list->items, sizeof(TauValue) * list->capacity);
    }
    list->items[list->size++] = item;
}

static inline TauValue tauraro_list_get(TauList* list, size_t index) {
    if (!list || index >= list->size) {
        return tauraro_none();
    }
    return list->items[index];
}

static inline void tauraro_list_set(TauList* list, size_t index, TauValue value) {
    if (!list || index >= list->size) return;
    list->items[index] = value;
}

static inline void tauraro_list_free(TauList* list) {
    if (!list) return;
    if (list->items) free(list->items);
    free(list);
}

// Simple hash function
static inline size_t tauraro_hash(const char* key) {
    size_t hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// Dict helpers
static inline TauDict* tauraro_dict_new(void) {
    TauDict* dict = (TauDict*)malloc(sizeof(TauDict));
    dict->size = 0;
    dict->capacity = 16;
    dict->buckets = (TauDictEntry**)calloc(dict->capacity, sizeof(TauDictEntry*));
    dict->refcount = 1;
    return dict;
}

static inline TauDict* tauraro_create_dict(void) {
    return tauraro_dict_new();
}

static inline void tauraro_dict_set(TauDict* dict, const char* key, TauValue value) {
    if (!dict || !key) return;
    
    size_t index = tauraro_hash(key) % dict->capacity;
    TauDictEntry* entry = dict->buckets[index];
    
    // Check if key exists
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    
    // Create new entry
    TauDictEntry* new_entry = (TauDictEntry*)malloc(sizeof(TauDictEntry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = dict->buckets[index];
    dict->buckets[index] = new_entry;
    dict->size++;
    
    // Resize if load factor > 0.75
    if (dict->size > dict->capacity * 3 / 4) {
        size_t old_capacity = dict->capacity;
        TauDictEntry** old_buckets = dict->buckets;
        
        dict->capacity *= 2;
        dict->buckets = (TauDictEntry**)calloc(dict->capacity, sizeof(TauDictEntry*));
        dict->size = 0;
        
        // Rehash all entries
        for (size_t i = 0; i < old_capacity; i++) {
            TauDictEntry* entry = old_buckets[i];
            while (entry != NULL) {
                TauDictEntry* next = entry->next;
                tauraro_dict_set(dict, entry->key, entry->value);
                free(entry->key);
                free(entry);
                entry = next;
            }
        }
        free(old_buckets);
    }
}

static inline TauValue tauraro_dict_get(TauDict* dict, const char* key) {
    if (!dict || !key) return tauraro_none();
    
    size_t index = tauraro_hash(key) % dict->capacity;
    TauDictEntry* entry = dict->buckets[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return tauraro_none();
}

static inline int tauraro_dict_has_key(TauDict* dict, const char* key) {
    if (!dict || !key) return 0;
    
    size_t index = tauraro_hash(key) % dict->capacity;
    TauDictEntry* entry = dict->buckets[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return 1;
        }
        entry = entry->next;
    }
    
    return 0;
}

static inline void tauraro_dict_free(TauDict* dict) {
    if (!dict) return;
    
    for (size_t i = 0; i < dict->capacity; i++) {
        TauDictEntry* entry = dict->buckets[i];
        while (entry != NULL) {
            TauDictEntry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(dict->buckets);
    free(dict);
}

// String helpers
static inline char* tauraro_string_concat(const char* s1, const char* s2) {
    if (!s1) s1 = "";
    if (!s2) s2 = "";
    
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* result = (char*)malloc(len1 + len2 + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static inline int tauraro_string_equals(const char* s1, const char* s2) {
    if (!s1 && !s2) return 1;
    if (!s1 || !s2) return 0;
    return strcmp(s1, s2) == 0;
}

static inline char* tauraro_string_copy(const char* s) {
    if (!s) return NULL;
    return strdup(s);
}

#endif // TAURARO_RUNTIME_H
