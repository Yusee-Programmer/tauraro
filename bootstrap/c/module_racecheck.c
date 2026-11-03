#include "tauraro_types.h"

RcFn* _rc_new_fn(TrStr name, TrStr file, long long line, bool is_lib);
bool _rc_sync_ty(TrStr n);
bool _rc_mut_method(TrStr m);
bool _rc_has(List_TrStr* v, TrStr s);
bool _rc_nul(Expr* p);
TrStr _rc_root(Expr* e);
TrStr _rc_first_prop(Expr* e);
TrStr _rc_ctor_name(Expr* v);
TrStr _rc_ctor_arg(Expr* v);
long long _rc_dot(TrStr n);
bool _rc_is_thread_spawn(Expr* v);
bool _rc_is_lambda(Expr* v);
bool _rc_path_lib(TrStr p);
bool _rc_ifaces_sendable(ClassDef* c);

__attribute__((hot)) void RcCk_ty_check(RcCk* self, AstType** t, TrStr file, long long line) {
#line 222 "src/racecheck.tr"
    if ((((unsigned long long)(t)) == ((unsigned long long)(0LL)))) {
#line 222 "src/racecheck.tr"
        return;
    }
#line 223 "src/racecheck.tr"
    AstType* ty = (*t);
#line 224 "src/racecheck.tr"
    bool coll = ((((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL))));
#line 225 "src/racecheck.tr"
    long long i = 0LL;
#line 226 "src/racecheck.tr"
    while ((i < ty->args->len)) {
#line 227 "src/racecheck.tr"
        AstType** a = ((AstType**)List_ptr_get(ty->args, i));
#line 228 "src/racecheck.tr"
        if (((((coll && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL))))) && (((unsigned long long)(a)) != ((unsigned long long)(0LL)))) && (_tr_str_eqv(((*a)->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv(((*a)->name), (_tr_str_lit_len("Weak", 4LL)))))) {
#line 229 "src/racecheck.tr"
            ({ __auto_type _ord_t3352 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((file), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-11] '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3353 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3352); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot hold '", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3354 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3353); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*a)->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[..]' elements: reference-counted shared handles are supported as `List`/`Vec` elements only.\n      FIX: use a `List[", 117LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3355 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3354); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*a)->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[..]]`, or wrap the handle in a small class (`class Handle: pub inner: Shared[T]`) and store that.", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3355); _tr_str_release(_ord_t3355); });
#line 230 "src/racecheck.tr"
            return;
        }
#line 231 "src/racecheck.tr"
        RcCk_ty_check(self, a, file, line);
#line 232 "src/racecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) TrStr RcCk_where(RcCk* self, long long line) {
#line 235 "src/racecheck.tr"
    long long l = line;
#line 236 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 236 "src/racecheck.tr"
        l = self->cur->line;
    }
#line 237 "src/racecheck.tr"
    return ({ __auto_type _ord_t3356 = (_tr_strx_concatv((self->cur->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3356); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) TrStr RcCk_ltype(RcCk* self, RcFn* f, TrStr name) {
#line 240 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 241 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 242 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3357 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t3358 = (name); _Bool _cmpr_t3359 = _tr_str_eqv(_cmpa_t3357, _cmpb_t3358); _tr_str_release(_cmpa_t3357); _cmpr_t3359; })) {
#line 242 "src/racecheck.tr"
            return List_TrStr_get(f->ptypes, i);
        }
#line 243 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 244 "src/racecheck.tr"
    i = (f->ltn->len - 1LL);
#line 245 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 246 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3360 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3361 = (name); _Bool _cmpr_t3362 = _tr_str_eqv(_cmpa_t3360, _cmpb_t3361); _tr_str_release(_cmpa_t3360); _cmpr_t3362; })) {
#line 246 "src/racecheck.tr"
            return List_TrStr_get(f->ltt, i);
        }
#line 247 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 248 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr RcCk_ltarg_of(RcCk* self, RcFn* f, TrStr name) {
#line 251 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 252 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 253 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3363 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t3364 = (name); _Bool _cmpr_t3365 = _tr_str_eqv(_cmpa_t3363, _cmpb_t3364); _tr_str_release(_cmpa_t3363); _cmpr_t3365; })) {
#line 253 "src/racecheck.tr"
            return List_TrStr_get(f->pinner, i);
        }
#line 254 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 255 "src/racecheck.tr"
    i = (f->ltn->len - 1LL);
#line 256 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 257 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3366 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3367 = (name); _Bool _cmpr_t3368 = _tr_str_eqv(_cmpa_t3366, _cmpb_t3367); _tr_str_release(_cmpa_t3366); _cmpr_t3368; })) {
#line 257 "src/racecheck.tr"
            return List_TrStr_get(f->ltarg, i);
        }
#line 258 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 259 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr RcCk_lclass(RcCk* self, RcFn* f, TrStr name) {
#line 263 "src/racecheck.tr"
    TrStr t = RcCk_ltype(self, f, name);
#line 264 "src/racecheck.tr"
    if (_tr_str_eqv((t), (_tr_str_lit_len("Shared", 6LL)))) {
#line 264 "src/racecheck.tr"
        TrStr _trret585 = RcCk_ltarg_of(self, f, name);
        _tr_str_release(t);
        return _trret585;
    }
#line 265 "src/racecheck.tr"
    TrStr _trret586 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret586;
}

__attribute__((hot)) long long RcCk_ldepth_of(RcCk* self, RcFn* f, TrStr name) {
#line 268 "src/racecheck.tr"
    if (_rc_has(f->params, name)) {
#line 268 "src/racecheck.tr"
        return 0LL;
    }
#line 269 "src/racecheck.tr"
    long long i = (f->ltn->len - 1LL);
#line 270 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 271 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3369 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3370 = (name); _Bool _cmpr_t3371 = _tr_str_eqv(_cmpa_t3369, _cmpb_t3370); _tr_str_release(_cmpa_t3369); _cmpr_t3371; })) {
#line 271 "src/racecheck.tr"
            return List_i64_get(f->ldepth, i);
        }
#line 272 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 273 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool RcCk_mentions(RcCk* self, Stmt* sp, TrStr name) {
#line 277 "src/racecheck.tr"
    RcFn* tmp = ({ TrStr _apt_t3372 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t3372, self->cur->line, true)); _tr_str_release(_apt_t3372); _wr; });
#line 278 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 279 "src/racecheck.tr"
    long long sl = self->cur_line;
#line 280 "src/racecheck.tr"
    bool ss = self->seen_spawn;
#line 281 "src/racecheck.tr"
    RcFn* _cltmp_t3373 = _tr_obj_retain(tmp);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3373;
#line 282 "src/racecheck.tr"
    RcCk_stmt(self, sp);
#line 283 "src/racecheck.tr"
    RcFn* _cltmp_t3374 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3374;
#line 284 "src/racecheck.tr"
    self->cur_line = sl;
#line 285 "src/racecheck.tr"
    self->seen_spawn = ss;
#line 286 "src/racecheck.tr"
    bool _trret587 = _rc_has(tmp->idents, name);
    _tr_obj_release(tmp, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
    return _trret587;
}

__attribute__((hot)) void RcCk_note(RcCk* self, TrStr name, bool w) {
#line 289 "src/racecheck.tr"
    if ((_rc_has(self->cur->locals, name) || _rc_has(self->cur->params, name))) {
#line 289 "src/racecheck.tr"
        return;
    }
#line 290 "src/racecheck.tr"
    List_TrStr_append(self->cur->touched, name);
#line 291 "src/racecheck.tr"
    List_bool_append(self->cur->touched_w, w);
#line 292 "src/racecheck.tr"
    List_bool_append(self->cur->touched_after, self->seen_spawn);
#line 293 "src/racecheck.tr"
    List_i64_append(self->cur->touched_line, self->cur_line);
}

__attribute__((hot)) void RcCk_wpath(RcCk* self, Expr* target) {
#line 296 "src/racecheck.tr"
    TrStr r = _rc_root(target);
#line 297 "src/racecheck.tr"
    if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 297 "src/racecheck.tr"
        _tr_str_release(r);
        return;
    }
#line 298 "src/racecheck.tr"
    List_TrStr_append(self->cur->wroot, r);
#line 299 "src/racecheck.tr"
    ({ TrStr _ord_t3375 = (_rc_first_prop(target)); List_TrStr_append(self->cur->wprop, _ord_t3375); _tr_str_release(_ord_t3375); });
#line 300 "src/racecheck.tr"
    List_bool_append(self->cur->wafter, self->seen_spawn);
#line 301 "src/racecheck.tr"
    RcCk_note(self, r, true);
    _tr_str_release(r);
}

__attribute__((hot)) void RcCk_block(RcCk* self, Block* b) {
#line 304 "src/racecheck.tr"
    long long i = 0LL;
#line 305 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 306 "src/racecheck.tr"
        RcCk_stmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
#line 307 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 308 "src/racecheck.tr"
    RcCk_lock_scan(self, b);
}

__attribute__((hot)) void RcCk_pblock(RcCk* self, Block** pb) {
#line 311 "src/racecheck.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 311 "src/racecheck.tr"
        return;
    }
#line 312 "src/racecheck.tr"
    RcCk_block(self, (*pb));
}

__attribute__((hot)) void RcCk_declare(RcCk* self, TrStr name) {
#line 315 "src/racecheck.tr"
    List_TrStr_append(self->cur->locals, name);
}

__attribute__((hot)) void RcCk_inherit_held(RcCk* self, Stmt* sp, List_TrStr* held) {
#line 321 "src/racecheck.tr"
    __auto_type _t3376 = (*sp);
    if (_t3376.tag == Stmt_SIf) {
        __auto_type tb = _t3376.data.SIf.then_b;
__auto_type elifs = _t3376.data.SIf.elifs;
__auto_type eb = _t3376.data.SIf.else_b;
#line 323 "src/racecheck.tr"
        RcCk_rescan_held(self, tb, held);
#line 324 "src/racecheck.tr"
        RcCk_rescan_held(self, eb, held);
#line 325 "src/racecheck.tr"
        long long ei = 0LL;
#line 326 "src/racecheck.tr"
        while ((ei < elifs->len)) {
#line 327 "src/racecheck.tr"
            if ((((unsigned long long)(((ElifClause*)List_ptr_get(elifs, ei))->body)) != ((unsigned long long)(0LL)))) {
#line 327 "src/racecheck.tr"
                RcCk_rescan_held(self, (*((ElifClause*)List_ptr_get(elifs, ei))->body), held);
            }
#line 328 "src/racecheck.tr"
            ei = (ei + 1LL);
        }
    } else if (_t3376.tag == Stmt_SWhile) {
        __auto_type wb = _t3376.data.SWhile.body;
        RcCk_rescan_held(self, wb, held);
    } else if (_t3376.tag == Stmt_SFor) {
        __auto_type fb = _t3376.data.SFor.body;
        RcCk_rescan_held(self, fb, held);
    } else if (_t3376.tag == Stmt_SForUnpack) {
        __auto_type fub = _t3376.data.SForUnpack.body;
        RcCk_rescan_held(self, fub, held);
    } else if (_t3376.tag == Stmt_SUnsafe) {
        __auto_type ub = _t3376.data.SUnsafe.body;
        RcCk_rescan_held(self, ub, held);
    } else if (_t3376.tag == Stmt_SWith) {
        __auto_type wb2 = _t3376.data.SWith.body;
        RcCk_rescan_held(self, wb2, held);
    } else if (_t3376.tag == Stmt_STry) {
        __auto_type tb2 = _t3376.data.STry.try_body;
        RcCk_rescan_held(self, tb2, held);
    } else if (1) {
        __auto_type _ = _t3376;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_rescan_held(RcCk* self, Block* nb, List_TrStr* held) {
#line 338 "src/racecheck.tr"
    TrStr key = _tr_str_wrap(_tr_uint_to_str((unsigned long long)(((unsigned long long)(nb)))));
#line 339 "src/racecheck.tr"
    _tr_dict_set(self->held_for, _tr_strz(key), held);
#line 340 "src/racecheck.tr"
    long long n0 = self->cur->msgs->len;
#line 341 "src/racecheck.tr"
    RcCk_lock_scan(self, nb);
#line 342 "src/racecheck.tr"
    while ((self->cur->msgs->len > n0)) {
#line 342 "src/racecheck.tr"
        List_TrStr_pop(self->cur->msgs);
    }
#line 343 "src/racecheck.tr"
    _tr_dict_remove(self->held_for, _tr_strz(key));
    _tr_str_release(key);
}

__attribute__((hot)) void RcCk_lock_scan(RcCk* self, Block* b) {
#line 346 "src/racecheck.tr"
    List_TrStr* names = (void*)List_TrStr_new();
#line 347 "src/racecheck.tr"
    List_i64* state = (void*)List_i64_new();
#line 348 "src/racecheck.tr"
    List_TrStr* kinds = (void*)List_TrStr_new();
#line 349 "src/racecheck.tr"
    TrStr bkey = _tr_str_wrap(_tr_uint_to_str((unsigned long long)(((unsigned long long)(b)))));
#line 350 "src/racecheck.tr"
    if (_tr_dict_contains(self->held_for, _tr_strz(bkey))) {
#line 351 "src/racecheck.tr"
        List_TrStr* hv = ((List_TrStr*)(uintptr_t)_tr_dict_get(self->held_for, _tr_strz(bkey)));
#line 352 "src/racecheck.tr"
        long long hk = 0LL;
#line 353 "src/racecheck.tr"
        while ((hk < hv->len)) {
#line 354 "src/racecheck.tr"
            ({ TrStr _at_t3377 = (List_TrStr_get(hv, hk)); List_TrStr_append(names, _at_t3377); _tr_str_release(_at_t3377); });
#line 355 "src/racecheck.tr"
            List_i64_append(state, 1LL);
#line 356 "src/racecheck.tr"
            List_TrStr_append(kinds, _tr_str_lit_len("Mutex", 5LL));
#line 357 "src/racecheck.tr"
            hk = (hk + 1LL);
        }
    }
#line 358 "src/racecheck.tr"
    List_TrStr* guards = (void*)List_TrStr_new();
#line 359 "src/racecheck.tr"
    List_TrStr* gmx = (void*)List_TrStr_new();
#line 360 "src/racecheck.tr"
    List_bool* grel = (void*)List_bool_new();
#line 361 "src/racecheck.tr"
    long long line = self->cur->line;
#line 362 "src/racecheck.tr"
    long long i = 0LL;
#line 363 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 364 "src/racecheck.tr"
        Stmt* sp = ((Stmt*)List_ptr_get(b->stmts, i));
#line 365 "src/racecheck.tr"
        i = (i + 1LL);
#line 366 "src/racecheck.tr"
        Expr* call = (Expr*)(0LL);
#line 367 "src/racecheck.tr"
        bool simple = true;
#line 368 "src/racecheck.tr"
        bool is_line = false;
#line 369 "src/racecheck.tr"
        TrStr letname = _tr_str_lit_len("", 0LL);
#line 370 "src/racecheck.tr"
        __auto_type _t3378 = (*sp);
        if (_t3378.tag == Stmt_SLine) {
            __auto_type n = _t3378.data.SLine.n;
#line 372 "src/racecheck.tr"
            line = n;
#line 373 "src/racecheck.tr"
            is_line = true;
        } else if (_t3378.tag == Stmt_SExpr) {
            __auto_type e = _t3378.data.SExpr.expr;
            call = e;
        } else if (_t3378.tag == Stmt_SLet) {
            __auto_type lnm = _t3378.data.SLet.name;
__auto_type v = _t3378.data.SLet.val;
#line 376 "src/racecheck.tr"
            call = v;
#line 377 "src/racecheck.tr"
            TrStr _strtmp_t3379 = _tr_str_retain(lnm);
            _tr_str_release(letname);
            letname = _strtmp_t3379;
        } else if (_t3378.tag == Stmt_SAssign) {
            __auto_type tg2 = _t3378.data.SAssign.target;
__auto_type v2 = _t3378.data.SAssign.val;
#line 379 "src/racecheck.tr"
            call = v2;
#line 381 "src/racecheck.tr"
            if ((!_rc_nul(tg2))) {
#line 382 "src/racecheck.tr"
                __auto_type _t3380 = (*tg2);
                if (_t3380.tag == Expr_EIdent) {
                    __auto_type tgn = _t3380.data.EIdent.name;
#line 384 "src/racecheck.tr"
                    if ((!_rc_has(self->cur->params, tgn))) {
#line 384 "src/racecheck.tr"
                        TrStr _strtmp_t3381 = _tr_str_retain(tgn);
                        _tr_str_release(letname);
                        letname = _strtmp_t3381;
                    }
                } else if (1) {
                    __auto_type _ = _t3380;
                    /* pass */
                }
            }
        } else if (1) {
            __auto_type _ = _t3378;
            simple = false;
        }
#line 387 "src/racecheck.tr"
        if (is_line) {
#line 387 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 388 "src/racecheck.tr"
        long long gk = 0LL;
#line 389 "src/racecheck.tr"
        while ((gk < guards->len)) {
#line 390 "src/racecheck.tr"
            if (((List_bool_get(grel, gk) && ({ TrStr _cmpa_t3382 = (List_TrStr_get(guards, gk)); TrStr _cmpb_t3383 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3384 = !_tr_str_eqv(_cmpa_t3382, _cmpb_t3383); _tr_str_release(_cmpa_t3382); _cmpr_t3384; })) && ({ TrStr _at_t3385 = (List_TrStr_get(guards, gk)); __auto_type _wr = (RcCk_mentions(self, sp, _at_t3385)); _tr_str_release(_at_t3385); _wr; }))) {
#line 391 "src/racecheck.tr"
                ({ __auto_type _ord_t3386 = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3387 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3386); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was obtained from `", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3388 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3387); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()` and is used after the lock was released: the guarded object is then reachable by other threads while this one still reads/writes it (a data race).\n      FIX: keep every use of '", 185LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3389 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3388); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before `", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3390 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3389); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` / `", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3391 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3390); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(..)`, or copy the data out while the lock is held.", 55LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3391); _tr_str_release(_ord_t3391); });
#line 392 "src/racecheck.tr"
                List_TrStr_set(guards, gk, _tr_str_lit_len("", 0LL));
            }
#line 393 "src/racecheck.tr"
            gk = (gk + 1LL);
        }
#line 394 "src/racecheck.tr"
        if ((!simple)) {
#line 397 "src/racecheck.tr"
            List_TrStr* heldv = (void*)List_TrStr_new();
#line 398 "src/racecheck.tr"
            long long hj = 0LL;
#line 399 "src/racecheck.tr"
            while ((hj < names->len)) {
#line 400 "src/racecheck.tr"
                if ((({ TrStr _cmpa_t3392 = (List_TrStr_get(kinds, hj)); TrStr _cmpb_t3393 = (_tr_str_lit_len("Mutex", 5LL)); _Bool _cmpr_t3394 = _tr_str_eqv(_cmpa_t3392, _cmpb_t3393); _tr_str_release(_cmpa_t3392); _cmpr_t3394; }) && (List_i64_get(state, hj) == 1LL))) {
#line 400 "src/racecheck.tr"
                    ({ TrStr _at_t3395 = (List_TrStr_get(names, hj)); List_TrStr_append(heldv, _at_t3395); _tr_str_release(_at_t3395); });
                }
#line 401 "src/racecheck.tr"
                hj = (hj + 1LL);
            }
#line 402 "src/racecheck.tr"
            if ((heldv->len > 0LL)) {
#line 402 "src/racecheck.tr"
                RcCk_inherit_held(self, sp, heldv);
            }
#line 403 "src/racecheck.tr"
            names = (void*)List_TrStr_new();
#line 404 "src/racecheck.tr"
            state = (void*)List_i64_new();
#line 405 "src/racecheck.tr"
            kinds = (void*)List_TrStr_new();
#line 406 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 407 "src/racecheck.tr"
        if (_rc_nul(call)) {
#line 407 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 408 "src/racecheck.tr"
        __auto_type _t3396 = (*call);
        if (_t3396.tag == Expr_EMethodCall) {
            __auto_type o = _t3396.data.EMethodCall.obj;
__auto_type m = _t3396.data.EMethodCall.method;
__auto_type margs = _t3396.data.EMethodCall.args;
#line 410 "src/racecheck.tr"
            TrStr r = _tr_str_lit_len("", 0LL);
#line 411 "src/racecheck.tr"
            if ((!_rc_nul(o))) {
#line 412 "src/racecheck.tr"
                __auto_type _t3397 = (*o);
                if (_t3397.tag == Expr_EIdent) {
                    __auto_type rn = _t3397.data.EIdent.name;
                    TrStr _strtmp_t3398 = _tr_str_retain(rn);
                    _tr_str_release(r);
                    r = _strtmp_t3398;
                } else if (1) {
                    __auto_type _ = _t3397;
                    /* pass */
                }
            }
#line 415 "src/racecheck.tr"
            if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 415 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(letname);
                continue;
            }
#line 416 "src/racecheck.tr"
            TrStr k = RcCk_ltype(self, self->cur, r);
#line 417 "src/racecheck.tr"
            if (((_tr_str_eqv((k), (_tr_str_lit_len("", 0LL))) && (!_rc_has(self->cur->locals, r))) && (!_rc_has(self->cur->params, r)))) {
#line 418 "src/racecheck.tr"
                long long gq = 0LL;
#line 419 "src/racecheck.tr"
                while ((gq < self->gnames->len)) {
#line 420 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3399 = (List_TrStr_get(self->gnames, gq)); TrStr _cmpb_t3400 = (r); _Bool _cmpr_t3401 = _tr_str_eqv(_cmpa_t3399, _cmpb_t3400); _tr_str_release(_cmpa_t3399); _cmpr_t3401; })) {
#line 420 "src/racecheck.tr"
                        TrStr _strtmp_t3402 = List_TrStr_get(self->gtn, gq);
                        _tr_str_release(k);
                        k = _strtmp_t3402;
                    }
#line 421 "src/racecheck.tr"
                    gq = (gq + 1LL);
                }
            }
#line 422 "src/racecheck.tr"
            if ((!((_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))))) {
#line 422 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                _tr_str_release(letname);
                continue;
            }
#line 423 "src/racecheck.tr"
            if ((((margs->len != 0LL) && (!_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) && (!_tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL)))))) {
#line 423 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                _tr_str_release(letname);
                continue;
            }
#line 424 "src/racecheck.tr"
            long long idx = (0LL - 1LL);
#line 425 "src/racecheck.tr"
            long long j = 0LL;
#line 426 "src/racecheck.tr"
            while ((j < names->len)) {
#line 427 "src/racecheck.tr"
                if (({ TrStr _cmpa_t3403 = (List_TrStr_get(names, j)); TrStr _cmpb_t3404 = (r); _Bool _cmpr_t3405 = _tr_str_eqv(_cmpa_t3403, _cmpb_t3404); _tr_str_release(_cmpa_t3403); _cmpr_t3405; })) {
#line 427 "src/racecheck.tr"
                    idx = j;
                }
#line 428 "src/racecheck.tr"
                j = (j + 1LL);
            }
#line 429 "src/racecheck.tr"
            if ((idx < 0LL)) {
#line 430 "src/racecheck.tr"
                List_TrStr_append(names, r);
#line 431 "src/racecheck.tr"
                List_i64_append(state, 0LL);
#line 432 "src/racecheck.tr"
                List_TrStr_append(kinds, k);
#line 433 "src/racecheck.tr"
                idx = (names->len - 1LL);
            }
#line 434 "src/racecheck.tr"
            long long st = List_i64_get(state, idx);
#line 435 "src/racecheck.tr"
            if (_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 436 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) {
#line 437 "src/racecheck.tr"
                    long long gj2 = 0LL;
#line 438 "src/racecheck.tr"
                    while ((gj2 < names->len)) {
#line 439 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3406 = (List_TrStr_get(kinds, gj2)); TrStr _cmpb_t3407 = (_tr_str_lit_len("Mutex", 5LL)); _Bool _cmpr_t3408 = _tr_str_eqv(_cmpa_t3406, _cmpb_t3407); _tr_str_release(_cmpa_t3406); _cmpr_t3408; }) && (List_i64_get(state, gj2) == 1LL)) && ({ TrStr _cmpa_t3409 = (List_TrStr_get(names, gj2)); TrStr _cmpb_t3410 = (r); _Bool _cmpr_t3411 = !_tr_str_eqv(_cmpa_t3409, _cmpb_t3410); _tr_str_release(_cmpa_t3409); _cmpr_t3411; }))) {
#line 440 "src/racecheck.tr"
                            if ((!(((_rc_has(self->cur->locals, r) || _rc_has(self->cur->params, r)) || ({ __auto_type _ord_t3413 = (self->cur->locals); TrStr _at_t3412 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3413, _at_t3412)); _tr_str_release(_at_t3412); _wr; })) || ({ __auto_type _ord_t3415 = (self->cur->params); TrStr _at_t3414 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3415, _at_t3414)); _tr_str_release(_at_t3414); _wr; })))) {
#line 441 "src/racecheck.tr"
                                bool pdup = false;
#line 442 "src/racecheck.tr"
                                long long pdk = 0LL;
#line 443 "src/racecheck.tr"
                                while ((pdk < self->cur->pairs_a->len)) {
#line 444 "src/racecheck.tr"
                                    if ((({ __auto_type _ord_t3416 = (List_TrStr_get(self->cur->pairs_a, pdk)); __auto_type _wr = (({ TrStr _cmpa_t3417 = (_ord_t3416); TrStr _cmpb_t3418 = (List_TrStr_get(names, gj2)); _Bool _cmpr_t3419 = _tr_str_eqv(_cmpa_t3417, _cmpb_t3418); _tr_str_release(_cmpa_t3417); _tr_str_release(_cmpb_t3418); _cmpr_t3419; })); _wr; }) && ({ TrStr _cmpa_t3420 = (List_TrStr_get(self->cur->pairs_b, pdk)); TrStr _cmpb_t3421 = (r); _Bool _cmpr_t3422 = _tr_str_eqv(_cmpa_t3420, _cmpb_t3421); _tr_str_release(_cmpa_t3420); _cmpr_t3422; }))) {
#line 444 "src/racecheck.tr"
                                        pdup = true;
                                    }
#line 445 "src/racecheck.tr"
                                    pdk = (pdk + 1LL);
                                }
#line 446 "src/racecheck.tr"
                                if ((!pdup)) {
#line 447 "src/racecheck.tr"
                                    ({ TrStr _ord_t3423 = (List_TrStr_get(names, gj2)); List_TrStr_append(self->cur->pairs_a, _ord_t3423); _tr_str_release(_ord_t3423); });
#line 448 "src/racecheck.tr"
                                    List_TrStr_append(self->cur->pairs_b, r);
                                }
                            }
                        }
#line 449 "src/racecheck.tr"
                        gj2 = (gj2 + 1LL);
                    }
#line 450 "src/racecheck.tr"
                    TrStr pa = RcCk_ltarg_of(self, self->cur, r);
#line 451 "src/racecheck.tr"
                    long long ci0 = RcCk_find_class(self, pa);
#line 452 "src/racecheck.tr"
                    if (((!_tr_str_eqv((letname), (_tr_str_lit_len("", 0LL)))) && (((((_tr_str_eqv((pa), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((pa), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Map", 3LL)))) || ((ci0 >= 0LL) && (!List_bool_get(self->class_lib, ci0)))))) {
#line 453 "src/racecheck.tr"
                        List_TrStr_append(guards, letname);
#line 454 "src/racecheck.tr"
                        List_TrStr_append(gmx, r);
#line 455 "src/racecheck.tr"
                        List_bool_append(grel, false);
                    }
#line 456 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 456 "src/racecheck.tr"
                        ({ TrStr _ord_t3424 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()' acquires the lock again while this thread already holds it (no `set()`/`unlock()` since the previous `get()`): the second call blocks forever.\n      FIX: finish with `", 175LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(v)` or `", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` before locking again, or reuse the first value.", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3424); _tr_str_release(_ord_t3424); });
                    }
#line 457 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("unlock", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) {
#line 459 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
#line 460 "src/racecheck.tr"
                    long long gr = 0LL;
#line 461 "src/racecheck.tr"
                    while ((gr < guards->len)) {
#line 462 "src/racecheck.tr"
                        if (({ TrStr _cmpa_t3425 = (List_TrStr_get(gmx, gr)); TrStr _cmpb_t3426 = (r); _Bool _cmpr_t3427 = _tr_str_eqv(_cmpa_t3425, _cmpb_t3426); _tr_str_release(_cmpa_t3425); _cmpr_t3427; })) {
#line 462 "src/racecheck.tr"
                            List_bool_set(grel, gr, true);
                        }
#line 463 "src/racecheck.tr"
                        gr = (gr + 1LL);
                    }
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 465 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL)))) {
#line 465 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("read_unlock", 11LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL))))) {
#line 466 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
                } else if (_tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) {
#line 468 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 468 "src/racecheck.tr"
                        ({ TrStr _ord_t3428 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()' takes the write lock while this thread still holds the read lock from `", 81LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read()`: the write waits for readers (itself included) to leave, which never happens (deadlock).\n      FIX: call `", 115LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read_unlock()` before `", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()`.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3428); _tr_str_release(_ord_t3428); });
                    }
#line 469 "src/racecheck.tr"
                    List_i64_set(state, idx, 2LL);
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))) {
#line 471 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL)))) {
#line 472 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 472 "src/racecheck.tr"
                        ({ TrStr _ord_t3429 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] thread '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is joined twice: joining an already-joined thread handle is undefined behaviour.\n      FIX: join each thread exactly once.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3429); _tr_str_release(_ord_t3429); });
                    }
#line 473 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                }
            }
            _tr_str_release(r);
            _tr_str_release(k);
        } else if (1) {
            __auto_type _ = _t3396;
            /* pass */
        }
        _tr_str_release(letname);
    }
    List_TrStr_free(names);
    List_i64_free(state);
    List_TrStr_free(kinds);
    _tr_str_release(bkey);
    List_TrStr_free(guards);
    List_TrStr_free(gmx);
    List_bool_free(grel);
}

__attribute__((hot)) void RcCk_stmt(RcCk* self, Stmt* sp) {
#line 477 "src/racecheck.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 477 "src/racecheck.tr"
        return;
    }
#line 478 "src/racecheck.tr"
    __auto_type _t3430 = (*sp);
    if (_t3430.tag == Stmt_SLine) {
        __auto_type n = _t3430.data.SLine.n;
        self->cur_line = n;
    } else if (_t3430.tag == Stmt_SExpr) {
        __auto_type e = _t3430.data.SExpr.expr;
        RcCk_expr(self, e);
    } else if (_t3430.tag == Stmt_SLet) {
        __auto_type name = _t3430.data.SLet.name;
__auto_type is_sh = _t3430.data.SLet.is_shared;
__auto_type ty = _t3430.data.SLet.ty;
__auto_type v = _t3430.data.SLet.val;
#line 482 "src/racecheck.tr"
        ({ TrStr _apt_t3431 = _tr_str_retain(self->cur->file); RcCk_ty_check(self, ty, _apt_t3431, self->cur_line); _tr_str_release(_apt_t3431); });
#line 483 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 484 "src/racecheck.tr"
        RcCk_declare(self, name);
#line 485 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 486 "src/racecheck.tr"
        if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 486 "src/racecheck.tr"
            TrStr _strtmp_t3432 = _tr_str_retain((*ty)->name);
            _tr_str_release(tn);
            tn = _strtmp_t3432;
        }
#line 487 "src/racecheck.tr"
        if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 487 "src/racecheck.tr"
            TrStr _strtmp_t3433 = _rc_ctor_name(v);
            _tr_str_release(tn);
            tn = _strtmp_t3433;
        }
#line 488 "src/racecheck.tr"
        TrStr targ = _rc_ctor_arg(v);
#line 489 "src/racecheck.tr"
        if (((((unsigned long long)(ty)) != ((unsigned long long)(0LL))) && ((*ty)->args->len > 0LL))) {
#line 489 "src/racecheck.tr"
            TrStr _strtmp_t3434 = _tr_str_retain((*((AstType**)List_ptr_get((*ty)->args, 0LL)))->name);
            _tr_str_release(targ);
            targ = _strtmp_t3434;
        }
#line 490 "src/racecheck.tr"
        if ((is_sh && (!_rc_nul(v)))) {
#line 491 "src/racecheck.tr"
            __auto_type _t3435 = (*v);
            if (_t3435.tag == Expr_EIdent) {
                __auto_type src = _t3435.data.EIdent.name;
#line 493 "src/racecheck.tr"
                TrStr _strtmp_t3436 = RcCk_ltype(self, self->cur, src);
                _tr_str_release(tn);
                tn = _strtmp_t3436;
#line 494 "src/racecheck.tr"
                TrStr _strtmp_t3437 = RcCk_ltarg_of(self, self->cur, src);
                _tr_str_release(targ);
                targ = _strtmp_t3437;
            } else if (1) {
                __auto_type _ = _t3435;
                /* pass */
            }
        }
#line 496 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltn, name);
#line 497 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltt, tn);
#line 498 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltarg, targ);
#line 499 "src/racecheck.tr"
        List_i64_append(self->cur->ldepth, self->loop_depth);
#line 500 "src/racecheck.tr"
        if (_rc_is_thread_spawn(v)) {
#line 501 "src/racecheck.tr"
            List_TrStr_append(self->cur->thr_made, name);
#line 502 "src/racecheck.tr"
            List_i64_append(self->cur->thr_line, self->cur_line);
        }
#line 503 "src/racecheck.tr"
        if (is_sh) {
#line 503 "src/racecheck.tr"
            List_TrStr_append(self->cur->shvars, name);
        }
#line 504 "src/racecheck.tr"
        if (_rc_is_lambda(v)) {
#line 504 "src/racecheck.tr"
            List_TrStr_append(self->cur->closures, name);
        }
        _tr_str_release(tn);
        _tr_str_release(targ);
    } else if (_t3430.tag == Stmt_SMultiLet) {
        __auto_type names = _t3430.data.SMultiLet.names;
__auto_type v = _t3430.data.SMultiLet.val;
#line 506 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 507 "src/racecheck.tr"
        long long mi = 0LL;
#line 508 "src/racecheck.tr"
        while ((mi < names->len)) {
#line 509 "src/racecheck.tr"
            ({ TrStr _at_t3438 = (List_TrStr_get(names, mi)); RcCk_declare(self, _at_t3438); _tr_str_release(_at_t3438); });
#line 510 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t3430.tag == Stmt_SAssign) {
        __auto_type t = _t3430.data.SAssign.target;
__auto_type v = _t3430.data.SAssign.val;
#line 512 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 513 "src/racecheck.tr"
        RcCk_expr(self, t);
#line 514 "src/racecheck.tr"
        RcCk_wpath(self, t);
#line 516 "src/racecheck.tr"
        if ((!_rc_nul(t))) {
#line 517 "src/racecheck.tr"
            __auto_type _t3439 = (*t);
            if (_t3439.tag == Expr_EIdent) {
                __auto_type bn = _t3439.data.EIdent.name;
#line 519 "src/racecheck.tr"
                if ((({ TrStr _cmpa_t3440 = (RcCk_ltype(self, self->cur, bn)); TrStr _cmpb_t3441 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3442 = _tr_str_eqv(_cmpa_t3440, _cmpb_t3441); _tr_str_release(_cmpa_t3440); _cmpr_t3442; }) && (!_rc_has(self->cur->params, bn)))) {
#line 520 "src/racecheck.tr"
                    List_TrStr_append(self->cur->ltn, bn);
#line 521 "src/racecheck.tr"
                    ({ TrStr _ord_t3443 = (_rc_ctor_name(v)); List_TrStr_append(self->cur->ltt, _ord_t3443); _tr_str_release(_ord_t3443); });
#line 522 "src/racecheck.tr"
                    ({ TrStr _ord_t3444 = (_rc_ctor_arg(v)); List_TrStr_append(self->cur->ltarg, _ord_t3444); _tr_str_release(_ord_t3444); });
#line 523 "src/racecheck.tr"
                    List_i64_append(self->cur->ldepth, self->loop_depth);
                }
            } else if (1) {
                __auto_type _ = _t3439;
                /* pass */
            }
        }
    } else if (_t3430.tag == Stmt_SReturn) {
        __auto_type v = _t3430.data.SReturn.val;
        RcCk_expr(self, v);
    } else if (_t3430.tag == Stmt_SBreak) {
        __auto_type v = _t3430.data.SBreak.val;
        RcCk_expr(self, v);
    } else if (_t3430.tag == Stmt_SRaise) {
        __auto_type v = _t3430.data.SRaise.val;
        RcCk_expr(self, v);
    } else if (_t3430.tag == Stmt_SUnsafe) {
        __auto_type b = _t3430.data.SUnsafe.body;
        RcCk_block(self, b);
    } else if (_t3430.tag == Stmt_SIf) {
        __auto_type c = _t3430.data.SIf.cond;
__auto_type tb = _t3430.data.SIf.then_b;
__auto_type elifs = _t3430.data.SIf.elifs;
__auto_type eb = _t3430.data.SIf.else_b;
#line 530 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 531 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 532 "src/racecheck.tr"
        long long ei = 0LL;
#line 533 "src/racecheck.tr"
        while ((ei < elifs->len)) {
#line 534 "src/racecheck.tr"
            RcCk_expr(self, ((ElifClause*)List_ptr_get(elifs, ei))->cond);
#line 535 "src/racecheck.tr"
            RcCk_pblock(self, ((ElifClause*)List_ptr_get(elifs, ei))->body);
#line 536 "src/racecheck.tr"
            ei = (ei + 1LL);
        }
#line 537 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (_t3430.tag == Stmt_SWhile) {
        __auto_type c = _t3430.data.SWhile.cond;
__auto_type b = _t3430.data.SWhile.body;
#line 539 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 540 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 541 "src/racecheck.tr"
        RcCk_block(self, b);
#line 542 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3430.tag == Stmt_SFor) {
        __auto_type v = _t3430.data.SFor.var;
__auto_type it = _t3430.data.SFor.iter;
__auto_type b = _t3430.data.SFor.body;
#line 544 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 545 "src/racecheck.tr"
        RcCk_declare(self, v);
#line 546 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 547 "src/racecheck.tr"
        RcCk_block(self, b);
#line 548 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3430.tag == Stmt_SForUnpack) {
        __auto_type vs = _t3430.data.SForUnpack.vars;
__auto_type it = _t3430.data.SForUnpack.iter;
__auto_type b = _t3430.data.SForUnpack.body;
#line 550 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 551 "src/racecheck.tr"
        long long fi = 0LL;
#line 552 "src/racecheck.tr"
        while ((fi < vs->len)) {
#line 553 "src/racecheck.tr"
            ({ TrStr _at_t3445 = (List_TrStr_get(vs, fi)); RcCk_declare(self, _at_t3445); _tr_str_release(_at_t3445); });
#line 554 "src/racecheck.tr"
            fi = (fi + 1LL);
        }
#line 555 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 556 "src/racecheck.tr"
        RcCk_block(self, b);
#line 557 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3430.tag == Stmt_SMatch) {
        __auto_type e = _t3430.data.SMatch.expr;
__auto_type arms = _t3430.data.SMatch.arms;
#line 559 "src/racecheck.tr"
        RcCk_expr(self, e);
#line 560 "src/racecheck.tr"
        long long ai = 0LL;
#line 561 "src/racecheck.tr"
        while ((ai < arms->len)) {
#line 562 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, ai))->guard);
#line 563 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, ai))->body);
#line 564 "src/racecheck.tr"
            ai = (ai + 1LL);
        }
    } else if (_t3430.tag == Stmt_STry) {
        __auto_type tb = _t3430.data.STry.try_body;
__auto_type catches = _t3430.data.STry.catches;
__auto_type fb = _t3430.data.STry.finally_b;
#line 566 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 567 "src/racecheck.tr"
        long long ci = 0LL;
#line 568 "src/racecheck.tr"
        while ((ci < catches->len)) {
#line 569 "src/racecheck.tr"
            ({ TrStr _apt_t3446 = _tr_str_retain((*((CatchClause**)List_ptr_get(catches, ci)))->err_name); RcCk_declare(self, _apt_t3446); _tr_str_release(_apt_t3446); });
#line 570 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci)))->body);
#line 571 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 572 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t3430.tag == Stmt_SAssert) {
        __auto_type c = _t3430.data.SAssert.cond;
__auto_type m = _t3430.data.SAssert.msg;
#line 574 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 575 "src/racecheck.tr"
        RcCk_expr(self, m);
    } else if (_t3430.tag == Stmt_SWith) {
        __auto_type items = _t3430.data.SWith.items;
__auto_type aliases = _t3430.data.SWith.aliases;
__auto_type b = _t3430.data.SWith.body;
#line 577 "src/racecheck.tr"
        long long wi = 0LL;
#line 578 "src/racecheck.tr"
        while ((wi < items->len)) {
#line 579 "src/racecheck.tr"
            RcCk_expr(self, ((Expr*)List_ptr_get(items, wi)));
#line 580 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 581 "src/racecheck.tr"
        wi = 0LL;
#line 582 "src/racecheck.tr"
        while ((wi < aliases->len)) {
#line 583 "src/racecheck.tr"
            ({ TrStr _at_t3447 = (List_TrStr_get(aliases, wi)); RcCk_declare(self, _at_t3447); _tr_str_release(_at_t3447); });
#line 584 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 585 "src/racecheck.tr"
        RcCk_block(self, b);
    } else if (_t3430.tag == Stmt_SSpawn) {
        __auto_type e = _t3430.data.SSpawn.expr;
#line 587 "src/racecheck.tr"
        RcCk_spawn_expr(self, e);
    } else if (_t3430.tag == Stmt_STaskGroup) {
        __auto_type b = _t3430.data.STaskGroup.body;
#line 589 "src/racecheck.tr"
        RcCk_block(self, b);
#line 590 "src/racecheck.tr"
        self->seen_spawn = false;
    } else if (_t3430.tag == Stmt_SGpuBlock) {
        __auto_type b = _t3430.data.SGpuBlock.body;
        RcCk_block(self, b);
    } else if (_t3430.tag == Stmt_SDefer) {
        __auto_type s = _t3430.data.SDefer.stmt;
        RcCk_stmt(self, s);
    } else if (_t3430.tag == Stmt_SLocalDecl) {
        __auto_type d = _t3430.data.SLocalDecl.decl;
#line 594 "src/racecheck.tr"
        __auto_type _t3448 = (*d);
        if (_t3448.tag == Decl_DFunction) {
            __auto_type f = _t3448.data.DFunction.func;
#line 596 "src/racecheck.tr"
            long long nidx = self->fns->len;
#line 597 "src/racecheck.tr"
            ({ TrStr _apt_t3449 = _tr_str_retain(self->cur->file); RcCk_add_fn(self, f, _apt_t3449, self->cur->is_lib); _tr_str_release(_apt_t3449); });
#line 598 "src/racecheck.tr"
            if (((nidx < self->fns->len) && (!self->cur->is_lib))) {
#line 599 "src/racecheck.tr"
                RcFn* nf = ((RcFn*)List_ptr_get(self->fns, nidx));
#line 600 "src/racecheck.tr"
                long long nk = 0LL;
#line 601 "src/racecheck.tr"
                while ((nk < nf->touched->len)) {
#line 602 "src/racecheck.tr"
                    TrStr nn = List_TrStr_get(nf->touched, nk);
#line 603 "src/racecheck.tr"
                    if ((_rc_has(self->cur->locals, nn) || _rc_has(self->cur->params, nn))) {
#line 604 "src/racecheck.tr"
                        ({ __auto_type _ord_t3450 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[N-3] the nested function '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3451 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3450); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the enclosing function '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3452 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3451); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->cur->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but a nested `def` does not capture the enclosing function's locals (it is an ordinary function).\n      FIX: pass '", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as a parameter, or use a `lambda` (which does capture), or move the value to module level.", 92LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3452); _tr_str_release(_ord_t3452); });
#line 605 "src/racecheck.tr"
                        _tr_str_release(nn);
                        break;
                    }
#line 606 "src/racecheck.tr"
                    nk = (nk + 1LL);
                    _tr_str_release(nn);
                }
            }
        } else if (1) {
            __auto_type _ = _t3448;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t3430;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_spawn_expr(RcCk* self, Expr* e) {
#line 612 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 612 "src/racecheck.tr"
        return;
    }
#line 613 "src/racecheck.tr"
    __auto_type _t3453 = (*e);
    if (_t3453.tag == Expr_ECall) {
        __auto_type c = _t3453.data.ECall.callee;
__auto_type args = _t3453.data.ECall.args;
#line 615 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 616 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 617 "src/racecheck.tr"
            __auto_type _t3454 = (*c);
            if (_t3454.tag == Expr_EIdent) {
                __auto_type n = _t3454.data.EIdent.name;
                TrStr _strtmp_t3455 = _tr_str_retain(n);
                _tr_str_release(tn);
                tn = _strtmp_t3455;
            } else if (1) {
                __auto_type _ = _t3454;
                /* pass */
            }
        }
#line 620 "src/racecheck.tr"
        RcCk_record_site(self, tn, args, 0LL);
#line 621 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(tn);
    } else if (_t3453.tag == Expr_EMethodCall) {
        __auto_type o = _t3453.data.EMethodCall.obj;
__auto_type m = _t3453.data.EMethodCall.method;
__auto_type margs = _t3453.data.EMethodCall.args;
#line 623 "src/racecheck.tr"
        TrStr r = _tr_str_lit_len("", 0LL);
#line 624 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 625 "src/racecheck.tr"
            __auto_type _t3456 = (*o);
            if (_t3456.tag == Expr_EIdent) {
                __auto_type rn = _t3456.data.EIdent.name;
                TrStr _strtmp_t3457 = _tr_str_retain(rn);
                _tr_str_release(r);
                r = _strtmp_t3457;
            } else if (1) {
                __auto_type _ = _t3456;
                /* pass */
            }
        }
#line 628 "src/racecheck.tr"
        if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 629 "src/racecheck.tr"
            RcCk_record_site(self, _tr_str_lit_len("", 0LL), margs, 0LL);
#line 630 "src/racecheck.tr"
            ((RcSite*)List_ptr_get(self->cur->sites, (self->cur->sites->len - 1LL)))->recv = _tr_str_retain(r);
#line 631 "src/racecheck.tr"
            ((RcSite*)List_ptr_get(self->cur->sites, (self->cur->sites->len - 1LL)))->meth = _tr_str_retain(m);
        }
#line 632 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(r);
    } else if (1) {
        __auto_type _ = _t3453;
        RcCk_expr(self, e);
    }
}

__attribute__((hot)) void RcCk_record_site(RcCk* self, TrStr target, List_ptr* args, long long skip) {
#line 636 "src/racecheck.tr"
    RcSite* s = ((RcSite*)_tr_obj_alloc(sizeof(RcSite)));
#line 637 "src/racecheck.tr"
    TrStr _sfo_t3458 = s->target;
    s->target = _tr_str_retain(target);
    _tr_str_release(_sfo_t3458);
#line 638 "src/racecheck.tr"
    s->args = (void*)List_ptr_new();
#line 639 "src/racecheck.tr"
    long long i = skip;
#line 640 "src/racecheck.tr"
    while ((i < args->len)) {
#line 641 "src/racecheck.tr"
        ({ __auto_type _ord_t3459 = (((Expr*)List_ptr_get(args, i))); List_ptr_append(s->args, _ord_t3459); });
#line 642 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 643 "src/racecheck.tr"
    s->line = self->cur_line;
#line 644 "src/racecheck.tr"
    s->in_loop = (self->loop_depth > 0LL);
#line 645 "src/racecheck.tr"
    s->depth = self->loop_depth;
#line 646 "src/racecheck.tr"
    TrStr _sfo_t3460 = s->recv;
    s->recv = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t3460);
#line 647 "src/racecheck.tr"
    TrStr _sfo_t3461 = s->meth;
    s->meth = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t3461);
#line 648 "src/racecheck.tr"
    List_ptr_append(self->cur->sites, _tr_obj_retain(s));
#line 649 "src/racecheck.tr"
    self->seen_spawn = true;
    _tr_obj_release(s, _trdrop_RcSite);
}

__attribute__((hot)) void RcCk_exprs(RcCk* self, List_ptr* v) {
#line 652 "src/racecheck.tr"
    long long i = 0LL;
#line 653 "src/racecheck.tr"
    while ((i < v->len)) {
#line 654 "src/racecheck.tr"
        RcCk_expr(self, ((Expr*)List_ptr_get(v, i)));
#line 655 "src/racecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void RcCk_comps(RcCk* self, Expr* element, List_ptr* gens) {
#line 658 "src/racecheck.tr"
    long long i = 0LL;
#line 659 "src/racecheck.tr"
    while ((i < gens->len)) {
#line 660 "src/racecheck.tr"
        ({ TrStr _apt_t3462 = _tr_str_retain((*((Comprehension**)List_ptr_get(gens, i)))->target); RcCk_declare(self, _apt_t3462); _tr_str_release(_apt_t3462); });
#line 661 "src/racecheck.tr"
        RcCk_expr(self, (*((Comprehension**)List_ptr_get(gens, i)))->iter);
#line 662 "src/racecheck.tr"
        RcCk_exprs(self, (*((Comprehension**)List_ptr_get(gens, i)))->ifs);
#line 663 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 664 "src/racecheck.tr"
    RcCk_expr(self, element);
}

__attribute__((hot)) void RcCk_expr(RcCk* self, Expr* e) {
#line 667 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 667 "src/racecheck.tr"
        return;
    }
#line 668 "src/racecheck.tr"
    __auto_type _t3463 = (*e);
    if (_t3463.tag == Expr_EIdent) {
        __auto_type n = _t3463.data.EIdent.name;
#line 670 "src/racecheck.tr"
        List_TrStr_append(self->cur->idents, n);
#line 671 "src/racecheck.tr"
        RcCk_note(self, n, false);
    } else if (_t3463.tag == Expr_EBinOp) {
        __auto_type l = _t3463.data.EBinOp.left;
__auto_type r = _t3463.data.EBinOp.right;
#line 673 "src/racecheck.tr"
        RcCk_expr(self, l);
#line 674 "src/racecheck.tr"
        RcCk_expr(self, r);
    } else if (_t3463.tag == Expr_EUnaryOp) {
        __auto_type x = _t3463.data.EUnaryOp.expr;
        RcCk_expr(self, x);
    } else if (_t3463.tag == Expr_ECall) {
        __auto_type c = _t3463.data.ECall.callee;
__auto_type args = _t3463.data.ECall.args;
#line 677 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 678 "src/racecheck.tr"
            __auto_type _t3464 = (*c);
            if (_t3464.tag == Expr_EIdent) {
                __auto_type fnm = _t3464.data.EIdent.name;
#line 680 "src/racecheck.tr"
                List_TrStr_append(self->cur->calls, fnm);
#line 681 "src/racecheck.tr"
                long long cai = 0LL;
#line 682 "src/racecheck.tr"
                while ((cai < args->len)) {
#line 683 "src/racecheck.tr"
                    __auto_type _t3465 = (*((Expr*)List_ptr_get(args, cai)));
                    if (_t3465.tag == Expr_EIdent) {
                        __auto_type can = _t3465.data.EIdent.name;
#line 685 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_fn, fnm);
#line 686 "src/racecheck.tr"
                        List_i64_append(self->cur->ca_idx, cai);
#line 687 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_name, can);
                    } else if (1) {
                        __auto_type _ = _t3465;
                        /* pass */
                    }
#line 689 "src/racecheck.tr"
                    cai = (cai + 1LL);
                }
#line 690 "src/racecheck.tr"
                if (_tr_str_eqv((fnm), (_tr_str_lit_len("await_all", 9LL)))) {
#line 691 "src/racecheck.tr"
                    long long ai = 0LL;
#line 692 "src/racecheck.tr"
                    while ((ai < args->len)) {
#line 693 "src/racecheck.tr"
                        __auto_type _t3466 = (*((Expr*)List_ptr_get(args, ai)));
                        if (_t3466.tag == Expr_ECall) {
                            __auto_type c2 = _t3466.data.ECall.callee;
__auto_type a2 = _t3466.data.ECall.args;
#line 695 "src/racecheck.tr"
                            TrStr t2 = _tr_str_lit_len("", 0LL);
#line 696 "src/racecheck.tr"
                            if ((!_rc_nul(c2))) {
#line 697 "src/racecheck.tr"
                                __auto_type _t3467 = (*c2);
                                if (_t3467.tag == Expr_EIdent) {
                                    __auto_type n2 = _t3467.data.EIdent.name;
                                    TrStr _strtmp_t3468 = _tr_str_retain(n2);
                                    _tr_str_release(t2);
                                    t2 = _strtmp_t3468;
                                } else if (1) {
                                    __auto_type _ = _t3467;
                                    /* pass */
                                }
                            }
#line 700 "src/racecheck.tr"
                            RcCk_record_site(self, t2, a2, 0LL);
                            _tr_str_release(t2);
                        } else if (1) {
                            __auto_type _ = _t3466;
                            /* pass */
                        }
#line 702 "src/racecheck.tr"
                        ai = (ai + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t3464;
                RcCk_expr(self, c);
            }
        }
#line 704 "src/racecheck.tr"
        RcCk_exprs(self, args);
    } else if (_t3463.tag == Expr_EMethodCall) {
        __auto_type o = _t3463.data.EMethodCall.obj;
__auto_type m = _t3463.data.EMethodCall.method;
__auto_type args = _t3463.data.EMethodCall.args;
#line 706 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 707 "src/racecheck.tr"
        RcCk_exprs(self, args);
#line 708 "src/racecheck.tr"
        TrStr _tr_v_recv = _rc_root(o);
#line 709 "src/racecheck.tr"
        TrStr direct = _tr_str_lit_len("", 0LL);
#line 710 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 711 "src/racecheck.tr"
            __auto_type _t3469 = (*o);
            if (_t3469.tag == Expr_EIdent) {
                __auto_type dn = _t3469.data.EIdent.name;
                TrStr _strtmp_t3470 = _tr_str_retain(dn);
                _tr_str_release(direct);
                direct = _strtmp_t3470;
            } else if (1) {
                __auto_type _ = _t3469;
                /* pass */
            }
        }
#line 714 "src/racecheck.tr"
        if ((!_tr_str_eqv((direct), (_tr_str_lit_len("", 0LL))))) {
#line 715 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcrecv, direct);
#line 716 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcname, m);
        }
#line 717 "src/racecheck.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("wait", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("await", 5LL))))) {
#line 717 "src/racecheck.tr"
            self->seen_spawn = false;
        }
#line 718 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && (args->len > 0LL))) {
#line 719 "src/racecheck.tr"
            TrStr t = _tr_str_lit_len("", 0LL);
#line 720 "src/racecheck.tr"
            __auto_type _t3471 = (*((Expr*)List_ptr_get(args, 0LL)));
            if (_t3471.tag == Expr_EIdent) {
                __auto_type tn2 = _t3471.data.EIdent.name;
                TrStr _strtmp_t3472 = _tr_str_retain(tn2);
                _tr_str_release(t);
                t = _strtmp_t3472;
            } else if (_t3471.tag == Expr_EClosure) {
                __auto_type cps = _t3471.data.EClosure.params;
__auto_type cbody = _t3471.data.EClosure.body;
#line 724 "src/racecheck.tr"
                RcFn* tmp = ({ TrStr _apt_t3473 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t3473, self->cur->line, true)); _tr_str_release(_apt_t3473); _wr; });
#line 725 "src/racecheck.tr"
                long long ci = 0LL;
#line 726 "src/racecheck.tr"
                while ((ci < cps->len)) {
#line 727 "src/racecheck.tr"
                    ({ __auto_type _ord_t3474 = (((Param*)List_ptr_get(cps, ci))->name); List_TrStr_append(tmp->params, _ord_t3474); });
#line 728 "src/racecheck.tr"
                    ci = (ci + 1LL);
                }
#line 729 "src/racecheck.tr"
                RcFn* saved = _tr_obj_retain(self->cur);
#line 730 "src/racecheck.tr"
                RcFn* _cltmp_t3475 = _tr_obj_retain(tmp);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3475;
#line 731 "src/racecheck.tr"
                RcCk_block(self, cbody);
#line 732 "src/racecheck.tr"
                RcFn* _cltmp_t3476 = _tr_obj_retain(saved);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3476;
#line 733 "src/racecheck.tr"
                long long ck = 0LL;
#line 734 "src/racecheck.tr"
                while ((ck < tmp->touched->len)) {
#line 735 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3478 = (self->cur->locals); TrStr _at_t3477 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3478, _at_t3477)); _tr_str_release(_at_t3477); _wr; }) || ({ __auto_type _ord_t3480 = (self->cur->params); TrStr _at_t3479 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3480, _at_t3479)); _tr_str_release(_at_t3479); _wr; }))) {
#line 736 "src/racecheck.tr"
                        ({ __auto_type _ord_t3481 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] the closure passed to `spawn` captures '", 46LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3482 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3481); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the spawning function by reference: the new thread would race on it and outlive it.\n      FIX: spawn a named `def` and pass '", 132LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3483 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3482); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as an argument (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3483); _tr_str_release(_ord_t3483); });
#line 737 "src/racecheck.tr"
                        break;
                    }
#line 738 "src/racecheck.tr"
                    ck = (ck + 1LL);
                }
                _tr_obj_release(tmp, _trdrop_RcFn);
                _tr_obj_release(saved, _trdrop_RcFn);
            } else if (1) {
                __auto_type _ = _t3471;
                /* pass */
            }
#line 740 "src/racecheck.tr"
            if ((!_tr_str_eqv((t), (_tr_str_lit_len("", 0LL))))) {
#line 740 "src/racecheck.tr"
                RcCk_record_site(self, t, args, 1LL);
            }
        }
#line 741 "src/racecheck.tr"
        if (((!_tr_str_eqv((_tr_v_recv), (_tr_str_lit_len("", 0LL)))) && _rc_mut_method(m))) {
#line 742 "src/racecheck.tr"
            List_TrStr_append(self->cur->wroot, _tr_v_recv);
#line 743 "src/racecheck.tr"
            ({ TrStr _ord_t3484 = (_rc_first_prop(o)); List_TrStr_append(self->cur->wprop, _ord_t3484); _tr_str_release(_ord_t3484); });
#line 744 "src/racecheck.tr"
            List_bool_append(self->cur->wafter, self->seen_spawn);
#line 745 "src/racecheck.tr"
            RcCk_note(self, _tr_v_recv, true);
        }
        _tr_str_release(_tr_v_recv);
        _tr_str_release(direct);
    } else if (_t3463.tag == Expr_EPropAccess) {
        __auto_type o = _t3463.data.EPropAccess.obj;
        RcCk_expr(self, o);
    } else if (_t3463.tag == Expr_EIndex) {
        __auto_type o = _t3463.data.EIndex.obj;
__auto_type i = _t3463.data.EIndex._tr_v_index;
#line 748 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 749 "src/racecheck.tr"
        RcCk_expr(self, i);
    } else if (_t3463.tag == Expr_ECast) {
        __auto_type x = _t3463.data.ECast.expr;
        RcCk_expr(self, x);
    } else if (_t3463.tag == Expr_EFString) {
        __auto_type parts = _t3463.data.EFString.parts;
#line 752 "src/racecheck.tr"
        long long pi = 0LL;
#line 753 "src/racecheck.tr"
        while ((pi < parts->len)) {
#line 754 "src/racecheck.tr"
            if (((FStringPart*)List_ptr_get(parts, pi))->is_expr) {
#line 754 "src/racecheck.tr"
                RcCk_expr(self, ((FStringPart*)List_ptr_get(parts, pi))->expr);
            }
#line 755 "src/racecheck.tr"
            pi = (pi + 1LL);
        }
    } else if (_t3463.tag == Expr_ETryExpr) {
        __auto_type x = _t3463.data.ETryExpr.expr;
        RcCk_expr(self, x);
    } else if (_t3463.tag == Expr_EClosure) {
        __auto_type params = _t3463.data.EClosure.params;
__auto_type body = _t3463.data.EClosure.body;
#line 758 "src/racecheck.tr"
        long long ci = 0LL;
#line 759 "src/racecheck.tr"
        while ((ci < params->len)) {
#line 760 "src/racecheck.tr"
            ({ TrStr _apt_t3485 = _tr_str_retain(((Param*)List_ptr_get(params, ci))->name); RcCk_declare(self, _apt_t3485); _tr_str_release(_apt_t3485); });
#line 761 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 762 "src/racecheck.tr"
        RcCk_block(self, body);
    } else if (_t3463.tag == Expr_ESuperMethodCall) {
        __auto_type args = _t3463.data.ESuperMethodCall.args;
        RcCk_exprs(self, args);
    } else if (_t3463.tag == Expr_EList) {
        __auto_type items = _t3463.data.EList.items;
        RcCk_exprs(self, items);
    } else if (_t3463.tag == Expr_ESet) {
        __auto_type items = _t3463.data.ESet.items;
        RcCk_exprs(self, items);
    } else if (_t3463.tag == Expr_ETuple) {
        __auto_type items = _t3463.data.ETuple.items;
        RcCk_exprs(self, items);
    } else if (_t3463.tag == Expr_EDict) {
        __auto_type ks = _t3463.data.EDict.keys;
__auto_type vs = _t3463.data.EDict.vals;
#line 768 "src/racecheck.tr"
        RcCk_exprs(self, ks);
#line 769 "src/racecheck.tr"
        RcCk_exprs(self, vs);
    } else if (_t3463.tag == Expr_EListComp) {
        __auto_type el = _t3463.data.EListComp.element;
__auto_type gens = _t3463.data.EListComp.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3463.tag == Expr_EGeneratorExpr) {
        __auto_type el = _t3463.data.EGeneratorExpr.element;
__auto_type gens = _t3463.data.EGeneratorExpr.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3463.tag == Expr_ESlice) {
        __auto_type a = _t3463.data.ESlice.start;
__auto_type b = _t3463.data.ESlice.stop;
__auto_type c = _t3463.data.ESlice.step;
#line 773 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 774 "src/racecheck.tr"
        RcCk_expr(self, b);
#line 775 "src/racecheck.tr"
        RcCk_expr(self, c);
    } else if (_t3463.tag == Expr_EAwait) {
        __auto_type x = _t3463.data.EAwait.expr;
        RcCk_expr(self, x);
    } else if (_t3463.tag == Expr_EYield) {
        __auto_type x = _t3463.data.EYield.expr;
        RcCk_expr(self, x);
    } else if (_t3463.tag == Expr_ETry) {
        __auto_type tb = _t3463.data.ETry.try_body;
__auto_type catches = _t3463.data.ETry.catches;
__auto_type fb = _t3463.data.ETry.finally_b;
#line 779 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 780 "src/racecheck.tr"
        long long ci2 = 0LL;
#line 781 "src/racecheck.tr"
        while ((ci2 < catches->len)) {
#line 782 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci2)))->body);
#line 783 "src/racecheck.tr"
            ci2 = (ci2 + 1LL);
        }
#line 784 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t3463.tag == Expr_ERange) {
        __auto_type a = _t3463.data.ERange.start;
__auto_type b = _t3463.data.ERange.end;
#line 786 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 787 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3463.tag == Expr_EIfElse) {
        __auto_type c = _t3463.data.EIfElse.cond;
__auto_type a = _t3463.data.EIfElse.then_expr;
__auto_type b = _t3463.data.EIfElse.else_expr;
#line 789 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 790 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 791 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3463.tag == Expr_EDo) {
        __auto_type b = _t3463.data.EDo.body;
        RcCk_block(self, b);
    } else if (_t3463.tag == Expr_EMatch) {
        __auto_type s = _t3463.data.EMatch.subj;
__auto_type arms = _t3463.data.EMatch.arms;
#line 794 "src/racecheck.tr"
        RcCk_expr(self, s);
#line 795 "src/racecheck.tr"
        long long mi = 0LL;
#line 796 "src/racecheck.tr"
        while ((mi < arms->len)) {
#line 797 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, mi))->guard);
#line 798 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, mi))->body);
#line 799 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t3463.tag == Expr_EMacroCall) {
        __auto_type args = _t3463.data.EMacroCall.args;
        RcCk_exprs(self, args);
    } else if (_t3463.tag == Expr_ELoop) {
        __auto_type b = _t3463.data.ELoop.body;
        RcCk_block(self, b);
    } else if (_t3463.tag == Expr_EWhileExpr) {
        __auto_type c = _t3463.data.EWhileExpr.cond;
__auto_type b = _t3463.data.EWhileExpr.body;
__auto_type eb = _t3463.data.EWhileExpr.else_body;
#line 803 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 804 "src/racecheck.tr"
        RcCk_block(self, b);
#line 805 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (1) {
        __auto_type _ = _t3463;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_add_method(RcCk* self, FunctionDef* f, TrStr cls, TrStr file, bool lib) {
#line 809 "src/racecheck.tr"
    ({ TrStr _at_t3486 = (({ TrStr _tr_cl = (_tr_strx_concatv((cls), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_add_fn_named(self, f, _at_t3486, file, lib); _tr_str_release(_at_t3486); });
}

__attribute__((hot)) void RcCk_add_fn(RcCk* self, FunctionDef* f, TrStr file, bool lib) {
#line 812 "src/racecheck.tr"
    ({ TrStr _apt_t3487 = _tr_str_retain(f->name); RcCk_add_fn_named(self, f, _apt_t3487, file, lib); _tr_str_release(_apt_t3487); });
}

__attribute__((hot)) void RcCk_add_fn_named(RcCk* self, FunctionDef* f, TrStr nm, TrStr file, bool lib) {
#line 815 "src/racecheck.tr"
    RcFn* rf = _rc_new_fn(nm, file, f->line, (lib || f->is_lib));
#line 816 "src/racecheck.tr"
    long long pi = 0LL;
#line 817 "src/racecheck.tr"
    while ((pi < f->params->len)) {
#line 818 "src/racecheck.tr"
        ({ __auto_type _ord_t3488 = (((Param*)List_ptr_get(f->params, pi))->name); List_TrStr_append(rf->params, _ord_t3488); });
#line 819 "src/racecheck.tr"
        if ((!rf->is_lib)) {
#line 819 "src/racecheck.tr"
            ({ __auto_type _ord_t3489 = (((Param*)List_ptr_get(f->params, pi))->ty); __auto_type _ord_t3490 = (f->line); RcCk_ty_check(self, _ord_t3489, file, _ord_t3490); });
        }
#line 820 "src/racecheck.tr"
        AstType** pt = ((Param*)List_ptr_get(f->params, pi))->ty;
#line 821 "src/racecheck.tr"
        if ((((unsigned long long)(pt)) != ((unsigned long long)(0LL)))) {
#line 822 "src/racecheck.tr"
            ({ __auto_type _ord_t3491 = ((*pt)->name); List_TrStr_append(rf->ptypes, _ord_t3491); });
#line 823 "src/racecheck.tr"
            if (((*pt)->args->len > 0LL)) {
#line 823 "src/racecheck.tr"
                ({ __auto_type _ord_t3492 = ((*((AstType**)List_ptr_get((*pt)->args, 0LL)))->name); List_TrStr_append(rf->pinner, _ord_t3492); });
            } else {
#line 824 "src/racecheck.tr"
                List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
            }
        } else {
#line 826 "src/racecheck.tr"
            List_TrStr_append(rf->ptypes, _tr_str_lit_len("", 0LL));
#line 827 "src/racecheck.tr"
            List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
        }
#line 828 "src/racecheck.tr"
        pi = (pi + 1LL);
    }
#line 829 "src/racecheck.tr"
    List_ptr_append(self->fns, _tr_obj_retain(rf));
#line 830 "src/racecheck.tr"
    if (rf->is_lib) {
#line 830 "src/racecheck.tr"
        _tr_obj_release(rf, _trdrop_RcFn);
        return;
    }
#line 831 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 832 "src/racecheck.tr"
    long long saved_line = self->cur_line;
#line 833 "src/racecheck.tr"
    long long saved_ld = self->loop_depth;
#line 834 "src/racecheck.tr"
    bool saved_sp = self->seen_spawn;
#line 835 "src/racecheck.tr"
    RcFn* _cltmp_t3493 = _tr_obj_retain(rf);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3493;
#line 836 "src/racecheck.tr"
    self->cur_line = f->line;
#line 837 "src/racecheck.tr"
    self->loop_depth = 0LL;
#line 838 "src/racecheck.tr"
    self->seen_spawn = false;
#line 839 "src/racecheck.tr"
    RcCk_block(self, f->body);
#line 840 "src/racecheck.tr"
    long long tw = 0LL;
#line 841 "src/racecheck.tr"
    while ((tw < rf->thr_made->len)) {
#line 842 "src/racecheck.tr"
        if (({ __auto_type _ord_t3495 = (rf->idents); TrStr _at_t3494 = (List_TrStr_get(rf->thr_made, tw)); __auto_type _wr = ((!_rc_has(_ord_t3495, _at_t3494))); _tr_str_release(_at_t3494); _wr; })) {
#line 843 "src/racecheck.tr"
            ({ __auto_type _ord_t3496 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("W|", 2LL)), (file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(List_i64_get(rf->thr_line, tw))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] the Thread handle '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3497 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3496); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is never joined, detached or used: the thread runs unsupervised and the program may exit while it is still working.\n      FIX: call `", 135LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3498 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3497); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".join()` (wait for it) or `", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3499 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3498); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".detach()` (run independently).", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(rf->msgs, _ord_t3499); _tr_str_release(_ord_t3499); });
        }
#line 844 "src/racecheck.tr"
        tw = (tw + 1LL);
    }
#line 845 "src/racecheck.tr"
    RcFn* _cltmp_t3500 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3500;
#line 846 "src/racecheck.tr"
    self->cur_line = saved_line;
#line 847 "src/racecheck.tr"
    self->loop_depth = saved_ld;
#line 848 "src/racecheck.tr"
    self->seen_spawn = saved_sp;
    _tr_obj_release(rf, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
}

__attribute__((hot)) void RcCk_add_global(RcCk* self, TrStr name, bool is_mut, bool is_const, AstType** ty, Expr* v, TrStr df) {
#line 851 "src/racecheck.tr"
    List_TrStr_append(self->gnames, name);
#line 852 "src/racecheck.tr"
    List_TrStr_append(self->gfiles, df);
#line 853 "src/racecheck.tr"
    TrStr tn = _tr_str_lit_len("", 0LL);
#line 854 "src/racecheck.tr"
    if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 854 "src/racecheck.tr"
        TrStr _strtmp_t3501 = _tr_str_retain((*ty)->name);
        _tr_str_release(tn);
        tn = _strtmp_t3501;
    }
#line 855 "src/racecheck.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 855 "src/racecheck.tr"
        TrStr _strtmp_t3502 = _rc_ctor_name(v);
        _tr_str_release(tn);
        tn = _strtmp_t3502;
    }
#line 856 "src/racecheck.tr"
    bool sync = _rc_sync_ty(tn);
#line 857 "src/racecheck.tr"
    if (((!sync) && (!_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))))) {
#line 858 "src/racecheck.tr"
        long long ci0 = RcCk_find_class(self, tn);
#line 859 "src/racecheck.tr"
        if (((ci0 >= 0LL) && List_bool_get(self->class_lib, ci0))) {
#line 859 "src/racecheck.tr"
            sync = true;
        }
    }
#line 860 "src/racecheck.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 861 "src/racecheck.tr"
        __auto_type _t3503 = (*v);
        if (_t3503.tag == Expr_ELitInt) {
            __auto_type _ = _t3503.data.ELitInt.val;
            /* pass */
        } else if (_t3503.tag == Expr_ELitFloat) {
            __auto_type _ = _t3503.data.ELitFloat.val;
            /* pass */
        } else if (_t3503.tag == Expr_ELitStr) {
            /* pass */
        } else if (_t3503.tag == Expr_ELitBool) {
            __auto_type _ = _t3503.data.ELitBool.val;
            /* pass */
        } else if (_t3503.tag == Expr_EList) {
            __auto_type _ = _t3503.data.EList.items;
            /* pass */
        } else if (_t3503.tag == Expr_EDict) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t3503;
            sync = true;
        }
    }
#line 869 "src/racecheck.tr"
    if ((!(is_mut && (!is_const)))) {
#line 871 "src/racecheck.tr"
        bool content_mut = (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || (RcCk_find_class(self, tn) >= 0LL));
#line 872 "src/racecheck.tr"
        if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 873 "src/racecheck.tr"
            __auto_type _t3504 = (*v);
            if (_t3504.tag == Expr_EList) {
                __auto_type _ = _t3504.data.EList.items;
                content_mut = true;
            } else if (_t3504.tag == Expr_EDict) {
                content_mut = true;
            } else if (_t3504.tag == Expr_ESet) {
                __auto_type _ = _t3504.data.ESet.items;
                content_mut = true;
            } else if (1) {
                __auto_type _ = _t3504;
                /* pass */
            }
        }
#line 878 "src/racecheck.tr"
        if ((!content_mut)) {
#line 878 "src/racecheck.tr"
            sync = true;
        }
    }
#line 879 "src/racecheck.tr"
    List_bool_append(self->gsync, sync);
#line 880 "src/racecheck.tr"
    List_TrStr_append(self->gtn, tn);
    _tr_str_release(tn);
}

__attribute__((hot)) long long RcCk_find_fn(RcCk* self, TrStr name) {
#line 883 "src/racecheck.tr"
    long long i = 0LL;
#line 884 "src/racecheck.tr"
    while ((i < self->fns->len)) {
#line 885 "src/racecheck.tr"
        if ((_tr_str_eqv((((RcFn*)List_ptr_get(self->fns, i))->name), (name)) && (!((RcFn*)List_ptr_get(self->fns, i))->is_lib))) {
#line 885 "src/racecheck.tr"
            return i;
        }
#line 886 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 887 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) long long RcCk_find_class(RcCk* self, TrStr name) {
#line 890 "src/racecheck.tr"
    long long i = 0LL;
#line 891 "src/racecheck.tr"
    while ((i < self->classes->len)) {
#line 892 "src/racecheck.tr"
        if (_tr_str_eqv((((ClassDef*)List_ptr_get(self->classes, i))->name), (name))) {
#line 892 "src/racecheck.tr"
            return i;
        }
#line 893 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 894 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool RcCk_field_plain(RcCk* self, TrStr cn, TrStr prop) {
#line 898 "src/racecheck.tr"
    long long ci = RcCk_find_class(self, cn);
#line 899 "src/racecheck.tr"
    if ((ci < 0LL)) {
#line 899 "src/racecheck.tr"
        return false;
    }
#line 900 "src/racecheck.tr"
    ClassDef* cls = ((ClassDef*)List_ptr_get(self->classes, ci));
#line 901 "src/racecheck.tr"
    long long i = 0LL;
#line 902 "src/racecheck.tr"
    while ((i < cls->fields->len)) {
#line 903 "src/racecheck.tr"
        if (_tr_str_eqv((((FieldDef*)List_ptr_get(cls->fields, i))->name), (prop))) {
#line 904 "src/racecheck.tr"
            AstType** ft = ((FieldDef*)List_ptr_get(cls->fields, i))->ty;
#line 905 "src/racecheck.tr"
            if ((((unsigned long long)(ft)) == ((unsigned long long)(0LL)))) {
#line 905 "src/racecheck.tr"
                return false;
            }
#line 906 "src/racecheck.tr"
            TrStr tn = _tr_str_retain((*ft)->name);
#line 907 "src/racecheck.tr"
            if (_rc_sync_ty(tn)) {
#line 907 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 908 "src/racecheck.tr"
            long long ni = RcCk_find_class(self, tn);
#line 909 "src/racecheck.tr"
            if (((ni >= 0LL) && List_bool_get(self->class_lib, ni))) {
#line 909 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 910 "src/racecheck.tr"
            _tr_str_release(tn);
            return true;
        }
#line 911 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 912 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_i64* RcCk_reach(RcCk* self, long long ti) {
#line 916 "src/racecheck.tr"
    List_i64* seen = (void*)List_i64_new();
#line 917 "src/racecheck.tr"
    List_i64_append(seen, ti);
#line 918 "src/racecheck.tr"
    long long k = 0LL;
#line 919 "src/racecheck.tr"
    while ((k < seen->len)) {
#line 920 "src/racecheck.tr"
        RcFn* f = ({ __auto_type _ord_t3505 = (List_i64_get(seen, k)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3505))); _wr; });
#line 921 "src/racecheck.tr"
        long long ci = 0LL;
#line 922 "src/racecheck.tr"
        while ((ci < f->calls->len)) {
#line 923 "src/racecheck.tr"
            long long ni = ({ TrStr _at_t3506 = (List_TrStr_get(f->calls, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3506)); _tr_str_release(_at_t3506); _wr; });
#line 924 "src/racecheck.tr"
            if ((ni >= 0LL)) {
#line 925 "src/racecheck.tr"
                bool dup = false;
#line 926 "src/racecheck.tr"
                long long q = 0LL;
#line 927 "src/racecheck.tr"
                while ((q < seen->len)) {
#line 928 "src/racecheck.tr"
                    if ((List_i64_get(seen, q) == ni)) {
#line 928 "src/racecheck.tr"
                        dup = true;
                    }
#line 929 "src/racecheck.tr"
                    q = (q + 1LL);
                }
#line 930 "src/racecheck.tr"
                if ((!dup)) {
#line 930 "src/racecheck.tr"
                    List_i64_append(seen, ni);
                }
            }
#line 931 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 932 "src/racecheck.tr"
        long long mci = 0LL;
#line 933 "src/racecheck.tr"
        while ((mci < f->mcrecv->len)) {
#line 934 "src/racecheck.tr"
            TrStr rcv = List_TrStr_get(f->mcrecv, mci);
#line 935 "src/racecheck.tr"
            TrStr rcn = _tr_str_lit_len("", 0LL);
#line 936 "src/racecheck.tr"
            if (_tr_str_eqv((rcv), (_tr_str_lit_len("self", 4LL)))) {
#line 937 "src/racecheck.tr"
                long long dot = ({ TrStr _apt_t3507 = _tr_str_retain(f->name); __auto_type _wr = (_rc_dot(_apt_t3507)); _tr_str_release(_apt_t3507); _wr; });
#line 938 "src/racecheck.tr"
                if ((dot > 0LL)) {
#line 938 "src/racecheck.tr"
                    TrStr _strtmp_t3508 = _tr_str_slicev((f->name), 0LL, dot);
                    _tr_str_release(rcn);
                    rcn = _strtmp_t3508;
                }
            } else {
#line 940 "src/racecheck.tr"
                TrStr _strtmp_t3509 = RcCk_lclass(self, f, rcv);
                _tr_str_release(rcn);
                rcn = _strtmp_t3509;
#line 941 "src/racecheck.tr"
                if (_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL)))) {
#line 942 "src/racecheck.tr"
                    long long gj = 0LL;
#line 943 "src/racecheck.tr"
                    while ((gj < self->gnames->len)) {
#line 944 "src/racecheck.tr"
                        if (({ TrStr _cmpa_t3510 = (List_TrStr_get(self->gnames, gj)); TrStr _cmpb_t3511 = (rcv); _Bool _cmpr_t3512 = _tr_str_eqv(_cmpa_t3510, _cmpb_t3511); _tr_str_release(_cmpa_t3510); _cmpr_t3512; })) {
#line 944 "src/racecheck.tr"
                            TrStr _strtmp_t3513 = List_TrStr_get(self->gtn, gj);
                            _tr_str_release(rcn);
                            rcn = _strtmp_t3513;
                        }
#line 945 "src/racecheck.tr"
                        gj = (gj + 1LL);
                    }
                }
            }
#line 946 "src/racecheck.tr"
            if ((!_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL))))) {
#line 947 "src/racecheck.tr"
                long long mni = ({ TrStr _at_t3514 = (({ TrStr _tr_cl = (_tr_strx_concatv((rcn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(f->mcname, mci)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3514)); _tr_str_release(_at_t3514); _wr; });
#line 948 "src/racecheck.tr"
                if ((mni >= 0LL)) {
#line 949 "src/racecheck.tr"
                    bool dup2 = false;
#line 950 "src/racecheck.tr"
                    long long q2 = 0LL;
#line 951 "src/racecheck.tr"
                    while ((q2 < seen->len)) {
#line 952 "src/racecheck.tr"
                        if ((List_i64_get(seen, q2) == mni)) {
#line 952 "src/racecheck.tr"
                            dup2 = true;
                        }
#line 953 "src/racecheck.tr"
                        q2 = (q2 + 1LL);
                    }
#line 954 "src/racecheck.tr"
                    if ((!dup2)) {
#line 954 "src/racecheck.tr"
                        List_i64_append(seen, mni);
                    }
                }
            }
#line 955 "src/racecheck.tr"
            mci = (mci + 1LL);
            _tr_str_release(rcv);
            _tr_str_release(rcn);
        }
#line 956 "src/racecheck.tr"
        k = (k + 1LL);
    }
#line 957 "src/racecheck.tr"
    return seen;
}

__attribute__((hot)) void RcCk_analyze_site(RcCk* self, RcFn* f, RcSite* s) {
#line 961 "src/racecheck.tr"
    if (((!_tr_str_eqv((s->target), (_tr_str_lit_len("", 0LL)))) && ({ __auto_type _ord_t3516 = (f->closures); TrStr _apt_t3515 = _tr_str_retain(s->target); __auto_type _wr = (_rc_has(_ord_t3516, _apt_t3515)); _tr_str_release(_apt_t3515); _wr; }))) {
#line 962 "src/racecheck.tr"
        ({ __auto_type _ord_t3517 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3518 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3517); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a closure value: it captures its surrounding locals by reference into this stack frame, so a spawned thread would race on them and outlive them.\n      FIX: spawn a named `def` and pass the data it needs as arguments (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 289LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3518); _tr_str_release(_ord_t3518); });
#line 963 "src/racecheck.tr"
        return;
    }
#line 964 "src/racecheck.tr"
    if ((!_tr_str_eqv((s->recv), (_tr_str_lit_len("", 0LL))))) {
#line 965 "src/racecheck.tr"
        RcCk_analyze_method_site(self, f, s);
#line 966 "src/racecheck.tr"
        return;
    }
#line 967 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3519 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3519)); _tr_str_release(_apt_t3519); _wr; });
#line 968 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 968 "src/racecheck.tr"
        return;
    }
#line 969 "src/racecheck.tr"
    ({ TrStr _apt_t3520 = _tr_str_retain(s->target); RcCk_check_globals(self, f, s, ti, _apt_t3520); _tr_str_release(_apt_t3520); });
#line 971 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, ti));
#line 972 "src/racecheck.tr"
    long long ai = 0LL;
#line 973 "src/racecheck.tr"
    while (((ai < s->args->len) && (ai < tf->params->len))) {
#line 974 "src/racecheck.tr"
        TrStr an = _tr_str_lit_len("", 0LL);
#line 975 "src/racecheck.tr"
        __auto_type _t3521 = (*((Expr*)List_ptr_get(s->args, ai)));
        if (_t3521.tag == Expr_EIdent) {
            __auto_type nn = _t3521.data.EIdent.name;
            TrStr _strtmp_t3522 = _tr_str_retain(nn);
            _tr_str_release(an);
            an = _strtmp_t3522;
        } else if (1) {
            __auto_type _ = _t3521;
            /* pass */
        }
#line 978 "src/racecheck.tr"
        TrStr ptn = List_TrStr_get(tf->ptypes, ai);
#line 979 "src/racecheck.tr"
        if ((!_tr_str_eqv((an), (_tr_str_lit_len("", 0LL))))) {
#line 980 "src/racecheck.tr"
            long long cnt = 0LL;
#line 981 "src/racecheck.tr"
            long long si = 0LL;
#line 982 "src/racecheck.tr"
            while ((si < f->sites->len)) {
#line 983 "src/racecheck.tr"
                long long oa = 0LL;
#line 984 "src/racecheck.tr"
                while ((oa < ((RcSite*)List_ptr_get(f->sites, si))->args->len)) {
#line 985 "src/racecheck.tr"
                    __auto_type _t3523 = (*((Expr*)List_ptr_get(((RcSite*)List_ptr_get(f->sites, si))->args, oa)));
                    if (_t3523.tag == Expr_EIdent) {
                        __auto_type on = _t3523.data.EIdent.name;
#line 987 "src/racecheck.tr"
                        if (_tr_str_eqv((on), (an))) {
#line 987 "src/racecheck.tr"
                            cnt = (cnt + 1LL);
                        }
                    } else if (1) {
                        __auto_type _ = _t3523;
                        /* pass */
                    }
#line 989 "src/racecheck.tr"
                    oa = (oa + 1LL);
                }
#line 990 "src/racecheck.tr"
                si = (si + 1LL);
            }
#line 991 "src/racecheck.tr"
            bool is_sh = (_rc_has(f->shvars, an) || ({ TrStr _cmpa_t3524 = (RcCk_ltype(self, f, an)); TrStr _cmpb_t3525 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3526 = _tr_str_eqv(_cmpa_t3524, _cmpb_t3525); _tr_str_release(_cmpa_t3524); _cmpr_t3526; }));
#line 992 "src/racecheck.tr"
            if (((is_sh && (!_tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL))))) && (!_tr_str_eqv((ptn), (_tr_str_lit_len("", 0LL)))))) {
#line 993 "src/racecheck.tr"
                ({ __auto_type _ord_t3527 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-10] '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a `Shared` handle but '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3528 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3527); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares parameter '", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3529 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3528); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': the thread would be handed the shared control block where it expects the object itself (a type confusion).\n      FIX: declare the worker's parameter as `", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3530 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3529); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Shared[", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3530); _tr_str_release(_ord_t3530); });
#line 994 "src/racecheck.tr"
                ai = (ai + 1LL);
#line 995 "src/racecheck.tr"
                _tr_str_release(an);
                _tr_str_release(ptn);
                continue;
            }
#line 996 "src/racecheck.tr"
            long long dd = RcCk_ldepth_of(self, f, an);
#line 997 "src/racecheck.tr"
            bool multi = ((((cnt >= 2LL) || is_sh) || _tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL)))) || ((dd >= 0LL) && (s->depth > dd)));
#line 998 "src/racecheck.tr"
            if (multi) {
#line 999 "src/racecheck.tr"
                RcCk_check_shared_writes(self, f, s, tf, ai, an);
#line 1000 "src/racecheck.tr"
                if ((is_sh || _tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL))))) {
#line 1000 "src/racecheck.tr"
                    RcCk_check_parent_writes(self, f, s, an);
                }
            }
        } else if (_tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL)))) {
#line 1003 "src/racecheck.tr"
            RcCk_check_shared_writes(self, f, s, tf, ai, _tr_str_lit_len("a shared handle", 15LL));
        }
#line 1004 "src/racecheck.tr"
        ai = (ai + 1LL);
        _tr_str_release(an);
        _tr_str_release(ptn);
    }
}

__attribute__((hot)) void RcCk_analyze_method_site(RcCk* self, RcFn* f, RcSite* s) {
#line 1008 "src/racecheck.tr"
    TrStr cn = ({ TrStr _apt_t3531 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_lclass(self, f, _apt_t3531)); _tr_str_release(_apt_t3531); _wr; });
#line 1009 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1009 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1010 "src/racecheck.tr"
    long long mi = ({ TrStr _at_t3532 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3532)); _tr_str_release(_at_t3532); _wr; });
#line 1011 "src/racecheck.tr"
    if ((mi < 0LL)) {
#line 1011 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1012 "src/racecheck.tr"
    ({ TrStr _at_t3533 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_check_globals(self, f, s, mi, _at_t3533); _tr_str_release(_at_t3533); });
#line 1013 "src/racecheck.tr"
    long long cnt = 0LL;
#line 1014 "src/racecheck.tr"
    long long si = 0LL;
#line 1015 "src/racecheck.tr"
    while ((si < f->sites->len)) {
#line 1016 "src/racecheck.tr"
        if (({ __auto_type _ord_t3534 = (((RcSite*)List_ptr_get(f->sites, si))->recv); __auto_type _wr = (_tr_str_eqv((_ord_t3534), (s->recv))); _wr; })) {
#line 1016 "src/racecheck.tr"
            cnt = (cnt + 1LL);
        }
#line 1017 "src/racecheck.tr"
        si = (si + 1LL);
    }
#line 1018 "src/racecheck.tr"
    bool is_sh = (({ __auto_type _ord_t3536 = (f->shvars); TrStr _apt_t3535 = _tr_str_retain(s->recv); __auto_type _wr = (_rc_has(_ord_t3536, _apt_t3535)); _tr_str_release(_apt_t3535); _wr; }) || ({ TrStr _apt_t3537 = _tr_str_retain(s->recv); __auto_type _wr = (({ TrStr _cmpa_t3538 = (RcCk_ltype(self, f, _apt_t3537)); TrStr _cmpb_t3539 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3540 = _tr_str_eqv(_cmpa_t3538, _cmpb_t3539); _tr_str_release(_cmpa_t3538); _cmpr_t3540; })); _tr_str_release(_apt_t3537); _wr; }));
#line 1019 "src/racecheck.tr"
    long long dd = ({ TrStr _apt_t3541 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_ldepth_of(self, f, _apt_t3541)); _tr_str_release(_apt_t3541); _wr; });
#line 1020 "src/racecheck.tr"
    if ((!(((cnt >= 2LL) || is_sh) || ((dd >= 0LL) && (s->depth > dd))))) {
#line 1020 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1021 "src/racecheck.tr"
    RcFn* mf = ((RcFn*)List_ptr_get(self->fns, mi));
#line 1022 "src/racecheck.tr"
    long long k = 0LL;
#line 1023 "src/racecheck.tr"
    while ((k < mf->wroot->len)) {
#line 1024 "src/racecheck.tr"
        if (((({ TrStr _cmpa_t3542 = (List_TrStr_get(mf->wroot, k)); TrStr _cmpb_t3543 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3544 = _tr_str_eqv(_cmpa_t3542, _cmpb_t3543); _tr_str_release(_cmpa_t3542); _cmpr_t3544; }) && ({ TrStr _cmpa_t3545 = (List_TrStr_get(mf->wprop, k)); TrStr _cmpb_t3546 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3547 = !_tr_str_eqv(_cmpa_t3545, _cmpb_t3546); _tr_str_release(_cmpa_t3545); _cmpr_t3547; })) && ({ TrStr _at_t3548 = (List_TrStr_get(mf->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3548)); _tr_str_release(_at_t3548); _wr; }))) {
#line 1025 "src/racecheck.tr"
            ({ TrStr _ord_t3549 = (List_TrStr_get(mf->wprop, k)); __auto_type _ord_t3550 = (s->recv); RcCk_shared_msg(self, f, s, mf, cn, _ord_t3549, _ord_t3550); _tr_str_release(_ord_t3549); });
#line 1026 "src/racecheck.tr"
            _tr_str_release(cn);
            return;
        }
#line 1027 "src/racecheck.tr"
        k = (k + 1LL);
    }
#line 1028 "src/racecheck.tr"
    if (is_sh) {
#line 1028 "src/racecheck.tr"
        ({ TrStr _apt_t3551 = _tr_str_retain(s->recv); RcCk_check_parent_writes(self, f, s, _apt_t3551); _tr_str_release(_apt_t3551); });
    }
    _tr_str_release(cn);
}

__attribute__((hot)) void RcCk_check_parent_writes(RcCk* self, RcFn* f, RcSite* s, TrStr an) {
#line 1032 "src/racecheck.tr"
    TrStr cn = RcCk_lclass(self, f, an);
#line 1033 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1033 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1034 "src/racecheck.tr"
    long long k = 0LL;
#line 1035 "src/racecheck.tr"
    while ((k < f->wroot->len)) {
#line 1036 "src/racecheck.tr"
        if ((((({ TrStr _cmpa_t3552 = (List_TrStr_get(f->wroot, k)); TrStr _cmpb_t3553 = (an); _Bool _cmpr_t3554 = _tr_str_eqv(_cmpa_t3552, _cmpb_t3553); _tr_str_release(_cmpa_t3552); _cmpr_t3554; }) && List_bool_get(f->wafter, k)) && ({ TrStr _cmpa_t3555 = (List_TrStr_get(f->wprop, k)); TrStr _cmpb_t3556 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3557 = !_tr_str_eqv(_cmpa_t3555, _cmpb_t3556); _tr_str_release(_cmpa_t3555); _cmpr_t3557; })) && ({ TrStr _at_t3558 = (List_TrStr_get(f->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3558)); _tr_str_release(_at_t3558); _wr; }))) {
#line 1037 "src/racecheck.tr"
            ({ __auto_type _ord_t3559 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3560 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3559); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' writes the plain field '", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3561 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3560); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while a spawned thread holds the same shared object: the two threads race on that field.\n      FIX: make '", 108LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3562 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3561); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` or guard the object with a `Mutex[T]`, or finish the writes before spawning / after the threads are joined.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3562); _tr_str_release(_ord_t3562); });
#line 1038 "src/racecheck.tr"
            _tr_str_release(cn);
            return;
        }
#line 1039 "src/racecheck.tr"
        k = (k + 1LL);
    }
    _tr_str_release(cn);
}

__attribute__((hot)) void RcCk_check_globals(RcCk* self, RcFn* f, RcSite* s, long long ti, TrStr label) {
#line 1042 "src/racecheck.tr"
    List_i64* reach = RcCk_reach(self, ti);
#line 1044 "src/racecheck.tr"
    long long gi = 0LL;
#line 1045 "src/racecheck.tr"
    while ((gi < self->gnames->len)) {
#line 1046 "src/racecheck.tr"
        if ((!List_bool_get(self->gsync, gi))) {
#line 1047 "src/racecheck.tr"
            TrStr g = List_TrStr_get(self->gnames, gi);
#line 1048 "src/racecheck.tr"
            bool wr = false;
#line 1049 "src/racecheck.tr"
            bool rd = false;
#line 1050 "src/racecheck.tr"
            bool called = false;
#line 1051 "src/racecheck.tr"
            bool mwrites = false;
#line 1052 "src/racecheck.tr"
            long long ri = 0LL;
#line 1053 "src/racecheck.tr"
            while ((ri < reach->len)) {
#line 1054 "src/racecheck.tr"
                RcFn* rf = ({ __auto_type _ord_t3563 = (List_i64_get(reach, ri)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3563))); _wr; });
#line 1055 "src/racecheck.tr"
                long long ti2 = 0LL;
#line 1056 "src/racecheck.tr"
                while ((ti2 < rf->touched->len)) {
#line 1057 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3564 = (List_TrStr_get(rf->touched, ti2)); TrStr _cmpb_t3565 = (g); _Bool _cmpr_t3566 = _tr_str_eqv(_cmpa_t3564, _cmpb_t3565); _tr_str_release(_cmpa_t3564); _cmpr_t3566; })) {
#line 1058 "src/racecheck.tr"
                        if (List_bool_get(rf->touched_w, ti2)) {
#line 1058 "src/racecheck.tr"
                            wr = true;
                        } else {
#line 1059 "src/racecheck.tr"
                            rd = true;
                        }
                    }
#line 1060 "src/racecheck.tr"
                    ti2 = (ti2 + 1LL);
                }
#line 1061 "src/racecheck.tr"
                if (({ TrStr _at_t3567 = (List_TrStr_get(self->gtn, gi)); __auto_type _wr = ((RcCk_find_class(self, _at_t3567) >= 0LL)); _tr_str_release(_at_t3567); _wr; })) {
#line 1063 "src/racecheck.tr"
                    long long mq = 0LL;
#line 1064 "src/racecheck.tr"
                    while ((mq < rf->mcrecv->len)) {
#line 1065 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3568 = (List_TrStr_get(rf->mcrecv, mq)); TrStr _cmpb_t3569 = (g); _Bool _cmpr_t3570 = _tr_str_eqv(_cmpa_t3568, _cmpb_t3569); _tr_str_release(_cmpa_t3568); _cmpr_t3570; }) && (!_rc_has(rf->locals, g))) && (!_rc_has(rf->params, g)))) {
#line 1065 "src/racecheck.tr"
                            called = true;
                        }
#line 1066 "src/racecheck.tr"
                        mq = (mq + 1LL);
                    }
#line 1067 "src/racecheck.tr"
                    TrStr pre = ({ TrStr _tr_cl = (List_TrStr_get(self->gtn, gi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 1068 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3571 = (_tr_str_lenv((rf->name))); __auto_type _wr = ((_ord_t3571 > _tr_str_lenv((pre)))); _wr; }) && ({ __auto_type _ord_t3572 = (_tr_str_lenv((pre))); __auto_type _wr = (({ TrStr _cmpa_t3573 = (_tr_str_slicev((rf->name), 0LL, _ord_t3572)); TrStr _cmpb_t3574 = (pre); _Bool _cmpr_t3575 = _tr_str_eqv(_cmpa_t3573, _cmpb_t3574); _tr_str_release(_cmpa_t3573); _cmpr_t3575; })); _wr; }))) {
#line 1069 "src/racecheck.tr"
                        long long mk = 0LL;
#line 1070 "src/racecheck.tr"
                        while ((mk < rf->wroot->len)) {
#line 1071 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3576 = (List_TrStr_get(rf->wroot, mk)); TrStr _cmpb_t3577 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3578 = _tr_str_eqv(_cmpa_t3576, _cmpb_t3577); _tr_str_release(_cmpa_t3576); _cmpr_t3578; }) && ({ TrStr _cmpa_t3579 = (List_TrStr_get(rf->wprop, mk)); TrStr _cmpb_t3580 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3581 = !_tr_str_eqv(_cmpa_t3579, _cmpb_t3580); _tr_str_release(_cmpa_t3579); _cmpr_t3581; })) && ({ TrStr _ord_t3582 = (List_TrStr_get(self->gtn, gi)); TrStr _ord_t3583 = (List_TrStr_get(rf->wprop, mk)); __auto_type _wr = (RcCk_field_plain(self, _ord_t3582, _ord_t3583)); _tr_str_release(_ord_t3582); _tr_str_release(_ord_t3583); _wr; }))) {
#line 1071 "src/racecheck.tr"
                                mwrites = true;
                            }
#line 1072 "src/racecheck.tr"
                            mk = (mk + 1LL);
                        }
                    }
                }
#line 1073 "src/racecheck.tr"
                ri = (ri + 1LL);
            }
#line 1074 "src/racecheck.tr"
            if ((called && mwrites)) {
#line 1074 "src/racecheck.tr"
                wr = true;
            }
#line 1075 "src/racecheck.tr"
            if ((rd && (!wr))) {
#line 1076 "src/racecheck.tr"
                long long fi = 0LL;
#line 1077 "src/racecheck.tr"
                while ((fi < self->fns->len)) {
#line 1078 "src/racecheck.tr"
                    RcFn* of = ((RcFn*)List_ptr_get(self->fns, fi));
#line 1079 "src/racecheck.tr"
                    if ((!of->is_lib)) {
#line 1080 "src/racecheck.tr"
                        long long oi = 0LL;
#line 1081 "src/racecheck.tr"
                        while ((oi < of->touched->len)) {
#line 1082 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3584 = (List_TrStr_get(of->touched, oi)); TrStr _cmpb_t3585 = (g); _Bool _cmpr_t3586 = _tr_str_eqv(_cmpa_t3584, _cmpb_t3585); _tr_str_release(_cmpa_t3584); _cmpr_t3586; }) && List_bool_get(of->touched_w, oi)) && List_bool_get(of->touched_after, oi))) {
#line 1082 "src/racecheck.tr"
                                wr = true;
                            }
#line 1083 "src/racecheck.tr"
                            oi = (oi + 1LL);
                        }
                    }
#line 1084 "src/racecheck.tr"
                    fi = (fi + 1LL);
                }
            }
#line 1085 "src/racecheck.tr"
            if (wr) {
#line 1086 "src/racecheck.tr"
                ({ TrStr _ord_t3587 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] the spawned function '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (label)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses the module-level mutable '", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is plain data shared by every thread: concurrent access is a data race.\n      FIX: make '", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (counters/flags), a `Mutex[T]`/`RwLock[T]` (compound state), or pass the state to the thread explicitly.", 121LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3587); _tr_str_release(_ord_t3587); });
            } else if (rd) {
#line 1088 "src/racecheck.tr"
                /* pass */
            }
        }
#line 1089 "src/racecheck.tr"
        gi = (gi + 1LL);
    }
}

__attribute__((hot)) TrStr RcCk_param_write_prop(RcCk* self, long long fi, TrStr pname, TrStr cn, long long depth) {
#line 1094 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, fi));
#line 1095 "src/racecheck.tr"
    long long wi = 0LL;
#line 1096 "src/racecheck.tr"
    while ((wi < tf->wroot->len)) {
#line 1097 "src/racecheck.tr"
        if (((({ TrStr _cmpa_t3588 = (List_TrStr_get(tf->wroot, wi)); TrStr _cmpb_t3589 = (pname); _Bool _cmpr_t3590 = _tr_str_eqv(_cmpa_t3588, _cmpb_t3589); _tr_str_release(_cmpa_t3588); _cmpr_t3590; }) && ({ TrStr _cmpa_t3591 = (List_TrStr_get(tf->wprop, wi)); TrStr _cmpb_t3592 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3593 = !_tr_str_eqv(_cmpa_t3591, _cmpb_t3592); _tr_str_release(_cmpa_t3591); _cmpr_t3593; })) && ({ TrStr _at_t3594 = (List_TrStr_get(tf->wprop, wi)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3594)); _tr_str_release(_at_t3594); _wr; }))) {
#line 1097 "src/racecheck.tr"
            TrStr _trret588 = List_TrStr_get(tf->wprop, wi);
            return _trret588;
        }
#line 1098 "src/racecheck.tr"
        wi = (wi + 1LL);
    }
#line 1099 "src/racecheck.tr"
    long long mi = 0LL;
#line 1100 "src/racecheck.tr"
    while ((mi < tf->mcrecv->len)) {
#line 1101 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3595 = (List_TrStr_get(tf->mcrecv, mi)); TrStr _cmpb_t3596 = (pname); _Bool _cmpr_t3597 = _tr_str_eqv(_cmpa_t3595, _cmpb_t3596); _tr_str_release(_cmpa_t3595); _cmpr_t3597; })) {
#line 1102 "src/racecheck.tr"
            long long mf = ({ TrStr _at_t3598 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(tf->mcname, mi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3598)); _tr_str_release(_at_t3598); _wr; });
#line 1103 "src/racecheck.tr"
            if ((mf >= 0LL)) {
#line 1104 "src/racecheck.tr"
                RcFn* mfn = ((RcFn*)List_ptr_get(self->fns, mf));
#line 1105 "src/racecheck.tr"
                long long k = 0LL;
#line 1106 "src/racecheck.tr"
                while ((k < mfn->wroot->len)) {
#line 1107 "src/racecheck.tr"
                    if (((({ TrStr _cmpa_t3599 = (List_TrStr_get(mfn->wroot, k)); TrStr _cmpb_t3600 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3601 = _tr_str_eqv(_cmpa_t3599, _cmpb_t3600); _tr_str_release(_cmpa_t3599); _cmpr_t3601; }) && ({ TrStr _cmpa_t3602 = (List_TrStr_get(mfn->wprop, k)); TrStr _cmpb_t3603 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3604 = !_tr_str_eqv(_cmpa_t3602, _cmpb_t3603); _tr_str_release(_cmpa_t3602); _cmpr_t3604; })) && ({ TrStr _at_t3605 = (List_TrStr_get(mfn->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3605)); _tr_str_release(_at_t3605); _wr; }))) {
#line 1107 "src/racecheck.tr"
                        TrStr _trret589 = List_TrStr_get(mfn->wprop, k);
                        return _trret589;
                    }
#line 1108 "src/racecheck.tr"
                    k = (k + 1LL);
                }
            }
        }
#line 1109 "src/racecheck.tr"
        mi = (mi + 1LL);
    }
#line 1110 "src/racecheck.tr"
    if ((depth < 4LL)) {
#line 1111 "src/racecheck.tr"
        long long ci = 0LL;
#line 1112 "src/racecheck.tr"
        while ((ci < tf->ca_fn->len)) {
#line 1113 "src/racecheck.tr"
            if (({ TrStr _cmpa_t3606 = (List_TrStr_get(tf->ca_name, ci)); TrStr _cmpb_t3607 = (pname); _Bool _cmpr_t3608 = _tr_str_eqv(_cmpa_t3606, _cmpb_t3607); _tr_str_release(_cmpa_t3606); _cmpr_t3608; })) {
#line 1114 "src/racecheck.tr"
                long long ni = ({ TrStr _at_t3609 = (List_TrStr_get(tf->ca_fn, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3609)); _tr_str_release(_at_t3609); _wr; });
#line 1115 "src/racecheck.tr"
                if (((ni >= 0LL) && ({ __auto_type _ord_t3610 = (List_i64_get(tf->ca_idx, ci)); __auto_type _wr = ((_ord_t3610 < ((RcFn*)List_ptr_get(self->fns, ni))->params->len)); _wr; }))) {
#line 1116 "src/racecheck.tr"
                    TrStr r = ({ __auto_type _ord_t3611 = (List_i64_get(tf->ca_idx, ci)); TrStr _at_t3612 = (List_TrStr_get(((RcFn*)List_ptr_get(self->fns, ni))->params, _ord_t3611)); __auto_type _wr = (RcCk_param_write_prop(self, ni, _at_t3612, cn, (depth + 1LL))); _tr_str_release(_at_t3612); _wr; });
#line 1117 "src/racecheck.tr"
                    if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 1117 "src/racecheck.tr"
                        TrStr _trret590 = _tr_str_retain(r);
                        _tr_str_release(r);
                        return _trret590;
                    }
                }
            }
#line 1118 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
    }
#line 1119 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void RcCk_check_shared_writes(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, long long ai, TrStr an) {
#line 1122 "src/racecheck.tr"
    TrStr pn = List_TrStr_get(tf->params, ai);
#line 1123 "src/racecheck.tr"
    TrStr cn = List_TrStr_get(tf->ptypes, ai);
#line 1124 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("Shared", 6LL)))) {
#line 1124 "src/racecheck.tr"
        TrStr _strtmp_t3613 = List_TrStr_get(tf->pinner, ai);
        _tr_str_release(cn);
        cn = _strtmp_t3613;
    }
#line 1125 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1126 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 1127 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3614 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3614)); _tr_str_release(_apt_t3614); _wr; });
#line 1128 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 1128 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 1129 "src/racecheck.tr"
    TrStr pr = RcCk_param_write_prop(self, ti, pn, cn, 0LL);
#line 1130 "src/racecheck.tr"
    if ((!_tr_str_eqv((pr), (_tr_str_lit_len("", 0LL))))) {
#line 1130 "src/racecheck.tr"
        RcCk_shared_msg(self, f, s, tf, cn, pr, an);
    }
    _tr_str_release(pn);
    _tr_str_release(cn);
    _tr_str_release(pr);
}

__attribute__((hot)) void RcCk_shared_msg(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, TrStr cn, TrStr prop, TrStr an) {
#line 1133 "src/racecheck.tr"
    ({ __auto_type _ord_t3615 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3616 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3615); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' mutates the plain field '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is shared by several threads: `Shared[T]` only makes the reference COUNT atomic, not the fields, so the concurrent writes race.\n      FIX: make '", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (or guard the whole object with `Mutex[", 56LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`), or give each thread its own value.", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3616); _tr_str_release(_ord_t3616); });
}

__attribute__((hot)) TrStr RcCk_where2(RcCk* self, RcFn* f, long long line) {
#line 1136 "src/racecheck.tr"
    long long l = line;
#line 1137 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 1137 "src/racecheck.tr"
        l = f->line;
    }
#line 1138 "src/racecheck.tr"
    return ({ __auto_type _ord_t3617 = (_tr_strx_concatv((f->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3617); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) RcFn* _rc_new_fn(TrStr name, TrStr file, long long line, bool is_lib) {
#line 66 "src/racecheck.tr"
    RcFn* f = ((RcFn*)_tr_obj_alloc(sizeof(RcFn)));
#line 67 "src/racecheck.tr"
    TrStr _sfo_t3618 = f->name;
    f->name = _tr_str_retain(name);
    _tr_str_release(_sfo_t3618);
#line 68 "src/racecheck.tr"
    TrStr _sfo_t3619 = f->file;
    f->file = _tr_str_retain(file);
    _tr_str_release(_sfo_t3619);
#line 69 "src/racecheck.tr"
    f->line = line;
#line 70 "src/racecheck.tr"
    f->is_lib = is_lib;
#line 71 "src/racecheck.tr"
    f->params = (void*)List_TrStr_new();
#line 72 "src/racecheck.tr"
    f->ptypes = (void*)List_TrStr_new();
#line 73 "src/racecheck.tr"
    f->pinner = (void*)List_TrStr_new();
#line 74 "src/racecheck.tr"
    f->locals = (void*)List_TrStr_new();
#line 75 "src/racecheck.tr"
    f->ltn = (void*)List_TrStr_new();
#line 76 "src/racecheck.tr"
    f->ltt = (void*)List_TrStr_new();
#line 77 "src/racecheck.tr"
    f->ltarg = (void*)List_TrStr_new();
#line 78 "src/racecheck.tr"
    f->ldepth = (void*)List_i64_new();
#line 79 "src/racecheck.tr"
    f->wafter = (void*)List_bool_new();
#line 80 "src/racecheck.tr"
    f->idents = (void*)List_TrStr_new();
#line 81 "src/racecheck.tr"
    f->ca_fn = (void*)List_TrStr_new();
#line 82 "src/racecheck.tr"
    f->ca_idx = (void*)List_i64_new();
#line 83 "src/racecheck.tr"
    f->ca_name = (void*)List_TrStr_new();
#line 84 "src/racecheck.tr"
    f->thr_made = (void*)List_TrStr_new();
#line 85 "src/racecheck.tr"
    f->thr_line = (void*)List_i64_new();
#line 86 "src/racecheck.tr"
    f->pairs_a = (void*)List_TrStr_new();
#line 87 "src/racecheck.tr"
    f->pairs_b = (void*)List_TrStr_new();
#line 88 "src/racecheck.tr"
    f->touched = (void*)List_TrStr_new();
#line 89 "src/racecheck.tr"
    f->touched_w = (void*)List_bool_new();
#line 90 "src/racecheck.tr"
    f->touched_after = (void*)List_bool_new();
#line 91 "src/racecheck.tr"
    f->touched_line = (void*)List_i64_new();
#line 92 "src/racecheck.tr"
    f->calls = (void*)List_TrStr_new();
#line 93 "src/racecheck.tr"
    f->sites = (void*)List_ptr_new();
#line 94 "src/racecheck.tr"
    f->closures = (void*)List_TrStr_new();
#line 95 "src/racecheck.tr"
    f->shvars = (void*)List_TrStr_new();
#line 96 "src/racecheck.tr"
    f->wroot = (void*)List_TrStr_new();
#line 97 "src/racecheck.tr"
    f->wprop = (void*)List_TrStr_new();
#line 98 "src/racecheck.tr"
    f->mcrecv = (void*)List_TrStr_new();
#line 99 "src/racecheck.tr"
    f->mcname = (void*)List_TrStr_new();
#line 100 "src/racecheck.tr"
    f->msgs = (void*)List_TrStr_new();
#line 101 "src/racecheck.tr"
    return f;
}

__attribute__((hot)) bool _rc_sync_ty(TrStr n) {
#line 119 "src/racecheck.tr"
    return (((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("WaitGroup", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Semaphore", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Once", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Barrier", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Condvar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL))));
}

__attribute__((hot)) bool _rc_mut_method(TrStr m) {
#line 122 "src/racecheck.tr"
    return (((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("update", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("delete", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_back", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop_back", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("truncate", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL))));
}

__attribute__((hot)) bool _rc_has(List_TrStr* v, TrStr s) {
#line 125 "src/racecheck.tr"
    long long i = 0LL;
#line 126 "src/racecheck.tr"
    while ((i < v->len)) {
#line 127 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3620 = (List_TrStr_get(v, i)); TrStr _cmpb_t3621 = (s); _Bool _cmpr_t3622 = _tr_str_eqv(_cmpa_t3620, _cmpb_t3621); _tr_str_release(_cmpa_t3620); _cmpr_t3622; })) {
#line 127 "src/racecheck.tr"
            return true;
        }
#line 128 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 129 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_nul(Expr* p) {
#line 132 "src/racecheck.tr"
    return (((unsigned long long)(p)) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr _rc_root(Expr* e) {
#line 136 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 136 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 137 "src/racecheck.tr"
    __auto_type _t3623 = (*e);
    if (_t3623.tag == Expr_EIdent) {
        __auto_type n = _t3623.data.EIdent.name;
        TrStr _trret591 = _tr_str_retain(n);
        return _trret591;
    } else if (_t3623.tag == Expr_EPropAccess) {
        __auto_type o = _t3623.data.EPropAccess.obj;
        return _rc_root(o);
    } else if (_t3623.tag == Expr_EIndex) {
        __auto_type o = _t3623.data.EIndex.obj;
        return _rc_root(o);
    } else if (1) {
        __auto_type _ = _t3623;
        return _tr_str_lit_len("", 0LL);
    }
#line 142 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_first_prop(Expr* e) {
#line 146 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 146 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 147 "src/racecheck.tr"
    __auto_type _t3624 = (*e);
    if (_t3624.tag == Expr_EPropAccess) {
        __auto_type o = _t3624.data.EPropAccess.obj;
__auto_type p = _t3624.data.EPropAccess.prop;
#line 149 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 150 "src/racecheck.tr"
            __auto_type _t3625 = (*o);
            if (_t3625.tag == Expr_EIdent) {
                __auto_type _ = _t3625.data.EIdent.name;
                TrStr _trret592 = _tr_str_retain(p);
                return _trret592;
            } else if (1) {
                __auto_type _ = _t3625;
                /* pass */
            }
        }
#line 153 "src/racecheck.tr"
        TrStr _trret593 = _rc_first_prop(o);
        return _trret593;
    } else if (_t3624.tag == Expr_EIndex) {
        __auto_type o = _t3624.data.EIndex.obj;
        return _rc_first_prop(o);
    } else if (1) {
        __auto_type _ = _t3624;
        return _tr_str_lit_len("", 0LL);
    }
#line 156 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_ctor_name(Expr* v) {
#line 160 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 160 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 161 "src/racecheck.tr"
    __auto_type _t3626 = (*v);
    if (_t3626.tag == Expr_EMethodCall) {
        __auto_type o = _t3626.data.EMethodCall.obj;
        return _rc_root(o);
    } else if (_t3626.tag == Expr_ECall) {
        __auto_type c = _t3626.data.ECall.callee;
        return _rc_root(c);
    } else if (1) {
        __auto_type _ = _t3626;
        return _tr_str_lit_len("", 0LL);
    }
#line 165 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_ctor_arg(Expr* v) {
#line 169 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 169 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 170 "src/racecheck.tr"
    __auto_type _t3627 = (*v);
    if (_t3627.tag == Expr_EMethodCall) {
        __auto_type o = _t3627.data.EMethodCall.obj;
__auto_type margs = _t3627.data.EMethodCall.args;
#line 172 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 173 "src/racecheck.tr"
            __auto_type _t3628 = (*o);
            if (_t3628.tag == Expr_EIndex) {
                __auto_type ix = _t3628.data.EIndex._tr_v_index;
#line 175 "src/racecheck.tr"
                if ((!_rc_nul(ix))) {
#line 176 "src/racecheck.tr"
                    __auto_type _t3629 = (*ix);
                    if (_t3629.tag == Expr_EIdent) {
                        __auto_type an = _t3629.data.EIdent.name;
                        TrStr _trret594 = _tr_str_retain(an);
                        return _trret594;
                    } else if (1) {
                        __auto_type _ = _t3629;
                        return _tr_str_lit_len("", 0LL);
                    }
                }
            } else if (_t3628.tag == Expr_EIdent) {
                __auto_type on = _t3628.data.EIdent.name;
#line 181 "src/racecheck.tr"
                if ((_tr_str_eqv((on), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((on), (_tr_str_lit_len("RwLock", 6LL))))) {
#line 182 "src/racecheck.tr"
                    if (((margs->len > 0LL) && (!_rc_nul(((Expr*)List_ptr_get(margs, 0LL)))))) {
#line 183 "src/racecheck.tr"
                        __auto_type _t3630 = (*((Expr*)List_ptr_get(margs, 0LL)));
                        if (_t3630.tag == Expr_ECall) {
                            __auto_type pc = _t3630.data.ECall.callee;
#line 185 "src/racecheck.tr"
                            if ((!_rc_nul(pc))) {
#line 186 "src/racecheck.tr"
                                __auto_type _t3631 = (*pc);
                                if (_t3631.tag == Expr_EIdent) {
                                    __auto_type pcn = _t3631.data.EIdent.name;
                                    TrStr _trret595 = _tr_str_retain(pcn);
                                    return _trret595;
                                } else if (1) {
                                    __auto_type _ = _t3631;
                                    return _tr_str_lit_len("", 0LL);
                                }
                            }
                        } else if (_t3630.tag == Expr_EList) {
                            __auto_type _ = _t3630.data.EList.items;
                            return _tr_str_lit_len("List", 4LL);
                        } else if (_t3630.tag == Expr_EDict) {
                            return _tr_str_lit_len("Dict", 4LL);
                        } else if (1) {
                            __auto_type _ = _t3630;
                            return _tr_str_lit_len("", 0LL);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t3628;
                return _tr_str_lit_len("", 0LL);
            }
        }
    } else if (1) {
        __auto_type _ = _t3627;
        return _tr_str_lit_len("", 0LL);
    }
#line 194 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long _rc_dot(TrStr n) {
#line 197 "src/racecheck.tr"
    long long i = 0LL;
#line 198 "src/racecheck.tr"
    while ((i < _tr_str_lenv((n)))) {
#line 199 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3632 = (_tr_str_slicev((n), i, (i + 1LL))); TrStr _cmpb_t3633 = (_tr_str_lit_len(".", 1LL)); _Bool _cmpr_t3634 = _tr_str_eqv(_cmpa_t3632, _cmpb_t3633); _tr_str_release(_cmpa_t3632); _cmpr_t3634; })) {
#line 199 "src/racecheck.tr"
            return i;
        }
#line 200 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 201 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool _rc_is_thread_spawn(Expr* v) {
#line 204 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 204 "src/racecheck.tr"
        return false;
    }
#line 205 "src/racecheck.tr"
    __auto_type _t3635 = (*v);
    if (_t3635.tag == Expr_EMethodCall) {
        __auto_type o = _t3635.data.EMethodCall.obj;
__auto_type m = _t3635.data.EMethodCall.method;
#line 207 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && ({ TrStr _cmpa_t3636 = (_rc_root(o)); TrStr _cmpb_t3637 = (_tr_str_lit_len("Thread", 6LL)); _Bool _cmpr_t3638 = _tr_str_eqv(_cmpa_t3636, _cmpb_t3637); _tr_str_release(_cmpa_t3636); _cmpr_t3638; }))) {
#line 207 "src/racecheck.tr"
            return true;
        }
    } else if (1) {
        __auto_type _ = _t3635;
        /* pass */
    }
#line 209 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_is_lambda(Expr* v) {
#line 212 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 212 "src/racecheck.tr"
        return false;
    }
#line 213 "src/racecheck.tr"
    __auto_type _t3639 = (*v);
    if (_t3639.tag == Expr_EClosure) {
        return true;
    } else if (1) {
        __auto_type _ = _t3639;
        return false;
    }
#line 216 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_path_lib(TrStr p) {
#line 1142 "src/racecheck.tr"
    return (((_tr_str_containsv((p), (_tr_str_lit_len("std/", 4LL))) || _tr_str_containsv((p), (_tr_str_lit_len("core/", 5LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("std\\", 4LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("core\\", 5LL))));
}

__attribute__((hot)) bool _rc_ifaces_sendable(ClassDef* c) {
#line 1145 "src/racecheck.tr"
    long long i = 0LL;
#line 1146 "src/racecheck.tr"
    while ((i < c->iface_names->len)) {
#line 1147 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3640 = (List_TrStr_get(c->iface_names, i)); TrStr _cmpb_t3641 = (_tr_str_lit_len("Sendable", 8LL)); _Bool _cmpr_t3642 = _tr_str_eqv(_cmpa_t3640, _cmpb_t3641); _tr_str_release(_cmpa_t3640); _cmpr_t3642; })) {
#line 1147 "src/racecheck.tr"
            return true;
        }
#line 1148 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 1149 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_TrStr* check_data_races(Program* prog, List_TrStr* decl_files, TrStr input_path) {
#line 1152 "src/racecheck.tr"
    RcCk* ck = ((RcCk*)_tr_obj_alloc(sizeof(RcCk)));
#line 1153 "src/racecheck.tr"
    ck->held_for = _tr_dict_new(8LL);
#line 1154 "src/racecheck.tr"
    ck->fns = (void*)List_ptr_new();
#line 1155 "src/racecheck.tr"
    ck->classes = (void*)List_ptr_new();
#line 1156 "src/racecheck.tr"
    ck->class_lib = (void*)List_bool_new();
#line 1157 "src/racecheck.tr"
    ck->gnames = (void*)List_TrStr_new();
#line 1158 "src/racecheck.tr"
    ck->gfiles = (void*)List_TrStr_new();
#line 1159 "src/racecheck.tr"
    ck->gsync = (void*)List_bool_new();
#line 1160 "src/racecheck.tr"
    ck->gtn = (void*)List_TrStr_new();
#line 1161 "src/racecheck.tr"
    ck->msgs = (void*)List_TrStr_new();
#line 1162 "src/racecheck.tr"
    RcFn* _cltmp_t3643 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3643;
#line 1163 "src/racecheck.tr"
    RcFn* _cltmp_t3644 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3644;
#line 1164 "src/racecheck.tr"
    long long i = 0LL;
#line 1165 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1166 "src/racecheck.tr"
        TrStr dfc = _tr_str_retain(input_path);
#line 1167 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1167 "src/racecheck.tr"
            TrStr _strtmp_t3645 = List_TrStr_get(decl_files, i);
            _tr_str_release(dfc);
            dfc = _strtmp_t3645;
        }
#line 1168 "src/racecheck.tr"
        __auto_type _t3646 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3646.tag == Decl_DClass) {
            __auto_type c = _t3646.data.DClass.cls;
#line 1170 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c));
#line 1171 "src/racecheck.tr"
            ({ __auto_type _ord_t3647 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3647); });
#line 1172 "src/racecheck.tr"
            if ((!_rc_path_lib(dfc))) {
#line 1173 "src/racecheck.tr"
                long long fq = 0LL;
#line 1174 "src/racecheck.tr"
                while ((fq < c->fields->len)) {
#line 1175 "src/racecheck.tr"
                    ({ __auto_type _ord_t3648 = (((FieldDef*)List_ptr_get(c->fields, fq))->ty); __auto_type _ord_t3649 = (c->line); RcCk_ty_check(ck, _ord_t3648, dfc, _ord_t3649); });
#line 1176 "src/racecheck.tr"
                    fq = (fq + 1LL);
                }
            }
        } else if (_t3646.tag == Decl_DActor) {
            __auto_type c2 = _t3646.data.DActor.cls;
#line 1178 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c2));
#line 1179 "src/racecheck.tr"
            ({ __auto_type _ord_t3650 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3650); });
        } else if (1) {
            __auto_type _ = _t3646;
            /* pass */
        }
#line 1181 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(dfc);
    }
#line 1182 "src/racecheck.tr"
    i = 0LL;
#line 1183 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1184 "src/racecheck.tr"
        TrStr df = _tr_str_retain(input_path);
#line 1185 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1185 "src/racecheck.tr"
            TrStr _strtmp_t3651 = List_TrStr_get(decl_files, i);
            _tr_str_release(df);
            df = _strtmp_t3651;
        }
#line 1186 "src/racecheck.tr"
        bool lib = _rc_path_lib(df);
#line 1187 "src/racecheck.tr"
        __auto_type _t3652 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3652.tag == Decl_DTopLevelStmt) {
            __auto_type sp = _t3652.data.DTopLevelStmt.stmt;
#line 1189 "src/racecheck.tr"
            __auto_type _t3653 = (*sp);
            if (_t3653.tag == Stmt_SLet) {
                __auto_type name = _t3653.data.SLet.name;
__auto_type is_mut = _t3653.data.SLet.is_mut;
__auto_type is_const = _t3653.data.SLet.is_const;
__auto_type ty = _t3653.data.SLet.ty;
__auto_type v = _t3653.data.SLet.val;
#line 1191 "src/racecheck.tr"
                if ((!lib)) {
#line 1191 "src/racecheck.tr"
                    RcCk_add_global(ck, name, is_mut, is_const, ty, v, df);
                }
            } else if (_t3653.tag == Stmt_SAssign) {
                __auto_type tg = _t3653.data.SAssign.target;
__auto_type v3 = _t3653.data.SAssign.val;
#line 1194 "src/racecheck.tr"
                if (((!lib) && (!_rc_nul(tg)))) {
#line 1195 "src/racecheck.tr"
                    __auto_type _t3654 = (*tg);
                    if (_t3654.tag == Expr_EIdent) {
                        __auto_type gn = _t3654.data.EIdent.name;
                        RcCk_add_global(ck, gn, false, false, (AstType**)(0LL), v3, df);
                    } else if (1) {
                        __auto_type _ = _t3654;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t3653;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t3652;
            /* pass */
        }
#line 1200 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df);
    }
#line 1201 "src/racecheck.tr"
    i = 0LL;
#line 1202 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1203 "src/racecheck.tr"
        TrStr df2 = _tr_str_retain(input_path);
#line 1204 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1204 "src/racecheck.tr"
            TrStr _strtmp_t3655 = List_TrStr_get(decl_files, i);
            _tr_str_release(df2);
            df2 = _strtmp_t3655;
        }
#line 1205 "src/racecheck.tr"
        bool lib2 = _rc_path_lib(df2);
#line 1206 "src/racecheck.tr"
        __auto_type _t3656 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3656.tag == Decl_DFunction) {
            __auto_type f = _t3656.data.DFunction.func;
            RcCk_add_fn(ck, f, df2, lib2);
        } else if (_t3656.tag == Decl_DClass) {
            __auto_type c3 = _t3656.data.DClass.cls;
#line 1209 "src/racecheck.tr"
            long long mi = 0LL;
#line 1210 "src/racecheck.tr"
            while ((mi < c3->methods->len)) {
#line 1211 "src/racecheck.tr"
                ({ __auto_type _ord_t3657 = (((FunctionDef*)List_ptr_get(c3->methods, mi))); __auto_type _ord_t3658 = (c3->name); RcCk_add_method(ck, _ord_t3657, _ord_t3658, df2, lib2); });
#line 1212 "src/racecheck.tr"
                mi = (mi + 1LL);
            }
        } else if (_t3656.tag == Decl_DActor) {
            __auto_type c4 = _t3656.data.DActor.cls;
#line 1214 "src/racecheck.tr"
            long long mi2 = 0LL;
#line 1215 "src/racecheck.tr"
            while ((mi2 < c4->methods->len)) {
#line 1216 "src/racecheck.tr"
                ({ __auto_type _ord_t3659 = (((FunctionDef*)List_ptr_get(c4->methods, mi2))); __auto_type _ord_t3660 = (c4->name); RcCk_add_method(ck, _ord_t3659, _ord_t3660, df2, lib2); });
#line 1217 "src/racecheck.tr"
                mi2 = (mi2 + 1LL);
            }
        } else if (_t3656.tag == Decl_DExtend) {
            __auto_type tgt = _t3656.data.DExtend.target;
__auto_type methods = _t3656.data.DExtend.methods;
#line 1219 "src/racecheck.tr"
            long long mi3 = 0LL;
#line 1220 "src/racecheck.tr"
            while ((mi3 < methods->len)) {
#line 1221 "src/racecheck.tr"
                RcCk_add_method(ck, ((FunctionDef*)List_ptr_get(methods, mi3)), tgt, df2, lib2);
#line 1222 "src/racecheck.tr"
                mi3 = (mi3 + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t3656;
            /* pass */
        }
#line 1224 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df2);
    }
#line 1225 "src/racecheck.tr"
    i = 0LL;
#line 1226 "src/racecheck.tr"
    while ((i < ck->fns->len)) {
#line 1227 "src/racecheck.tr"
        RcFn* f = ((RcFn*)List_ptr_get(ck->fns, i));
#line 1228 "src/racecheck.tr"
        if ((!f->is_lib)) {
#line 1229 "src/racecheck.tr"
            long long mj = 0LL;
#line 1230 "src/racecheck.tr"
            while ((mj < f->msgs->len)) {
#line 1231 "src/racecheck.tr"
                ({ TrStr _ord_t3661 = (List_TrStr_get(f->msgs, mj)); List_TrStr_append(ck->msgs, _ord_t3661); _tr_str_release(_ord_t3661); });
#line 1232 "src/racecheck.tr"
                mj = (mj + 1LL);
            }
#line 1233 "src/racecheck.tr"
            long long sj = 0LL;
#line 1234 "src/racecheck.tr"
            while ((sj < f->sites->len)) {
#line 1235 "src/racecheck.tr"
                RcCk_analyze_site(ck, f, ((RcSite*)List_ptr_get(f->sites, sj)));
#line 1236 "src/racecheck.tr"
                sj = (sj + 1LL);
            }
        }
#line 1237 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 1239 "src/racecheck.tr"
    long long pi = 0LL;
#line 1240 "src/racecheck.tr"
    while ((pi < ck->fns->len)) {
#line 1241 "src/racecheck.tr"
        RcFn* pf = ((RcFn*)List_ptr_get(ck->fns, pi));
#line 1242 "src/racecheck.tr"
        long long pk = 0LL;
#line 1243 "src/racecheck.tr"
        while ((pk < pf->pairs_a->len)) {
#line 1244 "src/racecheck.tr"
            long long qi = (pi + 1LL);
#line 1245 "src/racecheck.tr"
            while ((qi < ck->fns->len)) {
#line 1246 "src/racecheck.tr"
                RcFn* qf = ((RcFn*)List_ptr_get(ck->fns, qi));
#line 1247 "src/racecheck.tr"
                long long qk = 0LL;
#line 1248 "src/racecheck.tr"
                while ((qk < qf->pairs_a->len)) {
#line 1249 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3662 = (List_TrStr_get(qf->pairs_a, qk)); __auto_type _wr = (({ TrStr _cmpa_t3663 = (_ord_t3662); TrStr _cmpb_t3664 = (List_TrStr_get(pf->pairs_b, pk)); _Bool _cmpr_t3665 = _tr_str_eqv(_cmpa_t3663, _cmpb_t3664); _tr_str_release(_cmpa_t3663); _tr_str_release(_cmpb_t3664); _cmpr_t3665; })); _wr; }) && ({ __auto_type _ord_t3666 = (List_TrStr_get(qf->pairs_b, qk)); __auto_type _wr = (({ TrStr _cmpa_t3667 = (_ord_t3666); TrStr _cmpb_t3668 = (List_TrStr_get(pf->pairs_a, pk)); _Bool _cmpr_t3669 = _tr_str_eqv(_cmpa_t3667, _cmpb_t3668); _tr_str_release(_cmpa_t3667); _tr_str_release(_cmpb_t3668); _cmpr_t3669; })); _wr; }))) {
#line 1250 "src/racecheck.tr"
                        ({ __auto_type _ord_t3670 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("W|", 2LL)), (pf->file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3671 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3670); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pf->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] lock-order inversion: '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3672 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3671); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks '", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3673 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3672); TrStr _tr_cr = (List_TrStr_get(pf->pairs_a, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' then '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3674 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3673); TrStr _tr_cr = (List_TrStr_get(pf->pairs_b, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3675 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3674); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (qf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks them in the opposite order: two threads running them can deadlock.\n      FIX: always acquire the mutexes in one global order.", 133LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(ck->msgs, _ord_t3675); _tr_str_release(_ord_t3675); });
                    }
#line 1251 "src/racecheck.tr"
                    qk = (qk + 1LL);
                }
#line 1252 "src/racecheck.tr"
                qi = (qi + 1LL);
            }
#line 1253 "src/racecheck.tr"
            pk = (pk + 1LL);
        }
#line 1254 "src/racecheck.tr"
        pi = (pi + 1LL);
    }
#line 1255 "src/racecheck.tr"
    return ck->msgs;
}

