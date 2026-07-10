#include "tauraro_types.h"

TrStr _print_i64_sym();
void _lir_lower_function(LModule* m, HirFunction* f);
bool lower_block(LModule* m, LFunc* lf, HirBlock* hb);
bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s);
TrStr _ident_name(HirExpr* e);
bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e);
bool _int_op(TrStr op);
long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx);
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
    } else if (_t2239.tag == HirStmt_SAutoDrop) {
        return true;
    } else if (_t2239.tag == HirStmt_SFree) {
        __auto_type _ = _t2239.data.SFree.name;
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
        LFunc_set_var_type(lf, name, LFunc_vreg_type(lf, v));
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
        LFunc_set_var_type(lf, tn, LFunc_vreg_type(lf, v2));
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
            long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((av < 0LL)) {
                /* pass */
                _tr_str_release(fname);
                return false;
            }
            /* pass */
            TrStr sym = _print_i64_sym();
            /* pass */
            if ((LFunc_vreg_type(lf, av) == 1LL)) {
                /* pass */
                TrStr _strtmp_t2242 = _tr_str_lit("_tr_rt_print_cstr");
                _tr_str_release(sym);
                sym = _strtmp_t2242;
            }
            /* pass */
            LModule_add_extern(m, sym);
            /* pass */
            List_i64* cargs = (void*)List_i64_new();
            /* pass */
            List_i64_append(cargs, av);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), sym, cargs));
            /* pass */
            _tr_str_release(fname);
            _tr_str_release(sym);
            return true;
        }
        /* pass */
        long long r = lower_expr(m, lf, e);
        /* pass */
        _tr_str_release(fname);
        return (r >= 0LL);
    } else if (_t2241.tag == HirExpr_EMethodCall) {
        /* pass */
        long long rm = lower_expr(m, lf, e);
        /* pass */
        return (rm >= 0LL);
    } else if (1) {
        __auto_type _ = _t2241;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _int_op(TrStr op) {
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

__attribute__((hot)) long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx) {
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_list_get_i64"));
    /* pass */
    List_i64* gargs = (void*)List_i64_new();
    /* pass */
    List_i64_append(gargs, handle);
    /* pass */
    List_i64_append(gargs, idx);
    /* pass */
    long long gd = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(gd, _tr_str_lit("_tr_rt_list_get_i64"), gargs));
    /* pass */
    return gd;
}

__attribute__((hot)) long long lower_expr(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2243 = (*e);
    if (_t2243.tag == HirExpr_ELitInt) {
        __auto_type v = _t2243.data.ELitInt.val;
        /* pass */
        long long d = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(d, v));
        /* pass */
        return d;
    } else if (_t2243.tag == HirExpr_ELitStr) {
        __auto_type sv = _t2243.data.ELitStr.val;
        /* pass */
        long long idx = LModule_add_string(m, sv);
        /* pass */
        long long ds = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStr(ds, idx));
        /* pass */
        LFunc_set_vreg_type(lf, ds, 1LL);
        /* pass */
        return ds;
    } else if (_t2243.tag == HirExpr_EIdent) {
        __auto_type name = _t2243.data.EIdent.name;
        /* pass */
        LFunc_add_var(lf, name);
        /* pass */
        long long d2 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(d2, name));
        /* pass */
        LFunc_set_vreg_type(lf, d2, LFunc_var_type(lf, name));
        /* pass */
        return d2;
    } else if (_t2243.tag == HirExpr_EBinOp) {
        __auto_type op = _t2243.data.EBinOp.op;
__auto_type l = _t2243.data.EBinOp.left;
__auto_type r = _t2243.data.EBinOp.right;
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
        long long at = LFunc_vreg_type(lf, a);
        /* pass */
        long long bt = LFunc_vreg_type(lf, b);
        /* pass */
        if ((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) && (at == 1LL)) && (bt == 1LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_concat"));
            /* pass */
            List_i64* ca = (void*)List_i64_new();
            /* pass */
            List_i64_append(ca, a);
            /* pass */
            List_i64_append(ca, b);
            /* pass */
            long long dc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(dc, _tr_str_lit("_tr_rt_str_concat"), ca));
            /* pass */
            LFunc_set_vreg_type(lf, dc, 1LL);
            /* pass */
            return dc;
        }
        /* pass */
        if (((at == 1LL) || (bt == 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((!_int_op(op))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long d3 = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IBinOp(d3, op, a, b));
        /* pass */
        return d3;
    } else if (_t2243.tag == HirExpr_ECall) {
        __auto_type callee = _t2243.data.ECall.callee;
__auto_type args = _t2243.data.ECall.args;
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
            long long avr = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, ai)));
            /* pass */
            if ((avr < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                List_i64_free(argvregs);
                return (-1LL);
            }
            /* pass */
            List_i64_append(argvregs, avr);
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
    } else if (_t2243.tag == HirExpr_EList) {
        __auto_type items = _t2243.data.EList.items;
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_list_new"));
        /* pass */
        long long hv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(hv, _tr_str_lit("_tr_rt_list_new"), (void*)List_i64_new()));
        /* pass */
        LFunc_set_vreg_type(lf, hv, 2LL);
        /* pass */
        long long li = 0LL;
        /* pass */
        while ((li < items->len)) {
            /* pass */
            long long ev = lower_expr(m, lf, ((HirExpr*)List_ptr_get(items, li)));
            /* pass */
            if ((ev < 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, ev) != 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_push_i64"));
            /* pass */
            List_i64* pa = (void*)List_i64_new();
            /* pass */
            List_i64_append(pa, hv);
            /* pass */
            List_i64_append(pa, ev);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_push_i64"), pa));
            /* pass */
            li = (li + 1LL);
        }
        /* pass */
        return hv;
    } else if (_t2243.tag == HirExpr_EIndex) {
        __auto_type obj = _t2243.data.EIndex.obj;
__auto_type idx = _t2243.data.EIndex._tr_v_index;
        /* pass */
        long long ov = lower_expr(m, lf, obj);
        /* pass */
        if ((ov < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((LFunc_vreg_type(lf, ov) != 2LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long iv = lower_expr(m, lf, idx);
        /* pass */
        if ((iv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _list_get(m, lf, ov, iv);
    } else if (_t2243.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t2243.data.EPropAccess.obj;
__auto_type prop = _t2243.data.EPropAccess.prop;
        /* pass */
        if ((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("len"))) != 0)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long ovl = lower_expr(m, lf, obj);
        /* pass */
        if ((ovl < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((LFunc_vreg_type(lf, ovl) != 2LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_list_len"));
        /* pass */
        List_i64* la = (void*)List_i64_new();
        /* pass */
        List_i64_append(la, ovl);
        /* pass */
        long long ld = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(ld, _tr_str_lit("_tr_rt_list_len"), la));
        /* pass */
        return ld;
    } else if (_t2243.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t2243.data.EMethodCall.obj;
__auto_type method = _t2243.data.EMethodCall.method;
__auto_type margs = _t2243.data.EMethodCall.args;
        /* pass */
        long long ovm = lower_expr(m, lf, obj);
        /* pass */
        if ((ovm < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((LFunc_vreg_type(lf, ovm) != 2LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("push"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("append"))) == 0))) {
            /* pass */
            if ((margs->len != 1LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((av < 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, av) != 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_push_i64"));
            /* pass */
            List_i64* ppa = (void*)List_i64_new();
            /* pass */
            List_i64_append(ppa, ovm);
            /* pass */
            List_i64_append(ppa, av);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall((-1LL), _tr_str_lit("_tr_rt_list_push_i64"), ppa));
            /* pass */
            return ovm;
        }
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0)) {
            /* pass */
            if ((margs->len != 1LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long giv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
            /* pass */
            if ((giv < 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            return _list_get(m, lf, ovm, giv);
        }
        /* pass */
        return (-1LL);
    } else if (1) {
        __auto_type _ = _t2243;
        /* pass */
        return (-1LL);
    }
}

