#include "tauraro_types.h"

bool mv_has(List_TrStr* v, TrStr s);
void mv_add(List_TrStr* v, TrStr s);
List_TrStr* mv_without(List_TrStr* v, TrStr s);
List_TrStr* mv_inter(List_TrStr* a, List_TrStr* b);
void mv_assign_from(MvState* into, MvState* o);
void mv_merge(MvState* into, MvState* o);
bool mv_null(HirExpr* e);
bool mv_scalar(TrStr n);
bool mv_is_move(MvCtx* cx, AstType* t);
bool mv_tracked(MvCtx* cx, TrStr nm);
void mv_use(MvCtx* cx, TrStr nm, MvState* st);
void mv_mark(MvCtx* cx, TrStr nm, MvState* st);
void mv_reinit(MvCtx* cx, TrStr nm, MvState* st);
void mv_declare(MvCtx* cx, TrStr nm, MvState* st);
void mv_idents(HirExpr* e, List_TrStr* out);
TrStr mv_strip(TrStr n);
List_i64* mv_all_mask(long long n, long long v);
List_i64* mv_sig_mask(MvCtx* cx, TrStr key, long long n, bool skip_self);
bool mv_coll_type(TrStr t);
bool mv_store_method(TrStr m);
bool mv_ctor_type(TrStr t);
TrStr mv_callee_name(HirExpr* c);
TrStr mv_static_base(MvCtx* cx, HirExpr* o);
List_i64* mv_mask_static(MvCtx* cx, TrStr tn, TrStr v, long long n);
List_i64* mv_mask_method(MvCtx* cx, TrStr rt, TrStr m, long long n);
void mv_block(MvCtx* cx, HirBlock* b, MvState* st);
void mv_exprs(MvCtx* cx, List_ptr* v, MvState* st);
void mv_branchy_block(MvCtx* cx, HirBlock* b, MvState* st);
void mv_consume_caps(MvCtx* cx, List_TrStr* caps, MvState* st);
List_TrStr* mv_closure_caps(MvCtx* cx, List_ptr* caps);
void mv_consume(MvCtx* cx, HirExpr* e, MvState* st, long long mode);
void mv_args(MvCtx* cx, List_ptr* args, List_i64* mask, MvState* st, TrStr cname);
void mv_items(MvCtx* cx, List_ptr* items, MvState* st);
void mv_expr(MvCtx* cx, HirExpr* e, MvState* st);
void mv_pat_names(Pattern p, List_TrStr* out);
void mv_pat(MvCtx* cx, Pattern p, MvState* st);
void mv_exit_loop_ctx(MvCtx* cx, MvState* brk_out, MvState* cnt_out);
void mv_loop(MvCtx* cx, MvState* st, HirExpr* cond, List_TrStr* lvs, HirBlock* body, bool always);
void mv_bind_rhs(MvCtx* cx, HirExpr* val, MvState* st);
void mv_note_closure(MvCtx* cx, TrStr nm, HirExpr* val);
void mv_stmt(MvCtx* cx, HirStmt* sp, MvState* st);
List_TrStr* mv_run_fn(MvCtx* cx, HirFunction* hf);
void mv_add_sig(MvCtx* cx, HirFunction* f, TrStr key, List_TrStr* cgens);

__attribute__((malloc,returns_nonnull,hot)) MvState* MvState_init() {
#line 37 "src/movecheck.tr"
    MvState* s = ((MvState*)_tr_obj_alloc(sizeof(MvState)));
#line 38 "src/movecheck.tr"
    s->any = (void*)List_TrStr_new();
#line 39 "src/movecheck.tr"
    s->all = (void*)List_TrStr_new();
#line 40 "src/movecheck.tr"
    s->lp = (void*)List_TrStr_new();
#line 41 "src/movecheck.tr"
    s->dead = false;
#line 42 "src/movecheck.tr"
    return s;
}

__attribute__((hot)) MvState* MvState_copy(MvState* self) {
#line 45 "src/movecheck.tr"
    MvState* s = MvState_init();
#line 46 "src/movecheck.tr"
    long long i = 0LL;
#line 47 "src/movecheck.tr"
    while ((i < self->any->len)) {
#line 48 "src/movecheck.tr"
        ({ TrStr _ord_t1030 = (List_TrStr_get(self->any, i)); List_TrStr_append(s->any, _ord_t1030); _tr_str_release(_ord_t1030); });
#line 49 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 50 "src/movecheck.tr"
    i = 0LL;
#line 51 "src/movecheck.tr"
    while ((i < self->all->len)) {
#line 52 "src/movecheck.tr"
        ({ TrStr _ord_t1031 = (List_TrStr_get(self->all, i)); List_TrStr_append(s->all, _ord_t1031); _tr_str_release(_ord_t1031); });
#line 53 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 54 "src/movecheck.tr"
    i = 0LL;
#line 55 "src/movecheck.tr"
    while ((i < self->lp->len)) {
#line 56 "src/movecheck.tr"
        ({ TrStr _ord_t1032 = (List_TrStr_get(self->lp, i)); List_TrStr_append(s->lp, _ord_t1032); _tr_str_release(_ord_t1032); });
#line 57 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 58 "src/movecheck.tr"
    s->dead = self->dead;
#line 59 "src/movecheck.tr"
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) MvCtx* MvCtx_init() {
#line 137 "src/movecheck.tr"
    MvCtx* c = ((MvCtx*)_tr_obj_alloc(sizeof(MvCtx)));
#line 138 "src/movecheck.tr"
    c->errs = (void*)List_TrStr_new();
#line 139 "src/movecheck.tr"
    c->line = 0LL;
#line 140 "src/movecheck.tr"
    TrStr _sfo_t1033 = c->fn_name;
    c->fn_name = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t1033);
#line 141 "src/movecheck.tr"
    c->mv = _tr_dict_new(8LL);
#line 142 "src/movecheck.tr"
    c->old = _tr_dict_new(8LL);
#line 143 "src/movecheck.tr"
    c->sigs = _tr_dict_new(8LL);
#line 144 "src/movecheck.tr"
    c->amb = _tr_dict_new(2LL);
#line 145 "src/movecheck.tr"
    c->declared = (void*)List_TrStr_new();
#line 146 "src/movecheck.tr"
    c->mline = _tr_dict_new(4LL);
#line 147 "src/movecheck.tr"
    c->clcaps = _tr_dict_new(2LL);
#line 148 "src/movecheck.tr"
    c->defers = (void*)List_TrStr_new();
#line 149 "src/movecheck.tr"
    c->brk = (void*)List_ptr_new();
#line 150 "src/movecheck.tr"
    c->brk_base = (void*)List_i64_new();
#line 151 "src/movecheck.tr"
    c->cnt = (void*)List_ptr_new();
#line 152 "src/movecheck.tr"
    c->cnt_base = (void*)List_i64_new();
#line 153 "src/movecheck.tr"
    c->unsafe_depth = 0LL;
#line 154 "src/movecheck.tr"
    c->in_defer = false;
#line 155 "src/movecheck.tr"
    c->sh_name = (void*)List_TrStr_new();
#line 156 "src/movecheck.tr"
    c->sh_bits = (void*)List_i64_new();
#line 157 "src/movecheck.tr"
    c->sh_base = (void*)List_i64_new();
#line 158 "src/movecheck.tr"
    c->try_acc = (void*)List_TrStr_new();
#line 159 "src/movecheck.tr"
    c->try_depth = 0LL;
#line 160 "src/movecheck.tr"
    TrStr _sfo_t1034 = c->site;
    c->site = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t1034);
#line 161 "src/movecheck.tr"
    TrStr _sfo_t1035 = c->callee;
    c->callee = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t1035);
#line 162 "src/movecheck.tr"
    c->msite = _tr_dict_new(4LL);
#line 163 "src/movecheck.tr"
    return c;
}

__attribute__((hot)) void MvCtx_err(MvCtx* self, TrStr msg) {
#line 166 "src/movecheck.tr"
    TrStr e = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(self->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("|", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (msg)); _tr_str_release(_tr_cl); _tr_cres; });
#line 167 "src/movecheck.tr"
    if ((!mv_has(self->errs, e))) {
#line 167 "src/movecheck.tr"
        List_TrStr_append(self->errs, e);
    }
    _tr_str_release(e);
}

__attribute__((hot)) bool mv_has(List_TrStr* v, TrStr s) {
#line 62 "src/movecheck.tr"
    long long i = 0LL;
#line 63 "src/movecheck.tr"
    while ((i < v->len)) {
#line 64 "src/movecheck.tr"
        if (({ TrStr _cmpa_t1036 = (List_TrStr_get(v, i)); TrStr _cmpb_t1037 = (s); _Bool _cmpr_t1038 = _tr_str_eqv(_cmpa_t1036, _cmpb_t1037); _tr_str_release(_cmpa_t1036); _cmpr_t1038; })) {
#line 64 "src/movecheck.tr"
            return true;
        }
#line 65 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 66 "src/movecheck.tr"
    return false;
}

__attribute__((hot)) void mv_add(List_TrStr* v, TrStr s) {
#line 69 "src/movecheck.tr"
    if ((!mv_has(v, s))) {
#line 69 "src/movecheck.tr"
        List_TrStr_append(v, s);
    }
}

__attribute__((hot)) List_TrStr* mv_without(List_TrStr* v, TrStr s) {
#line 72 "src/movecheck.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 73 "src/movecheck.tr"
    long long i = 0LL;
#line 74 "src/movecheck.tr"
    while ((i < v->len)) {
#line 75 "src/movecheck.tr"
        if (({ TrStr _cmpa_t1039 = (List_TrStr_get(v, i)); TrStr _cmpb_t1040 = (s); _Bool _cmpr_t1041 = !_tr_str_eqv(_cmpa_t1039, _cmpb_t1040); _tr_str_release(_cmpa_t1039); _cmpr_t1041; })) {
#line 75 "src/movecheck.tr"
            ({ TrStr _at_t1042 = (List_TrStr_get(v, i)); List_TrStr_append(out, _at_t1042); _tr_str_release(_at_t1042); });
        }
#line 76 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 77 "src/movecheck.tr"
    return out;
}

__attribute__((hot)) List_TrStr* mv_inter(List_TrStr* a, List_TrStr* b) {
#line 80 "src/movecheck.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 81 "src/movecheck.tr"
    long long i = 0LL;
#line 82 "src/movecheck.tr"
    while ((i < a->len)) {
#line 83 "src/movecheck.tr"
        if (({ TrStr _at_t1043 = (List_TrStr_get(a, i)); __auto_type _wr = (mv_has(b, _at_t1043)); _tr_str_release(_at_t1043); _wr; })) {
#line 83 "src/movecheck.tr"
            ({ TrStr _at_t1044 = (List_TrStr_get(a, i)); List_TrStr_append(out, _at_t1044); _tr_str_release(_at_t1044); });
        }
#line 84 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 85 "src/movecheck.tr"
    return out;
}

__attribute__((hot)) void mv_assign_from(MvState* into, MvState* o) {
#line 88 "src/movecheck.tr"
    into->any = MvState_copy(o)->any;
#line 89 "src/movecheck.tr"
    into->all = MvState_copy(o)->all;
#line 90 "src/movecheck.tr"
    into->lp = MvState_copy(o)->lp;
#line 91 "src/movecheck.tr"
    into->dead = o->dead;
}

__attribute__((hot)) void mv_merge(MvState* into, MvState* o) {
#line 94 "src/movecheck.tr"
    if (o->dead) {
#line 94 "src/movecheck.tr"
        return;
    }
#line 95 "src/movecheck.tr"
    if (into->dead) {
#line 96 "src/movecheck.tr"
        mv_assign_from(into, o);
#line 97 "src/movecheck.tr"
        return;
    }
#line 98 "src/movecheck.tr"
    long long i = 0LL;
#line 99 "src/movecheck.tr"
    while ((i < o->any->len)) {
#line 100 "src/movecheck.tr"
        ({ __auto_type _ord_t1046 = (into->any); TrStr _at_t1045 = (List_TrStr_get(o->any, i)); mv_add(_ord_t1046, _at_t1045); _tr_str_release(_at_t1045); });
#line 101 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 102 "src/movecheck.tr"
    i = 0LL;
#line 103 "src/movecheck.tr"
    while ((i < o->lp->len)) {
#line 104 "src/movecheck.tr"
        ({ __auto_type _ord_t1048 = (into->lp); TrStr _at_t1047 = (List_TrStr_get(o->lp, i)); mv_add(_ord_t1048, _at_t1047); _tr_str_release(_at_t1047); });
#line 105 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 106 "src/movecheck.tr"
    into->all = mv_inter(into->all, o->all);
}

__attribute__((hot)) bool mv_null(HirExpr* e) {
#line 170 "src/movecheck.tr"
    return (((unsigned long long)(e)) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) bool mv_scalar(TrStr n) {
#line 175 "src/movecheck.tr"
    return (((((((((((((((((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("int", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("void", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("None", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("", 0LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("i64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("u64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("usize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("isize", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Pointer", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RawPtr", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StrView", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("def", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("lambda", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Any", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("any", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("object", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ref", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("mut_ref", 7LL))));
}

__attribute__((hot)) bool mv_is_move(MvCtx* cx, AstType* t) {
#line 178 "src/movecheck.tr"
    if (t->is_borrow) {
#line 178 "src/movecheck.tr"
        return false;
    }
#line 179 "src/movecheck.tr"
    TrStr n = _tr_str_retain(t->name);
#line 180 "src/movecheck.tr"
    if (mv_scalar(n)) {
#line 180 "src/movecheck.tr"
        _tr_str_release(n);
        return false;
    }
#line 181 "src/movecheck.tr"
    if ((t->array_size > 0LL)) {
#line 182 "src/movecheck.tr"
        if ((t->args->len > 0LL)) {
#line 182 "src/movecheck.tr"
            bool _trret200 = mv_is_move(cx, (*((AstType**)List_ptr_get(t->args, 0LL))));
            _tr_str_release(n);
            return _trret200;
        }
#line 183 "src/movecheck.tr"
        _tr_str_release(n);
        return false;
    }
#line 184 "src/movecheck.tr"
    if ((((_tr_str_eqv((n), (_tr_str_lit_len("Tuple", 5LL))) || _tr_str_eqv((n), (_tr_str_lit_len("tuple", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Result", 6LL))))) {
#line 185 "src/movecheck.tr"
        long long i = 0LL;
#line 186 "src/movecheck.tr"
        while ((i < t->args->len)) {
#line 187 "src/movecheck.tr"
            if (mv_is_move(cx, (*((AstType**)List_ptr_get(t->args, i))))) {
#line 187 "src/movecheck.tr"
                _tr_str_release(n);
                return true;
            }
#line 188 "src/movecheck.tr"
            i = (i + 1LL);
        }
#line 189 "src/movecheck.tr"
        _tr_str_release(n);
        return false;
    }
#line 190 "src/movecheck.tr"
    if ((((((((((_tr_str_eqv((n), (_tr_str_lit_len("str", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("List", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Deque", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL))))) {
#line 190 "src/movecheck.tr"
        _tr_str_release(n);
        return true;
    }
#line 191 "src/movecheck.tr"
    if (_tr_dict_contains(cx->mv, _tr_strz(n))) {
#line 191 "src/movecheck.tr"
        bool _trret201 = ((bool)(uintptr_t)_tr_dict_get(cx->mv, _tr_strz(n)));
        _tr_str_release(n);
        return _trret201;
    }
#line 192 "src/movecheck.tr"
    _tr_str_release(n);
    return false;
}

__attribute__((hot)) bool mv_tracked(MvCtx* cx, TrStr nm) {
#line 195 "src/movecheck.tr"
    return ((!_tr_str_eqv((nm), (_tr_str_lit_len("self", 4LL)))) && mv_has(cx->declared, nm));
}

__attribute__((hot)) void mv_use(MvCtx* cx, TrStr nm, MvState* st) {
#line 200 "src/movecheck.tr"
    if (st->dead) {
#line 200 "src/movecheck.tr"
        return;
    }
#line 201 "src/movecheck.tr"
    if ((!mv_has(st->any, nm))) {
#line 201 "src/movecheck.tr"
        return;
    }
#line 202 "src/movecheck.tr"
    TrStr at = _tr_str_lit_len("", 0LL);
#line 203 "src/movecheck.tr"
    if (_tr_dict_contains(cx->mline, _tr_strz(nm))) {
#line 204 "src/movecheck.tr"
        TrStr _strtmp_t1049 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(((long long)(uintptr_t)_tr_dict_get(cx->mline, _tr_strz(nm))))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(" (moved at line ", 16LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(at);
        at = _strtmp_t1049;
#line 205 "src/movecheck.tr"
        if (_tr_dict_contains(cx->msite, _tr_strz(nm))) {
#line 205 "src/movecheck.tr"
            ({ TrStr _ap_t1050 = (_tr_str_lit_len(", into ", 7LL)); TrStr _ap_t1051 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(cx->msite, _tr_strz(nm))))); _tr_str_append(&at, _ap_t1050); _tr_str_append(&at, _ap_t1051); _tr_str_release(_ap_t1051); });
        }
#line 206 "src/movecheck.tr"
        ({ TrStr _ap_t1052 = (_tr_str_lit_len(")", 1LL)); _tr_str_append(&at, _ap_t1052); });
    }
#line 207 "src/movecheck.tr"
    if (mv_has(st->all, nm)) {
#line 208 "src/movecheck.tr"
        ({ TrStr _at_t1053 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was moved", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and cannot be used again (--strict: passing a value by value consumes it).\n      FIX: pass a copy to the call that consumes it (`", 130LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".clone()`; for a `str` use `str(", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")`), pass a `ref` borrow, or stop using '", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after the move; assign it a new value to re-initialise it.", 60LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1053); _tr_str_release(_at_t1053); });
    } else if (mv_has(st->lp, nm)) {
#line 210 "src/movecheck.tr"
        ({ TrStr _at_t1054 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is moved inside a loop", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and used again by the next iteration: it was already moved in the previous one.\n      FIX: move a `clone()` of it, give '", 122LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' a new value before the end of the loop body, or `break` right after the move.", 79LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1054); _tr_str_release(_at_t1054); });
    } else {
#line 212 "src/movecheck.tr"
        ({ TrStr _at_t1055 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-5] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' may have been moved", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" on some code paths, making this use unsafe (--strict).\n      FIX: do not move '", 80LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' on any branch before this point (move a `clone()`), or restructure so the use is inside the branch where it is still valid.", 125LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1055); _tr_str_release(_at_t1055); });
    }
    _tr_str_release(at);
}

__attribute__((hot)) void mv_mark(MvCtx* cx, TrStr nm, MvState* st) {
#line 215 "src/movecheck.tr"
    if (((st->dead || (cx->unsafe_depth > 0LL)) || cx->in_defer)) {
#line 215 "src/movecheck.tr"
        return;
    }
#line 216 "src/movecheck.tr"
    if (mv_has(cx->defers, nm)) {
#line 217 "src/movecheck.tr"
        ({ TrStr _at_t1056 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is read by a `defer` statement of this function and is moved here; the deferred statement would run after its new owner released it.\n      FIX: move a `clone()` instead, or do not move '", 188LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after deferring a use of it.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1056); _tr_str_release(_at_t1056); });
    }
#line 218 "src/movecheck.tr"
    mv_add(st->any, nm);
#line 219 "src/movecheck.tr"
    mv_add(st->all, nm);
#line 220 "src/movecheck.tr"
    _tr_dict_set(cx->mline, _tr_strz(nm), cx->line);
#line 221 "src/movecheck.tr"
    _tr_dict_set_str(cx->msite, _tr_strz(nm), _tr_str_box(_tr_str_retain(cx->site)));
#line 222 "src/movecheck.tr"
    if ((cx->try_depth > 0LL)) {
#line 222 "src/movecheck.tr"
        mv_add(cx->try_acc, nm);
    }
}

__attribute__((hot)) void mv_reinit(MvCtx* cx, TrStr nm, MvState* st) {
#line 225 "src/movecheck.tr"
    st->any = mv_without(st->any, nm);
#line 226 "src/movecheck.tr"
    st->all = mv_without(st->all, nm);
#line 227 "src/movecheck.tr"
    st->lp = mv_without(st->lp, nm);
#line 228 "src/movecheck.tr"
    if (_tr_dict_contains(cx->clcaps, _tr_strz(nm))) {
#line 228 "src/movecheck.tr"
        _tr_dict_remove(cx->clcaps, _tr_strz(nm));
    }
}

__attribute__((hot)) void mv_declare(MvCtx* cx, TrStr nm, MvState* st) {
#line 231 "src/movecheck.tr"
    mv_add(cx->declared, nm);
#line 232 "src/movecheck.tr"
    mv_reinit(cx, nm, st);
}

__attribute__((hot)) void mv_idents(HirExpr* e, List_TrStr* out) {
#line 237 "src/movecheck.tr"
    if (mv_null(e)) {
#line 237 "src/movecheck.tr"
        return;
    }
#line 238 "src/movecheck.tr"
    __auto_type _t1057 = (*e);
    if (_t1057.tag == HirExpr_EIdent) {
        __auto_type nm = _t1057.data.EIdent.name;
        mv_add(out, nm);
    } else if (_t1057.tag == HirExpr_EBinOp) {
        __auto_type l = _t1057.data.EBinOp.left;
__auto_type r = _t1057.data.EBinOp.right;
#line 241 "src/movecheck.tr"
        mv_idents(l, out);
#line 242 "src/movecheck.tr"
        mv_idents(r, out);
    } else if (_t1057.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t1057.data.EUnaryOp.expr;
        mv_idents(x, out);
    } else if (_t1057.tag == HirExpr_ECall) {
        __auto_type c = _t1057.data.ECall.callee;
__auto_type args = _t1057.data.ECall.args;
#line 245 "src/movecheck.tr"
        mv_idents(c, out);
#line 246 "src/movecheck.tr"
        long long i = 0LL;
#line 247 "src/movecheck.tr"
        while ((i < args->len)) {
#line 248 "src/movecheck.tr"
            mv_idents(((HirExpr*)List_ptr_get(args, i)), out);
#line 249 "src/movecheck.tr"
            i = (i + 1LL);
        }
    } else if (_t1057.tag == HirExpr_EMethodCall) {
        __auto_type o = _t1057.data.EMethodCall.obj;
__auto_type args = _t1057.data.EMethodCall.args;
#line 251 "src/movecheck.tr"
        mv_idents(o, out);
#line 252 "src/movecheck.tr"
        long long j = 0LL;
#line 253 "src/movecheck.tr"
        while ((j < args->len)) {
#line 254 "src/movecheck.tr"
            mv_idents(((HirExpr*)List_ptr_get(args, j)), out);
#line 255 "src/movecheck.tr"
            j = (j + 1LL);
        }
    } else if (_t1057.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1057.data.EPropAccess.obj;
        mv_idents(o, out);
    } else if (_t1057.tag == HirExpr_EIndex) {
        __auto_type o = _t1057.data.EIndex.obj;
__auto_type i2 = _t1057.data.EIndex._tr_v_index;
#line 258 "src/movecheck.tr"
        mv_idents(o, out);
#line 259 "src/movecheck.tr"
        mv_idents(i2, out);
    } else if (_t1057.tag == HirExpr_ECast) {
        __auto_type x = _t1057.data.ECast.expr;
        mv_idents(x, out);
    } else if (_t1057.tag == HirExpr_EFString) {
        __auto_type parts = _t1057.data.EFString.parts;
#line 262 "src/movecheck.tr"
        long long k = 0LL;
#line 263 "src/movecheck.tr"
        while ((k < parts->len)) {
#line 264 "src/movecheck.tr"
            if (((HirFStringPart*)List_ptr_get(parts, k))->is_expr) {
#line 264 "src/movecheck.tr"
                mv_idents(((HirFStringPart*)List_ptr_get(parts, k))->expr, out);
            }
#line 265 "src/movecheck.tr"
            k = (k + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1057;
        /* pass */
    }
}

__attribute__((hot)) TrStr mv_strip(TrStr n) {
#line 271 "src/movecheck.tr"
    return _tr_str_retain(n);
}

__attribute__((hot)) List_i64* mv_all_mask(long long n, long long v) {
#line 275 "src/movecheck.tr"
    List_i64* m = (void*)List_i64_new();
#line 276 "src/movecheck.tr"
    long long i = 0LL;
#line 277 "src/movecheck.tr"
    while ((i < n)) {
#line 278 "src/movecheck.tr"
        List_i64_append(m, v);
#line 279 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 280 "src/movecheck.tr"
    return m;
}

__attribute__((hot)) List_i64* mv_sig_mask(MvCtx* cx, TrStr key, long long n, bool skip_self) {
#line 283 "src/movecheck.tr"
    if ((_tr_dict_contains(cx->amb, _tr_strz(key)) || (!_tr_dict_contains(cx->sigs, _tr_strz(key))))) {
#line 283 "src/movecheck.tr"
        return mv_all_mask(n, 0LL);
    }
#line 284 "src/movecheck.tr"
    MvSig* sg = ((MvSig*)(uintptr_t)_tr_dict_get(cx->sigs, _tr_strz(key)));
#line 285 "src/movecheck.tr"
    HirFunction* f = _tr_obj_retain(sg->f);
#line 286 "src/movecheck.tr"
    if (f->is_extern) {
#line 286 "src/movecheck.tr"
        _tr_obj_release(f, _trdrop_HirFunction);
        return mv_all_mask(n, 0LL);
    }
#line 287 "src/movecheck.tr"
    long long off = 0LL;
#line 288 "src/movecheck.tr"
    if (((skip_self && (f->params->len > 0LL)) && _tr_str_eqv((((HirParam*)List_ptr_get(f->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 289 "src/movecheck.tr"
        off = 1LL;
    }
#line 290 "src/movecheck.tr"
    long long i = 0LL;
#line 291 "src/movecheck.tr"
    List_i64* out = (void*)List_i64_new();
#line 292 "src/movecheck.tr"
    while ((i < n)) {
#line 293 "src/movecheck.tr"
        long long pi = (i + off);
#line 294 "src/movecheck.tr"
        long long v = 0LL;
#line 295 "src/movecheck.tr"
        if ((pi < f->params->len)) {
#line 296 "src/movecheck.tr"
            AstType* pt = ((HirParam*)List_ptr_get(f->params, pi))->ty;
#line 297 "src/movecheck.tr"
            if (((!_tr_str_eqv((((HirParam*)List_ptr_get(f->params, pi))->name), (_tr_str_lit_len("self", 4LL)))) && (!pt->is_borrow))) {
#line 298 "src/movecheck.tr"
                bool generic = ({ __auto_type _ord_t1059 = (sg->gens); TrStr _apt_t1058 = _tr_str_retain(pt->name); __auto_type _wr = (mv_has(_ord_t1059, _apt_t1058)); _tr_str_release(_apt_t1058); _wr; });
#line 299 "src/movecheck.tr"
                if (generic) {
#line 300 "src/movecheck.tr"
                    v = 2LL;
                } else if ((!sg->lib)) {
#line 302 "src/movecheck.tr"
                    v = 1LL;
                }
            }
        }
#line 303 "src/movecheck.tr"
        List_i64_append(out, v);
#line 304 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 305 "src/movecheck.tr"
    _tr_obj_release(f, _trdrop_HirFunction);
    return out;
}

__attribute__((hot)) bool mv_coll_type(TrStr t) {
#line 308 "src/movecheck.tr"
    return (((((((((_tr_str_eqv((t), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((t), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Deque", 5LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Heap", 4LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Queue", 5LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Stack", 5LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("PriorityQueue", 13LL))));
}

__attribute__((hot)) bool mv_store_method(TrStr m) {
#line 311 "src/movecheck.tr"
    return (((((((((_tr_str_eqv((m), (_tr_str_lit_len("append", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_back", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_front", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("enqueue", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("appendleft", 10LL))));
}

__attribute__((hot)) bool mv_ctor_type(TrStr t) {
#line 314 "src/movecheck.tr"
    return (_tr_str_eqv((t), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((t), (_tr_str_lit_len("Box", 3LL))));
}

__attribute__((hot)) TrStr mv_callee_name(HirExpr* c) {
#line 317 "src/movecheck.tr"
    if (mv_null(c)) {
#line 317 "src/movecheck.tr"
        return _tr_str_lit_len("?", 1LL);
    }
#line 318 "src/movecheck.tr"
    __auto_type _t1060 = (*c);
    if (_t1060.tag == HirExpr_EIdent) {
        __auto_type n = _t1060.data.EIdent.name;
        TrStr _trret202 = _tr_str_retain(n);
        return _trret202;
    } else if (_t1060.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1060.data.EPropAccess.obj;
__auto_type p = _t1060.data.EPropAccess.prop;
#line 321 "src/movecheck.tr"
        if (mv_null(o)) {
#line 321 "src/movecheck.tr"
            TrStr _trret203 = _tr_str_retain(p);
            return _trret203;
        }
#line 322 "src/movecheck.tr"
        __auto_type _t1061 = (*o);
        if (_t1061.tag == HirExpr_EIdent) {
            __auto_type on = _t1061.data.EIdent.name;
            TrStr _trret204 = ({ TrStr _tr_cl = (_tr_strx_concatv((on), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
            return _trret204;
        } else if (_t1061.tag == HirExpr_EIndex) {
            __auto_type b = _t1061.data.EIndex.obj;
#line 325 "src/movecheck.tr"
            __auto_type _t1062 = (*b);
            if (_t1062.tag == HirExpr_EIdent) {
                __auto_type bn = _t1062.data.EIdent.name;
                TrStr _trret205 = ({ TrStr _tr_cl = (_tr_strx_concatv((bn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
                return _trret205;
            } else if (1) {
                __auto_type _ = _t1062;
                TrStr _trret206 = _tr_str_retain(p);
                return _trret206;
            }
        } else if (1) {
            __auto_type _ = _t1061;
            TrStr _trret207 = _tr_str_retain(p);
            return _trret207;
        }
    } else if (1) {
        __auto_type _ = _t1060;
        return _tr_str_lit_len("?", 1LL);
    }
#line 330 "src/movecheck.tr"
    return _tr_str_lit_len("?", 1LL);
}

__attribute__((hot)) TrStr mv_static_base(MvCtx* cx, HirExpr* o) {
#line 333 "src/movecheck.tr"
    if (mv_null(o)) {
#line 333 "src/movecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 334 "src/movecheck.tr"
    __auto_type _t1063 = (*o);
    if (_t1063.tag == HirExpr_EIdent) {
        __auto_type nm = _t1063.data.EIdent.name;
#line 336 "src/movecheck.tr"
        if (mv_has(cx->declared, nm)) {
#line 336 "src/movecheck.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 337 "src/movecheck.tr"
        TrStr _trret208 = _tr_str_retain(nm);
        return _trret208;
    } else if (_t1063.tag == HirExpr_EIndex) {
        __auto_type b = _t1063.data.EIndex.obj;
#line 339 "src/movecheck.tr"
        return mv_static_base(cx, b);
    } else if (_t1063.tag == HirExpr_EMethodCall) {
        __auto_type b2 = _t1063.data.EMethodCall.obj;
__auto_type mm = _t1063.data.EMethodCall.method;
#line 341 "src/movecheck.tr"
        if (_tr_str_eqv((mm), (_tr_str_lit_len("get_index", 9LL)))) {
#line 341 "src/movecheck.tr"
            TrStr _trret209 = mv_static_base(cx, b2);
            return _trret209;
        }
#line 342 "src/movecheck.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t1063;
        return _tr_str_lit_len("", 0LL);
    }
#line 344 "src/movecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) List_i64* mv_mask_static(MvCtx* cx, TrStr tn, TrStr v, long long n) {
#line 347 "src/movecheck.tr"
    if (({ TrStr _dkt_t1064 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(cx->sigs, _tr_strz(_dkt_t1064))); _tr_str_release(_dkt_t1064); _wr; })) {
#line 347 "src/movecheck.tr"
        return ({ TrStr _at_t1065 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (mv_sig_mask(cx, _at_t1065, n, false)); _tr_str_release(_at_t1065); _wr; });
    }
#line 348 "src/movecheck.tr"
    if ((_tr_dict_contains(cx->old, _tr_strz(tn)) && ((bool)(uintptr_t)_tr_dict_get(cx->old, _tr_strz(tn))))) {
#line 349 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 350 "src/movecheck.tr"
    if (((_tr_str_eqv((tn), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("ThreadPool", 10LL)))) && (((_tr_str_eqv((v), (_tr_str_lit_len("spawn", 5LL))) || _tr_str_eqv((v), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((v), (_tr_str_lit_len("submit", 6LL)))) || _tr_str_eqv((v), (_tr_str_lit_len("execute", 7LL)))))) {
#line 350 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 351 "src/movecheck.tr"
    if ((mv_ctor_type(tn) && (_tr_str_eqv((v), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((v), (_tr_str_lit_len("new", 3LL)))))) {
#line 351 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 352 "src/movecheck.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL))))) {
#line 352 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 353 "src/movecheck.tr"
    return mv_all_mask(n, 0LL);
}

__attribute__((hot)) List_i64* mv_mask_method(MvCtx* cx, TrStr rt, TrStr m, long long n) {
#line 356 "src/movecheck.tr"
    if (({ TrStr _dkt_t1066 = (({ TrStr _tr_cl = (_tr_strx_concatv((rt), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(cx->sigs, _tr_strz(_dkt_t1066))); _tr_str_release(_dkt_t1066); _wr; })) {
#line 356 "src/movecheck.tr"
        return ({ TrStr _at_t1067 = (({ TrStr _tr_cl = (_tr_strx_concatv((rt), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (mv_sig_mask(cx, _at_t1067, n, true)); _tr_str_release(_at_t1067); _wr; });
    }
#line 357 "src/movecheck.tr"
    if ((mv_coll_type(rt) && mv_store_method(m))) {
#line 357 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 358 "src/movecheck.tr"
    if (((_tr_str_eqv((rt), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((rt), (_tr_str_lit_len("Channel", 7LL)))) && ((_tr_str_eqv((m), (_tr_str_lit_len("send", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("try_send", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("send_timeout", 12LL)))))) {
#line 359 "src/movecheck.tr"
        List_i64* mm = mv_all_mask(n, 0LL);
#line 360 "src/movecheck.tr"
        if ((n > 0LL)) {
#line 360 "src/movecheck.tr"
            List_i64_set(mm, 0LL, 2LL);
        }
#line 361 "src/movecheck.tr"
        return mm;
    }
#line 362 "src/movecheck.tr"
    if (((_tr_str_eqv((rt), (_tr_str_lit_len("ThreadPool", 10LL))) || _tr_str_eqv((rt), (_tr_str_lit_len("Thread", 6LL)))) && ((_tr_str_eqv((m), (_tr_str_lit_len("submit", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("execute", 7LL)))))) {
#line 362 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 363 "src/movecheck.tr"
    return mv_all_mask(n, 0LL);
}

__attribute__((hot)) void mv_block(MvCtx* cx, HirBlock* b, MvState* st) {
#line 368 "src/movecheck.tr"
    List_i64_append(cx->sh_base, cx->sh_name->len);
#line 369 "src/movecheck.tr"
    long long i = 0LL;
#line 370 "src/movecheck.tr"
    while ((i < b->stmts->len)) {
#line 371 "src/movecheck.tr"
        mv_stmt(cx, ((HirStmt*)List_ptr_get(b->stmts, i)), st);
#line 372 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 374 "src/movecheck.tr"
    long long base = List_i64_get(cx->sh_base, (cx->sh_base->len - 1LL));
#line 375 "src/movecheck.tr"
    List_i64_pop(cx->sh_base);
#line 376 "src/movecheck.tr"
    while ((cx->sh_name->len > base)) {
#line 377 "src/movecheck.tr"
        long long k = (cx->sh_name->len - 1LL);
#line 378 "src/movecheck.tr"
        TrStr nm = List_TrStr_get(cx->sh_name, k);
#line 379 "src/movecheck.tr"
        long long bits = List_i64_get(cx->sh_bits, k);
#line 380 "src/movecheck.tr"
        List_TrStr_pop(cx->sh_name);
#line 381 "src/movecheck.tr"
        List_i64_pop(cx->sh_bits);
#line 382 "src/movecheck.tr"
        st->any = mv_without(st->any, nm);
#line 383 "src/movecheck.tr"
        st->all = mv_without(st->all, nm);
#line 384 "src/movecheck.tr"
        st->lp = mv_without(st->lp, nm);
#line 385 "src/movecheck.tr"
        if ((_tr_imod((long long)(bits), (long long)(2LL)) == 1LL)) {
#line 385 "src/movecheck.tr"
            List_TrStr_append(st->any, nm);
        }
#line 386 "src/movecheck.tr"
        if ((_tr_imod((long long)(_tr_idiv((long long)(bits), (long long)(2LL))), (long long)(2LL)) == 1LL)) {
#line 386 "src/movecheck.tr"
            List_TrStr_append(st->all, nm);
        }
#line 387 "src/movecheck.tr"
        if ((_tr_imod((long long)(_tr_idiv((long long)(bits), (long long)(4LL))), (long long)(2LL)) == 1LL)) {
#line 387 "src/movecheck.tr"
            List_TrStr_append(st->lp, nm);
        }
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void mv_exprs(MvCtx* cx, List_ptr* v, MvState* st) {
#line 390 "src/movecheck.tr"
    long long i = 0LL;
#line 391 "src/movecheck.tr"
    while ((i < v->len)) {
#line 392 "src/movecheck.tr"
        mv_expr(cx, ((HirExpr*)List_ptr_get(v, i)), st);
#line 393 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_branchy_block(MvCtx* cx, HirBlock* b, MvState* st) {
#line 396 "src/movecheck.tr"
    MvState* c = MvState_copy(st);
#line 397 "src/movecheck.tr"
    mv_block(cx, b, c);
#line 398 "src/movecheck.tr"
    mv_merge(st, c);
    _tr_obj_release(c, _trdrop_MvState);
}

__attribute__((hot)) void mv_consume_caps(MvCtx* cx, List_TrStr* caps, MvState* st) {
#line 402 "src/movecheck.tr"
    long long i = 0LL;
#line 403 "src/movecheck.tr"
    while ((i < caps->len)) {
#line 404 "src/movecheck.tr"
        ({ TrStr _at_t1068 = (List_TrStr_get(caps, i)); mv_use(cx, _at_t1068, st); _tr_str_release(_at_t1068); });
#line 405 "src/movecheck.tr"
        ({ TrStr _at_t1069 = (List_TrStr_get(caps, i)); mv_mark(cx, _at_t1069, st); _tr_str_release(_at_t1069); });
#line 406 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_TrStr* mv_closure_caps(MvCtx* cx, List_ptr* caps) {
#line 409 "src/movecheck.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 410 "src/movecheck.tr"
    long long i = 0LL;
#line 411 "src/movecheck.tr"
    while ((i < caps->len)) {
#line 412 "src/movecheck.tr"
        TrStr nm = _tr_str_retain(((HirParam*)List_ptr_get(caps, i))->name);
#line 413 "src/movecheck.tr"
        if ((mv_tracked(cx, nm) && mv_is_move(cx, ((HirParam*)List_ptr_get(caps, i))->ty))) {
#line 413 "src/movecheck.tr"
            List_TrStr_append(out, nm);
        }
#line 414 "src/movecheck.tr"
        i = (i + 1LL);
        _tr_str_release(nm);
    }
#line 415 "src/movecheck.tr"
    return out;
}

__attribute__((hot)) void mv_consume(MvCtx* cx, HirExpr* e, MvState* st, long long mode) {
#line 420 "src/movecheck.tr"
    if (mv_null(e)) {
#line 420 "src/movecheck.tr"
        return;
    }
#line 421 "src/movecheck.tr"
    __auto_type _t1070 = (*e);
    if (_t1070.tag == HirExpr_EIdent) {
        __auto_type nm = _t1070.data.EIdent.name;
__auto_type ty = _t1070.data.EIdent.ty;
#line 423 "src/movecheck.tr"
        mv_use(cx, nm, st);
#line 424 "src/movecheck.tr"
        if (mv_tracked(cx, nm)) {
#line 425 "src/movecheck.tr"
            if (mv_is_move(cx, ty)) {
#line 425 "src/movecheck.tr"
                mv_mark(cx, nm, st);
            } else if (((mode == 2LL) && _tr_dict_contains(cx->clcaps, _tr_strz(nm)))) {
#line 426 "src/movecheck.tr"
                mv_consume_caps(cx, ((List_TrStr*)(uintptr_t)_tr_dict_get(cx->clcaps, _tr_strz(nm))), st);
            }
        }
    } else if (_t1070.tag == HirExpr_EClosure) {
        __auto_type caps = _t1070.data.EClosure.captures;
#line 428 "src/movecheck.tr"
        mv_expr(cx, e, st);
#line 429 "src/movecheck.tr"
        if ((mode == 2LL)) {
#line 429 "src/movecheck.tr"
            mv_consume_caps(cx, mv_closure_caps(cx, caps), st);
        }
    } else if (_t1070.tag == HirExpr_ETuple) {
        __auto_type items = _t1070.data.ETuple.items;
#line 431 "src/movecheck.tr"
        long long i = 0LL;
#line 432 "src/movecheck.tr"
        while ((i < items->len)) {
#line 433 "src/movecheck.tr"
            mv_consume(cx, ((HirExpr*)List_ptr_get(items, i)), st, 2LL);
#line 434 "src/movecheck.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1070;
        mv_expr(cx, e, st);
    }
}

__attribute__((hot)) void mv_args(MvCtx* cx, List_ptr* args, List_i64* mask, MvState* st, TrStr cname) {
#line 438 "src/movecheck.tr"
    long long i = 0LL;
#line 439 "src/movecheck.tr"
    while ((i < args->len)) {
#line 440 "src/movecheck.tr"
        long long k = 0LL;
#line 441 "src/movecheck.tr"
        if ((i < mask->len)) {
#line 441 "src/movecheck.tr"
            k = List_i64_get(mask, i);
        }
#line 442 "src/movecheck.tr"
        if ((k == 0LL)) {
#line 442 "src/movecheck.tr"
            mv_expr(cx, ((HirExpr*)List_ptr_get(args, i)), st);
        } else {
#line 444 "src/movecheck.tr"
            TrStr _sfo_t1071 = cx->site;
            cx->site = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((i + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(_sfo_t1071);
#line 445 "src/movecheck.tr"
            mv_consume(cx, ((HirExpr*)List_ptr_get(args, i)), st, k);
        }
#line 446 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_items(MvCtx* cx, List_ptr* items, MvState* st) {
#line 449 "src/movecheck.tr"
    TrStr _sfo_t1072 = cx->site;
    cx->site = _tr_str_lit_len("a list/tuple/dict/set literal", 29LL);
    _tr_str_release(_sfo_t1072);
#line 450 "src/movecheck.tr"
    long long i = 0LL;
#line 451 "src/movecheck.tr"
    while ((i < items->len)) {
#line 452 "src/movecheck.tr"
        mv_consume(cx, ((HirExpr*)List_ptr_get(items, i)), st, 2LL);
#line 453 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_expr(MvCtx* cx, HirExpr* e, MvState* st) {
#line 456 "src/movecheck.tr"
    if (mv_null(e)) {
#line 456 "src/movecheck.tr"
        return;
    }
#line 457 "src/movecheck.tr"
    if (st->dead) {
#line 457 "src/movecheck.tr"
        return;
    }
#line 458 "src/movecheck.tr"
    __auto_type _t1073 = (*e);
    if (_t1073.tag == HirExpr_EIdent) {
        __auto_type nm = _t1073.data.EIdent.name;
        mv_use(cx, nm, st);
    } else if (_t1073.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1073.data.EPropAccess.obj;
        mv_expr(cx, o, st);
    } else if (_t1073.tag == HirExpr_EBinOp) {
        __auto_type bop = _t1073.data.EBinOp.op;
__auto_type l = _t1073.data.EBinOp.left;
__auto_type r = _t1073.data.EBinOp.right;
#line 462 "src/movecheck.tr"
        mv_expr(cx, l, st);
#line 463 "src/movecheck.tr"
        if ((((_tr_str_eqv((bop), (_tr_str_lit_len("and", 3LL))) || _tr_str_eqv((bop), (_tr_str_lit_len("or", 2LL)))) || _tr_str_eqv((bop), (_tr_str_lit_len("&&", 2LL)))) || _tr_str_eqv((bop), (_tr_str_lit_len("||", 2LL))))) {
#line 465 "src/movecheck.tr"
            MvState* rs = MvState_copy(st);
#line 466 "src/movecheck.tr"
            mv_expr(cx, r, rs);
#line 467 "src/movecheck.tr"
            mv_merge(st, rs);
        } else {
#line 469 "src/movecheck.tr"
            mv_expr(cx, r, st);
        }
    } else if (_t1073.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t1073.data.EUnaryOp.expr;
        mv_expr(cx, x, st);
    } else if (_t1073.tag == HirExpr_ECall) {
        __auto_type callee = _t1073.data.ECall.callee;
__auto_type args = _t1073.data.ECall.args;
#line 472 "src/movecheck.tr"
        List_i64* mask = mv_all_mask(args->len, 0LL);
#line 473 "src/movecheck.tr"
        TrStr cname = mv_callee_name(callee);
#line 474 "src/movecheck.tr"
        if ((!mv_null(callee))) {
#line 475 "src/movecheck.tr"
            __auto_type _t1074 = (*callee);
            if (_t1074.tag == HirExpr_EIdent) {
                __auto_type fnm = _t1074.data.EIdent.name;
#line 477 "src/movecheck.tr"
                if ((((_tr_str_eqv((fnm), (_tr_str_lit_len("Some", 4LL))) || _tr_str_eqv((fnm), (_tr_str_lit_len("Ok", 2LL)))) || _tr_str_eqv((fnm), (_tr_str_lit_len("Err", 3LL)))) || _tr_str_eqv((fnm), (_tr_str_lit_len("spawn", 5LL))))) {
#line 478 "src/movecheck.tr"
                    mask = mv_all_mask(args->len, 2LL);
                } else if (mv_has(cx->declared, fnm)) {
#line 480 "src/movecheck.tr"
                    mv_use(cx, fnm, st);
                } else if (_tr_dict_contains(cx->sigs, _tr_strz(fnm))) {
#line 482 "src/movecheck.tr"
                    mask = mv_sig_mask(cx, fnm, args->len, false);
                } else if ((_tr_dict_contains(cx->mv, _tr_strz(fnm)) && _tr_dict_contains(cx->old, _tr_strz(fnm)))) {
#line 484 "src/movecheck.tr"
                    if (({ TrStr _dkt_t1075 = (_tr_strx_concatv((fnm), (_tr_str_lit_len(".init", 5LL)))); __auto_type _wr = (_tr_dict_contains(cx->sigs, _tr_strz(_dkt_t1075))); _tr_str_release(_dkt_t1075); _wr; })) {
#line 484 "src/movecheck.tr"
                        mask = ({ TrStr _at_t1076 = (_tr_strx_concatv((fnm), (_tr_str_lit_len(".init", 5LL)))); __auto_type _wr = (mv_sig_mask(cx, _at_t1076, args->len, false)); _tr_str_release(_at_t1076); _wr; });
                    } else {
#line 485 "src/movecheck.tr"
                        mask = mv_all_mask(args->len, 2LL);
                    }
                }
            } else if (_t1074.tag == HirExpr_EPropAccess) {
                __auto_type co = _t1074.data.EPropAccess.obj;
__auto_type cv = _t1074.data.EPropAccess.prop;
#line 487 "src/movecheck.tr"
                TrStr sb = mv_static_base(cx, co);
#line 488 "src/movecheck.tr"
                if ((!_tr_str_eqv((sb), (_tr_str_lit_len("", 0LL))))) {
#line 488 "src/movecheck.tr"
                    mask = mv_mask_static(cx, sb, cv, args->len);
                } else {
#line 490 "src/movecheck.tr"
                    mv_expr(cx, callee, st);
#line 491 "src/movecheck.tr"
                    mask = ({ TrStr _apt_t1077 = _tr_str_retain(hir_expr_type(co)->name); __auto_type _wr = (mv_mask_method(cx, _apt_t1077, cv, args->len)); _tr_str_release(_apt_t1077); _wr; });
                }
                _tr_str_release(sb);
            } else if (1) {
                __auto_type _ = _t1074;
                mv_expr(cx, callee, st);
            }
        }
#line 493 "src/movecheck.tr"
        mv_args(cx, args, mask, st, cname);
        _tr_str_release(cname);
    } else if (_t1073.tag == HirExpr_EMethodCall) {
        __auto_type o = _t1073.data.EMethodCall.obj;
__auto_type m = _t1073.data.EMethodCall.method;
__auto_type args = _t1073.data.EMethodCall.args;
#line 495 "src/movecheck.tr"
        TrStr cname2 = _tr_str_retain(m);
#line 496 "src/movecheck.tr"
        TrStr sb2 = mv_static_base(cx, o);
#line 497 "src/movecheck.tr"
        List_i64* mask2 = mv_all_mask(args->len, 0LL);
#line 498 "src/movecheck.tr"
        if (((!_tr_str_eqv((sb2), (_tr_str_lit_len("", 0LL)))) && (((((_tr_dict_contains(cx->mv, _tr_strz(sb2)) || mv_ctor_type(sb2)) || _tr_str_eqv((sb2), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((sb2), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((sb2), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((sb2), (_tr_str_lit_len("Result", 6LL)))))) {
#line 499 "src/movecheck.tr"
            mask2 = mv_mask_static(cx, sb2, m, args->len);
        } else {
#line 501 "src/movecheck.tr"
            mv_expr(cx, o, st);
#line 502 "src/movecheck.tr"
            TrStr rt = _tr_str_retain(hir_expr_type(o)->name);
#line 503 "src/movecheck.tr"
            mask2 = mv_mask_method(cx, rt, m, args->len);
        }
#line 504 "src/movecheck.tr"
        mv_args(cx, args, mask2, st, cname2);
        _tr_str_release(cname2);
        _tr_str_release(sb2);
    } else if (_t1073.tag == HirExpr_EIndex) {
        __auto_type o = _t1073.data.EIndex.obj;
__auto_type i = _t1073.data.EIndex._tr_v_index;
#line 506 "src/movecheck.tr"
        mv_expr(cx, o, st);
#line 507 "src/movecheck.tr"
        mv_expr(cx, i, st);
    } else if (_t1073.tag == HirExpr_ECast) {
        __auto_type x = _t1073.data.ECast.expr;
        mv_expr(cx, x, st);
    } else if (_t1073.tag == HirExpr_EFString) {
        __auto_type parts = _t1073.data.EFString.parts;
#line 510 "src/movecheck.tr"
        long long pi = 0LL;
#line 511 "src/movecheck.tr"
        while ((pi < parts->len)) {
#line 512 "src/movecheck.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, pi));
#line 513 "src/movecheck.tr"
            if (part->is_expr) {
#line 514 "src/movecheck.tr"
                mv_expr(cx, part->expr, st);
#line 515 "src/movecheck.tr"
                mv_exprs(cx, part->spec_args, st);
            }
#line 516 "src/movecheck.tr"
            pi = (pi + 1LL);
        }
    } else if (_t1073.tag == HirExpr_ETryExpr) {
        __auto_type x = _t1073.data.ETryExpr.expr;
        mv_expr(cx, x, st);
    } else if (_t1073.tag == HirExpr_EClosure) {
        __auto_type caps = _t1073.data.EClosure.captures;
#line 519 "src/movecheck.tr"
        long long ci = 0LL;
#line 520 "src/movecheck.tr"
        while ((ci < caps->len)) {
#line 521 "src/movecheck.tr"
            ({ TrStr _apt_t1078 = _tr_str_retain(((HirParam*)List_ptr_get(caps, ci))->name); mv_use(cx, _apt_t1078, st); _tr_str_release(_apt_t1078); });
#line 522 "src/movecheck.tr"
            ci = (ci + 1LL);
        }
    } else if (_t1073.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t1073.data.ESuperMethodCall.args;
        mv_exprs(cx, args, st);
    } else if (_t1073.tag == HirExpr_EList) {
        __auto_type items = _t1073.data.EList.items;
        mv_items(cx, items, st);
    } else if (_t1073.tag == HirExpr_ESet) {
        __auto_type items = _t1073.data.ESet.items;
        mv_items(cx, items, st);
    } else if (_t1073.tag == HirExpr_ETuple) {
        __auto_type items = _t1073.data.ETuple.items;
        mv_items(cx, items, st);
    } else if (_t1073.tag == HirExpr_EDict) {
        __auto_type keys = _t1073.data.EDict.keys;
__auto_type vals = _t1073.data.EDict.vals;
#line 528 "src/movecheck.tr"
        mv_items(cx, keys, st);
#line 529 "src/movecheck.tr"
        mv_items(cx, vals, st);
    } else if (_t1073.tag == HirExpr_EListComp) {
        __auto_type el = _t1073.data.EListComp.element;
__auto_type gens = _t1073.data.EListComp.generators;
#line 531 "src/movecheck.tr"
        long long gi = 0LL;
#line 532 "src/movecheck.tr"
        while ((gi < gens->len)) {
#line 533 "src/movecheck.tr"
            mv_expr(cx, (*((HirComprehension**)List_ptr_get(gens, gi)))->iter, st);
#line 534 "src/movecheck.tr"
            gi = (gi + 1LL);
        }
    } else if (_t1073.tag == HirExpr_EGeneratorExpr) {
        __auto_type el2 = _t1073.data.EGeneratorExpr.element;
__auto_type gens2 = _t1073.data.EGeneratorExpr.generators;
#line 536 "src/movecheck.tr"
        long long gj = 0LL;
#line 537 "src/movecheck.tr"
        while ((gj < gens2->len)) {
#line 538 "src/movecheck.tr"
            mv_expr(cx, (*((HirComprehension**)List_ptr_get(gens2, gj)))->iter, st);
#line 539 "src/movecheck.tr"
            gj = (gj + 1LL);
        }
    } else if (_t1073.tag == HirExpr_ESlice) {
        __auto_type a = _t1073.data.ESlice.start;
__auto_type b = _t1073.data.ESlice.stop;
__auto_type c = _t1073.data.ESlice.step;
#line 541 "src/movecheck.tr"
        mv_expr(cx, a, st);
#line 542 "src/movecheck.tr"
        mv_expr(cx, b, st);
#line 543 "src/movecheck.tr"
        mv_expr(cx, c, st);
    } else if (_t1073.tag == HirExpr_EAwait) {
        __auto_type x = _t1073.data.EAwait.expr;
        mv_expr(cx, x, st);
    } else if (_t1073.tag == HirExpr_EAwaitTimeout) {
        __auto_type x = _t1073.data.EAwaitTimeout.expr;
__auto_type t = _t1073.data.EAwaitTimeout.timeout_ms;
#line 546 "src/movecheck.tr"
        mv_expr(cx, x, st);
#line 547 "src/movecheck.tr"
        mv_expr(cx, t, st);
    } else if (_t1073.tag == HirExpr_EYield) {
        __auto_type x = _t1073.data.EYield.expr;
        mv_expr(cx, x, st);
    } else if (_t1073.tag == HirExpr_ERange) {
        __auto_type a2 = _t1073.data.ERange.start;
__auto_type b2 = _t1073.data.ERange.end;
#line 550 "src/movecheck.tr"
        mv_expr(cx, a2, st);
#line 551 "src/movecheck.tr"
        mv_expr(cx, b2, st);
    } else if (_t1073.tag == HirExpr_EIfElse) {
        __auto_type c2 = _t1073.data.EIfElse.cond;
__auto_type t2 = _t1073.data.EIfElse.then_e;
__auto_type f2 = _t1073.data.EIfElse.else_e;
#line 553 "src/movecheck.tr"
        mv_expr(cx, c2, st);
#line 554 "src/movecheck.tr"
        MvState* s1 = MvState_copy(st);
#line 555 "src/movecheck.tr"
        MvState* s2 = MvState_copy(st);
#line 556 "src/movecheck.tr"
        mv_expr(cx, t2, s1);
#line 557 "src/movecheck.tr"
        mv_expr(cx, f2, s2);
#line 558 "src/movecheck.tr"
        s1->dead = false;
#line 559 "src/movecheck.tr"
        s2->dead = false;
#line 560 "src/movecheck.tr"
        mv_assign_from(st, s1);
#line 561 "src/movecheck.tr"
        mv_merge(st, s2);
        _tr_obj_release(s1, _trdrop_MvState);
        _tr_obj_release(s2, _trdrop_MvState);
    } else if (_t1073.tag == HirExpr_EDo) {
        __auto_type body = _t1073.data.EDo.body;
        mv_branchy_block(cx, body, st);
    } else if (_t1073.tag == HirExpr_ELoop) {
        __auto_type body2 = _t1073.data.ELoop.body;
        mv_branchy_block(cx, body2, st);
    } else if (_t1073.tag == HirExpr_EWhileExpr) {
        __auto_type c3 = _t1073.data.EWhileExpr.cond;
__auto_type body3 = _t1073.data.EWhileExpr.body;
__auto_type eb = _t1073.data.EWhileExpr.else_body;
#line 565 "src/movecheck.tr"
        mv_expr(cx, c3, st);
#line 566 "src/movecheck.tr"
        mv_branchy_block(cx, body3, st);
#line 567 "src/movecheck.tr"
        mv_branchy_block(cx, eb, st);
    } else if (_t1073.tag == HirExpr_EMatchExpr) {
        __auto_type subj = _t1073.data.EMatchExpr.subj;
__auto_type arms = _t1073.data.EMatchExpr.arms;
#line 569 "src/movecheck.tr"
        mv_expr(cx, subj, st);
#line 570 "src/movecheck.tr"
        bool first = true;
#line 571 "src/movecheck.tr"
        MvState* acc = MvState_init();
#line 572 "src/movecheck.tr"
        acc->dead = true;
#line 573 "src/movecheck.tr"
        long long ai = 0LL;
#line 574 "src/movecheck.tr"
        while ((ai < arms->len)) {
#line 575 "src/movecheck.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 576 "src/movecheck.tr"
            MvState* c4 = MvState_copy(st);
#line 577 "src/movecheck.tr"
            mv_pat(cx, a->pat, c4);
#line 578 "src/movecheck.tr"
            mv_expr(cx, a->guard, c4);
#line 579 "src/movecheck.tr"
            mv_block(cx, a->body, c4);
#line 580 "src/movecheck.tr"
            mv_merge(acc, c4);
#line 581 "src/movecheck.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c4, _trdrop_MvState);
        }
#line 582 "src/movecheck.tr"
        if ((arms->len > 0LL)) {
#line 582 "src/movecheck.tr"
            mv_assign_from(st, acc);
        }
        _tr_obj_release(acc, _trdrop_MvState);
    } else if (_t1073.tag == HirExpr_ETry) {
        __auto_type tb = _t1073.data.ETry.try_body;
__auto_type catches = _t1073.data.ETry.catches;
__auto_type fb = _t1073.data.ETry.finally_b;
#line 584 "src/movecheck.tr"
        mv_branchy_block(cx, tb, st);
#line 585 "src/movecheck.tr"
        long long ti = 0LL;
#line 586 "src/movecheck.tr"
        while ((ti < catches->len)) {
#line 587 "src/movecheck.tr"
            ({ TrStr _apt_t1079 = _tr_str_retain((*((HirCatchClause**)List_ptr_get(catches, ti)))->err_name); mv_declare(cx, _apt_t1079, st); _tr_str_release(_apt_t1079); });
#line 588 "src/movecheck.tr"
            mv_branchy_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ti)))->body, st);
#line 589 "src/movecheck.tr"
            ti = (ti + 1LL);
        }
#line 590 "src/movecheck.tr"
        mv_branchy_block(cx, fb, st);
    } else if (1) {
        __auto_type _ = _t1073;
        /* pass */
    }
}

__attribute__((hot)) void mv_pat_names(Pattern p, List_TrStr* out) {
#line 594 "src/movecheck.tr"
    __auto_type _t1080 = p;
    if (_t1080.tag == Pattern_PBind) {
        __auto_type n = _t1080.data.PBind.name;
        List_TrStr_append(out, n);
    } else if (_t1080.tag == Pattern_PVariantBind) {
        __auto_type f = _t1080.data.PVariantBind.field;
        List_TrStr_append(out, f);
    } else if (_t1080.tag == Pattern_PVariantBindMany) {
        __auto_type fs = _t1080.data.PVariantBindMany.fields;
#line 598 "src/movecheck.tr"
        long long i = 0LL;
#line 599 "src/movecheck.tr"
        while ((i < fs->len)) {
#line 600 "src/movecheck.tr"
            ({ TrStr _at_t1081 = (List_TrStr_get(fs, i)); List_TrStr_append(out, _at_t1081); _tr_str_release(_at_t1081); });
#line 601 "src/movecheck.tr"
            i = (i + 1LL);
        }
    } else if (_t1080.tag == Pattern_PTuple) {
        __auto_type a = _t1080.data.PTuple.first;
__auto_type b = _t1080.data.PTuple.second;
#line 603 "src/movecheck.tr"
        List_TrStr_append(out, a);
#line 604 "src/movecheck.tr"
        List_TrStr_append(out, b);
    } else if (_t1080.tag == Pattern_POr) {
        __auto_type ps = _t1080.data.POr.patterns;
#line 606 "src/movecheck.tr"
        long long j = 0LL;
#line 607 "src/movecheck.tr"
        while ((j < ps->len)) {
#line 608 "src/movecheck.tr"
            mv_pat_names(List_Pattern_get(ps, j), out);
#line 609 "src/movecheck.tr"
            j = (j + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1080;
        /* pass */
    }
}

__attribute__((hot)) void mv_pat(MvCtx* cx, Pattern p, MvState* st) {
#line 613 "src/movecheck.tr"
    List_TrStr* names = (void*)List_TrStr_new();
#line 614 "src/movecheck.tr"
    mv_pat_names(p, names);
#line 615 "src/movecheck.tr"
    long long i = 0LL;
#line 616 "src/movecheck.tr"
    while ((i < names->len)) {
#line 617 "src/movecheck.tr"
        ({ TrStr _at_t1082 = (List_TrStr_get(names, i)); mv_declare(cx, _at_t1082, st); _tr_str_release(_at_t1082); });
#line 618 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_exit_loop_ctx(MvCtx* cx, MvState* brk_out, MvState* cnt_out) {
#line 623 "src/movecheck.tr"
    long long bb = List_i64_get(cx->brk_base, (cx->brk_base->len - 1LL));
#line 624 "src/movecheck.tr"
    List_i64_pop(cx->brk_base);
#line 625 "src/movecheck.tr"
    while ((cx->brk->len > bb)) {
#line 626 "src/movecheck.tr"
        mv_merge(brk_out, ((MvState*)List_ptr_get(cx->brk, (cx->brk->len - 1LL))));
#line 627 "src/movecheck.tr"
        ((MvState*)List_ptr_pop(cx->brk));
    }
#line 628 "src/movecheck.tr"
    long long cb = List_i64_get(cx->cnt_base, (cx->cnt_base->len - 1LL));
#line 629 "src/movecheck.tr"
    List_i64_pop(cx->cnt_base);
#line 630 "src/movecheck.tr"
    while ((cx->cnt->len > cb)) {
#line 631 "src/movecheck.tr"
        mv_merge(cnt_out, ((MvState*)List_ptr_get(cx->cnt, (cx->cnt->len - 1LL))));
#line 632 "src/movecheck.tr"
        ((MvState*)List_ptr_pop(cx->cnt));
    }
}

__attribute__((hot)) void mv_loop(MvCtx* cx, MvState* st, HirExpr* cond, List_TrStr* lvs, HirBlock* body, bool always) {
#line 635 "src/movecheck.tr"
    MvState* entry = MvState_copy(st);
#line 636 "src/movecheck.tr"
    MvState* head = MvState_copy(entry);
#line 637 "src/movecheck.tr"
    long long pass_i = 0LL;
#line 638 "src/movecheck.tr"
    MvState* exit_st = MvState_init();
#line 639 "src/movecheck.tr"
    exit_st->dead = true;
#line 640 "src/movecheck.tr"
    while ((pass_i < 2LL)) {
#line 641 "src/movecheck.tr"
        MvState* s = MvState_copy(head);
#line 642 "src/movecheck.tr"
        mv_expr(cx, cond, s);
#line 643 "src/movecheck.tr"
        MvState* after_cond = MvState_copy(s);
#line 644 "src/movecheck.tr"
        long long li = 0LL;
#line 645 "src/movecheck.tr"
        while ((li < lvs->len)) {
#line 646 "src/movecheck.tr"
            ({ TrStr _at_t1083 = (List_TrStr_get(lvs, li)); mv_declare(cx, _at_t1083, s); _tr_str_release(_at_t1083); });
#line 647 "src/movecheck.tr"
            li = (li + 1LL);
        }
#line 648 "src/movecheck.tr"
        List_i64_append(cx->brk_base, cx->brk->len);
#line 649 "src/movecheck.tr"
        List_i64_append(cx->cnt_base, cx->cnt->len);
#line 650 "src/movecheck.tr"
        mv_block(cx, body, s);
#line 651 "src/movecheck.tr"
        MvState* bo = MvState_init();
#line 652 "src/movecheck.tr"
        bo->dead = true;
#line 653 "src/movecheck.tr"
        MvState* co = MvState_init();
#line 654 "src/movecheck.tr"
        co->dead = true;
#line 655 "src/movecheck.tr"
        mv_exit_loop_ctx(cx, bo, co);
#line 656 "src/movecheck.tr"
        MvState* back = MvState_copy(s);
#line 657 "src/movecheck.tr"
        mv_merge(back, co);
#line 658 "src/movecheck.tr"
        if ((pass_i == 0LL)) {
#line 659 "src/movecheck.tr"
            MvState* _cltmp_t1084 = MvState_copy(entry);
            _tr_obj_release(head, _trdrop_MvState);
            head = _cltmp_t1084;
#line 660 "src/movecheck.tr"
            if ((!back->dead)) {
#line 661 "src/movecheck.tr"
                long long bi = 0LL;
#line 662 "src/movecheck.tr"
                while ((bi < back->any->len)) {
#line 663 "src/movecheck.tr"
                    TrStr bn = List_TrStr_get(back->any, bi);
#line 664 "src/movecheck.tr"
                    if ((!mv_has(entry->any, bn))) {
#line 664 "src/movecheck.tr"
                        mv_add(head->lp, bn);
                    }
#line 665 "src/movecheck.tr"
                    bi = (bi + 1LL);
                    _tr_str_release(bn);
                }
            }
#line 666 "src/movecheck.tr"
            mv_merge(head, back);
        } else {
#line 668 "src/movecheck.tr"
            if ((!always)) {
#line 668 "src/movecheck.tr"
                mv_merge(exit_st, after_cond);
            }
#line 669 "src/movecheck.tr"
            mv_merge(exit_st, bo);
        }
#line 670 "src/movecheck.tr"
        pass_i = (pass_i + 1LL);
        _tr_obj_release(s, _trdrop_MvState);
        _tr_obj_release(after_cond, _trdrop_MvState);
        _tr_obj_release(bo, _trdrop_MvState);
        _tr_obj_release(co, _trdrop_MvState);
        _tr_obj_release(back, _trdrop_MvState);
    }
#line 671 "src/movecheck.tr"
    mv_assign_from(st, exit_st);
    _tr_obj_release(entry, _trdrop_MvState);
    _tr_obj_release(head, _trdrop_MvState);
    _tr_obj_release(exit_st, _trdrop_MvState);
}

__attribute__((hot)) void mv_bind_rhs(MvCtx* cx, HirExpr* val, MvState* st) {
#line 676 "src/movecheck.tr"
    if (mv_null(val)) {
#line 676 "src/movecheck.tr"
        return;
    }
#line 677 "src/movecheck.tr"
    TrStr _sfo_t1085 = cx->site;
    cx->site = _tr_str_lit_len("a binding transfer (`b = a`)", 28LL);
    _tr_str_release(_sfo_t1085);
#line 678 "src/movecheck.tr"
    __auto_type _t1086 = (*val);
    if (_t1086.tag == HirExpr_EIdent) {
        __auto_type nm = _t1086.data.EIdent.name;
__auto_type ty = _t1086.data.EIdent.ty;
#line 680 "src/movecheck.tr"
        mv_use(cx, nm, st);
#line 681 "src/movecheck.tr"
        if (((mv_tracked(cx, nm) && mv_is_move(cx, ty)) && (!_tr_dict_contains(cx->old, _tr_strz(ty->name))))) {
#line 681 "src/movecheck.tr"
            mv_mark(cx, nm, st);
        }
    } else if (1) {
        __auto_type _ = _t1086;
        mv_expr(cx, val, st);
    }
}

__attribute__((hot)) void mv_note_closure(MvCtx* cx, TrStr nm, HirExpr* val) {
#line 685 "src/movecheck.tr"
    if (mv_null(val)) {
#line 685 "src/movecheck.tr"
        return;
    }
#line 686 "src/movecheck.tr"
    __auto_type _t1087 = (*val);
    if (_t1087.tag == HirExpr_EClosure) {
        __auto_type caps = _t1087.data.EClosure.captures;
#line 688 "src/movecheck.tr"
        ({ __auto_type _ord_t1088 = (mv_closure_caps(cx, caps)); _tr_dict_set(cx->clcaps, _tr_strz(nm), _ord_t1088); });
    } else if (1) {
        __auto_type _ = _t1087;
        /* pass */
    }
}

__attribute__((hot)) void mv_stmt(MvCtx* cx, HirStmt* sp, MvState* st) {
#line 692 "src/movecheck.tr"
    if (st->dead) {
#line 692 "src/movecheck.tr"
        return;
    }
#line 693 "src/movecheck.tr"
    __auto_type _t1089 = (*sp);
    if (_t1089.tag == HirStmt_SLineMarker) {
        __auto_type n = _t1089.data.SLineMarker.n;
        cx->line = n;
    } else if (_t1089.tag == HirStmt_SExpr) {
        __auto_type e = _t1089.data.SExpr.expr;
        mv_expr(cx, e, st);
    } else if (_t1089.tag == HirStmt_SLet) {
        __auto_type nm = _t1089.data.SLet.name;
__auto_type ty = _t1089.data.SLet.ty;
__auto_type val = _t1089.data.SLet.val;
#line 697 "src/movecheck.tr"
        if (ty->is_borrow) {
#line 697 "src/movecheck.tr"
            mv_expr(cx, val, st);
        } else {
#line 698 "src/movecheck.tr"
            mv_bind_rhs(cx, val, st);
        }
#line 699 "src/movecheck.tr"
        if ((mv_has(cx->declared, nm) && (cx->sh_base->len > 1LL))) {
#line 700 "src/movecheck.tr"
            long long bits = 0LL;
#line 701 "src/movecheck.tr"
            if (mv_has(st->any, nm)) {
#line 701 "src/movecheck.tr"
                bits = (bits + 1LL);
            }
#line 702 "src/movecheck.tr"
            if (mv_has(st->all, nm)) {
#line 702 "src/movecheck.tr"
                bits = (bits + 2LL);
            }
#line 703 "src/movecheck.tr"
            if (mv_has(st->lp, nm)) {
#line 703 "src/movecheck.tr"
                bits = (bits + 4LL);
            }
#line 704 "src/movecheck.tr"
            List_TrStr_append(cx->sh_name, nm);
#line 705 "src/movecheck.tr"
            List_i64_append(cx->sh_bits, bits);
        }
#line 706 "src/movecheck.tr"
        mv_declare(cx, nm, st);
#line 707 "src/movecheck.tr"
        mv_note_closure(cx, nm, val);
    } else if (_t1089.tag == HirStmt_SMultiLet) {
        __auto_type names = _t1089.data.SMultiLet.names;
__auto_type val = _t1089.data.SMultiLet.val;
#line 709 "src/movecheck.tr"
        mv_expr(cx, val, st);
#line 710 "src/movecheck.tr"
        long long ni = 0LL;
#line 711 "src/movecheck.tr"
        while ((ni < names->len)) {
#line 712 "src/movecheck.tr"
            ({ TrStr _at_t1090 = (List_TrStr_get(names, ni)); mv_declare(cx, _at_t1090, st); _tr_str_release(_at_t1090); });
#line 713 "src/movecheck.tr"
            ni = (ni + 1LL);
        }
    } else if (_t1089.tag == HirStmt_SAssign) {
        __auto_type tgt = _t1089.data.SAssign.target;
__auto_type val = _t1089.data.SAssign.val;
#line 715 "src/movecheck.tr"
        if (mv_null(tgt)) {
#line 716 "src/movecheck.tr"
            mv_expr(cx, val, st);
        } else {
#line 718 "src/movecheck.tr"
            __auto_type _t1091 = (*tgt);
            if (_t1091.tag == HirExpr_EIdent) {
                __auto_type tn = _t1091.data.EIdent.name;
#line 720 "src/movecheck.tr"
                mv_bind_rhs(cx, val, st);
#line 721 "src/movecheck.tr"
                if (mv_tracked(cx, tn)) {
#line 722 "src/movecheck.tr"
                    mv_reinit(cx, tn, st);
#line 723 "src/movecheck.tr"
                    mv_note_closure(cx, tn, val);
                }
            } else if (_t1091.tag == HirExpr_ETuple) {
                __auto_type titems = _t1091.data.ETuple.items;
#line 726 "src/movecheck.tr"
                mv_expr(cx, val, st);
#line 727 "src/movecheck.tr"
                long long tti = 0LL;
#line 728 "src/movecheck.tr"
                while ((tti < titems->len)) {
#line 729 "src/movecheck.tr"
                    if ((!mv_null(((HirExpr*)List_ptr_get(titems, tti))))) {
#line 730 "src/movecheck.tr"
                        __auto_type _t1092 = (*((HirExpr*)List_ptr_get(titems, tti)));
                        if (_t1092.tag == HirExpr_EIdent) {
                            __auto_type tin = _t1092.data.EIdent.name;
                            mv_reinit(cx, tin, st);
                        } else if (1) {
                            __auto_type _ = _t1092;
                            mv_expr(cx, ((HirExpr*)List_ptr_get(titems, tti)), st);
                        }
                    }
#line 733 "src/movecheck.tr"
                    tti = (tti + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1091;
#line 736 "src/movecheck.tr"
                mv_expr(cx, tgt, st);
#line 737 "src/movecheck.tr"
                TrStr _sfo_t1093 = cx->site;
                cx->site = _tr_str_lit_len("a field/element store", 21LL);
                _tr_str_release(_sfo_t1093);
#line 738 "src/movecheck.tr"
                mv_consume(cx, val, st, 2LL);
            }
        }
    } else if (_t1089.tag == HirStmt_SReturn) {
        __auto_type v = _t1089.data.SReturn.val;
#line 740 "src/movecheck.tr"
        if (mv_null(v)) {
#line 740 "src/movecheck.tr"
            /* pass */
        } else {
#line 742 "src/movecheck.tr"
            __auto_type _t1094 = (*v);
            if (_t1094.tag == HirExpr_EClosure) {
                __auto_type caps = _t1094.data.EClosure.captures;
#line 744 "src/movecheck.tr"
                mv_expr(cx, v, st);
#line 745 "src/movecheck.tr"
                mv_consume_caps(cx, mv_closure_caps(cx, caps), st);
            } else if (1) {
                __auto_type _ = _t1094;
#line 747 "src/movecheck.tr"
                TrStr _sfo_t1095 = cx->site;
                cx->site = _tr_str_lit_len("a return", 8LL);
                _tr_str_release(_sfo_t1095);
#line 748 "src/movecheck.tr"
                mv_consume(cx, v, st, 2LL);
            }
        }
#line 749 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1089.tag == HirStmt_SRaise) {
        __auto_type v = _t1089.data.SRaise.val;
#line 751 "src/movecheck.tr"
        mv_expr(cx, v, st);
#line 752 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1089.tag == HirStmt_SDefer) {
        __auto_type ds = _t1089.data.SDefer.stmt;
#line 754 "src/movecheck.tr"
        List_TrStr* ids = (void*)List_TrStr_new();
#line 755 "src/movecheck.tr"
        __auto_type _t1096 = (*ds);
        if (_t1096.tag == HirStmt_SExpr) {
            __auto_type de = _t1096.data.SExpr.expr;
            mv_idents(de, ids);
        } else if (_t1096.tag == HirStmt_SAssign) {
            __auto_type dt = _t1096.data.SAssign.target;
__auto_type dv = _t1096.data.SAssign.val;
#line 758 "src/movecheck.tr"
            mv_idents(dt, ids);
#line 759 "src/movecheck.tr"
            mv_idents(dv, ids);
        } else if (1) {
            __auto_type _ = _t1096;
            /* pass */
        }
#line 761 "src/movecheck.tr"
        long long di = 0LL;
#line 762 "src/movecheck.tr"
        while ((di < ids->len)) {
#line 763 "src/movecheck.tr"
            if (({ TrStr _at_t1097 = (List_TrStr_get(ids, di)); __auto_type _wr = (mv_tracked(cx, _at_t1097)); _tr_str_release(_at_t1097); _wr; })) {
#line 763 "src/movecheck.tr"
                ({ __auto_type _ord_t1099 = (cx->defers); TrStr _at_t1098 = (List_TrStr_get(ids, di)); mv_add(_ord_t1099, _at_t1098); _tr_str_release(_at_t1098); });
            }
#line 764 "src/movecheck.tr"
            di = (di + 1LL);
        }
    } else if (_t1089.tag == HirStmt_SBreak) {
        __auto_type v = _t1089.data.SBreak.val;
#line 766 "src/movecheck.tr"
        mv_expr(cx, v, st);
#line 767 "src/movecheck.tr"
        if ((cx->brk_base->len > 0LL)) {
#line 767 "src/movecheck.tr"
            ({ MvState* _ord_t1100 = (MvState_copy(st)); List_ptr_append(cx->brk, _tr_obj_retain(_ord_t1100)); _tr_obj_release(_ord_t1100, _trdrop_MvState); });
        }
#line 768 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1089.tag == HirStmt_SContinue) {
#line 770 "src/movecheck.tr"
        if ((cx->cnt_base->len > 0LL)) {
#line 770 "src/movecheck.tr"
            ({ MvState* _ord_t1101 = (MvState_copy(st)); List_ptr_append(cx->cnt, _tr_obj_retain(_ord_t1101)); _tr_obj_release(_ord_t1101, _trdrop_MvState); });
        }
#line 771 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1089.tag == HirStmt_SUnsafe) {
        __auto_type body = _t1089.data.SUnsafe.body;
#line 773 "src/movecheck.tr"
        cx->unsafe_depth = (cx->unsafe_depth + 1LL);
#line 774 "src/movecheck.tr"
        mv_block(cx, body, st);
#line 775 "src/movecheck.tr"
        cx->unsafe_depth = (cx->unsafe_depth - 1LL);
    } else if (_t1089.tag == HirStmt_SIf) {
        __auto_type cond = _t1089.data.SIf.cond;
__auto_type tb = _t1089.data.SIf.then_b;
__auto_type eb = _t1089.data.SIf.else_b;
#line 777 "src/movecheck.tr"
        mv_expr(cx, cond, st);
#line 778 "src/movecheck.tr"
        MvState* s1 = MvState_copy(st);
#line 779 "src/movecheck.tr"
        MvState* s2 = MvState_copy(st);
#line 780 "src/movecheck.tr"
        mv_block(cx, tb, s1);
#line 781 "src/movecheck.tr"
        mv_block(cx, eb, s2);
#line 782 "src/movecheck.tr"
        mv_assign_from(st, s1);
#line 783 "src/movecheck.tr"
        mv_merge(st, s2);
        _tr_obj_release(s1, _trdrop_MvState);
        _tr_obj_release(s2, _trdrop_MvState);
    } else if (_t1089.tag == HirStmt_SWhile) {
        __auto_type cond = _t1089.data.SWhile.cond;
__auto_type body = _t1089.data.SWhile.body;
#line 785 "src/movecheck.tr"
        bool always = false;
#line 786 "src/movecheck.tr"
        if ((!mv_null(cond))) {
#line 787 "src/movecheck.tr"
            __auto_type _t1102 = (*cond);
            if (_t1102.tag == HirExpr_ELitBool) {
                __auto_type bv = _t1102.data.ELitBool.val;
                always = bv;
            } else if (1) {
                __auto_type _ = _t1102;
                /* pass */
            }
        }
#line 790 "src/movecheck.tr"
        mv_loop(cx, st, cond, (void*)List_TrStr_new(), body, always);
    } else if (_t1089.tag == HirStmt_SFor) {
        __auto_type var = _t1089.data.SFor.var;
__auto_type it = _t1089.data.SFor.iter;
__auto_type body = _t1089.data.SFor.body;
#line 792 "src/movecheck.tr"
        mv_expr(cx, it, st);
#line 793 "src/movecheck.tr"
        List_TrStr* lv = (void*)List_TrStr_new();
#line 794 "src/movecheck.tr"
        List_TrStr_append(lv, var);
#line 795 "src/movecheck.tr"
        mv_add(cx->declared, var);
#line 796 "src/movecheck.tr"
        mv_loop(cx, st, (HirExpr*)(0LL), lv, body, false);
    } else if (_t1089.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t1089.data.SForUnpack.vars;
__auto_type it = _t1089.data.SForUnpack.iter;
__auto_type body = _t1089.data.SForUnpack.body;
#line 798 "src/movecheck.tr"
        mv_expr(cx, it, st);
#line 799 "src/movecheck.tr"
        long long vi = 0LL;
#line 800 "src/movecheck.tr"
        while ((vi < vars->len)) {
#line 801 "src/movecheck.tr"
            ({ __auto_type _ord_t1104 = (cx->declared); TrStr _at_t1103 = (List_TrStr_get(vars, vi)); mv_add(_ord_t1104, _at_t1103); _tr_str_release(_at_t1103); });
#line 802 "src/movecheck.tr"
            vi = (vi + 1LL);
        }
#line 803 "src/movecheck.tr"
        mv_loop(cx, st, (HirExpr*)(0LL), vars, body, false);
    } else if (_t1089.tag == HirStmt_SMatch) {
        __auto_type subj = _t1089.data.SMatch.expr;
__auto_type arms = _t1089.data.SMatch.arms;
#line 805 "src/movecheck.tr"
        mv_expr(cx, subj, st);
#line 806 "src/movecheck.tr"
        MvState* acc = MvState_init();
#line 807 "src/movecheck.tr"
        acc->dead = true;
#line 808 "src/movecheck.tr"
        long long ai = 0LL;
#line 809 "src/movecheck.tr"
        while ((ai < arms->len)) {
#line 810 "src/movecheck.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 811 "src/movecheck.tr"
            MvState* c = MvState_copy(st);
#line 812 "src/movecheck.tr"
            mv_pat(cx, a->pat, c);
#line 813 "src/movecheck.tr"
            mv_expr(cx, a->guard, c);
#line 814 "src/movecheck.tr"
            mv_block(cx, a->body, c);
#line 815 "src/movecheck.tr"
            mv_merge(acc, c);
#line 816 "src/movecheck.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c, _trdrop_MvState);
        }
#line 817 "src/movecheck.tr"
        if ((arms->len > 0LL)) {
#line 817 "src/movecheck.tr"
            mv_assign_from(st, acc);
        }
        _tr_obj_release(acc, _trdrop_MvState);
    } else if (_t1089.tag == HirStmt_STry) {
        __auto_type tb = _t1089.data.STry.try_body;
__auto_type catches = _t1089.data.STry.catches;
__auto_type fb = _t1089.data.STry.finally_b;
#line 819 "src/movecheck.tr"
        MvState* entry = MvState_copy(st);
#line 820 "src/movecheck.tr"
        MvState* s1 = MvState_copy(st);
#line 821 "src/movecheck.tr"
        List_TrStr* outer_acc = cx->try_acc;
#line 822 "src/movecheck.tr"
        cx->try_acc = (void*)List_TrStr_new();
#line 823 "src/movecheck.tr"
        cx->try_depth = (cx->try_depth + 1LL);
#line 824 "src/movecheck.tr"
        mv_block(cx, tb, s1);
#line 825 "src/movecheck.tr"
        cx->try_depth = (cx->try_depth - 1LL);
#line 826 "src/movecheck.tr"
        List_TrStr* moved_in_try = cx->try_acc;
#line 827 "src/movecheck.tr"
        cx->try_acc = outer_acc;
#line 828 "src/movecheck.tr"
        long long tai = 0LL;
#line 829 "src/movecheck.tr"
        while ((tai < moved_in_try->len)) {
#line 830 "src/movecheck.tr"
            if ((cx->try_depth > 0LL)) {
#line 830 "src/movecheck.tr"
                ({ __auto_type _ord_t1106 = (cx->try_acc); TrStr _at_t1105 = (List_TrStr_get(moved_in_try, tai)); mv_add(_ord_t1106, _at_t1105); _tr_str_release(_at_t1105); });
            }
#line 831 "src/movecheck.tr"
            tai = (tai + 1LL);
        }
#line 832 "src/movecheck.tr"
        MvState* acc = MvState_copy(s1);
#line 833 "src/movecheck.tr"
        MvState* hs = MvState_copy(entry);
#line 834 "src/movecheck.tr"
        mv_merge(hs, s1);
#line 835 "src/movecheck.tr"
        hs->dead = false;
#line 837 "src/movecheck.tr"
        tai = 0LL;
#line 838 "src/movecheck.tr"
        while ((tai < moved_in_try->len)) {
#line 839 "src/movecheck.tr"
            ({ __auto_type _ord_t1108 = (hs->any); TrStr _at_t1107 = (List_TrStr_get(moved_in_try, tai)); mv_add(_ord_t1108, _at_t1107); _tr_str_release(_at_t1107); });
#line 840 "src/movecheck.tr"
            tai = (tai + 1LL);
        }
#line 841 "src/movecheck.tr"
        long long ci = 0LL;
#line 842 "src/movecheck.tr"
        while ((ci < catches->len)) {
#line 843 "src/movecheck.tr"
            MvState* cs = MvState_copy(hs);
#line 844 "src/movecheck.tr"
            ({ TrStr _apt_t1109 = _tr_str_retain((*((HirCatchClause**)List_ptr_get(catches, ci)))->err_name); mv_declare(cx, _apt_t1109, cs); _tr_str_release(_apt_t1109); });
#line 845 "src/movecheck.tr"
            mv_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body, cs);
#line 846 "src/movecheck.tr"
            mv_merge(acc, cs);
#line 847 "src/movecheck.tr"
            ci = (ci + 1LL);
            _tr_obj_release(cs, _trdrop_MvState);
        }
#line 848 "src/movecheck.tr"
        mv_block(cx, fb, acc);
#line 849 "src/movecheck.tr"
        mv_assign_from(st, acc);
        _tr_obj_release(entry, _trdrop_MvState);
        _tr_obj_release(s1, _trdrop_MvState);
        _tr_obj_release(acc, _trdrop_MvState);
        _tr_obj_release(hs, _trdrop_MvState);
    } else if (_t1089.tag == HirStmt_SAssert) {
        __auto_type c = _t1089.data.SAssert.cond;
__auto_type m = _t1089.data.SAssert.msg;
#line 851 "src/movecheck.tr"
        mv_expr(cx, c, st);
#line 852 "src/movecheck.tr"
        mv_expr(cx, m, st);
    } else if (_t1089.tag == HirStmt_SWith) {
        __auto_type items = _t1089.data.SWith.items;
__auto_type aliases = _t1089.data.SWith.aliases;
__auto_type body = _t1089.data.SWith.body;
#line 854 "src/movecheck.tr"
        mv_exprs(cx, items, st);
#line 855 "src/movecheck.tr"
        long long wi = 0LL;
#line 856 "src/movecheck.tr"
        while ((wi < aliases->len)) {
#line 857 "src/movecheck.tr"
            ({ TrStr _at_t1110 = (List_TrStr_get(aliases, wi)); mv_declare(cx, _at_t1110, st); _tr_str_release(_at_t1110); });
#line 858 "src/movecheck.tr"
            wi = (wi + 1LL);
        }
#line 859 "src/movecheck.tr"
        mv_block(cx, body, st);
    } else if (_t1089.tag == HirStmt_SSpawn) {
        __auto_type e = _t1089.data.SSpawn.expr;
#line 861 "src/movecheck.tr"
        if (mv_null(e)) {
#line 861 "src/movecheck.tr"
            /* pass */
        } else {
#line 863 "src/movecheck.tr"
            __auto_type _t1111 = (*e);
            if (_t1111.tag == HirExpr_ECall) {
                __auto_type sc = _t1111.data.ECall.callee;
__auto_type sargs = _t1111.data.ECall.args;
#line 865 "src/movecheck.tr"
                mv_expr(cx, sc, st);
#line 866 "src/movecheck.tr"
                mv_args(cx, sargs, mv_all_mask(sargs->len, 2LL), st, _tr_str_lit_len("spawn", 5LL));
            } else if (_t1111.tag == HirExpr_EClosure) {
                __auto_type caps = _t1111.data.EClosure.captures;
#line 868 "src/movecheck.tr"
                mv_expr(cx, e, st);
#line 869 "src/movecheck.tr"
                mv_consume_caps(cx, mv_closure_caps(cx, caps), st);
            } else if (1) {
                __auto_type _ = _t1111;
                mv_expr(cx, e, st);
            }
        }
    } else if (_t1089.tag == HirStmt_STaskGroup) {
        __auto_type body = _t1089.data.STaskGroup.body;
        mv_block(cx, body, st);
    } else if (_t1089.tag == HirStmt_SGpuBlock) {
        __auto_type body = _t1089.data.SGpuBlock.body;
        mv_block(cx, body, st);
    } else if (1) {
        __auto_type _ = _t1089;
        /* pass */
    }
}

__attribute__((hot)) List_TrStr* mv_run_fn(MvCtx* cx, HirFunction* hf) {
#line 876 "src/movecheck.tr"
    cx->errs = (void*)List_TrStr_new();
#line 877 "src/movecheck.tr"
    TrStr _sfo_t1112 = cx->fn_name;
    cx->fn_name = _tr_str_retain(hf->name);
    _tr_str_release(_sfo_t1112);
#line 878 "src/movecheck.tr"
    cx->declared = (void*)List_TrStr_new();
#line 879 "src/movecheck.tr"
    cx->mline = _tr_dict_new(4LL);
#line 880 "src/movecheck.tr"
    cx->clcaps = _tr_dict_new(2LL);
#line 881 "src/movecheck.tr"
    cx->defers = (void*)List_TrStr_new();
#line 882 "src/movecheck.tr"
    cx->brk = (void*)List_ptr_new();
#line 883 "src/movecheck.tr"
    cx->brk_base = (void*)List_i64_new();
#line 884 "src/movecheck.tr"
    cx->cnt = (void*)List_ptr_new();
#line 885 "src/movecheck.tr"
    cx->cnt_base = (void*)List_i64_new();
#line 886 "src/movecheck.tr"
    cx->unsafe_depth = 0LL;
#line 887 "src/movecheck.tr"
    cx->in_defer = false;
#line 888 "src/movecheck.tr"
    cx->line = 0LL;
#line 889 "src/movecheck.tr"
    cx->sh_name = (void*)List_TrStr_new();
#line 890 "src/movecheck.tr"
    cx->sh_bits = (void*)List_i64_new();
#line 891 "src/movecheck.tr"
    cx->sh_base = (void*)List_i64_new();
#line 892 "src/movecheck.tr"
    cx->try_acc = (void*)List_TrStr_new();
#line 893 "src/movecheck.tr"
    cx->try_depth = 0LL;
#line 894 "src/movecheck.tr"
    long long pi = 0LL;
#line 895 "src/movecheck.tr"
    while ((pi < hf->params->len)) {
#line 896 "src/movecheck.tr"
        ({ __auto_type _ord_t1114 = (cx->declared); TrStr _apt_t1113 = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name); mv_add(_ord_t1114, _apt_t1113); _tr_str_release(_apt_t1113); });
#line 897 "src/movecheck.tr"
        pi = (pi + 1LL);
    }
#line 898 "src/movecheck.tr"
    MvState* st = MvState_init();
#line 899 "src/movecheck.tr"
    mv_block(cx, hf->body, st);
#line 900 "src/movecheck.tr"
    _tr_obj_release(st, _trdrop_MvState);
    return cx->errs;
}

__attribute__((hot)) void mv_add_sig(MvCtx* cx, HirFunction* f, TrStr key, List_TrStr* cgens) {
#line 903 "src/movecheck.tr"
    if (_tr_dict_contains(cx->sigs, _tr_strz(key))) {
#line 905 "src/movecheck.tr"
        HirFunction* ex = _tr_obj_retain(((MvSig*)(uintptr_t)_tr_dict_get(cx->sigs, _tr_strz(key)))->f);
#line 906 "src/movecheck.tr"
        bool same = (ex->params->len == f->params->len);
#line 907 "src/movecheck.tr"
        long long pj = 0LL;
#line 908 "src/movecheck.tr"
        while ((same && (pj < f->params->len))) {
#line 909 "src/movecheck.tr"
            if ((({ __auto_type _ord_t1115 = (((HirParam*)List_ptr_get(ex->params, pj))->ty->name); __auto_type _wr = ((!_tr_str_eqv((_ord_t1115), (((HirParam*)List_ptr_get(f->params, pj))->ty->name)))); _wr; }) || ({ __auto_type _ord_t1116 = (((HirParam*)List_ptr_get(ex->params, pj))->ty->is_borrow); __auto_type _wr = ((_ord_t1116 != ((HirParam*)List_ptr_get(f->params, pj))->ty->is_borrow)); _wr; }))) {
#line 909 "src/movecheck.tr"
                same = false;
            }
#line 910 "src/movecheck.tr"
            pj = (pj + 1LL);
        }
#line 911 "src/movecheck.tr"
        if ((!same)) {
#line 911 "src/movecheck.tr"
            _tr_dict_set(cx->amb, _tr_strz(key), true);
        }
#line 912 "src/movecheck.tr"
        _tr_obj_release(ex, _trdrop_HirFunction);
        return;
    }
#line 913 "src/movecheck.tr"
    MvSig* sg = ((MvSig*)_tr_obj_alloc(sizeof(MvSig)));
#line 914 "src/movecheck.tr"
    HirFunction* _cltmp_t1117 = _tr_obj_retain(f);
    _tr_obj_release(sg->f, _trdrop_HirFunction);
    sg->f = _cltmp_t1117;
#line 915 "src/movecheck.tr"
    sg->lib = f->is_lib;
#line 916 "src/movecheck.tr"
    List_TrStr* gs = (void*)List_TrStr_new();
#line 917 "src/movecheck.tr"
    long long i = 0LL;
#line 918 "src/movecheck.tr"
    while ((i < f->generics->len)) {
#line 919 "src/movecheck.tr"
        ({ TrStr _at_t1118 = (List_TrStr_get(f->generics, i)); List_TrStr_append(gs, _at_t1118); _tr_str_release(_at_t1118); });
#line 920 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 921 "src/movecheck.tr"
    i = 0LL;
#line 922 "src/movecheck.tr"
    while ((i < cgens->len)) {
#line 923 "src/movecheck.tr"
        ({ TrStr _at_t1119 = (List_TrStr_get(cgens, i)); List_TrStr_append(gs, _at_t1119); _tr_str_release(_at_t1119); });
#line 924 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 925 "src/movecheck.tr"
    sg->gens = gs;
#line 926 "src/movecheck.tr"
    _tr_dict_set_obj(cx->sigs, _tr_strz(key), _tr_obj_retain(sg), _trdrop_MvSig);
    _tr_obj_release(sg, _trdrop_MvSig);
}

__attribute__((hot)) List_TrStr* movecheck_check(List_ptr* fns, HirProgram* hp, TrMap* mvt, TrMap* oldt) {
#line 932 "src/movecheck.tr"
    MvCtx* cx = MvCtx_init();
#line 933 "src/movecheck.tr"
    cx->mv = mvt;
#line 934 "src/movecheck.tr"
    cx->old = oldt;
#line 935 "src/movecheck.tr"
    List_TrStr* nogens = (void*)List_TrStr_new();
#line 936 "src/movecheck.tr"
    long long i = 0LL;
#line 937 "src/movecheck.tr"
    while ((i < hp->functions->len)) {
#line 938 "src/movecheck.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(hp->functions, i));
#line 939 "src/movecheck.tr"
        if (_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) {
#line 939 "src/movecheck.tr"
            ({ TrStr _apt_t1120 = _tr_str_retain(f->name); mv_add_sig(cx, f, _apt_t1120, nogens); _tr_str_release(_apt_t1120); });
        } else {
#line 940 "src/movecheck.tr"
            ({ TrStr _at_t1121 = (({ TrStr _tr_cl = (_tr_strx_concatv((f->class_name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); mv_add_sig(cx, f, _at_t1121, nogens); _tr_str_release(_at_t1121); });
        }
#line 941 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 942 "src/movecheck.tr"
    i = 0LL;
#line 943 "src/movecheck.tr"
    while ((i < hp->classes->len)) {
#line 944 "src/movecheck.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, i));
#line 945 "src/movecheck.tr"
        long long j = 0LL;
#line 946 "src/movecheck.tr"
        while ((j < c->methods->len)) {
#line 947 "src/movecheck.tr"
            ({ __auto_type _ord_t1124 = (((HirFunction*)List_ptr_get(c->methods, j))); __auto_type _ord_t1122 = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1123 = (({ TrStr _tr_cl = (_ord_t1122); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(c->methods, j))->name)); _tr_str_release(_tr_cl); _tr_cres; })); mv_add_sig(cx, _ord_t1124, _at_t1123, c->generics); _tr_str_release(_at_t1123); });
#line 948 "src/movecheck.tr"
            j = (j + 1LL);
        }
#line 949 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 950 "src/movecheck.tr"
    i = 0LL;
#line 951 "src/movecheck.tr"
    while ((i < hp->enums->len)) {
#line 952 "src/movecheck.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, i));
#line 953 "src/movecheck.tr"
        long long j2 = 0LL;
#line 954 "src/movecheck.tr"
        while ((j2 < en->methods->len)) {
#line 955 "src/movecheck.tr"
            ({ __auto_type _ord_t1127 = (((HirFunction*)List_ptr_get(en->methods, j2))); __auto_type _ord_t1125 = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1126 = (({ TrStr _tr_cl = (_ord_t1125); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(en->methods, j2))->name)); _tr_str_release(_tr_cl); _tr_cres; })); mv_add_sig(cx, _ord_t1127, _at_t1126, en->generics); _tr_str_release(_at_t1126); });
#line 956 "src/movecheck.tr"
            j2 = (j2 + 1LL);
        }
#line 957 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 958 "src/movecheck.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 959 "src/movecheck.tr"
    long long fi = 0LL;
#line 960 "src/movecheck.tr"
    while ((fi < fns->len)) {
#line 961 "src/movecheck.tr"
        HirFunction* hf = ((HirFunction*)List_ptr_get(fns, fi));
#line 962 "src/movecheck.tr"
        if ((!hf->is_lib)) {
#line 963 "src/movecheck.tr"
            List_TrStr* errs = mv_run_fn(cx, hf);
#line 964 "src/movecheck.tr"
            long long ei = 0LL;
#line 965 "src/movecheck.tr"
            while ((ei < errs->len)) {
#line 966 "src/movecheck.tr"
                ({ __auto_type _ord_t1128 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(fi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("@", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1129 = (({ TrStr _tr_cl = (_ord_t1128); TrStr _tr_cr = (List_TrStr_get(errs, ei)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(out, _at_t1129); _tr_str_release(_at_t1129); });
#line 967 "src/movecheck.tr"
                ei = (ei + 1LL);
            }
        }
#line 968 "src/movecheck.tr"
        fi = (fi + 1LL);
    }
#line 969 "src/movecheck.tr"
    _tr_obj_release(cx, _trdrop_MvCtx);
    return out;
}

