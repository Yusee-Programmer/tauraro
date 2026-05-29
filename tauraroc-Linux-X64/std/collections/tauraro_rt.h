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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdatomic.h>
#include <setjmp.h>
#include <ctype.h>

// Wrappers for core library to avoid signature conflicts
static inline void* _tr_c_malloc(size_t size) {
    void* p = malloc(size);
    return p;
}
static inline void* _tr_c_calloc(size_t count, size_t size) {
    void* p = calloc(count, size);
    if (!p && count * size > 0) { fprintf(stderr, "tauraro: calloc out of memory\n"); abort(); }
    return p;
}

static inline void _tr_free(void* p) {
    if (p) {
        free(p);
    }
}
static inline void _tr_c_free(void* ptr) { _tr_free(ptr); }

static inline void _tr_print(char* s) { printf("%s\n", s); }

static inline void* _tr_c_realloc(void* ptr, size_t size) {
    void* p = realloc(ptr, size);
    return p;
}

static inline void* _tr_checked_alloc(size_t sz) {
    void* p = calloc(1, sz);
    if (!p && sz > 0) { fprintf(stderr, "tauraro: out of memory\n"); abort(); }
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

static inline void* _tr_c_memcpy(void* dst, void* src, size_t n) { return memcpy(dst, src, n); }
static inline void* _tr_c_memset(void* ptr, int val, size_t n) { return memset(ptr, val, n); }
static inline void* _tr_c_memmove(void* dst, void* src, size_t n) { return memmove(dst, src, n); }
static inline void* _tr_c_fopen(const char* path, const char* mode) { return (void*)fopen(path, mode); }
static inline int _tr_c_fclose(void* fp) { return fclose((FILE*)fp); }
static inline size_t _tr_c_fread(void* ptr, size_t size, size_t nmemb, void* fp) { return fread(ptr, size, nmemb, (FILE*)fp); }
static inline size_t _tr_c_fwrite(const void* ptr, size_t size, size_t nmemb, void* fp) { return fwrite(ptr, size, nmemb, (FILE*)fp); }
static inline int _tr_c_fseek(void* fp, long offset, int whence) { return fseek((FILE*)fp, offset, whence); }
static inline long _tr_c_ftell(void* fp) { return ftell((FILE*)fp); }
static inline char* _tr_getenv(const char* name) { return getenv(name); }

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

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

typedef HANDLE _TrThread;
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fn, arg, 0, NULL);
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

#else
#include <pthread.h>
#include <time.h>

typedef pthread_t _TrThread;
static _TrThread _tr_thread_start(void*(*fn)(void*), void* arg) {
    pthread_t t; pthread_create(&t, NULL, fn, arg); return t;
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

/* ── Channel (thread-safe queue) ────────────────────────────────────── */

typedef struct _TrChanNode { void* data; struct _TrChanNode* next; } _TrChanNode;
typedef struct {
    _TrChanNode* head; _TrChanNode* tail;
    size_t len; _TrCondMutex cm; int closed;
} _TrChan;

static _TrChan* _tr_chan_new(void) {
    _TrChan* c = (_TrChan*)calloc(1, sizeof(_TrChan));
    _tr_condmutex_init(&c->cm);
    return c;
}
static void _tr_chan_send(_TrChan* c, void* v) {
    _TrChanNode* n = (_TrChanNode*)malloc(sizeof(_TrChanNode));
    n->data = v; n->next = NULL;
    _tr_condmutex_lock(&c->cm);
    if (c->tail) c->tail->next = n; else c->head = n;
    c->tail = n; c->len++;
    _tr_condmutex_signal(&c->cm);
    _tr_condmutex_unlock(&c->cm);
}
static void* _tr_chan_recv(_TrChan* c) {
    _tr_condmutex_lock(&c->cm);
    while (!c->head && !c->closed) _tr_condmutex_wait(&c->cm);
    if (!c->head) { _tr_condmutex_unlock(&c->cm); return NULL; }
    _TrChanNode* n = c->head; void* v = n->data;
    c->head = n->next; if (!c->head) c->tail = NULL;
    c->len--; _tr_free(n);
    _tr_condmutex_unlock(&c->cm);
    return v;
}
static void _tr_chan_close(_TrChan* c) {
    _tr_condmutex_lock(&c->cm); c->closed = 1;
    _tr_condmutex_signal(&c->cm); _tr_condmutex_unlock(&c->cm);
}
static long long _tr_chan_len(_TrChan* c) { return c ? (long long)c->len : 0; }

/* ── Mutex (user-facing, heap-allocated for safe sharing) ───────────── */

typedef struct _TrMutexImpl { _TrMutex mu; } _TrMutexImpl;
typedef _TrMutexImpl* Mutex;
static Mutex Mutex_new_fn(void) {
    _TrMutexImpl* m = (_TrMutexImpl*)calloc(1, sizeof(_TrMutexImpl));
    _tr_mutex_init(&m->mu); return m;
}
static void Mutex_lock(Mutex m)   { _tr_mutex_lock(&m->mu); }
static void Mutex_unlock(Mutex m) { _tr_mutex_unlock(&m->mu); }

/* ── WaitGroup (count-down latch, heap-allocated) ────────────────────── */

typedef struct _TrWaitGroupImpl { int count; _TrCondMutex cm; } _TrWaitGroupImpl;
typedef _TrWaitGroupImpl* WaitGroup;
static WaitGroup WaitGroup_new_fn(void) {
    _TrWaitGroupImpl* w = (_TrWaitGroupImpl*)calloc(1, sizeof(_TrWaitGroupImpl));
    _tr_condmutex_init(&w->cm); return w;
}
static void WaitGroup_add(WaitGroup w, int n) {
    _tr_condmutex_lock(&w->cm); w->count += n; _tr_condmutex_unlock(&w->cm);
}
static void WaitGroup_done(WaitGroup w) {
    _tr_condmutex_lock(&w->cm);
    w->count--;
    if (w->count <= 0) _tr_condmutex_signal(&w->cm);
    _tr_condmutex_unlock(&w->cm);
}
static void WaitGroup_wait(WaitGroup w) {
    _tr_condmutex_lock(&w->cm);
    while (w->count > 0) _tr_condmutex_wait(&w->cm);
    _tr_condmutex_unlock(&w->cm);
}

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
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
        return buf;
    }
    return (char*)"";
}
static void yield_val(void* v) { (void)v; }

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

#ifdef _WIN32
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

/* ── TCP socket helpers ─────────────────────────────────────────────── */
#ifdef _WIN32
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

/* _tr_c_memset defined above */

static inline void _tr_bounds_check(long long i, size_t len) {
    if (i < 0 || (size_t)i >= len) {
        fprintf(stderr, "Index %lld out of bounds (length %zu)\n", i, len);
        abort();
    }
}

#ifdef _TR_MAIN
  #define _TR_GLOBAL
#else
  #define _TR_GLOBAL extern
#endif

/* ── TaskGroup: spawn threads + join all ─────────────────────────────── */
#define _TR_MAX_TG_THREADS 64
typedef struct { _TrThread ths[_TR_MAX_TG_THREADS]; int count; } _TrTaskGroup;
_TR_GLOBAL _TrTaskGroup _tr_tg;
static inline void _tr_tg_begin(void) { _tr_tg.count = 0; }
static inline void _tr_tg_push(_TrThread t) {
    if (_tr_tg.count < _TR_MAX_TG_THREADS) _tr_tg.ths[_tr_tg.count++] = t;
}
static inline void _tr_taskgroup_wait(void) {
    for (int i = 0; i < _tr_tg.count; i++) _tr_thread_join_wait(_tr_tg.ths[i]);
    _tr_tg.count = 0;
}

/* ── Exception stack (setjmp/longjmp based) ─────────────────────────── */

#define _TR_MAX_EXC 64
_TR_GLOBAL jmp_buf*  _tr_exc_bufs[_TR_MAX_EXC];
_TR_GLOBAL char**    _tr_exc_msgs[_TR_MAX_EXC];
_TR_GLOBAL int       _tr_exc_sp;

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
    fprintf(stderr, "Unhandled exception: %s\n", msg ? msg : "(null)");
    abort();
}

/* ── String helpers ─────────────────────────────────────────────────── */

static char* _tr_str_concat(const char* a, const char* b) {
    if (!a) a=""; if (!b) b="";
    size_t la=strlen(a), lb=strlen(b);
    char* r=(char*)malloc(la+lb+1);
    memcpy(r,a,la); memcpy(r+la,b,lb+1);
    return r;
}
static char* _tr_str_upper(const char* s) {
    if (!s) return "";
    char* r=(char*)malloc(strlen(s)+1);
    for (int i=0; (r[i]=(char)toupper((unsigned char)s[i])) || s[i]; i++);
    return r;
}
static char* _tr_str_lower(const char* s) {
    if (!s) return "";
    char* r=(char*)malloc(strlen(s)+1);
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
    if (!*s) { char* e=(char*)malloc(1); *e='\0'; return e; }
    const char* end = s+strlen(s)-1;
    while (end>s && isspace((unsigned char)*end)) end--;
    size_t len=(size_t)(end-s+1);
    char* r=(char*)malloc(len+1); memcpy(r,s,len); r[len]='\0'; return r;
}
static char* _tr_str_replace(const char* s, const char* old, const char* nw) {
    if (!s||!old||!nw) return (char*)s;
    size_t sl=strlen(s), ol=strlen(old), nl=strlen(nw);
    int cnt=0; const char* p=s;
    while ((p=strstr(p,old))) { cnt++; p+=ol; }
    char* r=(char*)malloc(sl+(size_t)cnt*(nl>ol?nl-ol:0)+1);
    char* dst=r; p=s;
    while (*p) {
        if (strncmp(p,old,ol)==0) { memcpy(dst,nw,nl); dst+=nl; p+=ol; }
        else { *dst++=*p++; }
    }
    *dst='\0'; return r;
}
static char*     _tr_int_to_str(long long n)   { char* b=(char*)malloc(32); snprintf(b,32,"%lld",n); return b; }
static char*     _tr_float_to_str(double n)    { char* b=(char*)malloc(32); snprintf(b,32,"%g",n);   return b; }
static char*     _tr_bool_to_str(bool b)       { return b ? "true" : "false"; }
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

/* ── Char code → 1-char string ───────────────────────────────────────── */
static inline char* _tr_char_to_str(long long code) {
    char* s = (char*)_tr_checked_alloc(2);
    s[0] = (char)(code & 0xFF);
    s[1] = '\0';
    return s;
}
static inline char* _tr_char_to_str_alloc(long long code) { return _tr_char_to_str(code); }

/* ── Shell command execution ─────────────────────────────────────────── */
static inline int _tr_system(const char* cmd) { return system(cmd); }

/* ── Panic / error ───────────────────────────────────────────────────── */
static inline void _tr_panic(const char* msg) {
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

/* ── List types (bootstrap phase) ─────────────────────────────────── */

typedef struct { long long* data; size_t len; size_t capacity; } List_i64;
static inline List_i64* List_i64_new(void) { List_i64* l=(List_i64*)malloc(sizeof(List_i64)); l->data=(long long*)malloc(sizeof(long long)*8); l->len=0; l->capacity=8; return l; }
static inline void List_i64_append(List_i64* l, long long val) { if(l->len==l->capacity){ l->capacity*=2; l->data=(long long*)realloc(l->data,sizeof(long long)*l->capacity); } l->data[l->len++]=val; }
static inline bool List_i64_contains(List_i64* l, long long val) { for (size_t i = 0; i < l->len; i++) { if (l->data[i] == val) return true; } return false; }
static inline long long List_i64_pop(List_i64* l) { if(!l||l->len==0) return 0LL; l->len--; return l->data[l->len]; }
static inline void List_i64_set(List_i64* l, long long i, long long v) { if(l&&(size_t)i<l->len) l->data[i]=v; }
static inline long long List_i64_get(List_i64* l, long long i) { if(l&&(size_t)i<l->len) return l->data[i]; return 0LL; }
static inline void List_i64_free(List_i64* l) { if(l){ _tr_free(l->data); _tr_free(l); } }

typedef struct { double* data; size_t len; size_t capacity; } List_f64;
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

static inline List_str* _tr_str_split(const char* s, const char* sep) {
    List_str* l=List_str_new(); if(!s||!sep||!*sep) return l;
    char* cp=(char*)malloc(strlen(s)+1); strcpy(cp,s);
    char* tok=strtok(cp,(char*)sep);
    while(tok){ List_str_append(l,strdup(tok)); tok=strtok(NULL,(char*)sep); }
    _tr_free(cp); return l;
}

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
typedef struct { char* data; long long len; long long capacity; } StringObj;
typedef struct StringBuilder { StringObj* buf; } StringBuilder;

static inline StringBuilder* StringBuilder_init(long long cap) {
    StringBuilder* sb = (StringBuilder*)_tr_checked_alloc(sizeof(StringBuilder));
    sb->buf = (StringObj*)_tr_checked_alloc(sizeof(StringObj));
    if (cap < 64) cap = 64;
    sb->buf->data = (char*)_tr_checked_alloc((size_t)cap);
    sb->buf->data[0] = '\0';
    sb->buf->len = 0;
    sb->buf->capacity = cap;
    return sb;
}
static inline void StringBuilder_append(StringBuilder* sb, char* s) {
    long long slen = (long long)strlen(s);
    if (sb->buf->len + slen + 1 > sb->buf->capacity) {
        long long newcap = sb->buf->capacity * 2 + slen + 1;
        sb->buf->data = (char*)realloc(sb->buf->data, (size_t)newcap);
        sb->buf->capacity = newcap;
    }
    memcpy(sb->buf->data + sb->buf->len, s, (size_t)slen);
    sb->buf->len += slen;
    sb->buf->data[sb->buf->len] = '\0';
}
static inline void StringBuilder_append_char(StringBuilder* sb, long long c) {
    if (sb->buf->len + 2 > sb->buf->capacity) {
        long long newcap = sb->buf->capacity * 2 + 2;
        sb->buf->data = (char*)realloc(sb->buf->data, (size_t)newcap);
        sb->buf->capacity = newcap;
    }
    sb->buf->data[sb->buf->len++] = (char)c;
    sb->buf->data[sb->buf->len] = '\0';
}
static inline char* StringBuilder_to_string(StringBuilder* sb) {
    char* out = (char*)_tr_checked_alloc((size_t)(sb->buf->len + 1));
    memcpy(out, sb->buf->data, (size_t)(sb->buf->len + 1));
    return out;
}
static inline long long StringBuilder_length(StringBuilder* sb) { return sb->buf->len; }
static inline void StringBuilder_free(StringBuilder* sb) {
    free(sb->buf->data); free(sb->buf); free(sb);
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

#endif /* TAURARO_RT_H */
