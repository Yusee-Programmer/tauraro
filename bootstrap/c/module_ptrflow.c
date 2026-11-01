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
#line 34 "src/ptrflow.tr"
    PfState* s = ((PfState*)_tr_obj_alloc(sizeof(PfState)));
#line 35 "src/ptrflow.tr"
    s->freed = (void*)List_TrStr_new();
#line 36 "src/ptrflow.tr"
    s->pa = (void*)List_TrStr_new();
#line 37 "src/ptrflow.tr"
    s->pb = (void*)List_TrStr_new();
#line 38 "src/ptrflow.tr"
    s->stk = (void*)List_TrStr_new();
#line 39 "src/ptrflow.tr"
    s->own = (void*)List_TrStr_new();
#line 40 "src/ptrflow.tr"
    s->nul = (void*)List_TrStr_new();
#line 41 "src/ptrflow.tr"
    s->sz = (void*)List_TrStr_new();
#line 42 "src/ptrflow.tr"
    s->dead = false;
#line 43 "src/ptrflow.tr"
    return s;
}

__attribute__((hot)) PfState* PfState_copy(PfState* self) {
#line 46 "src/ptrflow.tr"
    PfState* s = PfState_init();
#line 47 "src/ptrflow.tr"
    long long i = 0LL;
#line 48 "src/ptrflow.tr"
    while ((i < self->freed->len)) {
#line 49 "src/ptrflow.tr"
        ({ TrStr _ord_t824 = (List_TrStr_get(self->freed, i)); List_TrStr_append(s->freed, _ord_t824); _tr_str_release(_ord_t824); });
#line 50 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 51 "src/ptrflow.tr"
    i = 0LL;
#line 52 "src/ptrflow.tr"
    while ((i < self->pa->len)) {
#line 53 "src/ptrflow.tr"
        ({ TrStr _ord_t825 = (List_TrStr_get(self->pa, i)); List_TrStr_append(s->pa, _ord_t825); _tr_str_release(_ord_t825); });
#line 54 "src/ptrflow.tr"
        ({ TrStr _ord_t826 = (List_TrStr_get(self->pb, i)); List_TrStr_append(s->pb, _ord_t826); _tr_str_release(_ord_t826); });
#line 55 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 56 "src/ptrflow.tr"
    i = 0LL;
#line 57 "src/ptrflow.tr"
    while ((i < self->stk->len)) {
#line 58 "src/ptrflow.tr"
        ({ TrStr _ord_t827 = (List_TrStr_get(self->stk, i)); List_TrStr_append(s->stk, _ord_t827); _tr_str_release(_ord_t827); });
#line 59 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 60 "src/ptrflow.tr"
    s->own = pf_vcopy(self->own);
#line 61 "src/ptrflow.tr"
    s->nul = pf_vcopy(self->nul);
#line 62 "src/ptrflow.tr"
    s->sz = pf_vcopy(self->sz);
#line 63 "src/ptrflow.tr"
    s->dead = self->dead;
#line 64 "src/ptrflow.tr"
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) PfCtx* PfCtx_init(TrMap* summaries) {
#line 165 "src/ptrflow.tr"
    PfCtx* c = ((PfCtx*)_tr_obj_alloc(sizeof(PfCtx)));
#line 166 "src/ptrflow.tr"
    c->errs = (void*)List_TrStr_new();
#line 167 "src/ptrflow.tr"
    c->line = 0LL;
#line 168 "src/ptrflow.tr"
    c->fn_name = _tr_str_lit_len("", 0LL);
#line 169 "src/ptrflow.tr"
    c->ret_from = false;
#line 170 "src/ptrflow.tr"
    c->is_main = false;
#line 171 "src/ptrflow.tr"
    c->params = (void*)List_TrStr_new();
#line 172 "src/ptrflow.tr"
    c->locals = (void*)List_TrStr_new();
#line 173 "src/ptrflow.tr"
    c->summaries = summaries;
#line 174 "src/ptrflow.tr"
    PfState* _cltmp_t828 = PfState_init();
    _tr_obj_release(c->ret_acc, _trdrop_PfState);
    c->ret_acc = _cltmp_t828;
#line 175 "src/ptrflow.tr"
    c->ret_acc->dead = true;
#line 176 "src/ptrflow.tr"
    c->ret_seen = false;
#line 177 "src/ptrflow.tr"
    c->brk = (void*)List_ptr_new();
#line 178 "src/ptrflow.tr"
    c->brk_base = (void*)List_i64_new();
#line 179 "src/ptrflow.tr"
    c->cnt = (void*)List_ptr_new();
#line 180 "src/ptrflow.tr"
    c->cnt_base = (void*)List_i64_new();
#line 181 "src/ptrflow.tr"
    c->report = true;
#line 182 "src/ptrflow.tr"
    c->alloc_line = _tr_dict_new(4LL);
#line 183 "src/ptrflow.tr"
    c->noesc = false;
#line 184 "src/ptrflow.tr"
    c->in_defer = false;
#line 185 "src/ptrflow.tr"
    c->unsafe_depth = 0LL;
#line 186 "src/ptrflow.tr"
    c->externs = _tr_dict_new(2LL);
#line 187 "src/ptrflow.tr"
    return c;
}

__attribute__((hot)) void PfCtx_err(PfCtx* self, TrStr msg) {
#line 190 "src/ptrflow.tr"
    if ((!self->report)) {
#line 190 "src/ptrflow.tr"
        return;
    }
#line 191 "src/ptrflow.tr"
    TrStr e = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(self->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (msg)); _tr_str_release(_tr_cl); _tr_cres; });
#line 192 "src/ptrflow.tr"
    if ((!pf_has(self->errs, e))) {
#line 192 "src/ptrflow.tr"
        List_TrStr_append(self->errs, e);
    }
    _tr_str_release(e);
}

__attribute__((hot)) bool pf_has(List_TrStr* v, TrStr s) {
#line 67 "src/ptrflow.tr"
    long long i = 0LL;
#line 68 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 69 "src/ptrflow.tr"
        if (({ TrStr _cmpa_t829 = (List_TrStr_get(v, i)); TrStr _cmpb_t830 = (s); _Bool _cmpr_t831 = _tr_str_eqv(_cmpa_t829, _cmpb_t830); _tr_str_release(_cmpa_t829); _cmpr_t831; })) {
#line 69 "src/ptrflow.tr"
            return true;
        }
#line 70 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 71 "src/ptrflow.tr"
    return false;
}

__attribute__((hot)) void pf_add(List_TrStr* v, TrStr s) {
#line 74 "src/ptrflow.tr"
    if ((!pf_has(v, s))) {
#line 74 "src/ptrflow.tr"
        List_TrStr_append(v, s);
    }
}

__attribute__((hot)) bool pf_under(TrStr p, TrStr n) {
#line 78 "src/ptrflow.tr"
    if (_tr_str_eqv((p), (n))) {
#line 78 "src/ptrflow.tr"
        return true;
    }
#line 79 "src/ptrflow.tr"
    return _tr_str_starts_withv((p), (_tr_strx_concatv((n), (_tr_str_lit_len(".", 1LL)))));
}

__attribute__((hot)) List_TrStr* pf_vcopy(List_TrStr* v) {
#line 82 "src/ptrflow.tr"
    List_TrStr* o = (void*)List_TrStr_new();
#line 83 "src/ptrflow.tr"
    long long i = 0LL;
#line 84 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 85 "src/ptrflow.tr"
        ({ TrStr _at_t832 = (List_TrStr_get(v, i)); List_TrStr_append(o, _at_t832); _tr_str_release(_at_t832); });
#line 86 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 87 "src/ptrflow.tr"
    return o;
}

__attribute__((hot)) List_TrStr* pf_inter(List_TrStr* a, List_TrStr* b) {
#line 90 "src/ptrflow.tr"
    List_TrStr* o = (void*)List_TrStr_new();
#line 91 "src/ptrflow.tr"
    long long i = 0LL;
#line 92 "src/ptrflow.tr"
    while ((i < a->len)) {
#line 93 "src/ptrflow.tr"
        if (({ TrStr _at_t833 = (List_TrStr_get(a, i)); __auto_type _wr = (pf_has(b, _at_t833)); _tr_str_release(_at_t833); _wr; })) {
#line 93 "src/ptrflow.tr"
            ({ TrStr _at_t834 = (List_TrStr_get(a, i)); List_TrStr_append(o, _at_t834); _tr_str_release(_at_t834); });
        }
#line 94 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 95 "src/ptrflow.tr"
    return o;
}

__attribute__((hot)) void pf_assign_from(PfState* into, PfState* o) {
#line 98 "src/ptrflow.tr"
    into->freed = pf_vcopy(o->freed);
#line 99 "src/ptrflow.tr"
    into->pa = pf_vcopy(o->pa);
#line 100 "src/ptrflow.tr"
    into->pb = pf_vcopy(o->pb);
#line 101 "src/ptrflow.tr"
    into->stk = pf_vcopy(o->stk);
#line 102 "src/ptrflow.tr"
    into->own = pf_vcopy(o->own);
#line 103 "src/ptrflow.tr"
    into->nul = pf_vcopy(o->nul);
#line 104 "src/ptrflow.tr"
    into->sz = pf_vcopy(o->sz);
#line 105 "src/ptrflow.tr"
    into->dead = o->dead;
}

__attribute__((hot)) bool pf_has_pair(PfState* st, TrStr a, TrStr b) {
#line 108 "src/ptrflow.tr"
    long long i = 0LL;
#line 109 "src/ptrflow.tr"
    while ((i < st->pa->len)) {
#line 110 "src/ptrflow.tr"
        if ((({ TrStr _cmpa_t835 = (List_TrStr_get(st->pa, i)); TrStr _cmpb_t836 = (a); _Bool _cmpr_t837 = _tr_str_eqv(_cmpa_t835, _cmpb_t836); _tr_str_release(_cmpa_t835); _cmpr_t837; }) && ({ TrStr _cmpa_t838 = (List_TrStr_get(st->pb, i)); TrStr _cmpb_t839 = (b); _Bool _cmpr_t840 = _tr_str_eqv(_cmpa_t838, _cmpb_t839); _tr_str_release(_cmpa_t838); _cmpr_t840; }))) {
#line 110 "src/ptrflow.tr"
            return true;
        }
#line 111 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 112 "src/ptrflow.tr"
    return false;
}

__attribute__((hot)) void pf_merge(PfState* into, PfState* o) {
#line 116 "src/ptrflow.tr"
    if (o->dead) {
#line 116 "src/ptrflow.tr"
        return;
    }
#line 117 "src/ptrflow.tr"
    if (into->dead) {
#line 118 "src/ptrflow.tr"
        pf_assign_from(into, o);
#line 119 "src/ptrflow.tr"
        return;
    }
#line 120 "src/ptrflow.tr"
    long long i = 0LL;
#line 121 "src/ptrflow.tr"
    while ((i < o->freed->len)) {
#line 122 "src/ptrflow.tr"
        ({ __auto_type _ord_t842 = (into->freed); TrStr _at_t841 = (List_TrStr_get(o->freed, i)); pf_add(_ord_t842, _at_t841); _tr_str_release(_at_t841); });
#line 123 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 124 "src/ptrflow.tr"
    i = 0LL;
#line 125 "src/ptrflow.tr"
    while ((i < o->pa->len)) {
#line 126 "src/ptrflow.tr"
        if (({ TrStr _at_t843 = (List_TrStr_get(o->pa, i)); TrStr _at_t844 = (List_TrStr_get(o->pb, i)); __auto_type _wr = ((!pf_has_pair(into, _at_t843, _at_t844))); _tr_str_release(_at_t843); _tr_str_release(_at_t844); _wr; })) {
#line 127 "src/ptrflow.tr"
            ({ TrStr _ord_t845 = (List_TrStr_get(o->pa, i)); List_TrStr_append(into->pa, _ord_t845); _tr_str_release(_ord_t845); });
#line 128 "src/ptrflow.tr"
            ({ TrStr _ord_t846 = (List_TrStr_get(o->pb, i)); List_TrStr_append(into->pb, _ord_t846); _tr_str_release(_ord_t846); });
        }
#line 129 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 130 "src/ptrflow.tr"
    i = 0LL;
#line 131 "src/ptrflow.tr"
    while ((i < o->stk->len)) {
#line 132 "src/ptrflow.tr"
        ({ __auto_type _ord_t848 = (into->stk); TrStr _at_t847 = (List_TrStr_get(o->stk, i)); pf_add(_ord_t848, _at_t847); _tr_str_release(_at_t847); });
#line 133 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 134 "src/ptrflow.tr"
    i = 0LL;
#line 135 "src/ptrflow.tr"
    while ((i < o->own->len)) {
#line 136 "src/ptrflow.tr"
        ({ __auto_type _ord_t850 = (into->own); TrStr _at_t849 = (List_TrStr_get(o->own, i)); pf_add(_ord_t850, _at_t849); _tr_str_release(_at_t849); });
#line 137 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 138 "src/ptrflow.tr"
    into->nul = pf_inter(into->nul, o->nul);
#line 139 "src/ptrflow.tr"
    into->sz = pf_inter(into->sz, o->sz);
}

__attribute__((hot)) bool pf_null(HirExpr* e) {
#line 197 "src/ptrflow.tr"
    return (((unsigned long long)(e)) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr pf_path(HirExpr* e) {
#line 201 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 201 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 202 "src/ptrflow.tr"
    __auto_type _t851 = (*e);
    if (_t851.tag == HirExpr_EIdent) {
        __auto_type nm = _t851.data.EIdent.name;
        TrStr _trret179 = _tr_str_retain(nm);
        return _trret179;
    } else if (_t851.tag == HirExpr_EPropAccess) {
        __auto_type o = _t851.data.EPropAccess.obj;
__auto_type p = _t851.data.EPropAccess.prop;
#line 205 "src/ptrflow.tr"
        TrStr po = pf_path(o);
#line 206 "src/ptrflow.tr"
        if ((!_tr_str_eqv((po), (_tr_str_lit_len("", 0LL))))) {
#line 206 "src/ptrflow.tr"
            TrStr _trret180 = ({ TrStr _tr_cl = (_tr_strx_concatv((po), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(po);
            return _trret180;
        }
#line 207 "src/ptrflow.tr"
        _tr_str_release(po);
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t851;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) TrStr pf_root(HirExpr* e) {
#line 212 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 212 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 213 "src/ptrflow.tr"
    __auto_type _t852 = (*e);
    if (_t852.tag == HirExpr_EIdent) {
        __auto_type nm = _t852.data.EIdent.name;
        TrStr _trret181 = _tr_str_retain(nm);
        return _trret181;
    } else if (_t852.tag == HirExpr_EPropAccess) {
        __auto_type o = _t852.data.EPropAccess.obj;
__auto_type p = _t852.data.EPropAccess.prop;
        TrStr _trret182 = pf_path(e);
        return _trret182;
    } else if (_t852.tag == HirExpr_ECast) {
        __auto_type x = _t852.data.ECast.expr;
        return pf_root(x);
    } else if (_t852.tag == HirExpr_EMethodCall) {
        __auto_type o = _t852.data.EMethodCall.obj;
__auto_type m = _t852.data.EMethodCall.method;
#line 218 "src/ptrflow.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cast", 4LL))))) {
#line 218 "src/ptrflow.tr"
            TrStr _trret183 = pf_root(o);
            return _trret183;
        }
#line 219 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t852;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool pf_is_dealloc(HirExpr* callee) {
#line 223 "src/ptrflow.tr"
    if (pf_null(callee)) {
#line 223 "src/ptrflow.tr"
        return false;
    }
#line 224 "src/ptrflow.tr"
    __auto_type _t853 = (*callee);
    if (_t853.tag == HirExpr_EIdent) {
        __auto_type nm = _t853.data.EIdent.name;
        bool _trret184 = _tr_str_eqv((nm), (_tr_str_lit_len("dealloc", 7LL)));
        return _trret184;
    } else if (_t853.tag == HirExpr_EIndex) {
        __auto_type o = _t853.data.EIndex.obj;
#line 227 "src/ptrflow.tr"
        __auto_type _t854 = (*o);
        if (_t854.tag == HirExpr_EIdent) {
            __auto_type nm = _t854.data.EIdent.name;
            bool _trret185 = _tr_str_eqv((nm), (_tr_str_lit_len("dealloc", 7LL)));
            return _trret185;
        } else if (1) {
            __auto_type _ = _t854;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t853;
        return false;
    }
}

__attribute__((hot)) TrStr pf_callee_name(HirExpr* callee) {
#line 233 "src/ptrflow.tr"
    if (pf_null(callee)) {
#line 233 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 234 "src/ptrflow.tr"
    __auto_type _t855 = (*callee);
    if (_t855.tag == HirExpr_EIdent) {
        __auto_type nm = _t855.data.EIdent.name;
        TrStr _trret186 = _tr_str_retain(nm);
        return _trret186;
    } else if (1) {
        __auto_type _ = _t855;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) TrStr pf_stack_src(HirExpr* e, PfState* st, PfCtx* cx) {
#line 240 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 240 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 241 "src/ptrflow.tr"
    __auto_type _t856 = (*e);
    if (_t856.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t856.data.EUnaryOp.op;
__auto_type x = _t856.data.EUnaryOp.expr;
#line 243 "src/ptrflow.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL))) && (!pf_null(x)))) {
#line 244 "src/ptrflow.tr"
            __auto_type _t857 = (*x);
            if (_t857.tag == HirExpr_EIdent) {
                __auto_type nm = _t857.data.EIdent.name;
#line 246 "src/ptrflow.tr"
                if (((!_tr_str_eqv((nm), (_tr_str_lit_len("self", 4LL)))) && (pf_has(cx->locals, nm) || pf_has(cx->params, nm)))) {
#line 246 "src/ptrflow.tr"
                    TrStr _trret187 = _tr_str_retain(nm);
                    return _trret187;
                }
            } else if (1) {
                __auto_type _ = _t857;
                /* pass */
            }
        }
#line 248 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t856.tag == HirExpr_EIdent) {
        __auto_type nm = _t856.data.EIdent.name;
#line 250 "src/ptrflow.tr"
        if (pf_has(st->stk, nm)) {
#line 250 "src/ptrflow.tr"
            TrStr _trret188 = _tr_str_retain(nm);
            return _trret188;
        }
#line 251 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (_t856.tag == HirExpr_ECast) {
        __auto_type x = _t856.data.ECast.expr;
        return pf_stack_src(x, st, cx);
    } else if (_t856.tag == HirExpr_EMethodCall) {
        __auto_type o = _t856.data.EMethodCall.obj;
__auto_type m = _t856.data.EMethodCall.method;
#line 254 "src/ptrflow.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sub", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("cast", 4LL))))) {
#line 254 "src/ptrflow.tr"
            TrStr _trret189 = pf_stack_src(o, st, cx);
            return _trret189;
        }
#line 255 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t856;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) long long pf_lit(HirExpr* e) {
#line 260 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 260 "src/ptrflow.tr"
        return (-4611686018427387904LL);
    }
#line 261 "src/ptrflow.tr"
    __auto_type _t858 = (*e);
    if (_t858.tag == HirExpr_ELitInt) {
        __auto_type v = _t858.data.ELitInt.val;
        return v;
    } else if (_t858.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t858.data.EUnaryOp.op;
__auto_type x = _t858.data.EUnaryOp.expr;
#line 264 "src/ptrflow.tr"
        if ((_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL))) && (!pf_null(x)))) {
#line 265 "src/ptrflow.tr"
            __auto_type _t859 = (*x);
            if (_t859.tag == HirExpr_ELitInt) {
                __auto_type v2 = _t859.data.ELitInt.val;
                long long _trret190 = (0LL - v2);
                return _trret190;
            } else if (1) {
                __auto_type _ = _t859;
                return (-4611686018427387904LL);
            }
        }
#line 268 "src/ptrflow.tr"
        return (-4611686018427387904LL);
    } else if (1) {
        __auto_type _ = _t858;
        return (-4611686018427387904LL);
    }
}

__attribute__((hot)) bool pf_is_lit(HirExpr* e) {
#line 272 "src/ptrflow.tr"
    return (pf_lit(e) != (-4611686018427387904LL));
}

__attribute__((hot)) bool pf_named_call(HirExpr* callee, TrStr want) {
#line 275 "src/ptrflow.tr"
    if (pf_null(callee)) {
#line 275 "src/ptrflow.tr"
        return false;
    }
#line 276 "src/ptrflow.tr"
    __auto_type _t860 = (*callee);
    if (_t860.tag == HirExpr_EIdent) {
        __auto_type nm = _t860.data.EIdent.name;
        bool _trret191 = _tr_str_eqv((nm), (want));
        return _trret191;
    } else if (_t860.tag == HirExpr_EIndex) {
        __auto_type o = _t860.data.EIndex.obj;
#line 279 "src/ptrflow.tr"
        __auto_type _t861 = (*o);
        if (_t861.tag == HirExpr_EIdent) {
            __auto_type nm = _t861.data.EIdent.name;
            bool _trret192 = _tr_str_eqv((nm), (want));
            return _trret192;
        } else if (1) {
            __auto_type _ = _t861;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t860;
        return false;
    }
}

__attribute__((hot)) bool pf_is_alloc(HirExpr* val) {
#line 285 "src/ptrflow.tr"
    if (pf_null(val)) {
#line 285 "src/ptrflow.tr"
        return false;
    }
#line 286 "src/ptrflow.tr"
    __auto_type _t862 = (*val);
    if (_t862.tag == HirExpr_ECall) {
        __auto_type callee = _t862.data.ECall.callee;
        return pf_named_call(callee, _tr_str_lit_len("alloc", 5LL));
    } else if (1) {
        __auto_type _ = _t862;
        return false;
    }
}

__attribute__((hot)) long long pf_alloc_n(HirExpr* val) {
#line 291 "src/ptrflow.tr"
    __auto_type _t863 = (*val);
    if (_t863.tag == HirExpr_ECall) {
        __auto_type args = _t863.data.ECall.args;
#line 293 "src/ptrflow.tr"
        if ((args->len == 1LL)) {
#line 293 "src/ptrflow.tr"
            return pf_lit(((HirExpr*)List_ptr_get(args, 0LL)));
        }
#line 294 "src/ptrflow.tr"
        return (-4611686018427387904LL);
    } else if (1) {
        __auto_type _ = _t863;
        return (-4611686018427387904LL);
    }
}

__attribute__((hot)) bool pf_is_null_lit(HirExpr* val) {
#line 299 "src/ptrflow.tr"
    if (pf_null(val)) {
#line 299 "src/ptrflow.tr"
        return false;
    }
#line 300 "src/ptrflow.tr"
    __auto_type _t864 = (*val);
    if (_t864.tag == HirExpr_ELitNone) {
        __auto_type _ = _t864.data.ELitNone.ty;
        return true;
    } else if (_t864.tag == HirExpr_ELitInt) {
        __auto_type v = _t864.data.ELitInt.val;
__auto_type ty = _t864.data.ELitInt.ty;
        bool _trret193 = ((v == 0LL) && _tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))));
        return _trret193;
    } else if (_t864.tag == HirExpr_ECast) {
        __auto_type x = _t864.data.ECast.expr;
__auto_type ty = _t864.data.ECast.target_ty;
#line 304 "src/ptrflow.tr"
        if ((pf_null(x) || (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL)))))) {
#line 304 "src/ptrflow.tr"
            return false;
        }
#line 305 "src/ptrflow.tr"
        __auto_type _t865 = (*x);
        if (_t865.tag == HirExpr_ELitInt) {
            __auto_type v = _t865.data.ELitInt.val;
            bool _trret194 = (v == 0LL);
            return _trret194;
        } else if (1) {
            __auto_type _ = _t865;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t864;
        return false;
    }
}

__attribute__((hot)) long long pf_size_of(PfState* st, TrStr path) {
#line 311 "src/ptrflow.tr"
    long long i = 0LL;
#line 312 "src/ptrflow.tr"
    while ((i < st->sz->len)) {
#line 313 "src/ptrflow.tr"
        TrStr e = List_TrStr_get(st->sz, i);
#line 314 "src/ptrflow.tr"
        long long eq = _tr_str_index_ofv((e), (_tr_str_lit_len("=", 1LL)));
#line 315 "src/ptrflow.tr"
        if (({ TrStr _cmpa_t866 = (_tr_str_slicev((e), 0LL, eq)); TrStr _cmpb_t867 = (path); _Bool _cmpr_t868 = _tr_str_eqv(_cmpa_t866, _cmpb_t867); _tr_str_release(_cmpa_t866); _cmpr_t868; })) {
#line 315 "src/ptrflow.tr"
            long long _trret195 = _tr_str_to_int(_tr_strz(_tr_str_slicev((e), (eq + 1LL), _tr_str_lenv((e)))));
            _tr_str_release(e);
            return _trret195;
        }
#line 316 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(e);
    }
#line 317 "src/ptrflow.tr"
    return (-1LL);
}

__attribute__((hot)) void pf_kill(PfState* st, TrStr path) {
#line 322 "src/ptrflow.tr"
    List_TrStr* nf = (void*)List_TrStr_new();
#line 323 "src/ptrflow.tr"
    long long i = 0LL;
#line 324 "src/ptrflow.tr"
    while ((i < st->freed->len)) {
#line 325 "src/ptrflow.tr"
        if (({ TrStr _at_t869 = (List_TrStr_get(st->freed, i)); __auto_type _wr = ((!pf_under(_at_t869, path))); _tr_str_release(_at_t869); _wr; })) {
#line 325 "src/ptrflow.tr"
            ({ TrStr _at_t870 = (List_TrStr_get(st->freed, i)); List_TrStr_append(nf, _at_t870); _tr_str_release(_at_t870); });
        }
#line 326 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 327 "src/ptrflow.tr"
    st->freed = nf;
#line 328 "src/ptrflow.tr"
    List_TrStr* na = (void*)List_TrStr_new();
#line 329 "src/ptrflow.tr"
    List_TrStr* nb = (void*)List_TrStr_new();
#line 330 "src/ptrflow.tr"
    i = 0LL;
#line 331 "src/ptrflow.tr"
    while ((i < st->pa->len)) {
#line 332 "src/ptrflow.tr"
        if ((({ TrStr _at_t871 = (List_TrStr_get(st->pa, i)); __auto_type _wr = ((!pf_under(_at_t871, path))); _tr_str_release(_at_t871); _wr; }) && ({ TrStr _at_t872 = (List_TrStr_get(st->pb, i)); __auto_type _wr = ((!pf_under(_at_t872, path))); _tr_str_release(_at_t872); _wr; }))) {
#line 333 "src/ptrflow.tr"
            ({ TrStr _at_t873 = (List_TrStr_get(st->pa, i)); List_TrStr_append(na, _at_t873); _tr_str_release(_at_t873); });
#line 334 "src/ptrflow.tr"
            ({ TrStr _at_t874 = (List_TrStr_get(st->pb, i)); List_TrStr_append(nb, _at_t874); _tr_str_release(_at_t874); });
        }
#line 335 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 336 "src/ptrflow.tr"
    st->pa = na;
#line 337 "src/ptrflow.tr"
    st->pb = nb;
#line 338 "src/ptrflow.tr"
    List_TrStr* ns = (void*)List_TrStr_new();
#line 339 "src/ptrflow.tr"
    i = 0LL;
#line 340 "src/ptrflow.tr"
    while ((i < st->stk->len)) {
#line 341 "src/ptrflow.tr"
        if (({ TrStr _at_t875 = (List_TrStr_get(st->stk, i)); __auto_type _wr = ((!pf_under(_at_t875, path))); _tr_str_release(_at_t875); _wr; })) {
#line 341 "src/ptrflow.tr"
            ({ TrStr _at_t876 = (List_TrStr_get(st->stk, i)); List_TrStr_append(ns, _at_t876); _tr_str_release(_at_t876); });
        }
#line 342 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 343 "src/ptrflow.tr"
    st->stk = ns;
#line 344 "src/ptrflow.tr"
    st->own = pf_drop_under(st->own, path);
#line 345 "src/ptrflow.tr"
    st->nul = pf_drop_under(st->nul, path);
#line 346 "src/ptrflow.tr"
    List_TrStr* nz = (void*)List_TrStr_new();
#line 347 "src/ptrflow.tr"
    i = 0LL;
#line 348 "src/ptrflow.tr"
    while ((i < st->sz->len)) {
#line 349 "src/ptrflow.tr"
        TrStr e = List_TrStr_get(st->sz, i);
#line 350 "src/ptrflow.tr"
        long long eq = _tr_str_index_ofv((e), (_tr_str_lit_len("=", 1LL)));
#line 351 "src/ptrflow.tr"
        if (({ TrStr _at_t877 = (_tr_str_slicev((e), 0LL, eq)); __auto_type _wr = ((!pf_under(_at_t877, path))); _tr_str_release(_at_t877); _wr; })) {
#line 351 "src/ptrflow.tr"
            List_TrStr_append(nz, e);
        }
#line 352 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(e);
    }
#line 353 "src/ptrflow.tr"
    st->sz = nz;
}

__attribute__((hot)) List_TrStr* pf_drop_under(List_TrStr* v, TrStr path) {
#line 356 "src/ptrflow.tr"
    List_TrStr* o = (void*)List_TrStr_new();
#line 357 "src/ptrflow.tr"
    long long i = 0LL;
#line 358 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 359 "src/ptrflow.tr"
        if (({ TrStr _at_t878 = (List_TrStr_get(v, i)); __auto_type _wr = ((!pf_under(_at_t878, path))); _tr_str_release(_at_t878); _wr; })) {
#line 359 "src/ptrflow.tr"
            ({ TrStr _at_t879 = (List_TrStr_get(v, i)); List_TrStr_append(o, _at_t879); _tr_str_release(_at_t879); });
        }
#line 360 "src/ptrflow.tr"
        i = (i + 1LL);
    }
#line 361 "src/ptrflow.tr"
    return o;
}

__attribute__((hot)) void pf_kill_fields(PfState* st, TrStr path) {
#line 365 "src/ptrflow.tr"
    List_TrStr* nf = (void*)List_TrStr_new();
#line 366 "src/ptrflow.tr"
    long long i = 0LL;
#line 367 "src/ptrflow.tr"
    while ((i < st->freed->len)) {
#line 368 "src/ptrflow.tr"
        TrStr f = List_TrStr_get(st->freed, i);
#line 369 "src/ptrflow.tr"
        if ((!((!_tr_str_eqv((f), (path))) && pf_under(f, path)))) {
#line 369 "src/ptrflow.tr"
            List_TrStr_append(nf, f);
        }
#line 370 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(f);
    }
#line 371 "src/ptrflow.tr"
    st->freed = nf;
}

__attribute__((hot)) void pf_link(PfState* st, TrStr a, TrStr b) {
#line 374 "src/ptrflow.tr"
    if (_tr_str_eqv((a), (b))) {
#line 374 "src/ptrflow.tr"
        return;
    }
#line 375 "src/ptrflow.tr"
    if ((!pf_has_pair(st, a, b))) {
#line 376 "src/ptrflow.tr"
        List_TrStr_append(st->pa, a);
#line 377 "src/ptrflow.tr"
        List_TrStr_append(st->pb, b);
    }
#line 378 "src/ptrflow.tr"
    if ((!pf_has_pair(st, b, a))) {
#line 379 "src/ptrflow.tr"
        List_TrStr_append(st->pa, b);
#line 380 "src/ptrflow.tr"
        List_TrStr_append(st->pb, a);
    }
}

__attribute__((hot)) void pf_free(PfState* st, TrStr path) {
#line 384 "src/ptrflow.tr"
    pf_add(st->freed, path);
#line 385 "src/ptrflow.tr"
    bool changed = true;
#line 386 "src/ptrflow.tr"
    long long guard = 0LL;
#line 387 "src/ptrflow.tr"
    while ((changed && (guard < 8LL))) {
#line 388 "src/ptrflow.tr"
        changed = false;
#line 389 "src/ptrflow.tr"
        guard = (guard + 1LL);
#line 390 "src/ptrflow.tr"
        long long i = 0LL;
#line 391 "src/ptrflow.tr"
        while ((i < st->pa->len)) {
#line 392 "src/ptrflow.tr"
            if ((({ __auto_type _ord_t881 = (st->freed); TrStr _at_t880 = (List_TrStr_get(st->pa, i)); __auto_type _wr = (pf_has(_ord_t881, _at_t880)); _tr_str_release(_at_t880); _wr; }) && ({ __auto_type _ord_t883 = (st->freed); TrStr _at_t882 = (List_TrStr_get(st->pb, i)); __auto_type _wr = ((!pf_has(_ord_t883, _at_t882))); _tr_str_release(_at_t882); _wr; }))) {
#line 393 "src/ptrflow.tr"
                ({ TrStr _ord_t884 = (List_TrStr_get(st->pb, i)); List_TrStr_append(st->freed, _ord_t884); _tr_str_release(_ord_t884); });
#line 394 "src/ptrflow.tr"
                changed = true;
            }
#line 395 "src/ptrflow.tr"
            i = (i + 1LL);
        }
    }
}

__attribute__((hot)) TrStr pf_is_freed(PfState* st, TrStr path) {
#line 399 "src/ptrflow.tr"
    long long i = 0LL;
#line 400 "src/ptrflow.tr"
    while ((i < st->freed->len)) {
#line 401 "src/ptrflow.tr"
        TrStr f = List_TrStr_get(st->freed, i);
#line 402 "src/ptrflow.tr"
        if (_tr_str_eqv((f), (path))) {
#line 402 "src/ptrflow.tr"
            TrStr _trret196 = _tr_str_retain(f);
            _tr_str_release(f);
            return _trret196;
        }
#line 403 "src/ptrflow.tr"
        i = (i + 1LL);
        _tr_str_release(f);
    }
#line 404 "src/ptrflow.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool pf_zeroish(HirExpr* e) {
#line 409 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 409 "src/ptrflow.tr"
        return false;
    }
#line 410 "src/ptrflow.tr"
    if (pf_is_null_lit(e)) {
#line 410 "src/ptrflow.tr"
        return true;
    }
#line 411 "src/ptrflow.tr"
    __auto_type _t885 = (*e);
    if (_t885.tag == HirExpr_ELitInt) {
        __auto_type v = _t885.data.ELitInt.val;
        return (v == 0LL);
    } else if (_t885.tag == HirExpr_ECast) {
        __auto_type x = _t885.data.ECast.expr;
        return pf_zeroish(x);
    } else if (1) {
        __auto_type _ = _t885;
        return false;
    }
}

__attribute__((hot)) TrStr pf_nulltest(HirExpr* cond) {
#line 418 "src/ptrflow.tr"
    if (pf_null(cond)) {
#line 418 "src/ptrflow.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 419 "src/ptrflow.tr"
    __auto_type _t886 = (*cond);
    if (_t886.tag == HirExpr_EBinOp) {
        __auto_type op = _t886.data.EBinOp.op;
__auto_type l = _t886.data.EBinOp.left;
__auto_type r = _t886.data.EBinOp.right;
#line 421 "src/ptrflow.tr"
        if (((!_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) && (!_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))))) {
#line 421 "src/ptrflow.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 422 "src/ptrflow.tr"
        TrStr pth = _tr_str_lit_len("", 0LL);
#line 423 "src/ptrflow.tr"
        if (pf_zeroish(r)) {
#line 423 "src/ptrflow.tr"
            TrStr _strtmp_t887 = pf_root(l);
            _tr_str_release(pth);
            pth = _strtmp_t887;
        } else if (pf_zeroish(l)) {
#line 424 "src/ptrflow.tr"
            TrStr _strtmp_t888 = pf_root(r);
            _tr_str_release(pth);
            pth = _strtmp_t888;
        }
#line 425 "src/ptrflow.tr"
        if (_tr_str_eqv((pth), (_tr_str_lit_len("", 0LL)))) {
#line 425 "src/ptrflow.tr"
            _tr_str_release(pth);
            return _tr_str_lit_len("", 0LL);
        }
#line 426 "src/ptrflow.tr"
        TrStr _trret197 = ({ TrStr _tr_cl = (_tr_strx_concatv((op), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pth)); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(pth);
        return _trret197;
    } else if (1) {
        __auto_type _ = _t886;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) void pf_refine(PfState* st, TrStr test, bool taken) {
#line 431 "src/ptrflow.tr"
    if (_tr_str_eqv((test), (_tr_str_lit_len("", 0LL)))) {
#line 431 "src/ptrflow.tr"
        return;
    }
#line 432 "src/ptrflow.tr"
    TrStr op = _tr_str_slicev((test), 0LL, 2LL);
#line 433 "src/ptrflow.tr"
    TrStr pth = _tr_str_slicev((test), 3LL, _tr_str_lenv((test)));
#line 434 "src/ptrflow.tr"
    bool is_null_here = (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL))) == taken);
#line 435 "src/ptrflow.tr"
    if ((!is_null_here)) {
#line 435 "src/ptrflow.tr"
        st->nul = pf_drop_under(st->nul, pth);
    }
    _tr_str_release(op);
    _tr_str_release(pth);
}

__attribute__((hot)) void pf_block(PfCtx* cx, HirBlock* b, PfState* st) {
#line 438 "src/ptrflow.tr"
    long long i = 0LL;
#line 439 "src/ptrflow.tr"
    while ((i < b->stmts->len)) {
#line 440 "src/ptrflow.tr"
        pf_stmt(cx, ((HirStmt*)List_ptr_get(b->stmts, i)), st);
#line 441 "src/ptrflow.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void pf_use_msg(PfCtx* cx, TrStr name) {
#line 444 "src/ptrflow.tr"
    ({ TrStr _at_t889 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was freed by 'dealloc()' and can no longer be used.\n      FIX: Remove all uses of '", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after 'dealloc()', or restructure so the pointer is freed only when no longer needed (or re-assign it a fresh value first). A pointer that is deallocated on only some paths, or in a previous loop iteration, counts as freed.", 225LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t889); _tr_str_release(_at_t889); });
}

__attribute__((hot)) void pf_exprs(PfCtx* cx, List_ptr* v, PfState* st) {
#line 447 "src/ptrflow.tr"
    long long i = 0LL;
#line 448 "src/ptrflow.tr"
    while ((i < v->len)) {
#line 449 "src/ptrflow.tr"
        pf_expr(cx, ((HirExpr*)List_ptr_get(v, i)), st);
#line 450 "src/ptrflow.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void pf_branchy_block(PfCtx* cx, HirBlock* b, PfState* st) {
#line 454 "src/ptrflow.tr"
    PfState* c = PfState_copy(st);
#line 455 "src/ptrflow.tr"
    pf_block(cx, b, c);
#line 456 "src/ptrflow.tr"
    pf_merge(st, c);
    _tr_obj_release(c, _trdrop_PfState);
}

__attribute__((hot)) void pf_ptr_checks(PfCtx* cx, PfState* st, HirExpr* o, TrStr m, List_ptr* args) {
#line 459 "src/ptrflow.tr"
    TrStr np = pf_path(o);
#line 460 "src/ptrflow.tr"
    if (((!_tr_str_eqv((np), (_tr_str_lit_len("", 0LL)))) && (_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))))) {
#line 461 "src/ptrflow.tr"
        if (pf_has(st->nul, np)) {
#line 462 "src/ptrflow.tr"
            ({ TrStr _at_t890 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '.", 8LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' on '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (np)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is a null pointer here (null dereference).\n      FIX: allocate it (alloc[T](n)) or check it for null before dereferencing.", 131LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t890); _tr_str_release(_at_t890); });
        } else if ((pf_size_of(st, np) == 0LL)) {
#line 464 "src/ptrflow.tr"
            ({ TrStr _at_t891 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '.", 8LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' on '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (np)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which was allocated with 0 elements (out of bounds).\n      FIX: allocate at least one element, or do not dereference an empty block.", 135LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t891); _tr_str_release(_at_t891); });
        }
    }
#line 465 "src/ptrflow.tr"
    if (((_tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))) && (!_tr_str_eqv((np), (_tr_str_lit_len("", 0LL))))) && (args->len == 1LL))) {
#line 466 "src/ptrflow.tr"
        long long sz = pf_size_of(st, np);
#line 467 "src/ptrflow.tr"
        long long k = pf_lit(((HirExpr*)List_ptr_get(args, 0LL)));
#line 468 "src/ptrflow.tr"
        if ((((sz >= 0LL) && pf_is_lit(((HirExpr*)List_ptr_get(args, 0LL)))) && ((k < 0LL) || (k > sz)))) {
#line 469 "src/ptrflow.tr"
            ({ __auto_type _ord_t892 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '", 7LL)), (np))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".offset(", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(k)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")' moves outside the ", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t893 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t892); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(sz)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("-element block '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (np)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was allocated with (undefined behaviour even before it is dereferenced).\n      FIX: keep the offset in 0 .. n (one-past-the-end) of the alloc[T](n) block.", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t893); _tr_str_release(_at_t893); });
        }
    }
#line 470 "src/ptrflow.tr"
    if (((_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) && (!pf_null(o)))) {
#line 471 "src/ptrflow.tr"
        __auto_type _t894 = (*o);
        if (_t894.tag == HirExpr_EMethodCall) {
            __auto_type bo = _t894.data.EMethodCall.obj;
__auto_type bm = _t894.data.EMethodCall.method;
__auto_type bargs = _t894.data.EMethodCall.args;
#line 473 "src/ptrflow.tr"
            if ((_tr_str_eqv((bm), (_tr_str_lit_len("offset", 6LL))) && (bargs->len == 1LL))) {
#line 474 "src/ptrflow.tr"
                TrStr bp = pf_path(bo);
#line 475 "src/ptrflow.tr"
                long long bsz = (-1LL);
#line 476 "src/ptrflow.tr"
                if ((!_tr_str_eqv((bp), (_tr_str_lit_len("", 0LL))))) {
#line 476 "src/ptrflow.tr"
                    bsz = pf_size_of(st, bp);
                }
#line 477 "src/ptrflow.tr"
                long long bk = pf_lit(((HirExpr*)List_ptr_get(bargs, 0LL)));
#line 478 "src/ptrflow.tr"
                if ((((bsz >= 0LL) && pf_is_lit(((HirExpr*)List_ptr_get(bargs, 0LL)))) && ((bk < 0LL) || (bk >= bsz)))) {
#line 479 "src/ptrflow.tr"
                    ({ __auto_type _ord_t895 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] '", 7LL)), (bp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".offset(", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(bk)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()' is outside the ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t896 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t895); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(bsz)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("-element block '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was allocated with (out-of-bounds access).\n      FIX: keep the offset in 0 .. n-1 of the alloc[T](n) block, or use a List[T] (bounds-checked).", 144LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t896); _tr_str_release(_at_t896); });
                }
            }
        } else if (1) {
            __auto_type _ = _t894;
            /* pass */
        }
    }
    _tr_str_release(np);
}

__attribute__((hot)) void pf_expr(PfCtx* cx, HirExpr* e, PfState* st) {
#line 483 "src/ptrflow.tr"
    if (pf_null(e)) {
#line 483 "src/ptrflow.tr"
        return;
    }
#line 484 "src/ptrflow.tr"
    __auto_type _t897 = (*e);
    if (_t897.tag == HirExpr_EIdent) {
        __auto_type nm = _t897.data.EIdent.name;
#line 486 "src/ptrflow.tr"
        if (pf_has(st->freed, nm)) {
#line 487 "src/ptrflow.tr"
            pf_use_msg(cx, nm);
#line 488 "src/ptrflow.tr"
            pf_kill(st, nm);
        }
#line 489 "src/ptrflow.tr"
        if (((!cx->noesc) && pf_has(st->own, nm))) {
#line 489 "src/ptrflow.tr"
            st->own = pf_drop_under(st->own, nm);
        }
    } else if (_t897.tag == HirExpr_EPropAccess) {
        __auto_type o = _t897.data.EPropAccess.obj;
__auto_type p = _t897.data.EPropAccess.prop;
#line 491 "src/ptrflow.tr"
        TrStr pp = pf_path(e);
#line 492 "src/ptrflow.tr"
        if (((!_tr_str_eqv((pp), (_tr_str_lit_len("", 0LL)))) && pf_has(st->freed, pp))) {
#line 493 "src/ptrflow.tr"
            pf_use_msg(cx, pp);
#line 494 "src/ptrflow.tr"
            pf_kill(st, pp);
        }
#line 495 "src/ptrflow.tr"
        pf_expr(cx, o, st);
        _tr_str_release(pp);
    } else if (_t897.tag == HirExpr_EBinOp) {
        __auto_type bop = _t897.data.EBinOp.op;
__auto_type l = _t897.data.EBinOp.left;
__auto_type r = _t897.data.EBinOp.right;
#line 497 "src/ptrflow.tr"
        if (((((cx->unsafe_depth == 0LL) && (_tr_str_eqv((bop), (_tr_str_lit_len("+", 1LL))) || _tr_str_eqv((bop), (_tr_str_lit_len("-", 1LL))))) && (!pf_null(l))) && _tr_str_eqv((hir_expr_type(l)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 498 "src/ptrflow.tr"
            ({ TrStr _at_t898 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-2] pointer arithmetic with '", 31LL)), (bop))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' can create an out-of-bounds pointer and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a safe slice / List[T] instead of pointer math.", 164LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t898); _tr_str_release(_at_t898); });
        }
#line 499 "src/ptrflow.tr"
        pf_expr(cx, l, st);
#line 500 "src/ptrflow.tr"
        pf_expr(cx, r, st);
    } else if (_t897.tag == HirExpr_EUnaryOp) {
        __auto_type uop = _t897.data.EUnaryOp.op;
__auto_type x = _t897.data.EUnaryOp.expr;
#line 502 "src/ptrflow.tr"
        if (((((cx->unsafe_depth == 0LL) && (_tr_str_eqv((uop), (_tr_str_lit_len("*", 1LL))) || _tr_str_eqv((uop), (_tr_str_lit_len("deref", 5LL))))) && (!pf_null(x))) && _tr_str_eqv((hir_expr_type(x)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 503 "src/ptrflow.tr"
            PfCtx_err(cx, _tr_str_lit_len("[P-2] '*' dereferences a raw pointer (may read freed or invalid memory) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or hold the value in a safe Box[T] / List[T] / class reference instead of a raw pointer.", 232LL));
        }
#line 504 "src/ptrflow.tr"
        pf_expr(cx, x, st);
    } else if (_t897.tag == HirExpr_ECall) {
        __auto_type callee = _t897.data.ECall.callee;
__auto_type args = _t897.data.ECall.args;
#line 506 "src/ptrflow.tr"
        if (pf_is_dealloc(callee)) {
#line 507 "src/ptrflow.tr"
            if ((args->len > 0LL)) {
#line 508 "src/ptrflow.tr"
                TrStr dp = pf_root(((HirExpr*)List_ptr_get(args, 0LL)));
#line 509 "src/ptrflow.tr"
                if (((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL)))) && pf_has(st->freed, dp))) {
#line 510 "src/ptrflow.tr"
                    ({ TrStr _at_t899 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-6] '", 7LL)), (dp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is deallocated twice (double free): it was already freed by 'dealloc()' on this or another path (an alias, a call, or the previous loop iteration).\n      FIX: dealloc each allocation exactly once; clear or re-assign the pointer after dealloc, and break out of the loop right after freeing.", 291LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t899); _tr_str_release(_at_t899); });
#line 511 "src/ptrflow.tr"
                    pf_kill(st, dp);
                }
#line 512 "src/ptrflow.tr"
                TrStr dss = pf_stack_src(((HirExpr*)List_ptr_get(args, 0LL)), st, cx);
#line 513 "src/ptrflow.tr"
                if ((!_tr_str_eqv((dss), (_tr_str_lit_len("", 0LL))))) {
#line 514 "src/ptrflow.tr"
                    ({ TrStr _at_t900 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] dealloc() of a pointer to the stack local '", 49LL)), (dss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': only memory returned by alloc[T] may be deallocated.\n      FIX: remove the dealloc, or allocate the block with alloc[T] instead of taking the address of a local.", 164LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t900); _tr_str_release(_at_t900); });
                } else if (((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL)))) && pf_has(st->nul, dp))) {
#line 516 "src/ptrflow.tr"
                    ({ TrStr _at_t901 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] dealloc() of '", 20LL)), (dp))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is null here — there is nothing to free.\n      FIX: remove the dealloc (or guard it with a null check on a pointer that may be non-null).", 148LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t901); _tr_str_release(_at_t901); });
                }
#line 517 "src/ptrflow.tr"
                pf_expr(cx, ((HirExpr*)List_ptr_get(args, 0LL)), st);
#line 518 "src/ptrflow.tr"
                if (((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL)))) && (!cx->in_defer))) {
#line 518 "src/ptrflow.tr"
                    pf_free(st, dp);
                }
#line 519 "src/ptrflow.tr"
                if ((!_tr_str_eqv((dp), (_tr_str_lit_len("", 0LL))))) {
#line 519 "src/ptrflow.tr"
                    st->own = pf_drop_under(st->own, dp);
                }
            }
#line 520 "src/ptrflow.tr"
            return;
        }
#line 521 "src/ptrflow.tr"
        TrStr cn = pf_callee_name(callee);
#line 522 "src/ptrflow.tr"
        if ((((!_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) && (cx->unsafe_depth == 0LL)) && _tr_dict_contains(cx->externs, _tr_strz(cn)))) {
#line 523 "src/ptrflow.tr"
            ({ TrStr _at_t902 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-5] extern function '", 23LL)), (cn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' takes a raw Pointer; C cannot check it, so the call must be inside an 'unsafe:' block.\n      FIX: wrap the call in 'unsafe:' (ideally inside a small safe wrapper function that validates its arguments).", 203LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t902); _tr_str_release(_at_t902); });
        }
#line 524 "src/ptrflow.tr"
        if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 525 "src/ptrflow.tr"
            pf_expr(cx, callee, st);
        }
#line 526 "src/ptrflow.tr"
        pf_exprs(cx, args, st);
#line 527 "src/ptrflow.tr"
        long long ki = 0LL;
#line 528 "src/ptrflow.tr"
        while ((ki < args->len)) {
#line 529 "src/ptrflow.tr"
            TrStr kp = pf_path(((HirExpr*)List_ptr_get(args, ki)));
#line 530 "src/ptrflow.tr"
            if ((!_tr_str_eqv((kp), (_tr_str_lit_len("", 0LL))))) {
#line 530 "src/ptrflow.tr"
                pf_kill_fields(st, kp);
            }
#line 531 "src/ptrflow.tr"
            ki = (ki + 1LL);
            _tr_str_release(kp);
        }
#line 532 "src/ptrflow.tr"
        if (((!_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) && _tr_dict_contains(cx->summaries, _tr_strz(cn)))) {
#line 533 "src/ptrflow.tr"
            List_i64* fr = ((List_i64*)(uintptr_t)_tr_dict_get(cx->summaries, _tr_strz(cn)));
#line 534 "src/ptrflow.tr"
            long long k = 0LL;
#line 535 "src/ptrflow.tr"
            while ((k < fr->len)) {
#line 536 "src/ptrflow.tr"
                if (({ __auto_type _ord_t903 = (List_i64_get(fr, k)); __auto_type _wr = ((_ord_t903 < args->len)); _wr; })) {
#line 537 "src/ptrflow.tr"
                    TrStr ap = pf_root(((HirExpr*)List_ptr_get(args, List_i64_get(fr, k))));
#line 538 "src/ptrflow.tr"
                    if ((!_tr_str_eqv((ap), (_tr_str_lit_len("", 0LL))))) {
#line 538 "src/ptrflow.tr"
                        pf_free(st, ap);
                    }
                }
#line 539 "src/ptrflow.tr"
                k = (k + 1LL);
            }
        }
        _tr_str_release(cn);
    } else if (_t897.tag == HirExpr_EMethodCall) {
        __auto_type o = _t897.data.EMethodCall.obj;
__auto_type m = _t897.data.EMethodCall.method;
__auto_type args = _t897.data.EMethodCall.args;
#line 541 "src/ptrflow.tr"
        if (((_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("offset", 6LL))))) {
#line 542 "src/ptrflow.tr"
            pf_ptr_checks(cx, st, o, m, args);
        }
#line 543 "src/ptrflow.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL))))) {
#line 544 "src/ptrflow.tr"
            cx->noesc = true;
#line 545 "src/ptrflow.tr"
            pf_expr(cx, o, st);
#line 546 "src/ptrflow.tr"
            cx->noesc = false;
        } else {
#line 548 "src/ptrflow.tr"
            pf_expr(cx, o, st);
        }
#line 549 "src/ptrflow.tr"
        pf_exprs(cx, args, st);
#line 550 "src/ptrflow.tr"
        TrStr mkp = pf_path(o);
#line 551 "src/ptrflow.tr"
        if ((!_tr_str_eqv((mkp), (_tr_str_lit_len("", 0LL))))) {
#line 551 "src/ptrflow.tr"
            pf_kill_fields(st, mkp);
        }
#line 552 "src/ptrflow.tr"
        if ((!cx->is_main)) {
#line 554 "src/ptrflow.tr"
            if ((((((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("send", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL))))) {
#line 555 "src/ptrflow.tr"
                long long ai = 0LL;
#line 556 "src/ptrflow.tr"
                while ((ai < args->len)) {
#line 557 "src/ptrflow.tr"
                    TrStr ss = pf_stack_src(((HirExpr*)List_ptr_get(args, ai)), st, cx);
#line 558 "src/ptrflow.tr"
                    if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
#line 559 "src/ptrflow.tr"
                        TrStr rp = pf_path(o);
#line 560 "src/ptrflow.tr"
                        TrStr r0 = _tr_str_retain(rp);
#line 561 "src/ptrflow.tr"
                        long long dot = _tr_str_index_ofv((r0), (_tr_str_lit_len(".", 1LL)));
#line 562 "src/ptrflow.tr"
                        if ((dot >= 0LL)) {
#line 562 "src/ptrflow.tr"
                            TrStr _strtmp_t904 = _tr_str_slicev((r0), 0LL, dot);
                            _tr_str_release(r0);
                            r0 = _strtmp_t904;
                        }
#line 563 "src/ptrflow.tr"
                        if (((_tr_str_eqv((r0), (_tr_str_lit_len("", 0LL))) || (!pf_has(cx->locals, r0))) || _tr_str_eqv((r0), (_tr_str_lit_len("self", 4LL))))) {
#line 564 "src/ptrflow.tr"
                            if (((!_tr_str_eqv((r0), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("write", 5LL))))) {
#line 565 "src/ptrflow.tr"
                                ({ TrStr _at_t905 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] the address of local '", 28LL)), (ss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is stored (via '.", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()') into memory that outlives this call of '", 45LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cx->fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it dangles once the function returns.\n      FIX: store an owned value, or heap-allocate the data with alloc[T] (and dealloc it when its owner is done) instead of pointing at a stack local.", 194LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t905); _tr_str_release(_at_t905); });
                            }
                        }
                    }
#line 566 "src/ptrflow.tr"
                    ai = (ai + 1LL);
                    _tr_str_release(ss);
                }
            }
        }
        _tr_str_release(mkp);
    } else if (_t897.tag == HirExpr_EIndex) {
        __auto_type o = _t897.data.EIndex.obj;
__auto_type i = _t897.data.EIndex._tr_v_index;
#line 568 "src/ptrflow.tr"
        if ((((cx->unsafe_depth == 0LL) && (!pf_null(o))) && _tr_str_eqv((hir_expr_type(o)->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 569 "src/ptrflow.tr"
            PfCtx_err(cx, _tr_str_lit_len("[P-2] indexing a raw Pointer reads or writes unchecked memory and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a List[T] (bounds-checked).", 168LL));
        }
#line 570 "src/ptrflow.tr"
        TrStr ip = pf_path(o);
#line 571 "src/ptrflow.tr"
        if ((!_tr_str_eqv((ip), (_tr_str_lit_len("", 0LL))))) {
#line 572 "src/ptrflow.tr"
            if (pf_has(st->nul, ip)) {
#line 573 "src/ptrflow.tr"
                ({ TrStr _at_t906 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-3] indexing '", 16LL)), (ip))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is a null pointer here.\n      FIX: allocate it (alloc[T](n)) or check it for null before use.", 102LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t906); _tr_str_release(_at_t906); });
            } else {
#line 575 "src/ptrflow.tr"
                long long isz = pf_size_of(st, ip);
#line 576 "src/ptrflow.tr"
                long long ik = pf_lit(i);
#line 577 "src/ptrflow.tr"
                if ((((isz >= 0LL) && pf_is_lit(i)) && ((ik < 0LL) || (ik >= isz)))) {
#line 578 "src/ptrflow.tr"
                    ({ __auto_type _ord_t907 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ik)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[P-3] index ", 12LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is outside the ", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t908 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t907); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(isz)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("-element block '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ip)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was allocated with.\n      FIX: keep indexes in 0 .. n-1 of the alloc[T](n) block, or use a List[T] (bounds-checked).", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t908); _tr_str_release(_at_t908); });
                }
            }
        }
#line 579 "src/ptrflow.tr"
        cx->noesc = true;
#line 580 "src/ptrflow.tr"
        pf_expr(cx, o, st);
#line 581 "src/ptrflow.tr"
        cx->noesc = false;
#line 582 "src/ptrflow.tr"
        pf_expr(cx, i, st);
        _tr_str_release(ip);
    } else if (_t897.tag == HirExpr_ECast) {
        __auto_type x = _t897.data.ECast.expr;
        pf_expr(cx, x, st);
    } else if (_t897.tag == HirExpr_EFString) {
        __auto_type parts = _t897.data.EFString.parts;
#line 585 "src/ptrflow.tr"
        long long pi = 0LL;
#line 586 "src/ptrflow.tr"
        while ((pi < parts->len)) {
#line 587 "src/ptrflow.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, pi));
#line 588 "src/ptrflow.tr"
            if (part->is_expr) {
#line 589 "src/ptrflow.tr"
                pf_expr(cx, part->expr, st);
#line 590 "src/ptrflow.tr"
                pf_exprs(cx, part->spec_args, st);
            }
#line 591 "src/ptrflow.tr"
            pi = (pi + 1LL);
        }
    } else if (_t897.tag == HirExpr_ETryExpr) {
        __auto_type x = _t897.data.ETryExpr.expr;
        pf_expr(cx, x, st);
    } else if (_t897.tag == HirExpr_EClosure) {
        __auto_type body = _t897.data.EClosure.body;
__auto_type caps = _t897.data.EClosure.captures;
#line 594 "src/ptrflow.tr"
        long long cpi = 0LL;
#line 595 "src/ptrflow.tr"
        while ((cpi < caps->len)) {
#line 596 "src/ptrflow.tr"
            st->own = ({ __auto_type _ord_t910 = (st->own); TrStr _apt_t909 = _tr_str_retain(((HirParam*)List_ptr_get(caps, cpi))->name); __auto_type _wr = (pf_drop_under(_ord_t910, _apt_t909)); _tr_str_release(_apt_t909); _wr; });
#line 597 "src/ptrflow.tr"
            cpi = (cpi + 1LL);
        }
#line 598 "src/ptrflow.tr"
        PfState* c = PfState_copy(st);
#line 599 "src/ptrflow.tr"
        pf_block(cx, body, c);
        _tr_obj_release(c, _trdrop_PfState);
    } else if (_t897.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t897.data.ESuperMethodCall.args;
        pf_exprs(cx, args, st);
    } else if (_t897.tag == HirExpr_EList) {
        __auto_type items = _t897.data.EList.items;
        pf_exprs(cx, items, st);
    } else if (_t897.tag == HirExpr_ESet) {
        __auto_type items = _t897.data.ESet.items;
        pf_exprs(cx, items, st);
    } else if (_t897.tag == HirExpr_ETuple) {
        __auto_type items = _t897.data.ETuple.items;
        pf_exprs(cx, items, st);
    } else if (_t897.tag == HirExpr_EDict) {
        __auto_type keys = _t897.data.EDict.keys;
__auto_type vals = _t897.data.EDict.vals;
#line 605 "src/ptrflow.tr"
        pf_exprs(cx, keys, st);
#line 606 "src/ptrflow.tr"
        pf_exprs(cx, vals, st);
    } else if (_t897.tag == HirExpr_EListComp) {
        __auto_type el = _t897.data.EListComp.element;
__auto_type gens = _t897.data.EListComp.generators;
#line 608 "src/ptrflow.tr"
        long long gi = 0LL;
#line 609 "src/ptrflow.tr"
        while ((gi < gens->len)) {
#line 610 "src/ptrflow.tr"
            pf_expr(cx, (*((HirComprehension**)List_ptr_get(gens, gi)))->iter, st);
#line 611 "src/ptrflow.tr"
            gi = (gi + 1LL);
        }
    } else if (_t897.tag == HirExpr_ESlice) {
        __auto_type a = _t897.data.ESlice.start;
__auto_type b = _t897.data.ESlice.stop;
__auto_type c = _t897.data.ESlice.step;
#line 613 "src/ptrflow.tr"
        pf_expr(cx, a, st);
#line 614 "src/ptrflow.tr"
        pf_expr(cx, b, st);
#line 615 "src/ptrflow.tr"
        pf_expr(cx, c, st);
    } else if (_t897.tag == HirExpr_EAwait) {
        __auto_type x = _t897.data.EAwait.expr;
        pf_expr(cx, x, st);
    } else if (_t897.tag == HirExpr_EAwaitTimeout) {
        __auto_type x = _t897.data.EAwaitTimeout.expr;
__auto_type t = _t897.data.EAwaitTimeout.timeout_ms;
#line 618 "src/ptrflow.tr"
        pf_expr(cx, x, st);
#line 619 "src/ptrflow.tr"
        pf_expr(cx, t, st);
    } else if (_t897.tag == HirExpr_EYield) {
        __auto_type x = _t897.data.EYield.expr;
        pf_expr(cx, x, st);
    } else if (_t897.tag == HirExpr_ERange) {
        __auto_type a = _t897.data.ERange.start;
__auto_type b = _t897.data.ERange.end;
#line 622 "src/ptrflow.tr"
        pf_expr(cx, a, st);
#line 623 "src/ptrflow.tr"
        pf_expr(cx, b, st);
    } else if (_t897.tag == HirExpr_EIfElse) {
        __auto_type c = _t897.data.EIfElse.cond;
__auto_type t = _t897.data.EIfElse.then_e;
__auto_type f = _t897.data.EIfElse.else_e;
#line 625 "src/ptrflow.tr"
        pf_expr(cx, c, st);
#line 626 "src/ptrflow.tr"
        PfState* s1 = PfState_copy(st);
#line 627 "src/ptrflow.tr"
        PfState* s2 = PfState_copy(st);
#line 628 "src/ptrflow.tr"
        pf_expr(cx, t, s1);
#line 629 "src/ptrflow.tr"
        pf_expr(cx, f, s2);
#line 630 "src/ptrflow.tr"
        s1->dead = false;
#line 631 "src/ptrflow.tr"
        s2->dead = false;
#line 632 "src/ptrflow.tr"
        pf_assign_from(st, s1);
#line 633 "src/ptrflow.tr"
        pf_merge(st, s2);
        _tr_obj_release(s1, _trdrop_PfState);
        _tr_obj_release(s2, _trdrop_PfState);
    } else if (_t897.tag == HirExpr_EDo) {
        __auto_type body = _t897.data.EDo.body;
        pf_branchy_block(cx, body, st);
    } else if (_t897.tag == HirExpr_ELoop) {
        __auto_type body = _t897.data.ELoop.body;
        pf_branchy_block(cx, body, st);
    } else if (_t897.tag == HirExpr_EWhileExpr) {
        __auto_type c = _t897.data.EWhileExpr.cond;
__auto_type body = _t897.data.EWhileExpr.body;
__auto_type eb = _t897.data.EWhileExpr.else_body;
#line 637 "src/ptrflow.tr"
        pf_expr(cx, c, st);
#line 638 "src/ptrflow.tr"
        pf_branchy_block(cx, body, st);
#line 639 "src/ptrflow.tr"
        pf_branchy_block(cx, eb, st);
    } else if (_t897.tag == HirExpr_EMatchExpr) {
        __auto_type subj = _t897.data.EMatchExpr.subj;
__auto_type arms = _t897.data.EMatchExpr.arms;
#line 641 "src/ptrflow.tr"
        pf_expr(cx, subj, st);
#line 642 "src/ptrflow.tr"
        long long ai = 0LL;
#line 643 "src/ptrflow.tr"
        while ((ai < arms->len)) {
#line 644 "src/ptrflow.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 645 "src/ptrflow.tr"
            PfState* c = PfState_copy(st);
#line 646 "src/ptrflow.tr"
            pf_expr(cx, a->guard, c);
#line 647 "src/ptrflow.tr"
            pf_block(cx, a->body, c);
#line 648 "src/ptrflow.tr"
            pf_merge(st, c);
#line 649 "src/ptrflow.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c, _trdrop_PfState);
        }
    } else if (_t897.tag == HirExpr_ETry) {
        __auto_type tb = _t897.data.ETry.try_body;
__auto_type catches = _t897.data.ETry.catches;
__auto_type fb = _t897.data.ETry.finally_b;
#line 651 "src/ptrflow.tr"
        pf_branchy_block(cx, tb, st);
#line 652 "src/ptrflow.tr"
        long long ci = 0LL;
#line 653 "src/ptrflow.tr"
        while ((ci < catches->len)) {
#line 654 "src/ptrflow.tr"
            pf_branchy_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body, st);
#line 655 "src/ptrflow.tr"
            ci = (ci + 1LL);
        }
#line 656 "src/ptrflow.tr"
        pf_branchy_block(cx, fb, st);
    } else if (1) {
        __auto_type _ = _t897;
        /* pass */
    }
}

__attribute__((hot)) void pf_leak_warn(PfCtx* cx, TrStr name, TrStr why) {
#line 662 "src/ptrflow.tr"
    if (_tr_str_starts_withv((cx->fn_name), (_tr_str_lit_len("<", 1LL)))) {
#line 662 "src/ptrflow.tr"
        return;
    }
#line 663 "src/ptrflow.tr"
    long long ln = 0LL;
#line 664 "src/ptrflow.tr"
    if (_tr_dict_contains(cx->alloc_line, _tr_strz(name))) {
#line 664 "src/ptrflow.tr"
        ln = ((long long)(uintptr_t)_tr_dict_get(cx->alloc_line, _tr_strz(name)));
    }
#line 665 "src/ptrflow.tr"
    TrStr at = _tr_str_lit_len("", 0LL);
#line 666 "src/ptrflow.tr"
    if ((ln > 0LL)) {
#line 666 "src/ptrflow.tr"
        TrStr _strtmp_t911 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(" (allocated at line ", 20LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(at);
        at = _strtmp_t911;
    }
#line 667 "src/ptrflow.tr"
    ({ TrStr _at_t912 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[P-4] the alloc[] block held by '", 33LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" is ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (why)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": it leaks.\n      FIX: dealloc(", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(") on every path, return/store the pointer so a caller owns it, or wrap the pair in a class with init/drop.", 106LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t912); _tr_str_release(_at_t912); });
    _tr_str_release(at);
}

__attribute__((hot)) void pf_leak_check(PfCtx* cx, PfState* st, TrStr why) {
#line 670 "src/ptrflow.tr"
    if (st->dead) {
#line 670 "src/ptrflow.tr"
        return;
    }
#line 671 "src/ptrflow.tr"
    long long i = 0LL;
#line 672 "src/ptrflow.tr"
    while ((i < st->own->len)) {
#line 673 "src/ptrflow.tr"
        ({ TrStr _at_t913 = (List_TrStr_get(st->own, i)); pf_leak_warn(cx, _at_t913, why); _tr_str_release(_at_t913); });
#line 674 "src/ptrflow.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void pf_bind(PfCtx* cx, PfState* st, TrStr path, HirExpr* val, bool simple) {
#line 678 "src/ptrflow.tr"
    if (pf_is_alloc(val)) {
#line 679 "src/ptrflow.tr"
        if (simple) {
#line 680 "src/ptrflow.tr"
            pf_add(st->own, path);
#line 681 "src/ptrflow.tr"
            _tr_dict_set(cx->alloc_line, _tr_strz(path), cx->line);
        }
#line 682 "src/ptrflow.tr"
        long long n = pf_alloc_n(val);
#line 683 "src/ptrflow.tr"
        if ((n != (-4611686018427387904LL))) {
#line 684 "src/ptrflow.tr"
            if ((n < 0LL)) {
#line 685 "src/ptrflow.tr"
                ({ TrStr _at_t914 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("[P-3] alloc with a negative element count (", 43LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(").\n      FIX: pass a non-negative element count.", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t914); _tr_str_release(_at_t914); });
            } else {
#line 687 "src/ptrflow.tr"
                ({ TrStr _ord_t915 = (({ TrStr _tr_cl = (_tr_strx_concatv((path), (_tr_str_lit_len("=", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(st->sz, _ord_t915); _tr_str_release(_ord_t915); });
            }
        }
    } else if (pf_is_null_lit(val)) {
#line 689 "src/ptrflow.tr"
        pf_add(st->nul, path);
    } else {
#line 691 "src/ptrflow.tr"
        TrStr r = pf_root(val);
#line 692 "src/ptrflow.tr"
        if ((((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) && pf_has(st->nul, r)) && (!_tr_str_eqv((r), (path))))) {
#line 692 "src/ptrflow.tr"
            pf_add(st->nul, path);
        }
        _tr_str_release(r);
    }
}

__attribute__((hot)) void pf_declare(PfCtx* cx, PfState* st, TrStr name, HirExpr* val) {
#line 695 "src/ptrflow.tr"
    if (pf_has(st->own, name)) {
#line 695 "src/ptrflow.tr"
        pf_leak_warn(cx, name, _tr_str_lit_len("overwritten by a new declaration before it is dealloc'd", 55LL));
    }
#line 696 "src/ptrflow.tr"
    pf_kill(st, name);
#line 697 "src/ptrflow.tr"
    if ((!pf_has(cx->locals, name))) {
#line 697 "src/ptrflow.tr"
        List_TrStr_append(cx->locals, name);
    }
#line 698 "src/ptrflow.tr"
    TrStr r = pf_root(val);
#line 699 "src/ptrflow.tr"
    if (((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((r), (name))))) {
#line 699 "src/ptrflow.tr"
        pf_link(st, name, r);
    }
#line 700 "src/ptrflow.tr"
    TrStr ss = pf_stack_src(val, st, cx);
#line 701 "src/ptrflow.tr"
    if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
#line 701 "src/ptrflow.tr"
        pf_add(st->stk, name);
    }
#line 702 "src/ptrflow.tr"
    pf_bind(cx, st, name, val, true);
    _tr_str_release(r);
    _tr_str_release(ss);
}

__attribute__((hot)) void pf_assign(PfCtx* cx, PfState* st, HirExpr* tgt, HirExpr* val) {
#line 705 "src/ptrflow.tr"
    pf_expr(cx, val, st);
#line 706 "src/ptrflow.tr"
    TrStr tp = pf_path(tgt);
#line 707 "src/ptrflow.tr"
    if (_tr_str_eqv((tp), (_tr_str_lit_len("", 0LL)))) {
#line 708 "src/ptrflow.tr"
        pf_expr(cx, tgt, st);
#line 709 "src/ptrflow.tr"
        _tr_str_release(tp);
        return;
    }
#line 710 "src/ptrflow.tr"
    __auto_type _t916 = (*tgt);
    if (_t916.tag == HirExpr_EPropAccess) {
        __auto_type o = _t916.data.EPropAccess.obj;
        pf_expr(cx, o, st);
    } else if (1) {
        __auto_type _ = _t916;
        /* pass */
    }
#line 713 "src/ptrflow.tr"
    TrStr ss = pf_stack_src(val, st, cx);
#line 714 "src/ptrflow.tr"
    TrStr root0 = _tr_str_retain(tp);
#line 715 "src/ptrflow.tr"
    long long dot = _tr_str_index_ofv((root0), (_tr_str_lit_len(".", 1LL)));
#line 716 "src/ptrflow.tr"
    if ((dot >= 0LL)) {
#line 716 "src/ptrflow.tr"
        TrStr _strtmp_t917 = _tr_str_slicev((root0), 0LL, dot);
        _tr_str_release(root0);
        root0 = _strtmp_t917;
    }
#line 717 "src/ptrflow.tr"
    bool outlives = false;
#line 718 "src/ptrflow.tr"
    if ((dot >= 0LL)) {
#line 720 "src/ptrflow.tr"
        if (((!pf_has(cx->locals, root0)) || _tr_str_eqv((root0), (_tr_str_lit_len("self", 4LL))))) {
#line 720 "src/ptrflow.tr"
            outlives = true;
        }
    } else if (((!pf_has(cx->locals, root0)) && (!pf_has(cx->params, root0)))) {
#line 722 "src/ptrflow.tr"
        outlives = true;
    }
#line 723 "src/ptrflow.tr"
    if ((((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL)))) && outlives) && (!cx->is_main))) {
#line 724 "src/ptrflow.tr"
        ({ TrStr _at_t918 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] the address of local '", 28LL)), (ss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is stored into '", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tp)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which outlives this call of '", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cx->fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it dangles once the function returns.\n      FIX: store an owned value, or heap-allocate the data with alloc[T] (and dealloc it when its owner is done) instead of pointing at a stack local.", 194LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t918); _tr_str_release(_at_t918); });
    }
#line 725 "src/ptrflow.tr"
    if (((dot < 0LL) && pf_has(st->own, tp))) {
#line 725 "src/ptrflow.tr"
        pf_leak_warn(cx, tp, _tr_str_lit_len("overwritten before it is dealloc'd", 34LL));
    }
#line 726 "src/ptrflow.tr"
    pf_kill(st, tp);
#line 727 "src/ptrflow.tr"
    TrStr r = pf_root(val);
#line 728 "src/ptrflow.tr"
    if ((((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((r), (tp)))) && (!pf_under(r, tp)))) {
#line 728 "src/ptrflow.tr"
        pf_link(st, tp, r);
    }
#line 729 "src/ptrflow.tr"
    if (((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL)))) && (dot < 0LL))) {
#line 729 "src/ptrflow.tr"
        pf_add(st->stk, tp);
    }
#line 730 "src/ptrflow.tr"
    pf_bind(cx, st, tp, val, ((dot < 0LL) && pf_has(cx->locals, tp)));
    _tr_str_release(tp);
    _tr_str_release(ss);
    _tr_str_release(root0);
    _tr_str_release(r);
}

__attribute__((hot)) void pf_exit_loop_ctx(PfCtx* cx, PfState* brk_out, PfState* cnt_out) {
#line 733 "src/ptrflow.tr"
    long long bb = List_i64_get(cx->brk_base, (cx->brk_base->len - 1LL));
#line 734 "src/ptrflow.tr"
    List_i64_pop(cx->brk_base);
#line 735 "src/ptrflow.tr"
    while ((cx->brk->len > bb)) {
#line 736 "src/ptrflow.tr"
        pf_merge(brk_out, ((PfState*)List_ptr_get(cx->brk, (cx->brk->len - 1LL))));
#line 737 "src/ptrflow.tr"
        ((PfState*)List_ptr_pop(cx->brk));
    }
#line 738 "src/ptrflow.tr"
    long long cb = List_i64_get(cx->cnt_base, (cx->cnt_base->len - 1LL));
#line 739 "src/ptrflow.tr"
    List_i64_pop(cx->cnt_base);
#line 740 "src/ptrflow.tr"
    while ((cx->cnt->len > cb)) {
#line 741 "src/ptrflow.tr"
        pf_merge(cnt_out, ((PfState*)List_ptr_get(cx->cnt, (cx->cnt->len - 1LL))));
#line 742 "src/ptrflow.tr"
        ((PfState*)List_ptr_pop(cx->cnt));
    }
}

__attribute__((hot)) void pf_loop(PfCtx* cx, PfState* st, HirExpr* cond, TrStr lv, HirBlock* body, bool always) {
#line 746 "src/ptrflow.tr"
    PfState* entry = PfState_copy(st);
#line 747 "src/ptrflow.tr"
    PfState* head = PfState_copy(entry);
#line 748 "src/ptrflow.tr"
    long long pass_i = 0LL;
#line 749 "src/ptrflow.tr"
    PfState* exit_st = PfState_init();
#line 750 "src/ptrflow.tr"
    exit_st->dead = true;
#line 751 "src/ptrflow.tr"
    while ((pass_i < 2LL)) {
#line 752 "src/ptrflow.tr"
        PfState* s = PfState_copy(head);
#line 753 "src/ptrflow.tr"
        pf_expr(cx, cond, s);
#line 754 "src/ptrflow.tr"
        PfState* after_cond = PfState_copy(s);
#line 755 "src/ptrflow.tr"
        TrStr ltest = pf_nulltest(cond);
#line 756 "src/ptrflow.tr"
        pf_refine(s, ltest, true);
#line 757 "src/ptrflow.tr"
        pf_refine(after_cond, ltest, false);
#line 758 "src/ptrflow.tr"
        if ((!_tr_str_eqv((lv), (_tr_str_lit_len("", 0LL))))) {
#line 758 "src/ptrflow.tr"
            pf_kill(s, lv);
        }
#line 759 "src/ptrflow.tr"
        List_i64_append(cx->brk_base, cx->brk->len);
#line 760 "src/ptrflow.tr"
        List_i64_append(cx->cnt_base, cx->cnt->len);
#line 761 "src/ptrflow.tr"
        pf_block(cx, body, s);
#line 762 "src/ptrflow.tr"
        PfState* bo = PfState_init();
#line 763 "src/ptrflow.tr"
        bo->dead = true;
#line 764 "src/ptrflow.tr"
        PfState* co = PfState_init();
#line 765 "src/ptrflow.tr"
        co->dead = true;
#line 766 "src/ptrflow.tr"
        pf_exit_loop_ctx(cx, bo, co);
#line 767 "src/ptrflow.tr"
        PfState* back = PfState_copy(s);
#line 768 "src/ptrflow.tr"
        pf_merge(back, co);
#line 769 "src/ptrflow.tr"
        if ((pass_i == 0LL)) {
#line 770 "src/ptrflow.tr"
            PfState* _cltmp_t919 = PfState_copy(entry);
            _tr_obj_release(head, _trdrop_PfState);
            head = _cltmp_t919;
#line 771 "src/ptrflow.tr"
            pf_merge(head, back);
        } else {
#line 773 "src/ptrflow.tr"
            if ((!always)) {
#line 773 "src/ptrflow.tr"
                pf_merge(exit_st, after_cond);
            }
#line 774 "src/ptrflow.tr"
            pf_merge(exit_st, bo);
        }
#line 775 "src/ptrflow.tr"
        pass_i = (pass_i + 1LL);
        _tr_obj_release(s, _trdrop_PfState);
        _tr_obj_release(after_cond, _trdrop_PfState);
        _tr_str_release(ltest);
        _tr_obj_release(bo, _trdrop_PfState);
        _tr_obj_release(co, _trdrop_PfState);
        _tr_obj_release(back, _trdrop_PfState);
    }
#line 776 "src/ptrflow.tr"
    pf_assign_from(st, exit_st);
    _tr_obj_release(entry, _trdrop_PfState);
    _tr_obj_release(head, _trdrop_PfState);
    _tr_obj_release(exit_st, _trdrop_PfState);
}

__attribute__((hot)) void pf_stmt(PfCtx* cx, HirStmt* sp, PfState* st) {
#line 779 "src/ptrflow.tr"
    if (st->dead) {
#line 779 "src/ptrflow.tr"
        return;
    }
#line 780 "src/ptrflow.tr"
    __auto_type _t920 = (*sp);
    if (_t920.tag == HirStmt_SLineMarker) {
        __auto_type n = _t920.data.SLineMarker.n;
        cx->line = n;
    } else if (_t920.tag == HirStmt_SExpr) {
        __auto_type e = _t920.data.SExpr.expr;
        pf_expr(cx, e, st);
    } else if (_t920.tag == HirStmt_SLet) {
        __auto_type nm = _t920.data.SLet.name;
__auto_type val = _t920.data.SLet.val;
#line 784 "src/ptrflow.tr"
        pf_expr(cx, val, st);
#line 785 "src/ptrflow.tr"
        pf_declare(cx, st, nm, val);
    } else if (_t920.tag == HirStmt_SMultiLet) {
        __auto_type names = _t920.data.SMultiLet.names;
__auto_type val = _t920.data.SMultiLet.val;
#line 787 "src/ptrflow.tr"
        pf_expr(cx, val, st);
#line 788 "src/ptrflow.tr"
        long long ni = 0LL;
#line 789 "src/ptrflow.tr"
        while ((ni < names->len)) {
#line 790 "src/ptrflow.tr"
            ({ TrStr _at_t921 = (List_TrStr_get(names, ni)); pf_kill(st, _at_t921); _tr_str_release(_at_t921); });
#line 791 "src/ptrflow.tr"
            if (({ __auto_type _ord_t923 = (cx->locals); TrStr _at_t922 = (List_TrStr_get(names, ni)); __auto_type _wr = ((!pf_has(_ord_t923, _at_t922))); _tr_str_release(_at_t922); _wr; })) {
#line 791 "src/ptrflow.tr"
                ({ TrStr _ord_t924 = (List_TrStr_get(names, ni)); List_TrStr_append(cx->locals, _ord_t924); _tr_str_release(_ord_t924); });
            }
#line 792 "src/ptrflow.tr"
            ni = (ni + 1LL);
        }
    } else if (_t920.tag == HirStmt_SAssign) {
        __auto_type tgt = _t920.data.SAssign.target;
__auto_type val = _t920.data.SAssign.val;
        pf_assign(cx, st, tgt, val);
    } else if (_t920.tag == HirStmt_SReturn) {
        __auto_type v = _t920.data.SReturn.val;
#line 795 "src/ptrflow.tr"
        pf_expr(cx, v, st);
#line 796 "src/ptrflow.tr"
        if (((!cx->ret_from) && (!cx->is_main))) {
#line 797 "src/ptrflow.tr"
            TrStr ss = pf_stack_src(v, st, cx);
#line 798 "src/ptrflow.tr"
            if ((!_tr_str_eqv((ss), (_tr_str_lit_len("", 0LL))))) {
#line 799 "src/ptrflow.tr"
                ({ TrStr _at_t925 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[L-1] returning the address of local '", 38LL)), (ss))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': the local dies when '", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cx->fn_name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' returns, so the pointer dangles.\n      FIX: return an owned value, heap-allocate with alloc[T] (the caller must dealloc it), or annotate the return type with 'from <param>' when the pointer borrows from a parameter.", 217LL))); _tr_str_release(_tr_cl); _tr_cres; })); PfCtx_err(cx, _at_t925); _tr_str_release(_at_t925); });
            }
        }
#line 800 "src/ptrflow.tr"
        pf_leak_check(cx, st, _tr_str_lit_len("not dealloc'd on this path before the return", 44LL));
#line 801 "src/ptrflow.tr"
        pf_merge(cx->ret_acc, st);
#line 802 "src/ptrflow.tr"
        cx->ret_seen = true;
#line 803 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t920.tag == HirStmt_SRaise) {
        __auto_type v = _t920.data.SRaise.val;
#line 805 "src/ptrflow.tr"
        pf_expr(cx, v, st);
#line 806 "src/ptrflow.tr"
        pf_leak_check(cx, st, _tr_str_lit_len("not dealloc'd on this path before the raise", 43LL));
#line 807 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t920.tag == HirStmt_SDefer) {
        __auto_type ds = _t920.data.SDefer.stmt;
#line 809 "src/ptrflow.tr"
        cx->in_defer = true;
#line 810 "src/ptrflow.tr"
        pf_stmt(cx, ds, st);
#line 811 "src/ptrflow.tr"
        cx->in_defer = false;
    } else if (_t920.tag == HirStmt_SBreak) {
        __auto_type v = _t920.data.SBreak.val;
#line 813 "src/ptrflow.tr"
        pf_expr(cx, v, st);
#line 814 "src/ptrflow.tr"
        if ((cx->brk_base->len > 0LL)) {
#line 814 "src/ptrflow.tr"
            ({ PfState* _ord_t926 = (PfState_copy(st)); List_ptr_append(cx->brk, _tr_obj_retain(_ord_t926)); _tr_obj_release(_ord_t926, _trdrop_PfState); });
        }
#line 815 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t920.tag == HirStmt_SContinue) {
#line 817 "src/ptrflow.tr"
        if ((cx->cnt_base->len > 0LL)) {
#line 817 "src/ptrflow.tr"
            ({ PfState* _ord_t927 = (PfState_copy(st)); List_ptr_append(cx->cnt, _tr_obj_retain(_ord_t927)); _tr_obj_release(_ord_t927, _trdrop_PfState); });
        }
#line 818 "src/ptrflow.tr"
        st->dead = true;
    } else if (_t920.tag == HirStmt_SUnsafe) {
        __auto_type body = _t920.data.SUnsafe.body;
#line 820 "src/ptrflow.tr"
        cx->unsafe_depth = (cx->unsafe_depth + 1LL);
#line 821 "src/ptrflow.tr"
        pf_block(cx, body, st);
#line 822 "src/ptrflow.tr"
        cx->unsafe_depth = (cx->unsafe_depth - 1LL);
    } else if (_t920.tag == HirStmt_SIf) {
        __auto_type cond = _t920.data.SIf.cond;
__auto_type tb = _t920.data.SIf.then_b;
__auto_type eb = _t920.data.SIf.else_b;
#line 824 "src/ptrflow.tr"
        pf_expr(cx, cond, st);
#line 825 "src/ptrflow.tr"
        PfState* s1 = PfState_copy(st);
#line 826 "src/ptrflow.tr"
        PfState* s2 = PfState_copy(st);
#line 827 "src/ptrflow.tr"
        TrStr ntest = pf_nulltest(cond);
#line 828 "src/ptrflow.tr"
        pf_refine(s1, ntest, true);
#line 829 "src/ptrflow.tr"
        pf_refine(s2, ntest, false);
#line 830 "src/ptrflow.tr"
        pf_block(cx, tb, s1);
#line 831 "src/ptrflow.tr"
        pf_block(cx, eb, s2);
#line 832 "src/ptrflow.tr"
        pf_assign_from(st, s1);
#line 833 "src/ptrflow.tr"
        pf_merge(st, s2);
        _tr_obj_release(s1, _trdrop_PfState);
        _tr_obj_release(s2, _trdrop_PfState);
        _tr_str_release(ntest);
    } else if (_t920.tag == HirStmt_SWhile) {
        __auto_type cond = _t920.data.SWhile.cond;
__auto_type body = _t920.data.SWhile.body;
#line 835 "src/ptrflow.tr"
        bool always = false;
#line 836 "src/ptrflow.tr"
        if ((!pf_null(cond))) {
#line 837 "src/ptrflow.tr"
            __auto_type _t928 = (*cond);
            if (_t928.tag == HirExpr_ELitBool) {
                __auto_type bv = _t928.data.ELitBool.val;
                always = bv;
            } else if (1) {
                __auto_type _ = _t928;
                /* pass */
            }
        }
#line 840 "src/ptrflow.tr"
        pf_loop(cx, st, cond, _tr_str_lit_len("", 0LL), body, always);
    } else if (_t920.tag == HirStmt_SFor) {
        __auto_type var = _t920.data.SFor.var;
__auto_type it = _t920.data.SFor.iter;
__auto_type body = _t920.data.SFor.body;
#line 842 "src/ptrflow.tr"
        pf_expr(cx, it, st);
#line 843 "src/ptrflow.tr"
        if ((!pf_has(cx->locals, var))) {
#line 843 "src/ptrflow.tr"
            List_TrStr_append(cx->locals, var);
        }
#line 844 "src/ptrflow.tr"
        pf_loop(cx, st, (HirExpr*)(0LL), var, body, false);
    } else if (_t920.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t920.data.SForUnpack.vars;
__auto_type it = _t920.data.SForUnpack.iter;
__auto_type body = _t920.data.SForUnpack.body;
#line 846 "src/ptrflow.tr"
        pf_expr(cx, it, st);
#line 847 "src/ptrflow.tr"
        long long vi = 0LL;
#line 848 "src/ptrflow.tr"
        while ((vi < vars->len)) {
#line 849 "src/ptrflow.tr"
            if (({ __auto_type _ord_t930 = (cx->locals); TrStr _at_t929 = (List_TrStr_get(vars, vi)); __auto_type _wr = ((!pf_has(_ord_t930, _at_t929))); _tr_str_release(_at_t929); _wr; })) {
#line 849 "src/ptrflow.tr"
                ({ TrStr _ord_t931 = (List_TrStr_get(vars, vi)); List_TrStr_append(cx->locals, _ord_t931); _tr_str_release(_ord_t931); });
            }
#line 850 "src/ptrflow.tr"
            vi = (vi + 1LL);
        }
#line 851 "src/ptrflow.tr"
        pf_loop(cx, st, (HirExpr*)(0LL), _tr_str_lit_len("", 0LL), body, false);
    } else if (_t920.tag == HirStmt_SMatch) {
        __auto_type subj = _t920.data.SMatch.expr;
__auto_type arms = _t920.data.SMatch.arms;
#line 853 "src/ptrflow.tr"
        pf_expr(cx, subj, st);
#line 854 "src/ptrflow.tr"
        PfState* acc = PfState_init();
#line 855 "src/ptrflow.tr"
        acc->dead = true;
#line 856 "src/ptrflow.tr"
        long long ai = 0LL;
#line 857 "src/ptrflow.tr"
        while ((ai < arms->len)) {
#line 858 "src/ptrflow.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 859 "src/ptrflow.tr"
            PfState* c = PfState_copy(st);
#line 860 "src/ptrflow.tr"
            pf_expr(cx, a->guard, c);
#line 861 "src/ptrflow.tr"
            pf_block(cx, a->body, c);
#line 862 "src/ptrflow.tr"
            pf_merge(acc, c);
#line 863 "src/ptrflow.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c, _trdrop_PfState);
        }
#line 864 "src/ptrflow.tr"
        if ((arms->len > 0LL)) {
#line 865 "src/ptrflow.tr"
            pf_assign_from(st, acc);
        }
        _tr_obj_release(acc, _trdrop_PfState);
    } else if (_t920.tag == HirStmt_STry) {
        __auto_type tb = _t920.data.STry.try_body;
__auto_type catches = _t920.data.STry.catches;
__auto_type fb = _t920.data.STry.finally_b;
#line 867 "src/ptrflow.tr"
        PfState* entry = PfState_copy(st);
#line 868 "src/ptrflow.tr"
        PfState* s1 = PfState_copy(st);
#line 869 "src/ptrflow.tr"
        pf_block(cx, tb, s1);
#line 870 "src/ptrflow.tr"
        PfState* acc = PfState_copy(s1);
#line 871 "src/ptrflow.tr"
        PfState* hs = PfState_copy(entry);
#line 872 "src/ptrflow.tr"
        pf_merge(hs, s1);
#line 873 "src/ptrflow.tr"
        hs->dead = false;
#line 874 "src/ptrflow.tr"
        long long ci = 0LL;
#line 875 "src/ptrflow.tr"
        while ((ci < catches->len)) {
#line 876 "src/ptrflow.tr"
            PfState* cs = PfState_copy(hs);
#line 877 "src/ptrflow.tr"
            pf_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body, cs);
#line 878 "src/ptrflow.tr"
            pf_merge(acc, cs);
#line 879 "src/ptrflow.tr"
            ci = (ci + 1LL);
            _tr_obj_release(cs, _trdrop_PfState);
        }
#line 880 "src/ptrflow.tr"
        pf_block(cx, fb, acc);
#line 881 "src/ptrflow.tr"
        pf_assign_from(st, acc);
        _tr_obj_release(entry, _trdrop_PfState);
        _tr_obj_release(s1, _trdrop_PfState);
        _tr_obj_release(acc, _trdrop_PfState);
        _tr_obj_release(hs, _trdrop_PfState);
    } else if (_t920.tag == HirStmt_SAssert) {
        __auto_type c = _t920.data.SAssert.cond;
__auto_type m = _t920.data.SAssert.msg;
#line 883 "src/ptrflow.tr"
        pf_expr(cx, c, st);
#line 884 "src/ptrflow.tr"
        pf_expr(cx, m, st);
    } else if (_t920.tag == HirStmt_SWith) {
        __auto_type items = _t920.data.SWith.items;
__auto_type body = _t920.data.SWith.body;
#line 886 "src/ptrflow.tr"
        pf_exprs(cx, items, st);
#line 887 "src/ptrflow.tr"
        pf_block(cx, body, st);
    } else if (_t920.tag == HirStmt_SSpawn) {
        __auto_type e = _t920.data.SSpawn.expr;
        pf_expr(cx, e, st);
    } else if (_t920.tag == HirStmt_STaskGroup) {
        __auto_type body = _t920.data.STaskGroup.body;
        pf_branchy_block(cx, body, st);
    } else if (_t920.tag == HirStmt_SGpuBlock) {
        __auto_type body = _t920.data.SGpuBlock.body;
        pf_branchy_block(cx, body, st);
    } else if (1) {
        __auto_type _ = _t920;
        /* pass */
    }
}

__attribute__((hot)) List_i64* pf_run_fn(HirFunction* hf, TrMap* summaries, TrMap* externs, bool report, List_TrStr* errs) {
#line 896 "src/ptrflow.tr"
    PfCtx* cx = PfCtx_init(summaries);
#line 897 "src/ptrflow.tr"
    cx->externs = externs;
#line 898 "src/ptrflow.tr"
    cx->report = report;
#line 899 "src/ptrflow.tr"
    cx->fn_name = _tr_str_retain(hf->name);
#line 900 "src/ptrflow.tr"
    cx->is_main = (_tr_str_eqv((hf->name), (_tr_str_lit_len("main", 4LL))) && _tr_str_eqv((hf->class_name), (_tr_str_lit_len("", 0LL))));
#line 901 "src/ptrflow.tr"
    cx->ret_from = (!_tr_str_eqv((hf->ret_ty->from_param), (_tr_str_lit_len("", 0LL))));
#line 902 "src/ptrflow.tr"
    long long pi = 0LL;
#line 903 "src/ptrflow.tr"
    while ((pi < hf->params->len)) {
#line 904 "src/ptrflow.tr"
        ({ __auto_type _ord_t932 = (((HirParam*)List_ptr_get(hf->params, pi))->name); List_TrStr_append(cx->params, _ord_t932); });
#line 905 "src/ptrflow.tr"
        pi = (pi + 1LL);
    }
#line 906 "src/ptrflow.tr"
    PfState* st = PfState_init();
#line 907 "src/ptrflow.tr"
    pf_block(cx, hf->body, st);
#line 908 "src/ptrflow.tr"
    pf_leak_check(cx, st, _tr_str_lit_len("not dealloc'd on the path that falls off the end of the function", 64LL));
#line 909 "src/ptrflow.tr"
    pf_merge(cx->ret_acc, st);
#line 910 "src/ptrflow.tr"
    List_i64* out = (void*)List_i64_new();
#line 911 "src/ptrflow.tr"
    pi = 0LL;
#line 912 "src/ptrflow.tr"
    while ((pi < hf->params->len)) {
#line 913 "src/ptrflow.tr"
        if ((({ __auto_type _ord_t934 = (cx->ret_acc->freed); TrStr _apt_t933 = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name); __auto_type _wr = (pf_has(_ord_t934, _apt_t933)); _tr_str_release(_apt_t933); _wr; }) && _tr_str_eqv((((HirParam*)List_ptr_get(hf->params, pi))->ty->name), (_tr_str_lit_len("Pointer", 7LL))))) {
#line 913 "src/ptrflow.tr"
            List_i64_append(out, pi);
        }
#line 914 "src/ptrflow.tr"
        pi = (pi + 1LL);
    }
#line 915 "src/ptrflow.tr"
    long long ei = 0LL;
#line 916 "src/ptrflow.tr"
    while ((ei < cx->errs->len)) {
#line 917 "src/ptrflow.tr"
        ({ TrStr _at_t935 = (List_TrStr_get(cx->errs, ei)); List_TrStr_append(errs, _at_t935); _tr_str_release(_at_t935); });
#line 918 "src/ptrflow.tr"
        ei = (ei + 1LL);
    }
#line 919 "src/ptrflow.tr"
    _tr_obj_release(cx, _trdrop_PfCtx);
    _tr_obj_release(st, _trdrop_PfState);
    return out;
}

__attribute__((hot)) List_TrStr* ptrflow_check(List_ptr* fns, TrMap* externs) {
#line 924 "src/ptrflow.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 925 "src/ptrflow.tr"
    TrMap* summaries = _tr_dict_new(8LL);
#line 926 "src/ptrflow.tr"
    long long rnd = 0LL;
#line 927 "src/ptrflow.tr"
    while ((rnd < 2LL)) {
#line 928 "src/ptrflow.tr"
        long long fi = 0LL;
#line 929 "src/ptrflow.tr"
        while ((fi < fns->len)) {
#line 930 "src/ptrflow.tr"
            HirFunction* hf = ((HirFunction*)List_ptr_get(fns, fi));
#line 931 "src/ptrflow.tr"
            if (_tr_str_eqv((hf->class_name), (_tr_str_lit_len("", 0LL)))) {
#line 932 "src/ptrflow.tr"
                List_TrStr* sink = (void*)List_TrStr_new();
#line 933 "src/ptrflow.tr"
                List_i64* fr = pf_run_fn(hf, summaries, externs, false, sink);
#line 934 "src/ptrflow.tr"
                if ((fr->len > 0LL)) {
#line 934 "src/ptrflow.tr"
                    _tr_dict_set(summaries, _tr_strz(hf->name), fr);
                }
            }
#line 935 "src/ptrflow.tr"
            fi = (fi + 1LL);
        }
#line 936 "src/ptrflow.tr"
        rnd = (rnd + 1LL);
    }
#line 937 "src/ptrflow.tr"
    long long fi2 = 0LL;
#line 938 "src/ptrflow.tr"
    while ((fi2 < fns->len)) {
#line 939 "src/ptrflow.tr"
        List_TrStr* errs = (void*)List_TrStr_new();
#line 940 "src/ptrflow.tr"
        pf_run_fn(((HirFunction*)List_ptr_get(fns, fi2)), summaries, externs, true, errs);
#line 941 "src/ptrflow.tr"
        long long ei = 0LL;
#line 942 "src/ptrflow.tr"
        while ((ei < errs->len)) {
#line 943 "src/ptrflow.tr"
            ({ __auto_type _ord_t936 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(fi2)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("@", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t937 = (({ TrStr _tr_cl = (_ord_t936); TrStr _tr_cr = (List_TrStr_get(errs, ei)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(out, _at_t937); _tr_str_release(_at_t937); });
#line 944 "src/ptrflow.tr"
            ei = (ei + 1LL);
        }
#line 945 "src/ptrflow.tr"
        fi2 = (fi2 + 1LL);
    }
#line 946 "src/ptrflow.tr"
    return out;
}

