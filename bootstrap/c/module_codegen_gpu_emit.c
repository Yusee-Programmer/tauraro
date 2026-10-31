#include "tauraro_types.h"


__attribute__((hot)) GVal* GVal_make(TrStr ty, TrStr val) {
#line 61 "src/codegen/gpu/emit.tr"
    GVal* v = ((GVal*)_tr_obj_alloc(sizeof(GVal)));
#line 62 "src/codegen/gpu/emit.tr"
    v->ty = _tr_str_retain(ty);
#line 63 "src/codegen/gpu/emit.tr"
    v->val = _tr_str_retain(val);
#line 64 "src/codegen/gpu/emit.tr"
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) GpuEmitter* GpuEmitter_init(TrStr target) {
#line 81 "src/codegen/gpu/emit.tr"
    GpuEmitter* e = ((GpuEmitter*)_tr_obj_alloc(sizeof(GpuEmitter)));
#line 82 "src/codegen/gpu/emit.tr"
    e->sb = StringBuilder_init(1024LL);
#line 83 "src/codegen/gpu/emit.tr"
    e->target = _tr_str_retain(target);
#line 84 "src/codegen/gpu/emit.tr"
    e->tmp = 0LL;
#line 85 "src/codegen/gpu/emit.tr"
    e->lbl = 0LL;
#line 86 "src/codegen/gpu/emit.tr"
    e->ok = true;
#line 87 "src/codegen/gpu/emit.tr"
    e->fail_note = _tr_str_lit_len("", 0LL);
#line 88 "src/codegen/gpu/emit.tr"
    e->var_names = (void*)List_TrStr_new();
#line 89 "src/codegen/gpu/emit.tr"
    e->var_ll = (void*)List_TrStr_new();
#line 90 "src/codegen/gpu/emit.tr"
    e->var_elem = (void*)List_TrStr_new();
#line 91 "src/codegen/gpu/emit.tr"
    e->ret_llty = _tr_str_lit_len("void", 4LL);
#line 92 "src/codegen/gpu/emit.tr"
    e->dev_fns = (void*)List_ptr_new();
#line 93 "src/codegen/gpu/emit.tr"
    return e;
}

__attribute__((hot)) TrStr GpuEmitter__gpu_ret_ty(GpuEmitter* self, AstType* t) {
#line 97 "src/codegen/gpu/emit.tr"
    if (((_tr_str_eqv((t->name), (_tr_str_lit_len("void", 4LL))) || _tr_str_eqv((t->name), (_tr_str_lit_len("None", 4LL)))) || _tr_str_eqv((t->name), (_tr_str_lit_len("", 0LL))))) {
#line 97 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("void", 4LL);
    }
#line 98 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((t->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 98 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("ptr addrspace(1)", 16LL);
    }
#line 99 "src/codegen/gpu/emit.tr"
    return _gpu_scalar_ty(t->name);
}

__attribute__((hot)) void GpuEmitter_w(GpuEmitter* self, TrStr s) {
#line 102 "src/codegen/gpu/emit.tr"
    StringBuilder_append(self->sb, s);
}

__attribute__((hot)) TrStr GpuEmitter_fresh(GpuEmitter* self) {
#line 105 "src/codegen/gpu/emit.tr"
    TrStr r = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->tmp)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("%t", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 106 "src/codegen/gpu/emit.tr"
    self->tmp = (self->tmp + 1LL);
#line 107 "src/codegen/gpu/emit.tr"
    TrStr _trret1902 = _tr_str_retain(r);
    _tr_str_release(r);
    return _trret1902;
}

__attribute__((hot)) TrStr GpuEmitter_newlbl(GpuEmitter* self) {
#line 110 "src/codegen/gpu/emit.tr"
    TrStr r = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->lbl)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("L", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 111 "src/codegen/gpu/emit.tr"
    self->lbl = (self->lbl + 1LL);
#line 112 "src/codegen/gpu/emit.tr"
    TrStr _trret1903 = _tr_str_retain(r);
    _tr_str_release(r);
    return _trret1903;
}

__attribute__((hot)) void GpuEmitter_fail(GpuEmitter* self, TrStr why) {
#line 115 "src/codegen/gpu/emit.tr"
    self->ok = false;
#line 116 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 116 "src/codegen/gpu/emit.tr"
        self->fail_note = _tr_str_retain(why);
    }
}

__attribute__((hot)) void GpuEmitter_add_var(GpuEmitter* self, TrStr name, AstType* ty) {
#line 120 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 121 "src/codegen/gpu/emit.tr"
    while ((i < self->var_names->len)) {
#line 122 "src/codegen/gpu/emit.tr"
        if (({ TrStr _cmpa_t7314 = (List_TrStr_get(self->var_names, i)); TrStr _cmpb_t7315 = (name); _Bool _cmpr_t7316 = _tr_str_eqv(_cmpa_t7314, _cmpb_t7315); _tr_str_release(_cmpa_t7314); _cmpr_t7316; })) {
#line 122 "src/codegen/gpu/emit.tr"
            return;
        }
#line 123 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
#line 124 "src/codegen/gpu/emit.tr"
    if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (ty->args->len > 0LL))) {
#line 125 "src/codegen/gpu/emit.tr"
        TrStr et = _gpu_scalar_ty((*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
#line 126 "src/codegen/gpu/emit.tr"
        List_TrStr_append(self->var_names, name);
#line 127 "src/codegen/gpu/emit.tr"
        List_TrStr_append(self->var_ll, _tr_str_lit_len("ptr addrspace(1)", 16LL));
#line 128 "src/codegen/gpu/emit.tr"
        List_TrStr_append(self->var_elem, et);
        _tr_str_release(et);
    } else {
#line 130 "src/codegen/gpu/emit.tr"
        List_TrStr_append(self->var_names, name);
#line 131 "src/codegen/gpu/emit.tr"
        ({ TrStr _ord_t7317 = (_gpu_scalar_ty(ty->name)); List_TrStr_append(self->var_ll, _ord_t7317); _tr_str_release(_ord_t7317); });
#line 132 "src/codegen/gpu/emit.tr"
        List_TrStr_append(self->var_elem, _tr_str_lit_len("", 0LL));
    }
}

__attribute__((hot)) TrStr GpuEmitter_var_slot_ty(GpuEmitter* self, TrStr name) {
#line 135 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 136 "src/codegen/gpu/emit.tr"
    while ((i < self->var_names->len)) {
#line 137 "src/codegen/gpu/emit.tr"
        if (({ TrStr _cmpa_t7318 = (List_TrStr_get(self->var_names, i)); TrStr _cmpb_t7319 = (name); _Bool _cmpr_t7320 = _tr_str_eqv(_cmpa_t7318, _cmpb_t7319); _tr_str_release(_cmpa_t7318); _cmpr_t7320; })) {
#line 137 "src/codegen/gpu/emit.tr"
            return List_TrStr_get(self->var_ll, i);
        }
#line 138 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
#line 139 "src/codegen/gpu/emit.tr"
    return _tr_str_lit_len("i64", 3LL);
}

__attribute__((hot)) TrStr GpuEmitter_var_elem_ty(GpuEmitter* self, TrStr name) {
#line 142 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 143 "src/codegen/gpu/emit.tr"
    while ((i < self->var_names->len)) {
#line 144 "src/codegen/gpu/emit.tr"
        if (({ TrStr _cmpa_t7321 = (List_TrStr_get(self->var_names, i)); TrStr _cmpb_t7322 = (name); _Bool _cmpr_t7323 = _tr_str_eqv(_cmpa_t7321, _cmpb_t7322); _tr_str_release(_cmpa_t7321); _cmpr_t7323; })) {
#line 144 "src/codegen/gpu/emit.tr"
            return List_TrStr_get(self->var_elem, i);
        }
#line 145 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
#line 146 "src/codegen/gpu/emit.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void GpuEmitter_scan_vars_block(GpuEmitter* self, HirBlock* b) {
#line 150 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 151 "src/codegen/gpu/emit.tr"
    while ((i < b->stmts->len)) {
#line 152 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_stmt(self, (*((HirStmt*)List_ptr_get(b->stmts, i))));
#line 153 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void GpuEmitter_scan_vars_stmt(GpuEmitter* self, HirStmt s) {
#line 156 "src/codegen/gpu/emit.tr"
    __auto_type _t7324 = s;
    if (_t7324.tag == HirStmt_SLet) {
        __auto_type name = _t7324.data.SLet.name;
__auto_type ownership = _t7324.data.SLet.ownership;
__auto_type is_mut = _t7324.data.SLet.is_mut;
__auto_type is_const = _t7324.data.SLet.is_const;
__auto_type is_shared = _t7324.data.SLet.is_shared;
__auto_type ty = _t7324.data.SLet.ty;
__auto_type val = _t7324.data.SLet.val;
#line 158 "src/codegen/gpu/emit.tr"
        AstType* vt = ty;
#line 159 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((vt->name), (_tr_str_lit_len("", 0LL)))) {
#line 159 "src/codegen/gpu/emit.tr"
            vt = hir_expr_type(val);
        }
#line 160 "src/codegen/gpu/emit.tr"
        GpuEmitter_add_var(self, name, vt);
    } else if (_t7324.tag == HirStmt_SIf) {
        __auto_type cond = _t7324.data.SIf.cond;
__auto_type then_b = _t7324.data.SIf.then_b;
__auto_type else_b = _t7324.data.SIf.else_b;
#line 162 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, then_b);
#line 163 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, else_b);
    } else if (_t7324.tag == HirStmt_SWhile) {
        __auto_type cond = _t7324.data.SWhile.cond;
__auto_type body = _t7324.data.SWhile.body;
#line 165 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, body);
    } else if (_t7324.tag == HirStmt_SUnsafe) {
        __auto_type body = _t7324.data.SUnsafe.body;
#line 167 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, body);
    } else if (1) {
        __auto_type _ = _t7324;
#line 169 "src/codegen/gpu/emit.tr"
        /* pass */
    }
}

__attribute__((hot)) TrStr GpuEmitter_emit_kernel(GpuEmitter* self, HirFunction* f) {
#line 174 "src/codegen/gpu/emit.tr"
    long long pi = 0LL;
#line 175 "src/codegen/gpu/emit.tr"
    while ((pi < f->params->len)) {
#line 176 "src/codegen/gpu/emit.tr"
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
#line 177 "src/codegen/gpu/emit.tr"
        GpuEmitter_add_var(self, p->name, p->ty);
#line 178 "src/codegen/gpu/emit.tr"
        pi = (pi + 1LL);
    }
#line 179 "src/codegen/gpu/emit.tr"
    GpuEmitter_scan_vars_block(self, f->body);
#line 181 "src/codegen/gpu/emit.tr"
    TrStr cc = _tr_str_lit_len("spir_kernel", 11LL);
#line 182 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((self->target), (_tr_str_lit_len("nvptx", 5LL)))) {
#line 182 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t7325 = _tr_str_lit_len("ptx_kernel", 10LL);
        _tr_str_release(cc);
        cc = _strtmp_t7325;
    }
#line 185 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t7326 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (cc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" void @", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7326); _tr_str_release(_at_t7326); });
#line 186 "src/codegen/gpu/emit.tr"
    long long pj = 0LL;
#line 187 "src/codegen/gpu/emit.tr"
    while ((pj < f->params->len)) {
#line 188 "src/codegen/gpu/emit.tr"
        HirParam* pp = ((HirParam*)List_ptr_get(f->params, pj));
#line 189 "src/codegen/gpu/emit.tr"
        if ((pj > 0LL)) {
#line 189 "src/codegen/gpu/emit.tr"
            GpuEmitter_w(self, _tr_str_lit_len(", ", 2LL));
        }
#line 190 "src/codegen/gpu/emit.tr"
        ({ __auto_type _ord_t7327 = (({ TrStr _tr_cl = (GpuEmitter_var_slot_ty(self, pp->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7328 = (({ TrStr _tr_cl = (_ord_t7327); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pp->name)); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7328); _tr_str_release(_at_t7328); });
#line 191 "src/codegen/gpu/emit.tr"
        pj = (pj + 1LL);
    }
#line 192 "src/codegen/gpu/emit.tr"
    GpuEmitter_w(self, _tr_str_lit_len(") {\nentry:\n", 11LL));
#line 195 "src/codegen/gpu/emit.tr"
    long long vi = 0LL;
#line 196 "src/codegen/gpu/emit.tr"
    while ((vi < self->var_names->len)) {
#line 197 "src/codegen/gpu/emit.tr"
        ({ __auto_type _ord_t7329 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(self->var_names, vi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7330 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7329); TrStr _tr_cr = (List_TrStr_get(self->var_ll, vi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7330); _tr_str_release(_at_t7330); });
#line 198 "src/codegen/gpu/emit.tr"
        vi = (vi + 1LL);
    }
#line 199 "src/codegen/gpu/emit.tr"
    long long pk = 0LL;
#line 200 "src/codegen/gpu/emit.tr"
    while ((pk < f->params->len)) {
#line 201 "src/codegen/gpu/emit.tr"
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pk))->name);
#line 202 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7331 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7331); _tr_str_release(_at_t7331); });
#line 203 "src/codegen/gpu/emit.tr"
        pk = (pk + 1LL);
        _tr_str_release(pn);
    }
#line 205 "src/codegen/gpu/emit.tr"
    GpuEmitter_emit_block(self, f->body);
#line 206 "src/codegen/gpu/emit.tr"
    GpuEmitter_w(self, _tr_str_lit_len("  ret void\n}\n\n", 14LL));
#line 207 "src/codegen/gpu/emit.tr"
    if ((!self->ok)) {
#line 208 "src/codegen/gpu/emit.tr"
        TrStr _trret1904 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("; kernel '", 10LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' NOT emitted: ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->fail_note)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(cc);
        return _trret1904;
    }
#line 209 "src/codegen/gpu/emit.tr"
    TrStr _trret1905 = StringObj_as_str(StringBuilder_to_string(self->sb));
    _tr_str_release(cc);
    return _trret1905;
}

__attribute__((hot)) TrStr GpuEmitter_emit_device_fn(GpuEmitter* self, HirFunction* f) {
#line 214 "src/codegen/gpu/emit.tr"
    self->ret_llty = GpuEmitter__gpu_ret_ty(self, f->ret_ty);
#line 215 "src/codegen/gpu/emit.tr"
    long long pi = 0LL;
#line 216 "src/codegen/gpu/emit.tr"
    while ((pi < f->params->len)) {
#line 217 "src/codegen/gpu/emit.tr"
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
#line 218 "src/codegen/gpu/emit.tr"
        GpuEmitter_add_var(self, p->name, p->ty);
#line 219 "src/codegen/gpu/emit.tr"
        pi = (pi + 1LL);
    }
#line 220 "src/codegen/gpu/emit.tr"
    GpuEmitter_scan_vars_block(self, f->body);
#line 222 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t7332 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7332); _tr_str_release(_at_t7332); });
#line 223 "src/codegen/gpu/emit.tr"
    long long pj = 0LL;
#line 224 "src/codegen/gpu/emit.tr"
    while ((pj < f->params->len)) {
#line 225 "src/codegen/gpu/emit.tr"
        HirParam* pp = ((HirParam*)List_ptr_get(f->params, pj));
#line 226 "src/codegen/gpu/emit.tr"
        if ((pj > 0LL)) {
#line 226 "src/codegen/gpu/emit.tr"
            GpuEmitter_w(self, _tr_str_lit_len(", ", 2LL));
        }
#line 227 "src/codegen/gpu/emit.tr"
        ({ __auto_type _ord_t7333 = (({ TrStr _tr_cl = (GpuEmitter_var_slot_ty(self, pp->name)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7334 = (({ TrStr _tr_cl = (_ord_t7333); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pp->name)); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7334); _tr_str_release(_at_t7334); });
#line 228 "src/codegen/gpu/emit.tr"
        pj = (pj + 1LL);
    }
#line 229 "src/codegen/gpu/emit.tr"
    GpuEmitter_w(self, _tr_str_lit_len(") {\nentry:\n", 11LL));
#line 230 "src/codegen/gpu/emit.tr"
    long long vi = 0LL;
#line 231 "src/codegen/gpu/emit.tr"
    while ((vi < self->var_names->len)) {
#line 232 "src/codegen/gpu/emit.tr"
        ({ __auto_type _ord_t7335 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(self->var_names, vi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t7336 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t7335); TrStr _tr_cr = (List_TrStr_get(self->var_ll, vi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7336); _tr_str_release(_at_t7336); });
#line 233 "src/codegen/gpu/emit.tr"
        vi = (vi + 1LL);
    }
#line 234 "src/codegen/gpu/emit.tr"
    long long pk = 0LL;
#line 235 "src/codegen/gpu/emit.tr"
    while ((pk < f->params->len)) {
#line 236 "src/codegen/gpu/emit.tr"
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pk))->name);
#line 237 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7337 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7337); _tr_str_release(_at_t7337); });
#line 238 "src/codegen/gpu/emit.tr"
        pk = (pk + 1LL);
        _tr_str_release(pn);
    }
#line 239 "src/codegen/gpu/emit.tr"
    GpuEmitter_emit_block(self, f->body);
#line 241 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("void", 4LL)))) {
#line 242 "src/codegen/gpu/emit.tr"
        GpuEmitter_w(self, _tr_str_lit_len("  ret void\n}\n\n", 14LL));
    } else if (_gpu_is_float(self->ret_llty)) {
#line 244 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7338 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 0.0\n}\n\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7338); _tr_str_release(_at_t7338); });
    } else if (_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("ptr addrspace(1)", 16LL)))) {
#line 246 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7339 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" null\n}\n\n", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7339); _tr_str_release(_at_t7339); });
    } else {
#line 248 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7340 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 0\n}\n\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7340); _tr_str_release(_at_t7340); });
    }
#line 249 "src/codegen/gpu/emit.tr"
    if ((!self->ok)) {
#line 250 "src/codegen/gpu/emit.tr"
        return ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("; device fn '", 13LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' NOT emitted: ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->fail_note)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
    }
#line 251 "src/codegen/gpu/emit.tr"
    return StringObj_as_str(StringBuilder_to_string(self->sb));
}

__attribute__((hot)) void GpuEmitter_emit_block(GpuEmitter* self, HirBlock* b) {
#line 254 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 255 "src/codegen/gpu/emit.tr"
    while ((i < b->stmts->len)) {
#line 256 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_stmt(self, (*((HirStmt*)List_ptr_get(b->stmts, i))));
#line 257 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
}

__attribute__((hot)) void GpuEmitter_emit_stmt(GpuEmitter* self, HirStmt s) {
#line 260 "src/codegen/gpu/emit.tr"
    __auto_type _t7341 = s;
    if (_t7341.tag == HirStmt_SLet) {
        __auto_type name = _t7341.data.SLet.name;
__auto_type ownership = _t7341.data.SLet.ownership;
__auto_type is_mut = _t7341.data.SLet.is_mut;
__auto_type is_const = _t7341.data.SLet.is_const;
__auto_type is_shared = _t7341.data.SLet.is_shared;
__auto_type ty = _t7341.data.SLet.ty;
__auto_type val = _t7341.data.SLet.val;
#line 262 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, val);
#line 263 "src/codegen/gpu/emit.tr"
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
#line 264 "src/codegen/gpu/emit.tr"
        TrStr cv = GpuEmitter_coerce(self, v, slot);
#line 265 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7342 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (slot))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7342); _tr_str_release(_at_t7342); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (_t7341.tag == HirStmt_SAssign) {
        __auto_type target = _t7341.data.SAssign.target;
__auto_type val = _t7341.data.SAssign.val;
#line 267 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_assign(self, (*target), val);
    } else if (_t7341.tag == HirStmt_SExpr) {
        __auto_type expr = _t7341.data.SExpr.expr;
#line 269 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_stmt_expr(self, (*expr));
    } else if (_t7341.tag == HirStmt_SIf) {
        __auto_type cond = _t7341.data.SIf.cond;
__auto_type then_b = _t7341.data.SIf.then_b;
__auto_type else_b = _t7341.data.SIf.else_b;
#line 271 "src/codegen/gpu/emit.tr"
        GVal* c = GpuEmitter_emit_expr(self, cond);
#line 272 "src/codegen/gpu/emit.tr"
        TrStr cc = GpuEmitter_coerce_bool(self, c);
#line 273 "src/codegen/gpu/emit.tr"
        TrStr lt = GpuEmitter_newlbl(self);
#line 274 "src/codegen/gpu/emit.tr"
        TrStr le = GpuEmitter_newlbl(self);
#line 275 "src/codegen/gpu/emit.tr"
        TrStr lend = GpuEmitter_newlbl(self);
#line 276 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7343 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (cc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (le)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7343); _tr_str_release(_at_t7343); });
#line 277 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7344 = (_tr_strx_concatv((lt), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t7344); _tr_str_release(_at_t7344); });
#line 278 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, then_b);
#line 279 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7345 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lend))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7345); _tr_str_release(_at_t7345); });
#line 280 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7346 = (_tr_strx_concatv((le), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t7346); _tr_str_release(_at_t7346); });
#line 281 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, else_b);
#line 282 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7347 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lend))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7347); _tr_str_release(_at_t7347); });
#line 283 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7348 = (_tr_strx_concatv((lend), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t7348); _tr_str_release(_at_t7348); });
        _tr_obj_release(c, _trdrop_GVal);
        _tr_str_release(cc);
        _tr_str_release(lt);
        _tr_str_release(le);
        _tr_str_release(lend);
    } else if (_t7341.tag == HirStmt_SWhile) {
        __auto_type cond = _t7341.data.SWhile.cond;
__auto_type body = _t7341.data.SWhile.body;
#line 285 "src/codegen/gpu/emit.tr"
        TrStr lc = GpuEmitter_newlbl(self);
#line 286 "src/codegen/gpu/emit.tr"
        TrStr lb = GpuEmitter_newlbl(self);
#line 287 "src/codegen/gpu/emit.tr"
        TrStr lend2 = GpuEmitter_newlbl(self);
#line 288 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7349 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7349); _tr_str_release(_at_t7349); });
#line 289 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7350 = (_tr_strx_concatv((lc), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t7350); _tr_str_release(_at_t7350); });
#line 290 "src/codegen/gpu/emit.tr"
        GVal* cnd = GpuEmitter_emit_expr(self, cond);
#line 291 "src/codegen/gpu/emit.tr"
        TrStr cb = GpuEmitter_coerce_bool(self, cnd);
#line 292 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7351 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (cb))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lend2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7351); _tr_str_release(_at_t7351); });
#line 293 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7352 = (_tr_strx_concatv((lb), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t7352); _tr_str_release(_at_t7352); });
#line 294 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, body);
#line 295 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7353 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7353); _tr_str_release(_at_t7353); });
#line 296 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7354 = (_tr_strx_concatv((lend2), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t7354); _tr_str_release(_at_t7354); });
        _tr_str_release(lc);
        _tr_str_release(lb);
        _tr_str_release(lend2);
        _tr_obj_release(cnd, _trdrop_GVal);
        _tr_str_release(cb);
    } else if (_t7341.tag == HirStmt_SUnsafe) {
        __auto_type body = _t7341.data.SUnsafe.body;
#line 298 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, body);
    } else if (_t7341.tag == HirStmt_SReturn) {
        __auto_type val = _t7341.data.SReturn.val;
#line 300 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("void", 4LL))) || (((unsigned long long)(val)) == ((unsigned long long)(0LL))))) {
#line 301 "src/codegen/gpu/emit.tr"
            GpuEmitter_w(self, _tr_str_lit_len("  ret void\n", 11LL));
        } else {
#line 303 "src/codegen/gpu/emit.tr"
            GVal* rv = GpuEmitter_emit_expr(self, val);
#line 304 "src/codegen/gpu/emit.tr"
            if (_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("ptr addrspace(1)", 16LL)))) {
#line 305 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t7355 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ptr addrspace(1) ", 23LL)), (rv->val))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7355); _tr_str_release(_at_t7355); });
            } else {
#line 307 "src/codegen/gpu/emit.tr"
                TrStr cv = GpuEmitter_coerce(self, rv, self->ret_llty);
#line 308 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t7356 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7356); _tr_str_release(_at_t7356); });
            }
        }
#line 310 "src/codegen/gpu/emit.tr"
        TrStr lu = GpuEmitter_newlbl(self);
#line 311 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7357 = (_tr_strx_concatv((lu), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t7357); _tr_str_release(_at_t7357); });
        _tr_str_release(lu);
    } else if (_t7341.tag == HirStmt_SPass) {
#line 313 "src/codegen/gpu/emit.tr"
        /* pass */
    } else if (_t7341.tag == HirStmt_SLineMarker) {
        __auto_type n = _t7341.data.SLineMarker.n;
#line 315 "src/codegen/gpu/emit.tr"
        /* pass */
    } else if (1) {
        __auto_type _ = _t7341;
#line 317 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported statement in kernel", 31LL));
    }
}

__attribute__((hot)) void GpuEmitter_emit_assign(GpuEmitter* self, HirExpr target, HirExpr* val) {
#line 321 "src/codegen/gpu/emit.tr"
    __auto_type _t7358 = target;
    if (_t7358.tag == HirExpr_EIdent) {
        __auto_type name = _t7358.data.EIdent.name;
__auto_type ty = _t7358.data.EIdent.ty;
__auto_type is_move = _t7358.data.EIdent.is_move;
#line 323 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, val);
#line 324 "src/codegen/gpu/emit.tr"
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
#line 325 "src/codegen/gpu/emit.tr"
        TrStr cv = GpuEmitter_coerce(self, v, slot);
#line 326 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7359 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (slot))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7359); _tr_str_release(_at_t7359); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t7358;
#line 328 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported assignment target in kernel", 39LL));
    }
}

__attribute__((hot)) void GpuEmitter_emit_stmt_expr(GpuEmitter* self, HirExpr e) {
#line 332 "src/codegen/gpu/emit.tr"
    __auto_type _t7360 = e;
    if (_t7360.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t7360.data.EMethodCall.obj;
__auto_type method = _t7360.data.EMethodCall.method;
__auto_type args = _t7360.data.EMethodCall.args;
__auto_type ty = _t7360.data.EMethodCall.ty;
#line 334 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("write", 5LL))) && (args->len == 1LL))) {
#line 335 "src/codegen/gpu/emit.tr"
            ({ __auto_type _ord_t7361 = ((*obj)); __auto_type _ord_t7362 = (((HirExpr*)List_ptr_get(args, 0LL))); GpuEmitter_emit_store(self, _ord_t7361, _ord_t7362); });
#line 336 "src/codegen/gpu/emit.tr"
            return;
        }
#line 338 "src/codegen/gpu/emit.tr"
        GVal* _v = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v, _trdrop_GVal);
    } else if (_t7360.tag == HirExpr_ECall) {
        __auto_type callee = _t7360.data.ECall.callee;
__auto_type args = _t7360.data.ECall.args;
__auto_type ty = _t7360.data.ECall.ty;
#line 340 "src/codegen/gpu/emit.tr"
        if (({ TrStr _cmpa_t7363 = (GpuEmitter_callee_name(self, (*callee))); TrStr _cmpb_t7364 = (_tr_str_lit_len("gpu_barrier", 11LL)); _Bool _cmpr_t7365 = _tr_str_eqv(_cmpa_t7363, _cmpb_t7364); _tr_str_release(_cmpa_t7363); _cmpr_t7365; })) {
#line 341 "src/codegen/gpu/emit.tr"
            if (_tr_str_eqv((self->target), (_tr_str_lit_len("nvptx", 5LL)))) {
#line 341 "src/codegen/gpu/emit.tr"
                GpuEmitter_w(self, _tr_str_lit_len("  call void @llvm.nvvm.barrier0()\n", 34LL));
            } else {
#line 342 "src/codegen/gpu/emit.tr"
                GpuEmitter_w(self, _tr_str_lit_len("  call spir_func void @_Z7barrierj(i32 1)\n", 42LL));
            }
#line 343 "src/codegen/gpu/emit.tr"
            return;
        }
#line 344 "src/codegen/gpu/emit.tr"
        GVal* _v2 = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v2, _trdrop_GVal);
    } else if (1) {
        __auto_type _ = _t7360;
#line 346 "src/codegen/gpu/emit.tr"
        GVal* _v3 = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v3, _trdrop_GVal);
    }
}

__attribute__((hot)) void GpuEmitter_emit_store(GpuEmitter* self, HirExpr chain, HirExpr* valp) {
#line 351 "src/codegen/gpu/emit.tr"
    __auto_type _t7366 = chain;
    if (_t7366.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t7366.data.EMethodCall.obj;
__auto_type pmeth = _t7366.data.EMethodCall.method;
__auto_type pargs = _t7366.data.EMethodCall.args;
__auto_type pty = _t7366.data.EMethodCall.ty;
#line 353 "src/codegen/gpu/emit.tr"
        if (((!_tr_str_eqv((pmeth), (_tr_str_lit_len("offset", 6LL)))) || (pargs->len != 1LL))) {
#line 354 "src/codegen/gpu/emit.tr"
            GpuEmitter_fail(self, _tr_str_lit_len("store target must be p.offset(i).write(v)", 41LL));
#line 355 "src/codegen/gpu/emit.tr"
            return;
        }
#line 356 "src/codegen/gpu/emit.tr"
        GVal* pr = GpuEmitter_emit_ptr(self, (*pobj));
#line 357 "src/codegen/gpu/emit.tr"
        GVal* idx = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(pargs, 0LL)));
#line 358 "src/codegen/gpu/emit.tr"
        TrStr i64idx = GpuEmitter_coerce(self, idx, _tr_str_lit_len("i64", 3LL));
#line 359 "src/codegen/gpu/emit.tr"
        TrStr gep = GpuEmitter_fresh(self);
#line 360 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7367 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (i64idx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7367); _tr_str_release(_at_t7367); });
#line 361 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, valp);
#line 362 "src/codegen/gpu/emit.tr"
        TrStr cv = GpuEmitter_coerce(self, v, pr->ty);
#line 363 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7368 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (pr->ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7368); _tr_str_release(_at_t7368); });
        _tr_obj_release(pr, _trdrop_GVal);
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t7366;
#line 365 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("store target must be p.offset(i).write(v)", 41LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_ptr(GpuEmitter* self, HirExpr e) {
#line 370 "src/codegen/gpu/emit.tr"
    __auto_type _t7369 = e;
    if (_t7369.tag == HirExpr_EIdent) {
        __auto_type name = _t7369.data.EIdent.name;
__auto_type ty = _t7369.data.EIdent.ty;
__auto_type is_move = _t7369.data.EIdent.is_move;
#line 372 "src/codegen/gpu/emit.tr"
        TrStr et = GpuEmitter_var_elem_ty(self, name);
#line 373 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((et), (_tr_str_lit_len("", 0LL)))) {
#line 373 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t7370 = _tr_str_lit_len("float", 5LL);
            _tr_str_release(et);
            et = _strtmp_t7370;
        }
#line 374 "src/codegen/gpu/emit.tr"
        TrStr r = GpuEmitter_fresh(self);
#line 375 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7371 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ptr addrspace(1), ptr %var_", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7371); _tr_str_release(_at_t7371); });
#line 376 "src/codegen/gpu/emit.tr"
        return GVal_make(et, r);
    } else if (_t7369.tag == HirExpr_ECast) {
        __auto_type inner = _t7369.data.ECast.expr;
__auto_type target_ty = _t7369.data.ECast.target_ty;
#line 378 "src/codegen/gpu/emit.tr"
        GVal* g = GpuEmitter_emit_ptr(self, (*inner));
#line 379 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((target_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (target_ty->args->len > 0LL))) {
#line 380 "src/codegen/gpu/emit.tr"
            g->ty = _gpu_scalar_ty((*((AstType**)List_ptr_get(target_ty->args, 0LL)))->name);
        }
#line 381 "src/codegen/gpu/emit.tr"
        return g;
    } else if (1) {
        __auto_type _ = _t7369;
#line 383 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported pointer expression in kernel", 40LL));
#line 384 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("float", 5LL), _tr_str_lit_len("null", 4LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_expr(GpuEmitter* self, HirExpr* ep) {
#line 388 "src/codegen/gpu/emit.tr"
    return GpuEmitter_emit_expr_hir(self, (*ep));
}

__attribute__((hot)) GVal* GpuEmitter_emit_expr_hir(GpuEmitter* self, HirExpr e) {
#line 391 "src/codegen/gpu/emit.tr"
    __auto_type _t7372 = e;
    if (_t7372.tag == HirExpr_ELitInt) {
        __auto_type val = _t7372.data.ELitInt.val;
__auto_type ty = _t7372.data.ELitInt.ty;
#line 393 "src/codegen/gpu/emit.tr"
        TrStr t = _gpu_scalar_ty(ty->name);
#line 394 "src/codegen/gpu/emit.tr"
        if (_gpu_is_float(t)) {
#line 394 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t7373 = _tr_str_lit_len("i64", 3LL);
            _tr_str_release(t);
            t = _strtmp_t7373;
        }
#line 395 "src/codegen/gpu/emit.tr"
        return ({ TrStr _at_t7374 = (_tr_str_wrap(_tr_int_to_str((long long)(val)))); __auto_type _wr = (GVal_make(t, _at_t7374)); _tr_str_release(_at_t7374); _wr; });
    } else if (_t7372.tag == HirExpr_ELitFloat) {
        __auto_type val = _t7372.data.ELitFloat.val;
__auto_type ty = _t7372.data.ELitFloat.ty;
#line 397 "src/codegen/gpu/emit.tr"
        TrStr ft = _gpu_scalar_ty(ty->name);
#line 398 "src/codegen/gpu/emit.tr"
        if ((!_gpu_is_float(ft))) {
#line 398 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t7375 = _tr_str_lit_len("double", 6LL);
            _tr_str_release(ft);
            ft = _strtmp_t7375;
        }
#line 399 "src/codegen/gpu/emit.tr"
        return ({ TrStr _at_t7376 = (GpuEmitter_float_lit(self, val)); __auto_type _wr = (GVal_make(ft, _at_t7376)); _tr_str_release(_at_t7376); _wr; });
    } else if (_t7372.tag == HirExpr_ELitBool) {
        __auto_type val = _t7372.data.ELitBool.val;
__auto_type ty = _t7372.data.ELitBool.ty;
#line 401 "src/codegen/gpu/emit.tr"
        if (val) {
#line 401 "src/codegen/gpu/emit.tr"
            return GVal_make(_tr_str_lit_len("i1", 2LL), _tr_str_lit_len("1", 1LL));
        }
#line 402 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i1", 2LL), _tr_str_lit_len("0", 1LL));
    } else if (_t7372.tag == HirExpr_EIdent) {
        __auto_type name = _t7372.data.EIdent.name;
__auto_type ty = _t7372.data.EIdent.ty;
__auto_type is_move = _t7372.data.EIdent.is_move;
#line 404 "src/codegen/gpu/emit.tr"
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
#line 405 "src/codegen/gpu/emit.tr"
        TrStr r = GpuEmitter_fresh(self);
#line 406 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7377 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (slot)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7377); _tr_str_release(_at_t7377); });
#line 407 "src/codegen/gpu/emit.tr"
        return GVal_make(slot, r);
    } else if (_t7372.tag == HirExpr_EBinOp) {
        __auto_type op = _t7372.data.EBinOp.op;
__auto_type left = _t7372.data.EBinOp.left;
__auto_type right = _t7372.data.EBinOp.right;
__auto_type ty = _t7372.data.EBinOp.ty;
#line 409 "src/codegen/gpu/emit.tr"
        return GpuEmitter_emit_binop(self, op, left, right);
    } else if (_t7372.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t7372.data.EUnaryOp.op;
__auto_type expr = _t7372.data.EUnaryOp.expr;
__auto_type ty = _t7372.data.EUnaryOp.ty;
#line 411 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, expr);
#line 412 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 413 "src/codegen/gpu/emit.tr"
            TrStr r = GpuEmitter_fresh(self);
#line 414 "src/codegen/gpu/emit.tr"
            if (_gpu_is_float(v->ty)) {
#line 414 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t7378 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fneg ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7378); _tr_str_release(_at_t7378); });
            } else {
#line 415 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t7379 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sub ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 0, ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7379); _tr_str_release(_at_t7379); });
            }
#line 416 "src/codegen/gpu/emit.tr"
            return GVal_make(v->ty, r);
        }
#line 417 "src/codegen/gpu/emit.tr"
        return v;
    } else if (_t7372.tag == HirExpr_ECast) {
        __auto_type inner = _t7372.data.ECast.expr;
__auto_type target_ty = _t7372.data.ECast.target_ty;
#line 419 "src/codegen/gpu/emit.tr"
        GVal* iv = GpuEmitter_emit_expr(self, inner);
#line 420 "src/codegen/gpu/emit.tr"
        TrStr tt = _gpu_scalar_ty(target_ty->name);
#line 421 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((target_ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 421 "src/codegen/gpu/emit.tr"
            _tr_str_release(tt);
            return iv;
        }
#line 422 "src/codegen/gpu/emit.tr"
        return ({ TrStr _at_t7380 = (GpuEmitter_coerce(self, iv, tt)); __auto_type _wr = (GVal_make(tt, _at_t7380)); _tr_str_release(_at_t7380); _wr; });
    } else if (_t7372.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t7372.data.EMethodCall.obj;
__auto_type method = _t7372.data.EMethodCall.method;
__auto_type args = _t7372.data.EMethodCall.args;
__auto_type ty = _t7372.data.EMethodCall.ty;
#line 424 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) && (args->len == 0LL))) {
#line 425 "src/codegen/gpu/emit.tr"
            return GpuEmitter_emit_load(self, (*obj));
        }
#line 426 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7381 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unsupported method '", 20LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in kernel", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_fail(self, _at_t7381); _tr_str_release(_at_t7381); });
#line 427 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    } else if (_t7372.tag == HirExpr_ECall) {
        __auto_type callee = _t7372.data.ECall.callee;
__auto_type args = _t7372.data.ECall.args;
__auto_type ty = _t7372.data.ECall.ty;
#line 429 "src/codegen/gpu/emit.tr"
        return GpuEmitter_emit_call(self, (*callee), args);
    } else if (1) {
        __auto_type _ = _t7372;
#line 431 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported expression in kernel", 32LL));
#line 432 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_load(GpuEmitter* self, HirExpr chain) {
#line 436 "src/codegen/gpu/emit.tr"
    __auto_type _t7382 = chain;
    if (_t7382.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t7382.data.EMethodCall.obj;
__auto_type pmeth = _t7382.data.EMethodCall.method;
__auto_type pargs = _t7382.data.EMethodCall.args;
__auto_type pty = _t7382.data.EMethodCall.ty;
#line 438 "src/codegen/gpu/emit.tr"
        if (((!_tr_str_eqv((pmeth), (_tr_str_lit_len("offset", 6LL)))) || (pargs->len != 1LL))) {
#line 439 "src/codegen/gpu/emit.tr"
            GpuEmitter_fail(self, _tr_str_lit_len("load must be p.offset(i).read()", 31LL));
#line 440 "src/codegen/gpu/emit.tr"
            return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
        }
#line 441 "src/codegen/gpu/emit.tr"
        GVal* pr = GpuEmitter_emit_ptr(self, (*pobj));
#line 442 "src/codegen/gpu/emit.tr"
        GVal* idx = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(pargs, 0LL)));
#line 443 "src/codegen/gpu/emit.tr"
        TrStr i64idx = GpuEmitter_coerce(self, idx, _tr_str_lit_len("i64", 3LL));
#line 444 "src/codegen/gpu/emit.tr"
        TrStr gep = GpuEmitter_fresh(self);
#line 445 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7383 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (i64idx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7383); _tr_str_release(_at_t7383); });
#line 446 "src/codegen/gpu/emit.tr"
        TrStr r = GpuEmitter_fresh(self);
#line 447 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7384 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7384); _tr_str_release(_at_t7384); });
#line 448 "src/codegen/gpu/emit.tr"
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        return GVal_make(pr->ty, r);
    } else if (1) {
        __auto_type _ = _t7382;
#line 450 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("load must be p.offset(i).read()", 31LL));
#line 451 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
}

__attribute__((hot)) TrStr GpuEmitter_callee_name(GpuEmitter* self, HirExpr c) {
#line 454 "src/codegen/gpu/emit.tr"
    __auto_type _t7385 = c;
    if (_t7385.tag == HirExpr_EIdent) {
        __auto_type name = _t7385.data.EIdent.name;
__auto_type ty = _t7385.data.EIdent.ty;
__auto_type is_move = _t7385.data.EIdent.is_move;
        TrStr _trret1906 = _tr_str_retain(name);
        return _trret1906;
    } else if (1) {
        __auto_type _ = _t7385;
        return _tr_str_lit_len("", 0LL);
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_call(GpuEmitter* self, HirExpr callee, List_ptr* args) {
#line 460 "src/codegen/gpu/emit.tr"
    TrStr nm = GpuEmitter_callee_name(self, callee);
#line 461 "src/codegen/gpu/emit.tr"
    if ((((((_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_id", 13LL))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_id", 12LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) || _tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL))))) {
#line 462 "src/codegen/gpu/emit.tr"
        long long dim = 0LL;
#line 463 "src/codegen/gpu/emit.tr"
        if ((args->len == 1LL)) {
#line 463 "src/codegen/gpu/emit.tr"
            dim = GpuEmitter_literal_int(self, (*((HirExpr*)List_ptr_get(args, 0LL))));
        }
#line 464 "src/codegen/gpu/emit.tr"
        return GpuEmitter_emit_gpu_builtin(self, nm, dim);
    }
#line 467 "src/codegen/gpu/emit.tr"
    long long di = 0LL;
#line 468 "src/codegen/gpu/emit.tr"
    while ((di < self->dev_fns->len)) {
#line 469 "src/codegen/gpu/emit.tr"
        HirFunction* df = ((HirFunction*)List_ptr_get(self->dev_fns, di));
#line 470 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((df->name), (nm))) {
#line 471 "src/codegen/gpu/emit.tr"
            TrStr rty = GpuEmitter__gpu_ret_ty(self, df->ret_ty);
#line 472 "src/codegen/gpu/emit.tr"
            TrStr argstr = _tr_str_lit_len("", 0LL);
#line 473 "src/codegen/gpu/emit.tr"
            long long ai = 0LL;
#line 474 "src/codegen/gpu/emit.tr"
            while ((ai < args->len)) {
#line 475 "src/codegen/gpu/emit.tr"
                if ((ai > 0LL)) {
#line 475 "src/codegen/gpu/emit.tr"
                    ({ TrStr _ap_t7386 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&argstr, _ap_t7386); });
                }
#line 476 "src/codegen/gpu/emit.tr"
                GVal* av = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(args, ai)));
#line 477 "src/codegen/gpu/emit.tr"
                TrStr aty = _tr_str_retain(av->ty);
#line 478 "src/codegen/gpu/emit.tr"
                TrStr aval = _tr_str_retain(av->val);
#line 479 "src/codegen/gpu/emit.tr"
                if ((ai < df->params->len)) {
#line 480 "src/codegen/gpu/emit.tr"
                    TrStr ptt = GpuEmitter__gpu_ret_ty(self, ((HirParam*)List_ptr_get(df->params, ai))->ty);
#line 481 "src/codegen/gpu/emit.tr"
                    if (((!_tr_str_eqv((ptt), (_tr_str_lit_len("ptr addrspace(1)", 16LL)))) && (!_tr_str_eqv((aty), (ptt))))) {
#line 482 "src/codegen/gpu/emit.tr"
                        TrStr _strtmp_t7387 = GpuEmitter_coerce(self, av, ptt);
                        _tr_str_release(aval);
                        aval = _strtmp_t7387;
#line 483 "src/codegen/gpu/emit.tr"
                        TrStr _strtmp_t7388 = _tr_str_retain(ptt);
                        _tr_str_release(aty);
                        aty = _strtmp_t7388;
                    }
                }
#line 484 "src/codegen/gpu/emit.tr"
                ({ TrStr _ap_t7389 = (aty); TrStr _ap_t7390 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t7391 = (aval); _tr_str_append(&argstr, _ap_t7389); _tr_str_append(&argstr, _ap_t7390); _tr_str_append(&argstr, _ap_t7391); });
#line 485 "src/codegen/gpu/emit.tr"
                ai = (ai + 1LL);
                _tr_obj_release(av, _trdrop_GVal);
                _tr_str_release(aty);
                _tr_str_release(aval);
            }
#line 486 "src/codegen/gpu/emit.tr"
            if (_tr_str_eqv((rty), (_tr_str_lit_len("void", 4LL)))) {
#line 487 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t7392 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  call void @", 13LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (argstr)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7392); _tr_str_release(_at_t7392); });
#line 488 "src/codegen/gpu/emit.tr"
                _tr_str_release(nm);
                _tr_str_release(rty);
                _tr_str_release(argstr);
                return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
            }
#line 489 "src/codegen/gpu/emit.tr"
            TrStr r = GpuEmitter_fresh(self);
#line 490 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7393 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (argstr)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7393); _tr_str_release(_at_t7393); });
#line 491 "src/codegen/gpu/emit.tr"
            _tr_str_release(nm);
            _tr_str_release(argstr);
            return GVal_make(rty, r);
        }
#line 492 "src/codegen/gpu/emit.tr"
        di = (di + 1LL);
    }
#line 493 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t7394 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("call to '", 9LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' not allowed in kernel (mark it @device, or use a GPU builtin)", 63LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_fail(self, _at_t7394); _tr_str_release(_at_t7394); });
#line 494 "src/codegen/gpu/emit.tr"
    _tr_str_release(nm);
    return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
}

__attribute__((hot)) GVal* GpuEmitter_emit_gpu_builtin(GpuEmitter* self, TrStr nm, long long dim) {
#line 497 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((self->target), (_tr_str_lit_len("nvptx", 5LL)))) {
#line 498 "src/codegen/gpu/emit.tr"
        TrStr axis = _tr_str_lit_len("x", 1LL);
#line 499 "src/codegen/gpu/emit.tr"
        if ((dim == 1LL)) {
#line 499 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t7395 = _tr_str_lit_len("y", 1LL);
            _tr_str_release(axis);
            axis = _strtmp_t7395;
        }
#line 500 "src/codegen/gpu/emit.tr"
        if ((dim == 2LL)) {
#line 500 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t7396 = _tr_str_lit_len("z", 1LL);
            _tr_str_release(axis);
            axis = _strtmp_t7396;
        }
#line 501 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_id", 12LL)))) {
#line 502 "src/codegen/gpu/emit.tr"
            TrStr r = GpuEmitter_fresh(self);
#line 503 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7397 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.tid.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7397); _tr_str_release(_at_t7397); });
#line 504 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r);
        }
#line 505 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) {
#line 506 "src/codegen/gpu/emit.tr"
            TrStr r2 = GpuEmitter_fresh(self);
#line 507 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7398 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7398); _tr_str_release(_at_t7398); });
#line 508 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r2);
        }
#line 509 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) {
#line 510 "src/codegen/gpu/emit.tr"
            TrStr r3 = GpuEmitter_fresh(self);
#line 511 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7399 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r3))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid.", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7399); _tr_str_release(_at_t7399); });
#line 512 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r3);
        }
#line 513 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL)))) {
#line 514 "src/codegen/gpu/emit.tr"
            TrStr r4 = GpuEmitter_fresh(self);
#line 515 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7400 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r4))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid.", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7400); _tr_str_release(_at_t7400); });
#line 516 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r4);
        }
#line 518 "src/codegen/gpu/emit.tr"
        TrStr ct = GpuEmitter_fresh(self);
#line 518 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7401 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (ct))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7401); _tr_str_release(_at_t7401); });
#line 519 "src/codegen/gpu/emit.tr"
        TrStr nt = GpuEmitter_fresh(self);
#line 519 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7402 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (nt))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid.", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7402); _tr_str_release(_at_t7402); });
#line 520 "src/codegen/gpu/emit.tr"
        TrStr m = GpuEmitter_fresh(self);
#line 520 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7403 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = mul i32 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7403); _tr_str_release(_at_t7403); });
#line 521 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) {
#line 522 "src/codegen/gpu/emit.tr"
            TrStr nc = GpuEmitter_fresh(self);
#line 522 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7404 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (nc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid.", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7404); _tr_str_release(_at_t7404); });
#line 523 "src/codegen/gpu/emit.tr"
            TrStr gs = GpuEmitter_fresh(self);
#line 523 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7405 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gs))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = mul i32 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7405); _tr_str_release(_at_t7405); });
#line 524 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            _tr_str_release(ct);
            _tr_str_release(nt);
            _tr_str_release(m);
            _tr_str_release(nc);
            return GpuEmitter_i32_to_i64(self, gs);
        }
#line 525 "src/codegen/gpu/emit.tr"
        TrStr tid = GpuEmitter_fresh(self);
#line 525 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7406 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tid))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.tid.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7406); _tr_str_release(_at_t7406); });
#line 526 "src/codegen/gpu/emit.tr"
        TrStr g = GpuEmitter_fresh(self);
#line 526 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7407 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (g))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = add i32 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tid)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7407); _tr_str_release(_at_t7407); });
#line 527 "src/codegen/gpu/emit.tr"
        _tr_str_release(axis);
        _tr_str_release(ct);
        _tr_str_release(nt);
        _tr_str_release(m);
        _tr_str_release(tid);
        return GpuEmitter_i32_to_i64(self, g);
    }
#line 529 "src/codegen/gpu/emit.tr"
    TrStr mangled = _tr_str_lit_len("@_Z13get_global_idj", 19LL);
#line 530 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_id", 12LL)))) {
#line 530 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t7408 = _tr_str_lit_len("@_Z12get_local_idj", 18LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t7408;
    }
#line 531 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) {
#line 531 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t7409 = _tr_str_lit_len("@_Z12get_group_idj", 18LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t7409;
    }
#line 532 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) {
#line 532 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t7410 = _tr_str_lit_len("@_Z14get_local_sizej", 20LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t7410;
    }
#line 533 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) {
#line 533 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t7411 = _tr_str_lit_len("@_Z15get_global_sizej", 21LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t7411;
    }
#line 534 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL)))) {
#line 534 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t7412 = _tr_str_lit_len("@_Z14get_num_groupsj", 20LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t7412;
    }
#line 535 "src/codegen/gpu/emit.tr"
    TrStr sr = GpuEmitter_fresh(self);
#line 536 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t7413 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (sr))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call spir_func i64 ", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mangled)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(i32 ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(dim)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7413); _tr_str_release(_at_t7413); });
#line 537 "src/codegen/gpu/emit.tr"
    _tr_str_release(mangled);
    return GVal_make(_tr_str_lit_len("i64", 3LL), sr);
}

__attribute__((hot)) GVal* GpuEmitter_i32_to_i64(GpuEmitter* self, TrStr v) {
#line 540 "src/codegen/gpu/emit.tr"
    TrStr r = GpuEmitter_fresh(self);
#line 541 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t7414 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sext i32 ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7414); _tr_str_release(_at_t7414); });
#line 542 "src/codegen/gpu/emit.tr"
    return GVal_make(_tr_str_lit_len("i64", 3LL), r);
}

__attribute__((hot)) GVal* GpuEmitter_emit_binop(GpuEmitter* self, TrStr op, HirExpr* lp, HirExpr* rp) {
#line 546 "src/codegen/gpu/emit.tr"
    GVal* l = GpuEmitter_emit_expr(self, lp);
#line 547 "src/codegen/gpu/emit.tr"
    GVal* r = GpuEmitter_emit_expr(self, rp);
#line 548 "src/codegen/gpu/emit.tr"
    TrStr ct = GpuEmitter_common_ty(self, l->ty, r->ty);
#line 549 "src/codegen/gpu/emit.tr"
    TrStr la = GpuEmitter_coerce(self, l, ct);
#line 550 "src/codegen/gpu/emit.tr"
    TrStr ra = GpuEmitter_coerce(self, r, ct);
#line 551 "src/codegen/gpu/emit.tr"
    bool isf = _gpu_is_float(ct);
#line 552 "src/codegen/gpu/emit.tr"
    TrStr res = GpuEmitter_fresh(self);
#line 553 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("+", 1LL)))) {
#line 554 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 554 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7415 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fadd ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7415); _tr_str_release(_at_t7415); });
        } else {
#line 555 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7416 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = add ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7416); _tr_str_release(_at_t7416); });
        }
#line 556 "src/codegen/gpu/emit.tr"
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
#line 557 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 558 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 558 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7417 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fsub ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7417); _tr_str_release(_at_t7417); });
        } else {
#line 559 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7418 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sub ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7418); _tr_str_release(_at_t7418); });
        }
#line 560 "src/codegen/gpu/emit.tr"
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
#line 561 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("*", 1LL)))) {
#line 562 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 562 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7419 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fmul ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7419); _tr_str_release(_at_t7419); });
        } else {
#line 563 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7420 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = mul ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7420); _tr_str_release(_at_t7420); });
        }
#line 564 "src/codegen/gpu/emit.tr"
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
#line 565 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("/", 1LL)))) {
#line 566 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 566 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7421 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fdiv ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7421); _tr_str_release(_at_t7421); });
        } else {
#line 567 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7422 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sdiv ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7422); _tr_str_release(_at_t7422); });
        }
#line 568 "src/codegen/gpu/emit.tr"
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
#line 569 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("%", 1LL)))) {
#line 570 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 570 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7423 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = frem ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7423); _tr_str_release(_at_t7423); });
        } else {
#line 571 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7424 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = srem ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7424); _tr_str_release(_at_t7424); });
        }
#line 572 "src/codegen/gpu/emit.tr"
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
#line 574 "src/codegen/gpu/emit.tr"
    TrStr pred = GpuEmitter_cmp_pred(self, op, isf);
#line 575 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((pred), (_tr_str_lit_len("", 0LL)))) {
#line 576 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7425 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unsupported operator '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in kernel", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_fail(self, _at_t7425); _tr_str_release(_at_t7425); });
#line 577 "src/codegen/gpu/emit.tr"
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(ct);
        _tr_str_release(la);
        _tr_str_release(ra);
        _tr_str_release(res);
        _tr_str_release(pred);
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
#line 578 "src/codegen/gpu/emit.tr"
    if (isf) {
#line 578 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7426 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fcmp ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pred)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7426); _tr_str_release(_at_t7426); });
    } else {
#line 579 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7427 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = icmp ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pred)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7427); _tr_str_release(_at_t7427); });
    }
#line 580 "src/codegen/gpu/emit.tr"
    _tr_obj_release(l, _trdrop_GVal);
    _tr_obj_release(r, _trdrop_GVal);
    _tr_str_release(ct);
    _tr_str_release(la);
    _tr_str_release(ra);
    _tr_str_release(pred);
    return GVal_make(_tr_str_lit_len("i1", 2LL), res);
}

__attribute__((hot)) TrStr GpuEmitter_cmp_pred(GpuEmitter* self, TrStr op, bool isf) {
#line 583 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("==", 2LL)))) {
#line 584 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 584 "src/codegen/gpu/emit.tr"
            return _tr_str_lit_len("oeq", 3LL);
        }
#line 585 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("eq", 2LL);
    }
#line 586 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("!=", 2LL)))) {
#line 587 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 587 "src/codegen/gpu/emit.tr"
            return _tr_str_lit_len("one", 3LL);
        }
#line 588 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("ne", 2LL);
    }
#line 589 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<", 1LL)))) {
#line 590 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 590 "src/codegen/gpu/emit.tr"
            return _tr_str_lit_len("olt", 3LL);
        }
#line 591 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("slt", 3LL);
    }
#line 592 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len("<=", 2LL)))) {
#line 593 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 593 "src/codegen/gpu/emit.tr"
            return _tr_str_lit_len("ole", 3LL);
        }
#line 594 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("sle", 3LL);
    }
#line 595 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">", 1LL)))) {
#line 596 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 596 "src/codegen/gpu/emit.tr"
            return _tr_str_lit_len("ogt", 3LL);
        }
#line 597 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("sgt", 3LL);
    }
#line 598 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((op), (_tr_str_lit_len(">=", 2LL)))) {
#line 599 "src/codegen/gpu/emit.tr"
        if (isf) {
#line 599 "src/codegen/gpu/emit.tr"
            return _tr_str_lit_len("oge", 3LL);
        }
#line 600 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("sge", 3LL);
    }
#line 601 "src/codegen/gpu/emit.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr GpuEmitter_common_ty(GpuEmitter* self, TrStr a, TrStr b) {
#line 604 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((a), (b))) {
#line 604 "src/codegen/gpu/emit.tr"
        return _tr_str_retain(a);
    }
#line 605 "src/codegen/gpu/emit.tr"
    bool af = _gpu_is_float(a);
#line 606 "src/codegen/gpu/emit.tr"
    bool bf = _gpu_is_float(b);
#line 607 "src/codegen/gpu/emit.tr"
    if ((af || bf)) {
#line 608 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((a), (_tr_str_lit_len("double", 6LL))) || _tr_str_eqv((b), (_tr_str_lit_len("double", 6LL))))) {
#line 608 "src/codegen/gpu/emit.tr"
            return _tr_str_lit_len("double", 6LL);
        }
#line 609 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("float", 5LL);
    }
#line 611 "src/codegen/gpu/emit.tr"
    if (({ __auto_type _ord_t7428 = (_gpu_iwidth(a)); __auto_type _wr = ((_ord_t7428 >= _gpu_iwidth(b))); _wr; })) {
#line 611 "src/codegen/gpu/emit.tr"
        return _tr_str_retain(a);
    }
#line 612 "src/codegen/gpu/emit.tr"
    return _tr_str_retain(b);
}

__attribute__((hot)) TrStr GpuEmitter_coerce(GpuEmitter* self, GVal* v, TrStr target) {
#line 616 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((v->ty), (target))) {
#line 616 "src/codegen/gpu/emit.tr"
        return _tr_str_retain(v->val);
    }
#line 617 "src/codegen/gpu/emit.tr"
    TrStr r = GpuEmitter_fresh(self);
#line 618 "src/codegen/gpu/emit.tr"
    bool sf = _gpu_is_float(v->ty);
#line 619 "src/codegen/gpu/emit.tr"
    bool tf = _gpu_is_float(target);
#line 620 "src/codegen/gpu/emit.tr"
    if ((sf && tf)) {
#line 621 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((target), (_tr_str_lit_len("double", 6LL)))) {
#line 621 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7429 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fpext float ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to double\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7429); _tr_str_release(_at_t7429); });
        } else {
#line 622 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t7430 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fptrunc double ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to float\n", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7430); _tr_str_release(_at_t7430); });
        }
#line 623 "src/codegen/gpu/emit.tr"
        TrStr _trret1907 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret1907;
    }
#line 624 "src/codegen/gpu/emit.tr"
    if ((tf && (!sf))) {
#line 625 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7431 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sitofp ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7431); _tr_str_release(_at_t7431); });
#line 626 "src/codegen/gpu/emit.tr"
        TrStr _trret1908 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret1908;
    }
#line 627 "src/codegen/gpu/emit.tr"
    if ((sf && (!tf))) {
#line 628 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7432 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fptosi ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7432); _tr_str_release(_at_t7432); });
#line 629 "src/codegen/gpu/emit.tr"
        TrStr _trret1909 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret1909;
    }
#line 630 "src/codegen/gpu/emit.tr"
    long long sw = _gpu_iwidth(v->ty);
#line 631 "src/codegen/gpu/emit.tr"
    long long tw = _gpu_iwidth(target);
#line 632 "src/codegen/gpu/emit.tr"
    if ((tw > sw)) {
#line 633 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7433 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sext ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7433); _tr_str_release(_at_t7433); });
#line 634 "src/codegen/gpu/emit.tr"
        TrStr _trret1910 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret1910;
    }
#line 635 "src/codegen/gpu/emit.tr"
    if ((tw < sw)) {
#line 636 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7434 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = trunc ", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7434); _tr_str_release(_at_t7434); });
#line 637 "src/codegen/gpu/emit.tr"
        TrStr _trret1911 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret1911;
    }
#line 638 "src/codegen/gpu/emit.tr"
    TrStr _trret1912 = _tr_str_retain(v->val);
    _tr_str_release(r);
    return _trret1912;
}

__attribute__((hot)) TrStr GpuEmitter_coerce_bool(GpuEmitter* self, GVal* v) {
#line 642 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((v->ty), (_tr_str_lit_len("i1", 2LL)))) {
#line 642 "src/codegen/gpu/emit.tr"
        return _tr_str_retain(v->val);
    }
#line 643 "src/codegen/gpu/emit.tr"
    TrStr r = GpuEmitter_fresh(self);
#line 644 "src/codegen/gpu/emit.tr"
    if (_gpu_is_float(v->ty)) {
#line 644 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7435 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fcmp one ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", 0.0\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7435); _tr_str_release(_at_t7435); });
    } else {
#line 645 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t7436 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = icmp ne ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", 0\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t7436); _tr_str_release(_at_t7436); });
    }
#line 646 "src/codegen/gpu/emit.tr"
    TrStr _trret1913 = _tr_str_retain(r);
    _tr_str_release(r);
    return _trret1913;
}

__attribute__((hot)) long long GpuEmitter_literal_int(GpuEmitter* self, HirExpr e) {
#line 650 "src/codegen/gpu/emit.tr"
    __auto_type _t7437 = e;
    if (_t7437.tag == HirExpr_ELitInt) {
        __auto_type val = _t7437.data.ELitInt.val;
__auto_type ty = _t7437.data.ELitInt.ty;
        long long _trret1914 = val;
        return _trret1914;
    } else if (1) {
        __auto_type _ = _t7437;
        return 0LL;
    }
}

__attribute__((hot)) TrStr GpuEmitter_float_lit(GpuEmitter* self, double v) {
#line 657 "src/codegen/gpu/emit.tr"
    return ({ TrStr _tr_cr = (_hex16(_gpu_f64_bits(v))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("0x", 2LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
}

__attribute__((hot)) bool fn_is_kernel(HirFunction* f) {
#line 18 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 19 "src/codegen/gpu/emit.tr"
    while ((i < f->decorators->len)) {
#line 20 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((((Decorator*)List_ptr_get(f->decorators, i))->name), (_tr_str_lit_len("kernel", 6LL)))) {
#line 20 "src/codegen/gpu/emit.tr"
            return true;
        }
#line 21 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
#line 22 "src/codegen/gpu/emit.tr"
    return false;
}

__attribute__((hot)) bool fn_is_device(HirFunction* f) {
#line 27 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 28 "src/codegen/gpu/emit.tr"
    while ((i < f->decorators->len)) {
#line 29 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((((Decorator*)List_ptr_get(f->decorators, i))->name), (_tr_str_lit_len("device", 6LL)))) {
#line 29 "src/codegen/gpu/emit.tr"
            return true;
        }
#line 30 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
#line 31 "src/codegen/gpu/emit.tr"
    return false;
}

__attribute__((hot)) bool _gpu_is_float(TrStr t) {
#line 35 "src/codegen/gpu/emit.tr"
    return (_tr_str_eqv((t), (_tr_str_lit_len("float", 5LL))) || _tr_str_eqv((t), (_tr_str_lit_len("double", 6LL))));
}

__attribute__((hot)) long long _gpu_iwidth(TrStr t) {
#line 38 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((t), (_tr_str_lit_len("i1", 2LL)))) {
#line 38 "src/codegen/gpu/emit.tr"
        return 1LL;
    }
#line 39 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((t), (_tr_str_lit_len("i8", 2LL)))) {
#line 39 "src/codegen/gpu/emit.tr"
        return 8LL;
    }
#line 40 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((t), (_tr_str_lit_len("i16", 3LL)))) {
#line 40 "src/codegen/gpu/emit.tr"
        return 16LL;
    }
#line 41 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((t), (_tr_str_lit_len("i32", 3LL)))) {
#line 41 "src/codegen/gpu/emit.tr"
        return 32LL;
    }
#line 42 "src/codegen/gpu/emit.tr"
    return 64LL;
}

__attribute__((hot)) TrStr _gpu_scalar_ty(TrStr n) {
#line 46 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("f32", 3LL)))) {
#line 46 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("float", 5LL);
    }
#line 47 "src/codegen/gpu/emit.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("f64", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("float", 5LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("double", 6LL))))) {
#line 47 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("double", 6LL);
    }
#line 48 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((n), (_tr_str_lit_len("bool", 4LL)))) {
#line 48 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("i1", 2LL);
    }
#line 49 "src/codegen/gpu/emit.tr"
    if ((((((_tr_str_eqv((n), (_tr_str_lit_len("i8", 2LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u8", 2LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_char", 6LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("char", 4LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_schar", 7LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_uchar", 7LL))))) {
#line 49 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("i8", 2LL);
    }
#line 50 "src/codegen/gpu/emit.tr"
    if ((_tr_str_eqv((n), (_tr_str_lit_len("i16", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u16", 3LL))))) {
#line 50 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("i16", 3LL);
    }
#line 51 "src/codegen/gpu/emit.tr"
    if (((_tr_str_eqv((n), (_tr_str_lit_len("i32", 3LL))) || _tr_str_eqv((n), (_tr_str_lit_len("u32", 3LL)))) || _tr_str_eqv((n), (_tr_str_lit_len("c_int", 5LL))))) {
#line 51 "src/codegen/gpu/emit.tr"
        return _tr_str_lit_len("i32", 3LL);
    }
#line 52 "src/codegen/gpu/emit.tr"
    return _tr_str_lit_len("i64", 3LL);
}

__attribute__((hot)) long long _gpu_f64_bits(double v) {
#line 662 "src/codegen/gpu/emit.tr"
    /* unsafe block */
#line 663 "src/codegen/gpu/emit.tr"
    double* p = ((double*)_tr_c_calloc((size_t)(1LL), sizeof(double)));
#line 664 "src/codegen/gpu/emit.tr"
    (*p = v);
#line 665 "src/codegen/gpu/emit.tr"
    long long* ip = ((long long*)(p));
#line 666 "src/codegen/gpu/emit.tr"
    return (*ip);
}

__attribute__((hot)) long long _hexdig(long long n) {
#line 669 "src/codegen/gpu/emit.tr"
    if ((n < 10LL)) {
#line 669 "src/codegen/gpu/emit.tr"
        return (48LL + n);
    }
#line 670 "src/codegen/gpu/emit.tr"
    return (65LL + (n - 10LL));
}

__attribute__((hot)) TrStr _hex16(long long v) {
#line 673 "src/codegen/gpu/emit.tr"
    StringBuilder* sb = StringBuilder_init(16LL);
#line 674 "src/codegen/gpu/emit.tr"
    long long k = 15LL;
#line 675 "src/codegen/gpu/emit.tr"
    while ((k >= 0LL)) {
#line 676 "src/codegen/gpu/emit.tr"
        long long nib = ((v >> (4LL * k)) & 15LL);
#line 677 "src/codegen/gpu/emit.tr"
        StringBuilder_append_char(sb, _hexdig(nib));
#line 678 "src/codegen/gpu/emit.tr"
        k = (k - 1LL);
    }
#line 679 "src/codegen/gpu/emit.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

