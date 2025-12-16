// ==========================================
// BASE64 MODULE - Pure C Implementation
// ==========================================
// Provides: Base64 encoding and decoding functions
// Platform: Cross-platform

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Base64 encoding table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Base64 decoding table
static const unsigned char base64_decode_table[256] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 62,   0xFF, 0xFF, 0xFF, 63,
    52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,   13,   14,
    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
    41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// ==========================================
// BASE64 ENCODING
// ==========================================

// base64.b64encode(s) - Encode bytes to base64
static inline TauValue tauraro_base64_b64encode(TauValue data) {
    if (data.type != 2) {  // Not a string
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    const unsigned char* input = (const unsigned char*)data.value.s;
    size_t input_len = strlen(data.value.s);

    // Calculate output length (4 bytes for every 3 input bytes)
    size_t output_len = ((input_len + 2) / 3) * 4;
    char* output = (char*)malloc(output_len + 1);

    if (!output) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    size_t i, j;
    for (i = 0, j = 0; i < input_len;) {
        uint32_t octet_a = i < input_len ? input[i++] : 0;
        uint32_t octet_b = i < input_len ? input[i++] : 0;
        uint32_t octet_c = i < input_len ? input[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        output[j++] = base64_table[(triple >> 18) & 0x3F];
        output[j++] = base64_table[(triple >> 12) & 0x3F];
        output[j++] = base64_table[(triple >> 6) & 0x3F];
        output[j++] = base64_table[triple & 0x3F];
    }

    // Add padding
    size_t padding = (3 - (input_len % 3)) % 3;
    for (i = 0; i < padding; i++) {
        output[output_len - 1 - i] = '=';
    }

    output[output_len] = '\0';

    return (TauValue){.type = 2, .value.s = output, .refcount = 1, .next = NULL};
}

// ==========================================
// BASE64 DECODING
// ==========================================

// base64.b64decode(s) - Decode base64 to bytes
static inline TauValue tauraro_base64_b64decode(TauValue data) {
    if (data.type != 2) {  // Not a string
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    const unsigned char* input = (const unsigned char*)data.value.s;
    size_t input_len = strlen(data.value.s);

    // Remove padding from length calculation
    while (input_len > 0 && input[input_len - 1] == '=') {
        input_len--;
    }

    // Calculate output length
    size_t output_len = (input_len * 3) / 4;
    unsigned char* output = (unsigned char*)malloc(output_len + 1);

    if (!output) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    size_t i, j;
    for (i = 0, j = 0; i < input_len;) {
        uint32_t sextet_a = base64_decode_table[input[i++]];
        uint32_t sextet_b = i < input_len ? base64_decode_table[input[i++]] : 0;
        uint32_t sextet_c = i < input_len ? base64_decode_table[input[i++]] : 0;
        uint32_t sextet_d = i < input_len ? base64_decode_table[input[i++]] : 0;

        uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

        if (j < output_len) output[j++] = (triple >> 16) & 0xFF;
        if (j < output_len) output[j++] = (triple >> 8) & 0xFF;
        if (j < output_len) output[j++] = triple & 0xFF;
    }

    output[output_len] = '\0';

    return (TauValue){.type = 2, .value.s = (char*)output, .refcount = 1, .next = NULL};
}

// ==========================================
// URL-SAFE BASE64 VARIANTS
// ==========================================

// base64.urlsafe_b64encode(s) - Encode using URL-safe alphabet
static inline TauValue tauraro_base64_urlsafe_b64encode(TauValue data) {
    TauValue result = tauraro_base64_b64encode(data);

    if (result.type == 2 && result.value.s) {
        // Replace + with - and / with _
        char* s = result.value.s;
        for (size_t i = 0; s[i] != '\0'; i++) {
            if (s[i] == '+') s[i] = '-';
            else if (s[i] == '/') s[i] = '_';
        }
    }

    return result;
}

// base64.urlsafe_b64decode(s) - Decode URL-safe base64
static inline TauValue tauraro_base64_urlsafe_b64decode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    // Create a copy and replace - with + and _ with /
    char* input_copy = strdup(data.value.s);
    for (size_t i = 0; input_copy[i] != '\0'; i++) {
        if (input_copy[i] == '-') input_copy[i] = '+';
        else if (input_copy[i] == '_') input_copy[i] = '/';
    }

    TauValue temp = {.type = 2, .value.s = input_copy, .refcount = 1, .next = NULL};
    TauValue result = tauraro_base64_b64decode(temp);

    free(input_copy);
    return result;
}

// ==========================================
// BASE16 (HEX) ENCODING
// ==========================================

// base64.b16encode(s) - Encode bytes to hexadecimal
static inline TauValue tauraro_base64_b16encode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    const unsigned char* input = (const unsigned char*)data.value.s;
    size_t input_len = strlen(data.value.s);

    char* output = (char*)malloc(input_len * 2 + 1);
    if (!output) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    for (size_t i = 0; i < input_len; i++) {
        sprintf(output + (i * 2), "%02X", input[i]);
    }

    return (TauValue){.type = 2, .value.s = output, .refcount = 1, .next = NULL};
}

// base64.b16decode(s) - Decode hexadecimal to bytes
static inline TauValue tauraro_base64_b16decode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    const char* input = data.value.s;
    size_t input_len = strlen(input);

    if (input_len % 2 != 0) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    size_t output_len = input_len / 2;
    unsigned char* output = (unsigned char*)malloc(output_len + 1);

    if (!output) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    for (size_t i = 0; i < output_len; i++) {
        sscanf(input + (i * 2), "%2hhx", &output[i]);
    }

    output[output_len] = '\0';

    return (TauValue){.type = 2, .value.s = (char*)output, .refcount = 1, .next = NULL};
}

// ==========================================
// BASE32 ENCODING (Simplified)
// ==========================================

static const char base32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

// base64.b32encode(s) - Encode bytes to base32
static inline TauValue tauraro_base64_b32encode(TauValue data) {
    if (data.type != 2) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    const unsigned char* input = (const unsigned char*)data.value.s;
    size_t input_len = strlen(data.value.s);

    size_t output_len = ((input_len + 4) / 5) * 8;
    char* output = (char*)malloc(output_len + 1);

    if (!output) {
        return (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    }

    size_t i, j;
    for (i = 0, j = 0; i < input_len;) {
        uint64_t buffer = 0;
        int bytes_read = 0;

        for (int k = 0; k < 5 && i < input_len; k++, i++) {
            buffer = (buffer << 8) | input[i];
            bytes_read++;
        }

        buffer <<= (5 - bytes_read) * 8;

        for (int k = 0; k < 8; k++) {
            if (j < output_len) {
                output[j++] = base32_table[(buffer >> (35 - k * 5)) & 0x1F];
            }
        }
    }

    // Add padding
    while (j < output_len) {
        output[j++] = '=';
    }

    output[output_len] = '\0';

    return (TauValue){.type = 2, .value.s = output, .refcount = 1, .next = NULL};
}
