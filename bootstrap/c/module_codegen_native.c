#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) NativeGenerator* NativeGenerator_init() {
#line 32 "src/codegen/native/mod.tr"
    NativeGenerator* g = ((NativeGenerator*)_tr_obj_alloc(sizeof(NativeGenerator)));
#line 33 "src/codegen/native/mod.tr"
    g->target = _tr_str_lit_len("x86_64-linux-elf", 16LL);
#line 34 "src/codegen/native/mod.tr"
    g->ready = true;
#line 35 "src/codegen/native/mod.tr"
    g->fail_note = _tr_str_lit_len("", 0LL);
#line 36 "src/codegen/native/mod.tr"
    return g;
}

__attribute__((hot)) bool NativeGenerator_emit_object(NativeGenerator* self, HirProgram* prog, TrStr out_path) {
#line 41 "src/codegen/native/mod.tr"
    LModule* m = lower_to_lir(prog);
#line 42 "src/codegen/native/mod.tr"
    if ((!m->ok)) {
#line 43 "src/codegen/native/mod.tr"
        self->fail_note = _tr_str_retain(m->fail_note);
#line 44 "src/codegen/native/mod.tr"
        _tr_obj_release(m, _trdrop_LModule);
        return false;
    }
#line 45 "src/codegen/native/mod.tr"
    if ((!emit_lir_object(m, out_path))) {
#line 46 "src/codegen/native/mod.tr"
        self->fail_note = _tr_str_lit_len("object emission failed (encode/ELF write)", 41LL);
#line 47 "src/codegen/native/mod.tr"
        _tr_obj_release(m, _trdrop_LModule);
        return false;
    }
#line 48 "src/codegen/native/mod.tr"
    _tr_obj_release(m, _trdrop_LModule);
    return true;
}

