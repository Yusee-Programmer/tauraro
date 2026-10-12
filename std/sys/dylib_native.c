/*
 * std/sys/dylib_native.c -- dynamic library loading for std.sys.dylib:
 * dlopen/dlsym (POSIX) or LoadLibraryW/GetProcAddress (Windows), plus call
 * trampolines with exact prototypes for the signatures std.sys.dylib exposes.
 *
 * Linked through `# tauraro-cpp-shim: dylib_native.c` in std/sys/dylib.tr.
 */
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(_GNU_SOURCE)
#  define _GNU_SOURCE
#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <dlfcn.h>
#endif

/* Same layout as the runtime's TrStr (char* data; size_t len; long* rc). */
typedef struct { char* data; size_t len; long* rc; } dl_str;

static char dl_err[512];

static void dl_set_err(const char* what, const char* detail) {
    size_t a = strlen(what), b = detail ? strlen(detail) : 0;
    if (a > sizeof dl_err - 1) a = sizeof dl_err - 1;
    memcpy(dl_err, what, a);
    if (b > sizeof dl_err - 1 - a) b = sizeof dl_err - 1 - a;
    if (b) memcpy(dl_err + a, detail, b);
    dl_err[a + b] = 0;
}

#if defined(_WIN32)
static void dl_win_err(const char* what) {
    char buf[300]; DWORD e = GetLastError(); DWORD n;
    n = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e, 0, buf, sizeof buf, NULL);
    while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r' || buf[n - 1] == '.')) buf[--n] = 0;
    if (n == 0) { buf[0] = 0; }
    dl_set_err(what, buf);
}
#endif

/* Last error text (NUL-terminated, static). */
const char* trc_dl_error(void) { return dl_err; }

/* flags: bit 0 = global symbols (RTLD_GLOBAL), bit 1 = lazy binding. Returns a handle or 0. */
int64_t trc_dl_open(const char* path, int64_t flags) {
#if defined(_WIN32)
    wchar_t wpath[1024]; HMODULE h;
    (void)flags;
    if (!MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, 1024)) { dl_set_err("path too long", ""); return 0; }
    h = LoadLibraryExW(wpath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!h) { dl_win_err("cannot load library: "); return 0; }
    return (int64_t)(intptr_t)h;
#else
    int mode = (flags & 2) ? RTLD_LAZY : RTLD_NOW;
    void* h;
    mode |= (flags & 1) ? RTLD_GLOBAL : RTLD_LOCAL;
    h = dlopen(path[0] ? path : NULL, mode);
    if (!h) { const char* e = dlerror(); dl_set_err("", e ? e : "dlopen failed"); return 0; }
    return (int64_t)(intptr_t)h;
#endif
}

int64_t trc_dl_sym(int64_t h, const char* name) {
#if defined(_WIN32)
    FARPROC p = GetProcAddress((HMODULE)(intptr_t)h, name);
    if (!p) { dl_win_err("symbol not found: "); return 0; }
    return (int64_t)(intptr_t)p;
#else
    void* p;
    dlerror();
    p = dlsym((void*)(intptr_t)h, name);
    if (!p) { const char* e = dlerror(); dl_set_err("", e ? e : "symbol not found"); return 0; }
    return (int64_t)(intptr_t)p;
#endif
}

int64_t trc_dl_close(int64_t h) {
#if defined(_WIN32)
    return FreeLibrary((HMODULE)(intptr_t)h) ? 0 : -1;
#else
    return dlclose((void*)(intptr_t)h) == 0 ? 0 : -1;
#endif
}

/* 1 Linux/BSD (.so), 2 macOS (.dylib), 3 Windows (.dll). */
int64_t trc_dl_platform(void) {
#if defined(_WIN32)
    return 3;
#elif defined(__APPLE__)
    return 2;
#else
    return 1;
#endif
}

/* ── call trampolines (exact prototypes per argument count) ───────────────── */
typedef int64_t I;
typedef double D;

I trc_dl_call_i(I fn, I n, const I* a) {
    void* f = (void*)(intptr_t)fn;
    switch (n) {
    case 0: return ((I(*)(void))f)();
    case 1: return ((I(*)(I))f)(a[0]);
    case 2: return ((I(*)(I, I))f)(a[0], a[1]);
    case 3: return ((I(*)(I, I, I))f)(a[0], a[1], a[2]);
    case 4: return ((I(*)(I, I, I, I))f)(a[0], a[1], a[2], a[3]);
    case 5: return ((I(*)(I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4]);
    case 6: return ((I(*)(I, I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4], a[5]);
    case 7: return ((I(*)(I, I, I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
    case 8: return ((I(*)(I, I, I, I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
    default: return 0;
    }
}

void trc_dl_call_v(I fn, I n, const I* a) {
    void* f = (void*)(intptr_t)fn;
    switch (n) {
    case 0: ((void(*)(void))f)(); break;
    case 1: ((void(*)(I))f)(a[0]); break;
    case 2: ((void(*)(I, I))f)(a[0], a[1]); break;
    case 3: ((void(*)(I, I, I))f)(a[0], a[1], a[2]); break;
    case 4: ((void(*)(I, I, I, I))f)(a[0], a[1], a[2], a[3]); break;
    case 5: ((void(*)(I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4]); break;
    case 6: ((void(*)(I, I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4], a[5]); break;
    case 7: ((void(*)(I, I, I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4], a[5], a[6]); break;
    case 8: ((void(*)(I, I, I, I, I, I, I, I))f)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]); break;
    default: break;
    }
}

D trc_dl_call_d(I fn, I n, const D* a) {
    void* f = (void*)(intptr_t)fn;
    switch (n) {
    case 0: return ((D(*)(void))f)();
    case 1: return ((D(*)(D))f)(a[0]);
    case 2: return ((D(*)(D, D))f)(a[0], a[1]);
    case 3: return ((D(*)(D, D, D))f)(a[0], a[1], a[2]);
    case 4: return ((D(*)(D, D, D, D))f)(a[0], a[1], a[2], a[3]);
    case 5: return ((D(*)(D, D, D, D, D))f)(a[0], a[1], a[2], a[3], a[4]);
    case 6: return ((D(*)(D, D, D, D, D, D))f)(a[0], a[1], a[2], a[3], a[4], a[5]);
    default: return 0.0;
    }
}

/* Functions exported by a `tauraroc --lib` library taking and returning `str`. */
dl_str trc_dl_call_s(I fn, I n, dl_str a, dl_str b, dl_str c, dl_str d) {
    void* f = (void*)(intptr_t)fn;
    dl_str none; memset(&none, 0, sizeof none);
    switch (n) {
    case 0: return ((dl_str(*)(void))f)();
    case 1: return ((dl_str(*)(dl_str))f)(a);
    case 2: return ((dl_str(*)(dl_str, dl_str))f)(a, b);
    case 3: return ((dl_str(*)(dl_str, dl_str, dl_str))f)(a, b, c);
    case 4: return ((dl_str(*)(dl_str, dl_str, dl_str, dl_str))f)(a, b, c, d);
    default: return none;
    }
}

/* int-returning function taking one `str` then up to 3 ints (e.g. a plugin
 * hook `def handle(event: str, arg: int) -> int`). */
I trc_dl_call_si(I fn, I n, dl_str s, I a, I b, I c) {
    void* f = (void*)(intptr_t)fn;
    switch (n) {
    case 0: return ((I(*)(dl_str))f)(s);
    case 1: return ((I(*)(dl_str, I))f)(s, a);
    case 2: return ((I(*)(dl_str, I, I))f)(s, a, b);
    case 3: return ((I(*)(dl_str, I, I, I))f)(s, a, b, c);
    default: return 0;
    }
}

/* C string helpers for C APIs that take/return char*. */
I trc_dl_strlen(I p) { return p ? (I)strlen((const char*)(intptr_t)p) : 0; }
void trc_dl_memcpy(char* dst, I src, I n) { if (n > 0) memcpy(dst, (const void*)(intptr_t)src, (size_t)n); }
I trc_dl_addr(const char* p) { return (I)(intptr_t)p; }
