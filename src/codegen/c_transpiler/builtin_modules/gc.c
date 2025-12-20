// ==========================================
// GC MODULE - Enhanced Pure C Implementation
// ==========================================
// Provides: garbage collection interface
// Platform: Cross-platform

#ifndef TAURARO_GC_MODULE_H
#define TAURARO_GC_MODULE_H

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

// GC statistics
typedef struct {
    int collections;
    int objects;
    int unreachable;
    int enabled;
} GCStats;

static GCStats g_gc_stats = {0, 0, 0, 1};

// gc.collect() - Run garbage collection
static inline TauValue tauraro_gc_collect(void) {
    if (!g_gc_stats.enabled) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    // Simplified GC: just count unreachable objects
    int unreachable = g_gc_stats.unreachable;
    g_gc_stats.unreachable = 0;
    g_gc_stats.collections++;
    
    return (TauValue){.type = 0, .value.i = unreachable, .refcount = 1, .next = NULL};
}

// gc.enable() - Enable automatic garbage collection
static inline TauValue tauraro_gc_enable(void) {
    g_gc_stats.enabled = 1;
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// gc.disable() - Disable automatic garbage collection
static inline TauValue tauraro_gc_disable(void) {
    g_gc_stats.enabled = 0;
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// gc.isenabled() - Check if GC is enabled
static inline TauValue tauraro_gc_isenabled(void) {
    return (TauValue){.type = 3, .value.i = g_gc_stats.enabled, .refcount = 1, .next = NULL};
}

// gc.get_objects() - Get all tracked objects
static inline TauValue tauraro_gc_get_objects(void) {
    TauList* objects = (TauList*)malloc(sizeof(TauList));
    objects->size = g_gc_stats.objects;
    objects->capacity = g_gc_stats.objects * 2;
    
    if (g_gc_stats.objects > 0) {
        objects->items = (TauValue*)malloc(sizeof(TauValue) * objects->capacity);
    } else {
        objects->items = NULL;
    }
    
    return (TauValue){.type = 4, .value.list = objects, .refcount = 1, .next = NULL};
}

// gc.set_debug(flags)
static inline TauValue tauraro_gc_set_debug(TauValue flags) {
    // Simplified: just accept the flags
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// gc.get_debug() - Get debug flags
static inline TauValue tauraro_gc_get_debug(void) {
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// gc.get_referents(*objs) - Get referents of objects
static inline TauValue tauraro_gc_get_referents(TauValue objs) {
    TauList* referents = (TauList*)malloc(sizeof(TauList));
    referents->size = 0;
    referents->capacity = 10;
    referents->items = (TauValue*)malloc(sizeof(TauValue) * referents->capacity);
    
    return (TauValue){.type = 4, .value.list = referents, .refcount = 1, .next = NULL};
}

// gc.get_referrers(*objs) - Get objects referring to objects
static inline TauValue tauraro_gc_get_referrers(TauValue objs) {
    TauList* referrers = (TauList*)malloc(sizeof(TauList));
    referrers->size = 0;
    referrers->capacity = 10;
    referrers->items = (TauValue*)malloc(sizeof(TauValue) * referrers->capacity);
    
    return (TauValue){.type = 4, .value.list = referrers, .refcount = 1, .next = NULL};
}

// gc.is_tracked(obj) - Check if object is tracked
static inline TauValue tauraro_gc_is_tracked(TauValue obj) {
    int tracked = (obj.refcount > 0);
    return (TauValue){.type = 3, .value.i = tracked, .refcount = 1, .next = NULL};
}

#endif // TAURARO_GC_MODULE_H
