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
    
    return (TauValue){.type = 6, .value.p = (void*)req, .refcount = 1, .next = NULL};
}

// httptools.HttpRequestParser.feed_data(data)
static inline TauValue tauraro_httptools_HttpRequestParser_feed_data(TauValue parser, TauValue data) {
    if (data.type != 2) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    HttpRequest* req = (HttpRequest*)parser.value.p;
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
    
    return (TauValue){.type = 6, .value.p = (void*)resp, .refcount = 1, .next = NULL};
}

// httptools.HttpResponseParser.feed_data(data)
static inline TauValue tauraro_httptools_HttpResponseParser_feed_data(TauValue parser, TauValue data) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// httptools.parse_url(url)
static inline TauValue tauraro_httptools_parse_url(TauValue url) {
    if (url.type != 2) return (TauValue){.type = 5, .value.p = NULL, .refcount = 1, .next = NULL};
    
    // Would parse URL
    return (TauValue){.type = 5, .value.p = NULL, .refcount = 1, .next = NULL};  // Dict
}

// httptools.URL class
static inline TauValue tauraro_httptools_URL(TauValue url) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
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
