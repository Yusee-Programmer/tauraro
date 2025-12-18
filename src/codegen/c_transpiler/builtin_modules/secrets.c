// ==========================================
// SECRETS MODULE - Pure C Implementation
// ==========================================
// Provides: secrets.randbelow(), secrets.choice(), secrets.token_hex(), secrets.token_urlsafe()
// Platform: Cross-platform

#ifndef TAURARO_SECRETS_MODULE_H
#define TAURARO_SECRETS_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #include <wincrypt.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

// Secure random number generator using OS entropy
static uint64_t tauraro_secure_random_bytes(unsigned char* buffer, size_t length) {
#ifdef _WIN32
    HCRYPTPROV hProv = 0;
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) {
        if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
            return 0;
        }
    }
    
    BOOL success = CryptGenRandom(hProv, (DWORD)length, buffer);
    CryptReleaseContext(hProv, 0);
    return success ? length : 0;
#else
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        return 0;
    }
    
    ssize_t n = read(fd, buffer, length);
    close(fd);
    return (n > 0) ? n : 0;
#endif
}

// secrets.randbelow(n) - Return random integer in [0, n)
static inline TauValue tauraro_secrets_randbelow(TauValue n) {
    if (n.type != 0 || n.value.i <= 0) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    unsigned char bytes[8];
    if (!tauraro_secure_random_bytes(bytes, sizeof(uint64_t))) {
        // Fallback to rand() if secure random fails
        return (TauValue){.type = 0, .value.i = rand() % n.value.i, .refcount = 1, .next = NULL};
    }

    uint64_t random_val = 0;
    for (int i = 0; i < 8; i++) {
        random_val = (random_val << 8) | bytes[i];
    }

    return (TauValue){.type = 0, .value.i = random_val % n.value.i, .refcount = 1, .next = NULL};
}

// secrets.choice(sequence) - Return random element from list
static inline TauValue tauraro_secrets_choice(TauValue sequence) {
    if (sequence.type != 4) {  // Not a list
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    TauList* list = sequence.value.list;
    if (list->size == 0) {
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    TauValue size_val = (TauValue){.type = 0, .value.i = (int64_t)list->size, .refcount = 1, .next = NULL};
    TauValue idx_val = tauraro_secrets_randbelow(size_val);

    return list->items[(size_t)idx_val.value.i];
}

// Helper: Convert byte to hex string
static const char hex_chars[] = "0123456789abcdef";

// secrets.token_hex(nbytes) - Generate hex token
static inline TauValue tauraro_secrets_token_hex(TauValue nbytes) {
    size_t num_bytes = 32;  // Default 32 bytes

    if (nbytes.type == 0 && nbytes.value.i > 0) {
        num_bytes = (size_t)nbytes.value.i;
    }

    unsigned char* random_bytes = malloc(num_bytes);
    if (!tauraro_secure_random_bytes(random_bytes, num_bytes)) {
        // Fallback to pseudo-random if secure random fails
        for (size_t i = 0; i < num_bytes; i++) {
            random_bytes[i] = rand() % 256;
        }
    }

    // Convert to hex string
    char* hex_string = malloc(num_bytes * 2 + 1);
    for (size_t i = 0; i < num_bytes; i++) {
        hex_string[i * 2] = hex_chars[random_bytes[i] >> 4];
        hex_string[i * 2 + 1] = hex_chars[random_bytes[i] & 0x0F];
    }
    hex_string[num_bytes * 2] = '\0';

    free(random_bytes);
    TauValue __result = (TauValue){.type = 2, .value.s = hex_string, .refcount = 1, .next = NULL}; return __result;
}

// Base64url alphabet
static const char base64url_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// secrets.token_urlsafe(nbytes) - Generate URL-safe base64 token
static inline TauValue tauraro_secrets_token_urlsafe(TauValue nbytes) {
    size_t num_bytes = 32;  // Default 32 bytes

    if (nbytes.type == 0 && nbytes.value.i > 0) {
        num_bytes = (size_t)nbytes.value.i;
    }

    unsigned char* random_bytes = malloc(num_bytes);
    if (!tauraro_secure_random_bytes(random_bytes, num_bytes)) {
        for (size_t i = 0; i < num_bytes; i++) {
            random_bytes[i] = rand() % 256;
        }
    }

    // Base64url encoding
    size_t output_len = (num_bytes + 2) / 3 * 4;
    char* encoded = malloc(output_len + 1);
    
    size_t out_idx = 0;
    for (size_t i = 0; i < num_bytes; i += 3) {
        uint32_t val = 0;
        int bytes_read = (num_bytes - i < 3) ? (num_bytes - i) : 3;

        for (int j = 0; j < bytes_read; j++) {
            val = (val << 8) | random_bytes[i + j];
        }

        if (bytes_read == 3) {
            encoded[out_idx++] = base64url_chars[(val >> 18) & 0x3F];
            encoded[out_idx++] = base64url_chars[(val >> 12) & 0x3F];
            encoded[out_idx++] = base64url_chars[(val >> 6) & 0x3F];
            encoded[out_idx++] = base64url_chars[val & 0x3F];
        } else if (bytes_read == 2) {
            val <<= 8;
            encoded[out_idx++] = base64url_chars[(val >> 18) & 0x3F];
            encoded[out_idx++] = base64url_chars[(val >> 12) & 0x3F];
            encoded[out_idx++] = base64url_chars[(val >> 6) & 0x3F];
        } else {
            val <<= 16;
            encoded[out_idx++] = base64url_chars[(val >> 18) & 0x3F];
            encoded[out_idx++] = base64url_chars[(val >> 12) & 0x3F];
        }
    }

    encoded[out_idx] = '\0';
    free(random_bytes);

    TauValue __result = (TauValue){.type = 2, .value.s = encoded, .refcount = 1, .next = NULL}; return __result;
}

// secrets.token_bytes(nbytes) - Generate random bytes as string
static inline TauValue tauraro_secrets_token_bytes(TauValue nbytes) {
    size_t num_bytes = 32;  // Default 32 bytes

    if (nbytes.type == 0 && nbytes.value.i > 0) {
        num_bytes = (size_t)nbytes.value.i;
    }

    unsigned char* random_bytes = malloc(num_bytes);
    if (!tauraro_secure_random_bytes(random_bytes, num_bytes)) {
        for (size_t i = 0; i < num_bytes; i++) {
            random_bytes[i] = rand() % 256;
        }
    }

    // Convert bytes to string (as printable representation)
    char* result = malloc(num_bytes + 1);
    memcpy(result, random_bytes, num_bytes);
    result[num_bytes] = '\0';

    free(random_bytes);
    TauValue __result = (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL}; return __result;
}

// secrets.compare_digest(a, b) - Constant-time comparison
static inline TauValue tauraro_secrets_compare_digest(TauValue a, TauValue b) {
    if (a.type != 2 || b.type != 2) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    const char* str_a = a.value.s;
    const char* str_b = b.value.s;
    size_t len_a = strlen(str_a);
    size_t len_b = strlen(str_b);

    if (len_a != len_b) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }

    // Constant-time comparison
    volatile unsigned int result = 0;
    for (size_t i = 0; i < len_a; i++) {
        result |= (unsigned int)(str_a[i] ^ str_b[i]);
    }

    return (TauValue){.type = 3, .value.i = (result == 0) ? 1 : 0, .refcount = 1, .next = NULL};
}


#endif // TAURARO_SECRETS_MODULE_H
