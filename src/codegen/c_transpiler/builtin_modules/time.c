// ==========================================
// TIME MODULE - Pure C Implementation
// ==========================================
// Provides: Complete time module matching Python's time module
// Platform: Cross-platform (Windows/Linux/macOS)

#ifndef TAURARO_TIME_MODULE_H
#define TAURARO_TIME_MODULE_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/time.h>
#endif

// ==========================================
// BASIC TIME FUNCTIONS
// ==========================================

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
    return (TauValue){.type = 3, .value.i = 0, .refcount = 1, .next = NULL};  // None
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

    return (TauValue){.type = 1, .value.f = time_val, .refcount = 1, .next = NULL};
}

// time.monotonic() - Monotonic clock
static inline TauValue tauraro_time_monotonic(void) {
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
        // Unix: Use CLOCK_MONOTONIC
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        time_val = (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
    #endif

    return (TauValue){.type = 1, .value.f = time_val, .refcount = 1, .next = NULL};
}

// time.process_time() - Process CPU time
static inline TauValue tauraro_time_process_time(void) {
    #ifdef _WIN32
        // Windows: Use GetProcessTimes
        HANDLE hProcess = GetCurrentProcess();
        FILETIME creation, exit, kernel, user;
        GetProcessTimes(hProcess, &creation, &exit, &kernel, &user);

        ULARGE_INTEGER ulUser;
        ulUser.LowPart = user.dwLowDateTime;
        ulUser.HighPart = user.dwHighDateTime;

        ULARGE_INTEGER ulKernel;
        ulKernel.LowPart = kernel.dwLowDateTime;
        ulKernel.HighPart = kernel.dwHighDateTime;

        // Convert from 100-nanosecond intervals to seconds
        double time_val = (double)(ulUser.QuadPart + ulKernel.QuadPart) / 10000000.0;
        return (TauValue){.type = 1, .value.f = time_val, .refcount = 1, .next = NULL};
    #else
        // Unix: Use times() or clock()
        clock_t clocks = clock();
        double time_val = (double)clocks / (double)CLOCKS_PER_SEC;
        return (TauValue){.type = 1, .value.f = time_val, .refcount = 1, .next = NULL};
    #endif
}

// ==========================================
// TIME CONVERSION FUNCTIONS
// ==========================================

// time.gmtime(timestamp) - Convert timestamp to UTC struct_time
static inline TauValue tauraro_time_gmtime(TauValue timestamp) {
    time_t t = (timestamp.type == 0) ? timestamp.value.i : (time_t)timestamp.value.f;
    struct tm* gm = gmtime(&t);

    // Return as list/tuple (year, month, day, hour, minute, second, weekday, yearday, isdst)
    TauList* result = malloc(sizeof(TauList));
    result->size = 9;
    result->capacity = 9;
    result->items = malloc(sizeof(TauValue) * 9);

    result->items[0] = (TauValue){.type = 0, .value.i = gm->tm_year + 1900, .refcount = 1, .next = NULL};  // year
    result->items[1] = (TauValue){.type = 0, .value.i = gm->tm_mon + 1, .refcount = 1, .next = NULL};      // month
    result->items[2] = (TauValue){.type = 0, .value.i = gm->tm_mday, .refcount = 1, .next = NULL};        // day
    result->items[3] = (TauValue){.type = 0, .value.i = gm->tm_hour, .refcount = 1, .next = NULL};        // hour
    result->items[4] = (TauValue){.type = 0, .value.i = gm->tm_min, .refcount = 1, .next = NULL};         // minute
    result->items[5] = (TauValue){.type = 0, .value.i = gm->tm_sec, .refcount = 1, .next = NULL};         // second
    result->items[6] = (TauValue){.type = 0, .value.i = gm->tm_wday, .refcount = 1, .next = NULL};        // weekday
    result->items[7] = (TauValue){.type = 0, .value.i = gm->tm_yday, .refcount = 1, .next = NULL};        // yearday
    result->items[8] = (TauValue){.type = 0, .value.i = 0, .refcount = 1, .next = NULL};                   // isdst

    return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
}

// time.localtime(timestamp) - Convert timestamp to local struct_time
static inline TauValue tauraro_time_localtime(TauValue timestamp) {
    time_t t = (timestamp.type == 0) ? timestamp.value.i : (time_t)timestamp.value.f;
    struct tm* lm = localtime(&t);

    // Return as list/tuple
    TauList* result = malloc(sizeof(TauList));
    result->size = 9;
    result->capacity = 9;
    result->items = malloc(sizeof(TauValue) * 9);

    result->items[0] = (TauValue){.type = 0, .value.i = lm->tm_year + 1900, .refcount = 1, .next = NULL};  // year
    result->items[1] = (TauValue){.type = 0, .value.i = lm->tm_mon + 1, .refcount = 1, .next = NULL};      // month
    result->items[2] = (TauValue){.type = 0, .value.i = lm->tm_mday, .refcount = 1, .next = NULL};        // day
    result->items[3] = (TauValue){.type = 0, .value.i = lm->tm_hour, .refcount = 1, .next = NULL};        // hour
    result->items[4] = (TauValue){.type = 0, .value.i = lm->tm_min, .refcount = 1, .next = NULL};         // minute
    result->items[5] = (TauValue){.type = 0, .value.i = lm->tm_sec, .refcount = 1, .next = NULL};         // second
    result->items[6] = (TauValue){.type = 0, .value.i = lm->tm_wday, .refcount = 1, .next = NULL};        // weekday
    result->items[7] = (TauValue){.type = 0, .value.i = lm->tm_yday, .refcount = 1, .next = NULL};        // yearday
    result->items[8] = (TauValue){.type = 0, .value.i = lm->tm_isdst, .refcount = 1, .next = NULL};       // isdst

    return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
}

// time.mktime(timetuple) - Convert struct_time to timestamp
static inline TauValue tauraro_time_mktime(TauValue timetuple) {
    if (timetuple.type != 4 || !timetuple.value.list || timetuple.value.list->size < 9) {
        return (TauValue){.type = 1, .value.f = 0.0, .refcount = 1, .next = NULL};
    }

    TauList* tuple = timetuple.value.list;
    struct tm tm_struct = {0};

    tm_struct.tm_year = (int)tuple->items[0].value.i - 1900;
    tm_struct.tm_mon = (int)tuple->items[1].value.i - 1;
    tm_struct.tm_mday = (int)tuple->items[2].value.i;
    tm_struct.tm_hour = (int)tuple->items[3].value.i;
    tm_struct.tm_min = (int)tuple->items[4].value.i;
    tm_struct.tm_sec = (int)tuple->items[5].value.i;
    tm_struct.tm_wday = (int)tuple->items[6].value.i;
    tm_struct.tm_yday = (int)tuple->items[7].value.i;
    tm_struct.tm_isdst = (int)tuple->items[8].value.i;

    time_t t = mktime(&tm_struct);
    return (TauValue){.type = 1, .value.f = (double)t, .refcount = 1, .next = NULL};
}

// ==========================================
// TIME FORMATTING FUNCTIONS
// ==========================================

// time.asctime(timetuple) - Format time as asctime string
static inline TauValue tauraro_time_asctime(TauValue timetuple) {
    if (timetuple.type != 4 || !timetuple.value.list) {
        return (TauValue){.type = 2, .value.s = "N/A", .refcount = 1, .next = NULL};
    }

    TauList* tuple = timetuple.value.list;
    struct tm tm_struct = {0};

    if (tuple->size >= 9) {
        tm_struct.tm_year = (int)tuple->items[0].value.i - 1900;
        tm_struct.tm_mon = (int)tuple->items[1].value.i - 1;
        tm_struct.tm_mday = (int)tuple->items[2].value.i;
        tm_struct.tm_hour = (int)tuple->items[3].value.i;
        tm_struct.tm_min = (int)tuple->items[4].value.i;
        tm_struct.tm_sec = (int)tuple->items[5].value.i;
        tm_struct.tm_wday = (int)tuple->items[6].value.i;
    }

    char buffer[64];
    asctime_r(&tm_struct, buffer);
    return (TauValue){.type = 2, .value.s = strdup(buffer), .refcount = 1, .next = NULL};
}

// time.ctime(timestamp) - Format timestamp as ctime string
static inline TauValue tauraro_time_ctime(TauValue timestamp) {
    time_t t = (timestamp.type == 0) ? timestamp.value.i : (time_t)timestamp.value.f;
    char buffer[64];
    
    struct tm* tm_info = localtime(&t);
    asctime_r(tm_info, buffer);
    return (TauValue){.type = 2, .value.s = strdup(buffer), .refcount = 1, .next = NULL};
}

// time.strftime(format, timetuple) - Format time according to format string
static inline TauValue tauraro_time_strftime(TauValue format_str, TauValue timetuple) {
    if (format_str.type != 2) {
        return (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL};
    }

    struct tm tm_struct = {0};
    
    if (timetuple.type == 4 && timetuple.value.list && timetuple.value.list->size >= 9) {
        TauList* tuple = timetuple.value.list;
        tm_struct.tm_year = (int)tuple->items[0].value.i - 1900;
        tm_struct.tm_mon = (int)tuple->items[1].value.i - 1;
        tm_struct.tm_mday = (int)tuple->items[2].value.i;
        tm_struct.tm_hour = (int)tuple->items[3].value.i;
        tm_struct.tm_min = (int)tuple->items[4].value.i;
        tm_struct.tm_sec = (int)tuple->items[5].value.i;
        tm_struct.tm_wday = (int)tuple->items[6].value.i;
        tm_struct.tm_yday = (int)tuple->items[7].value.i;
        tm_struct.tm_isdst = (int)tuple->items[8].value.i;
    }

    char buffer[256];
    strftime(buffer, sizeof(buffer), format_str.value.s, &tm_struct);
    return (TauValue){.type = 2, .value.s = strdup(buffer), .refcount = 1, .next = NULL};
}

// ==========================================
// TIMEZONE FUNCTIONS
// ==========================================

// time.timezone - Get timezone offset in seconds
static inline TauValue tauraro_time_timezone_get(void) {
    #ifdef _WIN32
        TIME_ZONE_INFORMATION tzi;
        GetTimeZoneInformation(&tzi);
        long tz_offset = -(long)tzi.Bias * 60;
    #else
        long tz_offset = -timezone;
    #endif

    return (TauValue){.type = 0, .value.i = tz_offset, .refcount = 1, .next = NULL};
}

// time.tzname - Get timezone names
static inline TauValue tauraro_time_tzname_get(void) {
    TauList* names = malloc(sizeof(TauList));
    names->size = 2;
    names->capacity = 2;
    names->items = malloc(sizeof(TauValue) * 2);

    #ifdef _WIN32
        names->items[0] = (TauValue){.type = 2, .value.s = "STD", .refcount = 1, .next = NULL};
        names->items[1] = (TauValue){.type = 2, .value.s = "DST", .refcount = 1, .next = NULL};
    #else
        names->items[0] = (TauValue){.type = 2, .value.s = tzname[0], .refcount = 1, .next = NULL};
        names->items[1] = (TauValue){.type = 2, .value.s = tzname[1], .refcount = 1, .next = NULL};
    #endif

    return (TauValue){.type = 4, .value.list = names, .refcount = 1, .next = NULL};
}

// time.daylight - Get daylight saving flag
static inline TauValue tauraro_time_daylight_get(void) {
    #ifdef _WIN32
        TIME_ZONE_INFORMATION tzi;
        GetTimeZoneInformation(&tzi);
        int daylight = (tzi.StandardBias != tzi.DaylightBias) ? 1 : 0;
    #else
        int daylight = daylight_var;
    #endif

    return (TauValue){.type = 0, .value.i = daylight, .refcount = 1, .next = NULL};
}

// time.altzone - Get alternate timezone offset (DST)
static inline TauValue tauraro_time_altzone_get(void) {
    #ifdef _WIN32
        TIME_ZONE_INFORMATION tzi;
        GetTimeZoneInformation(&tzi);
        long alt_tz = -(long)tzi.DaylightBias * 60;
    #else
        long alt_tz = -altzone;
    #endif

    return (TauValue){.type = 0, .value.i = alt_tz, .refcount = 1, .next = NULL};
}


#endif // TAURARO_TIME_MODULE_H
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


#endif // TAURARO_TIME_MODULE_H
