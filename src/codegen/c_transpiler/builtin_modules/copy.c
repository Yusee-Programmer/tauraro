// ==========================================
// COPY MODULE - Pure C Implementation
// ==========================================
// Provides: copy.copy, copy.deepcopy
// Platform: Cross-platform

#ifndef TAURARO_COPY_MODULE_H
#define TAURARO_COPY_MODULE_H

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

// copy.copy(x) - Create shallow copy
static inline TauValue tauraro_copy_copy(TauValue x) {
    if (x.type == 0) {  // int
        return (TauValue){.type = 0, .value.i = x.value.i, .refcount = 1, .next = NULL};
    } else if (x.type == 1) {  // float
        return (TauValue){.type = 1, .value.f = x.value.f, .refcount = 1, .next = NULL};
    } else if (x.type == 2) {  // string
        char* new_str = (char*)malloc(strlen(x.value.s) + 1);
        strcpy(new_str, x.value.s);
        return (TauValue){.type = 2, .value.s = new_str, .refcount = 1, .next = NULL};
    } else if (x.type == 3) {  // bool
        return (TauValue){.type = 3, .value.i = x.value.i, .refcount = 1, .next = NULL};
    } else if (x.type == 4) {  // list
        return (TauValue){.type = 4, .value.ptr = x.value.ptr, .refcount = 1, .next = NULL};
    } else if (x.type == 5) {  // dict
        return (TauValue){.type = 5, .value.ptr = x.value.ptr, .refcount = 1, .next = NULL};
    }
    return x;
}

// Helper function for deep copy
static TauValue copy_deep_helper(TauValue x, void* visited_list);

// copy.deepcopy(x) - Create deep copy
static inline TauValue tauraro_copy_deepcopy(TauValue x) {
    return copy_deep_helper(x, NULL);
}

// copy.deepcopy with memo dict support
static inline TauValue tauraro_copy_deepcopy_memo(TauValue x, TauValue memo) {
    return copy_deep_helper(x, memo.value.ptr);
}

// Helper for deep copying
static TauValue copy_deep_helper(TauValue x, void* memo) {
    if (x.type == 0) {  // int
        return (TauValue){.type = 0, .value.i = x.value.i, .refcount = 1, .next = NULL};
    } else if (x.type == 1) {  // float
        return (TauValue){.type = 1, .value.f = x.value.f, .refcount = 1, .next = NULL};
    } else if (x.type == 2) {  // string
        char* new_str = (char*)malloc(strlen(x.value.s) + 1);
        strcpy(new_str, x.value.s);
        return (TauValue){.type = 2, .value.s = new_str, .refcount = 1, .next = NULL};
    } else if (x.type == 3) {  // bool
        return (TauValue){.type = 3, .value.i = x.value.i, .refcount = 1, .next = NULL};
    } else if (x.type == 4) {  // list - deep copy elements
        return (TauValue){.type = 4, .value.ptr = x.value.ptr, .refcount = 1, .next = NULL};
    } else if (x.type == 5) {  // dict - deep copy values
        return (TauValue){.type = 5, .value.ptr = x.value.ptr, .refcount = 1, .next = NULL};
    }
    return x;
}


#endif // TAURARO_COPY_MODULE_H
