#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>

// Tauraro runtime data structures
typedef enum {
    TAURARO_INT,
    TAURARO_FLOAT,
    TAURARO_BOOL,
    TAURARO_STRING,
    TAURARO_LIST,
    TAURARO_DICT,
    TAURARO_TUPLE,
    TAURARO_SET,
    TAURARO_NONE,
    TAURARO_OBJECT,
    TAURARO_FUNCTION,
    TAURARO_BYTES,
    TAURARO_COMPLEX,
    TAURARO_RANGE,
    TAURARO_FROZENSET
} tauraro_type_t;

typedef struct tauraro_value {
    tauraro_type_t type;
    int ref_count;
    union {
        int64_t int_val;
        double float_val;
        bool bool_val;
        char* str_val;
        struct tauraro_list* list_val;
        struct tauraro_dict* dict_val;
        struct tauraro_tuple* tuple_val;
        struct tauraro_set* set_val;
        struct tauraro_object* obj_val;
        struct tauraro_function* func_val;
        struct tauraro_bytes* bytes_val;
        struct tauraro_complex* complex_val;
        struct tauraro_range* range_val;
        void* ptr_val;
    } data;
} tauraro_value_t;

typedef struct tauraro_list {
    tauraro_value_t** items;
    size_t size;
    size_t capacity;
} tauraro_list_t;

typedef struct tauraro_dict {
    char** keys;
    tauraro_value_t** values;
    size_t size;
    size_t capacity;
} tauraro_dict_t;

typedef struct tauraro_tuple {
    tauraro_value_t** items;
    size_t size;
    size_t capacity;
} tauraro_tuple_t;

typedef struct tauraro_set {
    tauraro_value_t** items;
    size_t size;
    size_t capacity;
} tauraro_set_t;

typedef struct tauraro_bytes {
    unsigned char* data;
    size_t size;
} tauraro_bytes_t;

typedef struct tauraro_complex {
    double real;
    double imag;
} tauraro_complex_t;

typedef struct tauraro_range {
    int64_t start;
    int64_t stop;
    int64_t step;
} tauraro_range_t;

typedef struct tauraro_function {
    char* name;
    void* func_ptr;
    tauraro_value_t** defaults;
    size_t num_defaults;
    struct tauraro_dict* closure;
} tauraro_function_t;

// OOP struct definitions

typedef struct tauraro_object {
    char* class_name;
    struct tauraro_class* class_ptr;
    struct tauraro_dict* fields;
    struct tauraro_dict* methods;
    struct tauraro_list* bases;
} tauraro_object_t;

typedef struct tauraro_class {
    char* name;
    struct tauraro_list* bases;
    struct tauraro_list* mro;  // Method Resolution Order
    struct tauraro_dict* methods;
    struct tauraro_dict* class_attrs;
    struct tauraro_dict* properties;
} tauraro_class_t;


// Type utility functions
tauraro_value_t* tauraro_value_new();
void tauraro_value_free(tauraro_value_t* value);
tauraro_value_t* tauraro_value_copy(tauraro_value_t* value);
void tauraro_incref(tauraro_value_t* value);
void tauraro_decref(tauraro_value_t* value);

// OOP function declarations

tauraro_value_t* tauraro_object_create(const char* class_name);
void tauraro_object_set_attr(tauraro_value_t* object, const char* attr, tauraro_value_t* value);
tauraro_value_t* tauraro_object_get_attr(tauraro_value_t* object, const char* attr);
bool tauraro_object_has_attr(tauraro_value_t* object, const char* attr);
void tauraro_object_del_attr(tauraro_value_t* object, const char* attr);

tauraro_class_t* tauraro_class_create(const char* name, tauraro_list_t* bases);
void tauraro_class_add_method(tauraro_class_t* class, const char* name, void* method_ptr);
tauraro_value_t* tauraro_class_get_method(tauraro_class_t* class, const char* name);
void tauraro_compute_mro(tauraro_class_t* class);

tauraro_value_t* tauraro_super(tauraro_value_t* object, const char* method_name);
tauraro_value_t* tauraro_super_call(int argc, tauraro_value_t** args);
bool tauraro_isinstance_check(tauraro_value_t* object, const char* class_name);
bool tauraro_issubclass_check(const char* derived, const char* base);

// Builtin function declarations
tauraro_value_t* tauraro_print(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_isinstance(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_range(int argc, tauraro_value_t** args);

// Runtime operators
tauraro_value_t* tauraro_add(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_sub(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_mul(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_div(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_mod(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_eq(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_ne(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_lt(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_le(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_gt(tauraro_value_t* left, tauraro_value_t* right);
tauraro_value_t* tauraro_ge(tauraro_value_t* left, tauraro_value_t* right);

// Optimized typed operations
int64_t tauraro_add_int(int64_t left, int64_t right);
double tauraro_add_float(double left, double right);
char* tauraro_add_string(char* left, char* right);

// Type utility function implementations
tauraro_value_t* tauraro_value_new() {
    tauraro_value_t* value = malloc(sizeof(tauraro_value_t));
    value->ref_count = 1;
    value->type = TAURARO_NONE;
    return value;
}

void tauraro_value_free(tauraro_value_t* value) {
    if (!value) return;

    switch (value->type) {
        case TAURARO_STRING:
            if (value->data.str_val) free(value->data.str_val);
            break;
        case TAURARO_LIST:
            if (value->data.list_val) {
                for (size_t i = 0; i < value->data.list_val->size; i++) {
                    tauraro_decref(value->data.list_val->items[i]);
                }
                free(value->data.list_val->items);
                free(value->data.list_val);
            }
            break;
        case TAURARO_DICT:
            if (value->data.dict_val) {
                for (size_t i = 0; i < value->data.dict_val->size; i++) {
                    free(value->data.dict_val->keys[i]);
                    tauraro_decref(value->data.dict_val->values[i]);
                }
                free(value->data.dict_val->keys);
                free(value->data.dict_val->values);
                free(value->data.dict_val);
            }
            break;
        default:
            break;
    }

    free(value);
}

tauraro_value_t* tauraro_value_copy(tauraro_value_t* value) {
    if (!value) return NULL;

    tauraro_value_t* copy = malloc(sizeof(tauraro_value_t));
    copy->type = value->type;
    copy->ref_count = 1;

    switch (value->type) {
        case TAURARO_INT:
            copy->data.int_val = value->data.int_val;
            break;
        case TAURARO_FLOAT:
            copy->data.float_val = value->data.float_val;
            break;
        case TAURARO_BOOL:
            copy->data.bool_val = value->data.bool_val;
            break;
        case TAURARO_STRING:
            copy->data.str_val = strdup(value->data.str_val);
            break;
        default:
            // For complex types, we'll implement proper deep copy later
            copy->data = value->data;
            break;
    }

    return copy;
}

void tauraro_incref(tauraro_value_t* value) {
    if (value) {
        value->ref_count++;
    }
}

void tauraro_decref(tauraro_value_t* value) {
    if (value && --value->ref_count == 0) {
        tauraro_value_free(value);
    }
}

// OOP function implementations

tauraro_value_t* tauraro_object_create(const char* class_name) {
    tauraro_value_t* result = malloc(sizeof(tauraro_value_t));
    result->type = TAURARO_OBJECT;
    result->ref_count = 1;

    tauraro_object_t* obj = malloc(sizeof(tauraro_object_t));
    obj->class_name = strdup(class_name);
    obj->class_ptr = NULL;

    obj->fields = malloc(sizeof(tauraro_dict_t));
    obj->fields->size = 0;
    obj->fields->capacity = 10;
    obj->fields->keys = malloc(sizeof(char*) * obj->fields->capacity);
    obj->fields->values = malloc(sizeof(tauraro_value_t*) * obj->fields->capacity);

    obj->methods = malloc(sizeof(tauraro_dict_t));
    obj->methods->size = 0;
    obj->methods->capacity = 10;
    obj->methods->keys = malloc(sizeof(char*) * obj->methods->capacity);
    obj->methods->values = malloc(sizeof(tauraro_value_t*) * obj->methods->capacity);

    obj->bases = malloc(sizeof(tauraro_list_t));
    obj->bases->size = 0;
    obj->bases->capacity = 5;
    obj->bases->items = malloc(sizeof(tauraro_value_t*) * obj->bases->capacity);

    result->data.obj_val = obj;
    return result;
}

void tauraro_object_set_attr(tauraro_value_t* object, const char* attr, tauraro_value_t* value) {
    if (object->type != TAURARO_OBJECT) return;

    tauraro_object_t* obj = (tauraro_object_t*)object->data.obj_val;

    // Check if attribute already exists
    for (size_t i = 0; i < obj->fields->size; i++) {
        if (strcmp(obj->fields->keys[i], attr) == 0) {
            tauraro_decref(obj->fields->values[i]);
            obj->fields->values[i] = value;
            tauraro_incref(value);
            return;
        }
    }

    // Add new attribute
    if (obj->fields->size >= obj->fields->capacity) {
        obj->fields->capacity *= 2;
        obj->fields->keys = realloc(obj->fields->keys, sizeof(char*) * obj->fields->capacity);
        obj->fields->values = realloc(obj->fields->values, sizeof(tauraro_value_t*) * obj->fields->capacity);
    }

    obj->fields->keys[obj->fields->size] = strdup(attr);
    obj->fields->values[obj->fields->size] = value;
    tauraro_incref(value);
    obj->fields->size++;
}

tauraro_value_t* tauraro_object_get_attr(tauraro_value_t* object, const char* attr) {
    if (object->type != TAURARO_OBJECT) return NULL;

    tauraro_object_t* obj = (tauraro_object_t*)object->data.obj_val;

    // First check object fields
    for (size_t i = 0; i < obj->fields->size; i++) {
        if (strcmp(obj->fields->keys[i], attr) == 0) {
            return obj->fields->values[i];
        }
    }

    // Then check object methods
    for (size_t i = 0; i < obj->methods->size; i++) {
        if (strcmp(obj->methods->keys[i], attr) == 0) {
            return obj->methods->values[i];
        }
    }

    // Finally check class methods if class_ptr is set
    if (obj->class_ptr) {
        return tauraro_class_get_method(obj->class_ptr, attr);
    }

    return NULL;
}

bool tauraro_object_has_attr(tauraro_value_t* object, const char* attr) {
    return tauraro_object_get_attr(object, attr) != NULL;
}

void tauraro_object_del_attr(tauraro_value_t* object, const char* attr) {
    if (object->type != TAURARO_OBJECT) return;

    tauraro_object_t* obj = (tauraro_object_t*)object->data.obj_val;

    for (size_t i = 0; i < obj->fields->size; i++) {
        if (strcmp(obj->fields->keys[i], attr) == 0) {
            free(obj->fields->keys[i]);
            tauraro_decref(obj->fields->values[i]);

            // Shift remaining elements
            for (size_t j = i; j < obj->fields->size - 1; j++) {
                obj->fields->keys[j] = obj->fields->keys[j + 1];
                obj->fields->values[j] = obj->fields->values[j + 1];
            }
            obj->fields->size--;
            return;
        }
    }
}

tauraro_class_t* tauraro_class_create(const char* name, tauraro_list_t* bases) {
    tauraro_class_t* class = malloc(sizeof(tauraro_class_t));
    class->name = strdup(name);
    class->bases = bases;

    class->mro = malloc(sizeof(tauraro_list_t));
    class->mro->size = 0;
    class->mro->capacity = 10;
    class->mro->items = malloc(sizeof(tauraro_value_t*) * class->mro->capacity);

    class->methods = malloc(sizeof(tauraro_dict_t));
    class->methods->size = 0;
    class->methods->capacity = 20;
    class->methods->keys = malloc(sizeof(char*) * class->methods->capacity);
    class->methods->values = malloc(sizeof(tauraro_value_t*) * class->methods->capacity);

    class->class_attrs = malloc(sizeof(tauraro_dict_t));
    class->class_attrs->size = 0;
    class->class_attrs->capacity = 10;
    class->class_attrs->keys = malloc(sizeof(char*) * class->class_attrs->capacity);
    class->class_attrs->values = malloc(sizeof(tauraro_value_t*) * class->class_attrs->capacity);

    class->properties = malloc(sizeof(tauraro_dict_t));
    class->properties->size = 0;
    class->properties->capacity = 10;
    class->properties->keys = malloc(sizeof(char*) * class->properties->capacity);
    class->properties->values = malloc(sizeof(tauraro_value_t*) * class->properties->capacity);

    // Compute MRO
    tauraro_compute_mro(class);

    return class;
}

void tauraro_class_add_method(tauraro_class_t* class, const char* name, void* method_ptr) {
    if (class->methods->size >= class->methods->capacity) {
        class->methods->capacity *= 2;
        class->methods->keys = realloc(class->methods->keys, sizeof(char*) * class->methods->capacity);
        class->methods->values = realloc(class->methods->values, sizeof(tauraro_value_t*) * class->methods->capacity);
    }

    tauraro_value_t* method_val = malloc(sizeof(tauraro_value_t));
    method_val->type = TAURARO_FUNCTION;
    method_val->ref_count = 1;
    method_val->data.ptr_val = method_ptr;

    class->methods->keys[class->methods->size] = strdup(name);
    class->methods->values[class->methods->size] = method_val;
    class->methods->size++;
}

tauraro_value_t* tauraro_class_get_method(tauraro_class_t* class, const char* name) {
    // Search in class methods using MRO
    for (size_t mro_idx = 0; mro_idx < class->mro->size; mro_idx++) {
        tauraro_value_t* base_val = class->mro->items[mro_idx];
        if (base_val->type == TAURARO_OBJECT) {
            tauraro_object_t* base_obj = (tauraro_object_t*)base_val->data.obj_val;
            if (base_obj->class_ptr) {
                for (size_t i = 0; i < base_obj->class_ptr->methods->size; i++) {
                    if (strcmp(base_obj->class_ptr->methods->keys[i], name) == 0) {
                        return base_obj->class_ptr->methods->values[i];
                    }
                }
            }
        }
    }

    // Direct search in this class
    for (size_t i = 0; i < class->methods->size; i++) {
        if (strcmp(class->methods->keys[i], name) == 0) {
            return class->methods->values[i];
        }
    }

    return NULL;
}

void tauraro_compute_mro(tauraro_class_t* class) {
    // Improved C3 linearization algorithm
    // Add self to MRO
    tauraro_value_t* self_val = malloc(sizeof(tauraro_value_t));
    self_val->type = TAURARO_OBJECT;
    self_val->ref_count = 1;
    self_val->data.obj_val = malloc(sizeof(tauraro_object_t));
    self_val->data.obj_val->class_name = strdup(class->name);

    // Ensure capacity
    if (class->mro->size >= class->mro->capacity) {
        class->mro->capacity *= 2;
        class->mro->items = realloc(class->mro->items, sizeof(tauraro_value_t*) * class->mro->capacity);
    }
    class->mro->items[class->mro->size++] = self_val;

    // Add bases to MRO
    if (class->bases) {
        for (size_t i = 0; i < class->bases->size; i++) {
            if (class->mro->size >= class->mro->capacity) {
                class->mro->capacity *= 2;
                class->mro->items = realloc(class->mro->items, sizeof(tauraro_value_t*) * class->mro->capacity);
            }
            class->mro->items[class->mro->size++] = class->bases->items[i];
        }
    }
    
    // Add object as the ultimate base class if not already present
    bool has_object = false;
    for (size_t i = 0; i < class->mro->size; i++) {
        if (class->mro->items[i]->type == TAURARO_OBJECT) {
            tauraro_object_t* obj = (tauraro_object_t*)class->mro->items[i]->data.obj_val;
            if (strcmp(obj->class_name, "object") == 0) {
                has_object = true;
                break;
            }
        }
    }
    
    if (!has_object) {
        // Add object class
        tauraro_value_t* object_val = malloc(sizeof(tauraro_value_t));
        object_val->type = TAURARO_OBJECT;
        object_val->ref_count = 1;
        object_val->data.obj_val = malloc(sizeof(tauraro_object_t));
        object_val->data.obj_val->class_name = strdup("object");
        object_val->data.obj_val->class_ptr = NULL;
        object_val->data.obj_val->fields = malloc(sizeof(tauraro_dict_t));
        object_val->data.obj_val->fields->size = 0;
        object_val->data.obj_val->fields->capacity = 10;
        object_val->data.obj_val->fields->keys = malloc(sizeof(char*) * object_val->data.obj_val->fields->capacity);
        object_val->data.obj_val->fields->values = malloc(sizeof(tauraro_value_t*) * object_val->data.obj_val->fields->capacity);
        object_val->data.obj_val->methods = malloc(sizeof(tauraro_dict_t));
        object_val->data.obj_val->methods->size = 0;
        object_val->data.obj_val->methods->capacity = 10;
        object_val->data.obj_val->methods->keys = malloc(sizeof(char*) * object_val->data.obj_val->methods->capacity);
        object_val->data.obj_val->methods->values = malloc(sizeof(tauraro_value_t*) * object_val->data.obj_val->methods->capacity);
        object_val->data.obj_val->bases = malloc(sizeof(tauraro_list_t));
        object_val->data.obj_val->bases->size = 0;
        object_val->data.obj_val->bases->capacity = 5;
        object_val->data.obj_val->bases->items = malloc(sizeof(tauraro_value_t*) * object_val->data.obj_val->bases->capacity);
        
        if (class->mro->size >= class->mro->capacity) {
            class->mro->capacity *= 2;
            class->mro->items = realloc(class->mro->items, sizeof(tauraro_value_t*) * class->mro->capacity);
        }
        class->mro->items[class->mro->size++] = object_val;
    }
}

tauraro_value_t* tauraro_super_call(int argc, tauraro_value_t** args) {
    // Enhanced super() implementation
    // In a real implementation, this would need access to the current class context
    if (argc < 1) {
        return NULL;
    }
    
    // For now, we'll just return the first argument (the instance)
    // In a full implementation, we would resolve the appropriate parent method
    tauraro_incref(args[0]);
    return args[0];
}

tauraro_value_t* tauraro_super(tauraro_value_t* object, const char* method_name) {
    if (object->type != TAURARO_OBJECT) return NULL;

    tauraro_object_t* obj = (tauraro_object_t*)object->data.obj_val;

    // Search in base classes (starting from index 1 in MRO to skip current class)
    if (obj->class_ptr && obj->class_ptr->mro && obj->class_ptr->mro->size > 1) {
        for (size_t i = 1; i < obj->class_ptr->mro->size; i++) {
            tauraro_value_t* base = obj->class_ptr->mro->items[i];
            if (base->type == TAURARO_OBJECT) {
                tauraro_object_t* base_obj = (tauraro_object_t*)base->data.obj_val;
                if (base_obj->class_ptr) {
                    tauraro_value_t* method = tauraro_class_get_method(base_obj->class_ptr, method_name);
                    if (method) return method;
                }
            }
        }
    }

    return NULL;
}

bool tauraro_isinstance_check(tauraro_value_t* object, const char* class_name) {
    if (object->type != TAURARO_OBJECT) return false;

    tauraro_object_t* obj = (tauraro_object_t*)object->data.obj_val;

    // Check direct class name
    if (strcmp(obj->class_name, class_name) == 0) return true;

    // Check bases via MRO
    if (obj->class_ptr && obj->class_ptr->mro) {
        for (size_t i = 0; i < obj->class_ptr->mro->size; i++) {
            tauraro_value_t* base = obj->class_ptr->mro->items[i];
            if (base->type == TAURARO_OBJECT) {
                tauraro_object_t* base_obj = (tauraro_object_t*)base->data.obj_val;
                if (strcmp(base_obj->class_name, class_name) == 0) return true;
            }
        }
    }

    return false;
}

bool tauraro_issubclass_check(const char* derived, const char* base) {
    // Simplified implementation
    // In a full implementation, this would check the class hierarchy
    return strcmp(derived, base) == 0;
}

// Builtin function implementations
tauraro_value_t* tauraro_print(int argc, tauraro_value_t** args) {
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

tauraro_value_t* tauraro_isinstance(int argc, tauraro_value_t** args) {
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

tauraro_value_t* tauraro_range(int argc, tauraro_value_t** args) {
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

// Runtime support functions (operators only)

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

// Optimized typed operation implementations
int64_t tauraro_add_int(int64_t left, int64_t right) {
    return left + right;
}

double tauraro_add_float(double left, double right) {
    return left + right;
}

char* tauraro_add_string(char* left, char* right) {
    size_t left_len = strlen(left);
    size_t right_len = strlen(right);
    char* result = malloc(left_len + right_len + 1);
    strcpy(result, left);
    strcat(result, right);
    return result;
}

// Global variables
tauraro_value_t* total;

int main() {
    double temp_result = 0.0;
    double arg_1 = 0.0;
    char* arg_0 = NULL;
    tauraro_value_t* temp = NULL;
    int64_t i = 0;
    double total = 0.0;
    double binop_right = 0.0;
    double var_total_temp = 0.0;
    double binop_left = 0.0;
    // Simple float test
    var_total_temp = 0;
    total = var_total_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_INT; arg_0->data.int_val = 1000;
    tauraro_value_t* arg_0_as_value = tauraro_value_new(); arg_0_as_value->type = TAURARO_STRING; arg_0_as_value->data.str_val = arg_0; temp = tauraro_range(1, (tauraro_value_t*[]){arg_0_as_value});
    // For loop: i in temp (optimized)
    tauraro_value_t* i_iter = temp;
    int i_index = 0;
    if (i_iter->type == TAURARO_RANGE) {
        int start = i_iter->data.range_val->start;
        int stop = i_iter->data.range_val->stop;
        int step = i_iter->data.range_val->step;
        for (i = start; (step > 0) ? (i < stop) : (i > stop); i += step) {
            binop_left = total;
            binop_right = 1.5;
            temp_result = binop_left + binop_right;
            total = temp_result;
        }
    } else {
        // Fallback for non-range iterables
        // TODO: Handle list/tuple/etc
    }
    arg_0 = strdup("Float total:");
    arg_1 = total;
    tauraro_value_t* arg_0_as_value = tauraro_value_new(); arg_0_as_value->type = TAURARO_STRING; arg_0_as_value->data.str_val = arg_0; tauraro_value_t* arg_1_as_value = tauraro_value_new(); arg_1_as_value->type = TAURARO_FLOAT; arg_1_as_value->data.float_val = arg_1; temp = tauraro_print(2, (tauraro_value_t*[]){arg_0_as_value, arg_1_as_value});
    return 0;
}
