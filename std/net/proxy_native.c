/*
 * std/net/proxy_native.c -- socket helpers for std.net.proxy: a TCP connect
 * with a timeout (IPv4 and IPv6 via getaddrinfo) and a readiness wait.
 *
 * Linked through `# tauraro-cpp-shim: proxy_native.c` in std/net/proxy.tr.
 * The returned descriptors are ordinary blocking sockets, interchangeable
 * with the runtime's _tr_tcp_* calls (an int on POSIX, a SOCKET cast to int
 * on Windows, as the runtime does).
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
typedef SOCKET px_sock;
#  define PX_BAD INVALID_SOCKET
#  define px_close closesocket
static void px_wsa(void) { static int done = 0; if (!done) { WSADATA w; WSAStartup(MAKEWORD(2, 2), &w); done = 1; } }
static int px_poll(px_sock s, int events, int ms) {
    WSAPOLLFD p; p.fd = s; p.events = (short)events; p.revents = 0;
    return WSAPoll(&p, 1, ms);
}
#  define PX_IN POLLRDNORM
#  define PX_OUT POLLWRNORM
static int px_set_nb(px_sock s, int on) { u_long m = on ? 1 : 0; return ioctlsocket(s, FIONBIO, &m); }
static int px_in_progress(void) { int e = WSAGetLastError(); return e == WSAEWOULDBLOCK || e == WSAEINPROGRESS; }
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <poll.h>
#  include <errno.h>
typedef int px_sock;
#  define PX_BAD (-1)
#  define px_close close
static void px_wsa(void) {}
static int px_poll(px_sock s, int events, int ms) {
    struct pollfd p; int r;
    p.fd = s; p.events = (short)events; p.revents = 0;
    do { r = poll(&p, 1, ms); } while (r < 0 && errno == EINTR);
    return r;
}
#  define PX_IN POLLIN
#  define PX_OUT POLLOUT
static int px_set_nb(px_sock s, int on) {
    int fl = fcntl(s, F_GETFL, 0);
    if (fl < 0) return -1;
    return fcntl(s, F_SETFL, on ? (fl | O_NONBLOCK) : (fl & ~O_NONBLOCK));
}
static int px_in_progress(void) { return errno == EINPROGRESS || errno == EWOULDBLOCK; }
#endif

/* Connect to host:port within timeout_ms (<= 0: 30 s). Tries every address
 * getaddrinfo returns. Returns a blocking socket, -1 if the name does not
 * resolve, -2 if every address refused / failed, -3 on timeout. */
int64_t trc_px_connect(const char* host, int64_t hostlen, int64_t port, int64_t timeout_ms) {
    char name[256], pbuf[16]; struct addrinfo hints, *res = NULL, *ai; int64_t rc = -2;
    if (hostlen <= 0 || hostlen >= (int64_t)sizeof name) return -1;
    memcpy(name, host, (size_t)hostlen); name[hostlen] = 0;
    if (name[0] == '[' && hostlen > 2 && name[hostlen - 1] == ']') {   /* [v6] */
        memmove(name, name + 1, (size_t)hostlen - 2); name[hostlen - 2] = 0;
    }
    if (timeout_ms <= 0) timeout_ms = 30000;
    px_wsa();
    snprintf(pbuf, sizeof pbuf, "%d", (int)port);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(name, pbuf, &hints, &res) != 0 || !res) return -1;
    for (ai = res; ai; ai = ai->ai_next) {
        px_sock s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol); int r;
        if (s == PX_BAD) continue;
        px_set_nb(s, 1);
        r = connect(s, ai->ai_addr, (int)ai->ai_addrlen);
        if (r != 0 && px_in_progress()) {
            int w = px_poll(s, PX_OUT, (int)timeout_ms);
            if (w == 0) { px_close(s); rc = -3; continue; }
            if (w > 0) {
                int err = 0; socklen_t el = (socklen_t)sizeof err;
                getsockopt(s, SOL_SOCKET, SO_ERROR, (char*)&err, &el);
                r = err == 0 ? 0 : -1;
            } else r = -1;
        }
        if (r == 0) {
            int one = 1;
            px_set_nb(s, 0);
            setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, (socklen_t)sizeof one);
            freeaddrinfo(res);
            return (int64_t)s;
        }
        px_close(s);
    }
    freeaddrinfo(res);
    return rc;
}

/* Wait until fd is readable: 1 ready, 0 timeout, -1 error. ms <= 0 waits forever. */
int64_t trc_px_wait_readable(int64_t fd, int64_t ms) {
    int r = px_poll((px_sock)fd, PX_IN, ms <= 0 ? -1 : (int)ms);
    return r > 0 ? 1 : (r == 0 ? 0 : -1);
}

/* recv into buf: bytes read, 0 at EOF, -1 on error. */
int64_t trc_px_recv(int64_t fd, char* buf, int64_t cap) {
    int r = (int)recv((px_sock)fd, buf, (int)cap, 0);
    return r < 0 ? -1 : r;
}

/* send all n bytes: 0 on success, -1 on error. */
int64_t trc_px_send(int64_t fd, const char* buf, int64_t n) {
    int64_t off = 0;
    while (off < n) {
#if defined(_WIN32)
        int r = send((px_sock)fd, buf + off, (int)(n - off), 0);
#else
#  ifdef MSG_NOSIGNAL
        ssize_t r = send((px_sock)fd, buf + off, (size_t)(n - off), MSG_NOSIGNAL);
#  else
        ssize_t r = send((px_sock)fd, buf + off, (size_t)(n - off), 0);
#  endif
#endif
        if (r <= 0) return -1;
        off += r;
    }
    return 0;
}

void trc_px_close(int64_t fd) { if (fd >= 0) px_close((px_sock)fd); }
