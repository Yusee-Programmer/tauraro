// ==========================================
// ABC MODULE - Pure C Implementation
// ==========================================
// Provides: abc.ABC, abc.abstractmethod, abc.ABCMeta
// Platform: Cross-platform

#ifndef TAURARO_ABC_MODULE_H
#define TAURARO_ABC_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifndef TAU_HELPER_FUNCTIONS_DEFINED
#define TAU_HELPER_FUNCTIONS_DEFINED

static inline double tau_to_double(TauValue v) {
    if (v.type == 0) return (double)v.value.i;
    if (v.type == 1) return v.value.f;
    return 0.0;
}

static inline int64_t tau_to_int64(TauValue v) {
    if (v.type == 0) return v.value.i;
    if (v.type == 1) return (int64_t)v.value.f;
    return 0;
}

static inline bool tau_to_bool(TauValue v) {
    if (v.type == 3) return v.value.i != 0;
    if (v.type == 0) return v.value.i != 0;
    if (v.type == 1) return v.value.f != 0.0;
    if (v.type == 2) return v.value.s != NULL && v.value.s[0] != '\0';
    return true;
}

static inline char* tau_to_string(TauValue v) {
    if (v.type == 2) return v.value.s;
    return NULL;
}
#endif // TAU_HELPER_FUNCTIONS_DEFINED

// abc.ABC - Base class for Abstract Base Classes
static inline TauValue tauraro_abc_ABC(void) {
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};  // Abstract base class
}

// abc.abstractmethod(func) - Mark method as abstract
static inline TauValue tauraro_abc_abstractmethod(TauValue func) {
    // Return the function marked as abstract
    // In C transpilation, abstract methods are just regular methods
    return func;
}

// abc.ABCMeta - Metaclass for abstract base classes
static inline TauValue tauraro_abc_ABCMeta(TauValue name, TauValue bases, TauValue attrs) {
    // Return a new class with ABCMeta as metaclass
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// abc.get_cache_token() - Get cache token
static inline TauValue tauraro_abc_get_cache_token(void) {
    return (TauValue){.type = 0, .value.i = 1, .refcount = 1, .next = NULL};
}

// abc.ABCMeta.register(cls, subclass) - Register a virtual subclass
static inline TauValue tauraro_abc_ABCMeta_register(TauValue cls, TauValue subclass) {
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};  // Return success
}


#endif // TAURARO_ABC_MODULE_H
