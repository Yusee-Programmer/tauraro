#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) LBlock* LBlock_init() {
    /* pass */
    LBlock* b = ((LBlock*)_tr_obj_alloc(sizeof(LBlock)));
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
    LBlock* _cltmp_t2237 = LBlock_init();
    _tr_obj_release(f->block, _trdrop_LBlock);
    f->block = _cltmp_t2237;
    /* pass */
    f->is_main = false;
    /* pass */
    f->n_vregs = 0LL;
    /* pass */
    f->vars = (void*)List_TrStr_new();
    /* pass */
    return f;
}

__attribute__((hot)) long long LFunc_new_vreg(LFunc* self) {
    /* pass */
    long long id = self->n_vregs;
    /* pass */
    self->n_vregs = (self->n_vregs + 1LL);
    /* pass */
    return id;
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

__attribute__((hot)) void LFunc_emit(LFunc* self, LInst i) {
    /* pass */
    List_ptr_append(self->block->insts, box_linst(i));
}

__attribute__((malloc,returns_nonnull,hot)) LModule* LModule_init() {
    /* pass */
    LModule* m = ((LModule*)_tr_obj_alloc(sizeof(LModule)));
    /* pass */
    m->funcs = (void*)List_ptr_new();
    /* pass */
    m->externs = (void*)List_TrStr_new();
    /* pass */
    m->ok = true;
    /* pass */
    return m;
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

