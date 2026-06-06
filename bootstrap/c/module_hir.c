#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) HirFStringPart* HirFStringPart_init() {
    /* pass */
    HirFStringPart* p = ((HirFStringPart*)_tr_checked_alloc(sizeof(HirFStringPart)));
    /* pass */
    p->is_expr = false;
    /* pass */
    p->text = "";
    /* pass */
    p->expr = (HirExpr*)(0LL);
    /* pass */
    p->fmt_spec = "";
    /* pass */
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) HirMatchArm* HirMatchArm_init(Pattern pat, HirBlock* body) {
    /* pass */
    HirMatchArm* a = ((HirMatchArm*)_tr_checked_alloc(sizeof(HirMatchArm)));
    /* pass */
    a->pat = pat;
    /* pass */
    a->body = body;
    /* pass */
    a->guard = (HirExpr*)(0LL);
    /* pass */
    return a;
}

__attribute__((malloc,returns_nonnull,hot)) HirBlock* HirBlock_init() {
    /* pass */
    HirBlock* b = ((HirBlock*)_tr_checked_alloc(sizeof(HirBlock)));
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
    HirProgram* p = ((HirProgram*)_tr_checked_alloc(sizeof(HirProgram)));
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
    p->type_alias_names = (void*)List_str_new();
    /* pass */
    p->type_alias_types = (void*)List_ptr_new();
    /* pass */
    return p;
}

__attribute__((hot)) HirExpr* box_hirexpr(HirExpr e) {
    /* pass */
    HirExpr* p = ((HirExpr*)_tr_c_calloc((size_t)(1LL), sizeof(HirExpr)));
    /* pass */
    (*p = e);
    /* pass */
    return p;
}

__attribute__((hot)) HirStmt* box_hirstmt(HirStmt s) {
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
        return AstType_init("void");
    }
    /* pass */
    __auto_type _t128 = (*e);
    if (_t128.tag == HirExpr_ELitInt) {
        __auto_type ty = _t128.data.ELitInt.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ELitFloat) {
        __auto_type ty = _t128.data.ELitFloat.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ELitStr) {
        __auto_type ty = _t128.data.ELitStr.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ELitBytes) {
        __auto_type ty = _t128.data.ELitBytes.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ERawStr) {
        __auto_type ty = _t128.data.ERawStr.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ELitChar) {
        __auto_type ty = _t128.data.ELitChar.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ELitBool) {
        __auto_type ty = _t128.data.ELitBool.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ELitNone) {
        __auto_type ty = _t128.data.ELitNone.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EIdent) {
        __auto_type ty = _t128.data.EIdent.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EBinOp) {
        __auto_type ty = _t128.data.EBinOp.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EUnaryOp) {
        __auto_type ty = _t128.data.EUnaryOp.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ECall) {
        __auto_type ty = _t128.data.ECall.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EMethodCall) {
        __auto_type ty = _t128.data.EMethodCall.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EPropAccess) {
        __auto_type ty = _t128.data.EPropAccess.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EIndex) {
        __auto_type ty = _t128.data.EIndex.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ECast) {
        __auto_type ty = _t128.data.ECast.target_ty;
        return ty;
    } else if (_t128.tag == HirExpr_EFString) {
        __auto_type ty = _t128.data.EFString.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ETryExpr) {
        __auto_type ty = _t128.data.ETryExpr.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EClosure) {
        return AstType_init("lambda");
    } else if (_t128.tag == HirExpr_ESuperMethodCall) {
        __auto_type ty = _t128.data.ESuperMethodCall.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ESuperPropAccess) {
        __auto_type ty = _t128.data.ESuperPropAccess.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EList) {
        __auto_type ty = _t128.data.EList.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ESet) {
        __auto_type ty = _t128.data.ESet.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EDict) {
        __auto_type ty = _t128.data.EDict.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ETuple) {
        __auto_type ty = _t128.data.ETuple.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EListComp) {
        __auto_type ty = _t128.data.EListComp.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EGeneratorExpr) {
        __auto_type ty = _t128.data.EGeneratorExpr.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ESlice) {
        __auto_type ty = _t128.data.ESlice.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EAwait) {
        __auto_type ty = _t128.data.EAwait.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EAwaitTimeout) {
        __auto_type ty = _t128.data.EAwaitTimeout.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EYield) {
        __auto_type ty = _t128.data.EYield.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ETry) {
        __auto_type ty = _t128.data.ETry.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ERange) {
        __auto_type ty = _t128.data.ERange.ty;
        return ty;
    } else if (_t128.tag == HirExpr_ESizeOf) {
        __auto_type ty = _t128.data.ESizeOf.ty;
        return ty;
    } else if (_t128.tag == HirExpr_EIfElse) {
        __auto_type ty = _t128.data.EIfElse.ty;
        return ty;
    } else if (1) {
        __auto_type _ = _t128;
        /* pass */
        /* pass */
    }
    /* pass */
    return AstType_init("void");
}

__attribute__((hot)) long long _tr_str_len(char* s) {
    /* pass */
    /* unsafe block */
    /* pass */
    char* p = ((char*)(s));
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

