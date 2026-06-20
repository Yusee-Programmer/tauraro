#include "tauraro_types.h"

void lower_stmts(MirBuilder* b, HirBlock* hb);
bool set_contains(List_TrStr* v, TrStr s);
bool set_add(List_TrStr* v, TrStr s);
void collect_uses(HirExpr* e, List_TrStr* out);
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
bool is_if_body(MirFunction* mf, HirBlock* hb);
DropSite* site_for(List_ptr* out, HirBlock* hb);
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
    return b;
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
    __auto_type _t202 = (*((MirBlock*)List_ptr_get(self->blocks, self->cur))->term);
    if (_t202.tag == MirTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t202;
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
        if (({ TrStr _at_t203 = (List_TrStr_get(other->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t203)); _tr_str_release(_at_t203); _wr; })) {
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
        if (({ TrStr _at_t204 = (List_TrStr_get(src->items, i)); __auto_type _wr = ((!LiveSet_has(block, _at_t204))); _tr_str_release(_at_t204); _wr; })) {
            /* pass */
            if (({ TrStr _at_t205 = (List_TrStr_get(src->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t205)); _tr_str_release(_at_t205); _wr; })) {
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
        ({ TrStr _at_t206 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _at_t206); _tr_str_release(_at_t206); });
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
        if (({ TrStr _at_t207 = (List_TrStr_get(self->items, i)); __auto_type _wr = (LiveSet_has(other, _at_t207)); _tr_str_release(_at_t207); _wr; })) {
            /* pass */
            ({ TrStr _at_t208 = (List_TrStr_get(self->items, i)); List_TrStr_append(keep, _at_t208); _tr_str_release(_at_t208); });
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
        if (({ TrStr _at_t209 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t209))); _tr_str_release(_at_t209); _wr; })) {
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
        ({ TrStr _at_t210 = (List_TrStr_get(other->items, i)); List_TrStr_append(fresh, _at_t210); _tr_str_release(_at_t210); });
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
        if (({ TrStr _at_t211 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t211))); _tr_str_release(_at_t211); _wr; })) {
            /* pass */
            ({ TrStr _at_t212 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _at_t212); _tr_str_release(_at_t212); });
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return r;
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
        __auto_type _t213 = (*sp);
        if (_t213.tag == HirStmt_SLet) {
            __auto_type nm = _t213.data.SLet.name;
__auto_type val = _t213.data.SLet.val;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MDeclare(nm, val));
        } else if (_t213.tag == HirStmt_SAssign) {
            __auto_type tgt = _t213.data.SAssign.target;
__auto_type val = _t213.data.SAssign.val;
            /* pass */
            __auto_type _t214 = (*tgt);
            if (_t214.tag == HirExpr_EIdent) {
                __auto_type tn = _t214.data.EIdent.name;
                MirBuilder_push_stmt(b, MirStmt_ctor_MAssign(tn, val));
            } else if (1) {
                __auto_type _ = _t214;
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(val));
            }
        } else if (_t213.tag == HirStmt_SExpr) {
            __auto_type e = _t213.data.SExpr.expr;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(e));
        } else if (_t213.tag == HirStmt_SReturn) {
            __auto_type v = _t213.data.SReturn.val;
            /* pass */
            MirBuilder_set_term(b, MirTerm_ctor_TReturn(v));
        } else if (_t213.tag == HirStmt_SIf) {
            __auto_type cond = _t213.data.SIf.cond;
__auto_type then_b = _t213.data.SIf.then_b;
__auto_type else_b = _t213.data.SIf.else_b;
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
        } else if (_t213.tag == HirStmt_SWhile) {
            __auto_type cond = _t213.data.SWhile.cond;
__auto_type body = _t213.data.SWhile.body;
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
            lower_stmts(b, body);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
            }
            /* pass */
            b->cur = exit_b;
        } else if (_t213.tag == HirStmt_SFor) {
            __auto_type fiter = _t213.data.SFor.iter;
__auto_type fbody = _t213.data.SFor.body;
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
            lower_stmts(b, fbody);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
            }
            /* pass */
            b->cur = for_exit;
        } else if (_t213.tag == HirStmt_SForUnpack) {
            __auto_type fuiter = _t213.data.SForUnpack.iter;
__auto_type fubody = _t213.data.SForUnpack.body;
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
            lower_stmts(b, fubody);
            /* pass */
            if ((!MirBuilder_terminated(b))) {
                /* pass */
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
            }
            /* pass */
            b->cur = fu_exit;
        } else if (_t213.tag == HirStmt_STry) {
            __auto_type try_body = _t213.data.STry.try_body;
__auto_type catches = _t213.data.STry.catches;
__auto_type finally_b = _t213.data.STry.finally_b;
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
        } else if (_t213.tag == HirStmt_SMatch) {
            __auto_type msubj = _t213.data.SMatch.expr;
__auto_type marms = _t213.data.SMatch.arms;
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
        } else if (_t213.tag == HirStmt_SAssert) {
            __auto_type acond = _t213.data.SAssert.cond;
__auto_type amsg = _t213.data.SAssert.msg;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(acond));
            /* pass */
            if ((((unsigned long long)(amsg)) != ((unsigned long long)(0LL)))) {
                /* pass */
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(amsg));
            }
        } else if (_t213.tag == HirStmt_SRaise) {
            __auto_type rval = _t213.data.SRaise.val;
            /* pass */
            if ((((unsigned long long)(rval)) != ((unsigned long long)(0LL)))) {
                /* pass */
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(rval));
            }
        } else if (_t213.tag == HirStmt_SWith) {
            __auto_type witems = _t213.data.SWith.items;
__auto_type wbody = _t213.data.SWith.body;
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
        } else if (_t213.tag == HirStmt_SMultiLet) {
            __auto_type mlval = _t213.data.SMultiLet.val;
            /* pass */
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(mlval));
        } else if (_t213.tag == HirStmt_SLineMarker) {
            __auto_type _ = _t213.data.SLineMarker.n;
            /* pass */
            /* pass */
        } else if (_t213.tag == HirStmt_SAutoDrop) {
            /* pass */
            /* pass */
        } else if (1) {
            __auto_type _ = _t213;
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
    mf->blocks = b->blocks;
    /* pass */
    mf->complete = b->complete;
    /* pass */
    mf->if_bodies = b->if_bodies;
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
    __auto_type _t215 = (*e);
    if (_t215.tag == HirExpr_EIdent) {
        __auto_type nm = _t215.data.EIdent.name;
        set_add(out, nm);
    } else if (_t215.tag == HirExpr_EBinOp) {
        __auto_type l = _t215.data.EBinOp.left;
__auto_type r = _t215.data.EBinOp.right;
        /* pass */
        collect_uses(l, out);
        /* pass */
        collect_uses(r, out);
    } else if (_t215.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t215.data.EUnaryOp.expr;
        collect_uses(x, out);
    } else if (_t215.tag == HirExpr_ECall) {
        __auto_type callee = _t215.data.ECall.callee;
__auto_type args = _t215.data.ECall.args;
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
    } else if (_t215.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t215.data.EMethodCall.obj;
__auto_type args = _t215.data.EMethodCall.args;
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
    } else if (_t215.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t215.data.EPropAccess.obj;
        collect_uses(obj, out);
    } else if (_t215.tag == HirExpr_EIndex) {
        __auto_type obj = _t215.data.EIndex.obj;
__auto_type idx = _t215.data.EIndex._tr_v_index;
        /* pass */
        collect_uses(obj, out);
        /* pass */
        collect_uses(idx, out);
    } else if (_t215.tag == HirExpr_ECast) {
        __auto_type x = _t215.data.ECast.expr;
        collect_uses(x, out);
    } else if (_t215.tag == HirExpr_EAwait) {
        __auto_type x = _t215.data.EAwait.expr;
        collect_uses(x, out);
    } else if (_t215.tag == HirExpr_EIfElse) {
        __auto_type c = _t215.data.EIfElse.cond;
__auto_type t = _t215.data.EIfElse.then_e;
__auto_type f = _t215.data.EIfElse.else_e;
        /* pass */
        collect_uses(c, out);
        /* pass */
        collect_uses(t, out);
        /* pass */
        collect_uses(f, out);
    } else if (1) {
        __auto_type _ = _t215;
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
        if (({ TrStr _at_t216 = (List_TrStr_get(tmp, i)); __auto_type _wr = ((!set_contains(kill, _at_t216))); _tr_str_release(_at_t216); _wr; })) {
            /* pass */
            ({ TrStr _at_t217 = (List_TrStr_get(tmp, i)); set_add(gen, _at_t217); _tr_str_release(_at_t217); });
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
        __auto_type _t218 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t218.tag == MirStmt_MDeclare) {
            __auto_type p = _t218.data.MDeclare.place;
__auto_type val = _t218.data.MDeclare.value;
            /* pass */
            add_exposed(val, gen, kill);
            /* pass */
            set_add(kill, p);
        } else if (_t218.tag == MirStmt_MAssign) {
            __auto_type p = _t218.data.MAssign.place;
__auto_type val = _t218.data.MAssign.value;
            /* pass */
            add_exposed(val, gen, kill);
            /* pass */
            set_add(kill, p);
        } else if (_t218.tag == MirStmt_MEval) {
            __auto_type val = _t218.data.MEval.value;
            add_exposed(val, gen, kill);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    __auto_type _t219 = (*blk->term);
    if (_t219.tag == MirTerm_TBranch) {
        __auto_type cond = _t219.data.TBranch.cond;
        add_exposed(cond, gen, kill);
    } else if (_t219.tag == MirTerm_TReturn) {
        __auto_type val = _t219.data.TReturn.value;
        add_exposed(val, gen, kill);
    } else if (1) {
        __auto_type _ = _t219;
        /* pass */
    }
}

__attribute__((hot)) void block_succs(MirBlock* blk, List_i64* out) {
    /* pass */
    __auto_type _t220 = (*blk->term);
    if (_t220.tag == MirTerm_TGoto) {
        __auto_type n = _t220.data.TGoto.target;
        List_i64_append(out, n);
    } else if (_t220.tag == MirTerm_TBranch) {
        __auto_type t = _t220.data.TBranch.t;
__auto_type e = _t220.data.TBranch.e;
        /* pass */
        List_i64_append(out, t);
        /* pass */
        List_i64_append(out, e);
    } else if (1) {
        __auto_type _ = _t220;
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
    __auto_type _t221 = (*e);
    if (_t221.tag == HirExpr_EIdent) {
        __auto_type nm = _t221.data.EIdent.name;
__auto_type ismv = _t221.data.EIdent.is_move;
        /* pass */
        if (ismv) {
            /* pass */
            set_add(out, nm);
        }
    } else if (_t221.tag == HirExpr_EBinOp) {
        __auto_type l = _t221.data.EBinOp.left;
__auto_type r = _t221.data.EBinOp.right;
        /* pass */
        collect_moves(l, out);
        /* pass */
        collect_moves(r, out);
    } else if (_t221.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t221.data.EUnaryOp.expr;
        collect_moves(x, out);
    } else if (_t221.tag == HirExpr_ECall) {
        __auto_type callee = _t221.data.ECall.callee;
__auto_type args = _t221.data.ECall.args;
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
    } else if (_t221.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t221.data.EMethodCall.obj;
__auto_type args = _t221.data.EMethodCall.args;
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
    } else if (_t221.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t221.data.EPropAccess.obj;
        collect_moves(obj, out);
    } else if (_t221.tag == HirExpr_EIndex) {
        __auto_type obj = _t221.data.EIndex.obj;
__auto_type idx = _t221.data.EIndex._tr_v_index;
        /* pass */
        collect_moves(obj, out);
        /* pass */
        collect_moves(idx, out);
    } else if (_t221.tag == HirExpr_ECast) {
        __auto_type x = _t221.data.ECast.expr;
        collect_moves(x, out);
    } else if (_t221.tag == HirExpr_EAwait) {
        __auto_type x = _t221.data.EAwait.expr;
        collect_moves(x, out);
    } else if (_t221.tag == HirExpr_EIfElse) {
        __auto_type c = _t221.data.EIfElse.cond;
__auto_type t = _t221.data.EIfElse.then_e;
__auto_type f = _t221.data.EIfElse.else_e;
        /* pass */
        collect_moves(c, out);
        /* pass */
        collect_moves(t, out);
        /* pass */
        collect_moves(f, out);
    } else if (1) {
        __auto_type _ = _t221;
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
    __auto_type _t222 = (*arg);
    if (_t222.tag == HirExpr_EIdent) {
        __auto_type n = _t222.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (_t222.tag == HirExpr_ECast) {
        __auto_type x = _t222.data.ECast.expr;
        return free_target(x);
    } else if (1) {
        __auto_type _ = _t222;
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
    __auto_type _t223 = (*e);
    if (_t223.tag == HirExpr_ECall) {
        __auto_type callee = _t223.data.ECall.callee;
__auto_type args = _t223.data.ECall.args;
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t224 = (*callee);
            if (_t224.tag == HirExpr_EIdent) {
                __auto_type cn = _t224.data.EIdent.name;
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
                __auto_type _ = _t224;
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
    } else if (_t223.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t223.data.EMethodCall.obj;
__auto_type m = _t223.data.EMethodCall.method;
__auto_type args = _t223.data.EMethodCall.args;
        /* pass */
        if ((strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("free"))) == 0)) {
            /* pass */
            __auto_type _t225 = (*obj);
            if (_t225.tag == HirExpr_EIdent) {
                __auto_type on = _t225.data.EIdent.name;
                set_add(out, on);
            } else if (1) {
                __auto_type _ = _t225;
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
    } else if (_t223.tag == HirExpr_EBinOp) {
        __auto_type l = _t223.data.EBinOp.left;
__auto_type r = _t223.data.EBinOp.right;
        /* pass */
        collect_frees(l, out);
        /* pass */
        collect_frees(r, out);
    } else if (_t223.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t223.data.EUnaryOp.expr;
        collect_frees(x, out);
    } else if (_t223.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t223.data.EPropAccess.obj;
        collect_frees(obj, out);
    } else if (_t223.tag == HirExpr_EIndex) {
        __auto_type obj = _t223.data.EIndex.obj;
__auto_type idx = _t223.data.EIndex._tr_v_index;
        /* pass */
        collect_frees(obj, out);
        /* pass */
        collect_frees(idx, out);
    } else if (_t223.tag == HirExpr_ECast) {
        __auto_type x = _t223.data.ECast.expr;
        collect_frees(x, out);
    } else if (_t223.tag == HirExpr_EAwait) {
        __auto_type x = _t223.data.EAwait.expr;
        collect_frees(x, out);
    } else if (1) {
        __auto_type _ = _t223;
        /* pass */
    }
}

__attribute__((hot)) void block_gen_own(MirBlock* blk, List_TrStr* out) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < blk->stmts->len)) {
        /* pass */
        __auto_type _t226 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t226.tag == MirStmt_MDeclare) {
            __auto_type p = _t226.data.MDeclare.place;
            set_add(out, p);
        } else if (_t226.tag == MirStmt_MAssign) {
            __auto_type p = _t226.data.MAssign.place;
            set_add(out, p);
        } else if (_t226.tag == MirStmt_MEval) {
            __auto_type _ = _t226.data.MEval.value;
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
        __auto_type _t227 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t227.tag == MirStmt_MDeclare) {
            __auto_type val = _t227.data.MDeclare.value;
            /* pass */
            collect_moves(val, out);
            /* pass */
            collect_frees(val, out);
        } else if (_t227.tag == MirStmt_MAssign) {
            __auto_type val = _t227.data.MAssign.value;
            /* pass */
            collect_moves(val, out);
            /* pass */
            collect_frees(val, out);
        } else if (_t227.tag == MirStmt_MEval) {
            __auto_type val = _t227.data.MEval.value;
            /* pass */
            collect_moves(val, out);
            /* pass */
            collect_frees(val, out);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    __auto_type _t228 = (*blk->term);
    if (_t228.tag == MirTerm_TBranch) {
        __auto_type cond = _t228.data.TBranch.cond;
        collect_moves(cond, out);
    } else if (_t228.tag == MirTerm_TReturn) {
        __auto_type val = _t228.data.TReturn.value;
        /* pass */
        collect_moves(val, out);
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t229 = (*val);
            if (_t229.tag == HirExpr_EIdent) {
                __auto_type rnm = _t229.data.EIdent.name;
                set_add(out, rnm);
            } else if (1) {
                __auto_type _ = _t229;
                /* pass */
            }
        }
    } else if (1) {
        __auto_type _ = _t228;
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
        List_ptr_append(drops, LiveSet_minus(owned_i, ((LiveSet*)List_ptr_get(live_out, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return drops;
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

__attribute__((hot)) List_ptr* mir_if_drop_plan(HirFunction* hf) {
    /* pass */
    List_ptr* out = (void*)List_ptr_new();
    /* pass */
    MirFunction* mf = lower_function(hf);
    /* pass */
    if ((!mf->complete)) {
        /* pass */
        return out;
    }
    /* pass */
    if ((mf->if_bodies->len == 0LL)) {
        /* pass */
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
                ({ TrStr _at_t230 = (List_TrStr_get(dd->items, pi)); set_add(site->places, _at_t230); _tr_str_release(_at_t230); });
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
            TrStr _strtmp_t231 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t231;
        }
        /* pass */
        TrStr _strtmp_t232 = ({ TrStr _cr = (List_TrStr_get(v, i)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t232;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}")));
}

__attribute__((hot)) TrStr term_str(MirTerm* t) {
    /* pass */
    __auto_type _t233 = (*t);
    if (_t233.tag == MirTerm_TGoto) {
        __auto_type n = _t233.data.TGoto.target;
        return ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("goto bb")), _cr.data); _tr_str_release(_cr); _cres; });
    } else if (_t233.tag == MirTerm_TBranch) {
        __auto_type tt = _t233.data.TBranch.t;
__auto_type ee = _t233.data.TBranch.e;
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(tt)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("branch <cond> ? bb")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" : bb"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ee)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    } else if (_t233.tag == MirTerm_TReturn) {
        __auto_type _ = _t233.data.TReturn.value;
        return _tr_str_lit("return <expr>");
    } else if (_t233.tag == MirTerm_TReturnVoid) {
        return _tr_str_lit("return");
    } else if (_t233.tag == MirTerm_TUnset) {
        return _tr_str_lit("<unset>");
    }
    /* pass */
    return _tr_str_lit("<?>");
}

__attribute__((hot)) TrStr stmt_str(MirStmt* s) {
    /* pass */
    __auto_type _t234 = (*s);
    if (_t234.tag == MirStmt_MDeclare) {
        __auto_type p = _t234.data.MDeclare.place;
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("let ")), _tr_strz(p))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = <expr>"))); _tr_str_release(_cl); _cres; });
    } else if (_t234.tag == MirStmt_MAssign) {
        __auto_type p = _t234.data.MAssign.place;
        return _tr_strx_concat(_tr_strz(p), _tr_strz(_tr_str_lit(" = <expr>")));
    } else if (_t234.tag == MirStmt_MEval) {
        __auto_type _ = _t234.data.MEval.value;
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
            TrStr _strtmp_t235 = _tr_str_lit(" [INCOMPLETE - falls back to HIR]");
            _tr_str_release(tag);
            tag = _strtmp_t235;
        }
        /* pass */
        TrStr _strtmp_t236 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("fn ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(f->params->len)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" params), "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(f->blocks->len)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" blocks"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(out);
        out = _strtmp_t236;
        /* pass */
        List_ptr* live = compute_liveness(f);
        /* pass */
        List_ptr* drops = compute_drops(f, live);
        /* pass */
        long long bi = 0LL;
        /* pass */
        while ((bi < f->blocks->len)) {
            /* pass */
            MirBlock* blk = ((MirBlock*)List_ptr_get(f->blocks, bi));
            /* pass */
            TrStr _strtmp_t237 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("  bb")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t237;
            /* pass */
            long long si = 0LL;
            /* pass */
            while ((si < blk->stmts->len)) {
                /* pass */
                TrStr _strtmp_t238 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ")))); TrStr _cr = (stmt_str(((MirStmt*)List_ptr_get(blk->stmts, si)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(out);
                out = _strtmp_t238;
                /* pass */
                si = (si + 1LL);
            }
            /* pass */
            TrStr _strtmp_t239 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ")))); TrStr _cr = (term_str(blk->term)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t239;
            /* pass */
            TrStr _strtmp_t240 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ; live_out ")))); TrStr _cr = (set_str(((LiveSet*)List_ptr_get(live, blk->id))->items)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t240;
            /* pass */
            TrStr _strtmp_t241 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("    ; drop ")))); TrStr _cr = (set_str(((LiveSet*)List_ptr_get(drops, blk->id))->items)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(out);
            out = _strtmp_t241;
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

