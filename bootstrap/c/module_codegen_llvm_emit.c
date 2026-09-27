#include "tauraro_types.h"

TrStr _tbaa_suffix(long long aclass);
bool _is_fresh_alloc_ret(TrStr name);
TrStr _ll_int_instr(TrStr op);
TrStr _ll_icmp_pred(TrStr op);
TrStr _ll_float_instr(TrStr op);
TrStr _ll_fcmp_pred(TrStr op);

__attribute__((malloc,returns_nonnull,hot)) LlvmEmitter* LlvmEmitter_init(LModule* m) {
    /* pass */
    LlvmEmitter* e = ((LlvmEmitter*)_tr_obj_alloc(sizeof(LlvmEmitter)));
    /* pass */
    e->out = StringBuilder_init(4096LL);
    /* pass */
    LModule* _cltmp_t3327 = _tr_obj_retain(m);
    _tr_obj_release(e->m, _trdrop_LModule);
    e->m = _cltmp_t3327;
    /* pass */
    e->tmp = 0LL;
    /* pass */
    e->cur_ret = _tr_str_lit_len("i64", 3LL);
    /* pass */
    e->cur_main = false;
    /* pass */
    return e;
}

__attribute__((hot)) void LlvmEmitter_w(LlvmEmitter* self, TrStr s) {
    /* pass */
    StringBuilder_append(self->out, s);
}

__attribute__((hot)) TrStr LlvmEmitter_newtmp(LlvmEmitter* self) {
    /* pass */
    self->tmp = (self->tmp + 1LL);
    /* pass */
    return ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->tmp)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("%t", 2LL)), _cr); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) TrStr LlvmEmitter_vty(LlvmEmitter* self, long long v) {
    /* pass */
    return _ll_ty(LFunc_vreg_type(self->cur, v));
}

__attribute__((hot)) TrStr LlvmEmitter_load_vreg(LlvmEmitter* self, long long v) {
    /* pass */
    TrStr t = LlvmEmitter_newtmp(self);
    /* pass */
    ({ TrStr _at_t3328 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (t))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (LlvmEmitter_vty(self, v)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3328); _tr_str_release(_at_t3328); });
    /* pass */
    return t;
}

__attribute__((hot)) TrStr LlvmEmitter_load_vreg_as(LlvmEmitter* self, long long v, TrStr ty) {
    /* pass */
    TrStr t = LlvmEmitter_newtmp(self);
    /* pass */
    ({ TrStr _at_t3329 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (t))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3329); _tr_str_release(_at_t3329); });
    /* pass */
    return t;
}

__attribute__((hot)) void LlvmEmitter_store_vreg(LlvmEmitter* self, long long v, TrStr ty, TrStr val) {
    /* pass */
    ({ TrStr _at_t3330 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (ty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3330); _tr_str_release(_at_t3330); });
}

__attribute__((hot)) TrStr LlvmEmitter_addr_of_base(LlvmEmitter* self, long long v) {
    /* pass */
    if (_tr_str_eqv((LlvmEmitter_vty(self, v)), (_tr_str_lit_len("ptr", 3LL)))) {
        /* pass */
        return LlvmEmitter_load_vreg_as(self, v, _tr_str_lit_len("ptr", 3LL));
    }
    /* pass */
    TrStr bi = LlvmEmitter_load_vreg_as(self, v, _tr_str_lit_len("i64", 3LL));
    /* pass */
    TrStr bp = LlvmEmitter_newtmp(self);
    /* pass */
    ({ TrStr _at_t3331 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = inttoptr i64 ", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (bi)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to ptr\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3331); _tr_str_release(_at_t3331); });
    /* pass */
    _tr_str_release(bi);
    return bp;
}

__attribute__((hot)) TrStr LlvmEmitter_user_ret_ty(LlvmEmitter* self, TrStr name) {
    /* pass */
    return _ll_ty(LModule_fn_ret_tag(self->m, name));
}

__attribute__((hot)) void LlvmEmitter_emit_inst(LlvmEmitter* self, LInst inst) {
    /* pass */
    __auto_type _t3332 = inst;
    if (_t3332.tag == LInst_IConst) {
        __auto_type dst = _t3332.data.IConst.dst;
__auto_type v = _t3332.data.IConst.v;
        /* pass */
        if (_tr_str_eqv((LlvmEmitter_vty(self, dst)), (_tr_str_lit_len("double", 6LL)))) {
            /* pass */
            ({ TrStr _at_t3333 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_ll_hexpad16(v)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  store double 0x", 17LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %v", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(dst)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3333); _tr_str_release(_at_t3333); });
        } else {
            /* pass */
            ({ TrStr _at_t3334 = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), _at_t3334); _tr_str_release(_at_t3334); });
        }
    } else if (_t3332.tag == LInst_IStr) {
        __auto_type dst = _t3332.data.IStr.dst;
__auto_type str_idx = _t3332.data.IStr.str_idx;
        /* pass */
        ({ TrStr _at_t3335 = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(str_idx)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("@.str.", 6LL)), _cr); _tr_str_release(_cr); _cres; })); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t3335); _tr_str_release(_at_t3335); });
    } else if (_t3332.tag == LInst_IBinOp) {
        __auto_type dst = _t3332.data.IBinOp.dst;
__auto_type op = _t3332.data.IBinOp.op;
__auto_type a = _t3332.data.IBinOp.a;
__auto_type b = _t3332.data.IBinOp.b;
        /* pass */
        if ((_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL))) && _tr_str_eqv((LlvmEmitter_vty(self, dst)), (_tr_str_lit_len("ptr", 3LL))))) {
            /* pass */
            TrStr pbase = LlvmEmitter_addr_of_base(self, a);
            /* pass */
            TrStr poff = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("i64", 3LL));
            /* pass */
            TrStr pg = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3336 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (pg))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pbase)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (poff)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3336); _tr_str_release(_at_t3336); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), pg);
            _tr_str_release(pbase);
            _tr_str_release(poff);
            _tr_str_release(pg);
        } else {
            /* pass */
            TrStr la = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit_len("i64", 3LL));
            /* pass */
            TrStr lb = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("i64", 3LL));
            /* pass */
            TrStr pred = _ll_icmp_pred(op);
            /* pass */
            if ((!_tr_str_eqv((pred), (_tr_str_lit_len("", 0LL))))) {
                /* pass */
                TrStr c = LlvmEmitter_newtmp(self);
                /* pass */
                ({ TrStr _at_t3337 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (c))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = icmp ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" i64 ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3337); _tr_str_release(_at_t3337); });
                /* pass */
                TrStr z = LlvmEmitter_newtmp(self);
                /* pass */
                ({ TrStr _at_t3338 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (z))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = zext i1 ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (c)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3338); _tr_str_release(_at_t3338); });
                /* pass */
                LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), z);
                _tr_str_release(c);
                _tr_str_release(z);
            } else {
                /* pass */
                TrStr r = LlvmEmitter_newtmp(self);
                /* pass */
                ({ TrStr _at_t3339 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_int_instr(op)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" i64 ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3339); _tr_str_release(_at_t3339); });
                /* pass */
                LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), r);
                _tr_str_release(r);
            }
        }
    } else if (_t3332.tag == LInst_IFBinOp) {
        __auto_type dst = _t3332.data.IFBinOp.dst;
__auto_type op = _t3332.data.IFBinOp.op;
__auto_type a = _t3332.data.IFBinOp.a;
__auto_type b = _t3332.data.IFBinOp.b;
        /* pass */
        TrStr fa = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr fb = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr fpred = _ll_fcmp_pred(op);
        /* pass */
        if ((!_tr_str_eqv((fpred), (_tr_str_lit_len("", 0LL))))) {
            /* pass */
            TrStr c2 = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3340 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (c2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fcmp ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fpred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" double ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fa)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3340); _tr_str_release(_at_t3340); });
            /* pass */
            TrStr z2 = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3341 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (z2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = zext i1 ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (c2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3341); _tr_str_release(_at_t3341); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), z2);
            _tr_str_release(c2);
            _tr_str_release(z2);
        } else {
            /* pass */
            TrStr fr = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3342 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (fr))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = ", 3LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_float_instr(op)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" double ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fa)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3342); _tr_str_release(_at_t3342); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), fr);
            _tr_str_release(fr);
        }
        _tr_str_release(fa);
        _tr_str_release(fb);
        _tr_str_release(fpred);
    } else if (_t3332.tag == LInst_IIToF) {
        __auto_type dst = _t3332.data.IIToF.dst;
__auto_type src = _t3332.data.IIToF.src;
        /* pass */
        TrStr si = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("i64", 3LL));
        /* pass */
        TrStr rf = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3343 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (rf))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = sitofp i64 ", 14LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (si)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to double\n", 11LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3343); _tr_str_release(_at_t3343); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), rf);
        _tr_str_release(si);
        _tr_str_release(rf);
    } else if (_t3332.tag == LInst_IFToI) {
        __auto_type dst = _t3332.data.IFToI.dst;
__auto_type src = _t3332.data.IFToI.src;
        /* pass */
        TrStr sf = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr ri = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3344 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (ri))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fptosi double ", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sf)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3344); _tr_str_release(_at_t3344); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), ri);
        _tr_str_release(sf);
        _tr_str_release(ri);
    } else if (_t3332.tag == LInst_ILoadVar) {
        __auto_type dst = _t3332.data.ILoadVar.dst;
__auto_type name = _t3332.data.ILoadVar.name;
        /* pass */
        TrStr vt = _ll_ty(LFunc_var_type(self->cur, name));
        /* pass */
        TrStr t = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3345 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (t))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3345); _tr_str_release(_at_t3345); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, vt, t);
        _tr_str_release(vt);
        _tr_str_release(t);
    } else if (_t3332.tag == LInst_IStoreVar) {
        __auto_type name = _t3332.data.IStoreVar.name;
__auto_type src = _t3332.data.IStoreVar.src;
        /* pass */
        TrStr st = LlvmEmitter_vty(self, src);
        /* pass */
        TrStr ls = LlvmEmitter_load_vreg_as(self, src, st);
        /* pass */
        ({ TrStr _at_t3346 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (st))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ls)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3346); _tr_str_release(_at_t3346); });
        _tr_str_release(st);
        _tr_str_release(ls);
    } else if (_t3332.tag == LInst_ILoadGlobal) {
        __auto_type dst = _t3332.data.ILoadGlobal.dst;
__auto_type gidx = _t3332.data.ILoadGlobal.gidx;
        /* pass */
        TrStr gt = _ll_ty(List_i64_get(self->m->global_types, gidx));
        /* pass */
        TrStr tg = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3347 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tg))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr @g.", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3347); _tr_str_release(_at_t3347); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, gt, tg);
        _tr_str_release(gt);
        _tr_str_release(tg);
    } else if (_t3332.tag == LInst_IStoreGlobal) {
        __auto_type gidx = _t3332.data.IStoreGlobal.gidx;
__auto_type src = _t3332.data.IStoreGlobal.src;
        /* pass */
        TrStr gt2 = LlvmEmitter_vty(self, src);
        /* pass */
        TrStr lg = LlvmEmitter_load_vreg_as(self, src, gt2);
        /* pass */
        ({ TrStr _at_t3348 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (gt2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lg)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr @g.", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3348); _tr_str_release(_at_t3348); });
        _tr_str_release(gt2);
        _tr_str_release(lg);
    } else if (_t3332.tag == LInst_ICall) {
        __auto_type dst = _t3332.data.ICall.dst;
__auto_type callee = _t3332.data.ICall.callee;
__auto_type args = _t3332.data.ICall.args;
        /* pass */
        LlvmEmitter_emit_call(self, dst, callee, args);
    } else if (_t3332.tag == LInst_IFCall1) {
        __auto_type dst = _t3332.data.IFCall1.dst;
__auto_type callee = _t3332.data.IFCall1.callee;
__auto_type arg = _t3332.data.IFCall1.arg;
        /* pass */
        TrStr a1 = LlvmEmitter_load_vreg_as(self, arg, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr rt1 = _tr_str_lit_len("void", 4LL);
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3349 = LlvmEmitter_vty(self, dst);
            _tr_str_release(rt1);
            rt1 = _strtmp_t3349;
        }
        /* pass */
        if (_tr_str_eqv((rt1), (_tr_str_lit_len("void", 4LL)))) {
            /* pass */
            ({ TrStr _at_t3350 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  call void @", 13LL)), (callee))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (a1)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3350); _tr_str_release(_at_t3350); });
        } else {
            /* pass */
            TrStr r1 = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3351 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r1))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (rt1)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (a1)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3351); _tr_str_release(_at_t3351); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, rt1, r1);
            _tr_str_release(r1);
        }
        _tr_str_release(a1);
        _tr_str_release(rt1);
    } else if (_t3332.tag == LInst_IFCallF) {
        __auto_type dst = _t3332.data.IFCallF.dst;
__auto_type callee = _t3332.data.IFCallF.callee;
__auto_type arg = _t3332.data.IFCallF.arg;
        /* pass */
        TrStr a2 = LlvmEmitter_load_vreg_as(self, arg, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr r2 = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3352 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call double @", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (a2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3352); _tr_str_release(_at_t3352); });
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), r2);
        }
        _tr_str_release(a2);
        _tr_str_release(r2);
    } else if (_t3332.tag == LInst_IFCall2F) {
        __auto_type dst = _t3332.data.IFCall2F.dst;
__auto_type callee = _t3332.data.IFCall2F.callee;
__auto_type a = _t3332.data.IFCall2F.a;
__auto_type b = _t3332.data.IFCall2F.b;
        /* pass */
        TrStr fa2 = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr fb2 = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr r3 = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3353 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r3))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call double @", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(double ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fa2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", double ", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fb2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3353); _tr_str_release(_at_t3353); });
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), r3);
        }
        _tr_str_release(fa2);
        _tr_str_release(fb2);
        _tr_str_release(r3);
    } else if (_t3332.tag == LInst_IBitsF) {
        __auto_type dst = _t3332.data.IBitsF.dst;
__auto_type src = _t3332.data.IBitsF.src;
        /* pass */
        TrStr bi = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("i64", 3LL));
        /* pass */
        TrStr bf = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3354 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bf))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = bitcast i64 ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (bi)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to double\n", 11LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3354); _tr_str_release(_at_t3354); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("double", 6LL), bf);
        _tr_str_release(bi);
        _tr_str_release(bf);
    } else if (_t3332.tag == LInst_IFBits) {
        __auto_type dst = _t3332.data.IFBits.dst;
__auto_type src = _t3332.data.IFBits.src;
        /* pass */
        TrStr bd = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("double", 6LL));
        /* pass */
        TrStr bx = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3355 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bx))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = bitcast double ", 18LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (bd)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3355); _tr_str_release(_at_t3355); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), bx);
        _tr_str_release(bd);
        _tr_str_release(bx);
    } else if (_t3332.tag == LInst_IAddrVar) {
        __auto_type dst = _t3332.data.IAddrVar.dst;
__auto_type name = _t3332.data.IAddrVar.name;
        /* pass */
        ({ TrStr _at_t3356 = (_tr_strx_concatv((_tr_str_lit_len("%var_", 5LL)), (name))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t3356); _tr_str_release(_at_t3356); });
    } else if (_t3332.tag == LInst_IAddrGlobal) {
        __auto_type dst = _t3332.data.IAddrGlobal.dst;
__auto_type gidx = _t3332.data.IAddrGlobal.gidx;
        /* pass */
        ({ TrStr _at_t3357 = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("@g.", 3LL)), _cr); _tr_str_release(_cr); _cres; })); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t3357); _tr_str_release(_at_t3357); });
    } else if (_t3332.tag == LInst_IFuncAddr) {
        __auto_type dst = _t3332.data.IFuncAddr.dst;
__auto_type fname = _t3332.data.IFuncAddr.fname;
        /* pass */
        ({ TrStr _at_t3358 = (_tr_strx_concatv((_tr_str_lit_len("@", 1LL)), (fname))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("ptr", 3LL), _at_t3358); _tr_str_release(_at_t3358); });
    } else if (_t3332.tag == LInst_ICallInd) {
        __auto_type dst = _t3332.data.ICallInd.dst;
__auto_type fnreg = _t3332.data.ICallInd.fnreg;
__auto_type iargs = _t3332.data.ICallInd.args;
        /* pass */
        TrStr ind_args = _tr_str_lit_len("", 0LL);
        /* pass */
        long long ii2 = 0LL;
        /* pass */
        while ((ii2 < iargs->len)) {
            /* pass */
            long long iav = List_i64_get(iargs, ii2);
            /* pass */
            TrStr iaty = LlvmEmitter_vty(self, iav);
            /* pass */
            TrStr ial = LlvmEmitter_load_vreg_as(self, iav, iaty);
            /* pass */
            if ((ii2 > 0LL)) {
                /* pass */
                TrStr _strtmp_t3359 = _tr_strx_concatv((ind_args), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(ind_args);
                ind_args = _strtmp_t3359;
            }
            /* pass */
            TrStr _strtmp_t3360 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((ind_args), (iaty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ial)); _tr_str_release(_cl); _cres; });
            _tr_str_release(ind_args);
            ind_args = _strtmp_t3360;
            /* pass */
            ii2 = (ii2 + 1LL);
            _tr_str_release(iaty);
            _tr_str_release(ial);
        }
        /* pass */
        TrStr fp = LlvmEmitter_load_vreg_as(self, fnreg, _tr_str_lit_len("ptr", 3LL));
        /* pass */
        TrStr ind_ret = _tr_str_lit_len("void", 4LL);
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3361 = LlvmEmitter_vty(self, dst);
            _tr_str_release(ind_ret);
            ind_ret = _strtmp_t3361;
        }
        /* pass */
        if (_tr_str_eqv((ind_ret), (_tr_str_lit_len("void", 4LL)))) {
            /* pass */
            ({ TrStr _at_t3362 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  call void ", 12LL)), (fp))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ind_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3362); _tr_str_release(_at_t3362); });
        } else {
            /* pass */
            TrStr ir = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3363 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (ir))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ind_ret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ind_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3363); _tr_str_release(_at_t3363); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, ind_ret, ir);
            _tr_str_release(ir);
        }
        _tr_str_release(ind_args);
        _tr_str_release(fp);
        _tr_str_release(ind_ret);
    } else if (_t3332.tag == LInst_IAsm) {
        __auto_type code = _t3332.data.IAsm.code;
__auto_type cons = _t3332.data.IAsm.cons;
        /* pass */
        ({ TrStr _at_t3364 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_ll_str_escape(code, _tr_str_lenv((code)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  call void asm sideeffect \"", 28LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\", \"", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cons)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\"()\n", 4LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3364); _tr_str_release(_at_t3364); });
    } else if (_t3332.tag == LInst_ILoad) {
        __auto_type dst = _t3332.data.ILoad.dst;
__auto_type base = _t3332.data.ILoad.base;
__auto_type off = _t3332.data.ILoad.off;
__auto_type aclass = _t3332.data.ILoad.aclass;
        /* pass */
        TrStr lbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr lep = _tr_str_retain(lbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3365 = LlvmEmitter_newtmp(self);
            _tr_str_release(lep);
            lep = _strtmp_t3365;
            /* pass */
            ({ TrStr _at_t3366 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (lep))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3366); _tr_str_release(_at_t3366); });
        }
        /* pass */
        TrStr ldty = LlvmEmitter_vty(self, dst);
        /* pass */
        TrStr lv = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3367 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (lv))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ldty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lep)); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tbaa_suffix(aclass)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3367); _tr_str_release(_at_t3367); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, ldty, lv);
        _tr_str_release(lbp);
        _tr_str_release(lep);
        _tr_str_release(ldty);
        _tr_str_release(lv);
    } else if (_t3332.tag == LInst_IStore) {
        __auto_type base = _t3332.data.IStore.base;
__auto_type off = _t3332.data.IStore.off;
__auto_type src = _t3332.data.IStore.src;
__auto_type aclass = _t3332.data.IStore.aclass;
        /* pass */
        TrStr sbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr sep = _tr_str_retain(sbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3368 = LlvmEmitter_newtmp(self);
            _tr_str_release(sep);
            sep = _strtmp_t3368;
            /* pass */
            ({ TrStr _at_t3369 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (sep))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3369); _tr_str_release(_at_t3369); });
        }
        /* pass */
        TrStr ssty = LlvmEmitter_vty(self, src);
        /* pass */
        TrStr ssv = LlvmEmitter_load_vreg(self, src);
        /* pass */
        ({ TrStr _at_t3370 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (ssty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ssv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (sep)); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tbaa_suffix(aclass)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3370); _tr_str_release(_at_t3370); });
        _tr_str_release(sbp);
        _tr_str_release(sep);
        _tr_str_release(ssty);
        _tr_str_release(ssv);
    } else if (_t3332.tag == LInst_ILoadB) {
        __auto_type dst = _t3332.data.ILoadB.dst;
__auto_type base = _t3332.data.ILoadB.base;
__auto_type off = _t3332.data.ILoadB.off;
        /* pass */
        TrStr bbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr bep = _tr_str_retain(bbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3371 = LlvmEmitter_newtmp(self);
            _tr_str_release(bep);
            bep = _strtmp_t3371;
            /* pass */
            ({ TrStr _at_t3372 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bep))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (bbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3372); _tr_str_release(_at_t3372); });
        }
        /* pass */
        TrStr bb = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3373 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bb))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load i8, ptr ", 16LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (bep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", !tbaa !5\n", 11LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3373); _tr_str_release(_at_t3373); });
        /* pass */
        TrStr bz = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3374 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (bz))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = zext i8 ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (bb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3374); _tr_str_release(_at_t3374); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit_len("i64", 3LL), bz);
        _tr_str_release(bbp);
        _tr_str_release(bep);
        _tr_str_release(bb);
        _tr_str_release(bz);
    } else if (_t3332.tag == LInst_IStoreB) {
        __auto_type base = _t3332.data.IStoreB.base;
__auto_type off = _t3332.data.IStoreB.off;
__auto_type src = _t3332.data.IStoreB.src;
        /* pass */
        TrStr tbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr tep = _tr_str_retain(tbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3375 = LlvmEmitter_newtmp(self);
            _tr_str_release(tep);
            tep = _strtmp_t3375;
            /* pass */
            ({ TrStr _at_t3376 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tep))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = getelementptr i8, ptr ", 25LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3376); _tr_str_release(_at_t3376); });
        }
        /* pass */
        TrStr tsv = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit_len("i64", 3LL));
        /* pass */
        TrStr tt = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3377 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tt))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = trunc i64 ", 13LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tsv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i8\n", 7LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3377); _tr_str_release(_at_t3377); });
        /* pass */
        ({ TrStr _at_t3378 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store i8 ", 11LL)), (tt))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", !tbaa !5\n", 11LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3378); _tr_str_release(_at_t3378); });
        _tr_str_release(tbp);
        _tr_str_release(tep);
        _tr_str_release(tsv);
        _tr_str_release(tt);
    }
}

__attribute__((hot)) void LlvmEmitter_emit_call(LlvmEmitter* self, long long dst, TrStr callee, List_i64* args) {
    /* pass */
    TrStr arglist = _tr_str_lit_len("", 0LL);
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < args->len)) {
        /* pass */
        long long av = List_i64_get(args, ai);
        /* pass */
        TrStr aty = LlvmEmitter_vty(self, av);
        /* pass */
        TrStr al = LlvmEmitter_load_vreg_as(self, av, aty);
        /* pass */
        if ((ai > 0LL)) {
            /* pass */
            TrStr _strtmp_t3379 = _tr_strx_concatv((arglist), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(arglist);
            arglist = _strtmp_t3379;
        }
        /* pass */
        TrStr _strtmp_t3380 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((arglist), (aty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (al)); _tr_str_release(_cl); _cres; });
        _tr_str_release(arglist);
        arglist = _strtmp_t3380;
        /* pass */
        ai = (ai + 1LL);
        _tr_str_release(aty);
        _tr_str_release(al);
    }
    /* pass */
    TrStr retty = _tr_str_lit_len("void", 4LL);
    /* pass */
    if (LModule_is_user_fn(self->m, callee)) {
        /* pass */
        TrStr _strtmp_t3381 = LlvmEmitter_user_ret_ty(self, callee);
        _tr_str_release(retty);
        retty = _strtmp_t3381;
    } else if ((dst >= 0LL)) {
        /* pass */
        TrStr _strtmp_t3382 = LlvmEmitter_vty(self, dst);
        _tr_str_release(retty);
        retty = _strtmp_t3382;
    }
    /* pass */
    if (_tr_str_eqv((retty), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        ({ TrStr _at_t3383 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  call void @", 13LL)), (callee))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arglist)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3383); _tr_str_release(_at_t3383); });
    } else {
        /* pass */
        TrStr r = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3384 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (retty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (arglist)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3384); _tr_str_release(_at_t3384); });
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            LlvmEmitter_store_vreg(self, dst, retty, r);
        }
    }
    _tr_str_release(arglist);
    _tr_str_release(retty);
}

__attribute__((hot)) void LlvmEmitter_emit_term(LlvmEmitter* self, LTerm t) {
    /* pass */
    __auto_type _t3385 = t;
    if (_t3385.tag == LTerm_TRetInt) {
        __auto_type v = _t3385.data.TRetInt.v;
        /* pass */
        if (self->cur_main) {
            /* pass */
            ({ TrStr _at_t3386 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  ret i32 ", 10LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3386); _tr_str_release(_at_t3386); });
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("double", 6LL)))) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit_len("  ret double 0.0\n", 17LL));
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("ptr", 3LL)))) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit_len("  ret ptr null\n", 15LL));
        } else {
            /* pass */
            ({ TrStr _at_t3387 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  ret i64 ", 10LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3387); _tr_str_release(_at_t3387); });
        }
    } else if (_t3385.tag == LTerm_TRetVal) {
        __auto_type v = _t3385.data.TRetVal.v;
        /* pass */
        if (self->cur_main) {
            /* pass */
            TrStr lv = LlvmEmitter_load_vreg_as(self, v, _tr_str_lit_len("i64", 3LL));
            /* pass */
            TrStr tr = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3388 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tr))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = trunc i64 ", 13LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i32\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3388); _tr_str_release(_at_t3388); });
            /* pass */
            ({ TrStr _at_t3389 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ret i32 ", 10LL)), (tr))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3389); _tr_str_release(_at_t3389); });
            _tr_str_release(lv);
            _tr_str_release(tr);
        } else {
            /* pass */
            TrStr lv2 = LlvmEmitter_load_vreg_as(self, v, self->cur_ret);
            /* pass */
            ({ TrStr _at_t3390 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->cur_ret))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lv2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3390); _tr_str_release(_at_t3390); });
            _tr_str_release(lv2);
        }
    } else if (_t3385.tag == LTerm_TRetVoid) {
        /* pass */
        if (self->cur_main) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit_len("  ret i32 0\n", 12LL));
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("double", 6LL)))) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit_len("  ret double 0.0\n", 17LL));
        } else if (_tr_str_eqv((self->cur_ret), (_tr_str_lit_len("ptr", 3LL)))) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit_len("  ret ptr null\n", 15LL));
        } else {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit_len("  ret i64 0\n", 12LL));
        }
    } else if (_t3385.tag == LTerm_TBr) {
        __auto_type target = _t3385.data.TBr.target;
        /* pass */
        ({ TrStr _at_t3391 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(target)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  br label %bb", 14LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3391); _tr_str_release(_at_t3391); });
    } else if (_t3385.tag == LTerm_TCondBr) {
        __auto_type cond = _t3385.data.TCondBr.cond;
__auto_type then_b = _t3385.data.TCondBr.then_b;
__auto_type else_b = _t3385.data.TCondBr.else_b;
        /* pass */
        TrStr lc = LlvmEmitter_load_vreg_as(self, cond, _tr_str_lit_len("i64", 3LL));
        /* pass */
        TrStr c = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3392 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (c))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = icmp ne i64 ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", 0\n", 4LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3392); _tr_str_release(_at_t3392); });
        /* pass */
        ({ TrStr _at_t3393 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (c))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", label %bb", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(then_b)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", label %bb", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(else_b)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3393); _tr_str_release(_at_t3393); });
        _tr_str_release(lc);
        _tr_str_release(c);
    } else if (_t3385.tag == LTerm_TUnset) {
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit_len("  unreachable\n", 14LL));
    }
}

__attribute__((hot)) void LlvmEmitter_emit_function(LlvmEmitter* self, LFunc* lf) {
    /* pass */
    self->tmp = 0LL;
    /* pass */
    LFunc* _cltmp_t3394 = _tr_obj_retain(lf);
    _tr_obj_release(self->cur, _trdrop_LFunc);
    self->cur = _cltmp_t3394;
    /* pass */
    self->cur_main = lf->is_main;
    /* pass */
    if (lf->is_main) {
        /* pass */
        self->cur_ret = _tr_str_lit_len("i32", 3LL);
    } else {
        /* pass */
        self->cur_ret = _ll_ty(LModule_fn_ret_tag(self->m, lf->name));
    }
    /* pass */
    TrStr fname = _tr_str_retain(lf->name);
    /* pass */
    bool main_args = (lf->is_main && (lf->params->len > 0LL));
    /* pass */
    ({ TrStr _at_t3395 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (self->cur_ret))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (fname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3395); _tr_str_release(_at_t3395); });
    /* pass */
    if (main_args) {
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit_len("i32 %argc, ptr %argv", 20LL));
    } else {
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < lf->params->len)) {
            /* pass */
            TrStr pn = List_TrStr_get(lf->params, pi);
            /* pass */
            TrStr pty = _ll_ty(LFunc_var_type(lf, pn));
            /* pass */
            if ((pi > 0LL)) {
                /* pass */
                LlvmEmitter_w(self, _tr_str_lit_len(", ", 2LL));
            }
            /* pass */
            ({ TrStr _at_t3396 = (({ TrStr _cl = (_tr_strx_concatv((pty), (_tr_str_lit_len(" %arg_", 6LL)))); TrStr _cres = _tr_strx_concatv(_cl, (pn)); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3396); _tr_str_release(_at_t3396); });
            /* pass */
            pi = (pi + 1LL);
            _tr_str_release(pn);
            _tr_str_release(pty);
        }
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len(") {\nentry:\n", 11LL));
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < lf->vars->len)) {
        /* pass */
        TrStr vn = List_TrStr_get(lf->vars, vi);
        /* pass */
        long long van = List_i64_get(lf->var_arr, vi);
        /* pass */
        if ((van > 0LL)) {
            /* pass */
            ({ TrStr _at_t3397 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), (vn))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = alloca [", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(van)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" x i64]\n", 8LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3397); _tr_str_release(_at_t3397); });
        } else {
            /* pass */
            ({ TrStr _at_t3398 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), (vn))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_ty(LFunc_var_type(lf, vn))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3398); _tr_str_release(_at_t3398); });
        }
        /* pass */
        vi = (vi + 1LL);
        _tr_str_release(vn);
    }
    /* pass */
    long long ri = 0LL;
    /* pass */
    while ((ri < lf->n_vregs)) {
        /* pass */
        ({ TrStr _at_t3399 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ri)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  %v", 4LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_ty(LFunc_vreg_type(lf, ri))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3399); _tr_str_release(_at_t3399); });
        /* pass */
        ri = (ri + 1LL);
    }
    /* pass */
    if (main_args) {
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit_len("  %argc64 = sext i32 %argc to i64\n", 34LL));
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit_len("  %argl = call ptr @_tr_rt_argv_list(i64 %argc64, ptr %argv)\n", 61LL));
        /* pass */
        ({ TrStr _at_t3400 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(lf->params, 0LL)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  store ptr %argl, ptr %var_", 28LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3400); _tr_str_release(_at_t3400); });
    } else {
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < lf->params->len)) {
            /* pass */
            TrStr pn2 = List_TrStr_get(lf->params, pi);
            /* pass */
            TrStr pty2 = _ll_ty(LFunc_var_type(lf, pn2));
            /* pass */
            ({ TrStr _at_t3401 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (pty2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3401); _tr_str_release(_at_t3401); });
            /* pass */
            pi = (pi + 1LL);
            _tr_str_release(pn2);
            _tr_str_release(pty2);
        }
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("  br label %bb0\n", 16LL));
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < lf->blocks->len)) {
        /* pass */
        LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
        /* pass */
        ({ TrStr _at_t3402 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("bb", 2LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(":\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3402); _tr_str_release(_at_t3402); });
        /* pass */
        long long ii = 0LL;
        /* pass */
        while ((ii < blk->insts->len)) {
            /* pass */
            LlvmEmitter_emit_inst(self, (*((LInst*)List_ptr_get(blk->insts, ii))));
            /* pass */
            ii = (ii + 1LL);
        }
        /* pass */
        LlvmEmitter_emit_term(self, blk->term);
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("}\n\n", 3LL));
    _tr_str_release(fname);
}

__attribute__((hot)) void LlvmEmitter_emit_extern_decls(LlvmEmitter* self) {
    /* pass */
    TrMap* seen = _tr_dict_new(64LL);
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < self->m->funcs->len)) {
        /* pass */
        LFunc* lf = ((LFunc*)List_ptr_get(self->m->funcs, fi));
        /* pass */
        LFunc* _cltmp_t3403 = _tr_obj_retain(lf);
        _tr_obj_release(self->cur, _trdrop_LFunc);
        self->cur = _cltmp_t3403;
        /* pass */
        long long bi = 0LL;
        /* pass */
        while ((bi < lf->blocks->len)) {
            /* pass */
            LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
            /* pass */
            long long ii = 0LL;
            /* pass */
            while ((ii < blk->insts->len)) {
                /* pass */
                LlvmEmitter_scan_call_decl(self, (*((LInst*)List_ptr_get(blk->insts, ii))), seen);
                /* pass */
                ii = (ii + 1LL);
            }
            /* pass */
            bi = (bi + 1LL);
        }
        /* pass */
        fi = (fi + 1LL);
    }
}

__attribute__((hot)) void LlvmEmitter_scan_call_decl(LlvmEmitter* self, LInst inst, TrMap* seen) {
    /* pass */
    __auto_type _t3404 = inst;
    if (_t3404.tag == LInst_ICall) {
        __auto_type dst = _t3404.data.ICall.dst;
__auto_type callee = _t3404.data.ICall.callee;
__auto_type args = _t3404.data.ICall.args;
        /* pass */
        if (LModule_is_user_fn(self->m, callee)) {
            /* pass */
            return;
        }
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        TrStr retty = _tr_str_lit_len("void", 4LL);
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3405 = LlvmEmitter_vty(self, dst);
            _tr_str_release(retty);
            retty = _strtmp_t3405;
        }
        /* pass */
        TrStr params = _tr_str_lit_len("", 0LL);
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < args->len)) {
            /* pass */
            if ((ai > 0LL)) {
                /* pass */
                TrStr _strtmp_t3406 = _tr_strx_concatv((params), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(params);
                params = _strtmp_t3406;
            }
            /* pass */
            TrStr _strtmp_t3407 = ({ TrStr _cr = (LlvmEmitter_vty(self, List_i64_get(args, ai))); TrStr _cres = _tr_strx_concatv((params), _cr); _tr_str_release(_cr); _cres; });
            _tr_str_release(params);
            params = _strtmp_t3407;
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        TrStr _na = _tr_str_lit_len("", 0LL);
        /* pass */
        if ((_tr_str_eqv((retty), (_tr_str_lit_len("ptr", 3LL))) && _is_fresh_alloc_ret(callee))) {
            /* pass */
            TrStr _strtmp_t3408 = _tr_str_lit_len("noalias ", 8LL);
            _tr_str_release(_na);
            _na = _strtmp_t3408;
        }
        /* pass */
        ({ TrStr _at_t3409 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("declare ", 8LL)), (_na))); TrStr _cres = _tr_strx_concatv(_cl, (retty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (params)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3409); _tr_str_release(_at_t3409); });
        _tr_str_release(retty);
        _tr_str_release(params);
        _tr_str_release(_na);
    } else if (_t3404.tag == LInst_IFCall1) {
        __auto_type dst = _t3404.data.IFCall1.dst;
__auto_type callee = _t3404.data.IFCall1.callee;
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        TrStr rt1 = _tr_str_lit_len("void", 4LL);
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3410 = LlvmEmitter_vty(self, dst);
            _tr_str_release(rt1);
            rt1 = _strtmp_t3410;
        }
        /* pass */
        ({ TrStr _at_t3411 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("declare ", 8LL)), (rt1))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(double)\n", 9LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3411); _tr_str_release(_at_t3411); });
        _tr_str_release(rt1);
    } else if (_t3404.tag == LInst_IFCallF) {
        __auto_type callee = _t3404.data.IFCallF.callee;
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        ({ TrStr _at_t3412 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("declare double @", 16LL)), (callee))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(double)\n", 9LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3412); _tr_str_release(_at_t3412); });
    } else if (_t3404.tag == LInst_IFCall2F) {
        __auto_type callee = _t3404.data.IFCall2F.callee;
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        ({ TrStr _at_t3413 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("declare double @", 16LL)), (callee))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(double, double)\n", 17LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3413); _tr_str_release(_at_t3413); });
    } else if (1) {
        __auto_type _ = _t3404;
        /* pass */
        /* pass */
    }
}

__attribute__((hot)) TrStr LlvmEmitter_emit_module(LlvmEmitter* self) {
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("; Tauraro LLVM IR — backend A (taumir LIR -> LLVM IR -> llc)\n\n", 64LL));
    /* pass */
    long long si = 0LL;
    /* pass */
    while ((si < self->m->strings->len)) {
        /* pass */
        TrStr s = List_TrStr_get(self->m->strings, si);
        /* pass */
        long long blen = (List_i64_get(self->m->string_lens, si) + 1LL);
        /* pass */
        ({ TrStr _at_t3414 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(si)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("@.str.", 6LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = private unnamed_addr constant [", 34LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(blen)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" x i8] c\"", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_str_escape(s, List_i64_get(self->m->string_lens, si))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\\00\"\n", 5LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3414); _tr_str_release(_at_t3414); });
        /* pass */
        si = (si + 1LL);
        _tr_str_release(s);
    }
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < self->m->globals->len)) {
        /* pass */
        long long gan = List_i64_get(self->m->global_arr, gi);
        /* pass */
        if ((gan > 0LL)) {
            /* pass */
            ({ TrStr _at_t3415 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gi)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("@g.", 3LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = internal global [", 20LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gan)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" x i64] zeroinitializer\n", 24LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3415); _tr_str_release(_at_t3415); });
        } else {
            /* pass */
            TrStr gty = _ll_ty(List_i64_get(self->m->global_types, gi));
            /* pass */
            TrStr init = _tr_str_lit_len("0", 1LL);
            /* pass */
            if (_tr_str_eqv((gty), (_tr_str_lit_len("double", 6LL)))) {
                /* pass */
                TrStr _strtmp_t3416 = _tr_str_lit_len("0.0", 3LL);
                _tr_str_release(init);
                init = _strtmp_t3416;
            }
            /* pass */
            if (_tr_str_eqv((gty), (_tr_str_lit_len("ptr", 3LL)))) {
                /* pass */
                TrStr _strtmp_t3417 = _tr_str_lit_len("null", 4LL);
                _tr_str_release(init);
                init = _strtmp_t3417;
            }
            /* pass */
            ({ TrStr _at_t3418 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gi)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("@g.", 3LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = internal global ", 19LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (init)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3418); _tr_str_release(_at_t3418); });
            _tr_str_release(gty);
            _tr_str_release(init);
        }
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("\n", 1LL));
    /* pass */
    LlvmEmitter_emit_extern_decls(self);
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("declare ptr @_tr_rt_argv_list(i64, ptr)\n", 40LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("\n", 1LL));
    /* pass */
    List_TrStr* emitted = (void*)List_TrStr_new();
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < self->m->funcs->len)) {
        /* pass */
        LFunc* efn = ((LFunc*)List_ptr_get(self->m->funcs, fi));
        /* pass */
        bool dup = false;
        /* pass */
        long long ei = 0LL;
        /* pass */
        while ((ei < emitted->len)) {
            /* pass */
            if (_tr_str_eqv((List_TrStr_get(emitted, ei)), (efn->name))) {
                /* pass */
                dup = true;
                /* pass */
                ei = emitted->len;
            } else {
                /* pass */
                ei = (ei + 1LL);
            }
        }
        /* pass */
        if ((!dup)) {
            /* pass */
            List_TrStr_append(emitted, efn->name);
            /* pass */
            LlvmEmitter_emit_function(self, efn);
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("\n", 1LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("!0 = !{!\"tauraro_tbaa_root\"}\n", 29LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("!1 = !{!\"listhdr\", !0, i64 0}\n", 30LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("!2 = !{!\"listelem\", !0, i64 0}\n", 31LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("!3 = !{!\"objfield\", !0, i64 0}\n", 31LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("!4 = !{!1, !1, i64 0}\n", 22LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("!5 = !{!2, !2, i64 0}\n", 22LL));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit_len("!6 = !{!3, !3, i64 0}\n", 22LL));
    /* pass */
    List_TrStr_free(emitted);
    return StringObj_as_str(StringBuilder_to_string(self->out));
}

__attribute__((hot)) TrStr _tbaa_suffix(long long aclass) {
    /* pass */
    if ((aclass == 1LL)) {
        /* pass */
        return _tr_str_lit_len(", !tbaa !4", 10LL);
    }
    /* pass */
    if ((aclass == 2LL)) {
        /* pass */
        return _tr_str_lit_len(", !tbaa !5", 10LL);
    }
    /* pass */
    if ((aclass == 3LL)) {
        /* pass */
        return _tr_str_lit_len(", !tbaa !6", 10LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _is_fresh_alloc_ret(TrStr name) {
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("malloc", 6LL))) || _tr_str_eqv((name), (_tr_str_lit_len("calloc", 6LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("_tr_c_malloc", 12LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_c_calloc", 12LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("_tr_checked_alloc", 17LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_raw_alloc", 16LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_list_new", 15LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_argv_list", 16LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_blist_new", 16LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_idict_new", 16LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_sdict_new", 16LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_dict_new", 12LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_idict_new", 13LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_set_new", 14LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_set_new", 11LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_iset_new", 12LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_pset_new", 12LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_sset_new", 12LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_obj_alloc", 16LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_obj_alloc", 13LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((_tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_str_new", 14LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_str_new", 11LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_rt_str_alloc", 16LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_char_to_str_alloc", 21LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_eqv((name), (_tr_str_lit_len("_tr_bytes_new", 13LL))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_range_new", 13LL)))) || _tr_str_eqv((name), (_tr_str_lit_len("_tr_shared_new", 14LL))))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _ll_int_instr(TrStr op) {
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("add", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("sub", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("mul", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("sdiv", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("//", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("sdiv", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("srem", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("and", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("|", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("or", 2LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("^", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("xor", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<<", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("shl", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">>", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("ashr", 4LL);
    }
    /* pass */
    return _tr_str_lit_len("add", 3LL);
}

__attribute__((hot)) TrStr _ll_icmp_pred(TrStr op) {
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("eq", 2LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("ne", 2LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("slt", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("sle", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("sgt", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("sge", 3LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _ll_float_instr(TrStr op) {
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("fadd", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("fsub", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("fmul", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("fdiv", 4LL);
    }
    /* pass */
    return _tr_str_lit_len("fadd", 4LL);
}

__attribute__((hot)) TrStr _ll_fcmp_pred(TrStr op) {
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("oeq", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("one", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("olt", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("ole", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
        /* pass */
        return _tr_str_lit_len("ogt", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
        /* pass */
        return _tr_str_lit_len("oge", 3LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

