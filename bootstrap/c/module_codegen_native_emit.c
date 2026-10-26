#include "tauraro_types.h"


__attribute__((hot)) bool emit_lir_object(LModule* m, TrStr out_path) {
#line 13 "src/codegen/native/emit.tr"
    if ((!m->ok)) {
#line 13 "src/codegen/native/emit.tr"
        return false;
    }
#line 14 "src/codegen/native/emit.tr"
    if ((m->funcs->len == 0LL)) {
#line 14 "src/codegen/native/emit.tr"
        return false;
    }
#line 15 "src/codegen/native/emit.tr"
    List_ptr* encoded = (void*)List_ptr_new();
#line 16 "src/codegen/native/emit.tr"
    long long i = 0LL;
#line 17 "src/codegen/native/emit.tr"
    while ((i < m->funcs->len)) {
#line 18 "src/codegen/native/emit.tr"
        List_ptr_append(encoded, encode_func(((LFunc*)List_ptr_get(m->funcs, i))));
#line 19 "src/codegen/native/emit.tr"
        i = (i + 1LL);
    }
#line 20 "src/codegen/native/emit.tr"
    return write_elf_object(out_path, encoded, m->externs, m->strings, m->string_lens, m->globals->len);
}

