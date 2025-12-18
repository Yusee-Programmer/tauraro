// ==========================================
// EXTERNAL LIBRARIES CONFIGURATION HEADER
// ==========================================
// Manages availability of external C libraries
// Auto-configured by CMake and build.rs
// Platform: Cross-platform (Windows/Linux/macOS)

#ifndef TAURARO_EXTERNAL_LIBS_CONFIG_H
#define TAURARO_EXTERNAL_LIBS_CONFIG_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// FEATURE DETECTION MACROS
// ==========================================

// Set by CMake during build configuration
// Default: try to detect at runtime

#ifndef HAVE_OPENSSL
#define HAVE_OPENSSL 0
#endif

#ifndef HAVE_SQLITE3
#define HAVE_SQLITE3 0
#endif

#ifndef HAVE_CURL
#define HAVE_CURL 0
#endif

#ifndef HAVE_ZLIB
#define HAVE_ZLIB 0
#endif

#ifndef HAVE_PCRE2
#define HAVE_PCRE2 0
#endif

#ifndef HAVE_PCRE
#define HAVE_PCRE 0
#endif

#ifndef HAVE_LIBFFI
#define HAVE_LIBFFI 0
#endif

#ifndef HAVE_LIBUV
#define HAVE_LIBUV 0
#endif

#ifndef HAVE_LIBEV
#define HAVE_LIBEV 0
#endif

// ==========================================
// CONDITIONAL INCLUDES
// ==========================================

// OpenSSL Headers
#if HAVE_OPENSSL
    #include <openssl/ssl.h>
    #include <openssl/err.h>
    #include <openssl/md5.h>
    #include <openssl/sha.h>
    #include <openssl/evp.h>
    #include <openssl/aes.h>
    #include <openssl/rand.h>
    #define OPENSSL_AVAILABLE 1
    
    // Version check
    #define OPENSSL_MIN_VERSION 0x10101000L
    #if OPENSSL_VERSION_NUMBER < OPENSSL_MIN_VERSION
        #error "OpenSSL 1.1.1+ required"
    #endif
#else
    #define OPENSSL_AVAILABLE 0
    #define SSL_CTX void
    #define SSL void
    #define EVP_PKEY void
    #define X509 void
#endif

// SQLite3 Headers
#if HAVE_SQLITE3
    #include <sqlite3.h>
    #define SQLITE3_AVAILABLE 1
    
    // Version check
    #if SQLITE_VERSION_NUMBER < 3008000
        #error "SQLite 3.8.0+ required"
    #endif
#else
    #define SQLITE3_AVAILABLE 0
    #define sqlite3 void
    #define sqlite3_stmt void
#endif

// libcurl Headers
#if HAVE_CURL
    #include <curl/curl.h>
    #define CURL_AVAILABLE 1
#else
    #define CURL_AVAILABLE 0
    #define CURL void
#endif

// ZLIB Headers
#if HAVE_ZLIB
    #include <zlib.h>
    #define ZLIB_AVAILABLE 1
#else
    #define ZLIB_AVAILABLE 0
    #define z_stream void
#endif

// PCRE2 Headers
#if HAVE_PCRE2
    #define PCRE2_CODE_UNIT_WIDTH 8
    #include <pcre2.h>
    #define REGEX_AVAILABLE 1
    #define REGEX_ENGINE "PCRE2"
#elif HAVE_PCRE
    #include <pcre.h>
    #define REGEX_AVAILABLE 1
    #define REGEX_ENGINE "PCRE"
#else
    #define REGEX_AVAILABLE 0
    #define REGEX_ENGINE "NONE"
    #define pcre void
    #define pcre_extra void
#endif

// libffi Headers
#if HAVE_LIBFFI
    #include <ffi.h>
    #define FFI_AVAILABLE 1
#else
    #define FFI_AVAILABLE 0
#endif

// libuv Headers
#if HAVE_LIBUV
    #include <uv.h>
    #define ASYNC_AVAILABLE 1
    #define ASYNC_ENGINE "libuv"
#elif HAVE_LIBEV
    #include <ev.h>
    #define ASYNC_AVAILABLE 1
    #define ASYNC_ENGINE "libev"
#else
    #define ASYNC_AVAILABLE 0
    #define ASYNC_ENGINE "NONE"
#endif

// ==========================================
// PLATFORM-SPECIFIC HEADERS
// ==========================================

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #define PLATFORM "Windows"
    
    #if HAVE_CURL
        // WebView2 SDK (if available)
        #pragma comment(lib, "ws2_32.lib")
        #pragma comment(lib, "crypt32.lib")
    #endif
#elif defined(__APPLE__)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #define PLATFORM "macOS"
    
    // macOS frameworks
    #ifdef __OBJC__
        #import <Foundation/Foundation.h>
        #import <Cocoa/Cocoa.h>
        #import <WebKit/WebKit.h>
    #endif
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #define PLATFORM "Linux"
    
    // GTK headers (if GUI support enabled)
    // #include <gtk/gtk.h>
    // #include <webkit2/webkit2.h>
#endif

// ==========================================
// RUNTIME CAPABILITY DETECTION
// ==========================================

typedef struct {
    int openssl;
    int sqlite3;
    int curl;
    int zlib;
    int pcre;
    int ffi;
    int libuv;
} LibraryCapabilities;

// Detect available libraries at runtime
static inline LibraryCapabilities detect_libraries(void) {
    LibraryCapabilities caps = {0};
    
    #if HAVE_OPENSSL
    caps.openssl = 1;
    #endif
    
    #if HAVE_SQLITE3
    caps.sqlite3 = 1;
    #endif
    
    #if HAVE_CURL
    caps.curl = 1;
    #endif
    
    #if HAVE_ZLIB
    caps.zlib = 1;
    #endif
    
    #if HAVE_PCRE2 || HAVE_PCRE
    caps.pcre = 1;
    #endif
    
    #if HAVE_LIBFFI
    caps.ffi = 1;
    #endif
    
    #if HAVE_LIBUV
    caps.libuv = 1;
    #endif
    
    return caps;
}

// ==========================================
// ERROR HANDLING
// ==========================================

typedef struct {
    char* library;
    char* function;
    char* error_message;
    int error_code;
} LibraryError;

// Error reporting for missing libraries
static inline void report_library_missing(const char* lib_name) {
    fprintf(stderr, "WARNING: External library '%s' not available\n", lib_name);
    fprintf(stderr, "         Some functionality will be limited.\n");
    fprintf(stderr, "         For full support, install: %s\n\n", lib_name);
}

// ==========================================
// FALLBACK IMPLEMENTATIONS
// ==========================================

// For when libraries are not available, provide minimal fallbacks

#if !HAVE_OPENSSL
    // Minimal hash implementation (NOT SECURE - for demo only)
    static inline void simple_sha256(const unsigned char* data, size_t len, unsigned char* digest) {
        // This is a placeholder - do NOT use for real crypto
        unsigned int hash = 5381;
        for (size_t i = 0; i < len; i++) {
            hash = ((hash << 5) + hash) + data[i];
        }
        memset(digest, 0, 32);
        for (int i = 0; i < 4; i++) {
            ((unsigned int*)digest)[i] = hash ^ (hash >> (i * 8));
        }
    }
#endif

#if !HAVE_SQLITE3
    // Fallback: In-memory data store (very limited)
    typedef struct {
        char* key;
        void* value;
        struct sqlite3* next;
    } SimpleSQLiteEntry;
#endif

// ==========================================
// LIBRARY INFORMATION
// ==========================================

static inline void print_library_info(void) {
    printf("\n=== Tauraro External Libraries Info ===\n\n");
    printf("Platform: %s\n\n", PLATFORM);
    printf("Available Libraries:\n");
    
    #if HAVE_OPENSSL
    printf("  ✓ OpenSSL (version: %s)\n", OPENSSL_VERSION_TEXT);
    #else
    printf("  ✗ OpenSSL - NOT AVAILABLE\n");
    #endif
    
    #if HAVE_SQLITE3
    printf("  ✓ SQLite3 (version: %s)\n", SQLITE_VERSION);
    #else
    printf("  ✗ SQLite3 - NOT AVAILABLE\n");
    #endif
    
    #if HAVE_CURL
    printf("  ✓ libcurl\n");
    #else
    printf("  ✗ libcurl - NOT AVAILABLE\n");
    #endif
    
    #if HAVE_ZLIB
    printf("  ✓ ZLIB (version: %s)\n", ZLIB_VERSION);
    #else
    printf("  ✗ ZLIB - NOT AVAILABLE\n");
    #endif
    
    #if HAVE_PCRE2
    printf("  ✓ PCRE2 (regex)\n");
    #elif HAVE_PCRE
    printf("  ✓ PCRE (regex)\n");
    #else
    printf("  ✗ PCRE/PCRE2 - NOT AVAILABLE\n");
    #endif
    
    #if HAVE_LIBFFI
    printf("  ✓ libffi (FFI)\n");
    #else
    printf("  ✗ libffi - NOT AVAILABLE\n");
    #endif
    
    #if HAVE_LIBUV
    printf("  ✓ libuv (async I/O)\n");
    #elif HAVE_LIBEV
    printf("  ✓ libev (async I/O)\n");
    #else
    printf("  ✗ libuv/libev - NOT AVAILABLE\n");
    #endif
    
    printf("\n");
}

#endif // TAURARO_EXTERNAL_LIBS_CONFIG_H
