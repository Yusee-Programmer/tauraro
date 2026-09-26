#include "tauraro_types.h"


__attribute__((hot)) GVal* GVal_make(TrStr ty, TrStr val) {
    /* pass */
    GVal* v = ((GVal*)_tr_obj_alloc(sizeof(GVal)));
    /* pass */
    v->ty = _tr_str_retain(ty);
    /* pass */
    v->val = _tr_str_retain(val);
    /* pass */
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) GpuEmitter* GpuEmitter_init(TrStr target) {
    /* pass */
    GpuEmitter* e = ((GpuEmitter*)_tr_obj_alloc(sizeof(GpuEmitter)));
    /* pass */
    e->sb = StringBuilder_init(1024LL);
    /* pass */
    e->target = _tr_str_retain(target);
    /* pass */
    e->tmp = 0LL;
    /* pass */
    e->lbl = 0LL;
    /* pass */
    e->ok = true;
    /* pass */
    e->fail_note = _tr_str_lit_len("", 0LL);
    /* pass */
    e->var_names = (void*)List_TrStr_new();
    /* pass */
    e->var_ll = (void*)List_TrStr_new();
    /* pass */
    e->var_elem = (void*)List_TrStr_new();
    /* pass */
    e->ret_llty = _tr_str_lit_len("void", 4LL);
    /* pass */
    e->dev_fns = (void*)List_ptr_new();
    /* pass */
    return e;
}

__attribute__((hot)) TrStr GpuEmitter__gpu_ret_ty(GpuEmitter* self, AstType* t) {
    /* pass */
    if (((_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((t->name), (_tr_str_lit_len("None", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("", 0LL))))) {
        /* pass */
        return _tr_str_lit_len("void", 4LL);
    }
    /* pass */
    if (_tr_str_eqv((t->name), (_tr_str_lit_len("Pointer", 7LL)))) {
        /* pass */
        return _tr_str_lit_len("ptr addrspace(1)", 16LL);
    }
    /* pass */
    return _gpu_scalar_ty(t->name);
}

__attribute__((hot)) void GpuEmitter_w(GpuEmitter* self, TrStr s) {
    /* pass */
    StringBuilder_append(self->sb, s);
}

__attribute__((hot)) TrStr GpuEmitter_fresh(GpuEmitter* self) {
    /* pass */
    TrStr r = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->tmp)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("%t", 2LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    self->tmp = (self->tmp + 1LL);
    /* pass */
    return r;
}

__attribute__((hot)) TrStr GpuEmitter_newlbl(GpuEmitter* self) {
    /* pass */
    TrStr r = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->lbl)))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("L", 1LL)), _cr); _tr_str_release(_cr); _cres; });
    /* pass */
    self->lbl = (self->lbl + 1LL);
    /* pass */
    return r;
}

__attribute__((hot)) void GpuEmitter_fail(GpuEmitter* self, TrStr why) {
    /* pass */
    self->ok = false;
    /* pass */
    if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        self->fail_note = _tr_str_retain(why);
    }
}

__attribute__((hot)) void GpuEmitter_add_var(GpuEmitter* self, TrStr name, AstType* ty) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->var_names->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(self->var_names, i)), (name))) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (ty->args->len > 0LL))) {
        /* pass */
        TrStr et = _gpu_scalar_ty((*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
        /* pass */
        List_TrStr_append(self->var_names, name);
        /* pass */
        List_TrStr_append(self->var_ll, _tr_str_lit_len("ptr addrspace(1)", 16LL));
        /* pass */
        List_TrStr_append(self->var_elem, et);
        _tr_str_release(et);
    } else {
        /* pass */
        List_TrStr_append(self->var_names, name);
        /* pass */
        ({ TrStr _at_t3424 = (_gpu_scalar_ty(ty->name)); List_TrStr_append(self->var_ll, _at_t3424); _tr_str_release(_at_t3424); });
        /* pass */
        List_TrStr_append(self->var_elem, _tr_str_lit_len("", 0LL));
    }
}

__attribute__((hot)) TrStr GpuEmitter_var_slot_ty(GpuEmitter* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->var_names->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(self->var_names, i)), (name))) {
            /* pass */
            return List_TrStr_get(self->var_ll, i);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit_len("i64", 3LL);
}

__attribute__((hot)) TrStr GpuEmitter_var_elem_ty(GpuEmitter* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->var_names->len)) {
        /* pass */
        if (_tr_str_eqv((List_TrStr_get(self->var_names, i)), (name))) {
            /* pass */
            return List_TrStr_get(self->var_elem, i);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void GpuEmitter_scan_vars_block(GpuEmitter* self, HirBlock* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        GpuEmitter_scan_vars_stmt(self, (*((HirStmt*)List_ptr_get(b->stmts, i))));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void GpuEmitter_scan_vars_stmt(GpuEmitter* self, HirStmt s) {
    /* pass */
    __auto_type _t3425 = s;
    if (_t3425.tag == HirStmt_SLet) {
        __auto_type name = _t3425.data.SLet.name;
__auto_type ownership = _t3425.data.SLet.ownership;
__auto_type is_mut = _t3425.data.SLet.is_mut;
__auto_type is_const = _t3425.data.SLet.is_const;
__auto_type is_shared = _t3425.data.SLet.is_shared;
__auto_type ty = _t3425.data.SLet.ty;
__auto_type val = _t3425.data.SLet.val;
        /* pass */
        AstType* vt = ty;
        /* pass */
        if (_tr_str_eqv((vt->name), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            vt = hir_expr_type(val);
        }
        /* pass */
        GpuEmitter_add_var(self, name, vt);
    } else if (_t3425.tag == HirStmt_SIf) {
        __auto_type cond = _t3425.data.SIf.cond;
__auto_type then_b = _t3425.data.SIf.then_b;
__auto_type else_b = _t3425.data.SIf.else_b;
        /* pass */
        GpuEmitter_scan_vars_block(self, then_b);
        /* pass */
        GpuEmitter_scan_vars_block(self, else_b);
    } else if (_t3425.tag == HirStmt_SWhile) {
        __auto_type cond = _t3425.data.SWhile.cond;
__auto_type body = _t3425.data.SWhile.body;
        /* pass */
        GpuEmitter_scan_vars_block(self, body);
    } else if (_t3425.tag == HirStmt_SUnsafe) {
        __auto_type body = _t3425.data.SUnsafe.body;
        /* pass */
        GpuEmitter_scan_vars_block(self, body);
    } else if (1) {
        __auto_type _ = _t3425;
        /* pass */
        /* pass */
    }
}

__attribute__((hot)) TrStr GpuEmitter_emit_kernel(GpuEmitter* self, HirFunction* f) {
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        GpuEmitter_add_var(self, p->name, p->ty);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    GpuEmitter_scan_vars_block(self, f->body);
    /* pass */
    TrStr cc = _tr_str_lit_len("spir_kernel", 11LL);
    /* pass */
    if (_tr_str_eqv((self->target), (_tr_str_lit_len("nvptx", 5LL)))) {
        /* pass */
        TrStr _strtmp_t3426 = _tr_str_lit_len("ptx_kernel", 10LL);
        _tr_str_release(cc);
        cc = _strtmp_t3426;
    }
    /* pass */
    ({ TrStr _at_t3427 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (cc))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" void @", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3427); _tr_str_release(_at_t3427); });
    /* pass */
    long long pj = 0LL;
    /* pass */
    while ((pj < f->params->len)) {
        /* pass */
        HirParam* pp = ((HirParam*)List_ptr_get(f->params, pj));
        /* pass */
        if ((pj > 0LL)) {
            /* pass */
            GpuEmitter_w(self, _tr_str_lit_len(", ", 2LL));
        }
        /* pass */
        ({ TrStr _at_t3428 = (({ TrStr _cl = (({ TrStr _cl = (GpuEmitter_var_slot_ty(self, pp->name)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pp->name)); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3428); _tr_str_release(_at_t3428); });
        /* pass */
        pj = (pj + 1LL);
    }
    /* pass */
    GpuEmitter_w(self, _tr_str_lit_len(") {\nentry:\n", 11LL));
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < self->var_names->len)) {
        /* pass */
        ({ TrStr _at_t3429 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(self->var_names, vi)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(self->var_ll, vi)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3429); _tr_str_release(_at_t3429); });
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    long long pk = 0LL;
    /* pass */
    while ((pk < f->params->len)) {
        /* pass */
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pk))->name);
        /* pass */
        ({ TrStr _at_t3430 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3430); _tr_str_release(_at_t3430); });
        /* pass */
        pk = (pk + 1LL);
        _tr_str_release(pn);
    }
    /* pass */
    GpuEmitter_emit_block(self, f->body);
    /* pass */
    GpuEmitter_w(self, _tr_str_lit_len("  ret void\n}\n\n", 14LL));
    /* pass */
    if ((!self->ok)) {
        /* pass */
        _tr_str_release(cc);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("; kernel '", 10LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' NOT emitted: ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->fail_note)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n\n", 2LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    _tr_str_release(cc);
    return StringObj_as_str(StringBuilder_to_string(self->sb));
}

__attribute__((hot)) TrStr GpuEmitter_emit_device_fn(GpuEmitter* self, HirFunction* f) {
    /* pass */
    self->ret_llty = GpuEmitter__gpu_ret_ty(self, f->ret_ty);
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        GpuEmitter_add_var(self, p->name, p->ty);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    GpuEmitter_scan_vars_block(self, f->body);
    /* pass */
    ({ TrStr _at_t3431 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (self->ret_llty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3431); _tr_str_release(_at_t3431); });
    /* pass */
    long long pj = 0LL;
    /* pass */
    while ((pj < f->params->len)) {
        /* pass */
        HirParam* pp = ((HirParam*)List_ptr_get(f->params, pj));
        /* pass */
        if ((pj > 0LL)) {
            /* pass */
            GpuEmitter_w(self, _tr_str_lit_len(", ", 2LL));
        }
        /* pass */
        ({ TrStr _at_t3432 = (({ TrStr _cl = (({ TrStr _cl = (GpuEmitter_var_slot_ty(self, pp->name)); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pp->name)); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3432); _tr_str_release(_at_t3432); });
        /* pass */
        pj = (pj + 1LL);
    }
    /* pass */
    GpuEmitter_w(self, _tr_str_lit_len(") {\nentry:\n", 11LL));
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < self->var_names->len)) {
        /* pass */
        ({ TrStr _at_t3433 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(self->var_names, vi)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(self->var_ll, vi)); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3433); _tr_str_release(_at_t3433); });
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    long long pk = 0LL;
    /* pass */
    while ((pk < f->params->len)) {
        /* pass */
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pk))->name);
        /* pass */
        ({ TrStr _at_t3434 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), _cr); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3434); _tr_str_release(_at_t3434); });
        /* pass */
        pk = (pk + 1LL);
        _tr_str_release(pn);
    }
    /* pass */
    GpuEmitter_emit_block(self, f->body);
    /* pass */
    if (_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("void", 4LL)))) {
        /* pass */
        GpuEmitter_w(self, _tr_str_lit_len("  ret void\n}\n\n", 14LL));
    } else if (_gpu_is_float(self->ret_llty)) {
        /* pass */
        ({ TrStr _at_t3435 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" 0.0\n}\n\n", 8LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3435); _tr_str_release(_at_t3435); });
    } else if (_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("ptr addrspace(1)", 16LL)))) {
        /* pass */
        ({ TrStr _at_t3436 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" null\n}\n\n", 9LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3436); _tr_str_release(_at_t3436); });
    } else {
        /* pass */
        ({ TrStr _at_t3437 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" 0\n}\n\n", 6LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3437); _tr_str_release(_at_t3437); });
    }
    /* pass */
    if ((!self->ok)) {
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("; device fn '", 13LL)), (f->name))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' NOT emitted: ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (self->fail_note)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n\n", 2LL))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->sb));
}

__attribute__((hot)) void GpuEmitter_emit_block(GpuEmitter* self, HirBlock* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        GpuEmitter_emit_stmt(self, (*((HirStmt*)List_ptr_get(b->stmts, i))));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void GpuEmitter_emit_stmt(GpuEmitter* self, HirStmt s) {
    /* pass */
    __auto_type _t3438 = s;
    if (_t3438.tag == HirStmt_SLet) {
        __auto_type name = _t3438.data.SLet.name;
__auto_type ownership = _t3438.data.SLet.ownership;
__auto_type is_mut = _t3438.data.SLet.is_mut;
__auto_type is_const = _t3438.data.SLet.is_const;
__auto_type is_shared = _t3438.data.SLet.is_shared;
__auto_type ty = _t3438.data.SLet.ty;
__auto_type val = _t3438.data.SLet.val;
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, val);
        /* pass */
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
        /* pass */
        TrStr cv = GpuEmitter_coerce(self, v, slot);
        /* pass */
        ({ TrStr _at_t3439 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (slot))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3439); _tr_str_release(_at_t3439); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (_t3438.tag == HirStmt_SAssign) {
        __auto_type target = _t3438.data.SAssign.target;
__auto_type val = _t3438.data.SAssign.val;
        /* pass */
        GpuEmitter_emit_assign(self, (*target), val);
    } else if (_t3438.tag == HirStmt_SExpr) {
        __auto_type expr = _t3438.data.SExpr.expr;
        /* pass */
        GpuEmitter_emit_stmt_expr(self, (*expr));
    } else if (_t3438.tag == HirStmt_SIf) {
        __auto_type cond = _t3438.data.SIf.cond;
__auto_type then_b = _t3438.data.SIf.then_b;
__auto_type else_b = _t3438.data.SIf.else_b;
        /* pass */
        GVal* c = GpuEmitter_emit_expr(self, cond);
        /* pass */
        TrStr cc = GpuEmitter_coerce_bool(self, c);
        /* pass */
        TrStr lt = GpuEmitter_newlbl(self);
        /* pass */
        TrStr le = GpuEmitter_newlbl(self);
        /* pass */
        TrStr lend = GpuEmitter_newlbl(self);
        /* pass */
        ({ TrStr _at_t3440 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (cc))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (le)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3440); _tr_str_release(_at_t3440); });
        /* pass */
        ({ TrStr _at_t3441 = (_tr_strx_concatv((lt), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t3441); _tr_str_release(_at_t3441); });
        /* pass */
        GpuEmitter_emit_block(self, then_b);
        /* pass */
        ({ TrStr _at_t3442 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lend))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3442); _tr_str_release(_at_t3442); });
        /* pass */
        ({ TrStr _at_t3443 = (_tr_strx_concatv((le), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t3443); _tr_str_release(_at_t3443); });
        /* pass */
        GpuEmitter_emit_block(self, else_b);
        /* pass */
        ({ TrStr _at_t3444 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lend))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3444); _tr_str_release(_at_t3444); });
        /* pass */
        ({ TrStr _at_t3445 = (_tr_strx_concatv((lend), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t3445); _tr_str_release(_at_t3445); });
        _tr_obj_release(c, _trdrop_GVal);
        _tr_str_release(cc);
        _tr_str_release(lt);
        _tr_str_release(le);
        _tr_str_release(lend);
    } else if (_t3438.tag == HirStmt_SWhile) {
        __auto_type cond = _t3438.data.SWhile.cond;
__auto_type body = _t3438.data.SWhile.body;
        /* pass */
        TrStr lc = GpuEmitter_newlbl(self);
        /* pass */
        TrStr lb = GpuEmitter_newlbl(self);
        /* pass */
        TrStr lend2 = GpuEmitter_newlbl(self);
        /* pass */
        ({ TrStr _at_t3446 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lc))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3446); _tr_str_release(_at_t3446); });
        /* pass */
        ({ TrStr _at_t3447 = (_tr_strx_concatv((lc), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t3447); _tr_str_release(_at_t3447); });
        /* pass */
        GVal* cnd = GpuEmitter_emit_expr(self, cond);
        /* pass */
        TrStr cb = GpuEmitter_coerce_bool(self, cnd);
        /* pass */
        ({ TrStr _at_t3448 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (cb))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (lend2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3448); _tr_str_release(_at_t3448); });
        /* pass */
        ({ TrStr _at_t3449 = (_tr_strx_concatv((lb), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t3449); _tr_str_release(_at_t3449); });
        /* pass */
        GpuEmitter_emit_block(self, body);
        /* pass */
        ({ TrStr _at_t3450 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lc))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3450); _tr_str_release(_at_t3450); });
        /* pass */
        ({ TrStr _at_t3451 = (_tr_strx_concatv((lend2), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t3451); _tr_str_release(_at_t3451); });
        _tr_str_release(lc);
        _tr_str_release(lb);
        _tr_str_release(lend2);
        _tr_obj_release(cnd, _trdrop_GVal);
        _tr_str_release(cb);
    } else if (_t3438.tag == HirStmt_SUnsafe) {
        __auto_type body = _t3438.data.SUnsafe.body;
        /* pass */
        GpuEmitter_emit_block(self, body);
    } else if (_t3438.tag == HirStmt_SReturn) {
        __auto_type val = _t3438.data.SReturn.val;
        /* pass */
        if ((_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("void", 4LL))) || (((unsigned long long)(val)) == ((unsigned long long)(0LL))))) {
            /* pass */
            GpuEmitter_w(self, _tr_str_lit_len("  ret void\n", 11LL));
        } else {
            /* pass */
            GVal* rv = GpuEmitter_emit_expr(self, val);
            /* pass */
            if (_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("ptr addrspace(1)", 16LL)))) {
                /* pass */
                ({ TrStr _at_t3452 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ptr addrspace(1) ", 23LL)), (rv->val))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3452); _tr_str_release(_at_t3452); });
            } else {
                /* pass */
                TrStr cv = GpuEmitter_coerce(self, rv, self->ret_llty);
                /* pass */
                ({ TrStr _at_t3453 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3453); _tr_str_release(_at_t3453); });
            }
        }
        /* pass */
        TrStr lu = GpuEmitter_newlbl(self);
        /* pass */
        ({ TrStr _at_t3454 = (_tr_strx_concatv((lu), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t3454); _tr_str_release(_at_t3454); });
        _tr_str_release(lu);
    } else if (_t3438.tag == HirStmt_SPass) {
        /* pass */
        /* pass */
    } else if (_t3438.tag == HirStmt_SLineMarker) {
        __auto_type n = _t3438.data.SLineMarker.n;
        /* pass */
        /* pass */
    } else if (1) {
        __auto_type _ = _t3438;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported statement in kernel", 31LL));
    }
}

__attribute__((hot)) void GpuEmitter_emit_assign(GpuEmitter* self, HirExpr target, HirExpr* val) {
    /* pass */
    __auto_type _t3455 = target;
    if (_t3455.tag == HirExpr_EIdent) {
        __auto_type name = _t3455.data.EIdent.name;
__auto_type ty = _t3455.data.EIdent.ty;
__auto_type is_move = _t3455.data.EIdent.is_move;
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, val);
        /* pass */
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
        /* pass */
        TrStr cv = GpuEmitter_coerce(self, v, slot);
        /* pass */
        ({ TrStr _at_t3456 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (slot))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3456); _tr_str_release(_at_t3456); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t3455;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported assignment target in kernel", 39LL));
    }
}

__attribute__((hot)) void GpuEmitter_emit_stmt_expr(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3457 = e;
    if (_t3457.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t3457.data.EMethodCall.obj;
__auto_type method = _t3457.data.EMethodCall.method;
__auto_type args = _t3457.data.EMethodCall.args;
__auto_type ty = _t3457.data.EMethodCall.ty;
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("write", 5LL))) && (args->len == 1LL))) {
            /* pass */
            GpuEmitter_emit_store(self, (*obj), ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            return;
        }
        /* pass */
        GVal* _v = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v, _trdrop_GVal);
    } else if (_t3457.tag == HirExpr_ECall) {
        __auto_type callee = _t3457.data.ECall.callee;
__auto_type args = _t3457.data.ECall.args;
__auto_type ty = _t3457.data.ECall.ty;
        /* pass */
        if (_tr_str_eqv((GpuEmitter_callee_name(self, (*callee))), (_tr_str_lit_len("gpu_barrier", 11LL)))) {
            /* pass */
            if (_tr_str_eqv((self->target), (_tr_str_lit_len("nvptx", 5LL)))) {
                /* pass */
                GpuEmitter_w(self, _tr_str_lit_len("  call void @llvm.nvvm.barrier0()\n", 34LL));
            } else {
                /* pass */
                GpuEmitter_w(self, _tr_str_lit_len("  call spir_func void @_Z7barrierj(i32 1)\n", 42LL));
            }
            /* pass */
            return;
        }
        /* pass */
        GVal* _v2 = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v2, _trdrop_GVal);
    } else if (1) {
        __auto_type _ = _t3457;
        /* pass */
        GVal* _v3 = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v3, _trdrop_GVal);
    }
}

__attribute__((hot)) void GpuEmitter_emit_store(GpuEmitter* self, HirExpr chain, HirExpr* valp) {
    /* pass */
    __auto_type _t3458 = chain;
    if (_t3458.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t3458.data.EMethodCall.obj;
__auto_type pmeth = _t3458.data.EMethodCall.method;
__auto_type pargs = _t3458.data.EMethodCall.args;
__auto_type pty = _t3458.data.EMethodCall.ty;
        /* pass */
        if (((!_tr_str_eqv((pmeth), (_tr_str_lit_len("offset", 6LL)))) || (pargs->len != 1LL))) {
            /* pass */
            GpuEmitter_fail(self, _tr_str_lit_len("store target must be p.offset(i).write(v)", 41LL));
            /* pass */
            return;
        }
        /* pass */
        GVal* pr = GpuEmitter_emit_ptr(self, (*pobj));
        /* pass */
        GVal* idx = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(pargs, 0LL)));
        /* pass */
        TrStr i64idx = GpuEmitter_coerce(self, idx, _tr_str_lit_len("i64", 3LL));
        /* pass */
        TrStr gep = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3459 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gep))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = getelementptr ", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pr->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pr->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (i64idx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3459); _tr_str_release(_at_t3459); });
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, valp);
        /* pass */
        TrStr cv = GpuEmitter_coerce(self, v, pr->ty);
        /* pass */
        ({ TrStr _at_t3460 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (pr->ty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3460); _tr_str_release(_at_t3460); });
        _tr_obj_release(pr, _trdrop_GVal);
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t3458;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit_len("store target must be p.offset(i).write(v)", 41LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_ptr(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3461 = e;
    if (_t3461.tag == HirExpr_EIdent) {
        __auto_type name = _t3461.data.EIdent.name;
__auto_type ty = _t3461.data.EIdent.ty;
__auto_type is_move = _t3461.data.EIdent.is_move;
        /* pass */
        TrStr et = GpuEmitter_var_elem_ty(self, name);
        /* pass */
        if (_tr_str_eqv((et), (_tr_str_lit_len("", 0LL)))) {
            /* pass */
            TrStr _strtmp_t3462 = _tr_str_lit_len("float", 5LL);
            _tr_str_release(et);
            et = _strtmp_t3462;
        }
        /* pass */
        TrStr r = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3463 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ptr addrspace(1), ptr %var_", 35LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3463); _tr_str_release(_at_t3463); });
        /* pass */
        return GVal_make(et, r);
    } else if (_t3461.tag == HirExpr_ECast) {
        __auto_type inner = _t3461.data.ECast.expr;
__auto_type target_ty = _t3461.data.ECast.target_ty;
        /* pass */
        GVal* g = GpuEmitter_emit_ptr(self, (*inner));
        /* pass */
        if ((_tr_str_eqv((target_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (target_ty->args->len > 0LL))) {
            /* pass */
            g->ty = _gpu_scalar_ty((*((AstType**)List_ptr_get(target_ty->args, 0LL)))->name);
        }
        /* pass */
        return g;
    } else if (1) {
        __auto_type _ = _t3461;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported pointer expression in kernel", 40LL));
        /* pass */
        return GVal_make(_tr_str_lit_len("float", 5LL), _tr_str_lit_len("null", 4LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_expr(GpuEmitter* self, HirExpr* ep) {
    /* pass */
    return GpuEmitter_emit_expr_hir(self, (*ep));
}

__attribute__((hot)) GVal* GpuEmitter_emit_expr_hir(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3464 = e;
    if (_t3464.tag == HirExpr_ELitInt) {
        __auto_type val = _t3464.data.ELitInt.val;
__auto_type ty = _t3464.data.ELitInt.ty;
        /* pass */
        TrStr t = _gpu_scalar_ty(ty->name);
        /* pass */
        if (_gpu_is_float(t)) {
            /* pass */
            TrStr _strtmp_t3465 = _tr_str_lit_len("i64", 3LL);
            _tr_str_release(t);
            t = _strtmp_t3465;
        }
        /* pass */
        return ({ TrStr _at_t3466 = (_tr_str_wrap(_tr_int_to_str((long long)(val)))); __auto_type _wr = (GVal_make(t, _at_t3466)); _tr_str_release(_at_t3466); _wr; });
    } else if (_t3464.tag == HirExpr_ELitFloat) {
        __auto_type val = _t3464.data.ELitFloat.val;
__auto_type ty = _t3464.data.ELitFloat.ty;
        /* pass */
        TrStr ft = _gpu_scalar_ty(ty->name);
        /* pass */
        if ((!_gpu_is_float(ft))) {
            /* pass */
            TrStr _strtmp_t3467 = _tr_str_lit_len("double", 6LL);
            _tr_str_release(ft);
            ft = _strtmp_t3467;
        }
        /* pass */
        return ({ TrStr _at_t3468 = (GpuEmitter_float_lit(self, val)); __auto_type _wr = (GVal_make(ft, _at_t3468)); _tr_str_release(_at_t3468); _wr; });
    } else if (_t3464.tag == HirExpr_ELitBool) {
        __auto_type val = _t3464.data.ELitBool.val;
__auto_type ty = _t3464.data.ELitBool.ty;
        /* pass */
        if (val) {
            /* pass */
            return GVal_make(_tr_str_lit_len("i1", 2LL), _tr_str_lit_len("1", 1LL));
        }
        /* pass */
        return GVal_make(_tr_str_lit_len("i1", 2LL), _tr_str_lit_len("0", 1LL));
    } else if (_t3464.tag == HirExpr_EIdent) {
        __auto_type name = _t3464.data.EIdent.name;
__auto_type ty = _t3464.data.EIdent.ty;
__auto_type is_move = _t3464.data.EIdent.is_move;
        /* pass */
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
        /* pass */
        TrStr r = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3469 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3469); _tr_str_release(_at_t3469); });
        /* pass */
        return GVal_make(slot, r);
    } else if (_t3464.tag == HirExpr_EBinOp) {
        __auto_type op = _t3464.data.EBinOp.op;
__auto_type left = _t3464.data.EBinOp.left;
__auto_type right = _t3464.data.EBinOp.right;
__auto_type ty = _t3464.data.EBinOp.ty;
        /* pass */
        return GpuEmitter_emit_binop(self, op, left, right);
    } else if (_t3464.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t3464.data.EUnaryOp.op;
__auto_type expr = _t3464.data.EUnaryOp.expr;
__auto_type ty = _t3464.data.EUnaryOp.ty;
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, expr);
        /* pass */
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
            /* pass */
            TrStr r = GpuEmitter_fresh(self);
            /* pass */
            if (_gpu_is_float(v->ty)) {
                /* pass */
                ({ TrStr _at_t3470 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fneg ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3470); _tr_str_release(_at_t3470); });
            } else {
                /* pass */
                ({ TrStr _at_t3471 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = sub ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" 0, ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3471); _tr_str_release(_at_t3471); });
            }
            /* pass */
            return GVal_make(v->ty, r);
        }
        /* pass */
        return v;
    } else if (_t3464.tag == HirExpr_ECast) {
        __auto_type inner = _t3464.data.ECast.expr;
__auto_type target_ty = _t3464.data.ECast.target_ty;
        /* pass */
        GVal* iv = GpuEmitter_emit_expr(self, inner);
        /* pass */
        TrStr tt = _gpu_scalar_ty(target_ty->name);
        /* pass */
        if (_tr_str_eqv((target_ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
            /* pass */
            _tr_str_release(tt);
            return iv;
        }
        /* pass */
        return ({ TrStr _at_t3472 = (GpuEmitter_coerce(self, iv, tt)); __auto_type _wr = (GVal_make(tt, _at_t3472)); _tr_str_release(_at_t3472); _wr; });
    } else if (_t3464.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t3464.data.EMethodCall.obj;
__auto_type method = _t3464.data.EMethodCall.method;
__auto_type args = _t3464.data.EMethodCall.args;
__auto_type ty = _t3464.data.EMethodCall.ty;
        /* pass */
        if ((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) && (args->len == 0LL))) {
            /* pass */
            return GpuEmitter_emit_load(self, (*obj));
        }
        /* pass */
        ({ TrStr _at_t3473 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("unsupported method '", 20LL)), (method))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' in kernel", 11LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_fail(self, _at_t3473); _tr_str_release(_at_t3473); });
        /* pass */
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    } else if (_t3464.tag == HirExpr_ECall) {
        __auto_type callee = _t3464.data.ECall.callee;
__auto_type args = _t3464.data.ECall.args;
__auto_type ty = _t3464.data.ECall.ty;
        /* pass */
        return GpuEmitter_emit_call(self, (*callee), args);
    } else if (1) {
        __auto_type _ = _t3464;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported expression in kernel", 32LL));
        /* pass */
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_load(GpuEmitter* self, HirExpr chain) {
    /* pass */
    __auto_type _t3474 = chain;
    if (_t3474.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t3474.data.EMethodCall.obj;
__auto_type pmeth = _t3474.data.EMethodCall.method;
__auto_type pargs = _t3474.data.EMethodCall.args;
__auto_type pty = _t3474.data.EMethodCall.ty;
        /* pass */
        if (((!_tr_str_eqv((pmeth), (_tr_str_lit_len("offset", 6LL)))) || (pargs->len != 1LL))) {
            /* pass */
            GpuEmitter_fail(self, _tr_str_lit_len("load must be p.offset(i).read()", 31LL));
            /* pass */
            return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
        }
        /* pass */
        GVal* pr = GpuEmitter_emit_ptr(self, (*pobj));
        /* pass */
        GVal* idx = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(pargs, 0LL)));
        /* pass */
        TrStr i64idx = GpuEmitter_coerce(self, idx, _tr_str_lit_len("i64", 3LL));
        /* pass */
        TrStr gep = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3475 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gep))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = getelementptr ", 17LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pr->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pr->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (i64idx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3475); _tr_str_release(_at_t3475); });
        /* pass */
        TrStr r = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3476 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pr->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (gep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3476); _tr_str_release(_at_t3476); });
        /* pass */
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        return GVal_make(pr->ty, r);
    } else if (1) {
        __auto_type _ = _t3474;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit_len("load must be p.offset(i).read()", 31LL));
        /* pass */
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
}

__attribute__((hot)) TrStr GpuEmitter_callee_name(GpuEmitter* self, HirExpr c) {
    /* pass */
    __auto_type _t3477 = c;
    if (_t3477.tag == HirExpr_EIdent) {
        __auto_type name = _t3477.data.EIdent.name;
__auto_type ty = _t3477.data.EIdent.ty;
__auto_type is_move = _t3477.data.EIdent.is_move;
        return _tr_str_retain(name);
    } else if (1) {
        __auto_type _ = _t3477;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_call(GpuEmitter* self, HirExpr callee, List_ptr* args) {
    /* pass */
    TrStr nm = GpuEmitter_callee_name(self, callee);
    /* pass */
    if ((((((_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_id", 13LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_id", 12LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL))))) {
        /* pass */
        long long dim = 0LL;
        /* pass */
        if ((args->len == 1LL)) {
            /* pass */
            dim = GpuEmitter_literal_int(self, (*((HirExpr*)List_ptr_get(args, 0LL))));
        }
        /* pass */
        return GpuEmitter_emit_gpu_builtin(self, nm, dim);
    }
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < self->dev_fns->len)) {
        /* pass */
        HirFunction* df = ((HirFunction*)List_ptr_get(self->dev_fns, di));
        /* pass */
        if (_tr_str_eqv((df->name), (nm))) {
            /* pass */
            TrStr rty = GpuEmitter__gpu_ret_ty(self, df->ret_ty);
            /* pass */
            TrStr argstr = _tr_str_lit_len("", 0LL);
            /* pass */
            long long ai = 0LL;
            /* pass */
            while ((ai < args->len)) {
                /* pass */
                if ((ai > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t3478 = _tr_strx_concatv((argstr), (_tr_str_lit_len(", ", 2LL)));
                    _tr_str_release(argstr);
                    argstr = _strtmp_t3478;
                }
                /* pass */
                GVal* av = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(args, ai)));
                /* pass */
                TrStr aty = _tr_str_retain(av->ty);
                /* pass */
                TrStr aval = _tr_str_retain(av->val);
                /* pass */
                if ((ai < df->params->len)) {
                    /* pass */
                    TrStr ptt = GpuEmitter__gpu_ret_ty(self, ((HirParam*)List_ptr_get(df->params, ai))->ty);
                    /* pass */
                    if (((!_tr_str_eqv((ptt), (_tr_str_lit_len("ptr addrspace(1)", 16LL)))) && (!_tr_str_eqv((aty), (ptt))))) {
                        /* pass */
                        TrStr _strtmp_t3479 = GpuEmitter_coerce(self, av, ptt);
                        _tr_str_release(aval);
                        aval = _strtmp_t3479;
                        /* pass */
                        TrStr _strtmp_t3480 = _tr_str_retain(ptt);
                        _tr_str_release(aty);
                        aty = _strtmp_t3480;
                    }
                }
                /* pass */
                TrStr _strtmp_t3481 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((argstr), (aty))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (aval)); _tr_str_release(_cl); _cres; });
                _tr_str_release(argstr);
                argstr = _strtmp_t3481;
                /* pass */
                ai = (ai + 1LL);
                _tr_obj_release(av, _trdrop_GVal);
                _tr_str_release(aty);
                _tr_str_release(aval);
            }
            /* pass */
            if (_tr_str_eqv((rty), (_tr_str_lit_len("void", 4LL)))) {
                /* pass */
                ({ TrStr _at_t3482 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  call void @", 13LL)), (nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (argstr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3482); _tr_str_release(_at_t3482); });
                /* pass */
                _tr_str_release(nm);
                _tr_str_release(rty);
                _tr_str_release(argstr);
                return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
            }
            /* pass */
            TrStr r = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3483 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (rty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (argstr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3483); _tr_str_release(_at_t3483); });
            /* pass */
            _tr_str_release(nm);
            _tr_str_release(argstr);
            return GVal_make(rty, r);
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    ({ TrStr _at_t3484 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("call to '", 9LL)), (nm))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' not allowed in kernel (mark it @device, or use a GPU builtin)", 63LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_fail(self, _at_t3484); _tr_str_release(_at_t3484); });
    /* pass */
    _tr_str_release(nm);
    return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
}

__attribute__((hot)) GVal* GpuEmitter_emit_gpu_builtin(GpuEmitter* self, TrStr nm, long long dim) {
    /* pass */
    if (_tr_str_eqv((self->target), (_tr_str_lit_len("nvptx", 5LL)))) {
        /* pass */
        TrStr axis = _tr_str_lit_len("x", 1LL);
        /* pass */
        if ((dim == 1LL)) {
            /* pass */
            TrStr _strtmp_t3485 = _tr_str_lit_len("y", 1LL);
            _tr_str_release(axis);
            axis = _strtmp_t3485;
        }
        /* pass */
        if ((dim == 2LL)) {
            /* pass */
            TrStr _strtmp_t3486 = _tr_str_lit_len("z", 1LL);
            _tr_str_release(axis);
            axis = _strtmp_t3486;
        }
        /* pass */
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_id", 12LL)))) {
            /* pass */
            TrStr r = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3487 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.tid.", 41LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3487); _tr_str_release(_at_t3487); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r);
        }
        /* pass */
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) {
            /* pass */
            TrStr r2 = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3488 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r2))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.", 43LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3488); _tr_str_release(_at_t3488); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r2);
        }
        /* pass */
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) {
            /* pass */
            TrStr r3 = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3489 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r3))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid.", 42LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3489); _tr_str_release(_at_t3489); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r3);
        }
        /* pass */
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL)))) {
            /* pass */
            TrStr r4 = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3490 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r4))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid.", 44LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3490); _tr_str_release(_at_t3490); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r4);
        }
        /* pass */
        TrStr ct = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3491 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (ct))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.", 43LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3491); _tr_str_release(_at_t3491); });
        /* pass */
        TrStr nt = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3492 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (nt))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid.", 42LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3492); _tr_str_release(_at_t3492); });
        /* pass */
        TrStr m = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3493 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (m))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = mul i32 ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (nt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3493); _tr_str_release(_at_t3493); });
        /* pass */
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) {
            /* pass */
            TrStr nc = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3494 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (nc))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid.", 44LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3494); _tr_str_release(_at_t3494); });
            /* pass */
            TrStr gs = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3495 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gs))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = mul i32 ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (nc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (nt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3495); _tr_str_release(_at_t3495); });
            /* pass */
            _tr_str_release(axis);
            _tr_str_release(ct);
            _tr_str_release(nt);
            _tr_str_release(m);
            _tr_str_release(nc);
            return GpuEmitter_i32_to_i64(self, gs);
        }
        /* pass */
        TrStr tid = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3496 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tid))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.tid.", 41LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3496); _tr_str_release(_at_t3496); });
        /* pass */
        TrStr g = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3497 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (g))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = add i32 ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (m)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (tid)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3497); _tr_str_release(_at_t3497); });
        /* pass */
        _tr_str_release(axis);
        _tr_str_release(ct);
        _tr_str_release(nt);
        _tr_str_release(m);
        _tr_str_release(tid);
        return GpuEmitter_i32_to_i64(self, g);
    }
    /* pass */
    TrStr mangled = _tr_str_lit_len("@_Z13get_global_idj", 19LL);
    /* pass */
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_id", 12LL)))) {
        /* pass */
        TrStr _strtmp_t3498 = _tr_str_lit_len("@_Z12get_local_idj", 18LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t3498;
    }
    /* pass */
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) {
        /* pass */
        TrStr _strtmp_t3499 = _tr_str_lit_len("@_Z12get_group_idj", 18LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t3499;
    }
    /* pass */
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) {
        /* pass */
        TrStr _strtmp_t3500 = _tr_str_lit_len("@_Z14get_local_sizej", 20LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t3500;
    }
    /* pass */
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) {
        /* pass */
        TrStr _strtmp_t3501 = _tr_str_lit_len("@_Z15get_global_sizej", 21LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t3501;
    }
    /* pass */
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL)))) {
        /* pass */
        TrStr _strtmp_t3502 = _tr_str_lit_len("@_Z14get_num_groupsj", 20LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t3502;
    }
    /* pass */
    TrStr sr = GpuEmitter_fresh(self);
    /* pass */
    ({ TrStr _at_t3503 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (sr))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = call spir_func i64 ", 22LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (mangled)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("(i32 ", 5LL))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(dim)))); TrStr _cres = _tr_strx_concatv(_cl, _cr); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3503); _tr_str_release(_at_t3503); });
    /* pass */
    _tr_str_release(mangled);
    return GVal_make(_tr_str_lit_len("i64", 3LL), sr);
}

__attribute__((hot)) GVal* GpuEmitter_i32_to_i64(GpuEmitter* self, TrStr v) {
    /* pass */
    TrStr r = GpuEmitter_fresh(self);
    /* pass */
    ({ TrStr _at_t3504 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = sext i32 ", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3504); _tr_str_release(_at_t3504); });
    /* pass */
    return GVal_make(_tr_str_lit_len("i64", 3LL), r);
}

__attribute__((hot)) GVal* GpuEmitter_emit_binop(GpuEmitter* self, TrStr op, HirExpr* lp, HirExpr* rp) {
    /* pass */
    GVal* l = GpuEmitter_emit_expr(self, lp);
    /* pass */
    GVal* r = GpuEmitter_emit_expr(self, rp);
    /* pass */
    TrStr ct = GpuEmitter_common_ty(self, l->ty, r->ty);
    /* pass */
    TrStr la = GpuEmitter_coerce(self, l, ct);
    /* pass */
    TrStr ra = GpuEmitter_coerce(self, r, ct);
    /* pass */
    bool isf = _gpu_is_float(ct);
    /* pass */
    TrStr res = GpuEmitter_fresh(self);
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3505 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fadd ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3505); _tr_str_release(_at_t3505); });
        } else {
            /* pass */
            ({ TrStr _at_t3506 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = add ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3506); _tr_str_release(_at_t3506); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3507 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fsub ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3507); _tr_str_release(_at_t3507); });
        } else {
            /* pass */
            ({ TrStr _at_t3508 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = sub ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3508); _tr_str_release(_at_t3508); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3509 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fmul ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3509); _tr_str_release(_at_t3509); });
        } else {
            /* pass */
            ({ TrStr _at_t3510 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = mul ", 7LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3510); _tr_str_release(_at_t3510); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3511 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fdiv ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3511); _tr_str_release(_at_t3511); });
        } else {
            /* pass */
            ({ TrStr _at_t3512 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = sdiv ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3512); _tr_str_release(_at_t3512); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3513 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = frem ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3513); _tr_str_release(_at_t3513); });
        } else {
            /* pass */
            ({ TrStr _at_t3514 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = srem ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3514); _tr_str_release(_at_t3514); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    TrStr pred = GpuEmitter_cmp_pred(self, op, isf);
    /* pass */
    if (_tr_str_eqv((pred), (_tr_str_lit_len("", 0LL)))) {
        /* pass */
        ({ TrStr _at_t3515 = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("unsupported operator '", 22LL)), (op))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("' in kernel", 11LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_fail(self, _at_t3515); _tr_str_release(_at_t3515); });
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(ct);
        _tr_str_release(la);
        _tr_str_release(ra);
        _tr_str_release(res);
        _tr_str_release(pred);
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
    /* pass */
    if (isf) {
        /* pass */
        ({ TrStr _at_t3516 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fcmp ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3516); _tr_str_release(_at_t3516); });
    } else {
        /* pass */
        ({ TrStr _at_t3517 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = icmp ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (pred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3517); _tr_str_release(_at_t3517); });
    }
    /* pass */
    _tr_obj_release(l, _trdrop_GVal);
    _tr_obj_release(r, _trdrop_GVal);
    _tr_str_release(ct);
    _tr_str_release(la);
    _tr_str_release(ra);
    _tr_str_release(pred);
    return GVal_make(_tr_str_lit_len("i1", 2LL), res);
}

__attribute__((hot)) TrStr GpuEmitter_cmp_pred(GpuEmitter* self, TrStr op, bool isf) {
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit_len("oeq", 3LL);
        }
        /* pass */
        return _tr_str_lit_len("eq", 2LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit_len("one", 3LL);
        }
        /* pass */
        return _tr_str_lit_len("ne", 2LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit_len("olt", 3LL);
        }
        /* pass */
        return _tr_str_lit_len("slt", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit_len("ole", 3LL);
        }
        /* pass */
        return _tr_str_lit_len("sle", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit_len("ogt", 3LL);
        }
        /* pass */
        return _tr_str_lit_len("sgt", 3LL);
    }
    /* pass */
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit_len("oge", 3LL);
        }
        /* pass */
        return _tr_str_lit_len("sge", 3LL);
    }
    /* pass */
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr GpuEmitter_common_ty(GpuEmitter* self, TrStr a, TrStr b) {
    /* pass */
    if (_tr_str_eqv((a), (b))) {
        /* pass */
        return _tr_str_retain(a);
    }
    /* pass */
    bool af = _gpu_is_float(a);
    /* pass */
    bool bf = _gpu_is_float(b);
    /* pass */
    if ((af || bf)) {
        /* pass */
        if ((_tr_str_eqv((a), (_tr_str_lit_len("double", 6LL))) || _tr_str_eqv((b), (_tr_str_lit_len("double", 6LL))))) {
            /* pass */
            return _tr_str_lit_len("double", 6LL);
        }
        /* pass */
        return _tr_str_lit_len("float", 5LL);
    }
    /* pass */
    if ((_gpu_iwidth(a) >= _gpu_iwidth(b))) {
        /* pass */
        return _tr_str_retain(a);
    }
    /* pass */
    return _tr_str_retain(b);
}

__attribute__((hot)) TrStr GpuEmitter_coerce(GpuEmitter* self, GVal* v, TrStr target) {
    /* pass */
    if (_tr_str_eqv((v->ty), (target))) {
        /* pass */
        return _tr_str_retain(v->val);
    }
    /* pass */
    TrStr r = GpuEmitter_fresh(self);
    /* pass */
    bool sf = _gpu_is_float(v->ty);
    /* pass */
    bool tf = _gpu_is_float(target);
    /* pass */
    if ((sf && tf)) {
        /* pass */
        if (_tr_str_eqv((target), (_tr_str_lit_len("double", 6LL)))) {
            /* pass */
            ({ TrStr _at_t3518 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fpext float ", 15LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to double\n", 11LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3518); _tr_str_release(_at_t3518); });
        } else {
            /* pass */
            ({ TrStr _at_t3519 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fptrunc double ", 18LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to float\n", 10LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3519); _tr_str_release(_at_t3519); });
        }
        /* pass */
        return r;
    }
    /* pass */
    if ((tf && (!sf))) {
        /* pass */
        ({ TrStr _at_t3520 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = sitofp ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3520); _tr_str_release(_at_t3520); });
        /* pass */
        return r;
    }
    /* pass */
    if ((sf && (!tf))) {
        /* pass */
        ({ TrStr _at_t3521 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fptosi ", 10LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3521); _tr_str_release(_at_t3521); });
        /* pass */
        return r;
    }
    /* pass */
    long long sw = _gpu_iwidth(v->ty);
    /* pass */
    long long tw = _gpu_iwidth(target);
    /* pass */
    if ((tw > sw)) {
        /* pass */
        ({ TrStr _at_t3522 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = sext ", 8LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3522); _tr_str_release(_at_t3522); });
        /* pass */
        return r;
    }
    /* pass */
    if ((tw < sw)) {
        /* pass */
        ({ TrStr _at_t3523 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = trunc ", 9LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3523); _tr_str_release(_at_t3523); });
        /* pass */
        return r;
    }
    /* pass */
    _tr_str_release(r);
    return _tr_str_retain(v->val);
}

__attribute__((hot)) TrStr GpuEmitter_coerce_bool(GpuEmitter* self, GVal* v) {
    /* pass */
    if (_tr_str_eqv((v->ty), (_tr_str_lit_len("i1", 2LL)))) {
        /* pass */
        return _tr_str_retain(v->val);
    }
    /* pass */
    TrStr r = GpuEmitter_fresh(self);
    /* pass */
    if (_gpu_is_float(v->ty)) {
        /* pass */
        ({ TrStr _at_t3524 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = fcmp one ", 12LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", 0.0\n", 6LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3524); _tr_str_release(_at_t3524); });
    } else {
        /* pass */
        ({ TrStr _at_t3525 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" = icmp ne ", 11LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concatv(_cl, (_tr_str_lit_len(", 0\n", 4LL))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3525); _tr_str_release(_at_t3525); });
    }
    /* pass */
    return r;
}

__attribute__((hot)) long long GpuEmitter_literal_int(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3526 = e;
    if (_t3526.tag == HirExpr_ELitInt) {
        __auto_type val = _t3526.data.ELitInt.val;
__auto_type ty = _t3526.data.ELitInt.ty;
        return val;
    } else if (1) {
        __auto_type _ = _t3526;
        return 0LL;
    }
}

__attribute__((hot)) TrStr GpuEmitter_float_lit(GpuEmitter* self, double v) {
    /* pass */
    return ({ TrStr _cr = (_hex16(_gpu_f64_bits(v))); TrStr _cres = _tr_strx_concatv((_tr_str_lit_len("0x", 2LL)), _cr); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) bool fn_is_kernel(HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->decorators->len)) {
        /* pass */
        if (_tr_str_eqv((((Decorator*)List_ptr_get(f->decorators, i))->name), (_tr_str_lit_len("kernel", 6LL)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool fn_is_device(HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->decorators->len)) {
        /* pass */
        if (_tr_str_eqv((((Decorator*)List_ptr_get(f->decorators, i))->name), (_tr_str_lit_len("device", 6LL)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _gpu_is_float(TrStr t) {
    /* pass */
    return (_tr_str_eqv((t), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((t), (_tr_str_lit_len("double", 6LL))));
}

__attribute__((hot)) long long _gpu_iwidth(TrStr t) {
    /* pass */
    if (_tr_str_eqv((t), (_tr_str_lit_len("i1", 2LL)))) {
        /* pass */
        return 1LL;
    }
    /* pass */
    if (_tr_str_eqv((t), (_tr_str_lit_len("i8", 2LL)))) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if (_tr_str_eqv((t), (_tr_str_lit_len("i16", 3LL)))) {
        /* pass */
        return 16LL;
    }
    /* pass */
    if (_tr_str_eqv((t), (_tr_str_lit_len("i32", 3LL)))) {
        /* pass */
        return 32LL;
    }
    /* pass */
    return 64LL;
}

__attribute__((hot)) TrStr _gpu_scalar_ty(TrStr n) {
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) {
        /* pass */
        return _tr_str_lit_len("float", 5LL);
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("double", 6LL))))) {
        /* pass */
        return _tr_str_lit_len("double", 6LL);
    }
    /* pass */
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
        /* pass */
        return _tr_str_lit_len("i1", 2LL);
    }
    /* pass */
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_schar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uchar", 7LL))))) {
        /* pass */
        return _tr_str_lit_len("i8", 2LL);
    }
    /* pass */
    if ((_tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL))))) {
        /* pass */
        return _tr_str_lit_len("i16", 3LL);
    }
    /* pass */
    if (((_tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int", 5LL))))) {
        /* pass */
        return _tr_str_lit_len("i32", 3LL);
    }
    /* pass */
    return _tr_str_lit_len("i64", 3LL);
}

__attribute__((hot)) long long _gpu_f64_bits(double v) {
    /* pass */
    /* unsafe block */
    /* pass */
    double* p = ((double*)_tr_c_calloc((size_t)(1LL), sizeof(double)));
    /* pass */
    (*p = v);
    /* pass */
    long long* ip = ((long long*)(p));
    /* pass */
    return (*ip);
}

__attribute__((hot)) long long _hexdig(long long n) {
    /* pass */
    if ((n < 10LL)) {
        /* pass */
        return (48LL + n);
    }
    /* pass */
    return (65LL + (n - 10LL));
}

__attribute__((hot)) TrStr _hex16(long long v) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(16LL);
    /* pass */
    long long k = 15LL;
    /* pass */
    while ((k >= 0LL)) {
        /* pass */
        long long nib = ((v >> (4LL * k)) & 15LL);
        /* pass */
        StringBuilder_append_char(sb, _hexdig(nib));
        /* pass */
        k = (k - 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

