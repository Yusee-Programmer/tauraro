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

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET socket_t;
    #define CLOSE_SOCKET closesocket
    #define SOCKET_ERROR_CHECK(s) ((s) == INVALID_SOCKET)
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int socket_t;
    #define CLOSE_SOCKET close
    #define SOCKET_ERROR_CHECK(s) ((s) < 0)
    #define INVALID_SOCKET -1
#endif

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

// Helper macro to create a placeholder function value
#define TAURARO_FUNC_VALUE(func_ptr) \
    ((TauValue){.type = 7, .value.func = NULL, .refcount = 1, .next = NULL})

// Global function pointer for the user's app function
static TauValue (*tauraro_serveit_app_handler)(TauValue) = NULL;

// Function to set the app handler (called by generated code)
static inline void tauraro_serveit_set_app_handler(TauValue (*handler)(TauValue)) {
    tauraro_serveit_app_handler = handler;
}

// Helper function to parse HTTP request path
static char* parse_http_path(const char* request_line) {
    // Find first space (after method)
    const char* path_start = strchr(request_line, ' ');
    if (!path_start) return strdup("/");
    path_start++; // Skip space
    
    // Find second space (before HTTP version)
    const char* path_end = strchr(path_start, ' ');
    if (!path_end) return strdup("/");
    
    // Also check for query string
    const char* query_start = strchr(path_start, '?');
    if (query_start && query_start < path_end) {
        path_end = query_start;  // Stop at the query string
    }
    
    // Extract path
    size_t path_len = path_end - path_start;
    if (path_len == 0) return strdup("/");
    
    char* path = (char*)malloc(path_len + 1);
    strncpy(path, path_start, path_len);
    path[path_len] = '\0';
    return path;
}

// serveit.run(app, host='localhost', port=8000)
static inline TauValue tauraro_serveit_run(TauValue app, TauValue host, TauValue port) {
    char* host_str = (host.type == 2) ? host.value.s : "127.0.0.1";
    int p = (port.type == 0) ? port.value.i : 8000;
    
    printf("\n");
    printf("╭─────────────────────────────────────────────────────╮\n");
    printf("│  ServEit - C Compiled HTTP Server                  │\n");
    printf("╰─────────────────────────────────────────────────────╯\n");
    printf("\n");
    printf("  Starting server at: http://%s:%d\n", host_str, p);
    printf("  Press CTRL+C to quit\n\n");
    
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("  [ERROR] Failed to initialize Winsock\n");
        return (TauValue){.type = -1, .value.i = 0, .refcount = 1, .next = NULL};
    }
#endif
    
    // Create socket
    socket_t server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (SOCKET_ERROR_CHECK(server_socket)) {
        printf("  [ERROR] Failed to create socket\n");
#ifdef _WIN32
        WSACleanup();
#endif
        return (TauValue){.type = -1, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    // Set socket options
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    
    // Bind socket
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(p);
    server_addr.sin_addr.s_addr = inet_addr(host_str);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("  [ERROR] Failed to bind to port %d\n", p);
        CLOSE_SOCKET(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return (TauValue){.type = -1, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    // Listen
    if (listen(server_socket, 10) < 0) {
        printf("  [ERROR] Failed to listen on socket\n");
        CLOSE_SOCKET(server_socket);
#ifdef _WIN32
        WSACleanup();
#endif
        return (TauValue){.type = -1, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    printf("  Server started successfully!\n");
    printf("  Listening on: http://%s:%d\n\n", host_str, p);
    
    // Main server loop
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        socket_t client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        
        if (SOCKET_ERROR_CHECK(client_socket)) {
            continue;
        }
        
        // Read request
        char buffer[8192] = {0};
        int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            
            // Parse request path
            char* path = parse_http_path(buffer);
            
            // Create scope dict for ASGI
            TauDict* scope_dict = tauraro_create_dict();
            
            // Add path to scope
            TauValue path_value = {.type = 2, .value.s = path, .refcount = 1, .next = NULL};
            tauraro_dict_set(scope_dict, "path", path_value);
            
            // Add type to scope
            TauValue type_value = {.type = 2, .value.s = "http", .refcount = 1, .next = NULL};
            tauraro_dict_set(scope_dict, "type", type_value);
            
            // Add method to scope
            TauValue method_value = {.type = 2, .value.s = "GET", .refcount = 1, .next = NULL};
            tauraro_dict_set(scope_dict, "method", method_value);
            
            TauValue scope_arg = {.type = 5, .value.dict = scope_dict, .refcount = 1, .next = NULL};
            
            // Call app function if handler is set
            TauValue response;
            if (tauraro_serveit_app_handler != NULL) {
                response = tauraro_serveit_app_handler(scope_arg);
            } else {
                // Fallback response if no handler is set
                TauDict* resp_dict = tauraro_create_dict();
                TauValue status_val = {.type = 0, .value.i = 500, .refcount = 1, .next = NULL};
                tauraro_dict_set(resp_dict, "status", status_val);
                
                TauValue body_val = {.type = 2, .value.s = "<h1>500 Internal Server Error</h1><p>No app handler registered</p>", .refcount = 1, .next = NULL};
                tauraro_dict_set(resp_dict, "body", body_val);
                
                response = (TauValue){.type = 5, .value.dict = resp_dict, .refcount = 1, .next = NULL};
            }
            
            // Extract response fields
            int status_code = 200;
            char* response_body = "OK";
            char* content_type = "text/html; charset=utf-8";
            
            if (response.type == 5) {
                TauDict* resp_dict = response.value.dict;
                
                // Get status
                TauValue* status_val = tauraro_dict_get(resp_dict, "status");
                if (status_val && status_val->type == 0) {
                    status_code = status_val->value.i;
                }
                
                // Get body
                TauValue* body_val = tauraro_dict_get(resp_dict, "body");
                if (body_val && body_val->type == 2) {
                    response_body = body_val->value.s;
                }
                
                // Get content type from headers
                TauValue* headers_val = tauraro_dict_get(resp_dict, "headers");
                if (headers_val && headers_val->type == 5) {
                    TauValue* ct_val = tauraro_dict_get(headers_val->value.dict, "Content-Type");
                    if (ct_val && ct_val->type == 2) {
                        content_type = ct_val->value.s;
                    }
                }
            }
            
            // Build HTTP response
            char http_response[16384];
            int response_len = snprintf(http_response, sizeof(http_response),
                "HTTP/1.1 %d OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %zu\r\n"
                "Connection: close\r\n"
                "\r\n"
                "%s",
                status_code, content_type, strlen(response_body), response_body);
            
            // Send response
            send(client_socket, http_response, response_len, 0);
            
            // Log request
            printf("  %s %s - %d\n", "GET", path, status_code);
            
            free(path);
        }
        
        CLOSE_SOCKET(client_socket);
    }
    
    // Cleanup (never reached in this simple implementation)
    CLOSE_SOCKET(server_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    
    return (TauValue){.type = -1, .value.i = 0, .refcount = 1, .next = NULL};
}

// serveit.Request class
static inline TauValue tauraro_serveit_Request(void) {
    Request* req = (Request*)malloc(sizeof(Request));
    req->method = NULL;
    req->path = NULL;
    req->body = NULL;
    req->headers = NULL;
    req->header_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)req, .refcount = 1, .next = NULL};
}

// serveit.Request.method property
static inline TauValue tauraro_serveit_Request_method(TauValue req) {
    if (req.type != 6) return (TauValue){.type = 2, .value.s = "GET", .refcount = 1, .next = NULL};
    
    Request* r = (Request*)req.value.ptr;
    return (TauValue){.type = 2, .value.s = r->method ? r->method : "GET", .refcount = 1, .next = NULL};
}

// serveit.Request.path property
static inline TauValue tauraro_serveit_Request_path(TauValue req) {
    if (req.type != 6) return (TauValue){.type = 2, .value.s = "/", .refcount = 1, .next = NULL};
    
    Request* r = (Request*)req.value.ptr;
    return (TauValue){.type = 2, .value.s = r->path ? r->path : "/", .refcount = 1, .next = NULL};
}

// serveit.Request.body property
static inline TauValue tauraro_serveit_Request_body(TauValue req) {
    if (req.type != 6) return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    
    Request* r = (Request*)req.value.ptr;
    return (TauValue){.type = 2, .value.s = r->body ? r->body : "", .refcount = 1, .next = NULL};
}

// serveit.Request.json() method
static inline TauValue tauraro_serveit_Request_json(TauValue req) {
    return (TauValue){.type = 5, .value.ptr = NULL, .refcount = 1, .next = NULL};  // Dict
}

// serveit.Response class - Response(status, body)
static inline TauValue tauraro_serveit_Response(TauValue status, TauValue body) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status
    int status_code = (status.type == 0) ? status.value.i : 200;
    TauValue status_val = (TauValue){.type = 0, .value.i = status_code, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // Set body
    if (body.type == 2) {
        tauraro_dict_set(response_dict, "body", body);
    } else {
        TauValue empty = (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
        tauraro_dict_set(response_dict, "body", empty);
    }
    
    // Create headers dict
    TauDict* headers_dict = tauraro_create_dict();
    TauValue content_type = (TauValue){.type = 2, .value.s = strdup("text/html; charset=utf-8"), .refcount = 1, .next = NULL};
    tauraro_dict_set(headers_dict, "content-type", content_type);
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}

// serveit.Response with status code as first argument: Response(status, body)
static inline TauValue tauraro_serveit_Response_2(TauValue status, TauValue body) {
    return tauraro_serveit_Response(status, body);
}

// serveit.Response with status code
static inline TauValue tauraro_serveit_Response_status(TauValue body, TauValue status) {
    Response* resp = (Response*)malloc(sizeof(Response));
    resp->status_code = status.type == 0 ? status.value.i : 200;
    resp->body = (char*)malloc(strlen(body.type == 2 ? body.value.s : ""));
    strcpy(resp->body, body.type == 2 ? body.value.s : "");
    resp->headers = NULL;
    resp->header_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)resp, .refcount = 1, .next = NULL};
}

// serveit.Response.json(data)
static inline TauValue tauraro_serveit_Response_json(TauValue data) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// serveit.Response.html(html)
static inline TauValue tauraro_serveit_Response_html(TauValue html) {
    TauValue status = (TauValue){.type = 0, .value.i = 200, .refcount = 1, .next = NULL};
    return tauraro_serveit_Response(status, html);
}

// serveit.Router class
static inline TauValue tauraro_serveit_Router(void) {
    Router* router = (Router*)malloc(sizeof(Router));
    router->routes = NULL;
    router->handlers = NULL;
    router->route_count = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)router, .refcount = 1, .next = NULL};
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
    
    return (TauValue){.type = 6, .value.ptr = (void*)app, .refcount = 1, .next = NULL};
}

// serveit.WebApp.run()
static inline TauValue tauraro_serveit_WebApp_run(TauValue app) {
    if (app.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    WebApp* webapp = (WebApp*)app.value.ptr;
    webapp->running = 1;
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// serveit.HTMLResponse(body, status=200)
static inline TauValue tauraro_serveit_HTMLResponse(TauValue body) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status
    TauValue status_val = (TauValue){.type = 0, .value.i = 200, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // Set body
    if (body.type == 2) {
        tauraro_dict_set(response_dict, "body", body);
    } else {
        TauValue empty = (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
        tauraro_dict_set(response_dict, "body", empty);
    }
    
    // Create headers dict
    TauDict* headers_dict = tauraro_create_dict();
    TauValue content_type = (TauValue){.type = 2, .value.s = strdup("text/html; charset=utf-8"), .refcount = 1, .next = NULL};
    tauraro_dict_set(headers_dict, "content-type", content_type);
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}

// serveit.HTMLResponse with custom status
static inline TauValue tauraro_serveit_HTMLResponse_status(TauValue body, TauValue status) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status
    int status_code = (status.type == 0) ? status.value.i : 200;
    TauValue status_val = (TauValue){.type = 0, .value.i = status_code, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // Set body
    if (body.type == 2) {
        tauraro_dict_set(response_dict, "body", body);
    } else {
        TauValue empty = (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
        tauraro_dict_set(response_dict, "body", empty);
    }
    
    // Create headers dict
    TauDict* headers_dict = tauraro_create_dict();
    TauValue content_type = (TauValue){.type = 2, .value.s = strdup("text/html; charset=utf-8"), .refcount = 1, .next = NULL};
    tauraro_dict_set(headers_dict, "content-type", content_type);
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}

// serveit.JSONResponse(data, status=200)
static inline TauValue tauraro_serveit_JSONResponse(TauValue data) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status
    TauValue status_val = (TauValue){.type = 0, .value.i = 200, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // For JSON body, we need to serialize the data
    // For now, if it's already a string, use it directly
    // Otherwise, create a simple JSON representation
    char json_body[4096] = {0};
    
    if (data.type == 2) {
        // Already a string, use it
        strncpy(json_body, data.value.s, sizeof(json_body) - 1);
    } else if (data.type == 5) {
        // Dictionary - create simple JSON
        strcpy(json_body, "{");
        TauDict* dict = data.value.dict;
        if (dict && dict->buckets) {
            int first = 1;
            for (size_t i = 0; i < dict->capacity; i++) {
                TauDictEntry* entry = dict->buckets[i];
                while (entry) {
                    if (!first) strcat(json_body, ",");
                    first = 0;
                    strcat(json_body, "\"");
                    strcat(json_body, entry->key);
                    strcat(json_body, "\":");
                    
                    TauValue val = entry->value;
                    if (val.type == 2) {
                        strcat(json_body, "\"");
                        strncat(json_body, val.value.s, 256);
                        strcat(json_body, "\"");
                    } else if (val.type == 0) {
                        char num[32];
                        snprintf(num, sizeof(num), "%lld", val.value.i);
                        strcat(json_body, num);
                    } else if (val.type == 1) {
                        char num[32];
                        snprintf(num, sizeof(num), "%f", val.value.f);
                        strcat(json_body, num);
                    } else if (val.type == 3) {
                        strcat(json_body, val.value.i ? "true" : "false");
                    }
                    entry = entry->next;
                }
            }
        }
        strcat(json_body, "}");
    } else {
        strcpy(json_body, "{}");
    }
    
    TauValue body_val = (TauValue){.type = 2, .value.s = strdup(json_body), .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "body", body_val);
    
    // Create headers dict
    TauDict* headers_dict = tauraro_create_dict();
    TauValue content_type = (TauValue){.type = 2, .value.s = strdup("application/json; charset=utf-8"), .refcount = 1, .next = NULL};
    tauraro_dict_set(headers_dict, "content-type", content_type);
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}

// serveit.JSONResponse with custom status
static inline TauValue tauraro_serveit_JSONResponse_status(TauValue data, TauValue status) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status
    int status_code = (status.type == 0) ? status.value.i : 200;
    TauValue status_val = (TauValue){.type = 0, .value.i = status_code, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // For JSON body, serialize the data
    char json_body[4096] = {0};
    
    if (data.type == 2) {
        strncpy(json_body, data.value.s, sizeof(json_body) - 1);
    } else if (data.type == 5) {
        strcpy(json_body, "{");
        TauDict* dict = data.value.dict;
        if (dict && dict->buckets) {
            int first = 1;
            for (size_t i = 0; i < dict->capacity; i++) {
                TauDictEntry* entry = dict->buckets[i];
                while (entry) {
                    if (!first) strcat(json_body, ",");
                    first = 0;
                    strcat(json_body, "\"");
                    strcat(json_body, entry->key);
                    strcat(json_body, "\":");
                    
                    TauValue val = entry->value;
                    if (val.type == 2) {
                        strcat(json_body, "\"");
                        strncat(json_body, val.value.s, 256);
                        strcat(json_body, "\"");
                    } else if (val.type == 0) {
                        char num[32];
                        snprintf(num, sizeof(num), "%lld", val.value.i);
                        strcat(json_body, num);
                    } else if (val.type == 1) {
                        char num[32];
                        snprintf(num, sizeof(num), "%f", val.value.f);
                        strcat(json_body, num);
                    } else if (val.type == 3) {
                        strcat(json_body, val.value.i ? "true" : "false");
                    }
                    entry = entry->next;
                }
            }
        }
        strcat(json_body, "}");
    } else {
        strcpy(json_body, "{}");
    }
    
    TauValue body_val = (TauValue){.type = 2, .value.s = strdup(json_body), .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "body", body_val);
    
    // Create headers dict
    TauDict* headers_dict = tauraro_create_dict();
    TauValue content_type = (TauValue){.type = 2, .value.s = strdup("application/json; charset=utf-8"), .refcount = 1, .next = NULL};
    tauraro_dict_set(headers_dict, "content-type", content_type);
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}

// serveit.RedirectResponse(url, status=307)
static inline TauValue tauraro_serveit_RedirectResponse(TauValue url) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status (307 = Temporary Redirect)
    TauValue status_val = (TauValue){.type = 0, .value.i = 307, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // Set empty body
    TauValue body_val = (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "body", body_val);
    
    // Create headers dict with location
    TauDict* headers_dict = tauraro_create_dict();
    if (url.type == 2) {
        tauraro_dict_set(headers_dict, "location", url);
    }
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}

// serveit.RedirectResponse with custom status
static inline TauValue tauraro_serveit_RedirectResponse_status(TauValue url, TauValue status) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status
    int status_code = (status.type == 0) ? status.value.i : 307;
    TauValue status_val = (TauValue){.type = 0, .value.i = status_code, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // Set empty body
    TauValue body_val = (TauValue){.type = 2, .value.s = strdup(""), .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "body", body_val);
    
    // Create headers dict with location
    TauDict* headers_dict = tauraro_create_dict();
    if (url.type == 2) {
        tauraro_dict_set(headers_dict, "location", url);
    }
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}

// serveit.FileResponse(path)
static inline TauValue tauraro_serveit_FileResponse(TauValue path) {
    // Create response dictionary
    TauDict* response_dict = tauraro_create_dict();
    
    // Set status
    TauValue status_val = (TauValue){.type = 0, .value.i = 200, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "status", status_val);
    
    // Read file content
    char* file_content = NULL;
    size_t content_length = 0;
    
    if (path.type == 2) {
        FILE* file = fopen(path.value.s, "rb");
        if (file) {
            fseek(file, 0, SEEK_END);
            content_length = ftell(file);
            fseek(file, 0, SEEK_SET);
            
            file_content = (char*)malloc(content_length + 1);
            if (file_content) {
                fread(file_content, 1, content_length, file);
                file_content[content_length] = '\0';
            }
            fclose(file);
        }
    }
    
    if (file_content) {
        TauValue body_val = (TauValue){.type = 2, .value.s = file_content, .refcount = 1, .next = NULL};
        tauraro_dict_set(response_dict, "body", body_val);
    } else {
        // File not found
        status_val.value.i = 404;
        tauraro_dict_set(response_dict, "status", status_val);
        TauValue body_val = (TauValue){.type = 2, .value.s = strdup("File not found"), .refcount = 1, .next = NULL};
        tauraro_dict_set(response_dict, "body", body_val);
    }
    
    // Create headers dict with content-type
    TauDict* headers_dict = tauraro_create_dict();
    
    // Simple MIME type detection based on file extension
    char* content_type_str = "application/octet-stream";
    if (path.type == 2) {
        char* ext = strrchr(path.value.s, '.');
        if (ext) {
            if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) content_type_str = "text/html";
            else if (strcmp(ext, ".css") == 0) content_type_str = "text/css";
            else if (strcmp(ext, ".js") == 0) content_type_str = "application/javascript";
            else if (strcmp(ext, ".json") == 0) content_type_str = "application/json";
            else if (strcmp(ext, ".png") == 0) content_type_str = "image/png";
            else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) content_type_str = "image/jpeg";
            else if (strcmp(ext, ".gif") == 0) content_type_str = "image/gif";
            else if (strcmp(ext, ".svg") == 0) content_type_str = "image/svg+xml";
            else if (strcmp(ext, ".txt") == 0) content_type_str = "text/plain";
        }
    }
    
    TauValue content_type = (TauValue){.type = 2, .value.s = strdup(content_type_str), .refcount = 1, .next = NULL};
    tauraro_dict_set(headers_dict, "content-type", content_type);
    
    TauValue headers_val = (TauValue){.type = 5, .value.dict = headers_dict, .refcount = 1, .next = NULL};
    tauraro_dict_set(response_dict, "headers", headers_val);
    
    return (TauValue){.type = 5, .value.dict = response_dict, .refcount = 1, .next = NULL};
}


#endif // TAURARO_SERVEIT_MODULE_H
