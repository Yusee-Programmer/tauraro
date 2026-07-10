#include "tauraro_types.h"

long long _arg_opcode_movimm(long long idx);

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

__attribute__((hot)) long long _arg_opcode_movimm(long long idx) {
    /* pass */
    if ((idx == 0LL)) {
        /* pass */
        return 191LL;
    }
    /* pass */
    if ((idx == 1LL)) {
        /* pass */
        return 190LL;
    }
    /* pass */
    if ((idx == 2LL)) {
        /* pass */
        return 186LL;
    }
    /* pass */
    if ((idx == 3LL)) {
        /* pass */
        return 185LL;
    }
    /* pass */
    return 191LL;
}

__attribute__((hot)) EncodedFunc* encode_func(LFunc* lf) {
    /* pass */
    EncodedFunc* e = EncodedFunc_init(lf->name);
    /* pass */
    e->is_main = lf->is_main;
    /* pass */
    ByteBuf* c = _tr_obj_retain(e->code);
    /* pass */
    ByteBuf_u8(c, 85LL);
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 137LL);
    /* pass */
    ByteBuf_u8(c, 229LL);
    /* pass */
    long long ii = 0LL;
    /* pass */
    while ((ii < lf->block->insts->len)) {
        /* pass */
        __auto_type _t2243 = (*((LInst*)List_ptr_get(lf->block->insts, ii)));
        if (_t2243.tag == LInst_ICall) {
            __auto_type callee = _t2243.data.ICall.callee;
__auto_type args = _t2243.data.ICall.args;
            /* pass */
            long long ai = 0LL;
            /* pass */
            while ((ai < args->len)) {
                /* pass */
                __auto_type _t2244 = (*((LVal*)List_ptr_get(args, ai)));
                if (_t2244.tag == LVal_VConst) {
                    __auto_type v = _t2244.data.VConst.v;
                    /* pass */
                    ByteBuf_u8(c, _arg_opcode_movimm(ai));
                    /* pass */
                    ByteBuf_u32(c, v);
                } else if (1) {
                    __auto_type _ = _t2244;
                    /* pass */
                    /* pass */
                }
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
            _tr_obj_release(r, _trdrop_Reloc);
        }
        /* pass */
        ii = (ii + 1LL);
    }
    /* pass */
    __auto_type _t2245 = lf->block->term;
    if (_t2245.tag == LTerm_TRetInt) {
        __auto_type rv = _t2245.data.TRetInt.v;
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
        __auto_type _ = _t2245;
        /* pass */
        ByteBuf_u8(c, 49LL);
        /* pass */
        ByteBuf_u8(c, 192LL);
    }
    /* pass */
    ByteBuf_u8(c, 93LL);
    /* pass */
    ByteBuf_u8(c, 195LL);
    /* pass */
    _tr_obj_release(c, _trdrop_ByteBuf);
    return e;
}

