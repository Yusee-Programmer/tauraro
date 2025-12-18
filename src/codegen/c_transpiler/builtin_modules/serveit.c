// ==========================================
// SERVEIT MODULE - Pure C Implementation (Web Server)
// ==========================================
// Provides: run, Request, Response, Router, WebApp
// Platform: Cross-platform

#ifndef TAURARO_SERVEIT_MODULE_H
#define TAURARO_SERVEIT_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Request structure
typedef struct {
    char* method;
    char* path;
    char* body;
    char** headers;
    int header_count;
} Request;

// Response structure
typedef struct {
    int status_code;
    char* body;
    char** headers;
    int header_count;
} Response;

// Router structure
typedef struct {
    char** routes;
    void** handlers;
    int route_count;
} Router;

// WebApp structure
typedef struct {
    Router router;
    int port;
    int running;
} WebApp;

// serveit.run(app, host='localhost', port=8000)
static inline TauValue tauraro_serveit_run(TauValue app, TauValue host, TauValue port) {
    int p = port.type == 0 ? port.value.i : 8000;
    printf("Starting server on port %d\n", p);
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// serveit.Request class
static inline TauValue tauraro_serveit_Request(void) {
    Request* req = (Request*)malloc(sizeof(Request));
    req->method = NULL;
    req->path = NULL;
    req->body = NULL;
    req->headers = NULL;
    req->header_count = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)req, .refcount = 1, .next = NULL};
}

// serveit.Request.method property
static inline TauValue tauraro_serveit_Request_method(TauValue req) {
    if (req.type != 6) return (TauValue){.type = 2, .value.s = "GET", .refcount = 1, .next = NULL};
    
    Request* r = (Request*)req.value.p;
    return (TauValue){.type = 2, .value.s = r->method ? r->method : "GET", .refcount = 1, .next = NULL};
}

// serveit.Request.path property
static inline TauValue tauraro_serveit_Request_path(TauValue req) {
    if (req.type != 6) return (TauValue){.type = 2, .value.s = "/", .refcount = 1, .next = NULL};
    
    Request* r = (Request*)req.value.p;
    return (TauValue){.type = 2, .value.s = r->path ? r->path : "/", .refcount = 1, .next = NULL};
}

// serveit.Request.body property
static inline TauValue tauraro_serveit_Request_body(TauValue req) {
    if (req.type != 6) return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    
    Request* r = (Request*)req.value.p;
    return (TauValue){.type = 2, .value.s = r->body ? r->body : "", .refcount = 1, .next = NULL};
}

// serveit.Request.json() method
static inline TauValue tauraro_serveit_Request_json(TauValue req) {
    return (TauValue){.type = 5, .value.p = NULL, .refcount = 1, .next = NULL};  // Dict
}

// serveit.Response class
static inline TauValue tauraro_serveit_Response(TauValue body) {
    Response* resp = (Response*)malloc(sizeof(Response));
    resp->status_code = 200;
    resp->body = (char*)malloc(strlen(body.type == 2 ? body.value.s : ""));
    strcpy(resp->body, body.type == 2 ? body.value.s : "");
    resp->headers = NULL;
    resp->header_count = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)resp, .refcount = 1, .next = NULL};
}

// serveit.Response with status code
static inline TauValue tauraro_serveit_Response_status(TauValue body, TauValue status) {
    Response* resp = (Response*)malloc(sizeof(Response));
    resp->status_code = status.type == 0 ? status.value.i : 200;
    resp->body = (char*)malloc(strlen(body.type == 2 ? body.value.s : ""));
    strcpy(resp->body, body.type == 2 ? body.value.s : "");
    resp->headers = NULL;
    resp->header_count = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)resp, .refcount = 1, .next = NULL};
}

// serveit.Response.json(data)
static inline TauValue tauraro_serveit_Response_json(TauValue data) {
    return (TauValue){.type = 6, .value.p = NULL, .refcount = 1, .next = NULL};
}

// serveit.Response.html(html)
static inline TauValue tauraro_serveit_Response_html(TauValue html) {
    return tauraro_serveit_Response(html);
}

// serveit.Router class
static inline TauValue tauraro_serveit_Router(void) {
    Router* router = (Router*)malloc(sizeof(Router));
    router->routes = NULL;
    router->handlers = NULL;
    router->route_count = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)router, .refcount = 1, .next = NULL};
}

// serveit.Router.get(path, handler)
static inline TauValue tauraro_serveit_Router_get(TauValue router, TauValue path, TauValue handler) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// serveit.Router.post(path, handler)
static inline TauValue tauraro_serveit_Router_post(TauValue router, TauValue path, TauValue handler) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// serveit.Router.put(path, handler)
static inline TauValue tauraro_serveit_Router_put(TauValue router, TauValue path, TauValue handler) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// serveit.Router.delete(path, handler)
static inline TauValue tauraro_serveit_Router_delete(TauValue router, TauValue path, TauValue handler) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// serveit.WebApp class
static inline TauValue tauraro_serveit_WebApp(void) {
    WebApp* app = (WebApp*)malloc(sizeof(WebApp));
    app->port = 8000;
    app->running = 0;
    app->router.routes = NULL;
    app->router.handlers = NULL;
    app->router.route_count = 0;
    
    return (TauValue){.type = 6, .value.p = (void*)app, .refcount = 1, .next = NULL};
}

// serveit.WebApp.run()
static inline TauValue tauraro_serveit_WebApp_run(TauValue app) {
    if (app.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebApp* webapp = (WebApp*)app.value.p;
    webapp->running = 1;
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}


#endif // TAURARO_SERVEIT_MODULE_H
