// ==========================================
// TIME MODULE - Pure C Implementation
// ==========================================
// Provides: time.time(), time.sleep(), time.perf_counter()
// Platform: Cross-platform (Windows/Linux/macOS)

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
    #include <unistd.h>
    #include <sys/time.h>
#endif

// time.time() - Returns current Unix timestamp as float
static inline TauValue tauraro_time_time(void) {
    double time_val;

    #ifdef _WIN32
        // Windows: Use GetSystemTimeAsFileTime
        FILETIME ft;
        ULARGE_INTEGER uli;
        GetSystemTimeAsFileTime(&ft);
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        // Convert from 100-nanosecond intervals since 1601 to Unix epoch
        time_val = (double)(uli.QuadPart / 10000000ULL - 11644473600ULL);
        time_val += (double)(uli.QuadPart % 10000000ULL) / 10000000.0;
    #else
        // Unix/Linux/macOS: Use clock_gettime with CLOCK_REALTIME
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        time_val = (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
    #endif

    return (TauValue){
        .type = 1,              // Float type
        .value.f = time_val,
        .refcount = 1,
        .next = NULL
    };
}

// time.sleep(seconds) - Sleep for specified duration
static inline TauValue tauraro_time_sleep(TauValue duration) {
    double seconds;

    // Convert duration to double
    if (duration.type == 1) {
        seconds = duration.value.f;
    } else if (duration.type == 0) {
        seconds = (double)duration.value.i;
    } else {
        // Invalid type - return None
        return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
    }

    if (seconds < 0) seconds = 0;

    #ifdef _WIN32
        // Windows: Sleep takes milliseconds
        Sleep((DWORD)(seconds * 1000));
    #else
        // Unix: usleep takes microseconds (deprecated but widely available)
        // For better precision, use nanosleep
        struct timespec req, rem;
        req.tv_sec = (time_t)seconds;
        req.tv_nsec = (long)((seconds - (double)req.tv_sec) * 1000000000.0);

        // Handle interrupts
        while (nanosleep(&req, &rem) == -1) {
            req = rem;
        }
    #endif

    // Return None
    return (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};
}

// time.perf_counter() - High-resolution performance counter
static inline TauValue tauraro_time_perf_counter(void) {
    double time_val;

    #ifdef _WIN32
        // Windows: Use QueryPerformanceCounter
        static LARGE_INTEGER frequency = {0};
        LARGE_INTEGER counter;

        if (frequency.QuadPart == 0) {
            QueryPerformanceFrequency(&frequency);
        }
        QueryPerformanceCounter(&counter);
        time_val = (double)counter.QuadPart / (double)frequency.QuadPart;
    #else
        // Unix: Use CLOCK_MONOTONIC for better precision
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        time_val = (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
    #endif

    return (TauValue){
        .type = 1,
        .value.f = time_val,
        .refcount = 1,
        .next = NULL
    };
}

// time.monotonic() - Monotonic clock (cannot go backwards)
static inline TauValue tauraro_time_monotonic(void) {
    return tauraro_time_perf_counter();  // Same implementation
}
