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
#line 208 "src/racecheck.tr"
    if ((((unsigned long long)(t)) == ((unsigned long long)(0LL)))) {
#line 208 "src/racecheck.tr"
        return;
    }
#line 209 "src/racecheck.tr"
    AstType* ty = (*t);
#line 210 "src/racecheck.tr"
    bool coll = ((((_tr_str_eqv((ty->name), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((ty->name), (_tr_str_lit_len("Map", 3LL))));
#line 211 "src/racecheck.tr"
    long long i = 0LL;
#line 212 "src/racecheck.tr"
    while ((i < ty->args->len)) {
#line 213 "src/racecheck.tr"
        AstType** a = ((AstType**)List_ptr_get(ty->args, i));
#line 214 "src/racecheck.tr"
        if (((coll && (((unsigned long long)(a)) != ((unsigned long long)(0LL)))) && (_tr_str_eqv(((*a)->name), (_tr_str_lit_len("Shared", 6LL))) || _tr_str_eqv(((*a)->name), (_tr_str_lit_len("Weak", 4LL)))))) {
#line 215 "src/racecheck.tr"
            ({ __auto_type _ord_t3147 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((file), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-11] '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3148 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3147); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot hold '", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3149 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3148); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*a)->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[..]' elements: reference-counted shared handles are not supported as collection elements.\n      FIX: wrap the handle in a small class (`class Handle: pub inner: Shared[T]`) and store that, or keep the handles in separate variables.", 232LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3149); _tr_str_release(_ord_t3149); });
#line 216 "src/racecheck.tr"
            return;
        }
#line 217 "src/racecheck.tr"
        RcCk_ty_check(self, a, file, line);
#line 218 "src/racecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) TrStr RcCk_where(RcCk* self, long long line) {
#line 221 "src/racecheck.tr"
    long long l = line;
#line 222 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 222 "src/racecheck.tr"
        l = self->cur->line;
    }
#line 223 "src/racecheck.tr"
    return ({ __auto_type _ord_t3150 = (_tr_strx_concatv((self->cur->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3150); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) TrStr RcCk_ltype(RcCk* self, RcFn* f, TrStr name) {
#line 226 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 227 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 228 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3151 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t3152 = (name); _Bool _cmpr_t3153 = _tr_str_eqv(_cmpa_t3151, _cmpb_t3152); _tr_str_release(_cmpa_t3151); _cmpr_t3153; })) {
#line 228 "src/racecheck.tr"
            return List_TrStr_get(f->ptypes, i);
        }
#line 229 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 230 "src/racecheck.tr"
    i = (f->ltn->len - 1LL);
#line 231 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 232 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3154 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3155 = (name); _Bool _cmpr_t3156 = _tr_str_eqv(_cmpa_t3154, _cmpb_t3155); _tr_str_release(_cmpa_t3154); _cmpr_t3156; })) {
#line 232 "src/racecheck.tr"
            return List_TrStr_get(f->ltt, i);
        }
#line 233 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 234 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr RcCk_ltarg_of(RcCk* self, RcFn* f, TrStr name) {
#line 237 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 238 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 239 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3157 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t3158 = (name); _Bool _cmpr_t3159 = _tr_str_eqv(_cmpa_t3157, _cmpb_t3158); _tr_str_release(_cmpa_t3157); _cmpr_t3159; })) {
#line 239 "src/racecheck.tr"
            return List_TrStr_get(f->pinner, i);
        }
#line 240 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 241 "src/racecheck.tr"
    i = (f->ltn->len - 1LL);
#line 242 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 243 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3160 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3161 = (name); _Bool _cmpr_t3162 = _tr_str_eqv(_cmpa_t3160, _cmpb_t3161); _tr_str_release(_cmpa_t3160); _cmpr_t3162; })) {
#line 243 "src/racecheck.tr"
            return List_TrStr_get(f->ltarg, i);
        }
#line 244 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 245 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr RcCk_lclass(RcCk* self, RcFn* f, TrStr name) {
#line 249 "src/racecheck.tr"
    TrStr t = RcCk_ltype(self, f, name);
#line 250 "src/racecheck.tr"
    if (_tr_str_eqv((t), (_tr_str_lit_len("Shared", 6LL)))) {
#line 250 "src/racecheck.tr"
        TrStr _trret562 = RcCk_ltarg_of(self, f, name);
        _tr_str_release(t);
        return _trret562;
    }
#line 251 "src/racecheck.tr"
    TrStr _trret563 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret563;
}

__attribute__((hot)) long long RcCk_ldepth_of(RcCk* self, RcFn* f, TrStr name) {
#line 254 "src/racecheck.tr"
    if (_rc_has(f->params, name)) {
#line 254 "src/racecheck.tr"
        return 0LL;
    }
#line 255 "src/racecheck.tr"
    long long i = (f->ltn->len - 1LL);
#line 256 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 257 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3163 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t3164 = (name); _Bool _cmpr_t3165 = _tr_str_eqv(_cmpa_t3163, _cmpb_t3164); _tr_str_release(_cmpa_t3163); _cmpr_t3165; })) {
#line 257 "src/racecheck.tr"
            return List_i64_get(f->ldepth, i);
        }
#line 258 "src/racecheck.tr"
        i = (i - 1LL);
    }
#line 259 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool RcCk_mentions(RcCk* self, Stmt* sp, TrStr name) {
#line 263 "src/racecheck.tr"
    RcFn* tmp = ({ TrStr _apt_t3166 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t3166, self->cur->line, true)); _tr_str_release(_apt_t3166); _wr; });
#line 264 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 265 "src/racecheck.tr"
    long long sl = self->cur_line;
#line 266 "src/racecheck.tr"
    bool ss = self->seen_spawn;
#line 267 "src/racecheck.tr"
    RcFn* _cltmp_t3167 = _tr_obj_retain(tmp);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3167;
#line 268 "src/racecheck.tr"
    RcCk_stmt(self, sp);
#line 269 "src/racecheck.tr"
    RcFn* _cltmp_t3168 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3168;
#line 270 "src/racecheck.tr"
    self->cur_line = sl;
#line 271 "src/racecheck.tr"
    self->seen_spawn = ss;
#line 272 "src/racecheck.tr"
    bool _trret564 = _rc_has(tmp->idents, name);
    _tr_obj_release(tmp, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
    return _trret564;
}

__attribute__((hot)) void RcCk_note(RcCk* self, TrStr name, bool w) {
#line 275 "src/racecheck.tr"
    if ((_rc_has(self->cur->locals, name) || _rc_has(self->cur->params, name))) {
#line 275 "src/racecheck.tr"
        return;
    }
#line 276 "src/racecheck.tr"
    List_TrStr_append(self->cur->touched, name);
#line 277 "src/racecheck.tr"
    List_bool_append(self->cur->touched_w, w);
#line 278 "src/racecheck.tr"
    List_bool_append(self->cur->touched_after, self->seen_spawn);
#line 279 "src/racecheck.tr"
    List_i64_append(self->cur->touched_line, self->cur_line);
}

__attribute__((hot)) void RcCk_wpath(RcCk* self, Expr* target) {
#line 282 "src/racecheck.tr"
    TrStr r = _rc_root(target);
#line 283 "src/racecheck.tr"
    if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 283 "src/racecheck.tr"
        _tr_str_release(r);
        return;
    }
#line 284 "src/racecheck.tr"
    List_TrStr_append(self->cur->wroot, r);
#line 285 "src/racecheck.tr"
    ({ TrStr _ord_t3169 = (_rc_first_prop(target)); List_TrStr_append(self->cur->wprop, _ord_t3169); _tr_str_release(_ord_t3169); });
#line 286 "src/racecheck.tr"
    List_bool_append(self->cur->wafter, self->seen_spawn);
#line 287 "src/racecheck.tr"
    RcCk_note(self, r, true);
    _tr_str_release(r);
}

__attribute__((hot)) void RcCk_block(RcCk* self, Block* b) {
#line 290 "src/racecheck.tr"
    long long i = 0LL;
#line 291 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 292 "src/racecheck.tr"
        RcCk_stmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
#line 293 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 294 "src/racecheck.tr"
    RcCk_lock_scan(self, b);
}

__attribute__((hot)) void RcCk_pblock(RcCk* self, Block** pb) {
#line 297 "src/racecheck.tr"
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
#line 297 "src/racecheck.tr"
        return;
    }
#line 298 "src/racecheck.tr"
    RcCk_block(self, (*pb));
}

__attribute__((hot)) void RcCk_declare(RcCk* self, TrStr name) {
#line 301 "src/racecheck.tr"
    List_TrStr_append(self->cur->locals, name);
}

__attribute__((hot)) void RcCk_lock_scan(RcCk* self, Block* b) {
#line 305 "src/racecheck.tr"
    List_TrStr* names = (void*)List_TrStr_new();
#line 306 "src/racecheck.tr"
    List_i64* state = (void*)List_i64_new();
#line 307 "src/racecheck.tr"
    List_TrStr* kinds = (void*)List_TrStr_new();
#line 308 "src/racecheck.tr"
    List_TrStr* guards = (void*)List_TrStr_new();
#line 309 "src/racecheck.tr"
    List_TrStr* gmx = (void*)List_TrStr_new();
#line 310 "src/racecheck.tr"
    List_bool* grel = (void*)List_bool_new();
#line 311 "src/racecheck.tr"
    long long line = self->cur->line;
#line 312 "src/racecheck.tr"
    long long i = 0LL;
#line 313 "src/racecheck.tr"
    while ((i < b->stmts->len)) {
#line 314 "src/racecheck.tr"
        Stmt* sp = ((Stmt*)List_ptr_get(b->stmts, i));
#line 315 "src/racecheck.tr"
        i = (i + 1LL);
#line 316 "src/racecheck.tr"
        Expr* call = (Expr*)(0LL);
#line 317 "src/racecheck.tr"
        bool simple = true;
#line 318 "src/racecheck.tr"
        bool is_line = false;
#line 319 "src/racecheck.tr"
        TrStr letname = _tr_str_lit_len("", 0LL);
#line 320 "src/racecheck.tr"
        __auto_type _t3170 = (*sp);
        if (_t3170.tag == Stmt_SLine) {
            __auto_type n = _t3170.data.SLine.n;
#line 322 "src/racecheck.tr"
            line = n;
#line 323 "src/racecheck.tr"
            is_line = true;
        } else if (_t3170.tag == Stmt_SExpr) {
            __auto_type e = _t3170.data.SExpr.expr;
            call = e;
        } else if (_t3170.tag == Stmt_SLet) {
            __auto_type lnm = _t3170.data.SLet.name;
__auto_type v = _t3170.data.SLet.val;
#line 326 "src/racecheck.tr"
            call = v;
#line 327 "src/racecheck.tr"
            TrStr _strtmp_t3171 = _tr_str_retain(lnm);
            _tr_str_release(letname);
            letname = _strtmp_t3171;
        } else if (_t3170.tag == Stmt_SAssign) {
            __auto_type v2 = _t3170.data.SAssign.val;
            call = v2;
        } else if (1) {
            __auto_type _ = _t3170;
            simple = false;
        }
#line 330 "src/racecheck.tr"
        if (is_line) {
#line 330 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 331 "src/racecheck.tr"
        long long gk = 0LL;
#line 332 "src/racecheck.tr"
        while ((gk < guards->len)) {
#line 333 "src/racecheck.tr"
            if (((List_bool_get(grel, gk) && ({ TrStr _cmpa_t3172 = (List_TrStr_get(guards, gk)); TrStr _cmpb_t3173 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3174 = !_tr_str_eqv(_cmpa_t3172, _cmpb_t3173); _tr_str_release(_cmpa_t3172); _cmpr_t3174; })) && ({ TrStr _at_t3175 = (List_TrStr_get(guards, gk)); __auto_type _wr = (RcCk_mentions(self, sp, _at_t3175)); _tr_str_release(_at_t3175); _wr; }))) {
#line 334 "src/racecheck.tr"
                ({ __auto_type _ord_t3176 = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3177 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3176); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was obtained from `", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3178 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3177); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()` and is used after the lock was released: the guarded object is then reachable by other threads while this one still reads/writes it (a data race).\n      FIX: keep every use of '", 185LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3179 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3178); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before `", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3180 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3179); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` / `", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3181 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3180); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(..)`, or copy the data out while the lock is held.", 55LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3181); _tr_str_release(_ord_t3181); });
#line 335 "src/racecheck.tr"
                List_TrStr_set(guards, gk, _tr_str_lit_len("", 0LL));
            }
#line 336 "src/racecheck.tr"
            gk = (gk + 1LL);
        }
#line 337 "src/racecheck.tr"
        if ((!simple)) {
#line 338 "src/racecheck.tr"
            names = (void*)List_TrStr_new();
#line 339 "src/racecheck.tr"
            state = (void*)List_i64_new();
#line 340 "src/racecheck.tr"
            kinds = (void*)List_TrStr_new();
#line 341 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 342 "src/racecheck.tr"
        if (_rc_nul(call)) {
#line 342 "src/racecheck.tr"
            _tr_str_release(letname);
            continue;
        }
#line 343 "src/racecheck.tr"
        __auto_type _t3182 = (*call);
        if (_t3182.tag == Expr_EMethodCall) {
            __auto_type o = _t3182.data.EMethodCall.obj;
__auto_type m = _t3182.data.EMethodCall.method;
__auto_type margs = _t3182.data.EMethodCall.args;
#line 345 "src/racecheck.tr"
            TrStr r = _tr_str_lit_len("", 0LL);
#line 346 "src/racecheck.tr"
            if ((!_rc_nul(o))) {
#line 347 "src/racecheck.tr"
                __auto_type _t3183 = (*o);
                if (_t3183.tag == Expr_EIdent) {
                    __auto_type rn = _t3183.data.EIdent.name;
                    TrStr _strtmp_t3184 = _tr_str_retain(rn);
                    _tr_str_release(r);
                    r = _strtmp_t3184;
                } else if (1) {
                    __auto_type _ = _t3183;
                    /* pass */
                }
            }
#line 350 "src/racecheck.tr"
            if (_tr_str_eqv((r), (_tr_str_lit_len("", 0LL)))) {
#line 350 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(letname);
                continue;
            }
#line 351 "src/racecheck.tr"
            TrStr k = RcCk_ltype(self, self->cur, r);
#line 352 "src/racecheck.tr"
            if (((_tr_str_eqv((k), (_tr_str_lit_len("", 0LL))) && (!_rc_has(self->cur->locals, r))) && (!_rc_has(self->cur->params, r)))) {
#line 353 "src/racecheck.tr"
                long long gq = 0LL;
#line 354 "src/racecheck.tr"
                while ((gq < self->gnames->len)) {
#line 355 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3185 = (List_TrStr_get(self->gnames, gq)); TrStr _cmpb_t3186 = (r); _Bool _cmpr_t3187 = _tr_str_eqv(_cmpa_t3185, _cmpb_t3186); _tr_str_release(_cmpa_t3185); _cmpr_t3187; })) {
#line 355 "src/racecheck.tr"
                        TrStr _strtmp_t3188 = List_TrStr_get(self->gtn, gq);
                        _tr_str_release(k);
                        k = _strtmp_t3188;
                    }
#line 356 "src/racecheck.tr"
                    gq = (gq + 1LL);
                }
            }
#line 357 "src/racecheck.tr"
            if ((!((_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL))) || _tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))))) {
#line 357 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                _tr_str_release(letname);
                continue;
            }
#line 358 "src/racecheck.tr"
            if ((((margs->len != 0LL) && (!_tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) && (!_tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL)))))) {
#line 358 "src/racecheck.tr"
                _tr_str_release(r);
                _tr_str_release(k);
                _tr_str_release(letname);
                continue;
            }
#line 359 "src/racecheck.tr"
            long long idx = (0LL - 1LL);
#line 360 "src/racecheck.tr"
            long long j = 0LL;
#line 361 "src/racecheck.tr"
            while ((j < names->len)) {
#line 362 "src/racecheck.tr"
                if (({ TrStr _cmpa_t3189 = (List_TrStr_get(names, j)); TrStr _cmpb_t3190 = (r); _Bool _cmpr_t3191 = _tr_str_eqv(_cmpa_t3189, _cmpb_t3190); _tr_str_release(_cmpa_t3189); _cmpr_t3191; })) {
#line 362 "src/racecheck.tr"
                    idx = j;
                }
#line 363 "src/racecheck.tr"
                j = (j + 1LL);
            }
#line 364 "src/racecheck.tr"
            if ((idx < 0LL)) {
#line 365 "src/racecheck.tr"
                List_TrStr_append(names, r);
#line 366 "src/racecheck.tr"
                List_i64_append(state, 0LL);
#line 367 "src/racecheck.tr"
                List_TrStr_append(kinds, k);
#line 368 "src/racecheck.tr"
                idx = (names->len - 1LL);
            }
#line 369 "src/racecheck.tr"
            long long st = List_i64_get(state, idx);
#line 370 "src/racecheck.tr"
            if (_tr_str_eqv((k), (_tr_str_lit_len("Mutex", 5LL)))) {
#line 371 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("get", 3LL)))) {
#line 372 "src/racecheck.tr"
                    long long gj2 = 0LL;
#line 373 "src/racecheck.tr"
                    while ((gj2 < names->len)) {
#line 374 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3192 = (List_TrStr_get(kinds, gj2)); TrStr _cmpb_t3193 = (_tr_str_lit_len("Mutex", 5LL)); _Bool _cmpr_t3194 = _tr_str_eqv(_cmpa_t3192, _cmpb_t3193); _tr_str_release(_cmpa_t3192); _cmpr_t3194; }) && (List_i64_get(state, gj2) == 1LL)) && ({ TrStr _cmpa_t3195 = (List_TrStr_get(names, gj2)); TrStr _cmpb_t3196 = (r); _Bool _cmpr_t3197 = !_tr_str_eqv(_cmpa_t3195, _cmpb_t3196); _tr_str_release(_cmpa_t3195); _cmpr_t3197; }))) {
#line 375 "src/racecheck.tr"
                            if ((!(((_rc_has(self->cur->locals, r) || _rc_has(self->cur->params, r)) || ({ __auto_type _ord_t3199 = (self->cur->locals); TrStr _at_t3198 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3199, _at_t3198)); _tr_str_release(_at_t3198); _wr; })) || ({ __auto_type _ord_t3201 = (self->cur->params); TrStr _at_t3200 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3201, _at_t3200)); _tr_str_release(_at_t3200); _wr; })))) {
#line 376 "src/racecheck.tr"
                                ({ TrStr _ord_t3202 = (List_TrStr_get(names, gj2)); List_TrStr_append(self->cur->pairs_a, _ord_t3202); _tr_str_release(_ord_t3202); });
#line 377 "src/racecheck.tr"
                                List_TrStr_append(self->cur->pairs_b, r);
                            }
                        }
#line 378 "src/racecheck.tr"
                        gj2 = (gj2 + 1LL);
                    }
#line 379 "src/racecheck.tr"
                    TrStr pa = RcCk_ltarg_of(self, self->cur, r);
#line 380 "src/racecheck.tr"
                    long long ci0 = RcCk_find_class(self, pa);
#line 381 "src/racecheck.tr"
                    if (((!_tr_str_eqv((letname), (_tr_str_lit_len("", 0LL)))) && (((((_tr_str_eqv((pa), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((pa), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((pa), (_tr_str_lit_len("Map", 3LL)))) || ((ci0 >= 0LL) && (!List_bool_get(self->class_lib, ci0)))))) {
#line 382 "src/racecheck.tr"
                        List_TrStr_append(guards, letname);
#line 383 "src/racecheck.tr"
                        List_TrStr_append(gmx, r);
#line 384 "src/racecheck.tr"
                        List_bool_append(grel, false);
                    }
#line 385 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 385 "src/racecheck.tr"
                        ({ TrStr _ord_t3203 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()' acquires the lock again while this thread already holds it (no `set()`/`unlock()` since the previous `get()`): the second call blocks forever.\n      FIX: finish with `", 175LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(v)` or `", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` before locking again, or reuse the first value.", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3203); _tr_str_release(_ord_t3203); });
                    }
#line 386 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("unlock", 6LL))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL))))) {
#line 388 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
#line 389 "src/racecheck.tr"
                    long long gr = 0LL;
#line 390 "src/racecheck.tr"
                    while ((gr < guards->len)) {
#line 391 "src/racecheck.tr"
                        if (({ TrStr _cmpa_t3204 = (List_TrStr_get(gmx, gr)); TrStr _cmpb_t3205 = (r); _Bool _cmpr_t3206 = _tr_str_eqv(_cmpa_t3204, _cmpb_t3205); _tr_str_release(_cmpa_t3204); _cmpr_t3206; })) {
#line 391 "src/racecheck.tr"
                            List_bool_set(grel, gr, true);
                        }
#line 392 "src/racecheck.tr"
                        gr = (gr + 1LL);
                    }
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("RwLock", 6LL)))) {
#line 394 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("read", 4LL)))) {
#line 394 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                } else if ((_tr_str_eqv((m), (_tr_str_lit_len("read_unlock", 11LL))) || _tr_str_eqv((m), (_tr_str_lit_len("write_set", 9LL))))) {
#line 395 "src/racecheck.tr"
                    List_i64_set(state, idx, 0LL);
                } else if (_tr_str_eqv((m), (_tr_str_lit_len("write", 5LL)))) {
#line 397 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 397 "src/racecheck.tr"
                        ({ TrStr _ord_t3207 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()' takes the write lock while this thread still holds the read lock from `", 81LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read()`: the write waits for readers (itself included) to leave, which never happens (deadlock).\n      FIX: call `", 115LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read_unlock()` before `", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()`.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3207); _tr_str_release(_ord_t3207); });
                    }
#line 398 "src/racecheck.tr"
                    List_i64_set(state, idx, 2LL);
                }
            } else if (_tr_str_eqv((k), (_tr_str_lit_len("Thread", 6LL)))) {
#line 400 "src/racecheck.tr"
                if (_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL)))) {
#line 401 "src/racecheck.tr"
                    if ((st == 1LL)) {
#line 401 "src/racecheck.tr"
                        ({ TrStr _ord_t3208 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] thread '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is joined twice: joining an already-joined thread handle is undefined behaviour.\n      FIX: join each thread exactly once.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3208); _tr_str_release(_ord_t3208); });
                    }
#line 402 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                }
            }
            _tr_str_release(r);
            _tr_str_release(k);
        } else if (1) {
            __auto_type _ = _t3182;
            /* pass */
        }
        _tr_str_release(letname);
    }
    List_TrStr_free(names);
    List_i64_free(state);
    List_TrStr_free(kinds);
    List_TrStr_free(guards);
    List_TrStr_free(gmx);
    List_bool_free(grel);
}

__attribute__((hot)) void RcCk_stmt(RcCk* self, Stmt* sp) {
#line 406 "src/racecheck.tr"
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
#line 406 "src/racecheck.tr"
        return;
    }
#line 407 "src/racecheck.tr"
    __auto_type _t3209 = (*sp);
    if (_t3209.tag == Stmt_SLine) {
        __auto_type n = _t3209.data.SLine.n;
        self->cur_line = n;
    } else if (_t3209.tag == Stmt_SExpr) {
        __auto_type e = _t3209.data.SExpr.expr;
        RcCk_expr(self, e);
    } else if (_t3209.tag == Stmt_SLet) {
        __auto_type name = _t3209.data.SLet.name;
__auto_type is_sh = _t3209.data.SLet.is_shared;
__auto_type ty = _t3209.data.SLet.ty;
__auto_type v = _t3209.data.SLet.val;
#line 411 "src/racecheck.tr"
        ({ TrStr _apt_t3210 = _tr_str_retain(self->cur->file); RcCk_ty_check(self, ty, _apt_t3210, self->cur_line); _tr_str_release(_apt_t3210); });
#line 412 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 413 "src/racecheck.tr"
        RcCk_declare(self, name);
#line 414 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 415 "src/racecheck.tr"
        if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 415 "src/racecheck.tr"
            TrStr _strtmp_t3211 = _tr_str_retain((*ty)->name);
            _tr_str_release(tn);
            tn = _strtmp_t3211;
        }
#line 416 "src/racecheck.tr"
        if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 416 "src/racecheck.tr"
            TrStr _strtmp_t3212 = _rc_ctor_name(v);
            _tr_str_release(tn);
            tn = _strtmp_t3212;
        }
#line 417 "src/racecheck.tr"
        TrStr targ = _rc_ctor_arg(v);
#line 418 "src/racecheck.tr"
        if (((((unsigned long long)(ty)) != ((unsigned long long)(0LL))) && ((*ty)->args->len > 0LL))) {
#line 418 "src/racecheck.tr"
            TrStr _strtmp_t3213 = _tr_str_retain((*((AstType**)List_ptr_get((*ty)->args, 0LL)))->name);
            _tr_str_release(targ);
            targ = _strtmp_t3213;
        }
#line 419 "src/racecheck.tr"
        if ((is_sh && (!_rc_nul(v)))) {
#line 420 "src/racecheck.tr"
            __auto_type _t3214 = (*v);
            if (_t3214.tag == Expr_EIdent) {
                __auto_type src = _t3214.data.EIdent.name;
#line 422 "src/racecheck.tr"
                TrStr _strtmp_t3215 = RcCk_ltype(self, self->cur, src);
                _tr_str_release(tn);
                tn = _strtmp_t3215;
#line 423 "src/racecheck.tr"
                TrStr _strtmp_t3216 = RcCk_ltarg_of(self, self->cur, src);
                _tr_str_release(targ);
                targ = _strtmp_t3216;
            } else if (1) {
                __auto_type _ = _t3214;
                /* pass */
            }
        }
#line 425 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltn, name);
#line 426 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltt, tn);
#line 427 "src/racecheck.tr"
        List_TrStr_append(self->cur->ltarg, targ);
#line 428 "src/racecheck.tr"
        List_i64_append(self->cur->ldepth, self->loop_depth);
#line 429 "src/racecheck.tr"
        if (_rc_is_thread_spawn(v)) {
#line 430 "src/racecheck.tr"
            List_TrStr_append(self->cur->thr_made, name);
#line 431 "src/racecheck.tr"
            List_i64_append(self->cur->thr_line, self->cur_line);
        }
#line 432 "src/racecheck.tr"
        if (is_sh) {
#line 432 "src/racecheck.tr"
            List_TrStr_append(self->cur->shvars, name);
        }
#line 433 "src/racecheck.tr"
        if (_rc_is_lambda(v)) {
#line 433 "src/racecheck.tr"
            List_TrStr_append(self->cur->closures, name);
        }
        _tr_str_release(tn);
        _tr_str_release(targ);
    } else if (_t3209.tag == Stmt_SMultiLet) {
        __auto_type names = _t3209.data.SMultiLet.names;
__auto_type v = _t3209.data.SMultiLet.val;
#line 435 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 436 "src/racecheck.tr"
        long long mi = 0LL;
#line 437 "src/racecheck.tr"
        while ((mi < names->len)) {
#line 438 "src/racecheck.tr"
            ({ TrStr _at_t3217 = (List_TrStr_get(names, mi)); RcCk_declare(self, _at_t3217); _tr_str_release(_at_t3217); });
#line 439 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t3209.tag == Stmt_SAssign) {
        __auto_type t = _t3209.data.SAssign.target;
__auto_type v = _t3209.data.SAssign.val;
#line 441 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 442 "src/racecheck.tr"
        RcCk_expr(self, t);
#line 443 "src/racecheck.tr"
        RcCk_wpath(self, t);
    } else if (_t3209.tag == Stmt_SReturn) {
        __auto_type v = _t3209.data.SReturn.val;
        RcCk_expr(self, v);
    } else if (_t3209.tag == Stmt_SBreak) {
        __auto_type v = _t3209.data.SBreak.val;
        RcCk_expr(self, v);
    } else if (_t3209.tag == Stmt_SRaise) {
        __auto_type v = _t3209.data.SRaise.val;
        RcCk_expr(self, v);
    } else if (_t3209.tag == Stmt_SUnsafe) {
        __auto_type b = _t3209.data.SUnsafe.body;
        RcCk_block(self, b);
    } else if (_t3209.tag == Stmt_SIf) {
        __auto_type c = _t3209.data.SIf.cond;
__auto_type tb = _t3209.data.SIf.then_b;
__auto_type elifs = _t3209.data.SIf.elifs;
__auto_type eb = _t3209.data.SIf.else_b;
#line 449 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 450 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 451 "src/racecheck.tr"
        long long ei = 0LL;
#line 452 "src/racecheck.tr"
        while ((ei < elifs->len)) {
#line 453 "src/racecheck.tr"
            RcCk_expr(self, ((ElifClause*)List_ptr_get(elifs, ei))->cond);
#line 454 "src/racecheck.tr"
            RcCk_pblock(self, ((ElifClause*)List_ptr_get(elifs, ei))->body);
#line 455 "src/racecheck.tr"
            ei = (ei + 1LL);
        }
#line 456 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (_t3209.tag == Stmt_SWhile) {
        __auto_type c = _t3209.data.SWhile.cond;
__auto_type b = _t3209.data.SWhile.body;
#line 458 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 459 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 460 "src/racecheck.tr"
        RcCk_block(self, b);
#line 461 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3209.tag == Stmt_SFor) {
        __auto_type v = _t3209.data.SFor.var;
__auto_type it = _t3209.data.SFor.iter;
__auto_type b = _t3209.data.SFor.body;
#line 463 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 464 "src/racecheck.tr"
        RcCk_declare(self, v);
#line 465 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 466 "src/racecheck.tr"
        RcCk_block(self, b);
#line 467 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3209.tag == Stmt_SForUnpack) {
        __auto_type vs = _t3209.data.SForUnpack.vars;
__auto_type it = _t3209.data.SForUnpack.iter;
__auto_type b = _t3209.data.SForUnpack.body;
#line 469 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 470 "src/racecheck.tr"
        long long fi = 0LL;
#line 471 "src/racecheck.tr"
        while ((fi < vs->len)) {
#line 472 "src/racecheck.tr"
            ({ TrStr _at_t3218 = (List_TrStr_get(vs, fi)); RcCk_declare(self, _at_t3218); _tr_str_release(_at_t3218); });
#line 473 "src/racecheck.tr"
            fi = (fi + 1LL);
        }
#line 474 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 475 "src/racecheck.tr"
        RcCk_block(self, b);
#line 476 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3209.tag == Stmt_SMatch) {
        __auto_type e = _t3209.data.SMatch.expr;
__auto_type arms = _t3209.data.SMatch.arms;
#line 478 "src/racecheck.tr"
        RcCk_expr(self, e);
#line 479 "src/racecheck.tr"
        long long ai = 0LL;
#line 480 "src/racecheck.tr"
        while ((ai < arms->len)) {
#line 481 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, ai))->guard);
#line 482 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, ai))->body);
#line 483 "src/racecheck.tr"
            ai = (ai + 1LL);
        }
    } else if (_t3209.tag == Stmt_STry) {
        __auto_type tb = _t3209.data.STry.try_body;
__auto_type catches = _t3209.data.STry.catches;
__auto_type fb = _t3209.data.STry.finally_b;
#line 485 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 486 "src/racecheck.tr"
        long long ci = 0LL;
#line 487 "src/racecheck.tr"
        while ((ci < catches->len)) {
#line 488 "src/racecheck.tr"
            ({ TrStr _apt_t3219 = _tr_str_retain((*((CatchClause**)List_ptr_get(catches, ci)))->err_name); RcCk_declare(self, _apt_t3219); _tr_str_release(_apt_t3219); });
#line 489 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci)))->body);
#line 490 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 491 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t3209.tag == Stmt_SAssert) {
        __auto_type c = _t3209.data.SAssert.cond;
__auto_type m = _t3209.data.SAssert.msg;
#line 493 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 494 "src/racecheck.tr"
        RcCk_expr(self, m);
    } else if (_t3209.tag == Stmt_SWith) {
        __auto_type items = _t3209.data.SWith.items;
__auto_type aliases = _t3209.data.SWith.aliases;
__auto_type b = _t3209.data.SWith.body;
#line 496 "src/racecheck.tr"
        long long wi = 0LL;
#line 497 "src/racecheck.tr"
        while ((wi < items->len)) {
#line 498 "src/racecheck.tr"
            RcCk_expr(self, ((Expr*)List_ptr_get(items, wi)));
#line 499 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 500 "src/racecheck.tr"
        wi = 0LL;
#line 501 "src/racecheck.tr"
        while ((wi < aliases->len)) {
#line 502 "src/racecheck.tr"
            ({ TrStr _at_t3220 = (List_TrStr_get(aliases, wi)); RcCk_declare(self, _at_t3220); _tr_str_release(_at_t3220); });
#line 503 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 504 "src/racecheck.tr"
        RcCk_block(self, b);
    } else if (_t3209.tag == Stmt_SSpawn) {
        __auto_type e = _t3209.data.SSpawn.expr;
#line 506 "src/racecheck.tr"
        RcCk_spawn_expr(self, e);
    } else if (_t3209.tag == Stmt_STaskGroup) {
        __auto_type b = _t3209.data.STaskGroup.body;
#line 508 "src/racecheck.tr"
        RcCk_block(self, b);
#line 509 "src/racecheck.tr"
        self->seen_spawn = false;
    } else if (_t3209.tag == Stmt_SGpuBlock) {
        __auto_type b = _t3209.data.SGpuBlock.body;
        RcCk_block(self, b);
    } else if (_t3209.tag == Stmt_SDefer) {
        __auto_type s = _t3209.data.SDefer.stmt;
        RcCk_stmt(self, s);
    } else if (_t3209.tag == Stmt_SLocalDecl) {
        __auto_type d = _t3209.data.SLocalDecl.decl;
#line 513 "src/racecheck.tr"
        __auto_type _t3221 = (*d);
        if (_t3221.tag == Decl_DFunction) {
            __auto_type f = _t3221.data.DFunction.func;
#line 515 "src/racecheck.tr"
            long long nidx = self->fns->len;
#line 516 "src/racecheck.tr"
            ({ TrStr _apt_t3222 = _tr_str_retain(self->cur->file); RcCk_add_fn(self, f, _apt_t3222, self->cur->is_lib); _tr_str_release(_apt_t3222); });
#line 517 "src/racecheck.tr"
            if (((nidx < self->fns->len) && (!self->cur->is_lib))) {
#line 518 "src/racecheck.tr"
                RcFn* nf = ((RcFn*)List_ptr_get(self->fns, nidx));
#line 519 "src/racecheck.tr"
                long long nk = 0LL;
#line 520 "src/racecheck.tr"
                while ((nk < nf->touched->len)) {
#line 521 "src/racecheck.tr"
                    TrStr nn = List_TrStr_get(nf->touched, nk);
#line 522 "src/racecheck.tr"
                    if ((_rc_has(self->cur->locals, nn) || _rc_has(self->cur->params, nn))) {
#line 523 "src/racecheck.tr"
                        ({ __auto_type _ord_t3223 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[N-3] the nested function '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3224 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3223); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the enclosing function '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3225 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3224); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->cur->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but a nested `def` does not capture the enclosing function's locals (it is an ordinary function).\n      FIX: pass '", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as a parameter, or use a `lambda` (which does capture), or move the value to module level.", 92LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3225); _tr_str_release(_ord_t3225); });
#line 524 "src/racecheck.tr"
                        _tr_str_release(nn);
                        break;
                    }
#line 525 "src/racecheck.tr"
                    nk = (nk + 1LL);
                    _tr_str_release(nn);
                }
            }
        } else if (1) {
            __auto_type _ = _t3221;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t3209;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_spawn_expr(RcCk* self, Expr* e) {
#line 531 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 531 "src/racecheck.tr"
        return;
    }
#line 532 "src/racecheck.tr"
    __auto_type _t3226 = (*e);
    if (_t3226.tag == Expr_ECall) {
        __auto_type c = _t3226.data.ECall.callee;
__auto_type args = _t3226.data.ECall.args;
#line 534 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 535 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 536 "src/racecheck.tr"
            __auto_type _t3227 = (*c);
            if (_t3227.tag == Expr_EIdent) {
                __auto_type n = _t3227.data.EIdent.name;
                TrStr _strtmp_t3228 = _tr_str_retain(n);
                _tr_str_release(tn);
                tn = _strtmp_t3228;
            } else if (1) {
                __auto_type _ = _t3227;
                /* pass */
            }
        }
#line 539 "src/racecheck.tr"
        RcCk_record_site(self, tn, args, 0LL);
#line 540 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(tn);
    } else if (_t3226.tag == Expr_EMethodCall) {
        __auto_type o = _t3226.data.EMethodCall.obj;
__auto_type m = _t3226.data.EMethodCall.method;
__auto_type margs = _t3226.data.EMethodCall.args;
#line 542 "src/racecheck.tr"
        TrStr r = _tr_str_lit_len("", 0LL);
#line 543 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 544 "src/racecheck.tr"
            __auto_type _t3229 = (*o);
            if (_t3229.tag == Expr_EIdent) {
                __auto_type rn = _t3229.data.EIdent.name;
                TrStr _strtmp_t3230 = _tr_str_retain(rn);
                _tr_str_release(r);
                r = _strtmp_t3230;
            } else if (1) {
                __auto_type _ = _t3229;
                /* pass */
            }
        }
#line 547 "src/racecheck.tr"
        if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 548 "src/racecheck.tr"
            RcCk_record_site(self, _tr_str_lit_len("", 0LL), margs, 0LL);
#line 549 "src/racecheck.tr"
            ((RcSite*)List_ptr_get(self->cur->sites, (self->cur->sites->len - 1LL)))->recv = _tr_str_retain(r);
#line 550 "src/racecheck.tr"
            ((RcSite*)List_ptr_get(self->cur->sites, (self->cur->sites->len - 1LL)))->meth = _tr_str_retain(m);
        }
#line 551 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(r);
    } else if (1) {
        __auto_type _ = _t3226;
        RcCk_expr(self, e);
    }
}

__attribute__((hot)) void RcCk_record_site(RcCk* self, TrStr target, List_ptr* args, long long skip) {
#line 555 "src/racecheck.tr"
    RcSite* s = ((RcSite*)_tr_obj_alloc(sizeof(RcSite)));
#line 556 "src/racecheck.tr"
    TrStr _sfo_t3231 = s->target;
    s->target = _tr_str_retain(target);
    _tr_str_release(_sfo_t3231);
#line 557 "src/racecheck.tr"
    s->args = (void*)List_ptr_new();
#line 558 "src/racecheck.tr"
    long long i = skip;
#line 559 "src/racecheck.tr"
    while ((i < args->len)) {
#line 560 "src/racecheck.tr"
        ({ __auto_type _ord_t3232 = (((Expr*)List_ptr_get(args, i))); List_ptr_append(s->args, _ord_t3232); });
#line 561 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 562 "src/racecheck.tr"
    s->line = self->cur_line;
#line 563 "src/racecheck.tr"
    s->in_loop = (self->loop_depth > 0LL);
#line 564 "src/racecheck.tr"
    s->depth = self->loop_depth;
#line 565 "src/racecheck.tr"
    TrStr _sfo_t3233 = s->recv;
    s->recv = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t3233);
#line 566 "src/racecheck.tr"
    TrStr _sfo_t3234 = s->meth;
    s->meth = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t3234);
#line 567 "src/racecheck.tr"
    List_ptr_append(self->cur->sites, _tr_obj_retain(s));
#line 568 "src/racecheck.tr"
    self->seen_spawn = true;
    _tr_obj_release(s, _trdrop_RcSite);
}

__attribute__((hot)) void RcCk_exprs(RcCk* self, List_ptr* v) {
#line 571 "src/racecheck.tr"
    long long i = 0LL;
#line 572 "src/racecheck.tr"
    while ((i < v->len)) {
#line 573 "src/racecheck.tr"
        RcCk_expr(self, ((Expr*)List_ptr_get(v, i)));
#line 574 "src/racecheck.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void RcCk_comps(RcCk* self, Expr* element, List_ptr* gens) {
#line 577 "src/racecheck.tr"
    long long i = 0LL;
#line 578 "src/racecheck.tr"
    while ((i < gens->len)) {
#line 579 "src/racecheck.tr"
        ({ TrStr _apt_t3235 = _tr_str_retain((*((Comprehension**)List_ptr_get(gens, i)))->target); RcCk_declare(self, _apt_t3235); _tr_str_release(_apt_t3235); });
#line 580 "src/racecheck.tr"
        RcCk_expr(self, (*((Comprehension**)List_ptr_get(gens, i)))->iter);
#line 581 "src/racecheck.tr"
        RcCk_exprs(self, (*((Comprehension**)List_ptr_get(gens, i)))->ifs);
#line 582 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 583 "src/racecheck.tr"
    RcCk_expr(self, element);
}

__attribute__((hot)) void RcCk_expr(RcCk* self, Expr* e) {
#line 586 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 586 "src/racecheck.tr"
        return;
    }
#line 587 "src/racecheck.tr"
    __auto_type _t3236 = (*e);
    if (_t3236.tag == Expr_EIdent) {
        __auto_type n = _t3236.data.EIdent.name;
#line 589 "src/racecheck.tr"
        List_TrStr_append(self->cur->idents, n);
#line 590 "src/racecheck.tr"
        RcCk_note(self, n, false);
    } else if (_t3236.tag == Expr_EBinOp) {
        __auto_type l = _t3236.data.EBinOp.left;
__auto_type r = _t3236.data.EBinOp.right;
#line 592 "src/racecheck.tr"
        RcCk_expr(self, l);
#line 593 "src/racecheck.tr"
        RcCk_expr(self, r);
    } else if (_t3236.tag == Expr_EUnaryOp) {
        __auto_type x = _t3236.data.EUnaryOp.expr;
        RcCk_expr(self, x);
    } else if (_t3236.tag == Expr_ECall) {
        __auto_type c = _t3236.data.ECall.callee;
__auto_type args = _t3236.data.ECall.args;
#line 596 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 597 "src/racecheck.tr"
            __auto_type _t3237 = (*c);
            if (_t3237.tag == Expr_EIdent) {
                __auto_type fnm = _t3237.data.EIdent.name;
#line 599 "src/racecheck.tr"
                List_TrStr_append(self->cur->calls, fnm);
#line 600 "src/racecheck.tr"
                long long cai = 0LL;
#line 601 "src/racecheck.tr"
                while ((cai < args->len)) {
#line 602 "src/racecheck.tr"
                    __auto_type _t3238 = (*((Expr*)List_ptr_get(args, cai)));
                    if (_t3238.tag == Expr_EIdent) {
                        __auto_type can = _t3238.data.EIdent.name;
#line 604 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_fn, fnm);
#line 605 "src/racecheck.tr"
                        List_i64_append(self->cur->ca_idx, cai);
#line 606 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_name, can);
                    } else if (1) {
                        __auto_type _ = _t3238;
                        /* pass */
                    }
#line 608 "src/racecheck.tr"
                    cai = (cai + 1LL);
                }
#line 609 "src/racecheck.tr"
                if (_tr_str_eqv((fnm), (_tr_str_lit_len("await_all", 9LL)))) {
#line 610 "src/racecheck.tr"
                    long long ai = 0LL;
#line 611 "src/racecheck.tr"
                    while ((ai < args->len)) {
#line 612 "src/racecheck.tr"
                        __auto_type _t3239 = (*((Expr*)List_ptr_get(args, ai)));
                        if (_t3239.tag == Expr_ECall) {
                            __auto_type c2 = _t3239.data.ECall.callee;
__auto_type a2 = _t3239.data.ECall.args;
#line 614 "src/racecheck.tr"
                            TrStr t2 = _tr_str_lit_len("", 0LL);
#line 615 "src/racecheck.tr"
                            if ((!_rc_nul(c2))) {
#line 616 "src/racecheck.tr"
                                __auto_type _t3240 = (*c2);
                                if (_t3240.tag == Expr_EIdent) {
                                    __auto_type n2 = _t3240.data.EIdent.name;
                                    TrStr _strtmp_t3241 = _tr_str_retain(n2);
                                    _tr_str_release(t2);
                                    t2 = _strtmp_t3241;
                                } else if (1) {
                                    __auto_type _ = _t3240;
                                    /* pass */
                                }
                            }
#line 619 "src/racecheck.tr"
                            RcCk_record_site(self, t2, a2, 0LL);
                            _tr_str_release(t2);
                        } else if (1) {
                            __auto_type _ = _t3239;
                            /* pass */
                        }
#line 621 "src/racecheck.tr"
                        ai = (ai + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t3237;
                RcCk_expr(self, c);
            }
        }
#line 623 "src/racecheck.tr"
        RcCk_exprs(self, args);
    } else if (_t3236.tag == Expr_EMethodCall) {
        __auto_type o = _t3236.data.EMethodCall.obj;
__auto_type m = _t3236.data.EMethodCall.method;
__auto_type args = _t3236.data.EMethodCall.args;
#line 625 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 626 "src/racecheck.tr"
        RcCk_exprs(self, args);
#line 627 "src/racecheck.tr"
        TrStr _tr_v_recv = _rc_root(o);
#line 628 "src/racecheck.tr"
        TrStr direct = _tr_str_lit_len("", 0LL);
#line 629 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 630 "src/racecheck.tr"
            __auto_type _t3242 = (*o);
            if (_t3242.tag == Expr_EIdent) {
                __auto_type dn = _t3242.data.EIdent.name;
                TrStr _strtmp_t3243 = _tr_str_retain(dn);
                _tr_str_release(direct);
                direct = _strtmp_t3243;
            } else if (1) {
                __auto_type _ = _t3242;
                /* pass */
            }
        }
#line 633 "src/racecheck.tr"
        if ((!_tr_str_eqv((direct), (_tr_str_lit_len("", 0LL))))) {
#line 634 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcrecv, direct);
#line 635 "src/racecheck.tr"
            List_TrStr_append(self->cur->mcname, m);
        }
#line 636 "src/racecheck.tr"
        if ((((_tr_str_eqv((m), (_tr_str_lit_len("join", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("wait", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("await", 5LL))))) {
#line 636 "src/racecheck.tr"
            self->seen_spawn = false;
        }
#line 637 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && (args->len > 0LL))) {
#line 638 "src/racecheck.tr"
            TrStr t = _tr_str_lit_len("", 0LL);
#line 639 "src/racecheck.tr"
            __auto_type _t3244 = (*((Expr*)List_ptr_get(args, 0LL)));
            if (_t3244.tag == Expr_EIdent) {
                __auto_type tn2 = _t3244.data.EIdent.name;
                TrStr _strtmp_t3245 = _tr_str_retain(tn2);
                _tr_str_release(t);
                t = _strtmp_t3245;
            } else if (_t3244.tag == Expr_EClosure) {
                __auto_type cps = _t3244.data.EClosure.params;
__auto_type cbody = _t3244.data.EClosure.body;
#line 643 "src/racecheck.tr"
                RcFn* tmp = ({ TrStr _apt_t3246 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t3246, self->cur->line, true)); _tr_str_release(_apt_t3246); _wr; });
#line 644 "src/racecheck.tr"
                long long ci = 0LL;
#line 645 "src/racecheck.tr"
                while ((ci < cps->len)) {
#line 646 "src/racecheck.tr"
                    ({ __auto_type _ord_t3247 = (((Param*)List_ptr_get(cps, ci))->name); List_TrStr_append(tmp->params, _ord_t3247); });
#line 647 "src/racecheck.tr"
                    ci = (ci + 1LL);
                }
#line 648 "src/racecheck.tr"
                RcFn* saved = _tr_obj_retain(self->cur);
#line 649 "src/racecheck.tr"
                RcFn* _cltmp_t3248 = _tr_obj_retain(tmp);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3248;
#line 650 "src/racecheck.tr"
                RcCk_block(self, cbody);
#line 651 "src/racecheck.tr"
                RcFn* _cltmp_t3249 = _tr_obj_retain(saved);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3249;
#line 652 "src/racecheck.tr"
                long long ck = 0LL;
#line 653 "src/racecheck.tr"
                while ((ck < tmp->touched->len)) {
#line 654 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3251 = (self->cur->locals); TrStr _at_t3250 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3251, _at_t3250)); _tr_str_release(_at_t3250); _wr; }) || ({ __auto_type _ord_t3253 = (self->cur->params); TrStr _at_t3252 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3253, _at_t3252)); _tr_str_release(_at_t3252); _wr; }))) {
#line 655 "src/racecheck.tr"
                        ({ __auto_type _ord_t3254 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] the closure passed to `spawn` captures '", 46LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3255 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3254); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the spawning function by reference: the new thread would race on it and outlive it.\n      FIX: spawn a named `def` and pass '", 132LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3256 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3255); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as an argument (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3256); _tr_str_release(_ord_t3256); });
#line 656 "src/racecheck.tr"
                        break;
                    }
#line 657 "src/racecheck.tr"
                    ck = (ck + 1LL);
                }
                _tr_obj_release(tmp, _trdrop_RcFn);
                _tr_obj_release(saved, _trdrop_RcFn);
            } else if (1) {
                __auto_type _ = _t3244;
                /* pass */
            }
#line 659 "src/racecheck.tr"
            if ((!_tr_str_eqv((t), (_tr_str_lit_len("", 0LL))))) {
#line 659 "src/racecheck.tr"
                RcCk_record_site(self, t, args, 1LL);
            }
        }
#line 660 "src/racecheck.tr"
        if (((!_tr_str_eqv((_tr_v_recv), (_tr_str_lit_len("", 0LL)))) && _rc_mut_method(m))) {
#line 661 "src/racecheck.tr"
            List_TrStr_append(self->cur->wroot, _tr_v_recv);
#line 662 "src/racecheck.tr"
            ({ TrStr _ord_t3257 = (_rc_first_prop(o)); List_TrStr_append(self->cur->wprop, _ord_t3257); _tr_str_release(_ord_t3257); });
#line 663 "src/racecheck.tr"
            List_bool_append(self->cur->wafter, self->seen_spawn);
#line 664 "src/racecheck.tr"
            RcCk_note(self, _tr_v_recv, true);
        }
        _tr_str_release(_tr_v_recv);
        _tr_str_release(direct);
    } else if (_t3236.tag == Expr_EPropAccess) {
        __auto_type o = _t3236.data.EPropAccess.obj;
        RcCk_expr(self, o);
    } else if (_t3236.tag == Expr_EIndex) {
        __auto_type o = _t3236.data.EIndex.obj;
__auto_type i = _t3236.data.EIndex._tr_v_index;
#line 667 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 668 "src/racecheck.tr"
        RcCk_expr(self, i);
    } else if (_t3236.tag == Expr_ECast) {
        __auto_type x = _t3236.data.ECast.expr;
        RcCk_expr(self, x);
    } else if (_t3236.tag == Expr_EFString) {
        __auto_type parts = _t3236.data.EFString.parts;
#line 671 "src/racecheck.tr"
        long long pi = 0LL;
#line 672 "src/racecheck.tr"
        while ((pi < parts->len)) {
#line 673 "src/racecheck.tr"
            if (((FStringPart*)List_ptr_get(parts, pi))->is_expr) {
#line 673 "src/racecheck.tr"
                RcCk_expr(self, ((FStringPart*)List_ptr_get(parts, pi))->expr);
            }
#line 674 "src/racecheck.tr"
            pi = (pi + 1LL);
        }
    } else if (_t3236.tag == Expr_ETryExpr) {
        __auto_type x = _t3236.data.ETryExpr.expr;
        RcCk_expr(self, x);
    } else if (_t3236.tag == Expr_EClosure) {
        __auto_type params = _t3236.data.EClosure.params;
__auto_type body = _t3236.data.EClosure.body;
#line 677 "src/racecheck.tr"
        long long ci = 0LL;
#line 678 "src/racecheck.tr"
        while ((ci < params->len)) {
#line 679 "src/racecheck.tr"
            ({ TrStr _apt_t3258 = _tr_str_retain(((Param*)List_ptr_get(params, ci))->name); RcCk_declare(self, _apt_t3258); _tr_str_release(_apt_t3258); });
#line 680 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 681 "src/racecheck.tr"
        RcCk_block(self, body);
    } else if (_t3236.tag == Expr_ESuperMethodCall) {
        __auto_type args = _t3236.data.ESuperMethodCall.args;
        RcCk_exprs(self, args);
    } else if (_t3236.tag == Expr_EList) {
        __auto_type items = _t3236.data.EList.items;
        RcCk_exprs(self, items);
    } else if (_t3236.tag == Expr_ESet) {
        __auto_type items = _t3236.data.ESet.items;
        RcCk_exprs(self, items);
    } else if (_t3236.tag == Expr_ETuple) {
        __auto_type items = _t3236.data.ETuple.items;
        RcCk_exprs(self, items);
    } else if (_t3236.tag == Expr_EDict) {
        __auto_type ks = _t3236.data.EDict.keys;
__auto_type vs = _t3236.data.EDict.vals;
#line 687 "src/racecheck.tr"
        RcCk_exprs(self, ks);
#line 688 "src/racecheck.tr"
        RcCk_exprs(self, vs);
    } else if (_t3236.tag == Expr_EListComp) {
        __auto_type el = _t3236.data.EListComp.element;
__auto_type gens = _t3236.data.EListComp.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3236.tag == Expr_EGeneratorExpr) {
        __auto_type el = _t3236.data.EGeneratorExpr.element;
__auto_type gens = _t3236.data.EGeneratorExpr.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3236.tag == Expr_ESlice) {
        __auto_type a = _t3236.data.ESlice.start;
__auto_type b = _t3236.data.ESlice.stop;
__auto_type c = _t3236.data.ESlice.step;
#line 692 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 693 "src/racecheck.tr"
        RcCk_expr(self, b);
#line 694 "src/racecheck.tr"
        RcCk_expr(self, c);
    } else if (_t3236.tag == Expr_EAwait) {
        __auto_type x = _t3236.data.EAwait.expr;
        RcCk_expr(self, x);
    } else if (_t3236.tag == Expr_EYield) {
        __auto_type x = _t3236.data.EYield.expr;
        RcCk_expr(self, x);
    } else if (_t3236.tag == Expr_ETry) {
        __auto_type tb = _t3236.data.ETry.try_body;
__auto_type catches = _t3236.data.ETry.catches;
__auto_type fb = _t3236.data.ETry.finally_b;
#line 698 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 699 "src/racecheck.tr"
        long long ci2 = 0LL;
#line 700 "src/racecheck.tr"
        while ((ci2 < catches->len)) {
#line 701 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci2)))->body);
#line 702 "src/racecheck.tr"
            ci2 = (ci2 + 1LL);
        }
#line 703 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t3236.tag == Expr_ERange) {
        __auto_type a = _t3236.data.ERange.start;
__auto_type b = _t3236.data.ERange.end;
#line 705 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 706 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3236.tag == Expr_EIfElse) {
        __auto_type c = _t3236.data.EIfElse.cond;
__auto_type a = _t3236.data.EIfElse.then_expr;
__auto_type b = _t3236.data.EIfElse.else_expr;
#line 708 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 709 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 710 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3236.tag == Expr_EDo) {
        __auto_type b = _t3236.data.EDo.body;
        RcCk_block(self, b);
    } else if (_t3236.tag == Expr_EMatch) {
        __auto_type s = _t3236.data.EMatch.subj;
__auto_type arms = _t3236.data.EMatch.arms;
#line 713 "src/racecheck.tr"
        RcCk_expr(self, s);
#line 714 "src/racecheck.tr"
        long long mi = 0LL;
#line 715 "src/racecheck.tr"
        while ((mi < arms->len)) {
#line 716 "src/racecheck.tr"
            RcCk_expr(self, ((MatchArm*)List_ptr_get(arms, mi))->guard);
#line 717 "src/racecheck.tr"
            RcCk_pblock(self, ((MatchArm*)List_ptr_get(arms, mi))->body);
#line 718 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t3236.tag == Expr_EMacroCall) {
        __auto_type args = _t3236.data.EMacroCall.args;
        RcCk_exprs(self, args);
    } else if (_t3236.tag == Expr_ELoop) {
        __auto_type b = _t3236.data.ELoop.body;
        RcCk_block(self, b);
    } else if (_t3236.tag == Expr_EWhileExpr) {
        __auto_type c = _t3236.data.EWhileExpr.cond;
__auto_type b = _t3236.data.EWhileExpr.body;
__auto_type eb = _t3236.data.EWhileExpr.else_body;
#line 722 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 723 "src/racecheck.tr"
        RcCk_block(self, b);
#line 724 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (1) {
        __auto_type _ = _t3236;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_add_method(RcCk* self, FunctionDef* f, TrStr cls, TrStr file, bool lib) {
#line 728 "src/racecheck.tr"
    ({ TrStr _at_t3259 = (({ TrStr _tr_cl = (_tr_strx_concatv((cls), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_add_fn_named(self, f, _at_t3259, file, lib); _tr_str_release(_at_t3259); });
}

__attribute__((hot)) void RcCk_add_fn(RcCk* self, FunctionDef* f, TrStr file, bool lib) {
#line 731 "src/racecheck.tr"
    ({ TrStr _apt_t3260 = _tr_str_retain(f->name); RcCk_add_fn_named(self, f, _apt_t3260, file, lib); _tr_str_release(_apt_t3260); });
}

__attribute__((hot)) void RcCk_add_fn_named(RcCk* self, FunctionDef* f, TrStr nm, TrStr file, bool lib) {
#line 734 "src/racecheck.tr"
    RcFn* rf = _rc_new_fn(nm, file, f->line, (lib || f->is_lib));
#line 735 "src/racecheck.tr"
    long long pi = 0LL;
#line 736 "src/racecheck.tr"
    while ((pi < f->params->len)) {
#line 737 "src/racecheck.tr"
        ({ __auto_type _ord_t3261 = (((Param*)List_ptr_get(f->params, pi))->name); List_TrStr_append(rf->params, _ord_t3261); });
#line 738 "src/racecheck.tr"
        if ((!rf->is_lib)) {
#line 738 "src/racecheck.tr"
            ({ __auto_type _ord_t3262 = (((Param*)List_ptr_get(f->params, pi))->ty); __auto_type _ord_t3263 = (f->line); RcCk_ty_check(self, _ord_t3262, file, _ord_t3263); });
        }
#line 739 "src/racecheck.tr"
        AstType** pt = ((Param*)List_ptr_get(f->params, pi))->ty;
#line 740 "src/racecheck.tr"
        if ((((unsigned long long)(pt)) != ((unsigned long long)(0LL)))) {
#line 741 "src/racecheck.tr"
            ({ __auto_type _ord_t3264 = ((*pt)->name); List_TrStr_append(rf->ptypes, _ord_t3264); });
#line 742 "src/racecheck.tr"
            if (((*pt)->args->len > 0LL)) {
#line 742 "src/racecheck.tr"
                ({ __auto_type _ord_t3265 = ((*((AstType**)List_ptr_get((*pt)->args, 0LL)))->name); List_TrStr_append(rf->pinner, _ord_t3265); });
            } else {
#line 743 "src/racecheck.tr"
                List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
            }
        } else {
#line 745 "src/racecheck.tr"
            List_TrStr_append(rf->ptypes, _tr_str_lit_len("", 0LL));
#line 746 "src/racecheck.tr"
            List_TrStr_append(rf->pinner, _tr_str_lit_len("", 0LL));
        }
#line 747 "src/racecheck.tr"
        pi = (pi + 1LL);
    }
#line 748 "src/racecheck.tr"
    List_ptr_append(self->fns, _tr_obj_retain(rf));
#line 749 "src/racecheck.tr"
    if (rf->is_lib) {
#line 749 "src/racecheck.tr"
        _tr_obj_release(rf, _trdrop_RcFn);
        return;
    }
#line 750 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 751 "src/racecheck.tr"
    long long saved_line = self->cur_line;
#line 752 "src/racecheck.tr"
    long long saved_ld = self->loop_depth;
#line 753 "src/racecheck.tr"
    bool saved_sp = self->seen_spawn;
#line 754 "src/racecheck.tr"
    RcFn* _cltmp_t3266 = _tr_obj_retain(rf);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3266;
#line 755 "src/racecheck.tr"
    self->cur_line = f->line;
#line 756 "src/racecheck.tr"
    self->loop_depth = 0LL;
#line 757 "src/racecheck.tr"
    self->seen_spawn = false;
#line 758 "src/racecheck.tr"
    RcCk_block(self, f->body);
#line 759 "src/racecheck.tr"
    long long tw = 0LL;
#line 760 "src/racecheck.tr"
    while ((tw < rf->thr_made->len)) {
#line 761 "src/racecheck.tr"
        if (({ __auto_type _ord_t3268 = (rf->idents); TrStr _at_t3267 = (List_TrStr_get(rf->thr_made, tw)); __auto_type _wr = ((!_rc_has(_ord_t3268, _at_t3267))); _tr_str_release(_at_t3267); _wr; })) {
#line 762 "src/racecheck.tr"
            ({ __auto_type _ord_t3269 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("W|", 2LL)), (file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(List_i64_get(rf->thr_line, tw))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] the Thread handle '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3270 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3269); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is never joined, detached or used: the thread runs unsupervised and the program may exit while it is still working.\n      FIX: call `", 135LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3271 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3270); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".join()` (wait for it) or `", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3272 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3271); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".detach()` (run independently).", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(rf->msgs, _ord_t3272); _tr_str_release(_ord_t3272); });
        }
#line 763 "src/racecheck.tr"
        tw = (tw + 1LL);
    }
#line 764 "src/racecheck.tr"
    RcFn* _cltmp_t3273 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3273;
#line 765 "src/racecheck.tr"
    self->cur_line = saved_line;
#line 766 "src/racecheck.tr"
    self->loop_depth = saved_ld;
#line 767 "src/racecheck.tr"
    self->seen_spawn = saved_sp;
    _tr_obj_release(rf, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
}

__attribute__((hot)) void RcCk_add_global(RcCk* self, TrStr name, bool is_mut, bool is_const, AstType** ty, Expr* v, TrStr df) {
#line 770 "src/racecheck.tr"
    List_TrStr_append(self->gnames, name);
#line 771 "src/racecheck.tr"
    List_TrStr_append(self->gfiles, df);
#line 772 "src/racecheck.tr"
    TrStr tn = _tr_str_lit_len("", 0LL);
#line 773 "src/racecheck.tr"
    if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 773 "src/racecheck.tr"
        TrStr _strtmp_t3274 = _tr_str_retain((*ty)->name);
        _tr_str_release(tn);
        tn = _strtmp_t3274;
    }
#line 774 "src/racecheck.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 774 "src/racecheck.tr"
        TrStr _strtmp_t3275 = _rc_ctor_name(v);
        _tr_str_release(tn);
        tn = _strtmp_t3275;
    }
#line 775 "src/racecheck.tr"
    bool sync = _rc_sync_ty(tn);
#line 776 "src/racecheck.tr"
    if (((!sync) && (!_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))))) {
#line 777 "src/racecheck.tr"
        long long ci0 = RcCk_find_class(self, tn);
#line 778 "src/racecheck.tr"
        if (((ci0 >= 0LL) && List_bool_get(self->class_lib, ci0))) {
#line 778 "src/racecheck.tr"
            sync = true;
        }
    }
#line 779 "src/racecheck.tr"
    if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 780 "src/racecheck.tr"
        __auto_type _t3276 = (*v);
        if (_t3276.tag == Expr_ELitInt) {
            __auto_type _ = _t3276.data.ELitInt.val;
            /* pass */
        } else if (_t3276.tag == Expr_ELitFloat) {
            __auto_type _ = _t3276.data.ELitFloat.val;
            /* pass */
        } else if (_t3276.tag == Expr_ELitStr) {
            /* pass */
        } else if (_t3276.tag == Expr_ELitBool) {
            __auto_type _ = _t3276.data.ELitBool.val;
            /* pass */
        } else if (_t3276.tag == Expr_EList) {
            __auto_type _ = _t3276.data.EList.items;
            /* pass */
        } else if (_t3276.tag == Expr_EDict) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t3276;
            sync = true;
        }
    }
#line 788 "src/racecheck.tr"
    if ((!(is_mut && (!is_const)))) {
#line 790 "src/racecheck.tr"
        bool content_mut = (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || (RcCk_find_class(self, tn) >= 0LL));
#line 791 "src/racecheck.tr"
        if ((_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL))) && (!_rc_nul(v)))) {
#line 792 "src/racecheck.tr"
            __auto_type _t3277 = (*v);
            if (_t3277.tag == Expr_EList) {
                __auto_type _ = _t3277.data.EList.items;
                content_mut = true;
            } else if (_t3277.tag == Expr_EDict) {
                content_mut = true;
            } else if (_t3277.tag == Expr_ESet) {
                __auto_type _ = _t3277.data.ESet.items;
                content_mut = true;
            } else if (1) {
                __auto_type _ = _t3277;
                /* pass */
            }
        }
#line 797 "src/racecheck.tr"
        if ((!content_mut)) {
#line 797 "src/racecheck.tr"
            sync = true;
        }
    }
#line 798 "src/racecheck.tr"
    List_bool_append(self->gsync, sync);
#line 799 "src/racecheck.tr"
    List_TrStr_append(self->gtn, tn);
    _tr_str_release(tn);
}

__attribute__((hot)) long long RcCk_find_fn(RcCk* self, TrStr name) {
#line 802 "src/racecheck.tr"
    long long i = 0LL;
#line 803 "src/racecheck.tr"
    while ((i < self->fns->len)) {
#line 804 "src/racecheck.tr"
        if ((_tr_str_eqv((((RcFn*)List_ptr_get(self->fns, i))->name), (name)) && (!((RcFn*)List_ptr_get(self->fns, i))->is_lib))) {
#line 804 "src/racecheck.tr"
            return i;
        }
#line 805 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 806 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) long long RcCk_find_class(RcCk* self, TrStr name) {
#line 809 "src/racecheck.tr"
    long long i = 0LL;
#line 810 "src/racecheck.tr"
    while ((i < self->classes->len)) {
#line 811 "src/racecheck.tr"
        if (_tr_str_eqv((((ClassDef*)List_ptr_get(self->classes, i))->name), (name))) {
#line 811 "src/racecheck.tr"
            return i;
        }
#line 812 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 813 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool RcCk_field_plain(RcCk* self, TrStr cn, TrStr prop) {
#line 817 "src/racecheck.tr"
    long long ci = RcCk_find_class(self, cn);
#line 818 "src/racecheck.tr"
    if ((ci < 0LL)) {
#line 818 "src/racecheck.tr"
        return false;
    }
#line 819 "src/racecheck.tr"
    ClassDef* cls = ((ClassDef*)List_ptr_get(self->classes, ci));
#line 820 "src/racecheck.tr"
    long long i = 0LL;
#line 821 "src/racecheck.tr"
    while ((i < cls->fields->len)) {
#line 822 "src/racecheck.tr"
        if (_tr_str_eqv((((FieldDef*)List_ptr_get(cls->fields, i))->name), (prop))) {
#line 823 "src/racecheck.tr"
            AstType** ft = ((FieldDef*)List_ptr_get(cls->fields, i))->ty;
#line 824 "src/racecheck.tr"
            if ((((unsigned long long)(ft)) == ((unsigned long long)(0LL)))) {
#line 824 "src/racecheck.tr"
                return false;
            }
#line 825 "src/racecheck.tr"
            TrStr tn = _tr_str_retain((*ft)->name);
#line 826 "src/racecheck.tr"
            if (_rc_sync_ty(tn)) {
#line 826 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 827 "src/racecheck.tr"
            long long ni = RcCk_find_class(self, tn);
#line 828 "src/racecheck.tr"
            if (((ni >= 0LL) && List_bool_get(self->class_lib, ni))) {
#line 828 "src/racecheck.tr"
                _tr_str_release(tn);
                return false;
            }
#line 829 "src/racecheck.tr"
            _tr_str_release(tn);
            return true;
        }
#line 830 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 831 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_i64* RcCk_reach(RcCk* self, long long ti) {
#line 835 "src/racecheck.tr"
    List_i64* seen = (void*)List_i64_new();
#line 836 "src/racecheck.tr"
    List_i64_append(seen, ti);
#line 837 "src/racecheck.tr"
    long long k = 0LL;
#line 838 "src/racecheck.tr"
    while ((k < seen->len)) {
#line 839 "src/racecheck.tr"
        RcFn* f = ({ __auto_type _ord_t3278 = (List_i64_get(seen, k)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3278))); _wr; });
#line 840 "src/racecheck.tr"
        long long ci = 0LL;
#line 841 "src/racecheck.tr"
        while ((ci < f->calls->len)) {
#line 842 "src/racecheck.tr"
            long long ni = ({ TrStr _at_t3279 = (List_TrStr_get(f->calls, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3279)); _tr_str_release(_at_t3279); _wr; });
#line 843 "src/racecheck.tr"
            if ((ni >= 0LL)) {
#line 844 "src/racecheck.tr"
                bool dup = false;
#line 845 "src/racecheck.tr"
                long long q = 0LL;
#line 846 "src/racecheck.tr"
                while ((q < seen->len)) {
#line 847 "src/racecheck.tr"
                    if ((List_i64_get(seen, q) == ni)) {
#line 847 "src/racecheck.tr"
                        dup = true;
                    }
#line 848 "src/racecheck.tr"
                    q = (q + 1LL);
                }
#line 849 "src/racecheck.tr"
                if ((!dup)) {
#line 849 "src/racecheck.tr"
                    List_i64_append(seen, ni);
                }
            }
#line 850 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 851 "src/racecheck.tr"
        long long mci = 0LL;
#line 852 "src/racecheck.tr"
        while ((mci < f->mcrecv->len)) {
#line 853 "src/racecheck.tr"
            TrStr rcv = List_TrStr_get(f->mcrecv, mci);
#line 854 "src/racecheck.tr"
            TrStr rcn = _tr_str_lit_len("", 0LL);
#line 855 "src/racecheck.tr"
            if (_tr_str_eqv((rcv), (_tr_str_lit_len("self", 4LL)))) {
#line 856 "src/racecheck.tr"
                long long dot = ({ TrStr _apt_t3280 = _tr_str_retain(f->name); __auto_type _wr = (_rc_dot(_apt_t3280)); _tr_str_release(_apt_t3280); _wr; });
#line 857 "src/racecheck.tr"
                if ((dot > 0LL)) {
#line 857 "src/racecheck.tr"
                    TrStr _strtmp_t3281 = _tr_str_slicev((f->name), 0LL, dot);
                    _tr_str_release(rcn);
                    rcn = _strtmp_t3281;
                }
            } else {
#line 859 "src/racecheck.tr"
                TrStr _strtmp_t3282 = RcCk_lclass(self, f, rcv);
                _tr_str_release(rcn);
                rcn = _strtmp_t3282;
#line 860 "src/racecheck.tr"
                if (_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL)))) {
#line 861 "src/racecheck.tr"
                    long long gj = 0LL;
#line 862 "src/racecheck.tr"
                    while ((gj < self->gnames->len)) {
#line 863 "src/racecheck.tr"
                        if (({ TrStr _cmpa_t3283 = (List_TrStr_get(self->gnames, gj)); TrStr _cmpb_t3284 = (rcv); _Bool _cmpr_t3285 = _tr_str_eqv(_cmpa_t3283, _cmpb_t3284); _tr_str_release(_cmpa_t3283); _cmpr_t3285; })) {
#line 863 "src/racecheck.tr"
                            TrStr _strtmp_t3286 = List_TrStr_get(self->gtn, gj);
                            _tr_str_release(rcn);
                            rcn = _strtmp_t3286;
                        }
#line 864 "src/racecheck.tr"
                        gj = (gj + 1LL);
                    }
                }
            }
#line 865 "src/racecheck.tr"
            if ((!_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL))))) {
#line 866 "src/racecheck.tr"
                long long mni = ({ TrStr _at_t3287 = (({ TrStr _tr_cl = (_tr_strx_concatv((rcn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(f->mcname, mci)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3287)); _tr_str_release(_at_t3287); _wr; });
#line 867 "src/racecheck.tr"
                if ((mni >= 0LL)) {
#line 868 "src/racecheck.tr"
                    bool dup2 = false;
#line 869 "src/racecheck.tr"
                    long long q2 = 0LL;
#line 870 "src/racecheck.tr"
                    while ((q2 < seen->len)) {
#line 871 "src/racecheck.tr"
                        if ((List_i64_get(seen, q2) == mni)) {
#line 871 "src/racecheck.tr"
                            dup2 = true;
                        }
#line 872 "src/racecheck.tr"
                        q2 = (q2 + 1LL);
                    }
#line 873 "src/racecheck.tr"
                    if ((!dup2)) {
#line 873 "src/racecheck.tr"
                        List_i64_append(seen, mni);
                    }
                }
            }
#line 874 "src/racecheck.tr"
            mci = (mci + 1LL);
            _tr_str_release(rcv);
            _tr_str_release(rcn);
        }
#line 875 "src/racecheck.tr"
        k = (k + 1LL);
    }
#line 876 "src/racecheck.tr"
    return seen;
}

__attribute__((hot)) void RcCk_analyze_site(RcCk* self, RcFn* f, RcSite* s) {
#line 880 "src/racecheck.tr"
    if (((!_tr_str_eqv((s->target), (_tr_str_lit_len("", 0LL)))) && ({ __auto_type _ord_t3289 = (f->closures); TrStr _apt_t3288 = _tr_str_retain(s->target); __auto_type _wr = (_rc_has(_ord_t3289, _apt_t3288)); _tr_str_release(_apt_t3288); _wr; }))) {
#line 881 "src/racecheck.tr"
        ({ __auto_type _ord_t3290 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3291 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3290); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a closure value: it captures its surrounding locals by reference into this stack frame, so a spawned thread would race on them and outlive them.\n      FIX: spawn a named `def` and pass the data it needs as arguments (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 289LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3291); _tr_str_release(_ord_t3291); });
#line 882 "src/racecheck.tr"
        return;
    }
#line 883 "src/racecheck.tr"
    if ((!_tr_str_eqv((s->recv), (_tr_str_lit_len("", 0LL))))) {
#line 884 "src/racecheck.tr"
        RcCk_analyze_method_site(self, f, s);
#line 885 "src/racecheck.tr"
        return;
    }
#line 886 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3292 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3292)); _tr_str_release(_apt_t3292); _wr; });
#line 887 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 887 "src/racecheck.tr"
        return;
    }
#line 888 "src/racecheck.tr"
    ({ TrStr _apt_t3293 = _tr_str_retain(s->target); RcCk_check_globals(self, f, s, ti, _apt_t3293); _tr_str_release(_apt_t3293); });
#line 890 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, ti));
#line 891 "src/racecheck.tr"
    long long ai = 0LL;
#line 892 "src/racecheck.tr"
    while (((ai < s->args->len) && (ai < tf->params->len))) {
#line 893 "src/racecheck.tr"
        TrStr an = _tr_str_lit_len("", 0LL);
#line 894 "src/racecheck.tr"
        __auto_type _t3294 = (*((Expr*)List_ptr_get(s->args, ai)));
        if (_t3294.tag == Expr_EIdent) {
            __auto_type nn = _t3294.data.EIdent.name;
            TrStr _strtmp_t3295 = _tr_str_retain(nn);
            _tr_str_release(an);
            an = _strtmp_t3295;
        } else if (1) {
            __auto_type _ = _t3294;
            /* pass */
        }
#line 897 "src/racecheck.tr"
        TrStr ptn = List_TrStr_get(tf->ptypes, ai);
#line 898 "src/racecheck.tr"
        if ((!_tr_str_eqv((an), (_tr_str_lit_len("", 0LL))))) {
#line 899 "src/racecheck.tr"
            long long cnt = 0LL;
#line 900 "src/racecheck.tr"
            long long si = 0LL;
#line 901 "src/racecheck.tr"
            while ((si < f->sites->len)) {
#line 902 "src/racecheck.tr"
                long long oa = 0LL;
#line 903 "src/racecheck.tr"
                while ((oa < ((RcSite*)List_ptr_get(f->sites, si))->args->len)) {
#line 904 "src/racecheck.tr"
                    __auto_type _t3296 = (*((Expr*)List_ptr_get(((RcSite*)List_ptr_get(f->sites, si))->args, oa)));
                    if (_t3296.tag == Expr_EIdent) {
                        __auto_type on = _t3296.data.EIdent.name;
#line 906 "src/racecheck.tr"
                        if (_tr_str_eqv((on), (an))) {
#line 906 "src/racecheck.tr"
                            cnt = (cnt + 1LL);
                        }
                    } else if (1) {
                        __auto_type _ = _t3296;
                        /* pass */
                    }
#line 908 "src/racecheck.tr"
                    oa = (oa + 1LL);
                }
#line 909 "src/racecheck.tr"
                si = (si + 1LL);
            }
#line 910 "src/racecheck.tr"
            bool is_sh = (_rc_has(f->shvars, an) || ({ TrStr _cmpa_t3297 = (RcCk_ltype(self, f, an)); TrStr _cmpb_t3298 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3299 = _tr_str_eqv(_cmpa_t3297, _cmpb_t3298); _tr_str_release(_cmpa_t3297); _cmpr_t3299; }));
#line 911 "src/racecheck.tr"
            if (((is_sh && (!_tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL))))) && (!_tr_str_eqv((ptn), (_tr_str_lit_len("", 0LL)))))) {
#line 912 "src/racecheck.tr"
                ({ __auto_type _ord_t3300 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-10] '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a `Shared` handle but '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3301 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3300); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares parameter '", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3302 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3301); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': the thread would be handed the shared control block where it expects the object itself (a type confusion).\n      FIX: declare the worker's parameter as `", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3303 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3302); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Shared[", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3303); _tr_str_release(_ord_t3303); });
#line 913 "src/racecheck.tr"
                ai = (ai + 1LL);
#line 914 "src/racecheck.tr"
                _tr_str_release(an);
                _tr_str_release(ptn);
                continue;
            }
#line 915 "src/racecheck.tr"
            long long dd = RcCk_ldepth_of(self, f, an);
#line 916 "src/racecheck.tr"
            bool multi = ((((cnt >= 2LL) || is_sh) || _tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL)))) || ((dd >= 0LL) && (s->depth > dd)));
#line 917 "src/racecheck.tr"
            if (multi) {
#line 918 "src/racecheck.tr"
                RcCk_check_shared_writes(self, f, s, tf, ai, an);
#line 919 "src/racecheck.tr"
                if ((is_sh || _tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL))))) {
#line 919 "src/racecheck.tr"
                    RcCk_check_parent_writes(self, f, s, an);
                }
            }
        } else if (_tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL)))) {
#line 922 "src/racecheck.tr"
            RcCk_check_shared_writes(self, f, s, tf, ai, _tr_str_lit_len("a shared handle", 15LL));
        }
#line 923 "src/racecheck.tr"
        ai = (ai + 1LL);
        _tr_str_release(an);
        _tr_str_release(ptn);
    }
}

__attribute__((hot)) void RcCk_analyze_method_site(RcCk* self, RcFn* f, RcSite* s) {
#line 927 "src/racecheck.tr"
    TrStr cn = ({ TrStr _apt_t3304 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_lclass(self, f, _apt_t3304)); _tr_str_release(_apt_t3304); _wr; });
#line 928 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 928 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 929 "src/racecheck.tr"
    long long mi = ({ TrStr _at_t3305 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3305)); _tr_str_release(_at_t3305); _wr; });
#line 930 "src/racecheck.tr"
    if ((mi < 0LL)) {
#line 930 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 931 "src/racecheck.tr"
    ({ TrStr _at_t3306 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_check_globals(self, f, s, mi, _at_t3306); _tr_str_release(_at_t3306); });
#line 932 "src/racecheck.tr"
    long long cnt = 0LL;
#line 933 "src/racecheck.tr"
    long long si = 0LL;
#line 934 "src/racecheck.tr"
    while ((si < f->sites->len)) {
#line 935 "src/racecheck.tr"
        if (({ __auto_type _ord_t3307 = (((RcSite*)List_ptr_get(f->sites, si))->recv); __auto_type _wr = (_tr_str_eqv((_ord_t3307), (s->recv))); _wr; })) {
#line 935 "src/racecheck.tr"
            cnt = (cnt + 1LL);
        }
#line 936 "src/racecheck.tr"
        si = (si + 1LL);
    }
#line 937 "src/racecheck.tr"
    bool is_sh = (({ __auto_type _ord_t3309 = (f->shvars); TrStr _apt_t3308 = _tr_str_retain(s->recv); __auto_type _wr = (_rc_has(_ord_t3309, _apt_t3308)); _tr_str_release(_apt_t3308); _wr; }) || ({ TrStr _apt_t3310 = _tr_str_retain(s->recv); __auto_type _wr = (({ TrStr _cmpa_t3311 = (RcCk_ltype(self, f, _apt_t3310)); TrStr _cmpb_t3312 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3313 = _tr_str_eqv(_cmpa_t3311, _cmpb_t3312); _tr_str_release(_cmpa_t3311); _cmpr_t3313; })); _tr_str_release(_apt_t3310); _wr; }));
#line 938 "src/racecheck.tr"
    long long dd = ({ TrStr _apt_t3314 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_ldepth_of(self, f, _apt_t3314)); _tr_str_release(_apt_t3314); _wr; });
#line 939 "src/racecheck.tr"
    if ((!(((cnt >= 2LL) || is_sh) || ((dd >= 0LL) && (s->depth > dd))))) {
#line 939 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 940 "src/racecheck.tr"
    RcFn* mf = ((RcFn*)List_ptr_get(self->fns, mi));
#line 941 "src/racecheck.tr"
    long long k = 0LL;
#line 942 "src/racecheck.tr"
    while ((k < mf->wroot->len)) {
#line 943 "src/racecheck.tr"
        if (((({ TrStr _cmpa_t3315 = (List_TrStr_get(mf->wroot, k)); TrStr _cmpb_t3316 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3317 = _tr_str_eqv(_cmpa_t3315, _cmpb_t3316); _tr_str_release(_cmpa_t3315); _cmpr_t3317; }) && ({ TrStr _cmpa_t3318 = (List_TrStr_get(mf->wprop, k)); TrStr _cmpb_t3319 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3320 = !_tr_str_eqv(_cmpa_t3318, _cmpb_t3319); _tr_str_release(_cmpa_t3318); _cmpr_t3320; })) && ({ TrStr _at_t3321 = (List_TrStr_get(mf->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3321)); _tr_str_release(_at_t3321); _wr; }))) {
#line 944 "src/racecheck.tr"
            ({ TrStr _ord_t3322 = (List_TrStr_get(mf->wprop, k)); __auto_type _ord_t3323 = (s->recv); RcCk_shared_msg(self, f, s, mf, cn, _ord_t3322, _ord_t3323); _tr_str_release(_ord_t3322); });
#line 945 "src/racecheck.tr"
            _tr_str_release(cn);
            return;
        }
#line 946 "src/racecheck.tr"
        k = (k + 1LL);
    }
#line 947 "src/racecheck.tr"
    if (is_sh) {
#line 947 "src/racecheck.tr"
        ({ TrStr _apt_t3324 = _tr_str_retain(s->recv); RcCk_check_parent_writes(self, f, s, _apt_t3324); _tr_str_release(_apt_t3324); });
    }
    _tr_str_release(cn);
}

__attribute__((hot)) void RcCk_check_parent_writes(RcCk* self, RcFn* f, RcSite* s, TrStr an) {
#line 951 "src/racecheck.tr"
    TrStr cn = RcCk_lclass(self, f, an);
#line 952 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 952 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 953 "src/racecheck.tr"
    long long k = 0LL;
#line 954 "src/racecheck.tr"
    while ((k < f->wroot->len)) {
#line 955 "src/racecheck.tr"
        if ((((({ TrStr _cmpa_t3325 = (List_TrStr_get(f->wroot, k)); TrStr _cmpb_t3326 = (an); _Bool _cmpr_t3327 = _tr_str_eqv(_cmpa_t3325, _cmpb_t3326); _tr_str_release(_cmpa_t3325); _cmpr_t3327; }) && List_bool_get(f->wafter, k)) && ({ TrStr _cmpa_t3328 = (List_TrStr_get(f->wprop, k)); TrStr _cmpb_t3329 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3330 = !_tr_str_eqv(_cmpa_t3328, _cmpb_t3329); _tr_str_release(_cmpa_t3328); _cmpr_t3330; })) && ({ TrStr _at_t3331 = (List_TrStr_get(f->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3331)); _tr_str_release(_at_t3331); _wr; }))) {
#line 956 "src/racecheck.tr"
            ({ __auto_type _ord_t3332 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3333 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3332); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' writes the plain field '", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3334 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3333); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while a spawned thread holds the same shared object: the two threads race on that field.\n      FIX: make '", 108LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3335 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3334); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` or guard the object with a `Mutex[T]`, or finish the writes before spawning / after the threads are joined.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3335); _tr_str_release(_ord_t3335); });
#line 957 "src/racecheck.tr"
            _tr_str_release(cn);
            return;
        }
#line 958 "src/racecheck.tr"
        k = (k + 1LL);
    }
    _tr_str_release(cn);
}

__attribute__((hot)) void RcCk_check_globals(RcCk* self, RcFn* f, RcSite* s, long long ti, TrStr label) {
#line 961 "src/racecheck.tr"
    List_i64* reach = RcCk_reach(self, ti);
#line 963 "src/racecheck.tr"
    long long gi = 0LL;
#line 964 "src/racecheck.tr"
    while ((gi < self->gnames->len)) {
#line 965 "src/racecheck.tr"
        if ((!List_bool_get(self->gsync, gi))) {
#line 966 "src/racecheck.tr"
            TrStr g = List_TrStr_get(self->gnames, gi);
#line 967 "src/racecheck.tr"
            bool wr = false;
#line 968 "src/racecheck.tr"
            bool rd = false;
#line 969 "src/racecheck.tr"
            bool called = false;
#line 970 "src/racecheck.tr"
            bool mwrites = false;
#line 971 "src/racecheck.tr"
            long long ri = 0LL;
#line 972 "src/racecheck.tr"
            while ((ri < reach->len)) {
#line 973 "src/racecheck.tr"
                RcFn* rf = ({ __auto_type _ord_t3336 = (List_i64_get(reach, ri)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3336))); _wr; });
#line 974 "src/racecheck.tr"
                long long ti2 = 0LL;
#line 975 "src/racecheck.tr"
                while ((ti2 < rf->touched->len)) {
#line 976 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3337 = (List_TrStr_get(rf->touched, ti2)); TrStr _cmpb_t3338 = (g); _Bool _cmpr_t3339 = _tr_str_eqv(_cmpa_t3337, _cmpb_t3338); _tr_str_release(_cmpa_t3337); _cmpr_t3339; })) {
#line 977 "src/racecheck.tr"
                        if (List_bool_get(rf->touched_w, ti2)) {
#line 977 "src/racecheck.tr"
                            wr = true;
                        } else {
#line 978 "src/racecheck.tr"
                            rd = true;
                        }
                    }
#line 979 "src/racecheck.tr"
                    ti2 = (ti2 + 1LL);
                }
#line 980 "src/racecheck.tr"
                if (({ TrStr _at_t3340 = (List_TrStr_get(self->gtn, gi)); __auto_type _wr = ((RcCk_find_class(self, _at_t3340) >= 0LL)); _tr_str_release(_at_t3340); _wr; })) {
#line 982 "src/racecheck.tr"
                    long long mq = 0LL;
#line 983 "src/racecheck.tr"
                    while ((mq < rf->mcrecv->len)) {
#line 984 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3341 = (List_TrStr_get(rf->mcrecv, mq)); TrStr _cmpb_t3342 = (g); _Bool _cmpr_t3343 = _tr_str_eqv(_cmpa_t3341, _cmpb_t3342); _tr_str_release(_cmpa_t3341); _cmpr_t3343; }) && (!_rc_has(rf->locals, g))) && (!_rc_has(rf->params, g)))) {
#line 984 "src/racecheck.tr"
                            called = true;
                        }
#line 985 "src/racecheck.tr"
                        mq = (mq + 1LL);
                    }
#line 986 "src/racecheck.tr"
                    TrStr pre = ({ TrStr _tr_cl = (List_TrStr_get(self->gtn, gi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 987 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3344 = (_tr_str_lenv((rf->name))); __auto_type _wr = ((_ord_t3344 > _tr_str_lenv((pre)))); _wr; }) && ({ __auto_type _ord_t3345 = (_tr_str_lenv((pre))); __auto_type _wr = (({ TrStr _cmpa_t3346 = (_tr_str_slicev((rf->name), 0LL, _ord_t3345)); TrStr _cmpb_t3347 = (pre); _Bool _cmpr_t3348 = _tr_str_eqv(_cmpa_t3346, _cmpb_t3347); _tr_str_release(_cmpa_t3346); _cmpr_t3348; })); _wr; }))) {
#line 988 "src/racecheck.tr"
                        long long mk = 0LL;
#line 989 "src/racecheck.tr"
                        while ((mk < rf->wroot->len)) {
#line 990 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3349 = (List_TrStr_get(rf->wroot, mk)); TrStr _cmpb_t3350 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3351 = _tr_str_eqv(_cmpa_t3349, _cmpb_t3350); _tr_str_release(_cmpa_t3349); _cmpr_t3351; }) && ({ TrStr _cmpa_t3352 = (List_TrStr_get(rf->wprop, mk)); TrStr _cmpb_t3353 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3354 = !_tr_str_eqv(_cmpa_t3352, _cmpb_t3353); _tr_str_release(_cmpa_t3352); _cmpr_t3354; })) && ({ TrStr _ord_t3355 = (List_TrStr_get(self->gtn, gi)); TrStr _ord_t3356 = (List_TrStr_get(rf->wprop, mk)); __auto_type _wr = (RcCk_field_plain(self, _ord_t3355, _ord_t3356)); _tr_str_release(_ord_t3355); _tr_str_release(_ord_t3356); _wr; }))) {
#line 990 "src/racecheck.tr"
                                mwrites = true;
                            }
#line 991 "src/racecheck.tr"
                            mk = (mk + 1LL);
                        }
                    }
                }
#line 992 "src/racecheck.tr"
                ri = (ri + 1LL);
            }
#line 993 "src/racecheck.tr"
            if ((called && mwrites)) {
#line 993 "src/racecheck.tr"
                wr = true;
            }
#line 994 "src/racecheck.tr"
            if ((rd && (!wr))) {
#line 995 "src/racecheck.tr"
                long long fi = 0LL;
#line 996 "src/racecheck.tr"
                while ((fi < self->fns->len)) {
#line 997 "src/racecheck.tr"
                    RcFn* of = ((RcFn*)List_ptr_get(self->fns, fi));
#line 998 "src/racecheck.tr"
                    if ((!of->is_lib)) {
#line 999 "src/racecheck.tr"
                        long long oi = 0LL;
#line 1000 "src/racecheck.tr"
                        while ((oi < of->touched->len)) {
#line 1001 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3357 = (List_TrStr_get(of->touched, oi)); TrStr _cmpb_t3358 = (g); _Bool _cmpr_t3359 = _tr_str_eqv(_cmpa_t3357, _cmpb_t3358); _tr_str_release(_cmpa_t3357); _cmpr_t3359; }) && List_bool_get(of->touched_w, oi)) && List_bool_get(of->touched_after, oi))) {
#line 1001 "src/racecheck.tr"
                                wr = true;
                            }
#line 1002 "src/racecheck.tr"
                            oi = (oi + 1LL);
                        }
                    }
#line 1003 "src/racecheck.tr"
                    fi = (fi + 1LL);
                }
            }
#line 1004 "src/racecheck.tr"
            if (wr) {
#line 1005 "src/racecheck.tr"
                ({ TrStr _ord_t3360 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] the spawned function '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (label)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses the module-level mutable '", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is plain data shared by every thread: concurrent access is a data race.\n      FIX: make '", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (counters/flags), a `Mutex[T]`/`RwLock[T]` (compound state), or pass the state to the thread explicitly.", 121LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3360); _tr_str_release(_ord_t3360); });
            } else if (rd) {
#line 1007 "src/racecheck.tr"
                /* pass */
            }
        }
#line 1008 "src/racecheck.tr"
        gi = (gi + 1LL);
    }
}

__attribute__((hot)) TrStr RcCk_param_write_prop(RcCk* self, long long fi, TrStr pname, TrStr cn, long long depth) {
#line 1013 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, fi));
#line 1014 "src/racecheck.tr"
    long long wi = 0LL;
#line 1015 "src/racecheck.tr"
    while ((wi < tf->wroot->len)) {
#line 1016 "src/racecheck.tr"
        if (((({ TrStr _cmpa_t3361 = (List_TrStr_get(tf->wroot, wi)); TrStr _cmpb_t3362 = (pname); _Bool _cmpr_t3363 = _tr_str_eqv(_cmpa_t3361, _cmpb_t3362); _tr_str_release(_cmpa_t3361); _cmpr_t3363; }) && ({ TrStr _cmpa_t3364 = (List_TrStr_get(tf->wprop, wi)); TrStr _cmpb_t3365 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3366 = !_tr_str_eqv(_cmpa_t3364, _cmpb_t3365); _tr_str_release(_cmpa_t3364); _cmpr_t3366; })) && ({ TrStr _at_t3367 = (List_TrStr_get(tf->wprop, wi)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3367)); _tr_str_release(_at_t3367); _wr; }))) {
#line 1016 "src/racecheck.tr"
            TrStr _trret565 = List_TrStr_get(tf->wprop, wi);
            return _trret565;
        }
#line 1017 "src/racecheck.tr"
        wi = (wi + 1LL);
    }
#line 1018 "src/racecheck.tr"
    long long mi = 0LL;
#line 1019 "src/racecheck.tr"
    while ((mi < tf->mcrecv->len)) {
#line 1020 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3368 = (List_TrStr_get(tf->mcrecv, mi)); TrStr _cmpb_t3369 = (pname); _Bool _cmpr_t3370 = _tr_str_eqv(_cmpa_t3368, _cmpb_t3369); _tr_str_release(_cmpa_t3368); _cmpr_t3370; })) {
#line 1021 "src/racecheck.tr"
            long long mf = ({ TrStr _at_t3371 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(tf->mcname, mi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3371)); _tr_str_release(_at_t3371); _wr; });
#line 1022 "src/racecheck.tr"
            if ((mf >= 0LL)) {
#line 1023 "src/racecheck.tr"
                RcFn* mfn = ((RcFn*)List_ptr_get(self->fns, mf));
#line 1024 "src/racecheck.tr"
                long long k = 0LL;
#line 1025 "src/racecheck.tr"
                while ((k < mfn->wroot->len)) {
#line 1026 "src/racecheck.tr"
                    if (((({ TrStr _cmpa_t3372 = (List_TrStr_get(mfn->wroot, k)); TrStr _cmpb_t3373 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3374 = _tr_str_eqv(_cmpa_t3372, _cmpb_t3373); _tr_str_release(_cmpa_t3372); _cmpr_t3374; }) && ({ TrStr _cmpa_t3375 = (List_TrStr_get(mfn->wprop, k)); TrStr _cmpb_t3376 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3377 = !_tr_str_eqv(_cmpa_t3375, _cmpb_t3376); _tr_str_release(_cmpa_t3375); _cmpr_t3377; })) && ({ TrStr _at_t3378 = (List_TrStr_get(mfn->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3378)); _tr_str_release(_at_t3378); _wr; }))) {
#line 1026 "src/racecheck.tr"
                        TrStr _trret566 = List_TrStr_get(mfn->wprop, k);
                        return _trret566;
                    }
#line 1027 "src/racecheck.tr"
                    k = (k + 1LL);
                }
            }
        }
#line 1028 "src/racecheck.tr"
        mi = (mi + 1LL);
    }
#line 1029 "src/racecheck.tr"
    if ((depth < 4LL)) {
#line 1030 "src/racecheck.tr"
        long long ci = 0LL;
#line 1031 "src/racecheck.tr"
        while ((ci < tf->ca_fn->len)) {
#line 1032 "src/racecheck.tr"
            if (({ TrStr _cmpa_t3379 = (List_TrStr_get(tf->ca_name, ci)); TrStr _cmpb_t3380 = (pname); _Bool _cmpr_t3381 = _tr_str_eqv(_cmpa_t3379, _cmpb_t3380); _tr_str_release(_cmpa_t3379); _cmpr_t3381; })) {
#line 1033 "src/racecheck.tr"
                long long ni = ({ TrStr _at_t3382 = (List_TrStr_get(tf->ca_fn, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3382)); _tr_str_release(_at_t3382); _wr; });
#line 1034 "src/racecheck.tr"
                if (((ni >= 0LL) && ({ __auto_type _ord_t3383 = (List_i64_get(tf->ca_idx, ci)); __auto_type _wr = ((_ord_t3383 < ((RcFn*)List_ptr_get(self->fns, ni))->params->len)); _wr; }))) {
#line 1035 "src/racecheck.tr"
                    TrStr r = ({ __auto_type _ord_t3384 = (List_i64_get(tf->ca_idx, ci)); TrStr _at_t3385 = (List_TrStr_get(((RcFn*)List_ptr_get(self->fns, ni))->params, _ord_t3384)); __auto_type _wr = (RcCk_param_write_prop(self, ni, _at_t3385, cn, (depth + 1LL))); _tr_str_release(_at_t3385); _wr; });
#line 1036 "src/racecheck.tr"
                    if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 1036 "src/racecheck.tr"
                        TrStr _trret567 = _tr_str_retain(r);
                        _tr_str_release(r);
                        return _trret567;
                    }
                }
            }
#line 1037 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
    }
#line 1038 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void RcCk_check_shared_writes(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, long long ai, TrStr an) {
#line 1041 "src/racecheck.tr"
    TrStr pn = List_TrStr_get(tf->params, ai);
#line 1042 "src/racecheck.tr"
    TrStr cn = List_TrStr_get(tf->ptypes, ai);
#line 1043 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("Shared", 6LL)))) {
#line 1043 "src/racecheck.tr"
        TrStr _strtmp_t3386 = List_TrStr_get(tf->pinner, ai);
        _tr_str_release(cn);
        cn = _strtmp_t3386;
    }
#line 1044 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1045 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 1046 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3387 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3387)); _tr_str_release(_apt_t3387); _wr; });
#line 1047 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 1047 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 1048 "src/racecheck.tr"
    TrStr pr = RcCk_param_write_prop(self, ti, pn, cn, 0LL);
#line 1049 "src/racecheck.tr"
    if ((!_tr_str_eqv((pr), (_tr_str_lit_len("", 0LL))))) {
#line 1049 "src/racecheck.tr"
        RcCk_shared_msg(self, f, s, tf, cn, pr, an);
    }
    _tr_str_release(pn);
    _tr_str_release(cn);
    _tr_str_release(pr);
}

__attribute__((hot)) void RcCk_shared_msg(RcCk* self, RcFn* f, RcSite* s, RcFn* tf, TrStr cn, TrStr prop, TrStr an) {
#line 1052 "src/racecheck.tr"
    ({ __auto_type _ord_t3388 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3389 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3388); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' mutates the plain field '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is shared by several threads: `Shared[T]` only makes the reference COUNT atomic, not the fields, so the concurrent writes race.\n      FIX: make '", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (or guard the whole object with `Mutex[", 56LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`), or give each thread its own value.", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3389); _tr_str_release(_ord_t3389); });
}

__attribute__((hot)) TrStr RcCk_where2(RcCk* self, RcFn* f, long long line) {
#line 1055 "src/racecheck.tr"
    long long l = line;
#line 1056 "src/racecheck.tr"
    if ((l <= 0LL)) {
#line 1056 "src/racecheck.tr"
        l = f->line;
    }
#line 1057 "src/racecheck.tr"
    return ({ __auto_type _ord_t3390 = (_tr_strx_concatv((f->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3390); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) RcFn* _rc_new_fn(TrStr name, TrStr file, long long line, bool is_lib) {
#line 66 "src/racecheck.tr"
    RcFn* f = ((RcFn*)_tr_obj_alloc(sizeof(RcFn)));
#line 67 "src/racecheck.tr"
    TrStr _sfo_t3391 = f->name;
    f->name = _tr_str_retain(name);
    _tr_str_release(_sfo_t3391);
#line 68 "src/racecheck.tr"
    TrStr _sfo_t3392 = f->file;
    f->file = _tr_str_retain(file);
    _tr_str_release(_sfo_t3392);
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
#line 118 "src/racecheck.tr"
    return (((((((((((((((_tr_str_eqv((n), (_tr_str_lit_len("Atomic", 6LL))) || _tr_str_eqv((n), (_tr_str_lit_len("Mutex", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("RwLock", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Chan", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadLocal", 11LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Thread", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("ThreadPool", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("AsyncPool", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Shared", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Weak", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("WaitGroup", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Semaphore", 9LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Once", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Barrier", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Condvar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("Channel", 7LL))));
}

__attribute__((hot)) bool _rc_mut_method(TrStr m) {
#line 121 "src/racecheck.tr"
    return (((((((((((((((((_tr_str_eqv((m), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((m), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("sort", 4LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("reverse", 7LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("update", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("delete", 6LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("push_back", 9LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("pop_back", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("truncate", 8LL)))) || _tr_str_eqv((m), (_tr_str_lit_len("swap", 4LL))));
}

__attribute__((hot)) bool _rc_has(List_TrStr* v, TrStr s) {
#line 124 "src/racecheck.tr"
    long long i = 0LL;
#line 125 "src/racecheck.tr"
    while ((i < v->len)) {
#line 126 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3393 = (List_TrStr_get(v, i)); TrStr _cmpb_t3394 = (s); _Bool _cmpr_t3395 = _tr_str_eqv(_cmpa_t3393, _cmpb_t3394); _tr_str_release(_cmpa_t3393); _cmpr_t3395; })) {
#line 126 "src/racecheck.tr"
            return true;
        }
#line 127 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 128 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_nul(Expr* p) {
#line 131 "src/racecheck.tr"
    return (((unsigned long long)(p)) == ((unsigned long long)(0LL)));
}

__attribute__((hot)) TrStr _rc_root(Expr* e) {
#line 135 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 135 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 136 "src/racecheck.tr"
    __auto_type _t3396 = (*e);
    if (_t3396.tag == Expr_EIdent) {
        __auto_type n = _t3396.data.EIdent.name;
        TrStr _trret568 = _tr_str_retain(n);
        return _trret568;
    } else if (_t3396.tag == Expr_EPropAccess) {
        __auto_type o = _t3396.data.EPropAccess.obj;
        return _rc_root(o);
    } else if (_t3396.tag == Expr_EIndex) {
        __auto_type o = _t3396.data.EIndex.obj;
        return _rc_root(o);
    } else if (1) {
        __auto_type _ = _t3396;
        return _tr_str_lit_len("", 0LL);
    }
#line 141 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_first_prop(Expr* e) {
#line 145 "src/racecheck.tr"
    if (_rc_nul(e)) {
#line 145 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 146 "src/racecheck.tr"
    __auto_type _t3397 = (*e);
    if (_t3397.tag == Expr_EPropAccess) {
        __auto_type o = _t3397.data.EPropAccess.obj;
__auto_type p = _t3397.data.EPropAccess.prop;
#line 148 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 149 "src/racecheck.tr"
            __auto_type _t3398 = (*o);
            if (_t3398.tag == Expr_EIdent) {
                __auto_type _ = _t3398.data.EIdent.name;
                TrStr _trret569 = _tr_str_retain(p);
                return _trret569;
            } else if (1) {
                __auto_type _ = _t3398;
                /* pass */
            }
        }
#line 152 "src/racecheck.tr"
        TrStr _trret570 = _rc_first_prop(o);
        return _trret570;
    } else if (_t3397.tag == Expr_EIndex) {
        __auto_type o = _t3397.data.EIndex.obj;
        return _rc_first_prop(o);
    } else if (1) {
        __auto_type _ = _t3397;
        return _tr_str_lit_len("", 0LL);
    }
#line 155 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_ctor_name(Expr* v) {
#line 159 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 159 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 160 "src/racecheck.tr"
    __auto_type _t3399 = (*v);
    if (_t3399.tag == Expr_EMethodCall) {
        __auto_type o = _t3399.data.EMethodCall.obj;
        return _rc_root(o);
    } else if (_t3399.tag == Expr_ECall) {
        __auto_type c = _t3399.data.ECall.callee;
        return _rc_root(c);
    } else if (1) {
        __auto_type _ = _t3399;
        return _tr_str_lit_len("", 0LL);
    }
#line 164 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _rc_ctor_arg(Expr* v) {
#line 168 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 168 "src/racecheck.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 169 "src/racecheck.tr"
    __auto_type _t3400 = (*v);
    if (_t3400.tag == Expr_EMethodCall) {
        __auto_type o = _t3400.data.EMethodCall.obj;
#line 171 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 172 "src/racecheck.tr"
            __auto_type _t3401 = (*o);
            if (_t3401.tag == Expr_EIndex) {
                __auto_type ix = _t3401.data.EIndex._tr_v_index;
#line 174 "src/racecheck.tr"
                if ((!_rc_nul(ix))) {
#line 175 "src/racecheck.tr"
                    __auto_type _t3402 = (*ix);
                    if (_t3402.tag == Expr_EIdent) {
                        __auto_type an = _t3402.data.EIdent.name;
                        TrStr _trret571 = _tr_str_retain(an);
                        return _trret571;
                    } else if (1) {
                        __auto_type _ = _t3402;
                        return _tr_str_lit_len("", 0LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t3401;
                return _tr_str_lit_len("", 0LL);
            }
        }
    } else if (1) {
        __auto_type _ = _t3400;
        return _tr_str_lit_len("", 0LL);
    }
#line 180 "src/racecheck.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long _rc_dot(TrStr n) {
#line 183 "src/racecheck.tr"
    long long i = 0LL;
#line 184 "src/racecheck.tr"
    while ((i < _tr_str_lenv((n)))) {
#line 185 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3403 = (_tr_str_slicev((n), i, (i + 1LL))); TrStr _cmpb_t3404 = (_tr_str_lit_len(".", 1LL)); _Bool _cmpr_t3405 = _tr_str_eqv(_cmpa_t3403, _cmpb_t3404); _tr_str_release(_cmpa_t3403); _cmpr_t3405; })) {
#line 185 "src/racecheck.tr"
            return i;
        }
#line 186 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 187 "src/racecheck.tr"
    return (0LL - 1LL);
}

__attribute__((hot)) bool _rc_is_thread_spawn(Expr* v) {
#line 190 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 190 "src/racecheck.tr"
        return false;
    }
#line 191 "src/racecheck.tr"
    __auto_type _t3406 = (*v);
    if (_t3406.tag == Expr_EMethodCall) {
        __auto_type o = _t3406.data.EMethodCall.obj;
__auto_type m = _t3406.data.EMethodCall.method;
#line 193 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && ({ TrStr _cmpa_t3407 = (_rc_root(o)); TrStr _cmpb_t3408 = (_tr_str_lit_len("Thread", 6LL)); _Bool _cmpr_t3409 = _tr_str_eqv(_cmpa_t3407, _cmpb_t3408); _tr_str_release(_cmpa_t3407); _cmpr_t3409; }))) {
#line 193 "src/racecheck.tr"
            return true;
        }
    } else if (1) {
        __auto_type _ = _t3406;
        /* pass */
    }
#line 195 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_is_lambda(Expr* v) {
#line 198 "src/racecheck.tr"
    if (_rc_nul(v)) {
#line 198 "src/racecheck.tr"
        return false;
    }
#line 199 "src/racecheck.tr"
    __auto_type _t3410 = (*v);
    if (_t3410.tag == Expr_EClosure) {
        return true;
    } else if (1) {
        __auto_type _ = _t3410;
        return false;
    }
#line 202 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) bool _rc_path_lib(TrStr p) {
#line 1061 "src/racecheck.tr"
    return (((_tr_str_containsv((p), (_tr_str_lit_len("std/", 4LL))) || _tr_str_containsv((p), (_tr_str_lit_len("core/", 5LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("std\\", 4LL)))) || _tr_str_containsv((p), (_tr_str_lit_len("core\\", 5LL))));
}

__attribute__((hot)) bool _rc_ifaces_sendable(ClassDef* c) {
#line 1064 "src/racecheck.tr"
    long long i = 0LL;
#line 1065 "src/racecheck.tr"
    while ((i < c->iface_names->len)) {
#line 1066 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3411 = (List_TrStr_get(c->iface_names, i)); TrStr _cmpb_t3412 = (_tr_str_lit_len("Sendable", 8LL)); _Bool _cmpr_t3413 = _tr_str_eqv(_cmpa_t3411, _cmpb_t3412); _tr_str_release(_cmpa_t3411); _cmpr_t3413; })) {
#line 1066 "src/racecheck.tr"
            return true;
        }
#line 1067 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 1068 "src/racecheck.tr"
    return false;
}

__attribute__((hot)) List_TrStr* check_data_races(Program* prog, List_TrStr* decl_files, TrStr input_path) {
#line 1071 "src/racecheck.tr"
    RcCk* ck = ((RcCk*)_tr_obj_alloc(sizeof(RcCk)));
#line 1072 "src/racecheck.tr"
    ck->fns = (void*)List_ptr_new();
#line 1073 "src/racecheck.tr"
    ck->classes = (void*)List_ptr_new();
#line 1074 "src/racecheck.tr"
    ck->class_lib = (void*)List_bool_new();
#line 1075 "src/racecheck.tr"
    ck->gnames = (void*)List_TrStr_new();
#line 1076 "src/racecheck.tr"
    ck->gfiles = (void*)List_TrStr_new();
#line 1077 "src/racecheck.tr"
    ck->gsync = (void*)List_bool_new();
#line 1078 "src/racecheck.tr"
    ck->gtn = (void*)List_TrStr_new();
#line 1079 "src/racecheck.tr"
    ck->msgs = (void*)List_TrStr_new();
#line 1080 "src/racecheck.tr"
    RcFn* _cltmp_t3414 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3414;
#line 1081 "src/racecheck.tr"
    RcFn* _cltmp_t3415 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3415;
#line 1082 "src/racecheck.tr"
    long long i = 0LL;
#line 1083 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1084 "src/racecheck.tr"
        TrStr dfc = _tr_str_retain(input_path);
#line 1085 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1085 "src/racecheck.tr"
            TrStr _strtmp_t3416 = List_TrStr_get(decl_files, i);
            _tr_str_release(dfc);
            dfc = _strtmp_t3416;
        }
#line 1086 "src/racecheck.tr"
        __auto_type _t3417 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3417.tag == Decl_DClass) {
            __auto_type c = _t3417.data.DClass.cls;
#line 1088 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c));
#line 1089 "src/racecheck.tr"
            ({ __auto_type _ord_t3418 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3418); });
#line 1090 "src/racecheck.tr"
            if ((!_rc_path_lib(dfc))) {
#line 1091 "src/racecheck.tr"
                long long fq = 0LL;
#line 1092 "src/racecheck.tr"
                while ((fq < c->fields->len)) {
#line 1093 "src/racecheck.tr"
                    ({ __auto_type _ord_t3419 = (((FieldDef*)List_ptr_get(c->fields, fq))->ty); __auto_type _ord_t3420 = (c->line); RcCk_ty_check(ck, _ord_t3419, dfc, _ord_t3420); });
#line 1094 "src/racecheck.tr"
                    fq = (fq + 1LL);
                }
            }
        } else if (_t3417.tag == Decl_DActor) {
            __auto_type c2 = _t3417.data.DActor.cls;
#line 1096 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c2));
#line 1097 "src/racecheck.tr"
            ({ __auto_type _ord_t3421 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3421); });
        } else if (1) {
            __auto_type _ = _t3417;
            /* pass */
        }
#line 1099 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(dfc);
    }
#line 1100 "src/racecheck.tr"
    i = 0LL;
#line 1101 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1102 "src/racecheck.tr"
        TrStr df = _tr_str_retain(input_path);
#line 1103 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1103 "src/racecheck.tr"
            TrStr _strtmp_t3422 = List_TrStr_get(decl_files, i);
            _tr_str_release(df);
            df = _strtmp_t3422;
        }
#line 1104 "src/racecheck.tr"
        bool lib = _rc_path_lib(df);
#line 1105 "src/racecheck.tr"
        __auto_type _t3423 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3423.tag == Decl_DTopLevelStmt) {
            __auto_type sp = _t3423.data.DTopLevelStmt.stmt;
#line 1107 "src/racecheck.tr"
            __auto_type _t3424 = (*sp);
            if (_t3424.tag == Stmt_SLet) {
                __auto_type name = _t3424.data.SLet.name;
__auto_type is_mut = _t3424.data.SLet.is_mut;
__auto_type is_const = _t3424.data.SLet.is_const;
__auto_type ty = _t3424.data.SLet.ty;
__auto_type v = _t3424.data.SLet.val;
#line 1109 "src/racecheck.tr"
                if ((!lib)) {
#line 1109 "src/racecheck.tr"
                    RcCk_add_global(ck, name, is_mut, is_const, ty, v, df);
                }
            } else if (_t3424.tag == Stmt_SAssign) {
                __auto_type tg = _t3424.data.SAssign.target;
__auto_type v3 = _t3424.data.SAssign.val;
#line 1112 "src/racecheck.tr"
                if (((!lib) && (!_rc_nul(tg)))) {
#line 1113 "src/racecheck.tr"
                    __auto_type _t3425 = (*tg);
                    if (_t3425.tag == Expr_EIdent) {
                        __auto_type gn = _t3425.data.EIdent.name;
                        RcCk_add_global(ck, gn, false, false, (AstType**)(0LL), v3, df);
                    } else if (1) {
                        __auto_type _ = _t3425;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t3424;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t3423;
            /* pass */
        }
#line 1118 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df);
    }
#line 1119 "src/racecheck.tr"
    i = 0LL;
#line 1120 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1121 "src/racecheck.tr"
        TrStr df2 = _tr_str_retain(input_path);
#line 1122 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1122 "src/racecheck.tr"
            TrStr _strtmp_t3426 = List_TrStr_get(decl_files, i);
            _tr_str_release(df2);
            df2 = _strtmp_t3426;
        }
#line 1123 "src/racecheck.tr"
        bool lib2 = _rc_path_lib(df2);
#line 1124 "src/racecheck.tr"
        __auto_type _t3427 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3427.tag == Decl_DFunction) {
            __auto_type f = _t3427.data.DFunction.func;
            RcCk_add_fn(ck, f, df2, lib2);
        } else if (_t3427.tag == Decl_DClass) {
            __auto_type c3 = _t3427.data.DClass.cls;
#line 1127 "src/racecheck.tr"
            long long mi = 0LL;
#line 1128 "src/racecheck.tr"
            while ((mi < c3->methods->len)) {
#line 1129 "src/racecheck.tr"
                ({ __auto_type _ord_t3428 = (((FunctionDef*)List_ptr_get(c3->methods, mi))); __auto_type _ord_t3429 = (c3->name); RcCk_add_method(ck, _ord_t3428, _ord_t3429, df2, lib2); });
#line 1130 "src/racecheck.tr"
                mi = (mi + 1LL);
            }
        } else if (_t3427.tag == Decl_DActor) {
            __auto_type c4 = _t3427.data.DActor.cls;
#line 1132 "src/racecheck.tr"
            long long mi2 = 0LL;
#line 1133 "src/racecheck.tr"
            while ((mi2 < c4->methods->len)) {
#line 1134 "src/racecheck.tr"
                ({ __auto_type _ord_t3430 = (((FunctionDef*)List_ptr_get(c4->methods, mi2))); __auto_type _ord_t3431 = (c4->name); RcCk_add_method(ck, _ord_t3430, _ord_t3431, df2, lib2); });
#line 1135 "src/racecheck.tr"
                mi2 = (mi2 + 1LL);
            }
        } else if (_t3427.tag == Decl_DExtend) {
            __auto_type tgt = _t3427.data.DExtend.target;
__auto_type methods = _t3427.data.DExtend.methods;
#line 1137 "src/racecheck.tr"
            long long mi3 = 0LL;
#line 1138 "src/racecheck.tr"
            while ((mi3 < methods->len)) {
#line 1139 "src/racecheck.tr"
                RcCk_add_method(ck, ((FunctionDef*)List_ptr_get(methods, mi3)), tgt, df2, lib2);
#line 1140 "src/racecheck.tr"
                mi3 = (mi3 + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t3427;
            /* pass */
        }
#line 1142 "src/racecheck.tr"
        i = (i + 1LL);
        _tr_str_release(df2);
    }
#line 1143 "src/racecheck.tr"
    i = 0LL;
#line 1144 "src/racecheck.tr"
    while ((i < ck->fns->len)) {
#line 1145 "src/racecheck.tr"
        RcFn* f = ((RcFn*)List_ptr_get(ck->fns, i));
#line 1146 "src/racecheck.tr"
        if ((!f->is_lib)) {
#line 1147 "src/racecheck.tr"
            long long mj = 0LL;
#line 1148 "src/racecheck.tr"
            while ((mj < f->msgs->len)) {
#line 1149 "src/racecheck.tr"
                ({ TrStr _ord_t3432 = (List_TrStr_get(f->msgs, mj)); List_TrStr_append(ck->msgs, _ord_t3432); _tr_str_release(_ord_t3432); });
#line 1150 "src/racecheck.tr"
                mj = (mj + 1LL);
            }
#line 1151 "src/racecheck.tr"
            long long sj = 0LL;
#line 1152 "src/racecheck.tr"
            while ((sj < f->sites->len)) {
#line 1153 "src/racecheck.tr"
                RcCk_analyze_site(ck, f, ((RcSite*)List_ptr_get(f->sites, sj)));
#line 1154 "src/racecheck.tr"
                sj = (sj + 1LL);
            }
        }
#line 1155 "src/racecheck.tr"
        i = (i + 1LL);
    }
#line 1157 "src/racecheck.tr"
    long long pi = 0LL;
#line 1158 "src/racecheck.tr"
    while ((pi < ck->fns->len)) {
#line 1159 "src/racecheck.tr"
        RcFn* pf = ((RcFn*)List_ptr_get(ck->fns, pi));
#line 1160 "src/racecheck.tr"
        long long pk = 0LL;
#line 1161 "src/racecheck.tr"
        while ((pk < pf->pairs_a->len)) {
#line 1162 "src/racecheck.tr"
            long long qi = (pi + 1LL);
#line 1163 "src/racecheck.tr"
            while ((qi < ck->fns->len)) {
#line 1164 "src/racecheck.tr"
                RcFn* qf = ((RcFn*)List_ptr_get(ck->fns, qi));
#line 1165 "src/racecheck.tr"
                long long qk = 0LL;
#line 1166 "src/racecheck.tr"
                while ((qk < qf->pairs_a->len)) {
#line 1167 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3433 = (List_TrStr_get(qf->pairs_a, qk)); __auto_type _wr = (({ TrStr _cmpa_t3434 = (_ord_t3433); TrStr _cmpb_t3435 = (List_TrStr_get(pf->pairs_b, pk)); _Bool _cmpr_t3436 = _tr_str_eqv(_cmpa_t3434, _cmpb_t3435); _tr_str_release(_cmpa_t3434); _tr_str_release(_cmpb_t3435); _cmpr_t3436; })); _wr; }) && ({ __auto_type _ord_t3437 = (List_TrStr_get(qf->pairs_b, qk)); __auto_type _wr = (({ TrStr _cmpa_t3438 = (_ord_t3437); TrStr _cmpb_t3439 = (List_TrStr_get(pf->pairs_a, pk)); _Bool _cmpr_t3440 = _tr_str_eqv(_cmpa_t3438, _cmpb_t3439); _tr_str_release(_cmpa_t3438); _tr_str_release(_cmpb_t3439); _cmpr_t3440; })); _wr; }))) {
#line 1168 "src/racecheck.tr"
                        ({ __auto_type _ord_t3441 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("W|", 2LL)), (pf->file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3442 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3441); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pf->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] lock-order inversion: '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3443 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3442); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks '", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3444 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3443); TrStr _tr_cr = (List_TrStr_get(pf->pairs_a, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' then '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3445 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3444); TrStr _tr_cr = (List_TrStr_get(pf->pairs_b, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3446 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3445); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (qf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks them in the opposite order: two threads running them can deadlock.\n      FIX: always acquire the mutexes in one global order.", 133LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(ck->msgs, _ord_t3446); _tr_str_release(_ord_t3446); });
                    }
#line 1169 "src/racecheck.tr"
                    qk = (qk + 1LL);
                }
#line 1170 "src/racecheck.tr"
                qi = (qi + 1LL);
            }
#line 1171 "src/racecheck.tr"
            pk = (pk + 1LL);
        }
#line 1172 "src/racecheck.tr"
        pi = (pi + 1LL);
    }
#line 1173 "src/racecheck.tr"
    return ck->msgs;
}

