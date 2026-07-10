#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) NativeGenerator* NativeGenerator_init() {
    /* pass */
    NativeGenerator* g = ((NativeGenerator*)_tr_obj_alloc(sizeof(NativeGenerator)));
    /* pass */
    g->target = _tr_str_lit("x86_64-linux-elf");
    /* pass */
    g->ready = false;
    /* pass */
    return g;
}

__attribute__((hot)) bool NativeGenerator_emit_object(NativeGenerator* self, HirProgram* prog, TrStr out_path) {
    /* pass */
    if ((!self->ready)) {
        /* pass */
        return false;
    }
    /* pass */
    return false;
}

