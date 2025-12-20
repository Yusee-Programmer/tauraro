// ==========================================
// IMPORTLIB MODULE - Enhanced Pure C Implementation
// ==========================================
// Provides: import_module, reload, find_loader, find_spec
// Platform: Cross-platform

#ifndef TAURARO_IMPORTLIB_MODULE_H
#define TAURARO_IMPORTLIB_MODULE_H

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

// Module cache
typedef struct {
    char** module_names;
    void** modules;
    int count;
    int capacity;
} ModuleCache;

static ModuleCache g_module_cache = {NULL, NULL, 0, 100};

// importlib.import_module(name, package)
static inline TauValue tauraro_importlib_import_module(TauValue name, TauValue package) {
    if (name.type != 2) {
        return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
    }
    
    // Check cache
    for (int i = 0; i < g_module_cache.count; i++) {
        if (strcmp(g_module_cache.module_names[i], name.value.s) == 0) {
            return (TauValue){.type = 6, .value.ptr = g_module_cache.modules[i], .refcount = 1, .next = NULL};
        }
    }
    
    // Create module stub
    typedef struct {
        char* name;
        TauDict* dict;
    } Module;
    
    Module* mod = (Module*)malloc(sizeof(Module));
    mod->name = (char*)malloc(strlen(name.value.s) + 1);
    strcpy(mod->name, name.value.s);
    mod->dict = (TauDict*)malloc(sizeof(TauDict));
    mod->dict->size = 0;
    mod->dict->capacity = 0;
    mod->dict->keys = NULL;
    mod->dict->values = NULL;
    
    // Cache it
    if (g_module_cache.count >= g_module_cache.capacity) {
        g_module_cache.capacity *= 2;
        g_module_cache.module_names = (char**)realloc(g_module_cache.module_names, sizeof(char*) * g_module_cache.capacity);
        g_module_cache.modules = (void**)realloc(g_module_cache.modules, sizeof(void*) * g_module_cache.capacity);
    }
    
    g_module_cache.module_names[g_module_cache.count] = (char*)malloc(strlen(name.value.s) + 1);
    strcpy(g_module_cache.module_names[g_module_cache.count], name.value.s);
    g_module_cache.modules[g_module_cache.count] = (void*)mod;
    g_module_cache.count++;
    
    return (TauValue){.type = 6, .value.ptr = (void*)mod, .refcount = 1, .next = NULL};
}

// importlib.reload(module)
static inline TauValue tauraro_importlib_reload(TauValue module) {
    if (module.type != 6) {
        return module;
    }
    // Simplified: just return the module as-is
    return module;
}

// importlib.find_loader(name)
static inline TauValue tauraro_importlib_find_loader(TauValue name) {
    if (name.type != 2) {
        return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
    }
    
    typedef struct {
        char* module_name;
        char* loader_type;
    } Loader;
    
    Loader* loader = (Loader*)malloc(sizeof(Loader));
    loader->module_name = (char*)malloc(strlen(name.value.s) + 1);
    strcpy(loader->module_name, name.value.s);
    loader->loader_type = "SourceFileLoader";
    
    return (TauValue){.type = 6, .value.ptr = (void*)loader, .refcount = 1, .next = NULL};
}

// importlib.find_spec(name, package)
static inline TauValue tauraro_importlib_find_spec(TauValue name, TauValue package) {
    if (name.type != 2) {
        return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
    }
    
    typedef struct {
        char* name;
        char* origin;
        int cached;
    } ModuleSpec;
    
    ModuleSpec* spec = (ModuleSpec*)malloc(sizeof(ModuleSpec));
    spec->name = (char*)malloc(strlen(name.value.s) + 1);
    strcpy(spec->name, name.value.s);
    spec->origin = "";
    spec->cached = 1;
    
    return (TauValue){.type = 6, .value.ptr = (void*)spec, .refcount = 1, .next = NULL};
}

// importlib.invalidate_caches()
static inline TauValue tauraro_importlib_invalidate_caches(void) {
    // Clear the module cache
    for (int i = 0; i < g_module_cache.count; i++) {
        free(g_module_cache.module_names[i]);
        // Free module contents
    }
    g_module_cache.count = 0;
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

#endif // TAURARO_IMPORTLIB_MODULE_H
