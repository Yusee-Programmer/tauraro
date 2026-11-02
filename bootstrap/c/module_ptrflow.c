#include "tauraro_types.h"

bool pf_has(List_TrStr* v, TrStr s);
void pf_add(List_TrStr* v, TrStr s);
bool pf_under(TrStr p, TrStr n);
List_TrStr* pf_vcopy(List_TrStr* v);
List_TrStr* pf_inter(List_TrStr* a, List_TrStr* b);
void pf_assign_from(PfState* into, PfState* o);
bool pf_has_pair(PfState* st, TrStr a, TrStr b);
void pf_merge(PfState* into, PfState* o);
bool pf_null(HirExpr* e);
TrStr pf_path(HirExpr* e);
TrStr pf_root(HirExpr* e);
bool pf_is_dealloc(HirExpr* callee);
TrStr pf_callee_name(HirExpr* callee);
TrStr pf_stack_src(HirExpr* e, PfState* st, PfCtx* cx);
long long pf_lit(HirExpr* e);
bool pf_is_lit(HirExpr* e);
bool pf_named_call(HirExpr* callee, TrStr want);
bool pf_is_alloc(HirExpr* val);
long long pf_alloc_n(HirExpr* val);
bool pf_is_null_lit(HirExpr* val);
long long pf_size_of(PfState* st, TrStr path);
void pf_kill(PfState* st, TrStr path);
List_TrStr* pf_drop_under(List_TrStr* v, TrStr path);
void pf_kill_fields(PfState* st, TrStr path);
void pf_link(PfState* st, TrStr a, TrStr b);
void pf_free(PfState* st, TrStr path);
TrStr pf_is_freed(PfState* st, TrStr path);
bool pf_zeroish(HirExpr* e);
TrStr pf_nulltest(HirExpr* cond);
void pf_refine(PfState* st, TrStr test, bool taken);
void pf_block(PfCtx* cx, HirBlock* b, PfState* st);
void pf_use_msg(PfCtx* cx, TrStr name);
void pf_exprs(PfCtx* cx, List_ptr* v, PfState* st);
void pf_branchy_block(PfCtx* cx, HirBlock* b, PfState* st);
void pf_ptr_checks(PfCtx* cx, PfState* st, HirExpr* o, TrStr m, List_ptr* args);
void pf_expr(PfCtx* cx, HirExpr* e, PfState* st);
void pf_leak_warn(PfCtx* cx, TrStr name, TrStr why);
void pf_leak_check(PfCtx* cx, PfState* st, TrStr why);
void pf_bind(PfCtx* cx, PfState* st, TrStr path, HirExpr* val, bool simple);
void pf_declare(PfCtx* cx, PfState* st, TrStr name, HirExpr* val);
void pf_assign(PfCtx* cx, PfState* st, HirExpr* tgt, HirExpr* val);
void pf_exit_loop_ctx(PfCtx* cx, PfState* brk_out, PfState* cnt_out);
void pf_loop(PfCtx* cx, PfState* st, HirExpr* cond, TrStr lv, HirBlock* body, bool always);
void pf_stmt(PfCtx* cx, HirStmt* sp, PfState* st);
List_i64* pf_run_fn(HirFunction* hf, TrMap* summaries, TrMap* externs, bool report, List_TrStr* errs);

__attribute__((malloc,returns_nonnull,hot)) PfState* PfState_init() {
#line 35 "src/ptrflow.tr"
    PfState* s = ((PfState*)_tr_obj_alloc(sizeof(PfState)));
#line 36 "src/ptrflow.tr"
    s->freed = (void*)List_TrStr_new();
#line 37 "src/ptrflow.tr"
    s->pa = (void*)List_TrStr_new();
#line 38 "src/ptrflow.tr"
    s->pb = (void*)List_TrStr_new();
#line 39 "src/ptrflow.tr"
    s->stk = (void*)List_TrStr_new();
#line 40 "src/ptrflow.tr"
    s->inval = (void*)List_TrStr_new();
#line 41 "src/ptrflow.tr"
    s->own = (void*)List_TrStr_new();
#line 42 "src/ptrflow.tr"
    s->nul = (void*)List_TrStr_new();
#line 43 "src/ptrflow.tr"
    s->sz = (void*)List_TrStr_new();
#line 44 "src/ptrflow.tr"
    s->dead = false;
#line 45 "src/ptrflow.tr"
    return s;
}

__attribute__((hot)) PfState* PfState_copy(PfState* self) {
#line 48 "src/ptrflow.tr"
    PfState* s = PfState_init();
#line 49 "src/ptrflow.tr"
    long long i = 0LL;
#line 50 "src/ptrflow.tr"
    while ((i < self->freed->len)) {
#line 51 "src/ptrflow.tr"
        ({ TrStr _ord_t873 = (List_TrStr_get(self->freed, i)); List_TrStr_append(s->freed, _ord_t873); _tr_str_release(_ord_t873); });
#line 52 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 53 "src/ptrflow.tr"
    i = 0LL;
#line 54 "src/ptrflow.tr"
    while ((i < self->pa->len)) {
#line 55 "src/ptrflow.tr"
        ({ TrStr _ord_t874 = (List_TrStr_get(self->pa, i)); List_TrStr_append(s->pa, _ord_t874); _tr_str_release(_ord_t874); });
#line 56 "src/ptrflow.tr"
        ({ TrStr _ord_t875 = (List_TrStr_get(self->pb, i)); List_TrStr_append(s->pb, _ord_t875); _tr_str_release(_ord_t875); });
#line 57 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 58 "src/ptrflow.tr"
    i = 0LL;
#line 59 "src/ptrflow.tr"
    while ((i < self->stk->len)) {
#line 60 "src/ptrflow.tr"
        ({ TrStr _ord_t876 = (List_TrStr_get(self->stk, i)); List_TrStr_append(s->stk, _ord_t876); _tr_str_release(_ord_t876); });
#line 61 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 62 "src/ptrflow.tr"
    s->inval = pf_vcopy(self->inval);
#line 63 "src/ptrflow.tr"
    s->own = pf_vcopy(self->own);
#line 64 "src/ptrflow.tr"
    s->nul = pf_vcopy(self->nul);
#line 65 "src/ptrflow.tr"
    s->sz = pf_vcopy(self->sz);
#line 66 "src/ptrflow.tr"
    s->dead = self->dead;
#line 67 "src/ptrflow.tr"
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) PfCtx* PfCtx_init(TrMap* summaries) {
#line 174 "src/ptrflow.tr"
    PfCtx* c = ((PfCtx*)_tr_obj_alloc(sizeof(PfCtx)));
#line 175 "src/ptrflow.tr"
    c->errs = (void*)List_TrStr_new();
#line 176 "src/ptrflow.tr"
    c->line = 0LL;
#line 177 "src/ptrflow.tr"
    TrStr _sfo_t877 = c->fn_name;
    c->fn_name = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t877);
#line 178 "src/ptrflow.tr"
    c->ret_from = false;
#line 179 "src/ptrflow.tr"
    c->is_main = false;
#line 180 "src/ptrflow.tr"
    c->params = (void*)List_TrStr_new();
#line 181 "src/ptrflow.tr"
    c->locals = (void*)List_TrStr_new();
#line 182 "src/ptrflow.tr"
    c->summaries = summaries;
#line 183 "src/ptrflow.tr"
    PfState* _cltmp_t878 = PfState_init();
    _tr_obj_release(c->ret_acc, _trdrop_PfState);
    c->ret_acc = _cltmp_t878;
#line 184 "src/ptrflow.tr"
    c->ret_acc->dead = true;
#line 185 "src/ptrflow.tr"
    c->ret_seen = false;
#line 186 "src/ptrflow.tr"
    c->brk = (void*)List_ptr_new();
#line 187 "src/ptrflow.tr"
    c->brk_base = (void*)List_i64_new();
#line 188 "src/ptrflow.tr"
    c->cnt = (void*)List_ptr_new();
#line 189 "src/ptrflow.tr"
    c->cnt_base = (void*)List_i64_new();
#line 190 "src/ptrflow.tr"
    c->report = true;
#line 191 "src/ptrflow.tr"
    c->alloc_line = _tr_dict_new(4LL);
#line 192 "src/ptrflow.tr"
    c->noesc = false;
#line 193 "src/ptrflow.tr"
    c->in_defer = false;
#line 194 "src/ptrflow.tr"
    c->unsafe_depth = 0LL;
#line 195 "src/ptrflow.tr"
    c->origin = _tr_dict_new(4LL);
#line 196 "src/ptrflow.tr"
    c->externs = _tr_dict_new(2LL);
#line 197 "src/ptrflow.tr"
    return c;
}

__attribute__((hot)) void PfCtx_err(PfCtx* self, TrStr msg) {
#line 200 "src/ptrflow.tr"
    if ((!self->report)) {
#line 200 "src/ptrflow.tr"
        return;
    }
#line 201 "src/ptrflow.tr"
    TrStr e = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(self->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (msg)); _tr_str_release(_tr_cl); _tr_cres; });
#line 202 "src/ptrflow.tr"
    if ((!pf_has(self->errs, e))) {
#line 202 "src/ptrflow.tr"
        List_TrStr_append(self->errs, e);
    }
    _tr_str_release(e);
}

__attribute__((hot)) bool pf_has(List_TrStr* v, TrStr s) {
#line 70 "src/ptrflow.tr"
    long long i = 0LL;
#line 71 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 72 "src/ptrflow.tr"
        if (({ TrStr _cmpa_t879 = (List_TrStr_get(v, i)); TrStr _cmpb_t880 = (s); _Bool _cmpr_t881 = _tr_str_eqv(_cmpa_t879, _cmpb_t880); _tr_str_release(_cmpa_t879); _cmpr_t881; })) {
#line 72 "src/ptrflow.tr"
            return true;
        }
#line 73 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 74 "src/ptrflow.tr"
    return false;
}

__attribute__((hot)) void pf_add(List_TrStr* v, TrStr s) {
#line 77 "src/ptrflow.tr"
    if ((!pf_has(v, s))) {
#line 77 "src/ptrflow.tr"
        List_TrStr_append(v, s);
    }
}

__attribute__((hot)) bool pf_under(TrStr p, TrStr n) {
#line 81 "src/ptrflow.tr"
    if (_tr_str_eqv((p), (n))) {
#line 81 "src/ptrflow.tr"
        return true;
    }
#line 82 "src/ptrflow.tr"
    return _tr_str_starts_withv((p), (_tr_strx_concatv((n), (_tr_str_lit_len(".", 1LL)))));
}

__attribute__((hot)) List_TrStr* pf_vcopy(List_TrStr* v) {
#line 85 "src/ptrflow.tr"
    List_TrStr* o = (void*)List_TrStr_new();
#line 86 "src/ptrflow.tr"
    long long i = 0LL;
#line 87 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 88 "src/ptrflow.tr"
        ({ TrStr _at_t882 = (List_TrStr_get(v, i)); List_TrStr_append(o, _at_t882); _tr_str_release(_at_t882); });
#line 89 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 90 "src/ptrflow.tr"
    return o;
}

__attribute__((hot)) List_TrStr* pf_inter(List_TrStr* a, List_TrStr* b) {
#line 93 "src/ptrflow.tr"
    List_TrStr* o = (void*)List_TrStr_new();
#line 94 "src/ptrflow.tr"
    long long i = 0LL;
#line 95 "src/ptrflow.tr"
    while ((i < a->len)) {
#line 96 "src/ptrflow.tr"
        if (({ TrStr _at_t883 = (List_TrStr_get(a, i)); __auto_type _wr = (pf_has(b, _at_t883)); _tr_str_release(_at_t883); _wr; })) {
#line 96 "src/ptrflow.tr"
            ({ TrStr _at_t884 = (List_TrStr_get(a, i)); List_TrStr_append(o, _at_t884); _tr_str_release(_at_t884); });
        }
#line 97 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 98 "src/ptrflow.tr"
    return o;
}

__attribute__((hot)) void pf_assign_from(PfState* into, PfState* o) {
#line 101 "src/ptrflow.tr"
    into->freed = pf_vcopy(o->freed);
#line 102 "src/ptrflow.tr"
    into->pa = pf_vcopy(o->pa);
#line 103 "src/ptrflow.tr"
    into->pb = pf_vcopy(o->pb);
#line 104 "src/ptrflow.tr"
    into->stk = pf_vcopy(o->stk);
#line 105 "src/ptrflow.tr"
    into->inval = pf_vcopy(o->inval);
#line 106 "src/ptrflow.tr"
    into->own = pf_vcopy(o->own);
#line 107 "src/ptrflow.tr"
    into->nul = pf_vcopy(o->nul);
#line 108 "src/ptrflow.tr"
    into->sz = pf_vcopy(o->sz);
#line 109 "src/ptrflow.tr"
    into->dead = o->dead;
}

__attribute__((hot)) bool pf_has_pair(PfState* st, TrStr a, TrStr b) {
#line 112 "src/ptrflow.tr"
    long long i = 0LL;
#line 113 "src/ptrflow.tr"
    while ((i < st->pa->len)) {
#line 114 "src/ptrflow.tr"
        if ((({ TrStr _cmpa_t885 = (List_TrStr_get(st->pa, i)); TrStr _cmpb_t886 = (a); _Bool _cmpr_t887 = _tr_str_eqv(_cmpa_t885, _cmpb_t886); _tr_str_release(_cmpa_t885); _cmpr_t887; }) && ({ TrStr _cmpa_t888 = (List_TrStr_get(st->pb, i)); TrStr _cmpb_t889 = (b); _Bool _cmpr_t890 = _tr_str_eqv(_cmpa_t888, _cmpb_t889); _tr_str_release(_cmpa_t888); _cmpr_t890; }))) {
#line 114 "src/ptrflow.tr"
            return true;
        }
#line 115 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 116 "src/ptrflow.tr"
    return false;
}

__attribute__((hot)) void pf_merge(PfState* into, PfState* o) {
#line 120 "src/ptrflow.tr"
    if (o->dead) {
#line 120 "src/ptrflow.tr"
        return;
    }
#line 121 "src/ptrflow.tr"
    if (into->dead) {
#line 122 "src/ptrflow.tr"
        pf_assign_from(into, o);
#line 123 "src/ptrflow.tr"
        return;
    }
#line 124 "src/ptrflow.tr"
    long long i = 0LL;
#line 125 "src/ptrflow.tr"
    while ((i < o->freed->len)) {
#line 126 "src/ptrflow.tr"
        ({ __auto_type _ord_t892 = (into->freed); TrStr _at_t891 = (List_TrStr_get(o->freed, i)); pf_add(_ord_t892, _at_t891); _tr_str_release(_at_t891); });
#line 127 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 128 "src/ptrflow.tr"
    i = 0LL;
#line 129 "src/ptrflow.tr"
    while ((i < o->pa->len)) {
#line 130 "src/ptrflow.tr"
        if (({ TrStr _at_t893 = (List_TrStr_get(o->pa, i)); TrStr _at_t894 = (List_TrStr_get(o->pb, i)); __auto_type _wr = ((!pf_has_pair(into, _at_t893, _at_t894))); _tr_str_release(_at_t893); _tr_str_release(_at_t894); _wr; })) {
#line 131 "src/ptrflow.tr"
            ({ TrStr _ord_t895 = (List_TrStr_get(o->pa, i)); List_TrStr_append(into->pa, _ord_t895); _tr_str_release(_ord_t895); });
#line 132 "src/ptrflow.tr"
            ({ TrStr _ord_t896 = (List_TrStr_get(o->pb, i)); List_TrStr_append(into->pb, _ord_t896); _tr_str_release(_ord_t896); });
        }
#line 133 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 134 "src/ptrflow.tr"
    i = 0LL;
#line 135 "src/ptrflow.tr"
    while ((i < o->stk->len)) {
#line 136 "src/ptrflow.tr"
        ({ __auto_type _ord_t898 = (into->stk); TrStr _at_t897 = (List_TrStr_get(o->stk, i)); pf_add(_ord_t898, _at_t897); _tr_str_release(_at_t897); });
#line 137 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 138 "src/ptrflow.tr"
    i = 0LL;
#line 139 "src/ptrflow.tr"
    while ((i < o->own->len)) {
#line 140 "src/ptrflow.tr"
        ({ __auto_type _ord_t900 = (into->own); TrStr _at_t899 = (List_TrStr_get(o->own, i)); pf_add(_ord_t900, _at_t899); _tr_str_release(_at_t899); });
#line 141 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 142 "src/ptrflow.tr"
    i = 0LL;
#line 143 "src/ptrflow.tr"
    while ((i < o->inval->len)) {
#line 144 "src/ptrflow.tr"
        ({ __auto_type _ord_t902 = (into->inval); TrStr _at_t901 = (List_TrStr_get(o->inval, i)); pf_add(_ord_t902, _at_t901); _tr_str_release(_at_t901); });
#line 145 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 146 "src/ptrflow.tr"
    into->nul = pf_inter(into->nul, o->nul);
#line 147 "src/ptrflow.tr"
    into->sz = pf_inter(into->sz, o->sz);
}

__attribute__((hot)) bool pf_null(HirExpr* e) {
#line 207 "src/ptrflow.tr"
    return (((unsigned long long)(e)) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr pf_path(HirExpr* e) {
#line 211 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 211 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 212 "src/ptrflow.tr"
    __auto_type _t903 = (*e);
    if (_t903.tag == HirExpr_EIdent) {
        __auto_type nm = _t903.data.EIdent.name;
        TrStr _trret179 = _tr_str_retain(nm);
        return _trret179;
    } else if (_t903.tag == HirExpr_EPropAccess) {
        __auto_type o = _t903.data.EPropAccess.obj;
__auto_type p = _t903.data.EPropAccess.prop;
#line 215 "src/ptrflow.tr"
        TrStr po = pf_path(o);
#line 216 "src/ptrflow.tr"
        if ((!_tr_str_eqv((po), (_tr_str_lit_len("", 0LL))))) {
#line 216 "src/ptrflow.tr"
            TrStr _trret180 = ({ TrStr _tr_cl = (_tr_strx_concatv((po), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(po);
            return _trret180;
        }
#line 217 "src/ptrflow.tr"
        _tr_str_release(po);
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t903;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) TrStr pf_root(HirExpr* e) {
#line 222 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 222 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 223 "src/ptrflow.tr"
    __auto_type _t904 = (*e);
    if (_t904.tag == HirExpr_EIdent) {
        __auto_type nm = _t904.data.EIdent.name;
        TrStr _trret181 = _tr_str_retain(nm);
        return _trret181;
    } else if (_t904.tag == HirExpr_EPropAccess) {
        __auto_type o = _t904.data.EPropAccess.obj;
__auto_type p = _t904.data.EPropAccess.prop;
        TrStr _trret182 = pf_path(e);
        return _trret182;
    } else if (_t904.tag == HirExpr_ECast) {
        __auto_type x = _t904.data.ECast.expr;
        return pf_root(x);
    } else if (_t904.tag == HirExpr_EMethodCall) {
        __auto_type o = _t904.data.EMethodCall.obj;
__auto_type m = _t904.data.EMethodCall.method;
#line 228 "src/ptrflow.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cast", 4LL))))) {
#line 228 "src/ptrflow.tr"
            TrStr _trret183 = pf_root(o);
            return _trret183;
        }
#line 229 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t904;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool pf_is_dealloc(HirExpr* callee) {
#line 233 "src/ptrflow.tr"
    if (pf_null(callee)) {
#line 233 "src/ptrflow.tr"
        return false;
    }
#line 234 "src/ptrflow.tr"
    __auto_type _t905 = (*callee);
    if (_t905.tag == HirExpr_EIdent) {
        __auto_type nm = _t905.data.EIdent.name;
        bool _trret184 = _tr_str_eqv((nm), (_tr_str_lit_len("dealloc", 7LL)));
        return _trret184;
    } else if (_t905.tag == HirExpr_EIndex) {
        __auto_type o = _t905.data.EIndex.obj;
#line 237 "src/ptrflow.tr"
        __auto_type _t906 = (*o);
        if (_t906.tag == HirExpr_EIdent) {
            __auto_type nm = _t906.data.EIdent.name;
            bool _trret185 = _tr_str_eqv((nm), (_tr_str_lit_len("dealloc", 7LL)));
            return _trret185;
        } else if (1) {
            __auto_type _ = _t906;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t905;
        return false;
    }
}

__attribute__((hot)) TrStr pf_callee_name(HirExpr* callee) {
#line 243 "src/ptrflow.tr"
    if (pf_null(callee)) {
#line 243 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 244 "src/ptrflow.tr"
    __auto_type _t907 = (*callee);
    if (_t907.tag == HirExpr_EIdent) {
        __auto_type nm = _t907.data.EIdent.name;
        TrStr _trret186 = _tr_str_retain(nm);
        return _trret186;
    } else if (1) {
        __auto_type _ = _t907;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) TrStr pf_stack_src(HirExpr* e, PfState* st, PfCtx* cx) {
#line 250 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 250 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 251 "src/ptrflow.tr"
    __auto_type _t908 = (*e);
    if (_t908.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t908.data.EUnaryOp.op;
__auto_type x = _t908.data.EUnaryOp.expr;
#line 253 "src/ptrflow.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL))) && (!pf_null(x)))) {
#line 254 "src/ptrflow.tr"
            __auto_type _t909 = (*x);
            if (_t909.tag == HirExpr_EIdent) {
                __auto_type nm = _t909.data.EIdent.name;
#line 256 "src/ptrflow.tr"
                if (((!_tr_str_eqv((nm), (_tr_str_lit_len("self", 4LL)))) && (pf_has(cx->locals, nm) || pf_has(cx->params, nm)))) {
#line 256 "src/ptrflow.tr"
                    TrStr _trret187 = _tr_str_retain(nm);
                    return _trret187;
                }
            } else if (1) {
                __auto_type _ = _t909;
                /* pass */
            }
        }
#line 258 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t908.tag == HirExpr_EIdent) {
        __auto_type nm = _t908.data.EIdent.name;
#line 260 "src/ptrflow.tr"
        if (pf_has(st->stk, nm)) {
#line 261 "src/ptrflow.tr"
            if (_tr_dict_contains(cx->origin, _tr_strz(nm))) {
#line 261 "src/ptrflow.tr"
                TrStr _trret188 = _tr_str_retain(_tr_str_unbox(_tr_dict_get(cx->origin, _tr_strz(nm))));
                return _trret188;
            }
#line 262 "src/ptrflow.tr"
            TrStr _trret189 = _tr_str_retain(nm);
            return _trret189;
        }
#line 263 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t908.tag == HirExpr_ECast) {
        __auto_type x = _t908.data.ECast.expr;
        return pf_stack_src(x, st, cx);
    } else if (_t908.tag == HirExpr_EMethodCall) {
        __auto_type o = _t908.data.EMethodCall.obj;
__auto_type m = _t908.data.EMethodCall.method;
#line 266 "src/ptrflow.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cast", 4LL))))) {
#line 266 "src/ptrflow.tr"
            TrStr _trret190 = pf_stack_src(o, st, cx);
            return _trret190;
        }
#line 267 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t908;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) long long pf_lit(HirExpr* e) {
#line 272 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 272 "src/ptrflow.tr"
        return (-4611686018427387904LL);
    }
#line 273 "src/ptrflow.tr"
    __auto_type _t910 = (*e);
    if (_t910.tag == HirExpr_ELitInt) {
        __auto_type v = _t910.data.ELitInt.val;
        return v;
    } else if (_t910.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t910.data.EUnaryOp.op;
__auto_type x = _t910.data.EUnaryOp.expr;
#line 276 "src/ptrflow.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) && (!pf_null(x)))) {
#line 277 "src/ptrflow.tr"
            __auto_type _t911 = (*x);
            if (_t911.tag == HirExpr_ELitInt) {
                __auto_type v2 = _t911.data.ELitInt.val;
                long long _trret191 = (0LL - v2);
                return _trret191;
            } else if (1) {
                __auto_type _ = _t911;
                return (-4611686018427387904LL);
            }
        }
#line 280 "src/ptrflow.tr"
        return (-4611686018427387904LL);
    } else if (1) {
        __auto_type _ = _t910;
        return (-4611686018427387904LL);
    }
}

__attribute__((hot)) bool pf_is_lit(HirExpr* e) {
#line 284 "src/ptrflow.tr"
    return (pf_lit(e) != (-4611686018427387904LL));
}

__attribute__((hot)) bool pf_named_call(HirExpr* callee, TrStr want) {
#line 287 "src/ptrflow.tr"
    if (pf_null(callee)) {
#line 287 "src/ptrflow.tr"
        return false;
    }
#line 288 "src/ptrflow.tr"
    __auto_type _t912 = (*callee);
    if (_t912.tag == HirExpr_EIdent) {
        __auto_type nm = _t912.data.EIdent.name;
        bool _trret192 = _tr_str_eqv((nm), (want));
        return _trret192;
    } else if (_t912.tag == HirExpr_EIndex) {
        __auto_type o = _t912.data.EIndex.obj;
#line 291 "src/ptrflow.tr"
        __auto_type _t913 = (*o);
        if (_t913.tag == HirExpr_EIdent) {
            __auto_type nm = _t913.data.EIdent.name;
            bool _trret193 = _tr_str_eqv((nm), (want));
            return _trret193;
        } else if (1) {
            __auto_type _ = _t913;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t912;
        return false;
    }
}

__attribute__((hot)) bool pf_is_alloc(HirExpr* val) {
#line 297 "src/ptrflow.tr"
    if (pf_null(val)) {
#line 297 "src/ptrflow.tr"
        return false;
    }
#line 298 "src/ptrflow.tr"
    __auto_type _t914 = (*val);
    if (_t914.tag == HirExpr_ECall) {
        __auto_type callee = _t914.data.ECall.callee;
        return pf_named_call(callee, _tr_str_lit_len("alloc", 5LL));
    } else if (1) {
        __auto_type _ = _t914;
        return false;
    }
}

__attribute__((hot)) long long pf_alloc_n(HirExpr* val) {
#line 303 "src/ptrflow.tr"
    __auto_type _t915 = (*val);
    if (_t915.tag == HirExpr_ECall) {
        __auto_type args = _t915.data.ECall.args;
#line 305 "src/ptrflow.tr"
        if ((args->len == 1LL)) {
#line 305 "src/ptrflow.tr"
            return pf_lit(((HirExpr*)List_ptr_get(args, 0LL)));
        }
#line 306 "src/ptrflow.tr"
        return (-4611686018427387904LL);
    } else if (1) {
        __auto_type _ = _t915;
        return (-4611686018427387904LL);
    }
}

__attribute__((hot)) bool pf_is_null_lit(HirExpr* val) {
#line 311 "src/ptrflow.tr"
    if (pf_null(val)) {
#line 311 "src/ptrflow.tr"
        return false;
    }
#line 312 "src/ptrflow.tr"
    __auto_type _t916 = (*val);
    if (_t916.tag == HirExpr_ELitNone) {
        __auto_type _ = _t916.data.ELitNone.ty;
        return true;
    } else if (_t916.tag == HirExpr_ELitInt) {
        __auto_type v = _t916.data.ELitInt.val;
__auto_type ty = _t916.data.ELitInt.ty;
        bool _trret194 = ((v == 0LL) && _tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))));
        return _trret194;
    } else if (_t916.tag == HirExpr_ECast) {
        __auto_type x = _t916.data.ECast.expr;
__auto_type ty = _t916.data.ECast.target_ty;
#line 316 "src/ptrflow.tr"
        if ((pf_null(x) || (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL)))))) {
#line 316 "src/ptrflow.tr"
            return false;
        }
#line 317 "src/ptrflow.tr"
        __auto_type _t917 = (*x);
        if (_t917.tag == HirExpr_ELitInt) {
            __auto_type v = _t917.data.ELitInt.val;
            bool _trret195 = (v == 0LL);
            return _trret195;
        } else if (1) {
            __auto_type _ = _t917;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t916;
        return false;
    }
}

__attribute__((hot)) long long pf_size_of(PfState* st, TrStr path) {
#line 323 "src/ptrflow.tr"
    long long i = 0LL;
#line 324 "src/ptrflow.tr"
    while ((i < st->sz->len)) {
#line 325 "src/ptrflow.tr"
        TrStr e = List_TrStr_get(st->sz, i);
#line 326 "src/ptrflow.tr"
        long long eq = _tr_str_index_ofv((e), (_tr_str_lit_len("=", 1LL)));
#line 327 "src/ptrflow.tr"
        if (({ TrStr _cmpa_t918 = (_tr_str_slicev((e), 0LL, eq)); TrStr _cmpb_t919 = (path); _Bool _cmpr_t920 = _tr_str_eqv(_cmpa_t918, _cmpb_t919); _tr_str_release(_cmpa_t918); _cmpr_t920; })) {
#line 327 "src/ptrflow.tr"
            long long _trret196 = _tr_str_to_int(_tr_strz(_tr_str_slicev((e), (eq + 1LL), _tr_str_lenv((e)))));
            _tr_str_release(e);
            return _trret196;
        }
#line 328 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(e);
    }
#line 329 "src/ptrflow.tr"
    return (-1LL);
}

__attribute__((hot)) void pf_kill(PfState* st, TrStr path) {
#line 334 "src/ptrflow.tr"
    List_TrStr* nf = (void*)List_TrStr_new();
#line 335 "src/ptrflow.tr"
    long long i = 0LL;
#line 336 "src/ptrflow.tr"
    while ((i < st->freed->len)) {
#line 337 "src/ptrflow.tr"
        if (({ TrStr _at_t921 = (List_TrStr_get(st->freed, i)); __auto_type _wr = ((!pf_under(_at_t921, path))); _tr_str_release(_at_t921); _wr; })) {
#line 337 "src/ptrflow.tr"
            ({ TrStr _at_t922 = (List_TrStr_get(st->freed, i)); List_TrStr_append(nf, _at_t922); _tr_str_release(_at_t922); });
        }
#line 338 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 339 "src/ptrflow.tr"
    st->freed = nf;
#line 340 "src/ptrflow.tr"
    List_TrStr* na = (void*)List_TrStr_new();
#line 341 "src/ptrflow.tr"
    List_TrStr* nb = (void*)List_TrStr_new();
#line 342 "src/ptrflow.tr"
    i = 0LL;
#line 343 "src/ptrflow.tr"
    while ((i < st->pa->len)) {
#line 344 "src/ptrflow.tr"
        if ((({ TrStr _at_t923 = (List_TrStr_get(st->pa, i)); __auto_type _wr = ((!pf_under(_at_t923, path))); _tr_str_release(_at_t923); _wr; }) && ({ TrStr _at_t924 = (List_TrStr_get(st->pb, i)); __auto_type _wr = ((!pf_under(_at_t924, path))); _tr_str_release(_at_t924); _wr; }))) {
#line 345 "src/ptrflow.tr"
            ({ TrStr _at_t925 = (List_TrStr_get(st->pa, i)); List_TrStr_append(na, _at_t925); _tr_str_release(_at_t925); });
#line 346 "src/ptrflow.tr"
            ({ TrStr _at_t926 = (List_TrStr_get(st->pb, i)); List_TrStr_append(nb, _at_t926); _tr_str_release(_at_t926); });
        }
#line 347 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 348 "src/ptrflow.tr"
    st->pa = na;
#line 349 "src/ptrflow.tr"
    st->pb = nb;
#line 350 "src/ptrflow.tr"
    List_TrStr* ns = (void*)List_TrStr_new();
#line 351 "src/ptrflow.tr"
    i = 0LL;
#line 352 "src/ptrflow.tr"
    while ((i < st->stk->len)) {
#line 353 "src/ptrflow.tr"
        if (({ TrStr _at_t927 = (List_TrStr_get(st->stk, i)); __auto_type _wr = ((!pf_under(_at_t927, path))); _tr_str_release(_at_t927); _wr; })) {
#line 353 "src/ptrflow.tr"
            ({ TrStr _at_t928 = (List_TrStr_get(st->stk, i)); List_TrStr_append(ns, _at_t928); _tr_str_release(_at_t928); });
        }
#line 354 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 355 "src/ptrflow.tr"
    st->stk = ns;
#line 356 "src/ptrflow.tr"
    st->own = pf_drop_under(st->own, path);
#line 357 "src/ptrflow.tr"
    st->inval = pf_drop_under(st->inval, path);
#line 358 "src/ptrflow.tr"
    st->nul = pf_drop_under(st->nul, path);
#line 359 "src/ptrflow.tr"
    List_TrStr* nz = (void*)List_TrStr_new();
#line 360 "src/ptrflow.tr"
    i = 0LL;
#line 361 "src/ptrflow.tr"
    while ((i < st->sz->len)) {
#line 362 "src/ptrflow.tr"
        TrStr e = List_TrStr_get(st->sz, i);
#line 363 "src/ptrflow.tr"
        long long eq = _tr_str_index_ofv((e), (_tr_str_lit_len("=", 1LL)));
#line 364 "src/ptrflow.tr"
        if (({ TrStr _at_t929 = (_tr_str_slicev((e), 0LL, eq)); __auto_type _wr = ((!pf_under(_at_t929, path))); _tr_str_release(_at_t929); _wr; })) {
#line 364 "src/ptrflow.tr"
            List_TrStr_append(nz, e);
        }
#line 365 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(e);
    }
#line 366 "src/ptrflow.tr"
    st->sz = nz;
}

__attribute__((hot)) List_TrStr* pf_drop_under(List_TrStr* v, TrStr path) {
#line 369 "src/ptrflow.tr"
    List_TrStr* o = (void*)List_TrStr_new();
#line 370 "src/ptrflow.tr"
    long long i = 0LL;
#line 371 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 372 "src/ptrflow.tr"
        if (({ TrStr _at_t930 = (List_TrStr_get(v, i)); __auto_type _wr = ((!pf_under(_at_t930, path))); _tr_str_release(_at_t930); _wr; })) {
#line 372 "src/ptrflow.tr"
            ({ TrStr _at_t931 = (List_TrStr_get(v, i)); List_TrStr_append(o, _at_t931); _tr_str_release(_at_t931); });
        }
#line 373 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 374 "src/ptrflow.tr"
    return o;
}

__attribute__((hot)) void pf_kill_fields(PfState* st, TrStr path) {
#line 378 "src/ptrflow.tr"
    List_TrStr* nf = (void*)List_TrStr_new();
#line 379 "src/ptrflow.tr"
    long long i = 0LL;
#line 380 "src/ptrflow.tr"
    while ((i < st->freed->len)) {
#line 381 "src/ptrflow.tr"
        TrStr f = List_TrStr_get(st->freed, i);
#line 382 "src/ptrflow.tr"
        if ((!((!_tr_str_eqv((f), (path))) && pf_under(f, path)))) {
#line 382 "src/ptrflow.tr"
            List_TrStr_append(nf, f);
        }
#line 383 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(f);
    }
#line 384 "src/ptrflow.tr"
    st->freed = nf;
}

__attribute__((hot)) void pf_link(PfState* st, TrStr a, TrStr b) {
#line 387 "src/ptrflow.tr"
    if (_tr_str_eqv((a), (b))) {
#line 387 "src/ptrflow.tr"
        return;
    }
#line 388 "src/ptrflow.tr"
    if ((!pf_has_pair(st, a, b))) {
#line 389 "src/ptrflow.tr"
        List_TrStr_append(st->pa, a);
#line 390 "src/ptrflow.tr"
        List_TrStr_append(st->pb, b);
    }
#line 391 "src/ptrflow.tr"
    if ((!pf_has_pair(st, b, a))) {
#line 392 "src/ptrflow.tr"
        List_TrStr_append(st->pa, b);
#line 393 "src/ptrflow.tr"
        List_TrStr_append(st->pb, a);
    }
}

__attribute__((hot)) void pf_free(PfState* st, TrStr path) {
#line 397 "src/ptrflow.tr"
    pf_add(st->freed, path);
#line 398 "src/ptrflow.tr"
    bool changed = true;
#line 399 "src/ptrflow.tr"
    long long guard = 0LL;
#line 400 "src/ptrflow.tr"
    while ((changed && (guard < 8LL))) {
#line 401 "src/ptrflow.tr"
        changed = false;
#line 402 "src/ptrflow.tr"
        guard = (guard + 1LL);
#line 403 "src/ptrflow.tr"
        long long i = 0LL;
#line 404 "src/ptrflow.tr"
        while ((i < st->pa->len)) {
#line 405 "src/ptrflow.tr"
            if ((({ __auto_type _ord_t933 = (st->freed); TrStr _at_t932 = (List_TrStr_get(st->pa, i)); __auto_type _wr = (pf_has(_ord_t933, _at_t932)); _tr_str_release(_at_t932); _wr; }) && ({ __auto_type _ord_t935 = (st->freed); TrStr _at_t934 = (List_TrStr_get(st->pb, i)); __auto_type _wr = ((!pf_has(_ord_t935, _at_t934))); _tr_str_release(_at_t934); _wr; }))) {
#line 406 "src/ptrflow.tr"
                ({ TrStr _ord_t936 = (List_TrStr_get(st->pb, i)); List_TrStr_append(st->freed, _ord_t936); _tr_str_release(_ord_t936); });
#line 407 "src/ptrflow.tr"
                changed = true;
            }
#line 408 "src/ptrflow.tr"
            i = (i + 1LL);
        }
    }
}

__attribute__((hot)) TrStr pf_is_freed(PfState* st, TrStr path) {
#line 412 "src/ptrflow.tr"
    long long i = 0LL;
#line 413 "src/ptrflow.tr"
    while ((i < st->freed->len)) {
#line 414 "src/ptrflow.tr"
        TrStr f = List_TrStr_get(st->freed, i);
#line 415 "src/ptrflow.tr"
        if (_tr_str_eqv((f), (path))) {
#line 415 "src/ptrflow.tr"
            TrStr _trret197 = _tr_str_retain(f);
            _tr_str_release(f);
            return _trret197;
        }
#line 416 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(f);
    }
#line 417 "src/ptrflow.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool pf_zeroish(HirExpr* e) {
#line 422 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 422 "src/ptrflow.tr"
        return false;
    }
#line 423 "src/ptrflow.tr"
    if (pf_is_null_lit(e)) {
#line 423 "src/ptrflow.tr"
        return true;
    }
#line 424 "src/ptrflow.tr"
    __auto_type _t937 = (*e);
    if (_t937.tag == HirExpr_ELitInt) {
        __auto_type v = _t937.data.ELitInt.val;
        return (v == 0LL);
    } else if (_t937.tag == HirExpr_ECast) {
        __auto_type x = _t937.data.ECast.expr;
        return pf_zeroish(x);
    } else if (1) {
        __auto_type _ = _t937;
        return false;
    }
}

__attribute__((hot)) TrStr pf_nulltest(HirExpr* cond) {
#line 431 "src/ptrflow.tr"
    if (pf_null(cond)) {
#line 431 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 432 "src/ptrflow.tr"
    __auto_type _t938 = (*cond);
    if (_t938.tag == HirExpr_EBinOp) {
        __auto_type op = _t938.data.EBinOp.op;
__auto_type l = _t938.data.EBinOp.left;
__auto_type r = _t938.data.EBinOp.right;
#line 434 "src/ptrflow.tr"
        if (((!_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) && (!_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))))) {
#line 434 "src/ptrflow.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 435 "src/ptrflow.tr"
        TrStr pth = _tr_str_lit_len("", 0LL);
#line 436 "src/ptrflow.tr"
        if (pf_zeroish(r)) {
#line 436 "src/ptrflow.tr"
            TrStr _strtmp_t939 = pf_root(l);
            _tr_str_release(pth);
            pth = _strtmp_t939;
        } else if (pf_zeroish(l)) {
#line 437 "src/ptrflow.tr"
            TrStr _strtmp_t940 = pf_root(r);
            _tr_str_release(pth);
            pth = _strtmp_t940;
        }
#line 438 "src/ptrflow.tr"
        if (_tr_str_eqv((pth), (_tr_str_lit_len("", 0LL)))) {
#line 438 "src/ptrflow.tr"
            _tr_str_release(pth);
            return _tr_str_lit_len("", 0LL);
        }
#line 439 "src/ptrflow.tr"
        TrStr _trret198 = ({ TrStr _tr_cl = (_tr_strx_concatv((op), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pth)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(pth);
        return _trret198;
    } else if (1) {
        __auto_type _ = _t938;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) void pf_refine(PfState* st, TrStr test, bool taken) {
#line 444 "src/ptrflow.tr"
    if (_tr_str_eqv((test), (_tr_str_lit_len("", 0LL)))) {
#line 444 "src/ptrflow.tr"
        return;
    }
#line 445 "src/ptrflow.tr"
    TrStr op = _tr_str_slicev((test), 0LL, 2LL);
#line 446 "src/ptrflow.tr"
    TrStr pth = _tr_str_slicev((test), 3LL, _tr_str_lenv((test)));
#line 447 "src/ptrflow.tr"
    bool is_null_here = (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) == taken);
#line 448 "src/ptrflow.tr"
    if ((!is_null_here)) {
#line 448 "src/ptrflow.tr"
        st->nul = pf_drop_under(st->nul, pth);
    }
    _tr_str_release(op);
    _tr_str_release(pth);
}

__attribute__((hot)) void pf_block(PfCtx* cx, HirBlock* b, PfState* st) {
#line 451 "src/ptrflow.tr"
    long long i = 0LL;
#line 452 "src/ptrflow.tr"
    while ((i < b->stmts->len)) {
#line 453 "src/ptrflow.tr"
        pf_stmt(cx, ((HirStmt*)List_ptr_get(b->stmts, i)), st);
#line 454 "src/ptrflow.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void pf_use_msg(PfCtx* cx, TrStr name) {
#line 457 "src/ptrflow.tr"
    ({ TrStr _at_t941 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was freed by 'dealloc()' and can no longer be used.\n      FIX: Remove all uses of '", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after 'dealloc()', or restructure so the pointer is freed only when no longer needed (or re-assign it a fresh value first). A pointer that is deallocated on only some paths, or in a previous loop iteration, counts as freed.", 225LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t941); _tr_str_release(_at_t941); });
}

__attribute__((hot)) void pf_exprs(PfCtx* cx, List_ptr* v, PfState* st) {
#line 460 "src/ptrflow.tr"
    long long i = 0LL;
#line 461 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 462 "src/ptrflow.tr"
        pf_expr(cx, ((HirExpr*)List_ptr_get(v, i)), st);
#line 463 "src/ptrflow.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void pf_branchy_block(PfCtx* cx, HirBlock* b, PfState* st) {
#line 467 "src/ptrflow.tr"
    PfState* c = PfState_copy(st);
#line 468 "src/ptrflow.tr"
    pf_block(cx, b, c);
#line 469 "src/ptrflow.tr"
    pf_merge(st, c);
    _tr_obj_release(c, _trdrop_PfState);
}

__attribute__((hot)) void pf_ptr_checks(PfCtx* cx, PfState* st, HirExpr* o, TrStr m, List_ptr* args) {
#line 472 "src/ptrflow.tr"
    TrStr np = pf_path(o);
#line 473 "src/ptrflow.tr"
    if (((!_tr_str_eqv((np), (_tr_str_lit_len("", 0LL)))) && (_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))))) {
#line 474 "src/ptrflow.tr"
        if (pf_has(st->nul, np)) {
#line 475 "src/ptrflow.tr"
            ({ TrStr _at_t942 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '.", 8LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' on '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (np)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is a null pointer here (null dereference).\n      FIX: allocate it (alloc[T](n)) or check it for null before dereferencing.", 131LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t942); _tr_str_release(_at_t942); });
        } else if ((pf_size_of(st, np) == 0LL)) {
#line 477 "src/ptrflow.tr"
            ({ TrStr _at_t943 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '.", 8LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' on '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (np)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which was allocated with 0 elements (out of bounds).\n      FIX: allocate at least one element, or do not dereference an empty block.", 135LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t943); _tr_str_release(_at_t943); });
        }
    }
#line 478 "src/ptrflow.tr"
    if (((_tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))) && (!_tr_str_eqv((np), (_tr_str_lit_len("", 0LL))))) && (args->len == 1LL))) {
#line 479 "src/ptrflow.tr"
        long long sz = pf_size_of(st, np);
#line 480 "src/ptrflow.tr"
        long long k = pf_lit(((HirExpr*)List_ptr_get(args, 0LL)));
#line 481 "src/ptrflow.tr"
        if ((((sz >= 0LL) && pf_is_lit(((HirExpr*)List_ptr_get(args, 0LL)))) && ((k < 0LL) || (k > sz)))) {
#line 482 "src/ptrflow.tr"
            ({ __auto_type _ord_t944 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '", 7LL)), (np))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".offset(", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(k)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")' moves outside the ", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t945 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t944); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(sz)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("-element block '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (np)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was allocated with (undefined behaviour even before it is dereferenced).\n      FIX: keep the offset in 0 .. n (one-past-the-end) of the alloc[T](n) block.", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t945); _tr_str_release(_at_t945); });
        }
    }
#line 483 "src/ptrflow.tr"
    if (((_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) && (!pf_null(o)))) {
#line 484 "src/ptrflow.tr"
        __auto_type _t946 = (*o);
        if (_t946.tag == HirExpr_EMethodCall) {
            __auto_type bo = _t946.data.EMethodCall.obj;
__auto_type bm = _t946.data.EMethodCall.method;
__auto_type bargs = _t946.data.EMethodCall.args;
#line 486 "src/ptrflow.tr"
            if ((_tr_str_eqv((bm), (_tr_str_lit_len("offset", 6LL))) && (bargs->len == 1LL))) {
#line 487 "src/ptrflow.tr"
                TrStr bp = pf_path(bo);
#line 488 "src/ptrflow.tr"
                long long bsz = (-1LL);
#line 489 "src/ptrflow.tr"
                if ((!_tr_str_eqv((bp), (_tr_str_lit_len("", 0LL))))) {
#line 489 "src/ptrflow.tr"
                    bsz = pf_size_of(st, bp);
                }
#line 490 "src/ptrflow.tr"
                long long bk = pf_lit(((HirExpr*)List_ptr_get(bargs, 0LL)));
#line 491 "src/ptrflow.tr"
                if ((((bsz >= 0LL) && pf_is_lit(((HirExpr*)List_ptr_get(bargs, 0LL)))) && ((bk < 0LL) || (bk >= bsz)))) {
#line 492 "src/ptrflow.tr"
                    ({ __auto_type _ord_t947 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '", 7LL)), (bp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".offset(", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(bk)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' is outside the ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t948 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t947); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(bsz)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("-element block '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was allocated with (out-of-bounds access).\n      FIX: keep the offset in 0 .. n-1 of the alloc[T](n) block, or use a List[T] (bounds-checked).", 144LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t948); _tr_str_release(_at_t948); });
                }
            }
        } else if (1) {
            __auto_type _ = _t946;
            /* pass */
        }
    }
    _tr_str_release(np);
}

__attribute__((hot)) void pf_expr(PfCtx* cx, HirExpr* e, PfState* st) {
#line 496 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 496 "src/ptrflow.tr"
        return;
    }
#line 497 "src/ptrflow.tr"
    __auto_type _t949 = (*e);
    if (_t949.tag == HirExpr_EIdent) {
        __auto_type nm = _t949.data.EIdent.name;
#line 499 "src/ptrflow.tr"
        if (pf_has(st->freed, nm)) {
#line 500 "src/ptrflow.tr"
            pf_use_msg(cx, nm);
#line 501 "src/ptrflow.tr"
            pf_kill(st, nm);
        }
#line 502 "src/ptrflow.tr"
        if (((!cx->noesc) && pf_has(st->own, nm))) {
#line 502 "src/ptrflow.tr"
            st->own = pf_drop_under(st->own, nm);
        }
#line 503 "src/ptrflow.tr"
        if (pf_has(st->inval, nm)) {
#line 504 "src/ptrflow.tr"
            ({ TrStr _at_t950 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' points into the buffer of a List that was grown, shrunk or cleared after the pointer was taken; the buffer may have moved or been freed, so the pointer dangles.\n      FIX: take the pointer (`l as Pointer[T]`) again after the last mutation of the list, or index the list instead of holding a raw pointer.", 305LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t950); _tr_str_release(_at_t950); });
#line 505 "src/ptrflow.tr"
            st->inval = pf_drop_under(st->inval, nm);
        }
    } else if (_t949.tag == HirExpr_EPropAccess) {
        __auto_type o = _t949.data.EPropAccess.obj;
__auto_type p = _t949.data.EPropAccess.prop;
#line 507 "src/ptrflow.tr"
        if (((!pf_null(o)) && _tr_str_eqv((hir_expr_type(o)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 508 "src/ptrflow.tr"
            TrStr wh = _tr_str_lit_len("inside an 'unsafe:' block ", 26LL);
#line 509 "src/ptrflow.tr"
            if ((cx->unsafe_depth == 0LL)) {
#line 509 "src/ptrflow.tr"
                TrStr _strtmp_t951 = _tr_str_lit_len("", 0LL);
                _tr_str_release(wh);
                wh = _strtmp_t951;
            }
#line 510 "src/ptrflow.tr"
            ({ TrStr _at_t952 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-2] '.", 8LL)), (p))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a field access through a raw Pointer, which has no fields.\n      FIX: read the pointee ", 92LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (wh)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("first, e.g. `unsafe:` then `p.read().", 37LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("` (and `p.write(v)` to store a whole value), or keep the data in a class / List[T].", 83LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t952); _tr_str_release(_at_t952); });
        }
#line 511 "src/ptrflow.tr"
        TrStr pp = pf_path(e);
#line 512 "src/ptrflow.tr"
        if (((!_tr_str_eqv((pp), (_tr_str_lit_len("", 0LL)))) && pf_has(st->freed, pp))) {
#line 513 "src/ptrflow.tr"
            pf_use_msg(cx, pp);
#line 514 "src/ptrflow.tr"
            pf_kill(st, pp);
        }
#line 515 "src/ptrflow.tr"
        pf_expr(cx, o, st);
        _tr_str_release(pp);
    } else if (_t949.tag == HirExpr_EBinOp) {
        __auto_type bop = _t949.data.EBinOp.op;
__auto_type l = _t949.data.EBinOp.left;
__auto_type r = _t949.data.EBinOp.right;
#line 517 "src/ptrflow.tr"
        if (((((cx->unsafe_depth == 0LL) && (_tr_str_eqv((bop), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((bop), (_tr_str_lit_len("-", 1LL))))) && (!pf_null(l))) && _tr_str_eqv((hir_expr_type(l)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 518 "src/ptrflow.tr"
            ({ TrStr _at_t953 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-2] pointer arithmetic with '", 31LL)), (bop))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' can create an out-of-bounds pointer and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a safe slice / List[T] instead of pointer math.", 164LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t953); _tr_str_release(_at_t953); });
        }
#line 519 "src/ptrflow.tr"
        pf_expr(cx, l, st);
#line 520 "src/ptrflow.tr"
        pf_expr(cx, r, st);
    } else if (_t949.tag == HirExpr_EUnaryOp) {
        __auto_type uop = _t949.data.EUnaryOp.op;
__auto_type x = _t949.data.EUnaryOp.expr;
#line 522 "src/ptrflow.tr"
        if (((((cx->unsafe_depth == 0LL) && (_tr_str_eqv((uop), (_tr_str_lit_len("*", 1LL))) || _tr_str_eqv((uop), (_tr_str_lit_len("deref", 5LL))))) && (!pf_null(x))) && _tr_str_eqv((hir_expr_type(x)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 523 "src/ptrflow.tr"
            PfCtx_err(cx, _tr_str_lit_len("[P-2] '*' dereferences a raw pointer (may read freed or invalid memory) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or hold the value in a safe Box[T] / List[T] / class reference instead of a raw pointer.", 232LL));
        }
#line 524 "src/ptrflow.tr"
        pf_expr(cx, x, st);
    } else if (_t949.tag == HirExpr_ECall) {
        __auto_type callee = _t949.data.ECall.callee;
__auto_type args = _t949.data.ECall.args;
#line 526 "src/ptrflow.tr"
        if (pf_is_dealloc(callee)) {
#line 527 "src/ptrflow.tr"
            if ((args->len > 0LL)) {
#line 528 "src/ptrflow.tr"
                TrStr dp = pf_root(((HirExpr*)List_ptr_get(args, 0LL)));
#line 529 "src/ptrflow.tr"
                if (((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL)))) && pf_has(st->freed, dp))) {
#line 530 "src/ptrflow.tr"
                    ({ TrStr _at_t954 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (dp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is deallocated twice (double free): it was already freed by 'dealloc()' on this or another path (an alias, a call, or the previous loop iteration).\n      FIX: dealloc each allocation exactly once; clear or re-assign the pointer after dealloc, and break out of the loop right after freeing.", 291LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t954); _tr_str_release(_at_t954); });
#line 531 "src/ptrflow.tr"
                    pf_kill(st, dp);
                }
#line 532 "src/ptrflow.tr"
                TrStr dss = pf_stack_src(((HirExpr*)List_ptr_get(args, 0LL)), st, cx);
#line 533 "src/ptrflow.tr"
                if ((!_tr_str_eqv((dss), (_tr_str_lit_len("", 0LL))))) {
#line 534 "src/ptrflow.tr"
                    ({ TrStr _at_t955 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] dealloc() of a pointer to the stack local '", 49LL)), (dss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': only memory returned by alloc[T] may be deallocated.\n      FIX: remove the dealloc, or allocate the block with alloc[T] instead of taking the address of a local.", 164LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t955); _tr_str_release(_at_t955); });
                } else if (((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL)))) && pf_has(st->nul, dp))) {
#line 536 "src/ptrflow.tr"
                    ({ TrStr _at_t956 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] dealloc() of '", 20LL)), (dp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is null here — there is nothing to free.\n      FIX: remove the dealloc (or guard it with a null check on a pointer that may be non-null).", 148LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t956); _tr_str_release(_at_t956); });
                }
#line 537 "src/ptrflow.tr"
                pf_expr(cx, ((HirExpr*)List_ptr_get(args, 0LL)), st);
#line 538 "src/ptrflow.tr"
                if (((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL)))) && (!cx->in_defer))) {
#line 538 "src/ptrflow.tr"
                    pf_free(st, dp);
                }
#line 539 "src/ptrflow.tr"
                if ((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL))))) {
#line 539 "src/ptrflow.tr"
                    st->own = pf_drop_under(st->own, dp);
                }
            }
#line 540 "src/ptrflow.tr"
            return;
        }
#line 541 "src/ptrflow.tr"
        TrStr cn = pf_callee_name(callee);
#line 542 "src/ptrflow.tr"
        if ((((!_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) && (cx->unsafe_depth == 0LL)) && _tr_dict_contains(cx->externs, _tr_strz(cn)))) {
#line 543 "src/ptrflow.tr"
            ({ TrStr _at_t957 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-5] extern function '", 23LL)), (cn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' takes a raw Pointer; C cannot check it, so the call must be inside an 'unsafe:' block.\n      FIX: wrap the call in 'unsafe:' (ideally inside a small safe wrapper function that validates its arguments).", 203LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t957); _tr_str_release(_at_t957); });
        }
#line 544 "src/ptrflow.tr"
        if ((_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL))) || pf_has(st->freed, cn))) {
#line 545 "src/ptrflow.tr"
            pf_expr(cx, callee, st);
        }
#line 546 "src/ptrflow.tr"
        pf_exprs(cx, args, st);
#line 547 "src/ptrflow.tr"
        long long ki = 0LL;
#line 548 "src/ptrflow.tr"
        while ((ki < args->len)) {
#line 549 "src/ptrflow.tr"
            TrStr kp = pf_path(((HirExpr*)List_ptr_get(args, ki)));
#line 550 "src/ptrflow.tr"
            if ((!_tr_str_eqv((kp), (_tr_str_lit_len("", 0LL))))) {
#line 550 "src/ptrflow.tr"
                pf_kill_fields(st, kp);
            }
#line 551 "src/ptrflow.tr"
            ki = (ki + 1LL);
            _tr_str_release(kp);
        }
#line 552 "src/ptrflow.tr"
        if (((!_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(cx->summaries, _tr_strz(cn)))) {
#line 553 "src/ptrflow.tr"
            List_i64* fr = ((List_i64*)(uintptr_t)_tr_dict_get(cx->summaries, _tr_strz(cn)));
#line 554 "src/ptrflow.tr"
            long long k = 0LL;
#line 555 "src/ptrflow.tr"
            while ((k < fr->len)) {
#line 556 "src/ptrflow.tr"
                if (({ __auto_type _ord_t958 = (List_i64_get(fr, k)); __auto_type _wr = ((_ord_t958 < args->len)); _wr; })) {
#line 557 "src/ptrflow.tr"
                    TrStr ap = pf_root(((HirExpr*)List_ptr_get(args, List_i64_get(fr, k))));
#line 558 "src/ptrflow.tr"
                    if ((!_tr_str_eqv((ap), (_tr_str_lit_len("", 0LL))))) {
#line 558 "src/ptrflow.tr"
                        pf_free(st, ap);
                    }
                }
#line 559 "src/ptrflow.tr"
                k = (k + 1LL);
            }
        }
        _tr_str_release(cn);
    } else if (_t949.tag == HirExpr_EMethodCall) {
        __auto_type o = _t949.data.EMethodCall.obj;
__auto_type m = _t949.data.EMethodCall.method;
__auto_type args = _t949.data.EMethodCall.args;
#line 561 "src/ptrflow.tr"
        if (((_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))))) {
#line 562 "src/ptrflow.tr"
            pf_ptr_checks(cx, st, o, m, args);
        }
#line 563 "src/ptrflow.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL))))) {
#line 564 "src/ptrflow.tr"
            cx->noesc = true;
#line 565 "src/ptrflow.tr"
            pf_expr(cx, o, st);
#line 566 "src/ptrflow.tr"
            cx->noesc = false;
        } else {
#line 568 "src/ptrflow.tr"
            pf_expr(cx, o, st);
        }
#line 569 "src/ptrflow.tr"
        pf_exprs(cx, args, st);
#line 570 "src/ptrflow.tr"
        TrStr mkp = pf_path(o);
#line 571 "src/ptrflow.tr"
        if ((!_tr_str_eqv((mkp), (_tr_str_lit_len("", 0LL))))) {
#line 571 "src/ptrflow.tr"
            pf_kill_fields(st, mkp);
        }
#line 572 "src/ptrflow.tr"
        if (((!_tr_str_eqv((mkp), (_tr_str_lit_len("", 0LL)))) && (((((((((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("resize", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reserve", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("truncate", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_back", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop_back", 8LL)))))) {
#line 573 "src/ptrflow.tr"
            TrStr ot = _tr_str_retain(hir_expr_type(o)->name);
#line 574 "src/ptrflow.tr"
            if ((_tr_str_eqv((ot), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ot), (_tr_str_lit_len("Vec", 3LL))))) {
#line 575 "src/ptrflow.tr"
                long long li = 0LL;
#line 576 "src/ptrflow.tr"
                while ((li < st->pa->len)) {
#line 577 "src/ptrflow.tr"
                    if ((({ TrStr _cmpa_t959 = (List_TrStr_get(st->pa, li)); TrStr _cmpb_t960 = (mkp); _Bool _cmpr_t961 = _tr_str_eqv(_cmpa_t959, _cmpb_t960); _tr_str_release(_cmpa_t959); _cmpr_t961; }) && ({ TrStr _cmpa_t962 = (List_TrStr_get(st->pb, li)); TrStr _cmpb_t963 = (mkp); _Bool _cmpr_t964 = !_tr_str_eqv(_cmpa_t962, _cmpb_t963); _tr_str_release(_cmpa_t962); _cmpr_t964; }))) {
#line 577 "src/ptrflow.tr"
                        ({ __auto_type _ord_t966 = (st->inval); TrStr _at_t965 = (List_TrStr_get(st->pb, li)); pf_add(_ord_t966, _at_t965); _tr_str_release(_at_t965); });
                    }
#line 578 "src/ptrflow.tr"
                    li = (li + 1LL);
                }
            }
        }
#line 579 "src/ptrflow.tr"
        if ((!cx->is_main)) {
#line 581 "src/ptrflow.tr"
            if ((((((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("send", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL))))) {
#line 582 "src/ptrflow.tr"
                long long ai = 0LL;
#line 583 "src/ptrflow.tr"
                while ((ai < args->len)) {
#line 584 "src/ptrflow.tr"
                    TrStr ss = pf_stack_src(((HirExpr*)List_ptr_get(args, ai)), st, cx);
#line 585 "src/ptrflow.tr"
                    if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
#line 586 "src/ptrflow.tr"
                        TrStr rp = pf_path(o);
#line 587 "src/ptrflow.tr"
                        TrStr r0 = _tr_str_retain(rp);
#line 588 "src/ptrflow.tr"
                        long long dot = _tr_str_index_ofv((r0), (_tr_str_lit_len(".", 1LL)));
#line 589 "src/ptrflow.tr"
                        if ((dot >= 0LL)) {
#line 589 "src/ptrflow.tr"
                            TrStr _strtmp_t967 = _tr_str_slicev((r0), 0LL, dot);
                            _tr_str_release(r0);
                            r0 = _strtmp_t967;
                        }
#line 590 "src/ptrflow.tr"
                        if (((_tr_str_eqv((r0), (_tr_str_lit_len("", 0LL))) || (!pf_has(cx->locals, r0))) || _tr_str_eqv((r0), (_tr_str_lit_len("self", 4LL))))) {
#line 591 "src/ptrflow.tr"
                            if (((!_tr_str_eqv((r0), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL))))) {
#line 592 "src/ptrflow.tr"
                                ({ TrStr _at_t968 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] the address of local '", 28LL)), (ss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is stored (via '.", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()') into memory that outlives this call of '", 45LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cx->fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it dangles once the function returns.\n      FIX: store an owned value, or heap-allocate the data with alloc[T] (and dealloc it when its owner is done) instead of pointing at a stack local.", 194LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t968); _tr_str_release(_at_t968); });
                            }
                        }
                    }
#line 593 "src/ptrflow.tr"
                    ai = (ai + 1LL);
                    _tr_str_release(ss);
                }
            }
        }
        _tr_str_release(mkp);
    } else if (_t949.tag == HirExpr_EIndex) {
        __auto_type o = _t949.data.EIndex.obj;
__auto_type i = _t949.data.EIndex._tr_v_index;
#line 595 "src/ptrflow.tr"
        if ((((cx->unsafe_depth == 0LL) && (!pf_null(o))) && _tr_str_eqv((hir_expr_type(o)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 596 "src/ptrflow.tr"
            PfCtx_err(cx, _tr_str_lit_len("[P-2] indexing a raw Pointer reads or writes unchecked memory and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a List[T] (bounds-checked).", 168LL));
        }
#line 597 "src/ptrflow.tr"
        TrStr ip = pf_path(o);
#line 598 "src/ptrflow.tr"
        if ((!_tr_str_eqv((ip), (_tr_str_lit_len("", 0LL))))) {
#line 599 "src/ptrflow.tr"
            if (pf_has(st->nul, ip)) {
#line 600 "src/ptrflow.tr"
                ({ TrStr _at_t969 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] indexing '", 16LL)), (ip))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is a null pointer here.\n      FIX: allocate it (alloc[T](n)) or check it for null before use.", 102LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t969); _tr_str_release(_at_t969); });
            } else {
#line 602 "src/ptrflow.tr"
                long long isz = pf_size_of(st, ip);
#line 603 "src/ptrflow.tr"
                long long ik = pf_lit(i);
#line 604 "src/ptrflow.tr"
                if ((((isz >= 0LL) && pf_is_lit(i)) && ((ik < 0LL) || (ik >= isz)))) {
#line 605 "src/ptrflow.tr"
                    ({ __auto_type _ord_t970 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ik)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[P-3] index ", 12LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is outside the ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t971 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t970); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(isz)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("-element block '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ip)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was allocated with.\n      FIX: keep indexes in 0 .. n-1 of the alloc[T](n) block, or use a List[T] (bounds-checked).", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t971); _tr_str_release(_at_t971); });
                }
            }
        }
#line 606 "src/ptrflow.tr"
        cx->noesc = true;
#line 607 "src/ptrflow.tr"
        pf_expr(cx, o, st);
#line 608 "src/ptrflow.tr"
        cx->noesc = false;
#line 609 "src/ptrflow.tr"
        pf_expr(cx, i, st);
        _tr_str_release(ip);
    } else if (_t949.tag == HirExpr_ECast) {
        __auto_type x = _t949.data.ECast.expr;
        pf_expr(cx, x, st);
    } else if (_t949.tag == HirExpr_EFString) {
        __auto_type parts = _t949.data.EFString.parts;
#line 612 "src/ptrflow.tr"
        long long pi = 0LL;
#line 613 "src/ptrflow.tr"
        while ((pi < parts->len)) {
#line 614 "src/ptrflow.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, pi));
#line 615 "src/ptrflow.tr"
            if (part->is_expr) {
#line 616 "src/ptrflow.tr"
                pf_expr(cx, part->expr, st);
#line 617 "src/ptrflow.tr"
                pf_exprs(cx, part->spec_args, st);
            }
#line 618 "src/ptrflow.tr"
            pi = (pi + 1LL);
        }
    } else if (_t949.tag == HirExpr_ETryExpr) {
        __auto_type x = _t949.data.ETryExpr.expr;
        pf_expr(cx, x, st);
    } else if (_t949.tag == HirExpr_EClosure) {
        __auto_type body = _t949.data.EClosure.body;
__auto_type caps = _t949.data.EClosure.captures;
#line 621 "src/ptrflow.tr"
        long long cpi = 0LL;
#line 622 "src/ptrflow.tr"
        while ((cpi < caps->len)) {
#line 623 "src/ptrflow.tr"
            st->own = ({ __auto_type _ord_t973 = (st->own); TrStr _apt_t972 = _tr_str_retain(((HirParam*)List_ptr_get(caps, cpi))->name); __auto_type _wr = (pf_drop_under(_ord_t973, _apt_t972)); _tr_str_release(_apt_t972); _wr; });
#line 624 "src/ptrflow.tr"
            cpi = (cpi + 1LL);
        }
#line 625 "src/ptrflow.tr"
        PfState* c = PfState_copy(st);
#line 626 "src/ptrflow.tr"
        pf_block(cx, body, c);
        _tr_obj_release(c, _trdrop_PfState);
    } else if (_t949.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t949.data.ESuperMethodCall.args;
        pf_exprs(cx, args, st);
    } else if (_t949.tag == HirExpr_EList) {
        __auto_type items = _t949.data.EList.items;
        pf_exprs(cx, items, st);
    } else if (_t949.tag == HirExpr_ESet) {
        __auto_type items = _t949.data.ESet.items;
        pf_exprs(cx, items, st);
    } else if (_t949.tag == HirExpr_ETuple) {
        __auto_type items = _t949.data.ETuple.items;
        pf_exprs(cx, items, st);
    } else if (_t949.tag == HirExpr_EDict) {
        __auto_type keys = _t949.data.EDict.keys;
__auto_type vals = _t949.data.EDict.vals;
#line 632 "src/ptrflow.tr"
        pf_exprs(cx, keys, st);
#line 633 "src/ptrflow.tr"
        pf_exprs(cx, vals, st);
    } else if (_t949.tag == HirExpr_EListComp) {
        __auto_type el = _t949.data.EListComp.element;
__auto_type gens = _t949.data.EListComp.generators;
#line 635 "src/ptrflow.tr"
        long long gi = 0LL;
#line 636 "src/ptrflow.tr"
        while ((gi < gens->len)) {
#line 637 "src/ptrflow.tr"
            pf_expr(cx, (*((HirComprehension**)List_ptr_get(gens, gi)))->iter, st);
#line 638 "src/ptrflow.tr"
            gi = (gi + 1LL);
        }
    } else if (_t949.tag == HirExpr_ESlice) {
        __auto_type a = _t949.data.ESlice.start;
__auto_type b = _t949.data.ESlice.stop;
__auto_type c = _t949.data.ESlice.step;
#line 640 "src/ptrflow.tr"
        pf_expr(cx, a, st);
#line 641 "src/ptrflow.tr"
        pf_expr(cx, b, st);
#line 642 "src/ptrflow.tr"
        pf_expr(cx, c, st);
    } else if (_t949.tag == HirExpr_EAwait) {
        __auto_type x = _t949.data.EAwait.expr;
        pf_expr(cx, x, st);
    } else if (_t949.tag == HirExpr_EAwaitTimeout) {
        __auto_type x = _t949.data.EAwaitTimeout.expr;
__auto_type t = _t949.data.EAwaitTimeout.timeout_ms;
#line 645 "src/ptrflow.tr"
        pf_expr(cx, x, st);
#line 646 "src/ptrflow.tr"
        pf_expr(cx, t, st);
    } else if (_t949.tag == HirExpr_EYield) {
        __auto_type x = _t949.data.EYield.expr;
        pf_expr(cx, x, st);
    } else if (_t949.tag == HirExpr_ERange) {
        __auto_type a = _t949.data.ERange.start;
__auto_type b = _t949.data.ERange.end;
#line 649 "src/ptrflow.tr"
        pf_expr(cx, a, st);
#line 650 "src/ptrflow.tr"
        pf_expr(cx, b, st);
    } else if (_t949.tag == HirExpr_EIfElse) {
        __auto_type c = _t949.data.EIfElse.cond;
__auto_type t = _t949.data.EIfElse.then_e;
__auto_type f = _t949.data.EIfElse.else_e;
#line 652 "src/ptrflow.tr"
        pf_expr(cx, c, st);
#line 653 "src/ptrflow.tr"
        PfState* s1 = PfState_copy(st);
#line 654 "src/ptrflow.tr"
        PfState* s2 = PfState_copy(st);
#line 655 "src/ptrflow.tr"
        pf_expr(cx, t, s1);
#line 656 "src/ptrflow.tr"
        pf_expr(cx, f, s2);
#line 657 "src/ptrflow.tr"
        s1->dead = false;
#line 658 "src/ptrflow.tr"
        s2->dead = false;
#line 659 "src/ptrflow.tr"
        pf_assign_from(st, s1);
#line 660 "src/ptrflow.tr"
        pf_merge(st, s2);
        _tr_obj_release(s1, _trdrop_PfState);
        _tr_obj_release(s2, _trdrop_PfState);
    } else if (_t949.tag == HirExpr_EDo) {
        __auto_type body = _t949.data.EDo.body;
        pf_branchy_block(cx, body, st);
    } else if (_t949.tag == HirExpr_ELoop) {
        __auto_type body = _t949.data.ELoop.body;
        pf_branchy_block(cx, body, st);
    } else if (_t949.tag == HirExpr_EWhileExpr) {
        __auto_type c = _t949.data.EWhileExpr.cond;
__auto_type body = _t949.data.EWhileExpr.body;
__auto_type eb = _t949.data.EWhileExpr.else_body;
#line 664 "src/ptrflow.tr"
        pf_expr(cx, c, st);
#line 665 "src/ptrflow.tr"
        pf_branchy_block(cx, body, st);
#line 666 "src/ptrflow.tr"
        pf_branchy_block(cx, eb, st);
    } else if (_t949.tag == HirExpr_EMatchExpr) {
        __auto_type subj = _t949.data.EMatchExpr.subj;
__auto_type arms = _t949.data.EMatchExpr.arms;
#line 668 "src/ptrflow.tr"
        pf_expr(cx, subj, st);
#line 669 "src/ptrflow.tr"
        long long ai = 0LL;
#line 670 "src/ptrflow.tr"
        while ((ai < arms->len)) {
#line 671 "src/ptrflow.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 672 "src/ptrflow.tr"
            PfState* c = PfState_copy(st);
#line 673 "src/ptrflow.tr"
            pf_expr(cx, a->guard, c);
#line 674 "src/ptrflow.tr"
            pf_block(cx, a->body, c);
#line 675 "src/ptrflow.tr"
            pf_merge(st, c);
#line 676 "src/ptrflow.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c, _trdrop_PfState);
        }
    } else if (_t949.tag == HirExpr_ETry) {
        __auto_type tb = _t949.data.ETry.try_body;
__auto_type catches = _t949.data.ETry.catches;
__auto_type fb = _t949.data.ETry.finally_b;
#line 678 "src/ptrflow.tr"
        pf_branchy_block(cx, tb, st);
#line 679 "src/ptrflow.tr"
        long long ci = 0LL;
#line 680 "src/ptrflow.tr"
        while ((ci < catches->len)) {
#line 681 "src/ptrflow.tr"
            pf_branchy_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body, st);
#line 682 "src/ptrflow.tr"
            ci = (ci + 1LL);
        }
#line 683 "src/ptrflow.tr"
        pf_branchy_block(cx, fb, st);
    } else if (1) {
        __auto_type _ = _t949;
        /* pass */
    }
}

__attribute__((hot)) void pf_leak_warn(PfCtx* cx, TrStr name, TrStr why) {
#line 689 "src/ptrflow.tr"
    if (_tr_str_starts_withv((cx->fn_name), (_tr_str_lit_len("<", 1LL)))) {
#line 689 "src/ptrflow.tr"
        return;
    }
#line 690 "src/ptrflow.tr"
    long long ln = 0LL;
#line 691 "src/ptrflow.tr"
    if (_tr_dict_contains(cx->alloc_line, _tr_strz(name))) {
#line 691 "src/ptrflow.tr"
        ln = ((long long)(uintptr_t)_tr_dict_get(cx->alloc_line, _tr_strz(name)));
    }
#line 692 "src/ptrflow.tr"
    TrStr at = _tr_str_lit_len("", 0LL);
#line 693 "src/ptrflow.tr"
    if ((ln > 0LL)) {
#line 693 "src/ptrflow.tr"
        TrStr _strtmp_t974 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(" (allocated at line ", 20LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(at);
        at = _strtmp_t974;
    }
#line 694 "src/ptrflow.tr"
    ({ TrStr _at_t975 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-4] the alloc[] block held by '", 33LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (why)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": it leaks.\n      FIX: dealloc(", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") on every path, return/store the pointer so a caller owns it, or wrap the pair in a class with init/drop.", 106LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t975); _tr_str_release(_at_t975); });
    _tr_str_release(at);
}

__attribute__((hot)) void pf_leak_check(PfCtx* cx, PfState* st, TrStr why) {
#line 697 "src/ptrflow.tr"
    if (st->dead) {
#line 697 "src/ptrflow.tr"
        return;
    }
#line 698 "src/ptrflow.tr"
    long long i = 0LL;
#line 699 "src/ptrflow.tr"
    while ((i < st->own->len)) {
#line 700 "src/ptrflow.tr"
        ({ TrStr _at_t976 = (List_TrStr_get(st->own, i)); pf_leak_warn(cx, _at_t976, why); _tr_str_release(_at_t976); });
#line 701 "src/ptrflow.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void pf_bind(PfCtx* cx, PfState* st, TrStr path, HirExpr* val, bool simple) {
#line 705 "src/ptrflow.tr"
    if (pf_is_alloc(val)) {
#line 706 "src/ptrflow.tr"
        if (simple) {
#line 707 "src/ptrflow.tr"
            pf_add(st->own, path);
#line 708 "src/ptrflow.tr"
            _tr_dict_set(cx->alloc_line, _tr_strz(path), cx->line);
        }
#line 709 "src/ptrflow.tr"
        long long n = pf_alloc_n(val);
#line 710 "src/ptrflow.tr"
        if ((n != (-4611686018427387904LL))) {
#line 711 "src/ptrflow.tr"
            if ((n < 0LL)) {
#line 712 "src/ptrflow.tr"
                ({ TrStr _at_t977 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[P-3] alloc with a negative element count (", 43LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: pass a non-negative element count.", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t977); _tr_str_release(_at_t977); });
            } else {
#line 714 "src/ptrflow.tr"
                ({ TrStr _ord_t978 = (({ TrStr _tr_cl = (_tr_strx_concatv((path), (_tr_str_lit_len("=", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(st->sz, _ord_t978); _tr_str_release(_ord_t978); });
            }
        }
    } else if (pf_is_null_lit(val)) {
#line 716 "src/ptrflow.tr"
        pf_add(st->nul, path);
    } else {
#line 718 "src/ptrflow.tr"
        TrStr r = pf_root(val);
#line 719 "src/ptrflow.tr"
        if ((((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) && pf_has(st->nul, r)) && (!_tr_str_eqv((r), (path))))) {
#line 719 "src/ptrflow.tr"
            pf_add(st->nul, path);
        }
        _tr_str_release(r);
    }
}

__attribute__((hot)) void pf_declare(PfCtx* cx, PfState* st, TrStr name, HirExpr* val) {
#line 722 "src/ptrflow.tr"
    if (pf_has(st->own, name)) {
#line 722 "src/ptrflow.tr"
        pf_leak_warn(cx, name, _tr_str_lit_len("overwritten by a new declaration before it is dealloc'd", 55LL));
    }
#line 723 "src/ptrflow.tr"
    pf_kill(st, name);
#line 724 "src/ptrflow.tr"
    if ((!pf_has(cx->locals, name))) {
#line 724 "src/ptrflow.tr"
        List_TrStr_append(cx->locals, name);
    }
#line 725 "src/ptrflow.tr"
    TrStr r = pf_root(val);
#line 726 "src/ptrflow.tr"
    if (((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((r), (name))))) {
#line 726 "src/ptrflow.tr"
        pf_link(st, name, r);
    }
#line 727 "src/ptrflow.tr"
    if ((!pf_null(val))) {
#line 728 "src/ptrflow.tr"
        __auto_type _t979 = (*val);
        if (_t979.tag == HirExpr_EClosure) {
            __auto_type ccaps = _t979.data.EClosure.captures;
#line 730 "src/ptrflow.tr"
            long long cci = 0LL;
#line 731 "src/ptrflow.tr"
            while ((cci < ccaps->len)) {
#line 732 "src/ptrflow.tr"
                ({ TrStr _apt_t980 = _tr_str_retain(((HirParam*)List_ptr_get(ccaps, cci))->name); pf_link(st, name, _apt_t980); _tr_str_release(_apt_t980); });
#line 733 "src/ptrflow.tr"
                cci = (cci + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t979;
            /* pass */
        }
    }
#line 735 "src/ptrflow.tr"
    TrStr ss = pf_stack_src(val, st, cx);
#line 736 "src/ptrflow.tr"
    if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
#line 737 "src/ptrflow.tr"
        pf_add(st->stk, name);
#line 738 "src/ptrflow.tr"
        _tr_dict_set_str(cx->origin, _tr_strz(name), _tr_str_box(_tr_str_retain(ss)));
    }
#line 739 "src/ptrflow.tr"
    pf_bind(cx, st, name, val, true);
    _tr_str_release(r);
    _tr_str_release(ss);
}

__attribute__((hot)) void pf_assign(PfCtx* cx, PfState* st, HirExpr* tgt, HirExpr* val) {
#line 742 "src/ptrflow.tr"
    pf_expr(cx, val, st);
#line 743 "src/ptrflow.tr"
    TrStr tp = pf_path(tgt);
#line 744 "src/ptrflow.tr"
    if (_tr_str_eqv((tp), (_tr_str_lit_len("", 0LL)))) {
#line 745 "src/ptrflow.tr"
        pf_expr(cx, tgt, st);
#line 746 "src/ptrflow.tr"
        _tr_str_release(tp);
        return;
    }
#line 747 "src/ptrflow.tr"
    __auto_type _t981 = (*tgt);
    if (_t981.tag == HirExpr_EPropAccess) {
        __auto_type o = _t981.data.EPropAccess.obj;
        pf_expr(cx, o, st);
    } else if (1) {
        __auto_type _ = _t981;
        /* pass */
    }
#line 750 "src/ptrflow.tr"
    TrStr ss = pf_stack_src(val, st, cx);
#line 751 "src/ptrflow.tr"
    TrStr root0 = _tr_str_retain(tp);
#line 752 "src/ptrflow.tr"
    long long dot = _tr_str_index_ofv((root0), (_tr_str_lit_len(".", 1LL)));
#line 753 "src/ptrflow.tr"
    if ((dot >= 0LL)) {
#line 753 "src/ptrflow.tr"
        TrStr _strtmp_t982 = _tr_str_slicev((root0), 0LL, dot);
        _tr_str_release(root0);
        root0 = _strtmp_t982;
    }
#line 754 "src/ptrflow.tr"
    bool outlives = false;
#line 755 "src/ptrflow.tr"
    if ((dot >= 0LL)) {
#line 757 "src/ptrflow.tr"
        if (((!pf_has(cx->locals, root0)) || _tr_str_eqv((root0), (_tr_str_lit_len("self", 4LL))))) {
#line 757 "src/ptrflow.tr"
            outlives = true;
        }
    } else if (((!pf_has(cx->locals, root0)) && (!pf_has(cx->params, root0)))) {
#line 759 "src/ptrflow.tr"
        outlives = true;
    }
#line 760 "src/ptrflow.tr"
    if ((((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL)))) && outlives) && (!cx->is_main))) {
#line 761 "src/ptrflow.tr"
        ({ TrStr _at_t983 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] the address of local '", 28LL)), (ss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is stored into '", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which outlives this call of '", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cx->fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it dangles once the function returns.\n      FIX: store an owned value, or heap-allocate the data with alloc[T] (and dealloc it when its owner is done) instead of pointing at a stack local.", 194LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t983); _tr_str_release(_at_t983); });
    }
#line 762 "src/ptrflow.tr"
    if (((dot < 0LL) && pf_has(st->own, tp))) {
#line 762 "src/ptrflow.tr"
        pf_leak_warn(cx, tp, _tr_str_lit_len("overwritten before it is dealloc'd", 34LL));
    }
#line 763 "src/ptrflow.tr"
    pf_kill(st, tp);
#line 764 "src/ptrflow.tr"
    TrStr r = pf_root(val);
#line 765 "src/ptrflow.tr"
    if ((((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((r), (tp)))) && (!pf_under(r, tp)))) {
#line 765 "src/ptrflow.tr"
        pf_link(st, tp, r);
    }
#line 766 "src/ptrflow.tr"
    if (((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL)))) && (dot < 0LL))) {
#line 767 "src/ptrflow.tr"
        pf_add(st->stk, tp);
#line 768 "src/ptrflow.tr"
        _tr_dict_set_str(cx->origin, _tr_strz(tp), _tr_str_box(_tr_str_retain(ss)));
    }
#line 769 "src/ptrflow.tr"
    pf_bind(cx, st, tp, val, ((dot < 0LL) && pf_has(cx->locals, tp)));
    _tr_str_release(tp);
    _tr_str_release(ss);
    _tr_str_release(root0);
    _tr_str_release(r);
}

__attribute__((hot)) void pf_exit_loop_ctx(PfCtx* cx, PfState* brk_out, PfState* cnt_out) {
#line 772 "src/ptrflow.tr"
    long long bb = List_i64_get(cx->brk_base, (cx->brk_base->len - 1LL));
#line 773 "src/ptrflow.tr"
    List_i64_pop(cx->brk_base);
#line 774 "src/ptrflow.tr"
    while ((cx->brk->len > bb)) {
#line 775 "src/ptrflow.tr"
        pf_merge(brk_out, ((PfState*)List_ptr_get(cx->brk, (cx->brk->len - 1LL))));
#line 776 "src/ptrflow.tr"
        ((PfState*)List_ptr_pop(cx->brk));
    }
#line 777 "src/ptrflow.tr"
    long long cb = List_i64_get(cx->cnt_base, (cx->cnt_base->len - 1LL));
#line 778 "src/ptrflow.tr"
    List_i64_pop(cx->cnt_base);
#line 779 "src/ptrflow.tr"
    while ((cx->cnt->len > cb)) {
#line 780 "src/ptrflow.tr"
        pf_merge(cnt_out, ((PfState*)List_ptr_get(cx->cnt, (cx->cnt->len - 1LL))));
#line 781 "src/ptrflow.tr"
        ((PfState*)List_ptr_pop(cx->cnt));
    }
}

__attribute__((hot)) void pf_loop(PfCtx* cx, PfState* st, HirExpr* cond, TrStr lv, HirBlock* body, bool always) {
#line 785 "src/ptrflow.tr"
    PfState* entry = PfState_copy(st);
#line 786 "src/ptrflow.tr"
    PfState* head = PfState_copy(entry);
#line 787 "src/ptrflow.tr"
    long long pass_i = 0LL;
#line 788 "src/ptrflow.tr"
    PfState* exit_st = PfState_init();
#line 789 "src/ptrflow.tr"
    exit_st->dead = true;
#line 790 "src/ptrflow.tr"
    while ((pass_i < 2LL)) {
#line 791 "src/ptrflow.tr"
        PfState* s = PfState_copy(head);
#line 792 "src/ptrflow.tr"
        pf_expr(cx, cond, s);
#line 793 "src/ptrflow.tr"
        PfState* after_cond = PfState_copy(s);
#line 794 "src/ptrflow.tr"
        TrStr ltest = pf_nulltest(cond);
#line 795 "src/ptrflow.tr"
        pf_refine(s, ltest, true);
#line 796 "src/ptrflow.tr"
        pf_refine(after_cond, ltest, false);
#line 797 "src/ptrflow.tr"
        if ((!_tr_str_eqv((lv), (_tr_str_lit_len("", 0LL))))) {
#line 797 "src/ptrflow.tr"
            pf_kill(s, lv);
        }
#line 798 "src/ptrflow.tr"
        List_i64_append(cx->brk_base, cx->brk->len);
#line 799 "src/ptrflow.tr"
        List_i64_append(cx->cnt_base, cx->cnt->len);
#line 800 "src/ptrflow.tr"
        pf_block(cx, body, s);
#line 801 "src/ptrflow.tr"
        PfState* bo = PfState_init();
#line 802 "src/ptrflow.tr"
        bo->dead = true;
#line 803 "src/ptrflow.tr"
        PfState* co = PfState_init();
#line 804 "src/ptrflow.tr"
        co->dead = true;
#line 805 "src/ptrflow.tr"
        pf_exit_loop_ctx(cx, bo, co);
#line 806 "src/ptrflow.tr"
        PfState* back = PfState_copy(s);
#line 807 "src/ptrflow.tr"
        pf_merge(back, co);
#line 808 "src/ptrflow.tr"
        if ((pass_i == 0LL)) {
#line 809 "src/ptrflow.tr"
            PfState* _cltmp_t984 = PfState_copy(entry);
            _tr_obj_release(head, _trdrop_PfState);
            head = _cltmp_t984;
#line 810 "src/ptrflow.tr"
            pf_merge(head, back);
        } else {
#line 812 "src/ptrflow.tr"
            if ((!always)) {
#line 812 "src/ptrflow.tr"
                pf_merge(exit_st, after_cond);
            }
#line 813 "src/ptrflow.tr"
            pf_merge(exit_st, bo);
        }
#line 814 "src/ptrflow.tr"
        pass_i = (pass_i + 1LL);
        _tr_obj_release(s, _trdrop_PfState);
        _tr_obj_release(after_cond, _trdrop_PfState);
        _tr_str_release(ltest);
        _tr_obj_release(bo, _trdrop_PfState);
        _tr_obj_release(co, _trdrop_PfState);
        _tr_obj_release(back, _trdrop_PfState);
    }
#line 815 "src/ptrflow.tr"
    pf_assign_from(st, exit_st);
    _tr_obj_release(entry, _trdrop_PfState);
    _tr_obj_release(head, _trdrop_PfState);
    _tr_obj_release(exit_st, _trdrop_PfState);
}

__attribute__((hot)) void pf_stmt(PfCtx* cx, HirStmt* sp, PfState* st) {
#line 818 "src/ptrflow.tr"
    if (st->dead) {
#line 818 "src/ptrflow.tr"
        return;
    }
#line 819 "src/ptrflow.tr"
    __auto_type _t985 = (*sp);
    if (_t985.tag == HirStmt_SLineMarker) {
        __auto_type n = _t985.data.SLineMarker.n;
        cx->line = n;
    } else if (_t985.tag == HirStmt_SExpr) {
        __auto_type e = _t985.data.SExpr.expr;
        pf_expr(cx, e, st);
    } else if (_t985.tag == HirStmt_SLet) {
        __auto_type nm = _t985.data.SLet.name;
__auto_type val = _t985.data.SLet.val;
#line 823 "src/ptrflow.tr"
        pf_expr(cx, val, st);
#line 824 "src/ptrflow.tr"
        pf_declare(cx, st, nm, val);
    } else if (_t985.tag == HirStmt_SMultiLet) {
        __auto_type names = _t985.data.SMultiLet.names;
__auto_type val = _t985.data.SMultiLet.val;
#line 826 "src/ptrflow.tr"
        pf_expr(cx, val, st);
#line 827 "src/ptrflow.tr"
        long long ni = 0LL;
#line 828 "src/ptrflow.tr"
        while ((ni < names->len)) {
#line 829 "src/ptrflow.tr"
            ({ TrStr _at_t986 = (List_TrStr_get(names, ni)); pf_kill(st, _at_t986); _tr_str_release(_at_t986); });
#line 830 "src/ptrflow.tr"
            if (({ __auto_type _ord_t988 = (cx->locals); TrStr _at_t987 = (List_TrStr_get(names, ni)); __auto_type _wr = ((!pf_has(_ord_t988, _at_t987))); _tr_str_release(_at_t987); _wr; })) {
#line 830 "src/ptrflow.tr"
                ({ TrStr _ord_t989 = (List_TrStr_get(names, ni)); List_TrStr_append(cx->locals, _ord_t989); _tr_str_release(_ord_t989); });
            }
#line 831 "src/ptrflow.tr"
            ni = (ni + 1LL);
        }
    } else if (_t985.tag == HirStmt_SAssign) {
        __auto_type tgt = _t985.data.SAssign.target;
__auto_type val = _t985.data.SAssign.val;
        pf_assign(cx, st, tgt, val);
    } else if (_t985.tag == HirStmt_SReturn) {
        __auto_type v = _t985.data.SReturn.val;
#line 834 "src/ptrflow.tr"
        pf_expr(cx, v, st);
#line 835 "src/ptrflow.tr"
        if (((!cx->ret_from) && (!cx->is_main))) {
#line 836 "src/ptrflow.tr"
            TrStr ss = pf_stack_src(v, st, cx);
#line 837 "src/ptrflow.tr"
            if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
#line 838 "src/ptrflow.tr"
                ({ TrStr _at_t990 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] returning the address of local '", 38LL)), (ss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': the local dies when '", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cx->fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' returns, so the pointer dangles.\n      FIX: return an owned value, heap-allocate with alloc[T] (the caller must dealloc it), or annotate the return type with 'from <param>' when the pointer borrows from a parameter.", 217LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t990); _tr_str_release(_at_t990); });
            }
        }
#line 839 "src/ptrflow.tr"
        pf_leak_check(cx, st, _tr_str_lit_len("not dealloc'd on this path before the return", 44LL));
#line 840 "src/ptrflow.tr"
        pf_merge(cx->ret_acc, st);
#line 841 "src/ptrflow.tr"
        cx->ret_seen = true;
#line 842 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t985.tag == HirStmt_SRaise) {
        __auto_type v = _t985.data.SRaise.val;
#line 844 "src/ptrflow.tr"
        pf_expr(cx, v, st);
#line 845 "src/ptrflow.tr"
        pf_leak_check(cx, st, _tr_str_lit_len("not dealloc'd on this path before the raise", 43LL));
#line 846 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t985.tag == HirStmt_SDefer) {
        __auto_type ds = _t985.data.SDefer.stmt;
#line 848 "src/ptrflow.tr"
        cx->in_defer = true;
#line 849 "src/ptrflow.tr"
        pf_stmt(cx, ds, st);
#line 850 "src/ptrflow.tr"
        cx->in_defer = false;
    } else if (_t985.tag == HirStmt_SBreak) {
        __auto_type v = _t985.data.SBreak.val;
#line 852 "src/ptrflow.tr"
        pf_expr(cx, v, st);
#line 853 "src/ptrflow.tr"
        if ((cx->brk_base->len > 0LL)) {
#line 853 "src/ptrflow.tr"
            ({ PfState* _ord_t991 = (PfState_copy(st)); List_ptr_append(cx->brk, _tr_obj_retain(_ord_t991)); _tr_obj_release(_ord_t991, _trdrop_PfState); });
        }
#line 854 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t985.tag == HirStmt_SContinue) {
#line 856 "src/ptrflow.tr"
        if ((cx->cnt_base->len > 0LL)) {
#line 856 "src/ptrflow.tr"
            ({ PfState* _ord_t992 = (PfState_copy(st)); List_ptr_append(cx->cnt, _tr_obj_retain(_ord_t992)); _tr_obj_release(_ord_t992, _trdrop_PfState); });
        }
#line 857 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t985.tag == HirStmt_SUnsafe) {
        __auto_type body = _t985.data.SUnsafe.body;
#line 859 "src/ptrflow.tr"
        cx->unsafe_depth = (cx->unsafe_depth + 1LL);
#line 860 "src/ptrflow.tr"
        pf_block(cx, body, st);
#line 861 "src/ptrflow.tr"
        cx->unsafe_depth = (cx->unsafe_depth - 1LL);
    } else if (_t985.tag == HirStmt_SIf) {
        __auto_type cond = _t985.data.SIf.cond;
__auto_type tb = _t985.data.SIf.then_b;
__auto_type eb = _t985.data.SIf.else_b;
#line 863 "src/ptrflow.tr"
        pf_expr(cx, cond, st);
#line 864 "src/ptrflow.tr"
        PfState* s1 = PfState_copy(st);
#line 865 "src/ptrflow.tr"
        PfState* s2 = PfState_copy(st);
#line 866 "src/ptrflow.tr"
        TrStr ntest = pf_nulltest(cond);
#line 867 "src/ptrflow.tr"
        pf_refine(s1, ntest, true);
#line 868 "src/ptrflow.tr"
        pf_refine(s2, ntest, false);
#line 869 "src/ptrflow.tr"
        pf_block(cx, tb, s1);
#line 870 "src/ptrflow.tr"
        pf_block(cx, eb, s2);
#line 871 "src/ptrflow.tr"
        pf_assign_from(st, s1);
#line 872 "src/ptrflow.tr"
        pf_merge(st, s2);
        _tr_obj_release(s1, _trdrop_PfState);
        _tr_obj_release(s2, _trdrop_PfState);
        _tr_str_release(ntest);
    } else if (_t985.tag == HirStmt_SWhile) {
        __auto_type cond = _t985.data.SWhile.cond;
__auto_type body = _t985.data.SWhile.body;
#line 874 "src/ptrflow.tr"
        bool always = false;
#line 875 "src/ptrflow.tr"
        if ((!pf_null(cond))) {
#line 876 "src/ptrflow.tr"
            __auto_type _t993 = (*cond);
            if (_t993.tag == HirExpr_ELitBool) {
                __auto_type bv = _t993.data.ELitBool.val;
                always = bv;
            } else if (1) {
                __auto_type _ = _t993;
                /* pass */
            }
        }
#line 879 "src/ptrflow.tr"
        pf_loop(cx, st, cond, _tr_str_lit_len("", 0LL), body, always);
    } else if (_t985.tag == HirStmt_SFor) {
        __auto_type var = _t985.data.SFor.var;
__auto_type it = _t985.data.SFor.iter;
__auto_type body = _t985.data.SFor.body;
#line 881 "src/ptrflow.tr"
        pf_expr(cx, it, st);
#line 882 "src/ptrflow.tr"
        if ((!pf_has(cx->locals, var))) {
#line 882 "src/ptrflow.tr"
            List_TrStr_append(cx->locals, var);
        }
#line 883 "src/ptrflow.tr"
        pf_loop(cx, st, (HirExpr*)(0LL), var, body, false);
    } else if (_t985.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t985.data.SForUnpack.vars;
__auto_type it = _t985.data.SForUnpack.iter;
__auto_type body = _t985.data.SForUnpack.body;
#line 885 "src/ptrflow.tr"
        pf_expr(cx, it, st);
#line 886 "src/ptrflow.tr"
        long long vi = 0LL;
#line 887 "src/ptrflow.tr"
        while ((vi < vars->len)) {
#line 888 "src/ptrflow.tr"
            if (({ __auto_type _ord_t995 = (cx->locals); TrStr _at_t994 = (List_TrStr_get(vars, vi)); __auto_type _wr = ((!pf_has(_ord_t995, _at_t994))); _tr_str_release(_at_t994); _wr; })) {
#line 888 "src/ptrflow.tr"
                ({ TrStr _ord_t996 = (List_TrStr_get(vars, vi)); List_TrStr_append(cx->locals, _ord_t996); _tr_str_release(_ord_t996); });
            }
#line 889 "src/ptrflow.tr"
            vi = (vi + 1LL);
        }
#line 890 "src/ptrflow.tr"
        pf_loop(cx, st, (HirExpr*)(0LL), _tr_str_lit_len("", 0LL), body, false);
    } else if (_t985.tag == HirStmt_SMatch) {
        __auto_type subj = _t985.data.SMatch.expr;
__auto_type arms = _t985.data.SMatch.arms;
#line 892 "src/ptrflow.tr"
        pf_expr(cx, subj, st);
#line 893 "src/ptrflow.tr"
        PfState* acc = PfState_init();
#line 894 "src/ptrflow.tr"
        acc->dead = true;
#line 895 "src/ptrflow.tr"
        long long ai = 0LL;
#line 896 "src/ptrflow.tr"
        while ((ai < arms->len)) {
#line 897 "src/ptrflow.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 898 "src/ptrflow.tr"
            PfState* c = PfState_copy(st);
#line 899 "src/ptrflow.tr"
            pf_expr(cx, a->guard, c);
#line 900 "src/ptrflow.tr"
            pf_block(cx, a->body, c);
#line 901 "src/ptrflow.tr"
            pf_merge(acc, c);
#line 902 "src/ptrflow.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c, _trdrop_PfState);
        }
#line 903 "src/ptrflow.tr"
        if ((arms->len > 0LL)) {
#line 904 "src/ptrflow.tr"
            pf_assign_from(st, acc);
        }
        _tr_obj_release(acc, _trdrop_PfState);
    } else if (_t985.tag == HirStmt_STry) {
        __auto_type tb = _t985.data.STry.try_body;
__auto_type catches = _t985.data.STry.catches;
__auto_type fb = _t985.data.STry.finally_b;
#line 906 "src/ptrflow.tr"
        PfState* entry = PfState_copy(st);
#line 907 "src/ptrflow.tr"
        PfState* s1 = PfState_copy(st);
#line 908 "src/ptrflow.tr"
        pf_block(cx, tb, s1);
#line 909 "src/ptrflow.tr"
        PfState* acc = PfState_copy(s1);
#line 910 "src/ptrflow.tr"
        PfState* hs = PfState_copy(entry);
#line 911 "src/ptrflow.tr"
        pf_merge(hs, s1);
#line 912 "src/ptrflow.tr"
        hs->dead = false;
#line 913 "src/ptrflow.tr"
        long long ci = 0LL;
#line 914 "src/ptrflow.tr"
        while ((ci < catches->len)) {
#line 915 "src/ptrflow.tr"
            PfState* cs = PfState_copy(hs);
#line 916 "src/ptrflow.tr"
            pf_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body, cs);
#line 917 "src/ptrflow.tr"
            pf_merge(acc, cs);
#line 918 "src/ptrflow.tr"
            ci = (ci + 1LL);
            _tr_obj_release(cs, _trdrop_PfState);
        }
#line 919 "src/ptrflow.tr"
        pf_block(cx, fb, acc);
#line 920 "src/ptrflow.tr"
        pf_assign_from(st, acc);
        _tr_obj_release(entry, _trdrop_PfState);
        _tr_obj_release(s1, _trdrop_PfState);
        _tr_obj_release(acc, _trdrop_PfState);
        _tr_obj_release(hs, _trdrop_PfState);
    } else if (_t985.tag == HirStmt_SAssert) {
        __auto_type c = _t985.data.SAssert.cond;
__auto_type m = _t985.data.SAssert.msg;
#line 922 "src/ptrflow.tr"
        pf_expr(cx, c, st);
#line 923 "src/ptrflow.tr"
        pf_expr(cx, m, st);
    } else if (_t985.tag == HirStmt_SWith) {
        __auto_type items = _t985.data.SWith.items;
__auto_type body = _t985.data.SWith.body;
#line 925 "src/ptrflow.tr"
        pf_exprs(cx, items, st);
#line 926 "src/ptrflow.tr"
        pf_block(cx, body, st);
    } else if (_t985.tag == HirStmt_SSpawn) {
        __auto_type e = _t985.data.SSpawn.expr;
        pf_expr(cx, e, st);
    } else if (_t985.tag == HirStmt_STaskGroup) {
        __auto_type body = _t985.data.STaskGroup.body;
        pf_branchy_block(cx, body, st);
    } else if (_t985.tag == HirStmt_SGpuBlock) {
        __auto_type body = _t985.data.SGpuBlock.body;
        pf_branchy_block(cx, body, st);
    } else if (1) {
        __auto_type _ = _t985;
        /* pass */
    }
}

__attribute__((hot)) List_i64* pf_run_fn(HirFunction* hf, TrMap* summaries, TrMap* externs, bool report, List_TrStr* errs) {
#line 935 "src/ptrflow.tr"
    PfCtx* cx = PfCtx_init(summaries);
#line 936 "src/ptrflow.tr"
    cx->externs = externs;
#line 937 "src/ptrflow.tr"
    cx->report = report;
#line 938 "src/ptrflow.tr"
    TrStr _sfo_t997 = cx->fn_name;
    cx->fn_name = _tr_str_retain(hf->name);
    _tr_str_release(_sfo_t997);
#line 939 "src/ptrflow.tr"
    cx->is_main = (_tr_str_eqv((hf->name), (_tr_str_lit_len("main", 4LL))) && _tr_str_eqv((hf->class_name), (_tr_str_lit_len("", 0LL))));
#line 940 "src/ptrflow.tr"
    cx->ret_from = (!_tr_str_eqv((hf->ret_ty->from_param), (_tr_str_lit_len("", 0LL))));
#line 941 "src/ptrflow.tr"
    long long pi = 0LL;
#line 942 "src/ptrflow.tr"
    while ((pi < hf->params->len)) {
#line 943 "src/ptrflow.tr"
        ({ __auto_type _ord_t998 = (((HirParam*)List_ptr_get(hf->params, pi))->name); List_TrStr_append(cx->params, _ord_t998); });
#line 944 "src/ptrflow.tr"
        pi = (pi + 1LL);
    }
#line 945 "src/ptrflow.tr"
    PfState* st = PfState_init();
#line 946 "src/ptrflow.tr"
    pf_block(cx, hf->body, st);
#line 947 "src/ptrflow.tr"
    pf_leak_check(cx, st, _tr_str_lit_len("not dealloc'd on the path that falls off the end of the function", 64LL));
#line 948 "src/ptrflow.tr"
    pf_merge(cx->ret_acc, st);
#line 949 "src/ptrflow.tr"
    List_i64* out = (void*)List_i64_new();
#line 950 "src/ptrflow.tr"
    pi = 0LL;
#line 951 "src/ptrflow.tr"
    while ((pi < hf->params->len)) {
#line 952 "src/ptrflow.tr"
        if ((({ __auto_type _ord_t1000 = (cx->ret_acc->freed); TrStr _apt_t999 = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name); __auto_type _wr = (pf_has(_ord_t1000, _apt_t999)); _tr_str_release(_apt_t999); _wr; }) && _tr_str_eqv((((HirParam*)List_ptr_get(hf->params, pi))->ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 952 "src/ptrflow.tr"
            List_i64_append(out, pi);
        }
#line 953 "src/ptrflow.tr"
        pi = (pi + 1LL);
    }
#line 954 "src/ptrflow.tr"
    long long ei = 0LL;
#line 955 "src/ptrflow.tr"
    while ((ei < cx->errs->len)) {
#line 956 "src/ptrflow.tr"
        ({ TrStr _at_t1001 = (List_TrStr_get(cx->errs, ei)); List_TrStr_append(errs, _at_t1001); _tr_str_release(_at_t1001); });
#line 957 "src/ptrflow.tr"
        ei = (ei + 1LL);
    }
#line 958 "src/ptrflow.tr"
    _tr_obj_release(cx, _trdrop_PfCtx);
    _tr_obj_release(st, _trdrop_PfState);
    return out;
}

__attribute__((hot)) List_TrStr* ptrflow_check(List_ptr* fns, TrMap* externs) {
#line 963 "src/ptrflow.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 964 "src/ptrflow.tr"
    TrMap* summaries = _tr_dict_new(8LL);
#line 965 "src/ptrflow.tr"
    long long rnd = 0LL;
#line 966 "src/ptrflow.tr"
    while ((rnd < 2LL)) {
#line 967 "src/ptrflow.tr"
        long long fi = 0LL;
#line 968 "src/ptrflow.tr"
        while ((fi < fns->len)) {
#line 969 "src/ptrflow.tr"
            HirFunction* hf = ((HirFunction*)List_ptr_get(fns, fi));
#line 970 "src/ptrflow.tr"
            if (_tr_str_eqv((hf->class_name), (_tr_str_lit_len("", 0LL)))) {
#line 971 "src/ptrflow.tr"
                List_TrStr* sink = (void*)List_TrStr_new();
#line 972 "src/ptrflow.tr"
                List_i64* fr = pf_run_fn(hf, summaries, externs, false, sink);
#line 973 "src/ptrflow.tr"
                if ((fr->len > 0LL)) {
#line 973 "src/ptrflow.tr"
                    _tr_dict_set(summaries, _tr_strz(hf->name), fr);
                }
            }
#line 974 "src/ptrflow.tr"
            fi = (fi + 1LL);
        }
#line 975 "src/ptrflow.tr"
        rnd = (rnd + 1LL);
    }
#line 976 "src/ptrflow.tr"
    long long fi2 = 0LL;
#line 977 "src/ptrflow.tr"
    while ((fi2 < fns->len)) {
#line 978 "src/ptrflow.tr"
        List_TrStr* errs = (void*)List_TrStr_new();
#line 979 "src/ptrflow.tr"
        pf_run_fn(((HirFunction*)List_ptr_get(fns, fi2)), summaries, externs, true, errs);
#line 980 "src/ptrflow.tr"
        long long ei = 0LL;
#line 981 "src/ptrflow.tr"
        while ((ei < errs->len)) {
#line 982 "src/ptrflow.tr"
            ({ __auto_type _ord_t1002 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(fi2)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("@", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1003 = (({ TrStr _tr_cl = (_ord_t1002); TrStr _tr_cr = (List_TrStr_get(errs, ei)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(out, _at_t1003); _tr_str_release(_at_t1003); });
#line 983 "src/ptrflow.tr"
            ei = (ei + 1LL);
        }
#line 984 "src/ptrflow.tr"
        fi2 = (fi2 + 1LL);
    }
#line 985 "src/ptrflow.tr"
    return out;
}

