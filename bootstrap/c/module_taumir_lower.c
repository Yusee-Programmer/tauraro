#include "tauraro_types.h"

TrStr _print_i64_sym();
void _lir_lower_function(LModule* m, HirFunction* f);
bool lower_block(LModule* m, LFunc* lf, HirBlock* hb);
bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s);
TrStr _ident_name(HirExpr* e);
bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e);
bool _supported_op(TrStr op);
long long lower_expr(LModule* m, LFunc* lf, HirExpr* e);

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
        HirFunction* f0 = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((strcmp(_tr_strz(f0->class_name), _tr_strz(_tr_str_lit(""))) == 0) && (!f0->is_extern))) {
            /* pass */
            List_TrStr_append(m->fn_names, f0->name);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, i));
        /* pass */
        if (((strcmp(_tr_strz(f->class_name), _tr_strz(_tr_str_lit(""))) == 0) && (!f->is_extern))) {
            /* pass */
            _lir_lower_function(m, f);
            /* pass */
            if ((!m->ok)) {
                /* pass */
                return m;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return m;
}

__attribute__((hot)) void _lir_lower_function(LModule* m, HirFunction* f) {
    /* pass */
    LFunc* lf = LFunc_init(f->name);
    /* pass */
    if ((strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("main"))) == 0)) {
        /* pass */
        lf->is_main = true;
    }
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        TrStr pn = ((HirParam*)List_ptr_get(f->params, pi))->name;
        /* pass */
        if ((strcmp(_tr_strz(pn), _tr_strz(_tr_str_lit("self"))) != 0)) {
            /* pass */
            List_TrStr_append(lf->params, pn);
            /* pass */
            LFunc_add_var(lf, pn);
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    LFunc_set_cur(lf, LFunc_new_block(lf));
    /* pass */
    if ((!lower_block(m, lf, f->body))) {
        /* pass */
        m->ok = false;
        /* pass */
        _tr_obj_release(lf, _trdrop_LFunc);
        return;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
    /* pass */
    List_ptr_append(m->funcs, _tr_obj_retain(lf));
    _tr_obj_release(lf, _trdrop_LFunc);
}

__attribute__((hot)) bool lower_block(LModule* m, LFunc* lf, HirBlock* hb) {
    /* pass */
    long long si = 0LL;
    /* pass */
    while ((si < hb->stmts->len)) {
        /* pass */
        if ((!lower_stmt(m, lf, ((HirStmt*)List_ptr_get(hb->stmts, si))))) {
            /* pass */
            return false;
        }
        /* pass */
        si = (si + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    __auto_type _t2239 = (*s);
    if (_t2239.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2239.data.SLineMarker.n;
        return true;
    } else if (_t2239.tag == HirStmt_SPass) {
        return true;
    } else if (_t2239.tag == HirStmt_SReturn) {
        __auto_type val = _t2239.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            long long rv = lower_expr(m, lf, val);
            /* pass */
            if ((rv < 0LL)) {
                /* pass */
                return false;
            }
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TRetVal(rv));
        } else {
            /* pass */
            LFunc_set_term(lf, LTerm_ctor_TRetInt(0LL));
        }
        /* pass */
        return true;
    } else if (_t2239.tag == HirStmt_SLet) {
        __auto_type name = _t2239.data.SLet.name;
__auto_type val = _t2239.data.SLet.val;
        /* pass */
        if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        long long v = lower_expr(m, lf, val);
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
    } else if (_t2239.tag == HirStmt_SAssign) {
        __auto_type target = _t2239.data.SAssign.target;
__auto_type val = _t2239.data.SAssign.val;
        /* pass */
        TrStr tn = _ident_name(target);
        /* pass */
        if ((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        long long v2 = lower_expr(m, lf, val);
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
    } else if (_t2239.tag == HirStmt_SIf) {
        __auto_type cond = _t2239.data.SIf.cond;
__auto_type then_b = _t2239.data.SIf.then_b;
__auto_type else_b = _t2239.data.SIf.else_b;
        /* pass */
        long long cv = lower_expr(m, lf, cond);
        /* pass */
        if ((cv < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long then_id = LFunc_new_block(lf);
        /* pass */
        long long else_id = LFunc_new_block(lf);
        /* pass */
        long long end_id = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(cv, then_id, else_id));
        /* pass */
        LFunc_set_cur(lf, then_id);
        /* pass */
        if ((!lower_block(m, lf, then_b))) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
        /* pass */
        LFunc_set_cur(lf, else_id);
        /* pass */
        if ((!lower_block(m, lf, else_b))) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_id));
        /* pass */
        LFunc_set_cur(lf, end_id);
        /* pass */
        return true;
    } else if (_t2239.tag == HirStmt_SWhile) {
        __auto_type cond = _t2239.data.SWhile.cond;
__auto_type body = _t2239.data.SWhile.body;
        /* pass */
        long long hdr = LFunc_new_block(lf);
        /* pass */
        long long bdy = LFunc_new_block(lf);
        /* pass */
        long long ext = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
        /* pass */
        LFunc_set_cur(lf, hdr);
        /* pass */
        long long cv2 = lower_expr(m, lf, cond);
        /* pass */
        if ((cv2 < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(cv2, bdy, ext));
        /* pass */
        LFunc_set_cur(lf, bdy);
        /* pass */
        if ((!lower_block(m, lf, body))) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
        /* pass */
        LFunc_set_cur(lf, ext);
        /* pass */
        return true;
    } else if (_t2239.tag == HirStmt_SExpr) {
        __auto_type e = _t2239.data.SExpr.expr;
        /* pass */
        return lower_expr_stmt(m, lf, e);
    } else if (1) {
        __auto_type _ = _t2239;
        /* pass */
        return false;
    }
}

__attribute__((hot)) TrStr _ident_name(HirExpr* e) {
    /* pass */
    __auto_type _t2240 = (*e);
    if (_t2240.tag == HirExpr_EIdent) {
        __auto_type n = _t2240.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (1) {
        __auto_type _ = _t2240;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2241 = (*e);
    if (_t2241.tag == HirExpr_ECall) {
        __auto_type callee = _t2241.data.ECall.callee;
__auto_type args = _t2241.data.ECall.args;
        /* pass */
        TrStr fname = _ident_name(callee);
        /* pass */
        if ((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit("print"))) == 0)) {
            /* pass */
            if ((args->len != 1LL)) {
                /* pass */
                _tr_str_release(fname);
                return false;
            }
            /* pass */
            long long a = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((a < 0LL)) {
                /* pass */
                _tr_str_release(fname);
                return false;
            }
            /* pass */
            ({ TrStr _at_t2242 = (_print_i64_sym()); LModule_add_extern(m, _at_t2242); _tr_str_release(_at_t2242); });
            /* pass */
            List_i64* cargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(cargs, a);
            /* pass */
            ({ TrStr _at_t2243 = (_print_i64_sym()); LFunc_emit(lf, LInst_ctor_ICall((-1LL), _at_t2243, cargs)); _tr_str_release(_at_t2243); });
            /* pass */
            _tr_str_release(fname);
            return true;
        }
        /* pass */
        long long r = lower_expr(m, lf, e);
        /* pass */
        _tr_str_release(fname);
        return (r >= 0LL);
    } else if (1) {
        __auto_type _ = _t2241;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _supported_op(TrStr op) {
    /* pass */
    if ((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long lower_expr(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2244 = (*e);
    if (_t2244.tag == HirExpr_ELitInt) {
        __auto_type v = _t2244.data.ELitInt.val;
        /* pass */
        long long d = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(d, v));
        /* pass */
        return d;
    } else if (_t2244.tag == HirExpr_EIdent) {
        __auto_type name = _t2244.data.EIdent.name;
        /* pass */
        LFunc_add_var(lf, name);
        /* pass */
        long long d2 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(d2, name));
        /* pass */
        return d2;
    } else if (_t2244.tag == HirExpr_EBinOp) {
        __auto_type op = _t2244.data.EBinOp.op;
__auto_type l = _t2244.data.EBinOp.left;
__auto_type r = _t2244.data.EBinOp.right;
        /* pass */
        if ((!_supported_op(op))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long a = lower_expr(m, lf, l);
        /* pass */
        if ((a < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long b = lower_expr(m, lf, r);
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
    } else if (_t2244.tag == HirExpr_ECall) {
        __auto_type callee = _t2244.data.ECall.callee;
__auto_type args = _t2244.data.ECall.args;
        /* pass */
        TrStr fn = _ident_name(callee);
        /* pass */
        if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if ((!LModule_is_user_fn(m, fn))) {
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if ((args->len > 4LL)) {
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        List_i64* argvregs = (void*)List_i64_new();
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < args->len)) {
            /* pass */
            long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
            /* pass */
            if ((av < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                List_i64_free(argvregs);
                return (-1LL);
            }
            /* pass */
            List_i64_append(argvregs, av);
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        long long d4 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(d4, fn, argvregs));
        /* pass */
        _tr_str_release(fn);
        return d4;
    } else if (1) {
        __auto_type _ = _t2244;
        /* pass */
        return (-1LL);
    }
}

