#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) LBlock* LBlock_init(long long id) {
    /* pass */
    LBlock* b = ((LBlock*)_tr_obj_alloc(sizeof(LBlock)));
    /* pass */
    b->id = id;
    /* pass */
    b->insts = (void*)List_ptr_new();
    /* pass */
    b->term = LTerm_make_TUnset();
    /* pass */
    return b;
}

__attribute__((malloc,returns_nonnull,hot)) LFunc* LFunc_init(TrStr name) {
    /* pass */
    LFunc* f = ((LFunc*)_tr_obj_alloc(sizeof(LFunc)));
    /* pass */
    f->name = _tr_str_retain(name);
    /* pass */
    f->is_main = false;
    /* pass */
    f->blocks = (void*)List_ptr_new();
    /* pass */
    f->cur = (-1LL);
    /* pass */
    f->n_vregs = 0LL;
    /* pass */
    f->vreg_types = (void*)List_i64_new();
    /* pass */
    f->vars = (void*)List_TrStr_new();
    /* pass */
    f->var_types = (void*)List_i64_new();
    /* pass */
    f->var_cls = (void*)List_TrStr_new();
    /* pass */
    f->params = (void*)List_TrStr_new();
    /* pass */
    f->tmp_ctr = 0LL;
    /* pass */
    f->loop_cont = (void*)List_i64_new();
    /* pass */
    f->loop_brk = (void*)List_i64_new();
    /* pass */
    f->fresh_strs = (void*)List_i64_new();
    /* pass */
    return f;
}

__attribute__((hot)) long long LFunc_fresh_id(LFunc* self) {
    /* pass */
    long long id = self->tmp_ctr;
    /* pass */
    self->tmp_ctr = (self->tmp_ctr + 1LL);
    /* pass */
    return id;
}

__attribute__((hot)) long long LFunc_new_block(LFunc* self) {
    /* pass */
    long long id = self->blocks->len;
    /* pass */
    List_ptr_append(self->blocks, LBlock_init(id));
    /* pass */
    return id;
}

__attribute__((hot)) void LFunc_set_cur(LFunc* self, long long id) {
    /* pass */
    self->cur = id;
}

__attribute__((hot)) void LFunc_emit(LFunc* self, LInst i) {
    /* pass */
    List_ptr_append(((LBlock*)List_ptr_get(self->blocks, self->cur))->insts, box_linst(i));
}

__attribute__((hot)) void LFunc_set_term(LFunc* self, LTerm t) {
    /* pass */
    LBlock* b = ((LBlock*)List_ptr_get(self->blocks, self->cur));
    /* pass */
    __auto_type _t2221 = b->term;
    if (_t2221.tag == LTerm_TUnset) {
        b->term = t;
    } else if (1) {
        __auto_type _ = _t2221;
        /* pass */
    }
}

__attribute__((hot)) bool LFunc_cur_terminated(LFunc* self) {
    /* pass */
    __auto_type _t2222 = ((LBlock*)List_ptr_get(self->blocks, self->cur))->term;
    if (_t2222.tag == LTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t2222;
        return true;
    }
}

__attribute__((hot)) long long LFunc_new_vreg(LFunc* self) {
    /* pass */
    long long id = self->n_vregs;
    /* pass */
    self->n_vregs = (self->n_vregs + 1LL);
    /* pass */
    List_i64_append(self->vreg_types, 0LL);
    /* pass */
    return id;
}

__attribute__((hot)) void LFunc_set_vreg_type(LFunc* self, long long id, long long t) {
    /* pass */
    if (((id >= 0LL) && (id < self->vreg_types->len))) {
        /* pass */
        List_i64_set(self->vreg_types, id, t);
    }
}

__attribute__((hot)) long long LFunc_vreg_type(LFunc* self, long long id) {
    /* pass */
    if (((id >= 0LL) && (id < self->vreg_types->len))) {
        /* pass */
        return List_i64_get(self->vreg_types, id);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) void LFunc_add_var(LFunc* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->vars->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->vars, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->vars, name);
    /* pass */
    List_i64_append(self->var_types, 0LL);
    /* pass */
    List_TrStr_append(self->var_cls, _tr_str_lit(""));
}

__attribute__((hot)) long long LFunc_var_index(LFunc* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->vars->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->vars, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) void LFunc_set_var_type(LFunc* self, TrStr name, long long t) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        List_i64_set(self->var_types, idx, t);
    }
}

__attribute__((hot)) long long LFunc_var_type(LFunc* self, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_i64_get(self->var_types, idx);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) void LFunc_set_var_cls(LFunc* self, TrStr name, TrStr cls) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        List_TrStr_set(self->var_cls, idx, cls);
    }
}

__attribute__((hot)) TrStr LFunc_var_cls_of(LFunc* self, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_TrStr_get(self->var_cls, idx);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((malloc,returns_nonnull,hot)) ClassLayout* ClassLayout_init(TrStr name) {
    /* pass */
    ClassLayout* c = ((ClassLayout*)_tr_obj_alloc(sizeof(ClassLayout)));
    /* pass */
    c->name = _tr_str_retain(name);
    /* pass */
    c->fields = (void*)List_TrStr_new();
    /* pass */
    c->ftags = (void*)List_i64_new();
    /* pass */
    return c;
}

__attribute__((hot)) long long ClassLayout_field_index(ClassLayout* self, TrStr fname) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fields->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fields, i)), _tr_strz(fname)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((malloc,returns_nonnull,hot)) LModule* LModule_init() {
    /* pass */
    LModule* m = ((LModule*)_tr_obj_alloc(sizeof(LModule)));
    /* pass */
    m->funcs = (void*)List_ptr_new();
    /* pass */
    m->classes = (void*)List_ptr_new();
    /* pass */
    m->externs = (void*)List_TrStr_new();
    /* pass */
    m->fn_names = (void*)List_TrStr_new();
    /* pass */
    m->fn_ret = (void*)List_i64_new();
    /* pass */
    m->strings = (void*)List_TrStr_new();
    /* pass */
    m->globals = (void*)List_TrStr_new();
    /* pass */
    m->global_types = (void*)List_i64_new();
    /* pass */
    m->global_inits = (void*)List_ptr_new();
    /* pass */
    m->ok = true;
    /* pass */
    return m;
}

__attribute__((hot)) long long LModule_add_global(LModule* self, TrStr name, long long tag) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->globals->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->globals, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->globals, name);
    /* pass */
    List_i64_append(self->global_types, tag);
    /* pass */
    return (self->globals->len - 1LL);
}

__attribute__((hot)) long long LModule_global_index(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->globals->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->globals, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_global(LModule* self, TrStr name) {
    /* pass */
    return (LModule_global_index(self, name) >= 0LL);
}

__attribute__((hot)) long long LModule_global_type(LModule* self, TrStr name) {
    /* pass */
    long long idx = LModule_global_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_i64_get(self->global_types, idx);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long LModule_fn_ret_tag(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fn_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fn_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return List_i64_get(self->fn_ret, i);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long LModule_add_string(LModule* self, TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->strings->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->strings, i)), _tr_strz(s)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->strings, s);
    /* pass */
    return (self->strings->len - 1LL);
}

__attribute__((hot)) void LModule_add_extern(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->externs->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->externs, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->externs, name);
}

__attribute__((hot)) bool LModule_is_user_fn(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fn_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fn_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void LModule_add_class(LModule* self, ClassLayout* cl) {
    /* pass */
    List_ptr_append(self->classes, _tr_obj_retain(cl));
}

__attribute__((hot)) long long LModule_class_index(LModule* self, TrStr name) {
    /* pass */
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->classes->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((ClassLayout*)List_ptr_get(self->classes, i))->name), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_class(LModule* self, TrStr name) {
    /* pass */
    return (LModule_class_index(self, name) >= 0LL);
}

__attribute__((hot)) long long LModule_class_size(LModule* self, TrStr name) {
    /* pass */
    long long ci = LModule_class_index(self, name);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    long long n = ((ClassLayout*)List_ptr_get(self->classes, ci))->fields->len;
    /* pass */
    if ((n < 1LL)) {
        /* pass */
        n = 1LL;
    }
    /* pass */
    return (n * 8LL);
}

__attribute__((hot)) long long LModule_field_offset(LModule* self, TrStr cls, TrStr fld) {
    /* pass */
    long long ci = LModule_class_index(self, cls);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
    /* pass */
    if ((fi < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    return (fi * 8LL);
}

__attribute__((hot)) long long LModule_field_tag(LModule* self, TrStr cls, TrStr fld) {
    /* pass */
    long long ci = LModule_class_index(self, cls);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
    /* pass */
    if ((fi < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    return List_i64_get(((ClassLayout*)List_ptr_get(self->classes, ci))->ftags, fi);
}

__attribute__((hot)) LInst* box_linst(LInst i) {
    /* pass */
    /* unsafe block */
    /* pass */
    LInst* p = ((LInst*)_tr_c_calloc((size_t)(1LL), sizeof(LInst)));
    /* pass */
    (*p = i);
    /* pass */
    return p;
}

