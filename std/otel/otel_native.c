/*
 * std/otel/otel_native.c -- OS hooks for std.otel.
 *
 *   trc_otel_now_ns       wall-clock time, nanoseconds since the Unix epoch
 *   trc_otel_fmt_double   shortest round-trip-safe "%.17g" style double text
 *   trc_otel_ctx_*        per-thread stack of active span contexts (the
 *                         "current context"), stored as short strings
 *   trc_otel_q_*          a bounded, thread-safe FIFO of byte strings with
 *                         flush tickets, used by the batch processors to hand
 *                         encoded spans / metrics / log records to their
 *                         background export thread
 *   trc_otel_slot_*       process-wide registry slots (used by the std.log
 *                         bridge, whose hook is a plain function)
 *
 * Linked through `# tauraro-cpp-shim: otel_native.c` in std/otel/native.tr.
 * Builds on Linux, macOS and Windows (Win32 CRITICAL_SECTION +
 * CONDITION_VARIABLE; POSIX pthread mutex + condvar).
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#else
#  include <pthread.h>
#  include <time.h>
#  include <sys/time.h>
#  include <errno.h>
#endif

#if defined(_MSC_VER)
#  define OTEL_TLS __declspec(thread)
#else
#  define OTEL_TLS __thread
#endif

/* ---------------------------------------------------------------- time */

int64_t trc_otel_now_ns(void) {
#if defined(_WIN32)
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);
    uint64_t t = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    /* 100 ns ticks since 1601-01-01 -> ns since 1970-01-01 */
    return (int64_t)((t - 116444736000000000ULL) * 100ULL);
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (int64_t)ts.tv_sec * 1000000000LL + (int64_t)ts.tv_nsec;
#endif
}

int64_t trc_otel_mono_ms(void) {
#if defined(_WIN32)
    return (int64_t)GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000LL + (int64_t)(ts.tv_nsec / 1000000);
#endif
}

void trc_otel_sleep_ms(int64_t ms) {
    if (ms <= 0) return;
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000);
    ts.tv_nsec = (long)((ms % 1000) * 1000000L);
    while (nanosleep(&ts, &ts) != 0 && errno == EINTR) {}
#endif
}

/* Non-cryptographic fallback ids (splitmix64), used only when the OS random
 * source behind std.crypto.random is unavailable. Writes 2*nbytes hex chars. */
void trc_otel_fallback_hex(char* out, int64_t nbytes) {
    static uint64_t state = 0;
    if (state == 0) state = (uint64_t)trc_otel_now_ns() ^ (uint64_t)(uintptr_t)out ^ 0x9E3779B97F4A7C15ULL;
    static const char hx[] = "0123456789abcdef";
    for (int64_t i = 0; i < nbytes; i++) {
        uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        z ^= z >> 31;
        unsigned b = (unsigned)(z & 255u);
        out[2 * i] = hx[b >> 4];
        out[2 * i + 1] = hx[b & 15];
    }
}

/* JSON text for a double: shortest of %.15g/%.16g/%.17g that round-trips.
 * NaN / Inf have no JSON number form; OTLP/JSON writes them as strings, which
 * the caller handles (return -1). `out` must hold 32 bytes. */
int64_t trc_otel_fmt_double(double v, char* out) {
    if (isnan(v) || isinf(v)) return -1;
    int prec;
    for (prec = 15; prec <= 17; prec++) {
        snprintf(out, 32, "%.*g", prec, v);
        if (strtod(out, NULL) == v) break;
    }
    /* Make integral values unambiguous doubles is not required by JSON. */
    return (int64_t)strlen(out);
}

/* ------------------------------------------------- per-thread context */

#define OTEL_CTX_MAX 256

typedef struct {
    int depth;
    char* items[OTEL_CTX_MAX];
    int64_t lens[OTEL_CTX_MAX];
} OtelCtxStack;

static void otel_ctx_free_stack(OtelCtxStack* c) {
    if (!c) return;
    for (int i = 0; i < c->depth; i++) free(c->items[i]);
    free(c);
}

static OTEL_TLS OtelCtxStack* otel_ctx_tls = NULL;

/* The per-thread stack is heap-allocated on first use and freed by a
 * thread-exit destructor (pthread key / fiber-local storage callback). */
#if defined(_WIN32)
static DWORD otel_fls = FLS_OUT_OF_INDEXES;
static INIT_ONCE otel_fls_once = INIT_ONCE_STATIC_INIT;
static void WINAPI otel_ctx_dtor(void* p) { otel_ctx_free_stack((OtelCtxStack*)p); }
static BOOL CALLBACK otel_fls_init(PINIT_ONCE o, PVOID a, PVOID* c) {
    (void)o; (void)a; (void)c;
    otel_fls = FlsAlloc(otel_ctx_dtor);
    return TRUE;
}
#else
static pthread_key_t otel_key;
static pthread_once_t otel_key_once = PTHREAD_ONCE_INIT;
static void otel_ctx_dtor(void* p) { otel_ctx_free_stack((OtelCtxStack*)p); }
static void otel_key_init(void) { pthread_key_create(&otel_key, otel_ctx_dtor); }
#endif

static OtelCtxStack* otel_ctx(void) {
    if (!otel_ctx_tls) {
        otel_ctx_tls = (OtelCtxStack*)calloc(1, sizeof(OtelCtxStack));
#if defined(_WIN32)
        InitOnceExecuteOnce(&otel_fls_once, otel_fls_init, NULL, NULL);
        if (otel_fls != FLS_OUT_OF_INDEXES) FlsSetValue(otel_fls, otel_ctx_tls);
#else
        pthread_once(&otel_key_once, otel_key_init);
        pthread_setspecific(otel_key, otel_ctx_tls);
#endif
    }
    return otel_ctx_tls;
}

/* Push an encoded context; returns the new depth (the token), or -1. */
int64_t trc_otel_ctx_push(const char* s, int64_t n) {
    OtelCtxStack* c = otel_ctx();
    if (!c || c->depth >= OTEL_CTX_MAX || n < 0) return -1;
    char* copy = (char*)malloc((size_t)n + 1);
    if (!copy) return -1;
    memcpy(copy, s, (size_t)n);
    copy[n] = 0;
    c->items[c->depth] = copy;
    c->lens[c->depth] = n;
    c->depth++;
    return c->depth;
}

/* Pop back to depth token-1 (detaching everything attached after it too).
 * Returns 0 when the token was the innermost entry, 1 when entries above it
 * were dropped as well, -1 when the token is stale. */
int64_t trc_otel_ctx_pop(int64_t token) {
    OtelCtxStack* c = otel_ctx();
    if (!c || token <= 0 || token > c->depth) return -1;
    int r = (token == c->depth) ? 0 : 1;
    while (c->depth > (int)token - 1) {
        c->depth--;
        free(c->items[c->depth]);
        c->items[c->depth] = NULL;
    }
    return r;
}

int64_t trc_otel_ctx_depth(void) {
    OtelCtxStack* c = otel_ctx();
    return c ? c->depth : 0;
}

/* Length of the top entry (or -1 when empty); copy it with ctx_top_copy. */
int64_t trc_otel_ctx_top_len(void) {
    OtelCtxStack* c = otel_ctx();
    if (!c || c->depth == 0) return -1;
    return c->lens[c->depth - 1];
}

void trc_otel_ctx_top_copy(char* out) {
    OtelCtxStack* c = otel_ctx();
    if (!c || c->depth == 0) return;
    memcpy(out, c->items[c->depth - 1], (size_t)c->lens[c->depth - 1]);
}

/* Free this thread's stack (optional; call before a thread exits). */
void trc_otel_ctx_release(void) {
    if (otel_ctx_tls) {
#if defined(_WIN32)
        if (otel_fls != FLS_OUT_OF_INDEXES) FlsSetValue(otel_fls, NULL);
#else
        pthread_setspecific(otel_key, NULL);
#endif
        otel_ctx_free_stack(otel_ctx_tls);
        otel_ctx_tls = NULL;
    }
}

/* ------------------------------------------------------------- queue */

typedef struct OtelItem {
    struct OtelItem* next;
    int64_t len;
    char data[1];
} OtelItem;

typedef struct {
#if defined(_WIN32)
    CRITICAL_SECTION mu;
    CONDITION_VARIABLE cv;     /* signalled on push / flush request / close */
    CONDITION_VARIABLE done;   /* signalled when the worker finishes a flush */
#else
    pthread_mutex_t mu;
    pthread_cond_t cv;
    pthread_cond_t done;
#endif
    OtelItem* head;
    OtelItem* tail;
    int64_t len;
    int64_t cap;
    int64_t dropped;
    int64_t pushed;
    int64_t flush_req;      /* highest requested flush ticket */
    int64_t flush_done;     /* highest completed flush ticket */
    int closed;
    int worker_exited;
} OtelQueue;

static void q_lock(OtelQueue* q) {
#if defined(_WIN32)
    EnterCriticalSection(&q->mu);
#else
    pthread_mutex_lock(&q->mu);
#endif
}
static void q_unlock(OtelQueue* q) {
#if defined(_WIN32)
    LeaveCriticalSection(&q->mu);
#else
    pthread_mutex_unlock(&q->mu);
#endif
}
static void q_broadcast_cv(OtelQueue* q) {
#if defined(_WIN32)
    WakeAllConditionVariable(&q->cv);
#else
    pthread_cond_broadcast(&q->cv);
#endif
}
static void q_broadcast_done(OtelQueue* q) {
#if defined(_WIN32)
    WakeAllConditionVariable(&q->done);
#else
    pthread_cond_broadcast(&q->done);
#endif
}
/* Wait on `which` (0 = cv, 1 = done) for at most ms milliseconds. */
static void q_timedwait(OtelQueue* q, int which, int64_t ms) {
    if (ms < 0) ms = 0;
#if defined(_WIN32)
    SleepConditionVariableCS(which ? &q->done : &q->cv, &q->mu, (DWORD)ms);
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += (time_t)(ms / 1000);
    ts.tv_nsec += (long)((ms % 1000) * 1000000L);
    if (ts.tv_nsec >= 1000000000L) { ts.tv_sec += 1; ts.tv_nsec -= 1000000000L; }
    pthread_cond_timedwait(which ? &q->done : &q->cv, &q->mu, &ts);
#endif
}

void* trc_otel_q_new(int64_t cap) {
    OtelQueue* q = (OtelQueue*)calloc(1, sizeof(OtelQueue));
    if (!q) return NULL;
    q->cap = cap < 1 ? 1 : cap;
#if defined(_WIN32)
    InitializeCriticalSection(&q->mu);
    InitializeConditionVariable(&q->cv);
    InitializeConditionVariable(&q->done);
#else
    pthread_mutex_init(&q->mu, NULL);
    pthread_cond_init(&q->cv, NULL);
    pthread_cond_init(&q->done, NULL);
#endif
    return q;
}

/* 0 = queued, -1 = full (counted as dropped), -2 = closed. When `wake` is set
 * the worker is woken once the queue holds `wake` items or more. */
int64_t trc_otel_q_push(void* h, const char* s, int64_t n, int64_t wake) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q || n < 0) return -2;
    q_lock(q);
    if (q->closed) { q_unlock(q); return -2; }
    if (q->len >= q->cap) { q->dropped++; q_unlock(q); return -1; }
    OtelItem* it = (OtelItem*)malloc(sizeof(OtelItem) + (size_t)n);
    if (!it) { q->dropped++; q_unlock(q); return -1; }
    it->next = NULL;
    it->len = n;
    memcpy(it->data, s, (size_t)n);
    it->data[n] = 0;
    if (q->tail) q->tail->next = it; else q->head = it;
    q->tail = it;
    q->len++;
    q->pushed++;
    if (wake > 0 && q->len >= wake) q_broadcast_cv(q);
    q_unlock(q);
    return 0;
}

/* Worker side: wait until len >= min_items, a flush was requested, the queue
 * was closed, or timeout_ms passed. Returns the current length. */
int64_t trc_otel_q_wait(void* h, int64_t timeout_ms, int64_t min_items) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 0;
    int64_t deadline = trc_otel_mono_ms() + timeout_ms;
    q_lock(q);
    while (!q->closed && q->len < min_items && q->flush_req <= q->flush_done) {
        int64_t left = deadline - trc_otel_mono_ms();
        if (left <= 0) break;
        q_timedwait(q, 0, left);
    }
    int64_t n = q->len;
    q_unlock(q);
    return n;
}

int64_t trc_otel_q_len(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 0;
    q_lock(q);
    int64_t n = q->len;
    q_unlock(q);
    return n;
}

/* Length of the head item, or -1 when empty. Single consumer only. */
int64_t trc_otel_q_head_len(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return -1;
    q_lock(q);
    int64_t n = q->head ? q->head->len : -1;
    q_unlock(q);
    return n;
}

/* Copy the head item into out (sized from q_head_len) and remove it. */
int64_t trc_otel_q_pop(void* h, char* out) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return -1;
    q_lock(q);
    OtelItem* it = q->head;
    if (!it) { q_unlock(q); return -1; }
    q->head = it->next;
    if (!q->head) q->tail = NULL;
    q->len--;
    q_unlock(q);
    int64_t n = it->len;
    memcpy(out, it->data, (size_t)n);
    free(it);
    return n;
}

int64_t trc_otel_q_flush_ticket(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 0;
    q_lock(q);
    int64_t t = q->flush_req;
    q_unlock(q);
    return t;
}

int64_t trc_otel_q_request_flush(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 0;
    q_lock(q);
    int64_t t = ++q->flush_req;
    q_broadcast_cv(q);
    q_unlock(q);
    return t;
}

void trc_otel_q_mark_flushed(void* h, int64_t ticket) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return;
    q_lock(q);
    if (ticket > q->flush_done) q->flush_done = ticket;
    q_broadcast_done(q);
    q_unlock(q);
}

/* Wait until the worker finished `ticket` (1) or timeout (0). */
int64_t trc_otel_q_wait_flushed(void* h, int64_t ticket, int64_t timeout_ms) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 0;
    int64_t deadline = trc_otel_mono_ms() + timeout_ms;
    q_lock(q);
    while (q->flush_done < ticket && !q->worker_exited) {
        int64_t left = deadline - trc_otel_mono_ms();
        if (left <= 0) break;
        q_timedwait(q, 1, left);
    }
    int64_t ok = q->flush_done >= ticket ? 1 : 0;
    q_unlock(q);
    return ok;
}

void trc_otel_q_close(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return;
    q_lock(q);
    q->closed = 1;
    q_broadcast_cv(q);
    q_unlock(q);
}

int64_t trc_otel_q_is_closed(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 1;
    q_lock(q);
    int64_t c = q->closed;
    q_unlock(q);
    return c;
}

void trc_otel_q_worker_exit(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return;
    q_lock(q);
    q->worker_exited = 1;
    q_broadcast_done(q);
    q_unlock(q);
}

int64_t trc_otel_q_dropped(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 0;
    q_lock(q);
    int64_t d = q->dropped;
    q_unlock(q);
    return d;
}

int64_t trc_otel_q_pushed(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return 0;
    q_lock(q);
    int64_t d = q->pushed;
    q_unlock(q);
    return d;
}

/* Free a queue (and any items still in it). Only after the worker exited. */
void trc_otel_q_free(void* h) {
    OtelQueue* q = (OtelQueue*)h;
    if (!q) return;
    OtelItem* it = q->head;
    while (it) { OtelItem* nx = it->next; free(it); it = nx; }
#if defined(_WIN32)
    DeleteCriticalSection(&q->mu);
#else
    pthread_mutex_destroy(&q->mu);
    pthread_cond_destroy(&q->cv);
    pthread_cond_destroy(&q->done);
#endif
    free(q);
}

/* ------------------------------------------------- process-wide slots */

#define OTEL_SLOTS 8
static void* otel_slot_ptr[OTEL_SLOTS];
static char* otel_slot_str[OTEL_SLOTS];
static int64_t otel_slot_len[OTEL_SLOTS];
#if defined(_WIN32)
static SRWLOCK otel_slot_mu = SRWLOCK_INIT;
#  define SLOT_LOCK()   AcquireSRWLockExclusive(&otel_slot_mu)
#  define SLOT_UNLOCK() ReleaseSRWLockExclusive(&otel_slot_mu)
#else
static pthread_mutex_t otel_slot_mu = PTHREAD_MUTEX_INITIALIZER;
#  define SLOT_LOCK()   pthread_mutex_lock(&otel_slot_mu)
#  define SLOT_UNLOCK() pthread_mutex_unlock(&otel_slot_mu)
#endif

void trc_otel_slot_set(int64_t i, void* p, const char* s, int64_t n) {
    if (i < 0 || i >= OTEL_SLOTS) return;
    SLOT_LOCK();
    free(otel_slot_str[i]);
    otel_slot_str[i] = NULL;
    otel_slot_len[i] = 0;
    otel_slot_ptr[i] = p;
    if (s && n > 0) {
        otel_slot_str[i] = (char*)malloc((size_t)n + 1);
        if (otel_slot_str[i]) {
            memcpy(otel_slot_str[i], s, (size_t)n);
            otel_slot_str[i][n] = 0;
            otel_slot_len[i] = n;
        }
    }
    SLOT_UNLOCK();
}

void* trc_otel_slot_ptr(int64_t i) {
    if (i < 0 || i >= OTEL_SLOTS) return NULL;
    SLOT_LOCK();
    void* p = otel_slot_ptr[i];
    SLOT_UNLOCK();
    return p;
}

int64_t trc_otel_slot_len(int64_t i) {
    if (i < 0 || i >= OTEL_SLOTS) return 0;
    SLOT_LOCK();
    int64_t n = otel_slot_len[i];
    SLOT_UNLOCK();
    return n;
}

/* Copy up to cap bytes of the slot string; returns bytes copied. */
int64_t trc_otel_slot_copy(int64_t i, char* out, int64_t cap) {
    if (i < 0 || i >= OTEL_SLOTS) return 0;
    SLOT_LOCK();
    int64_t n = otel_slot_len[i];
    if (n > cap) n = cap;
    if (n > 0) memcpy(out, otel_slot_str[i], (size_t)n);
    SLOT_UNLOCK();
    return n;
}

/* Push to the queue held in slot i (the log bridge path): same returns as
 * trc_otel_q_push, -3 when the slot is empty. Holding the slot lock while
 * pushing keeps the queue alive against a concurrent uninstall. */
int64_t trc_otel_slot_push(int64_t i, const char* s, int64_t n, int64_t wake) {
    if (i < 0 || i >= OTEL_SLOTS) return -3;
    SLOT_LOCK();
    void* q = otel_slot_ptr[i];
    int64_t r = q ? trc_otel_q_push(q, s, n, wake) : -3;
    SLOT_UNLOCK();
    return r;
}
