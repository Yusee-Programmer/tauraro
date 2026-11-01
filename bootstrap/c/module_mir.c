#include "tauraro_types.h"

TrStr _pl_root(HirExpr* e);
TrStr _pl_path(HirExpr* e);
bool _pl_overlap(TrStr a, TrStr b);
bool _is_mutating_call_on(HirExpr* val, TrStr source, TrMap* mm, TrStr bpath);
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
bool _pwrite_hits(List_ptr* pws, long long blk, long long idx, TrStr root, TrStr bpath);
List_bool* _reach_from(MirFunction* mf, long long start);
long long _decl_index(MirBlock* blk, TrStr name);
bool borrower_live_after(MirBlock* blk, long long after_idx, TrStr name, LiveSet* lo);
TrStr _b3_root_ident(HirExpr* e);
bool _b3_is_plain_ident(HirExpr* e);
bool _b3_nested_mut_call(HirExpr* e, TrStr pn, TrMap* mm);
void _b3_block(HirBlock* b, TrStr pn, TrMap* mm, List_TrStr* out);
void _b3_stmt(HirStmt* s, TrStr pn, TrMap* mm, List_TrStr* out);
TrStr set_str(List_TrStr* v);
TrStr term_str(MirTerm* t);
TrStr stmt_str(MirStmt* s);

__attribute__((malloc,returns_nonnull,hot)) MirProgram* MirProgram_init() {
#line 292 "src/mir.tr"
    MirProgram* p = ((MirProgram*)_tr_obj_alloc(sizeof(MirProgram)));
#line 293 "src/mir.tr"
    p->functions = (void*)List_ptr_new();
#line 294 "src/mir.tr"
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) MirBuilder* MirBuilder_init() {
#line 201 "src/mir.tr"
    MirBuilder* b = ((MirBuilder*)_tr_obj_alloc(sizeof(MirBuilder)));
#line 202 "src/mir.tr"
    b->blocks = (void*)List_ptr_new();
#line 203 "src/mir.tr"
    b->cur = (-1LL);
#line 204 "src/mir.tr"
    b->complete = true;
#line 205 "src/mir.tr"
    b->if_bodies = (void*)List_ptr_new();
#line 206 "src/mir.tr"
    b->in_unsafe = 0LL;
#line 207 "src/mir.tr"
    LiveSet* _cltmp_t626 = LiveSet_init();
    _tr_obj_release(b->unsafe_pinned, _trdrop_LiveSet);
    b->unsafe_pinned = _cltmp_t626;
#line 208 "src/mir.tr"
    b->loop_continue = (void*)List_i64_new();
#line 209 "src/mir.tr"
    b->loop_break = (void*)List_i64_new();
#line 210 "src/mir.tr"
    b->borrows = (void*)List_ptr_new();
#line 211 "src/mir.tr"
    b->pwrites = (void*)List_ptr_new();
#line 212 "src/mir.tr"
    return b;
}

__attribute__((hot)) void MirBuilder_record_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
#line 216 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 217 "src/mir.tr"
    e->borrower = _tr_str_retain(borrower);
#line 218 "src/mir.tr"
    e->source = _tr_str_retain(source);
#line 219 "src/mir.tr"
    e->decl_block = self->cur;
#line 220 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 221 "src/mir.tr"
    e->path = _tr_str_lit_len("", 0LL);
#line 222 "src/mir.tr"
    e->via_collection = false;
#line 223 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) void MirBuilder_record_coll_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
#line 227 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 228 "src/mir.tr"
    e->borrower = _tr_str_retain(borrower);
#line 229 "src/mir.tr"
    e->source = _tr_str_retain(source);
#line 230 "src/mir.tr"
    e->decl_block = self->cur;
#line 231 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 232 "src/mir.tr"
    e->path = _tr_str_lit_len("", 0LL);
#line 233 "src/mir.tr"
    e->via_collection = true;
#line 234 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) void MirBuilder_record_path_borrow(MirBuilder* self, TrStr borrower, TrStr source, TrStr path, bool exclusive) {
#line 240 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 241 "src/mir.tr"
    e->borrower = _tr_str_retain(borrower);
#line 242 "src/mir.tr"
    e->source = _tr_str_retain(source);
#line 243 "src/mir.tr"
    e->decl_block = self->cur;
#line 244 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 245 "src/mir.tr"
    e->path = _tr_str_retain(path);
#line 246 "src/mir.tr"
    e->via_collection = true;
#line 247 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) void MirBuilder_record_pwrite(MirBuilder* self, TrStr root, TrStr path) {
#line 251 "src/mir.tr"
    if ((self->cur < 0LL)) {
#line 251 "src/mir.tr"
        return;
    }
#line 252 "src/mir.tr"
    PWrite* w = ((PWrite*)_tr_obj_alloc(sizeof(PWrite)));
#line 253 "src/mir.tr"
    w->block = self->cur;
#line 254 "src/mir.tr"
    w->idx = (((MirBlock*)List_ptr_get(self->blocks, self->cur))->stmts->len - 1LL);
#line 255 "src/mir.tr"
    w->root = _tr_str_retain(root);
#line 256 "src/mir.tr"
    w->path = _tr_str_retain(path);
#line 257 "src/mir.tr"
    List_ptr_append(self->pwrites, _tr_obj_retain(w));
    _tr_obj_release(w, _trdrop_PWrite);
}

__attribute__((hot)) long long MirBuilder_new_block(MirBuilder* self) {
#line 261 "src/mir.tr"
    MirBlock* blk = ((MirBlock*)_tr_obj_alloc(sizeof(MirBlock)));
#line 262 "src/mir.tr"
    blk->id = self->blocks->len;
#line 263 "src/mir.tr"
    blk->stmts = (void*)List_ptr_new();
#line 264 "src/mir.tr"
    blk->term = box_mirterm(MirTerm_make_TUnset());
#line 265 "src/mir.tr"
    List_ptr_append(self->blocks, _tr_obj_retain(blk));
#line 266 "src/mir.tr"
    long long _trret168 = blk->id;
    _tr_obj_release(blk, _trdrop_MirBlock);
    return _trret168;
}

__attribute__((hot)) void MirBuilder_push_stmt(MirBuilder* self, MirStmt s) {
#line 269 "src/mir.tr"
    if ((self->cur >= 0LL)) {
#line 270 "src/mir.tr"
        MirBlock* blk = ((MirBlock*)List_ptr_get(self->blocks, self->cur));
#line 271 "src/mir.tr"
        ({ __auto_type _ord_t627 = (box_mirstmt(s)); List_ptr_append(blk->stmts, _ord_t627); });
#line 275 "src/mir.tr"
        if ((((unsigned long long)(blk->hir_block)) == ((unsigned long long)(0LL)))) {
#line 276 "src/mir.tr"
            HirBlock* _cltmp_t628 = _tr_obj_retain(self->cur_hb);
            _tr_obj_release(blk->hir_block, _trdrop_HirBlock);
            blk->hir_block = _cltmp_t628;
        }
    }
}

__attribute__((hot)) void MirBuilder_set_term(MirBuilder* self, MirTerm t) {
#line 279 "src/mir.tr"
    if ((self->cur >= 0LL)) {
#line 280 "src/mir.tr"
        __auto_type _asg_t629 = box_mirterm(t);
        ((MirBlock*)List_ptr_get(self->blocks, self->cur))->term = _asg_t629;
    }
}

__attribute__((hot)) bool MirBuilder_terminated(MirBuilder* self) {
#line 285 "src/mir.tr"
    if ((self->cur < 0LL)) {
#line 285 "src/mir.tr"
        return true;
    }
#line 286 "src/mir.tr"
    __auto_type _t630 = (*((MirBlock*)List_ptr_get(self->blocks, self->cur))->term);
    if (_t630.tag == MirTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t630;
        return true;
    }
}

__attribute__((malloc,returns_nonnull,hot)) LiveSet* LiveSet_init() {
#line 776 "src/mir.tr"
    LiveSet* s = ((LiveSet*)_tr_obj_alloc(sizeof(LiveSet)));
#line 777 "src/mir.tr"
    s->items = (void*)List_TrStr_new();
#line 778 "src/mir.tr"
    return s;
}

__attribute__((hot)) bool LiveSet_has(LiveSet* self, TrStr s) {
#line 781 "src/mir.tr"
    long long i = 0LL;
#line 782 "src/mir.tr"
    while ((i < self->items->len)) {
#line 783 "src/mir.tr"
        if (({ TrStr _cmpa_t631 = (List_TrStr_get(self->items, i)); TrStr _cmpb_t632 = (s); _Bool _cmpr_t633 = _tr_str_eqv(_cmpa_t631, _cmpb_t632); _tr_str_release(_cmpa_t631); _cmpr_t633; })) {
#line 783 "src/mir.tr"
            return true;
        }
#line 784 "src/mir.tr"
        i = (i + 1LL);
    }
#line 785 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool LiveSet_add(LiveSet* self, TrStr s) {
#line 788 "src/mir.tr"
    if (LiveSet_has(self, s)) {
#line 788 "src/mir.tr"
        return false;
    }
#line 789 "src/mir.tr"
    List_TrStr_append(self->items, s);
#line 790 "src/mir.tr"
    return true;
}

__attribute__((hot)) bool LiveSet_union_in(LiveSet* self, LiveSet* other) {
#line 794 "src/mir.tr"
    bool changed = false;
#line 795 "src/mir.tr"
    long long i = 0LL;
#line 796 "src/mir.tr"
    while ((i < other->items->len)) {
#line 797 "src/mir.tr"
        if (({ TrStr _at_t634 = (List_TrStr_get(other->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t634)); _tr_str_release(_at_t634); _wr; })) {
#line 797 "src/mir.tr"
            changed = true;
        }
#line 798 "src/mir.tr"
        i = (i + 1LL);
    }
#line 799 "src/mir.tr"
    return changed;
}

__attribute__((hot)) bool LiveSet_union_except(LiveSet* self, LiveSet* src, LiveSet* block) {
#line 803 "src/mir.tr"
    bool changed = false;
#line 804 "src/mir.tr"
    long long i = 0LL;
#line 805 "src/mir.tr"
    while ((i < src->items->len)) {
#line 806 "src/mir.tr"
        if (({ TrStr _at_t635 = (List_TrStr_get(src->items, i)); __auto_type _wr = ((!LiveSet_has(block, _at_t635))); _tr_str_release(_at_t635); _wr; })) {
#line 807 "src/mir.tr"
            if (({ TrStr _at_t636 = (List_TrStr_get(src->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t636)); _tr_str_release(_at_t636); _wr; })) {
#line 807 "src/mir.tr"
                changed = true;
            }
        }
#line 808 "src/mir.tr"
        i = (i + 1LL);
    }
#line 809 "src/mir.tr"
    return changed;
}

__attribute__((hot)) LiveSet* LiveSet_clone(LiveSet* self) {
#line 812 "src/mir.tr"
    LiveSet* r = LiveSet_init();
#line 813 "src/mir.tr"
    long long i = 0LL;
#line 814 "src/mir.tr"
    while ((i < self->items->len)) {
#line 815 "src/mir.tr"
        ({ TrStr _ord_t637 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _ord_t637); _tr_str_release(_ord_t637); });
#line 816 "src/mir.tr"
        i = (i + 1LL);
    }
#line 817 "src/mir.tr"
    return r;
}

__attribute__((hot)) void LiveSet_retain_common(LiveSet* self, LiveSet* other) {
#line 821 "src/mir.tr"
    List_TrStr* keep = (void*)List_TrStr_new();
#line 822 "src/mir.tr"
    long long i = 0LL;
#line 823 "src/mir.tr"
    while ((i < self->items->len)) {
#line 824 "src/mir.tr"
        if (({ TrStr _at_t638 = (List_TrStr_get(self->items, i)); __auto_type _wr = (LiveSet_has(other, _at_t638)); _tr_str_release(_at_t638); _wr; })) {
#line 824 "src/mir.tr"
            ({ TrStr _at_t639 = (List_TrStr_get(self->items, i)); List_TrStr_append(keep, _at_t639); _tr_str_release(_at_t639); });
        }
#line 825 "src/mir.tr"
        i = (i + 1LL);
    }
#line 826 "src/mir.tr"
    self->items = keep;
}

__attribute__((hot)) bool LiveSet_equals(LiveSet* self, LiveSet* other) {
#line 829 "src/mir.tr"
    if ((self->items->len != other->items->len)) {
#line 829 "src/mir.tr"
        return false;
    }
#line 830 "src/mir.tr"
    long long i = 0LL;
#line 831 "src/mir.tr"
    while ((i < self->items->len)) {
#line 832 "src/mir.tr"
        if (({ TrStr _at_t640 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t640))); _tr_str_release(_at_t640); _wr; })) {
#line 832 "src/mir.tr"
            return false;
        }
#line 833 "src/mir.tr"
        i = (i + 1LL);
    }
#line 834 "src/mir.tr"
    return true;
}

__attribute__((hot)) void LiveSet_set_to(LiveSet* self, LiveSet* other) {
#line 839 "src/mir.tr"
    List_TrStr* fresh = (void*)List_TrStr_new();
#line 840 "src/mir.tr"
    long long i = 0LL;
#line 841 "src/mir.tr"
    while ((i < other->items->len)) {
#line 842 "src/mir.tr"
        ({ TrStr _at_t641 = (List_TrStr_get(other->items, i)); List_TrStr_append(fresh, _at_t641); _tr_str_release(_at_t641); });
#line 843 "src/mir.tr"
        i = (i + 1LL);
    }
#line 844 "src/mir.tr"
    self->items = fresh;
}

__attribute__((hot)) LiveSet* LiveSet_minus(LiveSet* self, LiveSet* other) {
#line 848 "src/mir.tr"
    LiveSet* r = LiveSet_init();
#line 849 "src/mir.tr"
    long long i = 0LL;
#line 850 "src/mir.tr"
    while ((i < self->items->len)) {
#line 851 "src/mir.tr"
        if (({ TrStr _at_t642 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t642))); _tr_str_release(_at_t642); _wr; })) {
#line 851 "src/mir.tr"
            ({ TrStr _ord_t643 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _ord_t643); _tr_str_release(_ord_t643); });
        }
#line 852 "src/mir.tr"
        i = (i + 1LL);
    }
#line 853 "src/mir.tr"
    return r;
}

__attribute__((hot)) TrStr _pl_root(HirExpr* e) {
#line 28 "src/mir.tr"
    HirExpr* cur = e;
#line 29 "src/mir.tr"
    long long guard = 0LL;
#line 30 "src/mir.tr"
    while (((((unsigned long long)(cur)) != ((unsigned long long)(0LL))) && (guard < 64LL))) {
#line 31 "src/mir.tr"
        __auto_type _t644 = (*cur);
        if (_t644.tag == HirExpr_EIdent) {
            __auto_type n = _t644.data.EIdent.name;
            TrStr _trret163 = _tr_str_retain(n);
            return _trret163;
        } else if (_t644.tag == HirExpr_EPropAccess) {
            __auto_type o = _t644.data.EPropAccess.obj;
            cur = o;
        } else if (_t644.tag == HirExpr_EIndex) {
            __auto_type o = _t644.data.EIndex.obj;
            cur = o;
        } else if (1) {
            __auto_type _ = _t644;
            return _tr_str_lit_len("", 0LL);
        }
#line 36 "src/mir.tr"
        guard = (guard + 1LL);
    }
#line 37 "src/mir.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _pl_path(HirExpr* e) {
#line 40 "src/mir.tr"
    List_TrStr* segs = (void*)List_TrStr_new();
#line 41 "src/mir.tr"
    HirExpr* cur = e;
#line 42 "src/mir.tr"
    long long guard = 0LL;
#line 43 "src/mir.tr"
    while (((((unsigned long long)(cur)) != ((unsigned long long)(0LL))) && (guard < 64LL))) {
#line 44 "src/mir.tr"
        __auto_type _t645 = (*cur);
        if (_t645.tag == HirExpr_EPropAccess) {
            __auto_type o = _t645.data.EPropAccess.obj;
__auto_type prop = _t645.data.EPropAccess.prop;
#line 46 "src/mir.tr"
            List_TrStr_append(segs, prop);
#line 47 "src/mir.tr"
            cur = o;
        } else if (_t645.tag == HirExpr_EIndex) {
            __auto_type o = _t645.data.EIndex.obj;
#line 49 "src/mir.tr"
            List_TrStr_append(segs, _tr_str_lit_len("[]", 2LL));
#line 50 "src/mir.tr"
            cur = o;
        } else if (1) {
            __auto_type _ = _t645;
            cur = ((HirExpr*)(0LL));
        }
#line 52 "src/mir.tr"
        guard = (guard + 1LL);
    }
#line 53 "src/mir.tr"
    TrStr out = _tr_str_lit_len("", 0LL);
#line 54 "src/mir.tr"
    long long i = (segs->len - 1LL);
#line 55 "src/mir.tr"
    while ((i >= 0LL)) {
#line 56 "src/mir.tr"
        if ((!_tr_str_eqv((out), (_tr_str_lit_len("", 0LL))))) {
#line 56 "src/mir.tr"
            ({ TrStr _ap_t646 = (_tr_str_lit_len(".", 1LL)); _tr_str_append(&out, _ap_t646); });
        }
#line 57 "src/mir.tr"
        ({ TrStr _ap_t647 = (List_TrStr_get(segs, i)); _tr_str_append(&out, _ap_t647); _tr_str_release(_ap_t647); });
#line 58 "src/mir.tr"
        i = (i - 1LL);
    }
#line 59 "src/mir.tr"
    TrStr _trret164 = _tr_str_retain(out);
    List_TrStr_free(segs);
    _tr_str_release(out);
    return _trret164;
}

__attribute__((hot)) bool _pl_overlap(TrStr a, TrStr b) {
#line 64 "src/mir.tr"
    if ((_tr_str_eqv((a), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((b), (_tr_str_lit_len("", 0LL))))) {
#line 64 "src/mir.tr"
        return true;
    }
#line 65 "src/mir.tr"
    if (_tr_str_eqv((a), (b))) {
#line 65 "src/mir.tr"
        return true;
    }
#line 66 "src/mir.tr"
    if (_tr_str_starts_withv((a), (_tr_strx_concatv((b), (_tr_str_lit_len(".", 1LL)))))) {
#line 66 "src/mir.tr"
        return true;
    }
#line 67 "src/mir.tr"
    if (_tr_str_starts_withv((b), (_tr_strx_concatv((a), (_tr_str_lit_len(".", 1LL)))))) {
#line 67 "src/mir.tr"
        return true;
    }
#line 68 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool _is_mutating_call_on(HirExpr* val, TrStr source, TrMap* mm, TrStr bpath) {
#line 76 "src/mir.tr"
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 76 "src/mir.tr"
        return false;
    }
#line 77 "src/mir.tr"
    __auto_type _t648 = (*val);
    if (_t648.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t648.data.EMethodCall.obj;
__auto_type method = _t648.data.EMethodCall.method;
#line 79 "src/mir.tr"
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 79 "src/mir.tr"
            return false;
        }
#line 80 "src/mir.tr"
        __auto_type _t649 = (*obj);
        if (_t649.tag == HirExpr_EIdent) {
            __auto_type onm = _t649.data.EIdent.name;
#line 82 "src/mir.tr"
            if ((!_tr_str_eqv((onm), (source)))) {
#line 82 "src/mir.tr"
                return false;
            }
#line 84 "src/mir.tr"
            if ((((((((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("put", 3LL))))) {
#line 85 "src/mir.tr"
                return true;
            }
#line 86 "src/mir.tr"
            bool _trret165 = ({ TrStr _dkt_t650 = (({ TrStr _tr_cl = (_tr_strx_concatv((hir_expr_type(obj)->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t650))); _tr_str_release(_dkt_t650); _wr; });
            return _trret165;
        } else if (_t649.tag == HirExpr_EPropAccess) {
#line 88 "src/mir.tr"
            if (({ TrStr _cmpa_t651 = (_pl_root(obj)); TrStr _cmpb_t652 = (source); _Bool _cmpr_t653 = !_tr_str_eqv(_cmpa_t651, _cmpb_t652); _tr_str_release(_cmpa_t651); _cmpr_t653; })) {
#line 88 "src/mir.tr"
                return false;
            }
#line 89 "src/mir.tr"
            if (({ TrStr _at_t654 = (_pl_path(obj)); __auto_type _wr = ((!_pl_overlap(_at_t654, bpath))); _tr_str_release(_at_t654); _wr; })) {
#line 89 "src/mir.tr"
                return false;
            }
#line 90 "src/mir.tr"
            TrStr tn = _tr_str_retain(hir_expr_type(obj)->name);
#line 91 "src/mir.tr"
            if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 92 "src/mir.tr"
                bool _trret166 = (((((((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))));
                _tr_str_release(tn);
                return _trret166;
            }
#line 93 "src/mir.tr"
            bool _trret167 = ({ TrStr _dkt_t655 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t655))); _tr_str_release(_dkt_t655); _wr; });
            _tr_str_release(tn);
            return _trret167;
        } else if (1) {
            __auto_type _ = _t649;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t648;
        return false;
    }
}

__attribute__((hot)) MirStmt* box_mirstmt(MirStmt s) {
#line 112 "src/mir.tr"
    /* unsafe block */
#line 113 "src/mir.tr"
    MirStmt* p = ((MirStmt*)_tr_c_calloc((size_t)(1LL), sizeof(MirStmt)));
#line 114 "src/mir.tr"
    (*p = s);
#line 115 "src/mir.tr"
    return p;
}

__attribute__((hot)) MirTerm* box_mirterm(MirTerm t) {
#line 118 "src/mir.tr"
    /* unsafe block */
#line 119 "src/mir.tr"
    MirTerm* p = ((MirTerm*)_tr_c_calloc((size_t)(1LL), sizeof(MirTerm)));
#line 120 "src/mir.tr"
    (*p = t);
#line 121 "src/mir.tr"
    return p;
}

__attribute__((hot)) void lower_stmts(MirBuilder* b, HirBlock* hb) {
#line 298 "src/mir.tr"
    HirBlock* saved_hb = _tr_obj_retain(b->cur_hb);
#line 299 "src/mir.tr"
    HirBlock* _cltmp_t656 = _tr_obj_retain(hb);
    _tr_obj_release(b->cur_hb, _trdrop_HirBlock);
    b->cur_hb = _cltmp_t656;
#line 300 "src/mir.tr"
    long long i = 0LL;
#line 301 "src/mir.tr"
    while ((i < hb->stmts->len)) {
#line 302 "src/mir.tr"
        if (MirBuilder_terminated(b)) {
#line 305 "src/mir.tr"
            b->cur = MirBuilder_new_block(b);
        }
#line 306 "src/mir.tr"
        HirStmt* sp = ((HirStmt*)List_ptr_get(hb->stmts, i));
#line 307 "src/mir.tr"
        __auto_type _t657 = (*sp);
        if (_t657.tag == HirStmt_SLet) {
            __auto_type nm = _t657.data.SLet.name;
__auto_type slet_ty = _t657.data.SLet.ty;
__auto_type val = _t657.data.SLet.val;
#line 309 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MDeclare(nm, val));
#line 313 "src/mir.tr"
            if ((slet_ty->is_borrow && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 314 "src/mir.tr"
                bool _slet_mut = false;
#line 315 "src/mir.tr"
                __auto_type _t658 = (*sp);
                if (_t658.tag == HirStmt_SLet) {
                    __auto_type _sm = _t658.data.SLet.is_mut;
                    _slet_mut = _sm;
                } else if (1) {
                    __auto_type _ = _t658;
                    /* pass */
                }
#line 318 "src/mir.tr"
                __auto_type _t659 = (*val);
                if (_t659.tag == HirExpr_EIdent) {
                    __auto_type src_nm = _t659.data.EIdent.name;
                    MirBuilder_record_borrow(b, nm, src_nm, (_slet_mut || slet_ty->is_mut_borrow));
                } else if (_t659.tag == HirExpr_EMethodCall) {
                    __auto_type mobj = _t659.data.EMethodCall.obj;
__auto_type mmethod = _t659.data.EMethodCall.method;
#line 325 "src/mir.tr"
                    if ((_tr_str_eqv((mmethod), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((mmethod), (_tr_str_lit_len("get_or", 6LL))))) {
#line 326 "src/mir.tr"
                        __auto_type _t660 = (*mobj);
                        if (_t660.tag == HirExpr_EIdent) {
                            __auto_type cnm = _t660.data.EIdent.name;
                            MirBuilder_record_coll_borrow(b, nm, cnm, (_slet_mut || slet_ty->is_mut_borrow));
                        } else if (1) {
                            __auto_type _ = _t660;
                            /* pass */
                        }
                    }
                } else if (_t659.tag == HirExpr_EPropAccess) {
#line 331 "src/mir.tr"
                    TrStr pb_root = _pl_root(val);
#line 332 "src/mir.tr"
                    if ((!_tr_str_eqv((pb_root), (_tr_str_lit_len("", 0LL))))) {
#line 332 "src/mir.tr"
                        ({ TrStr _ord_t661 = (_pl_path(val)); __auto_type _ord_t662 = ((_slet_mut || slet_ty->is_mut_borrow)); MirBuilder_record_path_borrow(b, nm, pb_root, _ord_t661, _ord_t662); _tr_str_release(_ord_t661); });
                    }
                    _tr_str_release(pb_root);
                } else if (_t659.tag == HirExpr_EIndex) {
                    __auto_type iobj = _t659.data.EIndex.obj;
#line 335 "src/mir.tr"
                    __auto_type _t663 = (*iobj);
                    if (_t663.tag == HirExpr_EIdent) {
                        __auto_type cnm2 = _t663.data.EIdent.name;
                        MirBuilder_record_coll_borrow(b, nm, cnm2, (_slet_mut || slet_ty->is_mut_borrow));
                    } else if (1) {
                        __auto_type _ = _t663;
                        /* pass */
                    }
                } else if (1) {
                    __auto_type _ = _t659;
                    /* pass */
                }
            }
        } else if (_t657.tag == HirStmt_SAssign) {
            __auto_type tgt = _t657.data.SAssign.target;
__auto_type val = _t657.data.SAssign.val;
#line 342 "src/mir.tr"
            __auto_type _t664 = (*tgt);
            if (_t664.tag == HirExpr_EIdent) {
                __auto_type tn = _t664.data.EIdent.name;
                MirBuilder_push_stmt(b, MirStmt_ctor_MAssign(tn, val));
            } else if (1) {
                __auto_type _ = _t664;
#line 345 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(val));
#line 346 "src/mir.tr"
                TrStr pw_root = _pl_root(tgt);
#line 347 "src/mir.tr"
                if ((!_tr_str_eqv((pw_root), (_tr_str_lit_len("", 0LL))))) {
#line 347 "src/mir.tr"
                    ({ TrStr _at_t665 = (_pl_path(tgt)); MirBuilder_record_pwrite(b, pw_root, _at_t665); _tr_str_release(_at_t665); });
                }
                _tr_str_release(pw_root);
            }
        } else if (_t657.tag == HirStmt_SExpr) {
            __auto_type e = _t657.data.SExpr.expr;
#line 349 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(e));
        } else if (_t657.tag == HirStmt_SReturn) {
            __auto_type v = _t657.data.SReturn.val;
#line 351 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TReturn(v));
        } else if (_t657.tag == HirStmt_SIf) {
            __auto_type cond = _t657.data.SIf.cond;
__auto_type then_b = _t657.data.SIf.then_b;
__auto_type else_b = _t657.data.SIf.else_b;
#line 353 "src/mir.tr"
            long long tb = MirBuilder_new_block(b);
#line 354 "src/mir.tr"
            long long eb = MirBuilder_new_block(b);
#line 355 "src/mir.tr"
            long long jb = MirBuilder_new_block(b);
#line 359 "src/mir.tr"
            if ((b->in_unsafe == 0LL)) {
#line 360 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(then_b));
#line 361 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(else_b));
            }
#line 362 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, tb, eb));
#line 363 "src/mir.tr"
            b->cur = tb;
#line 364 "src/mir.tr"
            lower_stmts(b, then_b);
#line 365 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 365 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
#line 366 "src/mir.tr"
            b->cur = eb;
#line 367 "src/mir.tr"
            lower_stmts(b, else_b);
#line 368 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 368 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
#line 369 "src/mir.tr"
            b->cur = jb;
        } else if (_t657.tag == HirStmt_SWhile) {
            __auto_type cond = _t657.data.SWhile.cond;
__auto_type body = _t657.data.SWhile.body;
#line 371 "src/mir.tr"
            long long hdr = MirBuilder_new_block(b);
#line 372 "src/mir.tr"
            long long body_b = MirBuilder_new_block(b);
#line 373 "src/mir.tr"
            long long exit_b = MirBuilder_new_block(b);
#line 374 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
#line 375 "src/mir.tr"
            b->cur = hdr;
#line 376 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, body_b, exit_b));
#line 377 "src/mir.tr"
            b->cur = body_b;
#line 378 "src/mir.tr"
            List_i64_append(b->loop_continue, hdr);
#line 379 "src/mir.tr"
            List_i64_append(b->loop_break, exit_b);
#line 380 "src/mir.tr"
            lower_stmts(b, body);
#line 381 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 382 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 383 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 383 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
            }
#line 384 "src/mir.tr"
            b->cur = exit_b;
        } else if (_t657.tag == HirStmt_SFor) {
            __auto_type fiter = _t657.data.SFor.iter;
__auto_type fbody = _t657.data.SFor.body;
#line 391 "src/mir.tr"
            long long for_hdr = MirBuilder_new_block(b);
#line 392 "src/mir.tr"
            long long for_body = MirBuilder_new_block(b);
#line 393 "src/mir.tr"
            long long for_exit = MirBuilder_new_block(b);
#line 394 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
#line 395 "src/mir.tr"
            b->cur = for_hdr;
#line 396 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fiter, for_body, for_exit));
#line 397 "src/mir.tr"
            b->cur = for_body;
#line 398 "src/mir.tr"
            List_i64_append(b->loop_continue, for_hdr);
#line 399 "src/mir.tr"
            List_i64_append(b->loop_break, for_exit);
#line 400 "src/mir.tr"
            lower_stmts(b, fbody);
#line 401 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 402 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 403 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 403 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
            }
#line 404 "src/mir.tr"
            b->cur = for_exit;
        } else if (_t657.tag == HirStmt_SForUnpack) {
            __auto_type fuiter = _t657.data.SForUnpack.iter;
__auto_type fubody = _t657.data.SForUnpack.body;
#line 406 "src/mir.tr"
            long long fu_hdr = MirBuilder_new_block(b);
#line 407 "src/mir.tr"
            long long fu_body = MirBuilder_new_block(b);
#line 408 "src/mir.tr"
            long long fu_exit = MirBuilder_new_block(b);
#line 409 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
#line 410 "src/mir.tr"
            b->cur = fu_hdr;
#line 411 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fuiter, fu_body, fu_exit));
#line 412 "src/mir.tr"
            b->cur = fu_body;
#line 413 "src/mir.tr"
            List_i64_append(b->loop_continue, fu_hdr);
#line 414 "src/mir.tr"
            List_i64_append(b->loop_break, fu_exit);
#line 415 "src/mir.tr"
            lower_stmts(b, fubody);
#line 416 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 417 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 418 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 418 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
            }
#line 419 "src/mir.tr"
            b->cur = fu_exit;
        } else if (_t657.tag == HirStmt_STry) {
            __auto_type try_body = _t657.data.STry.try_body;
__auto_type catches = _t657.data.STry.catches;
__auto_type finally_b = _t657.data.STry.finally_b;
#line 427 "src/mir.tr"
            long long try_b = MirBuilder_new_block(b);
#line 428 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 428 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_b));
            }
#line 429 "src/mir.tr"
            b->cur = try_b;
#line 430 "src/mir.tr"
            List_ptr_append(b->if_bodies, _tr_obj_retain(try_body));
#line 431 "src/mir.tr"
            lower_stmts(b, try_body);
#line 432 "src/mir.tr"
            long long tci = 0LL;
#line 433 "src/mir.tr"
            while ((tci < catches->len)) {
#line 434 "src/mir.tr"
                HirCatchClause* tcc = (*((HirCatchClause**)List_ptr_get(catches, tci)));
#line 435 "src/mir.tr"
                long long catch_b = MirBuilder_new_block(b);
#line 436 "src/mir.tr"
                if ((!MirBuilder_terminated(b))) {
#line 436 "src/mir.tr"
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(catch_b));
                }
#line 437 "src/mir.tr"
                b->cur = catch_b;
#line 438 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(tcc->body));
#line 439 "src/mir.tr"
                lower_stmts(b, tcc->body);
#line 440 "src/mir.tr"
                tci = (tci + 1LL);
            }
#line 441 "src/mir.tr"
            long long fin_b = MirBuilder_new_block(b);
#line 442 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 442 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fin_b));
            }
#line 443 "src/mir.tr"
            b->cur = fin_b;
#line 444 "src/mir.tr"
            List_ptr_append(b->if_bodies, _tr_obj_retain(finally_b));
#line 445 "src/mir.tr"
            lower_stmts(b, finally_b);
#line 446 "src/mir.tr"
            long long try_after = MirBuilder_new_block(b);
#line 447 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 447 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_after));
            }
#line 448 "src/mir.tr"
            b->cur = try_after;
        } else if (_t657.tag == HirStmt_SMatch) {
            __auto_type msubj = _t657.data.SMatch.expr;
__auto_type marms = _t657.data.SMatch.arms;
#line 455 "src/mir.tr"
            long long m_exit = MirBuilder_new_block(b);
#line 456 "src/mir.tr"
            long long mai = 0LL;
#line 457 "src/mir.tr"
            while ((mai < marms->len)) {
#line 458 "src/mir.tr"
                HirMatchArm* marm = ((HirMatchArm*)List_ptr_get(marms, mai));
#line 459 "src/mir.tr"
                long long arm_body = MirBuilder_new_block(b);
#line 460 "src/mir.tr"
                long long arm_next = MirBuilder_new_block(b);
#line 461 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TBranch(msubj, arm_body, arm_next));
#line 462 "src/mir.tr"
                b->cur = arm_body;
#line 463 "src/mir.tr"
                if ((((unsigned long long)(marm->guard)) != ((unsigned long long)(0LL)))) {
#line 464 "src/mir.tr"
                    MirBuilder_push_stmt(b, MirStmt_ctor_MEval(marm->guard));
                }
#line 465 "src/mir.tr"
                if ((b->in_unsafe == 0LL)) {
#line 465 "src/mir.tr"
                    List_ptr_append(b->if_bodies, _tr_obj_retain(marm->body));
                }
#line 466 "src/mir.tr"
                lower_stmts(b, marm->body);
#line 467 "src/mir.tr"
                if ((!MirBuilder_terminated(b))) {
#line 467 "src/mir.tr"
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
                }
#line 468 "src/mir.tr"
                b->cur = arm_next;
#line 469 "src/mir.tr"
                mai = (mai + 1LL);
            }
#line 470 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 470 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
            }
#line 471 "src/mir.tr"
            b->cur = m_exit;
        } else if (_t657.tag == HirStmt_SAssert) {
            __auto_type acond = _t657.data.SAssert.cond;
__auto_type amsg = _t657.data.SAssert.msg;
#line 473 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(acond));
#line 474 "src/mir.tr"
            if ((((unsigned long long)(amsg)) != ((unsigned long long)(0LL)))) {
#line 475 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(amsg));
            }
        } else if (_t657.tag == HirStmt_SUnsafe) {
            __auto_type ubody = _t657.data.SUnsafe.body;
#line 495 "src/mir.tr"
            b->in_unsafe = (b->in_unsafe + 1LL);
#line 496 "src/mir.tr"
            lower_stmts(b, ubody);
#line 497 "src/mir.tr"
            b->in_unsafe = (b->in_unsafe - 1LL);
        } else if (_t657.tag == HirStmt_SRaise) {
            __auto_type rval = _t657.data.SRaise.val;
#line 502 "src/mir.tr"
            if ((((unsigned long long)(rval)) != ((unsigned long long)(0LL)))) {
#line 503 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(rval));
            }
        } else if (_t657.tag == HirStmt_SWith) {
            __auto_type witems = _t657.data.SWith.items;
__auto_type wbody = _t657.data.SWith.body;
#line 508 "src/mir.tr"
            long long wi = 0LL;
#line 509 "src/mir.tr"
            while ((wi < witems->len)) {
#line 510 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(((HirExpr*)List_ptr_get(witems, wi))));
#line 511 "src/mir.tr"
                wi = (wi + 1LL);
            }
#line 512 "src/mir.tr"
            lower_stmts(b, wbody);
        } else if (_t657.tag == HirStmt_SMultiLet) {
            __auto_type mlval = _t657.data.SMultiLet.val;
#line 517 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(mlval));
        } else if (_t657.tag == HirStmt_SLineMarker) {
            __auto_type _ = _t657.data.SLineMarker.n;
#line 519 "src/mir.tr"
            /* pass */
        } else if (_t657.tag == HirStmt_SAutoDrop) {
#line 521 "src/mir.tr"
            /* pass */
        } else if (_t657.tag == HirStmt_SBreak) {
            __auto_type _ = _t657.data.SBreak.val;
#line 529 "src/mir.tr"
            if ((b->loop_break->len > 0LL)) {
#line 530 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_break, (b->loop_break->len - 1LL))));
            } else {
#line 532 "src/mir.tr"
                b->complete = false;
            }
        } else if (_t657.tag == HirStmt_SContinue) {
#line 536 "src/mir.tr"
            if ((b->loop_continue->len > 0LL)) {
#line 537 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_continue, (b->loop_continue->len - 1LL))));
            } else {
#line 539 "src/mir.tr"
                b->complete = false;
            }
        } else if (1) {
            __auto_type _ = _t657;
#line 544 "src/mir.tr"
            b->complete = false;
        }
#line 545 "src/mir.tr"
        i = (i + 1LL);
    }
#line 546 "src/mir.tr"
    HirBlock* _cltmp_t666 = _tr_obj_retain(saved_hb);
    _tr_obj_release(b->cur_hb, _trdrop_HirBlock);
    b->cur_hb = _cltmp_t666;
    _tr_obj_release(saved_hb, _trdrop_HirBlock);
}

__attribute__((hot)) MirFunction* lower_function(HirFunction* hf) {
#line 549 "src/mir.tr"
    MirFunction* mf = ((MirFunction*)_tr_obj_alloc(sizeof(MirFunction)));
#line 550 "src/mir.tr"
    mf->name = _tr_str_retain(hf->name);
#line 551 "src/mir.tr"
    mf->params = (void*)List_TrStr_new();
#line 552 "src/mir.tr"
    long long pi = 0LL;
#line 553 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 554 "src/mir.tr"
        ({ __auto_type _ord_t667 = (((HirParam*)List_ptr_get(hf->params, pi))->name); List_TrStr_append(mf->params, _ord_t667); });
#line 555 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 556 "src/mir.tr"
    MirBuilder* b = MirBuilder_init();
#line 557 "src/mir.tr"
    b->cur = MirBuilder_new_block(b);
#line 558 "src/mir.tr"
    lower_stmts(b, hf->body);
#line 559 "src/mir.tr"
    if ((!MirBuilder_terminated(b))) {
#line 559 "src/mir.tr"
        MirBuilder_set_term(b, MirTerm_make_TReturnVoid());
    }
#line 566 "src/mir.tr"
    long long rbi = 0LL;
#line 567 "src/mir.tr"
    while ((rbi < b->blocks->len)) {
#line 568 "src/mir.tr"
        MirBlock* rblk = ((MirBlock*)List_ptr_get(b->blocks, rbi));
#line 569 "src/mir.tr"
        long long rsi = 0LL;
#line 570 "src/mir.tr"
        while ((rsi < rblk->stmts->len)) {
#line 571 "src/mir.tr"
            __auto_type _t668 = (*((MirStmt*)List_ptr_get(rblk->stmts, rsi)));
            if (_t668.tag == MirStmt_MDeclare) {
                __auto_type rv = _t668.data.MDeclare.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t668.tag == MirStmt_MAssign) {
                __auto_type rv = _t668.data.MAssign.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t668.tag == MirStmt_MEval) {
                __auto_type rv = _t668.data.MEval.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            }
#line 575 "src/mir.tr"
            rsi = (rsi + 1LL);
        }
#line 576 "src/mir.tr"
        __auto_type _t669 = (*rblk->term);
        if (_t669.tag == MirTerm_TBranch) {
            __auto_type rc = _t669.data.TBranch.cond;
            collect_raw_borrows(rc, b->unsafe_pinned);
        } else if (_t669.tag == MirTerm_TReturn) {
            __auto_type rv = _t669.data.TReturn.value;
            collect_raw_borrows(rv, b->unsafe_pinned);
        } else if (1) {
            __auto_type _ = _t669;
            /* pass */
        }
#line 580 "src/mir.tr"
        rbi = (rbi + 1LL);
    }
#line 581 "src/mir.tr"
    mf->blocks = b->blocks;
#line 582 "src/mir.tr"
    mf->complete = b->complete;
#line 583 "src/mir.tr"
    mf->if_bodies = b->if_bodies;
#line 584 "src/mir.tr"
    LiveSet* _cltmp_t670 = _tr_obj_retain(b->unsafe_pinned);
    _tr_obj_release(mf->unsafe_pinned, _trdrop_LiveSet);
    mf->unsafe_pinned = _cltmp_t670;
#line 585 "src/mir.tr"
    mf->borrows = b->borrows;
#line 586 "src/mir.tr"
    mf->pwrites = b->pwrites;
#line 590 "src/mir.tr"
    long long hbi = 0LL;
#line 591 "src/mir.tr"
    if ((((unsigned long long)(hf->borrow_borrowers)) == ((unsigned long long)(0LL)))) {
#line 591 "src/mir.tr"
        _tr_obj_release(b, _trdrop_MirBuilder);
        return mf;
    }
#line 592 "src/mir.tr"
    if ((((unsigned long long)(hf->borrow_sources)) == ((unsigned long long)(0LL)))) {
#line 592 "src/mir.tr"
        _tr_obj_release(b, _trdrop_MirBuilder);
        return mf;
    }
#line 593 "src/mir.tr"
    while (((hbi < hf->borrow_borrowers->len) && (hbi < hf->borrow_sources->len))) {
#line 594 "src/mir.tr"
        bool he_dup = false;
#line 595 "src/mir.tr"
        long long hdi = 0LL;
#line 596 "src/mir.tr"
        while ((hdi < mf->borrows->len)) {
#line 597 "src/mir.tr"
            if ((({ __auto_type _ord_t671 = (((BorrowEdge*)List_ptr_get(mf->borrows, hdi))->borrower); __auto_type _wr = (({ TrStr _cmpa_t672 = (_ord_t671); TrStr _cmpb_t673 = (List_TrStr_get(hf->borrow_borrowers, hbi)); _Bool _cmpr_t674 = _tr_str_eqv(_cmpa_t672, _cmpb_t673); _tr_str_release(_cmpb_t673); _cmpr_t674; })); _wr; }) && ({ __auto_type _ord_t675 = (((BorrowEdge*)List_ptr_get(mf->borrows, hdi))->source); __auto_type _wr = (({ TrStr _cmpa_t676 = (_ord_t675); TrStr _cmpb_t677 = (List_TrStr_get(hf->borrow_sources, hbi)); _Bool _cmpr_t678 = _tr_str_eqv(_cmpa_t676, _cmpb_t677); _tr_str_release(_cmpb_t677); _cmpr_t678; })); _wr; }))) {
#line 597 "src/mir.tr"
                he_dup = true;
            }
#line 598 "src/mir.tr"
            hdi = (hdi + 1LL);
        }
#line 599 "src/mir.tr"
        if (he_dup) {
#line 600 "src/mir.tr"
            hbi = (hbi + 1LL);
#line 601 "src/mir.tr"
            continue;
        }
#line 602 "src/mir.tr"
        BorrowEdge* he = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 603 "src/mir.tr"
        he->borrower = List_TrStr_get(hf->borrow_borrowers, hbi);
#line 604 "src/mir.tr"
        he->source = List_TrStr_get(hf->borrow_sources, hbi);
#line 605 "src/mir.tr"
        he->path = _tr_str_lit_len("", 0LL);
#line 606 "src/mir.tr"
        he->decl_block = (-1LL);
#line 608 "src/mir.tr"
        List_ptr_append(mf->borrows, _tr_obj_retain(he));
#line 609 "src/mir.tr"
        hbi = (hbi + 1LL);
        _tr_obj_release(he, _trdrop_BorrowEdge);
    }
#line 610 "src/mir.tr"
    _tr_obj_release(b, _trdrop_MirBuilder);
    return mf;
}

__attribute__((hot)) MirProgram* lower_program(HirProgram* hir) {
#line 613 "src/mir.tr"
    MirProgram* mp = MirProgram_init();
#line 614 "src/mir.tr"
    long long i = 0LL;
#line 615 "src/mir.tr"
    while ((i < hir->functions->len)) {
#line 616 "src/mir.tr"
        ({ MirFunction* _ord_t679 = (lower_function(((HirFunction*)List_ptr_get(hir->functions, i)))); List_ptr_append(mp->functions, _tr_obj_retain(_ord_t679)); _tr_obj_release(_ord_t679, _trdrop_MirFunction); });
#line 617 "src/mir.tr"
        i = (i + 1LL);
    }
#line 618 "src/mir.tr"
    return mp;
}

__attribute__((hot)) bool set_contains(List_TrStr* v, TrStr s) {
#line 628 "src/mir.tr"
    long long i = 0LL;
#line 629 "src/mir.tr"
    while ((i < v->len)) {
#line 630 "src/mir.tr"
        if (({ TrStr _cmpa_t680 = (List_TrStr_get(v, i)); TrStr _cmpb_t681 = (s); _Bool _cmpr_t682 = _tr_str_eqv(_cmpa_t680, _cmpb_t681); _tr_str_release(_cmpa_t680); _cmpr_t682; })) {
#line 630 "src/mir.tr"
            return true;
        }
#line 631 "src/mir.tr"
        i = (i + 1LL);
    }
#line 632 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool set_add(List_TrStr* v, TrStr s) {
#line 635 "src/mir.tr"
    if (set_contains(v, s)) {
#line 635 "src/mir.tr"
        return false;
    }
#line 636 "src/mir.tr"
    List_TrStr_append(v, s);
#line 637 "src/mir.tr"
    return true;
}

__attribute__((hot)) void collect_uses(HirExpr* e, List_TrStr* out) {
#line 641 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 641 "src/mir.tr"
        return;
    }
#line 642 "src/mir.tr"
    __auto_type _t683 = (*e);
    if (_t683.tag == HirExpr_EIdent) {
        __auto_type nm = _t683.data.EIdent.name;
        set_add(out, nm);
    } else if (_t683.tag == HirExpr_EBinOp) {
        __auto_type l = _t683.data.EBinOp.left;
__auto_type r = _t683.data.EBinOp.right;
#line 645 "src/mir.tr"
        collect_uses(l, out);
#line 646 "src/mir.tr"
        collect_uses(r, out);
    } else if (_t683.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t683.data.EUnaryOp.expr;
        collect_uses(x, out);
    } else if (_t683.tag == HirExpr_ECall) {
        __auto_type callee = _t683.data.ECall.callee;
__auto_type args = _t683.data.ECall.args;
#line 649 "src/mir.tr"
        collect_uses(callee, out);
#line 650 "src/mir.tr"
        long long i = 0LL;
#line 651 "src/mir.tr"
        while ((i < args->len)) {
#line 652 "src/mir.tr"
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
#line 653 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t683.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t683.data.EMethodCall.obj;
__auto_type args = _t683.data.EMethodCall.args;
#line 655 "src/mir.tr"
        collect_uses(obj, out);
#line 656 "src/mir.tr"
        long long i = 0LL;
#line 657 "src/mir.tr"
        while ((i < args->len)) {
#line 658 "src/mir.tr"
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
#line 659 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t683.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t683.data.EPropAccess.obj;
        collect_uses(obj, out);
    } else if (_t683.tag == HirExpr_EIndex) {
        __auto_type obj = _t683.data.EIndex.obj;
__auto_type idx = _t683.data.EIndex._tr_v_index;
#line 662 "src/mir.tr"
        collect_uses(obj, out);
#line 663 "src/mir.tr"
        collect_uses(idx, out);
    } else if (_t683.tag == HirExpr_ECast) {
        __auto_type x = _t683.data.ECast.expr;
        collect_uses(x, out);
    } else if (_t683.tag == HirExpr_EAwait) {
        __auto_type x = _t683.data.EAwait.expr;
        collect_uses(x, out);
    } else if (_t683.tag == HirExpr_EIfElse) {
        __auto_type c = _t683.data.EIfElse.cond;
__auto_type t = _t683.data.EIfElse.then_e;
__auto_type f = _t683.data.EIfElse.else_e;
#line 667 "src/mir.tr"
        collect_uses(c, out);
#line 668 "src/mir.tr"
        collect_uses(t, out);
#line 669 "src/mir.tr"
        collect_uses(f, out);
    } else if (1) {
        __auto_type _ = _t683;
        /* pass */
    }
}

__attribute__((hot)) void collect_raw_borrows(HirExpr* e, LiveSet* out) {
#line 684 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 684 "src/mir.tr"
        return;
    }
#line 685 "src/mir.tr"
    __auto_type _t684 = (*e);
    if (_t684.tag == HirExpr_ECast) {
        __auto_type inner = _t684.data.ECast.expr;
__auto_type ty = _t684.data.ECast.target_ty;
#line 687 "src/mir.tr"
        if (_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 688 "src/mir.tr"
            __auto_type _t685 = (*inner);
            if (_t685.tag == HirExpr_EIdent) {
                __auto_type nm = _t685.data.EIdent.name;
                bool _a = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t685;
                /* pass */
            }
        }
#line 691 "src/mir.tr"
        collect_raw_borrows(inner, out);
    } else if (_t684.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t684.data.EUnaryOp.op;
__auto_type inner = _t684.data.EUnaryOp.expr;
#line 693 "src/mir.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 694 "src/mir.tr"
            __auto_type _t686 = (*inner);
            if (_t686.tag == HirExpr_EIdent) {
                __auto_type nm = _t686.data.EIdent.name;
                bool _b = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t686;
                /* pass */
            }
        }
#line 697 "src/mir.tr"
        collect_raw_borrows(inner, out);
    } else if (_t684.tag == HirExpr_EBinOp) {
        __auto_type l = _t684.data.EBinOp.left;
__auto_type r = _t684.data.EBinOp.right;
#line 699 "src/mir.tr"
        collect_raw_borrows(l, out);
#line 700 "src/mir.tr"
        collect_raw_borrows(r, out);
    } else if (_t684.tag == HirExpr_ECall) {
        __auto_type callee = _t684.data.ECall.callee;
__auto_type args = _t684.data.ECall.args;
#line 702 "src/mir.tr"
        collect_raw_borrows(callee, out);
#line 703 "src/mir.tr"
        long long i = 0LL;
#line 704 "src/mir.tr"
        while ((i < args->len)) {
#line 705 "src/mir.tr"
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
#line 706 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t684.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t684.data.EMethodCall.obj;
__auto_type args = _t684.data.EMethodCall.args;
#line 708 "src/mir.tr"
        collect_raw_borrows(obj, out);
#line 709 "src/mir.tr"
        long long i = 0LL;
#line 710 "src/mir.tr"
        while ((i < args->len)) {
#line 711 "src/mir.tr"
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
#line 712 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t684.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t684.data.EPropAccess.obj;
        collect_raw_borrows(obj, out);
    } else if (_t684.tag == HirExpr_EIndex) {
        __auto_type obj = _t684.data.EIndex.obj;
__auto_type idx = _t684.data.EIndex._tr_v_index;
#line 715 "src/mir.tr"
        collect_raw_borrows(obj, out);
#line 716 "src/mir.tr"
        collect_raw_borrows(idx, out);
    } else if (_t684.tag == HirExpr_EAwait) {
        __auto_type x = _t684.data.EAwait.expr;
        collect_raw_borrows(x, out);
    } else if (_t684.tag == HirExpr_EIfElse) {
        __auto_type c = _t684.data.EIfElse.cond;
__auto_type t = _t684.data.EIfElse.then_e;
__auto_type f = _t684.data.EIfElse.else_e;
#line 719 "src/mir.tr"
        collect_raw_borrows(c, out);
#line 720 "src/mir.tr"
        collect_raw_borrows(t, out);
#line 721 "src/mir.tr"
        collect_raw_borrows(f, out);
    } else if (1) {
        __auto_type _ = _t684;
        /* pass */
    }
}

__attribute__((hot)) void add_exposed(HirExpr* e, List_TrStr* gen, List_TrStr* kill) {
#line 729 "src/mir.tr"
    List_TrStr* tmp = (void*)List_TrStr_new();
#line 730 "src/mir.tr"
    collect_uses(e, tmp);
#line 731 "src/mir.tr"
    long long i = 0LL;
#line 732 "src/mir.tr"
    while ((i < tmp->len)) {
#line 733 "src/mir.tr"
        if (({ TrStr _at_t687 = (List_TrStr_get(tmp, i)); __auto_type _wr = ((!set_contains(kill, _at_t687))); _tr_str_release(_at_t687); _wr; })) {
#line 733 "src/mir.tr"
            ({ TrStr _at_t688 = (List_TrStr_get(tmp, i)); set_add(gen, _at_t688); _tr_str_release(_at_t688); });
        }
#line 734 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_use_def(MirBlock* blk, List_TrStr* gen, List_TrStr* kill) {
#line 739 "src/mir.tr"
    long long i = 0LL;
#line 740 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 741 "src/mir.tr"
        __auto_type _t689 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t689.tag == MirStmt_MDeclare) {
            __auto_type p = _t689.data.MDeclare.place;
__auto_type val = _t689.data.MDeclare.value;
#line 743 "src/mir.tr"
            add_exposed(val, gen, kill);
#line 744 "src/mir.tr"
            set_add(kill, p);
        } else if (_t689.tag == MirStmt_MAssign) {
            __auto_type p = _t689.data.MAssign.place;
__auto_type val = _t689.data.MAssign.value;
#line 746 "src/mir.tr"
            add_exposed(val, gen, kill);
#line 747 "src/mir.tr"
            set_add(kill, p);
        } else if (_t689.tag == MirStmt_MEval) {
            __auto_type val = _t689.data.MEval.value;
            add_exposed(val, gen, kill);
        }
#line 749 "src/mir.tr"
        i = (i + 1LL);
    }
#line 750 "src/mir.tr"
    __auto_type _t690 = (*blk->term);
    if (_t690.tag == MirTerm_TBranch) {
        __auto_type cond = _t690.data.TBranch.cond;
        add_exposed(cond, gen, kill);
    } else if (_t690.tag == MirTerm_TReturn) {
        __auto_type val = _t690.data.TReturn.value;
        add_exposed(val, gen, kill);
    } else if (1) {
        __auto_type _ = _t690;
        /* pass */
    }
}

__attribute__((hot)) void block_succs(MirBlock* blk, List_i64* out) {
#line 756 "src/mir.tr"
    __auto_type _t691 = (*blk->term);
    if (_t691.tag == MirTerm_TGoto) {
        __auto_type n = _t691.data.TGoto.target;
        List_i64_append(out, n);
    } else if (_t691.tag == MirTerm_TBranch) {
        __auto_type t = _t691.data.TBranch.t;
__auto_type e = _t691.data.TBranch.e;
#line 759 "src/mir.tr"
        List_i64_append(out, t);
#line 760 "src/mir.tr"
        List_i64_append(out, e);
    } else if (1) {
        __auto_type _ = _t691;
        /* pass */
    }
}

__attribute__((hot)) List_ptr* compute_liveness(MirFunction* mf) {
#line 860 "src/mir.tr"
    long long n = mf->blocks->len;
#line 861 "src/mir.tr"
    List_ptr* uses = (void*)List_ptr_new();
#line 862 "src/mir.tr"
    List_ptr* defs = (void*)List_ptr_new();
#line 863 "src/mir.tr"
    List_ptr* live_in = (void*)List_ptr_new();
#line 864 "src/mir.tr"
    List_ptr* live_out = (void*)List_ptr_new();
#line 865 "src/mir.tr"
    long long i = 0LL;
#line 866 "src/mir.tr"
    while ((i < n)) {
#line 867 "src/mir.tr"
        LiveSet* u = LiveSet_init();
#line 868 "src/mir.tr"
        LiveSet* d = LiveSet_init();
#line 869 "src/mir.tr"
        block_use_def(((MirBlock*)List_ptr_get(mf->blocks, i)), u->items, d->items);
#line 870 "src/mir.tr"
        List_ptr_append(uses, _tr_obj_retain(u));
#line 871 "src/mir.tr"
        List_ptr_append(defs, _tr_obj_retain(d));
#line 872 "src/mir.tr"
        List_ptr_append(live_in, LiveSet_init());
#line 873 "src/mir.tr"
        List_ptr_append(live_out, LiveSet_init());
#line 874 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(u, _trdrop_LiveSet);
        _tr_obj_release(d, _trdrop_LiveSet);
    }
#line 875 "src/mir.tr"
    bool changed = true;
#line 876 "src/mir.tr"
    while (changed) {
#line 877 "src/mir.tr"
        changed = false;
#line 878 "src/mir.tr"
        long long bi = (n - 1LL);
#line 879 "src/mir.tr"
        while ((bi >= 0LL)) {
#line 880 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
#line 881 "src/mir.tr"
            List_i64* sl = (void*)List_i64_new();
#line 882 "src/mir.tr"
            block_succs(blk, sl);
#line 883 "src/mir.tr"
            long long si = 0LL;
#line 884 "src/mir.tr"
            while ((si < sl->len)) {
#line 885 "src/mir.tr"
                if (({ __auto_type _ord_t692 = (((LiveSet*)List_ptr_get(live_out, bi))); __auto_type _ord_t693 = (((LiveSet*)List_ptr_get(live_in, List_i64_get(sl, si)))); __auto_type _wr = (LiveSet_union_in(_ord_t692, _ord_t693)); _wr; })) {
#line 885 "src/mir.tr"
                    changed = true;
                }
#line 886 "src/mir.tr"
                si = (si + 1LL);
            }
#line 887 "src/mir.tr"
            if (({ __auto_type _ord_t694 = (((LiveSet*)List_ptr_get(live_in, bi))); __auto_type _ord_t695 = (((LiveSet*)List_ptr_get(uses, bi))); __auto_type _wr = (LiveSet_union_in(_ord_t694, _ord_t695)); _wr; })) {
#line 887 "src/mir.tr"
                changed = true;
            }
#line 888 "src/mir.tr"
            if (({ __auto_type _ord_t696 = (((LiveSet*)List_ptr_get(live_in, bi))); __auto_type _ord_t697 = (((LiveSet*)List_ptr_get(live_out, bi))); __auto_type _ord_t698 = (((LiveSet*)List_ptr_get(defs, bi))); __auto_type _wr = (LiveSet_union_except(_ord_t696, _ord_t697, _ord_t698)); _wr; })) {
#line 888 "src/mir.tr"
                changed = true;
            }
#line 889 "src/mir.tr"
            bi = (bi - 1LL);
        }
    }
#line 890 "src/mir.tr"
    List_ptr_free_obj(uses, _trdrop_LiveSet);
    List_ptr_free_obj(defs, _trdrop_LiveSet);
    List_ptr_free_obj(live_in, _trdrop_LiveSet);
    return live_out;
}

__attribute__((hot)) void collect_moves(HirExpr* e, List_TrStr* out) {
#line 911 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 911 "src/mir.tr"
        return;
    }
#line 912 "src/mir.tr"
    __auto_type _t699 = (*e);
    if (_t699.tag == HirExpr_EIdent) {
        __auto_type nm = _t699.data.EIdent.name;
__auto_type ismv = _t699.data.EIdent.is_move;
#line 914 "src/mir.tr"
        if (ismv) {
#line 914 "src/mir.tr"
            set_add(out, nm);
        }
    } else if (_t699.tag == HirExpr_EBinOp) {
        __auto_type l = _t699.data.EBinOp.left;
__auto_type r = _t699.data.EBinOp.right;
#line 916 "src/mir.tr"
        collect_moves(l, out);
#line 917 "src/mir.tr"
        collect_moves(r, out);
    } else if (_t699.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t699.data.EUnaryOp.expr;
        collect_moves(x, out);
    } else if (_t699.tag == HirExpr_ECall) {
        __auto_type callee = _t699.data.ECall.callee;
__auto_type args = _t699.data.ECall.args;
#line 920 "src/mir.tr"
        collect_moves(callee, out);
#line 921 "src/mir.tr"
        long long i = 0LL;
#line 922 "src/mir.tr"
        while ((i < args->len)) {
#line 923 "src/mir.tr"
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
#line 924 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t699.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t699.data.EMethodCall.obj;
__auto_type args = _t699.data.EMethodCall.args;
#line 926 "src/mir.tr"
        collect_moves(obj, out);
#line 927 "src/mir.tr"
        long long i = 0LL;
#line 928 "src/mir.tr"
        while ((i < args->len)) {
#line 929 "src/mir.tr"
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
#line 930 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t699.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t699.data.EPropAccess.obj;
        collect_moves(obj, out);
    } else if (_t699.tag == HirExpr_EIndex) {
        __auto_type obj = _t699.data.EIndex.obj;
__auto_type idx = _t699.data.EIndex._tr_v_index;
#line 933 "src/mir.tr"
        collect_moves(obj, out);
#line 934 "src/mir.tr"
        collect_moves(idx, out);
    } else if (_t699.tag == HirExpr_ECast) {
        __auto_type x = _t699.data.ECast.expr;
        collect_moves(x, out);
    } else if (_t699.tag == HirExpr_EAwait) {
        __auto_type x = _t699.data.EAwait.expr;
        collect_moves(x, out);
    } else if (_t699.tag == HirExpr_EIfElse) {
        __auto_type c = _t699.data.EIfElse.cond;
__auto_type t = _t699.data.EIfElse.then_e;
__auto_type f = _t699.data.EIfElse.else_e;
#line 938 "src/mir.tr"
        collect_moves(c, out);
#line 939 "src/mir.tr"
        collect_moves(t, out);
#line 940 "src/mir.tr"
        collect_moves(f, out);
    } else if (1) {
        __auto_type _ = _t699;
        /* pass */
    }
}

__attribute__((hot)) bool is_free_fn(TrStr n) {
#line 945 "src/mir.tr"
    return (((((_tr_str_eqv((n), (_tr_str_lit_len("dealloc", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_c_free", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_free", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_str_release", 15LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_str_free", 12LL))));
}

__attribute__((hot)) TrStr free_target(HirExpr* arg) {
#line 950 "src/mir.tr"
    if ((((unsigned long long)(arg)) == ((unsigned long long)(0LL)))) {
#line 950 "src/mir.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 951 "src/mir.tr"
    __auto_type _t700 = (*arg);
    if (_t700.tag == HirExpr_EIdent) {
        __auto_type n = _t700.data.EIdent.name;
        TrStr _trret170 = _tr_str_retain(n);
        return _trret170;
    } else if (_t700.tag == HirExpr_ECast) {
        __auto_type x = _t700.data.ECast.expr;
        return free_target(x);
    } else if (1) {
        __auto_type _ = _t700;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) void collect_frees(HirExpr* e, List_TrStr* out) {
#line 964 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 964 "src/mir.tr"
        return;
    }
#line 965 "src/mir.tr"
    __auto_type _t701 = (*e);
    if (_t701.tag == HirExpr_ECall) {
        __auto_type callee = _t701.data.ECall.callee;
__auto_type args = _t701.data.ECall.args;
#line 967 "src/mir.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 968 "src/mir.tr"
            __auto_type _t702 = (*callee);
            if (_t702.tag == HirExpr_EIdent) {
                __auto_type cn = _t702.data.EIdent.name;
#line 970 "src/mir.tr"
                if ((is_free_fn(cn) && (args->len > 0LL))) {
#line 971 "src/mir.tr"
                    TrStr ft = free_target(((HirExpr*)List_ptr_get(args, 0LL)));
#line 972 "src/mir.tr"
                    if ((!_tr_str_eqv((ft), (_tr_str_lit_len("", 0LL))))) {
#line 972 "src/mir.tr"
                        set_add(out, ft);
                    }
                }
            } else if (1) {
                __auto_type _ = _t702;
                /* pass */
            }
        }
#line 974 "src/mir.tr"
        collect_frees(callee, out);
#line 975 "src/mir.tr"
        long long i = 0LL;
#line 976 "src/mir.tr"
        while ((i < args->len)) {
#line 977 "src/mir.tr"
            collect_frees(((HirExpr*)List_ptr_get(args, i)), out);
#line 978 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t701.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t701.data.EMethodCall.obj;
__auto_type m = _t701.data.EMethodCall.method;
__auto_type args = _t701.data.EMethodCall.args;
#line 980 "src/mir.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) {
#line 981 "src/mir.tr"
            __auto_type _t703 = (*obj);
            if (_t703.tag == HirExpr_EIdent) {
                __auto_type on = _t703.data.EIdent.name;
                set_add(out, on);
            } else if (1) {
                __auto_type _ = _t703;
                /* pass */
            }
        }
#line 984 "src/mir.tr"
        collect_frees(obj, out);
#line 985 "src/mir.tr"
        long long j = 0LL;
#line 986 "src/mir.tr"
        while ((j < args->len)) {
#line 987 "src/mir.tr"
            collect_frees(((HirExpr*)List_ptr_get(args, j)), out);
#line 988 "src/mir.tr"
            j = (j + 1LL);
        }
    } else if (_t701.tag == HirExpr_EBinOp) {
        __auto_type l = _t701.data.EBinOp.left;
__auto_type r = _t701.data.EBinOp.right;
#line 990 "src/mir.tr"
        collect_frees(l, out);
#line 991 "src/mir.tr"
        collect_frees(r, out);
    } else if (_t701.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t701.data.EUnaryOp.expr;
        collect_frees(x, out);
    } else if (_t701.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t701.data.EPropAccess.obj;
        collect_frees(obj, out);
    } else if (_t701.tag == HirExpr_EIndex) {
        __auto_type obj = _t701.data.EIndex.obj;
__auto_type idx = _t701.data.EIndex._tr_v_index;
#line 995 "src/mir.tr"
        collect_frees(obj, out);
#line 996 "src/mir.tr"
        collect_frees(idx, out);
    } else if (_t701.tag == HirExpr_ECast) {
        __auto_type x = _t701.data.ECast.expr;
        collect_frees(x, out);
    } else if (_t701.tag == HirExpr_EAwait) {
        __auto_type x = _t701.data.EAwait.expr;
        collect_frees(x, out);
    } else if (1) {
        __auto_type _ = _t701;
        /* pass */
    }
}

__attribute__((hot)) void block_gen_own(MirBlock* blk, List_TrStr* out) {
#line 1003 "src/mir.tr"
    long long i = 0LL;
#line 1004 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 1005 "src/mir.tr"
        __auto_type _t704 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t704.tag == MirStmt_MDeclare) {
            __auto_type p = _t704.data.MDeclare.place;
            set_add(out, p);
        } else if (_t704.tag == MirStmt_MAssign) {
            __auto_type p = _t704.data.MAssign.place;
            set_add(out, p);
        } else if (_t704.tag == MirStmt_MEval) {
            __auto_type _ = _t704.data.MEval.value;
            /* pass */
        }
#line 1009 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_moves(MirBlock* blk, List_TrStr* out) {
#line 1013 "src/mir.tr"
    long long i = 0LL;
#line 1014 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 1015 "src/mir.tr"
        __auto_type _t705 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t705.tag == MirStmt_MDeclare) {
            __auto_type val = _t705.data.MDeclare.value;
#line 1017 "src/mir.tr"
            collect_moves(val, out);
#line 1018 "src/mir.tr"
            collect_frees(val, out);
        } else if (_t705.tag == MirStmt_MAssign) {
            __auto_type val = _t705.data.MAssign.value;
#line 1020 "src/mir.tr"
            collect_moves(val, out);
#line 1021 "src/mir.tr"
            collect_frees(val, out);
        } else if (_t705.tag == MirStmt_MEval) {
            __auto_type val = _t705.data.MEval.value;
#line 1023 "src/mir.tr"
            collect_moves(val, out);
#line 1024 "src/mir.tr"
            collect_frees(val, out);
        }
#line 1025 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1026 "src/mir.tr"
    __auto_type _t706 = (*blk->term);
    if (_t706.tag == MirTerm_TBranch) {
        __auto_type cond = _t706.data.TBranch.cond;
        collect_moves(cond, out);
    } else if (_t706.tag == MirTerm_TReturn) {
        __auto_type val = _t706.data.TReturn.value;
#line 1029 "src/mir.tr"
        collect_moves(val, out);
#line 1033 "src/mir.tr"
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 1034 "src/mir.tr"
            __auto_type _t707 = (*val);
            if (_t707.tag == HirExpr_EIdent) {
                __auto_type rnm = _t707.data.EIdent.name;
                set_add(out, rnm);
            } else if (1) {
                __auto_type _ = _t707;
                /* pass */
            }
        }
    } else if (1) {
        __auto_type _ = _t706;
        /* pass */
    }
}

__attribute__((hot)) void preds_of(MirFunction* mf, long long b, List_i64* out) {
#line 1041 "src/mir.tr"
    long long i = 0LL;
#line 1042 "src/mir.tr"
    while ((i < mf->blocks->len)) {
#line 1043 "src/mir.tr"
        List_i64* sl = (void*)List_i64_new();
#line 1044 "src/mir.tr"
        block_succs(((MirBlock*)List_ptr_get(mf->blocks, i)), sl);
#line 1045 "src/mir.tr"
        long long j = 0LL;
#line 1046 "src/mir.tr"
        while ((j < sl->len)) {
#line 1047 "src/mir.tr"
            if ((List_i64_get(sl, j) == b)) {
#line 1047 "src/mir.tr"
                List_i64_append(out, i);
            }
#line 1048 "src/mir.tr"
            j = (j + 1LL);
        }
#line 1049 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_ptr* compute_drops(MirFunction* mf, List_ptr* live_out) {
#line 1053 "src/mir.tr"
    long long n = mf->blocks->len;
#line 1054 "src/mir.tr"
    List_ptr* gen = (void*)List_ptr_new();
#line 1055 "src/mir.tr"
    List_ptr* kill = (void*)List_ptr_new();
#line 1056 "src/mir.tr"
    List_ptr* own_out = (void*)List_ptr_new();
#line 1057 "src/mir.tr"
    LiveSet* universe = LiveSet_init();
#line 1058 "src/mir.tr"
    long long i = 0LL;
#line 1059 "src/mir.tr"
    while ((i < n)) {
#line 1060 "src/mir.tr"
        LiveSet* g = LiveSet_init();
#line 1061 "src/mir.tr"
        LiveSet* k = LiveSet_init();
#line 1062 "src/mir.tr"
        block_gen_own(((MirBlock*)List_ptr_get(mf->blocks, i)), g->items);
#line 1063 "src/mir.tr"
        block_moves(((MirBlock*)List_ptr_get(mf->blocks, i)), k->items);
#line 1064 "src/mir.tr"
        List_ptr_append(gen, _tr_obj_retain(g));
#line 1065 "src/mir.tr"
        List_ptr_append(kill, _tr_obj_retain(k));
#line 1066 "src/mir.tr"
        bool _u = LiveSet_union_in(universe, g);
#line 1067 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(g, _trdrop_LiveSet);
        _tr_obj_release(k, _trdrop_LiveSet);
    }
#line 1069 "src/mir.tr"
    i = 0LL;
#line 1070 "src/mir.tr"
    while ((i < n)) {
#line 1071 "src/mir.tr"
        List_ptr_append(own_out, LiveSet_clone(universe));
#line 1072 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1073 "src/mir.tr"
    bool changed = true;
#line 1074 "src/mir.tr"
    while (changed) {
#line 1075 "src/mir.tr"
        changed = false;
#line 1076 "src/mir.tr"
        long long b = 0LL;
#line 1077 "src/mir.tr"
        while ((b < n)) {
#line 1078 "src/mir.tr"
            List_i64* preds = (void*)List_i64_new();
#line 1079 "src/mir.tr"
            preds_of(mf, b, preds);
#line 1080 "src/mir.tr"
            LiveSet* own_in = LiveSet_init();
#line 1081 "src/mir.tr"
            if ((preds->len > 0LL)) {
#line 1082 "src/mir.tr"
                LiveSet* _cltmp_t708 = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, 0LL))));
                _tr_obj_release(own_in, _trdrop_LiveSet);
                own_in = _cltmp_t708;
#line 1083 "src/mir.tr"
                long long pi = 1LL;
#line 1084 "src/mir.tr"
                while ((pi < preds->len)) {
#line 1085 "src/mir.tr"
                    LiveSet_retain_common(own_in, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, pi))));
#line 1086 "src/mir.tr"
                    pi = (pi + 1LL);
                }
            }
#line 1087 "src/mir.tr"
            bool _g = LiveSet_union_in(own_in, ((LiveSet*)List_ptr_get(gen, b)));
#line 1091 "src/mir.tr"
            LiveSet* owned_b = LiveSet_minus(own_in, ((LiveSet*)List_ptr_get(kill, b)));
#line 1092 "src/mir.tr"
            LiveSet* new_oo = LiveSet_clone(owned_b);
#line 1093 "src/mir.tr"
            LiveSet_retain_common(new_oo, ((LiveSet*)List_ptr_get(live_out, b)));
#line 1094 "src/mir.tr"
            if ((!LiveSet_equals(new_oo, ((LiveSet*)List_ptr_get(own_out, b))))) {
#line 1095 "src/mir.tr"
                changed = true;
#line 1096 "src/mir.tr"
                LiveSet_set_to(((LiveSet*)List_ptr_get(own_out, b)), new_oo);
            }
#line 1097 "src/mir.tr"
            b = (b + 1LL);
            _tr_obj_release(own_in, _trdrop_LiveSet);
            _tr_obj_release(owned_b, _trdrop_LiveSet);
            _tr_obj_release(new_oo, _trdrop_LiveSet);
        }
    }
#line 1099 "src/mir.tr"
    List_ptr* drops = (void*)List_ptr_new();
#line 1100 "src/mir.tr"
    i = 0LL;
#line 1101 "src/mir.tr"
    while ((i < n)) {
#line 1102 "src/mir.tr"
        List_i64* preds2 = (void*)List_i64_new();
#line 1103 "src/mir.tr"
        preds_of(mf, i, preds2);
#line 1104 "src/mir.tr"
        LiveSet* own_in2 = LiveSet_init();
#line 1105 "src/mir.tr"
        if ((preds2->len > 0LL)) {
#line 1106 "src/mir.tr"
            LiveSet* _cltmp_t709 = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, 0LL))));
            _tr_obj_release(own_in2, _trdrop_LiveSet);
            own_in2 = _cltmp_t709;
#line 1107 "src/mir.tr"
            long long pj = 1LL;
#line 1108 "src/mir.tr"
            while ((pj < preds2->len)) {
#line 1109 "src/mir.tr"
                LiveSet_retain_common(own_in2, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, pj))));
#line 1110 "src/mir.tr"
                pj = (pj + 1LL);
            }
        }
#line 1111 "src/mir.tr"
        bool _g2 = LiveSet_union_in(own_in2, ((LiveSet*)List_ptr_get(gen, i)));
#line 1112 "src/mir.tr"
        LiveSet* owned_i = LiveSet_minus(own_in2, ((LiveSet*)List_ptr_get(kill, i)));
#line 1113 "src/mir.tr"
        LiveSet* dead_i = LiveSet_minus(owned_i, ((LiveSet*)List_ptr_get(live_out, i)));
#line 1114 "src/mir.tr"
        List_ptr_append(drops, LiveSet_minus(dead_i, mf->unsafe_pinned));
#line 1115 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(own_in2, _trdrop_LiveSet);
        _tr_obj_release(owned_i, _trdrop_LiveSet);
        _tr_obj_release(dead_i, _trdrop_LiveSet);
    }
#line 1116 "src/mir.tr"
    List_ptr_free_obj(gen, _trdrop_LiveSet);
    List_ptr_free_obj(kill, _trdrop_LiveSet);
    List_ptr_free_obj(own_out, _trdrop_LiveSet);
    _tr_obj_release(universe, _trdrop_LiveSet);
    return drops;
}

__attribute__((hot)) long long last_use_in_block(MirBlock* blk, TrStr name) {
#line 1133 "src/mir.tr"
    long long last = (-1LL);
#line 1134 "src/mir.tr"
    long long si = 0LL;
#line 1135 "src/mir.tr"
    while ((si < blk->stmts->len)) {
#line 1136 "src/mir.tr"
        List_TrStr* tmp = (void*)List_TrStr_new();
#line 1137 "src/mir.tr"
        __auto_type _t710 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t710.tag == MirStmt_MDeclare) {
            __auto_type val = _t710.data.MDeclare.value;
            collect_uses(val, tmp);
        } else if (_t710.tag == MirStmt_MAssign) {
            __auto_type val = _t710.data.MAssign.value;
            collect_uses(val, tmp);
        } else if (_t710.tag == MirStmt_MEval) {
            __auto_type val = _t710.data.MEval.value;
            collect_uses(val, tmp);
        }
#line 1141 "src/mir.tr"
        if (set_contains(tmp, name)) {
#line 1141 "src/mir.tr"
            last = si;
        }
#line 1142 "src/mir.tr"
        si = (si + 1LL);
    }
#line 1143 "src/mir.tr"
    List_TrStr* tt = (void*)List_TrStr_new();
#line 1144 "src/mir.tr"
    __auto_type _t711 = (*blk->term);
    if (_t711.tag == MirTerm_TBranch) {
        __auto_type cond = _t711.data.TBranch.cond;
        collect_uses(cond, tt);
    } else if (_t711.tag == MirTerm_TReturn) {
        __auto_type val = _t711.data.TReturn.value;
        collect_uses(val, tt);
    } else if (1) {
        __auto_type _ = _t711;
        /* pass */
    }
#line 1148 "src/mir.tr"
    if (set_contains(tt, name)) {
#line 1148 "src/mir.tr"
        last = blk->stmts->len;
    }
#line 1149 "src/mir.tr"
    return last;
}

__attribute__((hot)) List_bool* compute_borrow_outlives(MirFunction* mf, List_ptr* live_out) {
#line 1152 "src/mir.tr"
    List_bool* out = (void*)List_bool_new();
#line 1153 "src/mir.tr"
    long long i = 0LL;
#line 1154 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1155 "src/mir.tr"
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1156 "src/mir.tr"
        bool proven = true;
#line 1158 "src/mir.tr"
        long long bi = 0LL;
#line 1159 "src/mir.tr"
        while ((bi < mf->blocks->len)) {
#line 1160 "src/mir.tr"
            LiveSet* lo = ((LiveSet*)List_ptr_get(live_out, bi));
#line 1161 "src/mir.tr"
            if ((({ TrStr _apt_t712 = _tr_str_retain(edge->borrower); __auto_type _wr = (LiveSet_has(lo, _apt_t712)); _tr_str_release(_apt_t712); _wr; }) && ({ TrStr _apt_t713 = _tr_str_retain(edge->source); __auto_type _wr = ((!LiveSet_has(lo, _apt_t713))); _tr_str_release(_apt_t713); _wr; }))) {
#line 1162 "src/mir.tr"
                proven = false;
            }
#line 1163 "src/mir.tr"
            bi = (bi + 1LL);
        }
#line 1169 "src/mir.tr"
        long long db = edge->decl_block;
#line 1170 "src/mir.tr"
        if (((db >= 0LL) && (db < mf->blocks->len))) {
#line 1171 "src/mir.tr"
            if (({ __auto_type _ord_t714 = (((LiveSet*)List_ptr_get(live_out, db))); __auto_type _ord_t715 = (edge->source); __auto_type _wr = ((!LiveSet_has(_ord_t714, _ord_t715))); _wr; })) {
#line 1172 "src/mir.tr"
                MirBlock* dblk = ((MirBlock*)List_ptr_get(mf->blocks, db));
#line 1173 "src/mir.tr"
                long long src_last = ({ TrStr _apt_t716 = _tr_str_retain(edge->source); __auto_type _wr = (last_use_in_block(dblk, _apt_t716)); _tr_str_release(_apt_t716); _wr; });
#line 1174 "src/mir.tr"
                long long bor_last = ({ TrStr _apt_t717 = _tr_str_retain(edge->borrower); __auto_type _wr = (last_use_in_block(dblk, _apt_t717)); _tr_str_release(_apt_t717); _wr; });
#line 1175 "src/mir.tr"
                if (({ __auto_type _ord_t718 = (((LiveSet*)List_ptr_get(live_out, db))); __auto_type _ord_t719 = (edge->borrower); __auto_type _wr = (LiveSet_has(_ord_t718, _ord_t719)); _wr; })) {
#line 1176 "src/mir.tr"
                    bor_last = (dblk->stmts->len + 1LL);
                }
#line 1177 "src/mir.tr"
                if ((bor_last > src_last)) {
#line 1178 "src/mir.tr"
                    proven = false;
                }
            }
        }
#line 1179 "src/mir.tr"
        List_bool_append(out, proven);
#line 1180 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1181 "src/mir.tr"
    return out;
}

__attribute__((hot)) bool is_if_body(MirFunction* mf, HirBlock* hb) {
#line 1191 "src/mir.tr"
    long long i = 0LL;
#line 1192 "src/mir.tr"
    while ((i < mf->if_bodies->len)) {
#line 1193 "src/mir.tr"
        if ((((unsigned long long)(((HirBlock*)List_ptr_get(mf->if_bodies, i)))) == ((unsigned long long)(hb)))) {
#line 1193 "src/mir.tr"
            return true;
        }
#line 1194 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1195 "src/mir.tr"
    return false;
}

__attribute__((hot)) DropSite* site_for(List_ptr* out, HirBlock* hb) {
#line 1199 "src/mir.tr"
    long long i = 0LL;
#line 1200 "src/mir.tr"
    while ((i < out->len)) {
#line 1201 "src/mir.tr"
        if ((((unsigned long long)(((DropSite*)List_ptr_get(out, i))->hir_block)) == ((unsigned long long)(hb)))) {
#line 1201 "src/mir.tr"
            return _tr_obj_retain(((DropSite*)List_ptr_get(out, i)));
        }
#line 1202 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1203 "src/mir.tr"
    DropSite* s = ((DropSite*)_tr_obj_alloc(sizeof(DropSite)));
#line 1204 "src/mir.tr"
    HirBlock* _cltmp_t720 = _tr_obj_retain(hb);
    _tr_obj_release(s->hir_block, _trdrop_HirBlock);
    s->hir_block = _cltmp_t720;
#line 1205 "src/mir.tr"
    s->places = (void*)List_TrStr_new();
#line 1206 "src/mir.tr"
    List_ptr_append(out, _tr_obj_retain(s));
#line 1207 "src/mir.tr"
    return s;
}

__attribute__((hot)) List_TrStr* mir_proven_borrows(HirFunction* hf) {
#line 1215 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1216 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1217 "src/mir.tr"
    if ((!mf->complete)) {
#line 1217 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1218 "src/mir.tr"
    if ((mf->borrows->len == 0LL)) {
#line 1218 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1219 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1220 "src/mir.tr"
    List_bool* outl = compute_borrow_outlives(mf, live);
#line 1221 "src/mir.tr"
    long long i = 0LL;
#line 1222 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1223 "src/mir.tr"
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1227 "src/mir.tr"
        bool proven = List_bool_get(outl, i);
#line 1233 "src/mir.tr"
        if ((proven && edge->via_collection)) {
#line 1234 "src/mir.tr"
            long long bn = 0LL;
#line 1235 "src/mir.tr"
            while ((bn < mf->blocks->len)) {
#line 1236 "src/mir.tr"
                MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1237 "src/mir.tr"
                LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
#line 1238 "src/mir.tr"
                long long si = 0LL;
#line 1239 "src/mir.tr"
                while ((si < blk->stmts->len)) {
#line 1240 "src/mir.tr"
                    bool is_creation = false;
#line 1241 "src/mir.tr"
                    __auto_type _t721 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                    if (_t721.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t721.data.MDeclare.place;
#line 1243 "src/mir.tr"
                        if (_tr_str_eqv((dn), (edge->borrower))) {
#line 1243 "src/mir.tr"
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t721;
                        /* pass */
                    }
#line 1245 "src/mir.tr"
                    if ((!is_creation)) {
#line 1246 "src/mir.tr"
                        List_TrStr* uses = (void*)List_TrStr_new();
#line 1247 "src/mir.tr"
                        __auto_type _t722 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                        if (_t722.tag == MirStmt_MDeclare) {
                            __auto_type val = _t722.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t722.tag == MirStmt_MAssign) {
                            __auto_type ap = _t722.data.MAssign.place;
__auto_type val = _t722.data.MAssign.value;
#line 1250 "src/mir.tr"
                            collect_uses(val, uses);
#line 1251 "src/mir.tr"
                            if (_tr_str_eqv((ap), (edge->source))) {
#line 1251 "src/mir.tr"
                                ({ TrStr _apt_t723 = _tr_str_retain(edge->source); set_add(uses, _apt_t723); _tr_str_release(_apt_t723); });
                            }
                        } else if (_t722.tag == MirStmt_MEval) {
                            __auto_type val = _t722.data.MEval.value;
                            collect_uses(val, uses);
                        }
#line 1253 "src/mir.tr"
                        if (({ __auto_type _ord_t725 = (mf->pwrites); TrStr _apt_t724 = _tr_str_retain(edge->source); TrStr _apt_t726 = _tr_str_retain(edge->path); __auto_type _wr = (_pwrite_hits(_ord_t725, bn, si, _apt_t724, _apt_t726)); _tr_str_release(_apt_t724); _tr_str_release(_apt_t726); _wr; })) {
#line 1253 "src/mir.tr"
                            ({ TrStr _apt_t727 = _tr_str_retain(edge->source); set_add(uses, _apt_t727); _tr_str_release(_apt_t727); });
                        }
#line 1254 "src/mir.tr"
                        if ((({ TrStr _apt_t728 = _tr_str_retain(edge->source); __auto_type _wr = (set_contains(uses, _apt_t728)); _tr_str_release(_apt_t728); _wr; }) && ({ TrStr _apt_t729 = _tr_str_retain(edge->borrower); __auto_type _wr = (borrower_live_after(blk, si, _apt_t729, lo)); _tr_str_release(_apt_t729); _wr; }))) {
#line 1255 "src/mir.tr"
                            proven = false;
                        }
                    }
#line 1256 "src/mir.tr"
                    si = (si + 1LL);
                }
#line 1257 "src/mir.tr"
                bn = (bn + 1LL);
            }
        }
#line 1258 "src/mir.tr"
        if (proven) {
#line 1259 "src/mir.tr"
            ({ TrStr _apt_t730 = _tr_str_retain(edge->borrower); List_TrStr_append(out, _apt_t730); _tr_str_release(_apt_t730); });
        }
#line 1260 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1261 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_bool_free(outl);
    return out;
}

__attribute__((hot)) bool _pwrite_hits(List_ptr* pws, long long blk, long long idx, TrStr root, TrStr bpath) {
#line 1264 "src/mir.tr"
    long long i = 0LL;
#line 1265 "src/mir.tr"
    while ((i < pws->len)) {
#line 1266 "src/mir.tr"
        PWrite* w = ((PWrite*)List_ptr_get(pws, i));
#line 1267 "src/mir.tr"
        if (((((w->block == blk) && (w->idx == idx)) && _tr_str_eqv((w->root), (root))) && ({ TrStr _apt_t731 = _tr_str_retain(w->path); __auto_type _wr = (_pl_overlap(_apt_t731, bpath)); _tr_str_release(_apt_t731); _wr; }))) {
#line 1267 "src/mir.tr"
            return true;
        }
#line 1268 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1269 "src/mir.tr"
    return false;
}

__attribute__((hot)) List_bool* _reach_from(MirFunction* mf, long long start) {
#line 1274 "src/mir.tr"
    List_bool* seen = (void*)List_bool_new();
#line 1275 "src/mir.tr"
    long long k = 0LL;
#line 1276 "src/mir.tr"
    while ((k < mf->blocks->len)) {
#line 1277 "src/mir.tr"
        List_bool_append(seen, false);
#line 1278 "src/mir.tr"
        k = (k + 1LL);
    }
#line 1279 "src/mir.tr"
    List_i64* work = (void*)List_i64_new();
#line 1280 "src/mir.tr"
    List_i64_append(work, start);
#line 1281 "src/mir.tr"
    bool first = true;
#line 1282 "src/mir.tr"
    while ((work->len > 0LL)) {
#line 1283 "src/mir.tr"
        long long cur = List_i64_get(work, (work->len - 1LL));
#line 1284 "src/mir.tr"
        List_i64_pop(work);
#line 1285 "src/mir.tr"
        List_i64* succ = (void*)List_i64_new();
#line 1286 "src/mir.tr"
        __auto_type _t732 = (*((MirBlock*)List_ptr_get(mf->blocks, cur))->term);
        if (_t732.tag == MirTerm_TGoto) {
            __auto_type t = _t732.data.TGoto.target;
            List_i64_append(succ, t);
        } else if (_t732.tag == MirTerm_TBranch) {
            __auto_type t2 = _t732.data.TBranch.t;
__auto_type e2 = _t732.data.TBranch.e;
#line 1289 "src/mir.tr"
            List_i64_append(succ, t2);
#line 1290 "src/mir.tr"
            List_i64_append(succ, e2);
        } else if (1) {
            __auto_type _ = _t732;
            /* pass */
        }
#line 1292 "src/mir.tr"
        long long si = 0LL;
#line 1293 "src/mir.tr"
        while ((si < succ->len)) {
#line 1294 "src/mir.tr"
            long long nx = List_i64_get(succ, si);
#line 1295 "src/mir.tr"
            if ((((nx >= 0LL) && (nx < seen->len)) && (!List_bool_get(seen, nx)))) {
#line 1296 "src/mir.tr"
                List_bool_set(seen, nx, true);
#line 1297 "src/mir.tr"
                List_i64_append(work, nx);
            }
#line 1298 "src/mir.tr"
            si = (si + 1LL);
        }
        List_i64_free(succ);
    }
#line 1299 "src/mir.tr"
    List_i64_free(work);
    return seen;
}

__attribute__((hot)) long long _decl_index(MirBlock* blk, TrStr name) {
#line 1303 "src/mir.tr"
    long long i = 0LL;
#line 1304 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 1305 "src/mir.tr"
        __auto_type _t733 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t733.tag == MirStmt_MDeclare) {
            __auto_type dn = _t733.data.MDeclare.place;
#line 1307 "src/mir.tr"
            if (_tr_str_eqv((dn), (name))) {
#line 1307 "src/mir.tr"
                long long _trret171 = i;
                return _trret171;
            }
        } else if (1) {
            __auto_type _ = _t733;
            /* pass */
        }
#line 1309 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1310 "src/mir.tr"
    return (-1LL);
}

__attribute__((hot)) bool borrower_live_after(MirBlock* blk, long long after_idx, TrStr name, LiveSet* lo) {
#line 1317 "src/mir.tr"
    long long si = (after_idx + 1LL);
#line 1318 "src/mir.tr"
    while ((si < blk->stmts->len)) {
#line 1319 "src/mir.tr"
        List_TrStr* uses = (void*)List_TrStr_new();
#line 1320 "src/mir.tr"
        __auto_type _t734 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t734.tag == MirStmt_MDeclare) {
            __auto_type val = _t734.data.MDeclare.value;
            collect_uses(val, uses);
        } else if (_t734.tag == MirStmt_MAssign) {
            __auto_type val = _t734.data.MAssign.value;
            collect_uses(val, uses);
        } else if (_t734.tag == MirStmt_MEval) {
            __auto_type val = _t734.data.MEval.value;
            collect_uses(val, uses);
        }
#line 1324 "src/mir.tr"
        if (set_contains(uses, name)) {
#line 1324 "src/mir.tr"
            return true;
        }
#line 1325 "src/mir.tr"
        si = (si + 1LL);
    }
#line 1326 "src/mir.tr"
    List_TrStr* tu = (void*)List_TrStr_new();
#line 1327 "src/mir.tr"
    __auto_type _t735 = (*blk->term);
    if (_t735.tag == MirTerm_TBranch) {
        __auto_type cond = _t735.data.TBranch.cond;
        collect_uses(cond, tu);
    } else if (_t735.tag == MirTerm_TReturn) {
        __auto_type val = _t735.data.TReturn.value;
        collect_uses(val, tu);
    } else if (1) {
        __auto_type _ = _t735;
        /* pass */
    }
#line 1331 "src/mir.tr"
    if (set_contains(tu, name)) {
#line 1331 "src/mir.tr"
        return true;
    }
#line 1332 "src/mir.tr"
    return LiveSet_has(lo, name);
}

__attribute__((hot)) List_TrStr* mir_borrow_conflicts(HirFunction* hf, TrMap* mutating_methods) {
#line 1339 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1340 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1341 "src/mir.tr"
    if ((!mf->complete)) {
#line 1341 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1342 "src/mir.tr"
    if ((mf->borrows->len == 0LL)) {
#line 1342 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1343 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1350 "src/mir.tr"
    long long k = 0LL;
#line 1351 "src/mir.tr"
    while ((k < mf->borrows->len)) {
#line 1352 "src/mir.tr"
        BorrowEdge* be = ((BorrowEdge*)List_ptr_get(mf->borrows, k));
#line 1354 "src/mir.tr"
        List_bool* be_reach = (void*)List_bool_new();
#line 1355 "src/mir.tr"
        long long be_didx = (-1LL);
#line 1356 "src/mir.tr"
        if (((be->decl_block >= 0LL) && (be->decl_block < mf->blocks->len))) {
#line 1357 "src/mir.tr"
            be_reach = _reach_from(mf, be->decl_block);
#line 1358 "src/mir.tr"
            be_didx = ({ __auto_type _ord_t737 = (((MirBlock*)List_ptr_get(mf->blocks, be->decl_block))); TrStr _apt_t736 = _tr_str_retain(be->borrower); __auto_type _wr = (_decl_index(_ord_t737, _apt_t736)); _tr_str_release(_apt_t736); _wr; });
        }
#line 1359 "src/mir.tr"
        long long bn = 0LL;
#line 1360 "src/mir.tr"
        while ((bn < mf->blocks->len)) {
#line 1361 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1362 "src/mir.tr"
            LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
#line 1363 "src/mir.tr"
            long long si = 0LL;
#line 1364 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1365 "src/mir.tr"
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
#line 1366 "src/mir.tr"
                bool after_creation = true;
#line 1367 "src/mir.tr"
                if (((be_reach->len > 0LL) && (be_didx >= 0LL))) {
#line 1368 "src/mir.tr"
                    after_creation = List_bool_get(be_reach, bn);
#line 1369 "src/mir.tr"
                    if (((bn == be->decl_block) && (si > be_didx))) {
#line 1369 "src/mir.tr"
                        after_creation = true;
                    }
                }
#line 1370 "src/mir.tr"
                if ((!after_creation)) {
#line 1371 "src/mir.tr"
                    si = (si + 1LL);
#line 1372 "src/mir.tr"
                    continue;
                }
#line 1373 "src/mir.tr"
                TrStr invalid_desc = _tr_str_lit_len("", 0LL);
#line 1374 "src/mir.tr"
                HirExpr* sval = ((HirExpr*)(0LL));
#line 1375 "src/mir.tr"
                __auto_type _t738 = (*stmt);
                if (_t738.tag == MirStmt_MAssign) {
                    __auto_type wp = _t738.data.MAssign.place;
__auto_type wv = _t738.data.MAssign.value;
#line 1377 "src/mir.tr"
                    sval = wv;
#line 1378 "src/mir.tr"
                    if (_tr_str_eqv((wp), (be->source))) {
#line 1378 "src/mir.tr"
                        TrStr _strtmp_t739 = _tr_str_lit_len("assign to", 9LL);
                        _tr_str_release(invalid_desc);
                        invalid_desc = _strtmp_t739;
                    }
                } else if (_t738.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t738.data.MDeclare.value;
                    sval = dv;
                } else if (_t738.tag == MirStmt_MEval) {
                    __auto_type ev = _t738.data.MEval.value;
                    sval = ev;
                }
#line 1381 "src/mir.tr"
                if ((_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))) && ({ TrStr _apt_t740 = _tr_str_retain(be->source); TrStr _apt_t741 = _tr_str_retain(be->path); __auto_type _wr = (_is_mutating_call_on(sval, _apt_t740, mutating_methods, _apt_t741)); _tr_str_release(_apt_t740); _tr_str_release(_apt_t741); _wr; }))) {
#line 1382 "src/mir.tr"
                    TrStr _strtmp_t742 = _tr_str_lit_len("call a mutating method on", 25LL);
                    _tr_str_release(invalid_desc);
                    invalid_desc = _strtmp_t742;
                }
#line 1383 "src/mir.tr"
                if ((_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))) && ({ __auto_type _ord_t744 = (mf->pwrites); TrStr _apt_t743 = _tr_str_retain(be->source); TrStr _apt_t745 = _tr_str_retain(be->path); __auto_type _wr = (_pwrite_hits(_ord_t744, bn, si, _apt_t743, _apt_t745)); _tr_str_release(_apt_t743); _tr_str_release(_apt_t745); _wr; }))) {
#line 1384 "src/mir.tr"
                    TrStr _strtmp_t746 = _tr_str_lit_len("assign to a part of", 19LL);
                    _tr_str_release(invalid_desc);
                    invalid_desc = _strtmp_t746;
                }
#line 1385 "src/mir.tr"
                if ((!_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))))) {
#line 1386 "src/mir.tr"
                    if (({ TrStr _apt_t747 = _tr_str_retain(be->borrower); __auto_type _wr = (borrower_live_after(blk, si, _apt_t747, lo)); _tr_str_release(_apt_t747); _wr; })) {
#line 1387 "src/mir.tr"
                        ({ TrStr _at_t748 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot ", 7LL)), (invalid_desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" '", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->source)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is borrowed by '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — the borrow is still live", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t748); _tr_str_release(_at_t748); });
                    }
                } else if (be->is_exclusive) {
#line 1391 "src/mir.tr"
                    bool is_creation = false;
#line 1392 "src/mir.tr"
                    __auto_type _t749 = (*stmt);
                    if (_t749.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t749.data.MDeclare.place;
#line 1394 "src/mir.tr"
                        if (_tr_str_eqv((dn), (be->borrower))) {
#line 1394 "src/mir.tr"
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t749;
                        /* pass */
                    }
#line 1396 "src/mir.tr"
                    if ((!is_creation)) {
#line 1397 "src/mir.tr"
                        List_TrStr* uses = (void*)List_TrStr_new();
#line 1398 "src/mir.tr"
                        __auto_type _t750 = (*stmt);
                        if (_t750.tag == MirStmt_MDeclare) {
                            __auto_type val = _t750.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t750.tag == MirStmt_MAssign) {
                            __auto_type val = _t750.data.MAssign.value;
                            collect_uses(val, uses);
                        } else if (_t750.tag == MirStmt_MEval) {
                            __auto_type val = _t750.data.MEval.value;
                            collect_uses(val, uses);
                        }
#line 1402 "src/mir.tr"
                        if ((({ TrStr _apt_t751 = _tr_str_retain(be->source); __auto_type _wr = (set_contains(uses, _apt_t751)); _tr_str_release(_apt_t751); _wr; }) && ({ TrStr _apt_t752 = _tr_str_retain(be->borrower); __auto_type _wr = (borrower_live_after(blk, si, _apt_t752, lo)); _tr_str_release(_apt_t752); _wr; }))) {
#line 1403 "src/mir.tr"
                            ({ TrStr _at_t753 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot use '", 12LL)), (be->source))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is exclusively (mut) borrowed by '", 45LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t753); _tr_str_release(_at_t753); });
                        }
                    }
                }
#line 1404 "src/mir.tr"
                si = (si + 1LL);
                _tr_str_release(invalid_desc);
            }
#line 1405 "src/mir.tr"
            bn = (bn + 1LL);
        }
#line 1406 "src/mir.tr"
        k = (k + 1LL);
        List_bool_free(be_reach);
    }
#line 1407 "src/mir.tr"
    if ((mf->borrows->len < 2LL)) {
#line 1407 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        List_ptr_free_obj(live, _trdrop_LiveSet);
        return out;
    }
#line 1408 "src/mir.tr"
    long long i = 0LL;
#line 1409 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1410 "src/mir.tr"
        BorrowEdge* bi = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1411 "src/mir.tr"
        if (((bi->decl_block >= 0LL) && (bi->decl_block < mf->blocks->len))) {
#line 1412 "src/mir.tr"
            long long j = (i + 1LL);
#line 1413 "src/mir.tr"
            while ((j < mf->borrows->len)) {
#line 1414 "src/mir.tr"
                BorrowEdge* bj = ((BorrowEdge*)List_ptr_get(mf->borrows, j));
#line 1415 "src/mir.tr"
                if ((((bj->decl_block == bi->decl_block) && _tr_str_eqv((bi->source), (bj->source))) && (bi->is_exclusive || bj->is_exclusive))) {
#line 1416 "src/mir.tr"
                    MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi->decl_block));
#line 1417 "src/mir.tr"
                    long long pi = 0LL;
#line 1418 "src/mir.tr"
                    long long pj = 0LL;
#line 1419 "src/mir.tr"
                    long long bk = 0LL;
#line 1420 "src/mir.tr"
                    while ((bk < blk->stmts->len)) {
#line 1421 "src/mir.tr"
                        __auto_type _t754 = (*((MirStmt*)List_ptr_get(blk->stmts, bk)));
                        if (_t754.tag == MirStmt_MDeclare) {
                            __auto_type dn = _t754.data.MDeclare.place;
#line 1423 "src/mir.tr"
                            if (_tr_str_eqv((dn), (bi->borrower))) {
#line 1423 "src/mir.tr"
                                pi = bk;
                            }
#line 1424 "src/mir.tr"
                            if (_tr_str_eqv((dn), (bj->borrower))) {
#line 1424 "src/mir.tr"
                                pj = bk;
                            }
                        } else if (1) {
                            __auto_type _ = _t754;
                            /* pass */
                        }
#line 1426 "src/mir.tr"
                        bk = (bk + 1LL);
                    }
#line 1427 "src/mir.tr"
                    long long ui = ({ TrStr _apt_t755 = _tr_str_retain(bi->borrower); __auto_type _wr = (last_use_in_block(blk, _apt_t755)); _tr_str_release(_apt_t755); _wr; });
#line 1428 "src/mir.tr"
                    long long uj = ({ TrStr _apt_t756 = _tr_str_retain(bj->borrower); __auto_type _wr = (last_use_in_block(blk, _apt_t756)); _tr_str_release(_apt_t756); _wr; });
#line 1429 "src/mir.tr"
                    if ((ui < pi)) {
#line 1429 "src/mir.tr"
                        ui = pi;
                    }
#line 1430 "src/mir.tr"
                    if ((uj < pj)) {
#line 1430 "src/mir.tr"
                        uj = pj;
                    }
#line 1431 "src/mir.tr"
                    if (((pi <= uj) && (pj <= ui))) {
#line 1432 "src/mir.tr"
                        TrStr kind = _tr_str_lit_len("a mutable borrow", 16LL);
#line 1433 "src/mir.tr"
                        if ((!(bi->is_exclusive && bj->is_exclusive))) {
#line 1433 "src/mir.tr"
                            TrStr _strtmp_t757 = _tr_str_lit_len("a mutable and a shared borrow", 29LL);
                            _tr_str_release(kind);
                            kind = _strtmp_t757;
                        }
#line 1434 "src/mir.tr"
                        ({ TrStr _at_t758 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (bi->borrower))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' and '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bj->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' are ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kind)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bi->source)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' that are live at the same time", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t758); _tr_str_release(_at_t758); });
                    }
                }
#line 1435 "src/mir.tr"
                j = (j + 1LL);
            }
        }
#line 1436 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1437 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_ptr_free_obj(live, _trdrop_LiveSet);
    return out;
}

__attribute__((hot)) List_TrStr* mir_shared_ref_param_violations(HirFunction* hf, TrMap* mutating_methods) {
#line 1444 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1446 "src/mir.tr"
    List_TrStr* shps = (void*)List_TrStr_new();
#line 1447 "src/mir.tr"
    long long pi = 0LL;
#line 1448 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 1449 "src/mir.tr"
        AstType* pty = ((HirParam*)List_ptr_get(hf->params, pi))->ty;
#line 1450 "src/mir.tr"
        if ((pty->is_borrow && (!pty->is_mut_borrow))) {
#line 1451 "src/mir.tr"
            ({ TrStr _apt_t759 = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name); List_TrStr_append(shps, _apt_t759); _tr_str_release(_apt_t759); });
        }
#line 1452 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 1453 "src/mir.tr"
    if ((shps->len == 0LL)) {
#line 1453 "src/mir.tr"
        List_TrStr_free(shps);
        return out;
    }
#line 1454 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1455 "src/mir.tr"
    long long sk = 0LL;
#line 1456 "src/mir.tr"
    while ((sk < shps->len)) {
#line 1457 "src/mir.tr"
        TrStr pn = List_TrStr_get(shps, sk);
#line 1458 "src/mir.tr"
        long long bn = 0LL;
#line 1459 "src/mir.tr"
        while ((bn < mf->blocks->len)) {
#line 1460 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1461 "src/mir.tr"
            long long si = 0LL;
#line 1462 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1463 "src/mir.tr"
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
#line 1464 "src/mir.tr"
                HirExpr* sval = ((HirExpr*)(0LL));
#line 1465 "src/mir.tr"
                TrStr desc = _tr_str_lit_len("", 0LL);
#line 1466 "src/mir.tr"
                __auto_type _t760 = (*stmt);
                if (_t760.tag == MirStmt_MAssign) {
                    __auto_type wp = _t760.data.MAssign.place;
__auto_type wv = _t760.data.MAssign.value;
#line 1468 "src/mir.tr"
                    sval = wv;
#line 1469 "src/mir.tr"
                    if (_tr_str_eqv((wp), (pn))) {
#line 1469 "src/mir.tr"
                        TrStr _strtmp_t761 = _tr_str_lit_len("reassign", 8LL);
                        _tr_str_release(desc);
                        desc = _strtmp_t761;
                    }
                } else if (_t760.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t760.data.MDeclare.value;
                    sval = dv;
                } else if (_t760.tag == MirStmt_MEval) {
                    __auto_type ev = _t760.data.MEval.value;
                    sval = ev;
                }
#line 1472 "src/mir.tr"
                if ((_tr_str_eqv((desc), (_tr_str_lit_len("", 0LL))) && _is_mutating_call_on(sval, pn, mutating_methods, _tr_str_lit_len("", 0LL)))) {
#line 1473 "src/mir.tr"
                    TrStr _strtmp_t762 = _tr_str_lit_len("call a mutating method on", 25LL);
                    _tr_str_release(desc);
                    desc = _strtmp_t762;
                }
#line 1474 "src/mir.tr"
                if ((!_tr_str_eqv((desc), (_tr_str_lit_len("", 0LL))))) {
#line 1475 "src/mir.tr"
                    ({ TrStr _at_t763 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot ", 7LL)), (desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" '", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t763); _tr_str_release(_at_t763); });
                }
#line 1476 "src/mir.tr"
                si = (si + 1LL);
                _tr_str_release(desc);
            }
#line 1477 "src/mir.tr"
            bn = (bn + 1LL);
        }
#line 1478 "src/mir.tr"
        sk = (sk + 1LL);
        _tr_str_release(pn);
    }
#line 1479 "src/mir.tr"
    List_TrStr_free(shps);
    _tr_obj_release(mf, _trdrop_MirFunction);
    return out;
}

__attribute__((hot)) TrStr _b3_root_ident(HirExpr* e) {
#line 1486 "src/mir.tr"
    HirExpr* cur = e;
#line 1487 "src/mir.tr"
    long long guard = 0LL;
#line 1488 "src/mir.tr"
    while (((((unsigned long long)(cur)) != ((unsigned long long)(0LL))) && (guard < 64LL))) {
#line 1489 "src/mir.tr"
        __auto_type _t764 = (*cur);
        if (_t764.tag == HirExpr_EIdent) {
            __auto_type n = _t764.data.EIdent.name;
            TrStr _trret172 = _tr_str_retain(n);
            return _trret172;
        } else if (_t764.tag == HirExpr_EPropAccess) {
            __auto_type o = _t764.data.EPropAccess.obj;
            cur = o;
        } else if (_t764.tag == HirExpr_EIndex) {
            __auto_type o = _t764.data.EIndex.obj;
            cur = o;
        } else if (1) {
            __auto_type _ = _t764;
            return _tr_str_lit_len("", 0LL);
        }
#line 1494 "src/mir.tr"
        guard = (guard + 1LL);
    }
#line 1495 "src/mir.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _b3_is_plain_ident(HirExpr* e) {
#line 1498 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1498 "src/mir.tr"
        return false;
    }
#line 1499 "src/mir.tr"
    __auto_type _t765 = (*e);
    if (_t765.tag == HirExpr_EIdent) {
        return true;
    } else if (1) {
        __auto_type _ = _t765;
        return false;
    }
}

__attribute__((hot)) bool _b3_nested_mut_call(HirExpr* e, TrStr pn, TrMap* mm) {
#line 1504 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1504 "src/mir.tr"
        return false;
    }
#line 1505 "src/mir.tr"
    __auto_type _t766 = (*e);
    if (_t766.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t766.data.EMethodCall.obj;
__auto_type method = _t766.data.EMethodCall.method;
#line 1507 "src/mir.tr"
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 1507 "src/mir.tr"
            return false;
        }
#line 1508 "src/mir.tr"
        if (_b3_is_plain_ident(obj)) {
#line 1508 "src/mir.tr"
            return false;
        }
#line 1509 "src/mir.tr"
        if (({ TrStr _cmpa_t767 = (_b3_root_ident(obj)); TrStr _cmpb_t768 = (pn); _Bool _cmpr_t769 = !_tr_str_eqv(_cmpa_t767, _cmpb_t768); _tr_str_release(_cmpa_t767); _cmpr_t769; })) {
#line 1509 "src/mir.tr"
            return false;
        }
#line 1510 "src/mir.tr"
        TrStr tn = _tr_str_retain(hir_expr_type(obj)->name);
#line 1511 "src/mir.tr"
        if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 1512 "src/mir.tr"
            bool _trret173 = (((((((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))));
            _tr_str_release(tn);
            return _trret173;
        }
#line 1513 "src/mir.tr"
        bool _trret174 = ({ TrStr _dkt_t770 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t770))); _tr_str_release(_dkt_t770); _wr; });
        _tr_str_release(tn);
        return _trret174;
    } else if (1) {
        __auto_type _ = _t766;
        return false;
    }
}

__attribute__((hot)) void _b3_block(HirBlock* b, TrStr pn, TrMap* mm, List_TrStr* out) {
#line 1517 "src/mir.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 1517 "src/mir.tr"
        return;
    }
#line 1518 "src/mir.tr"
    long long i = 0LL;
#line 1519 "src/mir.tr"
    while ((i < b->stmts->len)) {
#line 1520 "src/mir.tr"
        _b3_stmt(((HirStmt*)List_ptr_get(b->stmts, i)), pn, mm, out);
#line 1521 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void _b3_stmt(HirStmt* s, TrStr pn, TrMap* mm, List_TrStr* out) {
#line 1524 "src/mir.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 1524 "src/mir.tr"
        return;
    }
#line 1525 "src/mir.tr"
    __auto_type _t771 = (*s);
    if (_t771.tag == HirStmt_SAssign) {
        __auto_type tgt = _t771.data.SAssign.target;
__auto_type val = _t771.data.SAssign.val;
#line 1527 "src/mir.tr"
        if ((((((unsigned long long)(tgt)) != ((unsigned long long)(0LL))) && (!_b3_is_plain_ident(tgt))) && ({ TrStr _cmpa_t772 = (_b3_root_ident(tgt)); TrStr _cmpb_t773 = (pn); _Bool _cmpr_t774 = _tr_str_eqv(_cmpa_t772, _cmpb_t773); _tr_str_release(_cmpa_t772); _cmpr_t774; }))) {
#line 1528 "src/mir.tr"
            ({ TrStr _at_t775 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot assign through '", 23LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t775); _tr_str_release(_at_t775); });
        } else if (_b3_nested_mut_call(val, pn, mm)) {
#line 1530 "src/mir.tr"
            ({ TrStr _at_t776 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot call a mutating method on a part of '", 44LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t776); _tr_str_release(_at_t776); });
        }
    } else if (_t771.tag == HirStmt_SExpr) {
        __auto_type e = _t771.data.SExpr.expr;
#line 1532 "src/mir.tr"
        if (_b3_nested_mut_call(e, pn, mm)) {
#line 1533 "src/mir.tr"
            ({ TrStr _at_t777 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot call a mutating method on a part of '", 44LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t777); _tr_str_release(_at_t777); });
        }
    } else if (_t771.tag == HirStmt_SLet) {
        __auto_type v = _t771.data.SLet.val;
#line 1535 "src/mir.tr"
        if (_b3_nested_mut_call(v, pn, mm)) {
#line 1536 "src/mir.tr"
            ({ TrStr _at_t778 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot call a mutating method on a part of '", 44LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t778); _tr_str_release(_at_t778); });
        }
    } else if (_t771.tag == HirStmt_SIf) {
        __auto_type tb = _t771.data.SIf.then_b;
__auto_type eb = _t771.data.SIf.else_b;
#line 1538 "src/mir.tr"
        _b3_block(tb, pn, mm, out);
#line 1539 "src/mir.tr"
        _b3_block(eb, pn, mm, out);
    } else if (_t771.tag == HirStmt_SWhile) {
        __auto_type body = _t771.data.SWhile.body;
        _b3_block(body, pn, mm, out);
    } else if (_t771.tag == HirStmt_SFor) {
        __auto_type body = _t771.data.SFor.body;
        _b3_block(body, pn, mm, out);
    } else if (_t771.tag == HirStmt_SForUnpack) {
        __auto_type body = _t771.data.SForUnpack.body;
        _b3_block(body, pn, mm, out);
    } else if (_t771.tag == HirStmt_SUnsafe) {
        __auto_type body = _t771.data.SUnsafe.body;
        _b3_block(body, pn, mm, out);
    } else if (_t771.tag == HirStmt_SWith) {
        __auto_type body = _t771.data.SWith.body;
        _b3_block(body, pn, mm, out);
    } else if (_t771.tag == HirStmt_SMatch) {
        __auto_type arms = _t771.data.SMatch.arms;
#line 1546 "src/mir.tr"
        long long i = 0LL;
#line 1547 "src/mir.tr"
        while ((i < arms->len)) {
#line 1548 "src/mir.tr"
            _b3_block(((HirMatchArm*)List_ptr_get(arms, i))->body, pn, mm, out);
#line 1549 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t771.tag == HirStmt_STry) {
        __auto_type tb = _t771.data.STry.try_body;
__auto_type catches = _t771.data.STry.catches;
__auto_type fb = _t771.data.STry.finally_b;
#line 1551 "src/mir.tr"
        _b3_block(tb, pn, mm, out);
#line 1552 "src/mir.tr"
        long long i = 0LL;
#line 1553 "src/mir.tr"
        while ((i < catches->len)) {
#line 1554 "src/mir.tr"
            _b3_block((*((HirCatchClause**)List_ptr_get(catches, i)))->body, pn, mm, out);
#line 1555 "src/mir.tr"
            i = (i + 1LL);
        }
#line 1556 "src/mir.tr"
        _b3_block(fb, pn, mm, out);
    } else if (_t771.tag == HirStmt_SDefer) {
        __auto_type inner = _t771.data.SDefer.stmt;
        _b3_stmt(inner, pn, mm, out);
    } else if (1) {
        __auto_type _ = _t771;
        /* pass */
    }
}

__attribute__((hot)) List_TrStr* mir_shared_ref_param_field_writes(HirFunction* hf, TrMap* mm) {
#line 1561 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1562 "src/mir.tr"
    long long pi = 0LL;
#line 1563 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 1564 "src/mir.tr"
        AstType* pty = ((HirParam*)List_ptr_get(hf->params, pi))->ty;
#line 1565 "src/mir.tr"
        if ((pty->is_borrow && (!pty->is_mut_borrow))) {
#line 1566 "src/mir.tr"
            ({ __auto_type _ord_t780 = (hf->body); TrStr _apt_t779 = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name); _b3_block(_ord_t780, _apt_t779, mm, out); _tr_str_release(_apt_t779); });
        }
#line 1567 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 1568 "src/mir.tr"
    return out;
}

__attribute__((hot)) List_ptr* mir_if_drop_plan(HirFunction* hf) {
#line 1571 "src/mir.tr"
    List_ptr* out = (void*)List_ptr_new();
#line 1572 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1573 "src/mir.tr"
    if ((!mf->complete)) {
#line 1573 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1574 "src/mir.tr"
    if ((mf->if_bodies->len == 0LL)) {
#line 1574 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1575 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1576 "src/mir.tr"
    List_ptr* drops = compute_drops(mf, live);
#line 1582 "src/mir.tr"
    long long bi = 0LL;
#line 1583 "src/mir.tr"
    while ((bi < mf->blocks->len)) {
#line 1584 "src/mir.tr"
        MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
#line 1585 "src/mir.tr"
        if (((((unsigned long long)(blk->hir_block)) != ((unsigned long long)(0LL))) && is_if_body(mf, blk->hir_block))) {
#line 1586 "src/mir.tr"
            LiveSet* dd = ((LiveSet*)List_ptr_get(drops, blk->id));
#line 1587 "src/mir.tr"
            long long pi = 0LL;
#line 1588 "src/mir.tr"
            while ((pi < dd->items->len)) {
#line 1589 "src/mir.tr"
                DropSite* site = site_for(out, blk->hir_block);
#line 1590 "src/mir.tr"
                ({ __auto_type _ord_t782 = (site->places); TrStr _at_t781 = (List_TrStr_get(dd->items, pi)); set_add(_ord_t782, _at_t781); _tr_str_release(_at_t781); });
#line 1591 "src/mir.tr"
                pi = (pi + 1LL);
            }
        }
#line 1592 "src/mir.tr"
        bi = (bi + 1LL);
    }
#line 1593 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_ptr_free_obj(drops, _trdrop_LiveSet);
    return out;
}

__attribute__((hot)) TrStr set_str(List_TrStr* v) {
#line 1596 "src/mir.tr"
    TrStr s = _tr_str_lit_len("{", 1LL);
#line 1597 "src/mir.tr"
    long long i = 0LL;
#line 1598 "src/mir.tr"
    while ((i < v->len)) {
#line 1599 "src/mir.tr"
        if ((i > 0LL)) {
#line 1599 "src/mir.tr"
            TrStr _strtmp_t784 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t784;
        }
#line 1600 "src/mir.tr"
        TrStr _strtmp_t786 = ({ TrStr _tr_cr = (List_TrStr_get(v, i)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t786;
#line 1601 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1602 "src/mir.tr"
    TrStr _trret175 = _tr_strx_concatv((s), (_tr_str_lit_len("}", 1LL)));
    _tr_str_release(s);
    return _trret175;
}

__attribute__((hot)) TrStr term_str(MirTerm* t) {
#line 1606 "src/mir.tr"
    __auto_type _t787 = (*t);
    if (_t787.tag == MirTerm_TGoto) {
        __auto_type n = _t787.data.TGoto.target;
        return ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("goto bb", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    } else if (_t787.tag == MirTerm_TBranch) {
        __auto_type tt = _t787.data.TBranch.t;
__auto_type ee = _t787.data.TBranch.e;
        return ({ __auto_type _ord_t788 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(tt)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("branch <cond> ? bb", 18LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" : bb", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t788); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ee)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
    } else if (_t787.tag == MirTerm_TReturn) {
        __auto_type _ = _t787.data.TReturn.value;
        return _tr_str_lit_len("return <expr>", 13LL);
    } else if (_t787.tag == MirTerm_TReturnVoid) {
        return _tr_str_lit_len("return", 6LL);
    } else if (_t787.tag == MirTerm_TUnset) {
        return _tr_str_lit_len("<unset>", 7LL);
    }
#line 1612 "src/mir.tr"
    return _tr_str_lit_len("<?>", 3LL);
}

__attribute__((hot)) TrStr stmt_str(MirStmt* s) {
#line 1615 "src/mir.tr"
    __auto_type _t789 = (*s);
    if (_t789.tag == MirStmt_MDeclare) {
        __auto_type p = _t789.data.MDeclare.place;
        TrStr _trret176 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("let ", 4LL)), (p))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = <expr>", 9LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret176;
    } else if (_t789.tag == MirStmt_MAssign) {
        __auto_type p = _t789.data.MAssign.place;
        TrStr _trret177 = _tr_strx_concatv((p), (_tr_str_lit_len(" = <expr>", 9LL)));
        return _trret177;
    } else if (_t789.tag == MirStmt_MEval) {
        __auto_type _ = _t789.data.MEval.value;
        return _tr_str_lit_len("eval <expr>", 11LL);
    }
#line 1619 "src/mir.tr"
    return _tr_str_lit_len("<?>", 3LL);
}

__attribute__((hot)) TrStr dump_mir(MirProgram* mp) {
#line 1622 "src/mir.tr"
    TrStr out = _tr_str_lit_len("", 0LL);
#line 1623 "src/mir.tr"
    long long fi = 0LL;
#line 1624 "src/mir.tr"
    while ((fi < mp->functions->len)) {
#line 1625 "src/mir.tr"
        MirFunction* f = ((MirFunction*)List_ptr_get(mp->functions, fi));
#line 1626 "src/mir.tr"
        TrStr tag = _tr_str_lit_len(" [complete]", 11LL);
#line 1627 "src/mir.tr"
        if ((!f->complete)) {
#line 1627 "src/mir.tr"
            TrStr _strtmp_t790 = _tr_str_lit_len(" [INCOMPLETE - falls back to HIR]", 33LL);
            _tr_str_release(tag);
            tag = _strtmp_t790;
        }
#line 1628 "src/mir.tr"
        ({ TrStr _ap_t791 = (_tr_str_lit_len("fn ", 3LL)); TrStr _ap_t792 = (f->name); TrStr _ap_t793 = (_tr_str_lit_len("(", 1LL)); TrStr _ap_t794 = (_tr_str_wrap(_tr_int_to_str((long long)(f->params->len)))); TrStr _ap_t795 = (_tr_str_lit_len(" params), ", 10LL)); TrStr _ap_t796 = (_tr_str_wrap(_tr_int_to_str((long long)(f->blocks->len)))); TrStr _ap_t797 = (_tr_str_lit_len(" blocks", 7LL)); TrStr _ap_t798 = (tag); TrStr _ap_t799 = (_tr_str_lit_len(":\n", 2LL)); _tr_str_append(&out, _ap_t791); _tr_str_append(&out, _ap_t792); _tr_str_append(&out, _ap_t793); _tr_str_append(&out, _ap_t794); _tr_str_append(&out, _ap_t795); _tr_str_append(&out, _ap_t796); _tr_str_append(&out, _ap_t797); _tr_str_append(&out, _ap_t798); _tr_str_append(&out, _ap_t799); _tr_str_release(_ap_t794); _tr_str_release(_ap_t796); });
#line 1629 "src/mir.tr"
        List_ptr* live = compute_liveness(f);
#line 1630 "src/mir.tr"
        List_ptr* drops = compute_drops(f, live);
#line 1632 "src/mir.tr"
        if ((f->borrows->len > 0LL)) {
#line 1633 "src/mir.tr"
            List_bool* outl = compute_borrow_outlives(f, live);
#line 1634 "src/mir.tr"
            long long ei = 0LL;
#line 1635 "src/mir.tr"
            while ((ei < f->borrows->len)) {
#line 1636 "src/mir.tr"
                BorrowEdge* ed = ((BorrowEdge*)List_ptr_get(f->borrows, ei));
#line 1637 "src/mir.tr"
                TrStr verdict = _tr_str_lit_len("NEEDS-EXTENSION", 15LL);
#line 1638 "src/mir.tr"
                if (List_bool_get(outl, ei)) {
#line 1638 "src/mir.tr"
                    TrStr _strtmp_t800 = _tr_str_lit_len("PROVEN", 6LL);
                    _tr_str_release(verdict);
                    verdict = _strtmp_t800;
                }
#line 1639 "src/mir.tr"
                ({ TrStr _ap_t801 = (_tr_str_lit_len("  ; borrow ", 11LL)); TrStr _ap_t802 = (ed->borrower); TrStr _ap_t803 = (_tr_str_lit_len(" <- ", 4LL)); TrStr _ap_t804 = (ed->source); TrStr _ap_t805 = (_tr_str_lit_len(" : ", 3LL)); TrStr _ap_t806 = (verdict); TrStr _ap_t807 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t801); _tr_str_append(&out, _ap_t802); _tr_str_append(&out, _ap_t803); _tr_str_append(&out, _ap_t804); _tr_str_append(&out, _ap_t805); _tr_str_append(&out, _ap_t806); _tr_str_append(&out, _ap_t807); });
#line 1640 "src/mir.tr"
                ei = (ei + 1LL);
                _tr_str_release(verdict);
            }
        }
#line 1641 "src/mir.tr"
        long long bi = 0LL;
#line 1642 "src/mir.tr"
        while ((bi < f->blocks->len)) {
#line 1643 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(f->blocks, bi));
#line 1644 "src/mir.tr"
            ({ TrStr _ap_t808 = (_tr_str_lit_len("  bb", 4LL)); TrStr _ap_t809 = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _ap_t810 = (_tr_str_lit_len(":\n", 2LL)); _tr_str_append(&out, _ap_t808); _tr_str_append(&out, _ap_t809); _tr_str_append(&out, _ap_t810); _tr_str_release(_ap_t809); });
#line 1645 "src/mir.tr"
            long long si = 0LL;
#line 1646 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1647 "src/mir.tr"
                ({ TrStr _ap_t811 = (_tr_str_lit_len("    ", 4LL)); TrStr _ap_t812 = (stmt_str(((MirStmt*)List_ptr_get(blk->stmts, si)))); TrStr _ap_t813 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t811); _tr_str_append(&out, _ap_t812); _tr_str_append(&out, _ap_t813); _tr_str_release(_ap_t812); });
#line 1648 "src/mir.tr"
                si = (si + 1LL);
            }
#line 1649 "src/mir.tr"
            ({ TrStr _ap_t814 = (_tr_str_lit_len("    ", 4LL)); TrStr _ap_t815 = (term_str(blk->term)); TrStr _ap_t816 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t814); _tr_str_append(&out, _ap_t815); _tr_str_append(&out, _ap_t816); _tr_str_release(_ap_t815); });
#line 1650 "src/mir.tr"
            TrStr _strtmp_t820 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((out), (_tr_str_lit_len("    ; live_out ", 15LL)))); TrStr _tr_cr = (set_str(((LiveSet*)List_ptr_get(live, blk->id))->items)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(out);
            out = _strtmp_t820;
#line 1651 "src/mir.tr"
            ({ TrStr _ap_t821 = (_tr_str_lit_len("    ; drop ", 11LL)); TrStr _ap_t822 = (set_str(((LiveSet*)List_ptr_get(drops, blk->id))->items)); TrStr _ap_t823 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t821); _tr_str_append(&out, _ap_t822); _tr_str_append(&out, _ap_t823); _tr_str_release(_ap_t822); });
#line 1652 "src/mir.tr"
            bi = (bi + 1LL);
        }
#line 1653 "src/mir.tr"
        fi = (fi + 1LL);
        _tr_str_release(tag);
        List_ptr_free_obj(drops, _trdrop_LiveSet);
    }
#line 1654 "src/mir.tr"
    TrStr _trret178 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret178;
}

