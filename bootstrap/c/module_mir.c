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
#line 201 "src/mir.tr"
    MirProgram* p = ((MirProgram*)_tr_obj_alloc(sizeof(MirProgram)));
#line 202 "src/mir.tr"
    p->functions = (void*)List_ptr_new();
#line 203 "src/mir.tr"
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) MirBuilder* MirBuilder_init() {
#line 136 "src/mir.tr"
    MirBuilder* b = ((MirBuilder*)_tr_obj_alloc(sizeof(MirBuilder)));
#line 137 "src/mir.tr"
    b->blocks = (void*)List_ptr_new();
#line 138 "src/mir.tr"
    b->cur = (-1LL);
#line 139 "src/mir.tr"
    b->complete = true;
#line 140 "src/mir.tr"
    b->if_bodies = (void*)List_ptr_new();
#line 141 "src/mir.tr"
    b->in_unsafe = 0LL;
#line 142 "src/mir.tr"
    LiveSet* _cltmp_t605 = LiveSet_init();
    _tr_obj_release(b->unsafe_pinned, _trdrop_LiveSet);
    b->unsafe_pinned = _cltmp_t605;
#line 143 "src/mir.tr"
    b->loop_continue = (void*)List_i64_new();
#line 144 "src/mir.tr"
    b->loop_break = (void*)List_i64_new();
#line 145 "src/mir.tr"
    b->borrows = (void*)List_ptr_new();
#line 146 "src/mir.tr"
    return b;
}

__attribute__((hot)) void MirBuilder_record_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
#line 150 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 151 "src/mir.tr"
    e->borrower = _tr_str_retain(borrower);
#line 152 "src/mir.tr"
    e->source = _tr_str_retain(source);
#line 153 "src/mir.tr"
    e->decl_block = self->cur;
#line 154 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 155 "src/mir.tr"
    e->via_collection = false;
#line 156 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) void MirBuilder_record_coll_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
#line 160 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 161 "src/mir.tr"
    e->borrower = _tr_str_retain(borrower);
#line 162 "src/mir.tr"
    e->source = _tr_str_retain(source);
#line 163 "src/mir.tr"
    e->decl_block = self->cur;
#line 164 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 165 "src/mir.tr"
    e->via_collection = true;
#line 166 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) long long MirBuilder_new_block(MirBuilder* self) {
#line 170 "src/mir.tr"
    MirBlock* blk = ((MirBlock*)_tr_obj_alloc(sizeof(MirBlock)));
#line 171 "src/mir.tr"
    blk->id = self->blocks->len;
#line 172 "src/mir.tr"
    blk->stmts = (void*)List_ptr_new();
#line 173 "src/mir.tr"
    blk->term = box_mirterm(MirTerm_make_TUnset());
#line 174 "src/mir.tr"
    List_ptr_append(self->blocks, _tr_obj_retain(blk));
#line 175 "src/mir.tr"
    long long _trret163 = blk->id;
    _tr_obj_release(blk, _trdrop_MirBlock);
    return _trret163;
}

__attribute__((hot)) void MirBuilder_push_stmt(MirBuilder* self, MirStmt s) {
#line 178 "src/mir.tr"
    if ((self->cur >= 0LL)) {
#line 179 "src/mir.tr"
        MirBlock* blk = ((MirBlock*)List_ptr_get(self->blocks, self->cur));
#line 180 "src/mir.tr"
        ({ __auto_type _ord_t606 = (box_mirstmt(s)); List_ptr_append(blk->stmts, _ord_t606); });
#line 184 "src/mir.tr"
        if ((((unsigned long long)(blk->hir_block)) == ((unsigned long long)(0LL)))) {
#line 185 "src/mir.tr"
            HirBlock* _cltmp_t607 = _tr_obj_retain(self->cur_hb);
            _tr_obj_release(blk->hir_block, _trdrop_HirBlock);
            blk->hir_block = _cltmp_t607;
        }
    }
}

__attribute__((hot)) void MirBuilder_set_term(MirBuilder* self, MirTerm t) {
#line 188 "src/mir.tr"
    if ((self->cur >= 0LL)) {
#line 189 "src/mir.tr"
        __auto_type _asg_t608 = box_mirterm(t);
        ((MirBlock*)List_ptr_get(self->blocks, self->cur))->term = _asg_t608;
    }
}

__attribute__((hot)) bool MirBuilder_terminated(MirBuilder* self) {
#line 194 "src/mir.tr"
    if ((self->cur < 0LL)) {
#line 194 "src/mir.tr"
        return true;
    }
#line 195 "src/mir.tr"
    __auto_type _t609 = (*((MirBlock*)List_ptr_get(self->blocks, self->cur))->term);
    if (_t609.tag == MirTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t609;
        return true;
    }
}

__attribute__((malloc,returns_nonnull,hot)) LiveSet* LiveSet_init() {
#line 668 "src/mir.tr"
    LiveSet* s = ((LiveSet*)_tr_obj_alloc(sizeof(LiveSet)));
#line 669 "src/mir.tr"
    s->items = (void*)List_TrStr_new();
#line 670 "src/mir.tr"
    return s;
}

__attribute__((hot)) bool LiveSet_has(LiveSet* self, TrStr s) {
#line 673 "src/mir.tr"
    long long i = 0LL;
#line 674 "src/mir.tr"
    while ((i < self->items->len)) {
#line 675 "src/mir.tr"
        if (({ TrStr _cmpa_t610 = (List_TrStr_get(self->items, i)); TrStr _cmpb_t611 = (s); _Bool _cmpr_t612 = _tr_str_eqv(_cmpa_t610, _cmpb_t611); _tr_str_release(_cmpa_t610); _cmpr_t612; })) {
#line 675 "src/mir.tr"
            return true;
        }
#line 676 "src/mir.tr"
        i = (i + 1LL);
    }
#line 677 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool LiveSet_add(LiveSet* self, TrStr s) {
#line 680 "src/mir.tr"
    if (LiveSet_has(self, s)) {
#line 680 "src/mir.tr"
        return false;
    }
#line 681 "src/mir.tr"
    List_TrStr_append(self->items, s);
#line 682 "src/mir.tr"
    return true;
}

__attribute__((hot)) bool LiveSet_union_in(LiveSet* self, LiveSet* other) {
#line 686 "src/mir.tr"
    bool changed = false;
#line 687 "src/mir.tr"
    long long i = 0LL;
#line 688 "src/mir.tr"
    while ((i < other->items->len)) {
#line 689 "src/mir.tr"
        if (({ TrStr _at_t613 = (List_TrStr_get(other->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t613)); _tr_str_release(_at_t613); _wr; })) {
#line 689 "src/mir.tr"
            changed = true;
        }
#line 690 "src/mir.tr"
        i = (i + 1LL);
    }
#line 691 "src/mir.tr"
    return changed;
}

__attribute__((hot)) bool LiveSet_union_except(LiveSet* self, LiveSet* src, LiveSet* block) {
#line 695 "src/mir.tr"
    bool changed = false;
#line 696 "src/mir.tr"
    long long i = 0LL;
#line 697 "src/mir.tr"
    while ((i < src->items->len)) {
#line 698 "src/mir.tr"
        if (({ TrStr _at_t614 = (List_TrStr_get(src->items, i)); __auto_type _wr = ((!LiveSet_has(block, _at_t614))); _tr_str_release(_at_t614); _wr; })) {
#line 699 "src/mir.tr"
            if (({ TrStr _at_t615 = (List_TrStr_get(src->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t615)); _tr_str_release(_at_t615); _wr; })) {
#line 699 "src/mir.tr"
                changed = true;
            }
        }
#line 700 "src/mir.tr"
        i = (i + 1LL);
    }
#line 701 "src/mir.tr"
    return changed;
}

__attribute__((hot)) LiveSet* LiveSet_clone(LiveSet* self) {
#line 704 "src/mir.tr"
    LiveSet* r = LiveSet_init();
#line 705 "src/mir.tr"
    long long i = 0LL;
#line 706 "src/mir.tr"
    while ((i < self->items->len)) {
#line 707 "src/mir.tr"
        ({ TrStr _ord_t616 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _ord_t616); _tr_str_release(_ord_t616); });
#line 708 "src/mir.tr"
        i = (i + 1LL);
    }
#line 709 "src/mir.tr"
    return r;
}

__attribute__((hot)) void LiveSet_retain_common(LiveSet* self, LiveSet* other) {
#line 713 "src/mir.tr"
    List_TrStr* keep = (void*)List_TrStr_new();
#line 714 "src/mir.tr"
    long long i = 0LL;
#line 715 "src/mir.tr"
    while ((i < self->items->len)) {
#line 716 "src/mir.tr"
        if (({ TrStr _at_t617 = (List_TrStr_get(self->items, i)); __auto_type _wr = (LiveSet_has(other, _at_t617)); _tr_str_release(_at_t617); _wr; })) {
#line 716 "src/mir.tr"
            ({ TrStr _at_t618 = (List_TrStr_get(self->items, i)); List_TrStr_append(keep, _at_t618); _tr_str_release(_at_t618); });
        }
#line 717 "src/mir.tr"
        i = (i + 1LL);
    }
#line 718 "src/mir.tr"
    self->items = keep;
}

__attribute__((hot)) bool LiveSet_equals(LiveSet* self, LiveSet* other) {
#line 721 "src/mir.tr"
    if ((self->items->len != other->items->len)) {
#line 721 "src/mir.tr"
        return false;
    }
#line 722 "src/mir.tr"
    long long i = 0LL;
#line 723 "src/mir.tr"
    while ((i < self->items->len)) {
#line 724 "src/mir.tr"
        if (({ TrStr _at_t619 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t619))); _tr_str_release(_at_t619); _wr; })) {
#line 724 "src/mir.tr"
            return false;
        }
#line 725 "src/mir.tr"
        i = (i + 1LL);
    }
#line 726 "src/mir.tr"
    return true;
}

__attribute__((hot)) void LiveSet_set_to(LiveSet* self, LiveSet* other) {
#line 731 "src/mir.tr"
    List_TrStr* fresh = (void*)List_TrStr_new();
#line 732 "src/mir.tr"
    long long i = 0LL;
#line 733 "src/mir.tr"
    while ((i < other->items->len)) {
#line 734 "src/mir.tr"
        ({ TrStr _at_t620 = (List_TrStr_get(other->items, i)); List_TrStr_append(fresh, _at_t620); _tr_str_release(_at_t620); });
#line 735 "src/mir.tr"
        i = (i + 1LL);
    }
#line 736 "src/mir.tr"
    self->items = fresh;
}

__attribute__((hot)) LiveSet* LiveSet_minus(LiveSet* self, LiveSet* other) {
#line 740 "src/mir.tr"
    LiveSet* r = LiveSet_init();
#line 741 "src/mir.tr"
    long long i = 0LL;
#line 742 "src/mir.tr"
    while ((i < self->items->len)) {
#line 743 "src/mir.tr"
        if (({ TrStr _at_t621 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t621))); _tr_str_release(_at_t621); _wr; })) {
#line 743 "src/mir.tr"
            ({ TrStr _ord_t622 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _ord_t622); _tr_str_release(_ord_t622); });
        }
#line 744 "src/mir.tr"
        i = (i + 1LL);
    }
#line 745 "src/mir.tr"
    return r;
}

__attribute__((hot)) bool _is_mutating_call_on(HirExpr* val, TrStr source, TrMap* mm) {
#line 29 "src/mir.tr"
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 29 "src/mir.tr"
        return false;
    }
#line 30 "src/mir.tr"
    __auto_type _t623 = (*val);
    if (_t623.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t623.data.EMethodCall.obj;
__auto_type method = _t623.data.EMethodCall.method;
#line 32 "src/mir.tr"
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 32 "src/mir.tr"
            return false;
        }
#line 33 "src/mir.tr"
        __auto_type _t624 = (*obj);
        if (_t624.tag == HirExpr_EIdent) {
            __auto_type onm = _t624.data.EIdent.name;
#line 35 "src/mir.tr"
            if ((!_tr_str_eqv((onm), (source)))) {
#line 35 "src/mir.tr"
                return false;
            }
#line 37 "src/mir.tr"
            if ((((((((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("put", 3LL))))) {
#line 38 "src/mir.tr"
                return true;
            }
#line 39 "src/mir.tr"
            bool _trret162 = ({ TrStr _dkt_t625 = (({ TrStr _tr_cl = (_tr_strx_concatv((hir_expr_type(obj)->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t625))); _tr_str_release(_dkt_t625); _wr; });
            return _trret162;
        } else if (1) {
            __auto_type _ = _t624;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t623;
        return false;
    }
}

__attribute__((hot)) MirStmt* box_mirstmt(MirStmt s) {
#line 58 "src/mir.tr"
    /* unsafe block */
#line 59 "src/mir.tr"
    MirStmt* p = ((MirStmt*)_tr_c_calloc((size_t)(1LL), sizeof(MirStmt)));
#line 60 "src/mir.tr"
    (*p = s);
#line 61 "src/mir.tr"
    return p;
}

__attribute__((hot)) MirTerm* box_mirterm(MirTerm t) {
#line 64 "src/mir.tr"
    /* unsafe block */
#line 65 "src/mir.tr"
    MirTerm* p = ((MirTerm*)_tr_c_calloc((size_t)(1LL), sizeof(MirTerm)));
#line 66 "src/mir.tr"
    (*p = t);
#line 67 "src/mir.tr"
    return p;
}

__attribute__((hot)) void lower_stmts(MirBuilder* b, HirBlock* hb) {
#line 207 "src/mir.tr"
    HirBlock* saved_hb = _tr_obj_retain(b->cur_hb);
#line 208 "src/mir.tr"
    HirBlock* _cltmp_t626 = _tr_obj_retain(hb);
    _tr_obj_release(b->cur_hb, _trdrop_HirBlock);
    b->cur_hb = _cltmp_t626;
#line 209 "src/mir.tr"
    long long i = 0LL;
#line 210 "src/mir.tr"
    while ((i < hb->stmts->len)) {
#line 211 "src/mir.tr"
        if (MirBuilder_terminated(b)) {
#line 214 "src/mir.tr"
            b->cur = MirBuilder_new_block(b);
        }
#line 215 "src/mir.tr"
        HirStmt* sp = ((HirStmt*)List_ptr_get(hb->stmts, i));
#line 216 "src/mir.tr"
        __auto_type _t627 = (*sp);
        if (_t627.tag == HirStmt_SLet) {
            __auto_type nm = _t627.data.SLet.name;
__auto_type slet_ty = _t627.data.SLet.ty;
__auto_type val = _t627.data.SLet.val;
#line 218 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MDeclare(nm, val));
#line 222 "src/mir.tr"
            if ((slet_ty->is_borrow && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 223 "src/mir.tr"
                bool _slet_mut = false;
#line 224 "src/mir.tr"
                __auto_type _t628 = (*sp);
                if (_t628.tag == HirStmt_SLet) {
                    __auto_type _sm = _t628.data.SLet.is_mut;
                    _slet_mut = _sm;
                } else if (1) {
                    __auto_type _ = _t628;
                    /* pass */
                }
#line 227 "src/mir.tr"
                __auto_type _t629 = (*val);
                if (_t629.tag == HirExpr_EIdent) {
                    __auto_type src_nm = _t629.data.EIdent.name;
                    MirBuilder_record_borrow(b, nm, src_nm, (_slet_mut || slet_ty->is_mut_borrow));
                } else if (_t629.tag == HirExpr_EMethodCall) {
                    __auto_type mobj = _t629.data.EMethodCall.obj;
__auto_type mmethod = _t629.data.EMethodCall.method;
#line 234 "src/mir.tr"
                    if ((_tr_str_eqv((mmethod), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((mmethod), (_tr_str_lit_len("get_or", 6LL))))) {
#line 235 "src/mir.tr"
                        __auto_type _t630 = (*mobj);
                        if (_t630.tag == HirExpr_EIdent) {
                            __auto_type cnm = _t630.data.EIdent.name;
                            MirBuilder_record_coll_borrow(b, nm, cnm, (_slet_mut || slet_ty->is_mut_borrow));
                        } else if (1) {
                            __auto_type _ = _t630;
                            /* pass */
                        }
                    }
                } else if (_t629.tag == HirExpr_EIndex) {
                    __auto_type iobj = _t629.data.EIndex.obj;
#line 240 "src/mir.tr"
                    __auto_type _t631 = (*iobj);
                    if (_t631.tag == HirExpr_EIdent) {
                        __auto_type cnm2 = _t631.data.EIdent.name;
                        MirBuilder_record_coll_borrow(b, nm, cnm2, (_slet_mut || slet_ty->is_mut_borrow));
                    } else if (1) {
                        __auto_type _ = _t631;
                        /* pass */
                    }
                } else if (1) {
                    __auto_type _ = _t629;
                    /* pass */
                }
            }
        } else if (_t627.tag == HirStmt_SAssign) {
            __auto_type tgt = _t627.data.SAssign.target;
__auto_type val = _t627.data.SAssign.val;
#line 247 "src/mir.tr"
            __auto_type _t632 = (*tgt);
            if (_t632.tag == HirExpr_EIdent) {
                __auto_type tn = _t632.data.EIdent.name;
                MirBuilder_push_stmt(b, MirStmt_ctor_MAssign(tn, val));
            } else if (1) {
                __auto_type _ = _t632;
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(val));
            }
        } else if (_t627.tag == HirStmt_SExpr) {
            __auto_type e = _t627.data.SExpr.expr;
#line 251 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(e));
        } else if (_t627.tag == HirStmt_SReturn) {
            __auto_type v = _t627.data.SReturn.val;
#line 253 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TReturn(v));
        } else if (_t627.tag == HirStmt_SIf) {
            __auto_type cond = _t627.data.SIf.cond;
__auto_type then_b = _t627.data.SIf.then_b;
__auto_type else_b = _t627.data.SIf.else_b;
#line 255 "src/mir.tr"
            long long tb = MirBuilder_new_block(b);
#line 256 "src/mir.tr"
            long long eb = MirBuilder_new_block(b);
#line 257 "src/mir.tr"
            long long jb = MirBuilder_new_block(b);
#line 261 "src/mir.tr"
            if ((b->in_unsafe == 0LL)) {
#line 262 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(then_b));
#line 263 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(else_b));
            }
#line 264 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, tb, eb));
#line 265 "src/mir.tr"
            b->cur = tb;
#line 266 "src/mir.tr"
            lower_stmts(b, then_b);
#line 267 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 267 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
#line 268 "src/mir.tr"
            b->cur = eb;
#line 269 "src/mir.tr"
            lower_stmts(b, else_b);
#line 270 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 270 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
#line 271 "src/mir.tr"
            b->cur = jb;
        } else if (_t627.tag == HirStmt_SWhile) {
            __auto_type cond = _t627.data.SWhile.cond;
__auto_type body = _t627.data.SWhile.body;
#line 273 "src/mir.tr"
            long long hdr = MirBuilder_new_block(b);
#line 274 "src/mir.tr"
            long long body_b = MirBuilder_new_block(b);
#line 275 "src/mir.tr"
            long long exit_b = MirBuilder_new_block(b);
#line 276 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
#line 277 "src/mir.tr"
            b->cur = hdr;
#line 278 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, body_b, exit_b));
#line 279 "src/mir.tr"
            b->cur = body_b;
#line 280 "src/mir.tr"
            List_i64_append(b->loop_continue, hdr);
#line 281 "src/mir.tr"
            List_i64_append(b->loop_break, exit_b);
#line 282 "src/mir.tr"
            lower_stmts(b, body);
#line 283 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 284 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 285 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 285 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
            }
#line 286 "src/mir.tr"
            b->cur = exit_b;
        } else if (_t627.tag == HirStmt_SFor) {
            __auto_type fiter = _t627.data.SFor.iter;
__auto_type fbody = _t627.data.SFor.body;
#line 293 "src/mir.tr"
            long long for_hdr = MirBuilder_new_block(b);
#line 294 "src/mir.tr"
            long long for_body = MirBuilder_new_block(b);
#line 295 "src/mir.tr"
            long long for_exit = MirBuilder_new_block(b);
#line 296 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
#line 297 "src/mir.tr"
            b->cur = for_hdr;
#line 298 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fiter, for_body, for_exit));
#line 299 "src/mir.tr"
            b->cur = for_body;
#line 300 "src/mir.tr"
            List_i64_append(b->loop_continue, for_hdr);
#line 301 "src/mir.tr"
            List_i64_append(b->loop_break, for_exit);
#line 302 "src/mir.tr"
            lower_stmts(b, fbody);
#line 303 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 304 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 305 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 305 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
            }
#line 306 "src/mir.tr"
            b->cur = for_exit;
        } else if (_t627.tag == HirStmt_SForUnpack) {
            __auto_type fuiter = _t627.data.SForUnpack.iter;
__auto_type fubody = _t627.data.SForUnpack.body;
#line 308 "src/mir.tr"
            long long fu_hdr = MirBuilder_new_block(b);
#line 309 "src/mir.tr"
            long long fu_body = MirBuilder_new_block(b);
#line 310 "src/mir.tr"
            long long fu_exit = MirBuilder_new_block(b);
#line 311 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
#line 312 "src/mir.tr"
            b->cur = fu_hdr;
#line 313 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fuiter, fu_body, fu_exit));
#line 314 "src/mir.tr"
            b->cur = fu_body;
#line 315 "src/mir.tr"
            List_i64_append(b->loop_continue, fu_hdr);
#line 316 "src/mir.tr"
            List_i64_append(b->loop_break, fu_exit);
#line 317 "src/mir.tr"
            lower_stmts(b, fubody);
#line 318 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 319 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 320 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 320 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
            }
#line 321 "src/mir.tr"
            b->cur = fu_exit;
        } else if (_t627.tag == HirStmt_STry) {
            __auto_type try_body = _t627.data.STry.try_body;
__auto_type catches = _t627.data.STry.catches;
__auto_type finally_b = _t627.data.STry.finally_b;
#line 329 "src/mir.tr"
            long long try_b = MirBuilder_new_block(b);
#line 330 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 330 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_b));
            }
#line 331 "src/mir.tr"
            b->cur = try_b;
#line 332 "src/mir.tr"
            List_ptr_append(b->if_bodies, _tr_obj_retain(try_body));
#line 333 "src/mir.tr"
            lower_stmts(b, try_body);
#line 334 "src/mir.tr"
            long long tci = 0LL;
#line 335 "src/mir.tr"
            while ((tci < catches->len)) {
#line 336 "src/mir.tr"
                HirCatchClause* tcc = (*((HirCatchClause**)List_ptr_get(catches, tci)));
#line 337 "src/mir.tr"
                long long catch_b = MirBuilder_new_block(b);
#line 338 "src/mir.tr"
                if ((!MirBuilder_terminated(b))) {
#line 338 "src/mir.tr"
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(catch_b));
                }
#line 339 "src/mir.tr"
                b->cur = catch_b;
#line 340 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(tcc->body));
#line 341 "src/mir.tr"
                lower_stmts(b, tcc->body);
#line 342 "src/mir.tr"
                tci = (tci + 1LL);
            }
#line 343 "src/mir.tr"
            long long fin_b = MirBuilder_new_block(b);
#line 344 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 344 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fin_b));
            }
#line 345 "src/mir.tr"
            b->cur = fin_b;
#line 346 "src/mir.tr"
            List_ptr_append(b->if_bodies, _tr_obj_retain(finally_b));
#line 347 "src/mir.tr"
            lower_stmts(b, finally_b);
#line 348 "src/mir.tr"
            long long try_after = MirBuilder_new_block(b);
#line 349 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 349 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_after));
            }
#line 350 "src/mir.tr"
            b->cur = try_after;
        } else if (_t627.tag == HirStmt_SMatch) {
            __auto_type msubj = _t627.data.SMatch.expr;
__auto_type marms = _t627.data.SMatch.arms;
#line 357 "src/mir.tr"
            long long m_exit = MirBuilder_new_block(b);
#line 358 "src/mir.tr"
            long long mai = 0LL;
#line 359 "src/mir.tr"
            while ((mai < marms->len)) {
#line 360 "src/mir.tr"
                HirMatchArm* marm = ((HirMatchArm*)List_ptr_get(marms, mai));
#line 361 "src/mir.tr"
                long long arm_body = MirBuilder_new_block(b);
#line 362 "src/mir.tr"
                long long arm_next = MirBuilder_new_block(b);
#line 363 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TBranch(msubj, arm_body, arm_next));
#line 364 "src/mir.tr"
                b->cur = arm_body;
#line 365 "src/mir.tr"
                if ((((unsigned long long)(marm->guard)) != ((unsigned long long)(0LL)))) {
#line 366 "src/mir.tr"
                    MirBuilder_push_stmt(b, MirStmt_ctor_MEval(marm->guard));
                }
#line 367 "src/mir.tr"
                if ((b->in_unsafe == 0LL)) {
#line 367 "src/mir.tr"
                    List_ptr_append(b->if_bodies, _tr_obj_retain(marm->body));
                }
#line 368 "src/mir.tr"
                lower_stmts(b, marm->body);
#line 369 "src/mir.tr"
                if ((!MirBuilder_terminated(b))) {
#line 369 "src/mir.tr"
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
                }
#line 370 "src/mir.tr"
                b->cur = arm_next;
#line 371 "src/mir.tr"
                mai = (mai + 1LL);
            }
#line 372 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 372 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
            }
#line 373 "src/mir.tr"
            b->cur = m_exit;
        } else if (_t627.tag == HirStmt_SAssert) {
            __auto_type acond = _t627.data.SAssert.cond;
__auto_type amsg = _t627.data.SAssert.msg;
#line 375 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(acond));
#line 376 "src/mir.tr"
            if ((((unsigned long long)(amsg)) != ((unsigned long long)(0LL)))) {
#line 377 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(amsg));
            }
        } else if (_t627.tag == HirStmt_SUnsafe) {
            __auto_type ubody = _t627.data.SUnsafe.body;
#line 397 "src/mir.tr"
            b->in_unsafe = (b->in_unsafe + 1LL);
#line 398 "src/mir.tr"
            lower_stmts(b, ubody);
#line 399 "src/mir.tr"
            b->in_unsafe = (b->in_unsafe - 1LL);
        } else if (_t627.tag == HirStmt_SRaise) {
            __auto_type rval = _t627.data.SRaise.val;
#line 404 "src/mir.tr"
            if ((((unsigned long long)(rval)) != ((unsigned long long)(0LL)))) {
#line 405 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(rval));
            }
        } else if (_t627.tag == HirStmt_SWith) {
            __auto_type witems = _t627.data.SWith.items;
__auto_type wbody = _t627.data.SWith.body;
#line 410 "src/mir.tr"
            long long wi = 0LL;
#line 411 "src/mir.tr"
            while ((wi < witems->len)) {
#line 412 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(((HirExpr*)List_ptr_get(witems, wi))));
#line 413 "src/mir.tr"
                wi = (wi + 1LL);
            }
#line 414 "src/mir.tr"
            lower_stmts(b, wbody);
        } else if (_t627.tag == HirStmt_SMultiLet) {
            __auto_type mlval = _t627.data.SMultiLet.val;
#line 419 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(mlval));
        } else if (_t627.tag == HirStmt_SLineMarker) {
            __auto_type _ = _t627.data.SLineMarker.n;
#line 421 "src/mir.tr"
            /* pass */
        } else if (_t627.tag == HirStmt_SAutoDrop) {
#line 423 "src/mir.tr"
            /* pass */
        } else if (_t627.tag == HirStmt_SBreak) {
            __auto_type _ = _t627.data.SBreak.val;
#line 431 "src/mir.tr"
            if ((b->loop_break->len > 0LL)) {
#line 432 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_break, (b->loop_break->len - 1LL))));
            } else {
#line 434 "src/mir.tr"
                b->complete = false;
            }
        } else if (_t627.tag == HirStmt_SContinue) {
#line 438 "src/mir.tr"
            if ((b->loop_continue->len > 0LL)) {
#line 439 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_continue, (b->loop_continue->len - 1LL))));
            } else {
#line 441 "src/mir.tr"
                b->complete = false;
            }
        } else if (1) {
            __auto_type _ = _t627;
#line 446 "src/mir.tr"
            b->complete = false;
        }
#line 447 "src/mir.tr"
        i = (i + 1LL);
    }
#line 448 "src/mir.tr"
    HirBlock* _cltmp_t633 = _tr_obj_retain(saved_hb);
    _tr_obj_release(b->cur_hb, _trdrop_HirBlock);
    b->cur_hb = _cltmp_t633;
    _tr_obj_release(saved_hb, _trdrop_HirBlock);
}

__attribute__((hot)) MirFunction* lower_function(HirFunction* hf) {
#line 451 "src/mir.tr"
    MirFunction* mf = ((MirFunction*)_tr_obj_alloc(sizeof(MirFunction)));
#line 452 "src/mir.tr"
    mf->name = _tr_str_retain(hf->name);
#line 453 "src/mir.tr"
    mf->params = (void*)List_TrStr_new();
#line 454 "src/mir.tr"
    long long pi = 0LL;
#line 455 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 456 "src/mir.tr"
        ({ __auto_type _ord_t634 = (((HirParam*)List_ptr_get(hf->params, pi))->name); List_TrStr_append(mf->params, _ord_t634); });
#line 457 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 458 "src/mir.tr"
    MirBuilder* b = MirBuilder_init();
#line 459 "src/mir.tr"
    b->cur = MirBuilder_new_block(b);
#line 460 "src/mir.tr"
    lower_stmts(b, hf->body);
#line 461 "src/mir.tr"
    if ((!MirBuilder_terminated(b))) {
#line 461 "src/mir.tr"
        MirBuilder_set_term(b, MirTerm_make_TReturnVoid());
    }
#line 468 "src/mir.tr"
    long long rbi = 0LL;
#line 469 "src/mir.tr"
    while ((rbi < b->blocks->len)) {
#line 470 "src/mir.tr"
        MirBlock* rblk = ((MirBlock*)List_ptr_get(b->blocks, rbi));
#line 471 "src/mir.tr"
        long long rsi = 0LL;
#line 472 "src/mir.tr"
        while ((rsi < rblk->stmts->len)) {
#line 473 "src/mir.tr"
            __auto_type _t635 = (*((MirStmt*)List_ptr_get(rblk->stmts, rsi)));
            if (_t635.tag == MirStmt_MDeclare) {
                __auto_type rv = _t635.data.MDeclare.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t635.tag == MirStmt_MAssign) {
                __auto_type rv = _t635.data.MAssign.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t635.tag == MirStmt_MEval) {
                __auto_type rv = _t635.data.MEval.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            }
#line 477 "src/mir.tr"
            rsi = (rsi + 1LL);
        }
#line 478 "src/mir.tr"
        __auto_type _t636 = (*rblk->term);
        if (_t636.tag == MirTerm_TBranch) {
            __auto_type rc = _t636.data.TBranch.cond;
            collect_raw_borrows(rc, b->unsafe_pinned);
        } else if (_t636.tag == MirTerm_TReturn) {
            __auto_type rv = _t636.data.TReturn.value;
            collect_raw_borrows(rv, b->unsafe_pinned);
        } else if (1) {
            __auto_type _ = _t636;
            /* pass */
        }
#line 482 "src/mir.tr"
        rbi = (rbi + 1LL);
    }
#line 483 "src/mir.tr"
    mf->blocks = b->blocks;
#line 484 "src/mir.tr"
    mf->complete = b->complete;
#line 485 "src/mir.tr"
    mf->if_bodies = b->if_bodies;
#line 486 "src/mir.tr"
    LiveSet* _cltmp_t637 = _tr_obj_retain(b->unsafe_pinned);
    _tr_obj_release(mf->unsafe_pinned, _trdrop_LiveSet);
    mf->unsafe_pinned = _cltmp_t637;
#line 487 "src/mir.tr"
    mf->borrows = b->borrows;
#line 491 "src/mir.tr"
    long long hbi = 0LL;
#line 492 "src/mir.tr"
    if ((((unsigned long long)(hf->borrow_borrowers)) == ((unsigned long long)(0LL)))) {
#line 492 "src/mir.tr"
        _tr_obj_release(b, _trdrop_MirBuilder);
        return mf;
    }
#line 493 "src/mir.tr"
    if ((((unsigned long long)(hf->borrow_sources)) == ((unsigned long long)(0LL)))) {
#line 493 "src/mir.tr"
        _tr_obj_release(b, _trdrop_MirBuilder);
        return mf;
    }
#line 494 "src/mir.tr"
    while (((hbi < hf->borrow_borrowers->len) && (hbi < hf->borrow_sources->len))) {
#line 495 "src/mir.tr"
        BorrowEdge* he = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 496 "src/mir.tr"
        he->borrower = List_TrStr_get(hf->borrow_borrowers, hbi);
#line 497 "src/mir.tr"
        he->source = List_TrStr_get(hf->borrow_sources, hbi);
#line 498 "src/mir.tr"
        he->decl_block = (-1LL);
#line 500 "src/mir.tr"
        List_ptr_append(mf->borrows, _tr_obj_retain(he));
#line 501 "src/mir.tr"
        hbi = (hbi + 1LL);
        _tr_obj_release(he, _trdrop_BorrowEdge);
    }
#line 502 "src/mir.tr"
    _tr_obj_release(b, _trdrop_MirBuilder);
    return mf;
}

__attribute__((hot)) MirProgram* lower_program(HirProgram* hir) {
#line 505 "src/mir.tr"
    MirProgram* mp = MirProgram_init();
#line 506 "src/mir.tr"
    long long i = 0LL;
#line 507 "src/mir.tr"
    while ((i < hir->functions->len)) {
#line 508 "src/mir.tr"
        ({ MirFunction* _ord_t638 = (lower_function(((HirFunction*)List_ptr_get(hir->functions, i)))); List_ptr_append(mp->functions, _tr_obj_retain(_ord_t638)); _tr_obj_release(_ord_t638, _trdrop_MirFunction); });
#line 509 "src/mir.tr"
        i = (i + 1LL);
    }
#line 510 "src/mir.tr"
    return mp;
}

__attribute__((hot)) bool set_contains(List_TrStr* v, TrStr s) {
#line 520 "src/mir.tr"
    long long i = 0LL;
#line 521 "src/mir.tr"
    while ((i < v->len)) {
#line 522 "src/mir.tr"
        if (({ TrStr _cmpa_t639 = (List_TrStr_get(v, i)); TrStr _cmpb_t640 = (s); _Bool _cmpr_t641 = _tr_str_eqv(_cmpa_t639, _cmpb_t640); _tr_str_release(_cmpa_t639); _cmpr_t641; })) {
#line 522 "src/mir.tr"
            return true;
        }
#line 523 "src/mir.tr"
        i = (i + 1LL);
    }
#line 524 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool set_add(List_TrStr* v, TrStr s) {
#line 527 "src/mir.tr"
    if (set_contains(v, s)) {
#line 527 "src/mir.tr"
        return false;
    }
#line 528 "src/mir.tr"
    List_TrStr_append(v, s);
#line 529 "src/mir.tr"
    return true;
}

__attribute__((hot)) void collect_uses(HirExpr* e, List_TrStr* out) {
#line 533 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 533 "src/mir.tr"
        return;
    }
#line 534 "src/mir.tr"
    __auto_type _t642 = (*e);
    if (_t642.tag == HirExpr_EIdent) {
        __auto_type nm = _t642.data.EIdent.name;
        set_add(out, nm);
    } else if (_t642.tag == HirExpr_EBinOp) {
        __auto_type l = _t642.data.EBinOp.left;
__auto_type r = _t642.data.EBinOp.right;
#line 537 "src/mir.tr"
        collect_uses(l, out);
#line 538 "src/mir.tr"
        collect_uses(r, out);
    } else if (_t642.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t642.data.EUnaryOp.expr;
        collect_uses(x, out);
    } else if (_t642.tag == HirExpr_ECall) {
        __auto_type callee = _t642.data.ECall.callee;
__auto_type args = _t642.data.ECall.args;
#line 541 "src/mir.tr"
        collect_uses(callee, out);
#line 542 "src/mir.tr"
        long long i = 0LL;
#line 543 "src/mir.tr"
        while ((i < args->len)) {
#line 544 "src/mir.tr"
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
#line 545 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t642.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t642.data.EMethodCall.obj;
__auto_type args = _t642.data.EMethodCall.args;
#line 547 "src/mir.tr"
        collect_uses(obj, out);
#line 548 "src/mir.tr"
        long long i = 0LL;
#line 549 "src/mir.tr"
        while ((i < args->len)) {
#line 550 "src/mir.tr"
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
#line 551 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t642.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t642.data.EPropAccess.obj;
        collect_uses(obj, out);
    } else if (_t642.tag == HirExpr_EIndex) {
        __auto_type obj = _t642.data.EIndex.obj;
__auto_type idx = _t642.data.EIndex._tr_v_index;
#line 554 "src/mir.tr"
        collect_uses(obj, out);
#line 555 "src/mir.tr"
        collect_uses(idx, out);
    } else if (_t642.tag == HirExpr_ECast) {
        __auto_type x = _t642.data.ECast.expr;
        collect_uses(x, out);
    } else if (_t642.tag == HirExpr_EAwait) {
        __auto_type x = _t642.data.EAwait.expr;
        collect_uses(x, out);
    } else if (_t642.tag == HirExpr_EIfElse) {
        __auto_type c = _t642.data.EIfElse.cond;
__auto_type t = _t642.data.EIfElse.then_e;
__auto_type f = _t642.data.EIfElse.else_e;
#line 559 "src/mir.tr"
        collect_uses(c, out);
#line 560 "src/mir.tr"
        collect_uses(t, out);
#line 561 "src/mir.tr"
        collect_uses(f, out);
    } else if (1) {
        __auto_type _ = _t642;
        /* pass */
    }
}

__attribute__((hot)) void collect_raw_borrows(HirExpr* e, LiveSet* out) {
#line 576 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 576 "src/mir.tr"
        return;
    }
#line 577 "src/mir.tr"
    __auto_type _t643 = (*e);
    if (_t643.tag == HirExpr_ECast) {
        __auto_type inner = _t643.data.ECast.expr;
__auto_type ty = _t643.data.ECast.target_ty;
#line 579 "src/mir.tr"
        if (_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 580 "src/mir.tr"
            __auto_type _t644 = (*inner);
            if (_t644.tag == HirExpr_EIdent) {
                __auto_type nm = _t644.data.EIdent.name;
                bool _a = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t644;
                /* pass */
            }
        }
#line 583 "src/mir.tr"
        collect_raw_borrows(inner, out);
    } else if (_t643.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t643.data.EUnaryOp.op;
__auto_type inner = _t643.data.EUnaryOp.expr;
#line 585 "src/mir.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 586 "src/mir.tr"
            __auto_type _t645 = (*inner);
            if (_t645.tag == HirExpr_EIdent) {
                __auto_type nm = _t645.data.EIdent.name;
                bool _b = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t645;
                /* pass */
            }
        }
#line 589 "src/mir.tr"
        collect_raw_borrows(inner, out);
    } else if (_t643.tag == HirExpr_EBinOp) {
        __auto_type l = _t643.data.EBinOp.left;
__auto_type r = _t643.data.EBinOp.right;
#line 591 "src/mir.tr"
        collect_raw_borrows(l, out);
#line 592 "src/mir.tr"
        collect_raw_borrows(r, out);
    } else if (_t643.tag == HirExpr_ECall) {
        __auto_type callee = _t643.data.ECall.callee;
__auto_type args = _t643.data.ECall.args;
#line 594 "src/mir.tr"
        collect_raw_borrows(callee, out);
#line 595 "src/mir.tr"
        long long i = 0LL;
#line 596 "src/mir.tr"
        while ((i < args->len)) {
#line 597 "src/mir.tr"
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
#line 598 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t643.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t643.data.EMethodCall.obj;
__auto_type args = _t643.data.EMethodCall.args;
#line 600 "src/mir.tr"
        collect_raw_borrows(obj, out);
#line 601 "src/mir.tr"
        long long i = 0LL;
#line 602 "src/mir.tr"
        while ((i < args->len)) {
#line 603 "src/mir.tr"
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
#line 604 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t643.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t643.data.EPropAccess.obj;
        collect_raw_borrows(obj, out);
    } else if (_t643.tag == HirExpr_EIndex) {
        __auto_type obj = _t643.data.EIndex.obj;
__auto_type idx = _t643.data.EIndex._tr_v_index;
#line 607 "src/mir.tr"
        collect_raw_borrows(obj, out);
#line 608 "src/mir.tr"
        collect_raw_borrows(idx, out);
    } else if (_t643.tag == HirExpr_EAwait) {
        __auto_type x = _t643.data.EAwait.expr;
        collect_raw_borrows(x, out);
    } else if (_t643.tag == HirExpr_EIfElse) {
        __auto_type c = _t643.data.EIfElse.cond;
__auto_type t = _t643.data.EIfElse.then_e;
__auto_type f = _t643.data.EIfElse.else_e;
#line 611 "src/mir.tr"
        collect_raw_borrows(c, out);
#line 612 "src/mir.tr"
        collect_raw_borrows(t, out);
#line 613 "src/mir.tr"
        collect_raw_borrows(f, out);
    } else if (1) {
        __auto_type _ = _t643;
        /* pass */
    }
}

__attribute__((hot)) void add_exposed(HirExpr* e, List_TrStr* gen, List_TrStr* kill) {
#line 621 "src/mir.tr"
    List_TrStr* tmp = (void*)List_TrStr_new();
#line 622 "src/mir.tr"
    collect_uses(e, tmp);
#line 623 "src/mir.tr"
    long long i = 0LL;
#line 624 "src/mir.tr"
    while ((i < tmp->len)) {
#line 625 "src/mir.tr"
        if (({ TrStr _at_t646 = (List_TrStr_get(tmp, i)); __auto_type _wr = ((!set_contains(kill, _at_t646))); _tr_str_release(_at_t646); _wr; })) {
#line 625 "src/mir.tr"
            ({ TrStr _at_t647 = (List_TrStr_get(tmp, i)); set_add(gen, _at_t647); _tr_str_release(_at_t647); });
        }
#line 626 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_use_def(MirBlock* blk, List_TrStr* gen, List_TrStr* kill) {
#line 631 "src/mir.tr"
    long long i = 0LL;
#line 632 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 633 "src/mir.tr"
        __auto_type _t648 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t648.tag == MirStmt_MDeclare) {
            __auto_type p = _t648.data.MDeclare.place;
__auto_type val = _t648.data.MDeclare.value;
#line 635 "src/mir.tr"
            add_exposed(val, gen, kill);
#line 636 "src/mir.tr"
            set_add(kill, p);
        } else if (_t648.tag == MirStmt_MAssign) {
            __auto_type p = _t648.data.MAssign.place;
__auto_type val = _t648.data.MAssign.value;
#line 638 "src/mir.tr"
            add_exposed(val, gen, kill);
#line 639 "src/mir.tr"
            set_add(kill, p);
        } else if (_t648.tag == MirStmt_MEval) {
            __auto_type val = _t648.data.MEval.value;
            add_exposed(val, gen, kill);
        }
#line 641 "src/mir.tr"
        i = (i + 1LL);
    }
#line 642 "src/mir.tr"
    __auto_type _t649 = (*blk->term);
    if (_t649.tag == MirTerm_TBranch) {
        __auto_type cond = _t649.data.TBranch.cond;
        add_exposed(cond, gen, kill);
    } else if (_t649.tag == MirTerm_TReturn) {
        __auto_type val = _t649.data.TReturn.value;
        add_exposed(val, gen, kill);
    } else if (1) {
        __auto_type _ = _t649;
        /* pass */
    }
}

__attribute__((hot)) void block_succs(MirBlock* blk, List_i64* out) {
#line 648 "src/mir.tr"
    __auto_type _t650 = (*blk->term);
    if (_t650.tag == MirTerm_TGoto) {
        __auto_type n = _t650.data.TGoto.target;
        List_i64_append(out, n);
    } else if (_t650.tag == MirTerm_TBranch) {
        __auto_type t = _t650.data.TBranch.t;
__auto_type e = _t650.data.TBranch.e;
#line 651 "src/mir.tr"
        List_i64_append(out, t);
#line 652 "src/mir.tr"
        List_i64_append(out, e);
    } else if (1) {
        __auto_type _ = _t650;
        /* pass */
    }
}

__attribute__((hot)) List_ptr* compute_liveness(MirFunction* mf) {
#line 752 "src/mir.tr"
    long long n = mf->blocks->len;
#line 753 "src/mir.tr"
    List_ptr* uses = (void*)List_ptr_new();
#line 754 "src/mir.tr"
    List_ptr* defs = (void*)List_ptr_new();
#line 755 "src/mir.tr"
    List_ptr* live_in = (void*)List_ptr_new();
#line 756 "src/mir.tr"
    List_ptr* live_out = (void*)List_ptr_new();
#line 757 "src/mir.tr"
    long long i = 0LL;
#line 758 "src/mir.tr"
    while ((i < n)) {
#line 759 "src/mir.tr"
        LiveSet* u = LiveSet_init();
#line 760 "src/mir.tr"
        LiveSet* d = LiveSet_init();
#line 761 "src/mir.tr"
        block_use_def(((MirBlock*)List_ptr_get(mf->blocks, i)), u->items, d->items);
#line 762 "src/mir.tr"
        List_ptr_append(uses, _tr_obj_retain(u));
#line 763 "src/mir.tr"
        List_ptr_append(defs, _tr_obj_retain(d));
#line 764 "src/mir.tr"
        List_ptr_append(live_in, LiveSet_init());
#line 765 "src/mir.tr"
        List_ptr_append(live_out, LiveSet_init());
#line 766 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(u, _trdrop_LiveSet);
        _tr_obj_release(d, _trdrop_LiveSet);
    }
#line 767 "src/mir.tr"
    bool changed = true;
#line 768 "src/mir.tr"
    while (changed) {
#line 769 "src/mir.tr"
        changed = false;
#line 770 "src/mir.tr"
        long long bi = (n - 1LL);
#line 771 "src/mir.tr"
        while ((bi >= 0LL)) {
#line 772 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
#line 773 "src/mir.tr"
            List_i64* sl = (void*)List_i64_new();
#line 774 "src/mir.tr"
            block_succs(blk, sl);
#line 775 "src/mir.tr"
            long long si = 0LL;
#line 776 "src/mir.tr"
            while ((si < sl->len)) {
#line 777 "src/mir.tr"
                if (({ __auto_type _ord_t651 = (((LiveSet*)List_ptr_get(live_out, bi))); __auto_type _ord_t652 = (((LiveSet*)List_ptr_get(live_in, List_i64_get(sl, si)))); __auto_type _wr = (LiveSet_union_in(_ord_t651, _ord_t652)); _wr; })) {
#line 777 "src/mir.tr"
                    changed = true;
                }
#line 778 "src/mir.tr"
                si = (si + 1LL);
            }
#line 779 "src/mir.tr"
            if (({ __auto_type _ord_t653 = (((LiveSet*)List_ptr_get(live_in, bi))); __auto_type _ord_t654 = (((LiveSet*)List_ptr_get(uses, bi))); __auto_type _wr = (LiveSet_union_in(_ord_t653, _ord_t654)); _wr; })) {
#line 779 "src/mir.tr"
                changed = true;
            }
#line 780 "src/mir.tr"
            if (({ __auto_type _ord_t655 = (((LiveSet*)List_ptr_get(live_in, bi))); __auto_type _ord_t656 = (((LiveSet*)List_ptr_get(live_out, bi))); __auto_type _ord_t657 = (((LiveSet*)List_ptr_get(defs, bi))); __auto_type _wr = (LiveSet_union_except(_ord_t655, _ord_t656, _ord_t657)); _wr; })) {
#line 780 "src/mir.tr"
                changed = true;
            }
#line 781 "src/mir.tr"
            bi = (bi - 1LL);
        }
    }
#line 782 "src/mir.tr"
    List_ptr_free_obj(uses, _trdrop_LiveSet);
    List_ptr_free_obj(defs, _trdrop_LiveSet);
    List_ptr_free_obj(live_in, _trdrop_LiveSet);
    return live_out;
}

__attribute__((hot)) void collect_moves(HirExpr* e, List_TrStr* out) {
#line 803 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 803 "src/mir.tr"
        return;
    }
#line 804 "src/mir.tr"
    __auto_type _t658 = (*e);
    if (_t658.tag == HirExpr_EIdent) {
        __auto_type nm = _t658.data.EIdent.name;
__auto_type ismv = _t658.data.EIdent.is_move;
#line 806 "src/mir.tr"
        if (ismv) {
#line 806 "src/mir.tr"
            set_add(out, nm);
        }
    } else if (_t658.tag == HirExpr_EBinOp) {
        __auto_type l = _t658.data.EBinOp.left;
__auto_type r = _t658.data.EBinOp.right;
#line 808 "src/mir.tr"
        collect_moves(l, out);
#line 809 "src/mir.tr"
        collect_moves(r, out);
    } else if (_t658.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t658.data.EUnaryOp.expr;
        collect_moves(x, out);
    } else if (_t658.tag == HirExpr_ECall) {
        __auto_type callee = _t658.data.ECall.callee;
__auto_type args = _t658.data.ECall.args;
#line 812 "src/mir.tr"
        collect_moves(callee, out);
#line 813 "src/mir.tr"
        long long i = 0LL;
#line 814 "src/mir.tr"
        while ((i < args->len)) {
#line 815 "src/mir.tr"
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
#line 816 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t658.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t658.data.EMethodCall.obj;
__auto_type args = _t658.data.EMethodCall.args;
#line 818 "src/mir.tr"
        collect_moves(obj, out);
#line 819 "src/mir.tr"
        long long i = 0LL;
#line 820 "src/mir.tr"
        while ((i < args->len)) {
#line 821 "src/mir.tr"
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
#line 822 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t658.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t658.data.EPropAccess.obj;
        collect_moves(obj, out);
    } else if (_t658.tag == HirExpr_EIndex) {
        __auto_type obj = _t658.data.EIndex.obj;
__auto_type idx = _t658.data.EIndex._tr_v_index;
#line 825 "src/mir.tr"
        collect_moves(obj, out);
#line 826 "src/mir.tr"
        collect_moves(idx, out);
    } else if (_t658.tag == HirExpr_ECast) {
        __auto_type x = _t658.data.ECast.expr;
        collect_moves(x, out);
    } else if (_t658.tag == HirExpr_EAwait) {
        __auto_type x = _t658.data.EAwait.expr;
        collect_moves(x, out);
    } else if (_t658.tag == HirExpr_EIfElse) {
        __auto_type c = _t658.data.EIfElse.cond;
__auto_type t = _t658.data.EIfElse.then_e;
__auto_type f = _t658.data.EIfElse.else_e;
#line 830 "src/mir.tr"
        collect_moves(c, out);
#line 831 "src/mir.tr"
        collect_moves(t, out);
#line 832 "src/mir.tr"
        collect_moves(f, out);
    } else if (1) {
        __auto_type _ = _t658;
        /* pass */
    }
}

__attribute__((hot)) bool is_free_fn(TrStr n) {
#line 837 "src/mir.tr"
    return (((((_tr_str_eqv((n), (_tr_str_lit_len("dealloc", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_c_free", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_free", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_str_release", 15LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_str_free", 12LL))));
}

__attribute__((hot)) TrStr free_target(HirExpr* arg) {
#line 842 "src/mir.tr"
    if ((((unsigned long long)(arg)) == ((unsigned long long)(0LL)))) {
#line 842 "src/mir.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 843 "src/mir.tr"
    __auto_type _t659 = (*arg);
    if (_t659.tag == HirExpr_EIdent) {
        __auto_type n = _t659.data.EIdent.name;
        TrStr _trret165 = _tr_str_retain(n);
        return _trret165;
    } else if (_t659.tag == HirExpr_ECast) {
        __auto_type x = _t659.data.ECast.expr;
        return free_target(x);
    } else if (1) {
        __auto_type _ = _t659;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) void collect_frees(HirExpr* e, List_TrStr* out) {
#line 856 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 856 "src/mir.tr"
        return;
    }
#line 857 "src/mir.tr"
    __auto_type _t660 = (*e);
    if (_t660.tag == HirExpr_ECall) {
        __auto_type callee = _t660.data.ECall.callee;
__auto_type args = _t660.data.ECall.args;
#line 859 "src/mir.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 860 "src/mir.tr"
            __auto_type _t661 = (*callee);
            if (_t661.tag == HirExpr_EIdent) {
                __auto_type cn = _t661.data.EIdent.name;
#line 862 "src/mir.tr"
                if ((is_free_fn(cn) && (args->len > 0LL))) {
#line 863 "src/mir.tr"
                    TrStr ft = free_target(((HirExpr*)List_ptr_get(args, 0LL)));
#line 864 "src/mir.tr"
                    if ((!_tr_str_eqv((ft), (_tr_str_lit_len("", 0LL))))) {
#line 864 "src/mir.tr"
                        set_add(out, ft);
                    }
                }
            } else if (1) {
                __auto_type _ = _t661;
                /* pass */
            }
        }
#line 866 "src/mir.tr"
        collect_frees(callee, out);
#line 867 "src/mir.tr"
        long long i = 0LL;
#line 868 "src/mir.tr"
        while ((i < args->len)) {
#line 869 "src/mir.tr"
            collect_frees(((HirExpr*)List_ptr_get(args, i)), out);
#line 870 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t660.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t660.data.EMethodCall.obj;
__auto_type m = _t660.data.EMethodCall.method;
__auto_type args = _t660.data.EMethodCall.args;
#line 872 "src/mir.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) {
#line 873 "src/mir.tr"
            __auto_type _t662 = (*obj);
            if (_t662.tag == HirExpr_EIdent) {
                __auto_type on = _t662.data.EIdent.name;
                set_add(out, on);
            } else if (1) {
                __auto_type _ = _t662;
                /* pass */
            }
        }
#line 876 "src/mir.tr"
        collect_frees(obj, out);
#line 877 "src/mir.tr"
        long long j = 0LL;
#line 878 "src/mir.tr"
        while ((j < args->len)) {
#line 879 "src/mir.tr"
            collect_frees(((HirExpr*)List_ptr_get(args, j)), out);
#line 880 "src/mir.tr"
            j = (j + 1LL);
        }
    } else if (_t660.tag == HirExpr_EBinOp) {
        __auto_type l = _t660.data.EBinOp.left;
__auto_type r = _t660.data.EBinOp.right;
#line 882 "src/mir.tr"
        collect_frees(l, out);
#line 883 "src/mir.tr"
        collect_frees(r, out);
    } else if (_t660.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t660.data.EUnaryOp.expr;
        collect_frees(x, out);
    } else if (_t660.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t660.data.EPropAccess.obj;
        collect_frees(obj, out);
    } else if (_t660.tag == HirExpr_EIndex) {
        __auto_type obj = _t660.data.EIndex.obj;
__auto_type idx = _t660.data.EIndex._tr_v_index;
#line 887 "src/mir.tr"
        collect_frees(obj, out);
#line 888 "src/mir.tr"
        collect_frees(idx, out);
    } else if (_t660.tag == HirExpr_ECast) {
        __auto_type x = _t660.data.ECast.expr;
        collect_frees(x, out);
    } else if (_t660.tag == HirExpr_EAwait) {
        __auto_type x = _t660.data.EAwait.expr;
        collect_frees(x, out);
    } else if (1) {
        __auto_type _ = _t660;
        /* pass */
    }
}

__attribute__((hot)) void block_gen_own(MirBlock* blk, List_TrStr* out) {
#line 895 "src/mir.tr"
    long long i = 0LL;
#line 896 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 897 "src/mir.tr"
        __auto_type _t663 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t663.tag == MirStmt_MDeclare) {
            __auto_type p = _t663.data.MDeclare.place;
            set_add(out, p);
        } else if (_t663.tag == MirStmt_MAssign) {
            __auto_type p = _t663.data.MAssign.place;
            set_add(out, p);
        } else if (_t663.tag == MirStmt_MEval) {
            __auto_type _ = _t663.data.MEval.value;
            /* pass */
        }
#line 901 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_moves(MirBlock* blk, List_TrStr* out) {
#line 905 "src/mir.tr"
    long long i = 0LL;
#line 906 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 907 "src/mir.tr"
        __auto_type _t664 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t664.tag == MirStmt_MDeclare) {
            __auto_type val = _t664.data.MDeclare.value;
#line 909 "src/mir.tr"
            collect_moves(val, out);
#line 910 "src/mir.tr"
            collect_frees(val, out);
        } else if (_t664.tag == MirStmt_MAssign) {
            __auto_type val = _t664.data.MAssign.value;
#line 912 "src/mir.tr"
            collect_moves(val, out);
#line 913 "src/mir.tr"
            collect_frees(val, out);
        } else if (_t664.tag == MirStmt_MEval) {
            __auto_type val = _t664.data.MEval.value;
#line 915 "src/mir.tr"
            collect_moves(val, out);
#line 916 "src/mir.tr"
            collect_frees(val, out);
        }
#line 917 "src/mir.tr"
        i = (i + 1LL);
    }
#line 918 "src/mir.tr"
    __auto_type _t665 = (*blk->term);
    if (_t665.tag == MirTerm_TBranch) {
        __auto_type cond = _t665.data.TBranch.cond;
        collect_moves(cond, out);
    } else if (_t665.tag == MirTerm_TReturn) {
        __auto_type val = _t665.data.TReturn.value;
#line 921 "src/mir.tr"
        collect_moves(val, out);
#line 925 "src/mir.tr"
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 926 "src/mir.tr"
            __auto_type _t666 = (*val);
            if (_t666.tag == HirExpr_EIdent) {
                __auto_type rnm = _t666.data.EIdent.name;
                set_add(out, rnm);
            } else if (1) {
                __auto_type _ = _t666;
                /* pass */
            }
        }
    } else if (1) {
        __auto_type _ = _t665;
        /* pass */
    }
}

__attribute__((hot)) void preds_of(MirFunction* mf, long long b, List_i64* out) {
#line 933 "src/mir.tr"
    long long i = 0LL;
#line 934 "src/mir.tr"
    while ((i < mf->blocks->len)) {
#line 935 "src/mir.tr"
        List_i64* sl = (void*)List_i64_new();
#line 936 "src/mir.tr"
        block_succs(((MirBlock*)List_ptr_get(mf->blocks, i)), sl);
#line 937 "src/mir.tr"
        long long j = 0LL;
#line 938 "src/mir.tr"
        while ((j < sl->len)) {
#line 939 "src/mir.tr"
            if ((List_i64_get(sl, j) == b)) {
#line 939 "src/mir.tr"
                List_i64_append(out, i);
            }
#line 940 "src/mir.tr"
            j = (j + 1LL);
        }
#line 941 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_ptr* compute_drops(MirFunction* mf, List_ptr* live_out) {
#line 945 "src/mir.tr"
    long long n = mf->blocks->len;
#line 946 "src/mir.tr"
    List_ptr* gen = (void*)List_ptr_new();
#line 947 "src/mir.tr"
    List_ptr* kill = (void*)List_ptr_new();
#line 948 "src/mir.tr"
    List_ptr* own_out = (void*)List_ptr_new();
#line 949 "src/mir.tr"
    LiveSet* universe = LiveSet_init();
#line 950 "src/mir.tr"
    long long i = 0LL;
#line 951 "src/mir.tr"
    while ((i < n)) {
#line 952 "src/mir.tr"
        LiveSet* g = LiveSet_init();
#line 953 "src/mir.tr"
        LiveSet* k = LiveSet_init();
#line 954 "src/mir.tr"
        block_gen_own(((MirBlock*)List_ptr_get(mf->blocks, i)), g->items);
#line 955 "src/mir.tr"
        block_moves(((MirBlock*)List_ptr_get(mf->blocks, i)), k->items);
#line 956 "src/mir.tr"
        List_ptr_append(gen, _tr_obj_retain(g));
#line 957 "src/mir.tr"
        List_ptr_append(kill, _tr_obj_retain(k));
#line 958 "src/mir.tr"
        bool _u = LiveSet_union_in(universe, g);
#line 959 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(g, _trdrop_LiveSet);
        _tr_obj_release(k, _trdrop_LiveSet);
    }
#line 961 "src/mir.tr"
    i = 0LL;
#line 962 "src/mir.tr"
    while ((i < n)) {
#line 963 "src/mir.tr"
        List_ptr_append(own_out, LiveSet_clone(universe));
#line 964 "src/mir.tr"
        i = (i + 1LL);
    }
#line 965 "src/mir.tr"
    bool changed = true;
#line 966 "src/mir.tr"
    while (changed) {
#line 967 "src/mir.tr"
        changed = false;
#line 968 "src/mir.tr"
        long long b = 0LL;
#line 969 "src/mir.tr"
        while ((b < n)) {
#line 970 "src/mir.tr"
            List_i64* preds = (void*)List_i64_new();
#line 971 "src/mir.tr"
            preds_of(mf, b, preds);
#line 972 "src/mir.tr"
            LiveSet* own_in = LiveSet_init();
#line 973 "src/mir.tr"
            if ((preds->len > 0LL)) {
#line 974 "src/mir.tr"
                LiveSet* _cltmp_t667 = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, 0LL))));
                _tr_obj_release(own_in, _trdrop_LiveSet);
                own_in = _cltmp_t667;
#line 975 "src/mir.tr"
                long long pi = 1LL;
#line 976 "src/mir.tr"
                while ((pi < preds->len)) {
#line 977 "src/mir.tr"
                    LiveSet_retain_common(own_in, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, pi))));
#line 978 "src/mir.tr"
                    pi = (pi + 1LL);
                }
            }
#line 979 "src/mir.tr"
            bool _g = LiveSet_union_in(own_in, ((LiveSet*)List_ptr_get(gen, b)));
#line 983 "src/mir.tr"
            LiveSet* owned_b = LiveSet_minus(own_in, ((LiveSet*)List_ptr_get(kill, b)));
#line 984 "src/mir.tr"
            LiveSet* new_oo = LiveSet_clone(owned_b);
#line 985 "src/mir.tr"
            LiveSet_retain_common(new_oo, ((LiveSet*)List_ptr_get(live_out, b)));
#line 986 "src/mir.tr"
            if ((!LiveSet_equals(new_oo, ((LiveSet*)List_ptr_get(own_out, b))))) {
#line 987 "src/mir.tr"
                changed = true;
#line 988 "src/mir.tr"
                LiveSet_set_to(((LiveSet*)List_ptr_get(own_out, b)), new_oo);
            }
#line 989 "src/mir.tr"
            b = (b + 1LL);
            _tr_obj_release(own_in, _trdrop_LiveSet);
            _tr_obj_release(owned_b, _trdrop_LiveSet);
            _tr_obj_release(new_oo, _trdrop_LiveSet);
        }
    }
#line 991 "src/mir.tr"
    List_ptr* drops = (void*)List_ptr_new();
#line 992 "src/mir.tr"
    i = 0LL;
#line 993 "src/mir.tr"
    while ((i < n)) {
#line 994 "src/mir.tr"
        List_i64* preds2 = (void*)List_i64_new();
#line 995 "src/mir.tr"
        preds_of(mf, i, preds2);
#line 996 "src/mir.tr"
        LiveSet* own_in2 = LiveSet_init();
#line 997 "src/mir.tr"
        if ((preds2->len > 0LL)) {
#line 998 "src/mir.tr"
            LiveSet* _cltmp_t668 = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, 0LL))));
            _tr_obj_release(own_in2, _trdrop_LiveSet);
            own_in2 = _cltmp_t668;
#line 999 "src/mir.tr"
            long long pj = 1LL;
#line 1000 "src/mir.tr"
            while ((pj < preds2->len)) {
#line 1001 "src/mir.tr"
                LiveSet_retain_common(own_in2, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, pj))));
#line 1002 "src/mir.tr"
                pj = (pj + 1LL);
            }
        }
#line 1003 "src/mir.tr"
        bool _g2 = LiveSet_union_in(own_in2, ((LiveSet*)List_ptr_get(gen, i)));
#line 1004 "src/mir.tr"
        LiveSet* owned_i = LiveSet_minus(own_in2, ((LiveSet*)List_ptr_get(kill, i)));
#line 1005 "src/mir.tr"
        LiveSet* dead_i = LiveSet_minus(owned_i, ((LiveSet*)List_ptr_get(live_out, i)));
#line 1006 "src/mir.tr"
        List_ptr_append(drops, LiveSet_minus(dead_i, mf->unsafe_pinned));
#line 1007 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(own_in2, _trdrop_LiveSet);
        _tr_obj_release(owned_i, _trdrop_LiveSet);
        _tr_obj_release(dead_i, _trdrop_LiveSet);
    }
#line 1008 "src/mir.tr"
    List_ptr_free_obj(gen, _trdrop_LiveSet);
    List_ptr_free_obj(kill, _trdrop_LiveSet);
    List_ptr_free_obj(own_out, _trdrop_LiveSet);
    _tr_obj_release(universe, _trdrop_LiveSet);
    return drops;
}

__attribute__((hot)) long long last_use_in_block(MirBlock* blk, TrStr name) {
#line 1025 "src/mir.tr"
    long long last = (-1LL);
#line 1026 "src/mir.tr"
    long long si = 0LL;
#line 1027 "src/mir.tr"
    while ((si < blk->stmts->len)) {
#line 1028 "src/mir.tr"
        List_TrStr* tmp = (void*)List_TrStr_new();
#line 1029 "src/mir.tr"
        __auto_type _t669 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t669.tag == MirStmt_MDeclare) {
            __auto_type val = _t669.data.MDeclare.value;
            collect_uses(val, tmp);
        } else if (_t669.tag == MirStmt_MAssign) {
            __auto_type val = _t669.data.MAssign.value;
            collect_uses(val, tmp);
        } else if (_t669.tag == MirStmt_MEval) {
            __auto_type val = _t669.data.MEval.value;
            collect_uses(val, tmp);
        }
#line 1033 "src/mir.tr"
        if (set_contains(tmp, name)) {
#line 1033 "src/mir.tr"
            last = si;
        }
#line 1034 "src/mir.tr"
        si = (si + 1LL);
    }
#line 1035 "src/mir.tr"
    List_TrStr* tt = (void*)List_TrStr_new();
#line 1036 "src/mir.tr"
    __auto_type _t670 = (*blk->term);
    if (_t670.tag == MirTerm_TBranch) {
        __auto_type cond = _t670.data.TBranch.cond;
        collect_uses(cond, tt);
    } else if (_t670.tag == MirTerm_TReturn) {
        __auto_type val = _t670.data.TReturn.value;
        collect_uses(val, tt);
    } else if (1) {
        __auto_type _ = _t670;
        /* pass */
    }
#line 1040 "src/mir.tr"
    if (set_contains(tt, name)) {
#line 1040 "src/mir.tr"
        last = blk->stmts->len;
    }
#line 1041 "src/mir.tr"
    return last;
}

__attribute__((hot)) List_bool* compute_borrow_outlives(MirFunction* mf, List_ptr* live_out) {
#line 1044 "src/mir.tr"
    List_bool* out = (void*)List_bool_new();
#line 1045 "src/mir.tr"
    long long i = 0LL;
#line 1046 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1047 "src/mir.tr"
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1048 "src/mir.tr"
        bool proven = true;
#line 1050 "src/mir.tr"
        long long bi = 0LL;
#line 1051 "src/mir.tr"
        while ((bi < mf->blocks->len)) {
#line 1052 "src/mir.tr"
            LiveSet* lo = ((LiveSet*)List_ptr_get(live_out, bi));
#line 1053 "src/mir.tr"
            if ((LiveSet_has(lo, edge->borrower) && (!LiveSet_has(lo, edge->source)))) {
#line 1054 "src/mir.tr"
                proven = false;
            }
#line 1055 "src/mir.tr"
            bi = (bi + 1LL);
        }
#line 1061 "src/mir.tr"
        long long db = edge->decl_block;
#line 1062 "src/mir.tr"
        if (((db >= 0LL) && (db < mf->blocks->len))) {
#line 1063 "src/mir.tr"
            if (({ __auto_type _ord_t671 = (((LiveSet*)List_ptr_get(live_out, db))); __auto_type _ord_t672 = (edge->source); __auto_type _wr = ((!LiveSet_has(_ord_t671, _ord_t672))); _wr; })) {
#line 1064 "src/mir.tr"
                MirBlock* dblk = ((MirBlock*)List_ptr_get(mf->blocks, db));
#line 1065 "src/mir.tr"
                long long src_last = last_use_in_block(dblk, edge->source);
#line 1066 "src/mir.tr"
                long long bor_last = last_use_in_block(dblk, edge->borrower);
#line 1067 "src/mir.tr"
                if (({ __auto_type _ord_t673 = (((LiveSet*)List_ptr_get(live_out, db))); __auto_type _ord_t674 = (edge->borrower); __auto_type _wr = (LiveSet_has(_ord_t673, _ord_t674)); _wr; })) {
#line 1068 "src/mir.tr"
                    bor_last = (dblk->stmts->len + 1LL);
                }
#line 1069 "src/mir.tr"
                if ((bor_last > src_last)) {
#line 1070 "src/mir.tr"
                    proven = false;
                }
            }
        }
#line 1071 "src/mir.tr"
        List_bool_append(out, proven);
#line 1072 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1073 "src/mir.tr"
    return out;
}

__attribute__((hot)) bool is_if_body(MirFunction* mf, HirBlock* hb) {
#line 1083 "src/mir.tr"
    long long i = 0LL;
#line 1084 "src/mir.tr"
    while ((i < mf->if_bodies->len)) {
#line 1085 "src/mir.tr"
        if ((((unsigned long long)(((HirBlock*)List_ptr_get(mf->if_bodies, i)))) == ((unsigned long long)(hb)))) {
#line 1085 "src/mir.tr"
            return true;
        }
#line 1086 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1087 "src/mir.tr"
    return false;
}

__attribute__((hot)) DropSite* site_for(List_ptr* out, HirBlock* hb) {
#line 1091 "src/mir.tr"
    long long i = 0LL;
#line 1092 "src/mir.tr"
    while ((i < out->len)) {
#line 1093 "src/mir.tr"
        if ((((unsigned long long)(((DropSite*)List_ptr_get(out, i))->hir_block)) == ((unsigned long long)(hb)))) {
#line 1093 "src/mir.tr"
            return _tr_obj_retain(((DropSite*)List_ptr_get(out, i)));
        }
#line 1094 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1095 "src/mir.tr"
    DropSite* s = ((DropSite*)_tr_obj_alloc(sizeof(DropSite)));
#line 1096 "src/mir.tr"
    HirBlock* _cltmp_t675 = _tr_obj_retain(hb);
    _tr_obj_release(s->hir_block, _trdrop_HirBlock);
    s->hir_block = _cltmp_t675;
#line 1097 "src/mir.tr"
    s->places = (void*)List_TrStr_new();
#line 1098 "src/mir.tr"
    List_ptr_append(out, _tr_obj_retain(s));
#line 1099 "src/mir.tr"
    return s;
}

__attribute__((hot)) List_TrStr* mir_proven_borrows(HirFunction* hf) {
#line 1107 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1108 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1109 "src/mir.tr"
    if ((!mf->complete)) {
#line 1109 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1110 "src/mir.tr"
    if ((mf->borrows->len == 0LL)) {
#line 1110 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1111 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1112 "src/mir.tr"
    List_bool* outl = compute_borrow_outlives(mf, live);
#line 1113 "src/mir.tr"
    long long i = 0LL;
#line 1114 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1115 "src/mir.tr"
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1119 "src/mir.tr"
        bool proven = List_bool_get(outl, i);
#line 1125 "src/mir.tr"
        if ((proven && edge->via_collection)) {
#line 1126 "src/mir.tr"
            long long bn = 0LL;
#line 1127 "src/mir.tr"
            while ((bn < mf->blocks->len)) {
#line 1128 "src/mir.tr"
                MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1129 "src/mir.tr"
                LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
#line 1130 "src/mir.tr"
                long long si = 0LL;
#line 1131 "src/mir.tr"
                while ((si < blk->stmts->len)) {
#line 1132 "src/mir.tr"
                    bool is_creation = false;
#line 1133 "src/mir.tr"
                    __auto_type _t676 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                    if (_t676.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t676.data.MDeclare.place;
#line 1135 "src/mir.tr"
                        if (_tr_str_eqv((dn), (edge->borrower))) {
#line 1135 "src/mir.tr"
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t676;
                        /* pass */
                    }
#line 1137 "src/mir.tr"
                    if ((!is_creation)) {
#line 1138 "src/mir.tr"
                        List_TrStr* uses = (void*)List_TrStr_new();
#line 1139 "src/mir.tr"
                        __auto_type _t677 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                        if (_t677.tag == MirStmt_MDeclare) {
                            __auto_type val = _t677.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t677.tag == MirStmt_MAssign) {
                            __auto_type ap = _t677.data.MAssign.place;
__auto_type val = _t677.data.MAssign.value;
#line 1142 "src/mir.tr"
                            collect_uses(val, uses);
#line 1143 "src/mir.tr"
                            if (_tr_str_eqv((ap), (edge->source))) {
#line 1143 "src/mir.tr"
                                set_add(uses, edge->source);
                            }
                        } else if (_t677.tag == MirStmt_MEval) {
                            __auto_type val = _t677.data.MEval.value;
                            collect_uses(val, uses);
                        }
#line 1145 "src/mir.tr"
                        if ((set_contains(uses, edge->source) && borrower_live_after(blk, si, edge->borrower, lo))) {
#line 1146 "src/mir.tr"
                            proven = false;
                        }
                    }
#line 1147 "src/mir.tr"
                    si = (si + 1LL);
                }
#line 1148 "src/mir.tr"
                bn = (bn + 1LL);
            }
        }
#line 1149 "src/mir.tr"
        if (proven) {
#line 1150 "src/mir.tr"
            List_TrStr_append(out, edge->borrower);
        }
#line 1151 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1152 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_bool_free(outl);
    return out;
}

__attribute__((hot)) bool borrower_live_after(MirBlock* blk, long long after_idx, TrStr name, LiveSet* lo) {
#line 1159 "src/mir.tr"
    long long si = (after_idx + 1LL);
#line 1160 "src/mir.tr"
    while ((si < blk->stmts->len)) {
#line 1161 "src/mir.tr"
        List_TrStr* uses = (void*)List_TrStr_new();
#line 1162 "src/mir.tr"
        __auto_type _t678 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t678.tag == MirStmt_MDeclare) {
            __auto_type val = _t678.data.MDeclare.value;
            collect_uses(val, uses);
        } else if (_t678.tag == MirStmt_MAssign) {
            __auto_type val = _t678.data.MAssign.value;
            collect_uses(val, uses);
        } else if (_t678.tag == MirStmt_MEval) {
            __auto_type val = _t678.data.MEval.value;
            collect_uses(val, uses);
        }
#line 1166 "src/mir.tr"
        if (set_contains(uses, name)) {
#line 1166 "src/mir.tr"
            return true;
        }
#line 1167 "src/mir.tr"
        si = (si + 1LL);
    }
#line 1168 "src/mir.tr"
    List_TrStr* tu = (void*)List_TrStr_new();
#line 1169 "src/mir.tr"
    __auto_type _t679 = (*blk->term);
    if (_t679.tag == MirTerm_TBranch) {
        __auto_type cond = _t679.data.TBranch.cond;
        collect_uses(cond, tu);
    } else if (_t679.tag == MirTerm_TReturn) {
        __auto_type val = _t679.data.TReturn.value;
        collect_uses(val, tu);
    } else if (1) {
        __auto_type _ = _t679;
        /* pass */
    }
#line 1173 "src/mir.tr"
    if (set_contains(tu, name)) {
#line 1173 "src/mir.tr"
        return true;
    }
#line 1174 "src/mir.tr"
    return LiveSet_has(lo, name);
}

__attribute__((hot)) List_TrStr* mir_borrow_conflicts(HirFunction* hf, TrMap* mutating_methods) {
#line 1181 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1182 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1183 "src/mir.tr"
    if ((!mf->complete)) {
#line 1183 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1184 "src/mir.tr"
    if ((mf->borrows->len == 0LL)) {
#line 1184 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1185 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1192 "src/mir.tr"
    long long k = 0LL;
#line 1193 "src/mir.tr"
    while ((k < mf->borrows->len)) {
#line 1194 "src/mir.tr"
        BorrowEdge* be = ((BorrowEdge*)List_ptr_get(mf->borrows, k));
#line 1195 "src/mir.tr"
        long long bn = 0LL;
#line 1196 "src/mir.tr"
        while ((bn < mf->blocks->len)) {
#line 1197 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1198 "src/mir.tr"
            LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
#line 1199 "src/mir.tr"
            long long si = 0LL;
#line 1200 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1201 "src/mir.tr"
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
#line 1202 "src/mir.tr"
                TrStr invalid_desc = _tr_str_lit_len("", 0LL);
#line 1203 "src/mir.tr"
                HirExpr* sval = ((HirExpr*)(0LL));
#line 1204 "src/mir.tr"
                __auto_type _t680 = (*stmt);
                if (_t680.tag == MirStmt_MAssign) {
                    __auto_type wp = _t680.data.MAssign.place;
__auto_type wv = _t680.data.MAssign.value;
#line 1206 "src/mir.tr"
                    sval = wv;
#line 1207 "src/mir.tr"
                    if (_tr_str_eqv((wp), (be->source))) {
#line 1207 "src/mir.tr"
                        TrStr _strtmp_t681 = _tr_str_lit_len("assign to", 9LL);
                        _tr_str_release(invalid_desc);
                        invalid_desc = _strtmp_t681;
                    }
                } else if (_t680.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t680.data.MDeclare.value;
                    sval = dv;
                } else if (_t680.tag == MirStmt_MEval) {
                    __auto_type ev = _t680.data.MEval.value;
                    sval = ev;
                }
#line 1210 "src/mir.tr"
                if ((_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))) && _is_mutating_call_on(sval, be->source, mutating_methods))) {
#line 1211 "src/mir.tr"
                    TrStr _strtmp_t682 = _tr_str_lit_len("call a mutating method on", 25LL);
                    _tr_str_release(invalid_desc);
                    invalid_desc = _strtmp_t682;
                }
#line 1212 "src/mir.tr"
                if ((!_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))))) {
#line 1213 "src/mir.tr"
                    if (borrower_live_after(blk, si, be->borrower, lo)) {
#line 1214 "src/mir.tr"
                        ({ TrStr _at_t683 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot ", 7LL)), (invalid_desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" '", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->source)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is borrowed by '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — the borrow is still live", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t683); _tr_str_release(_at_t683); });
                    }
                } else if (be->is_exclusive) {
#line 1218 "src/mir.tr"
                    bool is_creation = false;
#line 1219 "src/mir.tr"
                    __auto_type _t684 = (*stmt);
                    if (_t684.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t684.data.MDeclare.place;
#line 1221 "src/mir.tr"
                        if (_tr_str_eqv((dn), (be->borrower))) {
#line 1221 "src/mir.tr"
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t684;
                        /* pass */
                    }
#line 1223 "src/mir.tr"
                    if ((!is_creation)) {
#line 1224 "src/mir.tr"
                        List_TrStr* uses = (void*)List_TrStr_new();
#line 1225 "src/mir.tr"
                        __auto_type _t685 = (*stmt);
                        if (_t685.tag == MirStmt_MDeclare) {
                            __auto_type val = _t685.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t685.tag == MirStmt_MAssign) {
                            __auto_type val = _t685.data.MAssign.value;
                            collect_uses(val, uses);
                        } else if (_t685.tag == MirStmt_MEval) {
                            __auto_type val = _t685.data.MEval.value;
                            collect_uses(val, uses);
                        }
#line 1229 "src/mir.tr"
                        if ((set_contains(uses, be->source) && borrower_live_after(blk, si, be->borrower, lo))) {
#line 1230 "src/mir.tr"
                            ({ TrStr _at_t686 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot use '", 12LL)), (be->source))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is exclusively (mut) borrowed by '", 45LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t686); _tr_str_release(_at_t686); });
                        }
                    }
                }
#line 1231 "src/mir.tr"
                si = (si + 1LL);
                _tr_str_release(invalid_desc);
            }
#line 1232 "src/mir.tr"
            bn = (bn + 1LL);
        }
#line 1233 "src/mir.tr"
        k = (k + 1LL);
    }
#line 1234 "src/mir.tr"
    if ((mf->borrows->len < 2LL)) {
#line 1234 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        List_ptr_free_obj(live, _trdrop_LiveSet);
        return out;
    }
#line 1235 "src/mir.tr"
    long long i = 0LL;
#line 1236 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1237 "src/mir.tr"
        BorrowEdge* bi = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1238 "src/mir.tr"
        if (((bi->decl_block >= 0LL) && (bi->decl_block < mf->blocks->len))) {
#line 1239 "src/mir.tr"
            long long j = (i + 1LL);
#line 1240 "src/mir.tr"
            while ((j < mf->borrows->len)) {
#line 1241 "src/mir.tr"
                BorrowEdge* bj = ((BorrowEdge*)List_ptr_get(mf->borrows, j));
#line 1242 "src/mir.tr"
                if ((((bj->decl_block == bi->decl_block) && _tr_str_eqv((bi->source), (bj->source))) && (bi->is_exclusive || bj->is_exclusive))) {
#line 1243 "src/mir.tr"
                    MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi->decl_block));
#line 1244 "src/mir.tr"
                    long long pi = 0LL;
#line 1245 "src/mir.tr"
                    long long pj = 0LL;
#line 1246 "src/mir.tr"
                    long long bk = 0LL;
#line 1247 "src/mir.tr"
                    while ((bk < blk->stmts->len)) {
#line 1248 "src/mir.tr"
                        __auto_type _t687 = (*((MirStmt*)List_ptr_get(blk->stmts, bk)));
                        if (_t687.tag == MirStmt_MDeclare) {
                            __auto_type dn = _t687.data.MDeclare.place;
#line 1250 "src/mir.tr"
                            if (_tr_str_eqv((dn), (bi->borrower))) {
#line 1250 "src/mir.tr"
                                pi = bk;
                            }
#line 1251 "src/mir.tr"
                            if (_tr_str_eqv((dn), (bj->borrower))) {
#line 1251 "src/mir.tr"
                                pj = bk;
                            }
                        } else if (1) {
                            __auto_type _ = _t687;
                            /* pass */
                        }
#line 1253 "src/mir.tr"
                        bk = (bk + 1LL);
                    }
#line 1254 "src/mir.tr"
                    long long ui = last_use_in_block(blk, bi->borrower);
#line 1255 "src/mir.tr"
                    long long uj = last_use_in_block(blk, bj->borrower);
#line 1256 "src/mir.tr"
                    if ((ui < pi)) {
#line 1256 "src/mir.tr"
                        ui = pi;
                    }
#line 1257 "src/mir.tr"
                    if ((uj < pj)) {
#line 1257 "src/mir.tr"
                        uj = pj;
                    }
#line 1258 "src/mir.tr"
                    if (((pi <= uj) && (pj <= ui))) {
#line 1259 "src/mir.tr"
                        TrStr kind = _tr_str_lit_len("a mutable borrow", 16LL);
#line 1260 "src/mir.tr"
                        if ((!(bi->is_exclusive && bj->is_exclusive))) {
#line 1260 "src/mir.tr"
                            TrStr _strtmp_t688 = _tr_str_lit_len("a mutable and a shared borrow", 29LL);
                            _tr_str_release(kind);
                            kind = _strtmp_t688;
                        }
#line 1261 "src/mir.tr"
                        ({ TrStr _at_t689 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (bi->borrower))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' and '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bj->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' are ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kind)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bi->source)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' that are live at the same time", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t689); _tr_str_release(_at_t689); });
                    }
                }
#line 1262 "src/mir.tr"
                j = (j + 1LL);
            }
        }
#line 1263 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1264 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_ptr_free_obj(live, _trdrop_LiveSet);
    return out;
}

__attribute__((hot)) List_TrStr* mir_shared_ref_param_violations(HirFunction* hf, TrMap* mutating_methods) {
#line 1271 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1273 "src/mir.tr"
    List_TrStr* shps = (void*)List_TrStr_new();
#line 1274 "src/mir.tr"
    long long pi = 0LL;
#line 1275 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 1276 "src/mir.tr"
        AstType* pty = ((HirParam*)List_ptr_get(hf->params, pi))->ty;
#line 1277 "src/mir.tr"
        if ((pty->is_borrow && (!pty->is_mut_borrow))) {
#line 1278 "src/mir.tr"
            List_TrStr_append(shps, ((HirParam*)List_ptr_get(hf->params, pi))->name);
        }
#line 1279 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 1280 "src/mir.tr"
    if ((shps->len == 0LL)) {
#line 1280 "src/mir.tr"
        List_TrStr_free(shps);
        return out;
    }
#line 1281 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1282 "src/mir.tr"
    long long sk = 0LL;
#line 1283 "src/mir.tr"
    while ((sk < shps->len)) {
#line 1284 "src/mir.tr"
        TrStr pn = List_TrStr_get(shps, sk);
#line 1285 "src/mir.tr"
        long long bn = 0LL;
#line 1286 "src/mir.tr"
        while ((bn < mf->blocks->len)) {
#line 1287 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1288 "src/mir.tr"
            long long si = 0LL;
#line 1289 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1290 "src/mir.tr"
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
#line 1291 "src/mir.tr"
                HirExpr* sval = ((HirExpr*)(0LL));
#line 1292 "src/mir.tr"
                TrStr desc = _tr_str_lit_len("", 0LL);
#line 1293 "src/mir.tr"
                __auto_type _t690 = (*stmt);
                if (_t690.tag == MirStmt_MAssign) {
                    __auto_type wp = _t690.data.MAssign.place;
__auto_type wv = _t690.data.MAssign.value;
#line 1295 "src/mir.tr"
                    sval = wv;
#line 1296 "src/mir.tr"
                    if (_tr_str_eqv((wp), (pn))) {
#line 1296 "src/mir.tr"
                        TrStr _strtmp_t691 = _tr_str_lit_len("reassign", 8LL);
                        _tr_str_release(desc);
                        desc = _strtmp_t691;
                    }
                } else if (_t690.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t690.data.MDeclare.value;
                    sval = dv;
                } else if (_t690.tag == MirStmt_MEval) {
                    __auto_type ev = _t690.data.MEval.value;
                    sval = ev;
                }
#line 1299 "src/mir.tr"
                if ((_tr_str_eqv((desc), (_tr_str_lit_len("", 0LL))) && _is_mutating_call_on(sval, pn, mutating_methods))) {
#line 1300 "src/mir.tr"
                    TrStr _strtmp_t692 = _tr_str_lit_len("call a mutating method on", 25LL);
                    _tr_str_release(desc);
                    desc = _strtmp_t692;
                }
#line 1301 "src/mir.tr"
                if ((!_tr_str_eqv((desc), (_tr_str_lit_len("", 0LL))))) {
#line 1302 "src/mir.tr"
                    ({ TrStr _at_t693 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot ", 7LL)), (desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" '", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t693); _tr_str_release(_at_t693); });
                }
#line 1303 "src/mir.tr"
                si = (si + 1LL);
                _tr_str_release(desc);
            }
#line 1304 "src/mir.tr"
            bn = (bn + 1LL);
        }
#line 1305 "src/mir.tr"
        sk = (sk + 1LL);
        _tr_str_release(pn);
    }
#line 1306 "src/mir.tr"
    List_TrStr_free(shps);
    _tr_obj_release(mf, _trdrop_MirFunction);
    return out;
}

__attribute__((hot)) List_ptr* mir_if_drop_plan(HirFunction* hf) {
#line 1309 "src/mir.tr"
    List_ptr* out = (void*)List_ptr_new();
#line 1310 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1311 "src/mir.tr"
    if ((!mf->complete)) {
#line 1311 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1312 "src/mir.tr"
    if ((mf->if_bodies->len == 0LL)) {
#line 1312 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1313 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1314 "src/mir.tr"
    List_ptr* drops = compute_drops(mf, live);
#line 1320 "src/mir.tr"
    long long bi = 0LL;
#line 1321 "src/mir.tr"
    while ((bi < mf->blocks->len)) {
#line 1322 "src/mir.tr"
        MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
#line 1323 "src/mir.tr"
        if (((((unsigned long long)(blk->hir_block)) != ((unsigned long long)(0LL))) && is_if_body(mf, blk->hir_block))) {
#line 1324 "src/mir.tr"
            LiveSet* dd = ((LiveSet*)List_ptr_get(drops, blk->id));
#line 1325 "src/mir.tr"
            long long pi = 0LL;
#line 1326 "src/mir.tr"
            while ((pi < dd->items->len)) {
#line 1327 "src/mir.tr"
                DropSite* site = site_for(out, blk->hir_block);
#line 1328 "src/mir.tr"
                ({ __auto_type _ord_t695 = (site->places); TrStr _at_t694 = (List_TrStr_get(dd->items, pi)); set_add(_ord_t695, _at_t694); _tr_str_release(_at_t694); });
#line 1329 "src/mir.tr"
                pi = (pi + 1LL);
            }
        }
#line 1330 "src/mir.tr"
        bi = (bi + 1LL);
    }
#line 1331 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_ptr_free_obj(drops, _trdrop_LiveSet);
    return out;
}

__attribute__((hot)) TrStr set_str(List_TrStr* v) {
#line 1334 "src/mir.tr"
    TrStr s = _tr_str_lit_len("{", 1LL);
#line 1335 "src/mir.tr"
    long long i = 0LL;
#line 1336 "src/mir.tr"
    while ((i < v->len)) {
#line 1337 "src/mir.tr"
        if ((i > 0LL)) {
#line 1337 "src/mir.tr"
            TrStr _strtmp_t697 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t697;
        }
#line 1338 "src/mir.tr"
        TrStr _strtmp_t699 = ({ TrStr _tr_cr = (List_TrStr_get(v, i)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t699;
#line 1339 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1340 "src/mir.tr"
    TrStr _trret166 = _tr_strx_concatv((s), (_tr_str_lit_len("}", 1LL)));
    _tr_str_release(s);
    return _trret166;
}

__attribute__((hot)) TrStr term_str(MirTerm* t) {
#line 1344 "src/mir.tr"
    __auto_type _t700 = (*t);
    if (_t700.tag == MirTerm_TGoto) {
        __auto_type n = _t700.data.TGoto.target;
        return ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("goto bb", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    } else if (_t700.tag == MirTerm_TBranch) {
        __auto_type tt = _t700.data.TBranch.t;
__auto_type ee = _t700.data.TBranch.e;
        return ({ __auto_type _ord_t701 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(tt)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("branch <cond> ? bb", 18LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" : bb", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t701); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ee)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
    } else if (_t700.tag == MirTerm_TReturn) {
        __auto_type _ = _t700.data.TReturn.value;
        return _tr_str_lit_len("return <expr>", 13LL);
    } else if (_t700.tag == MirTerm_TReturnVoid) {
        return _tr_str_lit_len("return", 6LL);
    } else if (_t700.tag == MirTerm_TUnset) {
        return _tr_str_lit_len("<unset>", 7LL);
    }
#line 1350 "src/mir.tr"
    return _tr_str_lit_len("<?>", 3LL);
}

__attribute__((hot)) TrStr stmt_str(MirStmt* s) {
#line 1353 "src/mir.tr"
    __auto_type _t702 = (*s);
    if (_t702.tag == MirStmt_MDeclare) {
        __auto_type p = _t702.data.MDeclare.place;
        TrStr _trret167 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("let ", 4LL)), (p))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = <expr>", 9LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret167;
    } else if (_t702.tag == MirStmt_MAssign) {
        __auto_type p = _t702.data.MAssign.place;
        TrStr _trret168 = _tr_strx_concatv((p), (_tr_str_lit_len(" = <expr>", 9LL)));
        return _trret168;
    } else if (_t702.tag == MirStmt_MEval) {
        __auto_type _ = _t702.data.MEval.value;
        return _tr_str_lit_len("eval <expr>", 11LL);
    }
#line 1357 "src/mir.tr"
    return _tr_str_lit_len("<?>", 3LL);
}

__attribute__((hot)) TrStr dump_mir(MirProgram* mp) {
#line 1360 "src/mir.tr"
    TrStr out = _tr_str_lit_len("", 0LL);
#line 1361 "src/mir.tr"
    long long fi = 0LL;
#line 1362 "src/mir.tr"
    while ((fi < mp->functions->len)) {
#line 1363 "src/mir.tr"
        MirFunction* f = ((MirFunction*)List_ptr_get(mp->functions, fi));
#line 1364 "src/mir.tr"
        TrStr tag = _tr_str_lit_len(" [complete]", 11LL);
#line 1365 "src/mir.tr"
        if ((!f->complete)) {
#line 1365 "src/mir.tr"
            TrStr _strtmp_t703 = _tr_str_lit_len(" [INCOMPLETE - falls back to HIR]", 33LL);
            _tr_str_release(tag);
            tag = _strtmp_t703;
        }
#line 1366 "src/mir.tr"
        ({ TrStr _ap_t704 = (_tr_str_lit_len("fn ", 3LL)); TrStr _ap_t705 = (f->name); TrStr _ap_t706 = (_tr_str_lit_len("(", 1LL)); TrStr _ap_t707 = (_tr_str_wrap(_tr_int_to_str((long long)(f->params->len)))); TrStr _ap_t708 = (_tr_str_lit_len(" params), ", 10LL)); TrStr _ap_t709 = (_tr_str_wrap(_tr_int_to_str((long long)(f->blocks->len)))); TrStr _ap_t710 = (_tr_str_lit_len(" blocks", 7LL)); TrStr _ap_t711 = (tag); TrStr _ap_t712 = (_tr_str_lit_len(":\n", 2LL)); _tr_str_append(&out, _ap_t704); _tr_str_append(&out, _ap_t705); _tr_str_append(&out, _ap_t706); _tr_str_append(&out, _ap_t707); _tr_str_append(&out, _ap_t708); _tr_str_append(&out, _ap_t709); _tr_str_append(&out, _ap_t710); _tr_str_append(&out, _ap_t711); _tr_str_append(&out, _ap_t712); _tr_str_release(_ap_t707); _tr_str_release(_ap_t709); });
#line 1367 "src/mir.tr"
        List_ptr* live = compute_liveness(f);
#line 1368 "src/mir.tr"
        List_ptr* drops = compute_drops(f, live);
#line 1370 "src/mir.tr"
        if ((f->borrows->len > 0LL)) {
#line 1371 "src/mir.tr"
            List_bool* outl = compute_borrow_outlives(f, live);
#line 1372 "src/mir.tr"
            long long ei = 0LL;
#line 1373 "src/mir.tr"
            while ((ei < f->borrows->len)) {
#line 1374 "src/mir.tr"
                BorrowEdge* ed = ((BorrowEdge*)List_ptr_get(f->borrows, ei));
#line 1375 "src/mir.tr"
                TrStr verdict = _tr_str_lit_len("NEEDS-EXTENSION", 15LL);
#line 1376 "src/mir.tr"
                if (List_bool_get(outl, ei)) {
#line 1376 "src/mir.tr"
                    TrStr _strtmp_t713 = _tr_str_lit_len("PROVEN", 6LL);
                    _tr_str_release(verdict);
                    verdict = _strtmp_t713;
                }
#line 1377 "src/mir.tr"
                ({ TrStr _ap_t714 = (_tr_str_lit_len("  ; borrow ", 11LL)); TrStr _ap_t715 = (ed->borrower); TrStr _ap_t716 = (_tr_str_lit_len(" <- ", 4LL)); TrStr _ap_t717 = (ed->source); TrStr _ap_t718 = (_tr_str_lit_len(" : ", 3LL)); TrStr _ap_t719 = (verdict); TrStr _ap_t720 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t714); _tr_str_append(&out, _ap_t715); _tr_str_append(&out, _ap_t716); _tr_str_append(&out, _ap_t717); _tr_str_append(&out, _ap_t718); _tr_str_append(&out, _ap_t719); _tr_str_append(&out, _ap_t720); });
#line 1378 "src/mir.tr"
                ei = (ei + 1LL);
                _tr_str_release(verdict);
            }
        }
#line 1379 "src/mir.tr"
        long long bi = 0LL;
#line 1380 "src/mir.tr"
        while ((bi < f->blocks->len)) {
#line 1381 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(f->blocks, bi));
#line 1382 "src/mir.tr"
            ({ TrStr _ap_t721 = (_tr_str_lit_len("  bb", 4LL)); TrStr _ap_t722 = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _ap_t723 = (_tr_str_lit_len(":\n", 2LL)); _tr_str_append(&out, _ap_t721); _tr_str_append(&out, _ap_t722); _tr_str_append(&out, _ap_t723); _tr_str_release(_ap_t722); });
#line 1383 "src/mir.tr"
            long long si = 0LL;
#line 1384 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1385 "src/mir.tr"
                ({ TrStr _ap_t724 = (_tr_str_lit_len("    ", 4LL)); TrStr _ap_t725 = (stmt_str(((MirStmt*)List_ptr_get(blk->stmts, si)))); TrStr _ap_t726 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t724); _tr_str_append(&out, _ap_t725); _tr_str_append(&out, _ap_t726); _tr_str_release(_ap_t725); });
#line 1386 "src/mir.tr"
                si = (si + 1LL);
            }
#line 1387 "src/mir.tr"
            ({ TrStr _ap_t727 = (_tr_str_lit_len("    ", 4LL)); TrStr _ap_t728 = (term_str(blk->term)); TrStr _ap_t729 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t727); _tr_str_append(&out, _ap_t728); _tr_str_append(&out, _ap_t729); _tr_str_release(_ap_t728); });
#line 1388 "src/mir.tr"
            TrStr _strtmp_t733 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((out), (_tr_str_lit_len("    ; live_out ", 15LL)))); TrStr _tr_cr = (set_str(((LiveSet*)List_ptr_get(live, blk->id))->items)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(out);
            out = _strtmp_t733;
#line 1389 "src/mir.tr"
            ({ TrStr _ap_t734 = (_tr_str_lit_len("    ; drop ", 11LL)); TrStr _ap_t735 = (set_str(((LiveSet*)List_ptr_get(drops, blk->id))->items)); TrStr _ap_t736 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t734); _tr_str_append(&out, _ap_t735); _tr_str_append(&out, _ap_t736); _tr_str_release(_ap_t735); });
#line 1390 "src/mir.tr"
            bi = (bi + 1LL);
        }
#line 1391 "src/mir.tr"
        fi = (fi + 1LL);
        _tr_str_release(tag);
        List_ptr_free_obj(drops, _trdrop_LiveSet);
    }
#line 1392 "src/mir.tr"
    TrStr _trret169 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret169;
}

