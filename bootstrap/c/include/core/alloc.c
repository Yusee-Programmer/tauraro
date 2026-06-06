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

__attribute__((hot)) void* alloc(long long n_elems) {
    /* pass */
    /* unsafe block */
    /* pass */
    __auto_type sz = ((long long)sizeof(void));
    /* pass */
    void* ptr = ((void*)(_tr_c_calloc(n_elems, sz)));
    /* pass */
    return ptr;
}

__attribute__((hot)) void dealloc(void* ptr) {
    /* pass */
    _tr_c_free(((char*)(ptr)));
}

__attribute__((hot)) void* resize(void* ptr, long long new_count) {
    /* pass */
    /* unsafe block */
    /* pass */
    __auto_type sz = ((long long)sizeof(void));
    /* pass */
    return ((void*)(_tr_c_realloc(((char*)(ptr)), (new_count * sz))));
}

__attribute__((hot)) void copy(void* dst, void* src, long long n_elems) {
    /* pass */
    /* unsafe block */
    /* pass */
    __auto_type sz = ((long long)sizeof(void));
    /* pass */
    _tr_c_memcpy(((char*)(dst)), ((char*)(src)), (n_elems * sz));
}

