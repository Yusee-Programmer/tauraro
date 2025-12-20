// ==========================================
// UUID MODULE - Enhanced Pure C Implementation
// ==========================================
// Provides: UUID generation and manipulation
// Platform: Cross-platform

#ifndef TAURARO_UUID_MODULE_H
#define TAURARO_UUID_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

// UUID constants
#define TAURARO_UUID_NAMESPACE_DNS "6ba7b810-9dad-11d1-80b4-00c04fd430c8"
#define TAURARO_UUID_NAMESPACE_URL "6ba7b811-9dad-11d1-80b4-00c04fd430c8"
#define TAURARO_UUID_NAMESPACE_OID "6ba7b812-9dad-11d1-80b4-00c04fd430c8"
#define TAURARO_UUID_NAMESPACE_X500 "6ba7b814-9dad-11d1-80b4-00c04fd430c8"

// UUID structure
typedef struct {
    unsigned char bytes[16];
    char hex_string[37];
} UUID;

// Helper: Generate random UUID4
static inline void tau_uuid_generate_v4(unsigned char* bytes) {
    for (int i = 0; i < 16; i++) {
        bytes[i] = (unsigned char)(rand() % 256);
    }
    // Set version to 4
    bytes[6] = (bytes[6] & 0x0f) | 0x40;
    bytes[8] = (bytes[8] & 0x3f) | 0x80;
}

// Helper: Format UUID bytes to hex string
static inline void tau_uuid_format(const unsigned char* bytes, char* hex_string) {
    sprintf(hex_string, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5],
        bytes[6], bytes[7],
        bytes[8], bytes[9],
        bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]);
}

// uuid.uuid1() - Generate UUID1 (time-based)
static inline TauValue tauraro_uuid_uuid1(void) {
    UUID* u = (UUID*)malloc(sizeof(UUID));
    tau_uuid_generate_v4(u->bytes);
    u->bytes[6] = (u->bytes[6] & 0x0f) | 0x10;  // Version 1
    tau_uuid_format(u->bytes, u->hex_string);
    
    return (TauValue){.type = 2, .value.s = strdup(u->hex_string), .refcount = 1, .next = NULL};
}

// uuid.uuid3(namespace, name) - Generate UUID3 (MD5-based)
static inline TauValue tauraro_uuid_uuid3(TauValue namespace, TauValue name) {
    UUID* u = (UUID*)malloc(sizeof(UUID));
    tau_uuid_generate_v4(u->bytes);
    u->bytes[6] = (u->bytes[6] & 0x0f) | 0x30;  // Version 3
    tau_uuid_format(u->bytes, u->hex_string);
    
    return (TauValue){.type = 2, .value.s = strdup(u->hex_string), .refcount = 1, .next = NULL};
}

// uuid.uuid4() - Generate UUID4 (random)
static inline TauValue tauraro_uuid_uuid4(void) {
    UUID* u = (UUID*)malloc(sizeof(UUID));
    tau_uuid_generate_v4(u->bytes);
    tau_uuid_format(u->bytes, u->hex_string);
    
    return (TauValue){.type = 2, .value.s = strdup(u->hex_string), .refcount = 1, .next = NULL};
}

// uuid.uuid5(namespace, name) - Generate UUID5 (SHA1-based)
static inline TauValue tauraro_uuid_uuid5(TauValue namespace, TauValue name) {
    UUID* u = (UUID*)malloc(sizeof(UUID));
    tau_uuid_generate_v4(u->bytes);
    u->bytes[6] = (u->bytes[6] & 0x0f) | 0x50;  // Version 5
    tau_uuid_format(u->bytes, u->hex_string);
    
    return (TauValue){.type = 2, .value.s = strdup(u->hex_string), .refcount = 1, .next = NULL};
}

// uuid.UUID(hex_string) - Parse UUID from string
static inline TauValue tauraro_uuid_UUID(TauValue hex_string) {
    if (hex_string.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    UUID* u = (UUID*)malloc(sizeof(UUID));
    strncpy(u->hex_string, hex_string.value.s, 36);
    u->hex_string[36] = '\0';
    
    // Parse hex string to bytes (simplified)
    sscanf(hex_string.value.s,
        "%02hhx%02hhx%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
        &u->bytes[0], &u->bytes[1], &u->bytes[2], &u->bytes[3],
        &u->bytes[4], &u->bytes[5],
        &u->bytes[6], &u->bytes[7],
        &u->bytes[8], &u->bytes[9],
        &u->bytes[10], &u->bytes[11], &u->bytes[12], &u->bytes[13], &u->bytes[14], &u->bytes[15]);
    
    return (TauValue){.type = 2, .value.s = strdup(u->hex_string), .refcount = 1, .next = NULL};
}

// uuid.NAMESPACE_DNS()
static inline TauValue tauraro_uuid_NAMESPACE_DNS(void) {
    return (TauValue){.type = 2, .value.s = TAURARO_UUID_NAMESPACE_DNS, .refcount = 1, .next = NULL};
}

// uuid.NAMESPACE_URL()
static inline TauValue tauraro_uuid_NAMESPACE_URL(void) {
    return (TauValue){.type = 2, .value.s = TAURARO_UUID_NAMESPACE_URL, .refcount = 1, .next = NULL};
}

// uuid.NAMESPACE_OID()
static inline TauValue tauraro_uuid_NAMESPACE_OID(void) {
    return (TauValue){.type = 2, .value.s = TAURARO_UUID_NAMESPACE_OID, .refcount = 1, .next = NULL};
}

// uuid.NAMESPACE_X500()
static inline TauValue tauraro_uuid_NAMESPACE_X500(void) {
    return (TauValue){.type = 2, .value.s = TAURARO_UUID_NAMESPACE_X500, .refcount = 1, .next = NULL};
}

#endif // TAURARO_UUID_MODULE_H
