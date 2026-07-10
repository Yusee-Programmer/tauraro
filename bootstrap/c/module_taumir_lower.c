#include "tauraro_types.h"

TrStr _print_i64_sym();
void lower_main(LModule* m, HirFunction* f);
bool lower_stmt(LModule* m, LFunc* lf, HirStmt* s);
bool lower_print(LModule* m, LFunc* lf, HirExpr* e);

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
        HirStmt* s = ((HirStmt*)List_ptr_get(f->body->stmts, si));
        /* pass */
        if ((!lower_stmt(m, lf, s))) {
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
    if (_t2238.tag == HirStmt_SExpr) {
        __auto_type e = _t2238.data.SExpr.expr;
        /* pass */
        return lower_print(m, lf, e);
    } else if (_t2238.tag == HirStmt_SReturn) {
        __auto_type _ = _t2238.data.SReturn.val;
        /* pass */
        return true;
    } else if (_t2238.tag == HirStmt_SLineMarker) {
        __auto_type _ = _t2238.data.SLineMarker.n;
        /* pass */
        return true;
    } else if (_t2238.tag == HirStmt_SPass) {
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t2238;
        /* pass */
        return false;
    }
}

__attribute__((hot)) bool lower_print(LModule* m, LFunc* lf, HirExpr* e) {
    /* pass */
    __auto_type _t2239 = (*e);
    if (_t2239.tag == HirExpr_ECall) {
        __auto_type callee = _t2239.data.ECall.callee;
__auto_type args = _t2239.data.ECall.args;
        /* pass */
        bool is_print = false;
        /* pass */
        __auto_type _t2240 = (*callee);
        if (_t2240.tag == HirExpr_EIdent) {
            __auto_type n = _t2240.data.EIdent.name;
            /* pass */
            if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("print"))) == 0)) {
                /* pass */
                is_print = true;
            }
        } else if (1) {
            __auto_type _ = _t2240;
            /* pass */
        }
        /* pass */
        if ((!is_print)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((args->len != 1LL)) {
            /* pass */
            return false;
        }
        /* pass */
        __auto_type _t2241 = (*((HirExpr*)List_ptr_get(args, 0LL)));
        if (_t2241.tag == HirExpr_ELitInt) {
            __auto_type v = _t2241.data.ELitInt.val;
            /* pass */
            TrStr callee_sym = _print_i64_sym();
            /* pass */
            LModule_add_extern(m, callee_sym);
            /* pass */
            List_ptr* cargs = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(cargs, box_lval(LVal_ctor_VConst(v)));
            /* pass */
            List_ptr_append(lf->block->insts, box_linst(LInst_ctor_ICall((-1LL), callee_sym, cargs)));
            /* pass */
            _tr_str_release(callee_sym);
            return true;
        } else if (1) {
            __auto_type _ = _t2241;
            /* pass */
            return false;
        }
    } else if (1) {
        __auto_type _ = _t2239;
        /* pass */
        return false;
    }
}

