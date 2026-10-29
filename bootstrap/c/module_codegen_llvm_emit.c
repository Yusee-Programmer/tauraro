#include "tauraro_types.h"

TrStr _tbaa_suffix(long long aclass);
bool _is_fresh_alloc_ret(TrStr name);
TrStr _ll_int_instr(TrStr op);
TrStr _ll_icmp_pred(TrStr op);
TrStr _ll_float_instr(TrStr op);
TrStr _ll_fcmp_pred(TrStr op);

__attribute__((malloc,returns_nonnull,hot)) LlvmEmitter* LlvmEmitter_init(LModule* m) {
#line 27 "src/codegen/llvm/emit.tr"
    LlvmEmitter* e = ((LlvmEmitter*)_tr_obj_alloc(sizeof(LlvmEmitter)));
#line 28 "src/codegen/llvm/emit.tr"
    e->out = StringBuilder_init(4096LL);
#line 29 "src/codegen/llvm/emit.tr"
    LModule* _cltmp_t6997 = _tr_obj_retain(m);
    _tr_obj_release(e->m, _trdrop_LModule);
    e->m = _cltmp_t6997;
#line 30 "src/codegen/llvm/emit.tr"
    e->tmp = 0LL;
#line 31 "src/codegen/llvm/emit.tr"
    e->cur_ret = _tr_str_lit_len("i64", 3LL);
#line 32 "src/codegen/llvm/emit.tr"
    e->cur_main = false;
#line 33 "src/codegen/llvm/emit.tr"
    return e;
}

__attribute__((hot)) void LlvmEmitter_w(LlvmEmitter* self, TrStr s) {
#line 36 "src/codegen/llvm/emit.tr"
    StringBuilder_append(self->out, s);
}

__attribute__((hot)) TrStr LlvmEmitter_newtmp(LlvmEmitter* self) {
#line 39 "src/codegen/llvm/emit.tr"
    self->tmp = (self->tmp + 1LL);
#line 40 "src/codegen/llvm/emit.tr"
    return ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->tmp)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("%t", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
}

__attribute__((hot)) TrStr LlvmEmitter_vty(LlvmEmitter* self, long long v) {
#line 44 "src/codegen/llvm/emit.tr"
    return _ll_ty(LFunc_vreg_type(self->cur, v));
}

__attribute__((hot)) TrStr LlvmEmitter_load_vreg(LlvmEmitter* self, long long v) {
#line 48 "src/codegen/llvm/emit.tr"
    TrStr t = LlvmEmitter_newtmp(self);
#line 49 "src/codegen/llvm/emit.tr"
    ({ __auto_type _ord_t6998 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (t))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (LlvmEmitter_vty(self, v)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t6999 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t6998); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t6999); _tr_str_release(_at_t6999); });
#line 50 "src/codegen/llvm/emit.tr"
    TrStr _trret1881 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret1881;
}

__attribute__((hot)) TrStr LlvmEmitter_load_vreg_as(LlvmEmitter* self, long long v, TrStr ty) {
#line 54 "src/codegen/llvm/emit.tr"
    TrStr t = LlvmEmitter_newtmp(self);
#line 55 "src/codegen/llvm/emit.tr"
    ({ TrStr _at_t7000 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (t))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7000); _tr_str_release(_at_t7000); });
#line 56 "src/codegen/llvm/emit.tr"
    TrStr _trret1882 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret1882;
}

__attribute__((hot)) void LlvmEmitter_store_vreg(LlvmEmitter* self, long long v, TrStr ty, TrStr val) {
#line 59 "src/codegen/llvm/emit.tr"
    ({ TrStr _at_t7001 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7001); _tr_str_release(_at_t7001); });
}

__attribute__((hot)) TrStr LlvmEmitter_addr_of_base(LlvmEmitter* self, long long v) {
#line 64 "src/codegen/llvm/emit.tr"
    if (({ TrStr _cmpa_t7002 = (LlvmEmitter_vty(self, v)); TrStr _cmpb_t7003 = (_tr_str_lit_len("ptr", 3LL)); _Bool _cmpr_t7004 = _tr_str_eqv(_cmpa_t7002, _cmpb_t7003); _tr_str_release(_cmpa_t7002); _cmpr_t7004; })) {
#line 65 "src/codegen/llvm/emit.tr"
        return LlvmEmitter_load_vreg_as(self, v, _tr_str_lit_len("ptr", 3LL));
    }
#line 66 "src/codegen/llvm/emit.tr"
    TrStr bi = LlvmEmitter_load_vreg_as(self, v, _tr_str_lit_len("i64", 3LL));
#line 67 "src/codegen/llvm/emit.tr"
    TrStr bp = LlvmEmitter_newtmp(self);
#line 68 "src/codegen/llvm/emit.tr"
    ({ TrStr _at_t7005 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = inttoptr i64 ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bi)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ptr\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7005); _tr_str_release(_at_t7005); });
#line 69 "src/codegen/llvm/emit.tr"
    TrStr _trret1883 = _tr_str_retain(bp);
    _tr_str_release(bi);
    _tr_str_release(bp);
    return _trret1883;
}

__attribute__((hot)) TrStr LlvmEmitter_user_ret_ty(LlvmEmitter* self, TrStr name) {
#line 74 "src/codegen/llvm/emit.tr"
    return _ll_ty(LModule_fn_ret_tag(self->m, name));
}

__attribute__((hot)) void LlvmEmitter_emit_inst(LlvmEmitter* self, LInst inst) {
#line 78 "src/codegen/llvm/emit.tr"
    __auto_type _t7006 = inst;
    if (_t7006.tag == LInst_IConst) {
        __auto_type dst = _t7006.data.IConst.dst;
__auto_type v = _t7006.data.IConst.v;
#line 80 "src/codegen/llvm/emit.tr"
        if (({ TrStr _cmpa_t7007 = (LlvmEmitter_vty(self, dst)); TrStr _cmpb_t7008 = (_tr_str_lit_len("double", 6LL)); _Bool _cmpr_t7009 = _tr_str_eqv(_cmpa_t7007, _cmpb_t7008); _tr_str_release(_cmpa_t7007); _cmpr_t7009; })) {
#line 81 "src/codegen/llvm/emit.tr"
            ({ __auto_type _ord_t7010 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_ll_hexpad16(v)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  store double 0x", 17LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7011 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7010); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(dst)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7011); _tr_str_release(_at_t7011); });
        } else {
#line 83 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7012 = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), _at_t7012); _tr_str_release(_at_t7012); });
        }
    } else if (_t7006.tag == LInst_IStr) {
        __auto_type dst = _t7006.data.IStr.dst;
__auto_type str_idx = _t7006.data.IStr.str_idx;
#line 85 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7013 = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(str_idx)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("@.str.", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t7013); _tr_str_release(_at_t7013); });
    } else if (_t7006.tag == LInst_IBinOp) {
        __auto_type dst = _t7006.data.IBinOp.dst;
__auto_type op = _t7006.data.IBinOp.op;
__auto_type a = _t7006.data.IBinOp.a;
__auto_type b = _t7006.data.IBinOp.b;
#line 94 "src/codegen/llvm/emit.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) && ({ TrStr _cmpa_t7014 = (LlvmEmitter_vty(self, dst)); TrStr _cmpb_t7015 = (_tr_str_lit_len("ptr", 3LL)); _Bool _cmpr_t7016 = _tr_str_eqv(_cmpa_t7014, _cmpb_t7015); _tr_str_release(_cmpa_t7014); _cmpr_t7016; }))) {
#line 95 "src/codegen/llvm/emit.tr"
            TrStr pbase = LlvmEmitter_addr_of_base(self, a);
#line 96 "src/codegen/llvm/emit.tr"
            TrStr poff = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("i64", 3LL));
#line 97 "src/codegen/llvm/emit.tr"
            TrStr pg = LlvmEmitter_newtmp(self);
#line 98 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7017 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (pg))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pbase)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (poff)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7017); _tr_str_release(_at_t7017); });
#line 99 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), pg);
            _tr_str_release(pbase);
            _tr_str_release(poff);
            _tr_str_release(pg);
        } else {
#line 101 "src/codegen/llvm/emit.tr"
            TrStr la = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit_len("i64", 3LL));
#line 102 "src/codegen/llvm/emit.tr"
            TrStr lb = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("i64", 3LL));
#line 103 "src/codegen/llvm/emit.tr"
            TrStr pred = _ll_icmp_pred(op);
#line 104 "src/codegen/llvm/emit.tr"
            if ((!_tr_str_eqv((pred), (_tr_str_lit_len("", 0LL))))) {
#line 105 "src/codegen/llvm/emit.tr"
                TrStr c = LlvmEmitter_newtmp(self);
#line 106 "src/codegen/llvm/emit.tr"
                ({ TrStr _at_t7018 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (c))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = icmp ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pred)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" i64 ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7018); _tr_str_release(_at_t7018); });
#line 107 "src/codegen/llvm/emit.tr"
                TrStr z = LlvmEmitter_newtmp(self);
#line 108 "src/codegen/llvm/emit.tr"
                ({ TrStr _at_t7019 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (z))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = zext i1 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7019); _tr_str_release(_at_t7019); });
#line 109 "src/codegen/llvm/emit.tr"
                LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), z);
                _tr_str_release(c);
                _tr_str_release(z);
            } else {
#line 111 "src/codegen/llvm/emit.tr"
                TrStr r = LlvmEmitter_newtmp(self);
#line 112 "src/codegen/llvm/emit.tr"
                ({ TrStr _at_t7020 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_ll_int_instr(op)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" i64 ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7020); _tr_str_release(_at_t7020); });
#line 113 "src/codegen/llvm/emit.tr"
                LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), r);
                _tr_str_release(r);
            }
        }
    } else if (_t7006.tag == LInst_IFBinOp) {
        __auto_type dst = _t7006.data.IFBinOp.dst;
__auto_type op = _t7006.data.IFBinOp.op;
__auto_type a = _t7006.data.IFBinOp.a;
__auto_type b = _t7006.data.IFBinOp.b;
#line 115 "src/codegen/llvm/emit.tr"
        TrStr fa = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit_len("double", 6LL));
#line 116 "src/codegen/llvm/emit.tr"
        TrStr fb = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("double", 6LL));
#line 117 "src/codegen/llvm/emit.tr"
        TrStr fpred = _ll_fcmp_pred(op);
#line 118 "src/codegen/llvm/emit.tr"
        if ((!_tr_str_eqv((fpred), (_tr_str_lit_len("", 0LL))))) {
#line 119 "src/codegen/llvm/emit.tr"
            TrStr c2 = LlvmEmitter_newtmp(self);
#line 120 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7021 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (c2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fcmp ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fpred)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" double ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fa)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7021); _tr_str_release(_at_t7021); });
#line 121 "src/codegen/llvm/emit.tr"
            TrStr z2 = LlvmEmitter_newtmp(self);
#line 122 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7022 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (z2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = zext i1 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (c2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7022); _tr_str_release(_at_t7022); });
#line 123 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), z2);
            _tr_str_release(c2);
            _tr_str_release(z2);
        } else {
#line 125 "src/codegen/llvm/emit.tr"
            TrStr fr = LlvmEmitter_newtmp(self);
#line 126 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7023 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (fr))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_ll_float_instr(op)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" double ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fa)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7023); _tr_str_release(_at_t7023); });
#line 127 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), fr);
            _tr_str_release(fr);
        }
        _tr_str_release(fa);
        _tr_str_release(fb);
        _tr_str_release(fpred);
    } else if (_t7006.tag == LInst_IIToF) {
        __auto_type dst = _t7006.data.IIToF.dst;
__auto_type src = _t7006.data.IIToF.src;
#line 129 "src/codegen/llvm/emit.tr"
        TrStr si = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("i64", 3LL));
#line 130 "src/codegen/llvm/emit.tr"
        TrStr rf = LlvmEmitter_newtmp(self);
#line 131 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7024 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (rf))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sitofp i64 ", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (si)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to double\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7024); _tr_str_release(_at_t7024); });
#line 132 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), rf);
        _tr_str_release(si);
        _tr_str_release(rf);
    } else if (_t7006.tag == LInst_IFToI) {
        __auto_type dst = _t7006.data.IFToI.dst;
__auto_type src = _t7006.data.IFToI.src;
#line 134 "src/codegen/llvm/emit.tr"
        TrStr sf = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("double", 6LL));
#line 135 "src/codegen/llvm/emit.tr"
        TrStr ri = LlvmEmitter_newtmp(self);
#line 136 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7025 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (ri))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fptosi double ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sf)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7025); _tr_str_release(_at_t7025); });
#line 137 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), ri);
        _tr_str_release(sf);
        _tr_str_release(ri);
    } else if (_t7006.tag == LInst_ILoadVar) {
        __auto_type dst = _t7006.data.ILoadVar.dst;
__auto_type name = _t7006.data.ILoadVar.name;
#line 139 "src/codegen/llvm/emit.tr"
        TrStr vt = _ll_ty(LFunc_var_type(self->cur, name));
#line 140 "src/codegen/llvm/emit.tr"
        TrStr t = LlvmEmitter_newtmp(self);
#line 141 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7026 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (t))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (vt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7026); _tr_str_release(_at_t7026); });
#line 142 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, vt, t);
        _tr_str_release(vt);
        _tr_str_release(t);
    } else if (_t7006.tag == LInst_IStoreVar) {
        __auto_type name = _t7006.data.IStoreVar.name;
__auto_type src = _t7006.data.IStoreVar.src;
#line 144 "src/codegen/llvm/emit.tr"
        TrStr st = LlvmEmitter_vty(self, src);
#line 145 "src/codegen/llvm/emit.tr"
        TrStr ls = LlvmEmitter_load_vreg_as(self, src, st);
#line 146 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7027 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (st))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ls)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7027); _tr_str_release(_at_t7027); });
        _tr_str_release(st);
        _tr_str_release(ls);
    } else if (_t7006.tag == LInst_ILoadGlobal) {
        __auto_type dst = _t7006.data.ILoadGlobal.dst;
__auto_type gidx = _t7006.data.ILoadGlobal.gidx;
#line 148 "src/codegen/llvm/emit.tr"
        TrStr gt = _ll_ty(List_i64_get(self->m->global_types, gidx));
#line 149 "src/codegen/llvm/emit.tr"
        TrStr tg = LlvmEmitter_newtmp(self);
#line 150 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7028 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tg))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr @g.", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7028); _tr_str_release(_at_t7028); });
#line 151 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, gt, tg);
        _tr_str_release(gt);
        _tr_str_release(tg);
    } else if (_t7006.tag == LInst_IStoreGlobal) {
        __auto_type gidx = _t7006.data.IStoreGlobal.gidx;
__auto_type src = _t7006.data.IStoreGlobal.src;
#line 153 "src/codegen/llvm/emit.tr"
        TrStr gt2 = LlvmEmitter_vty(self, src);
#line 154 "src/codegen/llvm/emit.tr"
        TrStr lg = LlvmEmitter_load_vreg_as(self, src, gt2);
#line 155 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7029 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (gt2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lg)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr @g.", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7029); _tr_str_release(_at_t7029); });
        _tr_str_release(gt2);
        _tr_str_release(lg);
    } else if (_t7006.tag == LInst_ICall) {
        __auto_type dst = _t7006.data.ICall.dst;
__auto_type callee = _t7006.data.ICall.callee;
__auto_type args = _t7006.data.ICall.args;
#line 157 "src/codegen/llvm/emit.tr"
        LlvmEmitter_emit_call(self, dst, callee, args);
    } else if (_t7006.tag == LInst_IFCall1) {
        __auto_type dst = _t7006.data.IFCall1.dst;
__auto_type callee = _t7006.data.IFCall1.callee;
__auto_type arg = _t7006.data.IFCall1.arg;
#line 159 "src/codegen/llvm/emit.tr"
        TrStr a1 = LlvmEmitter_load_vreg_as(self, arg, _tr_str_lit_len("double", 6LL));
#line 160 "src/codegen/llvm/emit.tr"
        TrStr rt1 = _tr_str_lit_len("void", 4LL);
#line 161 "src/codegen/llvm/emit.tr"
        if ((dst >= 0LL)) {
#line 161 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7030 = LlvmEmitter_vty(self, dst);
            _tr_str_release(rt1);
            rt1 = _strtmp_t7030;
        }
#line 162 "src/codegen/llvm/emit.tr"
        if (_tr_str_eqv((rt1), (_tr_str_lit_len("void", 4LL)))) {
#line 163 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7031 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  call void @", 13LL)), (callee))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (a1)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7031); _tr_str_release(_at_t7031); });
        } else {
#line 165 "src/codegen/llvm/emit.tr"
            TrStr r1 = LlvmEmitter_newtmp(self);
#line 166 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7032 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r1))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rt1)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (callee)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (a1)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7032); _tr_str_release(_at_t7032); });
#line 167 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, rt1, r1);
            _tr_str_release(r1);
        }
        _tr_str_release(a1);
        _tr_str_release(rt1);
    } else if (_t7006.tag == LInst_IFCallF) {
        __auto_type dst = _t7006.data.IFCallF.dst;
__auto_type callee = _t7006.data.IFCallF.callee;
__auto_type arg = _t7006.data.IFCallF.arg;
#line 169 "src/codegen/llvm/emit.tr"
        TrStr a2 = LlvmEmitter_load_vreg_as(self, arg, _tr_str_lit_len("double", 6LL));
#line 170 "src/codegen/llvm/emit.tr"
        TrStr r2 = LlvmEmitter_newtmp(self);
#line 171 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7033 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call double @", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (callee)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (a2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7033); _tr_str_release(_at_t7033); });
#line 172 "src/codegen/llvm/emit.tr"
        if ((dst >= 0LL)) {
#line 172 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), r2);
        }
        _tr_str_release(a2);
        _tr_str_release(r2);
    } else if (_t7006.tag == LInst_IFCall2F) {
        __auto_type dst = _t7006.data.IFCall2F.dst;
__auto_type callee = _t7006.data.IFCall2F.callee;
__auto_type a = _t7006.data.IFCall2F.a;
__auto_type b = _t7006.data.IFCall2F.b;
#line 174 "src/codegen/llvm/emit.tr"
        TrStr fa2 = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit_len("double", 6LL));
#line 175 "src/codegen/llvm/emit.tr"
        TrStr fb2 = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("double", 6LL));
#line 176 "src/codegen/llvm/emit.tr"
        TrStr r3 = LlvmEmitter_newtmp(self);
#line 177 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7034 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r3))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call double @", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (callee)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fa2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", double ", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fb2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7034); _tr_str_release(_at_t7034); });
#line 178 "src/codegen/llvm/emit.tr"
        if ((dst >= 0LL)) {
#line 178 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), r3);
        }
        _tr_str_release(fa2);
        _tr_str_release(fb2);
        _tr_str_release(r3);
    } else if (_t7006.tag == LInst_IBitsF) {
        __auto_type dst = _t7006.data.IBitsF.dst;
__auto_type src = _t7006.data.IBitsF.src;
#line 180 "src/codegen/llvm/emit.tr"
        TrStr bi = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("i64", 3LL));
#line 181 "src/codegen/llvm/emit.tr"
        TrStr bf = LlvmEmitter_newtmp(self);
#line 182 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7035 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bf))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = bitcast i64 ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bi)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to double\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7035); _tr_str_release(_at_t7035); });
#line 183 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), bf);
        _tr_str_release(bi);
        _tr_str_release(bf);
    } else if (_t7006.tag == LInst_IFBits) {
        __auto_type dst = _t7006.data.IFBits.dst;
__auto_type src = _t7006.data.IFBits.src;
#line 185 "src/codegen/llvm/emit.tr"
        TrStr bd = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("double", 6LL));
#line 186 "src/codegen/llvm/emit.tr"
        TrStr bx = LlvmEmitter_newtmp(self);
#line 187 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7036 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = bitcast double ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bd)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7036); _tr_str_release(_at_t7036); });
#line 188 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), bx);
        _tr_str_release(bd);
        _tr_str_release(bx);
    } else if (_t7006.tag == LInst_IAddrVar) {
        __auto_type dst = _t7006.data.IAddrVar.dst;
__auto_type name = _t7006.data.IAddrVar.name;
#line 191 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7037 = (_tr_strx_concatv((_tr_str_lit_len("%var_", 5LL)), (name))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t7037); _tr_str_release(_at_t7037); });
    } else if (_t7006.tag == LInst_IAddrGlobal) {
        __auto_type dst = _t7006.data.IAddrGlobal.dst;
__auto_type gidx = _t7006.data.IAddrGlobal.gidx;
#line 194 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7038 = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("@g.", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t7038); _tr_str_release(_at_t7038); });
    } else if (_t7006.tag == LInst_IFuncAddr) {
        __auto_type dst = _t7006.data.IFuncAddr.dst;
__auto_type fname = _t7006.data.IFuncAddr.fname;
#line 196 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7039 = (_tr_strx_concatv((_tr_str_lit_len("@", 1LL)), (fname))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t7039); _tr_str_release(_at_t7039); });
    } else if (_t7006.tag == LInst_ICallInd) {
        __auto_type dst = _t7006.data.ICallInd.dst;
__auto_type fnreg = _t7006.data.ICallInd.fnreg;
__auto_type iargs = _t7006.data.ICallInd.args;
#line 198 "src/codegen/llvm/emit.tr"
        TrStr ind_args = _tr_str_lit_len("", 0LL);
#line 199 "src/codegen/llvm/emit.tr"
        long long ii2 = 0LL;
#line 200 "src/codegen/llvm/emit.tr"
        while ((ii2 < iargs->len)) {
#line 201 "src/codegen/llvm/emit.tr"
            long long iav = List_i64_get(iargs, ii2);
#line 202 "src/codegen/llvm/emit.tr"
            TrStr iaty = LlvmEmitter_vty(self, iav);
#line 203 "src/codegen/llvm/emit.tr"
            TrStr ial = LlvmEmitter_load_vreg_as(self, iav, iaty);
#line 204 "src/codegen/llvm/emit.tr"
            if ((ii2 > 0LL)) {
#line 204 "src/codegen/llvm/emit.tr"
                ({ TrStr _ap_t7040 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&ind_args, _ap_t7040); });
            }
#line 205 "src/codegen/llvm/emit.tr"
            ({ TrStr _ap_t7041 = (iaty); TrStr _ap_t7042 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t7043 = (ial); _tr_str_append(&ind_args, _ap_t7041); _tr_str_append(&ind_args, _ap_t7042); _tr_str_append(&ind_args, _ap_t7043); });
#line 206 "src/codegen/llvm/emit.tr"
            ii2 = (ii2 + 1LL);
            _tr_str_release(iaty);
            _tr_str_release(ial);
        }
#line 207 "src/codegen/llvm/emit.tr"
        TrStr fp = LlvmEmitter_load_vreg_as(self, fnreg, _tr_str_lit_len("ptr", 3LL));
#line 208 "src/codegen/llvm/emit.tr"
        TrStr ind_ret = _tr_str_lit_len("void", 4LL);
#line 209 "src/codegen/llvm/emit.tr"
        if ((dst >= 0LL)) {
#line 209 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7044 = LlvmEmitter_vty(self, dst);
            _tr_str_release(ind_ret);
            ind_ret = _strtmp_t7044;
        }
#line 210 "src/codegen/llvm/emit.tr"
        if (_tr_str_eqv((ind_ret), (_tr_str_lit_len("void", 4LL)))) {
#line 211 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7045 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  call void ", 12LL)), (fp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ind_args)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7045); _tr_str_release(_at_t7045); });
        } else {
#line 213 "src/codegen/llvm/emit.tr"
            TrStr ir = LlvmEmitter_newtmp(self);
#line 214 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7046 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (ir))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ind_ret)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ind_args)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7046); _tr_str_release(_at_t7046); });
#line 215 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, ind_ret, ir);
            _tr_str_release(ir);
        }
        _tr_str_release(ind_args);
        _tr_str_release(fp);
        _tr_str_release(ind_ret);
    } else if (_t7006.tag == LInst_IAsm) {
        __auto_type code = _t7006.data.IAsm.code;
__auto_type cons = _t7006.data.IAsm.cons;
#line 218 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7047 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_ll_str_escape(code, _tr_str_lenv((code)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  call void asm sideeffect \"", 28LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\", \"", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cons)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\"()\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7047); _tr_str_release(_at_t7047); });
    } else if (_t7006.tag == LInst_ILoad) {
        __auto_type dst = _t7006.data.ILoad.dst;
__auto_type base = _t7006.data.ILoad.base;
__auto_type off = _t7006.data.ILoad.off;
__auto_type aclass = _t7006.data.ILoad.aclass;
#line 222 "src/codegen/llvm/emit.tr"
        TrStr lbp = LlvmEmitter_addr_of_base(self, base);
#line 223 "src/codegen/llvm/emit.tr"
        TrStr lep = _tr_str_retain(lbp);
#line 224 "src/codegen/llvm/emit.tr"
        if ((off != 0LL)) {
#line 225 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7048 = LlvmEmitter_newtmp(self);
            _tr_str_release(lep);
            lep = _strtmp_t7048;
#line 226 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7049 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (lep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lbp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7049); _tr_str_release(_at_t7049); });
        }
#line 227 "src/codegen/llvm/emit.tr"
        TrStr ldty = LlvmEmitter_vty(self, dst);
#line 228 "src/codegen/llvm/emit.tr"
        TrStr lv = LlvmEmitter_newtmp(self);
#line 229 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7050 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (lv))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ldty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tbaa_suffix(aclass)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7050); _tr_str_release(_at_t7050); });
#line 230 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, ldty, lv);
        _tr_str_release(lbp);
        _tr_str_release(lep);
        _tr_str_release(ldty);
        _tr_str_release(lv);
    } else if (_t7006.tag == LInst_IStore) {
        __auto_type base = _t7006.data.IStore.base;
__auto_type off = _t7006.data.IStore.off;
__auto_type src = _t7006.data.IStore.src;
__auto_type aclass = _t7006.data.IStore.aclass;
#line 232 "src/codegen/llvm/emit.tr"
        TrStr sbp = LlvmEmitter_addr_of_base(self, base);
#line 233 "src/codegen/llvm/emit.tr"
        TrStr sep = _tr_str_retain(sbp);
#line 234 "src/codegen/llvm/emit.tr"
        if ((off != 0LL)) {
#line 235 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7051 = LlvmEmitter_newtmp(self);
            _tr_str_release(sep);
            sep = _strtmp_t7051;
#line 236 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7052 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (sep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sbp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7052); _tr_str_release(_at_t7052); });
        }
#line 237 "src/codegen/llvm/emit.tr"
        TrStr ssty = LlvmEmitter_vty(self, src);
#line 238 "src/codegen/llvm/emit.tr"
        TrStr ssv = LlvmEmitter_load_vreg(self, src);
#line 239 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7053 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (ssty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ssv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (sep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tbaa_suffix(aclass)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7053); _tr_str_release(_at_t7053); });
        _tr_str_release(sbp);
        _tr_str_release(sep);
        _tr_str_release(ssty);
        _tr_str_release(ssv);
    } else if (_t7006.tag == LInst_ILoadB) {
        __auto_type dst = _t7006.data.ILoadB.dst;
__auto_type base = _t7006.data.ILoadB.base;
__auto_type off = _t7006.data.ILoadB.off;
#line 244 "src/codegen/llvm/emit.tr"
        TrStr bbp = LlvmEmitter_addr_of_base(self, base);
#line 245 "src/codegen/llvm/emit.tr"
        TrStr bep = _tr_str_retain(bbp);
#line 246 "src/codegen/llvm/emit.tr"
        if ((off != 0LL)) {
#line 247 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7054 = LlvmEmitter_newtmp(self);
            _tr_str_release(bep);
            bep = _strtmp_t7054;
#line 248 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7055 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bbp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7055); _tr_str_release(_at_t7055); });
        }
#line 249 "src/codegen/llvm/emit.tr"
        TrStr bb = LlvmEmitter_newtmp(self);
#line 250 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7056 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bb))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load i8, ptr ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", !tbaa !5\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7056); _tr_str_release(_at_t7056); });
#line 251 "src/codegen/llvm/emit.tr"
        TrStr bz = LlvmEmitter_newtmp(self);
#line 252 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7057 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bz))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = zext i8 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7057); _tr_str_release(_at_t7057); });
#line 253 "src/codegen/llvm/emit.tr"
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), bz);
        _tr_str_release(bbp);
        _tr_str_release(bep);
        _tr_str_release(bb);
        _tr_str_release(bz);
    } else if (_t7006.tag == LInst_IStoreB) {
        __auto_type base = _t7006.data.IStoreB.base;
__auto_type off = _t7006.data.IStoreB.off;
__auto_type src = _t7006.data.IStoreB.src;
#line 255 "src/codegen/llvm/emit.tr"
        TrStr tbp = LlvmEmitter_addr_of_base(self, base);
#line 256 "src/codegen/llvm/emit.tr"
        TrStr tep = _tr_str_retain(tbp);
#line 257 "src/codegen/llvm/emit.tr"
        if ((off != 0LL)) {
#line 258 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7058 = LlvmEmitter_newtmp(self);
            _tr_str_release(tep);
            tep = _strtmp_t7058;
#line 259 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7059 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tbp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7059); _tr_str_release(_at_t7059); });
        }
#line 260 "src/codegen/llvm/emit.tr"
        TrStr tsv = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("i64", 3LL));
#line 261 "src/codegen/llvm/emit.tr"
        TrStr tt = LlvmEmitter_newtmp(self);
#line 262 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7060 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tt))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = trunc i64 ", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tsv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i8\n", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7060); _tr_str_release(_at_t7060); });
#line 263 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7061 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store i8 ", 11LL)), (tt))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", !tbaa !5\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7061); _tr_str_release(_at_t7061); });
        _tr_str_release(tbp);
        _tr_str_release(tep);
        _tr_str_release(tsv);
        _tr_str_release(tt);
    }
}

__attribute__((hot)) void LlvmEmitter_emit_call(LlvmEmitter* self, long long dst, TrStr callee, List_i64* args) {
#line 267 "src/codegen/llvm/emit.tr"
    TrStr arglist = _tr_str_lit_len("", 0LL);
#line 268 "src/codegen/llvm/emit.tr"
    long long ai = 0LL;
#line 269 "src/codegen/llvm/emit.tr"
    while ((ai < args->len)) {
#line 270 "src/codegen/llvm/emit.tr"
        long long av = List_i64_get(args, ai);
#line 271 "src/codegen/llvm/emit.tr"
        TrStr aty = LlvmEmitter_vty(self, av);
#line 272 "src/codegen/llvm/emit.tr"
        TrStr al = LlvmEmitter_load_vreg_as(self, av, aty);
#line 273 "src/codegen/llvm/emit.tr"
        if ((ai > 0LL)) {
#line 273 "src/codegen/llvm/emit.tr"
            ({ TrStr _ap_t7062 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&arglist, _ap_t7062); });
        }
#line 274 "src/codegen/llvm/emit.tr"
        ({ TrStr _ap_t7063 = (aty); TrStr _ap_t7064 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t7065 = (al); _tr_str_append(&arglist, _ap_t7063); _tr_str_append(&arglist, _ap_t7064); _tr_str_append(&arglist, _ap_t7065); });
#line 275 "src/codegen/llvm/emit.tr"
        ai = (ai + 1LL);
        _tr_str_release(aty);
        _tr_str_release(al);
    }
#line 276 "src/codegen/llvm/emit.tr"
    TrStr retty = _tr_str_lit_len("void", 4LL);
#line 277 "src/codegen/llvm/emit.tr"
    if (LModule_is_user_fn(self->m, callee)) {
#line 278 "src/codegen/llvm/emit.tr"
        TrStr _strtmp_t7066 = LlvmEmitter_user_ret_ty(self, callee);
        _tr_str_release(retty);
        retty = _strtmp_t7066;
    } else if ((dst >= 0LL)) {
#line 280 "src/codegen/llvm/emit.tr"
        TrStr _strtmp_t7067 = LlvmEmitter_vty(self, dst);
        _tr_str_release(retty);
        retty = _strtmp_t7067;
    }
#line 281 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((retty), (_tr_str_lit_len("void", 4LL)))) {
#line 282 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7068 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  call void @", 13LL)), (callee))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arglist)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7068); _tr_str_release(_at_t7068); });
    } else {
#line 284 "src/codegen/llvm/emit.tr"
        TrStr r = LlvmEmitter_newtmp(self);
#line 285 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7069 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (retty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (callee)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (arglist)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7069); _tr_str_release(_at_t7069); });
#line 286 "src/codegen/llvm/emit.tr"
        if ((dst >= 0LL)) {
#line 286 "src/codegen/llvm/emit.tr"
            LlvmEmitter_store_vreg(self, dst, retty, r);
        }
    }
    _tr_str_release(arglist);
    _tr_str_release(retty);
}

__attribute__((hot)) void LlvmEmitter_emit_term(LlvmEmitter* self, LTerm t) {
#line 290 "src/codegen/llvm/emit.tr"
    __auto_type _t7070 = t;
    if (_t7070.tag == LTerm_TRetInt) {
        __auto_type v = _t7070.data.TRetInt.v;
#line 292 "src/codegen/llvm/emit.tr"
        if (self->cur_main) {
#line 293 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7071 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ret i32 ", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7071); _tr_str_release(_at_t7071); });
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("double", 6LL)))) {
#line 295 "src/codegen/llvm/emit.tr"
            LlvmEmitter_w(self, _tr_str_lit_len("  ret double 0.0\n", 17LL));
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("ptr", 3LL)))) {
#line 297 "src/codegen/llvm/emit.tr"
            LlvmEmitter_w(self, _tr_str_lit_len("  ret ptr null\n", 15LL));
        } else {
#line 299 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7072 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  ret i64 ", 10LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7072); _tr_str_release(_at_t7072); });
        }
    } else if (_t7070.tag == LTerm_TRetVal) {
        __auto_type v = _t7070.data.TRetVal.v;
#line 301 "src/codegen/llvm/emit.tr"
        if (self->cur_main) {
#line 302 "src/codegen/llvm/emit.tr"
            TrStr lv = LlvmEmitter_load_vreg_as(self, v, _tr_str_lit_len("i64", 3LL));
#line 303 "src/codegen/llvm/emit.tr"
            TrStr tr = LlvmEmitter_newtmp(self);
#line 304 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7073 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tr))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = trunc i64 ", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i32\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7073); _tr_str_release(_at_t7073); });
#line 305 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7074 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret i32 ", 10LL)), (tr))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7074); _tr_str_release(_at_t7074); });
            _tr_str_release(lv);
            _tr_str_release(tr);
        } else {
#line 307 "src/codegen/llvm/emit.tr"
            TrStr lv2 = LlvmEmitter_load_vreg_as(self, v, self->cur_ret);
#line 308 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7075 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->cur_ret))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lv2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7075); _tr_str_release(_at_t7075); });
            _tr_str_release(lv2);
        }
    } else if (_t7070.tag == LTerm_TRetVoid) {
#line 310 "src/codegen/llvm/emit.tr"
        if (self->cur_main) {
#line 310 "src/codegen/llvm/emit.tr"
            LlvmEmitter_w(self, _tr_str_lit_len("  ret i32 0\n", 12LL));
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("double", 6LL)))) {
#line 311 "src/codegen/llvm/emit.tr"
            LlvmEmitter_w(self, _tr_str_lit_len("  ret double 0.0\n", 17LL));
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("ptr", 3LL)))) {
#line 312 "src/codegen/llvm/emit.tr"
            LlvmEmitter_w(self, _tr_str_lit_len("  ret ptr null\n", 15LL));
        } else {
#line 313 "src/codegen/llvm/emit.tr"
            LlvmEmitter_w(self, _tr_str_lit_len("  ret i64 0\n", 12LL));
        }
    } else if (_t7070.tag == LTerm_TBr) {
        __auto_type target = _t7070.data.TBr.target;
#line 315 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7076 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(target)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  br label %bb", 14LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7076); _tr_str_release(_at_t7076); });
    } else if (_t7070.tag == LTerm_TCondBr) {
        __auto_type cond = _t7070.data.TCondBr.cond;
__auto_type then_b = _t7070.data.TCondBr.then_b;
__auto_type else_b = _t7070.data.TCondBr.else_b;
#line 317 "src/codegen/llvm/emit.tr"
        TrStr lc = LlvmEmitter_load_vreg_as(self, cond, _tr_str_lit_len("i64", 3LL));
#line 318 "src/codegen/llvm/emit.tr"
        TrStr c = LlvmEmitter_newtmp(self);
#line 319 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7077 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (c))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = icmp ne i64 ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", 0\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7077); _tr_str_release(_at_t7077); });
#line 320 "src/codegen/llvm/emit.tr"
        ({ __auto_type _ord_t7078 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (c))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %bb", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(then_b)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %bb", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7079 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7078); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(else_b)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7079); _tr_str_release(_at_t7079); });
        _tr_str_release(lc);
        _tr_str_release(c);
    } else if (_t7070.tag == LTerm_TUnset) {
#line 322 "src/codegen/llvm/emit.tr"
        LlvmEmitter_w(self, _tr_str_lit_len("  unreachable\n", 14LL));
    }
}

__attribute__((hot)) void LlvmEmitter_emit_function(LlvmEmitter* self, LFunc* lf) {
#line 326 "src/codegen/llvm/emit.tr"
    self->tmp = 0LL;
#line 327 "src/codegen/llvm/emit.tr"
    LFunc* _cltmp_t7080 = _tr_obj_retain(lf);
    _tr_obj_release(self->cur, _trdrop_LFunc);
    self->cur = _cltmp_t7080;
#line 328 "src/codegen/llvm/emit.tr"
    self->cur_main = lf->is_main;
#line 329 "src/codegen/llvm/emit.tr"
    if (lf->is_main) {
#line 330 "src/codegen/llvm/emit.tr"
        self->cur_ret = _tr_str_lit_len("i32", 3LL);
    } else {
#line 332 "src/codegen/llvm/emit.tr"
        self->cur_ret = _ll_ty(LModule_fn_ret_tag(self->m, lf->name));
    }
#line 333 "src/codegen/llvm/emit.tr"
    TrStr fname = _tr_str_retain(lf->name);
#line 336 "src/codegen/llvm/emit.tr"
    bool main_args = (lf->is_main && (lf->params->len > 0LL));
#line 338 "src/codegen/llvm/emit.tr"
    ({ TrStr _at_t7081 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (self->cur_ret))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (fname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7081); _tr_str_release(_at_t7081); });
#line 339 "src/codegen/llvm/emit.tr"
    if (main_args) {
#line 340 "src/codegen/llvm/emit.tr"
        LlvmEmitter_w(self, _tr_str_lit_len("i32 %argc, ptr %argv", 20LL));
    } else {
#line 342 "src/codegen/llvm/emit.tr"
        long long pi = 0LL;
#line 343 "src/codegen/llvm/emit.tr"
        while ((pi < lf->params->len)) {
#line 344 "src/codegen/llvm/emit.tr"
            TrStr pn = List_TrStr_get(lf->params, pi);
#line 345 "src/codegen/llvm/emit.tr"
            TrStr pty = _ll_ty(LFunc_var_type(lf, pn));
#line 346 "src/codegen/llvm/emit.tr"
            if ((pi > 0LL)) {
#line 346 "src/codegen/llvm/emit.tr"
                LlvmEmitter_w(self, _tr_str_lit_len(", ", 2LL));
            }
#line 347 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7082 = (({ TrStr _tr_cl = (_tr_strx_concatv((pty), (_tr_str_lit_len(" %arg_", 6LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7082); _tr_str_release(_at_t7082); });
#line 348 "src/codegen/llvm/emit.tr"
            pi = (pi + 1LL);
            _tr_str_release(pn);
            _tr_str_release(pty);
        }
    }
#line 349 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len(") {\nentry:\n", 11LL));
#line 351 "src/codegen/llvm/emit.tr"
    long long vi = 0LL;
#line 352 "src/codegen/llvm/emit.tr"
    while ((vi < lf->vars->len)) {
#line 353 "src/codegen/llvm/emit.tr"
        TrStr vn = List_TrStr_get(lf->vars, vi);
#line 354 "src/codegen/llvm/emit.tr"
        long long van = List_i64_get(lf->var_arr, vi);
#line 355 "src/codegen/llvm/emit.tr"
        if ((van > 0LL)) {
#line 358 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7083 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), (vn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = alloca [", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(van)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" x i64]\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7083); _tr_str_release(_at_t7083); });
        } else {
#line 360 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7084 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), (vn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_ll_ty(LFunc_var_type(lf, vn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7084); _tr_str_release(_at_t7084); });
        }
#line 361 "src/codegen/llvm/emit.tr"
        vi = (vi + 1LL);
        _tr_str_release(vn);
    }
#line 362 "src/codegen/llvm/emit.tr"
    long long ri = 0LL;
#line 363 "src/codegen/llvm/emit.tr"
    while ((ri < lf->n_vregs)) {
#line 364 "src/codegen/llvm/emit.tr"
        ({ __auto_type _ord_t7085 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ri)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  %v", 4LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7086 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7085); TrStr _tr_cr = (_ll_ty(LFunc_vreg_type(lf, ri))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7086); _tr_str_release(_at_t7086); });
#line 365 "src/codegen/llvm/emit.tr"
        ri = (ri + 1LL);
    }
#line 367 "src/codegen/llvm/emit.tr"
    if (main_args) {
#line 369 "src/codegen/llvm/emit.tr"
        LlvmEmitter_w(self, _tr_str_lit_len("  %argc64 = sext i32 %argc to i64\n", 34LL));
#line 370 "src/codegen/llvm/emit.tr"
        LlvmEmitter_w(self, _tr_str_lit_len("  %argl = call ptr @_tr_rt_argv_list(i64 %argc64, ptr %argv)\n", 61LL));
#line 371 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7087 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(lf->params, 0LL)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  store ptr %argl, ptr %var_", 28LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7087); _tr_str_release(_at_t7087); });
    } else {
#line 373 "src/codegen/llvm/emit.tr"
        long long pi = 0LL;
#line 374 "src/codegen/llvm/emit.tr"
        while ((pi < lf->params->len)) {
#line 375 "src/codegen/llvm/emit.tr"
            TrStr pn2 = List_TrStr_get(lf->params, pi);
#line 376 "src/codegen/llvm/emit.tr"
            TrStr pty2 = _ll_ty(LFunc_var_type(lf, pn2));
#line 377 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7088 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (pty2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7088); _tr_str_release(_at_t7088); });
#line 378 "src/codegen/llvm/emit.tr"
            pi = (pi + 1LL);
            _tr_str_release(pn2);
            _tr_str_release(pty2);
        }
    }
#line 379 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("  br label %bb0\n", 16LL));
#line 381 "src/codegen/llvm/emit.tr"
    long long bi = 0LL;
#line 382 "src/codegen/llvm/emit.tr"
    while ((bi < lf->blocks->len)) {
#line 383 "src/codegen/llvm/emit.tr"
        LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
#line 384 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7089 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("bb", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7089); _tr_str_release(_at_t7089); });
#line 385 "src/codegen/llvm/emit.tr"
        long long ii = 0LL;
#line 386 "src/codegen/llvm/emit.tr"
        while ((ii < blk->insts->len)) {
#line 387 "src/codegen/llvm/emit.tr"
            LlvmEmitter_emit_inst(self, (*((LInst*)List_ptr_get(blk->insts, ii))));
#line 388 "src/codegen/llvm/emit.tr"
            ii = (ii + 1LL);
        }
#line 389 "src/codegen/llvm/emit.tr"
        LlvmEmitter_emit_term(self, blk->term);
#line 390 "src/codegen/llvm/emit.tr"
        bi = (bi + 1LL);
    }
#line 391 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("}\n\n", 3LL));
    _tr_str_release(fname);
}

__attribute__((hot)) void LlvmEmitter_emit_extern_decls(LlvmEmitter* self) {
#line 395 "src/codegen/llvm/emit.tr"
    TrMap* seen = _tr_dict_new(64LL);
#line 396 "src/codegen/llvm/emit.tr"
    long long fi = 0LL;
#line 397 "src/codegen/llvm/emit.tr"
    while ((fi < self->m->funcs->len)) {
#line 398 "src/codegen/llvm/emit.tr"
        LFunc* lf = ((LFunc*)List_ptr_get(self->m->funcs, fi));
#line 399 "src/codegen/llvm/emit.tr"
        LFunc* _cltmp_t7090 = _tr_obj_retain(lf);
        _tr_obj_release(self->cur, _trdrop_LFunc);
        self->cur = _cltmp_t7090;
#line 400 "src/codegen/llvm/emit.tr"
        long long bi = 0LL;
#line 401 "src/codegen/llvm/emit.tr"
        while ((bi < lf->blocks->len)) {
#line 402 "src/codegen/llvm/emit.tr"
            LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
#line 403 "src/codegen/llvm/emit.tr"
            long long ii = 0LL;
#line 404 "src/codegen/llvm/emit.tr"
            while ((ii < blk->insts->len)) {
#line 405 "src/codegen/llvm/emit.tr"
                LlvmEmitter_scan_call_decl(self, (*((LInst*)List_ptr_get(blk->insts, ii))), seen);
#line 406 "src/codegen/llvm/emit.tr"
                ii = (ii + 1LL);
            }
#line 407 "src/codegen/llvm/emit.tr"
            bi = (bi + 1LL);
        }
#line 408 "src/codegen/llvm/emit.tr"
        fi = (fi + 1LL);
    }
}

__attribute__((hot)) void LlvmEmitter_scan_call_decl(LlvmEmitter* self, LInst inst, TrMap* seen) {
#line 411 "src/codegen/llvm/emit.tr"
    __auto_type _t7091 = inst;
    if (_t7091.tag == LInst_ICall) {
        __auto_type dst = _t7091.data.ICall.dst;
__auto_type callee = _t7091.data.ICall.callee;
__auto_type args = _t7091.data.ICall.args;
#line 413 "src/codegen/llvm/emit.tr"
        if (LModule_is_user_fn(self->m, callee)) {
#line 413 "src/codegen/llvm/emit.tr"
            return;
        }
#line 414 "src/codegen/llvm/emit.tr"
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
#line 414 "src/codegen/llvm/emit.tr"
            return;
        }
#line 415 "src/codegen/llvm/emit.tr"
        _tr_dict_set(seen, _tr_strz(callee), true);
#line 416 "src/codegen/llvm/emit.tr"
        TrStr retty = _tr_str_lit_len("void", 4LL);
#line 417 "src/codegen/llvm/emit.tr"
        if ((dst >= 0LL)) {
#line 417 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7092 = LlvmEmitter_vty(self, dst);
            _tr_str_release(retty);
            retty = _strtmp_t7092;
        }
#line 418 "src/codegen/llvm/emit.tr"
        TrStr params = _tr_str_lit_len("", 0LL);
#line 419 "src/codegen/llvm/emit.tr"
        long long ai = 0LL;
#line 420 "src/codegen/llvm/emit.tr"
        while ((ai < args->len)) {
#line 421 "src/codegen/llvm/emit.tr"
            if ((ai > 0LL)) {
#line 421 "src/codegen/llvm/emit.tr"
                ({ TrStr _ap_t7093 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&params, _ap_t7093); });
            }
#line 422 "src/codegen/llvm/emit.tr"
            ({ TrStr _ap_t7094 = (LlvmEmitter_vty(self, List_i64_get(args, ai))); _tr_str_append(&params, _ap_t7094); _tr_str_release(_ap_t7094); });
#line 423 "src/codegen/llvm/emit.tr"
            ai = (ai + 1LL);
        }
#line 424 "src/codegen/llvm/emit.tr"
        TrStr _na = _tr_str_lit_len("", 0LL);
#line 425 "src/codegen/llvm/emit.tr"
        if ((_tr_str_eqv((retty), (_tr_str_lit_len("ptr", 3LL))) && _is_fresh_alloc_ret(callee))) {
#line 425 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7095 = _tr_str_lit_len("noalias ", 8LL);
            _tr_str_release(_na);
            _na = _strtmp_t7095;
        }
#line 426 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7096 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("declare ", 8LL)), (_na))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (retty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (callee)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (params)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7096); _tr_str_release(_at_t7096); });
        _tr_str_release(retty);
        _tr_str_release(params);
        _tr_str_release(_na);
    } else if (_t7091.tag == LInst_IFCall1) {
        __auto_type dst = _t7091.data.IFCall1.dst;
__auto_type callee = _t7091.data.IFCall1.callee;
#line 428 "src/codegen/llvm/emit.tr"
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
#line 428 "src/codegen/llvm/emit.tr"
            return;
        }
#line 429 "src/codegen/llvm/emit.tr"
        _tr_dict_set(seen, _tr_strz(callee), true);
#line 430 "src/codegen/llvm/emit.tr"
        TrStr rt1 = _tr_str_lit_len("void", 4LL);
#line 431 "src/codegen/llvm/emit.tr"
        if ((dst >= 0LL)) {
#line 431 "src/codegen/llvm/emit.tr"
            TrStr _strtmp_t7097 = LlvmEmitter_vty(self, dst);
            _tr_str_release(rt1);
            rt1 = _strtmp_t7097;
        }
#line 432 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7098 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("declare ", 8LL)), (rt1))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (callee)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(double)\n", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7098); _tr_str_release(_at_t7098); });
        _tr_str_release(rt1);
    } else if (_t7091.tag == LInst_IFCallF) {
        __auto_type callee = _t7091.data.IFCallF.callee;
#line 434 "src/codegen/llvm/emit.tr"
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
#line 434 "src/codegen/llvm/emit.tr"
            return;
        }
#line 435 "src/codegen/llvm/emit.tr"
        _tr_dict_set(seen, _tr_strz(callee), true);
#line 436 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7099 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("declare double @", 16LL)), (callee))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(double)\n", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7099); _tr_str_release(_at_t7099); });
    } else if (_t7091.tag == LInst_IFCall2F) {
        __auto_type callee = _t7091.data.IFCall2F.callee;
#line 438 "src/codegen/llvm/emit.tr"
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
#line 438 "src/codegen/llvm/emit.tr"
            return;
        }
#line 439 "src/codegen/llvm/emit.tr"
        _tr_dict_set(seen, _tr_strz(callee), true);
#line 440 "src/codegen/llvm/emit.tr"
        ({ TrStr _at_t7100 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("declare double @", 16LL)), (callee))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(double, double)\n", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7100); _tr_str_release(_at_t7100); });
    } else if (1) {
        __auto_type _ = _t7091;
#line 442 "src/codegen/llvm/emit.tr"
        /* pass */
    }
}

__attribute__((hot)) TrStr LlvmEmitter_emit_module(LlvmEmitter* self) {
#line 446 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("; Tauraro LLVM IR — backend A (taumir LIR -> LLVM IR -> llc)\n\n", 64LL));
#line 448 "src/codegen/llvm/emit.tr"
    long long si = 0LL;
#line 449 "src/codegen/llvm/emit.tr"
    while ((si < self->m->strings->len)) {
#line 450 "src/codegen/llvm/emit.tr"
        TrStr s = List_TrStr_get(self->m->strings, si);
#line 451 "src/codegen/llvm/emit.tr"
        long long blen = (List_i64_get(self->m->string_lens, si) + 1LL);
#line 452 "src/codegen/llvm/emit.tr"
        ({ __auto_type _ord_t7101 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(si)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("@.str.", 6LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = private unnamed_addr constant [", 34LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t7102 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7101); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(blen)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" x i8] c\"", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7103 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7102); TrStr _tr_cr = (_ll_str_escape(s, List_i64_get(self->m->string_lens, si))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\\00\"\n", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7103); _tr_str_release(_at_t7103); });
#line 453 "src/codegen/llvm/emit.tr"
        si = (si + 1LL);
        _tr_str_release(s);
    }
#line 455 "src/codegen/llvm/emit.tr"
    long long gi = 0LL;
#line 456 "src/codegen/llvm/emit.tr"
    while ((gi < self->m->globals->len)) {
#line 457 "src/codegen/llvm/emit.tr"
        long long gan = List_i64_get(self->m->global_arr, gi);
#line 458 "src/codegen/llvm/emit.tr"
        if ((gan > 0LL)) {
#line 461 "src/codegen/llvm/emit.tr"
            ({ __auto_type _ord_t7104 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(gi)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("@g.", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = internal global [", 20LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7105 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7104); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(gan)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" x i64] zeroinitializer\n", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7105); _tr_str_release(_at_t7105); });
        } else {
#line 463 "src/codegen/llvm/emit.tr"
            TrStr gty = _ll_ty(List_i64_get(self->m->global_types, gi));
#line 464 "src/codegen/llvm/emit.tr"
            TrStr init = _tr_str_lit_len("0", 1LL);
#line 465 "src/codegen/llvm/emit.tr"
            if (_tr_str_eqv((gty), (_tr_str_lit_len("double", 6LL)))) {
#line 465 "src/codegen/llvm/emit.tr"
                TrStr _strtmp_t7106 = _tr_str_lit_len("0.0", 3LL);
                _tr_str_release(init);
                init = _strtmp_t7106;
            }
#line 466 "src/codegen/llvm/emit.tr"
            if (_tr_str_eqv((gty), (_tr_str_lit_len("ptr", 3LL)))) {
#line 466 "src/codegen/llvm/emit.tr"
                TrStr _strtmp_t7107 = _tr_str_lit_len("null", 4LL);
                _tr_str_release(init);
                init = _strtmp_t7107;
            }
#line 467 "src/codegen/llvm/emit.tr"
            ({ TrStr _at_t7108 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(gi)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("@g.", 3LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = internal global ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (init)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); LlvmEmitter_w(self, _at_t7108); _tr_str_release(_at_t7108); });
            _tr_str_release(gty);
            _tr_str_release(init);
        }
#line 468 "src/codegen/llvm/emit.tr"
        gi = (gi + 1LL);
    }
#line 469 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("\n", 1LL));
#line 470 "src/codegen/llvm/emit.tr"
    LlvmEmitter_emit_extern_decls(self);
#line 471 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("declare ptr @_tr_rt_argv_list(i64, ptr)\n", 40LL));
#line 472 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("\n", 1LL));
#line 478 "src/codegen/llvm/emit.tr"
    List_TrStr* emitted = (void*)List_TrStr_new();
#line 479 "src/codegen/llvm/emit.tr"
    long long fi = 0LL;
#line 480 "src/codegen/llvm/emit.tr"
    while ((fi < self->m->funcs->len)) {
#line 481 "src/codegen/llvm/emit.tr"
        LFunc* efn = ((LFunc*)List_ptr_get(self->m->funcs, fi));
#line 482 "src/codegen/llvm/emit.tr"
        bool dup = false;
#line 483 "src/codegen/llvm/emit.tr"
        long long ei = 0LL;
#line 484 "src/codegen/llvm/emit.tr"
        while ((ei < emitted->len)) {
#line 485 "src/codegen/llvm/emit.tr"
            if (({ __auto_type _ord_t7109 = (List_TrStr_get(emitted, ei)); __auto_type _wr = (({ TrStr _cmpa_t7110 = (_ord_t7109); TrStr _cmpb_t7111 = (efn->name); _Bool _cmpr_t7112 = _tr_str_eqv(_cmpa_t7110, _cmpb_t7111); _tr_str_release(_cmpa_t7110); _cmpr_t7112; })); _wr; })) {
#line 485 "src/codegen/llvm/emit.tr"
                dup = true;
#line 485 "src/codegen/llvm/emit.tr"
                ei = emitted->len;
            } else {
#line 486 "src/codegen/llvm/emit.tr"
                ei = (ei + 1LL);
            }
        }
#line 487 "src/codegen/llvm/emit.tr"
        if ((!dup)) {
#line 488 "src/codegen/llvm/emit.tr"
            List_TrStr_append(emitted, efn->name);
#line 489 "src/codegen/llvm/emit.tr"
            LlvmEmitter_emit_function(self, efn);
        }
#line 490 "src/codegen/llvm/emit.tr"
        fi = (fi + 1LL);
    }
#line 496 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("\n", 1LL));
#line 497 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("!0 = !{!\"tauraro_tbaa_root\"}\n", 29LL));
#line 498 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("!1 = !{!\"listhdr\", !0, i64 0}\n", 30LL));
#line 499 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("!2 = !{!\"listelem\", !0, i64 0}\n", 31LL));
#line 500 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("!3 = !{!\"objfield\", !0, i64 0}\n", 31LL));
#line 501 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("!4 = !{!1, !1, i64 0}\n", 22LL));
#line 502 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("!5 = !{!2, !2, i64 0}\n", 22LL));
#line 503 "src/codegen/llvm/emit.tr"
    LlvmEmitter_w(self, _tr_str_lit_len("!6 = !{!3, !3, i64 0}\n", 22LL));
#line 504 "src/codegen/llvm/emit.tr"
    TrStr _trret1884 = StringObj_as_str(StringBuilder_to_string(self->out));
    List_TrStr_free(emitted);
    return _trret1884;
}

__attribute__((hot)) TrStr _tbaa_suffix(long long aclass) {
#line 509 "src/codegen/llvm/emit.tr"
    if ((aclass == 1LL)) {
#line 509 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len(", !tbaa !4", 10LL);
    }
#line 510 "src/codegen/llvm/emit.tr"
    if ((aclass == 2LL)) {
#line 510 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len(", !tbaa !5", 10LL);
    }
#line 511 "src/codegen/llvm/emit.tr"
    if ((aclass == 3LL)) {
#line 511 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len(", !tbaa !6", 10LL);
    }
#line 512 "src/codegen/llvm/emit.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _is_fresh_alloc_ret(TrStr name) {
#line 521 "src/codegen/llvm/emit.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("malloc", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("calloc", 6LL))))) {
#line 521 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 522 "src/codegen/llvm/emit.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("_tr_c_malloc", 12LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_c_calloc", 12LL))))) {
#line 522 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 523 "src/codegen/llvm/emit.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("_tr_checked_alloc", 17LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_raw_alloc", 16LL))))) {
#line 523 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 524 "src/codegen/llvm/emit.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_list_new", 15LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_argv_list", 16LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_blist_new", 16LL))))) {
#line 524 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 525 "src/codegen/llvm/emit.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_idict_new", 16LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_sdict_new", 16LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_dict_new", 12LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_idict_new", 13LL))))) {
#line 525 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 526 "src/codegen/llvm/emit.tr"
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_set_new", 14LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_set_new", 11LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_iset_new", 12LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_pset_new", 12LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_sset_new", 12LL))))) {
#line 526 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 527 "src/codegen/llvm/emit.tr"
    if ((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_obj_alloc", 16LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_obj_alloc", 13LL))))) {
#line 527 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 528 "src/codegen/llvm/emit.tr"
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_str_new", 14LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_str_new", 11LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_str_alloc", 16LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_char_to_str_alloc", 21LL))))) {
#line 528 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 529 "src/codegen/llvm/emit.tr"
    if (((_tr_str_eqv((name), (_tr_str_lit_len("_tr_bytes_new", 13LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_range_new", 13LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_shared_new", 14LL))))) {
#line 529 "src/codegen/llvm/emit.tr"
        return true;
    }
#line 530 "src/codegen/llvm/emit.tr"
    return false;
}

__attribute__((hot)) TrStr _ll_int_instr(TrStr op) {
#line 534 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
#line 534 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("add", 3LL);
    }
#line 535 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 535 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("sub", 3LL);
    }
#line 536 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 536 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("mul", 3LL);
    }
#line 537 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
#line 537 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("sdiv", 4LL);
    }
#line 538 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) {
#line 538 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("sdiv", 4LL);
    }
#line 539 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) {
#line 539 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("srem", 4LL);
    }
#line 540 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 540 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("and", 3LL);
    }
#line 541 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("|", 1LL)))) {
#line 541 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("or", 2LL);
    }
#line 542 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("^", 1LL)))) {
#line 542 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("xor", 3LL);
    }
#line 543 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<<", 2LL)))) {
#line 543 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("shl", 3LL);
    }
#line 544 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">>", 2LL)))) {
#line 544 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("ashr", 4LL);
    }
#line 545 "src/codegen/llvm/emit.tr"
    return _tr_str_lit_len("add", 3LL);
}

__attribute__((hot)) TrStr _ll_icmp_pred(TrStr op) {
#line 549 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
#line 549 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("eq", 2LL);
    }
#line 550 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
#line 550 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("ne", 2LL);
    }
#line 551 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
#line 551 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("slt", 3LL);
    }
#line 552 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
#line 552 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("sle", 3LL);
    }
#line 553 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
#line 553 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("sgt", 3LL);
    }
#line 554 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
#line 554 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("sge", 3LL);
    }
#line 555 "src/codegen/llvm/emit.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _ll_float_instr(TrStr op) {
#line 559 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
#line 559 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("fadd", 4LL);
    }
#line 560 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 560 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("fsub", 4LL);
    }
#line 561 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 561 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("fmul", 4LL);
    }
#line 562 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
#line 562 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("fdiv", 4LL);
    }
#line 563 "src/codegen/llvm/emit.tr"
    return _tr_str_lit_len("fadd", 4LL);
}

__attribute__((hot)) TrStr _ll_fcmp_pred(TrStr op) {
#line 567 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
#line 567 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("oeq", 3LL);
    }
#line 568 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
#line 568 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("one", 3LL);
    }
#line 569 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
#line 569 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("olt", 3LL);
    }
#line 570 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
#line 570 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("ole", 3LL);
    }
#line 571 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
#line 571 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("ogt", 3LL);
    }
#line 572 "src/codegen/llvm/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
#line 572 "src/codegen/llvm/emit.tr"
        return _tr_str_lit_len("oge", 3LL);
    }
#line 573 "src/codegen/llvm/emit.tr"
    return _tr_str_lit_len("", 0LL);
}

