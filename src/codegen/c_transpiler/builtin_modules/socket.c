// ==========================================
// SOCKET MODULE - Pure C Implementation
// ==========================================
// Provides: socket operations, constants, hostname/DNS functions
// Platform: Cross-platform (Windows/Unix)

#ifndef TAURARO_SOCKET_MODULE_H
#define TAURARO_SOCKET_MODULE_H

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
    #define SOCK_CLOSE closesocket
    #define SOCK_ERROR SOCKET_ERROR
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <fcntl.h>
    #define SOCK_CLOSE close
    #define SOCK_ERROR -1
#endif

// Socket constants
#define TAURARO_SOCKET_AF_INET      2
#define TAURARO_SOCKET_AF_INET6     10
#define TAURARO_SOCKET_SOCK_STREAM  1
#define TAURARO_SOCKET_SOCK_DGRAM   2
#define TAURARO_SOCKET_SOCK_RAW     3
#define TAURARO_SOCKET_SOL_SOCKET   1
#define TAURARO_SOCKET_SO_REUSEADDR 2
#define TAURARO_SOCKET_SO_KEEPALIVE 9
#define TAURARO_SOCKET_IPPROTO_TCP  6
#define TAURARO_SOCKET_IPPROTO_UDP  17

// Socket wrapper structure
typedef struct {
    int socket_fd;
    int domain;
    int type;
    int protocol;
    int is_connected;
} SocketWrapper;

// Helper: create socket wrapper
static inline TauValue tau_socket_create(int fd, int domain, int type) {
    SocketWrapper* sock = (SocketWrapper*)malloc(sizeof(SocketWrapper));
    sock->socket_fd = fd;
    sock->domain = domain;
    sock->type = type;
    sock->protocol = (type == TAURARO_SOCKET_SOCK_STREAM) ? TAURARO_SOCKET_IPPROTO_TCP : TAURARO_SOCKET_IPPROTO_UDP;
    sock->is_connected = 0;
    return (TauValue){.type = 6, .value.ptr = (void*)sock, .refcount = 1, .next = NULL};
}

// socket.socket(family=AF_INET, type=SOCK_STREAM, proto=0)
static inline TauValue tauraro_socket_socket(TauValue family, TauValue type) {
    int fam = family.type == 0 ? family.value.i : TAURARO_SOCKET_AF_INET;
    int typ = type.type == 0 ? type.value.i : TAURARO_SOCKET_SOCK_STREAM;
    
    int fd = socket(fam, typ, 0);
    if (fd == SOCK_ERROR) {
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    }
    
    return tau_socket_create(fd, fam, typ);
}

// socket.bind(sockfd, address) - address is tuple (host, port)
static inline TauValue tauraro_socket_bind(TauValue sockval, TauValue address) {
    if (sockval.type != 6) 
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    
    SocketWrapper* sock = (SocketWrapper*)sockval.value.ptr;
    
    // Parse address (host, port) from list
    if (address.type != 4 || !address.value.list || address.value.list->size < 2)
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    
    TauValue host_val = address.value.list->items[0];
    TauValue port_val = address.value.list->items[1];
    
    const char* host = host_val.type == 2 ? host_val.value.s : "0.0.0.0";
    int port = port_val.type == 0 ? port_val.value.i : 0;
    
    struct sockaddr_in addr;
    addr.sin_family = TAURARO_SOCKET_AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);
    
    if (bind(sock->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == SOCK_ERROR) {
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    }
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// socket.listen(sockfd, backlog)
static inline TauValue tauraro_socket_listen(TauValue sockval, TauValue backlog) {
    if (sockval.type != 6) 
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    
    SocketWrapper* sock = (SocketWrapper*)sockval.value.ptr;
    int back = backlog.type == 0 ? backlog.value.i : 1;
    
    if (listen(sock->socket_fd, back) == SOCK_ERROR) {
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    }
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// socket.accept(sockfd) - returns (client_socket, address)
static inline TauValue tauraro_socket_accept(TauValue sockval) {
    if (sockval.type != 6) 
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    
    SocketWrapper* sock = (SocketWrapper*)sockval.value.ptr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    int client_fd = accept(sock->socket_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd == SOCK_ERROR) {
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    }
    
    // Create result tuple [client_socket, (host, port)]
    TauList* result = (TauList*)malloc(sizeof(TauList));
    result->size = 2;
    result->capacity = 4;
    result->items = (TauValue*)malloc(sizeof(TauValue) * result->capacity);
    
    // Client socket
    result->items[0] = tau_socket_create(client_fd, sock->domain, sock->type);
    
    // Address tuple: [host_string, port]
    char host_str[16];
    inet_ntop(AF_INET, &client_addr.sin_addr, host_str, sizeof(host_str));
    int port = ntohs(client_addr.sin_port);
    
    TauList* addr_tuple = (TauList*)malloc(sizeof(TauList));
    addr_tuple->size = 2;
    addr_tuple->capacity = 4;
    addr_tuple->items = (TauValue*)malloc(sizeof(TauValue) * addr_tuple->capacity);
    addr_tuple->items[0] = (TauValue){.type = 2, .value.s = strdup(host_str), .refcount = 1, .next = NULL};
    addr_tuple->items[1] = (TauValue){.type = 0, .value.i = port, .refcount = 1, .next = NULL};
    
    result->items[1] = (TauValue){.type = 4, .value.list = addr_tuple, .refcount = 1, .next = NULL};
    
    return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
}

// socket.connect(sockfd, address)
static inline TauValue tauraro_socket_connect(TauValue sockval, TauValue address) {
    if (sockval.type != 6 || address.type != 4 || !address.value.list)
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    
    SocketWrapper* sock = (SocketWrapper*)sockval.value.ptr;
    TauValue host_val = address.value.list->items[0];
    TauValue port_val = address.value.list->items[1];
    
    const char* host = host_val.type == 2 ? host_val.value.s : "127.0.0.1";
    int port = port_val.type == 0 ? port_val.value.i : 0;
    
    struct sockaddr_in addr;
    addr.sin_family = TAURARO_SOCKET_AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);
    
    if (connect(sock->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == SOCK_ERROR) {
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    }
    
    sock->is_connected = 1;
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// socket.send(sockfd, data)
static inline TauValue tauraro_socket_send(TauValue sockval, TauValue data) {
    if (sockval.type != 6 || data.type != 2)
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    
    SocketWrapper* sock = (SocketWrapper*)sockval.value.ptr;
    const char* buf = data.value.s;
    int len = strlen(buf);
    
    #ifdef _WIN32
        int sent = send(sock->socket_fd, buf, len, 0);
    #else
        ssize_t sent = send(sock->socket_fd, buf, len, 0);
    #endif
    
    return (TauValue){.type = 0, .value.i = sent > 0 ? sent : -1, .refcount = 1, .next = NULL};
}

// socket.recv(sockfd, bufsize) - returns received string
static inline TauValue tauraro_socket_recv(TauValue sockval, TauValue bufsize) {
    if (sockval.type != 6)
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    
    SocketWrapper* sock = (SocketWrapper*)sockval.value.ptr;
    int size = bufsize.type == 0 ? bufsize.value.i : 1024;
    if (size > 65536) size = 65536;  // Max 64KB
    
    char* buf = (char*)malloc(size + 1);
    
    #ifdef _WIN32
        int recvd = recv(sock->socket_fd, buf, size, 0);
    #else
        ssize_t recvd = recv(sock->socket_fd, buf, size, 0);
    #endif
    
    if (recvd <= 0) {
        free(buf);
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    buf[recvd] = '\0';
    return (TauValue){.type = 2, .value.s = buf, .refcount = 1, .next = NULL};
}

// socket.close(sockfd)
static inline TauValue tauraro_socket_close(TauValue sockval) {
    if (sockval.type != 6)
        return (TauValue){.type = 3, .value.i = -1, .refcount = 1, .next = NULL};
    
    SocketWrapper* sock = (SocketWrapper*)sockval.value.ptr;
    SOCK_CLOSE(sock->socket_fd);
    free(sock);
    
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// socket.gethostbyname(hostname)
static inline TauValue tauraro_socket_gethostbyname(TauValue hostname) {
    if (hostname.type != 2) return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    
    struct hostent *he = gethostbyname(hostname.value.s);
    if (he == NULL) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    struct in_addr **addr_list = (struct in_addr**)he->h_addr_list;
    char* ip = inet_ntoa(*addr_list[0]);
    
    return (TauValue){.type = 2, .value.s = strdup(ip), .refcount = 1, .next = NULL};
}

// socket.gethostname()
static inline TauValue tauraro_socket_gethostname(void) {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }
    
    return (TauValue){.type = 2, .value.s = strdup(hostname), .refcount = 1, .next = NULL};
}

// socket.inet_aton(ip_string) - Convert IP string to packed address
static inline TauValue tauraro_socket_inet_aton(TauValue ip_string) {
    if (ip_string.type != 2) 
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    struct in_addr addr;
    if (inet_aton(ip_string.value.s, &addr) == 0) {
        return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    }
    
    return (TauValue){.type = 0, .value.i = (long long)addr.s_addr, .refcount = 1, .next = NULL};
}
    struct in_addr addr;
    addr.s_addr = packed_ip.type == 0 ? packed_ip.value.i : 0;
    
    char* ip_str = inet_ntoa(addr);
    char* result = (char*)malloc(strlen(ip_str) + 1);
    strcpy(result, ip_str);
    return (TauValue){.type = 2, .value.s = result, .refcount = 1, .next = NULL};
}

// socket.socketpair(family=AF_UNIX, type=SOCK_STREAM, proto=0)
static inline TauValue tauraro_socket_socketpair(void) {
    // Return tuple of two connected sockets
    return (TauValue){.type = 4, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// socket.getservbyname(servicename)
static inline TauValue tauraro_socket_getservbyname(TauValue service) {
    if (service.type != 2) return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    
    struct servent* se = getservbyname(service.value.s, "tcp");
    if (se == NULL) return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    
    return (TauValue){.type = 0, .value.i = ntohs(se->s_port), .refcount = 1, .next = NULL};
}

// socket.AF_INET = 2
static inline TauValue tauraro_socket_AF_INET(void) {
    return (TauValue){.type = 0, .value.i = 2, .refcount = 1, .next = NULL};
}

// socket.AF_INET6 = 10
static inline TauValue tauraro_socket_AF_INET6(void) {
    return (TauValue){.type = 0, .value.i = 10, .refcount = 1, .next = NULL};
}

// socket.SOCK_STREAM = 1
static inline TauValue tauraro_socket_SOCK_STREAM(void) {
    return (TauValue){.type = 0, .value.i = 1, .refcount = 1, .next = NULL};
}

// socket.SOCK_DGRAM = 2
static inline TauValue tauraro_socket_SOCK_DGRAM(void) {
    return (TauValue){.type = 0, .value.i = 2, .refcount = 1, .next = NULL};
}

// socket.SOL_SOCKET = 1
static inline TauValue tauraro_socket_SOL_SOCKET(void) {
    return (TauValue){.type = 0, .value.i = 1, .refcount = 1, .next = NULL};
}

// socket.SO_REUSEADDR = 2
static inline TauValue tauraro_socket_SO_REUSEADDR(void) {
    return (TauValue){.type = 0, .value.i = 2, .refcount = 1, .next = NULL};
}

// socket.SOMAXCONN = 128
static inline TauValue tauraro_socket_SOMAXCONN(void) {
    return (TauValue){.type = 0, .value.i = 128, .refcount = 1, .next = NULL};
}


#endif // TAURARO_SOCKET_MODULE_H
