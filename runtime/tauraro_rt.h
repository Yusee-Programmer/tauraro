/*
 * tauraro_rt.h — Tauraro Language Runtime
 *
 * Included by every compiled Tauraro program.
 * Provides: threading, channels, mutex, waitgroup,
 *           exceptions, string helpers, I/O, dict.
 *
 * Cross-platform: Windows (Win32 API) and POSIX (pthreads).
 */
#ifndef TAURARO_RT_H
#define TAURARO_RT_H

/* Must be defined before any system header to expose full POSIX/platform extensions:
 * pthread_rwlock_t, setenv, strdup, struct addrinfo, NI_NAMEREQD, clock_gettime, etc. */
#if defined(__linux__)
#  define _GNU_SOURCE
#elif defined(__APPLE__)
#  define _DARWIN_C_SOURCE
#elif defined(__unix__)
#  define _POSIX_C_SOURCE 200809L
#endif

/* ── Tauraro platform detection ──────────────────────────────────────────── *
 * Set TAURARO_NO_OS before including this header to target bare-metal or     *
 * freestanding environments (no OS, filesystem, networking, or threads).     *
 * These macros drive conditional compilation of all platform-specific code.  */
#if defined(__wasm__) || defined(__wasm32__) || defined(__EMSCRIPTEN__)
#  define TAURARO_WASM 1
#endif
#if defined(__ANDROID__)
#  define TAURARO_ANDROID 1
#endif
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#  define TAURARO_IOS 1
#endif
/* BARE = no OS: bare WASM (no WASI) or explicit TAURARO_NO_OS */
#if defined(TAURARO_NO_OS) || (defined(TAURARO_WASM) && !defined(__wasi__))
#  define TAURARO_BARE 1
#endif
/* KERNEL = Linux kernel module / bare-metal with user-supplied allocator.
 * Implies BARE (no OS threads/sockets), disables setjmp exceptions. */
#if defined(TAURARO_KERNEL)
#  if !defined(TAURARO_BARE)
#    define TAURARO_BARE 1
#  endif
#  define TAURARO_NO_EXCEPTIONS 1
#endif

#ifndef TAURARO_KERNEL
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdbool.h>
#  include <stdint.h>
#  include <string.h>
#  include <stdarg.h>
#  include <time.h>
#  include <math.h>
#  include <stdatomic.h>
#  include <setjmp.h>
#  include <ctype.h>
#else
/* Kernel / freestanding: caller supplies context headers.
 * At minimum: stddef.h, stdbool.h, stdint.h, stdatomic.h must exist. */
#  include <stddef.h>
#  include <stdbool.h>
#  include <stdint.h>
#  include <stdatomic.h>
#  ifdef __KERNEL__
#    include <linux/kernel.h>
#    include <linux/slab.h>
#    include <linux/string.h>
#  endif
#endif

/* ── Pluggable allocator macros ──────────────────────────────────────────── *
 * Override before including this header to redirect all runtime allocations: *
 *   #define TAURARO_ALLOC(sz)      kmalloc(sz, GFP_KERNEL)                   *
 *   #define TAURARO_FREE(p)        kfree(p)                                   *
 *   #define TAURARO_REALLOC(p,sz)  krealloc(p, sz, GFP_KERNEL)               *
 *   #define TAURARO_CALLOC(n,sz)   kzalloc((n)*(sz), GFP_KERNEL)             *
 * TAURARO_KERNEL mode requires all four to be defined externally.             */
#if defined(TAURARO_KERNEL)
#  if !defined(TAURARO_ALLOC) || !defined(TAURARO_FREE) || \
      !defined(TAURARO_REALLOC) || !defined(TAURARO_CALLOC)
#    error "TAURARO_KERNEL requires TAURARO_ALLOC/FREE/REALLOC/CALLOC to be defined"
#  endif
#else
#  ifndef TAURARO_ALLOC
#    define TAURARO_ALLOC(sz)      malloc(sz)
#  endif
#  ifndef TAURARO_FREE
#    define TAURARO_FREE(p)        free(p)
#  endif
#  ifndef TAURARO_REALLOC
#    define TAURARO_REALLOC(p,sz)  realloc(p,sz)
#  endif
#  ifndef TAURARO_CALLOC
#    define TAURARO_CALLOC(n,sz)   calloc(n,sz)
#  endif
#endif

/* ── Thread stack size ───────────────────────────────────────────────────── *
 * Override before including this header: -DTAURARO_THREAD_STACK_SIZE=N       *
 * 0 = use OS default (POSIX: skips setstacksize; Win32: passes 0 to          *
 * CreateThread which uses the executable's PE default, typically 1 MiB).     *
 * Platform defaults are applied below inside their respective #ifdef blocks.  */
#ifndef TAURARO_THREAD_STACK_SIZE
#  ifdef _WIN32
     /* Windows default: 2 MiB — matches legacy behaviour */
#    define TAURARO_THREAD_STACK_SIZE (2 * 1024 * 1024)
#  else
     /* POSIX default: 0 — let pthread use the OS default (typically 8 MiB) */
#    define TAURARO_THREAD_STACK_SIZE 0
#  endif
#endif

/* ── Panic / OOM hooks ───────────────────────────────────────────────────── */
#if defined(TAURARO_KERNEL) && defined(__KERNEL__)
#  define _TR_OOM_ABORT()    do { pr_err("tauraro: out of memory\n"); BUG(); } while(0)
#  define _TR_PANIC(msg)     do { pr_err("tauraro panic: %s\n", (msg)); BUG(); } while(0)
#elif defined(TAURARO_KERNEL)
#  define _TR_OOM_ABORT()    do { while(1); } while(0)
#  define _TR_PANIC(msg)     do { (void)(msg); while(1); } while(0)
#else
#  define _TR_OOM_ABORT()    do { fprintf(stderr, "tauraro: out of memory\n"); abort(); } while(0)
#  define _TR_PANIC(msg)     do { fprintf(stderr, "tauraro panic: %s\n", (msg)); abort(); } while(0)
#endif

/* ── Assert macros ───────────────────────────────────────────────────────── */
#if defined(TAURARO_KERNEL) && defined(__KERNEL__)
#  define _TR_ASSERT(cond)          do { if (!(cond)) { pr_err("assertion failed: %s  at %s:%d\n", #cond, __FILE__, __LINE__); BUG(); } } while(0)
#  define _TR_ASSERT_MSG(cond, msg) do { if (!(cond)) { pr_err("assertion failed: %s  message: %s  at %s:%d\n", #cond, (msg), __FILE__, __LINE__); BUG(); } } while(0)
#elif defined(TAURARO_KERNEL)
#  define _TR_ASSERT(cond)          do { if (!(cond)) { while(1); } } while(0)
#  define _TR_ASSERT_MSG(cond, msg) do { if (!(cond)) { (void)(msg); while(1); } } while(0)
#else
#  define _TR_ASSERT(cond) \
    do { if (!(cond)) { fprintf(stderr, "assertion failed: %s\n  at %s:%d\n", #cond, __FILE__, __LINE__); abort(); } } while(0)
#  define _TR_ASSERT_MSG(cond, msg) \
    do { if (!(cond)) { fprintf(stderr, "assertion failed: %s\n  message: %s\n  at %s:%d\n", #cond, (msg), __FILE__, __LINE__); abort(); } } while(0)
#endif

// Wrappers for core library to avoid signature conflicts
static inline void* _tr_c_malloc(size_t size) {
    return TAURARO_ALLOC(size);
}
static inline void* _tr_c_calloc(size_t count, size_t size) {
    void* p = TAURARO_CALLOC(count, size);
    if (!p && count * size > 0) { _TR_OOM_ABORT(); }
    return p;
}
static inline void _tr_free(void* p) {
    if (p) TAURARO_FREE(p);
}
static inline void _tr_c_free(void* ptr) { _tr_free(ptr); }

#ifndef TAURARO_KERNEL
static inline void _tr_print(char* s) { printf("%s\n", s); }
static inline void _tr_print_raw(char* s) { printf("%s", s); fflush(stdout); }
static inline void _tr_eprint(char* s) { fprintf(stderr, "%s\n", s); fflush(stderr); }
#else
static inline void _tr_print(char* s) { (void)s; }
static inline void _tr_print_raw(char* s) { (void)s; }
static inline void _tr_eprint(char* s) { (void)s; }
#endif

static inline void* _tr_c_realloc(void* ptr, size_t size) {
    return TAURARO_REALLOC(ptr, size);
}
static inline void* _tr_checked_alloc(size_t sz) {
    void* p = TAURARO_CALLOC(1, sz);
    if (!p && sz > 0) { _TR_OOM_ABORT(); }
    return p;
}
/* ── Shared ownership: reference-counted box (replaces Rc/Arc/Mutex in one keyword) ── */
typedef struct _TrSharedBox {
    _Atomic(int) refcount;
    void* data;
} _TrSharedBox;

static inline _TrSharedBox* _tr_shared_new(void* data) {
    _TrSharedBox* b = (_TrSharedBox*)_tr_checked_alloc(sizeof(_TrSharedBox));
    atomic_store(&b->refcount, 1);
    b->data = data;
    return b;
}
static inline _TrSharedBox* _tr_shared_clone(_TrSharedBox* b) {
    if (b) { atomic_fetch_add(&b->refcount, 1); }
    return b;
}
static inline void _tr_shared_drop(_TrSharedBox* b) {
    if (!b) return;
    if (atomic_fetch_sub(&b->refcount, 1) == 1) {
        _tr_free(b->data);
        _tr_free(b);
    }
}
/* ── Weak[T] — non-owning reference to a Shared[T] box ── */
typedef struct _TrWeakBox {
    _TrSharedBox* box;
} _TrWeakBox;
static inline _TrWeakBox* _tr_weak_new(_TrSharedBox* b) {
    _TrWeakBox* w = (_TrWeakBox*)_tr_checked_alloc(sizeof(_TrWeakBox));
    w->box = b;
    return w;
}
static inline bool _tr_weak_is_alive(_TrWeakBox* w) {
    if (!w || !w->box) return false;
    return atomic_load(&w->box->refcount) > 0;
}
static inline _TrSharedBox* _tr_weak_upgrade(_TrWeakBox* w) {
    if (!w || !w->box) return NULL;
    int old = atomic_load(&w->box->refcount);
    if (old <= 0) return NULL;
    atomic_fetch_add(&w->box->refcount, 1);
    return w->box;
}

static inline void* _tr_c_memcpy(void* dst, void* src, size_t n) { return memcpy(dst, src, n); }
static inline void* _tr_c_memset(void* ptr, int val, size_t n) { return memset(ptr, val, n); }
static inline void* _tr_c_memmove(void* dst, void* src, size_t n) { return memmove(dst, src, n); }
static inline void* _tr_c_fopen(const char* path, const char* mode) { return (void*)fopen(path, mode); }
static inline int _tr_c_fclose(void* fp) { return fclose((FILE*)fp); }
static inline size_t _tr_c_fread(void* ptr, size_t size, size_t nmemb, void* fp) { return fread(ptr, size, nmemb, (FILE*)fp); }
static inline size_t _tr_c_fwrite(const void* ptr, size_t size, size_t nmemb, void* fp) { return fwrite(ptr, size, nmemb, (FILE*)fp); }
static inline int _tr_c_fseek(void* fp, long offset, int whence) { return fseek((FILE*)fp, offset, whence); }
static inline long _tr_c_ftell(void* fp) { return ftell((FILE*)fp); }
static inline char* _tr_getenv(const char* name) { char* v = getenv(name); return v ? v : ""; }
#ifdef _WIN32
static inline int _tr_setenv(const char* name, const char* value) { return _putenv_s(name, value) == 0 ? 0 : -1; }
static inline int _tr_unsetenv(const char* name) { return _putenv_s(name, "") == 0 ? 0 : -1; }
#elif defined(TAURARO_BARE)
static inline int _tr_setenv(const char* name, const char* value) { (void)name; (void)value; return -1; }
static inline int _tr_unsetenv(const char* name) { (void)name; return -1; }
#else
static inline int _tr_setenv(const char* name, const char* value) { return setenv(name, value, 1) == 0 ? 0 : -1; }
static inline int _tr_unsetenv(const char* name) { return unsetenv(name) == 0 ? 0 : -1; }
#endif
#ifdef TAURARO_BARE
static inline char* _tr_popen_read(const char* cmd) { (void)cmd; return (char*)""; }
#else
static inline char* _tr_popen_read(const char* cmd) {
    if (!cmd) return "";
#  ifdef _WIN32
    FILE* fp = _popen(cmd, "r");
#  else
    FILE* fp = popen(cmd, "r");
#  endif
    if (!fp) return "";
    size_t cap = 4096, total = 0; char* buf = (char*)TAURARO_ALLOC(cap); char tmp[512];
    if (!buf) { fclose(fp); return ""; }
    while (fgets(tmp, sizeof(tmp), fp)) {
        size_t n = strlen(tmp);
        if (total + n + 1 > cap) { cap = cap * 2 + n + 1; buf = (char*)TAURARO_REALLOC(buf, cap); if (!buf) break; }
        memcpy(buf + total, tmp, n); total += n;
    }
    if (buf) buf[total] = '\0';
#ifdef _WIN32
    _pclose(fp);
#else
    pclose(fp);
#endif
    return buf ? buf : "";
}
#endif /* TAURARO_BARE popen guard */
#if !defined(_WIN32) && !defined(TAURARO_BARE)
static inline long long _tr_time_ns(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + (long long)ts.tv_nsec;
}
static inline char* _tr_path_canonicalize(const char* path) {
    char* r = realpath(path, NULL); return r ? r : (char*)path;
}
#elif defined(TAURARO_BARE)
static inline long long _tr_time_ns(void) {
#  ifdef __wasi__
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + (long long)ts.tv_nsec;
#  else
    return 0LL;
#  endif
}
static inline char* _tr_path_canonicalize(const char* path) { return (char*)path; }
#endif

/* ── Prelude: Option[T] ──────────────────────────────────────────────────── */
#ifndef _TR_ENUM_OPTION_DEFINED
#define _TR_ENUM_OPTION_DEFINED
typedef struct Option Option;
typedef enum { Option_Some, Option_None } Option_Tag;
struct Option {
    Option_Tag tag;
    union {
        struct { void* val; } Some;
        struct { int dummy; } None;
    } data;
};
#endif

/* ── Prelude: Result[T, E] ───────────────────────────────────────────────── */
#ifndef _TR_ENUM_RESULT_DEFINED
#define _TR_ENUM_RESULT_DEFINED
typedef struct Result Result;
typedef enum { Result_Ok, Result_Err } Result_Tag;
struct Result {
    Result_Tag tag;
    union {
        struct { void* val; } Ok;
        struct { void* err; } Err;
    } data;
};
#endif

/* ── Option[T] methods ───────────────────────────────────────────────── */
static inline bool Option_is_some(Option self) { return self.tag == Option_Some; }
static inline bool Option_is_none(Option self) { return self.tag == Option_None; }
static inline void* Option_unwrap(Option self) {
    if (self.tag != Option_Some) { fprintf(stderr, "Option.unwrap() called on None\n"); abort(); }
    return self.data.Some.val;
}
static inline void* Option_unwrap_or(Option self, void* _default) {
    return self.tag == Option_Some ? self.data.Some.val : _default;
}
static inline void* Option_expect(Option self, char* msg) {
    if (self.tag != Option_Some) { fprintf(stderr, "%s\n", msg); abort(); }
    return self.data.Some.val;
}
static inline Option Option_map(Option self, void* (*f)(void*)) {
    if (self.tag != Option_Some) return self;
    Option r; r.tag = Option_Some; r.data.Some.val = f(self.data.Some.val); return r;
}
static inline Option Option_and_then(Option self, Option (*f)(void*)) {
    return self.tag == Option_Some ? f(self.data.Some.val) : self;
}
static inline Option Option_or(Option self, Option other) {
    return self.tag == Option_Some ? self : other;
}
static inline Option Option_or_else(Option self, Option (*f)()) {
    return self.tag == Option_Some ? self : f();
}
static inline Result Option_ok_or(Option self, void* err);  /* defined after Result */

/* ── Result[T, E] methods ────────────────────────────────────────────── */
static inline bool Result_is_ok(Result self)  { return self.tag == Result_Ok;  }
static inline bool Result_is_err(Result self) { return self.tag == Result_Err; }
static inline void* Result_unwrap(Result self) {
    if (self.tag != Result_Ok) { fprintf(stderr, "Result.unwrap() called on Err\n"); abort(); }
    return self.data.Ok.val;
}
static inline void* Result_unwrap_err(Result self) {
    if (self.tag != Result_Err) { fprintf(stderr, "Result.unwrap_err() called on Ok\n"); abort(); }
    return self.data.Err.err;
}
static inline void* Result_unwrap_or(Result self, void* _default) {
    return self.tag == Result_Ok ? self.data.Ok.val : _default;
}
static inline void* Result_expect(Result self, char* msg) {
    if (self.tag != Result_Ok) { fprintf(stderr, "%s\n", msg); abort(); }
    return self.data.Ok.val;
}
static inline Result Result_map(Result self, void* (*f)(void*)) {
    if (self.tag != Result_Ok) return self;
    Result r; r.tag = Result_Ok; r.data.Ok.val = f(self.data.Ok.val); return r;
}
static inline Result Result_map_err(Result self, void* (*f)(void*)) {
    if (self.tag != Result_Err) return self;
    Result r; r.tag = Result_Err; r.data.Err.err = f(self.data.Err.err); return r;
}
static inline Result Result_and_then(Result self, Result (*f)(void*)) {
    return self.tag == Result_Ok ? f(self.data.Ok.val) : self;
}
static inline Option Result_ok(Result self) {
    Option o; o.tag = self.tag == Result_Ok ? Option_Some : Option_None;
    if (self.tag == Result_Ok) o.data.Some.val = self.data.Ok.val; return o;
}
static inline Option Result_err(Result self) {
    Option o; o.tag = self.tag == Result_Err ? Option_Some : Option_None;
    if (self.tag == Result_Err) o.data.Some.val = self.data.Err.err; return o;
}
/* Resolve forward decl */
static inline Result Option_ok_or(Option self, void* err) {
    Result r;
    r.tag = self.tag == Option_Some ? Result_Ok : Result_Err;
    if (self.tag == Option_Some) r.data.Ok.val = self.data.Some.val;
    else r.data.Err.err = err;
    return r;
}

/* ── Threading (cross-platform) ──────────────────────────────────────── */

/* Thread panic state — forward-declared before platform blocks so trampolines
 * can reference them.  Actual storage definitions live in the _TR_GLOBAL section. */
#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
static int     _tr_thread_has_panic_buf   = 0;
static jmp_buf _tr_thread_panic_jmpbuf;
static char*   _tr_thread_panic_message   = NULL;
#elif defined(__GNUC__) || defined(__clang__)
/* MinGW also defines _WIN32 but is GCC-based — must use __thread, not __declspec(thread) */
extern __thread int     _tr_thread_has_panic_buf;
extern __thread jmp_buf _tr_thread_panic_jmpbuf;
extern __thread char*   _tr_thread_panic_message;
#elif defined(_MSC_VER)
extern __declspec(thread) int     _tr_thread_has_panic_buf;
extern __declspec(thread) jmp_buf _tr_thread_panic_jmpbuf;
extern __declspec(thread) char*   _tr_thread_panic_message;
#else
extern _Thread_local int     _tr_thread_has_panic_buf;
extern _Thread_local jmp_buf _tr_thread_panic_jmpbuf;
extern _Thread_local char*   _tr_thread_panic_message;
#endif

/* Panic result: written by thread, read by joiner via _TrThreadObj */
typedef struct { int panicked; char* panic_msg; } _TrSpawnResult;

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

typedef HANDLE _TrThread;
/* Trampoline: routes void*(*)(void*) through DWORD WINAPI, installs
 * per-thread panic handler (setjmp), and enforces stack size.        */
typedef struct { void*(*fn)(void*); void* arg; _TrSpawnResult* result; } _TrWin32StartArg;
static DWORD WINAPI _tr_thread_start_trampoline(LPVOID raw) {
    _TrWin32StartArg* s = (_TrWin32StartArg*)raw;
    void*(*fn)(void*) = s->fn; void* arg = s->arg;
    _TrSpawnResult* result = s->result;
    free(s);
    /* Install per-thread panic handler */
    _tr_thread_has_panic_buf = 1;
    _tr_thread_panic_message = NULL;
    if (setjmp(_tr_thread_panic_jmpbuf) == 0) {
        fn(arg);
        if (result) { result->panicked = 0; result->panic_msg = NULL; }
    } else {
        if (result) { result->panicked = 1; result->panic_msg = _tr_thread_panic_message; }
        else { fprintf(stderr, "thread panic (detached): %s\n",
               _tr_thread_panic_message ? _tr_thread_panic_message : "?"); }
    }
    _tr_thread_has_panic_buf = 0;
    return 0;
}
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    _TrWin32StartArg* s = (_TrWin32StartArg*)malloc(sizeof(_TrWin32StartArg));
    s->fn = fn; s->arg = arg; s->result = NULL;
    SIZE_T ss = (SIZE_T)TAURARO_THREAD_STACK_SIZE;
    return CreateThread(NULL, ss, _tr_thread_start_trampoline, s, 0, NULL);
}
static void _tr_thread_detach(_TrThread t) { CloseHandle(t); }
static void _tr_thread_join_wait(_TrThread t) { WaitForSingleObject(t, INFINITE); CloseHandle(t); }

typedef CRITICAL_SECTION _TrMutex;
static void _tr_mutex_init(_TrMutex* m)   { InitializeCriticalSection(m); }
static void _tr_mutex_lock(_TrMutex* m)   { EnterCriticalSection(m); }
static void _tr_mutex_unlock(_TrMutex* m) { LeaveCriticalSection(m); }

typedef struct { CRITICAL_SECTION cs; CONDITION_VARIABLE cv; } _TrCondMutex;
static void _tr_condmutex_init(_TrCondMutex* cm)    { InitializeCriticalSection(&cm->cs); InitializeConditionVariable(&cm->cv); }
static void _tr_condmutex_lock(_TrCondMutex* cm)    { EnterCriticalSection(&cm->cs); }
static void _tr_condmutex_unlock(_TrCondMutex* cm)  { LeaveCriticalSection(&cm->cs); }
static void _tr_condmutex_wait(_TrCondMutex* cm)    { SleepConditionVariableCS(&cm->cv, &cm->cs, INFINITE); }
static void _tr_condmutex_signal(_TrCondMutex* cm)  { WakeConditionVariable(&cm->cv); }
static void _tr_sleep_ms(long ms) { Sleep((DWORD)(ms < 0 ? 0 : ms)); }
static inline long long _tr_time_ns(void) {
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq); QueryPerformanceCounter(&cnt);
    return (long long)((double)cnt.QuadPart * 1000000000.0 / (double)freq.QuadPart);
}
static inline char* _tr_path_canonicalize(const char* path) {
    char* buf = (char*)malloc(MAX_PATH);
    if (!buf) return (char*)path;
    DWORD n = GetFullPathNameA(path, MAX_PATH, buf, NULL);
    if (n == 0) { free(buf); return (char*)path; }
    return buf;
}

#elif defined(TAURARO_BARE)
/* ── BARE/WASM: single-threaded primitive stubs ──────────────────────── */
typedef int _TrThread;
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) { fn(arg); return 0; }
static void _tr_thread_detach(_TrThread t)      { (void)t; }
static void _tr_thread_join_wait(_TrThread t)   { (void)t; }

typedef int _TrMutex;
static void _tr_mutex_init(_TrMutex* m)         { (void)m; }
static void _tr_mutex_lock(_TrMutex* m)         { (void)m; }
static void _tr_mutex_unlock(_TrMutex* m)       { (void)m; }

typedef struct { int dummy; } _TrCondMutex;
static void _tr_condmutex_init(_TrCondMutex* cm)    { (void)cm; }
static void _tr_condmutex_lock(_TrCondMutex* cm)    { (void)cm; }
static void _tr_condmutex_unlock(_TrCondMutex* cm)  { (void)cm; }
static void _tr_condmutex_wait(_TrCondMutex* cm)    { (void)cm; }
static void _tr_condmutex_signal(_TrCondMutex* cm)  { (void)cm; }
static void _tr_sleep_ms(long ms) { (void)ms; }

#else
#include <pthread.h>
#include <time.h>

typedef pthread_t _TrThread;
/* POSIX panic trampoline — installs per-thread setjmp handler */
typedef struct { void*(*fn)(void*); void* arg; _TrSpawnResult* result; } _TrPosixStartArg;
static void* _tr_posix_thread_trampoline(void* raw) {
    _TrPosixStartArg* s = (_TrPosixStartArg*)raw;
    void*(*fn)(void*) = s->fn; void* arg = s->arg;
    _TrSpawnResult* result = s->result;
    free(s);
    _tr_thread_has_panic_buf = 1;
    _tr_thread_panic_message = NULL;
    if (setjmp(_tr_thread_panic_jmpbuf) == 0) {
        fn(arg);
        if (result) { result->panicked = 0; result->panic_msg = NULL; }
    } else {
        if (result) { result->panicked = 1; result->panic_msg = _tr_thread_panic_message; }
        else { fprintf(stderr, "thread panic (detached): %s\n",
               _tr_thread_panic_message ? _tr_thread_panic_message : "?"); }
    }
    _tr_thread_has_panic_buf = 0;
    return NULL;
}
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    _TrPosixStartArg* s = (_TrPosixStartArg*)malloc(sizeof(_TrPosixStartArg));
    s->fn = fn; s->arg = arg; s->result = NULL;
    pthread_attr_t attr; pthread_attr_init(&attr);
    if (TAURARO_THREAD_STACK_SIZE > 0)
        pthread_attr_setstacksize(&attr, (size_t)TAURARO_THREAD_STACK_SIZE);
    pthread_attr_setguardsize(&attr, 4096);  /* one-page guard against overflow */
    pthread_t t; pthread_create(&t, &attr, _tr_posix_thread_trampoline, s);
    pthread_attr_destroy(&attr); return t;
}
static void _tr_thread_detach(_TrThread t) { pthread_detach(t); }
static void _tr_thread_join_wait(_TrThread t) { pthread_join(t, NULL); }

typedef pthread_mutex_t _TrMutex;
static void _tr_mutex_init(_TrMutex* m)   { pthread_mutex_init(m, NULL); }
static void _tr_mutex_lock(_TrMutex* m)   { pthread_mutex_lock(m); }
static void _tr_mutex_unlock(_TrMutex* m) { pthread_mutex_unlock(m); }

typedef struct { pthread_mutex_t mu; pthread_cond_t cv; } _TrCondMutex;
static void _tr_condmutex_init(_TrCondMutex* cm)    { pthread_mutex_init(&cm->mu, NULL); pthread_cond_init(&cm->cv, NULL); }
static void _tr_condmutex_lock(_TrCondMutex* cm)    { pthread_mutex_lock(&cm->mu); }
static void _tr_condmutex_unlock(_TrCondMutex* cm)  { pthread_mutex_unlock(&cm->mu); }
static void _tr_condmutex_wait(_TrCondMutex* cm)    { pthread_cond_wait(&cm->cv, &cm->mu); }
static void _tr_condmutex_signal(_TrCondMutex* cm)  { pthread_cond_signal(&cm->cv); }
static void _tr_sleep_ms(long ms) {
    struct timespec ts = {ms/1000, (ms%1000)*1000000LL}; nanosleep(&ts, NULL);
}
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * Production async primitives — Win32 and POSIX implementations.
 * Design invariants (100% deadlock / race free):
 *   • Single lock per primitive — never acquire two locks simultaneously.
 *   • All condvar waits use while-loops — handles spurious wakeups.
 *   • Broadcast (not signal) on close/cancel — unblocks all waiters.
 *   • All heap-allocated; Tauraro holds opaque char* (void*) handles.
 * ═══════════════════════════════════════════════════════════════════════════*/
#include <limits.h>

#ifdef _WIN32

/* ── Bounded MPMC channel ────────────────────────────────────────────── */
typedef struct {
    long long* buf; long long head, tail, count, cap; volatile int closed;
    CRITICAL_SECTION mu; CONDITION_VARIABLE not_empty, not_full;
} _TrChan;
static _TrChan* _tr_chan_new(long long cap) {
    if (cap < 1) cap = 1;
    _TrChan* c = (_TrChan*)calloc(1, sizeof(_TrChan));
    c->buf = (long long*)calloc((size_t)cap, sizeof(long long)); c->cap = cap;
    InitializeCriticalSection(&c->mu);
    InitializeConditionVariable(&c->not_empty);
    InitializeConditionVariable(&c->not_full);
    return c;
}
static void _tr_chan_send(_TrChan* c, long long val) {
    EnterCriticalSection(&c->mu);
    while (c->count >= c->cap && !c->closed)
        SleepConditionVariableCS(&c->not_full, &c->mu, INFINITE);
    if (!c->closed) {
        c->buf[c->tail] = val; c->tail = (c->tail+1)%c->cap; c->count++;
        WakeConditionVariable(&c->not_empty);
    }
    LeaveCriticalSection(&c->mu);
}
static long long _tr_chan_recv(_TrChan* c) {
    EnterCriticalSection(&c->mu);
    while (c->count == 0 && !c->closed)
        SleepConditionVariableCS(&c->not_empty, &c->mu, INFINITE);
    long long v = 0;
    if (c->count > 0) {
        v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        WakeConditionVariable(&c->not_full);
    }
    LeaveCriticalSection(&c->mu); return v;
}
static bool _tr_chan_try_send(_TrChan* c, long long val) {
    EnterCriticalSection(&c->mu);
    bool ok = !c->closed && c->count < c->cap;
    if (ok) { c->buf[c->tail]=val; c->tail=(c->tail+1)%c->cap; c->count++; WakeConditionVariable(&c->not_empty); }
    LeaveCriticalSection(&c->mu); return ok;
}
static long long _tr_chan_try_recv_val(_TrChan* c) {
    EnterCriticalSection(&c->mu);
    long long v = LLONG_MIN;
    if (c->count > 0) { v=c->buf[c->head]; c->head=(c->head+1)%c->cap; c->count--; WakeConditionVariable(&c->not_full); }
    LeaveCriticalSection(&c->mu); return v;
}
static bool _tr_chan_send_timeout(_TrChan* c, long long val, long long ms) {
    EnterCriticalSection(&c->mu);
    ULONGLONG dl = GetTickCount64()+(ULONGLONG)ms; bool ok=true;
    while (c->count>=c->cap && !c->closed) {
        ULONGLONG now=GetTickCount64();
        if (now>=dl||!SleepConditionVariableCS(&c->not_full,&c->mu,(DWORD)(dl-now))){ok=false;break;}
    }
    if (ok&&!c->closed&&c->count<c->cap){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;WakeConditionVariable(&c->not_empty);}else ok=false;
    LeaveCriticalSection(&c->mu); return ok;
}
static long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms) {
    EnterCriticalSection(&c->mu);
    ULONGLONG dl=GetTickCount64()+(ULONGLONG)ms;
    while (c->count==0&&!c->closed){
        ULONGLONG now=GetTickCount64();
        if(now>=dl||!SleepConditionVariableCS(&c->not_empty,&c->mu,(DWORD)(dl-now))){LeaveCriticalSection(&c->mu);return LLONG_MIN;}
    }
    long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;WakeConditionVariable(&c->not_full);}
    LeaveCriticalSection(&c->mu); return v;
}
static void _tr_chan_close(_TrChan* c) {
    EnterCriticalSection(&c->mu); c->closed=1;
    WakeAllConditionVariable(&c->not_empty); WakeAllConditionVariable(&c->not_full);
    LeaveCriticalSection(&c->mu);
}
static bool   _tr_chan_is_closed(_TrChan* c) { EnterCriticalSection(&c->mu); bool r=c->closed!=0; LeaveCriticalSection(&c->mu); return r; }
static long long _tr_chan_len(_TrChan* c)    { EnterCriticalSection(&c->mu); long long n=c->count; LeaveCriticalSection(&c->mu); return n; }
static long long _tr_chan_cap(_TrChan* c)    { return c?c->cap:0; }
static void   _tr_chan_free(_TrChan* c)      { if(!c)return; DeleteCriticalSection(&c->mu); free(c->buf); free(c); }
static long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
    EnterCriticalSection(&c->mu);
    while (c->count == 0 && !c->closed)
        SleepConditionVariableCS(&c->not_empty, &c->mu, INFINITE);
    long long v = 0; *ok = 0;
    if (c->count > 0) {
        v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        WakeConditionVariable(&c->not_full); *ok = 1;
    }
    LeaveCriticalSection(&c->mu); return v;
}

/* ── Blocking task completion state ─────────────────────────────────── */
/* refcount=2: one for caller (_tr_task_free), one for worker (_tr_task_complete).
 * This prevents use-after-free when await_timeout abandons a still-running task. */
typedef struct {
    volatile long long result; char* error;
    volatile int done, cancelled, refcount;
    CRITICAL_SECTION mu; CONDITION_VARIABLE cv;
} _TrTaskState;
static _TrTaskState* _tr_task_new(void) {
    _TrTaskState* t=(_TrTaskState*)calloc(1,sizeof(_TrTaskState));
    InitializeCriticalSection(&t->mu); InitializeConditionVariable(&t->cv); t->error=""; t->refcount=2; return t;
}
static void _tr_task_complete(_TrTaskState* t, long long r) {
    int sf; EnterCriticalSection(&t->mu); if(!t->done){t->result=r;t->done=1;} WakeAllConditionVariable(&t->cv); sf=(--t->refcount<=0); LeaveCriticalSection(&t->mu);
    if(sf){DeleteCriticalSection(&t->mu);free(t);}
}
static void _tr_task_complete_err(_TrTaskState* t, const char* msg) {
    int sf; EnterCriticalSection(&t->mu); if(!t->done){t->error=msg?(char*)msg:"error";t->done=1;} WakeAllConditionVariable(&t->cv); sf=(--t->refcount<=0); LeaveCriticalSection(&t->mu);
    if(sf){DeleteCriticalSection(&t->mu);free(t);}
}
static void _tr_task_cancel(_TrTaskState* t) {
    EnterCriticalSection(&t->mu); if(!t->done){t->cancelled=1;t->done=1;} WakeAllConditionVariable(&t->cv); LeaveCriticalSection(&t->mu);
}
static long long _tr_task_await(_TrTaskState* t) {
    EnterCriticalSection(&t->mu);
    while(!t->done) SleepConditionVariableCS(&t->cv,&t->mu,INFINITE);
    long long r=t->result; LeaveCriticalSection(&t->mu); return r;
}
static bool _tr_task_await_timeout(_TrTaskState* t, long long ms, long long* out) {
    EnterCriticalSection(&t->mu);
    ULONGLONG dl=GetTickCount64()+(ULONGLONG)ms;
    while(!t->done){ULONGLONG now=GetTickCount64();if(now>=dl||!SleepConditionVariableCS(&t->cv,&t->mu,(DWORD)(dl-now))){LeaveCriticalSection(&t->mu);return false;}}
    *out=t->result; LeaveCriticalSection(&t->mu); return true;
}
static bool  _tr_task_is_done(_TrTaskState* t)      { EnterCriticalSection(&t->mu); bool r=t->done!=0;      LeaveCriticalSection(&t->mu); return r; }
static bool  _tr_task_is_cancelled(_TrTaskState* t)  { EnterCriticalSection(&t->mu); bool r=t->cancelled!=0; LeaveCriticalSection(&t->mu); return r; }
static bool  _tr_task_has_error(_TrTaskState* t)     { EnterCriticalSection(&t->mu); bool r=t->error&&t->error[0]; LeaveCriticalSection(&t->mu); return r; }
static char* _tr_task_get_error(_TrTaskState* t)     { EnterCriticalSection(&t->mu); char* e=t->error?t->error:""; LeaveCriticalSection(&t->mu); return e; }
static void  _tr_task_free(_TrTaskState* t) {
    if(!t)return; int sf; EnterCriticalSection(&t->mu); sf=(--t->refcount<=0); LeaveCriticalSection(&t->mu);
    if(sf){DeleteCriticalSection(&t->mu);free(t);}
}

/* ── Heap mutex ──────────────────────────────────────────────────────── */
typedef struct { CRITICAL_SECTION cs; } _TrMutexH;
static _TrMutexH* _tr_mutex_new(void)          { _TrMutexH* m=(_TrMutexH*)malloc(sizeof(_TrMutexH)); InitializeCriticalSection(&m->cs); return m; }
static void _tr_mutex_hlock(_TrMutexH* m)      { EnterCriticalSection(&m->cs); }
static void _tr_mutex_hunlock(_TrMutexH* m)    { LeaveCriticalSection(&m->cs); }
static bool _tr_mutex_htrylock(_TrMutexH* m)   { return TryEnterCriticalSection(&m->cs)!=0; }
static void _tr_mutex_hfree(_TrMutexH* m)      { if(!m)return; DeleteCriticalSection(&m->cs); free(m); }

/* ── Read-write lock (SRWLOCK) ───────────────────────────────────────── */
typedef struct { SRWLOCK l; } _TrRWL;
static _TrRWL* _tr_rwl_new(void)               { _TrRWL* r=(_TrRWL*)malloc(sizeof(_TrRWL)); InitializeSRWLock(&r->l); return r; }
static void _tr_rwl_read_lock(_TrRWL* r)       { AcquireSRWLockShared(&r->l); }
static void _tr_rwl_read_unlock(_TrRWL* r)     { ReleaseSRWLockShared(&r->l); }
static void _tr_rwl_write_lock(_TrRWL* r)      { AcquireSRWLockExclusive(&r->l); }
static void _tr_rwl_write_unlock(_TrRWL* r)    { ReleaseSRWLockExclusive(&r->l); }
static void _tr_rwl_free(_TrRWL* r)            { free(r); }

/* ── Counting semaphore ──────────────────────────────────────────────── */
typedef struct { HANDLE h; } _TrSema;
static _TrSema* _tr_sema_new(long long init, long long maxv) {
    _TrSema* s=(_TrSema*)malloc(sizeof(_TrSema));
    s->h=CreateSemaphoreA(NULL,(LONG)init,(LONG)(maxv>0?maxv:0x7fffffff),NULL); return s;
}
static void _tr_sema_acquire(_TrSema* s)             { WaitForSingleObject(s->h,INFINITE); }
static bool _tr_sema_try_acquire(_TrSema* s)         { return WaitForSingleObject(s->h,0)==WAIT_OBJECT_0; }
static bool _tr_sema_acquire_timeout(_TrSema* s, long long ms) { return WaitForSingleObject(s->h,(DWORD)ms)==WAIT_OBJECT_0; }
static void _tr_sema_release(_TrSema* s)             { ReleaseSemaphore(s->h,1,NULL); }
static void _tr_sema_free(_TrSema* s)                { if(!s)return; CloseHandle(s->h); free(s); }

/* ── WaitGroup ───────────────────────────────────────────────────────── */
typedef struct { volatile long long count; CRITICAL_SECTION mu; CONDITION_VARIABLE cv; } _TrWG;
static _TrWG* _tr_wg_new(void) { _TrWG* w=(_TrWG*)calloc(1,sizeof(_TrWG)); InitializeCriticalSection(&w->mu); InitializeConditionVariable(&w->cv); return w; }
static void _tr_wg_add(_TrWG* w, long long n)  { EnterCriticalSection(&w->mu); w->count+=n; if(w->count<=0)WakeAllConditionVariable(&w->cv); LeaveCriticalSection(&w->mu); }
static void _tr_wg_done(_TrWG* w)              { EnterCriticalSection(&w->mu); w->count--; if(w->count<=0)WakeAllConditionVariable(&w->cv); LeaveCriticalSection(&w->mu); }
static void _tr_wg_wait(_TrWG* w)              { EnterCriticalSection(&w->mu); while(w->count>0)SleepConditionVariableCS(&w->cv,&w->mu,INFINITE); LeaveCriticalSection(&w->mu); }
static bool _tr_wg_wait_timeout(_TrWG* w, long long ms) {
    EnterCriticalSection(&w->mu); ULONGLONG dl=GetTickCount64()+(ULONGLONG)ms; bool ok=true;
    while(w->count>0){ULONGLONG now=GetTickCount64();if(now>=dl||!SleepConditionVariableCS(&w->cv,&w->mu,(DWORD)(dl-now))){ok=false;break;}}
    LeaveCriticalSection(&w->mu); return ok;
}
static void _tr_wg_free(_TrWG* w) { if(!w)return; DeleteCriticalSection(&w->mu); free(w); }

/* ── Cyclic barrier ──────────────────────────────────────────────────── */
typedef struct { long long total,count,gen; CRITICAL_SECTION mu; CONDITION_VARIABLE cv; } _TrBarrier;
static _TrBarrier* _tr_barrier_new(long long n) { _TrBarrier* b=(_TrBarrier*)calloc(1,sizeof(_TrBarrier)); b->total=b->count=n; InitializeCriticalSection(&b->mu); InitializeConditionVariable(&b->cv); return b; }
static void _tr_barrier_wait(_TrBarrier* b) {
    EnterCriticalSection(&b->mu); long long g=b->gen;
    if(--b->count==0){b->gen++;b->count=b->total;WakeAllConditionVariable(&b->cv);}
    else while(b->gen==g) SleepConditionVariableCS(&b->cv,&b->mu,INFINITE);
    LeaveCriticalSection(&b->mu);
}
static void _tr_barrier_free(_TrBarrier* b) { if(!b)return; DeleteCriticalSection(&b->mu); free(b); }

/* ── Run-once guard: lockless atomic CAS — zero kernel object, zero heap mutex */
typedef struct { _Atomic int done; } _TrOnce;
static _TrOnce* _tr_once_new(void) {
    _TrOnce* o = (_TrOnce*)calloc(1, sizeof(_TrOnce));
    atomic_init(&o->done, 0); return o;
}
static bool _tr_once_do(_TrOnce* o) {
    int z = 0;
    return atomic_compare_exchange_strong_explicit(&o->done, &z, 1,
        memory_order_acq_rel, memory_order_relaxed);
}
static void _tr_once_free(_TrOnce* o) { if (o) free(o); }

/* ── Timer / Ticker ──────────────────────────────────────────────────── */
typedef struct { _TrChan* ch; long long ms; int periodic; volatile int stopped; CRITICAL_SECTION stop_mu; } _TrTimerState;
static DWORD WINAPI _tr_timer_thread_fn(LPVOID arg) {
    _TrTimerState* s=(_TrTimerState*)arg;
    do {
        Sleep((DWORD)s->ms);
        EnterCriticalSection(&s->stop_mu); int stopped=s->stopped; LeaveCriticalSection(&s->stop_mu);
        if(stopped) break;
        _tr_chan_try_send(s->ch, 1LL);
    } while(s->periodic);
    return 0;
}
static _TrTimerState* _tr_timer_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms;
    InitializeCriticalSection(&s->stop_mu);
    HANDLE t=CreateThread(NULL,0,_tr_timer_thread_fn,s,0,NULL); CloseHandle(t); return s;
}
static _TrTimerState* _tr_ticker_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms; s->periodic=1;
    InitializeCriticalSection(&s->stop_mu);
    HANDLE t=CreateThread(NULL,0,_tr_timer_thread_fn,s,0,NULL); CloseHandle(t); return s;
}
static void _tr_timer_stop(_TrTimerState* s) {
    if(!s)return; EnterCriticalSection(&s->stop_mu); s->stopped=1; LeaveCriticalSection(&s->stop_mu);
    _tr_chan_close(s->ch);
}

/* ── Thread-local storage (Win32 TLS slots) ──────────────────────────── */
typedef struct { DWORD key; } _TrTLS;
static inline _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)malloc(sizeof(_TrTLS));
    t->key = TlsAlloc();
    TlsSetValue(t->key, (LPVOID)(uintptr_t)(unsigned long long)init);
    return t;
}
static inline long long _tr_tls_get(_TrTLS* t) {
    return t ? (long long)(uintptr_t)TlsGetValue(t->key) : 0LL;
}
static inline void _tr_tls_set(_TrTLS* t, long long v) {
    if (t) TlsSetValue(t->key, (LPVOID)(uintptr_t)(unsigned long long)v);
}
static inline void _tr_tls_free(_TrTLS* t) { if (!t) return; TlsFree(t->key); free(t); }

#elif defined(TAURARO_BARE)
/* ═══════════════════════════════════════════════════════════════════════════
 * BARE/WASM: single-threaded async stubs — no locking, no blocking.
 * Channels are lock-free ring buffers; mutexes/semaphores are no-ops.
 * ═══════════════════════════════════════════════════════════════════════════*/

typedef struct {
    long long* buf; long long head, tail, count, cap; volatile int closed;
} _TrChan;
static _TrChan* _tr_chan_new(long long cap) {
    if (cap < 1) cap = 1;
    _TrChan* c = (_TrChan*)TAURARO_CALLOC(1, sizeof(_TrChan));
    c->buf = (long long*)TAURARO_CALLOC((size_t)cap, sizeof(long long)); c->cap = cap;
    return c;
}
static void _tr_chan_send(_TrChan* c, long long val) {
    if (!c || c->closed || c->count >= c->cap) return;
    c->buf[c->tail] = val; c->tail = (c->tail+1)%c->cap; c->count++;
}
static long long _tr_chan_recv(_TrChan* c) {
    if (!c || c->count == 0) return 0LL;
    long long v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
    return v;
}
static bool _tr_chan_try_send(_TrChan* c, long long val) {
    if (!c || c->closed || c->count >= c->cap) return false;
    c->buf[c->tail]=val; c->tail=(c->tail+1)%c->cap; c->count++; return true;
}
static long long _tr_chan_try_recv_val(_TrChan* c) {
    if (!c || c->count == 0) return LLONG_MIN;
    long long v=c->buf[c->head]; c->head=(c->head+1)%c->cap; c->count--; return v;
}
static bool  _tr_chan_send_timeout(_TrChan* c, long long val, long long ms)  { (void)ms; return _tr_chan_try_send(c, val); }
static long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms)         { (void)ms; return _tr_chan_try_recv_val(c); }
static void  _tr_chan_close(_TrChan* c)          { if (c) c->closed = 1; }
static bool  _tr_chan_is_closed(_TrChan* c)      { return c && c->closed; }
static long long _tr_chan_len(_TrChan* c)         { return c ? c->count : 0LL; }
static long long _tr_chan_cap(_TrChan* c)         { return c ? c->cap : 0LL; }
static void  _tr_chan_free(_TrChan* c)            { if (!c) return; TAURARO_FREE(c->buf); TAURARO_FREE(c); }
static long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
    if (c && c->count > 0) {
        long long v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        *ok = 1; return v;
    }
    *ok = 0; return 0LL;
}

typedef struct { volatile long long result; char* error; volatile int done, cancelled; } _TrTaskState;
static _TrTaskState* _tr_task_new(void) {
    _TrTaskState* t = (_TrTaskState*)TAURARO_CALLOC(1, sizeof(_TrTaskState)); t->error = ""; return t;
}
static void   _tr_task_complete(_TrTaskState* t, long long r)           { if (t&&!t->done){t->result=r;t->done=1;} }
static void   _tr_task_complete_err(_TrTaskState* t, const char* msg)   { if (t&&!t->done){t->error=msg?(char*)msg:"error";t->done=1;} }
static void   _tr_task_cancel(_TrTaskState* t)                           { if (t&&!t->done){t->cancelled=1;t->done=1;} }
static long long _tr_task_await(_TrTaskState* t)                         { return t?t->result:0LL; }
static bool   _tr_task_await_timeout(_TrTaskState* t, long long ms, long long* out) {
    (void)ms; if (t && out) *out = t->result; return t && t->done;
}
static bool   _tr_task_is_done(_TrTaskState* t)      { return t && t->done; }
static bool   _tr_task_is_cancelled(_TrTaskState* t) { return t && t->cancelled; }
static bool   _tr_task_has_error(_TrTaskState* t)    { return t && t->error && t->error[0]; }
static char*  _tr_task_get_error(_TrTaskState* t)    { return t && t->error ? t->error : ""; }
static void   _tr_task_free(_TrTaskState* t)          { if (t) TAURARO_FREE(t); }

typedef struct { int dummy; } _TrMutexH;
static _TrMutexH* _tr_mutex_new(void)             { return (_TrMutexH*)TAURARO_CALLOC(1, sizeof(_TrMutexH)); }
static void _tr_mutex_hlock(_TrMutexH* m)         { (void)m; }
static void _tr_mutex_hunlock(_TrMutexH* m)       { (void)m; }
static bool _tr_mutex_htrylock(_TrMutexH* m)      { (void)m; return true; }
static void _tr_mutex_hfree(_TrMutexH* m)         { if (m) TAURARO_FREE(m); }

typedef struct { int dummy; } _TrRWL;
static _TrRWL* _tr_rwl_new(void)                  { return (_TrRWL*)TAURARO_CALLOC(1, sizeof(_TrRWL)); }
static void _tr_rwl_read_lock(_TrRWL* r)          { (void)r; }
static void _tr_rwl_read_unlock(_TrRWL* r)        { (void)r; }
static void _tr_rwl_write_lock(_TrRWL* r)         { (void)r; }
static void _tr_rwl_write_unlock(_TrRWL* r)       { (void)r; }
static void _tr_rwl_free(_TrRWL* r)               { if (r) TAURARO_FREE(r); }

typedef struct { volatile long long count, maxv; } _TrSema;
static _TrSema* _tr_sema_new(long long init, long long maxv) {
    _TrSema* s = (_TrSema*)TAURARO_CALLOC(1, sizeof(_TrSema));
    s->count = init; s->maxv = maxv > 0 ? maxv : (long long)0x7fffffffffffffffLL; return s;
}
static void _tr_sema_acquire(_TrSema* s)           { if (s && s->count > 0) s->count--; }
static bool _tr_sema_try_acquire(_TrSema* s)       { if (s && s->count > 0) { s->count--; return true; } return false; }
static bool _tr_sema_acquire_timeout(_TrSema* s, long long ms) { (void)ms; return _tr_sema_try_acquire(s); }
static void _tr_sema_release(_TrSema* s)           { if (s && s->count < s->maxv) s->count++; }
static void _tr_sema_free(_TrSema* s)              { if (s) TAURARO_FREE(s); }

typedef struct { volatile long long count; } _TrWG;
static _TrWG* _tr_wg_new(void) { return (_TrWG*)TAURARO_CALLOC(1, sizeof(_TrWG)); }
static void _tr_wg_add(_TrWG* w, long long n)      { if (w) w->count += n; }
static void _tr_wg_done(_TrWG* w)                  { if (w && w->count > 0) w->count--; }
static void _tr_wg_wait(_TrWG* w)                  { (void)w; /* no blocking */ }
static bool _tr_wg_wait_timeout(_TrWG* w, long long ms) { (void)ms; return w ? w->count == 0 : true; }
static void _tr_wg_free(_TrWG* w)                  { if (w) TAURARO_FREE(w); }

typedef struct { long long total, count, gen; } _TrBarrier;
static _TrBarrier* _tr_barrier_new(long long n) {
    _TrBarrier* b = (_TrBarrier*)TAURARO_CALLOC(1, sizeof(_TrBarrier)); b->total = b->count = n; return b;
}
static void _tr_barrier_wait(_TrBarrier* b) {
    if (!b) return;
    if (--b->count == 0) { b->gen++; b->count = b->total; }
}
static void _tr_barrier_free(_TrBarrier* b) { if (b) TAURARO_FREE(b); }

/* _TrOnce: zero-cost run-once guard via atomic CAS — no mutex, no OS object */
typedef struct { _Atomic int done; } _TrOnce;
static _TrOnce* _tr_once_new(void) {
    _TrOnce* o = (_TrOnce*)TAURARO_CALLOC(1, sizeof(_TrOnce));
    atomic_init(&o->done, 0); return o;
}
static bool _tr_once_do(_TrOnce* o) {
    if (!o) return false;
    int z = 0;
    return atomic_compare_exchange_strong_explicit(&o->done, &z, 1,
        memory_order_acq_rel, memory_order_relaxed);
}
static void _tr_once_free(_TrOnce* o) { if (o) TAURARO_FREE(o); }

typedef struct { _TrChan* ch; long long ms; int periodic; volatile int stopped; } _TrTimerState;
static void* _tr_timer_thread_fn(void* arg) { (void)arg; return NULL; }
static _TrTimerState* _tr_timer_new(long long ms, _TrChan* ch) {
    _TrTimerState* s = (_TrTimerState*)TAURARO_CALLOC(1, sizeof(_TrTimerState));
    s->ch = ch; s->ms = ms;
    _tr_chan_try_send(ch, 1LL); /* fire immediately — no background thread */
    return s;
}
static _TrTimerState* _tr_ticker_new(long long ms, _TrChan* ch) {
    _TrTimerState* s = (_TrTimerState*)TAURARO_CALLOC(1, sizeof(_TrTimerState));
    s->ch = ch; s->ms = ms; s->periodic = 1;
    _tr_chan_try_send(ch, 1LL); return s;
}
static void _tr_timer_stop(_TrTimerState* s) {
    if (!s) return; s->stopped = 1;
    if (s->ch) _tr_chan_close(s->ch);
}

/* ── Thread-local storage (bare: single thread, single value) ────────── */
typedef struct { long long val; } _TrTLS;
static inline _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)TAURARO_ALLOC(sizeof(_TrTLS)); t->val = init; return t;
}
static inline long long _tr_tls_get(_TrTLS* t) { return t ? t->val : 0LL; }
static inline void _tr_tls_set(_TrTLS* t, long long v) { if (t) t->val = v; }
static inline void _tr_tls_free(_TrTLS* t) { if (t) TAURARO_FREE(t); }

/* ── BARE ThreadPool: runs jobs synchronously (no OS threads) ─────────── */
typedef struct { int _dummy; } _TrThreadPool;
static inline long long _tr_threadpool_auto_n(void)  { return 1LL; }
static inline _TrThreadPool* _tr_threadpool_new(long long n)  { (void)n; return (_TrThreadPool*)TAURARO_CALLOC(1,sizeof(_TrThreadPool)); }
static inline _TrThreadPool* _tr_threadpool_auto(void)        { return _tr_threadpool_new(1LL); }
static inline void _tr_threadpool_spawn(_TrThreadPool* p, void*(*fn)(void*), void* arg) { (void)p; fn(arg); }
static inline void _tr_threadpool_wait(_TrThreadPool* p)      { (void)p; }
static inline void _tr_threadpool_free(_TrThreadPool* p)      { if(p)TAURARO_FREE(p); }

#else /* POSIX ─────────────────────────────────────────────────────────── */

typedef struct {
    long long* buf; long long head,tail,count,cap; volatile int closed;
    pthread_mutex_t mu; pthread_cond_t not_empty, not_full;
} _TrChan;
static _TrChan* _tr_chan_new(long long cap) {
    if(cap<1)cap=1; _TrChan* c=(_TrChan*)calloc(1,sizeof(_TrChan));
    c->buf=(long long*)calloc((size_t)cap,sizeof(long long)); c->cap=cap;
    pthread_mutex_init(&c->mu,NULL); pthread_cond_init(&c->not_empty,NULL); pthread_cond_init(&c->not_full,NULL); return c;
}
static void _tr_chan_send(_TrChan* c, long long val) {
    pthread_mutex_lock(&c->mu);
    while(c->count>=c->cap&&!c->closed) pthread_cond_wait(&c->not_full,&c->mu);
    if(!c->closed){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}
    pthread_mutex_unlock(&c->mu);
}
static long long _tr_chan_recv(_TrChan* c) {
    pthread_mutex_lock(&c->mu);
    while(c->count==0&&!c->closed) pthread_cond_wait(&c->not_empty,&c->mu);
    long long v=0;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
static bool _tr_chan_try_send(_TrChan* c, long long val) {
    pthread_mutex_lock(&c->mu); bool ok=!c->closed&&c->count<c->cap;
    if(ok){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}
    pthread_mutex_unlock(&c->mu); return ok;
}
static long long _tr_chan_try_recv_val(_TrChan* c) {
    pthread_mutex_lock(&c->mu); long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
static bool _tr_chan_send_timeout(_TrChan* c, long long val, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&c->mu); bool ok=true;
    while(c->count>=c->cap&&!c->closed) if(pthread_cond_timedwait(&c->not_full,&c->mu,&ts)){ok=false;break;}
    if(ok&&!c->closed&&c->count<c->cap){c->buf[c->tail]=val;c->tail=(c->tail+1)%c->cap;c->count++;pthread_cond_signal(&c->not_empty);}else ok=false;
    pthread_mutex_unlock(&c->mu); return ok;
}
static long long _tr_chan_recv_timeout_val(_TrChan* c, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&c->mu);
    while(c->count==0&&!c->closed) if(pthread_cond_timedwait(&c->not_empty,&c->mu,&ts)){pthread_mutex_unlock(&c->mu);return LLONG_MIN;}
    long long v=LLONG_MIN;
    if(c->count>0){v=c->buf[c->head];c->head=(c->head+1)%c->cap;c->count--;pthread_cond_signal(&c->not_full);}
    pthread_mutex_unlock(&c->mu); return v;
}
static void _tr_chan_close(_TrChan* c) {
    pthread_mutex_lock(&c->mu); c->closed=1;
    pthread_cond_broadcast(&c->not_empty); pthread_cond_broadcast(&c->not_full); pthread_mutex_unlock(&c->mu);
}
static bool   _tr_chan_is_closed(_TrChan* c) { pthread_mutex_lock(&c->mu); bool r=c->closed!=0; pthread_mutex_unlock(&c->mu); return r; }
static long long _tr_chan_len(_TrChan* c)    { pthread_mutex_lock(&c->mu); long long n=c->count; pthread_mutex_unlock(&c->mu); return n; }
static long long _tr_chan_cap(_TrChan* c)    { return c?c->cap:0; }
static void   _tr_chan_free(_TrChan* c)      { if(!c)return; pthread_mutex_destroy(&c->mu); pthread_cond_destroy(&c->not_empty); pthread_cond_destroy(&c->not_full); free(c->buf); free(c); }
static long long _tr_chan_recv_ok(_TrChan* c, int* ok) {
    pthread_mutex_lock(&c->mu);
    while (c->count == 0 && !c->closed) pthread_cond_wait(&c->not_empty, &c->mu);
    long long v = 0; *ok = 0;
    if (c->count > 0) {
        v = c->buf[c->head]; c->head = (c->head+1)%c->cap; c->count--;
        pthread_cond_signal(&c->not_full); *ok = 1;
    }
    pthread_mutex_unlock(&c->mu); return v;
}

/* refcount=2: one for caller (_tr_task_free), one for worker (_tr_task_complete). */
typedef struct {
    volatile long long result; char* error; volatile int done, cancelled, refcount;
    pthread_mutex_t mu; pthread_cond_t cv;
} _TrTaskState;
static _TrTaskState* _tr_task_new(void) {
    _TrTaskState* t=(_TrTaskState*)calloc(1,sizeof(_TrTaskState));
    pthread_mutex_init(&t->mu,NULL); pthread_cond_init(&t->cv,NULL); t->error=""; t->refcount=2; return t;
}
static void _tr_task_complete(_TrTaskState* t, long long r) {
    int sf; pthread_mutex_lock(&t->mu); if(!t->done){t->result=r;t->done=1;} pthread_cond_broadcast(&t->cv); sf=(--t->refcount<=0); pthread_mutex_unlock(&t->mu);
    if(sf){pthread_mutex_destroy(&t->mu);pthread_cond_destroy(&t->cv);free(t);}
}
static void _tr_task_complete_err(_TrTaskState* t, const char* m) {
    int sf; pthread_mutex_lock(&t->mu); if(!t->done){t->error=m?(char*)m:"error";t->done=1;} pthread_cond_broadcast(&t->cv); sf=(--t->refcount<=0); pthread_mutex_unlock(&t->mu);
    if(sf){pthread_mutex_destroy(&t->mu);pthread_cond_destroy(&t->cv);free(t);}
}
static void _tr_task_cancel(_TrTaskState* t)                      { pthread_mutex_lock(&t->mu); if(!t->done){t->cancelled=1;t->done=1;} pthread_cond_broadcast(&t->cv); pthread_mutex_unlock(&t->mu); }
static long long _tr_task_await(_TrTaskState* t) {
    pthread_mutex_lock(&t->mu); while(!t->done) pthread_cond_wait(&t->cv,&t->mu);
    long long r=t->result; pthread_mutex_unlock(&t->mu); return r;
}
static bool _tr_task_await_timeout(_TrTaskState* t, long long ms, long long* out) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&t->mu);
    while(!t->done) if(pthread_cond_timedwait(&t->cv,&t->mu,&ts)){pthread_mutex_unlock(&t->mu);return false;}
    *out=t->result; pthread_mutex_unlock(&t->mu); return true;
}
static bool  _tr_task_is_done(_TrTaskState* t)      { pthread_mutex_lock(&t->mu); bool r=t->done!=0;      pthread_mutex_unlock(&t->mu); return r; }
static bool  _tr_task_is_cancelled(_TrTaskState* t)  { pthread_mutex_lock(&t->mu); bool r=t->cancelled!=0; pthread_mutex_unlock(&t->mu); return r; }
static bool  _tr_task_has_error(_TrTaskState* t)     { pthread_mutex_lock(&t->mu); bool r=t->error&&t->error[0]; pthread_mutex_unlock(&t->mu); return r; }
static char* _tr_task_get_error(_TrTaskState* t)     { pthread_mutex_lock(&t->mu); char* e=t->error?t->error:""; pthread_mutex_unlock(&t->mu); return e; }
static void  _tr_task_free(_TrTaskState* t) {
    if(!t)return; int sf; pthread_mutex_lock(&t->mu); sf=(--t->refcount<=0); pthread_mutex_unlock(&t->mu);
    if(sf){pthread_mutex_destroy(&t->mu);pthread_cond_destroy(&t->cv);free(t);}
}

typedef struct { pthread_mutex_t mu; } _TrMutexH;
static _TrMutexH* _tr_mutex_new(void)          { _TrMutexH* m=(_TrMutexH*)malloc(sizeof(_TrMutexH)); pthread_mutex_init(&m->mu,NULL); return m; }
static void _tr_mutex_hlock(_TrMutexH* m)      { pthread_mutex_lock(&m->mu); }
static void _tr_mutex_hunlock(_TrMutexH* m)    { pthread_mutex_unlock(&m->mu); }
static bool _tr_mutex_htrylock(_TrMutexH* m)   { return pthread_mutex_trylock(&m->mu)==0; }
static void _tr_mutex_hfree(_TrMutexH* m)      { if(!m)return; pthread_mutex_destroy(&m->mu); free(m); }

typedef struct { pthread_rwlock_t l; } _TrRWL;
static _TrRWL* _tr_rwl_new(void)               { _TrRWL* r=(_TrRWL*)malloc(sizeof(_TrRWL)); pthread_rwlock_init(&r->l,NULL); return r; }
static void _tr_rwl_read_lock(_TrRWL* r)       { pthread_rwlock_rdlock(&r->l); }
static void _tr_rwl_read_unlock(_TrRWL* r)     { pthread_rwlock_unlock(&r->l); }
static void _tr_rwl_write_lock(_TrRWL* r)      { pthread_rwlock_wrlock(&r->l); }
static void _tr_rwl_write_unlock(_TrRWL* r)    { pthread_rwlock_unlock(&r->l); }
static void _tr_rwl_free(_TrRWL* r)            { if(!r)return; pthread_rwlock_destroy(&r->l); free(r); }

typedef struct { volatile long long count, maxv; pthread_mutex_t mu; pthread_cond_t cv; } _TrSema;
static _TrSema* _tr_sema_new(long long init, long long maxv) {
    _TrSema* s=(_TrSema*)calloc(1,sizeof(_TrSema)); s->count=init; s->maxv=maxv>0?maxv:(long long)0x7fffffffffffffffLL;
    pthread_mutex_init(&s->mu,NULL); pthread_cond_init(&s->cv,NULL); return s;
}
static void _tr_sema_acquire(_TrSema* s)       { pthread_mutex_lock(&s->mu); while(s->count<=0)pthread_cond_wait(&s->cv,&s->mu); s->count--; pthread_mutex_unlock(&s->mu); }
static bool _tr_sema_try_acquire(_TrSema* s)   { pthread_mutex_lock(&s->mu); bool ok=s->count>0; if(ok)s->count--; pthread_mutex_unlock(&s->mu); return ok; }
static bool _tr_sema_acquire_timeout(_TrSema* s, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&s->mu);
    while(s->count<=0) if(pthread_cond_timedwait(&s->cv,&s->mu,&ts)){pthread_mutex_unlock(&s->mu);return false;}
    s->count--; pthread_mutex_unlock(&s->mu); return true;
}
static void _tr_sema_release(_TrSema* s)       { pthread_mutex_lock(&s->mu); if(s->count<s->maxv){s->count++;pthread_cond_signal(&s->cv);} pthread_mutex_unlock(&s->mu); }
static void _tr_sema_free(_TrSema* s)          { if(!s)return; pthread_mutex_destroy(&s->mu); pthread_cond_destroy(&s->cv); free(s); }

typedef struct { volatile long long count; pthread_mutex_t mu; pthread_cond_t cv; } _TrWG;
static _TrWG* _tr_wg_new(void) { _TrWG* w=(_TrWG*)calloc(1,sizeof(_TrWG)); pthread_mutex_init(&w->mu,NULL); pthread_cond_init(&w->cv,NULL); return w; }
static void _tr_wg_add(_TrWG* w, long long n)  { pthread_mutex_lock(&w->mu); w->count+=n; if(w->count<=0)pthread_cond_broadcast(&w->cv); pthread_mutex_unlock(&w->mu); }
static void _tr_wg_done(_TrWG* w)              { pthread_mutex_lock(&w->mu); w->count--; if(w->count<=0)pthread_cond_broadcast(&w->cv); pthread_mutex_unlock(&w->mu); }
static void _tr_wg_wait(_TrWG* w)              { pthread_mutex_lock(&w->mu); while(w->count>0)pthread_cond_wait(&w->cv,&w->mu); pthread_mutex_unlock(&w->mu); }
static bool _tr_wg_wait_timeout(_TrWG* w, long long ms) {
    struct timespec ts; clock_gettime(CLOCK_REALTIME,&ts);
    ts.tv_sec+=ms/1000; ts.tv_nsec+=(ms%1000)*1000000LL;
    if(ts.tv_nsec>=1000000000LL){ts.tv_sec++;ts.tv_nsec-=1000000000LL;}
    pthread_mutex_lock(&w->mu); bool ok=true;
    while(w->count>0) if(pthread_cond_timedwait(&w->cv,&w->mu,&ts)){ok=false;break;}
    pthread_mutex_unlock(&w->mu); return ok;
}
static void _tr_wg_free(_TrWG* w) { if(!w)return; pthread_mutex_destroy(&w->mu); pthread_cond_destroy(&w->cv); free(w); }

typedef struct { long long total,count,gen; pthread_mutex_t mu; pthread_cond_t cv; } _TrBarrier;
static _TrBarrier* _tr_barrier_new(long long n) { _TrBarrier* b=(_TrBarrier*)calloc(1,sizeof(_TrBarrier)); b->total=b->count=n; pthread_mutex_init(&b->mu,NULL); pthread_cond_init(&b->cv,NULL); return b; }
static void _tr_barrier_wait(_TrBarrier* b) {
    pthread_mutex_lock(&b->mu); long long g=b->gen;
    if(--b->count==0){b->gen++;b->count=b->total;pthread_cond_broadcast(&b->cv);}
    else while(b->gen==g) pthread_cond_wait(&b->cv,&b->mu);
    pthread_mutex_unlock(&b->mu);
}
static void _tr_barrier_free(_TrBarrier* b) { if(!b)return; pthread_mutex_destroy(&b->mu); pthread_cond_destroy(&b->cv); free(b); }

/* _TrOnce: lockless atomic CAS — no pthread_mutex, no heap lock object */
typedef struct { _Atomic int done; } _TrOnce;
static _TrOnce* _tr_once_new(void) {
    _TrOnce* o = (_TrOnce*)calloc(1, sizeof(_TrOnce));
    atomic_init(&o->done, 0); return o;
}
static bool _tr_once_do(_TrOnce* o) {
    int z = 0;
    return atomic_compare_exchange_strong_explicit(&o->done, &z, 1,
        memory_order_acq_rel, memory_order_relaxed);
}
static void _tr_once_free(_TrOnce* o) { if (o) free(o); }

typedef struct { _TrChan* ch; long long ms; int periodic; volatile int stopped; pthread_mutex_t stop_mu; } _TrTimerState;
static void* _tr_timer_thread_fn(void* arg) {
    _TrTimerState* s=(_TrTimerState*)arg;
    do {
        struct timespec ts={s->ms/1000,(s->ms%1000)*1000000LL}; nanosleep(&ts,NULL);
        pthread_mutex_lock(&s->stop_mu); int stopped=s->stopped; pthread_mutex_unlock(&s->stop_mu);
        if(stopped) break;
        _tr_chan_try_send(s->ch,1LL);
    } while(s->periodic);
    return NULL;
}
static _TrTimerState* _tr_timer_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms;
    pthread_mutex_init(&s->stop_mu,NULL);
    pthread_t t; pthread_create(&t,NULL,_tr_timer_thread_fn,s); pthread_detach(t); return s;
}
static _TrTimerState* _tr_ticker_new(long long ms, _TrChan* ch) {
    _TrTimerState* s=(_TrTimerState*)calloc(1,sizeof(_TrTimerState)); s->ch=ch; s->ms=ms; s->periodic=1;
    pthread_mutex_init(&s->stop_mu,NULL);
    pthread_t t; pthread_create(&t,NULL,_tr_timer_thread_fn,s); pthread_detach(t); return s;
}
static void _tr_timer_stop(_TrTimerState* s) {
    if(!s)return; pthread_mutex_lock(&s->stop_mu); s->stopped=1; pthread_mutex_unlock(&s->stop_mu);
    _tr_chan_close(s->ch);
}

/* ── Thread-local storage (POSIX pthread_key_t) ──────────────────────── */
typedef struct { pthread_key_t key; } _TrTLS;
static inline _TrTLS* _tr_tls_new(long long init) {
    _TrTLS* t = (_TrTLS*)malloc(sizeof(_TrTLS));
    pthread_key_create(&t->key, NULL);
    pthread_setspecific(t->key, (void*)(uintptr_t)(unsigned long long)init);
    return t;
}
static inline long long _tr_tls_get(_TrTLS* t) {
    return t ? (long long)(uintptr_t)pthread_getspecific(t->key) : 0LL;
}
static inline void _tr_tls_set(_TrTLS* t, long long v) {
    if (t) pthread_setspecific(t->key, (void*)(uintptr_t)(unsigned long long)v);
}
static inline void _tr_tls_free(_TrTLS* t) {
    if (!t) return; pthread_key_delete(t->key); free(t);
}

#endif /* POSIX async primitives */

/* ── MutexBox<T>: mutex-guarded single value ─────────────────────────── */
/* _Atomic int rc: thread-safe refcount via C11 atomics (stdatomic.h included above) */
/* _locked: 1 while this thread holds the lock; cleared by set_unlock/unlock.
 * Only one thread can hold the lock at a time so _locked is safe without
 * additional synchronization — it is always accessed under the mutex. */
typedef struct { _TrMutexH* mu; long long data; _Atomic int rc; volatile int _locked; } _TrMutexBox;
static inline _TrMutexBox* _tr_mutexbox_new(long long init) {
    _TrMutexBox* b = (_TrMutexBox*)TAURARO_ALLOC(sizeof(_TrMutexBox));
    b->mu = _tr_mutex_new(); b->data = init; b->_locked = 0;
    atomic_store(&b->rc, 1); return b;
}
static inline long long _tr_mutexbox_lock_get(_TrMutexBox* b) {
    _tr_mutex_hlock(b->mu); b->_locked = 1; return b->data;
}
static inline void _tr_mutexbox_set_unlock(_TrMutexBox* b, long long v) {
    b->data = v; b->_locked = 0; _tr_mutex_hunlock(b->mu);
}
static inline void _tr_mutexbox_unlock(_TrMutexBox* b) { b->_locked = 0; _tr_mutex_hunlock(b->mu); }
static inline void _tr_mutexbox_free(_TrMutexBox* b) {
    if (!b) return; _tr_mutex_hfree(b->mu); TAURARO_FREE(b);
}
static inline _TrMutexBox* _tr_mutexbox_clone(_TrMutexBox* b) {
    if (b) atomic_fetch_add(&b->rc, 1); return b;
}
static inline void _tr_mutexbox_drop(_TrMutexBox* b) {
    if (!b || atomic_fetch_sub(&b->rc, 1) > 1) return; _tr_mutexbox_free(b);
}
/* Auto-unlock cleanup — used by __attribute__((cleanup)) RAII guard in codegen.
 * Fires when the guard variable goes out of scope. No-op if already unlocked. */
static inline void _tr_mutexbox_cleanup(_TrMutexBox** bp) {
    if (bp && *bp && (*bp)->_locked) { (*bp)->_locked = 0; _tr_mutex_hunlock((*bp)->mu); }
}

/* ── RwLockBox<T>: reader-writer guarded single value ────────────────── */
/* _locked: 0=none, 1=write locked, 2=read locked. */
typedef struct { _TrRWL* rw; long long data; _Atomic int rc; volatile int _locked; } _TrRWLBox;
static inline _TrRWLBox* _tr_rwlbox_new(long long init) {
    _TrRWLBox* b = (_TrRWLBox*)TAURARO_ALLOC(sizeof(_TrRWLBox));
    b->rw = _tr_rwl_new(); b->data = init; b->_locked = 0;
    atomic_store(&b->rc, 1); return b;
}
static inline long long _tr_rwlbox_read_get(_TrRWLBox* b) {
    _tr_rwl_read_lock(b->rw); b->_locked = 2; return b->data;
}
static inline void _tr_rwlbox_read_unlock(_TrRWLBox* b) { b->_locked = 0; _tr_rwl_read_unlock(b->rw); }
static inline long long _tr_rwlbox_write_get(_TrRWLBox* b) {
    _tr_rwl_write_lock(b->rw); b->_locked = 1; return b->data;
}
static inline void _tr_rwlbox_write_set_unlock(_TrRWLBox* b, long long v) {
    b->data = v; b->_locked = 0; _tr_rwl_write_unlock(b->rw);
}
static inline void _tr_rwlbox_free(_TrRWLBox* b) {
    if (!b) return; _tr_rwl_free(b->rw); TAURARO_FREE(b);
}
static inline _TrRWLBox* _tr_rwlbox_clone(_TrRWLBox* b) {
    if (b) atomic_fetch_add(&b->rc, 1); return b;
}
static inline void _tr_rwlbox_drop(_TrRWLBox* b) {
    if (!b || atomic_fetch_sub(&b->rc, 1) > 1) return; _tr_rwlbox_free(b);
}
/* Auto-unlock cleanup for read/write guards. */
static inline void _tr_rwlbox_cleanup_r(_TrRWLBox** bp) {
    if (bp && *bp && (*bp)->_locked == 2) { (*bp)->_locked = 0; _tr_rwl_read_unlock((*bp)->rw); }
}
static inline void _tr_rwlbox_cleanup_w(_TrRWLBox** bp) {
    if (bp && *bp && (*bp)->_locked == 1) { (*bp)->_locked = 0; _tr_rwl_write_unlock((*bp)->rw); }
}

/* ── ThreadPool: fixed-N worker pool with a channel work queue ────────── */
/* BARE stub is defined inside the BARE platform block above. */
#ifndef TAURARO_BARE
typedef struct { void*(*fn)(void*); void* arg; } _TrPoolItem;
typedef struct {
    _TrChan* queue; _TrThread* workers; int n_workers;
    _TrWG* wg; volatile int shutdown;
} _TrThreadPool;
static void* _tr_pool_worker(void* arg) {
    _TrThreadPool* pool = (_TrThreadPool*)arg;
    for (;;) {
        int ok = 0;
        long long item_val = _tr_chan_recv_ok(pool->queue, &ok);
        if (!ok) break;
        /* uintptr_t cast is safe on both 32-bit and 64-bit platforms */
        _TrPoolItem* item = (_TrPoolItem*)(uintptr_t)(unsigned long long)item_val;
        item->fn(item->arg);
        TAURARO_FREE(item);
        _tr_wg_done(pool->wg);
    }
    return NULL;
}
static inline long long _tr_threadpool_auto_n(void) {
#ifdef _WIN32
    SYSTEM_INFO si; GetSystemInfo(&si); return (long long)si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
    /* _SC_NPROCESSORS_ONLN may not exist on all POSIX systems (Haiku, QNX, old BSDs) */
    long n = sysconf(_SC_NPROCESSORS_ONLN); return n > 0 ? (long long)n : 1LL;
#elif defined(HW_NCPU) /* BSD/macOS fallback via sysctl */
    int mib[2] = {CTL_HW, HW_NCPU}; int ncpu = 1; size_t len = sizeof(ncpu);
    sysctl(mib, 2, &ncpu, &len, NULL, 0); return (long long)(ncpu > 0 ? ncpu : 1);
#else
    return 1LL;
#endif
}
static inline _TrThreadPool* _tr_threadpool_new(long long n) {
    if (n < 1) n = 1;
    _TrThreadPool* p = (_TrThreadPool*)TAURARO_CALLOC(1, sizeof(_TrThreadPool));
    p->n_workers = (int)n;
    p->workers = (_TrThread*)TAURARO_ALLOC((size_t)n * sizeof(_TrThread));
    p->queue = _tr_chan_new(n * 4 + 16);
    p->wg = _tr_wg_new();
    for (int i = 0; i < (int)n; i++)
        p->workers[i] = _tr_thread_start(_tr_pool_worker, p);
    return p;
}
static inline _TrThreadPool* _tr_threadpool_auto(void) {
    return _tr_threadpool_new(_tr_threadpool_auto_n());
}
static inline void _tr_threadpool_spawn(_TrThreadPool* p, void*(*fn)(void*), void* arg) {
    _TrPoolItem* item = (_TrPoolItem*)TAURARO_ALLOC(sizeof(_TrPoolItem));
    item->fn = fn; item->arg = arg;
    _tr_wg_add(p->wg, 1);
    /* uintptr_t cast: safe on 32-bit and 64-bit; avoids sign-extension of intptr_t */
    _tr_chan_send(p->queue, (long long)(uintptr_t)(void*)item);
}
static inline void _tr_threadpool_wait(_TrThreadPool* p) { _tr_wg_wait(p->wg); }
static inline void _tr_threadpool_free(_TrThreadPool* p) {
    if (!p) return;
    _tr_chan_close(p->queue);
    for (int i = 0; i < p->n_workers; i++) _tr_thread_join_wait(p->workers[i]);
    _tr_chan_free(p->queue); _tr_wg_free(p->wg);
    TAURARO_FREE(p->workers); TAURARO_FREE(p);
}
#endif /* !TAURARO_BARE */

/* Global async pool — submits work to the thread pool; falls back to sync if pool is NULL */
static inline void _tr_async_pool_submit(_TrThreadPool* p, void*(*fn)(void*), void* arg) {
    if (p) _tr_threadpool_spawn(p, fn, arg);
    else fn(arg); /* synchronous fallback (BARE / pre-init) */
}

/* ── Atomic[T]: lock-free atomic integer (C11 _Atomic) ───────────────── */
typedef struct { _Atomic long long val; } _TrAtomic;
static inline _TrAtomic* _tr_atomic_new(long long init) {
    _TrAtomic* a = (_TrAtomic*)TAURARO_ALLOC(sizeof(_TrAtomic));
    atomic_init(&a->val, init); return a;
}
/* Hot-path ops: null-check removed — codegen never emits NULL _TrAtomic* */
static inline long long _tr_atomic_load(_TrAtomic* a)               { return atomic_load(&a->val); }
static inline void      _tr_atomic_store(_TrAtomic* a, long long v)  { atomic_store(&a->val, v); }
static inline long long _tr_atomic_add(_TrAtomic* a, long long v)    { return atomic_fetch_add(&a->val, v); }
static inline long long _tr_atomic_sub(_TrAtomic* a, long long v)    { return atomic_fetch_sub(&a->val, v); }
static inline long long _tr_atomic_swap(_TrAtomic* a, long long v)   { return atomic_exchange(&a->val, v); }
static inline bool _tr_atomic_cas(_TrAtomic* a, long long expected, long long desired) {
    return atomic_compare_exchange_strong(&a->val, &expected, desired);
}
static inline void _tr_atomic_free(_TrAtomic* a) { if (a) TAURARO_FREE(a); }

/* Atomic[T]: explicit memory-order variants (C11 stdatomic) */
static inline long long _tr_atomic_load_relaxed(_TrAtomic* a) { return atomic_load_explicit(&a->val, memory_order_relaxed); }
static inline long long _tr_atomic_load_acquire(_TrAtomic* a) { return atomic_load_explicit(&a->val, memory_order_acquire); }
static inline long long _tr_atomic_load_seqcst(_TrAtomic* a)  { return atomic_load_explicit(&a->val, memory_order_seq_cst); }
static inline void _tr_atomic_store_relaxed(_TrAtomic* a, long long v) { atomic_store_explicit(&a->val, v, memory_order_relaxed); }
static inline void _tr_atomic_store_release(_TrAtomic* a, long long v) { atomic_store_explicit(&a->val, v, memory_order_release); }
static inline void _tr_atomic_store_seqcst(_TrAtomic* a, long long v)  { atomic_store_explicit(&a->val, v, memory_order_seq_cst); }
static inline long long _tr_atomic_add_relaxed(_TrAtomic* a, long long v) { return atomic_fetch_add_explicit(&a->val, v, memory_order_relaxed); }
static inline long long _tr_atomic_add_release(_TrAtomic* a, long long v) { return atomic_fetch_add_explicit(&a->val, v, memory_order_release); }
static inline long long _tr_atomic_add_acqrel(_TrAtomic* a, long long v)  { return atomic_fetch_add_explicit(&a->val, v, memory_order_acq_rel); }
static inline long long _tr_atomic_sub_relaxed(_TrAtomic* a, long long v) { return atomic_fetch_sub_explicit(&a->val, v, memory_order_relaxed); }
static inline long long _tr_atomic_sub_release(_TrAtomic* a, long long v) { return atomic_fetch_sub_explicit(&a->val, v, memory_order_release); }
static inline bool _tr_atomic_cas_weak(_TrAtomic* a, long long exp, long long des)   { return atomic_compare_exchange_weak(&a->val, &exp, des); }
static inline bool _tr_atomic_cas_acqrel(_TrAtomic* a, long long exp, long long des) {
    return atomic_compare_exchange_strong_explicit(&a->val, &exp, des, memory_order_acq_rel, memory_order_relaxed);
}

/* ── Thread object: joinable OS-thread handle with panic recovery ──── */
typedef struct {
    _TrThread     handle;
    volatile int  done;
    _TrSpawnResult result; /* filled by trampoline on thread exit */
} _TrThreadObj;

/* Internal: thread_start variant that wires result into the trampoline. */
#ifdef _WIN32
static inline _TrThread _tr_thread_start_result(void*(*fn)(void*), void* arg, _TrSpawnResult* res) {
    _TrWin32StartArg* s = (_TrWin32StartArg*)malloc(sizeof(_TrWin32StartArg));
    s->fn = fn; s->arg = arg; s->result = res;
    SIZE_T ss = (SIZE_T)TAURARO_THREAD_STACK_SIZE;
    return CreateThread(NULL, ss, _tr_thread_start_trampoline, s, 0, NULL);
}
#elif !defined(TAURARO_BARE)
static inline _TrThread _tr_thread_start_result(void*(*fn)(void*), void* arg, _TrSpawnResult* res) {
    _TrPosixStartArg* s = (_TrPosixStartArg*)malloc(sizeof(_TrPosixStartArg));
    s->fn = fn; s->arg = arg; s->result = res;
    pthread_attr_t attr; pthread_attr_init(&attr);
    if (TAURARO_THREAD_STACK_SIZE > 0)
        pthread_attr_setstacksize(&attr, (size_t)TAURARO_THREAD_STACK_SIZE);
    pthread_attr_setguardsize(&attr, 4096);
    pthread_t t; pthread_create(&t, &attr, _tr_posix_thread_trampoline, s);
    pthread_attr_destroy(&attr); return t;
}
#else
static inline _TrThread _tr_thread_start_result(void*(*fn)(void*), void* arg, _TrSpawnResult* res) {
    (void)res; return _tr_thread_start(fn, arg);
}
#endif

static inline _TrThreadObj* _tr_threadobj_spawn(void*(*fn)(void*), void* arg) {
    _TrThreadObj* t = (_TrThreadObj*)TAURARO_CALLOC(1, sizeof(_TrThreadObj));
    t->result.panicked = 0; t->result.panic_msg = NULL;
    t->handle = _tr_thread_start_result(fn, arg, &t->result);
    return t;
}
static inline void _tr_threadobj_join(_TrThreadObj* t) {
    if (!t || t->done) return; t->done = 1; _tr_thread_join_wait(t->handle);
}
/* Re-raise the thread's panic in the calling thread after join */
static inline bool _tr_threadobj_panicked(_TrThreadObj* t) {
    return t && t->result.panicked;
}
static inline char* _tr_threadobj_panic_msg(_TrThreadObj* t) {
    return (t && t->result.panic_msg) ? t->result.panic_msg : "";
}
static inline void _tr_threadobj_detach(_TrThreadObj* t) {
    if (!t || t->done) return; t->done = 1; _tr_thread_detach(t->handle);
}
static inline void _tr_threadobj_free(_TrThreadObj* t) { if (t) TAURARO_FREE(t); }

/* ── Thread utilities: current-thread ID and sleep ───────────────────── */
#ifdef _WIN32
static inline long long _tr_thread_current_id(void) { return (long long)(uintptr_t)GetCurrentThreadId(); }
#elif defined(TAURARO_BARE)
static inline long long _tr_thread_current_id(void) { return 0LL; }
#else
static inline long long _tr_thread_current_id(void) { return (long long)(uintptr_t)pthread_self(); }
#endif
static inline void _tr_thread_sleep_ms(long long ms) { _tr_sleep_ms((long)(ms < 0 ? 0 : ms)); }

/* Monotonic millisecond clock — used by chan_select timeout */
static inline long long _tr_monotonic_ms(void) {
#if defined(_WIN32)
    return (long long)GetTickCount64();
#elif defined(TAURARO_BARE)
    return 0LL;
#else
    struct timespec _ts; clock_gettime(CLOCK_MONOTONIC, &_ts);
    return (long long)_ts.tv_sec * 1000LL + (long long)(_ts.tv_nsec / 1000000LL);
#endif
}

/* ── Platform-independent helpers ────────────────────────────────────── */
static bool _tr_task_await_timeout_ok(_TrTaskState* t, long long ms) {
    long long dummy=0; return _tr_task_await_timeout(t, ms, &dummy);
}

/* ── char* handle wrappers (used by Tauraro extern "C" declarations) ─── *
 * All struct* are cast to/from char* so Tauraro's Pointer[char] type      *
 * matches the C extern prototype without GCC type-mismatch warnings.       */

/* Channel */
static inline char* _tr_chan_new_h(long long cap)                              { return (char*)_tr_chan_new(cap); }
static inline void  _tr_chan_send_h(char* c, long long v)                      { _tr_chan_send((_TrChan*)c, v); }
static inline long long _tr_chan_recv_h(char* c)                               { return _tr_chan_recv((_TrChan*)c); }
static inline bool  _tr_chan_try_send_h(char* c, long long v)                  { return _tr_chan_try_send((_TrChan*)c, v); }
static inline long long _tr_chan_try_recv_val_h(char* c)                       { return _tr_chan_try_recv_val((_TrChan*)c); }
static inline bool  _tr_chan_send_timeout_h(char* c, long long v, long long ms){ return _tr_chan_send_timeout((_TrChan*)c, v, ms); }
static inline long long _tr_chan_recv_timeout_val_h(char* c, long long ms)     { return _tr_chan_recv_timeout_val((_TrChan*)c, ms); }
static inline void  _tr_chan_close_h(char* c)                                  { _tr_chan_close((_TrChan*)c); }
static inline bool  _tr_chan_is_closed_h(char* c)                              { return _tr_chan_is_closed((_TrChan*)c); }
static inline long long _tr_chan_len_h(char* c)                                { return _tr_chan_len((_TrChan*)c); }
static inline long long _tr_chan_cap_h(char* c)                                { return _tr_chan_cap((_TrChan*)c); }
static inline void  _tr_chan_free_h(char* c)                                   { _tr_chan_free((_TrChan*)c); }

/* Task / Future */
static inline char* _tr_task_new_h(void)                                       { return (char*)_tr_task_new(); }
static inline void  _tr_task_complete_h(char* t, long long r)                  { _tr_task_complete((_TrTaskState*)t, r); }
static inline void  _tr_task_complete_err_h(char* t, char* msg)                { _tr_task_complete_err((_TrTaskState*)t, msg); }
static inline void  _tr_task_cancel_h(char* t)                                 { _tr_task_cancel((_TrTaskState*)t); }
static inline long long _tr_task_await_h(char* t)                              { return _tr_task_await((_TrTaskState*)t); }
static inline bool  _tr_task_await_timeout_h(char* t, long long ms)            { return _tr_task_await_timeout_ok((_TrTaskState*)t, ms); }
static inline bool  _tr_task_is_done_h(char* t)                                { return _tr_task_is_done((_TrTaskState*)t); }
static inline bool  _tr_task_is_cancelled_h(char* t)                           { return _tr_task_is_cancelled((_TrTaskState*)t); }
static inline bool  _tr_task_has_error_h(char* t)                              { return _tr_task_has_error((_TrTaskState*)t); }
static inline char* _tr_task_get_error_h(char* t)                              { return _tr_task_get_error((_TrTaskState*)t); }
static inline void  _tr_task_free_h(char* t)                                   { _tr_task_free((_TrTaskState*)t); }

/* Mutex / RWLock */
static inline char* _tr_mutex_new_h(void)                                      { return (char*)_tr_mutex_new(); }
static inline void  _tr_mutex_lock_h(char* m)                                  { _tr_mutex_hlock((_TrMutexH*)m); }
static inline void  _tr_mutex_unlock_h(char* m)                                { _tr_mutex_hunlock((_TrMutexH*)m); }
static inline bool  _tr_mutex_trylock_h(char* m)                               { return _tr_mutex_htrylock((_TrMutexH*)m); }
static inline void  _tr_mutex_free_h(char* m)                                  { _tr_mutex_hfree((_TrMutexH*)m); }
static inline char* _tr_rwl_new_h(void)                                        { return (char*)_tr_rwl_new(); }
static inline void  _tr_rwl_read_lock_h(char* r)                               { _tr_rwl_read_lock((_TrRWL*)r); }
static inline void  _tr_rwl_read_unlock_h(char* r)                             { _tr_rwl_read_unlock((_TrRWL*)r); }
static inline void  _tr_rwl_write_lock_h(char* r)                              { _tr_rwl_write_lock((_TrRWL*)r); }
static inline void  _tr_rwl_write_unlock_h(char* r)                            { _tr_rwl_write_unlock((_TrRWL*)r); }
static inline void  _tr_rwl_free_h(char* r)                                    { _tr_rwl_free((_TrRWL*)r); }

/* Semaphore */
static inline char* _tr_sema_new_h(long long init, long long maxv)             { return (char*)_tr_sema_new(init, maxv); }
static inline void  _tr_sema_acquire_h(char* s)                                { _tr_sema_acquire((_TrSema*)s); }
static inline bool  _tr_sema_try_acquire_h(char* s)                            { return _tr_sema_try_acquire((_TrSema*)s); }
static inline bool  _tr_sema_acquire_timeout_h(char* s, long long ms)          { return _tr_sema_acquire_timeout((_TrSema*)s, ms); }
static inline void  _tr_sema_release_h(char* s)                                { _tr_sema_release((_TrSema*)s); }
static inline void  _tr_sema_free_h(char* s)                                   { _tr_sema_free((_TrSema*)s); }

/* WaitGroup */
static inline char* _tr_wg_new_h(void)                                         { return (char*)_tr_wg_new(); }
static inline void  _tr_wg_add_h(char* w, long long n)                         { _tr_wg_add((_TrWG*)w, n); }
static inline void  _tr_wg_done_h(char* w)                                     { _tr_wg_done((_TrWG*)w); }
static inline void  _tr_wg_wait_h(char* w)                                     { _tr_wg_wait((_TrWG*)w); }
static inline bool  _tr_wg_wait_timeout_h(char* w, long long ms)               { return _tr_wg_wait_timeout((_TrWG*)w, ms); }
static inline void  _tr_wg_free_h(char* w)                                     { _tr_wg_free((_TrWG*)w); }

/* Barrier */
static inline char* _tr_barrier_new_h(long long n)                             { return (char*)_tr_barrier_new(n); }
static inline void  _tr_barrier_wait_h(char* b)                                { _tr_barrier_wait((_TrBarrier*)b); }
static inline void  _tr_barrier_free_h(char* b)                                { _tr_barrier_free((_TrBarrier*)b); }

/* Once */
static inline char* _tr_once_new_h(void)                                       { return (char*)_tr_once_new(); }
static inline bool  _tr_once_do_h(char* o)                                     { return _tr_once_do((_TrOnce*)o); }
static inline void  _tr_once_free_h(char* o)                                   { _tr_once_free((_TrOnce*)o); }

/* Timer / Ticker */
static inline char* _tr_timer_new_h(long long ms, char* ch)                    { return (char*)_tr_timer_new(ms, (_TrChan*)ch); }
static inline char* _tr_ticker_new_h(long long ms, char* ch)                   { return (char*)_tr_ticker_new(ms, (_TrChan*)ch); }
static inline void  _tr_timer_stop_h(char* s)                                  { _tr_timer_stop((_TrTimerState*)s); }

/* Thread object (joinable handle) */
typedef void*(*_TrThreadFn)(void*);
static inline char* _tr_threadobj_spawn_h(char* fn, char* arg)                 { return (char*)_tr_threadobj_spawn((_TrThreadFn)(uintptr_t)fn, (void*)arg); }
static inline void  _tr_threadobj_join_h(char* t)                              { _tr_threadobj_join((_TrThreadObj*)t); }
static inline void  _tr_threadobj_detach_h(char* t)                            { _tr_threadobj_detach((_TrThreadObj*)t); }
static inline void  _tr_threadobj_free_h(char* t)                              { _tr_threadobj_free((_TrThreadObj*)t); }
static inline bool  _tr_threadobj_panicked_h(char* t)                          { return _tr_threadobj_panicked((_TrThreadObj*)t); }
static inline char* _tr_threadobj_panic_msg_h(char* t)                         { return _tr_threadobj_panic_msg((_TrThreadObj*)t); }
static inline long long _tr_thread_current_id_h(void)                          { return _tr_thread_current_id(); }
static inline void  _tr_thread_sleep_ms_h(long long ms)                        { _tr_thread_sleep_ms(ms); }

/* Atomic[T]: lock-free integer */
static inline char* _tr_atomic_new_h(long long init)                           { return (char*)_tr_atomic_new(init); }
static inline long long _tr_atomic_load_h(char* a)                             { return _tr_atomic_load((_TrAtomic*)a); }
static inline void  _tr_atomic_store_h(char* a, long long v)                   { _tr_atomic_store((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_h(char* a, long long v)                 { return _tr_atomic_add((_TrAtomic*)a, v); }
static inline long long _tr_atomic_sub_h(char* a, long long v)                 { return _tr_atomic_sub((_TrAtomic*)a, v); }
static inline long long _tr_atomic_swap_h(char* a, long long v)                { return _tr_atomic_swap((_TrAtomic*)a, v); }
static inline bool  _tr_atomic_cas_h(char* a, long long expected, long long desired) { return _tr_atomic_cas((_TrAtomic*)a, expected, desired); }
static inline void  _tr_atomic_free_h(char* a)                                 { _tr_atomic_free((_TrAtomic*)a); }
static inline long long _tr_atomic_load_relaxed_h(char* a)                     { return _tr_atomic_load_relaxed((_TrAtomic*)a); }
static inline long long _tr_atomic_load_acquire_h(char* a)                     { return _tr_atomic_load_acquire((_TrAtomic*)a); }
static inline long long _tr_atomic_load_seqcst_h(char* a)                      { return _tr_atomic_load_seqcst((_TrAtomic*)a); }
static inline void  _tr_atomic_store_relaxed_h(char* a, long long v)           { _tr_atomic_store_relaxed((_TrAtomic*)a, v); }
static inline void  _tr_atomic_store_release_h(char* a, long long v)           { _tr_atomic_store_release((_TrAtomic*)a, v); }
static inline void  _tr_atomic_store_seqcst_h(char* a, long long v)            { _tr_atomic_store_seqcst((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_relaxed_h(char* a, long long v)         { return _tr_atomic_add_relaxed((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_release_h(char* a, long long v)         { return _tr_atomic_add_release((_TrAtomic*)a, v); }
static inline long long _tr_atomic_add_acqrel_h(char* a, long long v)          { return _tr_atomic_add_acqrel((_TrAtomic*)a, v); }
static inline long long _tr_atomic_sub_relaxed_h(char* a, long long v)         { return _tr_atomic_sub_relaxed((_TrAtomic*)a, v); }
static inline long long _tr_atomic_sub_release_h(char* a, long long v)         { return _tr_atomic_sub_release((_TrAtomic*)a, v); }
static inline bool  _tr_atomic_cas_weak_h(char* a, long long exp, long long des)   { return _tr_atomic_cas_weak((_TrAtomic*)a, exp, des); }
static inline bool  _tr_atomic_cas_acqrel_h(char* a, long long exp, long long des) { return _tr_atomic_cas_acqrel((_TrAtomic*)a, exp, des); }

/* ThreadLocal[T]: per-thread storage */
static inline char* _tr_tls_new_h(long long init)                              { return (char*)_tr_tls_new(init); }
static inline long long _tr_tls_get_h(char* t)                                 { return _tr_tls_get((_TrTLS*)t); }
static inline void  _tr_tls_set_h(char* t, long long v)                        { _tr_tls_set((_TrTLS*)t, v); }
static inline void  _tr_tls_free_h(char* t)                                    { _tr_tls_free((_TrTLS*)t); }

/* ── Core runtime helpers ────────────────────────────────────────────── */

static char* input(const char* prompt) {
    if (prompt) printf("%s", prompt);
    char* buf = (char*)malloc(256);
    if (fgets(buf, 256, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        return buf;
    }
    return "";
}
static char* _tr_read_line(const char* prompt) {
    if (prompt && prompt[0]) printf("%s", prompt);
    char* buf = (char*)malloc(256);
    if (fgets(buf, 256, stdin)) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0'; /* strip \r on Windows */
        return buf;
    }
    return (char*)"";
}
static void yield_val(void* v) { (void)v; }

/* ── LSP / stdio primitives ──────────────────────────────────────────────
 * Low-level stdin/stdout helpers for programs that speak JSON-RPC or
 * any line-framed protocol over stdio.  Declared here so pure-Tauraro
 * code can use them via  extern "C": def _tr_read_stdin_line() -> str
 * without needing a hand-written C shim file.
 * ──────────────────────────────────────────────────────────────────────── */
#ifdef _WIN32
#  ifndef _INC_IO
#    include <io.h>     /* _isatty, _fileno on Windows */
#  endif
#else
#  ifndef _UNISTD_H
#    include <unistd.h> /* isatty, STDIN_FILENO on Unix */
#  endif
#endif

/* Read one line from stdin.  Returns the raw line INCLUDING any trailing
 * \r\n so callers can distinguish a blank separator ("\r\n") from true
 * EOF ("").  Never strips — the Tauraro caller calls .trim() itself.   */
static char* _tr_read_stdin_line(void) {
    char* buf = (char*)malloc(8192);
    if (!buf) return (char*)"";
    if (!fgets(buf, 8192, stdin)) { free(buf); return (char*)""; }
    return buf;
}

/* Read exactly n raw bytes from stdin.  Returns a null-terminated heap
 * string.  Returns "" on EOF or error.                                  */
static char* _tr_read_stdin_bytes(int64_t n) {
    if (n <= 0) return (char*)"";
    char* buf = (char*)malloc((size_t)(n + 1));
    if (!buf) return (char*)"";
    size_t total = 0;
    while ((int64_t)total < n) {
        size_t got = fread(buf + total, 1, (size_t)(n - (int64_t)total), stdin);
        if (got == 0) break;
        total += got;
    }
    buf[total] = '\0';
    return buf;
}

/* Write s to stdout without an extra newline.  Used by JSON-RPC writers
 * that build the full frame (Content-Length: …\r\n\r\n{…}) themselves. */
static void _tr_write_stdout(const char* s) {
    if (s) fputs(s, stdout);
}

/* Flush stdout so the peer receives the message without waiting for
 * the kernel buffer to fill up.                                         */
static void _tr_flush_stdout(void) { fflush(stdout); }

/* 1 if stdin is an interactive terminal (not a pipe); 0 otherwise.
 * Used by LSP servers to print a helpful message when started manually. */
static int64_t _tr_stdin_isatty(void) {
#ifdef _WIN32
    return _isatty(_fileno(stdin)) ? 1 : 0;
#else
    return isatty(STDIN_FILENO) ? 1 : 0;
#endif
}


static inline char* _tr_str_substring(const char* s, int start, int end) {
    if (!s) return NULL;
    int len = (int)strlen(s);
    if (start < 0) start = 0;
    if (end > len) end = len;
    int sublen = end - start;
    if (sublen < 0) sublen = 0;
    char* res = (char*)_tr_checked_alloc(sublen + 1);
    memcpy(res, s + start, sublen);
    res[sublen] = '\0';
    return res;
}

static inline void _tr_exit(long long code) { exit((int)code); }

#if defined(TAURARO_BARE) && !defined(__wasi__)
static inline long long _tr_getpid(void) { return 0LL; }
#elif defined(_WIN32)
#ifndef _TR_PID_INCLUDED
#define _TR_PID_INCLUDED
#include <process.h>
#endif
static inline long long _tr_getpid(void) { return (long long)_getpid(); }
#else
#include <unistd.h>
#include <time.h>
static inline long long _tr_getpid(void) { return (long long)getpid(); }
#endif

#include <time.h>
static inline long long _tr_timestamp(void) { return (long long)time(NULL); }

/* High-resolution millisecond wall-clock: QueryPerformanceCounter on Windows,
   CLOCK_MONOTONIC on POSIX.  Used by std.sys.time.time_ms / elapsed_ms. */
static inline long long _tr_time_ms(void) {
#if defined(TAURARO_BARE) && !defined(__wasi__)
    return 0LL;
#elif defined(_WIN32)
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (long long)(count.QuadPart * 1000LL / freq.QuadPart);
#else
    struct timespec _ts;
    clock_gettime(CLOCK_MONOTONIC, &_ts);
    return (long long)_ts.tv_sec * 1000LL + (long long)_ts.tv_nsec / 1000000LL;
#endif
}

/* Enable ANSI/VT100 colour codes on Windows Terminal; no-op elsewhere. */
static inline void _tr_enable_vt100(void) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        GetConsoleMode(h, &mode);
        SetConsoleMode(h, mode | 0x0004 /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */);
    }
#endif
}

/* ══════════════════════════════════════════════════════════════════════════
 * _TrIOPoll — Async I/O readiness abstraction
 *
 * Unified API over platform-specific event demultiplexers:
 *   Linux:       epoll (default) + io_uring (opt-in: -DTAURARO_IO_URING)
 *   Windows:     IOCP (I/O Completion Ports)
 *   macOS/BSD:   kqueue
 *   BARE/kernel: polling stub (returns immediately, no OS call)
 *
 * Use:
 *   _TrIOPoll* p = _tr_iopoll_create();
 *   _tr_iopoll_add(p, fd, TAURARO_POLLIN, userdata);
 *   int n = _tr_iopoll_wait(p, events, 64, timeout_ms);
 *   for (int i = 0; i < n; i++) { ... events[i].userdata ... }
 *   _tr_iopoll_destroy(p);
 * ══════════════════════════════════════════════════════════════════════════ */

#define TAURARO_POLLIN   0x01u
#define TAURARO_POLLOUT  0x02u
#define TAURARO_POLLERR  0x04u
#define TAURARO_POLLHUP  0x08u

typedef struct {
    int      fd;
    uint32_t events;
    void*    userdata;
} _TrIOEvent;

#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
/* ── BARE/Kernel: polling stub (no OS event loop) ────────────────────── */
typedef struct { int _dummy; } _TrIOPoll;
static inline _TrIOPoll* _tr_iopoll_create(void) {
    return (_TrIOPoll*)TAURARO_CALLOC(1, sizeof(_TrIOPoll));
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) { if (p) TAURARO_FREE(p); }
static inline int  _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { (void)p;(void)fd;(void)ev;(void)ud; return 0; }
static inline int  _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { (void)p;(void)fd;(void)ev;(void)ud; return 0; }
static inline int  _tr_iopoll_del(_TrIOPoll* p, int fd)
    { (void)p;(void)fd; return 0; }
static inline int  _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* ev, int maxev, int timeout_ms)
    { (void)p;(void)ev;(void)maxev;(void)timeout_ms; return 0; }

#elif defined(_WIN32)
/* ── Windows: IOCP-backed _TrIOPoll ──────────────────────────────────── */
#ifndef _TR_NET_INCLUDED
#define _TR_NET_INCLUDED
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
typedef struct {
    HANDLE   iocp;
    void**   ud_map;
    int*     fd_map;
    int      map_cap;
    int      map_cnt;
} _TrIOPoll;
static inline _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    p->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    p->map_cap = 64;
    p->ud_map  = (void**)calloc((size_t)p->map_cap, sizeof(void*));
    p->fd_map  = (int*)calloc((size_t)p->map_cap, sizeof(int));
    return p;
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (!p) return;
    if (p->iocp) CloseHandle(p->iocp);
    free(p->ud_map); free(p->fd_map); free(p);
}
static inline int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    /* Associate socket with IOCP; completion key = index into ud_map */
    if (p->map_cnt >= p->map_cap) {
        p->map_cap *= 2;
        p->ud_map = (void**)realloc(p->ud_map, (size_t)p->map_cap * sizeof(void*));
        p->fd_map = (int*)realloc(p->fd_map, (size_t)p->map_cap * sizeof(int));
    }
    int idx = p->map_cnt++;
    p->ud_map[idx] = ud; p->fd_map[idx] = fd; (void)ev;
    CreateIoCompletionPort((HANDLE)(uintptr_t)fd, p->iocp, (ULONG_PTR)idx, 0);
    return 0;
}
static inline int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { return _tr_iopoll_add(p, fd, ev, ud); }
static inline int _tr_iopoll_del(_TrIOPoll* p, int fd)
    { (void)p;(void)fd; return 0; }
static inline int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
    if (!p || !out || maxev <= 0) return 0;
    OVERLAPPED_ENTRY entries[64];
    ULONG n = 0;
    DWORD ms = timeout_ms < 0 ? INFINITE : (DWORD)timeout_ms;
    if (!GetQueuedCompletionStatusEx(p->iocp, entries,
            (ULONG)(maxev < 64 ? maxev : 64), &n, ms, FALSE)) return 0;
    for (ULONG i = 0; i < n; i++) {
        int idx = (int)entries[i].lpCompletionKey;
        out[i].fd       = (idx >= 0 && idx < p->map_cnt) ? p->fd_map[idx] : -1;
        out[i].userdata = (idx >= 0 && idx < p->map_cnt) ? p->ud_map[idx] : NULL;
        out[i].events   = TAURARO_POLLIN | TAURARO_POLLOUT;
    }
    return (int)n;
}

#elif defined(__linux__)
/* ── Linux: epoll-backed _TrIOPoll ───────────────────────────────────── */
#include <sys/epoll.h>
#include <unistd.h>
typedef struct { int epfd; } _TrIOPoll;
static inline _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    p->epfd = epoll_create1(EPOLL_CLOEXEC);
    return p;
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (!p) return; if (p->epfd >= 0) close(p->epfd); free(p);
}
static inline int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct epoll_event e = {0};
    if (ev & TAURARO_POLLIN)  e.events |= EPOLLIN;
    if (ev & TAURARO_POLLOUT) e.events |= EPOLLOUT;
    e.data.ptr = ud;
    return epoll_ctl(p->epfd, EPOLL_CTL_ADD, fd, &e);
}
static inline int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct epoll_event e = {0};
    if (ev & TAURARO_POLLIN)  e.events |= EPOLLIN;
    if (ev & TAURARO_POLLOUT) e.events |= EPOLLOUT;
    e.data.ptr = ud;
    return epoll_ctl(p->epfd, EPOLL_CTL_MOD, fd, &e);
}
static inline int _tr_iopoll_del(_TrIOPoll* p, int fd) {
    if (!p) return -1;
    return epoll_ctl(p->epfd, EPOLL_CTL_DEL, fd, NULL);
}
static inline int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
    if (!p || !out || maxev <= 0) return 0;
    struct epoll_event evs[64];
    int n = epoll_wait(p->epfd, evs, maxev < 64 ? maxev : 64, timeout_ms);
    if (n <= 0) return 0;
    for (int i = 0; i < n; i++) {
        uint32_t e = 0;
        if (evs[i].events & EPOLLIN)  e |= TAURARO_POLLIN;
        if (evs[i].events & EPOLLOUT) e |= TAURARO_POLLOUT;
        if (evs[i].events & EPOLLERR) e |= TAURARO_POLLERR;
        if (evs[i].events & EPOLLHUP) e |= TAURARO_POLLHUP;
        out[i].fd       = -1; /* epoll doesn't return fd in event */
        out[i].events   = e;
        out[i].userdata = evs[i].data.ptr;
    }
    return n;
}

#if defined(TAURARO_IO_URING)
/* ── io_uring support (Linux ≥5.1, opt-in with -DTAURARO_IO_URING) ─── *
 * Provides zero-syscall-per-op submission/completion ring interface.   *
 * WARNING: incorrect ring usage can crash/oops the kernel. Only use   *
 * after thorough testing. The epoll backend is the safe default.       */
#include <liburing.h>
typedef struct { struct io_uring ring; } _TrIOUring;
static inline _TrIOUring* _tr_iouring_create(unsigned entries) {
    _TrIOUring* u = (_TrIOUring*)calloc(1, sizeof(_TrIOUring));
    if (io_uring_queue_init(entries ? entries : 256u, &u->ring, 0) < 0) {
        free(u); return NULL;
    }
    return u;
}
static inline void _tr_iouring_destroy(_TrIOUring* u) {
    if (!u) return; io_uring_queue_exit(&u->ring); free(u);
}
static inline struct io_uring_sqe* _tr_iouring_get_sqe(_TrIOUring* u) {
    return u ? io_uring_get_sqe(&u->ring) : NULL;
}
static inline int _tr_iouring_submit(_TrIOUring* u) {
    return u ? io_uring_submit(&u->ring) : -1;
}
static inline int _tr_iouring_wait_cqe(_TrIOUring* u, struct io_uring_cqe** cqe) {
    return u ? io_uring_wait_cqe(&u->ring, cqe) : -1;
}
static inline void _tr_iouring_cqe_seen(_TrIOUring* u, struct io_uring_cqe* cqe) {
    if (u) io_uring_cqe_seen(&u->ring, cqe);
}
#endif /* TAURARO_IO_URING */

#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
/* ── macOS/BSD: kqueue-backed _TrIOPoll ──────────────────────────────── */
#include <sys/event.h>
#include <unistd.h>
typedef struct { int kqfd; } _TrIOPoll;
static inline _TrIOPoll* _tr_iopoll_create(void) {
    _TrIOPoll* p = (_TrIOPoll*)calloc(1, sizeof(_TrIOPoll));
    p->kqfd = kqueue(); return p;
}
static inline void _tr_iopoll_destroy(_TrIOPoll* p) {
    if (!p) return; if (p->kqfd >= 0) close(p->kqfd); free(p);
}
static inline int _tr_iopoll_add(_TrIOPoll* p, int fd, uint32_t ev, void* ud) {
    if (!p) return -1;
    struct kevent changes[2]; int n = 0;
    if (ev & TAURARO_POLLIN)
        EV_SET(&changes[n++], (uintptr_t)fd, EVFILT_READ,  EV_ADD|EV_ENABLE, 0, 0, ud);
    if (ev & TAURARO_POLLOUT)
        EV_SET(&changes[n++], (uintptr_t)fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, ud);
    return kevent(p->kqfd, changes, n, NULL, 0, NULL);
}
static inline int _tr_iopoll_mod(_TrIOPoll* p, int fd, uint32_t ev, void* ud)
    { return _tr_iopoll_add(p, fd, ev, ud); }
static inline int _tr_iopoll_del(_TrIOPoll* p, int fd) {
    if (!p) return -1;
    struct kevent changes[2];
    EV_SET(&changes[0], (uintptr_t)fd, EVFILT_READ,  EV_DELETE, 0, 0, NULL);
    EV_SET(&changes[1], (uintptr_t)fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    return kevent(p->kqfd, changes, 2, NULL, 0, NULL);
}
static inline int _tr_iopoll_wait(_TrIOPoll* p, _TrIOEvent* out, int maxev, int timeout_ms) {
    if (!p || !out || maxev <= 0) return 0;
    struct kevent evs[64];
    struct timespec ts = { timeout_ms / 1000, (timeout_ms % 1000) * 1000000L };
    struct timespec* tsp = timeout_ms < 0 ? NULL : &ts;
    int n = kevent(p->kqfd, NULL, 0, evs, maxev < 64 ? maxev : 64, tsp);
    if (n <= 0) return 0;
    for (int i = 0; i < n; i++) {
        uint32_t e = 0;
        if (evs[i].filter == EVFILT_READ)  e |= TAURARO_POLLIN;
        if (evs[i].filter == EVFILT_WRITE) e |= TAURARO_POLLOUT;
        if (evs[i].flags & EV_ERROR)       e |= TAURARO_POLLERR;
        if (evs[i].flags & EV_EOF)         e |= TAURARO_POLLHUP;
        out[i].fd       = (int)evs[i].ident;
        out[i].events   = e;
        out[i].userdata = evs[i].udata;
    }
    return n;
}
#else
/* ── Fallback: no async I/O on unknown platform ───────────────────────── */
typedef struct { int _dummy; } _TrIOPoll;
static inline _TrIOPoll* _tr_iopoll_create(void) { return (_TrIOPoll*)calloc(1,sizeof(_TrIOPoll)); }
static inline void _tr_iopoll_destroy(_TrIOPoll* p) { if(p) free(p); }
static inline int  _tr_iopoll_add(_TrIOPoll* p,int fd,uint32_t ev,void* ud){(void)p;(void)fd;(void)ev;(void)ud;return -1;}
static inline int  _tr_iopoll_mod(_TrIOPoll* p,int fd,uint32_t ev,void* ud){(void)p;(void)fd;(void)ev;(void)ud;return -1;}
static inline int  _tr_iopoll_del(_TrIOPoll* p,int fd){(void)p;(void)fd;return -1;}
static inline int  _tr_iopoll_wait(_TrIOPoll* p,_TrIOEvent* ev,int m,int t){(void)p;(void)ev;(void)m;(void)t;return 0;}
#endif /* _TrIOPoll platform backends */

/* _tr_iopoll_wait_raw: Tauraro-callable version.
 * out_buf is a caller-allocated byte array; each slot is sizeof(_TrIOEvent).
 * Returns number of events written.  Tauraro code reads fd/events/userdata
 * at offsets 0/4/8 within each 16-byte slot. */
static inline int _tr_iopoll_wait_raw(char* p_raw, char* out_buf, int maxev, int timeout_ms) {
    _TrIOPoll* p = (_TrIOPoll*)p_raw;
    _TrIOEvent tmp[64];
    if (maxev > 64) maxev = 64;
    int n = _tr_iopoll_wait(p, tmp, maxev, timeout_ms);
    for (int i = 0; i < n; i++) {
        char* slot = out_buf + i * 16;
        int   fd   = tmp[i].fd;
        int   ev   = (int)tmp[i].events;
        int   ud   = (int)(long long)tmp[i].userdata;
        memcpy(slot + 0, &fd, 4);
        memcpy(slot + 4, &ev, 4);
        memcpy(slot + 8, &ud, 4);
    }
    return n;
}

/* IOPoll char*-typed _h wrappers for Tauraro Pointer[char] interop */
static inline char* _tr_iopoll_create_h(void)
    { return (char*)_tr_iopoll_create(); }
static inline void  _tr_iopoll_destroy_h(char* p)
    { _tr_iopoll_destroy((_TrIOPoll*)p); }
static inline int   _tr_iopoll_add_h(char* p, long long fd, long long ev, long long ud)
    { return _tr_iopoll_add((_TrIOPoll*)p,(int)fd,(uint32_t)ev,(void*)(uintptr_t)(unsigned long long)ud); }
static inline int   _tr_iopoll_mod_h(char* p, long long fd, long long ev, long long ud)
    { return _tr_iopoll_mod((_TrIOPoll*)p,(int)fd,(uint32_t)ev,(void*)(uintptr_t)(unsigned long long)ud); }
static inline int   _tr_iopoll_del_h(char* p, long long fd)
    { return _tr_iopoll_del((_TrIOPoll*)p,(int)fd); }

/* ── TCP socket helpers ─────────────────────────────────────────────── */
#if defined(TAURARO_BARE) || defined(TAURARO_WASM)
/* No networking on bare WASM or freestanding targets */
static inline int _tr_net_init(void)                                              { return -1; }
static inline int _tr_tcp_connect(const char* h, int p)                           { (void)h;(void)p; return -1; }
static inline int _tr_tcp_send(int fd, const char* d, int l)                      { (void)fd;(void)d;(void)l; return -1; }
static inline int _tr_tcp_recv(int fd, char* b, int c)                            { (void)fd;(void)b;(void)c; return -1; }
static inline void _tr_tcp_close(int fd)                                           { (void)fd; }
static inline int _tr_tcp_listen(const char* h, int p, int bl)                    { (void)h;(void)p;(void)bl; return -1; }
static inline int _tr_tcp_accept(int s)                                            { (void)s; return -1; }
static inline char* _tr_tcp_peer_addr(int fd)                                      { (void)fd; return (char*)""; }
static inline int _tr_udp_socket(void)                                             { return -1; }
static inline int _tr_udp_bind(int fd, int p)                                      { (void)fd;(void)p; return -1; }
static inline int _tr_udp_send_to(int fd, const char* d, int l, const char* h, int p) { (void)fd;(void)d;(void)l;(void)h;(void)p; return -1; }
static inline int _tr_udp_recv_from(int fd, char* b, int c, char* src)            { (void)fd;(void)b;(void)c;(void)src; return -1; }
static inline void _tr_udp_close(int fd)                                           { (void)fd; }
static inline char* _tr_dns_resolve(const char* host)                              { (void)host; return (char*)""; }
static inline char* _tr_dns_reverse(const char* ip)                                { (void)ip;  return (char*)""; }
#elif defined(_WIN32)
#ifndef _TR_NET_INCLUDED
#define _TR_NET_INCLUDED
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

static inline int _tr_net_init(void) {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,2), &wsa) == 0 ? 0 : -1;
}
static inline int _tr_tcp_connect(const char* host, int port) {
    _tr_net_init();
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    char port_buf[16]; sprintf(port_buf, "%d", port);
    if (getaddrinfo(host, port_buf, &hints, &res) != 0) return -1;
    SOCKET fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == INVALID_SOCKET) { freeaddrinfo(res); return -1; }
    if (connect(fd, res->ai_addr, (int)res->ai_addrlen) != 0) {
        closesocket(fd); freeaddrinfo(res); return -1;
    }
    freeaddrinfo(res);
    return (int)fd;
}
static inline int  _tr_tcp_send(int fd, const char* data, int len) { return send((SOCKET)fd, data, len, 0); }
static inline int  _tr_tcp_recv(int fd, char* buf, int cap)        { return recv((SOCKET)fd, buf, cap, 0); }
static inline void _tr_tcp_close(int fd)                           { closesocket((SOCKET)fd); }

#else  /* POSIX */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static inline int _tr_net_init(void) { return 0; }
static inline int _tr_tcp_connect(const char* host, int port) {
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    char port_buf[16]; sprintf(port_buf, "%d", port);
    if (getaddrinfo(host, port_buf, &hints, &res) != 0) return -1;
    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd < 0) { freeaddrinfo(res); return -1; }
    if (connect(fd, res->ai_addr, res->ai_addrlen) != 0) {
        close(fd); freeaddrinfo(res); return -1;
    }
    freeaddrinfo(res);
    return fd;
}
static inline int  _tr_tcp_send(int fd, const char* data, int len) { return (int)send(fd, data, (size_t)len, 0); }
static inline int  _tr_tcp_recv(int fd, char* buf, int cap)        { return (int)recv(fd, buf, (size_t)cap, 0); }
static inline void _tr_tcp_close(int fd)                           { close(fd); }
#endif

/* ── Platform detection ──────────────────────────────────────────────── */
static inline bool _tr_is_windows(void) {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

/* ── Directory operations (cross-platform) ──────────────────────────── */
#if defined(TAURARO_BARE) && !defined(__wasi__)
/* Bare targets with no filesystem */
static inline int   _tr_mkdir(const char* p)     { (void)p; return -1; }
static inline int   _tr_rmdir(const char* p)     { (void)p; return -1; }
static inline bool  _tr_dir_exists(const char* p){ (void)p; return false; }
static inline bool  _tr_is_dir(const char* p)    { (void)p; return false; }
static inline bool  _tr_is_file(const char* p)   { (void)p; return false; }
static inline void* _tr_opendir(const char* p)   { (void)p; return NULL; }
static inline char* _tr_readdir(void* h)         { (void)h; return (char*)""; }
static inline void  _tr_closedir(void* h)        { (void)h; }
#elif defined(_WIN32)
static inline int  _tr_mkdir(const char* path)     { return CreateDirectoryA(path, NULL) ? 0 : -1; }
static inline int  _tr_rmdir(const char* path)     { return RemoveDirectoryA(path) ? 0 : -1; }
static inline bool _tr_dir_exists(const char* path) {
    if (!path) return false;
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY));
}
static inline bool _tr_is_dir(const char* path)  { return _tr_dir_exists(path); }
static inline bool _tr_is_file(const char* path) {
    if (!path) return false;
    DWORD attr = GetFileAttributesA(path);
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}
typedef struct { HANDLE h; WIN32_FIND_DATAA ffd; int first; } _TrDir;
static inline void* _tr_opendir(const char* path) {
    if (!path) return NULL;
    _TrDir* d = (_TrDir*)malloc(sizeof(_TrDir));
    char pat[4096]; snprintf(pat, sizeof(pat), "%s\\*", path);
    d->h = FindFirstFileA(pat, &d->ffd); d->first = 1;
    if (d->h == INVALID_HANDLE_VALUE) { free(d); return NULL; }
    return (void*)d;
}
static inline char* _tr_readdir(void* handle) {
    _TrDir* d = (_TrDir*)handle;
    if (!d || d->h == INVALID_HANDLE_VALUE) return (char*)"";
    if (d->first) { d->first = 0; return strdup(d->ffd.cFileName); }
    if (FindNextFileA(d->h, &d->ffd)) return strdup(d->ffd.cFileName);
    return (char*)"";
}
static inline void _tr_closedir(void* handle) {
    _TrDir* d = (_TrDir*)handle;
    if (d) { if (d->h != INVALID_HANDLE_VALUE) FindClose(d->h); free(d); }
}
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
static inline int  _tr_mkdir(const char* path)     { return mkdir(path, 0755) == 0 ? 0 : -1; }
static inline int  _tr_rmdir(const char* path)     { return rmdir(path) == 0 ? 0 : -1; }
static inline bool _tr_dir_exists(const char* path) {
    if (!path) return false;
    struct stat st; return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}
static inline bool _tr_is_dir(const char* path)  { return _tr_dir_exists(path); }
static inline bool _tr_is_file(const char* path) {
    if (!path) return false;
    struct stat st; return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}
static inline void* _tr_opendir(const char* path)  { return (void*)opendir(path); }
static inline char* _tr_readdir(void* handle) {
    DIR* d = (DIR*)handle;
    if (!d) return (char*)"";
    struct dirent* e = readdir(d);
    return e ? strdup(e->d_name) : (char*)"";
}
static inline void _tr_closedir(void* handle)       { if (handle) closedir((DIR*)handle); }
#endif

/* ── File-system helpers ─────────────────────────────────────────────── */
static inline int  _tr_file_delete(const char* path)                     { return remove(path) == 0 ? 0 : -1; }
static inline int  _tr_file_rename(const char* old_p, const char* new_p) { return rename(old_p, new_p) == 0 ? 0 : -1; }
static inline long long _tr_file_size(const char* path) {
    if (!path) return -1LL;
    FILE* f = fopen(path, "rb"); if (!f) return -1LL;
    fseek(f, 0, SEEK_END); long long sz = (long long)ftell(f); fclose(f); return sz;
}

/* _tr_c_memset defined above */

static inline void _tr_bounds_check(long long i, size_t len) {
    if (__builtin_expect(i < 0 || (size_t)i >= len, 0)) {
        fprintf(stderr, "Index %lld out of bounds (length %zu)\n", i, len);
        abort();
    }
}

#ifdef _TR_MAIN
  #define _TR_GLOBAL
#else
  #define _TR_GLOBAL extern
#endif

/* Thread-local storage qualifier for per-thread exception stacks */
#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
#  define _TR_THREAD_LOCAL
#elif defined(_MSC_VER)
#  define _TR_THREAD_LOCAL __declspec(thread)
#elif defined(__GNUC__) || defined(__clang__)
#  define _TR_THREAD_LOCAL __thread
#else
#  define _TR_THREAD_LOCAL _Thread_local
#endif

/* argc/argv made available to std.sys.env at runtime. */
_TR_GLOBAL int    _tr_argc;
_TR_GLOBAL char** _tr_argv;

static inline long long _tr_get_argc(void)       { return (long long)_tr_argc; }
static inline char*     _tr_get_arg(long long n) { return (_tr_argv && n >= 0 && (int)n < _tr_argc) ? _tr_argv[(int)n] : (char*)""; }

/* ── TaskGroup: spawn threads + join all (dynamic, unlimited) ────────── */
typedef struct { _TrThread* ths; int count; int cap; } _TrTaskGroup;
_TR_GLOBAL _TrTaskGroup _tr_tg;
_TR_GLOBAL _TrThreadPool* _tr_global_async_pool;
static inline void _tr_tg_begin(void) {
    _tr_tg.cap = 16; _tr_tg.count = 0;
    _tr_tg.ths = (_TrThread*)TAURARO_ALLOC((size_t)_tr_tg.cap * sizeof(_TrThread));
}
static inline void _tr_tg_push(_TrThread t) {
    if (_tr_tg.count >= _tr_tg.cap) {
        _tr_tg.cap *= 2;
        _tr_tg.ths = (_TrThread*)TAURARO_REALLOC(_tr_tg.ths, (size_t)_tr_tg.cap * sizeof(_TrThread));
    }
    _tr_tg.ths[_tr_tg.count++] = t;
}
static inline void _tr_taskgroup_wait(void) {
    for (int i = 0; i < _tr_tg.count; i++) _tr_thread_join_wait(_tr_tg.ths[i]);
    if (_tr_tg.ths) { TAURARO_FREE(_tr_tg.ths); _tr_tg.ths = NULL; }
    _tr_tg.count = 0; _tr_tg.cap = 0;
}

/* ── Per-thread panic state (storage definitions for _TR_MAIN TU) ─── */
#if !defined(TAURARO_BARE) && !defined(TAURARO_KERNEL)
_TR_GLOBAL _TR_THREAD_LOCAL int     _tr_thread_has_panic_buf;
_TR_GLOBAL _TR_THREAD_LOCAL jmp_buf _tr_thread_panic_jmpbuf;
_TR_GLOBAL _TR_THREAD_LOCAL char*   _tr_thread_panic_message;
#endif

/* ── Exception stack (setjmp/longjmp based, per-thread) ─────────────── */

#define _TR_MAX_EXC 64
_TR_GLOBAL _TR_THREAD_LOCAL jmp_buf*  _tr_exc_bufs[_TR_MAX_EXC];
_TR_GLOBAL _TR_THREAD_LOCAL char**    _tr_exc_msgs[_TR_MAX_EXC];
_TR_GLOBAL _TR_THREAD_LOCAL int       _tr_exc_sp;

static void _tr_exc_push(jmp_buf* b, char** m) {
    if (_tr_exc_sp < _TR_MAX_EXC) {
        _tr_exc_bufs[_tr_exc_sp] = b;
        _tr_exc_msgs[_tr_exc_sp] = m;
        _tr_exc_sp++;
    }
}
static void _tr_exc_pop(void)  { if (_tr_exc_sp > 0) _tr_exc_sp--; }
static void _tr_exc_raise(char* msg) {
    if (_tr_exc_sp > 0) {
        _tr_exc_sp--;
        *_tr_exc_msgs[_tr_exc_sp] = msg;
        longjmp(*_tr_exc_bufs[_tr_exc_sp], 1);
    }
    /* No user try-handler: escalate to thread panic handler if in a spawned thread */
    if (_tr_thread_has_panic_buf) {
        _tr_thread_panic_message = msg;
        longjmp(_tr_thread_panic_jmpbuf, 1);
    }
    fprintf(stderr, "Unhandled exception: %s\n", msg ? msg : "(null)");
    abort();
}

/* ── String helpers ─────────────────────────────────────────────────── */

static char* _tr_str_concat(const char* a, const char* b) {
    if (!a) a=""; if (!b) b="";
    size_t la=strlen(a), lb=strlen(b);
    char* r=(char*)TAURARO_ALLOC(la+lb+1);
    memcpy(r,a,la); memcpy(r+la,b,lb+1);
    return r;
}
static char* _tr_str_upper(const char* s) {
    if (!s) return "";
    char* r=(char*)TAURARO_ALLOC(strlen(s)+1);
    for (int i=0; (r[i]=(char)toupper((unsigned char)s[i])) || s[i]; i++);
    return r;
}
static char* _tr_str_lower(const char* s) {
    if (!s) return "";
    char* r=(char*)TAURARO_ALLOC(strlen(s)+1);
    for (int i=0; (r[i]=(char)tolower((unsigned char)s[i])) || s[i]; i++);
    return r;
}
static bool _tr_str_contains(const char* s, const char* sub) {
    return s && sub && strstr(s, sub) != NULL;
}
static bool _tr_str_starts_with(const char* s, const char* pre) {
    return s && pre && strncmp(s, pre, strlen(pre)) == 0;
}
static bool _tr_str_ends_with(const char* s, const char* suf) {
    if (!s||!suf) return false;
    size_t sl=strlen(s), sufl=strlen(suf);
    return sl>=sufl && strcmp(s+sl-sufl,suf)==0;
}
static char* _tr_str_strip(const char* s) {
    if (!s) return "";
    while (isspace((unsigned char)*s)) s++;
    if (!*s) { char* e=(char*)TAURARO_ALLOC(1); *e='\0'; return e; }
    const char* end = s+strlen(s)-1;
    while (end>s && isspace((unsigned char)*end)) end--;
    size_t len=(size_t)(end-s+1);
    char* r=(char*)TAURARO_ALLOC(len+1); memcpy(r,s,len); r[len]='\0'; return r;
}
static char* _tr_str_replace(const char* s, const char* old, const char* nw) {
    if (!s||!old||!nw) return (char*)s;
    size_t sl=strlen(s), ol=strlen(old), nl=strlen(nw);
    int cnt=0; const char* p=s;
    while ((p=strstr(p,old))) { cnt++; p+=ol; }
    char* r=(char*)TAURARO_ALLOC(sl+(size_t)cnt*(nl>ol?nl-ol:0)+1);
    char* dst=r; p=s;
    while (*p) {
        if (strncmp(p,old,ol)==0) { memcpy(dst,nw,nl); dst+=nl; p+=ol; }
        else { *dst++=*p++; }
    }
    *dst='\0'; return r;
}
static char* _tr_int_to_str(long long n)   { char* b=(char*)TAURARO_ALLOC(32); snprintf(b,32,"%lld",n); return b; }
static char* _tr_float_to_str(double n)    { char* b=(char*)TAURARO_ALLOC(32); snprintf(b,32,"%g",n);   return b; }
static char* _tr_float_to_c_lit(double n)  { char* b=(char*)TAURARO_ALLOC(32); snprintf(b,32,"%.17g",n); return b; }
static char* _tr_bool_to_str(bool b)       { return b ? "true" : "false"; }

/* _TR_AUTO_STR — convert any scalar to char* for f-string / print with unknown type.
 * Uses _Generic so __auto_type variables work without an explicit type annotation.
 * Each branch is a distinct typed helper to avoid cross-type implicit-cast errors. */
static inline char* _tr__ll_s(long long x)          { return _tr_int_to_str(x); }
static inline char* _tr__ull_s(unsigned long long x) { return _tr_int_to_str((long long)x); }
static inline char* _tr__i32_s(int x)               { return _tr_int_to_str((long long)x); }
static inline char* _tr__u32_s(unsigned int x)       { return _tr_int_to_str((long long)x); }
static inline char* _tr__i16_s(short x)              { return _tr_int_to_str((long long)x); }
static inline char* _tr__u16_s(unsigned short x)     { return _tr_int_to_str((long long)x); }
static inline char* _tr__i8_s(signed char x)         { return _tr_int_to_str((long long)x); }
static inline char* _tr__u8_s(unsigned char x)       { return _tr_int_to_str((long long)x); }
static inline char* _tr__dbl_s(double x)             { return _tr_float_to_str(x); }
static inline char* _tr__flt_s(float x)              { return _tr_float_to_str((double)x); }
static inline char* _tr__bool_s(bool x)              { return x ? "true" : "false"; }
static inline char* _tr__ptr_s(void* x)              { return (char*)x; }
#define _TR_AUTO_STR(x) _Generic((x), \
    long long:          _tr__ll_s,  \
    unsigned long long: _tr__ull_s, \
    int:                _tr__i32_s, \
    unsigned int:       _tr__u32_s, \
    short:              _tr__i16_s, \
    unsigned short:     _tr__u16_s, \
    signed char:        _tr__i8_s,  \
    unsigned char:      _tr__u8_s,  \
    double:             _tr__dbl_s, \
    float:              _tr__flt_s, \
    bool:               _tr__bool_s,\
    default:            _tr__ptr_s  \
)(x)
static long long _tr_str_to_int(const char* s) { return s ? strtoll(s,NULL,10) : 0LL; }
static double    _tr_str_to_float(const char* s){ return s ? strtod(s,NULL) : 0.0; }
static long long _tr_strlen(char* s)     { return s ? (long long)strlen(s) : 0LL; }

/* ── String equality ─────────────────────────────────────────────────── */
static inline bool _tr_str_eq(const char* a, const char* b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return strcmp(a, b) == 0;
}

/* ── String slice (alias for _tr_str_substring) ─────────────────────── */
static inline char* _tr_str_slice(const char* s, long long start, long long end) {
    if (!s) return (char*)"";
    long long len = (long long)strlen(s);
    if (start < 0) start = 0;
    if (end > len) end = len;
    if (start >= end) { char* e=(char*)_tr_checked_alloc(1); e[0]='\0'; return e; }
    long long sz = end - start;
    char* out = (char*)_tr_checked_alloc(sz + 1);
    memcpy(out, s + start, (size_t)sz);
    out[sz] = '\0';
    return out;
}

/* ── Additional string helpers ───────────────────────────────────────── */
static inline char* _tr_str_trim_left(const char* s) {
    if (!s) return (char*)"";
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    size_t n = strlen(s); char* r = (char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1); return r;
}
static inline char* _tr_str_trim_right(const char* s) {
    if (!s) return (char*)"";
    size_t n = strlen(s); const char* e = s+n-1;
    while (n > 0 && (*e==' '||*e=='\t'||*e=='\n'||*e=='\r')) { e--; n--; }
    char* r = (char*)_tr_checked_alloc(n+1); memcpy(r,s,n); r[n]='\0'; return r;
}
static inline char* _tr_str_capitalize(const char* s) {
    if (!s||!*s) return (char*)"";
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1);
    r[0]=(char)toupper((unsigned char)r[0]); for(size_t i=1;i<n;i++) r[i]=(char)tolower((unsigned char)r[i]);
    return r;
}
static inline char* _tr_str_title(const char* s) {
    if (!s) return (char*)"";
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1);
    bool ws=true;
    for(size_t i=0;i<n;i++){
        if(r[i]==' '||r[i]=='\t'||r[i]=='\n'){ws=true;}
        else if(ws){r[i]=(char)toupper((unsigned char)r[i]);ws=false;}
        else{r[i]=(char)tolower((unsigned char)r[i]);}
    }
    return r;
}
static inline char* _tr_str_reverse(const char* s) {
    if (!s) return (char*)"";
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1);
    for(size_t i=0;i<n;i++) r[i]=s[n-1-i]; r[n]='\0'; return r;
}
static inline char* _tr_str_repeat(const char* s, long long times) {
    if (!s||times<=0) { char* e=(char*)_tr_checked_alloc(1); e[0]='\0'; return e; }
    size_t slen=strlen(s); size_t total=(size_t)times*slen;
    char* r=(char*)_tr_checked_alloc(total+1); r[0]='\0';
    for(long long i=0;i<times;i++) memcpy(r+i*slen, s, slen);
    r[total]='\0'; return r;
}
static inline char* _tr_str_replace_first(const char* s, const char* old_s, const char* new_s) {
    if (!s||!old_s||!new_s) return s ? (char*)s : (char*)"";
    const char* p=strstr(s,old_s); if(!p) { size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1); return r; }
    size_t ol=strlen(old_s), nl=strlen(new_s), pre=(size_t)(p-s), sl=strlen(s);
    char* r=(char*)_tr_checked_alloc(sl-ol+nl+1);
    memcpy(r,s,pre); memcpy(r+pre,new_s,nl); memcpy(r+pre+nl,p+ol,sl-pre-ol+1); return r;
}
static inline char* _tr_str_strip_prefix(const char* s, const char* pre) {
    if (!s||!pre) return s?(char*)s:(char*)"";
    size_t pl=strlen(pre);
    if (strncmp(s,pre,pl)==0) { size_t n=strlen(s)-pl; char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s+pl,n+1); return r; }
    size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); memcpy(r,s,n+1); return r;
}
static inline char* _tr_str_strip_suffix(const char* s, const char* suf) {
    if (!s||!suf) return s?(char*)s:(char*)"";
    size_t sl=strlen(s), sufl=strlen(suf);
    if (sl>=sufl && strcmp(s+sl-sufl,suf)==0) { char* r=(char*)_tr_checked_alloc(sl-sufl+1); memcpy(r,s,sl-sufl); r[sl-sufl]='\0'; return r; }
    char* r=(char*)_tr_checked_alloc(sl+1); memcpy(r,s,sl+1); return r;
}
static inline char* _tr_str_remove_char(const char* s, const char* ch) {
    if (!s||!ch||!*ch) return s?(char*)s:(char*)"";
    char c=ch[0]; size_t n=strlen(s); char* r=(char*)_tr_checked_alloc(n+1); size_t j=0;
    for(size_t i=0;i<n;i++) if(s[i]!=c) r[j++]=s[i]; r[j]='\0'; return r;
}
static inline long long _tr_str_index_of(const char* s, const char* sub) {
    if (!s||!sub) return -1LL;
    const char* p=strstr(s,sub); return p ? (long long)(p-s) : -1LL;
}
static inline long long _tr_str_last_index_of(const char* s, const char* sub) {
    if (!s||!sub||!*sub) return -1LL;
    size_t sl=strlen(s), subl=strlen(sub); long long last=-1LL;
    for(size_t i=0;i+subl<=sl;i++) if(strncmp(s+i,sub,subl)==0) last=(long long)i;
    return last;
}
static inline long long _tr_str_count_occ(const char* s, const char* sub) {
    if (!s||!sub||!*sub) return 0LL;
    size_t subl=strlen(sub); long long c=0; const char* p=s;
    while((p=strstr(p,sub))!=NULL){c++;p+=subl;} return c;
}
static inline long long _tr_str_char_at_code(const char* s, long long i) {
    if (!s) return -1LL; long long n=(long long)strlen(s);
    if (i<0||i>=n) return -1LL; return (long long)(unsigned char)s[i];
}
static inline bool _tr_str_contains_char(const char* s, long long c) {
    if (!s) return false; return strchr(s,(char)c)!=NULL;
}
static inline bool _tr_str_parse_bool(const char* s) {
    if (!s) return false;
    return strcmp(s,"true")==0||strcmp(s,"1")==0||strcmp(s,"yes")==0;
}
static inline bool _tr_str_is_digit(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isdigit((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_alpha(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isalpha((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_alnum(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isalnum((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_space(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(!isspace((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_upper(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(isalpha((unsigned char)*p)&&!isupper((unsigned char)*p)) return false; return true;
}
static inline bool _tr_str_is_lower(const char* s) {
    if (!s||!*s) return false; for(const char* p=s;*p;p++) if(isalpha((unsigned char)*p)&&!islower((unsigned char)*p)) return false; return true;
}
/* _tr_str_lines and _tr_str_words defined after _tr_str_split below */
static inline char* _tr_str_lpad(const char* s, long long width, const char* pad) {
    if (!s) s=""; if (!pad||!*pad) pad=" ";
    long long slen=(long long)strlen(s); if(slen>=width){size_t n=strlen(s);char*r=(char*)_tr_checked_alloc(n+1);memcpy(r,s,n+1);return r;}
    long long plen=width-slen; char* r=(char*)_tr_checked_alloc((size_t)(plen+slen+1));
    for(long long i=0;i<plen;i++) r[i]=pad[0]; memcpy(r+plen,s,(size_t)slen); r[plen+slen]='\0'; return r;
}
static inline char* _tr_str_rpad(const char* s, long long width, const char* pad) {
    if (!s) s=""; if (!pad||!*pad) pad=" ";
    long long slen=(long long)strlen(s); if(slen>=width){size_t n=strlen(s);char*r=(char*)_tr_checked_alloc(n+1);memcpy(r,s,n+1);return r;}
    long long plen=width-slen; char* r=(char*)_tr_checked_alloc((size_t)(plen+slen+1));
    memcpy(r,s,(size_t)slen); for(long long i=0;i<plen;i++) r[slen+i]=pad[0]; r[slen+plen]='\0'; return r;
}
static inline char* _tr_str_center(const char* s, long long width) {
    if (!s) s="";
    long long slen=(long long)strlen(s); if(slen>=width){size_t n=strlen(s);char*r=(char*)_tr_checked_alloc(n+1);memcpy(r,s,n+1);return r;}
    long long total=width-slen, left=total/2, right=total-left;
    char* r=(char*)_tr_checked_alloc((size_t)(width+1));
    for(long long i=0;i<left;i++) r[i]=' '; memcpy(r+left,s,(size_t)slen); for(long long i=0;i<right;i++) r[left+slen+i]=' '; r[width]='\0'; return r;
}

/* ── Char code → 1-char string ───────────────────────────────────────── */
static inline char* _tr_char_to_str(long long code) {
    char* s = (char*)_tr_checked_alloc(2);
    s[0] = (char)(code & 0xFF);
    s[1] = '\0';
    return s;
}
static inline char* _tr_char_to_str_alloc(long long code) { return _tr_char_to_str(code); }

/* ── Shell command execution ─────────────────────────────────────────── */
#ifdef TAURARO_BARE
static inline int _tr_system(const char* cmd) { (void)cmd; return -1; }
#else
static inline int _tr_system(const char* cmd) { return system(cmd); }
#endif

/* ── Panic / error ───────────────────────────────────────────────────── */
static inline void _tr_panic(const char* msg) {
    if (_tr_thread_has_panic_buf) {
        /* In a spawned thread: unwind to thread boundary, not abort() */
        _tr_thread_panic_message = (char*)msg;
        longjmp(_tr_thread_panic_jmpbuf, 1);
    }
    fprintf(stderr, "panic: %s\n", msg ? msg : "(null)");
    abort();
}

/* ── Generic contains (for `in` operator on strings) ────────────────── */
static inline bool _tr_contains(const char* haystack, const char* needle) {
    return haystack && needle && strstr(haystack, needle) != NULL;
}

/* ── Range iteration helper ──────────────────────────────────────────── */
/* Note: range() on for-loops is compiled to C for() loops directly.     */
/* This stub satisfies any residual reference in generic code paths.     */
static inline long long _tr_range(long long start, long long end, long long step) {
    (void)start; (void)end; (void)step;
    return 0LL;
}

/* ── Dict (hash map: str → void*) ───────────────────────────────────── */

typedef struct _DictNode { char* key; void* value; struct _DictNode* next; } _DictNode;
typedef struct { _DictNode** buckets; size_t cap; size_t len; } Dict;

static size_t _dict_hash(const char* k, size_t cap) {
    size_t h=5381; unsigned char c;
    while ((c=(unsigned char)*k++)) h=h*33+c;
    return h%cap;
}
static Dict* Dict_new(void) {
    Dict* d=(Dict*)malloc(sizeof(Dict));
    d->cap=16; d->len=0;
    d->buckets=(_DictNode**)calloc(16,sizeof(_DictNode*));
    return d;
}
static void Dict_set(Dict* d, char* key, void* val) {
    if (!d || !key) return;
    size_t i=_dict_hash(key,d->cap);
    _DictNode* n=d->buckets[i];
    while (n) { if (strcmp(n->key,key)==0) { n->value=val; return; } n=n->next; }
    _DictNode* nd=(_DictNode*)malloc(sizeof(_DictNode));
    nd->key=strdup(key); nd->value=val; nd->next=d->buckets[i]; d->buckets[i]=nd; d->len++;
}
static void*     Dict_get(Dict* d, char* key) {
    if (!d||!key) return NULL;
    size_t i=_dict_hash(key,d->cap);
    _DictNode* n=d->buckets[i];
    while (n) { if (strcmp(n->key,key)==0) return n->value; n=n->next; }
    return NULL;
}
static bool      Dict_has(Dict* d, char* key) { return Dict_get(d,key)!=NULL; }
static long long Dict_len(Dict* d)  { return d?(long long)d->len:0LL; }
static void      Dict_free(Dict* d) {
    if (!d) return;
    for (size_t i=0; i<d->cap; i++) {
        _DictNode* n=d->buckets[i];
        while (n) { _DictNode* nx=n->next; if(n->key) _tr_free(n->key); _tr_free(n); n=nx; }
    }
    _tr_free(d->buckets); _tr_free(d);
}

typedef Dict TrMap;
static inline TrMap* _tr_dict_new(long long cap) { (void)cap; return Dict_new(); }
static inline void   _tr_dict_set_impl(TrMap* d, char* k, void* v) { if(d) Dict_set(d,k,v); }
/* Macro: casts any type (pointer, bool, int) safely through uintptr_t to void* */
#define _tr_dict_set(d, k, v) _tr_dict_set_impl((d), (k), (void*)(uintptr_t)(v))
static inline void*  _tr_dict_get(TrMap* d, char* k) { return d?Dict_get(d,k):NULL; }
static inline bool   _tr_dict_contains(TrMap* d, char* k) { return d&&Dict_has(d,k); }
#define _tr_dict_remove(d, k) _tr_dict_set_impl((d), (k), NULL)
static inline long long _tr_dict_len(TrMap* d) { return Dict_len(d); }

/* ── Int-keyed Dict (Dict[int, V]) ────────────────────────────────────── */
typedef struct _TrIDictNode { long long key; void* value; struct _TrIDictNode* next; } _TrIDictNode;
typedef struct { _TrIDictNode** buckets; size_t cap; size_t len; } TrIDict;
static inline TrIDict* _tr_idict_new(long long cap_hint) {
    size_t cap = (size_t)(cap_hint > 8 ? cap_hint : 8);
    TrIDict* d = (TrIDict*)calloc(1, sizeof(TrIDict));
    d->buckets = (_TrIDictNode**)calloc(cap, sizeof(_TrIDictNode*));
    d->cap = cap; d->len = 0; return d;
}
static inline void _tr_idict_set_impl(TrIDict* d, long long k, void* v) {
    if (!d) return;
    size_t idx = (size_t)((unsigned long long)k % d->cap);
    _TrIDictNode* n = d->buckets[idx];
    while (n) { if (n->key == k) { n->value = v; return; } n = n->next; }
    _TrIDictNode* nd = (_TrIDictNode*)malloc(sizeof(_TrIDictNode));
    nd->key = k; nd->value = v; nd->next = d->buckets[idx];
    d->buckets[idx] = nd; d->len++;
}
#define _tr_idict_set(d, k, v) _tr_idict_set_impl((d), (k), (void*)(uintptr_t)(v))
static inline void* _tr_idict_get(TrIDict* d, long long k) {
    if (!d) return NULL;
    size_t idx = (size_t)((unsigned long long)k % d->cap);
    _TrIDictNode* n = d->buckets[idx];
    while (n) { if (n->key == k) return n->value; n = n->next; }
    return NULL;
}
static inline bool   _tr_idict_contains(TrIDict* d, long long k) { return _tr_idict_get(d,k) != NULL; }
static inline void   _tr_idict_remove(TrIDict* d, long long k) { _tr_idict_set_impl(d, k, NULL); }
static inline long long _tr_idict_len(TrIDict* d) { return d ? (long long)d->len : 0LL; }

/* ── Built-in Tuple (up to 8 elements, all stored as long long) ────────── */
typedef struct { long long data[8]; } TrTuple;

/* ── List types (bootstrap phase) ─────────────────────────────────── */

typedef struct { long long* __restrict__ data; size_t len; size_t capacity; } List_i64;
static inline List_i64* List_i64_new(void) { List_i64* l=(List_i64*)malloc(sizeof(List_i64)); l->data=(long long*)malloc(sizeof(long long)*8); l->len=0; l->capacity=8; return l; }
static inline void List_i64_append(List_i64* l, long long val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(long long*)realloc(l->data,sizeof(long long)*l->capacity); } l->data[l->len++]=val; }
static inline bool List_i64_contains(List_i64* l, long long val) { for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return true; } return false; }
static inline long long List_i64_pop(List_i64* l) { if(!l||l->len==0) return 0LL; l->len--; return l->data[l->len]; }
static inline void List_i64_set(List_i64* l, long long i, long long v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline long long List_i64_get(List_i64* l, long long i) { if(l&&(size_t)i<l->len) return l->data[i]; return 0LL; }
static inline void List_i64_free(List_i64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { double* __restrict__ data; size_t len; size_t capacity; } List_f64;
static inline List_f64* List_f64_new(void) { List_f64* l=(List_f64*)malloc(sizeof(List_f64)); l->data=(double*)malloc(sizeof(double)*8); l->len=0; l->capacity=8; return l; }
static inline void List_f64_append(List_f64* l, double val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(double*)realloc(l->data,sizeof(double)*l->capacity); } l->data[l->len++]=val; }
static inline double List_f64_pop(List_f64* l) { if(!l||l->len==0) return 0.0; l->len--; return l->data[l->len]; }
static inline void List_f64_free(List_f64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { char** data; size_t len; size_t capacity; } List_str;
static inline List_str* List_str_new(void) { List_str* l=(List_str*)malloc(sizeof(List_str)); l->data=(char**)malloc(sizeof(char*)*8); l->len=0; l->capacity=8; return l; }
static inline void List_str_append(List_str* l, char* val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(char**)realloc(l->data,sizeof(char*)*l->capacity); } l->data[l->len++]=val; }
static inline char* List_str_pop(List_str* l) { if(!l||l->len==0) return NULL; l->len--; return l->data[l->len]; }
static inline void List_str_free(List_str* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { void** data; size_t len; size_t capacity; } List_ptr;
static inline List_ptr* List_ptr_new(void) { List_ptr* l=(List_ptr*)malloc(sizeof(List_ptr)); l->data=(void**)malloc(sizeof(void*)*8); l->len=0; l->capacity=8; return l; }
static inline void List_ptr_append(List_ptr* l, void* val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(void**)realloc(l->data,sizeof(void*)*l->capacity); } l->data[l->len++]=val; }
static inline void* List_ptr_pop(List_ptr* l) { if(!l||l->len==0) return NULL; l->len--; return l->data[l->len]; }
static inline void List_ptr_free(List_ptr* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { _Bool* data; size_t len; size_t capacity; } List_bool;
static inline List_bool* List_bool_new(void) { List_bool* l=(List_bool*)malloc(sizeof(List_bool)); l->data=(_Bool*)malloc(sizeof(_Bool)*8); l->len=0; l->capacity=8; return l; }
static inline void List_bool_append(List_bool* l, _Bool val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(_Bool*)realloc(l->data,sizeof(_Bool)*l->capacity); } l->data[l->len++]=val; }
static inline _Bool List_bool_get(List_bool* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_bool_set(List_bool* l, long long i, _Bool v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_bool_free(List_bool* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { int8_t* data; size_t len; size_t capacity; } List_i8;
static inline List_i8* List_i8_new(void) { List_i8* l=(List_i8*)malloc(sizeof(List_i8)); l->data=(int8_t*)malloc(sizeof(int8_t)*8); l->len=0; l->capacity=8; return l; }
static inline void List_i8_append(List_i8* l, int8_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(int8_t*)realloc(l->data,sizeof(int8_t)*l->capacity); } l->data[l->len++]=val; }
static inline int8_t List_i8_get(List_i8* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_i8_set(List_i8* l, long long i, int8_t v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_i8_free(List_i8* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { int* data; size_t len; size_t capacity; } List_i32;
static inline List_i32* List_i32_new(void) { List_i32* l=(List_i32*)malloc(sizeof(List_i32)); l->data=(int*)malloc(sizeof(int)*8); l->len=0; l->capacity=8; return l; }
static inline void List_i32_append(List_i32* l, int val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(int*)realloc(l->data,sizeof(int)*l->capacity); } l->data[l->len++]=val; }
static inline int List_i32_get(List_i32* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_i32_set(List_i32* l, long long i, int v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_i32_free(List_i32* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { char* data; size_t len; size_t capacity; } List_char;
static inline List_char* List_char_new(void) { List_char* l=(List_char*)malloc(sizeof(List_char)); l->data=(char*)malloc(sizeof(char)*8); l->len=0; l->capacity=8; return l; }
static inline void List_char_append(List_char* l, char val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(char*)realloc(l->data,sizeof(char)*l->capacity); } l->data[l->len++]=val; }
static inline char List_char_get(List_char* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_char_set(List_char* l, long long i, char v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_char_free(List_char* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── Dict[str,V] key/value iteration (after List types are defined) ─────── */
static inline List_str* _tr_dict_keys(TrMap* d) {
    List_str* out = List_str_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _DictNode* n = d->buckets[i];
        while (n) { if (n->key && n->value) List_str_append(out, n->key); n = n->next; }
    }
    return out;
}
static inline List_ptr* _tr_dict_values(TrMap* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _DictNode* n = d->buckets[i];
        while (n) { if (n->key && n->value) List_ptr_append(out, n->value); n = n->next; }
    }
    return out;
}
static inline List_i64* _tr_idict_keys(TrIDict* d) {
    List_i64* out = List_i64_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _TrIDictNode* n = d->buckets[i];
        while (n) { if (n->value) List_i64_append(out, n->key); n = n->next; }
    }
    return out;
}
static inline List_ptr* _tr_idict_values(TrIDict* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _TrIDictNode* n = d->buckets[i];
        while (n) { if (n->value) List_ptr_append(out, n->value); n = n->next; }
    }
    return out;
}

/* Key-value pair structs for dict.items() */
typedef struct { char* key; void* val; } TrKVPair;
typedef struct { long long key; void* val; } TrIKVPair;

static inline List_ptr* _tr_dict_items(TrMap* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _DictNode* n = d->buckets[i];
        while (n) {
            TrKVPair* p = (TrKVPair*)malloc(sizeof(TrKVPair));
            p->key = n->key; p->val = n->value;
            List_ptr_append(out, p); n = n->next;
        }
    }
    return out;
}
static inline List_ptr* _tr_idict_items(TrIDict* d) {
    List_ptr* out = List_ptr_new();
    if (!d) return out;
    for (size_t i = 0; i < d->cap; i++) {
        _TrIDictNode* n = d->buckets[i];
        while (n) {
            TrIKVPair* p = (TrIKVPair*)malloc(sizeof(TrIKVPair));
            p->key = n->key; p->val = n->value;
            List_ptr_append(out, p); n = n->next;
        }
    }
    return out;
}

typedef struct { uint8_t* data; size_t len; size_t capacity; } List_u8;
static inline List_u8* List_u8_new(void) { List_u8* l=(List_u8*)malloc(sizeof(List_u8)); l->data=(uint8_t*)malloc(sizeof(uint8_t)*8); l->len=0; l->capacity=8; return l; }
static inline void List_u8_append(List_u8* l, uint8_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(uint8_t*)realloc(l->data,sizeof(uint8_t)*l->capacity); } l->data[l->len++]=val; }
static inline uint8_t List_u8_get(List_u8* l, long long i) { _tr_bounds_check(i, l->len); return l->data[i]; }
static inline void List_u8_set(List_u8* l, long long i, uint8_t v) { _tr_bounds_check(i, l->len); l->data[i] = v; }
static inline void List_u8_free(List_u8* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

static inline List_u8* _tr_bytes_new(const uint8_t* data, size_t len) {
    List_u8* l = (List_u8*)malloc(sizeof(List_u8));
    l->len = len;
    l->capacity = len > 0 ? len : 8;
    l->data = (uint8_t*)malloc(l->capacity);
    if (len > 0) memcpy(l->data, data, len);
    return l;
}

typedef struct { uint32_t* data; size_t len; size_t capacity; } List_u32;
static inline List_u32* List_u32_new(void) { List_u32* l=(List_u32*)malloc(sizeof(List_u32)); l->data=(uint32_t*)malloc(sizeof(uint32_t)*8); l->len=0; l->capacity=8; return l; }
static inline void List_u32_append(List_u32* l, uint32_t val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(uint32_t*)realloc(l->data,sizeof(uint32_t)*l->capacity); } l->data[l->len++]=val; }
static inline void List_u32_free(List_u32* l) { if(l){ _tr_free(l->data); _tr_free(l); } }
/* ── Extended Vec/List operations: remove, swap, clear, is_empty, extend ──── */
static inline void List_i64_remove(List_i64* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_i64_swap(List_i64* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; long long t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_i64_clear(List_i64* l) { if(l) l->len=0; }
static inline bool List_i64_is_empty(List_i64* l) { return !l||l->len==0; }
static inline void List_i64_extend(List_i64* l, List_i64* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_i64_append(l,o->data[i]); }
static inline long long List_i64_index_of(List_i64* l, long long v) { if(!l) return -1LL; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return (long long)i; return -1LL; }
static inline void List_f64_remove(List_f64* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_f64_swap(List_f64* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; double t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_f64_clear(List_f64* l) { if(l) l->len=0; }
static inline bool List_f64_is_empty(List_f64* l) { return !l||l->len==0; }
static inline void List_f64_extend(List_f64* l, List_f64* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_f64_append(l,o->data[i]); }
static inline bool List_f64_contains(List_f64* l, double v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline double List_f64_get(List_f64* l, long long i) { if(l&&(size_t)i<l->len) return l->data[i]; return 0.0; }
static inline void List_f64_set(List_f64* l, long long i, double v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline void List_str_remove(List_str* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_str_swap(List_str* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; char* t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_str_clear(List_str* l) { if(l) l->len=0; }
static inline bool List_str_is_empty(List_str* l) { return !l||l->len==0; }
static inline void List_str_extend(List_str* l, List_str* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_str_append(l,o->data[i]); }
static inline bool List_str_contains(List_str* l, char* v) { if(!l||!v) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]&&strcmp(l->data[i],v)==0) return true; return false; }
static inline long long List_str_index_of(List_str* l, char* v) { if(!l||!v) return -1LL; for(size_t i=0;i<l->len;i++) if(l->data[i]&&strcmp(l->data[i],v)==0) return (long long)i; return -1LL; }
static inline void List_ptr_remove(List_ptr* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_ptr_swap(List_ptr* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; void* t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_ptr_clear(List_ptr* l) { if(l) l->len=0; }
static inline bool List_ptr_is_empty(List_ptr* l) { return !l||l->len==0; }
static inline void List_ptr_extend(List_ptr* l, List_ptr* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_ptr_append(l,o->data[i]); }
static inline bool List_ptr_contains(List_ptr* l, void* v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline void List_bool_remove(List_bool* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_bool_swap(List_bool* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; _Bool t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_bool_clear(List_bool* l) { if(l) l->len=0; }
static inline bool List_bool_is_empty(List_bool* l) { return !l||l->len==0; }
static inline void List_bool_extend(List_bool* l, List_bool* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_bool_append(l,o->data[i]); }
static inline bool List_bool_contains(List_bool* l, _Bool v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline long long List_bool_pop(List_bool* l) { if(!l||l->len==0) return 0; l->len--; return l->data[l->len]; }
static inline void List_i8_remove(List_i8* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_i8_swap(List_i8* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; int8_t t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_i8_clear(List_i8* l) { if(l) l->len=0; }
static inline bool List_i8_is_empty(List_i8* l) { return !l||l->len==0; }
static inline void List_i8_extend(List_i8* l, List_i8* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_i8_append(l,o->data[i]); }
static inline bool List_i8_contains(List_i8* l, int8_t v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline int8_t List_i8_pop(List_i8* l) { if(!l||l->len==0) return 0; l->len--; return l->data[l->len]; }
static inline void List_i32_remove(List_i32* l, long long i) { if(!l||(size_t)i>=l->len) return; for(size_t j=(size_t)i;j<l->len-1;j++) l->data[j]=l->data[j+1]; l->len--; }
static inline void List_i32_swap(List_i32* l, long long a, long long b) { if(!l||(size_t)a>=l->len||(size_t)b>=l->len) return; int t=l->data[a]; l->data[a]=l->data[b]; l->data[b]=t; }
static inline void List_i32_clear(List_i32* l) { if(l) l->len=0; }
static inline bool List_i32_is_empty(List_i32* l) { return !l||l->len==0; }
static inline void List_i32_extend(List_i32* l, List_i32* o) { if(!l||!o) return; for(size_t i=0;i<o->len;i++) List_i32_append(l,o->data[i]); }
static inline bool List_i32_contains(List_i32* l, int v) { if(!l) return false; for(size_t i=0;i<l->len;i++) if(l->data[i]==v) return true; return false; }
static inline int List_i32_pop(List_i32* l) { if(!l||l->len==0) return 0; l->len--; return l->data[l->len]; }


typedef struct { long long* data; size_t len; size_t capacity; } Set_i64;
static inline Set_i64* Set_i64_new(void) { Set_i64* l=(Set_i64*)malloc(sizeof(Set_i64)); l->data=(long long*)malloc(sizeof(long long)*8); l->len=0; l->capacity=8; return l; }
static inline void Set_i64_add(Set_i64* l, long long val) { 
    for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(long long*)realloc(l->data,sizeof(long long)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_i64_free(Set_i64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { void** data; size_t len; size_t capacity; } Set_ptr;
static inline Set_ptr* Set_ptr_new(void) { Set_ptr* l=(Set_ptr*)malloc(sizeof(Set_ptr)); l->data=(void**)malloc(sizeof(void*)*8); l->len=0; l->capacity=8; return l; }
static inline void Set_ptr_add(Set_ptr* l, void* val) { 
    for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(void**)realloc(l->data,sizeof(void*)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_ptr_free(Set_ptr* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { char** data; size_t len; size_t capacity; } Set_str;
static inline Set_str* Set_str_new(void) { Set_str* l=(Set_str*)malloc(sizeof(Set_str)); l->data=(char**)malloc(sizeof(char*)*8); l->len=0; l->capacity=8; return l; }
static inline void Set_str_add(Set_str* l, char* val) { 
    for (size_t i = 0; i < l->len; i++) { if (strcmp(l->data[i], val) == 0) return; }
    if(l->len==l->capacity){ l->capacity*=2; l->data=(char**)realloc(l->data,sizeof(char*)*l->capacity); } l->data[l->len++]=val; 
}
static inline void Set_str_free(Set_str* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

/* ── Bounds-checked list access ─────────────────────────────────────────── */
static inline List_i64* _tr_range_new(long long start, long long stop, bool inclusive) {
    List_i64* l = List_i64_new();
    long long end = inclusive ? stop : stop - 1;
    for (long long i = start; i <= end; i++) { List_i64_append(l, i); }
    return l;
}
static inline long long _tr_list_i64_get(List_i64* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline long long List_i64_get_index(List_i64* l, long long i) { return _tr_list_i64_get(l, i); }
static inline void _tr_list_i64_set(List_i64* l, long long i, long long v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_i64_set_index(List_i64* l, long long i, long long v) { _tr_list_i64_set(l, i, v); }

static inline double _tr_list_f64_get(List_f64* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline double List_f64_get_index(List_f64* l, long long i) { return _tr_list_f64_get(l, i); }
static inline void _tr_list_f64_set(List_f64* l, long long i, double v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_f64_set_index(List_f64* l, long long i, double v) { _tr_list_f64_set(l, i, v); }

static inline char* _tr_list_str_get(List_str* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline char* List_str_get_index(List_str* l, long long i) { return _tr_list_str_get(l, i); }
static inline char* List_str_get(List_str* l, long long i) { return _tr_list_str_get(l, i); }
static inline void _tr_list_str_set(List_str* l, long long i, char* v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_str_set_index(List_str* l, long long i, char* v) { _tr_list_str_set(l, i, v); }
static inline void List_str_set(List_str* l, long long i, char* v) { _tr_list_str_set(l, i, v); }

/* Vec[str] — always available so main(args: Vec[str]) works without an explicit import. */
#ifndef _TR_VEC_STR_DEFINED
#define _TR_VEC_STR_DEFINED
typedef struct Vec_str Vec_str;
struct Vec_str { List_str* data; long long len; long long cap; };
static inline Vec_str* Vec_str_init(long long cap) {
    Vec_str* v = (Vec_str*)_tr_checked_alloc(sizeof(Vec_str));
    v->data = List_str_new(); v->len = 0; v->cap = cap > 0 ? cap : 8;
    return v;
}
static inline void Vec_str_push(Vec_str* v, char* s) { List_str_append(v->data, s); v->len++; }
static inline char* Vec_str_get(Vec_str* v, long long i) { return List_str_get(v->data, i); }
static inline long long Vec_str_len(Vec_str* v) { return v ? v->len : 0LL; }
#endif

static inline void* _tr_list_ptr_get(List_ptr* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void* List_ptr_get_index(List_ptr* l, long long i) { return _tr_list_ptr_get(l, i); }
static inline void* List_ptr_get(List_ptr* l, long long i) { return _tr_list_ptr_get(l, i); }
static inline void _tr_list_ptr_set(List_ptr* l, long long i, void* v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_ptr_set_index(List_ptr* l, long long i, void* v) { _tr_list_ptr_set(l, i, v); }
static inline void List_ptr_set(List_ptr* l, long long i, void* v) { _tr_list_ptr_set(l, i, v); }

static inline _Bool _tr_list_bool_get(List_bool* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline _Bool List_bool_get_index(List_bool* l, long long i) { return _tr_list_bool_get(l, i); }
static inline void _tr_list_bool_set(List_bool* l, long long i, _Bool v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline void List_bool_set_index(List_bool* l, long long i, _Bool v) { _tr_list_bool_set(l, i, v); }
static inline int8_t _tr_list_i8_get(List_i8* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_i8_set(List_i8* l, long long i, int8_t v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline int _tr_list_i32_get(List_i32* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_i32_set(List_i32* l, long long i, int v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline char _tr_list_char_get(List_char* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_char_set(List_char* l, long long i, char v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline uint8_t _tr_list_u8_get(List_u8* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_u8_set(List_u8* l, long long i, uint8_t v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}
static inline uint32_t _tr_list_u32_get(List_u32* l, long long i) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    return l->data[i];
}
static inline void _tr_list_u32_set(List_u32* l, long long i, uint32_t v) {
    if (!l) { fprintf(stderr, "Null list access\n"); abort(); }
    _tr_bounds_check(i, l->len);
    l->data[i] = v;
}

static inline char* _tr_str_join(List_str* parts, const char* sep) {
    if (!parts || parts->len == 0) return (char*)"";
    size_t total = 0, seplen = sep ? strlen(sep) : 0;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i]) total += strlen(parts->data[i]);
        if (i + 1 < parts->len) total += seplen;
    }
    char* out = (char*)_tr_checked_alloc(total + 1);
    char* dst = out;
    for (size_t i = 0; i < parts->len; i++) {
        if (parts->data[i]) { size_t l = strlen(parts->data[i]); memcpy(dst, parts->data[i], l); dst += l; }
        if (i + 1 < parts->len && seplen) { memcpy(dst, sep, seplen); dst += seplen; }
    }
    *dst = '\0';
    return out;
}

static inline List_str* _tr_str_split(const char* s, const char* sep) {
    List_str* l=List_str_new(); if(!s||!sep||!*sep) return l;
    char* cp=(char*)malloc(strlen(s)+1); strcpy(cp,s);
    char* tok=strtok(cp,(char*)sep);
    while(tok){ List_str_append(l,strdup(tok)); tok=strtok(NULL,(char*)sep); }
    _tr_free(cp); return l;
}
static inline List_str* _tr_str_lines(const char* s) { return _tr_str_split(s, "\n"); }
static inline List_str* _tr_str_words(const char* s) { return _tr_str_split(s, " "); }

/* ── Test runner helpers ─────────────────────────────────────────────── */

_TR_GLOBAL int _tr_tests_passed;
_TR_GLOBAL int _tr_tests_failed;

static void _tr_run_test(const char* name, void(*fn)(void)) {
    jmp_buf _buf;
    char* _msg = NULL;
    _tr_exc_push(&_buf, &_msg);
    if (setjmp(_buf) == 0) {
        fn();
        _tr_exc_pop();
        _tr_tests_passed++;
        printf("\033[32mPASS\033[0m %s\n", name);
    } else {
        _tr_tests_failed++;
        printf("\033[31mFAIL\033[0m %s: %s\n", name, _msg ? _msg : "panic");
    }
}

static int _tr_test_report(void) {
    int total = _tr_tests_passed + _tr_tests_failed;
    if (_tr_tests_failed == 0) {
        printf("\n\033[32m%d/%d tests passed\033[0m\n", _tr_tests_passed, total);
    } else {
        printf("\n%d/%d tests passed, \033[31m%d failed\033[0m\n",
               _tr_tests_passed, total, _tr_tests_failed);
    }
    return _tr_tests_failed > 0 ? 1 : 0;
}

#ifndef TAURARO_NO_RT_HELPERS
/* When std library is compiled in, it provides its own StringBuilder and
   file I/O — suppress the lightweight rt.h fallback implementations. */
#ifndef TAURARO_STD_LIB
/* ── StringBuilder (suppressed when std.core.string provides its own) ───── */
#ifndef TAURARO_RT_NO_STRINGBUILDER
/* OOP layout — matches std.core.string.StringBuilder: buf is StringObj* so that
 * the c.tr codegen's sb->buf->len accesses compile correctly. */
typedef struct core_string_StringObj { char* data; long long len; long long capacity; } core_string_StringObj;
typedef core_string_StringObj StringObj;
static inline StringObj* StringObj_init(char* s) {
    StringObj* obj = (StringObj*)_tr_checked_alloc(sizeof(StringObj));
    long long slen = s ? (long long)strlen(s) : 0LL;
    obj->len = slen; obj->capacity = slen + 8;
    obj->data = (char*)_tr_checked_alloc((size_t)(obj->capacity));
    if (slen > 0) memcpy(obj->data, s, (size_t)slen);
    obj->data[slen] = '\0';
    return obj;
}
static inline char* StringObj_as_str(StringObj* obj) { return obj->data; }
typedef struct core_string_StringBuilder { StringObj* buf; } core_string_StringBuilder;
typedef core_string_StringBuilder StringBuilder;

static inline StringBuilder* StringBuilder_init(long long cap) {
    if (cap < 16) cap = 16;
    StringBuilder* sb = (StringBuilder*)_tr_checked_alloc(sizeof(StringBuilder));
    sb->buf = (StringObj*)_tr_checked_alloc(sizeof(StringObj));
    sb->buf->len = 0; sb->buf->capacity = cap + 1;
    sb->buf->data = (char*)_tr_checked_alloc((size_t)(sb->buf->capacity));
    sb->buf->data[0] = '\0';
    return sb;
}
static inline void StringBuilder_append(StringBuilder* sb, char* s) {
    long long slen = (long long)strlen(s);
    if (slen <= 0) return;
    if (sb->buf->len + slen >= sb->buf->capacity) {
        sb->buf->capacity = (sb->buf->len + slen) * 2 + 8;
        sb->buf->data = (char*)TAURARO_REALLOC(sb->buf->data, (size_t)sb->buf->capacity);
    }
    memcpy(sb->buf->data + sb->buf->len, s, (size_t)slen);
    sb->buf->len += slen;
    sb->buf->data[sb->buf->len] = '\0';
}
static inline void StringBuilder_append_char(StringBuilder* sb, long long c) {
    if (sb->buf->len + 1 >= sb->buf->capacity) {
        sb->buf->capacity = sb->buf->capacity * 2 + 8;
        sb->buf->data = (char*)TAURARO_REALLOC(sb->buf->data, (size_t)sb->buf->capacity);
    }
    sb->buf->data[sb->buf->len++] = (char)c;
    sb->buf->data[sb->buf->len] = '\0';
}
static inline StringObj* StringBuilder_to_string(StringBuilder* sb) {
    return StringObj_init(sb->buf->data);
}
static inline char* StringBuilder_to_owned(StringBuilder* sb) {
    long long sz = sb->buf->len + 1;
    char* out = (char*)_tr_checked_alloc(sz);
    memcpy(out, sb->buf->data, sz);
    return out;
}
static inline char* StringBuilder_as_str(StringBuilder* sb) { return sb->buf->data; }
static inline void StringBuilder_append_int(StringBuilder* sb, long long n) {
    char tmp[32]; snprintf(tmp, sizeof(tmp), "%lld", n);
    StringBuilder_append(sb, tmp);
}
static inline void StringBuilder_append_float(StringBuilder* sb, double f) {
    char tmp[32]; snprintf(tmp, sizeof(tmp), "%g", f);
    StringBuilder_append(sb, tmp);
}
static inline long long StringBuilder_length(StringBuilder* sb) { return sb->buf->len; }
static inline void StringBuilder_clear(StringBuilder* sb) {
    if (sb && sb->buf) { sb->buf->len = 0; if (sb->buf->data) sb->buf->data[0] = '\0'; }
}
static inline void StringBuilder_free(StringBuilder* sb) {
    TAURARO_FREE(sb->buf->data); TAURARO_FREE(sb->buf); TAURARO_FREE(sb);
}
#endif /* TAURARO_RT_NO_STRINGBUILDER */

/* ── File I/O helpers ────────────────────────────────────────────────── */
static inline char* read_file(char* path) {
    if (!path || !*path) return "";
    FILE* f = fopen(path, "rb");
    if (!f) return "";
    fseek(f, 0, SEEK_END); long sz = ftell(f); rewind(f);
    if (sz < 0) { fclose(f); return ""; }
    char* buf = (char*)_tr_checked_alloc((size_t)sz + 1);
    size_t rd = fread(buf, 1, (size_t)sz, f); fclose(f);
    buf[rd] = '\0';
    return buf;
}
static inline bool write_file(char* path, char* content) {
    if (!path || !content) return false;
    FILE* f = fopen(path, "wb");
    if (!f) return false;
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    return true;
}
static inline bool append_file(char* path, char* content) {
    if (!path || !content) return false;
    FILE* f = fopen(path, "ab");
    if (!f) return false;
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    return true;
}
static inline bool file_exists(char* path) {
    if (!path || !*path) return false;
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    fclose(f); return true;
}
#endif /* TAURARO_STD_LIB */
#endif /* TAURARO_NO_RT_HELPERS */

static inline char* _tr_c_strdup(char* s) {
    return s ? strdup(s) : (char*)0;
}
#define _tr_strdup _tr_c_strdup


static inline double _tr_get_inf(void) { return (double)INFINITY; }
static inline bool   _tr_is_inf(double x) { return isinf(x) != 0; }
static inline bool   _tr_is_nan(double x) { return isnan(x) != 0; }


#ifdef _WIN32
static inline void _tr_init_console(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}
#else
static inline void _tr_init_console(void) {}
#endif


/* ==========================================================================
   Extended runtime helpers: datetime, OS, net-server, UDP, DNS, random
   ========================================================================== */

/* -- DateTime helpers ------------------------------------------------------ */
static inline int    _tr_tm_year(long long ts)    { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_year+1900; }
static inline int    _tr_tm_month(long long ts)   { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_mon+1; }
static inline int    _tr_tm_day(long long ts)     { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_mday; }
static inline int    _tr_tm_hour(long long ts)    { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_hour; }
static inline int    _tr_tm_min(long long ts)     { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_min; }
static inline int    _tr_tm_sec(long long ts)     { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_sec; }
static inline int    _tr_tm_weekday(long long ts) { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_wday; }
static inline int    _tr_tm_yearday(long long ts) { time_t t=(time_t)ts; struct tm* m=localtime(&t); return m->tm_yday+1; }
static inline long long _tr_tm_make(int year,int month,int day,int hour,int mi,int sec) {
    struct tm t; memset(&t,0,sizeof(t));
    t.tm_year=year-1900; t.tm_mon=month-1; t.tm_mday=day;
    t.tm_hour=hour; t.tm_min=mi; t.tm_sec=sec; t.tm_isdst=-1;
    return (long long)mktime(&t);
}
static inline char* _tr_strftime(long long ts, const char* fmt) {
    time_t t=(time_t)ts; struct tm* m=localtime(&t);
    char* buf=(char*)_tr_c_malloc(256); if(!buf) return (char*)"";
    strftime(buf,256,fmt,m); return buf;
}

/* -- OS / System helpers (platform-specific) ------------------------------- */
#if defined(TAURARO_BARE) && !defined(__wasi__)
/* Bare / freestanding: no OS services */
static inline char* _tr_hostname(void)          { return (char*)"embedded"; }
static inline char* _tr_username(void)          { return (char*)""; }
static inline int   _tr_cpu_count(void)         { return 1; }
static inline char* _tr_cwd(void)               { return (char*)"/"; }
static inline int   _tr_chdir(const char* p)    { (void)p; return -1; }
static inline char* _tr_platform(void)          { return (char*)"embedded"; }
static inline char* _tr_os_machine(void)        {
#if defined(__aarch64__)
    return (char*)"arm64";
#elif defined(__arm__)
    return (char*)"arm";
#elif defined(__riscv)
    return (char*)"riscv";
#else
    return (char*)"unknown";
#endif
}
static inline long long _tr_memory_total_mb(void) { return 0LL; }
static inline void _tr_console_color(int code)  { (void)code; }
static inline void _tr_console_reset(void)      {}
static inline void _tr_console_clear(void)      {}
#elif defined(_WIN32)
static inline char* _tr_hostname(void) { char* b=(char*)_tr_c_malloc(256); DWORD n=256; GetComputerNameA(b,&n); return b; }
static inline char* _tr_username(void) { char* b=(char*)_tr_c_malloc(256); DWORD n=256; GetUserNameA(b,&n); return b; }
static inline int   _tr_cpu_count(void) { SYSTEM_INFO si; GetSystemInfo(&si); return (int)si.dwNumberOfProcessors; }
static inline char* _tr_cwd(void)       { char* b=(char*)_tr_c_malloc(4096); GetCurrentDirectoryA(4096,b); return b; }
static inline int   _tr_chdir(const char* p) { return SetCurrentDirectoryA(p)?0:-1; }
static inline char* _tr_platform(void) { return (char*)"windows"; }
static inline char* _tr_os_machine(void) {
    SYSTEM_INFO si; GetSystemInfo(&si);
    if(si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64) return (char*)"x86_64";
    if(si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_ARM64) return (char*)"arm64";
    return (char*)"x86";
}
static inline long long _tr_memory_total_mb(void) {
    MEMORYSTATUSEX ms; ms.dwLength=sizeof(ms); GlobalMemoryStatusEx(&ms);
    return (long long)(ms.ullTotalPhys/(1024LL*1024LL));
}
static inline int _tr_tcp_listen(const char* host,int port,int backlog) {
    _tr_net_init();
    SOCKET s=socket(AF_INET,SOCK_STREAM,0); if(s==INVALID_SOCKET) return -1;
    int opt=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char*)&opt,sizeof(opt));
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    if(bind(s,(struct sockaddr*)&a,sizeof(a))!=0){closesocket(s);return -1;}
    if(listen(s,backlog)!=0){closesocket(s);return -1;}
    return (int)s;
}
static inline int   _tr_tcp_accept(int srv) { SOCKET c=accept((SOCKET)srv,NULL,NULL); return (c==INVALID_SOCKET)?-1:(int)c; }
static inline char* _tr_tcp_peer_addr(int fd) {
    struct sockaddr_in a; int al=sizeof(a);
    if(getpeername((SOCKET)fd,(struct sockaddr*)&a,&al)!=0) return (char*)"";
    char* buf=(char*)_tr_c_malloc(64); char ip[32];
    inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));
    _snprintf(buf,63,"%s:%d",ip,(int)ntohs(a.sin_port)); return buf;
}
static inline int  _tr_udp_socket(void) { _tr_net_init(); SOCKET s=socket(AF_INET,SOCK_DGRAM,0); return (s==INVALID_SOCKET)?-1:(int)s; }
static inline int  _tr_udp_bind(int fd,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    return bind((SOCKET)fd,(struct sockaddr*)&a,sizeof(a))==0?0:-1;
}
static inline int  _tr_udp_send_to(int fd,const char* data,int len,const char* host,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=inet_addr(host);
    return (int)sendto((SOCKET)fd,data,len,0,(struct sockaddr*)&a,sizeof(a));
}
static inline int  _tr_udp_recv_from(int fd,char* buf,int cap,char* src) {
    struct sockaddr_in a; int al=sizeof(a);
    int n=(int)recvfrom((SOCKET)fd,buf,cap,0,(struct sockaddr*)&a,&al);
    if(n>0&&src){char ip[32];inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));_snprintf(src,63,"%s:%d",ip,(int)ntohs(a.sin_port));}
    return n;
}
static inline void _tr_udp_close(int fd) { closesocket((SOCKET)fd); }
static inline char* _tr_dns_resolve(const char* host) {
    _tr_net_init();
    struct addrinfo hints={0},*res=NULL; hints.ai_family=AF_INET;
    if(getaddrinfo(host,NULL,&hints,&res)!=0) return (char*)"";
    char* ip=(char*)_tr_c_malloc(64);
    inet_ntop(AF_INET,&((struct sockaddr_in*)res->ai_addr)->sin_addr,ip,64);
    freeaddrinfo(res); return ip;
}
static inline char* _tr_dns_reverse(const char* ip) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; inet_pton(AF_INET,ip,&a.sin_addr);
    char* buf=(char*)_tr_c_malloc(256);
    return (getnameinfo((struct sockaddr*)&a,sizeof(a),buf,256,NULL,0,0)==0)?buf:(char*)"";
}
static inline void _tr_console_color(int code) {
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE); int attr=0;
    if(code==31||code==91) attr=FOREGROUND_RED;
    else if(code==32||code==92) attr=FOREGROUND_GREEN;
    else if(code==33||code==93) attr=FOREGROUND_RED|FOREGROUND_GREEN;
    else if(code==34||code==94) attr=FOREGROUND_BLUE;
    else if(code==35||code==95) attr=FOREGROUND_RED|FOREGROUND_BLUE;
    else if(code==36||code==96) attr=FOREGROUND_GREEN|FOREGROUND_BLUE;
    else attr=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
    if(code>=90) attr|=FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(h,(WORD)attr);
}
static inline void _tr_console_reset(void) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); }
static inline void _tr_console_clear(void) { system("cls"); }
#else
#include <unistd.h>
static inline char* _tr_hostname(void) { char* b=(char*)_tr_c_malloc(256); gethostname(b,256); return b; }
static inline char* _tr_username(void) {
    const char* u=getenv("USER"); if(!u) u=getenv("LOGNAME"); return u?(char*)u:(char*)"";
}
static inline int   _tr_cpu_count(void) {
#if defined(_SC_NPROCESSORS_ONLN)
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(HW_NCPU)
    int mib[2]={CTL_HW,HW_NCPU}; int n=1; size_t l=sizeof(n); sysctl(mib,2,&n,&l,NULL,0); return n>0?n:1;
#else
    return 1;
#endif
}
static inline char* _tr_cwd(void)       { char* b=(char*)_tr_c_malloc(4096); return getcwd(b,4096); }
static inline int   _tr_chdir(const char* p) { return chdir(p); }
#ifdef __APPLE__
#  if defined(TAURARO_IOS)
static inline char* _tr_platform(void) { return (char*)"ios"; }
#  else
static inline char* _tr_platform(void) { return (char*)"macos"; }
#  endif
#elif defined(TAURARO_ANDROID)
static inline char* _tr_platform(void) { return (char*)"android"; }
#elif defined(TAURARO_WASM)
static inline char* _tr_platform(void) { return (char*)"wasm"; }
#else
static inline char* _tr_platform(void) { return (char*)"linux"; }
#endif
static inline char* _tr_os_machine(void) {
#if defined(__x86_64__)||defined(__amd64__)
    return (char*)"x86_64";
#elif defined(__aarch64__)
    return (char*)"arm64";
#elif defined(__arm__)
    return (char*)"arm";
#else
    return (char*)"unknown";
#endif
}
static inline long long _tr_memory_total_mb(void) {
    long p=sysconf(_SC_PHYS_PAGES),s=sysconf(_SC_PAGE_SIZE);
    return (p>0&&s>0)?(long long)p*s/(1024LL*1024LL):0;
}
static inline int _tr_tcp_listen(const char* host,int port,int backlog) {
    int s=socket(AF_INET,SOCK_STREAM,0); if(s<0) return -1;
    int opt=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    if(bind(s,(struct sockaddr*)&a,sizeof(a))<0){close(s);return -1;}
    if(listen(s,backlog)<0){close(s);return -1;} return s;
}
static inline int   _tr_tcp_accept(int srv) { return accept(srv,NULL,NULL); }
static inline char* _tr_tcp_peer_addr(int fd) {
    struct sockaddr_in a; socklen_t al=sizeof(a);
    if(getpeername(fd,(struct sockaddr*)&a,&al)<0) return (char*)"";
    char* buf=(char*)_tr_c_malloc(64); char ip[32];
    inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));
    snprintf(buf,63,"%s:%d",ip,(int)ntohs(a.sin_port)); return buf;
}
static inline int  _tr_udp_socket(void) { return socket(AF_INET,SOCK_DGRAM,0); }
static inline int  _tr_udp_bind(int fd,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=INADDR_ANY;
    return bind(fd,(struct sockaddr*)&a,sizeof(a))==0?0:-1;
}
static inline int  _tr_udp_send_to(int fd,const char* data,int len,const char* host,int port) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; a.sin_port=htons((unsigned short)port); a.sin_addr.s_addr=inet_addr(host);
    return (int)sendto(fd,data,len,0,(struct sockaddr*)&a,sizeof(a));
}
static inline int  _tr_udp_recv_from(int fd,char* buf,int cap,char* src) {
    struct sockaddr_in a; socklen_t al=sizeof(a);
    int n=(int)recvfrom(fd,buf,cap,0,(struct sockaddr*)&a,&al);
    if(n>0&&src){char ip[32];inet_ntop(AF_INET,&a.sin_addr,ip,sizeof(ip));snprintf(src,63,"%s:%d",ip,(int)ntohs(a.sin_port));}
    return n;
}
static inline void _tr_udp_close(int fd) { close(fd); }
static inline char* _tr_dns_resolve(const char* host) {
    struct addrinfo hints={0},*res=NULL; hints.ai_family=AF_INET;
    if(getaddrinfo(host,NULL,&hints,&res)!=0) return (char*)"";
    char* ip=(char*)_tr_c_malloc(64);
    inet_ntop(AF_INET,&((struct sockaddr_in*)res->ai_addr)->sin_addr,ip,64);
    freeaddrinfo(res); return ip;
}
static inline char* _tr_dns_reverse(const char* ip) {
    struct sockaddr_in a; memset(&a,0,sizeof(a));
    a.sin_family=AF_INET; inet_pton(AF_INET,ip,&a.sin_addr);
    char* buf=(char*)_tr_c_malloc(256);
    return (getnameinfo((struct sockaddr*)&a,sizeof(a),buf,256,NULL,0,NI_NAMEREQD)==0)?buf:(char*)"";
}
static inline void _tr_console_color(int code) { printf("\033[%dm",code); fflush(stdout); }
static inline void _tr_console_reset(void)     { printf("\033[0m"); fflush(stdout); }
static inline void _tr_console_clear(void)     { printf("\033[2J\033[H"); fflush(stdout); }
#endif

/* ══════════════════════════════════════════════════════════════════════════
 * Non-blocking socket API
 *
 * Return values:
 *   >= 0  : success / bytes transferred
 *   -1    : hard error
 *   TAURARO_WOULD_BLOCK (-2) : operation would block (EAGAIN/EWOULDBLOCK/WSAEWOULDBLOCK)
 *
 * Typical pattern with _TrIOPoll:
 *   int fd = _tr_tcp_connect_nb(host, port);   // initiates connect, may return fd immediately
 *   _tr_iopoll_add(poll, fd, TAURARO_POLLOUT, ctx); // wait for writable = connect done
 *   int n = _tr_tcp_recv_nb(fd, buf, cap);      // -2 means try again later
 * ══════════════════════════════════════════════════════════════════════════ */
#define TAURARO_WOULD_BLOCK (-2)

#if defined(TAURARO_BARE) || defined(TAURARO_KERNEL)
static inline int  _tr_tcp_set_nonblocking(int fd)                    { (void)fd; return -1; }
static inline int  _tr_tcp_recv_nb(int fd, char* b, int c)            { (void)fd;(void)b;(void)c; return -1; }
static inline int  _tr_tcp_send_nb(int fd, const char* d, int l)      { (void)fd;(void)d;(void)l; return -1; }
static inline int  _tr_tcp_accept_nb(int fd)                          { (void)fd; return TAURARO_WOULD_BLOCK; }
static inline int  _tr_tcp_connect_nb(const char* h, int p)           { (void)h;(void)p; return -1; }

#elif defined(_WIN32)
#ifndef _TR_NET_INCLUDED
#define _TR_NET_INCLUDED
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif
static inline int _tr_tcp_set_nonblocking(int fd) {
    u_long mode = 1;
    return ioctlsocket((SOCKET)fd, FIONBIO, &mode) == 0 ? 0 : -1;
}
static inline int _tr_tcp_recv_nb(int fd, char* buf, int cap) {
    int n = recv((SOCKET)fd, buf, cap, 0);
    if (n < 0 && WSAGetLastError() == WSAEWOULDBLOCK) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_send_nb(int fd, const char* data, int len) {
    int n = send((SOCKET)fd, data, len, 0);
    if (n < 0 && WSAGetLastError() == WSAEWOULDBLOCK) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_accept_nb(int server_fd) {
    SOCKET s = accept((SOCKET)server_fd, NULL, NULL);
    if (s == INVALID_SOCKET) {
        return (WSAGetLastError() == WSAEWOULDBLOCK) ? TAURARO_WOULD_BLOCK : -1;
    }
    return (int)s;
}
static inline int _tr_tcp_connect_nb(const char* host, int port) {
    _tr_net_init();
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
    char pbuf[16]; sprintf(pbuf, "%d", port);
    if (getaddrinfo(host, pbuf, &hints, &res) != 0) return -1;
    SOCKET fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == INVALID_SOCKET) { freeaddrinfo(res); return -1; }
    u_long mode = 1; ioctlsocket(fd, FIONBIO, &mode);
    connect(fd, res->ai_addr, (int)res->ai_addrlen); /* WSAEWOULDBLOCK is expected */
    freeaddrinfo(res);
    return (int)fd;
}

#else /* POSIX */
#include <fcntl.h>
#include <errno.h>
static inline int _tr_tcp_set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0 ? 0 : -1;
}
static inline int _tr_tcp_recv_nb(int fd, char* buf, int cap) {
    int n = (int)recv(fd, buf, (size_t)cap, 0);
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_send_nb(int fd, const char* data, int len) {
    int n = (int)send(fd, data, (size_t)len, 0);
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) return TAURARO_WOULD_BLOCK;
    return n;
}
static inline int _tr_tcp_accept_nb(int server_fd) {
    int fd = accept(server_fd, NULL, NULL);
    if (fd < 0) return (errno == EAGAIN || errno == EWOULDBLOCK) ? TAURARO_WOULD_BLOCK : -1;
    return fd;
}
static inline int _tr_tcp_connect_nb(const char* host, int port) {
    struct addrinfo hints = {0}, *res = NULL;
    hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
    char pbuf[16]; snprintf(pbuf, sizeof(pbuf), "%d", port);
    if (getaddrinfo(host, pbuf, &hints, &res) != 0) return -1;
    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd < 0) { freeaddrinfo(res); return -1; }
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    connect(fd, res->ai_addr, res->ai_addrlen); /* EINPROGRESS expected */
    freeaddrinfo(res);
    return fd;
}
#endif /* non-blocking socket API */

/* -- Random (LCG-64) ------------------------------------------------------- */
typedef struct { unsigned long long s; } _TrRng;
static inline _TrRng* _tr_rng_new(long long seed) {
    _TrRng* r=(_TrRng*)_tr_c_malloc(sizeof(_TrRng));
    r->s=(unsigned long long)(seed^0xdeadbeefcafeULL); return r;
}
static inline long long _tr_rng_next(_TrRng* r) {
    r->s=r->s*6364136223846793005ULL+1442695040888963407ULL;
    return (long long)((r->s>>1)&0x7FFFFFFFFFFFFFFFLL);
}
static inline void _tr_rng_free(_TrRng* r) { _tr_free(r); }


static inline char* _tr_float_fmt(double f, int decimals) {
    char fmt[16]; int d = decimals < 0 ? 6 : decimals;
    sprintf(fmt, "%%.%df", d);
    char* buf = (char*)_tr_c_malloc(64); if(!buf) return (char*)"";
    sprintf(buf, fmt, f); return buf;
}

/* ── Platform capability detection ──────────────────────────────────────
 * Call from Tauraro via  extern "C":  def _tr_target_has_filesystem() -> bool
 * ─────────────────────────────────────────────────────────────────────── */
static inline bool _tr_target_has_filesystem(void) {
#if defined(TAURARO_BARE) && !defined(__wasi__)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_target_has_networking(void) {
#if defined(TAURARO_BARE) || defined(TAURARO_WASM)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_target_has_threads(void) {
#ifdef TAURARO_BARE
    return false;
#else
    return true;
#endif
}
static inline bool _tr_target_has_os_services(void) {
#if defined(TAURARO_BARE) && !defined(__wasi__)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_is_android(void) {
#ifdef TAURARO_ANDROID
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_ios(void) {
#ifdef TAURARO_IOS
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_wasm(void) {
#ifdef TAURARO_WASM
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_embedded(void) {
#if defined(TAURARO_BARE) && !defined(TAURARO_WASM)
    return true;
#else
    return false;
#endif
}
static inline bool _tr_is_posix(void) {
#if defined(_WIN32) || defined(TAURARO_BARE)
    return false;
#else
    return true;
#endif
}
static inline bool _tr_is_mobile(void) {
#if defined(TAURARO_ANDROID) || defined(TAURARO_IOS)
    return true;
#else
    return false;
#endif
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Executable directory — returns directory containing the running binary.
 * ═══════════════════════════════════════════════════════════════════════════ */
#if defined(__APPLE__)
#  include <mach-o/dyld.h>
#endif
static inline char* _tr_exe_dir(void) {
#if defined(_WIN32)
    char* buf=(char*)_tr_c_malloc(4096);
    DWORD n=GetModuleFileNameA(NULL,buf,4096);
    if(!n){buf[0]='.';buf[1]='\0';return buf;}
    for(int i=(int)n-1;i>0;i--){if(buf[i]=='\\'||buf[i]=='/'){buf[i]='\0';break;}}
    return buf;
#elif defined(__APPLE__)
    char tmp[4096]; uint32_t sz=sizeof(tmp);
    if(_NSGetExecutablePath(tmp,&sz)!=0) return (char*)".";
    char* buf=(char*)_tr_c_malloc(4096);
    if(!realpath(tmp,buf)){strcpy(buf,".");return buf;}
    for(int i=(int)strlen(buf)-1;i>0;i--){if(buf[i]=='/'){buf[i]='\0';break;}}
    return buf;
#elif defined(__linux__)
    char* buf=(char*)_tr_c_malloc(4096);
    ssize_t n=readlink("/proc/self/exe",buf,4095);
    if(n<=0){buf[0]='.';buf[1]='\0';return buf;}
    buf[n]='\0';
    for(int i=(int)n-1;i>0;i--){if(buf[i]=='/'){buf[i]='\0';break;}}
    return buf;
#else
    return (char*)".";
#endif
}

/* ═══════════════════════════════════════════════════════════════════════════
 * REGEX — POSIX regex.h on Linux/Mac; stubs on Windows and bare-metal.
 * ═══════════════════════════════════════════════════════════════════════════ */
#ifndef TAURARO_BARE
#  if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#    include <regex.h>
#    define TAURARO_HAVE_REGEX 1
#  endif
#endif

#ifdef TAURARO_HAVE_REGEX
typedef struct { regex_t re; } _TrRegex;
static inline char* _tr_regex_compile(char* pattern, int icase) {
    _TrRegex* r = (_TrRegex*)TAURARO_ALLOC(sizeof(_TrRegex));
    if (!r) return NULL;
    int flags = REG_EXTENDED; if (icase) flags |= REG_ICASE;
    if (regcomp(&r->re, pattern, flags) != 0) { TAURARO_FREE(r); return NULL; }
    return (char*)r;
}
static inline bool _tr_regex_match(char* handle, char* text) {
    if (!handle || !text) return false;
    return regexec(&((_TrRegex*)handle)->re, text, 0, NULL, 0) == 0;
}
static inline int _tr_regex_find_start(char* handle, char* text, int from) {
    if (!handle || !text) return -1;
    regmatch_t m;
    if (regexec(&((_TrRegex*)handle)->re, text + from, 1, &m, 0) != 0) return -1;
    return from + (int)m.rm_so;
}
static inline int _tr_regex_find_len(char* handle, char* text, int from) {
    if (!handle || !text) return -1;
    regmatch_t m;
    if (regexec(&((_TrRegex*)handle)->re, text + from, 1, &m, 0) != 0) return -1;
    return (int)(m.rm_eo - m.rm_so);
}
static inline char* _tr_regex_replace_first(char* handle, char* text, char* repl) {
    if (!handle || !text || !repl) return _tr_strdup(text ? text : "");
    regmatch_t m;
    if (regexec(&((_TrRegex*)handle)->re, text, 1, &m, 0) != 0) return _tr_strdup(text);
    size_t pre = (size_t)m.rm_so, rlen = strlen(repl), post = strlen(text) - (size_t)m.rm_eo;
    char* out = (char*)TAURARO_ALLOC(pre + rlen + post + 1); if (!out) return _tr_strdup(text);
    memcpy(out, text, pre); memcpy(out+pre, repl, rlen); memcpy(out+pre+rlen, text+m.rm_eo, post);
    out[pre+rlen+post] = '\0'; return out;
}
static inline char* _tr_regex_replace_all(char* handle, char* text, char* repl) {
    if (!handle || !text || !repl) return _tr_strdup(text ? text : "");
    _TrRegex* r = (_TrRegex*)handle; regmatch_t m;
    size_t rlen = strlen(repl), cur = 0, tlen = strlen(text);
    char* result = _tr_strdup("");
    while (cur < tlen && regexec(&r->re, text + cur, 1, &m, 0) == 0) {
        size_t pre = (size_t)m.rm_so, olen = strlen(result);
        char* tmp = (char*)TAURARO_ALLOC(olen + pre + rlen + 1); if (!tmp) break;
        memcpy(tmp, result, olen); memcpy(tmp+olen, text+cur, pre);
        memcpy(tmp+olen+pre, repl, rlen); tmp[olen+pre+rlen] = '\0';
        TAURARO_FREE(result); result = tmp;
        size_t adv = (size_t)(m.rm_eo - m.rm_so); if (adv == 0) adv = 1;
        cur += pre + adv;
    }
    size_t rem = tlen - cur, olen2 = strlen(result);
    char* out = (char*)TAURARO_ALLOC(olen2 + rem + 1);
    if (out) { memcpy(out, result, olen2); memcpy(out+olen2, text+cur, rem); out[olen2+rem]='\0'; TAURARO_FREE(result); return out; }
    return result;
}
static inline int _tr_regex_count(char* handle, char* text) {
    if (!handle || !text) return 0;
    regmatch_t m; int n = 0; size_t cur = 0, tlen = strlen(text);
    while (cur < tlen && regexec(&((_TrRegex*)handle)->re, text+cur, 1, &m, 0) == 0) {
        n++; size_t adv=(size_t)(m.rm_eo-m.rm_so); if(adv==0)adv=1; cur+=(size_t)m.rm_so+adv;
    }
    return n;
}
static inline void _tr_regex_free(char* handle) {
    if (!handle) return; regfree(&((_TrRegex*)handle)->re); TAURARO_FREE(handle);
}
#else
static inline char* _tr_regex_compile(char* p, int i) { (void)p;(void)i; return NULL; }
static inline bool  _tr_regex_match(char* h, char* t) { (void)h;(void)t; return false; }
static inline int   _tr_regex_find_start(char* h, char* t, int f) { (void)h;(void)t;(void)f; return -1; }
static inline int   _tr_regex_find_len(char* h, char* t, int f) { (void)h;(void)t;(void)f; return -1; }
static inline char* _tr_regex_replace_first(char* h, char* t, char* r) { (void)h;(void)r; return _tr_strdup(t?t:""); }
static inline char* _tr_regex_replace_all(char* h, char* t, char* r) { (void)h;(void)r; return _tr_strdup(t?t:""); }
static inline int   _tr_regex_count(char* h, char* t) { (void)h;(void)t; return 0; }
static inline void  _tr_regex_free(char* h) { (void)h; }
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * SHA-256 — pure C, no external dependencies.
 * ═══════════════════════════════════════════════════════════════════════════ */
#define _TR_ROTR32(x,n) (((x)>>(n))|((x)<<(32-(n))))
#define _TR_S0(x) (_TR_ROTR32(x,2)^_TR_ROTR32(x,13)^_TR_ROTR32(x,22))
#define _TR_S1(x) (_TR_ROTR32(x,6)^_TR_ROTR32(x,11)^_TR_ROTR32(x,25))
#define _TR_s0(x) (_TR_ROTR32(x,7)^_TR_ROTR32(x,18)^((x)>>3))
#define _TR_s1(x) (_TR_ROTR32(x,17)^_TR_ROTR32(x,19)^((x)>>10))
#define _TR_CH(x,y,z) (((x)&(y))^(~(x)&(z)))
#define _TR_MAJ(x,y,z) (((x)&(y))^((x)&(z))^((y)&(z)))

static const uint32_t _tr_sha256_K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

typedef struct { uint32_t h[8]; uint8_t buf[64]; uint64_t bits; uint32_t buf_len; } _TrSHA256Ctx;

static inline void _tr_sha256_init(_TrSHA256Ctx* c) {
    c->h[0]=0x6a09e667;c->h[1]=0xbb67ae85;c->h[2]=0x3c6ef372;c->h[3]=0xa54ff53a;
    c->h[4]=0x510e527f;c->h[5]=0x9b05688c;c->h[6]=0x1f83d9ab;c->h[7]=0x5be0cd19;
    c->bits=0; c->buf_len=0;
}
static inline void _tr_sha256_block(_TrSHA256Ctx* c, const uint8_t* blk) {
    uint32_t w[64],a,b,cc,d,e,f,g,h,t1,t2; int i;
    for(i=0;i<16;i++) w[i]=((uint32_t)blk[i*4]<<24)|((uint32_t)blk[i*4+1]<<16)|((uint32_t)blk[i*4+2]<<8)|(uint32_t)blk[i*4+3];
    for(i=16;i<64;i++) w[i]=_TR_s1(w[i-2])+w[i-7]+_TR_s0(w[i-15])+w[i-16];
    a=c->h[0];b=c->h[1];cc=c->h[2];d=c->h[3];e=c->h[4];f=c->h[5];g=c->h[6];h=c->h[7];
    for(i=0;i<64;i++){
        t1=h+_TR_S1(e)+_TR_CH(e,f,g)+_tr_sha256_K[i]+w[i];
        t2=_TR_S0(a)+_TR_MAJ(a,b,cc);
        h=g;g=f;f=e;e=d+t1;d=cc;cc=b;b=a;a=t1+t2;
    }
    c->h[0]+=a;c->h[1]+=b;c->h[2]+=cc;c->h[3]+=d;c->h[4]+=e;c->h[5]+=f;c->h[6]+=g;c->h[7]+=h;
}
static inline void _tr_sha256_update(_TrSHA256Ctx* c, const uint8_t* data, size_t len) {
    for(size_t i=0;i<len;i++){
        c->buf[c->buf_len++]=data[i]; c->bits+=8;
        if(c->buf_len==64){_tr_sha256_block(c,c->buf);c->buf_len=0;}
    }
}
static inline void _tr_sha256_final(_TrSHA256Ctx* c, uint8_t* dig) {
    uint64_t bits=c->bits;
    c->buf[c->buf_len++]=0x80;
    while(c->buf_len!=56){if(c->buf_len==64){_tr_sha256_block(c,c->buf);c->buf_len=0;}c->buf[c->buf_len++]=0;}
    for(int i=7;i>=0;i--){c->buf[56+(7-i)]=(uint8_t)(bits>>((uint64_t)i*8));}
    _tr_sha256_block(c,c->buf);
    for(int i=0;i<8;i++){dig[i*4]=(uint8_t)(c->h[i]>>24);dig[i*4+1]=(uint8_t)(c->h[i]>>16);dig[i*4+2]=(uint8_t)(c->h[i]>>8);dig[i*4+3]=(uint8_t)c->h[i];}
}
static const char _tr_hex_lc[] = "0123456789abcdef";
static inline char* _tr_sha256_hex(char* input) {
    _TrSHA256Ctx ctx; uint8_t dig[32];
    _tr_sha256_init(&ctx);
    if(input) _tr_sha256_update(&ctx,(const uint8_t*)input,strlen(input));
    _tr_sha256_final(&ctx,dig);
    char* out=(char*)TAURARO_ALLOC(65); if(!out) return NULL;
    for(int i=0;i<32;i++){out[i*2]=_tr_hex_lc[dig[i]>>4];out[i*2+1]=_tr_hex_lc[dig[i]&15];}
    out[64]='\0'; return out;
}
static inline char* _tr_sha256_bytes_of(char* input, int ilen) {
    _TrSHA256Ctx ctx; uint8_t dig[32];
    _tr_sha256_init(&ctx);
    if(input&&ilen>0) _tr_sha256_update(&ctx,(const uint8_t*)input,(size_t)ilen);
    _tr_sha256_final(&ctx,dig);
    char* out=(char*)TAURARO_ALLOC(32); if(!out) return NULL;
    memcpy(out,dig,32); return out;
}

/* ── HMAC-SHA256 ────────────────────────────────────────────────────────── */
static inline char* _tr_hmac_sha256(char* key, int klen, char* msg) {
    uint8_t k[64]={0}; _TrSHA256Ctx ctx;
    if(klen>64){_tr_sha256_init(&ctx);_tr_sha256_update(&ctx,(const uint8_t*)key,(size_t)klen);uint8_t tmp[32];_tr_sha256_final(&ctx,tmp);memcpy(k,tmp,32);}
    else memcpy(k,key,(size_t)klen);
    uint8_t ipad[64],opad[64];
    for(int i=0;i<64;i++){ipad[i]=k[i]^0x36;opad[i]=k[i]^0x5c;}
    uint8_t inner[32];
    _tr_sha256_init(&ctx);_tr_sha256_update(&ctx,ipad,64);
    if(msg)_tr_sha256_update(&ctx,(const uint8_t*)msg,strlen(msg));
    _tr_sha256_final(&ctx,inner);
    _tr_sha256_init(&ctx);_tr_sha256_update(&ctx,opad,64);_tr_sha256_update(&ctx,inner,32);
    uint8_t dig[32]; _tr_sha256_final(&ctx,dig);
    char* out=(char*)TAURARO_ALLOC(65); if(!out) return NULL;
    for(int i=0;i<32;i++){out[i*2]=_tr_hex_lc[dig[i]>>4];out[i*2+1]=_tr_hex_lc[dig[i]&15];}
    out[64]='\0'; return out;
}

/* ── UUID v4 ────────────────────────────────────────────────────────────── */
static inline char* _tr_uuid_v4(void) {
    uint8_t b[16];
#if !defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
    FILE* f=fopen("/dev/urandom","rb");
    if(f){fread(b,1,16,f);fclose(f);}
    else{for(int i=0;i<16;i++)b[i]=(uint8_t)(rand()&0xff);}
#else
    for(int i=0;i<16;i++)b[i]=(uint8_t)(rand()&0xff);
#endif
    b[6]=(b[6]&0x0f)|0x40; b[8]=(b[8]&0x3f)|0x80;
    char* out=(char*)TAURARO_ALLOC(37); if(!out) return NULL;
    snprintf(out,37,"%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15]);
    return out;
}

/* ── MD5 (compact, for legacy use) ─────────────────────────────────────── */
static inline char* _tr_md5_hex(char* s) {
    /* Minimal MD5; message expanded inline. */
    static const uint32_t T[64]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
        0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
        0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
        0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
        0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
        0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
    };
    static const int S[64]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
                             5, 9,14,20,5, 9,14,20,5, 9,14,20,5, 9,14,20,
                             4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
                             6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
    size_t ilen = s ? strlen(s) : 0;
    size_t padlen = ((ilen+8)/64+1)*64;
    uint8_t* msg = (uint8_t*)TAURARO_CALLOC(1,padlen);
    if(!msg) return _tr_strdup("00000000000000000000000000000000");
    if(s) memcpy(msg,s,ilen);
    msg[ilen]=0x80;
    uint64_t bits=(uint64_t)ilen*8;
    for(int i=0;i<8;i++) msg[padlen-8+i]=(uint8_t)(bits>>(uint64_t)(i*8));
    uint32_t a=0x67452301,b=0xefcdab89,cc=0x98badcfe,d=0x10325476;
    for(size_t off=0;off<padlen;off+=64){
        uint32_t M[16],A=a,B=b,C=cc,D=d;
        for(int i=0;i<16;i++) M[i]=((uint32_t)msg[off+i*4])|((uint32_t)msg[off+i*4+1]<<8)|((uint32_t)msg[off+i*4+2]<<16)|((uint32_t)msg[off+i*4+3]<<24);
        for(int i=0;i<64;i++){
            uint32_t F,g2;
            if(i<16){F=(_TR_CH(B,C,D));g2=(uint32_t)i;}
            else if(i<32){F=(D^(B&(C^D)));g2=(uint32_t)(5*i+1)%16;}
            else if(i<48){F=(B^C^D);g2=(uint32_t)(3*i+5)%16;}
            else{F=(C^(B|(~D)));g2=(uint32_t)(7*i)%16;}
            F=F+A+T[i]+M[g2];
            A=D;D=C;C=B;B=B+((F<<S[i])|(F>>(32-S[i])));
        }
        a+=A;b+=B;cc+=C;d+=D;
    }
    TAURARO_FREE(msg);
    char* out=(char*)TAURARO_ALLOC(33); if(!out) return _tr_strdup("00000000000000000000000000000000");
    uint32_t r[4]={a,b,cc,d};
    for(int i=0;i<4;i++) for(int j=0;j<4;j++){
        uint8_t byte=(uint8_t)(r[i]>>(j*8));
        out[i*8+j*2]=_tr_hex_lc[byte>>4]; out[i*8+j*2+1]=_tr_hex_lc[byte&15];
    }
    out[32]='\0'; return out;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * TLS/HTTPS — OpenSSL (opt-in: -DTAURARO_TLS_OPENSSL -lssl -lcrypto).
 * ═══════════════════════════════════════════════════════════════════════════ */
#ifdef TAURARO_TLS_OPENSSL
#  include <openssl/ssl.h>
#  include <openssl/err.h>
typedef struct { SSL_CTX* ctx; SSL* ssl; int fd; } _TrTLSConn;
#  ifdef _WIN32
#    define _TR_SOCK_CLOSE(fd) closesocket(fd)
#  else
#    define _TR_SOCK_CLOSE(fd) close(fd)
#  endif
static inline char* _tr_tls_connect(char* host, int port) {
    static _Atomic int _tr_ssl_once = 0;
    if (atomic_fetch_add(&_tr_ssl_once,1)==0){SSL_library_init();SSL_load_error_strings();OpenSSL_add_all_algorithms();}
    struct addrinfo hints={0},*res=NULL;
    hints.ai_family=AF_UNSPEC;hints.ai_socktype=SOCK_STREAM;
    char pbuf[16]; snprintf(pbuf,sizeof(pbuf),"%d",port);
    if(getaddrinfo(host,pbuf,&hints,&res)!=0) return NULL;
    int fd=(int)socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if(fd<0){freeaddrinfo(res);return NULL;}
    if(connect(fd,res->ai_addr,(int)res->ai_addrlen)!=0){freeaddrinfo(res);_TR_SOCK_CLOSE(fd);return NULL;}
    freeaddrinfo(res);
    SSL_CTX* ctx=SSL_CTX_new(TLS_client_method());
    if(!ctx){_TR_SOCK_CLOSE(fd);return NULL;}
    SSL* ssl=SSL_new(ctx); SSL_set_fd(ssl,fd); SSL_set_tlsext_host_name(ssl,host);
    if(SSL_connect(ssl)!=1){SSL_free(ssl);SSL_CTX_free(ctx);_TR_SOCK_CLOSE(fd);return NULL;}
    _TrTLSConn* c=(_TrTLSConn*)TAURARO_ALLOC(sizeof(_TrTLSConn));
    if(!c){SSL_free(ssl);SSL_CTX_free(ctx);_TR_SOCK_CLOSE(fd);return NULL;}
    c->ctx=ctx;c->ssl=ssl;c->fd=fd; return (char*)c;
}
static inline int   _tr_tls_send(char* h, char* d) { if(!h||!d) return -1; return SSL_write(((_TrTLSConn*)h)->ssl,d,(int)strlen(d)); }
static inline char* _tr_tls_recv(char* h, int cap) {
    if(!h||cap<=0) return _tr_strdup("");
    char* buf=(char*)TAURARO_ALLOC((size_t)cap+1); if(!buf) return _tr_strdup("");
    int n=SSL_read(((_TrTLSConn*)h)->ssl,buf,cap);
    if(n<=0){TAURARO_FREE(buf);return _tr_strdup("");}
    buf[n]='\0'; return buf;
}
static inline void _tr_tls_close(char* h) {
    if(!h) return; _TrTLSConn* c=(_TrTLSConn*)h;
    SSL_shutdown(c->ssl);SSL_free(c->ssl);SSL_CTX_free(c->ctx);_TR_SOCK_CLOSE(c->fd);TAURARO_FREE(c);
}
#else
static inline char* _tr_tls_connect(char* h, int p) { (void)h;(void)p; return NULL; }
static inline int   _tr_tls_send(char* h, char* d)  { (void)h;(void)d; return -1; }
static inline char* _tr_tls_recv(char* h, int c)    { (void)h;(void)c; return _tr_strdup(""); }
static inline void  _tr_tls_close(char* h)          { (void)h; }
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * COMPRESS — zlib (opt-in: -DTAURARO_COMPRESS_ZLIB -lz).
 * ═══════════════════════════════════════════════════════════════════════════ */
#ifdef TAURARO_COMPRESS_ZLIB
#  include <zlib.h>
static inline char* _tr_zlib_compress(char* input, int ilen, int* out_len) {
    if(!input||ilen<=0){if(out_len)*out_len=0;return NULL;}
    uLong bound=compressBound((uLong)ilen);
    char* out=(char*)TAURARO_ALLOC(bound);if(!out){if(out_len)*out_len=0;return NULL;}
    uLong dlen=bound;
    if(compress2((Bytef*)out,&dlen,(const Bytef*)input,(uLong)ilen,Z_BEST_COMPRESSION)!=Z_OK){TAURARO_FREE(out);if(out_len)*out_len=0;return NULL;}
    if(out_len)*out_len=(int)dlen; return out;
}
static inline char* _tr_zlib_decompress(char* input, int ilen, int max_out) {
    if(!input||ilen<=0) return _tr_strdup("");
    char* out=(char*)TAURARO_ALLOC((size_t)max_out+1);if(!out) return _tr_strdup("");
    uLong dlen=(uLong)max_out;
    if(uncompress((Bytef*)out,&dlen,(const Bytef*)input,(uLong)ilen)!=Z_OK){TAURARO_FREE(out);return _tr_strdup("");}
    out[dlen]='\0'; return out;
}
static inline char* _tr_deflate(char* input, int ilen, int* out_len) {
    if(!input||ilen<=0){if(out_len)*out_len=0;return NULL;}
    z_stream s={0}; deflateInit2(&s,Z_BEST_COMPRESSION,Z_DEFLATED,-15,8,Z_DEFAULT_STRATEGY);
    uLong bound=deflateBound(&s,(uLong)ilen);
    char* out=(char*)TAURARO_ALLOC(bound);if(!out){deflateEnd(&s);if(out_len)*out_len=0;return NULL;}
    s.next_in=(Bytef*)input;s.avail_in=(uInt)ilen;s.next_out=(Bytef*)out;s.avail_out=(uInt)bound;
    deflate(&s,Z_FINISH);if(out_len)*out_len=(int)s.total_out;deflateEnd(&s);return out;
}
static inline char* _tr_inflate(char* input, int ilen, int max_out) {
    if(!input||ilen<=0) return _tr_strdup("");
    z_stream s={0};inflateInit2(&s,-15);
    char* out=(char*)TAURARO_ALLOC((size_t)max_out+1);if(!out){inflateEnd(&s);return _tr_strdup("");}
    s.next_in=(Bytef*)input;s.avail_in=(uInt)ilen;s.next_out=(Bytef*)out;s.avail_out=(uInt)max_out;
    inflate(&s,Z_FINISH);int n=(int)s.total_out;inflateEnd(&s);out[n]='\0';return out;
}
#else
static inline char* _tr_zlib_compress(char* i, int il, int* n) { (void)i;(void)il;if(n)*n=0;return NULL; }
static inline char* _tr_zlib_decompress(char* i, int il, int m) { (void)i;(void)il;(void)m;return _tr_strdup(""); }
static inline char* _tr_deflate(char* i, int il, int* n) { (void)i;(void)il;if(n)*n=0;return NULL; }
static inline char* _tr_inflate(char* i, int il, int m) { (void)i;(void)il;(void)m;return _tr_strdup(""); }
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * UNICODE / UTF-8 — pure C, no external dependencies.
 * ═══════════════════════════════════════════════════════════════════════════ */
static inline uint32_t _tr_utf8_next(const char** pp) {
    const uint8_t* p=(const uint8_t*)*pp; uint32_t cp;
    if(!*p){return 0;}
    if(p[0]<0x80){cp=p[0];*pp=(const char*)(p+1);return cp;}
    if((p[0]&0xe0)==0xc0&&(p[1]&0xc0)==0x80){cp=((p[0]&0x1f)<<6)|(p[1]&0x3f);*pp=(const char*)(p+2);return cp;}
    if((p[0]&0xf0)==0xe0&&(p[1]&0xc0)==0x80&&(p[2]&0xc0)==0x80){cp=((p[0]&0x0f)<<12)|((p[1]&0x3f)<<6)|(p[2]&0x3f);*pp=(const char*)(p+3);return cp;}
    if((p[0]&0xf8)==0xf0&&(p[1]&0xc0)==0x80&&(p[2]&0xc0)==0x80&&(p[3]&0xc0)==0x80){cp=((p[0]&0x07)<<18)|((p[1]&0x3f)<<12)|((p[2]&0x3f)<<6)|(p[3]&0x3f);*pp=(const char*)(p+4);return cp;}
    *pp=(const char*)(p+1);return 0xFFFD;
}
static inline int _tr_utf8_encode_cp(uint32_t cp, char* buf) {
    if(cp<0x80){buf[0]=(char)cp;return 1;}
    if(cp<0x800){buf[0]=(char)(0xc0|(cp>>6));buf[1]=(char)(0x80|(cp&0x3f));return 2;}
    if(cp<0x10000){buf[0]=(char)(0xe0|(cp>>12));buf[1]=(char)(0x80|((cp>>6)&0x3f));buf[2]=(char)(0x80|(cp&0x3f));return 3;}
    buf[0]=(char)(0xf0|(cp>>18));buf[1]=(char)(0x80|((cp>>12)&0x3f));buf[2]=(char)(0x80|((cp>>6)&0x3f));buf[3]=(char)(0x80|(cp&0x3f));return 4;
}
static inline int _tr_utf8_len(char* s) {
    if(!s) return 0; int n=0; const char* p=s; while(*p){_tr_utf8_next(&p);n++;} return n;
}
static inline bool _tr_utf8_valid(char* s) {
    if(!s) return false;
    const uint8_t* p=(const uint8_t*)s;
    while(*p){
        int seq;
        if(p[0]<0x80){p++;continue;}
        else if((p[0]&0xe0)==0xc0)seq=2;
        else if((p[0]&0xf0)==0xe0)seq=3;
        else if((p[0]&0xf8)==0xf0)seq=4;
        else return false;
        for(int i=1;i<seq;i++) if((p[i]&0xc0)!=0x80) return false;
        p+=seq;
    }
    return true;
}
static inline int _tr_utf8_char_at(char* s, int idx) {
    if(!s) return -1; const char* p=s; int n=0;
    while(*p){uint32_t cp=_tr_utf8_next(&p);if(n==idx)return(int)cp;n++;}
    return -1;
}
static inline char* _tr_utf8_slice(char* s, int start, int end_) {
    if(!s||start>=end_) return _tr_strdup("");
    const char* p=s; int n=0;
    while(*p&&n<start){_tr_utf8_next(&p);n++;}
    const char* begin=p;
    while(*p&&n<end_){_tr_utf8_next(&p);n++;}
    size_t len=(size_t)(p-begin);
    char* out=(char*)TAURARO_ALLOC(len+1);if(!out) return _tr_strdup("");
    memcpy(out,begin,len);out[len]='\0';return out;
}
static inline bool _tr_unicode_is_letter(int cp) {
    if((cp>=65&&cp<=90)||(cp>=97&&cp<=122)) return true;
    if(cp>=0xC0&&cp<=0x2AF) return true;
    if(cp>=0x4E00&&cp<=0x9FFF) return true;
    if(cp>=0x0400&&cp<=0x04FF) return true;
    if(cp>=0x0600&&cp<=0x06FF) return true;
    if(cp>=0xAC00&&cp<=0xD7AF) return true;
    return false;
}
static inline bool _tr_unicode_is_digit(int cp) {
    return (cp>=48&&cp<=57)||(cp>=0x0660&&cp<=0x0669)||(cp>=0x06F0&&cp<=0x06F9);
}
static inline int _tr_unicode_to_upper(int cp) {
    if(cp>=97&&cp<=122) return cp-32;
    if(cp>=0xE0&&cp<=0xFE&&cp!=0xF7) return cp-32;
    return cp;
}
static inline int _tr_unicode_to_lower(int cp) {
    if(cp>=65&&cp<=90) return cp+32;
    if(cp>=0xC0&&cp<=0xDE&&cp!=0xD7) return cp+32;
    return cp;
}
static inline char* _tr_utf8_to_upper(char* s) {
    if(!s) return _tr_strdup("");
    size_t cap=strlen(s)*4+4; char* out=(char*)TAURARO_ALLOC(cap);if(!out) return _tr_strdup("");
    const char* p=s; char* q=out; char tmp[5];
    while(*p){uint32_t cp=_tr_utf8_next(&p);int n=_tr_utf8_encode_cp((uint32_t)_tr_unicode_to_upper((int)cp),tmp);memcpy(q,tmp,(size_t)n);q+=n;}
    *q='\0'; return out;
}
static inline char* _tr_utf8_to_lower(char* s) {
    if(!s) return _tr_strdup("");
    size_t cap=strlen(s)*4+4; char* out=(char*)TAURARO_ALLOC(cap);if(!out) return _tr_strdup("");
    const char* p=s; char* q=out; char tmp[5];
    while(*p){uint32_t cp=_tr_utf8_next(&p);int n=_tr_utf8_encode_cp((uint32_t)_tr_unicode_to_lower((int)cp),tmp);memcpy(q,tmp,(size_t)n);q+=n;}
    *q='\0'; return out;
}
/* Return the codepoint category string: "L"=letter, "N"=digit, "Z"=space, "C"=other */
static inline char* _tr_unicode_category(int cp) {
    if(_tr_unicode_is_letter(cp)) return _tr_strdup("L");
    if(_tr_unicode_is_digit(cp))  return _tr_strdup("N");
    if(cp==32||cp==9||cp==10||cp==13) return _tr_strdup("Z");
    return _tr_strdup("C");
}

/* ── std.gpu helpers ─────────────────────────────────────────────────────── */
static inline bool _tr_gpu_is_openmp(void) {
#ifdef _OPENMP
    return true;
#else
    return false;
#endif
}

static inline int64_t _tr_gpu_thread_id(void) {
#ifdef _OPENMP
    return (int64_t)omp_get_thread_num();
#else
    return 0;
#endif
}

static inline int64_t _tr_gpu_num_threads(void) {
#ifdef _OPENMP
    return (int64_t)omp_get_num_threads();
#else
    return 1;
#endif
}

static inline void _tr_gpu_openmp_parallel_i64(int64_t n, void* fn_ptr) {
    typedef void (*_tr_gpu_fn)(int64_t);
    _tr_gpu_fn fn = (_tr_gpu_fn)fn_ptr;
#ifdef _OPENMP
    #pragma omp parallel for schedule(static)
    for (int64_t _i = 0; _i < n; _i++) { fn(_i); }
#else
    for (int64_t _i = 0; _i < n; _i++) { fn(_i); }
#endif
}

#endif /* TAURARO_RT_H */
