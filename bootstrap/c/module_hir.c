#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) HirFStringPart* HirFStringPart_init() {
#line 80 "src/hir.tr"
    HirFStringPart* p = ((HirFStringPart*)_tr_obj_alloc(sizeof(HirFStringPart)));
#line 81 "src/hir.tr"
    p->is_expr = false;
#line 82 "src/hir.tr"
    TrStr _sfo_t656 = p->text;
    p->text = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t656);
#line 83 "src/hir.tr"
    p->text_len = 0LL;
#line 84 "src/hir.tr"
    p->expr = (HirExpr*)(0LL);
#line 85 "src/hir.tr"
    TrStr _sfo_t657 = p->fmt_spec;
    p->fmt_spec = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t657);
#line 86 "src/hir.tr"
    p->spec_args = (void*)List_ptr_new();
#line 87 "src/hir.tr"
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) HirMatchArm* HirMatchArm_init(Pattern pat, HirBlock* body) {
#line 98 "src/hir.tr"
    HirMatchArm* a = ((HirMatchArm*)_tr_obj_alloc(sizeof(HirMatchArm)));
#line 99 "src/hir.tr"
    a->pat = pat;
#line 100 "src/hir.tr"
    HirBlock* _cltmp_t658 = _tr_obj_retain(body);
    _tr_obj_release(a->body, _trdrop_HirBlock);
    a->body = _cltmp_t658;
#line 101 "src/hir.tr"
    a->guard = (HirExpr*)(0LL);
#line 102 "src/hir.tr"
    return a;
}

__attribute__((malloc,returns_nonnull,hot)) HirBlock* HirBlock_init() {
#line 152 "src/hir.tr"
    HirBlock* b = ((HirBlock*)_tr_obj_alloc(sizeof(HirBlock)));
#line 153 "src/hir.tr"
    b->stmts = (void*)List_ptr_new();
#line 154 "src/hir.tr"
    return b;
}

__attribute__((hot)) void HirBlock_push(HirBlock* self, HirStmt* s) {
#line 157 "src/hir.tr"
    List_ptr_append(self->stmts, s);
}

__attribute__((malloc,returns_nonnull,hot)) HirProgram* HirProgram_init() {
#line 247 "src/hir.tr"
    HirProgram* p = ((HirProgram*)_tr_obj_alloc(sizeof(HirProgram)));
#line 248 "src/hir.tr"
    p->functions = (void*)List_ptr_new();
#line 249 "src/hir.tr"
    p->classes = (void*)List_ptr_new();
#line 250 "src/hir.tr"
    p->enums = (void*)List_ptr_new();
#line 251 "src/hir.tr"
    p->interfaces = (void*)List_ptr_new();
#line 252 "src/hir.tr"
    p->top_level_stmts = (void*)List_ptr_new();
#line 253 "src/hir.tr"
    p->extern_funcs = (void*)List_ptr_new();
#line 254 "src/hir.tr"
    p->decorator_defs = (void*)List_ptr_new();
#line 255 "src/hir.tr"
    p->type_alias_names = (void*)List_TrStr_new();
#line 256 "src/hir.tr"
    p->type_alias_types = (void*)List_ptr_new();
#line 257 "src/hir.tr"
    return p;
}

__attribute__((hot)) HirExpr* box_hirexpr(HirExpr e) {
#line 262 "src/hir.tr"
    /* unsafe block */
#line 263 "src/hir.tr"
    HirExpr* p = ((HirExpr*)_tr_c_calloc((size_t)(1LL), sizeof(HirExpr)));
#line 264 "src/hir.tr"
    (*p = e);
#line 265 "src/hir.tr"
    return p;
}

__attribute__((hot)) HirStmt* box_hirstmt(HirStmt s) {
#line 268 "src/hir.tr"
    /* unsafe block */
#line 269 "src/hir.tr"
    HirStmt* p = ((HirStmt*)_tr_c_calloc((size_t)(1LL), sizeof(HirStmt)));
#line 270 "src/hir.tr"
    (*p = s);
#line 271 "src/hir.tr"
    return p;
}

__attribute__((hot)) AstType* hir_expr_type(HirExpr* e) {
#line 274 "src/hir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 274 "src/hir.tr"
        return AstType_init(_tr_str_lit_len("void", 4LL));
    }
#line 275 "src/hir.tr"
    __auto_type _t659 = (*e);
    if (_t659.tag == HirExpr_ELitInt) {
        __auto_type ty = _t659.data.ELitInt.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ELitFloat) {
        __auto_type ty = _t659.data.ELitFloat.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ELitStr) {
        __auto_type ty = _t659.data.ELitStr.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ELitBytes) {
        __auto_type ty = _t659.data.ELitBytes.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ERawStr) {
        __auto_type ty = _t659.data.ERawStr.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ELitChar) {
        __auto_type ty = _t659.data.ELitChar.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ELitBool) {
        __auto_type ty = _t659.data.ELitBool.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ELitNone) {
        __auto_type ty = _t659.data.ELitNone.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EIdent) {
        __auto_type ty = _t659.data.EIdent.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EBinOp) {
        __auto_type ty = _t659.data.EBinOp.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EUnaryOp) {
        __auto_type ty = _t659.data.EUnaryOp.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ECall) {
        __auto_type ty = _t659.data.ECall.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EMethodCall) {
        __auto_type ty = _t659.data.EMethodCall.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EPropAccess) {
        __auto_type ty = _t659.data.EPropAccess.ty;
#line 293 "src/hir.tr"
        if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("Bits", 4LL))) && (ty->args->len > 0LL))) {
#line 293 "src/hir.tr"
            return (*((AstType**)List_ptr_get(ty->args, 0LL)));
        }
#line 294 "src/hir.tr"
        return ty;
    } else if (_t659.tag == HirExpr_EIndex) {
        __auto_type ty = _t659.data.EIndex.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ECast) {
        __auto_type ty = _t659.data.ECast.target_ty;
        return ty;
    } else if (_t659.tag == HirExpr_EFString) {
        __auto_type ty = _t659.data.EFString.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ETryExpr) {
        __auto_type ty = _t659.data.ETryExpr.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EClosure) {
        __auto_type _cl_ps = _t659.data.EClosure.params;
__auto_type _cl_ret = _t659.data.EClosure.ret_ty;
#line 303 "src/hir.tr"
        AstType* _clt = AstType_init(_tr_str_lit_len("def", 3LL));
#line 304 "src/hir.tr"
        long long _cli = 0LL;
#line 305 "src/hir.tr"
        while ((_cli < _cl_ps->len)) {
#line 306 "src/hir.tr"
            /* unsafe block */
#line 307 "src/hir.tr"
            AstType** _clp = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
#line 308 "src/hir.tr"
            (*_clp = ((HirParam*)List_ptr_get(_cl_ps, _cli))->ty);
#line 309 "src/hir.tr"
            List_ptr_append(_clt->args, _clp);
#line 310 "src/hir.tr"
            _cli = (_cli + 1LL);
        }
#line 311 "src/hir.tr"
        /* unsafe block */
#line 312 "src/hir.tr"
        AstType** _clrp = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
#line 313 "src/hir.tr"
        (*_clrp = _cl_ret);
#line 314 "src/hir.tr"
        List_ptr_append(_clt->args, _clrp);
#line 315 "src/hir.tr"
        return _clt;
    } else if (_t659.tag == HirExpr_ESuperMethodCall) {
        __auto_type ty = _t659.data.ESuperMethodCall.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ESuperPropAccess) {
        __auto_type ty = _t659.data.ESuperPropAccess.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EList) {
        __auto_type ty = _t659.data.EList.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ESet) {
        __auto_type ty = _t659.data.ESet.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EDict) {
        __auto_type ty = _t659.data.EDict.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ETuple) {
        __auto_type ty = _t659.data.ETuple.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EListComp) {
        __auto_type ty = _t659.data.EListComp.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EGeneratorExpr) {
        __auto_type ty = _t659.data.EGeneratorExpr.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ESlice) {
        __auto_type ty = _t659.data.ESlice.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EAwait) {
        __auto_type ty = _t659.data.EAwait.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EAwaitTimeout) {
        __auto_type ty = _t659.data.EAwaitTimeout.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EYield) {
        __auto_type ty = _t659.data.EYield.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ETry) {
        __auto_type ty = _t659.data.ETry.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ERange) {
        __auto_type ty = _t659.data.ERange.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ESizeOf) {
        __auto_type ty = _t659.data.ESizeOf.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EIfElse) {
        __auto_type ty = _t659.data.EIfElse.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EDo) {
        __auto_type ty = _t659.data.EDo.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EMatchExpr) {
        __auto_type ty = _t659.data.EMatchExpr.ty;
        return ty;
    } else if (_t659.tag == HirExpr_ELoop) {
        __auto_type ty = _t659.data.ELoop.ty;
        return ty;
    } else if (_t659.tag == HirExpr_EWhileExpr) {
        __auto_type ty = _t659.data.EWhileExpr.ty;
        return ty;
    } else if (1) {
        __auto_type _ = _t659;
#line 337 "src/hir.tr"
        /* pass */
    }
#line 338 "src/hir.tr"
    return AstType_init(_tr_str_lit_len("void", 4LL));
}

__attribute__((hot)) long long _tr_str_len(TrStr s) {
#line 341 "src/hir.tr"
    /* unsafe block */
#line 342 "src/hir.tr"
    char* p = ((char*)(_tr_strz(s)));
#line 343 "src/hir.tr"
    long long i = 0LL;
#line 344 "src/hir.tr"
    while ((((long long)((*_TR_PTR_OFF(p, i)))) != 0LL)) {
#line 345 "src/hir.tr"
        i = (i + 1LL);
    }
#line 346 "src/hir.tr"
    return i;
}

