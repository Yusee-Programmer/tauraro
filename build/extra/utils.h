#ifndef EXTRA_UTILS_H
#define EXTRA_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// String operations
char* tauraro_str_concat(const char* s1, const char* s2) {
    if (s1 == NULL || s2 == NULL) return NULL;
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* result = (char*)malloc(len1 + len2 + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static inline void* greet(void* name) {
    // Function implementation from IR
    char* tmp_0;
    // Format string: tmp_0
    size_t len = 0;
    len += 7;
    len += 32; // Space for expression
    len += 1;
    tmp_0 = (char*)malloc(len + 1);
    tmp_0[0] = '\0';
    strcat(tmp_0, "Hello, ");
    strcat(tmp_0, (char*)name);
    strcat(tmp_0, "!");
    return tmp_0;
    return tmp_0;
}

static inline int64_t calculate_area(int64_t length, int64_t width) {
    // Function implementation from IR
    int64_t tmp_1;
    tmp_1 = length * width;
    return tmp_1;
}


#endif // EXTRA_UTILS_H
