#include "tauraro_types.h"

bool _rc_spawned(RcCk* ck, TrStr name);
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
#line 234 "src/racecheck.tr"
    if ((((unsigned long long)(t)) == ((unsigned long long)(0LL)))) {
#line 234 "src/racecheck.tr"
        return;
    }
#line 235 "src/racecheck.tr"
    AstType* ty = (*t);
#line 236 "src/racecheck.tr"
    bool coll = ((((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL))));
#line 237 "src/racecheck.tr"
    long long i = 0LL;
#line 238 "src/racecheck.tr"
    while ((i < ty->args->len)) {
#line 239 "src/racecheck.tr"
        AstType** a = ((AstType**)List_ptr_get(ty->args, i));
#line 240 "src/racecheck.tr"
        if (((((coll && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))))) && (!_tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL))))) && (((unsigned long long)(a)) != ((unsigned long long)(0LL)))) && (_tr_str_eqv(((*a)->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv(((*a)->name), (_tr_str_lit_len("Weak", 4LL)))))) {
#line 241 "src/racecheck.tr"
            ({ __auto_type _ord_t3382 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((file), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-11] '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3383 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3382); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot hold '", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3384 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3383); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*a)->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[..]' elements: reference-counted shared handles are supported as `List`/`Vec` elements only.\n      FIX: use a `List[", 117LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3385 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3384); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*a)->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[..]]`, or wrap the handle in a small class (`class Handle: pub inner: Shared[T]`) and store that.", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3385); _tr_str_release(_ord_t3385); });
#line 242 "src/racecheck.tr"
            return;
        }
#line 243 "src/racecheck.tr"
        RcCk_ty_check(self, a, file, line);
#line 244 "src/racecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) TrStr RcCk_where(RcCk* self, long long line) {
#line 247 "src/racecheck.tr"
    long long l = line;
#line 248 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 248 "src/racecheck.tr"
        l = self->cur->line;
    }
#line 249 "src/racecheck.tr"
    return ({ __auto_type _ord_t3386 = (_tr_strx_concatv((self->cur->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3386); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) TrStr RcCk_ltype(RcCk* self, RcFn* f, TrStr name) {
#line 252 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 253 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 254 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3387 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t3388 = (name); _Bool _cmpr_t3389 = _tr_str_eqv(_cmpa_t3387, _cmpb_t3388); _tr_str_release(_cmpa_t3387); _cmpr_t3389; })) {
#line 254 "src/racecheck.tr"
            return List_TrStr_get(f->ptypes, i);
        }
#line 255 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 256 "src/racecheck.tr"
    i = (f->ltn->len - 1LL);
#line 257 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 258 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3390 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3391 = (name); _Bool _cmpr_t3392 = _tr_str_eqv(_cmpa_t3390, _cmpb_t3391); _tr_str_release(_cmpa_t3390); _cmpr_t3392; })) {
#line 258 "src/racecheck.tr"
            return List_TrStr_get(f->ltt, i);
        }
#line 259 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 260 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr RcCk_ltarg_of(RcCk* self, RcFn* f, TrStr name) {
#line 263 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 264 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 265 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3393 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t3394 = (name); _Bool _cmpr_t3395 = _tr_str_eqv(_cmpa_t3393, _cmpb_t3394); _tr_str_release(_cmpa_t3393); _cmpr_t3395; })) {
#line 265 "src/racecheck.tr"
            return List_TrStr_get(f->pinner, i);
        }
#line 266 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 267 "src/racecheck.tr"
    i = (f->ltn->len - 1LL);
#line 268 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 269 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3396 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3397 = (name); _Bool _cmpr_t3398 = _tr_str_eqv(_cmpa_t3396, _cmpb_t3397); _tr_str_release(_cmpa_t3396); _cmpr_t3398; })) {
#line 269 "src/racecheck.tr"
            return List_TrStr_get(f->ltarg, i);
        }
#line 270 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 271 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr RcCk_lclass(RcCk* self, RcFn* f, TrStr name) {
#line 275 "src/racecheck.tr"
    TrStr t = RcCk_ltype(self, f, name);
#line 276 "src/racecheck.tr"
    if (_tr_str_eqv((t), (_tr_str_lit_len("Shared", 6LL)))) {
#line 276 "src/racecheck.tr"
        TrStr _trret591 = RcCk_ltarg_of(self, f, name);
        _tr_str_release(t);
        return _trret591;
    }
#line 277 "src/racecheck.tr"
    TrStr _trret592 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret592;
}

__attribute__((hot)) long long RcCk_ldepth_of(RcCk* self, RcFn* f, TrStr name) {
#line 280 "src/racecheck.tr"
    if (_rc_has(f->params, name)) {
#line 280 "src/racecheck.tr"
        return 0LL;
    }
#line 281 "src/racecheck.tr"
    long long i = (f->ltn->len - 1LL);
#line 282 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 283 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3399 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3400 = (name); _Bool _cmpr_t3401 = _tr_str_eqv(_cmpa_t3399, _cmpb_t3400); _tr_str_release(_cmpa_t3399); _cmpr_t3401; })) {
#line 283 "src/racecheck.tr"
            return List_i64_get(f->ldepth, i);
        }
#line 284 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 285 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool RcCk_mentions(RcCk* self, Stmt* sp, TrStr name) {
#line 289 "src/racecheck.tr"
    RcFn* tmp = ({ TrStr _apt_t3402 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t3402, self->cur->line, true)); _tr_str_release(_apt_t3402); _wr; });
#line 290 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 291 "src/racecheck.tr"
    long long sl = self->cur_line;
#line 292 "src/racecheck.tr"
    bool ss = self->seen_spawn;
#line 293 "src/racecheck.tr"
    RcFn* _cltmp_t3403 = _tr_obj_retain(tmp);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3403;
#line 294 "src/racecheck.tr"
    RcCk_stmt(self, sp);
#line 295 "src/racecheck.tr"
    RcFn* _cltmp_t3404 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3404;
#line 296 "src/racecheck.tr"
    self->cur_line = sl;
#line 297 "src/racecheck.tr"
    self->seen_spawn = ss;
#line 298 "src/racecheck.tr"
    bool _trret593 = _rc_has(tmp->idents, name);
    _tr_obj_release(tmp, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
    return _trret593;
}

__attribute__((hot)) void RcCk_note(RcCk* self, TrStr name, bool w) {
#line 301 "src/racecheck.tr"
    if ((_rc_has(self->cur->locals, name) || _rc_has(self->cur->params, name))) {
#line 301 "src/racecheck.tr"
        return;
    }
#line 302 "src/racecheck.tr"
    List_TrStr_append(self->cur->touched, name);
#line 303 "src/racecheck.tr"
    List_bool_append(self->cur->touched_w, w);
#line 304 "src/racecheck.tr"
    List_bool_append(self->cur->touched_after, self->seen_spawn);
#line 305 "src/racecheck.tr"
    List_i64_append(self->cur->touched_line, self->cur_line);
}

__attribute__((hot)) void RcCk_wpath(RcCk* self, Expr* target) {
#line 308 "src/racecheck.tr"
    TrStr r = _rc_root(target);
#line 309 "src/racecheck.tr"
    if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 309 "src/racecheck.tr"
        _tr_str_release(r);
        return;
    }
#line 310 "src/racecheck.tr"
    List_TrStr_append(self->cur->wroot, r);
#line 311 "src/racecheck.tr"
    ({ TrStr _ord_t3405 = (_rc_first_prop(target)); List_TrStr_append(self->cur->wprop, _ord_t3405); _tr_str_release(_ord_t3405); });
#line 312 "src/racecheck.tr"
    List_bool_append(self->cur->wafter, self->seen_spawn);
#line 313 "src/racecheck.tr"
    RcCk_note(self, r, true);
    _tr_str_release(r);
}

__attribute__((hot)) void RcCk_block(RcCk* self, Block* b) {
#line 316 "src/racecheck.tr"
    long long i = 0LL;
#line 317 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 318 "src/racecheck.tr"
        RcCk_stmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
#line 319 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 320 "src/racecheck.tr"
    RcCk_lock_scan(self, b);
}

__attribute__((hot)) void RcCk_pblock(RcCk* self, Block** pb) {
#line 323 "src/racecheck.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 323 "src/racecheck.tr"
        return;
    }
#line 324 "src/racecheck.tr"
    RcCk_block(self, (*pb));
}

__attribute__((hot)) void RcCk_declare(RcCk* self, TrStr name) {
#line 327 "src/racecheck.tr"
    List_TrStr_append(self->cur->locals, name);
}

__attribute__((hot)) void RcCk_inherit_held(RcCk* self, Stmt* sp, List_TrStr* held) {
#line 333 "src/racecheck.tr"
    __auto_type _t3406 = (*sp);
    if (_t3406.tag == Stmt_SIf) {
        __auto_type tb = _t3406.data.SIf.then_b;
__auto_type elifs = _t3406.data.SIf.elifs;
__auto_type eb = _t3406.data.SIf.else_b;
#line 335 "src/racecheck.tr"
        RcCk_rescan_held(self, tb, held);
#line 336 "src/racecheck.tr"
        RcCk_rescan_held(self, eb, held);
#line 337 "src/racecheck.tr"
        long long ei = 0LL;
#line 338 "src/racecheck.tr"
        while ((ei < elifs->len)) {
#line 339 "src/racecheck.tr"
            if ((((unsigned long long)(((ElifClause*)List_ptr_get(elifs, ei))->body)) != ((unsigned long long)(0LL)))) {
#line 339 "src/racecheck.tr"
                RcCk_rescan_held(self, (*((ElifClause*)List_ptr_get(elifs, ei))->body), held);
            }
#line 340 "src/racecheck.tr"
            ei = (ei + 1LL);
        }
    } else if (_t3406.tag == Stmt_SWhile) {
        __auto_type wb = _t3406.data.SWhile.body;
        RcCk_rescan_held(self, wb, held);
    } else if (_t3406.tag == Stmt_SFor) {
        __auto_type fb = _t3406.data.SFor.body;
        RcCk_rescan_held(self, fb, held);
    } else if (_t3406.tag == Stmt_SForUnpack) {
        __auto_type fub = _t3406.data.SForUnpack.body;
        RcCk_rescan_held(self, fub, held);
    } else if (_t3406.tag == Stmt_SUnsafe) {
        __auto_type ub = _t3406.data.SUnsafe.body;
        RcCk_rescan_held(self, ub, held);
    } else if (_t3406.tag == Stmt_SWith) {
        __auto_type wb2 = _t3406.data.SWith.body;
        RcCk_rescan_held(self, wb2, held);
    } else if (_t3406.tag == Stmt_STry) {
        __auto_type tb2 = _t3406.data.STry.try_body;
        RcCk_rescan_held(self, tb2, held);
    } else if (1) {
        __auto_type _ = _t3406;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_rescan_held(RcCk* self, Block* nb, List_TrStr* held) {
#line 350 "src/racecheck.tr"
    TrStr key = _tr_str_wrap(_tr_uint_to_str((unsigned long long)(((unsigned long long)(nb)))));
#line 351 "src/racecheck.tr"
    _tr_dict_set(self->held_for, _tr_strz(key), held);
#line 352 "src/racecheck.tr"
    long long n0 = self->cur->msgs->len;
#line 353 "src/racecheck.tr"
    RcCk_lock_scan(self, nb);
#line 354 "src/racecheck.tr"
    while ((self->cur->msgs->len > n0)) {
#line 354 "src/racecheck.tr"
        List_TrStr_pop(self->cur->msgs);
    }
#line 355 "src/racecheck.tr"
    _tr_dict_remove(self->held_for, _tr_strz(key));
    _tr_str_release(key);
}

__attribute__((hot)) void RcCk_lock_scan(RcCk* self, Block* b) {
#line 358 "src/racecheck.tr"
    List_TrStr* names = (void*)List_TrStr_new();
#line 359 "src/racecheck.tr"
    List_i64* state = (void*)List_i64_new();
#line 360 "src/racecheck.tr"
    List_TrStr* kinds = (void*)List_TrStr_new();
#line 361 "src/racecheck.tr"
    TrStr bkey = _tr_str_wrap(_tr_uint_to_str((unsigned long long)(((unsigned long long)(b)))));
#line 362 "src/racecheck.tr"
    if (_tr_dict_contains(self->held_for, _tr_strz(bkey))) {
#line 363 "src/racecheck.tr"
        List_TrStr* hv = ((List_TrStr*)(uintptr_t)_tr_dict_get(self->held_for, _tr_strz(bkey)));
#line 364 "src/racecheck.tr"
        long long hk = 0LL;
#line 365 "src/racecheck.tr"
        while ((hk < hv->len)) {
#line 366 "src/racecheck.tr"
            ({ TrStr _at_t3407 = (List_TrStr_get(hv, hk)); List_TrStr_append(names, _at_t3407); _tr_str_release(_at_t3407); });
#line 367 "src/racecheck.tr"
            List_i64_append(state, 1LL);
#line 368 "src/racecheck.tr"
            List_TrStr_append(kinds, _tr_str_lit_len("Mutex", 5LL));
#line 369 "src/racecheck.tr"
            hk = (hk + 1LL);
        }
    }
#line 370 "src/racecheck.tr"
    List_TrStr* guards = (void*)List_TrStr_new();
#line 371 "src/racecheck.tr"
    List_TrStr* gmx = (void*)List_TrStr_new();
#line 372 "src/racecheck.tr"
    List_bool* grel = (void*)List_bool_new();
#line 373 "src/racecheck.tr"
    long long line = self->cur->line;
#line 374 "src/racecheck.tr"
    long long i = 0LL;
#line 375 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 376 "src/racecheck.tr"
        Stmt* sp = ((Stmt*)List_ptr_get(b->stmts, i));
#line 377 "src/racecheck.tr"
        i = (i + 1LL);
#line 378 "src/racecheck.tr"
        Expr* call = (Expr*)(0LL);
#line 379 "src/racecheck.tr"
        bool simple = true;
#line 380 "src/racecheck.tr"
        bool is_line = false;
#line 381 "src/racecheck.tr"
        TrStr letname = _tr_str_lit_len("", 0LL);
#line 382 "src/racecheck.tr"
        __auto_type _t3408 = (*sp);
        if (_t3408.tag == Stmt_SLine) {
            __auto_type n = _t3408.data.SLine.n;
#line 384 "src/racecheck.tr"
            line = n;
#line 385 "src/racecheck.tr"
            is_line = true;
        } else if (_t3408.tag == Stmt_SExpr) {
            __auto_type e = _t3408.data.SExpr.expr;
            call = e;
        } else if (_t3408.tag == Stmt_SLet) {
            __auto_type lnm = _t3408.data.SLet.name;
__auto_type v = _t3408.data.SLet.val;
#line 388 "src/racecheck.tr"
            call = v;
#line 389 "src/racecheck.tr"
            TrStr _strtmp_t3409 = _tr_str_retain(lnm);
            _tr_str_release(letname);
            letname = _strtmp_t3409;
        } else if (_t3408.tag == Stmt_SAssign) {
            __auto_type tg2 = _t3408.data.SAssign.target;
__auto_type v2 = _t3408.data.SAssign.val;
#line 391 "src/racecheck.tr"
            call = v2;
#line 393 "src/racecheck.tr"
            if ((!_rc_nul(tg2))) {
#line 394 "src/racecheck.tr"
                __auto_type _t3410 = (*tg2);
                if (_t3410.tag == Expr_EIdent) {
                    __auto_type tgn = _t3410.data.EIdent.name;
#line 396 "src/racecheck.tr"
                    if ((!_rc_has(self->cur->params, tgn))) {
#line 396 "src/racecheck.tr"
                        TrStr _strtmp_t3411 = _tr_str_retain(tgn);
                        _tr_str_release(letname);
                        letname = _strtmp_t3411;
                    }
                } else if (1) {
                    __auto_type _ = _t3410;
                    /* pass */
                }
            }
        } else if (1) {
            __auto_type _ = _t3408;
            simple = false;
        }
#line 399 "src/racecheck.tr"
        if (is_line) {
#line 399 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 400 "src/racecheck.tr"
        long long gk = 0LL;
#line 401 "src/racecheck.tr"
        while ((gk < guards->len)) {
#line 402 "src/racecheck.tr"
            if (((List_bool_get(grel, gk) && ({ TrStr _cmpa_t3412 = (List_TrStr_get(guards, gk)); TrStr _cmpb_t3413 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3414 = !_tr_str_eqv(_cmpa_t3412, _cmpb_t3413); _tr_str_release(_cmpa_t3412); _cmpr_t3414; })) && ({ TrStr _at_t3415 = (List_TrStr_get(guards, gk)); __auto_type _wr = (RcCk_mentions(self, sp, _at_t3415)); _tr_str_release(_at_t3415); _wr; }))) {
#line 403 "src/racecheck.tr"
                ({ __auto_type _ord_t3416 = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3417 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3416); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was obtained from `", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3418 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3417); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()` and is used after the lock was released: the guarded object is then reachable by other threads while this one still reads/writes it (a data race).\n      FIX: keep every use of '", 185LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3419 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3418); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before `", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3420 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3419); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` / `", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3421 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3420); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(..)`, or copy the data out while the lock is held.", 55LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3421); _tr_str_release(_ord_t3421); });
#line 404 "src/racecheck.tr"
                List_TrStr_set(guards, gk, _tr_str_lit_len("", 0LL));
            }
#line 405 "src/racecheck.tr"
            gk = (gk + 1LL);
        }
#line 406 "src/racecheck.tr"
        if ((!simple)) {
#line 409 "src/racecheck.tr"
            List_TrStr* heldv = (void*)List_TrStr_new();
#line 410 "src/racecheck.tr"
            long long hj = 0LL;
#line 411 "src/racecheck.tr"
            while ((hj < names->len)) {
#line 412 "src/racecheck.tr"
                if ((({ TrStr _cmpa_t3422 = (List_TrStr_get(kinds, hj)); TrStr _cmpb_t3423 = (_tr_str_lit_len("Mutex", 5LL)); _Bool _cmpr_t3424 = _tr_str_eqv(_cmpa_t3422, _cmpb_t3423); _tr_str_release(_cmpa_t3422); _cmpr_t3424; }) && (List_i64_get(state, hj) == 1LL))) {
#line 412 "src/racecheck.tr"
                    ({ TrStr _at_t3425 = (List_TrStr_get(names, hj)); List_TrStr_append(heldv, _at_t3425); _tr_str_release(_at_t3425); });
                }
#line 413 "src/racecheck.tr"
                hj = (hj + 1LL);
            }
#line 414 "src/racecheck.tr"
            if ((heldv->len > 0LL)) {
#line 414 "src/racecheck.tr"
                RcCk_inherit_held(self, sp, heldv);
            }
#line 415 "src/racecheck.tr"
            names = (void*)List_TrStr_new();
#line 416 "src/racecheck.tr"
            state = (void*)List_i64_new();
#line 417 "src/racecheck.tr"
            kinds = (void*)List_TrStr_new();
#line 418 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 419 "src/racecheck.tr"
        if (_rc_nul(call)) {
#line 419 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 420 "src/racecheck.tr"
        __auto_type _t3426 = (*call);
        if (_t3426.tag == Expr_EMethodCall) {
            __auto_type o = _t3426.data.EMethodCall.obj;
__auto_type m = _t3426.data.EMethodCall.method;
__auto_type margs = _t3426.data.EMethodCall.args;
#line 422 "src/racecheck.tr"
            TrStr r = _tr_str_lit_len("", 0LL);
#line 423 "src/racecheck.tr"
            if ((!_rc_nul(o))) {
#line 424 "src/racecheck.tr"
                __auto_type _t3427 = (*o);
                if (_t3427.tag == Expr_EIdent) {
                    __auto_type rn = _t3427.data.EIdent.name;
                    TrStr _strtmp_t3428 = _tr_str_retain(rn);
                    _tr_str_release(r);
                    r = _strtmp_t3428;
                } else if (1) {
                    __auto_type _ = _t3427;
                    /* pass */
                }
            }
#line 427 "src/racecheck.tr"
            if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 427 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(letname);
                continue;
            }
#line 428 "src/racecheck.tr"
            TrStr k = RcCk_ltype(self, self->cur, r);
#line 429 "src/racecheck.tr"
            if (((_tr_str_eqv((k), (_tr_str_lit_len("", 0LL))) && (!_rc_has(self->cur->locals, r))) && (!_rc_has(self->cur->params, r)))) {
#line 430 "src/racecheck.tr"
                long long gq = 0LL;
#line 431 "src/racecheck.tr"
                while ((gq < self->gnames->len)) {
#line 432 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3429 = (List_TrStr_get(self->gnames, gq)); TrStr _cmpb_t3430 = (r); _Bool _cmpr_t3431 = _tr_str_eqv(_cmpa_t3429, _cmpb_t3430); _tr_str_release(_cmpa_t3429); _cmpr_t3431; })) {
#line 432 "src/racecheck.tr"
                        TrStr _strtmp_t3432 = List_TrStr_get(self->gtn, gq);
                        _tr_str_release(k);
                        k = _strtmp_t3432;
                    }
#line 433 "src/racecheck.tr"
                    gq = (gq + 1LL);
                }
            }
#line 434 "src/racecheck.tr"
            if ((!((_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))))) {
#line 434 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                _tr_str_release(letname);
                continue;
            }
#line 435 "src/racecheck.tr"
            if ((((margs->len != 0LL) && (!_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) && (!_tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL)))))) {
#line 435 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                _tr_str_release(letname);
                continue;
            }
#line 436 "src/racecheck.tr"
            long long idx = (0LL - 1LL);
#line 437 "src/racecheck.tr"
            long long j = 0LL;
#line 438 "src/racecheck.tr"
            while ((j < names->len)) {
#line 439 "src/racecheck.tr"
                if (({ TrStr _cmpa_t3433 = (List_TrStr_get(names, j)); TrStr _cmpb_t3434 = (r); _Bool _cmpr_t3435 = _tr_str_eqv(_cmpa_t3433, _cmpb_t3434); _tr_str_release(_cmpa_t3433); _cmpr_t3435; })) {
#line 439 "src/racecheck.tr"
                    idx = j;
                }
#line 440 "src/racecheck.tr"
                j = (j + 1LL);
            }
#line 441 "src/racecheck.tr"
            if ((idx < 0LL)) {
#line 442 "src/racecheck.tr"
                List_TrStr_append(names, r);
#line 443 "src/racecheck.tr"
                List_i64_append(state, 0LL);
#line 444 "src/racecheck.tr"
                List_TrStr_append(kinds, k);
#line 445 "src/racecheck.tr"
                idx = (names->len - 1LL);
            }
#line 446 "src/racecheck.tr"
            long long st = List_i64_get(state, idx);
#line 447 "src/racecheck.tr"
            if (_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 448 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) {
#line 449 "src/racecheck.tr"
                    long long gj2 = 0LL;
#line 450 "src/racecheck.tr"
                    while ((gj2 < names->len)) {
#line 451 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3436 = (List_TrStr_get(kinds, gj2)); TrStr _cmpb_t3437 = (_tr_str_lit_len("Mutex", 5LL)); _Bool _cmpr_t3438 = _tr_str_eqv(_cmpa_t3436, _cmpb_t3437); _tr_str_release(_cmpa_t3436); _cmpr_t3438; }) && (List_i64_get(state, gj2) == 1LL)) && ({ TrStr _cmpa_t3439 = (List_TrStr_get(names, gj2)); TrStr _cmpb_t3440 = (r); _Bool _cmpr_t3441 = !_tr_str_eqv(_cmpa_t3439, _cmpb_t3440); _tr_str_release(_cmpa_t3439); _cmpr_t3441; }))) {
#line 452 "src/racecheck.tr"
                            if ((!(((_rc_has(self->cur->locals, r) || _rc_has(self->cur->params, r)) || ({ __auto_type _ord_t3443 = (self->cur->locals); TrStr _at_t3442 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3443, _at_t3442)); _tr_str_release(_at_t3442); _wr; })) || ({ __auto_type _ord_t3445 = (self->cur->params); TrStr _at_t3444 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3445, _at_t3444)); _tr_str_release(_at_t3444); _wr; })))) {
#line 453 "src/racecheck.tr"
                                bool pdup = false;
#line 454 "src/racecheck.tr"
                                long long pdk = 0LL;
#line 455 "src/racecheck.tr"
                                while ((pdk < self->cur->pairs_a->len)) {
#line 456 "src/racecheck.tr"
                                    if ((({ __auto_type _ord_t3446 = (List_TrStr_get(self->cur->pairs_a, pdk)); __auto_type _wr = (({ TrStr _cmpa_t3447 = (_ord_t3446); TrStr _cmpb_t3448 = (List_TrStr_get(names, gj2)); _Bool _cmpr_t3449 = _tr_str_eqv(_cmpa_t3447, _cmpb_t3448); _tr_str_release(_cmpa_t3447); _tr_str_release(_cmpb_t3448); _cmpr_t3449; })); _wr; }) && ({ TrStr _cmpa_t3450 = (List_TrStr_get(self->cur->pairs_b, pdk)); TrStr _cmpb_t3451 = (r); _Bool _cmpr_t3452 = _tr_str_eqv(_cmpa_t3450, _cmpb_t3451); _tr_str_release(_cmpa_t3450); _cmpr_t3452; }))) {
#line 456 "src/racecheck.tr"
                                        pdup = true;
                                    }
#line 457 "src/racecheck.tr"
                                    pdk = (pdk + 1LL);
                                }
#line 458 "src/racecheck.tr"
                                if ((!pdup)) {
#line 459 "src/racecheck.tr"
                                    ({ TrStr _ord_t3453 = (List_TrStr_get(names, gj2)); List_TrStr_append(self->cur->pairs_a, _ord_t3453); _tr_str_release(_ord_t3453); });
#line 460 "src/racecheck.tr"
                                    List_TrStr_append(self->cur->pairs_b, r);
                                }
                            }
                        }
#line 461 "src/racecheck.tr"
                        gj2 = (gj2 + 1LL);
                    }
#line 462 "src/racecheck.tr"
                    TrStr pa = RcCk_ltarg_of(self, self->cur, r);
#line 463 "src/racecheck.tr"
                    long long ci0 = RcCk_find_class(self, pa);
#line 464 "src/racecheck.tr"
                    if (((!_tr_str_eqv((letname), (_tr_str_lit_len("", 0LL)))) && (((((_tr_str_eqv((pa), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((pa), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Map", 3LL)))) || ((ci0 >= 0LL) && (!List_bool_get(self->class_lib, ci0)))))) {
#line 465 "src/racecheck.tr"
                        List_TrStr_append(guards, letname);
#line 466 "src/racecheck.tr"
                        List_TrStr_append(gmx, r);
#line 467 "src/racecheck.tr"
                        List_bool_append(grel, false);
                    }
#line 468 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 468 "src/racecheck.tr"
                        ({ TrStr _ord_t3454 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()' acquires the lock again while this thread already holds it (no `set()`/`unlock()` since the previous `get()`): the second call blocks forever.\n      FIX: finish with `", 175LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(v)` or `", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` before locking again, or reuse the first value.", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3454); _tr_str_release(_ord_t3454); });
                    }
#line 469 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("unlock", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) {
#line 471 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
#line 472 "src/racecheck.tr"
                    long long gr = 0LL;
#line 473 "src/racecheck.tr"
                    while ((gr < guards->len)) {
#line 474 "src/racecheck.tr"
                        if (({ TrStr _cmpa_t3455 = (List_TrStr_get(gmx, gr)); TrStr _cmpb_t3456 = (r); _Bool _cmpr_t3457 = _tr_str_eqv(_cmpa_t3455, _cmpb_t3456); _tr_str_release(_cmpa_t3455); _cmpr_t3457; })) {
#line 474 "src/racecheck.tr"
                            List_bool_set(grel, gr, true);
                        }
#line 475 "src/racecheck.tr"
                        gr = (gr + 1LL);
                    }
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 477 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL)))) {
#line 477 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("read_unlock", 11LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL))))) {
#line 478 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
                } else if (_tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) {
#line 480 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 480 "src/racecheck.tr"
                        ({ TrStr _ord_t3458 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()' takes the write lock while this thread still holds the read lock from `", 81LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read()`: the write waits for readers (itself included) to leave, which never happens (deadlock).\n      FIX: call `", 115LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read_unlock()` before `", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()`.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3458); _tr_str_release(_ord_t3458); });
                    }
#line 481 "src/racecheck.tr"
                    List_i64_set(state, idx, 2LL);
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))) {
#line 483 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL)))) {
#line 484 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 484 "src/racecheck.tr"
                        ({ TrStr _ord_t3459 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] thread '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is joined twice: joining an already-joined thread handle is undefined behaviour.\n      FIX: join each thread exactly once.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3459); _tr_str_release(_ord_t3459); });
                    }
#line 485 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                }
            }
            _tr_str_release(r);
            _tr_str_release(k);
        } else if (1) {
            __auto_type _ = _t3426;
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
#line 489 "src/racecheck.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 489 "src/racecheck.tr"
        return;
    }
#line 490 "src/racecheck.tr"
    __auto_type _t3460 = (*sp);
    if (_t3460.tag == Stmt_SLine) {
        __auto_type n = _t3460.data.SLine.n;
        self->cur_line = n;
    } else if (_t3460.tag == Stmt_SExpr) {
        __auto_type e = _t3460.data.SExpr.expr;
        RcCk_expr(self, e);
    } else if (_t3460.tag == Stmt_SLet) {
        __auto_type name = _t3460.data.SLet.name;
__auto_type is_sh = _t3460.data.SLet.is_shared;
__auto_type ty = _t3460.data.SLet.ty;
__auto_type v = _t3460.data.SLet.val;
#line 494 "src/racecheck.tr"
        ({ TrStr _apt_t3461 = _tr_str_retain(self->cur->file); RcCk_ty_check(self, ty, _apt_t3461, self->cur_line); _tr_str_release(_apt_t3461); });
#line 495 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 496 "src/racecheck.tr"
        RcCk_declare(self, name);
#line 497 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 498 "src/racecheck.tr"
        if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 498 "src/racecheck.tr"
            TrStr _strtmp_t3462 = _tr_str_retain((*ty)->name);
            _tr_str_release(tn);
            tn = _strtmp_t3462;
        }
#line 499 "src/racecheck.tr"
        if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 499 "src/racecheck.tr"
            TrStr _strtmp_t3463 = _rc_ctor_name(v);
            _tr_str_release(tn);
            tn = _strtmp_t3463;
        }
#line 500 "src/racecheck.tr"
        TrStr targ = _rc_ctor_arg(v);
#line 501 "src/racecheck.tr"
        if (((((unsigned long long)(ty)) != ((unsigned long long)(0LL))) && ((*ty)->args->len > 0LL))) {
#line 501 "src/racecheck.tr"
            TrStr _strtmp_t3464 = _tr_str_retain((*((AstType**)List_ptr_get((*ty)->args, 0LL)))->name);
            _tr_str_release(targ);
            targ = _strtmp_t3464;
        }
#line 502 "src/racecheck.tr"
        if ((is_sh && (!_rc_nul(v)))) {
#line 503 "src/racecheck.tr"
            __auto_type _t3465 = (*v);
            if (_t3465.tag == Expr_EIdent) {
                __auto_type src = _t3465.data.EIdent.name;
#line 505 "src/racecheck.tr"
                TrStr _strtmp_t3466 = RcCk_ltype(self, self->cur, src);
                _tr_str_release(tn);
                tn = _strtmp_t3466;
#line 506 "src/racecheck.tr"
                TrStr _strtmp_t3467 = RcCk_ltarg_of(self, self->cur, src);
                _tr_str_release(targ);
                targ = _strtmp_t3467;
            } else if (1) {
                __auto_type _ = _t3465;
                /* pass */
            }
        }
#line 508 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltn, name);
#line 509 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltt, tn);
#line 510 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltarg, targ);
#line 511 "src/racecheck.tr"
        List_i64_append(self->cur->ldepth, self->loop_depth);
#line 512 "src/racecheck.tr"
        if (_rc_is_thread_spawn(v)) {
#line 513 "src/racecheck.tr"
            List_TrStr_append(self->cur->thr_made, name);
#line 514 "src/racecheck.tr"
            List_i64_append(self->cur->thr_line, self->cur_line);
        }
#line 515 "src/racecheck.tr"
        if (is_sh) {
#line 515 "src/racecheck.tr"
            List_TrStr_append(self->cur->shvars, name);
        }
#line 516 "src/racecheck.tr"
        if (_rc_is_lambda(v)) {
#line 516 "src/racecheck.tr"
            List_TrStr_append(self->cur->closures, name);
        }
        _tr_str_release(tn);
        _tr_str_release(targ);
    } else if (_t3460.tag == Stmt_SMultiLet) {
        __auto_type names = _t3460.data.SMultiLet.names;
__auto_type v = _t3460.data.SMultiLet.val;
#line 518 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 519 "src/racecheck.tr"
        long long mi = 0LL;
#line 520 "src/racecheck.tr"
        while ((mi < names->len)) {
#line 521 "src/racecheck.tr"
            ({ TrStr _at_t3468 = (List_TrStr_get(names, mi)); RcCk_declare(self, _at_t3468); _tr_str_release(_at_t3468); });
#line 522 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t3460.tag == Stmt_SAssign) {
        __auto_type t = _t3460.data.SAssign.target;
__auto_type v = _t3460.data.SAssign.val;
#line 524 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 525 "src/racecheck.tr"
        RcCk_expr(self, t);
#line 526 "src/racecheck.tr"
        RcCk_wpath(self, t);
#line 528 "src/racecheck.tr"
        if ((!_rc_nul(t))) {
#line 529 "src/racecheck.tr"
            __auto_type _t3469 = (*t);
            if (_t3469.tag == Expr_EIdent) {
                __auto_type bn = _t3469.data.EIdent.name;
#line 531 "src/racecheck.tr"
                if ((({ TrStr _cmpa_t3470 = (RcCk_ltype(self, self->cur, bn)); TrStr _cmpb_t3471 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3472 = _tr_str_eqv(_cmpa_t3470, _cmpb_t3471); _tr_str_release(_cmpa_t3470); _cmpr_t3472; }) && (!_rc_has(self->cur->params, bn)))) {
#line 532 "src/racecheck.tr"
                    List_TrStr_append(self->cur->ltn, bn);
#line 533 "src/racecheck.tr"
                    ({ TrStr _ord_t3473 = (_rc_ctor_name(v)); List_TrStr_append(self->cur->ltt, _ord_t3473); _tr_str_release(_ord_t3473); });
#line 534 "src/racecheck.tr"
                    ({ TrStr _ord_t3474 = (_rc_ctor_arg(v)); List_TrStr_append(self->cur->ltarg, _ord_t3474); _tr_str_release(_ord_t3474); });
#line 535 "src/racecheck.tr"
                    List_i64_append(self->cur->ldepth, self->loop_depth);
                }
            } else if (1) {
                __auto_type _ = _t3469;
                /* pass */
            }
        }
    } else if (_t3460.tag == Stmt_SReturn) {
        __auto_type v = _t3460.data.SReturn.val;
        RcCk_expr(self, v);
    } else if (_t3460.tag == Stmt_SBreak) {
        __auto_type v = _t3460.data.SBreak.val;
        RcCk_expr(self, v);
    } else if (_t3460.tag == Stmt_SRaise) {
        __auto_type v = _t3460.data.SRaise.val;
        RcCk_expr(self, v);
    } else if (_t3460.tag == Stmt_SUnsafe) {
        __auto_type b = _t3460.data.SUnsafe.body;
        RcCk_block(self, b);
    } else if (_t3460.tag == Stmt_SIf) {
        __auto_type c = _t3460.data.SIf.cond;
__auto_type tb = _t3460.data.SIf.then_b;
__auto_type elifs = _t3460.data.SIf.elifs;
__auto_type eb = _t3460.data.SIf.else_b;
#line 542 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 543 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 544 "src/racecheck.tr"
        long long ei = 0LL;
#line 545 "src/racecheck.tr"
        while ((ei < elifs->len)) {
#line 546 "src/racecheck.tr"
            RcCk_expr(self, ((ElifClause*)List_ptr_get(elifs, ei))->cond);
#line 547 "src/racecheck.tr"
            RcCk_pblock(self, ((ElifClause*)List_ptr_get(elifs, ei))->body);
#line 548 "src/racecheck.tr"
            ei = (ei + 1LL);
        }
#line 549 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (_t3460.tag == Stmt_SWhile) {
        __auto_type c = _t3460.data.SWhile.cond;
__auto_type b = _t3460.data.SWhile.body;
#line 551 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 552 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 553 "src/racecheck.tr"
        RcCk_block(self, b);
#line 554 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3460.tag == Stmt_SFor) {
        __auto_type v = _t3460.data.SFor.var;
__auto_type it = _t3460.data.SFor.iter;
__auto_type b = _t3460.data.SFor.body;
#line 556 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 557 "src/racecheck.tr"
        RcCk_declare(self, v);
#line 558 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 559 "src/racecheck.tr"
        RcCk_block(self, b);
#line 560 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3460.tag == Stmt_SForUnpack) {
        __auto_type vs = _t3460.data.SForUnpack.vars;
__auto_type it = _t3460.data.SForUnpack.iter;
__auto_type b = _t3460.data.SForUnpack.body;
#line 562 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 563 "src/racecheck.tr"
        long long fi = 0LL;
#line 564 "src/racecheck.tr"
        while ((fi < vs->len)) {
#line 565 "src/racecheck.tr"
            ({ TrStr _at_t3475 = (List_TrStr_get(vs, fi)); RcCk_declare(self, _at_t3475); _tr_str_release(_at_t3475); });
#line 566 "src/racecheck.tr"
            fi = (fi + 1LL);
        }
#line 567 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 568 "src/racecheck.tr"
        RcCk_block(self, b);
#line 569 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3460.tag == Stmt_SMatch) {
        __auto_type e = _t3460.data.SMatch.expr;
__auto_type arms = _t3460.data.SMatch.arms;
#line 571 "src/racecheck.tr"
        RcCk_expr(self, e);
#line 572 "src/racecheck.tr"
        long long ai = 0LL;
#line 573 "src/racecheck.tr"
        while ((ai < arms->len)) {
#line 574 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, ai))->guard);
#line 575 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, ai))->body);
#line 576 "src/racecheck.tr"
            ai = (ai + 1LL);
        }
    } else if (_t3460.tag == Stmt_STry) {
        __auto_type tb = _t3460.data.STry.try_body;
__auto_type catches = _t3460.data.STry.catches;
__auto_type fb = _t3460.data.STry.finally_b;
#line 578 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 579 "src/racecheck.tr"
        long long ci = 0LL;
#line 580 "src/racecheck.tr"
        while ((ci < catches->len)) {
#line 581 "src/racecheck.tr"
            ({ TrStr _apt_t3476 = _tr_str_retain((*((CatchClause**)List_ptr_get(catches, ci)))->err_name); RcCk_declare(self, _apt_t3476); _tr_str_release(_apt_t3476); });
#line 582 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci)))->body);
#line 583 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 584 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t3460.tag == Stmt_SAssert) {
        __auto_type c = _t3460.data.SAssert.cond;
__auto_type m = _t3460.data.SAssert.msg;
#line 586 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 587 "src/racecheck.tr"
        RcCk_expr(self, m);
    } else if (_t3460.tag == Stmt_SWith) {
        __auto_type items = _t3460.data.SWith.items;
__auto_type aliases = _t3460.data.SWith.aliases;
__auto_type b = _t3460.data.SWith.body;
#line 589 "src/racecheck.tr"
        long long wi = 0LL;
#line 590 "src/racecheck.tr"
        while ((wi < items->len)) {
#line 591 "src/racecheck.tr"
            RcCk_expr(self, ((Expr*)List_ptr_get(items, wi)));
#line 592 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 593 "src/racecheck.tr"
        wi = 0LL;
#line 594 "src/racecheck.tr"
        while ((wi < aliases->len)) {
#line 595 "src/racecheck.tr"
            ({ TrStr _at_t3477 = (List_TrStr_get(aliases, wi)); RcCk_declare(self, _at_t3477); _tr_str_release(_at_t3477); });
#line 596 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 597 "src/racecheck.tr"
        RcCk_block(self, b);
    } else if (_t3460.tag == Stmt_SSpawn) {
        __auto_type e = _t3460.data.SSpawn.expr;
#line 599 "src/racecheck.tr"
        RcCk_spawn_expr(self, e);
    } else if (_t3460.tag == Stmt_STaskGroup) {
        __auto_type b = _t3460.data.STaskGroup.body;
#line 601 "src/racecheck.tr"
        RcCk_block(self, b);
#line 602 "src/racecheck.tr"
        self->seen_spawn = false;
    } else if (_t3460.tag == Stmt_SGpuBlock) {
        __auto_type b = _t3460.data.SGpuBlock.body;
        RcCk_block(self, b);
    } else if (_t3460.tag == Stmt_SDefer) {
        __auto_type s = _t3460.data.SDefer.stmt;
        RcCk_stmt(self, s);
    } else if (_t3460.tag == Stmt_SLocalDecl) {
        __auto_type d = _t3460.data.SLocalDecl.decl;
#line 606 "src/racecheck.tr"
        __auto_type _t3478 = (*d);
        if (_t3478.tag == Decl_DFunction) {
            __auto_type f = _t3478.data.DFunction.func;
#line 608 "src/racecheck.tr"
            long long nidx = self->fns->len;
#line 609 "src/racecheck.tr"
            ({ TrStr _apt_t3479 = _tr_str_retain(self->cur->file); RcCk_add_fn(self, f, _apt_t3479, self->cur->is_lib); _tr_str_release(_apt_t3479); });
#line 610 "src/racecheck.tr"
            if (((nidx < self->fns->len) && (!self->cur->is_lib))) {
#line 611 "src/racecheck.tr"
                RcFn* nf = ((RcFn*)List_ptr_get(self->fns, nidx));
#line 612 "src/racecheck.tr"
                long long nk = 0LL;
#line 613 "src/racecheck.tr"
                while ((nk < nf->touched->len)) {
#line 614 "src/racecheck.tr"
                    TrStr nn = List_TrStr_get(nf->touched, nk);
#line 615 "src/racecheck.tr"
                    if ((_rc_has(self->cur->locals, nn) || _rc_has(self->cur->params, nn))) {
#line 616 "src/racecheck.tr"
                        ({ __auto_type _ord_t3480 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[N-3] the nested function '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3481 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3480); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the enclosing function '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3482 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3481); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->cur->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but a nested `def` does not capture the enclosing function's locals (it is an ordinary function).\n      FIX: pass '", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as a parameter, or use a `lambda` (which does capture), or move the value to module level.", 92LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3482); _tr_str_release(_ord_t3482); });
#line 617 "src/racecheck.tr"
                        _tr_str_release(nn);
                        break;
                    }
#line 618 "src/racecheck.tr"
                    nk = (nk + 1LL);
                    _tr_str_release(nn);
                }
            }
        } else if (1) {
            __auto_type _ = _t3478;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t3460;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_spawn_expr(RcCk* self, Expr* e) {
#line 624 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 624 "src/racecheck.tr"
        return;
    }
#line 625 "src/racecheck.tr"
    __auto_type _t3483 = (*e);
    if (_t3483.tag == Expr_ECall) {
        __auto_type c = _t3483.data.ECall.callee;
__auto_type args = _t3483.data.ECall.args;
#line 627 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 628 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 629 "src/racecheck.tr"
            __auto_type _t3484 = (*c);
            if (_t3484.tag == Expr_EIdent) {
                __auto_type n = _t3484.data.EIdent.name;
                TrStr _strtmp_t3485 = _tr_str_retain(n);
                _tr_str_release(tn);
                tn = _strtmp_t3485;
            } else if (1) {
                __auto_type _ = _t3484;
                /* pass */
            }
        }
#line 632 "src/racecheck.tr"
        RcCk_record_site(self, tn, args, 0LL);
#line 633 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(tn);
    } else if (_t3483.tag == Expr_EMethodCall) {
        __auto_type o = _t3483.data.EMethodCall.obj;
__auto_type m = _t3483.data.EMethodCall.method;
__auto_type margs = _t3483.data.EMethodCall.args;
#line 635 "src/racecheck.tr"
        TrStr r = _tr_str_lit_len("", 0LL);
#line 636 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 637 "src/racecheck.tr"
            __auto_type _t3486 = (*o);
            if (_t3486.tag == Expr_EIdent) {
                __auto_type rn = _t3486.data.EIdent.name;
                TrStr _strtmp_t3487 = _tr_str_retain(rn);
                _tr_str_release(r);
                r = _strtmp_t3487;
            } else if (1) {
                __auto_type _ = _t3486;
                /* pass */
            }
        }
#line 640 "src/racecheck.tr"
        if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 641 "src/racecheck.tr"
            RcCk_record_site(self, _tr_str_lit_len("", 0LL), margs, 0LL);
#line 642 "src/racecheck.tr"
            ((RcSite*)List_ptr_get(self->cur->sites, (self->cur->sites->len - 1LL)))->recv = _tr_str_retain(r);
#line 643 "src/racecheck.tr"
            ((RcSite*)List_ptr_get(self->cur->sites, (self->cur->sites->len - 1LL)))->meth = _tr_str_retain(m);
        }
#line 644 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(r);
    } else if (1) {
        __auto_type _ = _t3483;
        RcCk_expr(self, e);
    }
}

__attribute__((hot)) void RcCk_record_site(RcCk* self, TrStr target, List_ptr* args, long long skip) {
#line 648 "src/racecheck.tr"
    RcSite* s = ((RcSite*)_tr_obj_alloc(sizeof(RcSite)));
#line 649 "src/racecheck.tr"
    TrStr _sfo_t3488 = s->target;
    s->target = _tr_str_retain(target);
    _tr_str_release(_sfo_t3488);
#line 650 "src/racecheck.tr"
    s->args = (void*)List_ptr_new();
#line 651 "src/racecheck.tr"
    long long i = skip;
#line 652 "src/racecheck.tr"
    while ((i < args->len)) {
#line 653 "src/racecheck.tr"
        ({ __auto_type _ord_t3489 = (((Expr*)List_ptr_get(args, i))); List_ptr_append(s->args, _ord_t3489); });
#line 654 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 655 "src/racecheck.tr"
    s->line = self->cur_line;
#line 656 "src/racecheck.tr"
    s->in_loop = (self->loop_depth > 0LL);
#line 657 "src/racecheck.tr"
    s->depth = self->loop_depth;
#line 658 "src/racecheck.tr"
    TrStr _sfo_t3490 = s->recv;
    s->recv = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t3490);
#line 659 "src/racecheck.tr"
    TrStr _sfo_t3491 = s->meth;
    s->meth = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t3491);
#line 660 "src/racecheck.tr"
    List_ptr_append(self->cur->sites, _tr_obj_retain(s));
#line 661 "src/racecheck.tr"
    self->seen_spawn = true;
    _tr_obj_release(s, _trdrop_RcSite);
}

__attribute__((hot)) void RcCk_exprs(RcCk* self, List_ptr* v) {
#line 664 "src/racecheck.tr"
    long long i = 0LL;
#line 665 "src/racecheck.tr"
    while ((i < v->len)) {
#line 666 "src/racecheck.tr"
        RcCk_expr(self, ((Expr*)List_ptr_get(v, i)));
#line 667 "src/racecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void RcCk_comps(RcCk* self, Expr* element, List_ptr* gens) {
#line 670 "src/racecheck.tr"
    long long i = 0LL;
#line 671 "src/racecheck.tr"
    while ((i < gens->len)) {
#line 672 "src/racecheck.tr"
        ({ TrStr _apt_t3492 = _tr_str_retain((*((Comprehension**)List_ptr_get(gens, i)))->target); RcCk_declare(self, _apt_t3492); _tr_str_release(_apt_t3492); });
#line 673 "src/racecheck.tr"
        RcCk_expr(self, (*((Comprehension**)List_ptr_get(gens, i)))->iter);
#line 674 "src/racecheck.tr"
        RcCk_exprs(self, (*((Comprehension**)List_ptr_get(gens, i)))->ifs);
#line 675 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 676 "src/racecheck.tr"
    RcCk_expr(self, element);
}

__attribute__((hot)) void RcCk_expr(RcCk* self, Expr* e) {
#line 679 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 679 "src/racecheck.tr"
        return;
    }
#line 680 "src/racecheck.tr"
    __auto_type _t3493 = (*e);
    if (_t3493.tag == Expr_EIdent) {
        __auto_type n = _t3493.data.EIdent.name;
#line 682 "src/racecheck.tr"
        List_TrStr_append(self->cur->idents, n);
#line 683 "src/racecheck.tr"
        RcCk_note(self, n, false);
    } else if (_t3493.tag == Expr_EBinOp) {
        __auto_type l = _t3493.data.EBinOp.left;
__auto_type r = _t3493.data.EBinOp.right;
#line 685 "src/racecheck.tr"
        RcCk_expr(self, l);
#line 686 "src/racecheck.tr"
        RcCk_expr(self, r);
    } else if (_t3493.tag == Expr_EUnaryOp) {
        __auto_type x = _t3493.data.EUnaryOp.expr;
        RcCk_expr(self, x);
    } else if (_t3493.tag == Expr_ECall) {
        __auto_type c = _t3493.data.ECall.callee;
__auto_type args = _t3493.data.ECall.args;
#line 689 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 690 "src/racecheck.tr"
            __auto_type _t3494 = (*c);
            if (_t3494.tag == Expr_EIdent) {
                __auto_type fnm = _t3494.data.EIdent.name;
#line 692 "src/racecheck.tr"
                List_TrStr_append(self->cur->calls, fnm);
#line 693 "src/racecheck.tr"
                long long cai = 0LL;
#line 694 "src/racecheck.tr"
                while ((cai < args->len)) {
#line 695 "src/racecheck.tr"
                    __auto_type _t3495 = (*((Expr*)List_ptr_get(args, cai)));
                    if (_t3495.tag == Expr_EIdent) {
                        __auto_type can = _t3495.data.EIdent.name;
#line 697 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_fn, fnm);
#line 698 "src/racecheck.tr"
                        List_i64_append(self->cur->ca_idx, cai);
#line 699 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_name, can);
                    } else if (1) {
                        __auto_type _ = _t3495;
                        /* pass */
                    }
#line 701 "src/racecheck.tr"
                    cai = (cai + 1LL);
                }
#line 702 "src/racecheck.tr"
                if (_tr_str_eqv((fnm), (_tr_str_lit_len("await_all", 9LL)))) {
#line 703 "src/racecheck.tr"
                    long long ai = 0LL;
#line 704 "src/racecheck.tr"
                    while ((ai < args->len)) {
#line 705 "src/racecheck.tr"
                        __auto_type _t3496 = (*((Expr*)List_ptr_get(args, ai)));
                        if (_t3496.tag == Expr_ECall) {
                            __auto_type c2 = _t3496.data.ECall.callee;
__auto_type a2 = _t3496.data.ECall.args;
#line 707 "src/racecheck.tr"
                            TrStr t2 = _tr_str_lit_len("", 0LL);
#line 708 "src/racecheck.tr"
                            if ((!_rc_nul(c2))) {
#line 709 "src/racecheck.tr"
                                __auto_type _t3497 = (*c2);
                                if (_t3497.tag == Expr_EIdent) {
                                    __auto_type n2 = _t3497.data.EIdent.name;
                                    TrStr _strtmp_t3498 = _tr_str_retain(n2);
                                    _tr_str_release(t2);
                                    t2 = _strtmp_t3498;
                                } else if (1) {
                                    __auto_type _ = _t3497;
                                    /* pass */
                                }
                            }
#line 712 "src/racecheck.tr"
                            RcCk_record_site(self, t2, a2, 0LL);
                            _tr_str_release(t2);
                        } else if (1) {
                            __auto_type _ = _t3496;
                            /* pass */
                        }
#line 714 "src/racecheck.tr"
                        ai = (ai + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t3494;
                RcCk_expr(self, c);
            }
        }
#line 716 "src/racecheck.tr"
        RcCk_exprs(self, args);
    } else if (_t3493.tag == Expr_EMethodCall) {
        __auto_type o = _t3493.data.EMethodCall.obj;
__auto_type m = _t3493.data.EMethodCall.method;
__auto_type args = _t3493.data.EMethodCall.args;
#line 718 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 719 "src/racecheck.tr"
        RcCk_exprs(self, args);
#line 720 "src/racecheck.tr"
        TrStr _tr_v_recv = _rc_root(o);
#line 721 "src/racecheck.tr"
        TrStr direct = _tr_str_lit_len("", 0LL);
#line 722 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 723 "src/racecheck.tr"
            __auto_type _t3499 = (*o);
            if (_t3499.tag == Expr_EIdent) {
                __auto_type dn = _t3499.data.EIdent.name;
                TrStr _strtmp_t3500 = _tr_str_retain(dn);
                _tr_str_release(direct);
                direct = _strtmp_t3500;
            } else if (1) {
                __auto_type _ = _t3499;
                /* pass */
            }
        }
#line 726 "src/racecheck.tr"
        if ((!_tr_str_eqv((direct), (_tr_str_lit_len("", 0LL))))) {
#line 727 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcrecv, direct);
#line 728 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcname, m);
        }
#line 729 "src/racecheck.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("wait", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("await", 5LL))))) {
#line 729 "src/racecheck.tr"
            self->seen_spawn = false;
        }
#line 730 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && (args->len > 0LL))) {
#line 731 "src/racecheck.tr"
            TrStr t = _tr_str_lit_len("", 0LL);
#line 732 "src/racecheck.tr"
            __auto_type _t3501 = (*((Expr*)List_ptr_get(args, 0LL)));
            if (_t3501.tag == Expr_EIdent) {
                __auto_type tn2 = _t3501.data.EIdent.name;
                TrStr _strtmp_t3502 = _tr_str_retain(tn2);
                _tr_str_release(t);
                t = _strtmp_t3502;
            } else if (_t3501.tag == Expr_EClosure) {
                __auto_type cps = _t3501.data.EClosure.params;
__auto_type cbody = _t3501.data.EClosure.body;
#line 736 "src/racecheck.tr"
                RcFn* tmp = ({ TrStr _apt_t3503 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t3503, self->cur->line, true)); _tr_str_release(_apt_t3503); _wr; });
#line 737 "src/racecheck.tr"
                long long ci = 0LL;
#line 738 "src/racecheck.tr"
                while ((ci < cps->len)) {
#line 739 "src/racecheck.tr"
                    ({ __auto_type _ord_t3504 = (((Param*)List_ptr_get(cps, ci))->name); List_TrStr_append(tmp->params, _ord_t3504); });
#line 740 "src/racecheck.tr"
                    ci = (ci + 1LL);
                }
#line 741 "src/racecheck.tr"
                RcFn* saved = _tr_obj_retain(self->cur);
#line 742 "src/racecheck.tr"
                RcFn* _cltmp_t3505 = _tr_obj_retain(tmp);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3505;
#line 743 "src/racecheck.tr"
                RcCk_block(self, cbody);
#line 744 "src/racecheck.tr"
                RcFn* _cltmp_t3506 = _tr_obj_retain(saved);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3506;
#line 745 "src/racecheck.tr"
                long long ck = 0LL;
#line 746 "src/racecheck.tr"
                while ((ck < tmp->touched->len)) {
#line 747 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3508 = (self->cur->locals); TrStr _at_t3507 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3508, _at_t3507)); _tr_str_release(_at_t3507); _wr; }) || ({ __auto_type _ord_t3510 = (self->cur->params); TrStr _at_t3509 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3510, _at_t3509)); _tr_str_release(_at_t3509); _wr; }))) {
#line 748 "src/racecheck.tr"
                        ({ __auto_type _ord_t3511 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] the closure passed to `spawn` captures '", 46LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3512 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3511); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the spawning function by reference: the new thread would race on it and outlive it.\n      FIX: spawn a named `def` and pass '", 132LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3513 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3512); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as an argument (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3513); _tr_str_release(_ord_t3513); });
#line 749 "src/racecheck.tr"
                        break;
                    }
#line 750 "src/racecheck.tr"
                    ck = (ck + 1LL);
                }
                _tr_obj_release(tmp, _trdrop_RcFn);
                _tr_obj_release(saved, _trdrop_RcFn);
            } else if (1) {
                __auto_type _ = _t3501;
                /* pass */
            }
#line 752 "src/racecheck.tr"
            if ((!_tr_str_eqv((t), (_tr_str_lit_len("", 0LL))))) {
#line 752 "src/racecheck.tr"
                RcCk_record_site(self, t, args, 1LL);
            }
        }
#line 753 "src/racecheck.tr"
        if (((!_tr_str_eqv((_tr_v_recv), (_tr_str_lit_len("", 0LL)))) && _rc_mut_method(m))) {
#line 754 "src/racecheck.tr"
            List_TrStr_append(self->cur->wroot, _tr_v_recv);
#line 755 "src/racecheck.tr"
            ({ TrStr _ord_t3514 = (_rc_first_prop(o)); List_TrStr_append(self->cur->wprop, _ord_t3514); _tr_str_release(_ord_t3514); });
#line 756 "src/racecheck.tr"
            List_bool_append(self->cur->wafter, self->seen_spawn);
#line 757 "src/racecheck.tr"
            RcCk_note(self, _tr_v_recv, true);
        }
        _tr_str_release(_tr_v_recv);
        _tr_str_release(direct);
    } else if (_t3493.tag == Expr_EPropAccess) {
        __auto_type o = _t3493.data.EPropAccess.obj;
        RcCk_expr(self, o);
    } else if (_t3493.tag == Expr_EIndex) {
        __auto_type o = _t3493.data.EIndex.obj;
__auto_type i = _t3493.data.EIndex._tr_v_index;
#line 760 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 761 "src/racecheck.tr"
        RcCk_expr(self, i);
    } else if (_t3493.tag == Expr_ECast) {
        __auto_type x = _t3493.data.ECast.expr;
        RcCk_expr(self, x);
    } else if (_t3493.tag == Expr_EFString) {
        __auto_type parts = _t3493.data.EFString.parts;
#line 764 "src/racecheck.tr"
        long long pi = 0LL;
#line 765 "src/racecheck.tr"
        while ((pi < parts->len)) {
#line 766 "src/racecheck.tr"
            if (((FStringPart*)List_ptr_get(parts, pi))->is_expr) {
#line 766 "src/racecheck.tr"
                RcCk_expr(self, ((FStringPart*)List_ptr_get(parts, pi))->expr);
            }
#line 767 "src/racecheck.tr"
            pi = (pi + 1LL);
        }
    } else if (_t3493.tag == Expr_ETryExpr) {
        __auto_type x = _t3493.data.ETryExpr.expr;
        RcCk_expr(self, x);
    } else if (_t3493.tag == Expr_EClosure) {
        __auto_type params = _t3493.data.EClosure.params;
__auto_type body = _t3493.data.EClosure.body;
#line 770 "src/racecheck.tr"
        long long ci = 0LL;
#line 771 "src/racecheck.tr"
        while ((ci < params->len)) {
#line 772 "src/racecheck.tr"
            ({ TrStr _apt_t3515 = _tr_str_retain(((Param*)List_ptr_get(params, ci))->name); RcCk_declare(self, _apt_t3515); _tr_str_release(_apt_t3515); });
#line 773 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 774 "src/racecheck.tr"
        RcCk_block(self, body);
    } else if (_t3493.tag == Expr_ESuperMethodCall) {
        __auto_type args = _t3493.data.ESuperMethodCall.args;
        RcCk_exprs(self, args);
    } else if (_t3493.tag == Expr_EList) {
        __auto_type items = _t3493.data.EList.items;
        RcCk_exprs(self, items);
    } else if (_t3493.tag == Expr_ESet) {
        __auto_type items = _t3493.data.ESet.items;
        RcCk_exprs(self, items);
    } else if (_t3493.tag == Expr_ETuple) {
        __auto_type items = _t3493.data.ETuple.items;
        RcCk_exprs(self, items);
    } else if (_t3493.tag == Expr_EDict) {
        __auto_type ks = _t3493.data.EDict.keys;
__auto_type vs = _t3493.data.EDict.vals;
#line 780 "src/racecheck.tr"
        RcCk_exprs(self, ks);
#line 781 "src/racecheck.tr"
        RcCk_exprs(self, vs);
    } else if (_t3493.tag == Expr_EListComp) {
        __auto_type el = _t3493.data.EListComp.element;
__auto_type gens = _t3493.data.EListComp.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3493.tag == Expr_EGeneratorExpr) {
        __auto_type el = _t3493.data.EGeneratorExpr.element;
__auto_type gens = _t3493.data.EGeneratorExpr.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3493.tag == Expr_ESlice) {
        __auto_type a = _t3493.data.ESlice.start;
__auto_type b = _t3493.data.ESlice.stop;
__auto_type c = _t3493.data.ESlice.step;
#line 785 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 786 "src/racecheck.tr"
        RcCk_expr(self, b);
#line 787 "src/racecheck.tr"
        RcCk_expr(self, c);
    } else if (_t3493.tag == Expr_EAwait) {
        __auto_type x = _t3493.data.EAwait.expr;
        RcCk_expr(self, x);
    } else if (_t3493.tag == Expr_EYield) {
        __auto_type x = _t3493.data.EYield.expr;
        RcCk_expr(self, x);
    } else if (_t3493.tag == Expr_ETry) {
        __auto_type tb = _t3493.data.ETry.try_body;
__auto_type catches = _t3493.data.ETry.catches;
__auto_type fb = _t3493.data.ETry.finally_b;
#line 791 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 792 "src/racecheck.tr"
        long long ci2 = 0LL;
#line 793 "src/racecheck.tr"
        while ((ci2 < catches->len)) {
#line 794 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci2)))->body);
#line 795 "src/racecheck.tr"
            ci2 = (ci2 + 1LL);
        }
#line 796 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t3493.tag == Expr_ERange) {
        __auto_type a = _t3493.data.ERange.start;
__auto_type b = _t3493.data.ERange.end;
#line 798 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 799 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3493.tag == Expr_EIfElse) {
        __auto_type c = _t3493.data.EIfElse.cond;
__auto_type a = _t3493.data.EIfElse.then_expr;
__auto_type b = _t3493.data.EIfElse.else_expr;
#line 801 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 802 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 803 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3493.tag == Expr_EDo) {
        __auto_type b = _t3493.data.EDo.body;
        RcCk_block(self, b);
    } else if (_t3493.tag == Expr_EMatch) {
        __auto_type s = _t3493.data.EMatch.subj;
__auto_type arms = _t3493.data.EMatch.arms;
#line 806 "src/racecheck.tr"
        RcCk_expr(self, s);
#line 807 "src/racecheck.tr"
        long long mi = 0LL;
#line 808 "src/racecheck.tr"
        while ((mi < arms->len)) {
#line 809 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, mi))->guard);
#line 810 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, mi))->body);
#line 811 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t3493.tag == Expr_EMacroCall) {
        __auto_type args = _t3493.data.EMacroCall.args;
        RcCk_exprs(self, args);
    } else if (_t3493.tag == Expr_ELoop) {
        __auto_type b = _t3493.data.ELoop.body;
        RcCk_block(self, b);
    } else if (_t3493.tag == Expr_EWhileExpr) {
        __auto_type c = _t3493.data.EWhileExpr.cond;
__auto_type b = _t3493.data.EWhileExpr.body;
__auto_type eb = _t3493.data.EWhileExpr.else_body;
#line 815 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 816 "src/racecheck.tr"
        RcCk_block(self, b);
#line 817 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (1) {
        __auto_type _ = _t3493;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_add_method(RcCk* self, FunctionDef* f, TrStr cls, TrStr file, bool lib) {
#line 821 "src/racecheck.tr"
    ({ TrStr _at_t3516 = (({ TrStr _tr_cl = (_tr_strx_concatv((cls), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_add_fn_named(self, f, _at_t3516, file, lib); _tr_str_release(_at_t3516); });
}

__attribute__((hot)) void RcCk_add_fn(RcCk* self, FunctionDef* f, TrStr file, bool lib) {
#line 824 "src/racecheck.tr"
    ({ TrStr _apt_t3517 = _tr_str_retain(f->name); RcCk_add_fn_named(self, f, _apt_t3517, file, lib); _tr_str_release(_apt_t3517); });
}

__attribute__((hot)) void RcCk_add_fn_named(RcCk* self, FunctionDef* f, TrStr nm, TrStr file, bool lib) {
#line 827 "src/racecheck.tr"
    RcFn* rf = _rc_new_fn(nm, file, f->line, (lib || f->is_lib));
#line 828 "src/racecheck.tr"
    long long pi = 0LL;
#line 829 "src/racecheck.tr"
    while ((pi < f->params->len)) {
#line 830 "src/racecheck.tr"
        ({ __auto_type _ord_t3518 = (((Param*)List_ptr_get(f->params, pi))->name); List_TrStr_append(rf->params, _ord_t3518); });
#line 831 "src/racecheck.tr"
        if ((!rf->is_lib)) {
#line 831 "src/racecheck.tr"
            ({ __auto_type _ord_t3519 = (((Param*)List_ptr_get(f->params, pi))->ty); __auto_type _ord_t3520 = (f->line); RcCk_ty_check(self, _ord_t3519, file, _ord_t3520); });
        }
#line 832 "src/racecheck.tr"
        AstType** pt = ((Param*)List_ptr_get(f->params, pi))->ty;
#line 833 "src/racecheck.tr"
        if ((((unsigned long long)(pt)) != ((unsigned long long)(0LL)))) {
#line 834 "src/racecheck.tr"
            ({ __auto_type _ord_t3521 = ((*pt)->name); List_TrStr_append(rf->ptypes, _ord_t3521); });
#line 835 "src/racecheck.tr"
            if (((*pt)->args->len > 0LL)) {
#line 835 "src/racecheck.tr"
                ({ __auto_type _ord_t3522 = ((*((AstType**)List_ptr_get((*pt)->args, 0LL)))->name); List_TrStr_append(rf->pinner, _ord_t3522); });
            } else {
#line 836 "src/racecheck.tr"
                List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
            }
        } else {
#line 838 "src/racecheck.tr"
            List_TrStr_append(rf->ptypes, _tr_str_lit_len("", 0LL));
#line 839 "src/racecheck.tr"
            List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
        }
#line 840 "src/racecheck.tr"
        pi = (pi + 1LL);
    }
#line 841 "src/racecheck.tr"
    List_ptr_append(self->fns, _tr_obj_retain(rf));
#line 842 "src/racecheck.tr"
    if (rf->is_lib) {
#line 842 "src/racecheck.tr"
        _tr_obj_release(rf, _trdrop_RcFn);
        return;
    }
#line 843 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 844 "src/racecheck.tr"
    long long saved_line = self->cur_line;
#line 845 "src/racecheck.tr"
    long long saved_ld = self->loop_depth;
#line 846 "src/racecheck.tr"
    bool saved_sp = self->seen_spawn;
#line 847 "src/racecheck.tr"
    RcFn* _cltmp_t3523 = _tr_obj_retain(rf);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3523;
#line 848 "src/racecheck.tr"
    self->cur_line = f->line;
#line 849 "src/racecheck.tr"
    self->loop_depth = 0LL;
#line 850 "src/racecheck.tr"
    self->seen_spawn = false;
#line 851 "src/racecheck.tr"
    RcCk_block(self, f->body);
#line 852 "src/racecheck.tr"
    long long tw = 0LL;
#line 853 "src/racecheck.tr"
    while ((tw < rf->thr_made->len)) {
#line 854 "src/racecheck.tr"
        if (({ __auto_type _ord_t3525 = (rf->idents); TrStr _at_t3524 = (List_TrStr_get(rf->thr_made, tw)); __auto_type _wr = ((!_rc_has(_ord_t3525, _at_t3524))); _tr_str_release(_at_t3524); _wr; })) {
#line 855 "src/racecheck.tr"
            ({ __auto_type _ord_t3526 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("W|", 2LL)), (file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(List_i64_get(rf->thr_line, tw))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] the Thread handle '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3527 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3526); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is never joined, detached or used: the thread runs unsupervised and the program may exit while it is still working.\n      FIX: call `", 135LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3528 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3527); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".join()` (wait for it) or `", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3529 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3528); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".detach()` (run independently).", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(rf->msgs, _ord_t3529); _tr_str_release(_ord_t3529); });
        }
#line 856 "src/racecheck.tr"
        tw = (tw + 1LL);
    }
#line 857 "src/racecheck.tr"
    RcFn* _cltmp_t3530 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3530;
#line 858 "src/racecheck.tr"
    self->cur_line = saved_line;
#line 859 "src/racecheck.tr"
    self->loop_depth = saved_ld;
#line 860 "src/racecheck.tr"
    self->seen_spawn = saved_sp;
    _tr_obj_release(rf, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
}

__attribute__((hot)) void RcCk_add_global(RcCk* self, TrStr name, bool is_mut, bool is_const, AstType** ty, Expr* v, TrStr df) {
#line 863 "src/racecheck.tr"
    List_TrStr_append(self->gnames, name);
#line 864 "src/racecheck.tr"
    List_TrStr_append(self->gfiles, df);
#line 865 "src/racecheck.tr"
    TrStr tn = _tr_str_lit_len("", 0LL);
#line 866 "src/racecheck.tr"
    if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 866 "src/racecheck.tr"
        TrStr _strtmp_t3531 = _tr_str_retain((*ty)->name);
        _tr_str_release(tn);
        tn = _strtmp_t3531;
    }
#line 867 "src/racecheck.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 867 "src/racecheck.tr"
        TrStr _strtmp_t3532 = _rc_ctor_name(v);
        _tr_str_release(tn);
        tn = _strtmp_t3532;
    }
#line 868 "src/racecheck.tr"
    bool sync = _rc_sync_ty(tn);
#line 869 "src/racecheck.tr"
    if (((!sync) && (!_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))))) {
#line 870 "src/racecheck.tr"
        long long ci0 = RcCk_find_class(self, tn);
#line 871 "src/racecheck.tr"
        if (((ci0 >= 0LL) && List_bool_get(self->class_lib, ci0))) {
#line 871 "src/racecheck.tr"
            sync = true;
        }
    }
#line 872 "src/racecheck.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 873 "src/racecheck.tr"
        __auto_type _t3533 = (*v);
        if (_t3533.tag == Expr_ELitInt) {
            __auto_type _ = _t3533.data.ELitInt.val;
            /* pass */
        } else if (_t3533.tag == Expr_ELitFloat) {
            __auto_type _ = _t3533.data.ELitFloat.val;
            /* pass */
        } else if (_t3533.tag == Expr_ELitStr) {
            /* pass */
        } else if (_t3533.tag == Expr_ELitBool) {
            __auto_type _ = _t3533.data.ELitBool.val;
            /* pass */
        } else if (_t3533.tag == Expr_EList) {
            __auto_type _ = _t3533.data.EList.items;
            /* pass */
        } else if (_t3533.tag == Expr_EDict) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t3533;
            sync = true;
        }
    }
#line 881 "src/racecheck.tr"
    if ((!(is_mut && (!is_const)))) {
#line 883 "src/racecheck.tr"
        bool content_mut = (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || (RcCk_find_class(self, tn) >= 0LL));
#line 884 "src/racecheck.tr"
        if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 885 "src/racecheck.tr"
            __auto_type _t3534 = (*v);
            if (_t3534.tag == Expr_EList) {
                __auto_type _ = _t3534.data.EList.items;
                content_mut = true;
            } else if (_t3534.tag == Expr_EDict) {
                content_mut = true;
            } else if (_t3534.tag == Expr_ESet) {
                __auto_type _ = _t3534.data.ESet.items;
                content_mut = true;
            } else if (1) {
                __auto_type _ = _t3534;
                /* pass */
            }
        }
#line 890 "src/racecheck.tr"
        if ((!content_mut)) {
#line 890 "src/racecheck.tr"
            sync = true;
        }
    }
#line 891 "src/racecheck.tr"
    List_bool_append(self->gsync, sync);
#line 892 "src/racecheck.tr"
    List_TrStr_append(self->gtn, tn);
    _tr_str_release(tn);
}

__attribute__((hot)) long long RcCk_find_fn(RcCk* self, TrStr name) {
#line 895 "src/racecheck.tr"
    long long i = 0LL;
#line 896 "src/racecheck.tr"
    while ((i < self->fns->len)) {
#line 897 "src/racecheck.tr"
        if ((_tr_str_eqv((((RcFn*)List_ptr_get(self->fns, i))->name), (name)) && (!((RcFn*)List_ptr_get(self->fns, i))->is_lib))) {
#line 897 "src/racecheck.tr"
            return i;
        }
#line 898 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 899 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) long long RcCk_find_class(RcCk* self, TrStr name) {
#line 902 "src/racecheck.tr"
    long long i = 0LL;
#line 903 "src/racecheck.tr"
    while ((i < self->classes->len)) {
#line 904 "src/racecheck.tr"
        if (_tr_str_eqv((((ClassDef*)List_ptr_get(self->classes, i))->name), (name))) {
#line 904 "src/racecheck.tr"
            return i;
        }
#line 905 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 906 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool RcCk_field_plain(RcCk* self, TrStr cn, TrStr prop) {
#line 910 "src/racecheck.tr"
    long long ci = RcCk_find_class(self, cn);
#line 911 "src/racecheck.tr"
    if ((ci < 0LL)) {
#line 911 "src/racecheck.tr"
        return false;
    }
#line 912 "src/racecheck.tr"
    ClassDef* cls = ((ClassDef*)List_ptr_get(self->classes, ci));
#line 913 "src/racecheck.tr"
    long long i = 0LL;
#line 914 "src/racecheck.tr"
    while ((i < cls->fields->len)) {
#line 915 "src/racecheck.tr"
        if (_tr_str_eqv((((FieldDef*)List_ptr_get(cls->fields, i))->name), (prop))) {
#line 916 "src/racecheck.tr"
            AstType** ft = ((FieldDef*)List_ptr_get(cls->fields, i))->ty;
#line 917 "src/racecheck.tr"
            if ((((unsigned long long)(ft)) == ((unsigned long long)(0LL)))) {
#line 917 "src/racecheck.tr"
                return false;
            }
#line 918 "src/racecheck.tr"
            TrStr tn = _tr_str_retain((*ft)->name);
#line 919 "src/racecheck.tr"
            if (_rc_sync_ty(tn)) {
#line 919 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 920 "src/racecheck.tr"
            long long ni = RcCk_find_class(self, tn);
#line 921 "src/racecheck.tr"
            if (((ni >= 0LL) && List_bool_get(self->class_lib, ni))) {
#line 921 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 922 "src/racecheck.tr"
            _tr_str_release(tn);
            return true;
        }
#line 923 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 924 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_i64* RcCk_reach(RcCk* self, long long ti) {
#line 928 "src/racecheck.tr"
    List_i64* seen = (void*)List_i64_new();
#line 929 "src/racecheck.tr"
    List_i64_append(seen, ti);
#line 930 "src/racecheck.tr"
    long long k = 0LL;
#line 931 "src/racecheck.tr"
    while ((k < seen->len)) {
#line 932 "src/racecheck.tr"
        RcFn* f = ({ __auto_type _ord_t3535 = (List_i64_get(seen, k)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3535))); _wr; });
#line 933 "src/racecheck.tr"
        long long ci = 0LL;
#line 934 "src/racecheck.tr"
        while ((ci < f->calls->len)) {
#line 935 "src/racecheck.tr"
            long long ni = ({ TrStr _at_t3536 = (List_TrStr_get(f->calls, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3536)); _tr_str_release(_at_t3536); _wr; });
#line 936 "src/racecheck.tr"
            if ((ni >= 0LL)) {
#line 937 "src/racecheck.tr"
                bool dup = false;
#line 938 "src/racecheck.tr"
                long long q = 0LL;
#line 939 "src/racecheck.tr"
                while ((q < seen->len)) {
#line 940 "src/racecheck.tr"
                    if ((List_i64_get(seen, q) == ni)) {
#line 940 "src/racecheck.tr"
                        dup = true;
                    }
#line 941 "src/racecheck.tr"
                    q = (q + 1LL);
                }
#line 942 "src/racecheck.tr"
                if ((!dup)) {
#line 942 "src/racecheck.tr"
                    List_i64_append(seen, ni);
                }
            }
#line 943 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 944 "src/racecheck.tr"
        long long mci = 0LL;
#line 945 "src/racecheck.tr"
        while ((mci < f->mcrecv->len)) {
#line 946 "src/racecheck.tr"
            TrStr rcv = List_TrStr_get(f->mcrecv, mci);
#line 947 "src/racecheck.tr"
            TrStr rcn = _tr_str_lit_len("", 0LL);
#line 948 "src/racecheck.tr"
            if (_tr_str_eqv((rcv), (_tr_str_lit_len("self", 4LL)))) {
#line 949 "src/racecheck.tr"
                long long dot = ({ TrStr _apt_t3537 = _tr_str_retain(f->name); __auto_type _wr = (_rc_dot(_apt_t3537)); _tr_str_release(_apt_t3537); _wr; });
#line 950 "src/racecheck.tr"
                if ((dot > 0LL)) {
#line 950 "src/racecheck.tr"
                    TrStr _strtmp_t3538 = _tr_str_slicev((f->name), 0LL, dot);
                    _tr_str_release(rcn);
                    rcn = _strtmp_t3538;
                }
            } else {
#line 952 "src/racecheck.tr"
                TrStr _strtmp_t3539 = RcCk_lclass(self, f, rcv);
                _tr_str_release(rcn);
                rcn = _strtmp_t3539;
#line 953 "src/racecheck.tr"
                if (_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL)))) {
#line 954 "src/racecheck.tr"
                    long long gj = 0LL;
#line 955 "src/racecheck.tr"
                    while ((gj < self->gnames->len)) {
#line 956 "src/racecheck.tr"
                        if (({ TrStr _cmpa_t3540 = (List_TrStr_get(self->gnames, gj)); TrStr _cmpb_t3541 = (rcv); _Bool _cmpr_t3542 = _tr_str_eqv(_cmpa_t3540, _cmpb_t3541); _tr_str_release(_cmpa_t3540); _cmpr_t3542; })) {
#line 956 "src/racecheck.tr"
                            TrStr _strtmp_t3543 = List_TrStr_get(self->gtn, gj);
                            _tr_str_release(rcn);
                            rcn = _strtmp_t3543;
                        }
#line 957 "src/racecheck.tr"
                        gj = (gj + 1LL);
                    }
                }
            }
#line 958 "src/racecheck.tr"
            if ((!_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL))))) {
#line 959 "src/racecheck.tr"
                long long mni = ({ TrStr _at_t3544 = (({ TrStr _tr_cl = (_tr_strx_concatv((rcn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(f->mcname, mci)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3544)); _tr_str_release(_at_t3544); _wr; });
#line 960 "src/racecheck.tr"
                if ((mni >= 0LL)) {
#line 961 "src/racecheck.tr"
                    bool dup2 = false;
#line 962 "src/racecheck.tr"
                    long long q2 = 0LL;
#line 963 "src/racecheck.tr"
                    while ((q2 < seen->len)) {
#line 964 "src/racecheck.tr"
                        if ((List_i64_get(seen, q2) == mni)) {
#line 964 "src/racecheck.tr"
                            dup2 = true;
                        }
#line 965 "src/racecheck.tr"
                        q2 = (q2 + 1LL);
                    }
#line 966 "src/racecheck.tr"
                    if ((!dup2)) {
#line 966 "src/racecheck.tr"
                        List_i64_append(seen, mni);
                    }
                }
            }
#line 967 "src/racecheck.tr"
            mci = (mci + 1LL);
            _tr_str_release(rcv);
            _tr_str_release(rcn);
        }
#line 968 "src/racecheck.tr"
        k = (k + 1LL);
    }
#line 969 "src/racecheck.tr"
    return seen;
}

__attribute__((hot)) void RcCk_analyze_site(RcCk* self, RcFn* f, RcSite* s) {
#line 973 "src/racecheck.tr"
    if (((!_tr_str_eqv((s->target), (_tr_str_lit_len("", 0LL)))) && ({ __auto_type _ord_t3546 = (f->closures); TrStr _apt_t3545 = _tr_str_retain(s->target); __auto_type _wr = (_rc_has(_ord_t3546, _apt_t3545)); _tr_str_release(_apt_t3545); _wr; }))) {
#line 974 "src/racecheck.tr"
        ({ __auto_type _ord_t3547 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3548 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3547); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a closure value: it captures its surrounding locals by reference into this stack frame, so a spawned thread would race on them and outlive them.\n      FIX: spawn a named `def` and pass the data it needs as arguments (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 289LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3548); _tr_str_release(_ord_t3548); });
#line 975 "src/racecheck.tr"
        return;
    }
#line 976 "src/racecheck.tr"
    if ((!_tr_str_eqv((s->recv), (_tr_str_lit_len("", 0LL))))) {
#line 977 "src/racecheck.tr"
        RcCk_analyze_method_site(self, f, s);
#line 978 "src/racecheck.tr"
        return;
    }
#line 979 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3549 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3549)); _tr_str_release(_apt_t3549); _wr; });
#line 980 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 980 "src/racecheck.tr"
        return;
    }
#line 981 "src/racecheck.tr"
    ({ TrStr _apt_t3550 = _tr_str_retain(s->target); RcCk_check_globals(self, f, s, ti, _apt_t3550); _tr_str_release(_apt_t3550); });
#line 983 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, ti));
#line 984 "src/racecheck.tr"
    long long ai = 0LL;
#line 985 "src/racecheck.tr"
    while (((ai < s->args->len) && (ai < tf->params->len))) {
#line 986 "src/racecheck.tr"
        TrStr an = _tr_str_lit_len("", 0LL);
#line 987 "src/racecheck.tr"
        __auto_type _t3551 = (*((Expr*)List_ptr_get(s->args, ai)));
        if (_t3551.tag == Expr_EIdent) {
            __auto_type nn = _t3551.data.EIdent.name;
            TrStr _strtmp_t3552 = _tr_str_retain(nn);
            _tr_str_release(an);
            an = _strtmp_t3552;
        } else if (1) {
            __auto_type _ = _t3551;
            /* pass */
        }
#line 990 "src/racecheck.tr"
        TrStr ptn = List_TrStr_get(tf->ptypes, ai);
#line 991 "src/racecheck.tr"
        if ((!_tr_str_eqv((an), (_tr_str_lit_len("", 0LL))))) {
#line 992 "src/racecheck.tr"
            long long cnt = 0LL;
#line 993 "src/racecheck.tr"
            long long si = 0LL;
#line 994 "src/racecheck.tr"
            while ((si < f->sites->len)) {
#line 995 "src/racecheck.tr"
                long long oa = 0LL;
#line 996 "src/racecheck.tr"
                while ((oa < ((RcSite*)List_ptr_get(f->sites, si))->args->len)) {
#line 997 "src/racecheck.tr"
                    __auto_type _t3553 = (*((Expr*)List_ptr_get(((RcSite*)List_ptr_get(f->sites, si))->args, oa)));
                    if (_t3553.tag == Expr_EIdent) {
                        __auto_type on = _t3553.data.EIdent.name;
#line 999 "src/racecheck.tr"
                        if (_tr_str_eqv((on), (an))) {
#line 999 "src/racecheck.tr"
                            cnt = (cnt + 1LL);
                        }
                    } else if (1) {
                        __auto_type _ = _t3553;
                        /* pass */
                    }
#line 1001 "src/racecheck.tr"
                    oa = (oa + 1LL);
                }
#line 1002 "src/racecheck.tr"
                si = (si + 1LL);
            }
#line 1003 "src/racecheck.tr"
            bool is_sh = (_rc_has(f->shvars, an) || ({ TrStr _cmpa_t3554 = (RcCk_ltype(self, f, an)); TrStr _cmpb_t3555 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3556 = _tr_str_eqv(_cmpa_t3554, _cmpb_t3555); _tr_str_release(_cmpa_t3554); _cmpr_t3556; }));
#line 1004 "src/racecheck.tr"
            if (((is_sh && (!_tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL))))) && (!_tr_str_eqv((ptn), (_tr_str_lit_len("", 0LL)))))) {
#line 1005 "src/racecheck.tr"
                ({ __auto_type _ord_t3557 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-10] '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a `Shared` handle but '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3558 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3557); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares parameter '", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3559 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3558); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': the thread would be handed the shared control block where it expects the object itself (a type confusion).\n      FIX: declare the worker's parameter as `", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3560 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3559); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Shared[", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3560); _tr_str_release(_ord_t3560); });
#line 1006 "src/racecheck.tr"
                ai = (ai + 1LL);
#line 1007 "src/racecheck.tr"
                _tr_str_release(an);
                _tr_str_release(ptn);
                continue;
            }
#line 1008 "src/racecheck.tr"
            long long dd = RcCk_ldepth_of(self, f, an);
#line 1009 "src/racecheck.tr"
            bool multi = ((((cnt >= 2LL) || is_sh) || _tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL)))) || ((dd >= 0LL) && (s->depth > dd)));
#line 1010 "src/racecheck.tr"
            if (multi) {
#line 1011 "src/racecheck.tr"
                RcCk_check_shared_writes(self, f, s, tf, ai, an);
#line 1012 "src/racecheck.tr"
                if ((is_sh || _tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL))))) {
#line 1012 "src/racecheck.tr"
                    RcCk_check_parent_writes(self, f, s, an);
                }
            }
        } else if (_tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL)))) {
#line 1015 "src/racecheck.tr"
            RcCk_check_shared_writes(self, f, s, tf, ai, _tr_str_lit_len("a shared handle", 15LL));
        }
#line 1016 "src/racecheck.tr"
        ai = (ai + 1LL);
        _tr_str_release(an);
        _tr_str_release(ptn);
    }
}

__attribute__((hot)) void RcCk_analyze_method_site(RcCk* self, RcFn* f, RcSite* s) {
#line 1020 "src/racecheck.tr"
    TrStr cn = ({ TrStr _apt_t3561 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_lclass(self, f, _apt_t3561)); _tr_str_release(_apt_t3561); _wr; });
#line 1021 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1021 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1022 "src/racecheck.tr"
    long long mi = ({ TrStr _at_t3562 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3562)); _tr_str_release(_at_t3562); _wr; });
#line 1023 "src/racecheck.tr"
    if ((mi < 0LL)) {
#line 1023 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1024 "src/racecheck.tr"
    ({ TrStr _at_t3563 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_check_globals(self, f, s, mi, _at_t3563); _tr_str_release(_at_t3563); });
#line 1025 "src/racecheck.tr"
    long long cnt = 0LL;
#line 1026 "src/racecheck.tr"
    long long si = 0LL;
#line 1027 "src/racecheck.tr"
    while ((si < f->sites->len)) {
#line 1028 "src/racecheck.tr"
        if (({ __auto_type _ord_t3564 = (((RcSite*)List_ptr_get(f->sites, si))->recv); __auto_type _wr = (_tr_str_eqv((_ord_t3564), (s->recv))); _wr; })) {
#line 1028 "src/racecheck.tr"
            cnt = (cnt + 1LL);
        }
#line 1029 "src/racecheck.tr"
        si = (si + 1LL);
    }
#line 1030 "src/racecheck.tr"
    bool is_sh = (({ __auto_type _ord_t3566 = (f->shvars); TrStr _apt_t3565 = _tr_str_retain(s->recv); __auto_type _wr = (_rc_has(_ord_t3566, _apt_t3565)); _tr_str_release(_apt_t3565); _wr; }) || ({ TrStr _apt_t3567 = _tr_str_retain(s->recv); __auto_type _wr = (({ TrStr _cmpa_t3568 = (RcCk_ltype(self, f, _apt_t3567)); TrStr _cmpb_t3569 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3570 = _tr_str_eqv(_cmpa_t3568, _cmpb_t3569); _tr_str_release(_cmpa_t3568); _cmpr_t3570; })); _tr_str_release(_apt_t3567); _wr; }));
#line 1031 "src/racecheck.tr"
    long long dd = ({ TrStr _apt_t3571 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_ldepth_of(self, f, _apt_t3571)); _tr_str_release(_apt_t3571); _wr; });
#line 1032 "src/racecheck.tr"
    if ((!(((cnt >= 2LL) || is_sh) || ((dd >= 0LL) && (s->depth > dd))))) {
#line 1032 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1033 "src/racecheck.tr"
    RcFn* mf = ((RcFn*)List_ptr_get(self->fns, mi));
#line 1034 "src/racecheck.tr"
    long long k = 0LL;
#line 1035 "src/racecheck.tr"
    while ((k < mf->wroot->len)) {
#line 1036 "src/racecheck.tr"
        if (((({ TrStr _cmpa_t3572 = (List_TrStr_get(mf->wroot, k)); TrStr _cmpb_t3573 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3574 = _tr_str_eqv(_cmpa_t3572, _cmpb_t3573); _tr_str_release(_cmpa_t3572); _cmpr_t3574; }) && ({ TrStr _cmpa_t3575 = (List_TrStr_get(mf->wprop, k)); TrStr _cmpb_t3576 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3577 = !_tr_str_eqv(_cmpa_t3575, _cmpb_t3576); _tr_str_release(_cmpa_t3575); _cmpr_t3577; })) && ({ TrStr _at_t3578 = (List_TrStr_get(mf->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3578)); _tr_str_release(_at_t3578); _wr; }))) {
#line 1037 "src/racecheck.tr"
            ({ TrStr _ord_t3579 = (List_TrStr_get(mf->wprop, k)); __auto_type _ord_t3580 = (s->recv); RcCk_shared_msg(self, f, s, mf, cn, _ord_t3579, _ord_t3580); _tr_str_release(_ord_t3579); });
#line 1038 "src/racecheck.tr"
            _tr_str_release(cn);
            return;
        }
#line 1039 "src/racecheck.tr"
        k = (k + 1LL);
    }
#line 1040 "src/racecheck.tr"
    if (is_sh) {
#line 1040 "src/racecheck.tr"
        ({ TrStr _apt_t3581 = _tr_str_retain(s->recv); RcCk_check_parent_writes(self, f, s, _apt_t3581); _tr_str_release(_apt_t3581); });
    }
    _tr_str_release(cn);
}

__attribute__((hot)) void RcCk_check_parent_writes(RcCk* self, RcFn* f, RcSite* s, TrStr an) {
#line 1044 "src/racecheck.tr"
    TrStr cn = RcCk_lclass(self, f, an);
#line 1045 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1045 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 1046 "src/racecheck.tr"
    long long k = 0LL;
#line 1047 "src/racecheck.tr"
    while ((k < f->wroot->len)) {
#line 1048 "src/racecheck.tr"
        if ((((({ TrStr _cmpa_t3582 = (List_TrStr_get(f->wroot, k)); TrStr _cmpb_t3583 = (an); _Bool _cmpr_t3584 = _tr_str_eqv(_cmpa_t3582, _cmpb_t3583); _tr_str_release(_cmpa_t3582); _cmpr_t3584; }) && List_bool_get(f->wafter, k)) && ({ TrStr _cmpa_t3585 = (List_TrStr_get(f->wprop, k)); TrStr _cmpb_t3586 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3587 = !_tr_str_eqv(_cmpa_t3585, _cmpb_t3586); _tr_str_release(_cmpa_t3585); _cmpr_t3587; })) && ({ TrStr _at_t3588 = (List_TrStr_get(f->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3588)); _tr_str_release(_at_t3588); _wr; }))) {
#line 1049 "src/racecheck.tr"
            ({ __auto_type _ord_t3589 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3590 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3589); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' writes the plain field '", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3591 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3590); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while a spawned thread holds the same shared object: the two threads race on that field.\n      FIX: make '", 108LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3592 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3591); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` or guard the object with a `Mutex[T]`, or finish the writes before spawning / after the threads are joined.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3592); _tr_str_release(_ord_t3592); });
#line 1050 "src/racecheck.tr"
            _tr_str_release(cn);
            return;
        }
#line 1051 "src/racecheck.tr"
        k = (k + 1LL);
    }
    _tr_str_release(cn);
}

__attribute__((hot)) void RcCk_check_globals(RcCk* self, RcFn* f, RcSite* s, long long ti, TrStr label) {
#line 1054 "src/racecheck.tr"
    List_i64* reach = RcCk_reach(self, ti);
#line 1056 "src/racecheck.tr"
    long long gi = 0LL;
#line 1057 "src/racecheck.tr"
    while ((gi < self->gnames->len)) {
#line 1058 "src/racecheck.tr"
        if ((!List_bool_get(self->gsync, gi))) {
#line 1059 "src/racecheck.tr"
            TrStr g = List_TrStr_get(self->gnames, gi);
#line 1060 "src/racecheck.tr"
            bool wr = false;
#line 1061 "src/racecheck.tr"
            bool rd = false;
#line 1062 "src/racecheck.tr"
            bool called = false;
#line 1063 "src/racecheck.tr"
            bool mwrites = false;
#line 1064 "src/racecheck.tr"
            long long ri = 0LL;
#line 1065 "src/racecheck.tr"
            while ((ri < reach->len)) {
#line 1066 "src/racecheck.tr"
                RcFn* rf = ({ __auto_type _ord_t3593 = (List_i64_get(reach, ri)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3593))); _wr; });
#line 1067 "src/racecheck.tr"
                long long ti2 = 0LL;
#line 1068 "src/racecheck.tr"
                while ((ti2 < rf->touched->len)) {
#line 1069 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3594 = (List_TrStr_get(rf->touched, ti2)); TrStr _cmpb_t3595 = (g); _Bool _cmpr_t3596 = _tr_str_eqv(_cmpa_t3594, _cmpb_t3595); _tr_str_release(_cmpa_t3594); _cmpr_t3596; })) {
#line 1070 "src/racecheck.tr"
                        if (List_bool_get(rf->touched_w, ti2)) {
#line 1070 "src/racecheck.tr"
                            wr = true;
                        } else {
#line 1071 "src/racecheck.tr"
                            rd = true;
                        }
                    }
#line 1072 "src/racecheck.tr"
                    ti2 = (ti2 + 1LL);
                }
#line 1073 "src/racecheck.tr"
                if (({ TrStr _at_t3597 = (List_TrStr_get(self->gtn, gi)); __auto_type _wr = ((RcCk_find_class(self, _at_t3597) >= 0LL)); _tr_str_release(_at_t3597); _wr; })) {
#line 1075 "src/racecheck.tr"
                    long long mq = 0LL;
#line 1076 "src/racecheck.tr"
                    while ((mq < rf->mcrecv->len)) {
#line 1077 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3598 = (List_TrStr_get(rf->mcrecv, mq)); TrStr _cmpb_t3599 = (g); _Bool _cmpr_t3600 = _tr_str_eqv(_cmpa_t3598, _cmpb_t3599); _tr_str_release(_cmpa_t3598); _cmpr_t3600; }) && (!_rc_has(rf->locals, g))) && (!_rc_has(rf->params, g)))) {
#line 1077 "src/racecheck.tr"
                            called = true;
                        }
#line 1078 "src/racecheck.tr"
                        mq = (mq + 1LL);
                    }
#line 1079 "src/racecheck.tr"
                    TrStr pre = ({ TrStr _tr_cl = (List_TrStr_get(self->gtn, gi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 1080 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3601 = (_tr_str_lenv((rf->name))); __auto_type _wr = ((_ord_t3601 > _tr_str_lenv((pre)))); _wr; }) && ({ __auto_type _ord_t3602 = (_tr_str_lenv((pre))); __auto_type _wr = (({ TrStr _cmpa_t3603 = (_tr_str_slicev((rf->name), 0LL, _ord_t3602)); TrStr _cmpb_t3604 = (pre); _Bool _cmpr_t3605 = _tr_str_eqv(_cmpa_t3603, _cmpb_t3604); _tr_str_release(_cmpa_t3603); _cmpr_t3605; })); _wr; }))) {
#line 1081 "src/racecheck.tr"
                        long long mk = 0LL;
#line 1082 "src/racecheck.tr"
                        while ((mk < rf->wroot->len)) {
#line 1083 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3606 = (List_TrStr_get(rf->wroot, mk)); TrStr _cmpb_t3607 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3608 = _tr_str_eqv(_cmpa_t3606, _cmpb_t3607); _tr_str_release(_cmpa_t3606); _cmpr_t3608; }) && ({ TrStr _cmpa_t3609 = (List_TrStr_get(rf->wprop, mk)); TrStr _cmpb_t3610 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3611 = !_tr_str_eqv(_cmpa_t3609, _cmpb_t3610); _tr_str_release(_cmpa_t3609); _cmpr_t3611; })) && ({ TrStr _ord_t3612 = (List_TrStr_get(self->gtn, gi)); TrStr _ord_t3613 = (List_TrStr_get(rf->wprop, mk)); __auto_type _wr = (RcCk_field_plain(self, _ord_t3612, _ord_t3613)); _tr_str_release(_ord_t3612); _tr_str_release(_ord_t3613); _wr; }))) {
#line 1083 "src/racecheck.tr"
                                mwrites = true;
                            }
#line 1084 "src/racecheck.tr"
                            mk = (mk + 1LL);
                        }
                    }
                }
#line 1085 "src/racecheck.tr"
                ri = (ri + 1LL);
            }
#line 1086 "src/racecheck.tr"
            if ((called && mwrites)) {
#line 1086 "src/racecheck.tr"
                wr = true;
            }
#line 1087 "src/racecheck.tr"
            if ((rd && (!wr))) {
#line 1088 "src/racecheck.tr"
                long long fi = 0LL;
#line 1089 "src/racecheck.tr"
                while ((fi < self->fns->len)) {
#line 1090 "src/racecheck.tr"
                    RcFn* of = ((RcFn*)List_ptr_get(self->fns, fi));
#line 1091 "src/racecheck.tr"
                    if ((!of->is_lib)) {
#line 1092 "src/racecheck.tr"
                        long long oi = 0LL;
#line 1093 "src/racecheck.tr"
                        while ((oi < of->touched->len)) {
#line 1094 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3614 = (List_TrStr_get(of->touched, oi)); TrStr _cmpb_t3615 = (g); _Bool _cmpr_t3616 = _tr_str_eqv(_cmpa_t3614, _cmpb_t3615); _tr_str_release(_cmpa_t3614); _cmpr_t3616; }) && List_bool_get(of->touched_w, oi)) && List_bool_get(of->touched_after, oi))) {
#line 1094 "src/racecheck.tr"
                                wr = true;
                            }
#line 1095 "src/racecheck.tr"
                            oi = (oi + 1LL);
                        }
                    }
#line 1096 "src/racecheck.tr"
                    fi = (fi + 1LL);
                }
            }
#line 1097 "src/racecheck.tr"
            if (wr) {
#line 1098 "src/racecheck.tr"
                ({ TrStr _ord_t3617 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] the spawned function '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (label)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses the module-level mutable '", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is plain data shared by every thread: concurrent access is a data race.\n      FIX: make '", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (counters/flags), a `Mutex[T]`/`RwLock[T]` (compound state), or pass the state to the thread explicitly.", 121LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3617); _tr_str_release(_ord_t3617); });
            } else if (rd) {
#line 1100 "src/racecheck.tr"
                /* pass */
            }
        }
#line 1101 "src/racecheck.tr"
        gi = (gi + 1LL);
    }
}

__attribute__((hot)) TrStr RcCk_param_write_prop(RcCk* self, long long fi, TrStr pname, TrStr cn, long long depth) {
#line 1106 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, fi));
#line 1107 "src/racecheck.tr"
    long long wi = 0LL;
#line 1108 "src/racecheck.tr"
    while ((wi < tf->wroot->len)) {
#line 1109 "src/racecheck.tr"
        if (((({ TrStr _cmpa_t3618 = (List_TrStr_get(tf->wroot, wi)); TrStr _cmpb_t3619 = (pname); _Bool _cmpr_t3620 = _tr_str_eqv(_cmpa_t3618, _cmpb_t3619); _tr_str_release(_cmpa_t3618); _cmpr_t3620; }) && ({ TrStr _cmpa_t3621 = (List_TrStr_get(tf->wprop, wi)); TrStr _cmpb_t3622 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3623 = !_tr_str_eqv(_cmpa_t3621, _cmpb_t3622); _tr_str_release(_cmpa_t3621); _cmpr_t3623; })) && ({ TrStr _at_t3624 = (List_TrStr_get(tf->wprop, wi)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3624)); _tr_str_release(_at_t3624); _wr; }))) {
#line 1109 "src/racecheck.tr"
            TrStr _trret594 = List_TrStr_get(tf->wprop, wi);
            return _trret594;
        }
#line 1110 "src/racecheck.tr"
        wi = (wi + 1LL);
    }
#line 1111 "src/racecheck.tr"
    long long mi = 0LL;
#line 1112 "src/racecheck.tr"
    while ((mi < tf->mcrecv->len)) {
#line 1113 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3625 = (List_TrStr_get(tf->mcrecv, mi)); TrStr _cmpb_t3626 = (pname); _Bool _cmpr_t3627 = _tr_str_eqv(_cmpa_t3625, _cmpb_t3626); _tr_str_release(_cmpa_t3625); _cmpr_t3627; })) {
#line 1114 "src/racecheck.tr"
            long long mf = ({ TrStr _at_t3628 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(tf->mcname, mi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3628)); _tr_str_release(_at_t3628); _wr; });
#line 1115 "src/racecheck.tr"
            if ((mf >= 0LL)) {
#line 1116 "src/racecheck.tr"
                RcFn* mfn = ((RcFn*)List_ptr_get(self->fns, mf));
#line 1117 "src/racecheck.tr"
                long long k = 0LL;
#line 1118 "src/racecheck.tr"
                while ((k < mfn->wroot->len)) {
#line 1119 "src/racecheck.tr"
                    if (((({ TrStr _cmpa_t3629 = (List_TrStr_get(mfn->wroot, k)); TrStr _cmpb_t3630 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3631 = _tr_str_eqv(_cmpa_t3629, _cmpb_t3630); _tr_str_release(_cmpa_t3629); _cmpr_t3631; }) && ({ TrStr _cmpa_t3632 = (List_TrStr_get(mfn->wprop, k)); TrStr _cmpb_t3633 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3634 = !_tr_str_eqv(_cmpa_t3632, _cmpb_t3633); _tr_str_release(_cmpa_t3632); _cmpr_t3634; })) && ({ TrStr _at_t3635 = (List_TrStr_get(mfn->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3635)); _tr_str_release(_at_t3635); _wr; }))) {
#line 1119 "src/racecheck.tr"
                        TrStr _trret595 = List_TrStr_get(mfn->wprop, k);
                        return _trret595;
                    }
#line 1120 "src/racecheck.tr"
                    k = (k + 1LL);
                }
            }
        }
#line 1121 "src/racecheck.tr"
        mi = (mi + 1LL);
    }
#line 1122 "src/racecheck.tr"
    if ((depth < 4LL)) {
#line 1123 "src/racecheck.tr"
        long long ci = 0LL;
#line 1124 "src/racecheck.tr"
        while ((ci < tf->ca_fn->len)) {
#line 1125 "src/racecheck.tr"
            if (({ TrStr _cmpa_t3636 = (List_TrStr_get(tf->ca_name, ci)); TrStr _cmpb_t3637 = (pname); _Bool _cmpr_t3638 = _tr_str_eqv(_cmpa_t3636, _cmpb_t3637); _tr_str_release(_cmpa_t3636); _cmpr_t3638; })) {
#line 1126 "src/racecheck.tr"
                long long ni = ({ TrStr _at_t3639 = (List_TrStr_get(tf->ca_fn, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3639)); _tr_str_release(_at_t3639); _wr; });
#line 1127 "src/racecheck.tr"
                if (((ni >= 0LL) && ({ __auto_type _ord_t3640 = (List_i64_get(tf->ca_idx, ci)); __auto_type _wr = ((_ord_t3640 < ((RcFn*)List_ptr_get(self->fns, ni))->params->len)); _wr; }))) {
#line 1128 "src/racecheck.tr"
                    TrStr r = ({ __auto_type _ord_t3641 = (List_i64_get(tf->ca_idx, ci)); TrStr _at_t3642 = (List_TrStr_get(((RcFn*)List_ptr_get(self->fns, ni))->params, _ord_t3641)); __auto_type _wr = (RcCk_param_write_prop(self, ni, _at_t3642, cn, (depth + 1LL))); _tr_str_release(_at_t3642); _wr; });
#line 1129 "src/racecheck.tr"
                    if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 1129 "src/racecheck.tr"
                        TrStr _trret596 = _tr_str_retain(r);
                        _tr_str_release(r);
                        return _trret596;
                    }
                }
            }
#line 1130 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
    }
#line 1131 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void RcCk_check_shared_writes(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, long long ai, TrStr an) {
#line 1134 "src/racecheck.tr"
    TrStr pn = List_TrStr_get(tf->params, ai);
#line 1135 "src/racecheck.tr"
    TrStr cn = List_TrStr_get(tf->ptypes, ai);
#line 1136 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("Shared", 6LL)))) {
#line 1136 "src/racecheck.tr"
        TrStr _strtmp_t3643 = List_TrStr_get(tf->pinner, ai);
        _tr_str_release(cn);
        cn = _strtmp_t3643;
    }
#line 1137 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1138 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 1139 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3644 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3644)); _tr_str_release(_apt_t3644); _wr; });
#line 1140 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 1140 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 1141 "src/racecheck.tr"
    TrStr pr = RcCk_param_write_prop(self, ti, pn, cn, 0LL);
#line 1142 "src/racecheck.tr"
    if ((!_tr_str_eqv((pr), (_tr_str_lit_len("", 0LL))))) {
#line 1142 "src/racecheck.tr"
        RcCk_shared_msg(self, f, s, tf, cn, pr, an);
    }
    _tr_str_release(pn);
    _tr_str_release(cn);
    _tr_str_release(pr);
}

__attribute__((hot)) void RcCk_shared_msg(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, TrStr cn, TrStr prop, TrStr an) {
#line 1145 "src/racecheck.tr"
    ({ __auto_type _ord_t3645 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3646 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3645); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' mutates the plain field '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is shared by several threads: `Shared[T]` only makes the reference COUNT atomic, not the fields, so the concurrent writes race.\n      FIX: make '", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (or guard the whole object with `Mutex[", 56LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`), or give each thread its own value.", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3646); _tr_str_release(_ord_t3646); });
}

__attribute__((hot)) TrStr RcCk_where2(RcCk* self, RcFn* f, long long line) {
#line 1148 "src/racecheck.tr"
    long long l = line;
#line 1149 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 1149 "src/racecheck.tr"
        l = f->line;
    }
#line 1150 "src/racecheck.tr"
    return ({ __auto_type _ord_t3647 = (_tr_strx_concatv((f->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3647); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) bool _rc_spawned(RcCk* ck, TrStr name) {
#line 22 "src/racecheck.tr"
    long long i = 0LL;
#line 23 "src/racecheck.tr"
    while ((i < ck->fns->len)) {
#line 24 "src/racecheck.tr"
        RcFn* f = ((RcFn*)List_ptr_get(ck->fns, i));
#line 25 "src/racecheck.tr"
        long long j = 0LL;
#line 26 "src/racecheck.tr"
        while ((j < f->sites->len)) {
#line 27 "src/racecheck.tr"
            if (_tr_str_eqv((((RcSite*)List_ptr_get(f->sites, j))->target), (name))) {
#line 27 "src/racecheck.tr"
                return true;
            }
#line 28 "src/racecheck.tr"
            j = (j + 1LL);
        }
#line 29 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 30 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) RcFn* _rc_new_fn(TrStr name, TrStr file, long long line, bool is_lib) {
#line 78 "src/racecheck.tr"
    RcFn* f = ((RcFn*)_tr_obj_alloc(sizeof(RcFn)));
#line 79 "src/racecheck.tr"
    TrStr _sfo_t3648 = f->name;
    f->name = _tr_str_retain(name);
    _tr_str_release(_sfo_t3648);
#line 80 "src/racecheck.tr"
    TrStr _sfo_t3649 = f->file;
    f->file = _tr_str_retain(file);
    _tr_str_release(_sfo_t3649);
#line 81 "src/racecheck.tr"
    f->line = line;
#line 82 "src/racecheck.tr"
    f->is_lib = is_lib;
#line 83 "src/racecheck.tr"
    f->params = (void*)List_TrStr_new();
#line 84 "src/racecheck.tr"
    f->ptypes = (void*)List_TrStr_new();
#line 85 "src/racecheck.tr"
    f->pinner = (void*)List_TrStr_new();
#line 86 "src/racecheck.tr"
    f->locals = (void*)List_TrStr_new();
#line 87 "src/racecheck.tr"
    f->ltn = (void*)List_TrStr_new();
#line 88 "src/racecheck.tr"
    f->ltt = (void*)List_TrStr_new();
#line 89 "src/racecheck.tr"
    f->ltarg = (void*)List_TrStr_new();
#line 90 "src/racecheck.tr"
    f->ldepth = (void*)List_i64_new();
#line 91 "src/racecheck.tr"
    f->wafter = (void*)List_bool_new();
#line 92 "src/racecheck.tr"
    f->idents = (void*)List_TrStr_new();
#line 93 "src/racecheck.tr"
    f->ca_fn = (void*)List_TrStr_new();
#line 94 "src/racecheck.tr"
    f->ca_idx = (void*)List_i64_new();
#line 95 "src/racecheck.tr"
    f->ca_name = (void*)List_TrStr_new();
#line 96 "src/racecheck.tr"
    f->thr_made = (void*)List_TrStr_new();
#line 97 "src/racecheck.tr"
    f->thr_line = (void*)List_i64_new();
#line 98 "src/racecheck.tr"
    f->pairs_a = (void*)List_TrStr_new();
#line 99 "src/racecheck.tr"
    f->pairs_b = (void*)List_TrStr_new();
#line 100 "src/racecheck.tr"
    f->touched = (void*)List_TrStr_new();
#line 101 "src/racecheck.tr"
    f->touched_w = (void*)List_bool_new();
#line 102 "src/racecheck.tr"
    f->touched_after = (void*)List_bool_new();
#line 103 "src/racecheck.tr"
    f->touched_line = (void*)List_i64_new();
#line 104 "src/racecheck.tr"
    f->calls = (void*)List_TrStr_new();
#line 105 "src/racecheck.tr"
    f->sites = (void*)List_ptr_new();
#line 106 "src/racecheck.tr"
    f->closures = (void*)List_TrStr_new();
#line 107 "src/racecheck.tr"
    f->shvars = (void*)List_TrStr_new();
#line 108 "src/racecheck.tr"
    f->wroot = (void*)List_TrStr_new();
#line 109 "src/racecheck.tr"
    f->wprop = (void*)List_TrStr_new();
#line 110 "src/racecheck.tr"
    f->mcrecv = (void*)List_TrStr_new();
#line 111 "src/racecheck.tr"
    f->mcname = (void*)List_TrStr_new();
#line 112 "src/racecheck.tr"
    f->msgs = (void*)List_TrStr_new();
#line 113 "src/racecheck.tr"
    return f;
}

__attribute__((hot)) bool _rc_sync_ty(TrStr n) {
#line 131 "src/racecheck.tr"
    return (((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("WaitGroup", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Semaphore", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Once", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Barrier", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Condvar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL))));
}

__attribute__((hot)) bool _rc_mut_method(TrStr m) {
#line 134 "src/racecheck.tr"
    return (((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("update", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("delete", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_back", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop_back", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("truncate", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL))));
}

__attribute__((hot)) bool _rc_has(List_TrStr* v, TrStr s) {
#line 137 "src/racecheck.tr"
    long long i = 0LL;
#line 138 "src/racecheck.tr"
    while ((i < v->len)) {
#line 139 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3650 = (List_TrStr_get(v, i)); TrStr _cmpb_t3651 = (s); _Bool _cmpr_t3652 = _tr_str_eqv(_cmpa_t3650, _cmpb_t3651); _tr_str_release(_cmpa_t3650); _cmpr_t3652; })) {
#line 139 "src/racecheck.tr"
            return true;
        }
#line 140 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 141 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_nul(Expr* p) {
#line 144 "src/racecheck.tr"
    return (((unsigned long long)(p)) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr _rc_root(Expr* e) {
#line 148 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 148 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 149 "src/racecheck.tr"
    __auto_type _t3653 = (*e);
    if (_t3653.tag == Expr_EIdent) {
        __auto_type n = _t3653.data.EIdent.name;
        TrStr _trret597 = _tr_str_retain(n);
        return _trret597;
    } else if (_t3653.tag == Expr_EPropAccess) {
        __auto_type o = _t3653.data.EPropAccess.obj;
        return _rc_root(o);
    } else if (_t3653.tag == Expr_EIndex) {
        __auto_type o = _t3653.data.EIndex.obj;
        return _rc_root(o);
    } else if (1) {
        __auto_type _ = _t3653;
        return _tr_str_lit_len("", 0LL);
    }
#line 154 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_first_prop(Expr* e) {
#line 158 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 158 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 159 "src/racecheck.tr"
    __auto_type _t3654 = (*e);
    if (_t3654.tag == Expr_EPropAccess) {
        __auto_type o = _t3654.data.EPropAccess.obj;
__auto_type p = _t3654.data.EPropAccess.prop;
#line 161 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 162 "src/racecheck.tr"
            __auto_type _t3655 = (*o);
            if (_t3655.tag == Expr_EIdent) {
                __auto_type _ = _t3655.data.EIdent.name;
                TrStr _trret598 = _tr_str_retain(p);
                return _trret598;
            } else if (1) {
                __auto_type _ = _t3655;
                /* pass */
            }
        }
#line 165 "src/racecheck.tr"
        TrStr _trret599 = _rc_first_prop(o);
        return _trret599;
    } else if (_t3654.tag == Expr_EIndex) {
        __auto_type o = _t3654.data.EIndex.obj;
        return _rc_first_prop(o);
    } else if (1) {
        __auto_type _ = _t3654;
        return _tr_str_lit_len("", 0LL);
    }
#line 168 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_ctor_name(Expr* v) {
#line 172 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 172 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 173 "src/racecheck.tr"
    __auto_type _t3656 = (*v);
    if (_t3656.tag == Expr_EMethodCall) {
        __auto_type o = _t3656.data.EMethodCall.obj;
        return _rc_root(o);
    } else if (_t3656.tag == Expr_ECall) {
        __auto_type c = _t3656.data.ECall.callee;
        return _rc_root(c);
    } else if (1) {
        __auto_type _ = _t3656;
        return _tr_str_lit_len("", 0LL);
    }
#line 177 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_ctor_arg(Expr* v) {
#line 181 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 181 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 182 "src/racecheck.tr"
    __auto_type _t3657 = (*v);
    if (_t3657.tag == Expr_EMethodCall) {
        __auto_type o = _t3657.data.EMethodCall.obj;
__auto_type margs = _t3657.data.EMethodCall.args;
#line 184 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 185 "src/racecheck.tr"
            __auto_type _t3658 = (*o);
            if (_t3658.tag == Expr_EIndex) {
                __auto_type ix = _t3658.data.EIndex._tr_v_index;
#line 187 "src/racecheck.tr"
                if ((!_rc_nul(ix))) {
#line 188 "src/racecheck.tr"
                    __auto_type _t3659 = (*ix);
                    if (_t3659.tag == Expr_EIdent) {
                        __auto_type an = _t3659.data.EIdent.name;
                        TrStr _trret600 = _tr_str_retain(an);
                        return _trret600;
                    } else if (1) {
                        __auto_type _ = _t3659;
                        return _tr_str_lit_len("", 0LL);
                    }
                }
            } else if (_t3658.tag == Expr_EIdent) {
                __auto_type on = _t3658.data.EIdent.name;
#line 193 "src/racecheck.tr"
                if ((_tr_str_eqv((on), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((on), (_tr_str_lit_len("RwLock", 6LL))))) {
#line 194 "src/racecheck.tr"
                    if (((margs->len > 0LL) && (!_rc_nul(((Expr*)List_ptr_get(margs, 0LL)))))) {
#line 195 "src/racecheck.tr"
                        __auto_type _t3660 = (*((Expr*)List_ptr_get(margs, 0LL)));
                        if (_t3660.tag == Expr_ECall) {
                            __auto_type pc = _t3660.data.ECall.callee;
#line 197 "src/racecheck.tr"
                            if ((!_rc_nul(pc))) {
#line 198 "src/racecheck.tr"
                                __auto_type _t3661 = (*pc);
                                if (_t3661.tag == Expr_EIdent) {
                                    __auto_type pcn = _t3661.data.EIdent.name;
                                    TrStr _trret601 = _tr_str_retain(pcn);
                                    return _trret601;
                                } else if (1) {
                                    __auto_type _ = _t3661;
                                    return _tr_str_lit_len("", 0LL);
                                }
                            }
                        } else if (_t3660.tag == Expr_EList) {
                            __auto_type _ = _t3660.data.EList.items;
                            return _tr_str_lit_len("List", 4LL);
                        } else if (_t3660.tag == Expr_EDict) {
                            return _tr_str_lit_len("Dict", 4LL);
                        } else if (1) {
                            __auto_type _ = _t3660;
                            return _tr_str_lit_len("", 0LL);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t3658;
                return _tr_str_lit_len("", 0LL);
            }
        }
    } else if (1) {
        __auto_type _ = _t3657;
        return _tr_str_lit_len("", 0LL);
    }
#line 206 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long _rc_dot(TrStr n) {
#line 209 "src/racecheck.tr"
    long long i = 0LL;
#line 210 "src/racecheck.tr"
    while ((i < _tr_str_lenv((n)))) {
#line 211 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3662 = (_tr_str_slicev((n), i, (i + 1LL))); TrStr _cmpb_t3663 = (_tr_str_lit_len(".", 1LL)); _Bool _cmpr_t3664 = _tr_str_eqv(_cmpa_t3662, _cmpb_t3663); _tr_str_release(_cmpa_t3662); _cmpr_t3664; })) {
#line 211 "src/racecheck.tr"
            return i;
        }
#line 212 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 213 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool _rc_is_thread_spawn(Expr* v) {
#line 216 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 216 "src/racecheck.tr"
        return false;
    }
#line 217 "src/racecheck.tr"
    __auto_type _t3665 = (*v);
    if (_t3665.tag == Expr_EMethodCall) {
        __auto_type o = _t3665.data.EMethodCall.obj;
__auto_type m = _t3665.data.EMethodCall.method;
#line 219 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && ({ TrStr _cmpa_t3666 = (_rc_root(o)); TrStr _cmpb_t3667 = (_tr_str_lit_len("Thread", 6LL)); _Bool _cmpr_t3668 = _tr_str_eqv(_cmpa_t3666, _cmpb_t3667); _tr_str_release(_cmpa_t3666); _cmpr_t3668; }))) {
#line 219 "src/racecheck.tr"
            return true;
        }
    } else if (1) {
        __auto_type _ = _t3665;
        /* pass */
    }
#line 221 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_is_lambda(Expr* v) {
#line 224 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 224 "src/racecheck.tr"
        return false;
    }
#line 225 "src/racecheck.tr"
    __auto_type _t3669 = (*v);
    if (_t3669.tag == Expr_EClosure) {
        return true;
    } else if (1) {
        __auto_type _ = _t3669;
        return false;
    }
#line 228 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_path_lib(TrStr p) {
#line 1154 "src/racecheck.tr"
    return (((_tr_str_containsv((p), (_tr_str_lit_len("std/", 4LL))) || _tr_str_containsv((p), (_tr_str_lit_len("core/", 5LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("std\\", 4LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("core\\", 5LL))));
}

__attribute__((hot)) bool _rc_ifaces_sendable(ClassDef* c) {
#line 1157 "src/racecheck.tr"
    long long i = 0LL;
#line 1158 "src/racecheck.tr"
    while ((i < c->iface_names->len)) {
#line 1159 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3670 = (List_TrStr_get(c->iface_names, i)); TrStr _cmpb_t3671 = (_tr_str_lit_len("Sendable", 8LL)); _Bool _cmpr_t3672 = _tr_str_eqv(_cmpa_t3670, _cmpb_t3671); _tr_str_release(_cmpa_t3670); _cmpr_t3672; })) {
#line 1159 "src/racecheck.tr"
            return true;
        }
#line 1160 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 1161 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_TrStr* check_data_races(Program* prog, List_TrStr* decl_files, TrStr input_path) {
#line 1164 "src/racecheck.tr"
    RcCk* ck = ((RcCk*)_tr_obj_alloc(sizeof(RcCk)));
#line 1165 "src/racecheck.tr"
    ck->held_for = _tr_dict_new(8LL);
#line 1166 "src/racecheck.tr"
    ck->fns = (void*)List_ptr_new();
#line 1167 "src/racecheck.tr"
    ck->classes = (void*)List_ptr_new();
#line 1168 "src/racecheck.tr"
    ck->class_lib = (void*)List_bool_new();
#line 1169 "src/racecheck.tr"
    ck->gnames = (void*)List_TrStr_new();
#line 1170 "src/racecheck.tr"
    ck->gfiles = (void*)List_TrStr_new();
#line 1171 "src/racecheck.tr"
    ck->gsync = (void*)List_bool_new();
#line 1172 "src/racecheck.tr"
    ck->gtn = (void*)List_TrStr_new();
#line 1173 "src/racecheck.tr"
    ck->msgs = (void*)List_TrStr_new();
#line 1174 "src/racecheck.tr"
    RcFn* _cltmp_t3673 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3673;
#line 1175 "src/racecheck.tr"
    RcFn* _cltmp_t3674 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3674;
#line 1176 "src/racecheck.tr"
    long long i = 0LL;
#line 1177 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1178 "src/racecheck.tr"
        TrStr dfc = _tr_str_retain(input_path);
#line 1179 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1179 "src/racecheck.tr"
            TrStr _strtmp_t3675 = List_TrStr_get(decl_files, i);
            _tr_str_release(dfc);
            dfc = _strtmp_t3675;
        }
#line 1180 "src/racecheck.tr"
        __auto_type _t3676 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3676.tag == Decl_DClass) {
            __auto_type c = _t3676.data.DClass.cls;
#line 1182 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c));
#line 1183 "src/racecheck.tr"
            ({ __auto_type _ord_t3677 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3677); });
#line 1184 "src/racecheck.tr"
            if ((!_rc_path_lib(dfc))) {
#line 1185 "src/racecheck.tr"
                long long fq = 0LL;
#line 1186 "src/racecheck.tr"
                while ((fq < c->fields->len)) {
#line 1187 "src/racecheck.tr"
                    ({ __auto_type _ord_t3678 = (((FieldDef*)List_ptr_get(c->fields, fq))->ty); __auto_type _ord_t3679 = (c->line); RcCk_ty_check(ck, _ord_t3678, dfc, _ord_t3679); });
#line 1188 "src/racecheck.tr"
                    fq = (fq + 1LL);
                }
            }
        } else if (_t3676.tag == Decl_DActor) {
            __auto_type c2 = _t3676.data.DActor.cls;
#line 1190 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c2));
#line 1191 "src/racecheck.tr"
            ({ __auto_type _ord_t3680 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3680); });
        } else if (1) {
            __auto_type _ = _t3676;
            /* pass */
        }
#line 1193 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(dfc);
    }
#line 1194 "src/racecheck.tr"
    i = 0LL;
#line 1195 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1196 "src/racecheck.tr"
        TrStr df = _tr_str_retain(input_path);
#line 1197 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1197 "src/racecheck.tr"
            TrStr _strtmp_t3681 = List_TrStr_get(decl_files, i);
            _tr_str_release(df);
            df = _strtmp_t3681;
        }
#line 1198 "src/racecheck.tr"
        bool lib = _rc_path_lib(df);
#line 1199 "src/racecheck.tr"
        __auto_type _t3682 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3682.tag == Decl_DTopLevelStmt) {
            __auto_type sp = _t3682.data.DTopLevelStmt.stmt;
#line 1201 "src/racecheck.tr"
            __auto_type _t3683 = (*sp);
            if (_t3683.tag == Stmt_SLet) {
                __auto_type name = _t3683.data.SLet.name;
__auto_type is_mut = _t3683.data.SLet.is_mut;
__auto_type is_const = _t3683.data.SLet.is_const;
__auto_type ty = _t3683.data.SLet.ty;
__auto_type v = _t3683.data.SLet.val;
#line 1203 "src/racecheck.tr"
                if ((!lib)) {
#line 1203 "src/racecheck.tr"
                    RcCk_add_global(ck, name, is_mut, is_const, ty, v, df);
                }
            } else if (_t3683.tag == Stmt_SAssign) {
                __auto_type tg = _t3683.data.SAssign.target;
__auto_type v3 = _t3683.data.SAssign.val;
#line 1206 "src/racecheck.tr"
                if (((!lib) && (!_rc_nul(tg)))) {
#line 1207 "src/racecheck.tr"
                    __auto_type _t3684 = (*tg);
                    if (_t3684.tag == Expr_EIdent) {
                        __auto_type gn = _t3684.data.EIdent.name;
                        RcCk_add_global(ck, gn, false, false, (AstType**)(0LL), v3, df);
                    } else if (1) {
                        __auto_type _ = _t3684;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t3683;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t3682;
            /* pass */
        }
#line 1212 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df);
    }
#line 1213 "src/racecheck.tr"
    i = 0LL;
#line 1214 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1215 "src/racecheck.tr"
        TrStr df2 = _tr_str_retain(input_path);
#line 1216 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1216 "src/racecheck.tr"
            TrStr _strtmp_t3685 = List_TrStr_get(decl_files, i);
            _tr_str_release(df2);
            df2 = _strtmp_t3685;
        }
#line 1217 "src/racecheck.tr"
        bool lib2 = _rc_path_lib(df2);
#line 1218 "src/racecheck.tr"
        __auto_type _t3686 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3686.tag == Decl_DFunction) {
            __auto_type f = _t3686.data.DFunction.func;
            RcCk_add_fn(ck, f, df2, lib2);
        } else if (_t3686.tag == Decl_DClass) {
            __auto_type c3 = _t3686.data.DClass.cls;
#line 1221 "src/racecheck.tr"
            long long mi = 0LL;
#line 1222 "src/racecheck.tr"
            while ((mi < c3->methods->len)) {
#line 1223 "src/racecheck.tr"
                ({ __auto_type _ord_t3687 = (((FunctionDef*)List_ptr_get(c3->methods, mi))); __auto_type _ord_t3688 = (c3->name); RcCk_add_method(ck, _ord_t3687, _ord_t3688, df2, lib2); });
#line 1224 "src/racecheck.tr"
                mi = (mi + 1LL);
            }
        } else if (_t3686.tag == Decl_DActor) {
            __auto_type c4 = _t3686.data.DActor.cls;
#line 1226 "src/racecheck.tr"
            long long mi2 = 0LL;
#line 1227 "src/racecheck.tr"
            while ((mi2 < c4->methods->len)) {
#line 1228 "src/racecheck.tr"
                ({ __auto_type _ord_t3689 = (((FunctionDef*)List_ptr_get(c4->methods, mi2))); __auto_type _ord_t3690 = (c4->name); RcCk_add_method(ck, _ord_t3689, _ord_t3690, df2, lib2); });
#line 1229 "src/racecheck.tr"
                mi2 = (mi2 + 1LL);
            }
        } else if (_t3686.tag == Decl_DExtend) {
            __auto_type tgt = _t3686.data.DExtend.target;
__auto_type methods = _t3686.data.DExtend.methods;
#line 1231 "src/racecheck.tr"
            long long mi3 = 0LL;
#line 1232 "src/racecheck.tr"
            while ((mi3 < methods->len)) {
#line 1233 "src/racecheck.tr"
                RcCk_add_method(ck, ((FunctionDef*)List_ptr_get(methods, mi3)), tgt, df2, lib2);
#line 1234 "src/racecheck.tr"
                mi3 = (mi3 + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t3686;
            /* pass */
        }
#line 1236 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df2);
    }
#line 1237 "src/racecheck.tr"
    i = 0LL;
#line 1238 "src/racecheck.tr"
    while ((i < ck->fns->len)) {
#line 1239 "src/racecheck.tr"
        RcFn* f = ((RcFn*)List_ptr_get(ck->fns, i));
#line 1240 "src/racecheck.tr"
        if ((!f->is_lib)) {
#line 1241 "src/racecheck.tr"
            long long mj = 0LL;
#line 1242 "src/racecheck.tr"
            while ((mj < f->msgs->len)) {
#line 1243 "src/racecheck.tr"
                ({ TrStr _ord_t3691 = (List_TrStr_get(f->msgs, mj)); List_TrStr_append(ck->msgs, _ord_t3691); _tr_str_release(_ord_t3691); });
#line 1244 "src/racecheck.tr"
                mj = (mj + 1LL);
            }
#line 1245 "src/racecheck.tr"
            long long sj = 0LL;
#line 1246 "src/racecheck.tr"
            while ((sj < f->sites->len)) {
#line 1247 "src/racecheck.tr"
                RcCk_analyze_site(ck, f, ((RcSite*)List_ptr_get(f->sites, sj)));
#line 1248 "src/racecheck.tr"
                sj = (sj + 1LL);
            }
        }
#line 1249 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 1251 "src/racecheck.tr"
    long long pi = 0LL;
#line 1252 "src/racecheck.tr"
    while ((pi < ck->fns->len)) {
#line 1253 "src/racecheck.tr"
        RcFn* pf = ((RcFn*)List_ptr_get(ck->fns, pi));
#line 1254 "src/racecheck.tr"
        long long pk = 0LL;
#line 1255 "src/racecheck.tr"
        while ((pk < pf->pairs_a->len)) {
#line 1256 "src/racecheck.tr"
            long long qi = (pi + 1LL);
#line 1257 "src/racecheck.tr"
            while ((qi < ck->fns->len)) {
#line 1258 "src/racecheck.tr"
                RcFn* qf = ((RcFn*)List_ptr_get(ck->fns, qi));
#line 1259 "src/racecheck.tr"
                long long qk = 0LL;
#line 1260 "src/racecheck.tr"
                while ((qk < qf->pairs_a->len)) {
#line 1261 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3692 = (List_TrStr_get(qf->pairs_a, qk)); __auto_type _wr = (({ TrStr _cmpa_t3693 = (_ord_t3692); TrStr _cmpb_t3694 = (List_TrStr_get(pf->pairs_b, pk)); _Bool _cmpr_t3695 = _tr_str_eqv(_cmpa_t3693, _cmpb_t3694); _tr_str_release(_cmpa_t3693); _tr_str_release(_cmpb_t3694); _cmpr_t3695; })); _wr; }) && ({ __auto_type _ord_t3696 = (List_TrStr_get(qf->pairs_b, qk)); __auto_type _wr = (({ TrStr _cmpa_t3697 = (_ord_t3696); TrStr _cmpb_t3698 = (List_TrStr_get(pf->pairs_a, pk)); _Bool _cmpr_t3699 = _tr_str_eqv(_cmpa_t3697, _cmpb_t3698); _tr_str_release(_cmpa_t3697); _tr_str_release(_cmpb_t3698); _cmpr_t3699; })); _wr; }))) {
#line 1265 "src/racecheck.tr"
                        TrStr inv_pre = _tr_str_lit_len("W|", 2LL);
#line 1266 "src/racecheck.tr"
                        if ((({ TrStr _apt_t3700 = _tr_str_retain(pf->name); __auto_type _wr = (_rc_spawned(ck, _apt_t3700)); _tr_str_release(_apt_t3700); _wr; }) && ({ TrStr _apt_t3701 = _tr_str_retain(qf->name); __auto_type _wr = (_rc_spawned(ck, _apt_t3701)); _tr_str_release(_apt_t3701); _wr; }))) {
#line 1266 "src/racecheck.tr"
                            TrStr _strtmp_t3702 = _tr_str_lit_len("", 0LL);
                            _tr_str_release(inv_pre);
                            inv_pre = _strtmp_t3702;
                        }
#line 1267 "src/racecheck.tr"
                        ({ __auto_type _ord_t3703 = (({ TrStr _tr_cl = (_tr_strx_concatv((inv_pre), (pf->file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3704 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3703); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pf->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] lock-order inversion: '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3705 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3704); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks '", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3706 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3705); TrStr _tr_cr = (List_TrStr_get(pf->pairs_a, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' then '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3707 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3706); TrStr _tr_cr = (List_TrStr_get(pf->pairs_b, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3708 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3707); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (qf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks them in the opposite order: two threads running them can deadlock.\n      FIX: always acquire the mutexes in one global order.", 133LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(ck->msgs, _ord_t3708); _tr_str_release(_ord_t3708); });
                    }
#line 1268 "src/racecheck.tr"
                    qk = (qk + 1LL);
                }
#line 1269 "src/racecheck.tr"
                qi = (qi + 1LL);
            }
#line 1270 "src/racecheck.tr"
            pk = (pk + 1LL);
        }
#line 1271 "src/racecheck.tr"
        pi = (pi + 1LL);
    }
#line 1272 "src/racecheck.tr"
    return ck->msgs;
}

