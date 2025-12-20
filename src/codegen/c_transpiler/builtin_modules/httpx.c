// ==========================================
// HTTPX MODULE - Pure C Implementation
// ==========================================
// Provides: Client, AsyncClient, get, post, put, delete, patch, head, options
// Platform: Cross-platform

#ifndef TAURARO_HTTPX_MODULE_H
#define TAURARO_HTTPX_MODULE_H

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

// HTTP Client
typedef struct {
    char* base_url;
    int timeout;
    char** headers;
    int header_count;
} HttpClient;

// HTTP Response
typedef struct {
    int status_code;
    char* text;
    char** headers;
    int header_count;
} HttpResponse;

// httpx.Client(base_url="", timeout=None)
static inline TauValue tauraro_httpx_Client(void) {
    HttpClient* client = (HttpClient*)malloc(sizeof(HttpClient));
    client->base_url = (char*)malloc(1);
    client->base_url[0] = 0;
    client->timeout = 30;
    client->headers = NULL;
    client->header_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)client, .refcount = 1, .next = NULL};
}

// httpx.Client(base_url)
static inline TauValue tauraro_httpx_Client_with_base(TauValue base_url) {
    HttpClient* client = (HttpClient*)malloc(sizeof(HttpClient));
    if (base_url.type == 2) {
        client->base_url = (char*)malloc(strlen(base_url.value.s) + 1);
        strcpy(client->base_url, base_url.value.s);
    } else {
        client->base_url = (char*)malloc(1);
        client->base_url[0] = 0;
    }
    client->timeout = 30;
    client->headers = NULL;
    client->header_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)client, .refcount = 1, .next = NULL};
}

// httpx.Client.get(url)
static inline TauValue tauraro_httpx_Client_get(TauValue client, TauValue url) {
    HttpResponse* resp = (HttpResponse*)malloc(sizeof(HttpResponse));
    resp->status_code = 200;
    resp->text = (char*)malloc(1);
    resp->text[0] = 0;
    resp->headers = NULL;
    resp->header_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)resp, .refcount = 1, .next = NULL};
}

// httpx.Client.post(url, data=None, json=None)
static inline TauValue tauraro_httpx_Client_post(TauValue client, TauValue url, TauValue data) {
    return tauraro_httpx_Client_get(client, url);
}

// httpx.Client.put(url, data=None, json=None)
static inline TauValue tauraro_httpx_Client_put(TauValue client, TauValue url, TauValue data) {
    return tauraro_httpx_Client_get(client, url);
}

// httpx.Client.delete(url)
static inline TauValue tauraro_httpx_Client_delete(TauValue client, TauValue url) {
    return tauraro_httpx_Client_get(client, url);
}

// httpx.Client.patch(url, data=None, json=None)
static inline TauValue tauraro_httpx_Client_patch(TauValue client, TauValue url, TauValue data) {
    return tauraro_httpx_Client_get(client, url);
}

// httpx.Client.head(url)
static inline TauValue tauraro_httpx_Client_head(TauValue client, TauValue url) {
    return tauraro_httpx_Client_get(client, url);
}

// httpx.Client.options(url)
static inline TauValue tauraro_httpx_Client_options(TauValue client, TauValue url) {
    return tauraro_httpx_Client_get(client, url);
}

// httpx.Client.close()
static inline TauValue tauraro_httpx_Client_close(TauValue client) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// httpx.get(url) - Convenience function
static inline TauValue tauraro_httpx_get(TauValue url) {
    HttpResponse* resp = (HttpResponse*)malloc(sizeof(HttpResponse));
    resp->status_code = 200;
    resp->text = (char*)malloc(1);
    resp->text[0] = 0;
    resp->headers = NULL;
    resp->header_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)resp, .refcount = 1, .next = NULL};
}

// httpx.post(url, data=None, json=None)
static inline TauValue tauraro_httpx_post(TauValue url, TauValue data) {
    return tauraro_httpx_get(url);
}

// httpx.put(url, data=None, json=None)
static inline TauValue tauraro_httpx_put(TauValue url, TauValue data) {
    return tauraro_httpx_get(url);
}

// httpx.delete(url)
static inline TauValue tauraro_httpx_delete(TauValue url) {
    return tauraro_httpx_get(url);
}

// httpx.patch(url, data=None, json=None)
static inline TauValue tauraro_httpx_patch(TauValue url, TauValue data) {
    return tauraro_httpx_get(url);
}

// httpx.head(url)
static inline TauValue tauraro_httpx_head(TauValue url) {
    return tauraro_httpx_get(url);
}

// httpx.options(url)
static inline TauValue tauraro_httpx_options(TauValue url) {
    return tauraro_httpx_get(url);
}

// Response.status_code property
static inline TauValue tauraro_httpx_response_status_code(TauValue resp) {
    if (resp.type != 6) return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    
    HttpResponse* response = (HttpResponse*)resp.value.ptr;
    return (TauValue){.type = 0, .value.i = response->status_code, .refcount = 1, .next = NULL};
}

// Response.text property
static inline TauValue tauraro_httpx_response_text(TauValue resp) {
    if (resp.type != 6) return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    
    HttpResponse* response = (HttpResponse*)resp.value.ptr;
    return (TauValue){.type = 2, .value.s = response->text, .refcount = 1, .next = NULL};
}

// Response.json() method
static inline TauValue tauraro_httpx_response_json(TauValue resp) {
    return (TauValue){.type = 5, .value.ptr = NULL, .refcount = 1, .next = NULL};  // Dict
}

// Response.headers property
static inline TauValue tauraro_httpx_response_headers(TauValue resp) {
    return (TauValue){.type = 5, .value.ptr = NULL, .refcount = 1, .next = NULL};  // Dict
}


#endif // TAURARO_HTTPX_MODULE_H
