#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) NativeGenerator* NativeGenerator_init() {
    /* pass */
    NativeGenerator* g = ((NativeGenerator*)_tr_obj_alloc(sizeof(NativeGenerator)));
    /* pass */
    g->target = _tr_str_lit("x86_64-linux-elf");
    /* pass */
    g->ready = true;
    /* pass */
    g->fail_note = _tr_str_lit("");
    /* pass */
    return g;
}

__attribute__((hot)) bool NativeGenerator_emit_object(NativeGenerator* self, HirProgram* prog, TrStr out_path) {
    /* pass */
    LModule* m = lower_to_lir(prog);
    /* pass */
    if ((!m->ok)) {
        /* pass */
        self->fail_note = m->fail_note;
        /* pass */
        _tr_obj_release(m, _trdrop_LModule);
        return false;
    }
    /* pass */
    if ((!emit_lir_object(m, out_path))) {
        /* pass */
        self->fail_note = _tr_str_lit("object emission failed (encode/ELF write)");
        /* pass */
        _tr_obj_release(m, _trdrop_LModule);
        return false;
    }
    /* pass */
    _tr_obj_release(m, _trdrop_LModule);
    return true;
}

