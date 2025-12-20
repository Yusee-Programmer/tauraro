// ==========================================
// SYS MODULE - Pure C Implementation
// ==========================================
// Provides: sys.argv, sys.exit(), sys.platform, sys.version, and all Python sys module functions
// Platform: Cross-platform (Windows/Linux/macOS)

#ifndef TAURARO_SYS_MODULE_H
#define TAURARO_SYS_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

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

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// Global sys module state (initialized in main)
static TauValue g_sys_argv = {0};
static TauValue g_sys_platform = {0};
static TauValue g_sys_version = {0};
static TauValue g_sys_version_info = {0};
static TauValue g_sys_path = {0};
static TauValue g_sys_builtin_module_names = {0};

// Initialize sys module with command line arguments
static inline void tauraro_sys_init(int argc, char* argv[]) {
    // Create sys.argv list
    TauList* argv_list = malloc(sizeof(TauList));
    argv_list->size = argc;
    argv_list->capacity = argc;
    argv_list->items = malloc(sizeof(TauValue) * argc);

    for (int i = 0; i < argc; i++) {
        argv_list->items[i] = (TauValue){.type = 2, .value.s = strdup(argv[i]), .refcount = 1, .next = NULL};
    }

    g_sys_argv = (TauValue){
        .type = 4,              // List type
        .value.list = argv_list,
        .refcount = 1,
        .next = NULL
    };

    // Set sys.platform
    #ifdef _WIN32
        g_sys_platform = (TauValue){.type = 2, .value.s = "win32", .refcount = 1, .next = NULL};
    #elif defined(__APPLE__)
        g_sys_platform = (TauValue){.type = 2, .value.s = "darwin", .refcount = 1, .next = NULL};
    #elif defined(__linux__)
        g_sys_platform = (TauValue){.type = 2, .value.s = "linux", .refcount = 1, .next = NULL};
    #else
        g_sys_platform = (TauValue){.type = 2, .value.s = "unknown", .refcount = 1, .next = NULL};
    #endif

    // Set sys.version
    g_sys_version = (TauValue){.type = 2, .value.s = "Tauraro 0.1.0 (C transpiled)", .refcount = 1, .next = NULL};

    // Set sys.version_info (major, minor, micro, releaselevel, serial)
    // Stored as tuple: (0, 1, 0, "final", 0)
    TauList* version_info = malloc(sizeof(TauList));
    version_info->size = 5;
    version_info->capacity = 5;
    version_info->items = malloc(sizeof(TauValue) * 5);
    version_info->items[0] = (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};      // major
    version_info->items[1] = (TauValue){.type = 0, .value.i = 1, .refcount = 1, .next = NULL};      // minor
    version_info->items[2] = (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};      // micro
    version_info->items[3] = (TauValue){.type = 2, .value.s = "final", .refcount = 1, .next = NULL}; // releaselevel
    version_info->items[4] = (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};      // serial

    g_sys_version_info = (TauValue){
        .type = 4,  // Tuple (stored as list)
        .value.list = version_info,
        .refcount = 1,
        .next = NULL
    };

    // Initialize sys.path
    TauList* path_list = malloc(sizeof(TauList));
    path_list->size = 4;
    path_list->capacity = 4;
    path_list->items = malloc(sizeof(TauValue) * 4);
    path_list->items[0] = (TauValue){.type = 2, .value.s = ".", .refcount = 1, .next = NULL};
    path_list->items[1] = (TauValue){.type = 2, .value.s = "tauraro_packages", .refcount = 1, .next = NULL};
    path_list->items[2] = (TauValue){.type = 2, .value.s = "tauraro_packages/externals", .refcount = 1, .next = NULL};
    path_list->items[3] = (TauValue){.type = 2, .value.s = "tauraro_packages/pysites", .refcount = 1, .next = NULL};

    g_sys_path = (TauValue){
        .type = 4,  // List
        .value.list = path_list,
        .refcount = 1,
        .next = NULL
    };

    // Initialize builtin module names
    TauList* builtin_mods = malloc(sizeof(TauList));
    const char* mods[] = {
        "sys", "os", "time", "datetime", "math", "random", "json", "csv", "io",
        "re", "threading", "multiprocessing", "asyncio", "socket", "subprocess",
        "base64", "hashlib", "pickle", "logging", "unittest", "copy", "functools",
        "itertools", "collections", "abc", "exceptions", "uuid", "secrets", "urllib",
        "websockets", "httpx", "httptools", "templa", "serveit", "orm"
    };
    int num_modules = sizeof(mods) / sizeof(mods[0]);

    builtin_mods->size = num_modules;
    builtin_mods->capacity = num_modules;
    builtin_mods->items = malloc(sizeof(TauValue) * num_modules);

    for (int i = 0; i < num_modules; i++) {
        builtin_mods->items[i] = (TauValue){.type = 2, .value.s = (char*)mods[i], .refcount = 1, .next = NULL};
    }

    g_sys_builtin_module_names = (TauValue){
        .type = 4,  // List
        .value.list = builtin_mods,
        .refcount = 1,
        .next = NULL
    };
}

// ==========================================
// MODULE ATTRIBUTE ACCESSORS
// ==========================================

static inline TauValue tauraro_sys_argv_get(void) {
    return g_sys_argv;
}

static inline TauValue tauraro_sys_platform_get(void) {
    return g_sys_platform;
}

static inline TauValue tauraro_sys_version_get(void) {
    return g_sys_version;
}

static inline TauValue tauraro_sys_version_info_get(void) {
    return g_sys_version_info;
}

static inline TauValue tauraro_sys_path_get(void) {
    return g_sys_path;
}

static inline TauValue tauraro_sys_builtin_module_names_get(void) {
    return g_sys_builtin_module_names;
}

// ==========================================
// SYSTEM INFORMATION FUNCTIONS
// ==========================================

// sys.exit(code) - Exit program with status code
static inline void tauraro_sys_exit(TauValue code) {
    int exit_code = 0;
    if (code.type == 0) {
        exit_code = (int)code.value.i;
    } else if (code.type == 1) {
        exit_code = (int)code.value.f;
    }
    exit(exit_code);
}

// sys.getrefcount(object) - Return reference count
static inline TauValue tauraro_sys_getrefcount(TauValue obj) {
    // In C, we use refcount field if available
    return (TauValue){.type = 0, .value.i = obj.refcount, .refcount = 1, .next = NULL};
}

// sys.getsizeof(object) - Return size of object in bytes
static inline TauValue tauraro_sys_getsizeof(TauValue obj) {
    int64_t size = 0;
    
    switch (obj.type) {
        case 0:  // Int
            size = 28;  // Python int object size
            break;
        case 1:  // Float
            size = 24;
            break;
        case 2:  // String
            if (obj.value.s) {
                size = strlen(obj.value.s) + 49;  // String overhead
            }
            break;
        case 3:  // Bool
            size = 28;
            break;
        case 4:  // List
            if (obj.value.list) {
                size = 56 + (obj.value.list->size * 8);  // List overhead + items
            }
            break;
        case 5:  // Dict
            if (obj.value.dict) {
                size = 72 + (obj.value.dict->size * 24);  // Dict overhead + entries
            }
            break;
        default:
            size = 64;  // Generic object
    }
    
    return (TauValue){.type = 0, .value.i = size, .refcount = 1, .next = NULL};
}

// sys.intern(string) - Intern a string
static inline TauValue tauraro_sys_intern(TauValue s) {
    // In C, we just return the string (no real interning)
    return s;
}

// sys.byteorder - Get byte order ('little' or 'big')
static inline TauValue tauraro_sys_byteorder_get(void) {
    #ifdef __BIG_ENDIAN__
        return (TauValue){.type = 2, .value.s = "big", .refcount = 1, .next = NULL};
    #else
        return (TauValue){.type = 2, .value.s = "little", .refcount = 1, .next = NULL};
    #endif
}

// sys.maxsize - Maximum int value
static inline TauValue tauraro_sys_maxsize_get(void) {
    return (TauValue){.type = 0, .value.i = INT64_MAX, .refcount = 1, .next = NULL};
}

// sys.executable - Get Python executable path
static inline TauValue tauraro_sys_executable_get(void) {
    #ifdef _WIN32
        char* exe = getenv("TAURARO_EXE");
        if (!exe) exe = "tauraro.exe";
    #else
        char* exe = getenv("TAURARO_EXE");
        if (!exe) exe = "/usr/local/bin/tauraro";
    #endif
    
    return (TauValue){.type = 2, .value.s = exe, .refcount = 1, .next = NULL};
}

// sys.copyright - Get copyright message
static inline TauValue tauraro_sys_copyright_get(void) {
    return (TauValue){.type = 2, .value.s = "Copyright (c) 2024 Tauraro Project", .refcount = 1, .next = NULL};
}

// sys.api_version - Get API version
static inline TauValue tauraro_sys_api_version_get(void) {
    return (TauValue){.type = 0, .value.i = 1, .refcount = 1, .next = NULL};
}

// sys.dont_write_bytecode - Get bytecode writing flag
static inline TauValue tauraro_sys_dont_write_bytecode_get(void) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // False
}

// ==========================================
// PATH MANIPULATION FUNCTIONS
// ==========================================

// sys.path_append(path) - Append to sys.path
static inline TauValue tauraro_sys_path_append(TauValue path_str) {
    if (g_sys_path.type == 4 && g_sys_path.value.list && path_str.type == 2) {
        TauList* path = g_sys_path.value.list;
        
        // Expand if needed
        if (path->size >= path->capacity) {
            path->capacity *= 2;
            path->items = realloc(path->items, sizeof(TauValue) * path->capacity);
        }
        
        path->items[path->size++] = path_str;
    }
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// sys.path_insert(index, path) - Insert into sys.path
static inline TauValue tauraro_sys_path_insert(TauValue idx, TauValue path_str) {
    if (g_sys_path.type == 4 && g_sys_path.value.list && path_str.type == 2 && idx.type == 0) {
        TauList* path = g_sys_path.value.list;
        int index = (int)idx.value.i;
        
        if (index < 0 || index > (int)path->size) {
            return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
        }
        
        // Expand if needed
        if (path->size >= path->capacity) {
            path->capacity *= 2;
            path->items = realloc(path->items, sizeof(TauValue) * path->capacity);
        }
        
        // Shift items
        for (size_t i = path->size; i > (size_t)index; i--) {
            path->items[i] = path->items[i - 1];
        }
        
        path->items[index] = path_str;
        path->size++;
    }
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// sys.path_remove(path) - Remove from sys.path
static inline TauValue tauraro_sys_path_remove(TauValue path_str) {
    if (g_sys_path.type == 4 && g_sys_path.value.list && path_str.type == 2) {
        TauList* path = g_sys_path.value.list;
        
        for (size_t i = 0; i < path->size; i++) {
            if (path->items[i].type == 2 && strcmp(path->items[i].value.s, path_str.value.s) == 0) {
                // Shift items
                for (size_t j = i; j < path->size - 1; j++) {
                    path->items[j] = path->items[j + 1];
                }
                path->size--;
                break;
            }
        }
    }
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

#endif // TAURARO_SYS_MODULE_H
