#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) HirFStringPart* HirFStringPart_init() {
    /* pass */
    HirFStringPart* p = ((HirFStringPart*)_tr_obj_alloc(sizeof(HirFStringPart)));
    /* pass */
    p->is_expr = false;
    /* pass */
    p->text = _tr_str_lit("");
    /* pass */
    p->expr = (HirExpr*)(0LL);
    /* pass */
    p->fmt_spec = _tr_str_lit("");
    /* pass */
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) HirMatchArm* HirMatchArm_init(Pattern pat, HirBlock* body) {
    /* pass */
    HirMatchArm* a = ((HirMatchArm*)_tr_obj_alloc(sizeof(HirMatchArm)));
    /* pass */
    a->pat = pat;
    /* pass */
    HirBlock* _cltmp_t239 = _tr_obj_retain(body);
    _tr_obj_release(a->body, _trdrop_HirBlock);
    a->body = _cltmp_t239;
    /* pass */
    a->guard = (HirExpr*)(0LL);
    /* pass */
    return a;
}

__attribute__((malloc,returns_nonnull,hot)) HirBlock* HirBlock_init() {
    /* pass */
    HirBlock* b = ((HirBlock*)_tr_obj_alloc(sizeof(HirBlock)));
    /* pass */
    b->stmts = (void*)List_ptr_new();
    /* pass */
    return b;
}

__attribute__((hot)) void HirBlock_push(HirBlock* self, HirStmt* s) {
    /* pass */
    List_ptr_append(self->stmts, s);
}

__attribute__((malloc,returns_nonnull,hot)) HirProgram* HirProgram_init() {
    /* pass */
    HirProgram* p = ((HirProgram*)_tr_obj_alloc(sizeof(HirProgram)));
    /* pass */
    p->functions = (void*)List_ptr_new();
    /* pass */
    p->classes = (void*)List_ptr_new();
    /* pass */
    p->enums = (void*)List_ptr_new();
    /* pass */
    p->interfaces = (void*)List_ptr_new();
    /* pass */
    p->top_level_stmts = (void*)List_ptr_new();
    /* pass */
    p->extern_funcs = (void*)List_ptr_new();
    /* pass */
    p->decorator_defs = (void*)List_ptr_new();
    /* pass */
    p->type_alias_names = (void*)List_TrStr_new();
    /* pass */
    p->type_alias_types = (void*)List_ptr_new();
    /* pass */
    return p;
}

__attribute__((hot)) HirExpr* box_hirexpr(HirExpr e) {
    /* pass */
    /* unsafe block */
    /* pass */
    HirExpr* p = ((HirExpr*)_tr_c_calloc((size_t)(1LL), sizeof(HirExpr)));
    /* pass */
    (*p = e);
    /* pass */
    return p;
}

__attribute__((hot)) HirStmt* box_hirstmt(HirStmt s) {
    /* pass */
    /* unsafe block */
    /* pass */
    HirStmt* p = ((HirStmt*)_tr_c_calloc((size_t)(1LL), sizeof(HirStmt)));
    /* pass */
    (*p = s);
    /* pass */
    return p;
}

__attribute__((hot)) AstType* hir_expr_type(HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return AstType_init(_tr_str_lit("void"));
    }
    /* pass */
    __auto_type _t240 = (*e);
    if (_t240.tag == HirExpr_ELitInt) {
        __auto_type ty = _t240.data.ELitInt.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ELitFloat) {
        __auto_type ty = _t240.data.ELitFloat.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ELitStr) {
        __auto_type ty = _t240.data.ELitStr.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ELitBytes) {
        __auto_type ty = _t240.data.ELitBytes.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ERawStr) {
        __auto_type ty = _t240.data.ERawStr.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ELitChar) {
        __auto_type ty = _t240.data.ELitChar.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ELitBool) {
        __auto_type ty = _t240.data.ELitBool.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ELitNone) {
        __auto_type ty = _t240.data.ELitNone.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EIdent) {
        __auto_type ty = _t240.data.EIdent.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EBinOp) {
        __auto_type ty = _t240.data.EBinOp.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EUnaryOp) {
        __auto_type ty = _t240.data.EUnaryOp.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ECall) {
        __auto_type ty = _t240.data.ECall.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EMethodCall) {
        __auto_type ty = _t240.data.EMethodCall.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EPropAccess) {
        __auto_type ty = _t240.data.EPropAccess.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EIndex) {
        __auto_type ty = _t240.data.EIndex.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ECast) {
        __auto_type ty = _t240.data.ECast.target_ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EFString) {
        __auto_type ty = _t240.data.EFString.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ETryExpr) {
        __auto_type ty = _t240.data.ETryExpr.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EClosure) {
        __auto_type _cl_ps = _t240.data.EClosure.params;
__auto_type _cl_ret = _t240.data.EClosure.ret_ty;
        /* pass */
        AstType* _clt = AstType_init(_tr_str_lit("def"));
        /* pass */
        long long _cli = 0LL;
        /* pass */
        while ((_cli < _cl_ps->len)) {
            /* pass */
            /* unsafe block */
            /* pass */
            AstType** _clp = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
            /* pass */
            (*_clp = ((HirParam*)List_ptr_get(_cl_ps, _cli))->ty);
            /* pass */
            List_ptr_append(_clt->args, _clp);
            /* pass */
            _cli = (_cli + 1LL);
        }
        /* pass */
        /* unsafe block */
        /* pass */
        AstType** _clrp = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
        /* pass */
        (*_clrp = _cl_ret);
        /* pass */
        List_ptr_append(_clt->args, _clrp);
        /* pass */
        return _clt;
    } else if (_t240.tag == HirExpr_ESuperMethodCall) {
        __auto_type ty = _t240.data.ESuperMethodCall.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ESuperPropAccess) {
        __auto_type ty = _t240.data.ESuperPropAccess.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EList) {
        __auto_type ty = _t240.data.EList.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ESet) {
        __auto_type ty = _t240.data.ESet.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EDict) {
        __auto_type ty = _t240.data.EDict.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ETuple) {
        __auto_type ty = _t240.data.ETuple.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EListComp) {
        __auto_type ty = _t240.data.EListComp.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EGeneratorExpr) {
        __auto_type ty = _t240.data.EGeneratorExpr.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ESlice) {
        __auto_type ty = _t240.data.ESlice.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EAwait) {
        __auto_type ty = _t240.data.EAwait.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EAwaitTimeout) {
        __auto_type ty = _t240.data.EAwaitTimeout.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EYield) {
        __auto_type ty = _t240.data.EYield.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ETry) {
        __auto_type ty = _t240.data.ETry.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ERange) {
        __auto_type ty = _t240.data.ERange.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ESizeOf) {
        __auto_type ty = _t240.data.ESizeOf.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EIfElse) {
        __auto_type ty = _t240.data.EIfElse.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EDo) {
        __auto_type ty = _t240.data.EDo.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EMatchExpr) {
        __auto_type ty = _t240.data.EMatchExpr.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_ELoop) {
        __auto_type ty = _t240.data.ELoop.ty;
        return _tr_obj_retain(ty);
    } else if (_t240.tag == HirExpr_EWhileExpr) {
        __auto_type ty = _t240.data.EWhileExpr.ty;
        return _tr_obj_retain(ty);
    } else if (1) {
        __auto_type _ = _t240;
        /* pass */
        /* pass */
    }
    /* pass */
    return AstType_init(_tr_str_lit("void"));
}

__attribute__((hot)) long long _tr_str_len(TrStr s) {
    /* pass */
    /* unsafe block */
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((((long long)((*(p + i)))) != 0LL)) {
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return i;
}

