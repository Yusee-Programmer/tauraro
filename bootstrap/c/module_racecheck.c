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
            ({ __auto_type _ord_t2956 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((file), (_tr_str_lit_len(":", 1LL)))); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-11] '", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2957 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2956); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ty->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' cannot hold '", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t2958 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2957); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, ((*a)->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[..]' elements: reference-counted shared handles are not supported as collection elements.\n      FIX: wrap the handle in a small class (`class Handle: pub inner: Shared[T]`) and store that, or keep the handles in separate variables.", 232LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t2958); _tr_str_release(_ord_t2958); });
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
    return ({ __auto_type _ord_t2959 = (_tr_strx_concatv((self->cur->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2959); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) TrStr RcCk_ltype(RcCk* self, RcFn* f, TrStr name) {
#line 226 "src/racecheck.tr"
    long long i = (f->params->len - 1LL);
#line 227 "src/racecheck.tr"
    while ((i >= 0LL)) {
#line 228 "src/racecheck.tr"
        if (({ TrStr _cmpa_t2960 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t2961 = (name); _Bool _cmpr_t2962 = _tr_str_eqv(_cmpa_t2960, _cmpb_t2961); _tr_str_release(_cmpa_t2960); _cmpr_t2962; })) {
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
        if (({ TrStr _cmpa_t2963 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t2964 = (name); _Bool _cmpr_t2965 = _tr_str_eqv(_cmpa_t2963, _cmpb_t2964); _tr_str_release(_cmpa_t2963); _cmpr_t2965; })) {
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
        if (({ TrStr _cmpa_t2966 = (List_TrStr_get(f->params, i)); TrStr _cmpb_t2967 = (name); _Bool _cmpr_t2968 = _tr_str_eqv(_cmpa_t2966, _cmpb_t2967); _tr_str_release(_cmpa_t2966); _cmpr_t2968; })) {
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
        if (({ TrStr _cmpa_t2969 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t2970 = (name); _Bool _cmpr_t2971 = _tr_str_eqv(_cmpa_t2969, _cmpb_t2970); _tr_str_release(_cmpa_t2969); _cmpr_t2971; })) {
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
        TrStr _trret557 = RcCk_ltarg_of(self, f, name);
        _tr_str_release(t);
        return _trret557;
    }
#line 251 "src/racecheck.tr"
    TrStr _trret558 = _tr_str_retain(t);
    _tr_str_release(t);
    return _trret558;
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
        if (({ TrStr _cmpa_t2972 = (List_TrStr_get(f->ltn, i)); TrStr _cmpb_t2973 = (name); _Bool _cmpr_t2974 = _tr_str_eqv(_cmpa_t2972, _cmpb_t2973); _tr_str_release(_cmpa_t2972); _cmpr_t2974; })) {
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
    RcFn* tmp = ({ TrStr _apt_t2975 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t2975, self->cur->line, true)); _tr_str_release(_apt_t2975); _wr; });
#line 264 "src/racecheck.tr"
    RcFn* saved = _tr_obj_retain(self->cur);
#line 265 "src/racecheck.tr"
    long long sl = self->cur_line;
#line 266 "src/racecheck.tr"
    bool ss = self->seen_spawn;
#line 267 "src/racecheck.tr"
    RcFn* _cltmp_t2976 = _tr_obj_retain(tmp);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t2976;
#line 268 "src/racecheck.tr"
    RcCk_stmt(self, sp);
#line 269 "src/racecheck.tr"
    RcFn* _cltmp_t2977 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t2977;
#line 270 "src/racecheck.tr"
    self->cur_line = sl;
#line 271 "src/racecheck.tr"
    self->seen_spawn = ss;
#line 272 "src/racecheck.tr"
    bool _trret559 = _rc_has(tmp->idents, name);
    _tr_obj_release(tmp, _trdrop_RcFn);
    _tr_obj_release(saved, _trdrop_RcFn);
    return _trret559;
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
    ({ TrStr _ord_t2978 = (_rc_first_prop(target)); List_TrStr_append(self->cur->wprop, _ord_t2978); _tr_str_release(_ord_t2978); });
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
        __auto_type _t2979 = (*sp);
        if (_t2979.tag == Stmt_SLine) {
            __auto_type n = _t2979.data.SLine.n;
#line 322 "src/racecheck.tr"
            line = n;
#line 323 "src/racecheck.tr"
            is_line = true;
        } else if (_t2979.tag == Stmt_SExpr) {
            __auto_type e = _t2979.data.SExpr.expr;
            call = e;
        } else if (_t2979.tag == Stmt_SLet) {
            __auto_type lnm = _t2979.data.SLet.name;
__auto_type v = _t2979.data.SLet.val;
#line 326 "src/racecheck.tr"
            call = v;
#line 327 "src/racecheck.tr"
            TrStr _strtmp_t2980 = _tr_str_retain(lnm);
            _tr_str_release(letname);
            letname = _strtmp_t2980;
        } else if (_t2979.tag == Stmt_SAssign) {
            __auto_type v2 = _t2979.data.SAssign.val;
            call = v2;
        } else if (1) {
            __auto_type _ = _t2979;
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
            if (((List_bool_get(grel, gk) && ({ TrStr _cmpa_t2981 = (List_TrStr_get(guards, gk)); TrStr _cmpb_t2982 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t2983 = !_tr_str_eqv(_cmpa_t2981, _cmpb_t2982); _tr_str_release(_cmpa_t2981); _cmpr_t2983; })) && ({ TrStr _at_t2984 = (List_TrStr_get(guards, gk)); __auto_type _wr = (RcCk_mentions(self, sp, _at_t2984)); _tr_str_release(_at_t2984); _wr; }))) {
#line 334 "src/racecheck.tr"
                ({ __auto_type _ord_t2985 = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2986 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2985); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' was obtained from `", 21LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2987 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2986); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()` and is used after the lock was released: the guarded object is then reachable by other threads while this one still reads/writes it (a data race).\n      FIX: keep every use of '", 185LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2988 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2987); TrStr _tr_cr = (List_TrStr_get(guards, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' before `", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t2989 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2988); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` / `", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t2990 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t2989); TrStr _tr_cr = (List_TrStr_get(gmx, gk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(..)`, or copy the data out while the lock is held.", 55LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t2990); _tr_str_release(_ord_t2990); });
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
        __auto_type _t2991 = (*call);
        if (_t2991.tag == Expr_EMethodCall) {
            __auto_type o = _t2991.data.EMethodCall.obj;
__auto_type m = _t2991.data.EMethodCall.method;
__auto_type margs = _t2991.data.EMethodCall.args;
#line 345 "src/racecheck.tr"
            TrStr r = _tr_str_lit_len("", 0LL);
#line 346 "src/racecheck.tr"
            if ((!_rc_nul(o))) {
#line 347 "src/racecheck.tr"
                __auto_type _t2992 = (*o);
                if (_t2992.tag == Expr_EIdent) {
                    __auto_type rn = _t2992.data.EIdent.name;
                    TrStr _strtmp_t2993 = _tr_str_retain(rn);
                    _tr_str_release(r);
                    r = _strtmp_t2993;
                } else if (1) {
                    __auto_type _ = _t2992;
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
                    if (({ TrStr _cmpa_t2994 = (List_TrStr_get(self->gnames, gq)); TrStr _cmpb_t2995 = (r); _Bool _cmpr_t2996 = _tr_str_eqv(_cmpa_t2994, _cmpb_t2995); _tr_str_release(_cmpa_t2994); _cmpr_t2996; })) {
#line 355 "src/racecheck.tr"
                        TrStr _strtmp_t2997 = List_TrStr_get(self->gtn, gq);
                        _tr_str_release(k);
                        k = _strtmp_t2997;
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
                if (({ TrStr _cmpa_t2998 = (List_TrStr_get(names, j)); TrStr _cmpb_t2999 = (r); _Bool _cmpr_t3000 = _tr_str_eqv(_cmpa_t2998, _cmpb_t2999); _tr_str_release(_cmpa_t2998); _cmpr_t3000; })) {
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
                        if (((({ TrStr _cmpa_t3001 = (List_TrStr_get(kinds, gj2)); TrStr _cmpb_t3002 = (_tr_str_lit_len("Mutex", 5LL)); _Bool _cmpr_t3003 = _tr_str_eqv(_cmpa_t3001, _cmpb_t3002); _tr_str_release(_cmpa_t3001); _cmpr_t3003; }) && (List_i64_get(state, gj2) == 1LL)) && ({ TrStr _cmpa_t3004 = (List_TrStr_get(names, gj2)); TrStr _cmpb_t3005 = (r); _Bool _cmpr_t3006 = !_tr_str_eqv(_cmpa_t3004, _cmpb_t3005); _tr_str_release(_cmpa_t3004); _cmpr_t3006; }))) {
#line 375 "src/racecheck.tr"
                            if ((!(((_rc_has(self->cur->locals, r) || _rc_has(self->cur->params, r)) || ({ __auto_type _ord_t3008 = (self->cur->locals); TrStr _at_t3007 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3008, _at_t3007)); _tr_str_release(_at_t3007); _wr; })) || ({ __auto_type _ord_t3010 = (self->cur->params); TrStr _at_t3009 = (List_TrStr_get(names, gj2)); __auto_type _wr = (_rc_has(_ord_t3010, _at_t3009)); _tr_str_release(_at_t3009); _wr; })))) {
#line 376 "src/racecheck.tr"
                                ({ TrStr _ord_t3011 = (List_TrStr_get(names, gj2)); List_TrStr_append(self->cur->pairs_a, _ord_t3011); _tr_str_release(_ord_t3011); });
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
                        ({ TrStr _ord_t3012 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".get()' acquires the lock again while this thread already holds it (no `set()`/`unlock()` since the previous `get()`): the second call blocks forever.\n      FIX: finish with `", 175LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".set(v)` or `", 13LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".unlock()` before locking again, or reuse the first value.", 58LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3012); _tr_str_release(_ord_t3012); });
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
                        if (({ TrStr _cmpa_t3013 = (List_TrStr_get(gmx, gr)); TrStr _cmpb_t3014 = (r); _Bool _cmpr_t3015 = _tr_str_eqv(_cmpa_t3013, _cmpb_t3014); _tr_str_release(_cmpa_t3013); _cmpr_t3015; })) {
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
                        ({ TrStr _ord_t3016 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()' takes the write lock while this thread still holds the read lock from `", 81LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read()`: the write waits for readers (itself included) to leave, which never happens (deadlock).\n      FIX: call `", 115LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".read_unlock()` before `", 24LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".write()`.", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3016); _tr_str_release(_ord_t3016); });
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
                        ({ TrStr _ord_t3017 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where(self, line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-9] thread '", 14LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (r)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is joined twice: joining an already-joined thread handle is undefined behaviour.\n      FIX: join each thread exactly once.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3017); _tr_str_release(_ord_t3017); });
                    }
#line 402 "src/racecheck.tr"
                    List_i64_set(state, idx, 1LL);
                }
            }
            _tr_str_release(r);
            _tr_str_release(k);
        } else if (1) {
            __auto_type _ = _t2991;
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
    __auto_type _t3018 = (*sp);
    if (_t3018.tag == Stmt_SLine) {
        __auto_type n = _t3018.data.SLine.n;
        self->cur_line = n;
    } else if (_t3018.tag == Stmt_SExpr) {
        __auto_type e = _t3018.data.SExpr.expr;
        RcCk_expr(self, e);
    } else if (_t3018.tag == Stmt_SLet) {
        __auto_type name = _t3018.data.SLet.name;
__auto_type is_sh = _t3018.data.SLet.is_shared;
__auto_type ty = _t3018.data.SLet.ty;
__auto_type v = _t3018.data.SLet.val;
#line 411 "src/racecheck.tr"
        ({ TrStr _apt_t3019 = _tr_str_retain(self->cur->file); RcCk_ty_check(self, ty, _apt_t3019, self->cur_line); _tr_str_release(_apt_t3019); });
#line 412 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 413 "src/racecheck.tr"
        RcCk_declare(self, name);
#line 414 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 415 "src/racecheck.tr"
        if ((((unsigned long long)(ty)) != ((unsigned long long)(0LL)))) {
#line 415 "src/racecheck.tr"
            TrStr _strtmp_t3020 = _tr_str_retain((*ty)->name);
            _tr_str_release(tn);
            tn = _strtmp_t3020;
        }
#line 416 "src/racecheck.tr"
        if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 416 "src/racecheck.tr"
            TrStr _strtmp_t3021 = _rc_ctor_name(v);
            _tr_str_release(tn);
            tn = _strtmp_t3021;
        }
#line 417 "src/racecheck.tr"
        TrStr targ = _rc_ctor_arg(v);
#line 418 "src/racecheck.tr"
        if (((((unsigned long long)(ty)) != ((unsigned long long)(0LL))) && ((*ty)->args->len > 0LL))) {
#line 418 "src/racecheck.tr"
            TrStr _strtmp_t3022 = _tr_str_retain((*((AstType**)List_ptr_get((*ty)->args, 0LL)))->name);
            _tr_str_release(targ);
            targ = _strtmp_t3022;
        }
#line 419 "src/racecheck.tr"
        if ((is_sh && (!_rc_nul(v)))) {
#line 420 "src/racecheck.tr"
            __auto_type _t3023 = (*v);
            if (_t3023.tag == Expr_EIdent) {
                __auto_type src = _t3023.data.EIdent.name;
#line 422 "src/racecheck.tr"
                TrStr _strtmp_t3024 = RcCk_ltype(self, self->cur, src);
                _tr_str_release(tn);
                tn = _strtmp_t3024;
#line 423 "src/racecheck.tr"
                TrStr _strtmp_t3025 = RcCk_ltarg_of(self, self->cur, src);
                _tr_str_release(targ);
                targ = _strtmp_t3025;
            } else if (1) {
                __auto_type _ = _t3023;
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
    } else if (_t3018.tag == Stmt_SMultiLet) {
        __auto_type names = _t3018.data.SMultiLet.names;
__auto_type v = _t3018.data.SMultiLet.val;
#line 435 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 436 "src/racecheck.tr"
        long long mi = 0LL;
#line 437 "src/racecheck.tr"
        while ((mi < names->len)) {
#line 438 "src/racecheck.tr"
            ({ TrStr _at_t3026 = (List_TrStr_get(names, mi)); RcCk_declare(self, _at_t3026); _tr_str_release(_at_t3026); });
#line 439 "src/racecheck.tr"
            mi = (mi + 1LL);
        }
    } else if (_t3018.tag == Stmt_SAssign) {
        __auto_type t = _t3018.data.SAssign.target;
__auto_type v = _t3018.data.SAssign.val;
#line 441 "src/racecheck.tr"
        RcCk_expr(self, v);
#line 442 "src/racecheck.tr"
        RcCk_expr(self, t);
#line 443 "src/racecheck.tr"
        RcCk_wpath(self, t);
    } else if (_t3018.tag == Stmt_SReturn) {
        __auto_type v = _t3018.data.SReturn.val;
        RcCk_expr(self, v);
    } else if (_t3018.tag == Stmt_SBreak) {
        __auto_type v = _t3018.data.SBreak.val;
        RcCk_expr(self, v);
    } else if (_t3018.tag == Stmt_SRaise) {
        __auto_type v = _t3018.data.SRaise.val;
        RcCk_expr(self, v);
    } else if (_t3018.tag == Stmt_SUnsafe) {
        __auto_type b = _t3018.data.SUnsafe.body;
        RcCk_block(self, b);
    } else if (_t3018.tag == Stmt_SIf) {
        __auto_type c = _t3018.data.SIf.cond;
__auto_type tb = _t3018.data.SIf.then_b;
__auto_type elifs = _t3018.data.SIf.elifs;
__auto_type eb = _t3018.data.SIf.else_b;
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
    } else if (_t3018.tag == Stmt_SWhile) {
        __auto_type c = _t3018.data.SWhile.cond;
__auto_type b = _t3018.data.SWhile.body;
#line 458 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 459 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 460 "src/racecheck.tr"
        RcCk_block(self, b);
#line 461 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3018.tag == Stmt_SFor) {
        __auto_type v = _t3018.data.SFor.var;
__auto_type it = _t3018.data.SFor.iter;
__auto_type b = _t3018.data.SFor.body;
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
    } else if (_t3018.tag == Stmt_SForUnpack) {
        __auto_type vs = _t3018.data.SForUnpack.vars;
__auto_type it = _t3018.data.SForUnpack.iter;
__auto_type b = _t3018.data.SForUnpack.body;
#line 469 "src/racecheck.tr"
        RcCk_expr(self, it);
#line 470 "src/racecheck.tr"
        long long fi = 0LL;
#line 471 "src/racecheck.tr"
        while ((fi < vs->len)) {
#line 472 "src/racecheck.tr"
            ({ TrStr _at_t3027 = (List_TrStr_get(vs, fi)); RcCk_declare(self, _at_t3027); _tr_str_release(_at_t3027); });
#line 473 "src/racecheck.tr"
            fi = (fi + 1LL);
        }
#line 474 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth + 1LL);
#line 475 "src/racecheck.tr"
        RcCk_block(self, b);
#line 476 "src/racecheck.tr"
        self->loop_depth = (self->loop_depth - 1LL);
    } else if (_t3018.tag == Stmt_SMatch) {
        __auto_type e = _t3018.data.SMatch.expr;
__auto_type arms = _t3018.data.SMatch.arms;
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
    } else if (_t3018.tag == Stmt_STry) {
        __auto_type tb = _t3018.data.STry.try_body;
__auto_type catches = _t3018.data.STry.catches;
__auto_type fb = _t3018.data.STry.finally_b;
#line 485 "src/racecheck.tr"
        RcCk_block(self, tb);
#line 486 "src/racecheck.tr"
        long long ci = 0LL;
#line 487 "src/racecheck.tr"
        while ((ci < catches->len)) {
#line 488 "src/racecheck.tr"
            ({ TrStr _apt_t3028 = _tr_str_retain((*((CatchClause**)List_ptr_get(catches, ci)))->err_name); RcCk_declare(self, _apt_t3028); _tr_str_release(_apt_t3028); });
#line 489 "src/racecheck.tr"
            RcCk_pblock(self, (*((CatchClause**)List_ptr_get(catches, ci)))->body);
#line 490 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 491 "src/racecheck.tr"
        RcCk_block(self, fb);
    } else if (_t3018.tag == Stmt_SAssert) {
        __auto_type c = _t3018.data.SAssert.cond;
__auto_type m = _t3018.data.SAssert.msg;
#line 493 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 494 "src/racecheck.tr"
        RcCk_expr(self, m);
    } else if (_t3018.tag == Stmt_SWith) {
        __auto_type items = _t3018.data.SWith.items;
__auto_type aliases = _t3018.data.SWith.aliases;
__auto_type b = _t3018.data.SWith.body;
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
            ({ TrStr _at_t3029 = (List_TrStr_get(aliases, wi)); RcCk_declare(self, _at_t3029); _tr_str_release(_at_t3029); });
#line 503 "src/racecheck.tr"
            wi = (wi + 1LL);
        }
#line 504 "src/racecheck.tr"
        RcCk_block(self, b);
    } else if (_t3018.tag == Stmt_SSpawn) {
        __auto_type e = _t3018.data.SSpawn.expr;
#line 506 "src/racecheck.tr"
        RcCk_spawn_expr(self, e);
    } else if (_t3018.tag == Stmt_STaskGroup) {
        __auto_type b = _t3018.data.STaskGroup.body;
#line 508 "src/racecheck.tr"
        RcCk_block(self, b);
#line 509 "src/racecheck.tr"
        self->seen_spawn = false;
    } else if (_t3018.tag == Stmt_SGpuBlock) {
        __auto_type b = _t3018.data.SGpuBlock.body;
        RcCk_block(self, b);
    } else if (_t3018.tag == Stmt_SDefer) {
        __auto_type s = _t3018.data.SDefer.stmt;
        RcCk_stmt(self, s);
    } else if (_t3018.tag == Stmt_SLocalDecl) {
        __auto_type d = _t3018.data.SLocalDecl.decl;
#line 513 "src/racecheck.tr"
        __auto_type _t3030 = (*d);
        if (_t3030.tag == Decl_DFunction) {
            __auto_type f = _t3030.data.DFunction.func;
#line 515 "src/racecheck.tr"
            long long nidx = self->fns->len;
#line 516 "src/racecheck.tr"
            ({ TrStr _apt_t3031 = _tr_str_retain(self->cur->file); RcCk_add_fn(self, f, _apt_t3031, self->cur->is_lib); _tr_str_release(_apt_t3031); });
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
                        ({ __auto_type _ord_t3032 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[N-3] the nested function '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3033 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3032); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the enclosing function '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3034 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3033); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->cur->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but a nested `def` does not capture the enclosing function's locals (it is an ordinary function).\n      FIX: pass '", 118LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as a parameter, or use a `lambda` (which does capture), or move the value to module level.", 92LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3034); _tr_str_release(_ord_t3034); });
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
            __auto_type _ = _t3030;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t3018;
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
    __auto_type _t3035 = (*e);
    if (_t3035.tag == Expr_ECall) {
        __auto_type c = _t3035.data.ECall.callee;
__auto_type args = _t3035.data.ECall.args;
#line 534 "src/racecheck.tr"
        TrStr tn = _tr_str_lit_len("", 0LL);
#line 535 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 536 "src/racecheck.tr"
            __auto_type _t3036 = (*c);
            if (_t3036.tag == Expr_EIdent) {
                __auto_type n = _t3036.data.EIdent.name;
                TrStr _strtmp_t3037 = _tr_str_retain(n);
                _tr_str_release(tn);
                tn = _strtmp_t3037;
            } else if (1) {
                __auto_type _ = _t3036;
                /* pass */
            }
        }
#line 539 "src/racecheck.tr"
        RcCk_record_site(self, tn, args, 0LL);
#line 540 "src/racecheck.tr"
        RcCk_expr(self, e);
        _tr_str_release(tn);
    } else if (_t3035.tag == Expr_EMethodCall) {
        __auto_type o = _t3035.data.EMethodCall.obj;
__auto_type m = _t3035.data.EMethodCall.method;
__auto_type margs = _t3035.data.EMethodCall.args;
#line 542 "src/racecheck.tr"
        TrStr r = _tr_str_lit_len("", 0LL);
#line 543 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 544 "src/racecheck.tr"
            __auto_type _t3038 = (*o);
            if (_t3038.tag == Expr_EIdent) {
                __auto_type rn = _t3038.data.EIdent.name;
                TrStr _strtmp_t3039 = _tr_str_retain(rn);
                _tr_str_release(r);
                r = _strtmp_t3039;
            } else if (1) {
                __auto_type _ = _t3038;
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
        __auto_type _ = _t3035;
        RcCk_expr(self, e);
    }
}

__attribute__((hot)) void RcCk_record_site(RcCk* self, TrStr target, List_ptr* args, long long skip) {
#line 555 "src/racecheck.tr"
    RcSite* s = ((RcSite*)_tr_obj_alloc(sizeof(RcSite)));
#line 556 "src/racecheck.tr"
    s->target = _tr_str_retain(target);
#line 557 "src/racecheck.tr"
    s->args = (void*)List_ptr_new();
#line 558 "src/racecheck.tr"
    long long i = skip;
#line 559 "src/racecheck.tr"
    while ((i < args->len)) {
#line 560 "src/racecheck.tr"
        ({ __auto_type _ord_t3040 = (((Expr*)List_ptr_get(args, i))); List_ptr_append(s->args, _ord_t3040); });
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
    s->recv = _tr_str_lit_len("", 0LL);
#line 566 "src/racecheck.tr"
    s->meth = _tr_str_lit_len("", 0LL);
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
        ({ TrStr _apt_t3041 = _tr_str_retain((*((Comprehension**)List_ptr_get(gens, i)))->target); RcCk_declare(self, _apt_t3041); _tr_str_release(_apt_t3041); });
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
    __auto_type _t3042 = (*e);
    if (_t3042.tag == Expr_EIdent) {
        __auto_type n = _t3042.data.EIdent.name;
#line 589 "src/racecheck.tr"
        List_TrStr_append(self->cur->idents, n);
#line 590 "src/racecheck.tr"
        RcCk_note(self, n, false);
    } else if (_t3042.tag == Expr_EBinOp) {
        __auto_type l = _t3042.data.EBinOp.left;
__auto_type r = _t3042.data.EBinOp.right;
#line 592 "src/racecheck.tr"
        RcCk_expr(self, l);
#line 593 "src/racecheck.tr"
        RcCk_expr(self, r);
    } else if (_t3042.tag == Expr_EUnaryOp) {
        __auto_type x = _t3042.data.EUnaryOp.expr;
        RcCk_expr(self, x);
    } else if (_t3042.tag == Expr_ECall) {
        __auto_type c = _t3042.data.ECall.callee;
__auto_type args = _t3042.data.ECall.args;
#line 596 "src/racecheck.tr"
        if ((!_rc_nul(c))) {
#line 597 "src/racecheck.tr"
            __auto_type _t3043 = (*c);
            if (_t3043.tag == Expr_EIdent) {
                __auto_type fnm = _t3043.data.EIdent.name;
#line 599 "src/racecheck.tr"
                List_TrStr_append(self->cur->calls, fnm);
#line 600 "src/racecheck.tr"
                long long cai = 0LL;
#line 601 "src/racecheck.tr"
                while ((cai < args->len)) {
#line 602 "src/racecheck.tr"
                    __auto_type _t3044 = (*((Expr*)List_ptr_get(args, cai)));
                    if (_t3044.tag == Expr_EIdent) {
                        __auto_type can = _t3044.data.EIdent.name;
#line 604 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_fn, fnm);
#line 605 "src/racecheck.tr"
                        List_i64_append(self->cur->ca_idx, cai);
#line 606 "src/racecheck.tr"
                        List_TrStr_append(self->cur->ca_name, can);
                    } else if (1) {
                        __auto_type _ = _t3044;
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
                        __auto_type _t3045 = (*((Expr*)List_ptr_get(args, ai)));
                        if (_t3045.tag == Expr_ECall) {
                            __auto_type c2 = _t3045.data.ECall.callee;
__auto_type a2 = _t3045.data.ECall.args;
#line 614 "src/racecheck.tr"
                            TrStr t2 = _tr_str_lit_len("", 0LL);
#line 615 "src/racecheck.tr"
                            if ((!_rc_nul(c2))) {
#line 616 "src/racecheck.tr"
                                __auto_type _t3046 = (*c2);
                                if (_t3046.tag == Expr_EIdent) {
                                    __auto_type n2 = _t3046.data.EIdent.name;
                                    TrStr _strtmp_t3047 = _tr_str_retain(n2);
                                    _tr_str_release(t2);
                                    t2 = _strtmp_t3047;
                                } else if (1) {
                                    __auto_type _ = _t3046;
                                    /* pass */
                                }
                            }
#line 619 "src/racecheck.tr"
                            RcCk_record_site(self, t2, a2, 0LL);
                            _tr_str_release(t2);
                        } else if (1) {
                            __auto_type _ = _t3045;
                            /* pass */
                        }
#line 621 "src/racecheck.tr"
                        ai = (ai + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t3043;
                RcCk_expr(self, c);
            }
        }
#line 623 "src/racecheck.tr"
        RcCk_exprs(self, args);
    } else if (_t3042.tag == Expr_EMethodCall) {
        __auto_type o = _t3042.data.EMethodCall.obj;
__auto_type m = _t3042.data.EMethodCall.method;
__auto_type args = _t3042.data.EMethodCall.args;
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
            __auto_type _t3048 = (*o);
            if (_t3048.tag == Expr_EIdent) {
                __auto_type dn = _t3048.data.EIdent.name;
                TrStr _strtmp_t3049 = _tr_str_retain(dn);
                _tr_str_release(direct);
                direct = _strtmp_t3049;
            } else if (1) {
                __auto_type _ = _t3048;
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
            __auto_type _t3050 = (*((Expr*)List_ptr_get(args, 0LL)));
            if (_t3050.tag == Expr_EIdent) {
                __auto_type tn2 = _t3050.data.EIdent.name;
                TrStr _strtmp_t3051 = _tr_str_retain(tn2);
                _tr_str_release(t);
                t = _strtmp_t3051;
            } else if (_t3050.tag == Expr_EClosure) {
                __auto_type cps = _t3050.data.EClosure.params;
__auto_type cbody = _t3050.data.EClosure.body;
#line 643 "src/racecheck.tr"
                RcFn* tmp = ({ TrStr _apt_t3052 = _tr_str_retain(self->cur->file); __auto_type _wr = (_rc_new_fn(_tr_str_lit_len("", 0LL), _apt_t3052, self->cur->line, true)); _tr_str_release(_apt_t3052); _wr; });
#line 644 "src/racecheck.tr"
                long long ci = 0LL;
#line 645 "src/racecheck.tr"
                while ((ci < cps->len)) {
#line 646 "src/racecheck.tr"
                    ({ __auto_type _ord_t3053 = (((Param*)List_ptr_get(cps, ci))->name); List_TrStr_append(tmp->params, _ord_t3053); });
#line 647 "src/racecheck.tr"
                    ci = (ci + 1LL);
                }
#line 648 "src/racecheck.tr"
                RcFn* saved = _tr_obj_retain(self->cur);
#line 649 "src/racecheck.tr"
                RcFn* _cltmp_t3054 = _tr_obj_retain(tmp);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3054;
#line 650 "src/racecheck.tr"
                RcCk_block(self, cbody);
#line 651 "src/racecheck.tr"
                RcFn* _cltmp_t3055 = _tr_obj_retain(saved);
                _tr_obj_release(self->cur, _trdrop_RcFn);
                self->cur = _cltmp_t3055;
#line 652 "src/racecheck.tr"
                long long ck = 0LL;
#line 653 "src/racecheck.tr"
                while ((ck < tmp->touched->len)) {
#line 654 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3057 = (self->cur->locals); TrStr _at_t3056 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3057, _at_t3056)); _tr_str_release(_at_t3056); _wr; }) || ({ __auto_type _ord_t3059 = (self->cur->params); TrStr _at_t3058 = (List_TrStr_get(tmp->touched, ck)); __auto_type _wr = (_rc_has(_ord_t3059, _at_t3058)); _tr_str_release(_at_t3058); _wr; }))) {
#line 655 "src/racecheck.tr"
                        ({ __auto_type _ord_t3060 = (({ TrStr _tr_cl = (RcCk_where(self, self->cur_line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] the closure passed to `spawn` captures '", 46LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3061 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3060); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' from the spawning function by reference: the new thread would race on it and outlive it.\n      FIX: spawn a named `def` and pass '", 132LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3062 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3061); TrStr _tr_cr = (List_TrStr_get(tmp->touched, ck)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' as an argument (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 85LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->cur->msgs, _ord_t3062); _tr_str_release(_ord_t3062); });
#line 656 "src/racecheck.tr"
                        break;
                    }
#line 657 "src/racecheck.tr"
                    ck = (ck + 1LL);
                }
                _tr_obj_release(tmp, _trdrop_RcFn);
                _tr_obj_release(saved, _trdrop_RcFn);
            } else if (1) {
                __auto_type _ = _t3050;
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
            ({ TrStr _ord_t3063 = (_rc_first_prop(o)); List_TrStr_append(self->cur->wprop, _ord_t3063); _tr_str_release(_ord_t3063); });
#line 663 "src/racecheck.tr"
            List_bool_append(self->cur->wafter, self->seen_spawn);
#line 664 "src/racecheck.tr"
            RcCk_note(self, _tr_v_recv, true);
        }
        _tr_str_release(_tr_v_recv);
        _tr_str_release(direct);
    } else if (_t3042.tag == Expr_EPropAccess) {
        __auto_type o = _t3042.data.EPropAccess.obj;
        RcCk_expr(self, o);
    } else if (_t3042.tag == Expr_EIndex) {
        __auto_type o = _t3042.data.EIndex.obj;
__auto_type i = _t3042.data.EIndex._tr_v_index;
#line 667 "src/racecheck.tr"
        RcCk_expr(self, o);
#line 668 "src/racecheck.tr"
        RcCk_expr(self, i);
    } else if (_t3042.tag == Expr_ECast) {
        __auto_type x = _t3042.data.ECast.expr;
        RcCk_expr(self, x);
    } else if (_t3042.tag == Expr_EFString) {
        __auto_type parts = _t3042.data.EFString.parts;
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
    } else if (_t3042.tag == Expr_ETryExpr) {
        __auto_type x = _t3042.data.ETryExpr.expr;
        RcCk_expr(self, x);
    } else if (_t3042.tag == Expr_EClosure) {
        __auto_type params = _t3042.data.EClosure.params;
__auto_type body = _t3042.data.EClosure.body;
#line 677 "src/racecheck.tr"
        long long ci = 0LL;
#line 678 "src/racecheck.tr"
        while ((ci < params->len)) {
#line 679 "src/racecheck.tr"
            ({ TrStr _apt_t3064 = _tr_str_retain(((Param*)List_ptr_get(params, ci))->name); RcCk_declare(self, _apt_t3064); _tr_str_release(_apt_t3064); });
#line 680 "src/racecheck.tr"
            ci = (ci + 1LL);
        }
#line 681 "src/racecheck.tr"
        RcCk_block(self, body);
    } else if (_t3042.tag == Expr_ESuperMethodCall) {
        __auto_type args = _t3042.data.ESuperMethodCall.args;
        RcCk_exprs(self, args);
    } else if (_t3042.tag == Expr_EList) {
        __auto_type items = _t3042.data.EList.items;
        RcCk_exprs(self, items);
    } else if (_t3042.tag == Expr_ESet) {
        __auto_type items = _t3042.data.ESet.items;
        RcCk_exprs(self, items);
    } else if (_t3042.tag == Expr_ETuple) {
        __auto_type items = _t3042.data.ETuple.items;
        RcCk_exprs(self, items);
    } else if (_t3042.tag == Expr_EDict) {
        __auto_type ks = _t3042.data.EDict.keys;
__auto_type vs = _t3042.data.EDict.vals;
#line 687 "src/racecheck.tr"
        RcCk_exprs(self, ks);
#line 688 "src/racecheck.tr"
        RcCk_exprs(self, vs);
    } else if (_t3042.tag == Expr_EListComp) {
        __auto_type el = _t3042.data.EListComp.element;
__auto_type gens = _t3042.data.EListComp.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3042.tag == Expr_EGeneratorExpr) {
        __auto_type el = _t3042.data.EGeneratorExpr.element;
__auto_type gens = _t3042.data.EGeneratorExpr.generators;
        RcCk_comps(self, el, gens);
    } else if (_t3042.tag == Expr_ESlice) {
        __auto_type a = _t3042.data.ESlice.start;
__auto_type b = _t3042.data.ESlice.stop;
__auto_type c = _t3042.data.ESlice.step;
#line 692 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 693 "src/racecheck.tr"
        RcCk_expr(self, b);
#line 694 "src/racecheck.tr"
        RcCk_expr(self, c);
    } else if (_t3042.tag == Expr_EAwait) {
        __auto_type x = _t3042.data.EAwait.expr;
        RcCk_expr(self, x);
    } else if (_t3042.tag == Expr_EYield) {
        __auto_type x = _t3042.data.EYield.expr;
        RcCk_expr(self, x);
    } else if (_t3042.tag == Expr_ETry) {
        __auto_type tb = _t3042.data.ETry.try_body;
__auto_type catches = _t3042.data.ETry.catches;
__auto_type fb = _t3042.data.ETry.finally_b;
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
    } else if (_t3042.tag == Expr_ERange) {
        __auto_type a = _t3042.data.ERange.start;
__auto_type b = _t3042.data.ERange.end;
#line 705 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 706 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3042.tag == Expr_EIfElse) {
        __auto_type c = _t3042.data.EIfElse.cond;
__auto_type a = _t3042.data.EIfElse.then_expr;
__auto_type b = _t3042.data.EIfElse.else_expr;
#line 708 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 709 "src/racecheck.tr"
        RcCk_expr(self, a);
#line 710 "src/racecheck.tr"
        RcCk_expr(self, b);
    } else if (_t3042.tag == Expr_EDo) {
        __auto_type b = _t3042.data.EDo.body;
        RcCk_block(self, b);
    } else if (_t3042.tag == Expr_EMatch) {
        __auto_type s = _t3042.data.EMatch.subj;
__auto_type arms = _t3042.data.EMatch.arms;
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
    } else if (_t3042.tag == Expr_EMacroCall) {
        __auto_type args = _t3042.data.EMacroCall.args;
        RcCk_exprs(self, args);
    } else if (_t3042.tag == Expr_ELoop) {
        __auto_type b = _t3042.data.ELoop.body;
        RcCk_block(self, b);
    } else if (_t3042.tag == Expr_EWhileExpr) {
        __auto_type c = _t3042.data.EWhileExpr.cond;
__auto_type b = _t3042.data.EWhileExpr.body;
__auto_type eb = _t3042.data.EWhileExpr.else_body;
#line 722 "src/racecheck.tr"
        RcCk_expr(self, c);
#line 723 "src/racecheck.tr"
        RcCk_block(self, b);
#line 724 "src/racecheck.tr"
        RcCk_block(self, eb);
    } else if (1) {
        __auto_type _ = _t3042;
        /* pass */
    }
}

__attribute__((hot)) void RcCk_add_method(RcCk* self, FunctionDef* f, TrStr cls, TrStr file, bool lib) {
#line 728 "src/racecheck.tr"
    ({ TrStr _at_t3065 = (({ TrStr _tr_cl = (_tr_strx_concatv((cls), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_add_fn_named(self, f, _at_t3065, file, lib); _tr_str_release(_at_t3065); });
}

__attribute__((hot)) void RcCk_add_fn(RcCk* self, FunctionDef* f, TrStr file, bool lib) {
#line 731 "src/racecheck.tr"
    ({ TrStr _apt_t3066 = _tr_str_retain(f->name); RcCk_add_fn_named(self, f, _apt_t3066, file, lib); _tr_str_release(_apt_t3066); });
}

__attribute__((hot)) void RcCk_add_fn_named(RcCk* self, FunctionDef* f, TrStr nm, TrStr file, bool lib) {
#line 734 "src/racecheck.tr"
    RcFn* rf = _rc_new_fn(nm, file, f->line, (lib || f->is_lib));
#line 735 "src/racecheck.tr"
    long long pi = 0LL;
#line 736 "src/racecheck.tr"
    while ((pi < f->params->len)) {
#line 737 "src/racecheck.tr"
        ({ __auto_type _ord_t3067 = (((Param*)List_ptr_get(f->params, pi))->name); List_TrStr_append(rf->params, _ord_t3067); });
#line 738 "src/racecheck.tr"
        if ((!rf->is_lib)) {
#line 738 "src/racecheck.tr"
            ({ __auto_type _ord_t3068 = (((Param*)List_ptr_get(f->params, pi))->ty); __auto_type _ord_t3069 = (f->line); RcCk_ty_check(self, _ord_t3068, file, _ord_t3069); });
        }
#line 739 "src/racecheck.tr"
        AstType** pt = ((Param*)List_ptr_get(f->params, pi))->ty;
#line 740 "src/racecheck.tr"
        if ((((unsigned long long)(pt)) != ((unsigned long long)(0LL)))) {
#line 741 "src/racecheck.tr"
            ({ __auto_type _ord_t3070 = ((*pt)->name); List_TrStr_append(rf->ptypes, _ord_t3070); });
#line 742 "src/racecheck.tr"
            if (((*pt)->args->len > 0LL)) {
#line 742 "src/racecheck.tr"
                ({ __auto_type _ord_t3071 = ((*((AstType**)List_ptr_get((*pt)->args, 0LL)))->name); List_TrStr_append(rf->pinner, _ord_t3071); });
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
    RcFn* _cltmp_t3072 = _tr_obj_retain(rf);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3072;
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
        if (({ __auto_type _ord_t3074 = (rf->idents); TrStr _at_t3073 = (List_TrStr_get(rf->thr_made, tw)); __auto_type _wr = ((!_rc_has(_ord_t3074, _at_t3073))); _tr_str_release(_at_t3073); _wr; })) {
#line 762 "src/racecheck.tr"
            ({ __auto_type _ord_t3075 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("W|", 2LL)), (file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(List_i64_get(rf->thr_line, tw))))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] the Thread handle '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3076 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3075); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is never joined, detached or used: the thread runs unsupervised and the program may exit while it is still working.\n      FIX: call `", 135LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3077 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3076); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".join()` (wait for it) or `", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3078 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3077); TrStr _tr_cr = (List_TrStr_get(rf->thr_made, tw)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".detach()` (run independently).", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(rf->msgs, _ord_t3078); _tr_str_release(_ord_t3078); });
        }
#line 763 "src/racecheck.tr"
        tw = (tw + 1LL);
    }
#line 764 "src/racecheck.tr"
    RcFn* _cltmp_t3079 = _tr_obj_retain(saved);
    _tr_obj_release(self->cur, _trdrop_RcFn);
    self->cur = _cltmp_t3079;
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
        TrStr _strtmp_t3080 = _tr_str_retain((*ty)->name);
        _tr_str_release(tn);
        tn = _strtmp_t3080;
    }
#line 774 "src/racecheck.tr"
    if (_tr_str_eqv((tn), (_tr_str_lit_len("", 0LL)))) {
#line 774 "src/racecheck.tr"
        TrStr _strtmp_t3081 = _rc_ctor_name(v);
        _tr_str_release(tn);
        tn = _strtmp_t3081;
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
        __auto_type _t3082 = (*v);
        if (_t3082.tag == Expr_ELitInt) {
            __auto_type _ = _t3082.data.ELitInt.val;
            /* pass */
        } else if (_t3082.tag == Expr_ELitFloat) {
            __auto_type _ = _t3082.data.ELitFloat.val;
            /* pass */
        } else if (_t3082.tag == Expr_ELitStr) {
            /* pass */
        } else if (_t3082.tag == Expr_ELitBool) {
            __auto_type _ = _t3082.data.ELitBool.val;
            /* pass */
        } else if (_t3082.tag == Expr_EList) {
            __auto_type _ = _t3082.data.EList.items;
            /* pass */
        } else if (_t3082.tag == Expr_EDict) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t3082;
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
            __auto_type _t3083 = (*v);
            if (_t3083.tag == Expr_EList) {
                __auto_type _ = _t3083.data.EList.items;
                content_mut = true;
            } else if (_t3083.tag == Expr_EDict) {
                content_mut = true;
            } else if (_t3083.tag == Expr_ESet) {
                __auto_type _ = _t3083.data.ESet.items;
                content_mut = true;
            } else if (1) {
                __auto_type _ = _t3083;
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
        RcFn* f = ({ __auto_type _ord_t3084 = (List_i64_get(seen, k)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3084))); _wr; });
#line 840 "src/racecheck.tr"
        long long ci = 0LL;
#line 841 "src/racecheck.tr"
        while ((ci < f->calls->len)) {
#line 842 "src/racecheck.tr"
            long long ni = ({ TrStr _at_t3085 = (List_TrStr_get(f->calls, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3085)); _tr_str_release(_at_t3085); _wr; });
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
                long long dot = ({ TrStr _apt_t3086 = _tr_str_retain(f->name); __auto_type _wr = (_rc_dot(_apt_t3086)); _tr_str_release(_apt_t3086); _wr; });
#line 857 "src/racecheck.tr"
                if ((dot > 0LL)) {
#line 857 "src/racecheck.tr"
                    TrStr _strtmp_t3087 = _tr_str_slicev((f->name), 0LL, dot);
                    _tr_str_release(rcn);
                    rcn = _strtmp_t3087;
                }
            } else {
#line 859 "src/racecheck.tr"
                TrStr _strtmp_t3088 = RcCk_lclass(self, f, rcv);
                _tr_str_release(rcn);
                rcn = _strtmp_t3088;
#line 860 "src/racecheck.tr"
                if (_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL)))) {
#line 861 "src/racecheck.tr"
                    long long gj = 0LL;
#line 862 "src/racecheck.tr"
                    while ((gj < self->gnames->len)) {
#line 863 "src/racecheck.tr"
                        if (({ TrStr _cmpa_t3089 = (List_TrStr_get(self->gnames, gj)); TrStr _cmpb_t3090 = (rcv); _Bool _cmpr_t3091 = _tr_str_eqv(_cmpa_t3089, _cmpb_t3090); _tr_str_release(_cmpa_t3089); _cmpr_t3091; })) {
#line 863 "src/racecheck.tr"
                            TrStr _strtmp_t3092 = List_TrStr_get(self->gtn, gj);
                            _tr_str_release(rcn);
                            rcn = _strtmp_t3092;
                        }
#line 864 "src/racecheck.tr"
                        gj = (gj + 1LL);
                    }
                }
            }
#line 865 "src/racecheck.tr"
            if ((!_tr_str_eqv((rcn), (_tr_str_lit_len("", 0LL))))) {
#line 866 "src/racecheck.tr"
                long long mni = ({ TrStr _at_t3093 = (({ TrStr _tr_cl = (_tr_strx_concatv((rcn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(f->mcname, mci)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3093)); _tr_str_release(_at_t3093); _wr; });
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
    if (((!_tr_str_eqv((s->target), (_tr_str_lit_len("", 0LL)))) && ({ __auto_type _ord_t3095 = (f->closures); TrStr _apt_t3094 = _tr_str_retain(s->target); __auto_type _wr = (_rc_has(_ord_t3095, _apt_t3094)); _tr_str_release(_apt_t3094); _wr; }))) {
#line 881 "src/racecheck.tr"
        ({ __auto_type _ord_t3096 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-6] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3097 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3096); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a closure value: it captures its surrounding locals by reference into this stack frame, so a spawned thread would race on them and outlive them.\n      FIX: spawn a named `def` and pass the data it needs as arguments (an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle).", 289LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3097); _tr_str_release(_ord_t3097); });
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
    long long ti = ({ TrStr _apt_t3098 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3098)); _tr_str_release(_apt_t3098); _wr; });
#line 887 "src/racecheck.tr"
    if ((ti < 0LL)) {
#line 887 "src/racecheck.tr"
        return;
    }
#line 888 "src/racecheck.tr"
    ({ TrStr _apt_t3099 = _tr_str_retain(s->target); RcCk_check_globals(self, f, s, ti, _apt_t3099); _tr_str_release(_apt_t3099); });
#line 890 "src/racecheck.tr"
    RcFn* tf = ((RcFn*)List_ptr_get(self->fns, ti));
#line 891 "src/racecheck.tr"
    long long ai = 0LL;
#line 892 "src/racecheck.tr"
    while (((ai < s->args->len) && (ai < tf->params->len))) {
#line 893 "src/racecheck.tr"
        TrStr an = _tr_str_lit_len("", 0LL);
#line 894 "src/racecheck.tr"
        __auto_type _t3100 = (*((Expr*)List_ptr_get(s->args, ai)));
        if (_t3100.tag == Expr_EIdent) {
            __auto_type nn = _t3100.data.EIdent.name;
            TrStr _strtmp_t3101 = _tr_str_retain(nn);
            _tr_str_release(an);
            an = _strtmp_t3101;
        } else if (1) {
            __auto_type _ = _t3100;
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
                    __auto_type _t3102 = (*((Expr*)List_ptr_get(((RcSite*)List_ptr_get(f->sites, si))->args, oa)));
                    if (_t3102.tag == Expr_EIdent) {
                        __auto_type on = _t3102.data.EIdent.name;
#line 906 "src/racecheck.tr"
                        if (_tr_str_eqv((on), (an))) {
#line 906 "src/racecheck.tr"
                            cnt = (cnt + 1LL);
                        }
                    } else if (1) {
                        __auto_type _ = _t3102;
                        /* pass */
                    }
#line 908 "src/racecheck.tr"
                    oa = (oa + 1LL);
                }
#line 909 "src/racecheck.tr"
                si = (si + 1LL);
            }
#line 910 "src/racecheck.tr"
            bool is_sh = (_rc_has(f->shvars, an) || ({ TrStr _cmpa_t3103 = (RcCk_ltype(self, f, an)); TrStr _cmpb_t3104 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3105 = _tr_str_eqv(_cmpa_t3103, _cmpb_t3104); _tr_str_release(_cmpa_t3103); _cmpr_t3105; }));
#line 911 "src/racecheck.tr"
            if (((is_sh && (!_tr_str_eqv((ptn), (_tr_str_lit_len("Shared", 6LL))))) && (!_tr_str_eqv((ptn), (_tr_str_lit_len("", 0LL)))))) {
#line 912 "src/racecheck.tr"
                ({ __auto_type _ord_t3106 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-10] '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a `Shared` handle but '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3107 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3106); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' declares parameter '", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3108 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3107); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("': the thread would be handed the shared control block where it expects the object itself (a type confusion).\n      FIX: declare the worker's parameter as `", 156LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3109 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3108); TrStr _tr_cr = (List_TrStr_get(tf->params, ai)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": Shared[", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ptn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`.", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3109); _tr_str_release(_ord_t3109); });
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
    TrStr cn = ({ TrStr _apt_t3110 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_lclass(self, f, _apt_t3110)); _tr_str_release(_apt_t3110); _wr; });
#line 928 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 928 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 929 "src/racecheck.tr"
    long long mi = ({ TrStr _at_t3111 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3111)); _tr_str_release(_at_t3111); _wr; });
#line 930 "src/racecheck.tr"
    if ((mi < 0LL)) {
#line 930 "src/racecheck.tr"
        _tr_str_release(cn);
        return;
    }
#line 931 "src/racecheck.tr"
    ({ TrStr _at_t3112 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->meth)); _tr_str_release(_tr_cl); _tr_cres; })); RcCk_check_globals(self, f, s, mi, _at_t3112); _tr_str_release(_at_t3112); });
#line 932 "src/racecheck.tr"
    long long cnt = 0LL;
#line 933 "src/racecheck.tr"
    long long si = 0LL;
#line 934 "src/racecheck.tr"
    while ((si < f->sites->len)) {
#line 935 "src/racecheck.tr"
        if (({ __auto_type _ord_t3113 = (((RcSite*)List_ptr_get(f->sites, si))->recv); __auto_type _wr = (_tr_str_eqv((_ord_t3113), (s->recv))); _wr; })) {
#line 935 "src/racecheck.tr"
            cnt = (cnt + 1LL);
        }
#line 936 "src/racecheck.tr"
        si = (si + 1LL);
    }
#line 937 "src/racecheck.tr"
    bool is_sh = (({ __auto_type _ord_t3115 = (f->shvars); TrStr _apt_t3114 = _tr_str_retain(s->recv); __auto_type _wr = (_rc_has(_ord_t3115, _apt_t3114)); _tr_str_release(_apt_t3114); _wr; }) || ({ TrStr _apt_t3116 = _tr_str_retain(s->recv); __auto_type _wr = (({ TrStr _cmpa_t3117 = (RcCk_ltype(self, f, _apt_t3116)); TrStr _cmpb_t3118 = (_tr_str_lit_len("Shared", 6LL)); _Bool _cmpr_t3119 = _tr_str_eqv(_cmpa_t3117, _cmpb_t3118); _tr_str_release(_cmpa_t3117); _cmpr_t3119; })); _tr_str_release(_apt_t3116); _wr; }));
#line 938 "src/racecheck.tr"
    long long dd = ({ TrStr _apt_t3120 = _tr_str_retain(s->recv); __auto_type _wr = (RcCk_ldepth_of(self, f, _apt_t3120)); _tr_str_release(_apt_t3120); _wr; });
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
        if (((({ TrStr _cmpa_t3121 = (List_TrStr_get(mf->wroot, k)); TrStr _cmpb_t3122 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3123 = _tr_str_eqv(_cmpa_t3121, _cmpb_t3122); _tr_str_release(_cmpa_t3121); _cmpr_t3123; }) && ({ TrStr _cmpa_t3124 = (List_TrStr_get(mf->wprop, k)); TrStr _cmpb_t3125 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3126 = !_tr_str_eqv(_cmpa_t3124, _cmpb_t3125); _tr_str_release(_cmpa_t3124); _cmpr_t3126; })) && ({ TrStr _at_t3127 = (List_TrStr_get(mf->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3127)); _tr_str_release(_at_t3127); _wr; }))) {
#line 944 "src/racecheck.tr"
            ({ TrStr _ord_t3128 = (List_TrStr_get(mf->wprop, k)); __auto_type _ord_t3129 = (s->recv); RcCk_shared_msg(self, f, s, mf, cn, _ord_t3128, _ord_t3129); _tr_str_release(_ord_t3128); });
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
        ({ TrStr _apt_t3130 = _tr_str_retain(s->recv); RcCk_check_parent_writes(self, f, s, _apt_t3130); _tr_str_release(_apt_t3130); });
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
        if ((((({ TrStr _cmpa_t3131 = (List_TrStr_get(f->wroot, k)); TrStr _cmpb_t3132 = (an); _Bool _cmpr_t3133 = _tr_str_eqv(_cmpa_t3131, _cmpb_t3132); _tr_str_release(_cmpa_t3131); _cmpr_t3133; }) && List_bool_get(f->wafter, k)) && ({ TrStr _cmpa_t3134 = (List_TrStr_get(f->wprop, k)); TrStr _cmpb_t3135 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3136 = !_tr_str_eqv(_cmpa_t3134, _cmpb_t3135); _tr_str_release(_cmpa_t3134); _cmpr_t3136; })) && ({ TrStr _at_t3137 = (List_TrStr_get(f->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3137)); _tr_str_release(_at_t3137); _wr; }))) {
#line 956 "src/racecheck.tr"
            ({ __auto_type _ord_t3138 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3139 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3138); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' writes the plain field '", 26LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3140 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3139); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while a spawned thread holds the same shared object: the two threads race on that field.\n      FIX: make '", 108LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3141 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3140); TrStr _tr_cr = (List_TrStr_get(f->wprop, k)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` or guard the object with a `Mutex[T]`, or finish the writes before spawning / after the threads are joined.", 124LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3141); _tr_str_release(_ord_t3141); });
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
                RcFn* rf = ({ __auto_type _ord_t3142 = (List_i64_get(reach, ri)); __auto_type _wr = (((RcFn*)List_ptr_get(self->fns, _ord_t3142))); _wr; });
#line 974 "src/racecheck.tr"
                long long ti2 = 0LL;
#line 975 "src/racecheck.tr"
                while ((ti2 < rf->touched->len)) {
#line 976 "src/racecheck.tr"
                    if (({ TrStr _cmpa_t3143 = (List_TrStr_get(rf->touched, ti2)); TrStr _cmpb_t3144 = (g); _Bool _cmpr_t3145 = _tr_str_eqv(_cmpa_t3143, _cmpb_t3144); _tr_str_release(_cmpa_t3143); _cmpr_t3145; })) {
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
                if (({ TrStr _at_t3146 = (List_TrStr_get(self->gtn, gi)); __auto_type _wr = ((RcCk_find_class(self, _at_t3146) >= 0LL)); _tr_str_release(_at_t3146); _wr; })) {
#line 982 "src/racecheck.tr"
                    long long mq = 0LL;
#line 983 "src/racecheck.tr"
                    while ((mq < rf->mcrecv->len)) {
#line 984 "src/racecheck.tr"
                        if (((({ TrStr _cmpa_t3147 = (List_TrStr_get(rf->mcrecv, mq)); TrStr _cmpb_t3148 = (g); _Bool _cmpr_t3149 = _tr_str_eqv(_cmpa_t3147, _cmpb_t3148); _tr_str_release(_cmpa_t3147); _cmpr_t3149; }) && (!_rc_has(rf->locals, g))) && (!_rc_has(rf->params, g)))) {
#line 984 "src/racecheck.tr"
                            called = true;
                        }
#line 985 "src/racecheck.tr"
                        mq = (mq + 1LL);
                    }
#line 986 "src/racecheck.tr"
                    TrStr pre = ({ TrStr _tr_cl = (List_TrStr_get(self->gtn, gi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 987 "src/racecheck.tr"
                    if ((({ __auto_type _ord_t3150 = (_tr_str_lenv((rf->name))); __auto_type _wr = ((_ord_t3150 > _tr_str_lenv((pre)))); _wr; }) && ({ __auto_type _ord_t3151 = (_tr_str_lenv((pre))); __auto_type _wr = (({ TrStr _cmpa_t3152 = (_tr_str_slicev((rf->name), 0LL, _ord_t3151)); TrStr _cmpb_t3153 = (pre); _Bool _cmpr_t3154 = _tr_str_eqv(_cmpa_t3152, _cmpb_t3153); _tr_str_release(_cmpa_t3152); _cmpr_t3154; })); _wr; }))) {
#line 988 "src/racecheck.tr"
                        long long mk = 0LL;
#line 989 "src/racecheck.tr"
                        while ((mk < rf->wroot->len)) {
#line 990 "src/racecheck.tr"
                            if (((({ TrStr _cmpa_t3155 = (List_TrStr_get(rf->wroot, mk)); TrStr _cmpb_t3156 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3157 = _tr_str_eqv(_cmpa_t3155, _cmpb_t3156); _tr_str_release(_cmpa_t3155); _cmpr_t3157; }) && ({ TrStr _cmpa_t3158 = (List_TrStr_get(rf->wprop, mk)); TrStr _cmpb_t3159 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3160 = !_tr_str_eqv(_cmpa_t3158, _cmpb_t3159); _tr_str_release(_cmpa_t3158); _cmpr_t3160; })) && ({ TrStr _ord_t3161 = (List_TrStr_get(self->gtn, gi)); TrStr _ord_t3162 = (List_TrStr_get(rf->wprop, mk)); __auto_type _wr = (RcCk_field_plain(self, _ord_t3161, _ord_t3162)); _tr_str_release(_ord_t3161); _tr_str_release(_ord_t3162); _wr; }))) {
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
                            if (((({ TrStr _cmpa_t3163 = (List_TrStr_get(of->touched, oi)); TrStr _cmpb_t3164 = (g); _Bool _cmpr_t3165 = _tr_str_eqv(_cmpa_t3163, _cmpb_t3164); _tr_str_release(_cmpa_t3163); _cmpr_t3165; }) && List_bool_get(of->touched_w, oi)) && List_bool_get(of->touched_after, oi))) {
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
                ({ TrStr _ord_t3166 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] the spawned function '", 28LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (label)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' uses the module-level mutable '", 33LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is plain data shared by every thread: concurrent access is a data race.\n      FIX: make '", 98LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (g)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (counters/flags), a `Mutex[T]`/`RwLock[T]` (compound state), or pass the state to the thread explicitly.", 121LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3166); _tr_str_release(_ord_t3166); });
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
        if (((({ TrStr _cmpa_t3167 = (List_TrStr_get(tf->wroot, wi)); TrStr _cmpb_t3168 = (pname); _Bool _cmpr_t3169 = _tr_str_eqv(_cmpa_t3167, _cmpb_t3168); _tr_str_release(_cmpa_t3167); _cmpr_t3169; }) && ({ TrStr _cmpa_t3170 = (List_TrStr_get(tf->wprop, wi)); TrStr _cmpb_t3171 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3172 = !_tr_str_eqv(_cmpa_t3170, _cmpb_t3171); _tr_str_release(_cmpa_t3170); _cmpr_t3172; })) && ({ TrStr _at_t3173 = (List_TrStr_get(tf->wprop, wi)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3173)); _tr_str_release(_at_t3173); _wr; }))) {
#line 1016 "src/racecheck.tr"
            TrStr _trret560 = List_TrStr_get(tf->wprop, wi);
            return _trret560;
        }
#line 1017 "src/racecheck.tr"
        wi = (wi + 1LL);
    }
#line 1018 "src/racecheck.tr"
    long long mi = 0LL;
#line 1019 "src/racecheck.tr"
    while ((mi < tf->mcrecv->len)) {
#line 1020 "src/racecheck.tr"
        if (({ TrStr _cmpa_t3174 = (List_TrStr_get(tf->mcrecv, mi)); TrStr _cmpb_t3175 = (pname); _Bool _cmpr_t3176 = _tr_str_eqv(_cmpa_t3174, _cmpb_t3175); _tr_str_release(_cmpa_t3174); _cmpr_t3176; })) {
#line 1021 "src/racecheck.tr"
            long long mf = ({ TrStr _at_t3177 = (({ TrStr _tr_cl = (_tr_strx_concatv((cn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cr = (List_TrStr_get(tf->mcname, mi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (RcCk_find_fn(self, _at_t3177)); _tr_str_release(_at_t3177); _wr; });
#line 1022 "src/racecheck.tr"
            if ((mf >= 0LL)) {
#line 1023 "src/racecheck.tr"
                RcFn* mfn = ((RcFn*)List_ptr_get(self->fns, mf));
#line 1024 "src/racecheck.tr"
                long long k = 0LL;
#line 1025 "src/racecheck.tr"
                while ((k < mfn->wroot->len)) {
#line 1026 "src/racecheck.tr"
                    if (((({ TrStr _cmpa_t3178 = (List_TrStr_get(mfn->wroot, k)); TrStr _cmpb_t3179 = (_tr_str_lit_len("self", 4LL)); _Bool _cmpr_t3180 = _tr_str_eqv(_cmpa_t3178, _cmpb_t3179); _tr_str_release(_cmpa_t3178); _cmpr_t3180; }) && ({ TrStr _cmpa_t3181 = (List_TrStr_get(mfn->wprop, k)); TrStr _cmpb_t3182 = (_tr_str_lit_len("", 0LL)); _Bool _cmpr_t3183 = !_tr_str_eqv(_cmpa_t3181, _cmpb_t3182); _tr_str_release(_cmpa_t3181); _cmpr_t3183; })) && ({ TrStr _at_t3184 = (List_TrStr_get(mfn->wprop, k)); __auto_type _wr = (RcCk_field_plain(self, cn, _at_t3184)); _tr_str_release(_at_t3184); _wr; }))) {
#line 1026 "src/racecheck.tr"
                        TrStr _trret561 = List_TrStr_get(mfn->wprop, k);
                        return _trret561;
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
            if (({ TrStr _cmpa_t3185 = (List_TrStr_get(tf->ca_name, ci)); TrStr _cmpb_t3186 = (pname); _Bool _cmpr_t3187 = _tr_str_eqv(_cmpa_t3185, _cmpb_t3186); _tr_str_release(_cmpa_t3185); _cmpr_t3187; })) {
#line 1033 "src/racecheck.tr"
                long long ni = ({ TrStr _at_t3188 = (List_TrStr_get(tf->ca_fn, ci)); __auto_type _wr = (RcCk_find_fn(self, _at_t3188)); _tr_str_release(_at_t3188); _wr; });
#line 1034 "src/racecheck.tr"
                if (((ni >= 0LL) && ({ __auto_type _ord_t3189 = (List_i64_get(tf->ca_idx, ci)); __auto_type _wr = ((_ord_t3189 < ((RcFn*)List_ptr_get(self->fns, ni))->params->len)); _wr; }))) {
#line 1035 "src/racecheck.tr"
                    TrStr r = ({ __auto_type _ord_t3190 = (List_i64_get(tf->ca_idx, ci)); TrStr _at_t3191 = (List_TrStr_get(((RcFn*)List_ptr_get(self->fns, ni))->params, _ord_t3190)); __auto_type _wr = (RcCk_param_write_prop(self, ni, _at_t3191, cn, (depth + 1LL))); _tr_str_release(_at_t3191); _wr; });
#line 1036 "src/racecheck.tr"
                    if ((!_tr_str_eqv((r), (_tr_str_lit_len("", 0LL))))) {
#line 1036 "src/racecheck.tr"
                        TrStr _trret562 = _tr_str_retain(r);
                        _tr_str_release(r);
                        return _trret562;
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
        TrStr _strtmp_t3192 = List_TrStr_get(tf->pinner, ai);
        _tr_str_release(cn);
        cn = _strtmp_t3192;
    }
#line 1044 "src/racecheck.tr"
    if (_tr_str_eqv((cn), (_tr_str_lit_len("", 0LL)))) {
#line 1045 "src/racecheck.tr"
        _tr_str_release(pn);
        _tr_str_release(cn);
        return;
    }
#line 1046 "src/racecheck.tr"
    long long ti = ({ TrStr _apt_t3193 = _tr_str_retain(s->target); __auto_type _wr = (RcCk_find_fn(self, _apt_t3193)); _tr_str_release(_apt_t3193); _wr; });
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
    ({ __auto_type _ord_t3194 = (({ TrStr _tr_cl = (RcCk_where2(self, f, s->line)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("[T-8] '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3195 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3194); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (s->target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' mutates the plain field '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(".", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' of '", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (an)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', which is shared by several threads: `Shared[T]` only makes the reference COUNT atomic, not the fields, so the concurrent writes race.\n      FIX: make '", 154LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (prop)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' an `Atomic[T]` (or guard the whole object with `Mutex[", 56LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("]`), or give each thread its own value.", 39LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(self->msgs, _ord_t3195); _tr_str_release(_ord_t3195); });
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
    return ({ __auto_type _ord_t3196 = (_tr_strx_concatv((f->file), (_tr_str_lit_len(":", 1LL)))); __auto_type _wr = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3196); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(l)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); _wr; });
}

__attribute__((hot)) RcFn* _rc_new_fn(TrStr name, TrStr file, long long line, bool is_lib) {
#line 66 "src/racecheck.tr"
    RcFn* f = ((RcFn*)_tr_obj_alloc(sizeof(RcFn)));
#line 67 "src/racecheck.tr"
    f->name = _tr_str_retain(name);
#line 68 "src/racecheck.tr"
    f->file = _tr_str_retain(file);
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
        if (({ TrStr _cmpa_t3197 = (List_TrStr_get(v, i)); TrStr _cmpb_t3198 = (s); _Bool _cmpr_t3199 = _tr_str_eqv(_cmpa_t3197, _cmpb_t3198); _tr_str_release(_cmpa_t3197); _cmpr_t3199; })) {
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
    __auto_type _t3200 = (*e);
    if (_t3200.tag == Expr_EIdent) {
        __auto_type n = _t3200.data.EIdent.name;
        TrStr _trret563 = _tr_str_retain(n);
        return _trret563;
    } else if (_t3200.tag == Expr_EPropAccess) {
        __auto_type o = _t3200.data.EPropAccess.obj;
        return _rc_root(o);
    } else if (_t3200.tag == Expr_EIndex) {
        __auto_type o = _t3200.data.EIndex.obj;
        return _rc_root(o);
    } else if (1) {
        __auto_type _ = _t3200;
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
    __auto_type _t3201 = (*e);
    if (_t3201.tag == Expr_EPropAccess) {
        __auto_type o = _t3201.data.EPropAccess.obj;
__auto_type p = _t3201.data.EPropAccess.prop;
#line 148 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 149 "src/racecheck.tr"
            __auto_type _t3202 = (*o);
            if (_t3202.tag == Expr_EIdent) {
                __auto_type _ = _t3202.data.EIdent.name;
                TrStr _trret564 = _tr_str_retain(p);
                return _trret564;
            } else if (1) {
                __auto_type _ = _t3202;
                /* pass */
            }
        }
#line 152 "src/racecheck.tr"
        TrStr _trret565 = _rc_first_prop(o);
        return _trret565;
    } else if (_t3201.tag == Expr_EIndex) {
        __auto_type o = _t3201.data.EIndex.obj;
        return _rc_first_prop(o);
    } else if (1) {
        __auto_type _ = _t3201;
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
    __auto_type _t3203 = (*v);
    if (_t3203.tag == Expr_EMethodCall) {
        __auto_type o = _t3203.data.EMethodCall.obj;
        return _rc_root(o);
    } else if (_t3203.tag == Expr_ECall) {
        __auto_type c = _t3203.data.ECall.callee;
        return _rc_root(c);
    } else if (1) {
        __auto_type _ = _t3203;
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
    __auto_type _t3204 = (*v);
    if (_t3204.tag == Expr_EMethodCall) {
        __auto_type o = _t3204.data.EMethodCall.obj;
#line 171 "src/racecheck.tr"
        if ((!_rc_nul(o))) {
#line 172 "src/racecheck.tr"
            __auto_type _t3205 = (*o);
            if (_t3205.tag == Expr_EIndex) {
                __auto_type ix = _t3205.data.EIndex._tr_v_index;
#line 174 "src/racecheck.tr"
                if ((!_rc_nul(ix))) {
#line 175 "src/racecheck.tr"
                    __auto_type _t3206 = (*ix);
                    if (_t3206.tag == Expr_EIdent) {
                        __auto_type an = _t3206.data.EIdent.name;
                        TrStr _trret566 = _tr_str_retain(an);
                        return _trret566;
                    } else if (1) {
                        __auto_type _ = _t3206;
                        return _tr_str_lit_len("", 0LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t3205;
                return _tr_str_lit_len("", 0LL);
            }
        }
    } else if (1) {
        __auto_type _ = _t3204;
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
        if (({ TrStr _cmpa_t3207 = (_tr_str_slicev((n), i, (i + 1LL))); TrStr _cmpb_t3208 = (_tr_str_lit_len(".", 1LL)); _Bool _cmpr_t3209 = _tr_str_eqv(_cmpa_t3207, _cmpb_t3208); _tr_str_release(_cmpa_t3207); _cmpr_t3209; })) {
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
    __auto_type _t3210 = (*v);
    if (_t3210.tag == Expr_EMethodCall) {
        __auto_type o = _t3210.data.EMethodCall.obj;
__auto_type m = _t3210.data.EMethodCall.method;
#line 193 "src/racecheck.tr"
        if ((_tr_str_eqv((m), (_tr_str_lit_len("spawn", 5LL))) && ({ TrStr _cmpa_t3211 = (_rc_root(o)); TrStr _cmpb_t3212 = (_tr_str_lit_len("Thread", 6LL)); _Bool _cmpr_t3213 = _tr_str_eqv(_cmpa_t3211, _cmpb_t3212); _tr_str_release(_cmpa_t3211); _cmpr_t3213; }))) {
#line 193 "src/racecheck.tr"
            return true;
        }
    } else if (1) {
        __auto_type _ = _t3210;
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
    __auto_type _t3214 = (*v);
    if (_t3214.tag == Expr_EClosure) {
        return true;
    } else if (1) {
        __auto_type _ = _t3214;
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
        if (({ TrStr _cmpa_t3215 = (List_TrStr_get(c->iface_names, i)); TrStr _cmpb_t3216 = (_tr_str_lit_len("Sendable", 8LL)); _Bool _cmpr_t3217 = _tr_str_eqv(_cmpa_t3215, _cmpb_t3216); _tr_str_release(_cmpa_t3215); _cmpr_t3217; })) {
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
    RcFn* _cltmp_t3218 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3218;
#line 1081 "src/racecheck.tr"
    RcFn* _cltmp_t3219 = _rc_new_fn(_tr_str_lit_len("", 0LL), input_path, 0LL, true);
    _tr_obj_release(ck->cur, _trdrop_RcFn);
    ck->cur = _cltmp_t3219;
#line 1082 "src/racecheck.tr"
    long long i = 0LL;
#line 1083 "src/racecheck.tr"
    while ((i < prog->decls->len)) {
#line 1084 "src/racecheck.tr"
        TrStr dfc = _tr_str_retain(input_path);
#line 1085 "src/racecheck.tr"
        if ((decl_files->len == prog->decls->len)) {
#line 1085 "src/racecheck.tr"
            TrStr _strtmp_t3220 = List_TrStr_get(decl_files, i);
            _tr_str_release(dfc);
            dfc = _strtmp_t3220;
        }
#line 1086 "src/racecheck.tr"
        __auto_type _t3221 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3221.tag == Decl_DClass) {
            __auto_type c = _t3221.data.DClass.cls;
#line 1088 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c));
#line 1089 "src/racecheck.tr"
            ({ __auto_type _ord_t3222 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3222); });
#line 1090 "src/racecheck.tr"
            if ((!_rc_path_lib(dfc))) {
#line 1091 "src/racecheck.tr"
                long long fq = 0LL;
#line 1092 "src/racecheck.tr"
                while ((fq < c->fields->len)) {
#line 1093 "src/racecheck.tr"
                    ({ __auto_type _ord_t3223 = (((FieldDef*)List_ptr_get(c->fields, fq))->ty); __auto_type _ord_t3224 = (c->line); RcCk_ty_check(ck, _ord_t3223, dfc, _ord_t3224); });
#line 1094 "src/racecheck.tr"
                    fq = (fq + 1LL);
                }
            }
        } else if (_t3221.tag == Decl_DActor) {
            __auto_type c2 = _t3221.data.DActor.cls;
#line 1096 "src/racecheck.tr"
            List_ptr_append(ck->classes, _tr_obj_retain(c2));
#line 1097 "src/racecheck.tr"
            ({ __auto_type _ord_t3225 = (_rc_path_lib(dfc)); List_bool_append(ck->class_lib, _ord_t3225); });
        } else if (1) {
            __auto_type _ = _t3221;
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
            TrStr _strtmp_t3226 = List_TrStr_get(decl_files, i);
            _tr_str_release(df);
            df = _strtmp_t3226;
        }
#line 1104 "src/racecheck.tr"
        bool lib = _rc_path_lib(df);
#line 1105 "src/racecheck.tr"
        __auto_type _t3227 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3227.tag == Decl_DTopLevelStmt) {
            __auto_type sp = _t3227.data.DTopLevelStmt.stmt;
#line 1107 "src/racecheck.tr"
            __auto_type _t3228 = (*sp);
            if (_t3228.tag == Stmt_SLet) {
                __auto_type name = _t3228.data.SLet.name;
__auto_type is_mut = _t3228.data.SLet.is_mut;
__auto_type is_const = _t3228.data.SLet.is_const;
__auto_type ty = _t3228.data.SLet.ty;
__auto_type v = _t3228.data.SLet.val;
#line 1109 "src/racecheck.tr"
                if ((!lib)) {
#line 1109 "src/racecheck.tr"
                    RcCk_add_global(ck, name, is_mut, is_const, ty, v, df);
                }
            } else if (_t3228.tag == Stmt_SAssign) {
                __auto_type tg = _t3228.data.SAssign.target;
__auto_type v3 = _t3228.data.SAssign.val;
#line 1112 "src/racecheck.tr"
                if (((!lib) && (!_rc_nul(tg)))) {
#line 1113 "src/racecheck.tr"
                    __auto_type _t3229 = (*tg);
                    if (_t3229.tag == Expr_EIdent) {
                        __auto_type gn = _t3229.data.EIdent.name;
                        RcCk_add_global(ck, gn, false, false, (AstType**)(0LL), v3, df);
                    } else if (1) {
                        __auto_type _ = _t3229;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t3228;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t3227;
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
            TrStr _strtmp_t3230 = List_TrStr_get(decl_files, i);
            _tr_str_release(df2);
            df2 = _strtmp_t3230;
        }
#line 1123 "src/racecheck.tr"
        bool lib2 = _rc_path_lib(df2);
#line 1124 "src/racecheck.tr"
        __auto_type _t3231 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3231.tag == Decl_DFunction) {
            __auto_type f = _t3231.data.DFunction.func;
            RcCk_add_fn(ck, f, df2, lib2);
        } else if (_t3231.tag == Decl_DClass) {
            __auto_type c3 = _t3231.data.DClass.cls;
#line 1127 "src/racecheck.tr"
            long long mi = 0LL;
#line 1128 "src/racecheck.tr"
            while ((mi < c3->methods->len)) {
#line 1129 "src/racecheck.tr"
                ({ __auto_type _ord_t3232 = (((FunctionDef*)List_ptr_get(c3->methods, mi))); __auto_type _ord_t3233 = (c3->name); RcCk_add_method(ck, _ord_t3232, _ord_t3233, df2, lib2); });
#line 1130 "src/racecheck.tr"
                mi = (mi + 1LL);
            }
        } else if (_t3231.tag == Decl_DActor) {
            __auto_type c4 = _t3231.data.DActor.cls;
#line 1132 "src/racecheck.tr"
            long long mi2 = 0LL;
#line 1133 "src/racecheck.tr"
            while ((mi2 < c4->methods->len)) {
#line 1134 "src/racecheck.tr"
                ({ __auto_type _ord_t3234 = (((FunctionDef*)List_ptr_get(c4->methods, mi2))); __auto_type _ord_t3235 = (c4->name); RcCk_add_method(ck, _ord_t3234, _ord_t3235, df2, lib2); });
#line 1135 "src/racecheck.tr"
                mi2 = (mi2 + 1LL);
            }
        } else if (_t3231.tag == Decl_DExtend) {
            __auto_type tgt = _t3231.data.DExtend.target;
__auto_type methods = _t3231.data.DExtend.methods;
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
            __auto_type _ = _t3231;
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
                ({ TrStr _ord_t3236 = (List_TrStr_get(f->msgs, mj)); List_TrStr_append(ck->msgs, _ord_t3236); _tr_str_release(_ord_t3236); });
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
                    if ((({ __auto_type _ord_t3237 = (List_TrStr_get(qf->pairs_a, qk)); __auto_type _wr = (({ TrStr _cmpa_t3238 = (_ord_t3237); TrStr _cmpb_t3239 = (List_TrStr_get(pf->pairs_b, pk)); _Bool _cmpr_t3240 = _tr_str_eqv(_cmpa_t3238, _cmpb_t3239); _tr_str_release(_cmpa_t3238); _tr_str_release(_cmpb_t3239); _cmpr_t3240; })); _wr; }) && ({ __auto_type _ord_t3241 = (List_TrStr_get(qf->pairs_b, qk)); __auto_type _wr = (({ TrStr _cmpa_t3242 = (_ord_t3241); TrStr _cmpb_t3243 = (List_TrStr_get(pf->pairs_a, pk)); _Bool _cmpr_t3244 = _tr_str_eqv(_cmpa_t3242, _cmpb_t3243); _tr_str_release(_cmpa_t3242); _tr_str_release(_cmpb_t3243); _cmpr_t3244; })); _wr; }))) {
#line 1168 "src/racecheck.tr"
                        ({ __auto_type _ord_t3245 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("W|", 2LL)), (pf->file))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(":", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3246 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3245); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(pf->line)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": [T-9] lock-order inversion: '", 31LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3247 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3246); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks '", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3248 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3247); TrStr _tr_cr = (List_TrStr_get(pf->pairs_a, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' then '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _ord_t3249 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3248); TrStr _tr_cr = (List_TrStr_get(pf->pairs_b, pk)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', but '", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _ord_t3250 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t3249); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (qf->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' locks them in the opposite order: two threads running them can deadlock.\n      FIX: always acquire the mutexes in one global order.", 133LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(ck->msgs, _ord_t3250); _tr_str_release(_ord_t3250); });
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

