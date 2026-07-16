#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) LlvmGenerator* LlvmGenerator_init() {
    /* pass */
    LlvmGenerator* g = ((LlvmGenerator*)_tr_obj_alloc(sizeof(LlvmGenerator)));
    /* pass */
    g->ok = true;
    /* pass */
    g->fail_note = _tr_str_lit("");
    /* pass */
    return g;
}

__attribute__((hot)) TrStr LlvmGenerator_generate(LlvmGenerator* self, HirProgram* prog) {
    /* pass */
    LModule* m = lower_to_lir(prog);
    /* pass */
    if ((!m->ok)) {
        /* pass */
        self->ok = false;
        /* pass */
        self->fail_note = _tr_str_retain(m->fail_note);
        /* pass */
        _tr_obj_release(m, _trdrop_LModule);
        return _tr_str_lit("");
    }
    /* pass */
    self->ok = true;
    /* pass */
    LlvmEmitter* e = LlvmEmitter_init(m);
    /* pass */
    _tr_obj_release(m, _trdrop_LModule);
    return LlvmEmitter_emit_module(e);
}

