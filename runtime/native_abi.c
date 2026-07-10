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
#include <math.h>

/* Float math methods (x.sqrt() etc.) — thin wrappers over libm so the native backend
 * has stable extern symbols. The final link must include -lm. */
double _tr_rt_sqrt(double x)  { return sqrt(x); }
double _tr_rt_floor(double x) { return floor(x); }
double _tr_rt_ceil(double x)  { return ceil(x); }
double _tr_rt_round(double x) { return round(x); }
double _tr_rt_fabs(double x)  { return fabs(x); }
double _tr_rt_log(double x)   { return log(x); }
double _tr_rt_log2(double x)  { return log2(x); }
double _tr_rt_log10(double x) { return log10(x); }
double _tr_rt_exp(double x)   { return exp(x); }
double _tr_rt_sin(double x)   { return sin(x); }
double _tr_rt_cos(double x)   { return cos(x); }
double _tr_rt_tan(double x)   { return tan(x); }
double _tr_rt_asin(double x)  { return asin(x); }
double _tr_rt_acos(double x)  { return acos(x); }
double _tr_rt_atan(double x)  { return atan(x); }
double _tr_rt_pow(double a, double b)   { return pow(a, b); }
double _tr_rt_atan2(double a, double b) { return atan2(a, b); }
long long _tr_rt_f64_is_nan(double x) { return isnan(x) ? 1 : 0; }
long long _tr_rt_f64_is_inf(double x) { return isinf(x) ? 1 : 0; }

/* List[int]/List[str] backing store: a dynamic 8-byte-slot array (a str element is just
 * a char* stored in the same slot). Declared up top so every _tr_rt_list_* helper sees it. */
typedef struct { long long* data; long long len; long long cap; } _TrNList;

/* -- print + string helpers the native backend calls ---------------------------- */
void _tr_rt_print_i64(long long v) { printf("%lld\n", v); }
void _tr_rt_print_cstr(const char* s) { fputs(s ? s : "", stdout); fputc('\n', stdout); }
void _tr_rt_print_bool(long long v) { fputs(v ? "true" : "false", stdout); fputc('\n', stdout); }
void _tr_rt_print_f64(double v) { printf("%g\n", v); }   /* matches C backend's "%g" */
void _tr_rt_write_f64(double v) { printf("%g", v); }

/* strcmp / strlen the native backend calls for string comparison and len(). */
long long _tr_rt_str_cmp(const char* a, const char* b) {
    if (!a) a = ""; if (!b) b = "";
    return (long long)strcmp(a, b);
}
long long _tr_rt_strlen(const char* s) {
    if (!s) return 0;
    long long n = 0; while (s[n]) n++;
    return n;
}

/* No-newline writers + separators, for multi-argument print (Python-style: each arg
 * written with its own format, single-space separated, one trailing newline). */
void _tr_rt_write_i64(long long v) { printf("%lld", v); }
void _tr_rt_write_cstr(const char* s) { fputs(s ? s : "", stdout); }
void _tr_rt_write_bool(long long v) { fputs(v ? "true" : "false", stdout); }
void _tr_rt_write_sp(void) { fputc(' ', stdout); }
void _tr_rt_write_nl(void) { fputc('\n', stdout); }

/* abs / min / max integer builtins. */
long long _tr_rt_abs_i64(long long x) { return x < 0 ? -x : x; }
long long _tr_rt_min_i64(long long a, long long b) { return a < b ? a : b; }
long long _tr_rt_max_i64(long long a, long long b) { return a > b ? a : b; }

/* Conversions: str(int)/str(bool), int(str). (i64_to_str/str_repeat malloc -> leak; -O0 dev.) */
char* _tr_rt_i64_to_str(long long v) {
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "%lld", v);
    char* r = (char*)malloc((size_t)n + 1);
    if (!r) return (char*)"";
    for (int i = 0; i <= n; i++) r[i] = buf[i];
    return r;
}
char* _tr_rt_bool_to_str(long long v) { return (char*)(v ? "true" : "false"); }
long long _tr_rt_str_to_i64(const char* s) { return s ? (long long)strtoll(s, 0, 10) : 0; }

/* "ab" * 3 -> "ababab" */
char* _tr_rt_str_repeat(const char* s, long long n) {
    if (!s) s = "";
    if (n < 0) n = 0;
    size_t l = strlen(s);
    char* r = (char*)malloc(l * (size_t)n + 1);
    if (!r) return (char*)"";
    char* p = r;
    for (long long i = 0; i < n; i++) { for (size_t j = 0; j < l; j++) *p++ = s[j]; }
    *p = 0;
    return r;
}

/* xs.pop() -> last element, shrinking the list. */
long long _tr_rt_list_pop_i64(void* h) {
    _TrNList* l = (_TrNList*)h;
    if (!l || l->len == 0) return 0;
    l->len--;
    return l->data[l->len];
}

/* String methods (match tauraro_rt.h semantics: isspace/toupper/strstr). malloc -> -O0. */
char* _tr_rt_str_upper(const char* s) {
    if (!s) s = "";
    size_t n = strlen(s); char* r = (char*)malloc(n + 1);
    for (size_t i = 0; i <= n; i++) r[i] = (char)toupper((unsigned char)s[i]);
    return r;
}
char* _tr_rt_str_lower(const char* s) {
    if (!s) s = "";
    size_t n = strlen(s); char* r = (char*)malloc(n + 1);
    for (size_t i = 0; i <= n; i++) r[i] = (char)tolower((unsigned char)s[i]);
    return r;
}
char* _tr_rt_str_strip(const char* s) {
    if (!s) s = "";
    while (isspace((unsigned char)*s)) s++;
    const char* e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) e--;
    size_t l = (size_t)(e - s); char* r = (char*)malloc(l + 1);
    for (size_t i = 0; i < l; i++) r[i] = s[i];
    r[l] = 0; return r;
}
char* _tr_rt_str_replace(const char* s, const char* a, const char* b) {
    if (!s) s = ""; if (!a) a = ""; if (!b) b = "";
    size_t sl = strlen(s), al = strlen(a), bl = strlen(b);
    if (al == 0) { char* r = (char*)malloc(sl + 1); for (size_t i = 0; i <= sl; i++) r[i] = s[i]; return r; }
    int cnt = 0; const char* p = s;
    while ((p = strstr(p, a))) { cnt++; p += al; }
    char* r = (char*)malloc(sl + (bl > al ? (bl - al) * (size_t)cnt : 0) + 1);
    char* w = r; p = s; const char* q;
    while ((q = strstr(p, a))) {
        for (const char* c = p; c < q; c++) *w++ = *c;
        for (size_t j = 0; j < bl; j++) *w++ = b[j];
        p = q + al;
    }
    while (*p) *w++ = *p++;
    *w = 0; return r;
}
long long _tr_rt_str_find(const char* s, const char* sub) {
    if (!s || !sub) return -1;
    const char* p = strstr(s, sub);
    return p ? (long long)(p - s) : -1;
}
long long _tr_rt_str_starts_with(const char* s, const char* p) {
    if (!s || !p) return 0;
    return strncmp(s, p, strlen(p)) == 0 ? 1 : 0;
}
long long _tr_rt_str_ends_with(const char* s, const char* p) {
    if (!s || !p) return 0;
    size_t sl = strlen(s), pl = strlen(p);
    if (pl > sl) return 0;
    return strcmp(s + sl - pl, p) == 0 ? 1 : 0;
}
/* More string methods — bodies mirror tauraro_rt.h exactly (case/whitespace rules). */
static char* _trn_dup(const char* s) {
    size_t n = strlen(s); char* r = (char*)malloc(n + 1);
    for (size_t i = 0; i <= n; i++) r[i] = s[i]; return r;
}
char* _tr_rt_str_capitalize(const char* s) {
    if (!s || !*s) { char* e = (char*)malloc(1); e[0] = 0; return e; }
    char* r = _trn_dup(s);
    r[0] = (char)toupper((unsigned char)r[0]);
    for (size_t i = 1; r[i]; i++) r[i] = (char)tolower((unsigned char)r[i]);
    return r;
}
char* _tr_rt_str_title(const char* s) {
    if (!s) { char* e = (char*)malloc(1); e[0] = 0; return e; }
    char* r = _trn_dup(s);
    int ws = 1;
    for (size_t i = 0; r[i]; i++) {
        if (r[i] == ' ' || r[i] == '\t' || r[i] == '\n') ws = 1;
        else if (ws) { r[i] = (char)toupper((unsigned char)r[i]); ws = 0; }
        else r[i] = (char)tolower((unsigned char)r[i]);
    }
    return r;
}
char* _tr_rt_str_trim_left(const char* s) {
    if (!s) s = "";
    while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
    return _trn_dup(s);
}
char* _tr_rt_str_trim_right(const char* s) {
    if (!s) s = "";
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t' || s[n-1] == '\n' || s[n-1] == '\r')) n--;
    char* r = (char*)malloc(n + 1);
    for (size_t i = 0; i < n; i++) r[i] = s[i];
    r[n] = 0; return r;
}
char* _tr_rt_str_reverse(const char* s) {
    if (!s) s = "";
    size_t n = strlen(s); char* r = (char*)malloc(n + 1);
    for (size_t i = 0; i < n; i++) r[i] = s[n-1-i];
    r[n] = 0; return r;
}
char* _tr_rt_str_strip_prefix(const char* s, const char* pre) {
    if (!s) s = ""; if (!pre) pre = "";
    size_t pl = strlen(pre);
    if (strncmp(s, pre, pl) == 0) return _trn_dup(s + pl);
    return _trn_dup(s);
}
char* _tr_rt_str_strip_suffix(const char* s, const char* suf) {
    if (!s) s = ""; if (!suf) suf = "";
    size_t sl = strlen(s), sufl = strlen(suf);
    if (sl >= sufl && strcmp(s + sl - sufl, suf) == 0) {
        char* r = (char*)malloc(sl - sufl + 1);
        for (size_t i = 0; i < sl - sufl; i++) r[i] = s[i];
        r[sl - sufl] = 0; return r;
    }
    return _trn_dup(s);
}
char* _tr_rt_str_replace_first(const char* s, const char* a, const char* b) {
    if (!s) s = ""; if (!a) a = ""; if (!b) b = "";
    const char* p = strstr(s, a);
    if (!p || !*a) return _trn_dup(s);
    size_t ol = strlen(a), nl = strlen(b), pre = (size_t)(p - s), sl = strlen(s);
    char* r = (char*)malloc(sl - ol + nl + 1);
    char* w = r;
    for (size_t i = 0; i < pre; i++) *w++ = s[i];
    for (size_t j = 0; j < nl; j++) *w++ = b[j];
    for (const char* q = s + pre + ol; *q; q++) *w++ = *q;
    *w = 0; return r;
}
long long _tr_rt_str_parse_bool(const char* s) {
    if (!s) return 0;
    return (strcmp(s, "true") == 0 || strcmp(s, "1") == 0 || strcmp(s, "yes") == 0) ? 1 : 0;
}
long long _tr_rt_str_is_empty(const char* s) { return (!s || !*s) ? 1 : 0; }
long long _tr_rt_str_ord(const char* s) { return s ? (long long)(unsigned char)s[0] : 0; }

/* s.slice(a,b) / s.count(sub) / s.char_at(i) / s.contains(sub) — match tauraro_rt.h. */
char* _tr_rt_str_slice(const char* s, long long start, long long end) {
    char* e;
    long long len, sz;
    if (!s) { e = (char*)malloc(1); e[0] = 0; return e; }
    len = (long long)strlen(s);
    if (start < 0) start = 0;
    if (end > len) end = len;
    if (start >= end) { e = (char*)malloc(1); e[0] = 0; return e; }
    sz = end - start;
    e = (char*)malloc((size_t)sz + 1);
    for (long long i = 0; i < sz; i++) e[i] = s[start + i];
    e[sz] = 0;
    return e;
}
long long _tr_rt_str_count(const char* s, const char* sub) {
    if (!s || !sub || !*sub) return 0;
    size_t subl = strlen(sub); long long c = 0; const char* p = s;
    while ((p = strstr(p, sub))) { c++; p += subl; }
    return c;
}
long long _tr_rt_str_char_at(const char* s, long long i) {
    if (!s) return -1;
    long long n = (long long)strlen(s);
    if (i < 0 || i >= n) return -1;
    return (long long)(unsigned char)s[i];
}
long long _tr_rt_str_contains(const char* s, const char* sub) {
    if (!s || !sub) return 0;
    return strstr(s, sub) != 0 ? 1 : 0;
}

/* Int utility methods (x.to_hex() etc.) — match tauraro_rt.h formats. */
static char* _trn_fmt(const char* fmt, long long v) {
    char b[40]; int n = snprintf(b, sizeof(b), fmt, v);
    char* r = (char*)malloc((size_t)n + 1);
    for (int i = 0; i <= n; i++) r[i] = b[i];
    return r;
}
char* _tr_rt_i64_to_hex(long long v)       { return _trn_fmt("%llx", v); }
char* _tr_rt_i64_to_hex_upper(long long v) { return _trn_fmt("%llX", v); }
char* _tr_rt_i64_to_oct(long long v)       { return _trn_fmt("%llo", v); }
char* _tr_rt_i64_to_bin(long long n) {
    if (n == 0) { char* z = (char*)malloc(2); z[0] = '0'; z[1] = 0; return z; }
    char buf[70]; int pos = 68; buf[69] = 0;
    unsigned long long v = (unsigned long long)n;
    while (v > 0) { buf[pos--] = (char)('0' + (int)(v & 1)); v >>= 1; }
    size_t nd = (size_t)(68 - pos);
    char* r = (char*)malloc(nd + 1);
    for (size_t i = 0; i < nd; i++) r[i] = buf[pos + 1 + i];
    r[nd] = 0;
    return r;
}
long long _tr_rt_gcd_i64(long long a, long long b) {
    a = a < 0 ? -a : a; b = b < 0 ? -b : b;
    while (b) { long long t = b; b = a % b; a = t; }
    return a;
}
long long _tr_rt_lcm_i64(long long a, long long b) {
    long long g = _tr_rt_gcd_i64(a, b);
    return g ? (a / g * b) : 0;
}
long long _tr_rt_clamp_i64(long long v, long long lo, long long hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

long long _tr_rt_list_sum_i64(void* h) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return 0;
    long long t = 0;
    for (long long i = 0; i < l->len; i++) t += l->data[i];
    return t;
}
long long _tr_rt_list_any_i64(void* h) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return 0;
    for (long long i = 0; i < l->len; i++) if (l->data[i]) return 1;
    return 0;
}
long long _tr_rt_list_all_i64(void* h) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return 1;
    for (long long i = 0; i < l->len; i++) if (!l->data[i]) return 0;
    return 1;
}
long long _tr_rt_list_is_empty(void* h) {
    _TrNList* l = (_TrNList*)h;
    return (!l || l->len == 0) ? 1 : 0;
}
long long _tr_rt_list_first_i64(void* h) {
    _TrNList* l = (_TrNList*)h;
    return (l && l->len > 0) ? l->data[0] : 0;
}
long long _tr_rt_list_last_i64(void* h) {
    _TrNList* l = (_TrNList*)h;
    return (l && l->len > 0) ? l->data[l->len - 1] : 0;
}
void _tr_rt_list_reverse(void* h) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return;
    for (long long i = 0, j = l->len - 1; i < j; i++, j--) {
        long long t = l->data[i]; l->data[i] = l->data[j]; l->data[j] = t;
    }
}
void _tr_rt_list_clear(void* h) {
    _TrNList* l = (_TrNList*)h;
    if (l) l->len = 0;
}
/* in-place insertion sort; dir>0 ascending, dir<0 descending. */
void _tr_rt_list_sort(void* h, long long dir) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return;
    for (long long i = 1; i < l->len; i++) {
        long long v = l->data[i], j = i - 1;
        while (j >= 0 && (dir > 0 ? l->data[j] > v : l->data[j] < v)) { l->data[j + 1] = l->data[j]; j--; }
        l->data[j + 1] = v;
    }
}

/* `x in xs` membership for List[int] / List[str]. */
long long _tr_rt_list_contains_i64(void* h, long long v) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return 0;
    for (long long i = 0; i < l->len; i++) if (l->data[i] == v) return 1;
    return 0;
}
long long _tr_rt_list_contains_str(void* h, const char* s) {
    _TrNList* l = (_TrNList*)h;
    if (!l) return 0;
    if (!s) s = "";
    for (long long i = 0; i < l->len; i++) {
        const char* e = (const char*)l->data[i];
        if (!e) e = "";
        if (strcmp(e, s) == 0) return 1;
    }
    return 0;
}

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
