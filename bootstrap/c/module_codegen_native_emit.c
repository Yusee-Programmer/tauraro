#include "tauraro_types.h"


__attribute__((hot)) bool emit_lir_object(LModule* m, TrStr out_path) {
    /* pass */
    if ((!m->ok)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((m->funcs->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    List_ptr* encoded = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < m->funcs->len)) {
        /* pass */
        List_ptr_append(encoded, encode_func(((LFunc*)List_ptr_get(m->funcs, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return write_elf_object(out_path, encoded, m->externs, m->strings);
}

