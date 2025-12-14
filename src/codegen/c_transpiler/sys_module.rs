//! sys Module Support for C Transpilation
//!
//! This module provides implementation of Python's sys module
//! functionality in C, including sys.argv, sys.exit, sys.platform, etc.

/// Generate sys module type definitions and globals
pub fn generate_sys_module_types() -> String {
    r#"// sys module type definitions
typedef struct {
    tauraro_value_t* argv;      // List of command-line arguments
    tauraro_value_t* path;      // List of module search paths
    tauraro_value_t* platform;  // Platform identifier
    tauraro_value_t* version;   // Tauraro version string
    tauraro_value_t* stdin_obj; // stdin file object
    tauraro_value_t* stdout_obj; // stdout file object
    tauraro_value_t* stderr_obj; // stderr file object
    int exit_code;              // Program exit code
} TauraroSysModule;

// Global sys module instance
static TauraroSysModule g_sys_module;
"#.to_string()
}

/// Generate sys module initialization function
pub fn generate_sys_module_init() -> String {
    r#"// Initialize sys module from main()
void tauraro_sys_init(int argc, char* argv[]) {
    // Create sys.argv list
    tauraro_value_t* argv_list = tauraro_value_new();
    argv_list->type = TAURARO_LIST;

    tauraro_list_t* list = malloc(sizeof(tauraro_list_t));
    list->size = 0;
    list->capacity = argc > 0 ? argc : 10;
    list->items = malloc(sizeof(tauraro_value_t*) * list->capacity);

    for (int i = 0; i < argc; i++) {
        tauraro_value_t* arg = tauraro_value_new();
        arg->type = TAURARO_STRING;
        arg->data.str_val = strdup(argv[i]);
        arg->ref_count = 1;
        list->items[list->size++] = arg;
    }

    argv_list->data.list_val = list;
    argv_list->ref_count = 1;
    g_sys_module.argv = argv_list;

    // Set platform
    tauraro_value_t* platform = tauraro_value_new();
    platform->type = TAURARO_STRING;
    #ifdef _WIN32
        platform->data.str_val = strdup("win32");
    #elif __linux__
        platform->data.str_val = strdup("linux");
    #elif __APPLE__
        platform->data.str_val = strdup("darwin");
    #elif __FreeBSD__
        platform->data.str_val = strdup("freebsd");
    #else
        platform->data.str_val = strdup("unknown");
    #endif
    platform->ref_count = 1;
    g_sys_module.platform = platform;

    // Set version
    tauraro_value_t* version = tauraro_value_new();
    version->type = TAURARO_STRING;
    version->data.str_val = strdup("Tauraro 0.1.0");
    version->ref_count = 1;
    g_sys_module.version = version;

    // Initialize path (empty list for now)
    tauraro_value_t* path_list = tauraro_value_new();
    path_list->type = TAURARO_LIST;
    tauraro_list_t* path = malloc(sizeof(tauraro_list_t));
    path->size = 0;
    path->capacity = 10;
    path->items = malloc(sizeof(tauraro_value_t*) * path->capacity);
    path_list->data.list_val = path;
    path_list->ref_count = 1;
    g_sys_module.path = path_list;

    // Initialize exit code
    g_sys_module.exit_code = 0;

    // TODO: Initialize stdin, stdout, stderr file objects
    g_sys_module.stdin_obj = NULL;
    g_sys_module.stdout_obj = NULL;
    g_sys_module.stderr_obj = NULL;
}
"#.to_string()
}

/// Generate sys module accessor functions
pub fn generate_sys_module_accessors() -> String {
    r#"// sys.argv - Access command-line arguments
tauraro_value_t* tauraro_sys_get_argv() {
    return g_sys_module.argv;
}

// sys.platform - Get platform identifier
tauraro_value_t* tauraro_sys_get_platform() {
    return g_sys_module.platform;
}

// sys.version - Get Tauraro version
tauraro_value_t* tauraro_sys_get_version() {
    return g_sys_module.version;
}

// sys.path - Get module search paths
tauraro_value_t* tauraro_sys_get_path() {
    return g_sys_module.path;
}

// sys.exit(code=0) - Exit program with code
void tauraro_sys_exit(int argc, tauraro_value_t** args) {
    int exit_code = 0;

    if (argc > 0 && args[0]->type == TAURARO_INT) {
        exit_code = (int)args[0]->data.int_val;
    }

    g_sys_module.exit_code = exit_code;
    exit(exit_code);
}

// sys.getrefcount(object) - Get reference count
tauraro_value_t* tauraro_sys_getrefcount(int argc, tauraro_value_t** args) {
    if (argc < 1) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = 0;
        return result;
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = args[0]->ref_count;
    return result;
}

// sys.getsizeof(object) - Get size of object
tauraro_value_t* tauraro_sys_getsizeof(int argc, tauraro_value_t** args) {
    if (argc < 1) {
        tauraro_value_t* result = tauraro_value_new();
        result->type = TAURARO_INT;
        result->data.int_val = 0;
        return result;
    }

    tauraro_value_t* obj = args[0];
    size_t size = sizeof(tauraro_value_t);

    switch (obj->type) {
        case TAURARO_STRING:
            size += strlen(obj->data.str_val) + 1;
            break;
        case TAURARO_LIST:
            size += sizeof(tauraro_list_t);
            size += obj->data.list_val->capacity * sizeof(tauraro_value_t*);
            break;
        case TAURARO_DICT:
            size += sizeof(tauraro_dict_t);
            size += obj->data.dict_val->capacity * (sizeof(char*) + sizeof(tauraro_value_t*));
            break;
        case TAURARO_TUPLE:
            size += sizeof(tauraro_tuple_t);
            size += obj->data.tuple_val->capacity * sizeof(tauraro_value_t*);
            break;
        default:
            break;
    }

    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_INT;
    result->data.int_val = size;
    return result;
}

// sys.exc_info() - Get current exception info
tauraro_value_t* tauraro_sys_exc_info() {
    // Returns tuple (type, value, traceback)
    // For now, return None tuple
    tauraro_value_t* result = tauraro_value_new();
    result->type = TAURARO_TUPLE;

    tauraro_tuple_t* tuple = malloc(sizeof(tauraro_tuple_t));
    tuple->size = 3;
    tuple->capacity = 3;
    tuple->items = malloc(sizeof(tauraro_value_t*) * 3);

    // All None for now
    for (int i = 0; i < 3; i++) {
        tauraro_value_t* none = tauraro_value_new();
        none->type = TAURARO_NONE;
        tuple->items[i] = none;
    }

    result->data.tuple_val = tuple;
    return result;
}
"#.to_string()
}

/// Generate sys module forward declarations
pub fn generate_sys_module_declarations() -> String {
    r#"// sys module forward declarations
void tauraro_sys_init(int argc, char* argv[]);
tauraro_value_t* tauraro_sys_get_argv();
tauraro_value_t* tauraro_sys_get_platform();
tauraro_value_t* tauraro_sys_get_version();
tauraro_value_t* tauraro_sys_get_path();
void tauraro_sys_exit(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_sys_getrefcount(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_sys_getsizeof(int argc, tauraro_value_t** args);
tauraro_value_t* tauraro_sys_exc_info();
"#.to_string()
}

/// Generate complete sys module implementation
pub fn generate_sys_module_complete() -> String {
    let mut code = String::new();

    code.push_str(&generate_sys_module_types());
    code.push_str("\n");
    code.push_str(&generate_sys_module_declarations());
    code.push_str("\n");
    code.push_str(&generate_sys_module_init());
    code.push_str("\n");
    code.push_str(&generate_sys_module_accessors());

    code
}
