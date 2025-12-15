//! sys Module Support for C Transpilation
//!
//! This module provides implementation of Python's sys module functionality
//! in C, including sys.argv, sys.exit, sys.platform, etc.
//! Uses the correct TauValue/TauList type system.

/// Generate sys module global variable declaration
pub fn generate_sys_module_globals() -> String {
    r#"// Global sys module instance (initialized in main)
static TauModule* g_sys_module = NULL;

"#.to_string()
}

/// Generate sys module initialization function
/// This creates a TauModule with populated exports for sys.argv, sys.platform, etc.
pub fn generate_sys_module_init() -> String {
    r#"// Initialize sys module and populate it with system information
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
"#.to_string()
}

/// Generate complete sys module support code
pub fn generate_sys_module_complete() -> String {
    let mut output = String::new();
    output.push_str(&generate_sys_module_globals());
    output.push_str(&generate_sys_module_init());
    output
}
