#include "tauraro_types.h"

MacroVal* str_list(List_TrStr* items);
MacroVal* param_rec(Param* p);
MacroVal* params_list(List_ptr* ps);
MacroVal* fn_rec(FunctionDef* f);
MacroVal* method_list(List_ptr* ms);
void parse_into(TrStr src, List_ptr* gen);
List_ptr* decl_decorators(Decl* declptr);

__attribute__((malloc,returns_nonnull,hot)) MacroCtx* MacroCtx_init() {
    /* pass */
    MacroCtx* c = ((MacroCtx*)_tr_checked_alloc(sizeof(MacroCtx)));
    /* pass */
    c->env = _tr_dict_new(16LL);
    /* pass */
    c->returned = false;
    /* pass */
    c->result = _tr_str_lit("");
    /* pass */
    c->has_error = false;
    /* pass */
    c->error_msg = _tr_str_lit("");
    /* pass */
    return c;
}

__attribute__((hot)) void MacroCtx_fail(MacroCtx* self, TrStr msg) {
    /* pass */
    if ((!self->has_error)) {
        /* pass */
        self->has_error = true;
        /* pass */
        self->error_msg = _tr_str_retain(msg);
    }
}

__attribute__((hot)) MacroVal* MacroCtx_eval_binop(MacroCtx* self, TrStr op, MacroVal* lv, MacroVal* rv) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        __auto_type _t2210 = (*lv);
        if (_t2210.tag == MacroVal_MInt) {
            __auto_type a = _t2210.data.MInt.n;
            /* pass */
            __auto_type _t2211 = (*rv);
            if (_t2211.tag == MacroVal_MInt) {
                __auto_type b = _t2211.data.MInt.n;
                return box_mv(MacroVal_ctor_MInt((a + b)));
            } else if (1) {
                __auto_type _ = _t2211;
                return ({ TrStr _at_t2212 = (({ TrStr _cl = (mv_to_str(lv)); TrStr _cr = (mv_to_str(rv)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t2212))); _tr_str_release(_at_t2212); _wr; });
            }
        } else if (1) {
            __auto_type _ = _t2210;
            return ({ TrStr _at_t2213 = (({ TrStr _cl = (mv_to_str(lv)); TrStr _cr = (mv_to_str(rv)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t2213))); _tr_str_release(_at_t2213); _wr; });
        }
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool(mv_eq(lv, rv)));
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool((!mv_eq(lv, rv))));
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("and"))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool((mv_truthy(lv) && mv_truthy(rv))));
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("or"))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool((mv_truthy(lv) || mv_truthy(rv))));
    }
    /* pass */
    __auto_type _t2214 = (*lv);
    if (_t2214.tag == MacroVal_MInt) {
        __auto_type a = _t2214.data.MInt.n;
        /* pass */
        __auto_type _t2215 = (*rv);
        if (_t2215.tag == MacroVal_MInt) {
            __auto_type b = _t2215.data.MInt.n;
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a < b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a > b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a <= b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a >= b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MInt((a - b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MInt((a * b)));
            }
        } else if (1) {
            __auto_type _ = _t2215;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t2214;
        /* pass */
    }
    /* pass */
    ({ TrStr _at_t2216 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("macro: unsupported operator '")), _tr_strz(op))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in macro body"))); _tr_str_release(_cl); _cres; })); MacroCtx_fail(self, _at_t2216); _tr_str_release(_at_t2216); });
    /* pass */
    return box_mv(MacroVal_make_MNil());
}

__attribute__((hot)) MacroVal* MacroCtx_eval_mexpr(MacroCtx* self, Expr* eptr) {
    /* pass */
    if ((((unsigned long long)(eptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
    /* pass */
    __auto_type _t2217 = (*eptr);
    if (_t2217.tag == Expr_ELitStr) {
        __auto_type s = _t2217.data.ELitStr.val;
        return box_mv(MacroVal_ctor_MStr(s));
    } else if (_t2217.tag == Expr_ERawStr) {
        __auto_type s = _t2217.data.ERawStr.val;
        return box_mv(MacroVal_ctor_MStr(s));
    } else if (_t2217.tag == Expr_ELitInt) {
        __auto_type n = _t2217.data.ELitInt.val;
        return box_mv(MacroVal_ctor_MInt(n));
    } else if (_t2217.tag == Expr_ELitBool) {
        __auto_type b = _t2217.data.ELitBool.val;
        return box_mv(MacroVal_ctor_MBool(b));
    } else if (_t2217.tag == Expr_EIdent) {
        __auto_type name = _t2217.data.EIdent.name;
        /* pass */
        if (_tr_dict_contains(self->env, _tr_strz(name))) {
            /* pass */
            return ((MacroVal*)(uintptr_t)_tr_dict_get(self->env, _tr_strz(name)));
        }
        /* pass */
        ({ TrStr _at_t2218 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("macro: unknown name '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; })); MacroCtx_fail(self, _at_t2218); _tr_str_release(_at_t2218); });
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t2217.tag == Expr_EBinOp) {
        __auto_type op = _t2217.data.EBinOp.op;
__auto_type l = _t2217.data.EBinOp.left;
__auto_type r = _t2217.data.EBinOp.right;
        /* pass */
        return MacroCtx_eval_binop(self, op, MacroCtx_eval_mexpr(self, l), MacroCtx_eval_mexpr(self, r));
    } else if (_t2217.tag == Expr_EFString) {
        __auto_type parts = _t2217.data.EFString.parts;
        /* pass */
        TrStr out = _tr_str_lit("");
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < parts->len)) {
            /* pass */
            FStringPart* part = ((FStringPart*)List_ptr_get(parts, i));
            /* pass */
            if (part->is_expr) {
                /* pass */
                TrStr _strtmp_t2219 = ({ TrStr _cr = (mv_to_str(MacroCtx_eval_mexpr(self, part->expr))); TrStr _cres = _tr_strx_concat(_tr_strz(out), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(out);
                out = _strtmp_t2219;
            } else {
                /* pass */
                TrStr _strtmp_t2220 = _tr_strx_concat(_tr_strz(out), _tr_strz(part->text));
                _tr_str_release(out);
                out = _strtmp_t2220;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return box_mv(MacroVal_ctor_MStr(out));
    } else if (_t2217.tag == Expr_EPropAccess) {
        __auto_type obj = _t2217.data.EPropAccess.obj;
__auto_type prop = _t2217.data.EPropAccess.prop;
        /* pass */
        return mrec_get(MacroCtx_eval_mexpr(self, obj), prop);
    } else if (_t2217.tag == Expr_EMethodCall) {
        __auto_type obj = _t2217.data.EMethodCall.obj;
__auto_type method = _t2217.data.EMethodCall.method;
__auto_type margs = _t2217.data.EMethodCall.args;
        /* pass */
        MacroVal* ov = MacroCtx_eval_mexpr(self, obj);
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0)) {
            /* pass */
            __auto_type _t2221 = (*ov);
            if (_t2221.tag == MacroVal_MList) {
                __auto_type items = _t2221.data.MList.items;
                return box_mv(MacroVal_ctor_MInt(items->len));
            } else if (1) {
                __auto_type _ = _t2221;
                /* pass */
            }
            /* pass */
            return box_mv(MacroVal_ctor_MInt(0LL));
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_str"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_string"))) == 0))) {
            /* pass */
            return ({ TrStr _at_t2222 = (mv_to_str(ov)); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t2222))); _tr_str_release(_at_t2222); _wr; });
        }
        /* pass */
        ({ TrStr _at_t2223 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("macro: unsupported method '")), _tr_strz(method))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in macro body"))); _tr_str_release(_cl); _cres; })); MacroCtx_fail(self, _at_t2223); _tr_str_release(_at_t2223); });
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t2217.tag == Expr_ECall) {
        __auto_type callee = _t2217.data.ECall.callee;
__auto_type cargs = _t2217.data.ECall.args;
        /* pass */
        __auto_type _t2224 = (*callee);
        if (_t2224.tag == Expr_EIdent) {
            __auto_type cn = _t2224.data.EIdent.name;
            /* pass */
            if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("macro_error"))) == 0)) {
                /* pass */
                TrStr msg = _tr_str_lit("macro_error");
                /* pass */
                if ((cargs->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t2225 = mv_to_str(MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(cargs, 0LL))));
                    _tr_str_release(msg);
                    msg = _strtmp_t2225;
                }
                /* pass */
                MacroCtx_fail(self, msg);
                /* pass */
                _tr_str_release(msg);
                return box_mv(MacroVal_make_MNil());
            }
        } else if (1) {
            __auto_type _ = _t2224;
            /* pass */
        }
        /* pass */
        MacroCtx_fail(self, _tr_str_lit("macro: unsupported call in macro body"));
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t2217.tag == Expr_EList) {
        __auto_type items = _t2217.data.EList.items;
        /* pass */
        List_ptr* l = (void*)List_ptr_new();
        /* pass */
        long long i2 = 0LL;
        /* pass */
        while ((i2 < items->len)) {
            /* pass */
            List_ptr_append(l, MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(items, i2))));
            /* pass */
            i2 = (i2 + 1LL);
        }
        /* pass */
        return box_mv(MacroVal_ctor_MList(l));
    } else if (1) {
        __auto_type _ = _t2217;
        /* pass */
        MacroCtx_fail(self, _tr_str_lit("macro: unsupported expression in macro body"));
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
}

__attribute__((hot)) void MacroCtx_eval_mblock(MacroCtx* self, Block* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        if ((self->returned || self->has_error)) {
            /* pass */
            return;
        }
        /* pass */
        MacroCtx_eval_mstmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void MacroCtx_eval_mstmt(MacroCtx* self, Stmt* sptr) {
    /* pass */
    if ((((unsigned long long)(sptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t2226 = (*sptr);
    if (_t2226.tag == Stmt_SLet) {
        __auto_type name = _t2226.data.SLet.name;
__auto_type val = _t2226.data.SLet.val;
        /* pass */
        _tr_dict_set(self->env, _tr_strz(name), MacroCtx_eval_mexpr(self, val));
    } else if (_t2226.tag == Stmt_SAssign) {
        __auto_type target = _t2226.data.SAssign.target;
__auto_type val = _t2226.data.SAssign.val;
        /* pass */
        __auto_type _t2227 = (*target);
        if (_t2227.tag == Expr_EIdent) {
            __auto_type nm = _t2227.data.EIdent.name;
            _tr_dict_set(self->env, _tr_strz(nm), MacroCtx_eval_mexpr(self, val));
        } else if (1) {
            __auto_type _ = _t2227;
            MacroCtx_fail(self, _tr_str_lit("macro: assignment target must be a simple variable"));
        }
    } else if (_t2226.tag == Stmt_SReturn) {
        __auto_type val = _t2226.data.SReturn.val;
        /* pass */
        self->result = mv_to_str(MacroCtx_eval_mexpr(self, val));
        /* pass */
        self->returned = true;
    } else if (_t2226.tag == Stmt_SExpr) {
        __auto_type e = _t2226.data.SExpr.expr;
        /* pass */
        MacroVal* _r = MacroCtx_eval_mexpr(self, e);
    } else if (_t2226.tag == Stmt_SFor) {
        __auto_type var = _t2226.data.SFor.var;
__auto_type iter = _t2226.data.SFor.iter;
__auto_type body = _t2226.data.SFor.body;
        /* pass */
        MacroVal* iv = MacroCtx_eval_mexpr(self, iter);
        /* pass */
        __auto_type _t2228 = (*iv);
        if (_t2228.tag == MacroVal_MList) {
            __auto_type items = _t2228.data.MList.items;
            /* pass */
            long long i = 0LL;
            /* pass */
            while ((i < items->len)) {
                /* pass */
                if ((self->returned || self->has_error)) {
                    /* pass */
                    return;
                }
                /* pass */
                _tr_dict_set(self->env, _tr_strz(var), ((MacroVal*)List_ptr_get(items, i)));
                /* pass */
                MacroCtx_eval_mblock(self, body);
                /* pass */
                i = (i + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t2228;
            MacroCtx_fail(self, _tr_str_lit("macro: 'for' needs a list (e.g. item.fields)"));
        }
    } else if (_t2226.tag == Stmt_SIf) {
        __auto_type cond = _t2226.data.SIf.cond;
__auto_type then_b = _t2226.data.SIf.then_b;
__auto_type elifs = _t2226.data.SIf.elifs;
__auto_type else_b = _t2226.data.SIf.else_b;
        /* pass */
        if (mv_truthy(MacroCtx_eval_mexpr(self, cond))) {
            /* pass */
            MacroCtx_eval_mblock(self, then_b);
        } else {
            /* pass */
            bool done = false;
            /* pass */
            long long i = 0LL;
            /* pass */
            while (((i < elifs->len) && (!done))) {
                /* pass */
                ElifClause* ec = ((ElifClause*)List_ptr_get(elifs, i));
                /* pass */
                if (mv_truthy(MacroCtx_eval_mexpr(self, ec->cond))) {
                    /* pass */
                    MacroCtx_eval_mblock(self, (*ec->body));
                    /* pass */
                    done = true;
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            if ((!done)) {
                /* pass */
                MacroCtx_eval_mblock(self, else_b);
            }
        }
    } else if (_t2226.tag == Stmt_SPass) {
        /* pass */
    } else if (_t2226.tag == Stmt_SLine) {
        __auto_type _ = _t2226.data.SLine.n;
        /* pass */
    } else if (1) {
        __auto_type _ = _t2226;
        /* pass */
        MacroCtx_fail(self, _tr_str_lit("macro: unsupported statement in macro body"));
    }
}

__attribute__((hot)) MacroVal* box_mv(MacroVal v) {
    /* pass */
    /* unsafe block */
    /* pass */
    MacroVal* p = ((MacroVal*)_tr_c_calloc((size_t)(1LL), sizeof(MacroVal)));
    /* pass */
    (*p = v);
    /* pass */
    return p;
}

__attribute__((hot)) MacroVal* mrec(List_TrStr* keys, List_ptr* vals) {
    /* pass */
    return box_mv(MacroVal_ctor_MRec(keys, vals));
}

__attribute__((hot)) MacroVal* mrec_get(MacroVal* recptr, TrStr key) {
    /* pass */
    if ((((unsigned long long)(recptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
    /* pass */
    __auto_type _t2229 = (*recptr);
    if (_t2229.tag == MacroVal_MRec) {
        __auto_type keys = _t2229.data.MRec.keys;
__auto_type vals = _t2229.data.MRec.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            if ((strcmp(_tr_strz(List_TrStr_get(keys, i)), _tr_strz(key)) == 0)) {
                /* pass */
                return ((MacroVal*)List_ptr_get(vals, i));
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t2229;
        /* pass */
    }
    /* pass */
    return box_mv(MacroVal_make_MNil());
}

__attribute__((hot)) TrStr mv_to_str(MacroVal* vptr) {
    /* pass */
    if ((((unsigned long long)(vptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type _t2230 = (*vptr);
    if (_t2230.tag == MacroVal_MStr) {
        __auto_type s = _t2230.data.MStr.s;
        return _tr_str_retain(s);
    } else if (_t2230.tag == MacroVal_MInt) {
        __auto_type n = _t2230.data.MInt.n;
        return _tr_str_wrap(_tr_int_to_str((long long)(n)));
    } else if (_t2230.tag == MacroVal_MBool) {
        __auto_type b = _t2230.data.MBool.b;
        /* pass */
        if (b) {
            /* pass */
            return _tr_str_lit("true");
        }
        /* pass */
        return _tr_str_lit("false");
    } else if (1) {
        __auto_type _ = _t2230;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool mv_truthy(MacroVal* vptr) {
    /* pass */
    if ((((unsigned long long)(vptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t2231 = (*vptr);
    if (_t2231.tag == MacroVal_MBool) {
        __auto_type b = _t2231.data.MBool.b;
        return b;
    } else if (_t2231.tag == MacroVal_MInt) {
        __auto_type n = _t2231.data.MInt.n;
        return (n != 0LL);
    } else if (_t2231.tag == MacroVal_MStr) {
        __auto_type s = _t2231.data.MStr.s;
        return (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit(""))) != 0);
    } else if (1) {
        __auto_type _ = _t2231;
        return false;
    }
}

__attribute__((hot)) bool mv_eq(MacroVal* a, MacroVal* b) {
    /* pass */
    return (strcmp(_tr_strz(mv_to_str(a)), _tr_strz(mv_to_str(b))) == 0);
}

__attribute__((hot)) TrStr render_type(AstType** typtr) {
    /* pass */
    if ((((unsigned long long)(typtr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    AstType* t = (*typtr);
    /* pass */
    TrStr base = t->name;
    /* pass */
    if ((t->args->len > 0LL)) {
        /* pass */
        TrStr s = _tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("[")));
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < t->args->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t2232 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(s);
                s = _strtmp_t2232;
            }
            /* pass */
            TrStr _strtmp_t2233 = ({ TrStr _cr = (render_type(((AstType**)List_ptr_get(t->args, i)))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t2233;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        base = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(s);
    }
    /* pass */
    if (t->is_mut_borrow) {
        /* pass */
        return _tr_strx_concat(_tr_strz(_tr_str_lit("mut ref ")), _tr_strz(base));
    }
    /* pass */
    if (t->is_borrow) {
        /* pass */
        return _tr_strx_concat(_tr_strz(_tr_str_lit("ref ")), _tr_strz(base));
    }
    /* pass */
    return _tr_str_retain(base);
}

__attribute__((hot)) MacroVal* str_list(List_TrStr* items) {
    /* pass */
    List_ptr* l = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < items->len)) {
        /* pass */
        ({ TrStr _at_t2234 = (List_TrStr_get(items, i)); List_ptr_append(l, box_mv(MacroVal_ctor_MStr(_at_t2234))); _tr_str_release(_at_t2234); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* param_rec(Param* p) {
    /* pass */
    List_TrStr* ks = (void*)List_TrStr_new();
    /* pass */
    List_ptr* vs = (void*)List_ptr_new();
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("name"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(p->name)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("type"));
    /* pass */
    ({ TrStr _at_t2235 = (render_type(p->ty)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t2235))); _tr_str_release(_at_t2235); });
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_ref"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_ref)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_mut"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_mut_ref)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_variadic"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_variadic)));
    /* pass */
    return mrec(ks, vs);
}

__attribute__((hot)) MacroVal* params_list(List_ptr* ps) {
    /* pass */
    List_ptr* l = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ps->len)) {
        /* pass */
        List_ptr_append(l, param_rec(((Param*)List_ptr_get(ps, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* fn_rec(FunctionDef* f) {
    /* pass */
    List_TrStr* ks = (void*)List_TrStr_new();
    /* pass */
    List_ptr* vs = (void*)List_ptr_new();
    /* pass */
    TrStr arglist = _tr_str_lit("");
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < f->params->len)) {
        /* pass */
        if ((ai > 0LL)) {
            /* pass */
            TrStr _strtmp_t2236 = _tr_strx_concat(_tr_strz(arglist), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(arglist);
            arglist = _strtmp_t2236;
        }
        /* pass */
        TrStr _strtmp_t2237 = _tr_strx_concat(_tr_strz(arglist), _tr_strz(((Param*)List_ptr_get(f->params, ai))->name));
        _tr_str_release(arglist);
        arglist = _strtmp_t2237;
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    TrStr thr = _tr_str_lit("");
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr _strtmp_t2238 = render_type(f->throws_ty);
        _tr_str_release(thr);
        thr = _strtmp_t2238;
    }
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("kind"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_tr_str_lit("fn"))));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("name"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(f->name)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("params"));
    /* pass */
    List_ptr_append(vs, params_list(f->params));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("ret"));
    /* pass */
    ({ TrStr _at_t2239 = (render_type(f->ret_ty)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t2239))); _tr_str_release(_at_t2239); });
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("arglist"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(arglist)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_pub"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(f->is_public)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_async"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(f->is_async)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("throws"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(thr)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("generics"));
    /* pass */
    List_ptr_append(vs, str_list(f->generics));
    /* pass */
    _tr_str_release(arglist);
    _tr_str_release(thr);
    return mrec(ks, vs);
}

__attribute__((hot)) MacroVal* method_list(List_ptr* ms) {
    /* pass */
    List_ptr* l = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ms->len)) {
        /* pass */
        List_ptr_append(l, fn_rec(((FunctionDef*)List_ptr_get(ms, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* build_item(Decl* declptr) {
    /* pass */
    if ((((unsigned long long)(declptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
    /* pass */
    __auto_type _t2240 = (*declptr);
    if (_t2240.tag == Decl_DClass) {
        __auto_type c = _t2240.data.DClass.cls;
        /* pass */
        List_TrStr* ks = (void*)List_TrStr_new();
        /* pass */
        List_ptr* vs = (void*)List_ptr_new();
        /* pass */
        List_ptr* fl = (void*)List_ptr_new();
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < c->fields->len)) {
            /* pass */
            FieldDef* fd = ((FieldDef*)List_ptr_get(c->fields, i));
            /* pass */
            List_TrStr* fks = (void*)List_TrStr_new();
            /* pass */
            List_ptr* fvs = (void*)List_ptr_new();
            /* pass */
            List_TrStr_append(fks, _tr_str_lit("name"));
            /* pass */
            List_ptr_append(fvs, box_mv(MacroVal_ctor_MStr(fd->name)));
            /* pass */
            List_TrStr_append(fks, _tr_str_lit("type"));
            /* pass */
            ({ TrStr _at_t2241 = (render_type(fd->ty)); List_ptr_append(fvs, box_mv(MacroVal_ctor_MStr(_at_t2241))); _tr_str_release(_at_t2241); });
            /* pass */
            List_ptr_append(fl, mrec(fks, fvs));
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("kind"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_tr_str_lit("class"))));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("name"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(c->name)));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("fields"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MList(fl)));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("methods"));
        /* pass */
        List_ptr_append(vs, method_list(c->methods));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("bases"));
        /* pass */
        List_ptr_append(vs, str_list(c->base_classes));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("interfaces"));
        /* pass */
        List_ptr_append(vs, str_list(c->iface_names));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("generics"));
        /* pass */
        List_ptr_append(vs, str_list(c->generics));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("is_value_type"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MBool((!c->is_class))));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("is_pub"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(c->is_public)));
        /* pass */
        return mrec(ks, vs);
    } else if (_t2240.tag == Decl_DFunction) {
        __auto_type f = _t2240.data.DFunction.func;
        /* pass */
        return fn_rec(f);
    } else if (_t2240.tag == Decl_DEnum) {
        __auto_type e = _t2240.data.DEnum.enm;
        /* pass */
        List_TrStr* ks2 = (void*)List_TrStr_new();
        /* pass */
        List_ptr* vs2 = (void*)List_ptr_new();
        /* pass */
        List_ptr* vl = (void*)List_ptr_new();
        /* pass */
        long long i2 = 0LL;
        /* pass */
        while ((i2 < e->variants->len)) {
            /* pass */
            VariantDef* vd = ((VariantDef*)List_ptr_get(e->variants, i2));
            /* pass */
            List_TrStr* vks = (void*)List_TrStr_new();
            /* pass */
            List_ptr* vvs = (void*)List_ptr_new();
            /* pass */
            List_TrStr* tys = (void*)List_TrStr_new();
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < vd->fields->len)) {
                /* pass */
                ({ TrStr _at_t2242 = (render_type(((Param*)List_ptr_get(vd->fields, j))->ty)); List_TrStr_append(tys, _at_t2242); _tr_str_release(_at_t2242); });
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            List_TrStr_append(vks, _tr_str_lit("name"));
            /* pass */
            List_ptr_append(vvs, box_mv(MacroVal_ctor_MStr(vd->name)));
            /* pass */
            List_TrStr_append(vks, _tr_str_lit("fields"));
            /* pass */
            List_ptr_append(vvs, str_list(tys));
            /* pass */
            List_ptr_append(vl, mrec(vks, vvs));
            /* pass */
            i2 = (i2 + 1LL);
        }
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("kind"));
        /* pass */
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MStr(_tr_str_lit("enum"))));
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("name"));
        /* pass */
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MStr(e->name)));
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("variants"));
        /* pass */
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MList(vl)));
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("generics"));
        /* pass */
        List_ptr_append(vs2, str_list(e->generics));
        /* pass */
        return mrec(ks2, vs2);
    } else if (_t2240.tag == Decl_DInterface) {
        __auto_type ifc = _t2240.data.DInterface.iface;
        /* pass */
        List_TrStr* ks3 = (void*)List_TrStr_new();
        /* pass */
        List_ptr* vs3 = (void*)List_ptr_new();
        /* pass */
        List_TrStr_append(ks3, _tr_str_lit("kind"));
        /* pass */
        List_ptr_append(vs3, box_mv(MacroVal_ctor_MStr(_tr_str_lit("interface"))));
        /* pass */
        List_TrStr_append(ks3, _tr_str_lit("name"));
        /* pass */
        List_ptr_append(vs3, box_mv(MacroVal_ctor_MStr(ifc->name)));
        /* pass */
        List_TrStr_append(ks3, _tr_str_lit("methods"));
        /* pass */
        List_ptr_append(vs3, method_list(ifc->methods));
        /* pass */
        return mrec(ks3, vs3);
    } else if (1) {
        __auto_type _ = _t2240;
        return box_mv(MacroVal_make_MNil());
    }
}

__attribute__((hot)) void parse_into(TrStr src, List_ptr* gen) {
    /* pass */
    Lexer* lx = Lexer_init(src);
    /* pass */
    List_Token* toks = Lexer_tokenize(lx);
    /* pass */
    Parser* ps = Parser_init(toks, lx->token_lines);
    /* pass */
    Program* gp = Parser_parse_program(ps);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < gp->decls->len)) {
        /* pass */
        List_ptr_append(gen, ((Decl*)List_ptr_get(gp->decls, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_ptr* decl_decorators(Decl* declptr) {
    /* pass */
    __auto_type _t2243 = (*declptr);
    if (_t2243.tag == Decl_DClass) {
        __auto_type c = _t2243.data.DClass.cls;
        return c->decorators;
    } else if (_t2243.tag == Decl_DFunction) {
        __auto_type f = _t2243.data.DFunction.func;
        return f->decorators;
    } else if (_t2243.tag == Decl_DEnum) {
        __auto_type e = _t2243.data.DEnum.enm;
        return e->decorators;
    } else if (_t2243.tag == Decl_DInterface) {
        __auto_type ifc = _t2243.data.DInterface.iface;
        return ifc->decorators;
    } else if (1) {
        __auto_type _ = _t2243;
        return (void*)List_ptr_new();
    }
}

__attribute__((hot)) long long expand_macros(Program* prog) {
    /* pass */
    TrMap* macros = _tr_dict_new(16LL);
    /* pass */
    long long nmac = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        __auto_type _t2244 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t2244.tag == Decl_DDecoratorDef) {
            __auto_type f = _t2244.data.DDecoratorDef.func;
            /* pass */
            if (f->is_macro) {
                /* pass */
                _tr_dict_set(macros, _tr_strz(f->name), f);
                /* pass */
                nmac = (nmac + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t2244;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((nmac == 0LL)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    long long errors = 0LL;
    /* pass */
    List_ptr* out = (void*)List_ptr_new();
    /* pass */
    List_ptr* gen = (void*)List_ptr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Decl* dptr = ((Decl*)List_ptr_get(prog->decls, i));
        /* pass */
        bool is_macro_def = false;
        /* pass */
        __auto_type _t2245 = (*dptr);
        if (_t2245.tag == Decl_DDecoratorDef) {
            __auto_type f = _t2245.data.DDecoratorDef.func;
            /* pass */
            if (f->is_macro) {
                /* pass */
                is_macro_def = true;
            }
        } else if (1) {
            __auto_type _ = _t2245;
            /* pass */
        }
        /* pass */
        if ((!is_macro_def)) {
            /* pass */
            List_ptr* decs = decl_decorators(dptr);
            /* pass */
            long long di = 0LL;
            /* pass */
            while ((di < decs->len)) {
                /* pass */
                TrStr dname = ((Decorator*)List_ptr_get(decs, di))->name;
                /* pass */
                if (_tr_dict_contains(macros, _tr_strz(dname))) {
                    /* pass */
                    MacroCtx* ctx = MacroCtx_init();
                    /* pass */
                    FunctionDef* mdef = ((FunctionDef*)(uintptr_t)_tr_dict_get(macros, _tr_strz(dname)));
                    /* pass */
                    if ((mdef->params->len > 0LL)) {
                        /* pass */
                        _tr_dict_set(ctx->env, _tr_strz(((Param*)List_ptr_get(mdef->params, 0LL))->name), build_item(dptr));
                    }
                    /* pass */
                    MacroCtx_eval_mblock(ctx, mdef->body);
                    /* pass */
                    if (ctx->has_error) {
                        /* pass */
                        printf("%s\n", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("error: [MACRO] @")), _tr_strz(dname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ctx->error_msg)); _tr_str_release(_cl); _cres; })));
                        /* pass */
                        errors = (errors + 1LL);
                    } else {
                        /* pass */
                        parse_into(ctx->result, gen);
                    }
                }
                /* pass */
                di = (di + 1LL);
            }
            /* pass */
            List_ptr_append(out, dptr);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < gen->len)) {
        /* pass */
        List_ptr_append(out, List_ptr_get(gen, gi));
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    prog->decls = out;
    /* pass */
    return errors;
}

