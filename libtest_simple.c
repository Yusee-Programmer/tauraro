#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdarg.h>
#include <setjmp.h>
#include <ctype.h>
#include <stdint.h>

// Math constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif
static const double tauraro_math_pi = M_PI;
static const double tauraro_math_e = M_E;

// Advanced Type definitions for Tauraro runtime
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

// Generic value type (for complex types)
struct TauValue {
    int type; // 0=int, 1=float, 2=string, 3=bool, 4=list, 5=dict, 6=object, 7=function, 8=exception
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
    TauNativeFunc native_func; // For C functions
    struct TauClosure* closure; // For closures
    int param_count;
    char** param_names;
    int is_native;
    int refcount;
};

// Closure with captured variables
struct TauClosure {
    TauDict* captured_vars; // Captured from outer scope
    TauFunction* function;
    int refcount;
};

// Iterator interface
struct TauIterator {
    void* data; // Iterator-specific data
    TauValue (*next)(struct TauIterator*);
    int (*has_next)(struct TauIterator*);
    void (*cleanup)(struct TauIterator*);
    int refcount;
};

// Exception type for error handling
struct TauException {
    char* type; // Exception type name
    char* message;
    char* traceback;
    TauValue value; // Optional associated value
    int refcount;
};

// Module system support
struct TauModule {
    char* name;
    char* path;
    TauDict* globals; // Module global variables
    TauDict* exports; // Exported symbols
    int is_loaded;
    int refcount;
};

// Bound method (method + instance)
struct TauMethod {
    TauObject* instance;
    TauFunction* function;
    int refcount;
};

// Runtime utility forward declarations
unsigned int tauraro_hash(const char* key);
TauDict* tauraro_create_dict();
void tauraro_dict_set(TauDict* dict, const char* key, TauValue value);
TauValue* tauraro_dict_get(TauDict* dict, const char* key);
TauValue tauraro_list_len(TauList* list);
TauList* tauraro_create_list(size_t initial_capacity);
void tauraro_list_append(TauList* list, TauValue item);
TauValue tauraro_list_get(TauList* list, long long index);
void tauraro_list_set(TauList* list, long long index, TauValue item);
TauValue tauraro_list_pop(TauList* list);
int tauraro_list_contains(TauList* list, TauValue item);
TauValue lst__append(TauValue lst, TauValue item);
TauValue text__upper(TauValue str);
TauValue text__lower(TauValue str);
TauValue text__strip(TauValue str);
TauValue text__split(TauValue str, TauValue delim);
TauValue text__join(TauValue delim, TauValue list);
TauValue text__replace(TauValue str, TauValue old_s, TauValue new_s);
TauValue text__startswith(TauValue str, TauValue prefix);
TauValue text__endswith(TauValue str, TauValue suffix);
TauValue text__find(TauValue str, TauValue substr);
TauValue text__title(TauValue str);
TauValue text__capitalize(TauValue str);
TauValue text__swapcase(TauValue str);
TauValue text__lstrip(TauValue str);
TauValue text__rstrip(TauValue str);
TauValue text__isdigit(TauValue str);
TauValue text__isalpha(TauValue str);
TauValue text__isalnum(TauValue str);
TauValue text__isspace(TauValue str);
TauValue text__isupper(TauValue str);
TauValue text__islower(TauValue str);
TauValue text__count(TauValue str, TauValue sub);
TauValue text__center(TauValue str, TauValue width);
TauValue text__ljust(TauValue str, TauValue width);
TauValue text__rjust(TauValue str, TauValue width);
TauValue text__zfill(TauValue str, TauValue width);
TauValue lst__pop(TauValue lst);
TauValue lst__insert(TauValue lst, TauValue index, TauValue item);
TauValue lst__remove(TauValue lst, TauValue item);
TauValue lst__extend(TauValue lst, TauValue other);
TauValue lst__index(TauValue lst, TauValue item);
TauValue lst__count(TauValue lst, TauValue item);
TauValue lst__reverse(TauValue lst);
TauValue lst__sort(TauValue lst);
TauValue lst__copy(TauValue lst);
TauValue lst__clear(TauValue lst);
TauValue range(TauValue end);
TauValue range2(TauValue start, TauValue end);
TauValue range3(TauValue start, TauValue end, TauValue step);
TauValue tauraro_abs(TauValue val);
TauValue tauraro_min(TauValue a, TauValue b);
TauValue tauraro_max(TauValue a, TauValue b);
TauValue tauraro_sum(TauValue list);
TauValue tauraro_super_call(TauObject* self, TauValue* args, int argc);
TauValue tauraro_sorted(TauValue list);
TauValue tauraro_reversed(TauValue list);
TauValue tauraro_enumerate_list(TauValue list, TauValue start);
TauValue tauraro_zip_lists(TauValue list1, TauValue list2);
TauValue tauraro_any(TauValue list);
TauValue tauraro_all(TauValue list);
TauValue tauraro_type_name(TauValue val);
TauValue tauraro_isinstance(TauValue obj, TauValue type_str);
TauValue tauraro_ord(TauValue ch);
TauValue tauraro_chr(TauValue num);
TauValue tauraro_round(TauValue num, TauValue places);
TauValue tauraro_pow(TauValue base, TauValue exp);
TauValue tauraro_sqrt(TauValue num);
TauValue tauraro_hex(TauValue num);
TauValue tauraro_bin(TauValue num);
TauValue tauraro_oct(TauValue num);
TauValue tauraro_divmod(TauValue a, TauValue b);
TauValue tauraro_to_list(TauValue val);
TauValue tauraro_to_set(TauValue val);
TauValue tauraro_repr(TauValue val);
TauValue tauraro_str_upper(TauValue str);
TauValue tauraro_str_lower(TauValue str);
TauValue tauraro_str_strip(TauValue str);
TauValue tauraro_str_lstrip(TauValue str);
TauValue tauraro_str_rstrip(TauValue str);
TauValue tauraro_str_title(TauValue str);
TauValue tauraro_str_capitalize(TauValue str);
TauValue tauraro_str_swapcase(TauValue str);
TauValue tauraro_str_isdigit(TauValue str);
TauValue tauraro_str_isalpha(TauValue str);
TauValue tauraro_str_isalnum(TauValue str);
TauValue tauraro_str_isspace(TauValue str);
TauValue tauraro_str_isupper(TauValue str);
TauValue tauraro_str_islower(TauValue str);
TauValue tauraro_str_count(TauValue str, TauValue sub);
TauValue tauraro_str_center(TauValue str, TauValue width);
TauValue tauraro_str_ljust(TauValue str, TauValue width);
TauValue tauraro_str_rjust(TauValue str, TauValue width);
TauValue tauraro_str_zfill(TauValue str, TauValue width);
TauValue tauraro_list_pop_v(TauValue list);
TauValue tauraro_list_insert(TauValue list, TauValue index, TauValue item);
TauValue tauraro_list_remove(TauValue list, TauValue item);
TauValue tauraro_list_extend_v(TauValue list, TauValue other);
int tauraro_equals(TauValue a, TauValue b);


// ===== SYS MODULE GLOBALS =====
// Global sys module instance (initialized in main)
static TauModule* g_sys_module = NULL;


// __name__ special variable
static TauValue __name__;

// ===== COMPREHENSIVE TAURARO RUNTIME UTILITIES =====

// Core value creation utilities
TauValue tauraro_int(long long i) {
    return (TauValue){.type = 0, .value.i = i, .refcount = 1, .next = NULL};
}

TauValue tauraro_float(double f) {
    return (TauValue){.type = 1, .value.f = f, .refcount = 1, .next = NULL};
}

TauValue tauraro_str(const char* s) {
    return (TauValue){.type = 2, .value.s = strdup(s), .refcount = 1, .next = NULL};
}

TauValue tauraro_bool(int b) {
    return (TauValue){.type = 3, .value.i = b ? 1 : 0, .refcount = 1, .next = NULL};
}

TauValue tauraro_none() {
    return (TauValue){.type = -1, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// Forward declaration for recursive formatting
char* tauraro_format_value(TauValue val);

// Format list to string recursively
char* tauraro_format_list(TauList* lst) {
    if (!lst) return strdup("[]");
    char* result = malloc(16384);
    result[0] = '[';
    result[1] = '\0';
    for (size_t i = 0; i < lst->size; i++) {
        if (i > 0) strcat(result, ", ");
        char* item = tauraro_format_value(lst->items[i]);
        strcat(result, item);
        free(item);
    }
    strcat(result, "]");
    return result;
}

// Format any value to string
char* tauraro_format_value(TauValue val) {
    char buffer[512];
    switch(val.type) {
        case 0: snprintf(buffer, sizeof(buffer), "%lld", val.value.i); return strdup(buffer);
        case 1: snprintf(buffer, sizeof(buffer), "%g", val.value.f); return strdup(buffer);
        case 2: {
            if (!val.value.s) return strdup("''");
            char* r = malloc(strlen(val.value.s) + 3);
            sprintf(r, "'%s'", val.value.s);
            return r;
        }
        case 3: return strdup(val.value.i ? "True" : "False");
        case 4: return tauraro_format_list(val.value.list);
        case 5: return strdup("<dict>");
        case 6: return strdup("<object>");
        case 7: return strdup("<function>");
        case -1: return strdup("None");
        default: return strdup("<unknown>");
    }
}

// String conversion utilities
TauValue tauraro_str_from_value(TauValue* val) {
    TauValue result = {.type = 2, .value.s = NULL, .refcount = 1};
    if (!val) {
        result.value.s = strdup("None");
        return result;
    }
    result.value.s = tauraro_format_value(*val);
    return result;
}

TauValue tauraro_get_attribute(TauObject* obj, const char* name) {
    if (!obj || !obj->attributes) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1};
    }
    
    unsigned int index = tauraro_hash(name) % obj->attributes->capacity;
    TauDictEntry* entry = obj->attributes->buckets[index];
    
    while (entry) {
        if (strcmp(entry->key, name) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1};
}

void tauraro_set_attribute(TauObject* obj, const char* name, TauValue value) {
    if (!obj) return;
    if (!obj->attributes) obj->attributes = tauraro_create_dict();
    tauraro_dict_set(obj->attributes, name, value);
}

// Polymorphic method dispatcher - handles method dispatch by checking actual class
// This is used for polymorphic calls where the object type is known at runtime
typedef TauValue (*MethodDispatcher)(TauValue obj);
TauValue tauraro_dispatch_method(TauValue obj, const char* method_name) {
    if (obj.type != 6 || !obj.value.obj || !obj.value.obj->class_name) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1};
    }
    const char* class_name = obj.value.obj->class_name;
    // Dispatch to appropriate method based on class name
    // Format: ClassName__method_name
    char full_method[256];
    snprintf(full_method, sizeof(full_method), "%s__%s", class_name, method_name);
    // This will be filled in by the caller with appropriate function pointers
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1};
}

// Advanced OOP support with inheritance
TauClass* tauraro_create_class(const char* name, TauClass* parent) {
    TauClass* cls = malloc(sizeof(TauClass));
    cls->name = strdup(name);
    cls->parent = parent;
    cls->methods = tauraro_create_dict();
    cls->static_methods = tauraro_create_dict();
    cls->properties = tauraro_create_dict();
    cls->instance_size = sizeof(TauObject);
    cls->refcount = 1;
    return cls;
}

TauObject* tauraro_create_object(const char* class_name) {
    TauObject* obj = malloc(sizeof(TauObject));
    obj->class_name = strdup(class_name);
    obj->class_ref = NULL; // Set by class system
    obj->attributes = tauraro_create_dict();
    obj->native_data = NULL;
    obj->refcount = 1;
    return obj;
}

void tauraro_class_add_method(TauClass* cls, const char* name, TauFunction* method) {
    if (!cls || !name || !method) return;
    TauValue method_val = {.type = 7, .value.func = method, .refcount = 1, .next = NULL};
    tauraro_dict_set(cls->methods, name, method_val);
}

TauValue tauraro_call_method(TauObject* obj, const char* method_name, int argc, TauValue* argv) {
    if (!obj || !method_name) return tauraro_none();
    TauClass* cls = obj->class_ref;
    while (cls) {
        TauValue* method_val = tauraro_dict_get(cls->methods, method_name);
        if (method_val && method_val->type == 7) {
            TauFunction* method = method_val->value.func;
            if (method->native_func) {
                return method->native_func(argc, argv);
            }
        }
        cls = cls->parent;
    }
    return tauraro_none();
}

// Function and closure support
TauFunction* tauraro_create_function(const char* name, TauNativeFunc func, int param_count) {
    TauFunction* f = malloc(sizeof(TauFunction));
    f->name = strdup(name);
    f->native_func = func;
    f->closure = NULL;
    f->param_count = param_count;
    f->param_names = NULL;
    f->is_native = 1;
    f->refcount = 1;
    return f;
}

TauClosure* tauraro_create_closure(TauFunction* func, int captured_count) {
    TauClosure* closure = malloc(sizeof(TauClosure));
    closure->captured_vars = tauraro_create_dict();
    closure->function = func;
    closure->refcount = 1;
    return closure;
}

void tauraro_closure_capture(TauClosure* closure, const char* var_name, TauValue* value) {
    if (!closure || !var_name || !value) return;
    tauraro_dict_set(closure->captured_vars, var_name, *value);
}

// Exception handling system
#include <setjmp.h>
jmp_buf tauraro_exception_buf;
TauException* tauraro_current_exception = NULL;

TauException* tauraro_create_exception(const char* type, const char* message) {
    TauException* exc = malloc(sizeof(TauException));
    exc->type = strdup(type);
    exc->message = strdup(message);
    exc->traceback = NULL;
    exc->value = tauraro_none();
    exc->refcount = 1;
    return exc;
}

void tauraro_throw_exception(TauException* exc) {
    tauraro_current_exception = exc;
    longjmp(tauraro_exception_buf, 1);
}

int tauraro_exception_matches(const char* type) {
    return tauraro_current_exception && 
           strcmp(tauraro_current_exception->type, type) == 0;
}

// Super call for inheritance
TauValue tauraro_super_call(TauObject* self, TauValue* args, int argc) {
    if (!self || !self->class_ref || !self->class_ref->parent) {
        return tauraro_none();
    }
    TauClass* parent = self->class_ref->parent;
    // Look up __init__ in parent class
    TauValue* init_method = tauraro_dict_get(parent->methods, "__init__");
    if (init_method && init_method->type == 7 && init_method->value.func) {
        return init_method->value.func->native_func(argc, args);
    }
    return tauraro_none();
}

// Tuple support (immutable fixed-size array)
typedef struct TauTuple {
    TauValue* items;
    size_t size;
    int refcount;
} TauTuple;

TauTuple* tauraro_create_tuple(size_t size) {
    TauTuple* tuple = malloc(sizeof(TauTuple));
    tuple->items = calloc(size, sizeof(TauValue));
    tuple->size = size;
    tuple->refcount = 1;
    return tuple;
}

TauValue tauraro_tuple_get(TauTuple* tuple, int index) {
    if (!tuple || index < 0 || index >= (int)tuple->size) return tauraro_none();
    return tuple->items[index];
}

TauValue tauraro_tuple_to_value(TauTuple* tuple) {
    return (TauValue){.type = 10, .value.ptr = tuple, .refcount = 1, .next = NULL};
}

// Set support (unordered unique values)
typedef struct TauSet {
    TauDict* data; // Use dict internally with null values
    int refcount;
} TauSet;

TauSet* tauraro_create_set() {
    TauSet* set = malloc(sizeof(TauSet));
    set->data = tauraro_create_dict();
    set->refcount = 1;
    return set;
}

void tauraro_set_add(TauSet* set, const char* value) {
    if (!set || !value) return;
    tauraro_dict_set(set->data, value, tauraro_bool(1));
}

int tauraro_set_contains(TauSet* set, const char* value) {
    if (!set || !value) return 0;
    return tauraro_dict_get(set->data, value) != NULL;
}

// Range iterator for for loops
typedef struct TauRange {
    long long start;
    long long stop;
    long long step;
    long long current;
} TauRange;

TauRange* tauraro_range(long long start, long long stop, long long step) {
    TauRange* r = malloc(sizeof(TauRange));
    r->start = start;
    r->stop = stop;
    r->step = step != 0 ? step : 1;
    r->current = start;
    return r;
}

int tauraro_range_has_next(TauRange* r) {
    if (!r) return 0;
    if (r->step > 0) return r->current < r->stop;
    return r->current > r->stop;
}

long long tauraro_range_next(TauRange* r) {
    if (!r) return 0;
    long long val = r->current;
    r->current += r->step;
    return val;
}

// Context manager support (for 'with' statement)
typedef struct TauContextManager {
    TauValue value;
    TauNativeFunc enter_func;
    TauNativeFunc exit_func;
} TauContextManager;

TauValue tauraro_context_enter(TauContextManager* ctx) {
    if (ctx && ctx->enter_func) {
        return ctx->enter_func(1, &ctx->value);
    }
    return ctx ? ctx->value : tauraro_none();
}

void tauraro_context_exit(TauContextManager* ctx, TauException* exc) {
    if (ctx && ctx->exit_func) {
        TauValue args[2] = { ctx->value, tauraro_none() };
        if (exc) args[1] = (TauValue){.type = 8, .value.exc = exc};
        ctx->exit_func(2, args);
    }
}

// Iterator support
TauIterator* tauraro_create_iterator(TauValue* iterable) {
    TauIterator* iter = malloc(sizeof(TauIterator));
    iter->data = iterable;
    iter->next = NULL;
    iter->has_next = NULL;
    iter->cleanup = NULL;
    iter->refcount = 1;
    return iter;
}

TauValue tauraro_iterator_next(TauIterator* iter) {
    if (!iter || !iter->next) return tauraro_none();
    return iter->next(iter);
}

// Module system support
TauModule* tauraro_create_module(const char* name, const char* path) {
    TauModule* mod = malloc(sizeof(TauModule));
    mod->name = strdup(name);
    mod->path = path ? strdup(path) : NULL;
    mod->globals = tauraro_create_dict();
    mod->exports = tauraro_create_dict();
    mod->is_loaded = 0;
    mod->refcount = 1;
    return mod;
}

TauModule* tauraro_import_module(const char* name) {
    if (strcmp(name, "sys") == 0 && g_sys_module != NULL) {
        return g_sys_module;
    }
    // Simplified import - in real implementation would load from file
    return tauraro_create_module(name, NULL);
}

TauValue tauraro_module_get(TauModule* mod, const char* name) {
    if (!mod || !name) return tauraro_none();
    TauValue* val = tauraro_dict_get(mod->exports, name);
    return val ? *val : tauraro_none();
}

TauValue tauraro_module_to_value(TauModule* mod) {
    return (TauValue){.type = 9, .value.ptr = mod, .refcount = 1, .next = NULL};
}

// Dictionary implementation
#define DICT_INITIAL_CAPACITY 16
#define DICT_LOAD_FACTOR 0.75

unsigned int tauraro_hash(const char* key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

TauDict* tauraro_create_dict() {
    TauDict* dict = malloc(sizeof(TauDict));
    if (dict) {
        dict->capacity = DICT_INITIAL_CAPACITY;
        dict->size = 0;
        dict->buckets = calloc(dict->capacity, sizeof(TauDictEntry*));
    }
    return dict;
}

void tauraro_dict_set(TauDict* dict, const char* key, TauValue value) {
    if (!dict || !key) return;
    
    unsigned int index = tauraro_hash(key) % dict->capacity;
    TauDictEntry* entry = dict->buckets[index];
    
    // Search for existing key
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    
    // Create new entry
    TauDictEntry* new_entry = malloc(sizeof(TauDictEntry));
    if (new_entry) {
        new_entry->key = strdup(key);
        new_entry->value = value;
        new_entry->next = dict->buckets[index];
        dict->buckets[index] = new_entry;
        dict->size++;
    }
}

TauValue* tauraro_dict_get(TauDict* dict, const char* key) {
    if (!dict || !key) return NULL;
    
    unsigned int index = tauraro_hash(key) % dict->capacity;
    TauDictEntry* entry = dict->buckets[index];
    
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return &entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

int tauraro_dict_len(TauDict* dict) {
    return dict ? dict->size : 0;
}

char* tauraro_dict_to_string(TauDict* dict) {
    if (!dict) return strdup("{}");
    
    static char buffer[2048];
    strcpy(buffer, "{");
    
    int first = 1;
    for (int i = 0; i < dict->capacity; i++) {
        TauDictEntry* entry = dict->buckets[i];
        while (entry) {
            if (!first) strcat(buffer, ", ");
            strcat(buffer, "'");
            strcat(buffer, entry->key);
            strcat(buffer, "': ");
            // Simple value representation
            char temp[64];
            snprintf(temp, sizeof(temp), "%p", entry->value);
            strcat(buffer, temp);
            first = 0;
            entry = entry->next;
        }
    }
    strcat(buffer, "}");
    return buffer;
}

// Enhanced built-in functions
TauValue tauraro_str_int(long long val) {
    static char buffer[32];
    sprintf(buffer, "%lld", val);
    return (TauValue){.type = 2, .value.s = strdup(buffer), .refcount = 1};
}

TauValue tauraro_str_double(double val) {
    static char buffer[32];
    sprintf(buffer, "%f", val);
    return (TauValue){.type = 2, .value.s = strdup(buffer), .refcount = 1};
}

TauValue tauraro_int_string(const char* str) {
    long long val = strtoll(str, NULL, 10);
    return (TauValue){.type = 0, .value.i = val, .refcount = 1};
}

TauValue tauraro_float_string(const char* str) {
    double val = strtod(str, NULL);
    return (TauValue){.type = 1, .value.f = val, .refcount = 1};
}

TauValue tauraro_list_len(TauList* list) {
    if (!list) return (TauValue){.type = 0, .value.i = 0, .refcount = 1};
    return (TauValue){.type = 0, .value.i = (long long)list->size, .refcount = 1};
}

TauValue lst__append(TauValue lst, TauValue item) {
    if (lst.type == 4 && lst.value.list) {
        TauList* list = lst.value.list;
        if (list->size >= list->capacity) {
            list->capacity = (list->capacity + 1) * 2;
            list->items = realloc(list->items, sizeof(TauValue) * list->capacity);
        }
        list->items[list->size++] = item;
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1}; // None
}

TauValue text__upper(TauValue str) {
    if (str.type == 2 && str.value.s) {
        char* result = strdup(str.value.s);
        for (int i = 0; result[i]; i++) {
            if (result[i] >= 'a' && result[i] <= 'z') {
                result[i] = result[i] - 32;
            }
        }
        return (TauValue){.type = 2, .value.s = result, .refcount = 1};
    }
    return str;
}

TauValue text__lower(TauValue str) {
    if (str.type == 2 && str.value.s) {
        char* result = strdup(str.value.s);
        for (int i = 0; result[i]; i++) {
            if (result[i] >= 'A' && result[i] <= 'Z') {
                result[i] = result[i] + 32;
            }
        }
        return (TauValue){.type = 2, .value.s = result, .refcount = 1};
    }
    return str;
}

TauValue range(TauValue end) {
    long long n = 0;
    if (end.type == 0) n = end.value.i;
    TauList* list = malloc(sizeof(TauList));
    list->size = n;
    list->capacity = n;
    list->items = malloc(sizeof(TauValue) * n);
    for (long long i = 0; i < n; i++) {
        list->items[i] = (TauValue){.type = 0, .value.i = i, .refcount = 1};
    }
    return (TauValue){.type = 4, .value.list = list, .refcount = 1};
}

TauValue range2(TauValue start, TauValue end) {
    long long s = 0, e = 0;
    if (start.type == 0) s = start.value.i;
    if (end.type == 0) e = end.value.i;
    long long n = e > s ? e - s : 0;
    TauList* list = malloc(sizeof(TauList));
    list->size = n;
    list->capacity = n;
    list->items = malloc(sizeof(TauValue) * n);
    for (long long i = 0; i < n; i++) {
        list->items[i] = (TauValue){.type = 0, .value.i = s + i, .refcount = 1};
    }
    return (TauValue){.type = 4, .value.list = list, .refcount = 1};
}

TauValue range3(TauValue start, TauValue end, TauValue step) {
    long long s = 0, e = 0, st = 1;
    if (start.type == 0) s = start.value.i;
    if (end.type == 0) e = end.value.i;
    if (step.type == 0 && step.value.i != 0) st = step.value.i;
    long long n = 0;
    if (st > 0 && e > s) n = (e - s + st - 1) / st;
    else if (st < 0 && s > e) n = (s - e - st - 1) / (-st);
    if (n < 0) n = 0;
    TauList* list = malloc(sizeof(TauList));
    list->size = n;
    list->capacity = n;
    list->items = malloc(sizeof(TauValue) * n);
    for (long long i = 0; i < n; i++) {
        list->items[i] = (TauValue){.type = 0, .value.i = s + i * st, .refcount = 1};
    }
    return (TauValue){.type = 4, .value.list = list, .refcount = 1};
}

// List utility functions
TauList* tauraro_create_list(size_t initial_capacity) {
    TauList* list = malloc(sizeof(TauList));
    list->size = 0;
    list->capacity = initial_capacity > 0 ? initial_capacity : 8;
    list->items = malloc(sizeof(TauValue) * list->capacity);
    list->refcount = 1;
    return list;
}

void tauraro_list_append(TauList* list, TauValue item) {
    if (!list) return;
    if (list->size >= list->capacity) {
        list->capacity = (list->capacity + 1) * 2;
        list->items = realloc(list->items, sizeof(TauValue) * list->capacity);
    }
    list->items[list->size++] = item;
}

TauValue tauraro_list_get(TauList* list, long long index) {
    if (!list) return tauraro_none();
    if (index < 0) index = list->size + index; // Negative indexing
    if (index < 0 || index >= (long long)list->size) return tauraro_none();
    return list->items[index];
}

void tauraro_list_set(TauList* list, long long index, TauValue item) {
    if (!list) return;
    if (index < 0) index = list->size + index;
    if (index < 0 || index >= (long long)list->size) return;
    list->items[index] = item;
}

TauValue tauraro_list_pop(TauList* list) {
    if (!list || list->size == 0) return tauraro_none();
    return list->items[--list->size];
}

TauValue tauraro_list_extend(TauList* list, TauList* other) {
    if (!list || !other) return tauraro_none();
    for (size_t i = 0; i < other->size; i++) {
        tauraro_list_append(list, other->items[i]);
    }
    return tauraro_none();
}

int tauraro_list_contains(TauList* list, TauValue item) {
    if (!list) return 0;
    for (size_t i = 0; i < list->size; i++) {
        if (list->items[i].type == item.type) {
            if (item.type == 0 && list->items[i].value.i == item.value.i) return 1;
            if (item.type == 2 && strcmp(list->items[i].value.s, item.value.s) == 0) return 1;
        }
    }
    return 0;
}

// String utility functions
TauValue text__strip(TauValue str) {
    if (str.type != 2 || !str.value.s) return str;
    char* s = str.value.s;
    while (*s == ' ' || *s == '\t' || *s == '\n') s++;
    char* result = strdup(s);
    size_t len = strlen(result);
    while (len > 0 && (result[len-1] == ' ' || result[len-1] == '\t' || result[len-1] == '\n')) {
        result[--len] = '\0';
    }
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue text__split(TauValue str, TauValue delim) {
    if (str.type != 2 || !str.value.s) return tauraro_none();
    char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : " ";
    TauList* result = tauraro_create_list(8);
    char* s = strdup(str.value.s);
    char* token = strtok(s, d);
    while (token) {
        tauraro_list_append(result, tauraro_str(token));
        token = strtok(NULL, d);
    }
    free(s);
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue text__join(TauValue delim, TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_str("");
    char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : "";
    TauList* lst = list.value.list;
    if (lst->size == 0) return tauraro_str("");
    size_t total_len = 0;
    for (size_t i = 0; i < lst->size; i++) {
        if (lst->items[i].type == 2 && lst->items[i].value.s) {
            total_len += strlen(lst->items[i].value.s);
        }
    }
    total_len += strlen(d) * (lst->size - 1) + 1;
    char* result = malloc(total_len);
    result[0] = '\0';
    for (size_t i = 0; i < lst->size; i++) {
        if (i > 0) strcat(result, d);
        if (lst->items[i].type == 2 && lst->items[i].value.s) {
            strcat(result, lst->items[i].value.s);
        }
    }
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue text__replace(TauValue str, TauValue old_s, TauValue new_s) {
    if (str.type != 2 || !str.value.s) return str;
    if (old_s.type != 2 || !old_s.value.s) return str;
    char* olds = old_s.value.s;
    char* news = (new_s.type == 2 && new_s.value.s) ? new_s.value.s : "";
    char* src = str.value.s;
    size_t old_len = strlen(olds);
    if (old_len == 0) return str;
    size_t count = 0;
    char* p = src;
    while ((p = strstr(p, olds))) { count++; p += old_len; }
    size_t new_len = strlen(news);
    size_t result_len = strlen(src) + count * (new_len - old_len) + 1;
    char* result = malloc(result_len);
    result[0] = '\0';
    p = src;
    char* r = result;
    while (*p) {
        if (strncmp(p, olds, old_len) == 0) {
            strcpy(r, news);
            r += new_len;
            p += old_len;
        } else {
            *r++ = *p++;
        }
    }
    *r = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue text__startswith(TauValue str, TauValue prefix) {
    if (str.type != 2 || !str.value.s || prefix.type != 2 || !prefix.value.s) {
        return tauraro_bool(0);
    }
    return tauraro_bool(strncmp(str.value.s, prefix.value.s, strlen(prefix.value.s)) == 0);
}

TauValue text__endswith(TauValue str, TauValue suffix) {
    if (str.type != 2 || !str.value.s || suffix.type != 2 || !suffix.value.s) {
        return tauraro_bool(0);
    }
    size_t str_len = strlen(str.value.s);
    size_t suf_len = strlen(suffix.value.s);
    if (suf_len > str_len) return tauraro_bool(0);
    return tauraro_bool(strcmp(str.value.s + str_len - suf_len, suffix.value.s) == 0);
}

TauValue text__find(TauValue str, TauValue substr) {
    if (str.type != 2 || !str.value.s || substr.type != 2 || !substr.value.s) {
        return tauraro_int(-1);
    }
    char* p = strstr(str.value.s, substr.value.s);
    if (!p) return tauraro_int(-1);
    return tauraro_int((long long)(p - str.value.s));
}

TauValue text__title(TauValue str) { return tauraro_str_title(str); }
TauValue text__capitalize(TauValue str) { return tauraro_str_capitalize(str); }
TauValue text__swapcase(TauValue str) { return tauraro_str_swapcase(str); }
TauValue text__lstrip(TauValue str) { return tauraro_str_lstrip(str); }
TauValue text__rstrip(TauValue str) { return tauraro_str_rstrip(str); }
TauValue text__isdigit(TauValue str) { return tauraro_str_isdigit(str); }
TauValue text__isalpha(TauValue str) { return tauraro_str_isalpha(str); }
TauValue text__isalnum(TauValue str) { return tauraro_str_isalnum(str); }
TauValue text__isspace(TauValue str) { return tauraro_str_isspace(str); }
TauValue text__isupper(TauValue str) { return tauraro_str_isupper(str); }
TauValue text__islower(TauValue str) { return tauraro_str_islower(str); }
TauValue text__count(TauValue val, TauValue sub) {
    if (val.type == 4 && val.value.list) { // list.count()
        TauList* list = val.value.list;
        long long cnt = 0;
        for (size_t i = 0; i < list->size; i++) {
            if (tauraro_equals(list->items[i], sub)) cnt++;
        }
        return tauraro_int(cnt);
    }
    return tauraro_str_count(val, sub); // str.count()
}
TauValue text__center(TauValue str, TauValue width) { return tauraro_str_center(str, width); }
TauValue text__ljust(TauValue str, TauValue width) { return tauraro_str_ljust(str, width); }
TauValue text__rjust(TauValue str, TauValue width) { return tauraro_str_rjust(str, width); }
TauValue text__zfill(TauValue str, TauValue width) { return tauraro_str_zfill(str, width); }

TauValue lst__pop(TauValue lst) {
    if (lst.type != 4 || !lst.value.list || lst.value.list->size == 0) return tauraro_none();
    return tauraro_list_pop_v(lst);
}

TauValue lst__insert(TauValue lst, TauValue index, TauValue item) {
    if (lst.type != 4 || !lst.value.list) return tauraro_none();
    return tauraro_list_insert(lst, index, item);
}

TauValue lst__remove(TauValue lst, TauValue item) {
    if (lst.type != 4 || !lst.value.list) return tauraro_none();
    return tauraro_list_remove(lst, item);
}

TauValue lst__extend(TauValue lst, TauValue other) {
    if (lst.type != 4 || !lst.value.list) return tauraro_none();
    return tauraro_list_extend_v(lst, other);
}

TauValue lst__index(TauValue lst, TauValue item) {
    if (lst.type != 4 || !lst.value.list) return tauraro_int(-1);
    TauList* list = lst.value.list;
    for (size_t i = 0; i < list->size; i++) {
        if (tauraro_equals(list->items[i], item)) return tauraro_int((long long)i);
    }
    return tauraro_int(-1);
}

TauValue lst__count(TauValue lst, TauValue item) {
    if (lst.type != 4 || !lst.value.list) return tauraro_int(0);
    TauList* list = lst.value.list;
    long long count = 0;
    for (size_t i = 0; i < list->size; i++) {
        if (tauraro_equals(list->items[i], item)) count++;
    }
    return tauraro_int(count);
}

TauValue lst__reverse(TauValue lst) {
    if (lst.type != 4 || !lst.value.list) return tauraro_none();
    TauList* list = lst.value.list;
    for (size_t i = 0; i < list->size / 2; i++) {
        TauValue tmp = list->items[i];
        list->items[i] = list->items[list->size - 1 - i];
        list->items[list->size - 1 - i] = tmp;
    }
    return tauraro_none();
}

TauValue lst__sort(TauValue lst) {
    if (lst.type != 4 || !lst.value.list || lst.value.list->size < 2) return tauraro_none();
    TauList* list = lst.value.list;
    // Simple bubble sort for now
    for (size_t i = 0; i < list->size - 1; i++) {
        for (size_t j = 0; j < list->size - 1 - i; j++) {
            TauValue a = list->items[j], b = list->items[j+1];
            int swap = 0;
            if (a.type == 0 && b.type == 0) swap = a.value.i > b.value.i;
            else if (a.type == 1 && b.type == 1) swap = a.value.f > b.value.f;
            else if (a.type == 0 && b.type == 1) swap = (double)a.value.i > b.value.f;
            else if (a.type == 1 && b.type == 0) swap = a.value.f > (double)b.value.i;
            else if (a.type == 2 && b.type == 2 && a.value.s && b.value.s) swap = strcmp(a.value.s, b.value.s) > 0;
            if (swap) { list->items[j] = b; list->items[j+1] = a; }
        }
    }
    return tauraro_none();
}

TauValue lst__copy(TauValue lst) {
    if (lst.type != 4 || !lst.value.list) return tauraro_none();
    TauList* src = lst.value.list;
    TauList* copy = tauraro_create_list(src->size);
    for (size_t i = 0; i < src->size; i++) {
        tauraro_list_append(copy, src->items[i]);
    }
    return (TauValue){.type = 4, .value.list = copy, .refcount = 1};
}

TauValue lst__clear(TauValue lst) {
    if (lst.type != 4 || !lst.value.list) return tauraro_none();
    lst.value.list->size = 0;
    return tauraro_none();
}

// Type conversion utilities
TauValue tauraro_abs(TauValue val) {
    if (val.type == 0) return tauraro_int(val.value.i < 0 ? -val.value.i : val.value.i);
    if (val.type == 1) return tauraro_float(val.value.f < 0 ? -val.value.f : val.value.f);
    return val;
}

int tauraro_equals(TauValue a, TauValue b) {
    if (a.type != b.type) {
        // Allow int/float comparison
        if ((a.type == 0 && b.type == 1) || (a.type == 1 && b.type == 0)) {
            double av = a.type == 0 ? (double)a.value.i : a.value.f;
            double bv = b.type == 0 ? (double)b.value.i : b.value.f;
            return av == bv;
        }
        return 0;
    }
    switch (a.type) {
        case 0: return a.value.i == b.value.i;
        case 1: return a.value.f == b.value.f;
        case 2: return (a.value.s && b.value.s) ? strcmp(a.value.s, b.value.s) == 0 : (a.value.s == b.value.s);
        case 3: return a.value.i == b.value.i; // bool stored as int
        default: return 0;
    }
}

TauValue tauraro_min(TauValue a, TauValue b) {
    if (a.type == 0 && b.type == 0) return tauraro_int(a.value.i < b.value.i ? a.value.i : b.value.i);
    if (a.type == 1 || b.type == 1) {
        double av = a.type == 0 ? (double)a.value.i : a.value.f;
        double bv = b.type == 0 ? (double)b.value.i : b.value.f;
        return tauraro_float(av < bv ? av : bv);
    }
    return a;
}

TauValue tauraro_max(TauValue a, TauValue b) {
    if (a.type == 0 && b.type == 0) return tauraro_int(a.value.i > b.value.i ? a.value.i : b.value.i);
    if (a.type == 1 || b.type == 1) {
        double av = a.type == 0 ? (double)a.value.i : a.value.f;
        double bv = b.type == 0 ? (double)b.value.i : b.value.f;
        return tauraro_float(av > bv ? av : bv);
    }
    return a;
}

TauValue tauraro_sum(TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_int(0);
    TauList* lst = list.value.list;
    long long isum = 0;
    double fsum = 0.0;
    int is_float = 0;
    for (size_t i = 0; i < lst->size; i++) {
        if (lst->items[i].type == 0) isum += lst->items[i].value.i;
        else if (lst->items[i].type == 1) { fsum += lst->items[i].value.f; is_float = 1; }
    }
    if (is_float) return tauraro_float(fsum + (double)isum);
    return tauraro_int(isum);
}

// Optimized list slicing
TauValue tauraro_list_slice(TauValue list, long long start, long long stop, long long step) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* src = list.value.list;
    long long len = (long long)src->size;
    // Normalize negative indices
    if (start < 0) start = start + len;
    if (stop < 0) stop = stop + len;
    // Clamp to bounds
    if (start < 0) start = 0;
    if (start > len) start = len;
    if (stop < 0) stop = 0;
    if (stop > len) stop = len;
    if (step == 0) step = 1; // Prevent infinite loop
    // Calculate result size
    size_t result_size = 0;
    if (step > 0 && start < stop) {
        result_size = (size_t)((stop - start + step - 1) / step);
    } else if (step < 0 && start > stop) {
        result_size = (size_t)((start - stop - step - 1) / (-step));
    }
    TauList* result = tauraro_create_list(result_size > 0 ? result_size : 1);
    if (step > 0) {
        for (long long i = start; i < stop; i += step) {
            tauraro_list_append(result, src->items[i]);
        }
    } else {
        for (long long i = start; i > stop; i += step) {
            tauraro_list_append(result, src->items[i]);
        }
    }
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue tauraro_string_slice(TauValue str, long long start, long long stop, long long step) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    long long len = (long long)strlen(str.value.s);
    if (start < 0) start = start + len;
    if (stop < 0) stop = stop + len;
    if (start < 0) start = 0;
    if (start > len) start = len;
    if (stop < 0) stop = 0;
    if (stop > len) stop = len;
    if (step == 0) step = 1;
    // Calculate result size
    size_t result_size = 0;
    if (step > 0 && start < stop) {
        result_size = (size_t)((stop - start + step - 1) / step);
    } else if (step < 0 && start > stop) {
        result_size = (size_t)((start - stop - step - 1) / (-step));
    }
    char* result = malloc(result_size + 1);
    size_t j = 0;
    if (step > 0) {
        for (long long i = start; i < stop && j < result_size; i += step) {
            result[j++] = str.value.s[i];
        }
    } else {
        for (long long i = start; i > stop && j < result_size; i += step) {
            result[j++] = str.value.s[i];
        }
    }
    result[j] = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_slice(TauValue obj, long long start, long long stop, long long step) {
    if (obj.type == 4) return tauraro_list_slice(obj, start, stop, step);
    if (obj.type == 2) return tauraro_string_slice(obj, start, stop, step);
    return tauraro_none();
}

// Note: tauraro_format_value is defined earlier with full list/dict support
TauValue tauraro_fstring_concat(int count, ...) {
    va_list args;
    va_start(args, count);
    // First pass: calculate total length
    size_t total_len = 1; // For null terminator
    char** parts = malloc(count * sizeof(char*));
    for (int i = 0; i < count; i++) {
        TauValue v = va_arg(args, TauValue);
        parts[i] = tauraro_format_value(v);
        total_len += strlen(parts[i]);
    }
    va_end(args);
    // Second pass: build result
    char* result = malloc(total_len);
    result[0] = '\0';
    for (int i = 0; i < count; i++) {
        strcat(result, parts[i]);
        free(parts[i]);
    }
    free(parts);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

// List comprehension optimization macros
#define TAURARO_LISTCOMP_BEGIN(result_var, capacity) \
    TauList* result_var = tauraro_create_list(capacity)

#define TAURARO_LISTCOMP_ADD(result_var, value) \
    tauraro_list_append(result_var, value)

#define TAURARO_LISTCOMP_END(result_var) \
    (TauValue){.type = 4, .value.list = result_var, .refcount = 1}

// Value equality check for sets/dicts
int tauraro_value_equals(TauValue a, TauValue b) {
    if (a.type != b.type) return 0;
    switch (a.type) {
        case 0: return a.value.i == b.value.i;
        case 1: return a.value.f == b.value.f;
        case 2: return strcmp(a.value.s ? a.value.s : "", b.value.s ? b.value.s : "") == 0;
        default: return 0;
    }
}

// Generator state structure
typedef struct TauGeneratorState {
    int state;
    TauValue last_value;
    void* context;
} TauGeneratorState;

TauGeneratorState* tauraro_create_generator_state() {
    TauGeneratorState* g = malloc(sizeof(TauGeneratorState));
    g->state = 0;
    g->last_value = tauraro_none();
    g->context = NULL;
    return g;
}

typedef struct TauEnumerate {
    TauList* list;
    size_t index;
    long long start;
} TauEnumerate;

TauEnumerate* tauraro_enumerate(TauValue list, long long start) {
    TauEnumerate* e = malloc(sizeof(TauEnumerate));
    e->list = list.type == 4 ? list.value.list : NULL;
    e->index = 0;
    e->start = start;
    return e;
}

int tauraro_enumerate_next(TauEnumerate* e, long long* idx, TauValue* val) {
    if (!e || !e->list || e->index >= e->list->size) return 0;
    *idx = e->start + (long long)e->index;
    *val = e->list->items[e->index++];
    return 1;
}

typedef struct TauZip {
    TauList** lists;
    size_t list_count;
    size_t index;
    size_t min_len;
} TauZip;

TauZip* tauraro_zip(int count, ...) {
    va_list args;
    va_start(args, count);
    TauZip* z = malloc(sizeof(TauZip));
    z->lists = malloc(count * sizeof(TauList*));
    z->list_count = count;
    z->index = 0;
    z->min_len = SIZE_MAX;
    for (int i = 0; i < count; i++) {
        TauValue v = va_arg(args, TauValue);
        z->lists[i] = v.type == 4 ? v.value.list : NULL;
        if (z->lists[i] && z->lists[i]->size < z->min_len) {
            z->min_len = z->lists[i]->size;
        }
    }
    va_end(args);
    return z;
}

int tauraro_zip_next(TauZip* z, TauValue* results) {
    if (!z || z->index >= z->min_len) return 0;
    for (size_t i = 0; i < z->list_count; i++) {
        results[i] = z->lists[i] ? z->lists[i]->items[z->index] : tauraro_none();
    }
    z->index++;
    return 1;
}

TauValue tauraro_all(TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_bool(1);
    TauList* lst = list.value.list;
    for (size_t i = 0; i < lst->size; i++) {
        TauValue v = lst->items[i];
        if (v.type == 0 && v.value.i == 0) return tauraro_bool(0);
        if (v.type == 1 && v.value.f == 0.0) return tauraro_bool(0);
        if (v.type == 3 && v.value.i == 0) return tauraro_bool(0);
        if (v.type == 2 && (!v.value.s || v.value.s[0] == '\0')) return tauraro_bool(0);
        if (v.type == 4 && (!v.value.list || v.value.list->size == 0)) return tauraro_bool(0);
        if (v.type == 5 && (!v.value.dict || v.value.dict->size == 0)) return tauraro_bool(0);
    }
    return tauraro_bool(1);
}

TauValue tauraro_any(TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_bool(0);
    TauList* lst = list.value.list;
    for (size_t i = 0; i < lst->size; i++) {
        TauValue v = lst->items[i];
        if (v.type == 0 && v.value.i != 0) return tauraro_bool(1);
        if (v.type == 1 && v.value.f != 0.0) return tauraro_bool(1);
        if (v.type == 3 && v.value.i != 0) return tauraro_bool(1);
        if (v.type == 2 && v.value.s && v.value.s[0] != '\0') return tauraro_bool(1);
        if (v.type == 4 && v.value.list && v.value.list->size > 0) return tauraro_bool(1);
        if (v.type == 5 && v.value.dict && v.value.dict->size > 0) return tauraro_bool(1);
    }
    return tauraro_bool(0);
}

int tauraro_compare_values(const void* a, const void* b) {
    TauValue* va = (TauValue*)a;
    TauValue* vb = (TauValue*)b;
    if (va->type == 0 && vb->type == 0) {
        return va->value.i < vb->value.i ? -1 : (va->value.i > vb->value.i ? 1 : 0);
    }
    if (va->type == 2 && vb->type == 2) {
        return strcmp(va->value.s ? va->value.s : "", vb->value.s ? vb->value.s : "");
    }
    return 0;
}

TauValue tauraro_sorted(TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* src = list.value.list;
    TauList* result = tauraro_create_list(src->size);
    for (size_t i = 0; i < src->size; i++) {
        tauraro_list_append(result, src->items[i]);
    }
    qsort(result->items, result->size, sizeof(TauValue), tauraro_compare_values);
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue tauraro_reversed(TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* src = list.value.list;
    TauList* result = tauraro_create_list(src->size);
    for (size_t i = src->size; i > 0; i--) {
        tauraro_list_append(result, src->items[i - 1]);
    }
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue tauraro_list_index(TauValue list, TauValue item) {
    if (list.type != 4 || !list.value.list) return tauraro_int(-1);
    TauList* lst = list.value.list;
    for (size_t i = 0; i < lst->size; i++) {
        if (tauraro_value_equals(lst->items[i], item)) return tauraro_int((long long)i);
    }
    return tauraro_int(-1);
}

TauValue tauraro_list_count(TauValue list, TauValue item) {
    if (list.type != 4 || !list.value.list) return tauraro_int(0);
    TauList* lst = list.value.list;
    long long count = 0;
    for (size_t i = 0; i < lst->size; i++) {
        if (tauraro_value_equals(lst->items[i], item)) count++;
    }
    return tauraro_int(count);
}

TauValue tauraro_dict_keys_v(TauValue dict) {
    if (dict.type != 5 || !dict.value.dict) return tauraro_none();
    TauDict* d = dict.value.dict;
    TauList* keys = tauraro_create_list(d->size);
    for (size_t i = 0; i < d->capacity; i++) {
        TauDictEntry* entry = d->buckets[i];
        while (entry) {
            tauraro_list_append(keys, tauraro_str(entry->key));
            entry = entry->next;
        }
    }
    return (TauValue){.type = 4, .value.list = keys, .refcount = 1};
}

TauValue tauraro_dict_values_v(TauValue dict) {
    if (dict.type != 5 || !dict.value.dict) return tauraro_none();
    TauDict* d = dict.value.dict;
    TauList* values = tauraro_create_list(d->size);
    for (size_t i = 0; i < d->capacity; i++) {
        TauDictEntry* entry = d->buckets[i];
        while (entry) {
            tauraro_list_append(values, entry->value);
            entry = entry->next;
        }
    }
    return (TauValue){.type = 4, .value.list = values, .refcount = 1};
}

TauValue tauraro_dict_items_v(TauValue dict) {
    if (dict.type != 5 || !dict.value.dict) return tauraro_none();
    TauDict* d = dict.value.dict;
    TauList* items = tauraro_create_list(d->size);
    for (size_t i = 0; i < d->capacity; i++) {
        TauDictEntry* entry = d->buckets[i];
        while (entry) {
            TauList* pair = tauraro_create_list(2);
            tauraro_list_append(pair, tauraro_str(entry->key));
            tauraro_list_append(pair, entry->value);
            TauValue pair_val = {.type = 4, .value.list = pair, .refcount = 1};
            tauraro_list_append(items, pair_val);
            entry = entry->next;
        }
    }
    return (TauValue){.type = 4, .value.list = items, .refcount = 1};
}

TauValue tauraro_dict_get_v(TauValue dict, TauValue key, TauValue default_val) {
    if (dict.type != 5 || !dict.value.dict) return default_val;
    if (key.type != 2 || !key.value.s) return default_val;
    TauValue* result = tauraro_dict_get(dict.value.dict, key.value.s);
    return result ? *result : default_val;
}

TauValue tauraro_dict_pop_v(TauValue dict, TauValue key, TauValue default_val) {
    if (dict.type != 5 || !dict.value.dict) return default_val;
    if (key.type != 2 || !key.value.s) return default_val;
    TauValue* result = tauraro_dict_get(dict.value.dict, key.value.s);
    if (!result) return default_val;
    TauValue val = *result;
    // Note: actual removal would require more complex logic
    return val;
}

// Simplified context manager for TauValue-based contexts
TauValue tauraro_ctx_enter(TauValue ctx) {
    // For objects with __enter__ method, call it
    // For now, just return the context itself
    return ctx;
}

void tauraro_ctx_exit(TauValue ctx) {
    // For objects with __exit__ method, call it
    // For now, do nothing
    (void)ctx;
}

// Lambda/Closure support with variable capture
typedef struct TauLambda {
    TauValue (*func)(struct TauLambda*, int, TauValue*);
    TauValue* captures;
    int capture_count;
    int param_count;
} TauLambda;

TauLambda* tauraro_create_lambda(TauValue (*func)(TauLambda*, int, TauValue*), int param_count, int capture_count) {
    TauLambda* l = malloc(sizeof(TauLambda));
    l->func = func;
    l->param_count = param_count;
    l->capture_count = capture_count;
    l->captures = capture_count > 0 ? malloc(capture_count * sizeof(TauValue)) : NULL;
    return l;
}

TauValue tauraro_call_lambda(TauLambda* l, int argc, TauValue* argv) {
    if (!l || !l->func) return tauraro_none();
    return l->func(l, argc, argv);
}

// Functional programming utilities
typedef TauValue (*TauMapFunc)(TauValue);
typedef int (*TauFilterFunc)(TauValue);
typedef TauValue (*TauReduceFunc)(TauValue, TauValue);

TauValue tauraro_map_fn(TauMapFunc fn, TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* src = list.value.list;
    TauList* result = tauraro_create_list(src->size);
    for (size_t i = 0; i < src->size; i++) {
        tauraro_list_append(result, fn(src->items[i]));
    }
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue tauraro_filter_fn(TauFilterFunc fn, TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* src = list.value.list;
    TauList* result = tauraro_create_list(src->size);
    for (size_t i = 0; i < src->size; i++) {
        if (fn(src->items[i])) tauraro_list_append(result, src->items[i]);
    }
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue tauraro_reduce_fn(TauReduceFunc fn, TauValue list, TauValue initial) {
    if (list.type != 4 || !list.value.list) return initial;
    TauList* src = list.value.list;
    TauValue acc = initial;
    for (size_t i = 0; i < src->size; i++) {
        acc = fn(acc, src->items[i]);
    }
    return acc;
}

// Range with step support
TauValue tauraro_range_list(long long start, long long stop, long long step) {
    if (step == 0) step = 1;
    size_t count = 0;
    if (step > 0 && start < stop) count = (size_t)((stop - start + step - 1) / step);
    else if (step < 0 && start > stop) count = (size_t)((start - stop - step - 1) / (-step));
    TauList* result = tauraro_create_list(count > 0 ? count : 1);
    if (step > 0) {
        for (long long i = start; i < stop; i += step) {
            tauraro_list_append(result, tauraro_int(i));
        }
    } else {
        for (long long i = start; i > stop; i += step) {
            tauraro_list_append(result, tauraro_int(i));
        }
    }
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

// String manipulation methods
TauValue tauraro_str_split(TauValue str, TauValue delim) {
    if (str.type != 2 || !str.value.s) return tauraro_none();
    const char* s = str.value.s;
    const char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : " ";
    TauList* result = tauraro_create_list(16);
    char* copy = strdup(s);
    char* token = strtok(copy, d);
    while (token) {
        tauraro_list_append(result, tauraro_str(token));
        token = strtok(NULL, d);
    }
    free(copy);
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue tauraro_str_join(TauValue delim, TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_str("");
    const char* d = (delim.type == 2 && delim.value.s) ? delim.value.s : "";
    TauList* lst = list.value.list;
    if (lst->size == 0) return tauraro_str("");
    // Calculate total length
    size_t total = 0, dlen = strlen(d);
    for (size_t i = 0; i < lst->size; i++) {
        if (lst->items[i].type == 2 && lst->items[i].value.s)
            total += strlen(lst->items[i].value.s);
        if (i < lst->size - 1) total += dlen;
    }
    char* result = malloc(total + 1);
    result[0] = '\0';
    for (size_t i = 0; i < lst->size; i++) {
        if (lst->items[i].type == 2 && lst->items[i].value.s)
            strcat(result, lst->items[i].value.s);
        if (i < lst->size - 1) strcat(result, d);
    }
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_replace(TauValue str, TauValue old, TauValue new_str) {
    if (str.type != 2 || !str.value.s) return str;
    if (old.type != 2 || !old.value.s || old.value.s[0] == '\0') return str;
    const char* s = str.value.s;
    const char* o = old.value.s;
    const char* n = (new_str.type == 2 && new_str.value.s) ? new_str.value.s : "";
    size_t olen = strlen(o), nlen = strlen(n), slen = strlen(s);
    // Count occurrences
    size_t count = 0;
    const char* p = s;
    while ((p = strstr(p, o)) != NULL) { count++; p += olen; }
    // Allocate result
    size_t rlen = slen + count * (nlen - olen);
    char* result = malloc(rlen + 1);
    char* r = result;
    p = s;
    const char* q;
    while ((q = strstr(p, o)) != NULL) {
        size_t len = q - p;
        memcpy(r, p, len); r += len;
        memcpy(r, n, nlen); r += nlen;
        p = q + olen;
    }
    strcpy(r, p);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_find(TauValue str, TauValue sub) {
    if (str.type != 2 || !str.value.s) return tauraro_int(-1);
    if (sub.type != 2 || !sub.value.s) return tauraro_int(-1);
    const char* p = strstr(str.value.s, sub.value.s);
    if (!p) return tauraro_int(-1);
    return tauraro_int((long long)(p - str.value.s));
}

TauValue tauraro_str_startswith(TauValue str, TauValue prefix) {
    if (str.type != 2 || !str.value.s) return tauraro_bool(0);
    if (prefix.type != 2 || !prefix.value.s) return tauraro_bool(0);
    size_t plen = strlen(prefix.value.s);
    return tauraro_bool(strncmp(str.value.s, prefix.value.s, plen) == 0);
}

TauValue tauraro_str_endswith(TauValue str, TauValue suffix) {
    if (str.type != 2 || !str.value.s) return tauraro_bool(0);
    if (suffix.type != 2 || !suffix.value.s) return tauraro_bool(0);
    size_t slen = strlen(str.value.s), xlen = strlen(suffix.value.s);
    if (xlen > slen) return tauraro_bool(0);
    return tauraro_bool(strcmp(str.value.s + slen - xlen, suffix.value.s) == 0);
}

TauValue tauraro_str_upper(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    char* result = strdup(str.value.s);
    for (char* p = result; *p; p++) *p = toupper((unsigned char)*p);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_lower(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    char* result = strdup(str.value.s);
    for (char* p = result; *p; p++) *p = tolower((unsigned char)*p);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_strip(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    const char* s = str.value.s;
    while (*s && isspace((unsigned char)*s)) s++;
    if (!*s) return tauraro_str("");
    const char* e = s + strlen(s) - 1;
    while (e > s && isspace((unsigned char)*e)) e--;
    size_t len = e - s + 1;
    char* result = malloc(len + 1);
    memcpy(result, s, len);
    result[len] = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_lstrip(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    const char* s = str.value.s;
    while (*s && isspace((unsigned char)*s)) s++;
    return tauraro_str(s);
}

TauValue tauraro_str_rstrip(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    size_t len = strlen(str.value.s);
    char* result = strdup(str.value.s);
    while (len > 0 && isspace((unsigned char)result[len-1])) len--;
    result[len] = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_title(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    char* result = strdup(str.value.s);
    int cap = 1;
    for (char* p = result; *p; p++) {
        if (isspace((unsigned char)*p)) { cap = 1; }
        else if (cap) { *p = toupper((unsigned char)*p); cap = 0; }
        else { *p = tolower((unsigned char)*p); }
    }
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_capitalize(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    char* result = strdup(str.value.s);
    if (result[0]) result[0] = toupper((unsigned char)result[0]);
    for (char* p = result + 1; *p; p++) *p = tolower((unsigned char)*p);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_swapcase(TauValue str) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    char* result = strdup(str.value.s);
    for (char* p = result; *p; p++) {
        if (isupper((unsigned char)*p)) *p = tolower((unsigned char)*p);
        else if (islower((unsigned char)*p)) *p = toupper((unsigned char)*p);
    }
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_isdigit(TauValue str) {
    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);
    for (const char* p = str.value.s; *p; p++) if (!isdigit((unsigned char)*p)) return tauraro_bool(0);
    return tauraro_bool(1);
}

TauValue tauraro_str_isalpha(TauValue str) {
    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);
    for (const char* p = str.value.s; *p; p++) if (!isalpha((unsigned char)*p)) return tauraro_bool(0);
    return tauraro_bool(1);
}

TauValue tauraro_str_isalnum(TauValue str) {
    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);
    for (const char* p = str.value.s; *p; p++) if (!isalnum((unsigned char)*p)) return tauraro_bool(0);
    return tauraro_bool(1);
}

TauValue tauraro_str_isspace(TauValue str) {
    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);
    for (const char* p = str.value.s; *p; p++) if (!isspace((unsigned char)*p)) return tauraro_bool(0);
    return tauraro_bool(1);
}

TauValue tauraro_str_isupper(TauValue str) {
    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);
    int has_cased = 0;
    for (const char* p = str.value.s; *p; p++) {
        if (islower((unsigned char)*p)) return tauraro_bool(0);
        if (isupper((unsigned char)*p)) has_cased = 1;
    }
    return tauraro_bool(has_cased);
}

TauValue tauraro_str_islower(TauValue str) {
    if (str.type != 2 || !str.value.s || !str.value.s[0]) return tauraro_bool(0);
    int has_cased = 0;
    for (const char* p = str.value.s; *p; p++) {
        if (isupper((unsigned char)*p)) return tauraro_bool(0);
        if (islower((unsigned char)*p)) has_cased = 1;
    }
    return tauraro_bool(has_cased);
}

TauValue tauraro_str_count(TauValue str, TauValue sub) {
    if (str.type != 2 || !str.value.s) return tauraro_int(0);
    if (sub.type != 2 || !sub.value.s || !sub.value.s[0]) return tauraro_int(0);
    long long count = 0;
    size_t sublen = strlen(sub.value.s);
    const char* p = str.value.s;
    while ((p = strstr(p, sub.value.s)) != NULL) { count++; p += sublen; }
    return tauraro_int(count);
}

TauValue tauraro_str_center(TauValue str, TauValue width) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    long long w = width.type == 0 ? width.value.i : 0;
    size_t slen = strlen(str.value.s);
    if (w <= (long long)slen) return str;
    size_t pad = (size_t)w - slen;
    size_t left = pad / 2, right = pad - left;
    char* result = malloc((size_t)w + 1);
    memset(result, ' ', left);
    memcpy(result + left, str.value.s, slen);
    memset(result + left + slen, ' ', right);
    result[w] = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_ljust(TauValue str, TauValue width) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    long long w = width.type == 0 ? width.value.i : 0;
    size_t slen = strlen(str.value.s);
    if (w <= (long long)slen) return str;
    char* result = malloc((size_t)w + 1);
    memcpy(result, str.value.s, slen);
    memset(result + slen, ' ', (size_t)w - slen);
    result[w] = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_rjust(TauValue str, TauValue width) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    long long w = width.type == 0 ? width.value.i : 0;
    size_t slen = strlen(str.value.s);
    if (w <= (long long)slen) return str;
    char* result = malloc((size_t)w + 1);
    size_t pad = (size_t)w - slen;
    memset(result, ' ', pad);
    memcpy(result + pad, str.value.s, slen);
    result[w] = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

TauValue tauraro_str_zfill(TauValue str, TauValue width) {
    if (str.type != 2 || !str.value.s) return tauraro_str("");
    long long w = width.type == 0 ? width.value.i : 0;
    size_t slen = strlen(str.value.s);
    if (w <= (long long)slen) return str;
    char* result = malloc((size_t)w + 1);
    size_t pad = (size_t)w - slen;
    int sign_offset = 0;
    if (str.value.s[0] == '+' || str.value.s[0] == '-') {
        result[0] = str.value.s[0];
        sign_offset = 1;
    }
    memset(result + sign_offset, '0', pad);
    memcpy(result + sign_offset + pad, str.value.s + sign_offset, slen - sign_offset);
    result[w] = '\0';
    return (TauValue){.type = 2, .value.s = result, .refcount = 1};
}

// List manipulation methods (TauValue wrappers)
TauValue tauraro_list_pop_v(TauValue list) {
    if (list.type != 4 || !list.value.list || list.value.list->size == 0)
        return tauraro_none();
    TauList* lst = list.value.list;
    TauValue val = lst->items[lst->size - 1];
    lst->size--;
    return val;
}

TauValue tauraro_list_insert(TauValue list, TauValue index, TauValue value) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* lst = list.value.list;
    long long idx = index.type == 0 ? index.value.i : 0;
    if (idx < 0) idx = (long long)lst->size + idx;
    if (idx < 0) idx = 0;
    if ((size_t)idx > lst->size) idx = (long long)lst->size;
    // Ensure capacity
    if (lst->size >= lst->capacity) {
        lst->capacity = lst->capacity * 2 + 1;
        lst->items = realloc(lst->items, lst->capacity * sizeof(TauValue));
    }
    // Shift elements
    for (size_t i = lst->size; i > (size_t)idx; i--) {
        lst->items[i] = lst->items[i - 1];
    }
    lst->items[idx] = value;
    lst->size++;
    return tauraro_none();
}

TauValue tauraro_list_remove(TauValue list, TauValue value) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* lst = list.value.list;
    for (size_t i = 0; i < lst->size; i++) {
        if (tauraro_value_equals(lst->items[i], value)) {
            for (size_t j = i; j < lst->size - 1; j++) {
                lst->items[j] = lst->items[j + 1];
            }
            lst->size--;
            return tauraro_none();
        }
    }
    return tauraro_none();
}

TauValue tauraro_list_extend_v(TauValue list, TauValue other) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    if (other.type != 4 || !other.value.list) return tauraro_none();
    TauList* lst = list.value.list;
    TauList* ext = other.value.list;
    for (size_t i = 0; i < ext->size; i++) {
        tauraro_list_append(lst, ext->items[i]);
    }
    return tauraro_none();
}

TauValue tauraro_list_clear(TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    list.value.list->size = 0;
    return tauraro_none();
}

TauValue tauraro_list_copy(TauValue list) {
    if (list.type != 4 || !list.value.list) return tauraro_none();
    TauList* src = list.value.list;
    TauList* dst = tauraro_create_list(src->size);
    for (size_t i = 0; i < src->size; i++) {
        tauraro_list_append(dst, src->items[i]);
    }
    return (TauValue){.type = 4, .value.list = dst, .refcount = 1};
}

// Extract raw values from TauValue (for internal use)
static inline long long tauraro_raw_int(TauValue val) {
    switch (val.type) {
        case 0: return val.value.i;
        case 1: return (long long)val.value.f;
        case 3: return val.value.i;
        default: return 0;
    }
}

// Type conversion functions
TauValue tauraro_to_int(TauValue val) {
    switch (val.type) {
        case 0: return val;
        case 1: return tauraro_int((long long)val.value.f);
        case 2: return tauraro_int(val.value.s ? atoll(val.value.s) : 0);
        case 3: return tauraro_int(val.value.i);
        default: return tauraro_int(0);
    }
}

TauValue tauraro_to_float(TauValue val) {
    switch (val.type) {
        case 0: return tauraro_float((double)val.value.i);
        case 1: return val;
        case 2: return tauraro_float(val.value.s ? atof(val.value.s) : 0.0);
        case 3: return tauraro_float((double)val.value.i);
        default: return tauraro_float(0.0);
    }
}

TauValue tauraro_to_str(TauValue val) {
    char* buf = tauraro_format_value(val);
    return (TauValue){.type = 2, .value.s = buf, .refcount = 1};
}

TauValue tauraro_to_bool(TauValue val) {
    switch (val.type) {
        case 0: return tauraro_bool(val.value.i != 0);
        case 1: return tauraro_bool(val.value.f != 0.0);
        case 2: return tauraro_bool(val.value.s && val.value.s[0] != '\0');
        case 3: return val;
        case 4: return tauraro_bool(val.value.list && val.value.list->size > 0);
        default: return tauraro_bool(0);
    }
}

// Input function
TauValue tauraro_input(TauValue prompt) {
    if (prompt.type == 2 && prompt.value.s) {
        printf("%s", prompt.value.s);
        fflush(stdout);
    }
    char buf[4096];
    if (fgets(buf, sizeof(buf), stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        return tauraro_str(buf);
    }
    return tauraro_str("");
}

// Assertion
void tauraro_assert(TauValue condition, TauValue message) {
    int cond = 0;
    if (condition.type == 0) cond = condition.value.i != 0;
    else if (condition.type == 3) cond = condition.value.i != 0;
    else if (condition.type == 2) cond = condition.value.s && condition.value.s[0];
    if (!cond) {
        if (message.type == 2 && message.value.s)
            fprintf(stderr, "AssertionError: %s\n", message.value.s);
        else
            fprintf(stderr, "AssertionError\n");
        exit(1);
    }
}

// Repr function for proper display
TauValue tauraro_repr(TauValue val) {
    char* buf = malloc(4096);
    buf[0] = '\0';
    switch (val.type) {
        case 0: snprintf(buf, 4096, "%lld", val.value.i); break;
        case 1: snprintf(buf, 4096, "%g", val.value.f); break;
        case 2: snprintf(buf, 4096, "'%s'", val.value.s ? val.value.s : ""); break;
        case 3: snprintf(buf, 4096, "%s", val.value.i ? "True" : "False"); break;
        case 4: {
            strcat(buf, "[");
            TauList* lst = val.value.list;
            if (lst) {
                for (size_t i = 0; i < lst->size && strlen(buf) < 3900; i++) {
                    if (i > 0) strcat(buf, ", ");
                    char* elem = tauraro_format_value(lst->items[i]);
                    strcat(buf, elem);
                    free(elem);
                }
            }
            strcat(buf, "]");
            break;
        }
        case 5: snprintf(buf, 4096, "<dict>"); break;
        case 6: snprintf(buf, 4096, "<object>"); break;
        default: snprintf(buf, 4096, "<unknown>"); break;
    }
    return (TauValue){.type = 2, .value.s = buf, .refcount = 1};
}

TauValue tauraro_enumerate_list(TauValue list, TauValue start_val) {
    if (list.type != 4 || !list.value.list) return (TauValue){.type = 4, .value.list = tauraro_create_list(0)};
    TauList* src = list.value.list;
    long long start = start_val.type == 0 ? start_val.value.i : 0;
    TauList* dst = tauraro_create_list(src->size);
    for (size_t i = 0; i < src->size; i++) {
        TauList* tuple = tauraro_create_list(2);
        tauraro_list_append(tuple, tauraro_int(start + (long long)i));
        tauraro_list_append(tuple, src->items[i]);
        tauraro_list_append(dst, (TauValue){.type = 4, .value.list = tuple});
    }
    return (TauValue){.type = 4, .value.list = dst, .refcount = 1};
}

TauValue tauraro_zip_lists(TauValue list1, TauValue list2) {
    if (list1.type != 4 || !list1.value.list || list2.type != 4 || !list2.value.list)
        return (TauValue){.type = 4, .value.list = tauraro_create_list(0)};
    TauList* src1 = list1.value.list;
    TauList* src2 = list2.value.list;
    size_t min_size = src1->size < src2->size ? src1->size : src2->size;
    TauList* dst = tauraro_create_list(min_size);
    for (size_t i = 0; i < min_size; i++) {
        TauList* tuple = tauraro_create_list(2);
        tauraro_list_append(tuple, src1->items[i]);
        tauraro_list_append(tuple, src2->items[i]);
        tauraro_list_append(dst, (TauValue){.type = 4, .value.list = tuple});
    }
    return (TauValue){.type = 4, .value.list = dst, .refcount = 1};
}

TauValue tauraro_type_name(TauValue val) {
    const char* names[] = {"int", "float", "str", "bool", "list", "dict", "object", "function", "exception", "module", "none"};
    int idx = val.type < 11 ? val.type : 10;
    return tauraro_str(names[idx]);
}

TauValue tauraro_isinstance(TauValue obj, TauValue type_str) {
    if (type_str.type != 2 || !type_str.value.s) return tauraro_bool(0);
    const char* t = type_str.value.s;
    if (strcmp(t, "int") == 0) return tauraro_bool(obj.type == 0);
    if (strcmp(t, "float") == 0) return tauraro_bool(obj.type == 1);
    if (strcmp(t, "str") == 0) return tauraro_bool(obj.type == 2);
    if (strcmp(t, "bool") == 0) return tauraro_bool(obj.type == 3);
    if (strcmp(t, "list") == 0) return tauraro_bool(obj.type == 4);
    if (strcmp(t, "dict") == 0) return tauraro_bool(obj.type == 5);
    return tauraro_bool(0);
}

TauValue tauraro_ord(TauValue ch) {
    if (ch.type == 2 && ch.value.s && ch.value.s[0]) {
        return tauraro_int((unsigned char)ch.value.s[0]);
    }
    return tauraro_int(0);
}

TauValue tauraro_chr(TauValue num) {
    char buf[2] = {0};
    if (num.type == 0 && num.value.i >= 0 && num.value.i <= 127) {
        buf[0] = (char)num.value.i;
    }
    return tauraro_str(buf);
}

TauValue tauraro_round(TauValue num, TauValue places) {
    double val = num.type == 0 ? (double)num.value.i : (num.type == 1 ? num.value.f : 0.0);
    long long p = places.type == 0 ? places.value.i : 0;
    if (p == 0) return tauraro_int((long long)round(val));
    double mult = pow(10.0, (double)p);
    return tauraro_float(round(val * mult) / mult);
}

TauValue tauraro_pow(TauValue base, TauValue exp) {
    double b = base.type == 0 ? (double)base.value.i : (base.type == 1 ? base.value.f : 0.0);
    double e = exp.type == 0 ? (double)exp.value.i : (exp.type == 1 ? exp.value.f : 0.0);
    double result = pow(b, e);
    if (base.type == 0 && exp.type == 0 && exp.value.i >= 0) return tauraro_int((long long)result);
    return tauraro_float(result);
}

TauValue tauraro_sqrt(TauValue num) {
    double val = num.type == 0 ? (double)num.value.i : (num.type == 1 ? num.value.f : 0.0);
    return tauraro_float(sqrt(val));
}

TauValue tauraro_hex(TauValue num) {
    char buf[32];
    long long n = num.type == 0 ? num.value.i : 0;
    if (n >= 0) snprintf(buf, 32, "0x%llx", n);
    else snprintf(buf, 32, "-0x%llx", -n);
    return tauraro_str(buf);
}

TauValue tauraro_bin(TauValue num) {
    char buf[72];
    long long n = num.type == 0 ? num.value.i : 0;
    int neg = n < 0;
    if (neg) n = -n;
    char* p = buf + 70;
    *p = '\0';
    if (n == 0) { *--p = '0'; }
    while (n > 0) { *--p = '0' + (n & 1); n >>= 1; }
    *--p = 'b'; *--p = '0';
    if (neg) *--p = '-';
    return tauraro_str(p);
}

TauValue tauraro_oct(TauValue num) {
    char buf[32];
    long long n = num.type == 0 ? num.value.i : 0;
    if (n >= 0) snprintf(buf, 32, "0o%llo", n);
    else snprintf(buf, 32, "-0o%llo", -n);
    return tauraro_str(buf);
}

TauValue tauraro_divmod(TauValue a, TauValue b) {
    long long av = a.type == 0 ? a.value.i : (long long)a.value.f;
    long long bv = b.type == 0 ? b.value.i : (long long)b.value.f;
    if (bv == 0) bv = 1;
    TauList* result = tauraro_create_list(2);
    tauraro_list_append(result, tauraro_int(av / bv));
    tauraro_list_append(result, tauraro_int(av % bv));
    return (TauValue){.type = 4, .value.list = result, .refcount = 1};
}

TauValue tauraro_to_list(TauValue val) {
    if (val.type == 4) return val;
    if (val.type == 2 && val.value.s) {
        size_t len = strlen(val.value.s);
        TauList* lst = tauraro_create_list(len);
        for (size_t i = 0; i < len; i++) {
            char c[2] = {val.value.s[i], '\0'};
            tauraro_list_append(lst, tauraro_str(c));
        }
        return (TauValue){.type = 4, .value.list = lst, .refcount = 1};
    }
    return (TauValue){.type = 4, .value.list = tauraro_create_list(0)};
}

TauValue tauraro_to_set(TauValue val) {
    TauDict* dict = tauraro_create_dict();
    if (val.type == 4 && val.value.list) {
        TauList* lst = val.value.list;
        for (size_t i = 0; i < lst->size; i++) {
            if (lst->items[i].type == 2 && lst->items[i].value.s) {
                tauraro_dict_set(dict, lst->items[i].value.s, tauraro_none());
            }
        }
    }
    return (TauValue){.type = 5, .value.dict = dict, .refcount = 1};
}

// Memory management structures
typedef struct tauraro_arena {
    void** buffers;
    size_t* sizes;
    size_t count;
    size_t capacity;
    char* name;
} tauraro_arena_t;

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

TauValue tauraro_allocate(int argc, TauValue** args) {
    if (argc != 1 || (*args)->type != 0) return tauraro_none();
    tauraro_memory_init();
    size_t size = (size_t)(*args)->value.i;
    void* buffer = malloc(size);
    if (!buffer) return tauraro_none();
    if (tauraro_memory_state.buffer_count >= tauraro_memory_state.buffer_capacity) {
        tauraro_memory_state.buffer_capacity *= 2;
        tauraro_memory_state.manual_buffers = (void**)realloc(tauraro_memory_state.manual_buffers, sizeof(void*) * tauraro_memory_state.buffer_capacity);
        tauraro_memory_state.buffer_sizes = (size_t*)realloc(tauraro_memory_state.buffer_sizes, sizeof(size_t) * tauraro_memory_state.buffer_capacity);
    }
    size_t idx = tauraro_memory_state.buffer_count++;
    tauraro_memory_state.manual_buffers[idx] = buffer;
    tauraro_memory_state.buffer_sizes[idx] = size;
    return tauraro_int((long long)(uintptr_t)buffer);
}

TauValue tauraro_free(int argc, TauValue** args) {
    if (argc != 1) return tauraro_none();
    void* ptr = (void*)(uintptr_t)(*args)->value.i;
    if (ptr) {
        for (size_t i = 0; i < tauraro_memory_state.buffer_count; i++) {
            if (tauraro_memory_state.manual_buffers[i] == ptr) {
                free(ptr);
                for (size_t j = i; j < tauraro_memory_state.buffer_count - 1; j++) {
                    tauraro_memory_state.manual_buffers[j] = tauraro_memory_state.manual_buffers[j + 1];
                    tauraro_memory_state.buffer_sizes[j] = tauraro_memory_state.buffer_sizes[j + 1];
                }
                tauraro_memory_state.buffer_count--;
                break;
            }
        }
    }
    return tauraro_none();
}

TauValue tauraro_create_arena(int argc, TauValue** args) {
    if (argc != 1 || (*args)->type != 2) return tauraro_none();
    tauraro_memory_init();
    char* name = (*args)->value.s;
    tauraro_arena_t* arena = (tauraro_arena_t*)malloc(sizeof(tauraro_arena_t));
    arena->name = strdup(name);
    arena->capacity = 64;
    arena->count = 0;
    arena->buffers = (void**)malloc(sizeof(void*) * arena->capacity);
    arena->sizes = (size_t*)malloc(sizeof(size_t) * arena->capacity);
    if (tauraro_memory_state.arena_count >= tauraro_memory_state.arena_capacity) {
        tauraro_memory_state.arena_capacity *= 2;
        tauraro_memory_state.arenas = (tauraro_arena_t**)realloc(tauraro_memory_state.arenas, sizeof(tauraro_arena_t*) * tauraro_memory_state.arena_capacity);
    }
    tauraro_memory_state.arenas[tauraro_memory_state.arena_count++] = arena;
    if (tauraro_memory_state.current_arena) free(tauraro_memory_state.current_arena);
    tauraro_memory_state.current_arena = strdup(name);
    return tauraro_none();
}

TauValue tauraro_destroy_arena(int argc, TauValue** args) {
    if (argc != 1 || (*args)->type != 2) return tauraro_none();
    char* name = (*args)->value.s;
    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++) {
        if (strcmp(tauraro_memory_state.arenas[i]->name, name) == 0) {
            tauraro_arena_t* arena = tauraro_memory_state.arenas[i];
            for (size_t j = 0; j < arena->count; j++) free(arena->buffers[j]);
            free(arena->buffers);
            free(arena->sizes);
            free(arena->name);
            free(arena);
            for (size_t j = i; j < tauraro_memory_state.arena_count - 1; j++)
                tauraro_memory_state.arenas[j] = tauraro_memory_state.arenas[j + 1];
            tauraro_memory_state.arena_count--;
            break;
        }
    }
    return tauraro_none();
}

TauValue tauraro_reset_arena(int argc, TauValue** args) {
    if (argc != 1 || (*args)->type != 2) return tauraro_none();
    char* name = (*args)->value.s;
    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++) {
        if (strcmp(tauraro_memory_state.arenas[i]->name, name) == 0) {
            tauraro_arena_t* arena = tauraro_memory_state.arenas[i];
            for (size_t j = 0; j < arena->count; j++) free(arena->buffers[j]);
            arena->count = 0;
            break;
        }
    }
    return tauraro_none();
}

TauValue tauraro_memory_stats(int argc, TauValue** args) {
    (void)argc; (void)args;
    tauraro_memory_init();
    size_t total_manual = 0;
    for (size_t i = 0; i < tauraro_memory_state.buffer_count; i++)
        total_manual += tauraro_memory_state.buffer_sizes[i];
    size_t total_arena = 0;
    for (size_t i = 0; i < tauraro_memory_state.arena_count; i++)
        for (size_t j = 0; j < tauraro_memory_state.arenas[i]->count; j++)
            total_arena += tauraro_memory_state.arenas[i]->sizes[j];
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "Memory Strategy: Manual\nManual Buffers: %zu (%zu bytes)\nArenas: %zu (%zu bytes)",
        tauraro_memory_state.buffer_count, total_manual, tauraro_memory_state.arena_count, total_arena);
    return tauraro_str(buffer);
}

TauValue tauraro_sizeof(int argc, TauValue** args) {
    if (argc != 1) return tauraro_int(0);
    // Parse string type names
    if ((*args)->type == 2 && (*args)->value.s) {
        const char* t = (*args)->value.s;
        if (strcmp(t, "int") == 0 || strcmp(t, "int32") == 0) return tauraro_int(4);
        if (strcmp(t, "int8") == 0 || strcmp(t, "char") == 0 || strcmp(t, "byte") == 0) return tauraro_int(1);
        if (strcmp(t, "int16") == 0 || strcmp(t, "short") == 0) return tauraro_int(2);
        if (strcmp(t, "int64") == 0 || strcmp(t, "long") == 0) return tauraro_int(8);
        if (strcmp(t, "float") == 0 || strcmp(t, "float32") == 0) return tauraro_int(4);
        if (strcmp(t, "float64") == 0 || strcmp(t, "double") == 0) return tauraro_int(8);
        if (strcmp(t, "pointer") == 0 || strcmp(t, "ptr") == 0) return tauraro_int(sizeof(void*));
        if (strcmp(t, "bool") == 0) return tauraro_int(1);
        return tauraro_int(sizeof(void*));
    }
    switch ((*args)->type) {
        case 0: return tauraro_int(sizeof(long long));
        case 1: return tauraro_int(sizeof(double));
        case 3: return tauraro_int(sizeof(int));
        default: return tauraro_int(sizeof(TauValue));
    }
}

TauValue tauraro_alignof(int argc, TauValue** args) {
    if (argc != 1) return tauraro_int(0);
    // Parse string type names
    if ((*args)->type == 2 && (*args)->value.s) {
        const char* t = (*args)->value.s;
        if (strcmp(t, "int8") == 0 || strcmp(t, "char") == 0 || strcmp(t, "byte") == 0 || strcmp(t, "bool") == 0) return tauraro_int(1);
        if (strcmp(t, "int16") == 0 || strcmp(t, "short") == 0) return tauraro_int(2);
        if (strcmp(t, "int") == 0 || strcmp(t, "int32") == 0 || strcmp(t, "float") == 0 || strcmp(t, "float32") == 0) return tauraro_int(4);
        return tauraro_int(8);
    }
    switch ((*args)->type) {
        case 0: return tauraro_int(_Alignof(long long));
        case 1: return tauraro_int(_Alignof(double));
        default: return tauraro_int(_Alignof(void*));
    }
}

TauValue tauraro_memcpy(int argc, TauValue** args) {
    if (argc != 3) return tauraro_none();
    void* dest = (void*)(uintptr_t)args[0]->value.i;
    void* src = (void*)(uintptr_t)args[1]->value.i;
    size_t n = (size_t)args[2]->value.i;
    if (dest && src && n > 0) memcpy(dest, src, n);
    return tauraro_int((long long)(uintptr_t)dest);
}

TauValue tauraro_memset(int argc, TauValue** args) {
    if (argc != 3) return tauraro_none();
    void* dest = (void*)(uintptr_t)args[0]->value.i;
    int value = (int)args[1]->value.i;
    size_t n = (size_t)args[2]->value.i;
    if (dest && n > 0) memset(dest, value, n);
    return tauraro_int((long long)(uintptr_t)dest);
}

TauValue tauraro_memmove(int argc, TauValue** args) {
    if (argc != 3) return tauraro_none();
    void* dest = (void*)(uintptr_t)args[0]->value.i;
    void* src = (void*)(uintptr_t)args[1]->value.i;
    size_t n = (size_t)args[2]->value.i;
    if (dest && src && n > 0) memmove(dest, src, n);
    return tauraro_int((long long)(uintptr_t)dest);
}

TauValue tauraro_memcmp(int argc, TauValue** args) {
    if (argc != 3) return tauraro_int(-1);
    void* s1 = (void*)(uintptr_t)args[0]->value.i;
    void* s2 = (void*)(uintptr_t)args[1]->value.i;
    size_t n = (size_t)args[2]->value.i;
    if (s1 && s2 && n > 0) return tauraro_int(memcmp(s1, s2, n));
    return tauraro_int(-1);
}

TauValue tauraro_ptr_read(int argc, TauValue** args) {
    if (argc < 1) return tauraro_none();
    void* ptr = (void*)(uintptr_t)args[0]->value.i;
    int byte_size = (argc > 1) ? (int)args[1]->value.i : 8;
    if (!ptr) return tauraro_int(0);
    switch (byte_size) {
        case 1: return tauraro_int(*(int8_t*)ptr);
        case 2: return tauraro_int(*(int16_t*)ptr);
        case 4: return tauraro_int(*(int32_t*)ptr);
        default: return tauraro_int(*(int64_t*)ptr);
    }
}

TauValue tauraro_ptr_write(int argc, TauValue** args) {
    if (argc < 2) return tauraro_none();
    void* ptr = (void*)(uintptr_t)args[0]->value.i;
    long long value = args[1]->value.i;
    int byte_size = (argc > 2) ? (int)args[2]->value.i : 8;
    if (ptr) {
        switch (byte_size) {
            case 1: *(int8_t*)ptr = (int8_t)value; break;
            case 2: *(int16_t*)ptr = (int16_t)value; break;
            case 4: *(int32_t*)ptr = (int32_t)value; break;
            default: *(int64_t*)ptr = (int64_t)value; break;
        }
    }
    return tauraro_none();
}

TauValue tauraro_ptr_offset(int argc, TauValue** args) {
    if (argc != 2) return tauraro_int(0);
    uintptr_t ptr = (uintptr_t)args[0]->value.i;
    long long offset = args[1]->value.i;
    return tauraro_int((long long)(ptr + offset));
}

TauValue tauraro_null_ptr(int argc, TauValue** args) {
    (void)argc; (void)args;
    return tauraro_int(0);
}

TauValue tauraro_is_null(int argc, TauValue** args) {
    if (argc != 1) return tauraro_bool(1);
    return tauraro_bool(args[0]->value.i == 0);
}

// Advanced System Programming Functions

TauValue tauraro_stack_alloc(int argc, TauValue** args) {
    if (argc != 1) return tauraro_int(0);
    size_t size = (size_t)args[0]->value.i;
#if defined(_MSC_VER)
    void* ptr = _alloca(size);
#elif defined(__GNUC__) || defined(__clang__)
    void* ptr = __builtin_alloca(size);
#else
    void* ptr = malloc(size); // Fallback to heap
#endif
    return tauraro_int((int64_t)(intptr_t)ptr);
}

TauValue tauraro_volatile_read(int argc, TauValue** args) {
    if (argc != 1) return tauraro_int(0);
    volatile int64_t* ptr = (volatile int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(*ptr);
}

TauValue tauraro_volatile_write(int argc, TauValue** args) {
    if (argc != 2) return (TauValue){.type = 5};
    volatile int64_t* ptr = (volatile int64_t*)(intptr_t)args[0]->value.i;
    if (ptr != NULL) *ptr = args[1]->value.i;
    return (TauValue){.type = 5};
}

TauValue tauraro_atomic_load(int argc, TauValue** args) {
    if (argc != 1) return tauraro_int(0);
#if defined(__GNUC__) || defined(__clang__)
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(__atomic_load_n(ptr, __ATOMIC_SEQ_CST));
#elif defined(_MSC_VER)
    volatile int64_t* ptr = (volatile int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(_InterlockedCompareExchange64(ptr, 0, 0));
#else
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(*ptr);
#endif
}

TauValue tauraro_atomic_store(int argc, TauValue** args) {
    if (argc != 2) return (TauValue){.type = 5};
#if defined(__GNUC__) || defined(__clang__)
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr != NULL) __atomic_store_n(ptr, args[1]->value.i, __ATOMIC_SEQ_CST);
#elif defined(_MSC_VER)
    volatile int64_t* ptr = (volatile int64_t*)(intptr_t)args[0]->value.i;
    if (ptr != NULL) _InterlockedExchange64(ptr, args[1]->value.i);
#else
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr != NULL) *ptr = args[1]->value.i;
#endif
    return (TauValue){.type = 5};
}

TauValue tauraro_atomic_add(int argc, TauValue** args) {
    if (argc != 2) return tauraro_int(0);
#if defined(__GNUC__) || defined(__clang__)
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(__atomic_fetch_add(ptr, args[1]->value.i, __ATOMIC_SEQ_CST));
#elif defined(_MSC_VER)
    volatile int64_t* ptr = (volatile int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(_InterlockedExchangeAdd64(ptr, args[1]->value.i));
#else
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    int64_t old = *ptr; *ptr += args[1]->value.i; return tauraro_int(old);
#endif
}

TauValue tauraro_atomic_sub(int argc, TauValue** args) {
    if (argc != 2) return tauraro_int(0);
#if defined(__GNUC__) || defined(__clang__)
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(__atomic_fetch_sub(ptr, args[1]->value.i, __ATOMIC_SEQ_CST));
#elif defined(_MSC_VER)
    volatile int64_t* ptr = (volatile int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    return tauraro_int(_InterlockedExchangeAdd64(ptr, -args[1]->value.i));
#else
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_int(0);
    int64_t old = *ptr; *ptr -= args[1]->value.i; return tauraro_int(old);
#endif
}

TauValue tauraro_atomic_cas(int argc, TauValue** args) {
    if (argc != 3) return tauraro_bool(0);
#if defined(__GNUC__) || defined(__clang__)
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_bool(0);
    int64_t expected = args[1]->value.i;
    return tauraro_bool(__atomic_compare_exchange_n(ptr, &expected, args[2]->value.i, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST));
#elif defined(_MSC_VER)
    volatile int64_t* ptr = (volatile int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_bool(0);
    int64_t result = _InterlockedCompareExchange64(ptr, args[2]->value.i, args[1]->value.i);
    return tauraro_bool(result == args[1]->value.i);
#else
    int64_t* ptr = (int64_t*)(intptr_t)args[0]->value.i;
    if (ptr == NULL) return tauraro_bool(0);
    if (*ptr == args[1]->value.i) { *ptr = args[2]->value.i; return tauraro_bool(1); }
    return tauraro_bool(0);
#endif
}

TauValue tauraro_memory_barrier(int argc, TauValue** args) {
    (void)argc; (void)args;
#if defined(__GNUC__) || defined(__clang__)
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
#elif defined(_MSC_VER)
    _ReadWriteBarrier(); MemoryBarrier();
#endif
    return (TauValue){.type = 5};
}

TauValue tauraro_prefetch(int argc, TauValue** args) {
    if (argc != 1) return (TauValue){.type = 5};
    void* ptr = (void*)(intptr_t)args[0]->value.i;
#if defined(__GNUC__) || defined(__clang__)
    __builtin_prefetch(ptr, 0, 3); // Read, high temporal locality
#elif defined(_MSC_VER)
    _mm_prefetch((char*)ptr, _MM_HINT_T0);
#endif
    return (TauValue){.type = 5};
}

TauValue tauraro_cache_line_size(int argc, TauValue** args) {
    (void)argc; (void)args;
    return tauraro_int(64); // Most modern CPUs use 64-byte cache lines
}

TauValue tauraro_bit_cast(int argc, TauValue** args) {
    if (argc != 2) return tauraro_int(0);
    const char* target = args[1]->value.s;
    if (strcmp(target, "int") == 0) {
        if (args[0]->type == 1) { // Float
            union { double d; int64_t i; } u;
            u.d = args[0]->value.f;
            return tauraro_int(u.i);
        }
        return tauraro_int(args[0]->value.i);
    } else if (strcmp(target, "float") == 0) {
        if (args[0]->type == 0) { // Int
            union { double d; int64_t i; } u;
            u.i = args[0]->value.i;
            return tauraro_float(u.d);
        }
        return tauraro_float(args[0]->value.f);
    }
    return tauraro_int(0);
}

TauValue tauraro_zero_memory(int argc, TauValue** args) {
    if (argc != 2) return (TauValue){.type = 5};
    void* ptr = (void*)(intptr_t)args[0]->value.i;
    size_t size = (size_t)args[1]->value.i;
    if (ptr != NULL && size > 0) memset(ptr, 0, size);
    return (TauValue){.type = 5};
}

TauValue tauraro_copy_memory(int argc, TauValue** args) {
    if (argc != 3) return (TauValue){.type = 5};
    void* dest = (void*)(intptr_t)args[0]->value.i;
    void* src = (void*)(intptr_t)args[1]->value.i;
    size_t size = (size_t)args[2]->value.i;
    if (dest != NULL && src != NULL && size > 0) memcpy(dest, src, size);
    return (TauValue){.type = 5};
}

TauValue tauraro_compare_memory(int argc, TauValue** args) {
    if (argc != 3) return tauraro_int(0);
    void* ptr1 = (void*)(intptr_t)args[0]->value.i;
    void* ptr2 = (void*)(intptr_t)args[1]->value.i;
    size_t size = (size_t)args[2]->value.i;
    if (ptr1 == NULL || ptr2 == NULL || size == 0) return tauraro_int(0);
    return tauraro_int(memcmp(ptr1, ptr2, size));
}

// File I/O support
TauValue open(TauValue filename, TauValue mode) {
    if (filename.type != 2 || mode.type != 2) return tauraro_none();
    FILE* fp = fopen(filename.value.s, mode.value.s);
    if (!fp) return tauraro_none();
    // Store FILE* as integer pointer
    return tauraro_int((long long)(uintptr_t)fp);
}

TauValue f__write(TauValue file, TauValue data) {
    if (file.type != 0 || data.type != 2) return tauraro_none();
    FILE* fp = (FILE*)(uintptr_t)file.value.i;
    if (!fp) return tauraro_none();
    fputs(data.value.s, fp);
    return tauraro_none();
}

TauValue f__read(TauValue file) {
    if (file.type != 0) return tauraro_str("");
    FILE* fp = (FILE*)(uintptr_t)file.value.i;
    if (!fp) return tauraro_str("");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) return tauraro_str("");
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    TauValue result = tauraro_str(buffer);
    free(buffer);
    return result;
}

TauValue f__close(TauValue file) {
    if (file.type != 0) return tauraro_none();
    FILE* fp = (FILE*)(uintptr_t)file.value.i;
    if (fp) fclose(fp);
    return tauraro_none();
}

// ===== FFI (FOREIGN FUNCTION INTERFACE) =====

// Platform-specific library loading
#ifdef _WIN32
    #include <windows.h>
    typedef HMODULE library_handle_t;
    #define LOAD_LIBRARY(name) LoadLibraryA(name)
    #define GET_FUNCTION(handle, name) GetProcAddress(handle, name)
    #define CLOSE_LIBRARY(handle) FreeLibrary(handle)
    #define LIBRARY_ERROR() "Windows library error"
#else
    #include <dlfcn.h>
    typedef void* library_handle_t;
    #define LOAD_LIBRARY(name) dlopen(name, RTLD_LAZY)
    #define GET_FUNCTION(handle, name) dlsym(handle, name)
    #define CLOSE_LIBRARY(handle) dlclose(handle)
    #define LIBRARY_ERROR() dlerror()
#endif

// FFI library structure
typedef struct {
    char* name;
    library_handle_t handle;
} ffi_library_t;

// FFI function structure
typedef struct {
    char* name;
    void* func_ptr;
    char* return_type;
    char** param_types;
    int param_count;
} ffi_function_t;

// Global FFI state
static ffi_library_t** ffi_libraries = NULL;
static int ffi_library_count = 0;
static int ffi_library_capacity = 0;

static ffi_function_t** ffi_functions = NULL;
static int ffi_function_count = 0;
static int ffi_function_capacity = 0;

// Find library by name
static ffi_library_t* find_ffi_library(const char* name) {
    for (int i = 0; i < ffi_library_count; i++) {
        if (strcmp(ffi_libraries[i]->name, name) == 0) {
            return ffi_libraries[i];
        }
    }
    return NULL;
}

// Find function by name
static ffi_function_t* find_ffi_function(const char* name) {
    for (int i = 0; i < ffi_function_count; i++) {
        if (strcmp(ffi_functions[i]->name, name) == 0) {
            return ffi_functions[i];
        }
    }
    return NULL;
}

TauValue load_library(TauValue library_name_val) {
    if (library_name_val.type != 2) {
        fprintf(stderr, "load_library() requires 1 string argument\n");
        return tauraro_bool(0);
    }

    const char* library_name = library_name_val.value.s;

    if (find_ffi_library(library_name) != NULL) {
        return tauraro_bool(1);
    }

    library_handle_t handle = LOAD_LIBRARY(library_name);
    if (handle == NULL) {
        fprintf(stderr, "Failed to load library %s: %s\n", library_name, LIBRARY_ERROR());
        return tauraro_bool(0);
    }

    if (ffi_libraries == NULL) {
        ffi_library_capacity = 16;
        ffi_libraries = (ffi_library_t**)malloc(sizeof(ffi_library_t*) * ffi_library_capacity);
    }

    ffi_library_t* lib = (ffi_library_t*)malloc(sizeof(ffi_library_t));
    lib->name = strdup(library_name);
    lib->handle = handle;

    if (ffi_library_count >= ffi_library_capacity) {
        ffi_library_capacity *= 2;
        ffi_libraries = (ffi_library_t**)realloc(ffi_libraries, sizeof(ffi_library_t*) * ffi_library_capacity);
    }
    ffi_libraries[ffi_library_count++] = lib;
    return tauraro_bool(1);
}

TauValue define_function(TauValue lib_name, TauValue func_name, TauValue ret_type, TauValue param_types) {
    if (lib_name.type != 2 || func_name.type != 2 || ret_type.type != 2) {
        fprintf(stderr, "define_function() first 3 args must be strings\n");
        return tauraro_none();
    }

    const char* library_name = lib_name.value.s;
    const char* function_name = func_name.value.s;
    const char* return_type = ret_type.value.s;

    ffi_library_t* lib = find_ffi_library(library_name);
    if (lib == NULL) {
        fprintf(stderr, "Library not loaded: %s\n", library_name);
        return tauraro_none();
    }

    void* func_ptr = GET_FUNCTION(lib->handle, function_name);
    if (func_ptr == NULL) {
        fprintf(stderr, "Function not found: %s in %s: %s\n", function_name, library_name, LIBRARY_ERROR());
        return tauraro_none();
    }

    if (ffi_functions == NULL) {
        ffi_function_capacity = 64;
        ffi_functions = (ffi_function_t**)malloc(sizeof(ffi_function_t*) * ffi_function_capacity);
    }

    ffi_function_t* func = (ffi_function_t*)malloc(sizeof(ffi_function_t));
    func->name = strdup(function_name);
    func->func_ptr = func_ptr;
    func->return_type = strdup(return_type);

    if (param_types.type == 4) {
        TauList* param_list = param_types.value.list;
        func->param_count = param_list->size;
        if (func->param_count > 0) {
            func->param_types = (char**)malloc(sizeof(char*) * func->param_count);
            for (int i = 0; i < func->param_count; i++) {
                if (param_list->items[i].type == 2) {
                    func->param_types[i] = strdup(param_list->items[i].value.s);
                } else {
                    func->param_types[i] = strdup("int");
                }
            }
        } else {
            func->param_types = NULL;
        }
    } else {
        func->param_count = 0;
        func->param_types = NULL;
    }

    if (ffi_function_count >= ffi_function_capacity) {
        ffi_function_capacity *= 2;
        ffi_functions = (ffi_function_t**)realloc(ffi_functions, sizeof(ffi_function_t*) * ffi_function_capacity);
    }
    ffi_functions[ffi_function_count++] = func;
    return tauraro_none();
}

TauValue call_function(TauValue func_name, TauValue arg1) {
    if (func_name.type != 2) {
        fprintf(stderr, "call_function() requires function name as first argument\n");
        return tauraro_none();
    }

    const char* function_name = func_name.value.s;
    ffi_function_t* func = find_ffi_function(function_name);
    if (func == NULL) {
        fprintf(stderr, "Function not defined: %s\n", function_name);
        return tauraro_none();
    }

    long long arg_value = 0;
    if (arg1.type == 0) {
        arg_value = (long long)arg1.value.i;
    } else if (arg1.type == 1) {
        *((double*)&arg_value) = arg1.value.f;
    } else if (arg1.type == 2) {
        arg_value = (long long)arg1.value.s;
    }

    typedef long long (*func_t)(long long);
    long long ret_int = ((func_t)func->func_ptr)(arg_value);

    if (strcmp(func->return_type, "void") == 0) {
        return tauraro_none();
    } else if (strcmp(func->return_type, "int") == 0 || strcmp(func->return_type, "int32") == 0 || strcmp(func->return_type, "int64") == 0) {
        return tauraro_int((int)ret_int);
    } else if (strcmp(func->return_type, "float") == 0 || strcmp(func->return_type, "double") == 0) {
        return tauraro_float((double)ret_int);
    } else if (strcmp(func->return_type, "string") == 0 || strcmp(func->return_type, "char*") == 0) {
        TauValue result = tauraro_none();
        result.type = 2;
        result.value.s = (char*)ret_int;
        return result;
    } else {
        return tauraro_int((int)ret_int);
    }
}

// ===== BARE-METAL / OS DEVELOPMENT STUBS =====

#ifndef TAURARO_FREESTANDING
// Stub implementations for user-mode (non-bare-metal) compilation

static inline uint8_t inb(uint16_t port) { (void)port; return 0; }
static inline void outb(uint16_t port, uint8_t val) { (void)port; (void)val; }
static inline uint16_t inw(uint16_t port) { (void)port; return 0; }
static inline void outw(uint16_t port, uint16_t val) { (void)port; (void)val; }
static inline uint32_t inl(uint16_t port) { (void)port; return 0; }
static inline void outl(uint16_t port, uint32_t val) { (void)port; (void)val; }

static inline uint8_t mmio_read8(uintptr_t addr) { (void)addr; return 0; }
static inline void mmio_write8(uintptr_t addr, uint8_t val) { (void)addr; (void)val; }
static inline uint16_t mmio_read16(uintptr_t addr) { (void)addr; return 0; }
static inline void mmio_write16(uintptr_t addr, uint16_t val) { (void)addr; (void)val; }
static inline uint32_t mmio_read32(uintptr_t addr) { (void)addr; return 0; }
static inline void mmio_write32(uintptr_t addr, uint32_t val) { (void)addr; (void)val; }
static inline uint64_t mmio_read64(uintptr_t addr) { (void)addr; return 0; }
static inline void mmio_write64(uintptr_t addr, uint64_t val) { (void)addr; (void)val; }

static inline void cli(void) { }
static inline void sti(void) { }
static inline void hlt(void) { }

static inline uint64_t read_cr0(void) { return 0; }
static inline void write_cr0(uint64_t val) { (void)val; }
static inline uint64_t read_cr3(void) { return 0; }
static inline void write_cr3(uint64_t val) { (void)val; }
static inline uint64_t read_msr(uint32_t msr) { (void)msr; return 0; }
static inline void write_msr(uint32_t msr, uint64_t val) { (void)msr; (void)val; }

#endif // TAURARO_FREESTANDING


// ===== SYS MODULE INIT =====
// Initialize sys module and populate it with system information
TauModule* tauraro_init_sys_module(int argc, char* argv[]) {
    TauModule* sys_mod = tauraro_create_module("sys", NULL);
    if (!sys_mod) return NULL;

    // Create sys.argv as a TauList
    TauList* argv_list = tauraro_create_list(argc > 0 ? argc : 1);
    if (argv_list) {
        for (int i = 0; i < argc; i++) {
            TauValue arg_val = (TauValue){
                .type = 2,  // String type
                .value.s = strdup(argv[i]),
                .refcount = 1,
                .next = NULL
            };
            tauraro_list_append(argv_list, arg_val);
        }
    }

    // Add argv to module exports
    TauValue argv_value = (TauValue){
        .type = 4,  // List type
        .value.list = argv_list,
        .refcount = 1,
        .next = NULL
    };
    tauraro_dict_set(sys_mod->exports, "argv", argv_value);

    // Create sys.platform string
    const char* platform_str;
    #ifdef _WIN32
        platform_str = "win32";
    #elif __linux__
        platform_str = "linux";
    #elif __APPLE__
        platform_str = "darwin";
    #elif __FreeBSD__
        platform_str = "freebsd";
    #else
        platform_str = "unknown";
    #endif

    TauValue platform_value = (TauValue){
        .type = 2,  // String type
        .value.s = strdup(platform_str),
        .refcount = 1,
        .next = NULL
    };
    tauraro_dict_set(sys_mod->exports, "platform", platform_value);

    // Create sys.version string
    TauValue version_value = (TauValue){
        .type = 2,  // String type
        .value.s = strdup("Tauraro 0.2.0"),
        .refcount = 1,
        .next = NULL
    };
    tauraro_dict_set(sys_mod->exports, "version", version_value);

    // Create sys.path (empty list for now)
    TauList* path_list = tauraro_create_list(10);
    TauValue path_value = (TauValue){
        .type = 4,  // List type
        .value.list = path_list,
        .refcount = 1,
        .next = NULL
    };
    tauraro_dict_set(sys_mod->exports, "path", path_value);

    sys_mod->is_loaded = 1;
    return sys_mod;
}

// Function forward declarations
TauValue add(TauValue a, TauValue b);
TauValue multiply(TauValue a, TauValue b);

// Function definitions
TauValue add(TauValue a, TauValue b) {
    TauValue temp_result, binop_right, binop_left;
    binop_left = a;
    binop_right = b;
    // Runtime type checking for + operation
    if ((binop_left.type == 2 || binop_right.type == 2)) {
        char temp_concat_rt_1[512] = {0};
        if (binop_left.type == 2) {
            strcpy(temp_concat_rt_1, binop_left.value.s);
        } else if (binop_left.type == 0) {
            char int_buf[64];
            snprintf(int_buf, sizeof(int_buf), "%lld", binop_left.value.i);
            strcpy(temp_concat_rt_1, int_buf);
        } else if (binop_left.type == 3) {
            strcpy(temp_concat_rt_1, binop_left.value.i ? "True" : "False");
        } else if (binop_left.type == 1) {
            char float_buf[64];
            snprintf(float_buf, sizeof(float_buf), "%f", binop_left.value.f);
            strcpy(temp_concat_rt_1, float_buf);
        }
        if (binop_right.type == 2) {
            strcat(temp_concat_rt_1, binop_right.value.s);
        } else if (binop_right.type == 0) {
            char int_buf[64];
            snprintf(int_buf, sizeof(int_buf), "%lld", binop_right.value.i);
            strcat(temp_concat_rt_1, int_buf);
        } else if (binop_right.type == 3) {
            strcat(temp_concat_rt_1, binop_right.value.i ? "True" : "False");
        } else if (binop_right.type == 1) {
            char float_buf[64];
            snprintf(float_buf, sizeof(float_buf), "%f", binop_right.value.f);
            strcat(temp_concat_rt_1, float_buf);
        }
    temp_result = (TauValue){.type = 2, .value.s = strdup(temp_concat_rt_1), .refcount = 1};
    } else {
    temp_result = (TauValue){.type = 0, .value.i = binop_left.value.i + binop_right.value.i};
    }
    return temp_result;
}

TauValue multiply(TauValue a, TauValue b) {
    TauValue temp_result, i, binop_left, comp_left, comp_right, binop_right, result;
    temp_result = (TauValue){.type = 0, .value.i = 0};
    result = temp_result;
    temp_result = (TauValue){.type = 0, .value.i = 0};
    i = temp_result;
    comp_left = i;
    comp_right = b;
    temp_result = (TauValue){.type = 0, .value.i = comp_left.value.i < comp_right.value.i};
    while (1) {
        comp_left = i;
        comp_right = b;
        temp_result = (TauValue){.type = 0, .value.i = comp_left.value.i < comp_right.value.i};
        if (!((temp_result.type == 3 ? temp_result.value.i : (temp_result.type == 0 ? (temp_result.value.i != 0) : 1)) )) break;
        binop_left = result;
        binop_right = a;
        // Runtime type checking for + operation
        if ((binop_left.type == 2 || binop_right.type == 2)) {
            char temp_concat_rt_1[512] = {0};
            if (binop_left.type == 2) {
                strcpy(temp_concat_rt_1, binop_left.value.s);
            } else if (binop_left.type == 0) {
                char int_buf[64];
                snprintf(int_buf, sizeof(int_buf), "%lld", binop_left.value.i);
                strcpy(temp_concat_rt_1, int_buf);
            } else if (binop_left.type == 3) {
                strcpy(temp_concat_rt_1, binop_left.value.i ? "True" : "False");
            } else if (binop_left.type == 1) {
                char float_buf[64];
                snprintf(float_buf, sizeof(float_buf), "%f", binop_left.value.f);
                strcpy(temp_concat_rt_1, float_buf);
            }
            if (binop_right.type == 2) {
                strcat(temp_concat_rt_1, binop_right.value.s);
            } else if (binop_right.type == 0) {
                char int_buf[64];
                snprintf(int_buf, sizeof(int_buf), "%lld", binop_right.value.i);
                strcat(temp_concat_rt_1, int_buf);
            } else if (binop_right.type == 3) {
                strcat(temp_concat_rt_1, binop_right.value.i ? "True" : "False");
            } else if (binop_right.type == 1) {
                char float_buf[64];
                snprintf(float_buf, sizeof(float_buf), "%f", binop_right.value.f);
                strcat(temp_concat_rt_1, float_buf);
            }
        temp_result = (TauValue){.type = 2, .value.s = strdup(temp_concat_rt_1), .refcount = 1};
        } else {
        temp_result = (TauValue){.type = 0, .value.i = binop_left.value.i + binop_right.value.i};
        }
        result = temp_result;
        binop_left = i;
        binop_right = (TauValue){.type = 0, .value.i = 1};
        // Runtime type checking for + operation
        if ((binop_left.type == 2 || binop_right.type == 2)) {
            char temp_concat_rt_2[512] = {0};
            if (binop_left.type == 2) {
                strcpy(temp_concat_rt_2, binop_left.value.s);
            } else if (binop_left.type == 0) {
                char int_buf[64];
                snprintf(int_buf, sizeof(int_buf), "%lld", binop_left.value.i);
                strcpy(temp_concat_rt_2, int_buf);
            } else if (binop_left.type == 3) {
                strcpy(temp_concat_rt_2, binop_left.value.i ? "True" : "False");
            } else if (binop_left.type == 1) {
                char float_buf[64];
                snprintf(float_buf, sizeof(float_buf), "%f", binop_left.value.f);
                strcpy(temp_concat_rt_2, float_buf);
            }
            if (binop_right.type == 2) {
                strcat(temp_concat_rt_2, binop_right.value.s);
            } else if (binop_right.type == 0) {
                char int_buf[64];
                snprintf(int_buf, sizeof(int_buf), "%lld", binop_right.value.i);
                strcat(temp_concat_rt_2, int_buf);
            } else if (binop_right.type == 3) {
                strcat(temp_concat_rt_2, binop_right.value.i ? "True" : "False");
            } else if (binop_right.type == 1) {
                char float_buf[64];
                snprintf(float_buf, sizeof(float_buf), "%f", binop_right.value.f);
                strcat(temp_concat_rt_2, float_buf);
            }
        temp_result = (TauValue){.type = 2, .value.s = strdup(temp_concat_rt_2), .refcount = 1};
        } else {
        temp_result = (TauValue){.type = 0, .value.i = binop_left.value.i + binop_right.value.i};
        }
        i = temp_result;
    }
        temp_result = result;
        return temp_result;
}


// Wrapper functions for default arguments

int main(int argc, char* argv[]) {
    // Initialize sys module
    g_sys_module = tauraro_init_sys_module(argc, argv);

    // Initialize __name__ to "__main__"
    __name__ = (TauValue){.type = 2, .value.s = strdup("__main__"), .refcount = 1, .next = NULL};

    TauValue arg_1_arg_0, temp_result, temp_left, temp_right, arg_0, arg_1_arg_1, temp, arg_1;
    temp = (TauValue){.type = 2, .value.s = strdup("\nSimple test for shared library generation (no imports)\n"), .refcount = 1};
    // Test
    arg_0 = (TauValue){.type = 2, .value.s = strdup("Testing: 5 + 3 ="), .refcount = 1};
    arg_1_arg_0 = (TauValue){.type = 0, .value.i = 5};
    arg_1_arg_1 = (TauValue){.type = 0, .value.i = 3};
    arg_1 = add(arg_1_arg_0, arg_1_arg_1);
    printf("%s %s\n", tauraro_str_from_value(&arg_0).value.s, tauraro_str_from_value(&arg_1).value.s);
    arg_0 = (TauValue){.type = 2, .value.s = strdup("Testing: 4 * 7 ="), .refcount = 1};
    arg_1_arg_0 = (TauValue){.type = 0, .value.i = 4};
    arg_1_arg_1 = (TauValue){.type = 0, .value.i = 7};
    arg_1 = multiply(arg_1_arg_0, arg_1_arg_1);
    printf("%s %s\n", tauraro_str_from_value(&arg_0).value.s, tauraro_str_from_value(&arg_1).value.s);

    return 0;
}
