// ==========================================
// OS MODULE - Pure C Implementation
// ==========================================
// Provides: Complete os module matching Python's os module
// Platform: Cross-platform (Windows/Linux/macOS)

#ifndef TAURARO_OS_MODULE_H
#define TAURARO_OS_MODULE_H

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

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #include <process.h>
    #define getcwd _getcwd
    #define chdir _chdir
    #define PATH_MAX MAX_PATH
    #define getpid _getpid
#else
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <limits.h>
    #include <libgen.h>
#endif

// os.getcwd() - Get current working directory
static inline TauValue tauraro_os_getcwd(void) {
    char* cwd = getcwd(NULL, 0);
    if (!cwd) {
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

    TauValue result = (TauValue){
        .type = 2,              // String type
        .value.s = cwd,
        .refcount = 1,
        .next = NULL
    };
    return result;
}

// os.getenv(key) - Get environment variable
static inline TauValue tauraro_os_getenv(TauValue key) {
    if (key.type != 2) {
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

    const char* value = getenv(key.value.s);
    if (!value) {
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

    return tauraro_string(strdup(value));
}

// os.putenv(key, value) - Set environment variable
static inline TauValue tauraro_os_putenv(TauValue key, TauValue value) {
    if (key.type != 2 || value.type != 2) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        SetEnvironmentVariable(key.value.s, value.value.s);
    #else
        setenv(key.value.s, value.value.s, 1);
    #endif

    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// os.listdir(path) - List directory contents
static inline TauValue tauraro_os_listdir(TauValue path) {
    if (path.type != 2) {
        return tauraro_list_new();
    }

    TauList* list = malloc(sizeof(TauList));
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;

    #ifdef _WIN32
        WIN32_FIND_DATA findData;
        char searchPath[MAX_PATH];
        snprintf(searchPath, MAX_PATH, "%s\\*", path.value.s);

        HANDLE hFind = FindFirstFile(searchPath, &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                    TauValue filename = tauraro_string(strdup(findData.cFileName));
                    tauraro_list_append(list, filename);
                }
            } while (FindNextFile(hFind, &findData));
            FindClose(hFind);
        }
    #else
        DIR* dir = opendir(path.value.s);
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    TauValue filename = tauraro_string(strdup(entry->d_name));
                    tauraro_list_append(list, filename);
                }
            }
            closedir(dir);
        }
    #endif

    return (TauValue){
        .type = 4,              // List type
        .value.list = list,
        .refcount = 1,
        .next = NULL
    };
}

// os.path.exists(path) - Check if path exists
static inline TauValue tauraro_os_path_exists(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // False
    }

    #ifdef _WIN32
        DWORD attrs = GetFileAttributes(path.value.s);
        int exists = (attrs != INVALID_FILE_ATTRIBUTES);
    #else
        struct stat st;
        int exists = (stat(path.value.s, &st) == 0);
    #endif

    return (TauValue){
        .type = 3,              // Bool type
        .value.i = exists ? 1 : 0,
        .refcount = 1,
        .next = NULL
    };
}

// os.path.isfile(path) - Check if path is a file
static inline TauValue tauraro_os_path_isfile(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        DWORD attrs = GetFileAttributes(path.value.s);
        int isfile = (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_DIRECTORY));
    #else
        struct stat st;
        int isfile = (stat(path.value.s, &st) == 0 && S_ISREG(st.st_mode));
    #endif

    return (TauValue){
        .type = 3,
        .value.i = isfile ? 1 : 0,
        .refcount = 1,
        .next = NULL
    };
}

// os.path.isdir(path) - Check if path is a directory
static inline TauValue tauraro_os_path_isdir(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        DWORD attrs = GetFileAttributes(path.value.s);
        int isdir = (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY));
    #else
        struct stat st;
        int isdir = (stat(path.value.s, &st) == 0 && S_ISDIR(st.st_mode));
    #endif

    return (TauValue){
        .type = 3,
        .value.i = isdir ? 1 : 0,
        .refcount = 1,
        .next = NULL
    };
}

// os.path.getsize(path) - Get file size
static inline TauValue tauraro_os_path_getsize(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        WIN32_FILE_ATTRIBUTE_DATA fileInfo;
        if (GetFileAttributesEx(path.value.s, GetFileExInfoStandard, &fileInfo)) {
            LARGE_INTEGER size;
            size.LowPart = fileInfo.nFileSizeLow;
            size.HighPart = fileInfo.nFileSizeHigh;
            return (TauValue){.type = 0, .value.i = size.QuadPart, .refcount = 1, .next = NULL};
        }
    #else
        struct stat st;
        if (stat(path.value.s, &st) == 0) {
            return (TauValue){.type = 0, .value.i = st.st_size, .refcount = 1, .next = NULL};
        }
    #endif

    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// os.remove(path) - Delete a file
static inline TauValue tauraro_os_remove(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    int result = remove(path.value.s);
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// os.remove(path) - Delete a file
static inline TauValue tauraro_os_remove(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    int result = remove(path.value.s);
    return (TauValue){
        .type = 0,
        .value.i = result,
        .refcount = 1,
        .next = NULL
    };
}

// ==========================================
// ADDITIONAL PATH FUNCTIONS
// ==========================================

// os.mkdir(path) - Create a directory
static inline TauValue tauraro_os_mkdir(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        int result = _mkdir(path.value.s);
    #else
        int result = mkdir(path.value.s, 0755);
    #endif

    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// os.makedirs(path) - Create all intermediate directories
static inline TauValue tauraro_os_makedirs(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    char* path_copy = strdup(path.value.s);
    char* sep = PATH_SEPARATOR;
    
    #ifdef _WIN32
        sep = "\\";
    #else
        sep = "/";
    #endif

    for (char* p = path_copy + 1; *p; p++) {
        #ifdef _WIN32
            if (*p == '/' || *p == '\\') {
                *p = '\\';
        #else
            if (*p == '/') {
        #endif
                *p = '\0';
                #ifdef _WIN32
                    _mkdir(path_copy);
                #else
                    mkdir(path_copy, 0755);
                #endif
                *p = sep[0];
            }
        }
    }

    #ifdef _WIN32
        int result = _mkdir(path_copy);
    #else
        int result = mkdir(path_copy, 0755);
    #endif

    free(path_copy);
    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// os.rmdir(path) - Remove a directory
static inline TauValue tauraro_os_rmdir(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        int result = _rmdir(path.value.s);
    #else
        int result = rmdir(path.value.s);
    #endif

    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// os.rename(src, dst) - Rename file or directory
static inline TauValue tauraro_os_rename(TauValue src, TauValue dst) {
    if (src.type != 2 || dst.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    int result = rename(src.value.s, dst.value.s);
    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// os.chdir(path) - Change current directory
static inline TauValue tauraro_os_chdir(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    int result = chdir(path.value.s);
    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// ==========================================
// PROCESS FUNCTIONS
// ==========================================

// os.getpid() - Get process ID
static inline TauValue tauraro_os_getpid(void) {
    #ifdef _WIN32
        int pid = _getpid();
    #else
        pid_t pid = getpid();
    #endif

    return (TauValue){.type = 0, .value.i = pid, .refcount = 1, .next = NULL};
}

// os.getppid() - Get parent process ID
static inline TauValue tauraro_os_getppid(void) {
    #ifdef _WIN32
        int ppid = 0;  // Windows doesn't have direct getppid()
    #else
        pid_t ppid = getppid();
    #endif

    return (TauValue){.type = 0, .value.i = ppid, .refcount = 1, .next = NULL};
}

// os.system(command) - Execute system command
static inline TauValue tauraro_os_system(TauValue cmd) {
    if (cmd.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    int result = system(cmd.value.s);
    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// ==========================================
// FILE ATTRIBUTE FUNCTIONS
// ==========================================

// os.chmod(path, mode) - Change file permissions
static inline TauValue tauraro_os_chmod(TauValue path, TauValue mode) {
    if (path.type != 2 || mode.type != 0) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        int m = (int)mode.value.i;
        int result = _chmod(path.value.s, m);
    #else
        mode_t m = (mode_t)mode.value.i;
        int result = chmod(path.value.s, m);
    #endif

    return (TauValue){.type = 0, .value.i = result, .refcount = 1, .next = NULL};
}

// os.stat(path) - Get file stats (returns dict with size, mode, etc.)
static inline TauValue tauraro_os_stat(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 5, .value.dict = NULL, .refcount = 1, .next = NULL};
    }

    struct stat st;
    if (stat(path.value.s, &st) != 0) {
        return (TauValue){.type = 5, .value.dict = NULL, .refcount = 1, .next = NULL};
    }

    // Return as dict using proper dict functions
    TauDict* stat_dict = tauraro_create_dict();
    
    // Store file stats
    TauValue size_val = {.type = 0, .value.i = st.st_size, .refcount = 1, .next = NULL};
    tauraro_dict_set(stat_dict, "st_size", size_val);
    
    TauValue mode_val = {.type = 0, .value.i = st.st_mode, .refcount = 1, .next = NULL};
    tauraro_dict_set(stat_dict, "st_mode", mode_val);
    
    TauValue mtime_val = {.type = 0, .value.i = st.st_mtime, .refcount = 1, .next = NULL};
    tauraro_dict_set(stat_dict, "st_mtime", mtime_val);

    return (TauValue){
        .type = 5,
        .value.dict = stat_dict,
        .refcount = 1,
        .next = NULL
    };
}

// os.access(path, mode) - Check access permissions
static inline TauValue tauraro_os_access(TauValue path, TauValue mode) {
    if (path.type != 2 || mode.type != 0) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // False
    }

    #ifdef _WIN32
        int m = (int)mode.value.i;
        int result = _access(path.value.s, m);
    #else
        int m = (int)mode.value.i;
        int result = access(path.value.s, m);
    #endif

    return (TauValue){
        .type = 3,
        .value.i = (result == 0) ? 1 : 0,
        .refcount = 1,
        .next = NULL
    };
}

// ==========================================
// OS PATH SUBMODULE FUNCTIONS
// ==========================================

// os.path.join(parts...) - Join path components
static inline TauValue tauraro_os_path_join(TauValue arg1, TauValue arg2) {
    if (arg1.type != 2 || arg2.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }

    char* sep = "/";
    #ifdef _WIN32
        sep = "\\";
    #endif

    size_t len = strlen(arg1.value.s) + strlen(arg2.value.s) + 2;
    char* result = malloc(len);
    snprintf(result, len, "%s%s%s", arg1.value.s, sep, arg2.value.s);

    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

// os.path.split(path) - Split path into directory and filename (returns list)
static inline TauValue tauraro_os_path_split(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 4, .value.list = NULL, .refcount = 1, .next = NULL};
    }

    char* path_copy = strdup(path.value.s);
    char* last_sep = strrchr(path_copy, '/');
    #ifdef _WIN32
        char* last_sep_win = strrchr(path_copy, '\\');
        if (last_sep_win > last_sep) last_sep = last_sep_win;
    #endif

    TauList* result = malloc(sizeof(TauList));
    result->size = 2;
    result->capacity = 2;
    result->items = malloc(sizeof(TauValue) * 2);

    if (last_sep) {
        *last_sep = '\0';
        result->items[0] = (TauValue){.type = 2, .value.s = strdup(path_copy), .refcount = 1, .next = NULL};
        result->items[1] = (TauValue){.type = 2, .value.s = strdup(last_sep + 1), .refcount = 1, .next = NULL};
    } else {
        result->items[0] = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
        result->items[1] = (TauValue){.type = 2, .value.s = strdup(path.value.s), .refcount = 1, .next = NULL};
    }

    free(path_copy);
    return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
}

// os.path.dirname(path) - Get directory name
static inline TauValue tauraro_os_path_dirname(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }

    char* path_copy = strdup(path.value.s);
    char* last_sep = strrchr(path_copy, '/');
    #ifdef _WIN32
        char* last_sep_win = strrchr(path_copy, '\\');
        if (last_sep_win > last_sep) last_sep = last_sep_win;
    #endif

    char* result;
    if (last_sep) {
        *last_sep = '\0';
        result = strdup(path_copy);
    } else {
        result = strdup("");
    }

    free(path_copy);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

// os.path.basename(path) - Get file name
static inline TauValue tauraro_os_path_basename(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }

    char* path_copy = strdup(path.value.s);
    char* last_sep = strrchr(path_copy, '/');
    #ifdef _WIN32
        char* last_sep_win = strrchr(path_copy, '\\');
        if (last_sep_win > last_sep) last_sep = last_sep_win;
    #endif

    char* result;
    if (last_sep) {
        result = strdup(last_sep + 1);
    } else {
        result = strdup(path.value.s);
    }

    free(path_copy);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

// os.path.abspath(path) - Get absolute path
static inline TauValue tauraro_os_path_abspath(TauValue path) {
    if (path.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }

    #ifdef _WIN32
        char abs_path[MAX_PATH];
        if (!_fullpath(abs_path, path.value.s, MAX_PATH)) {
            return (TauValue){.type = 2, .value.s = strdup(path.value.s), .refcount = 1, .next = NULL};
        }
    #else
        char abs_path[PATH_MAX];
        if (!realpath(path.value.s, abs_path)) {
            return (TauValue){.type = 2, .value.s = strdup(path.value.s), .refcount = 1, .next = NULL};
        }
    #endif

    return (TauValue){.type = 2, .value.s = strdup(abs_path), .refcount = 1, .next = NULL};
}

// os.path.realpath(path) - Get real path (resolving symlinks)
static inline TauValue tauraro_os_path_realpath(TauValue path) {
    // Same as abspath for now (no symlink support in simple C version)
    return tauraro_os_path_abspath(path);
}

// ==========================================
// CONSTANTS
// ==========================================

#define OS_F_OK 0   // File exists
#define OS_R_OK 4   // Readable
#define OS_W_OK 2   // Writable
#define OS_X_OK 1   // Executable

#define PATH_SEPARATOR "/"
#ifdef _WIN32
    #undef PATH_SEPARATOR
    #define PATH_SEPARATOR "\\"
#endif


#endif // TAURARO_OS_MODULE_H
