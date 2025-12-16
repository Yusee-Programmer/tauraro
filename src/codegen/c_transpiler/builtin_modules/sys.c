// ==========================================
// SYS MODULE - Pure C Implementation
// ==========================================
// Provides: sys.argv, sys.exit(), sys.platform, sys.version
// Platform: Cross-platform (Windows/Linux/macOS)

#include <stdlib.h>
#include <string.h>

// Global sys module state (initialized in main)
static TauValue g_sys_argv = {0};
static TauValue g_sys_platform = {0};
static TauValue g_sys_version = {0};

// Initialize sys module with command line arguments
static inline void tauraro_sys_init(int argc, char* argv[]) {
    // Create sys.argv list
    TauList* argv_list = malloc(sizeof(TauList));
    argv_list->size = argc;
    argv_list->capacity = argc;
    argv_list->items = malloc(sizeof(TauValue) * argc);

    for (int i = 0; i < argc; i++) {
        argv_list->items[i] = tauraro_string(strdup(argv[i]));
    }

    g_sys_argv = (TauValue){
        .type = 4,              // List type
        .value.list = argv_list,
        .refcount = 1,
        .next = NULL
    };

    // Set sys.platform
    #ifdef _WIN32
        g_sys_platform = tauraro_string("win32");
    #elif defined(__APPLE__)
        g_sys_platform = tauraro_string("darwin");
    #elif defined(__linux__)
        g_sys_platform = tauraro_string("linux");
    #else
        g_sys_platform = tauraro_string("unknown");
    #endif

    // Set sys.version
    g_sys_version = tauraro_string("Tauraro 0.1.0 (C transpiled)");
}

// sys.argv - Access command line arguments
static inline TauValue tauraro_sys_argv_get(void) {
    return g_sys_argv;
}

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

// sys.platform - Get platform string
static inline TauValue tauraro_sys_platform_get(void) {
    return g_sys_platform;
}

// sys.version - Get version string
static inline TauValue tauraro_sys_version_get(void) {
    return g_sys_version;
}
