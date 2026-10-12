/*
 * std/sys/ipc_native.c -- memory maps, shared memory, semaphores, pipes and
 * Unix-domain sockets for std.sys.{mmap,shm,pipe,unix_socket}.
 *
 * Linked through `# tauraro-cpp-shim: ipc_native.c` in std/sys/ipc.tr.
 * POSIX (Linux, macOS, BSD) and Windows implementations; where Windows has
 * no equivalent (FIFOs, fd passing) the call fails with a clear error code.
 *
 * Conventions: handles are opaque pointers (maps, semaphores) or integer
 * descriptors (pipes, sockets); errors are negative return values and the
 * thread's last OS error text is available through trc_ipc_last_error.
 */
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#if defined(_WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  include <windows.h>
#  include <io.h>
#  include <fcntl.h>
#  ifndef UNIX_PATH_MAX
#    define UNIX_PATH_MAX 108
     typedef struct { ADDRESS_FAMILY sun_family; char sun_path[UNIX_PATH_MAX]; } trc_sockaddr_un;
#  endif
#  ifndef AF_UNIX
#    define AF_UNIX 1
#  endif
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <sys/mman.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <sys/un.h>
#  include <semaphore.h>
#  include <poll.h>
#  include <time.h>
typedef struct sockaddr_un trc_sockaddr_un;
#endif

/* ── last error ── */
#if defined(_MSC_VER)
static __declspec(thread) char ipc_err[256];
#else
static __thread char ipc_err[256];
#endif
static int64_t ipc_fail(const char* what) {
#if defined(_WIN32)
    DWORD e = GetLastError(); char msg[200] = "";
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, 0, msg, sizeof msg, NULL);
    { size_t l = strlen(msg); while (l && (msg[l - 1] == '\n' || msg[l - 1] == '\r' || msg[l - 1] == ' ')) msg[--l] = 0; }
    snprintf(ipc_err, sizeof ipc_err, "%s: %s", what, msg[0] ? msg : "error");
#else
    snprintf(ipc_err, sizeof ipc_err, "%s: %s", what, strerror(errno));
#endif
    return -1;
}
static int64_t ipc_fail_msg(const char* msg) { snprintf(ipc_err, sizeof ipc_err, "%s", msg); return -1; }
int64_t trc_ipc_last_error(char* out, int64_t cap) {
    size_t n = strlen(ipc_err); if ((int64_t)n > cap) n = (size_t)cap;
    memcpy(out, ipc_err, n); return (int64_t)n;
}

/* ═══════════════════════════ memory maps ═════════════════════════════════
 * One struct serves file maps, anonymous maps and shared-memory segments. */
typedef struct {
    unsigned char* p; int64_t len;
    int writable;
#if defined(_WIN32)
    HANDLE file, mapping;
#else
    int fd;
#endif
} ipc_map;

static ipc_map* map_new(void) {
    ipc_map* m = (ipc_map*)calloc(1, sizeof(ipc_map));
#if defined(_WIN32)
    if (m) { m->file = INVALID_HANDLE_VALUE; m->mapping = NULL; }
#else
    if (m) m->fd = -1;
#endif
    return m;
}

/* mode: 0 read-only, 1 read-write (shared with the file), 2 copy-on-write
 * (private). length 0 = to the end of the file. Returns NULL on error. */
void* trc_mmap_file(const char* path, int64_t mode, int64_t offset, int64_t length) {
    ipc_map* m = map_new();
    if (!m) { ipc_fail_msg("mmap: out of memory"); return NULL; }
    m->writable = mode != 0;
#if defined(_WIN32)
    {
        LARGE_INTEGER sz; DWORD acc = mode == 1 ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;
        DWORD prot = mode == 0 ? PAGE_READONLY : mode == 1 ? PAGE_READWRITE : PAGE_WRITECOPY;
        DWORD view = mode == 0 ? FILE_MAP_READ : mode == 1 ? FILE_MAP_WRITE : FILE_MAP_COPY;
        SYSTEM_INFO si; int64_t align, delta;
        m->file = CreateFileA(path, acc, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m->file == INVALID_HANDLE_VALUE) { ipc_fail("mmap: open"); free(m); return NULL; }
        if (!GetFileSizeEx(m->file, &sz)) { ipc_fail("mmap: size"); CloseHandle(m->file); free(m); return NULL; }
        if (length == 0) length = sz.QuadPart - offset;
        if (length <= 0 || offset < 0 || offset + length > sz.QuadPart) { ipc_fail_msg("mmap: range outside the file (or empty file)"); CloseHandle(m->file); free(m); return NULL; }
        m->mapping = CreateFileMappingA(m->file, NULL, prot, 0, 0, NULL);
        if (!m->mapping) { ipc_fail("mmap: CreateFileMapping"); CloseHandle(m->file); free(m); return NULL; }
        GetSystemInfo(&si); align = si.dwAllocationGranularity; delta = offset % align;
        m->p = (unsigned char*)MapViewOfFile(m->mapping, view, (DWORD)((offset - delta) >> 32), (DWORD)(offset - delta), (SIZE_T)(length + delta));
        if (!m->p) { ipc_fail("mmap: MapViewOfFile"); CloseHandle(m->mapping); CloseHandle(m->file); free(m); return NULL; }
        m->p += delta; m->len = length;
        /* remember delta in the high part of writable for unmap */
        m->writable |= (int)(delta << 2);
    }
#else
    {
        struct stat st; long pg = sysconf(_SC_PAGESIZE); int64_t delta; void* p;
        m->fd = open(path, mode == 1 ? O_RDWR : O_RDONLY);
        if (m->fd < 0) { ipc_fail("mmap: open"); free(m); return NULL; }
        if (fstat(m->fd, &st) < 0) { ipc_fail("mmap: stat"); close(m->fd); free(m); return NULL; }
        if (length == 0) length = (int64_t)st.st_size - offset;
        if (length <= 0 || offset < 0 || offset + length > (int64_t)st.st_size) { ipc_fail_msg("mmap: range outside the file (or empty file)"); close(m->fd); free(m); return NULL; }
        delta = offset % pg;
        p = mmap(NULL, (size_t)(length + delta), mode == 0 ? PROT_READ : (PROT_READ | PROT_WRITE),
                 mode == 2 ? MAP_PRIVATE : MAP_SHARED, m->fd, (off_t)(offset - delta));
        if (p == MAP_FAILED) { ipc_fail("mmap"); close(m->fd); free(m); return NULL; }
        m->p = (unsigned char*)p + delta; m->len = length;
        m->writable |= (int)(delta << 2);
    }
#endif
    return m;
}

/* Private anonymous memory (zero-filled). */
void* trc_mmap_anon(int64_t size) {
    ipc_map* m;
    if (size <= 0) { ipc_fail_msg("mmap: size must be positive"); return NULL; }
    m = map_new();
    if (!m) return NULL;
#if defined(_WIN32)
    m->p = (unsigned char*)VirtualAlloc(NULL, (SIZE_T)size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!m->p) { ipc_fail("mmap: VirtualAlloc"); free(m); return NULL; }
    m->writable = 1 | (1 << 30);   /* flag: VirtualAlloc'd */
#else
    {
        void* p = mmap(NULL, (size_t)size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (p == MAP_FAILED) { ipc_fail("mmap"); free(m); return NULL; }
        m->p = (unsigned char*)p; m->writable = 1;
    }
#endif
    m->len = size;
    return m;
}

int64_t trc_map_len(void* h) { return h ? ((ipc_map*)h)->len : 0; }
int64_t trc_map_writable(void* h) { return h ? (((ipc_map*)h)->writable & 3) != 0 : 0; }
char* trc_map_ptr(void* h) { return h ? (char*)((ipc_map*)h)->p : NULL; }

int64_t trc_map_read(void* h, int64_t off, char* out, int64_t n) {
    ipc_map* m = (ipc_map*)h;
    if (!m || off < 0 || n < 0 || off + n > m->len) return ipc_fail_msg("map: read outside the mapping");
    memcpy(out, m->p + off, (size_t)n);
    return n;
}
int64_t trc_map_write(void* h, int64_t off, const char* d, int64_t n) {
    ipc_map* m = (ipc_map*)h;
    if (!m || off < 0 || n < 0 || off + n > m->len) return ipc_fail_msg("map: write outside the mapping");
    if (!(m->writable & 3)) return ipc_fail_msg("map: mapping is read-only");
    memcpy(m->p + off, d, (size_t)n);
    return n;
}
/* First index of needle at or after `from`, or -1. */
int64_t trc_map_find(void* h, const char* needle, int64_t nl, int64_t from) {
    ipc_map* m = (ipc_map*)h; int64_t i;
    if (!m || nl <= 0 || from < 0) return -1;
    for (i = from; i + nl <= m->len; i++) {
        if (m->p[i] == (unsigned char)needle[0] && memcmp(m->p + i, needle, (size_t)nl) == 0) return i;
    }
    return -1;
}
/* 64-bit atomics at an 8-byte aligned offset (for cross-process counters). */
static int64_t* map_slot(ipc_map* m, int64_t off) {
    if (!m || off < 0 || off + 8 > m->len || ((uintptr_t)(m->p + off) & 7)) return NULL;
    return (int64_t*)(void*)(m->p + off);
}
int64_t trc_map_atomic_load(void* h, int64_t off) { int64_t* s = map_slot((ipc_map*)h, off); return s ? __atomic_load_n(s, __ATOMIC_SEQ_CST) : 0; }
int64_t trc_map_atomic_store(void* h, int64_t off, int64_t v) { int64_t* s = map_slot((ipc_map*)h, off); if (!s) return -1; __atomic_store_n(s, v, __ATOMIC_SEQ_CST); return 0; }
int64_t trc_map_atomic_add(void* h, int64_t off, int64_t v) { int64_t* s = map_slot((ipc_map*)h, off); return s ? __atomic_fetch_add(s, v, __ATOMIC_SEQ_CST) : 0; }
/* 1 if *slot was `expect` and is now `desired`. */
int64_t trc_map_atomic_cas(void* h, int64_t off, int64_t expect, int64_t desired) {
    int64_t* s = map_slot((ipc_map*)h, off);
    if (!s) return 0;
    return __atomic_compare_exchange_n(s, &expect, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST) ? 1 : 0;
}
int64_t trc_map_flush(void* h) {
    ipc_map* m = (ipc_map*)h;
    if (!m) return -1;
#if defined(_WIN32)
    if (!FlushViewOfFile(m->p, (SIZE_T)m->len)) return ipc_fail("map: flush");
    if (m->file != INVALID_HANDLE_VALUE) FlushFileBuffers(m->file);
#else
    {
        long pg = sysconf(_SC_PAGESIZE); uintptr_t start = (uintptr_t)m->p & ~(uintptr_t)(pg - 1);
        if (m->fd >= 0 && msync((void*)start, (size_t)((uintptr_t)m->p + (uintptr_t)m->len - start), MS_SYNC) < 0) return ipc_fail("map: msync");
    }
#endif
    return 0;
}
void trc_map_close(void* h) {
    ipc_map* m = (ipc_map*)h; int64_t delta;
    if (!m) return;
    delta = (m->writable >> 2) & 0xfffffff;
#if defined(_WIN32)
    if (m->writable & (1 << 30)) VirtualFree(m->p, 0, MEM_RELEASE);
    else if (m->p) UnmapViewOfFile(m->p - delta);
    if (m->mapping) CloseHandle(m->mapping);
    if (m->file != INVALID_HANDLE_VALUE) CloseHandle(m->file);
#else
    if (m->p) munmap(m->p - delta, (size_t)(m->len + delta));
    if (m->fd >= 0) close(m->fd);
#endif
    free(m);
}

/* ═══════════════════════════ shared memory ═══════════════════════════════
 * Named segments: POSIX shm_open("/name"), Windows "Local\name" mappings.
 * create != 0 creates (or opens) with `size`; otherwise opens an existing
 * segment and maps all of it. */
void* trc_shm_open(const char* name, int64_t size, int64_t create, int64_t exclusive) {
    ipc_map* m = map_new(); char nm[300];
    if (!m) return NULL;
    m->writable = 1;
#if defined(_WIN32)
    snprintf(nm, sizeof nm, "Local\\%s", name[0] == '/' ? name + 1 : name);
    if (create) {
        m->mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, (DWORD)((uint64_t)size >> 32), (DWORD)size, nm);
        if (m->mapping && exclusive && GetLastError() == ERROR_ALREADY_EXISTS) { CloseHandle(m->mapping); free(m); ipc_fail_msg("shm: segment already exists"); return NULL; }
    } else {
        m->mapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, nm);
    }
    if (!m->mapping) { ipc_fail("shm: open"); free(m); return NULL; }
    m->p = (unsigned char*)MapViewOfFile(m->mapping, FILE_MAP_ALL_ACCESS, 0, 0, create ? (SIZE_T)size : 0);
    if (!m->p) { ipc_fail("shm: MapViewOfFile"); CloseHandle(m->mapping); free(m); return NULL; }
    {
        MEMORY_BASIC_INFORMATION mi;
        m->len = create ? size : (VirtualQuery(m->p, &mi, sizeof mi) ? (int64_t)mi.RegionSize : 0);
    }
#else
    {
        struct stat st; int flags = O_RDWR; void* p;
        snprintf(nm, sizeof nm, "%s%s", name[0] == '/' ? "" : "/", name);
        if (create) flags |= O_CREAT | (exclusive ? O_EXCL : 0);
        m->fd = shm_open(nm, flags, 0600);
        if (m->fd < 0) { ipc_fail("shm_open"); free(m); return NULL; }
        if (fstat(m->fd, &st) < 0) { ipc_fail("shm: stat"); close(m->fd); free(m); return NULL; }
        if (create && st.st_size < size) {
            if (ftruncate(m->fd, (off_t)size) < 0) { ipc_fail("shm: ftruncate"); close(m->fd); free(m); return NULL; }
            st.st_size = (off_t)size;
        }
        if (st.st_size <= 0) { ipc_fail_msg("shm: segment has zero size"); close(m->fd); free(m); return NULL; }
        p = mmap(NULL, (size_t)st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, m->fd, 0);
        if (p == MAP_FAILED) { ipc_fail("shm: mmap"); close(m->fd); free(m); return NULL; }
        m->p = (unsigned char*)p; m->len = (int64_t)st.st_size;
    }
#endif
    return m;
}
/* Remove the name (existing mappings stay valid). No-op on Windows, where a
 * segment disappears with its last handle. */
int64_t trc_shm_unlink(const char* name) {
#if defined(_WIN32)
    (void)name; return 0;
#else
    char nm[300];
    snprintf(nm, sizeof nm, "%s%s", name[0] == '/' ? "" : "/", name);
    if (shm_unlink(nm) < 0) return ipc_fail("shm_unlink");
    return 0;
#endif
}

/* ═══════════════════════════ named semaphores ════════════════════════════ */
typedef struct {
#if defined(_WIN32)
    HANDLE h;
#else
    sem_t* s;
#endif
} ipc_sem;

void* trc_sem_open(const char* name, int64_t create, int64_t initial) {
    ipc_sem* s = (ipc_sem*)calloc(1, sizeof(ipc_sem)); char nm[300];
    if (!s) return NULL;
#if defined(_WIN32)
    snprintf(nm, sizeof nm, "Local\\%s", name[0] == '/' ? name + 1 : name);
    s->h = create ? CreateSemaphoreA(NULL, (LONG)initial, 0x7fffffff, nm) : OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, nm);
    if (!s->h) { ipc_fail("semaphore: open"); free(s); return NULL; }
#else
    snprintf(nm, sizeof nm, "%s%s", name[0] == '/' ? "" : "/", name);
    s->s = create ? sem_open(nm, O_CREAT, 0600, (unsigned)initial) : sem_open(nm, 0);
    if (s->s == SEM_FAILED) { ipc_fail("sem_open"); free(s); return NULL; }
#endif
    return s;
}
int64_t trc_sem_post(void* h) {
    ipc_sem* s = (ipc_sem*)h;
#if defined(_WIN32)
    if (!ReleaseSemaphore(s->h, 1, NULL)) return ipc_fail("semaphore: post");
#else
    if (sem_post(s->s) < 0) return ipc_fail("sem_post");
#endif
    return 0;
}
/* timeout_ms < 0 waits forever, 0 tries once. 1 acquired, 0 timed out, -1 error. */
int64_t trc_sem_wait(void* h, int64_t timeout_ms) {
    ipc_sem* s = (ipc_sem*)h;
#if defined(_WIN32)
    DWORD r = WaitForSingleObject(s->h, timeout_ms < 0 ? INFINITE : (DWORD)timeout_ms);
    if (r == WAIT_OBJECT_0) return 1;
    if (r == WAIT_TIMEOUT) return 0;
    return ipc_fail("semaphore: wait");
#else
    if (timeout_ms < 0) {
        while (sem_wait(s->s) < 0) { if (errno != EINTR) return ipc_fail("sem_wait"); }
        return 1;
    }
    if (timeout_ms == 0) {
        if (sem_trywait(s->s) == 0) return 1;
        if (errno == EAGAIN) return 0;
        return ipc_fail("sem_trywait");
    }
#  if defined(__APPLE__)
    {   /* no sem_timedwait on macOS: poll */
        int64_t waited = 0;
        while (waited < timeout_ms) {
            struct timespec ts = {0, 2000000};
            if (sem_trywait(s->s) == 0) return 1;
            if (errno != EAGAIN) return ipc_fail("sem_trywait");
            nanosleep(&ts, NULL); waited += 2;
        }
        return sem_trywait(s->s) == 0 ? 1 : 0;
    }
#  else
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += (time_t)(timeout_ms / 1000);
        ts.tv_nsec += (long)((timeout_ms % 1000) * 1000000);
        if (ts.tv_nsec >= 1000000000L) { ts.tv_sec++; ts.tv_nsec -= 1000000000L; }
        while (sem_timedwait(s->s, &ts) < 0) {
            if (errno == ETIMEDOUT) return 0;
            if (errno != EINTR) return ipc_fail("sem_timedwait");
        }
        return 1;
    }
#  endif
#endif
}
void trc_sem_close(void* h) {
    ipc_sem* s = (ipc_sem*)h;
    if (!s) return;
#if defined(_WIN32)
    CloseHandle(s->h);
#else
    sem_close(s->s);
#endif
    free(s);
}
int64_t trc_sem_unlink(const char* name) {
#if defined(_WIN32)
    (void)name; return 0;
#else
    char nm[300];
    snprintf(nm, sizeof nm, "%s%s", name[0] == '/' ? "" : "/", name);
    if (sem_unlink(nm) < 0) return ipc_fail("sem_unlink");
    return 0;
#endif
}

/* ═══════════════════════════ pipes ═══════════════════════════════════════
 * Descriptors are C runtime fds on both platforms (Windows: _pipe). */
int64_t trc_pipe(int64_t* fds2) {
    int f[2];
#if defined(_WIN32)
    if (_pipe(f, 65536, _O_BINARY) < 0) return ipc_fail("pipe");
#else
    if (pipe(f) < 0) return ipc_fail("pipe");
#endif
    fds2[0] = f[0]; fds2[1] = f[1];
    return 0;
}
/* Create a FIFO (named pipe) node. POSIX only. */
int64_t trc_mkfifo(const char* path, int64_t mode) {
#if defined(_WIN32)
    (void)path; (void)mode;
    return ipc_fail_msg("mkfifo: FIFOs are not available on Windows");
#else
    if (mkfifo(path, (mode_t)mode) < 0) return ipc_fail("mkfifo");
    return 0;
#endif
}
/* Open a path for fd I/O. mode: 0 read, 1 write, 2 read-write; nonblock. */
int64_t trc_fd_open(const char* path, int64_t mode, int64_t nonblock) {
    int fl = mode == 0 ? O_RDONLY : mode == 1 ? O_WRONLY : O_RDWR, fd;
#if defined(_WIN32)
    (void)nonblock;
    fd = _open(path, fl | _O_BINARY);
#else
    if (nonblock) fl |= O_NONBLOCK;
    fd = open(path, fl);
#endif
    if (fd < 0) return ipc_fail("open");
    return fd;
}
int64_t trc_fd_read(int64_t fd, char* out, int64_t cap) {
    long r;
#if defined(_WIN32)
    r = _read((int)fd, out, (unsigned)cap);
#else
    do { r = (long)read((int)fd, out, (size_t)cap); } while (r < 0 && errno == EINTR);
    if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return -2;
#endif
    if (r < 0) return ipc_fail("read");
    return r;
}
int64_t trc_fd_write(int64_t fd, const char* d, int64_t n) {
    int64_t done = 0;
    while (done < n) {
        long r;
#if defined(_WIN32)
        r = _write((int)fd, d + done, (unsigned)(n - done));
#else
        r = (long)write((int)fd, d + done, (size_t)(n - done));
        if (r < 0 && errno == EINTR) continue;
        if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) { if (done) return done; return -2; }
#endif
        if (r < 0) return ipc_fail("write");
        done += r;
    }
    return done;
}
/* Wait until fd is readable: 1 ready, 0 timeout, -1 error. (POSIX; Windows pipes: always 1.) */
int64_t trc_fd_wait_readable(int64_t fd, int64_t timeout_ms) {
#if defined(_WIN32)
    (void)fd; (void)timeout_ms; return 1;
#else
    struct pollfd p; int r;
    p.fd = (int)fd; p.events = POLLIN; p.revents = 0;
    do { r = poll(&p, 1, (int)timeout_ms); } while (r < 0 && errno == EINTR);
    if (r < 0) return ipc_fail("poll");
    return r > 0 ? 1 : 0;
#endif
}
int64_t trc_fd_close(int64_t fd) {
#if defined(_WIN32)
    return _close((int)fd) < 0 ? ipc_fail("close") : 0;
#else
    return close((int)fd) < 0 ? ipc_fail("close") : 0;
#endif
}

/* ═══════════════════════════ Unix-domain sockets ═════════════════════════ */
#if defined(_WIN32)
static void ipc_wsa(void) { static int done = 0; if (!done) { WSADATA w; WSAStartup(MAKEWORD(2, 2), &w); done = 1; } }
#define SOCK_T SOCKET
#define BADSOCK INVALID_SOCKET
#define sclose closesocket
#else
#define SOCK_T int
#define BADSOCK (-1)
#define sclose close
#endif

static int fill_un(trc_sockaddr_un* a, const char* path) {
    size_t n = strlen(path);
    memset(a, 0, sizeof *a);
    a->sun_family = AF_UNIX;
    if (n >= sizeof a->sun_path) { ipc_fail_msg("unix socket: path too long"); return -1; }
    memcpy(a->sun_path, path, n);
    return 0;
}
/* Bind + listen. Removes a stale socket file first when unlink_first. */
int64_t trc_unix_listen(const char* path, int64_t backlog, int64_t unlink_first) {
    trc_sockaddr_un a; SOCK_T s;
#if defined(_WIN32)
    ipc_wsa();
    if (unlink_first) DeleteFileA(path);
#else
    if (unlink_first) unlink(path);
#endif
    if (fill_un(&a, path) < 0) return -1;
    s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s == BADSOCK) return ipc_fail("unix socket");
    if (bind(s, (struct sockaddr*)&a, (int)sizeof a) < 0) { ipc_fail("unix socket: bind"); sclose(s); return -1; }
    if (listen(s, (int)backlog) < 0) { ipc_fail("unix socket: listen"); sclose(s); return -1; }
    return (int64_t)s;
}
/* Accept with timeout (ms, <0 = forever). Returns fd, -2 timeout, -1 error. */
int64_t trc_unix_accept(int64_t lfd, int64_t timeout_ms) {
    SOCK_T c;
    if (timeout_ms >= 0) {
#if defined(_WIN32)
        fd_set rs; struct timeval tv; FD_ZERO(&rs); FD_SET((SOCKET)lfd, &rs);
        tv.tv_sec = (long)(timeout_ms / 1000); tv.tv_usec = (long)((timeout_ms % 1000) * 1000);
        if (select(0, &rs, NULL, NULL, &tv) == 0) return -2;
#else
        struct pollfd p; int r; p.fd = (int)lfd; p.events = POLLIN; p.revents = 0;
        do { r = poll(&p, 1, (int)timeout_ms); } while (r < 0 && errno == EINTR);
        if (r == 0) return -2;
        if (r < 0) return ipc_fail("unix socket: poll");
#endif
    }
    c = accept((SOCK_T)lfd, NULL, NULL);
    if (c == BADSOCK) return ipc_fail("unix socket: accept");
    return (int64_t)c;
}
int64_t trc_unix_connect(const char* path) {
    trc_sockaddr_un a; SOCK_T s;
#if defined(_WIN32)
    ipc_wsa();
#endif
    if (fill_un(&a, path) < 0) return -1;
    s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s == BADSOCK) return ipc_fail("unix socket");
    if (connect(s, (struct sockaddr*)&a, (int)sizeof a) < 0) { ipc_fail("unix socket: connect"); sclose(s); return -1; }
    return (int64_t)s;
}
/* A connected pair (POSIX socketpair). */
int64_t trc_unix_pair(int64_t* fds2) {
#if defined(_WIN32)
    (void)fds2;
    return ipc_fail_msg("socketpair: not available on Windows");
#else
    int f[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, f) < 0) return ipc_fail("socketpair");
    fds2[0] = f[0]; fds2[1] = f[1];
    return 0;
#endif
}
int64_t trc_sock_send(int64_t fd, const char* d, int64_t n) {
    int64_t done = 0;
    while (done < n) {
        long r;
#if defined(MSG_NOSIGNAL)
        r = (long)send((SOCK_T)fd, d + done, (int)(n - done), MSG_NOSIGNAL);
#else
        r = (long)send((SOCK_T)fd, d + done, (int)(n - done), 0);
#endif
        if (r < 0) {
#if !defined(_WIN32)
            if (errno == EINTR) continue;
#endif
            return ipc_fail("send");
        }
        done += r;
    }
    return done;
}
/* Returns bytes, 0 on EOF, -2 timeout, -1 error. */
int64_t trc_sock_recv(int64_t fd, char* out, int64_t cap, int64_t timeout_ms) {
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
#endif
    }
    do { r = (long)recv((SOCK_T)fd, out, (int)cap, 0); }
#if defined(_WIN32)
    while (0);
#else
    while (r < 0 && errno == EINTR);
#endif
    if (r < 0) return ipc_fail("recv");
    return r;
}
int64_t trc_sock_close(int64_t fd) { return sclose((SOCK_T)fd) < 0 ? ipc_fail("close") : 0; }
int64_t trc_sock_shutdown_write(int64_t fd) {
#if defined(_WIN32)
    return shutdown((SOCKET)fd, SD_SEND) < 0 ? ipc_fail("shutdown") : 0;
#else
    return shutdown((int)fd, SHUT_WR) < 0 ? ipc_fail("shutdown") : 0;
#endif
}
/* Pass a file descriptor over a Unix socket (SCM_RIGHTS), with one data byte. */
int64_t trc_unix_send_fd(int64_t sock, int64_t fd) {
#if defined(_WIN32)
    (void)sock; (void)fd;
    return ipc_fail_msg("fd passing: not available on Windows");
#else
    struct msghdr msg; struct iovec iov; char byte = 'F';
    union { struct cmsghdr h; char buf[CMSG_SPACE(sizeof(int))]; } cm;
    struct cmsghdr* c; int f = (int)fd;
    memset(&msg, 0, sizeof msg); memset(&cm, 0, sizeof cm);
    iov.iov_base = &byte; iov.iov_len = 1;
    msg.msg_iov = &iov; msg.msg_iovlen = 1;
    msg.msg_control = cm.buf; msg.msg_controllen = sizeof cm.buf;
    c = CMSG_FIRSTHDR(&msg);
    c->cmsg_level = SOL_SOCKET; c->cmsg_type = SCM_RIGHTS; c->cmsg_len = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(c), &f, sizeof f);
    if (sendmsg((int)sock, &msg, 0) < 0) return ipc_fail("sendmsg");
    return 0;
#endif
}
int64_t trc_unix_recv_fd(int64_t sock) {
#if defined(_WIN32)
    (void)sock;
    return ipc_fail_msg("fd passing: not available on Windows");
#else
    struct msghdr msg; struct iovec iov; char byte;
    union { struct cmsghdr h; char buf[CMSG_SPACE(sizeof(int))]; } cm;
    struct cmsghdr* c; int f = -1; long r;
    memset(&msg, 0, sizeof msg);
    iov.iov_base = &byte; iov.iov_len = 1;
    msg.msg_iov = &iov; msg.msg_iovlen = 1;
    msg.msg_control = cm.buf; msg.msg_controllen = sizeof cm.buf;
    do { r = (long)recvmsg((int)sock, &msg, 0); } while (r < 0 && errno == EINTR);
    if (r < 0) return ipc_fail("recvmsg");
    if (r == 0) return ipc_fail_msg("recv_fd: connection closed");
    for (c = CMSG_FIRSTHDR(&msg); c; c = CMSG_NXTHDR(&msg, c))
        if (c->cmsg_level == SOL_SOCKET && c->cmsg_type == SCM_RIGHTS) { memcpy(&f, CMSG_DATA(c), sizeof f); break; }
    if (f < 0) return ipc_fail_msg("recv_fd: no descriptor in the message");
    return f;
#endif
}
int64_t trc_unlink_path(const char* path) {
#if defined(_WIN32)
    return DeleteFileA(path) ? 0 : ipc_fail("unlink");
#else
    return unlink(path) < 0 ? ipc_fail("unlink") : 0;
#endif
}
