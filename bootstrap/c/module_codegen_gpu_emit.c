#include "tauraro_types.h"


__attribute__((hot)) GVal* GVal_make(TrStr ty, TrStr val) {
#line 61 "src/codegen/gpu/emit.tr"
    GVal* v = ((GVal*)_tr_obj_alloc(sizeof(GVal)));
#line 62 "src/codegen/gpu/emit.tr"
    TrStr _sfo_t8932 = v->ty;
    v->ty = _tr_str_retain(ty);
    _tr_str_release(_sfo_t8932);
#line 63 "src/codegen/gpu/emit.tr"
    TrStr _sfo_t8933 = v->val;
    v->val = _tr_str_retain(val);
    _tr_str_release(_sfo_t8933);
#line 64 "src/codegen/gpu/emit.tr"
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) GpuEmitter* GpuEmitter_init(TrStr target) {
#line 81 "src/codegen/gpu/emit.tr"
    GpuEmitter* e = ((GpuEmitter*)_tr_obj_alloc(sizeof(GpuEmitter)));
#line 82 "src/codegen/gpu/emit.tr"
    e->sb = StringBuilder_init(1024LL);
#line 83 "src/codegen/gpu/emit.tr"
    TrStr _sfo_t8934 = e->target;
    e->target = _tr_str_retain(target);
    _tr_str_release(_sfo_t8934);
#line 84 "src/codegen/gpu/emit.tr"
    e->tmp = 0LL;
#line 85 "src/codegen/gpu/emit.tr"
    e->lbl = 0LL;
#line 86 "src/codegen/gpu/emit.tr"
    e->ok = true;
#line 87 "src/codegen/gpu/emit.tr"
    TrStr _sfo_t8935 = e->fail_note;
    e->fail_note = _tr_str_lit_len("", 0LL);
    _tr_str_release(_sfo_t8935);
#line 88 "src/codegen/gpu/emit.tr"
    e->var_names = (void*)List_TrStr_new();
#line 89 "src/codegen/gpu/emit.tr"
    e->var_ll = (void*)List_TrStr_new();
#line 90 "src/codegen/gpu/emit.tr"
    e->var_elem = (void*)List_TrStr_new();
#line 91 "src/codegen/gpu/emit.tr"
    TrStr _sfo_t8936 = e->ret_llty;
    e->ret_llty = _tr_str_lit_len("void", 4LL);
    _tr_str_release(_sfo_t8936);
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
    return ({ TrStr _apt_t8937 = _tr_str_retain(t->name); __auto_type _wr = (_gpu_scalar_ty(_apt_t8937)); _tr_str_release(_apt_t8937); _wr; });
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
    TrStr _trret2008 = _tr_str_retain(r);
    _tr_str_release(r);
    return _trret2008;
}

__attribute__((hot)) TrStr GpuEmitter_newlbl(GpuEmitter* self) {
#line 110 "src/codegen/gpu/emit.tr"
    TrStr r = ({ TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->lbl)))); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("L", 1LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; });
#line 111 "src/codegen/gpu/emit.tr"
    self->lbl = (self->lbl + 1LL);
#line 112 "src/codegen/gpu/emit.tr"
    TrStr _trret2009 = _tr_str_retain(r);
    _tr_str_release(r);
    return _trret2009;
}

__attribute__((hot)) void GpuEmitter_fail(GpuEmitter* self, TrStr why) {
#line 115 "src/codegen/gpu/emit.tr"
    self->ok = false;
#line 116 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((self->fail_note), (_tr_str_lit_len("", 0LL)))) {
#line 116 "src/codegen/gpu/emit.tr"
        TrStr _sfo_t8938 = self->fail_note;
        self->fail_note = _tr_str_retain(why);
        _tr_str_release(_sfo_t8938);
    }
}

__attribute__((hot)) void GpuEmitter_add_var(GpuEmitter* self, TrStr name, AstType* ty) {
#line 120 "src/codegen/gpu/emit.tr"
    long long i = 0LL;
#line 121 "src/codegen/gpu/emit.tr"
    while ((i < self->var_names->len)) {
#line 122 "src/codegen/gpu/emit.tr"
        if (({ TrStr _cmpa_t8939 = (List_TrStr_get(self->var_names, i)); TrStr _cmpb_t8940 = (name); _Bool _cmpr_t8941 = _tr_str_eqv(_cmpa_t8939, _cmpb_t8940); _tr_str_release(_cmpa_t8939); _cmpr_t8941; })) {
#line 122 "src/codegen/gpu/emit.tr"
            return;
        }
#line 123 "src/codegen/gpu/emit.tr"
        i = (i + 1LL);
    }
#line 124 "src/codegen/gpu/emit.tr"
    if ((_tr_str_eqv((ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (ty->args->len > 0LL))) {
#line 125 "src/codegen/gpu/emit.tr"
        TrStr et = ({ TrStr _apt_t8942 = _tr_str_retain((*((AstType**)List_ptr_get(ty->args, 0LL)))->name); __auto_type _wr = (_gpu_scalar_ty(_apt_t8942)); _tr_str_release(_apt_t8942); _wr; });
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
        ({ TrStr _apt_t8943 = _tr_str_retain(ty->name); TrStr _ord_t8944 = (_gpu_scalar_ty(_apt_t8943)); List_TrStr_append(self->var_ll, _ord_t8944); _tr_str_release(_apt_t8943); _tr_str_release(_ord_t8944); });
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
        if (({ TrStr _cmpa_t8945 = (List_TrStr_get(self->var_names, i)); TrStr _cmpb_t8946 = (name); _Bool _cmpr_t8947 = _tr_str_eqv(_cmpa_t8945, _cmpb_t8946); _tr_str_release(_cmpa_t8945); _cmpr_t8947; })) {
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
        if (({ TrStr _cmpa_t8948 = (List_TrStr_get(self->var_names, i)); TrStr _cmpb_t8949 = (name); _Bool _cmpr_t8950 = _tr_str_eqv(_cmpa_t8948, _cmpb_t8949); _tr_str_release(_cmpa_t8948); _cmpr_t8950; })) {
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
    __auto_type _t8951 = s;
    if (_t8951.tag == HirStmt_SLet) {
        __auto_type name = _t8951.data.SLet.name;
__auto_type ownership = _t8951.data.SLet.ownership;
__auto_type is_mut = _t8951.data.SLet.is_mut;
__auto_type is_const = _t8951.data.SLet.is_const;
__auto_type is_shared = _t8951.data.SLet.is_shared;
__auto_type ty = _t8951.data.SLet.ty;
__auto_type val = _t8951.data.SLet.val;
#line 158 "src/codegen/gpu/emit.tr"
        AstType* vt = ty;
#line 159 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((vt->name), (_tr_str_lit_len("", 0LL)))) {
#line 159 "src/codegen/gpu/emit.tr"
            vt = hir_expr_type(val);
        }
#line 160 "src/codegen/gpu/emit.tr"
        GpuEmitter_add_var(self, name, vt);
    } else if (_t8951.tag == HirStmt_SIf) {
        __auto_type cond = _t8951.data.SIf.cond;
__auto_type then_b = _t8951.data.SIf.then_b;
__auto_type else_b = _t8951.data.SIf.else_b;
#line 162 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, then_b);
#line 163 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, else_b);
    } else if (_t8951.tag == HirStmt_SWhile) {
        __auto_type cond = _t8951.data.SWhile.cond;
__auto_type body = _t8951.data.SWhile.body;
#line 165 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, body);
    } else if (_t8951.tag == HirStmt_SUnsafe) {
        __auto_type body = _t8951.data.SUnsafe.body;
#line 167 "src/codegen/gpu/emit.tr"
        GpuEmitter_scan_vars_block(self, body);
    } else if (1) {
        __auto_type _ = _t8951;
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
        ({ TrStr _apt_t8952 = _tr_str_retain(p->name); GpuEmitter_add_var(self, _apt_t8952, p->ty); _tr_str_release(_apt_t8952); });
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
        TrStr _strtmp_t8953 = _tr_str_lit_len("ptx_kernel", 10LL);
        _tr_str_release(cc);
        cc = _strtmp_t8953;
    }
#line 185 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t8954 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (cc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" void @", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8954); _tr_str_release(_at_t8954); });
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
        ({ TrStr _apt_t8955 = _tr_str_retain(pp->name); __auto_type _ord_t8956 = (({ TrStr _tr_cl = (GpuEmitter_var_slot_ty(self, _apt_t8955)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t8957 = (({ TrStr _tr_cl = (_ord_t8956); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pp->name)); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8957); _tr_str_release(_apt_t8955); _tr_str_release(_at_t8957); });
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
        ({ __auto_type _ord_t8958 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(self->var_names, vi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t8959 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t8958); TrStr _tr_cr = (List_TrStr_get(self->var_ll, vi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8959); _tr_str_release(_at_t8959); });
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
        ({ TrStr _at_t8960 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8960); _tr_str_release(_at_t8960); });
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
        TrStr _trret2010 = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("; kernel '", 10LL)), (f->name))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' NOT emitted: ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (self->fail_note)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; });
        _tr_str_release(cc);
        return _trret2010;
    }
#line 209 "src/codegen/gpu/emit.tr"
    TrStr _trret2011 = StringObj_as_str(StringBuilder_to_string(self->sb));
    _tr_str_release(cc);
    return _trret2011;
}

__attribute__((hot)) TrStr GpuEmitter_emit_device_fn(GpuEmitter* self, HirFunction* f) {
#line 214 "src/codegen/gpu/emit.tr"
    TrStr _sfo_t8961 = self->ret_llty;
    self->ret_llty = GpuEmitter__gpu_ret_ty(self, f->ret_ty);
    _tr_str_release(_sfo_t8961);
#line 215 "src/codegen/gpu/emit.tr"
    long long pi = 0LL;
#line 216 "src/codegen/gpu/emit.tr"
    while ((pi < f->params->len)) {
#line 217 "src/codegen/gpu/emit.tr"
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
#line 218 "src/codegen/gpu/emit.tr"
        ({ TrStr _apt_t8962 = _tr_str_retain(p->name); GpuEmitter_add_var(self, _apt_t8962, p->ty); _tr_str_release(_apt_t8962); });
#line 219 "src/codegen/gpu/emit.tr"
        pi = (pi + 1LL);
    }
#line 220 "src/codegen/gpu/emit.tr"
    GpuEmitter_scan_vars_block(self, f->body);
#line 222 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t8963 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("define ", 7LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (f->name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8963); _tr_str_release(_at_t8963); });
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
        ({ TrStr _apt_t8964 = _tr_str_retain(pp->name); __auto_type _ord_t8965 = (({ TrStr _tr_cl = (GpuEmitter_var_slot_ty(self, _apt_t8964)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t8966 = (({ TrStr _tr_cl = (_ord_t8965); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pp->name)); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8966); _tr_str_release(_apt_t8964); _tr_str_release(_at_t8966); });
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
        ({ __auto_type _ord_t8967 = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (List_TrStr_get(self->var_names, vi)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  %var_", 7LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = alloca ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _at_t8968 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_ord_t8967); TrStr _tr_cr = (List_TrStr_get(self->var_ll, vi)); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8968); _tr_str_release(_at_t8968); });
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
        ({ TrStr _at_t8969 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _tr_cres = _tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), _tr_cr); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" %arg_", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pn)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8969); _tr_str_release(_at_t8969); });
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
    } else if (({ TrStr _apt_t8970 = _tr_str_retain(self->ret_llty); __auto_type _wr = (_gpu_is_float(_apt_t8970)); _tr_str_release(_apt_t8970); _wr; })) {
#line 244 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8971 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 0.0\n}\n\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8971); _tr_str_release(_at_t8971); });
    } else if (_tr_str_eqv((self->ret_llty), (_tr_str_lit_len("ptr addrspace(1)", 16LL)))) {
#line 246 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8972 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" null\n}\n\n", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8972); _tr_str_release(_at_t8972); });
    } else {
#line 248 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8973 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 0\n}\n\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8973); _tr_str_release(_at_t8973); });
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
    __auto_type _t8974 = s;
    if (_t8974.tag == HirStmt_SLet) {
        __auto_type name = _t8974.data.SLet.name;
__auto_type ownership = _t8974.data.SLet.ownership;
__auto_type is_mut = _t8974.data.SLet.is_mut;
__auto_type is_const = _t8974.data.SLet.is_const;
__auto_type is_shared = _t8974.data.SLet.is_shared;
__auto_type ty = _t8974.data.SLet.ty;
__auto_type val = _t8974.data.SLet.val;
#line 262 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, val);
#line 263 "src/codegen/gpu/emit.tr"
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
#line 264 "src/codegen/gpu/emit.tr"
        TrStr cv = GpuEmitter_coerce(self, v, slot);
#line 265 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8975 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (slot))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8975); _tr_str_release(_at_t8975); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (_t8974.tag == HirStmt_SAssign) {
        __auto_type target = _t8974.data.SAssign.target;
__auto_type val = _t8974.data.SAssign.val;
#line 267 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_assign(self, (*target), val);
    } else if (_t8974.tag == HirStmt_SExpr) {
        __auto_type expr = _t8974.data.SExpr.expr;
#line 269 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_stmt_expr(self, (*expr));
    } else if (_t8974.tag == HirStmt_SIf) {
        __auto_type cond = _t8974.data.SIf.cond;
__auto_type then_b = _t8974.data.SIf.then_b;
__auto_type else_b = _t8974.data.SIf.else_b;
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
        ({ TrStr _at_t8976 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (cc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (le)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8976); _tr_str_release(_at_t8976); });
#line 277 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8977 = (_tr_strx_concatv((lt), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t8977); _tr_str_release(_at_t8977); });
#line 278 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, then_b);
#line 279 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8978 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lend))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8978); _tr_str_release(_at_t8978); });
#line 280 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8979 = (_tr_strx_concatv((le), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t8979); _tr_str_release(_at_t8979); });
#line 281 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, else_b);
#line 282 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8980 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lend))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8980); _tr_str_release(_at_t8980); });
#line 283 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8981 = (_tr_strx_concatv((lend), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t8981); _tr_str_release(_at_t8981); });
        _tr_obj_release(c, _trdrop_GVal);
        _tr_str_release(cc);
        _tr_str_release(lt);
        _tr_str_release(le);
        _tr_str_release(lend);
    } else if (_t8974.tag == HirStmt_SWhile) {
        __auto_type cond = _t8974.data.SWhile.cond;
__auto_type body = _t8974.data.SWhile.body;
#line 285 "src/codegen/gpu/emit.tr"
        TrStr lc = GpuEmitter_newlbl(self);
#line 286 "src/codegen/gpu/emit.tr"
        TrStr lb = GpuEmitter_newlbl(self);
#line 287 "src/codegen/gpu/emit.tr"
        TrStr lend2 = GpuEmitter_newlbl(self);
#line 288 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8982 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8982); _tr_str_release(_at_t8982); });
#line 289 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8983 = (_tr_strx_concatv((lc), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t8983); _tr_str_release(_at_t8983); });
#line 290 "src/codegen/gpu/emit.tr"
        GVal* cnd = GpuEmitter_emit_expr(self, cond);
#line 291 "src/codegen/gpu/emit.tr"
        TrStr cb = GpuEmitter_coerce_bool(self, cnd);
#line 292 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8984 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br i1 ", 8LL)), (cb))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lb)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", label %", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (lend2)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8984); _tr_str_release(_at_t8984); });
#line 293 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8985 = (_tr_strx_concatv((lb), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t8985); _tr_str_release(_at_t8985); });
#line 294 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, body);
#line 295 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8986 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  br label %", 12LL)), (lc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8986); _tr_str_release(_at_t8986); });
#line 296 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8987 = (_tr_strx_concatv((lend2), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t8987); _tr_str_release(_at_t8987); });
        _tr_str_release(lc);
        _tr_str_release(lb);
        _tr_str_release(lend2);
        _tr_obj_release(cnd, _trdrop_GVal);
        _tr_str_release(cb);
    } else if (_t8974.tag == HirStmt_SUnsafe) {
        __auto_type body = _t8974.data.SUnsafe.body;
#line 298 "src/codegen/gpu/emit.tr"
        GpuEmitter_emit_block(self, body);
    } else if (_t8974.tag == HirStmt_SReturn) {
        __auto_type val = _t8974.data.SReturn.val;
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
                ({ TrStr _at_t8988 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ptr addrspace(1) ", 23LL)), (rv->val))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8988); _tr_str_release(_at_t8988); });
            } else {
#line 307 "src/codegen/gpu/emit.tr"
                TrStr cv = ({ TrStr _apt_t8989 = _tr_str_retain(self->ret_llty); __auto_type _wr = (GpuEmitter_coerce(self, rv, _apt_t8989)); _tr_str_release(_apt_t8989); _wr; });
#line 308 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t8990 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ret ", 6LL)), (self->ret_llty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8990); _tr_str_release(_at_t8990); });
            }
        }
#line 310 "src/codegen/gpu/emit.tr"
        TrStr lu = GpuEmitter_newlbl(self);
#line 311 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8991 = (_tr_strx_concatv((lu), (_tr_str_lit_len(":\n", 2LL)))); GpuEmitter_w(self, _at_t8991); _tr_str_release(_at_t8991); });
        _tr_str_release(lu);
    } else if (_t8974.tag == HirStmt_SPass) {
#line 313 "src/codegen/gpu/emit.tr"
        /* pass */
    } else if (_t8974.tag == HirStmt_SLineMarker) {
        __auto_type n = _t8974.data.SLineMarker.n;
#line 315 "src/codegen/gpu/emit.tr"
        /* pass */
    } else if (1) {
        __auto_type _ = _t8974;
#line 317 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported statement in kernel", 31LL));
    }
}

__attribute__((hot)) void GpuEmitter_emit_assign(GpuEmitter* self, HirExpr target, HirExpr* val) {
#line 321 "src/codegen/gpu/emit.tr"
    __auto_type _t8992 = target;
    if (_t8992.tag == HirExpr_EIdent) {
        __auto_type name = _t8992.data.EIdent.name;
__auto_type ty = _t8992.data.EIdent.ty;
__auto_type is_move = _t8992.data.EIdent.is_move;
#line 323 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, val);
#line 324 "src/codegen/gpu/emit.tr"
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
#line 325 "src/codegen/gpu/emit.tr"
        TrStr cv = GpuEmitter_coerce(self, v, slot);
#line 326 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t8993 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (slot))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t8993); _tr_str_release(_at_t8993); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t8992;
#line 328 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported assignment target in kernel", 39LL));
    }
}

__attribute__((hot)) void GpuEmitter_emit_stmt_expr(GpuEmitter* self, HirExpr e) {
#line 332 "src/codegen/gpu/emit.tr"
    __auto_type _t8994 = e;
    if (_t8994.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t8994.data.EMethodCall.obj;
__auto_type method = _t8994.data.EMethodCall.method;
__auto_type args = _t8994.data.EMethodCall.args;
__auto_type ty = _t8994.data.EMethodCall.ty;
#line 334 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("write", 5LL))) && (args->len == 1LL))) {
#line 335 "src/codegen/gpu/emit.tr"
            ({ __auto_type _ord_t8995 = ((*obj)); __auto_type _ord_t8996 = (((HirExpr*)List_ptr_get(args, 0LL))); GpuEmitter_emit_store(self, _ord_t8995, _ord_t8996); });
#line 336 "src/codegen/gpu/emit.tr"
            return;
        }
#line 338 "src/codegen/gpu/emit.tr"
        GVal* _v = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v, _trdrop_GVal);
    } else if (_t8994.tag == HirExpr_ECall) {
        __auto_type callee = _t8994.data.ECall.callee;
__auto_type args = _t8994.data.ECall.args;
__auto_type ty = _t8994.data.ECall.ty;
#line 340 "src/codegen/gpu/emit.tr"
        if (({ TrStr _cmpa_t8997 = (GpuEmitter_callee_name(self, (*callee))); TrStr _cmpb_t8998 = (_tr_str_lit_len("gpu_barrier", 11LL)); _Bool _cmpr_t8999 = _tr_str_eqv(_cmpa_t8997, _cmpb_t8998); _tr_str_release(_cmpa_t8997); _cmpr_t8999; })) {
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
        __auto_type _ = _t8994;
#line 346 "src/codegen/gpu/emit.tr"
        GVal* _v3 = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v3, _trdrop_GVal);
    }
}

__attribute__((hot)) void GpuEmitter_emit_store(GpuEmitter* self, HirExpr chain, HirExpr* valp) {
#line 351 "src/codegen/gpu/emit.tr"
    __auto_type _t9000 = chain;
    if (_t9000.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t9000.data.EMethodCall.obj;
__auto_type pmeth = _t9000.data.EMethodCall.method;
__auto_type pargs = _t9000.data.EMethodCall.args;
__auto_type pty = _t9000.data.EMethodCall.ty;
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
        ({ TrStr _at_t9001 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (i64idx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9001); _tr_str_release(_at_t9001); });
#line 361 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, valp);
#line 362 "src/codegen/gpu/emit.tr"
        TrStr cv = ({ TrStr _apt_t9002 = _tr_str_retain(pr->ty); __auto_type _wr = (GpuEmitter_coerce(self, v, _apt_t9002)); _tr_str_release(_apt_t9002); _wr; });
#line 363 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9003 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  store ", 8LL)), (pr->ty))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (cv)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9003); _tr_str_release(_at_t9003); });
        _tr_obj_release(pr, _trdrop_GVal);
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t9000;
#line 365 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("store target must be p.offset(i).write(v)", 41LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_ptr(GpuEmitter* self, HirExpr e) {
#line 370 "src/codegen/gpu/emit.tr"
    __auto_type _t9004 = e;
    if (_t9004.tag == HirExpr_EIdent) {
        __auto_type name = _t9004.data.EIdent.name;
__auto_type ty = _t9004.data.EIdent.ty;
__auto_type is_move = _t9004.data.EIdent.is_move;
#line 372 "src/codegen/gpu/emit.tr"
        TrStr et = GpuEmitter_var_elem_ty(self, name);
#line 373 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((et), (_tr_str_lit_len("", 0LL)))) {
#line 373 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t9005 = _tr_str_lit_len("float", 5LL);
            _tr_str_release(et);
            et = _strtmp_t9005;
        }
#line 374 "src/codegen/gpu/emit.tr"
        TrStr r = GpuEmitter_fresh(self);
#line 375 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9006 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ptr addrspace(1), ptr %var_", 35LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9006); _tr_str_release(_at_t9006); });
#line 376 "src/codegen/gpu/emit.tr"
        return GVal_make(et, r);
    } else if (_t9004.tag == HirExpr_ECast) {
        __auto_type inner = _t9004.data.ECast.expr;
__auto_type target_ty = _t9004.data.ECast.target_ty;
#line 378 "src/codegen/gpu/emit.tr"
        GVal* g = GpuEmitter_emit_ptr(self, (*inner));
#line 379 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((target_ty->name), (_tr_str_lit_len("Pointer", 7LL))) && (target_ty->args->len > 0LL))) {
#line 380 "src/codegen/gpu/emit.tr"
            TrStr _sfo_t9008 = g->ty;
            g->ty = ({ TrStr _apt_t9007 = _tr_str_retain((*((AstType**)List_ptr_get(target_ty->args, 0LL)))->name); __auto_type _wr = (_gpu_scalar_ty(_apt_t9007)); _tr_str_release(_apt_t9007); _wr; });
            _tr_str_release(_sfo_t9008);
        }
#line 381 "src/codegen/gpu/emit.tr"
        return g;
    } else if (1) {
        __auto_type _ = _t9004;
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
    __auto_type _t9009 = e;
    if (_t9009.tag == HirExpr_ELitInt) {
        __auto_type val = _t9009.data.ELitInt.val;
__auto_type ty = _t9009.data.ELitInt.ty;
#line 393 "src/codegen/gpu/emit.tr"
        TrStr t = ({ TrStr _apt_t9010 = _tr_str_retain(ty->name); __auto_type _wr = (_gpu_scalar_ty(_apt_t9010)); _tr_str_release(_apt_t9010); _wr; });
#line 394 "src/codegen/gpu/emit.tr"
        if (_gpu_is_float(t)) {
#line 394 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t9011 = _tr_str_lit_len("i64", 3LL);
            _tr_str_release(t);
            t = _strtmp_t9011;
        }
#line 395 "src/codegen/gpu/emit.tr"
        return ({ TrStr _at_t9012 = (_tr_str_wrap(_tr_int_to_str((long long)(val)))); __auto_type _wr = (GVal_make(t, _at_t9012)); _tr_str_release(_at_t9012); _wr; });
    } else if (_t9009.tag == HirExpr_ELitFloat) {
        __auto_type val = _t9009.data.ELitFloat.val;
__auto_type ty = _t9009.data.ELitFloat.ty;
#line 397 "src/codegen/gpu/emit.tr"
        TrStr ft = ({ TrStr _apt_t9013 = _tr_str_retain(ty->name); __auto_type _wr = (_gpu_scalar_ty(_apt_t9013)); _tr_str_release(_apt_t9013); _wr; });
#line 398 "src/codegen/gpu/emit.tr"
        if ((!_gpu_is_float(ft))) {
#line 398 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t9014 = _tr_str_lit_len("double", 6LL);
            _tr_str_release(ft);
            ft = _strtmp_t9014;
        }
#line 399 "src/codegen/gpu/emit.tr"
        return ({ TrStr _at_t9015 = (GpuEmitter_float_lit(self, val)); __auto_type _wr = (GVal_make(ft, _at_t9015)); _tr_str_release(_at_t9015); _wr; });
    } else if (_t9009.tag == HirExpr_ELitBool) {
        __auto_type val = _t9009.data.ELitBool.val;
__auto_type ty = _t9009.data.ELitBool.ty;
#line 401 "src/codegen/gpu/emit.tr"
        if (val) {
#line 401 "src/codegen/gpu/emit.tr"
            return GVal_make(_tr_str_lit_len("i1", 2LL), _tr_str_lit_len("1", 1LL));
        }
#line 402 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i1", 2LL), _tr_str_lit_len("0", 1LL));
    } else if (_t9009.tag == HirExpr_EIdent) {
        __auto_type name = _t9009.data.EIdent.name;
__auto_type ty = _t9009.data.EIdent.ty;
__auto_type is_move = _t9009.data.EIdent.is_move;
#line 404 "src/codegen/gpu/emit.tr"
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
#line 405 "src/codegen/gpu/emit.tr"
        TrStr r = GpuEmitter_fresh(self);
#line 406 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9016 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (slot)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr %var_", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (name)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9016); _tr_str_release(_at_t9016); });
#line 407 "src/codegen/gpu/emit.tr"
        return GVal_make(slot, r);
    } else if (_t9009.tag == HirExpr_EBinOp) {
        __auto_type op = _t9009.data.EBinOp.op;
__auto_type left = _t9009.data.EBinOp.left;
__auto_type right = _t9009.data.EBinOp.right;
__auto_type ty = _t9009.data.EBinOp.ty;
#line 409 "src/codegen/gpu/emit.tr"
        return GpuEmitter_emit_binop(self, op, left, right);
    } else if (_t9009.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t9009.data.EUnaryOp.op;
__auto_type expr = _t9009.data.EUnaryOp.expr;
__auto_type ty = _t9009.data.EUnaryOp.ty;
#line 411 "src/codegen/gpu/emit.tr"
        GVal* v = GpuEmitter_emit_expr(self, expr);
#line 412 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((op), (_tr_str_lit_len("-", 1LL)))) {
#line 413 "src/codegen/gpu/emit.tr"
            TrStr r = GpuEmitter_fresh(self);
#line 414 "src/codegen/gpu/emit.tr"
            if (({ TrStr _apt_t9017 = _tr_str_retain(v->ty); __auto_type _wr = (_gpu_is_float(_apt_t9017)); _tr_str_release(_apt_t9017); _wr; })) {
#line 414 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t9018 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fneg ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9018); _tr_str_release(_at_t9018); });
            } else {
#line 415 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t9019 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sub ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" 0, ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9019); _tr_str_release(_at_t9019); });
            }
#line 416 "src/codegen/gpu/emit.tr"
            return ({ TrStr _apt_t9020 = _tr_str_retain(v->ty); __auto_type _wr = (GVal_make(_apt_t9020, r)); _tr_str_release(_apt_t9020); _wr; });
        }
#line 417 "src/codegen/gpu/emit.tr"
        return v;
    } else if (_t9009.tag == HirExpr_ECast) {
        __auto_type inner = _t9009.data.ECast.expr;
__auto_type target_ty = _t9009.data.ECast.target_ty;
#line 419 "src/codegen/gpu/emit.tr"
        GVal* iv = GpuEmitter_emit_expr(self, inner);
#line 420 "src/codegen/gpu/emit.tr"
        TrStr tt = ({ TrStr _apt_t9021 = _tr_str_retain(target_ty->name); __auto_type _wr = (_gpu_scalar_ty(_apt_t9021)); _tr_str_release(_apt_t9021); _wr; });
#line 421 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((target_ty->name), (_tr_str_lit_len("Pointer", 7LL)))) {
#line 421 "src/codegen/gpu/emit.tr"
            _tr_str_release(tt);
            return iv;
        }
#line 422 "src/codegen/gpu/emit.tr"
        return ({ TrStr _at_t9022 = (GpuEmitter_coerce(self, iv, tt)); __auto_type _wr = (GVal_make(tt, _at_t9022)); _tr_str_release(_at_t9022); _wr; });
    } else if (_t9009.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t9009.data.EMethodCall.obj;
__auto_type method = _t9009.data.EMethodCall.method;
__auto_type args = _t9009.data.EMethodCall.args;
__auto_type ty = _t9009.data.EMethodCall.ty;
#line 424 "src/codegen/gpu/emit.tr"
        if ((_tr_str_eqv((method), (_tr_str_lit_len("read", 4LL))) && (args->len == 0LL))) {
#line 425 "src/codegen/gpu/emit.tr"
            return GpuEmitter_emit_load(self, (*obj));
        }
#line 426 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9023 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unsupported method '", 20LL)), (method))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in kernel", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_fail(self, _at_t9023); _tr_str_release(_at_t9023); });
#line 427 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    } else if (_t9009.tag == HirExpr_ECall) {
        __auto_type callee = _t9009.data.ECall.callee;
__auto_type args = _t9009.data.ECall.args;
__auto_type ty = _t9009.data.ECall.ty;
#line 429 "src/codegen/gpu/emit.tr"
        return GpuEmitter_emit_call(self, (*callee), args);
    } else if (1) {
        __auto_type _ = _t9009;
#line 431 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("unsupported expression in kernel", 32LL));
#line 432 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_load(GpuEmitter* self, HirExpr chain) {
#line 436 "src/codegen/gpu/emit.tr"
    __auto_type _t9024 = chain;
    if (_t9024.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t9024.data.EMethodCall.obj;
__auto_type pmeth = _t9024.data.EMethodCall.method;
__auto_type pargs = _t9024.data.EMethodCall.args;
__auto_type pty = _t9024.data.EMethodCall.ty;
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
        ({ TrStr _at_t9025 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gep))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = getelementptr ", 17LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", i64 ", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (i64idx)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9025); _tr_str_release(_at_t9025); });
#line 446 "src/codegen/gpu/emit.tr"
        TrStr r = GpuEmitter_fresh(self);
#line 447 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9026 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = load ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pr->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ptr addrspace(1) ", 19LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (gep)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9026); _tr_str_release(_at_t9026); });
#line 448 "src/codegen/gpu/emit.tr"
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        return ({ TrStr _apt_t9027 = _tr_str_retain(pr->ty); __auto_type _wr = (GVal_make(_apt_t9027, r)); _tr_str_release(_apt_t9027); _wr; });
    } else if (1) {
        __auto_type _ = _t9024;
#line 450 "src/codegen/gpu/emit.tr"
        GpuEmitter_fail(self, _tr_str_lit_len("load must be p.offset(i).read()", 31LL));
#line 451 "src/codegen/gpu/emit.tr"
        return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
    }
}

__attribute__((hot)) TrStr GpuEmitter_callee_name(GpuEmitter* self, HirExpr c) {
#line 454 "src/codegen/gpu/emit.tr"
    __auto_type _t9028 = c;
    if (_t9028.tag == HirExpr_EIdent) {
        __auto_type name = _t9028.data.EIdent.name;
__auto_type ty = _t9028.data.EIdent.ty;
__auto_type is_move = _t9028.data.EIdent.is_move;
        TrStr _trret2012 = _tr_str_retain(name);
        return _trret2012;
    } else if (1) {
        __auto_type _ = _t9028;
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
                    ({ TrStr _ap_t9029 = (_tr_str_lit_len(", ", 2LL)); _tr_str_append(&argstr, _ap_t9029); });
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
                        TrStr _strtmp_t9030 = GpuEmitter_coerce(self, av, ptt);
                        _tr_str_release(aval);
                        aval = _strtmp_t9030;
#line 483 "src/codegen/gpu/emit.tr"
                        TrStr _strtmp_t9031 = _tr_str_retain(ptt);
                        _tr_str_release(aty);
                        aty = _strtmp_t9031;
                    }
                }
#line 484 "src/codegen/gpu/emit.tr"
                ({ TrStr _ap_t9032 = (aty); TrStr _ap_t9033 = (_tr_str_lit_len(" ", 1LL)); TrStr _ap_t9034 = (aval); _tr_str_append(&argstr, _ap_t9032); _tr_str_append(&argstr, _ap_t9033); _tr_str_append(&argstr, _ap_t9034); });
#line 485 "src/codegen/gpu/emit.tr"
                ai = (ai + 1LL);
                _tr_obj_release(av, _trdrop_GVal);
                _tr_str_release(aty);
                _tr_str_release(aval);
            }
#line 486 "src/codegen/gpu/emit.tr"
            if (_tr_str_eqv((rty), (_tr_str_lit_len("void", 4LL)))) {
#line 487 "src/codegen/gpu/emit.tr"
                ({ TrStr _at_t9035 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  call void @", 13LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (argstr)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9035); _tr_str_release(_at_t9035); });
#line 488 "src/codegen/gpu/emit.tr"
                _tr_str_release(nm);
                _tr_str_release(rty);
                _tr_str_release(argstr);
                return GVal_make(_tr_str_lit_len("i64", 3LL), _tr_str_lit_len("0", 1LL));
            }
#line 489 "src/codegen/gpu/emit.tr"
            TrStr r = GpuEmitter_fresh(self);
#line 490 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9036 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (rty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" @", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nm)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (argstr)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9036); _tr_str_release(_at_t9036); });
#line 491 "src/codegen/gpu/emit.tr"
            _tr_str_release(nm);
            _tr_str_release(argstr);
            return GVal_make(rty, r);
        }
#line 492 "src/codegen/gpu/emit.tr"
        di = (di + 1LL);
    }
#line 493 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t9037 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("call to '", 9LL)), (nm))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' not allowed in kernel (mark it @device, or use a GPU builtin)", 63LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_fail(self, _at_t9037); _tr_str_release(_at_t9037); });
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
            TrStr _strtmp_t9038 = _tr_str_lit_len("y", 1LL);
            _tr_str_release(axis);
            axis = _strtmp_t9038;
        }
#line 500 "src/codegen/gpu/emit.tr"
        if ((dim == 2LL)) {
#line 500 "src/codegen/gpu/emit.tr"
            TrStr _strtmp_t9039 = _tr_str_lit_len("z", 1LL);
            _tr_str_release(axis);
            axis = _strtmp_t9039;
        }
#line 501 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_id", 12LL)))) {
#line 502 "src/codegen/gpu/emit.tr"
            TrStr r = GpuEmitter_fresh(self);
#line 503 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9040 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.tid.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9040); _tr_str_release(_at_t9040); });
#line 504 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r);
        }
#line 505 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) {
#line 506 "src/codegen/gpu/emit.tr"
            TrStr r2 = GpuEmitter_fresh(self);
#line 507 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9041 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r2))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9041); _tr_str_release(_at_t9041); });
#line 508 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r2);
        }
#line 509 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) {
#line 510 "src/codegen/gpu/emit.tr"
            TrStr r3 = GpuEmitter_fresh(self);
#line 511 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9042 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r3))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid.", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9042); _tr_str_release(_at_t9042); });
#line 512 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r3);
        }
#line 513 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL)))) {
#line 514 "src/codegen/gpu/emit.tr"
            TrStr r4 = GpuEmitter_fresh(self);
#line 515 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9043 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r4))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid.", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9043); _tr_str_release(_at_t9043); });
#line 516 "src/codegen/gpu/emit.tr"
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r4);
        }
#line 518 "src/codegen/gpu/emit.tr"
        TrStr ct = GpuEmitter_fresh(self);
#line 518 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9044 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (ct))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid.", 43LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9044); _tr_str_release(_at_t9044); });
#line 519 "src/codegen/gpu/emit.tr"
        TrStr nt = GpuEmitter_fresh(self);
#line 519 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9045 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (nt))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid.", 42LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9045); _tr_str_release(_at_t9045); });
#line 520 "src/codegen/gpu/emit.tr"
        TrStr m = GpuEmitter_fresh(self);
#line 520 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9046 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (m))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = mul i32 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9046); _tr_str_release(_at_t9046); });
#line 521 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) {
#line 522 "src/codegen/gpu/emit.tr"
            TrStr nc = GpuEmitter_fresh(self);
#line 522 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9047 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (nc))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid.", 44LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9047); _tr_str_release(_at_t9047); });
#line 523 "src/codegen/gpu/emit.tr"
            TrStr gs = GpuEmitter_fresh(self);
#line 523 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9048 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (gs))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = mul i32 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nc)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (nt)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9048); _tr_str_release(_at_t9048); });
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
        ({ TrStr _at_t9049 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (tid))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call i32 @llvm.nvvm.read.ptx.sreg.tid.", 41LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (axis)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("()\n", 3LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9049); _tr_str_release(_at_t9049); });
#line 526 "src/codegen/gpu/emit.tr"
        TrStr g = GpuEmitter_fresh(self);
#line 526 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9050 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (g))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = add i32 ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (m)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (tid)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9050); _tr_str_release(_at_t9050); });
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
        TrStr _strtmp_t9051 = _tr_str_lit_len("@_Z12get_local_idj", 18LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t9051;
    }
#line 531 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_group_id", 12LL)))) {
#line 531 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t9052 = _tr_str_lit_len("@_Z12get_group_idj", 18LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t9052;
    }
#line 532 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_local_size", 14LL)))) {
#line 532 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t9053 = _tr_str_lit_len("@_Z14get_local_sizej", 20LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t9053;
    }
#line 533 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_global_size", 15LL)))) {
#line 533 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t9054 = _tr_str_lit_len("@_Z15get_global_sizej", 21LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t9054;
    }
#line 534 "src/codegen/gpu/emit.tr"
    if (_tr_str_eqv((nm), (_tr_str_lit_len("gpu_num_groups", 14LL)))) {
#line 534 "src/codegen/gpu/emit.tr"
        TrStr _strtmp_t9055 = _tr_str_lit_len("@_Z14get_num_groupsj", 20LL);
        _tr_str_release(mangled);
        mangled = _strtmp_t9055;
    }
#line 535 "src/codegen/gpu/emit.tr"
    TrStr sr = GpuEmitter_fresh(self);
#line 536 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t9056 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (sr))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = call spir_func i64 ", 22LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (mangled)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("(i32 ", 5LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(dim)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(")\n", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9056); _tr_str_release(_at_t9056); });
#line 537 "src/codegen/gpu/emit.tr"
    _tr_str_release(mangled);
    return GVal_make(_tr_str_lit_len("i64", 3LL), sr);
}

__attribute__((hot)) GVal* GpuEmitter_i32_to_i64(GpuEmitter* self, TrStr v) {
#line 540 "src/codegen/gpu/emit.tr"
    TrStr r = GpuEmitter_fresh(self);
#line 541 "src/codegen/gpu/emit.tr"
    ({ TrStr _at_t9057 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sext i32 ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to i64\n", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9057); _tr_str_release(_at_t9057); });
#line 542 "src/codegen/gpu/emit.tr"
    return GVal_make(_tr_str_lit_len("i64", 3LL), r);
}

__attribute__((hot)) GVal* GpuEmitter_emit_binop(GpuEmitter* self, TrStr op, HirExpr* lp, HirExpr* rp) {
#line 546 "src/codegen/gpu/emit.tr"
    GVal* l = GpuEmitter_emit_expr(self, lp);
#line 547 "src/codegen/gpu/emit.tr"
    GVal* r = GpuEmitter_emit_expr(self, rp);
#line 548 "src/codegen/gpu/emit.tr"
    TrStr ct = ({ TrStr _apt_t9058 = _tr_str_retain(l->ty); TrStr _apt_t9059 = _tr_str_retain(r->ty); __auto_type _wr = (GpuEmitter_common_ty(self, _apt_t9058, _apt_t9059)); _tr_str_release(_apt_t9058); _tr_str_release(_apt_t9059); _wr; });
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
            ({ TrStr _at_t9060 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fadd ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9060); _tr_str_release(_at_t9060); });
        } else {
#line 555 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9061 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = add ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9061); _tr_str_release(_at_t9061); });
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
            ({ TrStr _at_t9062 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fsub ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9062); _tr_str_release(_at_t9062); });
        } else {
#line 559 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9063 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sub ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9063); _tr_str_release(_at_t9063); });
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
            ({ TrStr _at_t9064 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fmul ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9064); _tr_str_release(_at_t9064); });
        } else {
#line 563 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9065 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = mul ", 7LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9065); _tr_str_release(_at_t9065); });
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
            ({ TrStr _at_t9066 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fdiv ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9066); _tr_str_release(_at_t9066); });
        } else {
#line 567 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9067 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sdiv ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9067); _tr_str_release(_at_t9067); });
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
            ({ TrStr _at_t9068 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = frem ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9068); _tr_str_release(_at_t9068); });
        } else {
#line 571 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9069 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = srem ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9069); _tr_str_release(_at_t9069); });
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
        ({ TrStr _at_t9070 = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("unsupported operator '", 22LL)), (op))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("' in kernel", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_fail(self, _at_t9070); _tr_str_release(_at_t9070); });
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
        ({ TrStr _at_t9071 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fcmp ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pred)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9071); _tr_str_release(_at_t9071); });
    } else {
#line 579 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9072 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (res))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = icmp ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (pred)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ct)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (la)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", ", 2LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (ra)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9072); _tr_str_release(_at_t9072); });
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
    if (({ __auto_type _ord_t9073 = (_gpu_iwidth(a)); __auto_type _wr = ((_ord_t9073 >= _gpu_iwidth(b))); _wr; })) {
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
    bool sf = ({ TrStr _apt_t9074 = _tr_str_retain(v->ty); __auto_type _wr = (_gpu_is_float(_apt_t9074)); _tr_str_release(_apt_t9074); _wr; });
#line 619 "src/codegen/gpu/emit.tr"
    bool tf = _gpu_is_float(target);
#line 620 "src/codegen/gpu/emit.tr"
    if ((sf && tf)) {
#line 621 "src/codegen/gpu/emit.tr"
        if (_tr_str_eqv((target), (_tr_str_lit_len("double", 6LL)))) {
#line 621 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9075 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fpext float ", 15LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to double\n", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9075); _tr_str_release(_at_t9075); });
        } else {
#line 622 "src/codegen/gpu/emit.tr"
            ({ TrStr _at_t9076 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fptrunc double ", 18LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to float\n", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9076); _tr_str_release(_at_t9076); });
        }
#line 623 "src/codegen/gpu/emit.tr"
        TrStr _trret2013 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret2013;
    }
#line 624 "src/codegen/gpu/emit.tr"
    if ((tf && (!sf))) {
#line 625 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9077 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sitofp ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9077); _tr_str_release(_at_t9077); });
#line 626 "src/codegen/gpu/emit.tr"
        TrStr _trret2014 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret2014;
    }
#line 627 "src/codegen/gpu/emit.tr"
    if ((sf && (!tf))) {
#line 628 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9078 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fptosi ", 10LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9078); _tr_str_release(_at_t9078); });
#line 629 "src/codegen/gpu/emit.tr"
        TrStr _trret2015 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret2015;
    }
#line 630 "src/codegen/gpu/emit.tr"
    long long sw = ({ TrStr _apt_t9079 = _tr_str_retain(v->ty); __auto_type _wr = (_gpu_iwidth(_apt_t9079)); _tr_str_release(_apt_t9079); _wr; });
#line 631 "src/codegen/gpu/emit.tr"
    long long tw = _gpu_iwidth(target);
#line 632 "src/codegen/gpu/emit.tr"
    if ((tw > sw)) {
#line 633 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9080 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = sext ", 8LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9080); _tr_str_release(_at_t9080); });
#line 634 "src/codegen/gpu/emit.tr"
        TrStr _trret2016 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret2016;
    }
#line 635 "src/codegen/gpu/emit.tr"
    if ((tw < sw)) {
#line 636 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9081 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = trunc ", 9LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" to ", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (target)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("\n", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9081); _tr_str_release(_at_t9081); });
#line 637 "src/codegen/gpu/emit.tr"
        TrStr _trret2017 = _tr_str_retain(r);
        _tr_str_release(r);
        return _trret2017;
    }
#line 638 "src/codegen/gpu/emit.tr"
    TrStr _trret2018 = _tr_str_retain(v->val);
    _tr_str_release(r);
    return _trret2018;
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
    if (({ TrStr _apt_t9082 = _tr_str_retain(v->ty); __auto_type _wr = (_gpu_is_float(_apt_t9082)); _tr_str_release(_apt_t9082); _wr; })) {
#line 644 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9083 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = fcmp one ", 12LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", 0.0\n", 6LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9083); _tr_str_release(_at_t9083); });
    } else {
#line 645 "src/codegen/gpu/emit.tr"
        ({ TrStr _at_t9084 = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((_tr_str_lit_len("  ", 2LL)), (r))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" = icmp ne ", 11LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->ty)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(" ", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (v->val)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len(", 0\n", 4LL))); _tr_str_release(_tr_cl); _tr_cres; })); GpuEmitter_w(self, _at_t9084); _tr_str_release(_at_t9084); });
    }
#line 646 "src/codegen/gpu/emit.tr"
    TrStr _trret2019 = _tr_str_retain(r);
    _tr_str_release(r);
    return _trret2019;
}

__attribute__((hot)) long long GpuEmitter_literal_int(GpuEmitter* self, HirExpr e) {
#line 650 "src/codegen/gpu/emit.tr"
    __auto_type _t9085 = e;
    if (_t9085.tag == HirExpr_ELitInt) {
        __auto_type val = _t9085.data.ELitInt.val;
__auto_type ty = _t9085.data.ELitInt.ty;
        long long _trret2020 = val;
        return _trret2020;
    } else if (1) {
        __auto_type _ = _t9085;
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
        long long nib = ((v >> (((4LL * k)) & 63)) & 15LL);
#line 677 "src/codegen/gpu/emit.tr"
        StringBuilder_append_char(sb, _hexdig(nib));
#line 678 "src/codegen/gpu/emit.tr"
        k = (k - 1LL);
    }
#line 679 "src/codegen/gpu/emit.tr"
    return StringObj_as_str(StringBuilder_to_string(sb));
}

