/*
 * std/net/udpx_native.c -- UDP with timeouts, IPv6 and multicast, plus a
 * wall clock in milliseconds, for std.net.{ntp,mdns,dnsmsg}.
 *
 * Linked through `# tauraro-cpp-shim: udpx_native.c` in std/net/udpx.tr.
 */
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <windows.h>
typedef SOCKET ux_sock;
#  define UX_BAD INVALID_SOCKET
#  define ux_close closesocket
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <unistd.h>
#  include <poll.h>
#  include <errno.h>
typedef int ux_sock;
#  define UX_BAD (-1)
#  define ux_close close
#endif

#if defined(_WIN32)
static void ux_wsa(void) { static int done = 0; if (!done) { WSADATA w; WSAStartup(MAKEWORD(2, 2), &w); done = 1; } }
#else
static void ux_wsa(void) {}
#endif

/* Milliseconds since the Unix epoch (wall clock, with sub-second precision). */
int64_t trc_udpx_unix_ms(void) {
#if defined(_WIN32)
    FILETIME ft; ULARGE_INTEGER u;
    GetSystemTimeAsFileTime(&ft);
    u.LowPart = ft.dwLowDateTime; u.HighPart = ft.dwHighDateTime;
    return (int64_t)((u.QuadPart - 116444736000000000ULL) / 10000ULL);
#else
    struct timeval tv; gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

/* family: 4 or 6. bind_port 0 = ephemeral. reuse: SO_REUSEADDR (+ SO_REUSEPORT). */
int64_t trc_udpx_open(int64_t family, int64_t bind_port, int64_t reuse) {
    ux_sock s; int one = 1;
    ux_wsa();
    s = socket(family == 6 ? AF_INET6 : AF_INET, SOCK_DGRAM, 0);
    if (s == UX_BAD) return -1;
    if (reuse) {
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof one);
#if defined(SO_REUSEPORT)
        setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (const char*)&one, sizeof one);
#endif
    }
    if (family == 6) {
        struct sockaddr_in6 a; memset(&a, 0, sizeof a);
        a.sin6_family = AF_INET6; a.sin6_port = htons((uint16_t)bind_port); a.sin6_addr = in6addr_any;
        if (bind(s, (struct sockaddr*)&a, sizeof a) < 0) { ux_close(s); return -1; }
    } else {
        struct sockaddr_in a; memset(&a, 0, sizeof a);
        a.sin_family = AF_INET; a.sin_port = htons((uint16_t)bind_port); a.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(s, (struct sockaddr*)&a, sizeof a) < 0) { ux_close(s); return -1; }
    }
    return (int64_t)s;
}

/* Join an IPv4 multicast group (on the default interface) and set TTL / loopback. */
int64_t trc_udpx_join4(int64_t fd, const char* group, int64_t ttl, int64_t loop) {
    struct ip_mreq m; unsigned char t = (unsigned char)ttl, l = (unsigned char)(loop ? 1 : 0);
    memset(&m, 0, sizeof m);
    if (inet_pton(AF_INET, group, &m.imr_multiaddr) != 1) return -1;
    m.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt((ux_sock)fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&m, sizeof m) < 0) return -1;
    setsockopt((ux_sock)fd, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&t, sizeof t);
    setsockopt((ux_sock)fd, IPPROTO_IP, IP_MULTICAST_LOOP, (const char*)&l, sizeof l);
    return 0;
}
int64_t trc_udpx_set_multicast_ttl(int64_t fd, int64_t ttl) {
    unsigned char t = (unsigned char)ttl;
    return setsockopt((ux_sock)fd, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&t, sizeof t) < 0 ? -1 : 0;
}

/* Send to host:port (name or literal). Returns bytes sent or -1. */
int64_t trc_udpx_sendto(int64_t fd, const char* data, int64_t n, const char* host, int64_t port) {
    struct addrinfo hints, *res = NULL, *ai; char ps[16]; int64_t r = -1;
    struct sockaddr_storage local; socklen_t ll = sizeof local; int fam = AF_UNSPEC;
    ux_wsa();
    if (getsockname((ux_sock)fd, (struct sockaddr*)&local, &ll) == 0) fam = local.ss_family;
    snprintf(ps, sizeof ps, "%d", (int)port);
    memset(&hints, 0, sizeof hints); hints.ai_family = fam; hints.ai_socktype = SOCK_DGRAM;
    if (getaddrinfo(host, ps, &hints, &res) != 0) return -1;
    for (ai = res; ai; ai = ai->ai_next) {
        long s = (long)sendto((ux_sock)fd, data, (int)n, 0, ai->ai_addr, (int)ai->ai_addrlen);
        if (s >= 0) { r = s; break; }
    }
    freeaddrinfo(res);
    return r;
}

/* Receive one datagram. Returns its length, -2 on timeout, -1 on error. The
 * sender's address goes to addr_out (NUL-terminated, 64 bytes) and port_out. */
int64_t trc_udpx_recvfrom(int64_t fd, char* buf, int64_t cap, int64_t timeout_ms, char* addr_out, int64_t* port_out) {
    struct sockaddr_storage from; socklen_t fl = sizeof from; long r;
    if (timeout_ms >= 0) {
#if defined(_WIN32)
        fd_set rs; struct timeval tv; FD_ZERO(&rs); FD_SET((SOCKET)fd, &rs);
        tv.tv_sec = (long)(timeout_ms / 1000); tv.tv_usec = (long)((timeout_ms % 1000) * 1000);
        if (select(0, &rs, NULL, NULL, &tv) == 0) return -2;
#else
        struct pollfd p; int pr; p.fd = (int)fd; p.events = POLLIN; p.revents = 0;
        do { pr = poll(&p, 1, (int)timeout_ms); } while (pr < 0 && errno == EINTR);
        if (pr == 0) return -2;
        if (pr < 0) return -1;
#endif
    }
    r = (long)recvfrom((ux_sock)fd, buf, (int)cap, 0, (struct sockaddr*)&from, &fl);
    if (r < 0) return -1;
    addr_out[0] = 0; *port_out = 0;
    if (from.ss_family == AF_INET) {
        struct sockaddr_in* a = (struct sockaddr_in*)&from;
        inet_ntop(AF_INET, &a->sin_addr, addr_out, 64); *port_out = ntohs(a->sin_port);
    } else if (from.ss_family == AF_INET6) {
        struct sockaddr_in6* a = (struct sockaddr_in6*)&from;
        inet_ntop(AF_INET6, &a->sin6_addr, addr_out, 64); *port_out = ntohs(a->sin6_port);
    }
    return r;
}
/* Local port of a bound socket. */
int64_t trc_udpx_local_port(int64_t fd) {
    struct sockaddr_storage a; socklen_t l = sizeof a;
    if (getsockname((ux_sock)fd, (struct sockaddr*)&a, &l) < 0) return -1;
    if (a.ss_family == AF_INET) return ntohs(((struct sockaddr_in*)&a)->sin_port);
    return ntohs(((struct sockaddr_in6*)&a)->sin6_port);
}
void trc_udpx_close(int64_t fd) { ux_close((ux_sock)fd); }

/* Generic stream-socket helpers (used by std.net.linestream for plain TCP). */
int64_t trc_net_recv(int64_t fd, char* buf, int64_t cap, int64_t timeout_ms) {
    long r;
    if (timeout_ms >= 0) {
#if defined(_WIN32)
        fd_set rs; struct timeval tv; FD_ZERO(&rs); FD_SET((SOCKET)fd, &rs);
        tv.tv_sec = (long)(timeout_ms / 1000); tv.tv_usec = (long)((timeout_ms % 1000) * 1000);
        if (select(0, &rs, NULL, NULL, &tv) == 0) return -2;
#else
        struct pollfd p; int pr; p.fd = (int)fd; p.events = POLLIN; p.revents = 0;
        do { pr = poll(&p, 1, (int)timeout_ms); } while (pr < 0 && errno == EINTR);
        if (pr == 0) return -2;
        if (pr < 0) return -1;
#endif
    }
    for (;;) {
        r = (long)recv((ux_sock)fd, buf, (int)cap, 0);
#if !defined(_WIN32)
        if (r < 0 && errno == EINTR) continue;
        if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            struct pollfd p; p.fd = (int)fd; p.events = POLLIN; p.revents = 0;
            if (poll(&p, 1, timeout_ms >= 0 ? (int)timeout_ms : -1) == 0) return -2;
            continue;
        }
#endif
        break;
    }
    return r < 0 ? -1 : r;
}
int64_t trc_net_send(int64_t fd, const char* d, int64_t n) {
    int64_t done = 0;
    while (done < n) {
        long r;
#if defined(MSG_NOSIGNAL)
        r = (long)send((ux_sock)fd, d + done, (int)(n - done), MSG_NOSIGNAL);
#else
        r = (long)send((ux_sock)fd, d + done, (int)(n - done), 0);
#endif
        if (r < 0) {
#if !defined(_WIN32)
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) { struct pollfd p; p.fd = (int)fd; p.events = POLLOUT; p.revents = 0; poll(&p, 1, 1000); continue; }
#endif
            return -1;
        }
        done += r;
    }
    return done;
}
