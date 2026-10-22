/*
 * std/net/mqtt_native.c -- socket readiness helpers for std.net.mqtt:
 * wait on several sockets at once (the embedded broker's event loop) and a
 * monotonic millisecond clock (keep-alive and reconnect timing).
 *
 * Linked through `# tauraro-cpp-shim: mqtt_native.c` in std/net/mqtt.tr.
 */
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>

#if defined(_WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <windows.h>
#else
#  include <poll.h>
#  include <errno.h>
#  include <time.h>
#  include <stdlib.h>
#  if defined(__APPLE__)
#    include <mach/mach_time.h>
#  endif
#endif

/* Monotonic milliseconds (arbitrary origin). */
int64_t trc_mqtt_now_ms(void) {
#if defined(_WIN32)
    return (int64_t)GetTickCount64();
#elif defined(__APPLE__)
    static mach_timebase_info_data_t tb;
    if (tb.denom == 0) mach_timebase_info(&tb);
    return (int64_t)((mach_absolute_time() * tb.numer / tb.denom) / 1000000ULL);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}

/*
 * Wait until at least one of fds[0..n) is readable (or hung up / in error),
 * for at most timeout_ms (-1 = forever). ready[i] is set to 1 for each such
 * socket, 0 otherwise. Returns the number of ready sockets, 0 on timeout,
 * -1 on error. Negative fds are skipped.
 */
int64_t trc_mqtt_poll(const int64_t* fds, int64_t n, int64_t timeout_ms, int64_t* ready) {
    int64_t i, count = 0;
    if (n <= 0) return 0;
    for (i = 0; i < n; i++) ready[i] = 0;
#if defined(_WIN32)
    {
        WSAPOLLFD* p = (WSAPOLLFD*)calloc((size_t)n, sizeof(WSAPOLLFD));
        int r;
        if (!p) return -1;
        for (i = 0; i < n; i++) { p[i].fd = fds[i] < 0 ? INVALID_SOCKET : (SOCKET)fds[i]; p[i].events = POLLRDNORM; }
        r = WSAPoll(p, (ULONG)n, (INT)timeout_ms);
        if (r < 0) { free(p); return -1; }
        for (i = 0; i < n; i++) if (fds[i] >= 0 && (p[i].revents & (POLLRDNORM | POLLHUP | POLLERR | POLLNVAL))) { ready[i] = 1; count++; }
        free(p);
    }
#else
    {
        struct pollfd* p = (struct pollfd*)calloc((size_t)n, sizeof(struct pollfd));
        int r;
        if (!p) return -1;
        for (i = 0; i < n; i++) { p[i].fd = fds[i] < 0 ? -1 : (int)fds[i]; p[i].events = POLLIN; p[i].revents = 0; }
        do { r = poll(p, (nfds_t)n, (int)timeout_ms); } while (r < 0 && errno == EINTR);
        if (r < 0) { free(p); return -1; }
        for (i = 0; i < n; i++) if (fds[i] >= 0 && (p[i].revents & (POLLIN | POLLHUP | POLLERR | POLLNVAL))) { ready[i] = 1; count++; }
        free(p);
    }
#endif
    return count;
}

/* 1 when fd is readable within timeout_ms, 0 on timeout, -1 on error. */
int64_t trc_mqtt_readable(int64_t fd, int64_t timeout_ms) {
    int64_t f = fd, r = 0;
    int64_t got = trc_mqtt_poll(&f, 1, timeout_ms, &r);
    if (got < 0) return -1;
    return r ? 1 : 0;
}
