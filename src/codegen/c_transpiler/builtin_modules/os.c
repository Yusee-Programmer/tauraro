// ==========================================
// OS MODULE - Pure C Implementation
// ==========================================
// Provides: os.getcwd(), os.getenv(), os.listdir(), os.path operations
// Platform: Cross-platform (Windows/Linux/macOS)

#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #define getcwd _getcwd
    #define PATH_MAX MAX_PATH
#else
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <limits.h>
#endif

// os.getcwd() - Get current working directory
static inline TauValue tauraro_os_getcwd(void) {
    char* cwd = getcwd(NULL, 0);
    if (!cwd) {
        return tauraro_string("");
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
        return tauraro_string("");
    }

    const char* value = getenv(key.value.s);
    if (!value) {
        return tauraro_string("");
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
    return (TauValue){
        .type = 0,
        .value.i = result,
        .refcount = 1,
        .next = NULL
    };
}
