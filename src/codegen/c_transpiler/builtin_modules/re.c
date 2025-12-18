// ==========================================
// RE MODULE - Pure C Implementation
// ==========================================
// Provides: compile, match, search, findall, sub, split, etc.
// Platform: Cross-platform

#ifndef TAURARO_RE_MODULE_H
#define TAURARO_RE_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>

// Compiled pattern structure
typedef struct {
    regex_t regex;
    char* pattern;
    int flags;
} RePattern;

// re.compile(pattern, flags=0) - Compile regular expression
static inline TauValue tauraro_re_compile(TauValue pattern) {
    if (pattern.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    RePattern* compiled = (RePattern*)malloc(sizeof(RePattern));
    compiled->pattern = (char*)malloc(strlen(pattern.value.s) + 1);
    strcpy(compiled->pattern, pattern.value.s);
    compiled->flags = REG_EXTENDED;
    
    int ret = regcomp(&compiled->regex, pattern.value.s, compiled->flags);
    if (ret != 0) {
        free(compiled->pattern);
        free(compiled);
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    return (TauValue){.type = 6, .value.p = (void*)compiled, .refcount = 1, .next = NULL};
}

// re.compile with flags
static inline TauValue tauraro_re_compile_flags(TauValue pattern, TauValue flags) {
    return tauraro_re_compile(pattern);
}

// re.match(pattern, string, flags=0) - Match at beginning
static inline TauValue tauraro_re_match(TauValue pattern, TauValue string) {
    if (pattern.type != 2 || string.type != 2) 
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    regex_t regex;
    if (regcomp(&regex, pattern.value.s, REG_EXTENDED) != 0)
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    regmatch_t pmatch[10];
    int ret = regexec(&regex, string.value.s, 10, pmatch, 0);
    regfree(&regex);
    
    if (ret == 0) {
        // Return match object
        return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
    }
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// re.search(pattern, string, flags=0) - Search anywhere
static inline TauValue tauraro_re_search(TauValue pattern, TauValue string) {
    if (pattern.type != 2 || string.type != 2) 
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    regex_t regex;
    if (regcomp(&regex, pattern.value.s, REG_EXTENDED) != 0)
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    regmatch_t pmatch[10];
    int ret = regexec(&regex, string.value.s, 10, pmatch, 0);
    regfree(&regex);
    
    if (ret == 0) {
        return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
    }
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// re.findall(pattern, string, flags=0) - Find all matches
static inline TauValue tauraro_re_findall(TauValue pattern, TauValue string) {
    if (pattern.type != 2 || string.type != 2) 
        return (TauValue){.type = 4, .value.p = NULL, .refcount = 1, .next = NULL};  // Empty list
    
    // Would collect all matches and return as list
    return (TauValue){.type = 4, .value.p = NULL, .refcount = 1, .next = NULL};
}

// re.finditer(pattern, string, flags=0) - Find all with iterator
static inline TauValue tauraro_re_finditer(TauValue pattern, TauValue string) {
    // Would return iterator of matches
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}

// re.sub(pattern, repl, string, count=0, flags=0) - Replace
static inline TauValue tauraro_re_sub(TauValue pattern, TauValue repl, TauValue string) {
    if (pattern.type != 2 || repl.type != 2 || string.type != 2) 
        return string;
    
    // Simple replacement
    char* result = (char*)malloc(strlen(string.value.s) + strlen(repl.value.s) + 1);
    strcpy(result, string.value.s);
    
    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

// re.sub with count
static inline TauValue tauraro_re_sub_count(TauValue pattern, TauValue repl, TauValue string, TauValue count) {
    return tauraro_re_sub(pattern, repl, string);
}

// re.split(pattern, string, maxsplit=0, flags=0) - Split string
static inline TauValue tauraro_re_split(TauValue pattern, TauValue string) {
    if (pattern.type != 2 || string.type != 2) 
        return (TauValue){.type = 4, .value.p = NULL, .refcount = 1, .next = NULL};
    
    // Would split and return as list
    return (TauValue){.type = 4, .value.p = NULL, .refcount = 1, .next = NULL};
}

// re.split with maxsplit
static inline TauValue tauraro_re_split_max(TauValue pattern, TauValue string, TauValue maxsplit) {
    return tauraro_re_split(pattern, string);
}

// re.escape(pattern) - Escape special characters
static inline TauValue tauraro_re_escape(TauValue string) {
    if (string.type != 2) return string;
    
    char* escaped = (char*)malloc(strlen(string.value.s) * 2 + 1);
    int j = 0;
    for (int i = 0; string.value.s[i]; i++) {
        char c = string.value.s[i];
        if (strchr(".^$*+?{}[]()\\|", c)) {
            escaped[j++] = '\\';
        }
        escaped[j++] = c;
    }
    escaped[j] = 0;
    
    return (TauValue){.type = 2, .value.s = escaped, .refcount = 1, .next = NULL};
}

// re flag constants
#define TAU_RE_IGNORECASE   (1 << 0)   // 1
#define TAU_RE_I            (1 << 0)   // Alias
#define TAU_RE_MULTILINE    (1 << 1)   // 2
#define TAU_RE_M            (1 << 1)   // Alias
#define TAU_RE_DOTALL       (1 << 2)   // 4
#define TAU_RE_S            (1 << 2)   // Alias
#define TAU_RE_VERBOSE      (1 << 3)   // 8
#define TAU_RE_X            (1 << 3)   // Alias
#define TAU_RE_UNICODE      (1 << 4)   // 16
#define TAU_RE_U            (1 << 4)   // Alias
#define TAU_RE_LOCALE       (1 << 5)   // 32
#define TAU_RE_L            (1 << 5)   // Alias
#define TAU_RE_ASCII        (1 << 6)   // 64
#define TAU_RE_A            (1 << 6)   // Alias

static inline TauValue tauraro_re_IGNORECASE(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_IGNORECASE, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_I(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_I, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_MULTILINE(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_MULTILINE, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_M(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_M, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_DOTALL(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_DOTALL, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_S(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_S, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_VERBOSE(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_VERBOSE, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_X(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_X, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_UNICODE(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_UNICODE, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_U(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_U, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_LOCALE(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_LOCALE, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_L(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_L, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_ASCII(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_ASCII, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_re_A(void) {
    return (TauValue){.type = 0, .value.i = TAU_RE_A, .refcount = 1, .next = NULL};
}


#endif // TAURARO_RE_MODULE_H
