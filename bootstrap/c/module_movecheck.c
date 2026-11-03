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
void mv_reinit_name_quiet(MvState* st, TrStr nm);
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
        ({ TrStr _ord_t1040 = (List_TrStr_get(self->any, i)); List_TrStr_append(s->any, _ord_t1040); _tr_str_release(_ord_t1040); });
#line 49 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 50 "src/movecheck.tr"
    i = 0LL;
#line 51 "src/movecheck.tr"
    while ((i < self->all->len)) {
#line 52 "src/movecheck.tr"
        ({ TrStr _ord_t1041 = (List_TrStr_get(self->all, i)); List_TrStr_append(s->all, _ord_t1041); _tr_str_release(_ord_t1041); });
#line 53 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 54 "src/movecheck.tr"
    i = 0LL;
#line 55 "src/movecheck.tr"
    while ((i < self->lp->len)) {
#line 56 "src/movecheck.tr"
        ({ TrStr _ord_t1042 = (List_TrStr_get(self->lp, i)); List_TrStr_append(s->lp, _ord_t1042); _tr_str_release(_ord_t1042); });
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
    TrStr _sfo_t1043 = c->fn_name;
    c->fn_name = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t1043);
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
    TrStr _sfo_t1044 = c->site;
    c->site = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t1044);
#line 161 "src/movecheck.tr"
    TrStr _sfo_t1045 = c->callee;
    c->callee = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t1045);
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
        if (({ TrStr _cmpa_t1046 = (List_TrStr_get(v, i)); TrStr _cmpb_t1047 = (s); _Bool _cmpr_t1048 = _tr_str_eqv(_cmpa_t1046, _cmpb_t1047); _tr_str_release(_cmpa_t1046); _cmpr_t1048; })) {
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
        if (({ TrStr _cmpa_t1049 = (List_TrStr_get(v, i)); TrStr _cmpb_t1050 = (s); _Bool _cmpr_t1051 = !_tr_str_eqv(_cmpa_t1049, _cmpb_t1050); _tr_str_release(_cmpa_t1049); _cmpr_t1051; })) {
#line 75 "src/movecheck.tr"
            ({ TrStr _at_t1052 = (List_TrStr_get(v, i)); List_TrStr_append(out, _at_t1052); _tr_str_release(_at_t1052); });
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
        if (({ TrStr _at_t1053 = (List_TrStr_get(a, i)); __auto_type _wr = (mv_has(b, _at_t1053)); _tr_str_release(_at_t1053); _wr; })) {
#line 83 "src/movecheck.tr"
            ({ TrStr _at_t1054 = (List_TrStr_get(a, i)); List_TrStr_append(out, _at_t1054); _tr_str_release(_at_t1054); });
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
        ({ __auto_type _ord_t1056 = (into->any); TrStr _at_t1055 = (List_TrStr_get(o->any, i)); mv_add(_ord_t1056, _at_t1055); _tr_str_release(_at_t1055); });
#line 101 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 102 "src/movecheck.tr"
    i = 0LL;
#line 103 "src/movecheck.tr"
    while ((i < o->lp->len)) {
#line 104 "src/movecheck.tr"
        ({ __auto_type _ord_t1058 = (into->lp); TrStr _at_t1057 = (List_TrStr_get(o->lp, i)); mv_add(_ord_t1058, _at_t1057); _tr_str_release(_at_t1057); });
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
            bool _trret202 = mv_is_move(cx, (*((AstType**)List_ptr_get(t->args, 0LL))));
            _tr_str_release(n);
            return _trret202;
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
    if (((((((((_tr_str_eqv((n), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Deque", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("StringBuilder", 13LL))))) {
#line 190 "src/movecheck.tr"
        _tr_str_release(n);
        return true;
    }
#line 191 "src/movecheck.tr"
    if (_tr_dict_contains(cx->mv, _tr_strz(n))) {
#line 191 "src/movecheck.tr"
        bool _trret203 = ((bool)(uintptr_t)_tr_dict_get(cx->mv, _tr_strz(n)));
        _tr_str_release(n);
        return _trret203;
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
#line 203 "src/movecheck.tr"
    if (_tr_dict_contains(cx->clcaps, _tr_strz(nm))) {
#line 204 "src/movecheck.tr"
        List_TrStr* cc = ((List_TrStr*)(uintptr_t)_tr_dict_get(cx->clcaps, _tr_strz(nm)));
#line 205 "src/movecheck.tr"
        long long ci = 0LL;
#line 206 "src/movecheck.tr"
        while ((ci < cc->len)) {
#line 207 "src/movecheck.tr"
            TrStr cn = List_TrStr_get(cc, ci);
#line 208 "src/movecheck.tr"
            if (mv_has(st->any, cn)) {
#line 209 "src/movecheck.tr"
                TrStr cat = _tr_str_lit_len("", 0LL);
#line 210 "src/movecheck.tr"
                if (_tr_dict_contains(cx->mline, _tr_strz(cn))) {
#line 210 "src/movecheck.tr"
                    TrStr _strtmp_t1059 = ({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(((long long)(uintptr_t)_tr_dict_get(cx->mline, _tr_strz(cn))))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(" (moved at line ", 16LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
                    _tr_str_release(cat);
                    cat = _strtmp_t1059;
                }
#line 211 "src/movecheck.tr"
                if (mv_has(st->all, cn)) {
#line 212 "src/movecheck.tr"
                    ({ TrStr _at_t1060 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] closure '", 15LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' captured '", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which was moved", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" before this use of the closure (--strict: passing a value by value consumes it).\n      FIX: move a `clone()` of '", 114LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' instead, or stop using '", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after the move.", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1060); _tr_str_release(_at_t1060); });
                } else {
#line 214 "src/movecheck.tr"
                    ({ TrStr _at_t1061 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-5] closure '", 15LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' captured '", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which may have been moved", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cat)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" on some code paths before this use of the closure.\n      FIX: do not move '", 76LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' on any branch before using '", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' (move a `clone()`).", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1061); _tr_str_release(_at_t1061); });
                }
#line 215 "src/movecheck.tr"
                mv_reinit_name_quiet(st, cn);
            }
#line 216 "src/movecheck.tr"
            ci = (ci + 1LL);
            _tr_str_release(cn);
        }
    }
#line 217 "src/movecheck.tr"
    if ((!mv_has(st->any, nm))) {
#line 217 "src/movecheck.tr"
        return;
    }
#line 218 "src/movecheck.tr"
    TrStr at = _tr_str_lit_len("", 0LL);
#line 219 "src/movecheck.tr"
    if (_tr_dict_contains(cx->mline, _tr_strz(nm))) {
#line 220 "src/movecheck.tr"
        TrStr _strtmp_t1062 = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(((long long)(uintptr_t)_tr_dict_get(cx->mline, _tr_strz(nm))))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len(" (moved at line ", 16LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(at);
        at = _strtmp_t1062;
#line 221 "src/movecheck.tr"
        if (_tr_dict_contains(cx->msite, _tr_strz(nm))) {
#line 221 "src/movecheck.tr"
            ({ TrStr _ap_t1063 = (_tr_str_lit_len(", into ", 7LL)); TrStr _ap_t1064 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(cx->msite, _tr_strz(nm))))); _tr_str_append(&at, _ap_t1063); _tr_str_append(&at, _ap_t1064); _tr_str_release(_ap_t1064); });
        }
#line 222 "src/movecheck.tr"
        ({ TrStr _ap_t1065 = (_tr_str_lit_len(")", 1LL)); _tr_str_append(&at, _ap_t1065); });
    }
#line 223 "src/movecheck.tr"
    if (mv_has(st->all, nm)) {
#line 224 "src/movecheck.tr"
        ({ TrStr _at_t1066 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was moved", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and cannot be used again (--strict: passing a value by value consumes it).\n      FIX: pass a copy to the call that consumes it (`", 130LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".clone()`), pass a `ref` borrow, or stop using '", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after the move; assign it a new value to re-initialise it.", 60LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1066); _tr_str_release(_at_t1066); });
    } else if (mv_has(st->lp, nm)) {
#line 226 "src/movecheck.tr"
        ({ TrStr _at_t1067 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is moved inside a loop", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" and used again by the next iteration: it was already moved in the previous one.\n      FIX: move a `clone()` of it, give '", 122LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' a new value before the end of the loop body, or `break` right after the move.", 79LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1067); _tr_str_release(_at_t1067); });
    } else {
#line 228 "src/movecheck.tr"
        ({ TrStr _at_t1068 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-5] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' may have been moved", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (at)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" on some code paths, making this use unsafe (--strict).\n      FIX: do not move '", 80LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' on any branch before this point (move a `clone()`), or restructure so the use is inside the branch where it is still valid.", 125LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1068); _tr_str_release(_at_t1068); });
    }
    _tr_str_release(at);
}

__attribute__((hot)) void mv_reinit_name_quiet(MvState* st, TrStr nm) {
#line 232 "src/movecheck.tr"
    st->any = mv_without(st->any, nm);
#line 233 "src/movecheck.tr"
    st->all = mv_without(st->all, nm);
#line 234 "src/movecheck.tr"
    st->lp = mv_without(st->lp, nm);
}

__attribute__((hot)) void mv_mark(MvCtx* cx, TrStr nm, MvState* st) {
#line 237 "src/movecheck.tr"
    if (((st->dead || (cx->unsafe_depth > 0LL)) || cx->in_defer)) {
#line 237 "src/movecheck.tr"
        return;
    }
#line 238 "src/movecheck.tr"
    if (mv_has(cx->defers, nm)) {
#line 239 "src/movecheck.tr"
        ({ TrStr _at_t1069 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("[M-1] '", 7LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is read by a `defer` statement of this function and is moved here; the deferred statement would run after its new owner released it.\n      FIX: move a `clone()` instead, or do not move '", 188LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' after deferring a use of it.", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); MvCtx_err(cx, _at_t1069); _tr_str_release(_at_t1069); });
    }
#line 240 "src/movecheck.tr"
    mv_add(st->any, nm);
#line 241 "src/movecheck.tr"
    mv_add(st->all, nm);
#line 242 "src/movecheck.tr"
    _tr_dict_set(cx->mline, _tr_strz(nm), cx->line);
#line 243 "src/movecheck.tr"
    _tr_dict_set_str(cx->msite, _tr_strz(nm), _tr_str_box(_tr_str_retain(cx->site)));
#line 244 "src/movecheck.tr"
    if ((cx->try_depth > 0LL)) {
#line 244 "src/movecheck.tr"
        mv_add(cx->try_acc, nm);
    }
}

__attribute__((hot)) void mv_reinit(MvCtx* cx, TrStr nm, MvState* st) {
#line 247 "src/movecheck.tr"
    st->any = mv_without(st->any, nm);
#line 248 "src/movecheck.tr"
    st->all = mv_without(st->all, nm);
#line 249 "src/movecheck.tr"
    st->lp = mv_without(st->lp, nm);
#line 250 "src/movecheck.tr"
    if (_tr_dict_contains(cx->clcaps, _tr_strz(nm))) {
#line 250 "src/movecheck.tr"
        _tr_dict_remove(cx->clcaps, _tr_strz(nm));
    }
}

__attribute__((hot)) void mv_declare(MvCtx* cx, TrStr nm, MvState* st) {
#line 253 "src/movecheck.tr"
    mv_add(cx->declared, nm);
#line 254 "src/movecheck.tr"
    mv_reinit(cx, nm, st);
}

__attribute__((hot)) void mv_idents(HirExpr* e, List_TrStr* out) {
#line 259 "src/movecheck.tr"
    if (mv_null(e)) {
#line 259 "src/movecheck.tr"
        return;
    }
#line 260 "src/movecheck.tr"
    __auto_type _t1070 = (*e);
    if (_t1070.tag == HirExpr_EIdent) {
        __auto_type nm = _t1070.data.EIdent.name;
        mv_add(out, nm);
    } else if (_t1070.tag == HirExpr_EBinOp) {
        __auto_type l = _t1070.data.EBinOp.left;
__auto_type r = _t1070.data.EBinOp.right;
#line 263 "src/movecheck.tr"
        mv_idents(l, out);
#line 264 "src/movecheck.tr"
        mv_idents(r, out);
    } else if (_t1070.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t1070.data.EUnaryOp.expr;
        mv_idents(x, out);
    } else if (_t1070.tag == HirExpr_ECall) {
        __auto_type c = _t1070.data.ECall.callee;
__auto_type args = _t1070.data.ECall.args;
#line 267 "src/movecheck.tr"
        mv_idents(c, out);
#line 268 "src/movecheck.tr"
        long long i = 0LL;
#line 269 "src/movecheck.tr"
        while ((i < args->len)) {
#line 270 "src/movecheck.tr"
            mv_idents(((HirExpr*)List_ptr_get(args, i)), out);
#line 271 "src/movecheck.tr"
            i = (i + 1LL);
        }
    } else if (_t1070.tag == HirExpr_EMethodCall) {
        __auto_type o = _t1070.data.EMethodCall.obj;
__auto_type args = _t1070.data.EMethodCall.args;
#line 273 "src/movecheck.tr"
        mv_idents(o, out);
#line 274 "src/movecheck.tr"
        long long j = 0LL;
#line 275 "src/movecheck.tr"
        while ((j < args->len)) {
#line 276 "src/movecheck.tr"
            mv_idents(((HirExpr*)List_ptr_get(args, j)), out);
#line 277 "src/movecheck.tr"
            j = (j + 1LL);
        }
    } else if (_t1070.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1070.data.EPropAccess.obj;
        mv_idents(o, out);
    } else if (_t1070.tag == HirExpr_EIndex) {
        __auto_type o = _t1070.data.EIndex.obj;
__auto_type i2 = _t1070.data.EIndex._tr_v_index;
#line 280 "src/movecheck.tr"
        mv_idents(o, out);
#line 281 "src/movecheck.tr"
        mv_idents(i2, out);
    } else if (_t1070.tag == HirExpr_ECast) {
        __auto_type x = _t1070.data.ECast.expr;
        mv_idents(x, out);
    } else if (_t1070.tag == HirExpr_EFString) {
        __auto_type parts = _t1070.data.EFString.parts;
#line 284 "src/movecheck.tr"
        long long k = 0LL;
#line 285 "src/movecheck.tr"
        while ((k < parts->len)) {
#line 286 "src/movecheck.tr"
            if (((HirFStringPart*)List_ptr_get(parts, k))->is_expr) {
#line 286 "src/movecheck.tr"
                mv_idents(((HirFStringPart*)List_ptr_get(parts, k))->expr, out);
            }
#line 287 "src/movecheck.tr"
            k = (k + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1070;
        /* pass */
    }
}

__attribute__((hot)) TrStr mv_strip(TrStr n) {
#line 293 "src/movecheck.tr"
    return _tr_str_retain(n);
}

__attribute__((hot)) List_i64* mv_all_mask(long long n, long long v) {
#line 297 "src/movecheck.tr"
    List_i64* m = (void*)List_i64_new();
#line 298 "src/movecheck.tr"
    long long i = 0LL;
#line 299 "src/movecheck.tr"
    while ((i < n)) {
#line 300 "src/movecheck.tr"
        List_i64_append(m, v);
#line 301 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 302 "src/movecheck.tr"
    return m;
}

__attribute__((hot)) List_i64* mv_sig_mask(MvCtx* cx, TrStr key, long long n, bool skip_self) {
#line 305 "src/movecheck.tr"
    if ((_tr_dict_contains(cx->amb, _tr_strz(key)) || (!_tr_dict_contains(cx->sigs, _tr_strz(key))))) {
#line 305 "src/movecheck.tr"
        return mv_all_mask(n, 0LL);
    }
#line 306 "src/movecheck.tr"
    MvSig* sg = ((MvSig*)(uintptr_t)_tr_dict_get(cx->sigs, _tr_strz(key)));
#line 307 "src/movecheck.tr"
    HirFunction* f = _tr_obj_retain(sg->f);
#line 308 "src/movecheck.tr"
    if (f->is_extern) {
#line 308 "src/movecheck.tr"
        _tr_obj_release(f, _trdrop_HirFunction);
        return mv_all_mask(n, 0LL);
    }
#line 309 "src/movecheck.tr"
    long long off = 0LL;
#line 310 "src/movecheck.tr"
    if (((skip_self && (f->params->len > 0LL)) && _tr_str_eqv((((HirParam*)List_ptr_get(f->params, 0LL))->name), (_tr_str_lit_len("self", 4LL))))) {
#line 311 "src/movecheck.tr"
        off = 1LL;
    }
#line 312 "src/movecheck.tr"
    long long i = 0LL;
#line 313 "src/movecheck.tr"
    List_i64* out = (void*)List_i64_new();
#line 314 "src/movecheck.tr"
    while ((i < n)) {
#line 315 "src/movecheck.tr"
        long long pi = (i + off);
#line 316 "src/movecheck.tr"
        long long v = 0LL;
#line 317 "src/movecheck.tr"
        if ((pi < f->params->len)) {
#line 318 "src/movecheck.tr"
            AstType* pt = ((HirParam*)List_ptr_get(f->params, pi))->ty;
#line 319 "src/movecheck.tr"
            if (((!_tr_str_eqv((((HirParam*)List_ptr_get(f->params, pi))->name), (_tr_str_lit_len("self", 4LL)))) && (!pt->is_borrow))) {
#line 320 "src/movecheck.tr"
                bool generic = ({ __auto_type _ord_t1072 = (sg->gens); TrStr _apt_t1071 = _tr_str_retain(pt->name); __auto_type _wr = (mv_has(_ord_t1072, _apt_t1071)); _tr_str_release(_apt_t1071); _wr; });
#line 321 "src/movecheck.tr"
                if (generic) {
#line 322 "src/movecheck.tr"
                    v = 2LL;
                } else if ((!sg->lib)) {
#line 324 "src/movecheck.tr"
                    v = 1LL;
                }
            }
        }
#line 325 "src/movecheck.tr"
        List_i64_append(out, v);
#line 326 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 327 "src/movecheck.tr"
    _tr_obj_release(f, _trdrop_HirFunction);
    return out;
}

__attribute__((hot)) bool mv_coll_type(TrStr t) {
#line 330 "src/movecheck.tr"
    return (((((((((_tr_str_eqv((t), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((t), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Deque", 5LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Heap", 4LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Queue", 5LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("Stack", 5LL)))) || _tr_str_eqv((t), (_tr_str_lit_len("PriorityQueue", 13LL))));
}

__attribute__((hot)) bool mv_store_method(TrStr m) {
#line 333 "src/movecheck.tr"
    return (((((((((_tr_str_eqv((m), (_tr_str_lit_len("append", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_back", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_front", 10LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("enqueue", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("appendleft", 10LL))));
}

__attribute__((hot)) bool mv_ctor_type(TrStr t) {
#line 336 "src/movecheck.tr"
    return (_tr_str_eqv((t), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv((t), (_tr_str_lit_len("Box", 3LL))));
}

__attribute__((hot)) TrStr mv_callee_name(HirExpr* c) {
#line 339 "src/movecheck.tr"
    if (mv_null(c)) {
#line 339 "src/movecheck.tr"
        return _tr_str_lit_len("?", 1LL);
    }
#line 340 "src/movecheck.tr"
    __auto_type _t1073 = (*c);
    if (_t1073.tag == HirExpr_EIdent) {
        __auto_type n = _t1073.data.EIdent.name;
        TrStr _trret204 = _tr_str_retain(n);
        return _trret204;
    } else if (_t1073.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1073.data.EPropAccess.obj;
__auto_type p = _t1073.data.EPropAccess.prop;
#line 343 "src/movecheck.tr"
        if (mv_null(o)) {
#line 343 "src/movecheck.tr"
            TrStr _trret205 = _tr_str_retain(p);
            return _trret205;
        }
#line 344 "src/movecheck.tr"
        __auto_type _t1074 = (*o);
        if (_t1074.tag == HirExpr_EIdent) {
            __auto_type on = _t1074.data.EIdent.name;
            TrStr _trret206 = ({ TrStr _tr_cl = (_tr_strx_concatv((on), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
            return _trret206;
        } else if (_t1074.tag == HirExpr_EIndex) {
            __auto_type b = _t1074.data.EIndex.obj;
#line 347 "src/movecheck.tr"
            __auto_type _t1075 = (*b);
            if (_t1075.tag == HirExpr_EIdent) {
                __auto_type bn = _t1075.data.EIdent.name;
                TrStr _trret207 = ({ TrStr _tr_cl = (_tr_strx_concatv((bn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (p)); _tr_str_release(_tr_cl); _tr_cres; });
                return _trret207;
            } else if (1) {
                __auto_type _ = _t1075;
                TrStr _trret208 = _tr_str_retain(p);
                return _trret208;
            }
        } else if (1) {
            __auto_type _ = _t1074;
            TrStr _trret209 = _tr_str_retain(p);
            return _trret209;
        }
    } else if (1) {
        __auto_type _ = _t1073;
        return _tr_str_lit_len("?", 1LL);
    }
#line 352 "src/movecheck.tr"
    return _tr_str_lit_len("?", 1LL);
}

__attribute__((hot)) TrStr mv_static_base(MvCtx* cx, HirExpr* o) {
#line 355 "src/movecheck.tr"
    if (mv_null(o)) {
#line 355 "src/movecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 356 "src/movecheck.tr"
    __auto_type _t1076 = (*o);
    if (_t1076.tag == HirExpr_EIdent) {
        __auto_type nm = _t1076.data.EIdent.name;
#line 358 "src/movecheck.tr"
        if (mv_has(cx->declared, nm)) {
#line 358 "src/movecheck.tr"
            return _tr_str_lit_len("", 0LL);
        }
#line 359 "src/movecheck.tr"
        TrStr _trret210 = _tr_str_retain(nm);
        return _trret210;
    } else if (_t1076.tag == HirExpr_EIndex) {
        __auto_type b = _t1076.data.EIndex.obj;
#line 361 "src/movecheck.tr"
        return mv_static_base(cx, b);
    } else if (_t1076.tag == HirExpr_EMethodCall) {
        __auto_type b2 = _t1076.data.EMethodCall.obj;
__auto_type mm = _t1076.data.EMethodCall.method;
#line 363 "src/movecheck.tr"
        if (_tr_str_eqv((mm), (_tr_str_lit_len("get_index", 9LL)))) {
#line 363 "src/movecheck.tr"
            TrStr _trret211 = mv_static_base(cx, b2);
            return _trret211;
        }
#line 364 "src/movecheck.tr"
        return _tr_str_lit_len("", 0LL);
    } else if (1) {
        __auto_type _ = _t1076;
        return _tr_str_lit_len("", 0LL);
    }
#line 366 "src/movecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) List_i64* mv_mask_static(MvCtx* cx, TrStr tn, TrStr v, long long n) {
#line 369 "src/movecheck.tr"
    if (({ TrStr _dkt_t1077 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(cx->sigs, _tr_strz(_dkt_t1077))); _tr_str_release(_dkt_t1077); _wr; })) {
#line 369 "src/movecheck.tr"
        return ({ TrStr _at_t1078 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (mv_sig_mask(cx, _at_t1078, n, false)); _tr_str_release(_at_t1078); _wr; });
    }
#line 370 "src/movecheck.tr"
    if ((_tr_dict_contains(cx->old, _tr_strz(tn)) && ((bool)(uintptr_t)_tr_dict_get(cx->old, _tr_strz(tn))))) {
#line 371 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 372 "src/movecheck.tr"
    if (((_tr_str_eqv((tn), (_tr_str_lit_len("Thread", 6LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("ThreadPool", 10LL)))) && (((_tr_str_eqv((v), (_tr_str_lit_len("spawn", 5LL))) || _tr_str_eqv((v), (_tr_str_lit_len("new", 3LL)))) || _tr_str_eqv((v), (_tr_str_lit_len("submit", 6LL)))) || _tr_str_eqv((v), (_tr_str_lit_len("execute", 7LL)))))) {
#line 372 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 373 "src/movecheck.tr"
    if ((mv_ctor_type(tn) && (_tr_str_eqv((v), (_tr_str_lit_len("init", 4LL))) || _tr_str_eqv((v), (_tr_str_lit_len("new", 3LL)))))) {
#line 373 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 374 "src/movecheck.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("Option", 6LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Result", 6LL))))) {
#line 374 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 375 "src/movecheck.tr"
    return mv_all_mask(n, 0LL);
}

__attribute__((hot)) List_i64* mv_mask_method(MvCtx* cx, TrStr rt, TrStr m, long long n) {
#line 378 "src/movecheck.tr"
    if (({ TrStr _dkt_t1079 = (({ TrStr _tr_cl = (_tr_strx_concatv((rt), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(cx->sigs, _tr_strz(_dkt_t1079))); _tr_str_release(_dkt_t1079); _wr; })) {
#line 378 "src/movecheck.tr"
        return ({ TrStr _at_t1080 = (({ TrStr _tr_cl = (_tr_strx_concatv((rt), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (mv_sig_mask(cx, _at_t1080, n, true)); _tr_str_release(_at_t1080); _wr; });
    }
#line 379 "src/movecheck.tr"
    if ((mv_coll_type(rt) && mv_store_method(m))) {
#line 379 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 380 "src/movecheck.tr"
    if (((_tr_str_eqv((rt), (_tr_str_lit_len("Chan", 4LL))) || _tr_str_eqv((rt), (_tr_str_lit_len("Channel", 7LL)))) && ((_tr_str_eqv((m), (_tr_str_lit_len("send", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("try_send", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("send_timeout", 12LL)))))) {
#line 381 "src/movecheck.tr"
        List_i64* mm = mv_all_mask(n, 0LL);
#line 382 "src/movecheck.tr"
        if ((n > 0LL)) {
#line 382 "src/movecheck.tr"
            List_i64_set(mm, 0LL, 2LL);
        }
#line 383 "src/movecheck.tr"
        return mm;
    }
#line 384 "src/movecheck.tr"
    if (((_tr_str_eqv((rt), (_tr_str_lit_len("ThreadPool", 10LL))) || _tr_str_eqv((rt), (_tr_str_lit_len("Thread", 6LL)))) && ((_tr_str_eqv((m), (_tr_str_lit_len("submit", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("execute", 7LL)))))) {
#line 384 "src/movecheck.tr"
        return mv_all_mask(n, 2LL);
    }
#line 385 "src/movecheck.tr"
    return mv_all_mask(n, 0LL);
}

__attribute__((hot)) void mv_block(MvCtx* cx, HirBlock* b, MvState* st) {
#line 390 "src/movecheck.tr"
    List_i64_append(cx->sh_base, cx->sh_name->len);
#line 391 "src/movecheck.tr"
    long long i = 0LL;
#line 392 "src/movecheck.tr"
    while ((i < b->stmts->len)) {
#line 393 "src/movecheck.tr"
        mv_stmt(cx, ((HirStmt*)List_ptr_get(b->stmts, i)), st);
#line 394 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 396 "src/movecheck.tr"
    long long base = List_i64_get(cx->sh_base, (cx->sh_base->len - 1LL));
#line 397 "src/movecheck.tr"
    List_i64_pop(cx->sh_base);
#line 398 "src/movecheck.tr"
    while ((cx->sh_name->len > base)) {
#line 399 "src/movecheck.tr"
        long long k = (cx->sh_name->len - 1LL);
#line 400 "src/movecheck.tr"
        TrStr nm = List_TrStr_get(cx->sh_name, k);
#line 401 "src/movecheck.tr"
        long long bits = List_i64_get(cx->sh_bits, k);
#line 402 "src/movecheck.tr"
        List_TrStr_pop(cx->sh_name);
#line 403 "src/movecheck.tr"
        List_i64_pop(cx->sh_bits);
#line 404 "src/movecheck.tr"
        st->any = mv_without(st->any, nm);
#line 405 "src/movecheck.tr"
        st->all = mv_without(st->all, nm);
#line 406 "src/movecheck.tr"
        st->lp = mv_without(st->lp, nm);
#line 407 "src/movecheck.tr"
        if ((_tr_imod((long long)(bits), (long long)(2LL)) == 1LL)) {
#line 407 "src/movecheck.tr"
            List_TrStr_append(st->any, nm);
        }
#line 408 "src/movecheck.tr"
        if ((_tr_imod((long long)(_tr_idiv((long long)(bits), (long long)(2LL))), (long long)(2LL)) == 1LL)) {
#line 408 "src/movecheck.tr"
            List_TrStr_append(st->all, nm);
        }
#line 409 "src/movecheck.tr"
        if ((_tr_imod((long long)(_tr_idiv((long long)(bits), (long long)(4LL))), (long long)(2LL)) == 1LL)) {
#line 409 "src/movecheck.tr"
            List_TrStr_append(st->lp, nm);
        }
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void mv_exprs(MvCtx* cx, List_ptr* v, MvState* st) {
#line 412 "src/movecheck.tr"
    long long i = 0LL;
#line 413 "src/movecheck.tr"
    while ((i < v->len)) {
#line 414 "src/movecheck.tr"
        mv_expr(cx, ((HirExpr*)List_ptr_get(v, i)), st);
#line 415 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_branchy_block(MvCtx* cx, HirBlock* b, MvState* st) {
#line 418 "src/movecheck.tr"
    MvState* c = MvState_copy(st);
#line 419 "src/movecheck.tr"
    mv_block(cx, b, c);
#line 420 "src/movecheck.tr"
    mv_merge(st, c);
    _tr_obj_release(c, _trdrop_MvState);
}

__attribute__((hot)) void mv_consume_caps(MvCtx* cx, List_TrStr* caps, MvState* st) {
#line 424 "src/movecheck.tr"
    long long i = 0LL;
#line 425 "src/movecheck.tr"
    while ((i < caps->len)) {
#line 426 "src/movecheck.tr"
        ({ TrStr _at_t1081 = (List_TrStr_get(caps, i)); mv_use(cx, _at_t1081, st); _tr_str_release(_at_t1081); });
#line 427 "src/movecheck.tr"
        ({ TrStr _at_t1082 = (List_TrStr_get(caps, i)); mv_mark(cx, _at_t1082, st); _tr_str_release(_at_t1082); });
#line 428 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_TrStr* mv_closure_caps(MvCtx* cx, List_ptr* caps) {
#line 431 "src/movecheck.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 432 "src/movecheck.tr"
    long long i = 0LL;
#line 433 "src/movecheck.tr"
    while ((i < caps->len)) {
#line 434 "src/movecheck.tr"
        TrStr nm = _tr_str_retain(((HirParam*)List_ptr_get(caps, i))->name);
#line 435 "src/movecheck.tr"
        if ((mv_tracked(cx, nm) && mv_is_move(cx, ((HirParam*)List_ptr_get(caps, i))->ty))) {
#line 435 "src/movecheck.tr"
            List_TrStr_append(out, nm);
        }
#line 436 "src/movecheck.tr"
        i = (i + 1LL);
        _tr_str_release(nm);
    }
#line 437 "src/movecheck.tr"
    return out;
}

__attribute__((hot)) void mv_consume(MvCtx* cx, HirExpr* e, MvState* st, long long mode) {
#line 442 "src/movecheck.tr"
    if (mv_null(e)) {
#line 442 "src/movecheck.tr"
        return;
    }
#line 443 "src/movecheck.tr"
    __auto_type _t1083 = (*e);
    if (_t1083.tag == HirExpr_EIdent) {
        __auto_type nm = _t1083.data.EIdent.name;
__auto_type ty = _t1083.data.EIdent.ty;
#line 445 "src/movecheck.tr"
        mv_use(cx, nm, st);
#line 446 "src/movecheck.tr"
        if (mv_tracked(cx, nm)) {
#line 447 "src/movecheck.tr"
            if (mv_is_move(cx, ty)) {
#line 447 "src/movecheck.tr"
                mv_mark(cx, nm, st);
            } else if (((mode == 2LL) && _tr_dict_contains(cx->clcaps, _tr_strz(nm)))) {
#line 448 "src/movecheck.tr"
                mv_consume_caps(cx, ((List_TrStr*)(uintptr_t)_tr_dict_get(cx->clcaps, _tr_strz(nm))), st);
            }
        }
    } else if (_t1083.tag == HirExpr_EClosure) {
        __auto_type caps = _t1083.data.EClosure.captures;
#line 450 "src/movecheck.tr"
        mv_expr(cx, e, st);
#line 451 "src/movecheck.tr"
        if ((mode == 2LL)) {
#line 451 "src/movecheck.tr"
            mv_consume_caps(cx, mv_closure_caps(cx, caps), st);
        }
    } else if (_t1083.tag == HirExpr_ETuple) {
        __auto_type items = _t1083.data.ETuple.items;
#line 453 "src/movecheck.tr"
        long long i = 0LL;
#line 454 "src/movecheck.tr"
        while ((i < items->len)) {
#line 455 "src/movecheck.tr"
            mv_consume(cx, ((HirExpr*)List_ptr_get(items, i)), st, 2LL);
#line 456 "src/movecheck.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1083;
        mv_expr(cx, e, st);
    }
}

__attribute__((hot)) void mv_args(MvCtx* cx, List_ptr* args, List_i64* mask, MvState* st, TrStr cname) {
#line 460 "src/movecheck.tr"
    long long i = 0LL;
#line 461 "src/movecheck.tr"
    while ((i < args->len)) {
#line 462 "src/movecheck.tr"
        long long k = 0LL;
#line 463 "src/movecheck.tr"
        if ((i < mask->len)) {
#line 463 "src/movecheck.tr"
            k = List_i64_get(mask, i);
        }
#line 464 "src/movecheck.tr"
        if ((k == 0LL)) {
#line 464 "src/movecheck.tr"
            mv_expr(cx, ((HirExpr*)List_ptr_get(args, i)), st);
        } else {
#line 466 "src/movecheck.tr"
            TrStr _sfo_t1084 = cx->site;
            cx->site = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)((i + 1LL))))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("argument ", 9LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cname)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(_sfo_t1084);
#line 467 "src/movecheck.tr"
            mv_consume(cx, ((HirExpr*)List_ptr_get(args, i)), st, k);
        }
#line 468 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_items(MvCtx* cx, List_ptr* items, MvState* st) {
#line 471 "src/movecheck.tr"
    TrStr _sfo_t1085 = cx->site;
    cx->site = _tr_str_lit_len("a list/tuple/dict/set literal", 29LL);
    _tr_str_release(_sfo_t1085);
#line 472 "src/movecheck.tr"
    long long i = 0LL;
#line 473 "src/movecheck.tr"
    while ((i < items->len)) {
#line 474 "src/movecheck.tr"
        mv_consume(cx, ((HirExpr*)List_ptr_get(items, i)), st, 2LL);
#line 475 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_expr(MvCtx* cx, HirExpr* e, MvState* st) {
#line 478 "src/movecheck.tr"
    if (mv_null(e)) {
#line 478 "src/movecheck.tr"
        return;
    }
#line 479 "src/movecheck.tr"
    if (st->dead) {
#line 479 "src/movecheck.tr"
        return;
    }
#line 480 "src/movecheck.tr"
    __auto_type _t1086 = (*e);
    if (_t1086.tag == HirExpr_EIdent) {
        __auto_type nm = _t1086.data.EIdent.name;
        mv_use(cx, nm, st);
    } else if (_t1086.tag == HirExpr_EPropAccess) {
        __auto_type o = _t1086.data.EPropAccess.obj;
        mv_expr(cx, o, st);
    } else if (_t1086.tag == HirExpr_EBinOp) {
        __auto_type bop = _t1086.data.EBinOp.op;
__auto_type l = _t1086.data.EBinOp.left;
__auto_type r = _t1086.data.EBinOp.right;
#line 484 "src/movecheck.tr"
        mv_expr(cx, l, st);
#line 485 "src/movecheck.tr"
        if ((((_tr_str_eqv((bop), (_tr_str_lit_len("and", 3LL))) || _tr_str_eqv((bop), (_tr_str_lit_len("or", 2LL)))) || _tr_str_eqv((bop), (_tr_str_lit_len("&&", 2LL)))) || _tr_str_eqv((bop), (_tr_str_lit_len("||", 2LL))))) {
#line 487 "src/movecheck.tr"
            MvState* rs = MvState_copy(st);
#line 488 "src/movecheck.tr"
            mv_expr(cx, r, rs);
#line 489 "src/movecheck.tr"
            mv_merge(st, rs);
        } else {
#line 491 "src/movecheck.tr"
            mv_expr(cx, r, st);
        }
    } else if (_t1086.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t1086.data.EUnaryOp.expr;
        mv_expr(cx, x, st);
    } else if (_t1086.tag == HirExpr_ECall) {
        __auto_type callee = _t1086.data.ECall.callee;
__auto_type args = _t1086.data.ECall.args;
#line 494 "src/movecheck.tr"
        List_i64* mask = mv_all_mask(args->len, 0LL);
#line 495 "src/movecheck.tr"
        TrStr cname = mv_callee_name(callee);
#line 496 "src/movecheck.tr"
        if ((!mv_null(callee))) {
#line 497 "src/movecheck.tr"
            __auto_type _t1087 = (*callee);
            if (_t1087.tag == HirExpr_EIdent) {
                __auto_type fnm = _t1087.data.EIdent.name;
#line 499 "src/movecheck.tr"
                if ((((_tr_str_eqv((fnm), (_tr_str_lit_len("Some", 4LL))) || _tr_str_eqv((fnm), (_tr_str_lit_len("Ok", 2LL)))) || _tr_str_eqv((fnm), (_tr_str_lit_len("Err", 3LL)))) || _tr_str_eqv((fnm), (_tr_str_lit_len("spawn", 5LL))))) {
#line 500 "src/movecheck.tr"
                    mask = mv_all_mask(args->len, 2LL);
                } else if (mv_has(cx->declared, fnm)) {
#line 502 "src/movecheck.tr"
                    mv_use(cx, fnm, st);
                } else if (_tr_dict_contains(cx->sigs, _tr_strz(fnm))) {
#line 504 "src/movecheck.tr"
                    mask = mv_sig_mask(cx, fnm, args->len, false);
                } else if ((_tr_dict_contains(cx->mv, _tr_strz(fnm)) && _tr_dict_contains(cx->old, _tr_strz(fnm)))) {
#line 506 "src/movecheck.tr"
                    if (({ TrStr _dkt_t1088 = (_tr_strx_concatv((fnm), (_tr_str_lit_len(".init", 5LL)))); __auto_type _wr = (_tr_dict_contains(cx->sigs, _tr_strz(_dkt_t1088))); _tr_str_release(_dkt_t1088); _wr; })) {
#line 506 "src/movecheck.tr"
                        mask = ({ TrStr _at_t1089 = (_tr_strx_concatv((fnm), (_tr_str_lit_len(".init", 5LL)))); __auto_type _wr = (mv_sig_mask(cx, _at_t1089, args->len, false)); _tr_str_release(_at_t1089); _wr; });
                    } else {
#line 507 "src/movecheck.tr"
                        mask = mv_all_mask(args->len, 2LL);
                    }
                }
            } else if (_t1087.tag == HirExpr_EPropAccess) {
                __auto_type co = _t1087.data.EPropAccess.obj;
__auto_type cv = _t1087.data.EPropAccess.prop;
#line 509 "src/movecheck.tr"
                TrStr sb = mv_static_base(cx, co);
#line 510 "src/movecheck.tr"
                if ((!_tr_str_eqv((sb), (_tr_str_lit_len("", 0LL))))) {
#line 510 "src/movecheck.tr"
                    mask = mv_mask_static(cx, sb, cv, args->len);
                } else {
#line 512 "src/movecheck.tr"
                    mv_expr(cx, callee, st);
#line 513 "src/movecheck.tr"
                    mask = ({ TrStr _apt_t1090 = _tr_str_retain(hir_expr_type(co)->name); __auto_type _wr = (mv_mask_method(cx, _apt_t1090, cv, args->len)); _tr_str_release(_apt_t1090); _wr; });
                }
                _tr_str_release(sb);
            } else if (1) {
                __auto_type _ = _t1087;
                mv_expr(cx, callee, st);
            }
        }
#line 515 "src/movecheck.tr"
        mv_args(cx, args, mask, st, cname);
        _tr_str_release(cname);
    } else if (_t1086.tag == HirExpr_EMethodCall) {
        __auto_type o = _t1086.data.EMethodCall.obj;
__auto_type m = _t1086.data.EMethodCall.method;
__auto_type args = _t1086.data.EMethodCall.args;
#line 517 "src/movecheck.tr"
        TrStr cname2 = _tr_str_retain(m);
#line 518 "src/movecheck.tr"
        TrStr sb2 = mv_static_base(cx, o);
#line 519 "src/movecheck.tr"
        List_i64* mask2 = mv_all_mask(args->len, 0LL);
#line 520 "src/movecheck.tr"
        if (((!_tr_str_eqv((sb2), (_tr_str_lit_len("", 0LL)))) && (((((_tr_dict_contains(cx->mv, _tr_strz(sb2)) || mv_ctor_type(sb2)) || _tr_str_eqv((sb2), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((sb2), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((sb2), (_tr_str_lit_len("Option", 6LL)))) || _tr_str_eqv((sb2), (_tr_str_lit_len("Result", 6LL)))))) {
#line 521 "src/movecheck.tr"
            mask2 = mv_mask_static(cx, sb2, m, args->len);
        } else {
#line 523 "src/movecheck.tr"
            mv_expr(cx, o, st);
#line 524 "src/movecheck.tr"
            TrStr rt = _tr_str_retain(hir_expr_type(o)->name);
#line 525 "src/movecheck.tr"
            mask2 = mv_mask_method(cx, rt, m, args->len);
        }
#line 526 "src/movecheck.tr"
        mv_args(cx, args, mask2, st, cname2);
        _tr_str_release(cname2);
        _tr_str_release(sb2);
    } else if (_t1086.tag == HirExpr_EIndex) {
        __auto_type o = _t1086.data.EIndex.obj;
__auto_type i = _t1086.data.EIndex._tr_v_index;
#line 528 "src/movecheck.tr"
        mv_expr(cx, o, st);
#line 529 "src/movecheck.tr"
        mv_expr(cx, i, st);
    } else if (_t1086.tag == HirExpr_ECast) {
        __auto_type x = _t1086.data.ECast.expr;
        mv_expr(cx, x, st);
    } else if (_t1086.tag == HirExpr_EFString) {
        __auto_type parts = _t1086.data.EFString.parts;
#line 532 "src/movecheck.tr"
        long long pi = 0LL;
#line 533 "src/movecheck.tr"
        while ((pi < parts->len)) {
#line 534 "src/movecheck.tr"
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, pi));
#line 535 "src/movecheck.tr"
            if (part->is_expr) {
#line 536 "src/movecheck.tr"
                mv_expr(cx, part->expr, st);
#line 537 "src/movecheck.tr"
                mv_exprs(cx, part->spec_args, st);
            }
#line 538 "src/movecheck.tr"
            pi = (pi + 1LL);
        }
    } else if (_t1086.tag == HirExpr_ETryExpr) {
        __auto_type x = _t1086.data.ETryExpr.expr;
        mv_expr(cx, x, st);
    } else if (_t1086.tag == HirExpr_EClosure) {
        __auto_type caps = _t1086.data.EClosure.captures;
#line 541 "src/movecheck.tr"
        long long ci = 0LL;
#line 542 "src/movecheck.tr"
        while ((ci < caps->len)) {
#line 543 "src/movecheck.tr"
            ({ TrStr _apt_t1091 = _tr_str_retain(((HirParam*)List_ptr_get(caps, ci))->name); mv_use(cx, _apt_t1091, st); _tr_str_release(_apt_t1091); });
#line 544 "src/movecheck.tr"
            ci = (ci + 1LL);
        }
    } else if (_t1086.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t1086.data.ESuperMethodCall.args;
        mv_exprs(cx, args, st);
    } else if (_t1086.tag == HirExpr_EList) {
        __auto_type items = _t1086.data.EList.items;
        mv_items(cx, items, st);
    } else if (_t1086.tag == HirExpr_ESet) {
        __auto_type items = _t1086.data.ESet.items;
        mv_items(cx, items, st);
    } else if (_t1086.tag == HirExpr_ETuple) {
        __auto_type items = _t1086.data.ETuple.items;
        mv_items(cx, items, st);
    } else if (_t1086.tag == HirExpr_EDict) {
        __auto_type keys = _t1086.data.EDict.keys;
__auto_type vals = _t1086.data.EDict.vals;
#line 550 "src/movecheck.tr"
        mv_items(cx, keys, st);
#line 551 "src/movecheck.tr"
        mv_items(cx, vals, st);
    } else if (_t1086.tag == HirExpr_EListComp) {
        __auto_type el = _t1086.data.EListComp.element;
__auto_type gens = _t1086.data.EListComp.generators;
#line 553 "src/movecheck.tr"
        long long gi = 0LL;
#line 554 "src/movecheck.tr"
        while ((gi < gens->len)) {
#line 555 "src/movecheck.tr"
            mv_expr(cx, (*((HirComprehension**)List_ptr_get(gens, gi)))->iter, st);
#line 556 "src/movecheck.tr"
            gi = (gi + 1LL);
        }
    } else if (_t1086.tag == HirExpr_EGeneratorExpr) {
        __auto_type el2 = _t1086.data.EGeneratorExpr.element;
__auto_type gens2 = _t1086.data.EGeneratorExpr.generators;
#line 558 "src/movecheck.tr"
        long long gj = 0LL;
#line 559 "src/movecheck.tr"
        while ((gj < gens2->len)) {
#line 560 "src/movecheck.tr"
            mv_expr(cx, (*((HirComprehension**)List_ptr_get(gens2, gj)))->iter, st);
#line 561 "src/movecheck.tr"
            gj = (gj + 1LL);
        }
    } else if (_t1086.tag == HirExpr_ESlice) {
        __auto_type a = _t1086.data.ESlice.start;
__auto_type b = _t1086.data.ESlice.stop;
__auto_type c = _t1086.data.ESlice.step;
#line 563 "src/movecheck.tr"
        mv_expr(cx, a, st);
#line 564 "src/movecheck.tr"
        mv_expr(cx, b, st);
#line 565 "src/movecheck.tr"
        mv_expr(cx, c, st);
    } else if (_t1086.tag == HirExpr_EAwait) {
        __auto_type x = _t1086.data.EAwait.expr;
        mv_expr(cx, x, st);
    } else if (_t1086.tag == HirExpr_EAwaitTimeout) {
        __auto_type x = _t1086.data.EAwaitTimeout.expr;
__auto_type t = _t1086.data.EAwaitTimeout.timeout_ms;
#line 568 "src/movecheck.tr"
        mv_expr(cx, x, st);
#line 569 "src/movecheck.tr"
        mv_expr(cx, t, st);
    } else if (_t1086.tag == HirExpr_EYield) {
        __auto_type x = _t1086.data.EYield.expr;
        mv_expr(cx, x, st);
    } else if (_t1086.tag == HirExpr_ERange) {
        __auto_type a2 = _t1086.data.ERange.start;
__auto_type b2 = _t1086.data.ERange.end;
#line 572 "src/movecheck.tr"
        mv_expr(cx, a2, st);
#line 573 "src/movecheck.tr"
        mv_expr(cx, b2, st);
    } else if (_t1086.tag == HirExpr_EIfElse) {
        __auto_type c2 = _t1086.data.EIfElse.cond;
__auto_type t2 = _t1086.data.EIfElse.then_e;
__auto_type f2 = _t1086.data.EIfElse.else_e;
#line 575 "src/movecheck.tr"
        mv_expr(cx, c2, st);
#line 576 "src/movecheck.tr"
        MvState* s1 = MvState_copy(st);
#line 577 "src/movecheck.tr"
        MvState* s2 = MvState_copy(st);
#line 578 "src/movecheck.tr"
        mv_expr(cx, t2, s1);
#line 579 "src/movecheck.tr"
        mv_expr(cx, f2, s2);
#line 580 "src/movecheck.tr"
        s1->dead = false;
#line 581 "src/movecheck.tr"
        s2->dead = false;
#line 582 "src/movecheck.tr"
        mv_assign_from(st, s1);
#line 583 "src/movecheck.tr"
        mv_merge(st, s2);
        _tr_obj_release(s1, _trdrop_MvState);
        _tr_obj_release(s2, _trdrop_MvState);
    } else if (_t1086.tag == HirExpr_EDo) {
        __auto_type body = _t1086.data.EDo.body;
        mv_branchy_block(cx, body, st);
    } else if (_t1086.tag == HirExpr_ELoop) {
        __auto_type body2 = _t1086.data.ELoop.body;
        mv_branchy_block(cx, body2, st);
    } else if (_t1086.tag == HirExpr_EWhileExpr) {
        __auto_type c3 = _t1086.data.EWhileExpr.cond;
__auto_type body3 = _t1086.data.EWhileExpr.body;
__auto_type eb = _t1086.data.EWhileExpr.else_body;
#line 587 "src/movecheck.tr"
        mv_expr(cx, c3, st);
#line 588 "src/movecheck.tr"
        mv_branchy_block(cx, body3, st);
#line 589 "src/movecheck.tr"
        mv_branchy_block(cx, eb, st);
    } else if (_t1086.tag == HirExpr_EMatchExpr) {
        __auto_type subj = _t1086.data.EMatchExpr.subj;
__auto_type arms = _t1086.data.EMatchExpr.arms;
#line 591 "src/movecheck.tr"
        mv_expr(cx, subj, st);
#line 592 "src/movecheck.tr"
        bool first = true;
#line 593 "src/movecheck.tr"
        MvState* acc = MvState_init();
#line 594 "src/movecheck.tr"
        acc->dead = true;
#line 595 "src/movecheck.tr"
        long long ai = 0LL;
#line 596 "src/movecheck.tr"
        while ((ai < arms->len)) {
#line 597 "src/movecheck.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 598 "src/movecheck.tr"
            MvState* c4 = MvState_copy(st);
#line 599 "src/movecheck.tr"
            mv_pat(cx, a->pat, c4);
#line 600 "src/movecheck.tr"
            mv_expr(cx, a->guard, c4);
#line 601 "src/movecheck.tr"
            mv_block(cx, a->body, c4);
#line 602 "src/movecheck.tr"
            mv_merge(acc, c4);
#line 603 "src/movecheck.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c4, _trdrop_MvState);
        }
#line 604 "src/movecheck.tr"
        if ((arms->len > 0LL)) {
#line 604 "src/movecheck.tr"
            mv_assign_from(st, acc);
        }
        _tr_obj_release(acc, _trdrop_MvState);
    } else if (_t1086.tag == HirExpr_ETry) {
        __auto_type tb = _t1086.data.ETry.try_body;
__auto_type catches = _t1086.data.ETry.catches;
__auto_type fb = _t1086.data.ETry.finally_b;
#line 606 "src/movecheck.tr"
        mv_branchy_block(cx, tb, st);
#line 607 "src/movecheck.tr"
        long long ti = 0LL;
#line 608 "src/movecheck.tr"
        while ((ti < catches->len)) {
#line 609 "src/movecheck.tr"
            ({ TrStr _apt_t1092 = _tr_str_retain((*((HirCatchClause**)List_ptr_get(catches, ti)))->err_name); mv_declare(cx, _apt_t1092, st); _tr_str_release(_apt_t1092); });
#line 610 "src/movecheck.tr"
            mv_branchy_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ti)))->body, st);
#line 611 "src/movecheck.tr"
            ti = (ti + 1LL);
        }
#line 612 "src/movecheck.tr"
        mv_branchy_block(cx, fb, st);
    } else if (1) {
        __auto_type _ = _t1086;
        /* pass */
    }
}

__attribute__((hot)) void mv_pat_names(Pattern p, List_TrStr* out) {
#line 616 "src/movecheck.tr"
    __auto_type _t1093 = p;
    if (_t1093.tag == Pattern_PBind) {
        __auto_type n = _t1093.data.PBind.name;
        List_TrStr_append(out, n);
    } else if (_t1093.tag == Pattern_PVariantBind) {
        __auto_type f = _t1093.data.PVariantBind.field;
        List_TrStr_append(out, f);
    } else if (_t1093.tag == Pattern_PVariantBindMany) {
        __auto_type fs = _t1093.data.PVariantBindMany.fields;
#line 620 "src/movecheck.tr"
        long long i = 0LL;
#line 621 "src/movecheck.tr"
        while ((i < fs->len)) {
#line 622 "src/movecheck.tr"
            ({ TrStr _at_t1094 = (List_TrStr_get(fs, i)); List_TrStr_append(out, _at_t1094); _tr_str_release(_at_t1094); });
#line 623 "src/movecheck.tr"
            i = (i + 1LL);
        }
    } else if (_t1093.tag == Pattern_PTuple) {
        __auto_type a = _t1093.data.PTuple.first;
__auto_type b = _t1093.data.PTuple.second;
#line 625 "src/movecheck.tr"
        List_TrStr_append(out, a);
#line 626 "src/movecheck.tr"
        List_TrStr_append(out, b);
    } else if (_t1093.tag == Pattern_POr) {
        __auto_type ps = _t1093.data.POr.patterns;
#line 628 "src/movecheck.tr"
        long long j = 0LL;
#line 629 "src/movecheck.tr"
        while ((j < ps->len)) {
#line 630 "src/movecheck.tr"
            mv_pat_names(List_Pattern_get(ps, j), out);
#line 631 "src/movecheck.tr"
            j = (j + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t1093;
        /* pass */
    }
}

__attribute__((hot)) void mv_pat(MvCtx* cx, Pattern p, MvState* st) {
#line 635 "src/movecheck.tr"
    List_TrStr* names = (void*)List_TrStr_new();
#line 636 "src/movecheck.tr"
    mv_pat_names(p, names);
#line 637 "src/movecheck.tr"
    long long i = 0LL;
#line 638 "src/movecheck.tr"
    while ((i < names->len)) {
#line 639 "src/movecheck.tr"
        ({ TrStr _at_t1095 = (List_TrStr_get(names, i)); mv_declare(cx, _at_t1095, st); _tr_str_release(_at_t1095); });
#line 640 "src/movecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void mv_exit_loop_ctx(MvCtx* cx, MvState* brk_out, MvState* cnt_out) {
#line 645 "src/movecheck.tr"
    long long bb = List_i64_get(cx->brk_base, (cx->brk_base->len - 1LL));
#line 646 "src/movecheck.tr"
    List_i64_pop(cx->brk_base);
#line 647 "src/movecheck.tr"
    while ((cx->brk->len > bb)) {
#line 648 "src/movecheck.tr"
        mv_merge(brk_out, ((MvState*)List_ptr_get(cx->brk, (cx->brk->len - 1LL))));
#line 649 "src/movecheck.tr"
        ((MvState*)List_ptr_pop(cx->brk));
    }
#line 650 "src/movecheck.tr"
    long long cb = List_i64_get(cx->cnt_base, (cx->cnt_base->len - 1LL));
#line 651 "src/movecheck.tr"
    List_i64_pop(cx->cnt_base);
#line 652 "src/movecheck.tr"
    while ((cx->cnt->len > cb)) {
#line 653 "src/movecheck.tr"
        mv_merge(cnt_out, ((MvState*)List_ptr_get(cx->cnt, (cx->cnt->len - 1LL))));
#line 654 "src/movecheck.tr"
        ((MvState*)List_ptr_pop(cx->cnt));
    }
}

__attribute__((hot)) void mv_loop(MvCtx* cx, MvState* st, HirExpr* cond, List_TrStr* lvs, HirBlock* body, bool always) {
#line 657 "src/movecheck.tr"
    MvState* entry = MvState_copy(st);
#line 658 "src/movecheck.tr"
    MvState* head = MvState_copy(entry);
#line 659 "src/movecheck.tr"
    long long pass_i = 0LL;
#line 660 "src/movecheck.tr"
    MvState* exit_st = MvState_init();
#line 661 "src/movecheck.tr"
    exit_st->dead = true;
#line 662 "src/movecheck.tr"
    while ((pass_i < 2LL)) {
#line 663 "src/movecheck.tr"
        MvState* s = MvState_copy(head);
#line 664 "src/movecheck.tr"
        mv_expr(cx, cond, s);
#line 665 "src/movecheck.tr"
        MvState* after_cond = MvState_copy(s);
#line 666 "src/movecheck.tr"
        long long li = 0LL;
#line 667 "src/movecheck.tr"
        while ((li < lvs->len)) {
#line 668 "src/movecheck.tr"
            ({ TrStr _at_t1096 = (List_TrStr_get(lvs, li)); mv_declare(cx, _at_t1096, s); _tr_str_release(_at_t1096); });
#line 669 "src/movecheck.tr"
            li = (li + 1LL);
        }
#line 670 "src/movecheck.tr"
        List_i64_append(cx->brk_base, cx->brk->len);
#line 671 "src/movecheck.tr"
        List_i64_append(cx->cnt_base, cx->cnt->len);
#line 672 "src/movecheck.tr"
        mv_block(cx, body, s);
#line 673 "src/movecheck.tr"
        MvState* bo = MvState_init();
#line 674 "src/movecheck.tr"
        bo->dead = true;
#line 675 "src/movecheck.tr"
        MvState* co = MvState_init();
#line 676 "src/movecheck.tr"
        co->dead = true;
#line 677 "src/movecheck.tr"
        mv_exit_loop_ctx(cx, bo, co);
#line 678 "src/movecheck.tr"
        MvState* back = MvState_copy(s);
#line 679 "src/movecheck.tr"
        mv_merge(back, co);
#line 680 "src/movecheck.tr"
        if ((pass_i == 0LL)) {
#line 681 "src/movecheck.tr"
            MvState* _cltmp_t1097 = MvState_copy(entry);
            _tr_obj_release(head, _trdrop_MvState);
            head = _cltmp_t1097;
#line 682 "src/movecheck.tr"
            if ((!back->dead)) {
#line 683 "src/movecheck.tr"
                long long bi = 0LL;
#line 684 "src/movecheck.tr"
                while ((bi < back->any->len)) {
#line 685 "src/movecheck.tr"
                    TrStr bn = List_TrStr_get(back->any, bi);
#line 686 "src/movecheck.tr"
                    if ((!mv_has(entry->any, bn))) {
#line 686 "src/movecheck.tr"
                        mv_add(head->lp, bn);
                    }
#line 687 "src/movecheck.tr"
                    bi = (bi + 1LL);
                    _tr_str_release(bn);
                }
            }
#line 688 "src/movecheck.tr"
            mv_merge(head, back);
        } else {
#line 690 "src/movecheck.tr"
            if ((!always)) {
#line 690 "src/movecheck.tr"
                mv_merge(exit_st, after_cond);
            }
#line 691 "src/movecheck.tr"
            mv_merge(exit_st, bo);
        }
#line 692 "src/movecheck.tr"
        pass_i = (pass_i + 1LL);
        _tr_obj_release(s, _trdrop_MvState);
        _tr_obj_release(after_cond, _trdrop_MvState);
        _tr_obj_release(bo, _trdrop_MvState);
        _tr_obj_release(co, _trdrop_MvState);
        _tr_obj_release(back, _trdrop_MvState);
    }
#line 693 "src/movecheck.tr"
    mv_assign_from(st, exit_st);
    _tr_obj_release(entry, _trdrop_MvState);
    _tr_obj_release(head, _trdrop_MvState);
    _tr_obj_release(exit_st, _trdrop_MvState);
}

__attribute__((hot)) void mv_bind_rhs(MvCtx* cx, HirExpr* val, MvState* st) {
#line 698 "src/movecheck.tr"
    if (mv_null(val)) {
#line 698 "src/movecheck.tr"
        return;
    }
#line 699 "src/movecheck.tr"
    TrStr _sfo_t1098 = cx->site;
    cx->site = _tr_str_lit_len("a binding transfer (`b = a`)", 28LL);
    _tr_str_release(_sfo_t1098);
#line 700 "src/movecheck.tr"
    __auto_type _t1099 = (*val);
    if (_t1099.tag == HirExpr_EIdent) {
        __auto_type nm = _t1099.data.EIdent.name;
__auto_type ty = _t1099.data.EIdent.ty;
#line 702 "src/movecheck.tr"
        mv_use(cx, nm, st);
#line 703 "src/movecheck.tr"
        if (((mv_tracked(cx, nm) && mv_is_move(cx, ty)) && (!_tr_dict_contains(cx->old, _tr_strz(ty->name))))) {
#line 703 "src/movecheck.tr"
            mv_mark(cx, nm, st);
        }
    } else if (1) {
        __auto_type _ = _t1099;
        mv_expr(cx, val, st);
    }
}

__attribute__((hot)) void mv_note_closure(MvCtx* cx, TrStr nm, HirExpr* val) {
#line 707 "src/movecheck.tr"
    if (mv_null(val)) {
#line 707 "src/movecheck.tr"
        return;
    }
#line 708 "src/movecheck.tr"
    __auto_type _t1100 = (*val);
    if (_t1100.tag == HirExpr_EClosure) {
        __auto_type caps = _t1100.data.EClosure.captures;
#line 710 "src/movecheck.tr"
        ({ __auto_type _ord_t1101 = (mv_closure_caps(cx, caps)); _tr_dict_set(cx->clcaps, _tr_strz(nm), _ord_t1101); });
    } else if (1) {
        __auto_type _ = _t1100;
        /* pass */
    }
}

__attribute__((hot)) void mv_stmt(MvCtx* cx, HirStmt* sp, MvState* st) {
#line 714 "src/movecheck.tr"
    if (st->dead) {
#line 714 "src/movecheck.tr"
        return;
    }
#line 715 "src/movecheck.tr"
    __auto_type _t1102 = (*sp);
    if (_t1102.tag == HirStmt_SLineMarker) {
        __auto_type n = _t1102.data.SLineMarker.n;
        cx->line = n;
    } else if (_t1102.tag == HirStmt_SExpr) {
        __auto_type e = _t1102.data.SExpr.expr;
        mv_expr(cx, e, st);
    } else if (_t1102.tag == HirStmt_SLet) {
        __auto_type nm = _t1102.data.SLet.name;
__auto_type ty = _t1102.data.SLet.ty;
__auto_type val = _t1102.data.SLet.val;
#line 719 "src/movecheck.tr"
        if (ty->is_borrow) {
#line 719 "src/movecheck.tr"
            mv_expr(cx, val, st);
        } else {
#line 720 "src/movecheck.tr"
            mv_bind_rhs(cx, val, st);
        }
#line 721 "src/movecheck.tr"
        if ((mv_has(cx->declared, nm) && (cx->sh_base->len > 1LL))) {
#line 722 "src/movecheck.tr"
            long long bits = 0LL;
#line 723 "src/movecheck.tr"
            if (mv_has(st->any, nm)) {
#line 723 "src/movecheck.tr"
                bits = (bits + 1LL);
            }
#line 724 "src/movecheck.tr"
            if (mv_has(st->all, nm)) {
#line 724 "src/movecheck.tr"
                bits = (bits + 2LL);
            }
#line 725 "src/movecheck.tr"
            if (mv_has(st->lp, nm)) {
#line 725 "src/movecheck.tr"
                bits = (bits + 4LL);
            }
#line 726 "src/movecheck.tr"
            List_TrStr_append(cx->sh_name, nm);
#line 727 "src/movecheck.tr"
            List_i64_append(cx->sh_bits, bits);
        }
#line 728 "src/movecheck.tr"
        mv_declare(cx, nm, st);
#line 729 "src/movecheck.tr"
        mv_note_closure(cx, nm, val);
    } else if (_t1102.tag == HirStmt_SMultiLet) {
        __auto_type names = _t1102.data.SMultiLet.names;
__auto_type val = _t1102.data.SMultiLet.val;
#line 731 "src/movecheck.tr"
        mv_expr(cx, val, st);
#line 732 "src/movecheck.tr"
        long long ni = 0LL;
#line 733 "src/movecheck.tr"
        while ((ni < names->len)) {
#line 734 "src/movecheck.tr"
            ({ TrStr _at_t1103 = (List_TrStr_get(names, ni)); mv_declare(cx, _at_t1103, st); _tr_str_release(_at_t1103); });
#line 735 "src/movecheck.tr"
            ni = (ni + 1LL);
        }
    } else if (_t1102.tag == HirStmt_SAssign) {
        __auto_type tgt = _t1102.data.SAssign.target;
__auto_type val = _t1102.data.SAssign.val;
#line 737 "src/movecheck.tr"
        if (mv_null(tgt)) {
#line 738 "src/movecheck.tr"
            mv_expr(cx, val, st);
        } else {
#line 740 "src/movecheck.tr"
            __auto_type _t1104 = (*tgt);
            if (_t1104.tag == HirExpr_EIdent) {
                __auto_type tn = _t1104.data.EIdent.name;
#line 742 "src/movecheck.tr"
                mv_bind_rhs(cx, val, st);
#line 743 "src/movecheck.tr"
                if (mv_tracked(cx, tn)) {
#line 744 "src/movecheck.tr"
                    mv_reinit(cx, tn, st);
#line 745 "src/movecheck.tr"
                    mv_note_closure(cx, tn, val);
                }
            } else if (_t1104.tag == HirExpr_ETuple) {
                __auto_type titems = _t1104.data.ETuple.items;
#line 748 "src/movecheck.tr"
                mv_expr(cx, val, st);
#line 749 "src/movecheck.tr"
                long long tti = 0LL;
#line 750 "src/movecheck.tr"
                while ((tti < titems->len)) {
#line 751 "src/movecheck.tr"
                    if ((!mv_null(((HirExpr*)List_ptr_get(titems, tti))))) {
#line 752 "src/movecheck.tr"
                        __auto_type _t1105 = (*((HirExpr*)List_ptr_get(titems, tti)));
                        if (_t1105.tag == HirExpr_EIdent) {
                            __auto_type tin = _t1105.data.EIdent.name;
                            mv_reinit(cx, tin, st);
                        } else if (1) {
                            __auto_type _ = _t1105;
                            mv_expr(cx, ((HirExpr*)List_ptr_get(titems, tti)), st);
                        }
                    }
#line 755 "src/movecheck.tr"
                    tti = (tti + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t1104;
#line 758 "src/movecheck.tr"
                mv_expr(cx, tgt, st);
#line 759 "src/movecheck.tr"
                TrStr _sfo_t1106 = cx->site;
                cx->site = _tr_str_lit_len("a field/element store", 21LL);
                _tr_str_release(_sfo_t1106);
#line 760 "src/movecheck.tr"
                mv_consume(cx, val, st, 2LL);
            }
        }
    } else if (_t1102.tag == HirStmt_SReturn) {
        __auto_type v = _t1102.data.SReturn.val;
#line 762 "src/movecheck.tr"
        if (mv_null(v)) {
#line 762 "src/movecheck.tr"
            /* pass */
        } else {
#line 764 "src/movecheck.tr"
            __auto_type _t1107 = (*v);
            if (_t1107.tag == HirExpr_EClosure) {
                __auto_type caps = _t1107.data.EClosure.captures;
#line 766 "src/movecheck.tr"
                mv_expr(cx, v, st);
#line 767 "src/movecheck.tr"
                mv_consume_caps(cx, mv_closure_caps(cx, caps), st);
            } else if (1) {
                __auto_type _ = _t1107;
#line 769 "src/movecheck.tr"
                TrStr _sfo_t1108 = cx->site;
                cx->site = _tr_str_lit_len("a return", 8LL);
                _tr_str_release(_sfo_t1108);
#line 770 "src/movecheck.tr"
                mv_consume(cx, v, st, 2LL);
            }
        }
#line 771 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1102.tag == HirStmt_SRaise) {
        __auto_type v = _t1102.data.SRaise.val;
#line 773 "src/movecheck.tr"
        mv_expr(cx, v, st);
#line 774 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1102.tag == HirStmt_SDefer) {
        __auto_type ds = _t1102.data.SDefer.stmt;
#line 776 "src/movecheck.tr"
        List_TrStr* ids = (void*)List_TrStr_new();
#line 777 "src/movecheck.tr"
        __auto_type _t1109 = (*ds);
        if (_t1109.tag == HirStmt_SExpr) {
            __auto_type de = _t1109.data.SExpr.expr;
            mv_idents(de, ids);
        } else if (_t1109.tag == HirStmt_SAssign) {
            __auto_type dt = _t1109.data.SAssign.target;
__auto_type dv = _t1109.data.SAssign.val;
#line 780 "src/movecheck.tr"
            mv_idents(dt, ids);
#line 781 "src/movecheck.tr"
            mv_idents(dv, ids);
        } else if (1) {
            __auto_type _ = _t1109;
            /* pass */
        }
#line 783 "src/movecheck.tr"
        long long di = 0LL;
#line 784 "src/movecheck.tr"
        while ((di < ids->len)) {
#line 785 "src/movecheck.tr"
            if (({ TrStr _at_t1110 = (List_TrStr_get(ids, di)); __auto_type _wr = (mv_tracked(cx, _at_t1110)); _tr_str_release(_at_t1110); _wr; })) {
#line 785 "src/movecheck.tr"
                ({ __auto_type _ord_t1112 = (cx->defers); TrStr _at_t1111 = (List_TrStr_get(ids, di)); mv_add(_ord_t1112, _at_t1111); _tr_str_release(_at_t1111); });
            }
#line 786 "src/movecheck.tr"
            di = (di + 1LL);
        }
    } else if (_t1102.tag == HirStmt_SBreak) {
        __auto_type v = _t1102.data.SBreak.val;
#line 788 "src/movecheck.tr"
        mv_expr(cx, v, st);
#line 789 "src/movecheck.tr"
        if ((cx->brk_base->len > 0LL)) {
#line 789 "src/movecheck.tr"
            ({ MvState* _ord_t1113 = (MvState_copy(st)); List_ptr_append(cx->brk, _tr_obj_retain(_ord_t1113)); _tr_obj_release(_ord_t1113, _trdrop_MvState); });
        }
#line 790 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1102.tag == HirStmt_SContinue) {
#line 792 "src/movecheck.tr"
        if ((cx->cnt_base->len > 0LL)) {
#line 792 "src/movecheck.tr"
            ({ MvState* _ord_t1114 = (MvState_copy(st)); List_ptr_append(cx->cnt, _tr_obj_retain(_ord_t1114)); _tr_obj_release(_ord_t1114, _trdrop_MvState); });
        }
#line 793 "src/movecheck.tr"
        st->dead = true;
    } else if (_t1102.tag == HirStmt_SUnsafe) {
        __auto_type body = _t1102.data.SUnsafe.body;
#line 795 "src/movecheck.tr"
        cx->unsafe_depth = (cx->unsafe_depth + 1LL);
#line 796 "src/movecheck.tr"
        mv_block(cx, body, st);
#line 797 "src/movecheck.tr"
        cx->unsafe_depth = (cx->unsafe_depth - 1LL);
    } else if (_t1102.tag == HirStmt_SIf) {
        __auto_type cond = _t1102.data.SIf.cond;
__auto_type tb = _t1102.data.SIf.then_b;
__auto_type eb = _t1102.data.SIf.else_b;
#line 799 "src/movecheck.tr"
        mv_expr(cx, cond, st);
#line 800 "src/movecheck.tr"
        MvState* s1 = MvState_copy(st);
#line 801 "src/movecheck.tr"
        MvState* s2 = MvState_copy(st);
#line 802 "src/movecheck.tr"
        mv_block(cx, tb, s1);
#line 803 "src/movecheck.tr"
        mv_block(cx, eb, s2);
#line 804 "src/movecheck.tr"
        mv_assign_from(st, s1);
#line 805 "src/movecheck.tr"
        mv_merge(st, s2);
        _tr_obj_release(s1, _trdrop_MvState);
        _tr_obj_release(s2, _trdrop_MvState);
    } else if (_t1102.tag == HirStmt_SWhile) {
        __auto_type cond = _t1102.data.SWhile.cond;
__auto_type body = _t1102.data.SWhile.body;
#line 807 "src/movecheck.tr"
        bool always = false;
#line 808 "src/movecheck.tr"
        if ((!mv_null(cond))) {
#line 809 "src/movecheck.tr"
            __auto_type _t1115 = (*cond);
            if (_t1115.tag == HirExpr_ELitBool) {
                __auto_type bv = _t1115.data.ELitBool.val;
                always = bv;
            } else if (1) {
                __auto_type _ = _t1115;
                /* pass */
            }
        }
#line 812 "src/movecheck.tr"
        mv_loop(cx, st, cond, (void*)List_TrStr_new(), body, always);
    } else if (_t1102.tag == HirStmt_SFor) {
        __auto_type var = _t1102.data.SFor.var;
__auto_type it = _t1102.data.SFor.iter;
__auto_type body = _t1102.data.SFor.body;
#line 814 "src/movecheck.tr"
        mv_expr(cx, it, st);
#line 815 "src/movecheck.tr"
        List_TrStr* lv = (void*)List_TrStr_new();
#line 816 "src/movecheck.tr"
        List_TrStr_append(lv, var);
#line 817 "src/movecheck.tr"
        mv_add(cx->declared, var);
#line 818 "src/movecheck.tr"
        mv_loop(cx, st, (HirExpr*)(0LL), lv, body, false);
    } else if (_t1102.tag == HirStmt_SForUnpack) {
        __auto_type vars = _t1102.data.SForUnpack.vars;
__auto_type it = _t1102.data.SForUnpack.iter;
__auto_type body = _t1102.data.SForUnpack.body;
#line 820 "src/movecheck.tr"
        mv_expr(cx, it, st);
#line 821 "src/movecheck.tr"
        long long vi = 0LL;
#line 822 "src/movecheck.tr"
        while ((vi < vars->len)) {
#line 823 "src/movecheck.tr"
            ({ __auto_type _ord_t1117 = (cx->declared); TrStr _at_t1116 = (List_TrStr_get(vars, vi)); mv_add(_ord_t1117, _at_t1116); _tr_str_release(_at_t1116); });
#line 824 "src/movecheck.tr"
            vi = (vi + 1LL);
        }
#line 825 "src/movecheck.tr"
        mv_loop(cx, st, (HirExpr*)(0LL), vars, body, false);
    } else if (_t1102.tag == HirStmt_SMatch) {
        __auto_type subj = _t1102.data.SMatch.expr;
__auto_type arms = _t1102.data.SMatch.arms;
#line 827 "src/movecheck.tr"
        mv_expr(cx, subj, st);
#line 828 "src/movecheck.tr"
        MvState* acc = MvState_init();
#line 829 "src/movecheck.tr"
        acc->dead = true;
#line 830 "src/movecheck.tr"
        long long ai = 0LL;
#line 831 "src/movecheck.tr"
        while ((ai < arms->len)) {
#line 832 "src/movecheck.tr"
            HirMatchArm* a = ((HirMatchArm*)List_ptr_get(arms, ai));
#line 833 "src/movecheck.tr"
            MvState* c = MvState_copy(st);
#line 834 "src/movecheck.tr"
            mv_pat(cx, a->pat, c);
#line 835 "src/movecheck.tr"
            mv_expr(cx, a->guard, c);
#line 836 "src/movecheck.tr"
            mv_block(cx, a->body, c);
#line 837 "src/movecheck.tr"
            mv_merge(acc, c);
#line 838 "src/movecheck.tr"
            ai = (ai + 1LL);
            _tr_obj_release(c, _trdrop_MvState);
        }
#line 839 "src/movecheck.tr"
        if ((arms->len > 0LL)) {
#line 839 "src/movecheck.tr"
            mv_assign_from(st, acc);
        }
        _tr_obj_release(acc, _trdrop_MvState);
    } else if (_t1102.tag == HirStmt_STry) {
        __auto_type tb = _t1102.data.STry.try_body;
__auto_type catches = _t1102.data.STry.catches;
__auto_type fb = _t1102.data.STry.finally_b;
#line 841 "src/movecheck.tr"
        MvState* entry = MvState_copy(st);
#line 842 "src/movecheck.tr"
        MvState* s1 = MvState_copy(st);
#line 843 "src/movecheck.tr"
        List_TrStr* outer_acc = cx->try_acc;
#line 844 "src/movecheck.tr"
        cx->try_acc = (void*)List_TrStr_new();
#line 845 "src/movecheck.tr"
        cx->try_depth = (cx->try_depth + 1LL);
#line 846 "src/movecheck.tr"
        mv_block(cx, tb, s1);
#line 847 "src/movecheck.tr"
        cx->try_depth = (cx->try_depth - 1LL);
#line 848 "src/movecheck.tr"
        List_TrStr* moved_in_try = cx->try_acc;
#line 849 "src/movecheck.tr"
        cx->try_acc = outer_acc;
#line 850 "src/movecheck.tr"
        long long tai = 0LL;
#line 851 "src/movecheck.tr"
        while ((tai < moved_in_try->len)) {
#line 852 "src/movecheck.tr"
            if ((cx->try_depth > 0LL)) {
#line 852 "src/movecheck.tr"
                ({ __auto_type _ord_t1119 = (cx->try_acc); TrStr _at_t1118 = (List_TrStr_get(moved_in_try, tai)); mv_add(_ord_t1119, _at_t1118); _tr_str_release(_at_t1118); });
            }
#line 853 "src/movecheck.tr"
            tai = (tai + 1LL);
        }
#line 854 "src/movecheck.tr"
        MvState* acc = MvState_copy(s1);
#line 855 "src/movecheck.tr"
        MvState* hs = MvState_copy(entry);
#line 856 "src/movecheck.tr"
        mv_merge(hs, s1);
#line 857 "src/movecheck.tr"
        hs->dead = false;
#line 859 "src/movecheck.tr"
        tai = 0LL;
#line 860 "src/movecheck.tr"
        while ((tai < moved_in_try->len)) {
#line 861 "src/movecheck.tr"
            ({ __auto_type _ord_t1121 = (hs->any); TrStr _at_t1120 = (List_TrStr_get(moved_in_try, tai)); mv_add(_ord_t1121, _at_t1120); _tr_str_release(_at_t1120); });
#line 862 "src/movecheck.tr"
            tai = (tai + 1LL);
        }
#line 863 "src/movecheck.tr"
        long long ci = 0LL;
#line 864 "src/movecheck.tr"
        while ((ci < catches->len)) {
#line 865 "src/movecheck.tr"
            MvState* cs = MvState_copy(hs);
#line 866 "src/movecheck.tr"
            ({ TrStr _apt_t1122 = _tr_str_retain((*((HirCatchClause**)List_ptr_get(catches, ci)))->err_name); mv_declare(cx, _apt_t1122, cs); _tr_str_release(_apt_t1122); });
#line 867 "src/movecheck.tr"
            mv_block(cx, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body, cs);
#line 868 "src/movecheck.tr"
            mv_merge(acc, cs);
#line 869 "src/movecheck.tr"
            ci = (ci + 1LL);
            _tr_obj_release(cs, _trdrop_MvState);
        }
#line 870 "src/movecheck.tr"
        mv_block(cx, fb, acc);
#line 871 "src/movecheck.tr"
        mv_assign_from(st, acc);
        _tr_obj_release(entry, _trdrop_MvState);
        _tr_obj_release(s1, _trdrop_MvState);
        _tr_obj_release(acc, _trdrop_MvState);
        _tr_obj_release(hs, _trdrop_MvState);
    } else if (_t1102.tag == HirStmt_SAssert) {
        __auto_type c = _t1102.data.SAssert.cond;
__auto_type m = _t1102.data.SAssert.msg;
#line 873 "src/movecheck.tr"
        mv_expr(cx, c, st);
#line 874 "src/movecheck.tr"
        mv_expr(cx, m, st);
    } else if (_t1102.tag == HirStmt_SWith) {
        __auto_type items = _t1102.data.SWith.items;
__auto_type aliases = _t1102.data.SWith.aliases;
__auto_type body = _t1102.data.SWith.body;
#line 876 "src/movecheck.tr"
        mv_exprs(cx, items, st);
#line 877 "src/movecheck.tr"
        long long wi = 0LL;
#line 878 "src/movecheck.tr"
        while ((wi < aliases->len)) {
#line 879 "src/movecheck.tr"
            ({ TrStr _at_t1123 = (List_TrStr_get(aliases, wi)); mv_declare(cx, _at_t1123, st); _tr_str_release(_at_t1123); });
#line 880 "src/movecheck.tr"
            wi = (wi + 1LL);
        }
#line 881 "src/movecheck.tr"
        mv_block(cx, body, st);
    } else if (_t1102.tag == HirStmt_SSpawn) {
        __auto_type e = _t1102.data.SSpawn.expr;
#line 883 "src/movecheck.tr"
        if (mv_null(e)) {
#line 883 "src/movecheck.tr"
            /* pass */
        } else {
#line 885 "src/movecheck.tr"
            __auto_type _t1124 = (*e);
            if (_t1124.tag == HirExpr_ECall) {
                __auto_type sc = _t1124.data.ECall.callee;
__auto_type sargs = _t1124.data.ECall.args;
#line 887 "src/movecheck.tr"
                mv_expr(cx, sc, st);
#line 888 "src/movecheck.tr"
                mv_args(cx, sargs, mv_all_mask(sargs->len, 2LL), st, _tr_str_lit_len("spawn", 5LL));
            } else if (_t1124.tag == HirExpr_EClosure) {
                __auto_type caps = _t1124.data.EClosure.captures;
#line 890 "src/movecheck.tr"
                mv_expr(cx, e, st);
#line 891 "src/movecheck.tr"
                mv_consume_caps(cx, mv_closure_caps(cx, caps), st);
            } else if (1) {
                __auto_type _ = _t1124;
                mv_expr(cx, e, st);
            }
        }
    } else if (_t1102.tag == HirStmt_STaskGroup) {
        __auto_type body = _t1102.data.STaskGroup.body;
        mv_block(cx, body, st);
    } else if (_t1102.tag == HirStmt_SGpuBlock) {
        __auto_type body = _t1102.data.SGpuBlock.body;
        mv_block(cx, body, st);
    } else if (1) {
        __auto_type _ = _t1102;
        /* pass */
    }
}

__attribute__((hot)) List_TrStr* mv_run_fn(MvCtx* cx, HirFunction* hf) {
#line 898 "src/movecheck.tr"
    cx->errs = (void*)List_TrStr_new();
#line 899 "src/movecheck.tr"
    TrStr _sfo_t1125 = cx->fn_name;
    cx->fn_name = _tr_str_retain(hf->name);
    _tr_str_release(_sfo_t1125);
#line 900 "src/movecheck.tr"
    cx->declared = (void*)List_TrStr_new();
#line 901 "src/movecheck.tr"
    cx->mline = _tr_dict_new(4LL);
#line 902 "src/movecheck.tr"
    cx->clcaps = _tr_dict_new(2LL);
#line 903 "src/movecheck.tr"
    cx->defers = (void*)List_TrStr_new();
#line 904 "src/movecheck.tr"
    cx->brk = (void*)List_ptr_new();
#line 905 "src/movecheck.tr"
    cx->brk_base = (void*)List_i64_new();
#line 906 "src/movecheck.tr"
    cx->cnt = (void*)List_ptr_new();
#line 907 "src/movecheck.tr"
    cx->cnt_base = (void*)List_i64_new();
#line 908 "src/movecheck.tr"
    cx->unsafe_depth = 0LL;
#line 909 "src/movecheck.tr"
    cx->in_defer = false;
#line 910 "src/movecheck.tr"
    cx->line = 0LL;
#line 911 "src/movecheck.tr"
    cx->sh_name = (void*)List_TrStr_new();
#line 912 "src/movecheck.tr"
    cx->sh_bits = (void*)List_i64_new();
#line 913 "src/movecheck.tr"
    cx->sh_base = (void*)List_i64_new();
#line 914 "src/movecheck.tr"
    cx->try_acc = (void*)List_TrStr_new();
#line 915 "src/movecheck.tr"
    cx->try_depth = 0LL;
#line 916 "src/movecheck.tr"
    long long pi = 0LL;
#line 917 "src/movecheck.tr"
    while ((pi < hf->params->len)) {
#line 918 "src/movecheck.tr"
        ({ __auto_type _ord_t1127 = (cx->declared); TrStr _apt_t1126 = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name); mv_add(_ord_t1127, _apt_t1126); _tr_str_release(_apt_t1126); });
#line 919 "src/movecheck.tr"
        pi = (pi + 1LL);
    }
#line 920 "src/movecheck.tr"
    MvState* st = MvState_init();
#line 921 "src/movecheck.tr"
    mv_block(cx, hf->body, st);
#line 922 "src/movecheck.tr"
    _tr_obj_release(st, _trdrop_MvState);
    return cx->errs;
}

__attribute__((hot)) void mv_add_sig(MvCtx* cx, HirFunction* f, TrStr key, List_TrStr* cgens) {
#line 925 "src/movecheck.tr"
    if (_tr_dict_contains(cx->sigs, _tr_strz(key))) {
#line 927 "src/movecheck.tr"
        HirFunction* ex = _tr_obj_retain(((MvSig*)(uintptr_t)_tr_dict_get(cx->sigs, _tr_strz(key)))->f);
#line 928 "src/movecheck.tr"
        bool same = (ex->params->len == f->params->len);
#line 929 "src/movecheck.tr"
        long long pj = 0LL;
#line 930 "src/movecheck.tr"
        while ((same && (pj < f->params->len))) {
#line 931 "src/movecheck.tr"
            if ((({ __auto_type _ord_t1128 = (((HirParam*)List_ptr_get(ex->params, pj))->ty->name); __auto_type _wr = ((!_tr_str_eqv((_ord_t1128), (((HirParam*)List_ptr_get(f->params, pj))->ty->name)))); _wr; }) || ({ __auto_type _ord_t1129 = (((HirParam*)List_ptr_get(ex->params, pj))->ty->is_borrow); __auto_type _wr = ((_ord_t1129 != ((HirParam*)List_ptr_get(f->params, pj))->ty->is_borrow)); _wr; }))) {
#line 931 "src/movecheck.tr"
                same = false;
            }
#line 932 "src/movecheck.tr"
            pj = (pj + 1LL);
        }
#line 933 "src/movecheck.tr"
        if ((!same)) {
#line 933 "src/movecheck.tr"
            _tr_dict_set(cx->amb, _tr_strz(key), true);
        }
#line 934 "src/movecheck.tr"
        _tr_obj_release(ex, _trdrop_HirFunction);
        return;
    }
#line 935 "src/movecheck.tr"
    MvSig* sg = ((MvSig*)_tr_obj_alloc(sizeof(MvSig)));
#line 936 "src/movecheck.tr"
    HirFunction* _cltmp_t1130 = _tr_obj_retain(f);
    _tr_obj_release(sg->f, _trdrop_HirFunction);
    sg->f = _cltmp_t1130;
#line 937 "src/movecheck.tr"
    sg->lib = f->is_lib;
#line 938 "src/movecheck.tr"
    List_TrStr* gs = (void*)List_TrStr_new();
#line 939 "src/movecheck.tr"
    long long i = 0LL;
#line 940 "src/movecheck.tr"
    while ((i < f->generics->len)) {
#line 941 "src/movecheck.tr"
        ({ TrStr _at_t1131 = (List_TrStr_get(f->generics, i)); List_TrStr_append(gs, _at_t1131); _tr_str_release(_at_t1131); });
#line 942 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 943 "src/movecheck.tr"
    i = 0LL;
#line 944 "src/movecheck.tr"
    while ((i < cgens->len)) {
#line 945 "src/movecheck.tr"
        ({ TrStr _at_t1132 = (List_TrStr_get(cgens, i)); List_TrStr_append(gs, _at_t1132); _tr_str_release(_at_t1132); });
#line 946 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 947 "src/movecheck.tr"
    sg->gens = gs;
#line 948 "src/movecheck.tr"
    _tr_dict_set_obj(cx->sigs, _tr_strz(key), _tr_obj_retain(sg), _trdrop_MvSig);
    _tr_obj_release(sg, _trdrop_MvSig);
}

__attribute__((hot)) List_TrStr* movecheck_check(List_ptr* fns, HirProgram* hp, TrMap* mvt, TrMap* oldt) {
#line 954 "src/movecheck.tr"
    MvCtx* cx = MvCtx_init();
#line 955 "src/movecheck.tr"
    cx->mv = mvt;
#line 956 "src/movecheck.tr"
    cx->old = oldt;
#line 957 "src/movecheck.tr"
    List_TrStr* nogens = (void*)List_TrStr_new();
#line 958 "src/movecheck.tr"
    long long i = 0LL;
#line 959 "src/movecheck.tr"
    while ((i < hp->functions->len)) {
#line 960 "src/movecheck.tr"
        HirFunction* f = ((HirFunction*)List_ptr_get(hp->functions, i));
#line 961 "src/movecheck.tr"
        if (_tr_str_eqv((f->class_name), (_tr_str_lit_len("", 0LL)))) {
#line 961 "src/movecheck.tr"
            ({ TrStr _apt_t1133 = _tr_str_retain(f->name); mv_add_sig(cx, f, _apt_t1133, nogens); _tr_str_release(_apt_t1133); });
        } else {
#line 962 "src/movecheck.tr"
            ({ TrStr _at_t1134 = (({ TrStr _tr_cl = (_tr_strx_concatv((f->class_name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); mv_add_sig(cx, f, _at_t1134, nogens); _tr_str_release(_at_t1134); });
        }
#line 963 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 964 "src/movecheck.tr"
    i = 0LL;
#line 965 "src/movecheck.tr"
    while ((i < hp->classes->len)) {
#line 966 "src/movecheck.tr"
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, i));
#line 967 "src/movecheck.tr"
        long long j = 0LL;
#line 968 "src/movecheck.tr"
        while ((j < c->methods->len)) {
#line 969 "src/movecheck.tr"
            ({ __auto_type _ord_t1137 = (((HirFunction*)List_ptr_get(c->methods, j))); __auto_type _ord_t1135 = (_tr_strx_concatv((c->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1136 = (({ TrStr _tr_cl = (_ord_t1135); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(c->methods, j))->name)); _tr_str_release(_tr_cl); _tr_cres; })); mv_add_sig(cx, _ord_t1137, _at_t1136, c->generics); _tr_str_release(_at_t1136); });
#line 970 "src/movecheck.tr"
            j = (j + 1LL);
        }
#line 971 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 972 "src/movecheck.tr"
    i = 0LL;
#line 973 "src/movecheck.tr"
    while ((i < hp->enums->len)) {
#line 974 "src/movecheck.tr"
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, i));
#line 975 "src/movecheck.tr"
        long long j2 = 0LL;
#line 976 "src/movecheck.tr"
        while ((j2 < en->methods->len)) {
#line 977 "src/movecheck.tr"
            ({ __auto_type _ord_t1140 = (((HirFunction*)List_ptr_get(en->methods, j2))); __auto_type _ord_t1138 = (_tr_strx_concatv((en->name), (_tr_str_lit_len(".", 1LL)))); TrStr _at_t1139 = (({ TrStr _tr_cl = (_ord_t1138); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (((HirFunction*)List_ptr_get(en->methods, j2))->name)); _tr_str_release(_tr_cl); _tr_cres; })); mv_add_sig(cx, _ord_t1140, _at_t1139, en->generics); _tr_str_release(_at_t1139); });
#line 978 "src/movecheck.tr"
            j2 = (j2 + 1LL);
        }
#line 979 "src/movecheck.tr"
        i = (i + 1LL);
    }
#line 980 "src/movecheck.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 981 "src/movecheck.tr"
    long long fi = 0LL;
#line 982 "src/movecheck.tr"
    while ((fi < fns->len)) {
#line 983 "src/movecheck.tr"
        HirFunction* hf = ((HirFunction*)List_ptr_get(fns, fi));
#line 984 "src/movecheck.tr"
        if ((!hf->is_lib)) {
#line 985 "src/movecheck.tr"
            List_TrStr* errs = mv_run_fn(cx, hf);
#line 986 "src/movecheck.tr"
            long long ei = 0LL;
#line 987 "src/movecheck.tr"
            while ((ei < errs->len)) {
#line 988 "src/movecheck.tr"
                ({ __auto_type _ord_t1141 = (({ TrStr _tr_cl = (_tr_str_wrap(_tr_int_to_str((long long)(fi)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("@", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t1142 = (({ TrStr _tr_cl = (_ord_t1141); TrStr _tr_cr = (List_TrStr_get(errs, ei)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); List_TrStr_append(out, _at_t1142); _tr_str_release(_at_t1142); });
#line 989 "src/movecheck.tr"
                ei = (ei + 1LL);
            }
        }
#line 990 "src/movecheck.tr"
        fi = (fi + 1LL);
    }
#line 991 "src/movecheck.tr"
    _tr_obj_release(cx, _trdrop_MvCtx);
    return out;
}

