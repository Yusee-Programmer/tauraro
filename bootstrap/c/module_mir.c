#include "tauraro_types.h"

bool _is_mutating_call_on(HirExpr* val, TrStr source, TrMap* mm);
void lower_stmts(MirBuilder* b, HirBlock* hb);
bool set_contains(List_TrStr* v, TrStr s);
bool set_add(List_TrStr* v, TrStr s);
void collect_uses(HirExpr* e, List_TrStr* out);
void collect_raw_borrows(HirExpr* e, LiveSet* out);
void add_exposed(HirExpr* e, List_TrStr* gen, List_TrStr* kill);
void block_use_def(MirBlock* blk, List_TrStr* gen, List_TrStr* kill);
void block_succs(MirBlock* blk, List_i64* out);
void collect_moves(HirExpr* e, List_TrStr* out);
bool is_free_fn(TrStr n);
TrStr free_target(HirExpr* arg);
void collect_frees(HirExpr* e, List_TrStr* out);
void block_gen_own(MirBlock* blk, List_TrStr* out);
void block_moves(MirBlock* blk, List_TrStr* out);
void preds_of(MirFunction* mf, long long b, List_i64* out);
long long last_use_in_block(MirBlock* blk, TrStr name);
bool is_if_body(MirFunction* mf, HirBlock* hb);
DropSite* site_for(List_ptr* out, HirBlock* hb);
bool borrower_live_after(MirBlock* blk, long long after_idx, TrStr name, LiveSet* lo);
TrStr set_str(List_TrStr* v);
TrStr term_str(MirTerm* t);
TrStr stmt_str(MirStmt* s);

__attribute__((malloc,returns_nonnull,hot)) MirProgram* MirProgram_init() {
    /* pass */
    MirProgram* p = ((MirProgram*)_tr_checked_alloc(sizeof(MirProgram)));
    /* pass */
    p->functions = (void*)List_ptr_new();
    /* pass */
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) MirBuilder* MirBuilder_init() {
    /* pass */
    MirBuilder* b = ((MirBuilder*)_tr_checked_alloc(sizeof(MirBuilder)));
    /* pass */
    b->blocks = (void*)List_ptr_new();
    /* pass */
    b->cur = (-1LL);
    /* pass */
    b->complete = true;
    /* pass */
    b->if_bodies = (void*)List_ptr_new();
    /* pass */
    b->in_unsafe = 0LL;
    /* pass */
    b->unsafe_pinned = LiveSet_init();
    /* pass */
    b->loop_continue = (void*)List_i64_new();
    /* pass */
    b->loop_break = (void*)List_i64_new();
    /* pass */
    b->borrows = (void*)List_ptr_new();
    /* pass */
    return b;
}

__attribute__((hot)) void MirBuilder_record_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
    /* pass */
    BorrowEdge* e = ((BorrowEdge*)_tr_checked_alloc(sizeof(BorrowEdge)));
    /* pass */
    e->borrower = _tr_str_retain(borrower);
    /* pass */
    e->source = _tr_str_retain(source);
    /* pass */
    e->decl_block = self->cur;
    /* pass */
    e->is_exclusive = exclusive;
    /* pass */
    e->via_collection = false;
    /* pass */
    List_ptr_append(self->borrows, e);
}

__attribute__((hot)) void MirBuilder_record_coll_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
    /* pass */
    BorrowEdge* e = ((BorrowEdge*)_tr_checked_alloc(sizeof(BorrowEdge)));
    /* pass */
    e->borrower = _tr_str_retain(borrower);
    /* pass */
    e->source = _tr_str_retain(source);
    /* pass */
    e->decl_block = self->cur;
    /* pass */
    e->is_exclusive = exclusive;
    /* pass */
    e->via_collection = true;
    /* pass */
    List_ptr_append(self->borrows, e);
}

__attribute__((hot)) long long MirBuilder_new_block(MirBuilder* self) {
    /* pass */
    MirBlock* blk = ((MirBlock*)_tr_checked_alloc(sizeof(MirBlock)));
    /* pass */
    blk->id = self->blocks->len;
    /* pass */
    blk->stmts = (void*)List_ptr_new();
    /* pass */
    blk->term = box_mirterm(MirTerm_make_TUnset());
    /* pass */
    List_ptr_append(self->blocks, blk);
    /* pass */
    return blk->id;
}

__attribute__((hot)) void MirBuilder_push_stmt(MirBuilder* self, MirStmt s) {
    /* pass */
    if ((self->cur >= 0LL)) {
        /* pass */
        MirBlock* blk = ((MirBlock*)List_ptr_get(self->blocks, self->cur));
        /* pass */
        List_ptr_append(blk->stmts, box_mirstmt(s));
        /* pass */
        if ((((unsigned long long)(blk->hir_block)) == ((unsigned long long)(0LL)))) {
            /* pass */
            blk->hir_block = self->cur_hb;
        }
    }
}

__attribute__((hot)) void MirBuilder_set_term(MirBuilder* self, MirTerm t) {
    /* pass */
    if ((self->cur >= 0LL)) {
        /* pass */
        ((MirBlock*)List_ptr_get(self->blocks, self->cur))->term = box_mirterm(t);
    }
}

__attribute__((hot)) bool MirBuilder_terminated(MirBuilder* self) {
    /* pass */
    if ((self->cur < 0LL)) {
        /* pass */
        return true;
    }
    /* pass */
    __auto_type _t223 = (*((MirBlock*)List_ptr_get(self->blocks, self->cur))->term);
    if (_t223.tag == MirTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t223;
        return true;
    }
}

__attribute__((malloc,returns_nonnull,hot)) LiveSet* LiveSet_init() {
    /* pass */
    LiveSet* s = ((LiveSet*)_tr_checked_alloc(sizeof(LiveSet)));
    /* pass */
    s->items = (void*)List_TrStr_new();
    /* pass */
    return s;
}

__attribute__((hot)) bool LiveSet_has(LiveSet* self, TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->items->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->items, i)), _tr_strz(s)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool LiveSet_add(LiveSet* self, TrStr s) {
    /* pass */
    if (LiveSet_has(self, s)) {
        /* pass */
        return false;
    }
    /* pass */
    List_TrStr_append(self->items, s);
    /* pass */
    return true;
}

__attribute__((hot)) bool LiveSet_union_in(LiveSet* self, LiveSet* other) {
    /* pass */
    bool changed = false;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < other->items->len)) {
        /* pass */
        if (({ TrStr _at_t224 = (List_TrStr_get(other->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t224)); _tr_str_release(_at_t224); _wr; })) {
            /* pass */
            changed = true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return changed;
}

__attribute__((hot)) bool LiveSet_union_except(LiveSet* self, LiveSet* src, LiveSet* block) {
    /* pass */
    bool changed = false;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < src->items->len)) {
        /* pass */
        if (({ TrStr _at_t225 = (List_TrStr_get(src->items, i)); __auto_type _wr = ((!LiveSet_has(block, _at_t225))); _tr_str_release(_at_t225); _wr; })) {
            /* pass */
            if (({ TrStr _at_t226 = (List_TrStr_get(src->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t226)); _tr_str_release(_at_t226); _wr; })) {
                /* pass */
                changed = true;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return changed;
}

__attribute__((hot)) LiveSet* LiveSet_clone(LiveSet* self) {
    /* pass */
    LiveSet* r = LiveSet_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->items->len)) {
        /* pass */
        ({ TrStr _at_t227 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _at_t227); _tr_str_release(_at_t227); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return r;
}

__attribute__((hot)) void LiveSet_retain_common(LiveSet* self, LiveSet* other) {
    /* pass */
    List_TrStr* keep = (void*)List_TrStr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->items->len)) {
        /* pass */
        if (({ TrStr _at_t228 = (List_TrStr_get(self->items, i)); __auto_type _wr = (LiveSet_has(other, _at_t228)); _tr_str_release(_at_t228); _wr; })) {
            /* pass */
            ({ TrStr _at_t229 = (List_TrStr_get(self->items, i)); List_TrStr_append(keep, _at_t229); _tr_str_release(_at_t229); });
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    self->items = keep;
}

__attribute__((hot)) bool LiveSet_equals(LiveSet* self, LiveSet* other) {
    /* pass */
    if ((self->items->len != other->items->len)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->items->len)) {
        /* pass */
        if (({ TrStr _at_t230 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t230))); _tr_str_release(_at_t230); _wr; })) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) void LiveSet_set_to(LiveSet* self, LiveSet* other) {
    /* pass */
    List_TrStr* fresh = (void*)List_TrStr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < other->items->len)) {
        /* pass */
        ({ TrStr _at_t231 = (List_TrStr_get(other->items, i)); List_TrStr_append(fresh, _at_t231); _tr_str_release(_at_t231); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    self->items = fresh;
}

__attribute__((hot)) LiveSet* LiveSet_minus(LiveSet* self, LiveSet* other) {
    /* pass */
    LiveSet* r = LiveSet_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->items->len)) {
        /* pass */
        if (({ TrStr _at_t232 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t232))); _tr_str_release(_at_t232); _wr; })) {
            /* pass */
            ({ TrStr _at_t233 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _at_t233); _tr_str_release(_at_t233); });
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return r;
}

__attribute__((hot)) bool _is_mutating_call_on(HirExpr* val, TrStr source, TrMap* mm) {
    /* pass */
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t234 = (*val);
    if (_t234.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t234.data.EMethodCall.obj;
__auto_type method = _t234.data.EMethodCall.method;
        /* pass */
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        __auto_type _t235 = (*obj);
        if (_t235.tag == HirExpr_EIdent) {
            __auto_type onm = _t235.data.EIdent.name;
            /* pass */
            if ((strcmp(_tr_strz(onm), _tr_strz(source)) != 0)) {
                /* pass */
                return false;
            }
            /* pass */
            if (((((((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("push"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("append"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("insert"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("set"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("remove"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pop"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clear"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("extend"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("put"))) == 0))) {
                /* pass */
                return true;
            }
            /* pass */
            return ({ TrStr _dkt_t236 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(hir_expr_type(obj)->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t236))); _tr_str_release(_dkt_t236); _wr; });
        } else if (1) {
            __auto_type _ = _t235;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t234;
        return false;
    }
}

__attribute__((hot)) MirStmt* box_mirstmt(MirStmt s) {
    /* pass */
    /* unsafe block */
    /* pass */
    MirStmt* p = ((MirStmt*)_tr_c_calloc((size_t)(1LL), sizeof(MirStmt)));
    /* pass */
    (*p = s);
    /* pass */
    return p;
}

__attribute__((hot)) MirTerm* box_mirterm(MirTerm t) {
    /* pass */
    /* unsafe block */
    /* pass */
    MirTerm* p = ((MirTerm*)_tr_c_calloc((size_t)(1LL), sizeof(MirTerm)));
    /* pass */
    (*p = t);
    /* pass */
    return p;
}

__attribute__((hot)) void lower_stmts(MirBuilder* b, HirBlock* hb) {
    /* pass */
    HirBlock* saved_hb = b->cur_hb;
    /* pass */
    b->cur_hb = hb;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        if (MirBuilder_terminated(b)) {
            /* pass */
            b->cur = MirBuilder_new_block(b);
        }
        /* pass */
        HirStmt* sp = ((HirStmt*)List_ptr_get(hb->stmts, i));
        /* pass */
        __auto_type _t237 = (*sp);
        if (_t237.tag == HirStmt_SLet) {
            __auto_type nm = _t237.data.SLet.name;
__auto_type slet_ty = _t237.data.SLet.ty;
__auto_type val = _t237.data.SLet.val;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MDeclare(nm, val));
            /* pass */
            if ((slet_ty->is_borrow && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
                /* pass */
                bool _slet_mut = false;
                /* pass */
                __auto_type _t238 = (*sp);
                if (_t238.tag == HirStmt_SLet) {
                    __auto_type _sm = _t238.data.SLet.is_mut;
                    _slet_mut = _sm;
                } else if (1) {
                    __auto_type _ = _t238;
                    /* pass */
                }
                /* pass */
                __auto_type _t239 = (*val);
                if (_t239.tag == HirExpr_EIdent) {
                    __auto_type src_nm = _t239.data.EIdent.name;
                    MirBuilder_record_borrow(b, nm, src_nm, (_slet_mut || slet_ty->is_mut_borrow));
                } else if (_t239.tag == HirExpr_EMethodCall) {
                    __auto_type mobj = _t239.data.EMethodCall.obj;
__auto_type mmethod = _t239.data.EMethodCall.method;
                    /* pass */
                    if (((strcmp(_tr_strz(mmethod), _tr_strz(_tr_str_lit("get"))) == 0) || (strcmp(_tr_strz(mmethod), _tr_strz(_tr_str_lit("get_or"))) == 0))) {
                        /* pass */
                        __auto_type _t240 = (*mobj);
                        if (_t240.tag == HirExpr_EIdent) {
                            __auto_type cnm = _t240.data.EIdent.name;
                            MirBuilder_record_coll_borrow(b, nm, cnm, (_slet_mut || slet_ty->is_mut_borrow));
                        } else if (1) {
                            __auto_type _ = _t240;
                            /* pass */
                        }
                    }
                } else if (_t239.tag == HirExpr_EIndex) {
                    __auto_type iobj = _t239.data.EIndex.obj;
                    /* pass */
                    __auto_type _t241 = (*iobj);
                    if (_t241.tag == HirExpr_EIdent) {
                        __auto_type cnm2 = _t241.data.EIdent.name;
                        MirBuilder_record_coll_borrow(b, nm, cnm2, (_slet_mut || slet_ty->is_mut_borrow));
                    } else if (1) {
                        __auto_type _ = _t241;
                        /* pass */
                    }
                } else if (1) {
                    __auto_type _ = _t239;
                    /* pass */
                }
            }
        } else if (_t237.tag == HirStmt_SAssign) {
            __auto_type tgt = _t237.data.SAssign.target;
__auto_type val = _t237.data.SAssign.val;
            /* pass */
            __auto_type _t242 = (*tgt);
            if (_t242.tag == HirExpr_EIdent) {
                __auto_type tn = _t242.data.EIdent.name;
                MirBuilder_push_stmt(b, MirStmt_ctor_MAssign(tn, val));
            } else if (1) {
                __auto_type _ = _t242;
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(val));
            }
        } else if (_t237.tag == HirStmt_SExpr) {
            __auto_type e = _t237.data.SExpr.expr;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(e));
        } else if (_t237.tag == HirStmt_SReturn) {
            __auto_type v = _t237.data.SReturn.val;
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TReturn(v));
        } else if (_t237.tag == HirStmt_SIf) {
            __auto_type cond = _t237.data.SIf.cond;
__auto_type then_b = _t237.data.SIf.then_b;
__auto_type else_b = _t237.data.SIf.else_b;
            /* pass */
            long long tb = MirBuilder_new_block(b);
            /* pass */
            long long eb = MirBuilder_new_block(b);
            /* pass */
            long long jb = MirBuilder_new_block(b);
            /* pass */
            if ((b->in_unsafe == 0LL)) {
                /* pass */
                List_ptr_append(b->if_bodies, then_b);
                /* pass */
                List_ptr_append(b->if_bodies, else_b);
            }
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, tb, eb));
            /* pass */
            b->cur = tb;
            /* pass */
            lower_stmts(b, then_b);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
            /* pass */
            b->cur = eb;
            /* pass */
            lower_stmts(b, else_b);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
            /* pass */
            b->cur = jb;
        } else if (_t237.tag == HirStmt_SWhile) {
            __auto_type cond = _t237.data.SWhile.cond;
__auto_type body = _t237.data.SWhile.body;
            /* pass */
            long long hdr = MirBuilder_new_block(b);
            /* pass */
            long long body_b = MirBuilder_new_block(b);
            /* pass */
            long long exit_b = MirBuilder_new_block(b);
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
            /* pass */
            b->cur = hdr;
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, body_b, exit_b));
            /* pass */
            b->cur = body_b;
            /* pass */
            List_i64_append(b->loop_continue, hdr);
            /* pass */
            List_i64_append(b->loop_break, exit_b);
            /* pass */
            lower_stmts(b, body);
            /* pass */
            List_i64_pop(b->loop_continue);
            /* pass */
            List_i64_pop(b->loop_break);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
            }
            /* pass */
            b->cur = exit_b;
        } else if (_t237.tag == HirStmt_SFor) {
            __auto_type fiter = _t237.data.SFor.iter;
__auto_type fbody = _t237.data.SFor.body;
            /* pass */
            long long for_hdr = MirBuilder_new_block(b);
            /* pass */
            long long for_body = MirBuilder_new_block(b);
            /* pass */
            long long for_exit = MirBuilder_new_block(b);
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
            /* pass */
            b->cur = for_hdr;
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fiter, for_body, for_exit));
            /* pass */
            b->cur = for_body;
            /* pass */
            List_i64_append(b->loop_continue, for_hdr);
            /* pass */
            List_i64_append(b->loop_break, for_exit);
            /* pass */
            lower_stmts(b, fbody);
            /* pass */
            List_i64_pop(b->loop_continue);
            /* pass */
            List_i64_pop(b->loop_break);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
            }
            /* pass */
            b->cur = for_exit;
        } else if (_t237.tag == HirStmt_SForUnpack) {
            __auto_type fuiter = _t237.data.SForUnpack.iter;
__auto_type fubody = _t237.data.SForUnpack.body;
            /* pass */
            long long fu_hdr = MirBuilder_new_block(b);
            /* pass */
            long long fu_body = MirBuilder_new_block(b);
            /* pass */
            long long fu_exit = MirBuilder_new_block(b);
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
            /* pass */
            b->cur = fu_hdr;
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fuiter, fu_body, fu_exit));
            /* pass */
            b->cur = fu_body;
            /* pass */
            List_i64_append(b->loop_continue, fu_hdr);
            /* pass */
            List_i64_append(b->loop_break, fu_exit);
            /* pass */
            lower_stmts(b, fubody);
            /* pass */
            List_i64_pop(b->loop_continue);
            /* pass */
            List_i64_pop(b->loop_break);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
            }
            /* pass */
            b->cur = fu_exit;
        } else if (_t237.tag == HirStmt_STry) {
            __auto_type try_body = _t237.data.STry.try_body;
__auto_type catches = _t237.data.STry.catches;
__auto_type finally_b = _t237.data.STry.finally_b;
            /* pass */
            long long try_b = MirBuilder_new_block(b);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_b));
            }
            /* pass */
            b->cur = try_b;
            /* pass */
            List_ptr_append(b->if_bodies, try_body);
            /* pass */
            lower_stmts(b, try_body);
            /* pass */
            long long tci = 0LL;
            /* pass */
            while ((tci < catches->len)) {
                /* pass */
                HirCatchClause* tcc = (*((HirCatchClause**)List_ptr_get(catches, tci)));
                /* pass */
                long long catch_b = MirBuilder_new_block(b);
                /* pass */
                if ((!MirBuilder_terminated(b))) {
                    /* pass */
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(catch_b));
                }
                /* pass */
                b->cur = catch_b;
                /* pass */
                List_ptr_append(b->if_bodies, tcc->body);
                /* pass */
                lower_stmts(b, tcc->body);
                /* pass */
                tci = (tci + 1LL);
            }
            /* pass */
            long long fin_b = MirBuilder_new_block(b);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fin_b));
            }
            /* pass */
            b->cur = fin_b;
            /* pass */
            List_ptr_append(b->if_bodies, finally_b);
            /* pass */
            lower_stmts(b, finally_b);
            /* pass */
            long long try_after = MirBuilder_new_block(b);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_after));
            }
            /* pass */
            b->cur = try_after;
        } else if (_t237.tag == HirStmt_SMatch) {
            __auto_type msubj = _t237.data.SMatch.expr;
__auto_type marms = _t237.data.SMatch.arms;
            /* pass */
            long long m_exit = MirBuilder_new_block(b);
            /* pass */
            long long mai = 0LL;
            /* pass */
            while ((mai < marms->len)) {
                /* pass */
                HirMatchArm* marm = ((HirMatchArm*)List_ptr_get(marms, mai));
                /* pass */
                long long arm_body = MirBuilder_new_block(b);
                /* pass */
                long long arm_next = MirBuilder_new_block(b);
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TBranch(msubj, arm_body, arm_next));
                /* pass */
                b->cur = arm_body;
                /* pass */
                if ((((unsigned long long)(marm->guard)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    MirBuilder_push_stmt(b, MirStmt_ctor_MEval(marm->guard));
                }
                /* pass */
                if ((b->in_unsafe == 0LL)) {
                    /* pass */
                    List_ptr_append(b->if_bodies, marm->body);
                }
                /* pass */
                lower_stmts(b, marm->body);
                /* pass */
                if ((!MirBuilder_terminated(b))) {
                    /* pass */
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
                }
                /* pass */
                b->cur = arm_next;
                /* pass */
                mai = (mai + 1LL);
            }
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
            }
            /* pass */
            b->cur = m_exit;
        } else if (_t237.tag == HirStmt_SAssert) {
            __auto_type acond = _t237.data.SAssert.cond;
__auto_type amsg = _t237.data.SAssert.msg;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(acond));
            /* pass */
            if ((((unsigned long long)(amsg)) != ((unsigned long long)(0LL)))) {
                /* pass */
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(amsg));
            }
        } else if (_t237.tag == HirStmt_SUnsafe) {
            __auto_type ubody = _t237.data.SUnsafe.body;
            /* pass */
            b->in_unsafe = (b->in_unsafe + 1LL);
            /* pass */
            lower_stmts(b, ubody);
            /* pass */
            b->in_unsafe = (b->in_unsafe - 1LL);
        } else if (_t237.tag == HirStmt_SRaise) {
            __auto_type rval = _t237.data.SRaise.val;
            /* pass */
            if ((((unsigned long long)(rval)) != ((unsigned long long)(0LL)))) {
                /* pass */
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(rval));
            }
        } else if (_t237.tag == HirStmt_SWith) {
            __auto_type witems = _t237.data.SWith.items;
__auto_type wbody = _t237.data.SWith.body;
            /* pass */
            long long wi = 0LL;
            /* pass */
            while ((wi < witems->len)) {
                /* pass */
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(((HirExpr*)List_ptr_get(witems, wi))));
                /* pass */
                wi = (wi + 1LL);
            }
            /* pass */
            lower_stmts(b, wbody);
        } else if (_t237.tag == HirStmt_SMultiLet) {
            __auto_type mlval = _t237.data.SMultiLet.val;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(mlval));
        } else if (_t237.tag == HirStmt_SLineMarker) {
            __auto_type _ = _t237.data.SLineMarker.n;
            /* pass */
            /* pass */
        } else if (_t237.tag == HirStmt_SAutoDrop) {
            /* pass */
            /* pass */
        } else if (_t237.tag == HirStmt_SBreak) {
            /* pass */
            if ((b->loop_break->len > 0LL)) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_break, (b->loop_break->len - 1LL))));
            } else {
                /* pass */
                b->complete = false;
            }
        } else if (_t237.tag == HirStmt_SContinue) {
            /* pass */
            if ((b->loop_continue->len > 0LL)) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_continue, (b->loop_continue->len - 1LL))));
            } else {
                /* pass */
                b->complete = false;
            }
        } else if (1) {
            __auto_type _ = _t237;
            /* pass */
            b->complete = false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    b->cur_hb = saved_hb;
}

__attribute__((hot)) MirFunction* lower_function(HirFunction* hf) {
    /* pass */
    MirFunction* mf = ((MirFunction*)_tr_checked_alloc(sizeof(MirFunction)));
    /* pass */
    mf->name = hf->name;
    /* pass */
    mf->params = (void*)List_TrStr_new();
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < hf->params->len)) {
        /* pass */
        List_TrStr_append(mf->params, ((HirParam*)List_ptr_get(hf->params, pi))->name);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    MirBuilder* b = MirBuilder_init();
    /* pass */
    b->cur = MirBuilder_new_block(b);
    /* pass */
    lower_stmts(b, hf->body);
    /* pass */
    if ((!MirBuilder_terminated(b))) {
        /* pass */
        MirBuilder_set_term(b, MirTerm_make_TReturnVoid());
    }
    /* pass */
    long long rbi = 0LL;
    /* pass */
    while ((rbi < b->blocks->len)) {
        /* pass */
        MirBlock* rblk = ((MirBlock*)List_ptr_get(b->blocks, rbi));
        /* pass */
        long long rsi = 0LL;
        /* pass */
        while ((rsi < rblk->stmts->len)) {
            /* pass */
            __auto_type _t243 = (*((MirStmt*)List_ptr_get(rblk->stmts, rsi)));
            if (_t243.tag == MirStmt_MDeclare) {
                __auto_type rv = _t243.data.MDeclare.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t243.tag == MirStmt_MAssign) {
                __auto_type rv = _t243.data.MAssign.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t243.tag == MirStmt_MEval) {
                __auto_type rv = _t243.data.MEval.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            }
            /* pass */
            rsi = (rsi + 1LL);
        }
        /* pass */
        __auto_type _t244 = (*rblk->term);
        if (_t244.tag == MirTerm_TBranch) {
            __auto_type rc = _t244.data.TBranch.cond;
            collect_raw_borrows(rc, b->unsafe_pinned);
        } else if (_t244.tag == MirTerm_TReturn) {
            __auto_type rv = _t244.data.TReturn.value;
            collect_raw_borrows(rv, b->unsafe_pinned);
        } else if (1) {
            __auto_type _ = _t244;
            /* pass */
        }
        /* pass */
        rbi = (rbi + 1LL);
    }
    /* pass */
    mf->blocks = b->blocks;
    /* pass */
    mf->complete = b->complete;
    /* pass */
    mf->if_bodies = b->if_bodies;
    /* pass */
    mf->unsafe_pinned = b->unsafe_pinned;
    /* pass */
    mf->borrows = b->borrows;
    /* pass */
    long long hbi = 0LL;
    /* pass */
    if ((((unsigned long long)(hf->borrow_borrowers)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return mf;
    }
    /* pass */
    if ((((unsigned long long)(hf->borrow_sources)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return mf;
    }
    /* pass */
    while (((hbi < hf->borrow_borrowers->len) && (hbi < hf->borrow_sources->len))) {
        /* pass */
        BorrowEdge* he = ((BorrowEdge*)_tr_checked_alloc(sizeof(BorrowEdge)));
        /* pass */
        he->borrower = List_TrStr_get(hf->borrow_borrowers, hbi);
        /* pass */
        he->source = List_TrStr_get(hf->borrow_sources, hbi);
        /* pass */
        he->decl_block = (-1LL);
        /* pass */
        List_ptr_append(mf->borrows, he);
        /* pass */
        hbi = (hbi + 1LL);
    }
    /* pass */
    return mf;
}

__attribute__((hot)) MirProgram* lower_program(HirProgram* hir) {
    /* pass */
    MirProgram* mp = MirProgram_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hir->functions->len)) {
        /* pass */
        List_ptr_append(mp->functions, lower_function(((HirFunction*)List_ptr_get(hir->functions, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return mp;
}

__attribute__((hot)) bool set_contains(List_TrStr* v, TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < v->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(v, i)), _tr_strz(s)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool set_add(List_TrStr* v, TrStr s) {
    /* pass */
    if (set_contains(v, s)) {
        /* pass */
        return false;
    }
    /* pass */
    List_TrStr_append(v, s);
    /* pass */
    return true;
}

__attribute__((hot)) void collect_uses(HirExpr* e, List_TrStr* out) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t245 = (*e);
    if (_t245.tag == HirExpr_EIdent) {
        __auto_type nm = _t245.data.EIdent.name;
        set_add(out, nm);
    } else if (_t245.tag == HirExpr_EBinOp) {
        __auto_type l = _t245.data.EBinOp.left;
__auto_type r = _t245.data.EBinOp.right;
        /* pass */
        collect_uses(l, out);
        /* pass */
        collect_uses(r, out);
    } else if (_t245.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t245.data.EUnaryOp.expr;
        collect_uses(x, out);
    } else if (_t245.tag == HirExpr_ECall) {
        __auto_type callee = _t245.data.ECall.callee;
__auto_type args = _t245.data.ECall.args;
        /* pass */
        collect_uses(callee, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t245.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t245.data.EMethodCall.obj;
__auto_type args = _t245.data.EMethodCall.args;
        /* pass */
        collect_uses(obj, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t245.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t245.data.EPropAccess.obj;
        collect_uses(obj, out);
    } else if (_t245.tag == HirExpr_EIndex) {
        __auto_type obj = _t245.data.EIndex.obj;
__auto_type idx = _t245.data.EIndex._tr_v_index;
        /* pass */
        collect_uses(obj, out);
        /* pass */
        collect_uses(idx, out);
    } else if (_t245.tag == HirExpr_ECast) {
        __auto_type x = _t245.data.ECast.expr;
        collect_uses(x, out);
    } else if (_t245.tag == HirExpr_EAwait) {
        __auto_type x = _t245.data.EAwait.expr;
        collect_uses(x, out);
    } else if (_t245.tag == HirExpr_EIfElse) {
        __auto_type c = _t245.data.EIfElse.cond;
__auto_type t = _t245.data.EIfElse.then_e;
__auto_type f = _t245.data.EIfElse.else_e;
        /* pass */
        collect_uses(c, out);
        /* pass */
        collect_uses(t, out);
        /* pass */
        collect_uses(f, out);
    } else if (1) {
        __auto_type _ = _t245;
        /* pass */
    }
}

__attribute__((hot)) void collect_raw_borrows(HirExpr* e, LiveSet* out) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t246 = (*e);
    if (_t246.tag == HirExpr_ECast) {
        __auto_type inner = _t246.data.ECast.expr;
__auto_type ty = _t246.data.ECast.target_ty;
        /* pass */
        if ((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
            /* pass */
            __auto_type _t247 = (*inner);
            if (_t247.tag == HirExpr_EIdent) {
                __auto_type nm = _t247.data.EIdent.name;
                bool _a = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t247;
                /* pass */
            }
        }
        /* pass */
        collect_raw_borrows(inner, out);
    } else if (_t246.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t246.data.EUnaryOp.op;
__auto_type inner = _t246.data.EUnaryOp.expr;
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("&"))) == 0)) {
            /* pass */
            __auto_type _t248 = (*inner);
            if (_t248.tag == HirExpr_EIdent) {
                __auto_type nm = _t248.data.EIdent.name;
                bool _b = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t248;
                /* pass */
            }
        }
        /* pass */
        collect_raw_borrows(inner, out);
    } else if (_t246.tag == HirExpr_EBinOp) {
        __auto_type l = _t246.data.EBinOp.left;
__auto_type r = _t246.data.EBinOp.right;
        /* pass */
        collect_raw_borrows(l, out);
        /* pass */
        collect_raw_borrows(r, out);
    } else if (_t246.tag == HirExpr_ECall) {
        __auto_type callee = _t246.data.ECall.callee;
__auto_type args = _t246.data.ECall.args;
        /* pass */
        collect_raw_borrows(callee, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t246.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t246.data.EMethodCall.obj;
__auto_type args = _t246.data.EMethodCall.args;
        /* pass */
        collect_raw_borrows(obj, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t246.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t246.data.EPropAccess.obj;
        collect_raw_borrows(obj, out);
    } else if (_t246.tag == HirExpr_EIndex) {
        __auto_type obj = _t246.data.EIndex.obj;
__auto_type idx = _t246.data.EIndex._tr_v_index;
        /* pass */
        collect_raw_borrows(obj, out);
        /* pass */
        collect_raw_borrows(idx, out);
    } else if (_t246.tag == HirExpr_EAwait) {
        __auto_type x = _t246.data.EAwait.expr;
        collect_raw_borrows(x, out);
    } else if (_t246.tag == HirExpr_EIfElse) {
        __auto_type c = _t246.data.EIfElse.cond;
__auto_type t = _t246.data.EIfElse.then_e;
__auto_type f = _t246.data.EIfElse.else_e;
        /* pass */
        collect_raw_borrows(c, out);
        /* pass */
        collect_raw_borrows(t, out);
        /* pass */
        collect_raw_borrows(f, out);
    } else if (1) {
        __auto_type _ = _t246;
        /* pass */
    }
}

__attribute__((hot)) void add_exposed(HirExpr* e, List_TrStr* gen, List_TrStr* kill) {
    /* pass */
    List_TrStr* tmp = (void*)List_TrStr_new();
    /* pass */
    collect_uses(e, tmp);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < tmp->len)) {
        /* pass */
        if (({ TrStr _at_t249 = (List_TrStr_get(tmp, i)); __auto_type _wr = ((!set_contains(kill, _at_t249))); _tr_str_release(_at_t249); _wr; })) {
            /* pass */
            ({ TrStr _at_t250 = (List_TrStr_get(tmp, i)); set_add(gen, _at_t250); _tr_str_release(_at_t250); });
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_use_def(MirBlock* blk, List_TrStr* gen, List_TrStr* kill) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < blk->stmts->len)) {
        /* pass */
        __auto_type _t251 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t251.tag == MirStmt_MDeclare) {
            __auto_type p = _t251.data.MDeclare.place;
__auto_type val = _t251.data.MDeclare.value;
            /* pass */
            add_exposed(val, gen, kill);
            /* pass */
            set_add(kill, p);
        } else if (_t251.tag == MirStmt_MAssign) {
            __auto_type p = _t251.data.MAssign.place;
__auto_type val = _t251.data.MAssign.value;
            /* pass */
            add_exposed(val, gen, kill);
            /* pass */
            set_add(kill, p);
        } else if (_t251.tag == MirStmt_MEval) {
            __auto_type val = _t251.data.MEval.value;
            add_exposed(val, gen, kill);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    __auto_type _t252 = (*blk->term);
    if (_t252.tag == MirTerm_TBranch) {
        __auto_type cond = _t252.data.TBranch.cond;
        add_exposed(cond, gen, kill);
    } else if (_t252.tag == MirTerm_TReturn) {
        __auto_type val = _t252.data.TReturn.value;
        add_exposed(val, gen, kill);
    } else if (1) {
        __auto_type _ = _t252;
        /* pass */
    }
}

__attribute__((hot)) void block_succs(MirBlock* blk, List_i64* out) {
    /* pass */
    __auto_type _t253 = (*blk->term);
    if (_t253.tag == MirTerm_TGoto) {
        __auto_type n = _t253.data.TGoto.target;
        List_i64_append(out, n);
    } else if (_t253.tag == MirTerm_TBranch) {
        __auto_type t = _t253.data.TBranch.t;
__auto_type e = _t253.data.TBranch.e;
        /* pass */
        List_i64_append(out, t);
        /* pass */
        List_i64_append(out, e);
    } else if (1) {
        __auto_type _ = _t253;
        /* pass */
    }
}

__attribute__((hot)) List_ptr* compute_liveness(MirFunction* mf) {
    /* pass */
    long long n = mf->blocks->len;
    /* pass */
    List_ptr* uses = (void*)List_ptr_new();
    /* pass */
    List_ptr* defs = (void*)List_ptr_new();
    /* pass */
    List_ptr* live_in = (void*)List_ptr_new();
    /* pass */
    List_ptr* live_out = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        LiveSet* u = LiveSet_init();
        /* pass */
        LiveSet* d = LiveSet_init();
        /* pass */
        block_use_def(((MirBlock*)List_ptr_get(mf->blocks, i)), u->items, d->items);
        /* pass */
        List_ptr_append(uses, u);
        /* pass */
        List_ptr_append(defs, d);
        /* pass */
        List_ptr_append(live_in, LiveSet_init());
        /* pass */
        List_ptr_append(live_out, LiveSet_init());
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    bool changed = true;
    /* pass */
    while (changed) {
        /* pass */
        changed = false;
        /* pass */
        long long bi = (n - 1LL);
        /* pass */
        while ((bi >= 0LL)) {
            /* pass */
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
            /* pass */
            List_i64* sl = (void*)List_i64_new();
            /* pass */
            block_succs(blk, sl);
            /* pass */
            long long si = 0LL;
            /* pass */
            while ((si < sl->len)) {
                /* pass */
                if (LiveSet_union_in(((LiveSet*)List_ptr_get(live_out, bi)), ((LiveSet*)List_ptr_get(live_in, List_i64_get(sl, si))))) {
                    /* pass */
                    changed = true;
                }
                /* pass */
                si = (si + 1LL);
            }
            /* pass */
            if (LiveSet_union_in(((LiveSet*)List_ptr_get(live_in, bi)), ((LiveSet*)List_ptr_get(uses, bi)))) {
                /* pass */
                changed = true;
            }
            /* pass */
            if (LiveSet_union_except(((LiveSet*)List_ptr_get(live_in, bi)), ((LiveSet*)List_ptr_get(live_out, bi)), ((LiveSet*)List_ptr_get(defs, bi)))) {
                /* pass */
                changed = true;
            }
            /* pass */
            bi = (bi - 1LL);
        }
    }
    /* pass */
    return live_out;
}

__attribute__((hot)) void collect_moves(HirExpr* e, List_TrStr* out) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t254 = (*e);
    if (_t254.tag == HirExpr_EIdent) {
        __auto_type nm = _t254.data.EIdent.name;
__auto_type ismv = _t254.data.EIdent.is_move;
        /* pass */
        if (ismv) {
            /* pass */
            set_add(out, nm);
        }
    } else if (_t254.tag == HirExpr_EBinOp) {
        __auto_type l = _t254.data.EBinOp.left;
__auto_type r = _t254.data.EBinOp.right;
        /* pass */
        collect_moves(l, out);
        /* pass */
        collect_moves(r, out);
    } else if (_t254.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t254.data.EUnaryOp.expr;
        collect_moves(x, out);
    } else if (_t254.tag == HirExpr_ECall) {
        __auto_type callee = _t254.data.ECall.callee;
__auto_type args = _t254.data.ECall.args;
        /* pass */
        collect_moves(callee, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t254.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t254.data.EMethodCall.obj;
__auto_type args = _t254.data.EMethodCall.args;
        /* pass */
        collect_moves(obj, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t254.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t254.data.EPropAccess.obj;
        collect_moves(obj, out);
    } else if (_t254.tag == HirExpr_EIndex) {
        __auto_type obj = _t254.data.EIndex.obj;
__auto_type idx = _t254.data.EIndex._tr_v_index;
        /* pass */
        collect_moves(obj, out);
        /* pass */
        collect_moves(idx, out);
    } else if (_t254.tag == HirExpr_ECast) {
        __auto_type x = _t254.data.ECast.expr;
        collect_moves(x, out);
    } else if (_t254.tag == HirExpr_EAwait) {
        __auto_type x = _t254.data.EAwait.expr;
        collect_moves(x, out);
    } else if (_t254.tag == HirExpr_EIfElse) {
        __auto_type c = _t254.data.EIfElse.cond;
__auto_type t = _t254.data.EIfElse.then_e;
__auto_type f = _t254.data.EIfElse.else_e;
        /* pass */
        collect_moves(c, out);
        /* pass */
        collect_moves(t, out);
        /* pass */
        collect_moves(f, out);
    } else if (1) {
        __auto_type _ = _t254;
        /* pass */
    }
}

__attribute__((hot)) bool is_free_fn(TrStr n) {
    /* pass */
    return ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("dealloc"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("free"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("_tr_c_free"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("_tr_free"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("_tr_str_release"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("_tr_str_free"))) == 0));
}

__attribute__((hot)) TrStr free_target(HirExpr* arg) {
    /* pass */
    if ((((unsigned long long)(arg)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type _t255 = (*arg);
    if (_t255.tag == HirExpr_EIdent) {
        __auto_type n = _t255.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (_t255.tag == HirExpr_ECast) {
        __auto_type x = _t255.data.ECast.expr;
        return free_target(x);
    } else if (1) {
        __auto_type _ = _t255;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) void collect_frees(HirExpr* e, List_TrStr* out) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t256 = (*e);
    if (_t256.tag == HirExpr_ECall) {
        __auto_type callee = _t256.data.ECall.callee;
__auto_type args = _t256.data.ECall.args;
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t257 = (*callee);
            if (_t257.tag == HirExpr_EIdent) {
                __auto_type cn = _t257.data.EIdent.name;
                /* pass */
                if ((is_free_fn(cn) && (args->len > 0LL))) {
                    /* pass */
                    TrStr ft = free_target(((HirExpr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    if ((strcmp(_tr_strz(ft), _tr_strz(_tr_str_lit(""))) != 0)) {
                        /* pass */
                        set_add(out, ft);
                    }
                }
            } else if (1) {
                __auto_type _ = _t257;
                /* pass */
            }
        }
        /* pass */
        collect_frees(callee, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            collect_frees(((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t256.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t256.data.EMethodCall.obj;
__auto_type m = _t256.data.EMethodCall.method;
__auto_type args = _t256.data.EMethodCall.args;
        /* pass */
        if ((strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("free"))) == 0)) {
            /* pass */
            __auto_type _t258 = (*obj);
            if (_t258.tag == HirExpr_EIdent) {
                __auto_type on = _t258.data.EIdent.name;
                set_add(out, on);
            } else if (1) {
                __auto_type _ = _t258;
                /* pass */
            }
        }
        /* pass */
        collect_frees(obj, out);
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < args->len)) {
            /* pass */
            collect_frees(((HirExpr*)List_ptr_get(args, j)), out);
            /* pass */
            j = (j + 1LL);
        }
    } else if (_t256.tag == HirExpr_EBinOp) {
        __auto_type l = _t256.data.EBinOp.left;
__auto_type r = _t256.data.EBinOp.right;
        /* pass */
        collect_frees(l, out);
        /* pass */
        collect_frees(r, out);
    } else if (_t256.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t256.data.EUnaryOp.expr;
        collect_frees(x, out);
    } else if (_t256.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t256.data.EPropAccess.obj;
        collect_frees(obj, out);
    } else if (_t256.tag == HirExpr_EIndex) {
        __auto_type obj = _t256.data.EIndex.obj;
__auto_type idx = _t256.data.EIndex._tr_v_index;
        /* pass */
        collect_frees(obj, out);
        /* pass */
        collect_frees(idx, out);
    } else if (_t256.tag == HirExpr_ECast) {
        __auto_type x = _t256.data.ECast.expr;
        collect_frees(x, out);
    } else if (_t256.tag == HirExpr_EAwait) {
        __auto_type x = _t256.data.EAwait.expr;
        collect_frees(x, out);
    } else if (1) {
        __auto_type _ = _t256;
        /* pass */
    }
}

__attribute__((hot)) void block_gen_own(MirBlock* blk, List_TrStr* out) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < blk->stmts->len)) {
        /* pass */
        __auto_type _t259 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t259.tag == MirStmt_MDeclare) {
            __auto_type p = _t259.data.MDeclare.place;
            set_add(out, p);
        } else if (_t259.tag == MirStmt_MAssign) {
            __auto_type p = _t259.data.MAssign.place;
            set_add(out, p);
        } else if (_t259.tag == MirStmt_MEval) {
            __auto_type _ = _t259.data.MEval.value;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_moves(MirBlock* blk, List_TrStr* out) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < blk->stmts->len)) {
        /* pass */
        __auto_type _t260 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t260.tag == MirStmt_MDeclare) {
            __auto_type val = _t260.data.MDeclare.value;
            /* pass */
            collect_moves(val, out);
            /* pass */
            collect_frees(val, out);
        } else if (_t260.tag == MirStmt_MAssign) {
            __auto_type val = _t260.data.MAssign.value;
            /* pass */
            collect_moves(val, out);
            /* pass */
            collect_frees(val, out);
        } else if (_t260.tag == MirStmt_MEval) {
            __auto_type val = _t260.data.MEval.value;
            /* pass */
            collect_moves(val, out);
            /* pass */
            collect_frees(val, out);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    __auto_type _t261 = (*blk->term);
    if (_t261.tag == MirTerm_TBranch) {
        __auto_type cond = _t261.data.TBranch.cond;
        collect_moves(cond, out);
    } else if (_t261.tag == MirTerm_TReturn) {
        __auto_type val = _t261.data.TReturn.value;
        /* pass */
        collect_moves(val, out);
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t262 = (*val);
            if (_t262.tag == HirExpr_EIdent) {
                __auto_type rnm = _t262.data.EIdent.name;
                set_add(out, rnm);
            } else if (1) {
                __auto_type _ = _t262;
                /* pass */
            }
        }
    } else if (1) {
        __auto_type _ = _t261;
        /* pass */
    }
}

__attribute__((hot)) void preds_of(MirFunction* mf, long long b, List_i64* out) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < mf->blocks->len)) {
        /* pass */
        List_i64* sl = (void*)List_i64_new();
        /* pass */
        block_succs(((MirBlock*)List_ptr_get(mf->blocks, i)), sl);
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < sl->len)) {
            /* pass */
            if ((List_i64_get(sl, j) == b)) {
                /* pass */
                List_i64_append(out, i);
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_ptr* compute_drops(MirFunction* mf, List_ptr* live_out) {
    /* pass */
    long long n = mf->blocks->len;
    /* pass */
    List_ptr* gen = (void*)List_ptr_new();
    /* pass */
    List_ptr* kill = (void*)List_ptr_new();
    /* pass */
    List_ptr* own_out = (void*)List_ptr_new();
    /* pass */
    LiveSet* universe = LiveSet_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        LiveSet* g = LiveSet_init();
        /* pass */
        LiveSet* k = LiveSet_init();
        /* pass */
        block_gen_own(((MirBlock*)List_ptr_get(mf->blocks, i)), g->items);
        /* pass */
        block_moves(((MirBlock*)List_ptr_get(mf->blocks, i)), k->items);
        /* pass */
        List_ptr_append(gen, g);
        /* pass */
        List_ptr_append(kill, k);
        /* pass */
        bool _u = LiveSet_union_in(universe, g);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        List_ptr_append(own_out, LiveSet_clone(universe));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    bool changed = true;
    /* pass */
    while (changed) {
        /* pass */
        changed = false;
        /* pass */
        long long b = 0LL;
        /* pass */
        while ((b < n)) {
            /* pass */
            List_i64* preds = (void*)List_i64_new();
            /* pass */
            preds_of(mf, b, preds);
            /* pass */
            LiveSet* own_in = LiveSet_init();
            /* pass */
            if ((preds->len > 0LL)) {
                /* pass */
                own_in = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, 0LL))));
                /* pass */
                long long pi = 1LL;
                /* pass */
                while ((pi < preds->len)) {
                    /* pass */
                    LiveSet_retain_common(own_in, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, pi))));
                    /* pass */
                    pi = (pi + 1LL);
                }
            }
            /* pass */
            bool _g = LiveSet_union_in(own_in, ((LiveSet*)List_ptr_get(gen, b)));
            /* pass */
            LiveSet* owned_b = LiveSet_minus(own_in, ((LiveSet*)List_ptr_get(kill, b)));
            /* pass */
            LiveSet* new_oo = LiveSet_clone(owned_b);
            /* pass */
            LiveSet_retain_common(new_oo, ((LiveSet*)List_ptr_get(live_out, b)));
            /* pass */
            if ((!LiveSet_equals(new_oo, ((LiveSet*)List_ptr_get(own_out, b))))) {
                /* pass */
                changed = true;
                /* pass */
                LiveSet_set_to(((LiveSet*)List_ptr_get(own_out, b)), new_oo);
            }
            /* pass */
            b = (b + 1LL);
        }
    }
    /* pass */
    List_ptr* drops = (void*)List_ptr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        List_i64* preds2 = (void*)List_i64_new();
        /* pass */
        preds_of(mf, i, preds2);
        /* pass */
        LiveSet* own_in2 = LiveSet_init();
        /* pass */
        if ((preds2->len > 0LL)) {
            /* pass */
            own_in2 = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, 0LL))));
            /* pass */
            long long pj = 1LL;
            /* pass */
            while ((pj < preds2->len)) {
                /* pass */
                LiveSet_retain_common(own_in2, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, pj))));
                /* pass */
                pj = (pj + 1LL);
            }
        }
        /* pass */
        bool _g2 = LiveSet_union_in(own_in2, ((LiveSet*)List_ptr_get(gen, i)));
        /* pass */
        LiveSet* owned_i = LiveSet_minus(own_in2, ((LiveSet*)List_ptr_get(kill, i)));
        /* pass */
        LiveSet* dead_i = LiveSet_minus(owned_i, ((LiveSet*)List_ptr_get(live_out, i)));
        /* pass */
        List_ptr_append(drops, LiveSet_minus(dead_i, mf->unsafe_pinned));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return drops;
}

__attribute__((hot)) long long last_use_in_block(MirBlock* blk, TrStr name) {
    /* pass */
    long long last = (-1LL);
    /* pass */
    long long si = 0LL;
    /* pass */
    while ((si < blk->stmts->len)) {
        /* pass */
        List_TrStr* tmp = (void*)List_TrStr_new();
        /* pass */
        __auto_type _t263 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t263.tag == MirStmt_MDeclare) {
            __auto_type val = _t263.data.MDeclare.value;
            collect_uses(val, tmp);
        } else if (_t263.tag == MirStmt_MAssign) {
            __auto_type val = _t263.data.MAssign.value;
            collect_uses(val, tmp);
        } else if (_t263.tag == MirStmt_MEval) {
            __auto_type val = _t263.data.MEval.value;
            collect_uses(val, tmp);
        }
        /* pass */
        if (set_contains(tmp, name)) {
            /* pass */
            last = si;
        }
        /* pass */
        si = (si + 1LL);
    }
    /* pass */
    List_TrStr* tt = (void*)List_TrStr_new();
    /* pass */
    __auto_type _t264 = (*blk->term);
    if (_t264.tag == MirTerm_TBranch) {
        __auto_type cond = _t264.data.TBranch.cond;
        collect_uses(cond, tt);
    } else if (_t264.tag == MirTerm_TReturn) {
        __auto_type val = _t264.data.TReturn.value;
        collect_uses(val, tt);
    } else if (1) {
        __auto_type _ = _t264;
        /* pass */
    }
    /* pass */
    if (set_contains(tt, name)) {
        /* pass */
        last = blk->stmts->len;
    }
    /* pass */
    return last;
}

__attribute__((hot)) List_bool* compute_borrow_outlives(MirFunction* mf, List_ptr* live_out) {
    /* pass */
    List_bool* out = (void*)List_bool_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < mf->borrows->len)) {
        /* pass */
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
        /* pass */
        bool proven = true;
        /* pass */
        long long bi = 0LL;
        /* pass */
        while ((bi < mf->blocks->len)) {
            /* pass */
            LiveSet* lo = ((LiveSet*)List_ptr_get(live_out, bi));
            /* pass */
            if ((LiveSet_has(lo, edge->borrower) && (!LiveSet_has(lo, edge->source)))) {
                /* pass */
                proven = false;
            }
            /* pass */
            bi = (bi + 1LL);
        }
        /* pass */
        long long db = edge->decl_block;
        /* pass */
        if (((db >= 0LL) && (db < mf->blocks->len))) {
            /* pass */
            if ((!LiveSet_has(((LiveSet*)List_ptr_get(live_out, db)), edge->source))) {
                /* pass */
                MirBlock* dblk = ((MirBlock*)List_ptr_get(mf->blocks, db));
                /* pass */
                long long src_last = last_use_in_block(dblk, edge->source);
                /* pass */
                long long bor_last = last_use_in_block(dblk, edge->borrower);
                /* pass */
                if (LiveSet_has(((LiveSet*)List_ptr_get(live_out, db)), edge->borrower)) {
                    /* pass */
                    bor_last = (dblk->stmts->len + 1LL);
                }
                /* pass */
                if ((bor_last > src_last)) {
                    /* pass */
                    proven = false;
                }
            }
        }
        /* pass */
        List_bool_append(out, proven);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return out;
}

__attribute__((hot)) bool is_if_body(MirFunction* mf, HirBlock* hb) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < mf->if_bodies->len)) {
        /* pass */
        if ((((unsigned long long)(((HirBlock*)List_ptr_get(mf->if_bodies, i)))) == ((unsigned long long)(hb)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) DropSite* site_for(List_ptr* out, HirBlock* hb) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < out->len)) {
        /* pass */
        if ((((unsigned long long)(((DropSite*)List_ptr_get(out, i))->hir_block)) == ((unsigned long long)(hb)))) {
            /* pass */
            return ((DropSite*)List_ptr_get(out, i));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    DropSite* s = ((DropSite*)_tr_checked_alloc(sizeof(DropSite)));
    /* pass */
    s->hir_block = hb;
    /* pass */
    s->places = (void*)List_TrStr_new();
    /* pass */
    List_ptr_append(out, s);
    /* pass */
    return s;
}

__attribute__((hot)) List_TrStr* mir_proven_borrows(HirFunction* hf) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    MirFunction* mf = lower_function(hf);
    /* pass */
    if ((!mf->complete)) {
        /* pass */
        TrStr _ad_f_t265 = mf->name;
        _tr_str_release(_ad_f_t265);
        return out;
    }
    /* pass */
    if ((mf->borrows->len == 0LL)) {
        /* pass */
        TrStr _ad_f_t266 = mf->name;
        _tr_str_release(_ad_f_t266);
        return out;
    }
    /* pass */
    List_ptr* live = compute_liveness(mf);
    /* pass */
    List_bool* outl = compute_borrow_outlives(mf, live);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < mf->borrows->len)) {
        /* pass */
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
        /* pass */
        bool proven = List_bool_get(outl, i);
        /* pass */
        if ((proven && edge->via_collection)) {
            /* pass */
            long long bn = 0LL;
            /* pass */
            while ((bn < mf->blocks->len)) {
                /* pass */
                MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
                /* pass */
                LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
                /* pass */
                long long si = 0LL;
                /* pass */
                while ((si < blk->stmts->len)) {
                    /* pass */
                    bool is_creation = false;
                    /* pass */
                    __auto_type _t267 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                    if (_t267.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t267.data.MDeclare.place;
                        /* pass */
                        if ((strcmp(_tr_strz(dn), _tr_strz(edge->borrower)) == 0)) {
                            /* pass */
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t267;
                        /* pass */
                    }
                    /* pass */
                    if ((!is_creation)) {
                        /* pass */
                        List_TrStr* uses = (void*)List_TrStr_new();
                        /* pass */
                        __auto_type _t268 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                        if (_t268.tag == MirStmt_MDeclare) {
                            __auto_type val = _t268.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t268.tag == MirStmt_MAssign) {
                            __auto_type ap = _t268.data.MAssign.place;
__auto_type val = _t268.data.MAssign.value;
                            /* pass */
                            collect_uses(val, uses);
                            /* pass */
                            if ((strcmp(_tr_strz(ap), _tr_strz(edge->source)) == 0)) {
                                /* pass */
                                set_add(uses, edge->source);
                            }
                        } else if (_t268.tag == MirStmt_MEval) {
                            __auto_type val = _t268.data.MEval.value;
                            collect_uses(val, uses);
                        }
                        /* pass */
                        if ((set_contains(uses, edge->source) && borrower_live_after(blk, si, edge->borrower, lo))) {
                            /* pass */
                            proven = false;
                        }
                    }
                    /* pass */
                    si = (si + 1LL);
                }
                /* pass */
                bn = (bn + 1LL);
            }
        }
        /* pass */
        if (proven) {
            /* pass */
            List_TrStr_append(out, edge->borrower);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_bool_free(outl);
    return out;
}

__attribute__((hot)) bool borrower_live_after(MirBlock* blk, long long after_idx, TrStr name, LiveSet* lo) {
    /* pass */
    long long si = (after_idx + 1LL);
    /* pass */
    while ((si < blk->stmts->len)) {
        /* pass */
        List_TrStr* uses = (void*)List_TrStr_new();
        /* pass */
        __auto_type _t269 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t269.tag == MirStmt_MDeclare) {
            __auto_type val = _t269.data.MDeclare.value;
            collect_uses(val, uses);
        } else if (_t269.tag == MirStmt_MAssign) {
            __auto_type val = _t269.data.MAssign.value;
            collect_uses(val, uses);
        } else if (_t269.tag == MirStmt_MEval) {
            __auto_type val = _t269.data.MEval.value;
            collect_uses(val, uses);
        }
        /* pass */
        if (set_contains(uses, name)) {
            /* pass */
            return true;
        }
        /* pass */
        si = (si + 1LL);
    }
    /* pass */
    List_TrStr* tu = (void*)List_TrStr_new();
    /* pass */
    __auto_type _t270 = (*blk->term);
    if (_t270.tag == MirTerm_TBranch) {
        __auto_type cond = _t270.data.TBranch.cond;
        collect_uses(cond, tu);
    } else if (_t270.tag == MirTerm_TReturn) {
        __auto_type val = _t270.data.TReturn.value;
        collect_uses(val, tu);
    } else if (1) {
        __auto_type _ = _t270;
        /* pass */
    }
    /* pass */
    if (set_contains(tu, name)) {
        /* pass */
        return true;
    }
    /* pass */
    return LiveSet_has(lo, name);
}

__attribute__((hot)) List_TrStr* mir_borrow_conflicts(HirFunction* hf, TrMap* mutating_methods) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    MirFunction* mf = lower_function(hf);
    /* pass */
    if ((!mf->complete)) {
        /* pass */
        TrStr _ad_f_t271 = mf->name;
        _tr_str_release(_ad_f_t271);
        return out;
    }
    /* pass */
    if ((mf->borrows->len == 0LL)) {
        /* pass */
        TrStr _ad_f_t272 = mf->name;
        _tr_str_release(_ad_f_t272);
        return out;
    }
    /* pass */
    List_ptr* live = compute_liveness(mf);
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < mf->borrows->len)) {
        /* pass */
        BorrowEdge* be = ((BorrowEdge*)List_ptr_get(mf->borrows, k));
        /* pass */
        long long bn = 0LL;
        /* pass */
        while ((bn < mf->blocks->len)) {
            /* pass */
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
            /* pass */
            LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
            /* pass */
            long long si = 0LL;
            /* pass */
            while ((si < blk->stmts->len)) {
                /* pass */
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
                /* pass */
                TrStr invalid_desc = _tr_str_lit("");
                /* pass */
                HirExpr* sval = ((HirExpr*)(0LL));
                /* pass */
                __auto_type _t273 = (*stmt);
                if (_t273.tag == MirStmt_MAssign) {
                    __auto_type wp = _t273.data.MAssign.place;
__auto_type wv = _t273.data.MAssign.value;
                    /* pass */
                    sval = wv;
                    /* pass */
                    if ((strcmp(_tr_strz(wp), _tr_strz(be->source)) == 0)) {
                        /* pass */
                        TrStr _strtmp_t274 = _tr_str_lit("assign to");
                        _tr_str_release(invalid_desc);
                        invalid_desc = _strtmp_t274;
                    }
                } else if (_t273.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t273.data.MDeclare.value;
                    sval = dv;
                } else if (_t273.tag == MirStmt_MEval) {
                    __auto_type ev = _t273.data.MEval.value;
                    sval = ev;
                }
                /* pass */
                if (((strcmp(_tr_strz(invalid_desc), _tr_strz(_tr_str_lit(""))) == 0) && _is_mutating_call_on(sval, be->source, mutating_methods))) {
                    /* pass */
                    TrStr _strtmp_t275 = _tr_str_lit("call a mutating method on");
                    _tr_str_release(invalid_desc);
                    invalid_desc = _strtmp_t275;
                }
                /* pass */
                if ((strcmp(_tr_strz(invalid_desc), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    if (borrower_live_after(blk, si, be->borrower, lo)) {
                        /* pass */
                        ({ TrStr _at_t276 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("cannot ")), _tr_strz(invalid_desc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(be->source)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' while it is borrowed by '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(be->borrower)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' — the borrow is still live"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(out, _at_t276); _tr_str_release(_at_t276); });
                    }
                } else if (be->is_exclusive) {
                    /* pass */
                    bool is_creation = false;
                    /* pass */
                    __auto_type _t277 = (*stmt);
                    if (_t277.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t277.data.MDeclare.place;
                        /* pass */
                        if ((strcmp(_tr_strz(dn), _tr_strz(be->borrower)) == 0)) {
                            /* pass */
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t277;
                        /* pass */
                    }
                    /* pass */
                    if ((!is_creation)) {
                        /* pass */
                        List_TrStr* uses = (void*)List_TrStr_new();
                        /* pass */
                        __auto_type _t278 = (*stmt);
                        if (_t278.tag == MirStmt_MDeclare) {
                            __auto_type val = _t278.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t278.tag == MirStmt_MAssign) {
                            __auto_type val = _t278.data.MAssign.value;
                            collect_uses(val, uses);
                        } else if (_t278.tag == MirStmt_MEval) {
                            __auto_type val = _t278.data.MEval.value;
                            collect_uses(val, uses);
                        }
                        /* pass */
                        if ((set_contains(uses, be->source) && borrower_live_after(blk, si, be->borrower, lo))) {
                            /* pass */
                            ({ TrStr _at_t279 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("cannot use '")), _tr_strz(be->source))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' while it is exclusively (mut) borrowed by '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(be->borrower)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(out, _at_t279); _tr_str_release(_at_t279); });
                        }
                    }
                }
                /* pass */
                si = (si + 1LL);
                _tr_str_release(invalid_desc);
            }
            /* pass */
            bn = (bn + 1LL);
        }
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    if ((mf->borrows->len < 2LL)) {
        /* pass */
        return out;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < mf->borrows->len)) {
        /* pass */
        BorrowEdge* bi = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
        /* pass */
        if (((bi->decl_block >= 0LL) && (bi->decl_block < mf->blocks->len))) {
            /* pass */
            long long j = (i + 1LL);
            /* pass */
            while ((j < mf->borrows->len)) {
                /* pass */
                BorrowEdge* bj = ((BorrowEdge*)List_ptr_get(mf->borrows, j));
                /* pass */
                if ((((bj->decl_block == bi->decl_block) && (strcmp(_tr_strz(bi->source), _tr_strz(bj->source)) == 0)) && (bi->is_exclusive || bj->is_exclusive))) {
                    /* pass */
                    MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi->decl_block));
                    /* pass */
                    long long pi = 0LL;
                    /* pass */
                    long long pj = 0LL;
                    /* pass */
                    long long bk = 0LL;
                    /* pass */
                    while ((bk < blk->stmts->len)) {
                        /* pass */
                        __auto_type _t280 = (*((MirStmt*)List_ptr_get(blk->stmts, bk)));
                        if (_t280.tag == MirStmt_MDeclare) {
                            __auto_type dn = _t280.data.MDeclare.place;
                            /* pass */
                            if ((strcmp(_tr_strz(dn), _tr_strz(bi->borrower)) == 0)) {
                                /* pass */
                                pi = bk;
                            }
                            /* pass */
                            if ((strcmp(_tr_strz(dn), _tr_strz(bj->borrower)) == 0)) {
                                /* pass */
                                pj = bk;
                            }
                        } else if (1) {
                            __auto_type _ = _t280;
                            /* pass */
                        }
                        /* pass */
                        bk = (bk + 1LL);
                    }
                    /* pass */
                    long long ui = last_use_in_block(blk, bi->borrower);
                    /* pass */
                    long long uj = last_use_in_block(blk, bj->borrower);
                    /* pass */
                    if ((ui < pi)) {
                        /* pass */
                        ui = pi;
                    }
                    /* pass */
                    if ((uj < pj)) {
                        /* pass */
                        uj = pj;
                    }
                    /* pass */
                    if (((pi <= uj) && (pj <= ui))) {
                        /* pass */
                        TrStr kind = _tr_str_lit("a mutable borrow");
                        /* pass */
                        if ((!(bi->is_exclusive && bj->is_exclusive))) {
                            /* pass */
                            TrStr _strtmp_t281 = _tr_str_lit("a mutable and a shared borrow");
                            _tr_str_release(kind);
                            kind = _strtmp_t281;
                        }
                        /* pass */
                        ({ TrStr _at_t282 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("'")), _tr_strz(bi->borrower))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' and '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bj->borrower)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' are "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(kind)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" of '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bi->source)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' that are live at the same time"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(out, _at_t282); _tr_str_release(_at_t282); });
                    }
                }
                /* pass */
                j = (j + 1LL);
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return out;
}

__attribute__((hot)) List_TrStr* mir_shared_ref_param_violations(HirFunction* hf, TrMap* mutating_methods) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* shps = (void*)List_TrStr_new();
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < hf->params->len)) {
        /* pass */
        AstType* pty = ((HirParam*)List_ptr_get(hf->params, pi))->ty;
        /* pass */
        if ((pty->is_borrow && (!pty->is_mut_borrow))) {
            /* pass */
            List_TrStr_append(shps, ((HirParam*)List_ptr_get(hf->params, pi))->name);
        }
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    if ((shps->len == 0LL)) {
        /* pass */
        List_TrStr_free(shps);
        return out;
    }
    /* pass */
    MirFunction* mf = lower_function(hf);
    /* pass */
    long long sk = 0LL;
    /* pass */
    while ((sk < shps->len)) {
        /* pass */
        TrStr pn = List_TrStr_get(shps, sk);
        /* pass */
        long long bn = 0LL;
        /* pass */
        while ((bn < mf->blocks->len)) {
            /* pass */
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
            /* pass */
            long long si = 0LL;
            /* pass */
            while ((si < blk->stmts->len)) {
                /* pass */
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
                /* pass */
                HirExpr* sval = ((HirExpr*)(0LL));
                /* pass */
                TrStr desc = _tr_str_lit("");
                /* pass */
                __auto_type _t283 = (*stmt);
                if (_t283.tag == MirStmt_MAssign) {
                    __auto_type wp = _t283.data.MAssign.place;
__auto_type wv = _t283.data.MAssign.value;
                    /* pass */
                    sval = wv;
                    /* pass */
                    if ((strcmp(_tr_strz(wp), _tr_strz(pn)) == 0)) {
                        /* pass */
                        TrStr _strtmp_t284 = _tr_str_lit("reassign");
                        _tr_str_release(desc);
                        desc = _strtmp_t284;
                    }
                } else if (_t283.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t283.data.MDeclare.value;
                    sval = dv;
                } else if (_t283.tag == MirStmt_MEval) {
                    __auto_type ev = _t283.data.MEval.value;
                    sval = ev;
                }
                /* pass */
                if (((strcmp(_tr_strz(desc), _tr_strz(_tr_str_lit(""))) == 0) && _is_mutating_call_on(sval, pn, mutating_methods))) {
                    /* pass */
                    TrStr _strtmp_t285 = _tr_str_lit("call a mutating method on");
                    _tr_str_release(desc);
                    desc = _strtmp_t285;
                }
                /* pass */
                if ((strcmp(_tr_strz(desc), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    ({ TrStr _at_t286 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("cannot ")), _tr_strz(desc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' — it is a SHARED `ref` parameter (read-only)"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(out, _at_t286); _tr_str_release(_at_t286); });
                }
                /* pass */
                si = (si + 1LL);
                _tr_str_release(desc);
            }
            /* pass */
            bn = (bn + 1LL);
        }
        /* pass */
        sk = (sk + 1LL);
        _tr_str_release(pn);
    }
    /* pass */
    List_TrStr_free(shps);
    TrStr _ad_f_t287 = mf->name;
    _tr_str_release(_ad_f_t287);
    return out;
}

__attribute__((hot)) List_ptr* mir_if_drop_plan(HirFunction* hf) {
    /* pass */
    List_ptr* out = (void*)List_ptr_new();
    /* pass */
    MirFunction* mf = lower_function(hf);
    /* pass */
    if ((!mf->complete)) {
        /* pass */
        TrStr _ad_f_t288 = mf->name;
        _tr_str_release(_ad_f_t288);
        return out;
    }
    /* pass */
    if ((mf->if_bodies->len == 0LL)) {
        /* pass */
        TrStr _ad_f_t289 = mf->name;
        _tr_str_release(_ad_f_t289);
        return out;
    }
    /* pass */
    List_ptr* live = compute_liveness(mf);
    /* pass */
    List_ptr* drops = compute_drops(mf, live);
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < mf->blocks->len)) {
        /* pass */
        MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
        /* pass */
        if (((((unsigned long long)(blk->hir_block)) != ((unsigned long long)(0LL))) && is_if_body(mf, blk->hir_block))) {
            /* pass */
            LiveSet* dd = ((LiveSet*)List_ptr_get(drops, blk->id));
            /* pass */
            long long pi = 0LL;
            /* pass */
            while ((pi < dd->items->len)) {
                /* pass */
                DropSite* site = site_for(out, blk->hir_block);
                /* pass */
                ({ TrStr _at_t290 = (List_TrStr_get(dd->items, pi)); set_add(site->places, _at_t290); _tr_str_release(_at_t290); });
                /* pass */
                pi = (pi + 1LL);
            }
        }
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    return out;
}

__attribute__((hot)) TrStr set_str(List_TrStr* v) {
    /* pass */
    TrStr s = _tr_str_lit("{");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < v->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t291 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t291;
        }
        /* pass */
        TrStr _strtmp_t292 = ({ TrStr _cr = (List_TrStr_get(v, i)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t292;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}")));
}

__attribute__((hot)) TrStr term_str(MirTerm* t) {
    /* pass */
    __auto_type _t293 = (*t);
    if (_t293.tag == MirTerm_TGoto) {
        __auto_type n = _t293.data.TGoto.target;
        return ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("goto bb")), _cr.data); _tr_str_release(_cr); _cres; });
    } else if (_t293.tag == MirTerm_TBranch) {
        __auto_type tt = _t293.data.TBranch.t;
__auto_type ee = _t293.data.TBranch.e;
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(tt)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("branch <cond> ? bb")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" : bb"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ee)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    } else if (_t293.tag == MirTerm_TReturn) {
        __auto_type _ = _t293.data.TReturn.value;
        return _tr_str_lit("return <expr>");
    } else if (_t293.tag == MirTerm_TReturnVoid) {
        return _tr_str_lit("return");
    } else if (_t293.tag == MirTerm_TUnset) {
        return _tr_str_lit("<unset>");
    }
    /* pass */
    return _tr_str_lit("<?>");
}

__attribute__((hot)) TrStr stmt_str(MirStmt* s) {
    /* pass */
    __auto_type _t294 = (*s);
    if (_t294.tag == MirStmt_MDeclare) {
        __auto_type p = _t294.data.MDeclare.place;
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("let ")), _tr_strz(p))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = <expr>"))); _tr_str_release(_cl); _cres; });
    } else if (_t294.tag == MirStmt_MAssign) {
        __auto_type p = _t294.data.MAssign.place;
        return _tr_strx_concat(_tr_strz(p), _tr_strz(_tr_str_lit(" = <expr>")));
    } else if (_t294.tag == MirStmt_MEval) {
        __auto_type _ = _t294.data.MEval.value;
        return _tr_str_lit("eval <expr>");
    }
    /* pass */
    return _tr_str_lit("<?>");
}

__attribute__((hot)) TrStr dump_mir(MirProgram* mp) {
    /* pass */
    TrStr out = _tr_str_lit("");
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < mp->functions->len)) {
        /* pass */
        MirFunction* f = ((MirFunction*)List_ptr_get(mp->functions, fi));
        /* pass */
        TrStr tag = _tr_str_lit(" [complete]");
        /* pass */
        if ((!f->complete)) {
            /* pass */
            TrStr _strtmp_t295 = _tr_str_lit(" [INCOMPLETE - falls back to HIR]");
            _tr_str_release(tag);
            tag = _strtmp_t295;
        }
        /* pass */
        TrStr _strtmp_t296 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("fn ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(f->params->len)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" params), "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(f->blocks->len)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" blocks"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(out);
        out = _strtmp_t296;
        /* pass */
        List_ptr* live = compute_liveness(f);
        /* pass */
        List_ptr* drops = compute_drops(f, live);
        /* pass */
        if ((f->borrows->len > 0LL)) {
            /* pass */
            List_bool* outl = compute_borrow_outlives(f, live);
            /* pass */
            long long ei = 0LL;
            /* pass */
            while ((ei < f->borrows->len)) {
                /* pass */
                BorrowEdge* ed = ((BorrowEdge*)List_ptr_get(f->borrows, ei));
                /* pass */
                TrStr verdict = _tr_str_lit("NEEDS-EXTENSION");
                /* pass */
                if (List_bool_get(outl, ei)) {
                    /* pass */
                    TrStr _strtmp_t297 = _tr_str_lit("PROVEN");
                    _tr_str_release(verdict);
                    verdict = _strtmp_t297;
                }
                /* pass */
                TrStr _strtmp_t298 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("  ; borrow ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ed->borrower)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" <- "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ed->source)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" : "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(verdict)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(out);
                out = _strtmp_t298;
                /* pass */
                ei = (ei + 1LL);
                _tr_str_release(verdict);
            }
        }
        /* pass */
        long long bi = 0LL;
        /* pass */
        while ((bi < f->blocks->len)) {
            /* pass */
            MirBlock* blk = ((MirBlock*)List_ptr_get(f->blocks, bi));
            /* pass */
            TrStr _strtmp_t299 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("  bb")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t299;
            /* pass */
            long long si = 0LL;
            /* pass */
            while ((si < blk->stmts->len)) {
                /* pass */
                TrStr _strtmp_t300 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ")))); TrStr _cr = (stmt_str(((MirStmt*)List_ptr_get(blk->stmts, si)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(out);
                out = _strtmp_t300;
                /* pass */
                si = (si + 1LL);
            }
            /* pass */
            TrStr _strtmp_t301 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ")))); TrStr _cr = (term_str(blk->term)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t301;
            /* pass */
            TrStr _strtmp_t302 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ; live_out ")))); TrStr _cr = (set_str(((LiveSet*)List_ptr_get(live, blk->id))->items)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t302;
            /* pass */
            TrStr _strtmp_t303 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ; drop ")))); TrStr _cr = (set_str(((LiveSet*)List_ptr_get(drops, blk->id))->items)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t303;
            /* pass */
            bi = (bi + 1LL);
        }
        /* pass */
        fi = (fi + 1LL);
        _tr_str_release(tag);
    }
    /* pass */
    return out;
}

