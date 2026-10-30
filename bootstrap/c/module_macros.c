#include "tauraro_types.h"

MacroVal* str_list(List_TrStr* items);
MacroVal* param_rec(Param* p);
MacroVal* params_list(List_ptr* ps);
MacroVal* fn_rec(FunctionDef* f);
MacroVal* method_list(List_ptr* ms);
void parse_into(TrStr src, List_ptr* gen);
List_ptr* decl_decorators(Decl* declptr);
TrStr render_arg(Expr* e);
Expr* parse_expr_src(TrStr src);
TrStr kind_of(Expr* e);
MacroVal* arg_rec(Expr* e);
MacroCtx* run_fn_macro(FunctionDef* mdef, List_ptr* args, long long gbase);

__attribute__((malloc,returns_nonnull,hot)) MacroCtx* MacroCtx_init() {
#line 229 "src/macros.tr"
    MacroCtx* c = ((MacroCtx*)_tr_obj_alloc(sizeof(MacroCtx)));
#line 230 "src/macros.tr"
    c->env = _tr_dict_new(16LL);
#line 231 "src/macros.tr"
    c->returned = false;
#line 232 "src/macros.tr"
    c->result = _tr_str_lit_len("", 0LL);
#line 233 "src/macros.tr"
    c->has_error = false;
#line 234 "src/macros.tr"
    c->error_msg = _tr_str_lit_len("", 0LL);
#line 235 "src/macros.tr"
    c->gensym_ctr = 0LL;
#line 236 "src/macros.tr"
    return c;
}

__attribute__((hot)) void MacroCtx_fail(MacroCtx* self, TrStr msg) {
#line 239 "src/macros.tr"
    if ((!self->has_error)) {
#line 240 "src/macros.tr"
        self->has_error = true;
#line 241 "src/macros.tr"
        self->error_msg = _tr_str_retain(msg);
    }
}

__attribute__((hot)) MacroVal* MacroCtx_eval_binop(MacroCtx* self, TrStr op, MacroVal* lv, MacroVal* rv) {
#line 244 "src/macros.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
#line 245 "src/macros.tr"
        __auto_type _t7416 = (*lv);
        if (_t7416.tag == MacroVal_MInt) {
            __auto_type a = _t7416.data.MInt.n;
#line 247 "src/macros.tr"
            __auto_type _t7417 = (*rv);
            if (_t7417.tag == MacroVal_MInt) {
                __auto_type b = _t7417.data.MInt.n;
                return box_mv(MacroVal_ctor_MInt((a + b)));
            } else if (1) {
                __auto_type _ = _t7417;
                return ({ __auto_type _ord_t7418 = (mv_to_str(lv)); TrStr _at_t7419 = (({ TrStr _tr_cl = (_ord_t7418); TrStr _tr_cr = (mv_to_str(rv)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t7419))); _tr_str_release(_at_t7419); _wr; });
            }
        } else if (1) {
            __auto_type _ = _t7416;
            return ({ __auto_type _ord_t7420 = (mv_to_str(lv)); TrStr _at_t7421 = (({ TrStr _tr_cl = (_ord_t7420); TrStr _tr_cr = (mv_to_str(rv)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t7421))); _tr_str_release(_at_t7421); _wr; });
        }
    }
#line 251 "src/macros.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
#line 251 "src/macros.tr"
        return box_mv(MacroVal_ctor_MBool(mv_eq(lv, rv)));
    }
#line 252 "src/macros.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
#line 252 "src/macros.tr"
        return box_mv(MacroVal_ctor_MBool((!mv_eq(lv, rv))));
    }
#line 253 "src/macros.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("and", 3LL)))) {
#line 253 "src/macros.tr"
        return box_mv(MacroVal_ctor_MBool((mv_truthy(lv) && mv_truthy(rv))));
    }
#line 254 "src/macros.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("or", 2LL)))) {
#line 254 "src/macros.tr"
        return box_mv(MacroVal_ctor_MBool((mv_truthy(lv) || mv_truthy(rv))));
    }
#line 255 "src/macros.tr"
    __auto_type _t7422 = (*lv);
    if (_t7422.tag == MacroVal_MInt) {
        __auto_type a = _t7422.data.MInt.n;
#line 257 "src/macros.tr"
        __auto_type _t7423 = (*rv);
        if (_t7423.tag == MacroVal_MInt) {
            __auto_type b = _t7423.data.MInt.n;
#line 259 "src/macros.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
#line 259 "src/macros.tr"
                return box_mv(MacroVal_ctor_MBool((a < b)));
            }
#line 260 "src/macros.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
#line 260 "src/macros.tr"
                return box_mv(MacroVal_ctor_MBool((a > b)));
            }
#line 261 "src/macros.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
#line 261 "src/macros.tr"
                return box_mv(MacroVal_ctor_MBool((a <= b)));
            }
#line 262 "src/macros.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
#line 262 "src/macros.tr"
                return box_mv(MacroVal_ctor_MBool((a >= b)));
            }
#line 263 "src/macros.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 263 "src/macros.tr"
                return box_mv(MacroVal_ctor_MInt((a - b)));
            }
#line 264 "src/macros.tr"
            if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 264 "src/macros.tr"
                return box_mv(MacroVal_ctor_MInt((a * b)));
            }
        } else if (1) {
            __auto_type _ = _t7423;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t7422;
        /* pass */
    }
#line 267 "src/macros.tr"
    ({ TrStr _at_t7424 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("macro: unsupported operator '", 29LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in macro body", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); MacroCtx_fail(self, _at_t7424); _tr_str_release(_at_t7424); });
#line 268 "src/macros.tr"
    return box_mv(MacroVal_make_MNil());
}

__attribute__((hot)) MacroVal* MacroCtx_eval_mexpr(MacroCtx* self, Expr* eptr) {
#line 271 "src/macros.tr"
    if ((((unsigned long long)(eptr)) == ((unsigned long long)(0LL)))) {
#line 271 "src/macros.tr"
        return box_mv(MacroVal_make_MNil());
    }
#line 272 "src/macros.tr"
    __auto_type _t7425 = (*eptr);
    if (_t7425.tag == Expr_ELitStr) {
        __auto_type s = _t7425.data.ELitStr.val;
        MacroVal* _trret1939 = box_mv(MacroVal_ctor_MStr(s));
        return _trret1939;
    } else if (_t7425.tag == Expr_ERawStr) {
        __auto_type s = _t7425.data.ERawStr.val;
        MacroVal* _trret1940 = box_mv(MacroVal_ctor_MStr(s));
        return _trret1940;
    } else if (_t7425.tag == Expr_ELitInt) {
        __auto_type n = _t7425.data.ELitInt.val;
        return box_mv(MacroVal_ctor_MInt(n));
    } else if (_t7425.tag == Expr_ELitBool) {
        __auto_type b = _t7425.data.ELitBool.val;
        return box_mv(MacroVal_ctor_MBool(b));
    } else if (_t7425.tag == Expr_EIdent) {
        __auto_type name = _t7425.data.EIdent.name;
#line 278 "src/macros.tr"
        if (_tr_dict_contains(self->env, _tr_strz(name))) {
#line 278 "src/macros.tr"
            MacroVal* _trret1941 = ((MacroVal*)(uintptr_t)_tr_dict_get(self->env, _tr_strz(name)));
            return _trret1941;
        }
#line 279 "src/macros.tr"
        ({ TrStr _at_t7426 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("macro: unknown name '", 21LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); MacroCtx_fail(self, _at_t7426); _tr_str_release(_at_t7426); });
#line 280 "src/macros.tr"
        MacroVal* _trret1942 = box_mv(MacroVal_make_MNil());
        return _trret1942;
    } else if (_t7425.tag == Expr_EBinOp) {
        __auto_type op = _t7425.data.EBinOp.op;
__auto_type l = _t7425.data.EBinOp.left;
__auto_type r = _t7425.data.EBinOp.right;
#line 282 "src/macros.tr"
        MacroVal* _trret1943 = ({ __auto_type _ord_t7427 = (MacroCtx_eval_mexpr(self, l)); __auto_type _ord_t7428 = (MacroCtx_eval_mexpr(self, r)); __auto_type _wr = (MacroCtx_eval_binop(self, op, _ord_t7427, _ord_t7428)); _wr; });
        return _trret1943;
    } else if (_t7425.tag == Expr_EUnaryOp) {
        __auto_type op = _t7425.data.EUnaryOp.op;
__auto_type x = _t7425.data.EUnaryOp.expr;
#line 284 "src/macros.tr"
        MacroVal* xv = MacroCtx_eval_mexpr(self, x);
#line 285 "src/macros.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("not", 3LL)))) {
#line 285 "src/macros.tr"
            MacroVal* _trret1944 = box_mv(MacroVal_ctor_MBool((!mv_truthy(xv))));
            return _trret1944;
        }
#line 286 "src/macros.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 287 "src/macros.tr"
            __auto_type _t7429 = (*xv);
            if (_t7429.tag == MacroVal_MInt) {
                __auto_type n = _t7429.data.MInt.n;
                MacroVal* _trret1945 = box_mv(MacroVal_ctor_MInt((0LL - n)));
                return _trret1945;
            } else if (1) {
                __auto_type _ = _t7429;
                /* pass */
            }
        }
#line 290 "src/macros.tr"
        ({ TrStr _at_t7430 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("macro: unsupported unary operator '", 35LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in macro body", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); MacroCtx_fail(self, _at_t7430); _tr_str_release(_at_t7430); });
#line 291 "src/macros.tr"
        MacroVal* _trret1946 = box_mv(MacroVal_make_MNil());
        return _trret1946;
    } else if (_t7425.tag == Expr_EFString) {
        __auto_type parts = _t7425.data.EFString.parts;
#line 293 "src/macros.tr"
        TrStr out = _tr_str_lit_len("", 0LL);
#line 294 "src/macros.tr"
        long long i = 0LL;
#line 295 "src/macros.tr"
        while ((i < parts->len)) {
#line 296 "src/macros.tr"
            FStringPart* part = ((FStringPart*)List_ptr_get(parts, i));
#line 297 "src/macros.tr"
            if (part->is_expr) {
#line 298 "src/macros.tr"
                ({ TrStr _ap_t7431 = (mv_to_str(MacroCtx_eval_mexpr(self, part->expr))); _tr_str_append(&out, _ap_t7431); _tr_str_release(_ap_t7431); });
            } else {
#line 300 "src/macros.tr"
                ({ TrStr _ap_t7432 = (part->text); _tr_str_append(&out, _ap_t7432); });
            }
#line 301 "src/macros.tr"
            i = (i + 1LL);
        }
#line 302 "src/macros.tr"
        MacroVal* _trret1947 = box_mv(MacroVal_ctor_MStr(out));
        _tr_str_release(out);
        return _trret1947;
    } else if (_t7425.tag == Expr_EIndex) {
        __auto_type obj = _t7425.data.EIndex.obj;
__auto_type idx = _t7425.data.EIndex._tr_v_index;
#line 304 "src/macros.tr"
        MacroVal* ov = MacroCtx_eval_mexpr(self, obj);
#line 305 "src/macros.tr"
        MacroVal* iv = MacroCtx_eval_mexpr(self, idx);
#line 306 "src/macros.tr"
        __auto_type _t7433 = (*ov);
        if (_t7433.tag == MacroVal_MList) {
            __auto_type items = _t7433.data.MList.items;
#line 308 "src/macros.tr"
            __auto_type _t7434 = (*iv);
            if (_t7434.tag == MacroVal_MInt) {
                __auto_type n = _t7434.data.MInt.n;
#line 310 "src/macros.tr"
                if (((n >= 0LL) && (n < items->len))) {
#line 310 "src/macros.tr"
                    return ((MacroVal*)List_ptr_get(items, n));
                }
#line 311 "src/macros.tr"
                MacroCtx_fail(self, _tr_str_lit_len("macro: list index out of range", 30LL));
            } else if (1) {
                __auto_type _ = _t7434;
                MacroCtx_fail(self, _tr_str_lit_len("macro: list index must be an int", 32LL));
            }
        } else if (1) {
            __auto_type _ = _t7433;
            MacroCtx_fail(self, _tr_str_lit_len("macro: indexing requires a list", 31LL));
        }
#line 314 "src/macros.tr"
        return box_mv(MacroVal_make_MNil());
    } else if (_t7425.tag == Expr_EPropAccess) {
        __auto_type obj = _t7425.data.EPropAccess.obj;
__auto_type prop = _t7425.data.EPropAccess.prop;
#line 316 "src/macros.tr"
        MacroVal* _trret1948 = mrec_get(MacroCtx_eval_mexpr(self, obj), prop);
        return _trret1948;
    } else if (_t7425.tag == Expr_EMethodCall) {
        __auto_type obj = _t7425.data.EMethodCall.obj;
__auto_type method = _t7425.data.EMethodCall.method;
__auto_type margs = _t7425.data.EMethodCall.args;
#line 318 "src/macros.tr"
        MacroVal* ov = MacroCtx_eval_mexpr(self, obj);
#line 319 "src/macros.tr"
        if (_tr_str_eqv((method), (_tr_str_lit_len("len", 3LL)))) {
#line 320 "src/macros.tr"
            __auto_type _t7435 = (*ov);
            if (_t7435.tag == MacroVal_MList) {
                __auto_type items = _t7435.data.MList.items;
                MacroVal* _trret1949 = box_mv(MacroVal_ctor_MInt(items->len));
                return _trret1949;
            } else if (1) {
                __auto_type _ = _t7435;
                /* pass */
            }
#line 323 "src/macros.tr"
            MacroVal* _trret1950 = box_mv(MacroVal_ctor_MInt(0LL));
            return _trret1950;
        }
#line 324 "src/macros.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("to_str", 6LL))) || _tr_str_eqv((method), (_tr_str_lit_len("to_string", 9LL))))) {
#line 325 "src/macros.tr"
            MacroVal* _trret1951 = ({ TrStr _at_t7436 = (mv_to_str(ov)); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t7436))); _tr_str_release(_at_t7436); _wr; });
            return _trret1951;
        }
#line 326 "src/macros.tr"
        ({ TrStr _at_t7437 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("macro: unsupported method '", 27LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in macro body", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); MacroCtx_fail(self, _at_t7437); _tr_str_release(_at_t7437); });
#line 327 "src/macros.tr"
        MacroVal* _trret1952 = box_mv(MacroVal_make_MNil());
        return _trret1952;
    } else if (_t7425.tag == Expr_ECall) {
        __auto_type callee = _t7425.data.ECall.callee;
__auto_type cargs = _t7425.data.ECall.args;
#line 329 "src/macros.tr"
        __auto_type _t7438 = (*callee);
        if (_t7438.tag == Expr_EIdent) {
            __auto_type cn = _t7438.data.EIdent.name;
#line 331 "src/macros.tr"
            if (_tr_str_eqv((cn), (_tr_str_lit_len("macro_error", 11LL)))) {
#line 332 "src/macros.tr"
                TrStr msg = _tr_str_lit_len("macro_error", 11LL);
#line 333 "src/macros.tr"
                if ((cargs->len > 0LL)) {
#line 333 "src/macros.tr"
                    TrStr _strtmp_t7439 = mv_to_str(MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(cargs, 0LL))));
                    _tr_str_release(msg);
                    msg = _strtmp_t7439;
                }
#line 334 "src/macros.tr"
                MacroCtx_fail(self, msg);
#line 335 "src/macros.tr"
                MacroVal* _trret1953 = box_mv(MacroVal_make_MNil());
                _tr_str_release(msg);
                return _trret1953;
            }
#line 339 "src/macros.tr"
            if (_tr_str_eqv((cn), (_tr_str_lit_len("gensym", 6LL)))) {
#line 340 "src/macros.tr"
                self->gensym_ctr = (self->gensym_ctr + 1LL);
#line 341 "src/macros.tr"
                TrStr pfx = _tr_str_lit_len("g", 1LL);
#line 342 "src/macros.tr"
                if ((cargs->len > 0LL)) {
#line 342 "src/macros.tr"
                    TrStr _strtmp_t7440 = mv_to_str(MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(cargs, 0LL))));
                    _tr_str_release(pfx);
                    pfx = _strtmp_t7440;
                }
#line 343 "src/macros.tr"
                MacroVal* _trret1954 = ({ TrStr _at_t7441 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("__hyg_", 6LL)), (pfx))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->gensym_ctr)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t7441))); _tr_str_release(_at_t7441); _wr; });
                _tr_str_release(pfx);
                return _trret1954;
            }
#line 345 "src/macros.tr"
            if (_tr_str_eqv((cn), (_tr_str_lit_len("str", 3LL)))) {
#line 346 "src/macros.tr"
                if ((cargs->len > 0LL)) {
#line 346 "src/macros.tr"
                    MacroVal* _trret1955 = ({ TrStr _at_t7442 = (mv_to_str(MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(cargs, 0LL))))); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t7442))); _tr_str_release(_at_t7442); _wr; });
                    return _trret1955;
                }
#line 347 "src/macros.tr"
                MacroVal* _trret1956 = box_mv(MacroVal_ctor_MStr(_tr_str_lit_len("", 0LL)));
                return _trret1956;
            }
        } else if (1) {
            __auto_type _ = _t7438;
            /* pass */
        }
#line 349 "src/macros.tr"
        MacroCtx_fail(self, _tr_str_lit_len("macro: unsupported call in macro body", 37LL));
#line 350 "src/macros.tr"
        return box_mv(MacroVal_make_MNil());
    } else if (_t7425.tag == Expr_EList) {
        __auto_type items = _t7425.data.EList.items;
#line 352 "src/macros.tr"
        List_ptr* l = (void*)List_ptr_new();
#line 353 "src/macros.tr"
        long long i2 = 0LL;
#line 354 "src/macros.tr"
        while ((i2 < items->len)) {
#line 355 "src/macros.tr"
            List_ptr_append(l, MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(items, i2))));
#line 356 "src/macros.tr"
            i2 = (i2 + 1LL);
        }
#line 357 "src/macros.tr"
        return box_mv(MacroVal_ctor_MList(l));
    } else if (1) {
        __auto_type _ = _t7425;
#line 359 "src/macros.tr"
        MacroCtx_fail(self, _tr_str_lit_len("macro: unsupported expression in macro body", 43LL));
#line 360 "src/macros.tr"
        return box_mv(MacroVal_make_MNil());
    }
}

__attribute__((hot)) void MacroCtx_eval_mblock(MacroCtx* self, Block* b) {
#line 363 "src/macros.tr"
    long long i = 0LL;
#line 364 "src/macros.tr"
    while ((i < b->stmts->len)) {
#line 365 "src/macros.tr"
        if ((self->returned || self->has_error)) {
#line 365 "src/macros.tr"
            return;
        }
#line 366 "src/macros.tr"
        MacroCtx_eval_mstmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
#line 367 "src/macros.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void MacroCtx_eval_mstmt(MacroCtx* self, Stmt* sptr) {
#line 370 "src/macros.tr"
    if ((((unsigned long long)(sptr)) == ((unsigned long long)(0LL)))) {
#line 370 "src/macros.tr"
        return;
    }
#line 371 "src/macros.tr"
    __auto_type _t7443 = (*sptr);
    if (_t7443.tag == Stmt_SLet) {
        __auto_type name = _t7443.data.SLet.name;
__auto_type val = _t7443.data.SLet.val;
#line 373 "src/macros.tr"
        ({ __auto_type _ord_t7444 = (MacroCtx_eval_mexpr(self, val)); _tr_dict_set(self->env, _tr_strz(name), _ord_t7444); });
    } else if (_t7443.tag == Stmt_SAssign) {
        __auto_type target = _t7443.data.SAssign.target;
__auto_type val = _t7443.data.SAssign.val;
#line 375 "src/macros.tr"
        __auto_type _t7445 = (*target);
        if (_t7445.tag == Expr_EIdent) {
            __auto_type nm = _t7445.data.EIdent.name;
            ({ __auto_type _ord_t7446 = (MacroCtx_eval_mexpr(self, val)); _tr_dict_set(self->env, _tr_strz(nm), _ord_t7446); });
        } else if (1) {
            __auto_type _ = _t7445;
            MacroCtx_fail(self, _tr_str_lit_len("macro: assignment target must be a simple variable", 50LL));
        }
    } else if (_t7443.tag == Stmt_SReturn) {
        __auto_type val = _t7443.data.SReturn.val;
#line 379 "src/macros.tr"
        self->result = mv_to_str(MacroCtx_eval_mexpr(self, val));
#line 380 "src/macros.tr"
        self->returned = true;
    } else if (_t7443.tag == Stmt_SExpr) {
        __auto_type e = _t7443.data.SExpr.expr;
#line 382 "src/macros.tr"
        MacroVal* _r = MacroCtx_eval_mexpr(self, e);
    } else if (_t7443.tag == Stmt_SFor) {
        __auto_type var = _t7443.data.SFor.var;
__auto_type iter = _t7443.data.SFor.iter;
__auto_type body = _t7443.data.SFor.body;
#line 384 "src/macros.tr"
        MacroVal* iv = MacroCtx_eval_mexpr(self, iter);
#line 385 "src/macros.tr"
        __auto_type _t7447 = (*iv);
        if (_t7447.tag == MacroVal_MList) {
            __auto_type items = _t7447.data.MList.items;
#line 387 "src/macros.tr"
            long long i = 0LL;
#line 388 "src/macros.tr"
            while ((i < items->len)) {
#line 389 "src/macros.tr"
                if ((self->returned || self->has_error)) {
#line 389 "src/macros.tr"
                    return;
                }
#line 390 "src/macros.tr"
                ({ __auto_type _ord_t7448 = (((MacroVal*)List_ptr_get(items, i))); _tr_dict_set(self->env, _tr_strz(var), _ord_t7448); });
#line 391 "src/macros.tr"
                MacroCtx_eval_mblock(self, body);
#line 392 "src/macros.tr"
                i = (i + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t7447;
            MacroCtx_fail(self, _tr_str_lit_len("macro: 'for' needs a list (e.g. item.fields)", 44LL));
        }
    } else if (_t7443.tag == Stmt_SIf) {
        __auto_type cond = _t7443.data.SIf.cond;
__auto_type then_b = _t7443.data.SIf.then_b;
__auto_type elifs = _t7443.data.SIf.elifs;
__auto_type else_b = _t7443.data.SIf.else_b;
#line 395 "src/macros.tr"
        if (mv_truthy(MacroCtx_eval_mexpr(self, cond))) {
#line 396 "src/macros.tr"
            MacroCtx_eval_mblock(self, then_b);
        } else {
#line 398 "src/macros.tr"
            bool done = false;
#line 399 "src/macros.tr"
            long long i = 0LL;
#line 400 "src/macros.tr"
            while (((i < elifs->len) && (!done))) {
#line 401 "src/macros.tr"
                ElifClause* ec = ((ElifClause*)List_ptr_get(elifs, i));
#line 402 "src/macros.tr"
                if (mv_truthy(MacroCtx_eval_mexpr(self, ec->cond))) {
#line 403 "src/macros.tr"
                    MacroCtx_eval_mblock(self, (*ec->body));
#line 404 "src/macros.tr"
                    done = true;
                }
#line 405 "src/macros.tr"
                i = (i + 1LL);
            }
#line 406 "src/macros.tr"
            if ((!done)) {
#line 407 "src/macros.tr"
                MacroCtx_eval_mblock(self, else_b);
            }
        }
    } else if (_t7443.tag == Stmt_SPass) {
        /* pass */
    } else if (_t7443.tag == Stmt_SLine) {
        __auto_type _ = _t7443.data.SLine.n;
        /* pass */
    } else if (1) {
        __auto_type _ = _t7443;
#line 411 "src/macros.tr"
        MacroCtx_fail(self, _tr_str_lit_len("macro: unsupported statement in macro body", 42LL));
    }
}

__attribute__((malloc,returns_nonnull,hot)) FnMacroExpander* FnMacroExpander_init(TrMap* m) {
#line 512 "src/macros.tr"
    FnMacroExpander* e = ((FnMacroExpander*)_tr_obj_alloc(sizeof(FnMacroExpander)));
#line 513 "src/macros.tr"
    e->macros = m;
#line 514 "src/macros.tr"
    e->errors = 0LL;
#line 515 "src/macros.tr"
    e->depth = 0LL;
#line 516 "src/macros.tr"
    e->gensym_base = 0LL;
#line 517 "src/macros.tr"
    return e;
}

__attribute__((hot)) void FnMacroExpander_visit_expr(FnMacroExpander* self, Expr* eptr) {
#line 520 "src/macros.tr"
    if ((((unsigned long long)(eptr)) == ((unsigned long long)(0LL)))) {
#line 520 "src/macros.tr"
        return;
    }
#line 521 "src/macros.tr"
    __auto_type _t7449 = (*eptr);
    if (_t7449.tag == Expr_EMacroCall) {
        __auto_type name = _t7449.data.EMacroCall.name;
__auto_type args = _t7449.data.EMacroCall.args;
#line 523 "src/macros.tr"
        long long i = 0LL;
#line 524 "src/macros.tr"
        while ((i < args->len)) {
#line 525 "src/macros.tr"
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(args, i)));
#line 526 "src/macros.tr"
            i = (i + 1LL);
        }
#line 527 "src/macros.tr"
        if ((!_tr_dict_contains(self->macros, _tr_strz(name)))) {
#line 528 "src/macros.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("error: [MACRO] unknown function-like macro '", 44LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("!'", 2LL))); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 529 "src/macros.tr"
            self->errors = (self->errors + 1LL);
#line 530 "src/macros.tr"
            return;
        }
#line 531 "src/macros.tr"
        MacroCtx* ctx = run_fn_macro(((FunctionDef*)(uintptr_t)_tr_dict_get(self->macros, _tr_strz(name))), args, self->gensym_base);
#line 532 "src/macros.tr"
        self->gensym_base = ctx->gensym_ctr;
#line 533 "src/macros.tr"
        if (ctx->has_error) {
#line 534 "src/macros.tr"
            ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("error: [MACRO] ", 15LL)), (name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("!: ", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ctx->error_msg)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 535 "src/macros.tr"
            self->errors = (self->errors + 1LL);
#line 536 "src/macros.tr"
            _tr_obj_release(ctx, _trdrop_MacroCtx);
            return;
        }
#line 537 "src/macros.tr"
        Expr* newe = parse_expr_src(ctx->result);
#line 538 "src/macros.tr"
        if ((((unsigned long long)(newe)) != ((unsigned long long)(0LL)))) {
#line 539 "src/macros.tr"
            if ((self->depth < 64LL)) {
#line 540 "src/macros.tr"
                self->depth = (self->depth + 1LL);
#line 541 "src/macros.tr"
                FnMacroExpander_visit_expr(self, newe);
#line 542 "src/macros.tr"
                self->depth = (self->depth - 1LL);
            }
#line 544 "src/macros.tr"
            /* unsafe block */
#line 545 "src/macros.tr"
            (*eptr = (*newe));
        }
        _tr_obj_release(ctx, _trdrop_MacroCtx);
    } else if (_t7449.tag == Expr_EBinOp) {
        __auto_type l = _t7449.data.EBinOp.left;
__auto_type r = _t7449.data.EBinOp.right;
#line 547 "src/macros.tr"
        FnMacroExpander_visit_expr(self, l);
#line 548 "src/macros.tr"
        FnMacroExpander_visit_expr(self, r);
    } else if (_t7449.tag == Expr_EUnaryOp) {
        __auto_type x = _t7449.data.EUnaryOp.expr;
        FnMacroExpander_visit_expr(self, x);
    } else if (_t7449.tag == Expr_ECall) {
        __auto_type callee = _t7449.data.ECall.callee;
__auto_type cargs = _t7449.data.ECall.args;
#line 551 "src/macros.tr"
        FnMacroExpander_visit_expr(self, callee);
#line 552 "src/macros.tr"
        long long i = 0LL;
#line 553 "src/macros.tr"
        while ((i < cargs->len)) {
#line 554 "src/macros.tr"
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(cargs, i)));
#line 555 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (_t7449.tag == Expr_EMethodCall) {
        __auto_type obj = _t7449.data.EMethodCall.obj;
__auto_type margs = _t7449.data.EMethodCall.args;
#line 557 "src/macros.tr"
        FnMacroExpander_visit_expr(self, obj);
#line 558 "src/macros.tr"
        long long i = 0LL;
#line 559 "src/macros.tr"
        while ((i < margs->len)) {
#line 560 "src/macros.tr"
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(margs, i)));
#line 561 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (_t7449.tag == Expr_EPropAccess) {
        __auto_type obj = _t7449.data.EPropAccess.obj;
        FnMacroExpander_visit_expr(self, obj);
    } else if (_t7449.tag == Expr_EIndex) {
        __auto_type obj = _t7449.data.EIndex.obj;
__auto_type idx = _t7449.data.EIndex._tr_v_index;
#line 564 "src/macros.tr"
        FnMacroExpander_visit_expr(self, obj);
#line 565 "src/macros.tr"
        FnMacroExpander_visit_expr(self, idx);
    } else if (_t7449.tag == Expr_ECast) {
        __auto_type x = _t7449.data.ECast.expr;
        FnMacroExpander_visit_expr(self, x);
    } else if (_t7449.tag == Expr_ETryExpr) {
        __auto_type x = _t7449.data.ETryExpr.expr;
        FnMacroExpander_visit_expr(self, x);
    } else if (_t7449.tag == Expr_EIfElse) {
        __auto_type c = _t7449.data.EIfElse.cond;
__auto_type t = _t7449.data.EIfElse.then_expr;
__auto_type e2 = _t7449.data.EIfElse.else_expr;
#line 569 "src/macros.tr"
        FnMacroExpander_visit_expr(self, c);
#line 570 "src/macros.tr"
        FnMacroExpander_visit_expr(self, t);
#line 571 "src/macros.tr"
        FnMacroExpander_visit_expr(self, e2);
    } else if (_t7449.tag == Expr_EList) {
        __auto_type items = _t7449.data.EList.items;
#line 573 "src/macros.tr"
        long long i = 0LL;
#line 574 "src/macros.tr"
        while ((i < items->len)) {
#line 575 "src/macros.tr"
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(items, i)));
#line 576 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (_t7449.tag == Expr_ETuple) {
        __auto_type items = _t7449.data.ETuple.items;
#line 578 "src/macros.tr"
        long long i = 0LL;
#line 579 "src/macros.tr"
        while ((i < items->len)) {
#line 580 "src/macros.tr"
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(items, i)));
#line 581 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (_t7449.tag == Expr_EFString) {
        __auto_type parts = _t7449.data.EFString.parts;
#line 583 "src/macros.tr"
        long long i = 0LL;
#line 584 "src/macros.tr"
        while ((i < parts->len)) {
#line 585 "src/macros.tr"
            FStringPart* p = ((FStringPart*)List_ptr_get(parts, i));
#line 586 "src/macros.tr"
            if (p->is_expr) {
#line 586 "src/macros.tr"
                FnMacroExpander_visit_expr(self, p->expr);
            }
#line 587 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (_t7449.tag == Expr_EDo) {
        __auto_type body = _t7449.data.EDo.body;
        FnMacroExpander_visit_block(self, body);
    } else if (_t7449.tag == Expr_ELoop) {
        __auto_type body = _t7449.data.ELoop.body;
        FnMacroExpander_visit_block(self, body);
    } else if (_t7449.tag == Expr_EWhileExpr) {
        __auto_type c = _t7449.data.EWhileExpr.cond;
__auto_type body = _t7449.data.EWhileExpr.body;
__auto_type eb = _t7449.data.EWhileExpr.else_body;
#line 591 "src/macros.tr"
        FnMacroExpander_visit_expr(self, c);
#line 592 "src/macros.tr"
        FnMacroExpander_visit_block(self, body);
#line 593 "src/macros.tr"
        FnMacroExpander_visit_block(self, eb);
    } else if (_t7449.tag == Expr_EMatch) {
        __auto_type subj = _t7449.data.EMatch.subj;
__auto_type arms = _t7449.data.EMatch.arms;
#line 595 "src/macros.tr"
        FnMacroExpander_visit_expr(self, subj);
#line 596 "src/macros.tr"
        long long i = 0LL;
#line 597 "src/macros.tr"
        while ((i < arms->len)) {
#line 598 "src/macros.tr"
            FnMacroExpander_visit_block(self, (*((MatchArm*)List_ptr_get(arms, i))->body));
#line 599 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t7449;
        /* pass */
    }
}

__attribute__((hot)) void FnMacroExpander_visit_block(FnMacroExpander* self, Block* b) {
#line 603 "src/macros.tr"
    long long i = 0LL;
#line 604 "src/macros.tr"
    while ((i < b->stmts->len)) {
#line 605 "src/macros.tr"
        FnMacroExpander_visit_stmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
#line 606 "src/macros.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void FnMacroExpander_visit_stmt(FnMacroExpander* self, Stmt* sptr) {
#line 609 "src/macros.tr"
    if ((((unsigned long long)(sptr)) == ((unsigned long long)(0LL)))) {
#line 609 "src/macros.tr"
        return;
    }
#line 610 "src/macros.tr"
    __auto_type _t7450 = (*sptr);
    if (_t7450.tag == Stmt_SExpr) {
        __auto_type e = _t7450.data.SExpr.expr;
        FnMacroExpander_visit_expr(self, e);
    } else if (_t7450.tag == Stmt_SLet) {
        __auto_type v = _t7450.data.SLet.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t7450.tag == Stmt_SMultiLet) {
        __auto_type v = _t7450.data.SMultiLet.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t7450.tag == Stmt_SAssign) {
        __auto_type t = _t7450.data.SAssign.target;
__auto_type v = _t7450.data.SAssign.val;
#line 615 "src/macros.tr"
        FnMacroExpander_visit_expr(self, t);
#line 616 "src/macros.tr"
        FnMacroExpander_visit_expr(self, v);
    } else if (_t7450.tag == Stmt_SReturn) {
        __auto_type v = _t7450.data.SReturn.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t7450.tag == Stmt_SBreak) {
        __auto_type v = _t7450.data.SBreak.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t7450.tag == Stmt_SRaise) {
        __auto_type v = _t7450.data.SRaise.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t7450.tag == Stmt_SSpawn) {
        __auto_type v = _t7450.data.SSpawn.expr;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t7450.tag == Stmt_SAssert) {
        __auto_type c = _t7450.data.SAssert.cond;
__auto_type m = _t7450.data.SAssert.msg;
#line 622 "src/macros.tr"
        FnMacroExpander_visit_expr(self, c);
#line 623 "src/macros.tr"
        FnMacroExpander_visit_expr(self, m);
    } else if (_t7450.tag == Stmt_SIf) {
        __auto_type c = _t7450.data.SIf.cond;
__auto_type tb = _t7450.data.SIf.then_b;
__auto_type elifs = _t7450.data.SIf.elifs;
__auto_type eb = _t7450.data.SIf.else_b;
#line 625 "src/macros.tr"
        FnMacroExpander_visit_expr(self, c);
#line 626 "src/macros.tr"
        FnMacroExpander_visit_block(self, tb);
#line 627 "src/macros.tr"
        long long i = 0LL;
#line 628 "src/macros.tr"
        while ((i < elifs->len)) {
#line 629 "src/macros.tr"
            ElifClause* ec = ((ElifClause*)List_ptr_get(elifs, i));
#line 630 "src/macros.tr"
            FnMacroExpander_visit_expr(self, ec->cond);
#line 631 "src/macros.tr"
            FnMacroExpander_visit_block(self, (*ec->body));
#line 632 "src/macros.tr"
            i = (i + 1LL);
        }
#line 633 "src/macros.tr"
        FnMacroExpander_visit_block(self, eb);
    } else if (_t7450.tag == Stmt_SWhile) {
        __auto_type c = _t7450.data.SWhile.cond;
__auto_type b = _t7450.data.SWhile.body;
#line 635 "src/macros.tr"
        FnMacroExpander_visit_expr(self, c);
#line 636 "src/macros.tr"
        FnMacroExpander_visit_block(self, b);
    } else if (_t7450.tag == Stmt_SFor) {
        __auto_type it = _t7450.data.SFor.iter;
__auto_type b = _t7450.data.SFor.body;
#line 638 "src/macros.tr"
        FnMacroExpander_visit_expr(self, it);
#line 639 "src/macros.tr"
        FnMacroExpander_visit_block(self, b);
    } else if (_t7450.tag == Stmt_SForUnpack) {
        __auto_type it = _t7450.data.SForUnpack.iter;
__auto_type b = _t7450.data.SForUnpack.body;
#line 641 "src/macros.tr"
        FnMacroExpander_visit_expr(self, it);
#line 642 "src/macros.tr"
        FnMacroExpander_visit_block(self, b);
    } else if (_t7450.tag == Stmt_SMatch) {
        __auto_type subj = _t7450.data.SMatch.expr;
__auto_type arms = _t7450.data.SMatch.arms;
#line 644 "src/macros.tr"
        FnMacroExpander_visit_expr(self, subj);
#line 645 "src/macros.tr"
        long long i = 0LL;
#line 646 "src/macros.tr"
        while ((i < arms->len)) {
#line 647 "src/macros.tr"
            FnMacroExpander_visit_block(self, (*((MatchArm*)List_ptr_get(arms, i))->body));
#line 648 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (_t7450.tag == Stmt_SWith) {
        __auto_type items = _t7450.data.SWith.items;
__auto_type b = _t7450.data.SWith.body;
#line 650 "src/macros.tr"
        long long i = 0LL;
#line 651 "src/macros.tr"
        while ((i < items->len)) {
#line 652 "src/macros.tr"
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(items, i)));
#line 653 "src/macros.tr"
            i = (i + 1LL);
        }
#line 654 "src/macros.tr"
        FnMacroExpander_visit_block(self, b);
    } else if (_t7450.tag == Stmt_STry) {
        __auto_type tb = _t7450.data.STry.try_body;
__auto_type catches = _t7450.data.STry.catches;
__auto_type fb = _t7450.data.STry.finally_b;
#line 656 "src/macros.tr"
        FnMacroExpander_visit_block(self, tb);
#line 657 "src/macros.tr"
        FnMacroExpander_visit_block(self, fb);
#line 658 "src/macros.tr"
        long long i = 0LL;
#line 659 "src/macros.tr"
        while ((i < catches->len)) {
#line 660 "src/macros.tr"
            FnMacroExpander_visit_block(self, (*(*((CatchClause**)List_ptr_get(catches, i)))->body));
#line 661 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (_t7450.tag == Stmt_SUnsafe) {
        __auto_type b = _t7450.data.SUnsafe.body;
        FnMacroExpander_visit_block(self, b);
    } else if (_t7450.tag == Stmt_STaskGroup) {
        __auto_type b = _t7450.data.STaskGroup.body;
        FnMacroExpander_visit_block(self, b);
    } else if (_t7450.tag == Stmt_SGpuBlock) {
        __auto_type b = _t7450.data.SGpuBlock.body;
        FnMacroExpander_visit_block(self, b);
    } else if (_t7450.tag == Stmt_SDefer) {
        __auto_type s = _t7450.data.SDefer.stmt;
        FnMacroExpander_visit_stmt(self, s);
    } else if (1) {
        __auto_type _ = _t7450;
        /* pass */
    }
}

__attribute__((hot)) void FnMacroExpander_expand_decl(FnMacroExpander* self, Decl* dptr) {
#line 669 "src/macros.tr"
    __auto_type _t7451 = (*dptr);
    if (_t7451.tag == Decl_DFunction) {
        __auto_type f = _t7451.data.DFunction.func;
        FnMacroExpander_visit_block(self, f->body);
    } else if (_t7451.tag == Decl_DClass) {
        __auto_type c = _t7451.data.DClass.cls;
#line 672 "src/macros.tr"
        long long i = 0LL;
#line 673 "src/macros.tr"
        while ((i < c->methods->len)) {
#line 674 "src/macros.tr"
            FnMacroExpander_visit_block(self, ((FunctionDef*)List_ptr_get(c->methods, i))->body);
#line 675 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t7451;
        /* pass */
    }
}

__attribute__((hot)) MacroVal* box_mv(MacroVal v) {
#line 33 "src/macros.tr"
    /* unsafe block */
#line 34 "src/macros.tr"
    MacroVal* p = ((MacroVal*)_tr_c_calloc((size_t)(1LL), sizeof(MacroVal)));
#line 35 "src/macros.tr"
    (*p = v);
#line 36 "src/macros.tr"
    return p;
}

__attribute__((hot)) MacroVal* mrec(List_TrStr* keys, List_ptr* vals) {
#line 39 "src/macros.tr"
    return box_mv(MacroVal_ctor_MRec(keys, vals));
}

__attribute__((hot)) MacroVal* mrec_get(MacroVal* recptr, TrStr key) {
#line 43 "src/macros.tr"
    if ((((unsigned long long)(recptr)) == ((unsigned long long)(0LL)))) {
#line 43 "src/macros.tr"
        return box_mv(MacroVal_make_MNil());
    }
#line 44 "src/macros.tr"
    __auto_type _t7452 = (*recptr);
    if (_t7452.tag == MacroVal_MRec) {
        __auto_type keys = _t7452.data.MRec.keys;
__auto_type vals = _t7452.data.MRec.vals;
#line 46 "src/macros.tr"
        long long i = 0LL;
#line 47 "src/macros.tr"
        while ((i < keys->len)) {
#line 48 "src/macros.tr"
            if (({ TrStr _cmpa_t7453 = (List_TrStr_get(keys, i)); TrStr _cmpb_t7454 = (key); _Bool _cmpr_t7455 = _tr_str_eqv(_cmpa_t7453, _cmpb_t7454); _tr_str_release(_cmpa_t7453); _cmpr_t7455; })) {
#line 48 "src/macros.tr"
                MacroVal* _trret1928 = ((MacroVal*)List_ptr_get(vals, i));
                return _trret1928;
            }
#line 49 "src/macros.tr"
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t7452;
        /* pass */
    }
#line 51 "src/macros.tr"
    return box_mv(MacroVal_make_MNil());
}

__attribute__((hot)) TrStr mv_to_str(MacroVal* vptr) {
#line 54 "src/macros.tr"
    if ((((unsigned long long)(vptr)) == ((unsigned long long)(0LL)))) {
#line 54 "src/macros.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 55 "src/macros.tr"
    __auto_type _t7456 = (*vptr);
    if (_t7456.tag == MacroVal_MStr) {
        __auto_type s = _t7456.data.MStr.s;
        TrStr _trret1929 = _tr_str_retain(s);
        return _trret1929;
    } else if (_t7456.tag == MacroVal_MInt) {
        __auto_type n = _t7456.data.MInt.n;
        return _tr_str_wrap(_tr_int_to_str((long long)(n)));
    } else if (_t7456.tag == MacroVal_MBool) {
        __auto_type b = _t7456.data.MBool.b;
#line 59 "src/macros.tr"
        if (b) {
#line 59 "src/macros.tr"
            return _tr_str_lit_len("true", 4LL);
        }
#line 60 "src/macros.tr"
        return _tr_str_lit_len("false", 5LL);
    } else if (_t7456.tag == MacroVal_MRec) {
        return mv_to_str(mrec_get(vptr, _tr_str_lit_len("src", 3LL)));
    } else if (1) {
        __auto_type _ = _t7456;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) bool mv_truthy(MacroVal* vptr) {
#line 68 "src/macros.tr"
    if ((((unsigned long long)(vptr)) == ((unsigned long long)(0LL)))) {
#line 68 "src/macros.tr"
        return false;
    }
#line 69 "src/macros.tr"
    __auto_type _t7457 = (*vptr);
    if (_t7457.tag == MacroVal_MBool) {
        __auto_type b = _t7457.data.MBool.b;
        return b;
    } else if (_t7457.tag == MacroVal_MInt) {
        __auto_type n = _t7457.data.MInt.n;
        return (n != 0LL);
    } else if (_t7457.tag == MacroVal_MStr) {
        __auto_type s = _t7457.data.MStr.s;
        bool _trret1930 = (!_tr_str_eqv((s), (_tr_str_lit_len("", 0LL))));
        return _trret1930;
    } else if (1) {
        __auto_type _ = _t7457;
        return false;
    }
}

__attribute__((hot)) bool mv_eq(MacroVal* a, MacroVal* b) {
#line 79 "src/macros.tr"
    return ({ __auto_type _ord_t7458 = (mv_to_str(a)); __auto_type _wr = (({ TrStr _cmpa_t7459 = (_ord_t7458); TrStr _cmpb_t7460 = (mv_to_str(b)); _Bool _cmpr_t7461 = _tr_str_eqv(_cmpa_t7459, _cmpb_t7460); _tr_str_release(_cmpa_t7459); _tr_str_release(_cmpb_t7460); _cmpr_t7461; })); _wr; });
}

__attribute__((hot)) TrStr render_type(AstType** typtr) {
#line 83 "src/macros.tr"
    if ((((unsigned long long)(typtr)) == ((unsigned long long)(0LL)))) {
#line 83 "src/macros.tr"
        return _tr_str_lit_len("void", 4LL);
    }
#line 84 "src/macros.tr"
    AstType* t = (*typtr);
#line 85 "src/macros.tr"
    TrStr base = _tr_str_retain(t->name);
#line 86 "src/macros.tr"
    if ((t->args->len > 0LL)) {
#line 87 "src/macros.tr"
        TrStr s = _tr_strx_concatv((base), (_tr_str_lit_len("[", 1LL)));
#line 88 "src/macros.tr"
        long long i = 0LL;
#line 89 "src/macros.tr"
        while ((i < t->args->len)) {
#line 90 "src/macros.tr"
            if ((i > 0LL)) {
#line 90 "src/macros.tr"
                TrStr _strtmp_t7463 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
                _tr_str_release(s);
                s = _strtmp_t7463;
            }
#line 91 "src/macros.tr"
            TrStr _strtmp_t7465 = ({ TrStr _tr_cr = (render_type(((AstType**)List_ptr_get(t->args, i)))); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
            _tr_str_release(s);
            s = _strtmp_t7465;
#line 92 "src/macros.tr"
            i = (i + 1LL);
        }
#line 93 "src/macros.tr"
        TrStr _strtmp_t7466 = _tr_strx_concatv((s), (_tr_str_lit_len("]", 1LL)));
        _tr_str_release(base);
        base = _strtmp_t7466;
        _tr_str_release(s);
    }
#line 94 "src/macros.tr"
    if (t->is_mut_borrow) {
#line 94 "src/macros.tr"
        TrStr _trret1931 = _tr_strx_concatv((_tr_str_lit_len("mut ref ", 8LL)), (base));
        _tr_str_release(base);
        return _trret1931;
    }
#line 95 "src/macros.tr"
    if (t->is_borrow) {
#line 95 "src/macros.tr"
        TrStr _trret1932 = _tr_strx_concatv((_tr_str_lit_len("ref ", 4LL)), (base));
        _tr_str_release(base);
        return _trret1932;
    }
#line 96 "src/macros.tr"
    TrStr _trret1933 = _tr_str_retain(base);
    _tr_str_release(base);
    return _trret1933;
}

__attribute__((hot)) MacroVal* str_list(List_TrStr* items) {
#line 100 "src/macros.tr"
    List_ptr* l = (void*)List_ptr_new();
#line 101 "src/macros.tr"
    long long i = 0LL;
#line 102 "src/macros.tr"
    while ((i < items->len)) {
#line 103 "src/macros.tr"
        ({ TrStr _at_t7467 = (List_TrStr_get(items, i)); List_ptr_append(l, box_mv(MacroVal_ctor_MStr(_at_t7467))); _tr_str_release(_at_t7467); });
#line 104 "src/macros.tr"
        i = (i + 1LL);
    }
#line 105 "src/macros.tr"
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* param_rec(Param* p) {
#line 108 "src/macros.tr"
    List_TrStr* ks = (void*)List_TrStr_new();
#line 109 "src/macros.tr"
    List_ptr* vs = (void*)List_ptr_new();
#line 110 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("name", 4LL));
#line 110 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(p->name)));
#line 111 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("type", 4LL));
#line 111 "src/macros.tr"
    ({ TrStr _at_t7468 = (render_type(p->ty)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t7468))); _tr_str_release(_at_t7468); });
#line 112 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("is_ref", 6LL));
#line 112 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_ref)));
#line 113 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("is_mut", 6LL));
#line 113 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_mut_ref)));
#line 114 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("is_variadic", 11LL));
#line 114 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_variadic)));
#line 115 "src/macros.tr"
    return mrec(ks, vs);
}

__attribute__((hot)) MacroVal* params_list(List_ptr* ps) {
#line 118 "src/macros.tr"
    List_ptr* l = (void*)List_ptr_new();
#line 119 "src/macros.tr"
    long long i = 0LL;
#line 120 "src/macros.tr"
    while ((i < ps->len)) {
#line 121 "src/macros.tr"
        List_ptr_append(l, param_rec(((Param*)List_ptr_get(ps, i))));
#line 122 "src/macros.tr"
        i = (i + 1LL);
    }
#line 123 "src/macros.tr"
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* fn_rec(FunctionDef* f) {
#line 126 "src/macros.tr"
    List_TrStr* ks = (void*)List_TrStr_new();
#line 127 "src/macros.tr"
    List_ptr* vs = (void*)List_ptr_new();
#line 128 "src/macros.tr"
    TrStr arglist = _tr_str_lit_len("", 0LL);
#line 129 "src/macros.tr"
    long long ai = 0LL;
#line 130 "src/macros.tr"
    while ((ai < f->params->len)) {
#line 131 "src/macros.tr"
        if ((ai > 0LL)) {
#line 132 "src/macros.tr"
            ({ TrStr _ap_t7469 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&arglist, _ap_t7469); });
        }
#line 133 "src/macros.tr"
        ({ TrStr _ap_t7470 = (((Param*)List_ptr_get(f->params, ai))->name); _tr_str_append(&arglist, _ap_t7470); });
#line 134 "src/macros.tr"
        ai = (ai + 1LL);
    }
#line 135 "src/macros.tr"
    TrStr thr = _tr_str_lit_len("", 0LL);
#line 136 "src/macros.tr"
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
#line 137 "src/macros.tr"
        TrStr _strtmp_t7471 = render_type(f->throws_ty);
        _tr_str_release(thr);
        thr = _strtmp_t7471;
    }
#line 138 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("kind", 4LL));
#line 138 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_tr_str_lit_len("fn", 2LL))));
#line 139 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("name", 4LL));
#line 139 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(f->name)));
#line 140 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("params", 6LL));
#line 140 "src/macros.tr"
    List_ptr_append(vs, params_list(f->params));
#line 141 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("ret", 3LL));
#line 141 "src/macros.tr"
    ({ TrStr _at_t7472 = (render_type(f->ret_ty)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t7472))); _tr_str_release(_at_t7472); });
#line 142 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("arglist", 7LL));
#line 142 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(arglist)));
#line 143 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("is_pub", 6LL));
#line 143 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(f->is_public)));
#line 144 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("is_async", 8LL));
#line 144 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(f->is_async)));
#line 145 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("throws", 6LL));
#line 145 "src/macros.tr"
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(thr)));
#line 146 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("generics", 8LL));
#line 146 "src/macros.tr"
    List_ptr_append(vs, str_list(f->generics));
#line 147 "src/macros.tr"
    MacroVal* _trret1934 = mrec(ks, vs);
    _tr_str_release(arglist);
    _tr_str_release(thr);
    return _trret1934;
}

__attribute__((hot)) MacroVal* method_list(List_ptr* ms) {
#line 150 "src/macros.tr"
    List_ptr* l = (void*)List_ptr_new();
#line 151 "src/macros.tr"
    long long i = 0LL;
#line 152 "src/macros.tr"
    while ((i < ms->len)) {
#line 153 "src/macros.tr"
        List_ptr_append(l, fn_rec(((FunctionDef*)List_ptr_get(ms, i))));
#line 154 "src/macros.tr"
        i = (i + 1LL);
    }
#line 155 "src/macros.tr"
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* build_item(Decl* declptr) {
#line 158 "src/macros.tr"
    if ((((unsigned long long)(declptr)) == ((unsigned long long)(0LL)))) {
#line 158 "src/macros.tr"
        return box_mv(MacroVal_make_MNil());
    }
#line 159 "src/macros.tr"
    __auto_type _t7473 = (*declptr);
    if (_t7473.tag == Decl_DClass) {
        __auto_type c = _t7473.data.DClass.cls;
#line 161 "src/macros.tr"
        List_TrStr* ks = (void*)List_TrStr_new();
#line 162 "src/macros.tr"
        List_ptr* vs = (void*)List_ptr_new();
#line 164 "src/macros.tr"
        List_ptr* fl = (void*)List_ptr_new();
#line 165 "src/macros.tr"
        long long i = 0LL;
#line 166 "src/macros.tr"
        while ((i < c->fields->len)) {
#line 167 "src/macros.tr"
            FieldDef* fd = ((FieldDef*)List_ptr_get(c->fields, i));
#line 168 "src/macros.tr"
            List_TrStr* fks = (void*)List_TrStr_new();
#line 169 "src/macros.tr"
            List_ptr* fvs = (void*)List_ptr_new();
#line 170 "src/macros.tr"
            List_TrStr_append(fks, _tr_str_lit_len("name", 4LL));
#line 170 "src/macros.tr"
            List_ptr_append(fvs, box_mv(MacroVal_ctor_MStr(fd->name)));
#line 171 "src/macros.tr"
            List_TrStr_append(fks, _tr_str_lit_len("type", 4LL));
#line 171 "src/macros.tr"
            ({ TrStr _at_t7474 = (render_type(fd->ty)); List_ptr_append(fvs, box_mv(MacroVal_ctor_MStr(_at_t7474))); _tr_str_release(_at_t7474); });
#line 172 "src/macros.tr"
            List_ptr_append(fl, mrec(fks, fvs));
#line 173 "src/macros.tr"
            i = (i + 1LL);
        }
#line 174 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("kind", 4LL));
#line 174 "src/macros.tr"
        List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_tr_str_lit_len("class", 5LL))));
#line 175 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("name", 4LL));
#line 175 "src/macros.tr"
        List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(c->name)));
#line 176 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("fields", 6LL));
#line 176 "src/macros.tr"
        List_ptr_append(vs, box_mv(MacroVal_ctor_MList(fl)));
#line 177 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("methods", 7LL));
#line 177 "src/macros.tr"
        List_ptr_append(vs, method_list(c->methods));
#line 178 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("bases", 5LL));
#line 178 "src/macros.tr"
        List_ptr_append(vs, str_list(c->base_classes));
#line 179 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("interfaces", 10LL));
#line 179 "src/macros.tr"
        List_ptr_append(vs, str_list(c->iface_names));
#line 180 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("generics", 8LL));
#line 180 "src/macros.tr"
        List_ptr_append(vs, str_list(c->generics));
#line 181 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("is_value_type", 13LL));
#line 181 "src/macros.tr"
        List_ptr_append(vs, box_mv(MacroVal_ctor_MBool((!c->is_class))));
#line 182 "src/macros.tr"
        List_TrStr_append(ks, _tr_str_lit_len("is_pub", 6LL));
#line 182 "src/macros.tr"
        List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(c->is_public)));
#line 183 "src/macros.tr"
        MacroVal* _trret1935 = mrec(ks, vs);
        return _trret1935;
    } else if (_t7473.tag == Decl_DFunction) {
        __auto_type f = _t7473.data.DFunction.func;
#line 185 "src/macros.tr"
        MacroVal* _trret1936 = fn_rec(f);
        return _trret1936;
    } else if (_t7473.tag == Decl_DEnum) {
        __auto_type e = _t7473.data.DEnum.enm;
#line 187 "src/macros.tr"
        List_TrStr* ks2 = (void*)List_TrStr_new();
#line 188 "src/macros.tr"
        List_ptr* vs2 = (void*)List_ptr_new();
#line 189 "src/macros.tr"
        List_ptr* vl = (void*)List_ptr_new();
#line 190 "src/macros.tr"
        long long i2 = 0LL;
#line 191 "src/macros.tr"
        while ((i2 < e->variants->len)) {
#line 192 "src/macros.tr"
            VariantDef* vd = ((VariantDef*)List_ptr_get(e->variants, i2));
#line 193 "src/macros.tr"
            List_TrStr* vks = (void*)List_TrStr_new();
#line 194 "src/macros.tr"
            List_ptr* vvs = (void*)List_ptr_new();
#line 195 "src/macros.tr"
            List_TrStr* tys = (void*)List_TrStr_new();
#line 196 "src/macros.tr"
            long long j = 0LL;
#line 197 "src/macros.tr"
            while ((j < vd->fields->len)) {
#line 198 "src/macros.tr"
                ({ TrStr _at_t7475 = (render_type(((Param*)List_ptr_get(vd->fields, j))->ty)); List_TrStr_append(tys, _at_t7475); _tr_str_release(_at_t7475); });
#line 199 "src/macros.tr"
                j = (j + 1LL);
            }
#line 200 "src/macros.tr"
            List_TrStr_append(vks, _tr_str_lit_len("name", 4LL));
#line 200 "src/macros.tr"
            List_ptr_append(vvs, box_mv(MacroVal_ctor_MStr(vd->name)));
#line 201 "src/macros.tr"
            List_TrStr_append(vks, _tr_str_lit_len("fields", 6LL));
#line 201 "src/macros.tr"
            List_ptr_append(vvs, str_list(tys));
#line 202 "src/macros.tr"
            List_ptr_append(vl, mrec(vks, vvs));
#line 203 "src/macros.tr"
            i2 = (i2 + 1LL);
        }
#line 204 "src/macros.tr"
        List_TrStr_append(ks2, _tr_str_lit_len("kind", 4LL));
#line 204 "src/macros.tr"
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MStr(_tr_str_lit_len("enum", 4LL))));
#line 205 "src/macros.tr"
        List_TrStr_append(ks2, _tr_str_lit_len("name", 4LL));
#line 205 "src/macros.tr"
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MStr(e->name)));
#line 206 "src/macros.tr"
        List_TrStr_append(ks2, _tr_str_lit_len("variants", 8LL));
#line 206 "src/macros.tr"
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MList(vl)));
#line 207 "src/macros.tr"
        List_TrStr_append(ks2, _tr_str_lit_len("generics", 8LL));
#line 207 "src/macros.tr"
        List_ptr_append(vs2, str_list(e->generics));
#line 208 "src/macros.tr"
        MacroVal* _trret1937 = mrec(ks2, vs2);
        return _trret1937;
    } else if (_t7473.tag == Decl_DInterface) {
        __auto_type ifc = _t7473.data.DInterface.iface;
#line 210 "src/macros.tr"
        List_TrStr* ks3 = (void*)List_TrStr_new();
#line 211 "src/macros.tr"
        List_ptr* vs3 = (void*)List_ptr_new();
#line 212 "src/macros.tr"
        List_TrStr_append(ks3, _tr_str_lit_len("kind", 4LL));
#line 212 "src/macros.tr"
        List_ptr_append(vs3, box_mv(MacroVal_ctor_MStr(_tr_str_lit_len("interface", 9LL))));
#line 213 "src/macros.tr"
        List_TrStr_append(ks3, _tr_str_lit_len("name", 4LL));
#line 213 "src/macros.tr"
        List_ptr_append(vs3, box_mv(MacroVal_ctor_MStr(ifc->name)));
#line 214 "src/macros.tr"
        List_TrStr_append(ks3, _tr_str_lit_len("methods", 7LL));
#line 214 "src/macros.tr"
        List_ptr_append(vs3, method_list(ifc->methods));
#line 215 "src/macros.tr"
        MacroVal* _trret1938 = mrec(ks3, vs3);
        return _trret1938;
    } else if (1) {
        __auto_type _ = _t7473;
        return box_mv(MacroVal_make_MNil());
    }
}

__attribute__((hot)) void parse_into(TrStr src, List_ptr* gen) {
#line 415 "src/macros.tr"
    Lexer* lx = Lexer_init(src);
#line 416 "src/macros.tr"
    List_Token* toks = Lexer_tokenize(lx);
#line 417 "src/macros.tr"
    Parser* ps = Parser_init(toks, lx->token_lines);
#line 418 "src/macros.tr"
    Program* gp = Parser_parse_program(ps);
#line 419 "src/macros.tr"
    long long i = 0LL;
#line 420 "src/macros.tr"
    while ((i < gp->decls->len)) {
#line 421 "src/macros.tr"
        List_ptr_append(gen, ((Decl*)List_ptr_get(gp->decls, i)));
#line 422 "src/macros.tr"
        i = (i + 1LL);
    }
    _tr_obj_release(lx, _trdrop_Lexer);
    _tr_obj_release(ps, _trdrop_Parser);
    _tr_obj_release(gp, _trdrop_Program);
}

__attribute__((hot)) List_ptr* decl_decorators(Decl* declptr) {
#line 425 "src/macros.tr"
    __auto_type _t7476 = (*declptr);
    if (_t7476.tag == Decl_DClass) {
        __auto_type c = _t7476.data.DClass.cls;
        return c->decorators;
    } else if (_t7476.tag == Decl_DFunction) {
        __auto_type f = _t7476.data.DFunction.func;
        return f->decorators;
    } else if (_t7476.tag == Decl_DEnum) {
        __auto_type e = _t7476.data.DEnum.enm;
        return e->decorators;
    } else if (_t7476.tag == Decl_DInterface) {
        __auto_type ifc = _t7476.data.DInterface.iface;
        return ifc->decorators;
    } else if (1) {
        __auto_type _ = _t7476;
        return (void*)List_ptr_new();
    }
}

__attribute__((hot)) TrStr render_arg(Expr* e) {
#line 443 "src/macros.tr"
    Formatter* fm = ({ __auto_type _ord_t7477 = ((void*)List_i64_new()); __auto_type _ord_t7478 = ((void*)List_TrStr_new()); __auto_type _ord_t7479 = ((void*)List_bool_new()); __auto_type _wr = (Formatter_init(_ord_t7477, _ord_t7478, _ord_t7479)); _wr; });
#line 444 "src/macros.tr"
    TrStr _trret1975 = Formatter_expr_str(fm, e);
    _tr_obj_release(fm, _trdrop_Formatter);
    return _trret1975;
}

__attribute__((hot)) Expr* parse_expr_src(TrStr src) {
#line 448 "src/macros.tr"
    Lexer* lx = Lexer_init(src);
#line 449 "src/macros.tr"
    List_Token* toks = Lexer_tokenize(lx);
#line 450 "src/macros.tr"
    Parser* ps = Parser_init(toks, lx->token_lines);
#line 451 "src/macros.tr"
    Expr* _trret1976 = Parser_parse_expr(ps);
    _tr_obj_release(lx, _trdrop_Lexer);
    _tr_obj_release(ps, _trdrop_Parser);
    return _trret1976;
}

__attribute__((hot)) TrStr kind_of(Expr* e) {
#line 455 "src/macros.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 455 "src/macros.tr"
        return _tr_str_lit_len("nil", 3LL);
    }
#line 456 "src/macros.tr"
    __auto_type _t7480 = (*e);
    if (_t7480.tag == Expr_ELitInt) {
        __auto_type _ = _t7480.data.ELitInt.val;
        return _tr_str_lit_len("int", 3LL);
    } else if (_t7480.tag == Expr_ELitFloat) {
        __auto_type _ = _t7480.data.ELitFloat.val;
        return _tr_str_lit_len("float", 5LL);
    } else if (_t7480.tag == Expr_ELitStr) {
        return _tr_str_lit_len("str", 3LL);
    } else if (_t7480.tag == Expr_ERawStr) {
        return _tr_str_lit_len("str", 3LL);
    } else if (_t7480.tag == Expr_ELitBool) {
        __auto_type _ = _t7480.data.ELitBool.val;
        return _tr_str_lit_len("bool", 4LL);
    } else if (_t7480.tag == Expr_ELitChar) {
        __auto_type _ = _t7480.data.ELitChar.val;
        return _tr_str_lit_len("char", 4LL);
    } else if (_t7480.tag == Expr_EIdent) {
        __auto_type _ = _t7480.data.EIdent.name;
        return _tr_str_lit_len("ident", 5LL);
    } else if (_t7480.tag == Expr_ECall) {
        return _tr_str_lit_len("call", 4LL);
    } else if (_t7480.tag == Expr_EMethodCall) {
        return _tr_str_lit_len("call", 4LL);
    } else if (_t7480.tag == Expr_EBinOp) {
        return _tr_str_lit_len("binop", 5LL);
    } else if (_t7480.tag == Expr_EUnaryOp) {
        return _tr_str_lit_len("unop", 4LL);
    } else if (_t7480.tag == Expr_EList) {
        __auto_type _ = _t7480.data.EList.items;
        return _tr_str_lit_len("list", 4LL);
    } else if (_t7480.tag == Expr_ETuple) {
        __auto_type _ = _t7480.data.ETuple.items;
        return _tr_str_lit_len("tuple", 5LL);
    } else if (_t7480.tag == Expr_EIndex) {
        return _tr_str_lit_len("index", 5LL);
    } else if (_t7480.tag == Expr_EPropAccess) {
        return _tr_str_lit_len("field", 5LL);
    } else if (_t7480.tag == Expr_EFString) {
        __auto_type _ = _t7480.data.EFString.parts;
        return _tr_str_lit_len("fstring", 7LL);
    } else if (1) {
        __auto_type _ = _t7480;
        return _tr_str_lit_len("expr", 4LL);
    }
}

__attribute__((hot)) MacroVal* arg_rec(Expr* e) {
#line 479 "src/macros.tr"
    List_TrStr* ks = (void*)List_TrStr_new();
#line 480 "src/macros.tr"
    List_ptr* vs = (void*)List_ptr_new();
#line 481 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("src", 3LL));
#line 481 "src/macros.tr"
    ({ TrStr _at_t7481 = (render_arg(e)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t7481))); _tr_str_release(_at_t7481); });
#line 482 "src/macros.tr"
    List_TrStr_append(ks, _tr_str_lit_len("kind", 4LL));
#line 482 "src/macros.tr"
    ({ TrStr _at_t7482 = (kind_of(e)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t7482))); _tr_str_release(_at_t7482); });
#line 483 "src/macros.tr"
    return mrec(ks, vs);
}

__attribute__((hot)) MacroCtx* run_fn_macro(FunctionDef* mdef, List_ptr* args, long long gbase) {
#line 488 "src/macros.tr"
    MacroCtx* ctx = MacroCtx_init();
#line 489 "src/macros.tr"
    ctx->gensym_ctr = gbase;
#line 490 "src/macros.tr"
    List_ptr* argvals = (void*)List_ptr_new();
#line 491 "src/macros.tr"
    long long i = 0LL;
#line 492 "src/macros.tr"
    while ((i < args->len)) {
#line 493 "src/macros.tr"
        List_ptr_append(argvals, arg_rec(((Expr*)List_ptr_get(args, i))));
#line 494 "src/macros.tr"
        i = (i + 1LL);
    }
#line 495 "src/macros.tr"
    if ((mdef->params->len > 0LL)) {
#line 496 "src/macros.tr"
        ({ __auto_type _ord_t7483 = (((Param*)List_ptr_get(mdef->params, 0LL))->name); __auto_type _ord_t7484 = (box_mv(MacroVal_ctor_MList(argvals))); _tr_dict_set(ctx->env, _tr_strz(_ord_t7483), _ord_t7484); });
    }
#line 497 "src/macros.tr"
    MacroCtx_eval_mblock(ctx, mdef->body);
#line 498 "src/macros.tr"
    return ctx;
}

__attribute__((hot)) long long expand_macros(Program* prog) {
#line 680 "src/macros.tr"
    return ({ __auto_type _ord_t7485 = ((void*)List_ptr_new()); __auto_type _wr = (expand_macros_tracked(prog, _ord_t7485, (void*)List_TrStr_new())); _wr; });
}

__attribute__((hot)) long long expand_macros_tracked(Program* prog, List_ptr* all_decls, List_TrStr* all_decl_modules) {
#line 704 "src/macros.tr"
    TrMap* macros = _tr_dict_new(16LL);
#line 705 "src/macros.tr"
    long long nmac = 0LL;
#line 706 "src/macros.tr"
    long long i = 0LL;
#line 707 "src/macros.tr"
    while ((i < prog->decls->len)) {
#line 708 "src/macros.tr"
        __auto_type _t7486 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t7486.tag == Decl_DDecoratorDef) {
            __auto_type f = _t7486.data.DDecoratorDef.func;
#line 710 "src/macros.tr"
            if (f->is_macro) {
#line 711 "src/macros.tr"
                _tr_dict_set(macros, _tr_strz(f->name), _tr_obj_retain(f));
#line 712 "src/macros.tr"
                nmac = (nmac + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t7486;
            /* pass */
        }
#line 714 "src/macros.tr"
        i = (i + 1LL);
    }
#line 715 "src/macros.tr"
    if ((nmac == 0LL)) {
#line 715 "src/macros.tr"
        Dict_free_objval(macros, _trdrop_FunctionDef);
        return 0LL;
    }
#line 717 "src/macros.tr"
    long long errors = 0LL;
#line 718 "src/macros.tr"
    long long gbase = 0LL;
#line 719 "src/macros.tr"
    List_ptr* out = (void*)List_ptr_new();
#line 720 "src/macros.tr"
    List_ptr* gen = (void*)List_ptr_new();
#line 721 "src/macros.tr"
    List_TrStr* gen_modules = (void*)List_TrStr_new();
#line 722 "src/macros.tr"
    bool have_module_info = (all_decl_modules->len == prog->decls->len);
#line 723 "src/macros.tr"
    i = 0LL;
#line 724 "src/macros.tr"
    while ((i < prog->decls->len)) {
#line 725 "src/macros.tr"
        Decl* dptr = ((Decl*)List_ptr_get(prog->decls, i));
#line 726 "src/macros.tr"
        bool is_macro_def = false;
#line 727 "src/macros.tr"
        __auto_type _t7487 = (*dptr);
        if (_t7487.tag == Decl_DDecoratorDef) {
            __auto_type f = _t7487.data.DDecoratorDef.func;
#line 729 "src/macros.tr"
            if (f->is_macro) {
#line 729 "src/macros.tr"
                is_macro_def = true;
            }
        } else if (1) {
            __auto_type _ = _t7487;
            /* pass */
        }
#line 731 "src/macros.tr"
        if ((!is_macro_def)) {
#line 732 "src/macros.tr"
            List_ptr* decs = decl_decorators(dptr);
#line 733 "src/macros.tr"
            long long di = 0LL;
#line 734 "src/macros.tr"
            while ((di < decs->len)) {
#line 735 "src/macros.tr"
                TrStr dname = _tr_str_retain(((Decorator*)List_ptr_get(decs, di))->name);
#line 736 "src/macros.tr"
                if (_tr_dict_contains(macros, _tr_strz(dname))) {
#line 737 "src/macros.tr"
                    MacroCtx* ctx = MacroCtx_init();
#line 738 "src/macros.tr"
                    ctx->gensym_ctr = gbase;
#line 739 "src/macros.tr"
                    FunctionDef* mdef = ((FunctionDef*)(uintptr_t)_tr_dict_get(macros, _tr_strz(dname)));
#line 740 "src/macros.tr"
                    if ((mdef->params->len > 0LL)) {
#line 741 "src/macros.tr"
                        ({ __auto_type _ord_t7488 = (((Param*)List_ptr_get(mdef->params, 0LL))->name); __auto_type _ord_t7489 = (build_item(dptr)); _tr_dict_set(ctx->env, _tr_strz(_ord_t7488), _ord_t7489); });
                    }
#line 742 "src/macros.tr"
                    MacroCtx_eval_mblock(ctx, mdef->body);
#line 743 "src/macros.tr"
                    gbase = ctx->gensym_ctr;
#line 744 "src/macros.tr"
                    if (ctx->has_error) {
#line 745 "src/macros.tr"
                        ({ printf("%s", _tr_strz(({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("error: [MACRO] @", 16LL)), (dname))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(": ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ctx->error_msg)); _tr_str_release(_tr_cl); _tr_cres; }))); printf("\n"); });
#line 746 "src/macros.tr"
                        errors = (errors + 1LL);
                    } else {
#line 748 "src/macros.tr"
                        long long gen_before = gen->len;
#line 749 "src/macros.tr"
                        parse_into(ctx->result, gen);
#line 750 "src/macros.tr"
                        if (have_module_info) {
#line 751 "src/macros.tr"
                            TrStr trigger_mod = List_TrStr_get(all_decl_modules, i);
#line 752 "src/macros.tr"
                            long long gj = gen_before;
#line 753 "src/macros.tr"
                            while ((gj < gen->len)) {
#line 754 "src/macros.tr"
                                List_TrStr_append(gen_modules, trigger_mod);
#line 755 "src/macros.tr"
                                gj = (gj + 1LL);
                            }
                        }
                    }
                    _tr_obj_release(ctx, _trdrop_MacroCtx);
                }
#line 756 "src/macros.tr"
                di = (di + 1LL);
                _tr_str_release(dname);
            }
#line 757 "src/macros.tr"
            List_ptr_append(out, dptr);
        }
#line 758 "src/macros.tr"
        i = (i + 1LL);
    }
#line 759 "src/macros.tr"
    long long gi = 0LL;
#line 760 "src/macros.tr"
    while ((gi < gen->len)) {
#line 761 "src/macros.tr"
        List_ptr_append(out, ((Decl*)List_ptr_get(gen, gi)));
#line 762 "src/macros.tr"
        if (have_module_info) {
#line 763 "src/macros.tr"
            List_ptr_append(all_decls, ((Decl*)List_ptr_get(gen, gi)));
#line 764 "src/macros.tr"
            ({ TrStr _at_t7490 = (List_TrStr_get(gen_modules, gi)); List_TrStr_append(all_decl_modules, _at_t7490); _tr_str_release(_at_t7490); });
        }
#line 765 "src/macros.tr"
        gi = (gi + 1LL);
    }
#line 768 "src/macros.tr"
    FnMacroExpander* fexp = FnMacroExpander_init(macros);
#line 769 "src/macros.tr"
    fexp->gensym_base = gbase;
#line 770 "src/macros.tr"
    long long fi = 0LL;
#line 771 "src/macros.tr"
    while ((fi < out->len)) {
#line 772 "src/macros.tr"
        FnMacroExpander_expand_decl(fexp, ((Decl*)List_ptr_get(out, fi)));
#line 773 "src/macros.tr"
        fi = (fi + 1LL);
    }
#line 774 "src/macros.tr"
    errors = (errors + fexp->errors);
#line 775 "src/macros.tr"
    prog->decls = out;
#line 776 "src/macros.tr"
    long long _trret1977 = errors;
    Dict_free_objval(macros, _trdrop_FunctionDef);
    List_TrStr_free(gen_modules);
    _tr_obj_release(fexp, _trdrop_FnMacroExpander);
    return _trret1977;
}

