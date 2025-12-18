// ==========================================
// THREADING MODULE - Pure C Implementation
// ==========================================
// Provides: Lock, RLock, Semaphore, Event, thread operations
// Platform: Cross-platform (Windows/Linux/macOS)

#ifndef TAURARO_THREADING_MODULE_H
#define TAURARO_THREADING_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
    typedef CRITICAL_SECTION MutexType;
    typedef HANDLE EventType;
#else
    #include <pthread.h>
    #include <semaphore.h>
    typedef pthread_mutex_t MutexType;
    typedef pthread_cond_t CondType;
#endif

// Thread lock structure
typedef struct {
    MutexType mutex;
    int is_locked;
#ifdef _WIN32
    DWORD owner_thread;
#else
    pthread_t owner_thread;
#endif
} ThreadLock;

// Event structure
typedef struct {
    int is_set;
#ifdef _WIN32
    HANDLE event;
#else
    CondType cond;
    MutexType mutex;
#endif
} ThreadEvent;

// Semaphore structure
typedef struct {
    int count;
#ifdef _WIN32
    HANDLE semaphore;
#else
    sem_t semaphore;
#endif
} ThreadSemaphore;

// Condition variable structure
typedef struct {
    int is_signaled;
#ifdef _WIN32
    HANDLE condition;
#else
    pthread_cond_t cond;
    pthread_mutex_t mutex;
#endif
} ThreadCondition;

// Thread local storage
typedef struct {
    char key[256];
    void* value;
} ThreadLocalData;

// Lock operations
static inline TauValue tauraro_threading_Lock(void) {
    ThreadLock* lock = (ThreadLock*)malloc(sizeof(ThreadLock));
    lock->is_locked = 0;
    
#ifdef _WIN32
    InitializeCriticalSection(&lock->mutex);
#else
    pthread_mutex_init(&lock->mutex, NULL);
#endif
    
    return (TauValue){.type = 6, .value.p = (void*)lock, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_acquire(TauValue lock) {
    if (lock.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadLock* l = (ThreadLock*)lock.value.p;
    
#ifdef _WIN32
    EnterCriticalSection(&l->mutex);
    l->is_locked = 1;
#else
    pthread_mutex_lock(&l->mutex);
    l->is_locked = 1;
#endif
    
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_release(TauValue lock) {
    if (lock.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadLock* l = (ThreadLock*)lock.value.p;
    
#ifdef _WIN32
    LeaveCriticalSection(&l->mutex);
    l->is_locked = 0;
#else
    pthread_mutex_unlock(&l->mutex);
    l->is_locked = 0;
#endif
    
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_is_locked(TauValue lock) {
    if (lock.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadLock* l = (ThreadLock*)lock.value.p;
    return (TauValue){.type = 3, .value.i = l->is_locked, .refcount = 1, .next = NULL};
}

// Event operations
static inline TauValue tauraro_threading_Event(void) {
    ThreadEvent* event = (ThreadEvent*)malloc(sizeof(ThreadEvent));
    event->is_set = 0;
    
#ifdef _WIN32
    event->event = CreateEvent(NULL, TRUE, FALSE, NULL);
#else
    pthread_cond_init(&event->cond, NULL);
    pthread_mutex_init(&event->mutex, NULL);
#endif
    
    return (TauValue){.type = 6, .value.p = (void*)event, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_set(TauValue event) {
    if (event.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadEvent* e = (ThreadEvent*)event.value.p;
    e->is_set = 1;
    
#ifdef _WIN32
    SetEvent(e->event);
#else
    pthread_cond_broadcast(&e->cond);
#endif
    
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_clear(TauValue event) {
    if (event.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadEvent* e = (ThreadEvent*)event.value.p;
    e->is_set = 0;
    
#ifdef _WIN32
    ResetEvent(e->event);
#endif
    
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_is_set(TauValue event) {
    if (event.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadEvent* e = (ThreadEvent*)event.value.p;
    return (TauValue){.type = 3, .value.i = e->is_set, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_wait(TauValue event, TauValue timeout) {
    if (event.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadEvent* e = (ThreadEvent*)event.value.p;
    int wait_ms = timeout.type == 0 ? timeout.value.i * 1000 : -1;
    
    if (e->is_set) return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
    
#ifdef _WIN32
    DWORD result = WaitForSingleObject(e->event, wait_ms == -1 ? INFINITE : wait_ms);
    return (TauValue){.type = 3, .value.i = (result == WAIT_OBJECT_0), .refcount = 1, .next = NULL};
#else
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
#endif
}

// RLock (reentrant lock)
static inline TauValue tauraro_threading_RLock(void) {
    return tauraro_threading_Lock();  // Simplified: same as Lock
}

// Semaphore
static inline TauValue tauraro_threading_Semaphore(TauValue count) {
    ThreadSemaphore* sem = (ThreadSemaphore*)malloc(sizeof(ThreadSemaphore));
    sem->count = count.type == 0 ? count.value.i : 1;
    
#ifdef _WIN32
    sem->semaphore = CreateSemaphore(NULL, sem->count, LONG_MAX, NULL);
#else
    sem_init(&sem->semaphore, 0, sem->count);
#endif
    
    return (TauValue){.type = 6, .value.p = (void*)sem, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_acquire_semaphore(TauValue sem) {
    if (sem.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadSemaphore* s = (ThreadSemaphore*)sem.value.p;
    
#ifdef _WIN32
    WaitForSingleObject(s->semaphore, INFINITE);
#else
    sem_wait(&s->semaphore);
#endif
    
    s->count--;
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_release_semaphore(TauValue sem) {
    if (sem.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadSemaphore* s = (ThreadSemaphore*)sem.value.p;
    
#ifdef _WIN32
    ReleaseSemaphore(s->semaphore, 1, NULL);
#else
    sem_post(&s->semaphore);
#endif
    
    s->count++;
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

// Condition variable operations
static inline TauValue tauraro_threading_Condition(void) {
    ThreadCondition* cond = (ThreadCondition*)malloc(sizeof(ThreadCondition));
    cond->is_signaled = 0;
    
#ifdef _WIN32
    cond->condition = CreateEvent(NULL, FALSE, FALSE, NULL);
#else
    pthread_cond_init(&cond->cond, NULL);
    pthread_mutex_init(&cond->mutex, NULL);
#endif
    
    return (TauValue){.type = 6, .value.p = (void*)cond, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_Condition_wait(TauValue cond, TauValue lock) {
    if (cond.type != 6 || lock.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadCondition* c = (ThreadCondition*)cond.value.p;
    ThreadLock* l = (ThreadLock*)lock.value.p;
    
#ifdef _WIN32
    // Release lock, wait for signal, reacquire lock
    LeaveCriticalSection(&l->mutex);
    WaitForSingleObject(c->condition, INFINITE);
    EnterCriticalSection(&l->mutex);
#else
    pthread_cond_wait(&c->cond, &l->mutex);
#endif
    
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_Condition_notify(TauValue cond) {
    if (cond.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadCondition* c = (ThreadCondition*)cond.value.p;
    c->is_signaled = 1;
    
#ifdef _WIN32
    SetEvent(c->condition);
#else
    pthread_cond_signal(&c->cond);
#endif
    
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_Condition_notify_all(TauValue cond) {
    if (cond.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ThreadCondition* c = (ThreadCondition*)cond.value.p;
    c->is_signaled = 1;
    
#ifdef _WIN32
    SetEvent(c->condition);
#else
    pthread_cond_broadcast(&c->cond);
#endif
    
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

// Thread info
static inline TauValue tauraro_threading_current_thread(void) {
    char name[64];
    
#ifdef _WIN32
    snprintf(name, sizeof(name), "MainThread");
#else
    pthread_t tid = pthread_self();
    snprintf(name, sizeof(name), "Thread-%lu", (unsigned long)tid);
#endif
    
    TauDict* thread_info = (TauDict*)malloc(sizeof(TauDict));
    // Simplified: return thread name
    return (TauValue){.type = 2, .value.s = strdup(name), .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_threading_active_count(void) {
    return (TauValue){.type = 0, .value.i = 1, .refcount = 1, .next = NULL};  // Simplified
}

#endif // TAURARO_THREADING_MODULE_H
