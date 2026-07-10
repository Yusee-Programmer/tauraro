#include "tauraro_types.h"

TrStr _print_i64_sym();
bool _is_list_tag(long long t);
long long _list_elem_tag(long long t);
long long _list_tag_for_elem(long long et);
bool _is_cmp_op(TrStr op);
bool _is_int_typename(TrStr n);
long long _ast_type_tag(AstType* ty);
bool _register_global(LModule* m, HirStmt* s);
bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s);
void _lir_lower_function(LModule* m, HirFunction* f);
bool lower_block(LModule* m, LFunc* lf, HirBlock* hb);
bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s);
bool _lower_for(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
bool _lower_for_range(LModule* m, LFunc* lf, TrStr var, List_ptr* args, HirBlock* body);
bool _lower_for_list(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body);
void _emit_incr(LFunc* lf, TrStr name);
TrStr _ident_name(HirExpr* e);
TrStr _write_sym(long long t);
void _emit_call0(LModule* m, LFunc* lf, TrStr sym);
bool _lower_print(LModule* m, LFunc* lf, List_ptr* args);
bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e);
bool _int_op(TrStr op);
TrStr _lir_digit(long long d);
TrStr _lir_itoa(long long n);
long long _norm_bool(LFunc* lf, long long v);
long long _str_call0(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long restype);
long long _str_call1(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long arg, long long restype);
long long _lower_str_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs);
bool _is_const_int(HirExpr* e);
long long _const_int_val(HirExpr* e);
void _emit_add_const(LFunc* lf, TrStr name, long long delta);
long long _list_get(LModule* m, LFunc* lf, long long handle, long long idx);
long long lower_expr(LModule* m, LFunc* lf, HirExpr* e);

__attribute__((hot)) TrStr _print_i64_sym() {
    /* pass */
    return _tr_str_lit("_tr_rt_print_i64");
}

__attribute__((hot)) bool _is_list_tag(long long t) {
    /* pass */
    return ((t == 2LL) || (t == 3LL));
}

__attribute__((hot)) long long _list_elem_tag(long long t) {
    /* pass */
    if ((t == 3LL)) {
        /* pass */
        return 1LL;
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long _list_tag_for_elem(long long et) {
    /* pass */
    if ((et == 1LL)) {
        /* pass */
        return 3LL;
    }
    /* pass */
    return 2LL;
}

__attribute__((hot)) bool _is_cmp_op(TrStr op) {
    /* pass */
    return ((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0));
}

__attribute__((hot)) bool _is_int_typename(TrStr n) {
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i8"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("usize"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("isize"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u8"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long _ast_type_tag(AstType* ty) {
    /* pass */
    TrStr n = ty->name;
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("String"))) == 0))) {
        /* pass */
        return 1LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return 4LL;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            AstType* et = (*((AstType**)List_ptr_get(ty->args, 0LL)));
            /* pass */
            if (((strcmp(_tr_strz(et->name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(et->name), _tr_strz(_tr_str_lit("String"))) == 0))) {
                /* pass */
                return 3LL;
            }
            /* pass */
            if (_is_int_typename(et->name)) {
                /* pass */
                return 2LL;
            }
            /* pass */
            return (-1LL);
        }
        /* pass */
        return 2LL;
    }
    /* pass */
    if (_is_int_typename(n)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit(""))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("None"))) == 0))) {
        /* pass */
        return 0LL;
    }
    /* pass */
    return (-1LL);
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
            /* pass */
            List_i64_append(m->fn_ret, _ast_type_tag(f0->ret_ty));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long ti = 0LL;
    /* pass */
    while ((ti < prog->top_level_stmts->len)) {
        /* pass */
        if ((!_register_global(m, ((HirStmt*)List_ptr_get(prog->top_level_stmts, ti))))) {
            /* pass */
            m->ok = false;
            /* pass */
            return m;
        }
        /* pass */
        ti = (ti + 1LL);
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

__attribute__((hot)) bool _register_global(LModule* m, HirStmt* s) {
    /* pass */
    __auto_type _t2238 = (*s);
    if (_t2238.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2238.data.SLineMarker.n;
        return true;
    } else if (_t2238.tag == HirStmt_SPass) {
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
        long long tag = _ast_type_tag(hir_expr_type(val));
        /* pass */
        if ((tag < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LModule_add_global(m, name, tag);
        /* pass */
        List_ptr_append(m->global_inits, s);
        /* pass */
        return true;
    } else if (_t2238.tag == HirStmt_SAssign) {
        __auto_type target = _t2238.data.SAssign.target;
__auto_type val = _t2238.data.SAssign.val;
        /* pass */
        TrStr nm = _ident_name(target);
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(nm);
            return false;
        }
        /* pass */
        long long tag2 = _ast_type_tag(hir_expr_type(val));
        /* pass */
        if ((tag2 < 0LL)) {
            /* pass */
            _tr_str_release(nm);
            return false;
        }
        /* pass */
        LModule_add_global(m, nm, tag2);
        /* pass */
        List_ptr_append(m->global_inits, s);
        /* pass */
        _tr_str_release(nm);
        return true;
    } else if (1) {
        __auto_type _ = _t2238;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _lower_global_init(LModule* m, LFunc* lf, HirStmt* s) {
    /* pass */
    __auto_type _t2239 = (*s);
    if (_t2239.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2239.data.SLineMarker.n;
        return true;
    } else if (_t2239.tag == HirStmt_SPass) {
        return true;
    } else if (_t2239.tag == HirStmt_SLet) {
        __auto_type name = _t2239.data.SLet.name;
__auto_type val = _t2239.data.SLet.val;
        /* pass */
        long long v = lower_expr(m, lf, val);
        /* pass */
        if ((v < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long gidx = LModule_global_index(m, name);
        /* pass */
        if ((gidx < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreGlobal(gidx, v));
        /* pass */
        return true;
    } else if (_t2239.tag == HirStmt_SAssign) {
        __auto_type target = _t2239.data.SAssign.target;
__auto_type val = _t2239.data.SAssign.val;
        /* pass */
        long long v2 = lower_expr(m, lf, val);
        /* pass */
        if ((v2 < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long gidx2 = ({ TrStr _at_t2240 = (_ident_name(target)); __auto_type _wr = (LModule_global_index(m, _at_t2240)); _tr_str_release(_at_t2240); _wr; });
        /* pass */
        if ((gidx2 < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreGlobal(gidx2, v2));
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t2239;
        /* pass */
        return false;
    }
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
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        if ((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
            /* pass */
            long long ptag = _ast_type_tag(p->ty);
            /* pass */
            if ((ptag < 0LL)) {
                /* pass */
                m->ok = false;
                /* pass */
                _tr_obj_release(lf, _trdrop_LFunc);
                return;
            }
            /* pass */
            List_TrStr_append(lf->params, p->name);
            /* pass */
            LFunc_add_var(lf, p->name);
            /* pass */
            LFunc_set_var_type(lf, p->name, ptag);
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    LFunc_set_cur(lf, LFunc_new_block(lf));
    /* pass */
    if (lf->is_main) {
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < m->global_inits->len)) {
            /* pass */
            if ((!_lower_global_init(m, lf, ((HirStmt*)List_ptr_get(m->global_inits, gi))))) {
                /* pass */
                m->ok = false;
                /* pass */
                _tr_obj_release(lf, _trdrop_LFunc);
                return;
            }
            /* pass */
            gi = (gi + 1LL);
        }
    }
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
    __auto_type _t2241 = (*s);
    if (_t2241.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2241.data.SLineMarker.n;
        return true;
    } else if (_t2241.tag == HirStmt_SPass) {
        return true;
    } else if (_t2241.tag == HirStmt_SAutoDrop) {
        return true;
    } else if (_t2241.tag == HirStmt_SFree) {
        __auto_type _ = _t2241.data.SFree.name;
        return true;
    } else if (_t2241.tag == HirStmt_SReturn) {
        __auto_type val = _t2241.data.SReturn.val;
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
    } else if (_t2241.tag == HirStmt_SLet) {
        __auto_type name = _t2241.data.SLet.name;
__auto_type val = _t2241.data.SLet.val;
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
    } else if (_t2241.tag == HirStmt_SAssign) {
        __auto_type target = _t2241.data.SAssign.target;
__auto_type val = _t2241.data.SAssign.val;
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
        if (((LFunc_var_index(lf, tn) < 0LL) && LModule_is_global(m, tn))) {
            /* pass */
            LFunc_emit(lf, LInst_ctor_IStoreGlobal(LModule_global_index(m, tn), v2));
            /* pass */
            _tr_str_release(tn);
            return true;
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
    } else if (_t2241.tag == HirStmt_SIf) {
        __auto_type cond = _t2241.data.SIf.cond;
__auto_type then_b = _t2241.data.SIf.then_b;
__auto_type else_b = _t2241.data.SIf.else_b;
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
    } else if (_t2241.tag == HirStmt_SWhile) {
        __auto_type cond = _t2241.data.SWhile.cond;
__auto_type body = _t2241.data.SWhile.body;
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
        List_i64_append(lf->loop_cont, hdr);
        /* pass */
        List_i64_append(lf->loop_brk, ext);
        /* pass */
        bool wok = lower_block(m, lf, body);
        /* pass */
        List_i64_pop(lf->loop_cont);
        /* pass */
        List_i64_pop(lf->loop_brk);
        /* pass */
        if ((!wok)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
        /* pass */
        LFunc_set_cur(lf, ext);
        /* pass */
        return true;
    } else if (_t2241.tag == HirStmt_SBreak) {
        __auto_type bval = _t2241.data.SBreak.val;
        /* pass */
        if ((((unsigned long long)(bval)) != ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        if ((lf->loop_brk->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->loop_brk, (lf->loop_brk->len - 1LL))));
        /* pass */
        LFunc_set_cur(lf, LFunc_new_block(lf));
        /* pass */
        return true;
    } else if (_t2241.tag == HirStmt_SContinue) {
        /* pass */
        if ((lf->loop_cont->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(List_i64_get(lf->loop_cont, (lf->loop_cont->len - 1LL))));
        /* pass */
        LFunc_set_cur(lf, LFunc_new_block(lf));
        /* pass */
        return true;
    } else if (_t2241.tag == HirStmt_SFor) {
        __auto_type var = _t2241.data.SFor.var;
__auto_type iter = _t2241.data.SFor.iter;
__auto_type body = _t2241.data.SFor.body;
        /* pass */
        return _lower_for(m, lf, var, iter, body);
    } else if (_t2241.tag == HirStmt_SExpr) {
        __auto_type e = _t2241.data.SExpr.expr;
        /* pass */
        return lower_expr_stmt(m, lf, e);
    } else if (1) {
        __auto_type _ = _t2241;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _lower_for(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
    /* pass */
    __auto_type _t2242 = (*iter);
    if (_t2242.tag == HirExpr_ECall) {
        __auto_type callee = _t2242.data.ECall.callee;
__auto_type args = _t2242.data.ECall.args;
        /* pass */
        if ((strcmp(_tr_strz(_ident_name(callee)), _tr_strz(_tr_str_lit("range"))) == 0)) {
            /* pass */
            return _lower_for_range(m, lf, var, args, body);
        }
    } else if (1) {
        __auto_type _ = _t2242;
        /* pass */
        /* pass */
    }
    /* pass */
    return _lower_for_list(m, lf, var, iter, body);
}

__attribute__((hot)) bool _lower_for_range(LModule* m, LFunc* lf, TrStr var, List_ptr* args, HirBlock* body) {
    /* pass */
    if (((args->len < 1LL) || (args->len > 3LL))) {
        /* pass */
        return false;
    }
    /* pass */
    long long stepv = 1LL;
    /* pass */
    TrStr cmp = _tr_str_lit("<");
    /* pass */
    if ((args->len == 3LL)) {
        /* pass */
        if ((!_is_const_int(((HirExpr*)List_ptr_get(args, 2LL))))) {
            /* pass */
            _tr_str_release(cmp);
            return false;
        }
        /* pass */
        stepv = _const_int_val(((HirExpr*)List_ptr_get(args, 2LL)));
        /* pass */
        if ((stepv == 0LL)) {
            /* pass */
            _tr_str_release(cmp);
            return false;
        }
        /* pass */
        if ((stepv < 0LL)) {
            /* pass */
            TrStr _strtmp_t2243 = _tr_str_lit(">");
            _tr_str_release(cmp);
            cmp = _strtmp_t2243;
        }
    }
    /* pass */
    long long sv = (-1LL);
    /* pass */
    HirExpr* end_expr = ((HirExpr*)List_ptr_get(args, 0LL));
    /* pass */
    if ((args->len == 1LL)) {
        /* pass */
        long long z = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
        /* pass */
        sv = z;
    } else {
        /* pass */
        sv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
        /* pass */
        if ((sv < 0LL)) {
            /* pass */
            _tr_str_release(cmp);
            return false;
        }
        /* pass */
        end_expr = ((HirExpr*)List_ptr_get(args, 1LL));
    }
    /* pass */
    LFunc_add_var(lf, var);
    /* pass */
    LFunc_set_var_type(lf, var, 0LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, sv));
    /* pass */
    long long hdr = LFunc_new_block(lf);
    /* pass */
    long long bdy = LFunc_new_block(lf);
    /* pass */
    long long latch = LFunc_new_block(lf);
    /* pass */
    long long ext = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, hdr);
    /* pass */
    long long vv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(vv, var));
    /* pass */
    long long ev = lower_expr(m, lf, end_expr);
    /* pass */
    if ((ev < 0LL)) {
        /* pass */
        _tr_str_release(cmp);
        return false;
    }
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, cmp, vv, ev));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    List_i64_append(lf->loop_cont, latch);
    /* pass */
    List_i64_append(lf->loop_brk, ext);
    /* pass */
    bool rok = lower_block(m, lf, body);
    /* pass */
    List_i64_pop(lf->loop_cont);
    /* pass */
    List_i64_pop(lf->loop_brk);
    /* pass */
    if ((!rok)) {
        /* pass */
        _tr_str_release(cmp);
        return false;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
    /* pass */
    LFunc_set_cur(lf, latch);
    /* pass */
    _emit_add_const(lf, var, stepv);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, ext);
    /* pass */
    _tr_str_release(cmp);
    return true;
}

__attribute__((hot)) bool _lower_for_list(LModule* m, LFunc* lf, TrStr var, HirExpr* iter, HirBlock* body) {
    /* pass */
    long long lv = lower_expr(m, lf, iter);
    /* pass */
    if ((lv < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long ltag = LFunc_vreg_type(lf, lv);
    /* pass */
    if ((!_is_list_tag(ltag))) {
        /* pass */
        return false;
    }
    /* pass */
    long long elem_t = _list_elem_tag(ltag);
    /* pass */
    long long uid = LFunc_fresh_id(lf);
    /* pass */
    TrStr hname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__forlist")), _cr.data); _tr_str_release(_cr); _cres; });
    /* pass */
    TrStr iname = ({ TrStr _cr = (_lir_itoa(uid)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__foridx")), _cr.data); _tr_str_release(_cr); _cres; });
    /* pass */
    LFunc_add_var(lf, hname);
    /* pass */
    LFunc_set_var_type(lf, hname, ltag);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(hname, lv));
    /* pass */
    long long z = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
    /* pass */
    LFunc_add_var(lf, iname);
    /* pass */
    LFunc_set_var_type(lf, iname, 0LL);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(iname, z));
    /* pass */
    long long hdr = LFunc_new_block(lf);
    /* pass */
    long long bdy = LFunc_new_block(lf);
    /* pass */
    long long latch = LFunc_new_block(lf);
    /* pass */
    long long ext = LFunc_new_block(lf);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, hdr);
    /* pass */
    long long hv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv, hname));
    /* pass */
    LModule_add_extern(m, _tr_str_lit("_tr_rt_list_len"));
    /* pass */
    List_i64* la = (void*)List_i64_new();
    /* pass */
    List_i64_append(la, hv);
    /* pass */
    long long lenv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(lenv, _tr_str_lit("_tr_rt_list_len"), la));
    /* pass */
    long long iv = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv, iname));
    /* pass */
    long long cond = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(cond, _tr_str_lit("<"), iv, lenv));
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TCondBr(cond, bdy, ext));
    /* pass */
    LFunc_set_cur(lf, bdy);
    /* pass */
    long long hv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(hv2, hname));
    /* pass */
    long long iv2 = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(iv2, iname));
    /* pass */
    long long xval = _list_get(m, lf, hv2, iv2);
    /* pass */
    LFunc_set_vreg_type(lf, xval, elem_t);
    /* pass */
    LFunc_add_var(lf, var);
    /* pass */
    LFunc_set_var_type(lf, var, elem_t);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(var, xval));
    /* pass */
    List_i64_append(lf->loop_cont, latch);
    /* pass */
    List_i64_append(lf->loop_brk, ext);
    /* pass */
    bool fok = lower_block(m, lf, body);
    /* pass */
    List_i64_pop(lf->loop_cont);
    /* pass */
    List_i64_pop(lf->loop_brk);
    /* pass */
    if ((!fok)) {
        /* pass */
        _tr_str_release(hname);
        _tr_str_release(iname);
        return false;
    }
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(latch));
    /* pass */
    LFunc_set_cur(lf, latch);
    /* pass */
    _emit_incr(lf, iname);
    /* pass */
    LFunc_set_term(lf, LTerm_ctor_TBr(hdr));
    /* pass */
    LFunc_set_cur(lf, ext);
    /* pass */
    _tr_str_release(hname);
    _tr_str_release(iname);
    return true;
}

__attribute__((hot)) void _emit_incr(LFunc* lf, TrStr name) {
    /* pass */
    long long cur = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(cur, name));
    /* pass */
    long long one = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(one, 1LL));
    /* pass */
    long long inc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit("+"), cur, one));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(name, inc));
}

__attribute__((hot)) TrStr _ident_name(HirExpr* e) {
    /* pass */
    __auto_type _t2244 = (*e);
    if (_t2244.tag == HirExpr_EIdent) {
        __auto_type n = _t2244.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (1) {
        __auto_type _ = _t2244;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) TrStr _write_sym(long long t) {
    /* pass */
    if ((t == 1LL)) {
        /* pass */
        return _tr_str_lit("_tr_rt_write_cstr");
    }
    /* pass */
    if ((t == 4LL)) {
        /* pass */
        return _tr_str_lit("_tr_rt_write_bool");
    }
    /* pass */
    return _tr_str_lit("_tr_rt_write_i64");
}

__attribute__((hot)) void _emit_call0(LModule* m, LFunc* lf, TrStr sym) {
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall((-1LL), sym, (void*)List_i64_new()));
}

__attribute__((hot)) bool _lower_print(LModule* m, LFunc* lf, List_ptr* args) {
    /* pass */
    if ((args->len == 0LL)) {
        /* pass */
        _emit_call0(m, lf, _tr_str_lit("_tr_rt_write_nl"));
        /* pass */
        return true;
    }
    /* pass */
    if ((args->len == 1LL)) {
        /* pass */
        long long av = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
        /* pass */
        if ((av < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long avt = LFunc_vreg_type(lf, av);
        /* pass */
        if (_is_list_tag(avt)) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr sym = _print_i64_sym();
        /* pass */
        if ((avt == 1LL)) {
            /* pass */
            TrStr _strtmp_t2245 = _tr_str_lit("_tr_rt_print_cstr");
            _tr_str_release(sym);
            sym = _strtmp_t2245;
        } else if ((avt == 4LL)) {
            /* pass */
            TrStr _strtmp_t2246 = _tr_str_lit("_tr_rt_print_bool");
            _tr_str_release(sym);
            sym = _strtmp_t2246;
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
        _tr_str_release(sym);
        return true;
    }
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < args->len)) {
        /* pass */
        if ((pi > 0LL)) {
            /* pass */
            _emit_call0(m, lf, _tr_str_lit("_tr_rt_write_sp"));
        }
        /* pass */
        long long pv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, pi)));
        /* pass */
        if ((pv < 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        long long pvt = LFunc_vreg_type(lf, pv);
        /* pass */
        if (_is_list_tag(pvt)) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr wsym = _write_sym(pvt);
        /* pass */
        LModule_add_extern(m, wsym);
        /* pass */
        List_i64* wargs = (void*)List_i64_new();
        /* pass */
        List_i64_append(wargs, pv);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall((-1LL), wsym, wargs));
        /* pass */
        pi = (pi + 1LL);
        _tr_str_release(wsym);
    }
    /* pass */
    _emit_call0(m, lf, _tr_str_lit("_tr_rt_write_nl"));
    /* pass */
    return true;
}

__attribute__((hot)) bool lower_expr_stmt(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2247 = (*e);
    if (_t2247.tag == HirExpr_ECall) {
        __auto_type callee = _t2247.data.ECall.callee;
__auto_type args = _t2247.data.ECall.args;
        /* pass */
        TrStr fname = _ident_name(callee);
        /* pass */
        if ((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit("print"))) == 0)) {
            /* pass */
            _tr_str_release(fname);
            return _lower_print(m, lf, args);
        }
        /* pass */
        long long r = lower_expr(m, lf, e);
        /* pass */
        _tr_str_release(fname);
        return (r >= 0LL);
    } else if (_t2247.tag == HirExpr_EMethodCall) {
        /* pass */
        long long rm = lower_expr(m, lf, e);
        /* pass */
        return (rm >= 0LL);
    } else if (1) {
        __auto_type _ = _t2247;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool _int_op(TrStr op) {
    /* pass */
    if (((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("//"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("%"))) == 0))) {
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

__attribute__((hot)) TrStr _lir_digit(long long d) {
    /* pass */
    if ((d == 0LL)) {
        /* pass */
        return _tr_str_lit("0");
    }
    /* pass */
    if ((d == 1LL)) {
        /* pass */
        return _tr_str_lit("1");
    }
    /* pass */
    if ((d == 2LL)) {
        /* pass */
        return _tr_str_lit("2");
    }
    /* pass */
    if ((d == 3LL)) {
        /* pass */
        return _tr_str_lit("3");
    }
    /* pass */
    if ((d == 4LL)) {
        /* pass */
        return _tr_str_lit("4");
    }
    /* pass */
    if ((d == 5LL)) {
        /* pass */
        return _tr_str_lit("5");
    }
    /* pass */
    if ((d == 6LL)) {
        /* pass */
        return _tr_str_lit("6");
    }
    /* pass */
    if ((d == 7LL)) {
        /* pass */
        return _tr_str_lit("7");
    }
    /* pass */
    if ((d == 8LL)) {
        /* pass */
        return _tr_str_lit("8");
    }
    /* pass */
    return _tr_str_lit("9");
}

__attribute__((hot)) TrStr _lir_itoa(long long n) {
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return _tr_str_lit("0");
    }
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long x = n;
    /* pass */
    while ((x > 0LL)) {
        /* pass */
        TrStr _strtmp_t2248 = ({ TrStr _cl = (_lir_digit((x % 10LL))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(s)); _tr_str_release(_cl); _cres; });
        _tr_str_release(s);
        s = _strtmp_t2248;
        /* pass */
        x = (x / 10LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) long long _norm_bool(LFunc* lf, long long v) {
    /* pass */
    long long z = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(z, 0LL));
    /* pass */
    long long r = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(r, _tr_str_lit("!="), v, z));
    /* pass */
    return r;
}

__attribute__((hot)) long long _str_call0(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long restype) {
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, _tr_v_recv);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, restype);
    /* pass */
    return d;
}

__attribute__((hot)) long long _str_call1(LModule* m, LFunc* lf, TrStr sym, long long _tr_v_recv, long long arg, long long restype) {
    /* pass */
    LModule_add_extern(m, sym);
    /* pass */
    List_i64* a = (void*)List_i64_new();
    /* pass */
    List_i64_append(a, _tr_v_recv);
    /* pass */
    List_i64_append(a, arg);
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ICall(d, sym, a));
    /* pass */
    LFunc_set_vreg_type(lf, d, restype);
    /* pass */
    return d;
}

__attribute__((hot)) long long _lower_str_method(LModule* m, LFunc* lf, long long _tr_v_recv, TrStr method, List_ptr* margs) {
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("upper"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_upper"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_upper"), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lower"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_lower"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_lower"), _tr_v_recv, 1LL);
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("strip"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("trim"))) == 0)) && (margs->len == 0LL))) {
        /* pass */
        return _str_call0(m, lf, _tr_str_lit("_tr_rt_str_strip"), _tr_v_recv, 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("replace"))) == 0) && (margs->len == 2LL))) {
        /* pass */
        long long a0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((a0 < 0LL) || (LFunc_vreg_type(lf, a0) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long a1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 1LL)));
        /* pass */
        if (((a1 < 0LL) || (LFunc_vreg_type(lf, a1) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_str_replace"));
        /* pass */
        List_i64* ra = (void*)List_i64_new();
        /* pass */
        List_i64_append(ra, _tr_v_recv);
        /* pass */
        List_i64_append(ra, a0);
        /* pass */
        List_i64_append(ra, a1);
        /* pass */
        long long rd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit("_tr_rt_str_replace"), ra));
        /* pass */
        LFunc_set_vreg_type(lf, rd, 1LL);
        /* pass */
        return rd;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("find"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long fa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((fa < 0LL) || (LFunc_vreg_type(lf, fa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_find"), _tr_v_recv, fa, 0LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("starts_with"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long sa = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((sa < 0LL) || (LFunc_vreg_type(lf, sa) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_starts_with"), _tr_v_recv, sa, 4LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("ends_with"))) == 0) && (margs->len == 1LL))) {
        /* pass */
        long long ea = lower_expr(m, lf, ((HirExpr*)List_ptr_get(margs, 0LL)));
        /* pass */
        if (((ea < 0LL) || (LFunc_vreg_type(lf, ea) != 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        return _str_call1(m, lf, _tr_str_lit("_tr_rt_str_ends_with"), _tr_v_recv, ea, 4LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool _is_const_int(HirExpr* e) {
    /* pass */
    __auto_type _t2249 = (*e);
    if (_t2249.tag == HirExpr_ELitInt) {
        return true;
    } else if (_t2249.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t2249.data.EUnaryOp.op;
__auto_type sub = _t2249.data.EUnaryOp.expr;
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
            /* pass */
            __auto_type _t2250 = (*sub);
            if (_t2250.tag == HirExpr_ELitInt) {
                return true;
            } else if (1) {
                __auto_type _ = _t2250;
                return false;
            }
        }
        /* pass */
        return false;
    } else if (1) {
        __auto_type _ = _t2249;
        return false;
    }
}

__attribute__((hot)) long long _const_int_val(HirExpr* e) {
    /* pass */
    __auto_type _t2251 = (*e);
    if (_t2251.tag == HirExpr_ELitInt) {
        __auto_type v = _t2251.data.ELitInt.val;
        return v;
    } else if (_t2251.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t2251.data.EUnaryOp.op;
__auto_type sub = _t2251.data.EUnaryOp.expr;
        /* pass */
        __auto_type _t2252 = (*sub);
        if (_t2252.tag == HirExpr_ELitInt) {
            __auto_type v2 = _t2252.data.ELitInt.val;
            return (0LL - v2);
        } else if (1) {
            __auto_type _ = _t2252;
            return 0LL;
        }
    } else if (1) {
        __auto_type _ = _t2251;
        return 0LL;
    }
}

__attribute__((hot)) void _emit_add_const(LFunc* lf, TrStr name, long long delta) {
    /* pass */
    long long cur = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_ILoadVar(cur, name));
    /* pass */
    long long d = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IConst(d, delta));
    /* pass */
    long long inc = LFunc_new_vreg(lf);
    /* pass */
    LFunc_emit(lf, LInst_ctor_IBinOp(inc, _tr_str_lit("+"), cur, d));
    /* pass */
    LFunc_emit(lf, LInst_ctor_IStoreVar(name, inc));
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
    __auto_type _t2253 = (*e);
    if (_t2253.tag == HirExpr_ELitInt) {
        __auto_type v = _t2253.data.ELitInt.val;
        /* pass */
        long long d = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(d, v));
        /* pass */
        return d;
    } else if (_t2253.tag == HirExpr_ELitStr) {
        __auto_type sv = _t2253.data.ELitStr.val;
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
    } else if (_t2253.tag == HirExpr_ELitBool) {
        __auto_type bval = _t2253.data.ELitBool.val;
        /* pass */
        long long db = LFunc_new_vreg(lf);
        /* pass */
        long long bconst = 0LL;
        /* pass */
        if (bval) {
            /* pass */
            bconst = 1LL;
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IConst(db, bconst));
        /* pass */
        LFunc_set_vreg_type(lf, db, 4LL);
        /* pass */
        return db;
    } else if (_t2253.tag == HirExpr_EIdent) {
        __auto_type name = _t2253.data.EIdent.name;
        /* pass */
        if (((LFunc_var_index(lf, name) < 0LL) && LModule_is_global(m, name))) {
            /* pass */
            long long gd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ILoadGlobal(gd, LModule_global_index(m, name)));
            /* pass */
            LFunc_set_vreg_type(lf, gd, LModule_global_type(m, name));
            /* pass */
            return gd;
        }
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
    } else if (_t2253.tag == HirExpr_EIfElse) {
        __auto_type cond = _t2253.data.EIfElse.cond;
__auto_type then_e = _t2253.data.EIfElse.then_e;
__auto_type else_e = _t2253.data.EIfElse.else_e;
        /* pass */
        long long tcv = lower_expr(m, lf, cond);
        /* pass */
        if ((tcv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long tuid = LFunc_fresh_id(lf);
        /* pass */
        TrStr rname = ({ TrStr _cr = (_lir_itoa(tuid)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("__tern")), _cr.data); _tr_str_release(_cr); _cres; });
        /* pass */
        LFunc_add_var(lf, rname);
        /* pass */
        long long then_b = LFunc_new_block(lf);
        /* pass */
        long long else_b = LFunc_new_block(lf);
        /* pass */
        long long end_b = LFunc_new_block(lf);
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TCondBr(tcv, then_b, else_b));
        /* pass */
        LFunc_set_cur(lf, then_b);
        /* pass */
        long long tv = lower_expr(m, lf, then_e);
        /* pass */
        if ((tv < 0LL)) {
            /* pass */
            _tr_str_release(rname);
            return (-1LL);
        }
        /* pass */
        long long rtype = LFunc_vreg_type(lf, tv);
        /* pass */
        LFunc_set_var_type(lf, rname, rtype);
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(rname, tv));
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_b));
        /* pass */
        LFunc_set_cur(lf, else_b);
        /* pass */
        long long ev2 = lower_expr(m, lf, else_e);
        /* pass */
        if ((ev2 < 0LL)) {
            /* pass */
            _tr_str_release(rname);
            return (-1LL);
        }
        /* pass */
        LFunc_emit(lf, LInst_ctor_IStoreVar(rname, ev2));
        /* pass */
        LFunc_set_term(lf, LTerm_ctor_TBr(end_b));
        /* pass */
        LFunc_set_cur(lf, end_b);
        /* pass */
        long long trd = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ILoadVar(trd, rname));
        /* pass */
        LFunc_set_vreg_type(lf, trd, rtype);
        /* pass */
        _tr_str_release(rname);
        return trd;
    } else if (_t2253.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t2253.data.EUnaryOp.op;
__auto_type sub = _t2253.data.EUnaryOp.expr;
        /* pass */
        long long sv = lower_expr(m, lf, sub);
        /* pass */
        if ((sv < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long svt = LFunc_vreg_type(lf, sv);
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
            /* pass */
            if ((svt != 0LL)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long zn = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(zn, 0LL));
            /* pass */
            long long dneg = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(dneg, _tr_str_lit("-"), zn, sv));
            /* pass */
            return dneg;
        }
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("not"))) == 0)) {
            /* pass */
            if (((svt != 0LL) && (svt != 4LL))) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long zt = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(zt, 0LL));
            /* pass */
            long long dnot = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(dnot, _tr_str_lit("=="), sv, zt));
            /* pass */
            LFunc_set_vreg_type(lf, dnot, 4LL);
            /* pass */
            return dnot;
        }
        /* pass */
        return (-1LL);
    } else if (_t2253.tag == HirExpr_EBinOp) {
        __auto_type op = _t2253.data.EBinOp.op;
__auto_type l = _t2253.data.EBinOp.left;
__auto_type r = _t2253.data.EBinOp.right;
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
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("in"))) == 0)) {
            /* pass */
            if ((!_is_list_tag(bt))) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            long long want_e = _list_elem_tag(bt);
            /* pass */
            if ((at != want_e)) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            TrStr csym = _tr_str_lit("_tr_rt_list_contains_i64");
            /* pass */
            if ((want_e == 1LL)) {
                /* pass */
                TrStr _strtmp_t2254 = _tr_str_lit("_tr_rt_list_contains_str");
                _tr_str_release(csym);
                csym = _strtmp_t2254;
            }
            /* pass */
            LModule_add_extern(m, csym);
            /* pass */
            List_i64* cca = (void*)List_i64_new();
            /* pass */
            List_i64_append(cca, b);
            /* pass */
            List_i64_append(cca, a);
            /* pass */
            long long cd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cd, csym, cca));
            /* pass */
            LFunc_set_vreg_type(lf, cd, 4LL);
            /* pass */
            _tr_str_release(csym);
            return cd;
        }
        /* pass */
        if ((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0) && ((at == 1LL) || (bt == 1LL))) && ((at == 0LL) || (bt == 0LL)))) {
            /* pass */
            long long sreg = a;
            /* pass */
            long long nreg = b;
            /* pass */
            if ((at == 0LL)) {
                /* pass */
                sreg = b;
                /* pass */
                nreg = a;
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_repeat"));
            /* pass */
            List_i64* ra = (void*)List_i64_new();
            /* pass */
            List_i64_append(ra, sreg);
            /* pass */
            List_i64_append(ra, nreg);
            /* pass */
            long long rd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(rd, _tr_str_lit("_tr_rt_str_repeat"), ra));
            /* pass */
            LFunc_set_vreg_type(lf, rd, 1LL);
            /* pass */
            return rd;
        }
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
        if (((_is_cmp_op(op) && (at == 1LL)) && (bt == 1LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_cmp"));
            /* pass */
            List_i64* sca = (void*)List_i64_new();
            /* pass */
            List_i64_append(sca, a);
            /* pass */
            List_i64_append(sca, b);
            /* pass */
            long long cmpv = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(cmpv, _tr_str_lit("_tr_rt_str_cmp"), sca));
            /* pass */
            long long zc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IConst(zc, 0LL));
            /* pass */
            long long rc = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(rc, op, cmpv, zc));
            /* pass */
            LFunc_set_vreg_type(lf, rc, 4LL);
            /* pass */
            return rc;
        }
        /* pass */
        if (((at == 1LL) || (bt == 1LL))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if ((_is_list_tag(at) || _is_list_tag(bt))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("and"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("or"))) == 0))) {
            /* pass */
            long long na = _norm_bool(lf, a);
            /* pass */
            long long nb = _norm_bool(lf, b);
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("and"))) == 0)) {
                /* pass */
                long long dand = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_IBinOp(dand, _tr_str_lit("*"), na, nb));
                /* pass */
                LFunc_set_vreg_type(lf, dand, 4LL);
                /* pass */
                return dand;
            }
            /* pass */
            long long ssum = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_IBinOp(ssum, _tr_str_lit("+"), na, nb));
            /* pass */
            long long oro = _norm_bool(lf, ssum);
            /* pass */
            LFunc_set_vreg_type(lf, oro, 4LL);
            /* pass */
            return oro;
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
        if (_is_cmp_op(op)) {
            /* pass */
            LFunc_set_vreg_type(lf, d3, 4LL);
        }
        /* pass */
        return d3;
    } else if (_t2253.tag == HirExpr_ECall) {
        __auto_type callee = _t2253.data.ECall.callee;
__auto_type args = _t2253.data.ECall.args;
        /* pass */
        TrStr fn = _ident_name(callee);
        /* pass */
        if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("len"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long xv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((xv < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long xt = LFunc_vreg_type(lf, xv);
            /* pass */
            if (_is_list_tag(xt)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit("_tr_rt_list_len"));
                /* pass */
                List_i64* lla = (void*)List_i64_new();
                /* pass */
                List_i64_append(lla, xv);
                /* pass */
                long long lld = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(lld, _tr_str_lit("_tr_rt_list_len"), lla));
                /* pass */
                _tr_str_release(fn);
                return lld;
            }
            /* pass */
            if ((xt == 1LL)) {
                /* pass */
                LModule_add_extern(m, _tr_str_lit("_tr_rt_strlen"));
                /* pass */
                List_i64* sla = (void*)List_i64_new();
                /* pass */
                List_i64_append(sla, xv);
                /* pass */
                long long sld = LFunc_new_vreg(lf);
                /* pass */
                LFunc_emit(lf, LInst_ctor_ICall(sld, _tr_str_lit("_tr_rt_strlen"), sla));
                /* pass */
                _tr_str_release(fn);
                return sld;
            }
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("abs"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long xv2 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((xv2 < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, xv2) != 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_abs_i64"));
            /* pass */
            List_i64* aba = (void*)List_i64_new();
            /* pass */
            List_i64_append(aba, xv2);
            /* pass */
            long long abd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(abd, _tr_str_lit("_tr_rt_abs_i64"), aba));
            /* pass */
            _tr_str_release(fn);
            return abd;
        }
        /* pass */
        if ((((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("min"))) == 0) || (strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("max"))) == 0)) && (args->len == 2LL))) {
            /* pass */
            long long mm1 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((mm1 < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long mm2 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 1LL)));
            /* pass */
            if ((mm2 < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if (((LFunc_vreg_type(lf, mm1) != 0LL) || (LFunc_vreg_type(lf, mm2) != 0LL))) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            TrStr msym = _tr_str_lit("_tr_rt_min_i64");
            /* pass */
            if ((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("max"))) == 0)) {
                /* pass */
                TrStr _strtmp_t2255 = _tr_str_lit("_tr_rt_max_i64");
                _tr_str_release(msym);
                msym = _strtmp_t2255;
            }
            /* pass */
            LModule_add_extern(m, msym);
            /* pass */
            List_i64* mma = (void*)List_i64_new();
            /* pass */
            List_i64_append(mma, mm1);
            /* pass */
            List_i64_append(mma, mm2);
            /* pass */
            long long mmd = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(mmd, msym, mma));
            /* pass */
            _tr_str_release(fn);
            _tr_str_release(msym);
            return mmd;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("str"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long cv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((cv0 < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long cvt = LFunc_vreg_type(lf, cv0);
            /* pass */
            if ((cvt == 1LL)) {
                /* pass */
                _tr_str_release(fn);
                return cv0;
            }
            /* pass */
            TrStr ssym = _tr_str_lit("_tr_rt_i64_to_str");
            /* pass */
            if ((cvt == 4LL)) {
                /* pass */
                TrStr _strtmp_t2256 = _tr_str_lit("_tr_rt_bool_to_str");
                _tr_str_release(ssym);
                ssym = _strtmp_t2256;
            } else if ((cvt != 0LL)) {
                /* pass */
                _tr_str_release(fn);
                _tr_str_release(ssym);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, ssym);
            /* pass */
            List_i64* s2a = (void*)List_i64_new();
            /* pass */
            List_i64_append(s2a, cv0);
            /* pass */
            long long s2d = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(s2d, ssym, s2a));
            /* pass */
            LFunc_set_vreg_type(lf, s2d, 1LL);
            /* pass */
            _tr_str_release(fn);
            _tr_str_release(ssym);
            return s2d;
        }
        /* pass */
        if ((((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("_tr_fn_int"))) == 0)) && (args->len == 1LL))) {
            /* pass */
            long long iv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((iv0 < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long ivt = LFunc_vreg_type(lf, iv0);
            /* pass */
            if ((ivt == 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return iv0;
            }
            /* pass */
            if ((ivt == 4LL)) {
                /* pass */
                LFunc_set_vreg_type(lf, iv0, 0LL);
                /* pass */
                _tr_str_release(fn);
                return iv0;
            }
            /* pass */
            if ((ivt != 1LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_str_to_i64"));
            /* pass */
            List_i64* i2a = (void*)List_i64_new();
            /* pass */
            List_i64_append(i2a, iv0);
            /* pass */
            long long i2d = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(i2d, _tr_str_lit("_tr_rt_str_to_i64"), i2a));
            /* pass */
            _tr_str_release(fn);
            return i2d;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("sum"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long suv = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((suv < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if ((LFunc_vreg_type(lf, suv) != 2LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_sum_i64"));
            /* pass */
            List_i64* sua = (void*)List_i64_new();
            /* pass */
            List_i64_append(sua, suv);
            /* pass */
            long long sud = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(sud, _tr_str_lit("_tr_rt_list_sum_i64"), sua));
            /* pass */
            _tr_str_release(fn);
            return sud;
        }
        /* pass */
        if (((strcmp(_tr_strz(fn), _tr_strz(_tr_str_lit("bool"))) == 0) && (args->len == 1LL))) {
            /* pass */
            long long bv0 = lower_expr(m, lf, ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            if ((bv0 < 0LL)) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            if (_is_list_tag(LFunc_vreg_type(lf, bv0))) {
                /* pass */
                _tr_str_release(fn);
                return (-1LL);
            }
            /* pass */
            long long bnorm = _norm_bool(lf, bv0);
            /* pass */
            LFunc_set_vreg_type(lf, bnorm, 4LL);
            /* pass */
            _tr_str_release(fn);
            return bnorm;
        }
        /* pass */
        if ((!LModule_is_user_fn(m, fn))) {
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        if ((args->len > 6LL)) {
            /* pass */
            _tr_str_release(fn);
            return (-1LL);
        }
        /* pass */
        long long rtag = LModule_fn_ret_tag(m, fn);
        /* pass */
        if ((rtag < 0LL)) {
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
        LFunc_set_vreg_type(lf, d4, rtag);
        /* pass */
        _tr_str_release(fn);
        return d4;
    } else if (_t2253.tag == HirExpr_EList) {
        __auto_type items = _t2253.data.EList.items;
        /* pass */
        LModule_add_extern(m, _tr_str_lit("_tr_rt_list_new"));
        /* pass */
        long long hv = LFunc_new_vreg(lf);
        /* pass */
        LFunc_emit(lf, LInst_ctor_ICall(hv, _tr_str_lit("_tr_rt_list_new"), (void*)List_i64_new()));
        /* pass */
        long long elem_t = 0LL;
        /* pass */
        long long list_tag = 2LL;
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
            long long evt = LFunc_vreg_type(lf, ev);
            /* pass */
            if (((evt != 0LL) && (evt != 1LL))) {
                /* pass */
                return (-1LL);
            }
            /* pass */
            if ((li == 0LL)) {
                /* pass */
                elem_t = evt;
                /* pass */
                list_tag = _list_tag_for_elem(evt);
            } else if ((evt != elem_t)) {
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
        LFunc_set_vreg_type(lf, hv, list_tag);
        /* pass */
        return hv;
    } else if (_t2253.tag == HirExpr_EIndex) {
        __auto_type obj = _t2253.data.EIndex.obj;
__auto_type idx = _t2253.data.EIndex._tr_v_index;
        /* pass */
        long long ov = lower_expr(m, lf, obj);
        /* pass */
        if ((ov < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long ovt = LFunc_vreg_type(lf, ov);
        /* pass */
        if ((!_is_list_tag(ovt))) {
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
        long long gval = _list_get(m, lf, ov, iv);
        /* pass */
        LFunc_set_vreg_type(lf, gval, _list_elem_tag(ovt));
        /* pass */
        return gval;
    } else if (_t2253.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t2253.data.EPropAccess.obj;
__auto_type prop = _t2253.data.EPropAccess.prop;
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
        if ((!_is_list_tag(LFunc_vreg_type(lf, ovl)))) {
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
    } else if (_t2253.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t2253.data.EMethodCall.obj;
__auto_type method = _t2253.data.EMethodCall.method;
__auto_type margs = _t2253.data.EMethodCall.args;
        /* pass */
        long long ovm = lower_expr(m, lf, obj);
        /* pass */
        if ((ovm < 0LL)) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long ovmt = LFunc_vreg_type(lf, ovm);
        /* pass */
        if ((ovmt == 1LL)) {
            /* pass */
            return _lower_str_method(m, lf, ovm, method, margs);
        }
        /* pass */
        if ((!_is_list_tag(ovmt))) {
            /* pass */
            return (-1LL);
        }
        /* pass */
        long long want_elem = _list_elem_tag(ovmt);
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
            if ((LFunc_vreg_type(lf, av) != want_elem)) {
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
            long long gv2 = _list_get(m, lf, ovm, giv);
            /* pass */
            LFunc_set_vreg_type(lf, gv2, want_elem);
            /* pass */
            return gv2;
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pop"))) == 0) && (margs->len == 0LL))) {
            /* pass */
            LModule_add_extern(m, _tr_str_lit("_tr_rt_list_pop_i64"));
            /* pass */
            List_i64* poa = (void*)List_i64_new();
            /* pass */
            List_i64_append(poa, ovm);
            /* pass */
            long long pod = LFunc_new_vreg(lf);
            /* pass */
            LFunc_emit(lf, LInst_ctor_ICall(pod, _tr_str_lit("_tr_rt_list_pop_i64"), poa));
            /* pass */
            LFunc_set_vreg_type(lf, pod, want_elem);
            /* pass */
            return pod;
        }
        /* pass */
        return (-1LL);
    } else if (1) {
        __auto_type _ = _t2253;
        /* pass */
        return (-1LL);
    }
}

