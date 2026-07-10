#include "../../tauraro_types.h"


__attribute__((hot)) char* raw_alloc(long long size) {
    /* pass */
    return _tr_checked_alloc(size);
}

__attribute__((hot)) char* raw_realloc(char* ptr, long long size) {
    /* pass */
    return _tr_c_realloc(ptr, size);
}

__attribute__((hot)) void raw_free(char* ptr) {
    /* pass */
    _tr_c_free(ptr);
}

__attribute__((hot)) void raw_copy(char* dst, char* src, long long n) {
    /* pass */
    _tr_c_memcpy(dst, src, n);
}

__attribute__((hot)) void raw_zero(char* ptr, long long n) {
    /* pass */
    _tr_c_memset(ptr, 0LL, n);
}

__attribute__((hot)) void raw_move(char* dst, char* src, long long n) {
    /* pass */
    _tr_c_memmove(dst, src, n);
}

