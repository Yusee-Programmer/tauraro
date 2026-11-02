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
bool _b3_pass_to_mut(HirExpr* e, TrStr pn, TrMap* sigs);
void _b3_stmt_pass(HirStmt* s, TrStr pn, TrMap* sigs, List_TrStr* out);
void _b3_block_pass(HirBlock* b, TrStr pn, TrMap* sigs, List_TrStr* out);
void _b3_aliases(HirBlock* b, List_TrStr* names);
void _b3_alias_mut(HirBlock* b, TrStr an, TrMap* mm, List_TrStr* out, TrStr pn);
TrStr set_str(List_TrStr* v);
TrStr term_str(MirTerm* t);
TrStr stmt_str(MirStmt* s);

__attribute__((malloc,returns_nonnull,hot)) MirProgram* MirProgram_init() {
#line 302 "src/mir.tr"
    MirProgram* p = ((MirProgram*)_tr_obj_alloc(sizeof(MirProgram)));
#line 303 "src/mir.tr"
    p->functions = (void*)List_ptr_new();
#line 304 "src/mir.tr"
    return p;
}

__attribute__((malloc,returns_nonnull,hot)) MirBuilder* MirBuilder_init() {
#line 203 "src/mir.tr"
    MirBuilder* b = ((MirBuilder*)_tr_obj_alloc(sizeof(MirBuilder)));
#line 204 "src/mir.tr"
    b->blocks = (void*)List_ptr_new();
#line 205 "src/mir.tr"
    b->cur = (-1LL);
#line 206 "src/mir.tr"
    b->complete = true;
#line 207 "src/mir.tr"
    b->if_bodies = (void*)List_ptr_new();
#line 208 "src/mir.tr"
    b->in_unsafe = 0LL;
#line 209 "src/mir.tr"
    LiveSet* _cltmp_t660 = LiveSet_init();
    _tr_obj_release(b->unsafe_pinned, _trdrop_LiveSet);
    b->unsafe_pinned = _cltmp_t660;
#line 210 "src/mir.tr"
    b->loop_continue = (void*)List_i64_new();
#line 211 "src/mir.tr"
    b->loop_break = (void*)List_i64_new();
#line 212 "src/mir.tr"
    b->borrows = (void*)List_ptr_new();
#line 213 "src/mir.tr"
    b->pwrites = (void*)List_ptr_new();
#line 214 "src/mir.tr"
    return b;
}

__attribute__((hot)) void MirBuilder_record_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
#line 218 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 219 "src/mir.tr"
    TrStr _sfo_t661 = e->borrower;
    e->borrower = _tr_str_retain(borrower);
    _tr_str_release(_sfo_t661);
#line 220 "src/mir.tr"
    TrStr _sfo_t662 = e->source;
    e->source = _tr_str_retain(source);
    _tr_str_release(_sfo_t662);
#line 221 "src/mir.tr"
    e->decl_block = self->cur;
#line 222 "src/mir.tr"
    e->decl_idx = MirBuilder_cur_stmt_idx(self);
#line 223 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 224 "src/mir.tr"
    TrStr _sfo_t663 = e->path;
    e->path = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t663);
#line 225 "src/mir.tr"
    e->via_collection = false;
#line 226 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) void MirBuilder_record_coll_borrow(MirBuilder* self, TrStr borrower, TrStr source, bool exclusive) {
#line 230 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 231 "src/mir.tr"
    TrStr _sfo_t664 = e->borrower;
    e->borrower = _tr_str_retain(borrower);
    _tr_str_release(_sfo_t664);
#line 232 "src/mir.tr"
    TrStr _sfo_t665 = e->source;
    e->source = _tr_str_retain(source);
    _tr_str_release(_sfo_t665);
#line 233 "src/mir.tr"
    e->decl_block = self->cur;
#line 234 "src/mir.tr"
    e->decl_idx = MirBuilder_cur_stmt_idx(self);
#line 235 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 236 "src/mir.tr"
    TrStr _sfo_t666 = e->path;
    e->path = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t666);
#line 237 "src/mir.tr"
    e->via_collection = true;
#line 238 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) void MirBuilder_record_path_borrow(MirBuilder* self, TrStr borrower, TrStr source, TrStr path, bool exclusive) {
#line 244 "src/mir.tr"
    BorrowEdge* e = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 245 "src/mir.tr"
    TrStr _sfo_t667 = e->borrower;
    e->borrower = _tr_str_retain(borrower);
    _tr_str_release(_sfo_t667);
#line 246 "src/mir.tr"
    TrStr _sfo_t668 = e->source;
    e->source = _tr_str_retain(source);
    _tr_str_release(_sfo_t668);
#line 247 "src/mir.tr"
    e->decl_block = self->cur;
#line 248 "src/mir.tr"
    e->decl_idx = MirBuilder_cur_stmt_idx(self);
#line 249 "src/mir.tr"
    e->is_exclusive = exclusive;
#line 250 "src/mir.tr"
    TrStr _sfo_t669 = e->path;
    e->path = _tr_str_retain(path);
    _tr_str_release(_sfo_t669);
#line 251 "src/mir.tr"
    e->via_collection = true;
#line 252 "src/mir.tr"
    List_ptr_append(self->borrows, _tr_obj_retain(e));
    _tr_obj_release(e, _trdrop_BorrowEdge);
}

__attribute__((hot)) void MirBuilder_record_pwrite(MirBuilder* self, TrStr root, TrStr path) {
#line 256 "src/mir.tr"
    if ((self->cur < 0LL)) {
#line 256 "src/mir.tr"
        return;
    }
#line 257 "src/mir.tr"
    PWrite* w = ((PWrite*)_tr_obj_alloc(sizeof(PWrite)));
#line 258 "src/mir.tr"
    w->block = self->cur;
#line 259 "src/mir.tr"
    w->idx = (((MirBlock*)List_ptr_get(self->blocks, self->cur))->stmts->len - 1LL);
#line 260 "src/mir.tr"
    TrStr _sfo_t670 = w->root;
    w->root = _tr_str_retain(root);
    _tr_str_release(_sfo_t670);
#line 261 "src/mir.tr"
    TrStr _sfo_t671 = w->path;
    w->path = _tr_str_retain(path);
    _tr_str_release(_sfo_t671);
#line 262 "src/mir.tr"
    List_ptr_append(self->pwrites, _tr_obj_retain(w));
    _tr_obj_release(w, _trdrop_PWrite);
}

__attribute__((hot)) long long MirBuilder_cur_stmt_idx(MirBuilder* self) {
#line 266 "src/mir.tr"
    if ((self->cur < 0LL)) {
#line 266 "src/mir.tr"
        return (-1LL);
    }
#line 267 "src/mir.tr"
    return (((MirBlock*)List_ptr_get(self->blocks, self->cur))->stmts->len - 1LL);
}

__attribute__((hot)) long long MirBuilder_new_block(MirBuilder* self) {
#line 271 "src/mir.tr"
    MirBlock* blk = ((MirBlock*)_tr_obj_alloc(sizeof(MirBlock)));
#line 272 "src/mir.tr"
    blk->id = self->blocks->len;
#line 273 "src/mir.tr"
    blk->stmts = (void*)List_ptr_new();
#line 274 "src/mir.tr"
    blk->term = box_mirterm(MirTerm_make_TUnset());
#line 275 "src/mir.tr"
    List_ptr_append(self->blocks, _tr_obj_retain(blk));
#line 276 "src/mir.tr"
    long long _trret168 = blk->id;
    _tr_obj_release(blk, _trdrop_MirBlock);
    return _trret168;
}

__attribute__((hot)) void MirBuilder_push_stmt(MirBuilder* self, MirStmt s) {
#line 279 "src/mir.tr"
    if ((self->cur >= 0LL)) {
#line 280 "src/mir.tr"
        MirBlock* blk = ((MirBlock*)List_ptr_get(self->blocks, self->cur));
#line 281 "src/mir.tr"
        ({ __auto_type _ord_t672 = (box_mirstmt(s)); List_ptr_append(blk->stmts, _ord_t672); });
#line 285 "src/mir.tr"
        if ((((unsigned long long)(blk->hir_block)) == ((unsigned long long)(0LL)))) {
#line 286 "src/mir.tr"
            HirBlock* _cltmp_t673 = _tr_obj_retain(self->cur_hb);
            _tr_obj_release(blk->hir_block, _trdrop_HirBlock);
            blk->hir_block = _cltmp_t673;
        }
    }
}

__attribute__((hot)) void MirBuilder_set_term(MirBuilder* self, MirTerm t) {
#line 289 "src/mir.tr"
    if ((self->cur >= 0LL)) {
#line 290 "src/mir.tr"
        __auto_type _asg_t674 = box_mirterm(t);
        ((MirBlock*)List_ptr_get(self->blocks, self->cur))->term = _asg_t674;
    }
}

__attribute__((hot)) bool MirBuilder_terminated(MirBuilder* self) {
#line 295 "src/mir.tr"
    if ((self->cur < 0LL)) {
#line 295 "src/mir.tr"
        return true;
    }
#line 296 "src/mir.tr"
    __auto_type _t675 = (*((MirBlock*)List_ptr_get(self->blocks, self->cur))->term);
    if (_t675.tag == MirTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t675;
        return true;
    }
}

__attribute__((malloc,returns_nonnull,hot)) LiveSet* LiveSet_init() {
#line 821 "src/mir.tr"
    LiveSet* s = ((LiveSet*)_tr_obj_alloc(sizeof(LiveSet)));
#line 822 "src/mir.tr"
    s->items = (void*)List_TrStr_new();
#line 823 "src/mir.tr"
    return s;
}

__attribute__((hot)) bool LiveSet_has(LiveSet* self, TrStr s) {
#line 826 "src/mir.tr"
    long long i = 0LL;
#line 827 "src/mir.tr"
    while ((i < self->items->len)) {
#line 828 "src/mir.tr"
        if (({ TrStr _cmpa_t676 = (List_TrStr_get(self->items, i)); TrStr _cmpb_t677 = (s); _Bool _cmpr_t678 = _tr_str_eqv(_cmpa_t676, _cmpb_t677); _tr_str_release(_cmpa_t676); _cmpr_t678; })) {
#line 828 "src/mir.tr"
            return true;
        }
#line 829 "src/mir.tr"
        i = (i + 1LL);
    }
#line 830 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool LiveSet_add(LiveSet* self, TrStr s) {
#line 833 "src/mir.tr"
    if (LiveSet_has(self, s)) {
#line 833 "src/mir.tr"
        return false;
    }
#line 834 "src/mir.tr"
    List_TrStr_append(self->items, s);
#line 835 "src/mir.tr"
    return true;
}

__attribute__((hot)) bool LiveSet_union_in(LiveSet* self, LiveSet* other) {
#line 839 "src/mir.tr"
    bool changed = false;
#line 840 "src/mir.tr"
    long long i = 0LL;
#line 841 "src/mir.tr"
    while ((i < other->items->len)) {
#line 842 "src/mir.tr"
        if (({ TrStr _at_t679 = (List_TrStr_get(other->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t679)); _tr_str_release(_at_t679); _wr; })) {
#line 842 "src/mir.tr"
            changed = true;
        }
#line 843 "src/mir.tr"
        i = (i + 1LL);
    }
#line 844 "src/mir.tr"
    return changed;
}

__attribute__((hot)) bool LiveSet_union_except(LiveSet* self, LiveSet* src, LiveSet* block) {
#line 848 "src/mir.tr"
    bool changed = false;
#line 849 "src/mir.tr"
    long long i = 0LL;
#line 850 "src/mir.tr"
    while ((i < src->items->len)) {
#line 851 "src/mir.tr"
        if (({ TrStr _at_t680 = (List_TrStr_get(src->items, i)); __auto_type _wr = ((!LiveSet_has(block, _at_t680))); _tr_str_release(_at_t680); _wr; })) {
#line 852 "src/mir.tr"
            if (({ TrStr _at_t681 = (List_TrStr_get(src->items, i)); __auto_type _wr = (LiveSet_add(self, _at_t681)); _tr_str_release(_at_t681); _wr; })) {
#line 852 "src/mir.tr"
                changed = true;
            }
        }
#line 853 "src/mir.tr"
        i = (i + 1LL);
    }
#line 854 "src/mir.tr"
    return changed;
}

__attribute__((hot)) LiveSet* LiveSet_clone(LiveSet* self) {
#line 857 "src/mir.tr"
    LiveSet* r = LiveSet_init();
#line 858 "src/mir.tr"
    long long i = 0LL;
#line 859 "src/mir.tr"
    while ((i < self->items->len)) {
#line 860 "src/mir.tr"
        ({ TrStr _ord_t682 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _ord_t682); _tr_str_release(_ord_t682); });
#line 861 "src/mir.tr"
        i = (i + 1LL);
    }
#line 862 "src/mir.tr"
    return r;
}

__attribute__((hot)) void LiveSet_retain_common(LiveSet* self, LiveSet* other) {
#line 866 "src/mir.tr"
    List_TrStr* keep = (void*)List_TrStr_new();
#line 867 "src/mir.tr"
    long long i = 0LL;
#line 868 "src/mir.tr"
    while ((i < self->items->len)) {
#line 869 "src/mir.tr"
        if (({ TrStr _at_t683 = (List_TrStr_get(self->items, i)); __auto_type _wr = (LiveSet_has(other, _at_t683)); _tr_str_release(_at_t683); _wr; })) {
#line 869 "src/mir.tr"
            ({ TrStr _at_t684 = (List_TrStr_get(self->items, i)); List_TrStr_append(keep, _at_t684); _tr_str_release(_at_t684); });
        }
#line 870 "src/mir.tr"
        i = (i + 1LL);
    }
#line 871 "src/mir.tr"
    self->items = keep;
}

__attribute__((hot)) bool LiveSet_equals(LiveSet* self, LiveSet* other) {
#line 874 "src/mir.tr"
    if ((self->items->len != other->items->len)) {
#line 874 "src/mir.tr"
        return false;
    }
#line 875 "src/mir.tr"
    long long i = 0LL;
#line 876 "src/mir.tr"
    while ((i < self->items->len)) {
#line 877 "src/mir.tr"
        if (({ TrStr _at_t685 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t685))); _tr_str_release(_at_t685); _wr; })) {
#line 877 "src/mir.tr"
            return false;
        }
#line 878 "src/mir.tr"
        i = (i + 1LL);
    }
#line 879 "src/mir.tr"
    return true;
}

__attribute__((hot)) void LiveSet_set_to(LiveSet* self, LiveSet* other) {
#line 884 "src/mir.tr"
    List_TrStr* fresh = (void*)List_TrStr_new();
#line 885 "src/mir.tr"
    long long i = 0LL;
#line 886 "src/mir.tr"
    while ((i < other->items->len)) {
#line 887 "src/mir.tr"
        ({ TrStr _at_t686 = (List_TrStr_get(other->items, i)); List_TrStr_append(fresh, _at_t686); _tr_str_release(_at_t686); });
#line 888 "src/mir.tr"
        i = (i + 1LL);
    }
#line 889 "src/mir.tr"
    self->items = fresh;
}

__attribute__((hot)) LiveSet* LiveSet_minus(LiveSet* self, LiveSet* other) {
#line 893 "src/mir.tr"
    LiveSet* r = LiveSet_init();
#line 894 "src/mir.tr"
    long long i = 0LL;
#line 895 "src/mir.tr"
    while ((i < self->items->len)) {
#line 896 "src/mir.tr"
        if (({ TrStr _at_t687 = (List_TrStr_get(self->items, i)); __auto_type _wr = ((!LiveSet_has(other, _at_t687))); _tr_str_release(_at_t687); _wr; })) {
#line 896 "src/mir.tr"
            ({ TrStr _ord_t688 = (List_TrStr_get(self->items, i)); List_TrStr_append(r->items, _ord_t688); _tr_str_release(_ord_t688); });
        }
#line 897 "src/mir.tr"
        i = (i + 1LL);
    }
#line 898 "src/mir.tr"
    return r;
}

__attribute__((hot)) TrStr _pl_root(HirExpr* e) {
#line 29 "src/mir.tr"
    HirExpr* cur = e;
#line 30 "src/mir.tr"
    long long guard = 0LL;
#line 31 "src/mir.tr"
    while (((((unsigned long long)(cur)) != ((unsigned long long)(0LL))) && (guard < 64LL))) {
#line 32 "src/mir.tr"
        __auto_type _t689 = (*cur);
        if (_t689.tag == HirExpr_EIdent) {
            __auto_type n = _t689.data.EIdent.name;
            TrStr _trret163 = _tr_str_retain(n);
            return _trret163;
        } else if (_t689.tag == HirExpr_EPropAccess) {
            __auto_type o = _t689.data.EPropAccess.obj;
            cur = o;
        } else if (_t689.tag == HirExpr_EIndex) {
            __auto_type o = _t689.data.EIndex.obj;
            cur = o;
        } else if (1) {
            __auto_type _ = _t689;
            return _tr_str_lit_len("", 0LL);
        }
#line 37 "src/mir.tr"
        guard = (guard + 1LL);
    }
#line 38 "src/mir.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr _pl_path(HirExpr* e) {
#line 41 "src/mir.tr"
    List_TrStr* segs = (void*)List_TrStr_new();
#line 42 "src/mir.tr"
    HirExpr* cur = e;
#line 43 "src/mir.tr"
    long long guard = 0LL;
#line 44 "src/mir.tr"
    while (((((unsigned long long)(cur)) != ((unsigned long long)(0LL))) && (guard < 64LL))) {
#line 45 "src/mir.tr"
        __auto_type _t690 = (*cur);
        if (_t690.tag == HirExpr_EPropAccess) {
            __auto_type o = _t690.data.EPropAccess.obj;
__auto_type prop = _t690.data.EPropAccess.prop;
#line 47 "src/mir.tr"
            List_TrStr_append(segs, prop);
#line 48 "src/mir.tr"
            cur = o;
        } else if (_t690.tag == HirExpr_EIndex) {
            __auto_type o = _t690.data.EIndex.obj;
#line 50 "src/mir.tr"
            List_TrStr_append(segs, _tr_str_lit_len("[]", 2LL));
#line 51 "src/mir.tr"
            cur = o;
        } else if (1) {
            __auto_type _ = _t690;
            cur = ((HirExpr*)(0LL));
        }
#line 53 "src/mir.tr"
        guard = (guard + 1LL);
    }
#line 54 "src/mir.tr"
    TrStr out = _tr_str_lit_len("", 0LL);
#line 55 "src/mir.tr"
    long long i = (segs->len - 1LL);
#line 56 "src/mir.tr"
    while ((i >= 0LL)) {
#line 57 "src/mir.tr"
        if ((!_tr_str_eqv((out), (_tr_str_lit_len("", 0LL))))) {
#line 57 "src/mir.tr"
            ({ TrStr _ap_t691 = (_tr_str_lit_len(".", 1LL)); _tr_str_append(&out, _ap_t691); });
        }
#line 58 "src/mir.tr"
        ({ TrStr _ap_t692 = (List_TrStr_get(segs, i)); _tr_str_append(&out, _ap_t692); _tr_str_release(_ap_t692); });
#line 59 "src/mir.tr"
        i = (i - 1LL);
    }
#line 60 "src/mir.tr"
    TrStr _trret164 = _tr_str_retain(out);
    List_TrStr_free(segs);
    _tr_str_release(out);
    return _trret164;
}

__attribute__((hot)) bool _pl_overlap(TrStr a, TrStr b) {
#line 65 "src/mir.tr"
    if ((_tr_str_eqv((a), (_tr_str_lit_len("", 0LL))) || _tr_str_eqv((b), (_tr_str_lit_len("", 0LL))))) {
#line 65 "src/mir.tr"
        return true;
    }
#line 66 "src/mir.tr"
    if (_tr_str_eqv((a), (b))) {
#line 66 "src/mir.tr"
        return true;
    }
#line 67 "src/mir.tr"
    if (_tr_str_starts_withv((a), (_tr_strx_concatv((b), (_tr_str_lit_len(".", 1LL)))))) {
#line 67 "src/mir.tr"
        return true;
    }
#line 68 "src/mir.tr"
    if (_tr_str_starts_withv((b), (_tr_strx_concatv((a), (_tr_str_lit_len(".", 1LL)))))) {
#line 68 "src/mir.tr"
        return true;
    }
#line 69 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool _is_mutating_call_on(HirExpr* val, TrStr source, TrMap* mm, TrStr bpath) {
#line 77 "src/mir.tr"
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
#line 77 "src/mir.tr"
        return false;
    }
#line 78 "src/mir.tr"
    __auto_type _t693 = (*val);
    if (_t693.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t693.data.EMethodCall.obj;
__auto_type method = _t693.data.EMethodCall.method;
#line 80 "src/mir.tr"
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 80 "src/mir.tr"
            return false;
        }
#line 81 "src/mir.tr"
        __auto_type _t694 = (*obj);
        if (_t694.tag == HirExpr_EIdent) {
            __auto_type onm = _t694.data.EIdent.name;
#line 83 "src/mir.tr"
            if ((!_tr_str_eqv((onm), (source)))) {
#line 83 "src/mir.tr"
                return false;
            }
#line 85 "src/mir.tr"
            if ((((((((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("put", 3LL))))) {
#line 86 "src/mir.tr"
                return true;
            }
#line 87 "src/mir.tr"
            bool _trret165 = ({ TrStr _dkt_t695 = (({ TrStr _tr_cl = (_tr_strx_concatv((hir_expr_type(obj)->name), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t695))); _tr_str_release(_dkt_t695); _wr; });
            return _trret165;
        } else if (_t694.tag == HirExpr_EPropAccess) {
#line 89 "src/mir.tr"
            if (({ TrStr _cmpa_t696 = (_pl_root(obj)); TrStr _cmpb_t697 = (source); _Bool _cmpr_t698 = !_tr_str_eqv(_cmpa_t696, _cmpb_t697); _tr_str_release(_cmpa_t696); _cmpr_t698; })) {
#line 89 "src/mir.tr"
                return false;
            }
#line 90 "src/mir.tr"
            if (({ TrStr _at_t699 = (_pl_path(obj)); __auto_type _wr = ((!_pl_overlap(_at_t699, bpath))); _tr_str_release(_at_t699); _wr; })) {
#line 90 "src/mir.tr"
                return false;
            }
#line 91 "src/mir.tr"
            TrStr tn = _tr_str_retain(hir_expr_type(obj)->name);
#line 92 "src/mir.tr"
            if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 93 "src/mir.tr"
                bool _trret166 = (((((((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))));
                _tr_str_release(tn);
                return _trret166;
            }
#line 94 "src/mir.tr"
            bool _trret167 = ({ TrStr _dkt_t700 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t700))); _tr_str_release(_dkt_t700); _wr; });
            _tr_str_release(tn);
            return _trret167;
        } else if (1) {
            __auto_type _ = _t694;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t693;
        return false;
    }
}

__attribute__((hot)) MirStmt* box_mirstmt(MirStmt s) {
#line 113 "src/mir.tr"
    /* unsafe block */
#line 114 "src/mir.tr"
    MirStmt* p = ((MirStmt*)_tr_c_calloc((size_t)(1LL), sizeof(MirStmt)));
#line 115 "src/mir.tr"
    (*p = s);
#line 116 "src/mir.tr"
    return p;
}

__attribute__((hot)) MirTerm* box_mirterm(MirTerm t) {
#line 119 "src/mir.tr"
    /* unsafe block */
#line 120 "src/mir.tr"
    MirTerm* p = ((MirTerm*)_tr_c_calloc((size_t)(1LL), sizeof(MirTerm)));
#line 121 "src/mir.tr"
    (*p = t);
#line 122 "src/mir.tr"
    return p;
}

__attribute__((hot)) void lower_stmts(MirBuilder* b, HirBlock* hb) {
#line 308 "src/mir.tr"
    HirBlock* saved_hb = _tr_obj_retain(b->cur_hb);
#line 309 "src/mir.tr"
    HirBlock* _cltmp_t701 = _tr_obj_retain(hb);
    _tr_obj_release(b->cur_hb, _trdrop_HirBlock);
    b->cur_hb = _cltmp_t701;
#line 310 "src/mir.tr"
    long long i = 0LL;
#line 311 "src/mir.tr"
    while ((i < hb->stmts->len)) {
#line 312 "src/mir.tr"
        if (MirBuilder_terminated(b)) {
#line 315 "src/mir.tr"
            b->cur = MirBuilder_new_block(b);
        }
#line 316 "src/mir.tr"
        HirStmt* sp = ((HirStmt*)List_ptr_get(hb->stmts, i));
#line 317 "src/mir.tr"
        __auto_type _t702 = (*sp);
        if (_t702.tag == HirStmt_SLet) {
            __auto_type nm = _t702.data.SLet.name;
__auto_type slet_ty = _t702.data.SLet.ty;
__auto_type val = _t702.data.SLet.val;
#line 319 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MDeclare(nm, val));
#line 324 "src/mir.tr"
            if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 325 "src/mir.tr"
                __auto_type _t703 = (*val);
                if (_t703.tag == HirExpr_EClosure) {
                    __auto_type ccaps = _t703.data.EClosure.captures;
#line 327 "src/mir.tr"
                    long long cci = 0LL;
#line 328 "src/mir.tr"
                    while ((cci < ccaps->len)) {
#line 329 "src/mir.tr"
                        if ((!((HirParam*)List_ptr_get(ccaps, cci))->by_val)) {
#line 329 "src/mir.tr"
                            ({ TrStr _apt_t704 = _tr_str_retain(((HirParam*)List_ptr_get(ccaps, cci))->name); MirBuilder_record_borrow(b, nm, _apt_t704, false); _tr_str_release(_apt_t704); });
                        }
#line 330 "src/mir.tr"
                        cci = (cci + 1LL);
                    }
                } else if (1) {
                    __auto_type _ = _t703;
                    /* pass */
                }
            }
#line 335 "src/mir.tr"
            if ((slet_ty->is_borrow && (((unsigned long long)(val)) != ((unsigned long long)(0LL))))) {
#line 336 "src/mir.tr"
                bool _slet_mut = false;
#line 337 "src/mir.tr"
                __auto_type _t705 = (*sp);
                if (_t705.tag == HirStmt_SLet) {
                    __auto_type _sm = _t705.data.SLet.is_mut;
                    _slet_mut = _sm;
                } else if (1) {
                    __auto_type _ = _t705;
                    /* pass */
                }
#line 340 "src/mir.tr"
                __auto_type _t706 = (*val);
                if (_t706.tag == HirExpr_EIdent) {
                    __auto_type src_nm = _t706.data.EIdent.name;
                    MirBuilder_record_borrow(b, nm, src_nm, (_slet_mut || slet_ty->is_mut_borrow));
                } else if (_t706.tag == HirExpr_EMethodCall) {
                    __auto_type mobj = _t706.data.EMethodCall.obj;
__auto_type mmethod = _t706.data.EMethodCall.method;
#line 347 "src/mir.tr"
                    if ((_tr_str_eqv((mmethod), (_tr_str_lit_len("get", 3LL))) || _tr_str_eqv((mmethod), (_tr_str_lit_len("get_or", 6LL))))) {
#line 348 "src/mir.tr"
                        __auto_type _t707 = (*mobj);
                        if (_t707.tag == HirExpr_EIdent) {
                            __auto_type cnm = _t707.data.EIdent.name;
                            MirBuilder_record_coll_borrow(b, nm, cnm, (_slet_mut || slet_ty->is_mut_borrow));
                        } else if (1) {
                            __auto_type _ = _t707;
                            /* pass */
                        }
                    }
                } else if (_t706.tag == HirExpr_EPropAccess) {
#line 353 "src/mir.tr"
                    TrStr pb_root = _pl_root(val);
#line 354 "src/mir.tr"
                    if ((!_tr_str_eqv((pb_root), (_tr_str_lit_len("", 0LL))))) {
#line 354 "src/mir.tr"
                        ({ TrStr _ord_t708 = (_pl_path(val)); __auto_type _ord_t709 = ((_slet_mut || slet_ty->is_mut_borrow)); MirBuilder_record_path_borrow(b, nm, pb_root, _ord_t708, _ord_t709); _tr_str_release(_ord_t708); });
                    }
                    _tr_str_release(pb_root);
                } else if (_t706.tag == HirExpr_EIndex) {
                    __auto_type iobj = _t706.data.EIndex.obj;
#line 357 "src/mir.tr"
                    __auto_type _t710 = (*iobj);
                    if (_t710.tag == HirExpr_EIdent) {
                        __auto_type cnm2 = _t710.data.EIdent.name;
                        MirBuilder_record_coll_borrow(b, nm, cnm2, (_slet_mut || slet_ty->is_mut_borrow));
                    } else if (1) {
                        __auto_type _ = _t710;
                        /* pass */
                    }
                } else if (1) {
                    __auto_type _ = _t706;
                    /* pass */
                }
            }
        } else if (_t702.tag == HirStmt_SAssign) {
            __auto_type tgt = _t702.data.SAssign.target;
__auto_type val = _t702.data.SAssign.val;
#line 364 "src/mir.tr"
            __auto_type _t711 = (*tgt);
            if (_t711.tag == HirExpr_EIdent) {
                __auto_type tn = _t711.data.EIdent.name;
                MirBuilder_push_stmt(b, MirStmt_ctor_MAssign(tn, val));
            } else if (1) {
                __auto_type _ = _t711;
#line 367 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(val));
#line 368 "src/mir.tr"
                TrStr pw_root = _pl_root(tgt);
#line 369 "src/mir.tr"
                if ((!_tr_str_eqv((pw_root), (_tr_str_lit_len("", 0LL))))) {
#line 369 "src/mir.tr"
                    ({ TrStr _at_t712 = (_pl_path(tgt)); MirBuilder_record_pwrite(b, pw_root, _at_t712); _tr_str_release(_at_t712); });
                }
#line 371 "src/mir.tr"
                if (((!_tr_str_eqv((pw_root), (_tr_str_lit_len("", 0LL)))) && hir_expr_type(tgt)->is_borrow)) {
#line 372 "src/mir.tr"
                    TrStr fr_src = _pl_root(val);
#line 373 "src/mir.tr"
                    if (((!_tr_str_eqv((fr_src), (_tr_str_lit_len("", 0LL)))) && (!_tr_str_eqv((fr_src), (pw_root))))) {
#line 374 "src/mir.tr"
                        __auto_type _t713 = (*val);
                        if (_t713.tag == HirExpr_EIdent) {
                            MirBuilder_record_borrow(b, pw_root, fr_src, false);
                        } else if (1) {
                            __auto_type _ = _t713;
                            ({ TrStr _at_t714 = (_pl_path(val)); MirBuilder_record_path_borrow(b, pw_root, fr_src, _at_t714, false); _tr_str_release(_at_t714); });
                        }
                    }
                }
                _tr_str_release(pw_root);
            }
        } else if (_t702.tag == HirStmt_SExpr) {
            __auto_type e = _t702.data.SExpr.expr;
#line 378 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(e));
#line 380 "src/mir.tr"
            __auto_type _t715 = (*e);
            if (_t715.tag == HirExpr_EMethodCall) {
                __auto_type hobj = _t715.data.EMethodCall.obj;
__auto_type hm = _t715.data.EMethodCall.method;
__auto_type hargs = _t715.data.EMethodCall.args;
#line 382 "src/mir.tr"
                if (((_tr_str_eqv((hm), (_tr_str_lit_len("append", 6LL))) || _tr_str_eqv((hm), (_tr_str_lit_len("push", 4LL)))) || _tr_str_eqv((hm), (_tr_str_lit_len("insert", 6LL))))) {
#line 383 "src/mir.tr"
                    __auto_type _t716 = (*hobj);
                    if (_t716.tag == HirExpr_EIdent) {
                        __auto_type hname = _t716.data.EIdent.name;
__auto_type hty = _t716.data.EIdent.ty;
#line 385 "src/mir.tr"
                        if (((hty->args->len > 0LL) && (*((AstType**)List_ptr_get(hty->args, 0LL)))->is_borrow)) {
#line 386 "src/mir.tr"
                            long long hai = 0LL;
#line 387 "src/mir.tr"
                            while ((hai < hargs->len)) {
#line 388 "src/mir.tr"
                                __auto_type _t717 = (*((HirExpr*)List_ptr_get(hargs, hai)));
                                if (_t717.tag == HirExpr_EIdent) {
                                    __auto_type hsrc = _t717.data.EIdent.name;
                                    MirBuilder_record_borrow(b, hname, hsrc, false);
                                } else if (1) {
                                    __auto_type _ = _t717;
                                    /* pass */
                                }
#line 391 "src/mir.tr"
                                hai = (hai + 1LL);
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t716;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t715;
                /* pass */
            }
        } else if (_t702.tag == HirStmt_SReturn) {
            __auto_type v = _t702.data.SReturn.val;
#line 395 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TReturn(v));
        } else if (_t702.tag == HirStmt_SIf) {
            __auto_type cond = _t702.data.SIf.cond;
__auto_type then_b = _t702.data.SIf.then_b;
__auto_type else_b = _t702.data.SIf.else_b;
#line 397 "src/mir.tr"
            long long tb = MirBuilder_new_block(b);
#line 398 "src/mir.tr"
            long long eb = MirBuilder_new_block(b);
#line 399 "src/mir.tr"
            long long jb = MirBuilder_new_block(b);
#line 403 "src/mir.tr"
            if ((b->in_unsafe == 0LL)) {
#line 404 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(then_b));
#line 405 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(else_b));
            }
#line 406 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, tb, eb));
#line 407 "src/mir.tr"
            b->cur = tb;
#line 408 "src/mir.tr"
            lower_stmts(b, then_b);
#line 409 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 409 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
#line 410 "src/mir.tr"
            b->cur = eb;
#line 411 "src/mir.tr"
            lower_stmts(b, else_b);
#line 412 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 412 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(jb));
            }
#line 413 "src/mir.tr"
            b->cur = jb;
        } else if (_t702.tag == HirStmt_SWhile) {
            __auto_type cond = _t702.data.SWhile.cond;
__auto_type body = _t702.data.SWhile.body;
#line 415 "src/mir.tr"
            long long hdr = MirBuilder_new_block(b);
#line 416 "src/mir.tr"
            long long body_b = MirBuilder_new_block(b);
#line 417 "src/mir.tr"
            long long exit_b = MirBuilder_new_block(b);
#line 418 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
#line 419 "src/mir.tr"
            b->cur = hdr;
#line 420 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(cond, body_b, exit_b));
#line 421 "src/mir.tr"
            b->cur = body_b;
#line 422 "src/mir.tr"
            List_i64_append(b->loop_continue, hdr);
#line 423 "src/mir.tr"
            List_i64_append(b->loop_break, exit_b);
#line 424 "src/mir.tr"
            lower_stmts(b, body);
#line 425 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 426 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 427 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 427 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(hdr));
            }
#line 428 "src/mir.tr"
            b->cur = exit_b;
        } else if (_t702.tag == HirStmt_SFor) {
            __auto_type fiter = _t702.data.SFor.iter;
__auto_type fbody = _t702.data.SFor.body;
#line 435 "src/mir.tr"
            long long for_hdr = MirBuilder_new_block(b);
#line 436 "src/mir.tr"
            long long for_body = MirBuilder_new_block(b);
#line 437 "src/mir.tr"
            long long for_exit = MirBuilder_new_block(b);
#line 438 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
#line 439 "src/mir.tr"
            b->cur = for_hdr;
#line 440 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fiter, for_body, for_exit));
#line 441 "src/mir.tr"
            b->cur = for_body;
#line 442 "src/mir.tr"
            List_i64_append(b->loop_continue, for_hdr);
#line 443 "src/mir.tr"
            List_i64_append(b->loop_break, for_exit);
#line 444 "src/mir.tr"
            lower_stmts(b, fbody);
#line 445 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 446 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 447 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 447 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(for_hdr));
            }
#line 448 "src/mir.tr"
            b->cur = for_exit;
        } else if (_t702.tag == HirStmt_SForUnpack) {
            __auto_type fuiter = _t702.data.SForUnpack.iter;
__auto_type fubody = _t702.data.SForUnpack.body;
#line 450 "src/mir.tr"
            long long fu_hdr = MirBuilder_new_block(b);
#line 451 "src/mir.tr"
            long long fu_body = MirBuilder_new_block(b);
#line 452 "src/mir.tr"
            long long fu_exit = MirBuilder_new_block(b);
#line 453 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
#line 454 "src/mir.tr"
            b->cur = fu_hdr;
#line 455 "src/mir.tr"
            MirBuilder_set_term(b, MirTerm_ctor_TBranch(fuiter, fu_body, fu_exit));
#line 456 "src/mir.tr"
            b->cur = fu_body;
#line 457 "src/mir.tr"
            List_i64_append(b->loop_continue, fu_hdr);
#line 458 "src/mir.tr"
            List_i64_append(b->loop_break, fu_exit);
#line 459 "src/mir.tr"
            lower_stmts(b, fubody);
#line 460 "src/mir.tr"
            List_i64_pop(b->loop_continue);
#line 461 "src/mir.tr"
            List_i64_pop(b->loop_break);
#line 462 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 462 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fu_hdr));
            }
#line 463 "src/mir.tr"
            b->cur = fu_exit;
        } else if (_t702.tag == HirStmt_STry) {
            __auto_type try_body = _t702.data.STry.try_body;
__auto_type catches = _t702.data.STry.catches;
__auto_type finally_b = _t702.data.STry.finally_b;
#line 471 "src/mir.tr"
            long long try_b = MirBuilder_new_block(b);
#line 472 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 472 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_b));
            }
#line 473 "src/mir.tr"
            b->cur = try_b;
#line 474 "src/mir.tr"
            List_ptr_append(b->if_bodies, _tr_obj_retain(try_body));
#line 475 "src/mir.tr"
            lower_stmts(b, try_body);
#line 476 "src/mir.tr"
            long long tci = 0LL;
#line 477 "src/mir.tr"
            while ((tci < catches->len)) {
#line 478 "src/mir.tr"
                HirCatchClause* tcc = (*((HirCatchClause**)List_ptr_get(catches, tci)));
#line 479 "src/mir.tr"
                long long catch_b = MirBuilder_new_block(b);
#line 480 "src/mir.tr"
                if ((!MirBuilder_terminated(b))) {
#line 480 "src/mir.tr"
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(catch_b));
                }
#line 481 "src/mir.tr"
                b->cur = catch_b;
#line 482 "src/mir.tr"
                List_ptr_append(b->if_bodies, _tr_obj_retain(tcc->body));
#line 483 "src/mir.tr"
                lower_stmts(b, tcc->body);
#line 484 "src/mir.tr"
                tci = (tci + 1LL);
            }
#line 485 "src/mir.tr"
            long long fin_b = MirBuilder_new_block(b);
#line 486 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 486 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(fin_b));
            }
#line 487 "src/mir.tr"
            b->cur = fin_b;
#line 488 "src/mir.tr"
            List_ptr_append(b->if_bodies, _tr_obj_retain(finally_b));
#line 489 "src/mir.tr"
            lower_stmts(b, finally_b);
#line 490 "src/mir.tr"
            long long try_after = MirBuilder_new_block(b);
#line 491 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 491 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(try_after));
            }
#line 492 "src/mir.tr"
            b->cur = try_after;
        } else if (_t702.tag == HirStmt_SMatch) {
            __auto_type msubj = _t702.data.SMatch.expr;
__auto_type marms = _t702.data.SMatch.arms;
#line 499 "src/mir.tr"
            long long m_exit = MirBuilder_new_block(b);
#line 500 "src/mir.tr"
            long long mai = 0LL;
#line 501 "src/mir.tr"
            while ((mai < marms->len)) {
#line 502 "src/mir.tr"
                HirMatchArm* marm = ((HirMatchArm*)List_ptr_get(marms, mai));
#line 503 "src/mir.tr"
                long long arm_body = MirBuilder_new_block(b);
#line 504 "src/mir.tr"
                long long arm_next = MirBuilder_new_block(b);
#line 505 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TBranch(msubj, arm_body, arm_next));
#line 506 "src/mir.tr"
                b->cur = arm_body;
#line 507 "src/mir.tr"
                if ((((unsigned long long)(marm->guard)) != ((unsigned long long)(0LL)))) {
#line 508 "src/mir.tr"
                    MirBuilder_push_stmt(b, MirStmt_ctor_MEval(marm->guard));
                }
#line 509 "src/mir.tr"
                if ((b->in_unsafe == 0LL)) {
#line 509 "src/mir.tr"
                    List_ptr_append(b->if_bodies, _tr_obj_retain(marm->body));
                }
#line 510 "src/mir.tr"
                lower_stmts(b, marm->body);
#line 511 "src/mir.tr"
                if ((!MirBuilder_terminated(b))) {
#line 511 "src/mir.tr"
                    MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
                }
#line 512 "src/mir.tr"
                b->cur = arm_next;
#line 513 "src/mir.tr"
                mai = (mai + 1LL);
            }
#line 514 "src/mir.tr"
            if ((!MirBuilder_terminated(b))) {
#line 514 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(m_exit));
            }
#line 515 "src/mir.tr"
            b->cur = m_exit;
        } else if (_t702.tag == HirStmt_SAssert) {
            __auto_type acond = _t702.data.SAssert.cond;
__auto_type amsg = _t702.data.SAssert.msg;
#line 517 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(acond));
#line 518 "src/mir.tr"
            if ((((unsigned long long)(amsg)) != ((unsigned long long)(0LL)))) {
#line 519 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(amsg));
            }
        } else if (_t702.tag == HirStmt_SUnsafe) {
            __auto_type ubody = _t702.data.SUnsafe.body;
#line 539 "src/mir.tr"
            b->in_unsafe = (b->in_unsafe + 1LL);
#line 540 "src/mir.tr"
            lower_stmts(b, ubody);
#line 541 "src/mir.tr"
            b->in_unsafe = (b->in_unsafe - 1LL);
        } else if (_t702.tag == HirStmt_SRaise) {
            __auto_type rval = _t702.data.SRaise.val;
#line 546 "src/mir.tr"
            if ((((unsigned long long)(rval)) != ((unsigned long long)(0LL)))) {
#line 547 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(rval));
            }
        } else if (_t702.tag == HirStmt_SWith) {
            __auto_type witems = _t702.data.SWith.items;
__auto_type wbody = _t702.data.SWith.body;
#line 552 "src/mir.tr"
            long long wi = 0LL;
#line 553 "src/mir.tr"
            while ((wi < witems->len)) {
#line 554 "src/mir.tr"
                MirBuilder_push_stmt(b, MirStmt_ctor_MEval(((HirExpr*)List_ptr_get(witems, wi))));
#line 555 "src/mir.tr"
                wi = (wi + 1LL);
            }
#line 556 "src/mir.tr"
            lower_stmts(b, wbody);
        } else if (_t702.tag == HirStmt_SMultiLet) {
            __auto_type mlval = _t702.data.SMultiLet.val;
#line 561 "src/mir.tr"
            MirBuilder_push_stmt(b, MirStmt_ctor_MEval(mlval));
        } else if (_t702.tag == HirStmt_SLineMarker) {
            __auto_type _ = _t702.data.SLineMarker.n;
#line 563 "src/mir.tr"
            /* pass */
        } else if (_t702.tag == HirStmt_SAutoDrop) {
#line 565 "src/mir.tr"
            /* pass */
        } else if (_t702.tag == HirStmt_SBreak) {
            __auto_type _ = _t702.data.SBreak.val;
#line 573 "src/mir.tr"
            if ((b->loop_break->len > 0LL)) {
#line 574 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_break, (b->loop_break->len - 1LL))));
            } else {
#line 576 "src/mir.tr"
                b->complete = false;
            }
        } else if (_t702.tag == HirStmt_SContinue) {
#line 580 "src/mir.tr"
            if ((b->loop_continue->len > 0LL)) {
#line 581 "src/mir.tr"
                MirBuilder_set_term(b, MirTerm_ctor_TGoto(List_i64_get(b->loop_continue, (b->loop_continue->len - 1LL))));
            } else {
#line 583 "src/mir.tr"
                b->complete = false;
            }
        } else if (1) {
            __auto_type _ = _t702;
#line 588 "src/mir.tr"
            b->complete = false;
        }
#line 589 "src/mir.tr"
        i = (i + 1LL);
    }
#line 590 "src/mir.tr"
    HirBlock* _cltmp_t718 = _tr_obj_retain(saved_hb);
    _tr_obj_release(b->cur_hb, _trdrop_HirBlock);
    b->cur_hb = _cltmp_t718;
    _tr_obj_release(saved_hb, _trdrop_HirBlock);
}

__attribute__((hot)) MirFunction* lower_function(HirFunction* hf) {
#line 593 "src/mir.tr"
    MirFunction* mf = ((MirFunction*)_tr_obj_alloc(sizeof(MirFunction)));
#line 594 "src/mir.tr"
    TrStr _sfo_t719 = mf->name;
    mf->name = _tr_str_retain(hf->name);
    _tr_str_release(_sfo_t719);
#line 595 "src/mir.tr"
    mf->params = (void*)List_TrStr_new();
#line 596 "src/mir.tr"
    long long pi = 0LL;
#line 597 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 598 "src/mir.tr"
        ({ __auto_type _ord_t720 = (((HirParam*)List_ptr_get(hf->params, pi))->name); List_TrStr_append(mf->params, _ord_t720); });
#line 599 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 600 "src/mir.tr"
    MirBuilder* b = MirBuilder_init();
#line 601 "src/mir.tr"
    b->cur = MirBuilder_new_block(b);
#line 602 "src/mir.tr"
    lower_stmts(b, hf->body);
#line 603 "src/mir.tr"
    if ((!MirBuilder_terminated(b))) {
#line 603 "src/mir.tr"
        MirBuilder_set_term(b, MirTerm_make_TReturnVoid());
    }
#line 610 "src/mir.tr"
    long long rbi = 0LL;
#line 611 "src/mir.tr"
    while ((rbi < b->blocks->len)) {
#line 612 "src/mir.tr"
        MirBlock* rblk = ((MirBlock*)List_ptr_get(b->blocks, rbi));
#line 613 "src/mir.tr"
        long long rsi = 0LL;
#line 614 "src/mir.tr"
        while ((rsi < rblk->stmts->len)) {
#line 615 "src/mir.tr"
            __auto_type _t721 = (*((MirStmt*)List_ptr_get(rblk->stmts, rsi)));
            if (_t721.tag == MirStmt_MDeclare) {
                __auto_type rv = _t721.data.MDeclare.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t721.tag == MirStmt_MAssign) {
                __auto_type rv = _t721.data.MAssign.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            } else if (_t721.tag == MirStmt_MEval) {
                __auto_type rv = _t721.data.MEval.value;
                collect_raw_borrows(rv, b->unsafe_pinned);
            }
#line 619 "src/mir.tr"
            rsi = (rsi + 1LL);
        }
#line 620 "src/mir.tr"
        __auto_type _t722 = (*rblk->term);
        if (_t722.tag == MirTerm_TBranch) {
            __auto_type rc = _t722.data.TBranch.cond;
            collect_raw_borrows(rc, b->unsafe_pinned);
        } else if (_t722.tag == MirTerm_TReturn) {
            __auto_type rv = _t722.data.TReturn.value;
            collect_raw_borrows(rv, b->unsafe_pinned);
        } else if (1) {
            __auto_type _ = _t722;
            /* pass */
        }
#line 624 "src/mir.tr"
        rbi = (rbi + 1LL);
    }
#line 625 "src/mir.tr"
    mf->blocks = b->blocks;
#line 626 "src/mir.tr"
    mf->complete = b->complete;
#line 627 "src/mir.tr"
    mf->if_bodies = b->if_bodies;
#line 628 "src/mir.tr"
    LiveSet* _cltmp_t723 = _tr_obj_retain(b->unsafe_pinned);
    _tr_obj_release(mf->unsafe_pinned, _trdrop_LiveSet);
    mf->unsafe_pinned = _cltmp_t723;
#line 629 "src/mir.tr"
    mf->borrows = b->borrows;
#line 630 "src/mir.tr"
    mf->pwrites = b->pwrites;
#line 634 "src/mir.tr"
    long long hbi = 0LL;
#line 635 "src/mir.tr"
    if ((((unsigned long long)(hf->borrow_borrowers)) == ((unsigned long long)(0LL)))) {
#line 635 "src/mir.tr"
        _tr_obj_release(b, _trdrop_MirBuilder);
        return mf;
    }
#line 636 "src/mir.tr"
    if ((((unsigned long long)(hf->borrow_sources)) == ((unsigned long long)(0LL)))) {
#line 636 "src/mir.tr"
        _tr_obj_release(b, _trdrop_MirBuilder);
        return mf;
    }
#line 637 "src/mir.tr"
    while (((hbi < hf->borrow_borrowers->len) && (hbi < hf->borrow_sources->len))) {
#line 638 "src/mir.tr"
        bool he_dup = false;
#line 639 "src/mir.tr"
        long long hdi = 0LL;
#line 640 "src/mir.tr"
        while ((hdi < mf->borrows->len)) {
#line 641 "src/mir.tr"
            if ((({ __auto_type _ord_t724 = (((BorrowEdge*)List_ptr_get(mf->borrows, hdi))->borrower); __auto_type _wr = (({ TrStr _cmpa_t725 = (_ord_t724); TrStr _cmpb_t726 = (List_TrStr_get(hf->borrow_borrowers, hbi)); _Bool _cmpr_t727 = _tr_str_eqv(_cmpa_t725, _cmpb_t726); _tr_str_release(_cmpb_t726); _cmpr_t727; })); _wr; }) && ({ __auto_type _ord_t728 = (((BorrowEdge*)List_ptr_get(mf->borrows, hdi))->source); __auto_type _wr = (({ TrStr _cmpa_t729 = (_ord_t728); TrStr _cmpb_t730 = (List_TrStr_get(hf->borrow_sources, hbi)); _Bool _cmpr_t731 = _tr_str_eqv(_cmpa_t729, _cmpb_t730); _tr_str_release(_cmpb_t730); _cmpr_t731; })); _wr; }))) {
#line 641 "src/mir.tr"
                he_dup = true;
            }
#line 642 "src/mir.tr"
            hdi = (hdi + 1LL);
        }
#line 643 "src/mir.tr"
        if (he_dup) {
#line 644 "src/mir.tr"
            hbi = (hbi + 1LL);
#line 645 "src/mir.tr"
            continue;
        }
#line 646 "src/mir.tr"
        BorrowEdge* he = ((BorrowEdge*)_tr_obj_alloc(sizeof(BorrowEdge)));
#line 647 "src/mir.tr"
        TrStr _sfo_t732 = he->borrower;
        he->borrower = List_TrStr_get(hf->borrow_borrowers, hbi);
        _tr_str_release(_sfo_t732);
#line 648 "src/mir.tr"
        TrStr _sfo_t733 = he->source;
        he->source = List_TrStr_get(hf->borrow_sources, hbi);
        _tr_str_release(_sfo_t733);
#line 649 "src/mir.tr"
        TrStr _sfo_t734 = he->path;
        he->path = _tr_str_lit_len("", 0LL);
        _tr_str_release(_sfo_t734);
#line 650 "src/mir.tr"
        he->decl_idx = (-1LL);
#line 651 "src/mir.tr"
        he->decl_block = (-1LL);
#line 653 "src/mir.tr"
        List_ptr_append(mf->borrows, _tr_obj_retain(he));
#line 654 "src/mir.tr"
        hbi = (hbi + 1LL);
        _tr_obj_release(he, _trdrop_BorrowEdge);
    }
#line 655 "src/mir.tr"
    _tr_obj_release(b, _trdrop_MirBuilder);
    return mf;
}

__attribute__((hot)) MirProgram* lower_program(HirProgram* hir) {
#line 658 "src/mir.tr"
    MirProgram* mp = MirProgram_init();
#line 659 "src/mir.tr"
    long long i = 0LL;
#line 660 "src/mir.tr"
    while ((i < hir->functions->len)) {
#line 661 "src/mir.tr"
        ({ MirFunction* _ord_t735 = (lower_function(((HirFunction*)List_ptr_get(hir->functions, i)))); List_ptr_append(mp->functions, _tr_obj_retain(_ord_t735)); _tr_obj_release(_ord_t735, _trdrop_MirFunction); });
#line 662 "src/mir.tr"
        i = (i + 1LL);
    }
#line 663 "src/mir.tr"
    return mp;
}

__attribute__((hot)) bool set_contains(List_TrStr* v, TrStr s) {
#line 673 "src/mir.tr"
    long long i = 0LL;
#line 674 "src/mir.tr"
    while ((i < v->len)) {
#line 675 "src/mir.tr"
        if (({ TrStr _cmpa_t736 = (List_TrStr_get(v, i)); TrStr _cmpb_t737 = (s); _Bool _cmpr_t738 = _tr_str_eqv(_cmpa_t736, _cmpb_t737); _tr_str_release(_cmpa_t736); _cmpr_t738; })) {
#line 675 "src/mir.tr"
            return true;
        }
#line 676 "src/mir.tr"
        i = (i + 1LL);
    }
#line 677 "src/mir.tr"
    return false;
}

__attribute__((hot)) bool set_add(List_TrStr* v, TrStr s) {
#line 680 "src/mir.tr"
    if (set_contains(v, s)) {
#line 680 "src/mir.tr"
        return false;
    }
#line 681 "src/mir.tr"
    List_TrStr_append(v, s);
#line 682 "src/mir.tr"
    return true;
}

__attribute__((hot)) void collect_uses(HirExpr* e, List_TrStr* out) {
#line 686 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 686 "src/mir.tr"
        return;
    }
#line 687 "src/mir.tr"
    __auto_type _t739 = (*e);
    if (_t739.tag == HirExpr_EIdent) {
        __auto_type nm = _t739.data.EIdent.name;
        set_add(out, nm);
    } else if (_t739.tag == HirExpr_EBinOp) {
        __auto_type l = _t739.data.EBinOp.left;
__auto_type r = _t739.data.EBinOp.right;
#line 690 "src/mir.tr"
        collect_uses(l, out);
#line 691 "src/mir.tr"
        collect_uses(r, out);
    } else if (_t739.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t739.data.EUnaryOp.expr;
        collect_uses(x, out);
    } else if (_t739.tag == HirExpr_ECall) {
        __auto_type callee = _t739.data.ECall.callee;
__auto_type args = _t739.data.ECall.args;
#line 694 "src/mir.tr"
        collect_uses(callee, out);
#line 695 "src/mir.tr"
        long long i = 0LL;
#line 696 "src/mir.tr"
        while ((i < args->len)) {
#line 697 "src/mir.tr"
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
#line 698 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t739.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t739.data.EMethodCall.obj;
__auto_type args = _t739.data.EMethodCall.args;
#line 700 "src/mir.tr"
        collect_uses(obj, out);
#line 701 "src/mir.tr"
        long long i = 0LL;
#line 702 "src/mir.tr"
        while ((i < args->len)) {
#line 703 "src/mir.tr"
            collect_uses(((HirExpr*)List_ptr_get(args, i)), out);
#line 704 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t739.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t739.data.EPropAccess.obj;
        collect_uses(obj, out);
    } else if (_t739.tag == HirExpr_EIndex) {
        __auto_type obj = _t739.data.EIndex.obj;
__auto_type idx = _t739.data.EIndex._tr_v_index;
#line 707 "src/mir.tr"
        collect_uses(obj, out);
#line 708 "src/mir.tr"
        collect_uses(idx, out);
    } else if (_t739.tag == HirExpr_ECast) {
        __auto_type x = _t739.data.ECast.expr;
        collect_uses(x, out);
    } else if (_t739.tag == HirExpr_EAwait) {
        __auto_type x = _t739.data.EAwait.expr;
        collect_uses(x, out);
    } else if (_t739.tag == HirExpr_EIfElse) {
        __auto_type c = _t739.data.EIfElse.cond;
__auto_type t = _t739.data.EIfElse.then_e;
__auto_type f = _t739.data.EIfElse.else_e;
#line 712 "src/mir.tr"
        collect_uses(c, out);
#line 713 "src/mir.tr"
        collect_uses(t, out);
#line 714 "src/mir.tr"
        collect_uses(f, out);
    } else if (1) {
        __auto_type _ = _t739;
        /* pass */
    }
}

__attribute__((hot)) void collect_raw_borrows(HirExpr* e, LiveSet* out) {
#line 729 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 729 "src/mir.tr"
        return;
    }
#line 730 "src/mir.tr"
    __auto_type _t740 = (*e);
    if (_t740.tag == HirExpr_ECast) {
        __auto_type inner = _t740.data.ECast.expr;
__auto_type ty = _t740.data.ECast.target_ty;
#line 732 "src/mir.tr"
        if (_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 733 "src/mir.tr"
            __auto_type _t741 = (*inner);
            if (_t741.tag == HirExpr_EIdent) {
                __auto_type nm = _t741.data.EIdent.name;
                bool _a = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t741;
                /* pass */
            }
        }
#line 736 "src/mir.tr"
        collect_raw_borrows(inner, out);
    } else if (_t740.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t740.data.EUnaryOp.op;
__auto_type inner = _t740.data.EUnaryOp.expr;
#line 738 "src/mir.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("&", 1LL)))) {
#line 739 "src/mir.tr"
            __auto_type _t742 = (*inner);
            if (_t742.tag == HirExpr_EIdent) {
                __auto_type nm = _t742.data.EIdent.name;
                bool _b = LiveSet_add(out, nm);
            } else if (1) {
                __auto_type _ = _t742;
                /* pass */
            }
        }
#line 742 "src/mir.tr"
        collect_raw_borrows(inner, out);
    } else if (_t740.tag == HirExpr_EBinOp) {
        __auto_type l = _t740.data.EBinOp.left;
__auto_type r = _t740.data.EBinOp.right;
#line 744 "src/mir.tr"
        collect_raw_borrows(l, out);
#line 745 "src/mir.tr"
        collect_raw_borrows(r, out);
    } else if (_t740.tag == HirExpr_ECall) {
        __auto_type callee = _t740.data.ECall.callee;
__auto_type args = _t740.data.ECall.args;
#line 747 "src/mir.tr"
        collect_raw_borrows(callee, out);
#line 748 "src/mir.tr"
        long long i = 0LL;
#line 749 "src/mir.tr"
        while ((i < args->len)) {
#line 750 "src/mir.tr"
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
#line 751 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t740.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t740.data.EMethodCall.obj;
__auto_type args = _t740.data.EMethodCall.args;
#line 753 "src/mir.tr"
        collect_raw_borrows(obj, out);
#line 754 "src/mir.tr"
        long long i = 0LL;
#line 755 "src/mir.tr"
        while ((i < args->len)) {
#line 756 "src/mir.tr"
            collect_raw_borrows(((HirExpr*)List_ptr_get(args, i)), out);
#line 757 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t740.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t740.data.EPropAccess.obj;
        collect_raw_borrows(obj, out);
    } else if (_t740.tag == HirExpr_EIndex) {
        __auto_type obj = _t740.data.EIndex.obj;
__auto_type idx = _t740.data.EIndex._tr_v_index;
#line 760 "src/mir.tr"
        collect_raw_borrows(obj, out);
#line 761 "src/mir.tr"
        collect_raw_borrows(idx, out);
    } else if (_t740.tag == HirExpr_EAwait) {
        __auto_type x = _t740.data.EAwait.expr;
        collect_raw_borrows(x, out);
    } else if (_t740.tag == HirExpr_EIfElse) {
        __auto_type c = _t740.data.EIfElse.cond;
__auto_type t = _t740.data.EIfElse.then_e;
__auto_type f = _t740.data.EIfElse.else_e;
#line 764 "src/mir.tr"
        collect_raw_borrows(c, out);
#line 765 "src/mir.tr"
        collect_raw_borrows(t, out);
#line 766 "src/mir.tr"
        collect_raw_borrows(f, out);
    } else if (1) {
        __auto_type _ = _t740;
        /* pass */
    }
}

__attribute__((hot)) void add_exposed(HirExpr* e, List_TrStr* gen, List_TrStr* kill) {
#line 774 "src/mir.tr"
    List_TrStr* tmp = (void*)List_TrStr_new();
#line 775 "src/mir.tr"
    collect_uses(e, tmp);
#line 776 "src/mir.tr"
    long long i = 0LL;
#line 777 "src/mir.tr"
    while ((i < tmp->len)) {
#line 778 "src/mir.tr"
        if (({ TrStr _at_t743 = (List_TrStr_get(tmp, i)); __auto_type _wr = ((!set_contains(kill, _at_t743))); _tr_str_release(_at_t743); _wr; })) {
#line 778 "src/mir.tr"
            ({ TrStr _at_t744 = (List_TrStr_get(tmp, i)); set_add(gen, _at_t744); _tr_str_release(_at_t744); });
        }
#line 779 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_use_def(MirBlock* blk, List_TrStr* gen, List_TrStr* kill) {
#line 784 "src/mir.tr"
    long long i = 0LL;
#line 785 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 786 "src/mir.tr"
        __auto_type _t745 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t745.tag == MirStmt_MDeclare) {
            __auto_type p = _t745.data.MDeclare.place;
__auto_type val = _t745.data.MDeclare.value;
#line 788 "src/mir.tr"
            add_exposed(val, gen, kill);
#line 789 "src/mir.tr"
            set_add(kill, p);
        } else if (_t745.tag == MirStmt_MAssign) {
            __auto_type p = _t745.data.MAssign.place;
__auto_type val = _t745.data.MAssign.value;
#line 791 "src/mir.tr"
            add_exposed(val, gen, kill);
#line 792 "src/mir.tr"
            set_add(kill, p);
        } else if (_t745.tag == MirStmt_MEval) {
            __auto_type val = _t745.data.MEval.value;
            add_exposed(val, gen, kill);
        }
#line 794 "src/mir.tr"
        i = (i + 1LL);
    }
#line 795 "src/mir.tr"
    __auto_type _t746 = (*blk->term);
    if (_t746.tag == MirTerm_TBranch) {
        __auto_type cond = _t746.data.TBranch.cond;
        add_exposed(cond, gen, kill);
    } else if (_t746.tag == MirTerm_TReturn) {
        __auto_type val = _t746.data.TReturn.value;
        add_exposed(val, gen, kill);
    } else if (1) {
        __auto_type _ = _t746;
        /* pass */
    }
}

__attribute__((hot)) void block_succs(MirBlock* blk, List_i64* out) {
#line 801 "src/mir.tr"
    __auto_type _t747 = (*blk->term);
    if (_t747.tag == MirTerm_TGoto) {
        __auto_type n = _t747.data.TGoto.target;
        List_i64_append(out, n);
    } else if (_t747.tag == MirTerm_TBranch) {
        __auto_type t = _t747.data.TBranch.t;
__auto_type e = _t747.data.TBranch.e;
#line 804 "src/mir.tr"
        List_i64_append(out, t);
#line 805 "src/mir.tr"
        List_i64_append(out, e);
    } else if (1) {
        __auto_type _ = _t747;
        /* pass */
    }
}

__attribute__((hot)) List_ptr* compute_liveness(MirFunction* mf) {
#line 905 "src/mir.tr"
    long long n = mf->blocks->len;
#line 906 "src/mir.tr"
    List_ptr* uses = (void*)List_ptr_new();
#line 907 "src/mir.tr"
    List_ptr* defs = (void*)List_ptr_new();
#line 908 "src/mir.tr"
    List_ptr* live_in = (void*)List_ptr_new();
#line 909 "src/mir.tr"
    List_ptr* live_out = (void*)List_ptr_new();
#line 910 "src/mir.tr"
    long long i = 0LL;
#line 911 "src/mir.tr"
    while ((i < n)) {
#line 912 "src/mir.tr"
        LiveSet* u = LiveSet_init();
#line 913 "src/mir.tr"
        LiveSet* d = LiveSet_init();
#line 914 "src/mir.tr"
        block_use_def(((MirBlock*)List_ptr_get(mf->blocks, i)), u->items, d->items);
#line 915 "src/mir.tr"
        List_ptr_append(uses, _tr_obj_retain(u));
#line 916 "src/mir.tr"
        List_ptr_append(defs, _tr_obj_retain(d));
#line 917 "src/mir.tr"
        List_ptr_append(live_in, LiveSet_init());
#line 918 "src/mir.tr"
        List_ptr_append(live_out, LiveSet_init());
#line 919 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(u, _trdrop_LiveSet);
        _tr_obj_release(d, _trdrop_LiveSet);
    }
#line 920 "src/mir.tr"
    bool changed = true;
#line 921 "src/mir.tr"
    while (changed) {
#line 922 "src/mir.tr"
        changed = false;
#line 923 "src/mir.tr"
        long long bi = (n - 1LL);
#line 924 "src/mir.tr"
        while ((bi >= 0LL)) {
#line 925 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
#line 926 "src/mir.tr"
            List_i64* sl = (void*)List_i64_new();
#line 927 "src/mir.tr"
            block_succs(blk, sl);
#line 928 "src/mir.tr"
            long long si = 0LL;
#line 929 "src/mir.tr"
            while ((si < sl->len)) {
#line 930 "src/mir.tr"
                if (({ __auto_type _ord_t748 = (((LiveSet*)List_ptr_get(live_out, bi))); __auto_type _ord_t749 = (((LiveSet*)List_ptr_get(live_in, List_i64_get(sl, si)))); __auto_type _wr = (LiveSet_union_in(_ord_t748, _ord_t749)); _wr; })) {
#line 930 "src/mir.tr"
                    changed = true;
                }
#line 931 "src/mir.tr"
                si = (si + 1LL);
            }
#line 932 "src/mir.tr"
            if (({ __auto_type _ord_t750 = (((LiveSet*)List_ptr_get(live_in, bi))); __auto_type _ord_t751 = (((LiveSet*)List_ptr_get(uses, bi))); __auto_type _wr = (LiveSet_union_in(_ord_t750, _ord_t751)); _wr; })) {
#line 932 "src/mir.tr"
                changed = true;
            }
#line 933 "src/mir.tr"
            if (({ __auto_type _ord_t752 = (((LiveSet*)List_ptr_get(live_in, bi))); __auto_type _ord_t753 = (((LiveSet*)List_ptr_get(live_out, bi))); __auto_type _ord_t754 = (((LiveSet*)List_ptr_get(defs, bi))); __auto_type _wr = (LiveSet_union_except(_ord_t752, _ord_t753, _ord_t754)); _wr; })) {
#line 933 "src/mir.tr"
                changed = true;
            }
#line 934 "src/mir.tr"
            bi = (bi - 1LL);
        }
    }
#line 935 "src/mir.tr"
    List_ptr_free_obj(uses, _trdrop_LiveSet);
    List_ptr_free_obj(defs, _trdrop_LiveSet);
    List_ptr_free_obj(live_in, _trdrop_LiveSet);
    return live_out;
}

__attribute__((hot)) void collect_moves(HirExpr* e, List_TrStr* out) {
#line 956 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 956 "src/mir.tr"
        return;
    }
#line 957 "src/mir.tr"
    __auto_type _t755 = (*e);
    if (_t755.tag == HirExpr_EIdent) {
        __auto_type nm = _t755.data.EIdent.name;
__auto_type ismv = _t755.data.EIdent.is_move;
#line 959 "src/mir.tr"
        if (ismv) {
#line 959 "src/mir.tr"
            set_add(out, nm);
        }
    } else if (_t755.tag == HirExpr_EBinOp) {
        __auto_type l = _t755.data.EBinOp.left;
__auto_type r = _t755.data.EBinOp.right;
#line 961 "src/mir.tr"
        collect_moves(l, out);
#line 962 "src/mir.tr"
        collect_moves(r, out);
    } else if (_t755.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t755.data.EUnaryOp.expr;
        collect_moves(x, out);
    } else if (_t755.tag == HirExpr_ECall) {
        __auto_type callee = _t755.data.ECall.callee;
__auto_type args = _t755.data.ECall.args;
#line 965 "src/mir.tr"
        collect_moves(callee, out);
#line 966 "src/mir.tr"
        long long i = 0LL;
#line 967 "src/mir.tr"
        while ((i < args->len)) {
#line 968 "src/mir.tr"
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
#line 969 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t755.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t755.data.EMethodCall.obj;
__auto_type args = _t755.data.EMethodCall.args;
#line 971 "src/mir.tr"
        collect_moves(obj, out);
#line 972 "src/mir.tr"
        long long i = 0LL;
#line 973 "src/mir.tr"
        while ((i < args->len)) {
#line 974 "src/mir.tr"
            collect_moves(((HirExpr*)List_ptr_get(args, i)), out);
#line 975 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t755.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t755.data.EPropAccess.obj;
        collect_moves(obj, out);
    } else if (_t755.tag == HirExpr_EIndex) {
        __auto_type obj = _t755.data.EIndex.obj;
__auto_type idx = _t755.data.EIndex._tr_v_index;
#line 978 "src/mir.tr"
        collect_moves(obj, out);
#line 979 "src/mir.tr"
        collect_moves(idx, out);
    } else if (_t755.tag == HirExpr_ECast) {
        __auto_type x = _t755.data.ECast.expr;
        collect_moves(x, out);
    } else if (_t755.tag == HirExpr_EAwait) {
        __auto_type x = _t755.data.EAwait.expr;
        collect_moves(x, out);
    } else if (_t755.tag == HirExpr_EIfElse) {
        __auto_type c = _t755.data.EIfElse.cond;
__auto_type t = _t755.data.EIfElse.then_e;
__auto_type f = _t755.data.EIfElse.else_e;
#line 983 "src/mir.tr"
        collect_moves(c, out);
#line 984 "src/mir.tr"
        collect_moves(t, out);
#line 985 "src/mir.tr"
        collect_moves(f, out);
    } else if (1) {
        __auto_type _ = _t755;
        /* pass */
    }
}

__attribute__((hot)) bool is_free_fn(TrStr n) {
#line 990 "src/mir.tr"
    return (((((_tr_str_eqv((n), (_tr_str_lit_len("dealloc", 7LL))) || _tr_str_eqv((n), (_tr_str_lit_len("free", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_c_free", 10LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_free", 8LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_str_release", 15LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("_tr_str_free", 12LL))));
}

__attribute__((hot)) TrStr free_target(HirExpr* arg) {
#line 995 "src/mir.tr"
    if ((((unsigned long long)(arg)) == ((unsigned long long)(0LL)))) {
#line 995 "src/mir.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 996 "src/mir.tr"
    __auto_type _t756 = (*arg);
    if (_t756.tag == HirExpr_EIdent) {
        __auto_type n = _t756.data.EIdent.name;
        TrStr _trret170 = _tr_str_retain(n);
        return _trret170;
    } else if (_t756.tag == HirExpr_ECast) {
        __auto_type x = _t756.data.ECast.expr;
        return free_target(x);
    } else if (1) {
        __auto_type _ = _t756;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) void collect_frees(HirExpr* e, List_TrStr* out) {
#line 1009 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1009 "src/mir.tr"
        return;
    }
#line 1010 "src/mir.tr"
    __auto_type _t757 = (*e);
    if (_t757.tag == HirExpr_ECall) {
        __auto_type callee = _t757.data.ECall.callee;
__auto_type args = _t757.data.ECall.args;
#line 1012 "src/mir.tr"
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
#line 1013 "src/mir.tr"
            __auto_type _t758 = (*callee);
            if (_t758.tag == HirExpr_EIdent) {
                __auto_type cn = _t758.data.EIdent.name;
#line 1015 "src/mir.tr"
                if ((is_free_fn(cn) && (args->len > 0LL))) {
#line 1016 "src/mir.tr"
                    TrStr ft = free_target(((HirExpr*)List_ptr_get(args, 0LL)));
#line 1017 "src/mir.tr"
                    if ((!_tr_str_eqv((ft), (_tr_str_lit_len("", 0LL))))) {
#line 1017 "src/mir.tr"
                        set_add(out, ft);
                    }
                }
            } else if (1) {
                __auto_type _ = _t758;
                /* pass */
            }
        }
#line 1019 "src/mir.tr"
        collect_frees(callee, out);
#line 1020 "src/mir.tr"
        long long i = 0LL;
#line 1021 "src/mir.tr"
        while ((i < args->len)) {
#line 1022 "src/mir.tr"
            collect_frees(((HirExpr*)List_ptr_get(args, i)), out);
#line 1023 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t757.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t757.data.EMethodCall.obj;
__auto_type m = _t757.data.EMethodCall.method;
__auto_type args = _t757.data.EMethodCall.args;
#line 1025 "src/mir.tr"
        if (_tr_str_eqv((m), (_tr_str_lit_len("free", 4LL)))) {
#line 1026 "src/mir.tr"
            __auto_type _t759 = (*obj);
            if (_t759.tag == HirExpr_EIdent) {
                __auto_type on = _t759.data.EIdent.name;
                set_add(out, on);
            } else if (1) {
                __auto_type _ = _t759;
                /* pass */
            }
        }
#line 1029 "src/mir.tr"
        collect_frees(obj, out);
#line 1030 "src/mir.tr"
        long long j = 0LL;
#line 1031 "src/mir.tr"
        while ((j < args->len)) {
#line 1032 "src/mir.tr"
            collect_frees(((HirExpr*)List_ptr_get(args, j)), out);
#line 1033 "src/mir.tr"
            j = (j + 1LL);
        }
    } else if (_t757.tag == HirExpr_EBinOp) {
        __auto_type l = _t757.data.EBinOp.left;
__auto_type r = _t757.data.EBinOp.right;
#line 1035 "src/mir.tr"
        collect_frees(l, out);
#line 1036 "src/mir.tr"
        collect_frees(r, out);
    } else if (_t757.tag == HirExpr_EUnaryOp) {
        __auto_type x = _t757.data.EUnaryOp.expr;
        collect_frees(x, out);
    } else if (_t757.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t757.data.EPropAccess.obj;
        collect_frees(obj, out);
    } else if (_t757.tag == HirExpr_EIndex) {
        __auto_type obj = _t757.data.EIndex.obj;
__auto_type idx = _t757.data.EIndex._tr_v_index;
#line 1040 "src/mir.tr"
        collect_frees(obj, out);
#line 1041 "src/mir.tr"
        collect_frees(idx, out);
    } else if (_t757.tag == HirExpr_ECast) {
        __auto_type x = _t757.data.ECast.expr;
        collect_frees(x, out);
    } else if (_t757.tag == HirExpr_EAwait) {
        __auto_type x = _t757.data.EAwait.expr;
        collect_frees(x, out);
    } else if (1) {
        __auto_type _ = _t757;
        /* pass */
    }
}

__attribute__((hot)) void block_gen_own(MirBlock* blk, List_TrStr* out) {
#line 1048 "src/mir.tr"
    long long i = 0LL;
#line 1049 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 1050 "src/mir.tr"
        __auto_type _t760 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t760.tag == MirStmt_MDeclare) {
            __auto_type p = _t760.data.MDeclare.place;
            set_add(out, p);
        } else if (_t760.tag == MirStmt_MAssign) {
            __auto_type p = _t760.data.MAssign.place;
            set_add(out, p);
        } else if (_t760.tag == MirStmt_MEval) {
            __auto_type _ = _t760.data.MEval.value;
            /* pass */
        }
#line 1054 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void block_moves(MirBlock* blk, List_TrStr* out) {
#line 1058 "src/mir.tr"
    long long i = 0LL;
#line 1059 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 1060 "src/mir.tr"
        __auto_type _t761 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t761.tag == MirStmt_MDeclare) {
            __auto_type val = _t761.data.MDeclare.value;
#line 1062 "src/mir.tr"
            collect_moves(val, out);
#line 1063 "src/mir.tr"
            collect_frees(val, out);
        } else if (_t761.tag == MirStmt_MAssign) {
            __auto_type val = _t761.data.MAssign.value;
#line 1065 "src/mir.tr"
            collect_moves(val, out);
#line 1066 "src/mir.tr"
            collect_frees(val, out);
        } else if (_t761.tag == MirStmt_MEval) {
            __auto_type val = _t761.data.MEval.value;
#line 1068 "src/mir.tr"
            collect_moves(val, out);
#line 1069 "src/mir.tr"
            collect_frees(val, out);
        }
#line 1070 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1071 "src/mir.tr"
    __auto_type _t762 = (*blk->term);
    if (_t762.tag == MirTerm_TBranch) {
        __auto_type cond = _t762.data.TBranch.cond;
        collect_moves(cond, out);
    } else if (_t762.tag == MirTerm_TReturn) {
        __auto_type val = _t762.data.TReturn.value;
#line 1074 "src/mir.tr"
        collect_moves(val, out);
#line 1078 "src/mir.tr"
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
#line 1079 "src/mir.tr"
            __auto_type _t763 = (*val);
            if (_t763.tag == HirExpr_EIdent) {
                __auto_type rnm = _t763.data.EIdent.name;
                set_add(out, rnm);
            } else if (1) {
                __auto_type _ = _t763;
                /* pass */
            }
        }
    } else if (1) {
        __auto_type _ = _t762;
        /* pass */
    }
}

__attribute__((hot)) void preds_of(MirFunction* mf, long long b, List_i64* out) {
#line 1086 "src/mir.tr"
    long long i = 0LL;
#line 1087 "src/mir.tr"
    while ((i < mf->blocks->len)) {
#line 1088 "src/mir.tr"
        List_i64* sl = (void*)List_i64_new();
#line 1089 "src/mir.tr"
        block_succs(((MirBlock*)List_ptr_get(mf->blocks, i)), sl);
#line 1090 "src/mir.tr"
        long long j = 0LL;
#line 1091 "src/mir.tr"
        while ((j < sl->len)) {
#line 1092 "src/mir.tr"
            if ((List_i64_get(sl, j) == b)) {
#line 1092 "src/mir.tr"
                List_i64_append(out, i);
            }
#line 1093 "src/mir.tr"
            j = (j + 1LL);
        }
#line 1094 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_ptr* compute_drops(MirFunction* mf, List_ptr* live_out) {
#line 1098 "src/mir.tr"
    long long n = mf->blocks->len;
#line 1099 "src/mir.tr"
    List_ptr* gen = (void*)List_ptr_new();
#line 1100 "src/mir.tr"
    List_ptr* kill = (void*)List_ptr_new();
#line 1101 "src/mir.tr"
    List_ptr* own_out = (void*)List_ptr_new();
#line 1102 "src/mir.tr"
    LiveSet* universe = LiveSet_init();
#line 1103 "src/mir.tr"
    long long i = 0LL;
#line 1104 "src/mir.tr"
    while ((i < n)) {
#line 1105 "src/mir.tr"
        LiveSet* g = LiveSet_init();
#line 1106 "src/mir.tr"
        LiveSet* k = LiveSet_init();
#line 1107 "src/mir.tr"
        block_gen_own(((MirBlock*)List_ptr_get(mf->blocks, i)), g->items);
#line 1108 "src/mir.tr"
        block_moves(((MirBlock*)List_ptr_get(mf->blocks, i)), k->items);
#line 1109 "src/mir.tr"
        List_ptr_append(gen, _tr_obj_retain(g));
#line 1110 "src/mir.tr"
        List_ptr_append(kill, _tr_obj_retain(k));
#line 1111 "src/mir.tr"
        bool _u = LiveSet_union_in(universe, g);
#line 1112 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(g, _trdrop_LiveSet);
        _tr_obj_release(k, _trdrop_LiveSet);
    }
#line 1114 "src/mir.tr"
    i = 0LL;
#line 1115 "src/mir.tr"
    while ((i < n)) {
#line 1116 "src/mir.tr"
        List_ptr_append(own_out, LiveSet_clone(universe));
#line 1117 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1118 "src/mir.tr"
    bool changed = true;
#line 1119 "src/mir.tr"
    while (changed) {
#line 1120 "src/mir.tr"
        changed = false;
#line 1121 "src/mir.tr"
        long long b = 0LL;
#line 1122 "src/mir.tr"
        while ((b < n)) {
#line 1123 "src/mir.tr"
            List_i64* preds = (void*)List_i64_new();
#line 1124 "src/mir.tr"
            preds_of(mf, b, preds);
#line 1125 "src/mir.tr"
            LiveSet* own_in = LiveSet_init();
#line 1126 "src/mir.tr"
            if ((preds->len > 0LL)) {
#line 1127 "src/mir.tr"
                LiveSet* _cltmp_t764 = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, 0LL))));
                _tr_obj_release(own_in, _trdrop_LiveSet);
                own_in = _cltmp_t764;
#line 1128 "src/mir.tr"
                long long pi = 1LL;
#line 1129 "src/mir.tr"
                while ((pi < preds->len)) {
#line 1130 "src/mir.tr"
                    LiveSet_retain_common(own_in, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds, pi))));
#line 1131 "src/mir.tr"
                    pi = (pi + 1LL);
                }
            }
#line 1132 "src/mir.tr"
            bool _g = LiveSet_union_in(own_in, ((LiveSet*)List_ptr_get(gen, b)));
#line 1136 "src/mir.tr"
            LiveSet* owned_b = LiveSet_minus(own_in, ((LiveSet*)List_ptr_get(kill, b)));
#line 1137 "src/mir.tr"
            LiveSet* new_oo = LiveSet_clone(owned_b);
#line 1138 "src/mir.tr"
            LiveSet_retain_common(new_oo, ((LiveSet*)List_ptr_get(live_out, b)));
#line 1139 "src/mir.tr"
            if ((!LiveSet_equals(new_oo, ((LiveSet*)List_ptr_get(own_out, b))))) {
#line 1140 "src/mir.tr"
                changed = true;
#line 1141 "src/mir.tr"
                LiveSet_set_to(((LiveSet*)List_ptr_get(own_out, b)), new_oo);
            }
#line 1142 "src/mir.tr"
            b = (b + 1LL);
            _tr_obj_release(own_in, _trdrop_LiveSet);
            _tr_obj_release(owned_b, _trdrop_LiveSet);
            _tr_obj_release(new_oo, _trdrop_LiveSet);
        }
    }
#line 1144 "src/mir.tr"
    List_ptr* drops = (void*)List_ptr_new();
#line 1145 "src/mir.tr"
    i = 0LL;
#line 1146 "src/mir.tr"
    while ((i < n)) {
#line 1147 "src/mir.tr"
        List_i64* preds2 = (void*)List_i64_new();
#line 1148 "src/mir.tr"
        preds_of(mf, i, preds2);
#line 1149 "src/mir.tr"
        LiveSet* own_in2 = LiveSet_init();
#line 1150 "src/mir.tr"
        if ((preds2->len > 0LL)) {
#line 1151 "src/mir.tr"
            LiveSet* _cltmp_t765 = LiveSet_clone(((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, 0LL))));
            _tr_obj_release(own_in2, _trdrop_LiveSet);
            own_in2 = _cltmp_t765;
#line 1152 "src/mir.tr"
            long long pj = 1LL;
#line 1153 "src/mir.tr"
            while ((pj < preds2->len)) {
#line 1154 "src/mir.tr"
                LiveSet_retain_common(own_in2, ((LiveSet*)List_ptr_get(own_out, List_i64_get(preds2, pj))));
#line 1155 "src/mir.tr"
                pj = (pj + 1LL);
            }
        }
#line 1156 "src/mir.tr"
        bool _g2 = LiveSet_union_in(own_in2, ((LiveSet*)List_ptr_get(gen, i)));
#line 1157 "src/mir.tr"
        LiveSet* owned_i = LiveSet_minus(own_in2, ((LiveSet*)List_ptr_get(kill, i)));
#line 1158 "src/mir.tr"
        LiveSet* dead_i = LiveSet_minus(owned_i, ((LiveSet*)List_ptr_get(live_out, i)));
#line 1159 "src/mir.tr"
        List_ptr_append(drops, LiveSet_minus(dead_i, mf->unsafe_pinned));
#line 1160 "src/mir.tr"
        i = (i + 1LL);
        _tr_obj_release(own_in2, _trdrop_LiveSet);
        _tr_obj_release(owned_i, _trdrop_LiveSet);
        _tr_obj_release(dead_i, _trdrop_LiveSet);
    }
#line 1161 "src/mir.tr"
    List_ptr_free_obj(gen, _trdrop_LiveSet);
    List_ptr_free_obj(kill, _trdrop_LiveSet);
    List_ptr_free_obj(own_out, _trdrop_LiveSet);
    _tr_obj_release(universe, _trdrop_LiveSet);
    return drops;
}

__attribute__((hot)) long long last_use_in_block(MirBlock* blk, TrStr name) {
#line 1178 "src/mir.tr"
    long long last = (-1LL);
#line 1179 "src/mir.tr"
    long long si = 0LL;
#line 1180 "src/mir.tr"
    while ((si < blk->stmts->len)) {
#line 1181 "src/mir.tr"
        List_TrStr* tmp = (void*)List_TrStr_new();
#line 1182 "src/mir.tr"
        __auto_type _t766 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t766.tag == MirStmt_MDeclare) {
            __auto_type val = _t766.data.MDeclare.value;
            collect_uses(val, tmp);
        } else if (_t766.tag == MirStmt_MAssign) {
            __auto_type val = _t766.data.MAssign.value;
            collect_uses(val, tmp);
        } else if (_t766.tag == MirStmt_MEval) {
            __auto_type val = _t766.data.MEval.value;
            collect_uses(val, tmp);
        }
#line 1186 "src/mir.tr"
        if (set_contains(tmp, name)) {
#line 1186 "src/mir.tr"
            last = si;
        }
#line 1187 "src/mir.tr"
        si = (si + 1LL);
    }
#line 1188 "src/mir.tr"
    List_TrStr* tt = (void*)List_TrStr_new();
#line 1189 "src/mir.tr"
    __auto_type _t767 = (*blk->term);
    if (_t767.tag == MirTerm_TBranch) {
        __auto_type cond = _t767.data.TBranch.cond;
        collect_uses(cond, tt);
    } else if (_t767.tag == MirTerm_TReturn) {
        __auto_type val = _t767.data.TReturn.value;
        collect_uses(val, tt);
    } else if (1) {
        __auto_type _ = _t767;
        /* pass */
    }
#line 1193 "src/mir.tr"
    if (set_contains(tt, name)) {
#line 1193 "src/mir.tr"
        last = blk->stmts->len;
    }
#line 1194 "src/mir.tr"
    return last;
}

__attribute__((hot)) List_bool* compute_borrow_outlives(MirFunction* mf, List_ptr* live_out) {
#line 1197 "src/mir.tr"
    List_bool* out = (void*)List_bool_new();
#line 1198 "src/mir.tr"
    long long i = 0LL;
#line 1199 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1200 "src/mir.tr"
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1201 "src/mir.tr"
        bool proven = true;
#line 1203 "src/mir.tr"
        long long bi = 0LL;
#line 1204 "src/mir.tr"
        while ((bi < mf->blocks->len)) {
#line 1205 "src/mir.tr"
            LiveSet* lo = ((LiveSet*)List_ptr_get(live_out, bi));
#line 1206 "src/mir.tr"
            if ((({ TrStr _apt_t768 = _tr_str_retain(edge->borrower); __auto_type _wr = (LiveSet_has(lo, _apt_t768)); _tr_str_release(_apt_t768); _wr; }) && ({ TrStr _apt_t769 = _tr_str_retain(edge->source); __auto_type _wr = ((!LiveSet_has(lo, _apt_t769))); _tr_str_release(_apt_t769); _wr; }))) {
#line 1207 "src/mir.tr"
                proven = false;
            }
#line 1208 "src/mir.tr"
            bi = (bi + 1LL);
        }
#line 1214 "src/mir.tr"
        long long db = edge->decl_block;
#line 1215 "src/mir.tr"
        if (((db >= 0LL) && (db < mf->blocks->len))) {
#line 1216 "src/mir.tr"
            if (({ __auto_type _ord_t770 = (((LiveSet*)List_ptr_get(live_out, db))); __auto_type _ord_t771 = (edge->source); __auto_type _wr = ((!LiveSet_has(_ord_t770, _ord_t771))); _wr; })) {
#line 1217 "src/mir.tr"
                MirBlock* dblk = ((MirBlock*)List_ptr_get(mf->blocks, db));
#line 1218 "src/mir.tr"
                long long src_last = ({ TrStr _apt_t772 = _tr_str_retain(edge->source); __auto_type _wr = (last_use_in_block(dblk, _apt_t772)); _tr_str_release(_apt_t772); _wr; });
#line 1219 "src/mir.tr"
                long long bor_last = ({ TrStr _apt_t773 = _tr_str_retain(edge->borrower); __auto_type _wr = (last_use_in_block(dblk, _apt_t773)); _tr_str_release(_apt_t773); _wr; });
#line 1220 "src/mir.tr"
                if (({ __auto_type _ord_t774 = (((LiveSet*)List_ptr_get(live_out, db))); __auto_type _ord_t775 = (edge->borrower); __auto_type _wr = (LiveSet_has(_ord_t774, _ord_t775)); _wr; })) {
#line 1221 "src/mir.tr"
                    bor_last = (dblk->stmts->len + 1LL);
                }
#line 1222 "src/mir.tr"
                if ((bor_last > src_last)) {
#line 1223 "src/mir.tr"
                    proven = false;
                }
            }
        }
#line 1224 "src/mir.tr"
        List_bool_append(out, proven);
#line 1225 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1226 "src/mir.tr"
    return out;
}

__attribute__((hot)) bool is_if_body(MirFunction* mf, HirBlock* hb) {
#line 1236 "src/mir.tr"
    long long i = 0LL;
#line 1237 "src/mir.tr"
    while ((i < mf->if_bodies->len)) {
#line 1238 "src/mir.tr"
        if ((((unsigned long long)(((HirBlock*)List_ptr_get(mf->if_bodies, i)))) == ((unsigned long long)(hb)))) {
#line 1238 "src/mir.tr"
            return true;
        }
#line 1239 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1240 "src/mir.tr"
    return false;
}

__attribute__((hot)) DropSite* site_for(List_ptr* out, HirBlock* hb) {
#line 1244 "src/mir.tr"
    long long i = 0LL;
#line 1245 "src/mir.tr"
    while ((i < out->len)) {
#line 1246 "src/mir.tr"
        if ((((unsigned long long)(((DropSite*)List_ptr_get(out, i))->hir_block)) == ((unsigned long long)(hb)))) {
#line 1246 "src/mir.tr"
            return _tr_obj_retain(((DropSite*)List_ptr_get(out, i)));
        }
#line 1247 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1248 "src/mir.tr"
    DropSite* s = ((DropSite*)_tr_obj_alloc(sizeof(DropSite)));
#line 1249 "src/mir.tr"
    HirBlock* _cltmp_t776 = _tr_obj_retain(hb);
    _tr_obj_release(s->hir_block, _trdrop_HirBlock);
    s->hir_block = _cltmp_t776;
#line 1250 "src/mir.tr"
    s->places = (void*)List_TrStr_new();
#line 1251 "src/mir.tr"
    List_ptr_append(out, _tr_obj_retain(s));
#line 1252 "src/mir.tr"
    return s;
}

__attribute__((hot)) List_TrStr* mir_proven_borrows(HirFunction* hf) {
#line 1260 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1261 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1262 "src/mir.tr"
    if ((!mf->complete)) {
#line 1262 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1263 "src/mir.tr"
    if ((mf->borrows->len == 0LL)) {
#line 1263 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1264 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1265 "src/mir.tr"
    List_bool* outl = compute_borrow_outlives(mf, live);
#line 1266 "src/mir.tr"
    long long i = 0LL;
#line 1267 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1268 "src/mir.tr"
        BorrowEdge* edge = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1272 "src/mir.tr"
        bool proven = List_bool_get(outl, i);
#line 1278 "src/mir.tr"
        if ((proven && edge->via_collection)) {
#line 1279 "src/mir.tr"
            long long bn = 0LL;
#line 1280 "src/mir.tr"
            while ((bn < mf->blocks->len)) {
#line 1281 "src/mir.tr"
                MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1282 "src/mir.tr"
                LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
#line 1283 "src/mir.tr"
                long long si = 0LL;
#line 1284 "src/mir.tr"
                while ((si < blk->stmts->len)) {
#line 1285 "src/mir.tr"
                    bool is_creation = false;
#line 1286 "src/mir.tr"
                    __auto_type _t777 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                    if (_t777.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t777.data.MDeclare.place;
#line 1288 "src/mir.tr"
                        if (_tr_str_eqv((dn), (edge->borrower))) {
#line 1288 "src/mir.tr"
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t777;
                        /* pass */
                    }
#line 1290 "src/mir.tr"
                    if ((!is_creation)) {
#line 1291 "src/mir.tr"
                        List_TrStr* uses = (void*)List_TrStr_new();
#line 1292 "src/mir.tr"
                        __auto_type _t778 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
                        if (_t778.tag == MirStmt_MDeclare) {
                            __auto_type val = _t778.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t778.tag == MirStmt_MAssign) {
                            __auto_type ap = _t778.data.MAssign.place;
__auto_type val = _t778.data.MAssign.value;
#line 1295 "src/mir.tr"
                            collect_uses(val, uses);
#line 1296 "src/mir.tr"
                            if (_tr_str_eqv((ap), (edge->source))) {
#line 1296 "src/mir.tr"
                                ({ TrStr _apt_t779 = _tr_str_retain(edge->source); set_add(uses, _apt_t779); _tr_str_release(_apt_t779); });
                            }
                        } else if (_t778.tag == MirStmt_MEval) {
                            __auto_type val = _t778.data.MEval.value;
                            collect_uses(val, uses);
                        }
#line 1298 "src/mir.tr"
                        if (({ __auto_type _ord_t781 = (mf->pwrites); TrStr _apt_t780 = _tr_str_retain(edge->source); TrStr _apt_t782 = _tr_str_retain(edge->path); __auto_type _wr = (_pwrite_hits(_ord_t781, bn, si, _apt_t780, _apt_t782)); _tr_str_release(_apt_t780); _tr_str_release(_apt_t782); _wr; })) {
#line 1298 "src/mir.tr"
                            ({ TrStr _apt_t783 = _tr_str_retain(edge->source); set_add(uses, _apt_t783); _tr_str_release(_apt_t783); });
                        }
#line 1299 "src/mir.tr"
                        if ((({ TrStr _apt_t784 = _tr_str_retain(edge->source); __auto_type _wr = (set_contains(uses, _apt_t784)); _tr_str_release(_apt_t784); _wr; }) && ({ TrStr _apt_t785 = _tr_str_retain(edge->borrower); __auto_type _wr = (borrower_live_after(blk, si, _apt_t785, lo)); _tr_str_release(_apt_t785); _wr; }))) {
#line 1300 "src/mir.tr"
                            proven = false;
                        }
                    }
#line 1301 "src/mir.tr"
                    si = (si + 1LL);
                }
#line 1302 "src/mir.tr"
                bn = (bn + 1LL);
            }
        }
#line 1303 "src/mir.tr"
        if (proven) {
#line 1304 "src/mir.tr"
            ({ TrStr _apt_t786 = _tr_str_retain(edge->borrower); List_TrStr_append(out, _apt_t786); _tr_str_release(_apt_t786); });
        }
#line 1305 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1306 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_bool_free(outl);
    return out;
}

__attribute__((hot)) bool _pwrite_hits(List_ptr* pws, long long blk, long long idx, TrStr root, TrStr bpath) {
#line 1309 "src/mir.tr"
    long long i = 0LL;
#line 1310 "src/mir.tr"
    while ((i < pws->len)) {
#line 1311 "src/mir.tr"
        PWrite* w = ((PWrite*)List_ptr_get(pws, i));
#line 1312 "src/mir.tr"
        if (((((w->block == blk) && (w->idx == idx)) && _tr_str_eqv((w->root), (root))) && ({ TrStr _apt_t787 = _tr_str_retain(w->path); __auto_type _wr = (_pl_overlap(_apt_t787, bpath)); _tr_str_release(_apt_t787); _wr; }))) {
#line 1312 "src/mir.tr"
            return true;
        }
#line 1313 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1314 "src/mir.tr"
    return false;
}

__attribute__((hot)) List_bool* _reach_from(MirFunction* mf, long long start) {
#line 1319 "src/mir.tr"
    List_bool* seen = (void*)List_bool_new();
#line 1320 "src/mir.tr"
    long long k = 0LL;
#line 1321 "src/mir.tr"
    while ((k < mf->blocks->len)) {
#line 1322 "src/mir.tr"
        List_bool_append(seen, false);
#line 1323 "src/mir.tr"
        k = (k + 1LL);
    }
#line 1324 "src/mir.tr"
    List_i64* work = (void*)List_i64_new();
#line 1325 "src/mir.tr"
    List_i64_append(work, start);
#line 1326 "src/mir.tr"
    bool first = true;
#line 1327 "src/mir.tr"
    while ((work->len > 0LL)) {
#line 1328 "src/mir.tr"
        long long cur = List_i64_get(work, (work->len - 1LL));
#line 1329 "src/mir.tr"
        List_i64_pop(work);
#line 1330 "src/mir.tr"
        List_i64* succ = (void*)List_i64_new();
#line 1331 "src/mir.tr"
        __auto_type _t788 = (*((MirBlock*)List_ptr_get(mf->blocks, cur))->term);
        if (_t788.tag == MirTerm_TGoto) {
            __auto_type t = _t788.data.TGoto.target;
            List_i64_append(succ, t);
        } else if (_t788.tag == MirTerm_TBranch) {
            __auto_type t2 = _t788.data.TBranch.t;
__auto_type e2 = _t788.data.TBranch.e;
#line 1334 "src/mir.tr"
            List_i64_append(succ, t2);
#line 1335 "src/mir.tr"
            List_i64_append(succ, e2);
        } else if (1) {
            __auto_type _ = _t788;
            /* pass */
        }
#line 1337 "src/mir.tr"
        long long si = 0LL;
#line 1338 "src/mir.tr"
        while ((si < succ->len)) {
#line 1339 "src/mir.tr"
            long long nx = List_i64_get(succ, si);
#line 1340 "src/mir.tr"
            if ((((nx >= 0LL) && (nx < seen->len)) && (!List_bool_get(seen, nx)))) {
#line 1341 "src/mir.tr"
                List_bool_set(seen, nx, true);
#line 1342 "src/mir.tr"
                List_i64_append(work, nx);
            }
#line 1343 "src/mir.tr"
            si = (si + 1LL);
        }
        List_i64_free(succ);
    }
#line 1344 "src/mir.tr"
    List_i64_free(work);
    return seen;
}

__attribute__((hot)) long long _decl_index(MirBlock* blk, TrStr name) {
#line 1348 "src/mir.tr"
    long long i = 0LL;
#line 1349 "src/mir.tr"
    while ((i < blk->stmts->len)) {
#line 1350 "src/mir.tr"
        __auto_type _t789 = (*((MirStmt*)List_ptr_get(blk->stmts, i)));
        if (_t789.tag == MirStmt_MDeclare) {
            __auto_type dn = _t789.data.MDeclare.place;
#line 1352 "src/mir.tr"
            if (_tr_str_eqv((dn), (name))) {
#line 1352 "src/mir.tr"
                long long _trret171 = i;
                return _trret171;
            }
        } else if (1) {
            __auto_type _ = _t789;
            /* pass */
        }
#line 1354 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1355 "src/mir.tr"
    return (-1LL);
}

__attribute__((hot)) bool borrower_live_after(MirBlock* blk, long long after_idx, TrStr name, LiveSet* lo) {
#line 1362 "src/mir.tr"
    long long si = (after_idx + 1LL);
#line 1363 "src/mir.tr"
    while ((si < blk->stmts->len)) {
#line 1364 "src/mir.tr"
        List_TrStr* uses = (void*)List_TrStr_new();
#line 1365 "src/mir.tr"
        __auto_type _t790 = (*((MirStmt*)List_ptr_get(blk->stmts, si)));
        if (_t790.tag == MirStmt_MDeclare) {
            __auto_type val = _t790.data.MDeclare.value;
            collect_uses(val, uses);
        } else if (_t790.tag == MirStmt_MAssign) {
            __auto_type val = _t790.data.MAssign.value;
            collect_uses(val, uses);
        } else if (_t790.tag == MirStmt_MEval) {
            __auto_type val = _t790.data.MEval.value;
            collect_uses(val, uses);
        }
#line 1369 "src/mir.tr"
        if (set_contains(uses, name)) {
#line 1369 "src/mir.tr"
            return true;
        }
#line 1370 "src/mir.tr"
        si = (si + 1LL);
    }
#line 1371 "src/mir.tr"
    List_TrStr* tu = (void*)List_TrStr_new();
#line 1372 "src/mir.tr"
    __auto_type _t791 = (*blk->term);
    if (_t791.tag == MirTerm_TBranch) {
        __auto_type cond = _t791.data.TBranch.cond;
        collect_uses(cond, tu);
    } else if (_t791.tag == MirTerm_TReturn) {
        __auto_type val = _t791.data.TReturn.value;
        collect_uses(val, tu);
    } else if (1) {
        __auto_type _ = _t791;
        /* pass */
    }
#line 1376 "src/mir.tr"
    if (set_contains(tu, name)) {
#line 1376 "src/mir.tr"
        return true;
    }
#line 1377 "src/mir.tr"
    return LiveSet_has(lo, name);
}

__attribute__((hot)) List_TrStr* mir_borrow_conflicts(HirFunction* hf, TrMap* mutating_methods) {
#line 1384 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1385 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1386 "src/mir.tr"
    if ((!mf->complete)) {
#line 1386 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1387 "src/mir.tr"
    if ((mf->borrows->len == 0LL)) {
#line 1387 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1388 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1395 "src/mir.tr"
    long long k = 0LL;
#line 1396 "src/mir.tr"
    while ((k < mf->borrows->len)) {
#line 1397 "src/mir.tr"
        BorrowEdge* be = ((BorrowEdge*)List_ptr_get(mf->borrows, k));
#line 1399 "src/mir.tr"
        List_bool* be_reach = (void*)List_bool_new();
#line 1400 "src/mir.tr"
        long long be_didx = (-1LL);
#line 1401 "src/mir.tr"
        if (((be->decl_block >= 0LL) && (be->decl_block < mf->blocks->len))) {
#line 1402 "src/mir.tr"
            be_reach = _reach_from(mf, be->decl_block);
#line 1403 "src/mir.tr"
            be_didx = be->decl_idx;
#line 1404 "src/mir.tr"
            if ((be_didx < 0LL)) {
#line 1404 "src/mir.tr"
                be_didx = ({ __auto_type _ord_t793 = (((MirBlock*)List_ptr_get(mf->blocks, be->decl_block))); TrStr _apt_t792 = _tr_str_retain(be->borrower); __auto_type _wr = (_decl_index(_ord_t793, _apt_t792)); _tr_str_release(_apt_t792); _wr; });
            }
        }
#line 1405 "src/mir.tr"
        long long bn = 0LL;
#line 1406 "src/mir.tr"
        while ((bn < mf->blocks->len)) {
#line 1407 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1408 "src/mir.tr"
            LiveSet* lo = ((LiveSet*)List_ptr_get(live, bn));
#line 1409 "src/mir.tr"
            long long si = 0LL;
#line 1410 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1411 "src/mir.tr"
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
#line 1412 "src/mir.tr"
                bool after_creation = true;
#line 1413 "src/mir.tr"
                if (((be_reach->len > 0LL) && (be_didx >= 0LL))) {
#line 1414 "src/mir.tr"
                    after_creation = List_bool_get(be_reach, bn);
#line 1415 "src/mir.tr"
                    if (((bn == be->decl_block) && (si > be_didx))) {
#line 1415 "src/mir.tr"
                        after_creation = true;
                    }
                }
#line 1416 "src/mir.tr"
                if ((!after_creation)) {
#line 1417 "src/mir.tr"
                    si = (si + 1LL);
#line 1418 "src/mir.tr"
                    continue;
                }
#line 1419 "src/mir.tr"
                TrStr invalid_desc = _tr_str_lit_len("", 0LL);
#line 1420 "src/mir.tr"
                HirExpr* sval = ((HirExpr*)(0LL));
#line 1421 "src/mir.tr"
                __auto_type _t794 = (*stmt);
                if (_t794.tag == MirStmt_MAssign) {
                    __auto_type wp = _t794.data.MAssign.place;
__auto_type wv = _t794.data.MAssign.value;
#line 1423 "src/mir.tr"
                    sval = wv;
#line 1424 "src/mir.tr"
                    if (_tr_str_eqv((wp), (be->source))) {
#line 1424 "src/mir.tr"
                        TrStr _strtmp_t795 = _tr_str_lit_len("assign to", 9LL);
                        _tr_str_release(invalid_desc);
                        invalid_desc = _strtmp_t795;
                    }
                } else if (_t794.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t794.data.MDeclare.value;
                    sval = dv;
                } else if (_t794.tag == MirStmt_MEval) {
                    __auto_type ev = _t794.data.MEval.value;
                    sval = ev;
                }
#line 1427 "src/mir.tr"
                if ((_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))) && ({ TrStr _apt_t796 = _tr_str_retain(be->source); TrStr _apt_t797 = _tr_str_retain(be->path); __auto_type _wr = (_is_mutating_call_on(sval, _apt_t796, mutating_methods, _apt_t797)); _tr_str_release(_apt_t796); _tr_str_release(_apt_t797); _wr; }))) {
#line 1428 "src/mir.tr"
                    TrStr _strtmp_t798 = _tr_str_lit_len("call a mutating method on", 25LL);
                    _tr_str_release(invalid_desc);
                    invalid_desc = _strtmp_t798;
                }
#line 1429 "src/mir.tr"
                if ((_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))) && ({ __auto_type _ord_t800 = (mf->pwrites); TrStr _apt_t799 = _tr_str_retain(be->source); TrStr _apt_t801 = _tr_str_retain(be->path); __auto_type _wr = (_pwrite_hits(_ord_t800, bn, si, _apt_t799, _apt_t801)); _tr_str_release(_apt_t799); _tr_str_release(_apt_t801); _wr; }))) {
#line 1430 "src/mir.tr"
                    TrStr _strtmp_t802 = _tr_str_lit_len("assign to a part of", 19LL);
                    _tr_str_release(invalid_desc);
                    invalid_desc = _strtmp_t802;
                }
#line 1431 "src/mir.tr"
                if ((!_tr_str_eqv((invalid_desc), (_tr_str_lit_len("", 0LL))))) {
#line 1432 "src/mir.tr"
                    if (({ TrStr _apt_t803 = _tr_str_retain(be->borrower); __auto_type _wr = (borrower_live_after(blk, si, _apt_t803, lo)); _tr_str_release(_apt_t803); _wr; })) {
#line 1433 "src/mir.tr"
                        ({ TrStr _at_t804 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot ", 7LL)), (invalid_desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" '", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->source)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is borrowed by '", 27LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — the borrow is still live", 30LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t804); _tr_str_release(_at_t804); });
                    }
                } else if (be->is_exclusive) {
#line 1437 "src/mir.tr"
                    bool is_creation = false;
#line 1438 "src/mir.tr"
                    __auto_type _t805 = (*stmt);
                    if (_t805.tag == MirStmt_MDeclare) {
                        __auto_type dn = _t805.data.MDeclare.place;
#line 1440 "src/mir.tr"
                        if (_tr_str_eqv((dn), (be->borrower))) {
#line 1440 "src/mir.tr"
                            is_creation = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t805;
                        /* pass */
                    }
#line 1442 "src/mir.tr"
                    if ((!is_creation)) {
#line 1443 "src/mir.tr"
                        List_TrStr* uses = (void*)List_TrStr_new();
#line 1444 "src/mir.tr"
                        __auto_type _t806 = (*stmt);
                        if (_t806.tag == MirStmt_MDeclare) {
                            __auto_type val = _t806.data.MDeclare.value;
                            collect_uses(val, uses);
                        } else if (_t806.tag == MirStmt_MAssign) {
                            __auto_type val = _t806.data.MAssign.value;
                            collect_uses(val, uses);
                        } else if (_t806.tag == MirStmt_MEval) {
                            __auto_type val = _t806.data.MEval.value;
                            collect_uses(val, uses);
                        }
#line 1448 "src/mir.tr"
                        if ((({ TrStr _apt_t807 = _tr_str_retain(be->source); __auto_type _wr = (set_contains(uses, _apt_t807)); _tr_str_release(_apt_t807); _wr; }) && ({ TrStr _apt_t808 = _tr_str_retain(be->borrower); __auto_type _wr = (borrower_live_after(blk, si, _apt_t808, lo)); _tr_str_release(_apt_t808); _wr; }))) {
#line 1449 "src/mir.tr"
                            ({ TrStr _at_t809 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot use '", 12LL)), (be->source))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' while it is exclusively (mut) borrowed by '", 45LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (be->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("'", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t809); _tr_str_release(_at_t809); });
                        }
                    }
                }
#line 1450 "src/mir.tr"
                si = (si + 1LL);
                _tr_str_release(invalid_desc);
            }
#line 1451 "src/mir.tr"
            bn = (bn + 1LL);
        }
#line 1452 "src/mir.tr"
        k = (k + 1LL);
        List_bool_free(be_reach);
    }
#line 1453 "src/mir.tr"
    if ((mf->borrows->len < 2LL)) {
#line 1453 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        List_ptr_free_obj(live, _trdrop_LiveSet);
        return out;
    }
#line 1454 "src/mir.tr"
    long long i = 0LL;
#line 1455 "src/mir.tr"
    while ((i < mf->borrows->len)) {
#line 1456 "src/mir.tr"
        BorrowEdge* bi = ((BorrowEdge*)List_ptr_get(mf->borrows, i));
#line 1457 "src/mir.tr"
        if (((bi->decl_block >= 0LL) && (bi->decl_block < mf->blocks->len))) {
#line 1458 "src/mir.tr"
            long long j = (i + 1LL);
#line 1459 "src/mir.tr"
            while ((j < mf->borrows->len)) {
#line 1460 "src/mir.tr"
                BorrowEdge* bj = ((BorrowEdge*)List_ptr_get(mf->borrows, j));
#line 1461 "src/mir.tr"
                if ((((bj->decl_block == bi->decl_block) && _tr_str_eqv((bi->source), (bj->source))) && (bi->is_exclusive || bj->is_exclusive))) {
#line 1462 "src/mir.tr"
                    MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi->decl_block));
#line 1463 "src/mir.tr"
                    long long pi = 0LL;
#line 1464 "src/mir.tr"
                    long long pj = 0LL;
#line 1465 "src/mir.tr"
                    long long bk = 0LL;
#line 1466 "src/mir.tr"
                    while ((bk < blk->stmts->len)) {
#line 1467 "src/mir.tr"
                        __auto_type _t810 = (*((MirStmt*)List_ptr_get(blk->stmts, bk)));
                        if (_t810.tag == MirStmt_MDeclare) {
                            __auto_type dn = _t810.data.MDeclare.place;
#line 1469 "src/mir.tr"
                            if (_tr_str_eqv((dn), (bi->borrower))) {
#line 1469 "src/mir.tr"
                                pi = bk;
                            }
#line 1470 "src/mir.tr"
                            if (_tr_str_eqv((dn), (bj->borrower))) {
#line 1470 "src/mir.tr"
                                pj = bk;
                            }
                        } else if (1) {
                            __auto_type _ = _t810;
                            /* pass */
                        }
#line 1472 "src/mir.tr"
                        bk = (bk + 1LL);
                    }
#line 1473 "src/mir.tr"
                    long long ui = ({ TrStr _apt_t811 = _tr_str_retain(bi->borrower); __auto_type _wr = (last_use_in_block(blk, _apt_t811)); _tr_str_release(_apt_t811); _wr; });
#line 1474 "src/mir.tr"
                    long long uj = ({ TrStr _apt_t812 = _tr_str_retain(bj->borrower); __auto_type _wr = (last_use_in_block(blk, _apt_t812)); _tr_str_release(_apt_t812); _wr; });
#line 1475 "src/mir.tr"
                    if ((ui < pi)) {
#line 1475 "src/mir.tr"
                        ui = pi;
                    }
#line 1476 "src/mir.tr"
                    if ((uj < pj)) {
#line 1476 "src/mir.tr"
                        uj = pj;
                    }
#line 1477 "src/mir.tr"
                    if (((pi <= uj) && (pj <= ui))) {
#line 1478 "src/mir.tr"
                        TrStr kind = _tr_str_lit_len("a mutable borrow", 16LL);
#line 1479 "src/mir.tr"
                        if ((!(bi->is_exclusive && bj->is_exclusive))) {
#line 1479 "src/mir.tr"
                            TrStr _strtmp_t813 = _tr_str_lit_len("a mutable and a shared borrow", 29LL);
                            _tr_str_release(kind);
                            kind = _strtmp_t813;
                        }
#line 1480 "src/mir.tr"
                        ({ TrStr _at_t814 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("'", 1LL)), (bi->borrower))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' and '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bj->borrower)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' are ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (kind)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" of '", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (bi->source)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' that are live at the same time", 32LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t814); _tr_str_release(_at_t814); });
                    }
                }
#line 1481 "src/mir.tr"
                j = (j + 1LL);
            }
        }
#line 1482 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1483 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_ptr_free_obj(live, _trdrop_LiveSet);
    return out;
}

__attribute__((hot)) List_TrStr* mir_shared_ref_param_violations(HirFunction* hf, TrMap* mutating_methods) {
#line 1490 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1492 "src/mir.tr"
    List_TrStr* shps = (void*)List_TrStr_new();
#line 1493 "src/mir.tr"
    long long pi = 0LL;
#line 1494 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 1495 "src/mir.tr"
        AstType* pty = ((HirParam*)List_ptr_get(hf->params, pi))->ty;
#line 1496 "src/mir.tr"
        if ((pty->is_borrow && (!pty->is_mut_borrow))) {
#line 1497 "src/mir.tr"
            ({ TrStr _apt_t815 = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name); List_TrStr_append(shps, _apt_t815); _tr_str_release(_apt_t815); });
        }
#line 1498 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 1499 "src/mir.tr"
    if ((shps->len == 0LL)) {
#line 1499 "src/mir.tr"
        List_TrStr_free(shps);
        return out;
    }
#line 1500 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1501 "src/mir.tr"
    long long sk = 0LL;
#line 1502 "src/mir.tr"
    while ((sk < shps->len)) {
#line 1503 "src/mir.tr"
        TrStr pn = List_TrStr_get(shps, sk);
#line 1504 "src/mir.tr"
        long long bn = 0LL;
#line 1505 "src/mir.tr"
        while ((bn < mf->blocks->len)) {
#line 1506 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bn));
#line 1507 "src/mir.tr"
            long long si = 0LL;
#line 1508 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1509 "src/mir.tr"
                MirStmt* stmt = ((MirStmt*)List_ptr_get(blk->stmts, si));
#line 1510 "src/mir.tr"
                HirExpr* sval = ((HirExpr*)(0LL));
#line 1511 "src/mir.tr"
                TrStr desc = _tr_str_lit_len("", 0LL);
#line 1512 "src/mir.tr"
                __auto_type _t816 = (*stmt);
                if (_t816.tag == MirStmt_MAssign) {
                    __auto_type wp = _t816.data.MAssign.place;
__auto_type wv = _t816.data.MAssign.value;
#line 1514 "src/mir.tr"
                    sval = wv;
#line 1515 "src/mir.tr"
                    if (_tr_str_eqv((wp), (pn))) {
#line 1515 "src/mir.tr"
                        TrStr _strtmp_t817 = _tr_str_lit_len("reassign", 8LL);
                        _tr_str_release(desc);
                        desc = _strtmp_t817;
                    }
                } else if (_t816.tag == MirStmt_MDeclare) {
                    __auto_type dv = _t816.data.MDeclare.value;
                    sval = dv;
                } else if (_t816.tag == MirStmt_MEval) {
                    __auto_type ev = _t816.data.MEval.value;
                    sval = ev;
                }
#line 1518 "src/mir.tr"
                if ((_tr_str_eqv((desc), (_tr_str_lit_len("", 0LL))) && _is_mutating_call_on(sval, pn, mutating_methods, _tr_str_lit_len("", 0LL)))) {
#line 1519 "src/mir.tr"
                    TrStr _strtmp_t818 = _tr_str_lit_len("call a mutating method on", 25LL);
                    _tr_str_release(desc);
                    desc = _strtmp_t818;
                }
#line 1520 "src/mir.tr"
                if ((!_tr_str_eqv((desc), (_tr_str_lit_len("", 0LL))))) {
#line 1521 "src/mir.tr"
                    ({ TrStr _at_t819 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot ", 7LL)), (desc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" '", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t819); _tr_str_release(_at_t819); });
                }
#line 1522 "src/mir.tr"
                si = (si + 1LL);
                _tr_str_release(desc);
            }
#line 1523 "src/mir.tr"
            bn = (bn + 1LL);
        }
#line 1524 "src/mir.tr"
        sk = (sk + 1LL);
        _tr_str_release(pn);
    }
#line 1525 "src/mir.tr"
    List_TrStr_free(shps);
    _tr_obj_release(mf, _trdrop_MirFunction);
    return out;
}

__attribute__((hot)) TrStr _b3_root_ident(HirExpr* e) {
#line 1532 "src/mir.tr"
    HirExpr* cur = e;
#line 1533 "src/mir.tr"
    long long guard = 0LL;
#line 1534 "src/mir.tr"
    while (((((unsigned long long)(cur)) != ((unsigned long long)(0LL))) && (guard < 64LL))) {
#line 1535 "src/mir.tr"
        __auto_type _t820 = (*cur);
        if (_t820.tag == HirExpr_EIdent) {
            __auto_type n = _t820.data.EIdent.name;
            TrStr _trret172 = _tr_str_retain(n);
            return _trret172;
        } else if (_t820.tag == HirExpr_EPropAccess) {
            __auto_type o = _t820.data.EPropAccess.obj;
            cur = o;
        } else if (_t820.tag == HirExpr_EIndex) {
            __auto_type o = _t820.data.EIndex.obj;
            cur = o;
        } else if (1) {
            __auto_type _ = _t820;
            return _tr_str_lit_len("", 0LL);
        }
#line 1540 "src/mir.tr"
        guard = (guard + 1LL);
    }
#line 1541 "src/mir.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) bool _b3_is_plain_ident(HirExpr* e) {
#line 1544 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1544 "src/mir.tr"
        return false;
    }
#line 1545 "src/mir.tr"
    __auto_type _t821 = (*e);
    if (_t821.tag == HirExpr_EIdent) {
        return true;
    } else if (1) {
        __auto_type _ = _t821;
        return false;
    }
}

__attribute__((hot)) bool _b3_nested_mut_call(HirExpr* e, TrStr pn, TrMap* mm) {
#line 1550 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1550 "src/mir.tr"
        return false;
    }
#line 1551 "src/mir.tr"
    __auto_type _t822 = (*e);
    if (_t822.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t822.data.EMethodCall.obj;
__auto_type method = _t822.data.EMethodCall.method;
#line 1553 "src/mir.tr"
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
#line 1553 "src/mir.tr"
            return false;
        }
#line 1554 "src/mir.tr"
        if (_b3_is_plain_ident(obj)) {
#line 1554 "src/mir.tr"
            return false;
        }
#line 1555 "src/mir.tr"
        if (({ TrStr _cmpa_t823 = (_b3_root_ident(obj)); TrStr _cmpb_t824 = (pn); _Bool _cmpr_t825 = !_tr_str_eqv(_cmpa_t823, _cmpb_t824); _tr_str_release(_cmpa_t823); _cmpr_t825; })) {
#line 1555 "src/mir.tr"
            return false;
        }
#line 1556 "src/mir.tr"
        TrStr tn = _tr_str_retain(hir_expr_type(obj)->name);
#line 1557 "src/mir.tr"
        if (((((_tr_str_eqv((tn), (_tr_str_lit_len("List", 4LL))) || _tr_str_eqv((tn), (_tr_str_lit_len("Vec", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Dict", 4LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Map", 3LL)))) || _tr_str_eqv((tn), (_tr_str_lit_len("Set", 3LL))))) {
#line 1558 "src/mir.tr"
            bool _trret173 = (((((((((_tr_str_eqv((method), (_tr_str_lit_len("push", 4LL))) || _tr_str_eqv((method), (_tr_str_lit_len("append", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("insert", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("remove", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("pop", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("clear", 5LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("extend", 6LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("add", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("put", 3LL)))) || _tr_str_eqv((method), (_tr_str_lit_len("set", 3LL))));
            _tr_str_release(tn);
            return _trret173;
        }
#line 1559 "src/mir.tr"
        bool _trret174 = ({ TrStr _dkt_t826 = (({ TrStr _tr_cl = (_tr_strx_concatv((tn), (_tr_str_lit_len(".", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (_tr_dict_contains(mm, _tr_strz(_dkt_t826))); _tr_str_release(_dkt_t826); _wr; });
        _tr_str_release(tn);
        return _trret174;
    } else if (1) {
        __auto_type _ = _t822;
        return false;
    }
}

__attribute__((hot)) void _b3_block(HirBlock* b, TrStr pn, TrMap* mm, List_TrStr* out) {
#line 1563 "src/mir.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 1563 "src/mir.tr"
        return;
    }
#line 1564 "src/mir.tr"
    long long i = 0LL;
#line 1565 "src/mir.tr"
    while ((i < b->stmts->len)) {
#line 1566 "src/mir.tr"
        _b3_stmt(((HirStmt*)List_ptr_get(b->stmts, i)), pn, mm, out);
#line 1567 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void _b3_stmt(HirStmt* s, TrStr pn, TrMap* mm, List_TrStr* out) {
#line 1570 "src/mir.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 1570 "src/mir.tr"
        return;
    }
#line 1571 "src/mir.tr"
    __auto_type _t827 = (*s);
    if (_t827.tag == HirStmt_SAssign) {
        __auto_type tgt = _t827.data.SAssign.target;
__auto_type val = _t827.data.SAssign.val;
#line 1573 "src/mir.tr"
        if ((((((unsigned long long)(tgt)) != ((unsigned long long)(0LL))) && (!_b3_is_plain_ident(tgt))) && ({ TrStr _cmpa_t828 = (_b3_root_ident(tgt)); TrStr _cmpb_t829 = (pn); _Bool _cmpr_t830 = _tr_str_eqv(_cmpa_t828, _cmpb_t829); _tr_str_release(_cmpa_t828); _cmpr_t830; }))) {
#line 1574 "src/mir.tr"
            ({ TrStr _at_t831 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot assign through '", 23LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t831); _tr_str_release(_at_t831); });
        } else if (_b3_nested_mut_call(val, pn, mm)) {
#line 1576 "src/mir.tr"
            ({ TrStr _at_t832 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot call a mutating method on a part of '", 44LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t832); _tr_str_release(_at_t832); });
        }
    } else if (_t827.tag == HirStmt_SExpr) {
        __auto_type e = _t827.data.SExpr.expr;
#line 1578 "src/mir.tr"
        if (_b3_nested_mut_call(e, pn, mm)) {
#line 1579 "src/mir.tr"
            ({ TrStr _at_t833 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot call a mutating method on a part of '", 44LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t833); _tr_str_release(_at_t833); });
        }
    } else if (_t827.tag == HirStmt_SLet) {
        __auto_type v = _t827.data.SLet.val;
#line 1581 "src/mir.tr"
        if (_b3_nested_mut_call(v, pn, mm)) {
#line 1582 "src/mir.tr"
            ({ TrStr _at_t834 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot call a mutating method on a part of '", 44LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — it is a SHARED `ref` parameter (read-only)", 48LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t834); _tr_str_release(_at_t834); });
        }
    } else if (_t827.tag == HirStmt_SIf) {
        __auto_type tb = _t827.data.SIf.then_b;
__auto_type eb = _t827.data.SIf.else_b;
#line 1584 "src/mir.tr"
        _b3_block(tb, pn, mm, out);
#line 1585 "src/mir.tr"
        _b3_block(eb, pn, mm, out);
    } else if (_t827.tag == HirStmt_SWhile) {
        __auto_type body = _t827.data.SWhile.body;
        _b3_block(body, pn, mm, out);
    } else if (_t827.tag == HirStmt_SFor) {
        __auto_type body = _t827.data.SFor.body;
        _b3_block(body, pn, mm, out);
    } else if (_t827.tag == HirStmt_SForUnpack) {
        __auto_type body = _t827.data.SForUnpack.body;
        _b3_block(body, pn, mm, out);
    } else if (_t827.tag == HirStmt_SUnsafe) {
        __auto_type body = _t827.data.SUnsafe.body;
        _b3_block(body, pn, mm, out);
    } else if (_t827.tag == HirStmt_SWith) {
        __auto_type body = _t827.data.SWith.body;
        _b3_block(body, pn, mm, out);
    } else if (_t827.tag == HirStmt_SMatch) {
        __auto_type arms = _t827.data.SMatch.arms;
#line 1592 "src/mir.tr"
        long long i = 0LL;
#line 1593 "src/mir.tr"
        while ((i < arms->len)) {
#line 1594 "src/mir.tr"
            _b3_block(((HirMatchArm*)List_ptr_get(arms, i))->body, pn, mm, out);
#line 1595 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t827.tag == HirStmt_STry) {
        __auto_type tb = _t827.data.STry.try_body;
__auto_type catches = _t827.data.STry.catches;
__auto_type fb = _t827.data.STry.finally_b;
#line 1597 "src/mir.tr"
        _b3_block(tb, pn, mm, out);
#line 1598 "src/mir.tr"
        long long i = 0LL;
#line 1599 "src/mir.tr"
        while ((i < catches->len)) {
#line 1600 "src/mir.tr"
            _b3_block((*((HirCatchClause**)List_ptr_get(catches, i)))->body, pn, mm, out);
#line 1601 "src/mir.tr"
            i = (i + 1LL);
        }
#line 1602 "src/mir.tr"
        _b3_block(fb, pn, mm, out);
    } else if (_t827.tag == HirStmt_SDefer) {
        __auto_type inner = _t827.data.SDefer.stmt;
        _b3_stmt(inner, pn, mm, out);
    } else if (1) {
        __auto_type _ = _t827;
        /* pass */
    }
}

__attribute__((hot)) bool _b3_pass_to_mut(HirExpr* e, TrStr pn, TrMap* sigs) {
#line 1608 "src/mir.tr"
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
#line 1608 "src/mir.tr"
        return false;
    }
#line 1609 "src/mir.tr"
    __auto_type _t835 = (*e);
    if (_t835.tag == HirExpr_ECall) {
        __auto_type callee = _t835.data.ECall.callee;
__auto_type args = _t835.data.ECall.args;
#line 1611 "src/mir.tr"
        __auto_type _t836 = (*callee);
        if (_t836.tag == HirExpr_EIdent) {
            __auto_type fnm = _t836.data.EIdent.name;
#line 1613 "src/mir.tr"
            if (_tr_dict_contains(sigs, _tr_strz(fnm))) {
#line 1614 "src/mir.tr"
                AstType* sg = ((AstType*)(uintptr_t)_tr_dict_get(sigs, _tr_strz(fnm)));
#line 1615 "src/mir.tr"
                long long k = 0LL;
#line 1616 "src/mir.tr"
                while (((k < args->len) && (k < (sg->args->len - 1LL)))) {
#line 1617 "src/mir.tr"
                    AstType** pt = ((AstType**)List_ptr_get(sg->args, k));
#line 1618 "src/mir.tr"
                    if (((((unsigned long long)(pt)) != ((unsigned long long)(0LL))) && (*pt)->is_mut_borrow)) {
#line 1619 "src/mir.tr"
                        __auto_type _t837 = (*((HirExpr*)List_ptr_get(args, k)));
                        if (_t837.tag == HirExpr_EIdent) {
                            __auto_type an = _t837.data.EIdent.name;
#line 1621 "src/mir.tr"
                            if (_tr_str_eqv((an), (pn))) {
#line 1621 "src/mir.tr"
                                return true;
                            }
                        } else if (1) {
                            __auto_type _ = _t837;
                            /* pass */
                        }
                    }
#line 1623 "src/mir.tr"
                    k = (k + 1LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t836;
            /* pass */
        }
#line 1625 "src/mir.tr"
        long long j = 0LL;
#line 1626 "src/mir.tr"
        while ((j < args->len)) {
#line 1627 "src/mir.tr"
            if (_b3_pass_to_mut(((HirExpr*)List_ptr_get(args, j)), pn, sigs)) {
#line 1627 "src/mir.tr"
                return true;
            }
#line 1628 "src/mir.tr"
            j = (j + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t835;
        /* pass */
    }
#line 1630 "src/mir.tr"
    return false;
}

__attribute__((hot)) void _b3_stmt_pass(HirStmt* s, TrStr pn, TrMap* sigs, List_TrStr* out) {
#line 1633 "src/mir.tr"
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
#line 1633 "src/mir.tr"
        return;
    }
#line 1634 "src/mir.tr"
    __auto_type _t838 = (*s);
    if (_t838.tag == HirStmt_SExpr) {
        __auto_type e = _t838.data.SExpr.expr;
#line 1636 "src/mir.tr"
        if (_b3_pass_to_mut(e, pn, sigs)) {
#line 1636 "src/mir.tr"
            ({ TrStr _at_t839 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot pass '", 13LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to a `mut ref` parameter — it is a SHARED `ref` parameter (read-only)", 73LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t839); _tr_str_release(_at_t839); });
        }
    } else if (_t838.tag == HirStmt_SLet) {
        __auto_type v = _t838.data.SLet.val;
#line 1638 "src/mir.tr"
        if (_b3_pass_to_mut(v, pn, sigs)) {
#line 1638 "src/mir.tr"
            ({ TrStr _at_t840 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot pass '", 13LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to a `mut ref` parameter — it is a SHARED `ref` parameter (read-only)", 73LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t840); _tr_str_release(_at_t840); });
        }
    } else if (_t838.tag == HirStmt_SAssign) {
        __auto_type v = _t838.data.SAssign.val;
#line 1640 "src/mir.tr"
        if (_b3_pass_to_mut(v, pn, sigs)) {
#line 1640 "src/mir.tr"
            ({ TrStr _at_t841 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot pass '", 13LL)), (pn))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' to a `mut ref` parameter — it is a SHARED `ref` parameter (read-only)", 73LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t841); _tr_str_release(_at_t841); });
        }
    } else if (_t838.tag == HirStmt_SIf) {
        __auto_type tb = _t838.data.SIf.then_b;
__auto_type eb = _t838.data.SIf.else_b;
#line 1642 "src/mir.tr"
        _b3_block_pass(tb, pn, sigs, out);
#line 1643 "src/mir.tr"
        _b3_block_pass(eb, pn, sigs, out);
    } else if (_t838.tag == HirStmt_SWhile) {
        __auto_type body = _t838.data.SWhile.body;
        _b3_block_pass(body, pn, sigs, out);
    } else if (_t838.tag == HirStmt_SFor) {
        __auto_type body = _t838.data.SFor.body;
        _b3_block_pass(body, pn, sigs, out);
    } else if (_t838.tag == HirStmt_SForUnpack) {
        __auto_type body = _t838.data.SForUnpack.body;
        _b3_block_pass(body, pn, sigs, out);
    } else if (_t838.tag == HirStmt_SUnsafe) {
        __auto_type body = _t838.data.SUnsafe.body;
        _b3_block_pass(body, pn, sigs, out);
    } else if (_t838.tag == HirStmt_SWith) {
        __auto_type body = _t838.data.SWith.body;
        _b3_block_pass(body, pn, sigs, out);
    } else if (_t838.tag == HirStmt_SMatch) {
        __auto_type arms = _t838.data.SMatch.arms;
#line 1650 "src/mir.tr"
        long long i = 0LL;
#line 1651 "src/mir.tr"
        while ((i < arms->len)) {
#line 1652 "src/mir.tr"
            _b3_block_pass(((HirMatchArm*)List_ptr_get(arms, i))->body, pn, sigs, out);
#line 1653 "src/mir.tr"
            i = (i + 1LL);
        }
    } else if (_t838.tag == HirStmt_STry) {
        __auto_type tb = _t838.data.STry.try_body;
__auto_type catches = _t838.data.STry.catches;
__auto_type fb = _t838.data.STry.finally_b;
#line 1655 "src/mir.tr"
        _b3_block_pass(tb, pn, sigs, out);
#line 1656 "src/mir.tr"
        long long i = 0LL;
#line 1657 "src/mir.tr"
        while ((i < catches->len)) {
#line 1658 "src/mir.tr"
            _b3_block_pass((*((HirCatchClause**)List_ptr_get(catches, i)))->body, pn, sigs, out);
#line 1659 "src/mir.tr"
            i = (i + 1LL);
        }
#line 1660 "src/mir.tr"
        _b3_block_pass(fb, pn, sigs, out);
    } else if (1) {
        __auto_type _ = _t838;
        /* pass */
    }
}

__attribute__((hot)) void _b3_block_pass(HirBlock* b, TrStr pn, TrMap* sigs, List_TrStr* out) {
#line 1664 "src/mir.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 1664 "src/mir.tr"
        return;
    }
#line 1665 "src/mir.tr"
    long long i = 0LL;
#line 1666 "src/mir.tr"
    while ((i < b->stmts->len)) {
#line 1667 "src/mir.tr"
        _b3_stmt_pass(((HirStmt*)List_ptr_get(b->stmts, i)), pn, sigs, out);
#line 1668 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void _b3_aliases(HirBlock* b, List_TrStr* names) {
#line 1673 "src/mir.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 1673 "src/mir.tr"
        return;
    }
#line 1674 "src/mir.tr"
    long long i = 0LL;
#line 1675 "src/mir.tr"
    while ((i < b->stmts->len)) {
#line 1676 "src/mir.tr"
        __auto_type _t842 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t842.tag == HirStmt_SLet) {
            __auto_type ln = _t842.data.SLet.name;
__auto_type lv = _t842.data.SLet.val;
#line 1678 "src/mir.tr"
            if ((((unsigned long long)(lv)) != ((unsigned long long)(0LL)))) {
#line 1679 "src/mir.tr"
                __auto_type _t843 = (*lv);
                if (_t843.tag == HirExpr_EIdent) {
                    __auto_type src = _t843.data.EIdent.name;
#line 1681 "src/mir.tr"
                    long long k = 0LL;
#line 1682 "src/mir.tr"
                    bool known = false;
#line 1683 "src/mir.tr"
                    bool already = false;
#line 1684 "src/mir.tr"
                    while ((k < names->len)) {
#line 1685 "src/mir.tr"
                        if (({ TrStr _cmpa_t844 = (List_TrStr_get(names, k)); TrStr _cmpb_t845 = (src); _Bool _cmpr_t846 = _tr_str_eqv(_cmpa_t844, _cmpb_t845); _tr_str_release(_cmpa_t844); _cmpr_t846; })) {
#line 1685 "src/mir.tr"
                            known = true;
                        }
#line 1686 "src/mir.tr"
                        if (({ TrStr _cmpa_t847 = (List_TrStr_get(names, k)); TrStr _cmpb_t848 = (ln); _Bool _cmpr_t849 = _tr_str_eqv(_cmpa_t847, _cmpb_t848); _tr_str_release(_cmpa_t847); _cmpr_t849; })) {
#line 1686 "src/mir.tr"
                            already = true;
                        }
#line 1687 "src/mir.tr"
                        k = (k + 1LL);
                    }
#line 1688 "src/mir.tr"
                    if ((known && (!already))) {
#line 1688 "src/mir.tr"
                        List_TrStr_append(names, ln);
                    }
                } else if (1) {
                    __auto_type _ = _t843;
                    /* pass */
                }
            }
        } else if (_t842.tag == HirStmt_SIf) {
            __auto_type tb = _t842.data.SIf.then_b;
__auto_type eb = _t842.data.SIf.else_b;
#line 1691 "src/mir.tr"
            _b3_aliases(tb, names);
#line 1692 "src/mir.tr"
            _b3_aliases(eb, names);
        } else if (_t842.tag == HirStmt_SWhile) {
            __auto_type body = _t842.data.SWhile.body;
            _b3_aliases(body, names);
        } else if (_t842.tag == HirStmt_SFor) {
            __auto_type body = _t842.data.SFor.body;
            _b3_aliases(body, names);
        } else if (1) {
            __auto_type _ = _t842;
            /* pass */
        }
#line 1696 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void _b3_alias_mut(HirBlock* b, TrStr an, TrMap* mm, List_TrStr* out, TrStr pn) {
#line 1700 "src/mir.tr"
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
#line 1700 "src/mir.tr"
        return;
    }
#line 1701 "src/mir.tr"
    long long i = 0LL;
#line 1702 "src/mir.tr"
    while ((i < b->stmts->len)) {
#line 1703 "src/mir.tr"
        HirStmt* st = ((HirStmt*)List_ptr_get(b->stmts, i));
#line 1704 "src/mir.tr"
        HirExpr* ev = ((HirExpr*)(0LL));
#line 1705 "src/mir.tr"
        __auto_type _t850 = (*st);
        if (_t850.tag == HirStmt_SExpr) {
            __auto_type e = _t850.data.SExpr.expr;
            ev = e;
        } else if (_t850.tag == HirStmt_SLet) {
            __auto_type lv = _t850.data.SLet.val;
            ev = lv;
        } else if (_t850.tag == HirStmt_SAssign) {
            __auto_type av = _t850.data.SAssign.val;
            ev = av;
        } else if (_t850.tag == HirStmt_SIf) {
            __auto_type tb = _t850.data.SIf.then_b;
__auto_type eb = _t850.data.SIf.else_b;
#line 1710 "src/mir.tr"
            _b3_alias_mut(tb, an, mm, out, pn);
#line 1711 "src/mir.tr"
            _b3_alias_mut(eb, an, mm, out, pn);
        } else if (_t850.tag == HirStmt_SWhile) {
            __auto_type body = _t850.data.SWhile.body;
            _b3_alias_mut(body, an, mm, out, pn);
        } else if (_t850.tag == HirStmt_SFor) {
            __auto_type body = _t850.data.SFor.body;
            _b3_alias_mut(body, an, mm, out, pn);
        } else if (1) {
            __auto_type _ = _t850;
            /* pass */
        }
#line 1715 "src/mir.tr"
        if (_is_mutating_call_on(ev, an, mm, _tr_str_lit_len("", 0LL))) {
#line 1716 "src/mir.tr"
            ({ TrStr _at_t851 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("cannot call a mutating method on '", 34LL)), (an))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("', an alias of '", 16LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' — '", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' is a SHARED `ref` parameter (read-only)", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); List_TrStr_append(out, _at_t851); _tr_str_release(_at_t851); });
        }
#line 1717 "src/mir.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) List_TrStr* mir_shared_ref_param_field_writes(HirFunction* hf, TrMap* mm, TrMap* sigs) {
#line 1720 "src/mir.tr"
    List_TrStr* out = (void*)List_TrStr_new();
#line 1721 "src/mir.tr"
    long long pi = 0LL;
#line 1722 "src/mir.tr"
    while ((pi < hf->params->len)) {
#line 1723 "src/mir.tr"
        AstType* pty = ((HirParam*)List_ptr_get(hf->params, pi))->ty;
#line 1724 "src/mir.tr"
        if ((pty->is_borrow && (!pty->is_mut_borrow))) {
#line 1725 "src/mir.tr"
            TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(hf->params, pi))->name);
#line 1726 "src/mir.tr"
            _b3_block(hf->body, pn, mm, out);
#line 1727 "src/mir.tr"
            List_TrStr* names = (void*)List_TrStr_new();
#line 1728 "src/mir.tr"
            List_TrStr_append(names, pn);
#line 1729 "src/mir.tr"
            _b3_aliases(hf->body, names);
#line 1730 "src/mir.tr"
            long long ni = 0LL;
#line 1731 "src/mir.tr"
            while ((ni < names->len)) {
#line 1732 "src/mir.tr"
                ({ __auto_type _ord_t853 = (hf->body); TrStr _at_t852 = (List_TrStr_get(names, ni)); _b3_block_pass(_ord_t853, _at_t852, sigs, out); _tr_str_release(_at_t852); });
#line 1733 "src/mir.tr"
                if ((ni > 0LL)) {
#line 1733 "src/mir.tr"
                    ({ __auto_type _ord_t855 = (hf->body); TrStr _at_t854 = (List_TrStr_get(names, ni)); _b3_alias_mut(_ord_t855, _at_t854, mm, out, pn); _tr_str_release(_at_t854); });
                }
#line 1734 "src/mir.tr"
                ni = (ni + 1LL);
            }
        }
#line 1735 "src/mir.tr"
        pi = (pi + 1LL);
    }
#line 1736 "src/mir.tr"
    return out;
}

__attribute__((hot)) List_ptr* mir_if_drop_plan(HirFunction* hf) {
#line 1739 "src/mir.tr"
    List_ptr* out = (void*)List_ptr_new();
#line 1740 "src/mir.tr"
    MirFunction* mf = lower_function(hf);
#line 1741 "src/mir.tr"
    if ((!mf->complete)) {
#line 1741 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1742 "src/mir.tr"
    if ((mf->if_bodies->len == 0LL)) {
#line 1742 "src/mir.tr"
        _tr_obj_release(mf, _trdrop_MirFunction);
        return out;
    }
#line 1743 "src/mir.tr"
    List_ptr* live = compute_liveness(mf);
#line 1744 "src/mir.tr"
    List_ptr* drops = compute_drops(mf, live);
#line 1750 "src/mir.tr"
    long long bi = 0LL;
#line 1751 "src/mir.tr"
    while ((bi < mf->blocks->len)) {
#line 1752 "src/mir.tr"
        MirBlock* blk = ((MirBlock*)List_ptr_get(mf->blocks, bi));
#line 1753 "src/mir.tr"
        if (((((unsigned long long)(blk->hir_block)) != ((unsigned long long)(0LL))) && is_if_body(mf, blk->hir_block))) {
#line 1754 "src/mir.tr"
            LiveSet* dd = ((LiveSet*)List_ptr_get(drops, blk->id));
#line 1755 "src/mir.tr"
            long long pi = 0LL;
#line 1756 "src/mir.tr"
            while ((pi < dd->items->len)) {
#line 1757 "src/mir.tr"
                DropSite* site = site_for(out, blk->hir_block);
#line 1758 "src/mir.tr"
                ({ __auto_type _ord_t857 = (site->places); TrStr _at_t856 = (List_TrStr_get(dd->items, pi)); set_add(_ord_t857, _at_t856); _tr_str_release(_at_t856); });
#line 1759 "src/mir.tr"
                pi = (pi + 1LL);
            }
        }
#line 1760 "src/mir.tr"
        bi = (bi + 1LL);
    }
#line 1761 "src/mir.tr"
    _tr_obj_release(mf, _trdrop_MirFunction);
    List_ptr_free_obj(drops, _trdrop_LiveSet);
    return out;
}

__attribute__((hot)) TrStr set_str(List_TrStr* v) {
#line 1764 "src/mir.tr"
    TrStr s = _tr_str_lit_len("{", 1LL);
#line 1765 "src/mir.tr"
    long long i = 0LL;
#line 1766 "src/mir.tr"
    while ((i < v->len)) {
#line 1767 "src/mir.tr"
        if ((i > 0LL)) {
#line 1767 "src/mir.tr"
            TrStr _strtmp_t859 = _tr_strx_concatv((s), (_tr_str_lit_len(", ", 2LL)));
            _tr_str_release(s);
            s = _strtmp_t859;
        }
#line 1768 "src/mir.tr"
        TrStr _strtmp_t861 = ({ TrStr _tr_cr = (List_TrStr_get(v, i)); TrStr _tr_cres = _tr_strx_concatv((s), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
        _tr_str_release(s);
        s = _strtmp_t861;
#line 1769 "src/mir.tr"
        i = (i + 1LL);
    }
#line 1770 "src/mir.tr"
    TrStr _trret175 = _tr_strx_concatv((s), (_tr_str_lit_len("}", 1LL)));
    _tr_str_release(s);
    return _trret175;
}

__attribute__((hot)) TrStr term_str(MirTerm* t) {
#line 1774 "src/mir.tr"
    __auto_type _t862 = (*t);
    if (_t862.tag == MirTerm_TGoto) {
        __auto_type n = _t862.data.TGoto.target;
        return ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("goto bb", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
    } else if (_t862.tag == MirTerm_TBranch) {
        __auto_type tt = _t862.data.TBranch.t;
__auto_type ee = _t862.data.TBranch.e;
        return ({ __auto_type _ord_t863 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(tt)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("branch <cond> ? bb", 18LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" : bb", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); __auto_type _wr = (({ TrStr _tr_cl = (_ord_t863); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(ee)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); _wr; });
    } else if (_t862.tag == MirTerm_TReturn) {
        __auto_type _ = _t862.data.TReturn.value;
        return _tr_str_lit_len("return <expr>", 13LL);
    } else if (_t862.tag == MirTerm_TReturnVoid) {
        return _tr_str_lit_len("return", 6LL);
    } else if (_t862.tag == MirTerm_TUnset) {
        return _tr_str_lit_len("<unset>", 7LL);
    }
#line 1780 "src/mir.tr"
    return _tr_str_lit_len("<?>", 3LL);
}

__attribute__((hot)) TrStr stmt_str(MirStmt* s) {
#line 1783 "src/mir.tr"
    __auto_type _t864 = (*s);
    if (_t864.tag == MirStmt_MDeclare) {
        __auto_type p = _t864.data.MDeclare.place;
        TrStr _trret176 = ({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("let ", 4LL)), (p))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = <expr>", 9LL))); _tr_str_release(_tr_cl); _tr_cres; });
        return _trret176;
    } else if (_t864.tag == MirStmt_MAssign) {
        __auto_type p = _t864.data.MAssign.place;
        TrStr _trret177 = _tr_strx_concatv((p), (_tr_str_lit_len(" = <expr>", 9LL)));
        return _trret177;
    } else if (_t864.tag == MirStmt_MEval) {
        __auto_type _ = _t864.data.MEval.value;
        return _tr_str_lit_len("eval <expr>", 11LL);
    }
#line 1787 "src/mir.tr"
    return _tr_str_lit_len("<?>", 3LL);
}

__attribute__((hot)) TrStr dump_mir(MirProgram* mp) {
#line 1790 "src/mir.tr"
    TrStr out = _tr_str_lit_len("", 0LL);
#line 1791 "src/mir.tr"
    long long fi = 0LL;
#line 1792 "src/mir.tr"
    while ((fi < mp->functions->len)) {
#line 1793 "src/mir.tr"
        MirFunction* f = ((MirFunction*)List_ptr_get(mp->functions, fi));
#line 1794 "src/mir.tr"
        TrStr tag = _tr_str_lit_len(" [complete]", 11LL);
#line 1795 "src/mir.tr"
        if ((!f->complete)) {
#line 1795 "src/mir.tr"
            TrStr _strtmp_t865 = _tr_str_lit_len(" [INCOMPLETE - falls back to HIR]", 33LL);
            _tr_str_release(tag);
            tag = _strtmp_t865;
        }
#line 1796 "src/mir.tr"
        ({ TrStr _ap_t866 = (_tr_str_lit_len("fn ", 3LL)); TrStr _ap_t867 = (f->name); TrStr _ap_t868 = (_tr_str_lit_len("(", 1LL)); TrStr _ap_t869 = (_tr_str_wrap(_tr_int_to_str((long long)(f->params->len)))); TrStr _ap_t870 = (_tr_str_lit_len(" params), ", 10LL)); TrStr _ap_t871 = (_tr_str_wrap(_tr_int_to_str((long long)(f->blocks->len)))); TrStr _ap_t872 = (_tr_str_lit_len(" blocks", 7LL)); TrStr _ap_t873 = (tag); TrStr _ap_t874 = (_tr_str_lit_len(":\n", 2LL)); _tr_str_append(&out, _ap_t866); _tr_str_append(&out, _ap_t867); _tr_str_append(&out, _ap_t868); _tr_str_append(&out, _ap_t869); _tr_str_append(&out, _ap_t870); _tr_str_append(&out, _ap_t871); _tr_str_append(&out, _ap_t872); _tr_str_append(&out, _ap_t873); _tr_str_append(&out, _ap_t874); _tr_str_release(_ap_t869); _tr_str_release(_ap_t871); });
#line 1797 "src/mir.tr"
        List_ptr* live = compute_liveness(f);
#line 1798 "src/mir.tr"
        List_ptr* drops = compute_drops(f, live);
#line 1800 "src/mir.tr"
        if ((f->borrows->len > 0LL)) {
#line 1801 "src/mir.tr"
            List_bool* outl = compute_borrow_outlives(f, live);
#line 1802 "src/mir.tr"
            long long ei = 0LL;
#line 1803 "src/mir.tr"
            while ((ei < f->borrows->len)) {
#line 1804 "src/mir.tr"
                BorrowEdge* ed = ((BorrowEdge*)List_ptr_get(f->borrows, ei));
#line 1805 "src/mir.tr"
                TrStr verdict = _tr_str_lit_len("NEEDS-EXTENSION", 15LL);
#line 1806 "src/mir.tr"
                if (List_bool_get(outl, ei)) {
#line 1806 "src/mir.tr"
                    TrStr _strtmp_t875 = _tr_str_lit_len("PROVEN", 6LL);
                    _tr_str_release(verdict);
                    verdict = _strtmp_t875;
                }
#line 1807 "src/mir.tr"
                ({ TrStr _ap_t876 = (_tr_str_lit_len("  ; borrow ", 11LL)); TrStr _ap_t877 = (ed->borrower); TrStr _ap_t878 = (_tr_str_lit_len(" <- ", 4LL)); TrStr _ap_t879 = (ed->source); TrStr _ap_t880 = (_tr_str_lit_len(" : ", 3LL)); TrStr _ap_t881 = (verdict); TrStr _ap_t882 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t876); _tr_str_append(&out, _ap_t877); _tr_str_append(&out, _ap_t878); _tr_str_append(&out, _ap_t879); _tr_str_append(&out, _ap_t880); _tr_str_append(&out, _ap_t881); _tr_str_append(&out, _ap_t882); });
#line 1808 "src/mir.tr"
                ei = (ei + 1LL);
                _tr_str_release(verdict);
            }
        }
#line 1809 "src/mir.tr"
        long long bi = 0LL;
#line 1810 "src/mir.tr"
        while ((bi < f->blocks->len)) {
#line 1811 "src/mir.tr"
            MirBlock* blk = ((MirBlock*)List_ptr_get(f->blocks, bi));
#line 1812 "src/mir.tr"
            ({ TrStr _ap_t883 = (_tr_str_lit_len("  bb", 4LL)); TrStr _ap_t884 = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _ap_t885 = (_tr_str_lit_len(":\n", 2LL)); _tr_str_append(&out, _ap_t883); _tr_str_append(&out, _ap_t884); _tr_str_append(&out, _ap_t885); _tr_str_release(_ap_t884); });
#line 1813 "src/mir.tr"
            long long si = 0LL;
#line 1814 "src/mir.tr"
            while ((si < blk->stmts->len)) {
#line 1815 "src/mir.tr"
                ({ TrStr _ap_t886 = (_tr_str_lit_len("    ", 4LL)); TrStr _ap_t887 = (stmt_str(((MirStmt*)List_ptr_get(blk->stmts, si)))); TrStr _ap_t888 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t886); _tr_str_append(&out, _ap_t887); _tr_str_append(&out, _ap_t888); _tr_str_release(_ap_t887); });
#line 1816 "src/mir.tr"
                si = (si + 1LL);
            }
#line 1817 "src/mir.tr"
            ({ TrStr _ap_t889 = (_tr_str_lit_len("    ", 4LL)); TrStr _ap_t890 = (term_str(blk->term)); TrStr _ap_t891 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t889); _tr_str_append(&out, _ap_t890); _tr_str_append(&out, _ap_t891); _tr_str_release(_ap_t890); });
#line 1818 "src/mir.tr"
            TrStr _strtmp_t895 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((out), (_tr_str_lit_len("    ; live_out ", 15LL)))); TrStr _tr_cr = (set_str(((LiveSet*)List_ptr_get(live, blk->id))->items)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; });
            _tr_str_release(out);
            out = _strtmp_t895;
#line 1819 "src/mir.tr"
            ({ TrStr _ap_t896 = (_tr_str_lit_len("    ; drop ", 11LL)); TrStr _ap_t897 = (set_str(((LiveSet*)List_ptr_get(drops, blk->id))->items)); TrStr _ap_t898 = (_tr_str_lit_len("\n", 1LL)); _tr_str_append(&out, _ap_t896); _tr_str_append(&out, _ap_t897); _tr_str_append(&out, _ap_t898); _tr_str_release(_ap_t897); });
#line 1820 "src/mir.tr"
            bi = (bi + 1LL);
        }
#line 1821 "src/mir.tr"
        fi = (fi + 1LL);
        _tr_str_release(tag);
        List_ptr_free_obj(drops, _trdrop_LiveSet);
    }
#line 1822 "src/mir.tr"
    TrStr _trret178 = _tr_str_retain(out);
    _tr_str_release(out);
    return _trret178;
}

