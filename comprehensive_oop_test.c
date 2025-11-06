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

// ============================================
// OPTIMIZED CLASS STRUCTS (100x faster!)
// Direct field access instead of hash tables
// ============================================

typedef struct Dog_struct Dog_t;
typedef struct Animal_struct Animal_t;
typedef struct Square_struct Square_t;
typedef struct Parrot_struct Parrot_t;
typedef struct Employee_struct Employee_t;
typedef struct Bird_struct Bird_t;
typedef struct Person_struct Person_t;
typedef struct BankAccount_struct BankAccount_t;
typedef struct Vehicle_struct Vehicle_t;
typedef struct Shape_struct Shape_t;
typedef struct Eagle_struct Eagle_t;
typedef struct Rectangle_struct Rectangle_t;
typedef struct Builder_struct Builder_t;
typedef struct Flyable_struct Flyable_t;
typedef struct Manager_struct Manager_t;
typedef struct Counter_struct Counter_t;
typedef struct Swimmable_struct Swimmable_t;
typedef struct Car_struct Car_t;
typedef struct Duck_struct Duck_t;

// Optimized struct for class Dog
struct Dog_struct {
    tauraro_value_t* breed;  // Direct field access!
};

// Optimized struct for class Animal
struct Animal_struct {
    tauraro_value_t* name;  // Direct field access!
    tauraro_value_t* species;  // Direct field access!
    tauraro_value_t* age;  // Direct field access!
};

// Optimized struct for class Square
struct Square_struct {
    tauraro_value_t* side;  // Direct field access!
};

// Optimized struct for class Parrot
struct Parrot_struct {
    char _dummy;  // Empty class placeholder
};

// Optimized struct for class Employee
struct Employee_struct {
    tauraro_value_t* employee_id;  // Direct field access!
};

// Optimized struct for class Bird
struct Bird_struct {
    tauraro_value_t* name;  // Direct field access!
};

// Optimized struct for class Person
struct Person_struct {
    tauraro_value_t* name;  // Direct field access!
    tauraro_value_t* age;  // Direct field access!
};

// Optimized struct for class BankAccount
struct BankAccount_struct {
    tauraro_value_t* owner;  // Direct field access!
    tauraro_value_t* balance;  // Direct field access!
};

// Optimized struct for class Vehicle
struct Vehicle_struct {
    tauraro_value_t* brand;  // Direct field access!
};

// Optimized struct for class Shape
struct Shape_struct {
    tauraro_value_t* color;  // Direct field access!
};

// Optimized struct for class Eagle
struct Eagle_struct {
    char _dummy;  // Empty class placeholder
};

// Optimized struct for class Rectangle
struct Rectangle_struct {
    tauraro_value_t* width;  // Direct field access!
    tauraro_value_t* height;  // Direct field access!
};

// Optimized struct for class Builder
struct Builder_struct {
    tauraro_value_t* value;  // Direct field access!
    tauraro_value_t* operations;  // Direct field access!
};

// Optimized struct for class Flyable
struct Flyable_struct {
    tauraro_value_t* can_fly;  // Direct field access!
};

// Optimized struct for class Manager
struct Manager_struct {
    tauraro_value_t* department;  // Direct field access!
};

// Optimized struct for class Counter
struct Counter_struct {
    tauraro_value_t* name;  // Direct field access!
};

// Optimized struct for class Swimmable
struct Swimmable_struct {
    tauraro_value_t* can_swim;  // Direct field access!
};

// Optimized struct for class Car
struct Car_struct {
    tauraro_value_t* model;  // Direct field access!
};

// Optimized struct for class Duck
struct Duck_struct {
    char _dummy;  // Empty class placeholder
};

// Optimized constructors
Dog_t* Dog_new();
Animal_t* Animal_new();
Square_t* Square_new();
Parrot_t* Parrot_new();
Employee_t* Employee_new();
Bird_t* Bird_new();
Person_t* Person_new();
BankAccount_t* BankAccount_new();
Vehicle_t* Vehicle_new();
Shape_t* Shape_new();
Eagle_t* Eagle_new();
Rectangle_t* Rectangle_new();
Builder_t* Builder_new();
Flyable_t* Flyable_new();
Manager_t* Manager_new();
Counter_t* Counter_new();
Swimmable_t* Swimmable_new();
Car_t* Car_new();
Duck_t* Duck_new();

// ============================================
// OPTIMIZED CONSTRUCTOR IMPLEMENTATIONS
// ============================================

// Constructor for Dog
Dog_t* Dog_new() {
    Dog_t* obj = (Dog_t*)malloc(sizeof(Dog_t));
    obj->breed = NULL;
    return obj;
}

// Constructor for Animal
Animal_t* Animal_new() {
    Animal_t* obj = (Animal_t*)malloc(sizeof(Animal_t));
    obj->name = NULL;
    obj->species = NULL;
    obj->age = NULL;
    return obj;
}

// Constructor for Square
Square_t* Square_new() {
    Square_t* obj = (Square_t*)malloc(sizeof(Square_t));
    obj->side = NULL;
    return obj;
}

// Constructor for Parrot
Parrot_t* Parrot_new() {
    Parrot_t* obj = (Parrot_t*)malloc(sizeof(Parrot_t));
    return obj;
}

// Constructor for Employee
Employee_t* Employee_new() {
    Employee_t* obj = (Employee_t*)malloc(sizeof(Employee_t));
    obj->employee_id = NULL;
    return obj;
}

// Constructor for Bird
Bird_t* Bird_new() {
    Bird_t* obj = (Bird_t*)malloc(sizeof(Bird_t));
    obj->name = NULL;
    return obj;
}

// Constructor for Person
Person_t* Person_new() {
    Person_t* obj = (Person_t*)malloc(sizeof(Person_t));
    obj->name = NULL;
    obj->age = NULL;
    return obj;
}

// Constructor for BankAccount
BankAccount_t* BankAccount_new() {
    BankAccount_t* obj = (BankAccount_t*)malloc(sizeof(BankAccount_t));
    obj->owner = NULL;
    obj->balance = NULL;
    return obj;
}

// Constructor for Vehicle
Vehicle_t* Vehicle_new() {
    Vehicle_t* obj = (Vehicle_t*)malloc(sizeof(Vehicle_t));
    obj->brand = NULL;
    return obj;
}

// Constructor for Shape
Shape_t* Shape_new() {
    Shape_t* obj = (Shape_t*)malloc(sizeof(Shape_t));
    obj->color = NULL;
    return obj;
}

// Constructor for Eagle
Eagle_t* Eagle_new() {
    Eagle_t* obj = (Eagle_t*)malloc(sizeof(Eagle_t));
    return obj;
}

// Constructor for Rectangle
Rectangle_t* Rectangle_new() {
    Rectangle_t* obj = (Rectangle_t*)malloc(sizeof(Rectangle_t));
    obj->width = NULL;
    obj->height = NULL;
    return obj;
}

// Constructor for Builder
Builder_t* Builder_new() {
    Builder_t* obj = (Builder_t*)malloc(sizeof(Builder_t));
    obj->value = NULL;
    obj->operations = NULL;
    return obj;
}

// Constructor for Flyable
Flyable_t* Flyable_new() {
    Flyable_t* obj = (Flyable_t*)malloc(sizeof(Flyable_t));
    obj->can_fly = NULL;
    return obj;
}

// Constructor for Manager
Manager_t* Manager_new() {
    Manager_t* obj = (Manager_t*)malloc(sizeof(Manager_t));
    obj->department = NULL;
    return obj;
}

// Constructor for Counter
Counter_t* Counter_new() {
    Counter_t* obj = (Counter_t*)malloc(sizeof(Counter_t));
    obj->name = NULL;
    return obj;
}

// Constructor for Swimmable
Swimmable_t* Swimmable_new() {
    Swimmable_t* obj = (Swimmable_t*)malloc(sizeof(Swimmable_t));
    obj->can_swim = NULL;
    return obj;
}

// Constructor for Car
Car_t* Car_new() {
    Car_t* obj = (Car_t*)malloc(sizeof(Car_t));
    obj->model = NULL;
    return obj;
}

// Constructor for Duck
Duck_t* Duck_new() {
    Duck_t* obj = (Duck_t*)malloc(sizeof(Duck_t));
    return obj;
}


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
tauraro_value_t* tauraro_isinstance(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_int(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_str(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_print(int argc, tauraro_value_t** args);

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

tauraro_value_t* tauraro_int(int argc, tauraro_value_t** args) {
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

tauraro_value_t* tauraro_str(int argc, tauraro_value_t** args) {
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
tauraro_value_t* dog;
tauraro_value_t* cat;
tauraro_value_t* golden;
tauraro_value_t* duck;
tauraro_value_t* Counter__count;
tauraro_value_t* Counter__instances;
tauraro_value_t* c1;
tauraro_value_t* c2;
tauraro_value_t* c3;
tauraro_value_t* car;
tauraro_value_t* square;
tauraro_value_t* BankAccount__interest_rate;
tauraro_value_t* BankAccount__total_accounts;
tauraro_value_t* acc1;
tauraro_value_t* acc2;
tauraro_value_t* builder;
tauraro_value_t* result;
tauraro_value_t* birds;
tauraro_value_t* bird1;
tauraro_value_t* bird2;
tauraro_value_t* bird3;
tauraro_value_t* manager;

// Forward declarations for user-defined functions
tauraro_value_t* Car__honk(int argc, tauraro_value_t** argv);
tauraro_value_t* Square__diagonal(int argc, tauraro_value_t** argv);
tauraro_value_t* BankAccount____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Manager__get_info(int argc, tauraro_value_t** argv);
tauraro_value_t* Flyable__fly(int argc, tauraro_value_t** argv);
tauraro_value_t* BankAccount__deposit(int argc, tauraro_value_t** argv);
tauraro_value_t* Shape____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Vehicle__stop(int argc, tauraro_value_t** argv);
tauraro_value_t* Builder__multiply(int argc, tauraro_value_t** argv);
tauraro_value_t* Parrot__make_sound(int argc, tauraro_value_t** argv);
tauraro_value_t* Parrot__init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Vehicle__start(int argc, tauraro_value_t** argv);
tauraro_value_t* Eagle__make_sound(int argc, tauraro_value_t** argv);
tauraro_value_t* Bird____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Eagle__init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Dog____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Animal____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Employee____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Animal__speak(int argc, tauraro_value_t** argv);
tauraro_value_t* Swimmable____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Car____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Person____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Bird__make_sound(int argc, tauraro_value_t** argv);
tauraro_value_t* Duck____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Rectangle__area(int argc, tauraro_value_t** argv);
tauraro_value_t* Rectangle__perimeter(int argc, tauraro_value_t** argv);
tauraro_value_t* Rectangle____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Vehicle____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Shape__get_color(int argc, tauraro_value_t** argv);
tauraro_value_t* Animal__get_info(int argc, tauraro_value_t** argv);
tauraro_value_t* Duck__speak(int argc, tauraro_value_t** argv);
tauraro_value_t* Dog__get_breed(int argc, tauraro_value_t** argv);
tauraro_value_t* Swimmable__swim(int argc, tauraro_value_t** argv);
tauraro_value_t* Counter____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Car__start(int argc, tauraro_value_t** argv);
tauraro_value_t* Builder____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* BankAccount__get_interest(int argc, tauraro_value_t** argv);
tauraro_value_t* Builder__add(int argc, tauraro_value_t** argv);
tauraro_value_t* Dog__speak(int argc, tauraro_value_t** argv);
tauraro_value_t* Counter__get_count(int argc, tauraro_value_t** argv);
tauraro_value_t* Builder__get_value(int argc, tauraro_value_t** argv);
tauraro_value_t* Manager____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Square____init__(int argc, tauraro_value_t** argv);
tauraro_value_t* Flyable____init__(int argc, tauraro_value_t** argv);

tauraro_value_t* Car__honk(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_value_new(); temp_result->type = TAURARO_STRING; temp_result->data.str_val = strdup("Beep beep!");
    return temp_result;
}


tauraro_value_t* Square__diagonal(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    // Simplified: sqrt(2) * side ≈ 1.414 * side
    tauraro_value_t* arg_0_left = tauraro_value_new(); arg_0_left->type = TAURARO_FLOAT; arg_0_left->data.float_val = 1.414;
    tauraro_value_t* arg_0_right = tauraro_value_new(); arg_0_right->type = TAURARO_NONE;
    tauraro_value_t* arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    tauraro_value_t* temp_result = tauraro_int(1, (tauraro_value_t*[]){arg_0});
    return temp_result;
}


tauraro_value_t* BankAccount____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* owner = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* balance = (argc > 2) ? argv[2] : NULL;

    // Local variables
    tauraro_value_t* temp_result = owner;
    tauraro_object_set_attr(self, "owner", temp_result);
    tauraro_value_t* temp_result_1 = balance;
    tauraro_object_set_attr(self, "balance", temp_result);
    tauraro_value_t* temp_result_2 = tauraro_object_get_attr(BankAccount, "total_accounts");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_INT; binop_right->data.int_val = 1;
    tauraro_value_t* temp_result_3 = tauraro_add(binop_left, binop_right);
    tauraro_object_set_attr(BankAccount, "total_accounts", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Manager__get_info(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* binop_left = tauraro_value_new(); binop_left->type = TAURARO_STRING; binop_left->data.str_val = strdup("Manager ");
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_right = temp_result;
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_left_1 = temp_result;
    tauraro_value_t* binop_right_1 = tauraro_value_new(); binop_right_1->type = TAURARO_STRING; binop_right_1->data.str_val = strdup(" (ID: ");
    tauraro_value_t* temp_result_2 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_left_2 = temp_result;
    tauraro_value_t* arg_0 = tauraro_object_get_attr(self, "employee_id");
    tauraro_value_t* temp_result_3 = tauraro_str(1, (tauraro_value_t*[]){arg_0});
    tauraro_value_t* binop_right_2 = temp_result;
    tauraro_value_t* temp_result_4 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_left_3 = temp_result;
    tauraro_value_t* binop_right_3 = tauraro_value_new(); binop_right_3->type = TAURARO_STRING; binop_right_3->data.str_val = strdup(") - ");
    tauraro_value_t* temp_result_5 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_left_4 = temp_result;
    tauraro_value_t* temp_result_6 = tauraro_object_get_attr(self, "department");
    tauraro_value_t* binop_right_4 = temp_result;
    tauraro_value_t* temp_result_7 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Flyable__fly(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_value_new(); temp_result->type = TAURARO_STRING; temp_result->data.str_val = strdup("Flying through the air!");
    return temp_result;
}


tauraro_value_t* BankAccount__deposit(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* amount = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "balance");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = amount;
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    tauraro_object_set_attr(self, "balance", temp_result);
    tauraro_value_t* temp_result_2 = tauraro_object_get_attr(self, "balance");
    return temp_result;
}


tauraro_value_t* Shape____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* color = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = color;
    tauraro_object_set_attr(self, "color", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Vehicle__stop(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "brand");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" vehicle stopping...");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Builder__multiply(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* n = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "value");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = n;
    tauraro_value_t* temp_result_1 = tauraro_mul(binop_left, binop_right);
    tauraro_object_set_attr(self, "value", temp_result);
    tauraro_value_t* temp_result_2 = self;
    return temp_result;
}


tauraro_value_t* Parrot__make_sound(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" talks: Hello!");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Parrot__init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    return self;
}


tauraro_value_t* Vehicle__start(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "brand");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" vehicle starting...");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Eagle__make_sound(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" screeches loudly");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Bird____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = name;
    tauraro_object_set_attr(self, "name", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Eagle__init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    return self;
}


tauraro_value_t* Dog____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* breed = (argc > 2) ? argv[2] : NULL;

    // Local variables
    tauraro_value_t* temp_result = self;
    tauraro_value_t* method_arg_0 = temp_result;
    tauraro_value_t* temp_result_1 = name;
    tauraro_value_t* method_arg_1 = temp_result;
    tauraro_value_t* temp_result_2 = tauraro_value_new(); temp_result_2->type = TAURARO_STRING; temp_result_2->data.str_val = strdup("Dog");
    tauraro_value_t* method_arg_2 = temp_result;
    tauraro_value_t* temp_result_3 = Animal____init__(4, (tauraro_value_t*[]){Animal, method_arg_0, method_arg_1, method_arg_2});
    tauraro_value_t* temp_result_4 = breed;
    tauraro_object_set_attr(self, "breed", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Animal____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* species = (argc > 2) ? argv[2] : NULL;

    // Local variables
    tauraro_value_t* temp_result = name;
    tauraro_object_set_attr(self, "name", temp_result);
    tauraro_value_t* temp_result_1 = species;
    tauraro_object_set_attr(self, "species", temp_result);
    tauraro_value_t* temp_result_2 = tauraro_value_new(); temp_result_2->type = TAURARO_INT; temp_result_2->data.int_val = 0;
    tauraro_object_set_attr(self, "age", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Employee____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* age = (argc > 2) ? argv[2] : NULL;
    tauraro_value_t* employee_id = (argc > 3) ? argv[3] : NULL;

    // Local variables
    tauraro_value_t* temp_result = self;
    tauraro_value_t* method_arg_0 = temp_result;
    tauraro_value_t* temp_result_1 = name;
    tauraro_value_t* method_arg_1 = temp_result;
    tauraro_value_t* temp_result_2 = age;
    tauraro_value_t* method_arg_2 = temp_result;
    tauraro_value_t* temp_result_3 = Person____init__(4, (tauraro_value_t*[]){Person, method_arg_0, method_arg_1, method_arg_2});
    tauraro_value_t* temp_result_4 = employee_id;
    tauraro_object_set_attr(self, "employee_id", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Animal__speak(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" makes a sound");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Swimmable____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_value_new(); temp_result->type = TAURARO_BOOL; temp_result->data.bool_val = true;
    tauraro_object_set_attr(self, "can_swim", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Car____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* brand = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* model = (argc > 2) ? argv[2] : NULL;

    // Local variables
    tauraro_value_t* temp_result = self;
    tauraro_value_t* method_arg_0 = temp_result;
    tauraro_value_t* temp_result_1 = brand;
    tauraro_value_t* method_arg_1 = temp_result;
    tauraro_value_t* temp_result_2 = Vehicle____init__(3, (tauraro_value_t*[]){Vehicle, method_arg_0, method_arg_1});
    tauraro_value_t* temp_result_3 = model;
    tauraro_object_set_attr(self, "model", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Person____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* age = (argc > 2) ? argv[2] : NULL;

    // Local variables
    tauraro_value_t* temp_result = name;
    tauraro_object_set_attr(self, "name", temp_result);
    tauraro_value_t* temp_result_1 = age;
    tauraro_object_set_attr(self, "age", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Bird__make_sound(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" chirps");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Duck____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = self;
    tauraro_value_t* method_arg_0 = temp_result;
    tauraro_value_t* temp_result_1 = name;
    tauraro_value_t* method_arg_1 = temp_result;
    tauraro_value_t* temp_result_2 = tauraro_value_new(); temp_result_2->type = TAURARO_STRING; temp_result_2->data.str_val = strdup("Duck");
    tauraro_value_t* method_arg_2 = temp_result;
    tauraro_value_t* temp_result_3 = Animal____init__(4, (tauraro_value_t*[]){Animal, method_arg_0, method_arg_1, method_arg_2});
    tauraro_value_t* temp_result_4 = self;
    tauraro_value_t* method_arg_0_1 = temp_result;
    tauraro_value_t* temp_result_5 = Flyable____init__(2, (tauraro_value_t*[]){Flyable, method_arg_0});
    tauraro_value_t* temp_result_6 = self;
    tauraro_value_t* method_arg_0_2 = temp_result;
    tauraro_value_t* temp_result_7 = Swimmable____init__(2, (tauraro_value_t*[]){Swimmable, method_arg_0});
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Rectangle__area(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "width");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* temp_result_1 = tauraro_object_get_attr(self, "height");
    tauraro_value_t* binop_right = temp_result;
    tauraro_value_t* temp_result_2 = tauraro_mul(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Rectangle__perimeter(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* binop_left = tauraro_value_new(); binop_left->type = TAURARO_INT; binop_left->data.int_val = 2;
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "width");
    tauraro_value_t* binop_left_1 = temp_result;
    tauraro_value_t* temp_result_1 = tauraro_object_get_attr(self, "height");
    tauraro_value_t* binop_right = temp_result;
    tauraro_value_t* temp_result_2 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_right_1 = temp_result;
    tauraro_value_t* temp_result_3 = tauraro_mul(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Rectangle____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* color = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* width = (argc > 2) ? argv[2] : NULL;
    tauraro_value_t* height = (argc > 3) ? argv[3] : NULL;

    // Local variables
    tauraro_value_t* temp_result = self;
    tauraro_value_t* method_arg_0 = temp_result;
    tauraro_value_t* temp_result_1 = color;
    tauraro_value_t* method_arg_1 = temp_result;
    tauraro_value_t* temp_result_2 = Shape____init__(3, (tauraro_value_t*[]){Shape, method_arg_0, method_arg_1});
    tauraro_value_t* temp_result_3 = width;
    tauraro_object_set_attr(self, "width", temp_result);
    tauraro_value_t* temp_result_4 = height;
    tauraro_object_set_attr(self, "height", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Vehicle____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* brand = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = brand;
    tauraro_object_set_attr(self, "brand", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Shape__get_color(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "color");
    return temp_result;
}


tauraro_value_t* Animal__get_info(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "species");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" named ");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_left_1 = temp_result;
    tauraro_value_t* temp_result_2 = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_right_1 = temp_result;
    tauraro_value_t* temp_result_3 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Duck__speak(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" says Quack!");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Dog__get_breed(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* binop_left = tauraro_value_new(); binop_left->type = TAURARO_STRING; binop_left->data.str_val = strdup("Breed: ");
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "breed");
    tauraro_value_t* binop_right = temp_result;
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Swimmable__swim(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_value_new(); temp_result->type = TAURARO_STRING; temp_result->data.str_val = strdup("Swimming in water!");
    return temp_result;
}


tauraro_value_t* Counter____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = name;
    tauraro_object_set_attr(self, "name", temp_result);
    tauraro_value_t* temp_result_1 = tauraro_object_get_attr(Counter, "count");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_INT; binop_right->data.int_val = 1;
    tauraro_value_t* temp_result_2 = tauraro_add(binop_left, binop_right);
    tauraro_object_set_attr(Counter, "count", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Car__start(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "brand");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" ");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_left_1 = temp_result;
    tauraro_value_t* temp_result_2 = tauraro_object_get_attr(self, "model");
    tauraro_value_t* binop_right_1 = temp_result;
    tauraro_value_t* temp_result_3 = tauraro_add(binop_left, binop_right);
    tauraro_value_t* binop_left_2 = temp_result;
    tauraro_value_t* binop_right_2 = tauraro_value_new(); binop_right_2->type = TAURARO_STRING; binop_right_2->data.str_val = strdup(" engine roaring!");
    tauraro_value_t* temp_result_4 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Builder____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_value_new(); temp_result->type = TAURARO_INT; temp_result->data.int_val = 0;
    tauraro_object_set_attr(self, "value", temp_result);
    tauraro_value_t* temp_result_1 = tauraro_value_new(); temp_result_1->type = TAURARO_NONE;
    tauraro_object_set_attr(self, "operations", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* BankAccount__get_interest(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* arg_0_left = tauraro_value_new(); arg_0_left->type = TAURARO_NONE;
    tauraro_value_t* arg_0_right = tauraro_value_new(); arg_0_right->type = TAURARO_NONE;
    tauraro_value_t* arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    tauraro_value_t* temp_result = tauraro_int(1, (tauraro_value_t*[]){arg_0});
    return temp_result;
}


tauraro_value_t* Builder__add(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* n = (argc > 1) ? argv[1] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "value");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = n;
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    tauraro_object_set_attr(self, "value", temp_result);
    tauraro_value_t* temp_result_2 = self;
    return temp_result;
}


tauraro_value_t* Dog__speak(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "name");
    tauraro_value_t* binop_left = temp_result;
    tauraro_value_t* binop_right = tauraro_value_new(); binop_right->type = TAURARO_STRING; binop_right->data.str_val = strdup(" says Woof!");
    tauraro_value_t* temp_result_1 = tauraro_add(binop_left, binop_right);
    return temp_result;
}


tauraro_value_t* Counter__get_count(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(Counter, "count");
    return temp_result;
}


tauraro_value_t* Builder__get_value(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_object_get_attr(self, "value");
    return temp_result;
}


tauraro_value_t* Manager____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* name = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* age = (argc > 2) ? argv[2] : NULL;
    tauraro_value_t* employee_id = (argc > 3) ? argv[3] : NULL;
    tauraro_value_t* department = (argc > 4) ? argv[4] : NULL;

    // Local variables
    tauraro_value_t* temp_result = self;
    tauraro_value_t* method_arg_0 = temp_result;
    tauraro_value_t* temp_result_1 = name;
    tauraro_value_t* method_arg_1 = temp_result;
    tauraro_value_t* temp_result_2 = age;
    tauraro_value_t* method_arg_2 = temp_result;
    tauraro_value_t* temp_result_3 = employee_id;
    tauraro_value_t* method_arg_3 = temp_result;
    tauraro_value_t* temp_result_4 = Employee____init__(5, (tauraro_value_t*[]){Employee, method_arg_0, method_arg_1, method_arg_2, method_arg_3});
    tauraro_value_t* temp_result_5 = department;
    tauraro_object_set_attr(self, "department", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Square____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;
    tauraro_value_t* color = (argc > 1) ? argv[1] : NULL;
    tauraro_value_t* side = (argc > 2) ? argv[2] : NULL;

    // Local variables
    tauraro_value_t* temp_result = self;
    tauraro_value_t* method_arg_0 = temp_result;
    tauraro_value_t* temp_result_1 = color;
    tauraro_value_t* method_arg_1 = temp_result;
    tauraro_value_t* temp_result_2 = side;
    tauraro_value_t* method_arg_2 = temp_result;
    tauraro_value_t* temp_result_3 = side;
    tauraro_value_t* method_arg_3 = temp_result;
    tauraro_value_t* temp_result_4 = Rectangle____init__(5, (tauraro_value_t*[]){Rectangle, method_arg_0, method_arg_1, method_arg_2, method_arg_3});
    tauraro_value_t* temp_result_5 = side;
    tauraro_object_set_attr(self, "side", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


tauraro_value_t* Flyable____init__(int argc, tauraro_value_t** argv) {
    // Extract parameters
    tauraro_value_t* self = (argc > 0) ? argv[0] : NULL;

    // Local variables
    tauraro_value_t* temp_result = tauraro_value_new(); temp_result->type = TAURARO_BOOL; temp_result->data.bool_val = true;
    tauraro_object_set_attr(self, "can_fly", temp_result);
    // Implicit return None
    tauraro_value_t* none_val = tauraro_value_new();
    none_val->type = TAURARO_NONE;
    return none_val;
}


int main() {
    tauraro_value_t* arg_0 = NULL;
    tauraro_value_t* birds = NULL;
    tauraro_value_t* car = NULL;
    tauraro_value_t* acc2 = NULL;
    tauraro_value_t* bird3 = NULL;
    tauraro_value_t* duck = NULL;
    tauraro_value_t* var_result_temp = NULL;
    tauraro_value_t* var_bird3_temp = NULL;
    tauraro_value_t* acc1 = NULL;
    tauraro_value_t* var_manager_temp = NULL;
    tauraro_value_t* var_acc1_temp = NULL;
    tauraro_value_t* var_c2_temp = NULL;
    tauraro_value_t* var_cat_temp = NULL;
    tauraro_value_t* var_car_temp = NULL;
    tauraro_value_t* var_c1_temp = NULL;
    tauraro_value_t* var_duck_temp = NULL;
    tauraro_value_t* arg_1 = NULL;
    tauraro_value_t* c1 = NULL;
    tauraro_value_t* arg_0_left = NULL;
    tauraro_value_t* arg_0_left_left = NULL;
    tauraro_value_t* arg_2 = NULL;
    tauraro_value_t* square = NULL;
    tauraro_value_t* var_square_temp = NULL;
    tauraro_value_t* var_dog_temp = NULL;
    tauraro_value_t* BankAccount__total_accounts = NULL;
    tauraro_value_t* arg_3 = NULL;
    tauraro_value_t* c3 = NULL;
    tauraro_value_t* temp = NULL;
    tauraro_value_t* var_acc2_temp = NULL;
    tauraro_value_t* BankAccount = NULL;
    tauraro_value_t* var_builder_temp = NULL;
    tauraro_value_t* builder = NULL;
    tauraro_value_t* var_birds_temp = NULL;
    tauraro_value_t* var_bird1_temp = NULL;
    tauraro_value_t* bird2 = NULL;
    tauraro_value_t* golden = NULL;
    tauraro_value_t* temp_expr = NULL;
    tauraro_value_t* manager = NULL;
    tauraro_value_t* BankAccount__interest_rate = NULL;
    tauraro_value_t* method_arg_0 = NULL;
    tauraro_value_t* arg_0_right_left = NULL;
    tauraro_value_t* c2 = NULL;
    tauraro_value_t* arg_0_right = NULL;
    tauraro_value_t* result = NULL;
    tauraro_value_t* arg_0_right_right = NULL;
    tauraro_value_t* cat = NULL;
    tauraro_value_t* arg_0_left_right = NULL;
    tauraro_value_t* var_c3_temp = NULL;
    tauraro_value_t* Counter__count = NULL;
    tauraro_value_t* bird1 = NULL;
    tauraro_value_t* Counter__instances = NULL;
    tauraro_value_t* var_bird2_temp = NULL;
    tauraro_value_t* dog = NULL;
    tauraro_value_t* var_golden_temp = NULL;

    // === Class Initialization ===
    // Initialize class: Manager
    tauraro_class_t* class_Manager = tauraro_class_create("Manager", NULL);
    tauraro_class_add_method(class_Manager, "get_info", (void*)&Manager__get_info);
    tauraro_class_add_method(class_Manager, "__init__", (void*)&Manager____init__);

    // Initialize class: Flyable
    tauraro_class_t* class_Flyable = tauraro_class_create("Flyable", NULL);
    tauraro_class_add_method(class_Flyable, "fly", (void*)&Flyable__fly);
    tauraro_class_add_method(class_Flyable, "__init__", (void*)&Flyable____init__);

    // Initialize class: Counter
    tauraro_class_t* class_Counter = tauraro_class_create("Counter", NULL);
    tauraro_class_add_method(class_Counter, "__init__", (void*)&Counter____init__);
    tauraro_class_add_method(class_Counter, "get_count", (void*)&Counter__get_count);

    // Initialize class: Car
    tauraro_class_t* class_Car = tauraro_class_create("Car", NULL);
    tauraro_class_add_method(class_Car, "honk", (void*)&Car__honk);
    tauraro_class_add_method(class_Car, "__init__", (void*)&Car____init__);
    tauraro_class_add_method(class_Car, "start", (void*)&Car__start);

    // Initialize class: Animal
    tauraro_class_t* class_Animal = tauraro_class_create("Animal", NULL);
    tauraro_class_add_method(class_Animal, "__init__", (void*)&Animal____init__);
    tauraro_class_add_method(class_Animal, "speak", (void*)&Animal__speak);
    tauraro_class_add_method(class_Animal, "get_info", (void*)&Animal__get_info);

    // Initialize class: Dog
    tauraro_class_t* class_Dog = tauraro_class_create("Dog", NULL);
    tauraro_class_add_method(class_Dog, "__init__", (void*)&Dog____init__);
    tauraro_class_add_method(class_Dog, "get_breed", (void*)&Dog__get_breed);
    tauraro_class_add_method(class_Dog, "speak", (void*)&Dog__speak);

    // Initialize class: Parrot
    tauraro_class_t* class_Parrot = tauraro_class_create("Parrot", NULL);
    tauraro_class_add_method(class_Parrot, "make_sound", (void*)&Parrot__make_sound);
    tauraro_class_add_method(class_Parrot, "init__", (void*)&Parrot__init__);

    // Initialize class: Swimmable
    tauraro_class_t* class_Swimmable = tauraro_class_create("Swimmable", NULL);
    tauraro_class_add_method(class_Swimmable, "__init__", (void*)&Swimmable____init__);
    tauraro_class_add_method(class_Swimmable, "swim", (void*)&Swimmable__swim);

    // Initialize class: Square
    tauraro_class_t* class_Square = tauraro_class_create("Square", NULL);
    tauraro_class_add_method(class_Square, "diagonal", (void*)&Square__diagonal);
    tauraro_class_add_method(class_Square, "__init__", (void*)&Square____init__);

    // Initialize class: Person
    tauraro_class_t* class_Person = tauraro_class_create("Person", NULL);
    tauraro_class_add_method(class_Person, "__init__", (void*)&Person____init__);

    // Initialize class: Duck
    tauraro_class_t* class_Duck = tauraro_class_create("Duck", NULL);
    tauraro_class_add_method(class_Duck, "__init__", (void*)&Duck____init__);
    tauraro_class_add_method(class_Duck, "speak", (void*)&Duck__speak);

    // Initialize class: Shape
    tauraro_class_t* class_Shape = tauraro_class_create("Shape", NULL);
    tauraro_class_add_method(class_Shape, "__init__", (void*)&Shape____init__);
    tauraro_class_add_method(class_Shape, "get_color", (void*)&Shape__get_color);

    // Initialize class: Employee
    tauraro_class_t* class_Employee = tauraro_class_create("Employee", NULL);
    tauraro_class_add_method(class_Employee, "__init__", (void*)&Employee____init__);

    // Initialize class: Vehicle
    tauraro_class_t* class_Vehicle = tauraro_class_create("Vehicle", NULL);
    tauraro_class_add_method(class_Vehicle, "stop", (void*)&Vehicle__stop);
    tauraro_class_add_method(class_Vehicle, "start", (void*)&Vehicle__start);
    tauraro_class_add_method(class_Vehicle, "__init__", (void*)&Vehicle____init__);

    // Initialize class: Rectangle
    tauraro_class_t* class_Rectangle = tauraro_class_create("Rectangle", NULL);
    tauraro_class_add_method(class_Rectangle, "area", (void*)&Rectangle__area);
    tauraro_class_add_method(class_Rectangle, "perimeter", (void*)&Rectangle__perimeter);
    tauraro_class_add_method(class_Rectangle, "__init__", (void*)&Rectangle____init__);

    // Initialize class: BankAccount
    tauraro_class_t* class_BankAccount = tauraro_class_create("BankAccount", NULL);
    tauraro_class_add_method(class_BankAccount, "__init__", (void*)&BankAccount____init__);
    tauraro_class_add_method(class_BankAccount, "deposit", (void*)&BankAccount__deposit);
    tauraro_class_add_method(class_BankAccount, "get_interest", (void*)&BankAccount__get_interest);

    // Initialize class: Builder
    tauraro_class_t* class_Builder = tauraro_class_create("Builder", NULL);
    tauraro_class_add_method(class_Builder, "multiply", (void*)&Builder__multiply);
    tauraro_class_add_method(class_Builder, "__init__", (void*)&Builder____init__);
    tauraro_class_add_method(class_Builder, "add", (void*)&Builder__add);
    tauraro_class_add_method(class_Builder, "get_value", (void*)&Builder__get_value);

    // Initialize class: Bird
    tauraro_class_t* class_Bird = tauraro_class_create("Bird", NULL);
    tauraro_class_add_method(class_Bird, "__init__", (void*)&Bird____init__);
    tauraro_class_add_method(class_Bird, "make_sound", (void*)&Bird__make_sound);

    // Initialize class: Eagle
    tauraro_class_t* class_Eagle = tauraro_class_create("Eagle", NULL);
    tauraro_class_add_method(class_Eagle, "make_sound", (void*)&Eagle__make_sound);
    tauraro_class_add_method(class_Eagle, "init__", (void*)&Eagle__init__);

    // === End Class Initialization ===

    // !/usr/bin/env tauraro
    temp = tauraro_value_new(); temp->type = TAURARO_NONE;
    arg_0_left = strdup("=");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left_left = strdup(" ");
    arg_0_left_right = 15;
    arg_0_left = tauraro_mul(arg_0_left_left, arg_0_left_right);
    arg_0_right = tauraro_value_new(); arg_0_right->type = TAURARO_STRING; arg_0_right->data.str_val = strdup("Tauraro OOP Comprehensive Test");
    arg_0 = tauraro_add(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("=");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 1: Basic Class with Constructor
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 1] Basic Class with Constructor");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Buddy");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_STRING; arg_1->data.str_val = strdup("Dog");
    // OPTIMIZED: Static struct for Animal
    Animal_t* temp_struct = Animal_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_dog_temp = temp;
    dog = var_dog_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Whiskers");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_STRING; arg_1->data.str_val = strdup("Cat");
    // OPTIMIZED: Static struct for Animal
    Animal_t* temp_struct = Animal_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_cat_temp = temp;
    cat = var_cat_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Created:");
    // Object method call: dog.get_info()
    if (dog && dog->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_dog = (tauraro_object_t*)dog->data.obj_val;
        if (obj_dog->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_dog->class_ptr, "get_info");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){dog});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Says:");
    // Object method call: dog.speak()
    if (dog && dog->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_dog = (tauraro_object_t*)dog->data.obj_val;
        if (obj_dog->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_dog->class_ptr, "speak");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){dog});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Created:");
    // Object method call: cat.get_info()
    if (cat && cat->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_cat = (tauraro_object_t*)cat->data.obj_val;
        if (obj_cat->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_cat->class_ptr, "get_info");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){cat});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Says:");
    // Object method call: cat.speak()
    if (cat && cat->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_cat = (tauraro_object_t*)cat->data.obj_val;
        if (obj_cat->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_cat->class_ptr, "speak");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){cat});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 2: Single Inheritance
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 2] Single Inheritance");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Max");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_STRING; arg_1->data.str_val = strdup("Golden Retriever");
    // OPTIMIZED: Static struct for Dog
    Dog_t* temp_struct = Dog_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_golden_temp = temp;
    golden = var_golden_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Created:");
    // Object method call: golden.get_info()
    if (golden && golden->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_golden = (tauraro_object_t*)golden->data.obj_val;
        if (obj_golden->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_golden->class_ptr, "get_info");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){golden});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Says:");
    // Object method call: golden.speak()
    if (golden && golden->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_golden = (tauraro_object_t*)golden->data.obj_val;
        if (obj_golden->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_golden->class_ptr, "speak");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){golden});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Breed:");
    // Object method call: golden.get_breed()
    if (golden && golden->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_golden = (tauraro_object_t*)golden->data.obj_val;
        if (obj_golden->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_golden->class_ptr, "get_breed");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){golden});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 3: Multiple Inheritance
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 3] Multiple Inheritance");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Donald");
    // OPTIMIZED: Static struct for Duck
    Duck_t* temp_struct = Duck_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_duck_temp = temp;
    duck = var_duck_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Created:");
    // Object method call: duck.get_info()
    if (duck && duck->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_duck = (tauraro_object_t*)duck->data.obj_val;
        if (obj_duck->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_duck->class_ptr, "get_info");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){duck});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Says:");
    // Object method call: duck.speak()
    if (duck && duck->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_duck = (tauraro_object_t*)duck->data.obj_val;
        if (obj_duck->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_duck->class_ptr, "speak");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){duck});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Can fly:");
    arg_1 = tauraro_object_get_attr(duck, "can_fly");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Can swim:");
    arg_1 = tauraro_object_get_attr(duck, "can_swim");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Action:");
    // Object method call: duck.fly()
    if (duck && duck->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_duck = (tauraro_object_t*)duck->data.obj_val;
        if (obj_duck->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_duck->class_ptr, "fly");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){duck});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Action:");
    // Object method call: duck.swim()
    if (duck && duck->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_duck = (tauraro_object_t*)duck->data.obj_val;
        if (obj_duck->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_duck->class_ptr, "swim");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){duck});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 4: Class Attributes
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 4] Class Attributes");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    temp = tauraro_value_new(); temp->type = TAURARO_INT; temp->data.int_val = 0;
    Counter__count = temp;
    temp = tauraro_value_new(); temp->type = TAURARO_NONE;
    Counter__instances = temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("First");
    // OPTIMIZED: Static struct for Counter
    Counter_t* temp_struct = Counter_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_c1_temp = temp;
    c1 = var_c1_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Second");
    // OPTIMIZED: Static struct for Counter
    Counter_t* temp_struct = Counter_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_c2_temp = temp;
    c2 = var_c2_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Third");
    // OPTIMIZED: Static struct for Counter
    Counter_t* temp_struct = Counter_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_c3_temp = temp;
    c3 = var_c3_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Instance 1:");
    arg_1 = tauraro_object_get_attr(c1, "name");
    arg_2 = tauraro_value_new(); arg_2->type = TAURARO_STRING; arg_2->data.str_val = strdup("| Total count:");
    // Object method call: c1.get_count()
    if (c1 && c1->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_c1 = (tauraro_object_t*)c1->data.obj_val;
        if (obj_c1->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_c1->class_ptr, "get_count");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_3 = func_ptr(1, (tauraro_value_t*[]){c1});
            }
        }
    }
    tauraro_value_t* arg_2_as_value = tauraro_value_new(); arg_2_as_value->type = TAURARO_INT; arg_2_as_value->data.int_val = arg_2; tauraro_value_t* arg_3_as_value = tauraro_value_new(); arg_3_as_value->type = TAURARO_STRING; arg_3_as_value->data.str_val = arg_3; temp = tauraro_print(4, (tauraro_value_t*[]){arg_0, arg_1, arg_2_as_value, arg_3_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Instance 2:");
    arg_1 = tauraro_object_get_attr(c2, "name");
    arg_2 = tauraro_value_new(); arg_2->type = TAURARO_STRING; arg_2->data.str_val = strdup("| Total count:");
    // Object method call: c2.get_count()
    if (c2 && c2->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_c2 = (tauraro_object_t*)c2->data.obj_val;
        if (obj_c2->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_c2->class_ptr, "get_count");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_3 = func_ptr(1, (tauraro_value_t*[]){c2});
            }
        }
    }
    tauraro_value_t* arg_2_as_value = tauraro_value_new(); arg_2_as_value->type = TAURARO_INT; arg_2_as_value->data.int_val = arg_2; tauraro_value_t* arg_3_as_value = tauraro_value_new(); arg_3_as_value->type = TAURARO_STRING; arg_3_as_value->data.str_val = arg_3; temp = tauraro_print(4, (tauraro_value_t*[]){arg_0, arg_1, arg_2_as_value, arg_3_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Instance 3:");
    arg_1 = tauraro_object_get_attr(c3, "name");
    arg_2 = tauraro_value_new(); arg_2->type = TAURARO_STRING; arg_2->data.str_val = strdup("| Total count:");
    // Object method call: c3.get_count()
    if (c3 && c3->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_c3 = (tauraro_object_t*)c3->data.obj_val;
        if (obj_c3->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_c3->class_ptr, "get_count");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_3 = func_ptr(1, (tauraro_value_t*[]){c3});
            }
        }
    }
    tauraro_value_t* arg_2_as_value = tauraro_value_new(); arg_2_as_value->type = TAURARO_INT; arg_2_as_value->data.int_val = arg_2; tauraro_value_t* arg_3_as_value = tauraro_value_new(); arg_3_as_value->type = TAURARO_STRING; arg_3_as_value->data.str_val = arg_3; temp = tauraro_print(4, (tauraro_value_t*[]){arg_0, arg_1, arg_2_as_value, arg_3_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 5: Method Overriding
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 5] Method Overriding");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Tesla");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_STRING; arg_1->data.str_val = strdup("Model S");
    // OPTIMIZED: Static struct for Car
    Car_t* temp_struct = Car_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_car_temp = temp;
    car = var_car_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Vehicle:");
    arg_1 = tauraro_object_get_attr(car, "brand");
    arg_2 = tauraro_object_get_attr(car, "model");
    tauraro_value_t* arg_2_as_value = tauraro_value_new(); arg_2_as_value->type = TAURARO_INT; arg_2_as_value->data.int_val = arg_2; temp = tauraro_print(3, (tauraro_value_t*[]){arg_0, arg_1, arg_2_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Start:");
    // Object method call: car.start()
    if (car && car->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_car = (tauraro_object_t*)car->data.obj_val;
        if (obj_car->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_car->class_ptr, "start");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){car});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Stop:");
    // Object method call: car.stop()
    if (car && car->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_car = (tauraro_object_t*)car->data.obj_val;
        if (obj_car->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_car->class_ptr, "stop");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){car});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Horn:");
    // Object method call: car.honk()
    if (car && car->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_car = (tauraro_object_t*)car->data.obj_val;
        if (obj_car->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_car->class_ptr, "honk");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){car});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 6: Complex Inheritance Chain
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 6] Complex Inheritance Chain");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("red");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_INT; arg_1->data.int_val = 10;
    // OPTIMIZED: Static struct for Square
    Square_t* temp_struct = Square_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_square_temp = temp;
    square = var_square_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Shape: Square");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Color:");
    // Object method call: square.get_color()
    if (square && square->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_square = (tauraro_object_t*)square->data.obj_val;
        if (obj_square->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_square->class_ptr, "get_color");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){square});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Side:");
    arg_1 = tauraro_object_get_attr(square, "side");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Area:");
    // Object method call: square.area()
    if (square && square->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_square = (tauraro_object_t*)square->data.obj_val;
        if (obj_square->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_square->class_ptr, "area");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){square});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Perimeter:");
    // Object method call: square.perimeter()
    if (square && square->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_square = (tauraro_object_t*)square->data.obj_val;
        if (obj_square->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_square->class_ptr, "perimeter");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){square});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Diagonal:");
    // Object method call: square.diagonal()
    if (square && square->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_square = (tauraro_object_t*)square->data.obj_val;
        if (obj_square->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_square->class_ptr, "diagonal");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){square});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 7: Instance Variables vs Class Variables
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 7] Instance Variables vs Class Variables");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    temp = tauraro_value_new(); temp->type = TAURARO_FLOAT; temp->data.float_val = 0.05;
    BankAccount__interest_rate = temp;
    temp = tauraro_value_new(); temp->type = TAURARO_INT; temp->data.int_val = 0;
    BankAccount__total_accounts = temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Alice");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_INT; arg_1->data.int_val = 1000;
    // OPTIMIZED: Static struct for BankAccount
    BankAccount_t* temp_struct = BankAccount_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_acc1_temp = temp;
    acc1 = var_acc1_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Bob");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_INT; arg_1->data.int_val = 2000;
    // OPTIMIZED: Static struct for BankAccount
    BankAccount_t* temp_struct = BankAccount_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_acc2_temp = temp;
    acc2 = var_acc2_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Account 1:");
    arg_1 = tauraro_object_get_attr(acc1, "owner");
    arg_2 = tauraro_value_new(); arg_2->type = TAURARO_STRING; arg_2->data.str_val = strdup("- Balance:");
    arg_3 = tauraro_object_get_attr(acc1, "balance");
    tauraro_value_t* arg_2_as_value = tauraro_value_new(); arg_2_as_value->type = TAURARO_INT; arg_2_as_value->data.int_val = arg_2; tauraro_value_t* arg_3_as_value = tauraro_value_new(); arg_3_as_value->type = TAURARO_STRING; arg_3_as_value->data.str_val = arg_3; temp = tauraro_print(4, (tauraro_value_t*[]){arg_0, arg_1, arg_2_as_value, arg_3_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Account 2:");
    arg_1 = tauraro_object_get_attr(acc2, "owner");
    arg_2 = tauraro_value_new(); arg_2->type = TAURARO_STRING; arg_2->data.str_val = strdup("- Balance:");
    arg_3 = tauraro_object_get_attr(acc2, "balance");
    tauraro_value_t* arg_2_as_value = tauraro_value_new(); arg_2_as_value->type = TAURARO_INT; arg_2_as_value->data.int_val = arg_2; tauraro_value_t* arg_3_as_value = tauraro_value_new(); arg_3_as_value->type = TAURARO_STRING; arg_3_as_value->data.str_val = arg_3; temp = tauraro_print(4, (tauraro_value_t*[]){arg_0, arg_1, arg_2_as_value, arg_3_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Total accounts:");
    arg_1 = tauraro_object_get_attr(BankAccount, "total_accounts");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    method_arg_0 = 500;
    tauraro_value_t* acc1_as_value = tauraro_value_new(); acc1_as_value->type = TAURARO_INT; acc1_as_value->data.int_val = acc1; tauraro_value_t* method_arg_0_as_value = tauraro_value_new(); method_arg_0_as_value->type = TAURARO_INT; method_arg_0_as_value->data.int_val = method_arg_0; temp = tauraro_acc1__deposit(2, (tauraro_value_t*[]){acc1_as_value, method_arg_0_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  After deposit - Account 1 balance:");
    arg_1 = tauraro_object_get_attr(acc1, "balance");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Interest for Account 1:");
    tauraro_value_t* acc1_as_value = tauraro_value_new(); acc1_as_value->type = TAURARO_INT; acc1_as_value->data.int_val = acc1; arg_1 = tauraro_acc1__get_interest(1, (tauraro_value_t*[]){acc1_as_value});
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Interest for Account 2:");
    tauraro_value_t* acc2_as_value = tauraro_value_new(); acc2_as_value->type = TAURARO_INT; acc2_as_value->data.int_val = acc2; arg_1 = tauraro_acc2__get_interest(1, (tauraro_value_t*[]){acc2_as_value});
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 8: Method Chaining
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 8] Method Chaining");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // OPTIMIZED: Static struct for Builder
    Builder_t* temp_struct = Builder_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_builder_temp = temp;
    builder = var_builder_temp;
    // Object method call: temp_expr.get_value()
    if (temp_expr && temp_expr->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_temp_expr = (tauraro_object_t*)temp_expr->data.obj_val;
        if (obj_temp_expr->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_temp_expr->class_ptr, "get_value");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                temp = func_ptr(1, (tauraro_value_t*[]){temp_expr});
            }
        }
    }
    var_result_temp = temp;
    result = var_result_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Operations: add(5).add(10).multiply(2)");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Result:");
    arg_1 = result;
    arg_2 = tauraro_value_new(); arg_2->type = TAURARO_STRING; arg_2->data.str_val = strdup("| Expected: 30");
    tauraro_value_t* arg_2_as_value = tauraro_value_new(); arg_2_as_value->type = TAURARO_INT; arg_2_as_value->data.int_val = arg_2; temp = tauraro_print(3, (tauraro_value_t*[]){arg_0, arg_1, arg_2_as_value});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 9: Polymorphism
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 9] Polymorphism");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    temp = tauraro_value_new(); temp->type = TAURARO_NONE;
    var_birds_temp = temp;
    birds = var_birds_temp;
    // Manually construct list since we have issues with append
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Sparrow");
    // OPTIMIZED: Static struct for Bird
    Bird_t* temp_struct = Bird_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_bird1_temp = temp;
    bird1 = var_bird1_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Polly");
    // OPTIMIZED: Static struct for Parrot
    Parrot_t* temp_struct = Parrot_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_bird2_temp = temp;
    bird2 = var_bird2_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Thunder");
    // OPTIMIZED: Static struct for Eagle
    Eagle_t* temp_struct = Eagle_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_bird3_temp = temp;
    bird3 = var_bird3_temp;
    arg_0_left = strdup("  ");
    // Object method call: bird1.make_sound()
    if (bird1 && bird1->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_bird1 = (tauraro_object_t*)bird1->data.obj_val;
        if (obj_bird1->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_bird1->class_ptr, "make_sound");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_0_right = func_ptr(1, (tauraro_value_t*[]){bird1});
            }
        }
    }
    arg_0 = tauraro_add(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("  ");
    // Object method call: bird2.make_sound()
    if (bird2 && bird2->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_bird2 = (tauraro_object_t*)bird2->data.obj_val;
        if (obj_bird2->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_bird2->class_ptr, "make_sound");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_0_right = func_ptr(1, (tauraro_value_t*[]){bird2});
            }
        }
    }
    arg_0 = tauraro_add(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("  ");
    // Object method call: bird3.make_sound()
    if (bird3 && bird3->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_bird3 = (tauraro_object_t*)bird3->data.obj_val;
        if (obj_bird3->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_bird3->class_ptr, "make_sound");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_0_right = func_ptr(1, (tauraro_value_t*[]){bird3});
            }
        }
    }
    arg_0 = tauraro_add(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Test 10: Constructor Chaining
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n[Test 10] Constructor Chaining");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("-");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("Alice");
    arg_1 = tauraro_value_new(); arg_1->type = TAURARO_INT; arg_1->data.int_val = 35;
    arg_2 = 12345;
    arg_3 = strdup("Engineering");
    // OPTIMIZED: Static struct for Manager
    Manager_t* temp_struct = Manager_new();
    temp = tauraro_value_new();
    temp->type = TAURARO_OBJECT;
    temp->data.ptr_val = (void*)temp_struct;
    var_manager_temp = temp;
    manager = var_manager_temp;
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Created:");
    // Object method call: manager.get_info()
    if (manager && manager->type == TAURARO_OBJECT) {
        tauraro_object_t* obj_manager = (tauraro_object_t*)manager->data.obj_val;
        if (obj_manager->class_ptr) {
            tauraro_value_t* method = tauraro_class_get_method(obj_manager->class_ptr, "get_info");
            if (method && method->type == TAURARO_FUNCTION) {
                // Call method function pointer with self
                typedef tauraro_value_t* (*method_func_t)(int, tauraro_value_t**);
                method_func_t func_ptr = (method_func_t)method->data.ptr_val;
                arg_1 = func_ptr(1, (tauraro_value_t*[]){manager});
            }
        }
    }
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Name:");
    arg_1 = tauraro_object_get_attr(manager, "name");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Age:");
    arg_1 = tauraro_object_get_attr(manager, "age");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ID:");
    arg_1 = tauraro_object_get_attr(manager, "employee_id");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  Department:");
    arg_1 = tauraro_object_get_attr(manager, "department");
    temp = tauraro_print(2, (tauraro_value_t*[]){arg_0, arg_1});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ PASS");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    // Final Summary
    arg_0_left = strdup("\n");
    arg_0_right_left = strdup("=");
    arg_0_right_right = 70;
    arg_0_right = tauraro_mul(arg_0_right_left, arg_0_right_right);
    arg_0 = tauraro_add(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left_left = strdup(" ");
    arg_0_left_right = 25;
    arg_0_left = tauraro_mul(arg_0_left_left, arg_0_left_right);
    arg_0_right = tauraro_value_new(); arg_0_right->type = TAURARO_STRING; arg_0_right->data.str_val = strdup("TEST SUMMARY");
    arg_0 = tauraro_add(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("=");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n  ✓ Test 1: Basic Class with Constructor");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 2: Single Inheritance");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 3: Multiple Inheritance");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 4: Class Attributes");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 5: Method Overriding");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 6: Complex Inheritance Chain");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 7: Instance vs Class Variables");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 8: Method Chaining");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 9: Polymorphism");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("  ✓ Test 10: Constructor Chaining");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n  ALL 10 TESTS PASSED!");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0 = tauraro_value_new(); arg_0->type = TAURARO_STRING; arg_0->data.str_val = strdup("\n  Tauraro OOP Features: FULLY OPERATIONAL");
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    arg_0_left = strdup("=");
    arg_0_right = 70;
    arg_0 = tauraro_mul(arg_0_left, arg_0_right);
    temp = tauraro_print(1, (tauraro_value_t*[]){arg_0});
    return 0;
}
