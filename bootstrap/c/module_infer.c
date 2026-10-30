#include "tauraro_types.h"

bool _ib_has_value_return(Block* b);
bool _ipb_has_value_return(Block** pb);
bool _is_has_value_return(Stmt* sp);
bool _i_candidate(FunctionDef* f);
void _i_collect(Program* prog, List_ptr* fns, List_TrStr* files, List_TrStr* decl_files, TrStr input_path);
AstType** _i_box(AstType* t);

__attribute__((hot)) bool _ib_has_value_return(Block* b) {
#line 18 "src/infer.tr"
    long long i = 0LL;
#line 19 "src/infer.tr"
    while ((i < b->stmts->len)) {
#line 20 "src/infer.tr"
        if (_is_has_value_return(((Stmt*)List_ptr_get(b->stmts, i)))) {
#line 20 "src/infer.tr"
            return true;
        }
#line 21 "src/infer.tr"
        i = (i + 1LL);
    }
#line 22 "src/infer.tr"
    return false;
}

__attribute__((hot)) bool _ipb_has_value_return(Block** pb) {
#line 25 "src/infer.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 25 "src/infer.tr"
        return false;
    }
#line 26 "src/infer.tr"
    return _ib_has_value_return((*pb));
}

__attribute__((hot)) bool _is_has_value_return(Stmt* sp) {
#line 29 "src/infer.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 29 "src/infer.tr"
        return false;
    }
#line 30 "src/infer.tr"
    __auto_type _t2538 = (*sp);
    if (_t2538.tag == Stmt_SReturn) {
        __auto_type v = _t2538.data.SReturn.val;
#line 32 "src/infer.tr"
        return (((unsigned long long)(v)) != ((unsigned long long)(0LL)));
    } else if (_t2538.tag == Stmt_SIf) {
        __auto_type tb = _t2538.data.SIf.then_b;
__auto_type elifs = _t2538.data.SIf.elifs;
__auto_type eb = _t2538.data.SIf.else_b;
#line 34 "src/infer.tr"
        if ((_ib_has_value_return(tb) || _ib_has_value_return(eb))) {
#line 34 "src/infer.tr"
            return true;
        }
#line 35 "src/infer.tr"
        long long ei = 0LL;
#line 36 "src/infer.tr"
        while ((ei < elifs->len)) {
#line 37 "src/infer.tr"
            if (_ipb_has_value_return(((ElifClause*)List_ptr_get(elifs, ei))->body)) {
#line 37 "src/infer.tr"
                return true;
            }
#line 38 "src/infer.tr"
            ei = (ei + 1LL);
        }
#line 39 "src/infer.tr"
        return false;
    } else if (_t2538.tag == Stmt_SWhile) {
        __auto_type wb = _t2538.data.SWhile.body;
        bool _trret502 = _ib_has_value_return(wb);
        return _trret502;
    } else if (_t2538.tag == Stmt_SFor) {
        __auto_type fb = _t2538.data.SFor.body;
        bool _trret503 = _ib_has_value_return(fb);
        return _trret503;
    } else if (_t2538.tag == Stmt_SForUnpack) {
        __auto_type fub = _t2538.data.SForUnpack.body;
        bool _trret504 = _ib_has_value_return(fub);
        return _trret504;
    } else if (_t2538.tag == Stmt_SUnsafe) {
        __auto_type ub = _t2538.data.SUnsafe.body;
        bool _trret505 = _ib_has_value_return(ub);
        return _trret505;
    } else if (_t2538.tag == Stmt_SWith) {
        __auto_type wb2 = _t2538.data.SWith.body;
        bool _trret506 = _ib_has_value_return(wb2);
        return _trret506;
    } else if (_t2538.tag == Stmt_STaskGroup) {
        __auto_type tg = _t2538.data.STaskGroup.body;
        bool _trret507 = _ib_has_value_return(tg);
        return _trret507;
    } else if (_t2538.tag == Stmt_SDefer) {
        __auto_type ds = _t2538.data.SDefer.stmt;
        return _is_has_value_return(ds);
    } else if (_t2538.tag == Stmt_SMatch) {
        __auto_type arms = _t2538.data.SMatch.arms;
#line 48 "src/infer.tr"
        long long ai = 0LL;
#line 49 "src/infer.tr"
        while ((ai < arms->len)) {
#line 50 "src/infer.tr"
            if (_ipb_has_value_return(((MatchArm*)List_ptr_get(arms, ai))->body)) {
#line 50 "src/infer.tr"
                return true;
            }
#line 51 "src/infer.tr"
            ai = (ai + 1LL);
        }
#line 52 "src/infer.tr"
        return false;
    } else if (_t2538.tag == Stmt_STry) {
        __auto_type tb2 = _t2538.data.STry.try_body;
__auto_type catches = _t2538.data.STry.catches;
__auto_type fb2 = _t2538.data.STry.finally_b;
#line 54 "src/infer.tr"
        if ((_ib_has_value_return(tb2) || _ib_has_value_return(fb2))) {
#line 54 "src/infer.tr"
            return true;
        }
#line 55 "src/infer.tr"
        long long ci = 0LL;
#line 56 "src/infer.tr"
        while ((ci < catches->len)) {
#line 57 "src/infer.tr"
            if (_ipb_has_value_return((*((CatchClause**)List_ptr_get(catches, ci)))->body)) {
#line 57 "src/infer.tr"
                return true;
            }
#line 58 "src/infer.tr"
            ci = (ci + 1LL);
        }
#line 59 "src/infer.tr"
        return false;
    } else if (1) {
        __auto_type _ = _t2538;
        return false;
    }
#line 61 "src/infer.tr"
    return false;
}

__attribute__((hot)) bool _i_candidate(FunctionDef* f) {
#line 64 "src/infer.tr"
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
#line 64 "src/infer.tr"
        return false;
    }
#line 65 "src/infer.tr"
    if ((f->is_extern || f->is_macro)) {
#line 65 "src/infer.tr"
        return false;
    }
#line 66 "src/infer.tr"
    if (((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL))) && (!_tr_str_eqv(((*f->throws_ty)->name), (_tr_str_lit_len("", 0LL)))))) {
#line 66 "src/infer.tr"
        return false;
    }
#line 67 "src/infer.tr"
    return _ib_has_value_return(f->body);
}

__attribute__((hot)) void _i_collect(Program* prog, List_ptr* fns, List_TrStr* files, List_TrStr* decl_files, TrStr input_path) {
#line 70 "src/infer.tr"
    long long i = 0LL;
#line 71 "src/infer.tr"
    while ((i < prog->decls->len)) {
#line 72 "src/infer.tr"
        TrStr df = _tr_str_retain(input_path);
#line 73 "src/infer.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 73 "src/infer.tr"
            TrStr _strtmp_t2539 = List_TrStr_get(decl_files, i);
            _tr_str_release(df);
            df = _strtmp_t2539;
        }
#line 74 "src/infer.tr"
        __auto_type _t2540 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t2540.tag == Decl_DFunction) {
            __auto_type f = _t2540.data.DFunction.func;
#line 76 "src/infer.tr"
            if (_i_candidate(f)) {
#line 77 "src/infer.tr"
                List_ptr_append(fns, _tr_obj_retain(f));
#line 78 "src/infer.tr"
                List_TrStr_append(files, df);
            }
        } else if (_t2540.tag == Decl_DClass) {
            __auto_type c = _t2540.data.DClass.cls;
#line 80 "src/infer.tr"
            long long mi = 0LL;
#line 81 "src/infer.tr"
            while ((mi < c->methods->len)) {
#line 82 "src/infer.tr"
                if (_i_candidate(((FunctionDef*)List_ptr_get(c->methods, mi)))) {
#line 83 "src/infer.tr"
                    List_ptr_append(fns, _tr_obj_retain(((FunctionDef*)List_ptr_get(c->methods, mi))));
#line 84 "src/infer.tr"
                    List_TrStr_append(files, df);
                }
#line 85 "src/infer.tr"
                mi = (mi + 1LL);
            }
        } else if (_t2540.tag == Decl_DActor) {
            __auto_type c2 = _t2540.data.DActor.cls;
#line 87 "src/infer.tr"
            long long mi2 = 0LL;
#line 88 "src/infer.tr"
            while ((mi2 < c2->methods->len)) {
#line 89 "src/infer.tr"
                if (_i_candidate(((FunctionDef*)List_ptr_get(c2->methods, mi2)))) {
#line 90 "src/infer.tr"
                    List_ptr_append(fns, _tr_obj_retain(((FunctionDef*)List_ptr_get(c2->methods, mi2))));
#line 91 "src/infer.tr"
                    List_TrStr_append(files, df);
                }
#line 92 "src/infer.tr"
                mi2 = (mi2 + 1LL);
            }
        } else if (_t2540.tag == Decl_DExtend) {
            __auto_type methods = _t2540.data.DExtend.methods;
#line 94 "src/infer.tr"
            long long mi3 = 0LL;
#line 95 "src/infer.tr"
            while ((mi3 < methods->len)) {
#line 96 "src/infer.tr"
                if (_i_candidate(((FunctionDef*)List_ptr_get(methods, mi3)))) {
#line 97 "src/infer.tr"
                    List_ptr_append(fns, _tr_obj_retain(((FunctionDef*)List_ptr_get(methods, mi3))));
#line 98 "src/infer.tr"
                    List_TrStr_append(files, df);
                }
#line 99 "src/infer.tr"
                mi3 = (mi3 + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t2540;
            /* pass */
        }
#line 101 "src/infer.tr"
        i = (i + 1LL);
        _tr_str_release(df);
    }
}

__attribute__((hot)) AstType** _i_box(AstType* t) {
#line 104 "src/infer.tr"
    /* unsafe block */
#line 105 "src/infer.tr"
    AstType** p = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
#line 106 "src/infer.tr"
    (*p = t);
#line 107 "src/infer.tr"
    return p;
}

__attribute__((hot)) List_TrStr* infer_return_types(Program* prog, List_TrStr* decl_files, bool strict_mode, bool no_heap, bool boxes, TrStr input_path) {
#line 111 "src/infer.tr"
    List_TrStr* msgs = (void*)List_TrStr_new();
#line 112 "src/infer.tr"
    List_ptr* fns = (void*)List_ptr_new();
#line 113 "src/infer.tr"
    List_TrStr* files = (void*)List_TrStr_new();
#line 114 "src/infer.tr"
    _i_collect(prog, fns, files, decl_files, input_path);
#line 115 "src/infer.tr"
    if ((fns->len == 0LL)) {
#line 115 "src/infer.tr"
        return msgs;
    }
#line 116 "src/infer.tr"
    List_i64* done = (void*)List_i64_new();
#line 117 "src/infer.tr"
    List_bool* tentative = (void*)List_bool_new();
#line 118 "src/infer.tr"
    long long k = 0LL;
#line 119 "src/infer.tr"
    while ((k < fns->len)) {
#line 120 "src/infer.tr"
        List_i64_append(done, 0LL);
#line 121 "src/infer.tr"
        List_bool_append(tentative, false);
#line 122 "src/infer.tr"
        k = (k + 1LL);
    }
#line 123 "src/infer.tr"
    long long round = 0LL;
#line 124 "src/infer.tr"
    bool changed = true;
#line 125 "src/infer.tr"
    while ((changed && (round < 16LL))) {
#line 126 "src/infer.tr"
        changed = false;
#line 127 "src/infer.tr"
        round = (round + 1LL);
#line 129 "src/infer.tr"
        long long m = 0LL;
#line 130 "src/infer.tr"
        while ((m < fns->len)) {
#line 131 "src/infer.tr"
            if (((List_i64_get(done, m) == 0LL) && (((unsigned long long)(((FunctionDef*)List_ptr_get(fns, m))->ret_ty)) == ((unsigned long long)(0LL))))) {
#line 132 "src/infer.tr"
                __auto_type _asg_t2541 = _i_box(AstType_init(_tr_str_lit_len("__unk", 5LL)));
                ((FunctionDef*)List_ptr_get(fns, m))->ret_ty = _asg_t2541;
            }
#line 133 "src/infer.tr"
            m = (m + 1LL);
        }
#line 135 "src/infer.tr"
        List_ptr* cls_defs = (void*)List_ptr_new();
#line 136 "src/infer.tr"
        List_i64* cls_lens = (void*)List_i64_new();
#line 137 "src/infer.tr"
        long long ci = 0LL;
#line 138 "src/infer.tr"
        while ((ci < prog->decls->len)) {
#line 139 "src/infer.tr"
            __auto_type _t2542 = (*((Decl*)List_ptr_get(prog->decls, ci)));
            if (_t2542.tag == Decl_DClass) {
                __auto_type cd = _t2542.data.DClass.cls;
#line 141 "src/infer.tr"
                List_ptr_append(cls_defs, _tr_obj_retain(cd));
#line 142 "src/infer.tr"
                List_i64_append(cls_lens, cd->methods->len);
            } else if (_t2542.tag == Decl_DActor) {
                __auto_type cd2 = _t2542.data.DActor.cls;
#line 144 "src/infer.tr"
                List_ptr_append(cls_defs, _tr_obj_retain(cd2));
#line 145 "src/infer.tr"
                List_i64_append(cls_lens, cd2->methods->len);
            } else if (1) {
                __auto_type _ = _t2542;
                /* pass */
            }
#line 147 "src/infer.tr"
            ci = (ci + 1LL);
        }
#line 148 "src/infer.tr"
        Sema* sema = Sema_init();
#line 149 "src/infer.tr"
        sema->strict_mode = strict_mode;
#line 150 "src/infer.tr"
        sema->no_heap = no_heap;
#line 151 "src/infer.tr"
        sema->heap_boxes_tuples = boxes;
#line 152 "src/infer.tr"
        sema->current_file = _tr_str_retain(input_path);
#line 153 "src/infer.tr"
        sema->infer_mode = true;
#line 154 "src/infer.tr"
        long long dfi = 0LL;
#line 155 "src/infer.tr"
        while ((dfi < decl_files->len)) {
#line 156 "src/infer.tr"
            ({ TrStr _ord_t2543 = (List_TrStr_get(decl_files, dfi)); List_TrStr_append(sema->decl_files, _ord_t2543); _tr_str_release(_ord_t2543); });
#line 157 "src/infer.tr"
            dfi = (dfi + 1LL);
        }
#line 158 "src/infer.tr"
        Sema_analyze(sema, prog);
#line 159 "src/infer.tr"
        ci = 0LL;
#line 160 "src/infer.tr"
        while ((ci < cls_defs->len)) {
#line 161 "src/infer.tr"
            while (({ __auto_type _ord_t2544 = (((ClassDef*)List_ptr_get(cls_defs, ci))->methods->len); __auto_type _wr = ((_ord_t2544 > List_i64_get(cls_lens, ci))); _wr; })) {
#line 161 "src/infer.tr"
                ((FunctionDef*)List_ptr_pop(((ClassDef*)List_ptr_get(cls_defs, ci))->methods));
            }
#line 162 "src/infer.tr"
            ci = (ci + 1LL);
        }
#line 164 "src/infer.tr"
        m = 0LL;
#line 165 "src/infer.tr"
        while ((m < fns->len)) {
#line 166 "src/infer.tr"
            if (((((unsigned long long)(((FunctionDef*)List_ptr_get(fns, m))->ret_ty)) != ((unsigned long long)(0LL))) && _tr_str_eqv(((*((FunctionDef*)List_ptr_get(fns, m))->ret_ty)->name), (_tr_str_lit_len("__unk", 5LL))))) {
#line 167 "src/infer.tr"
                __auto_type _asg_t2545 = (AstType**)(0LL);
                ((FunctionDef*)List_ptr_get(fns, m))->ret_ty = _asg_t2545;
            }
#line 168 "src/infer.tr"
            m = (m + 1LL);
        }
#line 169 "src/infer.tr"
        long long r = 0LL;
#line 170 "src/infer.tr"
        while ((r < sema->infer_res_fn->len)) {
#line 171 "src/infer.tr"
            FunctionDef* rf = ((FunctionDef*)List_ptr_get(sema->infer_res_fn, r));
#line 172 "src/infer.tr"
            long long st = List_i64_get(sema->infer_res_state, r);
#line 173 "src/infer.tr"
            long long idx = (0LL - 1LL);
#line 174 "src/infer.tr"
            long long q = 0LL;
#line 175 "src/infer.tr"
            while ((q < fns->len)) {
#line 176 "src/infer.tr"
                if ((((unsigned long long)(((FunctionDef*)List_ptr_get(fns, q)))) == ((unsigned long long)(rf)))) {
#line 176 "src/infer.tr"
                    idx = q;
                }
#line 177 "src/infer.tr"
                q = (q + 1LL);
            }
#line 178 "src/infer.tr"
            if (((idx >= 0LL) && (List_i64_get(done, idx) == 0LL))) {
#line 179 "src/infer.tr"
                if (((st == 0LL) || (st == 1LL))) {
#line 180 "src/infer.tr"
                    __auto_type _asg_t2546 = ((AstType**)List_ptr_get(sema->infer_res_ty, r));
                    ((FunctionDef*)List_ptr_get(fns, idx))->ret_ty = _asg_t2546;
#line 181 "src/infer.tr"
                    List_i64_set(done, idx, 1LL);
#line 182 "src/infer.tr"
                    changed = true;
                } else if ((st == 4LL)) {
#line 184 "src/infer.tr"
                    List_i64_set(done, idx, 1LL);
#line 185 "src/infer.tr"
                    changed = true;
                } else if ((st == 3LL)) {
#line 187 "src/infer.tr"
                    List_i64_set(done, idx, 2LL);
#line 188 "src/infer.tr"
                    changed = true;
#line 189 "src/infer.tr"
                    ({ __auto_type _ord_t2547 = (({ TrStr _tr_cl = (List_TrStr_get(files, idx)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2548 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2547); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(((FunctionDef*)List_ptr_get(fns, idx))->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [R-2] the returns of '", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2549 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2548); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' have different types.\n      FIX: make every `return` yield the same type, or write the return type: `def ", 107LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2550 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2549); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...) -> T:`", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(msgs, _at_t2550); _tr_str_release(_at_t2550); });
                }
            }
#line 190 "src/infer.tr"
            r = (r + 1LL);
        }
        List_ptr_free_obj(cls_defs, _trdrop_ClassDef);
        List_i64_free(cls_lens);
        _tr_obj_release(sema, _trdrop_Sema);
    }
#line 191 "src/infer.tr"
    long long u = 0LL;
#line 192 "src/infer.tr"
    while ((u < fns->len)) {
#line 193 "src/infer.tr"
        if ((List_i64_get(done, u) == 0LL)) {
#line 194 "src/infer.tr"
            ({ __auto_type _ord_t2551 = (({ TrStr _tr_cl = (List_TrStr_get(files, u)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2552 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2551); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(((FunctionDef*)List_ptr_get(fns, u))->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [R-2] cannot infer the return type of '", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2553 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2552); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((FunctionDef*)List_ptr_get(fns, u))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (it depends on itself or on a value whose type is not known).\n      FIX: write the return type: `def ", 103LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t2554 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2553); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((FunctionDef*)List_ptr_get(fns, u))->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(...) -> T:`", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(msgs, _at_t2554); _tr_str_release(_at_t2554); });
        }
#line 195 "src/infer.tr"
        u = (u + 1LL);
    }
#line 196 "src/infer.tr"
    List_i64_free(done);
    List_bool_free(tentative);
    return msgs;
}

