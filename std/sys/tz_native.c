/*
 * std/sys/tz_native.c -- OS hooks for std.sys.tz.
 *
 *   trc_tz_readlink     readlink(2), to recover the zone name /etc/localtime
 *                       points at (POSIX; -1 on Windows).
 *   trc_tz_win_local    Windows only: the local zone from
 *                       GetDynamicTimeZoneInformation, as "KeyName\nPOSIX-TZ"
 *                       (-1 elsewhere).
 *
 * Linked through `# tauraro-cpp-shim: tz_native.c` in std/sys/tz.tr.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
#else
#  include <unistd.h>
#endif

int64_t trc_tz_readlink(const char* path, char* buf, int64_t cap) {
#if defined(_WIN32)
    (void)path; (void)buf; (void)cap;
    return -1;
#else
    if (cap <= 0) return -1;
    ssize_t n = readlink(path, buf, (size_t)cap);
    if (n < 0 || n >= cap) return -1;
    return (int64_t)n;
#endif
}

#if defined(_WIN32)
/* Append "<+HH[MM]>" for an offset east of UTC in minutes. */
static int tz_abbr(char* p, size_t cap, long east_min) {
    char sign = east_min < 0 ? '-' : '+';
    long a = east_min < 0 ? -east_min : east_min;
    if (a % 60) return snprintf(p, cap, "<%c%02ld%02ld>", sign, a / 60, a % 60);
    return snprintf(p, cap, "<%c%02ld>", sign, a / 60);
}

/* Append a POSIX offset (minutes WEST of UTC, i.e. the Windows bias). */
static int tz_off(char* p, size_t cap, long west_min) {
    const char* sign = west_min < 0 ? "-" : "";
    long a = west_min < 0 ? -west_min : west_min;
    if (a % 60) return snprintf(p, cap, "%s%ld:%02ld", sign, a / 60, a % 60);
    return snprintf(p, cap, "%s%ld", sign, a / 60);
}

static int tz_rule(char* p, size_t cap, const SYSTEMTIME* st) {
    return snprintf(p, cap, ",M%d.%d.%d/%d:%02d:%02d", (int)st->wMonth, (int)st->wDay,
                    (int)st->wDayOfWeek, (int)st->wHour, (int)st->wMinute, (int)st->wSecond);
}
#endif

int64_t trc_tz_win_local(char* buf, int64_t cap) {
#if defined(_WIN32)
    DYNAMIC_TIME_ZONE_INFORMATION z;
    char out[512];
    size_t n = 0;
    memset(&z, 0, sizeof z);
    if (GetDynamicTimeZoneInformation(&z) == TIME_ZONE_ID_INVALID) return -1;
    int kn = WideCharToMultiByte(CP_UTF8, 0, z.TimeZoneKeyName, -1, out, 200, NULL, NULL);
    if (kn <= 0) kn = 1;
    n = (size_t)(kn - 1);
    out[n++] = '\n';
    long std_west = (long)z.Bias + (long)z.StandardBias;
    long dst_west = (long)z.Bias + (long)z.DaylightBias;
    n += (size_t)tz_abbr(out + n, sizeof out - n, -std_west);
    n += (size_t)tz_off(out + n, sizeof out - n, std_west);
    if (z.DaylightDate.wMonth != 0 && z.StandardDate.wMonth != 0 && z.DaylightDate.wYear == 0 &&
        !z.DynamicDaylightTimeDisabled && dst_west != std_west) {
        n += (size_t)tz_abbr(out + n, sizeof out - n, -dst_west);
        n += (size_t)tz_off(out + n, sizeof out - n, dst_west);
        n += (size_t)tz_rule(out + n, sizeof out - n, &z.DaylightDate);
        n += (size_t)tz_rule(out + n, sizeof out - n, &z.StandardDate);
    }
    if ((int64_t)n > cap) return -1;
    memcpy(buf, out, n);
    return (int64_t)n;
#else
    (void)buf; (void)cap;
    return -1;
#endif
}
