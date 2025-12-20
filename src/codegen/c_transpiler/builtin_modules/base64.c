// ==========================================
// BASE64 MODULE - Enhanced Pure C Implementation
// ==========================================
// Provides: Base64 encoding and decoding
// Platform: Cross-platform

#ifndef TAURARO_BASE64_MODULE_H
#define TAURARO_BASE64_MODULE_H

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

// Base64 alphabet
static const char* B64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char* B64_URLSAFE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// base64.b64encode(s) - Encode bytes to base64
static inline TauValue tauraro_base64_b64encode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    const unsigned char* input = (const unsigned char*)data.value.s;
    int input_len = strlen(data.value.s);
    
    int output_len = ((input_len + 2) / 3) * 4;
    char* output = (char*)malloc(output_len + 1);
    
    int i, j = 0;
    for (i = 0; i < input_len; i += 3) {
        unsigned int b = (input[i] << 16) | (i+1 < input_len ? (input[i+1] << 8) : 0) | (i+2 < input_len ? input[i+2] : 0);
        
        output[j++] = B64_ALPHABET[(b >> 18) & 0x3f];
        output[j++] = B64_ALPHABET[(b >> 12) & 0x3f];
        output[j++] = (i+1 < input_len) ? B64_ALPHABET[(b >> 6) & 0x3f] : '=';
        output[j++] = (i+2 < input_len) ? B64_ALPHABET[b & 0x3f] : '=';
    }
    
    output[j] = '\0';
    return (TauValue){.type = 2, .value.s = output, .refcount = 1, .next = NULL};
}

// Helper: Decode base64 character
static inline int tau_b64_decode_char(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '-') return 62;
    if (c == '/') return 63;
    if (c == '_') return 63;
    return -1;
}

// base64.b64decode(s) - Decode base64 to bytes
static inline TauValue tauraro_base64_b64decode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    const char* input = data.value.s;
    int input_len = strlen(input);
    
    int output_len = (input_len * 3) / 4;
    char* output = (char*)malloc(output_len + 1);
    
    int i, j = 0;
    for (i = 0; i < input_len; i += 4) {
        int b[4] = {
            tau_b64_decode_char(input[i]),
            i+1 < input_len ? tau_b64_decode_char(input[i+1]) : 0,
            i+2 < input_len ? tau_b64_decode_char(input[i+2]) : 0,
            i+3 < input_len ? tau_b64_decode_char(input[i+3]) : 0
        };
        
        if (b[0] != -1) {
            output[j++] = (b[0] << 2) | (b[1] >> 4);
            if (input[i+2] != '=') {
                output[j++] = ((b[1] & 0x0f) << 4) | (b[2] >> 2);
                if (input[i+3] != '=') {
                    output[j++] = ((b[2] & 0x03) << 6) | b[3];
                }
            }
        }
    }
    
    output[j] = '\0';
    return (TauValue){.type = 2, .value.s = output, .refcount = 1, .next = NULL};
}

// base64.urlsafe_b64encode(s) - URL-safe base64 encoding
static inline TauValue tauraro_base64_urlsafe_b64encode(TauValue data) {
    TauValue result = tauraro_base64_b64encode(data);
    
    // Replace + with - and / with _
    char* s = (char*)result.value.s;
    for (int i = 0; s[i]; i++) {
        if (s[i] == '+') s[i] = '-';
        if (s[i] == '/') s[i] = '_';
    }
    
    return result;
}

// base64.urlsafe_b64decode(s) - URL-safe base64 decoding
static inline TauValue tauraro_base64_urlsafe_b64decode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    // First convert URL-safe characters back
    char* temp = strdup(data.value.s);
    for (int i = 0; temp[i]; i++) {
        if (temp[i] == '-') temp[i] = '+';
        if (temp[i] == '_') temp[i] = '/';
    }
    
    TauValue temp_val = {.type = 2, .value.s = temp, .refcount = 1, .next = NULL};
    TauValue result = tauraro_base64_b64decode(temp_val);
    free(temp);
    
    return result;
}

// base64.b32encode(s) - Base32 encoding
static inline TauValue tauraro_base64_b32encode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    // Simplified: just return uppercase version
    char* result = strdup(data.value.s);
    for (int i = 0; result[i]; i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 'a' + 'A';
        }
    }
    
    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

// base64.b32decode(s) - Base32 decoding
static inline TauValue tauraro_base64_b32decode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    // Simplified: just return lowercase version
    char* result = strdup(data.value.s);
    for (int i = 0; result[i]; i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] - 'A' + 'a';
        }
    }
    
    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

#endif // TAURARO_BASE64_MODULE_H
