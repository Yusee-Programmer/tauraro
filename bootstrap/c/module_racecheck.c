#include "tauraro_types.h"

RcFn* _rc_new_fn(TrStr name, TrStr file, long long line, bool is_lib);
bool _rc_sync_ty(TrStr n);
bool _rc_mut_method(TrStr m);
bool _rc_has(List_TrStr* v, TrStr s);
bool _rc_nul(Expr* p);
TrStr _rc_root(Expr* e);
TrStr _rc_first_prop(Expr* e);
TrStr _rc_ctor_name(Expr* v);
bool _rc_is_lambda(Expr* v);
bool _rc_path_lib(TrStr p);
bool _rc_ifaces_sendable(ClassDef* c);

__attribute__((hot)) TrStr RcCk_where(RcCk* self, long long line) {
#line 150 "src/racecheck.tr"
    long long l = line;
#line 151 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 151 "src/racecheck.tr"
        l = self->cur->line;
    }
#line 152 "src/racecheck.tr"
    return ({ __auto_type _ord_t2941 = (_tr_strx_concatv((self->cur->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2941); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) TrStr RcCk_ltype(RcCk* self, RcFn* f, TrStr name) {
#line 155 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 156 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 157 "src/racecheck.tr"
        if (({ TrStr _cmpa_t2942 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t2943 = (name); _Bool _cmpr_t2944 = _tr_str_eqv(_cmpa_t2942, _cmpb_t2943); _tr_str_release(_cmpa_t2942); _cmpr_t2944; })) {
#line 157 "src/racecheck.tr"
            return List_TrStr_get(f->ptypes, i);
        }
#line 158 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 159 "src/racecheck.tr"
    i = (f->ltn->len - 1LL);
#line 160 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 161 "src/racecheck.tr"
        if (({ TrStr _cmpa_t2945 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t2946 = (name); _Bool _cmpr_t2947 = _tr_str_eqv(_cmpa_t2945, _cmpb_t2946); _tr_str_release(_cmpa_t2945); _cmpr_t2947; })) {
#line 161 "src/racecheck.tr"
            return List_TrStr_get(f->ltt, i);
        }
#line 162 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 163 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void RcCk_note(RcCk* self, TrStr name, bool w) {
#line 166 "src/racecheck.tr"
    if ((_rc_has(self->cur->locals, name) || _rc_has(self->cur->params, name))) {
#line 166 "src/racecheck.tr"
        return;
    }
#line 167 "src/racecheck.tr"
    List_TrStr_append(self->cur->touched, name);
#line 168 "src/racecheck.tr"
    List_bool_append(self->cur->touched_w, w);
#line 169 "src/racecheck.tr"
    List_bool_append(self->cur->touched_after, self->seen_spawn);
#line 170 "src/racecheck.tr"
    List_i64_append(self->cur->touched_line, self->cur_line);
}

__attribute__((hot)) void RcCk_wpath(RcCk* self, Expr* target) {
#line 173 "src/racecheck.tr"
    TrStr r = _rc_root(target);
#line 174 "src/racecheck.tr"
    if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 174 "src/racecheck.tr"
        _tr_str_release(r);
        return;
    }
#line 175 "src/racecheck.tr"
    List_TrStr_append(self->cur->wroot, r);
#line 176 "src/racecheck.tr"
    ({ TrStr _ord_t2948 = (_rc_first_prop(target)); List_TrStr_append(self->cur->wprop, _ord_t2948); _tr_str_release(_ord_t2948); });
#line 177 "src/racecheck.tr"
    RcCk_note(self, r, true);
    _tr_str_release(r);
}

__attribute__((hot)) void RcCk_block(RcCk* self, Block* b) {
#line 180 "src/racecheck.tr"
    long long i = 0LL;
#line 181 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 182 "src/racecheck.tr"
        RcCk_stmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
#line 183 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 184 "src/racecheck.tr"
    RcCk_lock_scan(self, b);
}

__attribute__((hot)) void RcCk_pblock(RcCk* self, Block** pb) {
#line 187 "src/racecheck.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 187 "src/racecheck.tr"
        return;
    }
#line 188 "src/racecheck.tr"
    RcCk_block(self, (*pb));
}

__attribute__((hot)) void RcCk_declare(RcCk* self, TrStr name) {
#line 191 "src/racecheck.tr"
    List_TrStr_append(self->cur->locals, name);
}

__attribute__((hot)) void RcCk_lock_scan(RcCk* self, Block* b) {
#line 195 "src/racecheck.tr"
    List_TrStr* names = (void*)List_TrStr_new();
#line 196 "src/racecheck.tr"
    List_i64* state = (void*)List_i64_new();
#line 197 "src/racecheck.tr"
    List_TrStr* kinds = (void*)List_TrStr_new();
#line 198 "src/racecheck.tr"
    long long line = self->cur->line;
#line 199 "src/racecheck.tr"
    long long i = 0LL;
#line 200 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 201 "src/racecheck.tr"
        Stmt* sp = ((Stmt*)List_ptr_get(b->stmts, i));
#line 202 "src/racecheck.tr"
        i = (i + 1LL);
#line 203 "src/racecheck.tr"
        Expr* call = (Expr*)(0LL);
#line 204 "src/racecheck.tr"
        bool simple = true;
#line 205 "src/racecheck.tr"
        bool is_line = false;
#line 206 "src/racecheck.tr"
        __auto_type _t2949 = (*sp);
        if (_t2949.tag == Stmt_SLine) {
            __auto_type n = _t2949.data.SLine.n;
#line 208 "src/racecheck.tr"
            line = n;
#line 209 "src/racecheck.tr"
            is_line = true;
        } else if (_t2949.tag == Stmt_SExpr) {
            __auto_type e = _t2949.data.SExpr.expr;
            call = e;
        } else if (_t2949.tag == Stmt_SLet) {
            __auto_type v = _t2949.data.SLet.val;
            call = v;
        } else if (_t2949.tag == Stmt_SAssign) {
            __auto_type v2 = _t2949.data.SAssign.val;
            call = v2;
        } else if (1) {
            __auto_type _ = _t2949;
            simple = false;
        }
#line 214 "src/racecheck.tr"
        if (is_line) {
#line 214 "src/racecheck.tr"
            continue;
        }
#line 215 "src/racecheck.tr"
        if ((!simple)) {
#line 216 "src/racecheck.tr"
            names = (void*)List_TrStr_new();
#line 217 "src/racecheck.tr"
            state = (void*)List_i64_new();
#line 218 "src/racecheck.tr"
            kinds = (void*)List_TrStr_new();
#line 219 "src/racecheck.tr"
            continue;
        }
#line 220 "src/racecheck.tr"
        if (_rc_nul(call)) {
#line 220 "src/racecheck.tr"
            continue;
        }
#line 221 "src/racecheck.tr"
        __auto_type _t2950 = (*call);
        if (_t2950.tag == Expr_EMethodCall) {
            __auto_type o = _t2950.data.EMethodCall.obj;
__auto_type m = _t2950.data.EMethodCall.method;
__auto_type margs = _t2950.data.EMethodCall.args;
#line 223 "src/racecheck.tr"
            TrStr r = _tr_str_lit_len("", 0LL);
#line 224 "src/racecheck.tr"
            if ((!_rc_nul(o))) {
#line 225 "src/racecheck.tr"
                __auto_type _t2951 = (*o);
                if (_t2951.tag == Expr_EIdent) {
                    __auto_type rn = _t2951.data.EIdent.name;
                    TrStr _strtmp_t2952 = _tr_str_retain(rn);
                    _tr_str_release(r);
                    r = _strtmp_t2952;
                } else if (1) {
                    __auto_type _ = _t2951;
                    /* pass */
                }
            }
#line 228 "src/racecheck.tr"
            if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 228 "src/racecheck.tr"
                _tr_str_release(r);
                continue;
            }
#line 229 "src/racecheck.tr"
            TrStr k = RcCk_ltype(self, self->cur, r);
#line 230 "src/racecheck.tr"
            if ((!((_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))))) {
#line 230 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                continue;
            }
#line 231 "src/racecheck.tr"
            if ((((margs->len != 0LL) && (!_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) && (!_tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL)))))) {
#line 231 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                continue;
            }
#line 232 "src/racecheck.tr"
            long long idx = (0LL - 1LL);
#line 233 "src/racecheck.tr"
            long long j = 0LL;
#line 234 "src/racecheck.tr"
            while ((j < names->len)) {
#line 235 "src/racecheck.tr"
                if (({ TrStr _cmpa_t2953 = (List_TrStr_get(names, j)); TrStr _cmpb_t2954 = (r); _Bool _cmpr_t2955 = _tr_str_eqv(_cmpa_t2953, _cmpb_t2954); _tr_str_release(_cmpa_t2953); _cmpr_t2955; })) {
#line 235 "src/racecheck.tr"
                    idx = j;
                }
#line 236 "src/racecheck.tr"
                j = (j + 1LL);
            }
#line 237 "src/racecheck.tr"
            if ((idx < 0LL)) {
#line 238 "src/racecheck.tr"
                List_TrStr_append(names, r);
#line 239 "src/racecheck.tr"
                List_i64_append(state, 0LL);
#line 240 "src/racecheck.tr"
                List_TrStr_append(kinds, k);
#line 241 "src/racecheck.tr"
                idx = (names->len - 1LL);
            }
#line 242 "src/racecheck.tr"
            long long st = List_i64_get(state, idx);
#line 243 "src/racecheck.tr"
            if (_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 244 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) {
#line 245 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 245 "src/racecheck.tr"
                        ({ TrStr _ord_t2956 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()' acquires the lock again while this thread already holds it (no `set()`/`unlock()` since the previous `get()`): the second call blocks forever.\n      FIX: finish with `", 175LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(v)` or `", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` before locking again, or reuse the first value.", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t2956); _tr_str_release(_ord_t2956); });
                    }
#line 246 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("unlock", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) {
#line 247 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 249 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL)))) {
#line 249 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("read_unlock", 11LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL))))) {
#line 250 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
                } else if (_tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) {
#line 252 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 252 "src/racecheck.tr"
                        ({ TrStr _ord_t2957 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()' takes the write lock while this thread still holds the read lock from `", 81LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read()`: the write waits for readers (itself included) to leave, which never happens (deadlock).\n      FIX: call `", 115LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read_unlock()` before `", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()`.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t2957); _tr_str_release(_ord_t2957); });
                    }
#line 253 "src/racecheck.tr"
                    List_i64_set(state, idx, 2LL);
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))) {
#line 255 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL)))) {
#line 256 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 256 "src/racecheck.tr"
                        ({ TrStr _ord_t2958 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] thread '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is joined twice: joining an already-joined thread handle is undefined behaviour.\n      FIX: join each thread exactly once.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t2958); _tr_str_release(_ord_t2958); });
                    }
#line 257 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                }
            }
            _tr_str_release(r);
            _tr_str_release(k);
        } else if (1) {
            __auto_type _ = _t2950;
            /* pass */
        }
    }
    List_TrStr_free(names);
    List_i64_free(state);
    List_TrStr_free(kinds);
}

__attribute__((hot)) void RcCk_stmt(RcCk* self, Stmt* sp) {
#line 261 "src/racecheck.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 261 "src/racecheck.tr"
        return;
    }
#line 262 "src/racecheck.tr"
    __auto_type _t2959 = (*sp);
    if (_t2959.tag == Stmt_SLine) {
        __auto_type n = _t2959.data.SLine.n;
        self->cur_line = n;
    } else if (_t2959.tag == Stmt_SExpr) {
        __auto_type e = _t2959.data.SExpr.expr;
        RcCk_expr(self, e);
    } else if (_t2959.tag == Stmt_SLet) {
        __auto_type name = _t2959.data.SLet.name;
__auto_type is_sh = _t2959.data.SLet.is_shared;
__auto_type ty = _t2959.data.SLet.ty;
__auto_type v = _t2959.data.SLet.val;
#line 266 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 267 "src/racecheck.tr"
        RcCk_declare(self, name);
#line 268 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 269 "src/racecheck.tr"
        if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 269 "src/racecheck.tr"
            TrStr _strtmp_t2960 = _tr_str_retain((*ty)->name);
            _tr_str_release(tn);
            tn = _strtmp_t2960;
        }
#line 270 "src/racecheck.tr"
        if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 270 "src/racecheck.tr"
            TrStr _strtmp_t2961 = _rc_ctor_name(v);
            _tr_str_release(tn);
            tn = _strtmp_t2961;
        }
#line 271 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltn, name);
#line 272 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltt, tn);
#line 273 "src/racecheck.tr"
        if (is_sh) {
#line 273 "src/racecheck.tr"
            List_TrStr_append(self->cur->shvars, name);
        }
#line 274 "src/racecheck.tr"
        if (_rc_is_lambda(v)) {
#line 274 "src/racecheck.tr"
            List_TrStr_append(self->cur->closures, name);
        }
        _tr_str_release(tn);
    } else if (_t2959.tag == Stmt_SMultiLet) {
        __auto_type names = _t2959.data.SMultiLet.names;
__auto_type v = _t2959.data.SMultiLet.val;
#line 276 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 277 "src/racecheck.tr"
        long long mi = 0LL;
#line 278 "src/racecheck.tr"
        while ((mi < names->len)) {
#line 279 "src/racecheck.tr"
            ({ TrStr _at_t2962 = (List_TrStr_get(names, mi)); RcCk_declare(self, _at_t2962); _tr_str_release(_at_t2962); });
#line 280 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t2959.tag == Stmt_SAssign) {
        __auto_type t = _t2959.data.SAssign.target;
__auto_type v = _t2959.data.SAssign.val;
#line 282 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 283 "src/racecheck.tr"
        RcCk_expr(self, t);
#line 284 "src/racecheck.tr"
        RcCk_wpath(self, t);
    } else if (_t2959.tag == Stmt_SReturn) {
        __auto_type v = _t2959.data.SReturn.val;
        RcCk_expr(self, v);
    } else if (_t2959.tag == Stmt_SBreak) {
        __auto_type v = _t2959.data.SBreak.val;
        RcCk_expr(self, v);
    } else if (_t2959.tag == Stmt_SRaise) {
        __auto_type v = _t2959.data.SRaise.val;
        RcCk_expr(self, v);
    } else if (_t2959.tag == Stmt_SUnsafe) {
        __auto_type b = _t2959.data.SUnsafe.body;
        RcCk_block(self, b);
    } else if (_t2959.tag == Stmt_SIf) {
        __auto_type c = _t2959.data.SIf.cond;
__auto_type tb = _t2959.data.SIf.then_b;
__auto_type elifs = _t2959.data.SIf.elifs;
__auto_type eb = _t2959.data.SIf.else_b;
#line 290 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 291 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 292 "src/racecheck.tr"
        long long ei = 0LL;
#line 293 "src/racecheck.tr"
        while ((ei < elifs->len)) {
#line 294 "src/racecheck.tr"
            RcCk_expr(self, ((ElifClause*)List_ptr_get(elifs, ei))->cond);
#line 295 "src/racecheck.tr"
            RcCk_pblock(self, ((ElifClause*)List_ptr_get(elifs, ei))->body);
#line 296 "src/racecheck.tr"
            ei = (ei + 1LL);
        }
#line 297 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (_t2959.tag == Stmt_SWhile) {
        __auto_type c = _t2959.data.SWhile.cond;
__auto_type b = _t2959.data.SWhile.body;
#line 299 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 300 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 301 "src/racecheck.tr"
        RcCk_block(self, b);
#line 302 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t2959.tag == Stmt_SFor) {
        __auto_type v = _t2959.data.SFor.var;
__auto_type it = _t2959.data.SFor.iter;
__auto_type b = _t2959.data.SFor.body;
#line 304 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 305 "src/racecheck.tr"
        RcCk_declare(self, v);
#line 306 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 307 "src/racecheck.tr"
        RcCk_block(self, b);
#line 308 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t2959.tag == Stmt_SForUnpack) {
        __auto_type vs = _t2959.data.SForUnpack.vars;
__auto_type it = _t2959.data.SForUnpack.iter;
__auto_type b = _t2959.data.SForUnpack.body;
#line 310 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 311 "src/racecheck.tr"
        long long fi = 0LL;
#line 312 "src/racecheck.tr"
        while ((fi < vs->len)) {
#line 313 "src/racecheck.tr"
            ({ TrStr _at_t2963 = (List_TrStr_get(vs, fi)); RcCk_declare(self, _at_t2963); _tr_str_release(_at_t2963); });
#line 314 "src/racecheck.tr"
            fi = (fi + 1LL);
        }
#line 315 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 316 "src/racecheck.tr"
        RcCk_block(self, b);
#line 317 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t2959.tag == Stmt_SMatch) {
        __auto_type e = _t2959.data.SMatch.expr;
__auto_type arms = _t2959.data.SMatch.arms;
#line 319 "src/racecheck.tr"
        RcCk_expr(self, e);
#line 320 "src/racecheck.tr"
        long long ai = 0LL;
#line 321 "src/racecheck.tr"
        while ((ai < arms->len)) {
#line 322 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, ai))->guard);
#line 323 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, ai))->body);
#line 324 "src/racecheck.tr"
            ai = (ai + 1LL);
        }
    } else if (_t2959.tag == Stmt_STry) {
        __auto_type tb = _t2959.data.STry.try_body;
__auto_type catches = _t2959.data.STry.catches;
__auto_type fb = _t2959.data.STry.finally_b;
#line 326 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 327 "src/racecheck.tr"
        long long ci = 0LL;
#line 328 "src/racecheck.tr"
        while ((ci < catches->len)) {
#line 329 "src/racecheck.tr"
            ({ TrStr _apt_t2964 = _tr_str_retain((*((CatchClause**)List_ptr_get(catches, ci)))->err_name); RcCk_declare(self, _apt_t2964); _tr_str_release(_apt_t2964); });
#line 330 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci)))->body);
#line 331 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 332 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t2959.tag == Stmt_SAssert) {
        __auto_type c = _t2959.data.SAssert.cond;
__auto_type m = _t2959.data.SAssert.msg;
#line 334 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 335 "src/racecheck.tr"
        RcCk_expr(self, m);
    } else if (_t2959.tag == Stmt_SWith) {
        __auto_type items = _t2959.data.SWith.items;
__auto_type aliases = _t2959.data.SWith.aliases;
__auto_type b = _t2959.data.SWith.body;
#line 337 "src/racecheck.tr"
        long long wi = 0LL;
#line 338 "src/racecheck.tr"
        while ((wi < items->len)) {
#line 339 "src/racecheck.tr"
            RcCk_expr(self, ((Expr*)List_ptr_get(items, wi)));
#line 340 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 341 "src/racecheck.tr"
        wi = 0LL;
#line 342 "src/racecheck.tr"
        while ((wi < aliases->len)) {
#line 343 "src/racecheck.tr"
            ({ TrStr _at_t2965 = (List_TrStr_get(aliases, wi)); RcCk_declare(self, _at_t2965); _tr_str_release(_at_t2965); });
#line 344 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 345 "src/racecheck.tr"
        RcCk_block(self, b);
    } else if (_t2959.tag == Stmt_SSpawn) {
        __auto_type e = _t2959.data.SSpawn.expr;
#line 347 "src/racecheck.tr"
        RcCk_spawn_expr(self, e);
    } else if (_t2959.tag == Stmt_STaskGroup) {
        __auto_type b = _t2959.data.STaskGroup.body;
        RcCk_block(self, b);
    } else if (_t2959.tag == Stmt_SGpuBlock) {
        __auto_type b = _t2959.data.SGpuBlock.body;
        RcCk_block(self, b);
    } else if (_t2959.tag == Stmt_SDefer) {
        __auto_type s = _t2959.data.SDefer.stmt;
        RcCk_stmt(self, s);
    } else if (_t2959.tag == Stmt_SLocalDecl) {
        __auto_type d = _t2959.data.SLocalDecl.decl;
#line 352 "src/racecheck.tr"
        __auto_type _t2966 = (*d);
        if (_t2966.tag == Decl_DFunction) {
            __auto_type f = _t2966.data.DFunction.func;
#line 354 "src/racecheck.tr"
            ({ TrStr _apt_t2967 = _tr_str_retain(self->cur->file); RcCk_add_fn(self, f, _apt_t2967, self->cur->is_lib); _tr_str_release(_apt_t2967); });
        } else if (1) {
            __auto_type _ = _t2966;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t2959;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_spawn_expr(RcCk* self, Expr* e) {
#line 360 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 360 "src/racecheck.tr"
        return;
    }
#line 361 "src/racecheck.tr"
    __auto_type _t2968 = (*e);
    if (_t2968.tag == Expr_ECall) {
        __auto_type c = _t2968.data.ECall.callee;
__auto_type args = _t2968.data.ECall.args;
#line 363 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 364 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 365 "src/racecheck.tr"
            __auto_type _t2969 = (*c);
            if (_t2969.tag == Expr_EIdent) {
                __auto_type n = _t2969.data.EIdent.name;
                TrStr _strtmp_t2970 = _tr_str_retain(n);
                _tr_str_release(tn);
                tn = _strtmp_t2970;
            } else if (1) {
                __auto_type _ = _t2969;
                /* pass */
            }
        }
#line 368 "src/racecheck.tr"
        RcCk_record_site(self, tn, args, 0LL);
#line 369 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(tn);
    } else if (1) {
        __auto_type _ = _t2968;
        RcCk_expr(self, e);
    }
}

__attribute__((hot)) void RcCk_record_site(RcCk* self, TrStr target, List_ptr* args, long long skip) {
#line 373 "src/racecheck.tr"
    RcSite* s = ((RcSite*)_tr_obj_alloc(sizeof(RcSite)));
#line 374 "src/racecheck.tr"
    s->target = _tr_str_retain(target);
#line 375 "src/racecheck.tr"
    s->args = (void*)List_ptr_new();
#line 376 "src/racecheck.tr"
    long long i = skip;
#line 377 "src/racecheck.tr"
    while ((i < args->len)) {
#line 378 "src/racecheck.tr"
        ({ __auto_type _ord_t2971 = (((Expr*)List_ptr_get(args, i))); List_ptr_append(s->args, _ord_t2971); });
#line 379 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 380 "src/racecheck.tr"
    s->line = self->cur_line;
#line 381 "src/racecheck.tr"
    s->in_loop = (self->loop_depth > 0LL);
#line 382 "src/racecheck.tr"
    List_ptr_append(self->cur->sites, _tr_obj_retain(s));
#line 383 "src/racecheck.tr"
    self->seen_spawn = true;
    _tr_obj_release(s, _trdrop_RcSite);
}

__attribute__((hot)) void RcCk_exprs(RcCk* self, List_ptr* v) {
#line 386 "src/racecheck.tr"
    long long i = 0LL;
#line 387 "src/racecheck.tr"
    while ((i < v->len)) {
#line 388 "src/racecheck.tr"
        RcCk_expr(self, ((Expr*)List_ptr_get(v, i)));
#line 389 "src/racecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void RcCk_comps(RcCk* self, Expr* element, List_ptr* gens) {
#line 392 "src/racecheck.tr"
    long long i = 0LL;
#line 393 "src/racecheck.tr"
    while ((i < gens->len)) {
#line 394 "src/racecheck.tr"
        ({ TrStr _apt_t2972 = _tr_str_retain((*((Comprehension**)List_ptr_get(gens, i)))->target); RcCk_declare(self, _apt_t2972); _tr_str_release(_apt_t2972); });
#line 395 "src/racecheck.tr"
        RcCk_expr(self, (*((Comprehension**)List_ptr_get(gens, i)))->iter);
#line 396 "src/racecheck.tr"
        RcCk_exprs(self, (*((Comprehension**)List_ptr_get(gens, i)))->ifs);
#line 397 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 398 "src/racecheck.tr"
    RcCk_expr(self, element);
}

__attribute__((hot)) void RcCk_expr(RcCk* self, Expr* e) {
#line 401 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 401 "src/racecheck.tr"
        return;
    }
#line 402 "src/racecheck.tr"
    __auto_type _t2973 = (*e);
    if (_t2973.tag == Expr_EIdent) {
        __auto_type n = _t2973.data.EIdent.name;
        RcCk_note(self, n, false);
    } else if (_t2973.tag == Expr_EBinOp) {
        __auto_type l = _t2973.data.EBinOp.left;
__auto_type r = _t2973.data.EBinOp.right;
#line 405 "src/racecheck.tr"
        RcCk_expr(self, l);
#line 406 "src/racecheck.tr"
        RcCk_expr(self, r);
    } else if (_t2973.tag == Expr_EUnaryOp) {
        __auto_type x = _t2973.data.EUnaryOp.expr;
        RcCk_expr(self, x);
    } else if (_t2973.tag == Expr_ECall) {
        __auto_type c = _t2973.data.ECall.callee;
__auto_type args = _t2973.data.ECall.args;
#line 409 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 410 "src/racecheck.tr"
            __auto_type _t2974 = (*c);
            if (_t2974.tag == Expr_EIdent) {
                __auto_type fnm = _t2974.data.EIdent.name;
#line 412 "src/racecheck.tr"
                List_TrStr_append(self->cur->calls, fnm);
#line 413 "src/racecheck.tr"
                if (_tr_str_eqv((fnm), (_tr_str_lit_len("await_all", 9LL)))) {
#line 414 "src/racecheck.tr"
                    long long ai = 0LL;
#line 415 "src/racecheck.tr"
                    while ((ai < args->len)) {
#line 416 "src/racecheck.tr"
                        __auto_type _t2975 = (*((Expr*)List_ptr_get(args, ai)));
                        if (_t2975.tag == Expr_ECall) {
                            __auto_type c2 = _t2975.data.ECall.callee;
__auto_type a2 = _t2975.data.ECall.args;
#line 418 "src/racecheck.tr"
                            TrStr t2 = _tr_str_lit_len("", 0LL);
#line 419 "src/racecheck.tr"
                            if ((!_rc_nul(c2))) {
#line 420 "src/racecheck.tr"
                                __auto_type _t2976 = (*c2);
                                if (_t2976.tag == Expr_EIdent) {
                                    __auto_type n2 = _t2976.data.EIdent.name;
                                    TrStr _strtmp_t2977 = _tr_str_retain(n2);
                                    _tr_str_release(t2);
                                    t2 = _strtmp_t2977;
                                } else if (1) {
                                    __auto_type _ = _t2976;
                                    /* pass */
                                }
                            }
#line 423 "src/racecheck.tr"
                            RcCk_record_site(self, t2, a2, 0LL);
                            _tr_str_release(t2);
                        } else if (1) {
                            __auto_type _ = _t2975;
                            /* pass */
                        }
#line 425 "src/racecheck.tr"
                        ai = (ai + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t2974;
                RcCk_expr(self, c);
            }
        }
#line 427 "src/racecheck.tr"
        RcCk_exprs(self, args);
    } else if (_t2973.tag == Expr_EMethodCall) {
        __auto_type o = _t2973.data.EMethodCall.obj;
__auto_type m = _t2973.data.EMethodCall.method;
__auto_type args = _t2973.data.EMethodCall.args;
#line 429 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 430 "src/racecheck.tr"
        RcCk_exprs(self, args);
#line 431 "src/racecheck.tr"
        TrStr _tr_v_recv = _rc_root(o);
#line 432 "src/racecheck.tr"
        TrStr direct = _tr_str_lit_len("", 0LL);
#line 433 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 434 "src/racecheck.tr"
            __auto_type _t2978 = (*o);
            if (_t2978.tag == Expr_EIdent) {
                __auto_type dn = _t2978.data.EIdent.name;
                TrStr _strtmp_t2979 = _tr_str_retain(dn);
                _tr_str_release(direct);
                direct = _strtmp_t2979;
            } else if (1) {
                __auto_type _ = _t2978;
                /* pass */
            }
        }
#line 437 "src/racecheck.tr"
        if ((!_tr_str_eqv((direct), (_tr_str_lit_len("", 0LL))))) {
#line 438 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcrecv, direct);
#line 439 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcname, m);
        }
#line 440 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && (args->len > 0LL))) {
#line 441 "src/racecheck.tr"
            TrStr t = _tr_str_lit_len("", 0LL);
#line 442 "src/racecheck.tr"
            __auto_type _t2980 = (*((Expr*)List_ptr_get(args, 0LL)));
            if (_t2980.tag == Expr_EIdent) {
                __auto_type tn2 = _t2980.data.EIdent.name;
                TrStr _strtmp_t2981 = _tr_str_retain(tn2);
                _tr_str_release(t);
                t = _strtmp_t2981;
            } else if (1) {
                __auto_type _ = _t2980;
                /* pass */
            }
#line 445 "src/racecheck.tr"
            if ((!_tr_str_eqv((t), (_tr_str_lit_len("", 0LL))))) {
#line 445 "src/racecheck.tr"
                RcCk_record_site(self, t, args, 1LL);
            }
        }
#line 446 "src/racecheck.tr"
        if (((!_tr_str_eqv((_tr_v_recv), (_tr_str_lit_len("", 0LL)))) && _rc_mut_method(m))) {
#line 447 "src/racecheck.tr"
            List_TrStr_append(self->cur->wroot, _tr_v_recv);
#line 448 "src/racecheck.tr"
            ({ TrStr _ord_t2982 = (_rc_first_prop(o)); List_TrStr_append(self->cur->wprop, _ord_t2982); _tr_str_release(_ord_t2982); });
#line 449 "src/racecheck.tr"
            RcCk_note(self, _tr_v_recv, true);
        }
        _tr_str_release(_tr_v_recv);
        _tr_str_release(direct);
    } else if (_t2973.tag == Expr_EPropAccess) {
        __auto_type o = _t2973.data.EPropAccess.obj;
        RcCk_expr(self, o);
    } else if (_t2973.tag == Expr_EIndex) {
        __auto_type o = _t2973.data.EIndex.obj;
__auto_type i = _t2973.data.EIndex._tr_v_index;
#line 452 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 453 "src/racecheck.tr"
        RcCk_expr(self, i);
    } else if (_t2973.tag == Expr_ECast) {
        __auto_type x = _t2973.data.ECast.expr;
        RcCk_expr(self, x);
    } else if (_t2973.tag == Expr_EFString) {
        __auto_type parts = _t2973.data.EFString.parts;
#line 456 "src/racecheck.tr"
        long long pi = 0LL;
#line 457 "src/racecheck.tr"
        while ((pi < parts->len)) {
#line 458 "src/racecheck.tr"
            if (((FStringPart*)List_ptr_get(parts, pi))->is_expr) {
#line 458 "src/racecheck.tr"
                RcCk_expr(self, ((FStringPart*)List_ptr_get(parts, pi))->expr);
            }
#line 459 "src/racecheck.tr"
            pi = (pi + 1LL);
        }
    } else if (_t2973.tag == Expr_ETryExpr) {
        __auto_type x = _t2973.data.ETryExpr.expr;
        RcCk_expr(self, x);
    } else if (_t2973.tag == Expr_EClosure) {
        __auto_type params = _t2973.data.EClosure.params;
__auto_type body = _t2973.data.EClosure.body;
#line 462 "src/racecheck.tr"
        long long ci = 0LL;
#line 463 "src/racecheck.tr"
        while ((ci < params->len)) {
#line 464 "src/racecheck.tr"
            ({ TrStr _apt_t2983 = _tr_str_retain(((Param*)List_ptr_get(params, ci))->name); RcCk_declare(self, _apt_t2983); _tr_str_release(_apt_t2983); });
#line 465 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 466 "src/racecheck.tr"
        RcCk_block(self, body);
    } else if (_t2973.tag == Expr_ESuperMethodCall) {
        __auto_type args = _t2973.data.ESuperMethodCall.args;
        RcCk_exprs(self, args);
    } else if (_t2973.tag == Expr_EList) {
        __auto_type items = _t2973.data.EList.items;
        RcCk_exprs(self, items);
    } else if (_t2973.tag == Expr_ESet) {
        __auto_type items = _t2973.data.ESet.items;
        RcCk_exprs(self, items);
    } else if (_t2973.tag == Expr_ETuple) {
        __auto_type items = _t2973.data.ETuple.items;
        RcCk_exprs(self, items);
    } else if (_t2973.tag == Expr_EDict) {
        __auto_type ks = _t2973.data.EDict.keys;
__auto_type vs = _t2973.data.EDict.vals;
#line 472 "src/racecheck.tr"
        RcCk_exprs(self, ks);
#line 473 "src/racecheck.tr"
        RcCk_exprs(self, vs);
    } else if (_t2973.tag == Expr_EListComp) {
        __auto_type el = _t2973.data.EListComp.element;
__auto_type gens = _t2973.data.EListComp.generators;
        RcCk_comps(self, el, gens);
    } else if (_t2973.tag == Expr_EGeneratorExpr) {
        __auto_type el = _t2973.data.EGeneratorExpr.element;
__auto_type gens = _t2973.data.EGeneratorExpr.generators;
        RcCk_comps(self, el, gens);
    } else if (_t2973.tag == Expr_ESlice) {
        __auto_type a = _t2973.data.ESlice.start;
__auto_type b = _t2973.data.ESlice.stop;
__auto_type c = _t2973.data.ESlice.step;
#line 477 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 478 "src/racecheck.tr"
        RcCk_expr(self, b);
#line 479 "src/racecheck.tr"
        RcCk_expr(self, c);
    } else if (_t2973.tag == Expr_EAwait) {
        __auto_type x = _t2973.data.EAwait.expr;
        RcCk_expr(self, x);
    } else if (_t2973.tag == Expr_EYield) {
        __auto_type x = _t2973.data.EYield.expr;
        RcCk_expr(self, x);
    } else if (_t2973.tag == Expr_ETry) {
        __auto_type tb = _t2973.data.ETry.try_body;
__auto_type catches = _t2973.data.ETry.catches;
__auto_type fb = _t2973.data.ETry.finally_b;
#line 483 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 484 "src/racecheck.tr"
        long long ci2 = 0LL;
#line 485 "src/racecheck.tr"
        while ((ci2 < catches->len)) {
#line 486 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci2)))->body);
#line 487 "src/racecheck.tr"
            ci2 = (ci2 + 1LL);
        }
#line 488 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t2973.tag == Expr_ERange) {
        __auto_type a = _t2973.data.ERange.start;
__auto_type b = _t2973.data.ERange.end;
#line 490 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 491 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t2973.tag == Expr_EIfElse) {
        __auto_type c = _t2973.data.EIfElse.cond;
__auto_type a = _t2973.data.EIfElse.then_expr;
__auto_type b = _t2973.data.EIfElse.else_expr;
#line 493 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 494 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 495 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t2973.tag == Expr_EDo) {
        __auto_type b = _t2973.data.EDo.body;
        RcCk_block(self, b);
    } else if (_t2973.tag == Expr_EMatch) {
        __auto_type s = _t2973.data.EMatch.subj;
__auto_type arms = _t2973.data.EMatch.arms;
#line 498 "src/racecheck.tr"
        RcCk_expr(self, s);
#line 499 "src/racecheck.tr"
        long long mi = 0LL;
#line 500 "src/racecheck.tr"
        while ((mi < arms->len)) {
#line 501 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, mi))->guard);
#line 502 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, mi))->body);
#line 503 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t2973.tag == Expr_EMacroCall) {
        __auto_type args = _t2973.data.EMacroCall.args;
        RcCk_exprs(self, args);
    } else if (_t2973.tag == Expr_ELoop) {
        __auto_type b = _t2973.data.ELoop.body;
        RcCk_block(self, b);
    } else if (_t2973.tag == Expr_EWhileExpr) {
        __auto_type c = _t2973.data.EWhileExpr.cond;
__auto_type b = _t2973.data.EWhileExpr.body;
__auto_type eb = _t2973.data.EWhileExpr.else_body;
#line 507 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 508 "src/racecheck.tr"
        RcCk_block(self, b);
#line 509 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (1) {
        __auto_type _ = _t2973;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_add_method(RcCk* self, FunctionDef* f, TrStr cls, TrStr file, bool lib) {
#line 513 "src/racecheck.tr"
    ({ TrStr _at_t2984 = (({ TrStr _tr_cl = (_tr_strx_concatv((cls), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_add_fn_named(self, f, _at_t2984, file, lib); _tr_str_release(_at_t2984); });
}

__attribute__((hot)) void RcCk_add_fn(RcCk* self, FunctionDef* f, TrStr file, bool lib) {
#line 516 "src/racecheck.tr"
    ({ TrStr _apt_t2985 = _tr_str_retain(f->name); RcCk_add_fn_named(self, f, _apt_t2985, file, lib); _tr_str_release(_apt_t2985); });
}

__attribute__((hot)) void RcCk_add_fn_named(RcCk* self, FunctionDef* f, TrStr nm, TrStr file, bool lib) {
#line 519 "src/racecheck.tr"
    RcFn* rf = _rc_new_fn(nm, file, f->line, (lib || f->is_lib));
#line 520 "src/racecheck.tr"
    long long pi = 0LL;
#line 521 "src/racecheck.tr"
    while ((pi < f->params->len)) {
#line 522 "src/racecheck.tr"
        ({ __auto_type _ord_t2986 = (((Param*)List_ptr_get(f->params, pi))->name); List_TrStr_append(rf->params, _ord_t2986); });
#line 523 "src/racecheck.tr"
        AstType** pt = ((Param*)List_ptr_get(f->params, pi))->ty;
#line 524 "src/racecheck.tr"
        if ((((unsigned long long)(pt)) != ((unsigned long long)(0LL)))) {
#line 525 "src/racecheck.tr"
            ({ __auto_type _ord_t2987 = ((*pt)->name); List_TrStr_append(rf->ptypes, _ord_t2987); });
#line 526 "src/racecheck.tr"
            if (((*pt)->args->len > 0LL)) {
#line 526 "src/racecheck.tr"
                ({ __auto_type _ord_t2988 = ((*((AstType**)List_ptr_get((*pt)->args, 0LL)))->name); List_TrStr_append(rf->pinner, _ord_t2988); });
            } else {
#line 527 "src/racecheck.tr"
                List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
            }
        } else {
#line 529 "src/racecheck.tr"
            List_TrStr_append(rf->ptypes, _tr_str_lit_len("", 0LL));
#line 530 "src/racecheck.tr"
            List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
        }
#line 531 "src/racecheck.tr"
        pi = (pi + 1LL);
    }
#line 532 "src/racecheck.tr"
    List_ptr_append(self->fns, _tr_obj_retain(rf));
#line 533 "src/racecheck.tr"
    if (rf->is_lib) {
#line 533 "src/racecheck.tr"
        _tr_obj_release(rf, _trdrop_RcFn);
        return;
    }
#line 534 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 535 "src/racecheck.tr"
    long long saved_line = self->cur_line;
#line 536 "src/racecheck.tr"
    long long saved_ld = self->loop_depth;
#line 537 "src/racecheck.tr"
    bool saved_sp = self->seen_spawn;
#line 538 "src/racecheck.tr"
    RcFn* _cltmp_t2989 = _tr_obj_retain(rf);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t2989;
#line 539 "src/racecheck.tr"
    self->cur_line = f->line;
#line 540 "src/racecheck.tr"
    self->loop_depth = 0LL;
#line 541 "src/racecheck.tr"
    self->seen_spawn = false;
#line 542 "src/racecheck.tr"
    RcCk_block(self, f->body);
#line 543 "src/racecheck.tr"
    RcFn* _cltmp_t2990 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t2990;
#line 544 "src/racecheck.tr"
    self->cur_line = saved_line;
#line 545 "src/racecheck.tr"
    self->loop_depth = saved_ld;
#line 546 "src/racecheck.tr"
    self->seen_spawn = saved_sp;
    _tr_obj_release(rf, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
}

__attribute__((hot)) void RcCk_add_global(RcCk* self, TrStr name, bool is_mut, bool is_const, AstType** ty, Expr* v, TrStr df) {
#line 549 "src/racecheck.tr"
    List_TrStr_append(self->gnames, name);
#line 550 "src/racecheck.tr"
    List_TrStr_append(self->gfiles, df);
#line 551 "src/racecheck.tr"
    TrStr tn = _tr_str_lit_len("", 0LL);
#line 552 "src/racecheck.tr"
    if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 552 "src/racecheck.tr"
        TrStr _strtmp_t2991 = _tr_str_retain((*ty)->name);
        _tr_str_release(tn);
        tn = _strtmp_t2991;
    }
#line 553 "src/racecheck.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 553 "src/racecheck.tr"
        TrStr _strtmp_t2992 = _rc_ctor_name(v);
        _tr_str_release(tn);
        tn = _strtmp_t2992;
    }
#line 554 "src/racecheck.tr"
    bool sync = _rc_sync_ty(tn);
#line 555 "src/racecheck.tr"
    if (((!sync) && (!_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))))) {
#line 556 "src/racecheck.tr"
        long long ci0 = RcCk_find_class(self, tn);
#line 557 "src/racecheck.tr"
        if (((ci0 >= 0LL) && List_bool_get(self->class_lib, ci0))) {
#line 557 "src/racecheck.tr"
            sync = true;
        }
    }
#line 558 "src/racecheck.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 559 "src/racecheck.tr"
        __auto_type _t2993 = (*v);
        if (_t2993.tag == Expr_ELitInt) {
            __auto_type _ = _t2993.data.ELitInt.val;
            /* pass */
        } else if (_t2993.tag == Expr_ELitFloat) {
            __auto_type _ = _t2993.data.ELitFloat.val;
            /* pass */
        } else if (_t2993.tag == Expr_ELitStr) {
            /* pass */
        } else if (_t2993.tag == Expr_ELitBool) {
            __auto_type _ = _t2993.data.ELitBool.val;
            /* pass */
        } else if (_t2993.tag == Expr_EList) {
            __auto_type _ = _t2993.data.EList.items;
            /* pass */
        } else if (_t2993.tag == Expr_EDict) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t2993;
            sync = true;
        }
    }
#line 567 "src/racecheck.tr"
    if ((!(is_mut && (!is_const)))) {
#line 569 "src/racecheck.tr"
        bool content_mut = (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || (RcCk_find_class(self, tn) >= 0LL));
#line 570 "src/racecheck.tr"
        if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 571 "src/racecheck.tr"
            __auto_type _t2994 = (*v);
            if (_t2994.tag == Expr_EList) {
                __auto_type _ = _t2994.data.EList.items;
                content_mut = true;
            } else if (_t2994.tag == Expr_EDict) {
                content_mut = true;
            } else if (_t2994.tag == Expr_ESet) {
                __auto_type _ = _t2994.data.ESet.items;
                content_mut = true;
            } else if (1) {
                __auto_type _ = _t2994;
                /* pass */
            }
        }
#line 576 "src/racecheck.tr"
        if ((!content_mut)) {
#line 576 "src/racecheck.tr"
            sync = true;
        }
    }
#line 577 "src/racecheck.tr"
    List_bool_append(self->gsync, sync);
#line 578 "src/racecheck.tr"
    List_TrStr_append(self->gtn, tn);
    _tr_str_release(tn);
}

__attribute__((hot)) long long RcCk_find_fn(RcCk* self, TrStr name) {
#line 581 "src/racecheck.tr"
    long long i = 0LL;
#line 582 "src/racecheck.tr"
    while ((i < self->fns->len)) {
#line 583 "src/racecheck.tr"
        if ((_tr_str_eqv((((RcFn*)List_ptr_get(self->fns, i))->name), (name)) && (!((RcFn*)List_ptr_get(self->fns, i))->is_lib))) {
#line 583 "src/racecheck.tr"
            return i;
        }
#line 584 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 585 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) long long RcCk_find_class(RcCk* self, TrStr name) {
#line 588 "src/racecheck.tr"
    long long i = 0LL;
#line 589 "src/racecheck.tr"
    while ((i < self->classes->len)) {
#line 590 "src/racecheck.tr"
        if (_tr_str_eqv((((ClassDef*)List_ptr_get(self->classes, i))->name), (name))) {
#line 590 "src/racecheck.tr"
            return i;
        }
#line 591 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 592 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool RcCk_field_plain(RcCk* self, TrStr cn, TrStr prop) {
#line 596 "src/racecheck.tr"
    long long ci = RcCk_find_class(self, cn);
#line 597 "src/racecheck.tr"
    if ((ci < 0LL)) {
#line 597 "src/racecheck.tr"
        return false;
    }
#line 598 "src/racecheck.tr"
    ClassDef* cls = ((ClassDef*)List_ptr_get(self->classes, ci));
#line 599 "src/racecheck.tr"
    long long i = 0LL;
#line 600 "src/racecheck.tr"
    while ((i < cls->fields->len)) {
#line 601 "src/racecheck.tr"
        if (_tr_str_eqv((((FieldDef*)List_ptr_get(cls->fields, i))->name), (prop))) {
#line 602 "src/racecheck.tr"
            AstType** ft = ((FieldDef*)List_ptr_get(cls->fields, i))->ty;
#line 603 "src/racecheck.tr"
            if ((((unsigned long long)(ft)) == ((unsigned long long)(0LL)))) {
#line 603 "src/racecheck.tr"
                return false;
            }
#line 604 "src/racecheck.tr"
            TrStr tn = _tr_str_retain((*ft)->name);
#line 605 "src/racecheck.tr"
            if (_rc_sync_ty(tn)) {
#line 605 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 606 "src/racecheck.tr"
            long long ni = RcCk_find_class(self, tn);
#line 607 "src/racecheck.tr"
            if (((ni >= 0LL) && List_bool_get(self->class_lib, ni))) {
#line 607 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 608 "src/racecheck.tr"
            _tr_str_release(tn);
            return true;
        }
#line 609 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 610 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_i64* RcCk_reach(RcCk* self, long long ti) {
#line 614 "src/racecheck.tr"
    List_i64* seen = (void*)List_i64_new();
#line 615 "src/racecheck.tr"
    List_i64_append(seen, ti);
#line 616 "src/racecheck.tr"
    long long k = 0LL;
#line 617 "src/racecheck.tr"
    while ((k < seen->len)) {
#line 618 "src/racecheck.tr"
        RcFn* f = ({ __auto_type _ord_t2995 = (List_i64_get(seen, k)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t2995))); _wr; });
#line 619 "src/racecheck.tr"
        long long ci = 0LL;
#line 620 "src/racecheck.tr"
        while ((ci < f->calls->len)) {
#line 621 "src/racecheck.tr"
            long long ni = ({ TrStr _at_t2996 = (List_TrStr_get(f->calls, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t2996)); _tr_str_release(_at_t2996); _wr; });
#line 622 "src/racecheck.tr"
            if ((ni >= 0LL)) {
#line 623 "src/racecheck.tr"
                bool dup = false;
#line 624 "src/racecheck.tr"
                long long q = 0LL;
#line 625 "src/racecheck.tr"
                while ((q < seen->len)) {
#line 626 "src/racecheck.tr"
                    if ((List_i64_get(seen, q) == ni)) {
#line 626 "src/racecheck.tr"
                        dup = true;
                    }
#line 627 "src/racecheck.tr"
                    q = (q + 1LL);
                }
#line 628 "src/racecheck.tr"
                if ((!dup)) {
#line 628 "src/racecheck.tr"
                    List_i64_append(seen, ni);
                }
            }
#line 629 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 630 "src/racecheck.tr"
        k = (k + 1LL);
    }
#line 631 "src/racecheck.tr"
    return seen;
}

__attribute__((hot)) void RcCk_analyze_site(RcCk* self, RcFn* f, RcSite* s) {
#line 635 "src/racecheck.tr"
    if (({ __auto_type _ord_t2998 = (f->closures); TrStr _apt_t2997 = _tr_str_retain(s->target); __auto_type _wr = (_rc_has(_ord_t2998, _apt_t2997)); _tr_str_release(_apt_t2997); _wr; })) {
#line 636 "src/racecheck.tr"
        ({ __auto_type _ord_t2999 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3000 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2999); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a closure value: it captures its surrounding locals by reference into this stack frame, so a spawned thread would race on them and outlive them.\n      FIX: spawn a named `def` and pass the data it needs as arguments (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 289LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3000); _tr_str_release(_ord_t3000); });
#line 637 "src/racecheck.tr"
        return;
    }
#line 638 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3001 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3001)); _tr_str_release(_apt_t3001); _wr; });
#line 639 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 639 "src/racecheck.tr"
        return;
    }
#line 640 "src/racecheck.tr"
    List_i64* reach = RcCk_reach(self, ti);
#line 642 "src/racecheck.tr"
    long long gi = 0LL;
#line 643 "src/racecheck.tr"
    while ((gi < self->gnames->len)) {
#line 644 "src/racecheck.tr"
        if ((!List_bool_get(self->gsync, gi))) {
#line 645 "src/racecheck.tr"
            TrStr g = List_TrStr_get(self->gnames, gi);
#line 646 "src/racecheck.tr"
            bool wr = false;
#line 647 "src/racecheck.tr"
            bool rd = false;
#line 648 "src/racecheck.tr"
            long long ri = 0LL;
#line 649 "src/racecheck.tr"
            while ((ri < reach->len)) {
#line 650 "src/racecheck.tr"
                RcFn* rf = ({ __auto_type _ord_t3002 = (List_i64_get(reach, ri)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3002))); _wr; });
#line 651 "src/racecheck.tr"
                long long ti2 = 0LL;
#line 652 "src/racecheck.tr"
                while ((ti2 < rf->touched->len)) {
#line 653 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3003 = (List_TrStr_get(rf->touched, ti2)); TrStr _cmpb_t3004 = (g); _Bool _cmpr_t3005 = _tr_str_eqv(_cmpa_t3003, _cmpb_t3004); _tr_str_release(_cmpa_t3003); _cmpr_t3005; })) {
#line 654 "src/racecheck.tr"
                        if (List_bool_get(rf->touched_w, ti2)) {
#line 654 "src/racecheck.tr"
                            wr = true;
                        } else {
#line 655 "src/racecheck.tr"
                            rd = true;
                        }
                    }
#line 656 "src/racecheck.tr"
                    ti2 = (ti2 + 1LL);
                }
#line 657 "src/racecheck.tr"
                if (({ TrStr _at_t3006 = (List_TrStr_get(self->gtn, gi)); __auto_type _wr = ((RcCk_find_class(self, _at_t3006) >= 0LL)); _tr_str_release(_at_t3006); _wr; })) {
#line 659 "src/racecheck.tr"
                    long long mq = 0LL;
#line 660 "src/racecheck.tr"
                    while ((mq < rf->mcrecv->len)) {
#line 661 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3007 = (List_TrStr_get(rf->mcrecv, mq)); TrStr _cmpb_t3008 = (g); _Bool _cmpr_t3009 = _tr_str_eqv(_cmpa_t3007, _cmpb_t3008); _tr_str_release(_cmpa_t3007); _cmpr_t3009; }) && (!_rc_has(rf->locals, g))) && (!_rc_has(rf->params, g)))) {
#line 662 "src/racecheck.tr"
                            long long mfi = ({ __auto_type _ord_t3010 = (({ TrStr _tr_cl = (List_TrStr_get(self->gtn, gi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t3011 = (({ TrStr _tr_cl = (_ord_t3010); TrStr _tr_cr = (List_TrStr_get(rf->mcname, mq)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3011)); _tr_str_release(_at_t3011); _wr; });
#line 663 "src/racecheck.tr"
                            if ((mfi >= 0LL)) {
#line 664 "src/racecheck.tr"
                                RcFn* mff = ((RcFn*)List_ptr_get(self->fns, mfi));
#line 665 "src/racecheck.tr"
                                long long mk = 0LL;
#line 666 "src/racecheck.tr"
                                while ((mk < mff->wroot->len)) {
#line 667 "src/racecheck.tr"
                                    if (((({ TrStr _cmpa_t3012 = (List_TrStr_get(mff->wroot, mk)); TrStr _cmpb_t3013 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3014 = _tr_str_eqv(_cmpa_t3012, _cmpb_t3013); _tr_str_release(_cmpa_t3012); _cmpr_t3014; }) && ({ TrStr _cmpa_t3015 = (List_TrStr_get(mff->wprop, mk)); TrStr _cmpb_t3016 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3017 = !_tr_str_eqv(_cmpa_t3015, _cmpb_t3016); _tr_str_release(_cmpa_t3015); _cmpr_t3017; })) && ({ TrStr _ord_t3018 = (List_TrStr_get(self->gtn, gi)); TrStr _ord_t3019 = (List_TrStr_get(mff->wprop, mk)); __auto_type _wr = (RcCk_field_plain(self, _ord_t3018, _ord_t3019)); _tr_str_release(_ord_t3018); _tr_str_release(_ord_t3019); _wr; }))) {
#line 667 "src/racecheck.tr"
                                        wr = true;
                                    }
#line 668 "src/racecheck.tr"
                                    mk = (mk + 1LL);
                                }
                            }
                        }
#line 669 "src/racecheck.tr"
                        mq = (mq + 1LL);
                    }
                }
#line 670 "src/racecheck.tr"
                ri = (ri + 1LL);
            }
#line 671 "src/racecheck.tr"
            if ((rd && (!wr))) {
#line 672 "src/racecheck.tr"
                long long fi = 0LL;
#line 673 "src/racecheck.tr"
                while ((fi < self->fns->len)) {
#line 674 "src/racecheck.tr"
                    RcFn* of = ((RcFn*)List_ptr_get(self->fns, fi));
#line 675 "src/racecheck.tr"
                    if ((!of->is_lib)) {
#line 676 "src/racecheck.tr"
                        long long oi = 0LL;
#line 677 "src/racecheck.tr"
                        while ((oi < of->touched->len)) {
#line 678 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3020 = (List_TrStr_get(of->touched, oi)); TrStr _cmpb_t3021 = (g); _Bool _cmpr_t3022 = _tr_str_eqv(_cmpa_t3020, _cmpb_t3021); _tr_str_release(_cmpa_t3020); _cmpr_t3022; }) && List_bool_get(of->touched_w, oi)) && List_bool_get(of->touched_after, oi))) {
#line 678 "src/racecheck.tr"
                                wr = true;
                            }
#line 679 "src/racecheck.tr"
                            oi = (oi + 1LL);
                        }
                    }
#line 680 "src/racecheck.tr"
                    fi = (fi + 1LL);
                }
            }
#line 681 "src/racecheck.tr"
            if (wr) {
#line 682 "src/racecheck.tr"
                ({ __auto_type _ord_t3023 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] the spawned function '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3024 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3023); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses the module-level mutable '", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is plain data shared by every thread: concurrent access is a data race.\n      FIX: make '", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (counters/flags), a `Mutex[T]`/`RwLock[T]` (compound state), or pass the state to the thread explicitly.", 121LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3024); _tr_str_release(_ord_t3024); });
            } else if (rd) {
#line 684 "src/racecheck.tr"
                /* pass */
            }
        }
#line 685 "src/racecheck.tr"
        gi = (gi + 1LL);
    }
#line 687 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, ti));
#line 688 "src/racecheck.tr"
    long long ai = 0LL;
#line 689 "src/racecheck.tr"
    while (((ai < s->args->len) && (ai < tf->params->len))) {
#line 690 "src/racecheck.tr"
        TrStr an = _tr_str_lit_len("", 0LL);
#line 691 "src/racecheck.tr"
        __auto_type _t3025 = (*((Expr*)List_ptr_get(s->args, ai)));
        if (_t3025.tag == Expr_EIdent) {
            __auto_type nn = _t3025.data.EIdent.name;
            TrStr _strtmp_t3026 = _tr_str_retain(nn);
            _tr_str_release(an);
            an = _strtmp_t3026;
        } else if (1) {
            __auto_type _ = _t3025;
            /* pass */
        }
#line 694 "src/racecheck.tr"
        if ((!_tr_str_eqv((an), (_tr_str_lit_len("", 0LL))))) {
#line 695 "src/racecheck.tr"
            long long cnt = 0LL;
#line 696 "src/racecheck.tr"
            long long si = 0LL;
#line 697 "src/racecheck.tr"
            while ((si < f->sites->len)) {
#line 698 "src/racecheck.tr"
                long long oa = 0LL;
#line 699 "src/racecheck.tr"
                while ((oa < ((RcSite*)List_ptr_get(f->sites, si))->args->len)) {
#line 700 "src/racecheck.tr"
                    __auto_type _t3027 = (*((Expr*)List_ptr_get(((RcSite*)List_ptr_get(f->sites, si))->args, oa)));
                    if (_t3027.tag == Expr_EIdent) {
                        __auto_type on = _t3027.data.EIdent.name;
#line 702 "src/racecheck.tr"
                        if (_tr_str_eqv((on), (an))) {
#line 702 "src/racecheck.tr"
                            cnt = (cnt + 1LL);
                        }
                    } else if (1) {
                        __auto_type _ = _t3027;
                        /* pass */
                    }
#line 704 "src/racecheck.tr"
                    oa = (oa + 1LL);
                }
#line 705 "src/racecheck.tr"
                si = (si + 1LL);
            }
#line 706 "src/racecheck.tr"
            bool is_sh = (_rc_has(f->shvars, an) || ({ TrStr _cmpa_t3028 = (RcCk_ltype(self, f, an)); TrStr _cmpb_t3029 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3030 = _tr_str_eqv(_cmpa_t3028, _cmpb_t3029); _tr_str_release(_cmpa_t3028); _cmpr_t3030; }));
#line 707 "src/racecheck.tr"
            if (((is_sh && ({ TrStr _cmpa_t3031 = (List_TrStr_get(tf->ptypes, ai)); TrStr _cmpb_t3032 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3033 = !_tr_str_eqv(_cmpa_t3031, _cmpb_t3032); _tr_str_release(_cmpa_t3031); _cmpr_t3033; })) && ({ TrStr _cmpa_t3034 = (List_TrStr_get(tf->ptypes, ai)); TrStr _cmpb_t3035 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3036 = !_tr_str_eqv(_cmpa_t3034, _cmpb_t3035); _tr_str_release(_cmpa_t3034); _cmpr_t3036; }))) {
#line 708 "src/racecheck.tr"
                ({ __auto_type _ord_t3037 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-10] '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a `Shared` handle but '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3038 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3037); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares parameter '", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3039 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3038); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3040 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3039); TrStr _tr_cr = (List_TrStr_get(tf->ptypes, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': the thread would be handed the shared control block where it expects the object itself (a type confusion).\n      FIX: declare the worker's parameter as `", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3041 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3040); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Shared[", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3042 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3041); TrStr _tr_cr = (List_TrStr_get(tf->ptypes, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3042); _tr_str_release(_ord_t3042); });
#line 709 "src/racecheck.tr"
                ai = (ai + 1LL);
#line 710 "src/racecheck.tr"
                _tr_str_release(an);
                continue;
            }
#line 711 "src/racecheck.tr"
            bool multi = (((cnt >= 2LL) || _rc_has(f->shvars, an)) || ({ TrStr _cmpa_t3043 = (RcCk_ltype(self, f, an)); TrStr _cmpb_t3044 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3045 = _tr_str_eqv(_cmpa_t3043, _cmpb_t3044); _tr_str_release(_cmpa_t3043); _cmpr_t3045; }));
#line 712 "src/racecheck.tr"
            if (multi) {
#line 713 "src/racecheck.tr"
                RcCk_check_shared_writes(self, f, s, tf, ai, an);
            }
        }
#line 714 "src/racecheck.tr"
        ai = (ai + 1LL);
        _tr_str_release(an);
    }
}

__attribute__((hot)) void RcCk_check_shared_writes(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, long long ai, TrStr an) {
#line 717 "src/racecheck.tr"
    TrStr pn = List_TrStr_get(tf->params, ai);
#line 718 "src/racecheck.tr"
    TrStr cn = List_TrStr_get(tf->ptypes, ai);
#line 719 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("Shared", 6LL)))) {
#line 719 "src/racecheck.tr"
        TrStr _strtmp_t3046 = List_TrStr_get(tf->pinner, ai);
        _tr_str_release(cn);
        cn = _strtmp_t3046;
    }
#line 720 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 721 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 722 "src/racecheck.tr"
    long long wi = 0LL;
#line 723 "src/racecheck.tr"
    while ((wi < tf->wroot->len)) {
#line 724 "src/racecheck.tr"
        if ((({ TrStr _cmpa_t3047 = (List_TrStr_get(tf->wroot, wi)); TrStr _cmpb_t3048 = (pn); _Bool _cmpr_t3049 = _tr_str_eqv(_cmpa_t3047, _cmpb_t3048); _tr_str_release(_cmpa_t3047); _cmpr_t3049; }) && ({ TrStr _cmpa_t3050 = (List_TrStr_get(tf->wprop, wi)); TrStr _cmpb_t3051 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3052 = !_tr_str_eqv(_cmpa_t3050, _cmpb_t3051); _tr_str_release(_cmpa_t3050); _cmpr_t3052; }))) {
#line 725 "src/racecheck.tr"
            if (({ TrStr _at_t3053 = (List_TrStr_get(tf->wprop, wi)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3053)); _tr_str_release(_at_t3053); _wr; })) {
#line 726 "src/racecheck.tr"
                ({ TrStr _at_t3054 = (List_TrStr_get(tf->wprop, wi)); RcCk_shared_msg(self, f, s, tf, cn, _at_t3054, an); _tr_str_release(_at_t3054); });
#line 727 "src/racecheck.tr"
                _tr_str_release(pn);
                _tr_str_release(cn);
                return;
            }
        }
#line 728 "src/racecheck.tr"
        wi = (wi + 1LL);
    }
#line 729 "src/racecheck.tr"
    long long mi = 0LL;
#line 730 "src/racecheck.tr"
    while ((mi < tf->mcrecv->len)) {
#line 731 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3055 = (List_TrStr_get(tf->mcrecv, mi)); TrStr _cmpb_t3056 = (pn); _Bool _cmpr_t3057 = _tr_str_eqv(_cmpa_t3055, _cmpb_t3056); _tr_str_release(_cmpa_t3055); _cmpr_t3057; })) {
#line 732 "src/racecheck.tr"
            long long mf = ({ TrStr _at_t3058 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(tf->mcname, mi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3058)); _tr_str_release(_at_t3058); _wr; });
#line 733 "src/racecheck.tr"
            if ((mf >= 0LL)) {
#line 734 "src/racecheck.tr"
                RcFn* mfn = ((RcFn*)List_ptr_get(self->fns, mf));
#line 735 "src/racecheck.tr"
                long long k = 0LL;
#line 736 "src/racecheck.tr"
                while ((k < mfn->wroot->len)) {
#line 737 "src/racecheck.tr"
                    if ((({ TrStr _cmpa_t3059 = (List_TrStr_get(mfn->wroot, k)); TrStr _cmpb_t3060 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3061 = _tr_str_eqv(_cmpa_t3059, _cmpb_t3060); _tr_str_release(_cmpa_t3059); _cmpr_t3061; }) && ({ TrStr _cmpa_t3062 = (List_TrStr_get(mfn->wprop, k)); TrStr _cmpb_t3063 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3064 = !_tr_str_eqv(_cmpa_t3062, _cmpb_t3063); _tr_str_release(_cmpa_t3062); _cmpr_t3064; }))) {
#line 738 "src/racecheck.tr"
                        if (({ TrStr _at_t3065 = (List_TrStr_get(mfn->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3065)); _tr_str_release(_at_t3065); _wr; })) {
#line 739 "src/racecheck.tr"
                            ({ TrStr _at_t3066 = (List_TrStr_get(mfn->wprop, k)); RcCk_shared_msg(self, f, s, tf, cn, _at_t3066, an); _tr_str_release(_at_t3066); });
#line 740 "src/racecheck.tr"
                            _tr_str_release(pn);
                            _tr_str_release(cn);
                            return;
                        }
                    }
#line 741 "src/racecheck.tr"
                    k = (k + 1LL);
                }
            }
        }
#line 742 "src/racecheck.tr"
        mi = (mi + 1LL);
    }
    _tr_str_release(pn);
    _tr_str_release(cn);
}

__attribute__((hot)) void RcCk_shared_msg(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, TrStr cn, TrStr prop, TrStr an) {
#line 745 "src/racecheck.tr"
    ({ __auto_type _ord_t3067 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3068 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3067); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' mutates the plain field '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is shared by several threads: `Shared[T]` only makes the reference COUNT atomic, not the fields, so the concurrent writes race.\n      FIX: make '", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (or guard the whole object with `Mutex[", 56LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`), or give each thread its own value.", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3068); _tr_str_release(_ord_t3068); });
}

__attribute__((hot)) TrStr RcCk_where2(RcCk* self, RcFn* f, long long line) {
#line 748 "src/racecheck.tr"
    long long l = line;
#line 749 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 749 "src/racecheck.tr"
        l = f->line;
    }
#line 750 "src/racecheck.tr"
    return ({ __auto_type _ord_t3069 = (_tr_strx_concatv((f->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3069); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) RcFn* _rc_new_fn(TrStr name, TrStr file, long long line, bool is_lib) {
#line 52 "src/racecheck.tr"
    RcFn* f = ((RcFn*)_tr_obj_alloc(sizeof(RcFn)));
#line 53 "src/racecheck.tr"
    f->name = _tr_str_retain(name);
#line 54 "src/racecheck.tr"
    f->file = _tr_str_retain(file);
#line 55 "src/racecheck.tr"
    f->line = line;
#line 56 "src/racecheck.tr"
    f->is_lib = is_lib;
#line 57 "src/racecheck.tr"
    f->params = (void*)List_TrStr_new();
#line 58 "src/racecheck.tr"
    f->ptypes = (void*)List_TrStr_new();
#line 59 "src/racecheck.tr"
    f->pinner = (void*)List_TrStr_new();
#line 60 "src/racecheck.tr"
    f->locals = (void*)List_TrStr_new();
#line 61 "src/racecheck.tr"
    f->ltn = (void*)List_TrStr_new();
#line 62 "src/racecheck.tr"
    f->ltt = (void*)List_TrStr_new();
#line 63 "src/racecheck.tr"
    f->touched = (void*)List_TrStr_new();
#line 64 "src/racecheck.tr"
    f->touched_w = (void*)List_bool_new();
#line 65 "src/racecheck.tr"
    f->touched_after = (void*)List_bool_new();
#line 66 "src/racecheck.tr"
    f->touched_line = (void*)List_i64_new();
#line 67 "src/racecheck.tr"
    f->calls = (void*)List_TrStr_new();
#line 68 "src/racecheck.tr"
    f->sites = (void*)List_ptr_new();
#line 69 "src/racecheck.tr"
    f->closures = (void*)List_TrStr_new();
#line 70 "src/racecheck.tr"
    f->shvars = (void*)List_TrStr_new();
#line 71 "src/racecheck.tr"
    f->wroot = (void*)List_TrStr_new();
#line 72 "src/racecheck.tr"
    f->wprop = (void*)List_TrStr_new();
#line 73 "src/racecheck.tr"
    f->mcrecv = (void*)List_TrStr_new();
#line 74 "src/racecheck.tr"
    f->mcname = (void*)List_TrStr_new();
#line 75 "src/racecheck.tr"
    f->msgs = (void*)List_TrStr_new();
#line 76 "src/racecheck.tr"
    return f;
}

__attribute__((hot)) bool _rc_sync_ty(TrStr n) {
#line 93 "src/racecheck.tr"
    return (((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("WaitGroup", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Semaphore", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Once", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Barrier", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Condvar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL))));
}

__attribute__((hot)) bool _rc_mut_method(TrStr m) {
#line 96 "src/racecheck.tr"
    return (((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("update", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("delete", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_back", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop_back", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("truncate", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL))));
}

__attribute__((hot)) bool _rc_has(List_TrStr* v, TrStr s) {
#line 99 "src/racecheck.tr"
    long long i = 0LL;
#line 100 "src/racecheck.tr"
    while ((i < v->len)) {
#line 101 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3070 = (List_TrStr_get(v, i)); TrStr _cmpb_t3071 = (s); _Bool _cmpr_t3072 = _tr_str_eqv(_cmpa_t3070, _cmpb_t3071); _tr_str_release(_cmpa_t3070); _cmpr_t3072; })) {
#line 101 "src/racecheck.tr"
            return true;
        }
#line 102 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 103 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_nul(Expr* p) {
#line 106 "src/racecheck.tr"
    return (((unsigned long long)(p)) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr _rc_root(Expr* e) {
#line 110 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 110 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 111 "src/racecheck.tr"
    __auto_type _t3073 = (*e);
    if (_t3073.tag == Expr_EIdent) {
        __auto_type n = _t3073.data.EIdent.name;
        TrStr _trret556 = _tr_str_retain(n);
        return _trret556;
    } else if (_t3073.tag == Expr_EPropAccess) {
        __auto_type o = _t3073.data.EPropAccess.obj;
        return _rc_root(o);
    } else if (_t3073.tag == Expr_EIndex) {
        __auto_type o = _t3073.data.EIndex.obj;
        return _rc_root(o);
    } else if (1) {
        __auto_type _ = _t3073;
        return _tr_str_lit_len("", 0LL);
    }
#line 116 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_first_prop(Expr* e) {
#line 120 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 120 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 121 "src/racecheck.tr"
    __auto_type _t3074 = (*e);
    if (_t3074.tag == Expr_EPropAccess) {
        __auto_type o = _t3074.data.EPropAccess.obj;
__auto_type p = _t3074.data.EPropAccess.prop;
#line 123 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 124 "src/racecheck.tr"
            __auto_type _t3075 = (*o);
            if (_t3075.tag == Expr_EIdent) {
                __auto_type _ = _t3075.data.EIdent.name;
                TrStr _trret557 = _tr_str_retain(p);
                return _trret557;
            } else if (1) {
                __auto_type _ = _t3075;
                /* pass */
            }
        }
#line 127 "src/racecheck.tr"
        TrStr _trret558 = _rc_first_prop(o);
        return _trret558;
    } else if (_t3074.tag == Expr_EIndex) {
        __auto_type o = _t3074.data.EIndex.obj;
        return _rc_first_prop(o);
    } else if (1) {
        __auto_type _ = _t3074;
        return _tr_str_lit_len("", 0LL);
    }
#line 130 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_ctor_name(Expr* v) {
#line 134 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 134 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 135 "src/racecheck.tr"
    __auto_type _t3076 = (*v);
    if (_t3076.tag == Expr_EMethodCall) {
        __auto_type o = _t3076.data.EMethodCall.obj;
        return _rc_root(o);
    } else if (_t3076.tag == Expr_ECall) {
        __auto_type c = _t3076.data.ECall.callee;
        return _rc_root(c);
    } else if (1) {
        __auto_type _ = _t3076;
        return _tr_str_lit_len("", 0LL);
    }
#line 139 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _rc_is_lambda(Expr* v) {
#line 142 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 142 "src/racecheck.tr"
        return false;
    }
#line 143 "src/racecheck.tr"
    __auto_type _t3077 = (*v);
    if (_t3077.tag == Expr_EClosure) {
        return true;
    } else if (1) {
        __auto_type _ = _t3077;
        return false;
    }
#line 146 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_path_lib(TrStr p) {
#line 754 "src/racecheck.tr"
    return (((_tr_str_containsv((p), (_tr_str_lit_len("std/", 4LL))) || _tr_str_containsv((p), (_tr_str_lit_len("core/", 5LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("std\\", 4LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("core\\", 5LL))));
}

__attribute__((hot)) bool _rc_ifaces_sendable(ClassDef* c) {
#line 757 "src/racecheck.tr"
    long long i = 0LL;
#line 758 "src/racecheck.tr"
    while ((i < c->iface_names->len)) {
#line 759 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3078 = (List_TrStr_get(c->iface_names, i)); TrStr _cmpb_t3079 = (_tr_str_lit_len("Sendable", 8LL)); _Bool _cmpr_t3080 = _tr_str_eqv(_cmpa_t3078, _cmpb_t3079); _tr_str_release(_cmpa_t3078); _cmpr_t3080; })) {
#line 759 "src/racecheck.tr"
            return true;
        }
#line 760 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 761 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_TrStr* check_data_races(Program* prog, List_TrStr* decl_files, TrStr input_path) {
#line 764 "src/racecheck.tr"
    RcCk* ck = ((RcCk*)_tr_obj_alloc(sizeof(RcCk)));
#line 765 "src/racecheck.tr"
    ck->fns = (void*)List_ptr_new();
#line 766 "src/racecheck.tr"
    ck->classes = (void*)List_ptr_new();
#line 767 "src/racecheck.tr"
    ck->class_lib = (void*)List_bool_new();
#line 768 "src/racecheck.tr"
    ck->gnames = (void*)List_TrStr_new();
#line 769 "src/racecheck.tr"
    ck->gfiles = (void*)List_TrStr_new();
#line 770 "src/racecheck.tr"
    ck->gsync = (void*)List_bool_new();
#line 771 "src/racecheck.tr"
    ck->gtn = (void*)List_TrStr_new();
#line 772 "src/racecheck.tr"
    ck->msgs = (void*)List_TrStr_new();
#line 773 "src/racecheck.tr"
    RcFn* _cltmp_t3081 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3081;
#line 774 "src/racecheck.tr"
    long long i = 0LL;
#line 775 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 776 "src/racecheck.tr"
        TrStr dfc = _tr_str_retain(input_path);
#line 777 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 777 "src/racecheck.tr"
            TrStr _strtmp_t3082 = List_TrStr_get(decl_files, i);
            _tr_str_release(dfc);
            dfc = _strtmp_t3082;
        }
#line 778 "src/racecheck.tr"
        __auto_type _t3083 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3083.tag == Decl_DClass) {
            __auto_type c = _t3083.data.DClass.cls;
#line 780 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c));
#line 781 "src/racecheck.tr"
            ({ __auto_type _ord_t3084 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3084); });
        } else if (_t3083.tag == Decl_DActor) {
            __auto_type c2 = _t3083.data.DActor.cls;
#line 783 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c2));
#line 784 "src/racecheck.tr"
            ({ __auto_type _ord_t3085 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3085); });
        } else if (1) {
            __auto_type _ = _t3083;
            /* pass */
        }
#line 786 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(dfc);
    }
#line 787 "src/racecheck.tr"
    i = 0LL;
#line 788 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 789 "src/racecheck.tr"
        TrStr df = _tr_str_retain(input_path);
#line 790 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 790 "src/racecheck.tr"
            TrStr _strtmp_t3086 = List_TrStr_get(decl_files, i);
            _tr_str_release(df);
            df = _strtmp_t3086;
        }
#line 791 "src/racecheck.tr"
        bool lib = _rc_path_lib(df);
#line 792 "src/racecheck.tr"
        __auto_type _t3087 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3087.tag == Decl_DTopLevelStmt) {
            __auto_type sp = _t3087.data.DTopLevelStmt.stmt;
#line 794 "src/racecheck.tr"
            __auto_type _t3088 = (*sp);
            if (_t3088.tag == Stmt_SLet) {
                __auto_type name = _t3088.data.SLet.name;
__auto_type is_mut = _t3088.data.SLet.is_mut;
__auto_type is_const = _t3088.data.SLet.is_const;
__auto_type ty = _t3088.data.SLet.ty;
__auto_type v = _t3088.data.SLet.val;
#line 796 "src/racecheck.tr"
                if ((!lib)) {
#line 796 "src/racecheck.tr"
                    RcCk_add_global(ck, name, is_mut, is_const, ty, v, df);
                }
            } else if (_t3088.tag == Stmt_SAssign) {
                __auto_type tg = _t3088.data.SAssign.target;
__auto_type v3 = _t3088.data.SAssign.val;
#line 799 "src/racecheck.tr"
                if (((!lib) && (!_rc_nul(tg)))) {
#line 800 "src/racecheck.tr"
                    __auto_type _t3089 = (*tg);
                    if (_t3089.tag == Expr_EIdent) {
                        __auto_type gn = _t3089.data.EIdent.name;
                        RcCk_add_global(ck, gn, false, false, (AstType**)(0LL), v3, df);
                    } else if (1) {
                        __auto_type _ = _t3089;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t3088;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t3087;
            /* pass */
        }
#line 805 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df);
    }
#line 806 "src/racecheck.tr"
    i = 0LL;
#line 807 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 808 "src/racecheck.tr"
        TrStr df2 = _tr_str_retain(input_path);
#line 809 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 809 "src/racecheck.tr"
            TrStr _strtmp_t3090 = List_TrStr_get(decl_files, i);
            _tr_str_release(df2);
            df2 = _strtmp_t3090;
        }
#line 810 "src/racecheck.tr"
        bool lib2 = _rc_path_lib(df2);
#line 811 "src/racecheck.tr"
        __auto_type _t3091 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3091.tag == Decl_DFunction) {
            __auto_type f = _t3091.data.DFunction.func;
            RcCk_add_fn(ck, f, df2, lib2);
        } else if (_t3091.tag == Decl_DClass) {
            __auto_type c3 = _t3091.data.DClass.cls;
#line 814 "src/racecheck.tr"
            long long mi = 0LL;
#line 815 "src/racecheck.tr"
            while ((mi < c3->methods->len)) {
#line 816 "src/racecheck.tr"
                ({ __auto_type _ord_t3092 = (((FunctionDef*)List_ptr_get(c3->methods, mi))); __auto_type _ord_t3093 = (c3->name); RcCk_add_method(ck, _ord_t3092, _ord_t3093, df2, lib2); });
#line 817 "src/racecheck.tr"
                mi = (mi + 1LL);
            }
        } else if (_t3091.tag == Decl_DActor) {
            __auto_type c4 = _t3091.data.DActor.cls;
#line 819 "src/racecheck.tr"
            long long mi2 = 0LL;
#line 820 "src/racecheck.tr"
            while ((mi2 < c4->methods->len)) {
#line 821 "src/racecheck.tr"
                ({ __auto_type _ord_t3094 = (((FunctionDef*)List_ptr_get(c4->methods, mi2))); __auto_type _ord_t3095 = (c4->name); RcCk_add_method(ck, _ord_t3094, _ord_t3095, df2, lib2); });
#line 822 "src/racecheck.tr"
                mi2 = (mi2 + 1LL);
            }
        } else if (_t3091.tag == Decl_DExtend) {
            __auto_type tgt = _t3091.data.DExtend.target;
__auto_type methods = _t3091.data.DExtend.methods;
#line 824 "src/racecheck.tr"
            long long mi3 = 0LL;
#line 825 "src/racecheck.tr"
            while ((mi3 < methods->len)) {
#line 826 "src/racecheck.tr"
                RcCk_add_method(ck, ((FunctionDef*)List_ptr_get(methods, mi3)), tgt, df2, lib2);
#line 827 "src/racecheck.tr"
                mi3 = (mi3 + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t3091;
            /* pass */
        }
#line 829 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df2);
    }
#line 830 "src/racecheck.tr"
    i = 0LL;
#line 831 "src/racecheck.tr"
    while ((i < ck->fns->len)) {
#line 832 "src/racecheck.tr"
        RcFn* f = ((RcFn*)List_ptr_get(ck->fns, i));
#line 833 "src/racecheck.tr"
        if ((!f->is_lib)) {
#line 834 "src/racecheck.tr"
            long long mj = 0LL;
#line 835 "src/racecheck.tr"
            while ((mj < f->msgs->len)) {
#line 836 "src/racecheck.tr"
                ({ TrStr _ord_t3096 = (List_TrStr_get(f->msgs, mj)); List_TrStr_append(ck->msgs, _ord_t3096); _tr_str_release(_ord_t3096); });
#line 837 "src/racecheck.tr"
                mj = (mj + 1LL);
            }
#line 838 "src/racecheck.tr"
            long long sj = 0LL;
#line 839 "src/racecheck.tr"
            while ((sj < f->sites->len)) {
#line 840 "src/racecheck.tr"
                RcCk_analyze_site(ck, f, ((RcSite*)List_ptr_get(f->sites, sj)));
#line 841 "src/racecheck.tr"
                sj = (sj + 1LL);
            }
        }
#line 842 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 843 "src/racecheck.tr"
    return ck->msgs;
}

