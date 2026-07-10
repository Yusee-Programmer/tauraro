#include "tauraro_types.h"

TrStr _print_i64_sym();
void lower_main(LModule* m, HirFunction* f);
bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s);
TrStr _ident_name(HirExpr* e);
bool lower_call_stmt(LModule* m, LFunc* lf, HirExpr* e);
long long lower_expr(LFunc* lf, HirExpr* e);

__attribute__((hot)) TrStr _print_i64_sym() {
    /* pass */
    return _tr_str_lit("_tr_rt_print_i64");
}

__attribute__((hot)) LModule* lower_to_lir(HirProgram* prog) {
    /* pass */
    LModule* m = LModule_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if ((strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("main"))) == 0)) {
            /* pass */
            lower_main(m, f);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return m;
}

__attribute__((hot)) void lower_main(LModule* m, HirFunction* f) {
    /* pass */
    LFunc* lf = LFunc_init(_tr_str_lit("main"));
    /* pass */
    lf->is_main = true;
    /* pass */
    long long si = 0LL;
    /* pass */
    while ((si < f->body->stmts->len)) {
        /* pass */
        if ((!lower_stmt(m, lf, ((HirStmt*)List_ptr_get(f->body->stmts, si))))) {
            /* pass */
            m->ok = false;
            /* pass */
            _tr_obj_release(lf, _trdrop_LFunc);
            return;
        }
        /* pass */
        si = (si + 1LL);
    }
    /* pass */
    lf->block->term = LTerm_ctor_TRetInt(0LL);
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    __auto_type _t2238 = (*s);
    if (_t2238.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2238.data.SLineMarker.n;
        return true;
    } else if (_t2238.tag == HirStmt_SPass) {
        return true;
    } else if (_t2238.tag == HirStmt_SReturn) {
        __auto_type _ = _t2238.data.SReturn.val;
        return true;
    } else if (_t2238.tag == HirStmt_SLet) {
        __auto_type name = _t2238.data.SLet.name;
__auto_type val = _t2238.data.SLet.val;
        /* pass */
        if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        long long v = lower_expr(lf, val);
        /* pass */
        if ((v < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_add_var(lf, name);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(name, v));
        /* pass */
        return true;
    } else if (_t2238.tag == HirStmt_SAssign) {
        __auto_type target = _t2238.data.SAssign.target;
__auto_type val = _t2238.data.SAssign.val;
        /* pass */
        TrStr tn = _ident_name(target);
        /* pass */
        if ((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        long long v2 = lower_expr(lf, val);
        /* pass */
        if ((v2 < 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        LFunc_add_var(lf, tn);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(tn, v2));
        /* pass */
        _tr_str_release(tn);
        return true;
    } else if (_t2238.tag == HirStmt_SExpr) {
        __auto_type e = _t2238.data.SExpr.expr;
        /* pass */
        return lower_call_stmt(m, lf, e);
    } else if (1) {
        __auto_type _ = _t2238;
        /* pass */
        return false;
    }
}

__attribute__((hot)) TrStr _ident_name(HirExpr* e) {
    /* pass */
    __auto_type _t2239 = (*e);
    if (_t2239.tag == HirExpr_EIdent) {
        __auto_type n = _t2239.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (1) {
        __auto_type _ = _t2239;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool lower_call_stmt(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2240 = (*e);
    if (_t2240.tag == HirExpr_ECall) {
        __auto_type callee = _t2240.data.ECall.callee;
__auto_type args = _t2240.data.ECall.args;
        /* pass */
        if ((strcmp(_tr_strz(_ident_name(callee)), _tr_strz(_tr_str_lit("print"))) != 0)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((args->len != 1LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long a = lower_expr(lf, ((HirExpr*)List_ptr_get(args, 0LL)));
        /* pass */
        if ((a < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr sym = _print_i64_sym();
        /* pass */
        LModule_add_extern(m, sym);
        /* pass */
        List_i64* cargs = (void*)List_i64_new();
        /* pass */
        List_i64_append(cargs, a);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), sym, cargs));
        /* pass */
        _tr_str_release(sym);
        return true;
    } else if (1) {
        __auto_type _ = _t2240;
        /* pass */
        return false;
    }
}

__attribute__((hot)) long long lower_expr(LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2241 = (*e);
    if (_t2241.tag == HirExpr_ELitInt) {
        __auto_type v = _t2241.data.ELitInt.val;
        /* pass */
        long long d = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(d, v));
        /* pass */
        return d;
    } else if (_t2241.tag == HirExpr_EIdent) {
        __auto_type name = _t2241.data.EIdent.name;
        /* pass */
        LFunc_add_var(lf, name);
        /* pass */
        long long d2 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(d2, name));
        /* pass */
        return d2;
    } else if (_t2241.tag == HirExpr_EBinOp) {
        __auto_type op = _t2241.data.EBinOp.op;
__auto_type l = _t2241.data.EBinOp.left;
__auto_type r = _t2241.data.EBinOp.right;
        /* pass */
        if ((!(((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long a = lower_expr(lf, l);
        /* pass */
        if ((a < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long b = lower_expr(lf, r);
        /* pass */
        if ((b < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long d3 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(d3, op, a, b));
        /* pass */
        return d3;
    } else if (1) {
        __auto_type _ = _t2241;
        /* pass */
        return (-1LL);
    }
}

