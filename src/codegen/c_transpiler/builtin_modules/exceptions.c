// ==========================================
// EXCEPTIONS MODULE - Pure C Implementation
// ==========================================
// Provides: All Python exception types and hierarchy
// Platform: Cross-platform

#ifndef TAURARO_EXCEPTIONS_MODULE_H
#define TAURARO_EXCEPTIONS_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Base exception structure
typedef struct {
    char* message;
    char* exception_type;
    int lineno;
    char* filename;
    char* traceback;
} TauException;

// Exception creation helper
static inline TauValue tau_exception_create(const char* type, const char* message) {
    TauException* exc = (TauException*)malloc(sizeof(TauException));
    exc->message = (char*)malloc(strlen(message) + 1);
    strcpy(exc->message, message);
    exc->exception_type = (char*)malloc(strlen(type) + 1);
    strcpy(exc->exception_type, type);
    exc->lineno = 0;
    exc->filename = NULL;
    exc->traceback = NULL;
    
    return (TauValue){.type = 6, .value.p = (void*)exc, .refcount = 1, .next = NULL};
}

// Base exceptions
static inline TauValue tauraro_exceptions_Exception(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("Exception", message);
}

static inline TauValue tauraro_exceptions_BaseException(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("BaseException", message);
}

// Standard exceptions
static inline TauValue tauraro_exceptions_ValueError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("ValueError", message);
}

static inline TauValue tauraro_exceptions_TypeError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("TypeError", message);
}

static inline TauValue tauraro_exceptions_KeyError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("KeyError", message);
}

static inline TauValue tauraro_exceptions_IndexError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("IndexError", message);
}

static inline TauValue tauraro_exceptions_AttributeError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("AttributeError", message);
}

static inline TauValue tauraro_exceptions_NameError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("NameError", message);
}

static inline TauValue tauraro_exceptions_RuntimeError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("RuntimeError", message);
}

static inline TauValue tauraro_exceptions_NotImplementedError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("NotImplementedError", message);
}

static inline TauValue tauraro_exceptions_ImportError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("ImportError", message);
}

static inline TauValue tauraro_exceptions_ModuleNotFoundError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("ModuleNotFoundError", message);
}

static inline TauValue tauraro_exceptions_SyntaxError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("SyntaxError", message);
}

static inline TauValue tauraro_exceptions_IndentationError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("IndentationError", message);
}

static inline TauValue tauraro_exceptions_TabError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("TabError", message);
}

static inline TauValue tauraro_exceptions_ZeroDivisionError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "division by zero";
    return tau_exception_create("ZeroDivisionError", message);
}

static inline TauValue tauraro_exceptions_FloatingPointError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("FloatingPointError", message);
}

static inline TauValue tauraro_exceptions_OverflowError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("OverflowError", message);
}

// File/IO exceptions
static inline TauValue tauraro_exceptions_IOError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("IOError", message);
}

static inline TauValue tauraro_exceptions_FileNotFoundError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("FileNotFoundError", message);
}

static inline TauValue tauraro_exceptions_FileExistsError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("FileExistsError", message);
}

static inline TauValue tauraro_exceptions_IsADirectoryError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("IsADirectoryError", message);
}

static inline TauValue tauraro_exceptions_NotADirectoryError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("NotADirectoryError", message);
}

static inline TauValue tauraro_exceptions_PermissionError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("PermissionError", message);
}

static inline TauValue tauraro_exceptions_OSError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("OSError", message);
}

// Encoding exceptions
static inline TauValue tauraro_exceptions_UnicodeError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("UnicodeError", message);
}

static inline TauValue tauraro_exceptions_UnicodeDecodeError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("UnicodeDecodeError", message);
}

static inline TauValue tauraro_exceptions_UnicodeEncodeError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("UnicodeEncodeError", message);
}

static inline TauValue tauraro_exceptions_UnicodeTranslateError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("UnicodeTranslateError", message);
}

// Assertion and system exceptions
static inline TauValue tauraro_exceptions_AssertionError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("AssertionError", message);
}

static inline TauValue tauraro_exceptions_SystemError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("SystemError", message);
}

static inline TauValue tauraro_exceptions_SystemExit(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("SystemExit", message);
}

static inline TauValue tauraro_exceptions_KeyboardInterrupt(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("KeyboardInterrupt", message);
}

// Memory and resource exceptions
static inline TauValue tauraro_exceptions_MemoryError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("MemoryError", message);
}

static inline TauValue tauraro_exceptions_RecursionError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("RecursionError", message);
}

// Reference and value exceptions
static inline TauValue tauraro_exceptions_ReferenceError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("ReferenceError", message);
}

static inline TauValue tauraro_exceptions_StopIteration(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("StopIteration", message);
}

static inline TauValue tauraro_exceptions_GeneratorExit(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("GeneratorExit", message);
}

// Warning exceptions
static inline TauValue tauraro_exceptions_Warning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("Warning", message);
}

static inline TauValue tauraro_exceptions_DeprecationWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("DeprecationWarning", message);
}

static inline TauValue tauraro_exceptions_PendingDeprecationWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("PendingDeprecationWarning", message);
}

static inline TauValue tauraro_exceptions_RuntimeWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("RuntimeWarning", message);
}

static inline TauValue tauraro_exceptions_SyntaxWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("SyntaxWarning", message);
}

static inline TauValue tauraro_exceptions_UserWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("UserWarning", message);
}

static inline TauValue tauraro_exceptions_FutureWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("FutureWarning", message);
}

static inline TauValue tauraro_exceptions_ImportWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("ImportWarning", message);
}

static inline TauValue tauraro_exceptions_UnicodeWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("UnicodeWarning", message);
}

static inline TauValue tauraro_exceptions_BytesWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("BytesWarning", message);
}

static inline TauValue tauraro_exceptions_ResourceWarning(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("ResourceWarning", message);
}

// Arithmetic exceptions
static inline TauValue tauraro_exceptions_ArithmeticError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("ArithmeticError", message);
}

// Lookup exceptions
static inline TauValue tauraro_exceptions_LookupError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("LookupError", message);
}

// Environment exceptions  
static inline TauValue tauraro_exceptions_EnvironmentError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("EnvironmentError", message);
}

// Standard iterator exception
static inline TauValue tauraro_exceptions_EOFError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("EOFError", message);
}

// Custom runtime exceptions
static inline TauValue tauraro_exceptions_UnboundLocalError(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("UnboundLocalError", message);
}

static inline TauValue tauraro_exceptions_LookupError_Custom(TauValue msg) {
    const char* message = msg.type == 2 ? msg.value.s : "";
    return tau_exception_create("LookupError", message);
}

// Helper functions
static inline TauValue tauraro_exceptions_get_message(TauValue exc) {
    if (exc.type != 6) return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    TauException* e = (TauException*)exc.value.p;
    return (TauValue){.type = 2, .value.s = e->message, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_exceptions_get_type(TauValue exc) {
    if (exc.type != 6) return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    TauException* e = (TauException*)exc.value.p;
    return (TauValue){.type = 2, .value.s = e->exception_type, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_exceptions_set_lineno(TauValue exc, TauValue lineno) {
    if (exc.type != 6 || lineno.type != 0) return exc;
    TauException* e = (TauException*)exc.value.p;
    e->lineno = lineno.value.i;
    return exc;
}

static inline TauValue tauraro_exceptions_set_filename(TauValue exc, TauValue filename) {
    if (exc.type != 6 || filename.type != 2) return exc;
    TauException* e = (TauException*)exc.value.p;
    if (e->filename) free(e->filename);
    e->filename = (char*)malloc(strlen(filename.value.s) + 1);
    strcpy(e->filename, filename.value.s);
    return exc;
}

#endif // TAURARO_EXCEPTIONS_MODULE_H
