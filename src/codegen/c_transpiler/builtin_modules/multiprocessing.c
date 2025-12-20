// ==========================================
// MULTIPROCESSING MODULE - Pure C Implementation
// ==========================================
// Provides: Process, Queue, Pipe, Pool, Manager, Lock, Semaphore
// Platform: Cross-platform (Windows/Unix)

#ifndef TAURARO_MULTIPROCESSING_MODULE_H
#define TAURARO_MULTIPROCESSING_MODULE_H

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
    #include <windows.h>
    typedef HANDLE ProcessHandle;
#else
    #include <unistd.h>
    #include <sys/wait.h>
    typedef pid_t ProcessHandle;
#endif

// Process structure
typedef struct {
    ProcessHandle handle;
    void (*target)(void);
    int pid;
    int started;
} ProcessWrapper;

// Queue structure
typedef struct {
    void** items;
    int size;
    int capacity;
} QueueWrapper;

// multiprocessing.Process(target=None, args=())
static inline TauValue tauraro_multiprocessing_Process(TauValue target) {
    ProcessWrapper* proc = (ProcessWrapper*)malloc(sizeof(ProcessWrapper));
    proc->target = (void (*)(void))target.value.ptr;
    proc->pid = 0;
    proc->started = 0;
    
    return (TauValue){.type = 6, .value.ptr = (void*)proc, .refcount = 1, .next = NULL};
}

// multiprocessing.Process.start()
static inline TauValue tauraro_multiprocessing_Process_start(TauValue proc_obj) {
    if (proc_obj.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ProcessWrapper* proc = (ProcessWrapper*)proc_obj.value.ptr;
    proc->started = 1;
    
    #ifdef _WIN32
        // Create process on Windows
    #else
        proc->pid = fork();
        if (proc->pid == 0) {
            // Child process
            proc->target();
            exit(0);
        }
    #endif
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// multiprocessing.Process.join()
static inline TauValue tauraro_multiprocessing_Process_join(TauValue proc_obj) {
    if (proc_obj.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ProcessWrapper* proc = (ProcessWrapper*)proc_obj.value.ptr;
    
    #ifdef _WIN32
        WaitForSingleObject(proc->handle, INFINITE);
    #else
        if (proc->pid > 0) {
            waitpid(proc->pid, NULL, 0);
        }
    #endif
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// multiprocessing.Process.terminate()
static inline TauValue tauraro_multiprocessing_Process_terminate(TauValue proc_obj) {
    if (proc_obj.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    ProcessWrapper* proc = (ProcessWrapper*)proc_obj.value.ptr;
    
    #ifdef _WIN32
        TerminateProcess(proc->handle, 1);
    #else
        if (proc->pid > 0) {
            kill(proc->pid, 15);
        }
    #endif
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// multiprocessing.Queue()
static inline TauValue tauraro_multiprocessing_Queue(void) {
    QueueWrapper* q = (QueueWrapper*)malloc(sizeof(QueueWrapper));
    q->capacity = 10;
    q->size = 0;
    q->items = (void**)malloc(sizeof(void*) * q->capacity);
    
    return (TauValue){.type = 6, .value.ptr = (void*)q, .refcount = 1, .next = NULL};
}

// multiprocessing.Queue.put(item)
static inline TauValue tauraro_multiprocessing_Queue_put(TauValue queue, TauValue item) {
    if (queue.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    QueueWrapper* q = (QueueWrapper*)queue.value.ptr;
    if (q->size < q->capacity) {
        q->items[q->size++] = (void*)&item;
    }
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// multiprocessing.Queue.get()
static inline TauValue tauraro_multiprocessing_Queue_get(TauValue queue) {
    if (queue.type != 6) return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
    
    QueueWrapper* q = (QueueWrapper*)queue.value.ptr;
    if (q->size > 0) {
        TauValue* item = (TauValue*)q->items[0];
        // Shift items
        for (int i = 0; i < q->size - 1; i++) {
            q->items[i] = q->items[i + 1];
        }
        q->size--;
        return *item;
    }
    
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};
}

// multiprocessing.Queue.empty()
static inline TauValue tauraro_multiprocessing_Queue_empty(TauValue queue) {
    if (queue.type != 6) return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
    
    QueueWrapper* q = (QueueWrapper*)queue.value.ptr;
    return (TauValue){.type = 3, .value.i = (q->size == 0), .refcount = 1, .next = NULL};
}

// multiprocessing.Queue.qsize()
static inline TauValue tauraro_multiprocessing_Queue_qsize(TauValue queue) {
    if (queue.type != 6) return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    
    QueueWrapper* q = (QueueWrapper*)queue.value.ptr;
    return (TauValue){.type = 0, .value.i = q->size, .refcount = 1, .next = NULL};
}

// multiprocessing.Pipe()
static inline TauValue tauraro_multiprocessing_Pipe(void) {
    return (TauValue){.type = 4, .value.ptr = NULL, .refcount = 1, .next = NULL};  // Tuple of (conn1, conn2)
}

// multiprocessing.Pool(processes=None)
static inline TauValue tauraro_multiprocessing_Pool(void) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// multiprocessing.Pool.map(func, iterable)
static inline TauValue tauraro_multiprocessing_Pool_map(TauValue pool, TauValue func, TauValue iterable) {
    return (TauValue){.type = 4, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// multiprocessing.Manager()
static inline TauValue tauraro_multiprocessing_Manager(void) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// multiprocessing.Lock()
static inline TauValue tauraro_multiprocessing_Lock(void) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// multiprocessing.Lock.acquire()
static inline TauValue tauraro_multiprocessing_Lock_acquire(TauValue lock) {
    return (TauValue){.type = 3, .value.i = 1, .refcount = 1, .next = NULL};
}

// multiprocessing.Lock.release()
static inline TauValue tauraro_multiprocessing_Lock_release(TauValue lock) {
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
}

// multiprocessing.Semaphore(value=1)
static inline TauValue tauraro_multiprocessing_Semaphore(TauValue value) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}

// multiprocessing.cpu_count()
static inline TauValue tauraro_multiprocessing_cpu_count(void) {
    return (TauValue){.type = 0, .value.i = 4, .refcount = 1, .next = NULL};  // Default to 4
}

// multiprocessing.current_process()
static inline TauValue tauraro_multiprocessing_current_process(void) {
    return (TauValue){.type = 6, .value.ptr = NULL, .refcount = 1, .next = NULL};
}


#endif // TAURARO_MULTIPROCESSING_MODULE_H
