#include "tauraro_types.h"

long long _argreg_modrm(long long idx);
long long _round16(long long n);
long long _vreg_disp(long long id);
long long _var_disp(LFunc* lf, TrStr name);
void _st_rax(ByteBuf* c, long long disp);
void _ld_rax(ByteBuf* c, long long disp);
void _ld_rcx(ByteBuf* c, long long disp);
void _ld_argreg(ByteBuf* c, long long idx, long long disp);
void _mov_rax_imm64(ByteBuf* c, long long v);
void _emit_binop(ByteBuf* c, TrStr op);

__attribute__((malloc,returns_nonnull,hot)) EncodedFunc* EncodedFunc_init(TrStr name) {
    /* pass */
    EncodedFunc* e = ((EncodedFunc*)_tr_obj_alloc(sizeof(EncodedFunc)));
    /* pass */
    e->name = _tr_str_retain(name);
    /* pass */
    e->is_main = false;
    /* pass */
    ByteBuf* _cltmp_t2242 = ByteBuf_init();
    _tr_obj_release(e->code, _trdrop_ByteBuf);
    e->code = _cltmp_t2242;
    /* pass */
    e->relocs = (void*)List_ptr_new();
    /* pass */
    return e;
}

__attribute__((hot)) long long _argreg_modrm(long long idx) {
    /* pass */
    if ((idx == 0LL)) {
        /* pass */
        return 189LL;
    }
    /* pass */
    if ((idx == 1LL)) {
        /* pass */
        return 181LL;
    }
    /* pass */
    if ((idx == 2LL)) {
        /* pass */
        return 149LL;
    }
    /* pass */
    if ((idx == 3LL)) {
        /* pass */
        return 141LL;
    }
    /* pass */
    return 189LL;
}

__attribute__((hot)) long long _round16(long long n) {
    /* pass */
    long long r = n;
    /* pass */
    while (((r % 16LL) != 0LL)) {
        /* pass */
        r = (r + 1LL);
    }
    /* pass */
    return r;
}

__attribute__((hot)) long long _vreg_disp(long long id) {
    /* pass */
    return (0LL - (8LL * (id + 1LL)));
}

__attribute__((hot)) long long _var_disp(LFunc* lf, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(lf, name);
    /* pass */
    if ((idx < 0LL)) {
        /* pass */
        idx = 0LL;
    }
    /* pass */
    return (0LL - (8LL * ((lf->n_vregs + idx) + 1LL)));
}

__attribute__((hot)) void _st_rax(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 137LL);
    /* pass */
    ByteBuf_u8(c, 133LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_rax(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 139LL);
    /* pass */
    ByteBuf_u8(c, 133LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_rcx(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 139LL);
    /* pass */
    ByteBuf_u8(c, 141LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_argreg(ByteBuf* c, long long idx, long long disp) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 139LL);
    /* pass */
    ByteBuf_u8(c, _argreg_modrm(idx));
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _mov_rax_imm64(ByteBuf* c, long long v) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 184LL);
    /* pass */
    ByteBuf_u64(c, v);
}

__attribute__((hot)) void _emit_binop(ByteBuf* c, TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 1LL);
        /* pass */
        ByteBuf_u8(c, 200LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 41LL);
        /* pass */
        ByteBuf_u8(c, 200LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 15LL);
        /* pass */
        ByteBuf_u8(c, 175LL);
        /* pass */
        ByteBuf_u8(c, 193LL);
    }
}

__attribute__((hot)) EncodedFunc* encode_func(LFunc* lf) {
    /* pass */
    EncodedFunc* e = EncodedFunc_init(lf->name);
    /* pass */
    e->is_main = lf->is_main;
    /* pass */
    ByteBuf* c = _tr_obj_retain(e->code);
    /* pass */
    long long framesize = _round16(((lf->n_vregs + lf->vars->len) * 8LL));
    /* pass */
    ByteBuf_u8(c, 85LL);
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 137LL);
    /* pass */
    ByteBuf_u8(c, 229LL);
    /* pass */
    if ((framesize > 0LL)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 129LL);
        /* pass */
        ByteBuf_u8(c, 236LL);
        /* pass */
        ByteBuf_u32(c, framesize);
    }
    /* pass */
    long long ii = 0LL;
    /* pass */
    while ((ii < lf->block->insts->len)) {
        /* pass */
        __auto_type _t2243 = (*((LInst*)List_ptr_get(lf->block->insts, ii)));
        if (_t2243.tag == LInst_IConst) {
            __auto_type dst = _t2243.data.IConst.dst;
__auto_type v = _t2243.data.IConst.v;
            /* pass */
            _mov_rax_imm64(c, v);
            /* pass */
            _st_rax(c, _vreg_disp(dst));
        } else if (_t2243.tag == LInst_ILoadVar) {
            __auto_type dst = _t2243.data.ILoadVar.dst;
__auto_type name = _t2243.data.ILoadVar.name;
            /* pass */
            _ld_rax(c, _var_disp(lf, name));
            /* pass */
            _st_rax(c, _vreg_disp(dst));
        } else if (_t2243.tag == LInst_IStoreVar) {
            __auto_type name = _t2243.data.IStoreVar.name;
__auto_type src = _t2243.data.IStoreVar.src;
            /* pass */
            _ld_rax(c, _vreg_disp(src));
            /* pass */
            _st_rax(c, _var_disp(lf, name));
        } else if (_t2243.tag == LInst_IBinOp) {
            __auto_type dst = _t2243.data.IBinOp.dst;
__auto_type op = _t2243.data.IBinOp.op;
__auto_type a = _t2243.data.IBinOp.a;
__auto_type b = _t2243.data.IBinOp.b;
            /* pass */
            _ld_rax(c, _vreg_disp(a));
            /* pass */
            _ld_rcx(c, _vreg_disp(b));
            /* pass */
            _emit_binop(c, op);
            /* pass */
            _st_rax(c, _vreg_disp(dst));
        } else if (_t2243.tag == LInst_ICall) {
            __auto_type dst = _t2243.data.ICall.dst;
__auto_type callee = _t2243.data.ICall.callee;
__auto_type args = _t2243.data.ICall.args;
            /* pass */
            long long ai = 0LL;
            /* pass */
            while ((ai < args->len)) {
                /* pass */
                _ld_argreg(c, ai, _vreg_disp(List_i64_get(args, ai)));
                /* pass */
                ai = (ai + 1LL);
            }
            /* pass */
            ByteBuf_u8(c, 232LL);
            /* pass */
            Reloc* r = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
            /* pass */
            r->offset = c->len;
            /* pass */
            r->symbol = _tr_str_retain(callee);
            /* pass */
            List_ptr_append(e->relocs, _tr_obj_retain(r));
            /* pass */
            ByteBuf_u32(c, 0LL);
            /* pass */
            if ((dst >= 0LL)) {
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            }
            _tr_obj_release(r, _trdrop_Reloc);
        }
        /* pass */
        ii = (ii + 1LL);
    }
    /* pass */
    __auto_type _t2244 = lf->block->term;
    if (_t2244.tag == LTerm_TRetInt) {
        __auto_type rv = _t2244.data.TRetInt.v;
        /* pass */
        if ((rv == 0LL)) {
            /* pass */
            ByteBuf_u8(c, 49LL);
            /* pass */
            ByteBuf_u8(c, 192LL);
        } else {
            /* pass */
            ByteBuf_u8(c, 184LL);
            /* pass */
            ByteBuf_u32(c, rv);
        }
    } else if (1) {
        __auto_type _ = _t2244;
        /* pass */
        ByteBuf_u8(c, 49LL);
        /* pass */
        ByteBuf_u8(c, 192LL);
    }
    /* pass */
    ByteBuf_u8(c, 201LL);
    /* pass */
    ByteBuf_u8(c, 195LL);
    /* pass */
    _tr_obj_release(c, _trdrop_ByteBuf);
    return e;
}

