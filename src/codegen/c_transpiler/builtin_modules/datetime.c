// ==========================================
// DATETIME MODULE - Pure C Implementation
// ==========================================
// Provides: Date and time manipulation matching Python's datetime module
// Platform: Cross-platform (uses standard C time.h)

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

// Constants
#define DATETIME_MINYEAR 1
#define DATETIME_MAXYEAR 9999

// Datetime object structure (stored in TauValue as string for simplicity)
// Format: "YYYY-MM-DD HH:MM:SS.microsecond"

// ==========================================
// HELPER FUNCTIONS
// ==========================================

static inline TauValue tauraro_string_new(const char* str) {
    return (TauValue){.type = 2, .value.s = strdup(str), .refcount = 1, .next = NULL};
}

// ==========================================
// DATETIME CLASS FUNCTIONS
// ==========================================

// datetime.now() - Returns current local datetime
static inline TauValue tauraro_datetime_now(void) {
    time_t now_time = time(NULL);
    struct tm* now_tm = localtime(&now_time);

    #ifdef _WIN32
        SYSTEMTIME st;
        GetLocalTime(&st);
        int microsecond = st.wMilliseconds * 1000;
    #else
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int microsecond = tv.tv_usec;
    #endif

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.%06d",
             now_tm->tm_year + 1900,
             now_tm->tm_mon + 1,
             now_tm->tm_mday,
             now_tm->tm_hour,
             now_tm->tm_min,
             now_tm->tm_sec,
             microsecond);

    return tauraro_string_new(buffer);
}

// datetime.utcnow() - Returns current UTC datetime
static inline TauValue tauraro_datetime_utcnow(void) {
    time_t now_time = time(NULL);
    struct tm* now_tm = gmtime(&now_time);

    #ifdef _WIN32
        SYSTEMTIME st;
        GetSystemTime(&st);
        int microsecond = st.wMilliseconds * 1000;
    #else
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int microsecond = tv.tv_usec;
    #endif

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.%06d",
             now_tm->tm_year + 1900,
             now_tm->tm_mon + 1,
             now_tm->tm_mday,
             now_tm->tm_hour,
             now_tm->tm_min,
             now_tm->tm_sec,
             microsecond);

    return tauraro_string_new(buffer);
}

// datetime(year, month, day, hour, minute, second, microsecond)
static inline TauValue tauraro_datetime_new(TauValue year, TauValue month, TauValue day,
                                             TauValue hour, TauValue minute, TauValue second,
                                             TauValue microsecond) {
    int y = (year.type == 0) ? (int)year.value.i : (int)year.value.f;
    int mo = (month.type == 0) ? (int)month.value.i : (int)month.value.f;
    int d = (day.type == 0) ? (int)day.value.i : (int)day.value.f;
    int h = (hour.type == 0) ? (int)hour.value.i : (int)hour.value.f;
    int mi = (minute.type == 0) ? (int)minute.value.i : (int)minute.value.f;
    int s = (second.type == 0) ? (int)second.value.i : (int)second.value.f;
    int us = (microsecond.type == 0) ? (int)microsecond.value.i : (int)microsecond.value.f;

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.%06d",
             y, mo, d, h, mi, s, us);

    return tauraro_string_new(buffer);
}

// ==========================================
// DATE CLASS FUNCTIONS
// ==========================================

// date.today() - Returns current date
static inline TauValue tauraro_date_today(void) {
    time_t now_time = time(NULL);
    struct tm* now_tm = localtime(&now_time);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d",
             now_tm->tm_year + 1900,
             now_tm->tm_mon + 1,
             now_tm->tm_mday);

    return tauraro_string_new(buffer);
}

// date(year, month, day) - Create a date object
static inline TauValue tauraro_date_new(TauValue year, TauValue month, TauValue day) {
    int y = (year.type == 0) ? (int)year.value.i : (int)year.value.f;
    int mo = (month.type == 0) ? (int)month.value.i : (int)month.value.f;
    int d = (day.type == 0) ? (int)day.value.i : (int)day.value.f;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", y, mo, d);

    return tauraro_string_new(buffer);
}

// ==========================================
// TIME CLASS FUNCTIONS
// ==========================================

// time(hour, minute, second, microsecond) - Create a time object
static inline TauValue tauraro_time_new(TauValue hour, TauValue minute, TauValue second,
                                         TauValue microsecond) {
    int h = (hour.type == 0) ? (int)hour.value.i : (int)hour.value.f;
    int mi = (minute.type == 0) ? (int)minute.value.i : (int)minute.value.f;
    int s = (second.type == 0) ? (int)second.value.i : (int)second.value.f;
    int us = (microsecond.type == 0) ? (int)microsecond.value.i : (int)microsecond.value.f;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d.%06d", h, mi, s, us);

    return tauraro_string_new(buffer);
}

// ==========================================
// TIMEDELTA CLASS FUNCTIONS
// ==========================================

// timedelta(days, seconds, microseconds) - Create a timedelta object
static inline TauValue tauraro_timedelta_new(TauValue days, TauValue seconds, TauValue microseconds) {
    int d = (days.type == 0) ? (int)days.value.i : (int)days.value.f;
    int s = (seconds.type == 0) ? (int)seconds.value.i : (int)seconds.value.f;
    int us = (microseconds.type == 0) ? (int)microseconds.value.i : (int)microseconds.value.f;

    // Normalize to total seconds
    int64_t total_seconds = (int64_t)d * 86400 + s;

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "timedelta(days=%d, seconds=%d, microseconds=%d)",
             d, s, us);

    return tauraro_string_new(buffer);
}

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

// strftime(format, time_tuple) - Format time string
static inline TauValue tauraro_datetime_strftime(TauValue format_str, TauValue datetime_str) {
    if (format_str.type != 2 || datetime_str.type != 2) {
        return tauraro_string_new("");
    }

    // Parse datetime string (YYYY-MM-DD HH:MM:SS.microsecond)
    struct tm tm_time = {0};
    int year, month, day, hour, minute, second;

    sscanf(datetime_str.value.s, "%d-%d-%d %d:%d:%d",
           &year, &month, &day, &hour, &minute, &second);

    tm_time.tm_year = year - 1900;
    tm_time.tm_mon = month - 1;
    tm_time.tm_mday = day;
    tm_time.tm_hour = hour;
    tm_time.tm_min = minute;
    tm_time.tm_sec = second;

    char buffer[256];
    strftime(buffer, sizeof(buffer), format_str.value.s, &tm_time);

    return tauraro_string_new(buffer);
}

// strptime(date_string, format) - Parse time string
static inline TauValue tauraro_datetime_strptime(TauValue date_str, TauValue format_str) {
    if (date_str.type != 2 || format_str.type != 2) {
        return tauraro_string_new("1970-01-01 00:00:00.000000");
    }

    struct tm tm_time = {0};
    #ifdef _WIN32
        // Windows doesn't have strptime, use sscanf as fallback
        sscanf(date_str.value.s, "%d-%d-%d %d:%d:%d",
               &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday,
               &tm_time.tm_hour, &tm_time.tm_min, &tm_time.tm_sec);
        tm_time.tm_year -= 1900;
        tm_time.tm_mon -= 1;
    #else
        strptime(date_str.value.s, format_str.value.s, &tm_time);
    #endif

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.000000",
             tm_time.tm_year + 1900,
             tm_time.tm_mon + 1,
             tm_time.tm_mday,
             tm_time.tm_hour,
             tm_time.tm_min,
             tm_time.tm_sec);

    return tauraro_string_new(buffer);
}

// timestamp() - Convert datetime to Unix timestamp
static inline TauValue tauraro_datetime_timestamp(TauValue datetime_str) {
    if (datetime_str.type != 2) {
        return (TauValue){.type = 1, .value.f = 0.0, .refcount = 1, .next = NULL};
    }

    struct tm tm_time = {0};
    int year, month, day, hour, minute, second, microsecond;

    sscanf(datetime_str.value.s, "%d-%d-%d %d:%d:%d.%d",
           &year, &month, &day, &hour, &minute, &second, &microsecond);

    tm_time.tm_year = year - 1900;
    tm_time.tm_mon = month - 1;
    tm_time.tm_mday = day;
    tm_time.tm_hour = hour;
    tm_time.tm_min = minute;
    tm_time.tm_sec = second;

    time_t timestamp = mktime(&tm_time);
    double result = (double)timestamp + (double)microsecond / 1000000.0;

    return (TauValue){.type = 1, .value.f = result, .refcount = 1, .next = NULL};
}

// fromtimestamp(timestamp) - Create datetime from Unix timestamp
static inline TauValue tauraro_datetime_fromtimestamp(TauValue timestamp) {
    double ts = (timestamp.type == 1) ? timestamp.value.f : (double)timestamp.value.i;

    time_t time_val = (time_t)ts;
    struct tm* tm_time = localtime(&time_val);

    int microsecond = (int)((ts - (double)time_val) * 1000000.0);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.%06d",
             tm_time->tm_year + 1900,
             tm_time->tm_mon + 1,
             tm_time->tm_mday,
             tm_time->tm_hour,
             tm_time->tm_min,
             tm_time->tm_sec,
             microsecond);

    return tauraro_string_new(buffer);
}
