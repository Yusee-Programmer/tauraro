// Tauraro OS Module Implementation
// Auto-generated C implementation for the os built-in module

#include "os_module.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#define mkdir(path, mode) _mkdir(path)
#define rmdir _rmdir
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

static int os_initialized = 0;

void tauraro_os_module_init(void) {
    if (os_initialized) return;
    os_initialized = 1;
}

tauraro_value_t* tauraro_os_getcwd(int argc, tauraro_value_t** argv) {
    (void)argc; (void)argv;
    char buffer[4096];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return tauraro_string(buffer);
    }
    return tauraro_none();
}

tauraro_value_t* tauraro_os_chdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);  // TAURARO_STRING
    int result = chdir(argv[0]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_mkdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
    int result = mkdir(argv[0]->data.str_val, 0755);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_rmdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
    int result = rmdir(argv[0]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_remove(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
    int result = remove(argv[0]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_rename(int argc, tauraro_value_t** argv) {
    if (argc < 2 || argv == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3 || argv[1]->type != 3) return tauraro_bool(0);
    int result = rename(argv[0]->data.str_val, argv[1]->data.str_val);
    return tauraro_bool(result == 0);
}

tauraro_value_t* tauraro_os_getenv(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_none();
    if (argv[0]->type != 3) return tauraro_none();
    char* value = getenv(argv[0]->data.str_val);
    if (value != NULL) {
        return tauraro_string(value);
    }
    return tauraro_none();
}

tauraro_value_t* tauraro_os_system(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_int(-1);
    if (argv[0]->type != 3) return tauraro_int(-1);
    int result = system(argv[0]->data.str_val);
    return tauraro_int(result);
}

tauraro_value_t* tauraro_os_path_exists(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(argv[0]->data.str_val);
    return tauraro_bool(attrs != INVALID_FILE_ATTRIBUTES);
#else
    struct stat st;
    return tauraro_bool(stat(argv[0]->data.str_val, &st) == 0);
#endif
}

tauraro_value_t* tauraro_os_path_isfile(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(argv[0]->data.str_val);
    return tauraro_bool(attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    if (stat(argv[0]->data.str_val, &st) != 0) return tauraro_bool(0);
    return tauraro_bool(S_ISREG(st.st_mode));
#endif
}

tauraro_value_t* tauraro_os_path_isdir(int argc, tauraro_value_t** argv) {
    if (argc < 1 || argv == NULL || argv[0] == NULL) return tauraro_bool(0);
    if (argv[0]->type != 3) return tauraro_bool(0);
#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(argv[0]->data.str_val);
    return tauraro_bool(attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat st;
    if (stat(argv[0]->data.str_val, &st) != 0) return tauraro_bool(0);
    return tauraro_bool(S_ISDIR(st.st_mode));
#endif
}

tauraro_value_t* tauraro_os_get_attr(const char* name) {
    if (strcmp(name, "name") == 0) {
#ifdef _WIN32
        return tauraro_string("nt");
#else
        return tauraro_string("posix");
#endif
    }
    if (strcmp(name, "sep") == 0) {
#ifdef _WIN32
        return tauraro_string("\\");
#else
        return tauraro_string("/");
#endif
    }
    return tauraro_none();
}
