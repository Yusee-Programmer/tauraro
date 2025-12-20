// ==========================================
// IO MODULE - Pure C Implementation
// ==========================================
// Provides: io.StringIO, io.BytesIO, io.open() (simplified)
// Platform: Cross-platform

#ifndef TAURARO_IO_MODULE_H
#define TAURARO_IO_MODULE_H

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

// StringIO implementation - in-memory string buffer
typedef struct {
    char* buffer;
    size_t size;
    size_t capacity;
    size_t position;
} StringIOObject;

// io.StringIO(initial_value) - Create string buffer
static inline TauValue tauraro_io_StringIO(TauValue initial) {
    StringIOObject* sio = malloc(sizeof(StringIOObject));
    sio->capacity = 256;
    sio->buffer = malloc(sio->capacity);
    sio->position = 0;
    sio->size = 0;

    if (initial.type == 2) {  // String
        size_t len = strlen(initial.value.s);
        if (len > sio->capacity - 1) {
            sio->capacity = len + 256;
            sio->buffer = realloc(sio->buffer, sio->capacity);
        }
        strcpy(sio->buffer, initial.value.s);
        sio->size = len;
    }

    return (TauValue){
        .type = 5,  // Dict type for object representation
        .value.dict = (TauDict*)sio,
        .refcount = 1,
        .next = NULL
    };
}

// io.StringIO.write(value) - Write to string buffer
static inline TauValue tauraro_io_StringIO_write(TauValue self, TauValue value) {
    if (value.type != 2) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    StringIOObject* sio = (StringIOObject*)self.value.dict;
    const char* str = value.value.s;
    size_t len = strlen(str);

    // Resize buffer if needed
    while (sio->position + len >= sio->capacity) {
        sio->capacity *= 2;
        sio->buffer = realloc(sio->buffer, sio->capacity);
    }

    // Write at current position
    strcpy(sio->buffer + sio->position, str);
    sio->position += len;
    if (sio->position > sio->size) {
        sio->size = sio->position;
    }
    sio->buffer[sio->size] = '\0';

    return (TauValue){.type = 0, .value.i = (int64_t)len, .refcount = 1, .next = NULL};
}

// io.StringIO.getvalue() - Get buffer contents
static inline TauValue tauraro_io_StringIO_getvalue(TauValue self) {
    StringIOObject* sio = (StringIOObject*)self.value.dict;
    return tauraro_string(strdup(sio->buffer));
}

// io.StringIO.seek(position) - Set read/write position
static inline TauValue tauraro_io_StringIO_seek(TauValue self, TauValue position) {
    if (position.type != 0) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    StringIOObject* sio = (StringIOObject*)self.value.dict;
    sio->position = (size_t)position.value.i;
    if (sio->position > sio->size) {
        sio->position = sio->size;
    }

    return (TauValue){.type = 0, .value.i = (int64_t)sio->position, .refcount = 1, .next = NULL};
}

// io.StringIO.tell() - Get current position
static inline TauValue tauraro_io_StringIO_tell(TauValue self) {
    StringIOObject* sio = (StringIOObject*)self.value.dict;
    return (TauValue){.type = 0, .value.i = (int64_t)sio->position, .refcount = 1, .next = NULL};
}

// io.StringIO.read(size) - Read from buffer
static inline TauValue tauraro_io_StringIO_read(TauValue self, TauValue size) {
    StringIOObject* sio = (StringIOObject*)self.value.dict;
    size_t read_size = sio->size - sio->position;

    if (size.type == 0 && size.value.i > 0) {
        if ((size_t)size.value.i < read_size) {
            read_size = (size_t)size.value.i;
        }
    }

    char* result = malloc(read_size + 1);
    strncpy(result, sio->buffer + sio->position, read_size);
    result[read_size] = '\0';
    sio->position += read_size;

    TauValue __result = (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL}; return __result;
}

// io.BytesIO(initial_value) - Create bytes buffer
static inline TauValue tauraro_io_BytesIO(TauValue initial) {
    StringIOObject* bio = malloc(sizeof(StringIOObject));
    bio->capacity = 256;
    bio->buffer = malloc(bio->capacity);
    bio->position = 0;
    bio->size = 0;

    if (initial.type == 2) {  // String as bytes
        size_t len = strlen(initial.value.s);
        if (len > bio->capacity - 1) {
            bio->capacity = len + 256;
            bio->buffer = realloc(bio->buffer, bio->capacity);
        }
        strcpy(bio->buffer, initial.value.s);
        bio->size = len;
    }

    return (TauValue){
        .type = 5,
        .value.dict = (TauDict*)bio,
        .refcount = 1,
        .next = NULL
    };
}

// io.BytesIO.write(value) - Write bytes to buffer
static inline TauValue tauraro_io_BytesIO_write(TauValue self, TauValue value) {
    if (value.type != 2) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    StringIOObject* bio = (StringIOObject*)self.value.dict;
    const char* bytes = value.value.s;
    size_t len = strlen(bytes);

    while (bio->position + len >= bio->capacity) {
        bio->capacity *= 2;
        bio->buffer = realloc(bio->buffer, bio->capacity);
    }

    strcpy(bio->buffer + bio->position, bytes);
    bio->position += len;
    if (bio->position > bio->size) {
        bio->size = bio->position;
    }
    bio->buffer[bio->size] = '\0';

    return (TauValue){.type = 0, .value.i = (int64_t)len, .refcount = 1, .next = NULL};
}

// io.BytesIO.getvalue() - Get bytes buffer contents
static inline TauValue tauraro_io_BytesIO_getvalue(TauValue self) {
    StringIOObject* bio = (StringIOObject*)self.value.dict;
    return tauraro_string(strdup(bio->buffer));
}

// io.open(filename, mode) - Open file (simplified)
static inline TauValue tauraro_io_open(TauValue filename, TauValue mode) {
    if (filename.type != 2 || mode.type != 2) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // False
    }

    FILE* file = fopen(filename.value.s, mode.value.s);
    if (!file) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    // Return file handle as integer (cast to int64)
    return (TauValue){.type = 0, .value.i = (int64_t)(uintptr_t)file, .refcount = 1, .next = NULL};
}

// io.close(file_handle) - Close file
static inline TauValue tauraro_io_close(TauValue file_handle) {
    if (file_handle.type != 0) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    FILE* file = (FILE*)(uintptr_t)file_handle.value.i;
    if (file && fclose(file) == 0) {
        return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};  // True
    }
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// io.read_file(filename) - Read entire file
static inline TauValue tauraro_io_read_file(TauValue filename) {
    if (filename.type != 2) {
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

    FILE* file = fopen(filename.value.s, "r");
    if (!file) {
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size <= 0) {
        fclose(file);
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

    char* buffer = malloc(size + 1);
    size_t read = fread(buffer, 1, size, file);
    buffer[read] = '\0';
    fclose(file);

    TauValue __result = (TauValue){.type = 2, .value.s = buffer, .refcount = 1, .next = NULL}; return __result;
}

// io.write_file(filename, content) - Write entire file
static inline TauValue tauraro_io_write_file(TauValue filename, TauValue content) {
    if (filename.type != 2 || content.type != 2) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    FILE* file = fopen(filename.value.s, "w");
    if (!file) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    size_t written = fwrite(content.value.s, 1, strlen(content.value.s), file);
    fclose(file);

    return (TauValue){.type = 3, .value.i = (written > 0) ? 1 : 0, .refcount = 1, .next = NULL};
}


#endif // TAURARO_IO_MODULE_H
