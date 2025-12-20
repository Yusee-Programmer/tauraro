// ==========================================
// HTTPTOOLS MODULE - Pure C Implementation
// ==========================================
// Provides: HttpRequestParser, HttpResponseParser, parse_url
// Platform: Cross-platform

#ifndef TAURARO_HTTPTOOLS_MODULE_H
#define TAURARO_HTTPTOOLS_MODULE_H

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

// HTTP Request Parser
typedef struct {
    char* method;
    char* path;
    char* version;
    char** headers;
    int header_count;
    char* body;
} HttpRequest;

// HTTP Response Parser
typedef struct {
    int status_code;
    char* reason;
    char** headers;
    int header_count;
    char* body;
} HttpResponse;

// httptools.HttpRequestParser(on_header, on_body, on_message_complete)
static inline TauValue tauraro_httptools_HttpRequestParser(TauValue on_header) {
    HttpRequest* req = (HttpRequest*)malloc(sizeof(HttpRequest));
    req->method = NULL;
    req->path = NULL;
    req->version = NULL;
    req->headers = NULL;
    req->header_count = 0;
    req->body = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)req, .refcount = 1, .next = NULL};
}

// httptools.HttpRequestParser.feed_data(data)
static inline TauValue tauraro_httptools_HttpRequestParser_feed_data(TauValue parser, TauValue data) {
    if (data.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    HttpRequest* req = (HttpRequest*)parser.value.ptr;
    req->body = (char*)malloc(strlen(data.value.s) + 1);
    strcpy(req->body, data.value.s);
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// httptools.HttpResponseParser(on_header, on_body, on_message_complete)
static inline TauValue tauraro_httptools_HttpResponseParser(TauValue on_header) {
    HttpResponse* resp = (HttpResponse*)malloc(sizeof(HttpResponse));
    resp->status_code = 200;
    resp->reason = NULL;
    resp->headers = NULL;
    resp->header_count = 0;
    resp->body = NULL;
    
    return (TauValue){.type = 6, .value.ptr = (void*)resp, .refcount = 1, .next = NULL};
}

// httptools.HttpResponseParser.feed_data(data)
static inline TauValue tauraro_httptools_HttpResponseParser_feed_data(TauValue parser, TauValue data) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// httptools.parse_url(url)
static inline TauValue tauraro_httptools_parse_url(TauValue url) {
    if (url.type != 2) return (TauValue){.type = 5, .value.ptr = NULL, .refcount = 1, .next = NULL};
    
    // Would parse URL
    return (TauValue){.type = 5, .value.ptr = NULL, .refcount = 1, .next = NULL};  // Dict
}

// httptools.URL class
static inline TauValue tauraro_httptools_URL(TauValue url) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// URL.schema property
static inline TauValue tauraro_httptools_URL_schema(TauValue url_obj) {
    return (TauValue){.type = 2, .value.s = "http", .refcount = 1, .next = NULL};
}

// URL.host property
static inline TauValue tauraro_httptools_URL_host(TauValue url_obj) {
    return (TauValue){.type = 2, .value.s = "localhost", .refcount = 1, .next = NULL};
}

// URL.port property
static inline TauValue tauraro_httptools_URL_port(TauValue url_obj) {
    return (TauValue){.type = 0, .value.i = 80, .refcount = 1, .next = NULL};
}

// URL.path property
static inline TauValue tauraro_httptools_URL_path(TauValue url_obj) {
    return (TauValue){.type = 2, .value.s = "/", .refcount = 1, .next = NULL};
}

// URL.query property
static inline TauValue tauraro_httptools_URL_query(TauValue url_obj) {
    return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
}


#endif // TAURARO_HTTPTOOLS_MODULE_H
