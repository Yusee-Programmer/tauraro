// ==========================================
// LOGGING MODULE - Pure C Implementation
// ==========================================
// Provides: logging.debug(), logging.info(), logging.warning(), logging.error(), logging.critical()
// Platform: Cross-platform

#ifndef TAURARO_LOGGING_MODULE_H
#define TAURARO_LOGGING_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


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

// Logging levels
#define LOG_DEBUG 10
#define LOG_INFO 20
#define LOG_WARNING 30
#define LOG_ERROR 40
#define LOG_CRITICAL 50

// Handler structure for logging handlers
typedef struct {
    int level;
    char format[256];
    FILE* stream;
    char filename[512];
    int is_file;
} LogHandler;

// Formatter structure
typedef struct {
    char format[512];
} LogFormatter;

// Global logging state
static int g_log_level = LOG_WARNING;
static FILE* g_log_file = NULL;
static char g_log_format[256] = "[%(levelname)s] %(message)s";
static int g_use_timestamp = 0;

// logging.set_level(level) - Set minimum logging level
static inline TauValue tauraro_logging_set_level(TauValue level) {
    if (level.type == 0) {
        g_log_level = (int)level.value.i;
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.get_level() - Get current logging level
static inline TauValue tauraro_logging_get_level(void) {
    return (TauValue){.type = 0, .value.i = g_log_level, .refcount = 1, .next = NULL};
}

// Helper: Get level name string
static const char* tauraro_logging_level_name(int level) {
    switch (level) {
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_INFO:
            return "INFO";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERROR:
            return "ERROR";
        case LOG_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

// Helper: Format log message
static char* tauraro_logging_format_message(const char* levelname, const char* message) {
    char buffer[4096];
    char timestamp[32] = "";

    if (g_use_timestamp) {
        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    }

    // Simple format substitution
    const char* fmt = g_log_format;
    char* out = buffer;
    size_t remaining = sizeof(buffer) - 1;

    while (*fmt && remaining > 0) {
        if (fmt[0] == '%' && fmt[1] == '(') {
            // Find closing parenthesis
            const char* start = fmt + 2;
            const char* end = strchr(start, ')');
            if (end) {
                size_t var_len = end - start;
                
                if (strncmp(start, "levelname", var_len) == 0) {
                    size_t len = snprintf(out, remaining, "%s", levelname);
                    out += len;
                    remaining -= len;
                } else if (strncmp(start, "message", var_len) == 0) {
                    size_t len = snprintf(out, remaining, "%s", message);
                    out += len;
                    remaining -= len;
                } else if (strncmp(start, "asctime", var_len) == 0) {
                    size_t len = snprintf(out, remaining, "%s", timestamp);
                    out += len;
                    remaining -= len;
                }
                
                fmt = end + 1;
            } else {
                *out++ = *fmt++;
                remaining--;
            }
        } else if (*fmt == '\\' && fmt[1] == 'n') {
            *out++ = '\n';
            fmt += 2;
            remaining--;
        } else {
            *out++ = *fmt++;
            remaining--;
        }
    }

    *out = '\0';
    return strdup(buffer);
}

// Helper: Log message to appropriate destination
static void tauraro_logging_log_message(int level, const char* message) {
    if (level < g_log_level) {
        return;
    }

    const char* levelname = tauraro_logging_level_name(level);
    char* formatted = tauraro_logging_format_message(levelname, message);

    if (g_log_file) {
        fprintf(g_log_file, "%s", formatted);
        fflush(g_log_file);
    } else {
        // Log to stderr
        fprintf(stderr, "%s", formatted);
        fflush(stderr);
    }

    free(formatted);
}

// logging.debug(message) - Log debug message
static inline TauValue tauraro_logging_debug(TauValue message) {
    if (message.type == 2) {
        tauraro_logging_log_message(LOG_DEBUG, message.value.s);
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.info(message) - Log info message
static inline TauValue tauraro_logging_info(TauValue message) {
    if (message.type == 2) {
        tauraro_logging_log_message(LOG_INFO, message.value.s);
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.warning(message) - Log warning message
static inline TauValue tauraro_logging_warning(TauValue message) {
    if (message.type == 2) {
        tauraro_logging_log_message(LOG_WARNING, message.value.s);
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.error(message) - Log error message
static inline TauValue tauraro_logging_error(TauValue message) {
    if (message.type == 2) {
        tauraro_logging_log_message(LOG_ERROR, message.value.s);
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.critical(message) - Log critical message
static inline TauValue tauraro_logging_critical(TauValue message) {
    if (message.type == 2) {
        tauraro_logging_log_message(LOG_CRITICAL, message.value.s);
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.set_format(format) - Set log message format
static inline TauValue tauraro_logging_set_format(TauValue format) {
    if (format.type == 2) {
        strncpy(g_log_format, format.value.s, sizeof(g_log_format) - 1);
        g_log_format[sizeof(g_log_format) - 1] = '\0';
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.basicConfig(filename, level, format) - Configure logging
static inline TauValue tauraro_logging_basicConfig(TauValue filename, TauValue level, TauValue format) {
    // Set level if provided
    if (level.type == 0) {
        g_log_level = (int)level.value.i;
    }

    // Set format if provided
    if (format.type == 2) {
        strncpy(g_log_format, format.value.s, sizeof(g_log_format) - 1);
        g_log_format[sizeof(g_log_format) - 1] = '\0';
    }

    // Set output file if provided
    if (filename.type == 2) {
        if (g_log_file) {
            fclose(g_log_file);
        }
        g_log_file = fopen(filename.value.s, "a");
    }

    // Enable timestamps if format contains asctime
    g_use_timestamp = (strstr(g_log_format, "%(asctime)s") != NULL);

    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.disable(level) - Disable logging below level
static inline TauValue tauraro_logging_disable(TauValue level) {
    if (level.type == 0) {
        g_log_level = (int)level.value.i + 1;
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.enable() - Re-enable logging
static inline TauValue tauraro_logging_enable(void) {
    g_log_level = LOG_DEBUG;
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.shutdown() - Close log files
static inline TauValue tauraro_logging_shutdown(void) {
    if (g_log_file) {
        fclose(g_log_file);
        g_log_file = NULL;
    }
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.Handler() - Base handler class
static inline TauValue tauraro_logging_Handler(void) {
    LogHandler* handler = (LogHandler*)malloc(sizeof(LogHandler));
    handler->level = LOG_DEBUG;
    strcpy(handler->format, "[%(levelname)s] %(message)s");
    handler->stream = stderr;
    handler->is_file = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)handler, .refcount = 1, .next = NULL};
}

// logging.StreamHandler() - Handler for streams (stdout/stderr)
static inline TauValue tauraro_logging_StreamHandler(void) {
    LogHandler* handler = (LogHandler*)malloc(sizeof(LogHandler));
    handler->level = LOG_DEBUG;
    strcpy(handler->format, "[%(levelname)s] %(message)s");
    handler->stream = stderr;
    handler->is_file = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)handler, .refcount = 1, .next = NULL};
}

// logging.FileHandler(filename) - Handler for files
static inline TauValue tauraro_logging_FileHandler(TauValue filename) {
    LogHandler* handler = (LogHandler*)malloc(sizeof(LogHandler));
    handler->level = LOG_DEBUG;
    strcpy(handler->format, "[%(levelname)s] %(message)s");
    handler->is_file = 1;
    
    if (filename.type == 2) {
        strncpy(handler->filename, filename.value.s, sizeof(handler->filename) - 1);
        handler->filename[sizeof(handler->filename) - 1] = '\0';
        handler->stream = fopen(filename.value.s, "a");
        if (!handler->stream) {
            handler->stream = stderr;
            handler->is_file = 0;
        }
    }
    
    return (TauValue){.type = 6, .value.ptr = (void*)handler, .refcount = 1, .next = NULL};
}

// logging.Formatter(fmt) - Create formatter with format string
static inline TauValue tauraro_logging_Formatter(TauValue fmt) {
    LogFormatter* formatter = (LogFormatter*)malloc(sizeof(LogFormatter));
    
    if (fmt.type == 2) {
        strncpy(formatter->format, fmt.value.s, sizeof(formatter->format) - 1);
    } else {
        strcpy(formatter->format, "[%(levelname)s] %(message)s");
    }
    
    return (TauValue){.type = 6, .value.ptr = (void*)formatter, .refcount = 1, .next = NULL};
}

// logging.Handler.setLevel(level) - Set handler logging level
static inline TauValue tauraro_logging_Handler_setLevel(TauValue handler, TauValue level) {
    if (handler.type != 6) return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    
    LogHandler* h = (LogHandler*)handler.value.ptr;
    if (level.type == 0) {
        h->level = (int)level.value.i;
    }
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.Handler.setFormatter(formatter) - Set handler formatter
static inline TauValue tauraro_logging_Handler_setFormatter(TauValue handler, TauValue formatter) {
    if (handler.type != 6 || formatter.type != 6) 
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    
    LogHandler* h = (LogHandler*)handler.value.ptr;
    LogFormatter* f = (LogFormatter*)formatter.value.ptr;
    
    strncpy(h->format, f->format, sizeof(h->format) - 1);
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// logging.Handler.emit(record) - Emit log record
static inline TauValue tauraro_logging_Handler_emit(TauValue handler, TauValue message) {
    if (handler.type != 6 || message.type != 2) 
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    
    LogHandler* h = (LogHandler*)handler.value.ptr;
    
    if (h->stream) {
        fprintf(h->stream, "%s\n", message.value.s);
        fflush(h->stream);
    }
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// Logging constants
static TauValue tauraro_logging_DEBUG(void) {
    return (TauValue){.type = 0, .value.i = LOG_DEBUG, .refcount = 1, .next = NULL};
}

static TauValue tauraro_logging_INFO(void) {
    return (TauValue){.type = 0, .value.i = LOG_INFO, .refcount = 1, .next = NULL};
}

static TauValue tauraro_logging_WARNING(void) {
    return (TauValue){.type = 0, .value.i = LOG_WARNING, .refcount = 1, .next = NULL};
}

static TauValue tauraro_logging_ERROR(void) {
    return (TauValue){.type = 0, .value.i = LOG_ERROR, .refcount = 1, .next = NULL};
}

static TauValue tauraro_logging_CRITICAL(void) {
    return (TauValue){.type = 0, .value.i = LOG_CRITICAL, .refcount = 1, .next = NULL};
}


#endif // TAURARO_LOGGING_MODULE_H
