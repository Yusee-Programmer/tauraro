/* native_abi.c — extern-linkage entry points to the Tauraro runtime, for the NATIVE
 * and LLVM backends. tauraro_rt.h is header-only `static inline` C: the C backend
 * #includes it and lets gcc inline it, but native code (machine code / LLVM IR) can't
 * include a header — it must CALL runtime functions as external symbols. These thin
 * wrappers give the runtime callable, stable symbols. Compiled once into runtime.o by
 * scripts/build_runtime_o.sh (hosted target: links with libc). Grows as the native
 * backend covers more of the language (strings, collections, ARC, etc.).
 */
#define _TR_MAIN
#include "tauraro_rt.h"

/* -- print + string helpers the native backend calls ---------------------------- */
void _tr_rt_print_i64(long long v) { printf("%lld\n", v); }
void _tr_rt_print_cstr(const char* s) { fputs(s ? s : "", stdout); fputc('\n', stdout); }

/* Concatenate two C-strings into a freshly-allocated one. (No ARC in the native
 * backend yet — this leaks; the C/LLVM backends handle ownership. Fine for -O0 dev.) */
char* _tr_rt_str_concat(const char* a, const char* b) {
    if (!a) a = "";
    if (!b) b = "";
    size_t la = 0; while (a[la]) la++;
    size_t lb = 0; while (b[lb]) lb++;
    char* r = (char*)malloc(la + lb + 1);
    if (!r) return (char*)"";
    for (size_t i = 0; i < la; i++) r[i] = a[i];
    for (size_t j = 0; j < lb; j++) r[la + j] = b[j];
    r[la + lb] = 0;
    return r;
}

/* -- List[int]: a dynamic i64 array the native backend calls. Opaque handle (void*).
 * (No ARC/free in the native backend yet — this leaks; fine for -O0 dev.) */
typedef struct { long long* data; long long len; long long cap; } _TrNList;

void* _tr_rt_list_new(void) {
    _TrNList* l = (_TrNList*)malloc(sizeof(_TrNList));
    if (!l) return 0;
    l->data = 0; l->len = 0; l->cap = 0;
    return l;
}
void _tr_rt_list_push_i64(void* h, long long v) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return;
    if (l->len == l->cap) {
        long long nc = l->cap ? l->cap * 2 : 4;
        l->data = (long long*)realloc(l->data, (size_t)nc * sizeof(long long));
        l->cap = nc;
    }
    l->data[l->len++] = v;
}
long long _tr_rt_list_len(void* h) {
    _TrNList* l = (_TrNList*)h;
    return l ? l->len : 0;
}
long long _tr_rt_list_get_i64(void* h, long long i) {
    _TrNList* l = (_TrNList*)h;
    if (!l || i < 0 || i >= l->len) return 0;   /* out-of-range -> 0 (native -O0 dev) */
    return l->data[i];
}
