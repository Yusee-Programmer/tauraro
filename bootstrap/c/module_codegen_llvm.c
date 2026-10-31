#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) LlvmGenerator* LlvmGenerator_init() {
#line 27 "src/codegen/llvm/mod.tr"
    LlvmGenerator* g = ((LlvmGenerator*)_tr_obj_alloc(sizeof(LlvmGenerator)));
#line 28 "src/codegen/llvm/mod.tr"
    g->ok = true;
#line 29 "src/codegen/llvm/mod.tr"
    g->fail_note = _tr_str_lit_len("", 0LL);
#line 30 "src/codegen/llvm/mod.tr"
    return g;
}

__attribute__((hot)) TrStr LlvmGenerator_generate(LlvmGenerator* self, HirProgram* prog) {
#line 36 "src/codegen/llvm/mod.tr"
    return LlvmGenerator_generate_nh(self, prog, false);
}

__attribute__((hot)) TrStr LlvmGenerator_generate_nh(LlvmGenerator* self, HirProgram* prog, bool no_heap) {
#line 41 "src/codegen/llvm/mod.tr"
    LModule* m = lower_to_lir_nh(prog, true, no_heap);
#line 42 "src/codegen/llvm/mod.tr"
    if ((!m->ok)) {
#line 43 "src/codegen/llvm/mod.tr"
        self->ok = false;
#line 44 "src/codegen/llvm/mod.tr"
        self->fail_note = _tr_str_retain(m->fail_note);
#line 45 "src/codegen/llvm/mod.tr"
        _tr_obj_release(m, _trdrop_LModule);
        return _tr_str_lit_len("", 0LL);
    }
#line 46 "src/codegen/llvm/mod.tr"
    self->ok = true;
#line 47 "src/codegen/llvm/mod.tr"
    LlvmEmitter* e = LlvmEmitter_init(m);
#line 48 "src/codegen/llvm/mod.tr"
    TrStr _trret1899 = LlvmEmitter_emit_module(e);
    _tr_obj_release(m, _trdrop_LModule);
    _tr_obj_release(e, _trdrop_LlvmEmitter);
    return _trret1899;
}

