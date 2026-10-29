#include "tauraro_types.h"

bool _ir_bytes_eq(TrStr a, TrStr b, long long n);

__attribute__((malloc,returns_nonnull,hot)) LBlock* LBlock_init(long long id) {
#line 83 "src/taumir/ir.tr"
    LBlock* b = ((LBlock*)_tr_obj_alloc(sizeof(LBlock)));
#line 84 "src/taumir/ir.tr"
    b->id = id;
#line 85 "src/taumir/ir.tr"
    b->insts = (void*)List_ptr_new();
#line 86 "src/taumir/ir.tr"
    b->term = LTerm_make_TUnset();
#line 87 "src/taumir/ir.tr"
    return b;
}

__attribute__((malloc,returns_nonnull,hot)) LFunc* LFunc_init(TrStr name) {
#line 123 "src/taumir/ir.tr"
    LFunc* f = ((LFunc*)_tr_obj_alloc(sizeof(LFunc)));
#line 124 "src/taumir/ir.tr"
    f->name = _tr_str_retain(name);
#line 125 "src/taumir/ir.tr"
    f->is_main = false;
#line 126 "src/taumir/ir.tr"
    f->blocks = (void*)List_ptr_new();
#line 127 "src/taumir/ir.tr"
    f->cur = (-1LL);
#line 128 "src/taumir/ir.tr"
    f->n_vregs = 0LL;
#line 129 "src/taumir/ir.tr"
    f->vreg_types = (void*)List_i64_new();
#line 130 "src/taumir/ir.tr"
    f->vars = (void*)List_TrStr_new();
#line 131 "src/taumir/ir.tr"
    f->var_types = (void*)List_i64_new();
#line 132 "src/taumir/ir.tr"
    f->var_cls = (void*)List_TrStr_new();
#line 133 "src/taumir/ir.tr"
    f->var_arr = (void*)List_i64_new();
#line 134 "src/taumir/ir.tr"
    f->params = (void*)List_TrStr_new();
#line 135 "src/taumir/ir.tr"
    f->tmp_ctr = 0LL;
#line 136 "src/taumir/ir.tr"
    f->loop_cont = (void*)List_i64_new();
#line 137 "src/taumir/ir.tr"
    f->loop_brk = (void*)List_i64_new();
#line 138 "src/taumir/ir.tr"
    f->fresh_strs = (void*)List_i64_new();
#line 139 "src/taumir/ir.tr"
    f->fresh_objs = (void*)List_i64_new();
#line 140 "src/taumir/ir.tr"
    f->captures = (void*)List_TrStr_new();
#line 141 "src/taumir/ir.tr"
    f->cap_tags = (void*)List_i64_new();
#line 142 "src/taumir/ir.tr"
    f->var_xret = (void*)List_i64_new();
#line 143 "src/taumir/ir.tr"
    f->vreg_xret = (void*)List_i64_new();
#line 144 "src/taumir/ir.tr"
    f->try_blks = (void*)List_i64_new();
#line 145 "src/taumir/ir.tr"
    f->try_msgs = (void*)List_TrStr_new();
#line 146 "src/taumir/ir.tr"
    f->defers = (void*)List_ptr_new();
#line 147 "src/taumir/ir.tr"
    f->mutex_unlocks = (void*)List_TrStr_new();
#line 148 "src/taumir/ir.tr"
    f->blk_depth = 0LL;
#line 149 "src/taumir/ir.tr"
    f->in_defer = false;
#line 150 "src/taumir/ir.tr"
    f->is_throws = false;
#line 151 "src/taumir/ir.tr"
    f->throws_ok_tag = 0LL;
#line 152 "src/taumir/ir.tr"
    f->throws_err_tag = 0LL;
#line 153 "src/taumir/ir.tr"
    f->in_taskgroup = 0LL;
#line 154 "src/taumir/ir.tr"
    return f;
}

__attribute__((hot)) long long LFunc_fresh_id(LFunc* self) {
#line 159 "src/taumir/ir.tr"
    long long id = self->tmp_ctr;
#line 160 "src/taumir/ir.tr"
    self->tmp_ctr = (self->tmp_ctr + 1LL);
#line 161 "src/taumir/ir.tr"
    return id;
}

__attribute__((hot)) long long LFunc_new_block(LFunc* self) {
#line 165 "src/taumir/ir.tr"
    long long id = self->blocks->len;
#line 166 "src/taumir/ir.tr"
    ({ LBlock* _ord_t6544 = (LBlock_init(id)); List_ptr_append(self->blocks, _tr_obj_retain(_ord_t6544)); _tr_obj_release(_ord_t6544, _trdrop_LBlock); });
#line 167 "src/taumir/ir.tr"
    return id;
}

__attribute__((hot)) void LFunc_set_cur(LFunc* self, long long id) {
#line 170 "src/taumir/ir.tr"
    self->cur = id;
}

__attribute__((hot)) void LFunc_emit(LFunc* self, LInst i) {
#line 173 "src/taumir/ir.tr"
    ({ __auto_type _ord_t6545 = (box_linst(i)); List_ptr_append(((LBlock*)List_ptr_get(self->blocks, self->cur))->insts, _ord_t6545); });
}

__attribute__((hot)) void LFunc_set_term(LFunc* self, LTerm t) {
#line 179 "src/taumir/ir.tr"
    LBlock* b = ((LBlock*)List_ptr_get(self->blocks, self->cur));
#line 180 "src/taumir/ir.tr"
    __auto_type _t6546 = b->term;
    if (_t6546.tag == LTerm_TUnset) {
        b->term = t;
    } else if (1) {
        __auto_type _ = _t6546;
        /* pass */
    }
}

__attribute__((hot)) bool LFunc_cur_terminated(LFunc* self) {
#line 185 "src/taumir/ir.tr"
    __auto_type _t6547 = ((LBlock*)List_ptr_get(self->blocks, self->cur))->term;
    if (_t6547.tag == LTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t6547;
        return true;
    }
}

__attribute__((hot)) long long LFunc_new_vreg(LFunc* self) {
#line 190 "src/taumir/ir.tr"
    long long id = self->n_vregs;
#line 191 "src/taumir/ir.tr"
    self->n_vregs = (self->n_vregs + 1LL);
#line 192 "src/taumir/ir.tr"
    List_i64_append(self->vreg_types, 0LL);
#line 193 "src/taumir/ir.tr"
    List_i64_append(self->vreg_xret, (-1LL));
#line 194 "src/taumir/ir.tr"
    return id;
}

__attribute__((hot)) void LFunc_set_vreg_type(LFunc* self, long long id, long long t) {
#line 197 "src/taumir/ir.tr"
    if (((id >= 0LL) && (id < self->vreg_types->len))) {
#line 197 "src/taumir/ir.tr"
        List_i64_set(self->vreg_types, id, t);
    }
}

__attribute__((hot)) long long LFunc_vreg_type(LFunc* self, long long id) {
#line 200 "src/taumir/ir.tr"
    if (((id >= 0LL) && (id < self->vreg_types->len))) {
#line 200 "src/taumir/ir.tr"
        return List_i64_get(self->vreg_types, id);
    }
#line 201 "src/taumir/ir.tr"
    return 0LL;
}

__attribute__((hot)) void LFunc_add_var(LFunc* self, TrStr name) {
#line 204 "src/taumir/ir.tr"
    long long i = 0LL;
#line 205 "src/taumir/ir.tr"
    while ((i < self->vars->len)) {
#line 206 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6548 = (List_TrStr_get(self->vars, i)); TrStr _cmpb_t6549 = (name); _Bool _cmpr_t6550 = _tr_str_eqv(_cmpa_t6548, _cmpb_t6549); _tr_str_release(_cmpa_t6548); _cmpr_t6550; })) {
#line 206 "src/taumir/ir.tr"
            return;
        }
#line 207 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 208 "src/taumir/ir.tr"
    List_TrStr_append(self->vars, name);
#line 209 "src/taumir/ir.tr"
    List_i64_append(self->var_types, 0LL);
#line 210 "src/taumir/ir.tr"
    List_TrStr_append(self->var_cls, _tr_str_lit_len("", 0LL));
#line 211 "src/taumir/ir.tr"
    List_i64_append(self->var_arr, 0LL);
#line 212 "src/taumir/ir.tr"
    List_i64_append(self->var_xret, (-1LL));
}

__attribute__((hot)) long long LFunc_var_index(LFunc* self, TrStr name) {
#line 215 "src/taumir/ir.tr"
    long long i = 0LL;
#line 216 "src/taumir/ir.tr"
    while ((i < self->vars->len)) {
#line 217 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6551 = (List_TrStr_get(self->vars, i)); TrStr _cmpb_t6552 = (name); _Bool _cmpr_t6553 = _tr_str_eqv(_cmpa_t6551, _cmpb_t6552); _tr_str_release(_cmpa_t6551); _cmpr_t6553; })) {
#line 217 "src/taumir/ir.tr"
            return i;
        }
#line 218 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 219 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((hot)) void LFunc_set_var_type(LFunc* self, TrStr name, long long t) {
#line 222 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 223 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 223 "src/taumir/ir.tr"
        List_i64_set(self->var_types, idx, t);
    }
}

__attribute__((hot)) long long LFunc_var_type(LFunc* self, TrStr name) {
#line 226 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 227 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 227 "src/taumir/ir.tr"
        return List_i64_get(self->var_types, idx);
    }
#line 228 "src/taumir/ir.tr"
    return 0LL;
}

__attribute__((hot)) void LFunc_set_var_cls(LFunc* self, TrStr name, TrStr cls) {
#line 231 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 232 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 232 "src/taumir/ir.tr"
        List_TrStr_set(self->var_cls, idx, cls);
    }
}

__attribute__((hot)) TrStr LFunc_var_cls_of(LFunc* self, TrStr name) {
#line 235 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 236 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 236 "src/taumir/ir.tr"
        return List_TrStr_get(self->var_cls, idx);
    }
#line 237 "src/taumir/ir.tr"
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) void LFunc_set_var_arr(LFunc* self, TrStr name, long long n) {
#line 243 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 244 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 244 "src/taumir/ir.tr"
        List_i64_set(self->var_arr, idx, n);
    }
}

__attribute__((hot)) long long LFunc_var_arr_of(LFunc* self, TrStr name) {
#line 247 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 248 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 248 "src/taumir/ir.tr"
        return List_i64_get(self->var_arr, idx);
    }
#line 249 "src/taumir/ir.tr"
    return 0LL;
}

__attribute__((hot)) void LFunc_set_vreg_xret(LFunc* self, long long id, long long t) {
#line 254 "src/taumir/ir.tr"
    if (((id >= 0LL) && (id < self->vreg_xret->len))) {
#line 254 "src/taumir/ir.tr"
        List_i64_set(self->vreg_xret, id, t);
    }
}

__attribute__((hot)) long long LFunc_vreg_xret_of(LFunc* self, long long id) {
#line 257 "src/taumir/ir.tr"
    if (((id >= 0LL) && (id < self->vreg_xret->len))) {
#line 257 "src/taumir/ir.tr"
        return List_i64_get(self->vreg_xret, id);
    }
#line 258 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((hot)) void LFunc_set_var_xret(LFunc* self, TrStr name, long long t) {
#line 261 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 262 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 262 "src/taumir/ir.tr"
        List_i64_set(self->var_xret, idx, t);
    }
}

__attribute__((hot)) long long LFunc_var_xret_of(LFunc* self, TrStr name) {
#line 265 "src/taumir/ir.tr"
    long long idx = LFunc_var_index(self, name);
#line 266 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 266 "src/taumir/ir.tr"
        return List_i64_get(self->var_xret, idx);
    }
#line 267 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((hot)) long long LFunc_capture_index(LFunc* self, TrStr name) {
#line 270 "src/taumir/ir.tr"
    long long i = 0LL;
#line 271 "src/taumir/ir.tr"
    while ((i < self->captures->len)) {
#line 272 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6554 = (List_TrStr_get(self->captures, i)); TrStr _cmpb_t6555 = (name); _Bool _cmpr_t6556 = _tr_str_eqv(_cmpa_t6554, _cmpb_t6555); _tr_str_release(_cmpa_t6554); _cmpr_t6556; })) {
#line 272 "src/taumir/ir.tr"
            return i;
        }
#line 273 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 274 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((malloc,returns_nonnull,hot)) ClassLayout* ClassLayout_init(TrStr name) {
#line 289 "src/taumir/ir.tr"
    ClassLayout* c = ((ClassLayout*)_tr_obj_alloc(sizeof(ClassLayout)));
#line 290 "src/taumir/ir.tr"
    c->name = _tr_str_retain(name);
#line 291 "src/taumir/ir.tr"
    c->base = _tr_str_lit_len("", 0LL);
#line 292 "src/taumir/ir.tr"
    c->fields = (void*)List_TrStr_new();
#line 293 "src/taumir/ir.tr"
    c->ftags = (void*)List_i64_new();
#line 294 "src/taumir/ir.tr"
    c->fcls = (void*)List_TrStr_new();
#line 295 "src/taumir/ir.tr"
    c->is_value = false;
#line 296 "src/taumir/ir.tr"
    return c;
}

__attribute__((hot)) long long ClassLayout_field_index(ClassLayout* self, TrStr fname) {
#line 299 "src/taumir/ir.tr"
    long long i = 0LL;
#line 300 "src/taumir/ir.tr"
    while ((i < self->fields->len)) {
#line 301 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6557 = (List_TrStr_get(self->fields, i)); TrStr _cmpb_t6558 = (fname); _Bool _cmpr_t6559 = _tr_str_eqv(_cmpa_t6557, _cmpb_t6558); _tr_str_release(_cmpa_t6557); _cmpr_t6559; })) {
#line 301 "src/taumir/ir.tr"
            return i;
        }
#line 302 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 303 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((malloc,returns_nonnull,hot)) VariantLayout* VariantLayout_init(TrStr name) {
#line 315 "src/taumir/ir.tr"
    VariantLayout* v = ((VariantLayout*)_tr_obj_alloc(sizeof(VariantLayout)));
#line 316 "src/taumir/ir.tr"
    v->name = _tr_str_retain(name);
#line 317 "src/taumir/ir.tr"
    v->fields = (void*)List_TrStr_new();
#line 318 "src/taumir/ir.tr"
    v->ftags = (void*)List_i64_new();
#line 319 "src/taumir/ir.tr"
    v->fcls = (void*)List_TrStr_new();
#line 320 "src/taumir/ir.tr"
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) EnumLayout* EnumLayout_init(TrStr name) {
#line 329 "src/taumir/ir.tr"
    EnumLayout* e = ((EnumLayout*)_tr_obj_alloc(sizeof(EnumLayout)));
#line 330 "src/taumir/ir.tr"
    e->name = _tr_str_retain(name);
#line 331 "src/taumir/ir.tr"
    e->variants = (void*)List_ptr_new();
#line 332 "src/taumir/ir.tr"
    return e;
}

__attribute__((hot)) long long EnumLayout_variant_index(EnumLayout* self, TrStr vname) {
#line 335 "src/taumir/ir.tr"
    long long i = 0LL;
#line 336 "src/taumir/ir.tr"
    while ((i < self->variants->len)) {
#line 337 "src/taumir/ir.tr"
        if (_tr_str_eqv((((VariantLayout*)List_ptr_get(self->variants, i))->name), (vname))) {
#line 337 "src/taumir/ir.tr"
            return i;
        }
#line 338 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 339 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((malloc,returns_nonnull,hot)) LModule* LModule_init() {
#line 379 "src/taumir/ir.tr"
    LModule* m = ((LModule*)_tr_obj_alloc(sizeof(LModule)));
#line 380 "src/taumir/ir.tr"
    m->funcs = (void*)List_ptr_new();
#line 381 "src/taumir/ir.tr"
    m->classes = (void*)List_ptr_new();
#line 382 "src/taumir/ir.tr"
    m->enums = (void*)List_ptr_new();
#line 383 "src/taumir/ir.tr"
    m->ifaces = (void*)List_TrStr_new();
#line 384 "src/taumir/ir.tr"
    m->externs = (void*)List_TrStr_new();
#line 385 "src/taumir/ir.tr"
    m->fn_names = (void*)List_TrStr_new();
#line 386 "src/taumir/ir.tr"
    m->fn_ret = (void*)List_i64_new();
#line 387 "src/taumir/ir.tr"
    m->strings = (void*)List_TrStr_new();
#line 388 "src/taumir/ir.tr"
    m->string_lens = (void*)List_i64_new();
#line 389 "src/taumir/ir.tr"
    m->globals = (void*)List_TrStr_new();
#line 390 "src/taumir/ir.tr"
    m->global_types = (void*)List_i64_new();
#line 391 "src/taumir/ir.tr"
    m->global_arr = (void*)List_i64_new();
#line 392 "src/taumir/ir.tr"
    m->global_inits = (void*)List_ptr_new();
#line 393 "src/taumir/ir.tr"
    m->ok = true;
#line 394 "src/taumir/ir.tr"
    m->fail_note = _tr_str_lit_len("", 0LL);
#line 395 "src/taumir/ir.tr"
    m->unavail_names = (void*)List_TrStr_new();
#line 396 "src/taumir/ir.tr"
    m->unavail_notes = (void*)List_TrStr_new();
#line 397 "src/taumir/ir.tr"
    m->subst_names = (void*)List_TrStr_new();
#line 398 "src/taumir/ir.tr"
    m->subst_tys = (void*)List_ptr_new();
#line 399 "src/taumir/ir.tr"
    m->extfn_names = (void*)List_TrStr_new();
#line 400 "src/taumir/ir.tr"
    m->extfn_ret = (void*)List_i64_new();
#line 401 "src/taumir/ir.tr"
    m->fn_owned_names = (void*)List_TrStr_new();
#line 402 "src/taumir/ir.tr"
    m->overloaded_sigs = (void*)List_TrStr_new();
#line 403 "src/taumir/ir.tr"
    m->target_llvm = false;
#line 404 "src/taumir/ir.tr"
    m->no_heap = false;
#line 405 "src/taumir/ir.tr"
    return m;
}

__attribute__((hot)) void LModule_mark_overloaded(LModule* self, TrStr cls, TrStr method) {
#line 411 "src/taumir/ir.tr"
    TrStr key = ({ TrStr _tr_cl = (_tr_strx_concatv((cls), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; });
#line 412 "src/taumir/ir.tr"
    if (LModule_is_overloaded(self, cls, method)) {
#line 412 "src/taumir/ir.tr"
        _tr_str_release(key);
        return;
    }
#line 413 "src/taumir/ir.tr"
    List_TrStr_append(self->overloaded_sigs, key);
    _tr_str_release(key);
}

__attribute__((hot)) bool LModule_is_overloaded(LModule* self, TrStr cls, TrStr method) {
#line 416 "src/taumir/ir.tr"
    TrStr key = ({ TrStr _tr_cl = (_tr_strx_concatv((cls), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; });
#line 417 "src/taumir/ir.tr"
    long long i = 0LL;
#line 418 "src/taumir/ir.tr"
    while ((i < self->overloaded_sigs->len)) {
#line 419 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6560 = (List_TrStr_get(self->overloaded_sigs, i)); TrStr _cmpb_t6561 = (key); _Bool _cmpr_t6562 = _tr_str_eqv(_cmpa_t6560, _cmpb_t6561); _tr_str_release(_cmpa_t6560); _cmpr_t6562; })) {
#line 419 "src/taumir/ir.tr"
            _tr_str_release(key);
            return true;
        }
#line 420 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 421 "src/taumir/ir.tr"
    _tr_str_release(key);
    return false;
}

__attribute__((hot)) TrStr LModule_resolve_method_ov(LModule* self, TrStr cls, TrStr method, long long argc) {
#line 426 "src/taumir/ir.tr"
    TrStr cur = _tr_str_retain(cls);
#line 427 "src/taumir/ir.tr"
    long long depth = 0LL;
#line 428 "src/taumir/ir.tr"
    while ((depth < 32LL)) {
#line 429 "src/taumir/ir.tr"
        long long ci = LModule_class_index(self, cur);
#line 430 "src/taumir/ir.tr"
        if ((ci < 0LL)) {
#line 430 "src/taumir/ir.tr"
            _tr_str_release(cur);
            return _tr_str_lit_len("", 0LL);
        }
#line 431 "src/taumir/ir.tr"
        if (LModule_is_overloaded(self, cur, method)) {
#line 432 "src/taumir/ir.tr"
            TrStr mm = ({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (({ TrStr _tr_cl = (_tr_strx_concatv((cur), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("_", 1LL))); _tr_str_release(_tr_cl); _tr_cres; })); TrStr _tr_cr = (_tr_str_wrap(_tr_int_to_str((long long)(argc)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, _tr_cr); _tr_str_release(_tr_cl); _tr_str_release(_tr_cr); _tr_cres; })); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (_tr_str_lit_len("arg", 3LL))); _tr_str_release(_tr_cl); _tr_cres; });
#line 433 "src/taumir/ir.tr"
            if (LModule_is_user_fn(self, mm)) {
#line 433 "src/taumir/ir.tr"
                TrStr _trret1552 = _tr_str_retain(mm);
                _tr_str_release(cur);
                _tr_str_release(mm);
                return _trret1552;
            }
        }
#line 434 "src/taumir/ir.tr"
        TrStr mangled = ({ TrStr _tr_cl = (_tr_strx_concatv((cur), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; });
#line 435 "src/taumir/ir.tr"
        if (LModule_is_user_fn(self, mangled)) {
#line 435 "src/taumir/ir.tr"
            TrStr _trret1553 = _tr_str_retain(mangled);
            _tr_str_release(cur);
            _tr_str_release(mangled);
            return _trret1553;
        }
#line 436 "src/taumir/ir.tr"
        TrStr _strtmp_t6563 = _tr_str_retain(((ClassLayout*)List_ptr_get(self->classes, ci))->base);
        _tr_str_release(cur);
        cur = _strtmp_t6563;
#line 437 "src/taumir/ir.tr"
        if (((((unsigned long long)(((char*)(_tr_strz(cur))))) == ((unsigned long long)(0LL))) || _tr_str_eqv((cur), (_tr_str_lit_len("", 0LL))))) {
#line 437 "src/taumir/ir.tr"
            _tr_str_release(cur);
            _tr_str_release(mangled);
            return _tr_str_lit_len("", 0LL);
        }
#line 438 "src/taumir/ir.tr"
        depth = (depth + 1LL);
        _tr_str_release(mangled);
    }
#line 439 "src/taumir/ir.tr"
    _tr_str_release(cur);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) long long LModule_add_global(LModule* self, TrStr name, long long tag) {
#line 443 "src/taumir/ir.tr"
    long long i = 0LL;
#line 444 "src/taumir/ir.tr"
    while ((i < self->globals->len)) {
#line 445 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6564 = (List_TrStr_get(self->globals, i)); TrStr _cmpb_t6565 = (name); _Bool _cmpr_t6566 = _tr_str_eqv(_cmpa_t6564, _cmpb_t6565); _tr_str_release(_cmpa_t6564); _cmpr_t6566; })) {
#line 445 "src/taumir/ir.tr"
            return i;
        }
#line 446 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 447 "src/taumir/ir.tr"
    List_TrStr_append(self->globals, name);
#line 448 "src/taumir/ir.tr"
    List_i64_append(self->global_types, tag);
#line 449 "src/taumir/ir.tr"
    List_i64_append(self->global_arr, 0LL);
#line 450 "src/taumir/ir.tr"
    return (self->globals->len - 1LL);
}

__attribute__((hot)) void LModule_set_global_arr(LModule* self, long long idx, long long n) {
#line 455 "src/taumir/ir.tr"
    if (((idx >= 0LL) && (idx < self->global_arr->len))) {
#line 455 "src/taumir/ir.tr"
        List_i64_set(self->global_arr, idx, n);
    }
}

__attribute__((hot)) long long LModule_global_arr_of(LModule* self, TrStr name) {
#line 458 "src/taumir/ir.tr"
    long long idx = LModule_global_index(self, name);
#line 459 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 459 "src/taumir/ir.tr"
        return List_i64_get(self->global_arr, idx);
    }
#line 460 "src/taumir/ir.tr"
    return 0LL;
}

__attribute__((hot)) long long LModule_global_index(LModule* self, TrStr name) {
#line 463 "src/taumir/ir.tr"
    long long i = 0LL;
#line 464 "src/taumir/ir.tr"
    while ((i < self->globals->len)) {
#line 465 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6567 = (List_TrStr_get(self->globals, i)); TrStr _cmpb_t6568 = (name); _Bool _cmpr_t6569 = _tr_str_eqv(_cmpa_t6567, _cmpb_t6568); _tr_str_release(_cmpa_t6567); _cmpr_t6569; })) {
#line 465 "src/taumir/ir.tr"
            return i;
        }
#line 466 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 467 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_global(LModule* self, TrStr name) {
#line 470 "src/taumir/ir.tr"
    return (LModule_global_index(self, name) >= 0LL);
}

__attribute__((hot)) long long LModule_global_type(LModule* self, TrStr name) {
#line 473 "src/taumir/ir.tr"
    long long idx = LModule_global_index(self, name);
#line 474 "src/taumir/ir.tr"
    if ((idx >= 0LL)) {
#line 474 "src/taumir/ir.tr"
        return List_i64_get(self->global_types, idx);
    }
#line 475 "src/taumir/ir.tr"
    return 0LL;
}

__attribute__((hot)) long long LModule_fn_ret_tag(LModule* self, TrStr name) {
#line 479 "src/taumir/ir.tr"
    long long i = 0LL;
#line 480 "src/taumir/ir.tr"
    while ((i < self->fn_names->len)) {
#line 481 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6570 = (List_TrStr_get(self->fn_names, i)); TrStr _cmpb_t6571 = (name); _Bool _cmpr_t6572 = _tr_str_eqv(_cmpa_t6570, _cmpb_t6571); _tr_str_release(_cmpa_t6570); _cmpr_t6572; })) {
#line 481 "src/taumir/ir.tr"
            return List_i64_get(self->fn_ret, i);
        }
#line 482 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 483 "src/taumir/ir.tr"
    return 0LL;
}

__attribute__((hot)) long long LModule_add_string(LModule* self, TrStr s, long long blen) {
#line 493 "src/taumir/ir.tr"
    long long i = 0LL;
#line 494 "src/taumir/ir.tr"
    while ((i < self->strings->len)) {
#line 495 "src/taumir/ir.tr"
        if (((List_i64_get(self->string_lens, i) == blen) && ({ TrStr _at_t6573 = (List_TrStr_get(self->strings, i)); __auto_type _wr = (_ir_bytes_eq(_at_t6573, s, blen)); _tr_str_release(_at_t6573); _wr; }))) {
#line 496 "src/taumir/ir.tr"
            return i;
        }
#line 497 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 498 "src/taumir/ir.tr"
    List_TrStr_append(self->strings, s);
#line 499 "src/taumir/ir.tr"
    List_i64_append(self->string_lens, blen);
#line 500 "src/taumir/ir.tr"
    return (self->strings->len - 1LL);
}

__attribute__((hot)) void LModule_add_extern(LModule* self, TrStr name) {
#line 503 "src/taumir/ir.tr"
    long long i = 0LL;
#line 504 "src/taumir/ir.tr"
    while ((i < self->externs->len)) {
#line 505 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6574 = (List_TrStr_get(self->externs, i)); TrStr _cmpb_t6575 = (name); _Bool _cmpr_t6576 = _tr_str_eqv(_cmpa_t6574, _cmpb_t6575); _tr_str_release(_cmpa_t6574); _cmpr_t6576; })) {
#line 505 "src/taumir/ir.tr"
            return;
        }
#line 506 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 507 "src/taumir/ir.tr"
    List_TrStr_append(self->externs, name);
}

__attribute__((hot)) bool LModule_is_user_fn(LModule* self, TrStr name) {
#line 510 "src/taumir/ir.tr"
    long long i = 0LL;
#line 511 "src/taumir/ir.tr"
    while ((i < self->fn_names->len)) {
#line 512 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6577 = (List_TrStr_get(self->fn_names, i)); TrStr _cmpb_t6578 = (name); _Bool _cmpr_t6579 = _tr_str_eqv(_cmpa_t6577, _cmpb_t6578); _tr_str_release(_cmpa_t6577); _cmpr_t6579; })) {
#line 512 "src/taumir/ir.tr"
            return true;
        }
#line 513 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 514 "src/taumir/ir.tr"
    return false;
}

__attribute__((hot)) bool LModule_is_extern_fn(LModule* self, TrStr name) {
#line 517 "src/taumir/ir.tr"
    long long i = 0LL;
#line 518 "src/taumir/ir.tr"
    while ((i < self->extfn_names->len)) {
#line 519 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6580 = (List_TrStr_get(self->extfn_names, i)); TrStr _cmpb_t6581 = (name); _Bool _cmpr_t6582 = _tr_str_eqv(_cmpa_t6580, _cmpb_t6581); _tr_str_release(_cmpa_t6580); _cmpr_t6582; })) {
#line 519 "src/taumir/ir.tr"
            return true;
        }
#line 520 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 521 "src/taumir/ir.tr"
    return false;
}

__attribute__((hot)) long long LModule_extern_ret_tag(LModule* self, TrStr name) {
#line 524 "src/taumir/ir.tr"
    long long i = 0LL;
#line 525 "src/taumir/ir.tr"
    while ((i < self->extfn_names->len)) {
#line 526 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6583 = (List_TrStr_get(self->extfn_names, i)); TrStr _cmpb_t6584 = (name); _Bool _cmpr_t6585 = _tr_str_eqv(_cmpa_t6583, _cmpb_t6584); _tr_str_release(_cmpa_t6583); _cmpr_t6585; })) {
#line 526 "src/taumir/ir.tr"
            return List_i64_get(self->extfn_ret, i);
        }
#line 527 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 528 "src/taumir/ir.tr"
    return 0LL;
}

__attribute__((hot)) void LModule_mark_fn_owned(LModule* self, TrStr name) {
#line 532 "src/taumir/ir.tr"
    long long i = 0LL;
#line 533 "src/taumir/ir.tr"
    while ((i < self->fn_owned_names->len)) {
#line 534 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6586 = (List_TrStr_get(self->fn_owned_names, i)); TrStr _cmpb_t6587 = (name); _Bool _cmpr_t6588 = _tr_str_eqv(_cmpa_t6586, _cmpb_t6587); _tr_str_release(_cmpa_t6586); _cmpr_t6588; })) {
#line 534 "src/taumir/ir.tr"
            return;
        }
#line 535 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 536 "src/taumir/ir.tr"
    List_TrStr_append(self->fn_owned_names, name);
}

__attribute__((hot)) bool LModule_fn_ret_owned(LModule* self, TrStr name) {
#line 541 "src/taumir/ir.tr"
    long long i = 0LL;
#line 542 "src/taumir/ir.tr"
    while ((i < self->fn_owned_names->len)) {
#line 543 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6589 = (List_TrStr_get(self->fn_owned_names, i)); TrStr _cmpb_t6590 = (name); _Bool _cmpr_t6591 = _tr_str_eqv(_cmpa_t6589, _cmpb_t6590); _tr_str_release(_cmpa_t6589); _cmpr_t6591; })) {
#line 543 "src/taumir/ir.tr"
            return true;
        }
#line 544 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 545 "src/taumir/ir.tr"
    return false;
}

__attribute__((hot)) long long LModule_unavail_index(LModule* self, TrStr name) {
#line 549 "src/taumir/ir.tr"
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
#line 549 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 550 "src/taumir/ir.tr"
    long long i = 0LL;
#line 551 "src/taumir/ir.tr"
    while ((i < self->unavail_names->len)) {
#line 552 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6592 = (List_TrStr_get(self->unavail_names, i)); TrStr _cmpb_t6593 = (name); _Bool _cmpr_t6594 = _tr_str_eqv(_cmpa_t6592, _cmpb_t6593); _tr_str_release(_cmpa_t6592); _cmpr_t6594; })) {
#line 552 "src/taumir/ir.tr"
            return i;
        }
#line 553 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 554 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((hot)) void LModule_add_class(LModule* self, ClassLayout* cl) {
#line 558 "src/taumir/ir.tr"
    List_ptr_append(self->classes, _tr_obj_retain(cl));
}

__attribute__((hot)) long long LModule_class_index(LModule* self, TrStr name) {
#line 561 "src/taumir/ir.tr"
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
#line 561 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 562 "src/taumir/ir.tr"
    long long i = 0LL;
#line 563 "src/taumir/ir.tr"
    while ((i < self->classes->len)) {
#line 564 "src/taumir/ir.tr"
        if (_tr_str_eqv((((ClassLayout*)List_ptr_get(self->classes, i))->name), (name))) {
#line 564 "src/taumir/ir.tr"
            return i;
        }
#line 565 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 566 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_class(LModule* self, TrStr name) {
#line 569 "src/taumir/ir.tr"
    return (LModule_class_index(self, name) >= 0LL);
}

__attribute__((hot)) bool LModule_is_value_class(LModule* self, TrStr name) {
#line 573 "src/taumir/ir.tr"
    long long ci = LModule_class_index(self, name);
#line 574 "src/taumir/ir.tr"
    if ((ci < 0LL)) {
#line 574 "src/taumir/ir.tr"
        return false;
    }
#line 575 "src/taumir/ir.tr"
    return ((ClassLayout*)List_ptr_get(self->classes, ci))->is_value;
}

__attribute__((hot)) long long LModule_class_size(LModule* self, TrStr name) {
#line 579 "src/taumir/ir.tr"
    long long ci = LModule_class_index(self, name);
#line 580 "src/taumir/ir.tr"
    if ((ci < 0LL)) {
#line 580 "src/taumir/ir.tr"
        return 8LL;
    }
#line 581 "src/taumir/ir.tr"
    long long n = ((ClassLayout*)List_ptr_get(self->classes, ci))->fields->len;
#line 582 "src/taumir/ir.tr"
    if ((n < 1LL)) {
#line 582 "src/taumir/ir.tr"
        n = 1LL;
    }
#line 583 "src/taumir/ir.tr"
    return (n * 8LL);
}

__attribute__((hot)) long long LModule_field_offset(LModule* self, TrStr cls, TrStr fld) {
#line 587 "src/taumir/ir.tr"
    long long ci = LModule_class_index(self, cls);
#line 588 "src/taumir/ir.tr"
    if ((ci < 0LL)) {
#line 588 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 589 "src/taumir/ir.tr"
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
#line 590 "src/taumir/ir.tr"
    if ((fi < 0LL)) {
#line 590 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 591 "src/taumir/ir.tr"
    return (fi * 8LL);
}

__attribute__((hot)) long long LModule_field_tag(LModule* self, TrStr cls, TrStr fld) {
#line 595 "src/taumir/ir.tr"
    long long ci = LModule_class_index(self, cls);
#line 596 "src/taumir/ir.tr"
    if ((ci < 0LL)) {
#line 596 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 597 "src/taumir/ir.tr"
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
#line 598 "src/taumir/ir.tr"
    if ((fi < 0LL)) {
#line 598 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 599 "src/taumir/ir.tr"
    return List_i64_get(((ClassLayout*)List_ptr_get(self->classes, ci))->ftags, fi);
}

__attribute__((hot)) TrStr LModule_resolve_method(LModule* self, TrStr cls, TrStr method) {
#line 605 "src/taumir/ir.tr"
    TrStr cur = _tr_str_retain(cls);
#line 606 "src/taumir/ir.tr"
    long long depth = 0LL;
#line 607 "src/taumir/ir.tr"
    while ((depth < 32LL)) {
#line 608 "src/taumir/ir.tr"
        long long ci = LModule_class_index(self, cur);
#line 609 "src/taumir/ir.tr"
        if ((ci < 0LL)) {
#line 609 "src/taumir/ir.tr"
            _tr_str_release(cur);
            return _tr_str_lit_len("", 0LL);
        }
#line 610 "src/taumir/ir.tr"
        TrStr mangled = ({ TrStr _tr_cl = (_tr_strx_concatv((cur), (_tr_str_lit_len("_", 1LL)))); TrStr _tr_cres = _tr_strx_concatv(_tr_cl, (method)); _tr_str_release(_tr_cl); _tr_cres; });
#line 611 "src/taumir/ir.tr"
        if (LModule_is_user_fn(self, mangled)) {
#line 611 "src/taumir/ir.tr"
            TrStr _trret1554 = _tr_str_retain(mangled);
            _tr_str_release(cur);
            _tr_str_release(mangled);
            return _trret1554;
        }
#line 612 "src/taumir/ir.tr"
        TrStr _strtmp_t6595 = _tr_str_retain(((ClassLayout*)List_ptr_get(self->classes, ci))->base);
        _tr_str_release(cur);
        cur = _strtmp_t6595;
#line 613 "src/taumir/ir.tr"
        if (((((unsigned long long)(((char*)(_tr_strz(cur))))) == ((unsigned long long)(0LL))) || _tr_str_eqv((cur), (_tr_str_lit_len("", 0LL))))) {
#line 613 "src/taumir/ir.tr"
            _tr_str_release(cur);
            _tr_str_release(mangled);
            return _tr_str_lit_len("", 0LL);
        }
#line 614 "src/taumir/ir.tr"
        depth = (depth + 1LL);
        _tr_str_release(mangled);
    }
#line 615 "src/taumir/ir.tr"
    _tr_str_release(cur);
    return _tr_str_lit_len("", 0LL);
}

__attribute__((hot)) TrStr LModule_field_cls(LModule* self, TrStr cls, TrStr fld) {
#line 619 "src/taumir/ir.tr"
    long long ci = LModule_class_index(self, cls);
#line 620 "src/taumir/ir.tr"
    if ((ci < 0LL)) {
#line 620 "src/taumir/ir.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 621 "src/taumir/ir.tr"
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
#line 622 "src/taumir/ir.tr"
    if ((fi < 0LL)) {
#line 622 "src/taumir/ir.tr"
        return _tr_str_lit_len("", 0LL);
    }
#line 623 "src/taumir/ir.tr"
    return List_TrStr_get(((ClassLayout*)List_ptr_get(self->classes, ci))->fcls, fi);
}

__attribute__((hot)) void LModule_add_enum(LModule* self, EnumLayout* el) {
#line 627 "src/taumir/ir.tr"
    List_ptr_append(self->enums, _tr_obj_retain(el));
}

__attribute__((hot)) long long LModule_enum_index(LModule* self, TrStr name) {
#line 630 "src/taumir/ir.tr"
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
#line 630 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 631 "src/taumir/ir.tr"
    long long i = 0LL;
#line 632 "src/taumir/ir.tr"
    while ((i < self->enums->len)) {
#line 633 "src/taumir/ir.tr"
        if (_tr_str_eqv((((EnumLayout*)List_ptr_get(self->enums, i))->name), (name))) {
#line 633 "src/taumir/ir.tr"
            return i;
        }
#line 634 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 635 "src/taumir/ir.tr"
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_enum(LModule* self, TrStr name) {
#line 638 "src/taumir/ir.tr"
    return (LModule_enum_index(self, name) >= 0LL);
}

__attribute__((hot)) long long LModule_enum_size(LModule* self, TrStr name) {
#line 642 "src/taumir/ir.tr"
    long long ei = LModule_enum_index(self, name);
#line 643 "src/taumir/ir.tr"
    if ((ei < 0LL)) {
#line 643 "src/taumir/ir.tr"
        return 8LL;
    }
#line 644 "src/taumir/ir.tr"
    long long mx = 0LL;
#line 645 "src/taumir/ir.tr"
    long long vi = 0LL;
#line 646 "src/taumir/ir.tr"
    while ((vi < ((EnumLayout*)List_ptr_get(self->enums, ei))->variants->len)) {
#line 647 "src/taumir/ir.tr"
        long long n = ((VariantLayout*)List_ptr_get(((EnumLayout*)List_ptr_get(self->enums, ei))->variants, vi))->fields->len;
#line 648 "src/taumir/ir.tr"
        if ((n > mx)) {
#line 648 "src/taumir/ir.tr"
            mx = n;
        }
#line 649 "src/taumir/ir.tr"
        vi = (vi + 1LL);
    }
#line 650 "src/taumir/ir.tr"
    return ((1LL + mx) * 8LL);
}

__attribute__((hot)) long long LModule_enum_variant_index(LModule* self, TrStr ename, TrStr vname) {
#line 653 "src/taumir/ir.tr"
    long long ei = LModule_enum_index(self, ename);
#line 654 "src/taumir/ir.tr"
    if ((ei < 0LL)) {
#line 654 "src/taumir/ir.tr"
        return (-1LL);
    }
#line 655 "src/taumir/ir.tr"
    return EnumLayout_variant_index(((EnumLayout*)List_ptr_get(self->enums, ei)), vname);
}

__attribute__((hot)) void LModule_add_iface(LModule* self, TrStr name) {
#line 659 "src/taumir/ir.tr"
    List_TrStr_append(self->ifaces, name);
}

__attribute__((hot)) bool LModule_is_iface(LModule* self, TrStr name) {
#line 662 "src/taumir/ir.tr"
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
#line 662 "src/taumir/ir.tr"
        return false;
    }
#line 663 "src/taumir/ir.tr"
    long long i = 0LL;
#line 664 "src/taumir/ir.tr"
    while ((i < self->ifaces->len)) {
#line 665 "src/taumir/ir.tr"
        if (({ TrStr _cmpa_t6596 = (List_TrStr_get(self->ifaces, i)); TrStr _cmpb_t6597 = (name); _Bool _cmpr_t6598 = _tr_str_eqv(_cmpa_t6596, _cmpb_t6597); _tr_str_release(_cmpa_t6596); _cmpr_t6598; })) {
#line 665 "src/taumir/ir.tr"
            return true;
        }
#line 666 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 667 "src/taumir/ir.tr"
    return false;
}

__attribute__((hot)) LInst* box_linst(LInst i) {
#line 60 "src/taumir/ir.tr"
    /* unsafe block */
#line 61 "src/taumir/ir.tr"
    LInst* p = ((LInst*)_tr_c_calloc((size_t)(1LL), sizeof(LInst)));
#line 62 "src/taumir/ir.tr"
    (*p = i);
#line 63 "src/taumir/ir.tr"
    return p;
}

__attribute__((hot)) bool _ir_bytes_eq(TrStr a, TrStr b, long long n) {
#line 68 "src/taumir/ir.tr"
    char* pa = ((char*)(_tr_strz(a)));
#line 69 "src/taumir/ir.tr"
    char* pb = ((char*)(_tr_strz(b)));
#line 70 "src/taumir/ir.tr"
    long long i = 0LL;
#line 71 "src/taumir/ir.tr"
    while ((i < n)) {
#line 72 "src/taumir/ir.tr"
        if (({ __auto_type _ord_t6599 = ((*(pa + i))); __auto_type _wr = ((_ord_t6599 != (*(pb + i)))); _wr; })) {
#line 72 "src/taumir/ir.tr"
            return false;
        }
#line 73 "src/taumir/ir.tr"
        i = (i + 1LL);
    }
#line 74 "src/taumir/ir.tr"
    return true;
}

