/*
 * std/kv/kv_native.c -- file primitives for std.kv: positional I/O, fsync,
 * truncate, atomic replace-rename, an exclusive advisory lock file and CRC32.
 *
 * Linked through `# tauraro-cpp-shim: kv_native.c` in std/kv/native.tr.
 * POSIX (Linux, macOS, BSD) and Windows implementations. Every function is
 * flat C with int64 / pointer arguments; failures are negative values.
 */
#if !defined(_WIN32) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#if defined(_WIN32)
#  include <windows.h>
#  include <io.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <sys/file.h>
#endif

/* Open (creating if needed) for read+write, binary. Returns an fd or -1. */
int64_t trc_kv_open(const char* path, int64_t create) {
#if defined(_WIN32)
    int flags = _O_RDWR | _O_BINARY | (create ? _O_CREAT : 0);
    return (int64_t)_open(path, flags, _S_IREAD | _S_IWRITE);
#else
    int flags = O_RDWR | (create ? O_CREAT : 0);
#  ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#  endif
    return (int64_t)open(path, flags, 0644);
#endif
}

int64_t trc_kv_close(int64_t fd) {
#if defined(_WIN32)
    return (int64_t)_close((int)fd);
#else
    return (int64_t)close((int)fd);
#endif
}

int64_t trc_kv_size(int64_t fd) {
#if defined(_WIN32)
    return (int64_t)_filelengthi64((int)fd);
#else
    struct stat st;
    if (fstat((int)fd, &st) != 0) return -1;
    return (int64_t)st.st_size;
#endif
}

/* Read up to n bytes at off; returns the byte count (0 at EOF) or -1. */
int64_t trc_kv_pread(int64_t fd, int64_t off, char* buf, int64_t n) {
    int64_t done = 0;
#if defined(_WIN32)
    if (_lseeki64((int)fd, off, SEEK_SET) < 0) return -1;
    while (done < n) {
        int r = _read((int)fd, buf + done, (unsigned)(n - done > 1 << 30 ? 1 << 30 : n - done));
        if (r < 0) return -1;
        if (r == 0) break;
        done += r;
    }
#else
    while (done < n) {
        ssize_t r = pread((int)fd, buf + done, (size_t)(n - done), (off_t)(off + done));
        if (r < 0) { if (errno == EINTR) continue; return -1; }
        if (r == 0) break;
        done += r;
    }
#endif
    return done;
}

/* Write exactly n bytes at off; returns n or -1. */
int64_t trc_kv_pwrite(int64_t fd, int64_t off, const char* buf, int64_t n) {
    int64_t done = 0;
#if defined(_WIN32)
    if (_lseeki64((int)fd, off, SEEK_SET) < 0) return -1;
    while (done < n) {
        int r = _write((int)fd, buf + done, (unsigned)(n - done > 1 << 30 ? 1 << 30 : n - done));
        if (r <= 0) return -1;
        done += r;
    }
#else
    while (done < n) {
        ssize_t r = pwrite((int)fd, buf + done, (size_t)(n - done), (off_t)(off + done));
        if (r < 0) { if (errno == EINTR) continue; return -1; }
        if (r == 0) return -1;
        done += r;
    }
#endif
    return done;
}

int64_t trc_kv_sync(int64_t fd) {
#if defined(_WIN32)
    return (int64_t)_commit((int)fd);
#elif defined(__APPLE__)
    return (int64_t)(fcntl((int)fd, F_FULLFSYNC) == 0 ? 0 : fsync((int)fd));
#else
    return (int64_t)fsync((int)fd);
#endif
}

int64_t trc_kv_truncate(int64_t fd, int64_t size) {
#if defined(_WIN32)
    return (int64_t)_chsize_s((int)fd, size);
#else
    return (int64_t)ftruncate((int)fd, (off_t)size);
#endif
}

/* Atomically replace `to` with `from`. Returns 0 or -1. */
int64_t trc_kv_replace(const char* from, const char* to) {
#if defined(_WIN32)
    return MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) ? 0 : -1;
#else
    return (int64_t)rename(from, to);
#endif
}

int64_t trc_kv_unlink(const char* path) {
    return (int64_t)remove(path);
}

int64_t trc_kv_exists(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    fclose(f);
    return 1;
}

/* Flush the directory entry after a rename (no-op where unsupported). */
int64_t trc_kv_sync_dir(const char* path) {
#if defined(_WIN32)
    (void)path;
    return 0;
#else
    char buf[4096];
    size_t n = strlen(path), i;
    int fd;
    if (n >= sizeof buf) return -1;
    memcpy(buf, path, n + 1);
    for (i = n; i > 0 && buf[i - 1] != '/'; --i) {}
    if (i == 0) { buf[0] = '.'; buf[1] = 0; }
    else if (i == 1) buf[1] = 0;
    else buf[i - 1] = 0;
    fd = open(buf, O_RDONLY);
    if (fd < 0) return -1;
    (void)fsync(fd);
    close(fd);
    return 0;
#endif
}

/* Exclusive non-blocking lock on a lock file (created if missing). Returns an
   opaque handle (an fd, >= 0), -2 if somebody else holds it, -1 on error. */
int64_t trc_kv_lock(const char* path) {
#if defined(_WIN32)
    HANDLE h = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                           NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    OVERLAPPED ov;
    if (h == INVALID_HANDLE_VALUE) return -1;
    memset(&ov, 0, sizeof ov);
    if (!LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 1, 0, &ov)) {
        CloseHandle(h);
        return -2;
    }
    return (int64_t)(intptr_t)h;
#else
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    if (fd < 0) return -1;
#  ifdef FD_CLOEXEC
    (void)fcntl(fd, F_SETFD, FD_CLOEXEC);
#  endif
    if (flock(fd, LOCK_EX | LOCK_NB) != 0) {
        int busy = (errno == EWOULDBLOCK || errno == EAGAIN);
        close(fd);
        return busy ? -2 : -1;
    }
    return (int64_t)fd;
#endif
}

void trc_kv_unlock(int64_t h) {
#if defined(_WIN32)
    OVERLAPPED ov;
    memset(&ov, 0, sizeof ov);
    UnlockFileEx((HANDLE)(intptr_t)h, 0, 1, 0, &ov);
    CloseHandle((HANDLE)(intptr_t)h);
#else
    flock((int)h, LOCK_UN);
    close((int)h);
#endif
}

int64_t trc_kv_now_ms(void) {
#if defined(_WIN32)
    FILETIME ft;
    ULARGE_INTEGER u;
    GetSystemTimeAsFileTime(&ft);
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return (int64_t)((u.QuadPart - 116444736000000000ULL) / 10000ULL);
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)ts.tv_sec * 1000 + (int64_t)(ts.tv_nsec / 1000000);
#endif
}

/* Monotonic-ish nanoseconds, used with the wall clock to seed store ids. */
int64_t trc_kv_nanos(void) {
#if defined(_WIN32)
    LARGE_INTEGER c, f;
    QueryPerformanceCounter(&c);
    QueryPerformanceFrequency(&f);
    return (int64_t)((double)c.QuadPart * 1e9 / (double)f.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec;
#endif
}

/* CRC-32 (IEEE, reflected), continuing from `crc` (pass 0 to start). */
static uint32_t crc_table[256];
static int crc_ready = 0;
static void crc_init(void) {
    uint32_t i, j, c;
    for (i = 0; i < 256; ++i) {
        c = i;
        for (j = 0; j < 8; ++j) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        crc_table[i] = c;
    }
    crc_ready = 1;
}
int64_t trc_kv_crc32(int64_t crc, const char* buf, int64_t n) {
    uint32_t c = ~(uint32_t)crc;
    int64_t i;
    if (!crc_ready) crc_init();
    for (i = 0; i < n; ++i) c = crc_table[(c ^ (uint8_t)buf[i]) & 0xFF] ^ (c >> 8);
    return (int64_t)(~c);
}
